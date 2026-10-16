#!/usr/bin/env python3
"""Codex section 38: deterministic out-only variants, no production edits."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = Path(__file__).resolve().parent
BASE = ROOT / 'out/parent-translation-reuse-20261008'

def replace_once(s, old, new):
    assert s.count(old) == 1, (old, s.count(old))
    return s.replace(old, new)

raw = (ROOT / 'out/fragment-placement-20261008/pretl_owner_fragment.sv').read_text()
raw = replace_once(raw, '    logic [63:0] offset_q [2][ParentDepth];\n', '')
raw = replace_once(raw, '    logic [31:0] cycle_q;\n', '    logic [31:0] cycle_q;\n    logic [3:0] raw_highwater_q [2]; // observation only\n')
raw = replace_once(raw, '    initial begin\n', '''    function automatic logic [63:0] parent_offset(input int s, input int n);
        return parent_q[s][n].addr - src_q[s];
    endfunction
    initial begin
        $display("RAW4-DECL ar_bits=%0d functional_bits=%0d observation_bits=168", $bits(ar_t),
            2*ParentDepth*($bits(ar_t)+9)+2*4*32+2*3*64+2+
            D*(2+2*32+64+9)+4*32+9+3+$bits(ar_t)+18);
''')
raw = replace_once(raw, 'offset_q[s][n] +', 'parent_offset(s,n) +')
raw = replace_once(raw, "parent_q[s][n] <= '0; offset_q[s][n] <= 0;", "parent_q[s][n] <= '0;")
raw = replace_once(raw, '            for (int s = 0; s < 2; s++) begin\n                pa_q[s] <= 0; done_q[s] <= 0;\n', '            for (int s = 0; s < 2; s++) begin\n                pa_q[s] <= 0; done_q[s] <= 0; raw_highwater_q[s] <= 0;\n')
raw = replace_once(raw, '                offset_q[s][n] <= s_ar_i.addr-src_q[s];\n', '')
raw = replace_once(raw, '                pa_q[s] <= pa_q[s]+1;\n', '''                pa_q[s] <= pa_q[s]+1;
                if (pa_q[s]+1-done_q[s] > 32'(raw_highwater_q[s]))
                    raw_highwater_q[s] <= 4'(pa_q[s]+1-done_q[s]);
''')
raw = replace_once(raw, 'offset_q[candidate_owner][p_index(done_q[candidate_owner])]+', 'parent_offset(int\'(candidate_owner),p_index(done_q[candidate_owner]))+')
raw = replace_once(raw, 'endmodule\n', '''    final $display("RAW4-STATS depth=%0d highwater_v=%0d highwater_a=%0d live_v=%0d live_a=%0d",
        ParentDepth,raw_highwater_q[0],raw_highwater_q[1],pa_q[0]-done_q[0],pa_q[1]-done_q[1]);
endmodule
''')
(OUT/'compact_pretl_owner_fragment.sv').write_text(raw)

for name in ['early_actual_dma_top.sv', 'early_banked_top.sv', 'early_system_top.sv']:
    s=(BASE/name).read_text()
    s=replace_once(s, 'input logic reuse_enable_i, reuse_prepare_i,', 'input logic reuse_enable_i, reuse_prepare_i,\n    input logic [1:0] prepare_order_i,')
    if name != 'early_banked_top.sv':
        s=replace_once(s, '.reuse_enable_i(reuse_enable_i), .reuse_prepare_i(reuse_prepare_i),', '.reuse_enable_i(reuse_enable_i), .reuse_prepare_i(reuse_prepare_i), .prepare_order_i,')
    if name == 'early_actual_dma_top.sv':
        s=replace_once(s, 'pretl_owner_fragment #(.ParentDepth(1)', 'pretl_owner_fragment #(.ParentDepth(4)')
        begin=s.index('    logic [1:0] reuse_parent_valid,')
        end=s.index('    // The unchanged joint harness', begin)
        s=s[:begin]+'''    logic [1:0][3:0] reuse_parent_valid, reuse_parent_bank, reuse_parent_pid_valid, reuse_parent_priv;
    logic [1:0][3:0][63:0] reuse_parent_addr;
    logic [1:0][3:0][23:0] reuse_parent_did;
    logic [1:0][3:0][19:0] reuse_parent_pid;
    logic [1:0][3:0][31:0] reuse_parent_seq;
    logic [1:0][31:0] reuse_parent_head_seq;
    logic reuse_whead_valid, reuse_whead_owner;
    assign reuse_whead_valid = i_pre.vw_q > i_pre.vr_q && i_pre.pw_q > i_pre.pr_q &&
        i_pre.v_owner_q[i_pre.vr_q[2:0]] == i_pre.p_owner_q[i_pre.pr_q[2:0]] &&
        i_pre.v_seq_q[i_pre.vr_q[2:0]] == i_pre.p_seq_q[i_pre.pr_q[2:0]];
    assign reuse_whead_owner = i_pre.v_owner_q[i_pre.vr_q[2:0]];
    for (genvar f=0; f<2; f++) begin : gen_reuse_parent
        assign reuse_parent_head_seq[f] = i_pre.done_q[f];
        for (genvar n=0; n<4; n++) begin : gen_slot
            assign reuse_parent_seq[f][n] = i_pre.done_q[f] + ((32'(n)+4-(i_pre.done_q[f]%4))%4);
            assign reuse_parent_valid[f][n] = i_pre.active_q[f] && reuse_parent_seq[f][n] < i_pre.pa_q[f] &&
                i_pre.parent_q[f][n].len == 255 && i_pre.parent_q[f][n].size == 3 &&
                i_pre.parent_q[f][n].burst == 1 && !i_pre.parent_q[f][n].lock &&
                i_pre.parent_q[f][n].addr[11:0] <= 2048 && !i_pre.parent_q[f][n].prot[2] &&
                !(i_pre.parent_q[f][n].mmu_atst && !i_pre.parent_q[f][n].mmu_valid);
            assign reuse_parent_bank[f][n] = i_pre.parent_q[f][n].id[SameBankDmasEnable ? 3 : 2];
            assign reuse_parent_addr[f][n] = i_pre.parent_q[f][n].addr;
            assign reuse_parent_did[f][n] = i_pre.parent_q[f][n].stream_id;
            assign reuse_parent_pid[f][n] = i_pre.parent_q[f][n].substream_id;
            assign reuse_parent_pid_valid[f][n] = i_pre.parent_q[f][n].ss_id_valid;
            assign reuse_parent_priv[f][n] = i_pre.parent_q[f][n].prot[0];
        end
    end

'''+s[end:]
        s=replace_once(s, '.reuse_parent_seq_i(reuse_parent_seq),', '.reuse_parent_seq_i(reuse_parent_seq), .reuse_parent_head_seq_i(reuse_parent_head_seq),\n        .reuse_whead_valid_i(reuse_whead_valid), .reuse_whead_owner_i(reuse_whead_owner),')
    elif name == 'early_banked_top.sv':
        # Expand only the parent ports, not other flow/bank vectors.
        for suffix in ['valid_i, reuse_parent_bank_i','pid_valid_i, reuse_parent_priv_i']:
            s=replace_once(s, 'input logic [1:0] reuse_parent_'+suffix, 'input logic [1:0][3:0] reuse_parent_'+suffix)
        for width, field in [(63,'addr'),(23,'did'),(19,'pid'),(31,'seq')]:
            s=replace_once(s, f'input logic [1:0][{width}:0] reuse_parent_{field}_i,', f'input logic [1:0][3:0][{width}:0] reuse_parent_{field}_i,')
        s=replace_once(s, '    input logic [1:0][3:0][31:0] reuse_parent_seq_i,', '    input logic [1:0][3:0][31:0] reuse_parent_seq_i,\n    input logic [1:0][31:0] reuse_parent_head_seq_i,\n    input logic reuse_whead_valid_i, reuse_whead_owner_i,')
        s=replace_once(s, '.clk_i, .rst_ni, .enable_i(reuse_enable_i), .prepare_i(reuse_prepare_i),', '.clk_i, .rst_ni, .enable_i(reuse_enable_i), .prepare_i(reuse_prepare_i), .prepare_order_i,')
        s=replace_once(s, '.parent_seq_i(reuse_parent_seq_i),', '.parent_seq_i(reuse_parent_seq_i), .parent_head_seq_i(reuse_parent_head_seq_i),\n        .whead_valid_i(reuse_whead_valid_i), .whead_owner_i(reuse_whead_owner_i),')
    (OUT/name.replace('early_', 'multi_')).write_text(s)

s=(BASE/'early_test.cpp').read_text()
s=replace_once(s, 'static bool task_parent_reuse_enabled = true;', 'static unsigned task_prepare_order = 0;\nstatic bool task_parent_reuse_enabled = true;')
s=replace_once(s, 'top->reuse_prepare_i = task_parent_prepare_enabled;', 'top->reuse_prepare_i = task_parent_prepare_enabled; top->prepare_order_i = task_prepare_order;')
s=replace_once(s, '        if (const char* v = val("reuse")) {', '''        if (const char* v = val("prepare-order")) {
            const std::string setting(v);
            if (setting != "0" && setting != "1" && setting != "2") {
                std::fprintf(stderr, "prepare order must be 0|1|2\\n"); return 2;
            }
            task_prepare_order = unsigned(setting[0]-'0');
        }
        if (const char* v = val("reuse")) {''')
(OUT/'multi_test.cpp').write_text(s)

argv=json.loads((BASE/'build-early-command.json').read_text())
replacements={str(BASE/f'early_{n}'):str(OUT/f'multi_{n}') for n in ['actual_dma_top.sv','banked_top.sv','system_top.sv','test.cpp']}
replacements[str(BASE/'early_parent_translation.sv')]=str(OUT/'multi_parent_translation.sv')
replacements[str(ROOT/'out/fragment-placement-20261008/pretl_owner_fragment.sv')]=str(OUT/'compact_pretl_owner_fragment.sv')
replacements[str(BASE/'obj-early')]=str(OUT/'obj-multi')
replacements['-GParentDepth=1']='-GParentDepth=4'
argv=[replacements.get(x,x) for x in argv]
(OUT/'build-command.json').write_text(json.dumps(argv,indent=2)+'\n')
print('Created six out-only source variants and one build argv.')
