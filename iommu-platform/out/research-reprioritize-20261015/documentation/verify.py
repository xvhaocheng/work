from pathlib import Path
import hashlib,json,re
W=Path('/home/xvhaocheng/work');D=W/'iommu-platform/out/research-reprioritize-20261015/documentation';R=D.parent
sha=lambda b:hashlib.sha256(b).hexdigest()
r=json.loads((D/'receipt.json').read_text());m=json.loads((D/'before-write-manifest.json').read_text())
for n,v in m.items():
    b=(D/'before-write'/n).read_bytes();assert len(b)==v['bytes'] and sha(b)==v['sha256'],n
for n,v in r['after'].items():
    b=(W/n).read_bytes();assert len(b)==v['bytes'] and sha(b)==v['sha256'],n
def span(b,s,e=None):
    i=b.index(s.encode());return b[i:b.index(e.encode(),i)] if e else b[i:]
for n,s,e in [('CURRENT_STATUS.md','## 各分支最新进展','## 硬件与执行状态'),('CURRENT_STATUS.md','## 独立工程事项',None),('研究计划.md','## I：',None),('研究导航.md','## 已有成果怎样复用',None)]:
    assert span((W/n).read_bytes(),s,e)==span((D/'before-write'/n).read_bytes(),s,e),n
for n in ['HANDOFF.md','iommu-platform/docs/validation.md']:
    assert (W/n).read_bytes().endswith((D/'before-write'/n).read_bytes()),n
for n in ['AGENTS.md','iommu-platform/AGENTS.md']:
    assert (W/n).read_bytes()==(D/'before-write'/n).read_bytes(),n
for n,h in r['stable_root_input_hashes'].items():assert sha((R/'root'/n).read_bytes())==h,n
for item in r['new_local_links']:
    t=re.sub(r':\d+(?::\d+)?$','',item['target']);p=Path(t) if t.startswith('/') else (W/item['file']).parent/t
    assert p.exists(),str(p)
b=(R/'README.md').read_bytes();r['final_README']['bytes']=len(b);r['final_README']['sha256']=sha(b)
r['checks']['final_saved_bytes_hashes_protection_and_links']=True
r['check_count']=len(r['checks']);r['passed']=all(r['checks'].values())
(D/'receipt.json').write_text(json.dumps(r,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'passed':r['passed'],'checks':r['check_count'],'new_links':r['new_local_link_count'],'preexisting_broken':r['preexisting_broken_count']}))
