"""Codex critical integration: pre-TL fixed fragmentation, per-DMA parents.

Derived only in out/. Preserve real DMA CONF1/AW mux/TC/TL/DDR paths.
The pre-TL parent ingress bypasses shared AR arbitration; AW and W do not.
"""
from pathlib import Path
import json

ROOT=Path('/home/xvhaocheng/work/iommu-platform')
OUT=ROOT/'out/fragment-placement-20261008'
OLD=ROOT/'out/parent-capacity-20261008'
OUT.mkdir(parents=True,exist_ok=True)

def one(s,a,b):
    if s.count(a)!=1: raise RuntimeError(f'expected one interface: {a!r}, got {s.count(a)}')
    return s.replace(a,b,1)

# Reuse the exact fixed service / dual-W-order / original RLAST logic.
s=(OLD/'posttl_owner_fragment.sv').read_text()
s=one(s,'module posttl_owner_fragment #(', 'module pretl_owner_fragment #(')
s=one(s,'parameter int unsigned ParentDepth = 8', 'parameter int unsigned ParentDepth = 1')
s=one(s,'    input logic s_ar_valid_i,', '    output logic [1:0] flow_ready_o,\n    input logic s_ar_valid_i,')
s=one(s,'    logic [63:0] offset_q [2][ParentDepth], virtual_offset_q [2][D];',
      '    logic [63:0] offset_q [2][ParentDepth];')
s=one(s,'    logic [7:0] virtual_len_q [2][D];\n','')
s=one(s,'    logic [31:0] va_q [2], pa_q [2], done_q [2], awseq_q [2], paseq_q [2];',
      '    logic [31:0] pa_q [2], done_q [2], awseq_q [2], paseq_q [2];')
s=one(s,'    always_comb begin\n        s_ar_ready_o',
      '    always_comb begin\n        for (int f=0; f<2; f++)\n            flow_ready_o[f] = active_q[f] && pa_q[f]-done_q[f] < ParentDepth;\n        s_ar_ready_o')
s=s.replace('va_q[s] <= 0; pa_q[s] <= 0;', 'pa_q[s] <= 0;')
s=one(s,'                    virtual_offset_q[s][n] <= 0; virtual_len_q[s][n] <= 0;\n','')
a=s.index('            if (cycle_q[13:0] == 0 && cycle_q != 0 &&')
b=s.index('            if (vw_q < vr_q',a)
s=s[:a]+s[b:]
s=one(s,'if (va_q[s] < pa_q[s] || pa_q[s] < done_q[s] ||\n                    va_q[s]-pa_q[s] > D || pa_q[s]-done_q[s] > ParentDepth)',
      'if (pa_q[s] < done_q[s] || pa_q[s]-done_q[s] > ParentDepth)')
s=one(s,'!active_q[s] || va_q[s] != done_q[s]', '!active_q[s] || pa_q[s] != done_q[s]')
a=s.index('            if (v_ar_fire_i) begin')
b=s.index('            if (s_ar_valid_i && s_ar_ready_o)',a)
s=s[:a]+s[b:]
s=one(s,'                int s, n, vn;', '                int s, n;')
s=one(s,"                vn = int'(pa_q[s][2:0]);\n",'')
s=one(s,'if (va_q[s] <= pa_q[s] || s_ar_i.id != (s == 0 ? 4\'d0 : 4\'d4) ||\n                    s_ar_i.len != virtual_len_q[s][vn] ||',
      'if (s_ar_i.id != (s == 0 ? 4\'d0 : 4\'d4) ||\n                    s_ar_i.addr < src_q[s] ||\n                    s_ar_i.addr-src_q[s]+(64\'(s_ar_i.len)+1)*8 > length_q[s] ||')
s=one(s,'offset_q[s][n] <= virtual_offset_q[s][vn];', 'offset_q[s][n] <= s_ar_i.addr-src_q[s];')
s=one(s,'// Static contiguous descriptors, two live flows, 8 parents per flow, one child',
      '// Pre-TL static contiguous descriptors, two live flows, one parent each, one child')
(OUT/'pretl_owner_fragment.sv').write_text(s)

