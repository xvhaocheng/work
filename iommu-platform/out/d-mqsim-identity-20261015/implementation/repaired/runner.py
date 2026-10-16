#!/usr/bin/env python3
"""Four exact existing cases, once each; root review/execution only, no retries."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,shutil,subprocess,time,xml.etree.ElementTree as ET
HERE=Path(__file__).resolve().parent
RUN_ROOT=HERE/'runs'
STUDY=HERE.parents[1]
OLD=STUDY.parent/'d-persistent-source-20261015/offline'
CONTRACT=STUDY/'root/repair-contract.json'
LENGTHS=(512,2048,4096,8192)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def utc():return datetime.now(timezone.utc).isoformat()
def write(p,obj):p.write_text(json.dumps(obj,indent=2)+'\n')
def main():
 frozen=json.loads((HERE/'pre-build-manifest.json').read_text())
 assert sha(CONTRACT)==frozen['contracts'][str(CONTRACT)]
 assert sha(HERE/'source/MQSim')==json.loads((HERE/'build5-status.json').read_text())['binary_sha256']
 for name,h in frozen['new_source'].items():assert sha(HERE/'source'/name)==h,name
 RUN_ROOT.mkdir(exist_ok=False)
 aggregate={'started_utc':utc(),'actual_simulation_runs':0,'results':[],'automatic_retry':False}
 for n in LENGTHS:
  run=RUN_ROOT/str(n);run.mkdir()
  for name in ('ssdconfig.xml','workload.xml'):
   original=OLD/'runs'/str(n)/'instrumented'/name
   assert sha(original)==frozen['inputs'][str(original)]
   shutil.copyfile(original,run/name)
   assert sha(original)==sha(run/name)
  trace=Path(ET.parse(run/'workload.xml').find('.//File_Path').text)
  assert trace.resolve()==(OLD/'inputs'/('write-'+str(n)+'.trace')).resolve()
  assert sha(trace)==frozen['inputs'][str(trace)]
  files=[p for p in (HERE/'source').rglob('*') if p.is_file() and 'build' not in p.relative_to(HERE/'source').parts]
  files += [CONTRACT,STUDY/'root/identity-contract.json',STUDY/'design/repair-contract.md',HERE/'runner.py',HERE/'repair.patch',HERE/'pre-build-manifest.json',run/'ssdconfig.xml',run/'workload.xml',trace]
  cmd=[str(HERE/'source/MQSim'),'-i','ssdconfig.xml','-w','workload.xml']
  manifest={'utc':utc(),'input_n':n,'cwd':str(run),'command':cmd,'stdin':'DEVNULL','timeout_s':60,'hashes':{str(p):sha(p) for p in sorted(files)}}
  write(run/'pre-run-manifest.json',manifest)
  result={'input_n':n,'started_utc':utc(),'command':cmd,'cwd':str(run),'timeout_s':60,'stdin':'DEVNULL'}
  start=time.monotonic();aggregate['actual_simulation_runs']+=1
  with (run/'stdout.log').open('wb') as out,(run/'stderr.log').open('wb') as err:
   try:
    proc=subprocess.run(cmd,cwd=run,stdin=subprocess.DEVNULL,stdout=out,stderr=err,timeout=60)
    result.update(returncode=proc.returncode,timed_out=False)
   except subprocess.TimeoutExpired:result.update(returncode=None,timed_out=True)
   except OSError as exc:result.update(returncode=None,timed_out=False,process_error=str(exc))
  result.update(wall_s=time.monotonic()-start,ended_utc=utc(),xml_outputs={p.name:sha(p) for p in run.glob('*.xml')})
  write(run/'result.json',result);aggregate['results'].append(result)
  failed=result['returncode']!=0 or result['timed_out']
  aggregate['stopped_on_process_failure']=failed;write(RUN_ROOT/'status.json',aggregate)
  if failed:break
 aggregate['ended_utc']=utc();write(RUN_ROOT/'status.json',aggregate)
if __name__=='__main__':main()
