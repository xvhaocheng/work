#!/usr/bin/env python3
"""Freeze only the fixed source/numbered excerpts used by the top proposal."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--out',type=Path,required=True);args=ap.parse_args()
if args.out.exists():raise FileExistsError(args.out)
I=Path('/home/xvhaocheng/work-e6-pn-iommu');IC='5b7dfbc70e458244a8e5682e5ae3f8571320254c'
C=Path('/home/xvhaocheng/work-e6-pn-cva6');CC='8aac3fcd345af7686a55af1867783fd6b91a0069'
BASE=Path('/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor')
rows=[]
def add(repo,ref,path,ranges,working=None):
 blob=subprocess.check_output(['git','rev-parse',ref+':'+path],cwd=repo,text=True).strip();data=subprocess.check_output(['git','show',ref+':'+path],cwd=repo);lines=data.decode().splitlines()
 working=Path(working) if working else repo/path
 same=working.exists() and data==working.read_bytes();assert same,(working,ref,path)
 rows.append({'repository':str(repo),'commit':ref,'path':path,'git_blob':blob,'sha256':hashlib.sha256(data).hexdigest(),'read_path':str(working),'working_bytes_equal':same,'excerpts':[{'first':a,'last':b,'numbered':'\n'.join(str(n)+': '+lines[n-1] for n in range(a,min(b,len(lines))+1))} for a,b in ranges]})
entries={
'rtl/rv_iommu_top.sv':[(111,139),(224,251),(322,379),(451,519),(526,605),(610,680)],
'rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv':[(89,101),(112,212),(287,327),(348,475),(540,557),(647,684)],
'rtl/transaction_controller/rv_iommu_banked_ctl.sv':[(137,179),(244,280),(332,405),(522,584)],
'rtl/transaction_controller/rv_iommu_mrif_handler.sv':[(71,91),(173,218),(261,343)],
'rtl/transaction_controller/rv_iommu_ign_slv.sv':[(38,66),(80,131)],
'rtl/translation_logic/rv_iommu_tl_wrap.sv':[(279,338),(363,409),(530,550),(613,625),(758,770),(1397,1412)],
'rtl/translation_logic/rv_iommu_ddtw.sv':[(43,49),(99,107),(452,482)],
'rtl/translation_logic/rv_iommu_pdtw.sv':[(79,100)],
'rtl/translation_logic/rv_iommu_ptw.sv':[(90,99)],
'rtl/translation_logic/rv_iommu_msiptw.sv':[(77,100),(159,170),(241,294),(349,361),(410,471)],
'rtl/ext_interfaces/rv_iommu_ds_if.sv':[(77,219),(249,322)],
'rtl/ext_interfaces/rv_iommu_prog_if.sv':[(57,139)],
'vendor/modified/rv_iommu_axi2apb_64_32.sv':[(172,211),(214,352),(448,455),(534,542)],
'rtl/software_interface/rv_iommu_cq_handler.sv':[(108,155),(238,312),(348,395),(466,503)],
'rtl/software_interface/rv_iommu_fq_handler.sv':[(101,132),(182,218),(249,279)],
'rtl/software_interface/rv_iommu_fault_arbiter.sv':[(44,107)],
'rtl/software_interface/rv_iommu_pq_handler.sv':[(84,118),(130,139),(188,225),(228,283)],
'rtl/software_interface/rv_iommu_msi_ig.sv':[(65,89),(136,185),(187,224)],
'rtl/software_interface/rv_iommu_dbg_if_ctl.sv':[(47,105)],
'rtl/software_interface/rv_iommu_sw_if_wrap.sv':[(179,237),(243,307),(320,406),(456,519),(547,583)],
'rtl/software_interface/regmap/rv_iommu_regmap.sv':[(64,73),(2335,2361)],
'packages/rv_iommu_pkg.sv':[(480,497)]}
for path,ranges in entries.items():add(I,IC,path,ranges)
for path,ranges in {'src/axi_demux.sv':[(78,209)],'src/axi_demux_simple.sv':[(88,120),(149,253),(348,381),(513,557),(559,617)]}.items():add(BASE/'axi','e72230ef6c8cd24387b788b69d02c10174decd45',path,ranges,Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6/corev_apu/riscv-iommu/vendor/axi')/path)
for path,ranges in {'src/spill_register_flushable.sv':[(37,95)],'src/stream_fifo.sv':[(28,61)]}.items():add(BASE/'common_cells','f36828f3fd76fa6c79f80cb495829cf967bcf20c',path,ranges,Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6/corev_apu/riscv-iommu/vendor/common_cells')/path)
for path,ranges in {'src/axi_aw_buffer.sv':[(58,71)],'src/axi_single_slice.sv':[(20,100)]}.items():add(BASE/'axi_slice','bc864aaeaf5ed960f21fde86bba53d6edfa16683',path,ranges,Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6/corev_apu/riscv-iommu/vendor/axi_slice')/path)
for path,ranges in {'corev_apu/tb/ariane_soc_pkg.sv':[(26,34),(110,126)],'corev_apu/fpga/src/ariane_peripherals_xilinx.sv':[(868,870),(990,1012),(1031,1035),(1137,1183)],'vendor/pulp-platform/axi/src/axi_mux.sv':[(141,158)],'vendor/pulp-platform/axi/src/axi_id_prepend.sv':[(79,93)]}.items():add(C,CC,path,ranges)
root_contract=Path('/home/xvhaocheng/work-root-a/paper/ispass2027/exp/E6-pn/runtime-contract-v1.md')
result={'scope':'source-only proposal; no RTL/build/simulation/board execution','iommu_fixed':IC,'cva6_fixed':CC,'sources':rows,'root_runtime_contract':{'path':str(root_contract),'sha256':hashlib.sha256(root_contract.read_bytes()).hexdigest()},'board_cases':0,'models':'Codex-only; runtime model metadata not exposed; no settings/cache edits'}
args.out.write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n');print('fixed_sources='+str(len(rows))+' all_working_bytes_equal=true')
if __name__=='__main__':pass
