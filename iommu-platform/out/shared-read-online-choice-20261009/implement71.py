from pathlib import Path
D=Path(__file__).resolve().parent
p=D/'shared_pretl_read.sv';s=p.read_text();s=s.replace('ShareRepairSuffix = 1, Q = 2,','ShareRepairSuffix = 1, AdaptiveRepair = 0, RepairChoiceRule = 0, Q = 2,')
s=s.replace('logic independent_v_offer_q;','''logic independent_v_offer_q;
    logic repair_shared_q, repair_decided_q, repair_shared_active;
    logic [15:0] repair_mem_wait_q, repair_consumer_hold_q, repair_samples_q;
    logic [8:0] repair_decision_cursor_q, repair_decision_endpoint_q;
    logic [31:0] repair_memory_cost, repair_join_cost;
    assign repair_shared_active = AdaptiveRepair ? repair_shared_q : ShareRepairSuffix;
    assign repair_memory_cost = 32'(256-(child_offset_q+child_beats_q))*32'(repair_mem_wait_q);
    assign repair_join_cost = 32'(returned_q[0]-returned_q[1])*
                              (32'(repair_consumer_hold_q)+32'(repair_samples_q));''')
# Only behavior references are replaced; static parameter and ternary definition stay.
s=s.replace('repair_end = ShareRepairSuffix ?', 'repair_end = repair_shared_active ?').replace('!ShareRepairSuffix &&','!repair_shared_active &&').replace('end else if (ShareRepairSuffix &&','end else if (repair_shared_active &&')
s=s.replace('old_v_busy_q <= 0; independent_v_offer_q <= 0; parallel_overlap_ars_q <= 0;', '''old_v_busy_q <= 0; independent_v_offer_q <= 0; parallel_overlap_ars_q <= 0;
            repair_shared_q <= ShareRepairSuffix; repair_decided_q <= 0;
            repair_mem_wait_q <= 0; repair_consumer_hold_q <= 0; repair_samples_q <= 0;
            repair_decision_cursor_q <= 0; repair_decision_endpoint_q <= 0;''')
s=s.replace('if (held_ar_q &&', '''// A causal sample of source availability, excluding each first-R latency.
            if (AdaptiveRepair && !repair_decided_q && !start_cutover &&
                cutover_phase_q == 0 && busy_q && child_got_q != 0) begin
                if (m_r_ready_o && !m_r_valid_i && repair_mem_wait_q != 16'hffff)
                    repair_mem_wait_q <= repair_mem_wait_q+1;
                if (m_r_valid_i && !m_r_ready_o && repair_consumer_hold_q != 16'hffff)
                    repair_consumer_hold_q <= repair_consumer_hold_q+1;
                if (m_r_valid_i && m_r_ready_o && repair_samples_q != 16'hffff)
                    repair_samples_q <= repair_samples_q+1;
            end
            if (repair_decided_q && start_cutover) $fatal(1,"SR71 route rebound");
            if (held_ar_q &&''',1)
