#!/usr/bin/env python3
"""Host-only regression for bench/platform_sd_update.c.

The tool is compiled twice: a production binary without test hooks and a
test binary built with -DPLATFORM_SD_UPDATE_TEST. Every case runs against a
synthetic sparse 80 MiB GPT image in a temporary directory: no board, no
root and no real device are involved.
"""
import fcntl
import hashlib
import os
import shutil
import struct
import subprocess
import tempfile
import unittest
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / 'bench/platform_sd_update.c'

SECTOR = 512
START_LBA = 2048
PART_SECTORS = 65536
PART_OFFSET = START_LBA * SECTOR
PART_SIZE = PART_SECTORS * SECTOR
SECOND_LBA = 69632
SECOND_SECTORS = 65536
DISK_SIZE = 80 * 1024 * 1024

TYPE_GUID = '7412F7D5-A156-4B13-81DC-867174929325'
UNIQUE_GUID = '46298C61-BAC2-44C9-A8FF-8FA61CF2BEE0'
DISK_GUID = '0B5A91A8-0F2C-4E6F-9B0A-1234567890AB'
OTHER_TYPE_GUID = '0FC63DAF-8483-4772-8E79-3D69D8477DE4'
OTHER_UNIQUE_GUID = 'AAAAAAAA-BBBB-CCCC-DDDD-EEEEEEEEEEEE'


def guid_bytes(text):
    fields = text.split('-')
    return (struct.pack('<I', int(fields[0], 16)) +
            struct.pack('<H', int(fields[1], 16)) +
            struct.pack('<H', int(fields[2], 16)) +
            bytes.fromhex(fields[3]) +
            bytes.fromhex(fields[4]))


def build_entry(type_guid, unique_guid, first_lba, last_lba, name='boot'):
    entry = bytearray(128)
    entry[0:16] = guid_bytes(type_guid)
    entry[16:32] = guid_bytes(unique_guid)
    struct.pack_into('<Q', entry, 32, first_lba)
    struct.pack_into('<Q', entry, 40, last_lba)
    raw = name.encode('utf-16-le')[:70]
    entry[56:56 + len(raw)] = raw
    return bytes(entry)


def make_disk(path, *, disk_size=DISK_SIZE, entry0=None, entry1=None,
              entries_lba=2, entry_count=128, entry_size=128,
              signature=b'EFI PART', revision=0x00010000, current_lba=1,
              header_size=92, disk_guid=DISK_GUID):
    if entry0 is None:
        entry0 = build_entry(TYPE_GUID, UNIQUE_GUID, START_LBA, START_LBA + PART_SECTORS - 1)
    if entry1 is None:
        entry1 = build_entry(OTHER_TYPE_GUID, OTHER_UNIQUE_GUID,
                             SECOND_LBA, SECOND_LBA + SECOND_SECTORS - 1, 'data')
    entries = bytearray(entry_count * entry_size)
    entries[0:128] = entry0
    entries[128:256] = entry1
    entries_crc = zlib.crc32(bytes(entries)) & 0xFFFFFFFF
    disk_sectors = disk_size // SECTOR

    def build_header(entries_lba_value, current_lba_value, backup_lba_value):
        header = bytearray(512)
        header[0:8] = signature
        struct.pack_into('<I', header, 8, revision)
        struct.pack_into('<I', header, 12, header_size)
        struct.pack_into('<I', header, 20, 0)
        struct.pack_into('<Q', header, 24, current_lba_value)
        struct.pack_into('<Q', header, 32, backup_lba_value)
        struct.pack_into('<Q', header, 40, 34)
        struct.pack_into('<Q', header, 48, disk_sectors - 34)
        header[56:72] = guid_bytes(disk_guid)
        struct.pack_into('<Q', header, 72, entries_lba_value)
        struct.pack_into('<I', header, 80, entry_count)
        struct.pack_into('<I', header, 84, entry_size)
        struct.pack_into('<I', header, 88, entries_crc)
        struct.pack_into('<I', header, 16,
                         zlib.crc32(bytes(header[:header_size])) & 0xFFFFFFFF)
        return header

    primary_header = build_header(entries_lba, current_lba, disk_sectors - 1)
    backup_entries_sectors = (len(entries) + SECTOR - 1) // SECTOR
    backup_entries_lba = disk_sectors - 1 - backup_entries_sectors
    backup_header = build_header(backup_entries_lba, disk_sectors - 1, 1)
    entries_offset = entries_lba * SECTOR
    entries_fit = 0 <= entries_lba and entries_offset + len(entries) <= disk_size
    with open(path, 'wb') as handle:
        handle.truncate(disk_size)
        handle.write(bytes((offset * 31 + 7) & 0xFF for offset in range(4096)))
        handle.seek(SECTOR)
        handle.write(bytes(primary_header))
        if entries_fit:
            handle.seek(entries_offset)
            handle.write(bytes(entries))
        if (SECOND_LBA + 8) * SECTOR <= disk_size:
            handle.seek(SECOND_LBA * SECTOR)
            handle.write(bytes((offset * 13 + 5) & 0xFF for offset in range(4096)))
        handle.seek(backup_entries_lba * SECTOR)
        handle.write(bytes(entries))
        handle.seek((disk_sectors - 1) * SECTOR)
        handle.write(bytes(backup_header))
    return Path(path)


