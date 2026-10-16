#!/usr/bin/env python3
'''Unit tests for tools/github_sync.py.

Run with:  python3 -m unittest tools.test_github_sync -v
Git is required for the stage/audit tests; they skip when git is missing.
'''

import contextlib
import hashlib
import importlib.util
import io
import json
import os
import shutil
import subprocess
import sys
import tempfile
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
SPEC = importlib.util.spec_from_file_location(
    'github_sync', os.path.join(HERE, 'github_sync.py'))
gs = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(gs)

GIT = shutil.which('git')


def run_cli(args, env=None):
    '''Run the CLI in-process, capture stdout, return (returncode, json).'''
    buffer = io.StringIO()
    saved = dict(os.environ)
    if env:
        os.environ.update(env)
    try:
        with contextlib.redirect_stdout(buffer):
            try:
                returncode = gs.main(args)
            except SystemExit as exc:
                returncode = exc.code
    finally:
        os.environ.clear()
        os.environ.update(saved)
    text = buffer.getvalue().strip()
    return returncode, (json.loads(text) if text else None)


def read_jsonl(path):
    entries = []
    with open(path, 'rb') as handle:
        for line in handle:
            line = line.strip()
            if line:
                entries.append(json.loads(line.decode('utf-8')))
    return entries


def parse_ls_files(data):
    entries = {}
    for record in data.split(b'\0'):
        if not record:
            continue
        meta, _sep, path = record.partition(b'\t')
        fields = meta.split(b' ')
        if len(fields) >= 2 and path:
            entries[path] = (fields[0].decode('ascii'), fields[1].decode('ascii'))
    return entries


def tree_fingerprint(path):
    digest = hashlib.sha256()
    for dirpath, dirnames, filenames in os.walk(path):
        dirnames.sort()
        filenames.sort()
        for name in filenames:
            full = os.path.join(dirpath, name)
            rel = os.path.relpath(full, path)
            digest.update(rel.encode('utf-8', 'surrogateescape'))
            digest.update(b'\0')
            try:
                with open(full, 'rb') as handle:
                    digest.update(hashlib.sha256(handle.read()).digest())
            except OSError:
                digest.update(b'unreadable')
    return digest.hexdigest()


class Base(unittest.TestCase):
    def setUp(self):
        if GIT is None:
            self.skipTest('git is not available')
        self.tmp = tempfile.mkdtemp(prefix='ghsync-')
        self.addCleanup(shutil.rmtree, self.tmp, ignore_errors=True)
        self.root = os.path.join(self.tmp, 'root')
        os.makedirs(self.root)
        self.git('init', '-q')

    def git(self, *args, cwd=None, check=True):
        return subprocess.run([GIT] + list(args), cwd=cwd or self.root,
                              check=check, stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE)

    def write(self, rel, data=b'payload'):
        path = os.path.join(self.root, *rel.split('/'))
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, 'wb') as handle:
            handle.write(data)
        return path

    def plan(self, env=None):
        returncode, result = run_cli(['plan', '--root', self.root], env=env)
        self.assertEqual(returncode, 0, result)
        return result

    def stage(self, env=None):
        returncode, result = run_cli(['stage', '--root', self.root], env=env)
        self.assertEqual(returncode, 0, result)
        return result

    def included(self):
        return {entry['path'] for entry in
                read_jsonl(os.path.join(self.root, gs.INCLUDE_REL))}

    def excluded(self):
        return {entry['path']: entry['reason'] for entry in
                read_jsonl(os.path.join(self.root, gs.EXCLUDE_REL))}

    def large(self):
        return {entry['path'] for entry in
                read_jsonl(os.path.join(self.root, gs.LARGE_REL))}


