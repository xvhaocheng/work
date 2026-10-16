#!/usr/bin/env python3
"""Repack pinned newc rootfs without extraction/root permissions; replace old benches."""
from pathlib import Path
import stat

ROOT=Path(__file__).resolve().parents[1]

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

def main():
    entries=list(read_newc((ROOT/'assets/rootfs-base.cpio').read_bytes()))
    output=bytearray()
    ino=max(f[0] for _,f,_ in entries)+1
    # Remove legacy copies anywhere, including the former root-level executables.
    old_names={'idma_bench','idma_bench-2idma','idma_pair_bench','idma_multi_bench','idma_once'}
    for name,fields,body in entries:
        if Path(name).name not in old_names:
            write_entry(output,name,fields,body)
    for binary in sorted((ROOT/'out/bench').iterdir()):
        if not binary.is_file(): continue
        fields=[ino,stat.S_IFREG|0o755,0,0,1,0,0,0,0,0,0,0,0]
        write_entry(output,'usr/bin/'+binary.name,fields,binary.read_bytes())
        ino+=1
    smoke=ROOT/'tests/board-smoke.sh'
    write_entry(output,'usr/bin/platform-smoke',[ino,stat.S_IFREG|0o755,0,0,1,0,0,0,0,0,0,0,0],smoke.read_bytes())
    write_entry(output,'TRAILER!!!',[0]*13,b'')
    output.extend(b'\0'*((-len(output))%512))
    dest=ROOT/'out/rootfs.cpio'
    dest.parent.mkdir(parents=True,exist_ok=True)
    dest.write_bytes(output)
    # Reparse the result and verify every new executable is exactly the built one.
    packed={name:body for name,_,body in read_newc(output)}
    for binary in sorted((ROOT/'out/bench').iterdir()):
        if binary.is_file():
            assert packed['usr/bin/'+binary.name]==binary.read_bytes()
    print('PASS: rootfs repacked with current benchmarks, device nodes preserved')

if __name__=='__main__': main()
