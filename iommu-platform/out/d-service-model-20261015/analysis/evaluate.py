#!/usr/bin/env python3
"""Exact finite information-value diagnostic; no model execution or fitted policy."""
import argparse
from collections import defaultdict
from fractions import Fraction
import hashlib
import json
from pathlib import Path
import sys
import unittest

ACTIONS = tuple((m, w) for m in ('P_M', 'PN_M') for w in range(4))
PAID_ACTIONS = tuple(a for a in ACTIONS if a[1] >= 2)
SUPPORT = {(k, r) for k in range(7) for r in (0, 2, 4, 10, 12)}
FORBIDDEN = {'h', 'h0', 'end_h', 'prehistory', 'target', 'targets', 'total_J',
             'additional_time', 'winner', 'future', 'target_outcomes', 'walk_phase',
             'internal_device', 'provider_h', 'debug_h', 'trace_file'}

def canonical(x):
    return json.dumps(x, sort_keys=True, separators=(',', ':'), allow_nan=False)

def clean_observation(x):
    """Fail closed on explicitly forbidden observation keys, at any nesting depth."""
    if isinstance(x, dict):
        for k, v in x.items():
            if k in FORBIDDEN or k.startswith(('debug_', 'future_', 'target_')):
                raise ValueError(f'forbidden observation key: {k}')
            clean_observation(v)
    elif isinstance(x, list):
        for v in x:
            clean_observation(v)

def o0_key(row):
    public, events = row['public_config'], row['reference']['O0_events']
    if not isinstance(public, dict) or not isinstance(events, list):
        raise ValueError('public_config must be object and O0_events full list')
    clean_observation(public)
    clean_observation(events)
    return canonical([public, events])

def z_key(row):
    z = {'counters': row['reference']['Z'], 'overflow': row['reference']['Z_overflow']}
    if not isinstance(z, (list, dict)):
        raise ValueError('Z must contain bounded counters and explicit overflow flags')
    clean_observation(z)
    leaves = []
    def visit(v):
        if isinstance(v, dict):
            for a in v.values(): visit(a)
        elif isinstance(v, list):
            for a in v: visit(a)
        elif isinstance(v, bool): leaves.append(('flag', v))
        elif isinstance(v, int) and 0 <= v <= 65535: leaves.append(('counter', v))
        else: raise ValueError(f'invalid bounded Z field: {v!r}')
    visit(z)
    if sum(k == 'counter' for k, _ in leaves) != 3 or sum(k == 'flag' for k, _ in leaves) != 3:
        raise ValueError('Z requires exactly three 16-bit counters and three boolean overflow flags')
    return canonical(z)

def costs(row):
    result = {}
    for t in row['targets']:
        a = t['mode'], t['wait']
        if isinstance(a[1], bool) or not isinstance(a[1], int): raise ValueError('wait must be integer')
        if a in result: raise ValueError(f'duplicate action {a}')
        value = t['total_J']
        if isinstance(value, bool) or not isinstance(value, (int, str)):
            raise ValueError('total_J must be exact int or rational string')
        result[a] = Fraction(value)
        if result[a] < 0: raise ValueError('negative cost')
    if set(result) != set(ACTIONS): raise ValueError('expected exactly eight registered mode/wait actions')
    return result

def groups(rows, key):
    result = defaultdict(list)
    for r in rows: result[key(r)].append(r)
    return result

def mean(values):
    values = list(values)
    return sum(values, Fraction()) / len(values)

def action_name(a): return f'{a[0]}:wait{a[1]}'

def best(rows, menu):
    values = {a: mean(r['_costs'][a] for r in rows) for a in menu}
    low = min(values.values())
    tied = [a for a in menu if values[a] == low]
    return low, tied, values

def bound(rows, key, menu):
    return sum((len(rs) * best(rs, menu)[0] for rs in groups(rows, key).values()), Fraction()) / len(rows)

def value(rows, key, menu=ACTIONS):
    base = bound(rows, key, menu)
    extra = bound(rows, lambda r: (key(r), r['_Z']), menu)
    if extra > base: raise AssertionError('negative pure information gain')
    return base, extra, base-extra

def argmin(r):
    low = min(r['_costs'].values())
    return {a for a in ACTIONS if r['_costs'][a] == low}

def sign(x): return (x > 0) - (x < 0)

