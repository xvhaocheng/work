#!/usr/bin/env python3
"""Produce bibliography-only records; do not change the shared references library."""
from pathlib import Path
import csv
import datetime
import hashlib
import json

ROOT = Path('/home/xvhaocheng/work')
OUT = Path(__file__).resolve().parent
catalog = json.loads((ROOT/'references/catalog.json').read_text())['records']
by_id = {r['id']: r for r in catalog}
selected = ['riommu2015', 'socdma2018', 'iotlb_bottleneck2010', 'zero_day_iommu2023', 'mask2018']
records = []
headers = ['# 已有原文的题录页眉', '', '只提取题名/作者页眉；不形成技术总结或人类已读标记。', '']
for item in selected:
    if item == 'zero_day_iommu2023':
        records.append({
            'id': item,
            'title': 'Open-source RISC-V Input/Output Memory Management Unit (IOMMU) IP',
            'authors': ['Manuel Rodríguez', 'Francisco Costa', 'Bruno Sá', 'Sandro Pinto'],
            'venue': 'RISC-V Summit Europe 2023, Barcelona, Spain (extended abstract / poster)',
            'year': 2023,
            'pages': '1–2 (extended abstract)',
            'doi': None,
            'doi_link': None,
            'doi_status': 'not found in the checked primary sources; no DOI invented',
            'identification_status': 'primary-source match to the user description; no separate full journal/conference design-and-evaluation paper identified',
            'author_source': 'official two-page extended abstract, page 1',
            'poster_author_variant': ['Manuel Rodríguez', 'Francisco Marques', 'Bruno Sá', 'Sandro Pinto'],
            'author_variant_note': 'The official poster uses Francisco Marques while the official extended abstract uses Francisco Costa; preserved separately without assuming identity or silently merging names.',
            'local_references_original_present': False,
            'local_search_scope': 'references/catalog.json, references/papers/README.md, and text records beneath references; no matching indexed original',
            'local_originals': [],
            'acquisition_links': [
                'https://riscv-europe.org/summit/2023/media/proceedings/posters/2023-06-07-Manuel-RODR%C3%8DGUEZ-abstract.pdf',
                'https://riscv-europe.org/summit/2023/media/proceedings/posters/2023-06-07-Manuel-RODR%C3%8DGUEZ-poster.pdf'],
            'primary_sources': [
                'https://riscv-europe.org/summit/2023/media/proceedings/posters/2023-06-07-Manuel-RODR%C3%8DGUEZ-abstract.pdf',
                'https://riscv-europe.org/summit/2023/media/proceedings/posters/2023-06-07-Manuel-RODR%C3%8DGUEZ-poster.pdf',
                'https://algoritmi.uminho.pt/user/sandro-emanuel-salgado-pinto/'],
            'downloaded_missing_paper': False,
        })
        continue
    c = by_id[item]
    r = {k: c[k] for k in ['id', 'title', 'authors', 'venue', 'year']}
    r['local_references_original_present'] = True
    r['local_originals'] = []
    for original in c['originals']:
        idx = ROOT/'references'/original['index_path']
        actual = idx.resolve(strict=True)
        data = actual.read_bytes()
        actual_hash = hashlib.sha256(data).hexdigest()
        r['local_originals'].append({
            'references_index_path': str(idx.relative_to(ROOT)),
            'index_is_symlink': idx.is_symlink(),
            'canonical_path': str(actual.relative_to(ROOT)),
            'sha256': actual_hash,
            'bytes': len(data),
            'matches_catalog_sha256': actual_hash == original['sha256'],
        })
    if item in ['riommu2015', 'socdma2018', 'mask2018']:
        raw = json.loads((OUT/'raw'/f'{item}.crossref.json').read_text())['message']
        title = raw['title'][0]
        if raw.get('subtitle'):
            title += ': ' + raw['subtitle'][0]
        authors = [(a.get('given','')+' '+a['family']).strip() for a in raw['author']]
        r.update(title=title, authors=authors, venue=raw['container-title'][0],
                 year=raw['published']['date-parts'][0][0], doi=raw['DOI'],
                 doi_link='https://doi.org/'+raw['DOI'], pages=raw['page'],
                 metadata_source=f'raw/{item}.crossref.json')
    if item == 'riommu2015':
        r['venue_short'] = 'ASPLOS 2015 (20th), pp. 355–368'
        r['primary_sources'] = ['https://api.crossref.org/works/10.1145/2694344.2694355',
                               'https://dants.github.io/papers/riommu-asplos-2015.pdf']
        r['acquisition_links'] = ['https://dants.github.io/papers/riommu-asplos-2015.pdf']
    elif item == 'socdma2018':
        r['venue_short'] = 'ICCD 2018 (36th), pp. 292–300'
        r['primary_sources'] = ['https://api.crossref.org/works/10.1109/ICCD.2018.00052',
                               'https://arxiv.org/abs/1808.09751',
                               'https://iris.unimore.it/handle/11380/1179001?mode=complete']
        r['acquisition_links'] = ['https://arxiv.org/pdf/1808.09751']
        r['local_version_note'] = 'Existing PDF is the arXiv author manuscript (v1), not asserted to be the IEEE typeset version.'
    elif item == 'mask2018':
        r['venue_short'] = 'ASPLOS 2018 (23rd), pp. 503–518'
        r['primary_sources'] = ['https://api.crossref.org/works/10.1145/3173162.3173169',
                               'https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf']
        r['acquisition_links'] = [r['primary_sources'][1]]
    elif item == 'iotlb_bottleneck2010':
        r['venue_short'] = 'WIOSCA 2010 (6th workshop); author manuscript, HAL inria-00493752 v1'
        r['doi'] = None
        r['doi_link'] = None
        r['doi_status'] = 'The author page links the later revised chapter DOI; do not silently label the stored 2010 manuscript as the Springer edition.'
        r['primary_sources'] = ['https://nadav.amit.zone/publications/workshops/2010-iommu.html',
                               'https://nadav.amit.zone/publications/pdfs/amit2010iommu.pdf',
                               'https://link.springer.com/chapter/10.1007/978-3-642-24322-6_22',
                               'https://citation-needed.springer.com/v2/references/10.1007/978-3-642-24322-6_22?flavour=citation&format=bibtex']
        r['acquisition_links'] = [r['primary_sources'][1]]
        r['local_version_note'] = 'PDF page 1 states WIOSCA 2010 author manuscript; HAL v1 date 21 June 2010.'
        r['related_revised_edition'] = {
            'title': r['title'], 'authors': r['authors'],
            'venue': 'Computer Architecture: ISCA 2010 International Workshops A4MMC, AMAS-BT, EAMA, WEED, WIOSCA, Revised Selected Papers',
            'series': 'Lecture Notes in Computer Science, vol. 6161',
            'pages': '256–274',
            'doi': '10.1007/978-3-642-24322-6_22',
            'doi_link': 'https://doi.org/10.1007/978-3-642-24322-6_22',
            'year_from_publisher_bibtex': 2012,
            'year_from_publisher_visible_cite_this_paper': 2011,
            'book_print_published': '2011-10-28', 'book_ebook_published': '2012-02-15',
            'year_note': 'Keep WIOSCA 2010, Springer print/citation 2011, and publisher BibTeX/eBook 2012 distinct. Official export saved in raw/iotlb-revised.springer.bib.'}
    text = ROOT/c['originals'][0]['path'].replace('.pdf','.txt')
    if text.exists():
        lines = text.read_text().splitlines()
        n = 10 if item == 'iotlb_bottleneck2010' else 8
        r['existing_text_header_source'] = {'path': str(text.relative_to(ROOT)), 'lines': [1,n]}
        headers += [f'## {item}', '', f'原始文本 `{text.relative_to(ROOT)}`：', '', '```text']
        headers += [f'{i}: {lines[i-1]}' for i in range(1,n+1)]
        headers += ['```', '']
    records.append(r)

