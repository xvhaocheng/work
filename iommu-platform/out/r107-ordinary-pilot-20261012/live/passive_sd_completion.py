#!/usr/bin/env python3
import sys
sys.dont_write_bytecode=True
import importlib.util,subprocess,json,time,re
from pathlib import Path
root=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('r111_uart',root/'board_query.py');m=importlib.util.module_from_spec(spec);sys.modules[spec.name]=m;spec.loader.exec_module(m)
check=subprocess.run(['fuser',str(m.PORT.resolve())],capture_output=True,timeout=5)
if check.returncode!=1 or check.stdout or check.stderr:raise RuntimeError('UART ownership not available')
old=(root/'sd-original-backup-UART.raw.log').read_bytes();data=bytearray();start=time.monotonic_ns();transport=None
endmarker=b'__R2E_e6d2a4ed82500e9a_55c4e776d1f99b6f__'
try:
 transport=m.PosixTransport(str(m.PORT),115200,0.1)
 deadline=time.monotonic()+300
 while time.monotonic()<deadline:
  chunk=transport.read(65536)
  if chunk:data.extend(chunk)
  if re.search(rb'(?m)^'+re.escape(endmarker)+rb'([0-9]+)\r?$',old+data):break
finally:
 if transport:transport.close()
 (root/'sd-original-backup-passive-UART.raw.log').write_bytes(data)
 combined=old+data
 match=re.search(rb'(?m)^'+re.escape(endmarker)+rb'([0-9]+)\r?$',combined)
 sha=re.search(rb'(?m)^([0-9a-f]{64})\s+/tmp/r107-stage111/original-mmcblk0p1.bin\r?$',combined)
 size=re.search(rb'(?m)^33554432\s+/tmp/r107-stage111/original-mmcblk0p1.bin\r?$',combined)
 rec={'kind':'one passive extension of already running read-only SD backup; no new TX or read command','dell_monotonic_start_ns':start,'dell_monotonic_end_ns':time.monotonic_ns(),'transmitted_bytes':0,'extension_limit_seconds':300,'original_end_marker_received':bool(match),'original_exit_code':int(match[1]) if match else None,'board_backup_sha256':sha[1].decode() if sha else None,'board_backup_size_confirmed':bool(size),'board_backup_complete':bool(sha and size),'stdout':data.decode(errors='replace')}
 rec['dell_wall_seconds']=(rec['dell_monotonic_end_ns']-start)/1e9
 (root/'sd-original-backup-passive-result.json').write_text(json.dumps(rec,indent=2)+'\n');print(json.dumps(rec,indent=2))
