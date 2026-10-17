#!/usr/bin/env python3
"""Static reconstruction checks and recovery package; no RTL tools are run."""
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tarfile
from datetime import datetime, timezone

sys.dont_write_bytecode = True
from organize import OUT, REPO, ROOT, WT, PREFIX, UPSTREAM, HEAD, git, run, sha, save, old, replace, between

frozen = json.loads((OUT / 'frozen-input.json').read_text())
steps = json.loads((OUT / 'steps.json').read_text())
by_id = {s['feature_id']: s for s in steps}
end = steps[-1]['commit']
fixed = by_id['F5']['commit']
obs = by_id['O1']['commit']
TEMP = ROOT / 'out/source-organization-20261017/static-verification/iommu'
TEMP.mkdir(parents=True, exist_ok=True)

def index_env(name):
    p = TEMP / name
    if p.exists(): p.unlink()
    return dict(os.environ, GIT_INDEX_FILE=str(p))

def tree_entries(ref):
    result = {}
    for item in git('ls-tree', '-r', '-z', ref).decode().strip('\0').split('\0'):
        if not item: continue
        info, path = item.split('\t')
        mode, kind, oid = info.split()
        result[path] = {'mode': mode, 'object': oid, 'type': kind}
    return result

def normalized(text):
    return text.rstrip('\n') + '\n'

def observation_core():
    checked = []
    paths = ['rtl/rv_iommu_top.sv', 'rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv',
             'rtl/translation_logic/rv_iommu_iotlb.sv', 'rtl/translation_logic/rv_iommu_ptw.sv',
             'rtl/translation_logic/rv_iommu_tl_wrap.sv']
    for path in paths:
        text = old(obs, path)
        if path.endswith('rv_iommu_top.sv'):
            text = replace(text, '''    output logic [(RVIOMMUCfg.NumIntVec-1):0] wsi_wires_o,
    output rv_iommu::e3_ctl_t e3_ctl_o,
    output rv_iommu::e3_tl_t e3_tl_o,
    output rv_iommu::research_t research_o
''', '    output logic [(RVIOMMUCfg.NumIntVec-1):0] wsi_wires_o\n')
            text = replace(text, '        .in_flight_o        (in_flight),\n        .e3_o              (e3_ctl_o)\n', '        .in_flight_o        (in_flight)\n')
            text = replace(text, '            .fault_ready_i          (fault_ready),\n            .e3_o(e3_tl_o), .research_o\n', '            .fault_ready_i          (fault_ready)\n', 2)
        elif path.endswith('rv_iommu_trans_ctl_wrap.sv'):
            text = replace(text, '    output logic in_flight_o,\n    output rv_iommu::e3_ctl_t e3_o\n', '    output logic in_flight_o\n')
            text = between(text, '    logic [31:0] e3_waiting;\n', "    assign w_ready = (w_cnt_q != '0)")
        elif path.endswith('rv_iommu_iotlb.sv'):
            text = replace(text, '    output rv_iommu::iotlb_content_t        lu_content_o,\n    output rv_iommu::research_t            research_o\n', '    output rv_iommu::iotlb_content_t        lu_content_o\n')
            text = between(text, '    // Only read the actual pre-edge and next state. Snapshot readers must\n', '    // sequential process\n')
        elif path.endswith('rv_iommu_ptw.sv'):
            text = replace(text, '    output axi_req_t                    mem_req_o,\n    output logic [1:0]                  e3_level_o\n', '    output axi_req_t                    mem_req_o\n')
            text = replace(text, '    assign e3_level_o = s1_lvl_q;\n', '')
        else:
            text = replace(text, '    input  logic        fault_ready_i,\n    output rv_iommu::e3_tl_t e3_o,\n    output rv_iommu::research_t research_o\n', '    input  logic        fault_ready_i\n')
            text = replace(text, '    rv_iommu::research_t            iotlb_research;\n', '')
            text = replace(text, '        .lu_content_o   (iotlb_lu_content),\n        .research_o     (iotlb_research)\n', '        .lu_content_o   (iotlb_lu_content)\n')
            text = between(text, '    axi_req_t e3_ptw_req;\n', '    rv_iommu_ptw #(\n')
            text = replace(text, '        .active_o           (e3_ptw_active),\n        .e3_level_o         (e3_level),\n', '        .active_o           ( ),\n')
            text = replace(text, '        .mem_resp_i         (e3_ptw_resp),\n        .mem_req_o          (e3_ptw_req),\n', '        .mem_resp_i         (ptw_axi_resp_i),\n        .mem_req_o          (ptw_axi_req_o),\n')
        assert normalized(text) == normalized(old(fixed, path)), path
        checked.append({'path': path, 'result': 'exact functional source after removing passive declarations/taps and restoring direct wire names', 'normalized_trailing_newline': True})
    return checked

