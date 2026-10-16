from pathlib import Path
r=Path(__file__).parent/'source/src'
p=r/'identity_log.h';s=p.read_text();needle='template<class H> inline void host_generated'
s=s.replace(needle,'''inline void lpa_event(const char* event,uint64_t t,uint64_t stream,uint64_t lpa) {
 if(state().disabled)return;
 std::cerr<<"{\\\"event\\\":\\\""<<event<<"\\\",\\\"time_ns\\\":"<<t<<",\\\"stream\\\":"<<stream<<",\\\"lpa\\\":"<<lpa<<"}\\n";
}
'''+needle);p.write_text(s)
p=r/'ssd/Address_Mapping_Unit_Page_Level.cpp';s=p.read_text()
s=s.replace('domains[stream_id]->Locked_LPAs.insert(lpa);','domains[stream_id]->Locked_LPAs.insert(lpa);\n\t\tif (itr == domains[stream_id]->Locked_LPAs.end()) Identity::lpa_event("identity_lpa_lock", Simulator->Time(), stream_id, lpa);')
s=s.replace('domains[stream_id]->Locked_LPAs.erase(itr);','domains[stream_id]->Locked_LPAs.erase(itr);\n\t\tIdentity::lpa_event("identity_lpa_unlock", Simulator->Time(), stream_id, lpa);')
p.write_text(s)
