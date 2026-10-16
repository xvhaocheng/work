from pathlib import Path
import difflib
root=Path('/home/xvhaocheng/work/iommu-platform')
out=root/'out/d-dram-action-20261015/implementation'
a=(root/'tests/nonblock_actual_dma_test.cpp').read_text();s=a
s=s.replace('#include "verilated.h"','#include "verilated.h"\n#include "ramulator-bridge.h"')
s=s.replace('long     accepted_cycle = 0;','long     accepted_cycle = 0;\n        uint64_t bridge_uid = 0;')
s=s.replace('bool tail_hold_seen = false; long tail_release_cycle = 0;','bool tail_hold_seen = false; long tail_release_cycle = 0;\n        uint64_t bridge_uid = 0;')
s=s.replace('std::deque<uint32_t> cb_q;', 'std::deque<uint64_t> bridge_b_uids;\n    std::deque<uint32_t> cb_q;')
start=s.index('    void drive_ddr() {');end=s.index('\n#endif',start)
s=s[:start]+'''    void drive_ddr() {
        auto& b=*projection_bridge;
        ddr_setup_wait=false;ddr_model_empty=mem_q.empty();
        top->ddr_aw_ready=b.can_aw();
        top->ddr_w_ready=!caw_q.empty();
        // Reserve both simultaneous handshakes; READY itself does not allocate.
        top->ddr_ar_ready=b.ar_live<16 && b.credits + 33 + (top->ddr_aw_ready?32:0)<=1024;
        top->ddr_b_valid=!cb_q.empty() && !bridge_b_uids.empty() && b.b_ready(bridge_b_uids.front());
        top->ddr_b_id=cb_q.empty()?0:cb_q.front();top->ddr_b_resp=0;
        top->ddr_r_valid=0;
        if(mem_q.empty())return;
        MemRead& h=mem_q.front();ddr_setup_port=(h.id>>4)&3u;
        if(!b.read_ready(h.bridge_uid,h.beat)){ddr_setup_wait=true;return;}
        top->ddr_r_valid=1;top->ddr_r_data=read64(h.addr+uint64_t(h.beat)*8);
        top->ddr_r_id=h.id;top->ddr_r_resp=0;top->ddr_r_last=h.beat==h.len;
    }
''' + s[end:]
# Snapshot size/burst before rising edge (like all original transaction fields).
s=s.replace('    void tick() {','    void tick() {\n        projection_bridge->rtl=g_cycle;',1)
s=s.replace('const bool     f_ddr_ar   =' , '''const unsigned s_ar_size=top->ddr_ar_size,s_ar_burst=top->ddr_ar_burst;
        const unsigned s_aw_size=top->ddr_aw_size,s_aw_burst=top->ddr_aw_burst;
        const bool     f_ddr_ar   =''')
s=s.replace('r.accepted_cycle = g_cycle;\n            mem_q.push_back(r);','r.accepted_cycle = g_cycle;\n            r.bridge_uid=projection_bridge->accept(false,s_ddr_ar_a,s_ddr_ar_l,s_ddr_ar_i,s_ar_size,s_ar_burst);\n            mem_q.push_back(r);')
s=s.replace('c.len = (uint8_t)s_ddr_aw_l; c.got = 0;','c.len = (uint8_t)s_ddr_aw_l; c.got = 0;\n            c.bridge_uid=projection_bridge->accept(true,s_ddr_aw_a,s_ddr_aw_l,s_ddr_aw_i,s_aw_size,s_aw_burst);')
s=s.replace('write_beat(c.addr + (uint64_t)c.got * 8, s_ddr_w_d, s_ddr_w_s);','projection_bridge->beat(c.bridge_uid,s_ddr_w_d,s_ddr_w_s,s_ddr_w_l);\n            write_beat(c.addr + (uint64_t)c.got * 8, s_ddr_w_d, s_ddr_w_s);')
s=s.replace('cb_q.push_back(c.id);\n                caw_q.pop_front();','cb_q.push_back(c.id);\n                bridge_b_uids.push_back(c.bridge_uid);\n                caw_q.pop_front();',1)
s=s.replace('            cb_q.pop_front();\n        }\n        if (f_ddr_r)', '            projection_bridge->b_consumed(bridge_b_uids.front());\n            bridge_b_uids.pop_front();\n            cb_q.pop_front();\n        }\n        if (f_ddr_r)',1)
s=s.replace('            if (h.beat == h.len) mem_q.pop_front();','            projection_bridge->r_consumed(h.bridge_uid,h.beat);\n            if (h.beat == h.len) mem_q.pop_front();',1)
s=s.replace('            projection_bridge->r_consumed(h.bridge_uid,h.beat);','            projection_bridge->log("actual_R_data",h.bridge_uid,0,h.addr+uint64_t(h.beat)*8,top->ddr_r_data,"id="+std::to_string(h.id)+";last="+std::to_string(top->ddr_r_last));\n            projection_bridge->r_consumed(h.bridge_uid,h.beat);')
s=s.replace('        ++g_cycle;' ,'        projection_bridge->tick(g_cycle);\n        ++g_cycle;\n        CHECK(g_cycle <= 7900480L,"frozen total cycle limit");',1)
s=s.replace('        bool     er = top->cfg_error;','        bool     er = top->cfg_error;\n        projection_bridge->log(write?"cfg_write":"cfg_read",sel,0,addr,write?wdata:rd,"error="+std::to_string(er)+";strb="+std::to_string(wstrb));')
s=s.replace('        warm_hw_track = false;\n\n        // ---- post-warm', '        warm_hw_track = false;\n        projection_bridge->phase("original_warm_finish_before_byte_checks");\n\n        // ---- post-warm',1)
s=s.replace('        drain_a();\n        std::printf("WARM-SUMMARY', '        drain_a();\n        projection_bridge->phase("original_warm_and_A_drain");\n        std::printf("WARM-SUMMARY',1)
s=s.replace('        if (full_batch) {\n            run_full_batch(name, enable);', '''        if (full_batch) {
            projection_bridge->phase("pre_t0_after_configs");
            const long added_start=g_cycle;
            while(!projection_bridge->drained() || !mem_q.empty() || !caw_q.empty() || !cb_q.empty() || !bridge_b_uids.empty() || top->dbg_in_flight!=0 || top->dbg_dma_job_quiet!=3 || !top->dbg_cpu_quiet || !top->dbg_cpu_wr_quiet || !top->rst_ni){
                CHECK(g_cycle-added_start<=200000,"added strict drain watchdog");tick();
            }
            CHECK(top->dbg_in_flight==0 && top->dbg_dma_job_quiet==3 && top->dbg_cpu_quiet && top->dbg_cpu_wr_quiet && top->rst_ni,"strong terminal hardware quiet");
            projection_bridge->log("added_terminal_wait",0,0,0,g_cycle-added_start);
            projection_bridge->phase("strong_final_drain");
            // Original warm A source/destination checks already ran; V check
            // copied verbatim from the source scenario-wide full-byte check.
            for(uint32_t i=0;i<PAGE_BEATS;++i){
                CHECK(read64(V_SRC_PA+uint64_t(i)*8)==page_word(0,i),"V source page was modified");
                const uint64_t expected=(warmup>0)?page_word(0,i):page_old(0,i);
                CHECK(read64(V_DST_PA+uint64_t(i)*8)==expected,"V warm destination page does not match its expected state");
            }
            projection_bridge->log("full_warm_byte_checks_pass",0,0,0,warm_a_completed,"data_verified=true;full_task_executed=false;V512words;Aall_completed_ring_pages;source_and_destination");
            std::printf("PREPARATION-ONLY PASS extra_drain=%ld; no t0 or measurement launch executed\\n",g_cycle-added_start);
            return;
            run_full_batch(name, enable);''',1)
