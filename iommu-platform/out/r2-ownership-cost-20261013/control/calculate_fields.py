#!/usr/bin/env python3
"""This new task's one-shot static arithmetic/artifact generator, not an RTL validator."""
from pathlib import Path
import datetime
import hashlib
import json

ROOT = Path('/home/xvhaocheng/work/iommu-platform')
OUT = ROOT / 'out/r2-ownership-cost-20261013/control'
NOW = datetime.datetime.now(datetime.timezone.utc).isoformat()

def save(name, data):
    (OUT / name).write_text(json.dumps(data, ensure_ascii=False, indent=2) + '\n')

SOURCES = {
    'cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv': [(96,139),(174,180),(220,255),(325,360),(520,597),(674,727)],
    'cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv': [(158,184),(236,305),(359,407),(419,440),(526,584)],
    'cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux.sv': [(51,61),(89,181)],
    'cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv': [(69,70),(140,227),(237,293),(537,618)],
    'cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv': [(46,55),(260,344),(350,366)],
    'cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_err_slv.sv': [(90,153)],
    'cva6/corev_apu/riscv-iommu/vendor/common_cells/src/spill_register_flushable.sv': [(38,103)],
    'cva6/corev_apu/riscv-iommu/vendor/common_cells/src/delta_counter.sv': [(28,58),(60,81)],
    'cva6/corev_apu/riscv-iommu/vendor/common_cells/src/rr_arb_tree.sv': [(131,138),(144,198)],
    'cva6/vendor/pulp-platform/common_cells/src/fifo_v3.sv': [(15,60),(73,140)],
    'cva6/corev_apu/riscv-iommu/include/axi-iommu/typedef.svh': [(37,87)],
    'cva6/corev_apu/riscv-iommu/vendor/axi/include/axi/typedef.svh': [(31,91)],
    'cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_pkg.sv': [(48,64)],
    'cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv': [(125,165)],
    'cva6/corev_apu/riscv-iommu/packages/rv_iommu_pkg.sv': [(79,85),(352,358),(488,488),(528,529)],
    'cva6/corev_apu/tb/ariane_soc_pkg.sv': [(29,33),(110,140)],
    'cva6/core/include/ariane_pkg.sv': [(313,316)],
    'cva6/core/include/cv64a6_imafdc_sv39_config_pkg.sv': [(26,29)],
    'cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv': [(863,882),(1218,1244),(1255,1281),(1394,1410)],
    'cva6/corev_apu/fpga/src/ariane_xilinx.sv': [(158,162),(812,815)],
    'cva6/corev_apu/iDMA/src/idma_backend.sv': [(110,112),(632,646)],
    'tests/nonblock_banked_top.sv': [(203,211),(292,324),(365,389)],
    'out/mechanism-continuation-20261013/baseline/evidence.md': [(128,154)],
    'out/mechanism-continuation-20261013/baseline/baseline-contract.json': [],
    'out/mechanism-continuation-20261013/root/source-evidence.json': [],
    'out/mechanism-continuation-20261013/root/cost-boundaries.json': [],
    'out/r2-ownership-cost-20261013/execution-contract.json': [],
}
manifest=[]
for path,ranges in SOURCES.items():
    content=(ROOT/path).read_bytes()
    lines=content.decode().splitlines()
    manifest.append({'path':path,'sha256':hashlib.sha256(content).hexdigest(),'line_count':len(lines),
        'excerpts':[{'first_line':lo,'last_line':hi,'original':'\n'.join(f'{i}: {lines[i-1]}' for i in range(lo,hi+1))} for lo,hi in ranges]})
save('source-manifest.json',{'schema':'ordinary-B5-critical-sources-v1','created_utc':NOW,'verification':'direct source read/hash; bounded selected sources; no source modification','files':manifest})

