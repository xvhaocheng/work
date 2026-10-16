"""Record root's source/semantic acceptance; never executes a research model."""
from pathlib import Path
import datetime
import hashlib
import json

ROOT = Path(__file__).resolve().parent.parent
WORK = ROOT.parents[2]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read(name):
    return json.loads((ROOT / name).read_text())


def save(name, data):
    (ROOT / name).write_text(json.dumps(data, ensure_ascii=False, indent=2) + '\n')


now = datetime.datetime.now(datetime.timezone.utc).isoformat()
mapping = read('mapping/sources.json')
events = read('events/sources.json')
registered = []
for source in mapping['new_sources'] + mapping['reused_sources'] + events['sources']:
    path = Path(source['path'])
    assert path.exists() and sha(path) == source['sha256'], str(path)
    for lo, hi in source.get('read_ranges', []):
        assert 1 <= lo <= hi <= len(path.read_text().splitlines()), (str(path), lo, hi)
    registered.append({'path': str(path), 'sha256': sha(path),
                       'reported_ranges': source.get('read_ranges', []),
                       'range_read_owner': 'worker unless separately listed root direct review'})

root_ranges = {
    WORK / 'iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/sources/spdk_nvme_perf.c': [[1506, 1575], [1821, 1863]],
    ROOT / 'mapping/sources/spdk_env.c': [[45, 61], [80, 115]],
    ROOT / 'mapping/sources/spdk_memory.c': [[1046, 1119], [1512, 1538]],
    ROOT / 'mapping/sources/dpdk_eal_vfio.c': [[837, 915], [1399, 1435]],
    ROOT / 'mapping/sources/linux_iommu.c': [[2388, 2437]],
    ROOT / 'mapping/sources/linux_vfio_iommu_type1.c': [[1441, 1489]],
    ROOT / 'mapping/sources/spdk_nvme_pcie_common.c': [[628, 647], [675, 718], [1830, 1862]],
    ROOT / 'events/nvme_spec.h': [[1430, 1468], [4424, 4438]],
    WORK / 'iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv': [[35, 120], [152, 177]],
    ROOT / 'model/prototype.py': [[1, 42]],
    ROOT / 'model/replay.py': [[1, 91]],
    ROOT / 'model/verify.py': [[1, 110]],
}
for path, ranges in root_ranges.items():
    for lo, hi in ranges:
        assert 1 <= lo <= hi <= len(path.read_text().splitlines()), (str(path), lo, hi)
