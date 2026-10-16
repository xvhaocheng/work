"""Codex-authored test-only RTL control edits, applied once to §39 copies."""
from pathlib import Path
import shutil

base = Path(__file__).resolve().parent
old = base.parent / 'prepared-source-bypass-20261008'
names = ('multi_actual_dma_top.sv', 'multi_system_top.sv',
         'compact_pretl_owner_fragment.sv', 'multi_banked_top.sv',
         'hit_bypass_parent_translation.sv', 'multi_test.cpp')
for name in names:
    if (base / name).exists():
        raise RuntimeError(f'refuse overwriting existing {name}')
    shutil.copyfile(old / name, base / name)

def replace(name, before, after):
    path = base / name
    text = path.read_text()
    if text.count(before) != 1:
        raise RuntimeError(f'{name}: expected one match for {before[:60]!r}')
    path.write_text(text.replace(before, after))

ports = '''
    // §42 runtime diagnostic inputs; old obligations are never cancelled.
    input logic pause_a_i, pause_reads_a_i,
    output logic [31:0] dbg_pause_a_offers, dbg_pause_a_wlast,
        dbg_pause_a_bpending, dbg_pause_a_children, dbg_pause_a_parents,
    output logic [63:0] dbg_pause_a_end, dbg_pause_a_rbytes,
        dbg_pause_a_wbytes, dbg_pause_a_childbytes,
        dbg_pause_v_rbytes, dbg_pause_v_wbytes,
    output logic dbg_pause_a_held, dbg_pause_a_read_busy,
        dbg_pause_a_prepare_busy, dbg_pause_a_quiet,
'''
for name in ('multi_actual_dma_top.sv', 'multi_system_top.sv'):
    replace(name, '    input logic [1:0] prepare_order_i, physical_bypass_i,',
            '    input logic [1:0] prepare_order_i, physical_bypass_i,\n' + ports)
replace('multi_system_top.sv', '    ) i_dma (\n', '''    ) i_dma (
        .pause_a_i, .pause_reads_a_i,
        .dbg_pause_a_offers, .dbg_pause_a_wlast, .dbg_pause_a_bpending,
        .dbg_pause_a_children, .dbg_pause_a_parents, .dbg_pause_a_end,
        .dbg_pause_a_rbytes, .dbg_pause_a_wbytes, .dbg_pause_a_childbytes,
        .dbg_pause_v_rbytes, .dbg_pause_v_wbytes,
        .dbg_pause_a_held, .dbg_pause_a_read_busy,
        .dbg_pause_a_prepare_busy, .dbg_pause_a_quiet,
''')
replace('compact_pretl_owner_fragment.sv', '    input logic [1:0] physical_bypass_i,',
        '    input logic pause_reads_a_i,\n    input logic [1:0] physical_bypass_i,')
replace('compact_pretl_owner_fragment.sv',
        '            if (!offered_q && !busy_q && candidate_valid) begin',
        '''            // Preserve every previously offered/accepted AR and its R.
            // Only the creation of the next A child is blocked in mode1.
            if (!offered_q && !busy_q && candidate_valid &&
                !(pause_reads_a_i && candidate_owner)) begin''')
replace('multi_actual_dma_top.sv', '        .physical_bypass_i, .cache_valid_i(reuse_cache_valid),',
        '        .pause_reads_a_i, .physical_bypass_i, .cache_valid_i(reuse_cache_valid),')

