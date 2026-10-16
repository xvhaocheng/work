#!/usr/bin/env python3
'''github_sync.py - plan/stage/restore/audit helper for mirroring a large mixed
work tree into a private GitHub repository without uploading build artefacts.

The script only creates files below <root>/.git/sync-preparation/ and
<root>/.github-sync/.  It never deletes, moves or rewrites user files, never
runs git commit / git push / git reset / git rm / git clean, never follows
symlinks, never uses a shell or shell interpolation and never touches the
network.

Sub-commands:
    plan    --root PATH
    stage   --root PATH
    restore --root PATH [--destination PATH] [--overwrite]
    audit   --root PATH
'''

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import stat
import subprocess
import sys
import tempfile
import time

SCRIPT_VERSION = 1
MIB = 1024 * 1024
DEFAULT_LARGE_THRESHOLD = 49 * MIB
DEFAULT_CHUNK_SIZE = 49 * MIB

ENV_THRESHOLD = 'GITHUB_SYNC_THRESHOLD_BYTES'
ENV_CHUNK_SIZE = 'GITHUB_SYNC_CHUNK_SIZE'

PLAN_DIR_REL = '.git/sync-preparation'
SYNC_DIR_REL = '.github-sync'
CHUNK_DIR_REL = '.github-sync/chunks'
MANIFEST_REL = '.github-sync/large-files.json'
INCLUDE_REL = PLAN_DIR_REL + '/include.jsonl'
EXCLUDE_REL = PLAN_DIR_REL + '/exclude.jsonl'
LARGE_REL = PLAN_DIR_REL + '/large.jsonl'
SUMMARY_REL = PLAN_DIR_REL + '/summary.json'
STAGED_PATHS_REL = PLAN_DIR_REL + '/staged-paths.bin'
STAGE_STATE_REL = PLAN_DIR_REL + '/stage-state.json'
SYMLINK_BLOBS_REL = PLAN_DIR_REL + '/symlink-blobs'

HASH_BATCH_MAX = 128
HASH_BATCH_BYTES = 8192
CAT_BATCH = 256
READ_BLOCK = 1 << 20
ELF_PROBE_MAX_SIZE = 64 * MIB
ELF_PROBE_LIMIT = 20000

BACKSLASH = chr(92)
ABS_PATH_RE = re.compile(r'^[A-Za-z]:[\\/]')
SHA256_RE = re.compile(r'^[0-9a-f]{64}$')

# --- classification tables -------------------------------------------------
# Files whose extension is a build artefact (kept inside fixtures/assets).
ARTIFACT_EXTS = frozenset(['.o', '.a', '.so', '.pyc', '.dcp', '.sdf', '.bit',
                           '.mcs', '.rpx', '.gch', '.elf', '.dtb', '.ko'])
# Directories that are unambiguously build output.  Everything inside them is
# excluded except small text evidence (see EVIDENCE_EXT).
BUILD_DIR_NAMES = frozenset(['work-fpga', 'obj_dir', 'cmakefiles',
                             '__pycache__', '.xil'])
BUILD_DIR_SUFFIXES = ('.runs', '.cache', '.gen')
# <a>/<b> component pairs that identify a build tree (out/linux, out/opensbi,
# hero/build).  Deliberately a pair, not a bare directory name, so that build/
# target/ vendor/ dist/ directories are never dropped wholesale.
BUILD_DIR_SEQUENCES = frozenset([('out', 'linux'), ('out', 'opensbi'),
                                 ('hero', 'build')])
# Build directories that are only recognised below an out/ directory.
OUT_BUILD_DIR_NAMES = frozenset(['linux-output'])
OUT_BUILD_DIR_SEQUENCES = frozenset([('build', 'linux'),
                                     ('build', 'opensbi'),
                                     ('build', 'lloader')])
# Evidence kept even inside an excluded build directory.
EVIDENCE_EXTS = frozenset(['.log', '.rpt', '.json', '.md', '.txt', '.csv',
                           '.yaml', '.yml'])
EVIDENCE_NAMES = frozenset(['.config', 'system.map', 'module.symvers'])
# Directories whose contents are hand written / third party data: never apply
# the artefact rules there.
FIXTURE_DIRS = frozenset(['fixtures', 'fixture', 'inputs', 'testdata', 'test-data',
                          'test_data', 'assets'])
CREDENTIAL_BASENAMES = frozenset(['.netrc', '.git-credentials', '.pypirc',
                                  '.npmrc', '.dockercfg'])
PRIVATE_KEY_NAMES = frozenset(['id_rsa', 'id_ed25519', 'id_ecdsa', 'id_dsa'])
DOTENV_ALLOWED = ('example', 'sample', 'template', 'dist')

VERILATOR_NAME_RE = re.compile(r'^V[A-Za-z_][A-Za-z0-9_$]*(\.[A-Za-z0-9_$]+)*\.(cpp|cxx|cc|h|hh|hpp|mk|d|o|a|so|gch)$', re.I)
FW_PAYLOAD_RE = re.compile(r'fw_payload[^/]*\.bin')
ROOTFS_CPIO_RE = re.compile(r'rootfs[^/]*\.cpio')
KERNEL_CMD_RE = re.compile(r'^\..+\.(o|ko|mod)\.cmd$')

ELF_MAGIC = bytes((0x7f,)) + b'ELF'
MACHO_MAGICS = frozenset([
    bytes((0xfe, 0xed, 0xfa, 0xce)), bytes((0xfe, 0xed, 0xfa, 0xcf)),
    bytes((0xce, 0xfa, 0xed, 0xfe)), bytes((0xcf, 0xfa, 0xed, 0xfe)),
    bytes((0xca, 0xfe, 0xba, 0xbe)), bytes((0xbe, 0xba, 0xfe, 0xca)),
])


class SyncError(Exception):
    '''Fatal, user visible error.'''


def _log(msg):
    sys.stderr.write(msg + '\n')
    sys.stderr.flush()


def _now_iso():
    return time.strftime('%Y-%m-%dT%H:%M:%S%z', time.localtime())


def _fs(root, rel):
    '''Absolute path for a repository relative posix path.'''
    return os.path.join(root, *rel.split('/'))


def _load_json(path):
    with open(path, 'rb') as handle:
        return json.loads(handle.read().decode('utf-8'))


def _write_json_atomic(path, obj):
    directory = os.path.dirname(path)
    if directory:
        os.makedirs(directory, exist_ok=True)
    fd, tmp = tempfile.mkstemp(dir=directory or '.', prefix='.tmp-github-sync-')
    try:
        with os.fdopen(fd, 'wb') as handle:
            handle.write((json.dumps(obj, ensure_ascii=True, indent=1,
                                     sort_keys=True) + '\n').encode('ascii'))
        os.replace(tmp, path)
    except BaseException:
        try:
            os.unlink(tmp)
        except OSError:
            pass
        raise


