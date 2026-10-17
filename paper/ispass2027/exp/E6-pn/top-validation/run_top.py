#!/usr/bin/env python3
import argparse,json,subprocess,re,time
from pathlib import Path
ap=argparse.ArgumentParser();ap.add_argument('--out',required=True,type=Path);ap.add_argument('--lint',action='store_true');a=ap.parse_args()
repo=Path('/home/xvhaocheng/work-e6-pn-iommu');deps=Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6/corev_apu/riscv-iommu');ax=Path('/home/xvhaocheng/work-e6-pn-axi');out=a.out.resolve();out.mkdir(parents=True,exist_ok=False);driver=Path(__file__).resolve().parent
cc=['cf_math_pkg','lzc','counter','delta_counter','fifo_v3','spill_register_flushable','spill_register','rr_arb_tree','stream_fifo','stream_arbiter','stream_arbiter_flushable','stream_mux','stream_demux']
sources=[deps/'vendor/common_cells/src'/(x+'.sv') for x in cc]+[ax/'src'/(x+'.sv') for x in ['axi_pkg','axi_demux_simple','axi_demux','axi_mux','axi_id_prepend','axi_err_slv']]
sources +=[deps/'vendor/common_cells/src/deprecated/fifo_v2.sv']+[deps/'vendor/axi_slice/src'/(x+'.sv') for x in ['axi_single_slice','axi_aw_buffer','axi_w_buffer','axi_b_buffer','axi_ar_buffer','axi_r_buffer']]
lines=(repo/'Bender.yml').read_text().splitlines();rels=[]
for l in lines:
 m=re.match(r'  - (.+\.sv)$',l)
 if m and not m[1].startswith('sim/') and 'rv_iommu_wrap.sv' not in m[1]:rels.append(m[1])
sources += [repo/r if (repo/r).exists() else deps/r for r in rels]
config=out/'config.vlt';config.write_text('`verilator_config\nno_inline -module "rv_iommu_*"\n');sources+=[config,driver/'pn_top_test.sv']
inc=[deps/'vendor/axi/include',deps/'vendor/common_cells/include',deps/'vendor/register_interface/include',repo/'include']
v='/home/xvhaocheng/work/iommu-platform/out/stage117-real-cache-handoff-20261013/implementation/toolchain/verilator-5.040/bin/verilator'
cmd=[v,'--lint-only' if a.lint else '--cc','-Wno-fatal','--top-module','pn_top_test','-GGold=0']+['-I'+str(x) for x in inc]+list(map(str,sources))
if not a.lint:cmd[2:2]=['--exe','--build','--assert','-j','2','--Mdir',str(out/'obj')];cmd +=[str(driver/'pn_top_test.cpp')]
m={'command':cmd,'head':subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip(),'diff':subprocess.check_output(['git','diff'],cwd=repo,text=True),'scope':'actual complete IOMMU top; external memory is host fixture; no CVA/DMA/FPGA/board'}
(out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n');start=time.monotonic()
with(out/'build.log').open('w')as f:r=subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,timeout=300)
m.update(exit=r.returncode,elapsed_s=time.monotonic()-start)
if r.returncode==0 and not a.lint:
 with(out/'run.log').open('w')as f:r=subprocess.run([str(out/'obj/Vpn_top_test')],stdout=f,stderr=subprocess.STDOUT,timeout=30)
 m['run_exit']=r.returncode
(out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n');print(json.dumps({'out':str(out),'exit':r.returncode}));raise SystemExit(r.returncode)
