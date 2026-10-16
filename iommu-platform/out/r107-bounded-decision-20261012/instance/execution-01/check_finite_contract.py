#!/usr/bin/env python3
"""One frozen source-method/CPU-byte check. No GPU/DMA timing or adapter."""
import ast
import copy
import enum
import hashlib
import json
import math
import sys
import time
import typing
from pathlib import Path
from types import SimpleNamespace

HERE = Path(__file__).resolve().parent
PLATFORM = HERE.parents[2]
SOURCE = PLATFORM / 'out/ai-platform-constraints-20261012/ai-block-source'
VLLM = SOURCE / 'vllm-source'
OLD = PLATFORM / 'out/ai-mandatory-work-g0-20261012/validator/validate_g0_records.py'


def selected(path, names, namespace):
    tree = ast.parse(path.read_text(), filename=str(path))
    kept = [node for node in tree.body
            if isinstance(node, (ast.ClassDef, ast.FunctionDef)) and node.name in names
            or isinstance(node, ast.Assign) and any(
                isinstance(t, ast.Name) and t.id in names for t in node.targets)]
    exec(compile(ast.Module(body=kept, type_ignores=[]), str(path), 'exec'), namespace)


def write_json(name, value):
    (HERE / name).write_text(json.dumps(value, ensure_ascii=False, indent=2) + '\n')


start = time.monotonic()
contract_path = HERE / 'finite-contract.json'
frozen_raw = contract_path.read_bytes()
c = json.loads(frozen_raw)
g = c['geometry']
base_ns = {'hashlib': hashlib}
selected(OLD, {'Rejection', 'require', 'sha'}, base_ns)
require, sha = base_ns['require'], base_ns['sha']
require(c['freeze_state'] == 'frozen_before_static_source_check', 'contract freeze missing')
source_manifest = json.loads((SOURCE / 'source-manifest.json').read_text())
source_files = {'vllm/block.py', 'vllm/sequence.py', 'vllm/core/block_manager.py',
                'vllm/core/scheduler.py', 'vllm/worker/cache_engine.py',
                'vllm/worker/worker.py', 'vllm/config.py', 'vllm/engine/llm_engine.py',
                'vllm/model_executor/models/opt.py',
                'vllm/model_executor/layers/attention.py', 'csrc/cache_kernels.cu'}
for entry in source_manifest['files']:
    if entry['official_repo_path'] in source_files:
        require(sha((SOURCE / entry['file']).read_bytes()) == entry['sha256'],
                f"pinned source changed: {entry['file']}")
require(sha((HERE / c['source']['model_config_file']).read_bytes()) ==
        c['source']['model_config_sha256'], 'official model config changed')
model = json.loads((HERE / c['source']['model_config_file']).read_text())
for field in ('num_hidden_layers', 'hidden_size'):
    require(model[field] == g[field], f'model {field}')
require(model['num_attention_heads'] == g['total_attention_heads'] and
        model['torch_dtype'] == g['dtype'], 'official complete heads/dtype')

ns = dict(vars(typing), copy=copy, enum=enum, _BLANK_TOKEN_ID=-1,
          Device=enum.Enum('DeviceProxy', {'GPU': 'gpu', 'CPU': 'cpu'}),
          SamplingParams=object)
selected(VLLM / 'vllm/block.py', {'LogicalTokenBlock', 'PhysicalTokenBlock'}, ns)
selected(VLLM / 'vllm/sequence.py',
         {'SequenceStatus', 'SequenceData', 'Sequence', 'SequenceGroup'}, ns)
selected(VLLM / 'vllm/core/block_manager.py',
         {'BlockAllocator', 'BlockSpaceManager', 'BlockTable'}, ns)
Seq, Status, Group, Manager = [ns[n] for n in
    ('Sequence', 'SequenceStatus', 'SequenceGroup', 'BlockSpaceManager')]
seqs = [Seq(s['seq_id'], 'research synthetic tokens',
            c['group']['shared_prefix_token_ids'], g['tokens_per_block'])
        for s in c['group']['sequences']]
