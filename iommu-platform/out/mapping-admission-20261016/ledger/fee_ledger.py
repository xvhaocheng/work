#!/usr/bin/env python3
"""Finite conditional fee bookkeeping. CPU illustration units are not cycles."""
import argparse
import copy
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
POLICIES = ('O0', 'O1', 'O2', 'O3', 'offline_lower_bound')


def key(event):
    return (event['region'], event['generation'], 'B', 'READ')


def validate(document):
    p = document['parameters']
    assert p['d'] > 0 and p['F'] > 0
    assert min(p['x'], p['s']) > p['d']
    assert p['R'] == min(p['x'], p['s']) - p['d']
    assert document['parameter_unit'] == 'illustrative_CPU_units_not_cycles'
    active, retired, packets = {}, set(), set()
    for e in document['events']:
        assert e['type'] in ('forward', 'pass', 'region_retire')
        k = key(e)
        assert k not in retired, 'retired generation reused'
        if e['region'] in active:
            assert active[e['region']] == e['generation']
        active[e['region']] = e['generation']
        if e['type'] != 'region_retire':
            assert e['packet'] not in packets
            packets.add(e['packet'])
            assert e['previous_object_returned'] is True
        else:
            assert e['dma_and_refs_drained'] is True
            assert e['pool_cache_can_return'] is True
            assert e['necessary_revocation_complete'] is True
            retired.add(k)
            del active[e['region']]
    return p


def run(document, policy):
    p = validate(document)
    d, x, s, F, R = (p[n] for n in ('d', 'x', 's', 'F', 'R'))
    # Only this explicitly offline reference reads final reuse counts.
    future_counts = {}
    if policy == 'offline_lower_bound':
        for e in document['events']:
            if e['type'] == 'forward':
                future_counts[key(e)] = future_counts.get(key(e), 0) + 1
    history, mapped, retired = {}, set(), set()
    rows, total, queries, copies, short_maps, admissions, releases = [], 0, 0, 0, 0, 0, 0
    for i, e in enumerate(document['events']):
        k = key(e)
        before = {'a': history.get(k, 0), 'mapped': k in mapped}
        common, excess, action, prepaid_release = 0, 0, '', False
        if e['type'] == 'pass':
            action = 'local_udp_copy_consume_return'
            # Local UDP work is equal but not assigned a fabricated numerical cost.
        elif e['type'] == 'region_retire':
            prepaid_release = k in mapped
            action = 'retire_release_prepaid_mapping' if prepaid_release else 'retire_no_B_mapping'
            releases += int(prepaid_release)
            mapped.discard(k)
            history.pop(k, None)
            retired.add(k)
        else:
            common = d
            if policy == 'O3':
                queries += 1
            if k in mapped:
                action = 'direct_retained_B_mapping'
            elif policy == 'O0':
                action, excess = 'short_B_map_TX_unmap', s - d
            elif policy == 'O1':
                action, excess = 'copy_to_persistent_B_pool', x - d
            else:
                if policy == 'O2':
                    admit = True
                elif policy == 'offline_lower_bound':
                    admit = F <= future_counts[k] * R
                else:
                    # Online: current frozen parameters and accumulated history only.
                    admit = R > 0 and history.get(k, 0) + R >= F
                if admit:
                    action, excess = 'establish_retained_B_mapping_then_direct', F
                    mapped.add(k)
                    admissions += 1
                else:
                    action = 'copy_to_persistent_B_pool' if x <= s else 'short_B_map_TX_unmap'
                    excess = min(x, s) - d
                    history[k] = history.get(k, 0) + R
            copies += int(action == 'copy_to_persistent_B_pool')
            short_maps += int(action == 'short_B_map_TX_unmap')
        total += common + excess
        rows.append({'index': i, 'event_id': e['id'], 'key': list(k), 'type': e['type'],
                     'action': action, 'state_before': before,
                     'state_after': {'a': history.get(k, 0), 'mapped': k in mapped},
                     'common_CPU_units': common, 'excess_CPU_units': excess,
                     'CPU_subtotal_units': common + excess,
                     'final_B_revocation_CPU_prepaid_in_F': prepaid_release,
                     'A_unmap_triggered': None if e['type'] == 'region_retire' else False,
                     'A_retire_scope': 'outside_this_B_CPU_ledger/required_by_source_contract'
                     if e['type'] == 'region_retire' else None,
                     'generation_changed_by_event': False})
    pool_use = int(copies > 0)
    # Symbols are unmeasured costs, never zero-valued defaults.
    symbols = {'K_target_pool_lifecycle': pool_use, 'h_per_forward_query': queries,
               f'G_source_layout_retention_integration_{policy}': 1,
               f'L_residual_association_IOVA_PTE_sync_memory_management_callbacks_CPU_{policy}': 1,
               'U_local_UDP_common': sum(e['type'] == 'pass' for e in document['events'])}
    return {'CPU_unit': document['parameter_unit'], 'normalized_CPU_subtotal': total,
            'forward_common_CPU_units': sum(r['common_CPU_units'] for r in rows),
            'excess_CPU_units': sum(r['excess_CPU_units'] for r in rows),
            'symbolic_unmeasured_cost_coefficients': symbols,
            'counts': {'forward': sum(e['type'] == 'forward' for e in document['events']),
                       'local_udp': sum(e['type'] == 'pass' for e in document['events']),
                       'copies_to_B_pool': copies, 'short_B_maps': short_maps,
                       'persistent_admissions': admissions, 'persistent_releases': releases,
                       'forward_queries': queries, 'target_pool_lifecycle_charges': pool_use},
            'persistent_F_charged_once_per_admission': admissions * F,
            'retire_additional_normalized_CPU_units': 0,
            'retire_charge_explanation': 'Final B revoke CPU already included in F; other lifecycle costs remain symbolic.',
            'state_at_end': {'mapped': [list(k) for k in sorted(mapped)],
                             'history': [{'key': list(k), 'a': a} for k, a in sorted(history.items())]},
            'events': rows}


