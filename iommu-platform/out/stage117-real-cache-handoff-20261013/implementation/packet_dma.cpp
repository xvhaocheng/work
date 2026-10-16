// Same-owner packet protocol over unchanged ordinary two-DMA/IOMMU RTL.
#include "../../stage117-three-results-20261013/common/fixture.hpp"
#include "packet_protocol.hpp"
#include <fstream>
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "inherited_harness.hpp"
using Clock=std::chrono::steady_clock;
static long long ns(Clock::time_point a,Clock::time_point b){return std::chrono::duration_cast<std::chrono::nanoseconds>(b-a).count();}
static void save(const std::string& path,const std::vector<uint8_t>& bytes){std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());CHECK(f.good(),"snapshot output");}
static std::vector<packet_protocol::Spec> specs(){std::vector<packet_protocol::Spec> v;auto spans=c117::spans();for(const auto& p:c117::packets()){packet_protocol::Spec s{p.packet_id,p.cookie_id,p.length,{},{}};for(unsigned i=0;i<spans.size();++i)if(spans[i].packet==p.packet_id)s.spans.push_back(i);for(const auto& d:p.destination){uint64_t lo=d.offset&~15ULL,hi=(d.offset+d.capacity+15)&~15ULL;s.destination_line_closure.push_back({lo,hi-lo});}v.push_back(s);}return v;}

// All controller reads/writes of behavioral memory pass these two functions.
// Metadata, descriptor graphs and token state live outside the simulated pool.
class HostMemory {
    Tb& t;std::ofstream log;uint64_t sequence=0;
    void record(const char* phase,unsigned pid,bool write,uint64_t offset,const std::vector<uint8_t>& bytes,bool absolute){
        ++sequence;t.finite_event("host_access",-1,int(pid)-1,offset,bytes.size(),sequence,write);
        log<<"{\"sequence\":"<<sequence<<",\"cycle\":"<<t.g_cycle<<",\"phase\":\""<<phase<<"\",\"packet\":"<<pid<<",\"kind\":\""<<(write?"write":"read")<<"\",\"offset\":"<<offset<<",\"length\":"<<bytes.size()<<",\"absolute\":"<<(absolute?"true":"false")<<",\"hex\":\"";
        for(auto b:bytes)log<<std::hex<<std::setw(2)<<std::setfill('0')<<unsigned(b);log<<std::dec<<"\"}\n";CHECK(log.good(),"host access log");
    }
public:
    HostMemory(Tb& tb,const std::string& out):t(tb),log(out+"/host_access.jsonl"){CHECK(log.good(),"access log open");}
    void write(const char* phase,unsigned pid,uint64_t offset,const std::vector<uint8_t>& bytes,bool absolute=false){
        record(phase,pid,true,offset,bytes,absolute);uint64_t base=absolute?0:Tb::FINITE_PA;
        for(size_t i=0;i<bytes.size();++i)t.mem[base+offset+i]=bytes[i];
    }
    std::vector<uint8_t> read(const char* phase,unsigned pid,uint64_t offset,size_t length,bool absolute=false){
        uint64_t base=absolute?0:Tb::FINITE_PA;std::vector<uint8_t> bytes(length);
        for(size_t i=0;i<length;++i)bytes[i]=t.mem.at(base+offset+i);
        record(phase,pid,false,offset,bytes,absolute);return bytes;
    }
};