# Parents are accepted from each real DMA before shared AR arbitration. The
# original shared mux still carries all AW/W/B and demultiplexes restored R.
t=(ROOT/'tests/nonblock_actual_dma_top.sv').read_text()
t=one(t,'    assign pool_m_rsp = {slv_resps[1], slv_resps[0]};', '''    logic [1:0] pre_flow_ready;
    logic pre_owner, pre_parent_valid, pre_parent_ready;
    logic pre_ar_valid, pre_r_ready, pre_r_valid;
    axi_tr4_ar_chan_t pre_parent_ar, pre_ar;
    axi_tr4_r_chan_t pre_raw_r, pre_r;
    logic [31:0] pre_parent_count, pre_child_count, pre_merged, pre_lasts;
    always_comb begin
        pre_owner = !(pool_m_req[0].ar_valid && pre_flow_ready[0]);
        pre_parent_valid = pool_m_req[pre_owner].ar_valid && pre_flow_ready[pre_owner];
        pre_parent_ar = axi_tr4_ar_chan_t'(pool_m_req[pre_owner].ar);
        pre_parent_ar.id = {1'b0, pre_owner, pool_m_req[pre_owner].ar.id};
        pool_m_rsp = {slv_resps[1], slv_resps[0]};
        pool_m_rsp[0].ar_ready = pre_parent_valid && pre_parent_ready && !pre_owner;
        pool_m_rsp[1].ar_ready = pre_parent_valid && pre_parent_ready && pre_owner;
    end''')
t=one(t,'    assign slv_reqs[0] = pool_m_req[0];\n    assign slv_reqs[1] = pool_m_req[1];', '''    always_comb begin
        slv_reqs[0] = pool_m_req[0]; slv_reqs[1] = pool_m_req[1];
        slv_reqs[0].ar_valid = 0; slv_reqs[1].ar_valid = 0;
    end''')
# Prefix replacement applies once to the only mux-master AR unpack block.
t=t.replace('mux_mst_req.ar.', 'pre_ar.')
t=one(t,'tr_ar_valid      = mux_mst_req.ar_valid;', 'tr_ar_valid      = pre_ar_valid;')
t=one(t,'tr_r_ready       = mux_mst_req.r_ready;', 'tr_r_ready       = pre_r_ready;')
for a,b in [('tr_r_valid','pre_r_valid'),('tr_r_id','pre_r.id'),('tr_r_data','pre_r.data'),('tr_r_resp','pre_r.resp'),('tr_r_last','pre_r.last')]:
    key={'tr_r_valid':'mux_mst_resp.r_valid','tr_r_id':'mux_mst_resp.r.id','tr_r_data':'mux_mst_resp.r.data','tr_r_resp':'mux_mst_resp.r.resp','tr_r_last':'mux_mst_resp.r.last'}[a]
    import re
    pat=re.compile(r'('+re.escape(key)+r'\s*=\s*)'+a+r';')
    t,n=pat.subn(lambda m:m.group(1)+b+';',t)
    if n!=1: raise RuntimeError((a,n))

