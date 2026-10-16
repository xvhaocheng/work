#!/usr/bin/env python3
"""Freeze three generated functional Ethernet frames; no native arrival trace."""
from pathlib import Path
import json, hashlib
root=Path(__file__).resolve().parent
layout=[(60,[131],[60],[(7,8331,96)]),(65,[397],[65],[(11,8605,112)]),(1514,[791,3803],[503,1011],[(19,9031,515),(20,12055,1024)])]
packets=[]
for i,(length,src,lengths,desc) in enumerate(layout):
    frame=bytearray.fromhex('02000000000202000000000188b5')
    frame.extend(((j*37+i*53+17)&255) for j in range(length-14))
    frame[14:18]=(i+1).to_bytes(4,'big')
    packets.append(dict(packet_id=i+1,flow_id=1,order=i,cookie_id=desc[0][0],length=length,frame_hex=frame.hex(),source=[dict(offset=o,length=n) for o,n in zip(src,lengths)],destination=[dict(id=d,offset=o,capacity=c,next=desc[k+1][0] if k+1<len(desc) else None,writable=True) for k,(d,o,c) in enumerate(desc)],used_length=12+length))
fixture=dict(schema='c117r3-generated-functional-v1',provenance='generated functional fixture; not captured DPDK traffic, natural arrivals, or performance-gap evidence',memory_bytes=16384,source_region=[0,8192],destination_region=[8192,16384],header_bytes=12,header_hex='000000000000000000000100',frame_includes='Ethernet header and padding, excludes FCS/preamble/virtio header',ether_type='0x88b5 local experimental Ethernet payload',initial_source_fill=90,initial_destination_fill=165,packet_count=3,frame_bytes=sum(p['length'] for p in packets),destination_record_bytes=sum(p['used_length'] for p in packets),source_semantics='A already owns complete frames; B has preposted writable descriptor chains; no guest isolation claim',mapping_options='logical offsets; CPU uses process memory; RTL may translate one common IOVA base to a nonidentity PA base; no forced small pages or invalidation between requests',packets=packets)
raw=(json.dumps(fixture,indent=2)+'\n').encode();(root/'fixture.json').write_bytes(raw)
sha=hashlib.sha256(raw).hexdigest();(root/'fixture.sha256').write_text(sha+'  fixture.json\n')
lines=['// Generated from fixture.json; do not hand edit.','#pragma once','#include <array>','#include <cstdint>','#include <vector>','namespace c117 {',f'inline constexpr const char* kFixtureSha256="{sha}";','inline constexpr uint32_t kMemoryBytes=16384, kSourceEnd=8192, kHeaderBytes=12;','struct Region { uint32_t offset, length; };','struct Descriptor { uint32_t id, offset, capacity, next; bool has_next; };','struct Packet { uint32_t packet_id, flow_id, order, cookie_id, length; std::vector<uint8_t> frame; std::vector<Region> source; std::vector<Descriptor> destination; };','inline const std::vector<Packet>& packets() {','static const std::vector<Packet> p = {']
for p in packets:
    frame=','.join('0x'+p['frame_hex'][j:j+2] for j in range(0,len(p['frame_hex']),2))
    src=','.join('{%d,%d}'%(r['offset'],r['length']) for r in p['source'])
    dst=','.join('{%d,%d,%d,%d,%s}'%(d['id'],d['offset'],d['capacity'],d['next'] or 0,'true' if d['next'] is not None else 'false') for d in p['destination'])
    lines.append('{%d,%d,%d,%d,%d,{%s},{%s},{%s}},'%(p['packet_id'],p['flow_id'],p['order'],p['cookie_id'],p['length'],frame,src,dst))
lines+=['}; return p; }','} // namespace c117','']
(root/'fixture_data.hpp').write_text('\n'.join(lines))
print(sha)