save('root/critical-source-check.json', {
    'schema': 'd-spdk-critical-source-check-v1', 'utc': now,
    'source_registrations_checked': len(registered), 'registered': registered,
    'root_direct_reviews': [{'path': str(p), 'sha256': sha(p), 'read_ranges': v}
                            for p, v in root_ranges.items()],
    'check_meaning': 'Hash/range checks plus directed substantive reads; no whole-source or human-read claim.',
    'receipt_bounds_correction': 'Receipt construction caught two sed range ends beyond EOF: 178 vs 177, 1865 vs 1862. Recorded ranges corrected to actual EOF; no source or model rerun.',
})
model = read('model/verification.json')
replay = read('root/semantic-replay-check.json')
assert model['passed'] and replay['result'] == 'PASS'
save('root/root-result-acceptance.json', {
    'schema': 'd-spdk-contract-root-acceptance-v1', 'stable_utc': now,
    'disposition': 'source_chain_and_clockless_semantics_accepted_performance_model_not_admitted',
    'global_direction': 'D main/I lightweight; A5 and other assets preserved; Oct15-28 unchanged',
    'source_ref': {'spdk': '19524ad45373e35d5fdc06f4e081444d87ceed80',
                   'dpdk': mapping['spdk']['dpdk_commit'],
                   'linux': 'v6.12 reference implementation, not deployment'},
    'accepted_facts': [
        'Completion callback resubmits same task/buffer until count draining.',
        'Host buffer/IOVA differs from namespace LBA.',
        'Allocator/software IOVA/DPDK VFIO/PRP implementation chain verified; actual leaf is not fixed by IO/PRP/CPU hugepage.',
        'Implemented event schema and reader acquired, no measured event trace.',
        'SC/SCT determine error, not phase; io_completed includes errors.',
        'perf elapsed sampled before final drain.',
        'Executable source-semantic feedback recomputes submission order.',
    ],
    'evidence_levels': {
        'S': {'admission': 'accepted', 'scope': 'source semantics and synthetic clockless prototype'},
        'M': {'admission': 'NOT_ADMITTED for timing/performance; conditional requirements specified',
              'missing': ['frozen service provider and admissible parameter domain',
                          'direction/resources/action/costed observation and full endpoint',
                          'cross-quiet persistence, public O0, strong controls and holdout specification'],
              'real_joint_trace_required_for_any_conditional_model': False,
              'original_native_P_PN_required_for_model': False},
        'N': {'admission': 'NOT_ADMITTED', 'actual_trace_files': 0, 'joint_trace_files': 0,
              'actual_deployment': None, 'actual_iommu_leaf': None,
              'actual_shared_iotlb_ptw': None, 'native_effect': None},
    },
    'prototype': {
        'contract': 'root/semantic-prototype-contract.json',
        'contract_sha256': sha(ROOT / 'root/semantic-prototype-contract.json'),
        'code': 'model/', 'verification': 'model/verification.json',
        'root_replay': 'root/semantic-replay-check.json',
        'status': 'PASS_FOR_SOURCE_CALLBACK_SEMANTICS_ONLY', 'N': 4, 'Q': 2,
        'N_Q_role': 'verification constants; not application calibration',
        'success_callback_orders': model['source_interface_callback_orders'],
        'negative_mutations_rejected': len(model['negative_mutations_rejected']),
        'error_cases': len(model['error_results']), 'drain_counted': True,
        'generated_events': 'all synthetic interface-permitted overapproximation',
        'clock_service_iova_P_PN': None, 'protocol_native_hardware_completion_proven': False,
    },
    'ordinary_baselines': [
        'Stock persistent pool and callback replenishment.',
        'Legal large mappings and preparation with full costs.',
        'Stock QD/qpair/poll options; changed pool footprint disclosed.',
        'Fixed-pool admission/pacing as an explicit charged ordinary software extension.',
        'Fixed P/PN, all-public-O0 ordinary rules and paid trials including perturbation/drain/cleanup.',
        'Same paid new information supplied to ordinary policy.',
    ],
    'not_accepted': ['natural opposite-sign state pair', 'cross-quiet predictive hidden state',
                     'information value beyond complete O0', 'sensor/algorithm novelty',
                     'PN net/native benefit', 'old COPY as direction-correct free NVMe adapter'],
    'preservation': {
        'old_M0_gate': 'not passed unchanged; no expansion/gap/guard changes',
        'I': 'lightweight not_ready; no auto elevation',
        'A5': 'latest source/DAG results and native evidence gaps preserved',
        'MIG_goal_1792023314': 'paused; no FPGA/board/build',
        'old_research_method': 'intentionally deleted; not restored; ARS-Codex used',
        'Linux_RTL_UAPI_changes': False,
    },
    'next_decision': 'Freeze one source-to-service research projection: resources/action/boundary persistence/O0/Z/controls/full endpoint/falsifiers. Admit a bounded conditional model if internally consistent; native trace or premeasured sign is not a universal prerequisite.',
    'novelty': 'Reuse closest-work comparison without new reading coverage claims; specific information boundary, timely costed observation and complete decision value remain conditional. Semantic prototype is infrastructure only.',
    'model_policy': {
        'Codex_only': True,
        'requested_workers': [
            {'task': 'mapping', 'model': 'gpt-6.1-sol', 'reasoning': 'high'},
            {'task': 'events', 'model': 'gpt-6.1-sol', 'reasoning': 'high'},
            {'task': 'projection', 'model': 'gpt-6.1-sol', 'reasoning': 'xhigh'},
            {'task': 'semantic implementation', 'model': 'gpt-6.1-sol', 'reasoning': 'medium'},
        ],
        'actual_runtime_metadata': 'unknown; requests are not runtime attestation',
        'root_switch_claim': False, 'new_workers_ultra_requested': False, 'DeepSeek_Claude_calls': 0,
    },
    'acceptance_basis': 'Root critical source/code review, concrete worker runs, root read-only replay; model agreement is not proof.',
})
save('root/execution-ledger.json', {
    'schema': 'd-spdk-execution-ledger-v1', 'utc': now,
    'network': 'Purposeful official-source retrievals occurred; per-seat receipts retained. Not zero-network.',
    'mapping_receipt': 'mapping/sources.json', 'event_receipt': 'events/sources.json',
    'root_review_receipt': 'root/critical-source-check.json',
    'worker_generator_verification_runs': 1, 'worker_consumer_cli_sample_runs': 1,
    'root_read_only_consumer_batch_runs': 1, 'new_clockless_prototype': 1,
    'old_experiment_replays': 0, 'SPDK_native_DMA_runs': 0, 'new_RTL_simulations': 0,
    'Linux_RTL_UAPI_modifications': 0, 'builds_FPGA_board_actions': 0, 'collectors': 0,
    'goal': 'bounded SPDK-contract goal created; MIG and old goals not resumed',
    'performance_results': 0, 'actual_worker_runtime_model': 'unknown',
    'receipt_correction': 'Two range-EOF assertions corrected; no research model or failed experiment retried.',
})
print(json.dumps({'accepted': 'source+clockless semantics only',
                  'source_registrations_checked': len(registered),
                  'orders': model['source_interface_callback_orders'],
                  'performance_model': 'NOT_ADMITTED'}))
