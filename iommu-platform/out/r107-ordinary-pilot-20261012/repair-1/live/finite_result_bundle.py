#!/usr/bin/env python3
"""Transport-only receipt archive validation; never accepts the DMA experiment.

Extract only the eight expected regular files into a new task directory. The
original gzip archive is retained. Semantic acceptance remains the existing
validate_finite_pilot.py check of all original UAPI fields and full pool bytes.
"""
import argparse
import base64
import gzip
import hashlib
import io
import json
from pathlib import Path
import tarfile

POOL_BYTES = 2359296
EXPECTED = {
    'pool0-cpu-home-worker0.bin': POOL_BYTES,
    'pool1-gpu-resident-worker0.bin': POOL_BYTES,
    'pool2-cpu-home-worker1.bin': POOL_BYTES,
    'pool3-gpu-resident-worker1.bin': POOL_BYTES,
    'uapi-run.bin': 4520,
    'uapi-batches.bin': 192 * 136,
    'uapi-leaves.bin': 4096 * 88,
    'receipt.json': None,
}
MAX_RECEIPT = 2097152
MAX_COMPRESSED = 16777216
MAX_WIRE = 25165824
MAX_UNCOMPRESSED = 13631488


def require(ok, reason):
    if not ok:
        raise ValueError(reason)


def check_encoded(encoded):
    require(0 < len(encoded) <= MAX_WIRE, 'wire payload outside finite bound')
    compressed = base64.b64decode(b''.join(encoded.split()), validate=True)
    require(0 < len(compressed) <= MAX_COMPRESSED, 'archive outside finite bound')
    if compressed.startswith(b'\x1f\x8b'):
        with gzip.GzipFile(fileobj=io.BytesIO(compressed)) as reader:
            raw_tar = reader.read(MAX_UNCOMPRESSED + 1)
    else:
        # The actual frozen BusyBox tar has no -z feature. Keep its original
        # uncompressed tar bytes and apply exactly the same finite member gate.
        raw_tar = compressed
    require(len(raw_tar) <= MAX_UNCOMPRESSED, 'uncompressed archive exceeds finite bound')
    files = {}
    directory_seen = False
    with tarfile.open(fileobj=io.BytesIO(raw_tar), mode='r:') as archive:
        for member in archive:
            name = member.name
            if member.isdir() and name in ('.', './') and not directory_seen:
                directory_seen = True
                continue
            if name.startswith('./'):
                name = name[2:]
            require(name in EXPECTED and name not in files, 'unexpected or duplicate archive member')
            require(member.isfile() and not member.sparse, 'nonregular/sparse archive member refused')
            expected = EXPECTED[name]
            require(member.size == expected if expected is not None else
                    0 < member.size <= MAX_RECEIPT, 'original raw buffer length is incomplete')
            source = archive.extractfile(member)
            require(source is not None, 'unreadable archive member')
            data = source.read(member.size + 1)
            require(len(data) == member.size, 'short original archive member')
            files[name] = data
    require(set(files) == set(EXPECTED), 'missing original receipt/raw pool/UAPI buffer')
    receipt = json.loads(files['receipt.json'])
    require(receipt.get('schema') == 'r107-ordinary-finite-pilot-receipt-v1' and
            receipt.get('kind') == 'run', 'foreign receipt schema')
    return compressed, files


def accept_transport(encoded, output):
    compressed, files = check_encoded(encoded)
    # No existing directory or original file may be overwritten, even if a
    # duplicate network request arrives after the first accepted transfer.
    output.mkdir(mode=0o700)
    archive_name = ('original-board-result.tar.gz' if compressed.startswith(b'\x1f\x8b')
                    else 'original-board-result.tar')
    with (output / archive_name).open('xb') as f:
        f.write(compressed)
    rows = []
    for name, data in files.items():
        with (output / name).open('xb') as f:
            f.write(data)
        rows.append({'file': name, 'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest()})
    record = {'transport_complete': True, 'hardware_semantic_success': 'unverified',
              'wire_bytes': len(encoded), 'compressed_bytes': len(compressed),
              'archive_sha256': hashlib.sha256(compressed).hexdigest(), 'members': rows,
              'original_archive_file': archive_name,
              'archive_encoding': 'gzip tar' if archive_name.endswith('.gz') else 'plain tar',
              'next_required_check': 'root run host/validate_finite_pilot.py against the actual receipt/data'}
    (output / 'transport-receipt.json').write_text(json.dumps(record, indent=2) + '\n')
    return record


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('base64_file', type=Path)
    p.add_argument('--output-dir', type=Path, required=True)
    a = p.parse_args()
    require(a.base64_file.stat().st_size <= MAX_WIRE, 'oversized encoded archive')
    r = accept_transport(a.base64_file.read_bytes(), a.output_dir)
    print(json.dumps(r, indent=2))


if __name__ == '__main__':
    main()
