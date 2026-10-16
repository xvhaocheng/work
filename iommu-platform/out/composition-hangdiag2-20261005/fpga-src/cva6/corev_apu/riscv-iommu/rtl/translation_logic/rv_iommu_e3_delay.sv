// SPDX-License-Identifier: Solderpad-0.51
// One response register, identical D=0/D=128 paths. Only one selected response
// per ARM is held. Abort/disable never drops a captured response.
module rv_iommu_e3_delay #(
    parameter type req_t = logic,
    parameter type resp_t = logic
) (
    input logic clk_i, rst_ni, enable_i, arm_i, eligible_i, delay128_i,
    input req_t walker_req_i,
    output resp_t walker_resp_o,
    output req_t memory_req_o,
    input resp_t memory_resp_i,
    output logic capture_o, available_o, consume_o, error_o,
    output logic [7:0] extra_o
);
    resp_t saved_q;
    logic full_q, selected_q, used_q, announced_q;
    logic [7:0] remaining_q, held_q;
    logic take, give;
    always_comb begin
        memory_req_o = walker_req_i;
        walker_resp_o = memory_resp_i;
        if (enable_i || full_q) begin
            memory_req_o.r_ready = !full_q;
            walker_resp_o.r = saved_q.r;
            walker_resp_o.r_valid = full_q && remaining_q == 0;
        end
        take = (enable_i || full_q) && memory_resp_i.r_valid && memory_req_o.r_ready;
        give = full_q && remaining_q == 0 && walker_req_i.r_ready;
        capture_o = take && eligible_i && !used_q;
        available_o = full_q && selected_q && remaining_q == 0 && !announced_q;
        consume_o = give && selected_q;
        extra_o = held_q;
    end
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            full_q <= 0; selected_q <= 0; used_q <= 0; announced_q <= 0;
            remaining_q <= 0; held_q <= 0; saved_q <= '0; error_o <= 0;
        end else begin
            if (arm_i) begin
                used_q <= 0;
                error_o <= full_q;
                // Arm is only legal after software has drained both devices.
            end
            if (take) begin
                saved_q <= memory_resp_i;
                full_q <= 1;
                selected_q <= capture_o;
                announced_q <= 0;
                remaining_q <= capture_o && delay128_i ? 8'd128 : 0;
                held_q <= 0;
                if (capture_o) used_q <= 1;
                if (!memory_resp_i.r.last) error_o <= 1; // PTW uses single-beat reads.
            end
            if (full_q && remaining_q != 0) begin
                remaining_q <= remaining_q - 1'b1;
                held_q <= held_q + 1'b1;
            end
            if (available_o) announced_q <= 1;
            if (give) full_q <= 0;
        end
    end
endmodule
