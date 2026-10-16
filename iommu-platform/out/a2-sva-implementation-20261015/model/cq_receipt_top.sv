// Test-only scalar boundary wrapper; production CQ handler is unmodified.
`include "axi/typedef.svh"
module cq_receipt_top
  import rv_iommu::*;
  import rv_iommu_cfg::*;
(
 input logic clk_i, rst_ni, enable_i,
 input logic [31:0] tail_i,
 input logic ar_ready_i, r_valid_i, r_last_i,
 input logic [63:0] r_data_i,
 input logic [1:0] r_resp_i,
 input logic aw_ready_i, w_ready_i, b_valid_i,
 input logic [1:0] b_resp_i,
 output logic ar_valid_o, r_ready_o,
 output logic [63:0] ar_addr_o,
 output logic [3:0] ar_id_o,
 output logic [7:0] ar_len_o,
 output logic [2:0] ar_size_o,
 output logic [1:0] ar_burst_o,
 output logic aw_valid_o, w_valid_o, b_ready_o,
 output logic [63:0] aw_addr_o, w_data_o,
 output logic [3:0] aw_id_o,
 output logic [7:0] aw_len_o, w_strb_o,
 output logic [2:0] aw_size_o,
 output logic [1:0] aw_burst_o,
 output logic w_last_o,
 output logic [31:0] head_o,
 output logic on_o, mf_o, ill_o, to_o,
 output logic inv_ddt_o, inv_pdt_o, inv_dv_o,
 output logic [23:0] inv_did_o,
 output logic [19:0] inv_pid_o,
 output logic inv_vma_o, inv_gvma_o, inv_av_o, inv_gv_o, inv_pscv_o,
 output logic [28:0] inv_vpn_o,
 output logic [19:0] inv_pscid_o,
 output logic [15:0] inv_gscid_o
);
 typedef logic [63:0] addr_t;
 typedef logic [55:0] paddr_t;
 `AXI_TYPEDEF_AW_CHAN_T(aw_t, logic [63:0], logic [3:0], logic [0:0])
 `AXI_TYPEDEF_W_CHAN_T(w_t, logic [63:0], logic [7:0], logic [0:0])
 `AXI_TYPEDEF_B_CHAN_T(b_t, logic [3:0], logic [0:0])
 `AXI_TYPEDEF_AR_CHAN_T(ar_t, logic [63:0], logic [3:0], logic [0:0])
 `AXI_TYPEDEF_R_CHAN_T(r_t, logic [63:0], logic [3:0], logic [0:0])
 `AXI_TYPEDEF_REQ_T(req_t, aw_t, w_t, ar_t)
 `AXI_TYPEDEF_RESP_T(resp_t, b_t, r_t)
 function automatic rv_iommu_cfg_t config_cq();
   rv_iommu_cfg_t cfg = NullCfg;
   cfg.PAddrWidth = 56;
   cfg.AxiAddrWidth = 64;
   cfg.AxiDataWidth = 64;
   cfg.AxiIdWidth = 4;
   cfg.AxiUserWidth = 1;
   return cfg;
 endfunction
 localparam rv_iommu_cfg_t Cfg = config_cq();
 req_t req;
 resp_t resp;
 xdtc_inval_t dir_inv;
 iotlb_inval_t io_inv;
 logic [31:0] head_next;
 logic head_wen;
 logic mf_next, mf_wen, ill_next, ill_wen, to_next, to_wen;
 logic on_wen, busy, busy_wen, fence_next, fence_wen, cip, cip_wen;
 always_comb begin
   resp = '0;
   resp.ar_ready = ar_ready_i;
   resp.r_valid = r_valid_i;
   resp.r.id = 4'd3;
   resp.r.data = r_data_i;
   resp.r.resp = r_resp_i;
   resp.r.last = r_last_i;
   resp.aw_ready = aw_ready_i;
   resp.w_ready = w_ready_i;
   resp.b_valid = b_valid_i;
   resp.b.id = 4'd0;
   resp.b.resp = b_resp_i;
 end
 always_ff @(posedge clk_i or negedge rst_ni) begin
   if (!rst_ni) begin head_o <= 0; mf_o <= 0; ill_o <= 0; to_o <= 0; end
   else begin
     if (head_wen) head_o <= head_next;
     if (mf_wen) mf_o <= mf_next;
     if (ill_wen) ill_o <= ill_next;
     if (to_wen) to_o <= to_next;
   end
 end
 rv_iommu_cq_handler #(.RVIOMMUCfg(Cfg), .addr_t(addr_t), .paddr_t(paddr_t), .axi_req_t(req_t), .axi_resp_t(resp_t)) dut (
   .clk_i, .rst_ni, .cqb_ppn_i(44'h10), .cqb_size_i(5'd2),
   .cq_tail_i(tail_i), .cq_head_i(head_o), .cq_head_o(head_next), .cq_head_wen_o(head_wen),
   .cqcsr_en_i(enable_i), .cqcsr_ie_i(1'b0), .cqcsr_on_o(on_o), .cqcsr_on_wen_o(on_wen),
   .cqcsr_busy_o(busy), .cqcsr_busy_wen_o(busy_wen),
   .cqcsr_mf_i(mf_o), .cqcsr_mf_o(mf_next), .cqcsr_mf_wen_o(mf_wen),
   .cqcsr_ill_i(ill_o), .cqcsr_ill_o(ill_next), .cqcsr_ill_wen_o(ill_wen),
   .cqcsr_to_i(to_o), .cqcsr_to_o(to_next), .cqcsr_to_wen_o(to_wen),
   .cqcsr_fence_i(1'b0), .cqcsr_fence_o(fence_next), .cqcsr_fence_wen_o(fence_wen),
   .ipsr_cip_o(cip), .ipsr_cip_wen_o(cip_wen), .wsi_en_i(1'b0),
   .iodirinval_o(dir_inv), .iotinval_o(io_inv),
   .atsinval_o(), .atsinval_valid_o(), .atsinval_ready_i(1'b1),
   .atsinval_to_i(1'b0), .atsinval_inflight_i(1'b0), .ats_fence_valid_o(), .ats_fence_ready_i(1'b1),
   .atsprgr_o(), .atsprgr_valid_o(), .atsprgr_ready_i(1'b1), .mem_resp_i(resp), .mem_req_o(req)
 );
 assign ar_valid_o=req.ar_valid, ar_addr_o=req.ar.addr, ar_id_o=req.ar.id,
        ar_len_o=req.ar.len, ar_size_o=req.ar.size, ar_burst_o=req.ar.burst, r_ready_o=req.r_ready;
 assign aw_valid_o=req.aw_valid, aw_addr_o=req.aw.addr, aw_id_o=req.aw.id,
        aw_len_o=req.aw.len, aw_size_o=req.aw.size, aw_burst_o=req.aw.burst;
 assign w_valid_o=req.w_valid, w_data_o=req.w.data, w_strb_o=req.w.strb,
        w_last_o=req.w.last, b_ready_o=req.b_ready;
 assign inv_ddt_o=dir_inv.inval_ddtc, inv_pdt_o=dir_inv.inval_pdtc,
        inv_dv_o=dir_inv.dv, inv_did_o=dir_inv.did, inv_pid_o=dir_inv.pid;
 assign inv_vma_o=io_inv.inval_vma, inv_gvma_o=io_inv.inval_gvma, inv_av_o=io_inv.av,
        inv_gv_o=io_inv.gv, inv_pscv_o=io_inv.pscv, inv_vpn_o=io_inv.vpn,
        inv_pscid_o=io_inv.pscid, inv_gscid_o=io_inv.gscid;
endmodule
