#!/usr/bin/env python3
"""Validate library structure and local provenance; no network, experiments or builds."""
from pathlib import Path
import collections,json,hashlib,re,os
ROOT=Path(__file__).resolve().parents[1];REF=ROOT/'references'
a=json.loads((REF/'catalog.json').read_text());rs=a['records'];errors=[]
ids=[r['id'] for r in rs];counts=collections.Counter(ids)
errors += ['duplicate id '+k for k,v in counts.items() if v!=1]
legacy=json.loads((ROOT/'iommu-platform/out/shared-translation-literature-20261006/catalog.json').read_text());missing=[x['id'] for x in legacy if x['id'] not in ids]
errors += ['missing legacy record '+x for x in missing]
cache={};original_hashes={};citations=0
for r in rs:
 for f in ['id','title','authors','year','type','topic','reading_level','citations','problem','mechanism','supported_boundary','limitations','summary_path']:
  if f not in r:errors.append(r['id']+': missing '+f)
 if not (REF/r['summary_path']).is_file():errors.append('missing summary '+r['id'])
 if not r['citations']:errors.append('missing citations '+r['id'])
 if not (r['source_urls'] or r['originals']):errors.append('missing source '+r['id'])
 for c in r['citations']:
  citations+=1;p=ROOT/c['file']
  if not p.is_file():errors.append('missing citation file '+str(p));continue
  if c['file'] not in cache:cache[c['file']]=len(p.read_text(errors='replace').splitlines())
  if not 1<=c['line']<=max(cache[c['file']],1):errors.append('citation line out of range '+str(c))
 for o in r['originals']:
  p=ROOT/o['path'];q=REF/o['index_path']
  if not q.is_symlink() or not q.exists() or q.resolve()!=p.resolve():errors.append('broken original link '+str(q));continue
  actual=hashlib.sha256(p.read_bytes()).hexdigest();original_hashes[o['path']]=actual
  if actual!=o['sha256']:errors.append('original hash mismatch '+str(p))
# Validate local links emitted by this task, including all generated cards and indexes.
local_links=0
for p in REF.rglob('*.md'):
 for m in re.finditer(r'\[[^\]]*\]\(([^)]+)\)',p.read_text()):
  target=m.group(1)
  if target.startswith(('http:','https:','mailto:','#')):continue
  target=target.split('#',1)[0]
  if not target:continue
  local_links+=1
  if not (p.parent/target).exists():errors.append('broken markdown link '+str(p.relative_to(ROOT))+' -> '+target)
inv=json.loads((REF/'url-inventory.json').read_text())
for x in inv['items']:
 if x['disposition']=='catalogued' and x['reference_id'] not in ids:errors.append('inventory points to missing id '+x['reference_id'])
# Check baseline hashes recorded before this consolidation, not just the newly generated digest.
legacy_hash_checks=0
for r in legacy:
 for v in r['versions']:
  if not v.get('pdf') or not v.get('pdf_sha256'):continue
  p=ROOT/'iommu-platform/out/shared-translation-literature-20261006'/v['pdf']
  if p.exists():
   legacy_hash_checks+=1
   if hashlib.sha256(p.read_bytes()).hexdigest()!=v['pdf_sha256']:errors.append('legacy original changed '+str(p))
report={'status':'passed' if not errors else 'failed','scope':'Library structure, source locations, local Markdown links, symlinks, and canonical PDF hashes only; no scientific conclusions verified by this checker','date':'2026-10-16','references':len(rs),'legacy_123_covered':len(legacy)-len(missing),'unique_ids':len(set(ids)),'summaries':len(list((REF/'summaries').glob('*.md')))-1,'source_citations_checked':citations,'distinct_citation_files_checked':len(cache),'local_markdown_links_checked':local_links,'pdf_links_checked':sum(len(r['originals']) for r in rs),'unique_original_paths_hashed':len(original_hashes),'legacy_pdf_hashes_unchanged':legacy_hash_checks,'types':dict(collections.Counter(r['type'] for r in rs)),'reading_levels':dict(collections.Counter(r['reading_level'] for r in rs)),'incomplete_bibliography_records':sum(r['bibliography_status']!='complete_from_project_record' for r in rs),'url_inventory':inv['canonical_url_count'],'inventory_catalogued':sum(x['disposition']=='catalogued' for x in inv['items']),'inventory_navigation_operations':sum(x['disposition']!='catalogued' for x in inv['items']),'errors':errors}
(REF/'verification.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(report,ensure_ascii=False,indent=2));raise SystemExit(bool(errors))