s=s.replace('if (start_cutover) begin\n', '''if (start_cutover) begin
                repair_decided_q <= 1;
                repair_decision_cursor_q <= returned_q[0];
                repair_decision_endpoint_q <= child_offset_q+child_beats_q;
                if (AdaptiveRepair) begin
                    if (returned_q[0] < returned_q[1]) $fatal(1,"SR71 invalid initial lag");
                    repair_shared_q <= repair_samples_q >= 8 &&
                        (RepairChoiceRule == 0 ? repair_mem_wait_q > repair_consumer_hold_q :
                         repair_memory_cost > repair_join_cost);
                end
''',1)
s=s.replace('final $display("INDEPENDENT69-OBS', 'final $display("REPAIR71-OBS adaptive=%0d rule=%0d decided=%0d share=%0d M=%0d C=%0d H=%0d cursor=%0d E=%0d",AdaptiveRepair,RepairChoiceRule,repair_decided_q,repair_shared_active,repair_mem_wait_q,repair_consumer_hold_q,repair_samples_q,repair_decision_cursor_q,repair_decision_endpoint_q);\nfinal $display("INDEPENDENT69-OBS')
p.write_text(s)
for name in ['multi_actual_dma_top.sv','multi_system_top.sv']:
 p=D/name;s=p.read_text().replace('parameter bit ShareRepairSuffix = 1,','parameter bit ShareRepairSuffix = 1,\n    parameter bit AdaptiveRepair = 0, RepairChoiceRule = 0,')
 needle='output logic [63:0] dbg_shared_cutover_vcut_bytes, dbg_shared_cutover_oldend_bytes,'
 assert needle in s,name
 s=s.replace(needle,needle+'''\n    output logic dbg_repair_shared, dbg_repair_decided,
    output logic [15:0] dbg_repair_M, dbg_repair_C, dbg_repair_H,
    output logic [8:0] dbg_repair_cursor, dbg_repair_endpoint,''',1)
 if name=='multi_actual_dma_top.sv':
  s=s.replace('if (ShareRepairSuffix ||', 'if (i_pre.repair_shared_active ||')
  s=s.replace('.ShareRepairSuffix(ShareRepairSuffix),', '.ShareRepairSuffix(ShareRepairSuffix), .AdaptiveRepair(AdaptiveRepair), .RepairChoiceRule(RepairChoiceRule),')
  mark='    shared_pretl_read #('
  assign='''    assign dbg_repair_shared = i_pre.repair_shared_active;
    assign dbg_repair_decided = i_pre.repair_decided_q;
    assign dbg_repair_M = i_pre.repair_mem_wait_q;
    assign dbg_repair_C = i_pre.repair_consumer_hold_q;
    assign dbg_repair_H = i_pre.repair_samples_q;
    assign dbg_repair_cursor = i_pre.repair_decision_cursor_q;
    assign dbg_repair_endpoint = i_pre.repair_decision_endpoint_q;
'''
  s=s.replace(mark,assign+mark,1)
 else:
  s=s.replace('.ShareRepairSuffix (ShareRepairSuffix),','.ShareRepairSuffix (ShareRepairSuffix),\n        .AdaptiveRepair(AdaptiveRepair), .RepairChoiceRule(RepairChoiceRule),',1)
  s=s.replace('.dbg_shared_cutover, .dbg_shared_cutover_vcut_bytes, .dbg_shared_cutover_oldend_bytes,','.dbg_shared_cutover, .dbg_shared_cutover_vcut_bytes, .dbg_shared_cutover_oldend_bytes,\n        .dbg_repair_shared, .dbg_repair_decided, .dbg_repair_M, .dbg_repair_C, .dbg_repair_H, .dbg_repair_cursor, .dbg_repair_endpoint,',1)
 p.write_text(s)
p=D/'multi_test.cpp';s=p.read_text();s=s.replace('#ifdef REPAIR_INDEPENDENT_CONTROL\n                src_expect', '''#ifdef REPAIR_ADAPTIVE_CONTROL
                CHECK(top->dbg_repair_decided,"natural(adaptive): route not bound");
                src_expect = top->dbg_repair_shared ? e_beats + (want-fa_beats) : 2ULL*want;
#elif defined(REPAIR_INDEPENDENT_CONTROL)
                src_expect''',1)
s=s.replace('#ifdef REPAIR_INDEPENDENT_CONTROL\n            cutover_actual ?', '''#ifdef REPAIR_ADAPTIVE_CONTROL
            cutover_actual ? (top->dbg_repair_shared ? "oldend/8 + (want - initial_A_cut/8)" : "2*want (independent physical sources)")
                           : "legacy_detached_or_shared_formula");
#elif defined(REPAIR_INDEPENDENT_CONTROL)
            cutover_actual ?''',1)
s=s.replace('nat_report_source_cutover_json(beats);','''nat_report_source_cutover_json(beats);
#ifdef REPAIR_ADAPTIVE_CONTROL
        std::printf("REPAIR-ONLINE-JSON {\\\"schema\\\":\\\"repair-online-v1\\\",\\\"decided\\\":%d,\\\"share\\\":%d,\\\"M\\\":%u,\\\"C\\\":%u,\\\"H\\\":%u,\\\"cursor\\\":%u,\\\"E\\\":%u}\\n",
            (int)top->dbg_repair_decided,(int)top->dbg_repair_shared,(unsigned)top->dbg_repair_M,(unsigned)top->dbg_repair_C,(unsigned)top->dbg_repair_H,(unsigned)top->dbg_repair_cursor,(unsigned)top->dbg_repair_endpoint);
#endif''',1)
p.write_text(s)
print('authored scoped online route binding; four changed files')