class PlanTests(Base):
    def test_keeps_sources_data_and_evidence(self):
        keep = [
            'src/main.c', 'data/result.bin', 'logs/run.log', 'docs/paper.pdf',
            'out/exp1/result.rpt', 'out/exp1/run.log', 'out/exp1/table.csv',
            'notes.md', 'img/plot.png', 'backup/linux.tar.gz',
            'src/linux.bundle', 'presentation/deck.ppt',
            'assets/rootfs-base.cpio', 'test/fixtures/libfoo.so',
            'hw/rtl/top.v', 'hw/xilinx/ariane.srcs/sources_1/top.v',
        ]
        for rel in keep:
            self.write(rel)
        self.plan()
        included = self.included()
        for rel in keep:
            self.assertIn(rel, included)

    def test_excludes_build_artifacts(self):
        artifacts = [
            'build/a.o', 'build/b.dcp', 'x/c.sdf', 'x/d.bit', 'x/e.mcs',
            'x/f.rpx', 'x/g.gch', 'x/h.elf', 'x/i.dtb', 'x/j.pyc', 'x/k.a',
            'x/l.so', 'kernel/vmlinux', 'kernel/Image',
            'opensbi/fw_payload.bin', 'linux/rootfs.cpio',
        ]
        for rel in artifacts:
            self.write(rel)
        self.plan()
        excluded = self.excluded()
        included = self.included()
        for rel in artifacts:
            self.assertIn(rel, excluded)
            self.assertNotIn(rel, included)

    def test_fixtures_and_assets_override_artifact_rules(self):
        self.write('test/fixtures/libx.so')
        self.write('testdata/blob.o')
        self.write('assets/rootfs-base.cpio')
        self.write('test/fixtures/tool')
        self.plan()
        included = self.included()
        for rel in ('test/fixtures/libx.so', 'testdata/blob.o',
                    'assets/rootfs-base.cpio', 'test/fixtures/tool'):
            self.assertIn(rel, included)

    def test_build_directories_keep_evidence_only(self):
        excluded = [
            'p/out/linux/init/main.o', 'p/out/linux/drivers/x/y.c',
            'p/out/opensbi/firmware/fw.a', 'p/work-fpga/ariane_timesim.sdf',
            'p/work-fpga/ariane_timesim.v', 'p/obj_dir/Vtop.cpp',
            'p/obj_dir/Vtop.h', 'p/CMakeFiles/x.dir/a.cmake',
            'p/m/__pycache__/m.cpython-310.pyc',
            'p/Vivado/ariane.runs/synth_1/ariane_xilinx.v',
            'p/.Xil/tmp/x.tim',
        ]
        included = [
            'p/out/linux/build.log', 'p/out/linux/compile_commands.json',
            'p/work-fpga/utilization.rpt', 'p/out/exp/r.rpt',
            'p/out/exp/result.rpt', 'rtl/handwritten/top.v',
            'p/ariane.srcs/sources_1/top.v',
        ]
        for rel in excluded + included:
            self.write(rel)
        self.plan()
        got_excluded = self.excluded()
        got_included = self.included()
        for rel in excluded:
            self.assertIn(rel, got_excluded)
        for rel in included:
            self.assertIn(rel, got_included)

    def test_real_world_generated_paths_and_neighbours(self):
        excluded = [
            'out/n-prototype/fpga-src/cva6/corev_apu/fpga/xilinx/'
            'xlnx_axi_quad_spi/xlnx_axi_quad_spi.gen/sources_1/ip/'
            'xlnx_axi_quad_spi/hdl/blk_mem_gen_v8_4_vhsyn_rfs.vhd',
            'out/exp/obj/Vtop.cpp',
            'out/exp/obj-stream-model-r2/Vtop__Slow.cpp',
            'out/exp/obj-unit/Vfoo.h',
            'out/exp/obj-policy-system/Vbar.mk',
            'out/r107-platform-identity-recovery-20261012/kernel/'
            'reproduction/linux-output/init/main.o',
            'out/r107-ordinary-pilot-20261012/build/linux/init/main.o',
            'out/repair-1/build/linux/init/main.o',
            'out/r107-ordinary-pilot-20261012/build/opensbi/firmware/fw.a',
            'out/r107-ordinary-pilot-20261012/build/lloader/foo.o',
            'out/r107-platform-identity-recovery-20261012/kernel/'
            'reproduction/linux-output/.foo.o.cmd',
            'out/r107-platform-identity-recovery-20261012/kernel/'
            'reproduction/linux-output/.foo.ko.cmd',
            'out/r107-platform-identity-recovery-20261012/kernel/'
            'reproduction/linux-output/.foo.mod.cmd',
            'out/r107-platform-identity-recovery-20261012/kernel/'
            'reproduction/linux-output/modules.order',
            'out/r107-platform-identity-recovery-20261012/kernel/'
            'reproduction/linux-output/foo.mod.o',
            'out/r107-platform-identity-recovery-20261012/kernel/'
            'reproduction/linux-output/foo.ko',
            'out/r107-platform-identity-recovery-20261012/kernel/'
            'reproduction/linux-output/arch/riscv/boot/linux-rv64-cva6.bin',
            'images/linux-rv64-cva6.bin',
        ]
        included = [
            'out/n-prototype/fpga-src/cva6/corev_apu/fpga/xilinx/'
            'xlnx_axi_quad_spi/xlnx_axi_quad_spi.gen/report.log',
            'out/exp/obj/tb.cpp',
            'out/exp/obj/Version.h',
            'src/Version.h',
            'out/r107-platform-identity-recovery-20261012/source/linux/'
            'init/main.c',
            'out/r107-platform-identity-recovery-20261012/kernel/'
            'reproduction/linux-source/init/main.c',
            'out/r107-platform-identity-recovery-20261012/kernel/'
            'reproduction/linux-output/build.log',
            'out/r107-ordinary-pilot-20261012/build/linux/'
            'compile_commands.json',
            'data/result.bin',
            'data/metadata.bin',
            'data/payload.bin',
            'data/output.bin',
            'scripts/build.cmd',
            'test/fixtures/linux-rv64-cva6.bin',
            'test/fixtures/modules.order',
            'test/fixtures/.foo.o.cmd',
            'test/fixtures/foo.ko',
        ]
        for rel in excluded + included:
            self.write(rel)
        self.plan()
        got_excluded = self.excluded()
        got_included = self.included()
        for rel in excluded:
            self.assertIn(rel, got_excluded, rel)
            self.assertNotIn(rel, got_included, rel)
        for rel in included:
            self.assertIn(rel, got_included, rel)
            self.assertNotIn(rel, got_excluded, rel)

    def test_credentials_excluded_but_not_all_certs(self):
        secrets = ['.aws/credentials', '.env', '.env.local', 'id_rsa',
                   'id_ed25519', '.netrc']
        keep = ['.env.example', 'references/test.pem', 'certs/key.key',
                'config.json', 'id_rsa.pub']
        for rel in secrets + keep:
            self.write(rel)
        self.plan()
        got_excluded = self.excluded()
        got_included = self.included()
        for rel in secrets:
            self.assertIn(rel, got_excluded)
        for rel in keep:
            self.assertIn(rel, got_included)

    def test_paths_with_spaces_and_unicode(self):
        rels = ['源码 目录/我的 文件.c', 'data/报告 2026.rpt',
                'dir with spaces/a b c.txt']
        for rel in rels:
            self.write(rel)
        self.plan()
        included = self.included()
        for rel in rels:
            self.assertIn(rel, included)

    def test_symlinks_recorded_not_followed(self):
        self.write('real/child.txt', b'hello')
        os.symlink('real/child.txt', os.path.join(self.root, 'link to file'))
        os.symlink('real', os.path.join(self.root, 'linkdir'))
        self.plan()
        included = {entry['path']: entry for entry in
                    read_jsonl(os.path.join(self.root, gs.INCLUDE_REL))}
        self.assertIn('link to file', included)
        self.assertEqual(included['link to file']['kind'], 'symlink')
        self.assertEqual(included['link to file']['mode'], '120000')
        self.assertEqual(included['link to file']['link_target'], 'real/child.txt')
        self.assertIn('linkdir', included)
        self.assertEqual(included['linkdir']['link_target'], 'real')
        self.assertFalse(any(path.startswith('linkdir/')
                             for path in included))


