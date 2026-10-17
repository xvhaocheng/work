#!/usr/bin/env python3
"""Freeze and reorganize existing IOMMU source; never execute RTL or experiments."""
import hashlib
import io
import json
import os
from pathlib import Path
import stat
import subprocess
import sys
import tarfile
from datetime import datetime, timezone

ROOT = Path('/home/xvhaocheng/work/iommu-platform')
REPO = ROOT / 'cva6/corev_apu/riscv-iommu'
OUT = ROOT / 'provenance/source-organization-20261017/iommu'
WT = ROOT / 'out/source-organization-20261017/worktrees/iommu'
PREFIX = 'codex/organize-20261017-iommu-'
UPSTREAM = 'c80bba7ae74b54b50eb1028e6efd4292661ca75b'
HEAD = '57eb5621802ef9ed8d77c684b9a03205c6630391'

def run(*args, cwd=REPO, data=None, env=None):
    p = subprocess.run(args, cwd=cwd, input=data, stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE, env=env, check=True)
    return p.stdout

def git(*args, cwd=REPO, data=None, env=None):
    return run('git', *args, cwd=cwd, data=data, env=env)

def sha(data):
    return hashlib.sha256(data).hexdigest()

def save(name, obj):
    (OUT / name).write_text(json.dumps(obj, ensure_ascii=False, indent=2) + '\n')

def old(ref, path):
    return git('show', f'{ref}:{path}').decode()

def put(path, data):
    target = WT / path
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_bytes(data.encode() if isinstance(data, str) else data)

def replace(text, before, after, count=1):
    assert text.count(before) == count, (before, text.count(before), count)
    return text.replace(before, after)

def between(text, start, end, replacement=''):
    assert text.count(start) == 1, (start, end)
    a = text.index(start)
    b = text.index(end, a + len(start))
    assert a < b
    return text[:a] + replacement + text[b:]

def frozen(path):
    with tarfile.open(OUT / 'frozen-source.tar.gz', 'r:gz') as tar:
        return tar.extractfile(path).read().decode()

def freeze():
    assert git('rev-parse', 'HEAD').decode().strip() == HEAD
    OUT.mkdir(parents=True, exist_ok=True)
    initial = {
        'timestamp_utc': datetime.now(timezone.utc).isoformat(),
        'repo': str(REPO), 'head': HEAD,
        'symbolic_head': git('symbolic-ref', 'HEAD').decode().strip(),
        'refs': git('show-ref').decode(),
        'status_porcelain_v2': git('status', '--porcelain=v2').decode(),
        'index_sha256': sha((Path(git('rev-parse', '--absolute-git-dir').decode().strip()) / 'index').read_bytes()),
        'remote_output': git('remote', '-v').decode(),
        'upstream_ls_remote': git('ls-remote', 'upstream', 'refs/heads/mc/dti-ats', 'refs/heads/main').decode(),
    }
    assert f'{UPSTREAM}\trefs/heads/mc/dti-ats' in initial['upstream_ls_remote']
    tracked = git('ls-files', '-s', '-z').decode().split('\0')
    untracked = git('ls-files', '--others', '--exclude-standard', '-z').decode().strip('\0').split('\0')
    ignored = git('ls-files', '--others', '--ignored', '--exclude-standard', '--', 'rtl', 'packages').decode()
    assert ignored == '', 'Ignored source requires manual review'
    modes = {}
    for item in tracked:
        if not item: continue
        info, path = item.split('\t')
        mode, blob, stage = info.split()
        assert stage == '0'
        modes[path] = {'mode': mode, 'index_object': blob}
    for path in untracked:
        if path:
            assert path in {'rtl/transaction_controller/rv_iommu_banked_ctl.sv', 'rtl/translation_logic/rv_iommu_nonblock_hit.sv'}
            modes[path] = {'mode': '100755' if os.access(REPO / path, os.X_OK) else '100644', 'untracked': True}
    files = {}
    with tarfile.open(OUT / 'frozen-source.tar.gz', 'w:gz') as tar:
        for path, info in sorted(modes.items()):
            if info['mode'] == '160000':
                files[path] = info
                continue
            source = REPO / path
            data = os.readlink(source).encode() if info['mode'] == '120000' else source.read_bytes()
            info = dict(info, sha256=sha(data), bytes=len(data), frozen_blob=git('hash-object', '--stdin', data=data).decode().strip())
            files[path] = info
            tar.add(source, arcname=path, recursive=False)
    initial['files'] = files
    save('frozen-input.json', initial)
    (OUT / 'original-working.patch').write_bytes(git('diff', '--binary', 'HEAD'))
    (OUT / 'original-staged.patch').write_bytes(git('diff', '--cached', '--binary'))
    (OUT / 'upstream-ls-remote.txt').write_text(initial['timestamp_utc'] + '\n' + initial['upstream_ls_remote'])
    return initial

