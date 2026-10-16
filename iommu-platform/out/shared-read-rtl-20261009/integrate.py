import json, pathlib, shutil
p = pathlib.Path(__file__).resolve().parent
old = p.parent / 'dual-write-prefix-20261008'
argv = json.loads((old/'build-argv.json').read_text())
for name in ('hit_bypass_parent_translation.sv', 'multi_banked_top.sv',
             'compact_pretl_owner_fragment.sv', 'multi_actual_dma_top.sv',
             'multi_system_top.sv', 'multi_test.cpp'):
    shutil.copy2(old/name, p/name)
argv = [x.replace(str(old), str(p)).replace('-GSameBankDmasEnable=0', '-GSameBankDmasEnable=1') for x in argv]
argv.insert(-1, str(p/'q_write_splitter.sv'))
argv.insert(-1, str(p/'shared_pretl_read.sv'))
(p/'build-argv.json').write_text(json.dumps(argv, indent=2)+'\n')
def change(text, old, new):
    assert text.count(old)==1, (old[:100], text.count(old))
    return text.replace(old,new)
t=(p/'multi_actual_dma_top.sv').read_text()
t=change(t,'tc_comp_ar_id!=0 && tc_comp_ar_id!=4','tc_comp_ar_id!=0 && tc_comp_ar_id!=4 && tc_comp_ar_id!=6')
start=t.index('    // Functional state: held bit plus 64-bit A frontier')
end=t.index('    always_comb begin\n        pre_owner',start)
t=t[:start]+'''    // §50: functional pause is in the two q writers and shared source owner.
    logic [1:0] q_quiet, q_held;
    logic [1:0][31:0] q_groups, q_offers, q_pending;
    logic source_detached, source_a_hold, source_a_skid;
    logic [1:0][63:0] source_returned, source_coverage;
    assign dbg_pause_a_held = q_held[1];
    assign dbg_pause_a_offers = q_offers[1];
    assign dbg_pause_a_parents = i_pre.pa_q[1];
    assign dbg_pause_a_bpending = q_pending[1];
    assign dbg_pause_a_end = source_coverage[1];
    assign dbg_pause_a_rbytes = source_returned[1];
    assign dbg_pause_v_rbytes = source_returned[0];
    assign dbg_pause_a_read_busy = source_a_skid;
    assign dbg_pause_a_prepare_busy = i_sys.i_reuse.slow_owner_q &&
        (i_sys.i_reuse.prep_offer_q || (i_sys.i_reuse.slow_q && i_sys.i_reuse.slow_prepare_q));
    assign dbg_pause_a_quiet = source_detached && q_quiet[1] && !source_a_skid &&
        !dbg_pause_a_prepare_busy;
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            dbg_pause_a_wlast <= 0; dbg_pause_a_wbytes <= 0; dbg_pause_v_wbytes <= 0;
        end else if (comp_w_valid && comp_w_ready) begin
            if (physical_stream_order_q[physical_stream_rd_q]) begin
                dbg_pause_a_wbytes <= dbg_pause_a_wbytes+8;
                if (comp_w_last) dbg_pause_a_wlast <= dbg_pause_a_wlast+1;
            end else dbg_pause_v_wbytes <= dbg_pause_v_wbytes+8;
        end
    end

'''+t[end:]
t=change(t,'pre_owner = !(pool_m_req[0].ar_valid && pre_flow_ready[0] && pause_raw_eligible[0]);','pre_owner = !(pool_m_req[0].ar_valid && pre_flow_ready[0]);')
t=change(t,'pre_parent_valid = pool_m_req[pre_owner].ar_valid && pre_flow_ready[pre_owner] &&\n            pause_raw_eligible[pre_owner];','pre_parent_valid = pool_m_req[pre_owner].ar_valid && pre_flow_ready[pre_owner];')
t=change(t,'        pool_m_rsp[1].aw_ready = slv_resps[1].aw_ready && pause_aw_allow;\n','')
t=change(t,'        slv_reqs[1].aw_valid = pool_m_req[1].aw_valid && pause_aw_allow;\n','')
t=change(t,'''        assign pool_m_req = pool_s_req;
        assign pool_s_rsp = pool_m_rsp;''','''        for (genvar f=0; f<2; f++) begin : gen_q_writer
            q_write_splitter #(.Q(2), .req_t(axi_tr2_req_t), .rsp_t(axi_tr2_resp_t),
                .aw_t(axi_tr2_aw_chan_t)) i_q (
                .clk_i, .rst_ni, .hold_i(f==1 && source_a_hold), .detached_i(source_detached),
                .peer_groups_i(q_groups[1-f]), .s_req_i(pool_s_req[f]), .s_rsp_o(pool_s_rsp[f]),
                .m_req_o(pool_m_req[f]), .m_rsp_i(pool_m_rsp[f]),
                .quiet_o(q_quiet[f]), .held_o(q_held[f]), .groups_o(q_groups[f]),
                .offers_o(q_offers[f]), .pending_o(q_pending[f]));
        end''')