group = Group(c['group']['request_id'], seqs, None, None)
manager = Manager(g['tokens_per_block'], g['gpu_pool_slots_per_worker'],
                  g['cpu_pool_slots_per_worker'])
manager.allocate(group)
for seq, desc in zip(seqs, c['group']['sequences']):
    seq.status = Status.RUNNING
    seq.append_token_id(desc['research_tail_token_id'],
                        {desc['research_tail_token_id']: 0.0})
    require(manager.append_slot(seq) is None, 'unexpected COW')
initial_tables = [manager.get_block_table(s) for s in seqs]
require(initial_tables == [[7, 6], [7, 5]], 'source initial pool IDs')
require(len(manager._get_physical_blocks(group)) == 3, 'shared prefix not deduplicated')
swapout = manager.swap_out(group)
require(swapout == {5: 5, 6: 6, 7: 7}, 'source swapout dictionary')
cpu_tables = [manager.get_block_table(s) for s in seqs]
for seq, desc, table in zip(seqs, c['group']['sequences'], cpu_tables):
    require(table == desc['cpu_block_table_before_restore'], 'frozen CPU table')
    seq.status = Status.SWAPPED
require(manager.get_num_free_gpu_blocks() == 8 and manager.can_swap_in(group),
        'source restore capacity/reservation not met')
require(sorted((b.block_number, b.ref_count)
               for b in manager._get_physical_blocks(group)) == [(5, 1), (6, 1), (7, 2)],
        'CPU refcounts')
swapin = manager.swap_in(group)
require(swapin == {int(k): v for k, v in
                    c['complete_restore_work']['CPU_to_GPU_block_mapping'].items()},
        'source restore dictionary')
for seq, desc in zip(seqs, c['group']['sequences']):
    seq.status = Status.RUNNING
    require(manager.get_block_table(seq) == desc['gpu_block_table_after_restore'],
            'frozen restored GPU table')
require(manager.get_num_free_gpu_blocks() == 5 and
        manager.get_num_free_cpu_blocks() == 8 and manager.can_append_slot(group),
        'source logical capacity/refcount conservation')
require(sorted((b.block_number, b.ref_count)
               for b in manager._get_physical_blocks(group)) == [(5, 2), (6, 1), (7, 1)],
        'GPU restored refcounts')
logical_free_before_copy = manager.get_num_free_cpu_blocks() == 8

cache_ns = dict(vars(typing), CacheConfig=object, ModelConfig=object,
                ParallelConfig=object, KVCache=typing.Tuple[object, object],
                torch=SimpleNamespace(dtype=object, tensor=lambda *args, **kw:
                                      SimpleNamespace(element_size=lambda: 2)))
selected(VLLM / 'vllm/worker/cache_engine.py', {'CacheEngine', '_get_dtype_size'}, cache_ns)
cache = SimpleNamespace(head_size=g['head_size'], num_heads=g['heads_per_worker'],
                        block_size=g['tokens_per_block'], dtype=g['dtype'])
engine = cache_ns['CacheEngine']
require(list(engine.get_key_block_shape(cache)) == g['key_block_shape'] and
        list(engine.get_value_block_shape(cache)) == g['value_block_shape'], 'source K/V shapes')
bytes_slot = math.prod(g['key_block_shape']) * g['element_bytes']
require(bytes_slot == g['component_slot_bytes'] ==
        math.prod(g['value_block_shape']) * g['element_bytes'], 'full K/V slot bytes')
require(g['head_size'] == model['hidden_size'] // model['num_attention_heads'] and
        g['heads_per_worker'] * g['tensor_parallel_size'] == model['num_attention_heads'] and
        g['num_hidden_layers'] == model['num_hidden_layers'], 'full geometry')

raw_dir = HERE / 'raw'
raw_dir.mkdir(exist_ok=True)
jobs, verified_layers, artifacts = [], [], []
actual_restore_bytes = actual_prior_bytes = semantic_bytes = 0
pool_bytes = g['packed_pool_logical_bytes_per_worker_per_endpoint']