def _iter_jsonl(path):
    with open(path, 'rb') as handle:
        for line in handle:
            line = line.strip()
            if line:
                yield json.loads(line.decode('utf-8'))


def _read_nul(path):
    with open(path, 'rb') as handle:
        data = handle.read()
    return [item for item in data.split(b'\0') if item]


def _write_nul(path, items):
    fd, tmp = tempfile.mkstemp(dir=os.path.dirname(path), prefix='.tmp-github-sync-')
    try:
        with os.fdopen(fd, 'wb') as handle:
            for item in items:
                handle.write(item + b'\0')
        os.replace(tmp, path)
    except BaseException:
        try:
            os.unlink(tmp)
        except OSError:
            pass
        raise


def _file_sha256(path):
    digest = hashlib.sha256()
    with open(path, 'rb') as handle:
        while True:
            block = handle.read(READ_BLOCK)
            if not block:
                break
            digest.update(block)
    return digest.hexdigest()


def _env_int(name, default):
    raw = os.environ.get(name)
    if not raw:
        return default
    try:
        value = int(raw)
    except ValueError:
        raise SyncError('invalid value for %s: %r' % (name, raw))
    if value <= 0:
        raise SyncError('%s must be positive' % name)
    return value


def _threshold():
    return _env_int(ENV_THRESHOLD, DEFAULT_LARGE_THRESHOLD)


def _chunk_size():
    # a chunk must never be larger than the threshold it is meant to satisfy
    return min(_env_int(ENV_CHUNK_SIZE, DEFAULT_CHUNK_SIZE), _threshold())


# --- classification --------------------------------------------------------

def _credential_reason(low_dirs, low_name):
    if low_name == '.env' or (low_name.startswith('.env.')
                              and not any(token in low_name for token in DOTENV_ALLOWED)):
        return 'credentials:dotenv'
    if low_name in CREDENTIAL_BASENAMES:
        return 'credentials:' + low_name
    if low_name in PRIVATE_KEY_NAMES:
        return 'credentials:private-key'
    if low_dirs and low_dirs[-1] == '.aws' and low_name in ('credentials', 'config'):
        return 'credentials:aws'
    if low_dirs and low_dirs[-1] == '.docker' and low_name == 'config.json':
        return 'credentials:docker'
    return None


def _build_dir_reason(low_dirs):
    for name in low_dirs:
        if name in BUILD_DIR_NAMES:
            return 'build-dir:' + name
        for suffix in BUILD_DIR_SUFFIXES:
            if name.endswith(suffix) and len(name) > len(suffix):
                return 'build-dir:' + name
    for index in range(len(low_dirs) - 1):
        pair = (low_dirs[index], low_dirs[index + 1])
        if pair in BUILD_DIR_SEQUENCES:
            return 'build-dir:' + low_dirs[index] + '/' + low_dirs[index + 1]
    if 'out' in low_dirs:
        for name in low_dirs:
            if name in OUT_BUILD_DIR_NAMES:
                return 'build-dir:' + name
        for index in range(len(low_dirs) - 1):
            pair = (low_dirs[index], low_dirs[index + 1])
            if pair in OUT_BUILD_DIR_SEQUENCES:
                return 'build-dir:' + low_dirs[index] + '/' + low_dirs[index + 1]
    return None


def _artifact_name_reason(name, low_name):
    if low_name == 'vmlinux':
        return 'artifact:vmlinux'
    if name == 'Image':
        return 'artifact:Image'
    if low_name == 'linux-rv64-cva6.bin':
        return 'artifact:linux-image'
    if FW_PAYLOAD_RE.fullmatch(low_name):
        return 'artifact:fw_payload'
    if ROOTFS_CPIO_RE.fullmatch(low_name) and 'base' not in low_name:
        return 'artifact:rootfs-image'
    if low_name == 'modules.order':
        return 'artifact:modules-order'
    if KERNEL_CMD_RE.match(low_name):
        return 'artifact:kernel-cmd'
    return None


def _verilator_generated(low_dirs, low_name):
    if not any(name == 'obj_dir' or name == 'obj'
               or name.startswith('obj-')
               or name.startswith('obj_')
               or 'verilator' in name for name in low_dirs):
        return False
    if low_name == 'version.h':
        return False
    return bool(VERILATOR_NAME_RE.match(low_name))


def _sized(category, reason, size, threshold):
    if category == 'include' and size is not None and size > threshold:
        return 'large', reason
    return category, reason


def _classify(rel, kind, size, probe, threshold):
    '''Return (category, reason); category is include / exclude / large.'''
    parts = rel.split('/')
    name = parts[-1]
    low_name = name.lower()
    dirs = parts[:-1]
    low_dirs = [item.lower() for item in dirs]

    if rel == SYNC_DIR_REL or rel.startswith(SYNC_DIR_REL + '/'):
        return 'exclude', 'sync-tool-state'
    if '.git' in parts:
        return 'exclude', 'git-metadata'

    credential = _credential_reason(low_dirs, low_name)
    if credential:
        return 'exclude', credential

    if kind == 'symlink':
        return 'include', 'symlink'
    if kind != 'file':
        return 'exclude', 'special-file'

    in_fixture = any(item in FIXTURE_DIRS for item in low_dirs)
    build = None if in_fixture else _build_dir_reason(low_dirs)
    if build:
        ext = os.path.splitext(low_name)[1]
        if ext in EVIDENCE_EXTS or low_name in EVIDENCE_NAMES:
            return _sized('include', 'evidence-in-' + build, size, threshold)
        return 'exclude', build

    if not in_fixture:
        ext = os.path.splitext(low_name)[1]
        if ext in ARTIFACT_EXTS:
            return 'exclude', 'artifact-ext:' + ext
        by_name = _artifact_name_reason(name, low_name)
        if by_name:
            return 'exclude', by_name
        if _verilator_generated(low_dirs, low_name):
            return 'exclude', 'verilator-generated'
        if probe:
            return 'exclude', 'binary-executable:' + probe

    return _sized('include', 'default', size, threshold)


def _probe_binary_magic(path):
    '''Read the first four bytes of a suspicious extension-less executable.'''
    try:
        fd = os.open(path, os.O_RDONLY | getattr(os, 'O_NOFOLLOW', 0))
    except OSError:
        return None
    try:
        head = os.read(fd, 4)
    except OSError:
        return None
    finally:
        try:
            os.close(fd)
        except OSError:
            pass
    if head.startswith(ELF_MAGIC):
        return 'elf'
    if head in MACHO_MAGICS:
        return 'macho'
    return None


# --- plan ------------------------------------------------------------------