s=s.replace('        // V (DMA0 = bank0)', '''        if(top->dbg_dma_issue)projection_bridge->log("hardware_issue",0,0,0,top->dbg_dma_issue);
        if(top->dbg_dma_done)projection_bridge->log("hardware_done",0,0,0,top->dbg_dma_done);
        // V (DMA0 = bank0)''',1)
s=s.replace('        Tb tb;','''        CHECK(mode=="p" || mode=="pn","projection permits only P/PN");
        CHECK(full_batch && transfers==64 && a_target==64 && a_pages==8 && warmup==256 && v_gap==0 && !cpu_enable && !cpu_writes && !ds_priority && !owner_only && !read_latency_overlap && write_tail_pause==0 && read_admit_val==0 && !read_admit_measure_only && !v_cold_dst && start_delay==0 && v_start_delay==0 && a_start_delay==0 && full_watchdog==200000,"frozen projection knobs");
        const char* bridge_config=std::getenv("PROJECTION_CONFIG");
        const char* bridge_output=std::getenv("PROJECTION_OUTPUT");
        const char* bridge_history=std::getenv("PROJECTION_HISTORY");
        CHECK(bridge_config && bridge_output && bridge_history,"missing projection environment");
        CHECK(std::string(bridge_history)=="row0" || std::string(bridge_history)=="row1","bad history");
        projection_bridge=std::make_unique<RamBridge>(bridge_config,bridge_output,std::string(bridge_history)=="row1");
        Tb tb;''',1)
s=s.replace('NOTE: ptw latency is a plain model delay (DDL/DDR), NOT MIG-matched.','NOTE: private projection replaces DDR latency with Ramulator, 1:1 model ticks; NOT MIG-matched.')
# Snapshot the unchanged source functional queues at every harness phase.
s=s.replace('projection_bridge->phase(', 'projection_phase(')
s=s.replace('    void drive_ddr() {','''    void projection_phase(const std::string& label) {
        projection_bridge->log("functional_queues",0,0,0,mem_q.size(),"label="+label+";mem_q="+std::to_string(mem_q.size())+";caw_q="+std::to_string(caw_q.size())+";cb_q="+std::to_string(cb_q.size())+";bridge_b_uids="+std::to_string(bridge_b_uids.size())+";held_R="+std::to_string(ddr_r_held)+";next_rtl_tick="+std::to_string(g_cycle)+";dbg_in_flight="+std::to_string(top->dbg_in_flight)+";dbg_dma_job_quiet="+std::to_string(top->dbg_dma_job_quiet)+";dbg_cpu_quiet="+std::to_string(top->dbg_cpu_quiet)+";dbg_cpu_wr_quiet="+std::to_string(top->dbg_cpu_wr_quiet)+";rst_ni="+std::to_string(top->rst_ni));
        projection_bridge->phase(label);
    }
    void drive_ddr() {''',1)
(out/'preparation-harness.cpp').write_text(s)
(out/'private-source.patch').write_text(''.join(difflib.unified_diff(a.splitlines(True),s.splitlines(True),fromfile='canonical/nonblock_actual_dma_test.cpp',tofile='private/preparation-harness.cpp')))
