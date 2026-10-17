#!/usr/bin/env python3
"""Offline E6-PN S15 admission gate; never opens hardware or starts DS50."""
import argparse
import ast
from collections import defaultdict
from decimal import Decimal
import hashlib
import json
from pathlib import Path
import re
import statistics
import sys

sys.dont_write_bytecode=True
if not __debug__:
    raise RuntimeError('Python -O is unsupported: original parser assertions are required')

HERE=Path(__file__).resolve().parent
EXPERIMENT=HERE.parent
MAP_SHA='d9669346c23343117e2aa7408542c8c1ff7d6009abc6500b2fb9242dff2417fe'
KERNEL='6.5.0-rc2-ispass2027-pathv3'
LINUX='45bf76b15e36cf608dff95013d91f8bedb7b1b8e'
CLI_SHA='f91112f6d844af1fc683266a87a287edcbc42ddd9b3b419f40e37aac16a732f8'
GOLDEN={'Solo':Decimal('56.53868'),'A1':Decimal('107.09009'),'A8':Decimal('118.22441')}
IDENTITY_COMMAND='printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; '
WATERMARK_COMMAND=("pre_seq=$(dmesg | sed -n 's/.*idma_batch_obs seq=\\([0-9]*\\).*/\\1/p' | tail -n 1); "
    'printf "PRE_SEQ=%s\\n" "${pre_seq:-0}"; ')
READ_COMMAND=IDENTITY_COMMAND+WATERMARK_COMMAND+'pn-control read'
PARSER_ADAPTATION='Only replace the single legacy shared==sum(private accepts) assertion by same-window PATH0 full+carry AR/AW equalities; retain every other assertion.'


def require(value,message):
    if not value:raise ValueError(message)


def sha(raw):return hashlib.sha256(raw).hexdigest()


def encoded(value):return json.dumps(value,ensure_ascii=False,indent=2,allow_nan=False)+'\n'


def markers(value,path='$'):
    found=[]
    if isinstance(value,dict):
        for key,child in value.items():
            at=path+'.'+key
            flag=key in {'fixture_only','is_fixture','simulation_only','synthetic_fixture','mock_fixture'} and (child is True or str(child).lower() in {'true','1'})
            label=isinstance(child,str) and ((key in {'evidence_level','evidence_kind','source_kind'} and any(t in child.lower() for t in ('fixture','synthetic','mock'))) or (key in {'schema','condition'} and any(t in child.lower() for t in ('fixture','mock'))))
            boot=key=='boot_id' and isinstance(child,str) and child.startswith('HOST-FIXTURE')
            if flag or label or boot:found.append({'path':at,'value':child})
            found.extend(markers(child,at))
    elif isinstance(value,list):
        for index,child in enumerate(value):found.extend(markers(child,f'{path}[{index}]'))
    return found


def plan():return [[block,condition] for block in range(5) for condition in GOLDEN]


def command(condition):
    pages=1 if condition=='Solo' else int(condition[1:]);a='none' if condition=='Solo' else '1'
    return (IDENTITY_COMMAND+'printf "TAINT="; cat /proc/sys/kernel/tainted; '
        +WATERMARK_COMMAND+
        f'platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor {a} '
        f'--aggressor-pages {pages} --victim-pages 1 --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000; '
        'case_rc=$?; printf "CASE_EXIT=%s\\n" "$case_rc"; dmesg; '
        'printf "TAINT_AFTER="; cat /proc/sys/kernel/tainted')


class Inputs:
    def __init__(self,root,run,fixture):
        self.root=Path(root).resolve();self.run=Path(run).resolve();self.fixture=fixture;self.observed={};self.synthetic=[]

    def resolve(self,path):
        p=Path(path)
        if not p.is_absolute():p=self.root/p
        if not (p.is_relative_to(self.root) or p.is_relative_to(self.run)):
            for old in (Path('/home/xvhaocheng/work-root-a'),Path('/home/xvhaocheng/work')):
                if p.is_relative_to(old):p=self.root/p.relative_to(old);break
        p=p.resolve()
        require(p.is_relative_to(self.root) or p.is_relative_to(self.run),'source path outside explicit root/run: '+str(path))
        return p

    def read(self,path):
        p=self.resolve(path)
        if p not in self.observed:self.observed[p]=p.read_bytes()
        value=json.loads(self.observed[p]);found=markers(value,str(p));self.synthetic.extend(found)
        require(not found or self.fixture,'synthetic source cannot qualify for DS50: '+str(p))
        return p,value

    def unchanged(self):require(all(p.read_bytes()==raw for p,raw in self.observed.items()),'original input bytes changed during gate')

    def sources(self):return [{'path':str(p),'sha256':sha(raw)} for p,raw in self.observed.items()]


