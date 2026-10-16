// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Private research-model endpoint, authored by Codex, 2026-10-12.
// Frozen scalar uint32 HERO tile projected onto one source multiplier and one
// 96 KiB SRAM port. This is not original eight-PE software/CVA6 timing.
// Reused modules are compiled unchanged from their source files:
//   cva6/core/multiplier.sv
//   cva6/vendor/pulp-platform/tech_cells_generic/src/rtl/tc_sram.sv
//   cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_to_detailed_mem.sv
// Owner 0 = DMA/AXI, owner 1 = compute. All taps describe transfers consumed
// at the upcoming positive edge, and are sampled before that edge by runner.

module stream_spm_compute #(
    parameter type axi_req_t = logic,
    parameter type axi_resp_t = logic
) (
    input logic clk_i,
    input logic rst_ni,
    input axi_req_t axi_req_i,
    output axi_resp_t axi_resp_o,
    input logic comp_cmd_valid_i,
    input logic comp_aslot_i,
    input logic comp_bslot_i,
    input logic comp_cslot_i,
    input logic spm_pause_i,
    input logic [7:0] comp_s_i,
    input logic [7:0] comp_t_i,
    input logic [31:0] comp_agen_i,
    input logic [31:0] comp_bgen_i,
    input logic [31:0] comp_cgen_i,
    output logic comp_cmd_ready_o,
    output logic comp_busy_o,
    output logic comp_done_o,
    output logic spm_busy_o,
    output logic mem_grant_valid_o,
    output logic mem_grant_owner_o,
    output logic mem_grant_we_o,
    output logic mem_rsp_valid_o,
    output logic mem_rsp_owner_o,
    output logic mem_rsp_we_o,
    output logic mem_rsp_err_o,
    output logic mac_valid_o,
    output logic [7:0] comp_done_s_o,
    output logic [7:0] comp_done_t_o,
    output logic [7:0] mac_s_o,
    output logic [7:0] mac_t_o,
    output logic [7:0] mac_i_o,
    output logic [7:0] mac_j_o,
    output logic [7:0] mac_k_o,
    output logic [7:0] mem_grant_strb_o,
    output logic [31:0] comp_fault_o,
    output logic [31:0] mac_a_o,
    output logic [31:0] mac_b_o,
    output logic [31:0] mac_product_o,
    output logic [31:0] mac_acc_o,
    output logic [63:0] mem_grant_addr_o,
    output logic [63:0] mem_grant_wdata_o,
    output logic [63:0] mem_rsp_addr_o,
    output logic [63:0] mem_rsp_rdata_o,
    output logic spm_aw_valid_o,
    output logic spm_aw_ready_o,
    output logic [5:0] spm_aw_id_o,
    output logic [63:0] spm_aw_addr_o,
    output logic [7:0] spm_aw_len_o,
    output logic [2:0] spm_aw_size_o,
    output logic spm_ar_valid_o,
    output logic spm_ar_ready_o,
    output logic [5:0] spm_ar_id_o,
    output logic [63:0] spm_ar_addr_o,
    output logic [7:0] spm_ar_len_o,
    output logic [2:0] spm_ar_size_o,
    output logic spm_r_valid_o,
    output logic spm_r_ready_o,
    output logic [5:0] spm_r_id_o,
    output logic [1:0] spm_r_resp_o,
    output logic spm_b_valid_o,
    output logic spm_b_ready_o,
    output logic [5:0] spm_b_id_o,
    output logic [1:0] spm_b_resp_o,
    output logic spm_r_last_o,
    output logic spm_w_valid_o,
    output logic spm_w_ready_o,
    output logic spm_w_last_o,
    output logic [63:0] spm_r_data_o,
    output logic [63:0] spm_w_data_o,
    output logic [7:0] spm_w_strb_o
);

    localparam logic [63:0] SpmBase = 64'h0000_0001_0000_0000;
    localparam logic [63:0] SpmBytes = 64'd98304;
    localparam logic [63:0] SlotBytes = 64'd16384;
    localparam int unsigned NumWords = 12288;
    localparam int unsigned RamAddrWidth = 14;

    // One bridge bank, finite credit and finite request/response queues.
    logic bridge_busy;
    logic [0:0] dma_req, dma_gnt, dma_we, dma_lock;
    logic [0:0][63:0] dma_addr, dma_wdata, dma_rdata;
    logic [0:0][7:0] dma_strb;
    axi_pkg::atop_t [0:0] dma_atop;
    logic [0:0] dma_rvalid, dma_err;

    axi_to_detailed_mem #(
        .axi_req_t (axi_req_t),
        .axi_resp_t (axi_resp_t),
        .AddrWidth (64),
        .DataWidth (64),
        .IdWidth (6),
        .UserWidth ($bits(axi_req_i.aw.user)),
        .NumBanks (1),
        .BufDepth (1),
        .HideStrb (1'b0),
        .OutFifoDepth (1)
    ) i_axi_to_mem (
        .clk_i,
        .rst_ni,
        .busy_o (bridge_busy),
        .axi_req_i,
        .axi_resp_o,
        .mem_req_o (dma_req),
        .mem_gnt_i (dma_gnt),
        .mem_addr_o (dma_addr),
        .mem_wdata_o (dma_wdata),
        .mem_strb_o (dma_strb),
        .mem_atop_o (dma_atop),
        .mem_lock_o (dma_lock),
        .mem_we_o (dma_we),
        .mem_id_o (),
        .mem_user_o (),
        .mem_cache_o (),
        .mem_prot_o (),
        .mem_qos_o (),
        .mem_region_o (),
        .mem_rvalid_i (dma_rvalid),
        .mem_rdata_i (dma_rdata),
        .mem_err_i (dma_err),
        .mem_exokay_i (1'b0)
    );

    // Read and write AXI transaction counts cover internal queue windows that
    // the bridge's published busy_o alone might not cover. Counters decrement
    // only at real RLAST/B handshakes, including error responses.
    logic [15:0] axi_reads_q, axi_writes_q;
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            axi_reads_q <= '0;
            axi_writes_q <= '0;
        end else begin
            case ({(axi_req_i.ar_valid && axi_resp_o.ar_ready) ||
                   (axi_req_i.aw_valid && axi_resp_o.aw_ready && axi_req_i.aw.atop[5]),
                   axi_resp_o.r_valid && axi_req_i.r_ready && axi_resp_o.r.last})
                2'b10: axi_reads_q <= axi_reads_q + 16'd1;
                2'b01: axi_reads_q <= axi_reads_q - 16'd1;
                default: ;
            endcase
            case ({axi_req_i.aw_valid && axi_resp_o.aw_ready,
                   axi_resp_o.b_valid && axi_req_i.b_ready})
                2'b10: axi_writes_q <= axi_writes_q + 16'd1;
                2'b01: axi_writes_q <= axi_writes_q - 16'd1;
                default: ;
            endcase
        end
    end

    typedef enum logic [3:0] {
        Idle, AReq, AWait, BReq, BWait, MulIssue, MulWait,
        CReq, CWait, DonePending
    } comp_state_t;
    comp_state_t state_q;
    logic aslot_q, bslot_q, cslot_q;
    logic [7:0] s_q, t_q, i_q, j_q, k_q;
    // Generation tags belong to the accepted command and remain latched until
    // the next command. Slot ownership/generation legality is checked by the
    // common software controller; the hardware cannot infer DMA generations.
    logic [31:0] agen_q, bgen_q, cgen_q;
    logic [31:0] a_q, b_q, acc_q, fault_q;

    logic comp_req, comp_gnt, comp_we;
    logic [63:0] comp_addr, comp_wdata;
    logic [7:0] comp_strb;
    logic [63:0] a_scalar_addr, b_scalar_addr, c_scalar_addr;
    assign a_scalar_addr = SpmBase + (aslot_q ? SlotBytes : 64'd0) +
                           ({56'd0, i_q} << 10) + ({56'd0, k_q} << 2);
    assign b_scalar_addr = SpmBase + 64'd32768 + (bslot_q ? SlotBytes : 64'd0) +
                           ({56'd0, j_q} << 10) + ({56'd0, k_q} << 2);
    assign c_scalar_addr = SpmBase + 64'd65536 + (cslot_q ? SlotBytes : 64'd0) +
                           ({56'd0, i_q} << 10) + ({56'd0, t_q} << 6) +
                           ({56'd0, j_q} << 2);

    always_comb begin
        comp_req = 1'b0;
        comp_we = 1'b0;
        comp_addr = '0;
        comp_wdata = '0;
        comp_strb = '0;
        case (state_q)
            AReq: begin
                comp_req = 1'b1;
                comp_addr = a_scalar_addr & ~64'd7;
            end
            BReq: begin
                comp_req = 1'b1;
                comp_addr = b_scalar_addr & ~64'd7;
            end
            CReq: begin
                comp_req = 1'b1;
                comp_we = 1'b1;
                comp_addr = c_scalar_addr & ~64'd7;
                comp_wdata = c_scalar_addr[2] ? {acc_q, 32'd0} : {32'd0, acc_q};
                comp_strb = c_scalar_addr[2] ? 8'hf0 : 8'h0f;
            end
            default: ;
        endcase
    end

    // Round robin chooses the next owner whenever both sources request. Only
    // successful grants change priority. Pause never gates return paths.
    logic next_owner_q;
    logic grant_valid, grant_owner, grant_we, grant_err;
    logic [63:0] grant_addr, grant_wdata;
    logic [7:0] grant_strb;
    always_comb begin
        grant_valid = 1'b0;
        grant_owner = next_owner_q;
        grant_we = 1'b0;
        grant_addr = '0;
        grant_wdata = '0;
        grant_strb = '0;
        grant_err = 1'b0;
        dma_gnt = '0;
        comp_gnt = 1'b0;
        if (rst_ni && !spm_pause_i && (dma_req[0] || comp_req)) begin
            grant_valid = 1'b1;
            if (dma_req[0] && comp_req) grant_owner = next_owner_q;
            else grant_owner = comp_req;
            if (grant_owner) begin
                comp_gnt = 1'b1;
                grant_we = comp_we;
                grant_addr = comp_addr;
                grant_wdata = comp_wdata;
                grant_strb = comp_strb;
            end else begin
                dma_gnt[0] = 1'b1;
                grant_we = dma_we[0];
                grant_addr = dma_addr[0];
                grant_wdata = dma_wdata[0];
                grant_strb = dma_strb[0];
                // This endpoint is ordinary memory, not an atomic engine.
                grant_err = (dma_atop[0] != '0) || dma_lock[0];
            end
            // Validate the full 64-bit address before narrowing it to SRAM.
            // Requests in the reserved aperture beyond physical RAM return
            // errors one cycle later and never touch/alias a RAM word.
            if (grant_addr < SpmBase || grant_addr > SpmBase + SpmBytes - 64'd8)
                grant_err = 1'b1;
        end
    end

    // A single owner token accompanies every granted request, read OR write.
    // It is consumed exactly one edge later; a new grant may replace it on
    // that edge. SRAM writes commit at their grant edge and get an explicit
    // next-edge acknowledgment. Error/write response data is canonical zero.
    logic rsp_valid_q, rsp_owner_q, rsp_we_q, rsp_err_q;
    logic [63:0] rsp_addr_q, rsp_rdata;
    logic [0:0] ram_req, ram_we;
    logic [0:0][RamAddrWidth-1:0] ram_addr;
    logic [0:0][63:0] ram_wdata, ram_rdata;
    logic [0:0][7:0] ram_be;
    logic [63:0] ram_byte_offset;
    assign ram_byte_offset = grant_addr - SpmBase;
    assign ram_req[0] = grant_valid && !grant_err;
    assign ram_we[0] = grant_we;
    assign ram_addr[0] = ram_byte_offset[RamAddrWidth+2:3];
    assign ram_wdata[0] = grant_wdata;
    assign ram_be[0] = grant_strb;

    tc_sram #(
        .NumWords (NumWords),
        .DataWidth (64),
        .ByteWidth (8),
        .NumPorts (1),
        .Latency (1),
        .SimInit ("zeros"),
        .PrintSimCfg (1'b0)
    ) i_spm (
        .clk_i,
        .rst_ni,
        .req_i (ram_req),
        .we_i (ram_we),
        .addr_i (ram_addr),
        .wdata_i (ram_wdata),
        .be_i (ram_be),
        .rdata_o (ram_rdata)
    );

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            next_owner_q <= 1'b0;
            rsp_valid_q <= 1'b0;
            rsp_owner_q <= 1'b0;
            rsp_we_q <= 1'b0;
            rsp_err_q <= 1'b0;
            rsp_addr_q <= '0;
        end else begin
            rsp_valid_q <= grant_valid;
            if (grant_valid) begin
                next_owner_q <= ~grant_owner;
                rsp_owner_q <= grant_owner;
                rsp_we_q <= grant_we;
                rsp_err_q <= grant_err;
                rsp_addr_q <= grant_addr;
            end
        end
    end
    assign rsp_rdata = (rsp_we_q || rsp_err_q) ? 64'd0 : ram_rdata[0];
    assign dma_rvalid[0] = rsp_valid_q && !rsp_owner_q;
    assign dma_rdata[0] = rsp_rdata;
    assign dma_err[0] = rsp_err_q;

    // Use the unchanged, single-stage production multiplier. Inputs are
    // zero-extended uint32; low 32 result bits are exactly modular C product.
    logic mult_valid, mult_ready;
    logic [ariane_pkg::TRANS_ID_BITS-1:0] mult_trans_id;
    riscv::xlen_t mult_result;
    logic mult_issue, mac_consume;
    logic [31:0] acc_new;
    assign mult_issue = (state_q == MulIssue);
    multiplier i_multiplier (
        .clk_i,
        .rst_ni,
        .trans_id_i ('0),
        .mult_valid_i (mult_issue),
        .operator_i (ariane_pkg::MUL),
        .operand_a_i ({32'd0, a_q}),
        .operand_b_i ({32'd0, b_q}),
        .result_o (mult_result),
        .mult_valid_o (mult_valid),
        .mult_ready_o (mult_ready),
        .mult_trans_id_o (mult_trans_id)
    );
    assign mac_consume = (state_q == MulWait) && mult_valid && (fault_q == '0);
    assign acc_new = acc_q + mult_result[31:0];

    // Done is an observed handshake edge after the final C write has both
    // committed and returned its acknowledgment. Only compute-owned tokens
    // and multiplier output must be empty. Unrelated DMA activity must not
    // introduce an artificial dependency into the application's tile release.
    logic done_fire;
    assign done_fire = (state_q == DonePending) &&
                       !(rsp_valid_q && rsp_owner_q) && !mult_valid;
    assign comp_cmd_ready_o = rst_ni && (state_q == Idle) && (fault_q == '0);
    assign comp_busy_o = (state_q != Idle);
    assign comp_done_o = done_fire;
    assign comp_done_s_o = s_q;
    assign comp_done_t_o = t_q;
    assign comp_fault_o = fault_q;
    assign spm_busy_o = bridge_busy || dma_req[0] || comp_req || rsp_valid_q || mult_valid ||
                        (axi_reads_q != '0) || (axi_writes_q != '0);

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            state_q <= Idle;
            aslot_q <= 1'b0;
            bslot_q <= 1'b0;
            cslot_q <= 1'b0;
            s_q <= '0;
            t_q <= '0;
            i_q <= '0;
            j_q <= '0;
            k_q <= '0;
            agen_q <= '0;
            bgen_q <= '0;
            cgen_q <= '0;
            a_q <= '0;
            b_q <= '0;
            acc_q <= '0;
            fault_q <= '0;
        end else begin
            case (state_q)
                Idle: begin
                    if (comp_cmd_valid_i && comp_cmd_ready_o) begin
                        aslot_q <= comp_aslot_i;
                        bslot_q <= comp_bslot_i;
                        cslot_q <= comp_cslot_i;
                        s_q <= comp_s_i;
                        t_q <= comp_t_i;
                        agen_q <= comp_agen_i;
                        bgen_q <= comp_bgen_i;
                        cgen_q <= comp_cgen_i;
                        i_q <= '0;
                        j_q <= '0;
                        k_q <= '0;
                        acc_q <= '0;
                        if (comp_s_i >= 8'd16 || comp_t_i >= 8'd16) begin
                            fault_q[0] <= 1'b1;
                        end else begin
                            state_q <= AReq;
                        end
                    end
                end
                AReq: if (comp_gnt) state_q <= AWait;
                AWait: begin
                    if (rsp_valid_q && rsp_owner_q) begin
                        if (rsp_err_q || rsp_we_q) begin
                            fault_q[1] <= 1'b1;
                            state_q <= Idle;
                        end else begin
                            a_q <= k_q[0] ? rsp_rdata[63:32] : rsp_rdata[31:0];
                            state_q <= BReq;
                        end
                    end
                end
                BReq: if (comp_gnt) state_q <= BWait;
                BWait: begin
                    if (rsp_valid_q && rsp_owner_q) begin
                        if (rsp_err_q || rsp_we_q) begin
                            fault_q[1] <= 1'b1;
                            state_q <= Idle;
                        end else begin
                            b_q <= k_q[0] ? rsp_rdata[63:32] : rsp_rdata[31:0];
                            state_q <= MulIssue;
                        end
                    end
                end
                MulIssue: if (mult_ready) state_q <= MulWait;
                MulWait: begin
                    if (mac_consume) begin
                        if (mult_trans_id != '0) begin
                            fault_q[2] <= 1'b1;
                            state_q <= Idle;
                        end else begin
                            acc_q <= acc_new;
                            if (k_q == 8'd255) begin
                                state_q <= CReq;
                            end else begin
                                k_q <= k_q + 8'd1;
                                state_q <= AReq;
                            end
                        end
                    end
                end
                CReq: if (comp_gnt) state_q <= CWait;
                CWait: begin
                    if (rsp_valid_q && rsp_owner_q) begin
                        if (rsp_err_q || !rsp_we_q) begin
                            fault_q[1] <= 1'b1;
                            state_q <= Idle;
                        end else if (i_q == 8'd15 && j_q == 8'd15) begin
                            state_q <= DonePending;
                        end else begin
                            acc_q <= '0;
                            k_q <= '0;
                            if (j_q == 8'd15) begin
                                j_q <= '0;
                                i_q <= i_q + 8'd1;
                            end else begin
                                j_q <= j_q + 8'd1;
                            end
                            state_q <= AReq;
                        end
                    end
                end
                DonePending: if (done_fire) state_q <= Idle;
                default: begin
                    fault_q[3] <= 1'b1;
                    state_q <= Idle;
                end
            endcase
        end
    end

    // Pre-edge observational taps. Values are meaningful when their valid is
    // high; they do not grant permission or modify any endpoint handshake.
    assign mem_grant_valid_o = grant_valid;
    assign mem_grant_owner_o = grant_owner;
    assign mem_grant_we_o = grant_we;
    assign mem_grant_addr_o = grant_addr;
    assign mem_grant_wdata_o = grant_wdata;
    assign mem_grant_strb_o = grant_strb;
    assign mem_rsp_valid_o = rsp_valid_q;
    assign mem_rsp_owner_o = rsp_owner_q;
    assign mem_rsp_we_o = rsp_we_q;
    assign mem_rsp_err_o = rsp_err_q;
    assign mem_rsp_addr_o = rsp_addr_q;
    assign mem_rsp_rdata_o = rsp_rdata;
    assign mac_valid_o = mac_consume;
    assign mac_s_o = s_q;
    assign mac_t_o = t_q;
    assign mac_i_o = i_q;
    assign mac_j_o = j_q;
    assign mac_k_o = k_q;
    assign mac_a_o = a_q;
    assign mac_b_o = b_q;
    assign mac_product_o = mult_result[31:0];
    assign mac_acc_o = acc_new;

    assign spm_aw_valid_o = axi_req_i.aw_valid;
    assign spm_aw_ready_o = axi_resp_o.aw_ready;
    assign spm_aw_id_o = axi_req_i.aw.id;
    assign spm_aw_addr_o = axi_req_i.aw.addr;
    assign spm_aw_len_o = axi_req_i.aw.len;
    assign spm_aw_size_o = axi_req_i.aw.size;
    assign spm_ar_valid_o = axi_req_i.ar_valid;
    assign spm_ar_ready_o = axi_resp_o.ar_ready;
    assign spm_ar_id_o = axi_req_i.ar.id;
    assign spm_ar_addr_o = axi_req_i.ar.addr;
    assign spm_ar_len_o = axi_req_i.ar.len;
    assign spm_ar_size_o = axi_req_i.ar.size;
    assign spm_r_valid_o = axi_resp_o.r_valid;
    assign spm_r_ready_o = axi_req_i.r_ready;
    assign spm_r_id_o = axi_resp_o.r.id;
    assign spm_r_resp_o = axi_resp_o.r.resp;
    assign spm_r_last_o = axi_resp_o.r.last;
    assign spm_r_data_o = axi_resp_o.r.data;
    assign spm_b_valid_o = axi_resp_o.b_valid;
    assign spm_b_ready_o = axi_req_i.b_ready;
    assign spm_b_id_o = axi_resp_o.b.id;
    assign spm_b_resp_o = axi_resp_o.b.resp;
    assign spm_w_valid_o = axi_req_i.w_valid;
    assign spm_w_ready_o = axi_resp_o.w_ready;
    assign spm_w_last_o = axi_req_i.w.last;
    assign spm_w_data_o = axi_req_i.w.data;
    assign spm_w_strb_o = axi_req_i.w.strb;
endmodule : stream_spm_compute
