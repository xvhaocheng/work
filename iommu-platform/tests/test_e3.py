#!/usr/bin/env python3
from pathlib import Path
import subprocess

root=Path(__file__).resolve().parents[1]
out=root/'out/e3-followup-20260918'
out.mkdir(exist_ok=True,parents=True)
rtl=root/'cva6/corev_apu/riscv-iommu'
def build(name,files,extra=()):
    dest=out/name;dest.mkdir(exist_ok=True)
    cmd=['verilator','--cc','--exe','--build','-Wno-fatal','--top-module',name,'--Mdir',str(dest),*extra,*map(str,files)]
    with (dest/'build.log').open('w') as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
    subprocess.run([str(dest/('V'+name))],check=True)
build('e3_delay_top',[rtl/'rtl/translation_logic/rv_iommu_e3_delay.sv',root/'tests/e3_delay_top.sv',root/'tests/e3_delay_test.cpp'])
fpga=root/'cva6/corev_apu/fpga/src'
build('dma_e3_write_observer',[fpga/'dma_e3_write_observer.sv',root/'tests/e3_write_test.cpp'])
build('e3_observer_top',[rtl/'packages/rv_iommu_pkg.sv',rtl/'rtl/transaction_controller/rv_iommu_e3_queue.sv',fpga/'dma_path_observer.sv',fpga/'dma_e3_write_observer.sv',fpga/'dma_e3_observer.sv',root/'tests/e3_observer_top.sv',root/'tests/e3_observer_test.cpp'])
build('dma_observer',[fpga/'dma_path_observer.sv',fpga/'dma_observer.sv',root/'tests/e3_control_test.cpp'],['-GPathEnable=1','-GE3Enable=1'])
