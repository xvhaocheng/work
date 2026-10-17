// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Test-only complete original SW/program hierarchy, independent candidate/gold state.
`include "axi/typedef.svh"
`include "register_interface/typedef.svh"
module sw_quiet_top import rv_iommu::*; import rv_iommu_reg_pkg::*; #(
    parameter bit OmitFeatures = 0
)(
    input logic clk_i, rst_ni,
    input logic p_aw_valid_i,p_w_valid_i,p_ar_valid_i,p_b_ready_i,p_r_ready_i,
    input logic [63:0] p_aw_addr_i,p_ar_addr_i,p_w_data_i,
    input logic [5:0] p_aw_id_i,p_ar_id_i,
    input logic [2:0] p_aw_size_i,p_ar_size_i,
    input logic [7:0] p_aw_len_i,p_ar_len_i,p_w_strb_i,
    input logic p_w_last_i,
    output logic p_aw_ready_o,p_w_ready_o,p_ar_ready_o,p_b_valid_o,p_r_valid_o,p_r_last_o,
    output logic [1:0] p_b_resp_o,p_r_resp_o,
    output logic [5:0] p_b_id_o,p_r_id_o,
    output logic [63:0] p_r_data_o,
    input logic [3:0] m_ar_ready_i,m_aw_ready_i,m_w_ready_i,m_r_valid_i,m_b_valid_i,m_r_last_i,
    input logic [3:0][63:0] m_r_data_i,
    input logic [3:0][3:0] m_r_id_i,m_b_id_i,
    input logic [3:0][1:0] m_r_resp_i,m_b_resp_i,
    output logic [3:0] m_ar_valid_o,m_aw_valid_o,m_w_valid_o,m_r_ready_o,m_b_ready_o,m_w_last_o,
    output logic [3:0][63:0] m_ar_addr_o,m_aw_addr_o,m_w_data_o,
    output logic [3:0][3:0] m_ar_id_o,m_aw_id_o,
    output logic [3:0][7:0] m_ar_len_o,m_aw_len_o,m_w_strb_o,
    output logic [3:0][2:0] m_ar_size_o,m_aw_size_o,
    input logic [1:0] fault_valid_i,
    input logic [31:0] fault_serial_i,
    input logic [11:0] fault_cause_i,
    output logic [1:0] fault_ready_o,
    input logic dbg_req_ready_i,dbg_resp_valid_i,dbg_error_i,in_flight_i,hpm_valid_i,
    input logic [43:0] dbg_ppn_i,
    output logic dbg_req_valid_o,dbg_resp_ready_o,dbg_is_debug_o,
    output logic [63:0] dbg_iova_o,
    output logic [23:0] dbg_did_o,
    output logic [15:0] wsi_o,
    output logic [1:0] equal_o,
    output logic [1:0] sw_idle_o,prog_idle_o,
    output logic [5:0] child_idle_o,
    output logic [2:0] cq_state_o,
    output logic [1:0] cq_write_state_o,fq_state_o,pq_state_o,msi_state_o,
    output logic dbg_state_o,dbg_go_o,reg_valid_o,reg_write_o,
    output logic [63:0] reg_addr_o,
    output logic [31:0] reg_wdata_o,cq_head_o,cq_tail_o,fq_head_o,fq_tail_o,
    output logic [31:0] cqcsr_o,fqcsr_o,
    output logic [3:0] bridge_state_o,
    output logic [9:0] bridge_valids_o,
    output logic bridge_psel_o,bridge_penable_o,
    output logic [2:0] fault_spill_o,
    output logic fault_fifo_o,fault_arb_o,ddtp_pending_o,regmap_idle_o,
    output logic [15:0] msi_pending_o,
    output logic [2:0] msi_edged_o,irq_pending_o,
    output logic [8191:0] old_on_o,old_off_o,old_gold_o
);
    typedef logic [63:0] addr_t;
    typedef logic [55:0] paddr_t;
    typedef struct packed {addr_t iova;device_id_t did;logic pid_valid;process_id_t pid;ttype_t ttype;logic priv,is_debug;} tr_t;
    typedef struct packed {logic error,is_superpage;logic[1:0] pbmt;ppn_t ppn;} dbg_t;
    typedef struct packed {ttype_t trans_type;cause_t cause_code;addr_t iova;logic[40:0] gpaddr;device_id_t did;logic pv;process_id_t pid;logic is_supervisor,is_guest_pf,is_implicit;} fault_t;
    `AXI_TYPEDEF_ALL(ds,addr_t,logic[3:0],logic[63:0],logic[7:0],logic[0:0])
    `AXI_TYPEDEF_ALL(prog,addr_t,logic[5:0],logic[63:0],logic[7:0],logic[0:0])
    `REG_BUS_TYPEDEF_ALL(regbus,addr_t,logic[31:0],logic[3:0])
    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg = '{
        NumOutstandingTrans:8,WFifoDepth:32,MrifFifoDepth:8,FaultFifoDepth:8,
        NumIotlbEntries:16,NumDdtcEntries:8,NumPdtcEntries:8,NumMrifcEntries:8,
        InclPC:1,InclAxiBC:1,InclDbg:!OmitFeatures,InclATS:0,
        MSITrans:rv_iommu_cfg::MSI_BT_MRIF,IGS:OmitFeatures?rv_iommu_cfg::WSI_ONLY:rv_iommu_cfg::BOTH,
        NumIntVec:16,NumHpmCounters:31,PAddrWidth:56,AxiAddrWidth:64,AxiDataWidth:64,
        AxiIdWidth:4,AxiProgIdWidth:6,AxiUserWidth:1,AxiProIdWidth:20,AxiDevIdWidth:24,
        AxisDataWidth:160,AxisUserWidth:1,AxisKeepWidth:20,AxisStrbWidth:20,AxisIdWidth:1,AxisDestWidth:1,Freq:100};
    prog_req_t program_req;prog_resp_t program_rsp[3];
    regbus_req_t regreq[3];regbus_rsp_t regrsp[3];
    ds_req_t memreq[3][4];ds_resp_t memrsp[4];
    fault_t fault[2];dbg_t dbg_rsp;hpm_event_t hpmevent;
    tr_t dbgreq[3];logic[2:0] dbgvalid,dbgready;
    logic[1:0] fready[3];
    iommu_reg2hw_capabilities_reg_t caps[3];iommu_reg2hw_fctl_reg_t fctl[3];iommu_reg2hw_ddtp_reg_t ddtp[3];
    iotlb_inval_t inv[3];xdtc_inval_t dinv[3];
    cq_atsinval_t ainv[3];cq_atsprgr_t aprg[3];
    logic[2:0] avalid,prvalid,fencevalid,pqready;
    logic[31:0] pqhead[3],pqtail[3];pri_fault pqfault[3];logic[15:0] wsi[3];
    logic[2:0] swidle,progidle;
    always_comb begin
        program_req='0;program_req.aw_valid=p_aw_valid_i;program_req.aw.addr=p_aw_addr_i;
        program_req.aw.id=p_aw_id_i;program_req.aw.len=p_aw_len_i;program_req.aw.size=p_aw_size_i;program_req.aw.burst=axi_pkg::BURST_INCR;
        program_req.w_valid=p_w_valid_i;program_req.w.data=p_w_data_i;program_req.w.strb=p_w_strb_i;program_req.w.last=p_w_last_i;
        program_req.ar_valid=p_ar_valid_i;program_req.ar.addr=p_ar_addr_i;program_req.ar.id=p_ar_id_i;
        program_req.ar.len=p_ar_len_i;program_req.ar.size=p_ar_size_i;program_req.ar.burst=axi_pkg::BURST_INCR;
        program_req.b_ready=p_b_ready_i;program_req.r_ready=p_r_ready_i;
        for(int m=0;m<4;m++) begin
            memrsp[m]='0;memrsp[m].ar_ready=m_ar_ready_i[m];memrsp[m].aw_ready=m_aw_ready_i[m];memrsp[m].w_ready=m_w_ready_i[m];
            memrsp[m].r_valid=m_r_valid_i[m];memrsp[m].r.id=m_r_id_i[m];memrsp[m].r.data=m_r_data_i[m];memrsp[m].r.last=m_r_last_i[m];memrsp[m].r.resp=m_r_resp_i[m];
            memrsp[m].b_valid=m_b_valid_i[m];memrsp[m].b.id=m_b_id_i[m];memrsp[m].b.resp=m_b_resp_i[m];
        end
        for(int f=0;f<2;f++) begin
            fault[f]='0;fault[f].cause_code=cause_t'(fault_cause_i);fault[f].did=24'(f+1);
            fault[f].iova={32'h12340000,fault_serial_i};fault[f].trans_type=UNTRANSLATED_R;
        end
        dbg_rsp='0;dbg_rsp.error=dbg_error_i;dbg_rsp.ppn=dbg_ppn_i;
        hpmevent='0;hpmevent.valid=hpm_valid_i;hpmevent.etype_msk=9'b10;
    end
    `define PROGPARAMS .RVIOMMUCfg(Cfg),.axi_req_t(prog_req_t),.axi_resp_t(prog_resp_t),.iommu_reg_req_t(regbus_req_t),.iommu_reg_rsp_t(regbus_rsp_t)
    `define PROGPORTS .clk_i,.rst_ni,.prog_req_i(program_req),.prog_resp_o(program_rsp[i]),.regmap_req_o(regreq[i]),.regmap_resp_i(regrsp[i])
    `define SWPARAMS .RVIOMMUCfg(Cfg),.addr_t(addr_t),.paddr_t(paddr_t),.trans_req_data_t(tr_t),.dbg_resp_t(dbg_t),.fault_data_t(fault_t),.axi_req_t(ds_req_t),.axi_resp_t(ds_resp_t),.iommu_reg_req_t(regbus_req_t),.iommu_reg_rsp_t(regbus_rsp_t)
    `define SWPORTS .clk_i,.rst_ni,.regmap_req_i(regreq[i]),.regmap_resp_o(regrsp[i]),.cq_axi_req_o(memreq[i][0]),.cq_axi_resp_i(memrsp[0]),.fq_axi_req_o(memreq[i][1]),.fq_axi_resp_i(memrsp[1]),.pq_axi_req_o(memreq[i][2]),.pq_axi_resp_i(memrsp[2]),.msi_ig_axi_req_o(memreq[i][3]),.msi_ig_axi_resp_i(memrsp[3]),.capabilities_o(caps[i]),.fctl_o(fctl[i]),.ddtp_o(ddtp[i]),.dbg_req_data_o(dbgreq[i]),.dbg_req_valid_o(dbgvalid[i]),.dbg_req_ready_i(dbg_req_ready_i),.dbg_resp_data_i(dbg_rsp),.dbg_resp_valid_i(dbg_resp_valid_i),.dbg_resp_ready_o(dbgready[i]),.iotinval_o(inv[i]),.iodirinval_o(dinv[i]),.hpm_events_i(hpmevent),.trans_fault_data_i(fault[0]),.trans_fault_valid_i(fault_valid_i[0]),.trans_fault_ready_o(fready[i][0]),.mrif_fault_data_i(fault[1]),.mrif_fault_valid_i(fault_valid_i[1]),.mrif_fault_ready_o(fready[i][1]),.in_flight_i,.atsinval_o(ainv[i]),.atsinval_valid_o(avalid[i]),.atsinval_ready_i(1'b1),.atsinval_to_i(1'b0),.atsinval_inflight_i(1'b0),.ats_fence_valid_o(fencevalid[i]),.ats_fence_ready_i(1'b1),.pq_head_o(pqhead[i]),.pq_tail_o(pqtail[i]),.atsprgr_o(aprg[i]),.atsprgr_valid_o(prvalid[i]),.atsprgr_ready_i(1'b1),.pq_data_i('0),.pq_valid_i(1'b0),.pq_ready_o(pqready[i]),.pq_data_o(pqfault[i]),.wsi_wires_o(wsi[i])
    for(genvar i=0;i<3;i++) begin : lane
        if(i<2) begin : candidate
            rv_iommu_prog_if #(`PROGPARAMS,.PnEnable(i==0)) prog (`PROGPORTS,.pn_idle_o(progidle[i]));
            rv_iommu_sw_if_wrap #(`SWPARAMS,.PnEnable(i==0)) sw (`SWPORTS,.pn_idle_o(swidle[i]));
        end else begin : original
            rv_iommu_prog_if_gold #(`PROGPARAMS) prog (`PROGPORTS);
            rv_iommu_sw_if_wrap_gold #(`SWPARAMS) sw (`SWPORTS);
            assign swidle[i]=0;assign progidle[i]=0;
        end
    end
    `undef PROGPARAMS
    `undef PROGPORTS
    `undef SWPARAMS
    `undef SWPORTS
    logic[8191:0] old[3];
    for(genvar i=0;i<3;i++) begin
        assign old[i]={program_rsp[i],regreq[i],regrsp[i],memreq[i][0],memreq[i][1],memreq[i][2],memreq[i][3],caps[i],fctl[i],ddtp[i],dbgreq[i],dbgvalid[i],dbgready[i],inv[i],dinv[i],fready[i],ainv[i],avalid[i],fencevalid[i],pqhead[i],pqtail[i],aprg[i],prvalid[i],pqready[i],pqfault[i],wsi[i]};
        initial assert($bits({program_rsp[i],regreq[i],regrsp[i],memreq[i][0],memreq[i][1],memreq[i][2],memreq[i][3],caps[i],fctl[i],ddtp[i],dbgreq[i],dbgvalid[i],dbgready[i],inv[i],dinv[i],fready[i],ainv[i],avalid[i],fencevalid[i],pqhead[i],pqtail[i],aprg[i],prvalid[i],pqready[i],pqfault[i],wsi[i]})<=8192);
    end
    assign old_on_o=old[0],old_off_o=old[1],old_gold_o=old[2];
    assign equal_o={old[1]===old[2],old[0]===old[2]};
    assign sw_idle_o=swidle[1:0],prog_idle_o=progidle[1:0];
    assign p_aw_ready_o=program_rsp[0].aw_ready,p_w_ready_o=program_rsp[0].w_ready,p_ar_ready_o=program_rsp[0].ar_ready;
    assign p_b_valid_o=program_rsp[0].b_valid,p_r_valid_o=program_rsp[0].r_valid,p_r_last_o=program_rsp[0].r.last;
    assign p_b_resp_o=program_rsp[0].b.resp,p_r_resp_o=program_rsp[0].r.resp,p_b_id_o=program_rsp[0].b.id,p_r_id_o=program_rsp[0].r.id,p_r_data_o=program_rsp[0].r.data;
    for(genvar m=0;m<4;m++) begin
        assign m_ar_valid_o[m]=memreq[0][m].ar_valid,m_aw_valid_o[m]=memreq[0][m].aw_valid,m_w_valid_o[m]=memreq[0][m].w_valid,m_r_ready_o[m]=memreq[0][m].r_ready,m_b_ready_o[m]=memreq[0][m].b_ready,m_w_last_o[m]=memreq[0][m].w.last;
        assign m_ar_addr_o[m]=memreq[0][m].ar.addr,m_aw_addr_o[m]=memreq[0][m].aw.addr,m_w_data_o[m]=memreq[0][m].w.data;
        assign m_ar_id_o[m]=memreq[0][m].ar.id,m_aw_id_o[m]=memreq[0][m].aw.id,m_ar_len_o[m]=memreq[0][m].ar.len,m_aw_len_o[m]=memreq[0][m].aw.len,m_w_strb_o[m]=memreq[0][m].w.strb,m_ar_size_o[m]=memreq[0][m].ar.size,m_aw_size_o[m]=memreq[0][m].aw.size;
    end
    assign fault_ready_o=fready[0],dbg_req_valid_o=dbgvalid[0],dbg_resp_ready_o=dbgready[0],dbg_is_debug_o=dbgreq[0].is_debug,dbg_iova_o=dbgreq[0].iova,dbg_did_o=dbgreq[0].did,wsi_o=wsi[0];
    `define SW lane[0].candidate.sw
    `define BR lane[0].candidate.prog.i_axi2apb_64_32_iommu
    assign child_idle_o={`SW.pn_dbg_idle,`SW.pn_msi_idle,`SW.pn_pq_idle,`SW.pn_fq_idle,`SW.pn_fault_idle,`SW.pn_cq_idle};
    assign cq_state_o=`SW.i_rv_iommu_cq_handler.state_q,cq_write_state_o=`SW.i_rv_iommu_cq_handler.wr_state_q;
    assign fq_state_o=`SW.i_rv_iommu_fq_handler.state_q,pq_state_o=`SW.i_rv_iommu_pq_handler.state_q;
    assign cq_head_o=`SW.reg2hw.cqh.q,cq_tail_o=`SW.reg2hw.cqt.q,fq_head_o=`SW.reg2hw.fqh.q,fq_tail_o=`SW.reg2hw.fqt.q;
    assign cqcsr_o={20'b0,`SW.reg2hw.cqcsr.fence_w_ip.q,`SW.reg2hw.cqcsr.cmd_ill.q,`SW.reg2hw.cqcsr.cmd_to.q,`SW.reg2hw.cqcsr.cqmf.q,6'b0,`SW.reg2hw.cqcsr.cie.q,`SW.reg2hw.cqcsr.cqen.q};
    assign fqcsr_o={22'b0,`SW.reg2hw.fqcsr.fqof.q,`SW.reg2hw.fqcsr.fqmf.q,6'b0,`SW.reg2hw.fqcsr.fie.q,`SW.reg2hw.fqcsr.fqen.q};
    assign reg_valid_o=regreq[0].valid,reg_write_o=regreq[0].write,reg_addr_o=regreq[0].addr,reg_wdata_o=regreq[0].wdata;
    assign bridge_state_o=`BR.CS,bridge_psel_o=`BR.PSEL,bridge_penable_o=`BR.PENABLE;
    assign bridge_valids_o={`BR.RVALID_o,`BR.BVALID_o,`BR.RVALID,`BR.BVALID,`BR.ARVALID,`BR.WVALID,`BR.AWVALID,p_ar_valid_i,p_w_valid_i,p_aw_valid_i};
    assign fault_spill_o=`SW.i_rv_iommu_fault_arbiter.fault_valid_q,fault_fifo_o=`SW.i_rv_iommu_fault_arbiter.arbited_valid_q,fault_arb_o=`SW.i_rv_iommu_fault_arbiter.arbited_valid_d;
    assign regmap_idle_o=`SW.pn_regmap_idle;
    assign ddtp_pending_o=`SW.i_rv_iommu_regmap.write_l_q|`SW.i_rv_iommu_regmap.write_h_q;
    assign irq_pending_o=`SW.intp;
    if(!OmitFeatures) begin
        assign msi_state_o=`SW.gen_msi_ig.i_rv_iommu_msi_ig.state_q,msi_pending_o=`SW.gen_msi_ig.i_rv_iommu_msi_ig.pending_q,msi_edged_o=`SW.gen_msi_ig.i_rv_iommu_msi_ig.edged_q;
        assign dbg_state_o=`SW.gen_dbg_if.rv_iommu_dbg_if_ctl_i.state_q,dbg_go_o=`SW.reg2hw.tr_req_ctl.go.q;
    end else begin
        assign msi_state_o=0,msi_pending_o=0,msi_edged_o=0,dbg_state_o=0,dbg_go_o=0;
    end
    `undef SW
    `undef BR
endmodule
