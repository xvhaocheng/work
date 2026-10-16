#!/usr/bin/env python3
"""One bounded login using the user's board credential, with no login logging."""
import json
from pathlib import Path
import re
import subprocess
import time
from datetime import datetime, timezone
from read_current_board_identity import PosixTransport, PORT, HERE

ROLLOUT = Path('/home/xvhaocheng/.codex/sessions/2026/10/12/rollout-2026-10-12T16-24-37-01a12eff-c971-7c62-8071-a08b646a4b61.jsonl')


def supplied_board_credential():
    selected = None
    for line in ROLLOUT.open():
        row = json.loads(line)
        p = row.get('payload', {})
        if row.get('type') != 'response_item' or p.get('type') != 'message' or p.get('role') != 'user':
            continue
        message = ''.join(x.get('text', '') for x in p.get('content', []) if x.get('type') in ('input_text', 'text'))
        if '<send_user_message_question_reply>' not in message:
            continue
        try:
            answers = json.loads(message.split('<send_user_message_question_reply>', 1)[1].split('</send_user_message_question_reply>', 1)[0])
        except (ValueError, IndexError):
            continue
        for answer in answers:
            if str(answer.get('question', '')).startswith('板端串口目前停在登录密码提示'):
                pair = str(answer.get('answer', '')).split(',', 1)
                if len(pair) == 2:
                    selected = tuple(x.strip() for x in pair)
    if selected is None or not all(selected):
        raise RuntimeError('Explicit board login reply unavailable')
    return selected


def main():
    username, password = supplied_board_credential()
    record = {'started_at_utc': datetime.now(timezone.utc).isoformat(),
              'password_in_argv_environment_logs_or_receipt': False,
              'credential_transmissions': 0, 'login_raw_bytes_saved': False,
              'states': [], 'automatic_retry': False, 'flash_reset_reboot': False}
    transport = None
    buffer = bytearray()

    def receive(seconds):
        deadline = time.monotonic() + seconds
        while time.monotonic() < deadline:
            data = transport.read(4096)
            if data:
                buffer.extend(data)
        return bytes(buffer[-8192:]).decode(errors='replace')

    def state(text):
        if re.search(r'(?:^|[\r\n])[^\r\n]*login:\s*$', text): return 'login_prompt'
        if re.search(r'Password:\s*$', text): return 'password_prompt'
        if re.search(r'(?:^|[\r\n])[^\r\n]*#\s*$', text): return 'shell_prompt'
        return 'unrecognized'

    try:
        owner = subprocess.run(['fuser', str(PORT.resolve())], capture_output=True, timeout=5)
        if owner.returncode != 1 or owner.stdout.strip() or owner.stderr.strip():
            raise RuntimeError('UART ownership unavailable; do not take over')
        transport = PosixTransport(str(PORT), 115200, 0.1)
        current = state(receive(1))
        if current == 'unrecognized':
            transport.write(b'\n')
            current = state(receive(1))
        record['states'].append(current)
        if current == 'password_prompt':
            # Cancel the unknown prior username without supplying its password.
            transport.write(b'\x04')
            buffer.clear()
            current = state(receive(7))
            record['states'].append(current)
        if current == 'login_prompt':
            buffer.clear()
            transport.write(username.encode() + b'\n')
            current = state(receive(2))
            record['states'].append(current)
            if current == 'password_prompt':
                buffer.clear()
                # Direct transport write: never call the console's event logger.
                transport.write(password.encode() + b'\n')
                record['credential_transmissions'] = 1
                current = state(receive(3))
                record['states'].append(current)
        record['shell_prompt_seen'] = current == 'shell_prompt'
        record['status'] = 'shell_prompt_observed_identity_still_required' if record['shell_prompt_seen'] else 'login_not_accepted'
    except Exception as error:
        record['status'] = 'login_not_accepted'
        record['error'] = str(error).replace(password, '[redacted]')
    finally:
        if transport is not None:
            transport.close()
        buffer.clear()
        record['ended_at_utc'] = datetime.now(timezone.utc).isoformat()
        (HERE / 'board-login-receipt.json').write_text(json.dumps(record, indent=2) + '\n')
    print(json.dumps(record, indent=2))
    return 0 if record.get('shell_prompt_seen') else 2


if __name__ == '__main__':
    raise SystemExit(main())
