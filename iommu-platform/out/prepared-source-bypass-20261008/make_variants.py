#!/usr/bin/env python3
"""Codex §39: isolated ordinary physical source-read bypass, no production edit."""
import json, hashlib
from pathlib import Path
O=Path(__file__).resolve().parent
R=O.parents[1]
B=R/'out/multi-parent-preparation-20261008'
def edit(name,pairs):
    s=(B/name).read_text()
    for old,new in pairs:
        assert s.count(old)==1,(name,old[:90],s.count(old))
        s=s.replace(old,new)
    (O/name).write_text(s)

edit('hit_bypass_parent_translation.sv',[
 ('    input logic whead_valid_i, whead_owner_i,',
  '    output logic [1:0][Depth-1:0] cache_valid_o, cache_failed_o,\n'
  '    output logic [1:0][Depth-1:0][55:0] cache_pa_o,\n'
  '    input logic whead_valid_i, whead_owner_i,'),
 ('    always_comb begin\n        source_first=0;',
  '''    // The response already passed real TL privilege/access checks for the
    // full original normal read key. Restrict direct use to successful readable
    // results of the still-live same generation. Static-mapping PoC only.
    always_comb begin
        for (int f=0; f<2; f++) for (int n=0; n<Depth; n++) begin
            cache_valid_o[f][n] = enable_i && !flush_i && parent_valid_i[f][n] &&
                valid_q[f][n] && seq_q[f][n]==parent_seq_i[f][n] &&
                !value_q[f][n].error && !value_q[f][n].ignore && !value_q[f][n].is_mrif &&
                (value_q[f][n].r || value_q[f][n].bypass);
            cache_pa_o[f][n] = value_q[f][n].spaddr;
            cache_failed_o[f][n] = enable_i && !flush_i && parent_valid_i[f][n] &&
                prepared_q[f][n] && seq_q[f][n]==parent_seq_i[f][n] &&
                !cache_valid_o[f][n] &&
                !((slow_q || prep_offer_q) && slow_src_q && slow_owner_q==1'(f) &&
                  slow_index_q==$clog2(Depth)'(n) && slow_seq_q==parent_seq_i[f][n]);
        end
    end
    always_comb begin
        source_first=0;''')])

edit('compact_pretl_owner_fragment.sv',[
 ('    input logic clk_i, rst_ni, adaptive_i,',
  '    input logic clk_i, rst_ni, adaptive_i,\n'
  '    input logic [1:0] physical_bypass_i,\n'
  '    input logic [1:0][ParentDepth-1:0] cache_valid_i, cache_failed_i, cache_eligible_i,\n'
  '    input logic [1:0][ParentDepth-1:0][55:0] cache_pa_i,\n'
  '    output logic child_direct_o,'),
 ('    logic candidate_owner;', '    logic candidate_owner, candidate_direct, child_direct_q;'),
 ('    logic [3:0] raw_highwater_q [2]; // observation only',
  '    logic [3:0] raw_highwater_q [2]; // observation only\n'
  '    logic held_ar_q; ar_t held_ar_data_q; // observation, no feedback\n'
  '    assign child_direct_o = child_direct_q;\n'
  '    always_ff @(posedge clk_i or negedge rst_ni) begin\n'
  '        if (!rst_ni) begin held_ar_q<=0; held_ar_data_q<=\'0; end\n'
  '        else begin\n'
  '            if (held_ar_q && (!m_ar_valid_o || m_ar_o!=held_ar_data_q))\n'
  '                $fatal(1,"direct source changed held full AR attributes");\n'
  '            held_ar_q<=m_ar_valid_o && !m_ar_ready_i;\n'
  '            if (m_ar_valid_o && !m_ar_ready_i) held_ar_data_q<=m_ar_o;\n'
  '        end\n'
  '    end\n'
  '    final $display("DIRECT39-HOLD held=%0d observation_bits=%0d",held_ar_q,1+$bits(ar_t));'),
 ('D*(2+2*32+64+9)+4*32+9+3+$bits(ar_t)+18);',
  'D*(2+2*32+64+9)+4*32+9+4+$bits(ar_t)+18);'),
 ('        candidate_owner = 0; candidate_beats = 0;',
  '        candidate_owner = 0; candidate_beats = 0; candidate_direct = 0;'),
 ('                    candidate.len = 8\'(capacity - 1);',
  '''                    candidate.len = 8'(capacity - 1);
                    if (physical_bypass_i!=0 && cache_eligible_i[s][n]) begin
                        if (cache_valid_i[s][n]) begin
                            candidate_direct=1;
                            candidate.addr=64'(cache_pa_i[s][n])+64'(returned_q[s][n])*8;
                        end else if (physical_bypass_i==2 && !cache_failed_i[s][n])
                            candidate_valid=0; // wait for an ordinary preparation
                    end'''),
 ('            child_q <= \'0; child_beats_q <= 0; child_got_q <= 0;',
  '            child_q <= \'0; child_beats_q <= 0; child_got_q <= 0; child_direct_q<=0;'),
 ('                child_q <= candidate; child_owner_q <= candidate_owner;',
  '                child_q <= candidate; child_owner_q <= candidate_owner; child_direct_q<=candidate_direct;'),
 ('                if (m_r_i.last) busy_q <= 0;',
  '                if (m_r_i.last) begin busy_q <= 0; child_direct_q<=0; end')])

