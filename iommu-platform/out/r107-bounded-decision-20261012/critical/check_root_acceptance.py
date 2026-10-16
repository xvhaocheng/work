#!/usr/bin/env python3
"""Read source snapshots and raw CPU replay records; never execute a DUT."""
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

OUT = Path(__file__).resolve().parents[1]
PLATFORM = OUT.parents[1]
checks = []


def check(name, condition, detail=None):
    checks.append({'name': name, 'passed': bool(condition), 'detail': detail})
    if not condition:
        raise AssertionError(name)


def load(path):
    return json.loads(path.read_text())


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


contract = load(OUT / 'instance/finite-contract.json')
acceptance = load(OUT / 'instance/acceptance.json')
jobs = load(OUT / 'instance/expanded-restore-jobs.json')
geometry = contract['geometry']
model = load(OUT / 'instance/source/opt-125m-config.json')
check('accepted_frozen_contract_hash', acceptance['contract_sha256'] ==
      digest(OUT / 'instance/finite-contract.json'))
check('actual_complete_model_dimensions', model['num_hidden_layers'] ==
      geometry['num_hidden_layers'] == 12 and model['num_attention_heads'] ==
      geometry['total_attention_heads'] == 12 and model['hidden_size'] == 768)
check('two_distinct_head_shards', geometry['heads_per_worker'] == 6 and
      geometry['tensor_parallel_size'] == 2 and geometry['head_size'] == 64)
check('CPU_only_scope', not any(acceptance[k] for k in
      ('GPU', 'FPGA', 'native_attention', 'physical_DMA_allocations', 'G0')))
check('no_manufactured_hardware_events', not acceptance['hardware_events'] and
      not acceptance['cycle_or_deadline_fields'])
check('proxy_valid_byte_scope_present', bool(acceptance['valid_byte_claim_scope']))
check('finite_capacity_not_forced', acceptance['live_unique_slots'] == 3 and
      acceptance['source_conservative_reservation_demand_slots'] == 5 and
      geometry['gpu_pool_slots_per_worker'] == 8)
check('full_KV_byte_formula', 2 * 12 * 2 * 3 * 16 * 6 * 64 * 2 ==
      acceptance['full_restore_bytes'] == 1769472)
check('complete_copy_count', len(jobs) == acceptance['job_count'] == 144)
check('prior_swapout_counted', acceptance['full_prior_swapout_bytes'] == 1769472 and
      acceptance['two_direction_bytes'] == 3538944)
check('valid_plus_padding', acceptance['semantic_valid_bytes'] == 663552 and
      acceptance['tail_padding_bytes'] == 1105920 and
      acceptance['semantic_valid_bytes'] + acceptance['tail_padding_bytes'] ==
      acceptance['full_restore_bytes'])

raw = {}
for item in load(OUT / 'instance/raw-manifest.json'):
    path = OUT / 'instance' / item['file']
    check('raw_size:' + item['file'], path.stat().st_size == item['bytes'] == 2359296)
    check('raw_hash:' + item['file'], digest(path) == item['sha256'])
    raw[path.name] = path.read_bytes()

mapping = {7: 5, 6: 7, 5: 6}
seen = set()
slot_bytes = 12288
for job in jobs:
    worker, layer = job['worker'], job['layer']
    component = ('K', 'V').index(job['component'])
    src_block, dst_block = job['source_block'], job['destination_block']
    key = (worker, layer, component, src_block)
    check('unique_job:' + str(job['id']), key not in seen)
    seen.add(key)
    base = layer * 196608 + component * 98304
    so, do = base + src_block * slot_bytes, base + dst_block * slot_bytes
    check('job_binding:' + str(job['id']), worker in (0, 1) and 0 <= layer < 12 and
          mapping.get(src_block) == dst_block and job['bytes'] == slot_bytes and
          job['source_offset'] == so and job['destination_offset'] == do and
          job['PA'] is None and job['source_IOVA'] is None and
          job['destination_IOVA'] is None and not job['hardware_issue_or_done_observed'])
    initial = raw[f'worker{worker}-initial_gpu.bin']
    source = raw[f'worker{worker}-cpu_source.bin']
    restored = raw[f'worker{worker}-restored_gpu.bin']
    check('complete_prior_and_restore_bytes:' + str(job['id']),
          initial[so:so + slot_bytes] == source[so:so + slot_bytes] ==
          restored[do:do + slot_bytes])
for worker in (0, 1):
    for layer in range(12):
        for component in range(2):
            base = layer * 196608 + component * 98304
            guard = bytes([0xd9]) * (5 * slot_bytes)
            check(f'unused_slot_guards:{worker}:{layer}:{component}',
                  all(raw[f'worker{worker}-{kind}.bin'][base:base + len(guard)] == guard
                      for kind in ('initial_gpu', 'cpu_source', 'restored_gpu')))
check('all_finite_identities', seen == {(w, l, c, s) for w in (0, 1)
      for l in range(12) for c in range(2) for s in (5, 6, 7)})

# Coalescing changes the descriptor count, not the work or a use barrier.
check('ordinary_two_contiguous_pairs', mapping[5] + 1 == mapping[6])
coalesced_count = 2 * 12 * 2 * 2
check('coalesced_complete_bytes', coalesced_count == 96 and
      2 * 12 * 2 * (24576 + 12288) == 1769472)
check('separate_pool_2MiB_geometry', 4 * 2 * 2097152 == 16777216 and
      4 * 2359296 == 9437184)

for item in load(OUT / 'protected-inputs-before.json'):
    check('protected:' + item['path'], digest(PLATFORM / item['path']) == item['sha256'])
platform_validation = load(OUT / 'platform/validation-record.json')
check('platform_record_is_actual_once', platform_validation['passed'] and
      platform_validation['checks_total'] == 200 and not platform_validation['checks_failed'])

# Critical source excerpts are checked directly against current files. This is
# provenance verification, not another run of the old model or its matrix.
for record in load(OUT / 'critical/root-source-review.json')['records']:
    path = Path(record['absolute_path'])
    check('root_source_hash:' + record['path'], digest(path) == record['sha256'])
    lines = path.read_text().splitlines()
    for excerpt in record['excerpts']:
        start, end = excerpt['start_line'], excerpt['end_line']
        check(f'root_excerpt:{record["path"]}:{start}:{end}',
              '\n'.join(lines[start - 1:end]) == excerpt['text'])

report = {
    'stage': 108,
    'actor': 'Codex root',
    'executed_at_utc': datetime.now(timezone.utc).isoformat(),
    'status': 'PASS_current_source_and_record_review_only',
    'checks_total': len(checks),
    'checks': checks,
    'read_only_source_and_raw_records': True,
    'new_KV_CPU_source_check_runs': 1,
    'root_independent_record_checks': 1,
    'new_DUT_or_board_runs': 0,
    'stage106_matrix_reruns': 0,
    'ordinary_coalescing_descriptors': coalesced_count,
    'translation_loss_proven': False,
    'native_attention_required_read_bytes_proven': False,
    'actual_allocation_leaf_cache_costs_proven': False,
    'exact_hardware_performance_proven': False,
}
(OUT / 'critical/root-evidence-acceptance.json').write_text(
    json.dumps(report, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({'status': report['status'], 'checks': len(checks),
                  'full_restore_bytes': 1769472, 'coalesced_descriptors': coalesced_count,
                  'DUT_or_board': 0, 'stage106_matrix_reruns': 0}))