def parser(reader,mapping):
    source=next(x for x in mapping['existing_evidence'] if x['path'].endswith('collect.baseline100-as-dispatched.py'))
    path=reader.resolve(source['path']);raw=path.read_bytes()
    require(sha(raw)==source['sha256'],'frozen original parser changed')
    reader.observed[path]=raw
    function=next(n for n in ast.parse(raw).body if isinstance(n,ast.FunctionDef) and n.name=='parse')
    namespace=dict(re=re,json=json,defaultdict=defaultdict)
    exec(compile(ast.Module(body=[function],type_ignores=[]),str(path),'exec'),namespace)
    original=namespace['parse']
    target="a['shared_ar'] == v['ar'] + a['ar'] and a['shared_aw'] == v['aw'] + a['aw']"
    removed=[n for n in function.body if isinstance(n,ast.Assert) and ast.unparse(n.test)==target]
    require(len(removed)==1,'exact legacy assertion AST changed')
    function.body.remove(removed[0])
    adapted=dict(re=re,json=json,defaultdict=defaultdict)
    exec(compile(ast.Module(body=[function],type_ignores=[]),str(path),'exec'),adapted)
    def parse(stdout):
        item=adapted['parse'](stdout,KERNEL,True)
        paths=item['path'];require(set(paths)=={r+c+str(s) for r in 'va' for c in 'rw' for s in range(4)},'exact16 PATH stages required')
        a=item['observer']['axi_a']
        require(a['shared_ar']==sum(paths[k]['full']+paths[k]['carry'] for k in ('vr0','ar0')),'shared AR differs from selected PATH0 leaves')
        require(a['shared_aw']==sum(paths[k]['full']+paths[k]['carry'] for k in ('vw0','aw0')),'shared AW differs from selected PATH0 leaves')
        return item
    return original,parse,{'path':str(path),'sha256':sha(raw),'baseline_as_executed_sha256':source['sha256'],'adaptation':PARSER_ADAPTATION,'assertions_removed':1,'assertions_added':2}


def references(reader,mapping,original):
    ref=mapping['S15_reference_records'];path,records=reader.read(ref['path'])
    require(sha(reader.observed[path])==ref['sha256'],'fixed historical records changed')
    result={}
    require([x['condition'] for x in ref['conditions']]==list(GOLDEN),'fixed three reference cells changed')
    for cell in ref['conditions']:
        condition=cell['condition'];members=cell['members'];values=[]
        require(len(members)==10 and Decimal(str(cell['reference_median_hardware_ms']))==GOLDEN[condition],'fixed reference n/median changed')
        for member in members:
            cached=records[member['records_index']];path,raw=reader.read(member['raw_command_path'])
            require(sha(reader.observed[path])==member['raw_command_sha256'],'fixed historical raw changed')
            require(raw['command']==member['command']==command(condition) and raw['exit_code']==0,'historical command mismatch')
            parsed=original(raw['stdout'],KERNEL,True)
            require(all(parsed[k]==cached[k] for k in ('record','observer','path','boot_id','sequence')),'historical raw/cache mismatch')
            require(all(cached[k]==member[k] for k in ('case','block','boot_id','sequence')) and parsed['observer']['time']['cycles']==member['hardware_cycles'],'historical membership changed')
            values.append(Decimal(parsed['observer']['time']['cycles'])/Decimal(50000))
        require(statistics.median(values)==GOLDEN[condition],'historical raw median differs from fixed golden')
        result[condition]={'n':10,'hardware_median_ms':str(GOLDEN[condition]),'stable_row_id':cell['stable_row_id'],'members':members}
    return result


def software(reader,mapping,path):
    resolved,value=reader.read(path)
    expected=next(x['sha256'] for x in mapping['existing_evidence'] if x['path'].endswith('/manifests/software-build.json'))
    require(sha(reader.observed[resolved])==expected,'original software manifest bytes changed')
    require(value['status']=='passed' and value['selected_linux']==LINUX and value['kernel_release']==KERNEL,'original45bf kernel required')
    require(value['handwritten_inputs']['bench/idma_experiment.c']==CLI_SHA,'original f911 CLI required')


def cap_value(value):
    require(type(value) is int and 0<value<2**64,'actual nonzero64bit capability value required')
    return value


