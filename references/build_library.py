#!/usr/bin/env python3
"""Rebuild the source-bound root index; reads selected project prose, never source trees."""
from pathlib import Path
from urllib.parse import urlsplit,urlunsplit,parse_qsl,urlencode,unquote
import re,json,os,hashlib,collections
ROOT=Path(__file__).resolve().parents[1]
REF=ROOT/'references'
BASE=ROOT/'iommu-platform/out/shared-translation-literature-20261006'
PRUNE={'source','sources','linux','llvm-project','build','builds','before','before-write','before-at-write','documentation','originals','papers','web','node_modules','.git','patches','backup','snapshots','repair-1','delivery','bsp','cva6','opensbi'}
def canonical(u):
 u=u.strip().rstrip('.,;；。`\\');u=unquote(u.replace('&amp;','&'))
 p=urlsplit(u);host=p.netloc.lower().removeprefix('www.');path=p.path.rstrip('/')
 if host in ['doi.org','dx.doi.org']: return 'doi:'+path.lstrip('/').lower()
 if host in ['arxiv.org','export.arxiv.org']:
  m=re.search(r'/(?:abs|html|pdf)/(\d{4}\.\d{4,5})(?:v\d+)?',path)
  if m:return 'arxiv:'+m.group(1)
 if host in ['kernel.org','docs.kernel.org']:
  path=re.sub('^/doc/html/(latest|v?[0-9.]+)','',path)
  return 'https://docs.kernel.org'+path
 query=urlencode([(k,v) for k,v in parse_qsl(p.query) if not k.startswith('utm_') and k not in ['ref','source']])
 return urlunsplit(('https',host,path,query,''))
def files():
 out=[]
 for p in ROOT.glob('*.md'):
  if p.name!='AGENTS.md':out.append(p)
 out.extend((ROOT/'iommu-platform/docs').rglob('*.md'))
 out.extend((ROOT/'docs').rglob('*.md'))
 for folder,dirs,fs in os.walk(ROOT/'iommu-platform/out'):
  rel=Path(folder).relative_to(ROOT/'iommu-platform/out')
  dirs[:]=[d for d in dirs if d not in PRUNE and not d.startswith(('before','pre-','llvm-project','linux-'))]
  if len(rel.parts)>=4:dirs[:]=[]
  for f in fs:
   if f.endswith('.md') and not any(s in f.lower() for s in ['raw-response','deepseek-answer','deepseek-revised','source-excerpts','critical-source-excerpts']):out.append(Path(folder)/f)
 return sorted(set(out))
def scan():
 items=collections.defaultdict(list); paths=files()
 for p in paths:
  if p.stat().st_size>1500000: continue
  lines=p.read_text(errors='replace').splitlines()
  for no,line in enumerate(lines,1):
   if not ('http://' in line or 'https://' in line):continue
   for m in re.finditer(r'https?://[^\s<>"\]）`，；。【】“”]+',line):
    u=m.group().split(')')[0].rstrip('`*,;；。|')
    if len(u)<12 or u.endswith('doi.org/') or u.endswith('http'):continue
    labels=[x.group(1) for x in re.finditer(r'\[([^\]]+)\]\((https?://[^\s)]+)\)',line) if x.group(2)==u]
    # Avoid retaining entire long table rows for each link.
    ctx=line.strip()
    items[canonical(u)].append({'file':str(p.relative_to(ROOT)),'line':no,'url':u,'label':labels[0] if labels else None,'context':ctx[:2400]})
 return items,paths