edit('multi_banked_top.sv',[
 ('    input logic reuse_whead_valid_i, reuse_whead_owner_i,',
  '    input logic reuse_whead_valid_i, reuse_whead_owner_i,\n'
  '    output logic [1:0][3:0] reuse_cache_valid_o, reuse_cache_failed_o,\n'
  '    output logic [1:0][3:0][55:0] reuse_cache_pa_o,'),
 ('        .whead_valid_i(reuse_whead_valid_i), .whead_owner_i(reuse_whead_owner_i),',
  '        .whead_valid_i(reuse_whead_valid_i), .whead_owner_i(reuse_whead_owner_i),\n'
  '        .cache_valid_o(reuse_cache_valid_o), .cache_failed_o(reuse_cache_failed_o), .cache_pa_o(reuse_cache_pa_o),')])

signals='''    // One global source child: its normal TC AR and direct AR are exclusive.
    // IDs 0/4 are the only original source IDs in this harness; bit0 tags direct.
    logic pre_direct, direct_ar_valid, pre_ar_ready, pre_raw_r_valid;
    logic [1:0][3:0] reuse_cache_valid, reuse_cache_failed, direct_eligible;
    logic [1:0][3:0][55:0] reuse_cache_pa;
    logic tc_comp_ar_valid, tc_comp_ar_ready, tc_comp_r_valid, tc_comp_r_ready;
    logic [3:0] tc_comp_ar_id;
    logic [63:0] tc_comp_ar_addr;
    logic [7:0] tc_comp_ar_len;
    logic [2:0] tc_comp_ar_size, tc_comp_ar_prot;
    logic [1:0] tc_comp_ar_burst;
    logic tc_comp_ar_user;
    assign direct_ar_valid=pre_ar_valid && pre_direct;
    assign pre_ar_ready=pre_direct ? comp_ar_ready : tr_ar_ready;
    assign pre_raw_r_valid=pre_direct ? (comp_r_valid && comp_r_id[0]) : tr_r_valid;
    assign tc_comp_ar_ready=comp_ar_ready && !direct_ar_valid;
    assign tc_comp_r_valid=comp_r_valid && !comp_r_id[0];
    assign comp_r_ready=comp_r_id[0] ? (pre_direct && pre_r_ready) : tc_comp_r_ready;
    always_comb begin
        comp_ar_valid=direct_ar_valid || tc_comp_ar_valid;
        comp_ar_id=direct_ar_valid ? (pre_ar.id | 4'd1) : tc_comp_ar_id;
        comp_ar_addr=direct_ar_valid ? pre_ar.addr : tc_comp_ar_addr;
        comp_ar_len=direct_ar_valid ? pre_ar.len : tc_comp_ar_len;
        comp_ar_size=direct_ar_valid ? pre_ar.size : tc_comp_ar_size;
        comp_ar_burst=direct_ar_valid ? pre_ar.burst : tc_comp_ar_burst;
        comp_ar_prot=direct_ar_valid ? pre_ar.prot : tc_comp_ar_prot;
        comp_ar_user=direct_ar_valid ? pre_ar.user : tc_comp_ar_user;
    end
    logic [31:0] direct_count_q, fallback_count_q, write_wait_count_q, direct_cycle_q;
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            direct_count_q<=0; fallback_count_q<=0; write_wait_count_q<=0; direct_cycle_q<=0;
        end else begin
            direct_cycle_q<=direct_cycle_q+1;
            if (direct_ar_valid && tc_comp_ar_valid)
                $fatal(1,"exclusive source AR merge has two inputs");
            if (tc_comp_ar_valid && tc_comp_ar_id!=0 && tc_comp_ar_id!=4)
                $fatal(1,"normal source AR uses reserved direct ID");
            if (comp_r_valid && comp_r_id[0] && (!pre_direct || !i_pre.busy_q))
                $fatal(1,"direct R has no captured child route");
            if (pre_direct && tr_r_valid) $fatal(1,"TC R overlaps direct child");
            if (pre_ar_valid && pre_ar_ready) begin
                if (pre_direct) begin
                    direct_count_q<=direct_count_q+1;
                    if (i_sys.gen_banked.i_banked.gen_bank[0].i_bank.state_q==3'd2 &&
                        i_sys.gen_banked.i_banked.gen_bank[0].i_bank.is_write)
                        write_wait_count_q<=write_wait_count_q+1;
                    $display("DIRECT39-AR cycle=%0d owner=%0d parent=%0d addr=%0d tc_state=%0d tc_write=%0d",
                        direct_cycle_q,i_pre.child_owner_q,i_pre.done_q[i_pre.child_owner_q],pre_ar.addr,
                        i_sys.gen_banked.i_banked.gen_bank[0].i_bank.state_q,
                        i_sys.gen_banked.i_banked.gen_bank[0].i_bank.is_write);
                end else fallback_count_q<=fallback_count_q+1;
            end
        end
    end
    final $display("DIRECT39-STATS mode=%0d direct=%0d fallback=%0d write_wait=%0d offered=%0d busy=%0d observation_bits=128",
        physical_bypass_i,direct_count_q,fallback_count_q,write_wait_count_q,i_pre.offered_q,i_pre.busy_q);
'''
pairs=[
 ('    input logic [1:0] prepare_order_i,', '    input logic [1:0] prepare_order_i, physical_bypass_i,'),
 ('    logic pre_ar_valid, pre_r_ready, pre_r_valid;', '    logic pre_ar_valid, pre_r_ready, pre_r_valid;\n'+signals),
 ('        tr_ar_valid      = pre_ar_valid;', '        tr_ar_valid      = pre_ar_valid && !pre_direct;'),
 ('        tr_r_ready       = pre_r_ready;', '        tr_r_ready       = pre_r_ready && !pre_direct;'),
 ('        pre_raw_r.resp = tr_r_resp; pre_raw_r.last = tr_r_last;',
  '''        pre_raw_r.resp = tr_r_resp; pre_raw_r.last = tr_r_last;
        if (pre_direct) begin
            pre_raw_r.id=comp_r_id & 4'he; pre_raw_r.data=comp_r_data;
            pre_raw_r.resp=comp_r_resp; pre_raw_r.last=comp_r_last;
        end'''),
 ('        .clk_i, .rst_ni, .adaptive_i(1\'b0), .flow_ready_o(pre_flow_ready),',
  '        .clk_i, .rst_ni, .adaptive_i(1\'b0), .flow_ready_o(pre_flow_ready),\n'
  '        .physical_bypass_i, .cache_valid_i(reuse_cache_valid), .cache_failed_i(reuse_cache_failed),\n'
  '        .cache_eligible_i(direct_eligible), .cache_pa_i(reuse_cache_pa), .child_direct_o(pre_direct),'),
 ('        .m_ar_valid_o(pre_ar_valid), .m_ar_ready_i(tr_ar_ready), .m_ar_o(pre_ar),',
  '        .m_ar_valid_o(pre_ar_valid), .m_ar_ready_i(pre_ar_ready), .m_ar_o(pre_ar),'),
 ('        .m_r_valid_i(tr_r_valid), .m_r_ready_o(pre_r_ready), .m_r_i(pre_raw_r),',
  '        .m_r_valid_i(pre_raw_r_valid), .m_r_ready_o(pre_r_ready), .m_r_i(pre_raw_r),'),
 ('            assign reuse_parent_bank[f][n] =',
  '            assign direct_eligible[f][n] = reuse_parent_valid[f][n] && !i_pre.parent_q[f][n].mmu_atst;\n'
  '            assign reuse_parent_bank[f][n] ='),
 ('        .reuse_whead_valid_i(reuse_whead_valid), .reuse_whead_owner_i(reuse_whead_owner),',
  '        .reuse_whead_valid_i(reuse_whead_valid), .reuse_whead_owner_i(reuse_whead_owner),\n'
  '        .reuse_cache_valid_o(reuse_cache_valid), .reuse_cache_failed_o(reuse_cache_failed), .reuse_cache_pa_o(reuse_cache_pa),')]
