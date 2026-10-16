#include "ramulator-bridge.h"
#include <filesystem>
#include <stdexcept>
#include <sstream>
using namespace Ramulator;
std::unique_ptr<RamBridge> projection_bridge;
static void need(bool v,const char* s){if(!v)throw std::runtime_error(s);}
void RamBridge::log(const std::string& e,uint64_t p,uint64_t c,uint64_t a,uint64_t v,const std::string& d){events<<rtl<<','<<now<<','<<e<<','<<p<<','<<c<<','<<a<<','<<v<<",\""<<d<<"\"\n";}
RamBridge::RamBridge(const std::string& path,const std::string& output,bool row1):out(output){
 std::filesystem::create_directories(out);events.open(out+"/bridge-events.csv");need(bool(events),"cannot open bridge events");events<<"rtl_tick,dram_tick,event,parent_uid,child_uid,address,value,detail\n";
 auto cfg=YAML::LoadFile(path);cfg["MemorySystem"]["Controller"]["plugins"][0]["ControllerPlugin"]["path"]=out+"/official-trace";
 {std::ofstream f(out+"/effective-config.yaml");f<<cfg;}
 frontend=Factory::create_frontend(cfg);memory=Factory::create_memory_system(cfg);frontend->connect_memory_system(memory);memory->connect_frontend(frontend);dram=memory->get_ifce<IDRAM>();
 data_latency=dram->m_timing_vals("nCWL")+dram->m_timing_vals("nBL");recovery_latency=data_latency+dram->m_timing_vals("nWR");need(data_latency==16&&recovery_latency==34,"frozen timing changed");
 log("timing",0,0,0,data_latency,"nCWL=12;nBL=4;nWR=18;recovery=34");phase("cold");
 for(;now<256;){if(now==0||(row1&&now==96)){need(drained(),"history boundary not drained");auto p=accept(false,now==0?0:262144,0,0,3,1);parents.at(p).returned=true;--ar_live;log("history_read_discard",p,0,parents.at(p).address,0,"ordinary provider callback required;no RTL R consumer");}step();}
 need(drained(),"history not drained by tick256");phase("common_reset_start");
}
uint64_t RamBridge::accept(bool wr,uint64_t a,unsigned len,unsigned id,unsigned size,unsigned burst){
 need(size==3&&burst==1&&(a&7)==0&&len<=255,"unsupported AXI range/size/burst");need((a>>12)==((a+8ULL*(len+1)-1)>>12),"AXI 4K boundary");
 if(wr)need(can_aw()&&(a&63)==0&&((len+1)%8)==0,"write not full aligned 64B or capacity");else need(can_ar(a,len),"read capacity");
 // Fixed COPY profile reads source / translation tables, never uncompleted destinations.
 if(!wr)need(!((a>=0x80001000ULL&&a<0x80003000ULL)||(a>=0x90100000ULL&&a<0x90108000ULL)),"destination read outside fixed nonoverlap profile");
 const uint64_t uid=++serial;Parent p{uid,a,id,len,0,wr};
 const unsigned count=lines(a,len);need(!wr||count<=32,"AW child reservation overflow");
 for(unsigned i=0;i<count;++i){uint64_t c=++serial;uint64_t block=(a&~63ULL)+i*64ULL;children.emplace(c,Child{c,uid,block,wr});p.children.push_back(c);if(!wr){children.at(c).queued=true;offers.push_back(c);}log("child_create",uid,c,block,wr);}
 credits+=count;if(wr)++aw_live;else ++ar_live;parents.emplace(uid,p);max_ar=std::max(max_ar,ar_live);max_aw=std::max(max_aw,aw_live);max_credits=std::max(max_credits,credits);max_parents=std::max(max_parents,unsigned(parents.size()));max_children=std::max(max_children,unsigned(children.size()));log(wr?"AW":"AR",uid,0,a,len,"id="+std::to_string(id));return uid;
}
void RamBridge::beat(uint64_t uid,uint64_t data,unsigned strb,bool last){auto& p=parents.at(uid);need(p.write&&!p.last&&strb==255,"unsupported/extra W");need(last==(p.got==p.len),"WLAST early/missing");log("W",uid,0,p.address+p.got*8,data,"strb=255;last="+std::to_string(last));++p.got;if(p.got%8==0){auto& c=children.at(p.children.at(p.got/8-1));need(!c.queued,"duplicate write line");c.queued=true;offers.push_back(c.uid);}p.last=last;}
bool RamBridge::read_ready(uint64_t uid,unsigned beat) const{auto& p=parents.at(uid);auto i=((p.address+beat*8)>>6)-(p.address>>6);return children.at(p.children.at(i)).callback;}
bool RamBridge::b_ready(uint64_t uid) const{auto& p=parents.at(uid);if(!p.last)return false;for(auto c:p.children){auto& q=children.at(c);if(!q.issued||now<q.issue+data_latency)return false;}return true;}
void RamBridge::r_consumed(uint64_t uid,unsigned beat){auto& p=parents.at(uid);need(read_ready(uid,beat),"R before callback");log("R",uid,0,p.address+beat*8,beat,"id="+std::to_string(p.id)+";last="+std::to_string(beat==p.len));if(beat==p.len){need(!p.returned,"duplicate RLAST");p.returned=true;--ar_live;}}
void RamBridge::b_consumed(uint64_t uid){auto& p=parents.at(uid);need(b_ready(uid)&&!p.returned,"B before derived data end or duplicate");p.returned=true;--aw_live;last_b=now;log("B",uid,0,p.address,p.id);}
void RamBridge::tick(long cycle){rtl=cycle;step();}
void RamBridge::step(){
 if(!offers.empty()){auto uid=offers.front();auto& c=children.at(uid);need(!c.accepted,"duplicate send");Request req(c.address,c.write?Request::Type::Write:Request::Type::Read,0,[this,uid](Request& r){auto& q=children.at(uid);need(!q.write&&!q.callback,"invalid callback");q.callback=true;log("callback",q.parent,q.uid,q.address,r.depart,"arrive="+std::to_string(r.arrive)+";depart="+std::to_string(r.depart));});
 bool ok=memory->send(req);log("offer",c.parent,c.uid,c.address,ok);if(ok){c.accepted=true;offers.pop_front();log("accepted",c.parent,c.uid,c.address,c.write);}}
 ++now;memory->tick();auto es=BoundaryAudit::take_events();
 for(auto e:es){auto kind=e["kind"].as<std::string>();std::string cmd=e["command"]?e["command"].as<std::string>():kind;auto av=e["address_vector"].as<std::vector<int>>();std::ostringstream detail;for(auto x:av)detail<<x<<';';std::string fields=";event_clock="+std::to_string(e["clock"].as<long>());if(e["accepted"])fields+=";accepted="+std::to_string(e["accepted"].as<bool>());if(e["request_type"])fields+=";request_type="+std::to_string(e["request_type"].as<int>());if(e["channel"])fields+=";channel="+std::to_string(e["channel"].as<int>());log("dram_"+kind,0,0,0,e["clock"].as<long>(),cmd+";vec="+detail.str()+fields);
 if(kind=="issued"&&cmd=="WR"){
 need(av.size()==6,"unexpected address vector");auto count=dram->m_organization.count;uint64_t a=0;unsigned shift=6;auto put=[&](int idx,int width_adjust){unsigned bits=0;for(int n=count[idx];n>1;n>>=1)++bits;bits-=width_adjust;need(av[idx]>=0,"invalid WR address vector");a|=uint64_t(av[idx])<<shift;shift+=bits;};put(0,0);put(5,3);for(int i=1;i<=4;++i)put(i,0);
 Child* found=nullptr;for(auto& [uid,c]:children)if(c.write&&c.accepted&&!c.issued&&c.address==a){need(!found,"ambiguous issued write address");found=&c;}need(found,"unmatched WR");found->issued=true;found->issue=e["clock"].as<long>();need(found->issue==now,"WR clock mismatch");last_wr=now;last_data=now+data_latency;last_recovery=now+recovery_latency;log("WR_issue",found->parent,found->uid,a,now,detail.str());}}
 for(auto& [uid,c]:children)if(c.write&&c.issued){if(!c.data_end&&now>=c.issue+data_latency){c.data_end=true;log("derived_data_end",c.parent,uid,c.address,c.issue+data_latency);}if(!c.recovery&&now>=c.issue+recovery_latency){c.recovery=true;log("derived_recovery",c.parent,uid,c.address,c.issue+recovery_latency);}}
 reap();
}
void RamBridge::reap(){for(auto it=parents.begin();it!=parents.end();){bool done=it->second.returned;for(auto uid:it->second.children){auto& c=children.at(uid);done=done&&(c.write?c.recovery:c.callback);}if(done){unsigned n=it->second.children.size();credits-=n;std::string d="credits="+std::to_string(credits)+";ar_live="+std::to_string(ar_live)+";aw_live="+std::to_string(aw_live);for(auto uid:it->second.children){auto& c=children.at(uid);log("child_retire",c.parent,uid,c.address,c.write,d);children.erase(uid);}log("parent_retire",it->first,0,it->second.address,n,d);it=parents.erase(it);}else ++it;}}
bool RamBridge::drained(){if(!parents.empty()||!children.empty()||!offers.empty()||ar_live||aw_live||credits||now<last_recovery)return false;auto s=BoundaryAudit::snapshot(false);need(bool(s["dram"])&&bool(s["controller_0"])&&bool(s["refresh_0"]),"audit missing");for(auto x:s){auto key=x.first.as<std::string>();if(x.second["clock"])need(x.second["clock"].as<long>()==now,"audit clock mismatch");if(key.rfind("controller_",0)==0)for(auto k:{"read","write","active","priority","pending"})if(x.second[k].as<unsigned>()!=0)return false;if(key.rfind("refresh_",0)==0&&x.second["next_refresh"].as<long>()<=now)return false;}return s["dram"]["future_count"].as<unsigned>()==0&&s["dram"]["refreshing_banks"].as<unsigned>()==0;}
void RamBridge::phase(const std::string& label){log("phase_"+label,0,0,0,credits);events.flush();auto s=BoundaryAudit::snapshot(true);s["bridge"]["rtl_tick"]=rtl;s["bridge"]["dram_tick"]=now;s["bridge"]["parents"]=parents.size();s["bridge"]["children"]=children.size();s["bridge"]["line_credits"]=credits;s["bridge"]["offers"]=offers.size();s["bridge"]["ar_live"]=ar_live;s["bridge"]["aw_live"]=aw_live;s["bridge"]["max_ar_live"]=max_ar;s["bridge"]["max_aw_live"]=max_aw;s["bridge"]["max_credits"]=max_credits;s["bridge"]["max_parents_including_retired_B_recovery_metadata"]=max_parents;s["bridge"]["max_children"]=max_children;s["bridge"]["last_WR_issue"]=last_wr;s["bridge"]["last_data_end"]=last_data;s["bridge"]["last_recovery"]=last_recovery;s["bridge"]["last_B_handshake"]=last_b;std::ofstream f(out+"/phase-"+label+".yaml");f<<s;}
