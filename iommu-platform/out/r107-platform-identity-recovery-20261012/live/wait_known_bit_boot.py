#!/usr/bin/env python3
import sys,importlib.util,time,json,re,subprocess
from pathlib import Path
from datetime import datetime,timezone
sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
def now():return datetime.now(timezone.utc).isoformat()
def main():
 record={'started_at_utc':now(),'action':'passive UART boot wait only','maximum_seconds':360,'board_writes':0,'reload_reset_readback_SD_or_Flash_write':False,'automatic_retry':False,'raw':'known-bit-recovery-boot-extension-UART.raw.log','prior_capture':'known-bit-recovery-boot-UART.raw.log','gap_is_not_filled':True}
 console=None;start=time.monotonic()
 try:
  spec=importlib.util.spec_from_file_location('r110_wait_transport',HERE/'board_query.py');m=importlib.util.module_from_spec(spec);sys.modules[spec.name]=m;spec.loader.exec_module(m)
  owner=subprocess.run(['fuser',str(m.PORT.resolve())],capture_output=True,timeout=5)
  if owner.returncode!=1 or owner.stdout.strip() or owner.stderr.strip():raise RuntimeError('Visible UART owner or unverifiable ownership')
  spec=importlib.util.spec_from_file_location('r110_wait_console',m.SOURCE);ref=importlib.util.module_from_spec(spec);sys.modules[spec.name]=ref;spec.loader.exec_module(ref)
  console=ref.SerialConsole(str(m.PORT),HERE/record['raw'],HERE/'known-bit-recovery-boot-extension-UART.events.log',serial_factory=m.PosixTransport)
  console.start();print('Additional passive boot capture started; no board writes',flush=True)
  while time.monotonic()-start<360:
   with console._cond:data=bytes(console._buffer[-8192:])
   if b'SD card failed!' in data or b'sd copy return value:' in data:
    record['status']='explicit_SD_boot_error';break
   if re.search(rb'(?:^|[\r\n])[^\r\n]*login:\s*$',data):
    record['status']='login_prompt_observed';break
   time.sleep(.2)
  else:record['status']='observation_timeout_not_boot_failure_proof'
 except Exception as e:record['status']='passive_observation_failed';record['error']=str(e)
 finally:
  if console is not None:console.close()
  record['ended_at_utc']=now();record['elapsed_seconds']=time.monotonic()-start
  (HERE/'known-bit-boot-extension-result.json').write_text(json.dumps(record,indent=2)+'\n')
 print(json.dumps(record,indent=2))
 return 0 if record['status']=='login_prompt_observed' else 2
if __name__=='__main__':raise SystemExit(main())
