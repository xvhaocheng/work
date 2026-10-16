// C117R3 finite source-derived packet projection. Real iDMA and IOMMU execute every payload copy.
#include "../common/fixture.hpp"
#include <fstream>
#include <chrono>
#include <iostream>
#include <array>
#include "inherited_harness.hpp"
using Clock=std::chrono::steady_clock;
static long long ns(Clock::time_point a,Clock::time_point b){return std::chrono::duration_cast<std::chrono::nanoseconds>(b-a).count();}
int main(int argc,char** argv) {
 Verilated::commandArgs(argc,argv);
 if(argc!=2){std::fprintf(stderr,"usage: finite_dma output-directory\n");return 2;}
 const std::string out=argv[1];const auto begin=Clock::now();
 try {
  Tb t;t.cpu_enable=false;t.nonblock_enable=false;t.policy_partition=false;t.ptw_latency=1;
  t.a_pages=1;t.warmup=0;t.v_transfers=4;t.a_target=0;t.v_gap_cycles=0;
  const auto original=c117::initial_memory();
  // One owner DID10, extended DDT, S-stage Sv39 with a legal 2MiB leaf.
  // IOVA 0x10000000 -> PA 0x80000000; neither endpoint bypasses translation.
  t.mem.clear();for(uint64_t p=0x10000;p<0x11000;++p)t.mem[p]=0;
  for(uint64_t p=0x20000;p<0x21000;++p)t.mem[p]=0;
  for(uint64_t p=0x40000;p<0x41000;++p)t.mem[p]=0;
  t.write64(0x10280,1);t.write64(0x10288,0);t.write64(0x10290,0xA000);t.write64(0x10298,0x8000000000000020ULL);
  t.write64(0x20000,0x10001);t.write64(0x40400,((Tb::FINITE_PA>>12)<<10)|0xDFULL);
  for(size_t i=0;i<original.size();++i)t.mem[Tb::FINITE_PA+i]=original[i];
  t.reset(false);t.check_quiet("pre-issue");
  t.finite_trace=std::fopen((out+"/events.csv").c_str(),"w");CHECK(t.finite_trace,"trace open");
  std::fprintf(t.finite_trace,"cycle,event,address,length_or_strobe,id\n");
  t.finite_track=true;const auto execute_start=Clock::now();const long cycle_start=t.g_cycle;
  t.finite_event("source_retain_and_B_preposted",0,original.size());
  t.cfg_wr(SEL_V,REG_B_CTL,0);t.cfg_wr(SEL_V,REG_CONF,0);t.cfg_wr(SEL_V,0x48,1);
  CHECK(t.cfg_rd(SEL_V,REG_B_CTL)==0,"batch not disabled");CHECK(t.cfg_rd(SEL_V,REG_CONF)==0,"not DEFAULT config");CHECK(t.cfg_rd(SEL_V,0x48)==1,"N_TRANS not 1");
  std::vector<uint64_t> ids;uint64_t expected_bytes=0;
  for(const auto& s:c117::spans()) {
    t.cfg_wr(SEL_V,REG_SRC,Tb::FINITE_IOVA+s.src);t.cfg_wr(SEL_V,REG_DST,Tb::FINITE_IOVA+s.dst);t.cfg_wr(SEL_V,REG_NB,s.length);
    const uint64_t id=t.trigger(SEL_V);CHECK(id && (ids.empty() || id==ids.back()+1),"NEXT IDs not sequential");ids.push_back(id);
    t.finite_event("next_returned",s.dst,s.length,id);
    t.wait_done(SEL_V,id,"finite exact-span DONE");t.finite_event("done_register_matched",0,0,id);expected_bytes+=s.length;
  }
  const long last_done_poll=t.g_cycle;t.check_quiet("terminal");const long quiet_cycle=t.g_cycle;const auto quiet_time=Clock::now();
  CHECK(t.finite_issued==4 && t.finite_front==4 && t.finite_accepted==4 && t.finite_done==4,"submitted/accepted/completed conservation");
  CHECK(t.finite_ar>0 && t.finite_aw>0 && t.finite_b==t.finite_aw && t.finite_awq.empty(),"AW/B conservation");
  CHECK(t.finite_read_req_bytes==t.finite_read_bus_bytes,"AR/R bus byte conservation");
  CHECK(t.finite_write_bytes==expected_bytes && expected_bytes==1639,"actual useful write byte conservation");
  CHECK(t.n_tl_accept==t.n_tl_resp && t.n_tl_accept>0,"translation request/response conservation");
  CHECK(t.n_walk_ddtw>0 && t.n_walk_ptw>0 && t.n_iotlb_hit>0,"actual walks and resident translation missing");
  CHECK(t.n_fast==0,"nonblocking fast path accidentally active");
  for(const auto& s:c117::spans()) for(unsigned j=0;j<s.length;++j) {
    CHECK(t.finite_reads[s.src+j]>=1,"source useful byte was not read by DMA");CHECK(t.finite_writes[s.dst+j]==1,"destination useful byte not written exactly once");
  }
  for(unsigned i=0;i<c117::kSourceEnd;++i) CHECK(t.mem.at(Tb::FINITE_PA+i)==original[i],"source/neighbor changed before quiet");
  t.finite_event("global_quiet",0,expected_bytes);const auto checker_start=Clock::now();
  std::vector<uint8_t> image(original.size());for(size_t i=0;i<image.size();++i)image[i]=t.mem.at(Tb::FINITE_PA+i);
  {std::ofstream f(out+"/before_publish.bin",std::ios::binary);f.write(reinterpret_cast<const char*>(image.data()),image.size());}
  const auto publish_start=Clock::now();const auto used=c117::publish_after_quiet(image);
  // Only the 12-byte software virtio headers are written after ownership handoff.
  for(const auto& p:c117::packets())for(unsigned j=0;j<c117::kHeaderBytes;++j)t.mem.at(Tb::FINITE_PA+p.destination.front().offset+j)=image[p.destination.front().offset+j];
  t.finite_event("used_publish",0,used.size());
  // A may now be reused; B is still retained through consumption. This is not a payload copy.
  for(unsigned i=0;i<c117::kSourceEnd;++i)t.mem.at(Tb::FINITE_PA+i)=uint8_t(i*37+9);
  for(size_t i=0;i<image.size();++i)image[i]=t.mem.at(Tb::FINITE_PA+i);
  const auto consume_start=Clock::now();const auto checked=c117::consume(image,used);const auto consume_end=Clock::now();CHECK(checked.ok,checked.reason.c_str());
  t.finite_event("consumer_completed",0,checked.frame_bytes);t.check_quiet("before-release");
  CHECK(t.mem_q.empty()&&t.caw_q.empty()&&t.cb_q.empty()&&t.finite_awq.empty(),"release with pending memory work");
  const long release_cycle=t.g_cycle;t.finite_event("B_release_and_mapping_teardown",0,used.size());
  std::fclose(t.finite_trace);t.finite_trace=nullptr;
  {std::ofstream f(out+"/consumed.bin",std::ios::binary);f.write(reinterpret_cast<const char*>(image.data()),image.size());}
  // Final software teardown occurs only after quiet and consumer completion.
  t.mem.clear();const auto end=Clock::now();
  std::ofstream j(out+"/run.json");
  j<<"{\n  \"pass\":true,\n  \"fixture_kind\":\"generated finite functionality; not native DPDK or board performance\",\n  \"useful_frame_bytes\":"<<checked.frame_bytes<<",\n  \"consumer_record_bytes\":"<<checked.record_bytes<<",\n  \"consumer_guard_bytes\":"<<checked.guard_bytes<<",\n  \"consumer_digest\":"<<checked.digest<<",\n  \"spans\":4,\n  \"submitted\":"<<ids.size()<<",\n  \"issue_pulses\":"<<t.finite_issued<<",\n  \"frontend_accepted\":"<<t.finite_front<<",\n  \"backend_accepted\":"<<t.finite_accepted<<",\n  \"completed\":"<<t.finite_done<<",\n  \"rejected\":0,\n  \"timeouts\":0,\n  \"errors\":0,\n  \"payload_ar\":"<<t.finite_ar<<",\n  \"payload_r_beats\":"<<t.finite_read_beats<<",\n  \"payload_read_bus_bytes\":"<<t.finite_read_bus_bytes<<",\n  \"payload_aw\":"<<t.finite_aw<<",\n  \"payload_w_beats\":"<<t.finite_write_beats<<",\n  \"payload_write_strobe_bytes\":"<<t.finite_write_bytes<<",\n  \"payload_b\":"<<t.finite_b<<",\n  \"ds_ar\":"<<t.finite_ds_ar<<",\n  \"ds_r_beats\":"<<t.finite_ds_r<<",\n  \"ddtw_ar\":"<<t.n_walk_ddtw<<",\n  \"ptw_ar\":"<<t.n_walk_ptw<<",\n  \"translation_accepted\":"<<t.n_tl_accept<<",\n  \"translation_responded\":"<<t.n_tl_resp<<",\n  \"iotlb_lookups\":"<<t.n_iotlb_lookup<<",\n  \"iotlb_hit_observations\":"<<t.n_iotlb_hit<<",\n  \"nonblocking_fast_responses\":"<<t.n_fast<<",\n  \"cycles\":{\"before_config\":"<<cycle_start<<",\"last_done_poll\":"<<last_done_poll<<",\"quiet\":"<<quiet_cycle<<",\"release\":"<<release_cycle<<",\"config_to_quiet\":"<<quiet_cycle-cycle_start<<"},\n  \"host_ns\":{\"total_to_teardown\":"<<ns(begin,end)<<",\"rtl_execution_and_observation\":"<<ns(execute_start,quiet_time)<<",\"snapshot_trace_publish_consume_to_teardown\":"<<ns(checker_start,end)<<",\"publish_source_reuse_snapshot\":"<<ns(publish_start,consume_start)<<",\"consumer_every_byte_checker\":"<<ns(consume_start,consume_end)<<"},\n  \"mapping\":{\"type\":\"Sv39 nonidentity single 2MiB leaf; resident after first walk; no flush\",\"iova\":268435456,\"physical\":2147483648,\"leaf_bytes\":2097152,\"logical_allocated_bytes\":16384,\"table_allocated_bytes\":12288},\n  \"source_immutable_until_quiet\":true,\n  \"source_reused_before_B_consume\":true,\n  \"B_released_after_consume_and_quiet\":true,\n  \"remaining_queues\":0,\n  \"speedup_claim\":null\n}\n";
  CHECK(j.good(),"result output error");
  std::printf("C117R3-DMA PASS frames=3 bytes=%llu spans=4 accepted=%llu completed=%llu AR=%llu Rbytes=%llu AW=%llu Wstrobes=%llu B=%llu cycles_config_to_quiet=%ld\n",(unsigned long long)checked.frame_bytes,(unsigned long long)t.finite_accepted,(unsigned long long)t.finite_done,(unsigned long long)t.finite_ar,(unsigned long long)t.finite_read_bus_bytes,(unsigned long long)t.finite_aw,(unsigned long long)t.finite_write_bytes,(unsigned long long)t.finite_b,quiet_cycle-cycle_start);
  return 0;
 } catch(const std::exception& e) {
  std::fprintf(stderr,"C117R3-DMA FAIL; retain/quarantine all endpoint ownership; no used publication unless already safe: %s\n",e.what());
  std::ofstream f(out+"/failure.txt");f<<e.what()<<"\n";return 1;
 }
}