def source_cap_value(value):
    # The frozen build manifest uses hex strings; live helper JSON remains uint64.
    if type(value) is str:
        require(re.fullmatch(r'0x[0-9a-fA-F]{1,16}',value) is not None,'source capability hex required')
        value=int(value,16)
    return cap_value(value)


def readback(reader,path,boot,caps):
    resolved,raw=reader.read(path)
    require(raw['exit_code']==0 and raw['command']==READ_COMMAND,'exact read-only control command required')
    text=raw['stdout'].replace('\r','')
    require(re.findall(r'^BOOT=(.+)$',text,re.M)==[boot] and re.findall(r'^KERNEL=(.+)$',text,re.M)==[KERNEL],'readback boot/kernel mismatch')
    watermark=re.findall(r'^PRE_SEQ=(\d+)$',text,re.M);require(len(watermark)==1,'original readback observer watermark required')
    decoder=json.JSONDecoder();items=[decoder.raw_decode(text[m.start():])[0] for m in re.finditer(r'\{\s*"schema"\s*:\s*"idma\.pn_control\.v1"',text)]
    require(len(items)==1,'exactly one original control readback JSON required')
    item=items[0];found=markers(item,'decoded_control');reader.synthetic.extend(found)
    require(not found or reader.fixture,'decoded synthetic readback cannot qualify')
    # read_only means no experiment-configuration write. INDEX selector writes
    # are intrinsic to reading this bank and are not MODE/GAP/flush/reset actions.
    require(item['success'] is True and item['errno']==0 and item['operation']=='read' and item['read_only'] is True,'configuration-readonly success required')
    require([cap_value(item[k]) for k in ('cap','gap_cap')]==caps,'actual capabilities differ from source manifest')
    require(type(item['mode']) is int and item['mode']==0 and type(item['v_gap_cycles']) is int and item['v_gap_cycles']==0,'actual reset S/gap0 readback required')
    return {'path':str(resolved),'sha256':sha(reader.observed[resolved]),'record':item,'observer_watermark':int(watermark[0])}


