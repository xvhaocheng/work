#pragma once
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
