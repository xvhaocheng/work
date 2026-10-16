#!/usr/bin/env python3
"""Postprocess the accepted original receipt; no device access or experiments."""
from pathlib import Path
import json
import sys
import hashlib

sys.dont_write_bytecode = True
R = Path(__file__).resolve().parents[1]
P = R.parent
sys.path.insert(0, str(P / 'host'))
import validate_finite_pilot as v
import calibrate_finite_timer as t

raw = R / 'live/finite-pilot-result/receipt.json'
d = json.loads(raw.read_text())
run = d['run']
freq = json.loads((R/'live/actual-timer-bounds.json').read_text())['common_constant_rate_compatible_bounds']
assert json.loads((R/'validation/actual-finite-pilot-validation.json').read_text())['accepted']
rows = []
for i, s in enumerate(run['stages']):
    assert s['reached'] == 1 and s['status'] == 0
    delta = {k: s['end'][k] - s['begin'][k] for k in s['begin']}
    assert all(x > 0 for x in delta.values())
    rows.append({'index': i, 'name': v.STAGES[i], 'status': 0,
                 'begin_raw': s['begin'], 'end_raw': s['end'], 'delta_raw': delta,
                 'conditional_Dell_seconds': {k: t.stage_elapsed_bounds(delta[k], freq[k]) for k in t.COUNTERS}})
phases = []
for i in range(2):
    o, c = run['observer'][i], run['composition'][i]
    b, a = run['hpm_before'][i], run['hpm_after'][i]
    assert c[8] == o[0] and c[7] == i + 1 and c[9] == 0
    assert c[12] == 1728 and c[14] == (8 if i == 0 else 0) and c[26] == (8 if i == 0 else 0)
    phases.append({'phase': 'prior_swapout' if i == 0 else 'restore',
        'bytes': run['swapout_bytes' if i == 0 else 'restore_bytes'],
        'logical_batches': 48 if i == 0 else 96, 'hardware_tasks': 432,
        'observer_cycles': o[0], 'hardware_segment_cycles': o[1], 'refill_segment_cycles': o[3],
        'refill_share': o[3]/o[0], 'v_only_pending_cycles': o[11], 'neither_pending_cycles': o[10],
        'AR_AW_RLAST_B_each': o[14:18], 'lane_AR_AW_not_ready_cycles': o[18:20],
        'shared_AR_AW_not_ready_cycles': o[28:30], 'closing_outstanding': [o[32],o[33]],
        'normal_V_lookup_hit_miss': c[12:15], 'V_fills': c[26],
        'cross_PSCID_replacement': c[29], 'within_window_invalidations': c[31],
        'explicit_batch_GAP_cycles': c[24:26],
        'HPM_delta_globalcycles_missV_inhibitedA_S1V_S1A_S2V_S2A': [x-y for x,y in zip(a,b)],
        'HPM_snapshot_non_atomic': True, 'ordinary_S0_config': hex(c[2]), 'window_sequence': c[7]})

def wall(path):
    return json.loads(path.read_text())['dell_wall_seconds']

