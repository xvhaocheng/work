#!/usr/bin/env python3
"""Frozen nine-process runner; preparation never invokes it. Root review required."""
import argparse,datetime,hashlib,json,pathlib,subprocess,sys,time
base=pathlib.Path(__file__).resolve().parent
parser=argparse.ArgumentParser();parser.add_argument('--execute-root-reviewed-contract',action='store_true');args=parser.parse_args()
if not args.execute_root_reviewed_contract:parser.error('execution requires explicit root-reviewed-contract flag')
contract=base.parent/'root/dram-contract.json';expected=(base.parent/'root/dram-contract.sha256').read_text().split()[0]
if hashlib.sha256(contract.read_bytes()).hexdigest()!=expected:raise RuntimeError('frozen contract hash mismatch')
plan=json.loads(contract.read_text());runs=[(c['id'],base/'boundary-driver-instrumented','instrumented') for c in plan['cases']]+[('history_cap_warm',base/'boundary-driver-original','original')]
runroot=base.parent/'execution';runroot.mkdir(exist_ok=False)
inputs=[contract,base/'boundary-config.yaml',base/'boundary-driver.cpp',base/'boundary-driver-instrumented',base/'boundary-driver-original',base/'instrumented-source/libramulator.so',base/'ramulator2-be93be78055d922aa1d4d33e15bcc8f2b0c61a9d/libramulator.so']
(runroot/'pre-execution-inputs.json').write_text(json.dumps({str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs},indent=2)+'\n')
receipts=[]
for case,driver,kind in runs:
 out=runroot/(kind+'-'+case);out.mkdir();command=[str(driver),str(contract),str(base/'boundary-config.yaml'),case,str(out)]
 started=datetime.datetime.now(datetime.timezone.utc).isoformat();t=time.monotonic()
 with (out/'stdout.log').open('w') as stdout,(out/'stderr.log').open('w') as stderr:
  try:r=subprocess.run(command,stdout=stdout,stderr=stderr,timeout=30);rc=r.returncode;timedout=False
  except subprocess.TimeoutExpired:rc=124;timedout=True
 receipt={'argv':command,'started_utc':started,'elapsed_seconds':time.monotonic()-t,'exit_code':rc,'timeout_30s':timedout,'files':{str(p.relative_to(out)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(out.rglob('*')) if p.is_file()}}
 (out/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');receipts.append(receipt);(runroot/'run-ledger.json').write_text(json.dumps(receipts,indent=2)+'\n')
 if rc:print('Case failed; stopped without retries:',case,file=sys.stderr);sys.exit(rc)
print('Nine frozen runs completed; acceptance requires root evidence review.')
