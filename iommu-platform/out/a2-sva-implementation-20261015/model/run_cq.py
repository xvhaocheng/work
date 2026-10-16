#!/usr/bin/env python3
"""New bounded functional CQ-handler receipt test; no old test/FPGA execution."""
import hashlib
import json
from pathlib import Path
import subprocess
import sys

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
IOMMU=ROOT/'cva6/corev_apu/riscv-iommu'
TOOL=ROOT/'out/stage117-real-cache-handoff-20261013/implementation/toolchain/verilator-5.040/bin/verilator'
SOURCES=[IOMMU/'vendor/common_cells/src/cf_math_pkg.sv',IOMMU/'vendor/axi/src/axi_pkg.sv',
         IOMMU/'packages/rv_iommu_cfg_pkg.sv',IOMMU/'packages/rv_iommu_reg_pkg.sv',IOMMU/'packages/rv_iommu_pkg.sv',
         IOMMU/'rtl/software_interface/rv_iommu_cq_handler.sv',HERE/'cq_receipt_top.sv',HERE/'cq_receipt.cpp']
INCLUDES=[IOMMU/'vendor/axi/include',IOMMU/'vendor/common_cells/include']
obj=HERE/'obj-cq'
obj.mkdir(exist_ok=True)
argv=[str(TOOL),'--cc','--exe','--build','-j','4','-fno-gate','-Wno-fatal',
      '--top-module','cq_receipt_top','--Mdir',str(obj),'-CFLAGS','-O0 -UNDEBUG']
argv += ['-I'+str(path) for path in INCLUDES]+[str(path) for path in SOURCES]
dependencies=SOURCES+[IOMMU/'vendor/axi/include/axi/typedef.svh']
manifest={'schema':'a2-new-cq-functional-build-v1','argv':argv,
          'tool_version':subprocess.run([str(TOOL),'--version'],capture_output=True,text=True,check=True).stdout.strip(),
          'sources':[{'path':str(path),'sha256':hashlib.sha256(path.read_bytes()).hexdigest()} for path in dependencies],
          'parameters':'CQ-only production widths PAddr56/Data64/Addr64/ID4; ring8; no source modification',
          'old_experiments_executed':False,'fpga':False,'timing_claim':False}
manifest['test_drivers']=[{'path':str(HERE/name),'sha256':hashlib.sha256((HERE/name).read_bytes()).hexdigest()} for name in ('run_cq.py','check_cq.py')]
manifest['tool_sha256']=hashlib.sha256(TOOL.read_bytes()).hexdigest()
binpath=TOOL.with_name('verilator_bin')
if binpath.exists(): manifest['tool_binary_sha256']=hashlib.sha256(binpath.read_bytes()).hexdigest()
(HERE/'cq-build-inputs.json').write_text(json.dumps(manifest,indent=2)+'\n')
with (HERE/'cq-build.log').open('w') as output:
    result=subprocess.run(argv,stdout=output,stderr=subprocess.STDOUT)
if result.returncode:
    print((HERE/'cq-build.log').read_text()[-14000:],file=sys.stderr)
    raise SystemExit(result.returncode)
for scenario in ('normal','wrap','b-error','head-only'):
    with (HERE/f'cq-{scenario}.trace.jsonl').open('w') as output, (HERE/f'cq-{scenario}.stderr.log').open('w') as error:
        result=subprocess.run([str(obj/'Vcq_receipt_top'),scenario],stdout=output,stderr=error,timeout=20)
    if result.returncode:
        raise SystemExit(f'{scenario}: rc={result.returncode}; see trace/stderr')
subprocess.run([sys.executable,str(HERE/'check_cq.py')],check=True)
print(json.dumps({'compiled':True,'scenarios_executed':4,'rtl':'rv_iommu_cq_handler.sv','timing_claim':False}))
