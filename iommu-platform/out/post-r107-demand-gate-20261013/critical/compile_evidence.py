#!/usr/bin/env python3
"""Compile source excerpts and bounded arithmetic; never run a DUT/validator."""
from pathlib import Path
import datetime, hashlib, json

ROOT = Path('/home/xvhaocheng/work/iommu-platform')
OUT = ROOT / 'out/post-r107-demand-gate-20261013'
AI = 'out/ai-platform-constraints-20261012/ai-block-source/vllm-source/'
R111 = 'out/r107-ordinary-pilot-20261012/'
def write(name, value):
    (OUT / name).write_text(json.dumps(value, ensure_ascii=False, indent=2) + '\n')
def digest(path):
    b = path.read_bytes()
    return {'path': str(path), 'bytes': len(b), 'sha256': hashlib.sha256(b).hexdigest()}

specs = [
    ('native-worker', AI+'vllm/worker/worker.py', [(17,23),(39,50),(180,215),(243,285),(289,304)]),
    ('pool-event', AI+'vllm/worker/cache_engine.py', [(39,47),(66,100),(102,125)]),
    ('native-layer', AI+'vllm/model_executor/models/opt.py', [(55,94),(111,158),(200,227)]),
    ('native-use', AI+'vllm/model_executor/layers/attention.py', [(77,96),(119,164)]),
    ('native-memcpy', AI+'csrc/cache_kernels.cu', [(9,47)]),
    ('collective-linear', 'out/post-r107-demand-gate-20261013/source/tp-layers.py', [(320,340),(355,376),(415,448)]),
    ('collective-buffer', 'out/post-r107-demand-gate-20261013/source/parallel_state.py', [(199,211),(488,490),(520,570)]),
    ('real-use-proxy', R111+'repair-1/source/linux/drivers/misc/idma_finite.h', [(475,543),(689,716)]),
    ('real-serial-TC', 'out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv', [(625,669),(674,725)]),
    ('CPU-master', 'out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_xilinx.sv', [(709,728)]),
    ('OPT-shape', 'out/r107-bounded-decision-20261012/instance/source/opt-125m-config.json', [(1,28)]),
]
evidence = []
for name, rel, ranges in specs:
    p = ROOT/rel; lines = p.read_text().splitlines()
    sections = []
    for lo, hi in ranges:
        assert 1 <= lo <= hi <= len(lines), (rel, lo, hi, len(lines))
        sections.append({'first_line':lo,'last_line':hi,'text':'\n'.join(lines[lo-1:hi])})
    evidence.append({'id':name, **digest(p), 'excerpts':sections})
write('critical/root-source-review.json', {'reviewer':'Codex root','review_type':'direct primary source, not worker summary', 'items':evidence})

rp = ROOT/R111/'repair-1/live/finite-pilot-result/receipt.json'
r = json.loads(rp.read_text())['run']
vp = ROOT/R111/'repair-1/validation/actual-finite-pilot-validation.json'
v = json.loads(vp.read_text())
assert v['accepted'] and v['receipt_sha256'] == digest(rp)['sha256']
assert r['status'] == r['release_status'] == r['quarantined'] == 0
phases=[]
for i, name in enumerate(['priorout','restore']):
    phases.append({'name':name,'hpm_before':r['hpm_before'][i], 'hpm_after':r['hpm_after'][i],
        'hpm_delta':[b-a for a,b in zip(r['hpm_before'][i],r['hpm_after'][i])],
        'composition_prefix_32':r['composition'][i][:32], 'observer':r['observer'][i]})
assert phases[1]['composition_prefix_32'][12:15] == [1728,1728,0]
assert phases[1]['hpm_delta'][1:] == [0]*6
write('critical/existing-raw-evidence.json', {
    'kind':'read selected original raw records; no revalidation run or new device experiment',
    'receipt':digest(rp),'existing_validator_receipt':digest(vp),'validator_reexecuted':False,
    'run_status':{k:r[k] for k in ['status','release_status','quarantined','leaf_count','backing_bytes','worker_layers','joint_layers','restore_bytes']},
    'phases':phases,
    'scope':'old R107 finite card only; use proxies are CPU checks after full restore; absent native collective',
})

