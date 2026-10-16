#!/usr/bin/env python3
"""Final document navigation and preservation only; no simulator or experiment execution."""
from pathlib import Path
import hashlib,json,re,urllib.parse,datetime
W=Path('/home/xvhaocheng/work');R=W/'iommu-platform/out/d-persistent-source-20261015'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def part(p,start,end=None):
 b=p.read_bytes();s=b.index(start.encode());return b[s:b.index(end.encode(),s) if end else None]
def main():
 checks=[]
 def require(name,value):assert value,name;checks.append({'name':name,'pass':True})
 receipt=json.loads((R/'documentation/receipt.json').read_text())
 for z in receipt['files']:require('final doc hash '+z['path'],sha(W/z['path'])==z['after_sha256'])
 require('worker protection report',all(receipt['protected_assertions'].values()))
 before=R/'documentation/before-write'
 for f in ['HANDOFF.md','iommu-platform/docs/validation.md']:
  require('old full suffix '+f,(W/f).read_bytes().endswith((before/f).read_bytes()))
 require('A5 A2 exact bytes',part(W/'CURRENT_STATUS.md','## 各分支最新进展','## 硬件与执行状态')==part(before/'CURRENT_STATUS.md','## 各分支最新进展','## 硬件与执行状态'))
 require('engineering exact bytes',part(W/'CURRENT_STATUS.md','## 独立工程事项')==part(before/'CURRENT_STATUS.md','## 独立工程事项'))
 require('plan assets exact bytes',part(W/'研究计划.md','## 保留资产与执行边界')==part(before/'研究计划.md','## 保留资产与执行边界'))
 require('navigation assets exact bytes',part(W/'研究导航.md','## 已有成果怎样复用')==part(before/'研究导航.md','## 已有成果怎样复用'))
 initial=json.loads((R/'root/input-manifest.json').read_text())
 mut={'CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md'}
 for z in initial['files']:
  if z['path'] not in mut:require('protected source/history '+z['path'],sha(W/z['path'])==z['sha256'])
 require('old user method absent',not (W/'研究方法.md').exists())
 docs=[W/f for f in ['AGENTS.md','README.md','CURRENT_STATUS.md','研究计划.md','研究导航.md','iommu-platform/AGENTS.md','iommu-platform/README.md']]
 docs += [R/f for f in ['README.md','root/decision-analysis.md','ssd/evidence-card.md','ssd/assessment.md','ssd/boundary-hook.md','ssd/incomplete-flow-source.md','memory/card.md','memory/findings.md','observation/source-evidence.md']]
 texts={p:p.read_text() for p in docs}
 for f in ['HANDOFF.md','iommu-platform/docs/validation.md']:
  p=W/f;old=(before/f).read_bytes();now=p.read_bytes();texts[p]=now[:-len(old)].decode()
 links=[];broken=[]
 for p,t in texts.items():
  for raw in re.findall(r'\[[^\]\n]*\]\((<[^>]+>|[^)\n]+)\)',t):
   raw=raw.strip().strip('<>')
   if re.match(r'^[a-zA-Z][a-zA-Z0-9+.-]*:',raw) or raw.startswith('#'):continue
   target=urllib.parse.unquote(raw.split('#',1)[0]);q=Path(target) if target.startswith('/') else p.parent/target
   row={'document':str(p.relative_to(W)),'target':raw,'exists':q.exists()};links.append(row)
   if not row['exists']:broken.append(row)
 require('all maintained local links exist',not broken)
 require('source integrity already completed',json.loads((R/'root/source-integrity.json').read_text())['all_pass'])
 v=json.loads((R/'root/boundary-verification.json').read_text());require('accepted accounting and scoped incomplete verdict',v['structural_accounting']=='PASS' and v['complete_overwrite_boundaries']==0 and v['research_verdict'].startswith('NOT_ESTABLISHED'))
 for f in ['CURRENT_STATUS.md','研究计划.md','研究导航.md']:require('current entry points to new run '+f,'d-persistent-source-20261015' in (W/f).read_text())
 require('bounded next contract exists',(R/'root/next-minimal-contract.json').exists())
 report={'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'final document links/hashes/preserved bytes only; no simulator/replay/model/build/native/hardware executed','checks':checks,'check_count':len(checks),'maintained_local_links':len(links),'broken_links':broken,'link_details':links,'all_pass':True,'frozen_history_scope':'old full HANDOFF/validation suffix and original primary source bodies not reinterpreted as maintained navigation','research_verdict':'source facts accepted, complete overwrite boundary NOT_ESTABLISHED; no new P/PN or novelty acceptance'}
 (R/'root/delivery-verification.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ['check_count','maintained_local_links','all_pass']},ensure_ascii=False))
if __name__=='__main__':main()
