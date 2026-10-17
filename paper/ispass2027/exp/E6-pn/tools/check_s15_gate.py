#!/usr/bin/env python3
"""Finite synthetic protocol tests for the offline S15 gate; zero new board cases."""
import argparse
import copy
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile
from unittest.mock import patch

sys.dont_write_bytecode=True
import s15_gate as gate


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--source-root',type=Path,required=True)
    a=p.parse_args();a.output=a.output.resolve();require=gate.require
    require(a.output.is_relative_to(gate.EXPERIMENT) and not a.output.exists(),'new E6-local test receipt directory required')
    a.output.mkdir(parents=True);checks=[];public=[]
    mapping=json.loads((gate.EXPERIMENT/'tool-source-map.json').read_text());ref=mapping['S15_reference_records']
    reader=gate.Inputs(a.source_root,a.output,False);_,historical=reader.read(ref['path']);_,parse,parser_identity=gate.parser(reader,mapping)
    templates={c:next(r for r in historical if r['condition']==c) for c in gate.GOLDEN}
    expected_cva6='b'*40;expected_iommu='c'*40;bit_sha='d'*64;boot='12345678-1234-1234-1234-123456789abc'
    software=reader.resolve(next(x['path'] for x in mapping['existing_evidence'] if x['path'].endswith('/manifests/software-build.json')))
    def save(path,value):path.write_text(gate.encoded(value))
    with tempfile.TemporaryDirectory(prefix='s15-host-fixtures-',dir=gate.HERE) as temp:
        build=Path(temp)
        def make(name,*,delta_cycles=0,spill=False):
            run=build/name;(run/'uart').mkdir(parents=True)
            (run/'entry.py').write_text('# Explicit synthetic saved-entry source; not a board collector.\n')
            source={'fixture_only':True,'selected_cva6':expected_cva6,'iommu':expected_iommu,'linux':gate.LINUX,'reset_mode':0,'v_gap_reset_cycles':0,'pn_cap':111,'gap_cap':222}
            save(run/'source.json',source)
            fpga={'fixture_only':True,'status':'passed','action':'fpga-ds0','variant':'ds0','selected_cva6':expected_cva6,'selected_iommu':expected_iommu,'source_manifest':str(run/'source.json'),'source_manifest_sha256':gate.sha((run/'source.json').read_bytes()),'artifacts':{'actual-new-PN.bit':bit_sha}}
            save(run/'fpga.json',fpga)
            loaded={'fixture_only':True,'status':'boot_identity_verified','bit_sha256':bit_sha,'kernel_release':gate.KERNEL,'boot_id':boot,'program_file_sha256_verified':True}
            save(run/'load.json',loaded);records=[]
            for index,(block,condition) in enumerate(gate.plan()):
                base=copy.deepcopy(templates[condition]);row=base['record'];row['fixture_only']=True;row['input']['aggressor_pages']=1 if condition=='Solo' else int(condition[1:])
                observations=base['observer'];t=observations['time'];t['cycles']=int(gate.GOLDEN[condition]*50000)+delta_cycles;t['h']=t['cycles']-t['gap']
                v=observations['axi_v'];v.update(neither=0,a_only=0,v_only=t['cycles'] if condition=='Solo' else 0,both=0 if condition=='Solo' else t['cycles'])
                if spill and index==1:
                    observations['axi_a']['ar']+=1;observations['axi_a']['end']+=1<<16
                    base['path']['ar0']['entered']+=1;base['path']['ar0']['end']+=1;base['path']['ar0']['area']+=1
                def fields(values):return ' '.join(k+'='+str(v) for k,v in values.items())
                obs='\n'.join(f'idma_batch_obs seq={index+1} part={part} '+fields(values) for part,values in observations.items())
                paths='\n'.join(f'idma_path_obs seq={index+1} role={key[0]} channel={key[1]} stage={key[2]} '+fields(values) for key,values in base['path'].items())
                stdout=f'KERNEL={gate.KERNEL}\nBOOT={boot}\nTAINT=0\nPRE_SEQ={index}\n'+json.dumps(row)+'\nCASE_EXIT=0\n'+obs+'\n'+paths+'\nTAINT_AFTER=0\n'
                path=run/'uart'/f'command-{index+1:03}.json';save(path,{'fixture_only':True,'command':gate.command(condition),'exit_code':0,'stdout':stdout,'raw_path':str(path)})
                item=parse(stdout);item.update(case=index+1,block=block,condition=condition,ds='0/0',raw_command_path=str(path),fixture_only=True);records.append(item)
            for label,seq in [('pre',0),('post',15)]:
                item={'schema':'idma.pn_control.v1','fixture_only':True,'success':True,'errno':0,'operation':'read','read_only':True,'mode':0,'v_gap_cycles':0,'cap':111,'gap_cap':222}
                stdout=f'KERNEL={gate.KERNEL}\nBOOT={boot}\nPRE_SEQ={seq}\n'+json.dumps(item)+'\n'
                save(run/'uart'/f'command-{label}.json',{'fixture_only':True,'command':gate.READ_COMMAND,'exit_code':0,'stdout':stdout})
            manifest={'fixture_only':True,'phase':'e6-pn-s15','status':'passed','recorded_cases':15,'planned_cases':gate.plan(),'control_actions':[],
                'kernel_release':gate.KERNEL,'ds':'0/0','script_source':str(run/'entry.py'),'script_sha256':gate.sha((run/'entry.py').read_bytes()),'source_manifest':str(run/'source.json'),'source_manifest_sha256':gate.sha((run/'source.json').read_bytes()),
                'fpga_manifest':str(run/'fpga.json'),'fpga_manifest_sha256':gate.sha((run/'fpga.json').read_bytes()),'bit_sha256':bit_sha,'load_manifest':str(run/'load.json'),'load_manifest_sha256':gate.sha((run/'load.json').read_bytes()),
                'software_manifest':str(software),'pre_readback':str(run/'uart/command-pre.json'),'post_readback':str(run/'uart/command-post.json')}
            save(run/'manifest.json',manifest);save(run/'records.json',records);return run,records,manifest
        def analyze(run,**kw):return gate.analyze(run,a.source_root,run/'fpga.json',run/'source.json',expected_cva6,expected_iommu,_fixture_mode=True,**kw)[0]
        def reject(name,call):
            try:call()
            except (ValueError,AssertionError,KeyError,IndexError,json.JSONDecodeError) as error:checks.append({'name':name,'passed':True,'actual_error':type(error).__name__+': '+str(error)})
            else:raise AssertionError(name+' was accepted')
        run,records,manifest=make('complete15');before={p:p.read_bytes() for p in run.rglob('*.json')}
        result=gate.export(run,a.source_root,build/'synthetic-gate',run/'fpga.json',run/'source.json',expected_cva6,expected_iommu,_fixture_mode=True)
        assert result['numerical_gate_passed'] and not result['eligible_for_DS50'] and result['source_board_cases']==0 and result['fixture_only']
        assert all(c['n']==5 and c['passed'] for c in result['cells']) and len(result['members'])==15
        assert result['parser']['assertions_removed']==1 and result['parser']['assertions_added']==2
        assert all(p.read_bytes()==raw for p,raw in before.items())
        checks.append({'name':'raw15_fixed30_reference_three_n5_medians_original_input_bytes_and_synthetic_non_admission','passed':True})
        run,records,manifest=make('closing-spill',spill=True);result=analyze(run)
        raw=json.loads(Path(records[1]['raw_command_path']).read_text())
        rejected_old=False
        try:gate.parser(reader,mapping)[0](raw['stdout'],gate.KERNEL,True)
        except AssertionError:rejected_old=True
        assert rejected_old and result['numerical_gate_passed']
        assert records[1]['observer']['axi_a']['shared_ar']-records[1]['observer']['axi_v']['ar']-records[1]['observer']['axi_a']['ar']==-1
        checks.append({'name':'self_consistent_closing_spill_private_minus1_accepted_only_by_exact_PATH0_adaptation','passed':True})
        run,records,manifest=make('over-threshold',delta_cycles=30000);result=gate.export(run,a.source_root,build/'failed-numerical-gate',run/'fpga.json',run/'source.json',expected_cva6,expected_iommu,_fixture_mode=True)
        assert not result['numerical_gate_passed'] and not result['eligible_for_DS50'] and any(not c['passed'] for c in result['cells'])
        checks.append({'name':'numerical_threshold_failure_never_qualifies_or_changes_source_status','passed':True})
        # Exact Decimal boundary, independently of integer-clock median rounding.
        for ref in gate.GOLDEN.values():
            assert abs(ref*gate.Decimal('1.005')-ref)<=ref*gate.Decimal('.005')
            assert not abs(ref*gate.Decimal('1.0050001')-ref)<=ref*gate.Decimal('.005')
        checks.append({'name':'inclusive_half_percent_Decimal_boundary','passed':True})
        mutations=('n14','status','phase','plan','control_missing','mode_write','resume','kernel','collector_hash','collector_source_missing','source_reset','source_pin','fpga_status','fpga_ds50','fpga_hash','load','software','mode_pre','gap_post','cap_pre','gap_cap_post','watermark_pre','watermark_post','read_command','read_alias','extra_attempt','duplicate_raw','reorder','cross_boot','sequence_gap','command','ABI','V_pages','A_pages','data_error','A_bytes','cached_record','shared_tamper','PATH_tamper','missing_PATH')
        for kind in mutations:
            run,records,manifest=make('bad-'+kind)
            if kind=='n14':records.pop();save(run/'records.json',records)
            elif kind=='status':manifest['status']='failed'
            elif kind=='phase':manifest['phase']='ds0-middle'
            elif kind=='plan':manifest['planned_cases'].reverse()
            elif kind=='control_missing':manifest.pop('control_actions')
            elif kind=='mode_write':manifest['control_actions']=[{'operation':'set-mode','mode':0}]
            elif kind=='resume':manifest['recovered_cases']=15
            elif kind=='kernel':manifest['kernel_release']='wrong'
            elif kind=='collector_hash':manifest['script_sha256']='a'*64
            elif kind=='collector_source_missing':manifest.pop('script_source')
            elif kind.startswith('source_'):
                value=json.loads((run/'source.json').read_text());value['reset_mode' if kind=='source_reset' else 'selected_cva6']=1 if kind=='source_reset' else 'a'*40;save(run/'source.json',value)
            elif kind.startswith('fpga_'):
                if kind=='fpga_hash':manifest['fpga_manifest_sha256']='wrong'
                else:value=json.loads((run/'fpga.json').read_text());value['status' if kind=='fpga_status' else 'variant']='failed' if kind=='fpga_status' else 'ds50';save(run/'fpga.json',value)
            elif kind=='load':value=json.loads((run/'load.json').read_text());value['bit_sha256']='wrong';save(run/'load.json',value)
            elif kind=='software':
                value=json.loads(software.read_text());value['selected_linux']='wrong';save(run/'foreign-software.json',value);manifest['software_manifest']=str(run/'foreign-software.json')
            elif kind in ('mode_pre','gap_post','cap_pre','gap_cap_post','watermark_pre','watermark_post','read_command'):
                path=Path(manifest['pre_readback' if kind in ('mode_pre','cap_pre','watermark_pre','read_command') else 'post_readback']);value=json.loads(path.read_text())
                if kind=='read_command':value['command']='pn-control set-mode S; pn-control read'
                elif kind.startswith('watermark_'):value['stdout']=value['stdout'].replace('PRE_SEQ=0','PRE_SEQ=1').replace('PRE_SEQ=15','PRE_SEQ=14')
                elif kind in ('cap_pre','gap_cap_post'):value['stdout']=value['stdout'].replace('"cap": 111','"cap": 333') if kind=='cap_pre' else value['stdout'].replace('"gap_cap": 222','"gap_cap": 333')
                else:value['stdout']=value['stdout'].replace('"mode": 0','"mode": 1') if kind=='mode_pre' else value['stdout'].replace('"v_gap_cycles": 0','"v_gap_cycles": 1000')
                save(path,value)
            elif kind=='read_alias':manifest['post_readback']=manifest['pre_readback']
            elif kind=='extra_attempt':save(run/'uart/command-timeout.json',{'exit_code':None,'error':'timeout','stdout':'retained failed tail'})
            elif kind=='duplicate_raw':records[1]['raw_command_path']=records[0]['raw_command_path'];save(run/'records.json',records)
            elif kind=='reorder':records[0],records[1]=records[1],records[0];save(run/'records.json',records)
            else:
                path=Path(records[1]['raw_command_path']);raw=json.loads(path.read_text())
                if kind=='cross_boot':raw['stdout']=raw['stdout'].replace(boot,'22345678-1234-1234-1234-123456789abc')
                elif kind=='sequence_gap':raw['stdout']=raw['stdout'].replace('PRE_SEQ=1','PRE_SEQ=2').replace('obs seq=2 ','obs seq=3 ')
                elif kind=='command':raw['command']='wrong'
                elif kind in ('ABI','V_pages','A_pages','data_error','A_bytes','cached_record'):
                    opening=raw['stdout'].index('{"schema"');row,length=json.JSONDecoder().raw_decode(raw['stdout'][opening:])
                    if kind=='ABI':row['input']['abi_version']=2
                    elif kind=='V_pages':row['input']['victim_pages']=2
                    elif kind=='A_pages':row['input']['aggressor_pages']=8
                    elif kind=='data_error':row['result']['aggressor']['data_errors']=1
                    elif kind=='A_bytes':row['result']['aggressor']['completed_bytes']+=1
                    else:records[1]['record']['result']['victim_elapsed_ns']+=1;save(run/'records.json',records)
                    raw['stdout']=raw['stdout'][:opening]+json.dumps(row)+raw['stdout'][opening+length:]
                elif kind=='shared_tamper':raw['stdout']=re.sub(r'(part=axi_a[^\n]*shared_ar=)(\d+)',lambda m:m[1]+str(int(m[2])+1),raw['stdout'])
                elif kind=='PATH_tamper':raw['stdout']=re.sub(r'(role=v channel=r stage=1[^\n]*full=)(\d+)',lambda m:m[1]+str(int(m[2])+1),raw['stdout'])
                else:raw['stdout']=re.sub(r'idma_path_obs[^\n]*role=v channel=r stage=1[^\n]*\n','',raw['stdout'])
                save(path,raw)
            save(run/'manifest.json',manifest)
            target=build/('no-output-'+kind)
            reject(kind+'_rejected_before_output',lambda:gate.export(run,a.source_root,target,run/'fpga.json',run/'source.json',expected_cva6,expected_iommu,_fixture_mode=True))
            assert not target.exists()
        run,records,manifest=make('public-fixture');target=build/'public-no-output'
        argv=[sys.executable,str(Path(gate.__file__)),'--run',str(run),'--source-root',str(a.source_root),'--output',str(target),'--fpga-manifest',str(run/'fpga.json'),'--source-manifest',str(run/'source.json'),'--expected-cva6',expected_cva6,'--expected-iommu',expected_iommu]
        completed=subprocess.run(argv,capture_output=True,text=True,timeout=30);assert completed.returncode and not target.exists()
        public.append({'argv':argv,'exit_code':completed.returncode,'stdout':completed.stdout,'stderr':completed.stderr})
        checks.append({'name':'public_CLI_recursive_fixture_refusal_before_output','passed':True})
        # Isolate a nested marker in an otherwise untagged private test copy.
        def untag(value):
            if isinstance(value,dict):return {k:untag(v) for k,v in value.items() if k!='fixture_only'}
            if isinstance(value,list):return [untag(v) for v in value]
            return value
        for path in run.rglob('*.json'):save(path,untag(json.loads(path.read_text())))
        value=json.loads((run/'manifest.json').read_text());value['nested']={'is_fixture':True};save(run/'manifest.json',value)
        reject('nested_metadata_marker_public_refusal',lambda:gate.analyze(run,a.source_root,run/'fpga.json',run/'source.json',expected_cva6,expected_iommu))
        for kind in ('CLI','control'):
            run,records,manifest=make('decoded-nested-'+kind)
            for path in run.rglob('*.json'):
                value=untag(json.loads(path.read_text()))
                if 'stdout' in value:
                    opening=value['stdout'].index('{"schema"');row,length=json.JSONDecoder().raw_decode(value['stdout'][opening:])
                    value['stdout']=value['stdout'][:opening]+json.dumps(untag(row))+value['stdout'][opening+length:]
                save(path,value)
            fpga=json.loads((run/'fpga.json').read_text());fpga['source_manifest_sha256']=gate.sha((run/'source.json').read_bytes());save(run/'fpga.json',fpga)
            manifest=json.loads((run/'manifest.json').read_text());manifest.update(source_manifest_sha256=gate.sha((run/'source.json').read_bytes()),fpga_manifest_sha256=gate.sha((run/'fpga.json').read_bytes()),load_manifest_sha256=gate.sha((run/'load.json').read_bytes()));save(run/'manifest.json',manifest)
            path=run/'uart'/('command-001.json' if kind=='CLI' else 'command-pre.json');raw=json.loads(path.read_text());opening=raw['stdout'].index('{"schema"');row,length=json.JSONDecoder().raw_decode(raw['stdout'][opening:]);row['nested']={'is_fixture':True}
            raw['stdout']=raw['stdout'][:opening]+json.dumps(row)+raw['stdout'][opening+length:];save(path,raw)
            # Leave the cached record untagged: this negative must exercise the
            # decoded-raw guard before the later raw/cache equivalence check.
            target=build/('decoded-no-output-'+kind)
            reject('decoded_nested_'+kind+'_public_refusal',lambda:gate.export(run,a.source_root,target,run/'fpga.json',run/'source.json',expected_cva6,expected_iommu))
            assert not target.exists() and 'decoded synthetic' in checks[-1]['actual_error']
        run,records,manifest=make('cached-metric');records[0]['metrics']['hardware_ms']=-1;save(run/'records.json',records)
        result=analyze(run);assert result['numerical_gate_passed'] and all(gate.Decimal(c['hardware_median_ms'])>0 for c in result['cells'])
        checks.append({'name':'cached_metrics_ignored_actual_raw_hardware_cycles_recomputed','passed':True})
        run,records,manifest=make('race');original_unchanged=gate.Inputs.unchanged
        def change_then_check(self):
            path=Path(records[0]['raw_command_path']);path.write_bytes(path.read_bytes()+b' ');original_unchanged(self)
        with patch.object(gate.Inputs,'unchanged',change_then_check):reject('source_byte_change_during_validation_rejected',lambda:analyze(run))
        assert 'uart' not in sys.modules and 'serial' not in sys.modules
        checks.append({'name':'no_UART_or_serial_import_no_hardware_entry','passed':True})
    receipt={'schema':'ispass2027.e6_pn.s15_gate.host.v1','status':'passed','fixture_only':True,'source_board_cases':0,'checks':checks,'public_refusal_raw':public,
        'gate_sha256':gate.sha(Path(gate.__file__).read_bytes()),'test_sha256':gate.sha(Path(__file__).read_bytes()),'source_map_sha256':gate.MAP_SHA,'parser':parser_identity,
        'temporary_constructed_inputs':'deleted after validation; all synthetic, reproducible from this source and locked references'}
    (a.output/'validation.json').write_text(gate.encoded(receipt));print(json.dumps({'status':'passed','checks':len(checks),'source_board_cases':0,'uart_imported':False}))


if __name__=='__main__':main()