def cmd_plan(root):
    root = os.path.abspath(root)
    if not os.path.isdir(root):
        raise SyncError('root is not a directory: %s' % root)
    if not os.path.isdir(os.path.join(root, '.git')):
        raise SyncError('no .git directory under %s (run git init first)' % root)

    threshold = _threshold()
    plan_dir = os.path.join(root, PLAN_DIR_REL)
    os.makedirs(plan_dir, exist_ok=True)

    counts = {'include': 0, 'exclude': 0, 'large': 0, 'include_bytes': 0,
              'large_bytes': 0, 'symlinks': 0, 'special_files': 0,
              'probed_binaries': 0}
    reason_counts = {}
    pruned_git_dirs = []
    walk_errors = []
    errors = []

    include_path = os.path.join(plan_dir, 'include.jsonl')
    exclude_path = os.path.join(plan_dir, 'exclude.jsonl')
    large_path = os.path.join(plan_dir, 'large.jsonl')

    with open(include_path, 'wb') as include_handle, \
            open(exclude_path, 'wb') as exclude_handle, \
            open(large_path, 'wb') as large_handle:

        def emit(entry, category, reason):
            record = dict(entry)
            record['reason'] = reason
            data = (json.dumps(record, ensure_ascii=True, sort_keys=True)
                    + '\n').encode('ascii')
            counts[category] += 1
            reason_counts[reason] = reason_counts.get(reason, 0) + 1
            if category == 'include':
                include_handle.write(data)
                counts['include_bytes'] += record.get('size') or 0
            elif category == 'exclude':
                exclude_handle.write(data)
            else:
                large_handle.write(data)
                counts['large_bytes'] += record.get('size') or 0

        probe_budget = [ELF_PROBE_LIMIT]

        def scan_entry(rel, full):
            try:
                st = os.lstat(full)
            except OSError as exc:
                errors.append('%s: %s' % (rel, exc.strerror or 'error'))
                emit({'path': rel, 'kind': 'unknown', 'size': None,
                      'mode': None}, 'exclude',
                     'unreadable:' + (exc.strerror or 'error'))
                return

            if stat.S_ISLNK(st.st_mode):
                try:
                    target = os.readlink(full)
                except OSError as exc:
                    errors.append('%s: %s' % (rel, exc.strerror or 'error'))
                    emit({'path': rel, 'kind': 'symlink', 'size': None,
                          'mode': '120000'}, 'exclude',
                         'unreadable-symlink:' + (exc.strerror or 'error'))
                    return
                target_bytes = os.fsencode(target)
                entry = {'path': rel, 'kind': 'symlink',
                         'size': len(target_bytes), 'mode': '120000',
                         'mtime_ns': st.st_mtime_ns, 'link_target': target}
                category, reason = _classify(rel, 'symlink', len(target_bytes),
                                             None, threshold)
                counts['symlinks'] += 1
                emit(entry, category, reason)
                return

            if stat.S_ISREG(st.st_mode):
                is_exec = bool(st.st_mode & 0o111)
                mode = '100755' if is_exec else '100644'
                ext = os.path.splitext(os.path.basename(rel))[1]
                probe = None
                if (not ext) and is_exec and st.st_size <= ELF_PROBE_MAX_SIZE \
                        and probe_budget[0] > 0:
                    low_dirs = [item.lower() for item in rel.split('/')[:-1]]
                    if not any(item in FIXTURE_DIRS for item in low_dirs) \
                            and not _build_dir_reason(low_dirs):
                        probe_budget[0] -= 1
                        counts['probed_binaries'] += 1
                        probe = _probe_binary_magic(full)
                entry = {'path': rel, 'kind': 'file', 'size': st.st_size,
                         'mode': mode, 'mtime_ns': st.st_mtime_ns}
                category, reason = _classify(rel, 'file', st.st_size, probe,
                                             threshold)
                emit(entry, category, reason)
                return

            counts['special_files'] += 1
            emit({'path': rel, 'kind': 'special', 'size': None, 'mode': None},
                 'exclude', 'special-file')

        def on_walk_error(exc):
            walk_errors.append(exc)
            path = getattr(exc, 'filename', None)
            if path:
                try:
                    rel = os.path.relpath(os.path.abspath(path), root)
                    rel = rel.replace(os.sep, '/')
                except (OSError, ValueError):
                    rel = os.fsdecode(path)
            else:
                rel = ''
            message = exc.strerror or str(exc)
            errors.append(('%s: %s' % (rel, message)) if rel else message)

        for dirpath, dirnames, filenames in os.walk(root, topdown=True,
                                                    followlinks=False,
                                                    onerror=on_walk_error):
            rel_dir = os.path.relpath(dirpath, root)
            rel_dir = '' if rel_dir == '.' else rel_dir.replace(os.sep, '/')
            dirnames.sort()
            filenames.sort()
            keep = []
            for dirname in dirnames:
                rel = (rel_dir + '/' + dirname) if rel_dir else dirname
                full = os.path.join(dirpath, dirname)
                if dirname == '.git':
                    pruned_git_dirs.append(rel)
                    emit({'path': rel, 'kind': 'dir', 'size': None,
                          'mode': None}, 'exclude', 'git-metadata')
                    continue
                if rel == SYNC_DIR_REL:
                    emit({'path': rel, 'kind': 'dir', 'size': None,
                          'mode': None}, 'exclude', 'sync-tool-state')
                    continue
                if os.path.islink(full):
                    scan_entry(rel, full)
                    continue
                keep.append(dirname)
            dirnames[:] = keep
            for filename in filenames:
                rel = (rel_dir + '/' + filename) if rel_dir else filename
                scan_entry(rel, os.path.join(dirpath, filename))

    summary = {
        'version': SCRIPT_VERSION,
        'command': 'plan',
        'ok': not errors,
        'root': root,
        'generated_at': _now_iso(),
        'threshold_bytes': threshold,
        'counts': counts,
        'exclude_reasons': reason_counts,
        'pruned_git_dirs': pruned_git_dirs,
        'plan_files': {'include': INCLUDE_REL, 'exclude': EXCLUDE_REL,
                       'large': LARGE_REL},
        'errors': errors,
        'walk_errors': [str(item) for item in walk_errors][:50],
    }
    _write_json_atomic(os.path.join(plan_dir, 'summary.json'), summary)
    return summary


# --- git plumbing ----------------------------------------------------------

def _require_git(root):
    if not os.path.isdir(os.path.join(root, '.git')):
        raise SyncError('no .git directory under %s (run git init first)' % root)