U=1
widths={'address':64,'original_id':4,'physical_address':56,'user_example':U,
    'raw_AW_formula':'153+U','physical_AW_formula':'103+U','raw_AR_formula':'147+U','physical_AR_formula':'97+U',
    'W_formula':'73+U','B_formula':'6+U','R_formula':'71+U',
    'raw_AW':154,'physical_AW':104,'raw_AR':148,'physical_AR':98,'W':74,'B':7,'R':72,
    'historical_test_local_id_width':2,'historical_test_local_raw_AR':146,'historical_test_local_R':70,
    'normalTL_request':118,'normalTL_response':185,'normalTL_response_mrif_content':102,'normalTL_fault_code':12,
    'production_U':'ariane_pkg::AXI_USER_WIDTH; named cv64a6_imafdc_sv39 FetchUserEn/DataUserEn=0 implies U1; build selection not rerun',
    'test_U':'hardcoded1; actual final busID4'}

core={
    'AW_packet_memory_bits':8*154,
    'cell_phase_bits':8*2,
    'bank_ring_pointers_and_counts_bits':2*(2+2+2+3+3),
    'G_reference_array_bits':8*3,
    'G_pointer_and_count_bits':3+3+4,
    'TL_owner_phase_bits':2*2,
    'TL_owner_ref_additional_bits':0,
    'held_token_bits':3,
    'shared_request_rr_lock_upper_bits':1+1+2,
    'input_private_bank_owner_array_plus_controls_bits':8+3+3+4,
    'input_private_first_offer_commitment_bit':1,
    'input_accepted_AW_minus_input_WLAST_count_bits':4,
    'explicit_rank_bits':0,
    'generation_bits_in_selected_scope':0,
    'new_full_prepared_AW_copy_bits':0,
    'new_per_transaction_B_reorder_store_bits':0,
}
core_total=sum(core.values())
core_tap_total=core_total-core['input_private_bank_owner_array_plus_controls_bits']-core['input_private_first_offer_commitment_bit']

native_packets={
    'input_upstream_mux_AW_spill':{'slots':2,'packet_bits':154,'valid_bits':2,'bits':310},
    'input_upstream_mux_AR_spill':{'slots':2,'packet_bits':148,'valid_bits':2,'bits':298},
    'per_bank_completion_AW_and_select_spills':{'banks':2,'packet_slots_per_bank':2,'packet_bits':104,'select_bits_per_slot':1,'valid_bits_per_bank':4,'bits':428},
    'per_bank_completion_W_spills':{'banks':2,'packet_slots_per_bank':2,'packet_bits':74,'valid_bits_per_bank':2,'bits':300},
    'per_bank_completion_B_spills':{'banks':2,'packet_slots_per_bank':2,'packet_bits':7,'valid_bits_per_bank':2,'bits':32},
    'per_bank_completion_AR_and_select_spills':{'banks':2,'packet_slots_per_bank':2,'packet_bits':98,'select_bits_per_slot':1,'valid_bits_per_bank':4,'bits':404},
    'per_bank_completion_R_spills':{'banks':2,'packet_slots_per_bank':2,'packet_bits':72,'valid_bits_per_bank':2,'bits':292},
}

