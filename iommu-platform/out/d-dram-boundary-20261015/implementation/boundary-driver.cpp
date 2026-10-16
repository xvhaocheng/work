#include <fstream>
#include <filesystem>
#include <iostream>
#include <map>
#include <set>
#include "base/base.h"
#include "frontend/frontend.h"
#include "memory_system/memory_system.h"
#ifndef BOUNDARY_BASELINE
#include "boundary_audit.h"
#endif
using namespace Ramulator;
int main(int argc,char** argv){
 if(argc!=5){std::cerr<<"usage: driver contract.json config.yaml case outputdir\n";return 2;}
 YAML::Node log;YAML::Node records(YAML::NodeType::Sequence);
 try{
  const auto contract=YAML::LoadFile(argv[1]);YAML::Node chosen;
  for(auto c:contract["cases"])if(c["id"].as<std::string>()==argv[3])chosen=YAML::Clone(c);
  if(!chosen)throw std::runtime_error("unknown frozen case");
  const std::filesystem::path out=argv[4];std::filesystem::create_directories(out);
  YAML::Node config=YAML::LoadFile(argv[2]);config["MemorySystem"]["Controller"]["plugins"][0]["ControllerPlugin"]["path"]=(out/"official-trace").string();
  {std::ofstream f(out/"effective-config.yaml");f<<config<<"\n";}
  auto frontend=Factory::create_frontend(config);auto memory=Factory::create_memory_system(config);
  frontend->connect_memory_system(memory);memory->connect_frontend(frontend);
  log["case"]=argv[3];log["contract"]=contract["id"];log["clock_epoch"]="cold-zero";
#ifdef BOUNDARY_BASELINE
  log["instrumented"]=false;
#else
  log["instrumented"]=true;
#endif
  std::map<int,int> completed;int accepted=0;int tick=0;bool last_drained=true;bool failed=false;
  auto write_record=[&](YAML::Node r){r["tick"]=tick;records.push_back(r);};
  auto state=[&](bool full){
#ifndef BOUNDARY_BASELINE
   return BoundaryAudit::snapshot(full);
#else
   return YAML::Node();
#endif
  };
  auto drained=[&](const YAML::Node& s){
   int count=0;for(const auto& [id,n]:completed)count+=n;
   bool ok=count==accepted;
#ifndef BOUNDARY_BASELINE
   if(!s["dram"]||!s["controller_0"]||!s["refresh_0"])throw std::runtime_error("missing mandatory audit probe");
   for(const auto& item:s){auto key=item.first.as<std::string>();if(item.second["clock"]&&item.second["clock"].as<int>()!=tick)throw std::runtime_error("component clock differs from driver tick");if(key.rfind("refresh_",0)==0)ok=ok&&item.second["next_refresh"].as<long long>()>item.second["clock"].as<long long>();if(key.rfind("controller_",0)==0){for(auto k:{"read","write","active","priority","pending"})ok=ok&&item.second[k].as<unsigned long long>()==0;}}
   ok=ok&&s["dram"]["future_count"].as<int>()==0&&s["dram"]["refreshing_banks"].as<int>()==0;
#endif
   return ok;
  };
  auto boundary=[&](std::string label){YAML::Node r;r["kind"]=label;auto s=state(true);r["state"]=s;r["drained"]=drained(s);write_record(r);return r["drained"].as<bool>();};
  boundary("cold_initial");
  std::map<int,std::vector<YAML::Node>> schedule;for(auto h:chosen["history"])schedule[h["send_after_tick"].as<int>()].push_back(h);schedule[chosen["target"]["send_after_tick"].as<int>()].push_back(chosen["target"]);
  int id=0;const int end=chosen["run_until_tick"].as<int>();if(end>contract["bounds"]["maximum_tick"].as<int>())throw std::runtime_error("tick bound violation");
  for(tick=0;tick<=end;){
   if(schedule.count(tick))for(auto input:schedule[tick]){
    const std::string role=input["role"].as<std::string>();if(!boundary("before_"+role)){failed=true;throw std::runtime_error("fixed input boundary not drained");}
    const int uid=++id;const auto address=input["physical_address"].as<long long>();const int send_tick=tick;
    Request req(address,Request::Type::Read,0,[&,uid,address,role,send_tick](Request& r){
     completed[uid]++;YAML::Node e;e["kind"]="callback";e["id"]=uid;e["address"]=address;e["role"]=role;e["send_after_tick"]=send_tick;e["address_vector"]=r.addr_vec;e["arrive"]=r.arrive;e["depart"]=r.depart;write_record(e);
    });req.scratchpad[0]=uid;
    const bool ok=memory->send(req);if(ok)accepted++;
    YAML::Node e;e["kind"]="send";e["id"]=uid;e["address"]=address;e["role"]=role;e["accepted"]=ok;e["send_after_tick"]=tick;write_record(e);
    boundary("after_send_"+role);if(!ok)throw std::runtime_error("frozen send rejected");
   }
   if(tick==end)break;
   ++tick;memory->tick();
   YAML::Node events(YAML::NodeType::Sequence);
#ifndef BOUNDARY_BASELINE
   events=BoundaryAudit::take_events();
#endif
   auto compact=state(false);const bool now_drained=drained(compact);bool refresh_boundary=false;
   for(auto e:events){auto k=e["kind"].as<std::string>();if(k=="REFab_end"||(k=="issued"&&e["command"].as<std::string>()=="REFab"))refresh_boundary=true;}
   YAML::Node r;r["kind"]="post_tick";r["events"]=events;r["drained"]=now_drained;r["state"]=refresh_boundary||(last_drained!=now_drained)?state(true):compact;r["full_state"]=refresh_boundary||(last_drained!=now_drained);write_record(r);last_drained=now_drained;
  }
  bool final=boundary("final");if(!final)failed=true;
  for(int uid=1;uid<=accepted;++uid)if(completed[uid]!=1)failed=true;
  log["accepted"]=accepted;log["completion_counts"]=completed;log["valid"]=!failed;log["records"]=records;
  {std::ofstream f(out/"events.yaml");f<<log<<"\n";}
  spdlog::apply_all([](const std::shared_ptr<spdlog::logger>& l){l->flush();});
  return failed?1:0;
 }catch(const std::exception& e){log["valid"]=false;log["error"]=e.what();log["records"]=records;std::filesystem::create_directories(argv[4]);std::ofstream f(std::filesystem::path(argv[4])/"events.yaml");f<<log<<"\n";std::cerr<<e.what()<<"\n";return 1;}
}
