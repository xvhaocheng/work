#!/usr/bin/env python3
"""Finite G analysis fixtures only; no board and no publication override CLI."""
import argparse
from copy import deepcopy
import csv
import hashlib
import json
import math
from pathlib import Path
import shutil
import stat
import subprocess
import sys
from unittest.mock import patch

sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(HERE.parent))
sys.path.insert(0,str(HERE.parent/'collector-validation'))
import analyze_g as analysis
import collect_g as collector
import g_records as records
from verify_g_collector import fixture


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def save(path,value):path.write_text(json.dumps(value,indent=2)+'\n')


def make_run(out,cli_report):
    out.mkdir();bound=out/'bound';bound.mkdir()
    template=HERE.parent/'collector-validation/revision01/explicit-fixture40'
    m=json.loads((template/'manifest.json').read_text())
    for name,s in m['sources'].items():
        p=bound/(name+'.json');p.write_bytes(Path(s['path']).read_bytes());s['path']=str(p.resolve());s['sha256']=digest(p)
    cases=json.loads(cli_report.read_text())['cases']
    source={c['record']['condition']:c['record'] for c in cases if c.get('mode')=='' and 'record' in c and c['record']['success'] and c['record']['run_attempted']}
    ratios=list(range(1,11));solo=[1,100,2,100,3,100,4,100,5,100];items=[]
    for point in collector.plan():
        b,c=point['block'],point['condition'];row=deepcopy(source[c]);r=row['result_buffer']
        v=solo[b]*100000*(1 if c=='solo' else ratios[b]*(1 if c=='a8-gap0' else 2 if c=='a8-gap1000' else 3))
        awhole=(200000000+b*1000)*(1 if c=='a8-gap0' else 2 if c=='a8-gap1000' else 4)
        w=[int(x,16) for x in r['words_hex']];w[10]=w[16]=v;w[28]=w[27]+v
        if c!='solo':
            w[13]=awhole;w[30]=w[29]+awhole
            w[17]=w[18]=b;w[21]=int(b>0);w[23]=1;w[24]=b+1
            w[25]=1 if b else 0;w[26]=b
        r['words_hex']=[f'0x{x:016x}' for x in w]
        row['derived']['victim_whole_mib_s']=16*50000000/v
        row['derived']['aggressor_whole_mib_s']=16*50000000/awhole if c!='solo' else None
        r['victim_elapsed_ns']=v*20+1000;r['whole_software_elapsed_ns']=max(v*20,awhole*20 if c!='solo' else 0)+2000
        text=fixture(row,point['case'])
        if c!='solo' and b>=8:
            lines=text.splitlines();full=2*b
            for j,line in enumerate(lines):
                if 'idma_path_hist ' in line and 'role=a ' in line:
                    lines[j]=line.split('counts=',1)[0]+'counts='+','.join(map(str,[0,0,full]+[0]*29))
                if 'idma_path_obs ' in line and 'role=a ' in line:
                    lines[j]=line.replace(f'sum={full} max=1',f'sum={2*full} max=2').replace(f'area={full}',f'area={2*full}')
            text='\n'.join(lines)+'\n'
        raw=dict(command=collector.command(point),stdout=text,exit_code=0,
            fixture_only=True,evidence_level='synthetic_protocol_fixture')
        path=out/f'raw-{point["case"]:02d}.json';save(path,raw)
        item=records.parse(text,c,{64:1,1024:1,4096:2})
        item.update(point,raw_command_path=str(path.resolve()),fixture_only=True,evidence_level='synthetic_protocol_fixture',ds='0/0');items.append(item)
    save(out/'manifest.json',m);save(out/'records.json',items)
    return out


