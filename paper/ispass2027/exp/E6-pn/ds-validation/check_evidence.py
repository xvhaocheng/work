#!/usr/bin/env python3
"""Read-only verification of immutable DS sources, raw events and elaboration."""
import argparse,csv,gzip,hashlib,json,re,subprocess,xml.etree.ElementTree as ET
from pathlib import Path
HERE=Path(__file__).resolve().parent
checks=0
def need(ok,why):
 global checks
 checks+=1
 if not ok:raise AssertionError(why)
def sha(b):return hashlib.sha256(b).hexdigest()
def unpack(folder,name):
 z=folder/(name+'.gz');return gzip.decompress(z.read_bytes()) if z.exists() else (folder/name).read_bytes()
def edge(n,inc,dec):
 if inc==dec:return n,False
 if inc:return (255,True) if n==255 else (n+1,False)
 return (0,True) if not n else (n-1,False)
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--iommu-root',type=Path,required=True);a=ap.parse_args();repo=a.iommu_root.resolve()
 s=json.loads((HERE/'summary.json').read_text());commit=s['component_commit'];base=s['component_parent']
 def git(ref,path):return subprocess.check_output(['git','show',ref+':'+path],cwd=repo)
 for folder in sorted((HERE/'evidence').iterdir()):
  m=json.loads((folder/'manifest.json').read_text())
  for x in m['inputs']:
   origin=Path(x['path']);prefix=next(p for p in origin.parents if p.name=='source');p=folder/'source'/origin.relative_to(prefix)
   need(sha(p.read_bytes())==x['sha256'],'frozen source '+str(p))
  for name in ['build.log','run.log','cycle-trace.csv','cases.csv','elaboration.xml','elaboration.log']:
   if name in m:need(sha(unpack(folder,name))==m[name]['sha256'],'raw hash '+str(folder/name))
 final=HERE/'evidence'/s['final_run'];m=json.loads((final/'manifest.json').read_text());need(m['status']=='passed_finite_ds_only','execution status')
 need(subprocess.check_output(['git','diff','--name-only',base,commit],cwd=repo,text=True).splitlines()==s['changed_production_files'],'production scope')
 for x in s['source_commit_binding']:
  raw=git(commit,x['path']);need(sha(raw)==x['sha256'] and raw==(final/'source'/Path(x['path']).name).read_bytes(),'execution/commit byte binding '+x['path'])
 rel='rtl/ext_interfaces/rv_iommu_ds_if.sv';gold=git(base,rel)
 renamed,n=re.subn(rb'(?m)^module rv_iommu_ds_if\b',b'module rv_iommu_ds_if_gold',gold,count=1);need(n==1,'single module namespace transform');need(renamed==(final/'source/rv_iommu_ds_if_gold.sv').read_bytes(),'gold source transformation')
 candidate=git(commit,rel)
 candidate=candidate.replace(b'parameter type axi_resp_t   = logic,\n    parameter bit PnEnable      = 1\'b0',b'parameter type axi_resp_t   = logic')
 candidate=candidate.replace(b'output axi_resp_t   msi_ig_resp_o,\n\n    // Read-only PN configuration guards; not full-system lifecycle idle.\n    output logic       pn_buffers_quiet_o,\n    output logic       pn_transport_quiet_o',b'output axi_resp_t   msi_ig_resp_o')
 candidate,n=re.subn(rb'    if \(PnEnable\) begin : gen_pn_quiet\n.*?    //# Unused signals',b'    //# Unused signals',candidate,flags=re.S);need(n==1,'one optional guard block');need(candidate==gold,'all original DS datapath source bytes unchanged')
 walker='rtl/translation_logic/rv_iommu_msiptw.sv';need(git(commit,walker)==git(base,walker)==(final/'source/rv_iommu_msiptw.sv').read_bytes(),'real MSIPTW unchanged')
 # Elaborated parameter specialization, not a textual parameter assumption.
 xml=ET.fromstring(unpack(final,'elaboration.xml'));dsmods=[v for v in xml.findall('.//module') if v.get('origName')=='rv_iommu_ds_if'];need(len(dsmods)==2,'enabled/defaultoff specialized DS modules')
 flags={}
 for mod in dsmods:
  v=next(x for x in mod.findall('./var') if x.get('name')=='PnEnable');enabled=v.find('const').get('name')=="1'h1";debt=[x for x in mod.findall('.//instance') if x.get('defName')=='rv_iommu_ds_debt'];need(len(debt)==int(enabled),'debt exists iff PnEnable');flags[int(enabled)]=mod.get('name')
 need(set(flags)=={0,1},'both compile variants elaborated')
 debt=next(x for x in xml.findall('.//module') if x.get('origName')=='rv_iommu_ds_debt');types={x.get('id'):x for x in xml.findall('.//typetable/*')};widths={}
 for v in debt.findall('./var'):
  if v.get('name') in ['read_q','write_q','bad_q']:
   ty=types[v.get('dtype_id')];widths[v.get('name')]=abs(int(ty.get('left','0'))-int(ty.get('right','0')))+1
 need(widths=={'read_q':8,'write_q':8,'bad_q':1},'17 state bits in optional tracker')
 rows=list(csv.DictReader(unpack(final,'cycle-trace.csv').decode().splitlines()));need(len(rows)==s['finite_results']['evals'],'raw eval count');need(all(None not in r and None not in r.values() for r in rows),'CSV rectangular')
 d=[0,0,False];u=[0,0,False];prev=None;gapcycles=set();tailspill=False;wbefore=False;bad_data=0;actual_fill=0
 def step(state,ri,rd,wi,wd):
  nr,br=edge(state[0],ri,rd);nw,bw=edge(state[1],wi,wd);return [nr,nw,state[2] or br or bw]
 for row in rows:
  r={k:(int(v,16) if k in ['r_data','w_data','ar_addr','aw_addr'] else int(v)) for k,v in row.items() if k!='phase'};r['phase']=row['phase']
  if not r['rst']:d=[0,0,False];u=[0,0,False]
  elif prev and not prev['clk'] and r['clk']:
   p=prev;d=step(d,p['ar_valid'] and p['ar_ready'],p['r_valid'] and p['r_ready'] and p['rlast'],p['aw_valid'] and p['aw_ready'],p['b_valid'] and p['b_ready']);u=step(u,p['unit_ri'],p['unit_rl'],p['unit_wi'],p['unit_wd']);actual_fill+=p['msi_fill']
  need([r['read_debt'],r['write_debt'],bool(r['bad'])]==d,'independent raw DS debt replay')
  need([r['unit_read'],r['unit_write'],bool(r['unit_bad'])]==u,'independent raw unit debt replay')
  need(r['equal_on']==1 and r['equal_off']==1,'per-eval complete AXI struct gold equality')
  buffers=r['owner_empty'] and not any(r[k] for k in ['producer_offer','ar_spill','aw_spill','w_spill','r_spill','b_spill','r_valid','b_valid'])
  need(bool(r['buffers_quiet'])==bool(buffers),'raw offered/spill/owner buffer quiet')
  need(bool(r['transport_quiet'])==bool(buffers and not any(d)),'raw complete DS transport quiet')
  if r['phase']=='actual-MSIPTW-two-beat-BT-tail-gap' and r['msi_idle'] and r['buffers_quiet'] and r['read_debt']==1 and not r['r_valid']:
   need(not r['transport_quiet'] and not r['bad'],'real MSI idle gap not quiet');gapcycles.add(r['cycle'])
  if r['phase']=='actual-MSIPTW-tail-in-response-spill' and r['read_debt']==0 and r['r_spill'] and not r['r_valid']:
   need(not r['transport_quiet'],'real tail debt retired but response spill held');tailspill=True
  if r['phase']=='W-before-AW-owner-WLAST' and r['prod_w_valid'] and r['owner_empty'] and not r['prod_w_ready']:wbefore=True
  if r['bad']:bad_data+=1
  prev=r
 need(len(gapcycles)>=24,'at least24 actual BT IDLE/noVALID gap cycles');need(tailspill,'actual MSIPTW tail response spill edge');need(wbefore,'W before AW held while owner empty');need(bad_data>1000,'bad sticky with continued exact old dataflow');need(actual_fill==1,'one actual MSIPTW IOTLB update')
 need(any(r['msi_mode']=='1' and r['ar_valid']=='1' and r['ar_len']=='1' and r['ar_id']=='4' for r in rows),'real two-beat MSIPTW AR ID4')
 need(any(r['phase']=='actual-MSIPTW-two-beat-BT-tail-gap' and r['r_valid']=='1' and r['rlast']=='0' and int(r['r_data'],16)==((0x80000<<10)|7) for r in rows),'actual valid BT first PTE beat')
 for phase in ['unit-net-zero-at-zero','unit-net-zero-at-255','unit-overflow-saturates-sticky','unit-drain-cannot-clear-bad','unit-reset-only-and-underflow','unit-independent-cross-events','DS-read-overflow-preserves-original-dataflow','DS-write-overflow-preserves-original-dataflow','R-spill-after-external-RLAST','B-spill-after-external-B','legal-random-concurrent-backpressure']:
  need(any(r['phase']==phase for r in rows),'required finite phase '+phase)
 print(json.dumps({'status':'passed','checks':checks,'raw_evals':len(rows),'msi_idle_gap_cycles':len(gapcycles),'production_state_bits':17,'defaultoff_debt_instances':0,'board_cases':0},indent=2))
if __name__=='__main__':main()
