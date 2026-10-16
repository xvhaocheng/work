#!/usr/bin/env python3
"""Materialize the proposed native functional input, without opening any device."""
import hashlib
import json
import struct
from pathlib import Path

OUT = Path(__file__).resolve().parent
lengths = (60, 568, 1011)
frames = [struct.pack('<QI', p + 1, length) +
          bytes((17 * p + q) & 255 for q in range(length))
          for p, length in enumerate(lengths)]
blob = b''.join(frames)
(OUT / 'three-frames.bin').write_bytes(blob)
manifest = {
    'schema': 'a2-proposed-native-functional-task-v1',
    'status': 'input_materialized_adapter_not_implemented',
    'source_shape': 'stage117: three framed packets, four DMA spans',
    'source_layout': 'explicit_new_user_arena_not_old_physical_layout',
    'page_bytes': 4096, 'arena_pages': 8, 'arena_bytes': 32768,
    'packet_lengths': list(lengths), 'cookies': [1, 2, 3],
    'header': '<QI: little-endian cookie u64 and length u32',
    'payload_rule': '(17 * zero_based_packet + payload_offset) & 255',
    'payload_bytes': sum(lengths), 'record_bytes': len(blob),
    'expected_output': 'byte-identical framed stream after real consumer delivery',
    'input_sha256': hashlib.sha256(blob).hexdigest(),
    'actual_output_sha256': None,
    'descriptors': [
        {'name': 'd0', 'lane': 0, 'did': 10, 'packet': 0,
         'src_offset': 32, 'dst_offset': 4 * 4096 + 12, 'bytes': 60},
        {'name': 'd1', 'lane': 1, 'did': 11, 'packet': 1,
         'src_offset': 4096 + 32, 'dst_offset': 5 * 4096 + 12, 'bytes': 65},
        {'name': 'd2', 'lane': 1, 'did': 11, 'packet': 1,
         'src_offset': 2 * 4096 + 32, 'dst_offset': 5 * 4096 + 77, 'bytes': 503},
        {'name': 'd3', 'lane': 0, 'did': 10, 'packet': 2,
         'src_offset': 3 * 4096 + 32, 'dst_offset': 6 * 4096 + 12, 'bytes': 1011}
    ],
    'dependencies': ['d0 DONE before d3 publish', 'd1 DONE before d2 publish',
                     'd0 before packet0 acquire', 'd1 and d2 before packet1 acquire',
                     'd3 before packet2 acquire', 'consumer order packet0,1,2',
                     'each consumer ACK before slot return',
                     'DMA+CPU+CQ safe drain before unpin/context release'],
    'publication': {'mode': 'normal', 'CONF': 0, 'pv': 0,
                    'per_lane_normal_inflight': 1, 'software_pending': 0,
                    'extra_payload_permits': 0, 'translation_feedback': False},
    'address_modes': {'primary': 'C2 single-mm borrowed Sv39 root, not implemented',
                      'ordinary': 'C1s same-UVA independent shadow, not implemented'},
    'first_functional_policy': 'public-DONE global serial d0,d1,d2,d3',
    'controls': ['ordinary two-lane refill via public DONE after separate progress gate',
                 'ordinary public-DONE global serial',
                 'current-use ordering if dependencies permit',
                 'CPU direct copy and direct writev of original source spans',
                 'DS-first/F4 only with separately bound equal-cost hardware interface'],
    'time': {'ready': 'before input read, allocation, registration and supply',
             'finish': 'delivered consumer bytes plus all release and final quiet',
             'values': None, 'backend_implemented': False, 'runs': 0},
    'scope': 'functional integration input; not a performance positive or an executed SVA task'
}
(OUT / 'native-task-input.json').write_text(
    json.dumps(manifest, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({'artifact': 'three-frames.bin', 'bytes': len(blob),
                  'sha256': manifest['input_sha256'], 'device_opened': False,
                  'native_runs': 0}, ensure_ascii=False))
