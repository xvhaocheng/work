#include "../common/fixture.hpp"
#include <array>
#include <atomic>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
using Clock=std::chrono::steady_clock;
using Ns=std::chrono::nanoseconds;
using c117::Span; using c117::Used;
static uint64_t ns(Clock::time_point a,Clock::time_point b){return std::chrono::duration_cast<Ns>(b-a).count();}
struct Ledger {uint64_t submitted=0,accepted=0,completed=0,rejected=0,read_bytes=0,write_bytes=0;};
struct FrameView {uint32_t cookie,packet_id,length,span_count;std::array<c117::Region,2> payload;};
using Received=std::array<FrameView,3>;
struct Adapter {
  enum State:uint8_t{Empty,Submitted,Accepted,Complete};
  enum BState:uint8_t{Posted,Published,HandedOff,Consumed,Released};
  std::array<Span,4> request{};
  std::array<State,4> state{};
  std::vector<Used> used;
  std::atomic<uint32_t> used_count{0};
  std::array<bool,3> returned{};
  Ledger ledger;
  BState b=Posted;
  bool source_owned=true,source_retired=false;
  Adapter(){used.reserve(3);}
  void submit(){
    const auto v=c117::spans();
    if(v.size()!=request.size())throw std::runtime_error("wrong segment count");
    std::copy(v.begin(),v.end(),request.begin());
    for(size_t i=0;i<request.size();++i){
      const auto& s=request[i];
      if(s.src+s.length>c117::kSourceEnd || s.dst<c117::kSourceEnd || s.dst+s.length>c117::kMemoryBytes)
        throw std::runtime_error("request bounds");
      state[i]=Submitted;++ledger.submitted;
    }
  }
  void execute_one(std::vector<uint8_t>& m,size_t i){
    if(i>=state.size() || state[i]!=Submitted || !source_owned || b!=Posted)throw std::runtime_error("invalid execution ownership");
    state[i]=Accepted;++ledger.accepted;
    const auto& s=request[i];
    std::memcpy(m.data()+s.dst,m.data()+s.src,s.length);
    ledger.read_bytes+=s.length;ledger.write_bytes+=s.length;
    state[i]=Complete;++ledger.completed;
  }
  bool quiet()const{
    if(ledger.submitted!=request.size() || ledger.accepted!=ledger.submitted || ledger.completed!=ledger.accepted)return false;
    for(auto s:state)if(s!=Complete)return false;
    return true;
  }
  void retire_source(){if(!quiet())throw std::runtime_error("source still has pending reads");source_retired=true;source_owned=false;}
  void reuse_source(std::vector<uint8_t>& m){
    if(!source_retired)throw std::runtime_error("early source reuse rejected");
    for(const auto& p:c117::packets())for(const auto& s:p.source)std::memset(m.data()+s.offset,0xcc,s.length);
  }
  void publish(std::vector<uint8_t>& m){
    if(!quiet() || b!=Posted)throw std::runtime_error("early or duplicate used rejected");
    used=c117::publish_after_quiet(m);
    used_count.store(used.size(),std::memory_order_release);b=Published;
  }
  // Guest receive ownership: resolve original IDs and complete lengths; return
  // views of the posted buffers. Full byte/guard validation is a separate observer.
  Received receive(){
    if(b!=Published || used_count.load(std::memory_order_acquire)!=c117::packets().size())throw std::runtime_error("used not published");
    Received views{};
    for(size_t i=0;i<used.size();++i){
      const auto& p=c117::packets()[i];const auto& u=used[i];
      if(u.id!=p.cookie_id || u.length!=c117::kHeaderBytes+p.length)throw std::runtime_error("bad receive metadata");
      auto& v=views[i];v.cookie=u.id;v.packet_id=p.packet_id;v.length=p.length;
      uint32_t left=p.length,skip=c117::kHeaderBytes;
      for(const auto& d:p.destination){
        if(!left)break;
        uint32_t n=std::min(left,d.capacity-skip);
        if(v.span_count>=v.payload.size())throw std::runtime_error("view span capacity");
        v.payload[v.span_count++]={d.offset+skip,n};left-=n;skip=0;
      }
      if(left)throw std::runtime_error("incomplete consumer view");
    }
    b=HandedOff;return views;
  }
  void consumer_complete(uint32_t cookie){
    if(b!=HandedOff)throw std::runtime_error("consumer does not own buffers");
    size_t i=0;for(;i<c117::packets().size();++i)if(c117::packets()[i].cookie_id==cookie)break;
    if(i==returned.size() || returned[i])throw std::runtime_error("wrong or duplicate consumer return");
    returned[i]=true;if(std::all_of(returned.begin(),returned.end(),[](bool x){return x;}))b=Consumed;
  }
  void release(){if(b!=Consumed || !source_retired || !quiet())throw std::runtime_error("release before consumption/retirement");b=Released;}
};
static bool source_unchanged(const std::vector<uint8_t>& m){
  std::array<uint8_t,c117::kSourceEnd> expected;expected.fill(0x5a);
  for(const auto& p:c117::packets()){
    size_t done=0;for(const auto& s:p.source){std::copy_n(p.frame.begin()+done,s.length,expected.begin()+s.offset);done+=s.length;}
  }
  return std::equal(expected.begin(),expected.end(),m.begin());
}
static bool views_match_original_frames(const std::vector<uint8_t>& m,const Received& views){
  for(size_t i=0;i<views.size();++i){
    const auto& v=views[i];const auto& p=c117::packets()[i];size_t done=0;
    if(v.cookie!=p.cookie_id || v.packet_id!=p.packet_id || v.length!=p.length)return false;
    for(uint32_t j=0;j<v.span_count;++j){const auto& s=v.payload[j];
      if(s.offset<c117::kSourceEnd || s.offset+s.length>m.size() || done+s.length>p.frame.size())return false;
      if(!std::equal(m.begin()+s.offset,m.begin()+s.offset+s.length,p.frame.begin()+done))return false;
      done+=s.length;
    }
    if(done!=p.length)return false;
  }
  return true;
}
struct Phase {const char* name;uint64_t elapsed;bool observer;};
struct Measurement {
  std::vector<Phase> phases;Ledger ledger; c117::CheckResult checked{};
  uint64_t total=0,source_work=0,observer=0,unattributed=0,memory_capacity=0,used_capacity=0,used_final_capacity=0,consumer_bytes=0;
};
static Measurement baseline(){
  Measurement r;r.phases.reserve(13);std::vector<uint8_t> m;std::unique_ptr<Adapter> a;Received views{};
  auto phase=[&](const char* name,bool observer,auto fn){auto t=Clock::now();fn();auto dt=ns(t,Clock::now());r.phases.push_back({name,dt,observer});if(observer)r.observer+=dt;else r.source_work+=dt;};
  auto start=Clock::now();
  phase("allocate_and_value_initialize",false,[&]{m.resize(c117::kMemoryBytes);a=std::make_unique<Adapter>();r.memory_capacity=m.capacity();r.used_capacity=a->used.capacity();});
  phase("validation_guard_initialization",true,[&]{std::fill(m.begin(),m.end(),0xa5);std::fill(m.begin(),m.begin()+c117::kSourceEnd,0x5a);});
  phase("prepare_source_frames",false,[&]{for(const auto& p:c117::packets()){size_t done=0;for(const auto& s:p.source){std::copy_n(p.frame.begin()+done,s.length,m.begin()+s.offset);done+=s.length;}}});
  phase("prepare_exact_descriptors",false,[&]{a->submit();});
  phase("direct_cpu_copy_and_synchronous_completion",false,[&]{for(size_t i=0;i<a->request.size();++i)a->execute_one(m,i);});
  phase("source_and_source_guard_before_retirement_observer",true,[&]{if(!source_unchanged(m))throw std::runtime_error("source/neighbor modified before retirement");});
  phase("retire_source_reads",false,[&]{a->retire_source();});
  phase("header_and_used_release_publication",false,[&]{a->publish(m);r.used_final_capacity=a->used.capacity();});
  phase("guest_cookie_resolution_and_consumer_handoff",false,[&]{views=a->receive();for(const auto& v:views)r.consumer_bytes+=v.length;});
  phase("full_frame_header_guard_observer",true,[&]{if(!views_match_original_frames(m,views))throw std::runtime_error("returned view does not expose full original frame");r.checked=c117::consume(m,a->used);if(!r.checked.ok)throw std::runtime_error(r.checked.reason);});
  phase("source_reuse_and_independence_observer",true,[&]{a->reuse_source(m);auto c=c117::consume(m,a->used);if(!c.ok)throw std::runtime_error(c.reason);});
  phase("consumer_returns_original_buffers",false,[&]{for(const auto& v:views)a->consumer_complete(v.cookie);});
  phase("checked_release_and_deallocate",false,[&]{a->release();r.ledger=a->ledger;a.reset();std::vector<uint8_t>().swap(m);});
  r.total=ns(start,Clock::now());r.unattributed=r.total-r.source_work-r.observer;
  return r;
}
struct Test {std::string name;bool pass;std::string detected;};
static std::vector<Test> functional_tests(){
  std::vector<Test> out;
  auto throws=[&](std::string name,auto fn){try{fn();out.push_back({name,false,"not rejected"});}catch(const std::exception& e){out.push_back({name,true,e.what()});}};
  throws("used_before_any_copy",[]{auto m=c117::initial_memory();Adapter a;a.submit();a.publish(m);});
  throws("used_before_final_span",[]{auto m=c117::initial_memory();Adapter a;a.submit();for(size_t i=0;i<3;++i)a.execute_one(m,i);a.publish(m);});
  throws("source_reuse_before_reads_finish",[]{auto m=c117::initial_memory();Adapter a;a.submit();a.execute_one(m,0);a.reuse_source(m);});
  throws("buffer_release_before_consumer",[]{auto m=c117::initial_memory();Adapter a;a.submit();for(size_t i=0;i<4;++i)a.execute_one(m,i);a.retire_source();a.publish(m);a.release();});
  throws("buffer_release_after_handoff_before_return",[]{auto m=c117::initial_memory();Adapter a;a.submit();for(size_t i=0;i<4;++i)a.execute_one(m,i);a.retire_source();a.publish(m);a.receive();a.release();});
  {auto m=c117::initial_memory();Adapter a;a.submit();for(size_t i=0;i<4;++i)a.execute_one(m,i);a.retire_source();a.publish(m);auto v=a.receive();v[2].payload[1].offset+=1;out.push_back({"wrong_consumer_view_address",!views_match_original_frames(m,v),"returned view must expose complete original frame"});}
  auto checker_rejects=[&](std::string name,auto corrupt){auto m=c117::initial_memory();Adapter a;a.submit();for(size_t i=0;i<4;++i)a.execute_one(m,i);a.retire_source();a.publish(m);corrupt(m,a);auto c=c117::consume(m,a.used);out.push_back({name,!c.ok,c.reason});};
  checker_rejects("wrong_original_cookie",[](auto&,auto& a){a.used[0].id=999;});
  checker_rejects("wrong_used_length",[](auto&,auto& a){--a.used[2].length;});
  checker_rejects("out_of_source_order",[](auto&,auto& a){std::swap(a.used[0],a.used[1]);});
  checker_rejects("wrong_virtio_header",[](auto& m,auto&){m[c117::packets()[0].destination[0].offset+10]=2;});
  checker_rejects("wrong_frame_tail",[](auto& m,auto&){m[12055+1011-1]^=1;});
  checker_rejects("neighbor_overwrite",[](auto& m,auto&){m[12055+1011]^=1;});
  // Bypass the adapter deliberately: completion bookkeeping without actual
  // stores must fail the independent consumer, even if the used metadata looks valid.
  {auto m=c117::initial_memory();auto used=c117::publish_after_quiet(m);auto c=c117::consume(m,used);out.push_back({"false_done_or_pointer_only_no_copy",!c.ok,c.reason});}
  // Simulate unauthorized writer changing a source before the copy. The state
  // machine cannot prevent arbitrary CPU stores; content validation detects damage.
  {auto m=c117::initial_memory();Adapter a;a.submit();m[131]^=1;for(size_t i=0;i<4;++i)a.execute_one(m,i);a.retire_source();a.publish(m);auto c=c117::consume(m,a.used);out.push_back({"out_of_contract_early_source_mutation",!c.ok,c.reason});}
  {auto m=c117::initial_memory();m[8191]^=1;out.push_back({"source_neighbor_overwrite",!source_unchanged(m),"source-region original bytes and guards"});}
  {auto m=c117::initial_memory();Adapter a;a.submit();for(size_t i=0;i<4;++i)a.execute_one(m,i);a.retire_source();a.publish(m);a.reuse_source(m);auto c=c117::consume(m,a.used);auto views=a.receive();for(const auto& v:views)a.consumer_complete(v.cookie);a.release();out.push_back({"source_reuse_after_read_retirement",c.ok,c.reason});}
  return out;
}
int main(){try{
  // Materialize immutable fixture metadata before timing; it is experiment input.
  c117::packets();c117::spans();
  auto r=baseline();auto tests=functional_tests();bool pass=r.checked.ok;
  std::cout<<"{\n  \"fixture_sha256\":\""<<c117::kFixtureSha256<<"\",\n  \"kind\":\"one finite Dell CPU execution; generated functional input; no native application performance claim\",\n  \"frame_bytes\":"<<r.checked.frame_bytes<<",\n  \"record_bytes\":"<<r.checked.record_bytes<<",\n  \"guard_bytes_per_check\":"<<r.checked.guard_bytes<<",\n  \"frame_digest_fnv1a64\":"<<r.checked.digest<<",\n  \"consumer_bytes\":"<<r.consumer_bytes<<",\n  \"ledger\":{\"submitted\":"<<r.ledger.submitted<<",\"accepted\":"<<r.ledger.accepted<<",\"completed\":"<<r.ledger.completed<<",\"rejected\":"<<r.ledger.rejected<<",\"payload_read_bytes\":"<<r.ledger.read_bytes<<",\"payload_write_bytes\":"<<r.ledger.write_bytes<<"},\n";
  std::cout<<"  \"resources\":{\"memory_size\":"<<c117::kMemoryBytes<<",\"memory_capacity\":"<<r.memory_capacity<<",\"adapter_sizeof\":"<<sizeof(Adapter)<<",\"used_vector_capacity_elements\":"<<r.used_capacity<<",\"used_vector_final_capacity_elements\":"<<r.used_final_capacity<<",\"used_element_sizeof\":"<<sizeof(Used)<<",\"descriptor_source_span_sizeof\":"<<sizeof(Span)<<",\"staging_bytes\":0,\"cpu_iommu_mapping_bytes\":0},\n";
  uint64_t frame_capacity=0,source_capacity=0,destination_capacity=0;
  for(const auto& p:c117::packets()){frame_capacity+=p.frame.capacity()*sizeof(uint8_t);source_capacity+=p.source.capacity()*sizeof(c117::Region);destination_capacity+=p.destination.capacity()*sizeof(c117::Descriptor);}
  std::cout<<"  \"fixture_and_observer_resources\":{\"fixture_packet_vector_capacity_bytes\":"<<c117::packets().capacity()*sizeof(c117::Packet)<<",\"fixture_frame_capacity_bytes\":"<<frame_capacity<<",\"fixture_source_metadata_capacity_bytes\":"<<source_capacity<<",\"fixture_destination_metadata_capacity_bytes\":"<<destination_capacity<<",\"temporary_supply_span_capacity_bytes\":"<<c117::spans().capacity()*sizeof(Span)<<",\"consumer_views_stack_bytes\":"<<sizeof(Received)<<",\"checker_touched_stack_bytes\":"<<c117::kMemoryBytes*sizeof(bool)<<",\"source_expected_stack_bytes\":"<<c117::kSourceEnd<<",\"timing_phase_vector_capacity_bytes\":"<<r.phases.capacity()*sizeof(Phase)<<",\"allocator_metadata_and_rounding\":null,\"process_RSS\":null,\"checker_unordered_set_allocations\":\"dynamic; exact allocator overhead not measured\"},\n";
  std::cout<<"  \"timing_ns\":{\"total_observed_lifecycle\":"<<r.total<<",\"sum_source_delivery_phases\":"<<r.source_work<<",\"sum_validation_observer_phases\":"<<r.observer<<",\"clock_and_phase_bookkeeping_unattributed\":"<<r.unattributed<<",\"phases\":[\n";
  for(size_t i=0;i<r.phases.size();++i){auto p=r.phases[i];std::cout<<"    {\"name\":\""<<p.name<<"\",\"elapsed\":"<<p.elapsed<<",\"observer\":"<<(p.observer?"true":"false")<<"}"<<(i+1==r.phases.size()?"\n":",\n");}
  std::cout<<"  ]},\n  \"functional_tests\":[\n";
  for(size_t i=0;i<tests.size();++i){auto t=tests[i];pass&=t.pass;std::cout<<"    {\"name\":\""<<t.name<<"\",\"pass\":"<<(t.pass?"true":"false")<<",\"detected\":\""<<t.detected<<"\"}"<<(i+1==tests.size()?"\n":",\n");}
  std::cout<<"  ],\n  \"passed\":"<<(pass?"true":"false")<<"\n}\n";return pass?0:1;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 2;}}
