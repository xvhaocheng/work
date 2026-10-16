    // Pre-edge, read-only instrumentation of actual engine and AXI handshakes.
    static constexpr uint64_t FINITE_IOVA=0x10000000ULL, FINITE_PA=0x80000000ULL;
    bool finite_track=false, finite_held=false;
    uint64_t finite_held_src=0,finite_held_dst=0,finite_held_len=0;
    uint64_t finite_issued=0,finite_front=0,finite_accepted=0,finite_done=0;
    uint64_t finite_read_beats=0,finite_read_bus_bytes=0,finite_read_req_bytes=0;
    uint64_t finite_write_beats=0,finite_write_bytes=0,finite_ar=0,finite_aw=0,finite_b=0;
    uint64_t finite_ds_ar=0,finite_ds_r=0;
    std::array<unsigned,c117::kMemoryBytes> finite_reads{},finite_writes{};
    struct FiniteAw {uint64_t address;unsigned id,len,got;};
    std::deque<FiniteAw> finite_awq;
    FILE* finite_trace=nullptr;
    void finite_event(const char* event,uint64_t a=0,uint64_t n=0,uint64_t id=0) {
      if(finite_trace) std::fprintf(finite_trace,"%ld,%s,%llu,%llu,%llu\n",g_cycle,event,(unsigned long long)a,(unsigned long long)n,(unsigned long long)id);
    }
    bool finite_is_source(uint64_t p) {
      for(const auto& s:c117::spans()) if(p>=FINITE_PA+s.src && p<FINITE_PA+s.src+s.length) return true;
      return false;
    }
    bool finite_is_destination(uint64_t p) {
      for(const auto& s:c117::spans()) if(p>=FINITE_PA+s.dst && p<FINITE_PA+s.dst+s.length) return true;
      return false;
    }
    void finite_observe() {
      if(!finite_track || !top->rst_ni) return;
      CHECK(!top->dbg_dma_batch_active && !top->dbg_dma_batch_gap,"batch unexpectedly enabled");
      CHECK(!(top->dbg_dma_issue&2) && !(top->dbg_dma_done&2) && !(top->finite_front_fire&2),"second DMA unexpectedly active");
      CHECK(!top->dbg_cpu_issued && !top->dbg_cpu_wr_issued && !top->dbg_cpu_amo_issued,"synthetic CPU unexpectedly active");
      if(finite_held) CHECK(top->finite_req_valid && top->finite_req_src==finite_held_src && top->finite_req_dst==finite_held_dst && top->finite_req_length==finite_held_len,"backend VALID/payload changed before handshake");
      finite_held=top->finite_req_valid && !top->finite_req_ready;
      finite_held_src=top->finite_req_src;finite_held_dst=top->finite_req_dst;finite_held_len=top->finite_req_length;
      if(top->dbg_dma_issue&1) {++finite_issued;finite_event("issue",0,finite_issued);}
      if(top->finite_front_fire&1){++finite_front;finite_event("front_accepted",0,finite_front);}
      if(top->finite_req_valid && top->finite_req_ready) {
        auto spans=c117::spans();CHECK(finite_accepted<spans.size(),"extra backend acceptance");const auto& s=spans[finite_accepted];
        CHECK(top->finite_req_src==FINITE_IOVA+s.src && top->finite_req_dst==FINITE_IOVA+s.dst && top->finite_req_length==s.length,"actual backend addresses or exact length differ");
        ++finite_accepted;finite_event("backend_accepted",top->finite_req_src,top->finite_req_length,finite_accepted);
      }
      if(top->dbg_dma_done&1) {++finite_done;CHECK(finite_done<=finite_accepted,"DONE without accepted request");finite_event("done",0,finite_done);}
      if(top->ddr_ar_valid && top->ddr_ar_ready) {
        CHECK(top->ddr_ar_size==3 && top->ddr_ar_burst==1,"DDR model requires observed 64-bit INCR reads");
        unsigned port=(top->ddr_ar_id>>4)&3;
        if(port==2) {++finite_ar;finite_read_req_bytes+=(uint64_t(top->ddr_ar_len)+1)*8;finite_event("payload_ar",top->ddr_ar_addr,(uint64_t(top->ddr_ar_len)+1)*8,top->ddr_ar_id);}
        else {CHECK(port==3,"unexpected DDR read source");++finite_ds_ar;finite_event("page_table_ar",top->ddr_ar_addr,(uint64_t(top->ddr_ar_len)+1)*8,top->ddr_ar_id);}
      }
      if(top->ddr_aw_valid && top->ddr_aw_ready) {
        CHECK(top->ddr_aw_size==3 && top->ddr_aw_burst==1,"DDR model requires observed 64-bit INCR writes");
        CHECK(((top->ddr_aw_id>>4)&3)==2,"unexpected non-DMA write");
        finite_awq.push_back({top->ddr_aw_addr,top->ddr_aw_id,top->ddr_aw_len,0});++finite_aw;
        finite_event("payload_aw",top->ddr_aw_addr,(uint64_t(top->ddr_aw_len)+1)*8,top->ddr_aw_id);
      }
      if(top->ddr_w_valid && top->ddr_w_ready) {
        CHECK(!finite_awq.empty(),"write without actual AW");auto& w=finite_awq.front();
        auto p=(w.address&~uint64_t(7))+uint64_t(w.got)*8;++finite_write_beats;
        for(unsigned j=0;j<8;++j) if(top->ddr_w_strb&(1u<<j)) {
          CHECK(finite_is_destination(p+j),"actual WSTRB writes outside source-authorized destination spans");
          ++finite_writes[p+j-FINITE_PA];++finite_write_bytes;
        }
        finite_event("payload_w",p,top->ddr_w_strb,w.id);
        ++w.got;CHECK(bool(top->ddr_w_last)==(w.got==w.len+1),"write LAST/length mismatch");if(top->ddr_w_last)finite_awq.pop_front();
      }
      if(top->ddr_r_valid && top->ddr_r_ready) {
        CHECK(!mem_q.empty(),"read response without request");const auto& r=mem_q.front();
        if(((r.id>>4)&3)==2) {
          auto p=(r.addr&~uint64_t(7))+uint64_t(r.beat)*8;++finite_read_beats;finite_read_bus_bytes+=8;
          CHECK(p>=FINITE_PA && p+8<=FINITE_PA+c117::kMemoryBytes,"payload read beyond allocated pool");
          for(unsigned j=0;j<8;++j) if(finite_is_source(p+j)) ++finite_reads[p+j-FINITE_PA];
          finite_event("payload_r",p,8,r.id);
        } else ++finite_ds_r;
        CHECK(top->ddr_r_resp==0,"DDR R error");
      }
      if(top->ddr_b_valid && top->ddr_b_ready) {++finite_b;CHECK(top->ddr_b_resp==0,"DDR B error");finite_event("payload_b",0,0,top->ddr_b_id);}
    }
