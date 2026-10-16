"""Verify only the newly joined CPU/RTL delivery; never rerun either baseline."""
import hashlib
import json
from pathlib import Path

out = Path(__file__).resolve().parents[1]
platform = out.parents[1]
read = lambda p: json.loads(p.read_text())
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
cpu = read(out / 'cpu/run.json')
dma = read(out / 'dma/results/run.json')
cl = read(out / 'cpu/execution-ledger.json')
di = read(out / 'dma/results/compiled-inputs.json')
fixture = read(out / 'common/fixture.json')
checks = {}
checks['same_frozen_fixture'] = cpu['fixture_sha256'] == digest(out / 'common/fixture.json')
dma_hashes = {str((platform / x['path']).resolve()): x['sha256'] for x in di['files']}
for name in ('fixture.hpp', 'fixture_data.hpp'):
    p = out / 'common' / name
    checks['same_compiled_' + name] = (digest(p) == cl['manifest']['../common/' + name]['sha256'] == dma_hashes[str(p.resolve())])
for name in ('adapter_cpu.cpp', 'adapter_cpu'):
    checks['cpu_current_' + name] = digest(out / 'cpu' / name) == cl['manifest'][name]['sha256']
checks['dma_binary_matches_receipt'] = digest(platform / di['executable']['path']) == di['executable']['sha256']
checks['all_cpu_functional_checks'] = cpu['passed'] and len(cpu['functional_tests']) == 16 and all(x['pass'] for x in cpu['functional_tests'])
checks['same_complete_frames_and_records'] = cpu['frame_bytes'] == dma['useful_frame_bytes'] == fixture['frame_bytes'] == 1639 and cpu['record_bytes'] == dma['consumer_record_bytes'] == fixture['destination_record_bytes'] == 1675
checks['same_consumer_digest'] = cpu['frame_digest_fnv1a64'] == dma['consumer_digest']
checks['four_copies_each'] = all(cpu['ledger'][k] == 4 for k in ('submitted', 'accepted', 'completed')) and all(dma[k] == 4 for k in ('submitted', 'frontend_accepted', 'backend_accepted', 'completed'))
checks['dma_actual_traffic_conserved'] = dma['pass'] and dma['payload_aw'] == dma['payload_b'] == 6 and dma['payload_write_strobe_bytes'] == 1639 and dma['payload_read_bus_bytes'] == dma['payload_r_beats'] * 8 == 1672
t = cpu['timing_ns']
checks['cpu_timing_intervals_accounted'] = sum(x['elapsed'] for x in t['phases']) + t['clock_and_phase_bookkeeping_unattributed'] == t['total_observed_lifecycle'] and sum(x['elapsed'] for x in t['phases'] if x['observer']) == t['sum_validation_observer_phases'] and sum(x['elapsed'] for x in t['phases'] if not x['observer']) == t['sum_source_delivery_phases']
checks['independent_dma_images_and_trace_accepted'] = read(out / 'dma/results/artifact-verification.json')['pass']
report = {'scope': 'new cross-worker input, executable, full-byte and cost-accounting integration only; no benchmark replay', 'checks': checks, 'passed': all(checks.values()), 'paper_complete': False}
(out / 'root/integration-verification.json').write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n')
print(json.dumps(report, ensure_ascii=False, indent=2))
raise SystemExit(0 if report['passed'] else 1)
