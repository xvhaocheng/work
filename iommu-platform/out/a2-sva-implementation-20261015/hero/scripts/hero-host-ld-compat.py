#!/usr/bin/env python3
"""Translate one proved private-fork relocation number at the host ld boundary.

Fixed Hero LLVM d8 declares R_RISCV_32_PCREL=61; Ubuntu binutils declares it
57. Both implement S+A-P written as 32 bits. Convert ONLY .rela.eh_frame
entries in a COPY of fixed-d8 RV64 objects. No relocation or unwind data is
removed. Other uses of private relocation 61 fail closed for review.
"""
from pathlib import Path
import hashlib, json, os, struct, subprocess, sys, time
root=Path(__file__).resolve().parent.parent
out=root/'build/host-link-compat';out.mkdir(parents=True,exist_ok=True)
fixed=b'd8cffb8d7294483b45732283719e250116fbe6c0'
args=sys.argv[1:];rewritten=[];receipts=[]
for arg in args:
 if arg.startswith('@'):
  raise SystemExit('host compatibility wrapper rejects uninspected response files')
 p=Path(arg)
 if not p.is_file() or p.suffix!='.o': rewritten.append(arg);continue
 raw=p.read_bytes()
 if raw[:6]!=b'\x7fELF\x02\x01' or struct.unpack_from('<H',raw,18)[0]!=243:
  rewritten.append(arg);continue
 data=bytearray(raw)
 shoff=struct.unpack_from('<Q',data,40)[0]
 entsize,n,strings=struct.unpack_from('<HHH',data,58)
 if entsize!=64 or strings>=n:raise SystemExit('unsupported ELF section layout')
 sh=[struct.unpack_from('<IIQQQQIIQQ',data,shoff+i*entsize) for i in range(n)]
 ss=sh[strings];names=data[ss[4]:ss[4]+ss[5]]
 def name(i):
  off=sh[i][0];end=names.find(b'\0',off)
  if end<0:raise SystemExit('bad ELF string table')
  return bytes(names[off:end]).decode()
 changes=[]
 for i,s in enumerate(sh):
  if s[1]!=4:continue # SHT_RELA
  if s[9]!=24 or s[7]>=n:raise SystemExit('unsupported RELA layout')
  target=name(s[7])
  for off in range(s[4],s[4]+s[5],24):
   r_offset,r_info,r_addend=struct.unpack_from('<QQq',data,off)
   if (r_info & 0xffffffff)!=61:continue
   if name(i)!='.rela.eh_frame' or target!='.eh_frame':
    raise SystemExit(f'relocation 61 outside reviewed EH boundary: {p}:{name(i)}->{target}')
   if fixed not in raw:raise SystemExit(f'object is not labelled with fixed d8 compiler: {p}')
   if not (sh[s[7]][2]&2) or r_offset+4>sh[s[7]][5]:
    raise SystemExit('invalid allocated EH relocation offset')
   struct.pack_into('<Q',data,off+8,(r_info & ~0xffffffff)|57)
   changes.append({'relocation_section':name(i),'target_section':target,
    'r_offset':hex(r_offset),'symbol_index':r_info>>32,'addend':r_addend,
    'old_type':61,'new_type':57,'entry_file_offset':off,
    'semantics':'same signed 32-bit S+A-P; only the producer/consumer numeric ABI is translated'})
 if changes:
  before=hashlib.sha256(raw).hexdigest();after=hashlib.sha256(data).hexdigest()
  q=out/(before[:20]+'-'+p.name);q.write_bytes(data)
  receipts.append({'original':str(p.resolve()),'copy':str(q),'original_sha256':before,'converted_sha256':after,'changes':changes})
  rewritten.append(str(q))
 else:rewritten.append(arg)
ledger={'schema':'fixed-hero-d8-host-relocation-compatibility-v1','time_ns':time.time_ns(),
 'original_argv':args,'actual_linker':'/usr/bin/riscv64-linux-gnu-ld',
 'actual_argv':rewritten,'conversions':receipts,
 'acceptance':'preserve EH contents/symbols/addends/offsets; convert only private d8 numeric 61 to system numeric57 for the proved same R_RISCV_32_PCREL formula'}
ledger_path=root/'evidence'/f'host-link-compat-{os.getpid()}.json'
ledger_path.write_text(json.dumps(ledger,indent=2)+'\n')
r=subprocess.run(['/usr/bin/riscv64-linux-gnu-ld',*rewritten])
ledger['link_exit_code']=r.returncode;ledger_path.write_text(json.dumps(ledger,indent=2)+'\n')
raise SystemExit(r.returncode)
