#pragma once
#include <cstdint>
#include <deque>
#include <map>
#include <memory>
#include <fstream>
#include <vector>
#include <string>
#include "base/base.h"
#include "frontend/frontend.h"
#include "memory_system/memory_system.h"
#include "dram/dram.h"
#include "boundary_audit.h"
struct RamBridge {
 struct Child {uint64_t uid,parent,address; bool write,queued=false,accepted=false,callback=false,issued=false,data_end=false,recovery=false; long issue=-1;};
 struct Parent {uint64_t uid,address;unsigned id,len,got=0;bool write,last=false,returned=false;std::vector<uint64_t> children;};
 Ramulator::IFrontEnd* frontend; Ramulator::IMemorySystem* memory; Ramulator::IDRAM* dram;
 std::map<uint64_t,Parent> parents;std::map<uint64_t,Child> children;std::deque<uint64_t> offers;std::ofstream events;std::string out;long now=0,rtl=-1,last_wr=-1,last_data=-1,last_recovery=-1,last_b=-1;uint64_t serial=0;unsigned ar_live=0,aw_live=0,credits=0;unsigned max_ar=0,max_aw=0,max_credits=0,max_parents=0,max_children=0;int data_latency,recovery_latency;
 RamBridge(const std::string& config,const std::string& output,bool row1);
 void log(const std::string& event,uint64_t parent=0,uint64_t child=0,uint64_t address=0,uint64_t value=0,const std::string& detail="");
 static unsigned lines(uint64_t address,unsigned len){return unsigned(((address+8ULL*(len+1)-1)>>6)-(address>>6)+1);}
 bool can_ar(uint64_t address,unsigned len) const{return ar_live<16&&credits+lines(address,len)<=1024;}
 bool can_aw() const{return aw_live<16&&credits+32<=1024;}
 uint64_t accept(bool write,uint64_t address,unsigned len,unsigned id,unsigned size,unsigned burst);
 void beat(uint64_t parent,uint64_t data,unsigned strb,bool last);
 bool read_ready(uint64_t parent,unsigned beat) const;
 bool b_ready(uint64_t parent) const;
 void r_consumed(uint64_t parent,unsigned beat);void b_consumed(uint64_t parent);
 void tick(long cycle);void step();bool drained();void phase(const std::string& label);void reap();
};
extern std::unique_ptr<RamBridge> projection_bridge;
