#!/usr/bin/env python3
"""Compile/include the actual driver with host-only boundaries. No DMA execution."""
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
STUB=HERE/'include'
for header in ('linux/cpu.h','linux/delay.h','linux/highmem.h','linux/overflow.h',
               'linux/pgtable.h','linux/riscv_iommu_research.h','linux/sched/mm.h','asm/csr.h'):
    path=STUB/header
    path.parent.mkdir(parents=True,exist_ok=True)
    path.write_text('/* Host-only boundary types/functions are in harness.c. */\n')
path=STUB/'uapi/linux/idma_sva.h'
path.parent.mkdir(parents=True,exist_ok=True)
shutil.copyfile(ROOT/'linux/include/uapi/linux/idma_sva.h',path)
source=ROOT/'linux/drivers/misc/idma_sva_task.h'
sha=hashlib.sha256(source.read_bytes()).hexdigest()
(HERE/'idma_sva_task.actual-source.h').write_bytes(source.read_bytes())
command=['gcc','-std=gnu11','-O0','-Wall','-Wextra','-Wno-unused-function',
         '-Wno-misleading-indentation','-I',str(STUB),'-I',str(ROOT/'linux'),
         str(HERE/'harness.c'),'-o',str(HERE/'harness')]
build=subprocess.run(command,text=True,capture_output=True)
(HERE/'build.log').write_text(build.stdout+build.stderr)
if build.returncode:
    raise SystemExit(build.stderr)
cases=[('success',1),('success',2),('success',3),('partial-pin',3),('map',2),
       ('claim',3),('attach',3),('initial-barrier',3),('next-zero',3),('timeout',3),
       ('restore',3),('restore-barrier',3),('unmap',2),('release',3),('a-clear',1),('a-clear',2),('a-clear',3)]
records=[]
for scenario,mode in cases:
    result=subprocess.run([str(HERE/'harness'),scenario,str(mode)],text=True,capture_output=True,timeout=5)
    (HERE/f'{scenario}-{mode}.stdout.json').write_text(result.stdout)
    (HERE/f'{scenario}-{mode}.stderr.log').write_text(result.stderr)
    if result.returncode:
        raise SystemExit(f'{scenario}/{mode}: {result.returncode}: {result.stderr}')
    records.append(json.loads(result.stdout))
report={'schema':'a2-real-driver-host-lifecycle-v1',
        'classification':'actual driver C flow; Linux/MMIO/IOMMU stubbed; NOT hardware/kernel execution',
        'driver_source':str(source),'driver_sha256':sha,'hardware_evidence':False,
        'retain_capture':'longjmp only captures the real retain-loop scheduler boundary; no abort or recovery proven',
        'cases_passed':len(records),'cases':records}
(HERE/'results.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'driver_sha256':sha,'cases_passed':len(records),'hardware_evidence':False}))
