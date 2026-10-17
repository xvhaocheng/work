#!/usr/bin/env python3
"""Load one hashed BIT with native Vivado and capture the complete UART boot."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import re
import socket
import subprocess
import time
from uart import Console, DEFAULT_PORT
from auth import authenticate_password

VIVADO = '/tools/Xilinx/Vivado/2023.1/bin/vivado'
HW_SERVER = '/tools/Xilinx/Vivado/2023.1/bin/hw_server'
SERIAL = '200300BD5148B'


def save(path, value):
    Path(path).write_text(json.dumps(value, indent=2) + '\n')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--bit', type=Path, required=True)
    p.add_argument('--expected-sha256', required=True)
    p.add_argument('--expected-kernel', required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--port', default=DEFAULT_PORT)
    p.add_argument('--boot-timeout', type=int, default=1200)
    p.add_argument('--credential-helper', type=Path,
                   help='Trusted existing helper supplying the board credential in memory')
    a = p.parse_args()
    bit = a.bit.resolve()
    digest = hashlib.sha256(bit.read_bytes()).hexdigest()
    if digest != a.expected_sha256:
        p.error('BIT SHA256 mismatch; nothing loaded')
    if any(c in str(bit) for c in '{}\n\r'):
        p.error('unsupported BIT path for Tcl literal')
    a.output.mkdir(parents=True, exist_ok=False)
    manifest = {'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
                'bit_path': str(bit), 'bit_sha256': digest, 'expected_kernel': a.expected_kernel,
                'target_serial': SERIAL, 'status': 'prepared',
                'configuration_readback_verified': False,
                'script_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest()}
    save(a.output/'load.json', manifest)
    # Bind only a temporary local hardware server, matching the historical Dell route.
    probe = socket.socket(); probe.bind(('127.0.0.1', 0)); port = probe.getsockname()[1]; probe.close()
    tcl = (f'open_hw_manager\nconnect_hw_server -url 127.0.0.1:{port}\n'
        'set targets {}\nforeach candidate [get_hw_targets -quiet] {\n'
        f' if {{[string equal [lindex [split $candidate /] end] "{SERIAL}"]}} {{lappend targets $candidate}}\n'
        '}\nif {[llength $targets] != 1} {error "Exact Digilent serial not uniquely available"}\n'
        'set target [lindex $targets 0]\ncurrent_hw_target $target\n'
        'set_property PARAM.FREQUENCY 1000000 $target\nopen_hw_target $target\n'
        'set dev [get_hw_devices -quiet xc7k325t_0]\n'
        'if {[llength $dev] != 1 || [get_property PART $dev] ne "xc7k325t"} {error "Wrong FPGA"}\n'
        'puts "ISPASS_TARGET=$target"\nputs "ISPASS_IDCODE=[get_property IDCODE $dev]"\n'
        f'set_property PROGRAM.FILE {{{bit}}} $dev\nprogram_hw_devices $dev\n'
        'refresh_hw_device -update_hw_probes false $dev\n'
        'puts "ISPASS_CONFIG_STATUS=[get_property REGISTER.CONFIG_STATUS $dev]"\n'
        'puts "ISPASS_PROGRAM_RETURNED"\nclose_hw_target\ndisconnect_hw_server\nclose_hw_manager\n')
    (a.output/'program.tcl').write_text(tcl)
    console = server = client = None
    try:
        console = Console(a.port, a.output/'uart')
        with (a.output/'hw-server.log').open('wb') as log:
            server = subprocess.Popen([HW_SERVER, '-s', f'tcp::{port}'], stdout=log, stderr=subprocess.STDOUT)
        deadline = time.monotonic()+15
        while True:
            if server.poll() is not None:
                raise RuntimeError('hw_server exited')
            try:
                with socket.create_connection(('127.0.0.1', port), .2): break
            except OSError:
                if time.monotonic() > deadline: raise TimeoutError('hw_server start')
                time.sleep(.1)
        with (a.output/'program.log').open('wb') as log:
            argv = [VIVADO, '-mode', 'batch', '-nolog', '-nojournal', '-source', str((a.output/'program.tcl').resolve())]
            manifest.update(status='programming', command=argv)
            save(a.output/'load.json', manifest)
            client = subprocess.Popen(argv, cwd=a.output, stdout=log, stderr=subprocess.STDOUT)
        captured = b''
        deadline = time.monotonic()+a.boot_timeout
        logged_in = False
        fresh_boot_seen = False
        while time.monotonic() < deadline:
            captured += console.read(.2)
            fresh_boot_seen = fresh_boot_seen or bool(re.search(rb'OpenSBI|Linux version|Booting Linux', captured))
            if client.poll() not in (None, 0):
                raise RuntimeError('Vivado programming failed: '+str(client.returncode))
            tail = captured[-4096:].decode('utf8', 'replace').replace('\r', '')
            if not logged_in and re.search(r'(?:^|\n)[^\n]*login:\s*$', tail):
                console.send(b'root\n')
                logged_in = True
                captured = b''
            if logged_in and re.search(r'Password:\s*$', tail):
                if a.credential_helper is None:
                    raise RuntimeError('board requires the existing credential helper')
                authenticate_password(console, a.credential_helper)
                captured = b'\n# '
                tail = '\n# '
            if client.poll() == 0 and fresh_boot_seen and re.search(r'(?:^|\n)[^\n]*[#$] ?$', tail):
                break
        else:
            raise TimeoutError('boot/login prompt not observed; no automatic reload')
        identity = console.command('printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; printf "TAINT="; cat /proc/sys/kernel/tainted', 20)
        assert identity['exit_code'] == 0
        text = identity['stdout'].replace('\r', '')
        kernel = re.search(r'^KERNEL=(.+)$', text, re.M)
        boot = re.search(r'^BOOT=([0-9a-f-]{36})$', text, re.M)
        assert kernel and kernel[1] == a.expected_kernel, 'wrong kernel after load'
        assert boot and re.search(r'^TAINT=0$', text, re.M)
        assert client.poll() == 0 and 'ISPASS_PROGRAM_RETURNED' in (a.output/'program.log').read_text()
        manifest.update(status='boot_identity_verified', kernel_release=kernel[1], boot_id=boot[1],
                        vivado_exit_code=0, program_file_sha256_verified=True)
    except BaseException as error:
        manifest.update(status='failed', error=type(error).__name__+': '+str(error))
        raise
    finally:
        if client is not None and client.poll() is None:
            client.terminate(); client.wait(15)
        if console is not None: console.close()
        if server is not None:
            server.terminate(); server.wait(15)
        manifest['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
        save(a.output/'load.json', manifest)
    print(json.dumps(manifest, indent=2))


if __name__ == '__main__':
    main()
