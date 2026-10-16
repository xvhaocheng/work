import difflib,json,pathlib,sys,re
root=pathlib.Path(__file__).resolve().parents[2]
base=root/'out/p4m-baseline-20260920'
name=sys.argv[1]; data=json.loads((base/f'deepseek-{name}.json').read_text())
print(json.dumps({k:data.get(k) for k in ('model','usage','truncated','error_code','finish_reason')},ensure_ascii=False))
assert not data.get('truncated'), 'truncated proposal'
answer=data.get('answer',''); (base/f'deepseek-{name}-answer.txt').write_text(answer)
if answer.startswith('```'): answer=answer[answer.index('\n')+1:answer.rfind('```')]
p=json.loads(answer)
original={}; changed={}
for edit in p.get('edits',[]):
    path=edit['path']; assert not pathlib.Path(path).is_absolute() and '..' not in pathlib.Path(path).parts
    if path not in original: original[path]=(root/path).read_text();changed[path]=original[path]
    old=edit['old']
    if old and changed[path].count(old)==0:
        pattern=r'\s+'.join(re.escape(t) for t in re.split(r'\s+',old.strip()))
        matches=list(re.finditer(pattern,changed[path]))
        if len(matches)==1:
            m=matches[0]; old=changed[path][m.start():m.end()]; print('whitespace match:',path)
            edit['new']=edit['new'].strip()
    assert old and changed[path].count(old)==1,(path,'nonunique/missing match',old[:80])
    changed[path]=changed[path].replace(old,edit['new'],1)
for f in p.get('files',[]):
    if isinstance(f,str): continue
    path=f['path']; assert not (root/path).exists(),('not new file',path)
    assert not pathlib.Path(path).is_absolute() and '..' not in pathlib.Path(path).parts
    original[path]=''; changed[path]=f['content']
patch=[]
for path,new in changed.items():
    dest=base/f'{name}-proposed'/path;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_text(new)
    patch.extend(difflib.unified_diff(original[path].splitlines(True),new.splitlines(True),fromfile='a/'+path if original[path] else '/dev/null',tofile='b/'+path))
(base/f'{name}.patch').write_text(''.join(patch))
(base/f'{name}-proposal.json').write_text(json.dumps(p,ensure_ascii=False,indent=2))
print(p.get('summary'));print('files:',list(changed));print('checks:',p.get('checks'));print('risks:',p.get('risks'))
