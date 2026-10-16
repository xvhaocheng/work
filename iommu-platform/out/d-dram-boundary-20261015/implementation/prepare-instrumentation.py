from pathlib import Path
import shutil,json,hashlib,difflib
p=Path(__file__).resolve().parent;original=p/'ramulator2-be93be78055d922aa1d4d33e15bcc8f2b0c61a9d';private=p/'instrumented-source'
shutil.copytree(original,private,ignore=shutil.ignore_patterns('.git','libramulator.so'))
changes=[]
def modify(name,fn):
 f=private/name;raw=f.read_bytes();s=raw.decode();crlf='\r\n' in s;s=s.replace('\r\n','\n');new=fn(s);assert new!=s,name
 f.write_bytes(new.replace('\n','\r\n').encode() if crlf else new.encode());changes.append(name)
modify('CMakeLists.txt',lambda s:s+'\ntarget_sources(ramulator PRIVATE src/boundary_audit.cpp)\n')
(private/'src/boundary_audit.h').write_text('''#pragma once
#include <functional>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
namespace BoundaryAudit {
using Probe = std::function<YAML::Node(bool)>;
void add_probe(const std::string&, Probe);
YAML::Node snapshot(bool full);
void event(const YAML::Node&);
YAML::Node take_events();
inline YAML::Node event_base(const std::string& kind, long long clock) { YAML::Node n; n["kind"]=kind;n["clock"]=clock;return n; }
}
''')
(private/'src/boundary_audit.cpp').write_text('''#include "boundary_audit.h"
namespace BoundaryAudit {
static std::vector<std::pair<std::string,Probe>> probes;
static YAML::Node events(YAML::NodeType::Sequence);
void add_probe(const std::string& name,Probe fn){probes.emplace_back(name,fn);}
YAML::Node snapshot(bool full){YAML::Node n;for(const auto& p:probes)n[p.first]=p.second(full);return n;}
void event(const YAML::Node& n){events.push_back(YAML::Clone(n));}
YAML::Node take_events(){auto n=YAML::Clone(events);events=YAML::Node(YAML::NodeType::Sequence);return n;}
}
''')
def controller(s):
 s='#include "boundary_audit.h"\n'+s
 s=s.replace('      m_num_cores = frontend->get_num_cores();','''      BoundaryAudit::add_probe("controller_"+std::to_string(m_channel_id), [this](bool full) {
        YAML::Node n;n["clock"]=m_clk;n["read"]=m_read_buffer.size();n["write"]=m_write_buffer.size();n["active"]=m_active_buffer.size();n["priority"]=m_priority_buffer.size();n["pending"]=pending.size();
        if(full){n["write_mode"]=m_is_write_mode;auto emit=[&](const auto& items){YAML::Node a(YAML::NodeType::Sequence);for(const auto& r:items){YAML::Node q;q["address"]=r.addr;q["address_vector"]=r.addr_vec;q["type"]=r.type_id;q["id"]=r.scratchpad[0];q["command"]=r.command;q["final_command"]=r.final_command;q["arrive"]=r.arrive;q["depart"]=r.depart;a.push_back(q);}return a;};n["read_entries"]=emit(m_read_buffer.buffer);n["write_entries"]=emit(m_write_buffer.buffer);n["active_entries"]=emit(m_active_buffer.buffer);n["priority_entries"]=emit(m_priority_buffer.buffer);n["pending_entries"]=emit(pending);}return n;
      });
      m_num_cores = frontend->get_num_cores();''')
 s=s.replace('      return is_success;\n    }\n\n    void tick()', '''      auto e=BoundaryAudit::event_base("priority_send",m_clk);e["channel"]=m_channel_id;e["request_type"]=req.type_id;e["address_vector"]=req.addr_vec;e["accepted"]=is_success;BoundaryAudit::event(e);
      return is_success;
    }

    void tick()''')
 return s
modify('src/dram_controller/impl/generic_dram_controller.cpp',controller)
def row(s):
 s='#include "boundary_audit.h"\n'+s
 return s.replace('      m_col_accesses.resize(m_num_banks * m_num_bankgroups * m_num_ranks, 0);','''      m_col_accesses.resize(m_num_banks * m_num_bankgroups * m_num_ranks, 0);
      BoundaryAudit::add_probe("cap_"+std::to_string(m_ctrl->m_channel_id),[this](bool full){YAML::Node n;if(full){n["cap"]=m_cap;n["column_accesses"]=m_col_accesses;}return n;});''')
