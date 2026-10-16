#!/usr/bin/env python3
"""Execute exactly one frozen local-RTL case, preserve log and timeout state."""
import argparse,hashlib,json,subprocess,time
from pathlib import Path

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--binary',type=Path,required=True)
    ap.add_argument('--policy',type=int,required=True);ap.add_argument('--history',choices=['cold','hot'],required=True)
    ap.add_argument('--out',type=Path,required=True);a=ap.parse_args()
    r=Path(__file__).resolve().parents[1];out=a.out.resolve();exe=a.binary.resolve()
    if not out.is_relative_to(r) or out.exists():ap.error('fresh output within this task required')
    out.mkdir(parents=True)
    argv=[str(exe),'+history='+a.history,'+policy='+str(a.policy),'+event-trace='+str(out/'events.csv')]
    receipt={'argv':argv,'executable_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),
             'started_unix':time.time(),'timeout_seconds':60,'state':'running',
             'evidence_type':'real source RTL with reused behavioral DDR, not MIG/native application'}
    rp=out/'execution.json';rp.write_text(json.dumps(receipt,indent=2)+'\n')
    with (out/'run.log').open('w') as f:
        p=subprocess.Popen(argv,stdout=f,stderr=subprocess.STDOUT);receipt['pid']=p.pid
        rp.write_text(json.dumps(receipt,indent=2)+'\n')
        try:ret=p.wait(timeout=60)
        except subprocess.TimeoutExpired:
            p.kill();p.wait();ret=124;receipt['state']='timeout'
    log=(out/'run.log').read_text();result=None
    for line in log.splitlines():
        if line.startswith('FIRST-PTE-PAIRED-JSON '):result=json.loads(line.split(' ',1)[1])
    passed=ret==0 and result is not None and 'FIRST-PTE-PAIRED-PASS' in log
    receipt.update(exit_code=ret,ended_unix=time.time(),state='passed' if passed else receipt.get('state') if ret==124 else 'failed')
    rp.write_text(json.dumps(receipt,indent=2)+'\n')
    if result is not None:(out/'result.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'out':str(out),'passed':passed,'result':result},ensure_ascii=False))
    if not passed:print(log[-5000:])
    return 0 if passed else 1

if __name__=='__main__':raise SystemExit(main())
