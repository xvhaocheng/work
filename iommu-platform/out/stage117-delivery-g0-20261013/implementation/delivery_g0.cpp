// Test-only six-case projection over unchanged actual WT cache/controller,
// ordinary two iDMA frontends, original IOMMU and frozen finite fixture.
#include "../../stage117-three-results-20261013/common/fixture.hpp"
#include <fstream>
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "inherited_harness.hpp"
#include "cpu_cache.hpp"
#include "delivery_protocol.hpp"
using Clock=std::chrono::steady_clock;
static long long ns(Clock::time_point a,Clock::time_point b){return std::chrono::duration_cast<std::chrono::nanoseconds>(b-a).count();}
static void save(const std::string& path,const std::vector<uint8_t>& bytes){std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());CHECK(f.good(),"snapshot output");}
static void traces(Tb& t,const std::string& out){t.finite_trace=std::fopen((out+"/events.csv").c_str(),"w");CHECK(t.finite_trace,"event trace open");std::fprintf(t.finite_trace,"cycle,event,engine,span,address,n,id,value\n");t.finite_track=true;t.transport_trace=std::fopen((out+"/backend_access.csv").c_str(),"w");CHECK(t.transport_trace,"backing trace open");std::fprintf(t.transport_trace,"cycle,kind,address,length,strobe,value\n");t.cache_open(out);}
static void close_traces(Tb& t){t.cache_close();if(t.transport_trace){std::fclose(t.transport_trace);t.transport_trace=nullptr;}if(t.finite_trace){std::fclose(t.finite_trace);t.finite_trace=nullptr;}}
static std::vector<uint8_t> record(CpuCache& cpu,DeliveryProtocol& protocol,const c117::Packet& p){
 std::vector<uint8_t> bytes;uint32_t id=p.cookie_id,left=c117::kHeaderBytes+p.length;std::unordered_set<uint32_t> visited;
 while(left){const c117::Descriptor* found=nullptr;for(const auto& d:p.destination)if(d.id==id)found=&d;CHECK(found&&visited.insert(id).second,"consumer original cookie chain");const auto& d=*found;uint32_t n=std::min(left,d.capacity);protocol.allow_read(p.packet_id,d.offset,n);auto block=cpu.read("consumer",p.packet_id,d.offset,n);bytes.insert(bytes.end(),block.begin(),block.end());left-=n;CHECK(!left||d.has_next,"consumer chain capacity");id=d.next;}
 CHECK(bytes.size()==c117::kHeaderBytes+p.length,"record length");return bytes;
}
static std::vector<uint8_t> guards(CpuCache& cpu,DeliveryProtocol& protocol,const c117::Packet& p){
 std::vector<uint8_t> result;uint32_t left=c117::kHeaderBytes+p.length;
 for(const auto& d:p.destination){uint32_t used=std::min(left,d.capacity);left-=used;uint32_t lo=d.offset&~15u,hi=(d.offset+d.capacity+15)&~15u;for(auto r:std::array<std::pair<unsigned,unsigned>,2>{{{lo,d.offset},{d.offset+used,hi}}})if(r.first<r.second){protocol.allow_read(p.packet_id,r.first,r.second-r.first);auto block=cpu.read("consumer_guard",p.packet_id,r.first,r.second-r.first);result.insert(result.end(),block.begin(),block.end());}}
 CHECK(!left,"guard chain coverage");return result;
}
static int interface_probe(const std::string& out){
 Tb t;CpuCache cpu(t);t.cpu_enable=false;t.nonblock_enable=false;t.policy_partition=false;t.ptw_latency=1;t.warmup=0;t.a_target=0;
 try{traces(t,out);for(uint64_t a=Tb::FINITE_PA;a<Tb::FINITE_PA+c117::kMemoryBytes+192;++a)t.mem[a]=0;t.finite_track=false;t.reset(false);t.finite_track=true;
  std::vector<uint8_t> initial(c117::kMemoryBytes,0xa5);const unsigned src=4093,dst=12291,n=43;for(unsigned i=0;i<n;++i)initial[src+i]=uint8_t(i*37+19);cpu.write("probe_init",0,0,initial);cpu.fence("probe_startup",0);
  save(out+"/initial.bin",initial);t.finite_event("probe_begin");unsigned retired=0;cpu.copy({{1,src,dst,n}},{0},[&](unsigned pid){CHECK(pid==1&&!retired,"probe source return");retired=1;t.finite_event("probe_source_retired",-1,0,src,n);});cpu.drain("probe_copy_drain",1);t.finite_event("probe_copy_retired");
  cpu.meta_store("probe_used",1,2,n);cpu.drain("probe_used_drain",1);t.finite_event("probe_used_publish",-1,0,CpuCache::metadata(1)+16,n);
  auto result=cpu.read("probe_consumer",1,dst,n);save(out+"/consumer.bin",result);auto meta=cpu.meta_load("probe_meta",1,2);CHECK(meta==n,"probe metadata");for(unsigned i=0;i<n;++i)CHECK(result[i]==initial[src+i],"probe stitching mismatch");CHECK(cpu.copy_read_store_offer_cycles&&cpu.copy_read_store_grant_cycles&&cpu.copy_back_to_back_read_cycles,"probe overlap or pipelined witness absent");cpu.drain("probe_release",0);t.check_quiet("probe release");t.finite_event("probe_release_quiet");
  std::ofstream f(out+"/probe.json");f<<"{\"pass\":true,\"source\":"<<src<<",\"destination\":"<<dst<<",\"length\":"<<n<<",\"source_retired\":"<<retired<<",\"read_store_offer_cycles\":"<<cpu.copy_read_store_offer_cycles<<",\"read_store_grant_cycles\":"<<cpu.copy_read_store_grant_cycles<<",\"back_to_back_read_cycles\":"<<cpu.copy_back_to_back_read_cycles<<",\"peak_queued_words\":"<<cpu.copy_peak_queued_words<<",\"peak_queued_bytes\":"<<cpu.copy_peak_queued_bytes<<",\"partial_word_bytes\":8,\"old_microtests_replayed\":false}\n";close_traces(t);std::puts("NEW INTERFACE PROBE PASS");return 0;
 }catch(const std::exception& e){std::ofstream f(out+"/failure.json");f<<"{\"pass\":false,\"reason\":\""<<e.what()<<"\"}\n";close_traces(t);std::fprintf(stderr,"INTERFACE PROBE FAIL %s\n",e.what());return 1;}
}
int main(int argc,char** argv){
 Verilated::commandArgs(argc,argv);if(argc!=3){std::fprintf(stderr,"usage: delivery_g0 case output\n");return 2;}std::string mode=argv[1],out=argv[2];if(mode=="interface_probe")return interface_probe(out);const auto begin=Clock::now();Tb t;CpuCache cpu(t);DeliveryProtocol protocol;
 try{
  const bool CPU=mode=="cpu_eager"||mode=="cpu_batch",single=mode=="single_eager"||mode=="single_batch",parallel=mode=="parallel_eager"||mode=="parallel_batch",batch=mode.find("_batch")!=std::string::npos;CHECK(CPU||single||parallel,"unauthorized case");
  t.cpu_enable=false;t.nonblock_enable=false;t.policy_partition=false;t.ptw_latency=1;t.a_pages=1;t.warmup=0;t.v_transfers=4;t.a_target=0;t.v_gap_cycles=0;
  if(single)t.finite_expected[0]={0,1,2,3};if(parallel){t.finite_expected[0]={0,1};t.finite_expected[1]={2,3};}traces(t,out);
  auto initial=c117::initial_memory();const auto spans=c117::spans();auto word=[](std::vector<uint8_t>& v,size_t off,uint64_t value){for(unsigned i=0;i<8;++i)v.at(off+i)=uint8_t(value>>(8*i));};std::array<uint64_t,3> bases{{0x10000,0x20000,0x40000}};std::array<std::vector<uint8_t>,3> tables;for(auto& a:tables)a.assign(4096,0);
  for(uint64_t entry:{0x280ULL,0x2c0ULL}){word(tables[0],entry,1);word(tables[0],entry+8,0);word(tables[0],entry+16,0xa000);word(tables[0],entry+24,0x8000000000000020ULL);}word(tables[1],0,0x10001);word(tables[2],0x400,((Tb::FINITE_PA>>12)<<10)|0xdfULL);
  // Zero backing allocation before reset only. Frozen bytes/tables/metadata
  // enter shared physical DDR exclusively via actual CPU cache stores.
  for(uint64_t a=Tb::FINITE_PA;a<Tb::FINITE_PA+c117::kMemoryBytes+192;++a)t.mem[a]=0;for(auto base:bases)for(unsigned i=0;i<4096;++i)t.mem[base+i]=0;t.finite_track=false;t.reset(false);t.finite_track=true;
  for(unsigned i=0;i<3;++i)cpu.write("initialization",0,bases[i],tables[i],true);cpu.write("initialization",0,0,initial);
  for(const auto& p:c117::packets()){std::vector<uint8_t> h(c117::header().begin(),c117::header().end());cpu.write("header_prepare",p.packet_id,p.destination.front().offset,h);std::copy(h.begin(),h.end(),initial.begin()+p.destination.front().offset);t.finite_event("header_prepared",-1,p.packet_id-1,p.cookie_id,h.size(),1);}
  for(const auto& p:c117::packets()){cpu.meta_store("metadata_init",p.packet_id,0,1);cpu.meta_store("metadata_init",p.packet_id,1,p.cookie_id);for(unsigned k=2;k<8;++k)cpu.meta_store("metadata_init",p.packet_id,k,0);}
  cpu.fence("startup_fence",0);save(out+"/initial.bin",initial);t.check_quiet("pre-issue");const long common=t.g_cycle;t.finite_event("common_ready");const auto construction_end=Clock::now();
  std::array<size_t,2> pos{};std::array<uint64_t,2> current{},last_done{{1,1}},last_next{{1,1}};std::array<long,3> Areturned{},used{},consumer{},Breturned{},source_metadata{};std::array<bool,3> source_queued{};uint64_t prefix_groups=0,consumer_bytes=0,consumer_guard_bytes=0,public_polls=0;bool in_prefix=false,in_poll=false;
  auto packet_event=[&](const char* name,unsigned pid,uint64_t n=0,uint64_t value=1){const auto& p=c117::packets()[pid-1];t.finite_event(name,-1,pid-1,p.cookie_id,n,1,value);};
  auto logical_return=[&](unsigned pid){protocol.return_A(pid);Areturned[pid-1]=t.g_cycle;source_queued[pid-1]=true;packet_event("A_returned",pid,c117::packets()[pid-1].length);};
  auto prepare=[&](int e,int index){const auto& s=spans.at(index);t.finite_cfg_span=index;t.finite_event("prepare_begin",e,index);t.cfg_wr(e,REG_SRC,Tb::FINITE_IOVA+s.src);t.cfg_wr(e,REG_DST,Tb::FINITE_IOVA+s.dst);t.cfg_wr(e,REG_NB,s.length);t.finite_event("prepare_end",e,index);};
  auto launch=[&](int e){int index=t.finite_expected[e][pos[e]];const auto& s=spans[index];const auto& p=c117::packets()[s.packet-1];t.finite_cfg_span=index;uint64_t id=t.trigger(e);CHECK(id==last_next[e]+1,"unexpected NEXT identity");last_next[e]=id;current[e]=id;t.finite_event("next_returned",e,index,0,0,id);t.finite_event("span_bound",e,index,s.packet,p.cookie_id,id,1);};
  auto active=[&](){for(int e=0;e<2;++e)if(pos[e]<t.finite_expected[e].size())return true;return false;};
  // Poll callback performs no CPU cache operation and never services a prefix.
  auto poll=[&](){CHECK(!in_poll,"nested poll");CHECK(!t.top->c_halt&&!t.top->c_flush_d&&!t.top->c_fence,"public poll during fence halt");in_poll=true;t.cpu_phase="public_poll";t.cpu_packet=0;t.finite_event("poll_round_begin");
    for(int e=0;e<2;++e)if(pos[e]<t.finite_expected[e].size()){
      CHECK(t.g_cycle-common<200000,"public poll timeout");int index=t.finite_expected[e][pos[e]];t.finite_cfg_span=index;uint64_t done=t.cfg_rd(e,REG_DONE);++public_polls;t.finite_event("done_poll_return",e,index,0,0,current[e],done);
      if(done==current[e]){t.finite_event("done_register_matched",e,index,0,0,done);const auto& s=spans[index];t.finite_event("span_complete",e,index,s.packet,c117::packets()[s.packet-1].cookie_id,done,1);last_done[e]=done;
        if(protocol.complete_dma(index)){packet_event("packet_allspans_complete",s.packet,c117::packets()[s.packet-1].length);packet_event("A_read_retired",s.packet,c117::packets()[s.packet-1].length);logical_return(s.packet);}
        ++pos[e];if(pos[e]<t.finite_expected[e].size())launch(e);if(pos[e]+1<t.finite_expected[e].size())prepare(e,t.finite_expected[e][pos[e]+1]);
      }else CHECK(done==last_done[e],"unexpected public DONE");
    }t.finite_event("poll_round_end");in_poll=false;
  };
  if(!CPU){for(int e=0;e<2;++e)if(!t.finite_expected[e].empty()){t.finite_cfg_span=-1;t.cfg_wr(e,REG_B_CTL,0);t.cfg_wr(e,REG_CONF,0);t.cfg_wr(e,0x48,1);}for(int e=0;e<2;++e)if(!t.finite_expected[e].empty()){prepare(e,t.finite_expected[e][0]);launch(e);}for(int e=0;e<2;++e)if(t.finite_expected[e].size()>1)prepare(e,t.finite_expected[e][1]);cpu.operation_boundary=[&](){if(active())poll();};}
  auto service_prefix=[&](){CHECK(!in_prefix&&!in_poll,"nested prefix service");auto ids=protocol.ready_prefix();if(ids.empty())return;in_prefix=true;++prefix_groups;uint64_t mask=0;for(unsigned pid:ids)mask|=1u<<(pid-1);t.finite_event("prefix_claim",-1,int(ids.front())-1,mask,ids.size(),prefix_groups,ids.back());
    uint64_t receipt=CPU?1:cpu.fence(batch?"batch_acquire":"prefix_acquire",ids.front());
    for(unsigned pid:ids){protocol.acquire(pid,receipt);packet_event("cache_acquired",pid,protocol.packet(pid).nonce,receipt);cpu.meta_store("used_metadata",pid,3,protocol.packet(pid).nonce);cpu.meta_store("used_metadata",pid,2,c117::kHeaderBytes+c117::packets()[pid-1].length);}
    // All group stores receive one ordinary WT drain, mirroring a shared
    // completeprefix release boundary, without an artificial perpacket fence.
    std::vector<unsigned> source_fields;for(unsigned pid=1;pid<=3;++pid)if(source_queued[pid-1]&&!protocol.packet(pid).source_metadata_retired)source_fields.push_back(pid);
    for(unsigned pid:source_fields)cpu.meta_store("source_return_metadata",pid,6,1);
    cpu.drain("prefix_publish_drain",ids.front());
    for(unsigned pid:source_fields){protocol.packet(pid).source_metadata_retired=true;source_metadata[pid-1]=t.g_cycle;packet_event("source_return_metadata_retired",pid,8);}
    t.finite_event("prefix_release",-1,int(ids.front())-1,mask,ids.size(),prefix_groups,ids.back());for(unsigned pid:ids){protocol.publish(pid);used[pid-1]=t.g_cycle;packet_event("used_publish",pid,c117::kHeaderBytes+c117::packets()[pid-1].length,prefix_groups);}
    for(unsigned pid:ids){const auto& p=c117::packets()[pid-1];t.finite_event("consumer_start",-1,int(pid)-1,p.cookie_id);std::vector<uint8_t> actual_metadata;for(unsigned field=0;field<4;++field){uint64_t v=cpu.meta_load("consumer_metadata",pid,field);for(unsigned j=0;j<8;++j)actual_metadata.push_back(uint8_t(v>>(8*j)));}save(out+"/packet_"+std::to_string(pid)+"_used_metadata.bin",actual_metadata);
      auto data=record(cpu,protocol,p),guard=guards(cpu,protocol,p);consumer_bytes+=data.size();consumer_guard_bytes+=guard.size();save(out+"/packet_"+std::to_string(pid)+"_consumer.bin",data);save(out+"/packet_"+std::to_string(pid)+"_guards.bin",guard);
      cpu.meta_store("consumer_done_metadata",pid,4,1);cpu.drain("consumer_done_drain",pid);protocol.consume(pid);consumer[pid-1]=t.g_cycle;packet_event("consumer_completed",pid,data.size());cpu.meta_store("B_return_metadata",pid,5,1);cpu.drain("B_return_drain",pid);protocol.return_B(pid);Breturned[pid-1]=t.g_cycle;packet_event("B_returned",pid,data.size());
    }in_prefix=false;
  };
  if(CPU){
    auto source_retire=[&](unsigned pid){protocol.retire_source(pid);packet_event("A_read_retired",pid,c117::packets()[pid-1].length);logical_return(pid);};
    if(batch){cpu.copy(spans,{0,1,2,3},source_retire);cpu.drain("CPU_batch_copy_drain",0);for(unsigned pid=1;pid<=3;++pid){protocol.complete_cpu(pid);packet_event("packet_allspans_complete",pid,c117::packets()[pid-1].length);}service_prefix();}
    else for(unsigned pid=1;pid<=3;++pid){std::vector<unsigned> indices;for(unsigned k=0;k<spans.size();++k)if(spans[k].packet==pid)indices.push_back(k);cpu.copy(spans,indices,source_retire);cpu.drain("CPU_packet_copy_drain",pid);protocol.complete_cpu(pid);packet_event("packet_allspans_complete",pid,c117::packets()[pid-1].length);service_prefix();}
  }else if(batch){while(active())poll();service_prefix();}
  else{while(active()||protocol.published_prefix<3){if(active())poll();service_prefix();}}
  cpu.operation_boundary={};const long last_poll=t.g_cycle;cpu.drain("terminal_cpu_drain",0);t.check_quiet("terminal");const long quiet=t.g_cycle;t.finite_event("global_quiet");
  // Terminal observers alone inspect every source/destination/guard/table byte.
  // Expected frame data is never used as execution data or before publication.
  uint64_t dma_bytes=0;for(int e=0;e<2;++e){size_t n=t.finite_expected[e].size();CHECK(t.finite_issued[e]==n&&t.finite_front[e]==n&&t.finite_accepted[e]==n&&t.finite_done[e]==n&&t.finite_response[e]==n,"DMA capture/completion conservation");for(int k:t.finite_expected[e]){const auto& s=spans[k];dma_bytes+=s.length;for(unsigned j=0;j<s.length;++j){CHECK(t.finite_reads[s.src+j]>=1,"missing actual DMA source R");CHECK(t.finite_writes[s.dst+j]==1,"missing/duplicate actual DMA W");}}}
  CHECK(t.finite_arbytes==t.finite_rbytes&&t.finite_wbytes==dma_bytes&&t.finite_aw==t.finite_b&&t.finite_awq.empty(),"DMA bus conservation");for(const auto& q:t.finite_bq)CHECK(q.empty(),"pending DMA B");CHECK(t.n_tl_accept==t.n_tl_resp&&t.n_fast==0,"translation conservation");if(!CPU)CHECK(t.n_tl_accept>0,"missing DMA translations");
  cpu.fence("terminal_scan_fence",0);auto final=cpu.read("final_snapshot",0,0,c117::kMemoryBytes);save(out+"/final.bin",final);
  for(unsigned i=0;i<3;++i){auto actual=cpu.read("final_table_snapshot",0,bases[i],4096,true);save(out+"/table_final_"+std::to_string(bases[i])+".bin",actual);}
  for(unsigned pid=1;pid<=3;++pid)save(out+"/metadata_final_"+std::to_string(pid)+".bin",cpu.read("final_metadata_snapshot",pid,CpuCache::metadata(pid),64,true));
  t.finite_event("terminal_scan_complete",-1,-1,0,16384+12288+192);cpu.drain("release_cpu_drain",0);t.check_quiet("postscan release");const long release=t.g_cycle;t.finite_event("release_quiet");protocol.teardown();t.finite_event("mapping_teardown");close_traces(t);t.mem.clear();const auto end=Clock::now();
  std::ofstream j(out+"/run.json");j<<"{\"pass\":true,\"case\":\""<<mode<<"\",\"cache_backend\":\"ACTUAL_CVA6_WT16B_CONTROLLER_INPUT\",\"payload_bytes\":1639,\"record_bytes\":1675,\"dma_payload_bytes\":"<<dma_bytes<<",\"source_final_unchanged_claim_requires_checker\":true,\"prepublication_record_read_bytes\":0,\"source_reuse_write_bytes\":0,\"consumer_record_read_bytes\":"<<consumer_bytes<<",\"consumer_guard_read_bytes\":"<<consumer_guard_bytes<<",\"prefix_groups\":"<<prefix_groups<<",\"public_polls\":"<<public_polls<<",";
  auto vector=[&](const char* name,const std::array<long,3>& values){j<<"\""<<name<<"\":[";for(unsigned i=0;i<3;++i){if(i)j<<",";j<<values[i]-common;}j<<"],";};vector("used_publish_vector",used);vector("consumer_vector",consumer);vector("B_return_vector",Breturned);vector("A_return_vector",Areturned);vector("source_metadata_retired_vector",source_metadata);
  j<<"\"actual_cache\":{\"cpu_load_ops\":"<<cpu.load_ops<<",\"cpu_store_ops\":"<<cpu.store_ops<<",\"cpu_load_bytes\":"<<cpu.load_bytes<<",\"cpu_store_bytes\":"<<cpu.store_bytes<<",\"fence_cycles\":"<<cpu.fence_cycles<<",\"drain_cycles\":"<<cpu.drain_cycles<<",\"controller_fences\":"<<t.controller_flushes<<",\"misses\":"<<t.cache_misses<<",\"refill_AR\":"<<t.cache_read_requests<<",\"read_beats\":"<<t.cache_read_beats<<",\"write_AW\":"<<t.cache_write_requests<<",\"write_B\":"<<t.cache_write_responses<<"},";
  j<<"\"CPU_copy_frontend\":{\"source_load_ops\":"<<cpu.copy_load_ops<<",\"descriptor_count_peak\":"<<cpu.copy_descriptor_count_peak<<",\"descriptor_capacity_peak\":"<<cpu.copy_descriptor_capacity_peak<<",\"descriptor_storage_peak_bytes\":"<<cpu.copy_descriptor_storage_peak<<",\"read_descriptor_size_bytes\":"<<cpu.copy_read_descriptor_size<<",\"write_descriptor_size_bytes\":"<<cpu.copy_write_descriptor_size<<",\"pending_read_peak\":"<<cpu.copy_pending_reads_peak<<",\"dest_store_ops\":"<<cpu.copy_store_ops<<",\"peak_queued_words\":"<<cpu.copy_peak_queued_words<<",\"peak_queued_useful_bytes\":"<<cpu.copy_peak_queued_bytes<<",\"partial_word_bytes\":8,\"returned_word_bytes\":8,\"read_store_offer_cycles\":"<<cpu.copy_read_store_offer_cycles<<",\"read_store_grant_cycles\":"<<cpu.copy_read_store_grant_cycles<<",\"back_to_back_read_cycles\":"<<cpu.copy_back_to_back_read_cycles<<",\"host_descriptor_and_queue_allocation_cost\":null,\"ALU_shuffle_instruction_time\":null,\"full_CPU_instruction_time\":null},";
  j<<"\"counts\":{\"issued\":["<<t.finite_issued[0]<<","<<t.finite_issued[1]<<"],\"front\":["<<t.finite_front[0]<<","<<t.finite_front[1]<<"],\"backend\":["<<t.finite_accepted[0]<<","<<t.finite_accepted[1]<<"],\"response\":["<<t.finite_response[0]<<","<<t.finite_response[1]<<"],\"done\":["<<t.finite_done[0]<<","<<t.finite_done[1]<<"],\"ar\":"<<t.finite_ar<<",\"aw\":"<<t.finite_aw<<",\"b\":"<<t.finite_b<<",\"read_bus_bytes\":"<<t.finite_rbytes<<",\"write_useful_bytes\":"<<t.finite_wbytes<<",\"translation_accepted\":"<<t.n_tl_accept<<",\"translation_responded\":"<<t.n_tl_resp<<",\"DDTW_AR\":"<<t.n_walk_ddtw<<",\"PTW_AR\":"<<t.n_walk_ptw<<",\"IOTLB_lookup\":"<<t.n_iotlb_lookup<<",\"IOTLB_hit\":"<<t.n_iotlb_hit<<"},";
  j<<"\"protocol_diagnostic_cycles\":{\"common_ready\":"<<common<<",\"last_service_end\":"<<last_poll<<",\"global_quiet\":"<<quiet<<",\"release_quiet\":"<<release<<"},\"host_ns_execution_cost_only\":{\"construction\":"<<ns(begin,construction_end)<<",\"whole_main\":"<<ns(begin,end)<<"},\"limits\":{\"full_CPU_instruction_latency\":null,\"CPU_ALU_shuffle_latency\":null,\"full_software_latency\":null,\"application_latency\":null,\"board_latency\":null,\"physical_CPU_no_MMU\":true,\"Icache_inactive\":true,\"deterministic_optimistic_CPU_port_frontend\":true,\"old_microtests_replayed\":false,\"no_production_error_API_claim\":true},\"checked_teardown\":true}\n";CHECK(j.good(),"run JSON");std::printf("DELIVERY G0 PASS case=%s used=[%ld,%ld,%ld]\n",mode.c_str(),used[0]-common,used[1]-common,used[2]-common);return 0;
 }catch(const std::exception& e){t.finite_event("failure_retain");std::ofstream f(out+"/failure.json");f<<"{\"pass\":false,\"mapping_retained\":true,\"reason\":\""<<e.what()<<"\"}\n";close_traces(t);std::fprintf(stderr,"DELIVERY G0 FAIL %s: %s\n",mode.c_str(),e.what());return 1;}
}
