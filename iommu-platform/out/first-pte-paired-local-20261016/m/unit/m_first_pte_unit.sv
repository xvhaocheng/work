// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Finite protocol fixture: actual local gate and actual modified RR arbiter.
// Clock/reset and producer/consumer signals are driven by m_first_pte_unit.cpp.
module m_first_pte_unit (
  input logic clk, rst_n,
  input logic enable, pair_qualified, start, active, bank,
  input logic [3:0][5:0] ids,
  input logic [3:0] valid,
  input logic ready,
  output logic [3:0] allow, grant,
  output logic selected_valid,
  output logic [5:0] selected_id,
  output logic [1:0] selected_index,
  output logic [15:0] state,
  output logic arm, p1_commit, timeout_event, masked,
  output logic [2:0] quota
);
  m_first_pte_gate i_gate (
    .clk_i(clk), .rst_ni(rst_n), .enable_i(enable),
    .pair_qualified_i(pair_qualified), .start_i(start),
    .ptw_active_i(active), .start_bank_i(bank), .raw_id_i(ids),
    .raw_valid_i(valid), .select_valid_i(selected_valid),
    .select_ready_i(ready), .select_id_i(selected_id),
    .allow_o(allow), .state_o(state), .arm_o(arm),
    .p1_commit_o(p1_commit), .timeout_o(timeout_event), .masked_o(masked)
  );
  rr_arb_tree #(
    .NumIn(4), .DataWidth(6), .AxiVldRdy(1'b1), .LockIn(1'b1),
    .EnableReqMask(1'b1), .EnableDsFirst(1'b1), .DsPort(3)
  ) i_arbiter (
    .clk_i(clk), .rst_ni(rst_n), .flush_i(1'b0), .rr_i(2'b0),
    .req_i(valid), .req_allow_i(allow), .ds_quota_o(quota),
    .gnt_o(grant), .data_i(ids), .req_o(selected_valid),
    .gnt_i(ready), .data_o(selected_id), .idx_o(selected_index)
  );
endmodule
