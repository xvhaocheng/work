from pathlib import Path
p=Path('before-routefix/native/mig_read_debt_observer.sv');s=p.read_text()
s=s.replace('logic [63:0] t_ar, t_last, held;','logic [63:0] t_ar;')
s=s.replace('  logic event_we;','''  (* ram_style = "block" *) logic [127:0] completion_mem [LiveSlots];
  logic completion_we, rd_completion_q, completion_valid_q;
  logic [127:0] completion_wdata, completion_rdata_q;
  logic r_id_match;
  logic event_we;''')
s=s.replace('    event_we = 0;','''    completion_we = 0;
    completion_wdata = '0;
    r_id_match = 0;
    event_we = 0;''')
s=s.replace("      if (q.occupancy != 0 && live_q[q.head].valid && live_q[q.head].id == rid_i)\n        rslot = int'(q.head);",'''      // Error checking does not gate the large accounting fanout. Every
      // malformed return is still sticky-invalid. Valid FIFO traces agree.
      if (q.occupancy != 0) rslot = int'(q.head);
      r_id_match = rslot >= 0 && live_q[q.head].valid && live_q[q.head].id == rid_i;''')
s=s.replace('        if (rslot < 0) n.errors[2] = 1;','        if (!r_id_match) n.errors[2] = 1;')
# Advance by actual RLAST; exact remaining/LAST invariant is checked separately.
s=s.replace('if (live_q[q.head].remaining == 1) begin','if (rlast_i) begin')
s=s.replace('(r_fire && rslot >= 0 && live_q[q.head].remaining == 1)', '(r_fire && rslot >= 0 && rlast_i)')
s=s.replace('                pred_n[i].held = live_n[i].held;\n','')
s=s.replace('                    pred_n[i].t_last = q.tick;\n','')
s=s.replace("            pred_n[i].t_last = !live_n[i].valid ? q.tick : 0;\n",'')
s=s.replace('            pred_n[i].held = live_n[i].held;\n','')
needle='''          if (q.selected && !q.episode_done) begin
            for'''
assert s.count(needle)==1
s=s.replace(needle,'''          // One FIFO head can retire per UI edge. Record its final time and
          // lifetime held count in RAM, including a same-P-edge predecessor.
          if (r_fire && rlast_i && (select_now ||
              (q.selected && !q.episode_done && pred_q[q.head].valid &&
               pred_q[q.head].ordinal == q.head_ordinal))) begin
            completion_we = 1;
            completion_wdata = {live_q[q.head].held, q.tick};
          end
          if (q.selected && !q.episode_done) begin
            for''')
s=s.replace('event_wdata[75] = rslot >= 0;', 'event_wdata[75] = r_id_match;')
s=s.replace('              4: pred_word[i] = pred_q[i].t_last;\n','')
s=s.replace('              5: pred_word[i] = pred_q[i].held;\n','')
s=s.replace('''    if (event_we) event_mem[event_waddr] <= event_wdata;''','''    if (completion_we) completion_mem[q.head] <= completion_wdata;
    if (rd_req_i && int'(rd_index_i) >= 32 && int'(rd_index_i) < 32+LiveSlots*8)
      completion_rdata_q <= completion_mem[SlotWidth'(pred_read_slot)];
    if (event_we) event_mem[event_waddr] <= event_wdata;''')
s=s.replace('      rd_event_q <= 0;','      rd_completion_q <= 0;\n      completion_valid_q <= 0;\n      rd_event_q <= 0;')
s=s.replace('''        else if (rd_pred_q) rd_data_o <= pred_group_q[rd_group_q];''','''        else if (rd_pred_q && rd_completion_q)
          rd_data_o <= completion_valid_q ? (rd_half_q ? completion_rdata_q[127:64] : completion_rdata_q[63:0]) : '0;
        else if (rd_pred_q) rd_data_o <= pred_group_q[rd_group_q];''')
s=s.replace('''        static_data_q <= static_data;''','''        static_data_q <= static_data;
        rd_completion_q <= rd_index_i[2:0] == 4 || rd_index_i[2:0] == 5;
        completion_valid_q <= pred_q[SlotWidth'(pred_read_slot)].valid && pred_q[SlotWidth'(pred_read_slot)].completed;''')
Path('native/routefix/mig_read_debt_observer.sv').write_text(s)
