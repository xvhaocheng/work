#!/usr/bin/env python3
"""Export tables from accepted main-result JSON; never rerun simulation/reduction."""
import csv
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def write_csv(name, records):
    path = HERE / name
    with path.open('w', newline='') as stream:
        writer = csv.DictWriter(stream, fieldnames=list(records[0]))
        writer.writeheader()
        writer.writerows(records)
    return {'path': name, 'rows': len(records), 'sha256': sha(path)}


def main():
    summary_path = HERE / 'summary.json'
    summary = json.loads(summary_path.read_text())
    if summary.get('configuration_count') != 18 or not summary.get('all_attempts_valid'):
        raise RuntimeError('Export requires the accepted complete 18-configuration summary')
    translations, timelines, stages, inputs = [], [], [], []
    order = sorted(summary['comparison'], key=lambda row: (row['variant'], row['mode'], row['ring']))
    for main in order:
        name = f'{main["variant"]}-{main["mode"]}-r{main["ring"]}'
        selected = summary['cases'][name]
        detail_path = HERE / selected['detail']
        case = json.loads(detail_path.read_text())
        if not case['valid'] or case['attempt'] != main['main_attempt'] or case['attempt'] != selected['attempt']:
            raise RuntimeError(f'Main-result identity mismatch: {name}')
        inputs.append({'path': selected['detail'], 'sha256': sha(detail_path)})
        identity = {key: main[key] for key in ('variant', 'mode', 'ring')}
        requests = case['normal_requests'] + case['n_requests']
        lane_sids = {}
        for lane, device in ((0, 'V'), (1, 'A')):
            accepted = [request for request in requests if request['lane'] == lane]
            sids = {request['did'] for request in accepted}
            if len(sids) != 1:
                raise RuntimeError(f'Expected one actual DID/SID per device: {name}/{device}')
            sid = lane_sids[lane] = next(iter(sids))
            counts = selected['logical_counts_by_lane'][str(lane)]
            if counts['hit'] + counts['miss'] != len(accepted):
                raise RuntimeError(f'Accepted/hit/miss conservation mismatch: {name}/{device}')
            common = {**identity, 'device': device, 'lane': lane, 'sid': sid}
            translations.append({**common, 'accepted_requests': len(accepted),
                                 'hits': counts['hit'], 'misses': counts['miss'], 'walks': counts['walk'],
                                 'normal_accepted': sum(request['lane'] == lane for request in case['normal_requests']),
                                 'N_hit_accepted': sum(request['lane'] == lane for request in case['n_requests']),
                                 'main_attempt': main['main_attempt']})
            timeline = selected['timeline'][str(lane)]
            timelines.append({**common,
                              **{f'{key}_union_steps': value for key, value in timeline['union_cycles'].items()},
                              'PTW_overlap_payload_steps': timeline['PTW_overlap_payload_cycles'],
                              'translation_overlap_payload_steps': timeline['translation_overlap_payload_cycles'],
                              'main_attempt': main['main_attempt']})
        serial = selected['complete_serial_stages']
        slow_lane = serial['lane']
        components = serial['components']
        if sum(components.values()) != selected['times']['T_return']:
            raise RuntimeError(f'Accepted serial phase sum mismatch: {name}')
        stages.append({**identity, 'slow_device': 'V' if slow_lane == 0 else 'A',
                       'slow_lane': slow_lane, 'sid': lane_sids[slow_lane],
                       **components, 'stage_sum': serial['sum'], 'T_return': selected['times']['T_return'],
                       'main_attempt': main['main_attempt']})
    exports = [write_csv('device-translation.csv', translations),
               write_csv('device-timelines.csv', timelines), write_csv('complete-stages.csv', stages)]
    manifest = {'schema': 'plru-main-table-export-v1', 'summary': {'path': 'summary.json', 'sha256': sha(summary_path)},
                'main_case_inputs': inputs, 'exports': exports,
                'translation_semantics': 'Accepted logical requests only: normal first-lookup hit/miss plus accepted N-hit requests. Unaccepted N probes and post-fill rechecks are excluded. SID is the unique actual DID from accepted upstream requests.',
                'timeline_semantics': 'Half-open clock-step interval unions per device from t0. translation_pending=[request acceptance,response acceptance), excluding all pre-acceptance backpressure. PTW active follows state!=IDLE. payload_read=[DDR AR acceptance,RLAST acceptance+1); payload_write=[DDR AW acceptance,B acceptance+1): both include queueing/service and the final response edge, not pure bus busy time. completion_discovery=[hardware DONE,software exact-DONE poll end). These categories can overlap; neither columns nor different devices may be summed into total elapsed or recoverable time.',
                'stage_semantics': 'Serial parent milestones of the final slow device plus final DONE-to-quiet tail. Only these nonoverlapping columns sum exactly to T_return; their names identify event boundaries, not a pure causal allocation of all translation and memory delay.',
                'exporter_sha256': sha(Path(__file__)),
                'execution': 'Read/export of existing accepted JSON only; no simulation or event reduction performed.'}
    (HERE / 'table-exports.json').write_text(json.dumps(manifest, indent=2, ensure_ascii=False) + '\n')
    marker = '\n<!-- lookup-tables -->\n'
    findings_path = HERE / 'findings.md'
    findings = findings_path.read_text().split(marker)[0]
    note = ('\n汇总查数表（只导出现有18个主结果，不重新运行仿真或归约）：'
            '[逐设备命中/缺失/遍历](device-translation.csv)（36行）、'
            '[逐设备翻译、PTW、payload与完成发现时间并集](device-timelines.csv)（36行）、'
            '[每配置慢设备连续里程碑及最终尾段](complete-stages.csv)（18行）。'
            '字段口径及输入hash见[table-exports.json](table-exports.json)，可运行export_tables.py重新导出。\n\n'
            '命中表仅计实际接受的逻辑翻译：首次普通查找及接受的独立命中读；未接受探测与填充后重查不算新请求，'
            '每设备命中+缺失=接受数。SID来自这些实际请求的DID字段。'
            '翻译pending为[请求接受,响应接受)，不含接受前背压；PTW active按真实非IDLE状态计。'
            'payload读为[DDR AR接受,RLAST接受+1)，写为[DDR AW接受,B接受+1)，包含排队、服务和最后响应采样步，并非纯总线busy；'
            '完成发现为[硬件DONE,软件精确DONE轮询结束)。这些区间并集可重叠，跨列或跨设备都不能相加为完整时间或收益；'
            '只有慢设备非重叠里程碑表的各阶段加最终DONE→quiet尾段严格相加为完整返回。\n')
    findings_path.write_text(findings + marker + note)
    print(json.dumps({'exports': exports, 'simulation_or_reduction_rerun': False}))


if __name__ == '__main__':
    main()
