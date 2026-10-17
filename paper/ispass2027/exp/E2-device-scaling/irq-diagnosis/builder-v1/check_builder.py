#!/usr/bin/env python3
"""Bounded host checks; never enter finalization, source export or compilation."""
import argparse
import copy
import importlib.util
import json
from pathlib import Path
import shutil
import sys
import tempfile
from unittest.mock import patch

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepared', type=Path, required=True)
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location('e2_probe_builder', HERE/'build_timer_probe.py')
    builder = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(builder)
    base = builder.library()
    prepared = args.prepared.resolve()
    manifest_path = prepared/'build-manifest.json'
    manifest_bytes = manifest_path.read_bytes()
    manifest = json.loads(manifest_bytes)
    reference = builder.reference_manifest(base)
    results = []
    forbidden_calls = []

    def forbidden(*arguments, **keywords):
        forbidden_calls.append(str(arguments))
        raise AssertionError('source export or build command is forbidden in host checks')

    # All checks below can read Git/source data. No source archive or build runs.
    base.archive = forbidden
    base.run = forbidden

    def accept(name, function):
        function()
        results.append({'name': name, 'result': 'accepted'})

    def reject(name, function, expected):
        try:
            function()
        except (ValueError, FileExistsError) as error:
            if expected not in str(error):
                raise AssertionError((name, str(error), expected)) from error
            results.append({'name': name, 'result': 'rejected', 'error': str(error)})
        else:
            raise AssertionError('unexpected acceptance: ' + name)

    accept('actual frozen platform/Linux/source lock tuple',
           lambda: builder.validate_tuple(base, builder.PLATFORM_COMMIT, builder.M))
    accept('actual passed reference rootfs/7 binaries/config/DTS/UAPI',
           lambda: builder.validate_inputs(base, builder.BASE_SW, reference))
    accept('actual prepared package',
           lambda: builder.validate_prepared(base, prepared, manifest))
    reject('base platform cannot substitute probe platform',
           lambda: builder.validate_tuple(base, builder.BASE_PLATFORM, builder.M), 'frozen E2 probe platform only')
    reject('base Linux cannot substitute probe Linux',
           lambda: builder.validate_tuple(base, builder.PLATFORM_COMMIT, builder.BASE_LINUX), 'frozen E2 probe Linux only')

    original_show = base.show
    lock = json.loads(original_show(builder.PLATFORM, builder.PLATFORM_COMMIT, 'sources.lock.json'))
    for name, mutate, error in [
        ('duplicate source lock component', lambda obj: obj['components'].append(copy.deepcopy(obj['components'][0])),
         'duplicate source lock path'),
        ('changed OpenSBI source lock', lambda obj: obj['components'][2].update(commit='0'*40),
         'source lock changed beyond Linux'),
    ]:
        changed = copy.deepcopy(lock)
        mutate(changed)

        def altered_show(repo, commit, name, replacement=json.dumps(changed).encode()):
            if repo == builder.PLATFORM and commit == builder.PLATFORM_COMMIT and name == 'sources.lock.json':
                return replacement
            return original_show(repo, commit, name)

        with patch.object(base, 'show', altered_show):
            reject(name, lambda: builder.validate_tuple(base, builder.PLATFORM_COMMIT, builder.M), error)

    original_git = base.git

    def altered_git(repo, *arguments, **keywords):
        if repo == builder.PLATFORM and arguments == ('rev-parse', builder.PLATFORM_COMMIT+':opensbi'):
            return b'0000000000000000000000000000000000000000\n'
        return original_git(repo, *arguments, **keywords)

    with patch.object(base, 'git', altered_git):
        reject('gitlink and source lock disagree',
               lambda: builder.validate_tuple(base, builder.PLATFORM_COMMIT, builder.M), 'gitlink tuple mismatch: opensbi')

    for state in ['preparing', 'building', 'failed', 'passed']:
        changed = dict(manifest, status=state)
        reject('no retry/overwrite from ' + state,
               lambda changed=changed: builder.validate_prepared(base, prepared, changed), 'no retry/overwrite')
    reject('changed builder after preparation',
           lambda: builder.validate_prepared(base, prepared, dict(manifest, script_sha256='0'*64)),
           'builder changed after preparation')
    reject('changed prepared tuple',
           lambda: builder.validate_prepared(base, prepared, dict(manifest, selected_linux=builder.BASE_LINUX)),
           'prepared tuple changed')

    # Corrupt isolated copies only. The passed package and prepared package remain read-only.
    with tempfile.TemporaryDirectory(prefix='host-check-', dir=HERE/'artifacts') as temporary:
        fixture = Path(temporary)/'package'
        shutil.copytree(prepared, fixture)
        for relative, error in [('rootfs.cpio', 'rootfs bytes changed'),
                                ('bin/idma_scaling', 'CLI bytes changed: idma_scaling'),
                                ('inputs/configs/platform.dts', 'CLI/config/DTS inputs changed'),
                                ('uapi/linux/idma.h', 'preserved UAPI changed')]:
            path = fixture/relative
            original = path.read_bytes()
            path.write_bytes(original + b'\nHOST_CHECK_CORRUPTION\n')
            reject('changed ' + relative,
                   lambda: builder.validate_inputs(base, fixture, reference), error)
            path.write_bytes(original)

        config = fixture/'check.config'
        text = (fixture/'reference.config').read_text()
        allowed = '\n'.join('CONFIG_INITRAMFS_SOURCE="/isolated/fixture/rootfs.cpio"' if line.startswith('CONFIG_INITRAMFS_SOURCE=')
                            else 'CONFIG_LOCALVERSION="'+builder.RELEASE_SUFFIX+'"' if line.startswith('CONFIG_LOCALVERSION=')
                            else line for line in text.splitlines()) + '\n'
        config.write_text(allowed)
        accept('kernel config differs only by rootfs path/release',
               lambda: builder.validate_kernel_config(config, fixture/'reference.config'))
        for name, before, after in [('HZ changed', 'CONFIG_HZ=250', 'CONFIG_HZ=251'),
                                    ('watchdog changed', 'CONFIG_SOFTLOCKUP_DETECTOR=y', '# CONFIG_SOFTLOCKUP_DETECTOR is not set')]:
            if before not in allowed:
                raise AssertionError('reference option absent: ' + before)
            config.write_text(allowed.replace(before, after))
            reject(name, lambda: builder.validate_kernel_config(config, fixture/'reference.config'),
                   'generated kernel config changed beyond rootfs path/release')

        # Exercise pre-output action guards. No finalization action is invoked.
        for name, out, jobs, platform, error in [
            ('prepare refuses zero jobs', fixture/'unused', '0', builder.PLATFORM_COMMIT, 'jobs must be positive'),
            ('prepare refuses output outside task', Path(temporary).parent.parent/'outside-task', '1', builder.PLATFORM_COMMIT,
             'out must stay inside this E2 task artifacts/'),
            ('prepare refuses artifacts root itself', HERE/'artifacts', '1', builder.PLATFORM_COMMIT,
             'out must stay inside this E2 task artifacts/'),
            ('prepare refuses wrong platform before mkdir', fixture/'unused', '1', builder.BASE_PLATFORM,
             'frozen E2 probe platform only'),
            ('prepare refuses existing prepared output', prepared, '1', builder.PLATFORM_COMMIT, 'File exists'),
        ]:
            argv = ['build_timer_probe.py', 'prepare', '--out', str(out), '--jobs', jobs,
                    '--platform-commit', platform, '--linux-commit', builder.M]
            with patch.object(builder, 'library', lambda: base), patch.object(sys, 'argv', argv):
                reject(name, builder.main, error)
        if (fixture/'unused').exists():
            raise AssertionError('rejected preflight created output')

    accept('payload exactly 32MiB', lambda: builder.validate_payload_size(builder.MAX_PAYLOAD))
    for size in [0, -1, builder.MAX_PAYLOAD+1]:
        reject('payload rejected at size '+str(size), lambda size=size: builder.validate_payload_size(size),
               '32MiB SD payload budget exceeded')
    if forbidden_calls or manifest_path.read_bytes() != manifest_bytes:
        raise AssertionError('host checks mutated prepared manifest or attempted a source export/build')
    if list(prepared.glob('src')) or list(prepared.glob('linux-build')) or list(prepared.rglob('Image')):
        raise AssertionError('unexpected source export or Image in prepared output')
    result = {'schema': 'e2-timer-probe-host-checks-v1', 'status': 'passed', 'checked_utc': base.now(),
              'builder_sha256': base.sha(HERE/'build_timer_probe.py'), 'check_script_sha256': base.sha(__file__),
              'prepared_manifest_sha256': base.sha(manifest_path), 'checks': results,
              'checks_total': len(results), 'actual_source_exports': 0, 'actual_build_commands': 0,
              'finalize_action_invocations': 0, 'board_operations': 0,
              'limits': 'Host guards and isolated corruptions only; no runtime probe, compiler, SPI module or payload-size result.'}
    base.save(args.report, result)
    print(json.dumps({'status': 'passed', 'checks': len(results), 'report': str(args.report)}))


if __name__ == '__main__':
    main()
