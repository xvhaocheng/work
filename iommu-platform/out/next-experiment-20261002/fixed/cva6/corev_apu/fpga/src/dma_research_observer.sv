--- a/cva6/corev_apu/fpga/src/dma_research_observer.sv
+++ b/cva6/corev_apu/fpga/src/dma_research_observer.sv
@@
 // COST sessions run from p4m_arm to p4m_close; the (open, close] window
 //   excludes the opening edge and includes the closing edge.
+// COST_ERR[7] = response_error_i while active; COST_ERR[8] = aborted by ABORT;
+// COST_ERR[9] = supported_i low while active.
 //
 // Raw IOTLB entry (raw128, both words): gscid at bits [79:64], vpn[28:0] at 0,
@@
     logic [127:0] target_key_q [2];
     logic [1:0]  target_valid_q;
+    logic [127:0] target_ctx_q [2];
+    logic [1:0]  target_ctx_valid_q;
     logic [63:0] fp_time_q [2];
@@
     logic [63:0] sample_ram_q [SampleDepth*4];
+
+    // next-state (declared once; do not declare *_RAM_n because RAMs are FF-only)
+    logic [1:0]  mode_n;
+    logic [3:0]  cmd_n;
+    logic [2:0]  e2_phase_n;
+    logic [63:0] epoch_n, a_elapsed_n, astart_n, deadline_n, probestart_n, freeze_ts_n;
+    logic [15:0] event_count_n;
+    logic [31:0] inval_count_n, update_count_n, quiet_timeout_n;
+    logic        e2_ovf_n;
+    logic [15:0] e2_err_n;
+    logic [127:0] target_key_n [2];
+    logic [1:0]  target_valid_n;
+    logic [127:0] target_ctx_n [2];
+    logic [1:0]  target_ctx_valid_n;
+    logic [63:0] fp_time_n [2];
+    logic        fp_hit_n [2];
+    logic [15:0] fp_live_self_n [2];
+    logic [15:0] fp_live_oth_n [2];
+    logic [127:0] fp_key_n [2];
+    logic [1:0]  fp_done_n;
+    logic [2:0]  snapshot_valid_n;
+    logic [31:0] plru_snap_n [3];
+    logic [31:0] e2a_issue_cnt_n, e2a_done_cnt_n;
+    logic [63:0] e2a_first_n, e2a_last_n;
+    logic        e2a_first_seen_n;
+    logic [63:0] src_iova_n, dst_iova_n;
+    logic        src_set_n, dst_set_n;
+    logic [1:0]  lane_active_n;
+    logic [63:0] lane_id_n [2];
+    logic [1:0]  lane_in_window_n;
+    logic        cost_active_n;
+    logic [15:0] cost_err_n;
+    logic [CounterWidth-1:0] cost_cycles_n;
+    logic [CounterWidth-1:0] cost_main_n [2];
+    logic [CounterWidth-1:0] cost_comp_n [2];
+    logic [CounterWidth-1:0] cost_sel_n [2];
+    logic [CounterWidth-1:0] cost_hit_n [2];
+    logic [CounterWidth-1:0] cost_miss_n [2];
+    logic [CounterWidth-1:0] cost_issue_n [2];
+    logic [CounterWidth-1:0] cost_done_n [2];
+    logic [CounterWidth-1:0] cost_owner_err_n;
+    logic [CounterWidth-1:0] cost_a_done_all_n;
+    logic        window_open_n, window_closed_n;
+    logic [CounterWidth-1:0] win_cycles_n;
+    logic [CounterWidth-1:0] win_main_n [2];
+    logic [CounterWidth-1:0] win_comp_n [2];
+    logic [CounterWidth-1:0] win_sel_n [2];
+    logic [CounterWidth-1:0] win_hit_n [2];
+    logic [CounterWidth-1:0] win_miss_n [2];
+    logic        carry_in_n, carry_out_n;
+    logic [CounterWidth-1:0] eligible_n;
+    logic [15:0] sample_count_n;
+    logic        sample_ovf_n;
+    logic [31:0] cur_a_main_n, cur_a_req_n, cur_a_miss_n, cur_a_comp_n, cur_a_dur_n, cur_a_flags_n;
+    logic [31:0] cur_v_main_n, cur_v_comp_n;
+
+    function automatic logic ctx_match(input logic [127:0] a, input logic [127:0] b);
+        return (a[48:29] == b[48:29]) && (a[79:64] == b[79:64]) &&
+               (a[49] == b[49]) && (a[50] == b[50]);
+    endfunction
+
+    logic ctx_ok_both;
+    assign ctx_ok_both = (target_ctx_valid_q == 2'b11) &&
+                         ctx_match(target_ctx_q[0], target_ctx_q[1]);
@@
     assign acc_warm  = cmd_wr && !cmd_rsvd && (mode_q == MODE_E2) && (wdata_i[3:0] == 4'd1) &&
-                       (ph == E2_IDLE) && quiet_i && supported_i && iova_cfg_valid;
+                       (ph == E2_IDLE) && quiet_i && supported_i && iova_cfg_valid &&
+                       !update_i && !invalidate_i;
     assign acc_a     = cmd_wr && !cmd_rsvd && (mode_q == MODE_E2) && (wdata_i[3:0] == 4'd2) &&
                        (ph == E2_ARM_WARM) && quiet_i && supported_i &&
                        (target_valid_q == 2'b11) && tgt_present0 && tgt_present1 &&
-                       !update_i && !invalidate_i;
+                       !update_i && !invalidate_i && ctx_ok_both;
     assign acc_probe = cmd_wr && !cmd_rsvd && (mode_q == MODE_E2) && (wdata_i[3:0] == 4'd3) &&
                        (ph == E2_READY) && quiet_i && supported_i;
     assign acc_freeze= cmd_wr && !cmd_rsvd && (mode_q == MODE_E2) && (wdata_i[3:0] == 4'd4) &&
                        (ph == E2_PROBE) && quiet_i && supported_i &&
                        (fp_done_q == 2'b11) && !update_i && !invalidate_i;
-    assign acc_abort = cmd_wr && !cmd_rsvd && (mode_q == MODE_E2) && (wdata_i[3:0] == 4'd5) &&
-                       e2_active_phase;
-    assign acc_clear = cmd_wr && !cmd_rsvd && (mode_q == MODE_E2) && (wdata_i[3:0] == 4'd6) &&
-                       ((ph == E2_IDLE) || (ph == E2_FREEZE) || (ph == E2_ABORT)) && quiet_i;
+    logic abort_cmd;
+    assign abort_cmd = cmd_wr && !cmd_rsvd && (wdata_i[3:0] == 4'd5);
+    assign acc_clear = cmd_wr && !cmd_rsvd && (wdata_i[3:0] == 4'd6) && quiet_i && !cost_active_q &&
+                       ( (mode_q == MODE_NONE) ||
+                         (mode_q == MODE_COST) ||
+                         ((mode_q == MODE_E2) && ((ph == E2_IDLE) || (ph == E2_FREEZE) || (ph == E2_ABORT))) );
@@
     assign win_act  = cost_active_q && window_open_q && !window_closed_q;
+    logic win_act_issue;
+    assign win_act_issue = cost_active_q && (window_open_q || p4m_open_i) &&
+                           !window_closed_q && !p4m_close_i;
     assign main_cyc = (ctl_state_i == ST_REQ_TRANS) || (ctl_state_i == ST_AWAIT_RESP);
@@
         evt_meta[51]    = (changed_count == 5'd1) && (changed_mask != replace_mask_i);
-        evt_meta[52]    = target_valid_q[0] && id_match(entries_i[slot_idx], target_key_q[0]) &&
-                          !(id_match(entries_next_i[slot_idx], target_key_q[0]));
-        evt_meta[53]    = target_valid_q[1] && id_match(entries_i[slot_idx], target_key_q[1]) &&
-                          !(id_match(entries_next_i[slot_idx], target_key_q[1]));
+        evt_meta[52]    = target_valid_q[0] && entries_i[slot_idx][51] &&
+                          id_match(entries_i[slot_idx], target_key_q[0]) &&
+                          ( !entries_next_i[slot_idx][51] ||
+                            !id_match(entries_next_i[slot_idx], target_key_q[0]) );
+        evt_meta[53]    = target_valid_q[1] && entries_i[slot_idx][51] &&
+                          id_match(entries_i[slot_idx], target_key_q[1]) &&
+                          ( !entries_next_i[slot_idx][51] ||
+                            !id_match(entries_next_i[slot_idx], target_key_q[1]) );
         evt_meta[54]    = !slot_oh_valid;
@@
         target_valid_n   = target_valid_q;
+        target_ctx_valid_n = target_ctx_valid_q;
         fp_done_n        = fp_done_q;
@@
         for (int i = 0; i < 2; i++) begin
             target_key_n[i]   = target_key_q[i];
+            target_ctx_n[i]   = target_ctx_q[i];
             fp_time_n[i]      = fp_time_q[i];
@@
         // ------------------------------------------------- E2 epoch / deadline
         if ((ph == E2_ARM_WARM) || (ph == E2_ARM_A) || (ph == E2_READY) || (ph == E2_PROBE))
             epoch_n = epoch_q + 64'd1;
+        // E2 supported_i low is sticky in any active E2 phase.
+        if (e2_active_phase && !supported_i) e2_err_n[9] = 1'b1;
         if (ph == E2_ARM_A) a_elapsed_n = a_elapsed_q + 64'd1;
@@
             if (lookup_i) begin
                 if (!lookup_stage1_i || lookup_stage2_i) e2_err_n[5] = 1'b1;
                 if (ph == E2_ARM_A) begin
                     if (lookup_did_i != DID_A) e2_err_n[12] = 1'b1;
                 end else begin
                     if (lookup_did_i != DID_V) e2_err_n[12] = 1'b1;
+                    if (ph == E2_ARM_WARM &&
+                        !(page_match(lookup_iova_i, src_iova_q) ||
+                          page_match(lookup_iova_i, dst_iova_q)))
+                        e2_err_n[12] = 1'b1;
                     if ((ph == E2_PROBE) &&
                         !(page_match(lookup_iova_i, src_iova_q) ||
                           page_match(lookup_iova_i, dst_iova_q)))
                         e2_err_n[5] = 1'b1;
                 end
             end
@@
         // ------------------------------------------------- E2 target capture
-        if ((ph == E2_ARM_WARM) && lookup_i && lookup_stage1_i && !lookup_stage2_i &&
-            (lookup_did_i == DID_V)) begin
-            if (!target_valid_q[0] && page_match(lookup_iova_i, src_iova_q)) begin
-                if (cap_found) begin
-                    target_key_n[0]   = cap_key;
-                    target_valid_n[0] = 1'b1;
-                end else if (cap_bad_geom) begin
-                    e2_err_n[9] = 1'b1;
-                end
-            end else if (!target_valid_q[1] && page_match(lookup_iova_i, dst_iova_q)) begin
-                if (cap_found) begin
-                    target_key_n[1]   = cap_key;
-                    target_valid_n[1] = 1'b1;
-                end else if (cap_bad_geom) begin
-                    e2_err_n[9] = 1'b1;
-                end
-            end
-        end
+        if ((ph == E2_ARM_WARM) && lookup_i && lookup_stage1_i && !lookup_stage2_i &&
+            (lookup_did_i == DID_V)) begin
+            if (page_match(lookup_iova_i, src_iova_q)) begin
+                if (!target_ctx_valid_q[0]) begin
+                    target_ctx_n[0]       = ctx_key_128();
+                    target_ctx_valid_n[0] = 1'b1;
+                end else if (!ctx_match(ctx_key_128(), target_ctx_q[0])) begin
+                    e2_err_n[12] = 1'b1;
+                end
+                if (!target_valid_q[0]) begin
+                    if (cap_found) begin
+                        target_key_n[0]   = cap_key;
+                        target_valid_n[0] = 1'b1;
+                    end else if (cap_bad_geom) begin
+                        e2_err_n[9] = 1'b1;
+                    end
+                end
+            end else if (page_match(lookup_iova_i, dst_iova_q)) begin
+                if (!target_ctx_valid_q[1]) begin
+                    target_ctx_n[1]       = ctx_key_128();
+                    target_ctx_valid_n[1] = 1'b1;
+                end else if (!ctx_match(ctx_key_128(), target_ctx_q[1])) begin
+                    e2_err_n[12] = 1'b1;
+                end
+                if (!target_valid_q[1]) begin
+                    if (cap_found) begin
+                        target_key_n[1]   = cap_key;
+                        target_valid_n[1] = 1'b1;
+                    end else if (cap_bad_geom) begin
+                        e2_err_n[9] = 1'b1;
+                    end
+                end
+            end else begin
+                e2_err_n[12] = 1'b1;
+            end
+        end
@@
                 13'h0005: begin
                     cmd_n = wdata_i[3:0];
-                    if (cmd_rsvd || (mode_q != MODE_E2) || (wdata_i[3:0] == 4'd0) ||
-                        (wdata_i[3:0] > 4'd6)) begin
-                        e2_err_n[0] = 1'b1;
-                    end else if (!(acc_warm || acc_a || acc_probe || acc_freeze ||
-                                   acc_abort || acc_clear)) begin
-                        e2_err_n[0] = 1'b1;
-                        if (wdata_i[3:0] == 4'd2) begin
-                            if (!((target_valid_q == 2'b11) && tgt_present0 && tgt_present1))
-                                e2_err_n[6] = 1'b1;
-                            else if (update_i || invalidate_i)
-                                e2_err_n[7] = 1'b1;
-                        end
-                        if (wdata_i[3:0] == 4'd4) begin
-                            if (fp_done_q != 2'b11)                 e2_err_n[0] = 1'b1;
-                            else if (update_i || invalidate_i)      e2_err_n[7] = 1'b1;
-                        end
-                        if (!supported_i) e2_err_n[9] = 1'b1;
-                    end
+                    if (cmd_rsvd || (wdata_i[3:0] == 4'd0) || (wdata_i[3:0] > 4'd6)) begin
+                        e2_err_n[0] = 1'b1;
+                    end else begin
+                        unique case (wdata_i[3:0])
+                            4'd1: begin
+                                if (!acc_warm) begin
+                                    e2_err_n[0] = 1'b1;
+                                    if (update_i || invalidate_i) e2_err_n[7] = 1'b1;
+                                end
+                            end
+                            4'd2: begin
+                                if (!acc_a) begin
+                                    e2_err_n[0] = 1'b1;
+                                    if (!((target_valid_q == 2'b11) && tgt_present0 && tgt_present1))
+                                        e2_err_n[6] = 1'b1;
+                                    else if (update_i || invalidate_i)
+                                        e2_err_n[7] = 1'b1;
+                                    else if (!ctx_ok_both)
+                                        e2_err_n[12] = 1'b1;
+                                end
+                            end
+                            4'd3: begin
+                                if (!acc_probe) e2_err_n[0] = 1'b1;
+                            end
+                            4'd4: begin
+                                if (!acc_freeze) begin
+                                    e2_err_n[0] = 1'b1;
+                                    if (fp_done_q != 2'b11) e2_err_n[0] = 1'b1;
+                                    else if (update_i || invalidate_i) e2_err_n[7] = 1'b1;
+                                end
+                            end
+                            4'd5: begin
+                                // ABORT is a cleanup request; effect below decides no-op.
+                            end
+                            4'd6: begin
+                                if (!acc_clear) e2_err_n[0] = 1'b1;
+                            end
+                        endcase
+                    end
+                    if (!supported_i) e2_err_n[9] = 1'b1;
                 end
@@
             target_valid_n   = 2'b00;
+            target_ctx_valid_n = 2'b00;
             fp_done_n        = 2'b00;
@@
-        if (acc_abort) e2_phase_n = E2_ABORT;
+        if (abort_cmd) begin
+            if ((mode_q == MODE_E2) && e2_active_phase) begin
+                e2_phase_n = E2_ABORT;
+            end else if ((mode_q == MODE_COST) && cost_active_q) begin
+                cost_active_n = 1'b0;
+                cost_err_n[8] = 1'b1;
+            end
+        end
         if (acc_clear) begin
             e2_phase_n      = E2_IDLE;
             e2_err_n        = 16'd0;
+            cost_err_n      = 16'd0;
             e2_ovf_n        = 1'b0;
             quiet_timeout_n = 32'd0;
+            src_set_n       = 1'b0;
+            dst_set_n       = 1'b0;
         end
@@
         // ------------------------------------------- E2 first-probe records
-        if ((ph == E2_PROBE) && lookup_i && lookup_stage1_i && !lookup_stage2_i &&
-            (lookup_did_i == DID_V)) begin
-            if (target_valid_q[0] && !fp_done_q[0] && page_match(lookup_iova_i, src_iova_q)) begin
-                fp_time_n[0]      = epoch_q;
-                fp_hit_n[0]       = lookup_hit_i;
-                fp_live_self_n[0] = tgt_live0;
-                fp_live_oth_n[0]  = tgt_live1;
-                fp_key_n[0]       = cap_found ? cap_key : ctx_key_128();
-                fp_done_n[0]      = 1'b1;
-            end
-            if (target_valid_q[1] && !fp_done_q[1] && page_match(lookup_iova_i, dst_iova_q)) begin
-                fp_time_n[1]      = epoch_q;
-                fp_hit_n[1]       = lookup_hit_i;
-                fp_live_self_n[1] = tgt_live1;
-                fp_live_oth_n[1]  = tgt_live0;
-                fp_key_n[1]       = cap_found ? cap_key : ctx_key_128();
-                fp_done_n[1]      = 1'b1;
-            end
-        end
+        if ((ph == E2_PROBE) && lookup_i && lookup_stage1_i && !lookup_stage2_i &&
+            (lookup_did_i == DID_V)) begin
+            if (page_match(lookup_iova_i, src_iova_q)) begin
+                if (!target_ctx_valid_q[0] || !ctx_match(ctx_key_128(), target_ctx_q[0]))
+                    e2_err_n[12] = 1'b1;
+                if (lookup_hit_i != cap_found) e2_err_n[5] = 1'b1;
+                if (cap_bad_geom) e2_err_n[9] = 1'b1;
+                if (target_ctx_valid_q[0] && !fp_done_q[0]) begin
+                    fp_time_n[0]      = epoch_q;
+                    fp_hit_n[0]       = lookup_hit_i;
+                    fp_live_self_n[0] = tgt_live0;
+                    fp_live_oth_n[0]  = tgt_live1;
+                    fp_key_n[0]       = cap_found ? cap_key : ctx_key_128();
+                    fp_done_n[0]      = 1'b1;
+                end
+            end else if (page_match(lookup_iova_i, dst_iova_q)) begin
+                if (!target_ctx_valid_q[1] || !ctx_match(ctx_key_128(), target_ctx_q[1]))
+                    e2_err_n[12] = 1'b1;
+                if (lookup_hit_i != cap_found) e2_err_n[5] = 1'b1;
+                if (cap_bad_geom) e2_err_n[9] = 1'b1;
+                if (target_ctx_valid_q[1] && !fp_done_q[1]) begin
+                    fp_time_n[1]      = epoch_q;
+                    fp_hit_n[1]       = lookup_hit_i;
+                    fp_live_self_n[1] = tgt_live1;
+                    fp_live_oth_n[1]  = tgt_live0;
+                    fp_key_n[1]       = cap_found ? cap_key : ctx_key_128();
+                    fp_done_n[1]      = 1'b1;
+                end
+            end else begin
+                e2_err_n[5] = 1'b1;
+            end
+        end
@@
         if (cost_active_q) begin
+            if (!supported_i)   cost_err_n[9] = 1'b1;
+            if (response_error_i) cost_err_n[7] = 1'b1;
             if (&cost_cycles_q) cost_err_n[2] = 1'b1;
@@
             if (lane_active_q[1]) begin
+                if (&cur_a_dur_q) cost_err_n[2] = 1'b1;
                 cur_a_dur_n = cur_a_dur_q + 32'd1;
-                if (main_cyc && (ctl_did_i == DID_A))                       cur_a_main_n = cur_a_main_q + 32'd1;
-                if (select_fire_i && (select_did_i == DID_A))               cur_a_req_n  = cur_a_req_q  + 32'd1;
-                if (lookup_i && (lookup_did_i == DID_A) && !lookup_hit_i)   cur_a_miss_n = cur_a_miss_q + 32'd1;
-                if (comp_cyc && (ctl_did_i == DID_A))                       cur_a_comp_n = cur_a_comp_q + 32'd1;
+                if (main_cyc && (ctl_did_i == DID_A)) begin
+                    if (&cur_a_main_q) cost_err_n[2] = 1'b1;
+                    cur_a_main_n = cur_a_main_q + 32'd1;
+                end
+                if (select_fire_i && (select_did_i == DID_A)) begin
+                    if (&cur_a_req_q) cost_err_n[2] = 1'b1;
+                    cur_a_req_n  = cur_a_req_q  + 32'd1;
+                end
+                if (lookup_i && (lookup_did_i == DID_A) && !lookup_hit_i) begin
+                    if (&cur_a_miss_q) cost_err_n[2] = 1'b1;
+                    cur_a_miss_n = cur_a_miss_q + 32'd1;
+                end
+                if (comp_cyc && (ctl_did_i == DID_A)) begin
+                    if (&cur_a_comp_q) cost_err_n[2] = 1'b1;
+                    cur_a_comp_n = cur_a_comp_q + 32'd1;
+                end
             end
             if (lane_active_q[0]) begin
-                if (main_cyc && (ctl_did_i == DID_V))                       cur_v_main_n = cur_v_main_q + 32'd1;
-                if (comp_cyc && (ctl_did_i == DID_V))                       cur_v_comp_n = cur_v_comp_q + 32'd1;
+                if (main_cyc && (ctl_did_i == DID_V)) begin
+                    if (&cur_v_main_q) cost_err_n[2] = 1'b1;
+                    cur_v_main_n = cur_v_main_q + 32'd1;
+                end
+                if (comp_cyc && (ctl_did_i == DID_V)) begin
+                    if (&cur_v_comp_q) cost_err_n[2] = 1'b1;
+                    cur_v_comp_n = cur_v_comp_q + 32'd1;
+                end
             end
@@
                 cur_a_dur_n   = 32'd0;
                 cur_a_flags_n = 32'd0;
-                cur_a_flags_n[0] = !window_open_q;
-                cur_a_flags_n[1] = window_closed_q;
-                cur_a_flags_n[2] = win_act && !p4m_close_i;
-                lane_in_window_n[1] = win_act && !p4m_close_i;
+                cur_a_flags_n[0] = !(window_open_q || p4m_open_i);
+                cur_a_flags_n[1] = window_closed_q;
+                cur_a_flags_n[2] = win_act_issue;
+                lane_in_window_n[1] = win_act_issue;
             end
@@
         // p4m_arm dominates: reset and start (requires quiet and MODE_COST)
         if (p4m_arm_i) begin
-            if (mode_q != MODE_COST) begin
-                cost_err_n[6] = 1'b1;
-            end else if (!quiet_i) begin
-                cost_err_n[5] = 1'b1;
-            end else begin
+            if (mode_q == MODE_COST && !quiet_i) begin
+                cost_err_n[5] = 1'b1;
+            end else if (mode_q == MODE_COST && (cost_active_q || (e2_phase_q != E2_IDLE))) begin
+                cost_err_n[6] = 1'b1;
+            end else if (mode_q == MODE_COST) begin
                 cost_active_n        = 1'b1;
@@
             target_valid_q   <= 2'b00;
+            target_ctx_valid_q <= 2'b00;
             fp_done_q        <= 2'b00;
@@
             for (int i = 0; i < 2; i++) begin
                 target_key_q[i]   <= '0;
+                target_ctx_q[i]   <= '0;
                 fp_time_q[i]      <= 64'd0;
@@
             target_valid_q   <= target_valid_n;
+            target_ctx_valid_q <= target_ctx_valid_n;
             fp_done_q        <= fp_done_n;
@@
             for (int i = 0; i < 2; i++) begin
                 target_key_q[i]   <= target_key_n[i];
+                target_ctx_q[i]   <= target_ctx_n[i];
                 fp_time_q[i]      <= fp_time_n[i];
