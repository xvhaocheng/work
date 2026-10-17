#!/usr/bin/env python3
"""Package successful DS25/DS100 builds without rehashing existing artifacts."""
import json
import os
from pathlib import Path
import shutil

PACKAGE=Path(__file__).resolve().parent.parent
BOARD=PACKAGE.parent.parent
WORK=BOARD.parents[2]

def load(p): return json.loads(p.read_text())
def save(p,v): p.write_text(json.dumps(v,indent=2)+'\n')

def main():
    previous=load(BOARD/'packages/baseline100-20261017/manifest.json')
    manifest={'experiment':'E1b DS25/DS100 added-delay sweep','status':'builds_passed_pending_collection',
              'source_record':'source.json','kernel_release':previous['kernel_release'],
              'artifacts':{},'builds':{},'results':{'status':'not_started','planned_measurements':100,
              'schedule':'DS25 50 then DS100 50','per_ds_condition_repetitions':10,'automatic_retries':0}}
    for key in ('sd_payload','linux_Image'):
        manifest['artifacts'][key]=previous['artifacts'][key]
    # Refuse incomplete or unsuccessful builds before copying any new BIT.
    builds={}
    for cycles in (25,100):
        out=WORK/f'iommu-platform/out/ispass2027-20261017-ds{cycles}-01'
        launch=BOARD/f'runs/ds{cycles}-build-launch01'
        result=load(launch/'result.json')
        build=load(out/'build-manifest.json')
        assert result['returncode']==0 and build['status']=='passed' and build.get('end')
        t=build['timing']; boundary=build['routed_boundary']
        assert t['wns_ns']>=0 and all(t[k]==0 for k in ('setup_failing','hold_failing','pulse_failing'))
        assert build['routing_errors']==0 and boundary['passed'] and boundary['cpu_period_ns']==20
        assert boundary['delay_cell_count']>0 and boundary['forbidden_research_cell_count']==0
        assert build['parameters']['ds_cycles']==[cycles,cycles] and build['parameters']['clock_hz']==50000000
        builds[cycles]=(out,launch,build)
    for cycles,(out,launch,build) in builds.items():
        dest=PACKAGE/f'metadata/ds{cycles}'
        dest.mkdir(exist_ok=False)
        for name in ('build-manifest.json','routed-boundary.json','build.log'):
            shutil.copyfile(out/name,dest/name)
        for name in ('command.json','dispatch.json','result.json','child.json'):
            shutil.copyfile(launch/name,dest/('launch-'+name))
        result=out/'src/cva6/corev_apu/fpga/work-fpga'
        for name in ('ariane_xilinx_timing_summary_routed.rpt','ariane_xilinx_route_status.rpt','ariane_xilinx_utilization_placed.rpt'):
            shutil.copyfile(result/name,dest/name)
        bit=result/'ariane_xilinx.bit'
        os.link(bit,PACKAGE/f'artifacts/ds{cycles}.bit')
        manifest['artifacts'][f'ds{cycles}_bit']={'file':f'artifacts/ds{cycles}.bit',
            'original_file':str(bit.relative_to(WORK)),'sha256':build['artifacts'][str(bit)],
            'hash_source':'existing completed build manifest; no repeat hash','bytes':bit.stat().st_size}
        manifest['builds'][f'ds{cycles}']={'manifest':f'metadata/ds{cycles}/build-manifest.json',
            'launch_result':f'metadata/ds{cycles}/launch-result.json','clock_hz':50000000,
            'timing':build['timing'],'routing_errors':build['routing_errors'],
            'delayer_cells':build['routed_boundary']['delay_cell_count']}
    save(PACKAGE/'manifest.json',manifest)
    print(json.dumps({'status':manifest['status'],'builds':manifest['builds']},indent=2))

if __name__=='__main__': main()
