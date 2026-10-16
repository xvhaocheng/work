from pathlib import Path
import hashlib,json,re,datetime
w=Path('/home/xvhaocheng/work'); b=w/'iommu-platform/out/d-dram-boundary-20261015'; d=b/'documentation'
rel='iommu-platform/out/d-dram-boundary-20261015'
paths=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md']
# Re-read immediately before the only canonical write; capture these exact bytes.
old={p:(w/p).read_bytes() for p in paths}
snap=d/'before-write'; snap.mkdir(exist_ok=True)
for p,v in old.items():
 dest=snap/p;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(v)
sha=lambda x:hashlib.sha256(x).hexdigest()
(snap/'manifest.json').write_text(json.dumps({p:{'bytes':len(v),'sha256':sha(v)} for p,v in old.items()},ensure_ascii=False,indent=2)+'\n')
text={p:v.decode() for p,v in old.items()}
links=f'[本轮入口]({rel}/README.md)／[root裁决]({rel}/root/decision-analysis.md)／[稳定接受]({rel}/root/root-result-acceptance.json)／[下一最小合同]({rel}/root/next-minimal-contract.json)'
result='DRAM持续状态来源关已过：固定Ramulator2实现中，普通读前史在严格整模型排空与idle后保留开行/cap；独立bank warm保留、同bank冲突warm改写，cap4关行和完整全rank刷新在配对控制中洗去旧影响。9个冻结run（8私有＋1原版control）、25次读全部回调；105份全状态快照、50条官方命令核账通过，同O_phys重放和原版passivity全等，4种负变异均拒绝。仅接受物理读入口的来源证据，未证明额外信息、P/PN收益或原生性能。'
nextstep='下一仅D-DRAM-ACTION-CONTRACT-1：核所选任务完整quiet/config/clear/warm、实际物理轨迹及write完成域，再决定是否准入有限P/PN完整动作比较；合同尚未执行。不扩大独立状态矩阵或collector。本轮少量读warm不代表M0 warm256通过，WR issue不等于COPY/AXI B/应用完成。'
legacy='旧MQSim四冻结输入私有修复后完整完成、四个完整排空边界活动GC均0；本版cache-off有限覆写残留停止扩测保持，不否定整个D。[旧轮入口](iommu-platform/out/d-mqsim-identity-20261015/README.md)。'
execution='本轮原版/私有库各1次构建、2个driver最终编译成功；2次driver编译/链接失败及1次缺cmake前置失败日志保留。9个冻结模型run，0旧M0/CS1/MQSim实验、原生DMA、生产Linux/RTL/ABI修改、collector/FPGA或板操作。原MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。共享研究Codex-only；文档请求Sol/medium，实际模型/强度元数据unknown，不以请求参数冒充实际运行证明。'
s=text['CURRENT_STATUS.md'];s=s.replace('# 当前状态：D完成门已过，本版GC残留停止扩测','# 当前状态：DRAM来源关已过，下一核完整准备与动作合同',1)
a=s.index('- 一次身份重放');z=s.index('\n- I有条件',a)
s=s[:a]+'- '+result+nextstep+' '+links+'\n- '+legacy+s[z:]
a=s.index('## 硬件与执行状态\n');z=s.index('## 独立工程事项',a)
s=s[:a]+'## 硬件与执行状态\n\n'+execution+'\n\n'+s[z:];text['CURRENT_STATUS.md']=s
s=text['研究计划.md'];s=s.replace('CS1局部停止保持；MQSim四冻结输入完成门已过，本版GC残留停止扩测；先核DRAM行/刷新在显式warm后的保留/洗去，固定M0不扩大','CS1/MQSim本版局部停止保持；DRAM来源关已过，下一核完整准备、write完成域及有限动作准入，固定M0不扩大',1)
a=s.index('当前实际结果：');z=s.index('\n## D：',a)
s=s[:a]+'当前实际结果：'+result+' '+nextstep+' '+legacy+' 本轮2库构建和2个driver最终成功，2次driver失败及缺cmake前置失败记录保留；没有旧实验、生产或硬件操作。D主线、I轻量not_ready、A5/A2和10月15—28日窗口不变。'+links+'。\n'+s[z:]
a=s.index('持续服务来源关保留SSD/DRAM来源事实；');z=s.index('\n## I：',a)
s=s[:a]+'持续服务来源关保留SSD/DRAM来源事实；'+legacy+' '+result+' '+nextstep+' 完整O_phys回放只证明固定配置/epoch及完整物理入口历史下的本轮确定性，不证明实际O_host已足够或缺信息。若完整准备洗去状态、write完成域无法成立或同动作在两态都胜出，停止相应观察器版本；若准入，先看零价格全状态上界相对最佳固定动作，再讨论O0/新增Z与全部费用。常驻池、合法大映射/准备、固定动作、完整O0、队深/poll、全费用等待/试选仍是普通对照，付费Z同样交给普通规则。不扩大固定M0、不调gap、不加collector或改guard。'+links+'。\n'+s[z:];text['研究计划.md']=s
s=text['研究导航.md'];a=s.index('| D：有限观测');z=s.index('\n| I：',a)
s=s[:a]+f'| D：有限观测→完整控制选择 | 主线；DRAM严格排空/warm/刷新来源关已过；下一核完整准备与write完成域；MQSim/CS1本版局部停止保持 | {links} |'+s[z:]
a=s.index('MQSim四原冻结输入');z=s.index('\n## 已有成果怎样复用',a)
s=s[:a]+result+' '+nextstep+' '+legacy+' D主线、I轻量not_ready、A5/A2和10月15—28日窗口不变。\n'+s[z:];text['研究导航.md']=s
for p,prefix in [('HANDOFF.md',rel),('iommu-platform/docs/validation.md','../out/d-dram-boundary-20261015')]:
 note='# 2026年10月15日 D-DRAM来源边界接受与下一动作合同\n\n'+result+'\n\n'+nextstep+'\n\n'+execution+' D主线、I轻量not_ready、A5/A2及10月15—28日立题窗口保持；旧MQSim四边界GC=0的本版局部停止与固定M0/CS1裁决保持。\n\n'+f'[本轮入口]({prefix}/README.md)／[root裁决]({prefix}/root/decision-analysis.md)／[稳定接受]({prefix}/root/root-result-acceptance.json)／[下一最小合同]({prefix}/root/next-minimal-contract.json)／[实际执行账]({prefix}/root/execution-ledger.json)／[文档保护收据]({prefix}/documentation/receipt.json)。本节仅同步稳定接受；以下旧全文逐字节保留，旧文“下一DRAM未执行”是当时记录。\n\n'
 text[p]=note+text[p]
