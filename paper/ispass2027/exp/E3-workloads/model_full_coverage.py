#!/usr/bin/env python3
"""Planned address-set accounting, never a DMA or board measurement."""
import argparse
from collections import Counter
import json
from pathlib import Path

SIZES=(64,256,1024,4096,16384,65536)


def geometry(shape,size):
    if shape in ('p1_minimal','nic'):
        pages=max(1 if shape=='p1_minimal' else 8,(size+4095)//4096)
        return dict(shape=shape,geometry='page_packed' if shape=='nic' and size<=4096 else 'linear',
                    row_bytes=size,rows_per_operation=1,mapped_pages=pages,pitch_bytes=0,matrix_rows=0,columns=0)
    if shape=='storage':
        return dict(shape=shape,geometry='linear',row_bytes=size,rows_per_operation=1,mapped_pages=256,
                    pitch_bytes=0,matrix_rows=0,columns=0)
    row=min(size,1024)
    return dict(shape=shape,geometry='tile_rows',row_bytes=row,rows_per_operation=size//row,mapped_pages=256,
                pitch_bytes=16384,matrix_rows=64,columns=16384//row)


def period(g):
    if g['geometry']=='linear':return g['mapped_pages']*4096//g['row_bytes']
    if g['geometry']=='page_packed':return g['mapped_pages']*(4096//g['row_bytes'])
    return g['matrix_rows']*g['columns']


def offset(g,k):
    k%=period(g)
    if g['geometry']=='linear':return k*g['row_bytes']
    if g['geometry']=='page_packed':
        slots=4096//g['row_bytes'];return (k//slots)*4096+(k%slots)*g['row_bytes']
    op,row=divmod(k,g['rows_per_operation']);group,column=divmod(op,g['columns'])
    return (group*g['rows_per_operation']+row)*g['pitch_bytes']+column*g['row_bytes']


def prefix(g,descriptors):
    n=period(g);whole,tail=divmod(descriptors,n)
    pages=Counter();blocks=set();bursts=0;trace=[]
    # Repeated cycles are counted algebraically, avoiding enumeration of billions of A descriptors.
    for k in range(min(n,descriptors)):
        start=offset(g,k);end=start+g['row_bytes']
        assert start%64==0 and g['row_bytes']%64==0 and end<=g['mapped_pages']*4096
        times=whole+(k<tail) if descriptors>=n else 1
        pages.update({p:times for p in range(start//4096,(end-1)//4096+1)})
        blocks.update(range(start//64,end//64))
        bursts+=times*((start%2048+g['row_bytes']+2047)//2048)
        if k<4:trace.append({'descriptor':k,'offset':start,'length':g['row_bytes']})
    return {'completed_descriptors':descriptors,'completed_payload_bytes':descriptors*g['row_bytes'],
            'distinct_accessed_bytes':len(blocks)*64,'distinct_accessed_pages':len(pages),
            'page_set':sorted(pages),'descriptor_touches_per_page':dict(sorted(pages.items())),
            'full_rings':whole,'tail_descriptors':tail,'ring_completed':whole>0,
            'predicted_AXI_bursts_source_model':bursts,'first_descriptors':trace,
            'last_descriptor':{'descriptor':descriptors-1,'offset':offset(g,descriptors-1),'length':g['row_bytes']} if descriptors else None}


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',required=True,type=Path);a=p.parse_args()
    rows=[]
    for shape in ('p1_minimal','nic','storage','tile'):
        for size in SIZES+((1536,) if shape=='nic' else ()):
            g=geometry(shape,size);per_op=g['rows_per_operation'];ring_ops=period(g)//per_op
            protocols=[('fixed4096_256',4096,256)]
            if size in SIZES:
                protocols.append(('equal16MiB_one_V_ring_warmup',16*1048576//size,ring_ops))
            for protocol,ops,warm in protocols:
                measured=prefix(g,ops*per_op);warmup=prefix(g,warm*per_op)
                rows.append({'evidence_level':'planned_address_model','new_board_measurements':0,
                             'selected_research_protocol':protocol=='equal16MiB_one_V_ring_warmup',
                             'protocol':protocol,'logical_operation_bytes':size,'geometry':g,
                             'planned_V_operations':ops,'planned_V_warmup_operations':warm,
                             'ring_descriptor_period':period(g),'ring_operation_period':ring_ops,
                             'planned_V_measurement':measured,'planned_V_warmup':warmup,
                             'A_policy':'continuous batch until V completes then STOP/drain; actual A warmup/measured descriptors and footprint must come from returned counters',
                             'observed_A_completed_descriptors':None})
    # Decisive accounting examples; these are model checks, not performance tests.
    storage64=next(x for x in rows if x['geometry']['shape']=='storage' and x['logical_operation_bytes']==64 and x['protocol']=='fixed4096_256')
    assert storage64['planned_V_measurement']['distinct_accessed_pages']==64
    assert not storage64['planned_V_measurement']['ring_completed']
    assert storage64['planned_V_warmup']['distinct_accessed_pages']==4
    nic1536=next(x for x in rows if x['logical_operation_bytes']==1536)
    assert nic1536['planned_V_measurement']['distinct_accessed_bytes']==8*2*1536
    assert nic1536['planned_V_measurement']['distinct_accessed_pages']==8
    assert all(x['planned_V_measurement']['completed_descriptors']<=262144 for x in rows)
    assert all(x['planned_V_warmup']['completed_descriptors']<=16384 for x in rows)
    value={'schema':'e3-full-planned-coverage-v1','evidence_level':'planned_address_model','new_board_measurements':0,
           'status':'accepted_design_not_implemented','clock_or_runtime_measured':False,
           'selected_research_protocol':'equal16MiB_one_V_ring_warmup','P0_protocol_unchanged':True,'rows':rows,
           'notes':['mapped footprint, actual accessed byte/page sets, and repeated touches are separate',
                    'source and destination use separate equal geometry and the same relative offsets',
                    'predicted AXI burst totals are address-model predictions, not backend proof',
                    'warmup A work is not presumed equal to V or to a complete ring']}
    with a.output.open('x') as stream:json.dump(value,stream,indent=2);stream.write('\n')
    print(json.dumps({'status':'design_only','configurations':len(rows),'output':str(a.output.resolve())}))


if __name__=='__main__':main()
