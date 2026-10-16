from pathlib import Path
import hashlib,json,re,datetime
W=Path('/home/xvhaocheng/work'); R=W/'iommu-platform/out/d-mqsim-identity-20261015'; D=R/'documentation'
files=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md']
def sha(b): return hashlib.sha256(b).hexdigest()
assert not (D/'before-write').exists(), 'One-shot sync: do not replace original snapshots or prepend twice'
old={p:(W/p).read_bytes() for p in files}
for p,b in old.items():
 q=D/'before-write'/p;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(b)
texts={p:b.decode() for p,b in old.items()}; new=dict(texts)
base='iommu-platform/out/d-mqsim-identity-20261015/'
summary='一次身份重放复现旧全部事件/XML，确认UID1777/CID53/child2841在GC屏障释放时被mapping-only handler后删除、未自身写入。私有LPA release最小修复后，原512/2048/4096/8192输入全部完成，逐14848请求核因果；四个完整host＋模拟PCIe排空边界活动GC均0。停止本版cache-off有限覆写残留扩测，不调seed/cache/相位/长度追正例；不否定SSD其他状态或整个D。'
nexttext='下一有界候选为已核DRAM行/刷新：先按真实source规则核严格排空、quiet及显式warm/等待后状态保留或洗去；合同尚未执行，暂不接P/PN选策。'
links=f'[本轮入口]({base}README.md)／[root裁决]({base}root/decision-analysis.md)／[稳定接受]({base}root/root-result-acceptance.json)／[下一最小合同]({base}root/next-minimal-contract.json)'
s=new['CURRENT_STATUS.md'];s=s.replace('# 当前状态：D持续服务来源与完整完成门','# 当前状态：D完成门已过，本版GC残留停止扩测')
a=s.index('- SSD覆写');b=s.index('\n- I有条件',a)
s=s[:a]+'- '+summary+nexttext+links+'。'+s[b:]
a=s.index('本轮为host-only');b=s.index('\n\n## 独立工程事项',a)
s=s[:a]+'本轮新增5次私有host构建、1次身份重放、4次私有修复case；root审查后亲自工具执行，并独立只读核账。748项启动前hash及4种负变异通过；旧512事件/XML不变。0旧M0/CS1/硬件实验、原生DMA、生产Linux/RTL/ABI修改、collector/FPGA或板操作。原MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。共享研究Codex-only；请求模型/强度与实际运行元数据分开，后者unknown。'+s[b:];new['CURRENT_STATUS.md']=s
s=new['研究计划.md'];s=s.replace('CS1局部停止保持；SSD/DRAM来源已核，先定位MQSim固定2048输入缺完成，正确性通过后才接P/PN，固定M0不扩大','CS1局部停止保持；MQSim四冻结输入完成门已过，本版GC残留停止扩测；先核DRAM行/刷新在显式warm后的保留/洗去，固定M0不扩大')
a=s.index('当前实际结果：');b=s.index('\n\n## D：',a)
s=s[:a]+'当前实际结果：'+summary+' '+nexttext+' 本轮新增5次私有host构建、1次身份重放、4次修复case；root审查后亲自工具执行，独立核账748项hash与4种负变异。屏障重投0/1/2/4，满plane恢复0/37/91/205；旧512事件/XML不变。接受仅限四个整页USERIO write-only/cache-off单资源输入，不是通用上游修复或真实数据字节正确性认证。D主线、I轻量not_ready、A5/A2、10月15—28日窗口及固定M0/CS1局部停止保持。'+links+'。'+s[b:]
a=s.index('持续服务来源关已完成');b=s.index('\n\n## I：',a)
s=s[:a]+'持续服务来源关保留SSD/DRAM来源事实；MQSim身份故障已定位并最小私有修复，四输入完整完成后均无跨完整排空边界活动GC，所以停止该版cache-off有限覆写残留扩测。'+nexttext+' 下一合同先冻结真实默认规则、地址/读数/时钟与controller排空谓词，区分普通前史、独立bank warm、冲突行/cap4 PRE和完成刷新控制；不注入独立状态或随机刷新相位。O_phys可重放不证明真实O_host已足够，给普通对照相同实际可得信息；host日志、Z及观测/保存/传输/推断/切换/等待费用均须计入，真实费用unknown。若warm洗去状态、普通信息已足够或接口不能认证排空，限定结果，不强造传感器贡献。来源事实及私有修复不证明信息价值、P/PN收益、新意或原生性能；后续是否冻结共享资源动作模型由root裁决。常驻池、合法大映射/准备、固定动作、完整O0、队深/poll、全费用等待/试选仍是普通对照，付费Z同样交给普通规则；不扩大固定M0、不调gap、不加collector或改guard。'+links+'。'+s[b:];new['研究计划.md']=s
s=new['研究导航.md']; lines=s.splitlines();lines=[('| D：有限观测→完整控制选择 | 主线；MQSim身份根因/私有修复已核，四输入完成；本版GC残留停止扩测，下一核DRAM行/刷新边界；CS1局部停止保持 | '+links.replace('／',' · ')+' |') if x.startswith('| D：') else x for x in lines];s='\n'.join(lines)+'\n';a=s.index('两来源已核且优先SSD');b=s.index('\n\n## 已有成果',a);s=s[:a]+'MQSim四原冻结输入在私有修复后完整完成，但完整排空边界活动GC均0；停止本版cache-off有限覆写残留扩测，保留SSD来源事实，不自动改I/A安排，见[root裁决]('+base+'root/decision-analysis.md)。'+nexttext+' D主线、CS1局部停止、I/A保留及10月15—28日窗口不变，见[下一最小合同]('+base+'root/next-minimal-contract.json)。'+s[b:];new['研究导航.md']=s
stage='# 2026年10月15日 D-MQSim身份根因、私有修复与GC边界裁决\n\n'+summary+' '+nexttext+'\n\n一次初始身份重放与旧全部native事件/XML完全一致。根因已由原源码与唯一身份链绑定：UID1777/native1776/CID53/LBA2912/LPA182/child2841在屏障释放后被仅处理MAPPING的handler忽略并删除，未走自身写入。仅私有LPA release语义修复，保留原child和owner，解锁后正常重译/分配/模拟器program/owner remove/CQE；不补造成功。四冻结输入逐14848请求核唯一因果完成，屏障进入/重投0/1/2/4、满plane进入/恢复0/37/91/205；无锁内分配或GC候选带未完成用户program。748项启动前hash及4种负变异通过；原512事件/XML不变。三组覆写修复前后不能作性能比较。\n\n本轮新增5次私有host构建、1次身份重放、4次修复case；root审查补丁/运行器后亲自工具执行，独立核账不重跑模型。接受限四个整页USERIO write-only/cache-off单资源输入；不认证通用上游修复、混合读写/partial/cache/多资源或真实payload数据字节。没有P/PN信息价值、新意或原生性能结论。旧run原字节与当时NOT_ESTABLISHED裁决保留，不回填成功。\n\nD主线/I轻量not_ready、A5/A2及独立工程事项、10月15—28日窗口不变；固定M0/CS1局部停止保持。0旧M0/CS1/硬件实验、原生DMA、生产Linux/RTL/ABI修改、collector/FPGA或板操作。MIG1792023314 paused、build6 WNS −0.391ns、598项setup失败、0新加载采集/实际债务unknown。共享研究Codex-only，无DeepSeek/Claude；普通实现/文档请求Sol/medium、有界正确性设计Sol/xhigh，实际模型/强度元数据unknown；历史记录按当时保留，旧个人研究方法不恢复。\n\n'+links+'／[身份根因]('+base+'root/diagnosis.md)／[逐请求核账]('+base+'root/repair-verification.json)／[文档回执]('+base+'documentation/receipt.json)。文档整合仅核前后hash、本地链接、并发变化和保护段；下一DRAM合同未执行，以下旧全文逐字节保留。\n\n'
new['HANDOFF.md']=stage+texts['HANDOFF.md'];new['iommu-platform/docs/validation.md']=stage.replace(base,'../out/d-mqsim-identity-20261015/')+texts['iommu-platform/docs/validation.md']
readme='''# D-MQSim身份定位、私有修复与持续状态边界

2026年10月15日。固定身份根因与四输入完成门通过；四个合法host＋模拟PCIe排空边界活动GC均0，停止本版cache-off有限覆写残留扩测。下一DRAM行/刷新边界合同尚未执行。

- [root稳定接受](root/root-result-acceptance.json)、[裁决与范围](root/decision-analysis.md)、[根因原文/日志行](root/diagnosis.md)、[初始身份接受](root/identity-acceptance.json)、[缺失请求摘录](root/missing-request-events.json)。
- [观察实现与4次构建账本](implementation/README.md)、[观察补丁](implementation/identity-observer.patch)、[最终构建manifest](implementation/pre-build-manifest.json)、[构建结果](implementation/build-status.json)。实现README保留当时交付记录；root后续实际执行以root接受和run manifest为准。
- [唯一初始身份重放原始日志](implementation/run-2048/stderr.log)、[启动前manifest](implementation/run-2048/pre-run-manifest.json)、[运行结果](implementation/run-2048/status.json)；旧全部native事件/XML完全一致。
- [私有修复合同](root/repair-contract.json)、[root审查](root/repair-review.json)、[私有补丁](implementation/repaired/repair.patch)、[修复交付](implementation/repaired/README.md)、[第五次构建manifest](implementation/repaired/pre-build-manifest.json)、[构建结果](implementation/repaired/build5-status.json)。
- [四case原始运行与manifest](implementation/repaired/runs/)、[逐14848请求核账/748hash/4负变异](root/repair-verification.json)、[被丢请求修复后的自身完成链](root/repaired-target-events.json)。
- [下一DRAM最小来源合同，未执行](root/next-minimal-contract.json)、[文档整合回执](documentation/receipt.json)、[写入前5份入口快照](documentation/before-write/)。
- [上轮来源报告](../d-persistent-source-20261015/README.md)及其全部原始源、8运行、1调试重放与当时未建立裁决原样保留；本轮是后继新证据，不改写旧run。原始入口快照另在[before](before/)。

实际新增5次私有host构建、1次身份重放、4次修复case；root审查后亲自工具执行，独立核账不运行模型。仅四个冻结整页USERIO write-only/cache-off单资源输入获得模型因果完成验收，不是通用上游修复、真实数据字节正确性、SSD实测时间、P/PN信息价值或新意证明。禁止调seed/cache/相位/长度追本版正例，不否定SSD全部状态或D；D/I/A安排、CS1局部停止、旧硬件暂停与生产源码保持。

共享IOMMU Codex-only，无DeepSeek/Claude。普通实现/文档请求Sol/medium，有界正确性设计Sol/xhigh；实际推理元数据unknown，历史模型记录不重写。文档检查没有执行模型、测试、构建或新source调研。
'''
# Assert originals still unchanged immediately before writing.
for p,b in old.items(): assert (W/p).read_bytes()==b,p
for p,s in new.items(): (W/p).write_text(s)
(R/'README.md').write_text(readme)
# Verify protected byte slices and preserved historical suffixes.
assert new['CURRENT_STATUS.md'].split('## 各分支最新进展\n')[1].split('## 硬件与执行状态')[0]==texts['CURRENT_STATUS.md'].split('## 各分支最新进展\n')[1].split('## 硬件与执行状态')[0]
assert new['CURRENT_STATUS.md'].split('## 独立工程事项')[1]==texts['CURRENT_STATUS.md'].split('## 独立工程事项')[1]
assert new['研究计划.md'].split('## I：')[1]==texts['研究计划.md'].split('## I：')[1]
assert new['研究导航.md'].split('## 已有成果怎样复用')[1]==texts['研究导航.md'].split('## 已有成果怎样复用')[1]
for p in ['HANDOFF.md','iommu-platform/docs/validation.md']: assert (W/p).read_bytes().endswith(old[p])
def badlinks(s,src):
 bad=[];count=0
 for target in re.findall(r'\]\(([^)]+)\)',s):
  if re.match(r'[a-z]+://|#|mailto:',target): continue
  target=target.split('#')[0].strip('<>');target=re.sub(r':\d+$','',target)
  count+=1
  if not (src.parent/target).exists():bad.append(target)
 return count,bad
