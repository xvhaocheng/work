#!/usr/bin/env python3
"""Report bounded paired effects from already verified configuration reductions."""
import csv
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent


def main():
    summary = json.loads((HERE / 'summary.json').read_text())
    cases = summary['cases']
    effects = []
    stage_rows = []
    for mode in ('p', 'pn'):
        baseline = cases[f'v0-{mode}-r8']
        for variant in ('v1', 'v2'):
            changed = cases[f'{variant}-{mode}-r8']
            btime, ctime = baseline['times']['T_return'], changed['times']['T_return']
            stages = {name: value - baseline['complete_serial_stages']['components'][name]
                      for name, value in changed['complete_serial_stages']['components'].items()}
            bptw = sum(lane['union_cycles']['ptw_active'] for lane in baseline['timeline'].values())
            cptw = sum(lane['union_cycles']['ptw_active'] for lane in changed['timeline'].values())
            record = {'variant': variant, 'mode': mode, 'ring': 8, 'baseline_T_return': btime,
                      'T_return': ctime, 'saved_steps': btime - ctime,
                      'reduction_percent': 100 * (btime - ctime) / btime,
                      'slow_lane': changed['complete_serial_stages']['lane'],
                      'serial_milestone_deltas_new_minus_V0': stages,
                      'baseline_PTW_active_steps': bptw, 'PTW_active_steps': cptw,
                      'PTW_delta_context_only': cptw - bptw}
            if sum(stages.values()) != ctime - btime:
                raise RuntimeError('serial stage deltas do not sum to observed complete difference')
            effects.append(record)
            stage_rows.append({'variant': variant, 'mode': mode, 'ring': 8,
                               'T_return_delta': ctime - btime, **stages})
    (HERE / 'effects.json').write_text(json.dumps({
        'paired_R8_effects': effects,
        'PN_minus_P_return_gap': {variant: cases[f'{variant}-pn-r8']['times']['T_return'] - cases[f'{variant}-p-r8']['times']['T_return']
                                 for variant in ('v0', 'v1', 'v2')},
        'interpretation': 'Observed whole-workload replacement-policy intervention. The serial stage deltas sum to observed complete differences; PTW active is an overlapping context measure and is not added to stage savings.'
    }, indent=2, ensure_ascii=False) + '\n')
    with (HERE / 'stage-comparison.csv').open('w', newline='') as stream:
        writer = csv.DictWriter(stream, fieldnames=list(stage_rows[0]))
        writer.writeheader()
        writer.writerows(stage_rows)
    marker = '\n<!-- paired-effects -->\n'
    findings = (HERE / 'findings.md').read_text().split(marker)[0]
    lines = ['', 'R8的直接观察：V0有39次填充，其中30次淘汰有效项；淘汰条目仅way2/way3各15次。A固定源在way8，目的页0/2/4/6各8次遍历，1/3/5/7各1次；最终有效位0x01ff，获配但未使用的way9..15共7个。', '',
             'V1改成分区紧凑树后为25次遍历，仍有15次有效淘汰（均way2）；仅目的页0/4各8次，其他目的页各1次。14叶树仍非完全平衡，不能把V1称为精确LRU。V2为11次冷遍历、零有效淘汰，A全部8个目的页及固定源各1次。', '']
    for effect in effects:
        stages = effect['serial_milestone_deltas_new_minus_V0']
        lines.append(f'{effect["mode"].upper()}下{effect["variant"].upper()}比V0完整返回少{effect["saved_steps"]}步（{effect["reduction_percent"]:.2f}%）。慢设备V的source complete→last B减少{-stages["source_complete_to_last_b"]}步，同时backend→first R增加{stages["backend_to_first_r"]}步；first R→source complete及last B→DONE均不变，其余小差项见stage-comparison.csv。单PTW真实active为{effect["baseline_PTW_active_steps"]}→{effect["PTW_active_steps"]}步，这些区间与payload重叠，不能另加到完整时间收益中。')
    lines += ['', 'R1与R16的三个变体遍历数及完整返回均相同；改善限于本轮R8可容纳足迹。R8启用独立命中读通路相对仅分区的完整返回差仍为V0 16280、V1 9900、V2 3674步，精确LRU没有消除全部组织差异。', '',
              '18个配置及6个必要重复共24次运行全部通过原始父任务、数据、握手、翻译和victim逐笔核对。V0六个条件的完整返回、完整结果JSON、parents和events原字节均精确复现历史；六对R8重复也各自结果及parents/events字节完全一致，观察到确定性、无测得波动。']
    (HERE / 'findings.md').write_text(findings + marker + '\n'.join(lines) + '\n')
    print(json.dumps({'paired_effects': len(effects), 'stage_delta_sums_verified': True}))


if __name__ == '__main__':
    main()