contract={
    'schema':'ordinary-B5-normal-write-full-cost-v1','created_utc':NOW,'task_goal_id':1791902594,
    'author':'Codex gpt-6.1-sol/ultra; exclusive independent control task; forknone; no DeepSeek/Claude/Astra/subagents',
    'status':'algorithmic_scope_lifecycle_closed_pending_root_acceptance; no RTL or production integration',
    'scope':{'write':'INCR64/nonATOP; finite fair run; genuine matching W payload/last; normalTLsuccess or normalTLfault',
        'ids':[0,4,8,12],'bank_bit':2,'bank_sources':{'0':[0,8],'1':[4,12]},'context':'one frozen DID/PID/context per full ID',
        'normalTL':'one shared req/resp; one active owner/bank; response bank matches unique WAIT owner; exactly one response per accepted request',
        'native_capacities':{'AW_per_bank':4,'W_FIFO_per_bank':16,'upstream_source_W_owner':8,'output_bank_W_owner':8,'banks':2,'IOTLB':16,'PTW':1},
        'native_spills':'each enabled spill two packets, separately charged; native error Wdescriptor1 and Bdescriptor2 perbank',
        'finite_fair_assumptions':['TL eventually supplies one response per accepted request','all promised W beats appear','memory eventually accepts locked AW/W and returns B','upstream eventually accepts B','internal retained RR arbiters fair'],
        'excluded':['outstanding reset','async invalidation/context remap','ATOP/MRIF/ignore disposition','producer abandons W','malformed WLAST/payload','new read admission or R drain claim']},
    'ordinary_algorithm_name':'O-B5N-queue; O-B5N-tap is a legal lower-state ordinary alternative with explicitly paid interface',
    'ordered_actions':[
        {'event':'virtualAW handshake','requires':'own bank cell, G8 credit, bounded input owner credit; preserve immutable fullID','effect':'raw header capture and G append(bank,slot); used/rawcount++; acceptedWcount++'},
        {'event':'bank prepare owner idle and rawcount>0','requires':'nativebank TL owner not held by read/other request','effect':'prepare cursor cell→stable normal WRITE request; OFFER→WAIT on normalTLreq handshake; bank cannot issue another request while WAIT'},
        {'event':'normalTLrsp handshake','requires':'matching bank WAIT owner; cell still owned','effect':'success replaces addr64 with extendedspaddr56 and READY_OK, or READY_FAULT; owner retires immediately; prepare++; rawcount--'},
        {'event':'Ghead ready and held empty; first internalAWVALID','requires':'only Ghead may enter native AW+target spill; full payload/selection stable','effect':'stableoffer until native internalAW input handshake; no successor offered'},
        {'event':'native internalAW input handshake','requires':'full packet/target captured in existing owned spill','effect':'held=(valid,bank,target); Gpop; cell FREE and bank head/used update; not physicalAWauthorization'},
        {'event':'real targetAW handshake','requires':'success real physicalAW or fault real error sinkAW for held token','effect':'held retires; permit next Ghead only afterward; input capture/TL prep continue during hold'},
        {'event':'input W beat and input WLAST','requires':'matching AW already frontendaccepted; inputownerhead; bank FIFO space','effect':'retain original noninterleaved order; only true input WLAST retires inputowner and acceptedWcount'},
        {'event':'native W FIFO/spill/sink handshake','requires':'internalAW allowance and nativefirstofferWtarget lock; outputowner8 forsuccess','effect':'real sink WLAST creates errorB credit; successW progresses in originalsuccessfulAW/W order; no physicalAW/W forfault'},
        {'event':'B capture at native cut→Bspill','requires':'sameIDtarget binding or legal same target orderedresponse','effect':'packet owner moves into2entryBspill; old target debt can retire atcapture; no upstream debt retirement'},
        {'event':'upstreamB handshake','requires':'firstVALID stable Bpacket; older sameID responses preserved by target binding plus Bspill FIFO','effect':'native inputfullID counter and topinflightaw retire; final writeowner retired'},
    ],
    'input_variants':{
        'queue':{'state_bits':23,'interface_changes':['remove input AW samebank Wlock','preserve full-IDBcredit','new depth8 bankowner with first-offer lock','acceptedWcount4 gates preaccepted W'],'metadata_port':'one ownerhead read, one firstoffer push, one trueWLASTpop; globalAWsingleoffer'},
        'tap':{'state_bits':4,'interface_changes':['export stable upstreamfullsource-owner head/source2','remove inputAWsamebankWlock','preservefullIDBcredit','acceptedWcount4 gates preacceptedW'],'new_read_ports':1,'new_source_wires_bits':2,'native_exposed_api':False},
        'keep_native_banklock':{'allowed':'when candidate also retains it','crossbankaccept_before_priorvirtualWLAST':False}},
    'lifetime_proofs':{'cell_response_ABA':'free follows exactlyone rsp and full packetmigration; no outstandingresponse can target a freedslot','no_generation':'frozencontext, no cancel/reset, uniqueowner response and heldwithoutcellreference','sameIDerrorhole':'younger異targetAW stalls until olderB captured; capturedoldB ownsorderedspill untilupstreamaccept','header_copies':'prepared samecell; existing 2-entryspill stores charged; no newfullpreparedcopy','physicalAWorder':'one globalheld permits no successor internalAWuntilreal targetAWhandshake'},
    'integration_changes':[
        'replace head-only AW streamFIFO interface with 4cell bankring supporting separateprepare/read and issue/read; no crossbank borrowing',
        'decouple native TC normalTLowner lifetime from COMPLETE awaitinginternalAW; retain normalTLrequest/response and sharedbanktag routing',
        'on response overwrite cell address/phase; early requestowner release; do not duplicate8fullphysicalheaders',
        'globalFIFO+held control gate internalAW offers or equivalently postcut physicalfirstVALID; notify realerrorAW/physicalAWhandshake',
        'retain native success/error target binding, firstVALID locks, Wtargetlocks, AW/W/Bspills, errorsink and upstreamfullIDdebt',
        'provide explicit inputbankowner8 or paid upstreamsourceowner tap; original inputdemuxcrossbankWlock must be deliberatelychanged forfull earlyadmission',
        'reads retain original READ controller, req118/resp185 registers and state3 per bank; WRITE OFFER/WAIT and READ REQ/AWAIT/COMPLETE are mutually exclusive; response bank has no additional free READ/WRITE tag; no new read permit',
    ],
    'state_cost_summary':{'U1_queue_managed_AW_bits':core_total,'U1_tap_managed_AW_bits':core_tap_total,'native_common_outside_this_sum':'cost-envelope.json.native_components; these totals are not complete area','core_fields':core},
    'capability_not_required':['sameID異targetsimultaneousphysicaloutstanding; native targetlock remains'],
    'scope_equivalence_claim':'ordinary FIFO expresses normal WRITE prepare/commit/W/B lifetime; full scope equality only against candidate with the same explicit acceptance/scheduling/port assumptions',
    'unresolved':['production physical timing/area','exact application/source Bdebt bound','native fouriDMA shortburst timeline','readside/common diagnostics full synthesis mapping'],
    'execution':{'new_generator_attempts':2,'source_excerpt_guard_failed_before_arithmetic':1,'new_static_arithmetic_executions':1,'old_validator_runs':0,'RTL_runs':0,'performance_runs':0,'builds':0,'synthesis':0,'board':0,'production_source_changes':0},
}
save('ordinary-contract.json',contract)

