#!/usr/bin/env python3
# scripts/mutate_e3_controlled_review.py
"""Narrow mutation checks for out/e3-controlled-review-20260918/server_review_v6.py.

Every mutation is applied to a private temporary copy of the run folder; the
real evidence is opened read-only and hashed before/after.  Each mutation must
make the reviewer exit non-zero with at least one structural error; the
unmutated control must exit 0 with zero structural errors.

Exit codes: 0 = all cases behaved as required, 1 = a case did not, 2 = usage /
I-O problem, 3 = the real inputs changed while the script was running.
"""
import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
DEFAULT_REVIEWER = os.path.join(HERE, 'server_review_v6.py')
CLI_JSON_MARK = re.compile(r'\{\s*"schema"\s*:\s*"idma\.e3_controlled\.v1"')
UINT64_MASK = (1 << 64) - 1


def sha256(path):
    digest = hashlib.sha256()
    with open(path, 'rb') as handle:
        for chunk in iter(lambda: handle.read(1 << 20), b''):
            digest.update(chunk)
    return digest.hexdigest()


def read_text(path):
    with open(path, 'rb') as handle:
        return handle.read().decode('utf-8', 'surrogateescape')


def write_text(path, text):
    with open(path, 'wb') as handle:
        handle.write(text.encode('utf-8', 'surrogateescape'))


def store_pairs(path, pairs):
    with open(path, 'w', encoding='utf-8') as handle:
        json.dump(pairs, handle, separators=(',', ':'))
        handle.write('\n')


def load_pairs(path):
    with open(path, 'r', encoding='utf-8') as handle:
        return json.load(handle)


def apply_to_pair(pairs, sequence, mutate):
    for document in pairs:
        rounds = document.get('rounds') or []
        if rounds and rounds[0].get('sequence') == sequence:
            mutate(document)
            return document
    raise RuntimeError('pair document for sequence %d not found' % sequence)


def mirror_pair_into_console(text, sequence, document):
    decoder = json.JSONDecoder()
    for match in CLI_JSON_MARK.finditer(text):
        try:
            current, end = decoder.raw_decode(text, match.start())
        except ValueError:
            continue
        rounds = current.get('rounds') if isinstance(current, dict) else None
        if rounds and rounds[0].get('sequence') == sequence:
            return (text[:match.start()]
                    + json.dumps(document, separators=(',', ':'))
                    + text[end:])
    raise RuntimeError('console JSON document for sequence %d not found' % sequence)


def _bump_e3_word(index, delta):
    def mutate(document):
        words = document['rounds'][0]['e3_words']
        words[index] = (words[index] + delta) & UINT64_MASK
    return mutate


def _bump_launcher_word(index, delta):
    def mutate(document):
        words = document['rounds'][0]['controlled']['launcher']
        words[index] = (words[index] + delta) & UINT64_MASK
    return mutate


def _mirror(context, sequence, document):
    context['consoles'] = [mirror_pair_into_console(text, sequence, document)
                           for text in context['consoles']]


def _replace_all(context, old, new, minimum):
    replaced = 0
    texts = []
    for text in context['consoles']:
        count = text.count(old)
        replaced += count
        texts.append(text.replace(old, new))
    if replaced < minimum:
        raise RuntimeError('expected at least %d occurrences of %r, found %d'
                           % (minimum, old, replaced))
    context['consoles'] = texts


def case_control(context):
    pass


def case_e3_word_magic(context):
    document = apply_to_pair(context['pairs'], 1, _bump_e3_word(0, 1))
    _mirror(context, 1, document)


def case_e3_word_final_hit(context):
    document = apply_to_pair(context['pairs'], 1, _bump_e3_word(17, 1))
    _mirror(context, 1, document)


def case_launcher_word(context):
    document = apply_to_pair(context['pairs'], 1, _bump_launcher_word(0, 1))
    _mirror(context, 1, document)


def case_uart_time_field(context):
    _replace_all(context, 'cycles=99 ', 'cycles=100 ', 2)


def case_metadata_input(context):
    def mutate(document):
        document['input']['first_delay'] = 128
    document = apply_to_pair(context['pairs'], 1, mutate)
    _mirror(context, 1, document)