for k in ['valid','id','addr','len','size','burst','prot','user','ready']:
    # In the named port association, change only this i_sys AR connection.
    import re
    s=(B/'multi_actual_dma_top.sv').read_text()
    pat=r'(        \.comp_ar_'+k+r'\s*\( )comp_ar_'+k+r'(\s*\),)'
    matches=list(re.finditer(pat,s)); assert len(matches)==1,k
    old=matches[0].group(0)
    pairs.append((old,re.sub(pat,r'\1tc_comp_ar_'+k+r'\2',old)))
for k in ['valid','ready']:
    import re
    s=(B/'multi_actual_dma_top.sv').read_text()
    pat=r'(        \.comp_r_'+k+r'\s*\( )comp_r_'+k+r'(\s*\),)'
    matches=list(re.finditer(pat,s)); assert len(matches)==1,k
    old=matches[0].group(0)
    pairs.append((old,re.sub(pat,r'\1tc_comp_r_'+k+r'\2',old)))
edit('multi_actual_dma_top.sv',pairs)

arfields=['addr','len','size','burst','lock','cache','prot','qos','region','user']
edit('multi_system_top.sv',[
 ('    input logic [1:0] prepare_order_i,','    input logic [1:0] prepare_order_i, physical_bypass_i,'),
 ('        .reuse_enable_i(reuse_enable_i), .reuse_prepare_i(reuse_prepare_i), .prepare_order_i,',
  '        .reuse_enable_i(reuse_enable_i), .reuse_prepare_i(reuse_prepare_i), .prepare_order_i, .physical_bypass_i,'),
 ('        u_comp_ar_chan = i_dma.i_sys.comp_req.ar;',
  '        u_comp_ar_chan = i_dma.i_sys.comp_req.ar;\n'
  '        if (i_dma.direct_ar_valid) begin\n'
  '            u_comp_ar_chan.id = i_dma.pre_ar.id | 4\'d1;\n'+
  ''.join('            u_comp_ar_chan.'+f+' = i_dma.pre_ar.'+f+';\n' for f in arfields)+
  '        end')])
