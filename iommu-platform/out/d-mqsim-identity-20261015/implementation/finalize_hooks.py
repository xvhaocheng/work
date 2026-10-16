from pathlib import Path
p=Path(__file__).parent/'source/src/ssd/Address_Mapping_Unit_Page_Level.cpp';s=p.read_text()
for kind in ['read','program']:
 needle=f'domains[stream_id]->Waiting_unmapped_{kind}_transactions.insert('
 lines=s.splitlines(True);out=[]
 for line in lines:
  if needle in line:out.append(line[:len(line)-len(line.lstrip())]+'Identity::child_live_event("identity_mapping_wait_enter", Simulator->Time(), transaction);\n')
  if f'_my_instance->domains[transaction->Stream_id]->Waiting_unmapped_{kind}_transactions.erase(it2++);' in line:out.append(line[:len(line)-len(line.lstrip())]+'Identity::child_live_event("identity_mapping_wait_exit", Simulator->Time(), it2->second);\n')
  out.append(line)
 s=''.join(out)
needle='\t\t//First check if the transaction source is Mapping Module';s=s.replace(needle,'\t\tIdentity::child_live_event("identity_amu_callback", Simulator->Time(), transaction);\n'+needle)
needle='\t\tWrite_transactions_for_overfull_planes[transaction->Address';s=s.replace(needle,'\t\tIdentity::child_live_event("identity_overfull_wait_enter", Simulator->Time(), transaction);\n'+needle)
needle='\t\t\t\twaiting_write_list.erase(program++);';s=s.replace(needle,'\t\t\t\tIdentity::child_live_event("identity_overfull_wait_exit", Simulator->Time(), *program);\n'+needle)
p.write_text(s)
