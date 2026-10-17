#!/usr/bin/env python3
"""Separate actual certificate/donor unit test, not fullwrap completion proof."""
import argparse
import datetime as dt
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

HERE=Path(__file__).resolve().parent


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cva6',required=True,type=Path)
    p.add_argument('--verilator',required=True)
    p.add_argument('--output',required=True,type=Path)
    a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    src=out/'source';src.mkdir()
    shutil.copyfile(a.cva6/'corev_apu/iDMA/src/idma_fullbuffer_certificate.sv',src/'certificate.candidate.sv')
    donor=HERE/'source/certificate.donor.sv';shutil.copyfile(donor,src/'certificate.donor.sv')
    (src/'certificate.gold.sv').write_text(donor.read_text().replace('idma_fullbuffer_certificate','idma_fullbuffer_certificate_gold'))
    for n in ('certificate_top.sv','certificate_test.cpp','run_certificate.py'):shutil.copyfile(HERE/n,src/n)
    mf={'schema':'pn-certificate-unit-v1','status':'running','fixture_only':True,'evidence_level':'finite_certificate_unit',
        'fullwrap_cases':0,'board_points':0,'started_utc':dt.datetime.now(dt.timezone.utc).isoformat(),
        'inputs':{str(x.relative_to(out)):sha(x) for x in sorted(src.iterdir())},'commands':[],
        'scope':'Synthetic job-fire/retire unit inputs test actual donor state boundary; no backend completion is forced.'}
    manifest=out/'manifest.json'
    def save():manifest.write_text(json.dumps(mf,indent=2)+'\n')
    try:
        commands=[('build',[a.verilator,'--cc','--exe','--build','--assert','-j','2','-Wno-fatal',
                           '--top-module','certificate_top','--Mdir',str(out/'obj'),
                           *map(str,[src/'certificate.candidate.sv',src/'certificate.gold.sv',src/'certificate_top.sv',src/'certificate_test.cpp'])]),
                  ('run',[str(out/'obj/Vcertificate_top')])]
        for label,command in commands:
            entry={'argv':command,'log':label+'.log'};mf['commands'].append(entry);save()
            with (out/entry['log']).open('w') as log:r=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,timeout=180)
            entry.update(exit_code=r.returncode,sha256=sha(out/entry['log']));save()
            if r.returncode:raise RuntimeError(label+' failed')
        mf.update(status='passed',result=json.loads((out/'run.log').read_text()))
    except Exception as error:mf.update(status='failed',error=repr(error))
    mf['finished_utc']=dt.datetime.now(dt.timezone.utc).isoformat();save()
    print(json.dumps({'status':mf['status'],'manifest':str(manifest),'result':mf.get('result')}))
    return 0 if mf['status']=='passed' else 1


if __name__=='__main__':raise SystemExit(main())