def _git_run(root, args, input_bytes=None, check=True):
    proc = subprocess.run(['git'] + list(args), cwd=root, input=input_bytes,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if check and proc.returncode != 0:
        raise SyncError('git %s failed (%d): %s' % (
            ' '.join(args), proc.returncode,
            proc.stderr.decode('utf-8', 'replace').strip()[:2000]))
    return proc


def _git_ls_files(root):
    '''Return {path bytes: (mode, oid)} for the repository index.'''
    proc = _git_run(root, ['ls-files', '-s', '-z'])
    entries = {}
    for record in proc.stdout.split(b'\0'):
        if not record:
            continue
        meta, _sep, path = record.partition(b'\t')
        fields = meta.split(b' ')
        if len(fields) < 2 or not path:
            continue
        entries[path] = (fields[0].decode('ascii'), fields[1].decode('ascii'))
    return entries


class GitHashObject:
    '''One long lived git hash-object process hashing paths from stdin.

    Paths are handed over as raw bytes, one per line, which is what
    --stdin-paths expects; no C quoting and no per-file subprocess.
    '''

    def __init__(self, root):
        self.root = root
        self.stderr_file = tempfile.TemporaryFile()
        self.proc = subprocess.Popen(
            ['git', 'hash-object', '-w', '--no-filters', '--stdin-paths'],
            cwd=root, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            stderr=self.stderr_file)
        self.closed = False

    def _error(self):
        try:
            self.stderr_file.seek(0)
            message = self.stderr_file.read().decode('utf-8', 'replace')
        except Exception:
            message = ''
        return SyncError('git hash-object failed: %s' % message.strip()[:2000])

    def _hash_batch(self, paths):
        try:
            self.proc.stdin.write(b''.join(path + b'\n' for path in paths))
            self.proc.stdin.flush()
        except (BrokenPipeError, OSError):
            raise self._error()
        oids = []
        for _path in paths:
            line = self.proc.stdout.readline()
            if not line:
                raise self._error()
            oids.append(line.strip().decode('ascii'))
        return oids

    def _hash_single(self, relpath):
        proc = subprocess.run(
            ['git', 'hash-object', '-w', '--no-filters', '--',
             os.fsdecode(relpath)],
            cwd=self.root, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if proc.returncode != 0:
            raise SyncError('git hash-object failed for %r: %s' % (
                os.fsdecode(relpath),
                proc.stderr.decode('utf-8', 'replace').strip()[:500]))
        return proc.stdout.strip().decode('ascii')

    def hash_paths(self, paths):
        '''Hash a list of repository relative paths; order is preserved.'''
        result = []
        pending = []
        pending_bytes = 0
        for path in paths:
            if path.startswith(b'"') or b'\n' in path or b'\r' in path:
                if pending:
                    result.extend(self._hash_batch(pending))
                    pending = []
                    pending_bytes = 0
                result.append(self._hash_single(path))
                continue
            pending.append(path)
            pending_bytes += len(path) + 1
            if len(pending) >= HASH_BATCH_MAX or pending_bytes >= HASH_BATCH_BYTES:
                result.extend(self._hash_batch(pending))
                pending = []
                pending_bytes = 0
        if pending:
            result.extend(self._hash_batch(pending))
        return result

    def hash_path(self, path):
        return self.hash_paths([path])[0]

    def close(self):
        if self.closed:
            return
        self.closed = True
        try:
            self.proc.stdin.close()
        except OSError:
            pass
        returncode = self.proc.wait()
        error = None
        if returncode != 0:
            error = self._error()
        try:
            self.proc.stdout.close()
        except OSError:
            pass
        self.stderr_file.close()
        if error is not None:
            raise error


class GitUpdateIndex:
    '''One long lived git update-index --index-info -z process.'''

    def __init__(self, root):
        self.stderr_file = tempfile.TemporaryFile()
        self.proc = subprocess.Popen(
            ['git', 'update-index', '-z', '--index-info'], cwd=root,
            stdin=subprocess.PIPE, stdout=subprocess.DEVNULL,
            stderr=self.stderr_file)
        self.count = 0

    def _error(self):
        try:
            self.stderr_file.seek(0)
            message = self.stderr_file.read().decode('utf-8', 'replace')
        except Exception:
            message = ''
        return SyncError('git update-index failed: %s' % message.strip()[:2000])

    def _write(self, data):
        try:
            self.proc.stdin.write(data)
        except (BrokenPipeError, OSError):
            raise self._error()
        self.count += 1

    def add(self, mode, oid, path_bytes):
        self._write(b'%s %s\t%s\0' % (mode.encode('ascii'),
                                       oid.encode('ascii'), path_bytes))

    def remove(self, path_bytes):
        self._write(b'0 %s\t%s\0' % (b'0' * 40, path_bytes))

    def close(self):
        try:
            self.proc.stdin.close()
        except OSError:
            pass
        returncode = self.proc.wait()
        error = None
        if returncode != 0:
            error = self._error()
        self.stderr_file.close()
        if error is not None:
            raise error


class SymlinkBlobStore:
    '''Materialise symlink targets as small files under .git/sync-preparation
    so that the single long lived hash-object process can hash them.  Only
    files created by this run are removed again.'''

    def __init__(self, root):
        self.dir = os.path.join(root, PLAN_DIR_REL, 'symlink-blobs')
        self.created = []
        self.by_digest = {}
        os.makedirs(self.dir, exist_ok=True)

    def materialize(self, target_bytes):
        digest = hashlib.sha256(target_bytes).hexdigest()
        rel = self.by_digest.get(digest)
        if rel is None:
            name = digest + '.blob'
            path = os.path.join(self.dir, name)
            with open(path, 'wb') as handle:
                handle.write(target_bytes)
            self.created.append(path)
            rel = SYMLINK_BLOBS_REL + '/' + name
            self.by_digest[digest] = rel
        return rel

    def cleanup(self):
        for path in self.created:
            try:
                os.unlink(path)
            except OSError:
                pass
        self.created = []
        try:
            if not os.listdir(self.dir):
                os.rmdir(self.dir)
        except OSError:
            pass


# --- stage -----------------------------------------------------------------

def _precheck_entry(root, entry):
    rel = entry.get('path')
    if not rel:
        return 'entry without path'
    full = _fs(root, rel)
    try:
        st = os.lstat(full)
    except OSError as exc:
        return '%s: %s' % (rel, exc.strerror or exc)
    if entry.get('kind') == 'symlink':
        if not stat.S_ISLNK(st.st_mode):
            return '%s: expected a symlink' % rel
        if entry.get('mode') not in (None, '120000'):
            return '%s: unexpected symlink mode %s' % (rel, entry.get('mode'))
        try:
            target = os.readlink(full)
        except OSError as exc:
            return '%s: %s' % (rel, exc.strerror or exc)
        if target != entry.get('link_target'):
            return '%s: symlink target changed' % rel
        return None
    if not stat.S_ISREG(st.st_mode):
        return '%s: no longer a regular file' % rel
    actual_mode = '100755' if (st.st_mode & 0o111) else '100644'
    if entry.get('mode') and actual_mode != entry.get('mode'):
        return '%s: mode changed (%s != %s)' % (rel, actual_mode,
                                                entry.get('mode'))
    if st.st_size != entry.get('size'):
        return '%s: size changed (%s != %s)' % (rel, st.st_size,
                                                entry.get('size'))
    if entry.get('mtime_ns') is not None and st.st_mtime_ns != entry['mtime_ns']:
        return '%s: mtime changed' % rel
    return None


def _require_sha256(value, what):
    if not isinstance(value, str) or not SHA256_RE.fullmatch(value):
        raise SyncError('%s must be 64 lowercase hex characters: %r'
                        % (what, value))
    return value


def _chunks_ok(chunk_dir, record):
    chunks = record.get('chunks') or []
    if not chunks:
        return False
    whole = hashlib.sha256()
    total = 0
    for chunk in chunks:
        name = chunk.get('sha256')
        _require_sha256(name, '%s: chunk sha256' % record.get('path'))
        path = os.path.join(chunk_dir, name)
        try:
            st = os.lstat(path)
        except OSError:
            return False
        if not stat.S_ISREG(st.st_mode) or st.st_size != chunk.get('size'):
            return False
        digest = hashlib.sha256()
        count = 0
        with open(path, 'rb') as handle:
            while True:
                block = handle.read(READ_BLOCK)
                if not block:
                    break
                digest.update(block)
                whole.update(block)
                count += len(block)
        if count != chunk.get('size'):
            return False
        if digest.hexdigest() != name:
            raise SyncError(
                'existing chunk %s for %s is corrupted (sha256 mismatch); '
                'refusing to stage; remove the chunk and re-run plan'
                % (name, record.get('path')))
        total += count
    if total != record.get('size'):
        return False
    whole_sha = record.get('sha256')
    _require_sha256(whole_sha, '%s: manifest sha256' % record.get('path'))
    if whole.hexdigest() != whole_sha:
        raise SyncError(
            'existing chunks for %s do not rebuild the recorded sha256; '
            'refusing to stage' % record.get('path'))
    return True


def _split_into_chunks(root, rel, chunk_size, chunk_dir):
    source = _fs(root, rel)
    whole = hashlib.sha256()
    chunks = []
    wrote_new = False
    with open(source, 'rb') as handle:
        while True:
            block = handle.read(chunk_size)
            if not block:
                break
            digest = hashlib.sha256(block).hexdigest()
            chunks.append({'sha256': digest, 'size': len(block)})
            whole.update(block)
            destination = os.path.join(chunk_dir, digest)
            try:
                st = os.lstat(destination)
                if stat.S_ISREG(st.st_mode) and st.st_size == len(block) and \
                        _file_sha256(destination) == digest:
                    continue
            except OSError:
                pass
            fd, tmp = tempfile.mkstemp(dir=chunk_dir, prefix='.tmp-chunk-')
            try:
                with os.fdopen(fd, 'wb') as out:
                    out.write(block)
                os.replace(tmp, destination)
            except BaseException:
                try:
                    os.unlink(tmp)
                except OSError:
                    pass
                raise
            wrote_new = True
    return whole.hexdigest(), chunks, wrote_new


def _verify_chunks(chunk_dir, rel, chunks, whole_sha, size):
    '''Read the chunk files back: sizes, per chunk sha256 and whole file sha256.'''
    whole = hashlib.sha256()
    total = 0
    for chunk in chunks:
        path = os.path.join(chunk_dir, chunk['sha256'])
        digest = hashlib.sha256()
        count = 0
        with open(path, 'rb') as handle:
            while True:
                block = handle.read(READ_BLOCK)
                if not block:
                    break
                digest.update(block)
                whole.update(block)
                count += len(block)
        if count != chunk['size']:
            raise SyncError('%s: chunk %s has size %d, expected %d'
                            % (rel, chunk['sha256'], count, chunk['size']))
        if digest.hexdigest() != chunk['sha256']:
            raise SyncError('%s: chunk file %s is corrupted'
                            % (rel, chunk['sha256']))
        total += count
    if total != size:
        raise SyncError('%s: chunk sizes add up to %d, expected %d'
                        % (rel, total, size))
    if whole.hexdigest() != whole_sha:
        raise SyncError('%s: rebuilt sha256 mismatch after chunking' % rel)


def _stage_large_files(root, large_path, threshold, chunk_size, hasher,
                       updater, staged_handle, new_paths, chunk_dir, counters):
    existing = {}
    manifest_path = _fs(root, MANIFEST_REL)
    if os.path.isfile(manifest_path):
        try:
            old = _load_json(manifest_path)
            for record in old.get('files', []):
                if record.get('path'):
                    existing[record['path']] = record
        except (ValueError, OSError):
            existing = {}

    records = []
    indexed_chunks = set()
    for entry in _iter_jsonl(large_path):
        rel = entry['path']
        if (entry.get('size') or 0) <= threshold:
            raise SyncError('large entry %s is not above the threshold; re-run plan'
                            % rel)
        previous = existing.get(rel)
        reused = bool(
            previous
            and previous.get('size') == entry.get('size')
            and previous.get('mtime_ns') == entry.get('mtime_ns')
            and previous.get('mode') == entry.get('mode')
            and _chunks_ok(chunk_dir, previous))
        if reused:
            whole = previous['sha256']
            chunks = previous['chunks']
        else:
            whole, chunks, _wrote_new = _split_into_chunks(root, rel, chunk_size,
                                                          chunk_dir)
            _verify_chunks(chunk_dir, rel, chunks, whole, entry['size'])

        st = os.lstat(_fs(root, rel))
        if not stat.S_ISREG(st.st_mode):
            raise SyncError('large file %s changed type while chunking; re-run plan' % rel)
        actual_mode = '100755' if (st.st_mode & 0o111) else '100644'
        if (st.st_size != entry.get('size')
                or st.st_mtime_ns != entry.get('mtime_ns')
                or actual_mode != (entry.get('mode') or '100644')):
            raise SyncError('large file %s changed while chunking; re-run plan' % rel)

        for chunk in chunks:
            chunk_rel = CHUNK_DIR_REL + '/' + chunk['sha256']
            if chunk_rel in indexed_chunks:
                continue
            chunk_bytes = os.fsencode(chunk_rel)
            oid = hasher.hash_path(chunk_bytes)
            updater.add('100644', oid, chunk_bytes)
            staged_handle.write(chunk_bytes + b'\0')
            new_paths.add(chunk_bytes)
            indexed_chunks.add(chunk_rel)

        records.append({'path': rel, 'kind': 'file', 'size': entry['size'],
                        'mode': entry.get('mode') or '100644',
                        'mtime_ns': entry.get('mtime_ns'),
                        'sha256': whole, 'chunks': chunks})
        counters['chunks'] += len(chunks)
        _log('chunked %s (%d bytes, %d chunks%s)'
             % (rel, entry['size'], len(chunks), ', reused' if reused else ''))
    return records


def cmd_stage(root):
    root = os.path.abspath(root)
    _require_git(root)
    threshold = _threshold()
    chunk_size = _chunk_size()

    plan_dir = os.path.join(root, PLAN_DIR_REL)
    include_path = os.path.join(plan_dir, 'include.jsonl')
    large_path = os.path.join(plan_dir, 'large.jsonl')
    summary_path = os.path.join(plan_dir, 'summary.json')
    for path in (include_path, large_path, summary_path):
        if not os.path.isfile(path):
            raise SyncError('missing plan file %s; run the plan command first'
                            % path)
    summary = _load_json(summary_path)
    if int(summary.get('threshold_bytes', -1)) != threshold:
        raise SyncError('plan was produced with threshold %r, current threshold is %d; re-run plan'
                        % (summary.get('threshold_bytes'), threshold))
    plan_errors = summary.get('errors') or []
    if summary.get('ok') is False or plan_errors:
        raise SyncError('plan is incomplete (%d errors); fix and re-run plan: %s'
                        % (len(plan_errors), '; '.join(str(item) for item in plan_errors[:5])))

    # 1. plan must still describe the tree on disk - check before writing anything
    problems = []
    for path in (include_path, large_path):
        for entry in _iter_jsonl(path):
            problem = _precheck_entry(root, entry)
            if problem:
                problems.append(problem)
                if len(problems) >= 20:
                    break
        if len(problems) >= 20:
            break
    if problems:
        raise SyncError('plan is stale (%d changed entries, e.g. %s); re-run plan'
                        % (len(problems), '; '.join(problems[:5])))

    # 2. only touch an index that this script staged itself
    index_now = _git_ls_files(root)
    staged_paths_path = os.path.join(plan_dir, 'staged-paths.bin')
    previous = set(_read_nul(staged_paths_path)) \
        if os.path.isfile(staged_paths_path) else set()
    if index_now and not previous:
        raise SyncError('the git index is not empty and was not created by this script (e.g. %s); refusing to modify it'
                        % os.fsdecode(next(iter(index_now))))
    if previous:
        unexpected = [path for path in index_now if path not in previous]
        if unexpected:
            raise SyncError('the git index contains %d entries this script did not stage (e.g. %s); refusing to modify the index'
                            % (len(unexpected), os.fsdecode(unexpected[0])))

    # 3. stage
    chunk_dir = os.path.join(root, CHUNK_DIR_REL)
    os.makedirs(chunk_dir, exist_ok=True)
    staged_tmp = staged_paths_path + '.tmp'
    updater = GitUpdateIndex(root)
    hasher = GitHashObject(root)
    symlinks = SymlinkBlobStore(root)

    counters = {'files': 0, 'bytes': 0, 'symlinks': 0, 'chunks': 0,
                'manifest_entries': 0}
    new_paths = set()
    removed = []
    error = None

    try:
        with open(staged_tmp, 'wb') as staged_handle:
            for path in sorted(previous):
                staged_handle.write(path + b'\0')

            progress = {'files': 0, 'bytes': 0, 'time': time.time()}

            def report():
                now = time.time()
                if (counters['files'] - progress['files'] >= 5000
                        or counters['bytes'] - progress['bytes'] >= (1 << 30)
                        or now - progress['time'] >= 30.0):
                    _log('stage: %d files, %d MiB hashed'
                         % (counters['files'], counters['bytes'] // MIB))
                    progress['files'] = counters['files']
                    progress['bytes'] = counters['bytes']
                    progress['time'] = now

            batch = []
            batch_bytes = 0

            def flush_batch():
                nonlocal batch, batch_bytes
                if not batch:
                    return
                paths = [os.fsencode(item['path']) for item in batch]
                oids = hasher.hash_paths(paths)
                for item in batch:
                    problem = _precheck_entry(root, item)
                    if problem:
                        raise SyncError('include entry changed while hashing '
                                        '(%s); re-run plan' % problem)
                for item, rel_bytes, oid in zip(batch, paths, oids):
                    updater.add(item['mode'], oid, rel_bytes)
                    staged_handle.write(rel_bytes + b'\0')
                    new_paths.add(rel_bytes)
                    counters['files'] += 1
                    counters['bytes'] += item.get('size') or 0
                batch = []
                batch_bytes = 0
                report()

            for entry in _iter_jsonl(include_path):
                if entry.get('kind') == 'symlink':
                    flush_batch()
                    temp_rel = symlinks.materialize(os.fsencode(entry['link_target']))
                    oid = hasher.hash_path(os.fsencode(temp_rel))
                    problem = _precheck_entry(root, entry)
                    if problem:
                        raise SyncError('symlink changed while staging '
                                        '(%s); re-run plan' % problem)
                    rel_bytes = os.fsencode(entry['path'])
                    updater.add('120000', oid, rel_bytes)
                    staged_handle.write(rel_bytes + b'\0')
                    new_paths.add(rel_bytes)
                    counters['symlinks'] += 1
                    counters['files'] += 1
                    continue
                batch.append(entry)
                batch_bytes += len(entry['path']) + 1
                if len(batch) >= HASH_BATCH_MAX or batch_bytes >= HASH_BATCH_BYTES:
                    flush_batch()
            flush_batch()

            records = _stage_large_files(root, large_path, threshold, chunk_size,
                                         hasher, updater, staged_handle,
                                         new_paths, chunk_dir, counters)
            manifest = {'version': SCRIPT_VERSION, 'command': 'stage',
                        'root': root, 'generated_at': _now_iso(),
                        'chunk_size': chunk_size,
                        'threshold_bytes': threshold, 'files': records}
            _write_json_atomic(_fs(root, MANIFEST_REL), manifest)
            manifest_bytes = os.fsencode(MANIFEST_REL)
            updater.add('100644', hasher.hash_path(manifest_bytes), manifest_bytes)
            staged_handle.write(manifest_bytes + b'\0')
            new_paths.add(manifest_bytes)
            counters['manifest_entries'] = len(records)

            if previous:
                for rel_bytes in sorted(previous):
                    if rel_bytes not in new_paths:
                        updater.remove(rel_bytes)
                        removed.append(rel_bytes)
    except BaseException as exc:
        error = exc
    finally:
        for closer in (updater.close, hasher.close):
            try:
                closer()
            except Exception as exc:
                _log('warning: %s' % exc)
                if error is None:
                    error = exc
        symlinks.cleanup()

    if error is None:
        try:
            _write_nul(staged_paths_path, sorted(new_paths))
            os.unlink(staged_tmp)
        except OSError as exc:
            error = exc

    if error is not None:
        # keep whatever we managed so far so that a re-run can continue
        try:
            os.replace(staged_tmp, staged_paths_path)
        except OSError:
            pass
        raise error

    index_after = _git_ls_files(root)
    leftover = [os.fsdecode(path) for path in removed if path in index_after]
    if leftover:
        raise SyncError('failed to remove %d stale index entries (e.g. %s)'
                        % (len(leftover), leftover[0]))
    gitlinks = [path for path, (mode, _oid) in index_after.items()
                if mode == '160000']
    if gitlinks:
        raise SyncError('index contains gitlink entries after staging: %s'
                        % os.fsdecode(gitlinks[0]))

    state = {'version': SCRIPT_VERSION, 'command': 'stage', 'root': root,
             'generated_at': _now_iso(), 'threshold_bytes': threshold,
             'chunk_size': chunk_size, 'index_entries': len(index_after),
             'plan_summary_sha256': _file_sha256(summary_path),
             'staged_paths': STAGED_PATHS_REL}
    _write_json_atomic(os.path.join(plan_dir, 'stage-state.json'), state)

    return {'ok': True, 'command': 'stage', 'root': root,
            'index_entries': len(index_after),
            'files': counters['files'], 'symlinks': counters['symlinks'],
            'large_files': counters['manifest_entries'],
            'chunks': counters['chunks'], 'removed': len(removed),
            'manifest': MANIFEST_REL}


# --- restore ---------------------------------------------------------------

def _validate_rel(rel):
    if not isinstance(rel, str) or rel == '':
        raise SyncError('manifest entry with empty path')
    if '\0' in rel:
        raise SyncError('manifest path contains NUL')
    if rel.startswith('/') or rel.startswith(BACKSLASH) or ABS_PATH_RE.match(rel):
        raise SyncError('manifest path is absolute: %r' % rel)
    parts = rel.split('/')
    for part in parts:
        if part in ('', '.', '..'):
            raise SyncError('manifest path contains an unsafe component: %r' % rel)
        if part in ('.git', SYNC_DIR_REL):
            raise SyncError('manifest path would write tool/git metadata: %r' % rel)
    return parts


def _check_ancestors(dest, parts):
    current = dest
    for part in parts[:-1]:
        current = os.path.join(current, part)
        if os.path.islink(current):
            raise SyncError('refusing to write through a symlinked directory: %s'
                            % current)
        if os.path.exists(current) and not os.path.isdir(current):
            raise SyncError('path component is not a directory: %s' % current)


def _check_inside(dest, target):
    dest_real = os.path.realpath(dest)
    target_real = os.path.realpath(target)
    if target_real != dest_real and not target_real.startswith(dest_real + os.sep):
        raise SyncError('path escapes the destination: %s' % target)


def _restore_one(dest, chunk_dir, record, overwrite):
    rel = record.get('path')
    parts = _validate_rel(rel)
    target = os.path.join(dest, *parts)
    _check_ancestors(dest, parts)
    _check_inside(dest, target)

    size = record.get('size')
    whole_sha = _require_sha256(record.get('sha256'),
                                '%s: manifest sha256' % rel)
    chunks = record.get('chunks') or []
    if not chunks:
        raise SyncError('%s: manifest entry has no chunks' % rel)

    if os.path.lexists(target):
        st = os.lstat(target)
        if stat.S_ISLNK(st.st_mode):
            raise SyncError('%s: destination is a symlink; refusing' % rel)
        if not stat.S_ISREG(st.st_mode):
            raise SyncError('%s: destination is not a regular file' % rel)
        if st.st_size == size and _file_sha256(target) == whole_sha:
            return 'skipped'
        if not overwrite:
            raise SyncError('%s: destination exists with different content; '
                            'refusing without --overwrite' % rel)

    directory = os.path.dirname(target)
    os.makedirs(directory, exist_ok=True)
    fd, tmp = tempfile.mkstemp(dir=directory, prefix='.github-sync-restore-')
    whole = hashlib.sha256()
    total = 0
    try:
        with os.fdopen(fd, 'wb') as out:
            for chunk in chunks:
                chunk_sha = _require_sha256(chunk.get('sha256'),
                                            '%s: chunk sha256' % rel)
                chunk_path = os.path.join(chunk_dir, chunk_sha)
                try:
                    chunk_st = os.lstat(chunk_path)
                except OSError:
                    raise SyncError('%s: missing chunk %s' % (rel, chunk_sha))
                if not stat.S_ISREG(chunk_st.st_mode):
                    raise SyncError('%s: chunk %s is not a regular file'
                                    % (rel, chunk_sha))
                digest = hashlib.sha256()
                count = 0
                with open(chunk_path, 'rb') as handle:
                    while True:
                        block = handle.read(READ_BLOCK)
                        if not block:
                            break
                        digest.update(block)
                        whole.update(block)
                        out.write(block)
                        count += len(block)
                if count != chunk.get('size'):
                    raise SyncError('%s: chunk %s has size %d, expected %s'
                                    % (rel, chunk_sha, count,
                                       chunk.get('size')))
                if digest.hexdigest() != chunk_sha:
                    raise SyncError('%s: chunk %s is corrupted'
                                    % (rel, chunk_sha))
                total += count
        if total != size:
            raise SyncError('%s: rebuilt size %d, expected %s' % (rel, total, size))
        if whole.hexdigest() != whole_sha:
            raise SyncError('%s: rebuilt sha256 does not match the manifest' % rel)
        mode = 0o755 if str(record.get('mode', '')).endswith('755') else 0o644
        os.chmod(tmp, mode)
        os.replace(tmp, target)
    except BaseException:
        try:
            os.unlink(tmp)
        except OSError:
            pass
        raise
    return 'restored'


def cmd_restore(root, destination=None, overwrite=False):
    root = os.path.abspath(root)
    manifest_path = _fs(root, MANIFEST_REL)
    if not os.path.isfile(manifest_path):
        raise SyncError('manifest not found: %s' % manifest_path)
    manifest = _load_json(manifest_path)
    dest = os.path.abspath(destination) if destination else root
    chunk_dir = _fs(root, CHUNK_DIR_REL)

    restored = 0
    skipped = 0
    problems = []
    for record in manifest.get('files', []):
        try:
            action = _restore_one(dest, chunk_dir, record, overwrite)
        except SyncError as exc:
            problems.append(str(exc))
            continue
        if action == 'restored':
            restored += 1
        else:
            skipped += 1

    return {'ok': not problems, 'command': 'restore', 'root': root,
            'destination': dest, 'restored': restored, 'skipped': skipped,
            'problems': problems}


# --- audit -----------------------------------------------------------------

def _cat_file_sizes(root, oids):
    '''Map oid -> (type, size) using one long lived git cat-file process.'''
    result = {}
    oids = [oid for oid in oids if oid]
    if not oids:
        return result
    proc = subprocess.Popen(['git', 'cat-file', '--batch-check'], cwd=root,
                            stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                            stderr=subprocess.DEVNULL)
    try:
        for index in range(0, len(oids), CAT_BATCH):
            batch = oids[index:index + CAT_BATCH]
            proc.stdin.write(b''.join(oid.encode('ascii') + b'\n'
                                      for oid in batch))
            proc.stdin.flush()
            for _oid in batch:
                line = proc.stdout.readline()
                if not line:
                    raise SyncError('git cat-file terminated early')
                fields = line.decode('ascii', 'replace').split()
                if not fields:
                    continue
                if len(fields) >= 3:
                    try:
                        result[fields[0]] = (fields[1], int(fields[2]))
                    except ValueError:
                        result[fields[0]] = (fields[1], 0)
                else:
                    result[fields[0]] = ('missing', 0)
    finally:
        try:
            proc.stdin.close()
        except OSError:
            pass
        proc.wait()
        try:
            proc.stdout.close()
        except OSError:
            pass
    return result


def cmd_audit(root):
    root = os.path.abspath(root)
    _require_git(root)
    threshold = _threshold()
    issues = []
    stats = {'threshold_bytes': threshold}

    index = _git_ls_files(root)
    stats['index_entries'] = len(index)

    gitlinks = sorted(path for path, (mode, _oid) in index.items()
                      if mode == '160000')
    if gitlinks:
        issues.append('index contains %d gitlink entries (mode 160000), e.g. %s'
                      % (len(gitlinks), os.fsdecode(gitlinks[0])))

    blob_oids = sorted({oid for (mode, oid) in index.values()
                        if mode != '160000'})
    stats['blobs'] = len(blob_oids)
    sizes = _cat_file_sizes(root, blob_oids)

    too_big = []
    for path, (mode, oid) in index.items():
        if mode == '160000':
            continue
        _type, size = sizes.get(oid, ('missing', 0))
        if size > threshold:
            too_big.append((os.fsdecode(path), size))
    if too_big:
        issues.append('%d index blobs are larger than %d bytes, e.g. %s (%d)'
                      % (len(too_big), threshold, too_big[0][0], too_big[0][1]))

    include_path = _fs(root, INCLUDE_REL)
    large_path = _fs(root, LARGE_REL)
    summary_path = _fs(root, SUMMARY_REL)
    if os.path.isfile(summary_path):
        summary = _load_json(summary_path)
        stats['plan_counts'] = summary.get('counts')
        stats['plan_exclude_reasons'] = summary.get('exclude_reasons')
    stats['plan_include'] = 0
    stats['plan_large'] = 0

    if os.path.isfile(include_path):
        expected = {}
        for entry in _iter_jsonl(include_path):
            rel_bytes = os.fsencode(entry['path'])
            expected[rel_bytes] = (entry.get('size'),
                                   '120000' if entry.get('kind') == 'symlink'
                                   else (entry.get('mode') or '100644'))
        stats['plan_include'] = len(expected)
        missing = []
        for rel_bytes, (size, mode) in expected.items():
            if rel_bytes not in index:
                missing.append(os.fsdecode(rel_bytes) + ' (absent from index)')
                continue
            got_mode, oid = index[rel_bytes]
            if got_mode != mode:
                missing.append('%s (mode %s != %s)'
                               % (os.fsdecode(rel_bytes), got_mode, mode))
                continue
            _type, blob_size = sizes.get(oid, ('missing', 0))
            if size is not None and blob_size != size:
                missing.append('%s (blob size %d != plan %s)'
                               % (os.fsdecode(rel_bytes), blob_size, size))
        if missing:
            issues.append('%d planned files are missing or wrong in the index, e.g. %s'
                          % (len(missing), missing[:5]))
    else:
        issues.append('no plan found at %s; run plan first' % INCLUDE_REL)

    manifest = None
    manifest_path = _fs(root, MANIFEST_REL)
    if os.path.isfile(manifest_path):
        try:
            manifest = _load_json(manifest_path)
        except ValueError as exc:
            issues.append('manifest is not valid JSON: %s' % exc)

    if os.path.isfile(large_path):
        by_path = {}
        for record in (manifest or {}).get('files', []):
            if record.get('path'):
                by_path[record['path']] = record
        chunk_dir = _fs(root, CHUNK_DIR_REL)
        bad = []
        for entry in _iter_jsonl(large_path):
            stats['plan_large'] += 1
            rel = entry['path']
            record = by_path.get(rel)
            if not record:
                bad.append(rel + ' (no manifest entry)')
                continue
            if record.get('size') != entry.get('size') \
                    or record.get('mode') != entry.get('mode'):
                bad.append(rel + ' (manifest size/mode mismatch)')
            chunks = record.get('chunks') or []
            chunk_total = sum(chunk.get('size') or 0 for chunk in chunks)
            if chunk_total != record.get('size'):
                bad.append(rel + ' (chunk sizes do not add up)')
            for chunk in chunks:
                chunk_file = os.path.join(chunk_dir, chunk.get('sha256', ''))
                if not os.path.isfile(chunk_file) \
                        or os.path.getsize(chunk_file) != chunk.get('size'):
                    bad.append('%s (chunk %s missing or wrong size)'
                               % (rel, chunk.get('sha256')))
                    break
                if os.fsencode(CHUNK_DIR_REL + '/' + chunk['sha256']) not in index:
                    bad.append('%s (chunk %s not in the index)'
                               % (rel, chunk.get('sha256')))
                    break
            if os.fsencode(rel) in index:
                bad.append(rel + ' (large file is directly indexed)')
        if bad:
            issues.append('%d large files are not covered by the manifest, e.g. %s'
                          % (len(bad), bad[:5]))
        if manifest is None:
            issues.append('large files are planned but %s is missing' % MANIFEST_REL)

    return {'ok': not issues, 'command': 'audit', 'root': root,
            'stats': stats, 'issues': issues}


# --- cli -------------------------------------------------------------------

def main(argv=None):
    parser = argparse.ArgumentParser(
        prog='github_sync.py',
        description='plan/stage/restore/audit a work tree for a private GitHub mirror')
    subparsers = parser.add_subparsers(dest='command', required=True)
    for name in ('plan', 'stage', 'audit'):
        sub = subparsers.add_parser(name)
        sub.add_argument('--root', required=True)
    restore = subparsers.add_parser('restore')
    restore.add_argument('--root', required=True)
    restore.add_argument('--destination', default=None)
    restore.add_argument('--overwrite', action='store_true')

    args = parser.parse_args(argv)
    try:
        if args.command == 'plan':
            result = cmd_plan(args.root)
        elif args.command == 'stage':
            result = cmd_stage(args.root)
        elif args.command == 'restore':
            result = cmd_restore(args.root, args.destination, args.overwrite)
        else:
            result = cmd_audit(args.root)
        returncode = 0 if result.get('ok', True) else 1
    except (SyncError, OSError) as exc:
        result = {'ok': False, 'command': args.command, 'error': str(exc)}
        returncode = 1
    sys.stdout.write(json.dumps(result, ensure_ascii=True, indent=2,
                                sort_keys=True) + '\n')
    sys.stdout.flush()
    return returncode


if __name__ == '__main__':
    sys.exit(main())

