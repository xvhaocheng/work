#!/usr/bin/env python3
"""Only final document hashes, byte preservation and maintained local links."""
from pathlib import Path
import json,hashlib,re,urllib.parse,datetime
W=Path('/home/xvhaocheng/work');R=W/'iommu-platform/out/d-mqsim-identity-20261015'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def part(p,start,end=None):
 b=p.read_bytes();i=b.index(start.encode());return b[i:b.index(end.encode(),i) if end else None]
def main():
 checks=[]
 def require(label,ok):assert ok,label;checks.append({'name':label,'pass':True})
 rec=json.loads((R/'documentation/receipt.json').read_text());before=R/'documentation/before-write'
 for f,x in rec['files'].items():
  require('current document hash '+f,sha(W/f)==x['after_sha256'])
  require('snapshot hash '+f,sha(before/f)==x['before_write_sha256'])
 require('README hash',sha(R/'README.md')==rec['README_sha256'])
 require('worker byte-protection report',all(rec['protected_assertions'].values()))
 for f in ['HANDOFF.md','iommu-platform/docs/validation.md']:require('entire old suffix '+f,(W/f).read_bytes().endswith((before/f).read_bytes()))
 require('A5 A2 exact bytes',part(W/'CURRENT_STATUS.md','## 各分支最新进展','## 硬件与执行状态')==part(before/'CURRENT_STATUS.md','## 各分支最新进展','## 硬件与执行状态'))
 require('engineering exact bytes',part(W/'CURRENT_STATUS.md','## 独立工程事项')==part(before/'CURRENT_STATUS.md','## 独立工程事项'))
 require('I assets/hardware exact bytes',part(W/'研究计划.md','## I：')==part(before/'研究计划.md','## I：'))
 require('navigation assets exact bytes',part(W/'研究导航.md','## 已有成果怎样复用')==part(before/'研究导航.md','## 已有成果怎样复用'))
 changing={'CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md'}
 for z in json.loads((R/'root/input-manifest.json').read_text())['files']:
  if z['path'] not in changing:require('protected source/history '+z['path'],sha(W/z['path'])==z['sha256'])
 require('deleted old method remains absent',not (W/'研究方法.md').exists())
 files=[W/f for f in ['AGENTS.md','README.md','CURRENT_STATUS.md','研究计划.md','研究导航.md','iommu-platform/AGENTS.md','iommu-platform/README.md']]
 files +=[R/f for f in ['README.md','root/decision-analysis.md','root/diagnosis.md','design/repair-contract.md','implementation/README.md','implementation/repaired/README.md']]
 text={p:p.read_text() for p in files}
 for f in ['HANDOFF.md','iommu-platform/docs/validation.md']:
  p=W/f;old=(before/f).read_bytes();text[p]=p.read_bytes()[:-len(old)].decode()
 links=[]
 for p,s in text.items():
  for raw in re.findall(r'\[[^\]\n]*\]\((<[^>]+>|[^)\n]+)\)',s):
   raw=raw.strip().strip('<>')
   if re.match(r'^[A-Za-z][A-Za-z0-9+.-]*:',raw) or raw.startswith('#'):continue
   target=urllib.parse.unquote(raw.split('#',1)[0]);q=Path(target) if target.startswith('/') else p.parent/target
   links.append({'document':str(p.relative_to(W)),'target':raw,'exists':q.exists()})
 require('maintained local links all exist',all(x['exists'] for x in links))
 require('source byte identity checked',json.loads((R/'root/source-integrity.json').read_text())['all_pass'])
 v=json.loads((R/'root/repair-verification.json').read_text());require('four complete model cases accounted',v['status']=='PASS_FIXED_FOUR_WRITE_ONLY_CASES' and len(v['cases'])==4)
 require('no GC witness overclaim',all(c['gc_at_complete_host_pcie_boundary']==0 for c in v['cases']))
 for f in ['CURRENT_STATUS.md','研究计划.md','研究导航.md']:require('current entrance points to new evidence '+f,'d-mqsim-identity-20261015' in (W/f).read_text())
 require('explicit next source contract unexecuted',json.loads((R/'root/next-minimal-contract.json').read_text())['status'].endswith('not executed'))
 out={'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'document/hash/link preservation only; zero simulations or model retests','all_pass':True,'check_count':len(checks),'checks':checks,'maintained_local_links':len(links),'links':links,'frozen_history':'old HANDOFF/validation suffix not reclassified as maintained navigation; worker receipt separately records their pre-existing missing links','research_status':'fixed-case private completion accepted, no residual-GC witness, no native/P-PN/novelty acceptance'}
 (R/'root/delivery-verification.json').write_text(json.dumps(out,ensure_ascii=False,indent=2)+'\n');print(json.dumps({k:out[k] for k in ['all_pass','check_count','maintained_local_links']},ensure_ascii=False))
if __name__=='__main__':main()