class StageTests(Base):
    def test_nested_repo_tracked_as_plain_files(self):
        nested = os.path.join(self.root, 'vendor-repo')
        os.makedirs(nested)
        self.git('init', '-q', cwd=nested)
        with open(os.path.join(nested, 'a.txt'), 'w') as handle:
            handle.write('nested content\n')
        self.git('add', 'a.txt', cwd=nested)
        before = tree_fingerprint(nested)
        self.write('src/main.c', b'int main(void){return 0;}\n')
        self.plan()
        self.stage()
        entries = parse_ls_files(self.git('ls-files', '-s', '-z').stdout)
        self.assertIn(b'vendor-repo/a.txt', entries)
        self.assertEqual(entries[b'vendor-repo/a.txt'][0], '100644')
        self.assertTrue(all(mode != '160000' for mode, _oid in entries.values()))
        self.assertNotIn(b'vendor-repo/.git/config', entries)
        self.assertEqual(tree_fingerprint(nested), before)
        listed = self.git('ls-files', cwd=nested).stdout.split()
        self.assertEqual(listed, [b'a.txt'])

    def test_stage_detects_size_and_mtime_changes(self):
        self.write('src/a.c', b'aaaa')
        self.plan()
        self.write('src/a.c', b'aaaabbbb')
        returncode, result = run_cli(['stage', '--root', self.root])
        self.assertEqual(returncode, 1)
        self.assertIn('re-run plan', result['error'])
        self.assertEqual(self.git('ls-files').stdout, b'')

        self.write('src/b.c', b'bbbb')
        self.plan()
        os.utime(os.path.join(self.root, 'src', 'b.c'), (1234567, 1234567))
        returncode, result = run_cli(['stage', '--root', self.root])
        self.assertEqual(returncode, 1)
        self.assertIn('re-run plan', result['error'])

    def test_stage_refuses_unmanaged_index(self):
        self.write('src/a.c', b'a')
        self.plan()
        self.git('add', '-f', '--', 'src/a.c')
        returncode, result = run_cli(['stage', '--root', self.root])
        self.assertEqual(returncode, 1)
        self.assertIn('refusing', result['error'])
        entries = parse_ls_files(self.git('ls-files', '-s', '-z').stdout)
        self.assertEqual(sorted(entries), [b'src/a.c'])

    def test_stage_is_repeatable(self):
        self.write('src/a.c', b'a')
        self.plan()
        self.stage()
        returncode, result = run_cli(['stage', '--root', self.root])
        self.assertEqual(returncode, 0, result)

    def test_symlink_staged_as_link_blob(self):
        self.write('real/child.txt', b'hello')
        os.symlink('real/child.txt', os.path.join(self.root, 'linkfile'))
        self.plan()
        self.stage()
        entries = parse_ls_files(self.git('ls-files', '-s', '-z').stdout)
        self.assertEqual(entries[b'linkfile'][0], '120000')
        oid = entries[b'linkfile'][1]
        content = self.git('cat-file', '-p', oid).stdout
        self.assertEqual(content, b'real/child.txt')