def negative_cartesian(rows):
    """Independent prior x future worlds, conditional on each original H.

    Every past tuple is crossed with EVERY future cost vector in this config.
    Within H and within (H,Z), each action therefore has the same future mean.
    We evaluate the exact multiplicities rather than materializing the product.
    """
    future = {a: mean(r['_costs'][a] for r in rows) for a in ACTIONS}
    low = min(future.values())
    evidence = []
    for h, prior in groups(rows, lambda r: r['_H']).items():
        hz = groups(prior, lambda r: r['_Z'])
        for subset in hz.values():
            for a in ACTIONS:
                repeated_mean = sum((len(subset) * r['_costs'][a] for r in rows), Fraction()) / (len(subset)*len(rows))
                assert repeated_mean == future[a]
        evidence.append({'H_sha256': digest(h), 'prior_count': len(prior), 'Z_groups': len(hz),
                         'cartesian_pairs': len(prior)*len(rows), 'conditional_costs_equal': True})
    return {'kind': 'mathematical independent Cartesian resampling, not a physical reset/timing run',
            'future_support': len(rows), 'prior_support': len(rows), 'total_pairs': len(rows)**2,
            'future_action_means': {action_name(a): v for a,v in future.items()},
            'O0_bound': low, 'O0_Z_bound': low, 'pure_information_gain': Fraction(), 'groups': evidence}

def simulated_independent_control(rows, future_rows):
    """Use actual independently regenerated target trajectories; exact Cartesian prior x future."""
    by_tuple = groups(future_rows, lambda x:(x['future_prehistory']['k'],x['future_prehistory']['r']))
    if set(by_tuple)!=SUPPORT or any(len(v)!=8 for v in by_tuple.values()):
        raise ValueError('independent refresh requires 35 future tuples x8 actions')
    vectors=[]
    for tup,fs in by_tuple.items():
        vector={}
        for f in fs:
            if Fraction(f['future_prehistory']['weight'])!=Fraction(1,35):
                raise ValueError('unexpected future weight')
            a=f['mode'],f['wait']
            if a in vector:raise ValueError('duplicate refresh action')
            vector[a]=Fraction(f['additional_time'])
        if set(vector)!=set(ACTIONS):raise ValueError('refresh action support mismatch')
        vectors.append(vector)
    future_mean={a:mean(v[a] for v in vectors) for a in ACTIONS}
    low=min(future_mean.values())
    evidence=[]
    for h,prior in groups(rows,lambda r:r['_H']).items():
        zgroups=groups(prior,lambda r:r['_Z'])
        for zs in zgroups.values():
            for a in ACTIONS:
                assert sum((len(zs)*v[a] for v in vectors),Fraction())/(len(zs)*len(vectors))==future_mean[a]
        evidence.append({'H_sha256':digest(h),'prior_count':len(prior),'Z_groups':len(zgroups),
                         'cartesian_pairs':len(prior)*len(vectors),'future_conditional_action_means_equal':True})
    def total_bound(key):
        return sum((len(rs)*(mean(Fraction(r['reference']['duration']) for r in rs)+low)
                    for rs in groups(rows,key).values()),Fraction())/len(rows)
    b=total_bound(lambda r:r['_H']);z=total_bound(lambda r:(r['_H'],r['_Z']))
    assert b==z
    return {'kind':'simulated independent-refresh target trajectories crossed with every old prehistory; not physical reset measurement',
            'prior_support':len(rows),'future_support':len(vectors),'total_cartesian_pairs':len(rows)*len(vectors),
            'independent_future_action_means':{action_name(a):v for a,v in future_mean.items()},
            'O0_bound':b,'O0_Z_bound':z,'pure_information_gain':b-z,'groups':evidence,
            'cost_accounting':'old reference.duration plus provided future additional_time (reset,maintenance,idle,boundary wait,mode change,target); no old future reused'}

def digest(s): return hashlib.sha256(s.encode()).hexdigest()