def commit(feature, name, role, description, depends, evidence):
    git('add', '-A', cwd=WT)
    git('commit', '-m', f'organize: {description}', cwd=WT)
    oid = git('rev-parse', 'HEAD', cwd=WT).decode().strip()
    ref = f'refs/heads/{PREFIX}{name}'
    git('update-ref', ref, oid, '0' * 40)
    entry = {
        'feature_id': feature, 'name': name, 'role': role, 'commit': oid,
        'tree': git('rev-parse', f'{oid}^{{tree}}').decode().strip(),
        'ref': ref, 'parent': git('rev-parse', f'{oid}^').decode().strip(),
        'description': description, 'depends_on': depends, 'source_evidence': evidence,
        'changed_paths': git('diff-tree', '--no-commit-id', '--name-only', '-r', oid).decode().splitlines(),
        'validation': 'static source reconstruction only; no build, simulation, synthesis or board execution',
    }
    entries.append(entry)
    save('steps.json', entries)
    return entry

def baseline():
    global initial, entries
    initial = freeze()
    entries = []
    git('update-ref', f'refs/heads/{PREFIX}upstream', UPSTREAM, '0' * 40)
    WT.parent.mkdir(parents=True, exist_ok=True)
    git('worktree', 'add', '-b', PREFIX + 'series', str(WT), UPSTREAM)
    git('apply', data=git('diff', f'{UPSTREAM}..6cc8ed3075b7ebce51c1cfd38dd3c07f177b888b'), cwd=WT)
    commit('A1', 'adapted', 'necessary interface adaptation', 'retain multi-DMA translation sideband interface', ['U0'], ['6cc8ed3075b7ebce51c1cfd38dd3c07f177b888b:rtl/ext_interfaces/axi_iommu_intf.sv'])
    git('apply', data=git('diff', '7fbb2b9^', '7fbb2b9'), cwd=WT)
    commit('F1', 'fix-aw-progress', 'functional progress fix with service-order effect', 'select AW before AR for DMA write progress', ['A1'], ['7fbb2b994778b6f3f86c0240e38502aca6d15034:rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv'])
    put('rtl/software_interface/rv_iommu_hpm.sv', old('da8a801', 'rtl/software_interface/rv_iommu_hpm.sv'))
    commit('F2', 'fix-hpm-edge', 'functional counter fix', 'latch the HPM event edge', ['F1'], ['da8a8011f2c52fb1e0c5fd8ae9b9757a5f0ffbb5:rtl/software_interface/rv_iommu_hpm.sv'])
    put('rtl/ext_interfaces/rv_iommu_ds_if.sv', (REPO / 'rtl/ext_interfaces/rv_iommu_ds_if.sv').read_bytes())
    commit('F3', 'fix-ds-owner', 'functional AXI owner fix', 'retain accepted AW ownership until WLAST', ['F2'], ['frozen-input.json:rtl/ext_interfaces/rv_iommu_ds_if.sv'])
    git('apply', data=git('diff', 'HEAD', '--', 'rtl/translation_logic/rv_iommu_ptw.sv'), cwd=WT)
    commit('F4', 'fix-ptw-leaf', 'functional translation fix', 'accept valid 4 KiB and 2 MiB PTW leaves', ['F3'], ['frozen-input.json:rtl/translation_logic/rv_iommu_ptw.sv'])
    paths = ['rtl/transaction_controller/rv_iommu_axi_bc.sv'] + git('diff-tree', '--no-commit-id', '--name-only', '-r', HEAD, '--', 'sim/axi_bc').decode().splitlines()
    for path in paths: put(path, (REPO / path).read_bytes())
    end = commit('F5', 'baseline-fixed', 'cumulative adapted and functionally fixed baseline; not stock', 'correct AXI boundary arithmetic and retain existing regression sources', ['F4'], [f'{HEAD}:rtl/transaction_controller/rv_iommu_axi_bc.sv', f'{HEAD}:sim/axi_bc'])
    save('baseline-summary.json', {'upstream': UPSTREAM, 'adapted': entries[0], 'fixed': end, 'steps': entries, 'tested': False})
    print(json.dumps({'upstream': UPSTREAM, 'adapted': entries[0]['commit'], 'fixed': end['commit']}, indent=2))

