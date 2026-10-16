#!/usr/bin/env python3
"""Bound raw CAPS counter rates by independent Dell event-time intervals.

No device/serial/network access. No nominal kernel ns or CPU:TIME ratio is used.
The common interval is conditional on a constant-rate counter model; it does not
establish instantaneous frequency stability during a subsequent DMA operation.
"""
import argparse
import base64
import hashlib
import json
from fractions import Fraction
from pathlib import Path

SCHEMA = 'r107-finite-timer-samples-v1'
RESULT_SCHEMA = 'r107-finite-timer-bounds-v1'
CAPS_SCHEMA = 'r107-ordinary-finite-pilot-receipt-v1'
MIN_LONG_GAP_NS = 5_000_000_000
COUNTERS = ('cpu_cycles', 'time_ticks')
IDENTITY = ('abi_version', 'struct_size', 'magic', 'observer_magic',
            'composition_magic', 'composition_abi', 'composition_config',
            'composition_idle', 'batch_magic', 'device_id', 'did',
            'quarantined', 'online_cpus')


class TimerRejection(ValueError):
    pass


def require(condition, message):
    if not condition:
        raise TimerRejection(message)


def integer(value, name, maximum=(1 << 64) - 1):
    require(type(value) is int and 0 <= value <= maximum, f'{name}: missing/non-unsigned integer')
    return value


def outward_decimal(value, lower, places=9):
    scale = 10**places
    scaled = value.numerator*scale
    rounded = scaled//value.denominator if lower else -(-scaled//value.denominator)
    return f'{rounded//scale}.{rounded%scale:0{places}d}'


def rational(value):
    return {'numerator': value.numerator, 'denominator': value.denominator}


def unpack(value):
    return Fraction(value['numerator'], value['denominator'])


def interval(low, high, unit):
    require(0 <= low <= high, 'invalid derived interval')
    answer = {'lower_exact': rational(low), 'upper_exact': rational(high),
              'lower_decimal_outward': outward_decimal(low, True),
              'upper_decimal_outward': outward_decimal(high, False), 'unit': unit}
    if low+high > 0:
        answer['relative_halfwidth_ppm_exact'] = rational((high-low)/(high+low)*1_000_000)
        answer['relative_halfwidth_ppm_upper_decimal'] = outward_decimal((high-low)/(high+low)*1_000_000, False)
        answer['relative_halfwidth_definition'] = '(upper-lower)/(upper+lower); interval width, not statistical confidence'
    return answer