def analyze(run,source_root,fpga_manifest,source_manifest,expected_cva6,expected_iommu,*,_fixture_mode=False):
    reader=Inputs(source_root,run,_fixture_mode)
    map_path=EXPERIMENT/'tool-source-map.json';map_raw=map_path.read_bytes();require(sha(map_raw)==MAP_SHA,'approved historical source map changed')
    reader.observed[map_path]=map_raw;reader.observed[Path(__file__).resolve()]=Path(__file__).read_bytes()
    mapping=json.loads(map_raw);original,parse,parser_source=parser(reader,mapping);reference=references(reader,mapping,original)
    _,manifest=reader.read(Path(run)/'manifest.json');_,cached=reader.read(Path(run)/'records.json')
    require(manifest['phase']=='e6-pn-s15' and manifest['status']=='passed' and manifest['recorded_cases']==len(cached)==15,'original complete passed S15 required')
    require(manifest['planned_cases']==plan(),'five complete fixed-order blocks required')
    require(manifest['control_actions']==[],'S15 requires explicit empty control_actions')
    require(not any(manifest.get(k) for k in ('error','derived_snapshot','resume_run','recovered_cases','recovered_raw_commands','discarded_cases','failed_cases','replaced_cases')),'S15 cannot use resumed/replaced/partial data')
    require(manifest['kernel_release']==KERNEL and manifest['ds']=='0/0','original kernel/DS0 required')
    require(isinstance(manifest['script_sha256'],str) and re.fullmatch(r'[0-9a-f]{64}',manifest['script_sha256']),'actual collector as-executed hash required')
    collector_source=reader.resolve(manifest['script_source']);raw=collector_source.read_bytes();reader.observed[collector_source]=raw
    require(sha(raw)==manifest['script_sha256'],'actual collector source bytes changed')
    require(all(re.fullmatch(r'[0-9a-f]{40}',x) for x in (expected_cva6,expected_iommu)),'Root must explicitly pin full candidate commits')
    require(expected_cva6!=mapping['baseline_identity']['cva6_commit'] and expected_iommu!=mapping['baseline_identity']['iommu_commit'],'new PN source commits required')
    fpga_path,fpga=reader.read(fpga_manifest);source_path,source=reader.read(source_manifest)
    require(fpga['status']=='passed' and fpga['action']=='fpga-ds0' and fpga['variant']=='ds0','actual passed new DS0 FPGA manifest required')
    require(fpga['selected_cva6']==source['selected_cva6']==expected_cva6 and fpga['selected_iommu']==source['iommu']==expected_iommu,'Root-pinned candidate source mismatch')
    require(source['linux']==LINUX and type(source['reset_mode']) is int and source['reset_mode']==0 and type(source['v_gap_reset_cycles']) is int and source['v_gap_reset_cycles']==0,'actual source reset/software contract required')
    caps=[source_cap_value(source[k]) for k in ('pn_cap','gap_cap')]
    require(isinstance(fpga['source_manifest'],str) and fpga['source_manifest'],'original FPGA source-manifest reference missing')
    require(fpga['source_manifest_sha256']==sha(reader.observed[source_path])==manifest['source_manifest_sha256'],'actual source manifest bytes mismatch')
    require(sha(reader.observed[fpga_path])==manifest['fpga_manifest_sha256'],'Root-locked FPGA manifest bytes mismatch')
    require(reader.resolve(manifest['fpga_manifest'])==fpga_path and reader.resolve(manifest['source_manifest'])==source_path,'run cannot relabel locked manifests')
    bits=[value for path,value in fpga['artifacts'].items() if path.endswith('.bit')]
    require(len(bits)==1 and re.fullmatch(r'[0-9a-f]{64}',bits[0]),'unique existing BIT hash required')
    require(manifest['bit_sha256']==bits[0],'run BIT differs from actual build')
    loaded_path,loaded=reader.read(manifest['load_manifest'])
    require(sha(reader.observed[loaded_path])==manifest['load_manifest_sha256'],'original load receipt changed')
    require(loaded['status']=='boot_identity_verified' and loaded['bit_sha256']==bits[0] and loaded['kernel_release']==KERNEL,'actual load/software identity mismatch')
    require(loaded['program_file_sha256_verified'] is True,'actual programmed BIT verification receipt required')
    boot=loaded['boot_id'];software(reader,mapping,manifest['software_manifest'])
    pre=readback(reader,manifest['pre_readback'],boot,caps);post=readback(reader,manifest['post_readback'],boot,caps)
    require(pre['path']!=post['path'],'independent saved pre/post reads required')
    fresh=[];members=[];paths=set();seen=set()
    for index,(block,condition) in enumerate(plan()):
        item=cached[index];require(all(item[k]==v for k,v in {'case':index+1,'block':block,'condition':condition,'ds':'0/0'}.items()),'ordered case metadata changed')
        path,raw=reader.read(item['raw_command_path']);require(path.is_relative_to(Path(run).resolve()) and path not in paths,'unique original run-local raw per trial required');paths.add(path)
        require(raw['exit_code']==0 and raw['command']==command(condition),'original complete command mismatch/failure')
        parsed=parse(raw['stdout']);decoded=markers(parsed,'decoded_trial:'+str(index+1));reader.synthetic.extend(decoded)
        require(not decoded or _fixture_mode,'decoded synthetic CLI cannot qualify')
        inputs=parsed['record']['input'];result=parsed['record']['result'];pages=1 if condition=='Solo' else int(condition[1:])
        require(inputs=={'device':'/dev/idma0','caps_only':False,'abi_version':1,'struct_size':328,'flags':0,'aggressor_id':None if condition=='Solo' else 1,'victim_pages':1,'aggressor_pages':pages,'chunk_bytes':4096,'run_timeout_ms':60000,'victim_transfers':4096,'warmup_transfers':256,'reserved':[0]*8},'exact original S15 CLI input required')
        require(result['victim_did']==10 and (result['aggressor'] is None)==(condition=='Solo'),'exact V0/A1 roles required')
        if condition!='Solo':
            require(result['aggressor_did']==11 and result['aggressor']['completed']>0 and result['aggressor']['completed_bytes']==result['aggressor']['completed']*4096,'A final work/bytes mismatch')
        require(all(parsed[k]==item[k] for k in ('record','observer','path','boot_id','sequence')),'cached result differs from original raw')
        key=parsed['boot_id'],parsed['sequence'];require(key[0]==boot and key not in seen,'cross-boot/duplicate sample forbidden')
        if fresh:require(key[1]==fresh[-1]['sequence']+1,'S15 observer sequences must be consecutive')
        seen.add(key);parsed.update(case=index+1,block=block,condition=condition);fresh.append(parsed)
        members.append({'case':index+1,'block':block,'condition':condition,'boot_id':boot,'sequence':key[1],'raw_command_path':str(path),'raw_sha256':sha(reader.observed[path]),'hardware_cycles':parsed['observer']['time']['cycles']})
    require(pre['observer_watermark']==members[0]['sequence']-1 and post['observer_watermark']==members[-1]['sequence'],'pre/post reads do not bracket actual15 observer sequences')
    inventory={p.resolve() for p in Path(run).rglob('command*.json')}
    require(inventory==paths|{Path(pre['path']),Path(post['path'])},'original command inventory contains extra/missing attempts')
    cells=[]
    for condition,ref in GOLDEN.items():
        values=[Decimal(x['observer']['time']['cycles'])/Decimal(50000) for x in fresh if x['condition']==condition]
        require(len(values)==5,'each actual S15 cell must have n5')
        median=statistics.median(values);delta=median/ref-1;passed=abs(median-ref)<=ref*Decimal('.005')
        cells.append({'condition':condition,'n':5,'hardware_median_ms':str(median),'reference_n':10,'reference_median_ms':str(ref),'relative_difference':str(delta),'passed':passed})
    synthetic=_fixture_mode or bool(reader.synthetic);numerical=all(c['passed'] for c in cells);eligible=numerical and not synthetic
    reader.unchanged()
    result={'schema':'ispass2027.e6_pn.s15_gate.v1','status':'passed' if eligible else 'synthetic_validation' if synthetic else 'failed',
        'eligible_for_DS50':eligible,'numerical_gate_passed':numerical,'fixture_only':synthetic,'source_board_cases':0 if synthetic else 15,
        'source_cases':15,'phase':'e6-pn-s15','boot_id':boot,'kernel_release':KERNEL,'bit_sha256':bits[0],
        'selected_cva6':expected_cva6,'selected_iommu':expected_iommu,'control_actions':[],'cells':cells,'members':members,
        'historical_reference':reference,'readbacks':{'pre':pre,'post':post},'parser':parser_source,'source_map_sha256':MAP_SHA,
        'collector_recorded_script_sha256':manifest['script_sha256'],'collector_source_path':str(collector_source),'collector_source_bytes_verified':True,
        'original_campaign_status':manifest['status'],'source_files':reader.sources(),'synthetic_markers':reader.synthetic,
        'scope':'cross-boot descriptive median admission; no pairing with historical trials, no deleted/replacement points, no automatic hardware or DS50 action'}
    return result,reader