def evidence(feature, path, needle, context=4):
    s = by_id[feature]
    lines = old(s['commit'], path).splitlines()
    found = [i for i, line in enumerate(lines) if needle in line]
    assert found, (feature, path, needle)
    start, stop = max(0, found[0] - context), min(len(lines), found[0] + context + 1)
    return {'feature_id': feature, 'commit': s['commit'], 'path': path,
            'line_start': start+1, 'line_end': stop, 'exact_excerpt': '\n'.join(lines[start:stop])}

expected = {}
env = index_env('frozen-expected.index')
git('read-tree', HEAD, env=env)
with tarfile.open(OUT / 'frozen-source.tar.gz') as tar:
    for path, info in frozen['files'].items():
        if info['mode'] == '160000':
            expected[path] = {'mode': info['mode'], 'object': info['index_object'], 'type': 'commit'}
            continue
        member = tar.getmember(path)
        data = member.linkname.encode() if info['mode'] == '120000' else tar.extractfile(member).read()
        assert sha(data) == info['sha256']
        blob = git('hash-object', '-w', '--stdin', data=data).decode().strip()
        assert blob == info['frozen_blob']
        git('update-index', '--add', '--cacheinfo', f"{info['mode']},{blob},{path}", env=env)
        expected[path] = {'mode': info['mode'], 'object': blob, 'type': 'blob'}
frozen_tree = git('write-tree', env=env).decode().strip()
assert tree_entries(end) == expected
assert frozen_tree == steps[-1]['tree']

# Original checkout must still match every captured byte and every existing ref.
for path, info in frozen['files'].items():
    if info['mode'] == '160000': continue
    p = REPO / path
    data = os.readlink(p).encode() if info['mode'] == '120000' else p.read_bytes()
    assert sha(data) == info['sha256'], path
assert git('status', '--porcelain=v2').decode() == frozen['status_porcelain_v2']
assert git('rev-parse', 'HEAD').decode().strip() == frozen['head']
assert git('symbolic-ref', 'HEAD').decode().strip() == frozen['symbolic_head']
index = Path(git('rev-parse', '--absolute-git-dir').decode().strip()) / 'index'
assert sha(index.read_bytes()) == frozen['index_sha256']
current_refs = {line.split()[1]: line.split()[0] for line in git('show-ref').decode().splitlines()}
initial_refs = {line.split()[1]: line.split()[0] for line in frozen['refs'].splitlines()}
for name, oid in initial_refs.items(): assert current_refs[name] == oid
new_refs = set(current_refs) - set(initial_refs)
assert all(name.startswith('refs/heads/' + PREFIX) for name in new_refs)
assert git('remote', '-v').decode() == frozen['remote_output']
assert git('status', '--porcelain=v2', cwd=WT).decode() == ''

adapted_changed = git('diff', '--name-only', UPSTREAM, by_id['A1']['commit']).decode().splitlines()
assert adapted_changed == ['rtl/ext_interfaces/axi_iommu_intf.sv']
for path, item in tree_entries(fixed).items():
    if item['type'] != 'blob' or not (path.startswith(('rtl/', 'packages/')) or path == 'Bender.yml'): continue
    text = old(fixed, path)
    assert not re.search(r'E3Enable|BankedEnable|NonblockEnable|PolicyEnable|PartitionLocalHistory|rv_iommu_e3_|rv_iommu_banked_ctl|rv_iommu_nonblock_hit|read_admit_i|policy_partition_i', text), path
for path in ['rtl/rv_iommu_top.sv', 'rtl/translation_logic/rv_iommu_tl_wrap.sv']:
    text = old(obs, path)
    assert not re.search(r'E3Enable|e3_enable_i|e3_arm_i|e3_active_i|e3_waiting_i|e3_delay128_i|rv_iommu_e3_delay', text), path
assert 'rtl/translation_logic/rv_iommu_e3_delay.sv' not in tree_entries(obs)
core_results = observation_core()

