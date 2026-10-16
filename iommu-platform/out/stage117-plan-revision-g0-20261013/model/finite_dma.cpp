// Frozen C117R3 finite phase. All issuer decisions use public MMIO only.
#include "../../stage117-three-results-20261013/common/fixture.hpp"
#include <fstream>
#include <chrono>
#include <iostream>
#include <array>
#include "inherited_harness.hpp"
using Clock=std::chrono::steady_clock;
static long long ns(Clock::time_point a,Clock::time_point b){return std::chrono::duration_cast<std::chrono::nanoseconds>(b-a).count();}
static void dump(Tb& t,const std::string& path,uint64_t base,size_t n){std::ofstream f(path,std::ios::binary);for(size_t i=0;i<n;++i){char c=char(t.mem.at(base+i));f.write(&c,1);}CHECK(f.good(),"snapshot write");}
int main(int argc,char** argv) {
 Verilated::commandArgs(argc,argv);
 if(argc!=3){std::fprintf(stderr,"usage: finite_dma case output-directory\n");return 2;}
 const std::string mode=argv[1],out=argv[2];const auto begin=Clock::now();
 try {
  Tb t;t.cpu_enable=false;t.nonblock_enable=false;t.policy_partition=false;t.ptw_latency=1;
  t.a_pages=1;t.warmup=0;t.v_transfers=4;t.a_target=0;t.v_gap_cycles=0;
  if(mode=="solo0")t.finite_expected[0]={0,1};
  else if(mode=="solo1")t.finite_expected[1]={2,3};
  else if(mode=="single_FIFO_ahead")t.finite_expected[0]={0,1,2,3};
  else if(mode=="parallel_FIFO_ahead"){t.finite_expected[0]={0,1};t.finite_expected[1]={2,3};}
  else throw std::runtime_error("unknown frozen case");
  const bool whole=mode=="single_FIFO_ahead"||mode=="parallel_FIFO_ahead";
  const auto original=c117::initial_memory();const auto spans=c117::spans();
  t.mem.clear();for(uint64_t base:{0x10000ULL,0x20000ULL,0x40000ULL})for(uint64_t i=0;i<4096;++i)t.mem[base+i]=0;
  for(uint64_t entry:{0x10280ULL,0x102c0ULL}){t.write64(entry,1);t.write64(entry+8,0);t.write64(entry+16,0xa000);t.write64(entry+24,0x8000000000000020ULL);}
  t.write64(0x20000,0x10001);t.write64(0x40400,((Tb::FINITE_PA>>12)<<10)|0xdfULL);
  for(size_t i=0;i<original.size();++i)t.mem[Tb::FINITE_PA+i]=original[i];
  const auto construct_end=Clock::now();
  dump(t,out+"/initial.bin",Tb::FINITE_PA,original.size());
  for(uint64_t base:{0x10000ULL,0x20000ULL,0x40000ULL})dump(t,out+"/table_initial_"+std::to_string(base)+".bin",base,4096);
  t.reset(false);t.check_quiet("pre-issue");
  t.finite_trace=std::fopen((out+"/events.csv").c_str(),"w");CHECK(t.finite_trace,"trace open");
  std::fprintf(t.finite_trace,"cycle,event,engine,span,address,n,id,value\n");t.finite_track=true;
  const auto execution_begin=Clock::now();const long t0=t.g_cycle;t.finite_event("common_ready");
  // The source graph and mapping are already reserved; only active engines pay mode setup.
  for(int e=0;e<2;++e)if(!t.finite_expected[e].empty()){
    t.finite_cfg_span=-1;t.cfg_wr(e,REG_B_CTL,0);t.cfg_wr(e,REG_CONF,0);t.cfg_wr(e,0x48,1);
  }
  std::array<size_t,2> pos{};std::array<uint64_t,2> current_id{};
  auto prepare=[&](int e,int index){const auto& s=spans.at(index);t.finite_cfg_span=index;t.finite_event("prepare_begin",e,index);t.cfg_wr(e,REG_SRC,Tb::FINITE_IOVA+s.src);t.cfg_wr(e,REG_DST,Tb::FINITE_IOVA+s.dst);t.cfg_wr(e,REG_NB,s.length);t.finite_event("prepare_end",e,index);};
  auto launch=[&](int e){int index=t.finite_expected[e][pos[e]];t.finite_cfg_span=index;uint64_t id=t.trigger(e);CHECK(id && (!current_id[e] || id==current_id[e]+1),"NEXT ID not sequential");current_id[e]=id;t.finite_event("next_returned",e,index,0,0,id);};
  // First launch each independent group, then prepare successors. No debug feedback.
  for(int e=0;e<2;++e)if(!t.finite_expected[e].empty()){prepare(e,t.finite_expected[e][0]);launch(e);}
  for(int e=0;e<2;++e)if(t.finite_expected[e].size()>1)prepare(e,t.finite_expected[e][1]);
  bool running=true;
  while(running){
    running=false;
    for(int e=0;e<2;++e)if(pos[e]<t.finite_expected[e].size()){
      running=true;CHECK(t.g_cycle-t0<200000,"finite public-poll timeout");
      int index=t.finite_expected[e][pos[e]];t.finite_cfg_span=index;uint64_t done=t.cfg_rd(e,REG_DONE);
      t.finite_event("done_poll_return",e,index,0,0,current_id[e],done);
      if(done==current_id[e]){
        t.finite_event("done_register_matched",e,index,0,0,done);++pos[e];
        if(pos[e]<t.finite_expected[e].size()){
          launch(e);
          if(pos[e]+1<t.finite_expected[e].size())prepare(e,t.finite_expected[e][pos[e]+1]);
        }
      }
    }
  }
  const long last_poll=t.g_cycle;t.check_quiet("terminal");const long quiet=t.g_cycle;t.finite_event("global_quiet");
  const auto execution_end=Clock::now();const auto observer_begin=Clock::now();
  uint64_t expected=0;
  for(int e=0;e<2;++e){size_t n=t.finite_expected[e].size();CHECK(t.finite_issued[e]==n && t.finite_front[e]==n && t.finite_accepted[e]==n && t.finite_done[e]==n && t.finite_response[e]==n,"request/response/ID conservation");for(int k:t.finite_expected[e]){const auto& s=spans[k];expected+=s.length;for(unsigned j=0;j<s.length;++j){CHECK(t.finite_reads[s.src+j]>=1,"missing useful source R");CHECK(t.finite_writes[s.dst+j]==1,"missing or duplicated useful W");}}}
  CHECK(t.finite_arbytes==t.finite_rbytes && t.finite_wbytes==expected && t.finite_aw==t.finite_b && t.finite_awq.empty(),"payload bus conservation");for(const auto& q:t.finite_bq)CHECK(q.empty(),"pending B");
  CHECK(t.n_tl_accept==t.n_tl_resp && t.n_tl_accept>0 && t.n_fast==0,"ordinary translation conservation");
  for(unsigned i=0;i<c117::kSourceEnd;++i)CHECK(t.mem.at(Tb::FINITE_PA+i)==original[i],"source changed before release");
  dump(t,out+"/before_publish.bin",Tb::FINITE_PA,original.size());
  for(uint64_t base:{0x10000ULL,0x20000ULL,0x40000ULL})dump(t,out+"/table_final_"+std::to_string(base)+".bin",base,4096);
  std::vector<uint8_t> image(original.size());for(size_t i=0;i<image.size();++i)image[i]=t.mem.at(Tb::FINITE_PA+i);
  std::vector<c117::Used> used;
  if(whole){used=c117::publish_after_quiet(image);for(const auto& p:c117::packets())for(unsigned j=0;j<c117::kHeaderBytes;++j)t.mem.at(Tb::FINITE_PA+p.destination.front().offset+j)=image[p.destination.front().offset+j];for(size_t i=0;i<used.size();++i)t.finite_event("used_publish",-1,int(i),used[i].id,used[i].length);}
  else t.finite_event("solo_diagnostic_no_publication");
  // Reuse only each active source extent. Inactive groups and neighbor guards stay retained.
  for(int e=0;e<2;++e)for(int k:t.finite_expected[e]){const auto& s=spans[k];for(unsigned j=0;j<s.length;++j)t.mem.at(Tb::FINITE_PA+s.src+j)=uint8_t((s.src+j)*37+9);t.finite_event("source_reused",e,k,s.src,s.length);}
  for(size_t i=0;i<image.size();++i)image[i]=t.mem.at(Tb::FINITE_PA+i);
  const auto consume_begin=Clock::now();c117::CheckResult checked{};
  if(whole){checked=c117::consume(image,used);CHECK(checked.ok,checked.reason.c_str());t.finite_event("consumer_completed",-1,-1,0,checked.frame_bytes);}
  else {for(int e=0;e<2;++e)for(int k:t.finite_expected[e]){const auto& s=spans[k];for(unsigned j=0;j<s.length;++j)CHECK(image[s.dst+j]==original[s.src+j],"solo B corrupted after source reuse");}t.finite_event("diagnostic_buffer_check_complete",-1,-1,0,expected);}
  const auto consume_end=Clock::now();
  dump(t,out+"/after_reuse.bin",Tb::FINITE_PA,original.size());
  t.check_quiet("release");const long release=t.g_cycle;
  t.finite_event(whole?"B_return_release":"diagnostic_release");
  t.finite_event("mapping_teardown");std::fclose(t.finite_trace);t.finite_trace=nullptr;t.mem.clear();const auto end=Clock::now();
  std::ofstream j(out+"/run.json");
  j<<"{\n\"pass\":true,\"case\":\""<<mode<<"\",\"useful_bytes\":"<<expected<<",\"whole_application_logical_delivery\":"<<(whole?"true":"false")<<",\"application_RTL_latency\":null,";
  j<<"\"counts\":{\"issued\":["<<t.finite_issued[0]<<","<<t.finite_issued[1]<<"],\"front\":["<<t.finite_front[0]<<","<<t.finite_front[1]<<"],\"backend\":["<<t.finite_accepted[0]<<","<<t.finite_accepted[1]<<"],\"response\":["<<t.finite_response[0]<<","<<t.finite_response[1]<<"],\"done\":["<<t.finite_done[0]<<","<<t.finite_done[1]<<"],\"ar\":"<<t.finite_ar<<",\"aw\":"<<t.finite_aw<<",\"b\":"<<t.finite_b<<",\"read_bus_bytes\":"<<t.finite_rbytes<<",\"write_useful_bytes\":"<<t.finite_wbytes<<",\"translation_accepted\":"<<t.n_tl_accept<<",\"translation_responded\":"<<t.n_tl_resp<<",\"DDTW_AR\":"<<t.n_walk_ddtw<<",\"PTW_AR\":"<<t.n_walk_ptw<<",\"IOTLB_lookup\":"<<t.n_iotlb_lookup<<",\"IOTLB_hit\":"<<t.n_iotlb_hit<<",\"fast\":"<<t.n_fast<<"},";
  j<<"\"cycles\":{\"common_ready\":"<<t0<<",\"last_done_poll\":"<<last_poll<<",\"global_quiet\":"<<quiet<<",\"release\":"<<release<<",\"ready_to_quiet\":"<<quiet-t0<<"},";
  j<<"\"host_ns_instrumentation_only\":{\"construction\":"<<ns(begin,construct_end)<<",\"execution_with_observers\":"<<ns(execution_begin,execution_end)<<",\"postquiet_observation_publish_reuse_consume_teardown\":"<<ns(observer_begin,end)<<",\"consumer_or_diagnostic_checker\":"<<ns(consume_begin,consume_end)<<",\"main_through_teardown\":"<<ns(begin,end)<<"},\"remaining_queues\":0}\n";
  CHECK(j.good(),"JSON output");std::printf("G0 PASS case=%s bytes=%llu ready_to_quiet=%ld\n",mode.c_str(),(unsigned long long)expected,quiet-t0);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"G0 FAIL %s: %s\n",mode.c_str(),e.what());std::ofstream f(out+"/failure.txt");f<<e.what()<<"\n";return 1;}
}
