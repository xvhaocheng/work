#!/usr/bin/env python3
"""Freeze the exact TC/vendor source excerpts used by this source-only proposal."""
import argparse,hashlib,json,subprocess
from pathlib import Path
I=Path('/home/xvhaocheng/work-e6-pn-iommu');IC='9713cd0cee4ba23fa37041d9bf48e2d58ece74af'
V=Path('/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor')
D=Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6/corev_apu/riscv-iommu/vendor')
A='e72230ef6c8cd24387b788b69d02c10174decd45';C='f36828f3fd76fa6c79f80cb495829cf967bcf20c'
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--out',type=Path,required=True);a=ap.parse_args()
 if a.out.exists():raise FileExistsError(a.out)
 rows=[]
 def add(repo,ref,path,ranges,read=None):
  data=subprocess.check_output(['git','show',ref+':'+path],cwd=repo);lines=data.decode().splitlines();read=Path(read) if read else repo/path
  assert read.read_bytes()==data
  rows.append({'repository':str(repo),'commit':ref,'path':path,'git_blob':subprocess.check_output(['git','rev-parse',ref+':'+path],cwd=repo,text=True).strip(),'sha256':hashlib.sha256(data).hexdigest(),'read_path':str(read),'read_bytes_equal_git':True,'excerpts':[{'first':lo,'last':hi,'numbered':'\n'.join(f'{k}: '+lines[k-1] for k in range(lo,min(hi,len(lines))+1))} for lo,hi in ranges]})
 for p,r in {
 'rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv':[(19,55),(89,212),(287,327),(348,429),(431,539),(544,557),(578,684)],
 'rtl/transaction_controller/rv_iommu_banked_ctl.sv':[(47,57),(130,193),(240,308),(313,361),(368,447),(455,518),(522,613)],
 'rtl/transaction_controller/rv_iommu_mrif_handler.sv':[(23,91),(124,218),(219,255),(258,343),(352,377)],
 'rtl/transaction_controller/rv_iommu_ign_slv.sv':[(40,67),(80,131)],
 'Bender.yml':[(22,29),(48,72)],
 'rtl/ext_interfaces/rv_iommu_ds_if.sv':[(330,360)]}.items():add(I,IC,p,r)
 for p,r in {
 'src/axi_demux.sv':[(42,87),(89,117),(119,160),(162,209)],
 'src/axi_demux_simple.sv':[(43,120),(137,230),(232,253),(295,374),(380,458),(513,618)],
 'src/axi_mux.sv':[(48,76),(172,188),(283,347),(349,380),(392,463)],
 'src/axi_id_prepend.sv':[(84,126)],
 'src/axi_err_slv.sv':[(20,104)]}.items():add(V/'axi',A,p,r,D/'axi'/p)
 for p,r in {
 'src/stream_fifo.sv':[(12,64)],
 'src/fifo_v3.sv':[(45,75),(76,129)],
 'src/spill_register_flushable.sv':[(34,94)],
 'src/counter.sv':[(14,42)],
 'src/delta_counter.sv':[(14,120)],
 'src/rr_arb_tree.sv':[(40,100),(180,240)]}.items():add(V/'common_cells',C,p,r,D/'common_cells'/p)
 pins={x:subprocess.check_output(['git','rev-parse',IC+':vendor/'+x],cwd=I,text=True).strip() for x in ['axi','common_cells']}
 assert pins=={'axi':A,'common_cells':C}
 result={'scope':'source-only TC/banked/MRIF/vendor quiet proposal; no RTL edits/build/simulation/board','fixed_iommu':IC,'fixed_vendor_axi':A,'fixed_common_cells':C,'actual_iommu_gitlinks':pins,'sources':rows,'board_cases':0}
 a.out.write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n');print(f'{len(rows)} fixed blobs; read bytes equal Git; IOMMU gitlinks matched')
if __name__=='__main__':main()
