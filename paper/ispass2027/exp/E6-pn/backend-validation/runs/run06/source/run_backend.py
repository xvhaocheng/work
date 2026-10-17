#!/usr/bin/env python3
"""Finite actual fullwrap/frontend/jobFIFO/two real backend tests. No FPGA build."""
import argparse
import datetime as dt
import hashlib
import json
from pathlib import Path
import resource
import shutil
import subprocess

HERE = Path(__file__).resolve().parent
BASE = '8aac3fcd345af7686a55af1867783fd6b91a0069'
WRAP = 'corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv'
CERT = 'corev_apu/iDMA/src/idma_fullbuffer_certificate.sv'
CC = 'vendor/pulp-platform/common_cells'
AXI = 'vendor/pulp-platform/axi'
IDMA = 'corev_apu/iDMA'
REG = 'corev_apu/register_interface'
IOMMU = 'corev_apu/riscv-iommu'
SOURCES = [
    CC+'/src/cf_math_pkg.sv', AXI+'/src/axi_pkg.sv', IDMA+'/src/idma_pkg.sv',
    CC+'/src/fifo_v3.sv', CC+'/src/deprecated/fifo_v2.sv', CC+'/src/stream_fifo.sv',
    CC+'/src/spill_register_flushable.sv', CC+'/src/spill_register.sv', CC+'/src/fall_through_register.sv',
    CC+'/src/counter.sv', CC+'/src/delta_counter.sv', CC+'/src/lzc.sv', CC+'/src/onehot_to_bin.sv',
    CC+'/src/id_queue.sv', CC+'/src/rr_arb_tree.sv', CC+'/src/stream_arbiter_flushable.sv',
    CC+'/src/stream_arbiter.sv', CC+'/src/stream_register.sv',
    AXI+'/src/axi_demux_simple.sv', AXI+'/src/axi_demux.sv', AXI+'/src/axi_err_slv.sv',
    AXI+'/src/axi_atop_filter.sv', AXI+'/src/axi_burst_splitter.sv', AXI+'/src/axi_to_axi_lite.sv',
    REG+'/src/axi_lite_to_reg.sv', REG+'/src/axi_to_reg.sv',
    REG+'/vendor/lowrisc_opentitan/src/prim_subreg_arb.sv',
    REG+'/vendor/lowrisc_opentitan/src/prim_subreg.sv', REG+'/vendor/lowrisc_opentitan/src/prim_subreg_ext.sv',
    IDMA+'/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv',
    IDMA+'/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv',
    IDMA+'/src/frontends/idma_transfer_id_gen.sv',
    IDMA+'/src/frontends/register_64bit/idma_reg64_frontend.sv',
    IDMA+'/src/idma_stream_fifo.sv', IDMA+'/src/idma_buffer.sv', IDMA+'/src/idma_error_handler.sv',
    IDMA+'/src/idma_channel_coupler.sv', IDMA+'/src/idma_axi_transport_layer.sv',
    IDMA+'/src/idma_axi_lite_transport_layer.sv', IDMA+'/src/idma_obi_transport_layer.sv',
    IDMA+'/src/idma_legalizer.sv', IDMA+'/src/idma_backend.sv',
]
HEADERS = [CC+'/include/common_cells/assertions.svh', CC+'/include/common_cells/registers.svh',
           AXI+'/include/axi/typedef.svh', AXI+'/include/axi/assign.svh',
           IOMMU+'/include/axi-iommu/typedef.svh', IOMMU+'/include/axi-iommu/assign.svh',
           IDMA+'/src/include/idma/typedef.svh', IDMA+'/src/include/idma/guard.svh',
           REG+'/include/register_interface/typedef.svh', REG+'/include/register_interface/assign.svh']


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def utc():
    return dt.datetime.now(dt.timezone.utc).isoformat()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cva6', required=True, type=Path)
    p.add_argument('--deps', type=Path, default=Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6'))
    p.add_argument('--output', required=True, type=Path)
    p.add_argument('--only', choices=['0', '1'], action='append')
    p.add_argument('--verilator', default='verilator')
    a = p.parse_args()
    out, repo, deps = a.output.resolve(), a.cva6.resolve(), a.deps.resolve()
    out.mkdir(parents=True, exist_ok=False)
    frozen = out/'source'; frozen.mkdir()
    mf = {'schema':'pn-backend-real-finite-v1','status':'preparing','started_utc':utc(),
          'fixture_only':True,'evidence_level':'finite_real_fullwrap_backend_axi_memory',
          'board_points':0,'full_fpga_builds':0,'base_cva6':BASE,'actual_cva6':str(repo),
          'execution_head':subprocess.check_output(['git','-C',str(repo),'rev-parse','HEAD'],text=True).strip(),
          'execution_diff':subprocess.check_output(['git','-C',str(repo),'diff'],text=True),
          'commands':[],'summaries':[],'inputs':{},
          'parameters':{'DataWidth':64,'AddrWidth':64,'AxiIdWidth':2,'JobFifoDepth':8,'NumAxInFlight':8,
                        'BufferDepth':3,'NBufferDepth':512,'ErrorCap':'NO_ERROR_HANDLING','FPGA_EN':0},
          'limitations':['No IOMMU or P controller is instantiated; not whole-stack PN proof.',
                         'Original finite backend ASSERTS_OFF setting retained for vendor SVA; external C++ checks actual full handshakes, held payloads, data/guards, certificate and drain.',
                         'No synthesized RAM, fit, timing or board qualification.']}
    manifest = out/'manifest.json'
    def save(): manifest.write_text(json.dumps(mf,indent=2)+'\n')
    def execute(argv,label,timeout):
        entry={'argv':list(map(str,argv)),'log':label+'.log','started_utc':utc()};mf['commands'].append(entry);save()
        with (out/entry['log']).open('w') as log:
            r=subprocess.run(entry['argv'],stdout=log,stderr=subprocess.STDOUT,timeout=timeout,
                             preexec_fn=lambda:resource.setrlimit(resource.RLIMIT_CORE,(0,0)))
        entry.update(exit_code=r.returncode,finished_utc=utc(),sha256=sha(out/entry['log']));save()
        if r.returncode: raise RuntimeError(label+' failed; original log retained')
    save()
    try:
        baseline_path=deps.parents[1]/'build-manifest.json'
        baseline=json.loads(baseline_path.read_text())
        assert baseline['selected_cva6']==BASE
        mf['baseline_build_manifest']={'path':str(baseline_path),'sha256':sha(baseline_path)}
        for rel in SOURCES+HEADERS+['core/include/cv64a6_imafdc_sv39_config_pkg.sv']:
            source=deps/rel;dest=frozen/'deps'/rel;dest.parent.mkdir(parents=True,exist_ok=True)
            digest=sha(source)
            assert baseline['sources']['cva6/'+rel]==digest, 'baseline bytes mismatch: '+rel
            dest.write_bytes(source.read_bytes())
        original=subprocess.check_output(['git','-C',str(repo),'show',BASE+':'+WRAP])
        (frozen/'wrap.gold.original.sv').write_bytes(original)
        (frozen/'wrap.gold.renamed.sv').write_bytes(original.replace(b'dma_core_wrap',b'dma_core_wrap_gold'))
        shutil.copyfile(repo/WRAP,frozen/'wrap.candidate.sv')
        shutil.copyfile(repo/CERT,frozen/'certificate.candidate.sv')
        for name in ('pn_backend_top.sv','pn_backend_test.cpp','run_backend.py'):
            shutil.copyfile(HERE/name,frozen/name)
        (frozen/'ariane_config_stub.sv').write_text('// Fixed baseline config sets CVA6ConfigFPGAEn=0. No CPU instantiated.\npackage ariane_pkg; localparam bit FPGA_EN=0; endpackage\n')
        mf['inputs']={str(x.relative_to(out)):sha(x) for x in sorted(frozen.rglob('*')) if x.is_file()}
        mf['verilator_version']=subprocess.check_output([a.verilator,'--version'],text=True).strip();save()
        sources=[frozen/'ariane_config_stub.sv',*[frozen/'deps'/x for x in SOURCES],
                 frozen/'certificate.candidate.sv',frozen/'wrap.candidate.sv',frozen/'wrap.gold.renamed.sv',frozen/'pn_backend_top.sv']
        includes=['-I'+str(frozen/'deps'/x) for x in (CC+'/include',AXI+'/include',IOMMU+'/include',IDMA+'/src/include',REG+'/include')]
        for enable in (0,1):
            if a.only and str(enable) not in a.only: continue
            obj=out/('obj-enable'+str(enable));label='enable'+str(enable)
            execute([a.verilator,'--cc','--exe','--build','--assert','-j','2','-Wno-fatal',
                     '+define+ASSERTS_OFF','--top-module','pn_backend_top','--Mdir',obj,
                     '-GEnable='+str(enable),'-CFLAGS',f'-std=c++17 -O1 -DPN_ENABLE={enable}',
                     *includes,*sources,frozen/'pn_backend_test.cpp'],label+'-build',900)
            execute([obj/'Vpn_backend_top'],label+'-run',120)
            rows=[json.loads(x) for x in (out/(label+'-run.log')).read_text().splitlines() if x.startswith('{')]
            summaries=[x for x in rows if x['event']=='case_summary']
            assert rows[-1]=={'event':'all_passed','enable':enable}
            assert len(summaries)==(4 if enable==0 else 13)
            mf['summaries']+=summaries
            mf[label+'_binary_sha256']=sha(obj/'Vpn_backend_top');save()
        mf['status']='passed'
    except Exception as error:
        mf.update(status='failed',error=repr(error))
    mf.update(finished_utc=utc(),inputs_unchanged=all(sha(out/x)==s for x,s in mf['inputs'].items()))
    if not mf['inputs_unchanged']:mf.update(status='failed',error='compiled source changed')
    save();print(json.dumps({'status':mf['status'],'manifest':str(manifest),'cases':len(mf['summaries']),'error':mf.get('error')}))
    return 0 if mf['status']=='passed' else 1


if __name__=='__main__':raise SystemExit(main())
