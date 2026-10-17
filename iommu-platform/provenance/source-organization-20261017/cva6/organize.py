#!/usr/bin/env python3
"""Bounded source organization; never builds, runs RTL, or edits the old checkout."""
from pathlib import Path
import subprocess, json, os, hashlib, shutil, datetime

ROOT = Path('/home/xvhaocheng/work/iommu-platform')
REPO = ROOT / 'cva6'
OUT = ROOT / 'provenance/source-organization-20261017/cva6'
WT = ROOT / 'out/source-organization-20261017/worktrees/cva6'
UP = '1f992d808dc0abe599329dedf73842789f8013b8'
OLD = 'a1b08934391833aed21e6ad63b545634040808ac'
IFACE = '7bea3f202903279c18df99a2284d6f56ccdacdc6'
FIXED = '85b157ebe71a623e326e51fff7b7126fe77e8eb9'
LINK = 'corev_apu/riscv-iommu'
P = 'corev_apu/fpga/src/ariane_peripherals_xilinx.sv'
TOP = 'corev_apu/fpga/src/ariane_xilinx.sv'
FRONT = 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv'
WRAP = 'corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv'
steps = []

def git(*args, cwd=WT, data=None, env=None):
    return subprocess.check_output(['git', '-C', str(cwd), *args], input=data, env=env)

def source(commit, path):
    return git('show', f'{commit}:{path}', cwd=REPO)

def write(path, data):
    full = WT / path
    full.parent.mkdir(parents=True, exist_ok=True)
    full.write_bytes(data.encode() if isinstance(data, str) else data)

def bind(sha):
    git('update-index', '--add', '--cacheinfo', f'160000,{sha},{LINK}')

def commit(feature, title, origin, paths=(), dependencies=(), note='', link=None):
    if paths:
        git('add', '--', *paths)
    if link:
        bind(link)
    git('diff', '--cached', '--exit-code', '--quiet') if not paths and not link else None
    body = title + '\n\nSource-organization: 2026-10-17; new commit authored at actual current time.\n'
    body += f'Feature-ID: {feature}\nOriginal-source: {", ".join(origin) if origin else "frozen original working source"}\n'
    body += 'Validation scope: source/patch identity only; no build, simulation or experiment.\n'
    if note:
        body += note + '\n'
    git('commit', '--no-gpg-sign', '-m', body)
    sha = git('rev-parse', 'HEAD').decode().strip()
    steps.append(dict(feature_id=feature, title=title, commit=sha,
                      parent=git('rev-parse', 'HEAD^').decode().strip(),
                      tree=git('rev-parse', 'HEAD^{tree}').decode().strip(),
                      original_commits=origin, dependencies=list(dependencies), note=note,
                      changed_paths=git('diff-tree', '--no-commit-id', '--name-only', '-r', sha).decode().splitlines(),
                      iommu_gitlink=git('ls-tree', 'HEAD', LINK).decode().split()[2]))
    (OUT / 'steps.json').write_text(json.dumps(steps, indent=2) + '\n')
    print(feature, sha, flush=True)
    return sha

def original_patch(commit_sha):
    return git('diff', '--binary', f'{commit_sha}^', commit_sha, '--', '.', f':(exclude){LINK}', cwd=REPO)

def apply(data):
    git('apply', '--check', '-', data=data)
    git('apply', '--index', '-', data=data)

def historical(feature, title, origins, dependencies=()):
    for origin in origins:
        apply(original_patch(origin))
    paths = git('diff', '--cached', '--name-only').decode().splitlines()
    return commit(feature, title, origins, paths, dependencies)

def create_capture():
    index = OUT / 'capture.index'
    env = dict(os.environ, GIT_INDEX_FILE=str(index))
    git('read-tree', OLD, cwd=REPO, env=env)
    frozen = json.loads((OUT / 'frozen-manifest.json').read_text())
    for path in frozen['changed_paths']:
        blob = git('hash-object', '-w', '--stdin', cwd=REPO,
                   data=(OUT / 'frozen-source' / path).read_bytes()).decode().strip()
        git('update-index', '--add', '--cacheinfo', f'100644,{blob},{path}', cwd=REPO, env=env)
    tree = git('write-tree', cwd=REPO, env=env).decode().strip()
    message = ('Capture original CVA6 source files and original indexed gitlink\n\n'
               'This is a new capture commit, not a historical experiment commit.\n'
               'Indexed IOMMU pointer 238a39e differs from actual child HEAD 57eb562 plus dirty source.\n'
               'Reconstructed research profiles bind the independently organized IOMMU instead.\n')
    capture = git('commit-tree', tree, '-p', OLD, cwd=REPO, data=message.encode()).decode().strip()
    git('update-ref', 'refs/heads/codex/organize-20261017-cva6-capture', capture, '0'*40, cwd=REPO)
    index.unlink()
    frozen['capture_commit'] = capture
    frozen['capture_tree'] = tree
    frozen['original_index_iommu_gitlink'] = '238a39e66a07b27a67fb5749df40691682555027'
    frozen['actual_child_head_at_freeze'] = git('rev-parse', 'HEAD', cwd=REPO / LINK).decode().strip()
    frozen['actual_child_dirty_sha256'] = hashlib.sha256(git('diff', '--binary', 'HEAD', cwd=REPO / LINK)).hexdigest()
    (OUT / 'frozen-manifest.json').write_text(json.dumps(frozen, ensure_ascii=False, indent=2) + '\n')

