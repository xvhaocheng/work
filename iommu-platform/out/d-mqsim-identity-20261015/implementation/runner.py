#!/usr/bin/env python3
"""Root-reviewed invocation only. One frozen 2048 replay; never called by implementer."""
from pathlib import Path
import hashlib,json,shutil,subprocess,time,xml.etree.ElementTree as ET
HERE=Path(__file__).resolve().parent
RUN=HERE/'run-2048'
OLD=HERE.parents[1]/'d-persistent-source-20261015/offline'
CONTRACT=HERE.parent/'root/identity-contract.json'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 RUN.mkdir(exist_ok=False)
 for name in ('ssdconfig.xml','workload.xml'):
  shutil.copyfile(OLD/'runs/2048/instrumented'/name,RUN/name)
 trace=Path(ET.parse(RUN/'workload.xml').find('.//File_Path').text)
 assert trace.resolve()==(OLD/'inputs/write-2048.trace').resolve()
 files=sorted(p for p in (HERE/'source').rglob('*') if p.is_file() and 'build' not in p.relative_to(HERE/'source').parts)
 files += [CONTRACT,HERE.parent/'root/identity-contract.sha256',HERE/'runner.py',RUN/'ssdconfig.xml',RUN/'workload.xml',trace]
 expected=(HERE.parent/'root/identity-contract.sha256').read_text().split()[0]
 assert digest(CONTRACT)==expected
 manifest={str(p):digest(p) for p in files}
 (RUN/'pre-run-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
 start=time.monotonic();result={'actual_simulation_runs':1,'timeout_s':60,'stdin':'DEVNULL','input_n':2048}
 with (RUN/'stdout.log').open('wb') as out,(RUN/'stderr.log').open('wb') as err:
  try:
   p=subprocess.run([str(HERE/'source/MQSim'),'-i','ssdconfig.xml','-w','workload.xml'],cwd=RUN,stdin=subprocess.DEVNULL,stdout=out,stderr=err,timeout=60)
   result['returncode']=p.returncode;result['timed_out']=False
  except subprocess.TimeoutExpired:
   result['returncode']=None;result['timed_out']=True
 result['wall_s']=time.monotonic()-start
 result['xml_outputs']={p.name:digest(p) for p in RUN.glob('*.xml')}
 (RUN/'status.json').write_text(json.dumps(result,indent=2)+'\n')
if __name__=='__main__':main()