ep=ROOT/'out/mac-e3-controlled-20260918-164635-run-rk48bhte/all-original-pairs.json'
pairs=json.loads(ep.read_text()); pair=pairs[8]
assert pair['success'] and pair['input']['chunk_bytes']==64
assert [x['sequence'] for x in pair['rounds']]==[17,18]
rows=[]
for x in pair['rounds']:
    w=x['e3_words']
    rows.append({'sequence':x['sequence'],'delay':x['delay'],
        'selected_original_words':{str(i):w[i] for i in [4,39,41,42,69,70,71,73]},
        'A_remaining':w[41]-w[4], 'V_remaining':w[70]-w[4],
        'V_own':w[73]-w[71], 'pre_e4_e69':w[4]-w[69],'pre_e4_e39':w[4]-w[39]})
assert rows[1]['A_remaining']-rows[0]['A_remaining']==129
assert rows[1]['V_remaining']-rows[0]['V_remaining']==129
write('critical/existing-e3-boundary.json', {
    'kind':'old finite64B v6 causal pair; no replay, no new current-bit claim',
    'source':digest(ep),'JSON_pointer':'/8', 'source_pair':pair,'readings':rows,
    'existing_review':digest(ROOT/'out/e3-controlled-review-20260918/review-summary.json'),
    'scope':'A-miss remaining service can delay pending V-hit in this serial implementation; artificial diagnostic delay is not application arrival or demand proof',
})

cfg=json.loads((ROOT/'out/r107-bounded-decision-20261012/instance/source/opt-125m-config.json').read_text())
assert cfg['hidden_size']==768 and cfg['num_hidden_layers']==12 and cfg['torch_dtype']=='float16'
valid=2; padded=valid+(-valid)%8; hidden=cfg['hidden_size']; size=2; ranks=2; calls=2*cfg['num_hidden_layers']
b=padded*hidden*size; useful=valid*hidden*size; pool=2359296; backing=4194304
geometry={'kind':'source-derived normative shapes for unchanged two-sequence decode continuation; not native execution or new FPGA workload',
    'valid_rows':valid,'padded_rows':padded,'hidden_size':hidden,'element_bytes':size,'ranks':ranks,'layers':12,
    'row_linear_collective_occurrences_per_group':calls,'rank_API_invocations':ranks*calls,
    'active_prefix_bytes_per_rank_per_occurrence':b,'useful_prefix_bytes_per_rank':useful,'padding_bytes_per_rank':b-useful,
    'all_occurrences_rank_input_operand_bytes':ranks*calls*b,
    'all_occurrences_rank_result_operand_bytes':ranks*calls*b,
    'all_occurrences_useful_rank_partial_bytes':ranks*calls*useful,
    'all_occurrences_unique_useful_reduced_result_bytes':calls*useful,
    'network_bytes':None,'native_DMA_bytes':None,'NCCL_algorithm':None,'native_compute_times':None,'arrival_times':None,'deadlines':None,
    'maximum_configured_rows_M':None,'persistent_buffer_bytes_formula':'per-rank M*768*2; group R*M*768*2; M is not supplied by old finite KV contract',
    'ordinary_geometry_witness':{'addresses':'pool-relative only; no current physical/IOVA allocations claimed','pool_backing_bytes':backing,
        'old_logical_pool_bytes':pool,'new_buffer_start':pool,'new_buffer_end_exclusive':pool+b,
        'active_buffer_count':2,'active_prefix_total_bytes':ranks*b,
        'does_not_overlap_old_logical_bytes':True,'additional_large_leaves_for_active_prefix':0,
        'old_total_backing_slack_bytes':7340032,'remaining_slack_bytes_if_authorized':7340032-ranks*b,
        'condition':'new contract may authorize each rank output buffer in its owned GPU-pool backing slack; old R107 guard contract unchanged, no writes performed; warm same DID/mapping, no external eviction/invalidation. Full-M buffer/weights/collective backend resources not implied covered.'},
    'not_double_counted':'in-place reusable rank buffers; inputs and results are logical endpoint materialization, not additive allocated storage or wire traffic',
}
assert b==12288 and useful==3072 and ranks*calls*b==589824 and calls*useful==73728
assert pool%4096==0 and pool+b<=backing and pool//2097152==(pool+b-1)//2097152
write('source/finite-consumer-byte-contract.json',geometry)

