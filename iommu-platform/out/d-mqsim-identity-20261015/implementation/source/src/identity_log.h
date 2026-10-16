#ifndef MQ_IDENTITY_LOG_H
#define MQ_IDENTITY_LOG_H
#include <cstdint>
#include <map>
#include <iostream>
#include <cstddef>
#include <new>
#include <utility>
// Passive process-local bookkeeping only. No model fields, events, or decisions.
namespace Identity {
// Dedicated BSS arena: observer map nodes never consume the model malloc heap.
// No reclamation is needed for this single fresh-process frozen workload.
struct Arena {
 alignas(std::max_align_t) unsigned char bytes[32u*1024u*1024u];
 size_t used=0;
};
inline Arena& arena() { static Arena a; return a; }
template<class T> struct Allocator {
 typedef T value_type;
 template<class U> struct rebind { typedef Allocator<U> other; };
 Allocator() noexcept {}
 template<class U> Allocator(const Allocator<U>&) noexcept {}
 T* allocate(size_t n) {
  Arena& a=arena();const size_t alignment=alignof(T);
  const size_t start=(a.used+alignment-1)&~(alignment-1);
  if(start>sizeof(a.bytes) || n>(sizeof(a.bytes)-start)/sizeof(T)) throw std::bad_alloc();
  a.used=start+n*sizeof(T);return reinterpret_cast<T*>(a.bytes+start);
 }
 void deallocate(T*,size_t) noexcept {}
};
template<class T,class U> inline bool operator==(const Allocator<T>&,const Allocator<U>&) {return true;}
template<class T,class U> inline bool operator!=(const Allocator<T>&,const Allocator<U>&) {return false;}
template<class V> using Map=std::map<const void*,V,std::less<const void*>,Allocator<std::pair<const void* const,V>>>;

struct Child { uint64_t uid, owner, source, type, stream, lpa, ppa; };
struct State {
 uint64_t host_next=0, child_next=0;
 bool disabled=false;
 Map<uint64_t> hosts, sqes, users, cqes;
 Map<Child> children;
};
inline State& state() { static State s; return s; }
inline void error(uint64_t t,const char* reason,const void* p) {
 std::cerr << "{\"event\":\"identity_error\",\"time_ns\":"<<t<<",\"reason\":\""<<reason<<"\",\"pointer\":\""<<p<<"\"}\n";
}
inline uint64_t lookup(const Map<uint64_t>& m,const void* p,uint64_t t,const char* reason) {
 auto i=m.find(p); if(i==m.end()) {error(t,reason,p);return 0;} return i->second;
}
inline void lpa_event(const char* event,uint64_t t,uint64_t stream,uint64_t lpa) {
 if(state().disabled)return;
 std::cerr<<"{\"event\":\""<<event<<"\",\"time_ns\":"<<t<<",\"stream\":"<<stream<<",\"lpa\":"<<lpa<<"}\n";
}
template<class H> inline void host_generated(uint64_t t,H* p,uint64_t flow) {
 if(state().disabled)return;
 try {
 uint64_t uid=++state().host_next; state().hosts[p]=uid;
 std::cerr<<"{\"event\":\"identity_host_generated\",\"time_ns\":"<<t<<",\"host_uid\":"<<uid<<",\"host_ptr\":\""<<p<<"\",\"flow\":"<<flow<<",\"arrival_ns\":"<<p->Arrival_time<<",\"lba\":"<<p->Start_LBA<<",\"sectors\":"<<p->LBA_count<<",\"type\":"<<static_cast<unsigned>(p->Type)<<"}\n";
 } catch(const std::bad_alloc&) {
  state().disabled=true;error(t,"observer_arena_exhausted_identity_disabled",nullptr);
 }
}
template<class S,class H> inline void sqe_bind(uint64_t t,S* sqe,H* h) {
 if(state().disabled)return;
 try {
 auto uid=lookup(state().hosts,h,t,"sqe_missing_host"); state().sqes[sqe]=uid;
 std::cerr<<"{\"event\":\"identity_sqe_bind\",\"time_ns\":"<<t<<",\"host_uid\":"<<uid<<",\"host_ptr\":\""<<h<<"\",\"sqe_ptr\":\""<<sqe<<"\",\"cid\":"<<sqe->Command_Identifier<<"}\n";
 } catch(const std::bad_alloc&) {
  state().disabled=true;error(t,"observer_arena_exhausted_identity_disabled",nullptr);
 }
}
template<class U,class S> inline void user_bind(uint64_t t,U* u,S* sqe) {
 if(state().disabled)return;
 try {
 auto uid=lookup(state().sqes,sqe,t,"user_missing_sqe"); state().users[u]=uid;
 std::cerr<<"{\"event\":\"identity_user_bind\",\"time_ns\":"<<t<<",\"host_uid\":"<<uid<<",\"user_uid\":"<<uid<<",\"native_id\":"<<u->ID<<",\"user_ptr\":\""<<u<<"\",\"sqe_ptr\":\""<<sqe<<"\",\"cid\":"<<sqe->Command_Identifier<<",\"stream\":"<<u->Stream_id<<",\"lba\":"<<u->Start_LBA<<",\"sectors\":"<<u->SizeInSectors<<"}\n";
 } catch(const std::bad_alloc&) {
  state().disabled=true;error(t,"observer_arena_exhausted_identity_disabled",nullptr);
 }
}
template<class U> inline void user_event(const char* event,uint64_t t,U* u) {
 if(state().disabled)return;
 try {
 auto uid=lookup(state().users,u,t,"event_missing_user");
 std::cerr<<"{\"event\":\""<<event<<"\",\"time_ns\":"<<t<<",\"user_uid\":"<<uid<<",\"user_ptr\":\""<<u<<"\"}\n";
 } catch(const std::bad_alloc&) {
  state().disabled=true;error(t,"observer_arena_exhausted_identity_disabled",nullptr);
 }
}
inline void child_event(const char* event,uint64_t t,const void* p) {
 if(state().disabled)return;
 try {
 auto i=state().children.find(p); if(i==state().children.end()) {error(t,"event_missing_child",p);return;}
 const Child& c=i->second;
 std::cerr<<"{\"event\":\""<<event<<"\",\"time_ns\":"<<t<<",\"child_uid\":"<<c.uid<<",\"user_uid\":"<<c.owner<<",\"child_ptr\":\""<<p<<"\",\"source\":"<<c.source<<",\"type\":"<<c.type<<",\"stream\":"<<c.stream<<",\"creation_lpa\":"<<c.lpa<<",\"creation_ppa\":"<<c.ppa<<"}\n";
 } catch(const std::bad_alloc&) {
  state().disabled=true;error(t,"observer_arena_exhausted_identity_disabled",nullptr);
 }
}
inline void child_retire(const char* event,uint64_t t,const void* p) {
 if(state().disabled)return;
 try {
 child_event(event,t,p);state().children.erase(p);
 } catch(const std::bad_alloc&) {
  state().disabled=true;error(t,"observer_arena_exhausted_identity_disabled",nullptr);
 }
}
template<class T> inline void child_live_event(const char* event,uint64_t t,T* p) {
 if(state().disabled)return;
 try {
 // Scalar fields initialized by both base constructors; never dereference owner.
 child_event(event,t,p);
 auto i=state().children.find(p);if(i==state().children.end())return;
 std::cerr<<"{\"event\":\"identity_child_position\",\"time_ns\":"<<t<<",\"child_uid\":"<<i->second.uid<<",\"lpa\":"<<p->LPA<<",\"ppa\":"<<p->PPA<<"}\n";
 } catch(const std::bad_alloc&) {
  state().disabled=true;error(t,"observer_arena_exhausted_identity_disabled",nullptr);
 }
}
template<class T> inline void child_created(uint64_t t,T* p) {
 if(state().disabled)return;
 try {
 uint64_t owner=p->UserIORequest?lookup(state().users,p->UserIORequest,t,"child_missing_user"):0;
 Child c={++state().child_next,owner,static_cast<uint64_t>(p->Source),static_cast<uint64_t>(p->Type),p->Stream_id,p->LPA,p->PPA};
 state().children[p]=c; child_event("identity_child_created",t,p);
 } catch(const std::bad_alloc&) {
  state().disabled=true;error(t,"observer_arena_exhausted_identity_disabled",nullptr);
 }
}
template<class U,class C> inline void cqe_send(uint64_t t,U* u,C* c) {
 if(state().disabled)return;
 try {
 auto uid=lookup(state().users,u,t,"cqe_missing_user"); state().cqes[c]=uid;
 std::cerr<<"{\"event\":\"identity_cqe_send\",\"time_ns\":"<<t<<",\"user_uid\":"<<uid<<",\"user_ptr\":\""<<u<<"\",\"cqe_ptr\":\""<<c<<"\",\"cid\":"<<c->Command_Identifier<<"}\n";
 } catch(const std::bad_alloc&) {
  state().disabled=true;error(t,"observer_arena_exhausted_identity_disabled",nullptr);
 }
}
template<class H,class C> inline void host_completed(uint64_t t,H* h,C* c) {
 if(state().disabled)return;
 try {
 auto uid=lookup(state().hosts,h,t,"complete_missing_host"), sent=lookup(state().cqes,c,t,"complete_missing_cqe");
 if(uid!=sent) error(t,"host_cqe_uid_mismatch",c);
 std::cerr<<"{\"event\":\"identity_host_complete\",\"time_ns\":"<<t<<",\"host_uid\":"<<uid<<",\"cqe_user_uid\":"<<sent<<",\"host_ptr\":\""<<h<<"\",\"cqe_ptr\":\""<<c<<"\",\"cid\":"<<c->Command_Identifier<<"}\n";
 state().hosts.erase(h);state().cqes.erase(c);
 } catch(const std::bad_alloc&) {
  state().disabled=true;error(t,"observer_arena_exhausted_identity_disabled",nullptr);
 }
}
}
#endif