output = {'checked_at_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
          'scope': 'bibliographic metadata, local original existence/hash, and missing-original acquisition URLs only; no technical summaries or human-read attestations',
          'records': records}
(OUT/'metadata.json').write_text(json.dumps(output, ensure_ascii=False, indent=2)+'\n')
(OUT/'local-headers.md').write_text('\n'.join(headers)+'\n')
with (OUT/'metadata.csv').open('w', newline='') as f:
    w = csv.DictWriter(f, fieldnames=['id','authors','title','venue','year','doi_link','related_revised_doi_link','references_original_present','references_index','canonical_path','sha256','acquisition_links','notes'])
    w.writeheader()
    for r in records:
        loc = r['local_originals'][0] if r['local_originals'] else {}
        w.writerow({'id':r['id'], 'authors':'; '.join(r['authors']), 'title':r['title'], 'venue':r['venue'],
                    'year':r['year'], 'doi_link':r.get('doi_link',''),
                    'related_revised_doi_link':r.get('related_revised_edition',{}).get('doi_link',''),
                    'references_original_present':r['local_references_original_present'],
                    'references_index':loc.get('references_index_path',''), 'canonical_path':loc.get('canonical_path',''),
                    'sha256':loc.get('sha256',''), 'acquisition_links':'; '.join(r['acquisition_links']),
                    'notes':r.get('author_variant_note',r.get('local_version_note','')) + (' ' + r['related_revised_edition']['year_note'] if 'related_revised_edition' in r else '')})
print(json.dumps({'papers':len(records), 'local_indexed_pdfs':sum(r['local_references_original_present'] for r in records),
                  'all_existing_hashes_match':all(o['matches_catalog_sha256'] for r in records for o in r['local_originals']),
                  'missing_pdfs_downloaded':0}))