def component_base(layer, component):
    return layer * g['layer_KV_pool_bytes'] + component * g['component_tensor_bytes']


def expected_slot(worker, layer, component, slot):
    payload = bytearray([0xA0 + 4 * worker + slot['semantic_index']]) * bytes_slot
    for head in range(g['heads_per_worker']):
        for token in range(slot['valid_tokens']):
            for dimension in range(g['head_size']):
                index = ((layer * 2 + component) * 6 * 3 * 16 * 64 +
                         head * 3 * 16 * 64 + slot['semantic_index'] * 16 * 64 +
                         token * 64 + dimension)
                word = (0x0400 + index % 0x77ff) | (worker << 15)
                require(word & 0x7c00 != 0x7c00, 'nonfinite synthetic fp16 code')
                flat = ((((head * 8 + dimension // 8) * 16 + token) * 8 + dimension % 8)
                        if component == 0 else (head * 64 + dimension) * 16 + token)
                payload[flat * 2:flat * 2 + 2] = word.to_bytes(2, 'little')
    return bytes(payload)


for worker in range(2):
    gpu = bytearray([0xd9]) * pool_bytes
    cpu = bytearray([0xd9]) * pool_bytes
    expected = {}
    for layer in range(g['num_hidden_layers']):
        for component in range(2):
            base = component_base(layer, component)
            for slot in c['group']['unique_slots']:
                payload = expected_slot(worker, layer, component, slot)
                expected[(layer, component, slot['cpu_source_block'])] = payload
                offset = base + slot['initial_gpu_block'] * bytes_slot
                gpu[offset:offset + bytes_slot] = payload
    initial = bytes(gpu)
    for layer in range(g['num_hidden_layers']):
        for component in range(2):
            base = component_base(layer, component)
            for src, dst in sorted(swapout.items()):
                cpu[base + dst * bytes_slot:base + (dst + 1) * bytes_slot] = \
                    gpu[base + src * bytes_slot:base + (src + 1) * bytes_slot]
                actual_prior_bytes += bytes_slot
    source_before = bytes(cpu)
    for layer in range(g['num_hidden_layers']):
        for component in range(2):
            base = component_base(layer, component)
            for src, dst in sorted(swapin.items()):
                so, do = base + src * bytes_slot, base + dst * bytes_slot
                gpu[do:do + bytes_slot] = cpu[so:so + bytes_slot]
                jobs.append({'id': len(jobs), 'worker': worker, 'layer': layer,
                             'component': ['K', 'V'][component], 'source_block': src,
                             'destination_block': dst, 'bytes': bytes_slot,
                             'source_pool': c['shards'][worker]['source_pool'],
                             'destination_pool': c['shards'][worker]['destination_pool'],
                             'source_offset': so, 'destination_offset': do,
                             'PA': None, 'source_IOVA': None, 'destination_IOVA': None,
                             'hardware_issue_or_done_observed': False})
                actual_restore_bytes += bytes_slot
        for component in range(2):
            base = component_base(layer, component)
            for slot in c['group']['unique_slots']:
                dst = swapin[slot['cpu_source_block']]
                do = base + dst * bytes_slot
                require(gpu[do:do + bytes_slot] ==
                        expected[(layer, component, slot['cpu_source_block'])],
                        f'worker{worker}/layer{layer} full slot including padding')
                semantic_bytes += slot['valid_tokens'] * g['heads_per_worker'] * \
                    g['head_size'] * g['element_bytes']
            for free_slot in range(5):
                offset = base + free_slot * bytes_slot
                require(gpu[offset:offset + bytes_slot] == bytes([0xd9]) * bytes_slot,
                        'unused destination slot guard changed')
                require(cpu[offset:offset + bytes_slot] == bytes([0xd9]) * bytes_slot,
                        'unused source slot guard changed')
        verified_layers.append({'worker': worker, 'layer': layer,
                                'complete_slot_copies': 6, 'CPU_proxy_verified': True,
                                'attention_or_hardware_event': False})
    require(bytes(cpu) == source_before, 'CPU source changed during restore')
    for kind, data in [('initial_gpu', initial), ('cpu_source', bytes(cpu)),
                       ('restored_gpu', bytes(gpu))]:
        name = f'raw/worker{worker}-{kind}.bin'
        (HERE / name).write_bytes(data)
        artifacts.append({'file': name, 'bytes': len(data), 'sha256': sha(data)})
require(actual_restore_bytes == c['complete_restore_work']['bytes_all_workers'] and
        actual_prior_bytes == c['prior_and_other_work']['prior_swapout']['full_bytes_all_workers'] and
        len(jobs) == c['complete_restore_work']['complete_job_count'] and
        semantic_bytes == c['complete_restore_work']['semantic_valid_bytes_all_workers'],
        'full byte/job conservation')
for seq in seqs:
    manager.free(seq)
require(manager.get_num_free_gpu_blocks() == 8 and
        manager.get_num_free_cpu_blocks() == 8, 'final logical pool conservation')
require(contract_path.read_bytes() == frozen_raw, 'contract changed during check')
write_json('expanded-restore-jobs.json', jobs)
write_json('raw-manifest.json', artifacts)
write_json('acceptance.json', {
    'status': 'PASS_source_methods_and_CPU_full_byte_contract_only',
    'contract_sha256': sha(frozen_raw), 'job_count': len(jobs),
    'full_restore_bytes': actual_restore_bytes, 'full_prior_swapout_bytes': actual_prior_bytes,
    'two_direction_bytes': actual_restore_bytes + actual_prior_bytes,
    'semantic_valid_bytes': semantic_bytes,
    'tail_padding_bytes': actual_restore_bytes - semantic_bytes,
    'all_12_layers_and_12_global_heads': True, 'unique_shared_prefix_copied_once_per_worker': True,
    'source_pool_unchanged_during_restore': True, 'all_unused_pool_slots_guard_preserved': True,
    'source_block_tables_and_refcounts_match_contract': True,
    'pinned_source_hashes_match_existing_stage107_manifest': True,
    'logical_free_occurs_before_CPU_byte_restore': logical_free_before_copy,
    'COW_bytes': 0, 'live_unique_slots': 3, 'GPU_slots_after_restore': {'live': 3, 'free': 5},
    'source_conservative_reservation_demand_slots': 5,
    'layer_verifications': verified_layers, 'raw_artifact_total_bytes': sum(x['bytes'] for x in artifacts),
    'hardware_events': [], 'cycle_or_deadline_fields': False, 'native_attention': False,
    'GPU': False, 'FPGA': False, 'physical_DMA_allocations': False, 'G0': False,
    'stage106_reuse': {'executed': ['Rejection', 'require', 'sha'],
        'not_executed': ['validate_job_events', 'validate_output'],
        'reason': 'event function binds old ready0/actual cycles and NEXT/DONE; output returns old six-tensor/9447936 metadata'},
    'source_AST_scope': {'actual_classes': ['LogicalTokenBlock', 'PhysicalTokenBlock',
        'SequenceStatus', 'SequenceData', 'Sequence', 'SequenceGroup',
        'BlockAllocator', 'BlockSpaceManager', 'CacheEngine'],
        'proxies': ['Device enum names only', 'SamplingParams unused',
                    'torch tensor element_size()=2 shape helper only'],
        'not_executed': ['scheduler timing/preemption', 'CUDA', 'attention/model', 'torch allocation']},
    'elapsed_static_CPU_check_seconds': time.monotonic() - start,
    'elapsed_time_is_not_DMA_or_inference_performance': True
})
print(json.dumps({'status': 'PASS', 'jobs': len(jobs), 'restore_bytes': actual_restore_bytes,
                  'semantic_bytes': semantic_bytes, 'prior_swapout_bytes': actual_prior_bytes,
                  'hardware_GPU_FPGA': False}))
