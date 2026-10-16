#!/usr/bin/env python3
"""Synthetic timer-bound checks only; no board, serial, ioctl or prior replay."""
import copy
import importlib.util
import json
import sys
from pathlib import Path

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('finite_timer_bounds', ROOT/'host/calibrate_finite_timer.py')
t = importlib.util.module_from_spec(spec)
spec.loader.exec_module(t)


def fixture():
    base = 1_000_000_000_000
    # Deliberately asymmetric call windows and sampling offsets, and non-2:1 rates.
    windows = [(base, base+300_000_000),
               (base+6_000_000_000, base+6_500_000_000),
               (base+12_300_000_000, base+12_500_000_000)]
    events = [base+250_000_000, base+6_050_000_000, base+12_420_000_000]
    samples = []
    for (begin, end), event in zip(windows, events):
        payload = dict(schema=t.CAPS_SCHEMA, kind='caps', device='/dev/idma0',
                       ioctl={'attempted': True, 'return': 0, 'errno': 0},
                       timing_semantics={'physical_rate_uncalibrated': True,
                                         'independent_dell_reference': False,
                                         'physical_calibration_claim': False}, userspace_nominal_ns=1,
                       caps=dict(abi_version=1, struct_size=104, magic=0x49444d4146494e01,
                                 observer_magic=0x9999, composition_magic=0x49444d4143504e01,
                                 composition_abi=1, composition_config=0xA00, composition_idle=1,
                                 batch_magic=0x8888, device_id=0, did=10, quarantined=0, online_cpus=1,
                                 sample=dict(cpu_cycles=9_000_000_000+(event-events[0])*37_000_000//1_000_000_000,
                                             time_ticks=7_000_000_000+(event-events[0])*23_000_000//1_000_000_000,
                                             nominal_ns=event)))
        samples.append(dict(dell_begin_ns=begin, dell_end_ns=end, caps_JSON=payload))
    return dict(schema=t.SCHEMA, samples=samples)


def sample(document, index):
    return document['samples'][index]


def counter(document, index):
    return sample(document, index)['caps_JSON']['caps']['sample']


def main():
    original = fixture()
    result = t.analyze(original)
    checks = []
    for name, actual in [('cpu_cycles', 37_000_000), ('time_ticks', 23_000_000)]:
        bounds = result['common_constant_rate_compatible_bounds'][name]
        assert t.unpack(bounds['lower_exact']) <= actual <= t.unpack(bounds['upper_exact'])
        for row in result['all_pair_evidence']:
            p = row['constant_tick_frequency_bounds'][name]
            assert t.unpack(p['lower_exact']) <= actual <= t.unpack(p['upper_exact'])
        elapsed = t.stage_elapsed_bounds(actual//100, bounds)
        assert t.unpack(elapsed['lower_exact']) <= t.Fraction(1, 100) <= t.unpack(elapsed['upper_exact'])
    assert result['conditional_SI_frequency_bounds'] is None
    checks.append(dict(name='asymmetric_unknown_event_positions_and_non_2_to_1_rates', accepted=True,
                       complete_input=original, result=result,
                       synthetic_truth_only={'cpu_hz': 37_000_000, 'time_hz': 23_000_000}))
    supplied_ref = copy.deepcopy(original);supplied_ref['dell_reference_rate_error_ppm']=50
    ref_result = t.analyze(supplied_ref)
    for name in t.COUNTERS:
        normal = result['common_constant_rate_compatible_bounds'][name]
        si = ref_result['conditional_SI_frequency_bounds'][name]
        assert t.unpack(si['lower_exact']) == t.unpack(normal['lower_exact'])*t.Fraction(999950, 1_000_000)
        assert t.unpack(si['upper_exact']) == t.unpack(normal['upper_exact'])*t.Fraction(1000050, 1_000_000)
    checks.append(dict(name='explicit_synthetic_Dell_reference_ppm_propagated_outward', accepted=True,
                       complete_input=supplied_ref, result=ref_result))
    cases = [
        ('missing_Dell_endpoint', lambda x: sample(x, 1).pop('dell_end_ns'), 'missing dell_end_ns'),
        ('missing_TIME_counter', lambda x: counter(x, 1).pop('time_ticks'), 'missing raw counter fields'),
        ('CPU_counter_rollback', lambda x: counter(x, 1).update(cpu_cycles=counter(x, 0)['cpu_cycles']-1), 'rollback or nonpositive'),
        ('TIME_nonpositive_delta', lambda x: counter(x, 2).update(time_ticks=counter(x, 1)['time_ticks']), 'rollback or nonpositive'),
        ('overlapping_event_intervals', lambda x: sample(x, 1).update(dell_begin_ns=sample(x, 0)['dell_end_ns']), 'intervals overlap'),
        ('insufficient_two_5s_lower_intervals', lambda x: sample(x, 1).update(dell_end_ns=sample(x, 2)['dell_begin_ns']-4_000_000_000), 'two adjacent wall intervals'),
        ('inconsistent_CPU_rate_intervals', lambda x: counter(x, 2).update(cpu_cycles=counter(x, 1)['cpu_cycles']+100_000_000), 'do not share'),
        ('different_CAPS_identity', lambda x: sample(x, 2)['caps_JSON']['caps'].update(observer_magic=0x9998), 'different CAPS identity'),
        ('quarantined_CAPS', lambda x: sample(x, 1)['caps_JSON']['caps'].update(quarantined=1), 'quarantined CAPS'),
        ('nonzero_ioctl', lambda x: sample(x, 1)['caps_JSON']['ioctl'].update(errno=5, **{'return': -1}), 'actual CAPS ioctl'),
        ('fewer_than_three_samples', lambda x: x['samples'].pop(), 'at least three'),
        ('board_nominal_clock_claims_independent_Dell', lambda x: sample(x, 1)['caps_JSON']['timing_semantics'].update(independent_dell_reference=True), 'must not supply Dell calibration'),
        ('boolean_Dell_timestamp', lambda x: sample(x, 0).update(dell_begin_ns=True), 'non-unsigned integer'),
        ('unbounded_Dell_reference_error', lambda x: x.update(dell_reference_rate_error_ppm=1_000_000), 'non-unsigned integer'),
    ]
    for name, mutate, reason in cases:
        value = copy.deepcopy(original);mutate(value)
        try:
            t.analyze(value)
            raise AssertionError(f'{name}: malformed timer samples accepted')
        except t.TimerRejection as error:
            assert reason in str(error), (name, str(error))
            checks.append(dict(name=name, accepted=False, reason=str(error), complete_input=value))
    # Rational outward formatting is itself checked, independent of a frequency midpoint.
    assert t.outward_decimal(t.Fraction(1, 3), True) == '0.333333333'
    assert t.outward_decimal(t.Fraction(1, 3), False) == '0.333333334'
    output = {'scope': 'synthetic timer interval math; no actual CAPS/board/calibration acceptance',
              'actual_samples': [], 'hardware_validation': False, 'real_ioctl_calls': 0,
              'synthetic_positive': 2, 'synthetic_negative': len(cases), 'checks': checks,
              'rational_outward_rounding_verified': True}
    path = ROOT/'validation/timer-bounds-synthetic-tests.json'
    path.write_text(json.dumps(output, indent=2)+'\n')
    print(json.dumps({k: v for k, v in output.items() if k != 'checks'}, indent=2))


if __name__ == '__main__':
    main()
