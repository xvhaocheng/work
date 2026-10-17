"""Protocol checks on a pseudo terminal; no FPGA or SD access."""
import hashlib
import os
from pathlib import Path
import pty
import re
import tempfile
import threading
import unittest
import json
from uart import Console


class UartProtocolTest(unittest.TestCase):
    def exercise(self, upload=False):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            master, slave = pty.openpty()
            port = os.ttyname(slave)
            source = root/'input.bin'
            source.write_bytes(bytes(range(256))*256)
            errors = []

            def board():
                try:
                    for index in range(3 if upload else 1):
                        line = b''
                        while not line.endswith(b'\n'):
                            line += os.read(master, 1)
                        text = line.decode()
                        if 'head -c ' in text:
                            ready = re.search(r'__ISPASS_UPLOAD_READY_[0-9a-f]+__', text)[0]
                            end = re.search(r'__ISPASS_UPLOAD_END_[0-9a-f]+__', text)[0]
                            size = int(re.search(r'head -c (\d+)', text)[1])
                            os.write(master, ('\n'+ready+'\n').encode())
                            body = b''
                            while len(body) < size:
                                body += os.read(master, min(4096, size-len(body)))
                            self.assertEqual(body, source.read_bytes())
                            os.write(master, ('\n'+end+':0\n'+hashlib.sha256(body).hexdigest()+'  /tmp/uart-protocol-test.partial\n').encode())
                        else:
                            begin = re.search(r'__ISPASS_BEGIN_[0-9a-f]+__', text)[0]
                            end = re.search(r'__ISPASS_END_[0-9a-f]+__', text)[0]
                            # Include an echoed command containing both marker strings.
                            os.write(master, line+('\n'+begin+'\nexpected-output\n'+end+':0\n').encode())
                except BaseException as error:
                    errors.append(error)

            thread = threading.Thread(target=board, daemon=True)
            console = Console(port, root/'log', str(root/'lock'))
            thread.start()
            try:
                if upload:
                    result = console.put_file(source, '/tmp/uart-protocol-test', 10, verify=True)
                    self.assertTrue(result['remote_sha256_verified'])
                    self.assertFalse(result['sd_written'])
                else:
                    result = console.command('echo expected-output', 2)
                    self.assertEqual(result['stdout'], 'expected-output\n')
                    self.assertEqual(result['exit_code'], 0)
            finally:
                console.close()
                thread.join(3)
                os.close(master)
                os.close(slave)
            self.assertFalse(thread.is_alive())
            self.assertEqual(errors, [])

    def test_echo_cannot_satisfy_marker(self):
        self.exercise()

    def test_binary_transfer_requires_exact_length_and_sha(self):
        self.exercise(upload=True)

    def test_timeout_keeps_received_bytes_and_does_not_retry(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            master, slave = pty.openpty()
            console = Console(os.ttyname(slave), root/'log', str(root/'lock'))
            sent = []
            def incomplete_board():
                line = b''
                while not line.endswith(b'\n'):
                    line += os.read(master, 1)
                sent.append(line)
                begin = re.search(rb'__ISPASS_BEGIN_[0-9a-f]+__', line)[0]
                os.write(master, b'\n'+begin+b'\nwrite still running\n')
            thread = threading.Thread(target=incomplete_board, daemon=True)
            thread.start()
            try:
                result = console.command('long-running-operation', .3)
                self.assertIsNone(result['exit_code'])
                self.assertEqual(result['error'], 'timeout')
                self.assertIn('write still running', result['received'])
                receipts = list((root/'log').glob('command-*.json'))
                self.assertEqual(len(receipts), 1)
                self.assertEqual(json.loads(receipts[0].read_text()), result)
                self.assertIn(b'write still running', (root/'log'/'uart.raw.log').read_bytes())
                self.assertEqual(len(sent), 1)
            finally:
                console.close()
                thread.join(2)
                os.close(master)
                os.close(slave)


if __name__ == '__main__':
    unittest.main()
