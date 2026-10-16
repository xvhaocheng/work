#!/usr/bin/env python3
"""One user-approved volatile load. Keep boot capture separate from credentials."""
import importlib.util,sys,json,hashlib,time,subprocess,socket,re
from pathlib import Path
from datetime import datetime,timezone
sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
BASE=HERE.parent
BIT=Path('/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/route-opt/ariane_xilinx.bit')
EXPECT='f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca'
def now():return datetime.now(timezone.utc).isoformat()
def main():
    result={'started_at_utc':now(),'approval_receipt':'known-bit-reload-approval.json','volatile_program_attempts':0,'bounded_recovery':1,'configuration_readback_during_SD_boot':False,'same_approved_bit':True,'prior_masked_verify_passed':True,'Flash_SD_image_or_eFuse_write':False,'Linux_restart_is_authorized':True,'automatic_retry':False,'server_command':['/tools/Xilinx/Vivado/2023.1/bin/hw_server','-s','tcp:127.0.0.1:3123','-p0','-I15'],'client_command':['/tools/Xilinx/Vivado/2023.1/bin/vivado','-mode','batch','-nolog','-nojournal','-source',str(HERE/'recover_known_bit_without_readback.tcl')],'pre_reload_query':'../live/pre-known-bit-reload-result.json','recovery_contract':'known-bit-recovery-contract.json','pre_reload_sync_confirmed':False,'limits':'The pre-load UART query timed out without receiving a begin marker. Prior Linux query was successful; current responsiveness and sync are not assumed. No prior FPGA state is promised restorable.'}
    server=client=console=None
    start=time.monotonic()
    try:
        assert json.loads((HERE/'known-bit-reload-approval.json').read_text())['permission']=='approved'
        result['bit_sha256']=hashlib.sha256(BIT.read_bytes()).hexdigest()
        if result['bit_sha256']!=EXPECT:raise RuntimeError('Exact approved bit changed')
        packet=json.loads((HERE/'reference/reference-packet-link.json').read_text())
        if not packet['configuration_packet_suffix_equal'] or packet['original_bit_sha256']!=EXPECT:raise RuntimeError('Mask reference does not correspond to approved configuration packets')
        port=socket.socket();port.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1);port.bind(('127.0.0.1',3123));port.close()
        spec=importlib.util.spec_from_file_location('r110_capture_transport',BASE/'live/board_query.py');m=importlib.util.module_from_spec(spec);sys.modules[spec.name]=m;spec.loader.exec_module(m)
        owner=subprocess.run(['fuser',str(m.PORT.resolve())],capture_output=True,timeout=5)
        if owner.returncode!=1 or owner.stdout.strip() or owner.stderr.strip():raise RuntimeError('UART owner visible or ownership unavailable')
        spec=importlib.util.spec_from_file_location('r110_passive_console',m.SOURCE);ref=importlib.util.module_from_spec(spec);sys.modules[spec.name]=ref;spec.loader.exec_module(ref)
        console=ref.SerialConsole(str(m.PORT),BASE/'live/known-bit-recovery-boot-UART.raw.log',BASE/'live/known-bit-recovery-boot-UART.events.log',serial_factory=m.PosixTransport)
        console.start();result['passive_capture_started_at_utc']=now();print('Passive UART capture started; approved bit hash verified',flush=True)
        with (HERE/'recovery-hw-server.raw.log').open('wb') as slog:
            server=subprocess.Popen(result['server_command'],stdout=slog,stderr=subprocess.STDOUT,cwd=HERE)
            deadline=time.monotonic()+10
            while True:
                if server.poll() is not None:raise RuntimeError('Task hw_server exited before connection')
                try:
                    with socket.create_connection(('127.0.0.1',3123),timeout=.2):break
                except OSError:
                    if time.monotonic()>deadline:raise RuntimeError('Task hw_server startup timeout')
                    time.sleep(.1)
            with (HERE/'known-bit-recovery-no-readback.raw.log').open('wb') as clog:
                result['client_started_at_utc']=now();result['volatile_program_attempts']=1
                client=subprocess.Popen(result['client_command'],stdout=clog,stderr=subprocess.STDOUT,cwd=HERE)
                result['client_exit_code']=client.wait(timeout=360)
                result['client_ended_at_utc']=now()
            print('Program/verify client completed with exit '+str(result['client_exit_code']),flush=True)
            # Capture all boot output until a visible login prompt, or one bounded 120s wait.
            wait_start=time.monotonic()
            while time.monotonic()-wait_start<120:
                with console._cond:latest=bytes(console._buffer[-8192:])
                if re.search(rb'(?:^|[\r\n])[^\r\n]*login:\s*$',latest):
                    result['login_prompt_in_capture']=True;break
                time.sleep(.2)
            result['post_client_capture_seconds']=time.monotonic()-wait_start
            result['status']='program_and_verify_returned' if result['client_exit_code']==0 else 'program_or_verify_failure_requires_evidence_review'
    except Exception as exc:
        result['status']='not_accepted';result['error']=str(exc)
    finally:
        if client is not None and client.poll() is None:client.terminate();client.wait(timeout=10)
        if console is not None:console.close()
        if server is not None:
            if server.poll() is None:server.terminate()
            result['task_server_exit_code']=server.wait(timeout=10)
        result['ended_at_utc']=now();result['elapsed_seconds']=time.monotonic()-start
        (HERE/'known-bit-recovery-no-readback-result.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
    print(json.dumps(result,ensure_ascii=False,indent=2))
    return 0 if result.get('client_exit_code')==0 and result.get('status')=='program_and_verify_returned' else 2
if __name__=='__main__':raise SystemExit(main())