def caps_record(sample, index):
    require(isinstance(sample, dict), f'sample{index}: object required')
    for name in ('dell_begin_ns', 'dell_end_ns', 'caps_JSON'):
        require(name in sample, f'sample{index}: missing {name}')
    begin = integer(sample['dell_begin_ns'], f'sample{index}.dell_begin_ns')
    end = integer(sample['dell_end_ns'], f'sample{index}.dell_end_ns')
    require(begin < end, f'sample{index}: nonpositive Dell call interval')
    payload = sample['caps_JSON']
    require(isinstance(payload, dict), f'sample{index}.caps_JSON: parsed JSON object required')
    for name in ('schema', 'kind', 'device', 'ioctl', 'caps', 'timing_semantics', 'userspace_nominal_ns'):
        require(name in payload, f'sample{index}.caps_JSON: missing {name}')
    require(payload['schema'] == CAPS_SCHEMA and payload['kind'] == 'caps', f'sample{index}: finite CAPS JSON identity')
    require(isinstance(payload['device'], str) and payload['device'], f'sample{index}: CAPS device identity')
    io = payload['ioctl']
    require(isinstance(io, dict) and io.get('attempted') is True and
            type(io.get('return')) is int and io['return'] == 0 and
            type(io.get('errno')) is int and io['errno'] == 0, f'sample{index}: nonzero/absent actual CAPS ioctl')
    integer(payload['userspace_nominal_ns'], f'sample{index}.userspace_nominal_ns')
    semantics = payload['timing_semantics']
    require(isinstance(semantics, dict) and semantics.get('physical_rate_uncalibrated') is True and
            semantics.get('independent_dell_reference') is False and
            semantics.get('physical_calibration_claim') is False,
            f'sample{index}: board nominal clock must not supply Dell calibration')
    caps = payload['caps']
    require(isinstance(caps, dict), f'sample{index}: CAPS fields required')
    require(all(k in caps for k in (*IDENTITY, 'sample')), f'sample{index}: missing CAPS identity/sample fields')
    ident = {k: integer(caps[k], f'sample{index}.caps.{k}') for k in IDENTITY}
    require(ident['abi_version'] == 1 and ident['struct_size'] == 104 and
            ident['magic'] == 0x49444d4146494e01, f'sample{index}: frozen finite ABI')
    require(ident['composition_magic'] == 0x49444d4143504e01 and
            ident['composition_abi'] == 1 and ident['observer_magic'] > 0 and
            ident['batch_magic'] > 0, f'sample{index}: hardware capability identity')
    require(ident['device_id'] == 0 and ident['did'] == 10 and
            ident['composition_config'] == 0xA00 and ident['composition_idle'] == 1,
            f'sample{index}: expected DMA0/DID10 quiet S0 identity')
    require(ident['quarantined'] == 0, f'sample{index}: quarantined CAPS')
    require(ident['online_cpus'] == 1, f'sample{index}: single-hart counter ownership required')
    raw = caps['sample']
    require(isinstance(raw, dict) and all(k in raw for k in ('cpu_cycles', 'time_ticks', 'nominal_ns')),
            f'sample{index}: missing raw counter fields')
    counters = {k: integer(raw[k], f'sample{index}.sample.{k}') for k in ('cpu_cycles', 'time_ticks', 'nominal_ns')}
    return begin, end, (payload['device'], ident), counters


