from pathlib import Path
import shutil,hashlib,json,random,re,difflib,datetime,xml.etree.ElementTree as ET
base=Path(__file__).resolve().parent
original=base/'MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16'
dest=base/'instrumented'
assert not dest.exists()
shutil.copytree(original,dest,ignore=shutil.ignore_patterns('build','MQSim'))
def edit(path,old,new):
 p=dest/path;s=p.read_text();assert s.count(old)==1,(path,old,s.count(old));p.write_text(s.replace(old,new))
header=r'''#ifndef D_BOUNDARY_PROBE_H
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
'''
(dest/'src/d_boundary_probe.h').write_text(header)
for f in ['host/PCIe_Link.cpp','host/PCIe_Root_Complex.cpp','host/IO_Flow_Base.cpp','host/IO_Flow_Trace_Based.cpp','ssd/GC_and_WL_Unit_Base.cpp','ssd/GC_and_WL_Unit_Page_Level.cpp','sim/Engine.cpp']:
 p=dest/'src'/f;p.write_text('#include "../d_boundary_probe.h"\n'+p.read_text())
for q in ['Message_buffer_toward_root_complex','Message_buffer_toward_ssd_device']:
 edit('src/host/PCIe_Link.cpp',q+'.push(message);',q+'.push(message);\n\t\t\t\tDBoundary::link_deliver(Simulator->Time(), Message_buffer_toward_root_complex.size(), Message_buffer_toward_ssd_device.size());')
edit('src/host/PCIe_Link.cpp','\n\t}\n}\n','\n\t\tDBoundary::link_consumed(Simulator->Time(), Message_buffer_toward_root_complex.size(), Message_buffer_toward_ssd_device.size());\n\t}\n}\n')
edit('src/host/IO_Flow_Base.h','\t\tvoid Start_simulation();','\t\tvoid Start_simulation();\n\t\tvoid D_report_boundary_state(const char* event) const;')
p=dest/'src/host/IO_Flow_Base.cpp';s=p.read_text();pos=s.index('{',s.index('namespace Host_Components'))+1;s=s[:pos]+'''
    void IO_Flow_Base::D_report_boundary_state(const char* event) const {
        DBoundary::flow(Simulator->Time(), STAT_generated_request_count, STAT_serviced_request_count,
            total_requests_to_be_generated, waiting_requests.size(), nvme_software_request_queue.size(), event);
    }
'''+s[pos:];p.write_text(s)
edit('src/host/PCIe_Root_Complex.cpp','if (address >= DATA_MEMORY_REGION) {\n\t\t\t//nothing to do\n\t\t} else {','if (address >= DATA_MEMORY_REGION) {\n\t\t\tDBoundary::emit("host_payload_arrived_no_data_model", Simulator->Time());\n\t\t\t//nothing to do\n\t\t} else {')
# This call follows virtual trace consumption, including its CQ-head doorbell submission.
edit('src/host/PCIe_Root_Complex.cpp','((*IO_flows)[flow_id])->NVMe_consume_io_request((Completion_Queue_Entry*)payload);','((*IO_flows)[flow_id])->NVMe_consume_io_request((Completion_Queue_Entry*)payload);\n\t\t\t\t\t((*IO_flows)[flow_id])->D_report_boundary_state("host_cqe_consume_returned");')
edit('src/host/IO_Flow_Trace_Based.cpp','\n\tif (STAT_generated_request_count < total_requests_to_be_generated)\n','\n\tD_report_boundary_state("trace_generation_event");\n\tif (STAT_generated_request_count == total_requests_to_be_generated) DBoundary::generation_done(Simulator->Time());\n\tif (STAT_generated_request_count < total_requests_to_be_generated)\n')
edit('src/ssd/GC_and_WL_Unit_Page_Level.cpp','pbke->Ongoing_erase_operations.insert(gc_candidate_block_id);','pbke->Ongoing_erase_operations.insert(gc_candidate_block_id);\n\t\t\tDBoundary::gc_change(Simulator->Time(), gc_candidate_address, true);')
edit('src/ssd/GC_and_WL_Unit_Base.cpp','pbke->Ongoing_erase_operations.erase(pbke->Ongoing_erase_operations.find(transaction->Address.BlockID));','pbke->Ongoing_erase_operations.erase(pbke->Ongoing_erase_operations.find(transaction->Address.BlockID));\n\t\t\t\tDBoundary::gc_change(Simulator->Time(), transaction->Address, false);')
# Cover the other insertion too; static WL is disabled in frozen inputs.
edit('src/ssd/GC_and_WL_Unit_Base.cpp','pbke->Ongoing_erase_operations.insert(wl_candidate_block_id);','pbke->Ongoing_erase_operations.insert(wl_candidate_block_id);\n\t\tDBoundary::gc_change(Simulator->Time(), wl_candidate_address, true);')
edit('src/sim/Engine.cpp','ev->Target_sim_object->Execute_simulator_event(ev);','ev->Target_sim_object->Execute_simulator_event(ev);\n\t\t\t\t\tDBoundary::post_event(_sim_time);')
edit('src/sim/Engine.cpp','\t\t\t_EventList->Remove(minNode);\n\t\t}\n','\t\t\t_EventList->Remove(minNode);\n\t\t}\n\t\tDBoundary::engine_exit(_sim_time, _EventList->Count, stop);\n')
inputs=base/'inputs';inputs.mkdir()
config=ET.parse(original/'ssdconfig.xml')
overrides={'Seed':'321','Enabled_Preconditioning':'false','IO_Queue_Depth':'64','Queue_Fetch_Size':'16','Ideal_Mapping_Table':'false','Transaction_Scheduling_Policy':'OUT_OF_ORDER','Static_Wearleveling_Enabled':'false','Flash_Channel_Count':'1','Chip_No_Per_Channel':'1','Die_No_Per_Chip':'1','Plane_No_Per_Die':'1','Block_No_Per_Plane':'64','Page_No_Per_Block':'16','Page_Capacity':'8192'}
for key,val in overrides.items():
 nodes=config.findall('.//'+key);assert len(nodes)==1;nodes[0].text=val