protected=[ROOT/'out/ai-platform-constraints-20261012/ai-block-source/vllm-67d96c29fba9b72cb4c4edbc26211c208a00ebdd.tar.gz',
    ROOT/'out/r107-bounded-decision-20261012/instance/finite-contract.json',
    ROOT/R111/'next-goal-contract.json',ROOT/R111/'root-result-acceptance.json',
    ROOT/R111/'execution-ledger.json',rp,vp,
    ROOT/R111/'repair-1/costs-and-boundaries.json',ep,
    ROOT/'out/e3-controlled-review-20260918/review-summary.json']
protected.extend(ROOT/rel for _,rel,_ in specs if not rel.startswith('out/post-r107-demand-gate'))
write('protected-inputs-before.json', [digest(p) for p in dict.fromkeys(protected)])
write('execution-ledger.json', {
    'stage':112,'status':'active; bounded adjudication artifacts in progress','Codex_only':True,
    'models_requested':{'root':'Sol/ultra user preference; no new runtime model audit','child':'gpt-6.1-sol/ultra'},
    'subagent_spawn_attempts':1,'subagent_started':0,'subagent_error':'agent thread limit reached',
    'subagent_task':'exclusive native consumer/dependency boundary','subagent_result':None,'root_completed_critical_source_task':True,
    'DeepSeek_calls':0,'Claude_calls':0,'Astra_calls':0,
    'new_primary_systems':0,'direct_dependency_archive_files_extracted':4,'new_network_fetches':0,
    'new_scenes':0,'prior_replays_rerun':0,'existing_validators_rerun':0,'new_DUT_executions':0,
    'new_builds':0,'new_RTL':0,'new_Linux_changes':0,'new_board_commands':0,'new_SD_writes':0,'new_bit_loads':0,'new_G0':0,'new_G1':0,
    'corrections_preserved':[
        'first evidence compiler rejected OPT config excerpt 1-31 because original has28 lines; corrected to1-28; no old validator or experiment rerun',
        'requested rv_iommu_trans_ctl.sv/tl.sv absent in frozen overlay; actual wrapper path read instead; no source edits',
        'initial archive basename tensor_parallel.py/communication_op.py matched zero members; exact directory layers.py/mappings.py/__init__.py and direct parallel_state.py were read subsequently',
        'parallel_state.py is present in original archive but absent in previous 14-file extraction; read exact archive member into new artifact directory',
        'some batched exploratory tool output truncated; decisive source ranges and raw fields read in bounded followups; no absent field inferred from truncation'],
    'performed':['read actual instructions/status/successor contract','actual create_goal with no budget','snapshot five shared docs before update',
        'read unchanged source and selected raw records directly','extract four bounded direct dependency files from unchanged official archive',
        'source-only byte arithmetic and legal ordinary placement witness; no generated application values/timing','save evidence, costs, decision, next contract and sync docs'],
    'timing_cost_note':'new gate is source/contract work only. Prior FPGA costs remain labelled prior stage111; no native GPU or new execution costs measured.',
    'paper_complete':False,
})
print(json.dumps({'source_items':len(evidence),'protected_inputs':len(json.loads((OUT/'protected-inputs-before.json').read_text())),'padded_operand_bytes':b,'logical_rank_inputs':ranks*calls*b,'new_experiments':0},ensure_ascii=False))
