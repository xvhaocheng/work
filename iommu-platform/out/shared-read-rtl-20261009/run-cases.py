import json, pathlib, subprocess, time
p = pathlib.Path(__file__).resolve().parent
base = json.loads((p/'run04-argv.json').read_text())
cases = [
 ('run05', 'tile-no-pause', 2, []),
 ('run06', 'open-pause', 1, ['+pause-mode=2','+pause-offers=4','+pause-window=10000']),
 ('run07', 'tile-pause', 2, ['+pause-mode=2','+pause-offers=4','+pause-window=10000']),
 ('run08', 'open-early-resume-delayed-B-backpressure', 1,
  ['+pause-mode=2','+pause-offers=4','+pause-window=10','+pause-require-quiet=0',
   '+memory-b-delay=64','+memory-backpressure=1']),
 ('run09', 'tile-early-resume-delayed-B-backpressure', 2,
  ['+pause-mode=2','+pause-offers=4','+pause-window=10','+pause-require-quiet=0',
   '+memory-b-delay=64','+memory-backpressure=1']),
 ('run10', 'closed-pause', 0, ['+pause-mode=2','+pause-offers=4','+pause-window=10000']),
 ('run11', 'open-A-first', 1, ['+natural-first=a']),
]
for name, label, mode, extra in cases:
    argv = [x for x in base if not x.startswith(('+physical-bypass=','+natural-trace='))]
    if any(x.startswith('+natural-first=') for x in extra):
        argv = [x for x in argv if not x.startswith('+natural-first=')]
    argv += [f'+physical-bypass={mode}',f'+natural-trace={p/name}.csv']+extra
    (p/f'{name}-argv.json').write_text(json.dumps(argv,indent=2)+'\n')
    start=time.time()
    with (p/f'{name}.log').open('w') as log:
        result=subprocess.run(argv,stdout=log,stderr=subprocess.STDOUT,timeout=30)
    (p/f'{name}-result.json').write_text(json.dumps({
        'label':label,'exit_code':result.returncode,'elapsed_seconds':time.time()-start})+'\n')
    print(name,label,'exit',result.returncode,flush=True)
