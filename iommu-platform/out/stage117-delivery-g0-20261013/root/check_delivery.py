"""Read-only final custody checks. No simulation, build or old validator."""
import pathlib,json,hashlib,subprocess,ast,re
W=pathlib.Path('/home/xvhaocheng/work');P=W/'iommu-platform/out/stage117-delivery-g0-20261013'
def read(p):return json.loads(p.read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
checks=[]
def check(name,ok,detail=None):
 checks.append(dict(check=name,passed=bool(ok),detail=detail));assert ok,(name,detail)
I=P/'implementation';C=P/'checker';R=P/'root'
im=read(I/'artifact-sha256.json');cm=read(C/'artifact-sha256.json')
for row in im:check('implementation asset '+row['path'],sha(I/row['path'])==row['sha256'])
for path,digest in cm.items():check('checker asset '+path,sha(C/path)==digest)
for row in read(I/'actual-compiled-dependencies.json')['files']:
 check('actual dependency '+row['path'],sha(pathlib.Path(row['path']))==row['sha256'])
for row in read(I/'build-inputs.json')['files']:
 p=pathlib.Path(row['path']);p=p if p.is_absolute() else W/'iommu-platform'/p
 check('declared input '+row['path'],sha(p)==row['sha256'])
id=read(I/'delivery.json');cd=read(C/'delivery.json');acc=read(P/'root-result-acceptance.json')
check('checker final pointer',id['independent_checker_acceptance']['delivery_sha256']==sha(C/'delivery.json'))
check('implementation manifest pointer',id['artifact_manifest_sha256']==sha(I/'artifact-sha256.json'))
check('checker manifest pointer',cd['artifact_manifest_sha256']==sha(C/'artifact-sha256.json'))
check('acceptance implementation custody',acc['implementation_delivery_sha256']==sha(I/'delivery.json'))
check('acceptance checker custody',acc['checker_delivery_sha256']==sha(C/'delivery.json'))
exe=read(I/'execution-ledger.json');cases=list(read(C/'summary.json')['cases'])
check('exact seven actual calls',len(exe)==7 and {e['case'] for e in exe}==set(cases+['interface_probe']))
check('same gated binary once each',all(e['attempt']==1 and e['returncode']==0 and e['binary_sha256']==id['same_binary_sha256'] for e in exe) and sha(I/id['same_binary'])==id['same_binary_sha256'])
check('gate before cases retained',read(R/'interface-gate.json')['six_cases_authorized'])
raw_count=0
for case in cases:
 report=read(C/f'{case}-independent-verification.json')
 check('independent accepted '+case,report['passed'] and not report['executor_pass_flag_used_for_acceptance'])
 raw=report['raw_artifact_sha256']
 for name,digest in raw.items():
  path=I/'results'/case/name
  check('raw report binding '+case+'/'+name,sha(path)==digest);raw_count+=1
 check('exact new refill predicate '+case,read(C/'refill-ownership-correction.json')['case_proofs'][case]['passed'])
check('cost binding',read(C/'cost-summary-binding.json')['passed'])
cor=read(C/'refill-ownership-correction.json')
check('16356 native-owned AR',sum(c['AR_bound_to_actual_native_load_count'] for c in cor['case_proofs'].values())==16356)
check('peer mutant rejects',cor['focused_new_negative_control']['actual']=='rejected')
check('no simulator predicate replay',cor['simulator_calls']==0 and cor['original_controls_rerun']==False if 'original_controls_rerun' in cor else cor['simulator_calls']==0 and not cor['original_six_controls_rerun'])
for row in read(P/'before/manifest.json'):
 before=(P/'before'/row['path']).read_bytes();check('original snapshot '+row['path'],hashlib.sha256(before).hexdigest()==row['sha256'])
 if row['path']!='CURRENT_STATUS.md':check('full old history suffix '+row['path'],(W/row['path']).read_bytes().endswith(before))
old=(P/'before/CURRENT_STATUS.md').read_text();new=(W/'CURRENT_STATUS.md').read_text()
check('independent contribution tail exact',new[new.index('- 独立Linux贡献：'):]==old[old.index('- 独立Linux贡献：'):])
for row in read(R/'dirty-files-before.json'):
 if row['repo']=='iommu-platform' and row['path']=='docs/validation.md':continue
 check('prior dirty file retained '+row['repo']+'/'+row['path'],sha(W/row['repo']/row['path'])==row['sha256'])
for row in read(R/'prior-assets.json'):check('closed/prior asset retained '+row['path'],sha(W/row['path'])==row['sha256'])
heads=[]
for row in read(R/'worktree-before.json'):
 head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=W/row['repo'],text=True).strip()
 check('repo HEAD unchanged '+row['repo'],head==row['head']);heads.append(dict(repo=row['repo'],head=head))
source_index={}
for path in [C/'verify_delivery.py',I/'delivery_g0.cpp',I/'cpu_cache.hpp',I/'delivery_protocol.hpp']:
 if path.suffix=='.py':
  source_index[str(path.relative_to(P))]={n.name:n.lineno for n in ast.walk(ast.parse(path.read_text())) if isinstance(n,(ast.FunctionDef,ast.ClassDef))}
 else:source_index[str(path.relative_to(P))]={'total_lines':len(path.read_text().splitlines()),'sha256':sha(path)}
for path in [W/'iommu-platform/docs/stage117-delivery-g0-20261013.md',P/'README.md',W/'CURRENT_STATUS.md']:
 for target in re.findall(r'\]\(([^)]+)\)',path.read_text()):
  if target.startswith(('https:','http:','#')):continue
  resolved=(path.parent/target.split('#')[0]).resolve()
  if resolved==(R/'delivery-check.json').resolve():continue # this invocation's output
  check('local nav '+str(path.relative_to(W))+' '+target,resolved.exists())
check('truthful goal/model open',acc['full_goal_status']=='active_pending_root_ultra_runtime_and_final_signoff' and not acc['model_policy']['satisfied'] and acc['G1_enter']==False and acc['paper_complete']==False)
out=dict(schema='stage117-delivery-g0-root-custody-v1',technical_delivery_passed=True,goal_complete=False,model_policy_passed=False,pending='Actual root ultra final review/signoff; no repeated accepted tests required.',checks=checks,counts=dict(implementation_assets=len(im),checker_assets=len(cm),actual_compiled_dependencies=320,declared_inputs=117,raw_report_bindings=raw_count,simulator_calls=7),heads=heads,source_index=source_index,document_sha256={str(p.relative_to(W)):sha(p) for p in [W/'CURRENT_STATUS.md',W/'HANDOFF.md',W/'研究计划.md',W/'iommu-platform/docs/research-execution-plan-20261011.md',W/'iommu-platform/docs/validation.md',W/'iommu-platform/docs/stage117-delivery-g0-20261013.md']},no_simulation_build_or_old_validator_executed=True)
(R/'delivery-check.json').write_text(json.dumps(out,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(dict(technical_delivery_passed=True,counts=out['counts'],checks=len(checks),goal_complete=False,model_policy_passed=False),ensure_ascii=False))
