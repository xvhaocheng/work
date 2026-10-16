from pathlib import Path
from datetime import datetime,timezone
import subprocess,json,sys
root=Path('/home/xvhaocheng/work/iommu-platform')
b=root/'out/paper-route-review-20261004'
state={'started_at':datetime.now(timezone.utc).isoformat(),'profile':'migration-4dma-composition','status':'running','source_overlay':'out/paper-route-review-20261004/workspace-overlay.json'}
statefile=b/'fpga-build-state.json';statefile.write_text(json.dumps(state,indent=2)+'\n')
r=subprocess.run([sys.executable,str(root/'scripts/platform.py'),'fpga','--profile',state['profile'],'--workspace-overlay',str(b/'workspace-overlay.json')],cwd=root)
state['exit_code']=r.returncode;state['status']='complete' if r.returncode==0 else 'failed';state['finished_at']=datetime.now(timezone.utc).isoformat()
statefile.write_text(json.dumps(state,indent=2)+'\n')
raise SystemExit(r.returncode)
