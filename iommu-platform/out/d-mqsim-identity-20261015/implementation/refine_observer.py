from pathlib import Path
r=Path(__file__).parent/'source/src'
p=r/'identity_log.h';s=p.read_text();mark='template<class T> inline void child_created'
insert='''inline void child_retire(const char* event,uint64_t t,const void* p) {
 child_event(event,t,p);state().children.erase(p);
}
template<class T> inline void child_live_event(const char* event,uint64_t t,T* p) {
 // Scalar fields initialized by both base constructors; never dereference owner.
 child_event(event,t,p);
 auto i=state().children.find(p);if(i==state().children.end())return;
 std::cerr<<"{\\\"event\\\":\\\"identity_child_position\\\",\\\"time_ns\\\":"<<t<<",\\\"child_uid\\\":"<<i->second.uid<<",\\\"lpa\\\":"<<p->LPA<<",\\\"ppa\\\":"<<p->PPA<<"}\\n";
}
'''
s=s.replace(mark,insert+mark);p.write_text(s)
for f in ['ssd/NVM_PHY_ONFI.cpp','ssd/Address_Mapping_Unit_Page_Level.cpp','ssd/TSU_Base.h','ssd/TSU_FLIN.cpp']:
 p=r/f;s=p.read_text()
 for event in ['identity_phy_broadcast','identity_barrier_enter','identity_barrier_read_release','identity_barrier_write_release','identity_tsu_submit']:
  s=s.replace('Identity::child_event("'+event+'"','Identity::child_live_event("'+event+'"')
 for event in ['identity_phy_delete','identity_barrier_read_delete','identity_barrier_write_delete']:
  s=s.replace('Identity::child_event("'+event+'"','Identity::child_retire("'+event+'"')
 p.write_text(s)
# Cache removal is a model action independent of deletion, and must be named accordingly.
p=r/'ssd/Data_Cache_Manager_Flash_Advanced.cpp';s=p.read_text();s='#include "../identity_log.h"\n'+s
s=s.replace('user_request->Transaction_list.erase(it++);','Identity::child_event("identity_cache_detach", Simulator->Time(), *it);\n\t\t\tuser_request->Transaction_list.erase(it++);')
s=s.replace('transaction->UserIORequest->Transaction_list.remove(transaction);','Identity::child_live_event("identity_cache_child_serviced", Simulator->Time(), transaction);\n\t\t\t\t\ttransaction->UserIORequest->Transaction_list.remove(transaction);')
p.write_text(s)
