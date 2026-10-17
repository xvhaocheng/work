#!/usr/bin/env python3
"""Finish layered research source, preserving compatible child interface bindings."""
import organize as o
import json, subprocess, hashlib, os

OBS = '5304477888b627546fa5f46ef36735f78f0ee504'
E3 = '027583cd564af8ccb0cc05228756afa8a322d017'
RESEARCH = '0b7939445298a23ea3e131ffd4e003cebb15ac59'

def frozen(path):
    return (o.OUT / 'frozen-source' / path).read_bytes()

def main():
    old_steps = json.loads((o.OUT / 'steps.json').read_text())
    (o.OUT / 'provisional-steps.json').write_text(json.dumps(old_steps, indent=2)+'\n')
    o.steps[:] = old_steps[:10]
    o.git('switch', '-c', 'codex/organize-20261017-cva6-research', o.steps[-1]['commit'])
    o.apply(o.original_patch('44068498'))
    full = (o.WT / o.P).read_text()
    passive = full.replace('            .E3Enable(InclDMA),\n', '')
    passive = passive.replace('            .e3_enable_i(e3_control[0]), .e3_arm_i(e3_control[1]),\n            .e3_active_i(e3_control[3]), .e3_delay128_i(e3_control[7]),\n', '')
    assert full != passive
    o.write(o.P, passive)
    paths = o.git('diff', '--cached', '--name-only').decode().splitlines()
    o.commit('CVA6-E3-TRACE', 'Expose passive E3 request and completion timing traces', ['44068498'], paths,
             ['CVA6-PATH', 'IOMMU-O1'],
             'No E3 delay-control input is connected; IOMMU O1 physically has no E3 delay module or control inputs.', OBS)
    o.write(o.P, full)
    o.commit('CVA6-E3-ACTIVE', 'Connect optional E3 PTW delay experiment controls', ['44068498'], [o.P],
             ['CVA6-E3-TRACE', 'IOMMU-E1'],
             'Active fixture: E3Enable and enable/arm/active/delay128 inputs can alter PTW service timing.', E3)
    o.historical('CVA6-E3-OPPORTUNITIES', 'Record bounded E3 trigger opportunities and intervening identities', ['147c4d4a'], ['CVA6-E3-ACTIVE'])
    o.historical('CVA6-HELD-LAUNCH', 'Control finite held DMA jobs and A-read-triggered victim launch', ['2b04936d'], ['CVA6-BATCH', 'CVA6-E3-OPPORTUNITIES'])
    o.historical('CVA6-P4M-GAP', 'Add P4M batch gaps and common-window aggressor progress', ['e822bc9d'], ['CVA6-HELD-LAUNCH'])
    o.historical('CVA6-E2-COST', 'Add passive E2 and natural translation cost event recording', ['7602ee53'], ['CVA6-P4M-GAP', 'IOMMU-E1'])
    o.historical('CVA6-LOG-RAM', 'Preserve single-record LUT RAM evidence log writes', ['2ccb6401'], ['CVA6-E2-COST'])
    o.historical('CVA6-LEGACY-E3-SELECT', 'Allow research profiles to omit legacy E3 trace hardware', ['8c1e08b3'], ['CVA6-LOG-RAM'])
    o.historical('CVA6-COST-AGGREGATE', 'Aggregate eligible COST copies into six observation groups', ['0dc5e333'], ['CVA6-E2-COST'])
    o.historical('CVA6-RESEARCH-POLICY', 'Add research probe roles and DMA admission policy with accepted-command fixes',
                 ['6f968ec9', 'a0497a8d', '475f3e19', 'a1b08934'], ['CVA6-COST-AGGREGATE', 'CVA6-BATCH'])
    assert not o.git('diff', '--name-only', o.OLD, 'HEAD', '--', '.', f':(exclude){o.LINK}')
    (o.OUT / 'historical-replay-check.json').write_text(json.dumps(dict(
        matches_all_original_head_regular_files=True, original_head=o.OLD,
        reconstructed_head=o.git('rev-parse','HEAD').decode().strip(),
        intentional_difference='Child pointer follows separately organized IOMMU E1'), indent=2)+'\n')
    # Passive quiet/gap taps do not authorize switching until a later controller consumes them.
    o.write(o.FRONT, frozen(o.FRONT))
    final_wrap = frozen(o.WRAP).decode()
    tap_wrap = final_wrap.replace('  output logic fullbuffer_job_o,\n', '')
    tap_wrap = tap_wrap.replace('    assign fullbuffer_job_o = 1\'b0;\n', '')
    tap_wrap = tap_wrap.replace('      .fullbuffer_job_o,\n', '')
    start = tap_wrap.index('    idma_fullbuffer_certificate #(\n')
    end = tap_wrap.index('    stream_fifo #(\n', start)
    tap_wrap = tap_wrap[:start] + tap_wrap[end:]
    o.write(o.WRAP, tap_wrap)
    o.commit('CVA6-QUIET-TAPS', 'Expose passive batch-gap and complete frontend quiet status', [],
             [o.FRONT, o.WRAP], ['CVA6-RESEARCH-POLICY'],
             'Frozen dirty source; quiet covers idle state, backend idle, pending valid/issue and same-cycle launch reads. No switching controller yet.')
    cert = 'corev_apu/iDMA/src/idma_fullbuffer_certificate.sv'
    bender = 'corev_apu/iDMA/Bender.yml'
    for path in [cert, bender, 'Makefile', o.WRAP]:
        o.write(path, frozen(path))
    o.commit('CVA6-FULLBUFFER-CERT', 'Certify one complete aligned copy from accepted and retired jobs', [],
             [cert, bender, 'Makefile', o.WRAP], ['CVA6-QUIET-TAPS'],
             'New permission interface for the later banked research controller. Default legacy buffer depth remains unchanged here.')
    ram = ['corev_apu/iDMA/src/idma_stream_fifo.sv',
           'vendor/pulp-platform/common_cells/src/fifo_v3.sv',
           'vendor/pulp-platform/common_cells/src/stream_fifo.sv']
    for path in ram:
        o.write(path, frozen(path))
    o.commit('CVA6-FULLBUFFER-RAM', 'Select FPGA queue storage for complete-copy research buffers', [], ram,
             ['CVA6-FULLBUFFER-CERT'],
             'Tracked vendor delta; FORCE_FPGA_RAM is selected only for 512 byte-wide lanes. Resource and timing costs are not validated by this organization.')
    obs = 'corev_apu/fpga/src/dma_nonblock_observer.sv'
    o.write(obs, frozen(obs))
    o.commit('CVA6-NONBLOCK-OBS', 'Add passive accepted-request and nonblocking diagnostic counters', [], [obs],
             ['CVA6-FULLBUFFER-CERT'], 'New read-only observer module; top-level connection is introduced by CVA6-BANKED-PN-WIRING.')
    comp = 'corev_apu/fpga/src/dma_composition_control.sv'
    o.write(comp, frozen(comp))
    o.commit('CVA6-COMPOSITION-CONTROL', 'Add quiet-gated P/N mode generation, clear and window accounting', [], [comp],
             ['CVA6-QUIET-TAPS', 'CVA6-NONBLOCK-OBS'],
             'Contains both passive counters and active configuration outputs; mode/clear writes affect the later IOMMU policy interface. Not a purely passive observer.')
    legacy = 'corev_apu/fpga/src/dma_observer.sv'
    o.write(legacy, frozen(legacy))
    o.commit('CVA6-OBSERVER-V9', 'Distinguish the trimmed composition register interface version', [], [legacy],
             ['CVA6-COMPOSITION-CONTROL'], 'Magic v9 prevents old drivers accepting the composition profile without legacy path/E3 stores.')
    for path in [o.P, o.TOP]:
        o.write(path, frozen(path))
    final = o.commit('CVA6-BANKED-PN-WIRING', 'Connect banked P/N research modes and bind the complete IOMMU research source', [],
                     [o.P, o.TOP], ['CVA6-FULLBUFFER-RAM', 'CVA6-COMPOSITION-CONTROL', 'CVA6-OBSERVER-V9', 'IOMMU-P2'],
                     'Atomic ABI binding to child P2: mode1 banked S, mode2 N, mode3 quiet-gated S/N/P/PN; DMA0/1 512-beat buffers, full-copy permission, MSI_BT_ONLY, counters and guarded configuration. Legacy default mode remains 0, which does not make this tree stock.', RESEARCH)
    o.git('branch', 'codex/organize-20261017-cva6-final', final)
    f = json.loads((o.OUT / 'frozen-manifest.json').read_text())
    capture = f['capture_commit']
    assert not o.git('diff', '--name-only', capture, final, '--', '.', f':(exclude){o.LINK}')
    (o.OUT / 'final-summary.json').write_text(json.dumps(dict(
        upstream=o.UP, adapted=o.steps[1]['commit'], fixed=o.steps[3]['commit'],
        final=final, final_tree=o.git('rev-parse','HEAD^{tree}').decode().strip(),
        capture=capture, capture_tree=f['capture_tree'], source_files_match_capture=True,
        original_index_iommu_gitlink=f['original_index_iommu_gitlink'],
        actual_child_head_at_freeze=f['actual_child_head_at_freeze'],
        final_iommu_gitlink=RESEARCH, intentional_tree_difference_paths=[o.LINK]), indent=2)+'\n')

if __name__ == '__main__':
    main()