def clone_run(source,target):
    shutil.copytree(source,target)
    m=json.loads((target/'manifest.json').read_text());items=json.loads((target/'records.json').read_text())
    for s in m['sources'].values():s['path']=str((target/'bound'/Path(s['path']).name).resolve())
    for item in items:item['raw_command_path']=str((target/Path(item['raw_command_path']).name).resolve())
    save(target/'manifest.json',m);save(target/'records.json',items)
    return target,m,items


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cli-report',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False);out=a.output.resolve();checks=[]
    save(out/'fixture-context.json',dict(fixture_only=True,evidence_level='synthetic_protocol_fixture',status='running',
        hardware_executed=False,scope='actual G analyzer/parser; finite synthetic logs and marked build/load fixtures, no board'))
    run=make_run(out/'source40',a.cli_report)
    before={p:digest(p) for p in run.rglob('*') if p.is_file()}
    result=analysis.analyze(run,out/'analysis40',_fixture_mode=True)
    assert result['source_board_cases']==0 and result['campaign_complete'] is False and result['fixture_only'] is True
    assert all(digest(p)==d for p,d in before.items())
    checks.append('complete40 raw reparse; all source bytes unchanged; synthetic evidence propagated')
    assert all(row['n']==10 and row['statistics']['V_whole_cycles']['n']==10 for row in result['rows'])
    g0=next(x for x in result['paired_comparisons'] if x['condition']=='a8-gap0' and x['reference']=='solo')
    assert g0['n']==10 and [x['value'] for x in g0['pairs']]==list(range(1,11))
    assert {k:g0['statistics'][k] for k in ['median','q1','q3','iqr']}==dict(median=5.5,q1=3.25,q3=7.75,iqr=4.5)
    numerator=next(x for x in result['rows'] if x['condition']=='a8-gap0')['statistics']['V_whole_cycles']['median']
    denominator=result['rows'][0]['statistics']['V_whole_cycles']['median']
    assert not math.isclose(g0['statistics']['median'],numerator/denominator)
    checks.append('same-block ratios 1..10: n10 median5.5 Q1=3.25 Q3=7.75 IQR4.5, distinct from ratio of medians')
    for condition,vr,ar in [('a8-gap1000',2,.5),('a8-gap2000',3,.25)]:
        for metric,want in [('V_time_vs_Ggap0_ratio',vr),('A_own_whole_throughput_vs_Ggap0_ratio',ar),('A_own_whole_time_vs_Ggap0_ratio',1/ar)]:
            x=next(x for x in result['paired_comparisons'] if x['condition']==condition and x['metric']==metric)
            assert x['statistics']['n']==10 and x['statistics']['median']==want and x['statistics']['iqr']==0
    checks.append('gap1000/2000 own V/time and full16MiB A throughput costs use same-block Ggap0')
    solo=result['rows'][0]
    assert solo['statistics']['A_whole_cycles']['n']==0 and solo['statistics']['A_whole_MiB_s']['median'] is None
    assert solo['statistics']['path_ar0_mean_cycles']['n']==0 and solo['statistics']['path_ar0_max_cycles']['median'] is None
    hist=solo['histograms']['ar0'];pooled=hist['auxiliary_pooled_full_transactions']
    assert hist['valid_latency_trial_n']==0 and hist['zero_full_trial_n']==10
    assert pooled['total_transactions']==0 and pooled['mean_cycles'] is None and pooled['quantile_intervals']['p95'] is None
    assert all(t['valid_latency_trial_n']==0 and t['quantile_intervals']['p50'] is None and t['full_transaction_weight'] is None for t in hist['trials'])
    g0row=result['rows'][1];hist=g0row['histograms']['ar0'];pooled=hist['auxiliary_pooled_full_transactions']
    assert pooled['total_transactions']==90 and hist['valid_latency_trial_n']==9 and hist['trial_full_range']==dict(min=0,max=18)
    assert [t['full'] for t in hist['trials']]==list(range(0,20,2))
    assert [t['full_transaction_weight'] for t in hist['trials']]==[x/90 for x in range(0,20,2)]
    assert hist['trials'][0]['quantile_intervals']['p95'] is None and hist['trials'][8]['quantile_intervals']['p95']['cycle_lo']==2
    assert pooled['quantile_intervals']['p95']['cycle_lo']==2 and math.isclose(pooled['mean_cycles'],124/90)
    checks.append('per-block hist full/32bins and original bucket ranges; 9 valid/1 zero trials, full0..18 and weights full/90; auxiliary pooled differs from trial ranges')
    assert g0row['statistics']['A_completed']['median']==4096 and g0row['statistics']['A_completed_bytes']['median']==16777216
    assert 'not own A whole' in g0row['histograms']['ar0']['interpretation']
    checks.append('Solo A/zero-full valid n0 and N/A; A-window90 full transactions never reinterpreted as own40960 COPYs')
    for sample in g0row['samples']:
        raw=result['validated_records'][sample['records_index']]['g_words'];metrics=sample['metrics']
        assert metrics['V_whole_MiB_s']==16*50000000/raw[10] and metrics['A_whole_MiB_s']==16*50000000/raw[13]
        assert metrics['V_window_A_issue']==raw[17] and metrics['V_window_A_done']==raw[18] and metrics['V_window_A_carry_done']==raw[21] and metrics['V_window_A_open_edge_issue']==raw[22]
        assert metrics['A_HPM_iotlb_miss']==26
    checks.append('own denominators, separate window issue/done/carry/open-edge and HPM V-bracket retained')
    with (out/'analysis40/long-table.csv').open() as stream:
        reader=csv.DictReader(stream);assert reader.fieldnames==analysis.TABLES.FIELDS and len(reader.fieldnames)==17
        rows=list(reader)
    assert len({x['row_id'] for x in rows})==len(rows)
    for row in rows:
        expected=analysis.TABLES.locate(result,row['source_locator'])
        assert row['value']==('' if expected is None else str(expected))
        assert row['experiment']=='synthetic_G' and row['evidence_level']=='synthetic_protocol_fixture' and row['sample_set'].startswith('synthetic_')
    assert any('non_atomic_HPM_software_V_bracket' in x['condition'] for x in rows)
    assert all(x['n']=='' for x in rows if x['statistic'].startswith('auxiliary_pooled'))
    zero_quantiles=[x for x in rows if x['condition'].startswith('solo;') and x['metric'].startswith('histogram_ar0_trial_p')]
    assert zero_quantiles and all(x['value']=='' and x['n']=='0' and x['value_status']=='not_applicable' for x in zero_quantiles)
    checks.append('CSV zero-full trial interval n0/N/A; auxiliary pooled interval never labeled n10 trial statistic')
    assert any('not_A_own_whole_distribution' in x['condition'] for x in rows)
    checks.append('all common17 CSV rows unique and source-locator exact; synthetic and observation windows explicit')
    for entry in result['input_snapshot']:
        p=Path(entry['snapshot_path']);assert digest(p)==entry['sha256'] and p.stat().st_mode&0o222==0
        assert p.read_bytes()==Path(entry['original_path']).read_bytes()
    assert (out/'analysis40/inputs').stat().st_mode&0o222==0 and (out/'analysis40').stat().st_mode&0o222==0
    for row in result['rows']:
        for sample in row['samples']:assert Path(sample['snapshot_raw_path']).is_file()
    checks.append('snapshot raw links navigate immutable copies; entire analysis bundle read-only')
    try:analysis.analyze(run,out/'analysis40',_fixture_mode=True)
    except ValueError:pass
    else:raise AssertionError('overwrote analysis')
    checks.append('new read-only byte-exact input snapshot and exclusive output; second run refused')
    def reject(label,change):
        r,m,items=clone_run(run,out/('bad-'+label));change(r,m,items)
        target=out/('reject-'+label)
        try:analysis.analyze(r,target,_fixture_mode=True)
        except (ValueError,KeyError,TypeError,IndexError) as e:checks.append('reject '+label+': '+str(e))
        else:raise AssertionError(label)
        assert not target.exists()
    def manifest_change(key,value):
        def change(r,m,items):m[key]=value;save(r/'manifest.json',m)
        return change
    for key,val in [('status','failed'),('status','running'),('recorded_cases',39),('boot_id','wrong'),('bit_sha256','wrong'),('script_sha256','wrong'),('parser_sha256','wrong')]:
        reject(key+'-'+str(val),manifest_change(key,val))
    reject('changed-helper',lambda r,m,it:(m['helpers'].__setitem__('collector-source/legacy_obs_g.py','wrong'),save(r/'manifest.json',m)))
    reject('missing-point',lambda r,m,it:(it.pop(),save(r/'records.json',it)))
    reject('wrong-order',lambda r,m,it:(it.__setitem__(slice(0,2),it[1::-1]),save(r/'records.json',it)))
    reject('duplicate-block',lambda r,m,it:(it[4].__setitem__('block',0),save(r/'records.json',it)))
    reject('cached-derived',lambda r,m,it:(it[0]['metrics'].__setitem__('V_whole_cycles',1),save(r/'records.json',it)))
    reject('cached-result-buffer',lambda r,m,it:(it[0]['record']['result_buffer']['victim'].__setitem__('completed',1),save(r/'records.json',it)))
    reject('duplicate-raw-path',lambda r,m,it:(it[1].__setitem__('raw_command_path',it[0]['raw_command_path']),save(r/'records.json',it)))
    reject('source-sha',lambda r,m,it:(m['sources']['software'].__setitem__('sha256','wrong'),save(r/'manifest.json',m)))
    reject('changed-source-bytes',lambda r,m,it:(r/'bound/software.json').write_bytes((r/'bound/software.json').read_bytes()+b'\n'))
    def changed_binding(name,key,value):
        def change(r,m,items):
            p=r/'bound'/(name+'.json');x=json.loads(p.read_text());x[key]=value;save(p,x);m['sources'][name]['sha256']=digest(p);save(r/'manifest.json',m)
        return change
    reject('OpenSBI',changed_binding('software','selected_opensbi','wrong'))
    reject('fixture-backend',changed_binding('backend','fixture_only',True))
    reject('loaded-BIT',changed_binding('load','bit_sha256','wrong'))
    reject('FPGA-failed',changed_binding('fpga','status','failed'))
    def raw_change(r,m,it):
        p=Path(it[0]['raw_command_path']);x=json.loads(p.read_text());x['stdout']=x['stdout'].replace('part=time valid=1','part=time valid=0');save(p,x)
    reject('raw-final-invalid',raw_change)
    def sequence_change(r,m,it):
        p=Path(it[1]['raw_command_path']);x=json.loads(p.read_text());x['stdout']=x['stdout'].replace('PRE_SEQ=1','PRE_SEQ=2').replace('seq=2','seq=3').replace('"observer_sequence": 2','"observer_sequence": 3');save(p,x)
        parsed=records.parse(x['stdout'],it[1]['condition'],{64:1,1024:1,4096:2});it[1].update(parsed);save(r/'records.json',it)
    reject('fresh-but-gapped-sequence',sequence_change)
    # Mutate independent tool copies only. Accepted source and Image stay untouched.
    toolroot=out/'tool-copies';toolroot.mkdir()
    for rel in [*analysis.FROZEN,'analysis-source/table_schema.as-reused.py','analysis-source/g_statistics.py']:
        q=toolroot/rel;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes((analysis.HERE/rel).read_bytes())
    for rel in [*analysis.FROZEN,'analysis-source/table_schema.as-reused.py','analysis-source/g_statistics.py']:
        q=toolroot/rel;original=q.read_bytes();q.write_bytes(original+b'\n')
        with patch.object(analysis,'HERE',toolroot):
            try:analysis.read_collection(run,True)
            except ValueError:checks.append('reject changed actual source copy '+rel)
            else:raise AssertionError(rel)
        q.write_bytes(original)
    commands=[]
    target=out/'publication-must-not-exist'
    for extra in [[],['--fixture-mode']]:
        cmd=[sys.executable,str(HERE.parent/'analyze_g.py'),'--run',str(run),'--output',str(target),*extra]
        r=subprocess.run(cmd,text=True,capture_output=True,timeout=30);assert r.returncode!=0 and not target.exists()
        commands.append(dict(argv=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr))
    checks.append('actual public CLI rejects fixture before any output; no fixture bypass flag')
    report=dict(status='passed',fixture_only=True,evidence_level='synthetic_protocol_fixture',source_board_cases=0,
        check_count=len(checks),checks=checks,commands=commands,csv_rows=len(rows),
        scope='actual analyzer with finite synthetic complete40 and rejection cases; no hardware',
        sources={str(p):digest(p) for p in [HERE.parent/'analyze_g.py',Path(__file__)]})
    save(out/'report.json',report);save(out/'fixture-context.json',report)
    print(json.dumps({'status':'passed','checks':len(checks),'CSV_rows':len(rows),'report':str(out/'report.json')}))


if __name__=='__main__':main()
