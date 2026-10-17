#!/usr/bin/env python3
"""Read-only binding, raw quiet and original sequential-AST checks; no simulation."""
import argparse,csv,gzip,hashlib,json,re,subprocess,xml.etree.ElementTree as ET
from pathlib import Path
HERE=Path(__file__).resolve().parent;checks=0
def need(x,s):
 global checks
 checks+=1
 if not x:raise AssertionError(s)
def sha(b):return hashlib.sha256(b).hexdigest()
def raw(folder,name):
 p=folder/(name+'.gz');return gzip.decompress(p.read_bytes()) if p.exists() else (folder/name).read_bytes()
def norm(x):
 # Locations and local XML dtype indexes differ between namespace copies.
 attrs={k:v for k,v in sorted(x.attrib.items()) if k!='loc' and not k.endswith('dtype_id')}
 if x.tag=='sformatf' and 'name' in attrs:
  # Verilator embeds renamed diagnostic filenames/line numbers/module names.
  # Retain the actual assertion expression and diagnostic text.
  attrs['name']=re.sub(r'(\.sv:)\d+',r'\1LINE',attrs['name'].replace('_gold',''))
 return [x.tag,attrs,[norm(c) for c in x]]
def signatures(mod):
 out=[]
 for node in mod.findall('.//always'):
  if any(x.get('edgeType') in ['POS','NEG'] for x in node.findall('./sentree/senitem')):
   need(not any(x.get('name','').lower().startswith('pn_') for x in node.findall('.//varref')),'new PN logic in sequential process')
   out.append(sha(json.dumps(norm(node),sort_keys=True).encode()))
 return sorted(out)
