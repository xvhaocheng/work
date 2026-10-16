from pathlib import Path
import shutil
p=Path(__file__).resolve().parent
shutil.copytree(p.parent/'source',p/'source',ignore=shutil.ignore_patterns('build','MQSim'))
r=p/'source/src'
f=r/'ssd/Address_Mapping_Unit_Page_Level.cpp';s=f.read_text();a=s.index('\tinline void Address_Mapping_Unit_Page_Level::Remove_barrier_for_accessing_lpa(');b=s.index('\n\tinline void Address_Mapping_Unit_Page_Level::Remove_barrier_for_accessing_mvpn(',a)
s=s[:a]+'''	inline void Address_Mapping_Unit_Page_Level::Remove_barrier_for_accessing_lpa(stream_id_type stream_id, LPA_type lpa)
	{
		auto itr = domains[stream_id]->Locked_LPAs.find(lpa);
		if (itr == domains[stream_id]->Locked_LPAs.end()) {
			PRINT_ERROR("Illegal operation: Unlocking an LPA that has not been locked!");
		}

		// Borrow original children; owner lists retain their completion references.
		// Detach both fixed batches before any potentially reentrant translation.
		std::list<NVM_Transaction*> resumed;
		auto reads = domains[stream_id]->Read_transactions_behind_LPA_barrier.equal_range(lpa);
		for (auto it = reads.first; it != reads.second; ++it) resumed.push_back(it->second);
		domains[stream_id]->Read_transactions_behind_LPA_barrier.erase(reads.first, reads.second);
		auto writes = domains[stream_id]->Write_transactions_behind_LPA_barrier.equal_range(lpa);
		for (auto it = writes.first; it != writes.second; ++it) resumed.push_back(it->second);
		domains[stream_id]->Write_transactions_behind_LPA_barrier.erase(writes.first, writes.second);

		domains[stream_id]->Locked_LPAs.erase(itr);
		Identity::lpa_event("identity_lpa_unlock", Simulator->Time(), stream_id, lpa);
		for (auto transaction : resumed) {
			Identity::child_live_event(transaction->Type == Transaction_Type::READ ?
				"identity_barrier_read_resume" : "identity_barrier_write_resume",
				Simulator->Time(), static_cast<NVM_Transaction_Flash*>(transaction));
		}
		if (!resumed.empty()) Translate_lpa_to_ppa_and_dispatch(resumed);
	}
''' +s[b:]
needle='''			((NVM_Transaction_Flash_WR*)transaction)->write_sectors_bitmap | domain->Get_page_status(ideal_mapping_table, transaction->Stream_id, transaction->LPA));
	}

	void Address_Mapping_Unit_Page_Level::allocate_plane_for_translation_write'''
assert needle in s
s=s.replace(needle,'''			((NVM_Transaction_Flash_WR*)transaction)->write_sectors_bitmap | domain->Get_page_status(ideal_mapping_table, transaction->Stream_id, transaction->LPA));
		if (!is_for_gc) Identity::user_allocate(Simulator->Time(), transaction, old_ppa,
			domain->Locked_LPAs.find(transaction->LPA) != domain->Locked_LPAs.end());
	}

	void Address_Mapping_Unit_Page_Level::allocate_plane_for_translation_write''')
f.write_text(s)
f=r/'ssd/NVM_PHY_ONFI_NVDDR2.cpp';s=f.read_text();s='#include "../identity_log.h"\n'+s
needle='\tvoid NVM_PHY_ONFI_NVDDR2::Send_command_to_chip(std::list<NVM_Transaction_Flash*>& transaction_list)\n\t{';assert needle in s
s=s.replace(needle,needle+'\n\t\tfor (auto transaction : transaction_list) Identity::child_live_event("identity_phy_issue", Simulator->Time(), transaction);');f.write_text(s)
f=r/'ssd/GC_and_WL_Unit_Page_Level.cpp';s=f.read_text();s='#include "../identity_log.h"\n'+s
needle='\t\t\t//Run the state machine to protect against race condition';assert needle in s
s=s.replace(needle,'\t\t\tIdentity::gc_candidate(Simulator->Time(), gc_candidate_address, block->Stream_id, block->Ongoing_user_program_count);\n'+needle);f.write_text(s)
f=r/'identity_log.h';s=f.read_text();needle='template<class T> inline void child_created'
s=s.replace(needle,'''template<class T> inline void user_allocate(uint64_t t,T* p,uint64_t old_ppa,bool locked) {
 if(state().disabled)return;
 auto i=state().children.find(p);if(i==state().children.end()) {error(t,"allocation_missing_child",p);return;}
 std::cerr<<"{\\\"event\\\":\\\"identity_user_allocate\\\",\\\"time_ns\\\":"<<t<<",\\\"child_uid\\\":"<<i->second.uid
  <<",\\\"owner_uid\\\":"<<i->second.owner<<",\\\"user_uid\\\":"<<i->second.owner<<",\\\"stream\\\":"<<p->Stream_id<<",\\\"lpa\\\":"<<p->LPA
  <<",\\\"old_ppa\\\":"<<old_ppa<<",\\\"new_ppa\\\":"<<p->PPA<<",\\\"locked\\\":"<<(locked?"true":"false")<<"}\\n";
}
template<class A> inline void gc_candidate(uint64_t t,const A& a,uint64_t stream,uint64_t count) {
 if(state().disabled)return;
 std::cerr<<"{\\\"event\\\":\\\"identity_gc_candidate\\\",\\\"time_ns\\\":"<<t<<",\\\"stream\\\":"<<stream
  <<",\\\"channel\\\":"<<a.ChannelID<<",\\\"chip\\\":"<<a.ChipID<<",\\\"die\\\":"<<a.DieID<<",\\\"plane\\\":"<<a.PlaneID
  <<",\\\"block\\\":"<<a.BlockID<<",\\\"ongoing_user_program_count\\\":"<<count<<"}\\n";
}
'''+needle);f.write_text(s)