// Cookie and next fields, rather than DMA span generation, drive receive reads.
static std::vector<uint8_t> receive_record(HostMemory& memory,packet_protocol::Protocol& protocol,
        const c117::Packet& p,const packet_protocol::Token& token,bool consumer){
    std::vector<uint8_t> bytes;uint32_t id=p.cookie_id,left=c117::kHeaderBytes+p.length;
    std::unordered_set<uint32_t> visited;
    while(left){const c117::Descriptor* found=nullptr;for(const auto& d:p.destination)if(d.id==id)found=&d;
        CHECK(found && visited.insert(id).second,"consumer cookie chain");const auto& d=*found;
        uint32_t n=std::min(left,d.capacity);CHECK(d.offset>=c117::kSourceEnd && uint64_t(d.offset)+n<=c117::kMemoryBytes,"consumer bounds");
        protocol.allow_read(p.packet_id,&token,consumer,d.offset,n);
        auto block=memory.read(consumer?"consumer":"prepublish_validation",p.packet_id,d.offset,n);bytes.insert(bytes.end(),block.begin(),block.end());
        left-=n;CHECK(!left || d.has_next,"consumer chain exhausted");id=d.next;
    }
    CHECK(bytes.size()==c117::kHeaderBytes+p.length,"whole record length");
    for(size_t i=0;i<bytes.size();++i)CHECK(bytes[i]==(i<c117::kHeaderBytes?c117::header()[i]:p.frame[i-c117::kHeaderBytes]),"record actual bytes mismatch");
    return bytes;
}
static std::vector<uint8_t> receive_guards(HostMemory& memory,packet_protocol::Protocol& protocol,
        const c117::Packet& p,const packet_protocol::Token& token,bool consumer){
    std::vector<uint8_t> result;uint32_t left=c117::kHeaderBytes+p.length;
    for(const auto& d:p.destination){uint32_t used=std::min(left,d.capacity);left-=used;
        uint32_t lo=d.offset&~15u,hi=(d.offset+d.capacity+15)&~15u;
        for(auto r:std::array<std::pair<unsigned,unsigned>,2>{{{lo,d.offset},{d.offset+used,hi}}})if(r.first<r.second){
            protocol.allow_read(p.packet_id,&token,consumer,r.first,r.second-r.first);
            auto block=memory.read(consumer?"consumer_guard":"validation_guard",p.packet_id,r.first,r.second-r.first);
            for(auto b:block)CHECK(b==0xa5,"packet local capacity/line guard overwritten");result.insert(result.end(),block.begin(),block.end());
        }
    }
    CHECK(left==0,"guard descriptor coverage");return result;
}