readme='''# D-DRAM-BOUNDARY-1：来源关已过\n\n2026年10月15日。普通读前史在固定Ramulator2来源的严格排空后仍能影响下一读，显式warm可能保留、改写或洗去；这不是额外信息、P/PN收益或原生性能验收。下一仅核完整任务准备及write完成域，再决定有限动作比较。\n\n- [root裁决与结果表](root/decision-analysis.md) · [稳定接受](root/root-result-acceptance.json) · [下一最小合同](root/next-minimal-contract.json)（尚未执行）。\n- [事前合同](root/dram-contract.json) · [预测](root/pre-run-predictions.json) · [输入绑定](root/run-inputs.json) · [完整输入清单](root/input-manifest.json)。\n- [9个实际run](runs/) · [实际执行账](root/execution-ledger.json) · [原始摘录](root/raw-excerpts.json)。8私有＋1原版control，25次读全部回调；不重跑旧实验。\n- [来源接口边界](interface/boundary-card.md) · [源码阅读范围](interface/source-read-ranges.json) · [root源码审查](root/source-review.json) · [私有被动记录补丁](implementation/passive-audit.patch)。\n- [构建与失败日志入口](implementation/build-report.md) · [核账说明](verification/README.md) · [root实际执行的独立核账](verification/root-verified-report.json)。105全状态快照、50官方命令，重放/passivity全等及4负变异拒绝。\n- [本轮文档保护收据](documentation/receipt.json) · [写前五文件快照及hash](documentation/before-write/manifest.json) · [局部新增链接核对](documentation/link-check.json)。\n- [上一MQSim轮](../d-mqsim-identity-20261015/README.md)：四完整排空边界GC=0，本版局部停止保持。\n\n本轮2库构建＋2driver最终成功；2driver编译/链接失败和缺cmake前置失败保留。少量读warm不代表M0 warm256通过；WR issue不代表COPY完成。没有生产/硬件/旧实验改动。D主线、I轻量not_ready、A5/A2、MIG1792023314 paused（WNS −0.391ns、598 setup失败、0新加载）和10月15—28日窗口保持。共享研究Codex-only；文档请求Sol/medium，实际模型/强度元数据unknown。\n'''
# Assert precise byte protection before any canonical write.
checks=[]
def protected(p,start,end=None):
 v=old[p]; mark=start.encode(); a=v.index(mark);z=v.index(end.encode(),a) if end else len(v);segment=v[a:z]
 nv=text[p].encode();na=nv.index(mark);nz=nv.index(end.encode(),na) if end else len(nv)
 assert nv[na:nz]==segment,p
 checks.append({'path':p,'start':start,'end':end,'bytes':len(segment),'sha256_before':sha(segment),'sha256_after':sha(nv[na:nz]),'equal':True})
