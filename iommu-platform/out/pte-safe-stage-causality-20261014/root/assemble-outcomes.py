"""Deliver the bounded causal-gate result; no model/build/test replay."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re

W=Path('/home/xvhaocheng/work');O=W/'iommu-platform/out/pte-safe-stage-causality-20261014';M=O/'model'
now=datetime.now(timezone.utc).isoformat()
def save(name,x):(O/name).write_text(json.dumps(x,ensure_ascii=False,indent=2)+'\n')
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
analysis=json.loads((O/'root/prefix-analysis.json').read_text())
independent=json.loads((O/'ar-cause/raw-phase-verification.json').read_text())
assert independent['passed_critical_raw_checks'] and not independent['issues']

native_paths={
'iommu-platform/tests/nonblock_actual_dma_test.cpp':[(647,720),(829,842),(1290,1305),(1331,1399),(1500,1532)],
'iommu-platform/tests/nonblock_actual_dma_system_top.sv':[(391,480),(885,924)],
'iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv':[(384,409),(507,533)],
'iommu-platform/cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv':[(222,280)],
'iommu-platform/cva6/vendor/pulp-platform/axi/src/axi_delayer.sv':[(128,145)],
'iommu-platform/cva6/vendor/pulp-platform/common_cells/src/stream_delay.sv':[(45,97)],
'iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv':[(254,270)],
'iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv':[(517,532)],
'iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv':[(149,151),(517,535)],
'iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv':[(409,465)],
'iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv':[(347,374),(538,561),(600,617)],
'iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/rr_arb_tree.sv':[(145,196),(203,221)]
}
files=[]
for name,spans in native_paths.items():
 p=W/name;s=p.read_text().splitlines();files.append({'path':name,'sha256':digest(p),'ranges':[{'start':a,'end':b,'text':'\n'.join(f'{i}: {s[i-1]}' for i in range(a,b+1))+'\n'} for a,b in spans]})
save('root/source-snapshot.json',{'schema':'safe-root-critical-source-v1','created_utc':now,'files':files,'old_results_not_replayed':True})

# Actual Verilator source/include inputs; generated outputs and system tool binaries omitted.
vf=M/'obj/Vsafe_pte_stage_top__verFiles.dat';deps=[]
for line in vf.read_text().splitlines():
 if not line.startswith('S '):continue
 names=re.findall(r'"([^"]+)"',line)
 if not names:continue
 p=Path(names[-1])
 if str(p).startswith(str(W)) and p.suffix in ['.sv','.svh','.cpp','.h'] and p.exists():deps.append(p)
deps=sorted(set(deps));manifest=json.loads((M/'build-manifest.json').read_text())
save('root/actual-build-dependencies.json',{'schema':'safe-actual-build-dependencies-v1','source':'model/obj/Vsafe_pte_stage_top__verFiles.dat','explicit_SV_inputs':len(manifest['sources']),'actual_Verilator_source_include_records':len(deps),'files':[{'path':str(p),'sha256':digest(p)} for p in deps],'new_CPP':{'path':str(M/'safe_pte_stage_probe.cpp'),'sha256':digest(M/'safe_pte_stage_probe.cpp')},'count_limit':'S records are actual Verilator source/include inputs, not complete C++/system-header/host dependency count','rebuild_executed':False})

save('root/observation-ownership-cost-card.json',{
 'schema':'safe-stage-full-cost-boundary-v1','created_utc':now,
 'actual_probe':{'passive_tap_fields':37,'declared_tap_output_bits':637,'additional_RTL_registers':0,'added_payload_capacity':0,'NormalTL_AR_selector_changed':False,'forecast_logic_implemented':False,'host_run_seconds':json.loads((M/'run-result.json').read_text())['elapsed_host_seconds'],'timing_area_limit':'declared simulation views include padded constants; physical fanout/read/clock/area and end-to-end CPU effects unmeasured; no zero-engineering-cost inference'},
 'native_retained':['onePTW','IoTLB16','fullwalk','native NativeN and raw coupled metadata','two liveDMA buffer512 each','perbank4 outstanding/16W FIFO','all originalspills/cuts/fullID/sameID/heldVALID/WLAST/B/fault lifetime'],
 'owned_obligations':'one actualjob each,DID10/11,privateIOMMUID0/4,DDRpayload32/36; perjob2AR/2AW/512R/512W/2B; actual internalAW vs physicalAW distinct; no newtransaction budget',
 'observation_costs':['637-bit simulation view and host percycle logging','original full sourceID/job/context/IOVA/burst/remainingbytes binding','PTE generation/internalaccept/delayeraccept/sharedmuxaccept/DDRaccept/firstRVALID/handshake/PTWconsume ports and phase tags','existing slot/ARcredit/owner/R/W/B/fault tracking and accessible read paths','full completion timestamps rather than DONEpoll upperbounds','exact currentowned snapshots and completed-only history tags/valid/saturation','future predictive dual-order arithmetic/shadows/ports/select delay and held/fair life if later admitted','complete prospective ordinary DS-AR priority/masks/8bit declared selector state/fairness and native record reuse','all CPU/control/clock/physical memory/application costs'],
 'ordinary_DS_AR':'ar-cause/ordinary-ar-contract.json; 8-bit declared absolute selector state is a prospective encoding, not measured totaldelta or proven equally-timed implementation',
 'predicted_gain_and_cost':{'ABBA_delta':None,'online_gain':None,'new_F1_bits':None,'history_bytes':None,'selection_cycles':None,'observer_routing':None,'area':None,'Fmax':None,'energy':None,'CPU':None,'application_gain':None},
 'measurement_limits':['raw WDATA/WSTRB/functionalIDs/creditregisters absent; actual main byte comparison + aligned source + ordered handshakes support bounded normalcopy, no independent perbeat strobe claim','no new error injection/general fault validation','model service law/head preparation31 and DS50/50 are conditional, not FPGA measurements'],
 'complete_cost_gate':'not passed: no legal two-head action or causal positive in this prefix; numeric F1 observer/predictor/select freeze not authorized before difference',
 'reuse':'prior nine online ordinary controls and37 cost classes remain; no old counter/model/validator replay or R2 cost architecture inherited'
})

save('root-result-acceptance.json',{
 'schema':'safe-PTE-causal-root-acceptance-v1','created_utc':now,
 'status':'bounded causal gate delivered; fixed natural prefix not admitted for safe-head ABBA',
 'root_responsibility':'fresh/R/PTW/source admission, passive probe architecture and execution, direct code/source review, event/complete-cost and final verdict',
 'gates':{'native_common_two_safe_point':'one new fixedprefix captured0 anydualVALID/fresh-safe; local e=a+3 sufficient source condition retained, global native reachability unknown','PTE_phase_cause':'12 accuratelymatched PTEs observed0/56/88 relative phase; no sharedARcoVALID/held/PTEaccept stall, payloadinflight positive; not a controlledABBA causality theorem','ordinary_comparison':'currentVALID DS-first with fixed4 fairness completecontract covers unlocked RR displacement; onegrant nativehistory and2 includingprelock source bounds, not tested wholeF1 equivalence','full_normal_ownership':'actual4096B each source/dest bytecheck,2AR/2AW/512R/512W/2B each,zeroresponseerror andfullquiet; rawstrobe/data/functionalID limitations explicitly retained','four_performance_conditions':'not admitted,0 executed; F1 onlinepredictor/selector not implemented'},
 'new_evidence':{'one_new_passive_native_run':1,'continuous_rows':4151,'accurate_PTE_records':12,'fresh_any':0,'fresh_safe_READ_READ':0,'sharedAR_comp_DS_coVALID':0,'max_preedge_DDRqueue':3,'held_R_by_fullID':{'49_DDTW':208,'48_PTW':0,'32_or36_payload':0},'PTE_intervals':{'consume_to_nextbirth':1,'birth_to_internalaccept':0,'internalaccept_to_DDRaccept':56,'DDRaccept_to_PT Wconsume'.replace(' ',''):88},'actual_task_jobquiet_from_common_t0':[3190,4145],'allquiet_from_common_t0':4145,'DONE_poll_upperbounds':[3196,4150],'main_quiet_poll_return_upperbound':4151},
 'source_new_boundaries':['actual mixed vendor dependencies pinned','no free total8read credit: per-source16 localID/3bit demuxcounter full7, muxMaxWTrans8, CPPqueue16','internal DS accept !=DDRaccept; nominal50 source-derived stage52 and empty path56 matched by newtrace','safe payloadcapacity does not preventDSreturn self-backpressure; 208held DDTW cycles notpayloadPTEfeedback'],
 'accepted_artifacts':['native-choice/native-choice.md','native-choice/verdict.json','ar-cause/ar-cause.md','ar-cause/ordinary-ar-contract.json','ar-cause/raw-phase-verification.json','model/prefix.csv','model/run-result.json','root/prefix-analysis.json','root/PTE-stage-events.json','root/R-stage-and-gate.md','root/observation-ownership-cost-card.json'],
 'execution':{'new_private_passive_RTL_runs':1,'ABBA_runs':0,'new_performance_conditions':0,'new_build_attempts':4,'new_failed_build_attempts':3,'new_successful_builds':1,'old_experiment_replays':0,'old_build_replays':0,'old_validator_replays':0,'new_unit_test_suites':0,'new_production_RTL_driver_ABI_changes':0,'board_synthesis_runs':0,'added_capacity_walker_pressure':0,'external_models':0,'old_seats_restored':0},
 'diagnostics':'All3 build failures retained: missing nativeDMAwrapper dependency, passive ARgen_mux scope, Tb cycle member reference. Each nextbuild changed required input; no unchanged build or experiment rerun. Exactly1run passed.',
 'decision_scope':{'this_prefix':'close as a safe-dual-head/PTE-phase positive; sourcegate lacks legitimate ABBA action, no phasecontention witness','whole_F1_family':'open; another fixed legitimate prefix and generalservicefeedback unknown','PTE_phase_global_invariance':'not claimed','published_whole_coverage_or_novelty':'not claimed; priorprimarycomponent review retained','R2_R107_C112_and_old_closed_branches':'unchanged,not restored'},
 'unknowns':['fixed lawful normaljob input timing giving actual source synchronousfresh capture without phasegrid/debugkick/artificialhold','same-PTE relativeABBA difference outside ordinarycurrentVALID priority/precommit bounds','complete equallypaid onlineimplementation/select timing cost','physicalMIG/boardservice and applicationgain'],
 'completion':'after five docs sync and new delivery verification, actual complete receipt saved separately','paper_complete':False
})

save('next-goal-contract.json',{
 'schema':'safe-pte-next-contract-v1','status':'round ends; no automatic nextgoal/prefix sweep/implementation','next_unique_input_required':'one explicit fixed lawful source/job readiness and native capture proof satisfying the already given dualREAD synchronization, with allcarriedspills/IDs/bytes and pre-firstVALID commitment; cannot assume the current fixedprefix has a choice','new_cause_required':'same-PTE relative phasewait outside ordinarycurrentVALID DS-priority coverage, with full ownedcomplete gain net of inputrelease/observation/selection cost','do_not':['rerun this2jobprefix with more taps','phase/start-delay search toforce a tie','resumeoldsmallbuffer/turn/closedbranches','infer universal nofeedback from one trace','claim ordinarygap utilization as PTE waitingfeedback','auto change paperdirection'],'if_difference_survives':'one frozen onlineprototype <=4 newconditions positive/legalneighbor/identityswap/alternatelegalRW with bytes/PTE/R/W/B/fault/quiet/fullcost','after_survival':'one GPU/AIpriority real sharedtranslation workflow then RTL/FPGA keyvalidation','preserve':'8/9month foundation,NativeN/allhardwareassets,all oldrawresults,originalcumulativepaperclock','models':'Codex-onlygpt-6.1-sol ultra/native; necessaryfresh exclusiveforknone seats only; no addedtokenbudget','paper_complete':False})

save('root/execution-ledger.json',{
 'schema':'safe-phase-execution-ledger-v1','created_utc':now,'actual_goal_registration':'goal-registration.json; no token_budget','authors':['root Solultra criticalarchitecture/R/source/admission/newprobe/code/execution/finalverdict','new native-choice Solultra source seat','new ar-cause Solultra source/control plus within-goal raw-phase continuation'],
 'new_model_execution':'one passive RTLnativeprefix only; model/run-result.json and rawCSV; no oldrun_scenario/run_full_batch/warm/batch called','newbuilds':{'attempts':4,'failed':3,'passed':1,'records':['model/build-result.json.attempt1','model/build-result.json.attempt2','model/build-result.json.attempt3','model/build-result.json'],'unchanged_input_build_replay':0},
 'new_native_prefix_runs':1,'ABBA_runs':0,'performance_condition_runs':0,'old_experiment_replays':0,'old_validator_replays':0,'old_build_replays':0,'production_changes':0,'board_operations':0,'extra_capacity_orpressure':0,'old_seat_restores':0,
 'new_raw_binding_checks':'root single data binding/check + independentAR rawparse, no model repeat; purpose direct fullID/phase/owner/LAST/B/data/held/quiet checks','stage_result':'currentfixedprefix no legal safe dualpoint, phasecause unestablished; no performanceexpansion','paper_complete':False})
print('Stable root source/phase/cost/verdict/ledger delivered; no model or build rerun.')
