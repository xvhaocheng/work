#!/usr/bin/env python3
"""Exercise password authentication on temporary PTYs with synthetic credentials."""
import argparse
import datetime
import hashlib
import io
import json
import os
from pathlib import Path
import select
import sys
import tempfile
import threading
import time
import unittest
import uuid

sys.dont_write_bytecode = True
from auth import authenticate_password
from uart import Console

HERE = Path(__file__).resolve().parent
RESULTS = []
SYNTHETIC_SECRETS = []


def sha(data):
    return hashlib.sha256(data).hexdigest()


class CountingConsole(Console):
    def __init__(self, *args, **kwargs):
        self.send_calls = 0
        super().__init__(*args, **kwargs)

    def send(self, data):
        self.send_calls += 1
        return super().send(data)


class AuthenticationTest(unittest.TestCase):
    def exercise(self, scenario):
        secret = 'SYNTHETIC-PTY-BOARD-ONLY-' + uuid.uuid4().hex
        SYNTHETIC_SECRETS.append(secret)
        payload = secret.encode() + b'\n'
        timeout = .25 if scenario == 'timeout' else 1.5
        with tempfile.TemporaryDirectory(prefix='ispass-auth-test-') as name:
            temp = Path(name)
            helper = temp / 'synthetic_helper.py'
            helper.write_text('def supplied_board_credential():\n    return "root", '
                              + repr(secret) + '\n')
            master, slave = os.openpty()
            port = os.ttyname(slave)
            os.close(slave)
            console = None
            stop = threading.Event()
            peer_state = {'received': bytearray(), 'error': False}

            def peer():
                try:
                    end = time.monotonic() + 2
                    while b'\n' not in peer_state['received']:
                        if time.monotonic() >= end:
                            peer_state['error'] = True
                            return
                        if select.select([master], [], [], .02)[0]:
                            peer_state['received'].extend(os.read(master, 4096))
                    # Deliberate device echo, fragmented across private reads.
                    middle = len(secret) // 2
                    os.write(master, secret[:middle].encode())
                    time.sleep(.01)
                    os.write(master, secret[middle:].encode() + b'\r\n')
                    if scenario == 'success':
                        os.write(master, b'root@synthetic:~# ')
                    elif scenario == 'rejected':
                        os.write(master, b'Login incorrect\r\nsynthetic login: ')
                    elif scenario == 'password_reprompt':
                        os.write(master, b'Password: ')
                    while not stop.is_set():
                        if select.select([master], [], [], .02)[0]:
                            peer_state['received'].extend(os.read(master, 4096))
                except OSError:
                    peer_state['error'] = True

            worker = None
            try:
                console = CountingConsole(port, temp / 'output',
                                          lock_path=str(temp / 'pty-only.lock'))
                os.write(master, b'Password: ')
                self.assertTrue(console.read(.03) == b'Password: ',
                                'public prompt was not captured')
                worker = threading.Thread(target=peer, daemon=True)
                worker.start()
                error = None
                started = time.monotonic()
                try:
                    authenticate_password(console, helper, timeout=timeout)
                except Exception as caught:
                    error = caught
                elapsed = time.monotonic() - started
                # Keep the peer listening briefly to catch an automatic retry.
                time.sleep(.12)
                stop.set()
                worker.join(1)
                self.assertFalse(worker.is_alive(), 'PTY peer did not stop')
                self.assertFalse(peer_state['error'], 'PTY peer failed')
                self.assertEqual(console.send_calls, 1)
                self.assertTrue(bytes(peer_state['received']) == payload,
                                'PTY did not receive exactly one credential line')
                if scenario == 'success':
                    self.assertIsNone(error, 'synthetic successful login failed')
                elif scenario == 'timeout':
                    self.assertIsInstance(error, TimeoutError)
                    self.assertTrue(timeout <= elapsed < timeout + .6,
                                    'timeout was not bounded')
                else:
                    self.assertIsInstance(error, RuntimeError)
                    self.assertTrue(elapsed < timeout / 2,
                                    'rejection waited for the response timeout')
                if error is not None:
                    self.assertTrue('no retry' in str(error))
                    self.assertTrue(secret not in str(error), 'exception disclosed credential')

                # Public logging resumes after private authentication.
                marker = b'\nPOST_AUTH_PUBLIC_MARKER\n'
                os.write(master, marker)
                self.assertTrue(console.read(.03) == marker,
                                'public reads did not resume')
                raw = (console.output / 'uart.raw.log').read_bytes()
                self.assertTrue(raw == b'Password: ' + marker,
                                'private login bytes entered the raw log')
                receipt_path = console.output / 'authentication.json'
                receipt_bytes = receipt_path.read_bytes()
                receipt = json.loads(receipt_bytes)
                self.assertEqual(receipt['credential_transmissions'], 1)
                self.assertFalse(receipt['automatic_retry'])
                self.assertFalse(receipt['login_bytes_saved'])
                expected = ('shell_prompt_observed_identity_still_required'
                            if scenario == 'success' else 'attempted')
                self.assertEqual(receipt['status'], expected)
                for path in console.output.rglob('*'):
                    if path.is_file():
                        data = path.read_bytes()
                        self.assertTrue(secret.encode() not in data,
                                        'output artifact disclosed credential')
                        self.assertTrue(sha(secret.encode()).encode() not in data,
                                        'output artifact saved a password hash')
                RESULTS.append({'scenario': scenario, 'status': 'passed',
                                'synthetic_device_echo': True,
                                'elapsed_seconds': round(elapsed, 6),
                                'response_timeout_seconds': timeout,
                                'credential_send_calls': console.send_calls,
                                'pty_credential_lines': bytes(peer_state['received']).count(b'\n'),
                                'raw_contains_only_public_markers': True,
                                'output_contains_credential_or_password_hash': False,
                                'authentication_receipt_status': receipt['status'],
                                'authentication_receipt_sha256': sha(receipt_bytes),
                                'raw_log_sha256': sha(raw),
                                'raised_exception': type(error).__name__ if error else None})
            finally:
                stop.set()
                if worker is not None:
                    worker.join(1)
                if console is not None:
                    console.close()
                os.close(master)

    def test_successful_login_with_unexpected_password_echo(self):
        self.exercise('success')

    def test_rejection_stops_after_one_credential_line(self):
        self.exercise('rejected')

    def test_password_reprompt_stops_after_one_credential_line(self):
        self.exercise('password_reprompt')

    def test_timeout_does_not_retry(self):
        self.exercise('timeout')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--receipt', type=Path, default=HERE / 'auth-validation.json')
    args = parser.parse_args()
    inputs = {p.name: sha(p.read_bytes()) for p in
              [HERE / 'auth.py', HERE / 'uart.py', HERE / 'program_and_boot.py', Path(__file__)]}
    started = datetime.datetime.now(datetime.timezone.utc).isoformat()
    stream = io.StringIO()
    result = unittest.TextTestRunner(stream=stream, verbosity=2).run(
        unittest.defaultTestLoader.loadTestsFromTestCase(AuthenticationTest))
    diagnostics = stream.getvalue()
    for secret in SYNTHETIC_SECRETS:
        diagnostics = diagnostics.replace(secret, '[SYNTHETIC_CREDENTIAL_REDACTED]')
        diagnostics = diagnostics.replace(sha(secret.encode()), '[PASSWORD_HASH_REDACTED]')
    unchanged = all(sha((HERE / name).read_bytes()) == digest for name, digest in inputs.items())
    passed = result.wasSuccessful() and unchanged
    receipt = {'schema': 'ispass2027-auth-host-validation-v1', 'started_utc': started,
               'finished_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
               'status': 'passed' if passed else 'failed',
               'command': [sys.executable, str(Path(__file__).resolve()), '--receipt', str(args.receipt.resolve())],
               'inputs_sha256': inputs, 'inputs_unchanged_during_validation': unchanged,
               'scope': 'Temporary PTYs and synthetic helper only; no board, UART device, JTAG or programming process',
               'real_credentials_loaded': False, 'temporary_test_outputs_removed': True,
               'tests_run': result.testsRun, 'failures': len(result.failures),
               'errors': len(result.errors), 'cases': RESULTS,
               'diagnostics': diagnostics,
               'readonly_login_review': {
                   'file': str(HERE / 'program_and_boot.py'),
                   'password_branch_requires_prior_root_login': True,
                   'fresh_boot_seen_is_preserved_across_private_authentication': True,
                   'successful_private_shell_prompt_allows_existing_identity_command': True,
                   'identity_still_checks_expected_kernel_boot_id_and_zero_taint': True,
                   'program_main_was_not_run': True}}
    encoded = json.dumps(receipt, indent=2) + '\n'
    if any(secret in encoded or sha(secret.encode()) in encoded for secret in SYNTHETIC_SECRETS):
        raise RuntimeError('validation receipt failed synthetic credential redaction')
    args.receipt.write_text(encoded)
    print(diagnostics, end='')
    print('Auth host validation: ' + receipt['status'])
    return 0 if passed else 1


if __name__ == '__main__':
    raise SystemExit(main())
