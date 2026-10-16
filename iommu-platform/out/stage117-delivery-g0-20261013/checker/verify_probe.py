#!/usr/bin/env python3
"""Only the new bounded independent-port/tag/stitching probe. No RTL calls."""
import collections,json
from pathlib import Path
from verify_delivery import *

def verify_probe(directory):
    require(not (directory/'failure.json').exists(),'failed probe cannot be accepted')
    raw,ops,ports=check_cpu_handshake(directory);rows,by=load_events(directory/'events.csv');cache=check_axi(directory/'cache_axi.csv');memory=check_axi(directory/'memory_axi.csv');control,fences=check_control(directory,by,cache);bind_cache_ddr(cache,memory);check_no_cpu_in_halt(ops,control)
    src,dst,n=4093,12291,43;initial=bytearray([0xa5]*16384)
    for i in range(n):initial[src+i]=(i*37+19)&255
    expected=initial.copy();expected[dst:dst+n]=initial[src:src+n]
    pmap={1:{'source_bytes':list(range(src,src+n)),'payload_dest':list(range(dst,dst+n))}}
    copy=check_cpu_copy('cpu_probe',ops,pmap,[dict(id=0,packet=1,src=src,dst=dst,bytes=n)],initial,by)
    init={a:v for o in ops if o['phase']=='probe_init' for a,v in selected_bytes(o,'wdata')}
    require(set(init)==set(range(PA,PA+16384)) and bytes(init[a] for a in range(PA,PA+16384))==initial,'new probe original image actual CPU initialized')
    backing=collections.defaultdict(int);ri=iter(memory['reads']);wi=iter(memory['writes'])
    for row in memory['accepted']:
        if row['channel']=='W':
            b=next(wi)
            for l in range(8):
                if b['strb']&(1<<l):backing[b['address']+l]=(b['data']>>(8*l))&255
        elif row['channel']=='R':
            b=next(ri);require(b['data']==int.from_bytes(bytes(backing[b['address']+i] for i in range(8)),'little'),'new probe shared DDR R actual backing mismatch')
    require(bytes(backing[PA+i] for i in range(16384))==expected,'new probe source/header/guards/destination backing mismatch')
    store= [o for o in ops if o['phase']=='probe_used'];require(len(store)==1 and store[0]['we'] and store[0]['address']==META+16 and store[0]['wdata']==n,'new probe actual metadata store')
    publish=sole(by,'probe_used_publish');metaB=[b['request']['retire_cycle'] for b in cache['writes'] if b['address']==META+16]
    require(metaB and max(metaB)<publish['cycle'],'new probe metadata WT B before publication')
    con=[o for o in ops if o['phase']=='probe_consumer'];actual=[(a-PA,v) for o in con for a,v in selected_bytes(o,'rdata')]
    require([a for a,v in actual]==list(range(dst,dst+n)) and bytes(v for a,v in actual)==initial[src:src+n],'new probe actual unaligned consumer response bytes')
    require((directory/'consumer.bin').read_bytes()==bytes(v for a,v in actual),'new probe savedconsumer differs actualCPUresponses')
    meta=[o for o in ops if o['phase']=='probe_meta'];require(len(meta)==1 and not meta[0]['we'] and meta[0]['address']==META+16 and meta[0]['rdata']==n,'new probe physical metadata consumer')
    source_retired=sole(by,'probe_source_retired');require(copy['actual_source_read_retirement']['1']<source_retired['cycle'],'new probe source claimed returned before final source response')
    require(ports['load_response_next_grant_cycles'] and ports['load_store_overlap_cycles'],'new probe actual native pipeline/independentport witnesses missing')
    crosses=[r['cycle'] for r in raw if r['port']==1 and r['phase']=='cpu_copy' and r['req'] and r['tag_valid'] and r['address']//4096!=r['tag_address']//4096]
    require(crosses,'new probe cross4K previoustag/newindex witness missing')
    release=sole(by,'probe_release_quiet');require(all(r['retire_cycle']<release['cycle'] for r in cache['requests']+memory['requests']) and all(o['response_cycle']<release['cycle'] for o in ops),'new probe release before actual R/B/CPU retirement')
    return dict(schema='independent-stage117-delivery-new-interface-probe-v1',passed=True,probe_source=src,probe_destination=dst,probe_bytes=n,actual_source_response_to_destination_store_dependencies=len(copy['source_response_to_destination_grant_dependencies']),actual_CPU_port_witnesses=ports,cross4K_previous_tag_new_index_cycles=crosses,actual_metadata_WT_B_retirement=max(metaB),metadata_publication_cycle=publish['cycle'],actual_controller_fences=fences,raw_sha256={p.name:sha(p) for p in directory.iterdir() if p.is_file()},old_microtests_executed=False,simulations_run_by_checker=0,executor_probe_pass_flag_used=False,limits=['Bounded new independent-port/tag/unaligned data dependency check; no sixcase G0 outcome.','No native CPU instructions or ALU cost measured.'])
if __name__=='__main__':
    d=HERE.parent/'implementation/results/interface_probe'
    try:r=verify_probe(d)
    except Exception as e:r=dict(passed=False,error_type=type(e).__name__,error=str(e))
    (HERE/'new-interface-probe-independent.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({k:r[k] for k in ('passed','error') if k in r}));raise SystemExit(int(not r['passed']))
