#!/usr/bin/env python3
"""Package actual results and immutable evidence; performs no simulation run."""
import collections,csv,hashlib,json,pathlib,shlex,sys
HERE=pathlib.Path(__file__).resolve().parent
ROOT=HERE.parents[2]
OLD=ROOT/'out/stage117-plan-revision-g0-20261013/model'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def load(p):return json.loads(p.read_text())
def put(name,value):(HERE/name).write_text(json.dumps(value,indent=2,ensure_ascii=False)+'\n')
def line(path,needle):
    for n,s in enumerate((HERE/path).read_text().splitlines(),1):
        if needle in s:return {'path':path,'line':n}
    raise AssertionError((path,needle))
contract=load(HERE.parent/'implementation-contract.json')
cases=[c['id'] for c in contract['cases']]
manifest=load(HERE/'build-inputs.json')
for ent in manifest['files']:assert sha(ROOT/ent['path'])==ent['sha256'],ent['path']
deps={}
for text in (HERE/'obj/Vnonblock_actual_dma_system_top__verFiles.dat').read_text().splitlines():
    if not text.startswith('S '):continue
    fields=shlex.split(text);p=pathlib.Path(fields[-1]);st=p.stat()
    assert st.st_size==int(fields[1]) and st.st_ino==int(fields[2]) and st.st_mtime_ns==int(fields[5])*10**9+int(fields[6]) and st.st_ctime_ns==int(fields[3])*10**9+int(fields[4]),str(p)
    deps[str(p)]={'path':str(p),'sha256':sha(p),'recorded_by':'Verilator actual S dependency; size/inode/mtime still match compile stamp'}
for d in (HERE/'obj').glob('*.d'):
    for name in shlex.split(d.read_text().replace('\\\n',' ').partition(':')[2]):
        if name.endswith(':'):continue
        p=pathlib.Path(name);p=p if p.is_absolute() else HERE/'obj'/p;p=p.resolve()
        if p.is_file():deps.setdefault(str(p),{'path':str(p),'sha256':sha(p),'recorded_by':'actual C++ compiler dependency file; system standard-library headers are excluded by compiler -MMD'})
put('actual-compiled-dependencies.json',{'scope':'Actual Verilator source/include records plus C++ compiler dependencies; compiler/tool versions separate; standard-library headers excluded by -MMD','files':list(sorted(deps.values(),key=lambda x:x['path'])),'count':len(deps)})
assert load(HERE/'results/protocol-unit.json')['pass']
assert load(HERE/'checker/checker-selftests.json')['passed']
assert load(HERE/'checker/delivery.json')['passed']
reports={case:load(HERE/'checker'/f'{case}-independent-verification.json') for case in cases}
assert all(r['passed'] for r in reports.values())
runs={case:load(HERE/'results'/case/'run.json') for case in cases}
assert all(r['pass'] for r in runs.values())
exe=load(HERE/'execution-ledger.json')
assert len(exe['runs'])==2 and all(r['returncode']==0 for r in exe['runs'])
assert len({r['binary_sha256'] for r in exe['runs']})==1
assert exe['runs'][0]['binary_sha256']==sha(HERE/'obj/Vnonblock_actual_dma_system_top')
basecost=load(OLD/'results/cost-and-parameters.json')
mmio={}
for case in cases:
    rows=list(csv.DictReader((HERE/'results'/case/'events.csv').open()))
    counts=collections.Counter((int(r['engine']),int(r['address']),int(r['n'])) for r in rows if r['event']=='mmio_return')
    mmio[case]=[{'engine':e,'register_offset':a,'write':bool(w),'transactions':n} for (e,a,w),n in sorted(counts.items())]
put('inherited-inputs.json',{'old_files_not_executed_or_modified':[{'path':str((OLD/name).relative_to(ROOT)),'sha256':sha(OLD/name)} for name in ('finite_dma.cpp','finite_hooks.inc','inherited_harness.hpp','nonblock_actual_dma_system_top.sv','prepare_build.py','verify_suite.py','build-inputs.json')],
    'unchanged_wrapper':sha(HERE/'nonblock_actual_dma_system_top.sv')==sha(OLD/'nonblock_actual_dma_system_top.sv'),
    'unchanged_bus_observer':sha(HERE/'finite_hooks.inc')==sha(OLD/'finite_hooks.inc'),
    'harness_change':'only backend access logging; see harness-observer.diff',
    'fixture_sha256':sha(ROOT/contract['fixture']['path'])})
warning_counts=collections.Counter()
for s in (HERE/'raw/build.log').read_text().splitlines():
    if s.startswith('%Warning-'):warning_counts[s.split(':',1)[0][1:]]+=1
