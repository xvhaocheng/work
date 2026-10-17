#!/usr/bin/env python3
"""Immutable, source-only TL/MSIPTW proposal evidence."""
import argparse,hashlib,json,subprocess
from pathlib import Path
REPO=Path('/home/xvhaocheng/work-e6-pn-iommu');REF='6082d3bc2e0aeafcf4e86c42ddf1485c778fe941'
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--out',type=Path,required=True);a=ap.parse_args()
 if a.out.exists():raise FileExistsError(a.out)
 rows=[]
 for p,ranges in {
 'rtl/translation_logic/rv_iommu_tl_wrap.sv':[(25,70),(154,212),(279,299),(363,409),(528,562),(567,654),(695,738),(747,879),(1397,1507)],
 'rtl/translation_logic/rv_iommu_msiptw.sv':[(25,100),(123,170),(241,314),(349,375),(410,502)],
 'rtl/translation_logic/rv_iommu_ddtw.sv':[(42,49),(99,108)],
 'rtl/translation_logic/rv_iommu_pdtw.sv':[(38,46),(93,102)],
 'rtl/translation_logic/rv_iommu_ptw.sv':[(38,46),(91,100)],
 'rtl/software_interface/rv_iommu_dbg_if_ctl.sv':[(47,104)],
 'packages/rv_iommu_pkg.sv':[(481,497)],
 'rtl/ext_interfaces/rv_iommu_ds_if.sv':[(330,360)],
 'rtl/ext_interfaces/rv_iommu_ds_debt.sv':[(1,50)]}.items():
  raw=subprocess.check_output(['git','show',REF+':'+p],cwd=REPO);assert raw==(REPO/p).read_bytes();lines=raw.decode().splitlines()
  rows.append({'repository':str(REPO),'commit':REF,'path':p,'git_blob':subprocess.check_output(['git','rev-parse',REF+':'+p],cwd=REPO,text=True).strip(),'sha256':hashlib.sha256(raw).hexdigest(),'working_bytes_equal':True,'excerpts':[{'first':lo,'last':hi,'numbered':'\n'.join(str(n)+': '+lines[n-1] for n in range(lo,min(hi,len(lines))+1))}for lo,hi in ranges]})
 a.out.write_text(json.dumps({'scope':'source-only TL/MSIPTW readonly quiet+stable MSI-source correction proposal; no implementation/run/build/board','fixed_iommu':REF,'sources':rows,'board_cases':0},ensure_ascii=False,indent=2)+'\n');print(f'{len(rows)} fixed sources, actual bytes equal Git')
if __name__=='__main__':main()
