import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

TESTS_DIR = Path(__file__).resolve().parent
BOARD_FETCH = TESTS_DIR / 'board-fetch-update.sh'
BOARD_NET = TESTS_DIR / 'board-net-up.sh'

FAKE_WGET = r'''
import json
import os
import sys

log = os.environ.get('FAKE_LOG')
if log:
    with open(log, 'a', encoding='utf-8') as f:
        print(json.dumps({'cmd': 'wget', 'argv': sys.argv[1:]}), file=f)

args = sys.argv[1:]
out = None
for i, arg in enumerate(args):
    if arg == '-O' and i + 1 < len(args):
        out = args[i + 1]

mode = os.environ.get('FAKE_WGET_MODE', 'ok')
if out:
    if mode == 'fail':
        with open(out, 'wb') as f:
            f.write(b'partial')
        sys.exit(int(os.environ.get('FAKE_WGET_EXIT', '1')))
    if mode == 'empty':
        with open(out, 'wb') as f:
            pass
        sys.exit(0)
    if mode == 'oversized':
        size = int(os.environ.get('FAKE_WGET_SIZE', '33554433'))
        with open(out, 'wb') as f:
            f.truncate(size)
        sys.exit(0)
    data = os.environ.get('FAKE_WGET_DATA', 'firmware').encode()
    with open(out, 'wb') as f:
        f.write(data)
sys.exit(int(os.environ.get('FAKE_WGET_EXIT', '0')))
'''

FAKE_SD_UPDATE = r'''
import json
import os
import sys

log = os.environ.get('FAKE_LOG')
if log:
    with open(log, 'a', encoding='utf-8') as f:
        print(json.dumps({'cmd': 'platform-sd-update', 'argv': sys.argv[1:]}), file=f)
sys.exit(int(os.environ.get('FAKE_SD_UPDATE_EXIT', '0')))
'''

FAKE_IP = r'''
import json
import os
import sys

log = os.environ.get('FAKE_LOG')
if log:
    with open(log, 'a', encoding='utf-8') as f:
        print(json.dumps({'cmd': 'ip', 'argv': sys.argv[1:]}), file=f)

args = sys.argv[1:]

def state_path():
    return os.environ.get('FAKE_IP_STATE', '')

if len(args) >= 4 and args[0] == 'link' and args[1] == 'show' and args[2] == 'dev':
    ifname = args[3]
    if os.environ.get('FAKE_IP_LINK_MISSING') == ifname:
        sys.exit(1)
    sys.exit(0)

if len(args) >= 5 and args[0] == 'addr' and args[1] == 'replace' and args[3] == 'dev':
    addr = args[2]
    dev = args[4]
    path = state_path()
    if path:
        with open(path, 'w', encoding='utf-8') as f:
            print(addr, file=f)
            print(dev, file=f)
    sys.exit(0)

if len(args) >= 2 and args[0] == 'route' and args[1] == 'replace':
    sys.exit(0)

if len(args) >= 4 and args[0] == '-o' and args[1] == 'addr' and args[2] == 'show' and args[3] == 'dev':
    ifname = args[4] if len(args) > 4 else ''
    report = os.environ.get('FAKE_IP_REPORT_ADDR', '1') == '1'
    path = state_path()
    addr = ''
    dev = ''
    if path and os.path.exists(path):
        with open(path, 'r', encoding='utf-8') as f:
            lines = f.read().splitlines()
        if len(lines) >= 2:
            addr = lines[0]
            dev = lines[1]
    if report and addr and (not dev or dev == ifname):
        print('2: %s    inet %s brd 0.0.0.0 scope global %s' % (ifname, addr, ifname))
    sys.exit(0)

sys.exit(0)
'''

class BoardUpdateHelperTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.tmp_path = Path(self.tmp.name)
        self.bin_dir = self.tmp_path / 'bin'
        self.bin_dir.mkdir()
        self.log_path = self.tmp_path / 'fake.log'
        self.state_path = self.tmp_path / 'ip-state'
        self.env = os.environ.copy()
        self.env['PATH'] = str(self.bin_dir) + os.pathsep + self.env.get('PATH', '')
        self.env['FAKE_LOG'] = str(self.log_path)
        self.env['FAKE_IP_STATE'] = str(self.state_path)

    def _write_fake(self, name, code):
        path = self.bin_dir / name
        path.write_text('#!' + sys.executable + os.linesep + code + os.linesep, encoding='utf-8')
        path.chmod(0o755)

    def _run(self, script, args, extra_env=None, timeout=30):
        env = self.env.copy()
        if extra_env:
            env.update(extra_env)
        return subprocess.run(
            ['sh', str(script)] + list(args),
            env=env,
            text=True,
            capture_output=True,
            timeout=timeout,
        )

    def _log(self):
        if not self.log_path.exists():
            return []
        entries = []
        for line in self.log_path.read_text(encoding='utf-8').splitlines():
            if line.strip():
                entries.append(json.loads(line))
        return entries

    def _commands(self, name):
        return [entry['argv'] for entry in self._log() if entry.get('cmd') == name]

    def test_fetch_default_is_dry_run(self):
        self._write_fake('wget', FAKE_WGET)
        self._write_fake('platform-sd-update', FAKE_SD_UPDATE)
        output = self.tmp_path / 'fw.bin'
        proc = self._run(BOARD_FETCH, ['http://example.invalid/fw.bin', str(output)])
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertTrue(output.exists())
        self.assertEqual(output.read_bytes(), b'firmware')
        self.assertFalse((self.tmp_path / 'fw.bin.partial').exists())
        sd = self._commands('platform-sd-update')
        self.assertEqual(len(sd), 1)
        self.assertEqual(sd[0], ['--device', '/dev/mmcblk0p1', '--image', str(output), '--dry-run'])
        wget = self._commands('wget')
        self.assertEqual(len(wget), 1)
        self.assertIn('http://example.invalid/fw.bin', wget[0])
        self.assertIn('DRY-RUN', proc.stdout)

    def test_fetch_write_forwards_write_and_device(self):
        self._write_fake('wget', FAKE_WGET)
        self._write_fake('platform-sd-update', FAKE_SD_UPDATE)
        output = self.tmp_path / 'fw.bin'
        proc = self._run(BOARD_FETCH, ['http://example.invalid/fw.bin', str(output), '--write', '--device', '/dev/fake0'])
        self.assertEqual(proc.returncode, 0, proc.stderr)
        sd = self._commands('platform-sd-update')
        self.assertEqual(sd, [['--device', '/dev/fake0', '--image', str(output)]])
        self.assertIn('update applied', proc.stdout)

    def test_fetch_download_failure_never_updates_and_removes_partial(self):
        self._write_fake('wget', FAKE_WGET)
        self._write_fake('platform-sd-update', FAKE_SD_UPDATE)
        output = self.tmp_path / 'fw.bin'
        partial = self.tmp_path / 'fw.bin.partial'
        partial.write_bytes(b'stale')
        proc = self._run(
            BOARD_FETCH,
            ['http://example.invalid/fw.bin', str(output)],
            extra_env={'FAKE_WGET_MODE': 'fail', 'FAKE_WGET_EXIT': '7'},
        )
        self.assertNotEqual(proc.returncode, 0)
        self.assertFalse(output.exists())
        self.assertFalse(partial.exists())
        self.assertEqual(self._commands('platform-sd-update'), [])
        self.assertIn('download failed', proc.stderr)

    def test_fetch_empty_download_never_updates(self):
        self._write_fake('wget', FAKE_WGET)
        self._write_fake('platform-sd-update', FAKE_SD_UPDATE)
        output = self.tmp_path / 'fw.bin'
        proc = self._run(
            BOARD_FETCH,
            ['http://example.invalid/fw.bin', str(output)],
            extra_env={'FAKE_WGET_MODE': 'empty'},
        )
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertFalse(output.exists())
        self.assertFalse((self.tmp_path / 'fw.bin.partial').exists())
        self.assertEqual(self._commands('platform-sd-update'), [])
        self.assertIn('empty', proc.stderr)

    def test_fetch_oversized_download_never_updates(self):
        self._write_fake('wget', FAKE_WGET)
        self._write_fake('platform-sd-update', FAKE_SD_UPDATE)
        output = self.tmp_path / 'fw.bin'
        proc = self._run(
            BOARD_FETCH,
            ['http://example.invalid/fw.bin', str(output)],
            extra_env={'FAKE_WGET_MODE': 'oversized', 'FAKE_WGET_SIZE': '33554433'},
        )
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertFalse(output.exists())
        self.assertFalse((self.tmp_path / 'fw.bin.partial').exists())
        self.assertEqual(self._commands('platform-sd-update'), [])
        self.assertIn('larger than', proc.stderr)

    def test_fetch_rejects_non_http_urls_without_invoking_helpers(self):
        self._write_fake('wget', FAKE_WGET)
        self._write_fake('platform-sd-update', FAKE_SD_UPDATE)
        output = self.tmp_path / 'fw.bin'
        for url in ['ftp://example.invalid/fw.bin', 'https://example.invalid/fw.bin']:
            with self.subTest(url=url):
                if self.log_path.exists():
                    self.log_path.unlink()
                proc = self._run(BOARD_FETCH, [url, str(output)])
                self.assertEqual(proc.returncode, 2, proc.stderr)
                self.assertEqual(self._log(), [])
                self.assertFalse(output.exists())

    def test_fetch_propagates_updater_failure(self):
        self._write_fake('wget', FAKE_WGET)
        self._write_fake('platform-sd-update', FAKE_SD_UPDATE)
        output = self.tmp_path / 'fw.bin'
        proc = self._run(
            BOARD_FETCH,
            ['http://example.invalid/fw.bin', str(output)],
            extra_env={'FAKE_SD_UPDATE_EXIT': '9'},
        )
        self.assertEqual(proc.returncode, 9, proc.stderr)
        self.assertTrue(output.exists())
        sd = self._commands('platform-sd-update')
        self.assertEqual(len(sd), 1)
        self.assertIn('--dry-run', sd[0])
        self.assertNotIn('update applied', proc.stdout)

    def test_fetch_removes_stale_partial_on_success(self):
        self._write_fake('wget', FAKE_WGET)
        self._write_fake('platform-sd-update', FAKE_SD_UPDATE)
        output = self.tmp_path / 'fw.bin'
        partial = self.tmp_path / 'fw.bin.partial'
        partial.write_bytes(b'stale')
        proc = self._run(BOARD_FETCH, ['http://example.invalid/fw.bin', str(output)])
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertFalse(partial.exists())
        self.assertEqual(output.read_bytes(), b'firmware')

    def test_net_up_dry_run_does_not_change_state(self):
        self._write_fake('ip', FAKE_IP)
        proc = self._run(BOARD_NET, ['eth0', '--addr', '192.0.2.10/24'])
        self.assertEqual(proc.returncode, 0, proc.stderr)
        cmds = self._commands('ip')
        self.assertIn(['link', 'show', 'dev', 'eth0'], cmds)
        self.assertIn(['-o', 'addr', 'show', 'dev', 'eth0'], cmds)
        for argv in cmds:
            self.assertNotIn(argv[:2], [['link', 'set'], ['addr', 'replace'], ['route', 'replace']])
        self.assertIn('DRY-RUN', proc.stdout)

    def test_net_up_apply_touches_only_requested_interface_and_address(self):
        self._write_fake('ip', FAKE_IP)
        proc = self._run(
            BOARD_NET,
            ['eth0', '--addr', '192.0.2.10/24', '--gw', '192.0.2.1', '--apply'],
        )
        self.assertEqual(proc.returncode, 0, proc.stderr)
        cmds = self._commands('ip')
        expected = [
            ['link', 'show', 'dev', 'eth0'],
            ['-o', 'addr', 'show', 'dev', 'eth0'],
            ['link', 'set', 'dev', 'eth0', 'up'],
            ['addr', 'replace', '192.0.2.10/24', 'dev', 'eth0'],
            ['route', 'replace', 'default', 'via', '192.0.2.1', 'dev', 'eth0'],
            ['-o', 'addr', 'show', 'dev', 'eth0'],
        ]
        self.assertEqual(cmds, expected)
        self.assertIn('PASS', proc.stdout)

    def test_net_up_readback_failure_stops_pass(self):
        self._write_fake('ip', FAKE_IP)
        proc = self._run(
            BOARD_NET,
            ['eth0', '--addr', '192.0.2.10/24', '--apply'],
            extra_env={'FAKE_IP_REPORT_ADDR': '0'},
        )
        self.assertNotEqual(proc.returncode, 0)
        self.assertIn('FAIL', proc.stderr)
        self.assertNotIn('PASS', proc.stdout)
        self.assertNotIn('PASS', proc.stderr)


if __name__ == '__main__':
    unittest.main()
