from pathlib import Path
import os,signal,select,json,subprocess,time,datetime,hashlib
P=Path(__file__).parent
CLI='/home/xvhaocheng/.local/bin/codex'
def now(): return datetime.datetime.now(datetime.timezone.utc).isoformat()
def save(extra):
 r=json.loads((P/'update-restart-result.json').read_text());r.update(extra)
 temp=P/'update-restart-result.tmp';temp.write_text(json.dumps(r,ensure_ascii=False,indent=2)+'\n');temp.replace(P/'update-restart-result.json')
def event(name,**kw):
 with (P/'update-restart-events.jsonl').open('a') as f:f.write(json.dumps({'at_utc':now(),'event':name,**kw},ensure_ascii=False)+'\n')
def version():
 v=subprocess.run([CLI,'app-server','daemon','version'],capture_output=True,text=True,timeout=15)
 if v.returncode: return {'read_error':'version command exit '+str(v.returncode)}
 return json.loads(v.stdout)
try:
 req=json.loads((P/'graceful-restart-request.json').read_text());pid=req['old_pid'];proc=Path('/proc')/str(pid)
 fd=os.pidfd_open(pid)
 if str(proc.joinpath('exe').resolve())!=req['expected_old_executable']:raise RuntimeError('Old process executable changed; no signal sent')
 if proc.joinpath('stat').read_text().rsplit(')',1)[1].split()[19]!=req['expected_start_ticks']:raise RuntimeError('Old process identity changed; no signal sent')
 args=[v.decode() for v in proc.joinpath('cmdline').read_bytes().split(b'\0') if v]
 if args!=req['original_args']:raise RuntimeError('Startup arguments changed; no signal sent')
 environment={}
 for item in proc.joinpath('environ').read_bytes().split(b'\0'):
  if b'=' in item:
   k,v=item.split(b'=',1);environment[k.decode()]=v.decode()
 cwd=str(proc.joinpath('cwd').resolve())
 preflight=subprocess.run([CLI,'--version'],capture_output=True,text=True,timeout=15).stdout.strip()
 if preflight!='codex-cli 0.162.1':raise RuntimeError('Unexpected updated CLI version; no signal sent')
 signal.pidfd_send_signal(fd,signal.SIGHUP)
 save({'state':'waiting-for-running-turns-to-finish','restart_method':'native SIGHUP graceful-only drain; relaunch original arguments/environment with updated executable','graceful_restart_requested_at_utc':now(),'restart_worker_pid':os.getpid(),'force_signal_sent':False})
 event('graceful_shutdown_requested',old_pid=pid,signal='SIGHUP',new_cli_version=preflight)
 select.select([fd],[],[])
 os.close(fd)
 event('old_app_server_exited',old_pid=pid)
 save({'state':'starting-updated-app-server','old_app_server_exited_at_utc':now()})
 current=version()
 launched=None
 if current.get('status')!='running':
  with (P/'restarted-app-server.log').open('a') as f:
   launched=subprocess.Popen([CLI,*args[1:]],cwd=cwd,env=environment,stdin=subprocess.DEVNULL,stdout=f,stderr=f,start_new_session=True)
  event('updated_app_server_started',pid=launched.pid,args=[CLI,*args[1:]],cwd=cwd)
  time.sleep(2)
  current=version()
 actual_pid=launched.pid if launched and launched.poll() is None else None
 if current.get('status')=='running' and current.get('appServerVersion')=='0.162.1':
  extra={'state':'restart-confirmed','versions_after_restart':current,'restart_confirmed_at_utc':now(),'new_process_pid':actual_pid,'config_sha256_after_restart':hashlib.sha256(Path('/home/xvhaocheng/.codex/config.toml').read_bytes()).hexdigest(),'context_after_restart':'未验实：需目标客户端重新连接后读取原生目录及新turn运行记录'}
  if actual_pid:extra['new_executable']=str((Path('/proc')/str(actual_pid)/'exe').resolve())
  save(extra);event('restart_confirmed',versions=current,pid=actual_pid)
 else:
  save({'state':'restart-not-confirmed','versions_after_restart':current,'restart_check_at_utc':now(),'launched_exit_code':launched.poll() if launched else None})
  event('restart_not_confirmed',versions=current)
except BaseException as e:
 save({'state':'graceful-restart-error','restart_error':type(e).__name__+': '+str(e),'restart_error_at_utc':now()})
 event('restart_error',error=type(e).__name__+': '+str(e))