def main():
    create_capture()
    WT.parent.mkdir(parents=True, exist_ok=True)
    git('worktree', 'add', '-b', 'codex/organize-20261017-cva6-chain', str(WT), UP, cwd=REPO)
    # Adapt the ABI while preserving the ordinary direct DS service path.
    paths = ['Makefile', 'corev_apu/fpga/scripts/run.tcl',
             'corev_apu/iDMA/src/idma_backend.sv', 'corev_apu/iDMA/src/idma_legalizer.sv',
             'corev_apu/tb/ariane_soc_pkg.sv', P]
    for path in paths:
        write(path, source('b98b0ef7', path))
    text = (WT / P).read_text()
    text = text.replace('parameter bit InclIOMMU    =  0,\n    parameter int unsigned IOMMUDSFixedDelayInput  = 0,\n    parameter int unsigned IOMMUDSFixedDelayOutput = 0',
                        'parameter bit InclIOMMU    =  0')
    start = text.index('        axi_ds_req_t  axi_iommu_ds_delayed_req;')
    end = text.index('\n\n        // AXI Bus between IOMMU Completion IF', start)
    text = text[:start] + '        `AXI_ASSIGN_FROM_REQ(iommu_ds, axi_iommu_ds_req)\n        `AXI_ASSIGN_TO_RESP(axi_iommu_ds_resp, iommu_ds)' + text[end:]
    write(P, text)
    commit('CVA6-DTI-COMPAT', 'Adapt fixed IOMMU ABI with untranslated DMA metadata', ['b98b0ef7'], paths,
           ['CVA6-UPSTREAM', 'IOMMU-A1'],
           'No DS delayer, observer, active-cycle counter, E3 or research ports are introduced.', IFACE)
    # Reuse exact historic two-DMA mux/demux then four-DMA aperture expansion.
    for old in ['b63c5523', '7f642515']:
        data = git('diff', '--binary', f'{old}^', old, '--', P, 'corev_apu/tb/ariane_soc_pkg.sv', cwd=REPO)
        apply(data)
    integration = commit('CVA6-INTEGRATION', 'Connect four DMA lanes through disjoint address and AXI ID spaces',
                         ['b63c5523', '7f642515'], [P, 'corev_apu/tb/ariane_soc_pkg.sv'],
                         ['CVA6-DTI-COMPAT'],
                         'Each 4 KiB register page selects one DMA; mux source bits distinguish lanes. No scheduling or measurement policy.')
    git('branch', 'codex/organize-20261017-cva6-adapted', integration)
    historical('CVA6-SC-FIX', 'Retain SC response byte offset until completion', ['7f086b73'], ['CVA6-INTEGRATION'])
    fixed = commit('CVA6-IOMMU-FIXES', 'Bind independently separated IOMMU correctness fixes',
                   ['3e409bad', '7f642515'], (), ['CVA6-SC-FIX', 'IOMMU-F5'],
                   'The child has separately recorded service-order and correctness patches; this is not a claim of unchanged upstream timing.', FIXED)
    git('branch', 'codex/organize-20261017-cva6-baseline-fixed', fixed)
    # Restore only the previously removed active service-model experiment.
    write(P, source('7f642515', P))
    write(TOP, source('b98b0ef7', TOP))
    delay = 'vendor/pulp-platform/common_cells/src/stream_delay.sv'
    write(delay, source('b98b0ef7', delay))
    commit('CVA6-DS-DELAY', 'Add explicit experimental DS service delay with 50/50 defaults', ['b98b0ef7'],
           [P, TOP, delay], ['CVA6-IOMMU-FIXES'],
           'Active fixture: inserts axi_delayer and changes DS service timing. Excluded from both ordinary source baseline anchors.')
    write(FRONT, source('b98b0ef7', FRONT))
    commit('CVA6-ACTIVE-CYCLES', 'Expose passive DMA backend active-cycle counter', ['b98b0ef7'],
           [FRONT], ['CVA6-DS-DELAY'], 'Only counts backend activity and exposes offset 0x50; not a DMA admission policy.')
    cleanup = 'corev_apu/fpga/Makefile'
    write(cleanup, source('b98b0ef7', cleanup))
    commit('CVA6-BUILD-CLEANUP', 'Preserve existing Vivado cleanup target coverage', ['b98b0ef7'],
           [cleanup], ['CVA6-ACTIVE-CYCLES'], 'Source preservation only; this target is never executed by the organization workflow.')
    historical('CVA6-LIFECYCLE', 'Observe DMA lifecycle and overlap through passive taps', ['540544ce'], ['CVA6-ACTIVE-CYCLES'])
    historical('CVA6-BATCH', 'Add bounded autonomous copy batches and safe reuse accounting', ['1be70a4f', '7ed43840'], ['CVA6-LIFECYCLE'])
    historical('CVA6-PATH', 'Record passive per-ID DMA path timing', ['a1572926'], ['CVA6-LIFECYCLE', 'CVA6-BATCH'])
    # Split passive E3 trace collection from the wires that actuate a PTW delay.
    apply(original_patch('44068498'))
    full = (WT / P).read_text()
    passive = full.replace('            .E3Enable(InclDMA),\n', '')
    passive = passive.replace('            .e3_enable_i(e3_control[0]), .e3_arm_i(e3_control[1]),\n            .e3_active_i(e3_control[3]), .e3_delay128_i(e3_control[7]),\n', '')
    assert full != passive
    write(P, passive)
    paths = git('diff', '--cached', '--name-only').decode().splitlines()
    commit('CVA6-E3-TRACE', 'Expose passive E3 request and completion timing traces', ['44068498'], paths,
           ['CVA6-PATH', 'IOMMU-OBSERVATION'], 'No E3 delay-control input is connected at this stage; control-bank outputs only drive observation metadata.')
    write(P, full)
    commit('CVA6-E3-ACTIVE', 'Connect optional E3 PTW delay experiment controls', ['44068498'], [P],
           ['CVA6-E3-TRACE', 'IOMMU-E3'], 'Active control: E3Enable plus enable/arm/active/delay128 inputs can change PTW service timing.')
    historical('CVA6-E3-OPPORTUNITIES', 'Record bounded E3 trigger opportunities and intervening identities', ['147c4d4a'], ['CVA6-E3-ACTIVE'])
    historical('CVA6-HELD-LAUNCH', 'Control finite held DMA jobs and A-read-triggered victim launch', ['2b04936d'], ['CVA6-BATCH', 'CVA6-E3-OPPORTUNITIES'])
    historical('CVA6-P4M-GAP', 'Add P4M batch gaps and common-window aggressor progress', ['e822bc9d'], ['CVA6-HELD-LAUNCH'])
    historical('CVA6-E2-COST', 'Add passive E2 and natural translation cost event recording', ['7602ee53'], ['CVA6-P4M-GAP', 'IOMMU-RESEARCH-BUS'])
    historical('CVA6-LOG-RAM', 'Preserve single-record LUT RAM evidence log writes', ['2ccb6401'], ['CVA6-E2-COST'])
    historical('CVA6-LEGACY-E3-SELECT', 'Allow research profiles to omit legacy E3 trace hardware', ['8c1e08b3'], ['CVA6-LOG-RAM'])
    historical('CVA6-COST-AGGREGATE', 'Aggregate eligible COST copies into six observation groups', ['0dc5e333'], ['CVA6-E2-COST'])
    historical('CVA6-RESEARCH-POLICY', 'Add research probe roles and DMA admission policy with accepted-command fixes',
               ['6f968ec9', 'a0497a8d', '475f3e19', 'a1b08934'], ['CVA6-COST-AGGREGATE', 'CVA6-BATCH'])
    diff = git('diff', '--name-only', OLD, 'HEAD', '--', '.', f':(exclude){LINK}')
    assert not diff, diff.decode()
    (OUT / 'historical-replay-check.json').write_text(json.dumps(dict(
        matches_all_original_head_regular_files=True, original_head=OLD,
        reconstructed_head=git('rev-parse','HEAD').decode().strip(),
        intentional_difference='IOMMU gitlink follows separately organized component chain'), indent=2)+'\n')

if __name__ == '__main__':
    main()
