#!/usr/bin/env python3
"""Independent audit of frozen JSON, serialized bus trace and actual DDR snapshots."""
import collections,csv,hashlib,json,pathlib
D=pathlib.Path(__file__).resolve().parent
F=json.loads((D.parent/'common/fixture.json').read_text())
R=json.loads((D/'results/run.json').read_text())
rows=list(csv.DictReader((D/'results/events.csv').open()))
for r in rows:
 for k in ('cycle','address','length_or_strobe','id'):r[k]=int(r[k])
counts=collections.Counter(r['event'] for r in rows)
assert all(a['cycle']<=b['cycle'] for a,b in zip(rows,rows[1:]))
for e in ['issue','front_accepted','backend_accepted','done','next_returned','done_register_matched']:assert counts[e]==4,e
next_ids=[r['id'] for r in rows if r['event']=='next_returned']
done_ids=[r['id'] for r in rows if r['event']=='done_register_matched']
assert next_ids==done_ids==[2,3,4,5]
assert counts['payload_aw']==counts['payload_b']==R['payload_aw']==6
assert counts['payload_ar']==R['payload_ar']==6
assert sum(r['length_or_strobe'] for r in rows if r['event']=='payload_ar')==counts['payload_r']*8==R['payload_read_bus_bytes']==1672
assert sum(r['length_or_strobe'].bit_count() for r in rows if r['event']=='payload_w')==R['payload_write_strobe_bytes']==1639
initial=bytearray([F['initial_destination_fill']]*F['memory_bytes'])
initial[:F['source_region'][1]]=bytes([F['initial_source_fill']])*F['source_region'][1]
expected=initial.copy();dest_payload=set();src_useful=set()
for p in F['packets']:
 frame=bytes.fromhex(p['frame_hex']);assert len(frame)==p['length']
 cur=0
 for s in p['source']:
  initial[s['offset']:s['offset']+s['length']]=frame[cur:cur+s['length']]
  src_useful.update(range(s['offset'],s['offset']+s['length']));cur+=s['length']
 assert cur==len(frame)
expected=initial.copy()
# Traverse original posted chains; no executor spans used in this checker.
for p in F['packets']:
 frame=bytes.fromhex(p['frame_hex']);cursor=0;id=p['cookie_id'];first=True
 while cursor<len(frame):
  d=next(x for x in p['destination'] if x['id']==id)
  skip=F['header_bytes'] if first else 0;n=min(len(frame)-cursor,d['capacity']-skip)
  expected[d['offset']+skip:d['offset']+skip+n]=frame[cursor:cursor+n]
  dest_payload.update(range(d['offset']+skip,d['offset']+skip+n))
  cursor+=n;id=d['next'];first=False
before=(D/'results/before_publish.bin').read_bytes();assert before==expected,'whole prepublication image differs'
for p in F['packets']:
 d=next(x for x in p['destination'] if x['id']==p['cookie_id'])
 expected[d['offset']:d['offset']+F['header_bytes']]=bytes.fromhex(F['header_hex'])
for i in range(F['source_region'][1]):expected[i]=(i*37+9)&255
after=(D/'results/consumed.bin').read_bytes();assert after==expected,'whole postconsume image differs'
written=collections.Counter();read=set();PA=R['mapping']['physical']
for r in rows:
 if r['event']=='payload_w':
  for lane in range(8):
   if r['length_or_strobe']&(1<<lane):written[r['address']+lane-PA]+=1
 if r['event']=='payload_r':read.update(range(r['address']-PA,r['address']-PA+8))
assert set(written)==dest_payload and set(written.values())=={1}
assert src_useful<=read
quiet=next(r['cycle'] for r in rows if r['event']=='global_quiet')
publish=next(r['cycle'] for r in rows if r['event']=='used_publish')
consume=next(r['cycle'] for r in rows if r['event']=='consumer_completed')
release=next(r['cycle'] for r in rows if r['event']=='B_release_and_mapping_teardown')
assert max(r['cycle'] for r in rows if r['event']=='done_register_matched')<quiet<=publish<=consume<release
report={'pass':True,'independent_input':'common/fixture.json, actual events.csv and before_publish/consumed DDR images','whole_images_checked_bytes':len(before)+len(after),'exact_unique_payload_write_addresses':len(written),'covered_unique_useful_source_addresses':len(src_useful),'source_reuse_checked_bytes':F['source_region'][1],'next_ids':next_ids,'matched_done_ids':done_ids,'event_counts':dict(counts),'first_issue_cycle':next(r['cycle'] for r in rows if r['event']=='issue'),'last_hardware_done_cycle':max(r['cycle'] for r in rows if r['event']=='done'),'lifecycle':{'quiet':quiet,'publish':publish,'consume':consume,'release':release},'sha256':{p:hashlib.sha256((D/p).read_bytes()).hexdigest() for p in ['results/run.json','results/events.csv','results/before_publish.bin','results/consumed.bin']}}
(D/'results/artifact-verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