catalog=json.loads((ROOT/'out/mechanism-continuation-20261013/baseline/baseline-contract.json').read_text())['cost_field_catalog']
fields={
 'dma_data_payload_bytes':{'production':8240,'depths':[512,512,3,3],'four_small_hypothesis':96,'historical_two_small':48,'additional_by_B5':0,'meaning':'data-only; complete nativeDMAmetadata cost not these payloadbytes'},
 'common_tc_w_data_payload_bytes':{'FIFO':256,'four_native_Wspill_packets_data_bytes':32,'input_W_mux_defaultspill':0},
 'common_tc_w_packet_fields_bits':{'FIFO_formula':'32*(9+U)','U1_FIFO':320,'spill_formula':'4*(9+U)+4valid','U1_spill_nondata_and_valid':44,'FIFO_controls':26},
 'additional_return_data_payload_bytes':{'B5':0,'native_Rspill_packets_data_bytes':32,'other_R_reservation':'C_existing_R_policy, unchanged and not assumedzero if selectedreadpolicyreserves'},
 'return_packet_nondata_bits':{'native_Rspill_formula':'4*(7+U)+4valid','U1':36,'new_replay':0,'response_data_width':64},
 'raw_ar_header_slots':{'native':8,'native_storage_formula':'8*(147+U)','U1':1184,'upstream_rawARspill':2,'U1_rawARspill_with_valid':298},
 'prepared_ar_header_slots':{'new_B5':0,'native_physicalARspill':4,'U1_physical_ARpacket_select_valid':404,'no_new_read_rule':True},
 'aw_raw_prepared_slots':{'owned_raw_cells':8,'banks_each':4,'same_cell_prepared':True,'new_fullcopy':0,'upstream_rawAWspill':2,'native_physicalAWspill':4,'useful_globalheld_AWtokens_max':1,'rawcells_plus_held_max':9},
 'header_field_widths_bits':widths,
 'job_epoch_segment_metadata':{'new_B5':0,'scope':'frozen context; no newR/job/offsetpredicate','production_DMA_Wlast_metadata_bits':2124,'four_small_hypothesis_Wlast_bits':88,'other_common_job_metadata':'C_DMA_other_metadata(BufferDepth,NumAxInFlight8,MemSysDepth0), widths not source-audited in this exclusive WRITEcontroltask'},
 'source_id_metadata':{'wire_original_fullID_bits':4,'source_encode_bits':2,'bank_bits':1,'immutable_ids':[0,4,8,12],'additional_cell_source_dup':0,'Bspills_IDpayload_bits':16,'errorsink_WplusB_IDbits':24},
 'sequence_state_bits':{'explicit_rank':0,'G_refs':24,'G_order_controls':10,'bankring_prepare_order_in_controls':24,'generation':0,'proof':'no cellreuse untilnormalTLrsp and packetmigration; heldreferences banktargetonly'},
 'valid_phase_state_bits':{'cell_phase':16,'bankTLownerphase':4,'held_valid':1,'native_packetvalids':'native_components.packet_spills; exactly2valid/packetspill plus separatetargetspillvalids'},
 'fifo_pointer_occupancy_bits':{'bankring':24,'G':10,'privateinput_bankowner':10,'upstream_sourceowner':10,'output_bankowner':10,'TCW_FIFO':26,'nativeAR_FIFO':14,'errorsink_WplusB':16,'native_spill_has_valid_not_ptr':True},
 'aw_w_order_state_bits':{'upstream_sourceownerarray':16,'output_bankownerarray':8,'queue_variant_newbankownerarray':8,'queue_variant_firstofferlock':1,'acceptedWcount':4,'nativeWallowance_bankcounter_declared':8,'nativeinternal_targetWlock_formula':'2*(3counter+1overflow+1target+1AWlock)=12','nativeinputoldWlock':'replaced for earlycrossbank; old4counter+1overflow+1bank+1AWlock not silently kept/doublecharged'},
 'ar_armed_held_state_bits':{'new_B5':0,'native_raw_AR_spill_packet_valid_bits':298,'native_physical_AR_select_spill_packet_valid_bits':404,'native_AR_lock_rr':'C_native_AR_arbiters: twoinputmux RR plus perbank/fullIDrouteR arbiter; keep exact firstVALIDstable; no new readeligibility'},
 'aw_armed_held_state_bits':{'held':3,'native_upstream_AWspill':310,'native_internal_AWplus_targetspill':428,'native_globalmux_lock':1,'prepared_cell_held_offer':'existing Gheadandphase; no fullshadow copy','meaning':'transferintointernalspill is not physicalAW handshake'},
 'r_receive_replay_state_bits':{'new_B5':0,'native_Rspill':292,'errorsink_readpath_formula':'2*((I4+LEN8)+fifoctrl4+beat8+busy1)=50','existing_policy_state':'C_existing_R_policy, unchanged; not rederived or removed'},
 'b_ownership_order_state_bits':{'new_reorder_store':0,'input_AW_IDcounter_and_mapping_declared':'16*(4counter+1overflow+1bank)=96','internal_two_AW_IDcounter_and_mapping_declared':'2*16*(3counter+1overflow+1target)=160','Bspill_packet_and_valid':32,'errorsink_W_IDpluscontrols':16,'errorsink_B_IDpluscontrols':24,'accepted_debt_not_global8':True,'input_reservation_upper':57,'perbank_cut_upper':13,'selected_control_conservative_debt_bound':39,'native_topinflightAWbits':32,'counters_retire':'cutBcaptured into Bspill; inputB/upstreaminflight only trueupstreamBhandshake'},
 'error_cancel_retirement_state':{'normal_fault_new_fullqueue':0,'reuse_native_WtargetandBstate':True,'sink_WLAST_gates_B':True,'cancel_reset_invalidation':'OUT_OF_SCOPE_NAMED; no general cancellationclaim','normal_fault_scope_closed':'native targetbinding prevents uncapturedsameIDerrorhole; paid Bspill capture preserves order'},
 'tl_owner_generation_state':{'bankownerphase':4,'owner_refs':'preparecursor2/bank alreadyincluded in bankring24','generation':0,'normalTL_request_wire':118,'normalTL_response_wire':185,'native_READ_TC_req_resp_registers_bits':606,'native_READ_TC_state_bits':6,'new_WRITE_full_request_response_shadows':0,'read_coexistence_owner_kind':'no new tag; exclusive WRITE phase versus retained READ controller determines bank response owner; WRITE OFFER/WAIT cannot coexist READ REQ/AWAIT/COMPLETE','shared_bankowner':'req/respbank1; no secondactive/bank'},
 'fast_classification_state':{'new_B5':0,'eligibility_SCAN':0,'native_optional_diagnostics':'C_E3_queue_and_observer(E3Enable): preserved if buildenabled; actual fieldmapping beyond proof not source-audited; not silentlyfree'},
 'capture_ports_per_cycle':{'virtualAW':1,'normalTLrsp_total':1,'Bnative_cut_capture':2,'source_Bupstream_total':1,'new_cell_data_capture_per_bank_max':2,'new_cell_phase_updates_per_bank_max':3},
 'dispatch_ports_per_cycle':{'normalTLreq_global':1,'internalAWsubmit_global':1,'physicalAW':1,'physicalW':1,'inputW':1,'errorsink_W_bank_total':2,'physicalAR_R':'existing1port/each; no new permit','hold_policy':'no nextinternalAW while heldtoken awaits truetargetAW; optional samecycle bypass not assumed'},
 'metadata_read_write_ports':{'cell_per_bank':'2R (prepare/issue), 2dataW(capture/response addr), up to3phase updates(capture/response/free), distinct cells; use explicit arbitration if only1R/1W','G':'1headR,1appendW,1popcontrol','bank_input_owner':'1headR,1firstofferpushW,1WLASTpop; tapvariant extra upstreamsourceowner readinterface','native_IDtables':'each1lookup+1push+1pop with sameID net updates; no 60recordCAM','TL_response':'global1 address write/phaseupdate percycle'},
 'payload_read_write_ports':{'TCW':'eachbank1R/1W64data+strb/last/user; totalinput1, physicalout1, errorsinks can2','Wspills':'perbank2packetregs + mux; separateinput/outputhandshakes','Bspills':'perbank1capture/1drain and2packetregs; sourceBtotal1','new_Rreplay':0,'DMAprivate':'C_native_DMA_data_ports: existing perDMA byte-lane bufferports, unchanged'},
 'matching_comparators':{'new':'2banks count/full/phase constant comparisons, Gheadreadyphase, heldbank/target; no fullrank scan, no64bitoffset/epochmatch forWRITE prepare','native':'fullID indexed mapping16×(counterfull OR); targeteq1 and wbank/targeteq1; input/exitrouting fullIDbits4; eachfirstVALIDlock retained','new_Rpermit_compare':0},
 'selection_arbiter_and_mux':{'new_prepare':'two4cell selectedread muxes, eachfullraw154; perbankcursor, no free-slotsearch','new_issue':'oneglobal8refhead read; selected bank4cell physical104read; 2bank×104 mux or equivalent hierarchical wires','TL':'native2way stable RR mux118bits; response185 fanoutto2banks','AW_W':'nativeupstream4way raw154/W74, output2way physicalAW105internalIDwide/W74; native targetdemux2wayperbank','B':'native2way7packet perbank and globalbankB; paid2entryspills','small_rr_state':'Ninput rrupper idx_width(N)+1lock+Nrequestbits; N2=4,N4=7; count shared instance once'},
 'scan_bound':{'G_commit':1,'bank_prepare':1,'rank_min_scan':0,'free_slot_scan':0,'native_credit_ORfull_width':16,'no_diagnostic_admission_scan':True},
 'fanout_and_update_events':{'new_capture':'virtualAW→onebankcell andG/Wowner+count','prepare':'NormalTLreqfire updatesoneowner; responsebank185wirefanout2 butonecellupdate','commit':'internalAW→Gpop/cellfree/held, realtargetAW→heldclear','W':'inputWLAST→inputownerpop; output/sinkWLAST→nativeWowner/allowance','B':'cutcapture→nativeIDdecr+Bspill; upstreamB→inputfullIDdecr/inflightAWdecr','native_lifetime_gauges_declared':'inflightAW32+AR32+W32+partial1=97; not pertransactionbuffer'},
 'software_cpu_application_cost':None,
 'lut':None,'ff':None,'bram':None,'area':None,'timing_fmax':None,'energy':None,
}
assert set(fields)==set(catalog), (set(fields)^set(catalog))
native_components={
    'packet_spills':native_packets,
    'TC_W_FIFO_packet_bits':32*74,'TC_W_FIFO_control_bits':26,
    'native_AR_FIFO_packet_bits':8*148,'native_AR_FIFO_controls':14,
    'preserved_AW_B_tracking_declared_bits':96+160,
    'preserved_AR_R_tracking_declared_bits':96+160,
    'error_sink_write_state_bits':16+24,
    'error_sink_read_state_bits':50,
    'native_input_source_owner_array_plus_ctrl_plus_awlock':16+10+1,
    'native_output_bank_owner_array_plus_ctrl_plus_awlock':8+10+1,
    'native_W_FIFO_output_allowance_declared_bits':8,
    'native_internal_demux_Wcounter_target_AWlock_declared_bits':12,
    'native_top_lifetime_gauges_declared_bits':97,
    'retained_native_READ_TC_full_req_resp_registers_bits':606,
    'retained_native_READ_TC_state_bits':6,
    'unknown_named_common':['C_native_AR_RR_and_other_arbiters; formulas and instancecounts required for total','C_E3_queue_and_observer(E3Enable)','C_native_TLengine(IOTLB16,onePTW,DDTC8,PDTC8 andfaultqueues)','C_DMA_other_metadata(BufferDepth+8)','C_existing_R_policy','C_native_DMA_data_ports'],
    'accounting':'AW8packet/phase/ring/G/TLphase/held/inputbankowner are in managedAWsum and must not be addedagain; each native reusedtracking/shadow is paidonce, alternate optimization labeled',
}
envelope={
    'schema':'ordinary-B5-37-field-cost-envelope-v1','created_utc':NOW,'scope_contract':'ordinary-contract.json',
    'status':'logical_field/port_upper_envelope, not architecture lowerbound or synthesized area',
    'catalog_fields_count':len(fields),'fields':fields,'native_components':native_components,
    'managed_AW_core_fields':core,'managed_AW_core_U1_queue_bits':core_total,'managed_AW_core_U1_tap_bits':core_tap_total,
    'managed_AW_core_formula':'8*(153+U)+16phase+24bankring+24Grefs+10Gctrl+4TLphase+3held+4RR+23queueinput = 1332+8U',
    'phase_dependent_resident_packet_fields':{'formula':'nraw*(153+U)+nready*(103+U); nraw+nready<=8; heldpacket separately in nativeAWspill','all_raw_U1':1232,'all_prepared_U1':832,'raw_storage_capacity_U1':1232,'new_copy_storage':0,'native_AWspill_declared_packet_only_U1':416,'logical_residency_not_physical_area':True},
    'ports_upper_envelope':'2R/2dataW/bank plus up to3phase writes; TL single request and single response; global submit1; successW1; inputW1; errorconsume≤2; all held/compare/mux ranges in37fields',
    'alternative_encodings':['allow preparedphase metadata overlay in released50extensionbits, but rawphase/outstanding state still charged','freezeIDcontext table can deduplicate percell constants if bothmechanisms pay same informationtable; not used to claim lowerbound','remove only new WRITE shadow copies; original READ req118/resp185/state3 perbank retain612bits with exclusive bank owner arbitration','native selectedfourID counters may specialize decode but declaredfull16ID arrays retained in referenceenvelope'],
    'acceptance_reasons':['complete normal success/fault ownership exists using existing targetbinding and Bspills; no newBrecordperdebt','normalTLrsp permits rawaddrinplace reuse and ownerrelease','internalAWspill ownership permits cell/Gretirement with boundedheldrecord and no stalecellreferences','ordinaryFIFO no32bitrank or fullpreparedcopy necessary'],
    'rejected_comparisons':['copyonlyordinarybaseline','globalBdebt8 cap','celltiedtoW/B unnecessarily','internalspillAWcalledphysicalAW','oneentryenabledspillcost','portsfree because FIFOhead'],
    'measurement_values':{'software_cpu_application_cost':None,'lut':None,'ff':None,'bram':None,'area':None,'timing_fmax':None,'energy':None},
    'total_logical_bits':'finite symbolic sum: managedAWcore + preservednativecomponents + namedcommonunknowns; no single measured total claimed',
}
save('cost-envelope.json',envelope)

