"""Codex: mechanical integration of the reviewed postTL RTL into out copies."""
import json
from pathlib import Path

ROOT = Path('/home/xvhaocheng/work/iommu-platform')
OUT = ROOT / 'out/posttl-independent-20261008'
OLD = ROOT / 'out/natural-write-order-20261008'

def replace_once(text, old, new):
    if text.count(old) != 1:
        raise RuntimeError(f'expected exactly one interface: {old[:100]!r}')
    return text.replace(old, new, 1)

t = (OLD / 'system_top.sv').read_text()
t = replace_once(t, '    slv_ar_chan_t proxy_ar_chan;', '''    slv_ar_chan_t proxy_ar_chan;
    logic fragment_ar_valid, fragment_ar_ready, fragment_r_ready;
    logic fragment_r_valid;
    slv_ar_chan_t fragment_ar_chan;
    slv_r_chan_t fragment_r_chan;
    logic [31:0] fragment_parents, fragment_children, fragment_merged, fragment_lasts;''')
t = replace_once(t, '        comp_req.r_ready  = u_comp_r_ready;', '        comp_req.r_ready  = fragment_r_ready;')
# All attributes are taken from the real completion struct, avoiding flattening.
a = t.index('        comp_req.ar.id    = proxy_ar_chan.id;')
b = t.index('        comp_req.w_valid', a)
t = t[:a] + '        comp_req.ar = proxy_ar_chan;\n' + t[b:]
a = t.index("        u_comp_ar_chan = '0;")
b = t.index('\n    end', a)
t = t[:a] + '        u_comp_ar_chan = i_dma.i_sys.comp_req.ar;' + t[b:]
for old, new in [
    ('.comp_r_valid  (slv_resps[2].r_valid)', '.comp_r_valid  (fragment_r_valid)'),
    ('.comp_r_id     (slv_resps[2].r.id)', '.comp_r_id     (fragment_r_chan.id)'),
    ('.comp_r_data   (slv_resps[2].r.data)', '.comp_r_data   (fragment_r_chan.data)'),
    ('.comp_r_resp   (slv_resps[2].r.resp)', '.comp_r_resp   (fragment_r_chan.resp)'),
    ('.comp_r_last   (slv_resps[2].r.last)', '.comp_r_last   (fragment_r_chan.last)'),
    ('.s_ar_valid (u_comp_ar_valid)', '.s_ar_valid (fragment_ar_valid)'),
    ('.s_ar_ready (u_comp_ar_ready)', '.s_ar_ready (fragment_ar_ready)'),
    ('.s_ar_chan  (u_comp_ar_chan)', '.s_ar_chan  (fragment_ar_chan)'),
    ('.mode_i   (read_admit_mode_i)', ".mode_i   (3'd0)"),
]:
    t = replace_once(t, old, new)
