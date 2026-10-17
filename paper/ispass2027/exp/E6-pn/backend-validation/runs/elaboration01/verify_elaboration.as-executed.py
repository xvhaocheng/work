#!/usr/bin/env python3
"""Verilator-only static branch elimination receipt; no FPGA synthesis."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import xml.etree.ElementTree as ET


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--run',required=True,type=Path)
    p.add_argument('--output',required=True,type=Path)
    a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    manifest=a.run/'manifest.json';run=json.loads(manifest.read_text())
    assert run['status']=='passed'
    assert all(sha(a.run/name)==digest for name,digest in run['inputs'].items())
    receipt={'schema':'pn-backend-elaboration-v1','status':'running','fixture_only':True,
             'evidence_level':'verilator_elaboration_only','board_points':0,'full_fpga_builds':0,
             'source_manifest':str(manifest.resolve()),'source_manifest_sha256':sha(manifest),'commands':[],'results':[]}
    path=out/'manifest.json'
    def save():path.write_text(json.dumps(receipt,indent=2)+'\n')
    save()
    try:
        for enable in (0,1):
            original=next(c['argv'] for c in run['commands'] if c['log']==f'enable{enable}-build.log')
            args=[];i=0
            while i<len(original):
                item=original[i]
                if item in ('-j','--Mdir','-CFLAGS'):i+=2;continue
                if item in ('--cc','--exe','--build') or item.endswith('.cpp'):i+=1;continue
                args.append(item);i+=1
            xml=out/f'elaboration-enable{enable}.xml'
            command=args+['--xml-only','--xml-output',str(xml),'--Mdir',str(out/f'obj-enable{enable}')]
            entry={'argv':command,'log':f'enable{enable}.log'};receipt['commands'].append(entry);save()
            with (out/entry['log']).open('w') as log:r=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,timeout=300)
            entry.update(exit_code=r.returncode,sha256=sha(out/entry['log']));save()
            assert r.returncode==0
            raw=xml.read_bytes();root=ET.fromstring(raw)
            candidate=next(c for c in root.findall('.//cells/cell/cell') if c.get('name')=='candidate')
            names=[c.get('name') for c in candidate.iter('cell')]
            wanted=('i_idma_backend_fullbuffer','i_small_certificate','i_large_certificate')
            assert 'i_idma_backend' in names
            assert all((name in names)==bool(enable) for name in wanted)
            start=raw.index(b'<cells>');end=raw.index(b'</cells>',start)+len(b'</cells>')
            snippet=out/f'enable{enable}-cells.as-executed.xml';snippet.write_bytes(raw[start:end]+b'\n')
            receipt['results'].append({'enable':enable,'original_backend_present':True,
                'additional_backend_and_certificates_present':bool(enable),'cell_names':names,
                'raw_xml_local':str(xml),'raw_xml_sha256':sha(xml),'raw_bytes':len(raw),
                'verbatim_cells':snippet.name,'sha256':sha(snippet),'byte_interval':[start,end]})
            save()
        receipt['status']='passed'
    except Exception as error:receipt.update(status='failed',error=repr(error))
    save();print(json.dumps({'status':receipt['status'],'manifest':str(path),'error':receipt.get('error')}))
    return 0 if receipt['status']=='passed' else 1


if __name__=='__main__':raise SystemExit(main())
