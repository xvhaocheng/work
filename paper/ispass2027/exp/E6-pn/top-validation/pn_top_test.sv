// Test-only actual complete IOMMU top, real program AXI and host AXI memory.
`include "axi/typedef.svh"
`include "axi-iommu/typedef.svh"
module pn_top_test import rv_iommu::*; # (parameter bit Gold=1)(
output logic [63:0] test_config_o,test_iova_o,
output logic [15:0] test_state_o,
input logic clk_i,rst_ni,
input logic [1:0] mode_i,
input logic history_i,clear_i,
input logic [1:0] permit_i,
output logic quiet_o,nfire_o,
output logic [23:0] ndid_o,
output logic same_off_o,same_gold_o,
output logic [15:0] wsi_o,
input logic [23:0] t_did,
input logic t_aw_valid,
input logic [63:0] t_aw_addr,
input logic [3:0] t_aw_id,
input logic [7:0] t_aw_len,
input logic [2:0] t_aw_size,
input logic [5:0] t_aw_atop,
input logic t_w_valid,
input logic [63:0] t_w_data,
input logic [7:0] t_w_strb,
input logic t_w_last,
input logic t_b_ready,
input logic t_ar_valid,
input logic [63:0] t_ar_addr,
input logic [3:0] t_ar_id,
input logic [7:0] t_ar_len,
input logic [2:0] t_ar_size,
input logic t_r_ready,
output logic t_aw_ready,
output logic t_w_ready,
output logic t_b_valid,
output logic [1:0] t_b_resp,
output logic [3:0] t_b_id,
output logic t_ar_ready,
output logic t_r_valid,
output logic [1:0] t_r_resp,
output logic [3:0] t_r_id,
output logic t_r_last,
output logic [63:0] t_r_data,
input logic p_aw_valid,
input logic [63:0] p_aw_addr,
input logic [5:0] p_aw_id,
input logic [7:0] p_aw_len,
input logic [2:0] p_aw_size,
input logic [5:0] p_aw_atop,
input logic p_w_valid,
input logic [63:0] p_w_data,
input logic [7:0] p_w_strb,
input logic p_w_last,
input logic p_b_ready,
input logic p_ar_valid,
input logic [63:0] p_ar_addr,
input logic [5:0] p_ar_id,
input logic [7:0] p_ar_len,
input logic [2:0] p_ar_size,
input logic p_r_ready,
output logic p_aw_ready,
output logic p_w_ready,
output logic p_b_valid,
output logic [1:0] p_b_resp,
output logic [5:0] p_b_id,
output logic p_ar_ready,
output logic p_r_valid,
output logic [1:0] p_r_resp,
output logic [5:0] p_r_id,
output logic p_r_last,
output logic [63:0] p_r_data,
output logic c_aw_valid,
output logic [63:0] c_aw_addr,
output logic [3:0] c_aw_id,
output logic [7:0] c_aw_len,
output logic [2:0] c_aw_size,
output logic [5:0] c_aw_atop,
output logic c_w_valid,
output logic [63:0] c_w_data,
output logic [7:0] c_w_strb,
output logic c_w_last,
output logic c_b_ready,
output logic c_ar_valid,
output logic [63:0] c_ar_addr,
output logic [3:0] c_ar_id,
output logic [7:0] c_ar_len,
output logic [2:0] c_ar_size,
output logic c_r_ready,
input logic c_aw_ready,
input logic c_w_ready,
input logic c_b_valid,
input logic [1:0] c_b_resp,
input logic [3:0] c_b_id,
input logic c_ar_ready,
input logic c_r_valid,
input logic [1:0] c_r_resp,
input logic [3:0] c_r_id,
input logic c_r_last,
input logic [63:0] c_r_data,
output logic d_aw_valid,
output logic [63:0] d_aw_addr,
output logic [3:0] d_aw_id,
output logic [7:0] d_aw_len,
output logic [2:0] d_aw_size,
output logic [5:0] d_aw_atop,
output logic d_w_valid,
output logic [63:0] d_w_data,
output logic [7:0] d_w_strb,
output logic d_w_last,
output logic d_b_ready,
output logic d_ar_valid,
output logic [63:0] d_ar_addr,
output logic [3:0] d_ar_id,
output logic [7:0] d_ar_len,
output logic [2:0] d_ar_size,
output logic d_r_ready,
input logic d_aw_ready,
input logic d_w_ready,
input logic d_b_valid,
input logic [1:0] d_b_resp,
input logic [3:0] d_b_id,
input logic d_ar_ready,
input logic d_r_valid,
input logic [1:0] d_r_resp,
input logic [3:0] d_r_id,
input logic d_r_last,
input logic [63:0] d_r_data);
typedef logic [63:0] addr_t;
`AXI_TYPEDEF_ALL(a,addr_t,logic[3:0],logic[63:0],logic[7:0],logic[0:0])
`AXI_TYPEDEF_ALL(p,addr_t,logic[5:0],logic[63:0],logic[7:0],logic[0:0])
    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg = '{
        NumOutstandingTrans:8,WFifoDepth:32,MrifFifoDepth:8,FaultFifoDepth:8,
        NumIotlbEntries:16,NumDdtcEntries:8,NumPdtcEntries:8,NumMrifcEntries:8,
        InclPC:1,InclAxiBC:1,InclDbg:1,InclATS:0,
        MSITrans:rv_iommu_cfg::MSI_BT_MRIF,IGS:rv_iommu_cfg::BOTH,
        NumIntVec:16,NumHpmCounters:31,PAddrWidth:56,AxiAddrWidth:64,AxiDataWidth:64,
        AxiIdWidth:4,AxiProgIdWidth:6,AxiUserWidth:1,AxiProIdWidth:20,AxiDevIdWidth:24,
        AxisDataWidth:160,AxisUserWidth:1,AxisKeepWidth:20,AxisStrbWidth:20,AxisIdWidth:1,AxisDestWidth:1,Freq:100};