def export(run,source_root,output,fpga_manifest,source_manifest,expected_cva6,expected_iommu,*,_fixture_mode=False):
    output=Path(output).resolve();run=Path(run).resolve()
    require(output.is_relative_to(EXPERIMENT) and not output.exists() and not output.is_relative_to(run),'new E6-local output outside original run required')
    result,reader=analyze(run,source_root,fpga_manifest,source_manifest,expected_cva6,expected_iommu,_fixture_mode=_fixture_mode)
    reader.unchanged();output.mkdir(parents=True,exist_ok=False)
    result['script_sha256']=sha(reader.observed[Path(__file__).resolve()]);(output/'gate.json').write_text(encoded(result))
    lines=[f"S15 gate: {result['status']}; eligible_for_DS50={result['eligible_for_DS50']}; source_board_cases={result['source_board_cases']}",'',
        '| 条件 | n | 新median ms | 固定参考median ms (n10) | 相对偏差 | ≤0.5% |','| --- | ---: | ---: | ---: | ---: | --- |']
    lines += ['| '+c['condition']+' | 5 | '+c['hardware_median_ms']+' | '+c['reference_median_ms']+' | '+c['relative_difference']+' | '+str(c['passed'])+' |' for c in result['cells']]
    lines += ['',result['scope'],'', '合成验证无论文/DS50准入资格。' if result['fixture_only'] else '仅离线准入判定；无板操作或自动构建。','']
    (output/'report.md').write_text('\n'.join(lines));return result


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ('run','source-root','output','fpga-manifest','source-manifest'):p.add_argument('--'+name,type=Path,required=True)
    for name in ('expected-cva6','expected-iommu'):p.add_argument('--'+name,required=True)
    a=p.parse_args();result=export(a.run,a.source_root,a.output,a.fpga_manifest,a.source_manifest,a.expected_cva6,a.expected_iommu)
    print(json.dumps({'status':result['status'],'eligible_for_DS50':result['eligible_for_DS50'],'source_board_cases':result['source_board_cases']}))
    return 0 if result['eligible_for_DS50'] else 1


if __name__=='__main__':raise SystemExit(main())
