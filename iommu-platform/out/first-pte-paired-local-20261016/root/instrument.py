#!/usr/bin/env python3
"""Add observation/assertions to the reused behavioral-DDR C++ fixture."""
from pathlib import Path
R=Path(__file__).resolve().parents[1]
s=(R/'runner/paired_dma.cpp').read_text()
fields=r'''
    bool obs_backend_active[2]={false,false}, obs_local_held[2]={false,false};
    unsigned obs_rbeats[2]={0,0}, obs_local_outstanding[2]={0,0};
    bool obs_l0_complete=false, obs_sel_held=false;
    uint64_t obs_held_addr=0; unsigned obs_held_id=0,obs_held_len=0;
    bool obs_walk=false,obs_first_target=false,obs_first_accept=false,obs_first_local=false;
    bool obs_mask=false,obs_wait=false,obs_done=false;
    void root_event(const char* event,int tag=0,uint64_t addr=0,uint64_t value=0) {
        if(event_trace) std::fprintf(event_trace,"%ld,%s_%s,%d,%llu,%llu\n",
            g_cycle-paired_epoch_t0,paired_epoch_name,event,tag,
            (unsigned long long)addr,(unsigned long long)value);
    }
'''
assert s.count('    void paired_pre_edge() {')==1
s=s.replace('    void paired_pre_edge() {',fields+'\n    void paired_pre_edge() {')
hook=r'''
        // All checks inspect pre-edge signals. They never drive RTL or memory.
        if(obs_sel_held) {
            CHECK(top->dbg_sel_valid,"selector withdrew held VALID");
            CHECK(top->dbg_sel_id==obs_held_id && top->dbg_sel_addr==obs_held_addr &&
                  top->dbg_sel_len==obs_held_len,"selector changed held AR");
        }
        if(top->dbg_m_raw_start) {
            obs_walk=true;obs_first_target=false;obs_first_accept=false;obs_first_local=false;
            unsigned src=0;
            for(int c=0;c<2;++c)
                if(obs_backend_active[c] && (top->dbg_src_drained&(1u<<c))) src|=1u<<c;
            root_event("ptw_start",top->dbg_m_bank,top->dbg_m_root_dram,
                       (unsigned(top->dbg_m_start)<<8)|src);
        }
        if(top->dbg_ptw_local_valid && obs_walk && !obs_first_local) {
            obs_first_local=true;root_event("p1_local_first",top->dbg_m_bank);
        }
        if(top->dbg_m_arm) root_event("m_arm",0,top->dbg_raw_a_addr,top->dbg_raw_a_len);
        if(top->dbg_m_timeout) root_event("m_timeout");
        if(top->dbg_m_commit) {
            CHECK(obs_walk && !obs_first_accept,"M consumed a later PTE as P1");
            CHECK(top->dbg_sel_valid&&top->dbg_sel_ready&&top->dbg_sel_id==48,"M commit wrong endpoint/ID");
            root_event("m_commit",48,top->dbg_sel_addr,top->dbg_sel_len);
        }
        if(top->dbg_sel_valid) {
            if(!obs_sel_held)root_event("sel_first",top->dbg_sel_id,top->dbg_sel_addr,top->dbg_sel_len);
            if(top->dbg_sel_id==48 && !obs_first_target) {
                CHECK(obs_walk,"PTW request without actual start");
                obs_first_target=true;root_event("p1_selector_first",48,top->dbg_sel_addr);
            }
            if(top->dbg_sel_ready) {
                root_event("sel_accept",top->dbg_sel_id,top->dbg_sel_addr,top->dbg_sel_len);
                if(top->dbg_sel_id==48 && !obs_first_accept) {
                    obs_first_accept=true;root_event("p1_selector_accept",48,top->dbg_sel_addr);
                }
            }
        }
        obs_sel_held=top->dbg_sel_valid&&!top->dbg_sel_ready;
        if(obs_sel_held){obs_held_id=top->dbg_sel_id;obs_held_addr=top->dbg_sel_addr;obs_held_len=top->dbg_sel_len;}
        if(bool(top->dbg_m_masked)!=obs_mask){obs_mask=top->dbg_m_masked;root_event(obs_mask?"m_mask_begin":"m_mask_end",0,top->dbg_raw_a_addr,top->dbg_raw_a_len);}
        if(bool(top->dbg_o_wait)!=obs_wait){obs_wait=top->dbg_o_wait;root_event(obs_wait?"o_wait_begin":"o_wait_end");}
        if(bool(top->dbg_o_done)!=obs_done){obs_done=top->dbg_o_done;root_event(obs_done?"o_done_set":"o_done_clear");}
        if(paired_policy==3 && top->dbg_o_l1_offer) {
            CHECK(top->dbg_o_done&&obs_l0_complete,"prepared L1 offered before true L0 DMA RLAST");
        }
        for(int c=0;c<2;++c) {
            const unsigned bit=1u<<c;
            if(top->dbg_backend_fire&bit) {
                CHECK(!obs_backend_active[c],"parent overlap outside qualified profile");
                obs_backend_active[c]=true;obs_rbeats[c]=0;obs_local_outstanding[c]=0;
                if(c==1)obs_l0_complete=false;
                root_event("backend_accept",c);
            }
            const bool av=top->dbg_local_ar_valid&bit,ar=top->dbg_local_ar_ready&bit;
            if(av&&!obs_local_held[c]) {
                const auto a=top->dbg_local_ar_addr[c];const auto n=top->dbg_local_ar_len[c];
                root_event("dma_ar_first",c,a,n);
                if(paired_policy==2 && c==1 && (a&4095)==3072 && n==127)
                    CHECK(obs_l0_complete,"local L1 first VALID preceded accepted L0 RLAST");
                if(paired_policy==4)CHECK(obs_local_outstanding[c]==0,"job pacing offered before previous RLAST");
            }
            obs_local_held[c]=av&&!ar;
            if(av&&ar){++obs_local_outstanding[c];root_event("dma_ar_accept",c,top->dbg_local_ar_addr[c],top->dbg_local_ar_len[c]);}
            if(top->dbg_local_r_fire&bit) {
                CHECK(obs_backend_active[c],"R returned outside accepted parent");
                ++obs_rbeats[c];
                if(top->dbg_local_r_last&bit) {
                    CHECK(obs_local_outstanding[c]>0,"RLAST without local AR");
                    --obs_local_outstanding[c];root_event("dma_rlast",c,0,obs_rbeats[c]);
                    if(c==1 && obs_rbeats[c]==384)obs_l0_complete=true;
                }
            }
            if(top->dbg_dma_done&bit) {
                CHECK(obs_backend_active[c]&&obs_rbeats[c]==512&&obs_local_outstanding[c]==0,"parent retired without all source data");
                obs_backend_active[c]=false;root_event("parent_done",c);
            }
        }
'''
token='        // ROOT_TRACE_CALLBACK: insert observation of added debug ports here.'
assert s.count(token)==1;s=s.replace(token,hook)
s=s.replace('        paired_verify_payload();','        CHECK((top->dbg_m_state&1)==0,"quiet with live M association");\n        paired_verify_payload();')
(R/'combined').mkdir(exist_ok=True)
(R/'combined/paired_dma.cpp').write_text(s)
print(R/'combined/paired_dma.cpp')