def top_without_policy(text):
    text = replace(text, '    parameter bit PolicyEnable = 0,\n', '')
    text = replace(text, '    input  logic policy_partition_i, policy_clear_i,\n    input  rv_iommu::device_id_t policy_did_i,\n    output logic policy_quiet_o,\n', '')
    text = between(text, '    logic in_flight, policy_tl_quiet;\n', '    logic atsinval_to, atsinval_inflight;\n', '    logic in_flight;\n\n')
    text = replace(text, '            .PolicyEnable(PolicyEnable),\n', '', 2)
    text = replace(text, '            .policy_partition_i, .policy_clear_i, .policy_did_i,\n            .policy_quiet_o(policy_tl_quiet),\n', '', 2)
    return text

def research():
    global entries
    entries = json.loads((OUT / 'steps.json').read_text())
    assert git('rev-parse', 'HEAD', cwd=WT).decode().strip() == entries[-1]['commit']
    # O1: copy historical taps, with no active injection controls or memory mux.
    paths = ['packages/rv_iommu_pkg.sv', 'rtl/translation_logic/rv_iommu_iotlb.sv',
             'rtl/transaction_controller/rv_iommu_e3_queue.sv']
    for path in paths: put(path, old(HEAD, path))
    put('rtl/translation_logic/rv_iommu_ptw.sv', frozen('rtl/translation_logic/rv_iommu_ptw.sv'))
    top = old(HEAD, 'rtl/rv_iommu_top.sv')
    top = replace(top, '    parameter bit E3Enable = 0,\n', '')
    top = replace(top, '    input logic e3_enable_i, e3_arm_i, e3_active_i, e3_delay128_i,\n', '')
    top = replace(top, '\n        .E3Enable          (E3Enable),\n', '\n')
    top = replace(top, '\n            .E3Enable          (E3Enable),\n', '\n', 2)
    top = replace(top, '            .e3_enable_i, .e3_arm_i, .e3_active_i, .e3_delay128_i,\n            .e3_waiting_i(e3_ctl_o.victim_waiting != 0 && !e3_ctl_o.queue_error),\n', '', 2)
    put('rtl/rv_iommu_top.sv', top)
    ctl = old(HEAD, 'rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv')
    ctl = replace(ctl, "    parameter bit E3Enable = 1'b0,\n", '')
    start, end = ctl.index('    if (E3Enable) begin : gen_e3_queue\n'), ctl.index('    always_comb begin\n')
    queue = ctl[start:end]
    queue = queue[queue.index('        rv_iommu_e3_queue'):queue.index('    end else begin')]
    queue = '\n'.join(line[4:] if line.startswith('    ') else line for line in queue.split('\n'))
    ctl = ctl[:start] + queue + ctl[end:]
    put('rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv', ctl)
    tl = old(HEAD, 'rtl/translation_logic/rv_iommu_tl_wrap.sv')
    tl = replace(tl, '    parameter bit E3Enable = 0,\n', '')
    tl = replace(tl, '    input logic e3_enable_i, e3_arm_i, e3_active_i, e3_waiting_i, e3_delay128_i,\n', '')
    start, end = tl.index('    if (E3Enable) begin : gen_e3_delay\n'), tl.index('    rv_iommu_ptw #(\n')
    tl = tl[:start] + '''    // Observation projection: the PTW AXI request/response remain direct wires.
    assign ptw_axi_req_o = e3_ptw_req;
    assign e3_ptw_resp = ptw_axi_resp_i;
    assign e3_capture = 0;
    assign e3_available = 0;
    assign e3_consume = 0;
    assign e3_error = 0;
    assign e3_extra = 0;
''' + tl[end:]
    put('rtl/translation_logic/rv_iommu_tl_wrap.sv', tl)
    bender = replace(old(HEAD, 'Bender.yml'), '  - rtl/translation_logic/rv_iommu_e3_delay.sv\n', '')
    put('Bender.yml', bender)
    commit('O1', 'observation', 'passive observation projection; not a tested stock baseline', 'separate passive lifecycle taps from E3 injection', ['F5'], ['1519510', '137f5e5', '238a39e', 'queue outputs feed only e3_o in this projection; PTW request/response direct wires'])
    # E1: restore the historical active response-delay hook, explicitly research.
    for path in ['rtl/rv_iommu_top.sv', 'rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv',
                 'rtl/translation_logic/rv_iommu_tl_wrap.sv', 'rtl/translation_logic/rv_iommu_e3_delay.sv', 'Bender.yml']:
        put(path, old(HEAD, path))
    commit('E1', 'e3-injection', 'active research perturbation', 'add qualified one-shot E3 PTW response delay', ['O1'], ['1519510de552b6f3e01a4f7a0b5428947bd87737', '137f5e5fb1f2cc65a1346359226b16247c3f8445'])
    # B1: banked ownership and admission, without nonblocking hit or P policy.
    for path in ['rtl/transaction_controller/rv_iommu_banked_ctl.sv', 'rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv']:
        put(path, frozen(path))
    top = top_without_policy(frozen('rtl/rv_iommu_top.sv'))
    top = replace(top, '    parameter bit NonblockEnable = 0,\n', '')
    top = replace(top, '    input  logic nonblock_enable_i,\n', '')
    top = replace(top, '    output logic nonblock_fire_o,\n    output logic [15:0] nonblock_diag_o,\n', '')
    top = replace(top, '''    initial begin
        if (NonblockEnable && !BankedEnable)
            $fatal(1, "NonblockEnable requires lifecycle-tagged banked controller");
    end

''', '')
    top = replace(top, '            .NonblockEnable    (NonblockEnable),\n', '', 2)
    top = replace(top, '            .nonblock_enable_i(nonblock_enable_i),\n            .nonblock_fire_o(nonblock_fire_o),\n            .nonblock_diag_o(nonblock_diag_o),\n', '', 2)
    put('rtl/rv_iommu_top.sv', top)
    tl = old(HEAD, 'rtl/translation_logic/rv_iommu_tl_wrap.sv')
    tl = replace(tl, '    assign trans_resp_data_o.error      = error_q;\n', '    assign trans_resp_data_o.bank = req_data.bank;\n    assign ats_trans_resp_data_o.bank = req_data.bank;\n    assign trans_resp_data_o.error      = error_q;\n')
    put('rtl/translation_logic/rv_iommu_tl_wrap.sv', tl)
    bender = replace(old(HEAD, 'Bender.yml'), '  - rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv\n', '  - rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv\n  - rtl/transaction_controller/rv_iommu_banked_ctl.sv\n')
    put('Bender.yml', bender)
    commit('B1', 'banked', 'research controller and admission mechanism', 'add two-bank lifecycle tags and owner read admission', ['E1'], ['frozen-input.json:rtl/transaction_controller/rv_iommu_banked_ctl.sv', 'frozen-input.json:rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv', 'frozen-input.json:rtl/rv_iommu_top.sv'])
    # N1: actual cache probes/touches and response arbitration, without P policy.
    top = top_without_policy(frozen('rtl/rv_iommu_top.sv'))
    put('rtl/rv_iommu_top.sv', top)
    for path in ['rtl/translation_logic/rv_iommu_ddtc.sv', 'rtl/translation_logic/rv_iommu_nonblock_hit.sv', 'Bender.yml']:
        put(path, frozen(path))
    iotlb = old(HEAD, 'rtl/translation_logic/rv_iommu_iotlb.sv')
    iotlb = replace(iotlb, '    parameter rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfg = rv_iommu_cfg::NullCfg\n', '    parameter rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfg = rv_iommu_cfg::NullCfg,\n    parameter bit TouchEnable = 0\n')
    iotlb = replace(iotlb, '    input  logic                            lookup_i,\n', '    input  logic                            lookup_i,\n    input  logic                            touch_i,\n')
    iotlb = replace(iotlb, '            if (lu_hit[i]) begin\n', '            if (lu_hit[i] && (!TouchEnable || touch_i)) begin\n')
    put('rtl/translation_logic/rv_iommu_iotlb.sv', iotlb)
    tl = frozen('rtl/translation_logic/rv_iommu_tl_wrap.sv')
    tl = replace(tl, '    parameter bit PolicyEnable = 0,\n', '')
    tl = replace(tl, '    input logic policy_partition_i, policy_clear_i,\n    input rv_iommu::device_id_t policy_did_i,\n    output logic policy_quiet_o,\n', '')
    tl = replace(tl, '''    initial begin
        if (PolicyEnable && (!NonblockEnable || RVIOMMUCfg.NumIotlbEntries != 16))
            $fatal(1, "Composition requires nonblocking-capable 16-entry IOTLB");
    end
''', '')
    tl = between(tl, '    // All lifecycle storage must be empty; incoming VALID also excludes a\n', '    assign n_window = state_q == PT && e3_ptw_active && en_1S_q &&\n')
    tl = replace(tl, '        .PolicyEnable(PolicyEnable),\n        .PartitionLocalHistory(PolicyEnable),\n', '')
    tl = between(tl, '        .policy_clear_i (PolicyEnable && policy_clear_i),\n', '        .lookup_i       (iotlb_lu),\n', '\n')
    put('rtl/translation_logic/rv_iommu_tl_wrap.sv', tl)
    commit('N1', 'nonblock-hit', 'research hit bypass and probe touch mechanism', 'add bounded nonblocking hits with accepted cache touches', ['B1'], ['frozen-input.json:rtl/translation_logic/rv_iommu_nonblock_hit.sv', 'frozen-input.json:rtl/translation_logic/rv_iommu_tl_wrap.sv', 'frozen-input.json:rtl/translation_logic/rv_iommu_ddtc.sv'])
    # P1: partition allocation and quiescent clear; original shared PLRU history.
    put('rtl/rv_iommu_top.sv', frozen('rtl/rv_iommu_top.sv'))
    tl = replace(frozen('rtl/translation_logic/rv_iommu_tl_wrap.sv'), '        .PartitionLocalHistory(PolicyEnable),\n', '')
    put('rtl/translation_logic/rv_iommu_tl_wrap.sv', tl)
    iotlb = frozen('rtl/translation_logic/rv_iommu_iotlb.sv')
    iotlb = replace(iotlb, '''    parameter bit PolicyEnable = 0,
    // Composition experiment: independent history for fixed ways 0-1 / 2-15.
    // Other geometries/masks and the default instance retain shared PLRU.
    parameter bit PartitionLocalHistory = 0
''', '    parameter bit PolicyEnable = 0\n')
    iotlb = between(iotlb, '    function automatic logic [2*RVIOMMUCfg.NumIotlbEntries-2:0] history_nodes(\n', '    logic [2*RVIOMMUCfg.NumIotlbEntries-2:0] eligible_nodes;\n')
    iotlb = between(iotlb, '                    // Fill ownership belongs to the main request, but a hit\n', '                end\n            end\n        end\n', '                    plru_tree_n[idx_base + (i >> shift)] = new_index[0];\n')
    put('rtl/translation_logic/rv_iommu_iotlb.sv', iotlb)
    commit('P1', 'partition-allocation', 'research partition allocation and configuration mechanism', 'add allocation masks and quiet policy clear', ['N1'], ['frozen-input.json:rtl/translation_logic/rv_iommu_iotlb.sv', 'frozen-input.json:rtl/translation_logic/rv_iommu_tl_wrap.sv', 'frozen-input.json:rtl/rv_iommu_top.sv'])
    # P2: restore the frozen independent partition history, completing the tree.
    for path in ['rtl/translation_logic/rv_iommu_iotlb.sv', 'rtl/translation_logic/rv_iommu_tl_wrap.sv']:
        put(path, frozen(path))
    end = commit('P2', 'research-current', 'research partition-local replacement history; exact frozen endpoint', 'isolate replacement history for fixed 2 and 14 way partitions', ['P1'], ['frozen-input.json:rtl/translation_logic/rv_iommu_iotlb.sv', 'frozen-input.json:rtl/translation_logic/rv_iommu_tl_wrap.sv'])
    print(json.dumps({'research_endpoint': end['commit'], 'tree': end['tree'], 'steps': len(entries)}, indent=2))

if __name__ == '__main__':
    {'baseline': baseline, 'research': research}[sys.argv[1] if len(sys.argv) > 1 else 'baseline']()
