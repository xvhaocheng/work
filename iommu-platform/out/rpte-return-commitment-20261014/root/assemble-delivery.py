"""Write this new bounded source-gate delivery. No simulator, test, build or old checker."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json

W = Path('/home/xvhaocheng/work')
O = W / 'iommu-platform/out/rpte-return-commitment-20261014'
stamp = datetime.now(timezone.utc).isoformat()

def save(name, data):
    p = O / name
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(json.dumps(data, ensure_ascii=False, indent=2) + '\n')

def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()

# Only the exact root-critical boundaries inspected this round are quoted here.
ranges = {
    'iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv': [(275,279),(384,409),(474,524)],
    'iommu-platform/cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv': [(81,108),(249,262)],
    'iommu-platform/tests/nonblock_actual_dma_test.cpp': [(647,720),(829,834),(1168,1175),(1290,1305),(1331,1399)],
    'iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv': [(158,164),(243,277),(379,391),(549,586)],
    'iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv': [(625,690),(714,725)],
    'iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/stream_arbiter_flushable.sv': [(35,53)],
    'iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/rr_arb_tree.sv': [(145,172)],
    'iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv': [(519,532),(811,858)],
    'iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv': [(459,469)],
    'iommu-platform/cva6/vendor/pulp-platform/axi/src/axi_mux.sv': [(207,289)],
}
sources=[]
for name, spans in ranges.items():
    p=W/name
    lines=p.read_text().splitlines()
    sources.append({'path':name,'sha256':sha(p),'line_count':len(lines),
        'ranges':[{'start':a,'end':b,'text':'\n'.join(f'{i}: {lines[i-1]}' for i in range(a,b+1))+'\n'} for a,b in spans]})
save('root/source-snapshot.json', {'schema':'rpte-root-critical-source-v1','created_utc':stamp,
    'type':'source excerpts and hand proof only; no execution proof', 'files':sources})

private = {
    'id':'K0-private-sole-aligned4KiB',
    'kind':'native-source mathematical capacity exclusion, not a timing measurement',
    'same_t0':{'live_sources':[0,1],'iommu_ids':[0,4],'ddr_ids':[32,36],
        'job_bytes_each':4096,'native_buffer_beats_each':512,'normal_nonzero_1D':True,
        'prior_payload_residue':0,'DEFAULT_conf':0,'raw_coupler':False,
        'fresh_choice_condition':'if the two native legal heads become visible together before merged firstVALID; no synthetic waiting'},
    'orders':{},
    'proof':'For each source/lane, at most512 owned bytes enter a512-deep FIFO; the last beat sees at most511 earlier entries. W drains can only reduce occupancy. This own job cannot demand a513th payload beat.',
    'excluded_witness':'persistent heldR caused by this sole aligned4KiB job overflowing its own DMA payload capacity',
    'not_excluded':['transient ready propagation','other source carried occupancy','PTE/meta wait','other geometries/jobs','safe-return generation-window feedback'],
    'measured_completion_cycles':None,'measured_gain':None,
    'decision':'this payload-capacity positive is impossible in the frozen private instance; do not reduce buffers or enable extra sources to fabricate it'
}
for order in ['AB','BA']:
    private['orders'][order]=[
        {'event':'fresh NormalTL choice','value':order,'condition':'both heads native legal, unheld, unforced; otherwise native action'},
        {'event':'full walk and physical AR','value':'normal native path; exact actual walk count/acceptance cycles unknown'},
        {'event':'payload RfirstVALID / handshake / held','value':'own capacity overflow cannot cause sustained heldR under the stated bound; actual firstVALID/handshake cycles unknown'},
        {'event':'real R/W/B / fault retirement','value':'all still required; not executed, not asserted complete'},
        {'event':'all-task quiet','value':'not measured; no finite performance vector is fabricated'}]

cut = {
    'id':'K1-conditional-production-same-bank-return-cut',
    'kind':'source-derived conditional partial-order/capacity proof under the declared read-service law; no native DEFAULT reachability or board claim',
    'initial_state_id':'X0-identical-for-AB-and-BA',
    'native_DEFAULT_prefix_reachable':None,
    'same_t0':{
        'A':{'source':2,'bank':0,'iommu_id':8,'ddr_id':40,'DID':12,'type':'READ','captured_burst_beats':256,'captured_burst_bytes':2048,'job_bytes':4096,'buffer_beats':3},
        'B':{'source':1,'bank':1,'iommu_id':4,'ddr_id':36,'DID':11,'type':'READ','job_bytes':4096,'buffer_beats':512,'native_fullbuffer_certificate':'required, exact DID11 and owned empty sole aligned4KiB job'},
        'carried_owner':{'source':0,'bank':0,'iommu_id':0,'ddr_id':32,'DID':10,'role':'global W FIFO current head; remaining W bytes require R0 queued behind A in bank0 AR FIFO'},
        'already_paid_A_write_preparation':'all destination permission and actual W lease behind source0 accounted; no absent-WRITE shortcut',
        'native_bank0_AR_order':[2,0],
        'read_admit':'bank0 allowed; B uses only its real source1 fullbuffer certificate. New AW same-edge restrictions and AW-first local selection already satisfied.',
        'choice':'two fresh normal heads; no existing mergedVALID/forced fairness; all existing virtual/physical AW/AR/W locks preserved',
        'initial_memory':'no earlier blocking R; exact source0 required return not offered or queued ahead of A',
        'requests':'owned/captured heads and carried R/W/B debt only; no free future descriptor or PTE',
        'native_intermediate_R_slots_upper_bound':8,
        'board_opaque_extra_slots':None,
        'future_environment':'fixed valid stable mappings; finite full walks; W/B ready and W service independent; completion R FIFO order with full-run DS priority per beat and held-R preservation; no external intervention/cancellation/new control action'
    },
    'limits':[
        'X0 is a conditional interface state, not a proven fresh DEFAULT iDMA prefix or actual executed 4-source fixture',
        'Do not add the production small sources to the private fixture or reduce512 buffers',
        'This FIFO read law is the known behavioral boundary, not measured MIG service',
        'Finite opaque intermediate capacity gives eventual capacity failure only after enough beats; actual board capacity/held cycle unknown',
        'No R0 forwarding, cross-ID completion-R reorder, same-bank head reorder, new prepare origin, cancellation or timeout is granted to F1'
    ],
    'orders':{
        'AB':[
            {'event':'NormalTL A then B commitment','full_ids':[8,4],'detail':'A complete walk commits first; B offered while A busy is held, not reselected at PTW idle'},
            {'event':'physical AR_A','full_id':40,'detail':'actual AR acceptance creates2048B obligation; it precedes required physical AR_R0 by bank0 FIFO causality'},
            {'event':'PTE_B generation / internal / DDR acceptance','full_id':48,'detail':'PTE birth follows received predecessor PTE; accepted DS may preempt unheld payload per beat; exact relative cycles and counts unknown'},
            {'event':'payload firstRVALID and held','full_id':40,'detail':'if A continuously offered without local W drain, ≤11 aligned beats fit stated RTL boundary. After those positions fill, an A held beat cannot be displaced. PTE_B may finish earlier or wait; no whole-burst DS rule assumed'},
            {'event':'R0 / W0 / W_A dependency cut','full_ids':[32,40],'detail':'R0 cannot return through completion-R FIFO before A RLAST; A cannot reach RLAST without source2 W drain; source2 W cannot begin before source0 WLAST; W0 cannot finish without R0'},
            {'event':'R/W/B / faults / quiet','detail':'under X0 law no success completion of A, B or carried-owner debt; no final RLAST_A, final WLAST_A/B, final B_A/B or allquiet. Valid mapping has no automatic fault/cancel exit; fault injection not permitted'}
        ],
        'BA':[
            {'event':'NormalTL B then A commitment','full_ids':[4,8],'detail':'B complete walk before A; no retroactive preprepare or accepted-AR deletion'},
            {'event':'physical AR_B / PTE_A','full_ids':[36,48],'detail':'B returns fit its real512-beat sole-job capacity; already queued DS can preempt each unheld beat. Actual return/PTE cycles unknown'},
            {'event':'physical AR_A','full_id':40,'detail':'A acceptance still precedes required R0 physical AR by the unchanged bank0 FIFO; B order does not release source0 W ownership'},
            {'event':'payload firstRVALID and held','full_id':40,'detail':'same ≤3+8 beat capacity bound when source2 cannot drain; B may have earlier R/data progress but no final W/B before owner0 release'},
            {'event':'R0 / W0 / W_A dependency cut','full_ids':[32,40],'detail':'identical circular cut as AB; other-bank translation cannot supply source0 bytes or reorder bank0 AR heads'},
            {'event':'R/W/B / faults / quiet','detail':'same no-success/allquiet result under X0 law; a completed translation or earlier B payload R is not complete task success'}
        ]
    },
    'cut_edges':['A RLAST waits source2 W drain after ≤11 stored beats','source2 W drain waits source0 WLAST','source0 WLAST needs R0 bytes','R0 bytes wait A RLAST under unchanged completion-R FIFO and bank0 [A,R0] order'],
    'symbolic_complete_result':{'AB':'no finite all-task completion under X0 law','BA':'no finite all-task completion under X0 law'},
    'numeric_completion_cycles':{'A':None,'B':None,'owner0':None,'all':None},
    'numeric_gain':None,
    'exact_first_held_cycle':None,
    'decision':'this specific same-bank carried-debt cut cannot be rescued by a finite-latency two-bank head order. It supplies no complete-performance positive; actual DEFAULT prefix remains unknown, so no actual deadlock or whole-family closure is claimed'
}

equal = {
    'id':'K2-certified-safe-head-vs-unsafe-READ',
    'kind':'specific ordinary logical first-choice projection, not actual timing/cost equivalence or published whole coverage',
    'same_t0_conditions':['two native legal fresh heads','no held or forced fairness choice','complete full-source/job/return-footprint binding already paid','one independently certified safe READ or WRITE','other certified unsafe READ; missing safe certificate is unknown, not unsafe'],
    'candidate_condition':'only if F1 proposed useful action is choosing this unique safe head before unsafe READ; no valid history makes F1 fall back RR, not an invented prediction',
    'one_frozen_ordinary':'O-envelope-select',
    'ordinary_full_contract':'ordinary-envelope/ordinary-contract.json',
    'logical_selected_order_equal':True,
    'conditional_observable_event_equality':'If both offer/accept the same immutable header at the same time with the same native state, environment law, RR/fair updates and subsequent control, deterministic continuations including all R/W/B/fault outcomes agree. This is a conditional bisimulation fact, not a measured schedule.',
    'selection_delay_equal_proven':False,
    'numeric_total_cost_equal_proven':False,
    'independent_candidate_cost_advantage_proven':False,
    'uncovered_domains':['both safe heads with genuine birth/service timing differences','unknown binding/certificates','both hazards','forced fairness','unsafe prefix already accepted before t0'],
    'decision':'safe-before-unsafe alone supplies no distinct logical selection; complete timing and costs remain null, and this projection does not copy or close all F1 prediction'
}
save('root/abba-event-table.json', {'schema':'rpte-same-state-event-table-v1','created_utc':stamp,
    'interpretation':'source mathematical/conditional event bounds only; no generated scenario executions or cycle trace',
    'cases':[private,cut,equal],'task_bytes_or_IDs_exchanged':False,
    'positive_microbenchmark':'not admitted','neighbor_microbenchmark':'not admitted','identity_holdout':'not admitted','RW_holdout':'not admitted'})

cost = {
    'schema':'rpte-delta-observation-cost-card-v1','created_utc':stamp,
    'candidate':'F1 pre-firstVALID two legal native-head order, shared R/PTE feedback; only scoped held-return instances assessed this round',
    'retained_native':['one PTW','IoTLB16','whole walk','NativeN hit bypass','per-bank4 outstanding and16 W FIFO from native8/32 split','sameID/fullID and eager heldVALID','actual R/W/B/fault retirement'],
    'existing_payload_geometry':{'private_two_512_buffers_bytes':8192,'production_512_512_3_3_buffers_bytes':8240,'root_specific_R_boundary_max_payload_bytes':64,'new_capacity_bytes':0,'exclusive_available_shared_slots':'must be tracked; maximum8 is not eight free positions per source'},
    'observable_inputs':[
        {'input':'immutable native head DID/PID/IOVA/access/bank','status':'existing NormalTL fields; head legality still native'},
        {'input':'original full source ID and captured burst LEN/SIZE/mask/job association','status':'not carried by current mergedNormalTL header; paid read/tap/origin retention needed'},
        {'input':'resident/accepted/held return obligation with actual available capacities','status':'source and storage lifetime tracking; each byte/slot counted once; source identity cannot be bank'},
        {'input':'actual W source FIFO owner/ordered WRITE coverage/data supply and all W/B debt','status':'multiple native ownership domains; no bare WRITE-success/free permission tag'},
        {'input':'PTE source-stage internal acceptance vs realDDR acceptance, R firstVALID/held/handshake/retire','status':'new routed feedback ports and phase records; old first trace is only handshake'},
        {'input':'completed-request PTE counts/service history','status':'allowed only completed/owned records; no future PPW/PA/content or unknownDDR/MIG-state oracle'}
    ],
    'lifetime_and_full_cost':[
        {'item':'head metadata ports and full-source/job binding','lifetime':'capture before legal fresh choice; retain identity through dependent return/write responses','numeric_cost':None},
        {'item':'already owned R bytes/resident bytes and reservation map','lifetime':'before firstphysicalR/AR commitment through consumer real bytes drain, not merely DDR RLAST','numeric_cost':None},
        {'item':'complete normal WRITE preparation and W leases','lifetime':'success permissions/address/header/order through all real children WLAST/B/fault; native internalAW spill !=external accept','numeric_cost':None},
        {'item':'DS/PTE memory event ports and records','lifetime':'distinguish generation/internalaccept/DDRaccept/firstRVALID/held/handshake/fault; hold immutable request while VALID','numeric_cost':None},
        {'item':'history entries and key association','lifetime':'completed record validity/context and initialization/fair eviction; missing historyRR; no hiddenfuture','numeric_cost':None},
        {'item':'two-order forecast arithmetic/state/read ports','lifetime':'one frozen owned-obligation snapshot before firstVALID; charge computation and any readiness/select delay','numeric_cost':None},
        {'item':'held grant, age/RR and one-overtake fairness','lifetime':'firstVALID through native acceptance; compulsory service cannot be undone by hazard preference','numeric_cost':None},
        {'item':'O-envelope-select predicates/selection','lifetime':'same paid owned information; true safe-WRITE/complete capacity certificate not free; no double-rollout','numeric_cost':None},
        {'item':'prospective prepare/whole-envelope/one-primer/coupler','lifetime':'common descriptor origin through all real bytes/R/W/B/fault/invalidation drain; currentRAW0 has no coupler, onefirstR provides onecredit only if enabled','numeric_cost':None},
        {'item':'all ordinary/native memory and retirement service','lifetime':'DSpriority sharedt0 to all task/R/W/B/fault quiet, not V-DONE; exact admitted bytes and faults accounted','numeric_cost':None},
        {'item':'new logic/routing/timing/clock/host/application cost','lifetime':'complete implementation and same-resource comparison; null until frozen/measured','numeric_cost':None}
    ],
    'existing_full_cost_reference':'../tl-memory-feedback-20261014/ordinary-cost/cost-interface-card.md and ../r2-ownership-cost-20261013/root/full-cost-comparison.json (37 categories only; no old candidate architecture or numeric total inherited)',
    'numeric_totals':{'new_register_bits':None,'new_logic':None,'select_cycles':None,'forecast_cycles':None,'history_bytes':None,'area':None,'LUT':None,'FF':None,'Fmax':None,'power':None,'CPU':None,'application_gain':None},
    'decision':'logical action/proven bounds only; exact new widths/routing/predictor remain unfrozen, no numeric complete-cost advantage accepted; equally bill any added sensor/interface, omit predictor only when control does not need it',
    'no_new_implementation':True
}
save('root/observation-ownership-cost-card.json',cost)

verdict={
    'schema':'rpte-return-commitment-root-acceptance-v1','created_utc':stamp,
    'status':'bounded_source_gate_completed; named held-return instances_not_admitted',
    'root_ownership':'root directly inspected fresh NormalTL/PTW/DS/sharedDDR/heldR/nativeDEFAULT/complete IDs and accepted child source evidence; root owns event/cut/cost verdict',
    'gates':{
        'native_rready_and_retirement':'accepted under stated normal nonzero1D/source domains',
        'next_PTE_chain_and_held_commitment':'accepted source phase chain, no measured relative cycle witness',
        'same_fresh_native_positive':'K0 capacity positive excluded; K1 conditional interface prefix DEFAULT reachability unknown; no new fully reachable positive accepted',
        'complete_gain':'K1 conditional source/FIFO cut blocks allcomplete in either head order; partial progress is insufficient',
        'ordinary_specific_choice':'K2 same logical safe-before-unsafe projection O-envelope-select; actual latency/cost/effect equivalence only under explicit same-offer/same-continuation conditions',
        'numeric_independent_cost':'not established; unknowns null',
        'private_prototype_and_four_conditions':'not admitted; zero executed'
    },
    'accepted_new_facts':[
        'normal1D Rready consumer constant1; payload FIFO depends on full-source W handshake drain',
        'private sole aligned4KiB×512 cannot overflow own payload capacity',
        'production IDs0/4/8/12 and DDR32/36/40/44 have bank0={0,2},bank1={1,3}; private two-source bank counters alias production identities',
        '3-beat DMA plus8 named RTL R positions permits at most11 undrained aligned beats, not a claimed firstheld cycle or entireboard capacity',
        'eachbank native16 W FIFO and4 outstanding; no extra resource added',
        'RAWcoupler disabled; DEFAULT paired local metadata not firstR-to-AW native gate or arbitraryglobal order proof',
        'DSpriority chooses at each unheld beat; nextPTE needs source generation/spills/external admission; heldR cannot be displaced',
        'conditional same-bank [A,R0] circular return cut not solved by two-bank order under the declared FIFO read law',
        'one frozen complete O-envelope-select gives specific safe-before-unsafe logical action without predicting two schedules, only with genuine equallypaid certificates'
    ],
    'accepted_child_artifacts':[
        {'seat':'/root/rpte_native_ready','paths':['native-ready/native-ready-evidence.md','native-ready/source-snapshot.json','native-ready/native-ready-verdict.json'],'root_limits':'accept ready/identity/capacity/retirement source bounds; DEFAULT fresh native prefix remains unknown'},
        {'seat':'/root/rpte_ordinary_envelope','paths':['ordinary-envelope/evidence.md','ordinary-envelope/ordinary-contract.json','ordinary-envelope/comparison-verdict.json','ordinary-envelope/delivery-verification.json'],'root_limits':'accept complete frozen control and scoped first-choice projection; no global timing/cost/published equivalence claim'}
    ],
    'published_prior_art':'prior verified MASK / ISCA2018 component coverage retained; no new literature task or claim of wholeF1 coverage/novelty',
    'branch_decision':{'this_round':'terminate bounded source gate without prototype or matrix expansion','K0':'close sole4KiB private own-capacity positive','K1':'close this conditional same-bank cut as a complete-gain witness; actual DEFAULT reachability unknown, no actual deadlock claim','K2':'safe-before-unsafe alone not an independent logical selection','F1_whole_family':'open; safe-head birth/service-window timing domain not adjudicated','old_R2_R107_C112':'remain closed; none restored'},
    'execution':{'new_models':0,'new_simulations':0,'new_microbench_conditions':0,'new_tests':0,'builds':0,'RTL_changes':0,'driver_ABI_changes':0,'synthesis':0,'board_runs':0,'old_experiment_replays':0,'old_validator_replays':0,'external_models':0,'artificial_pressure':0,'extra_capacity':0},
    'measurements':{'A_complete':None,'B_complete':None,'all_complete':None,'performance_gain':None,'area':None,'Fmax':None,'application_gain':None},
    'unresolved':['nativeDEFAULT four-source X0 fresh prefix','board opaque R buffering/MIG return law','exact firstheld and relative nextPTE admission cycle','two-safe-head birth/service feedback difference against frozen short/debt/release ordinary rules','exact sensor/predictor/select implementation costs'],
    'completion_policy':'five docs sync and new delivery checks before actual tool complete; completion receipt stored separately, no paper-complete inference',
    'paper_complete':False
}
save('root-result-acceptance.json', verdict)

save('next-goal-contract.json',{
    'schema':'rpte-next-decision-contract-v1','status':'bounded round delivered; no automatic next goal or experiment',
    'next_single_question':'If continuing F1, can two genuinely safe legal heads have an observable not-yet-generated-PTE service-window choice difference versus the already frozen short-TL/debt/owner/release-density/two-stage ordinary controls?',
    'entry_requires':['one new source-supported fresh-state fact with complete originalsource/job/bytes binding','same common origin and native capacities, onePTW/16IoTLB and firstVALID unchanged','full-run DSpriority through all owned R/W/B/faultquiet','new AB/BA partial-order then complete-task witness not covered by a single full ordinary rule','freeze feedback/history/ports/select delay before conditional prototype'],
    'not_a_remaining_todo':['K0 smaller-buffer test','K1 arbitrary FIFO-prefix execution labeled DEFAULT reachability','safe-before-unsafe-only positive','tau or W-tail pause matrix','old source gate/coupler fix and old retesting'],
    'preserve':['8/9month assets and limited findings','F1 architecture and both completed source rounds','all old closed branches','native capacities and provenance'],
    'bounded_validation_if_admitted':'one private prototype, <=4 new conditions positive/legal neighbor/identityswap/alternatelegalRW; actual bytes/PTE/R/W/B/fault/quiet and full costs',
    'downstream':'only independent complete gain survives then one GPU/AI-priority real shared-translation workflow; then RTL/FPGA key validation',
    'execution_policy':'Codex-only gpt-6.1-sol/ultra/native, new exclusive seats only if critical; no extra budget unless explicitly requested; no old replay/pressure/capacity',
    'cumulative_original_paper_clock':True,'paper_complete':False})

save('root/execution-ledger.json',{
    'schema':'rpte-execution-ledger-v1','created_utc':stamp,
    'actual_goal_registration':'goal-registration.json; actual create_goal no token_budget',
    'authors':['root Codex mechanism/source/cut/finalreview','new native-ready exclusive Codex source seat','new ordinary-envelope exclusive Codex control seat'],
    'work':'source reads/excerpts/hash metadata, manual capacity/partial-order reasoning, new artifacts, five authorized docs and new artifact delivery checks',
    'new_model_or_scenario_runs':0,'simulation_runs':0,'unit_or_RTL_tests':0,'builds':0,'old_experiment_replays':0,'old_validator_replays':0,'board_operations':0,'production_changes':0,'closed_or_old_seat_restores':0,
    'lookup_errors':'several guessed source directory names absent; exact rg file lookup corrected before reading. These are read-only lookup diagnostics, not failed build/model runs.',
    'reuse':'prior snapshots/contracts/findings reused unchanged; selected hashes and critical delta evidence only',
    'artifact_checks':'new local JSON/hash/document/link delivery checks only; not evidence of mechanism runtime correctness',
    'paper_complete':False})
print('Wrote source-gate event/cost/verdict/next-contract/ledger artifacts; no model or test invoked.')
