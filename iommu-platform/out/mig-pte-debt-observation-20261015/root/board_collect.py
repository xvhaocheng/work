#!/usr/bin/env python3
"""Private post-load prepare/run. Root must accept timing/loaded identity first."""
from pathlib import Path
import argparse, base64, hashlib, io, json, os, re, shlex, sys, tarfile

PACK=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(PACK.parents[1]/'scripts'))
from mac_research2 import SerialConsole

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('stage',choices=['prepare','run'])
    ap.add_argument('--bit-sha',required=True)
    args=ap.parse_args()
    if not re.fullmatch('[0-9a-f]{64}',args.bit_sha):raise SystemExit('invalid BIT SHA')
    if not (PACK/'root/loaded-bit.json').is_file():raise SystemExit('missing actual root load receipt')
    loaded=json.loads((PACK/'root/loaded-bit.json').read_text())
    if loaded.get('sha256')!=args.bit_sha or loaded.get('program_exit_code')!=0:
        raise SystemExit('actual successful loaded-BIT receipt mismatch')
    marker=PACK/'root/workload-attempt-1-registered.json'
    if marker.exists():raise SystemExit('a workload attempt is already registered; no automatic retry')
    c=SerialConsole('/dev/ttyUSB0',PACK/'root/console-collection.raw.log',PACK/'root/console-collection.events.log')
    c.start()
    def command(cmd,timeout=30):
        r=c.run_shell(cmd,timeout)
        return r
    try:
        if args.stage=='prepare':
            q=command('uname -a; uname -r; cat /proc/sys/kernel/random/boot_id; for d in idma0 idma1 idma2 idma3; do readlink -f /sys/class/misc/$d/device; done',30)
            (PACK/'root/postload-board-identity.txt').write_bytes(q.stdout)
            if q.exit_code:raise RuntimeError('board identity command failed')
            tasks='/tmp/mig-observation-tools'
            ops=['mkdir -p '+tasks]
            for name in ['idma_composition','mig_capture_read.rv64','run_one_observation.sh']:
                ops.append('wget -q -O '+tasks+'/'+name+' http://192.168.77.1:8035/'+name)
            ops+=['chmod 700 '+tasks+'/*','sha256sum '+tasks+'/*']
            q=command(' && '.join(ops),60)
            (PACK/'root/postload-tool-fetch.txt').write_bytes(q.stdout)
            if q.exit_code:raise RuntimeError('board tool fetch failed')
            text=q.stdout.decode(errors='replace')
            for f in (PACK/'board-tools').iterdir():
                if f.is_file() and hashlib.sha256(f.read_bytes()).hexdigest() not in text:
                    raise RuntimeError('board tool SHA mismatch: '+f.name)
            q=command(tasks+'/mig_capture_read.rv64 --probe',30)
            (PACK/'root/postload-observer-probe.txt').write_bytes(q.stdout)
            (PACK/'root/postload-observer-probe-exit.json').write_text(json.dumps({'exit_code':q.exit_code})+'\n')
            print(json.dumps({'stage':'prepare','probe_exit':q.exit_code,'workload_attempted':False}))
            if q.exit_code:raise RuntimeError('actual observer preflight invalid; do not run workload')
            return
        identity=json.loads((PACK/'root/identity-accepted.json').read_text())
        for k in ['timing_accepted','bit_identity_verified','reset_to_first_AR_epoch_verified','exact_source_build_verified','source_ID_mapping_verified']:
            if identity.get(k) is not True:raise RuntimeError('identity gate false: '+k)
        if identity.get('loaded_bit_sha256')!=args.bit_sha:raise RuntimeError('identity BIT mismatch')
        # Hard local marker precedes dispatch. Even a broken transport cannot
        # silently repeat the fixed workload in a later continuation.
        fd=os.open(marker,os.O_WRONLY|os.O_CREAT|os.O_EXCL,0o600)
        with os.fdopen(fd,'w') as out:json.dump({'bit_sha256':args.bit_sha,'dispatch_registered':True,'workload_rule':'exactly one, no selection on debt'},out,indent=2)
        tasks='/tmp/mig-observation-tools';run='/tmp/mig-observation-run-20261015'
        cmd='sh '+tasks+'/run_one_observation.sh '+tasks+'/idma_composition '+tasks+'/mig_capture_read.rv64 '+run+' '+args.bit_sha
        q=command(cmd,90)
        (PACK/'root/board-launcher-stdout.txt').write_bytes(q.stdout)
        (PACK/'root/board-launcher-return.json').write_text(json.dumps({'exit_code':q.exit_code})+'\n')
        q=command('tar -czf /tmp/mig-observation-export.tgz -C /tmp mig-observation-run-20261015 && base64 /tmp/mig-observation-export.tgz',60)
        (PACK/'root/board-export.base64').write_bytes(q.stdout)
        if q.exit_code:raise RuntimeError('new raw record export failed')
        data=base64.b64decode(q.stdout)
        if len(data)>5*1024*1024:raise RuntimeError('unexpected export size')
        (PACK/'root/board-export.tgz').write_bytes(data)
        dest=PACK/'board-record';dest.mkdir(exist_ok=False)
        with tarfile.open(fileobj=io.BytesIO(data),mode='r:gz') as arc:
            for m in arc.getmembers():
                parts=Path(m.name).parts
                if not parts or parts[0]!='mig-observation-run-20261015' or Path(m.name).is_absolute() or '..' in parts or not (m.isdir() or m.isfile()):
                    raise RuntimeError('unexpected export member')
                target=dest.joinpath(*parts[1:])
                if m.isdir():target.mkdir(parents=True,exist_ok=True)
                else:
                    target.parent.mkdir(parents=True,exist_ok=True)
                    target.write_bytes(arc.extractfile(m).read())
        print(json.dumps({'stage':'run','new_record_saved':str(dest),'export_bytes':len(data),'no_retry':True}))
    finally:c.close()

if __name__=='__main__':main()
