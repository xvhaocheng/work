from pathlib import Path
import importlib.util,json,os,subprocess,sys
from datetime import datetime,timezone
root=Path('/home/xvhaocheng/work/iommu-platform')
b=root/'out/paper-route-review-20261004'
s=importlib.util.spec_from_file_location('cpn_platform',root/'scripts/platform.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
m.ENV['LOCALVERSION']='-cpn1'
state={'started_at':datetime.now(timezone.utc).isoformat(),'source_mode':'authorized_working_tree','sd_update_required':True,'localversion':'-cpn1','status':'running'}
state['linux_base_commit']=m.capture(['git','rev-parse','HEAD'],root/'linux')
state['inputs']={p:m.sha(root/p) for p in ['linux/drivers/misc/idma.c','linux/drivers/misc/idma_research.h','linux/include/uapi/linux/idma.h','bench/idma_composition.c','scripts/rootfs.py','configs/linux_defconfig','configs/platform.dts']}
(b/'software-build-state.json').write_text(json.dumps(state,indent=2)+'\n')
try:
 m.kernel(12)
 m.firmware(12)
 state['kernel_release']=(root/'out/linux/include/config/kernel.release').read_text().strip()
 state['artifacts']={p:(root/p).stat().st_size for p in ['out/linux/arch/riscv/boot/Image','out/linux/platform.dtb','out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin','out/rootfs.cpio','out/bench/idma_composition']}
 assert all(state['artifacts'].values())
 state['status']='complete';state['exit_code']=0
except Exception as e:
 state['status']='failed';state['exit_code']=1;state['error']=str(e)
 raise
finally:
 state['finished_at']=datetime.now(timezone.utc).isoformat()
 (b/'software-build-state.json').write_text(json.dumps(state,indent=2)+'\n')
