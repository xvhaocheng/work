#!/usr/bin/env python3
from pathlib import Path
import hashlib,json,re,struct,subprocess
root=Path(__file__).resolve().parent.parent
elf=root/'source/hero-tools/apps/omp/basic/axpy/axpy_carfield_snitch_cluster.elf'
def sections(raw):
 off=struct.unpack_from('<Q',raw,40)[0];size,n,ss=struct.unpack_from('<HHH',raw,58)
 sh=[struct.unpack_from('<IIQQQQIIQQ',raw,off+i*size) for i in range(n)]
 s=sh[ss];names=raw[s[4]:s[4]+s[5]]
 def name(i):return names[sh[i][0]:].split(b'\0')[0].decode()
 return sh,name
raw=elf.read_bytes();sh,name=sections(raw)
exec_ranges=[(x[3],x[3]+x[5],name(i)) for i,x in enumerate(sh) if x[2]&4]
types=[]
for i,s in enumerate(sh):
 if s[1]==4:
  for off in range(s[4],s[4]+s[5],s[9]):types.append(struct.unpack_from('<Q',raw,off+8)[0]&0xffffffff)
assert set(types)<={3,4,5},types
unwind=(root/'evidence/host-final-compat-eh-frame.txt').read_text()
fdes=[(int(a,16),int(b,16)) for a,b in re.findall(r'pc=([0-9a-f]+)\.\.([0-9a-f]+)',unwind)]
assert fdes
for a,b in fdes:assert a<=b and any(lo<=a and b<=hi for lo,hi,_ in exec_ranges),(a,b,exec_ranges)
assert not (root/'evidence/host-final-compat-llvm-unwind-errors.txt').read_bytes()
gnu_warnings=(root/'evidence/host-final-compat-eh-frame-errors.txt').read_text().splitlines()
assert all(re.fullmatch(r'readelf: Warning: Unrecognized form: 0x2[23]', x) for x in gnu_warnings)
dynamic=(root/'evidence/host-final-compat-dynamic.txt').read_text()
assert 'RUNPATH' not in dynamic and 'RPATH' not in dynamic
log=(root/'logs/hero-offload-host-abi-compat.log').read_text()
assert 'unsupported relocation' not in log and 'error:' not in log
ledgers=[]
for p in root.glob('evidence/host-link-compat-*.json'):
 j=json.loads(p.read_text())
 if not j['conversions']:continue
 assert j['link_exit_code']==0
 for r in j['conversions']:
  a=Path(r['original']).read_bytes();b=Path(r['copy']).read_bytes()
  assert hashlib.sha256(a).hexdigest()==r['original_sha256']
  assert hashlib.sha256(b).hexdigest()==r['converted_sha256']
  allowed={x['entry_file_offset']+8 for x in r['changes']}
  differences={i for i,(aa,bb) in enumerate(zip(a,b)) if aa!=bb}
  assert len(a)==len(b) and differences==allowed
  for i in differences:assert a[i]==61 and b[i]==57
  # Exercise refusal on a known producer with an unreviewed destination.
  bad=bytearray(a);s,nam=sections(bad)
  section=next(i for i in range(len(s)) if nam(i)=='.rela.eh_frame')
  text=next(i for i in range(len(s)) if nam(i)=='.text')
  shoff=struct.unpack_from('<Q',bad,40)[0]
  struct.pack_into('<I',bad,shoff+section*64+44,text)
  fixture=root/'build/host-link-compat/reject-outside-eh.o';fixture.write_bytes(bad)
  run=subprocess.run([str(root/'scripts/hero-host-ld-compat.py'),'-r','-o','/dev/null',str(fixture)],capture_output=True,text=True)
  assert run.returncode!=0 and 'outside reviewed EH boundary' in run.stderr
 ledgers.append(str(p.relative_to(root)))
assert ledgers
result={'status':'passed','scope':'ELF/link static checks only; no target execution',
 'dynamic_relocation_types':sorted(set(types)),'dynamic_relocation_count':len(types),
 'private_relocation61_left':False,'eh_frame_fde_count':len(fdes),
 'all_fde_ranges_inside_executable_sections':True,'fixed_llvm_unwind_decoder_stderr_empty':True,'gnu_debug_form_warning_count':len(gnu_warnings),
 'absolute_native_runpath_absent':True,'conversion_preserves_every_other_byte':True,
 'wrapper_rejects_conversion_outside_eh_frame':True,'ledgers':ledgers}
(root/'evidence/final-elf-acceptance.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
