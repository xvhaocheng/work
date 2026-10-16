"""Codex: stronger ordinary background policy, eligible current owner only.

This changes preparation selection only. It adds no state, parent slot,
payload buffer or data scheduler; normal TC requests still take priority.
"""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = Path(__file__).resolve().parent
OLD = ROOT / 'out/parent-translation-reuse-20261008'

def sub(text, old, new):
    assert text.count(old) == 1, (old, text.count(old))
    return text.replace(old, new, 1)

helper = (OLD / 'early_parent_translation.sv').read_text()
helper = sub(helper, '    input logic [1:0] parent_valid_i, parent_bank_i,',
    '    input logic [1:0] prepare_eligible_i,\n    input logic [1:0] parent_valid_i, parent_bank_i,')
helper = sub(helper, 'if (enable_i && prepare_i && !flush_i && parent_valid_i[f] &&',
    'if (enable_i && prepare_i && prepare_eligible_i[f] && !flush_i && parent_valid_i[f] &&')
(OUT / 'owner_ready_parent_translation.sv').write_text(helper)

banked = (OLD / 'early_banked_top.sv').read_text()
banked = sub(banked, '    input logic [1:0] reuse_parent_valid_i, reuse_parent_bank_i,',
    '    input logic [1:0] reuse_prepare_eligible_i,\n    input logic [1:0] reuse_parent_valid_i, reuse_parent_bank_i,')
banked = sub(banked, '        .parent_valid_i(reuse_parent_valid_i),',
    '        .prepare_eligible_i(reuse_prepare_eligible_i),\n        .parent_valid_i(reuse_parent_valid_i),')
(OUT / 'owner_ready_banked_top.sv').write_text(banked)

inner = (OLD / 'early_actual_dma_top.sv').read_text()
inner = sub(inner, '        .reuse_parent_valid_i(reuse_parent_valid),',
    '''        // Use the already existing child-service eligibility; no new W order.
        .reuse_prepare_eligible_i({
            i_pre.candidate_valid && i_pre.candidate_owner == 1'b1,
            i_pre.candidate_valid && i_pre.candidate_owner == 1'b0}),
        .reuse_parent_valid_i(reuse_parent_valid),''')
(OUT / 'owner_ready_actual_dma_top.sv').write_text(inner)

argv = json.loads((OLD / 'build-early-command.json').read_text())
mapping = {
    str(OLD / 'early_parent_translation.sv'): str(OUT / 'owner_ready_parent_translation.sv'),
    str(OLD / 'early_banked_top.sv'): str(OUT / 'owner_ready_banked_top.sv'),
    str(OLD / 'early_actual_dma_top.sv'): str(OUT / 'owner_ready_actual_dma_top.sv'),
    str(OLD / 'obj-early'): str(OUT / 'obj-owner-ready'),
}
assert all(k in argv for k in mapping)
(OUT / 'build-owner-ready-command.json').write_text(json.dumps([mapping.get(x,x) for x in argv],indent=2)+'\n')
