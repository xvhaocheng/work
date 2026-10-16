from pathlib import Path
import json, re, hashlib
W=Path('/home/xvhaocheng/work')
D=W/'iommu-platform/out/d-boundary-source-20261015/documentation'
r=json.loads((D/'receipt.json').read_text())
m=json.loads((D/'before-manifest.json').read_text())
sha=lambda b:hashlib.sha256(b).hexdigest()
for n,v in m.items():
    b=(D/'before'/n).read_bytes()
    assert len(b)==v['bytes'] and sha(b)==v['sha256'], n
def resolved(file,target):
    target=re.sub(r':\d+(?::\d+)?$','',target)
    return Path(target) if target.startswith('/') else (W/file).parent/target
root_check={'file':'iommu-platform/out/d-boundary-source-20261015/README.md','target':'root/source-and-protection-verification.json'}
if not any(x['file']==root_check['file'] and x['target']==root_check['target'] for x in r['new_local_links']):
    r['new_local_links'].append(root_check)
for item in r['new_local_links']:
    item['exists']=resolved(item['file'],item['target']).exists()
r['preexisting_line_links_valid_after_resolving_line_suffix']=r.get('preexisting_line_links_valid_after_resolving_line_suffix',[])+[x for x in r['preexisting_broken_local_links'] if resolved(x['file'],x['target']).exists()]
r['preexisting_broken_local_links']=[x for x in r['preexisting_broken_local_links'] if not resolved(x['file'],x['target']).exists()]
r['preexisting_broken_count']=len(r['preexisting_broken_local_links'])
r['new_local_link_count']=len(r['new_local_links'])
r['checks']['all_new_local_links_exist']=all(x['exists'] for x in r['new_local_links'])
r['checks']['before_manifest_bytes_and_sha256']=True
r['checks']['all_final_target_bytes_and_hashes']=all(len((W/n).read_bytes())==v['bytes'] and sha((W/n).read_bytes())==v['sha256'] for n,v in r['after'].items())
def span(b,s,e=None):
    start=b.index(s.encode());return b[start:b.index(e.encode(),start)] if e else b[start:]
for n,s,e in [('CURRENT_STATUS.md','## 各分支最新进展','## 硬件与执行状态'),('CURRENT_STATUS.md','## 独立工程事项',None),('研究计划.md','## I：',None),('研究导航.md','## 已有成果怎样复用',None)]:
    assert span((W/n).read_bytes(),s,e)==span((D/'before'/n).read_bytes(),s,e)
for n in ['HANDOFF.md','iommu-platform/docs/validation.md']:
    assert (W/n).read_bytes().endswith((D/'before'/n).read_bytes())
for n in ['AGENTS.md','iommu-platform/AGENTS.md']:
    assert (W/n).read_bytes()==(D/'before'/n).read_bytes()
r['checks']['final_protected_blocks_suffixes_and_agents']=True
r['link_check_method']='Unique (document,path target) pairs; fragment stripped and trailing :line[:column] normalized for filesystem existence. New means target absent from the same document before; all README links are new. Repeated occurrences are not counted separately. Historical bytes are unchanged.'
readme=(D.parent/'README.md').read_bytes()
r['final_readme']={'path':str((D.parent/'README.md').relative_to(W)),'bytes':len(readme),'sha256':sha(readme)}
r['check_count']=len(r['checks']);r['passed']=all(r['checks'].values())
assert r['passed']
(D/'receipt.json').write_text(json.dumps(r,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'passed':r['passed'],'checks':r['check_count'],'new_links':r['new_local_link_count'],'preexisting_broken':r['preexisting_broken_count'],'line_links_valid':len(r['preexisting_line_links_valid_after_resolving_line_suffix'])}))