int main(int argc,char** argv){
 Verilated::commandArgs(argc,argv);if(argc!=3){std::fprintf(stderr,"usage: packet_dma case output-directory\n");return 2;}
 const std::string mode=argv[1],out=argv[2];const auto begin=Clock::now();
 Tb t;packet_protocol::Protocol protocol(specs(),4);
 try{
  t.cpu_enable=false;t.nonblock_enable=false;t.policy_partition=false;t.ptw_latency=1;t.a_pages=1;t.warmup=0;t.v_transfers=4;t.a_target=0;t.v_gap_cycles=0;
  if(mode=="single_FIFO_ahead")t.finite_expected[0]={0,1,2,3};
  else if(mode=="parallel_FIFO_ahead"){t.finite_expected[0]={0,1};t.finite_expected[1]={2,3};}
  else throw std::runtime_error("only two new protocol cases authorized");
  t.finite_trace=std::fopen((out+"/events.csv").c_str(),"w");CHECK(t.finite_trace,"trace open");
  std::fprintf(t.finite_trace,"cycle,event,engine,span,address,n,id,value\n");t.finite_track=true;
  t.transport_trace=std::fopen((out+"/backend_access.csv").c_str(),"w");CHECK(t.transport_trace,"transport open");
  std::fprintf(t.transport_trace,"cycle,kind,address,length,strobe,value\n");
  HostMemory memory(t,out);auto original=c117::initial_memory();const auto spans=c117::spans();
  auto word=[](std::vector<uint8_t>& v,size_t off,uint64_t value){for(unsigned i=0;i<8;++i)v.at(off+i)=uint8_t(value>>(8*i));};
  std::array<uint64_t,3> bases{{0x10000,0x20000,0x40000}};
  std::array<std::vector<uint8_t>,3> tables;for(auto& a:tables)a.assign(4096,0);
  for(uint64_t entry:{0x280ULL,0x2c0ULL}){word(tables[0],entry,1);word(tables[0],entry+8,0);word(tables[0],entry+16,0xa000);word(tables[0],entry+24,0x8000000000000020ULL);}
  word(tables[1],0,0x10001);word(tables[2],0x400,((Tb::FINITE_PA>>12)<<10)|0xdfULL);
  for(unsigned i=0;i<3;++i)memory.write("initialization",0,bases[i],tables[i],true);
  memory.write("initialization",0,0,original);
  for(const auto& p:c117::packets()){
    std::vector<uint8_t> h(c117::header().begin(),c117::header().end());memory.write("header_prepare",p.packet_id,p.destination.front().offset,h);
    std::copy(h.begin(),h.end(),original.begin()+p.destination.front().offset);t.finite_event("header_prepared",-1,p.packet_id-1,p.cookie_id,h.size(),1);
  }
  save(out+"/initial.bin",original); // serialize constructed bytes, no pool read.
  t.finite_track=false;t.reset(false);t.check_quiet("pre-issue");t.finite_track=true;const long t0=t.g_cycle;t.finite_event("common_ready");
  const auto construction_end=Clock::now();
  for(int e=0;e<2;++e)if(!t.finite_expected[e].empty()){t.finite_cfg_span=-1;t.cfg_wr(e,REG_B_CTL,0);t.cfg_wr(e,REG_CONF,0);t.cfg_wr(e,0x48,1);}
  std::array<size_t,2> pos{};std::array<packet_protocol::Receipt,2> current{};std::array<uint64_t,2> last_done{{1,1}};
  auto prepare=[&](int e,int index){CHECK(!protocol.quarantined(),"prepare while quarantined");const auto& s=spans.at(index);t.finite_cfg_span=index;t.finite_event("prepare_begin",e,index);t.cfg_wr(e,REG_SRC,Tb::FINITE_IOVA+s.src);t.cfg_wr(e,REG_DST,Tb::FINITE_IOVA+s.dst);t.cfg_wr(e,REG_NB,s.length);t.finite_event("prepare_end",e,index);};
  auto launch=[&](int e){int index=t.finite_expected[e][pos[e]];const auto& s=spans.at(index);const auto& p=protocol.packet(s.packet);
    protocol.reserve(index,e,s.packet,p.generation,p.spec.cookie);t.finite_cfg_span=index;
    uint64_t id=t.trigger(e);current[e]=protocol.accept_next(e,id);t.finite_event("next_returned",e,index,0,0,id);
    t.finite_event("span_bound",e,index,s.packet,p.spec.cookie,id,p.generation);
  };
  auto packet_event=[&](const char* event,const c117::Packet& p,uint64_t n){t.finite_event(event,-1,int(p.packet_id)-1,p.cookie_id,n,protocol.packet(p.packet_id).generation);};
  uint64_t validation_bytes=0,consumer_bytes=0,validation_guard_bytes=0,consumer_guard_bytes=0;long long validation_ns=0,consumer_ns=0;
  auto process_prefix=[&](){
    while(unsigned pid=protocol.ready_prefix()){
      const auto& p=c117::packets().at(pid-1);const auto& state=protocol.packet(pid);auto token=protocol.acquire(pid,state.generation);
      t.finite_event("cache_acquired",-1,int(pid)-1,p.cookie_id,token.nonce,token.generation);
      const auto vb=Clock::now();auto record=receive_record(memory,protocol,p,token,false);auto guard=receive_guards(memory,protocol,p,token,false);
      validation_ns+=ns(vb,Clock::now());validation_bytes+=record.size();validation_guard_bytes+=guard.size();
      save(out+"/packet_"+std::to_string(pid)+"_validation.bin",record);save(out+"/packet_"+std::to_string(pid)+"_validation_guards.bin",guard);
      protocol.validated(pid,&token);packet_event("payload_validated",p,record.size());
      protocol.publish(pid,&token);packet_event("used_publish",p,record.size());
      // A lifetime is distinct from B. Reuse exact retired source after validation.
      protocol.allow_source_reuse(pid);
      for(const auto& s:p.source){std::vector<uint8_t> fresh(s.length);for(size_t i=0;i<fresh.size();++i)fresh[i]=uint8_t((s.offset+i)*37+9);memory.write("source_reuse",pid,s.offset,fresh);}
      protocol.source_reused(pid);packet_event("A_reused",p,p.length);
      const auto cb=Clock::now();record=receive_record(memory,protocol,p,token,true);guard=receive_guards(memory,protocol,p,token,true);
      consumer_ns+=ns(cb,Clock::now());consumer_bytes+=record.size();consumer_guard_bytes+=guard.size();
      save(out+"/packet_"+std::to_string(pid)+"_consumer.bin",record);save(out+"/packet_"+std::to_string(pid)+"_guards.bin",guard);
      protocol.consumer_done(pid,&token);packet_event("consumer_completed",p,record.size());
      protocol.return_B(pid);packet_event("B_returned",p,record.size());
    }
  };
  for(int e=0;e<2;++e)if(!t.finite_expected[e].empty()){prepare(e,t.finite_expected[e][0]);launch(e);}
  for(int e=0;e<2;++e)if(t.finite_expected[e].size()>1)prepare(e,t.finite_expected[e][1]);
  bool running=true;while(running){running=false;
    for(int e=0;e<2;++e)if(pos[e]<t.finite_expected[e].size()){
      running=true;if(t.g_cycle-t0>=200000){protocol.timeout();throw std::runtime_error("public poll timeout");}
      int index=t.finite_expected[e][pos[e]];t.finite_cfg_span=index;uint64_t done=t.cfg_rd(e,REG_DONE);
      t.finite_event("done_poll_return",e,index,0,0,current[e].id,done);
      if(done==current[e].id){
        t.finite_event("done_register_matched",e,index,0,0,done);bool complete=protocol.complete(current[e]);
        t.finite_event("span_complete",e,index,current[e].packet,current[e].cookie,done,current[e].generation);last_done[e]=done;
        if(complete){const auto& p=c117::packets().at(current[e].packet-1);packet_event("packet_allspans_complete",p,p.length);packet_event("A_read_retired",p,p.length);}
        ++pos[e];
        // No observer feedback: NEXT first, then publish any ready packet prefix.
        if(pos[e]<t.finite_expected[e].size())launch(e);
        process_prefix();
        if(pos[e]+1<t.finite_expected[e].size())prepare(e,t.finite_expected[e][pos[e]+1]);
      }else if(done!=last_done[e]){protocol.uncertain("unexpected public DONE identity");throw std::runtime_error(protocol.reason());}
    }
  }
  const long last_poll=t.g_cycle;t.check_quiet("terminal");const long quiet=t.g_cycle;t.finite_event("global_quiet");
  // Terminal observers alone use full-pool memory/guards and debug conservation.
  uint64_t expected=0;for(int e=0;e<2;++e){size_t n=t.finite_expected[e].size();CHECK(t.finite_issued[e]==n && t.finite_front[e]==n && t.finite_accepted[e]==n && t.finite_done[e]==n && t.finite_response[e]==n,"ID/capture/completion conservation");for(int k:t.finite_expected[e]){const auto& s=spans[k];expected+=s.length;for(unsigned j=0;j<s.length;++j){CHECK(t.finite_reads[s.src+j]>=1,"missing source R");CHECK(t.finite_writes[s.dst+j]==1,"missing/duplicate W");}}}
  CHECK(t.finite_arbytes==t.finite_rbytes && t.finite_wbytes==expected && t.finite_aw==t.finite_b && t.finite_awq.empty(),"bus conservation");for(const auto& q:t.finite_bq)CHECK(q.empty(),"pending B");
  CHECK(t.n_tl_accept==t.n_tl_resp && t.n_tl_accept>0 && t.n_fast==0,"translation conservation");
  auto final=memory.read("final_snapshot",0,0,c117::kMemoryBytes);auto expected_final=original;
  for(const auto& s:spans){for(unsigned i=0;i<s.length;++i){expected_final[s.dst+i]=original[s.src+i];expected_final[s.src+i]=uint8_t((s.src+i)*37+9);}}
  CHECK(final==expected_final,"terminal complete source/destination/guard equality");save(out+"/final.bin",final);
  for(unsigned i=0;i<3;++i){auto actual=memory.read("final_table_snapshot",0,bases[i],4096,true);CHECK(actual==tables[i],"table mutation");save(out+"/table_final_"+std::to_string(bases[i])+".bin",actual);}
  t.finite_event("terminal_conservation_verified",-1,-1,0,expected);
  protocol.teardown(true);t.finite_event("mapping_teardown");
  // Logical checked teardown: no in-phase unmap, no B-return physical unmap.
  t.mem.clear();std::fclose(t.transport_trace);t.transport_trace=nullptr;std::fclose(t.finite_trace);t.finite_trace=nullptr;const auto end=Clock::now();
  std::ofstream j(out+"/run.json");j<<"{\"pass\":true,\"case\":\""<<mode<<"\",\"cache_backend\":\""<<packet_protocol::cache_backend<<"\",\"application_RTL_latency\":null,\"cache_RTL_latency\":null,\"software_RTL_latency\":null,\"payload_bytes\":"<<expected<<",\"record_bytes\":1675,";
  j<<"\"logical_costs\":{\"header_prepare_bytes\":36,\"prepublish_record_read_bytes\":"<<validation_bytes<<",\"prepublish_guard_read_bytes\":"<<validation_guard_bytes<<",\"consumer_record_read_bytes\":"<<consumer_bytes<<",\"consumer_guard_read_bytes\":"<<consumer_guard_bytes<<",\"source_reuse_write_bytes\":1639},";
  j<<"\"counts\":{\"issued\":["<<t.finite_issued[0]<<","<<t.finite_issued[1]<<"],\"front\":["<<t.finite_front[0]<<","<<t.finite_front[1]<<"],\"backend\":["<<t.finite_accepted[0]<<","<<t.finite_accepted[1]<<"],\"response\":["<<t.finite_response[0]<<","<<t.finite_response[1]<<"],\"done\":["<<t.finite_done[0]<<","<<t.finite_done[1]<<"],\"ar\":"<<t.finite_ar<<",\"aw\":"<<t.finite_aw<<",\"b\":"<<t.finite_b<<",\"read_bus_bytes\":"<<t.finite_rbytes<<",\"write_useful_bytes\":"<<t.finite_wbytes<<",\"translation_accepted\":"<<t.n_tl_accept<<",\"translation_responded\":"<<t.n_tl_resp<<",\"DDTW_AR\":"<<t.n_walk_ddtw<<",\"PTW_AR\":"<<t.n_walk_ptw<<",\"IOTLB_lookup\":"<<t.n_iotlb_lookup<<",\"IOTLB_hit\":"<<t.n_iotlb_hit<<",\"fast\":"<<t.n_fast<<"},";
  j<<"\"protocol_diagnostic_cycles\":{\"common_ready\":"<<t0<<",\"last_done_poll\":"<<last_poll<<",\"global_quiet\":"<<quiet<<"},\"host_ns_execution_cost_only\":{\"construction\":"<<ns(begin,construction_end)<<",\"validation_with_logs\":"<<validation_ns<<",\"consumer_with_logs\":"<<consumer_ns<<",\"whole_main\":"<<ns(begin,end)<<"},\"quarantined\":false,\"checked_teardown\":true}\n";CHECK(j.good(),"run JSON");
  std::printf("PROTOCOL PASS case=%s payload=1639 records=1675\n",mode.c_str());return 0;
 }catch(const std::exception& e){
  protocol.uncertain(e.what());t.finite_event("quarantine_retain");
  // Never scan, publish, consume, clear or teardown uncertain DMA-owned memory.
  std::ofstream f(out+"/failure.json");f<<"{\"pass\":false,\"quarantined\":true,\"mapping_retained\":"<<(protocol.mapping_retained()?"true":"false")<<",\"reason\":\""<<e.what()<<"\"}\n";
  if(t.finite_trace){std::fclose(t.finite_trace);t.finite_trace=nullptr;}if(t.transport_trace){std::fclose(t.transport_trace);t.transport_trace=nullptr;}
  std::fprintf(stderr,"PROTOCOL FAIL %s: %s\n",mode.c_str(),e.what());return 1;
 }
}