edit('multi_test.cpp',[
 ('static unsigned task_prepare_order = 0;', 'static unsigned task_prepare_order = 0;\nstatic unsigned task_physical_bypass = 0;'),
 ('top->prepare_order_i = task_prepare_order;', 'top->prepare_order_i = task_prepare_order; top->physical_bypass_i = task_physical_bypass;'),
 ('        if (const char* v = val("prepare-order")) {',
  '''        if (const char* v = val("physical-bypass")) {
            const std::string setting(v);
            if (setting != "0" && setting != "1" && setting != "2") {
                std::fprintf(stderr, "physical bypass must be 0|1|2\\n"); return 2;
            }
            task_physical_bypass = unsigned(setting[0]-'0');
        }
        if (const char* v = val("prepare-order")) {'''),
 ('        CHECK(n_nat_var > 0 && n_nat_vaw > 0,',
  '        CHECK((n_nat_var > 0 || (task_physical_bypass>0 && n_nat_par>0)) && n_nat_vaw > 0,')])

cmd=json.loads((B/'build-bypass-aw-first-command.json').read_text())
cmd=[str(O/'obj') if x==str(B/'obj-bypass-aw') else x.replace(str(B)+'/',str(O)+'/') for x in cmd]
# Every rewritten source exists; never silently use missing variant.
for x in cmd:
    if x.startswith(str(O)) and x.endswith(('.sv','.cpp')): assert Path(x).is_file(),x
(O/'build-command.json').write_text(json.dumps(cmd,indent=2)+'\n')
before=json.loads((B/'production-before.json').read_text())
now={p:hashlib.sha256((R/p).read_bytes()).hexdigest() for p in before}
assert now==before and len(now)==76
(O/'production-before.json').write_text(json.dumps(now,indent=2)+'\n')
(O/'variant-hashes.json').write_text(json.dumps({p.name:hashlib.sha256(p.read_bytes()).hexdigest()
    for p in O.iterdir() if p.suffix in ['.sv','.cpp']},indent=2)+'\n')
print('prepared six isolated variants; production baseline unchanged')