put('build-review.json',{'pass':True,'production_source_files_match_frozen_manifest':True,'compiled_inputs_count':len(manifest['files']),
    'actual_compiled_dependency_count':len(deps),'actual_compiled_dependency_manifest':'actual-compiled-dependencies.json',
    'binary_sha256':sha(HERE/'obj/Vnonblock_actual_dma_system_top'),'unit_binary_sha256':sha(HERE/'protocol_unit'),
    'verilator':'4.038 2020-07-11 rev v4.036-114-g0cd4a57ad','compiler':'g++ Ubuntu 11.4.0-1ubuntu1~22.04',
    'warnings':dict(warning_counts),'warning_policy':'Inherited -Wno-fatal; preserved full build log. No claim of warning-free RTL.',
    'ASSERTS_OFF_inherited':True,'active_checks':'Explicit held-valid, capture/ID, R/W/LAST/B, exact-byte and protocol-state checks plus independent raw reconstruction.'})
cost={
 'schema':'packet-protocol-cost-boundaries-v1','scope':'bounded protocol function; not G0 or performance comparison',
 'mapping':contract['owner_and_mapping'],'parameters':basecost['parameters'],
 'hardware_budget':{k:v for k,v in basecost['hardware_budget'].items() if k!='active_engines_solo_or_single'},
 'active_resource_by_case':{c:basecost['active_resource_by_case'][c] for c in cases},
 'cache_backend':'MODEL_UNCACHED_ASSUMPTION', 'actual_CPU_or_cache_instantiated':False,
 'logical_owner_and_alias':'Single C++ owner addresses behavioral memory at PA=0x80000000; DMA uses original IOVA=0x10000000 and shared DID10/11 context. Metadata/used state/tokens are host heap outside DMA pool. This is not an actual CPU VA/PA alias or platform metadata layout proof.',
 'host_instrumentation_memory':{'peak_bytes_measured':None,'kind':'C++ behavioral memory map, original/expected/final vectors, packet record/guard snapshots, access logs and protocol metadata','DMA_staging_allocation_added':False,'limit':'These are validation/instrumentation host storage; no measured native scratch footprint or hidden extra DMA buffer claim.'},
 'line_closures_enforced':'Spec immutable per-packet descriptor-capacity 16B closures; every validation/consumer record/guard block checked by Protocol.allow_read before memory access.',
 'application_RTL_latency':None,'cache_RTL_latency':None,'software_RTL_latency':None,'actual_CVA6_cache_validated':False,
 'logical_costs_per_case':{c:runs[c]['logical_costs'] for c in cases},
 'actual_MMIO_transactions_by_register':mmio,
 'logical_protocol_operations_per_case':{'span_NEXT_bindings':4,'span_DONE_joins':4,'packet_allspans_retirements':3,'model_acquires':3,'full_record_validations':3,'prefix_used_publications':3,'packet_consumers':3,'B_returns':3,'A_reuses':3,'final_checked_teardown':1},
 'all_actual_host_memory_operation_counts':{c:reports[c]['memory'] for c in cases},
 'packet_source_holds':{c:reports[c]['memory']['packets'] for c in cases},
 'host_ns_execution_cost_only':{c:runs[c]['host_ns_execution_cost_only'] for c in cases},
 'external_process_wall_seconds':{r['case']:r['elapsed_wall_seconds'] for r in exe['runs']},
 'RTL_build_wall_seconds':load(HERE/'results/build.json')['elapsed_wall_seconds'],
 'cost_boundaries':[
  'All 36 header bytes prepared before first DMA. Publication modifies host protocol metadata only, never payload/header during peer DMA.',
  'Prepublication controlled-success validation reads 1675 record+133 local guard bytes. Actual consumer separately reads 1675 record+133 guard bytes. These are two distinct logical passes, not a free completion oracle.',
  'Each A source stays retained through all-packet public DONE and prepublication validation; exact retired A source bytes are then overwritten (1639B) before consumer reads B. Source and B lifetimes are distinct.',
  'Host packet work does not call tick: hardware is paused between public MMIO steps. Same diagnostic cycle numbers for acquire/validation/consumer do not imply zero CPU/cache/application cost or measured simultaneous hardware/software throughput.',
  'Hardware protocol timestamps prove ordering and a real peer backend-active state at handoff. They cannot measure application latency or saved hardware time.',
  'Host nanoseconds include logging/checking and measure this simulation process only; whole_main excludes later run.json output/destruction. External process wall time includes process exit.',
  'All whole-pool runtime reads (16384B) and final table reads (12288B) occur after checked global quiet. Initial.bin serializes the constructed initial vector, not a runtime memory read.',
  'Resident mapping is not revoked at B return. Final public completion, debug/queue checked quiet, raw conservation and whole-memory guards precede logical teardown.',
  'Unknown NEXT/public DONE, timeout or thrown integrity/ownership failure takes retain/quarantine path and prevents further controller operations. Unit faults simulate protocol errors; no production AXI exception recovery claim.'
  ,'Logical retained/quarantine state does not prove real-driver allocation lifetime or persistent isolation after this model process exits. OS process cleanup is outside the protocol ownership model.'
  ,'B is read through the modeled original-cookie descriptor-chain consumer; there is no native guest, vhost, DPDK consumer or real CPU cache in this execution.'
 ],
 'excluded_or_unmeasured':['actual fence.i/cache invalidation/refill','CVA6 software instruction/MMIO/poll execution cost','native VM/DPDK port','physical hugepage allocation/alias/metadata binding','Linux DMA pin/map/unmap/invalidation cost','production RRESP/BRESP/IOMMU fault-to-packet success receipt','FPGA frequency/timing/area/power/board performance'],
 'same_original_latency_configuration':True,'new_pressure_arrivals_deadlines_or_extra_packets':False}