(D/'receipt.json').touch(exist_ok=True)
checks={}
for p,s in new.items():
 c,bad=badlinks(s,W/p);_,prior=badlinks(texts[p],W/p)
 checks[p]={'checked_local_links':c,'preexisting_missing':sorted(set(prior)),'current_missing':sorted(set(bad)),'new_missing':sorted(set(bad)-set(prior))}
c,bad=badlinks(readme,R/'README.md');checks['README.md']={'checked_local_links':c,'new_missing':bad}
receipt={'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'documentation only; no models/tests/source investigation executed','requested_model':'gpt-6.1-sol/medium','actual_model_and_effort':'unknown','files':{},'protected_assertions':{'current_A5_A2_bytes':True,'current_independent_engineering_tail_bytes':True,'plan_I_assets_hardware_tail_bytes':True,'navigation_assets_code_history_tail_bytes':True,'handoff_old_full_suffix_bytes':True,'validation_old_full_suffix_bytes':True,'immediate_prewrite_five_file_reread_and_guard':True},'links':checks,'changes':['CURRENT D summary + execution attribution','plan current facts/stage/next minimal input','navigation D row + next gate','single new HANDOFF/validation stage prepended','new run README and documentation receipt'],'root_artifacts_modified':False,'old_runs_modified':False}
for p in files:
 prior=R/'before'/p
 receipt['files'][p]={'before_write_sha256':sha(old[p]),'after_sha256':sha((W/p).read_bytes()),'original_run_before_sha256':sha(prior.read_bytes()) if prior.exists() else None,'concurrent_change_since_original_before':prior.exists() and prior.read_bytes()!=old[p]}
receipt['README_sha256']=sha((R/'README.md').read_bytes())
(D/'receipt.json').write_text(json.dumps(receipt,ensure_ascii=False,indent=2)+'\n')
assert all(not v['new_missing'] for v in checks.values()),checks
print(json.dumps({'status':'PASS','receipt':str(D/'receipt.json'),'before_changed_since_run_snapshot':[p for p,v in receipt['files'].items() if v['concurrent_change_since_original_before']],'local_link_checks':sum(v['checked_local_links'] for v in checks.values()),'new_missing_links':0},ensure_ascii=False))