def run_tool(binary, *arguments):
    return subprocess.run([str(binary), *map(str, arguments)],
                          capture_output=True, text=True)


class PlatformSdUpdateTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = os.environ.get('CC') or shutil.which('cc') or shutil.which('gcc')
        if compiler is None:
            raise unittest.SkipTest('no host C compiler available')
        cls.workdir = Path(tempfile.mkdtemp(prefix='platform-sd-update-'))
        cls.addClassCleanup(shutil.rmtree, cls.workdir, ignore_errors=True)
        cls.production = cls.workdir / 'platform-sd-update'
        cls.test_binary = cls.workdir / 'platform-sd-update-test'
        cls.compile_binary(compiler, cls.production, [])
        cls.compile_binary(compiler, cls.test_binary, ['-DPLATFORM_SD_UPDATE_TEST'])

    @staticmethod
    def compile_binary(compiler, output, extra):
        command = [compiler, '-O2', '-Wall', '-Wextra', '-std=gnu11',
                   *extra, str(SOURCE), '-o', str(output)]
        completed = subprocess.run(command, capture_output=True, text=True)
        if completed.returncode != 0:
            raise AssertionError('compile failed: %s\n%s' % (' '.join(command), completed.stderr))

    def setUp(self):
        self.dir = Path(tempfile.mkdtemp(prefix='sd-update-case-'))
        self.addCleanup(shutil.rmtree, self.dir, ignore_errors=True)
        self.disk = self.dir / 'disk.img'
        self.image = self.dir / 'fw_payload.bin'

    def write_payload(self, size):
        data = bytes((index * 17 + 3) & 0xFF for index in range(size))
        self.image.write_bytes(data)
        return data

    def assert_rejected(self, *extra, message=None):
        before = hashlib.sha256(self.disk.read_bytes()).hexdigest()
        completed = run_tool(self.test_binary, '--test-disk', self.disk,
                             '--image', self.image, *extra)
        self.assertNotEqual(completed.returncode, 0, completed.stdout)
        self.assertNotIn('PASS', completed.stdout)
        if message is not None:
            self.assertIn(message, completed.stderr)
        after = hashlib.sha256(self.disk.read_bytes()).hexdigest()
        self.assertEqual(after, before, 'the rejected run modified the disk')
        return completed

    def test_dry_run_validates_and_leaves_the_disk_unchanged(self):
        make_disk(self.disk)
        self.write_payload(1024 * 1024)
        before = self.disk.read_bytes()
        completed = run_tool(self.test_binary, '--test-disk', self.disk,
                             '--image', self.image, '--dry-run')
        self.assertEqual(completed.returncode, 0, completed.stderr)
        self.assertIn('DRY-RUN', completed.stdout)
        self.assertEqual(self.disk.read_bytes(), before)

    def test_write_pads_with_zeros_and_preserves_the_rest_of_the_disk(self):
        make_disk(self.disk)
        payload = self.write_payload(3 * 1024 * 1024)
        before = self.disk.read_bytes()
        completed = run_tool(self.test_binary, '--test-disk', self.disk,
                             '--image', self.image)
        self.assertEqual(completed.returncode, 0, completed.stderr)
        self.assertIn('PASS', completed.stdout)
        self.assertNotIn('reboot', completed.stdout.lower())
        after = self.disk.read_bytes()
        self.assertEqual(after[:PART_OFFSET], before[:PART_OFFSET], 'GPT area changed')
        self.assertEqual(after[PART_OFFSET + PART_SIZE:], before[PART_OFFSET + PART_SIZE:],
                         'data after the boot partition changed')
        backup_header_offset = (DISK_SIZE // SECTOR - 1) * SECTOR
        self.assertEqual(after[backup_header_offset:backup_header_offset + SECTOR],
                         before[backup_header_offset:backup_header_offset + SECTOR],
                         'backup GPT header changed')
        self.assertEqual(after[PART_OFFSET:PART_OFFSET + len(payload)], payload)
        padding = after[PART_OFFSET + len(payload):PART_OFFSET + PART_SIZE]
        self.assertEqual(padding, b'\x00' * (PART_SIZE - len(payload)))

    def test_rejects_wrong_type_guid(self):
        make_disk(self.disk, entry0=build_entry(OTHER_TYPE_GUID, UNIQUE_GUID,
                                                START_LBA, START_LBA + PART_SECTORS - 1))
        self.write_payload(4096)
        self.assert_rejected(message='type GUID')

    def test_rejects_wrong_unique_guid(self):
        make_disk(self.disk, entry0=build_entry(TYPE_GUID, OTHER_UNIQUE_GUID,
                                                START_LBA, START_LBA + PART_SECTORS - 1))
        self.write_payload(4096)
        self.assert_rejected(message='unique GUID')

    def test_rejects_wrong_start_lba(self):
        make_disk(self.disk, entry0=build_entry(TYPE_GUID, UNIQUE_GUID,
                                                START_LBA + 1, START_LBA + PART_SECTORS))
        self.write_payload(4096)
        self.assert_rejected(message='geometry mismatch')

    def test_rejects_wrong_partition_length(self):
        make_disk(self.disk, entry0=build_entry(TYPE_GUID, UNIQUE_GUID,
                                                START_LBA, START_LBA + PART_SECTORS))
        self.write_payload(4096)
        self.assert_rejected(message='geometry mismatch')

    def test_rejects_short_partition(self):
        make_disk(self.disk, entry0=build_entry(TYPE_GUID, UNIQUE_GUID,
                                                START_LBA, START_LBA + PART_SECTORS - 2))
        self.write_payload(4096)
        self.assert_rejected(message='geometry mismatch')

    def test_rejects_corrupted_header_crc(self):
        make_disk(self.disk)
        self.write_payload(4096)
        with open(self.disk, 'r+b') as handle:
            handle.seek(SECTOR + 20)
            handle.write(b'\x01')
        self.assert_rejected(message='header CRC')

    def test_rejects_corrupted_entry_crc(self):
        make_disk(self.disk)
        self.write_payload(4096)
        entries_size = 128 * 128
        with open(self.disk, 'r+b') as handle:
            handle.seek(2 * SECTOR + entries_size - 1)
            handle.write(b'\x5a')
        self.assert_rejected(message='entry array CRC')

    def test_rejects_entry_array_overflow(self):
        make_disk(self.disk, entries_lba=0xFFFFFFFFFFFFFFFF)
        self.write_payload(4096)
        self.assert_rejected(message='entry array')

    def test_rejects_entry_array_overlapping_boot(self):
        make_disk(self.disk, entries_lba=START_LBA + 4)
        self.write_payload(4096)
        self.assert_rejected(message='entry array')

    def test_rejects_second_entry_overlapping_boot(self):
        make_disk(self.disk, entry1=build_entry(
            OTHER_TYPE_GUID, OTHER_UNIQUE_GUID,
            START_LBA + 1024, START_LBA + PART_SECTORS + 1024, 'data'))
        self.write_payload(4096)
        self.assert_rejected(message='overlap')

    def test_rejects_missing_gpt(self):
        with open(self.disk, 'wb') as handle:
            handle.truncate(DISK_SIZE)
        self.write_payload(4096)
        self.assert_rejected(message='GPT header')

    def test_rejects_undersized_disk(self):
        make_disk(self.disk, disk_size=20 * 1024 * 1024)
        self.write_payload(4096)
        self.assert_rejected(message='at least')

    def test_rejects_oversized_image(self):
        make_disk(self.disk)
        self.write_payload(PART_SIZE + 1)
        self.assert_rejected(message='larger than')

    def test_rejects_empty_image(self):
        make_disk(self.disk)
        self.image.write_bytes(b'')
        self.assert_rejected(message='is empty')

    def test_rejects_lock_contention(self):
        make_disk(self.disk)
        self.write_payload(4096)
        with open(self.disk, 'rb') as handle:
            fcntl.flock(handle.fileno(), fcntl.LOCK_EX)
            self.assert_rejected(message='lock')

    def test_rejects_fsync_failure_without_printing_success(self):
        make_disk(self.disk)
        self.write_payload(4096)
        completed = run_tool(self.test_binary, '--test-disk', self.disk,
                             '--image', self.image, '--test-fail-fsync')
        self.assertNotEqual(completed.returncode, 0)
        self.assertNotIn('PASS', completed.stdout)
        self.assertIn('fsync', completed.stderr)

    def test_production_binary_has_no_test_hooks(self):
        binary = self.production.read_bytes()
        self.assertNotIn(b'--test-disk', binary)
        self.assertNotIn(b'--test-fail-fsync', binary)

    def test_production_binary_rejects_test_disk_option(self):
        make_disk(self.disk)
        self.write_payload(4096)
        completed = run_tool(self.production, '--test-disk', self.disk,
                             '--image', self.image)
        self.assertNotEqual(completed.returncode, 0)
        self.assertIn('unknown option', completed.stderr)

    def test_production_binary_rejects_regular_device(self):
        make_disk(self.disk)
        self.write_payload(4096)
        completed = run_tool(self.production, '--device', self.disk,
                             '--image', self.image)
        self.assertNotEqual(completed.returncode, 0)
        self.assertIn('block device', completed.stderr)

    def test_production_binary_rejects_symlinked_device(self):
        target = self.dir / 'mmcblk0p1'
        target.write_bytes(b'not a device')
        link = self.dir / 'links' / 'mmcblk0p1'
        link.parent.mkdir()
        link.symlink_to(target)
        self.write_payload(4096)
        completed = run_tool(self.production, '--device', link, '--image', self.image)
        self.assertNotEqual(completed.returncode, 0)
        self.assertIn('symlink', completed.stderr)


if __name__ == '__main__':
    unittest.main()
