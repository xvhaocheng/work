#!/usr/bin/env python3
"""Frozen PN DS0 via existing FPGA flow; no hardware access. Pass Root-local source JSON."""
import argparse,importlib.util,json,re
from pathlib import Path
HERE=Path(__file__).resolve().parent
PLATFORM=Path('/home/xvhaocheng/work/iommu-platform')
CVA='109b48fdde862b668d072bb20ca13c27dbade1fe'
IOMMU='86c2cee6b82fc0588e233000926afe3ab9a2fb8f'
AXI='95f121700539d9e44430ec79786780d3a1f9e87d'
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',required=True,type=Path);p.add_argument('--out',required=True,type=Path);p.add_argument('--check-only',action='store_true');a=p.parse_args()
 source=a.source.resolve();selected=json.loads(source.read_text());out=a.out.resolve()
 spec=importlib.util.spec_from_file_location('pn_existing_builder',HERE/'source/build_baseline.as-reused.py');base=importlib.util.module_from_spec(spec);spec.loader.exec_module(base)
 base.WORK=HERE.parents[4];base.PLATFORM=PLATFORM;base.HERE=HERE
 def validate(path):
  s=json.loads(Path(path).read_text());assert s==selected
  assert s['schema']=='ispass2027-e6-pn-source-v1' and s['status']=='prepared'
  assert s['selected_cva6']==CVA and s['iommu']==IOMMU
  cs={x['path']:x['commit'] for x in s['components']};assert len(cs)==len(s['components'])
  assert cs['cva6']==CVA and cs['cva6/corev_apu/riscv-iommu']==IOMMU and cs['cva6/corev_apu/riscv-iommu/vendor/axi']==AXI
  for path,commit in cs.items():
   assert re.fullmatch('[0-9a-f]{40}',commit);assert base.git(PLATFORM/path,'rev-parse',commit+'^{commit}').decode().strip()==commit
  assert base.git(PLATFORM/'cva6','rev-parse',CVA+':corev_apu/riscv-iommu').decode().strip()==IOMMU
  assert base.git(PLATFORM/'cva6/corev_apu/riscv-iommu','rev-parse',IOMMU+':vendor/axi').decode().strip()==AXI
  rtl=base.show(PLATFORM/'cva6',CVA,'corev_apu/fpga/src/ariane_xilinx.sv').decode()
  for key in ['PnEnable','HistoryControlEnable']:assert re.search(r'localparam bit '+key+r'\s*=\s*1;',rtl)
  assert s['parameters']['PnEnable']==1 and s['parameters']['HistoryControlEnable']==1
  return s,cs
 base.validate_source=validate;validate(source)
 if a.check_only:print(json.dumps({'status':'validated','source':str(source),'cva6':CVA,'iommu':IOMMU}));return 0
 original=base.save
 def save(path,value):
  if Path(path).name=='build-manifest.json':
   value['schema']='ispass2027-e6-pn-build-v1';value['builder_wrapper_sha256']=base.sha(__file__)
   if 'parameters'in value:value['parameters'].update(selected['parameters'])
  return original(path,value)
 base.save=save
 base.fpga(out,source,'ds0')
 m=json.loads((out/'build-manifest.json').read_text());return 0 if m['status']=='passed' else 1
if __name__=='__main__':raise SystemExit(main())
