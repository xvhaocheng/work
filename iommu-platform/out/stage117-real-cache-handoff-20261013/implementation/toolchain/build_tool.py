import pathlib,subprocess,time,json,resource,hashlib
h=pathlib.Path(__file__).resolve().parent;s=h/'verilator-5.006';log=h/'tool-build.log';steps=[]
with log.open('w') as f:
 for args in [['autoconf'],['./configure','--prefix='+str(h/'installed')],['make','-C','src','-j','12','opt']]:
  before=resource.getrusage(resource.RUSAGE_CHILDREN);start=time.monotonic();p=subprocess.run(args,cwd=s,stdout=f,stderr=subprocess.STDOUT);after=resource.getrusage(resource.RUSAGE_CHILDREN)
  steps.append({'argv':args,'returncode':p.returncode,'wall_seconds':time.monotonic()-start,'user_seconds':after.ru_utime-before.ru_utime,'system_seconds':after.ru_stime-before.ru_stime,'peak_child_rss_kib':after.ru_maxrss})
  (h/'tool-build-ledger.json').write_text(json.dumps({'official_url':'https://github.com/verilator/verilator/archive/refs/tags/v5.006.tar.gz','source_sha256':hashlib.sha256((h/'verilator-v5.006.tar.gz').read_bytes()).hexdigest(),'steps':steps},indent=2)+'\n')
  print(json.dumps(steps[-1]),flush=True)
  if p.returncode:raise SystemExit(p.returncode)
