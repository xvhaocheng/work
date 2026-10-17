#!/usr/bin/env python3
"""Prepare same-source PN DS50 only after the existing real S15 gate passes."""
import argparse,importlib.util,json,re
from pathlib import Path
HERE=Path(__file__).resolve().parent
PLATFORM=Path('/home/xvhaocheng/work/iommu-platform')
CVA='109b48fdde862b668d072bb20ca13c27dbade1fe'
IOMMU='86c2cee6b82fc0588e233000926afe3ab9a2fb8f'
AXI='95f121700539d9e44430ec79786780d3a1f9e87d'
def _has_fixture(value):
 if isinstance(value,dict):
  return any((key=='fixture_only' and child is True) or
   (key in {'evidence_level','evidence_kind','source_kind'} and isinstance(child,str) and any(x in child.lower() for x in ['synthetic','fixture','mock'])) or _has_fixture(child)
   for key,child in value.items())
 if isinstance(value,list):return any(_has_fixture(x) for x in value)
 return False

def validate_gate(g):
 if _has_fixture(g):raise ValueError('synthetic/fixture gate cannot authorize DS50')
 if g.get('schema')!='ispass2027.e6_pn.s15_gate.v1' or g.get('status')!='passed' or g.get('eligible_for_DS50') is not True or g.get('numerical_gate_passed') is not True:raise ValueError('real passed S15 gate required')
 if g.get('fixture_only') is not False or g.get('source_board_cases')!=15 or g.get('source_cases')!=15 or g.get('synthetic_markers'):raise ValueError('S15 gate must contain 15 actual board cases and no fixture markers')
 if g.get('selected_cva6')!=CVA or g.get('selected_iommu')!=IOMMU or g.get('control_actions')!=[]:raise ValueError('S15 source/mode identity mismatch')
 cells=g.get('cells',[])
 if len(cells)!=3 or {c.get('condition') for c in cells}!={'Solo','A1','A8'} or any(c.get('n')!=5 or c.get('passed') is not True for c in cells):raise ValueError('all actual Solo/A1/A8 n5 cells must pass')
 return g

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',required=True,type=Path);p.add_argument('--s15-gate',required=True,type=Path);p.add_argument('--out',required=True,type=Path);p.add_argument('--check-only',action='store_true');a=p.parse_args()
 source=a.source.resolve();selected=json.loads(source.read_text());gate=a.s15_gate.resolve();validate_gate(json.loads(gate.read_text()))
 spec=importlib.util.spec_from_file_location('pn_ds50_existing_builder',HERE/'source/build_baseline.as-reused.py');base=importlib.util.module_from_spec(spec);spec.loader.exec_module(base);base.WORK=HERE.parents[4];base.PLATFORM=PLATFORM;base.HERE=HERE
 def validate(path):
  s=json.loads(Path(path).read_text());assert s==selected and s['schema']=='ispass2027-e6-pn-source-v1' and s['status']=='prepared'
  cs={x['path']:x['commit'] for x in s['components']};assert len(cs)==len(s['components'])
  assert s['selected_cva6']==cs['cva6']==CVA and s['iommu']==cs['cva6/corev_apu/riscv-iommu']==IOMMU and cs['cva6/corev_apu/riscv-iommu/vendor/axi']==AXI
  assert s['parameters']['PnEnable']==1 and s['parameters']['HistoryControlEnable']==1
  for path,commit in cs.items():assert re.fullmatch('[0-9a-f]{40}',commit) and base.git(PLATFORM/path,'rev-parse',commit+'^{commit}').decode().strip()==commit
  return s,cs
 base.validate_source=validate;validate(source)
 if a.check_only:print(json.dumps({'status':'validated_DS50_after_real_S15','source':str(source),'s15_gate':str(gate),'ds_cycles':[50,50]}));return 0
 original=base.save
 def save(path,value):
  if Path(path).name=='build-manifest.json':
   value.update(schema='ispass2027-e6-pn-build-v1',builder_wrapper_sha256=base.sha(__file__),s15_gate=str(gate),s15_gate_sha256=base.sha(gate))
   if 'parameters' in value:value['parameters'].update({**selected['parameters'],'ds_delay_enable':True,'ds_cycles':[50,50]})
  return original(path,value)
 base.save=save;base.fpga(a.out.resolve(),source,'ds50')
 m=json.loads((a.out.resolve()/'build-manifest.json').read_text());return 0 if m['status']=='passed' else 1
if __name__=='__main__':raise SystemExit(main())
