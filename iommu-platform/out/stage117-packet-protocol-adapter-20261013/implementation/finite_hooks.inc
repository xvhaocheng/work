    // All taps are observers. No scheduling decision uses these counters.
    static constexpr uint64_t FINITE_IOVA=0x10000000ULL, FINITE_PA=0x80000000ULL;
    bool finite_track=false;
    FILE* finite_trace=nullptr;
    int finite_cfg_span=-1;
    std::array<std::vector<int>,2> finite_expected;
    std::array<uint64_t,2> finite_issued{},finite_front{},finite_accepted{},finite_done{},finite_response{};
    uint64_t finite_ar=0,finite_aw=0,finite_b=0,finite_rbytes=0,finite_arbytes=0,finite_wbytes=0;
    std::array<unsigned,c117::kMemoryBytes> finite_reads{},finite_writes{};
    struct FiniteHeld {bool valid=false;uint64_t src=0,dst=0,len=0,did=0,pv=0;};
    std::array<FiniteHeld,2> finite_fheld{},finite_bheld{};
    struct FiniteAw {uint64_t address;unsigned id,len,got;};
    std::deque<FiniteAw> finite_awq;
    std::array<std::deque<uint64_t>,64> finite_bq;
    void finite_event(const char* name,int engine=-1,int span=-1,uint64_t address=0,uint64_t n=0,uint64_t id=0,uint64_t value=0) {
      if(finite_track && finite_trace) std::fprintf(finite_trace,"%ld,%s,%d,%d,%llu,%llu,%llu,%llu\n",g_cycle,name,engine,span,(unsigned long long)address,(unsigned long long)n,(unsigned long long)id,(unsigned long long)value);
    }
    int finite_span_at(uint64_t p,bool source) {
      const auto all=c117::spans();
      for(unsigned i=0;i<all.size();++i) {
        const auto& s=all[i];uint64_t base=FINITE_PA+(source?s.src:s.dst);
        if(p>=base && p<base+s.length) return int(i);
      }
      return -1;
    }
    int finite_engine_for_span(int span) {
      for(int e=0;e<2;++e) for(int s:finite_expected[e]) if(s==span) return e;
      return -1;
    }
    void finite_check_req(int e,bool front,bool valid,bool ready,uint64_t src,uint64_t dst,uint64_t len,uint64_t did,uint64_t pv) {
      auto& held=front?finite_fheld[e]:finite_bheld[e];
      if(held.valid) CHECK(valid && held.src==src && held.dst==dst && held.len==len && held.did==did && held.pv==pv,"held VALID/request changed before handshake");
      held={valid&&!ready,src,dst,len,did,pv};
      if(!valid || !ready) return;
      auto& count=front?finite_front[e]:finite_accepted[e];
      CHECK(count<finite_expected[e].size(),"unexpected request capture");
      const int idx=finite_expected[e][count];const auto s=c117::spans().at(idx);
      CHECK(src==FINITE_IOVA+s.src && dst==FINITE_IOVA+s.dst && len==s.length,"captured request differs from frozen exact span");
      CHECK(did==uint64_t(10+e) && pv==0,"physical DID/PV mismatch");
      ++count;
      finite_event(front?"front_accepted":"backend_accepted",e,idx,src,len,count,dst);
      finite_event(front?"front_identity":"backend_identity",e,idx,did,pv,count,0);
    }
    void finite_observe() {
      if(!finite_track || !top->rst_ni) return;
      CHECK(!top->dbg_dma_batch_active && !top->dbg_dma_batch_gap && !top->dbg_dma_launch_held && !top->dbg_dma_launch_error,"optional batch/launch active");
      CHECK(!top->dbg_cpu_issued && !top->dbg_cpu_wr_issued && !top->dbg_cpu_amo_issued,"synthetic CPU active");
      CHECK(top->f_ttype0==0 && top->f_ttype1==0,"DEFAULT ttype changed during ahead");
      finite_check_req(0,true,top->f_front_valid0,top->f_front_ready0,top->f_front_src0,top->f_front_dst0,top->f_front_length0,top->f_front_did0,top->f_front_pv0);
      finite_check_req(1,true,top->f_front_valid1,top->f_front_ready1,top->f_front_src1,top->f_front_dst1,top->f_front_length1,top->f_front_did1,top->f_front_pv1);
      finite_check_req(0,false,top->f_back_valid0,top->f_back_ready0,top->f_back_src0,top->f_back_dst0,top->f_back_length0,top->f_back_did0,top->f_back_pv0);
      finite_check_req(1,false,top->f_back_valid1,top->f_back_ready1,top->f_back_src1,top->f_back_dst1,top->f_back_length1,top->f_back_did1,top->f_back_pv1);
      for(int e=0;e<2;++e) {
        if(top->dbg_dma_issue&(1u<<e)) {
          CHECK(finite_issued[e]<finite_expected[e].size(),"extra issue pulse");
          int s=finite_expected[e][finite_issued[e]++];
          finite_event("issue",e,s,0,0,e?top->f_issue_id1:top->f_issue_id0);
        }
        if(e?top->f_response1:top->f_response0) {
          CHECK(finite_response[e]<finite_accepted[e],"response without accepted request");
          int s=finite_expected[e][finite_response[e]++];finite_event("backend_response",e,s);
        }
        if(top->dbg_dma_done&(1u<<e)) {
          CHECK(finite_done[e]<finite_accepted[e],"DONE without accepted request");
          int s=finite_expected[e][finite_done[e]++];
          finite_event("done",e,s,0,0,e?top->f_done_id1:top->f_done_id0);
        }
      }
      if(top->f_tr_ar_fire) finite_event("request_ar",-1,-1,top->f_tr_ar_addr,top->f_tr_ar_did,top->f_tr_ar_id);
      if(top->f_tr_aw_fire) finite_event("request_aw",-1,-1,top->f_tr_aw_addr,top->f_tr_aw_did,top->f_tr_aw_id);
      if(top->dbg_tl_req_valid && top->dbg_tl_req_ready) finite_event("translation_accept",int(top->dbg_tl_req_bank));
      if(top->dbg_tl_resp_valid && top->dbg_tl_resp_ready) finite_event("translation_response",int(top->dbg_tl_resp_bank));
      if(top->dbg_tl_iotlb_lookup) finite_event("iotlb_lookup",int(top->dbg_main_bank),-1,0,top->dbg_tl_iotlb_hit);
      if(top->dbg_iotlb_fill) finite_event("iotlb_fill",int(top->dbg_main_bank));
      if(top->dbg_ddtw_ar_fire) finite_event("ddtw_ar");
      if(top->dbg_ptw_ar_fire) finite_event("ptw_ar");
      if(top->ddr_ar_valid && top->ddr_ar_ready) {
        CHECK(top->ddr_ar_size==3 && top->ddr_ar_burst==1,"DDR requires 64-bit INCR reads");
        const unsigned port=(top->ddr_ar_id>>4)&3;uint64_t n=(uint64_t(top->ddr_ar_len)+1)*8;
        if(port==2){++finite_ar;finite_arbytes+=n;finite_event("payload_ar",int((top->ddr_ar_id>>2)&1),-1,top->ddr_ar_addr,n,top->ddr_ar_id);}
        else{CHECK(port==3,"unexpected DDR read source");finite_event("table_ar",-1,-1,top->ddr_ar_addr,n,top->ddr_ar_id);}
      }
      if(top->ddr_aw_valid && top->ddr_aw_ready) {
        CHECK(top->ddr_aw_size==3 && top->ddr_aw_burst==1 && ((top->ddr_aw_id>>4)&3)==2,"unexpected DDR AW");
        unsigned id=top->ddr_aw_id;finite_awq.push_back({top->ddr_aw_addr,id,top->ddr_aw_len,0});finite_bq[id].push_back(top->ddr_aw_addr);++finite_aw;
        finite_event("payload_aw",int((id>>2)&1),-1,top->ddr_aw_addr,(uint64_t(top->ddr_aw_len)+1)*8,id);
      }
      if(top->ddr_w_valid && top->ddr_w_ready) {
        CHECK(!finite_awq.empty(),"W without AW");auto& w=finite_awq.front();uint64_t p=(w.address&~uint64_t(7))+uint64_t(w.got)*8;
        int e=int((w.id>>2)&1);
        for(unsigned lane=0;lane<8;++lane) if(top->ddr_w_strb&(1u<<lane)) {
          int s=finite_span_at(p+lane,false);CHECK(s>=0 && finite_engine_for_span(s)==e,"WSTRB outside active authorized destination");
          ++finite_writes[p+lane-FINITE_PA];++finite_wbytes;
        }
        finite_event("payload_w",e,-1,p,top->ddr_w_strb,w.id,top->ddr_w_data);
        ++w.got;CHECK(bool(top->ddr_w_last)==(w.got==w.len+1),"W LAST mismatch");if(top->ddr_w_last){finite_event("payload_w_last",e,-1,p,w.got,w.id);finite_awq.pop_front();}
      }
      if(top->ddr_r_valid && top->ddr_r_ready) {
        CHECK(!mem_q.empty(),"R without AR");const auto& r=mem_q.front();uint64_t p=(r.addr&~uint64_t(7))+uint64_t(r.beat)*8;
        CHECK(top->ddr_r_resp==0 && top->ddr_r_id==r.id && bool(top->ddr_r_last)==(r.beat==r.len),"R response identity/length/error");
        if(((r.id>>4)&3)==2) {
          CHECK(p>=FINITE_PA && p+8<=FINITE_PA+c117::kMemoryBytes,"payload R outside pool");finite_rbytes+=8;
          int e=int((r.id>>2)&1);
          for(unsigned lane=0;lane<8;++lane){int s=finite_span_at(p+lane,true);if(s>=0 && finite_engine_for_span(s)==e)++finite_reads[p+lane-FINITE_PA];}
          finite_event("payload_r",e,-1,p,8,r.id,top->ddr_r_data);
        } else finite_event("table_r",-1,-1,p,8,r.id,top->ddr_r_data);
        if(top->ddr_r_last)finite_event(((r.id>>4)&3)==2?"payload_r_last":"table_r_last",-1,-1,p,r.beat+1,r.id);
      }
      if(top->ddr_b_valid && top->ddr_b_ready) {
        unsigned id=top->ddr_b_id;CHECK(top->ddr_b_resp==0 && !finite_bq[id].empty(),"B without AW or error");
        uint64_t p=finite_bq[id].front();finite_bq[id].pop_front();++finite_b;
        finite_event("payload_b",int((id>>2)&1),-1,p,0,id);
      }
    }
