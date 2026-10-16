#!/usr/bin/env python3
"""Repack pinned newc rootfs with an explicit executable list; no root needed."""
from pathlib import Path
import stat

ROOT=Path(__file__).resolve().parents[1]
LEGACY_NAMES={'idma_bench','idma_bench-2idma','idma_pair_bench','idma_multi_bench','idma_once'}
EXECUTABLES={
    'idma_e3_controlled': 'out/bench/idma_e3_controlled',
    'platform-e3-controlled': 'tests/board-e3-controlled.sh',
    'idma_e3_pair': 'out/bench/idma_e3_pair',
    'idma_experiment': 'out/bench/idma_experiment',
    'platform-smoke': 'tests/board-smoke.sh',
    'platform-p4m': 'tests/board-p4m.sh',
    'platform-timeout': 'out/bench/platform-timeout',
}

def read_newc(data):
    pos=0
    while pos+110 <= len(data):
        header=data[pos:pos+110]
        if header[:6] != b'070701':
            raise ValueError(f'Expected newc archive at byte {pos}')
        fields=[int(header[6+i*8:14+i*8],16) for i in range(13)]
        pos+=110
        name=data[pos:pos+fields[11]-1].decode()
        pos=(pos+fields[11]+3)&~3
        body=data[pos:pos+fields[6]]
        if len(body)!=fields[6]: raise ValueError('Truncated cpio')
        pos=(pos+fields[6]+3)&~3
        if name=='TRAILER!!!': return
        yield name,fields,body
    raise ValueError('Missing cpio trailer')

def write_entry(out,name,fields,body):
    fields=list(fields)
    encoded=name.encode()+b'\0'
    fields[5]=0  # deterministic archive time
    fields[6]=len(body)
    fields[11]=len(encoded)
    fields[12]=0
    out.extend(b'070701'+''.join(f'{v:08x}' for v in fields).encode()+encoded)
    out.extend(b'\0'*((-len(out))%4))
    out.extend(body)
    out.extend(b'\0'*((-len(out))%4))

def repack(root=ROOT):
    entries=list(read_newc((root/'assets/rootfs-base.cpio').read_bytes()))
    binaries={name:(root/path).read_bytes() for name,path in EXECUTABLES.items()}
    output=bytearray()
    ino=max(f[0] for _,f,_ in entries)+1
    # Remove every prior copy, including root-level and duplicate managed entries.
    replaced_names=LEGACY_NAMES | EXECUTABLES.keys()
    for name,fields,body in entries:
        if Path(name).name not in replaced_names:
            write_entry(output,name,fields,body)
    for name,body in binaries.items():
        fields=[ino,stat.S_IFREG|0o755,0,0,1,0,0,0,0,0,0,0,0]
        write_entry(output,'usr/bin/'+name,fields,body)
        ino+=1
    write_entry(output,'TRAILER!!!',[0]*13,b'')
    output.extend(b'\0'*((-len(output))%512))
    # Validate before replacing the output. Path normalizes leading './' aliases.
    packed={}
    for name,fields,body in read_newc(output):
        normalized=str(Path(name))
        if normalized in packed:
            raise ValueError(f'Duplicate rootfs entry: {normalized}')
        if Path(name).name in LEGACY_NAMES:
            raise ValueError(f'Legacy benchmark in rootfs: {name}')
        packed[normalized]=(fields,body)
    for name,body in binaries.items():
        actual_fields,actual_body=packed['usr/bin/'+name]
        if actual_body != body or actual_fields[1] != (stat.S_IFREG|0o755):
            raise ValueError(f'Executable verification failed: {name}')
        if sum(Path(path).name == name for path in packed) != 1:
            raise ValueError(f'Duplicate executable in rootfs: {name}')
    dest=root/'out/rootfs.cpio'
    dest.parent.mkdir(parents=True,exist_ok=True)
    dest.write_bytes(output)
    print('PASS: rootfs contains current experiment/smoke/watchdog; device nodes preserved')

if __name__=='__main__': repack()