def analyze(input_document):
    require(isinstance(input_document, dict) and input_document.get('schema') == SCHEMA, 'timer input schema')
    samples = input_document.get('samples')
    require(isinstance(samples, list) and len(samples) >= 3, 'at least three independent CAPS samples required')
    parsed = [caps_record(s, i) for i, s in enumerate(samples)]
    require(all(x[2] == parsed[0][2] for x in parsed), 'different CAPS identity across samples')
    qualifying = []
    for i in range(len(parsed)-1):
        require(parsed[i+1][0] > parsed[i][1], f'samples{i}/{i+1}: Dell event intervals overlap; no finite rate upper bound')
        if parsed[i+1][0]-parsed[i][1] >= MIN_LONG_GAP_NS:
            qualifying.append([i, i+1])
    require(len(qualifying) >= 2, 'two adjacent wall intervals need guaranteed minimum event separation >=5s')
    pairs = []
    common = {k: [Fraction(0), None] for k in COUNTERS}
    for i in range(len(parsed)):
        for j in range(i+1, len(parsed)):
            minimum = parsed[j][0]-parsed[i][1]
            maximum = parsed[j][1]-parsed[i][0]
            row = {'samples': [i, j], 'event_elapsed_ns_min': minimum,
                   'event_elapsed_ns_max': maximum, 'deltas': {},
                   'average_counter_advance_rate': {}, 'constant_tick_frequency_bounds': {}}
            for counter in COUNTERS:
                delta = parsed[j][3][counter]-parsed[i][3][counter]
                require(delta > 0, f'{counter} samples{i}/{j}: counter rollback or nonpositive delta')
                row['deltas'][counter] = delta
                # Exact observed integer advancement per unknown Dell-clock time.
                row['average_counter_advance_rate'][counter] = interval(
                    Fraction(delta*1_000_000_000, maximum),
                    Fraction(delta*1_000_000_000, minimum), 'ticks per Dell CLOCK_MONOTONIC second')
                # For a constant tick oscillator, endpoint phases add <=1 tick.
                low = Fraction(max(0, delta-1)*1_000_000_000, maximum)
                high = Fraction((delta+1)*1_000_000_000, minimum)
                row['constant_tick_frequency_bounds'][counter] = interval(low, high, 'Hz relative to Dell CLOCK_MONOTONIC second')
                common[counter][0] = max(common[counter][0], low)
                old_high = common[counter][1]
                common[counter][1] = high if old_high is None else min(old_high, high)
            pairs.append(row)
    aggregate = {}
    for counter, (low, high) in common.items():
        require(high is not None and 0 < low <= high, f'{counter}: rate intervals do not share a positive common interval')
        aggregate[counter] = interval(low, high, 'Hz relative to Dell CLOCK_MONOTONIC second')
    ppm = input_document.get('dell_reference_rate_error_ppm')
    si = None
    if ppm is not None:
        integer(ppm, 'dell_reference_rate_error_ppm', 999999)
        error = Fraction(ppm, 1_000_000)
        si = {k: interval(low*(1-error), high*(1+error), 'Hz relative to SI second; conditional on supplied Dell reference-rate error bound')
              for k, (low, high) in common.items()}
    return {'schema': RESULT_SCHEMA, 'input_intervals_accepted': True,
            'hardware_validation': False, 'sample_count': len(samples),
            'complete_input_document': input_document,
            'caps_identity': {'device': parsed[0][2][0], 'fields': parsed[0][2][1]},
            'guaranteed_at_least_5s_adjacent_pairs': qualifying,
            'all_pair_evidence': pairs, 'common_constant_rate_compatible_bounds': aggregate,
            'tick_endpoint_phase_allowance': 'one tick for each pair delta; no invented UART latency',
            'dell_reference_rate_error_ppm': ppm, 'conditional_SI_frequency_bounds': si,
            'SI_reference_accuracy_established_by_this_tool': False,
            'constant_rate_model_status': 'not falsified by input intervals; sampled averages do not prove instantaneous or future phase stability',
            'stage_conversion_condition': 'same continuous counters, same boot/hart, and stable oscillator rate between calibration and the reported raw stamp pair',
            'unsupported_claims': ['no exact point frequency', 'no use of CPU:TIME=2:1, DT nominal frequency, or kernel nominal ns as independent evidence',
                                   'no proof of new boot/BIT identity, no clock-gating/DVFS/drift proof',
                                   'Dell CLOCK_MONOTONIC is an external operational reference; supplied ppm is an assumption to be independently justified'],
            'source_time_location': 'each CSR_CYCLE and CSR_TIME read may occur separately anywhere inside its Dell begin/end interval'}


def stage_elapsed_bounds(delta_ticks, frequency_bounds):
    """Convert one raw stamp delta conditionally; do not read any device."""
    integer(delta_ticks, 'raw stage counter delta')
    low, high = unpack(frequency_bounds['lower_exact']), unpack(frequency_bounds['upper_exact'])
    require(0 < low <= high, 'positive frequency bounds required for bounded elapsed conversion')
    result = interval(Fraction(max(0, delta_ticks-1), 1)/high,
                      Fraction(delta_ticks+1, 1)/low, 'seconds in frequency bound reference')
    result['condition'] = 'only under the stable same-counter rate model; CSR_CYCLE/TIME sampling points need not be simultaneous'
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    raw = args.input.read_bytes()
    try:
        document = json.loads(raw)
        result = analyze(document)
        code = 0
    except (TimerRejection, KeyError, TypeError, ValueError) as error:
        result = {'schema': RESULT_SCHEMA, 'input_intervals_accepted': False,
                  'hardware_validation': False, 'reason': str(error)}
        code = 1
    result['original_input_path'] = str(args.input.resolve())
    result['original_input_sha256'] = hashlib.sha256(raw).hexdigest()
    result['original_input_text_utf8'] = raw.decode('utf-8', errors='replace')
    result['original_input_base64'] = base64.b64encode(raw).decode('ascii')
    args.output.write_text(json.dumps(result, indent=2, ensure_ascii=False)+'\n')
    print(json.dumps({k: v for k, v in result.items() if k not in ('complete_input_document', 'all_pair_evidence', 'original_input_text_utf8', 'original_input_base64')}, indent=2))
    return code


if __name__ == '__main__':
    raise SystemExit(main())