def make_event(kind, region, generation, number):
    e = {'id': f'{kind}-{region}-{generation}-{number}', 'type': kind,
         'region': region, 'generation': generation}
    if kind == 'region_retire':
        e.update(dma_and_refs_drained=True, pool_cache_can_return=True,
                 necessary_revocation_complete=True)
    else:
        e.update(packet=f'{region}-{generation}-packet-{number}', previous_object_returned=True)
    return e


def fixture(document, events):
    test = copy.deepcopy(document)
    test['events'] = events
    return test


def checks(document, results):
    p = document['parameters']
    prefix = [make_event('forward', 'p', 0, n) for n in range(2)]
    short = fixture(document, prefix + [make_event('region_retire', 'p', 0, 3)])
    long = fixture(document, prefix + [make_event('forward', 'p', 0, n) for n in range(2, 8)]
                   + [make_event('region_retire', 'p', 0, 9)])
    assert run(short, 'O3')['events'][:2] == run(long, 'O3')['events'][:2]
    reset_events = [make_event('forward', 'same_PFN', 0, n) for n in range(3)]
    reset_events += [make_event('region_retire', 'same_PFN', 0, 3)]
    reset_events += [make_event('forward', 'same_PFN', 1, n) for n in range(3)]
    reset_events += [make_event('region_retire', 'same_PFN', 1, 3)]
    reset = run(fixture(document, reset_events), 'O3')['events']
    assert reset[4]['state_before'] == {'a': 0, 'mapped': False}
    assert reset[4]['action'] == reset[0]['action']
    assert reset[6]['action'] == 'establish_retained_B_mapping_then_direct'
    pass_events = [make_event('forward', 'q', 0, 0), make_event('pass', 'q', 0, 1)]
    pass_events += [make_event('forward', 'q', 0, n) for n in (2, 3)]
    pass_events += [make_event('pass', 'q', 0, 4), make_event('region_retire', 'q', 0, 5)]
    passed = run(fixture(document, pass_events), 'O3')['events']
    for i in (1, 4):
        assert passed[i]['state_before'] == passed[i]['state_after']
        assert not passed[i]['A_unmap_triggered'] and not passed[i]['generation_changed_by_event']
    assert passed[4]['state_after']['mapped']
    for policy, result in results.items():
        assert result['persistent_F_charged_once_per_admission'] == result['counts']['persistent_admissions'] * p['F']
        assert result['counts']['persistent_admissions'] == result['counts']['persistent_releases']
        assert result['counts']['target_pool_lifecycle_charges'] == int(result['counts']['copies_to_B_pool'] > 0)
        symbols = result['symbolic_unmeasured_cost_coefficients']
        assert all(isinstance(c, int) and c >= 0 for c in symbols.values())
        assert symbols['K_target_pool_lifecycle'] == int(result['counts']['copies_to_B_pool'] > 0)
        assert symbols['h_per_forward_query'] == (10 if policy == 'O3' else 0)
        assert not result['state_at_end']['mapped'] and not result['state_at_end']['history']
        assert result['counts']['forward'] == 10 and result['counts']['local_udp'] == 2
        assert all(r['A_unmap_triggered'] is False for r in result['events'] if r['type'] == 'pass')
    expected = {'O0': 40, 'O1': 30, 'O2': 29, 'O3': 27, 'offline_lower_bound': 25}
    assert {k: v['normalized_CPU_subtotal'] for k, v in results.items()} == expected
    assert 2 * p['R'] < p['F'] <= 3 * p['R']
    return {'scope': 'finite bookkeeping semantic and arithmetic checks only',
            'common_prefix_independent_of_future_suffix': 'pass',
            'same_PFN_new_generation_resets_mapping_and_history': 'pass',
            'PASS_preserves_generation_history_and_mapping_without_A_unmap': 'pass',
            'F_and_K_counted_once_with_unmeasured_symbols_preserved': 'pass',
            'frozen_2R_less_F_at_most_3R_and_arithmetic': 'pass',
            'all_policies_same_forward_UDP_completion_and_final_drain': 'pass',
            'not_verified': ['driver correctness', 'IOMMU protocol', 'ownership protocol',
                             'native traffic feasibility', 'performance', 'measured costs']}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--events', type=Path, default=HERE / 'events.json')
    parser.add_argument('--output', type=Path, default=HERE / 'result.json')
    args = parser.parse_args()
    raw = args.events.read_bytes()
    document = json.loads(raw)
    results = {p: run(document, p) for p in POLICIES}
    result = {'scope': 'finite conditional event and fee ledger; not performance modeling or measurements',
              'events_sha256': hashlib.sha256(raw).hexdigest(),
              'parameters': document['parameters'], 'assumptions': document['assumptions'],
              'fixed_contract': document['fixed_contract'], 'results': results,
              'checks': checks(document, results),
              'candidate_example_is_O3_alias': True,
              'candidate_scope': 'User proposed a general organization idea, without a distinct controller. This particular cumulative-excess version is ordinary O3 by definition; no separate candidate implementation or equivalence test and no whole-family coverage claim.',
              'online_information': 'current event, history, frozen common F/R parameters; no future reuse count',
              'offline_bound_scope': 'knows final per-key r; min(rR,F) bounds only normalized excess, not full cost',
              'symbol_domains': {'K': 'nonnegative, unmeasured target pool full lifecycle CPU cost',
                                 'h': 'nonnegative, unmeasured per-B-forward lookup CPU cost',
                                 'G,L,U': 'nonnegative residual CPU costs not already included in d/x/s/F/K/h; signed differences unconstrained; F map/revoke work must not be charged again in L',
                                 'memory_bandwidth_completion': 'separate unmeasured quantities, not summed into CPU units'},
              'unmeasured_resources_separate_from_CPU': {
                  'source_and_target_memory_bytes': 'unknown per policy',
                  'source_and_target_memory_retention_time': 'unknown per policy',
                  'copy_and_translation_bandwidth': 'unknown per policy',
                  'completion_constraints': 'unknown; no timing guarantee'},
              'conditional_comparisons': {
                  'valid_parameter_range': 'R>0 and 2R<F<=3R; normalized formula only',
                  'O3': '10d + F + 4R + K + 10h + G_O3 + L_O3 + 2U',
                  'O2': '10d + 3F + G_O2 + L_O2 + 2U',
                  'O1': '10d + 10R + K + G_O1 + L_O1 + 2U (x=min(x,s) for frozen case)',
                  'O3_beats_O2_only_if': 'K+10h+(G_O3-G_O2)+(L_O3-L_O2) < 2F-4R = 2 illustrative CPU units',
                  'O3_beats_O1_only_if': '10h+(G_O3-G_O1)+(L_O3-L_O1)+(K_O3-K_O1) < 6R-F = 3 illustrative CPU units',
                  'K_cancellation_condition': 'O1 and O3 actually use the identical target pool lifecycle; otherwise retain K_O3-K_O1',
                  'source_cost_warning': 'Do not cancel three-RQ or aligned-PMD source layout costs against native O0 without evidence.'},
              'model_record': {'requested_model': 'gpt-6.1-sol', 'requested_reasoning': 'medium',
                               'actual_model': 'unknown', 'actual_reasoning': 'unknown'},
              'execution_record': {'command': 'python3 fee_ledger.py', 'execution': 'finite script executed by Codex',
                                   'production_source_edits': 0, 'builds': 0, 'Linux_or_device_runs': 0,
                                   'network_requests': 0, 'performance_models': 0}}
    args.output.write_text(json.dumps(result, indent=2, ensure_ascii=False) + '\n')
    print(json.dumps({'output': str(args.output), 'normalized_CPU_subtotals':
                      {k: v['normalized_CPU_subtotal'] for k, v in results.items()},
                      'semantic_arithmetic_checks': 'pass', 'unit': document['parameter_unit']}))


if __name__ == '__main__':
    main()
