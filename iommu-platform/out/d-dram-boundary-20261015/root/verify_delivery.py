import hashlib,json,pathlib,re,datetime,urllib.parse
ROOT=pathlib.Path('/home/xvhaocheng/work');B=ROOT/'iommu-platform/out/d-dram-boundary-20261015'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
checks=[]
def ck(name,ok,detail=None):
 checks.append({'check':name,'passed':bool(ok),'detail':detail})
a=json.loads((B/'root/root-result-acceptance.json').read_text())
for rel,want in a['evidence_hashes'].items():ck('accepted evidence hash '+rel,sha(B/rel)==want)
r=json.loads((B/'verification/root-verified-report.json').read_text())
ck('nine independently accounted cases',r['status']=='passed' and len(r['cases'])==9)
ck('25 own read completions',sum(x['callbacks'] for x in r['cases'].values())==25)
ck('four negative ledger mutations rejected',len(r['negative_mutations'])==4 and all(x['rejected'] for x in r['negative_mutations']))
ck('source passivity and conditional replay',r['original_passivity_control_equal'] and r['same_physical_trace_full_replay_equal'])
ck('effective washout controls',all(all(v.values()) for v in [{k:z for k,z in p.items() if k!='control'} for p in r['washout_pairs'].values()]))
before=B/'documentation/before-write'
def protected(rel,start,end=None):
 old=(before/rel).read_bytes();new=(ROOT/rel).read_bytes();s=start.encode();e=None if end is None else end.encode()
 f=lambda text:text[text.index(s):None if e is None else text.index(e,text.index(s))]
 ck('protected '+rel+' '+start,f(old)==f(new))
protected('CURRENT_STATUS.md','## 各分支最新进展','## 硬件与执行状态')
protected('CURRENT_STATUS.md','## 独立工程事项')
protected('研究计划.md','## I：')
protected('研究导航.md','## 已有成果怎样复用')
prefixes={}
for rel in ['HANDOFF.md','iommu-platform/docs/validation.md']:
 old=(before/rel).read_bytes();new=(ROOT/rel).read_bytes();ok=new.endswith(old);ck('entire historical suffix '+rel,ok);prefixes[rel]=new[:-len(old)].decode() if ok else ''
receipt=json.loads((B/'documentation/receipt.json').read_text())
for rel,v in receipt['files'].items():ck('current documentation hash '+rel,sha(ROOT/rel)==v['after_sha256'])
frozen=json.loads((B/'root/input-manifest.json').read_text())
canon={'CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md'}
for v in frozen['files']:
 if v['path'] not in canon:ck('unchanged protected input '+v['path'],sha(ROOT/v['path'])==v['sha256'])
ck('old personal method stays removed',not (ROOT/'研究方法.md').exists())
ck('next model remains a contract',json.loads((B/'root/next-minimal-contract.json').read_text())['status']=='next bounded design contract; not executed')
link_sources={ROOT/rel:(ROOT/rel).read_text() for rel in ['CURRENT_STATUS.md','研究计划.md','研究导航.md']}
for rel,text in prefixes.items():link_sources[ROOT/rel]=text
for rel in ['README.md','root/decision-analysis.md','verification/README.md']:
 link_sources[B/rel]=(B/rel).read_text()
links=[]
for src,text in link_sources.items():
 for target in re.findall(r'(?<!!)\[[^\]]*\]\(([^\n]+?)\)',text):
  target=target.strip().strip('<>')
  if target.startswith(('http://','https://','mailto:','app://','codex://','#')):continue
  target=urllib.parse.unquote(target.split('#')[0]);target=re.sub(r':\d+$','',target)
  p=pathlib.Path(target);p=p if p.is_absolute() else src.parent/p
  links.append({'source':str(src.relative_to(ROOT)),'target':target,'exists':p.exists()})
ck('maintained local links exist',all(x['exists'] for x in links),{'count':len(links),'missing':[x for x in links if not x['exists']]})
for rel in ['CURRENT_STATUS.md','研究计划.md','研究导航.md']:
 s=(ROOT/rel).read_text();ck('current D-DRAM entry '+rel,'d-dram-boundary-20261015' in s and 'D-DRAM-ACTION-CONTRACT-1' in s)
ck('hardware pause retained','1792023314' in (ROOT/'CURRENT_STATUS.md').read_text() and 'paused' in (ROOT/'CURRENT_STATUS.md').read_text())
report={'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'status':'passed' if all(x['passed'] for x in checks) else 'failed','checks':checks,'maintained_links':links,'scope':'current entries and new result documents; frozen historical suffix not claimed link-clean; fourteen pre-existing old plan links remain separately recorded','models_rerun':0,'source_result_sha256':sha(B/'root/decision-analysis.md'),'acceptance_sha256':sha(B/'root/root-result-acceptance.json')}
(B/'root/delivery-verification.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'status':report['status'],'checks':len(checks),'links':len(links),'failed':[x for x in checks if not x['passed']]},ensure_ascii=False))
raise SystemExit(0 if report['status']=='passed' else 1)
