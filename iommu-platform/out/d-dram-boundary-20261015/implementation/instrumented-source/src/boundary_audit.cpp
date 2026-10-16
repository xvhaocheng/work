#include "boundary_audit.h"
namespace BoundaryAudit {
static std::vector<std::pair<std::string,Probe>> probes;
static YAML::Node events(YAML::NodeType::Sequence);
void add_probe(const std::string& name,Probe fn){probes.emplace_back(name,fn);}
YAML::Node snapshot(bool full){YAML::Node n;for(const auto& p:probes)n[p.first]=p.second(full);return n;}
void event(const YAML::Node& n){events.push_back(YAML::Clone(n));}
YAML::Node take_events(){auto n=YAML::Clone(events);events=YAML::Node(YAML::NodeType::Sequence);return n;}
}
