import subprocess,sys,json,datetime,pathlib,hashlib,time
base=pathlib.Path(__file__).resolve().parent
name=sys.argv[1];command=sys.argv[2:];start=datetime.datetime.now(datetime.timezone.utc).isoformat();t=time.monotonic()
with (base/'receipts'/f'{name}.log').open('w') as log:
 try:
  result=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,timeout=180);rc=result.returncode;timeout=False
 except subprocess.TimeoutExpired:
  rc=124;timeout=True
 except OSError as exc:
  log.write(repr(exc)+'\n');rc=127;timeout=False
receipt={'name':name,'argv':command,'cwd':str(pathlib.Path.cwd()),'start_utc':start,'elapsed_seconds':time.monotonic()-t,'exit_code':rc,'timeout_180s':timeout,'log_sha256':hashlib.sha256((base/'receipts'/f'{name}.log').read_bytes()).hexdigest()}
(base/'receipts'/f'{name}.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt));sys.exit(rc)
