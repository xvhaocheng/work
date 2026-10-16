#pragma once
// Deterministic optimistic CPU port frontend, not a CPU instruction model.
// No CPU data access is made through Tb::mem. The independent real cache ports
// and shared DDR service advance the common clock for every operation.
class CpuCache {
    Tb& t;uint64_t fence_nonce=0;
    void context(const char* phase,unsigned packet){t.cpu_phase=phase;t.cpu_packet=packet;}
    void bounded(long start){CHECK(t.g_cycle-start<200000,"actual cache operation timeout");}
    Tb::CpuTraceRole role(const char* phase,unsigned pid,int span,uint64_t pa){return {phase,pid,span,++t.cpu_op_seq,pa};}
    void idle(){t.top->c_l_req=0;t.top->c_s_req=0;t.top->c_l_tag_valid=0;t.cpu_rsp_role={};}
    void boundary(){CHECK(!t.top->c_halt&&!t.top->c_flush_d&&!t.top->c_fence,"callback during controller halt");if(operation_boundary)operation_boundary();}
public:
    std::function<void()> operation_boundary;
    uint64_t load_ops=0,store_ops=0,load_bytes=0,store_bytes=0,fence_cycles=0,drain_cycles=0;
    uint64_t copy_descriptor_count_peak=0,copy_descriptor_capacity_peak=0,copy_descriptor_storage_peak=0,copy_read_descriptor_size=0,copy_write_descriptor_size=0,copy_pending_reads_peak=0;
    uint64_t copy_load_ops=0,copy_store_ops=0,copy_peak_queued_words=0,copy_peak_queued_bytes=0,copy_read_store_offer_cycles=0,copy_read_store_grant_cycles=0,copy_back_to_back_read_cycles=0;
    explicit CpuCache(Tb& tb):t(tb){idle();t.top->c_l_kill=0;t.top->c_fence=0;t.top->c_l_addr=0;t.top->c_l_tag_addr=0;t.top->c_s_addr=0;t.top->c_s_wdata=0;t.top->c_l_be=0;t.top->c_s_be=0;t.top->c_l_size=3;t.top->c_s_size=3;}
    uint64_t load(const char* phase,unsigned pid,uint64_t pa,uint8_t be=255,int span=-1){
      idle();context(phase,pid);auto r=role(phase,pid,span,pa);t.cpu_l_role=r;t.top->c_l_addr=pa;t.top->c_l_be=be;t.top->c_l_req=1;
      long start=t.g_cycle;do{t.tick();bounded(start);}while(!t.cache_l_gnt_seen);
      ++load_ops;load_bytes+=__builtin_popcount(be);t.top->c_l_req=0;t.top->c_l_tag_addr=pa;t.top->c_l_tag_valid=1;t.cpu_rsp_role=r;
      do{t.tick();bounded(start);}while(!t.cache_rvalid_seen);uint64_t value=t.cache_rdata_seen;idle();boundary();return value;
    }
    void store(const char* phase,unsigned pid,uint64_t pa,uint64_t value,uint8_t be=255,int span=-1){
      idle();context(phase,pid);t.cpu_s_role=role(phase,pid,span,pa);t.top->c_s_addr=pa;t.top->c_s_be=be;t.top->c_s_wdata=value;t.top->c_s_req=1;
      long start=t.g_cycle;do{t.tick();bounded(start);}while(!t.cache_s_gnt_seen);++store_ops;store_bytes+=__builtin_popcount(be);idle();boundary();
    }
    void drain(const char* phase,unsigned pid){
      idle();context(phase,pid);long start=t.g_cycle;
      do{t.tick();bounded(start);}while(!t.top->c_wb_empty||t.top->c_mshr||t.top->c_mshr1||t.cache_write_requests!=t.cache_write_responses);
      drain_cycles+=t.g_cycle-start;t.finite_event("cpu_store_retired",-1,int(pid)-1,0,t.g_cycle-start,t.cpu_op_seq);boundary();
    }
    uint64_t fence(const char* phase,unsigned pid){
      idle();context(phase,pid);long start=t.g_cycle;t.finite_event("fence_begin",-1,int(pid)-1,0,0,fence_nonce+1);t.top->c_fence=1;t.tick();t.top->c_fence=0;
      bool ack=false;do{t.tick();ack|=t.cache_ack_seen;bounded(start);}while(!ack||t.top->c_halt||t.top->c_flush_d||t.top->c_i_flush_en);
      CHECK(t.top->c_wb_empty&&!t.top->c_mshr&&!t.top->c_mshr1&&t.cache_write_requests==t.cache_write_responses,"fence returned before drain");++fence_nonce;fence_cycles+=t.g_cycle-start;
      t.finite_event("fence_complete",-1,int(pid)-1,0,t.g_cycle-start,fence_nonce);return fence_nonce;
    }
    std::vector<uint8_t> read(const char* phase,unsigned pid,uint64_t offset,size_t length,bool absolute=false){
      std::vector<uint8_t> result;uint64_t pa=(absolute?0:Tb::FINITE_PA)+offset;
      while(length){uint64_t a=pa&~7ULL;unsigned lane=pa&7u,n=std::min<size_t>(length,8-lane);uint8_t be=uint8_t(((1u<<n)-1)<<lane);uint64_t d=load(phase,pid,a,be);
        for(unsigned j=0;j<n;++j)result.push_back(uint8_t(d>>(8*(lane+j))));pa+=n;length-=n;}
      return result;
    }
    void write(const char* phase,unsigned pid,uint64_t offset,const std::vector<uint8_t>& bytes,bool absolute=false){
      uint64_t pa=(absolute?0:Tb::FINITE_PA)+offset;size_t pos=0;
      while(pos<bytes.size()){uint64_t a=pa&~7ULL;unsigned lane=pa&7u,n=std::min<size_t>(bytes.size()-pos,8-lane);uint64_t d=0;for(unsigned j=0;j<n;++j)d|=uint64_t(bytes[pos+j])<<(8*(lane+j));
        store(phase,pid,a,d,uint8_t(((1u<<n)-1)<<lane));pa+=n;pos+=n;}
    }
    // Reads and writes have independent queues. Every destination word comes
    // solely from actual returned source words. New read index and old read
    // tag are separate, as required by wt_dcache_ctrl's pipelined READ state.
    void copy(const std::vector<c117::Span>& spans,const std::vector<unsigned>& indices,const std::function<void(unsigned)>& source_retired){
      struct R {Tb::CpuTraceRole role;uint64_t pa;unsigned skip,n,delta;};
      struct W {Tb::CpuTraceRole role;uint64_t pa,value;uint8_t be;};
      std::vector<R> reads;std::array<unsigned,4> left{};
      for(unsigned k:indices){const auto& s=spans.at(k);for(unsigned delta=0;delta<s.length;){uint64_t pa=Tb::FINITE_PA+s.src+delta;unsigned lane=pa&7u,n=std::min(s.length-delta,8-lane);reads.push_back({{"cpu_copy",s.packet,int(k),0,pa&~7ULL},pa&~7ULL,lane,n,delta});++left.at(s.packet);delta+=n;}}
      copy_read_descriptor_size=sizeof(R);copy_write_descriptor_size=sizeof(W);copy_descriptor_count_peak=std::max<uint64_t>(copy_descriptor_count_peak,reads.size());copy_descriptor_capacity_peak=std::max<uint64_t>(copy_descriptor_capacity_peak,reads.capacity());copy_descriptor_storage_peak=std::max<uint64_t>(copy_descriptor_storage_peak,reads.capacity()*sizeof(R));
      std::deque<R> pending;std::deque<W> writes;size_t offered=0;bool partial=false;W word{};int partial_span=-1;
      auto emit=[&](){if(partial){writes.push_back(word);partial=false;uint64_t bytes=0;for(const auto& w:writes)bytes+=__builtin_popcount(w.be);copy_peak_queued_words=std::max<uint64_t>(copy_peak_queued_words,writes.size());copy_peak_queued_bytes=std::max(copy_peak_queued_bytes,bytes);}};
      idle();long start=t.g_cycle;
      while(offered<reads.size()||!pending.empty()||!writes.empty()){
        CHECK(!t.top->c_halt,"CPU copy while controller halted");
        if(offered<reads.size()){
          auto& r=reads[offered];if(!r.role.seq)r.role.seq=++t.cpu_op_seq;t.cpu_l_role=r.role;t.top->c_l_req=1;t.top->c_l_addr=r.pa;t.top->c_l_be=255;
        }else t.top->c_l_req=0;
        t.top->c_l_tag_valid=!pending.empty();if(!pending.empty()){t.top->c_l_tag_addr=pending.front().pa;t.cpu_rsp_role=pending.front().role;}else t.cpu_rsp_role={};
        if(!writes.empty()){
          auto& w=writes.front();if(!w.role.seq)w.role.seq=++t.cpu_op_seq;t.cpu_s_role=w.role;t.top->c_s_req=1;t.top->c_s_addr=w.pa;t.top->c_s_wdata=w.value;t.top->c_s_be=w.be;
        }else t.top->c_s_req=0;
        context("cpu_copy",offered<reads.size()?reads[offered].role.packet:!pending.empty()?pending.front().role.packet:!writes.empty()?writes.front().role.packet:0);
        const bool had_store=!writes.empty(),had_read=offered<reads.size();
        if(had_store&&had_read)++copy_read_store_offer_cycles;
        t.tick();bounded(start);
        if(had_store&&t.cache_s_gnt_seen){++store_ops;++copy_store_ops;store_bytes+=__builtin_popcount(writes.front().be);writes.pop_front();}
        if(had_store&&had_read&&t.cache_s_gnt_seen&&t.cache_l_gnt_seen)++copy_read_store_grant_cycles;
        // Response belongs to preexisting granted request, before any new grant.
        if(t.cache_rvalid_seen){
          CHECK(!pending.empty(),"copy response without source grant");R r=pending.front();pending.pop_front();const auto& s=spans.at(r.role.span);uint64_t value=t.cache_rdata_seen;
          for(unsigned j=0;j<r.n;++j){uint64_t dst=Tb::FINITE_PA+s.dst+r.delta+j,base=dst&~7ULL;unsigned lane=dst&7u;
            if(partial&&(word.pa!=base||partial_span!=r.role.span))emit();
            if(!partial){word={{"cpu_copy",s.packet,r.role.span,0,base},base,0,0};partial=true;partial_span=r.role.span;}
            CHECK(!(word.be&(1u<<lane)),"duplicate CPU destination lane");word.value|=uint64_t(uint8_t(value>>(8*(r.skip+j))))<<(8*lane);word.be|=1u<<lane;
            if(word.be==255)emit();
          }
          if(r.delta+r.n==s.length)emit();
          CHECK(left.at(s.packet)>0,"CPU source retirement underflow");if(--left.at(s.packet)==0)source_retired(s.packet);
        }
        if(had_read&&t.cache_l_gnt_seen){if(t.cache_rvalid_seen)++copy_back_to_back_read_cycles;pending.push_back(reads[offered++]);++load_ops;++copy_load_ops;load_bytes+=8;}
        copy_pending_reads_peak=std::max<uint64_t>(copy_pending_reads_peak,pending.size());
        if(t.copy_frontend_trace){uint64_t qb=0;for(const auto& w:writes)qb+=__builtin_popcount(w.be);std::fprintf(t.copy_frontend_trace,"%ld,%zu,%zu,%zu,%zu,%llu,%u,%d,%llu,%zu,cpu_copy\n",t.g_cycle,reads.size(),reads.capacity(),pending.size(),writes.size(),(unsigned long long)qb,unsigned(partial?word.be:0),partial?partial_span:-1,(unsigned long long)(partial?word.pa:0),offered);}
      }
      CHECK(!partial&&pending.empty()&&writes.empty(),"copy staging not empty");idle();
    }
    static uint64_t metadata(unsigned pid){return Tb::FINITE_PA+c117::kMemoryBytes+(pid-1)*64;}
    void meta_store(const char* phase,unsigned pid,unsigned field,uint64_t value){store(phase,pid,metadata(pid)+field*8,value);}
    uint64_t meta_load(const char* phase,unsigned pid,unsigned field){return load(phase,pid,metadata(pid)+field*8);}
};