patch_dir = OUT / 'patches'
patch_dir.mkdir(exist_ok=True)
patches = git('format-patch', '--no-signature', '--full-index', '--binary', '-o', str(patch_dir), f'{UPSTREAM}..{end}').decode().splitlines()
assert len(patches) == len(steps)
env = index_env('patch-reconstruction.index')
git('read-tree', UPSTREAM, env=env)
patch_results = []
for step, name in zip(steps, patches):
    data = Path(name).read_bytes()
    git('apply', '--cached', '--check', data=data, env=env)
    git('apply', '--cached', data=data, env=env)
    tree = git('write-tree', env=env).decode().strip()
    assert tree == step['tree'], step['feature_id']
    step['patch'] = str(Path(name).relative_to(OUT))
    step['patch_sha256'] = sha(data)
    patch_results.append({'feature_id': step['feature_id'], 'commit': step['commit'], 'tree': tree, 'matches_commit_tree': True})
(OUT / 'series').write_text('\n'.join(step['patch'] for step in steps) + '\n')

bundle = OUT / 'iommu-new-series.bundle'
if bundle.exists(): bundle.unlink()
refs = [step['ref'] for step in steps] + ['refs/heads/' + PREFIX + 'series']
git('bundle', 'create', str(bundle), *refs, '^' + UPSTREAM)
bundle_report = run('git', 'bundle', 'verify', str(bundle), cwd=REPO).decode()
recovery = TEMP / 'bundle-recovery.git'
assert not recovery.exists(), 'Use a fresh recovery directory instead of replacing previous recovery evidence'
run('git', 'init', '--bare', str(recovery), cwd=TEMP)
run('git', '-C', str(recovery), 'fetch', '--no-tags', '--no-recurse-submodules', str(REPO), UPSTREAM + ':refs/heads/prerequisite', cwd=TEMP)
run('git', '-C', str(recovery), 'bundle', 'verify', str(bundle), cwd=TEMP)
run('git', '-C', str(recovery), 'fetch', '--no-tags', str(bundle), *[ref+':'+ref for ref in refs], cwd=TEMP)
for step in steps:
    oid = run('git', '-C', str(recovery), 'rev-parse', step['ref'], cwd=TEMP).decode().strip()
    assert oid == step['commit']
    assert run('git', '-C', str(recovery), 'rev-parse', oid+'^{tree}', cwd=TEMP).decode().strip() == step['tree']

evidence_rows = [
    evidence('A1', 'rtl/ext_interfaces/axi_iommu_intf.sv', 'aw_mmu_flow'),
    evidence('F1', 'rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv', 'Forward AW before AR', 10),
    evidence('F2', 'rtl/software_interface/rv_iommu_hpm.sv', "edged_event_n = 1'b1"),
    evidence('F3', 'rtl/ext_interfaces/rv_iommu_ds_if.sv', 'assign aw_ready ='),
    evidence('F3', 'rtl/ext_interfaces/rv_iommu_ds_if.sv', 'Packed index must match', 8),
    evidence('F4', 'rtl/translation_logic/rv_iommu_ptw.sv', 'LVL3 is a valid', 7),
    evidence('F5', 'rtl/transaction_controller/rv_iommu_axi_bc.sv', 'assign beat_bytes', 8),
    evidence('O1', 'rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv', 'logic [31:0] e3_waiting', 18),
    evidence('O1', 'rtl/translation_logic/rv_iommu_tl_wrap.sv', 'Observation projection', 10),
    evidence('E1', 'rtl/translation_logic/rv_iommu_tl_wrap.sv', 'if (E3Enable)', 11),
    evidence('B1', 'rtl/transaction_controller/rv_iommu_banked_ctl.sv', 'assign restrict_reads', 15),
    evidence('B1', 'rtl/translation_logic/rv_iommu_tl_wrap.sv', 'assign trans_resp_data_o.bank'),
    evidence('N1', 'rtl/translation_logic/rv_iommu_nonblock_hit.sv', 'assign accept_o', 13),
    evidence('P1', 'rtl/translation_logic/rv_iommu_tl_wrap.sv', '.policy_mask_i', 5),
    evidence('P2', 'rtl/translation_logic/rv_iommu_iotlb.sv', 'assign partition_local_active', 8),
    evidence('P2', 'rtl/translation_logic/rv_iommu_iotlb.sv', 'Derive touch', 11),
]
save('critical-source-evidence.json', evidence_rows)
save('reconstruction-verification.json', {
    'timestamp_utc': datetime.now(timezone.utc).isoformat(), 'status': 'STATIC_RECONSTRUCTION_VERIFIED',
    'tests_or_experiments_run': [], 'build_or_simulation_verified': False,
    'frozen_tree': frozen_tree, 'endpoint_commit': end,
    'ordinary_source_files': sum(x['mode'] != '160000' for x in frozen['files'].values()),
    'gitlinks': sum(x['mode'] == '160000' for x in frozen['files'].values()),
    'untracked_source_files_included': [p for p, v in frozen['files'].items() if v.get('untracked')],
    'endpoint_bytes_modes_and_paths_match_freeze': True,
    'original_head_symbolic_head_existing_refs_index_status_and_source_unchanged': True,
    'original_remotes_unchanged': True,
    'pure_adaptation_changed_paths': adapted_changed,
    'fixed_baseline_physically_excludes_research_modules_and_controls': True,
    'observation_core_source_projection': core_results,
    'observation_timing_or_resource_equivalence_claimed': False,
    'patch_steps': patch_results,
    'bundle': {'path': bundle.name, 'sha256': sha(bundle.read_bytes()), 'prerequisite': UPSTREAM,
               'report': bundle_report, 'recovery_repo': str(recovery), 'all_exact_commit_and_tree_ids_recovered': True},
})
semantic = {'A1': ['U0'], 'F1': ['U0'], 'F2': ['U0'], 'F3': ['U0'], 'F4': ['U0'], 'F5': ['U0'],
            'O1': ['F1'], 'E1': ['O1'], 'B1': ['O1', 'F1'], 'N1': ['B1', 'O1'], 'P1': ['N1'], 'P2': ['P1']}