`AXI_TYPEDEF_AW_CHAN_EXT_T(t_aw_t,addr_t,logic[3:0],logic[0:0],device_id_t,logic,process_id_t)
`AXI_TYPEDEF_AR_CHAN_EXT_T(t_ar_t,addr_t,logic[3:0],logic[0:0],device_id_t,logic,process_id_t)
`AXI_TYPEDEF_REQ_T(t_req_t,t_aw_t,a_w_chan_t,t_ar_t)
t_req_t t_req;a_req_t c_req[3],d_req[3];a_resp_t t_rsp[3],c_rsp,d_rsp;p_req_t p_req;p_resp_t p_rsp[3];logic[15:0]wsi[3];logic[2:0] quiet,nfire;logic[23:0]ndid[3];
always_comb begin t_req='0;p_req='0;c_rsp='0;d_rsp='0;
t_req.aw_valid=t_aw_valid;
t_req.aw.addr=t_aw_addr;
t_req.aw.id=t_aw_id;
t_req.aw.len=t_aw_len;
t_req.aw.size=t_aw_size;
t_req.aw.atop=t_aw_atop;
t_req.w_valid=t_w_valid;
t_req.w.data=t_w_data;
t_req.w.strb=t_w_strb;
t_req.w.last=t_w_last;
t_req.b_ready=t_b_ready;
t_req.ar_valid=t_ar_valid;
t_req.ar.addr=t_ar_addr;
t_req.ar.id=t_ar_id;
t_req.ar.len=t_ar_len;
t_req.ar.size=t_ar_size;
t_req.r_ready=t_r_ready;
p_req.aw_valid=p_aw_valid;
p_req.aw.addr=p_aw_addr;
p_req.aw.id=p_aw_id;
p_req.aw.len=p_aw_len;
p_req.aw.size=p_aw_size;
p_req.aw.atop=p_aw_atop;
p_req.w_valid=p_w_valid;
p_req.w.data=p_w_data;
p_req.w.strb=p_w_strb;
p_req.w.last=p_w_last;
p_req.b_ready=p_b_ready;
p_req.ar_valid=p_ar_valid;
p_req.ar.addr=p_ar_addr;
p_req.ar.id=p_ar_id;
p_req.ar.len=p_ar_len;
p_req.ar.size=p_ar_size;
p_req.r_ready=p_r_ready;
c_rsp.aw_ready=c_aw_ready;
c_rsp.w_ready=c_w_ready;
c_rsp.b_valid=c_b_valid;
c_rsp.b.resp=c_b_resp;
c_rsp.b.id=c_b_id;
c_rsp.ar_ready=c_ar_ready;
c_rsp.r_valid=c_r_valid;
c_rsp.r.resp=c_r_resp;
c_rsp.r.id=c_r_id;
c_rsp.r.last=c_r_last;
c_rsp.r.data=c_r_data;
d_rsp.aw_ready=d_aw_ready;
d_rsp.w_ready=d_w_ready;
d_rsp.b_valid=d_b_valid;
d_rsp.b.resp=d_b_resp;
d_rsp.b.id=d_b_id;
d_rsp.ar_ready=d_ar_ready;
d_rsp.r_valid=d_r_valid;
d_rsp.r.resp=d_r_resp;
d_rsp.r.id=d_r_id;
d_rsp.r.last=d_r_last;
d_rsp.r.data=d_r_data;
t_req.aw.stream_id=t_did;t_req.ar.stream_id=t_did;t_req.aw.mmu_valid=1;t_req.ar.mmu_valid=1;t_req.aw.burst=1;t_req.ar.burst=1;p_req.aw.burst=1;p_req.ar.burst=1;end
assign t_aw_ready=t_rsp[0].aw_ready;
assign t_w_ready=t_rsp[0].w_ready;
assign t_b_valid=t_rsp[0].b_valid;
assign t_b_resp=t_rsp[0].b.resp;
assign t_b_id=t_rsp[0].b.id;
assign t_ar_ready=t_rsp[0].ar_ready;
assign t_r_valid=t_rsp[0].r_valid;
assign t_r_resp=t_rsp[0].r.resp;
assign t_r_id=t_rsp[0].r.id;
assign t_r_last=t_rsp[0].r.last;
assign t_r_data=t_rsp[0].r.data;
assign p_aw_ready=p_rsp[0].aw_ready;
assign p_w_ready=p_rsp[0].w_ready;
assign p_b_valid=p_rsp[0].b_valid;
assign p_b_resp=p_rsp[0].b.resp;
assign p_b_id=p_rsp[0].b.id;
assign p_ar_ready=p_rsp[0].ar_ready;
assign p_r_valid=p_rsp[0].r_valid;
assign p_r_resp=p_rsp[0].r.resp;
assign p_r_id=p_rsp[0].r.id;
assign p_r_last=p_rsp[0].r.last;
assign p_r_data=p_rsp[0].r.data;
assign c_aw_valid=c_req[0].aw_valid;
assign c_aw_addr=c_req[0].aw.addr;
assign c_aw_id=c_req[0].aw.id;
assign c_aw_len=c_req[0].aw.len;
assign c_aw_size=c_req[0].aw.size;
assign c_aw_atop=c_req[0].aw.atop;
assign c_w_valid=c_req[0].w_valid;
assign c_w_data=c_req[0].w.data;
assign c_w_strb=c_req[0].w.strb;
assign c_w_last=c_req[0].w.last;
assign c_b_ready=c_req[0].b_ready;
assign c_ar_valid=c_req[0].ar_valid;
assign c_ar_addr=c_req[0].ar.addr;
assign c_ar_id=c_req[0].ar.id;
assign c_ar_len=c_req[0].ar.len;
assign c_ar_size=c_req[0].ar.size;
assign c_r_ready=c_req[0].r_ready;
assign d_aw_valid=d_req[0].aw_valid;
assign d_aw_addr=d_req[0].aw.addr;
assign d_aw_id=d_req[0].aw.id;
assign d_aw_len=d_req[0].aw.len;
assign d_aw_size=d_req[0].aw.size;
assign d_aw_atop=d_req[0].aw.atop;
assign d_w_valid=d_req[0].w_valid;
assign d_w_data=d_req[0].w.data;
assign d_w_strb=d_req[0].w.strb;
assign d_w_last=d_req[0].w.last;
assign d_b_ready=d_req[0].b_ready;
assign d_ar_valid=d_req[0].ar_valid;
assign d_ar_addr=d_req[0].ar.addr;
assign d_ar_id=d_req[0].ar.id;
assign d_ar_len=d_req[0].ar.len;
assign d_ar_size=d_req[0].ar.size;
assign d_r_ready=d_req[0].r_ready;

for(genvar i=0;i<3;i++)begin:lanes
 if(i<2) begin:onoff
rv_iommu_top #(.RVIOMMUCfg(Cfg),.axi_tr_aw_chan_t(t_aw_t),.axi_tr_w_chan_t(a_w_chan_t),.axi_tr_b_chan_t(a_b_chan_t),.axi_tr_ar_chan_t(t_ar_t),.axi_tr_r_chan_t(a_r_chan_t),.axi_comp_aw_chan_t(a_aw_chan_t),.axi_comp_w_chan_t(a_w_chan_t),.axi_comp_b_chan_t(a_b_chan_t),.axi_comp_ar_chan_t(a_ar_chan_t),.axi_comp_r_chan_t(a_r_chan_t),.axi_ds_aw_chan_t(a_aw_chan_t),.axi_ds_w_chan_t(a_w_chan_t),.axi_ds_b_chan_t(a_b_chan_t),.axi_ds_ar_chan_t(a_ar_chan_t),.axi_ds_r_chan_t(a_r_chan_t),.axi_prog_aw_chan_t(p_aw_chan_t),.axi_prog_w_chan_t(p_w_chan_t),.axi_prog_b_chan_t(p_b_chan_t),.axi_prog_ar_chan_t(p_ar_chan_t),.axi_prog_r_chan_t(p_r_chan_t),.axi_tr_req_t(t_req_t),.axi_tr_resp_t(a_resp_t),.axi_comp_req_t(a_req_t),.axi_comp_resp_t(a_resp_t),.axi_ds_req_t(a_req_t),.axi_ds_resp_t(a_resp_t),.axi_prog_req_t(p_req_t),.axi_prog_resp_t(p_resp_t),.axis_req_t(logic),.axis_rsp_t(logic),.PnEnable(i==0),.HistoryControlEnable(i==0)) dut(.clk_i,.rst_ni,.tr_req_i(t_req),.tr_resp_o(t_rsp[i]),.comp_req_o(c_req[i]),.comp_resp_i(c_rsp),.ds_req_o(d_req[i]),.ds_resp_i(d_rsp),.prog_req_i(p_req),.prog_resp_o(p_rsp[i]),.ats_upstream_req_o(),.ats_upstream_resp_i(1'b0),.ats_downstream_req_i(1'b0),.ats_downstream_resp_o(),.wsi_wires_o(wsi[i]),.pn_mode_i(mode_i),.pn_history_isolate_i(history_i),.pn_policy_clear_i(clear_i),.pn_fullbuffer_jobs_i(permit_i),.pn_n_fire_o(nfire[i]),.pn_n_did_o(ndid[i]),.pn_quiet_o(quiet[i]));
end else if(Gold)begin:gold
rv_iommu_top_gold #(.RVIOMMUCfg(Cfg),.axi_tr_aw_chan_t(t_aw_t),.axi_tr_w_chan_t(a_w_chan_t),.axi_tr_b_chan_t(a_b_chan_t),.axi_tr_ar_chan_t(t_ar_t),.axi_tr_r_chan_t(a_r_chan_t),.axi_comp_aw_chan_t(a_aw_chan_t),.axi_comp_w_chan_t(a_w_chan_t),.axi_comp_b_chan_t(a_b_chan_t),.axi_comp_ar_chan_t(a_ar_chan_t),.axi_comp_r_chan_t(a_r_chan_t),.axi_ds_aw_chan_t(a_aw_chan_t),.axi_ds_w_chan_t(a_w_chan_t),.axi_ds_b_chan_t(a_b_chan_t),.axi_ds_ar_chan_t(a_ar_chan_t),.axi_ds_r_chan_t(a_r_chan_t),.axi_prog_aw_chan_t(p_aw_chan_t),.axi_prog_w_chan_t(p_w_chan_t),.axi_prog_b_chan_t(p_b_chan_t),.axi_prog_ar_chan_t(p_ar_chan_t),.axi_prog_r_chan_t(p_r_chan_t),.axi_tr_req_t(t_req_t),.axi_tr_resp_t(a_resp_t),.axi_comp_req_t(a_req_t),.axi_comp_resp_t(a_resp_t),.axi_ds_req_t(a_req_t),.axi_ds_resp_t(a_resp_t),.axi_prog_req_t(p_req_t),.axi_prog_resp_t(p_resp_t),.axis_req_t(logic),.axis_rsp_t(logic)) dut(.clk_i,.rst_ni,.tr_req_i(t_req),.tr_resp_o(t_rsp[i]),.comp_req_o(c_req[i]),.comp_resp_i(c_rsp),.ds_req_o(d_req[i]),.ds_resp_i(d_rsp),.prog_req_i(p_req),.prog_resp_o(p_rsp[i]),.ats_upstream_req_o(),.ats_upstream_resp_i(1'b0),.ats_downstream_req_i(1'b0),.ats_downstream_resp_o(),.wsi_wires_o(wsi[i]));
end else begin assign t_rsp[i]='0;assign c_req[i]='0;assign d_req[i]='0;assign p_rsp[i]='0;assign wsi[i]='0;end
end
assign same_off_o=t_rsp[0]===t_rsp[1]&&c_req[0]===c_req[1]&&d_req[0]===d_req[1]&&p_rsp[0]===p_rsp[1]&&wsi[0]===wsi[1];
assign same_gold_o=t_rsp[0]===t_rsp[2]&&c_req[0]===c_req[2]&&d_req[0]===d_req[2]&&p_rsp[0]===p_rsp[2]&&wsi[0]===wsi[2];
assign quiet_o=quiet[0],nfire_o=nfire[0],ndid_o=ndid[0],wsi_o=wsi[0];
assign test_config_o={16'b0,lanes[0].onoff.dut.ddtp.ppn.q,lanes[0].onoff.dut.ddtp.iommu_mode.q};
assign test_iova_o=lanes[0].onoff.dut.trans_req_data.iova;
assign test_state_o={4'b0,lanes[0].onoff.dut.gen_pn_tl.gen_dc_ext.i_rv_iommu_tl_wrap.state_q,lanes[0].onoff.dut.i_rv_iommu_trans_ctl_wrap.state_q,2'b0,lanes[0].onoff.dut.trans_req_valid,lanes[0].onoff.dut.trans_req_ready,lanes[0].onoff.dut.trans_resp_valid,lanes[0].onoff.dut.trans_resp_data.ignore};
endmodule
