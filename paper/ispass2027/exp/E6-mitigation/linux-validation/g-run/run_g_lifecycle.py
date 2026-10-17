#!/usr/bin/env python3
"""Run actual G/observer lifecycle functions against a bounded declared host model."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

HERE=Path(__file__).resolve().parent
COMMIT='69d8b9d4549c473477bcdc1df7c09ee1bb55528c'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--linux',required=True,type=Path)
    p.add_argument('--out',required=True,type=Path)
    a=p.parse_args();source=a.linux.resolve();out=a.out.resolve()
    assert out.is_relative_to((HERE/'runs').resolve()) and not out.exists()
    assert subprocess.check_output(['git','-C',str(source),'rev-parse','HEAD'],text=True).strip()==COMMIT
    assert subprocess.check_output(['git','-C',str(source),'diff','--binary',COMMIT])==b''
    out.mkdir(parents=True);build=out/'build';(build/'uapi/linux').mkdir(parents=True)
    for name in ['idma.h','idma_p0.h','idma_multi.h','idma_pages.h','idma_workload.h','idma_g.h']:
        shutil.copyfile(source/'include/uapi/linux'/name,build/'uapi/linux'/name)
    for name in ['idma-workload.h','idma-g.h']:
        shutil.copyfile(source/'drivers/misc'/name,build/name)
    (build/'shim/linux').mkdir(parents=True);(build/'shim/linux/string.h').write_text('#include <string.h>\n')
    text=(source/'drivers/misc/idma.c').read_text();(out/'idma.as-tested.c').write_text(text)
    # Preserve definition order and each complete production body verbatim.
    functions=re.findall(r'^static [^\n]*\b(idma_[a-z_]+)\([^;{}]*\)\s*\{.*?^\}',text,re.M|re.S)
    # Restrict to the already-used cohesive driver/observer interfaces; ioctl and
    # platform probe/remove require unrelated kernel types and are not exercised.
    wanted=['idma_buffer_page','idma_pattern','idma_stream_cleanup','idma_stream_allocate','idma_stream_prepare',
            'idma_quarantine','idma_submit','idma_poll','idma_run_window_many','idma_run_window','idma_drain','idma_verify',
            'idma_batch_supported','idma_batch_configure','idma_batch_stop','idma_batch_collect','idma_batch_disable',
            'idma_batch_window_many','idma_batch_window','idma_g_warmup','idma_g_batch_window','idma_g_wait_a',
            'idma_g_check_lanes','idma_multi_stop','idma_multi_drain_collect','idma_snapshot','idma_snapshot_delta',
            'idma_validate','idma_any_quarantined','idma_observer_has_path','idma_g_read_gap_cap','idma_g_probe_locked',
            'idma_observation_prepare','idma_observation_arm','idma_g_fail','idma_observation_hist_finish',
            'idma_observation_finish','idma_run_experiment_mapped','idma_run_experiment_core','idma_run_experiment_mode',
            'idma_validate_p0','idma_run_p0','idma_caps_p0','idma_pages_verify_buffer','idma_pages_contiguous',
            'idma_pages_map_buffer','idma_pages_prepare','idma_pages_cleanup','idma_pages_validate','idma_run_pages',
            'idma_caps_pages','idma_multi_validate','idma_multi_snapshot','idma_run_multi','idma_caps_multi',
            'idma_workload_validate','idma_run_workload','idma_caps_workload','idma_run_g']
    bodies=[]
    for name in wanted:
        match=re.findall(rf'^static [^\n]*\b{name}\([^;{{}}]*\)\s*\{{.*?^\}}',text,re.M|re.S)
        assert len(match)==1,(name,len(match));bodies.append((text.index(match[0]),match[0]))
    types='\n'.join(re.findall(r'^#define IDMA_.*$',text,re.M))+'\n'
    for name in ['idma_device','idma_stream','idma_observation','idma_multi_context','idma_pages_context',
                 'idma_workload_stream','idma_workload_context','idma_g_context']:
        types+=re.search(rf'^struct {name} \{{.*?^\}};',text,re.M|re.S)[0]+'\n'
    types+=re.search(r'^struct riscv_iommu_leaf_info \{.*?^\};',
                    (source/'include/linux/riscv_iommu_leaf.h').read_text(),re.M|re.S)[0]+'\n'
    for name in ['g_state.h','g_model.inc','g_cases.inc']:
        shutil.copyfile(HERE/name,build/name)
    harness=(HERE/'g_run_host.c').read_text().replace('/* TYPES */',types)
    harness=harness.replace('/* FUNCTIONS */','\n\n'.join(body for _,body in sorted(bodies)))
    (build/'actual-g-lifecycle.c').write_text(harness)
    command=['cc','-std=gnu11','-O1','-g','-Wall','-Wextra','-Werror','-Wno-unused-function',
             '-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie',
             '-I',str(build/'shim'),'-I',str(build),'-I',str(build/'uapi'),
             str(build/'actual-g-lifecycle.c'),'-o',str(build/'fixture')]
    m={'schema':'ispass2027-e6-g-lifecycle-fixture-v1','start':time.time(),'status':'running',
       'linux_commit':COMMIT,'fixture_only':True,'observation_words':'synthetic declared fixtures; not RTL events',
       'data_copies':'actual host source/destination ring copies and actual driver verification',
       'driver_sha256':sha(source/'drivers/misc/idma.c'),'functions':wanted,'command':command,
       'inputs':{name:sha(HERE/name) for name in ['g_run_host.c','g_state.h','g_model.inc','g_cases.inc','run_g_lifecycle.py']}}
    receipt=out/'manifest.json'
    try:
        receipt.write_text(json.dumps(m,indent=2)+'\n')
        with (out/'compile.log').open('w') as log:
            subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,timeout=120,check=True)
        with (out/'fixture.log').open('w') as log:
            subprocess.run([str(build/'fixture')],stdout=log,stderr=subprocess.STDOUT,timeout=60,check=True)
        m['status']='passed';print((out/'fixture.log').read_text().strip())
    except Exception as e:
        m.update(status='failed',error=repr(e));raise
    finally:
        m['end']=time.time();receipt.write_text(json.dumps(m,indent=2)+'\n')


if __name__=='__main__':main()
