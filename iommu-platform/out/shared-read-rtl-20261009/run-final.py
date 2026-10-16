import json, pathlib, subprocess, time
p=pathlib.Path(__file__).resolve().parent
cases=[('run12','run08'),('run13','run09'),('run14','run04'),('run15','run05'),
       ('run16','run06'),('run17','run07'),('run18','run10'),('run19','run11')]
for name, old in cases:
    argv=[x.replace(old+'.csv',name+'.csv') for x in json.loads((p/(old+'-argv.json')).read_text())]
    (p/(name+'-argv.json')).write_text(json.dumps(argv,indent=2)+'\n')
    start=time.time()
    with (p/(name+'.log')).open('w') as log:
        result=subprocess.run(argv,stdout=log,stderr=subprocess.STDOUT,timeout=30)
    (p/(name+'-result.json')).write_text(json.dumps({
        'source_case':old,'exit_code':result.returncode,'elapsed_seconds':time.time()-start})+'\n')
    print(name,old,'exit',result.returncode,flush=True)