protected('CURRENT_STATUS.md','## 各分支最新进展','## 硬件与执行状态')
protected('CURRENT_STATUS.md','## 独立工程事项')
protected('研究计划.md','## I：')
protected('研究导航.md','## 已有成果怎样复用')
for p in ['HANDOFF.md','iommu-platform/docs/validation.md']:
 assert text[p].encode().endswith(old[p]);checks.append({'path':p,'mode':'full_old_bytes_as_suffix','bytes':len(old[p]),'sha256_before':sha(old[p]),'sha256_after':sha(text[p].encode()[-len(old[p]):]),'equal':True})
# Detect concurrent changes between immediate read and write; do not overwrite.
for p in paths: assert (w/p).read_bytes()==old[p], 'concurrent change: '+p
for p in paths:(w/p).write_bytes(text[p].encode())
(b/'README.md').write_text(readme)
receipt={'task':'D-DRAM-BOUNDARY-1 documentation sync','utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'five canonical files, new README and documentation only','requested_model':'gpt-6.1-sol/medium','actual_model_effort':'unknown','protected_ranges':checks,'files':{p:{'before_sha256':sha(old[p]),'after_sha256':sha((w/p).read_bytes()),'before_bytes':len(old[p]),'after_bytes':len((w/p).read_bytes())} for p in paths},'no_model_rerun':True,'link_check':'link-check.json'}
(d/'receipt.json').write_text(json.dumps(receipt,ensure_ascii=False,indent=2)+'\n')
# Only introduced/changed navigation links, not old suffix or whole-repository scans.
newlinks=[]
for p in paths:
 oldlinks=set(re.findall(r'\]\(([^)]+)\)',old[p].decode()))
 newlinks.extend((w/p,u) for u in re.findall(r'\]\(([^)]+)\)',text[p]) if u not in oldlinks)
newlinks.extend((b/'README.md',u) for u in re.findall(r'\]\(([^)]+)\)',readme))
rows=[]
for p,u in newlinks:
 if '://' in u or u.startswith('#'):continue
 target=(p.parent/u.split('#')[0]).resolve(); rows.append({'file':str(p.relative_to(w)),'link':u,'exists':target.exists()})
(d/'link-check.json').write_text(json.dumps({'scope':'introduced or changed navigation links only','checked':len(rows),'missing':sum(not r['exists'] for r in rows),'links':rows},ensure_ascii=False,indent=2)+'\n')
assert all(r['exists'] for r in rows), 'missing introduced link'
print(json.dumps({'changed':paths,'new':'README.md; documentation/','protected_checks':len(checks),'local_links_checked':len(rows),'missing':0},ensure_ascii=False))