def case_missing_uart_row(context):
    pattern = re.compile(r'^.*idma_path_obs seq=7 role=a channel=w stage=3 .*\n',
                         re.MULTILINE)
    contexts = []
    removed = 0
    for text in context['consoles']:
        text, count = pattern.subn('', text, count=1)
        removed += count
        contexts.append(text)
    if removed < 1:
        raise RuntimeError('no seq=7 UART observation row found to delete')
    context['consoles'] = contexts


CASES = (
    ('control', case_control, False),
    ('e3_word_magic', case_e3_word_magic, True),
    ('e3_word_final_hit', case_e3_word_final_hit, True),
    ('launcher_word', case_launcher_word, True),
    ('uart_time_field', case_uart_time_field, True),
    ('metadata_input', case_metadata_input, True),
    ('missing_uart_row', case_missing_uart_row, True),
)


def input_hashes(run_folder, extra_paths):
    paths = []
    for root, _dirs, files in os.walk(run_folder):
        for name in sorted(files):
            paths.append(os.path.join(root, name))
    paths.extend(path for path in extra_paths if path and os.path.isfile(path))
    return {path: sha256(path) for path in sorted(paths)}


def run_case(name, mutate, expect_failure, args, console_name, readable_path):
    temp_root = tempfile.mkdtemp(prefix='e3-review-mutation-')
    try:
        staging = os.path.join(temp_root, 'run')
        shutil.copytree(args.run_folder, staging)
        console_paths = [os.path.join(staging, console_name)]
        if readable_path:
            shutil.copyfile(readable_path, os.path.join(staging, 'console.readable.log'))
            console_paths.append(os.path.join(staging, 'console.readable.log'))
        context = {
            'staging': staging,
            'pairs_path': os.path.join(staging, 'all-original-pairs.json'),
            'pairs': load_pairs(os.path.join(staging, 'all-original-pairs.json')),
            'console_paths': console_paths,
            'consoles': [read_text(path) for path in console_paths],
        }
        mutate(context)
        store_pairs(context['pairs_path'], context['pairs'])
        for path, text in zip(context['console_paths'], context['consoles']):
            write_text(path, text)

        out_json = os.path.join(temp_root, 'review.json')
        command = [sys.executable, args.reviewer, staging, out_json,
                   '--console', os.path.join(staging, console_name)]
        if readable_path:
            command.extend(['--readable-console',
                            os.path.join(staging, 'console.readable.log')])
        proc = subprocess.run(command, capture_output=True, text=True)
        report = {'case': name, 'exit_code': proc.returncode,
                  'stdout': proc.stdout.strip(), 'stderr': proc.stderr.strip()}
        if os.path.isfile(out_json):
            with open(out_json, 'r', encoding='utf-8') as handle:
                review = json.load(handle)
            errors = review.get('findings', {}).get('structural_errors') or []
            report['structural_errors'] = len(errors)
            report['first_error'] = errors[0] if errors else None
        else:
            report['structural_errors'] = None
            report['first_error'] = None
        if expect_failure:
            report['ok'] = bool(report['structural_errors']) and proc.returncode != 0
        else:
            report['ok'] = (proc.returncode == 0 and report['structural_errors'] == 0)
        return report
    finally:
        shutil.rmtree(temp_root, ignore_errors=True)


def main(argv=None):
    parser = argparse.ArgumentParser()
    parser.add_argument('--run-folder', required=True)
    parser.add_argument('--reviewer', default=DEFAULT_REVIEWER)
    parser.add_argument('--readable-console')
    parser.add_argument('--console-name', default='console.raw.log')
    args = parser.parse_args(argv)

    if not os.path.isdir(args.run_folder):
        print('error: run folder not found:', args.run_folder)
        return 2
    if not os.path.isfile(args.reviewer):
        print('error: reviewer not found:', args.reviewer)
        return 2

    before = input_hashes(args.run_folder, [args.readable_console])
    reports = []
    for name, mutate, expect_failure in CASES:
        try:
            report = run_case(name, mutate, expect_failure, args,
                              args.console_name, args.readable_console)
        except Exception as exc:  # noqa: BLE001 - report, never hide
            report = {'case': name, 'ok': False, 'error': repr(exc)}
        reports.append(report)
        print(json.dumps(report, sort_keys=True))
    after = input_hashes(args.run_folder, [args.readable_console])
    if before != after:
        print('error: real inputs changed during the mutation run')
        return 3
    return 0 if all(report.get('ok') for report in reports) else 1


if __name__ == '__main__':
    sys.exit(main())
