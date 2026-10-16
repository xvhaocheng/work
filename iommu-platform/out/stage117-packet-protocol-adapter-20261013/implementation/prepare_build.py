#!/usr/bin/env python3
"""One new isolated build, retaining original RTL/latency inputs exactly."""
import difflib,hashlib,json,pathlib,subprocess,time
HERE=pathlib.Path(__file__).resolve().parent
ROOT=HERE.parents[2]
OLD=ROOT/'out/stage117-plan-revision-g0-20261013/model'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
for directory in ('raw','results','obj'):(HERE/directory).mkdir(exist_ok=True)
frozen=json.loads((OLD/'build-inputs.json').read_text())
for ent in frozen['files']:
    p=ROOT/ent['path']
    assert p.exists() and sha(p)==ent['sha256'],f'Frozen RTL input changed: {p}'
assert sha(HERE/'nonblock_actual_dma_system_top.sv')==sha(OLD/'nonblock_actual_dma_system_top.sv')
assert sha(HERE/'finite_hooks.inc')==sha(OLD/'finite_hooks.inc')
assert sha(ROOT/'out/stage117-three-results-20261013/common/fixture.json')=='97763f1ae630a22d7231a9862967dea5df01e19741dbdcfd59c58111edc97da7'
args=[a.replace(str(OLD),str(HERE)).replace('/finite_dma.cpp','/packet_dma.cpp') for a in frozen['argv']]
inputs=[]
for ent in frozen['files']:
    p=ROOT/ent['path']
    if p==OLD/'nonblock_actual_dma_system_top.sv':p=HERE/p.name
    inputs.append(p)
inputs += [HERE/n for n in ('packet_dma.cpp','packet_protocol.hpp','inherited_harness.hpp','finite_hooks.inc','prepare_build.py')]
inputs += list((ROOT/'out/stage117-three-results-20261013/common').glob('*.hpp'))
inputs += [ROOT/'out/stage117-three-results-20261013/common/fixture.json',HERE.parent/'implementation-contract.json']
manifest={'scope':'protocol function only; unchanged production RTL and ordinary configuration','argv':args,'prior_manifest_sha256':sha(OLD/'build-inputs.json'),'contract_sha256':sha(HERE.parent/'implementation-contract.json'),'all_frozen_RTL_inputs_match':True,'files':[{'path':str(p.relative_to(ROOT)),'sha256':sha(p)} for p in inputs]}
(HERE/'build-inputs.json').write_text(json.dumps(manifest,indent=2)+'\n')
(HERE/'harness-observer.diff').write_text(''.join(difflib.unified_diff((OLD/'inherited_harness.hpp').read_text().splitlines(True),(HERE/'inherited_harness.hpp').read_text().splitlines(True),fromfile='baseline/inherited_harness.hpp',tofile='implementation/inherited_harness.hpp')))
t=time.monotonic()
with (HERE/'raw/build.log').open('w') as f:p=subprocess.run(args,stdout=f,stderr=subprocess.STDOUT)
result={'returncode':p.returncode,'elapsed_wall_seconds':time.monotonic()-t,'argv_manifest':'build-inputs.json'}
(HERE/'results/build.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
raise SystemExit(p.returncode)
