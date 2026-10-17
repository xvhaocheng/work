#!/usr/bin/env python3
"""Prepare, then explicitly launch one detached bounded checkpoint continuation.
No board access. Original source/export, logs, checkpoint bytes and manifest stay intact.
"""
import argparse
import copy
import datetime
import hashlib
import json
import os
from pathlib import Path
import re
import signal
import subprocess
import sys
import zipfile

HERE=Path(__file__).resolve().parent
PARENT=HERE/'artifacts/full-e3-ds0-build01'
CHECKPOINT_SHA='0f1fe07fed706f9461efeb1b84294ef60bf20394d2392a3b36e20fc91d69340f'
PARENT_SHA='ebeeb7dee87b57591c01ef11ea656e2dc38c0cac8c1c0ab1b8c5e18e05b8d597'
VIVADO='/tools/Xilinx/Vivado/2023.1/bin/vivado'
MAX_SECONDS=21600


def now():return datetime.datetime.now(datetime.timezone.utc).isoformat()
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def save(path,value):path.write_text(json.dumps(value,indent=2)+'\n')
def require(ok,message):
    if not ok:raise ValueError(message)


def timing_gate(report,routing):
    rows=Path(report).read_text().splitlines();i=next(i for i,line in enumerate(rows) if 'WNS(ns)' in line);v=rows[i+2].split()
    timing={'wns_ns':float(v[0]),'tns_ns':float(v[1]),'setup_failing':int(v[2]),'whs_ns':float(v[4]),
            'hold_failing':int(v[6]),'wpws_ns':float(v[8]),'pulse_failing':int(v[10]),
            'report':str(report),'sha256':sha(report),'full_io_cdc_signoff':False}
    errors=re.search(r'# of nets with routing errors\.*\s*:\s*(\d+)',Path(routing).read_text(),re.I)
    require(errors is not None,'routing error count absent from route status report');count=int(errors[1])
    passed=timing['wns_ns']>=0 and timing['setup_failing']==timing['hold_failing']==timing['pulse_failing']==count==0
    return timing,count,passed


def prepare(output):
    require(not output.exists() and output.is_relative_to((HERE/'artifacts').resolve()),'new E3 artifacts output required')
    require(sha(PARENT/'build-manifest.json')==PARENT_SHA,'original interrupted manifest changed')
    old=json.loads((PARENT/'build-manifest.json').read_text());require(old['status']=='running','original manifest state differs')
    fpga=PARENT/'src/cva6/corev_apu/fpga';cp=fpga/'ariane.runs/impl_1/ariane_xilinx_physopt.dcp'
    require(sha(cp)==CHECKPOINT_SHA,'wrong original physopt checkpoint')
    with zipfile.ZipFile(cp) as archive:
        require('<Part Name="xc7k325tffg900-2"/>' in archive.read('dcp.xml').decode(),'wrong checkpoint target part')
        require({'ariane_xilinx.xdc','ariane_xilinx_early.xdc','ariane_xilinx_late.xdc','ariane_xilinx_board.xdc'}<=set(archive.namelist()),'checkpoint embedded constraints incomplete')
    require(not (fpga/'ariane.runs/impl_1/ariane_xilinx_routed.dcp').exists(),'original has routed checkpoint; investigate instead of restart')
    source=Path(old['source_manifest']);require(sha(source)==old['source_manifest_sha256'],'frozen source manifest changed')
    generic=fpga/'full-e3-top-generic.json';require(json.loads(generic.read_text())['DMAPathQueueDepth']==64,'actual original top generic differs')
    rels=['constraints/ariane.xdc','constraints/genesys-2.xdc','scripts/run.tcl','xilinx/xlnx_clk_gen/tcl/run.tcl']
    for rel in rels:require(sha(fpga/rel)==old['sources']['cva6/corev_apu/fpga/'+rel],'frozen export changed: '+rel)
    output.mkdir();inputs=output/'inputs';inputs.mkdir();os.link(cp,inputs/'physopt.dcp')
    # A hardlink shares its inode: never chmod/write it, including through Vivado.
    frozen={'parent-manifest.json':PARENT/'build-manifest.json','source-manifest.json':source,
            'original-top-generic.json':generic,'original-implementation.tcl':fpga/'ariane.runs/impl_1/ariane_xilinx.tcl',
            'original-runme.sh':fpga/'ariane.runs/impl_1/runme.sh','original-build.log':PARENT/'build.log',
            'original-project.xpr':fpga/'ariane.xpr',
            'original-runme.log':fpga/'ariane.runs/impl_1/runme.log','original-placed-utilization.rpt':fpga/'ariane.runs/impl_1/ariane_xilinx_utilization_placed.rpt',
            'boundary.tcl':HERE/'verify_routed_full.tcl','route.tcl':HERE/'resume_full_route.tcl','runner.py':Path(__file__)}
    frozen.update({rel.replace('/','__'):fpga/rel for rel in rels})
    for name,path in frozen.items():(inputs/name).write_bytes(path.read_bytes())
    m=copy.deepcopy(old)
    m.update(schema='ispass2027-full-e3-build-v1',action='fpga-ds0-continue-physopt',status='prepared_not_launched',start=now(),commands=[],
             parent_interrupted_build={'path':str(PARENT.resolve()),'manifest_sha256':PARENT_SHA,'status_as_left':'running','termination_cause':'unknown'},
             continuation={'stage':'route from original post-place physical optimization checkpoint','attempt':1,
                'checkpoint':str(cp.resolve()),'checkpoint_sha256':CHECKPOINT_SHA,'input_hardlink':str((inputs/'physopt.dcp').resolve()),
                'checkpoint_access':'read-only use; no chmod/write to shared inode','directive':'RuntimeOptimized',
                'new_synthesis_or_placement':False,'new_constraints_or_RTL':False,'max_worker_seconds':MAX_SECONDS},
             parent_script_sha256=old['script_sha256'],script_sha256=sha(__file__),
             actual_vivado_top_generic={'path':str((inputs/'original-top-generic.json').resolve()),'sha256':sha(generic),**json.loads(generic.read_text())},
             continuation_inputs={name:sha(inputs/name) for name in frozen},
             execution_repository={'path':str(HERE.parents[3]),'checkout_commit':subprocess.check_output(['git','-C',str(HERE.parents[3]),'rev-parse','HEAD'],text=True).strip()})
    save(output/'build-manifest.json',m);return m