modify('src/dram_controller/impl/rowpolicy/basic_rowpolicies.cpp',row)
def refresh(s):
 s='#include "boundary_audit.h"\n'+s
 return s.replace('      m_next_refresh_cycle = m_nrefi;', '''      m_next_refresh_cycle = m_nrefi;
      BoundaryAudit::add_probe("refresh_"+std::to_string(m_ctrl->m_channel_id),[this](bool full){YAML::Node n;n["clock"]=m_clk;n["next_refresh"]=m_next_refresh_cycle;n["nREFI"]=m_nrefi;return n;});''')
modify('src/dram_controller/impl/refresh/all_bank_refresh.cpp',refresh)
def dram(s):
 s='#include "boundary_audit.h"\n'+s
 s=s.replace('      create_nodes();', '''      create_nodes();
      BoundaryAudit::add_probe("dram",[this](bool full){
        YAML::Node n;n["clock"]=m_clk;n["future_count"]=m_future_actions.size();int refreshing=0;YAML::Node nodes(YAML::NodeType::Sequence);
        std::function<void(const Node*,std::vector<int>)> walk=[&](const Node* node,std::vector<int> path){path.push_back(node->m_node_id);if(node->m_level==m_levels("bank")&&node->m_state==m_states("Refreshing"))refreshing++;
          if(full){YAML::Node q;q["path"]=path;q["level"]=node->m_level;q["state"]=node->m_state;q["command_ready_clock"]=node->m_cmd_ready_clk;YAML::Node histories(YAML::NodeType::Sequence);for(const auto& h:node->m_cmd_history){YAML::Node a(YAML::NodeType::Sequence);for(auto x:h)a.push_back(x);histories.push_back(a);}q["command_history"]=histories;YAML::Node rows(YAML::NodeType::Sequence);for(auto [id,state]:node->m_row_state){YAML::Node r;r["row"]=id;r["state"]=state;rows.push_back(r);}q["rows"]=rows;nodes.push_back(q);}for(auto child:node->m_child_nodes)walk(child,path);};
        for(auto channel:m_channels)walk(channel,{});n["refreshing_banks"]=refreshing;
        if(full){n["nodes"]=nodes;YAML::Node a(YAML::NodeType::Sequence);for(const auto& f:m_future_actions){YAML::Node q;q["command"]=f.cmd;q["address_vector"]=f.addr_vec;q["clock"]=f.clk;a.push_back(q);}n["future_actions"]=a;n["nREFI"]=m_timing_vals("nREFI");n["nRFC"]=m_timing_vals("nRFC");n["tCK_ps"]=m_timing_vals("tCK_ps");n["read_latency"]=m_read_latency;YAML::Node commands(YAML::NodeType::Sequence);for(int i=0;i<m_commands.size();++i)commands.push_back(std::string(m_commands(i)));n["command_names"]=commands;n["organization"]=m_organization.count;}return n;
      });''')
 s=s.replace('      check_future_action(command, addr_vec);','''      check_future_action(command, addr_vec);
      auto e=BoundaryAudit::event_base("issued",m_clk);e["command"]=std::string(m_commands(command));e["address_vector"]=addr_vec;BoundaryAudit::event(e);''')
 s=s.replace('          m_channels[channel_id]->update_states(m_commands("REFab_end"), addr_vec, m_clk);','''          m_channels[channel_id]->update_states(m_commands("REFab_end"), addr_vec, m_clk);
          {auto e=BoundaryAudit::event_base("REFab_end",m_clk);e["address_vector"]=addr_vec;BoundaryAudit::event(e);}''')
 return s
modify('src/dram/impl/DDR4.cpp',dram)
patch=''.join(''.join(difflib.unified_diff((original/f).read_text().splitlines(True),(private/f).read_text().splitlines(True),fromfile='a/'+f,tofile='b/'+f)) for f in changes)
(p/'passive-audit.patch').write_text(patch)
manifest={str(f.relative_to(private)):hashlib.sha256(f.read_bytes()).hexdigest() for f in sorted(private.rglob('*')) if f.is_file()}
(p/'receipts/instrumented-prebuild.json').write_text(json.dumps({'changed_existing_files':changes,'added_files':['src/boundary_audit.h','src/boundary_audit.cpp'],'files':manifest},indent=2)+'\n')
print('private instrumentation prepared',len(manifest),'files')
