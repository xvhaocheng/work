#ifndef D_BOUNDARY_PROBE_H
#define D_BOUNDARY_PROBE_H
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <cstdint>
// Passive observer. Scope: one flow/scenario per fresh process; no simulated events.
namespace DBoundary {
struct State {
    bool generation_done=false, host_boundary_seen=false, invalid=false;
    uint64_t generated=0, serviced=0, target=0, waiting=0, active=0;
    uint64_t pending=0, toward_host=0, toward_device=0, callbacks=0;
    uint64_t gc_starts=0, gc_ends=0;
    std::set<std::string> gc;
};
inline State& state() { static State value; return value; }
inline void emit(const char* event, uint64_t time) {
    const State& s=state();
    std::cerr << "{\"event\":\"" << event << "\",\"time_ns\":" << time
      << ",\"generation_done\":" << (s.generation_done?"true":"false")
      << ",\"generated\":" << s.generated << ",\"serviced\":" << s.serviced
      << ",\"target\":" << s.target << ",\"waiting\":" << s.waiting
      << ",\"active\":" << s.active << ",\"pcie_pending\":" << s.pending
      << ",\"toward_host\":" << s.toward_host << ",\"toward_device\":" << s.toward_device
      << ",\"gc_ongoing_protected_operations\":" << s.gc.size()
      << ",\"gc_starts\":" << s.gc_starts << ",\"gc_ends\":" << s.gc_ends
      << ",\"callbacks\":" << s.callbacks << ",\"observer_invalid\":" << (s.invalid?"true":"false") << "}\n";
}
inline void flow(uint64_t time,uint64_t gen,uint64_t served,uint64_t target,uint64_t waiting,uint64_t active,const char* event) {
    State& s=state(); s.generated=gen;s.serviced=served;s.target=target;s.waiting=waiting;s.active=active;emit(event,time);
}
inline void generation_done(uint64_t time) { state().generation_done=true;emit("generation_done",time); }
inline void link_deliver(uint64_t time,uint64_t host,uint64_t device) {
    State& s=state();++s.pending;s.toward_host=host;s.toward_device=device;emit("pcie_deliver",time);
}
inline void link_consumed(uint64_t time,uint64_t host,uint64_t device) {
    State& s=state();if(s.pending) --s.pending;else {s.invalid=true;emit("pcie_underflow",time);}
    s.toward_host=host;s.toward_device=device;emit("pcie_consumed_after_followup_schedule",time);
}
template<class Address> inline void gc_change(uint64_t time,const Address& a,bool start) {
    State& s=state();std::ostringstream key;key << a.ChannelID << ':' << a.ChipID << ':' << a.DieID << ':' << a.PlaneID << ':' << a.BlockID;
    if(start) {++s.gc_starts;if(!s.gc.insert(key.str()).second) s.invalid=true;}
    else {++s.gc_ends;if(s.gc.erase(key.str())!=1) s.invalid=true;}
    std::cerr << "{\"event\":\"" << (start?"gc_start":"gc_end") << "\",\"time_ns\":" << time
      << ",\"channel\":" << a.ChannelID << ",\"chip\":" << a.ChipID << ",\"die\":" << a.DieID
      << ",\"plane\":" << a.PlaneID << ",\"block\":" << a.BlockID
      << ",\"gc_ongoing_protected_operations\":" << s.gc.size()
      << ",\"observer_invalid\":" << (s.invalid?"true":"false") << "}\n";
}
inline void post_event(uint64_t time) {
    State& s=state();++s.callbacks;
    if(!s.host_boundary_seen && s.generation_done && s.generated==s.target && s.serviced==s.target &&
       s.waiting==0 && s.active==0 && s.pending==0 && s.toward_host==0 && s.toward_device==0 && !s.invalid) {
        s.host_boundary_seen=true;emit("host_boundary_after_complete_event",time);
    }
}
inline void engine_exit(uint64_t time,uint64_t remaining_events,bool stop) {
    emit("engine_exit_state",time);
    std::cerr << "{\"event\":\"engine_exit\",\"time_ns\":" << time << ",\"event_count_remaining\":" << remaining_events
      << ",\"callbacks\":" << state().callbacks << ",\"explicit_stop\":" << (stop?"true":"false") << "}\n";
}
}
#endif