record = {
    'actual_full_finite_pilot_accepted': True, 'source_receipt': str(raw),
    'receipt_sha256': hashlib.sha256(raw.read_bytes()).hexdigest(),
    'boot_ID': '370a9dd6-7d1a-4190-8395-acaf3e25f69c',
    'kernel_release': '6.5.0-rc2-r107-ordinary-stage111-r1',
    'timing_conditions': 'same continuous counter/boot/hart and rate within compatible bounds; CPU also no debug/PMU write; relative Dell CLOCK_MONOTONIC, no quantified SI error/drift; CPU/TIME read points non-atomic',
    'stages': rows, 'phases': phases, 'frequency_bounds': freq,
    'board_userspace_nominal_ns': d['userspace_cost_nominal_ns'], 'userspace_board_nominal_not_Dell': True,
    'memory': {'logical_pools_bytes': 9437184, 'DMA_backing_bytes': 16777216,
        'kernel_seed_refs_bytes': 4718592, 'kernel_batch_capacity_bytes': 192*136,
        'kernel_leaf_capacity_bytes': 4096*88, 'kernel_run_bytes': 4520,
        'userspace_buffers': d['userspace_memory'], 'guard_coverage_bytes': 13238272,
        'guard_overlap_with_logical_checks': True, 'extra_backing_slack_bytes': 7340032,
        'actual_table_pages': 2, 'actual_table_backing_bytes': 8192,
        'table_new_allocation_vs_existing_root': 'not separately exposed',
        'exact_allocator_pagecache_transient_peak_RAM': 'not measured; byte contracts do not imply exact peak RSS'},
    'release_status': 0, 'quarantined': 0, 'data_errors': 0,
    'cost_classification': {
        'ordinary_path': 'stable allocation/maps,cache handoffs,private checked invalidation boundaries,batch supply,hardware DONE/quiet,revoke/free',
        'experiment_replay_and_evidence': 'immutable seed/guard initialization,leaf/CPU-PA census,all-byte/slack checks,copy-to-user/metadata,CLI SHA/memcmp/raw files,build/deploy/transport; counted but not nativeGPU/attention costs'},
    'dell_wall_seconds': {
        'CLI_entire_wrapper': wall(R/'live/finite-pilot-once-identity-result.json'),
        'kernel_loader_OpenSBI_r0_build': 59.065947553142905,
        'corrective_r1_build': 59.541752237826586,
        'r0_SD_write_hash': 954.607105051, 'r1_SD_write_hash': 959.966255384,
        'r0_bit_and_boot_capture': 474.31074230838567, 'r1_bit_and_boot_capture': 474.1531378766522,
        'r1_payload_download_hash_dryrun': 234.239701922,
        'r1_assets_download_hash': wall(R/'live/new-data-fetch-identity-result.json'),
        'original_backup_host_receive': 89.988626564,
        'r1_raw_export_UART': wall(R/'live/finite-result-export-identity-result.json'),
        'r1_raw_receive': json.loads((R/'live/finite-transfer-root-acceptance.json').read_text())['dell_receive_wall_seconds']},
    'overlap_rules': ['stage11 contains0..10','kernel stages nest in userspace ioctl/CLI wrapper','receiver nests in export wrapper','construction/deployment/evidence wall is not production restoration latency'],
    'limitations': [
        'No nativeGPU compute/arrival/deadline or per-beat/every-task independent NEXT trace',
        'slack check and post-revoke PTE absence represented by reviewed kernel path/status, not separate returned raw snapshots',
        'VMA+IOFENCE helper uses quiesced noATS/noS2 CQ-fetch-head criterion for prior VMA pulse, not general fence-execution proof',
        'out/restore48/96 batch layouts differ; hardware-cycle difference not uniquely attributable to8PTWs',
        'REFILL mixes frontend between-task behavior and software between-batch supply',
        'zero sampled AR/AW backpressure does not prove every internal/hit-path latency absent'],
    'old_failed_attempt_preserved': True}
(R/'costs-and-boundaries.json').write_text(json.dumps(record,ensure_ascii=False,indent=2)+'\n')
route = {'copied_nested_validator_entry_failed_import': True,
    'reason': 'fixed ancestor path resolved /iommu-platform/out/out/ai-mandatory-work-g0-20261012/validator/validate_g0_records.py',
    'actual_successful_entry': str(P/'host/validate_finite_pilot.py'),
    'successful_entry_sha256': hashlib.sha256((P/'host/validate_finite_pilot.py').read_bytes()).hexdigest(),
    'actual_new_receipt_validations_completed': 1, 'accepted_validator_modified': False, 'old_matrix_replayed': False}
(R/'validation/validator-entry-route.json').write_text(json.dumps(route,indent=2)+'\n')
print(json.dumps({'stages': [{'index': x['index'], 'name': x['name'], 'delta': x['delta_raw'],
    'TIME_seconds': [x['conditional_Dell_seconds']['time_ticks']['lower_decimal_outward'],
                     x['conditional_Dell_seconds']['time_ticks']['upper_decimal_outward']]} for x in rows], 'phases': phases},indent=2))