def evaluate_config(cid, rows):
    for r in rows:
        r['_H'], r['_Z'], r['_costs'] = o0_key(r), z_key(r), costs(r)
    base, enriched, gain = value(rows, lambda r:r['_H'])
    paid_o0, paid_z, paid_gain = value(rows, lambda r:r['_H'], PAID_ACTIONS)
    fixed_best, fixed_ties, fixed_values = best(rows, ACTIONS)
    hg = []
    for h, rs in groups(rows, lambda r:r['_H']).items():
        opt_common = set.intersection(*(argmin(r) for r in rs))
        hb, hz, hv = value(rs, lambda r:r['_H'])
        h_states = sorted({r['reference']['end_h'] for r in rs})
        h0_states = sorted({r['prehistory']['h0'] for r in rs})
        signs = defaultdict(int)
        for r in rs: signs[str(sign(r['_costs'][('PN_M',0)]-r['_costs'][('P_M',0)]))] += 1
        hg.append({'H_sha256': digest(h), 'count':len(rs), 'weight':Fraction(len(rs),len(rows)),
                   'prehistory_tuples':[[r['prehistory']['k'],r['prehistory']['r']] for r in rs],
                   'private_diagnostic_only_reference_end_h_values':h_states,
                   'private_diagnostic_only_h0_values':h0_states,
                   'H_identifies_reference_end_h':len(h_states)==1,
                   'H_identifies_h0':len(h0_states)==1,
                   'H_has_common_cost_optimum':bool(opt_common),
                   'common_optimal_actions':[action_name(a) for a in ACTIONS if a in opt_common],
                   'Z_group_count':len(groups(rs,lambda r:r['_Z'])),
                   'world_action_costs_diagnostic_only':[{'prehistory':[r['prehistory']['k'],r['prehistory']['r']],
                    'costs':{action_name(a):v for a,v in r['_costs'].items()},
                    'optimal_actions':[action_name(a) for a in ACTIONS if a in argmin(r)]} for r in rs],
                   'P0_PN0_delta_PN_minus_P_sign_counts':dict(signs),
                   'simultaneous_action_means':{action_name(a):v for a,v in best(rs,ACTIONS)[2].items()},
                   'O0_best_expected':hb,'O0_Z_best_expected':hz,'pure_information_gain':hv,
                   'Z_groups':[{'Z':json.loads(z),'count':len(zs),'best_expected':best(zs,ACTIONS)[0],
                                'optimal_actions':[action_name(a) for a in best(zs,ACTIONS)[1]]}
                               for z,zs in groups(rs,lambda r:r['_Z']).items()]})
    initial_key = lambda r:(r['_H'],canonical(r['prehistory']))
    pib,piz,pig=value(rows,initial_key)
    assert pig==0, 'known initial tuple must determine this deterministic world'
    return {'config_id':cid,'split':rows[0]['split'],'public_config':rows[0]['public_config'],
            'worlds':len(rows),'weighting':'uniform over 35 (k,r) tuples; duplicate terminal states retain multiplicity',
            'fixed_action_means':{action_name(a):v for a,v in fixed_values.items()},
            'fixed_P0':fixed_values[('P_M',0)],'fixed_PN0':fixed_values[('PN_M',0)],
            'best_fixed_registered':fixed_best,'best_fixed_actions':[action_name(a) for a in fixed_ties],
            'O0_full_menu_bound':base,'O0_Z_full_menu_bound':enriched,'pure_information_gain':gain,
            'paid_Z_wait2_3_bound':paid_z,'O0_wait2_3_bound':paid_o0,'wait2_3_pure_information_gain':paid_gain,
            'paid_Z_minus_full_O0':paid_z-base,
            'fee_semantics':'wait2/3 trajectories already charge read+decision2; no fee subtraction; delays may cool provider',
            'H_group_count':len(hg),'all_H_identify_reference_end_h':all(g['H_identifies_reference_end_h'] for g in hg),
            'all_H_have_common_optimum':all(g['H_has_common_cost_optimum'] for g in hg),'H_groups':hg,
            'public_initial_negative':{'O0_known_tuple_bound':pib,'O0_known_tuple_Z_bound':piz,'pure_information_gain':pig},
            'independent_refresh_mathematical_negative':negative_cartesian(rows),
            'training':'not authorized; zero free full-menu gain implies stop without policy fitting' if gain==0 else 'positive bound only; root authorization required before fitting'}

def validate(rows):
    cfgs=groups(rows,lambda r:r['config_id'])
    if len(cfgs)!=8: raise ValueError(f'expected 8 configs, got {len(cfgs)}')
    splits=defaultdict(int)
    for cid,rs in cfgs.items():
        if len(rs)!=35 or {(r['prehistory']['k'],r['prehistory']['r']) for r in rs}!=SUPPORT:
            raise ValueError(f'{cid}: requires each of 35 distinct support tuples exactly once')
        if len({canonical(r['public_config']) for r in rs})!=1: raise ValueError(f'{cid}: inconsistent public_config')
        if len({r['split'] for r in rs})!=1: raise ValueError(f'{cid}: inconsistent split')
        splits[rs[0]['split']]+=1
    if dict(splits) != {'main':4,'holdout':4}: raise ValueError(f'expected 4 main and 4 holdouts: {dict(splits)}')
    if len({canonical(rs[0]['public_config']) for rs in cfgs.values()})!=8:
        raise ValueError('public configurations must be eight distinct objects')
    return cfgs