start=t.index('    pretl_owner_fragment #(')
end=t.index('    initial $display("PRETL-DECL',start)
t=t[:start]+'''    shared_pretl_read #(.ParentDepth(4), .ar_t(axi_tr4_ar_chan_t), .r_t(axi_tr4_r_chan_t)) i_pre (
        .clk_i, .rst_ni, .pause_i(pause_a_i), .mode_i(physical_bypass_i),
        .a_write_quiet_i(q_quiet[1]), .detached_o(source_detached), .a_hold_o(source_a_hold),
        .flow_ready_o(pre_flow_ready), .cache_valid_i(reuse_cache_valid),
        .cache_failed_i(reuse_cache_failed), .cache_pa_i(reuse_cache_pa), .child_direct_o(pre_direct),
        .s_ar_valid_i(pre_parent_valid), .s_ar_ready_o(pre_parent_ready), .s_ar_i(pre_parent_ar),
        .m_ar_valid_o(pre_ar_valid), .m_ar_ready_i(pre_ar_ready), .m_ar_o(pre_ar),
        .m_r_valid_i(pre_raw_r_valid), .m_r_ready_o(pre_r_ready), .m_r_i(pre_raw_r),
        .s_r_valid_o(pre_r_valid), .s_r_ready_i(mux_mst_req.r_ready), .s_r_o(pre_r),
        .job_fire_i({i_dma1.be_valid_d && i_dma1.be_ready_d, i_dma0.be_valid_d && i_dma0.be_ready_d}),
        .job_done_i({i_dma1.idma_rsp_valid && i_dma1.idma_rsp_ready, i_dma0.idma_rsp_valid && i_dma0.idma_rsp_ready}),
        .job_normal_i({i_dma1.ttype == idma_pkg::DEFAULT, i_dma0.ttype == idma_pkg::DEFAULT}),
        .job_src_i({64'(i_dma1.burst_req_d.src_addr), 64'(i_dma0.burst_req_d.src_addr)}),
        .job_dst_i({64'(i_dma1.burst_req_d.dst_addr), 64'(i_dma0.burst_req_d.dst_addr)}),
        .job_length_i({64'(i_dma1.burst_req_d.length), 64'(i_dma0.burst_req_d.length)}),
        .v_aw_offer_i(i_dma_iommu_tr_mux.gen_mux.w_fifo_push),
        .v_aw_owner_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2]),
        .v_aw_addr_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.addr),
        .v_aw_len_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.len),
        .v_aw_size_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.size),
        .returned_bytes_o(source_returned), .coverage_o(source_coverage), .a_skid_o(source_a_skid),
        .a_child_count_o(dbg_pause_a_children), .a_child_bytes_o(dbg_pause_a_childbytes),
        .parent_count_o(pre_parent_count), .child_count_o(pre_child_count),
        .merged_count_o(pre_merged), .parent_last_count_o(pre_lasts)
    );
'''+t[end:]
start=t.index('    assign reuse_whead_valid =')
end=t.index('    for (genvar f=0; f<2; f++) begin : gen_reuse_parent',start)
t=t[:start]+'''    assign reuse_whead_valid = !i_dma_iommu_tr_mux.gen_mux.w_fifo_empty;
    assign reuse_whead_owner = i_dma_iommu_tr_mux.gen_mux.w_fifo_data[0];
'''+t[end:]
# Read-only permission preparation is allowed for both already registered parents.
t=change(t,'''            assign reuse_prepare_eligible[f][n] = !pause_a_i || f == 0 ||
                (i_pre.parent_q[f][n].addr >= i_pre.src_q[f] &&
                 i_pre.parent_q[f][n].addr-i_pre.src_q[f] < dbg_pause_a_end);''',
'''            assign reuse_prepare_eligible[f][n] = 1'b1;''')
(p/'multi_actual_dma_top.sv').write_text(t)
# Explicit fixed normal memory fixture; both alternatives identical attributes.
t=(p/'multi_banked_top.sv').read_text()
t=change(t,"        tr_req.aw.burst = tr_aw_burst;", "        tr_req.aw.burst = tr_aw_burst;\n        tr_req.aw.cache = 4'b0010; // §50 bounded CONF129 fixture")
t=change(t,"        tr_req.ar.burst = tr_ar_burst;", "        tr_req.ar.burst = tr_ar_burst;\n        tr_req.ar.cache = 4'b0010;")
(p/'multi_banked_top.sv').write_text(t)
t=(p/'multi_system_top.sv').read_text()
t=change(t,'        comp_req.aw.user  = u_comp_aw_user;', '''        comp_req.aw.user  = u_comp_aw_user;
        comp_req.aw.cache = i_dma.i_sys.comp_req.aw.cache;
        comp_req.aw.lock = i_dma.i_sys.comp_req.aw.lock;
        comp_req.aw.qos = i_dma.i_sys.comp_req.aw.qos;
        comp_req.aw.region = i_dma.i_sys.comp_req.aw.region;''')
(p/'multi_system_top.sv').write_text(t)
t=(p/'multi_test.cpp').read_text()
for r in json.loads((p/'deepseek-test-proposal.json').read_text())['replacements']:
    t=change(t,r['old'],r['new'])
(p/'multi_test.cpp').write_text(t)
print('copied scoped wrappers, applied reviewed mapping, integrated q writers and shared source')