class ChunkTests(Base):
    ENV = {gs.ENV_THRESHOLD: '1024', gs.ENV_CHUNK_SIZE: '512'}

    def make_tree(self):
        data = bytes(range(256)) * 9
        self.write('big/blob.bin', data)
        self.write('small/note.txt', b'hello')
        return data

    def test_chunk_manifest_and_restore(self):
        data = self.make_tree()
        self.plan(env=self.ENV)
        self.assertIn('big/blob.bin', self.large())
        self.stage(env=self.ENV)
        manifest = json.load(open(os.path.join(self.root, gs.MANIFEST_REL),
                                  'rb'))
        record = manifest['files'][0]
        self.assertEqual(record['path'], 'big/blob.bin')
        self.assertEqual(record['sha256'], hashlib.sha256(data).hexdigest())
        self.assertEqual(sum(chunk['size'] for chunk in record['chunks']),
                         len(data))
        self.assertTrue(len(record['chunks']) >= 4)
        chunk_dir = os.path.join(self.root, gs.CHUNK_DIR_REL)
        for chunk in record['chunks']:
            self.assertTrue(os.path.isfile(os.path.join(chunk_dir,
                                                        chunk['sha256'])))

        destination = os.path.join(self.tmp, 'dest')
        returncode, result = run_cli(
            ['restore', '--root', self.root, '--destination', destination],
            env=self.ENV)
        self.assertEqual(returncode, 0, result)
        with open(os.path.join(destination, 'big', 'blob.bin'), 'rb') as handle:
            self.assertEqual(handle.read(), data)

        target = os.path.join(destination, 'big', 'blob.bin')
        with open(target, 'wb') as handle:
            handle.write(b'different')
        returncode, result = run_cli(
            ['restore', '--root', self.root, '--destination', destination],
            env=self.ENV)
        self.assertEqual(returncode, 1)
        with open(target, 'rb') as handle:
            self.assertEqual(handle.read(), b'different')

    def test_restore_detects_corrupted_chunk(self):
        self.make_tree()
        self.plan(env=self.ENV)
        self.stage(env=self.ENV)
        chunk_dir = os.path.join(self.root, gs.CHUNK_DIR_REL)
        victim = sorted(os.listdir(chunk_dir))[0]
        victim_path = os.path.join(chunk_dir, victim)
        with open(victim_path, 'r+b') as handle:
            block = bytearray(handle.read(1))
            block[0] = block[0] ^ 0xFF
            handle.seek(0)
            handle.write(bytes(block))
        destination = os.path.join(self.tmp, 'dest-corrupt')
        returncode, result = run_cli(
            ['restore', '--root', self.root, '--destination', destination],
            env=self.ENV)
        self.assertEqual(returncode, 1)
        self.assertFalse(os.path.exists(os.path.join(destination, 'big',
                                                     'blob.bin')))

    def test_restore_rejects_escape_paths(self):
        self.make_tree()
        self.plan(env=self.ENV)
        self.stage(env=self.ENV)
        manifest_path = os.path.join(self.root, gs.MANIFEST_REL)
        manifest = json.load(open(manifest_path, 'rb'))
        record = manifest['files'][0]
        absolute = os.path.join(self.tmp, 'abs-escaped.bin')
        manifest['files'] = [dict(record, path='../escaped.bin'),
                             dict(record, path=absolute)]
        with open(manifest_path, 'wb') as handle:
            handle.write(json.dumps(manifest).encode('utf-8'))
        destination = os.path.join(self.tmp, 'dest-escape')
        returncode, result = run_cli(
            ['restore', '--root', self.root, '--destination', destination],
            env=self.ENV)
        self.assertEqual(returncode, 1)
        self.assertFalse(os.path.exists(os.path.join(self.tmp, 'escaped.bin')))
        self.assertFalse(os.path.exists(absolute))
        self.assertFalse(os.path.exists(os.path.join(destination, 'big',
                                                     'blob.bin')))

    def test_restore_rejects_symlinked_ancestor(self):
        self.make_tree()
        self.plan(env=self.ENV)
        self.stage(env=self.ENV)
        manifest_path = os.path.join(self.root, gs.MANIFEST_REL)
        manifest = json.load(open(manifest_path, 'rb'))
        record = manifest['files'][0]
        manifest['files'] = [dict(record, path='sneaky/x.bin')]
        with open(manifest_path, 'wb') as handle:
            handle.write(json.dumps(manifest).encode('utf-8'))
        destination = os.path.join(self.tmp, 'dest-symlink')
        outside = os.path.join(self.tmp, 'outside')
        os.makedirs(destination)
        os.makedirs(outside)
        os.symlink(outside, os.path.join(destination, 'sneaky'))
        returncode, result = run_cli(
            ['restore', '--root', self.root, '--destination', destination],
            env=self.ENV)
        self.assertEqual(returncode, 1)
        self.assertFalse(os.path.exists(os.path.join(outside, 'x.bin')))


