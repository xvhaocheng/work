#!/usr/bin/env python3
"""Restore a bundle set to a NEW directory without any upstream network access."""
import hashlib
import json
from pathlib import Path
import subprocess
import sys

def main():
    if len(sys.argv)!=3:
        raise SystemExit('usage: restore.py BUNDLE_DIRECTORY NEW_PLATFORM_DIRECTORY')
    source=Path(sys.argv[1]).resolve()
    dest=Path(sys.argv[2]).resolve()
    if dest.exists(): raise SystemExit('Destination must not exist')
    entries=json.loads((source/'bundles.json').read_text())
    for item in entries:
        bundle=source/item['bundle']
        digest=hashlib.sha256()
        with bundle.open('rb') as f:
            for chunk in iter(lambda:f.read(1024*1024),b''): digest.update(chunk)
        if digest.hexdigest()!=item['sha256']: raise SystemExit(f'Hash mismatch: {bundle}')
    for item in entries:
        path=dest/item['path']
        path.parent.mkdir(parents=True,exist_ok=True)
        subprocess.run(['git','clone','--quiet','--no-checkout',str(source/item['bundle']),str(path)],check=True)
        subprocess.run(['git','-C',str(path),'switch','-c','platform',item['commit']],check=True)
        subprocess.run(['git','-C',str(path),'remote','remove','origin'],check=True)
        if path==dest: continue
        lock=json.loads((dest/'sources.lock.json').read_text())
        record=next(x for x in lock['components'] if x['path']==item['path'])
        if record.get('upstream'):
            subprocess.run(['git','-C',str(path),'remote','add','upstream',record['upstream']],check=True)
    subprocess.run([sys.executable,str(dest/'scripts/platform.py'),'check'],cwd=dest,check=True)

if __name__=='__main__': main()
