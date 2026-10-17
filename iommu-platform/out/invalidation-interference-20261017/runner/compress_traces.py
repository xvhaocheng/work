#!/usr/bin/env python3
"""Publish lossless deterministic gzip traces without deleting local raw data."""
import gzip
import hashlib
import json
import shutil
from pathlib import Path

TASK = Path(__file__).resolve().parents[1]


def digest(stream):
    h = hashlib.sha256()
    for block in iter(lambda: stream.read(1024 * 1024), b''):
        h.update(block)
    return h.hexdigest()


def main():
    records = []
    for manifest in (TASK / 'runs').glob('*/campaign.json'):
        if not json.loads(manifest.read_text()).get('finished_utc'):
            raise RuntimeError('Campaign is still open: ' + str(manifest))
    for source in sorted((TASK / 'runs').rglob('events.*')):
        if source.name not in ('events.csv', 'events.jsonl'):
            continue
        destination = source.with_suffix(source.suffix + '.gz')
        if not destination.exists():
            temporary = destination.with_suffix(destination.suffix + '.tmp')
            with source.open('rb') as raw, temporary.open('xb') as out:
                with gzip.GzipFile(filename='', fileobj=out, mode='wb', mtime=0) as compressed:
                    shutil.copyfileobj(raw, compressed, 1024 * 1024)
            temporary.rename(destination)
        with source.open('rb') as raw, gzip.open(destination, 'rb') as restored:
            raw_hash, restored_hash = digest(raw), digest(restored)
        if raw_hash != restored_hash:
            raise RuntimeError('Compressed trace differs: ' + str(source))
        with destination.open('rb') as compressed:
            compressed_hash = digest(compressed)
        records.append({
            'original': source.relative_to(TASK).as_posix(),
            'published': destination.relative_to(TASK).as_posix(),
            'original_sha256': raw_hash,
            'compressed_sha256': compressed_hash,
            'original_bytes': source.stat().st_size,
            'compressed_bytes': destination.stat().st_size,
            'decompressed_sha256_verified': True,
        })
    if not records:
        raise RuntimeError('No actual traces to compress')
    (TASK / 'runs/compression.json').write_text(json.dumps({
        'scope': 'Lossless published trace copies; original local files retained',
        'records': records,
    }, indent=2) + '\n')
    print(json.dumps({'traces': len(records),
                      'original_bytes': sum(r['original_bytes'] for r in records),
                      'compressed_bytes': sum(r['compressed_bytes'] for r in records)}))


if __name__ == '__main__':
    main()