class AuditTests(Base):
    def test_audit_passes_on_staged_tree(self):
        self.write('src/a.c', b'a')
        self.write('logs/run.log', b'log')
        self.plan()
        self.stage()
        returncode, result = run_cli(['audit', '--root', self.root])
        self.assertEqual(returncode, 0, result)
        self.assertTrue(result['ok'])
        self.assertEqual(result['stats']['plan_include'], 2)

    def test_audit_covers_large_files(self):
        env = {gs.ENV_THRESHOLD: '1024', gs.ENV_CHUNK_SIZE: '512'}
        self.write('big/blob.bin', bytes(range(256)) * 9)
        self.plan(env=env)
        self.stage(env=env)
        returncode, result = run_cli(['audit', '--root', self.root], env=env)
        self.assertEqual(returncode, 0, result)
        self.assertEqual(result['stats']['plan_large'], 1)

    def test_audit_reports_gitlink(self):
        self.write('src/a.c', b'a')
        self.plan()
        self.stage()
        fake = '0123456789abcdef0123456789abcdef01234567'
        proc = subprocess.run(
            [GIT, 'update-index', '--add', '--cacheinfo',
             '160000,%s,subrepo' % fake], cwd=self.root,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if proc.returncode != 0:
            self.skipTest('git refuses to record a synthetic gitlink')
        returncode, result = run_cli(['audit', '--root', self.root])
        self.assertEqual(returncode, 1)
        self.assertTrue(any('gitlink' in issue for issue in result['issues']))


class HardeningTests(Base):
    ENV = {gs.ENV_THRESHOLD: '1024', gs.ENV_CHUNK_SIZE: '512'}

    def test_stage_hashes_quoted_and_cr_filenames(self):
        names = ['"quoted.txt', 'cr' + chr(13) + 'name.txt',
                 'trailing' + chr(13)]
        for name in names:
            self.write(name, b'data')
        self.plan()
        self.stage()
        entries = parse_ls_files(self.git('ls-files', '-s', '-z').stdout)
        for name in names:
            self.assertIn(name.encode('utf-8'), entries)

    def test_restore_rejects_windows_drive_paths(self):
        self.write('big/blob.bin', bytes(range(256)) * 9)
        self.plan(env=self.ENV)
        self.stage(env=self.ENV)
        manifest_path = os.path.join(self.root, gs.MANIFEST_REL)
        manifest = json.load(open(manifest_path, 'rb'))
        record = manifest['files'][0]
        destination = os.path.join(self.tmp, 'dest-drive')
        for bad in (r'C:\evil.bin', r'C:/evil.bin'):
            manifest['files'] = [dict(record, path=bad)]
            with open(manifest_path, 'wb') as handle:
                handle.write(json.dumps(manifest).encode('utf-8'))
            returncode, result = run_cli(
                ['restore', '--root', self.root, '--destination', destination],
                env=self.ENV)
            self.assertEqual(returncode, 1)
        self.assertFalse(os.path.exists(os.path.join(self.tmp, 'evil.bin')))

    def test_restore_rejects_tool_metadata_paths(self):
        self.write('big/blob.bin', bytes(range(256)) * 9)
        self.plan(env=self.ENV)
        self.stage(env=self.ENV)
        manifest_path = os.path.join(self.root, gs.MANIFEST_REL)
        manifest = json.load(open(manifest_path, 'rb'))
        record = manifest['files'][0]
        destination = os.path.join(self.tmp, 'dest-meta')
        for bad in ('.git/config', '.github-sync/evil.bin',
                    'nested/.git/config'):
            manifest['files'] = [dict(record, path=bad)]
            with open(manifest_path, 'wb') as handle:
                handle.write(json.dumps(manifest).encode('utf-8'))
            returncode, result = run_cli(
                ['restore', '--root', self.root, '--destination', destination],
                env=self.ENV)
            self.assertEqual(returncode, 1)
        self.assertFalse(os.path.exists(os.path.join(destination, '.git',
                                                     'config')))
        self.assertFalse(os.path.exists(os.path.join(destination,
                                                     '.github-sync', 'evil.bin')))

    def test_stage_rejects_same_size_corrupt_chunk(self):
        data = bytes(range(256)) * 9
        self.write('big/blob.bin', data)
        self.plan(env=self.ENV)
        self.stage(env=self.ENV)
        chunk_dir = os.path.join(self.root, gs.CHUNK_DIR_REL)
        victim = sorted(os.listdir(chunk_dir))[0]
        victim_path = os.path.join(chunk_dir, victim)
        size_before = os.path.getsize(victim_path)
        with open(victim_path, 'r+b') as handle:
            block = bytearray(handle.read(1))
            block[0] = block[0] ^ 0xFF
            handle.seek(0)
            handle.write(bytes(block))
        returncode, result = run_cli(['stage', '--root', self.root],
                                     env=self.ENV)
        self.assertEqual(returncode, 1)
        self.assertIn('corrupt', result['error'].lower())
        self.assertEqual(os.path.getsize(victim_path), size_before)

    def test_restore_rejects_invalid_and_symlinked_chunks(self):
        self.write('big/blob.bin', bytes(range(256)) * 9)
        self.plan(env=self.ENV)
        self.stage(env=self.ENV)
        manifest_path = os.path.join(self.root, gs.MANIFEST_REL)
        manifest = json.load(open(manifest_path, 'rb'))
        record = manifest['files'][0]
        destination = os.path.join(self.tmp, 'dest-chunk')
        chunk_dir = os.path.join(self.root, gs.CHUNK_DIR_REL)

        manifest['files'] = [dict(record, chunks=[
            dict(record['chunks'][0], sha256='not-a-sha')])]
        with open(manifest_path, 'wb') as handle:
            handle.write(json.dumps(manifest).encode('utf-8'))
        returncode, result = run_cli(
            ['restore', '--root', self.root, '--destination', destination],
            env=self.ENV)
        self.assertEqual(returncode, 1)

        evil_sha = '0' * 64
        outside = os.path.join(self.tmp, 'outside-chunk.bin')
        with open(outside, 'wb') as handle:
            handle.write(b'evil')
        os.symlink(outside, os.path.join(chunk_dir, evil_sha))
        manifest['files'] = [dict(record, chunks=[
            {'sha256': evil_sha, 'size': 4}])]
        with open(manifest_path, 'wb') as handle:
            handle.write(json.dumps(manifest).encode('utf-8'))
        returncode, result = run_cli(
            ['restore', '--root', self.root, '--destination', destination],
            env=self.ENV)
        self.assertEqual(returncode, 1)
        self.assertFalse(os.path.exists(os.path.join(destination, 'big',
                                                     'blob.bin')))

    def test_restore_overwrite_flag_is_explicit(self):
        data = bytes(range(256)) * 9
        self.write('big/blob.bin', data)
        self.plan(env=self.ENV)
        self.stage(env=self.ENV)
        destination = os.path.join(self.tmp, 'dest-overwrite')
        returncode, result = run_cli(
            ['restore', '--root', self.root, '--destination', destination],
            env=self.ENV)
        self.assertEqual(returncode, 0, result)
        target = os.path.join(destination, 'big', 'blob.bin')
        with open(target, 'wb') as handle:
            handle.write(b'different')
        returncode, result = run_cli(
            ['restore', '--root', self.root, '--destination', destination],
            env=self.ENV)
        self.assertEqual(returncode, 1)
        self.assertFalse(result['ok'])
        returncode, result = run_cli(
            ['restore', '--root', self.root, '--destination', destination,
             '--overwrite'],
            env=self.ENV)
        self.assertEqual(returncode, 0, result)
        with open(target, 'rb') as handle:
            self.assertEqual(handle.read(), data)


if __name__ == '__main__':
    unittest.main()

