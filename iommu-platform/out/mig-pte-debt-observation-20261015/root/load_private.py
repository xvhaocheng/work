#!/usr/bin/env python3
"""Volatile JTAG load of one timing-accepted private observer image; no flash."""
from pathlib import Path
import argparse, hashlib, json, os, subprocess, threading, time
import serial
B=Path(__file__).resolve().parents[1]
def main():
    ap=argparse.ArgumentParser();ap.add_argument('closure',choices=['sixth']);args=ap.parse_args()
    result_path=B/'sixth-verification'/'result.json'
    d=json.loads(result_path.read_text())
    if d['rc']!=0 or not d['gates'] or not all(d['gates'].values()):raise SystemExit('timing/artifact gates not accepted')
    bit=Path(d['bitstream']);sha=hashlib.sha256(bit.read_bytes()).hexdigest()
    fd=os.open(B/'root/load-attempt-registered.json',os.O_WRONLY|os.O_CREAT|os.O_EXCL,0o600)
    with os.fdopen(fd,'w') as f:json.dump({'BIT':str(bit),'sha256':sha,'closure':str(result_path),'volatile_only':True},f,indent=2)
    tcl=B/'root/load-private.tcl'
    tcl.write_text('''open_hw_manager
connect_hw_server -url localhost:3121
set target [get_hw_targets localhost:3121/xilinx_tcf/Digilent/200300BD5148B]
if {[llength $target] != 1} {error {Expected exact known target}}
open_hw_target $target
set dev [get_hw_devices xc7k325t_0]
if {[llength $dev] != 1 || [get_property PART $dev] ne "xc7k325t"} {error {FPGA part mismatch}}
set_property PROGRAM.FILE {'''+str(bit)+'''} $dev
program_hw_devices $dev
refresh_hw_device $dev
puts "MIG_OBSERVATION_PROGRAM_SUCCESS"
close_hw_target
close_hw_manager
exit
''')
    stop=threading.Event();data=bytearray()
    uart=serial.Serial('/dev/ttyUSB0',115200,timeout=.1)
    def reader():
        with (B/'root/program-and-boot.raw.log').open('ab',buffering=0) as f:
            while not stop.is_set():
                x=uart.read(4096)
                if x:f.write(x);data.extend(x)
    th=threading.Thread(target=reader);th.start()
    try:
        with (B/'root/program-console.log').open('wb') as out:
            rc=subprocess.call(['/tools/Xilinx/Vivado/2023.1/bin/vivado','-mode','batch','-source',str(tcl),'-log',str(B/'root/program-vivado.log'),'-journal',str(B/'root/program-vivado.jou')],cwd=B,stdout=out,stderr=subprocess.STDOUT)
        after=hashlib.sha256(bit.read_bytes()).hexdigest()
        success=rc==0 and b'MIG_OBSERVATION_PROGRAM_SUCCESS' in (B/'root/program-console.log').read_bytes() and after==sha
        receipt={'sha256':sha,'path':str(bit),'closure':str(result_path),'program_exit_code':rc,'program_success_marker':success,'target':'200300BD5148B/xc7k325t_0','volatile_only':True,'flash_written':False}
        (B/'root/loaded-bit.json').write_text(json.dumps(receipt,indent=2)+'\n')
        if not success:raise RuntimeError('actual JTAG program did not succeed')
        deadline=time.monotonic()+120
        while time.monotonic()<deadline:
            tail=bytes(data[-4096:])
            if b'login:' in tail or b'root@' in tail or b'~#' in tail:break
            time.sleep(.25)
        print(json.dumps(receipt))
    finally:
        stop.set();th.join();uart.close()
if __name__=='__main__':main()