inst='''
    always_comb begin
        pre_raw_r = '0;
        pre_raw_r.id = tr_r_id; pre_raw_r.data = tr_r_data;
        pre_raw_r.resp = tr_r_resp; pre_raw_r.last = tr_r_last;
    end
    pretl_owner_fragment #(.ParentDepth(1), .ar_t(axi_tr4_ar_chan_t), .r_t(axi_tr4_r_chan_t)) i_pre (
        .clk_i, .rst_ni, .adaptive_i(1'b0), .flow_ready_o(pre_flow_ready),
        .s_ar_valid_i(pre_parent_valid), .s_ar_ready_o(pre_parent_ready), .s_ar_i(pre_parent_ar),
        .m_ar_valid_o(pre_ar_valid), .m_ar_ready_i(tr_ar_ready), .m_ar_o(pre_ar),
        .m_r_valid_i(tr_r_valid), .m_r_ready_o(pre_r_ready), .m_r_i(pre_raw_r),
        .s_r_valid_o(pre_r_valid), .s_r_ready_i(mux_mst_req.r_ready), .s_r_o(pre_r),
        .job_fire_i({i_dma1.be_valid_d && i_dma1.be_ready_d, i_dma0.be_valid_d && i_dma0.be_ready_d}),
        .job_done_i({i_dma1.idma_rsp_valid && i_dma1.idma_rsp_ready, i_dma0.idma_rsp_valid && i_dma0.idma_rsp_ready}),
        .job_normal_i({i_dma1.ttype == idma_pkg::DEFAULT, i_dma0.ttype == idma_pkg::DEFAULT}),
        .job_src_i({64'(i_dma1.burst_req_d.src_addr), 64'(i_dma0.burst_req_d.src_addr)}),
        .job_dst_i({64'(i_dma1.burst_req_d.dst_addr), 64'(i_dma0.burst_req_d.dst_addr)}),
        .job_length_i({64'(i_dma1.burst_req_d.length), 64'(i_dma0.burst_req_d.length)}),
        .v_ar_fire_i(1'b0), .v_ar_owner_i(1'b0), .v_ar_addr_i(64'd0), .v_ar_len_i(8'd0),
        .v_aw_offer_i(i_dma_iommu_tr_mux.gen_mux.w_fifo_push),
        .v_aw_owner_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2]),
        .v_aw_addr_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.addr),
        .v_aw_len_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.len),
        .v_aw_size_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.size),
        .v_w_fire_i(tr_w_valid && tr_w_ready), .v_wlast_i(tr_w_last),
        .p_aw_fire_i(comp_aw_valid && comp_aw_ready), .p_aw_owner_i(comp_aw_id[2]),
        .p_w_fire_i(comp_w_valid && comp_w_ready), .p_wlast_i(comp_w_last),
        .parent_count_o(pre_parent_count), .child_count_o(pre_child_count),
        .merged_count_o(pre_merged), .parent_last_count_o(pre_lasts)
    );
    initial $display("PRETL-DECL ar_bits=%0d r_bits=%0d", $bits(axi_tr4_ar_chan_t), $bits(axi_tr4_r_chan_t));
'''
t=one(t,'    // The unchanged joint harness (real banked CTL <-> real TL)',
      inst+'\n    // The unchanged joint harness (real banked CTL <-> real TL)')
(OUT/'pre_actual_dma_top.sv').write_text(t)

# Keep full completion AR packing from section 33. Only remove post-TL
# fragmentation: source children have already been split before the IOMMU.
t=(OLD/'system_top.sv').read_text()
a=t.index('    posttl_owner_fragment #(')
b=t.index('\n    nonblock_read_admission #(',a)
t=t[:a]+'''    assign fragment_ar_valid = u_comp_ar_valid;
    assign u_comp_ar_ready = fragment_ar_ready;
    assign fragment_ar_chan = u_comp_ar_chan;
    assign fragment_r_valid = slv_resps[2].r_valid;
    assign fragment_r_chan = slv_resps[2].r;
    assign fragment_r_ready = u_comp_r_ready;
    assign fragment_parents = i_dma.pre_parent_count;
    assign fragment_children = i_dma.pre_child_count;
    assign fragment_merged = i_dma.pre_merged;
    assign fragment_lasts = i_dma.pre_lasts;
'''+t[b:]
(OUT/'pre_system_top.sv').write_text(t)

c=ROOT/'out/posttl-independent-20261008/natural_fragment_test.cpp'
if not (OUT/'placement_test.cpp').exists(): (OUT/'placement_test.cpp').write_text(c.read_text())
cmd=json.loads((OLD/'build-depth1-command.json').read_text())
cmd[cmd.index('--Mdir')+1]=str(OUT/'obj-pre')
cmd[cmd.index(str(OLD/'posttl_owner_fragment.sv'))]=str(OUT/'pretl_owner_fragment.sv')
cmd[cmd.index(str(ROOT/'tests/nonblock_actual_dma_top.sv'))]=str(OUT/'pre_actual_dma_top.sv')
cmd[cmd.index(str(OLD/'system_top.sv'))]=str(OUT/'pre_system_top.sv')
# Module must be declared before the inner harness; order is explicit.
cmd.remove(str(OUT/'pretl_owner_fragment.sv'))
cmd.insert(cmd.index(str(OUT/'pre_actual_dma_top.sv')),str(OUT/'pretl_owner_fragment.sv'))
cmd[cmd.index(str(c))]=str(OUT/'placement_test.cpp')
(OUT/'build-pre-command.json').write_text(json.dumps(cmd,indent=2)+'\n')