replace('multi_actual_dma_top.sv', '''    logic [31:0] pre_parent_count, pre_child_count, pre_merged, pre_lasts;
    always_comb begin''', '''    logic [31:0] pre_parent_count, pre_child_count, pre_merged, pre_lasts;

    // Functional state: held bit plus 64-bit A frontier for raw/prep eligibility.
    // The full saved header and remaining counters are observation-only.
    logic pause_aw_held_q, pause_aw_allow;
    axi_tr2_aw_chan_t pause_aw_saved_q;
    logic [31:0] pause_a_vwlast_q;
    logic [1:0] pause_a_b_event;
    logic [1:0] pause_raw_eligible;
    assign pause_aw_allow = !pause_a_i || pause_aw_held_q;
    assign dbg_pause_a_held = pause_aw_held_q;
    assign dbg_pause_a_offers = i_pre.awseq_q[1];
    assign dbg_pause_a_parents = i_pre.pa_q[1];
    assign dbg_pause_a_read_busy = (i_pre.offered_q || i_pre.busy_q) &&
        i_pre.child_owner_q;
    assign dbg_pause_a_prepare_busy = i_sys.i_reuse.slow_owner_q &&
        (i_sys.i_reuse.prep_offer_q ||
         (i_sys.i_reuse.slow_q && i_sys.i_reuse.slow_prepare_q));
    assign dbg_pause_a_quiet = !pause_aw_held_q &&
        dbg_pause_a_offers == dbg_pause_a_wlast &&
        dbg_pause_a_offers == pause_a_vwlast_q &&
        dbg_pause_a_bpending == 0 && !dbg_pause_a_read_busy &&
        !dbg_pause_a_prepare_busy;
    assign pause_a_b_event = {
        comp_aw_valid && comp_aw_ready && comp_aw_id[2],
        comp_b_valid && comp_b_ready && comp_b_id[2]};
    assign pause_raw_eligible[0] = 1'b1;
    assign pause_raw_eligible[1] = !pause_a_i ||
        (pool_m_req[1].ar.addr >= i_pre.src_q[1] &&
         pool_m_req[1].ar.addr-i_pre.src_q[1] < dbg_pause_a_end);

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            pause_aw_held_q <= 0; pause_aw_saved_q <= '0;
            dbg_pause_a_wlast <= 0; pause_a_vwlast_q <= 0;
            dbg_pause_a_bpending <= 0; dbg_pause_a_children <= 0;
            dbg_pause_a_end <= 0; dbg_pause_a_rbytes <= 0;
            dbg_pause_a_wbytes <= 0; dbg_pause_a_childbytes <= 0;
            dbg_pause_v_rbytes <= 0; dbg_pause_v_wbytes <= 0;
        end else begin
            if (pause_aw_held_q &&
                (!slv_reqs[1].aw_valid || slv_reqs[1].aw != pause_aw_saved_q))
                $fatal(1,"PAUSE42 changed previously visible A AW");
            pause_aw_held_q <= slv_reqs[1].aw_valid && !slv_resps[1].aw_ready;
            if (slv_reqs[1].aw_valid && !slv_resps[1].aw_ready)
                pause_aw_saved_q <= slv_reqs[1].aw;
            if (!pause_aw_allow && pool_m_rsp[1].aw_ready)
                $fatal(1,"PAUSE42 returned AWREADY while new A AW is blocked");
            if (i_dma_iommu_tr_mux.gen_mux.w_fifo_push &&
                i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2])
                dbg_pause_a_end <= i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.addr -
                    i_pre.dst_q[1] +
                    (64'(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.len)+1)*8;
            if (tr_w_valid && tr_w_ready && tr_w_last &&
                i_pre.v_owner_q[i_pre.vr_q[2:0]])
                pause_a_vwlast_q <= pause_a_vwlast_q+1;
            if (comp_w_valid && comp_w_ready) begin
                if (i_pre.p_owner_q[i_pre.pr_q[2:0]]) begin
                    dbg_pause_a_wbytes <= dbg_pause_a_wbytes+8;
                    if (comp_w_last) dbg_pause_a_wlast <= dbg_pause_a_wlast+1;
                end else dbg_pause_v_wbytes <= dbg_pause_v_wbytes+8;
            end
            case (pause_a_b_event)
                2'b10: dbg_pause_a_bpending <= dbg_pause_a_bpending+1;
                2'b01: begin
                    if (dbg_pause_a_bpending == 0)
                        $fatal(1,"PAUSE42 A B without accepted physical AW");
                    dbg_pause_a_bpending <= dbg_pause_a_bpending-1;
                end
                default: ;
            endcase
            if (pre_raw_r_valid && pre_r_ready) begin
                if (i_pre.child_owner_q) dbg_pause_a_rbytes <= dbg_pause_a_rbytes+8;
                else dbg_pause_v_rbytes <= dbg_pause_v_rbytes+8;
            end
            if (!i_pre.offered_q && !i_pre.busy_q && i_pre.candidate_valid &&
                i_pre.candidate_owner && !pause_reads_a_i) begin
                dbg_pause_a_children <= dbg_pause_a_children+1;
                dbg_pause_a_childbytes <= dbg_pause_a_childbytes +
                    64'(i_pre.candidate_beats)*8;
            end
            if (dbg_pause_a_wlast > dbg_pause_a_offers ||
                dbg_pause_a_rbytes > dbg_pause_a_childbytes ||
                dbg_pause_a_wbytes > dbg_pause_a_rbytes)
                $fatal(1,"PAUSE42 source/write obligation conservation failed");
        end
    end

    always_comb begin''')
