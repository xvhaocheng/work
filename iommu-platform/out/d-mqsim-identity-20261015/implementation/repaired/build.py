from pathlib import Path
from datetime import datetime,timezone
import subprocess,time,json,hashlib
p=Path(__file__).resolve().parent;s=p/'source';t=time.monotonic()
result={'sequence':5,'trigger':'new root-approved release-only semantic fork plus three passive evidence hooks','started_utc':datetime.now(timezone.utc).isoformat(),'command':['make','-j2'],'cwd':str(s),'timeout_s':180,'actual_simulation_runs':0}
with (p/'build5-stdout.log').open('wb') as o,(p/'build5-stderr.log').open('wb') as e:
 clean=subprocess.run(['make','clean'],cwd=s,stdout=o,stderr=e,timeout=20)
 try:
  b=subprocess.run(['make','-j2'],cwd=s,stdout=o,stderr=e,timeout=180);result.update(clean_rc=clean.returncode,build_rc=b.returncode,timed_out=False)
 except subprocess.TimeoutExpired:result.update(clean_rc=clean.returncode,build_rc=None,timed_out=True)
result.update(wall_s=time.monotonic()-t,ended_utc=datetime.now(timezone.utc).isoformat())
if (s/'MQSim').exists():result['binary_sha256']=hashlib.sha256((s/'MQSim').read_bytes()).hexdigest()
(p/'build5-status.json').write_text(json.dumps(result,indent=2)+'\n');print(result)
