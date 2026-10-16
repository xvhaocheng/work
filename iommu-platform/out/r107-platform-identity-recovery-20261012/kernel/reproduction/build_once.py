import datetime, hashlib, json, os, pathlib, shutil, stat, subprocess, tarfile, time
R=pathlib.Path('/home/xvhaocheng/work/iommu-platform')
B=R/'out/r107-platform-identity-recovery-20261012/kernel/reproduction'
C=json.loads((B/'reproduction-contract.json').read_text())
S=B/'linux-source'; O=B/'linux-output'; F=B/'frozen-inputs'
start_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(); start=time.monotonic()
ledger=[]
def utc(): return datetime.datetime.now(datetime.timezone.utc).isoformat()
def sha(p):
    d=hashlib.sha256()
    with pathlib.Path(p).open('rb') as f:
        for chunk in iter(lambda:f.read(1024*1024),b''): d.update(chunk)
    return d.hexdigest()
def save(name,obj): (B/name).write_text(json.dumps(obj,indent=2,ensure_ascii=False)+'\n')
def cap(tag,argv,cwd):
    t=utc(); st=time.monotonic(); p=subprocess.run(argv,cwd=cwd,capture_output=True)
    (B/(tag+'.stdout')).write_bytes(p.stdout); (B/(tag+'.stderr')).write_bytes(p.stderr)
    ledger.append({'tag':tag,'argv':argv,'cwd':str(cwd),'started_at_utc':t,'finished_at_utc':utc(),'elapsed_seconds':time.monotonic()-st,'exit_code':p.returncode,'stdout_sha256':sha(B/(tag+'.stdout')),'stderr_sha256':sha(B/(tag+'.stderr'))})
    if p.returncode: raise RuntimeError(f'{tag} exit {p.returncode}')
    return p.stdout
