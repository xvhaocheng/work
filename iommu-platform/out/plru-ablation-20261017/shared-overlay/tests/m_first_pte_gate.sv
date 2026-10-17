// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// FIRST-PTE-PAIRED-LOCAL-1, fixed V=DMA0/bank0, A=DMA1/bank1.
// f1 is one receiver capture on the real PTW start edge. No event queue,
// marker, AXI payload buffer, source capture or replay is introduced.
module m_first_pte_gate #(
  parameter int unsigned NumIn = 4,
  parameter int unsigned FullIdWidth = 6,
  parameter int unsigned H = 64,
  parameter int unsigned DsPort = 3,
  parameter int unsigned PayloadPort = 2,
  parameter int unsigned RemWidth = $clog2(H+1)
) (
  input logic clk_i,
  input logic rst_ni,
  input logic enable_i,
  input logic pair_qualified_i,
  input logic start_i,
  input logic ptw_active_i,
  input logic start_bank_i,
  input logic [NumIn-1:0][FullIdWidth-1:0] raw_id_i,
  input logic [NumIn-1:0] raw_valid_i,
  input logic select_valid_i,
  input logic select_ready_i,
  input logic [FullIdWidth-1:0] select_id_i,
  output logic [NumIn-1:0] allow_o,
  output logic [15:0] state_o,
  output logic arm_o,
  output logic p1_commit_o,
  output logic timeout_o,
  output logic masked_o
);
  logic pending_q, pending_d, escape_q, escape_d;
  logic [RemWidth-1:0] remaining_q, remaining_d;
  logic gate_active, walk_retired, non_ds_commit, exit_event;

  // Full existing ID: {xbar source[1:0], original DMA source[1:0], local ID[1:0]}.
  localparam logic [FullIdWidth-1:0] PtwId = FullIdWidth'(48);
  localparam logic [FullIdWidth-1:0] AIdMask = FullIdWidth'(6'h3c);
  localparam logic [FullIdWidth-1:0] AIdPrefix = FullIdWidth'(6'h24);

  initial begin
    if (NumIn != 4 || FullIdWidth != 6 || DsPort != 3 || PayloadPort != 2 ||
        H != 64 || RemWidth != 7)
      $fatal(1, "M local gate requires frozen four-port ID6/H64 contract");
  end

  assign gate_active = pending_q && (remaining_q != '0) &&
                       enable_i && pair_qualified_i;
  assign p1_commit_o = pending_q && select_valid_i && select_ready_i &&
                       (select_id_i == PtwId);
  assign walk_retired = pending_q && !ptw_active_i;
  assign non_ds_commit = select_valid_i && select_ready_i &&
                         (select_id_i[FullIdWidth-1 -: 2] != 2'(DsPort));
  assign timeout_o = pending_q && (remaining_q == RemWidth'(1));
  assign exit_event = p1_commit_o || walk_retired || timeout_o ||
                      (pending_q && (remaining_q != '0) &&
                       (!enable_i || !pair_qualified_i));
  // Use old state only. A skipped occurrence is never queued for later replay.
  assign arm_o = !pending_q && !escape_q && enable_i && pair_qualified_i &&
                 start_i && !start_bank_i;

  always_comb begin
    allow_o = '1;
    if (gate_active && ((raw_id_i[PayloadPort] & AIdMask) == AIdPrefix))
      allow_o[PayloadPort] = 1'b0;
  end
  // This is a requested mask. An already locked winner remains eligible in
  // rr_arb_tree; use the selector trace to distinguish held from new work.
  assign masked_o = raw_valid_i[PayloadPort] && !allow_o[PayloadPort];

  always_comb begin
    pending_d = pending_q;
    remaining_d = remaining_q;
    escape_d = escape_q;
    if (pending_q && (remaining_q != '0)) begin
      remaining_d = remaining_q - 1'b1;
      if (!enable_i || !pair_qualified_i) remaining_d = '0;
    end
    // H expiry only ends eligibility suppression; the first-PTE association
    // survives until exact P1 acceptance or actual walker retirement.
    if (p1_commit_o || walk_retired) begin
      pending_d = 1'b0;
      remaining_d = '0;
    end
    if (exit_event) escape_d = 1'b1;
    // A non-DS acceptance on this same edge may satisfy escape. It cannot
    // retroactively qualify arm_o, which uses the old escape/pending bits.
    if (non_ds_commit) escape_d = 1'b0;
    if (arm_o) begin
      pending_d = 1'b1;
      remaining_d = RemWidth'(H);
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      pending_q <= 1'b0;
      remaining_q <= '0;
      escape_q <= 1'b0;
    end else begin
      pending_q <= pending_d;
      remaining_q <= remaining_d;
      escape_q <= escape_d;
    end
  end

  always_comb begin
    state_o = '0;
    state_o[0] = pending_q;
    state_o[1] = gate_active;
    state_o[2] = escape_q;
    state_o[9:3] = remaining_q;
    state_o[10] = enable_i && pair_qualified_i;
    state_o[11] = arm_o;
    state_o[12] = p1_commit_o;
    state_o[13] = timeout_o;
    state_o[14] = masked_o;
    state_o[15] = walk_retired;
  end
endmodule
