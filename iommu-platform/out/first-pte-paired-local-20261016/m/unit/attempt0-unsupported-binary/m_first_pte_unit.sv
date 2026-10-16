// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Finite protocol tests of the real local gate + actual modified RR tree.
// Inputs are producer/consumer boundary stimuli; no hierarchical DUT writes.
module m_first_pte_unit;
  timeunit 1ns;
  timeprecision 1ps;
  logic clk = 0;
  always #5 clk = ~clk;
  logic rst_n = 0;
  logic enable = 0, pair_qualified = 0, start = 0, active = 0, bank = 0;
  logic [3:0][5:0] ids;
  logic [3:0] valid = 0, allow, grant;
  logic ready = 0, selected_valid;
  logic [5:0] selected_id;
  logic [1:0] selected_index;
  logic [15:0] state;
  logic arm, p1_commit, timeout_event, masked;
  logic [2:0] quota;
  integer cycle = 0, checks = 0, scenarios = 0;
  integer arm_count = 0, p1_count = 0, timeout_count = 0;
  integer accepted_count = 0, ds_count = 0, non_ds_count = 0;
  logic held_last = 0;
  logic [5:0] held_id;
  logic [1:0] held_index;
  string phase = "reset";

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

  task automatic check(input bit condition, input string message_text);
    checks++;
    if (!condition) begin
      $display("FAIL cycle=%0d phase=%s check=%s", cycle, phase, message_text);
      $fatal(1, "Finite protocol assertion failed");
    end
  endtask

  // Common protocol checks execute on every active clock edge, before updates.
  // Unlike the old non-VERILATOR SVA in the tree, these run in this simulator.
  always @(posedge clk) begin
    cycle++;
    if (!rst_n) begin
      held_last = 0;
    end else begin
      check($onehot0(grant), "at most one upstream handshake grant");
      check((grant & ~valid) == 0, "no READY grant to an invalid producer");
      check((selected_valid && ready) == (|(grant & valid)),
            "selector and producer handshakes conserve requests");
      check(quota <= 3'd4, "common DS quota saturates at four");
      check(allow[3] && allow[1] && allow[0], "M never masks another source");
      if (held_last) begin
        check(selected_valid, "held VALID cannot be withdrawn");
        check(selected_id == held_id && selected_index == held_index,
              "held payload/index cannot be changed by M or DS/F4");
      end
      if (selected_valid && ready) begin
        check(valid[selected_index] && selected_id == ids[selected_index],
              "accepted payload retains its real source identity");
        accepted_count++;
        if (selected_index == 2'd3) ds_count++;
        else non_ds_count++;
      end
      if (arm) arm_count++;
      if (p1_commit) p1_count++;
      if (timeout_event) timeout_count++;
      $display("TRACE cycle=%0d phase=%s start=%0b active=%0b pending=%0b remain=%0d escape=%0b gate=%0b valid=%h allow=%h grant=%h out_valid=%0b ready=%0b out_id=%0d quota=%0d arm=%0b p1=%0b timeout=%0b",
        cycle, phase, start, active, state[0], state[9:3], state[2],
        state[1], valid, allow, grant, selected_valid, ready, selected_id,
        quota, arm, p1_commit, timeout_event);
      held_last = selected_valid && !ready;
      held_id = selected_id;
      held_index = selected_index;
    end
  end

  task automatic step;
    @(posedge clk);
    #1;
  endtask

  task automatic reset_case(input string name);
    @(negedge clk);
    rst_n = 0;
    enable = 1;
    pair_qualified = 1;
    start = 0;
    active = 0;
    bank = 0;
    valid = 0;
    ready = 1;
    ids[0] = 6'd0;
    ids[1] = 6'd16;
    ids[2] = 6'd36;
    ids[3] = 6'd48;
    phase = name;
    repeat (2) step();
    @(negedge clk);
    rst_n = 1;
    #1;
    check(state[9:0] == 0 && quota == 0, "common reset clears only test episode state");
    scenarios++;
  endtask

  // Mirrors PTW IDLE/start -> MEM_ACCESS active transition at the same edge.
  // Actual PTW/TL event generation is exercised separately in root integration.
  task automatic fresh_walk;
    @(negedge clk);
    start = 1;
    active = 0;
    bank = 0;
    #1;
    check(arm && !state[0], "fresh qualified occurrence arms without source queue");
    step();
    check(state[0] && state[1] && state[9:3] == 7'd64,
          "f1 receiver captures at that same occurrence edge");
    // These boundary signals model the PTW's state update after the edge.
    start = 0;
    active = 1;
    #1;
    check(!state[15], "new active walk cannot retire its just-created record");
  endtask

  integer before_count, before_timeout;
  initial begin
    // 1. Ineligible occurrences are skipped, with no deferred replay.
    reset_case("qualification");
    @(negedge clk);
    start = 1;
    bank = 1;
    #1;
    check(!arm, "non-V bank occurrence is ineligible");
    step();
    check(!state[0], "non-V start stores no pending work");
    @(negedge clk);
    start = 0;
    bank = 0;
    step();
    @(negedge clk);
    pair_qualified = 0;
    start = 1;
    #1;
    check(!arm, "unqualified pair occurrence is skipped");
    step();
    check(!state[0], "unqualified start stores no pending work");
    @(negedge clk);
    pair_qualified = 1;
    start = 0;
    step();
    check(!state[0], "later qualification does not replay a skipped start");
    @(negedge clk);
    enable = 0;
    start = 1;
    #1;
    check(!arm, "disabled occurrence is skipped");
    step();
    check(!state[0], "disabled start stores no pending work");

    // 2. Fresh f1 + exact P1 single consumption + escape/no replay.
    reset_case("commit_escape");
    fresh_walk();
    @(negedge clk);
    valid[3] = 1;
    before_count = p1_count;
    #1;
    check(selected_valid && selected_id == 6'd48 && p1_commit,
          "full-ID48 selector acceptance consumes exact P1");
    step();
    check(!state[0] && !state[1] && state[2], "P1 clears pending and starts escape");
    check(p1_count == before_count + 1, "P1 counted once on its acceptance edge");
    #1;
    check(!p1_commit, "same full ID without pending cannot consume a second P1");
    step();
    check(p1_count == before_count + 1, "later same-ID acceptance is not another consume");
    @(negedge clk);
    valid = 0;
    start = 1;
    active = 0;
    #1;
    check(!arm, "new occurrence is skipped while escape is outstanding");
    step();
    check(!state[0] && state[2], "escape skip is not queued");
    @(negedge clk);
    start = 0;
    valid[2] = 1;
    #1;
    check(grant[2] && selected_id == 6'd36, "ordinary A acceptance satisfies escape");
    step();
    check(!state[2] && !state[0], "escape clears without resurrecting skipped occurrence");
    @(negedge clk);
    valid = 0;
    repeat (3) step();
    check(!state[0], "no implicit replay after later idle cycles");
    fresh_walk();

    // 3. Exactly H64 suppression; expiry retains identity; wrong ID cannot consume.
    reset_case("timeout_identity");
    fresh_walk();
    before_timeout = timeout_count;
    for (int n = 1; n <= 64; n++) begin
      @(negedge clk);
      valid[2] = 1;
      start = (n == 7);  // another occurrence while pending must not renew/replay
      #1;
      check(!arm, "pending occurrence cannot renew the timer");
      check(state[0] && state[1] && state[9:3] == 7'(65-n),
            "H decrements in real clock cycles without renewal");
      check(masked && !allow[2] && !grant[2] && !selected_valid,
            "new masked A gets no false READY/VALID handshake");
      check(timeout_event == (n == 64), "timeout is the last fixed lease edge");
      step();
      check(state[0] && state[9:3] == 7'(64-n), "timeout never clears P1 identity");
      check(state[1] == (n != 64), "suppression ends exactly after H64 edges");
    end
    check(timeout_count == before_timeout + 1, "one timeout event, no renewal");
    check(state[2] && allow[2] && grant[2], "expired gate permits ordinary A service");
    @(negedge clk);
    start = 0;
    step(); // accept A after expiry, keeping the pending association
    check(state[0] && !state[1] && !state[2], "non-DS escape does not clear pending identity");
    @(negedge clk);
    valid[2] = 0;
    valid[3] = 1;
    ids[3] = 6'd49;
    #1;
    check(!p1_commit, "another DS identity cannot stand in for PTW48");
    step();
    check(state[0] && state[9:3] == 0, "wrong-ID acceptance leaves passive pending");
    @(negedge clk);
    ids[3] = 6'd48;
    #1;
    check(p1_commit, "late exact P1 still consumes the retained association");
    step();
    check(!state[0] && state[2], "late P1 retires association exactly once");

    // 4. True active drop closes an unconsumed association; simultaneous new start skips.
    reset_case("true_retire");
    fresh_walk();
    repeat (2) step();
    @(negedge clk);
    active = 0;
    start = 1;
    #1;
    check(state[15] && !arm, "true retired old walk is recognized before any new arm");
    step();
    check(!state[0] && state[9:3] == 0 && state[2],
          "true retirement clears pending/timer and requires escape");
    @(negedge clk);
    start = 0;
    valid[0] = 1;
    step();
    check(!state[0] && !state[2], "retire-time skipped occurrence is not replayed");

    // 5. Existing held A survives late intent and arriving high-priority DS.
    reset_case("held_A_late_intent");
    @(negedge clk);
    ready = 0;
    valid[2] = 1;
    #1;
    check(selected_valid && selected_id == 6'd36, "A first-offer precedes intent");
    step(); // original LockIn owns A now
    fresh_walk();
    check(masked && selected_valid && selected_id == 6'd36,
          "late mask request cannot withdraw already-held A");
    @(negedge clk);
    valid[3] = 1;
    repeat (3) begin
      #1;
      check(selected_valid && selected_id == 6'd36 && grant == 0,
            "DS priority and M retain stalled held A unchanged");
      step();
      @(negedge clk);
    end
    ready = 1;
    #1;
    check(grant[2] && !grant[3] && selected_id == 6'd36,
          "held A accepts before a later high-priority DS");
    step();
    check(state[0], "held non-DS acceptance does not consume P1");
    @(negedge clk);
    valid[2] = 0;
    #1;
    check(grant[3] && !grant[2] && selected_id == 6'd48 && p1_commit,
          "P1 is served after held A completes, without a fake A handshake");
    step();
    check(!state[0], "P1 closes the late-intent record");

    // 6. Shared current-VALID DS-first/F4 counts accepts, then serves non-DS.
    reset_case("common_F4");
    @(negedge clk);
    enable = 0;
    valid = 4'b1001;
    ids[3] = 6'd49;
    for (int n = 1; n <= 4; n++) begin
      #1;
      check(selected_valid && selected_index == 2'd3 && grant == 4'b1000,
            "current valid DS wins first four accepts");
      check(quota == 3'(n-1), "quota counts previous accepted DS only");
      step();
      check(quota == 3'(n), "one accepted DS increments quota exactly once");
      if (n != 4) @(negedge clk);
    end
    check(selected_index == 0 && selected_id == 0 && grant == 4'b0001,
          "eligible non-DS selected immediately after four DS accepts");
    @(negedge clk);
    ready = 0;
    repeat (3) begin
      step();
      check(quota == 4 && selected_id == 0 && selected_valid,
            "stalled non-DS retains selection and does not spend quota");
      @(negedge clk);
    end
    ready = 1;
    #1;
    check(grant == 4'b0001, "F4 non-DS handshake is not preempted by waiting DS");
    step();
    check(quota == 0, "accepted non-DS resets consecutive-DS quota");
    check(selected_index == 3 && grant == 4'b1000, "DS resumes after required non-DS service");

    // 7. If no non-DS exists, DS continues at quota4. A held DS still cannot be preempted.
    reset_case("F4_held_DS");
    @(negedge clk);
    enable = 0;
    valid = 4'b1000;
    ids[3] = 6'd49;
    repeat (5) step();
    check(quota == 4 && selected_index == 3, "saturated quota never blocks lone DS");
    @(negedge clk);
    ready = 0;
    step(); // DS becomes held with no non-DS present
    @(negedge clk);
    valid[0] = 1;
    #1;
    check(selected_index == 3 && selected_valid && grant == 0,
          "new non-DS at quota4 cannot steal already-held DS");
    step();
    @(negedge clk);
    ready = 1;
    #1;
    check(grant == 4'b1000, "held DS obligation completes despite saturated quota");
    step();
    check(quota == 4 && selected_index == 0 && grant == 4'b0001,
          "new arbitration serves non-DS after held DS completion");
    step();
    check(quota == 0, "post-held escape non-DS resets quota");

    // 8. Soft disable releases eligibility immediately but preserves old identity.
    reset_case("soft_disable");
    fresh_walk();
    @(negedge clk);
    valid[2] = 1;
    enable = 0;
    #1;
    check(state[0] && !state[1] && allow[2] && grant[2],
          "soft disable immediately permits A without deleting association");
    step();
    check(state[0] && state[9:3] == 0, "soft disable preserves passive pending");
    @(negedge clk);
    valid[2] = 0;
    valid[3] = 1;
    #1;
    check(p1_commit, "disabled record still consumes its eventual exact P1");
    step();
    check(!state[0], "disabled record closes normally");

    $display("PASS scenarios=%0d checks=%0d cycles=%0d arms=%0d p1_consumes=%0d timeouts=%0d accepted=%0d ds=%0d non_ds=%0d",
      scenarios, checks, cycle, arm_count, p1_count, timeout_count,
      accepted_count, ds_count, non_ds_count);
    $finish;
  end

  initial begin
    #20000;
    $fatal(1, "Finite unit test exceeded fixed simulation-time watchdog");
  end
endmodule
