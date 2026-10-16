#pragma once
#include "fixture_data.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_set>
namespace c117 {
struct Span { uint32_t packet, src, dst, length; };
struct Used { uint32_t id, length; };
struct CheckResult { bool ok; std::string reason; uint64_t frame_bytes, record_bytes, guard_bytes, digest; };
inline const std::array<uint8_t,kHeaderBytes>& header() {
  static const std::array<uint8_t,kHeaderBytes> h={0,0,0,0,0,0,0,0,0,0,1,0}; return h;
}
inline void initialize_memory(std::vector<uint8_t>& m) {
  if(m.size()!=kMemoryBytes) throw std::runtime_error("wrong memory size");
  std::fill(m.begin(),m.end(),0xa5);
  std::fill(m.begin(),m.begin()+kSourceEnd,0x5a);
  for (const auto& p:packets()) {
    size_t done=0;
    for (const auto& s:p.source) {
      std::copy_n(p.frame.begin()+done,s.length,m.begin()+s.offset); done+=s.length;
    }
    if(done!=p.length) throw std::runtime_error("bad source layout");
  }
}
inline std::vector<uint8_t> initial_memory() {
  std::vector<uint8_t> m(kMemoryBytes); initialize_memory(m); return m;
}
inline std::vector<Span> spans() {
  std::vector<Span> out;
  for(const auto& p:packets()) {
    size_t si=0,di=0; uint32_t so=0,dof=kHeaderBytes,left=p.length;
    while(left) {
      if(si>=p.source.size() || di>=p.destination.size()) throw std::runtime_error("capacity exhausted");
      const auto& s=p.source[si]; const auto& d=p.destination[di];
      uint32_t n=std::min({left,s.length-so,d.capacity-dof});
      if(!n) throw std::runtime_error("zero span");
      out.push_back({p.packet_id,s.offset+so,d.offset+dof,n}); left-=n;so+=n;dof+=n;
      if(so==s.length){++si;so=0;} if(dof==d.capacity){++di;dof=0;}
    }
  }
  return out;
}
// Caller must establish completed reads/writes and quiet before this software handoff.
// This helper is deliberately not a hardware completion oracle.
inline std::vector<Used> publish_after_quiet(std::vector<uint8_t>& m) {
  if(m.size()!=kMemoryBytes) throw std::runtime_error("wrong memory size");
  std::vector<Used> out;
  for(const auto& p:packets()) {
    std::copy(header().begin(),header().end(),m.begin()+p.destination.front().offset);
    out.push_back({p.cookie_id,kHeaderBytes+p.length});
  }
  return out;
}
// Independent receive-side checker: resolve the original posted ID, walk its
// chain, check header/complete frame/order and every untouched destination byte.
// Does not trust executor spans, completion counters or source after retirement.
inline CheckResult consume(const std::vector<uint8_t>& m,const std::vector<Used>& used) {
  CheckResult r{false,"",0,0,0,1469598103934665603ull};
  auto fail=[&](const std::string& s){r.reason=s;return r;};
  if(m.size()!=kMemoryBytes) return fail("memory size");
  if(used.size()!=packets().size()) return fail("used count");
  std::array<bool,kMemoryBytes> touched{};
  std::unordered_set<uint32_t> consumed;
  for(size_t order=0;order<used.size();++order) {
    const auto& u=used[order]; const Packet* packet=nullptr;
    for(const auto& p:packets()) if(p.cookie_id==u.id) packet=&p;
    if(!packet || !consumed.insert(u.id).second) return fail("wrong or repeated original cookie");
    const auto& p=*packet;
    if(p.order!=order) return fail("publication order");
    if(u.length!=kHeaderBytes+p.length) return fail("used length");
    uint32_t id=u.id,left=u.length,logical=0;
    std::unordered_set<uint32_t> visited;
    while(left) {
      const Descriptor* desc=nullptr;
      for(const auto& d:p.destination) if(d.id==id) desc=&d;
      if(!desc || !visited.insert(id).second) return fail("invalid descriptor chain");
      const auto& d=*desc; uint32_t n=std::min(left,d.capacity);
      if(d.offset<kSourceEnd || uint64_t(d.offset)+n>m.size()) return fail("destination bounds");
      for(uint32_t j=0;j<n;++j,++logical) {
        uint8_t expected=logical<kHeaderBytes?header()[logical]:p.frame[logical-kHeaderBytes];
        if(m[d.offset+j]!=expected) return fail(logical<kHeaderBytes?"virtio header mismatch":"frame byte mismatch");
        if(touched[d.offset+j]) return fail("overlapping receive records");
        touched[d.offset+j]=true; ++r.record_bytes;
        if(logical>=kHeaderBytes){++r.frame_bytes;r.digest=(r.digest^m[d.offset+j])*1099511628211ull;}
      }
      left-=n;if(left && !d.has_next) return fail("descriptor capacity"); id=d.next;
    }
  }
  for(uint32_t i=kSourceEnd;i<kMemoryBytes;++i) if(!touched[i]) {
    ++r.guard_bytes;if(m[i]!=0xa5) return fail("destination neighbor guard overwritten");
  }
  r.ok=true;r.reason="complete original-cookie records and guards verified";return r;
}
} // namespace c117
