#pragma once
// Only the shared physical DDR model accesses Tb::mem. CPU activity always
// uses actual WT cache request/response ports and advances the common clock.
class CpuCache {
    Tb& t;uint64_t fence_nonce=0;bool load_pending=false;
    void context(const char* phase,unsigned packet){t.cpu_phase=phase;t.cpu_packet=packet;}
    void bounded(long start){CHECK(t.g_cycle-start<100000,"actual cache operation timeout");}
public:
    uint64_t load_ops=0,store_ops=0,load_bytes=0,store_bytes=0,fence_cycles=0,drain_cycles=0;
    explicit CpuCache(Tb& tb):t(tb){t.top->c_req=0;t.top->c_we=0;t.top->c_tag_valid=1;t.top->c_kill=0;t.top->c_fence=0;t.top->c_addr=0;t.top->c_wdata=0;t.top->c_be=0;t.top->c_size=3;}
    void begin_load(const char* phase,unsigned packet,uint64_t pa,uint8_t be=255){
      CHECK(!load_pending,"overlapping CPU load");context(phase,packet);++t.cpu_op_seq;++load_ops;load_bytes+=__builtin_popcount(be);
      t.top->c_addr=pa;t.top->c_be=be;t.top->c_we=0;t.top->c_req=1;t.top->c_size=3;t.top->c_tag_valid=1;
      long start=t.g_cycle;do{t.tick();bounded(start);}while(!t.cache_gnt_seen);t.top->c_req=0;load_pending=true;
    }
    uint64_t end_load(){CHECK(load_pending,"no pending load");long start=t.g_cycle;do{t.tick();bounded(start);}while(!t.cache_rvalid_seen);load_pending=false;return t.cache_rdata_seen;}
    uint64_t load(const char* phase,unsigned packet,uint64_t pa,uint8_t be=255){begin_load(phase,packet,pa,be);return end_load();}
    void store(const char* phase,unsigned packet,uint64_t pa,uint64_t value,uint8_t be=255){
      CHECK(!load_pending,"CPU store overlaps outstanding load");context(phase,packet);++t.cpu_op_seq;++store_ops;store_bytes+=__builtin_popcount(be);
      t.top->c_addr=pa;t.top->c_be=be;t.top->c_we=1;t.top->c_req=1;t.top->c_size=3;t.top->c_wdata=value;t.top->c_tag_valid=0;
      long start=t.g_cycle;do{t.tick();bounded(start);}while(!t.cache_gnt_seen);t.top->c_req=0;
    }
    void drain(const char* phase,unsigned packet){
      CHECK(!load_pending,"drain with CPU load pending");context(phase,packet);long start=t.g_cycle;
      do{t.tick();bounded(start);}while(!t.top->c_wb_empty||t.top->c_mshr||t.top->c_mshr1||t.cache_write_requests!=t.cache_write_responses);
      drain_cycles+=t.g_cycle-start;t.finite_event("cpu_store_retired",-1,int(packet)-1,0,t.g_cycle-start,t.cpu_op_seq);
    }
    uint64_t fence(const char* phase,unsigned packet){
      CHECK(!load_pending,"ordinary acquire has pending CPU load");context(phase,packet);long start=t.g_cycle;
      t.finite_event("fence_begin",-1,int(packet)-1,0,0,fence_nonce+1);t.top->c_fence=1;t.tick();t.top->c_fence=0;
      bool ack=false;do{t.tick();ack|=t.cache_ack_seen;bounded(start);}while(!ack||t.top->c_halt||t.top->c_flush_d||t.top->c_i_flush_en);
      CHECK(t.top->c_wb_empty&&!t.top->c_mshr,"fence returned before drain");++fence_nonce;fence_cycles+=t.g_cycle-start;
      t.finite_event("fence_complete",-1,int(packet)-1,0,t.g_cycle-start,fence_nonce);return fence_nonce;
    }
    std::vector<uint8_t> read(const char* phase,unsigned packet,uint64_t offset,size_t length,bool absolute=false){
      std::vector<uint8_t> result;uint64_t pa=(absolute?0:Tb::FINITE_PA)+offset;
      while(length){uint64_t a=pa&~7ULL;unsigned lane=pa&7u,n=std::min<size_t>(length,8-lane);uint8_t be=uint8_t(((1u<<n)-1)<<lane);uint64_t d=load(phase,packet,a,be);
        for(unsigned j=0;j<n;++j)result.push_back(uint8_t(d>>(8*(lane+j))));pa+=n;length-=n;}
      return result;
    }
    void write(const char* phase,unsigned packet,uint64_t offset,const std::vector<uint8_t>& bytes,bool absolute=false){
      uint64_t pa=(absolute?0:Tb::FINITE_PA)+offset;size_t pos=0;
      while(pos<bytes.size()){uint64_t a=pa&~7ULL;unsigned lane=pa&7u,n=std::min<size_t>(bytes.size()-pos,8-lane);uint64_t d=0;for(unsigned j=0;j<n;++j)d|=uint64_t(bytes[pos+j])<<(8*(lane+j));
        store(phase,packet,a,d,uint8_t(((1u<<n)-1)<<lane));pa+=n;pos+=n;}
    }
    static uint64_t metadata(unsigned pid){return Tb::FINITE_PA+c117::kMemoryBytes+(pid-1)*64;}
    void meta_store(const char* phase,unsigned pid,unsigned field,uint64_t value){store(phase,pid,metadata(pid)+field*8,value);}
    uint64_t meta_load(const char* phase,unsigned pid,unsigned field){return load(phase,pid,metadata(pid)+field*8);}
};
