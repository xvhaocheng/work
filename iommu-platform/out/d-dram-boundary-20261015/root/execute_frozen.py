"""Execute only the nine predeclared source tests, never old experiments."""
import datetime,hashlib,json,pathlib,subprocess,time
BASE=pathlib.Path(__file__).resolve().parents[1]
IMPL=BASE/'implementation'
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def dump(p,d): p.write_text(json.dumps(d,ensure_ascii=False,indent=2)+'\n')
def main():
 contract=BASE/'root/dram-contract.json'
 expected=(BASE/'root/dram-contract.sha256').read_text().split()[0]
 assert sha(contract)==expected, 'frozen contract changed'
 c=json.loads(contract.read_text())
 original=IMPL/'ramulator2-be93be78055d922aa1d4d33e15bcc8f2b0c61a9d'
 private=IMPL/'instrumented-source'
 checked={}
 for manifest,root in [('source-prebuild.json',original),('instrumented-prebuild.json',private)]:
  raw=json.loads((IMPL/'receipts'/manifest).read_text())
  for rel,want in raw['files'].items():
   p=root/rel
   assert sha(p)==want, ('prebuild source mismatch',str(p))
   checked[str(p)]=want
 cmd=json.loads((IMPL/'receipts/final-driver-commands.json').read_text())
 bins={k:pathlib.Path(a[a.index('-o')+1]) for k,a in cmd.items()}
 extras=[contract,BASE/'root/pre-run-predictions.json',BASE/'root/verification-criteria.json',BASE/'root/source-review.json',IMPL/'boundary-driver.cpp',IMPL/'boundary-config.yaml',IMPL/'passive-audit.patch',original/'libramulator.so',private/'libramulator.so',*bins.values(),pathlib.Path(__file__).resolve()]
 for p in extras: checked[str(p)]=sha(p)
 frozen={'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'contract_sha256':expected,'files':checked,'count':len(checked),'request_model':'root session unchanged, no in-place switch','actual_model':'unknown','actual_effort':'unknown'}
 dump(BASE/'root/run-inputs.json',frozen)
 runs=BASE/'runs'
 runs.mkdir(exist_ok=False)
 plan=[(x['id'],x['id'],bins['instrumented']) for x in c['cases']]
 plan.append(('original_history_cap_warm','history_cap_warm',bins['baseline']))
 receipts=[]
 for out_id,case_id,binary in plan:
  # Inputs must stay fixed throughout this bounded batch; no silent recompile.
  for p,want in checked.items(): assert sha(pathlib.Path(p))==want, ('changed between cases',p)
  out=runs/out_id;out.mkdir()
  argv=[str(binary),str(contract),str(IMPL/'boundary-config.yaml'),case_id,str(out)]
  started=datetime.datetime.now(datetime.timezone.utc).isoformat();t=time.monotonic()
  try:
   result=subprocess.run(argv,cwd=str(IMPL),stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=30)
   code=result.returncode;stdout=result.stdout;stderr=result.stderr;timed_out=False
  except subprocess.TimeoutExpired as exc:
   code=None;stdout=exc.stdout or b'';stderr=exc.stderr or b'';timed_out=True
  (out/'stdout.log').write_bytes(stdout);(out/'stderr.log').write_bytes(stderr)
  rec={'output_id':out_id,'case_id':case_id,'argv':argv,'cwd':str(IMPL),'start_utc':started,'elapsed_seconds':time.monotonic()-t,'exit_code':code,'timeout':timed_out,'input_manifest_sha256':sha(BASE/'root/run-inputs.json'),'outputs':{str(p.relative_to(out)):sha(p) for p in sorted(out.rglob('*')) if p.is_file()}}
  dump(out/'execution.json',rec);receipts.append(rec)
  dump(BASE/'root/execution-ledger.json',{'receipts':receipts,'planned':9,'completed':len(receipts),'passed_processes':sum(r['exit_code']==0 for r in receipts),'scope':'fixed source boundary tests only'})
  print(out_id,code,round(rec['elapsed_seconds'],3),flush=True)
  if code!=0: raise SystemExit('Case failed; stopped batch without retry or input changes')
if __name__=='__main__': main()
