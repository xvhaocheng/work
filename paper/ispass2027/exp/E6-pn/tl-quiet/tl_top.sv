// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Actual TL/walkers + DS + debug-GO source. Hierarchical signals are test only.
`include "axi/typedef.svh"
module tl_top import rv_iommu::*; import rv_iommu_reg_pkg::*; (
 input logic clk_i,rst_ni,n_enable_i,restricted_i,gold_shared_debug_i,compare85_i,
 input logic req_valid_i,req_bank_i,req_pid_i,req_priv_i,req_debug_i,resp_ready_i,fault_ready_i,
 input logic [19:0] pid_i,
 input logic [23:0] did_i,
 input logic [63:0] iova_i,
 input logic [5:0] type_i,
 input logic dbg_go_i,dbg_read_i,
 input logic [63:0] dbg_iova_i,
 input logic [23:0] dbg_did_i,
 input logic inval_i,dc_inval_i,clear_i,
 input logic ds_ar_ready_i,ds_r_valid_i,ds_r_last_i,
 input logic [3:0] ds_r_id_i,
 input logic [63:0] ds_r_data_i,
 input logic [1:0] ds_r_resp_i,
 output logic ds_ar_valid_o,ds_r_ready_o,
 output logic [3:0] ds_ar_id_o,
 output logic [7:0] ds_ar_len_o,
 output logic [63:0] ds_ar_addr_o,
 output logic equal_old_o,equal_off_o,equal85_o,pn_idle_o,off_idle_o,policy_quiet_o,ds_quiet_o,ds_buffers_o,msi_idle_o,
 output logic [7:0] ds_debt_o,
 output logic ds_bad_o,req_ready_o,resp_valid_o,resp_error_o,resp_mrif_o,resp_ignore_o,msi_iotlb_fill_o,
 output logic [55:0] resp_addr_o,
 output logic [11:0] resp_code_o,
 output logic debug_req_valid_o,debug_req_ready_o,debug_valid_o,debug_error_o,
 output logic [43:0] debug_ppn_o,
 output logic fault_valid_o,
 output logic [11:0] fault_code_o,
 output logic [23:0] fault_did_o,
 output logic [2:0] state_o,
 output logic stable_debug_o,restricted_msi_o,stable_ats_o,stable_msg_o,
 output logic [5:0] stable_type_o,
 output logic dd_active_o,pd_active_o,pt_active_o,n_busy_o,n_accept_o,
 output logic ddt_update_o,pdt_update_o,iotlb_update_o,mrif_update_o,msi_init_o,
 output logic [1:0] bt_state_o,mrif_state_o,
 output logic branches_idle_o,branches_off_o,bt_only_idle_o
);
 typedef logic[63:0] addr_t;typedef logic[55:0] paddr_t;
 typedef struct packed {addr_t iova;device_id_t did;logic pid_valid;process_id_t pid;ttype_t ttype;logic priv,is_debug;} base_req_t;
 typedef struct packed {logic bank;addr_t iova;device_id_t did;logic pid_valid;process_id_t pid;ttype_t ttype;logic priv,is_debug;} req_t;
 typedef struct packed {logic bank,error,ignore;paddr_t spaddr;logic is_mrif;mrifc_content_t mrif_data;logic[2:0]range;logic bypass;cause_t fault_code;logic x,w,r,g,u,sum,t2gpa;} rsp_t;
 typedef struct packed {logic error,is_superpage;logic[1:0]pbmt;ppn_t ppn;} dbg_t;
 typedef struct packed {ttype_t trans_type;cause_t cause_code;addr_t iova;logic[40:0]gpaddr;device_id_t did;logic pv;process_id_t pid;logic is_supervisor,is_guest_pf,is_implicit;} fault_t;
 `AXI_TYPEDEF_ALL(axi,addr_t,logic[3:0],logic[63:0],logic[7:0],logic[0:0])
 localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg='{default:'0,NumOutstandingTrans:8,WFifoDepth:32,MrifFifoDepth:8,FaultFifoDepth:8,NumIotlbEntries:16,NumDdtcEntries:8,NumPdtcEntries:8,NumMrifcEntries:8,InclPC:1,InclAxiBC:1,InclDbg:1,InclATS:0,MSITrans:rv_iommu_cfg::MSI_BT_MRIF,IGS:rv_iommu_cfg::BOTH,NumIntVec:16,NumHpmCounters:31,PAddrWidth:56,AxiAddrWidth:64,AxiDataWidth:64,AxiIdWidth:4,AxiProgIdWidth:4,AxiUserWidth:1};
 req_t req,debug_req;
 base_req_t debug_base;
 logic dbg_valid,dbg_ready;
 rsp_t rsp[4],atsrsp[4];dbg_t dbg_rsp[4];fault_t fault[4];hpm_event_t hpm[4];
 logic[3:0]ready,valid,dbgvalid,fv,dbgreqready,atsready,atsvalid;logic[1:0]pn;
 axi_req_t dd[4],pd[4],pt[4],ms[4];axi_resp_t ddr,pdr,ptr,msr;
 iommu_reg2hw_tr_req_iova_reg_t dbg_iova;
 iommu_reg2hw_tr_req_ctl_reg_t dbg_ctl;
 always_comb begin
  req='0;req.bank=req_bank_i;req.iova=iova_i;req.did=did_i;req.pid_valid=req_pid_i;req.pid=pid_i;req.ttype=ttype_t'(type_i);req.priv=req_priv_i;req.is_debug=req_debug_i;
  dbg_iova='0;dbg_iova.vpn.q=dbg_iova_i[63:12];dbg_ctl='0;dbg_ctl.go.q=dbg_go_i;dbg_ctl.did.q=dbg_did_i;dbg_ctl.nw.q=dbg_read_i;
 end
 assign debug_req={1'b0,debug_base};
 rv_iommu_dbg_if_ctl #(.trans_req_data_t(base_req_t),.dbg_resp_t(dbg_t)) debug_source (
  .clk_i,.rst_ni,.dbg_iova_i(dbg_iova),.dbg_ctl_i(dbg_ctl),.dbg_resp_o(),.dbg_ctl_o(),.req_data_o(debug_base),.req_valid_o(dbg_valid),.req_ready_i(dbgreqready[0]),.resp_data_i(dbg_rsp[0]),.resp_valid_i(dbgvalid[0]),.resp_ready_o(dbg_ready));
 iommu_reg2hw_capabilities_reg_t caps;iommu_reg2hw_fctl_reg_t fctl;iommu_reg2hw_ddtp_reg_t ddtp;iotlb_inval_t inv;xdtc_inval_t dinv;
 always_comb begin
  caps='0;caps.pd8.q=1;caps.sv39.q=1;caps.sv39x4.q=1;caps.pas.q=56;caps.msi_flat.q=1;caps.msi_mrif.q=1;
  fctl='0;ddtp='0;ddtp.iommu_mode.q=2;ddtp.ppn.q=44'h10;inv='0;inv.inval_vma=inval_i;dinv='0;dinv.inval_ddtc=dc_inval_i;
 end
 `define PARAMS(C) .RVIOMMUCfg(C),.addr_t(addr_t),.paddr_t(paddr_t),.dc_t(dc_ext_t),.ddtc_up_t(ddtc_up_ext_t),.trans_req_data_t(req_t),.trans_resp_data_t(rsp_t),.dbg_resp_t(dbg_t),.fault_data_t(fault_t),.axi_req_t(axi_req_t),.axi_resp_t(axi_resp_t)
 `define PORTS .clk_i,.rst_ni,.trans_req_data_i(req),.trans_req_valid_i(req_valid_i&&(i!=3||compare85_i)),.trans_req_ready_o(ready[i]),.ats_trans_req_data_i('0),.ats_trans_req_valid_i(1'b0),.ats_trans_req_ready_o(atsready[i]),.debug_req_data_i(debug_req),.debug_req_valid_i(dbg_valid&&(i!=3||compare85_i)),.debug_req_ready_o(dbgreqready[i]),.trans_resp_data_o(rsp[i]),.trans_resp_valid_o(valid[i]),.trans_resp_ready_i(resp_ready_i),.ats_trans_resp_data_o(atsrsp[i]),.ats_trans_resp_valid_o(atsvalid[i]),.ats_trans_resp_ready_i(1'b1),.ats_ddtc_valid_i(1'b0),.ats_ddtc_did_i('0),.ats_ddtc_ready_o(),.ats_ddtc_tc_o(),.debug_resp_data_o(dbg_rsp[i]),.debug_resp_valid_o(dbgvalid[i]),.debug_resp_ready_i(dbg_ready),.capabilities_i(caps),.fctl_i(fctl),.ddtp_i(ddtp),.iotinval_i(inv),.iodirinval_i(dinv),.hpm_events_o(hpm[i]),.ddtw_axi_req_o(dd[i]),.ddtw_axi_resp_i(ddr),.pdtw_axi_req_o(pd[i]),.pdtw_axi_resp_i(pdr),.ptw_axi_req_o(pt[i]),.ptw_axi_resp_i(ptr),.msiptw_axi_req_o(ms[i]),.msiptw_axi_resp_i(msr),.fault_data_o(fault[i]),.fault_valid_o(fv[i]),.fault_ready_i(fault_ready_i)
 for(genvar i=0;i<4;i++)begin:lanes
  if(i<2)begin:candidate
   rv_iommu_tl_wrap #(`PARAMS(Cfg),.PolicyEnable(1),.NonblockEnable(1),.HistoryControlEnable(1),.PnEnable(i==0)) dut (
    `PORTS,.policy_partition_i(1'b0),.policy_clear_i(clear_i),.history_isolate_i(1'b1),.nonblock_enable_i(n_enable_i),.restricted_bank_i(restricted_i),.nonblock_fire_o(),.nonblock_busy_o(),.nonblock_did_o(),.policy_quiet_o(),.pn_idle_o(pn[i]));
  end else if(i==2)begin:old
   rv_iommu_tl_wrap_old #(`PARAMS(Cfg),.PolicyEnable(1),.NonblockEnable(1),.HistoryControlEnable(1)) dut (
    `PORTS,.policy_partition_i(1'b0),.policy_clear_i(clear_i),.history_isolate_i(1'b1),.nonblock_enable_i(n_enable_i),.restricted_bank_i(restricted_i&&!gold_shared_debug_i),.nonblock_fire_o(),.nonblock_busy_o(),.nonblock_did_o(),.policy_quiet_o());
  end else begin:original
   rv_iommu_tl_wrap_gold #(`PARAMS(Cfg)) dut (`PORTS);
  end
 end
 logic[3:0]same;
 always_comb begin
  for(int i=0;i<4;i++)same[i]=(ready[0]===ready[i])&&(valid[0]===valid[i])&&(rsp[0]===rsp[i])&&(atsrsp[0]===atsrsp[i])&&(atsvalid[0]===atsvalid[i])&&(atsready[0]===atsready[i])&&(dbgreqready[0]===dbgreqready[i])&&(dbgvalid[0]===dbgvalid[i])&&(dbg_rsp[0]===dbg_rsp[i])&&(fv[0]===fv[i])&&(fault[0]===fault[i])&&(hpm[0]===hpm[i])&&(dd[0]===dd[i])&&(pd[0]===pd[i])&&(pt[0]===pt[i])&&(ms[0]===ms[i]);
 end
 assign pn_idle_o=pn[0],off_idle_o=pn[1];
 assign equal_off_o=same[1],equal_old_o=same[2],equal85_o=same[3];
 axi_req_t ds;axi_resp_t ds_rsp;
 always_comb begin ds_rsp='0;ds_rsp.aw_ready=1;ds_rsp.w_ready=1;ds_rsp.ar_ready=ds_ar_ready_i;ds_rsp.r_valid=ds_r_valid_i;ds_rsp.r.id=ds_r_id_i;ds_rsp.r.data=ds_r_data_i;ds_rsp.r.last=ds_r_last_i;ds_rsp.r.resp=ds_r_resp_i;end
 rv_iommu_ds_if #(.aw_chan_t(axi_aw_chan_t),.w_chan_t(axi_w_chan_t),.b_chan_t(axi_b_chan_t),.ar_chan_t(axi_ar_chan_t),.r_chan_t(axi_r_chan_t),.axi_req_t(axi_req_t),.axi_resp_t(axi_resp_t),.PnEnable(1)) ds_dut (
 .clk_i,.rst_ni,.ds_resp_i(ds_rsp),.ds_req_o(ds),.ddtw_req_i(dd[0]),.ddtw_resp_o(ddr),.pdtw_req_i(pd[0]),.pdtw_resp_o(pdr),.ptw_req_i(pt[0]),.ptw_resp_o(ptr),.msiptw_req_i(ms[0]),.msiptw_resp_o(msr),.mrif_handler_req_i('0),.mrif_handler_resp_o(),.cq_req_i('0),.cq_resp_o(),.fq_req_i('0),.fq_resp_o(),.pq_req_i('0),.pq_resp_o(),.msi_ig_req_i('0),.msi_ig_resp_o(),.pn_buffers_quiet_o(ds_buffers_o),.pn_transport_quiet_o(ds_quiet_o));
 assign ds_ar_valid_o=ds.ar_valid,ds_ar_id_o=ds.ar.id,ds_ar_addr_o=ds.ar.addr,ds_ar_len_o=ds.ar.len,ds_r_ready_o=ds.r_ready;
 assign ds_debt_o=ds_dut.gen_pn_quiet.i_debt.read_q,ds_bad_o=ds_dut.gen_pn_quiet.i_debt.bad_q;
 assign req_ready_o=ready[0],resp_valid_o=valid[0],resp_error_o=rsp[0].error,resp_mrif_o=rsp[0].is_mrif,resp_ignore_o=rsp[0].ignore,resp_addr_o=rsp[0].spaddr,resp_code_o=rsp[0].fault_code;
 assign debug_req_valid_o=dbg_valid,debug_req_ready_o=dbgreqready[0],debug_valid_o=dbgvalid[0],debug_error_o=dbg_rsp[0].error,debug_ppn_o=dbg_rsp[0].ppn;
 assign fault_valid_o=fv[0],fault_code_o=fault[0].cause_code,fault_did_o=fault[0].did;
 assign policy_quiet_o=lanes[0].candidate.dut.policy_quiet_o;
 assign state_o=lanes[0].candidate.dut.state_q,stable_debug_o=lanes[0].candidate.dut.req_data.is_debug,stable_type_o=lanes[0].candidate.dut.req_data.ttype,restricted_msi_o=lanes[0].candidate.dut.restricted_msi,stable_ats_o=lanes[0].candidate.dut.is_pcie_ats_req,stable_msg_o=lanes[0].candidate.dut.is_pcie_page_req;
 assign dd_active_o=lanes[0].candidate.dut.pn_ddtw_active,pd_active_o=lanes[0].candidate.dut.gen_pdt_modules.pn_pdtw_active,pt_active_o=lanes[0].candidate.dut.ptw_active,n_busy_o=lanes[0].candidate.dut.n_busy,n_accept_o=lanes[0].candidate.dut.n_accept;
 assign ddt_update_o=lanes[0].candidate.dut.ddtc_update.update,pdt_update_o=lanes[0].candidate.dut.gen_pdt_modules.pdtc_update.update,iotlb_update_o=lanes[0].candidate.dut.iotlb_update.update,mrif_update_o=lanes[0].candidate.dut.gen_msi_support_tl.mrifc_update.update,msi_init_o=lanes[0].candidate.dut.gen_msi_support_tl.msi_ptw_init;
 assign msi_iotlb_fill_o=lanes[0].candidate.dut.msi_iotlb_update.update;
 assign msi_idle_o=lanes[0].candidate.dut.gen_msi_support_tl.pn_msiptw_idle,bt_state_o=lanes[0].candidate.dut.gen_msi_support_tl.i_rv_iommu_msiptw.bt_state_q,mrif_state_o=lanes[0].candidate.dut.gen_msi_support_tl.i_rv_iommu_msiptw.gen_mrif_support.mrif_state_q;
 // Constant no-PC/MSI-disabled/BT-only generation checks; no producer stimulus.
 logic [1:0] branch_idle,branch_off;
 for(genvar b=0;b<2;b++)begin:branches
  localparam rv_iommu_cfg::rv_iommu_cfg_t BCfg='{default:'0,NumOutstandingTrans:8,WFifoDepth:32,MrifFifoDepth:8,FaultFifoDepth:8,NumIotlbEntries:16,NumDdtcEntries:8,NumPdtcEntries:8,NumMrifcEntries:8,InclPC:0,InclDbg:1,InclATS:0,MSITrans:rv_iommu_cfg::msi_trans_t'(b),IGS:rv_iommu_cfg::BOTH,PAddrWidth:56,AxiAddrWidth:64,AxiDataWidth:64,AxiIdWidth:4,AxiProgIdWidth:4,AxiUserWidth:1};
  `define ZERO .clk_i,.rst_ni,.policy_partition_i(1'b0),.policy_clear_i(1'b0),.history_isolate_i(1'b1),.nonblock_enable_i(1'b0),.restricted_bank_i(1'b0),.nonblock_fire_o(),.nonblock_busy_o(),.nonblock_did_o(),.policy_quiet_o(),.trans_req_data_i('0),.trans_req_valid_i(1'b0),.trans_req_ready_o(),.ats_trans_req_data_i('0),.ats_trans_req_valid_i(1'b0),.ats_trans_req_ready_o(),.debug_req_data_i('0),.debug_req_valid_i(1'b0),.debug_req_ready_o(),.trans_resp_data_o(),.trans_resp_valid_o(),.trans_resp_ready_i(1'b1),.ats_trans_resp_data_o(),.ats_trans_resp_valid_o(),.ats_trans_resp_ready_i(1'b1),.ats_ddtc_valid_i(1'b0),.ats_ddtc_did_i('0),.ats_ddtc_ready_o(),.ats_ddtc_tc_o(),.debug_resp_data_o(),.debug_resp_valid_o(),.debug_resp_ready_i(1'b1),.capabilities_i(caps),.fctl_i(fctl),.ddtp_i(ddtp),.iotinval_i('0),.iodirinval_i('0),.hpm_events_o(),.ddtw_axi_req_o(),.ddtw_axi_resp_i('0),.pdtw_axi_req_o(),.pdtw_axi_resp_i('0),.ptw_axi_req_o(),.ptw_axi_resp_i('0),.msiptw_axi_req_o(),.msiptw_axi_resp_i('0),.fault_data_o(),.fault_valid_o(),.fault_ready_i(1'b1)
  rv_iommu_tl_wrap #(`PARAMS(BCfg),.PnEnable(1)) dut_on (`ZERO,.pn_idle_o(branch_idle[b]));
  rv_iommu_tl_wrap #(`PARAMS(BCfg)) dut_off (`ZERO,.pn_idle_o(branch_off[b]));
  `undef ZERO
 end
 assign branches_idle_o=&branch_idle,branches_off_o=|branch_off;
 assign bt_only_idle_o=branches[1].dut_on.gen_msi_support_tl.pn_msiptw_idle;
 `undef PARAMS
 `undef PORTS
endmodule