checks={
    'catalog37':len(fields)==37,
    'raw_AW_width':sum([64,4,8,3,2,1,4,3,4,4,6,U,24,1,20,2,1,1,1])==154,
    'physical_AW_width':sum([64,4,8,3,2,1,4,3,4,4,6,U])==104,
    'TLreq_width':sum([64,24,1,20,6,1,1,1])==118,
    'TLrsp_width':sum([1,1,1,56,1,102,3,1,12,7])==185,
    'bankcells4plus4':8==2*4,
    'W_FIFO16plus16_bytes':32*64//8==256,
    'productionDMApayload':sum([512,512,3,3])*8==8240,
    'DMA_Wlast_metadata':2*sum([520,520,11,11])==2124,
    'inputOR_full_selected_upper':15+3*14==57,
    'cutOR_full_selected_upper_perbank':7+6==13,
    'managed_core_formula':core_total==1332+8*U,
    'tap_saves_state_but_pays_interface':core_total-core_tap_total==19,
    'phys_spill_AW_actual2packet_per_bank':native_packets['per_bank_completion_AW_and_select_spills']['bits']==428,
}
save('static-calculation.json',{'schema':'r2_control_single_new_static_arithmetic-v1','created_utc':NOW,'execution_number':1,'checks':checks,'all_passed':all(checks.values()),'core_queue_bits':core_total,'core_tap_bits':core_tap_total,'not_proof_of_RTL_behavior':True})
print(json.dumps({'artifact_files':['ordinary-contract.json','cost-envelope.json','source-manifest.json','static-calculation.json'],'catalog_count':len(fields),'checks_passed':all(checks.values()),'checks_failed':[k for k,v in checks.items() if not v],'core_queue_bits':core_total,'core_tap_bits':core_tap_total},ensure_ascii=False))
