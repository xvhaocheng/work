#!/usr/bin/env python3
import json, os, subprocess, tempfile
from pathlib import Path

root = Path(__file__).resolve().parents[1]

with tempfile.TemporaryDirectory() as td:
    t = Path(td)
    (t / 'linux').mkdir()
    (t / 'linux/idma.h').write_bytes(
        (root / 'linux/include/uapi/linux/idma.h').read_bytes()
    )
    exe = t / 'e3_controlled'

    subprocess.run([
        'cc', '-std=gnu11', '-O2', '-Wall', '-Wextra', '-Werror',
        '-I', str(t),
        '-Wl,--wrap=open', '-Wl,--wrap=ioctl', '-Wl,--wrap=close',
        str(root / 'bench/idma_e3_controlled.c'),
        str(root / 'tests/e3_controlled_cli_stub.c'),
        '-o', str(exe),
    ], check=True)

    count = 0

    def run(args, ok, mode='', expect_no_open=False):
        global count
        env = {**os.environ, 'E3_MOCK': mode}
        if expect_no_open:
            env['E3_EXPECT_NO_OPEN'] = '1'
            env['E3_EXPECT_NO_IOCTL'] = '1'

        p = subprocess.run([str(exe), *args], env=env,
                           capture_output=True, text=True, timeout=3)
        if not p.stdout.strip():
            raise AssertionError((args, p.returncode, p.stdout, p.stderr))

        d = json.loads(p.stdout)
        assert (p.returncode == 0) == ok, (args, p.stdout, p.stderr)
        assert d['schema'] == 'idma.e3_controlled.v1'
        assert d['success'] == ok
        assert len(d['rounds']) == 2
        assert all(len(r['e3_words']) == 256 for r in d['rounds'])
        assert all(len(r['controlled']['launcher']) == 32 for r in d['rounds'])
        assert d['scope']['hpm'] == 'GO_to_both_drained'
        assert d['scope']['observer'] == 'V_window'
        count += 1
        return d

    p = subprocess.run([str(exe), '--help'], capture_output=True, text=True)
    assert p.returncode == 0 and 'Usage:' in p.stdout

    # Positive all modes/delays.
    for aggressor in ('none', '1'):
        for prep in ('cold', 'hot'):
            if aggressor == 'none' and prep != 'cold':
                continue
            for pair in ('complementary', 'zero'):
                for delay in ('0', '128'):
                    if pair == 'zero' and delay != '0':
                        continue
                    args = ['--aggressor', aggressor, '--prep', prep,
                            '--pair', pair, '--first-delay', delay]
                    d = run(args, True)
                    expected = [int(delay),
                                0 if pair == 'zero' else 128 - int(delay)]
                    assert [r['delay'] for r in d['rounds']] == expected, \
                        (args, d['rounds'])
                    assert all(
                        r['controlled']['observer_magic'] == 0x49444d414f425306
                        for r in d['rounds']
                    )
                    assert all(
                        r['e3_words'][0] == 0x49444d4145330002
                        for r in d['rounds']
                    )
                    # No-trigger coverage is retained as executed data, never required.
                    assert all(r['e3_words'][2] == 0 for r in d['rounds'])
                    # Reserved DIDs are read from the result, not the JSON inputs.
                    assert all(r['victim_did'] == 10 for r in d['rounds'])
                    expected_did = 0xffffffff if aggressor == 'none' else 11
                    assert all(r['aggressor_did'] == expected_did
                               for r in d['rounds']), args

    run([], True, 'nocoverage')

    # Negative input must not open or issue ioctls.  Rounds in this path are
    # rendered from a zeroed struct (delay pair [0, 128]); that is acceptable
    # because errno is set, run_attempted is false, and no run happened.
    negative = [
        ['--bad'],
        ['--prep'],
        ['--prep', 'cold', '--prep', 'hot'],
        ['--first-delay', '-1'],
        ['--first-delay', '1'],
        ['--first-delay', '129'],
        ['--timeout-ms', '0'],
        ['--timeout-ms', '60001'],
        ['--timeout-ms', '-1'],
        ['--timeout-ms', ' 1'],
        ['--timeout-ms', '1 '],
        ['--timeout-ms', '1x'],
        ['--timeout-ms', '9999999999999999999'],
        ['--timeout-ms', '0x10'],
        ['--aggressor', '0'],
        ['--aggressor', 'none', '--prep', 'hot'],
        ['--pair', 'zero', '--first-delay', '128'],
        ['--pair', 'bad'],
        ['--prep', 'warm'],
        ['--help', 'x'],
    ]
    for args in negative:
        d = run(args, False, expect_no_open=True)
        assert d['run_attempted'] is False, args
        assert d['stage'] == 'arguments', args

    # Old/wrong capabilities return without run.
    for mode in ('old', 'wrongcap', 'wrongdid', 'wrongpage',
                 'wrongdevice', 'wrongabi', 'wrongsize'):
        d = run([], False, mode)
        assert d['run_attempted'] is False, mode
        assert d['errno'] in (95, 22, 5), (mode, d['errno'])

    # Open failure is reported at the open stage with no ioctl.
    d = run([], False, 'openfail')
    assert d['run_attempted'] is False
    assert d['stage'] == 'open'
    assert d['errno'] == 5  # EIO

    # EINTR is not retried.
    d = run([], False, 'interrupt')
    assert d['run_attempted'] is True
    assert d['errno'] == 4  # EINTR

    # Partial failed round raw data is preserved.
    d = run([], False, 'partial')
    assert d['run_attempted'] is True
    assert d['rounds_attempted'] == 2 and d['rounds_completed'] == 1
    assert len(d['rounds'][1]['e3_words']) == 256
    assert len(d['rounds'][1]['controlled']['launcher']) == 32

    # Metadata failures produce EPROTO/EIO.
    metadata_modes = [
        'bad_magic', 'bad_bank', 'bad_error', 'bad_d', 'bad_stage',
        'bad_prep', 'bad_flush', 'bad_vcycles', 'bad_prime', 'bad_probe',
        'bad_launcher_magic', 'bad_launcher_error', 'bad_launcher_frozen',
        'bad_launcher_aborted', 'bad_launcher_active', 'bad_launcher_id',
        'bad_launcher_id_mismatch', 'bad_launcher_mask',
        'bad_launcher_expected', 'bad_launcher_count', 'bad_launcher_cancel',
        'bad_round_status', 'bad_measurement', 'bad_hpm', 'bad_victim',
        'bad_aggressor', 'bad_sequence', 'bad_pair_status',
        # Stricter launcher word checks: each mutant passed the old lax test.
        'bad_launcher_status', 'bad_launcher_seen', 'bad_launcher_held',
        'bad_launcher_zero', 'bad_launcher_cancel_high', 'bad_launcher_vcount',
        'bad_launcher_acount', 'bad_launcher_has_a', 'bad_launcher_select_iova',
        'bad_victim_did', 'bad_aggressor_did',
    ]
    for mode in metadata_modes:
        d = run([], False, mode)
        assert d['run_attempted'] is True, mode
        assert d['errno'] in (71, 5), (mode, d['errno'])

    # Solo requires every A id / select word to read back zero.
    solo_modes = [
        'bad_launcher_a_id_solo', 'bad_launcher_select_iova_solo',
    ]
    for mode in solo_modes:
        d = run(['--aggressor', 'none'], False, mode)
        assert d['run_attempted'] is True, mode
        assert d['errno'] in (71, 5), (mode, d['errno'])

    # Close failure is reported after a successful run.
    d = run([], False, 'closefail')
    assert d['run_attempted'] is True
    assert d['stage'] == 'close'
    assert d['errno'] == 5  # EIO

    # A failed stdout write must exit nonzero with a stderr diagnostic and must
    # not retry the run ioctl (host-only /dev/full proof).
    full = Path('/dev/full')
    if full.exists():
        with open(full, 'wb') as sink:
            p = subprocess.run([str(exe)], stdout=sink,
                               stderr=subprocess.PIPE, timeout=3)
        assert p.returncode != 0, (p.returncode, p.stderr)
        assert b'idma_e3_controlled' in p.stderr, p.stderr
        count += 1

    print(f'PASS E3 controlled CLI {count} cases (host ioctl model only)')
