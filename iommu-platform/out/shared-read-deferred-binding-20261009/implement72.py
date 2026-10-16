from pathlib import Path
D=Path(__file__).resolve().parent
p=D/'shared_pretl_read.sv';s=p.read_text().replace('RepairChoiceRule = 0, Q = 2,','RepairChoiceRule = 0, DeferredRepair = 0, Q = 2,\n    parameter int unsigned RepairWaitBudget = 64,')
s=s.replace('logic [31:0] repair_memory_cost, repair_join_cost;', 'logic [31:0] repair_memory_cost, repair_join_cost;\n    logic [15:0] repair_wait_q;\n    logic repair_join_ready;')
s=s.replace('assign repair_shared_active = AdaptiveRepair ? repair_shared_q : ShareRepairSuffix;', '''assign repair_shared_active = (AdaptiveRepair || DeferredRepair) ? repair_shared_q : ShareRepairSuffix;
    assign repair_join_ready = !skid_q && !old_v_busy_q && !busy_q && !offered_q &&
        returned_q[0] == old_end_q && returned_q[1] == old_end_q &&
        raw_written_i[0] == old_end_q && raw_written_i[1] == old_end_q;''')
s=s.replace('repair_shared_q <= ShareRepairSuffix; repair_decided_q <= 0;', 'repair_shared_q <= DeferredRepair ? 1 : ShareRepairSuffix; repair_decided_q <= 0; repair_wait_q <= 0;')
s=s.replace('repair_decided_q <= 1;\n                repair_decision_cursor_q', 'repair_decided_q <= !DeferredRepair;\n                repair_decision_cursor_q',1)
s=s.replace('if (AdaptiveRepair) begin\n                    if (returned', 'if (AdaptiveRepair && !DeferredRepair) begin\n                    if (returned',1)
needle='            if (ParallelPrefixRepair && !repair_shared_active && cutover_phase_q == 2 &&'
assert needle in s
s=s.replace(needle,'''            // An accepted A prefix is common work: never resized on timeout.
            // Only future suffix/continuation offers depend on this decision.
            if (DeferredRepair && cutover_phase_q == 2 && !repair_decided_q &&
                !old_v_busy_q && returned_q[0] == old_end_q) begin
                if (RepairWaitBudget == 0 || repair_wait_q >= RepairWaitBudget || repair_join_ready) begin
                    repair_decided_q <= 1;
                    repair_decision_cursor_q <= returned_q[0];
                    // Zero budget is the deliberately independent control.
                    repair_shared_q <= RepairWaitBudget != 0 && repair_join_ready;
                end else begin
                    repair_wait_q <= repair_wait_q+1;
                end
            end
'''+needle,1)
s=s.replace('end else if (repair_shared_active && !old_v_busy_q', 'end else if (repair_shared_active && (!DeferredRepair || repair_decided_q) &&\n                                 raw_written_i[0] == old_end_q && raw_written_i[1] == old_end_q && !old_v_busy_q',1)
s=s.replace('    final $display("SR50 mode=', '''    initial begin
        if ((AdaptiveRepair || DeferredRepair) && !ParallelPrefixRepair)
            $fatal(1,"SR72 adaptive/deferred requires parallel repair");
        if (RepairWaitBudget > 65534) $fatal(1,"SR72 finite wait budget out of range");
    end
    always_ff @(posedge clk_i) if (rst_ni && (AdaptiveRepair || DeferredRepair) && mode_i != 3)
        $fatal(1,"SR72 bound prototype requires mode3/FA0");
    final $display("DEFERRED72-OBS enabled=%0d budget=%0d waited=%0d decided=%0d share=%0d",DeferredRepair,RepairWaitBudget,repair_wait_q,repair_decided_q,repair_shared_active);
    final $display("SR50 mode=''')
p.write_text(s)
for name in ['multi_actual_dma_top.sv','multi_system_top.sv']:
 p=D/name;s=p.read_text().replace('parameter bit AdaptiveRepair = 0, RepairChoiceRule = 0,','parameter bit AdaptiveRepair = 0, RepairChoiceRule = 0, DeferredRepair = 0,\n    parameter int unsigned RepairWaitBudget = 64,')
 s=s.replace('output logic dbg_repair_shared, dbg_repair_decided,','output logic dbg_repair_shared, dbg_repair_decided,\n    output logic [15:0] dbg_repair_wait, dbg_repair_budget,')
 if name=='multi_actual_dma_top.sv':
  s=s.replace('if (i_pre.repair_shared_active || (f == 0', 'if (i_pre.repair_shared_active ||\n                        (DeferredRepair && f == 1 && i_pre.returned_q[1] < i_pre.old_end_q) || (f == 0',1)
  s=s.replace('if (f == 1 && start_beat <', 'if (start_beat <',1)
  s=s.replace('.RepairChoiceRule(RepairChoiceRule),', '.RepairChoiceRule(RepairChoiceRule), .DeferredRepair(DeferredRepair), .RepairWaitBudget(RepairWaitBudget),',1)
  s=s.replace('assign dbg_repair_shared =', 'assign dbg_repair_wait = i_pre.repair_wait_q;\n    assign dbg_repair_budget = 16\'(RepairWaitBudget);\n    assign dbg_repair_shared =',1)
 else:
  s=s.replace('.RepairChoiceRule(RepairChoiceRule),', '.RepairChoiceRule(RepairChoiceRule), .DeferredRepair(DeferredRepair), .RepairWaitBudget(RepairWaitBudget),',1)
  s=s.replace('.dbg_repair_shared, .dbg_repair_decided,', '.dbg_repair_shared, .dbg_repair_decided, .dbg_repair_wait, .dbg_repair_budget,',1)
 p.write_text(s)
p=D/'multi_test.cpp';s=p.read_text()
s=s.replace('\\\"cursor\\\":%u,\\\"E\\\":%u}\\n', '\\\"cursor\\\":%u,\\\"E\\\":%u,\\\"waited\\\":%u,\\\"budget\\\":%u}\\n',1)
s=s.replace('(unsigned)top->dbg_repair_endpoint);', '(unsigned)top->dbg_repair_endpoint,(unsigned)top->dbg_repair_wait,(unsigned)top->dbg_repair_budget);',1)
p.write_text(s)
print('root authored later route binding, data-debt-complete join, both prefix clipping')
