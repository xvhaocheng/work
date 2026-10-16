// Private research model only. No production RTL or memory scheduling changes.
// The DS branch follows ordinary-program.md:34-112, including its 8 control bits.
// DataType must be the ID-prepended AXI AR struct (with addr/id/len members).
module research_ar_selector #(
  parameter type DataType = logic
) (
  input  logic          clk_i,
  input  logic          rst_ni,
  input  logic [3:0]    req_i,
  output logic [3:0]    gnt_o,
  input  DataType [3:0] data_i,
  input  logic          gnt_i,
  output logic          req_o,
  output DataType       data_o,
  output logic [1:0]    idx_o
);
  // Read exactly once before reset release. There is no runtime policy port.
  integer mode;
  initial begin : p_mode
    if (!$value$plusargs("ar-ds-first=%d", mode)) mode = 0;
    if ((mode != 0) && (mode != 1))
      $fatal(1, "ar-ds-first must be 0 (original RR) or 1 (DS-first/F4)");
  end

  logic rr_active, ds_active;
  logic [3:0] rr_req_i, rr_gnt_o, ds_req_i, ds_gnt_o;
  logic rr_gnt_i, ds_gnt_i, rr_req_o, ds_req_o;
  logic [1:0] rr_idx_o, ds_idx_o;
  DataType rr_data_o, ds_data_o;

  assign rr_active = mode == 0;
  assign ds_active = mode == 1;
  assign rr_req_i = rr_active ? req_i : '0;
  assign ds_req_i = ds_active ? req_i : '0;
  assign rr_gnt_i = rr_active && gnt_i;
  assign ds_gnt_i = ds_active && gnt_i;

  // Retain the selected CVA6 common_cells implementation and all its defaults.
  rr_arb_tree #(
    .NumIn     ( 4        ),
    .DataType  ( DataType ),
    .AxiVldRdy ( 1'b1     ),
    .LockIn    ( 1'b1     )
  ) i_original_rr (
    .clk_i   ( clk_i      ),
    .rst_ni  ( rst_ni     ),
    .flush_i ( 1'b0       ),
    .rr_i    ( '0         ),
    .req_i   ( rr_req_i   ),
    .gnt_o   ( rr_gnt_o   ),
    .data_i  ( data_i     ),
    .gnt_i   ( rr_gnt_i   ),
    .req_o   ( rr_req_o   ),
    .data_o  ( rr_data_o  ),
    .idx_o   ( rr_idx_o   )
  );

  // Exact ordinary DS-first/F4 control state: 2+3+1+2 = 8 bits.
  logic [1:0] nd_rr_q, held_idx_q;
  logic [2:0] ds_streak_q;
  logic held_q;
  logic nd_pending, fresh_valid, ds_fire;
  logic [1:0] fresh_idx;

  function automatic logic [1:0] nd_pick(
    input logic [2:0] active,
    input logic [1:0] cursor
  );
    case (cursor)
      2'd1: nd_pick = active[1] ? 2'd1 :
                       (active[2] ? 2'd2 : 2'd0);
      2'd2: nd_pick = active[2] ? 2'd2 :
                       (active[0] ? 2'd0 : 2'd1);
      default: nd_pick = active[0] ? 2'd0 :
                         (active[1] ? 2'd1 : 2'd2);
    endcase
  endfunction

  always_comb begin
    nd_pending = |ds_req_i[2:0];
    fresh_valid = |ds_req_i;
    fresh_idx = 2'd0;
    if (ds_req_i[3] && !(ds_streak_q == 3'd4 && nd_pending))
      fresh_idx = 2'd3;
    else if (nd_pending)
      fresh_idx = nd_pick(ds_req_i[2:0], nd_rr_q);
    ds_idx_o = held_q ? held_idx_q : fresh_idx;
    ds_req_o = held_q || fresh_valid;
    ds_data_o = '0;
    ds_gnt_o = '0;
    if (ds_req_o) begin
      ds_data_o = data_i[ds_idx_o];
      ds_gnt_o[ds_idx_o] = ds_gnt_i;
    end
    ds_fire = ds_req_o && ds_gnt_i;
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin : p_ds_state
    if (!rst_ni) begin
      nd_rr_q <= 2'd0;
      ds_streak_q <= 3'd0;
      held_q <= 1'b0;
      held_idx_q <= 2'd0;
    end else begin
      held_q <= ds_req_o && !ds_gnt_i;
      if (!held_q && ds_req_o && !ds_gnt_i)
        held_idx_q <= ds_idx_o;
      if (ds_fire && ds_idx_o != 2'd3)
        nd_rr_q <= ds_idx_o == 2'd2 ? 2'd0 : ds_idx_o + 2'd1;
      if (!nd_pending)
        ds_streak_q <= 3'd0;
      else if (ds_fire) begin
        if (ds_idx_o != 2'd3)
          ds_streak_q <= 3'd0;
        else if (ds_streak_q != 3'd4)
          ds_streak_q <= ds_streak_q + 3'd1;
      end
    end
  end

  always_comb begin
    req_o = rr_active ? rr_req_o : ds_req_o;
    gnt_o = rr_active ? rr_gnt_o : ds_gnt_o;
    data_o = rr_active ? rr_data_o : ds_data_o;
    idx_o = rr_active ? rr_idx_o : ds_idx_o;
  end

  // Test-only monitors. They never feed the selection/handshake paths. These
  // copies, counters, logging, and reference arithmetic are not physical cost.
  // Direct procedural fatals remain effective when ASSERTSOFF is defined.
  // pragma translate_off
  longint unsigned mon_cycle_q;
  longint unsigned mon_sequence_q, mon_held_sequence_q;
  longint unsigned mon_first_offer_count_q, mon_accept_count_q;
  longint unsigned mon_dual_head_first_offer_count_q;
  longint unsigned mon_first_dual_head_cycle_q;
  longint unsigned mon_held_nonds_ds_seen_count_q;
  longint unsigned mon_held_nonds_late_ds_count_q;
  longint unsigned mon_source_accepts_q [4];
  logic mon_held_q, mon_held_saw_ds_q, mon_ds_present_at_offer_q;
  logic [1:0] mon_held_idx_q;
  DataType mon_held_data_q;
  integer mon_mode_q;
  integer unsigned mon_streak_q, mon_nd_cursor_q;

  // Independent arithmetic reference for the ordinary non-DS cyclic order.
  function automatic logic [1:0] monitor_nd_pick(
    input logic [2:0] active,
    input integer unsigned cursor
  );
    integer unsigned candidate;
    logic found;
    monitor_nd_pick = 2'd0;
    found = 1'b0;
    for (int unsigned offset = 0; offset < 3; offset++) begin
      candidate = cursor + offset;
      if (candidate >= 3) candidate = candidate - 3;
      if (!found && active[candidate]) begin
        monitor_nd_pick = 2'(candidate);
        found = 1'b1;
      end
    end
  endfunction

  task automatic emit_owner(
    input string phase,
    input longint unsigned owner_sequence
  );
    $display("AR-OWNER-JSON {\"cycle\":%0d,\"mode\":%0d,\"phase\":\"%s\",\"sequence\":%0d,\"source\":%0d,\"mask\":%0d,\"ready\":%0d,\"held\":%0d,\"addr\":\"0x%0h\",\"id\":%0d,\"len\":%0d,\"packet\":\"0x%0h\"}",
             mon_cycle_q + 64'd1, mode, phase, owner_sequence, idx_o, req_i,
             gnt_i, mon_held_q, data_o.addr, data_o.id, data_o.len, data_o);
  endtask

  always_ff @(posedge clk_i or negedge rst_ni) begin : p_monitor
    if (!rst_ni) begin
      mon_cycle_q <= 0;
      mon_sequence_q <= 0;
      mon_held_sequence_q <= 0;
      mon_first_offer_count_q <= 0;
      mon_accept_count_q <= 0;
      mon_dual_head_first_offer_count_q <= 0;
      mon_first_dual_head_cycle_q <= 0;
      mon_held_nonds_ds_seen_count_q <= 0;
      mon_held_nonds_late_ds_count_q <= 0;
      for (int unsigned src = 0; src < 4; src++)
        mon_source_accepts_q[src] <= 0;
      mon_held_q <= 1'b0;
      mon_held_saw_ds_q <= 1'b0;
      mon_ds_present_at_offer_q <= 1'b0;
      mon_held_idx_q <= 2'd0;
      mon_held_data_q <= '0;
      mon_mode_q <= mode;
      mon_streak_q <= 0;
      mon_nd_cursor_q <= 0;
    end else begin
      mon_cycle_q <= mon_cycle_q + 64'd1;
      if ((mode !== mon_mode_q) || ((mode != 0) && (mode != 1)))
        $fatal(1, "AR owner policy changed after reset");
      if ($isunknown({req_i, gnt_i, req_o, gnt_o}))
        $fatal(1, "AR owner has unknown handshake controls");
      if (!$onehot0(gnt_o))
        $fatal(1, "AR owner grants more than one input");
      if ((|gnt_o) && !gnt_i)
        $fatal(1, "AR owner grants an input while downstream is not ready");
      if (req_o !== (|req_i))
        $fatal(1, "AR owner valid is not supported by a current valid input");
      if (mon_first_offer_count_q !=
          mon_accept_count_q + (mon_held_q ? 64'd1 : 64'd0))
        $fatal(1, "AR owner first-offer/accept/held conservation failed");

      if (req_o) begin
        if ($isunknown(idx_o) || (req_i[idx_o] !== 1'b1))
          $fatal(1, "AR owner selected an invalid input");
        // RR's AxiVldRdy permits speculative idle READY. Only a valid output
        // constitutes an offered request or accept; idle READY is not a token.
        if (|(gnt_o & ~req_i))
          $fatal(1, "AR owner grants an invalid input during a valid offer");
        if (data_o !== data_i[idx_o])
          $fatal(1, "AR owner data does not belong to its selected input");
        if (data_o.id[$bits(data_o.id)-1 -: 2] !== idx_o)
          $fatal(1, "AR owner source index disagrees with prepended fullID");
        if (gnt_i && (gnt_o !== (4'b0001 << idx_o)))
          $fatal(1, "AR owner accepted output without its unique input grant");
      end

      if (mon_held_q) begin
        if ((req_o !== 1'b1) || (idx_o !== mon_held_idx_q) ||
            (data_o !== mon_held_data_q))
          $fatal(1, "AR owner changed a previously offered held full packet");
        if ((req_i[mon_held_idx_q] !== 1'b1) ||
            (data_i[mon_held_idx_q] !== mon_held_data_q))
          $fatal(1, "AR owner upstream withdrew or changed a held full packet");
        if (mon_held_idx_q != 2'd3 && req_i[3] && !mon_held_saw_ds_q) begin
          mon_held_nonds_ds_seen_count_q <= mon_held_nonds_ds_seen_count_q + 64'd1;
          if (!mon_ds_present_at_offer_q)
            mon_held_nonds_late_ds_count_q <= mon_held_nonds_late_ds_count_q + 64'd1;
          mon_held_saw_ds_q <= 1'b1;
        end
      end

      if (ds_active) begin
        if ((held_q !== mon_held_q) ||
            (32'(ds_streak_q) != mon_streak_q) ||
            (32'(nd_rr_q) != mon_nd_cursor_q))
          $fatal(1, "DS-first control state disagrees with accepted-history reference");
        if (req_o && !mon_held_q) begin
          if (req_i[3] && !((mon_streak_q == 4) && (|req_i[2:0]))) begin
            if (idx_o != 2'd3)
              $fatal(1, "DS-first new selection failed to select current DS");
          end else if (|req_i[2:0]) begin
            if (idx_o !== monitor_nd_pick(req_i[2:0], mon_nd_cursor_q))
              $fatal(1, "DS-first F4 escape or non-DS cyclic selection failed");
          end
        end
        // Reference streak counts selector/spill INPUT accepts, not cycles or R.
        if (!(|req_i[2:0]))
          mon_streak_q <= 0;
        else if (req_o && gnt_i) begin
          if (idx_o != 2'd3) mon_streak_q <= 0;
          else if (mon_streak_q < 4) mon_streak_q <= mon_streak_q + 1;
        end
        if (req_o && gnt_i && idx_o != 2'd3)
          mon_nd_cursor_q <= idx_o == 2'd2 ? 0 : 32'(idx_o) + 1;
      end

      if (req_o && !mon_held_q) begin
        mon_sequence_q <= mon_sequence_q + 64'd1;
        mon_first_offer_count_q <= mon_first_offer_count_q + 64'd1;
        emit_owner("firstoffer", mon_sequence_q + 64'd1);
        if (req_i[3] && (|req_i[2:0])) begin
          mon_dual_head_first_offer_count_q <= mon_dual_head_first_offer_count_q + 64'd1;
          if (mon_dual_head_first_offer_count_q == 0)
            mon_first_dual_head_cycle_q <= mon_cycle_q + 64'd1;
        end
      end
      if (req_o && gnt_i) begin
        mon_accept_count_q <= mon_accept_count_q + 64'd1;
        mon_source_accepts_q[idx_o] <= mon_source_accepts_q[idx_o] + 64'd1;
        emit_owner("accept", mon_held_q ? mon_held_sequence_q : mon_sequence_q + 64'd1);
      end

      mon_held_q <= req_o && !gnt_i;
      if (!mon_held_q && req_o && !gnt_i) begin
        mon_held_idx_q <= idx_o;
        mon_held_data_q <= data_o;
        mon_held_sequence_q <= mon_sequence_q + 64'd1;
        mon_held_saw_ds_q <= 1'b0;
        mon_ds_present_at_offer_q <= req_i[3];
      end
    end
  end

  final begin : p_summary
    $display("AR-OWNER-SUMMARY-JSON {\"cycle\":%0d,\"mode\":%0d,\"first_offer_count\":%0d,\"accept_count\":%0d,\"held\":%0d,\"dual_head_first_offer_count\":%0d,\"first_dual_head_cycle\":%0d,\"held_nonds_ds_seen_count\":%0d,\"held_nonds_late_ds_count\":%0d,\"source_accepts\":[%0d,%0d,%0d,%0d]}",
             mon_cycle_q, mode, mon_first_offer_count_q, mon_accept_count_q,
             mon_held_q, mon_dual_head_first_offer_count_q, mon_first_dual_head_cycle_q,
             mon_held_nonds_ds_seen_count_q, mon_held_nonds_late_ds_count_q,
             mon_source_accepts_q[0], mon_source_accepts_q[1],
             mon_source_accepts_q[2], mon_source_accepts_q[3]);
  end
  // pragma translate_on
endmodule