config.write(inputs/'ssdconfig.xml',encoding='us-ascii',xml_declaration=True)
rng=random.Random(913);pages=list(range(512))+[rng.randrange(512) for _ in range(8192-512)]
lines=[f'{100000+i*100000} 0 {page*16} 16 0\n' for i,page in enumerate(pages)]
for n in [512,2048,4096,8192]:
 trace=inputs/f'write-{n}.trace';trace.write_text(''.join(lines[:n]))
 root=ET.Element('MQSim_IO_Scenarios');scenario=ET.SubElement(root,'IO_Scenario');flow=ET.SubElement(scenario,'IO_Flow_Parameter_Set_Trace_Based')
 vals={'Priority_Class':'HIGH','Device_Level_Data_Caching_Mode':'TURNED_OFF','Channel_IDs':'0','Chip_IDs':'0','Die_IDs':'0','Plane_IDs':'0','Initial_Occupancy_Percentage':'0','File_Path':str(trace),'Percentage_To_Be_Executed':'100','Relay_Count':'1','Time_Unit':'NANOSECOND'}
 for key,val in vals.items(): ET.SubElement(flow,key).text=val
 ET.ElementTree(root).write(inputs/f'workload-{n}.xml',encoding='us-ascii',xml_declaration=True)
patch=''
for p in sorted((dest/'src').rglob('*')):
 if not p.is_file():continue
 rel=p.relative_to(dest);old=original/rel
 oldtext=old.read_text() if old.exists() else ''
 newtext=p.read_text()
 if oldtext!=newtext:patch+=''.join(difflib.unified_diff(oldtext.splitlines(True),newtext.splitlines(True),fromfile='a/'+str(rel) if old.exists() else '/dev/null',tofile='b/'+str(rel)))
(base/'instrumentation.patch').write_text(patch)
files=[base/'instrumentation.patch']+sorted(p for p in dest.rglob('*') if p.is_file())+sorted(p for p in inputs.rglob('*') if p.is_file())
manifest={'recorded_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'phase':'before instrumented build','contract_sha256':hashlib.sha256((base.parent/'root/boundary-contract.json').read_bytes()).hexdigest(),'files':[{'path':str(p.relative_to(base)),'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in files]}
(base/'instrumented-prebuild-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
print('Prepared',len(manifest['files']),'files, patch bytes',len(patch))