def json_ready(x):
    if isinstance(x,Fraction): return str(x)
    if isinstance(x,dict): return {k:json_ready(v) for k,v in x.items()}
    if isinstance(x,list): return [json_ready(v) for v in x]
    return x

class Checks(unittest.TestCase):
    def toy(self,distinct=False):
        return [{'_H':str(i) if distinct else 'same','_Z':str(i),
                 '_costs':{a:Fraction(0 if (a[0]=='P_M')==(i==0) else 10) for a in ACTIONS}} for i in range(2)]
    def test_opposite_optima_positive(self):
        self.assertEqual(value(self.toy(),lambda r:r['_H']),(5,0,5))
    def test_full_history_zero(self):
        self.assertEqual(value(self.toy(True),lambda r:r['_H']),(0,0,0))
    def test_independent_cartesian_zero(self):
        self.assertEqual(negative_cartesian(self.toy())['pure_information_gain'],0)
    def test_simulated_independent_refresh_exact_product(self):
        prior=self.toy()
        for i,r in enumerate(prior):r['reference']={'duration':10+i}
        future=[]
        for k,r in sorted(SUPPORT):
            for a in ACTIONS:
                future.append({'future_prehistory':{'k':k,'r':r,'weight':'1/35'},
                               'mode':a[0],'wait':a[1],'additional_time':k+r+(a[0]=='PN_M')})
        result=simulated_independent_control(prior,future)
        self.assertEqual(result['total_cartesian_pairs'],70)
        self.assertEqual(result['pure_information_gain'],0)
        self.assertEqual(result['O0_bound'],Fraction(191,10))
    def test_future_outcome_key_forbidden(self):
        for k in ('future_winner','target_outcomes','total_J','h0'):
            with self.assertRaises(ValueError): clean_observation([{'t':4,k:7}])
    def test_timestamps_preserved(self):
        a={'public_config':{},'reference':{'O0_events':[{'t':1,'event':'submit'}]}}
        b={'public_config':{},'reference':{'O0_events':[{'t':2,'event':'submit'}]}}
        self.assertNotEqual(o0_key(a),o0_key(b))
    def test_duplicate_weight_retained(self):
        rows=self.toy()+[self.toy()[0]]
        self.assertEqual(value(rows,lambda r:r['_H']),(Fraction(10,3),0,Fraction(10,3)))
    def test_nonnegative_value_with_ties(self):
        rows=self.toy()
        for r in rows:r['_costs']={a:Fraction(7) for a in ACTIONS}
        self.assertEqual(value(rows,lambda r:r['_H']),(7,7,0))

def main():
    p=argparse.ArgumentParser();p.add_argument('worlds',nargs='?');p.add_argument('--output',type=Path);p.add_argument('--self-test',action='store_true');p.add_argument('--negative-controls',type=Path);args=p.parse_args()
    if args.self_test:
        result=unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(Checks))
        if not result.wasSuccessful():return 1
    if args.worlds:
        raw=Path(args.worlds).read_bytes(); data=json.loads(raw);rows=data if isinstance(data,list) else data['worlds']
        cfgs=validate(rows)
        report={'schema':'exact-finite-information-diagnostic-v1','input_sha256':hashlib.sha256(raw).hexdigest(),
                'scope':'offline finite uniform synthetic model information bounds; no deployed policy, natural distribution, timing calibration, or novelty claim',
                'O0_key':'exact (public_config, full reference.O0_events), preserving every event field and timestamp',
                'Z_key':'all three bounded16 counters and explicit overflow flags; prior reference only',
                'tie_order':[action_name(a) for a in ACTIONS],
                'configs':[evaluate_config(cid,rs) for cid,rs in sorted(cfgs.items())]}
        if args.negative_controls:
            negative_raw=args.negative_controls.read_bytes(); negative=json.loads(negative_raw)
            refresh=groups(negative['independent_refresh'],lambda r:r['config_id'])
            if set(refresh)!=set(cfgs):raise ValueError('refresh config support mismatch')
            report['negative_controls_sha256']=hashlib.sha256(negative_raw).hexdigest()
            for c in report['configs']:
                c['independent_refresh_simulated_negative']=simulated_independent_control(cfgs[c['config_id']],refresh[c['config_id']])
        out=json.dumps(json_ready(report),indent=2,ensure_ascii=False)+'\n'
        if args.output:args.output.write_text(out)
        else:print(out)
    return 0
if __name__=='__main__':sys.exit(main())