for step in steps:
    step['linear_predecessor'] = step['parent']
    step['stack_prerequisites'] = step.pop('depends_on', step.get('stack_prerequisites', []))
    step['semantic_requires'] = semantic[step['feature_id']]
    step['independence_note'] = ('功能前提与实际Git堆叠顺序分别列出；修复不语义依赖其他无关修复。研究层共同继承F5已知修复基座。接口/补丁上下文能否独立重排、编译和运行均未验证，不声明任意cherry-pick组合可用。')
    step['common_fixed_baseline'] = fixed if step['feature_id'] in {'O1', 'E1', 'B1', 'N1', 'P1', 'P2'} else None
    step['source_evidence_rows'] = [i for i, row in enumerate(evidence_rows) if row['feature_id'] == step['feature_id']]
save('steps.json', steps)
save('manifest.json', {
    'schema': 'iommu-source-organization-v1', 'created_utc': datetime.now(timezone.utc).isoformat(),
    'scope': 'source history organization and static reconstruction only',
    'upstream': {'url': 'https://github.com/zero-day-labs/riscv-iommu.git', 'branch': 'mc/dti-ats',
                 'commit': UPSTREAM, 'tree': git('rev-parse', UPSTREAM+'^{tree}').decode().strip(),
                 'ref': 'refs/heads/'+PREFIX+'upstream', 'evidence': 'upstream-ls-remote.txt'},
    'original': {'head': HEAD, 'branch': frozen['symbolic_head'], 'capture': 'frozen-input.json',
                 'parent_cva6_index_gitlink': '238a39e66a07b27a67fb5749df40691682555027',
                 'head_is_not_complete_source_identity': True},
    'anchors': {'upstream': UPSTREAM, 'adapted_only': by_id['A1']['commit'], 'adapted_functionally_fixed': fixed,
                'passive_observation': obs, 'research_current': end},
    'linear_patch_order_required': True, 'independent_cherry_picking_verified': False,
    'steps': steps,
    'baseline_service_semantics': 'F1 selects AW before AR to restore DMA write progress; it changes service order and may affect performance. This cumulative fixed baseline is not stock.',
    'observation_semantics': 'O1 keeps victim-pending queue as an output-only monitor and direct PTW AXI wires. Core-source projection checked. Added monitor storage and physical timing have not been characterized.',
    'historical_experiments': 'No old experiment is relabeled with new commits. V1/V2 and other out overlays remain historical variants; no new performance, correctness, RTL simulation, synthesis or board checks were run.',
    'recovery': {'patch_series': 'series', 'bundle': bundle.name, 'bundle_prerequisite': UPSTREAM,
                 'frozen_tree': frozen_tree, 'frozen_sources': 'frozen-source.tar.gz',
                 'submodule_gitlink_objects_included': False,
                 'submodule_note': 'Four vendor gitlinks are preserved exactly; their repositories/history are separate prerequisites, unchanged by this component series.'},
    'verification': 'reconstruction-verification.json', 'critical_source': 'critical-source-evidence.json',
    'known_limits': ['No build or elaboration was run for any new intermediate commit.',
                     'No new native/behavioral/FPGA/board result is claimed.',
                     'The original main checkout and preexisting refs remain in place; this chain is an isolated review/recovery artifact.'],
})
print(json.dumps({'endpoint': end, 'frozen_tree': frozen_tree, 'patches_verified': len(patches), 'bundle_exact_commit_recovery': True}, indent=2))
