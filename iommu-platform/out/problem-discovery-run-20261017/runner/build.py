#!/usr/bin/env python3
"""Independent bounded discovery build, with one CVA6 AXI/common_cells vendor tree.

Only --out is written. --overlay mirrors iommu-platform relative source paths.
Never invokes either legacy runner main or a board build. Build only by default.
"""
import argparse, hashlib, importlib.util, json, os, signal, shlex, subprocess, sys, time
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in HERE.parents if (p/'tests/test_nonblock_actual_dma.py').is_file())
TOP='nonblock_actual_dma_system_top'

def load(path,name):
    spec=importlib.util.spec_from_file_location(name,path)
    module=importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
    return module

def unique(items): return list(dict.fromkeys(items))

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--overlay',required=True,type=Path)
    ap.add_argument('--out',required=True,type=Path)
    ap.add_argument('--policy',required=True,type=int,choices=[0])
    ap.add_argument('--jobs',default=8,type=int)
    ap.add_argument('--dry-run',action='store_true')
    ap.add_argument('--lint-only',action='store_true')
    ap.add_argument('--cpp',type=Path,default=HERE/'discovery_dma.cpp')
    ap.add_argument('--extra-sv',action='append',type=Path,default=[])
    args=ap.parse_args()
    overlay=args.overlay.resolve(); out=args.out.resolve()
    if out==ROOT/'out/nonblock-actual-dma' or not out.is_relative_to(ROOT/'out/problem-discovery-run-20261017'):
        ap.error('--out must be inside this task output tree')
    old=load(ROOT/'tests/test_nonblock_actual_dma.py','actual_source_list')
    banked=load(ROOT/'tests/test_nonblock_banked.py','banked_source_list')
    iommu_vendor=old.IOMMU/'vendor'
    cva6_vendor=old.CVA6/'vendor/pulp-platform'
    replacements=[]
    def unified(path):
        path=Path(path)
        for lib in ('axi','common_cells'):
            src=iommu_vendor/lib
            if path.is_relative_to(src):
                dst=cva6_vendor/lib/path.relative_to(src)
                replacements.append({'from':str(path.relative_to(ROOT)),'to':str(dst.relative_to(ROOT))})
                return dst
        return path
    def resolved(path):
        path=unified(path)
        override=overlay/path.relative_to(ROOT)
        chosen=override if override.is_file() else path
        if not chosen.is_file(): raise FileNotFoundError(chosen)
        return chosen
    atomics=old.CVA6/'corev_apu/src/axi_riscv_atomics/src'
    # This CVA6 vendor revision's stream/fall-through registers still instantiate
    # the deprecated fifo_v2 adapter. It depends only on fifo_v3, already present
    # above through the unified production FIFO family; keep the adapter from
    # the same vendor tree instead of reverting those registers to another tree.
    fifo_compat=[cva6_vendor/'common_cells/src/deprecated/fifo_v2.sv']
    raw=([old.ARIANE_PKG_SRC]+banked.SOURCES+old.idma_sources()+old.production_fifo_sources()+fifo_compat
         +[ROOT/'tests/nonblock_completion_pool_late.sv',ROOT/'tests/nonblock_completion_pool.sv',ROOT/'tests/nonblock_actual_dma_top.sv']
         +old.system_sources()+[atomics/n for n in ('axi_res_tbl.sv','axi_riscv_lrsc.sv','axi_riscv_amos_alu.sv','axi_riscv_amos.sv','axi_riscv_atomics.sv')])
    sources=unique(resolved(p) for p in raw)
    # Include all explicitly added overlay SV files once, before wrappers. The
    # optional file names in sources-extra.json are relative to overlay root.
    extra_file=overlay/'sources-extra.json'
    extras=[p.resolve() for p in args.extra_sv]
    gate=overlay/'tests/m_first_pte_gate.sv'
    if gate.is_file(): extras.append(gate)
    if extra_file.is_file():
        extras += [(overlay/p).resolve() for p in json.loads(extra_file.read_text())]
    if extras:
        for p in extras:
            if not p.is_file() or not p.is_relative_to(overlay): raise ValueError(p)
        # All packages imported by extra modules should be in the inherited
        # package block; no second package/module definition is allowed.
        idx=next(i for i,p in enumerate(sources) if p.name=='nonblock_banked_top.sv')
        sources=unique(sources[:idx]+extras+sources[idx:])
    # Compile includes from overlay first so policy copies can add headers.
    incdirs=[]
    for p in unique([unified(p) for p in banked.INCDIRS+old.idma_incdirs()]):
        alt=overlay/p.relative_to(ROOT)
        if alt.is_dir(): incdirs.append(alt)
        incdirs.append(p)
    # An all-overlay include root supports test-local policy header includes.
    incdirs=[overlay]+unique(incdirs)
    obj=out/'obj'; out.mkdir(parents=True,exist_ok=True)
    command=['verilator','--lint-only' if args.lint_only else '--cc']
    if not args.lint_only: command+=['--exe','--build','-j',str(args.jobs)]
    command+=['--assert','-Wno-fatal','+define+ASSERTS_OFF','-GPolicyEnable=1',f'-GPairPolicy={args.policy}',
              '--top-module',TOP,'--Mdir',str(obj),'-CFLAGS',f'-DACTUAL_DMA_SYSTEM -DPAIRED_POLICY={args.policy}']
    command += ['-I'+str(p) for p in incdirs]+[str(p) for p in sources]
    if not args.lint_only: command += [str(args.cpp.resolve())]
    manifest={'schema':'paired-build-v1','policy':args.policy,'top':TOP,'overlay':str(overlay),'out':str(out),
              'sources':[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in sources],
              'cpp':{'path':str(args.cpp.resolve()),'sha256':hashlib.sha256(args.cpp.read_bytes()).hexdigest()},
              'includes':[str(p) for p in incdirs],'vendor_replacements':unique(json.dumps(x,sort_keys=True) for x in replacements),
              'argv':command,'state':'dry_run' if args.dry_run else 'building','started_unix':time.time()}
    (out/'build-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (out/'command.sh').write_text('#!/bin/sh\nexec '+shlex.join(command)+'\n')
    if args.dry_run:
        print(json.dumps({'sources':len(sources),'manifest':str(out/'build-manifest.json'),'executable':str(obj/('V'+TOP))})); return 0
    with (out/'build.log').open('w') as log:
        process=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,cwd=ROOT,start_new_session=True)
        try:
            process.wait(timeout=900)
        except subprocess.TimeoutExpired:
            print('Build hard timeout: stopping the build process group',file=sys.stderr)
            os.killpg(process.pid,signal.SIGKILL)
            process.wait()
            manifest.update(state='timeout',exit_code=124,ended_unix=time.time())
            (out/'build-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
            print('Build hard timeout 900 seconds',file=sys.stderr)
            return 124
    manifest.update(state='passed' if process.returncode==0 else 'failed',exit_code=process.returncode,ended_unix=time.time())
    (out/'build-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    if process.returncode:
        print('\n'.join((out/'build.log').read_text(errors='replace').splitlines()[-100:]),file=sys.stderr)
    else: print(str(obj/('V'+TOP)))
    return process.returncode
if __name__=='__main__': sys.exit(main())