inst = '''
    posttl_owner_fragment #(.ar_t(slv_ar_chan_t), .r_t(slv_r_chan_t)) i_fragment (
        .clk_i, .rst_ni, .adaptive_i(read_admit_mode_i == 3'd1),
        .s_ar_valid_i(u_comp_ar_valid), .s_ar_ready_o(u_comp_ar_ready),
        .s_ar_i(u_comp_ar_chan), .m_ar_valid_o(fragment_ar_valid),
        .m_ar_ready_i(fragment_ar_ready), .m_ar_o(fragment_ar_chan),
        .m_r_valid_i(slv_resps[2].r_valid), .m_r_ready_o(fragment_r_ready),
        .m_r_i(slv_resps[2].r), .s_r_valid_o(fragment_r_valid),
        .s_r_ready_i(u_comp_r_ready), .s_r_o(fragment_r_chan),
        .job_fire_i({i_dma.i_dma1.be_valid_d && i_dma.i_dma1.be_ready_d,
                     i_dma.i_dma0.be_valid_d && i_dma.i_dma0.be_ready_d}),
        .job_done_i({i_dma.i_dma1.idma_rsp_valid && i_dma.i_dma1.idma_rsp_ready,
                     i_dma.i_dma0.idma_rsp_valid && i_dma.i_dma0.idma_rsp_ready}),
        .job_normal_i({i_dma.i_dma1.ttype == idma_pkg::DEFAULT,
                       i_dma.i_dma0.ttype == idma_pkg::DEFAULT}),
        .job_src_i({64'(i_dma.i_dma1.burst_req_d.src_addr), 64'(i_dma.i_dma0.burst_req_d.src_addr)}),
        .job_dst_i({64'(i_dma.i_dma1.burst_req_d.dst_addr), 64'(i_dma.i_dma0.burst_req_d.dst_addr)}),
        .job_length_i({64'(i_dma.i_dma1.burst_req_d.length), 64'(i_dma.i_dma0.burst_req_d.length)}),
        .v_ar_fire_i(i_dma.tr_ar_valid && i_dma.tr_ar_ready),
        .v_ar_owner_i(i_dma.tr_ar_id[2]), .v_ar_addr_i(i_dma.tr_ar_addr), .v_ar_len_i(i_dma.tr_ar_len),
        .v_aw_offer_i(i_dma.i_dma_iommu_tr_mux.gen_mux.w_fifo_push),
        .v_aw_owner_i(i_dma.i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2]),
        .v_aw_addr_i(i_dma.i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.addr),
        .v_aw_len_i(i_dma.i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.len),
        .v_aw_size_i(i_dma.i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.size),
        .v_w_fire_i(i_dma.tr_w_valid && i_dma.tr_w_ready),
        .v_wlast_i(i_dma.tr_w_last),
        .p_aw_fire_i(u_comp_aw_valid && slv_resps[2].aw_ready), .p_aw_owner_i(u_comp_aw_id[2]),
        .p_w_fire_i(u_comp_w_valid && slv_resps[2].w_ready), .p_wlast_i(u_comp_w_last),
        .parent_count_o(fragment_parents), .child_count_o(fragment_children),
        .merged_count_o(fragment_merged), .parent_last_count_o(fragment_lasts)
    );
'''
t = replace_once(t, '    nonblock_read_admission #(', inst + '\n    nonblock_read_admission #(')
# Existing top outputs are reused only as final statistics in this out model.
t = replace_once(t, '.dbg_frontier_grant(dbg_frontier_grant)', '.dbg_frontier_grant()')
t = replace_once(t, '.dbg_frontier_wait(dbg_frontier_wait)', '.dbg_frontier_wait()')
t = replace_once(t, '    assign dbg_ra_down_ready', '    assign dbg_frontier_grant = fragment_parents;\n    assign dbg_frontier_wait = fragment_children;\n    assign dbg_ra_down_ready')
(OUT / 'system_top.sv').write_text(t)

c = (OLD / 'natural_queue_test.cpp').read_text()
c = replace_once(c, '        if (read_admit != "none") {', '        if (read_admit != "none" && read_admit != "buffer") {')
c = replace_once(c, '        nat_report_json(beats);', '        std::printf("POSTTL-STATS parents=%u children=%u policy=%u\\n", (unsigned)top->dbg_frontier_grant, (unsigned)top->dbg_frontier_wait, (unsigned)read_admit_mode_val);\n        nat_report_json(beats);')
(OUT / 'natural_fragment_test.cpp').write_text(c)
cmd = json.loads((OLD / 'build-write-limit-command.json').read_text())
cmd[cmd.index('--Mdir') + 1] = str(OUT / 'obj-fragment')
cmd[cmd.index(str(OLD / 'system_top.sv'))] = str(OUT / 'system_top.sv')
cmd[cmd.index(str(OLD / 'natural_test.cpp'))] = str(OUT / 'natural_fragment_test.cpp')
cmd.insert(cmd.index(str(OUT / 'system_top.sv')), str(OUT / 'posttl_owner_fragment.sv'))
(OUT / 'build-command.json').write_text(json.dumps(cmd, indent=2) + '\n')
