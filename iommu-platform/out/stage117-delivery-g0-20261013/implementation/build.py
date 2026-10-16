#!/usr/bin/env python3
import pathlib,json,hashlib,subprocess,time,resource
H=pathlib.Path(__file__).resolve().parent;R=H.parents[2]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
argv=json.loads((H/'build-argv.json').read_text())
inputs=[pathlib.Path(a) for a in argv if pathlib.Path(a).is_file()]
inputs += [H/n for n in ['cpu_cache.hpp','cache_hooks.inc','inherited_harness.hpp','finite_hooks.inc','delivery_protocol.hpp','build.py','build-argv.json','interface-plan.json']]
inputs += [R/'out/stage117-three-results-20261013/common'/n for n in ['fixture.hpp','fixture_data.hpp','fixture.json']]
(H/'build-inputs.json').write_text(json.dumps({'scope':'new deterministic CPU read1/store2 frontend plus unchanged actual WT/controller/DMA/IOMMU RTL','argv':argv,'files':[{'path':str(p.relative_to(R)),'sha256':sha(p)} for p in inputs]},indent=2)+'\n')
attempt=1+len(list((H/'raw').glob('build-*.log')));before=resource.getrusage(resource.RUSAGE_CHILDREN);start=time.monotonic()
with (H/f'raw/build-{attempt}.log').open('w') as f:p=subprocess.run(argv,stdout=f,stderr=subprocess.STDOUT)
after=resource.getrusage(resource.RUSAGE_CHILDREN);x={'attempt':attempt,'returncode':p.returncode,'elapsed_wall_seconds':time.monotonic()-start,'user_cpu_seconds':after.ru_utime-before.ru_utime,'system_cpu_seconds':after.ru_stime-before.ru_stime,'peak_child_rss_kib':after.ru_maxrss,'log':f'raw/build-{attempt}.log','argv_manifest':'build-inputs.json'}
ledger=H/'build-ledger.json';old=json.loads(ledger.read_text()) if ledger.exists() else [];old.append(x);ledger.write_text(json.dumps(old,indent=2)+'\n');print(json.dumps(x),flush=True);raise SystemExit(p.returncode)
