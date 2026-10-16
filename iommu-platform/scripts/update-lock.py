#!/usr/bin/env python3
"""Record component HEADs after committing component changes and parent gitlinks."""
import json
from pathlib import Path
import subprocess

root=Path(__file__).resolve().parents[1]
path=root/'sources.lock.json'
data=json.loads(path.read_text())
for item in data['components']:
    repo=root/item['path']
    item['commit']=subprocess.check_output(['git','-C',str(repo),'rev-parse','HEAD'],text=True).strip()
path.write_text(json.dumps(data,indent=2)+'\n')
print('Updated sources.lock.json; commit it together with top-level submodule pointers, then make check.')