old_paths=[x['path'] for x in json.loads((B.parent/'audited-file-hashes.json').read_text())['files'] if x.get('exists')]
old_paths.extend(['out/linux/.version','out/linux/include/generated/utsrelease.h','out/linux/drivers/net/ethernet/lowrisc/lowrisc_ethernet.o'])
before={x:sha(R/x) for x in sorted(set(old_paths))}
save('original-inputs-before.json',{'observed_at_utc':utc(),'sha256':before})
state={'intent':C['intent'],'actual_started_at_utc':start_utc,'status':'freezing','build_attempts':0,'device_actions':0,'ordinary_adapter_added':False}
save('execution-state.json',state)
try:
    head=cap('original-head-before',['git','rev-parse','HEAD'],R/'linux').decode().strip()
    if head!=C['freeze']['commit']:raise RuntimeError('production HEAD changed')
    status_before=cap('original-status-before',['git','--no-optional-locks','status','--short'],R/'linux')
    dirty=cap('original-dirty-names',['git','diff','--name-only','HEAD'],R/'linux').decode().splitlines()
    if set(dirty)!=set(C['freeze']['tracked_overlay']):raise RuntimeError('unexpected tracked overlay scope')
    untracked=cap('untracked-lowrisc',['git','ls-files','--others','--exclude-standard','-z','--','drivers/net/ethernet/lowrisc'],R/'linux').decode().split('\0')
    untracked=[x for x in untracked if x]
    patch=cap('tracked-overlay-binary-diff',['git','diff','--binary','HEAD','--',*C['freeze']['tracked_overlay']],R/'linux')
    archive=B/'linux-base-c0f5e13.tar'
    cap('base-git-archive',['git','archive','--format=tar','--output='+str(archive),head],R/'linux')
    S.mkdir(exist_ok=False); O.mkdir(exist_ok=False); F.mkdir(exist_ok=False)
    with tarfile.open(archive,'r:') as t:
        for m in t.getmembers():
            if pathlib.PurePosixPath(m.name).is_absolute() or '..' in pathlib.PurePosixPath(m.name).parts: raise RuntimeError('unsafe archive member')
        t.extractall(S)
    overlays=[]
    for name in C['freeze']['tracked_overlay']+untracked:
        src=R/'linux'/name; dst=S/name; dst.parent.mkdir(parents=True,exist_ok=True)
        shutil.copy2(src,dst)
        if sha(src)!=sha(dst):raise RuntimeError('overlay copy mismatch')
        overlays.append({'path':name,'kind':'tracked_modified' if name in C['freeze']['tracked_overlay'] else 'untracked','sha256':sha(src),'bytes':src.stat().st_size,'mode':oct(stat.S_IMODE(src.stat().st_mode))})
    shutil.copy2(R/'out/rootfs.cpio',F/'rootfs.cpio')
    shutil.copy2(B.parent/'Image-embedded.config',F/'original-embedded.config')
    shutil.copy2(F/'original-embedded.config',O/'.config')
    if sha(F/'rootfs.cpio')!=before['out/rootfs.cpio']:raise RuntimeError('rootfs changed during copy')
    source=[]
    for dirpath,dirs,files in os.walk(S):
        dirs.sort(); files.sort()
        for name in dirs+files:
            p=pathlib.Path(dirpath)/name
            if p.is_symlink():source.append({'path':str(p.relative_to(S)),'kind':'symlink','target':os.readlink(p)});continue
            if p.is_file():source.append({'path':str(p.relative_to(S)),'kind':'file','sha256':sha(p),'bytes':p.stat().st_size,'mode':oct(stat.S_IMODE(p.stat().st_mode))})
    source.sort(key=lambda x:x['path']);save('frozen-source-manifest.json',{'observed_at_utc':utc(),'base_commit':head,'base_archive_sha256':sha(archive),'tracked_patch_sha256':sha(B/'tracked-overlay-binary-diff.stdout'),'overlays':overlays,'files':source})
    manifest_hash=sha(B/'frozen-source-manifest.json')
    env=os.environ.copy(); dropped=[]
    names=['MAKEFLAGS','MFLAGS','GNUMAKEFLAGS','KBUILD_OUTPUT','KBUILD_EXTMOD','KCONFIG_CONFIG','KCFLAGS','KCPPFLAGS','KAFLAGS','CFLAGS_KERNEL','AFLAGS_KERNEL','CFLAGS_MODULE','LDFLAGS','HOSTCFLAGS','HOSTCXXFLAGS','HOSTLDFLAGS','SOURCE_DATE_EPOCH','CC','LD','AR','AS','NM','STRIP','OBJCOPY','OBJDUMP','HOSTCC','HOSTCXX','LLVM','LLVM_IAS','LOCALVERSION','KBUILD_BUILD_VERSION','KBUILD_BUILD_TIMESTAMP','KBUILD_BUILD_USER','KBUILD_BUILD_HOST']
    for n in names:
        if n in env:dropped.append(n);del env[n]
    env.update(C['specified_reproduction_environment'])
    state.update({'status':'building','freeze_finished_at_utc':utc(),'freeze_elapsed_seconds':time.monotonic()-start,'frozen_source_files':len(source),'source_manifest_sha256':manifest_hash,'base_archive_sha256':sha(archive),'original_rootfs_sha256':before['out/rootfs.cpio'],'frozen_rootfs_sha256':sha(F/'rootfs.cpio'),'initial_config_sha256':sha(O/'.config'),'specified_reproduction_environment':C['specified_reproduction_environment'],'cleared_inherited_build_variable_names':dropped,'command':C['command'],'build_attempts':1,'build_started_at_utc':utc()})
    save('execution-state.json',state);save('command-ledger.json',ledger)
    print(json.dumps({'state':'building','source_files':len(source),'source_manifest_sha256':manifest_hash,'freeze_seconds':state['freeze_elapsed_seconds'],'command':C['command']},indent=2),flush=True)
    build_start=time.monotonic()
    with (B/'build.stdout.log').open('wb') as out,(B/'build.stderr.log').open('wb') as err:
        p=subprocess.run(C['command'],cwd=R,env=env,stdout=out,stderr=err)
    build_elapsed=time.monotonic()-build_start
    ledger.append({'tag':'clean-Image-build-attempt1','argv':C['command'],'cwd':str(R),'started_at_utc':state['build_started_at_utc'],'finished_at_utc':utc(),'elapsed_seconds':build_elapsed,'exit_code':p.returncode,'stdout_path':str(B/'build.stdout.log'),'stderr_path':str(B/'build.stderr.log'),'stdout_sha256':sha(B/'build.stdout.log'),'stderr_sha256':sha(B/'build.stderr.log')})
    state.update({'build_finished_at_utc':utc(),'build_elapsed_seconds':build_elapsed,'exit_code':p.returncode,'status':'build_complete' if p.returncode==0 else 'build_failed'})
    if p.returncode==0:
        artifacts={}
        for name in ['arch/riscv/boot/Image','vmlinux','.config','include/config/kernel.release','include/generated/compile.h','include/generated/utsversion.h','include/generated/utsrelease.h']:
            path=O/name;artifacts[name]={'sha256':sha(path),'bytes':path.stat().st_size}
        state['artifacts']=artifacts;state['Image_sha256_matches_old']=artifacts['arch/riscv/boot/Image']['sha256']==before['out/linux/arch/riscv/boot/Image'];state['ELF_sha256_matches_old']=artifacts['vmlinux']['sha256']==before['out/linux/vmlinux'];state['config_exact_equal']=artifacts['.config']['sha256']==before['out/linux/.config']
except BaseException as e:
    state.update({'status':'freeze_or_execution_failed','error':repr(e)})
    raise
finally:
    after={x:sha(R/x) if (R/x).is_file() else None for x in before}
    save('original-inputs-after.json',{'observed_at_utc':utc(),'sha256':after,'changed':[x for x in before if before[x]!=after[x]]})
    cap('original-status-after',['git','--no-optional-locks','status','--short'],R/'linux');cap('original-head-after',['git','rev-parse','HEAD'],R/'linux')
    state.update({'actual_finished_at_utc':utc(),'total_elapsed_seconds':time.monotonic()-start,'original_input_hashes_unchanged':before==after,'original_source_git_status_unchanged':(B/'original-status-before.stdout').read_bytes()==(B/'original-status-after.stdout').read_bytes()})
    save('command-ledger.json',ledger);save('execution-state.json',state);print(json.dumps(state,indent=2),flush=True)
