#!/usr/bin/env python3
"""Host-only packaging regression; does not build or run FPGA software."""
import contextlib
import importlib.util
import io
from pathlib import Path
import stat
import tempfile
import unittest


SCRIPT = Path(__file__).resolve().parents[1] / 'scripts/rootfs.py'
SPEC = importlib.util.spec_from_file_location('platform_rootfs', SCRIPT)
rootfs = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(rootfs)


class RootfsPackagingTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        for directory in ('assets', 'out/bench', 'tests'):
            (self.root / directory).mkdir(parents=True)
        # Create every fixture straight from the packaging manifests so that a
        # newly registered tool cannot be forgotten here.
        for name, source in rootfs.EXECUTABLES.items():
            path = self.root / source
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(('fixture for ' + name).encode())
        for packed_path, source in rootfs.DATA.items():
            path = self.root / source
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(('fixture for ' + packed_path).encode())
        # These old and unrelated artifacts must remain on disk but never be packed.
        for name in sorted(rootfs.LEGACY_NAMES | {'unexpected-program'}):
            (self.root / 'out/bench' / name).write_bytes(b'historical output')
        self.device = [1, stat.S_IFCHR | 0o600, 0, 0, 1, 0, 0, 0, 0, 240, 0, 0, 0]
        self.file = [2, stat.S_IFREG | 0o644, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0]

    def base(self, extra=()):
        data = bytearray()
        for name, fields, body in [
            ('dev/idma0', self.device, b''),
            ('etc/banner', self.file, b'preserve this file'),
            *extra,
        ]:
            rootfs.write_entry(data, name, fields, body)
        rootfs.write_entry(data, 'TRAILER!!!', [0] * 13, b'')
        (self.root / 'assets/rootfs-base.cpio').write_bytes(data)

    def pack(self):
        with contextlib.redirect_stdout(io.StringIO()):
            rootfs.repack(self.root)
        return list(rootfs.read_newc((self.root / 'out/rootfs.cpio').read_bytes()))

    def test_replaces_all_managed_copies_and_ignores_stale_outputs(self):
        names = list(rootfs.LEGACY_NAMES)
        for name in rootfs.EXECUTABLES:
            names += [f'usr/bin/{name}', f'./usr/bin/{name}', name]
        for packed_path in rootfs.DATA:
            parent, _, leaf = packed_path.rpartition('/')
            names += [packed_path, f'./{packed_path}']
            if parent:
                names.append(f'{parent}/./{leaf}')
            else:
                names.append(f'././{leaf}')
        self.base([(name, self.file, b'old copy') for name in names])
        entries = self.pack()
        packed = {name: (fields, body) for name, fields, body in entries}
        self.assertEqual(len(entries), len(packed))
        expected = {'dev/idma0', 'etc/banner'}
        expected |= {f'usr/bin/{name}' for name in rootfs.EXECUTABLES}
        expected |= set(rootfs.DATA)
        self.assertEqual(set(packed), expected)
        for name, source in rootfs.EXECUTABLES.items():
            fields, body = packed['usr/bin/' + name]
            self.assertEqual(fields[1], stat.S_IFREG | 0o755)
            self.assertEqual(body, (self.root / source).read_bytes())
        for path, source in rootfs.DATA.items():
            fields, body = packed[path]
            self.assertEqual(fields[1], stat.S_IFREG | 0o644)
            self.assertEqual(body, (self.root / source).read_bytes())
        device_fields, device_body = packed['dev/idma0']
        self.assertEqual(device_fields[1:11], self.device[1:11])
        self.assertEqual(device_body, b'')
        self.assertEqual(packed['etc/banner'][1], b'preserve this file')
        self.assertEqual((self.root / 'out/bench/idma_once').read_bytes(), b'historical output')

    def test_unrelated_data_basename_preserved(self):
        self.base([('var/platform-research-plan.tsv', self.file, b'unrelated data copy')])
        entries = self.pack()
        packed = {name: (fields, body) for name, fields, body in entries}
        self.assertEqual(packed['var/platform-research-plan.tsv'][1], b'unrelated data copy')
        self.assertEqual(packed['etc/platform-research-plan.tsv'][1],
                         (self.root / 'tests/research-plan.tsv').read_bytes())

    def test_missing_current_binary_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'out/bench/idma_experiment').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_missing_controlled_binary_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'out/bench/idma_e3_controlled').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_missing_controlled_runner_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'tests/board-e3-controlled.sh').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_missing_watchdog_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'out/bench/platform-timeout').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_missing_p4m_runner_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'tests/board-p4m.sh').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_missing_p4m_start_runner_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'tests/board-p4m-start.sh').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_missing_research_binary_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'out/bench/idma_research').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_missing_research_runner_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'tests/board-research.sh').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_missing_research_plan_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'tests/research-plan.tsv').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_unrelated_duplicate_rejected_before_output_replaced(self):
        self.base([('./etc/banner', self.file, b'ambiguous duplicate')])
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        with self.assertRaisesRegex(ValueError, 'Duplicate rootfs entry'):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_repacking_current_image_does_not_duplicate_entrypoints(self):
        self.base()
        self.pack()
        data = (self.root / 'out/rootfs.cpio').read_bytes()
        (self.root / 'assets/rootfs-base.cpio').write_bytes(data)
        entries = self.pack()
        for name in rootfs.EXECUTABLES:
            self.assertEqual(sum(Path(path).name == name for path, _, _ in entries), 1)
        for path in rootfs.DATA:
            self.assertEqual(sum(str(Path(name)) == path for name, _, _ in entries), 1)


if __name__ == '__main__':
    unittest.main()
