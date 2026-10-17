#!/usr/bin/env python3
"""Verify retained runtime/source identities and lossless event restoration."""
import gzip
import hashlib
import json
from pathlib import Path

TASK = Path(__file__).resolve().parents[1]


def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def main():
    attempts = []
    for receipt_path in sorted((TASK / 'runs').glob('*/attempt*/receipt.json')):
        receipt = json.loads(receipt_path.read_text())
        attempt = receipt_path.parent
        files = {name: sha(attempt / name) == expected
                 for name, expected in receipt['artifact_sha256'].items()}
        h = hashlib.sha256()
        restored_bytes = 0
        with gzip.open(attempt / 'events.csv.gz', 'rb') as stream:
            for block in iter(lambda: stream.read(1024 * 1024), b''):
                h.update(block)
                restored_bytes += len(block)
        expected = receipt['compression_verification']
        restore_exact = h.hexdigest() == expected['raw_sha256'] and restored_bytes == expected['raw_bytes']
        attempts.append({'attempt': str(attempt.relative_to(TASK)), 'artifacts_match_receipt': files,
                         'all_artifacts_match': all(files.values()), 'raw_bytes': expected['raw_bytes'],
                         'gzip_bytes': (attempt / 'events.csv.gz').stat().st_size,
                         'raw_sha256': expected['raw_sha256'], 'gzip_sha256': sha(attempt / 'events.csv.gz'),
                         'restored_sha256': h.hexdigest(), 'restored_bytes': restored_bytes,
                         'gzip_byte_exact_restoration': restore_exact})
    builds = []
    for path in sorted((TASK / 'builds').glob('v*-build*/build-manifest.json')):
        manifest = json.loads(path.read_text())
        inputs = {s['logical_path']: sha(Path(s['path'])) == s['sha256'] for s in manifest['sources']}
        builds.append({'manifest': str(path.relative_to(TASK)), 'manifest_sha256': sha(path),
                       'variant': manifest['variant'], 'state': manifest['state'],
                       'source_count': len(inputs), 'all_source_inputs_unchanged': all(inputs.values()),
                       'source_differences_from_original': manifest['source_differences'],
                       'cpp_unchanged': sha(Path(manifest['cpp']['path'])) == manifest['cpp']['sha256'],
                       'executable_matches': sha(Path(manifest['executable']['path'])) == manifest['executable']['sha256']})
    output = {'schema': 'plru-artifact-verification-v1', 'builds': builds, 'attempts': attempts,
              'all_passed': bool(attempts) and all(x['all_artifacts_match'] and x['gzip_byte_exact_restoration'] for x in attempts)
                            and all(x['all_source_inputs_unchanged'] and x['cpp_unchanged'] and x['executable_matches'] for x in builds),
              'implementation_sha256': {str(p.relative_to(TASK)): sha(p) for folder in ('runner', 'analysis')
                                        for p in sorted((TASK / folder).glob('*.py'))},
              'baseline_source_identity': 'runner/source-baseline-check.json',
              'baseline_behavior_identity': 'analysis/summary.json cases.*.baseline_reproduction',
              'replay_note': 'events.csv.gz restores the retained local events.csv byte for byte. The reducer accepts .csv.gz directly when .csv is absent.'}
    path = TASK / 'analysis/artifact-verification.json'
    path.write_text(json.dumps(output, indent=2, ensure_ascii=False) + '\n')
    print(json.dumps({'attempts': len(attempts), 'builds': len(builds), 'all_passed': output['all_passed']}))
    if not output['all_passed']:
        raise SystemExit(1)


if __name__ == '__main__':
    main()
