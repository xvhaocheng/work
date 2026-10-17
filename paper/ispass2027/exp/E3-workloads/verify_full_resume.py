#!/usr/bin/env python3
"""Finite resume protocol/timing gates. Tcl stubs, no Vivado or board execution."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import resume_full_fpga as runner
HERE=Path(__file__).resolve().parent


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
    checks=[]
    report=a.output/'timing.rpt';routing=a.output/'route.rpt'
    for field,value,passed in [('all',0,True),('wns',-.001,False),('setup',1,False),('hold',1,False),('pulse',1,False),('routing',1,False)]:
        data=[.1,0,0,1,.1,0,0,1,.1,0,0,1]
        if field!='all' and field!='routing':data[{'wns':0,'setup':2,'hold':6,'pulse':10}[field]]=value
        report.write_text(' WNS(ns) TNS(ns) ...\n ----------------\n '+' '.join(map(str,data))+'\n')
        routing.write_text('# of nets with routing errors..... : '+str(value if field=='routing' else 0)+'\n')
        assert runner.timing_gate(report,routing)[2] is passed
        checks.append('original timing/routing gate '+field)
    routing.write_text('missing counter\n')
    try:runner.timing_gate(report,routing)
    except ValueError:checks.append('missing routing counter rejected')
    else:raise AssertionError('missing counter accepted')
    cp=a.output/'physopt.dcp';cp.write_bytes(b'explicit host-only checkpoint fixture\n');before=cp.read_bytes()
    out=a.output/'tcl-output';out.mkdir();script=a.output/'harness.tcl'
    script.write_text(r'''
set calls {}
proc record {name args} { global calls; lappend calls [linsert $args 0 $name] }
foreach name {set_param reset_param set_property report_drc report_methodology report_power report_route_status report_timing_summary report_incremental_reuse report_clock_utilization report_bus_skew check_timing report_timing report_utilization close_design} {
    proc $name {args} [format {record %s {*}$args} $name]
}
proc open_checkpoint {path} {record open_checkpoint $path}
proc current_project {} {return project_fixture}
proc current_design {} {return design_fixture}
proc get_clocks {args} {return cpu_clock_fixture}
proc get_cells {args} {return {}}
proc get_property {name obj} {if {$name eq "PERIOD"} {return 20.0};if {$name eq "BITSTREAM.CONFIG.SPI_BUSWIDTH"} {return 4};error "unexpected property"}
proc route_design {args} {record route_design {*}$args}
proc write_checkpoint {args} {record write_checkpoint {*}$args}
proc write_bitstream {args} {record write_bitstream {*}$args}
proc write_cfgmem {args} {record write_cfgmem {*}$args}
set argc 3
set argv [list [lindex $argv 0] [lindex $argv 1] [lindex $argv 2]]
source [lindex $::env(E3_RESUME_TCL) 0]
set target [open $::env(E3_RESUME_CALLS) w]
foreach call $calls {puts $target $call}
close $target
''')
    import os
    env=dict(os.environ,E3_RESUME_TCL=str(HERE/'resume_full_route.tcl'),E3_RESUME_CALLS=str(a.output/'calls.txt'))
    result=subprocess.run(['tclsh',str(script),str(cp.resolve()),str(out.resolve()),str((HERE/'verify_routed_full.tcl').resolve())],env=env,capture_output=True,text=True)
    (a.output/'tcl.log').write_text(result.stdout+result.stderr);assert result.returncode==0,result.stderr
    calls=(a.output/'calls.txt').read_text().splitlines()
    assert [x for x in calls if x.startswith('route_design ')]==['route_design -directive RuntimeOptimized']
    assert not any(x.startswith(('synth_design ','place_design ','phys_opt_design ','read_xdc ')) for x in calls)
    assert [x for x in calls if x.startswith('write_checkpoint ')]==['write_checkpoint -force '+str(out.resolve()/'ariane_xilinx_routed.dcp')]
    assert cp.read_bytes()==before and json.loads((out/'routed-boundary.json').read_text())['passed'] is True
    assert len([x for x in calls if x.startswith('open_checkpoint ')])==2
    checks.append('actual resume Tcl issues original route once, writes only new checkpoint, no synthesis/place/physopt/read_xdc, original boundary verifier runs')
    required=['set_param chipscope.maxJobs 1','set_param checkpoint.writeSynthRtdsInDcp 1',
              'set_param synth.incrementalSynthesisCache ./.Xil/Vivado-1879670-xvhaocheng-dell/incrSyn','set_param runs.launchOptions { -jobs 1 }']
    # Tcl canonical list spacing is preserved in calls; compare tokens after normalization.
    actual={' '.join(x.split()) for x in calls}
    assert all(' '.join(x.split()) in actual for x in required),actual
    checks.append('original generated implementation set_param values retained')
    receipt={'status':'passed','fixture_only':True,'new_board_measurements':0,'checks':checks,
        'scope':'stub Tcl command protocol and actual original timing parser; not real routing/DRC/bitstream proof',
        'source_sha256':{name:hashlib.sha256((HERE/name).read_bytes()).hexdigest() for name in ['resume_full_fpga.py','resume_full_route.tcl','verify_full_resume.py']}}
    (a.output/'report.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps({'status':'passed','checks':len(checks),'new_board_measurements':0}))

if __name__=='__main__':main()
