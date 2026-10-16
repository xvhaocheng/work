set pagination off
set confirm off
set print elements 8
set print pretty on
break collect_results
run
printf "\nD_DIAGNOSTIC_SNAPSHOT_AFTER_ENGINE_RETURN\n"
p host.IO_flows[0]->STAT_generated_request_count
p host.IO_flows[0]->STAT_serviced_request_count
p host.IO_flows[0]->nvme_software_request_queue
set $dftl = (SSD_Components::FTL*)ssd.Firmware
set $damu = (SSD_Components::Address_Mapping_Unit_Page_Level*)$dftl->Address_Mapping_Unit
p $damu->domains[0]->Locked_LPAs
p $damu->domains[0]->Locked_MVPNs
p $damu->domains[0]->Read_transactions_behind_LPA_barrier
p $damu->domains[0]->Write_transactions_behind_LPA_barrier
p $damu->domains[0]->ArrivingMappingEntries
p $damu->Write_transactions_for_overfull_planes[0][0][0][0]
p *ssd.Host_interface
p *$dftl->TSU
quit
