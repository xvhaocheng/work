// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Finite real TL/walker/cache test. Test-only hierarchical taps, no feedback.
`include "axi/typedef.svh"
module history_n_top import rv_iommu::*; import rv_iommu_reg_pkg::*; (
    input logic clk_i, rst_ni, compare_i, partition_i, clear_i,
    input logic history_isolate_i,
    input logic req_valid_i, resp_ready_i, n_enable_i, req_bank_i, restricted_i,
    input logic req_pid_i, req_priv_i, req_debug_i, fault_ready_i, dc_inval_i,
    input logic [3:0] ddt_mode_i,
    input logic [23:0] did_i,
    input logic [63:0] iova_i,
    input logic [5:0] type_i,
    input logic inval_i,
    output logic req_ready_o, resp_valid_o, resp_error_o, resp_bank_o, resp_mrif_o,
    output logic n_accept_o, n_busy_o, n_window_o, n_probe_o, n_touch_o,
    output logic probe_no_touch_o, ddwalk_init_o, msi_walk_o, msi_fill_o,
    output logic [2:0] n_state_o,
    output logic fault_valid_o,
    output logic [11:0] fault_cause_o,
    output logic [23:0] n_did_o, fault_did_o,
    output logic [63:0] fault_iova_o,
    output logic [55:0] resp_addr_o,
    output logic [11:0] fault_code_o,
    output logic gold_equal_o, off_equal_o,
    output logic fill_o,
    output logic [23:0] fill_did_o,
    output logic [15:0] fill_mask_o, fill_victim_o, valid_o,
    output logic [14:0] plru_o,
    output logic ddt_ar_valid_o, ddt_r_ready_o,
    output logic [63:0] ddt_ar_addr_o,
    output logic [3:0] ddt_ar_id_o,
    output logic [7:0] ddt_ar_len_o,
    input logic ddt_ar_ready_i, ddt_r_valid_i, ddt_r_last_i,
    input logic [63:0] ddt_r_data_i,
    input logic [3:0] ddt_r_id_i,
    output logic ptw_ar_valid_o, ptw_r_ready_o,
    output logic [63:0] ptw_ar_addr_o,
    output logic [3:0] ptw_ar_id_o,
    output logic [7:0] ptw_ar_len_o,
    input logic ptw_ar_ready_i, ptw_r_valid_i, ptw_r_last_i,
    input logic [63:0] ptw_r_data_i,
    input logic [3:0] ptw_r_id_i
);
    typedef logic [63:0] addr_t;
    typedef logic [55:0] paddr_t;
    typedef struct packed {logic bank; addr_t iova; device_id_t did; logic pid_valid;
        process_id_t pid; ttype_t ttype; logic priv; logic is_debug;} req_t;
    typedef struct packed {logic bank; logic error, ignore; paddr_t spaddr; logic is_mrif;
        mrifc_content_t mrif_data; logic [2:0] range; logic bypass;
        cause_t fault_code; logic x,w,r,g,u,sum,t2gpa;} rsp_t;
    typedef struct packed {logic error,is_superpage; logic [1:0] pbmt; ppn_t ppn;} dbg_t;
    typedef struct packed {ttype_t trans_type; cause_t cause_code; addr_t iova;
        logic [40:0] gpaddr; device_id_t did; logic pv; process_id_t pid;
        logic is_supervisor,is_guest_pf,is_implicit;} fault_t;
    `AXI_TYPEDEF_ALL(axi, addr_t, logic[3:0], logic[63:0], logic[7:0], logic[0:0])
    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg = '{default:'0,
        NumOutstandingTrans:8, WFifoDepth:32, MrifFifoDepth:8, FaultFifoDepth:8,
        NumIotlbEntries:16, NumDdtcEntries:8, NumPdtcEntries:8, NumMrifcEntries:8,
        InclPC:1, InclAxiBC:1, InclDbg:1, InclATS:0,
        MSITrans:rv_iommu_cfg::MSI_BT_MRIF, IGS:rv_iommu_cfg::BOTH,
        NumIntVec:16, NumHpmCounters:31, PAddrWidth:56, AxiAddrWidth:64,
        AxiDataWidth:64, AxiIdWidth:4, AxiProgIdWidth:4, AxiUserWidth:1};
    req_t request;
    always_comb begin
        request='0; request.bank=req_bank_i; request.did=did_i; request.iova=iova_i; request.ttype=ttype_t'(type_i);
        request.pid_valid=req_pid_i; request.priv=req_priv_i; request.is_debug=req_debug_i;
    end
    rsp_t rsp[3]; logic [2:0] ready, valid, fv;
    hpm_event_t hpm[3]; fault_t faults[3];
    axi_req_t ddt_req[3], pdt_req[3], ptw_req[3], msi_req[3];
    axi_resp_t ddt_rsp, ptw_rsp;
    always_comb begin
        ddt_rsp='0; ddt_rsp.ar_ready=ddt_ar_ready_i;
        ddt_rsp.r_valid=ddt_r_valid_i; ddt_rsp.r.data=ddt_r_data_i;
        ddt_rsp.r.id=ddt_r_id_i; ddt_rsp.r.last=ddt_r_last_i;
        ptw_rsp='0; ptw_rsp.ar_ready=ptw_ar_ready_i;
        ptw_rsp.r_valid=ptw_r_valid_i; ptw_rsp.r.data=ptw_r_data_i;
        ptw_rsp.r.id=ptw_r_id_i; ptw_rsp.r.last=ptw_r_last_i;
    end
    iommu_reg2hw_capabilities_reg_t caps;
    iommu_reg2hw_fctl_reg_t fctl;
    iommu_reg2hw_ddtp_reg_t ddtp;
    iotlb_inval_t inv; xdtc_inval_t dinv;
    always_comb begin
        caps='0; caps.sv39.q=1; caps.sv39x4.q=1; caps.pas.q=56; caps.msi_flat.q=1; caps.msi_mrif.q=1;
        fctl='0; ddtp='0; ddtp.iommu_mode.q=ddt_mode_i; ddtp.ppn.q=44'h10;
        inv='0; inv.inval_vma=inval_i; dinv='0; dinv.inval_ddtc=dc_inval_i;
    end
    `define PARAMS .RVIOMMUCfg(Cfg), .addr_t(addr_t), .paddr_t(paddr_t), .dc_t(dc_ext_t), .ddtc_up_t(ddtc_up_ext_t), .trans_req_data_t(req_t), .trans_resp_data_t(rsp_t), .dbg_resp_t(dbg_t), .fault_data_t(fault_t), .axi_req_t(axi_req_t), .axi_resp_t(axi_resp_t)
    `define PORTS .clk_i,.rst_ni,.trans_req_data_i(request),.trans_req_valid_i(req_valid_i),.trans_req_ready_o(ready[i]),.ats_trans_req_data_i('0),.ats_trans_req_valid_i(1'b0),.ats_trans_req_ready_o(),.debug_req_data_i('0),.debug_req_valid_i(1'b0),.debug_req_ready_o(),.trans_resp_data_o(rsp[i]),.trans_resp_valid_o(valid[i]),.trans_resp_ready_i(resp_ready_i),.ats_trans_resp_data_o(),.ats_trans_resp_valid_o(),.ats_trans_resp_ready_i(1'b1),.ats_ddtc_valid_i(1'b0),.ats_ddtc_did_i('0),.ats_ddtc_ready_o(),.ats_ddtc_tc_o(),.debug_resp_data_o(),.debug_resp_valid_o(),.debug_resp_ready_i(1'b1),.capabilities_i(caps),.fctl_i(fctl),.ddtp_i(ddtp),.iotinval_i(inv),.iodirinval_i(dinv),.hpm_events_o(hpm[i]),.ddtw_axi_req_o(ddt_req[i]),.ddtw_axi_resp_i(ddt_rsp),.pdtw_axi_req_o(pdt_req[i]),.pdtw_axi_resp_i('0),.ptw_axi_req_o(ptw_req[i]),.ptw_axi_resp_i(ptw_rsp),.msiptw_axi_req_o(msi_req[i]),.msiptw_axi_resp_i('0),.fault_data_o(faults[i]),.fault_valid_o(fv[i]),.fault_ready_i(fault_ready_i)
    for(genvar i=0;i<3;i++) begin : lane
        if(i==0) begin : candidate
            rv_iommu_tl_wrap #(`PARAMS, .PolicyEnable(1), .NonblockEnable(1), .HistoryControlEnable(1)) dut (
                `PORTS, .policy_partition_i(partition_i), .policy_clear_i(clear_i),
                .history_isolate_i(history_isolate_i), .nonblock_enable_i(n_enable_i), .restricted_bank_i(restricted_i));
        end else if(i==1) begin : reference_iso
            rv_iommu_tl_wrap_old_iso #(`PARAMS, .PolicyEnable(1), .NonblockEnable(1)) dut (
                `PORTS, .policy_partition_i(partition_i), .policy_clear_i(clear_i),
                .nonblock_enable_i(n_enable_i), .restricted_bank_i(restricted_i));
        end else begin : reference_shared
            rv_iommu_tl_wrap_old_shared #(`PARAMS, .PolicyEnable(1), .NonblockEnable(1)) dut (
                `PORTS, .policy_partition_i(partition_i), .policy_clear_i(clear_i),
                .nonblock_enable_i(n_enable_i), .restricted_bank_i(restricted_i));
        end
    end
    `undef PARAMS
    `undef PORTS
    assign n_accept_o=lane[0].candidate.dut.nonblock_fire_o;
    assign n_did_o=lane[0].candidate.dut.nonblock_did_o;
    assign n_busy_o=lane[0].candidate.dut.nonblock_busy_o;
    assign n_window_o=lane[0].candidate.dut.n_window;
    assign n_probe_o=lane[0].candidate.dut.n_ddtc_lu || lane[0].candidate.dut.n_iotlb_lu;
    assign n_touch_o=lane[0].candidate.dut.n_ddtc_touch || lane[0].candidate.dut.n_iotlb_touch;
    assign n_state_o=lane[0].candidate.dut.gen_nonblock_hit.i_hit.state_q;
    assign probe_no_touch_o=lane[0].candidate.dut.i_rv_iommu_ddtc.plru_tree_q==lane[0].candidate.dut.i_rv_iommu_ddtc.plru_tree_n &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.plru_tree_q==lane[0].candidate.dut.i_rv_iommu_iotlb.plru_tree_n;
    assign ddwalk_init_o=lane[0].candidate.dut.ddtw_init;
    assign msi_walk_o=lane[0].candidate.dut.gen_msi_support_tl.msi_ptw_init || msi_req[0].ar_valid;
    assign msi_fill_o=lane[0].candidate.dut.msi_iotlb_update.update;
    assign fault_valid_o=fv[0]; assign fault_cause_o=faults[0].cause_code;
    assign fault_did_o=faults[0].did; assign fault_iova_o=faults[0].iova;
    assign resp_bank_o=rsp[0].bank; assign resp_mrif_o=rsp[0].is_mrif;
    assign req_ready_o=ready[0]; assign resp_valid_o=valid[0];
    assign resp_error_o=rsp[0].error; assign resp_addr_o=rsp[0].spaddr;
    assign fault_code_o=rsp[0].fault_code;
    assign gold_equal_o=ready[0]==ready[1] &&
        valid[0]==valid[1] &&
        rsp[0]==rsp[1] &&
        fv[0]==fv[1] &&
        faults[0]==faults[1] &&
        hpm[0]==hpm[1] &&
        ddt_req[0]==ddt_req[1] &&
        pdt_req[0]==pdt_req[1] &&
        ptw_req[0]==ptw_req[1] &&
        msi_req[0]==msi_req[1] &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.tags_q==lane[1].reference_iso.dut.i_rv_iommu_iotlb.tags_q &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.tags_n==lane[1].reference_iso.dut.i_rv_iommu_iotlb.tags_n &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.content_q==lane[1].reference_iso.dut.i_rv_iommu_iotlb.content_q &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.content_n==lane[1].reference_iso.dut.i_rv_iommu_iotlb.content_n &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.plru_tree_q==lane[1].reference_iso.dut.i_rv_iommu_iotlb.plru_tree_q &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.plru_tree_n==lane[1].reference_iso.dut.i_rv_iommu_iotlb.plru_tree_n &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.replace_en==lane[1].reference_iso.dut.i_rv_iommu_iotlb.replace_en &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.tags_q==lane[1].reference_iso.dut.i_rv_iommu_ddtc.tags_q &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.tags_n==lane[1].reference_iso.dut.i_rv_iommu_ddtc.tags_n &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.content_q==lane[1].reference_iso.dut.i_rv_iommu_ddtc.content_q &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.content_n==lane[1].reference_iso.dut.i_rv_iommu_ddtc.content_n &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.plru_tree_q==lane[1].reference_iso.dut.i_rv_iommu_ddtc.plru_tree_q &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.plru_tree_n==lane[1].reference_iso.dut.i_rv_iommu_ddtc.plru_tree_n &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.replace_en==lane[1].reference_iso.dut.i_rv_iommu_ddtc.replace_en;
    assign off_equal_o=ready[0]==ready[2] &&
        valid[0]==valid[2] &&
        rsp[0]==rsp[2] &&
        fv[0]==fv[2] &&
        faults[0]==faults[2] &&
        hpm[0]==hpm[2] &&
        ddt_req[0]==ddt_req[2] &&
        pdt_req[0]==pdt_req[2] &&
        ptw_req[0]==ptw_req[2] &&
        msi_req[0]==msi_req[2] &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.tags_q==lane[2].reference_shared.dut.i_rv_iommu_iotlb.tags_q &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.tags_n==lane[2].reference_shared.dut.i_rv_iommu_iotlb.tags_n &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.content_q==lane[2].reference_shared.dut.i_rv_iommu_iotlb.content_q &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.content_n==lane[2].reference_shared.dut.i_rv_iommu_iotlb.content_n &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.plru_tree_q==lane[2].reference_shared.dut.i_rv_iommu_iotlb.plru_tree_q &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.plru_tree_n==lane[2].reference_shared.dut.i_rv_iommu_iotlb.plru_tree_n &&
        lane[0].candidate.dut.i_rv_iommu_iotlb.replace_en==lane[2].reference_shared.dut.i_rv_iommu_iotlb.replace_en &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.tags_q==lane[2].reference_shared.dut.i_rv_iommu_ddtc.tags_q &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.tags_n==lane[2].reference_shared.dut.i_rv_iommu_ddtc.tags_n &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.content_q==lane[2].reference_shared.dut.i_rv_iommu_ddtc.content_q &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.content_n==lane[2].reference_shared.dut.i_rv_iommu_ddtc.content_n &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.plru_tree_q==lane[2].reference_shared.dut.i_rv_iommu_ddtc.plru_tree_q &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.plru_tree_n==lane[2].reference_shared.dut.i_rv_iommu_ddtc.plru_tree_n &&
        lane[0].candidate.dut.i_rv_iommu_ddtc.replace_en==lane[2].reference_shared.dut.i_rv_iommu_ddtc.replace_en;
    assign fill_o=lane[0].candidate.dut.iotlb_update.update && !clear_i && !inval_i;
    assign fill_did_o=lane[0].candidate.dut.req_data.did;
    assign fill_mask_o=lane[0].candidate.dut.i_rv_iommu_iotlb.policy_mask_i;
    assign fill_victim_o=lane[0].candidate.dut.i_rv_iommu_iotlb.replace_en;
    assign plru_o=lane[0].candidate.dut.i_rv_iommu_iotlb.plru_tree_q;
    for(genvar j=0;j<16;j++) begin : tags
        assign valid_o[j]=lane[0].candidate.dut.i_rv_iommu_iotlb.tags_q[j].valid;
    end
    assign ddt_ar_valid_o=ddt_req[0].ar_valid; assign ddt_ar_addr_o=ddt_req[0].ar.addr;
    assign ddt_ar_len_o=ddt_req[0].ar.len; assign ddt_ar_id_o=ddt_req[0].ar.id;
    assign ddt_r_ready_o=ddt_req[0].r_ready;
    assign ptw_ar_valid_o=ptw_req[0].ar_valid; assign ptw_ar_addr_o=ptw_req[0].ar.addr;
    assign ptw_ar_len_o=ptw_req[0].ar.len; assign ptw_ar_id_o=ptw_req[0].ar.id;
    assign ptw_r_ready_o=ptw_req[0].r_ready;
endmodule