put('cost-and-parameters.json',cost)
ledger={'schema':'packet-protocol-complete-execution-ledger-v1',
 'agents':[{'task':'/root/s117_protocol_impl_rtl','model_requested_by_parent':'gpt-6-astra','reasoning':'ultra','scope':'exclusive protocol implementation/unit/isolated RTL build/run and delivery; runtime model identity accepted by root separately'},
           {'task':'/root/s117_protocol_impl_rtl/independent_checker','model':'gpt-6.1-sol','reasoning':'ultra','fork_turns':'none','scope':'independent original-JSON/raw-AXI/host-access checker only'}],
 'external_models':{'DeepSeek':False,'Claude':False},
 'unit_compile_attempts':[{'attempt':1,'returncode':1,'observed_tool_wall_seconds':0.475890092,'command':'g++ -std=c++17 -O2 -Wall -Wextra -Werror -o protocol_unit protocol_unit.cpp','failure':'two -Werror=misleading-indentation diagnostics in teardown/reuse_generation; corrected with separate statements','diagnostics':'raw/unit-first-compile-diagnostics.txt'},
                          {'attempt':2,'returncode':0,'observed_tool_wall_seconds':0.624590484,'command':'g++ -std=c++17 -O2 -Wall -Wextra -Werror -o protocol_unit protocol_unit.cpp'}],
 'protocol_unit_execution':{'command':'./protocol_unit > results/protocol-unit.json','returncode':0,'count':load(HERE/'results/protocol-unit.json')['count'],'runs':1,'wall_seconds':None},
 'RTL_builds':[load(HERE/'results/build.json')],
 'RTL_integrations':exe['runs'],'RTL_execution_retries':0,'old_solo_matrix_or_validator_execution_count':0,
 'independent_checker_ledger':'checker/execution-ledger.json',
 'packaging':{'script':'finalize_delivery.py','simulations_executed':0,'existing_compiled_hashes_and_results_validated':True,
              'attempts':[{'attempt':1,'returncode':0,'reason':'Initial stable delivery and all-artifact hashes'},
                          {'attempt':2,'returncode':1,'reason':'Added actual compiler dependency manifest; packaging parser initially mistook Verilator ctime fields for mtime on /usr/bin/verilator_bin. No compiled source changed; no simulations run.'},
                          {'attempt':3,'returncode':0,'reason':'Corrected actual dependency timestamp fields; all size/inode/mtime/ctime matches verified. Final package includes .svh and compiler-recorded headers.'}]}}
put('complete-execution-ledger.json',ledger)
code={
 'protocol_input_and_closure_invariants':line('packet_protocol.hpp','explicit Protocol'),
 'full_engine_local_completion_receipt':line('packet_protocol.hpp','bool complete('),
 'model_cache_acquire':line('packet_protocol.hpp','Token acquire('),
 'per_access_ownership_and_closure_enforcement':line('packet_protocol.hpp','void allow_read('),
 'prefix_publication':line('packet_protocol.hpp','void publish('),
 'consumer_then_B_return':line('packet_protocol.hpp','void return_B('),
 'quarantine_guard':line('packet_protocol.hpp','void live()'),
 'all_controller_memory_access_sites':line('packet_dma.cpp','class HostMemory'),
 'cookie_chain_consumer':line('packet_dma.cpp','static std::vector<uint8_t> receive_record'),
 'local_guard_consumer':line('packet_dma.cpp','static std::vector<uint8_t> receive_guards'),
 'header_before_DMA':line('packet_dma.cpp','memory.write("header_prepare"'),
 'public_NEXT_issuer':line('packet_dma.cpp','auto launch='),
 'prefix_acquire_validation_consume_return':line('packet_dma.cpp','auto process_prefix='),
 'public_DONE_join_then_successor_launch':line('packet_dma.cpp','done_register_matched'),
 'whole_quiet_only_terminal_observers':line('packet_dma.cpp','const long last_poll='),
 'failure_retain_path':line('packet_dma.cpp','protocol.uncertain(e.what())'),
 'behavioral_transport_read_logging':line('inherited_harness.hpp','if(transport_trace) std::fprintf(transport_trace,"%ld,read'),
 'behavioral_transport_write_logging':line('inherited_harness.hpp','if(transport_trace) std::fprintf(transport_trace,"%ld,write'),
 'protocol_directed_units':line('protocol_unit.cpp','int main()'),
 'independent_original_fixture_reconstruction':line('checker/verify_protocol.py','def oracle()'),
 'independent_bus_retirement':line('checker/verify_protocol.py','def check_bus('),
 'independent_actual_host_accesses':line('checker/verify_protocol.py','def check_host(')}