def worker(output):
    path=output/'build-manifest.json';m=json.loads(path.read_text());require(m['status']=='prepared_not_launched','continuation already started')
    inputs=output/'inputs';require(sha(inputs/'physopt.dcp')==CHECKPOINT_SHA,'checkpoint bytes changed')
    for name,digest in m['continuation_inputs'].items():require(sha(inputs/name)==digest,'frozen continuation input changed: '+name)
    m.update(status='running',worker_pid=os.getpid(),worker_start=now());save(path,m)
    try:
        argv=[VIVADO,'-mode','batch','-nojournal','-log',str(output/'vivado.log'),'-source',str(inputs/'route.tcl'),
              '-tclargs',str(inputs/'physopt.dcp'),str(output),str(inputs/'boundary.tcl')]
        m['commands'].append({'argv':argv,'cwd':str(output),'start':now()});save(path,m)
        with (output/'build.log').open('xb') as log:
            proc=subprocess.Popen(argv,cwd=output,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
            try:code=proc.wait(timeout=MAX_SECONDS)
            except subprocess.TimeoutExpired:
                os.killpg(proc.pid,signal.SIGTERM)
                try:proc.wait(timeout=30)
                except subprocess.TimeoutExpired:os.killpg(proc.pid,signal.SIGKILL);proc.wait()
                raise TimeoutError('bounded continuation reached '+str(MAX_SECONDS)+' seconds; no retry')
        m['commands'][-1].update(end=now(),exit_code=code);require(code==0,'Vivado continuation failed exit='+str(code))
        timing=output/'ariane_xilinx_timing_summary_routed.rpt';routing=output/'ariane_xilinx_route_status.rpt'
        m['timing'],m['routing_errors'],passed=timing_gate(timing,routing)
        m['routed_boundary']=json.loads((output/'routed-boundary.json').read_text());require(m['routed_boundary']['passed'] is True,'routed boundary failed')
        m['routed_boundary_sha256']=sha(output/'routed-boundary.json')
        names=['ariane_xilinx.bit','ariane_xilinx.mcs','ariane_xilinx_routed.dcp','ariane_xilinx_timing_summary_routed.rpt',
               'ariane_xilinx_route_status.rpt','ariane_xilinx_drc_routed.rpt','ariane.utilization.rpt']
        m['artifacts']={str(output/name):sha(output/name) for name in names}
        m['status']='passed' if passed else 'timing_failed'
    except BaseException as e:m.update(status='failed',error=repr(e));raise
    finally:
        m['parent_checkpoint_unchanged']=sha(inputs/'physopt.dcp')==CHECKPOINT_SHA
        m['parent_manifest_unchanged']=sha(PARENT/'build-manifest.json')==PARENT_SHA
        if not m['parent_checkpoint_unchanged'] or not m['parent_manifest_unchanged']:m.update(status='failed',error='original input changed')
        m['end']=now();save(path,m)
    return 0 if m['status']=='passed' else 1


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--out',required=True,type=Path)
    mode=p.add_mutually_exclusive_group(required=True);mode.add_argument('--prepare',action='store_true');mode.add_argument('--launch',action='store_true');mode.add_argument('--worker',action='store_true',help=argparse.SUPPRESS)
    a=p.parse_args();output=a.out.resolve()
    if a.prepare:prepare(output);print(json.dumps({'status':'prepared_not_launched','manifest':str(output/'build-manifest.json')}))
    elif a.worker:return worker(output)
    else:
        m=json.loads((output/'build-manifest.json').read_text());require(m['status']=='prepared_not_launched' and not (output/'dispatch.json').exists(),'one continuation launch only')
        require(sha(__file__)==m['script_sha256'],'runner changed after preparation')
        with (output/'dispatcher.log').open('xb') as log:
            proc=subprocess.Popen([sys.executable,str(Path(__file__).resolve()),'--worker','--out',str(output)],stdin=subprocess.DEVNULL,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
        save(output/'dispatch.json',{'schema':'ispass2027-fpga-detached-dispatch-v1','pid':proc.pid,'start':now(),'max_worker_seconds':MAX_SECONDS,'source_script_sha256':sha(__file__),'out':str(output),'board_access':False})
        print(json.dumps({'status':'detached_worker_started','pid':proc.pid,'manifest':str(output/'build-manifest.json')}))

if __name__=='__main__':raise SystemExit(main())
