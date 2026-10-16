#!/usr/bin/env python3
import json,pathlib,subprocess,time,resource,hashlib,sys
H=pathlib.Path(__file__).resolve().parent;binary=H/'obj/Vnonblock_actual_dma_system_top'
mode=sys.argv[1];assert mode in ['microtests','single_FIFO_ahead','parallel_FIFO_ahead']
ledger=H/'execution-ledger.json';old=json.loads(ledger.read_text()) if ledger.exists() else []
attempt=1+sum(x['case']==mode for x in old);out=H/'results'/mode if attempt==1 else H/'results'/f'{mode}-attempt{attempt}'
out.mkdir(parents=True,exist_ok=False);before=resource.getrusage(resource.RUSAGE_CHILDREN);start=time.monotonic()
with (H/f'raw/{mode}-attempt{attempt}.log').open('w') as f:p=subprocess.run([str(binary),mode,str(out)],stdout=f,stderr=subprocess.STDOUT)
after=resource.getrusage(resource.RUSAGE_CHILDREN)
x={'case':mode,'attempt':attempt,'returncode':p.returncode,'elapsed_wall_seconds':time.monotonic()-start,'user_cpu_seconds':after.ru_utime-before.ru_utime,'system_cpu_seconds':after.ru_stime-before.ru_stime,'peak_child_rss_kib':after.ru_maxrss,'binary_sha256':hashlib.sha256(binary.read_bytes()).hexdigest(),'output':str(out.relative_to(H)),'log':f'raw/{mode}-attempt{attempt}.log'}
old.append(x);ledger.write_text(json.dumps(old,indent=2)+'\n');print(json.dumps(x),flush=True);raise SystemExit(p.returncode)
