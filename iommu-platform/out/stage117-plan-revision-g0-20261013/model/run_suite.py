#!/usr/bin/env python3
import hashlib,json,pathlib,subprocess,time
D=pathlib.Path(__file__).resolve().parent
contract=json.loads((D.parent/'causal/comparison-contract.json').read_text())
binary=D/'obj/Vnonblock_actual_dma_system_top'
ledger=[]
for case in contract['cases']:
    name=case['id'];out=D/'results'/name;out.mkdir(exist_ok=True)
    assert not (out/'run.json').exists(),'do not silently rerun an unchanged completed case'
    argv=[str(binary),name,str(out)];start=time.monotonic()
    with (D/f'raw/{name}.log').open('w') as f:proc=subprocess.run(argv,stdout=f,stderr=subprocess.STDOUT)
    receipt={'case':name,'argv':argv,'returncode':proc.returncode,'process_wall_seconds':time.monotonic()-start,'binary_sha256':hashlib.sha256(binary.read_bytes()).hexdigest()}
    ledger.append(receipt);(D/'results/execution.json').write_text(json.dumps(ledger,indent=2)+'\n')
    print(json.dumps(receipt),flush=True)
    if proc.returncode:raise SystemExit(proc.returncode)