# Generation deliberately preserves old evidence levels: possession of a PDF is not reading it.
def generate():
 inv,paths=scan(); records=[]; alias={};pdfs=[]; manifests=[]
 rootmeta=json.loads((BASE/'catalog.json').read_text())
 def linefor(path,needle):
  if not path.exists():return 1
  return next((i for i,l in enumerate(path.read_text(errors='replace').splitlines(),1) if needle in l),1)
 def addalias(key,rid):
  if key:alias[key]=rid
 def aslist(v):return v if isinstance(v,list) else ([v] if v else [])
 def localref(path,**extra):return {'file':str(path.relative_to(ROOT)),'line':1,**extra}
 for a in rootmeta:
  v=next(x for x in a['versions'] if x['id']==a['preferred_version_id']);card=v.get('worker_card') or {};orig=[]
  for ver in a['versions']:
   if ver.get('pdf') and (BASE/ver['pdf']).exists():
    orig.append({'path':str((BASE/ver['pdf']).relative_to(ROOT)),'version_id':ver['id'],'sha256':ver.get('pdf_sha256')})
   if ver.get('source_url'):addalias(canonical(ver['source_url']),a['id'])
   if ver.get('doi'):addalias('doi:'+ver['doi'].lower(),a['id'])
  if a.get('doi'):addalias('doi:'+a['doi'].lower(),a['id'])
  grade='local_partial' if a.get('root_spotchecked') else ('worker_fulltext_record' if a.get('reading_status')=='fulltext_worker_reviewed' else 'metadata_or_mention')
  source={'file':str((BASE/'catalog.json').relative_to(ROOT)),'line':linefor(BASE/'catalog.json','"id": "'+a['id']+'"'),'scope':'既有123项专题；保留原reading_status和root抽核标记'}
  ev=[]
  for e in (v.get('worker_evidence') or card.get('evidence') or []):
   p=ROOT/e.get('file','');loc=e.get('location','');m=re.search(r'L(\d+)',loc)
   if p.is_file():ev.append({'file':str(p.relative_to(ROOT)),'line':int(m.group(1)) if m else 1,'location':loc,'claim':e.get('claim')})
  records.append({'id':a['id'],'title':a['title'],'authors':a.get('authors') or 'unknown','year':a.get('year') or 'unknown','type':'preprint' if a.get('publication_status')=='preprint' else 'research_paper','venue':a.get('venue') or 'unknown','doi':a.get('doi'),'topic':a.get('topic','translation'),'source_urls':list(dict.fromkeys(x.get('source_url') for x in a['versions'] if x.get('source_url'))),'originals':orig,'reading_level':grade,'legacy_reading_status':a.get('reading_status'),'legacy_root_spotchecked':a.get('root_spotchecked',False),'publication_status_at_record':a.get('publication_status'),'citations':[source]+ev,'problem':card.get('problem') or 'unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。','mechanism':card.get('mechanism') or a.get('notes') or '未取得足够正文，机制级结论保持未核。','assumptions':aslist(card.get('assumptions')),'limitations':aslist(card.get('limitations'))+aslist(a.get('metadata_note')),'supported_boundary': '原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。' if a.get('fulltext_available') else '仅题录/摘要/索引片段，不得用于全文机制覆盖或收益裁决。','direction_relation':aslist(card.get('relations')) or [f"{a.get('topic','translation')}主题近邻；历史收录不代表当前主线采纳。"],'legacy_notes':a.get('notes'),'versions':[{'id':x['id'],'source_url':x.get('source_url'),'reading_status':x.get('reading_status'),'publication_status':x.get('publication_status')} for x in a['versions']]})
 # Cohesive supplemental records are manually source-bound, and remain partial rather than inflated to full reading.
 for a in json.loads((REF/'supplemental-records.json').read_text()):
  r={k:v for k,v in a.items() if k!='aliases'}
  r.update(authors=a.get('authors','unknown'),year=a.get('year','unknown'),type=a.get('type','research_paper'),venue='unknown',source_urls=[],originals=[],reading_level='local_partial',citations=[],problem='见项目核查记录中明确的问题和适用合同。',supported_boundary='仅支持已列核读章节/公开补丁；没有本项目复现证据。',limitations=[a.get('limitation','本轮仅整理既有引用，完整方法和结果未独立重审。')],direction_relation=[f"{a.get('topic','translation')}近邻；用于限定对应版本的主张，不自动选择论文主线。"])
  for u in a['aliases']:addalias(canonical(u) if u.startswith('http') else u,r['id'])
  records.append(r)
 byid={a['id']:a for a in records}
 # Add aliases for the same public manuscript hosted at more than one institution.
 manual={'damn-asplos-2018.pdf':'damn2018','asplos2018-damn.pdf':'damn2018','riommu-asplos-2015.pdf':'riommu2015','iotlb-peerj23.pdf':'iotlb_wall2023','mask-asplos18.pdf':'mask2018','gpu_page_walk_scheduler_isca_18.pdf':'walk_scheduling2018','sshin-gcox-isca18.pdf':'walk_scheduling2018','bpichai-asplos14.pdf':'pichai2014','binhpham-hpca14.pdf':'gpummu2014','binhpham-micro12.pdf':'colt2012','micro23_2.pdf':'idyll2023','asplos2016-iommu.pdf':'true_iommu2016','vattention_arxiv24.pdf':'vattention2025','2018.kpart.hpca.pdf':'kpart2018'}
 # Known exact institutional landing pages / alternate publisher records.
 for k,rid in {'https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted':'sharemmu2026','https://ieeexplore.ieee.org/document/11153110':'bermuda2025','https://ieeexplore.ieee.org/document/10247943':'orchestrated2023'}.items():addalias(k,rid)
 for a in records:
  for u in a['source_urls']:
   if '.pdf' in urlsplit(u).path:manual.setdefault(Path(urlsplit(u).path).name.lower(),a['id'])

 extra_aliases={
 'doi:10.1109/dac56929.2023.10247943':'orchestrated2023',
 'doi:10.1145/10.1145/2541940.2541942':'pichai2014',
 'https://usenix.org/conference/osdi24/presentation/xiang':'nomad2024',
 'arxiv:2401.13154':'nomad2024',
 'https://usenix.org/conference/nsdi25/presentation/wang-zixuan':'odrp2025',
 'https://collaborate.princeton.edu/en/publications/shared-last-level-tlbs-for-chip-multiprocessors':'slltlb2011',
 'https://pure.skku.edu/en/publications/a-case-for-speculative-address-translation-with-rapid-validation-':'avatar2024',
 'https://sandia.gov/research/publications/details/deact-architecture-aware-virtual-memory-support-for-fabric-attached-memory-2021-02-01':'deact2021',
 'https://skku-compasslab.github.io/compasslab/publications/papers/250715-micro-clap':'clap2025',
 'https://skku-compasslab.github.io/compasslab/publications/250715-micro-softwalker':'softwalker2025',
 'https://skku-compasslab.github.io/compasslab/publications/260710-micro-cato':'cato2026',
 'https://maxkev1n.github.io/publications/sigmetrics-2026':'cuptw2026',
 'https://nadav.amit.zone/publications/workshops/2010-iommu.html':'iotlb_bottleneck2010',
 'https://usenix.org/system/files/conference/osdi16/osdi16-kwon.pdf':'ingens2016'
 }
 for k,v in extra_aliases.items():addalias(k,v)
 manual.update({'18asplos_mask.pdf':'mask2018','micro2012-colt.pdf':'colt2012','hpca2023.pdf':'transfw2023','midgard-rebooting-virtual-memory-1.pdf':'midgard2021','mosaic-application-transparent-multiple-page-sizes-for-gpus_micro17.pdf':'mosaic2017'})

 # Legacy broad map adds data movement, KV memory, cache partitioning and official interfaces.
 manifest=ROOT/'iommu-platform/out/research-landscape-20261005/source-manifest.json'
 for a in json.loads(manifest.read_text()):
  u=a.get('url');k=canonical(u) if u else None;rid=alias.get(k)
  if rid is None and a['id'] in byid:rid=a['id']
  if rid is None and a['id']=='vattention2024':rid='vattention2025'
  if rid is None:
   rid=a['id'];r={'id':rid,'title':'unknown','display_label':a.get('title_hint') or rid,'authors':'unknown','year':'unknown','type':'technical_document' if a.get('html') else 'research_paper','venue':a.get('venue_hint') or 'unknown','topic':'data-lifecycle','source_urls':[],'originals':[],'reading_level':'local_partial','citations':[],'problem':'既有广域地图的逐篇证据卡，问题范围以该记录为准。','mechanism':'复用既有报告，见下面source_context；未根据短题名补写机制。','supported_boundary':'既有已存全文与逐篇笔记；本轮不重审评价。','limitations':['未复现论文实验；仅存全文不等于全文精读。'],'direction_relation':['数据搬运、资源组织与动态内存近邻，历史收录不代表当前采用。']};records.append(r);byid[rid]=r
  addalias(k,rid);r=byid[rid]
  if u and u not in r['source_urls']:r['source_urls'].append(u)
  if a.get('pdf'):
   p=manifest.parent/a['pdf']
   if p.exists() and not r['originals']:r['originals'].append({'path':str(p.relative_to(ROOT)),'version_id':a['id']})
  r['citations'].append({'file':str(manifest.relative_to(ROOT)),'line':linefor(manifest,'"id": "'+a['id']+'"'),'scope':a.get('status')})
 # Inventory local originals without descending source/build or historical duplicate trees.
 prune_pdf=PRUNE-{'papers','originals'}
 for folder,dirs,fs in os.walk(ROOT/'iommu-platform/out'):
  rel=Path(folder).relative_to(ROOT/'iommu-platform/out')
  dirs[:]=[d for d in dirs if d not in prune_pdf and not d.startswith(('before','pre-','llvm-project','linux-'))]
  if len(rel.parts)>=4:dirs[:]=[]
  for f in fs:
   if f.endswith('.pdf'):pdfs.append(Path(folder)/f)
 pdfby=collections.defaultdict(list)
 for p in pdfs:pdfby[p.name.lower()].append(p)
 generic_labels={'原文','来源','PDF','全文','作者全文','作者 PDF','正式 DOI','DOI','论文','作者原文','入口','作者页面','作者主页','来源链接','官方文档','原论文','官方 PDF','paper','source','HTML','回应','原讨论','固定源码'}
 def label(cs):
  xs=list(dict.fromkeys(c['label'].strip('*` ') for c in cs if c.get('label') and c['label'].strip('*` ') not in generic_labels))
  return max(xs,key=len) if xs else None
 def is_project(c):return '/out/' not in c['file'] or not any(s in c['file'] for s in ['replacement','restoration','snapshot','archive'])
 def typ(k):
  if '/patent/' in k or 'patentimages.' in k or 'patents.justia.' in k:return 'patent','patents'
  if any(z in k.lower() for z in ['ihi0022','ihi0089','vt-directed-io-spec','intel-data-streaming-accelerator-spec','48882_iommu','sprufl1c','sprugs5b','openmp-examples','genesys2_rm']):return 'technical_document','standards-software'
  if k.startswith('doi:10.5281/zenodo.') or 'zenodo.org/records/' in k or 'zenodo.org/api/records/' in k:return 'artifact_metadata','software-artifacts'
  if 'rfc-editor.org/rfc/' in k:return 'rfc','dma-dataflow'
  if k.startswith(('arxiv:','doi:')) or '.pdf' in k or '/purl/' in k:return 'research_paper','supplementary-research'
  if any(s in k for s in ['standards.ieee.org/','man7.org/','amrex-codes.github.io/','openocd.org/doc/','developers.openai.com/','docs.kernel.org','docs.riscv.org','documentation-service.arm.com','docs.amd.com','docs.nvidia.com','pcisig.com','openmp.org/spec','developer.arm.com/documentation','support.arm.com/documentation','docs.xiangshan.cc','pulp-platform.github.io','lmax-exchange.github.io']):return 'technical_document','standards-software'
  if any(s in k for s in ['lists.openwall.net/','lkml.','lists.infradead.org/','lore.kernel.org/']):return 'rfc_patch','mapping-lifecycle'
  if any(s in k for s in ['/en/publications/','/entities/publication/','/conference/','/publication/','/publications/','hal.science/','d-nb.info/','research-collection.ethz.ch/','drops.dagstuhl.de/entities/document/','easychair.org/publications/','sciencedirect.com/science/article/','onlinelibrary.wiley.com/doi/']):return 'research_record','supplementary-research'
  return None,None
 # Treat fixed-version code as technical material; grouping is per repository + explicit commit.
 def codegroup(k):
  p=urlsplit(k)
  if p.netloc=='raw.githubusercontent.com':
   parts=p.path.strip('/').split('/');return '/'.join(parts[:3]) if len(parts)>=3 else None
  if p.netloc=='github.com':
   parts=p.path.strip('/').split('/')
   if len(parts)<2:return None
   if len(parts)>2 and parts[2] in ['pull','issues','compare','actions','releases','discussions']:return None
   if len(parts)>3 and parts[2] in ['blob','tree','commit']:return '/'.join(parts[:2]+[parts[3]])
   return '/'.join(parts[:2])
  return None
 dispositions=[]
 for k,cs in sorted(inv.items()):
  rid=alias.get(k);bn=Path(urlsplit(k).path).name.lower()
  if rid is None and bn in manual and manual[bn] in byid:rid=manual[bn]
  if rid is None:
   tp,topic=typ(k);cg=codegroup(k)
   # Avoid tool/dependency installation links being counted as research documents.
   if cg and any(is_project(c) and ('/docs/' in c['file'] or c.get('label')) for c in cs) and not any(s in k.lower() for s in ['vivado-on-silicon','spdlog','yaml-cpp','argparse','/style-guide','/contributing','/verilator/verilator','/axboe/fio','/xvhaocheng/']):tp,topic='source_code','software-artifacts'
   if tp is None:
    dispositions.append({'key':k,'disposition':'navigation_or_operations_not_reference','citations':cs});continue
   rid=('code-'+re.sub('[^a-zA-Z0-9]+','-',cg).strip('-')[:100]) if tp=='source_code' else ('arxiv-'+k[6:] if k.startswith('arxiv:') else 'ref-'+hashlib.sha256(k.encode()).hexdigest()[:12])
   if rid not in byid:
    lb=label(cs)
    r={'id':rid,'title':'unknown','display_label':lb or bn or k,'authors':'unknown','year':'unknown','type':tp,'venue':'unknown','topic':topic,'source_urls':[],'originals':[],'reading_level':'metadata_or_mention','citations':[],'problem':'项目记录中用于何种问题，见逐条来源上下文。','mechanism':'本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。','supported_boundary':'项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。','limitations':['作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。'],'direction_relation':[f'{topic}资料；是否当前采用须回到总计划和本轮接受记录。']}
    if tp=='source_code':r['display_label']=cg;r['supported_boundary']='公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。'
    records.append(r);byid[rid]=r
  r=byid[rid];addalias(k,rid)
  if bn.endswith('.pdf') and len(bn)>15:manual.setdefault(bn,rid)
  for c in cs:
   if c['url'] not in r['source_urls']:r['source_urls'].append(c['url'])
  # Keep every location in machine catalog; summaries select the most useful project narrative citations.
  r['citations'].extend({'file':c['file'],'line':c['line'],'label':c.get('label'),'context':c['context']} for c in cs)
  if not r['originals'] and bn in pdfby:
   candidates=sorted(pdfby[bn],key=lambda p:('shared-translation-literature' not in str(p),len(str(p))))
   r['originals'].append({'path':str(candidates[0].relative_to(ROOT)),'version_id':'local_snapshot_identity_not_reverified'})
  dispositions.append({'key':k,'disposition':'catalogued','reference_id':rid,'citations':cs})

 # Targeted, dated updates never rewrite a legacy reading claim into a full-paper review.
 curated=json.loads((REF/'curated-updates.json').read_text())
 for rid,fields in curated['records'].items():
  if rid in byid:
   byid[rid].update(fields)
   for c in fields.get('root_read_evidence',[]):
    byid[rid]['citations'].append(dict(c))
 for rid,p in curated['local_originals'].items():
  if rid in byid and (ROOT/p).is_file() and not byid[rid]['originals']:
   byid[rid]['originals'].append({'path':p,'version_id':'existing_project_original'})
 # Recover already-written bibliographic and method cards from the 2026-10-05 broad map.
 for fn in ['L3_data_lifecycle.md','L2_sharing.md','L4_migration_observability_recovered.md']:
  p=manifest.parent/'workers'/fn
  if not p.exists():continue
  txt=p.read_text()
  starts=list(re.finditer(r'^### (?:卡|Card) [^\n]+',txt,re.M))
  for i,m in enumerate(starts):
   card=txt[m.start():starts[i+1].start() if i+1<len(starts) else txt.find('\n## ',m.end()) if txt.find('\n## ',m.end())!=-1 else len(txt)]
   for rid in ['dae1982','ucp2006','kpart2018','gps2021','dx1002025','vtensor2024','gmlake2024','minisa2026','tiara2026','wic2025','nds2021','pagedattention2023']:
    if rid not in byid or rid+'.txt' not in card:continue
    r=byid[rid]
    title=re.search(r'\*\*(?:题录|题录/状态/URL)\*\*：\*([^*]+)\*',card)
    if title:r['title']=title.group(1)
    for field,pat in [('problem',r'\*\*(?:问题|针对问题)\*\*：([^\n]+)'),('mechanism',r'\*\*(?:机制|核心机制)\*\*：([^\n]+)')]:
     z=re.search(pat,card)
     if z:r[field]=z.group(1)
    z=re.search(r'\*\*(?:不可外推|不能推出的结论)\*\*：([^\n]+)',card)
    if z:r['limitations'].append(z.group(1))
    r['citations'].append({'file':str(p.relative_to(ROOT)),'line':txt.count('\n',0,m.start())+1,'scope':'既有逐篇证据卡；本轮仅重组'})
    r['year']=next((int(z) for z in re.findall(r'20\d{2}|19\d{2}',rid)),r['year'])
 # Exact confirmed mirrors and versions merge into one record, retaining all citations and originals.
 for old,new in curated['merges'].items():
  if old not in byid or new not in byid or old==new:continue
  a,b=byid[old],byid[new]
  for f in ['source_urls','citations']:
   b[f].extend(a[f])
  if not b['originals']:b['originals']=a['originals']
  b.setdefault('merged_record_ids',[]).append(old)
  records.remove(a);del byid[old]
  for d in dispositions:
   if d.get('reference_id')==old:d['reference_id']=new

 if 'orchestrated2023' in byid:
  r=byid['orchestrated2023'];bad=[c for c in r['citations'] if c.get('line')==1788 and c['file'].endswith('papers/orchestrated2023.txt')]
  r['legacy_rejected_evidence']=[{**c,'reason':'Later root correction a-cards.md:314: current stored text has only 993 lines; old HPCA2024 inference not applicable to this hash'} for c in bad]
  r['citations']=[c for c in r['citations'] if c not in bad]
  r['limitations']=[z for z in r['limitations'] if 'HPCA 2024' not in z and '本地作者稿引文出现2024' not in z]
  r['limitations'].append(r['metadata_correction'])
 # Root model guidance is methodological documentation, separate from domain research papers.
 for r in records:
  if any('developers.openai.com/' in u for u in r['source_urls']):r['type']='technical_document';r['topic']='workflow-tools'

 # Stable context selection prioritizes substantive local records over indexes and long reference tables.
 for r in records:
  seen=set();r['citations']=[c for c in r['citations'] if not ((c['file'],c['line']) in seen or seen.add((c['file'],c['line'])))]
  contexts=[c for c in r['citations'] if c.get('context')]
  contexts.sort(key=lambda c:(not ('/docs/' in c['file'] and '/out/' not in c['file']),'/INDEX' in c['file'],len(c['context'])<100,-len(c['context'])))
  r['source_context']=[{'file':c['file'],'line':c['line'],'text':c['context']} for c in contexts[:4]]
  if not r['citations']:raise ValueError('missing evidence '+r['id'])
  if not r.get('source_urls') and not r.get('originals'):raise ValueError('missing original '+r['id'])
  r['summary_path']='summaries/'+r['id']+'.md'
  for o in r['originals']:
   p=ROOT/o['path'];actual=hashlib.sha256(p.read_bytes()).hexdigest()
   if o.get('sha256') and o['sha256']!=actual:raise ValueError('original digest differs '+str(p))
   o['sha256']=actual
   link=REF/'papers'/(r['id']+('-'+o['version_id'] if len(r['originals'])>1 else '')+'.pdf')
   target=os.path.relpath(p,link.parent)
   if link.is_symlink():
    if os.readlink(link)!=target:link.unlink();link.symlink_to(target)
   elif not link.exists():link.symlink_to(target)
   else:raise ValueError('will not overwrite existing original '+str(link))
   o['index_path']=str(link.relative_to(REF))
 # Render independent short cards. Context quotes are project notes, not copied paper passages.
 levels={'local_partial':'局部已核（保留原有核读范围）','worker_fulltext_record':'既有worker全文提取；非root逐篇精读','metadata_or_mention':'仅题录/摘要/提及或核读范围待整理'}
 def clean_context(t):return re.sub(r'\[([^]]+)\]\((?!https?://)[^)]+\)',r'\1',t)
 def mklink(p,line=None):return '../../'+p+('#L'+str(line) if line else '')
 for r in records:
  r['bibliography_status']='complete_from_project_record' if r['title']!='unknown' and r['authors']!='unknown' and r['year']!='unknown' else 'incomplete_metadata_explicit_unknown'
  title=r['title'] if r['title']!='unknown' else r.get('display_label','unknown')+'〔正式题名待整理〕'
  text=[f'# {title}',f"稳定ID：`{r['id']}` · 类型：`{r['type']}` · 主题：`{r['topic']}`",f"作者：{'; '.join(r['authors']) if isinstance(r['authors'],list) else r['authors']}；年份：{r['year']}；出处：{r.get('venue','unknown')}。",f"阅读等级：**{levels[r['reading_level']]}**。本轮是资料归档，不新增人读标记或复现实验结论。",f"问题：{r['problem']}",f"机制／已存认识：{r.get('mechanism','unknown')}",f"支持边界：{r['supported_boundary']}"]
  if r.get('assumptions'):text.append('条件：'+'；'.join(map(str,r['assumptions'])))
  text.append('限制：'+'；'.join(map(str,r['limitations'])) if r['limitations'] else '限制：原记录未单独列限制；不能据此认为没有限制。')
  rels=[]
  for rel in aslist(r.get('direction_relation')):
   if isinstance(rel,dict):
    target=rel.get('target_id') or rel.get('target') or '相邻工作'
    note=rel.get('note') or rel.get('relation') or rel.get('description') or rel.get('type') or '见原证据卡的关系记录'
    rels.append(('['+str(target)+']('+str(target)+'.md)' if target in byid else str(target))+'：'+str(note))
   else:rels.append(str(rel))
  text.append('与研究关系：'+(r.get('project_relation') or '本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。'))
  if rels:text.append('相邻资料：'+'；'.join(rels))
  if r.get('root_read_evidence'):text.append('后续定向核读（不改写旧目录状态）：\n\n'+'\n'.join(f"- {c['date']}：{c['scope']}，[{c['file']}:{c['line']}]({mklink(c['file'])})" for c in r['root_read_evidence']))
  text.append('原始来源：\n\n'+'\n'.join(f'- [来源{n}]({u})' for n,u in enumerate(r['source_urls'][:8],1)))
  if r['originals']:text.append('本地原件（相对symlink，未复制论文）：\n\n'+'\n'.join(f"- [PDF](../{o['index_path']}) · canonical：[原路径]({mklink(o['path'])}) · SHA-256 `{o['sha256']}`" for o in r['originals']))
  text.append('项目核查／引用入口（精确行保存在catalog；链接打开原文件）：\n\n'+'\n'.join(f"- [{c['file']}:{c['line']}]({mklink(c['file'])})" for c in r['citations'][:8]))
  if r['source_context']:text.append('既有项目记录的上下文（历史判断，不能覆盖最新合同）：\n\n'+'\n\n'.join(f"[{c['file']}:{c['line']}]({mklink(c['file'])})：\n\n{clean_context(c['text'])}" for c in r['source_context']))
  text.append('[返回总目录](../README.md)')
  (REF/r['summary_path']).write_text('\n\n'.join(text)+'\n')
 records.sort(key=lambda r:(r['topic'],str(r['year']),r['id']))
 output={'schema_version':'1.0','updated':'2026-10-16','purpose':'Consolidation of already-used project references; not a new literature search','reading_levels':levels,'records':records}
 known_summaries={r['id']+'.md' for r in records}
 previous_generated=json.loads((REF/'generated-files.json').read_text()).get('files',[]) if (REF/'generated-files.json').exists() else []
 for rel in previous_generated:
  p=REF/rel
  if p.parent==REF/'summaries' and p.name!='README.md' and p.name not in known_summaries and p.is_file():p.unlink()
 known_pdfs={o['index_path'] for r in records for o in r['originals']}
 for rel in previous_generated:
  p=REF/rel
  if p.parent==REF/'papers' and p.is_symlink() and rel not in known_pdfs:p.unlink()
 (REF/'generated-files.json').write_text(json.dumps({'note':'Only listed generated cards and PDF symlinks may be removed on rebuild; unknown/manual files are preserved','files':sorted(['summaries/'+n for n in known_summaries]+list(known_pdfs))},ensure_ascii=False,indent=2)+'\n')
 (REF/'catalog.json').write_text(json.dumps(output,ensure_ascii=False,indent=2)+'\n')
 (REF/'url-inventory.json').write_text(json.dumps({'scope':'project narrative markdown only; explicit source/build/snapshot directory pruning, out max depth 4; no tool HTTP logs','scanned_files':[str(p.relative_to(ROOT)) for p in paths],'canonical_url_count':len(inv),'items':dispositions},ensure_ascii=False,indent=2)+'\n')
 stats={'references':len(records),'by_type':dict(collections.Counter(r['type'] for r in records)),'by_reading_level':dict(collections.Counter(r['reading_level'] for r in records)),'original_pdf_links':sum(len(r['originals']) for r in records),'narrative_files_scanned':len(paths),'url_inventory':len(inv),'catalogued_urls':sum(x['disposition']=='catalogued' for x in dispositions),'unclassified_navigation_or_operations_urls':sum(x['disposition']!='catalogued' for x in dispositions)}
 (REF/'statistics.json').write_text(json.dumps(stats,ensure_ascii=False,indent=2)+'\n')
 from index_template import render
 render(REF,records,stats,levels)
 print(json.dumps(stats,ensure_ascii=False,indent=2))
if __name__=='__main__':generate()