replace('multi_actual_dma_top.sv',
        '        pre_owner = !(pool_m_req[0].ar_valid && pre_flow_ready[0]);\n        pre_parent_valid = pool_m_req[pre_owner].ar_valid && pre_flow_ready[pre_owner];',
        '''        pre_owner = !(pool_m_req[0].ar_valid && pre_flow_ready[0] && pause_raw_eligible[0]);
        pre_parent_valid = pool_m_req[pre_owner].ar_valid && pre_flow_ready[pre_owner] &&
            pause_raw_eligible[pre_owner];''')
replace('multi_actual_dma_top.sv', '        pool_m_rsp = {slv_resps[1], slv_resps[0]};',
        '''        pool_m_rsp = {slv_resps[1], slv_resps[0]};
        pool_m_rsp[1].aw_ready = slv_resps[1].aw_ready && pause_aw_allow;''')
replace('multi_actual_dma_top.sv', '        slv_reqs[0].ar_valid = 0; slv_reqs[1].ar_valid = 0;',
        '''        slv_reqs[0].ar_valid = 0; slv_reqs[1].ar_valid = 0;
        slv_reqs[1].aw_valid = pool_m_req[1].aw_valid && pause_aw_allow;''')

replace('hit_bypass_parent_translation.sv', '    input logic [1:0][Depth-1:0] parent_valid_i, parent_bank_i,',
        '''    // Stop only NEW unrelated preparations; preserve cache/in-flight lifetime.
    input logic [1:0][Depth-1:0] prepare_eligible_i,
    input logic [1:0][Depth-1:0] parent_valid_i, parent_bank_i,''')
replace('hit_bypass_parent_translation.sv', '                parent_valid_i[f][n] && !valid_q[f][n] &&',
        '                parent_valid_i[f][n] && prepare_eligible_i[f][n] && !valid_q[f][n] &&')
replace('multi_banked_top.sv', '    input logic [1:0][3:0] reuse_parent_valid_i, reuse_parent_bank_i,',
        '''    input logic [1:0][3:0] reuse_prepare_eligible_i,
    input logic [1:0][3:0] reuse_parent_valid_i, reuse_parent_bank_i,''')
replace('multi_banked_top.sv', '        .parent_valid_i(reuse_parent_valid_i),',
        '        .prepare_eligible_i(reuse_prepare_eligible_i), .parent_valid_i(reuse_parent_valid_i),')
replace('multi_actual_dma_top.sv', '    logic [1:0][31:0] reuse_parent_head_seq;',
        '    logic [1:0][3:0] reuse_prepare_eligible;\n    logic [1:0][31:0] reuse_parent_head_seq;')
replace('multi_actual_dma_top.sv', '            assign direct_eligible[f][n] = reuse_parent_valid[f][n]',
        '''            assign reuse_prepare_eligible[f][n] = !pause_a_i || f == 0 ||
                (i_pre.parent_q[f][n].addr >= i_pre.src_q[f] &&
                 i_pre.parent_q[f][n].addr-i_pre.src_q[f] < dbg_pause_a_end);
            assign direct_eligible[f][n] = reuse_parent_valid[f][n]''')
replace('multi_actual_dma_top.sv', '        .reuse_parent_valid_i(reuse_parent_valid),',
        '        .reuse_prepare_eligible_i(reuse_prepare_eligible), .reuse_parent_valid_i(reuse_parent_valid),')
print('Copied six files; root control applied to five SV files; C++ remains §39 until reviewed worker patch.')
