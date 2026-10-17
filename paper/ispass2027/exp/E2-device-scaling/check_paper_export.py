#!/usr/bin/env python3
"""Finite converter checks. All constructed campaigns are explicit fixtures."""
import argparse
import ast
import copy
import csv
import hashlib
import io
import json
from pathlib import Path
import re
import subprocess
import sys
import export_paper as analysis


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--fixture', type=Path, required=True)
    parser.add_argument('--schema-reference', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--software-profile',choices=tuple(analysis.profiles.PROFILES),default='p0-multi-v1')
    args = parser.parse_args()
    args.output = args.output.resolve()
    args.output.mkdir(parents=True, exist_ok=False)
    build = args.output/'build'
    build.mkdir()
    checks = []
    text = json.loads(args.fixture.read_text())['stdout'].replace('\r', '')
    sequence = int(re.search(r'^PRE_SEQ=(\d+)$', text, re.M)[1])+1
    boot = re.search(r'^BOOT=(.+)$', text, re.M)[1]
    observations = '\n'.join(re.findall(r'idma_(?:batch|path)_obs seq='+str(sequence)+r'\b[^\n]+',
        text.split('\nCASE_EXIT=0\n', 1)[1].split('\nTAINT_AFTER=', 1)[0]))
    match = re.search(r'\{\s*"schema"\s*:\s*"idma\.batch\.v1"', text)
    old, _ = json.JSONDecoder().raw_decode(text[match.start():])
    cli = json.loads((analysis.HERE/'multi-cli-cases.json').read_text())
    templates = {(x['output']['input']['nr_devices'], x['output']['input']['pages'][1]): x['output'] for x in cli if x['name']=='success'}

    def save(path, value):
        path.write_text(analysis.encoded(value))

    def reject(name, call):
        try:
            call()
        except (ValueError, AssertionError, KeyError):
            checks.append({'name': name, 'passed': True})
        else:
            raise AssertionError(name+' unexpectedly accepted')

    for mode in ('E2', 'E6'):
        protocol = analysis.protocol(mode)
        run = build/(mode+'-synthetic')
        (run/'uart').mkdir(parents=True)
        kernel = analysis.profiles.PROFILES[args.software_profile]['kernel_release'] if mode=='E2' else '6.5.0-rc2-ispass2027-pathv3'
        loaded = {'fixture_only': True, 'status': 'boot_identity_verified', 'kernel_release': kernel,
                  'boot_id': boot, 'bit_sha256': protocol['DS0_BIT_SHA256']}
        save(run/'load.json', loaded)
        source = {'fixture_only': True, 'actual_commits': {'linux':'45bf76b15e36cf608dff95013d91f8bedb7b1b8e',
                  'cva6':'8aac3fcd345af7686a55af1867783fd6b91a0069'}}
        if mode=='E2':source['actual_commits']=dict(analysis.profiles.BASELINE_COMMITS)
        save(run/'source.json', source)
        software = {'fixture_only': True, 'status':'passed', 'kernel_release':kernel,
                    'selected_linux':protocol.get('LINUX_COMMIT'),
                    'handwritten_inputs':{'bench/idma_scaling.c':protocol.get('CLI_SHA256')},
                    'artifacts':{'bin/idma_scaling':'explicit-fixture'}}
        if mode=='E2':software.update(analysis.profiles.PROFILES[args.software_profile])
        save(run/'software.json', software)
        records = []
        for index, (block, condition) in enumerate(protocol['plan']()):
            seq = index+1
            obs = observations.replace(f'obs seq={sequence} ', f'obs seq={seq} ')
            if mode == 'E2':
                row = copy.deepcopy(templates[protocol['CONDITION_GEOMETRY'][condition]])
                row['result']['observer_sequence'] = seq
            else:
                row = copy.deepcopy(old)
                row['input'].update(device='/dev/idma1', aggressor_id=None if condition=='Solo' else 0,
                                    aggressor_pages=8 if condition=='A8' else 1)
                row['result'].update(victim_did=11, aggressor_did=None if condition=='Solo' else 10)
                obs = obs.replace('v_did=10 a_did=11', 'v_did=11 a_did='+('4294967295' if condition=='Solo' else '10'))
                if condition == 'Solo':
                    row['result']['aggressor'] = None
                    lines = []
                    cycles = int(re.search(r'cycles=(\d+)', obs)[1])
                    for line in obs.splitlines():
                        if 'part=axi_v' in line:
                            for key, value in [('neither',0),('v_only',cycles),('a_only',0),('both',0)]:
                                line = re.sub(r'\b'+key+r'=\d+', key+'='+str(value), line)
                        if 'part=axi_a' in line or 'role=a ' in line:
                            for key, value in re.findall(r'(\w+)=(0x[0-9a-fA-F]+|\d+)', line):
                                if key not in ('seq','stage','valid'):
                                    line = re.sub(r'\b'+key+r'=(0x[0-9a-fA-F]+|\d+)', key+'='+str(8192 if key in ('shared_ar','shared_aw') else 0), line)
                        lines.append(line)
                    obs = '\n'.join(lines)
            row['fixture_only'] = True
            stdout = f'KERNEL={kernel}\nBOOT={boot}\nTAINT=0\nPRE_SEQ={seq-1}\n'+json.dumps(row)+'\nCASE_EXIT=0\n'+obs+'\nTAINT_AFTER=0\n'
            raw = run/'uart'/f'command-{seq:03}.json'
            save(raw, {'fixture_only':True, 'command':protocol['command'](condition), 'exit_code':0, 'stdout':stdout})
            item = protocol['parse'](stdout, kernel, condition)
            item.update(case=seq, block=block, condition=condition, ds='0/0', raw_command_path=str(raw.resolve()), fixture_only=True)
            records.append(item)
        manifest = {'schema':'ispass2027.'+('e2_multi' if mode=='E2' else 'e6_role_swap')+'.run.v1',
            'fixture_only':True, 'status':'passed', 'ds':'0/0', 'planned_cases':protocol['plan'](),
            'recorded_cases':len(records), 'load_manifest':str((run/'load.json').resolve()),
            'load_manifest_sha256':hashlib.sha256((run/'load.json').read_bytes()).hexdigest(),
            'source_record':str((run/'source.json').resolve()), 'selected_source':source,
            'software_manifest':str((run/'software.json').resolve()), 'selected_software':software,
            'kernel_release':kernel, 'boot_id':boot, 'bit_sha256':loaded['bit_sha256'], 'script_sha256':protocol['source']['sha256']}
        if mode=='E2':
            manifest.update(analysis.profiles.bind_identity(software,kernel,source,loaded),
                source_record_sha256=hashlib.sha256((run/'source.json').read_bytes()).hexdigest(),
                software_manifest_sha256=hashlib.sha256((run/'software.json').read_bytes()).hexdigest())
        save(run/'manifest.json', manifest); save(run/'records.json', records); save(run/'review.json', protocol['review'](records))
        initial = {path: path.read_bytes() for path in run.rglob('*.json')}
        result = analysis.read_collection(run, mode, _fixture_mode=True)
        assert result['fixture_only'] and result['source_board_cases'] == 0
        assert len(result['groups']) == (9 if mode=='E2' else 3) and all(x['n']==10 for x in result['groups'])
        rows = analysis.long_rows(result, build/(mode+'-summary.json'), 'fixture-only')
        assert all(list(x) == analysis.FIELDS and x['evidence_level']=='synthetic_protocol_fixture' for x in rows)
        table = io.StringIO(newline='')
        writer = csv.DictWriter(table, analysis.FIELDS, lineterminator='\n')
        writer.writeheader(); writer.writerows(rows)
        assert next(csv.reader(io.StringIO(table.getvalue()))) == analysis.FIELDS
        if mode == 'E2':
            assert all(x['statistics']['dma2_path_r0_mean_cycles']['n']==0 for x in result['groups'])
            assert result['groups'][0]['statistics']['dma3_iotlb_miss']['n']==0
            assert result['groups'][2]['statistics']['dma3_iotlb_miss']['n']==10
        else:
            assert result['paired_A8_over_A1']['n']==10
            paired_records = copy.deepcopy(records)
            times = {'Solo':[5]*10, 'A1':[1,2,3,100,101]*2, 'A8':[2,4,150,101,102]*2}
            for item in paired_records:
                item['raw_sha256'] = hashlib.sha256(Path(item['raw_command_path']).read_bytes()).hexdigest()
                item['metrics']['hardware_ms'] = times[item['condition']][item['block']]
            paired = analysis.summarize(paired_records, manifest, mode)
            assert paired['paired_A8_over_A1']['statistics']['median']==2
            medians = {x['condition']:x['statistics']['hardware_ms']['median'] for x in paired['groups']}
            assert medians['A8']/medians['A1']==101/3
            checks.append({'name':'E6_same_block_paired_median2_distinct_from_cell_medians101_over3', 'passed':True})
            next(x for x in paired_records if x['condition']=='A8')['boot_id']='foreign-boot'
            reject('E6_cross_boot_pair_forbidden', lambda:analysis.summarize(paired_records, manifest, mode))
            assert next(x for x in result['groups'] if x['condition']=='Solo')['statistics']['aggressor_iotlb_miss']['n']==0
        checks.append({'name':mode+'_full_raw_bound_fixture_and_exact_long_schema', 'passed':True, 'rows':len(rows)})
        reject(mode+'_public_loader_rejects_fixture', lambda:analysis.read_collection(run, mode))
        entry = analysis.HERE/'export_paper.py' if mode=='E2' else analysis.HERE.parent/'E6-mitigation/export_paper.py'
        output = entry.parent/'paper-export-host/build'/('must-not-publish-'+mode)
        cmd = [sys.executable, str(entry), '--run',str(run.resolve()), '--output',str(output), '--sample-set','fixture']
        completed = subprocess.run(cmd,capture_output=True,text=True)
        assert completed.returncode and 'fixture input cannot publish' in completed.stderr and not output.exists()
        checks.append({'name':mode+'_CLI_has_no_fixture_publication_bypass', 'passed':True})
        raw = Path(records[0]['raw_command_path']); value=json.loads(raw.read_text());value['exit_code']=1;save(raw,value)
        reject(mode+'_failed_original_command', lambda:analysis.read_collection(run,mode,_fixture_mode=True));raw.write_bytes(initial[raw])
        changed = copy.deepcopy(records);changed[0]['sequence']+=1;save(run/'records.json',changed)
        reject(mode+'_record_raw_mismatch', lambda:analysis.read_collection(run,mode,_fixture_mode=True));(run/'records.json').write_bytes(initial[run/'records.json'])
        changed = copy.deepcopy(records);changed[0]['metrics']={'hardware_ms':-123};save(run/'records.json',changed)
        fresh = analysis.read_collection(run,mode,_fixture_mode=True)
        assert fresh['groups'][0]['samples'][0]['metrics']['hardware_ms']>0
        checks.append({'name':mode+'_cached_metrics_recomputed_from_original_fields', 'passed':True})
        (run/'records.json').write_bytes(initial[run/'records.json'])
        changed = copy.deepcopy(manifest);changed['status']='running';save(run/'manifest.json',changed)
        reject(mode+'_incomplete_collection', lambda:analysis.read_collection(run,mode,_fixture_mode=True));(run/'manifest.json').write_bytes(initial[run/'manifest.json'])
        if mode=='E2':
            for key in ('software_profile','software_profile_helper_sha256','runtime_commits','source_record_sha256','software_manifest_sha256'):
                changed=copy.deepcopy(manifest);changed[key]='changed';save(run/'manifest.json',changed)
                reject('E2_'+key+'_rejected',lambda:analysis.read_collection(run,mode,_fixture_mode=True))
                (run/'manifest.json').write_bytes(initial[run/'manifest.json'])
        assert all(path.read_bytes()==data for path,data in initial.items())
        checks.append({'name':mode+'_read_only_input_preservation', 'passed':True})
    assert analysis.describe([1,2,10,20]) == {'n':4,'min':1,'median':6,'max':20,'q1':1.75,'q3':12.5,'iqr':10.75}
    assert analysis.describe([None])['n']==0 and analysis.describe([7])['iqr']==0
    checks.append({'name':'Type7_even_singleton_and_NA', 'passed':True})
    source = ast.parse(args.schema_reference.read_text())
    fields = next(ast.literal_eval(x.value) for x in source.body if isinstance(x, ast.Assign) and any(isinstance(t, ast.Name) and t.id=='FIELDS' for t in x.targets))
    assert fields == analysis.FIELDS and len(fields)==17
    assert 'uart' not in sys.modules and 'serial' not in sys.modules
    checks.append({'name':'exact_E1b17_columns_and_no_UART_import', 'passed':True})
    (args.output/'validation.json').write_text(analysis.encoded({'schema':'ispass2027.e2_e6.paper_export_host.v1',
        'scope':'Explicit synthetic campaigns only; no board data CSV published', 'passed':True, 'checks':checks,
        'converter_sha256':hashlib.sha256(Path(analysis.__file__).read_bytes()).hexdigest(),
        'schema_reference':str(args.schema_reference.resolve()),
        'schema_reference_sha256':hashlib.sha256(args.schema_reference.read_bytes()).hexdigest()}))
    print(json.dumps({'passed':True,'checks':len(checks)}))


if __name__ == '__main__':
    main()
