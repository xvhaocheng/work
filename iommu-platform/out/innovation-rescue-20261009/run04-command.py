import importlib.util,json,sys
from pathlib import Path
p=Path("iommu-platform/out/innovation-rescue-20261009/open_copy_model.py"); spec=importlib.util.spec_from_file_location("s49_focus",p); m=importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
sc=[]; checks=[]
for strategy in ("open_long_read","ordinary_tile_exit"):
 for kind in ("rready","awready"):
  cfg={"strategy":strategy,"N":16,"B":3,"q":2,"pause_P":0,"resume_at":6,"case":"deferred_resume_"+kind}
  kwargs={"r_ready_stall":{"A":(2,8)}} if kind=="rready" else {"aw_ready_stall":{"A":(0,8)}}
  s=m.Sim(**{k:cfg[k] for k in ("strategy","N","B","q","pause_P","resume_at")},**kwargs); s.run(); ev=m.eval_scenario(s,cfg)
  exits=[t for t,k,d in s.event_log if k=="EXIT_A"]
  holds=[t for t,k,d in s.event_log if k=="AW" and d["flow"]=="V" and d["len"]>s.q]
  c={"id":ev["id"]+":deferred_resume_was_exercised","ok":bool(exits and holds and holds[0]<s.resume_t<exits[0]),"detail":{"resume_t":s.resume_t,"first_v_long_aw":holds[:1],"exit_t":exits[:1]}}
  ev["checks"].append(c); ev["events"]=s.event_log; ev["config_with_ready_stalls"]={**cfg,**kwargs}; sc.append(ev); checks.extend(ev["checks"])
f=[c for c in checks if not c["ok"]]; summary={"scenario_count":len(sc),"check_count":len(checks),"failed_checks":len(f),"exit_code":int(bool(f))}
r={"scope":"focused real Sim deferred resume, not RTL or platform cycles","scenarios":sc,"checks":checks,"run_summary":summary}; Path("iommu-platform/out/innovation-rescue-20261009/result-run04.json").write_text(json.dumps(r,indent=2)+"\n"); print(json.dumps(summary)); print(json.dumps(f[:3])); sys.exit(bool(f))