put('evidence-index.json',{'source_entry_points':code,'new_raw_results':{c:{'events':f'results/{c}/events.csv','actual_controller_accesses':f'results/{c}/host_access.jsonl','actual_backend_transport_accesses':f'results/{c}/backend_access.csv','independent_verification':f'checker/{c}-independent-verification.json','packet_snapshot_pattern':f'results/{c}/packet_<id>_<validation|consumer|validation_guards|guards>.bin','final_pool_snapshot':f'results/{c}/final.bin'} for c in cases}})
delivery={'schema':'c117r3-packet-protocol-implementation-delivery-v1','status':'complete for parent critical acceptance; parent owns shared documentation and main-goal completion',
 'summary':'Original three packets complete/acquire/prefix-publish/consume/B-return correctly in both ordinary FIFO-ahead cases. Parallel trace independently proves two complete packets consumed while the other real DMA backend remains active.',
 'checker_selftests':{'pass':True,'negative_cases':14,'executor_flag_independence_positive_control':1,'total':15},
 'implemented':True,'unit_tests_pass':True,'protocol_unit_count':load(HERE/'results/protocol-unit.json')['count'],
 'independent_cases':{c:{'pass':reports[c]['passed'],'packet_count':3,'payload_bytes':1639,'record_bytes':1675,'raw_bus':reports[c]['raw_bus'],'peer_active_witnesses':reports[c]['real_backend_active_peer_witnesses'],'evidence':f'checker/{c}-independent-verification.json'} for c in cases},
 'source_entries':code,'contract_sha256':sha(HERE.parent/'implementation-contract.json'),
 'build_input_manifest':'build-inputs.json','actual_compiled_dependency_manifest':'actual-compiled-dependencies.json','binary_path':'obj/Vnonblock_actual_dma_system_top','binary_sha256':sha(HERE/'obj/Vnonblock_actual_dma_system_top'),
 'raw_and_local_final_snapshots':'results/','unit_test_result':'results/protocol-unit.json','independent_checker_delivery':'checker/delivery.json',
 'parameters_and_costs':'cost-and-parameters.json','actual_execution_ledger':'complete-execution-ledger.json','evidence_index':'evidence-index.json',
 'accepted_scope':'Same-owner controlled generated-fixture protocol function in MODEL_UNCACHED_ASSUMPTION only; no CPU/cache/application RTL latency claimed.',
 'unresolved':['Actual CVA6/cache/CPU alias and metadata ownership binding','Production per-packet successful-byte/fault correlation and physical release path; logical quarantine does not persist across model process exit','Native guest consumer and software costs, source-backed G0 target/comparisons, and any FPGA performance remain unmeasured','Peak host instrumentation/validation scratch footprint unmeasured; no additional DMA staging allocation'],
 'no_production_ABI_driver_RTL_change':True,'no_old_result_mutation_or_old_case_replay':True,'no_new_source_pressure_or_fourth_packet':True,
 'no_new_G0_innovation_or_board_claim':True,'no_DeepSeek_or_Claude':True,'main_goal_completed_by_this_agent':False,
 'authors':'Codex implementation seat authored/applied/executed; separate Codex Sol/ultra checker seat independently authored/executed raw evidence acceptance. No external model worker.'}
put('delivery.json',delivery)
files=[p for p in HERE.rglob('*') if p.is_file() and p.name!='artifact-sha256.json' and '__pycache__' not in p.parts]
put('artifact-sha256.json',{'schema':'implementation-all-artifact-hashes-v1','files':[{'path':str(p.relative_to(HERE)),'bytes':p.stat().st_size,'sha256':sha(p)} for p in sorted(files)],'count':len(files),'excludes':['artifact-sha256.json itself','Python bytecode cache']})
print(json.dumps({'delivery':'delivery.json','pass':True,'artifacts_hashed':len(files),'cases':cases,'protocol_unit_count':delivery['protocol_unit_count']}))
