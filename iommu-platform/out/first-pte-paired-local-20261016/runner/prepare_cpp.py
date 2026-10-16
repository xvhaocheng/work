#!/usr/bin/env python3
"""Create the isolated runner from the existing fixture; never edit the fixture."""
from pathlib import Path
import hashlib, json
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in HERE.parents if (p/'tests/nonblock_actual_dma_test.cpp').is_file())
src=ROOT/'tests/nonblock_actual_dma_test.cpp'
s=src.read_text()
# Keep existing queue service, AXI scoreboards and genuine register transactions.
s=s[:s.index('    void run_scenario(const char* name, bool enable) {')]
# Relocate data structures, preserving every IOVA and payload PA.
s=s.replace('write64(0x1', 'write64(0xA001')
for old,new in [('0x20000','0xA0020000'),('0x40400','0xA0040400'),('0x50000','0xA0050000'),('0x50008','0xA0050008'),('0x50010','0xA0050010'),('0x30000','0xA0030000'),('0x60800','0xA0060800'),('0x70000','0xA0070000')]:
    s=s.replace('write64('+old,'write64('+new)
for old,new in [('0x8000000000000020ULL','0x80000000000A0020ULL'),('0x8000000000000030ULL','0x80000000000A0030ULL'),('0x0000000000010001ULL','0x0000000028010001ULL'),('0x0000000000014001ULL','0x0000000028014001ULL'),('0x0000000000018001ULL','0x0000000028018001ULL'),('0x000000000001C001ULL','0x000000002801C001ULL')]:
    s=s.replace(old,new)
