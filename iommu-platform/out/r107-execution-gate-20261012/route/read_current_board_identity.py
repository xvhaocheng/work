#!/usr/bin/env python3
"""One framed identity query over the already connected Dell UART.

Reuse the preserved Mac SerialConsole protocol. No flash, restart, SD write,
DMA experiment, permissions change, automatic login or retry.
"""
import argparse
import fcntl
import hashlib
import importlib.util
import json
import os
import re
from pathlib import Path
import select
import subprocess
import sys
import termios
import time
from datetime import datetime, timezone

HERE = Path(__file__).resolve().parent
SOURCE = HERE / 'mac-reference/tools--composition-isolated-20261007--mac_research2.py'
PORT = Path('/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AV0KRCHT-if00-port0')
COMMAND = (
    "printf 'R109_UNAME='; uname -r; "
    "printf 'R109_BOOT_ID='; cat /proc/sys/kernel/random/boot_id; "
    "printf 'R109_TAINT='; cat /proc/sys/kernel/tainted; "
    "printf 'R109_DTB_SHA='; if test -r /sys/firmware/fdt; then sha256sum /sys/firmware/fdt; "
    "else printf 'UNAVAILABLE\\n'; fi; "
    "printf 'R109_MODEL='; if test -r /proc/device-tree/model; "
    "then tr '\\000' '\\n' < /proc/device-tree/model; else printf 'UNAVAILABLE\\n'; fi; "
    "printf 'R109_IDMA_NODES\\n'; ls -l /dev/idma* 2>/dev/null; "
    "printf 'R109_MEMINFO\\n'; head -n 3 /proc/meminfo; "
    "printf 'R109_READONLY_IDENTITY_END\\n'"
)


class PosixTransport:
    """Minimal Linux transport for the existing console's factory interface."""

    def __init__(self, port, baud, timeout):
        if baud != 115200:
            raise ValueError('only the recorded 115200/8N1 transport is accepted')
        self.timeout = timeout
        self.fd = os.open(port, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
        self.original = None
        try:
            fcntl.ioctl(self.fd, termios.TIOCEXCL)
            self.original = termios.tcgetattr(self.fd)
            attrs = termios.tcgetattr(self.fd)
            attrs[0] = 0
            attrs[1] = 0
            attrs[2] = termios.CS8 | termios.CREAD | termios.CLOCAL
            attrs[3] = 0
            attrs[4] = attrs[5] = termios.B115200
            attrs[6][termios.VMIN] = 0
            attrs[6][termios.VTIME] = 0
            termios.tcsetattr(self.fd, termios.TCSANOW, attrs)
        except BaseException:
            if self.original is not None:
                termios.tcsetattr(self.fd, termios.TCSANOW, self.original)
            os.close(self.fd)
            raise

    def read(self, count):
        if not select.select([self.fd], [], [], self.timeout)[0]:
            return b''
        try:
            return os.read(self.fd, count)
        except BlockingIOError:
            return b''

    def write(self, data):
        remaining = memoryview(data)
        deadline = time.monotonic() + 2
        while remaining:
            wait = deadline - time.monotonic()
            if wait <= 0 or not select.select([], [self.fd], [], wait)[1]:
                raise TimeoutError('UART write timeout; no retry')
            try:
                remaining = remaining[os.write(self.fd, remaining):]
            except BlockingIOError:
                continue

    def flush(self):
        # All bytes have been accepted by the transport. The framed response
        # confirms receipt; avoid an unbounded tcdrain wait on a broken device.
        pass

    def close(self):
        try:
            if self.original is not None:
                termios.tcsetattr(self.fd, termios.TCSANOW, self.original)
            fcntl.ioctl(self.fd, termios.TIOCNXCL)
        finally:
            os.close(self.fd)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--describe', action='store_true')
    parser.add_argument('--output-prefix', default='live')
    args = parser.parse_args()
    if not re.fullmatch(r'[a-zA-Z0-9_-]+', args.output_prefix):
        raise ValueError('output prefix must be a simple artifact name')
    record = {
        'started_at_utc': datetime.now(timezone.utc).isoformat(),
        'port': str(PORT), 'baud': 115200, 'format': '8N1',
        'command': COMMAND, 'reused_console_source': str(SOURCE),
        'source_sha256': hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
        'UART_opened': False, 'board_query_succeeded': False,
        'flash_restart_SD_DMA_or_permission_change': False,
        'automatic_retry': False,
        'limits': 'TTY open/configuration can affect host modem state; no explicit DTR/RTS control. '
                  'Only a framed software identity query; no proof of live BIT hash, mode, or physical clock.',
    }
    if args.describe:
        print(json.dumps(record, ensure_ascii=False, indent=2))
        return 0
    console = None
    try:
        if not PORT.exists() or not (os.access(PORT, os.R_OK) and os.access(PORT, os.W_OK)):
            raise PermissionError('current user lacks UART read/write access; obtain scoped operator access first')
        owner = subprocess.run(['fuser', str(PORT.resolve())], capture_output=True, timeout=5)
        record['visible_owner_check'] = {'exit_code': owner.returncode,
                                       'stdout': owner.stdout.decode(errors='replace'),
                                       'stderr': owner.stderr.decode(errors='replace')}
        if owner.returncode != 1 or owner.stdout.strip() or owner.stderr.strip():
            raise RuntimeError('visible UART owner or unverifiable ownership; do not take over')
        spec = importlib.util.spec_from_file_location('r109_existing_console', SOURCE)
        module = importlib.util.module_from_spec(spec)
        sys.modules[spec.name] = module
        spec.loader.exec_module(module)
        console = module.SerialConsole(str(PORT), HERE / (args.output_prefix + '-UART.raw.log'),
                                       HERE / (args.output_prefix + '-UART.events.log'), serial_factory=PosixTransport)
        console.start()
        record['UART_opened'] = True
        result = console.run_shell(COMMAND, timeout=8)
        payload = result.stdout.decode('utf-8', errors='replace')
        record['board_command_exit_code'] = result.exit_code
        record['board_stdout'] = payload
        uname = re.search(r'^R109_UNAME=([^\r\n]+)', payload, re.M)
        boot = re.search(r'^R109_BOOT_ID=([0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12})\s*$', payload, re.M)
        record['parsed_uname'] = uname.group(1) if uname else None
        record['parsed_boot_id'] = boot.group(1) if boot else None
        record['board_query_succeeded'] = bool(result.exit_code == 0 and uname and boot)
        if not record['board_query_succeeded']:
            raise RuntimeError('framed identity fields missing or command failed; no retry')
        record['status'] = 'software_identity_received_only'
    except Exception as error:
        record['status'] = 'identity_query_not_accepted'
        record['error'] = str(error)
    finally:
        if console is not None:
            console.close()
        record['ended_at_utc'] = datetime.now(timezone.utc).isoformat()
        (HERE / (args.output_prefix + '-identity-result.json')).write_text(
            json.dumps(record, ensure_ascii=False, indent=2) + '\n')
    print(json.dumps(record, ensure_ascii=False, indent=2))
    return 0 if record['board_query_succeeded'] else 2


if __name__ == '__main__':
    raise SystemExit(main())
