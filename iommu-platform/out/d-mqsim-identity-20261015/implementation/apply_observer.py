from pathlib import Path
root=Path(__file__).parent/'source/src'
def edit(name,old,new):
 p=root/name;s=p.read_text();assert old in s,(name,old);p.write_text(s.replace(old,new))
def include(name):
 p=root/name;s=p.read_text();p.write_text('#include "../identity_log.h"\n'+s)
for f in ['host/IO_Flow_Trace_Based.cpp','host/IO_Flow_Base.cpp','ssd/Host_Interface_NVMe.cpp','ssd/NVM_Transaction_Flash.cpp','ssd/NVM_PHY_ONFI.cpp','ssd/Address_Mapping_Unit_Page_Level.cpp','ssd/TSU_Base.h','ssd/TSU_FLIN.cpp','ssd/Data_Cache_Manager_Base.cpp']:include(f)
edit('host/IO_Flow_Trace_Based.cpp','STAT_generated_request_count++;','STAT_generated_request_count++;\n\tIdentity::host_generated(Simulator->Time(), request, flow_id);')
edit('host/IO_Flow_Base.cpp','\t\treturn sqe;','\t\tIdentity::sqe_bind(Simulator->Time(), sqe, request);\n\t\treturn sqe;')
edit('host/IO_Flow_Base.cpp','\t\tnvme_software_request_queue.erase(cqe->Command_Identifier);','\t\tIdentity::host_completed(Simulator->Time(), request, cqe);\n\t\tnvme_software_request_queue.erase(cqe->Command_Identifier);')
edit('ssd/Host_Interface_NVMe.cpp','\t\t((Input_Stream_Manager_NVMe *)(hi->input_stream_manager))->Handle_new_arrived_request(new_request);','\t\tIdentity::user_bind(Simulator->Time(), new_request, sqe);\n\t\t((Input_Stream_Manager_NVMe *)(hi->input_stream_manager))->Handle_new_arrived_request(new_request);')
edit('ssd/Host_Interface_NVMe.cpp','\tcqe->Command_Identifier = ((Submission_Queue_Entry *)request->IO_command_info)->Command_Identifier;','\tcqe->Command_Identifier = ((Submission_Queue_Entry *)request->IO_command_info)->Command_Identifier;\n\tIdentity::cqe_send(Simulator->Time(), request, cqe);')
edit('ssd/NVM_Transaction_Flash.cpp','\t{\n\t}', '\t{\n\t\tIdentity::child_created(Simulator->Time(), this);\n\t}')
edit('ssd/NVM_PHY_ONFI.cpp','\t\tfor (std::vector<TransactionServicedHandlerType>::iterator','\t\tIdentity::child_event("identity_phy_broadcast", Simulator->Time(), transaction);\n\t\tfor (std::vector<TransactionServicedHandlerType>::iterator')
edit('ssd/NVM_PHY_ONFI.cpp','\t\tdelete transaction;', '\t\tIdentity::child_event("identity_phy_delete", Simulator->Time(), transaction);\n\t\tdelete transaction;')
edit('ssd/Address_Mapping_Unit_Page_Level.cpp','\t\tstd::pair<LPA_type, NVM_Transaction_Flash*> entry(transaction->LPA, transaction);','\t\tIdentity::child_event("identity_barrier_enter", Simulator->Time(), transaction);\n\t\tstd::pair<LPA_type, NVM_Transaction_Flash*> entry(transaction->LPA, transaction);')
for kind in ['read','write']:
 edit('ssd/Address_Mapping_Unit_Page_Level.cpp',f'\t\t\thandle_transaction_serviced_signal_from_PHY((*{kind}_tr).second);',f'\t\t\tIdentity::child_event("identity_barrier_{kind}_release", Simulator->Time(), (*{kind}_tr).second);\n\t\t\thandle_transaction_serviced_signal_from_PHY((*{kind}_tr).second);')
 edit('ssd/Address_Mapping_Unit_Page_Level.cpp',f'\t\t\tdelete (*{kind}_tr).second;',f'\t\t\tIdentity::child_event("identity_barrier_{kind}_delete", Simulator->Time(), (*{kind}_tr).second);\n\t\t\tdelete (*{kind}_tr).second;')
for f in ['ssd/TSU_Base.h','ssd/TSU_FLIN.cpp']:
 edit(f,'\t\ttransaction_receive_slots.push_back(transaction);','\t\tIdentity::child_event("identity_tsu_submit", Simulator->Time(), transaction);\n\t\ttransaction_receive_slots.push_back(transaction);')
edit('ssd/Data_Cache_Manager_Base.cpp','\t\tfor (std::vector<UserRequestServicedSignalHanderType>::iterator','\t\tIdentity::user_event("identity_cache_owner_complete", Simulator->Time(), nvm_transaction);\n\t\tfor (std::vector<UserRequestServicedSignalHanderType>::iterator')
