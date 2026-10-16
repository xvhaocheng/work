#!/usr/bin/env python3
"""Store the explicitly supplied credential in Secret Service and scope access.

The credential is read only from this thread's latest matching user message.
It never appears in argv, environment, files, receipts, or stdout.
"""
import json
import os
from pathlib import Path
import pwd
import re
import subprocess
from datetime import datetime, timezone
import secretstorage

HERE = Path(__file__).resolve().parent
ROLLOUT = Path('/home/xvhaocheng/.codex/sessions/2026/10/12/rollout-2026-10-12T16-24-37-01a12eff-c971-7c62-8071-a08b646a4b61.jsonl')
ATTRIBUTES = {'application': 'codex', 'host': 'dell', 'username': 'xvhaocheng', 'purpose': 'sudo'}


def supplied_credential():
    credential = None
    for line in ROLLOUT.open():
        row = json.loads(line)
        payload = row.get('payload', {})
        if row.get('type') != 'response_item' or payload.get('type') != 'message' or payload.get('role') != 'user':
            continue
        message = ''.join(block.get('text', '') for block in payload.get('content', [])
                          if block.get('type') in ('input_text', 'text'))
        match = re.search(r'我给你dell 的sudo密码[:：]\s*(\d+)', message)
        if match:
            credential = match.group(1)
    if credential is None:
        raise RuntimeError('No explicit credential supplied in this thread')
    return credential


def device_by_serial(serial, vendor, product):
    matches = []
    for node in Path('/sys/bus/usb/devices').iterdir():
        try:
            if ((node / 'serial').read_text().strip() == serial and
                (node / 'idVendor').read_text().strip() == vendor and
                (node / 'idProduct').read_text().strip() == product):
                bus = int((node / 'busnum').read_text())
                dev = int((node / 'devnum').read_text())
                matches.append(Path(f'/dev/bus/usb/{bus:03d}/{dev:03d}'))
        except (OSError, ValueError):
            continue
    if len(matches) != 1:
        raise RuntimeError('Exact USB serial association unavailable')
    return matches[0]


def main():
    record = {'started_at_utc': datetime.now(timezone.utc).isoformat(),
              'credential_value_in_receipt': False,
              'persistent_credential_storage': None,
              'device_access': [], 'sudoers_or_group_change': False}
    credential = supplied_credential()
    try:
        os.environ['DBUS_SESSION_BUS_ADDRESS'] = 'unix:path=/run/user/1001/bus'
        connection = secretstorage.dbus_init()
        collection = secretstorage.get_default_collection(connection)
        if collection.is_locked():
            raise RuntimeError('Encrypted default collection is locked; no unlock prompt or plaintext fallback')
        collection.create_item('Codex Dell sudo', ATTRIBUTES, credential.encode(), replace=True)
        reference = Path('/home/xvhaocheng/.codex/credential-references/dell-sudo.json')
        reference.parent.mkdir(mode=0o700, exist_ok=True)
        reference.write_text(json.dumps({'backend': 'SecretService', 'attributes': ATTRIBUTES,
                                        'secret_value': 'stored only in encrypted OS credential collection',
                                        'reuse_authorized_by_user': True}, indent=2) + '\n')
        reference.chmod(0o600)
        record['persistent_credential_storage'] = {'backend': 'SecretService', 'stored': True,
                                                  'reference_file': str(reference), 'attributes': ATTRIBUTES}
    except Exception as error:
        record['persistent_credential_storage'] = {'stored': False, 'error_type': type(error).__name__,
                                                  'error': str(error).replace(credential, '[redacted]')}

    try:
        username = pwd.getpwuid(os.geteuid()).pw_name
        if username != 'xvhaocheng':
            raise RuntimeError('Unexpected local account; do not authorize another user')
        uart = Path('/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AV0KRCHT-if00-port0')
        device_by_serial('AV0KRCHT', '0403', '6001')
        jtag = device_by_serial('200300BD5148', '0403', '6010')
        devices = [uart.resolve(strict=True), jtag]
        for device in devices:
            before = subprocess.run(['getfacl', '-p', str(device)], capture_output=True, text=True, timeout=5)
            item = {'device': str(device), 'before_acl': before.stdout,
                    'before_read_write': os.access(device, os.R_OK | os.W_OK)}
            command = ['sudo', '-S', '-p', '', 'setfacl', '-m', f'u:{username}:rw', str(device)]
            result = subprocess.run(command, input=credential + '\n', capture_output=True, text=True, timeout=10)
            item.update({'command_without_credential': command, 'exit_code': result.returncode,
                         'stderr': result.stderr.replace(credential, '[redacted]'),
                         'after_read_write': os.access(device, os.R_OK | os.W_OK)})
            after = subprocess.run(['getfacl', '-p', str(device)], capture_output=True, text=True, timeout=5)
            item['after_acl'] = after.stdout
            record['device_access'].append(item)
            if result.returncode or not item['after_read_write']:
                raise RuntimeError('Scoped device authorization failed; no automatic retry')
        record['status'] = 'encrypted_credential_stored_and_scoped_access_granted'
    except Exception as error:
        record['status'] = 'authorization_incomplete'
        record['error'] = str(error).replace(credential, '[redacted]')
    finally:
        record['ended_at_utc'] = datetime.now(timezone.utc).isoformat()
        (HERE / 'scoped-device-authorization.json').write_text(json.dumps(record, ensure_ascii=False, indent=2) + '\n')
    print(json.dumps(record, ensure_ascii=False, indent=2))
    return 0 if record['status'].endswith('access_granted') else 2


if __name__ == '__main__':
    raise SystemExit(main())