s=s.replace('// Extended 1-level DDT at 0x10000', '// Relocated 1-level DDT at 0xA0010000')
s=s.replace('V root 0x20000','V root 0xA0020000').replace('A root 0x30000','A root 0xA0030000')
s=s.replace('// V page tables: root 0x20000, L1V 0x40000, L0V 0x50000.', '// V tables: root 0xA0020000, L1 0xA0040000, L0 0xA0050000.')
s=s.replace('// A page tables: root 0x30000, L1A 0x60000, L0A 0x70000.', '// A tables: root 0xA0030000, L1 0xA0060000, L0 0xA0070000.')
s=s.replace('        top->eval();\n\n#ifdef ACTUAL_DMA_SYSTEM\n        CHECK(!top->rst_ni', '        top->eval();\n        paired_pre_edge();  // root observation callback; clock is LOW\n\n#ifdef ACTUAL_DMA_SYSTEM\n        CHECK(!top->rst_ni',1)
s=s.replace('                ++full_b_n[chn];', '                ++full_b_n[chn];\n                paired_b_cycles[chn].push_back(g_cycle);',1)
# Capture issue/done relative to history as well as measurement via callback.
measure=(src.read_text().split('    void full_measure_begin() {',1)[1].split('    // Police every lane',1)[0])
measure='    void full_measure_begin() {'+measure
s+= '\n#ifdef ACTUAL_DMA_SYSTEM\n'+measure
s+=r'''
    // Fixed paired-local entry only.  No batch enable, STOP or old G0 controls.
    int paired_policy = 0;  // labels static elaboration; never drives DUT policy
    std::vector<long> paired_b_cycles[2];
    uint64_t paired_hw_issue[2] = {0, 0}, paired_hw_done[2] = {0, 0};
    long paired_cfg_begin[2] = {0, 0}, paired_cfg_end[2] = {0, 0};
    long paired_next_begin[2] = {0, 0}, paired_next_end[2] = {0, 0};
    uint64_t paired_expected_id[2] = {0, 0};
    long paired_epoch_t0 = 0;
    const char* paired_epoch_name = "setup";

    // Called once per tick after combinational eval and before rising edge.
    // Root may add read-only taps here.  Do not call eval/tick or drive signals.
    void paired_pre_edge() {
        if (!top->rst_ni) return;
        for (int c=0; c<2; ++c) {
            if (top->dbg_dma_issue & (1u<<c)) ++paired_hw_issue[c];
            if (top->dbg_dma_done  & (1u<<c)) ++paired_hw_done[c];
        }
        if (top->ddr_ar_valid && top->ddr_ar_ready) {
            CHECK(top->ddr_ar_addr>=0x80000000ULL && top->ddr_ar_addr<0xc0000000ULL,"AR outside frozen production DRAM decode");
            CHECK(top->ddr_ar_size==3 && top->ddr_ar_burst==1,"DDR AR attributes must retain 64bit INCR");
        }
        if (top->ddr_aw_valid && top->ddr_aw_ready) {
            CHECK(top->ddr_aw_addr>=0x80000000ULL && top->ddr_aw_addr<0xc0000000ULL,"AW outside frozen production DRAM decode");
            CHECK(top->ddr_aw_size==3 && top->ddr_aw_burst==1 && top->ddr_aw_len==255,"COPY AW must retain original 256beat 64bit INCR");
        }
        // ROOT_TRACE_CALLBACK: insert observation of added debug ports here.
    }
    void paired_event(const char* event, int ch, uint64_t value=0) {
        if (event_trace)
            std::fprintf(event_trace,"%ld,%s_%s,%d,0,%llu\n",g_cycle-paired_epoch_t0,
                         paired_epoch_name,event,ch,(unsigned long long)value);
    }
    void paired_verify_payload() {
        for (uint32_t i=0; i<PAGE_BEATS; ++i) {
            CHECK(read64(V_SRC_PA+i*8ULL)==page_word(0,i),"V source changed");
            CHECK(read64(V_DST_PA+i*8ULL)==page_word(0,i),"V COPY byte check");
            CHECK(read64(A_SRC_PA_BASE+i*8ULL)==page_word(1,i),"A source changed");
            CHECK(read64(A_DST_PA_BASE+i*8ULL)==page_word(1,i),"A COPY byte check");
        }
    }
    // Each invocation has exactly the same software operations and fixed order.
    void paired_program() {
        for (int c : {1,0}) {
            paired_cfg_begin[c]=g_cycle; paired_event("cfg_begin",c);
            program_single(c==1,c==1?A_IOVA_BASE:V_SRC_IOVA,
                           c==1?A_IOVA_BASE+PAGE_BYTES:V_DST_IOVA);
            paired_cfg_end[c]=g_cycle; paired_event("cfg_end",c);
        }
        for (int c : {1,0}) {
            paired_next_begin[c]=g_cycle; paired_event("next_begin",c);
            paired_expected_id[c]=trigger(c==1);
            paired_next_end[c]=g_cycle; paired_event("next_end",c,paired_expected_id[c]);
        }
        bool complete[2]={false,false};
        const long begin=g_cycle;
        while (!(complete[0]&&complete[1])) {
            tick();
            CHECK(g_cycle-begin<WATCHDOG_CYCLES,"paired parent completion timeout");
            CHECK(g_cycle-last_progress_cycle<WATCHDOG_CYCLES,"paired no protocol progress");
            for (int c : {0,1})
                if (!complete[c]) complete[c]=cfg_rd(c==1,REG_DONE)==paired_expected_id[c];
        }
        check_quiet(paired_epoch_name);
        paired_verify_payload();
    }
    static std::string paired_arr(const std::vector<long>& a) {
        std::string s="[";
        for (size_t i=0;i<a.size();++i) { if(i) s+=","; s+=std::to_string(a[i]); }
        return s+"]";
    }
    void paired_report(const char* history, long history_cycles,
                       long history_config_cycles, long history_program_cycles) {
        CHECK(top->dbg_cpu_issued==0 && top->dbg_cpu_completed==0,"CPU traffic unexpectedly enabled");
        CHECK(top->dbg_cpu_wr_issued==0 && top->dbg_cpu_amo_issued==0,"CPU writes/AMOs present");
        for (int c=0;c<2;++c) {
            CHECK(full_issue_n[c]==1 && full_done_n[c]==1,"exactly one normal COPY per lane required");
            CHECK(full_ready_cyc[c].size()==1 && full_issue_cyc[c].size()==1 && full_done_cyc[c].size()==1,"COPY timestamp array length");
            CHECK(full_ready_cyc[c][0]==full_t0 && full_t0<=full_issue_cyc[c][0] && full_issue_cyc[c][0]<=full_done_cyc[c][0],"COPY timestamp order");
            CHECK(full_rd_bytes[c]==PAGE_BYTES && full_wr_bytes[c]==PAGE_BYTES,"COPY payload byte accounting");
            CHECK(full_r_n[c]==PAGE_BEATS && full_r_return_bytes[c]==PAGE_BYTES,"COPY return beat accounting");
            CHECK(full_ar_n[c]>0 && full_aw_n[c]==2 && full_b_n[c]==2,"COPY must retain two AW and two B per lane");
            CHECK(full_err[c]==0,"COPY response errors");
            CHECK(paired_b_cycles[c].size()==full_b_n[c] && !paired_b_cycles[c].empty(),"B cycle array length");
            CHECK(paired_b_cycles[c].back()<=full_final_done[c],"parent DONE precedes final bus B");
            CHECK(cfg_rd(c==1,REG_DONE)==paired_expected_id[c],"normal frontend DONE ID mismatch");
            CHECK((cfg_rd(c==1,REG_B_CTL)&1)==0,"autonomous batch unexpectedly enabled");
        }
        CHECK(full_tl_accept==full_tl_resp,"translation request/response conservation");
        std::printf("FIRST-PTE-PAIRED-JSON {\"schema\":\"first-pte-paired-local-v1\",\"history\":\"%s\",\"policy\":%d,\"t0\":%ld,\"history_cycles\":%ld,\"history_config_cycles\":%ld,\"history_program_cycles\":%ld,\"config_prefix_cycles\":%ld,\"T_V\":%ld,\"T_A\":%ld,\"T_data\":%ld,\"T_return\":%ld,\"quiet\":true,\"cpu\":false,\"batch\":false,\"launch_order\":\"av\",\"delay\":0,\"channels\":[",
            history,paired_policy,full_t0,history_cycles,history_config_cycles,history_program_cycles,
            std::max(paired_cfg_end[0],paired_cfg_end[1])-full_t0,
            full_final_done[0]-full_t0,full_final_done[1]-full_t0,
            std::max(full_final_done[0],full_final_done[1])-full_t0,full_quiet_cycle-full_t0);
        for(int c=0;c<2;++c) {
            std::printf("%s{\"id\":\"%s\",\"bytes\":4096,\"issue\":%u,\"done\":%u,\"expected_id\":%llu,\"cfg_begin\":%ld,\"cfg_end\":%ld,\"next_begin\":%ld,\"next_end\":%ld,\"ready\":%s,\"issue_cyc\":%s,\"done_cyc\":%s,\"ar\":%llu,\"aw\":%llu,\"rd_bytes\":%llu,\"wr_bytes\":%llu,\"r_beats\":%llu,\"r_bytes\":%llu,\"b\":%llu,\"b_cyc\":%s,\"err\":%llu}",
                c?",":"",c?"A":"V",full_issue_n[c],full_done_n[c],(unsigned long long)paired_expected_id[c],
                paired_cfg_begin[c],paired_cfg_end[c],paired_next_begin[c],paired_next_end[c],
                paired_arr(full_ready_cyc[c]).c_str(),paired_arr(full_issue_cyc[c]).c_str(),paired_arr(full_done_cyc[c]).c_str(),
                (unsigned long long)full_ar_n[c],(unsigned long long)full_aw_n[c],
                (unsigned long long)full_rd_bytes[c],(unsigned long long)full_wr_bytes[c],
                (unsigned long long)full_r_n[c],(unsigned long long)full_r_return_bytes[c],
                (unsigned long long)full_b_n[c],paired_arr(paired_b_cycles[c]).c_str(),(unsigned long long)full_err[c]);
        }
        std::printf("],\"raw\":{\"tl_accept\":%llu,\"tl_resp\":%llu,\"iotlb_lookup\":%llu,\"iotlb_hit\":%llu,\"ptw_active_cycles\":%llu,\"ptw_r_fire\":%llu,\"ddtw_ar\":%llu,\"ptw_ar\":%llu}}\n",
            (unsigned long long)full_tl_accept,(unsigned long long)full_tl_resp,
            (unsigned long long)full_iotlb_lookup,(unsigned long long)full_iotlb_hit,
            (unsigned long long)full_ptw_active,(unsigned long long)full_ptw_r_fire,
            (unsigned long long)full_ddtw_ar,(unsigned long long)full_ptw_ar);
    }
    void run_paired(const char* history) {
        // Fixed fixture controls. Policy itself is a static SV parameter.
        a_pages=1; v_transfers=1; a_target=1; v_gap_cycles=0; warmup=0;
        cpu_enable=false; cpu_writes=false; ds_priority=false;
        owner_only=false; v_cold_dst=false; full_batch=false;
        read_admit_mode_val=0; write_tail_pause=0; read_latency_overlap=false;
        policy_partition=true; ptw_latency=31; measurement_phase=false;
        setup_memory(1); reset(true); scenario_start=g_cycle;
        check_quiet("initial");
        const long history_begin=g_cycle;
        long history_cfg=0, history_program=0;
        if(std::string(history)=="hot") {
            paired_epoch_name="history"; paired_epoch_t0=g_cycle;
            paired_program();
            CHECK(paired_hw_issue[0]==1&&paired_hw_issue[1]==1&&paired_hw_done[0]==1&&paired_hw_done[1]==1,"hot history must naturally complete two ordinary COPYs");
            history_cfg=std::max(paired_cfg_end[0],paired_cfg_end[1])-history_begin;
            history_program=g_cycle-history_begin;
            // Host fixture changes only DATA while all queues are quiet.
            // Mapping/context/source stay immutable during each COPY pair.
            // Poison destinations after warm so warm payload cannot fake PASS.
            for(uint32_t i=0;i<PAGE_BEATS;++i) {
                write64(V_DST_PA+i*8ULL,page_old(0,i));
                write64(A_DST_PA_BASE+i*8ULL,page_old(1,i));
            }
        }
        const long history_cycles=g_cycle-history_begin;
        measurement_phase=true; paired_epoch_name="measurement";
        full_measure_begin(); paired_epoch_t0=full_t0;
        paired_program(); full_quiet_cycle=g_cycle; full_measure_end();
        const uint64_t expected_total=std::string(history)=="hot"?2:1;
        for(int c=0;c<2;++c)
            CHECK(paired_hw_issue[c]==expected_total && paired_hw_done[c]==expected_total,"history+measured hardware job counts differ from fixed program");
        paired_report(history,history_cycles,history_cfg,history_program);
    }
#endif
};

int main(int argc,char** argv) {
    Verilated::commandArgs(argc,argv);
    std::string history="cold",trace;
    int policy=PAIRED_POLICY;
    for(int i=1;i<argc;++i) {
        const std::string a=argv[i];
        if(a.rfind("+history=",0)==0) history=a.substr(9);
        else if(a.rfind("+policy=",0)==0) policy=std::stoi(a.substr(8));
        else if(a.rfind("+event-trace=",0)==0) trace=a.substr(13);
        else { std::fprintf(stderr,"unsupported fixed-pair argument: %s\n",a.c_str()); return 2; }
    }
    if((history!="cold"&&history!="hot")||policy<0||policy>4||policy!=PAIRED_POLICY) return 2;
    try {
        Tb tb; tb.paired_policy=policy;
        if(!trace.empty()) { tb.event_trace=std::fopen(trace.c_str(),"w"); CHECK(tb.event_trace,"open event trace"); std::fprintf(tb.event_trace,"cycle,event,tag,address,value\n"); }
        tb.run_paired(history.c_str());
    } catch(const std::exception& e) { std::fprintf(stderr,"FIRST-PTE-PAIRED-FAIL: %s\n",e.what()); return 1; }
    std::puts("FIRST-PTE-PAIRED-PASS"); return 0;
}
'''
# This is a local copy; mark inherited commentary as provenance, not live entry.
s='// FIRST-PTE-PAIRED-LOCAL-1 isolated behavioral-fixture derivative.\n// Entry is run_paired(); legacy helpers below are retained but not called.\n'+s
out=HERE/'paired_dma.cpp'; out.write_text(s)
(HERE/'cpp-provenance.json').write_text(json.dumps({'source':str(src),'source_sha256':hashlib.sha256(src.read_bytes()).hexdigest(),'output':str(out),'output_sha256':hashlib.sha256(out.read_bytes()).hexdigest(),'changes':['fixed batch-OFF entry','quiet-before-configuration common t0','DDT/PTW relocate to 0xA00x0000','pre-edge read-only callback','exact 1 COPY/4096 bytes per lane','cold/hot fixture histories; no cache reset between warm and measurement']},indent=2)+'\n')