def value(mod,name):
 v=next(x for x in mod.findall('./var') if x.get('name')==name);s=v.find('const').get('name');_,s=s.split("'",1);s=s.lstrip('s');return int(s[1:],{'h':16,'d':10,'b':2,'o':8}[s[0]])
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--iommu-root',type=Path,required=True);ap.add_argument('--axi-root',type=Path,required=True);a=ap.parse_args();s=json.loads((HERE/'summary.json').read_text());repos={'iommu':a.iommu_root.resolve(),'axi':a.axi_root.resolve()}
 def git(repo,ref,path):return subprocess.check_output(['git','show',ref+':'+path],cwd=repo)
 for folder in sorted((HERE/'evidence').iterdir()):
  m=json.loads((folder/'manifest.json').read_text())
  for item in m['inputs']:
   p=Path(item['path']);prefix=next(x for x in p.parents if x.name=='source');q=folder/'source'/p.relative_to(prefix);need(sha(q.read_bytes())==item['sha256'],'immutable input '+str(q))
  for n in ['build.log','run.log','elaboration.xml','elaboration.log','cycle-trace.csv']:
   if n in m:need(sha(raw(folder,n))==m[n]['sha256'],'immutable raw '+str(folder/n))
  for t in m['gold_transforms']:
   repo=repos['axi' if t['commit']==s['vendor_axi_parent'] else 'iommu'];data=git(repo,t['commit'],t['path']);need(sha(data)==t['original_sha256'],'original gold blob')
   for old,new in t['namespace_identifier_replacements'].items():data=re.sub(rb'\b'+old.encode()+rb'\b',new.encode(),data)
   need(data==(folder/'source'/(Path(t['path']).stem+'_gold.sv')).read_bytes(),'gold namespace-only closure')
 for key,base,commit,expected in [('iommu',s['iommu_parent'],s['iommu_commit'],s['changed_iommu_files']),('axi',s['vendor_axi_parent'],s['vendor_axi_commit'],s['changed_axi_files'])]:
  need(subprocess.check_output(['git','diff','--name-only',base,commit],cwd=repos[key],text=True).splitlines()==expected,'component changed scope '+key)
 need(subprocess.check_output(['git','rev-parse',s['iommu_commit']+':vendor/axi'],cwd=repos['iommu'],text=True).strip()==s['vendor_axi_commit'],'actual vendor gitlink')
 for b in s['source_commit_binding']:
  data=git(repos[b['repository']],b['commit'],b['path']);need(sha(data)==b['sha256'] and data==(HERE/b['snapshot_path']).read_bytes(),'exact execution/commit binding '+b['path'])
  for run in s['runs']:
   p=HERE/'evidence'/run/'source'/('vendor' if b['repository']=='axi' else '')/Path(b['path']).name
   if p.exists():need(p.read_bytes()==data,'all passed executions same production bytes')
 targets=['axi_demux','axi_demux_simple','axi_demux_id_counters','rv_iommu_trans_ctl_wrap','rv_iommu_banked_ctl','rv_iommu_mrif_handler'];seqcomparisons=0;configs=set()
 for run in s['runs']:
  folder=HERE/'evidence'/run;m=json.loads((folder/'manifest.json').read_text());need(m['status'].startswith('passed_finite_'),'final status '+run);xml=ET.fromstring(raw(folder,'elaboration.xml'));mods=xml.findall('.//module')
  for name in targets:
   gold={tuple(signatures(x)) for x in mods if x.get('origName')==name+'_gold'}
   for x in mods:
    if x.get('origName')==name:
     need(tuple(signatures(x)) in gold,'original sequential AST unchanged '+name);seqcomparisons+=1
     if name=='axi_demux':configs.add((value(x,'NoMstPorts'),value(x,'UniqueIds'),value(x,'SpillAw'),value(x,'SpillAr'),value(x,'SpillW'),value(x,'SpillB'),value(x,'SpillR'),value(x,'PnEnable')))
 for n,spill in [(3,1),(2,1),(2,0)]:need((n,0,spill,spill,spill,spill,spill,1) in configs,'actual enabled main demux config')
 need(any(c[0]==1 and c[-1]==1 for c in configs),'actual pass-through failclosed specialization');need(any(c[1]==1 and c[-1]==1 for c in configs),'actual UniqueIds failclosed specialization')
 def rows(run):
  rs=list(csv.DictReader(raw(HERE/'evidence'/run,'cycle-trace.csv').decode().splitlines()));need(all(None not in r and None not in r.values() for r in rs),'rectangular trace '+run);return rs
 vr=rows('tc-vendor03');need(len(vr)==s['runs']['tc-vendor03']['evals'],'vendor eval count');triples=set();supported_idle=0;unsupported=0
 for row in vr:
  r={k:int(v) for k,v in row.items() if k!='phase'}
  need(r['equal_on']==r['equal_off']==1,'vendor complete gold struct')
  if r['kind']>=2:need(r['idle']==0,'unsupported no-inventory neveridle');unsupported+=1
  else:
   p=r['awempty'] and r['arempty'] and r['wopen']==0 and not any(r[k] for k in ['awlock','arlock','awchan','awsel','archan','arsel','wcut','bcut','rcut','awvalid','arvalid','wvalid','mbvalid','mrvalid'])
   need(bool(r['idle'])==bool(p),'actual vendor readonly predicate');supported_idle+=r['idle']
  if r['ar_push'] and r['atop_inject'] and r['ar_pop']:triples.add(r['kind'])
 need(triples=={0,1},'real 3-way original read-counter events in bypass/spill config');need(supported_idle>20 and unsupported>100,'finite positive/unsupported coverage')
 br=rows('tc-banked01');need(len(br)==s['runs']['tc-banked01']['pre_edge_trace_points'],'banked preedge trace count');neg=0;qi=0
 for row in br:
  r={k:int(v,16) if k=='iova' else int(v) for k,v in row.items()};need(r['equal_on']==r['equal_off']==1 and r['off_idle']==0,'banked original gold/default0')
  if r['w_unfilled']>=2**31:need(r['quiet']==0,'negative W-before-AW inventory notquiet');neg+=1
  if r['quiet']:
   need(r['mux_empty']==1 and r['mux_awlock']==0 and r['bank_idle']==3 and r['input_idle']==1,'real mux invariant and bothbanks');need(not any(r[k] for k in ['inflight_aw','inflight_ar','inflight_w','partial','w_unfilled','awvalid','arvalid','wvalid','tlvalid','respvalid','cawvalid','carvalid','cwvalid','bvalid','rvalid']),'quiet without inventory/currentoffer');qi+=1
 need(neg>0 and qi>100,'banked negative/positive scope')
 sr=rows('tc-single03');need(len(sr)==s['runs']['tc-single03']['evals'],'single eval count');stale=0;queue=0;fault=0;waitgap=0
 for row in sr:
  r={k:int(v,16) if k in ['maraddr','mawaddr','mwdata'] else int(v) for k,v in row.items() if k!='phase'};need(r['equal_on']==r['equal_off']==1,'single original complete gold')
  if r['quiet']:need(not any(r[k] for k in ['tc_state','handler_state','awfifo','arfifo','wfifo','mrif_fifo','w_count','tlvalid','respvalid','awvalid','arvalid','wvalid','bvalid','rvalid','cawvalid','carvalid','cwvalid','marvalid','mrvalid','mawvalid','mwvalid','mbvalid','cbvalid','crvalid','faultvalid']) and r['demux_idle']==r['handler_idle']==1,'single current/state/response quiet')
  if r['handler_state'] in [1,2,3,4,5,6]:need(r['quiet']==0,'real MRIF active notquiet')
  if r['handler_state']==0 and r['wr_state']==2 and r['quiet']:stale+=1
  if row['phase']=='MRIF-FIFO-held-behind-WAIT-INT-ID' and r['mrif_fifo'] and r['handler_state']==1:need(r['quiet']==0,'queuedMRIF');queue+=1
  if r['faultvalid'] and not r['faultready']:need(r['faultcause']==270 and r['quiet']==0,'real fault code/held');fault+=1
  if r['handler_state']==1 and not any(r[k] for k in ['marvalid','mawvalid','mwvalid','mrvalid','mbvalid']):waitgap+=1
 need(stale>0 and queue>30 and fault>30 and waitgap>30,'real MRIF branch evidence')
 pending=json.loads((HERE/'flattened-source-pending.json').read_text());crepo=Path(pending['cva_repo']);mk=git(crepo,pending['makefile_commit'],pending['makefile']);need(sha(mk)==pending['makefile_sha256'],'actual FPGA source-list identity')
 for line in pending['actual_source_list_lines']:need(mk.decode().splitlines()[line['line']-1]==line['text'],'actual source-list numbered line')
 for item in pending['sources']:
  old=git(crepo,item['cva_base'],item['cva_path']);name=Path(item['cva_path']).name;new=git(repos['axi'],item['required_vendor_commit'],'src/'+name)
  need(sha(old)==item['cva_original_sha256'] and old==git(repos['axi'],item['vendor_base'],'src/'+name),'CVA flattened original exactly e722')
  need(sha(new)==item['new_vendor_sha256'] and not item['cva_modified_this_batch'],'accepted vendor bytes pending later authorized CVA sync')
 need(s['added_state_bits']==0 and s['board_cases']==0,'scope counts');need(s['pending_actual_cva_source_sync']['required'],'actual flattened FPGA input stillpending')
 print(json.dumps({'status':'passed','checks':checks,'original_sequential_ast_specializations':seqcomparisons,'vendor_evals':len(vr),'banked_preedge_points':len(br),'banked_negative_w_unfilled_points':neg,'banked_quiet_mux_empty_points':qi,'single_evals':len(sr),'mrif_stale_BRESP_quiet_points':stale,'mrif_queued_points':queue,'board_cases':0},indent=2))
if __name__=='__main__':main()
