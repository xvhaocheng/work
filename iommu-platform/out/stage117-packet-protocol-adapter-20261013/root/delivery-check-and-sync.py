from pathlib import Path
import json,hashlib
w=Path('/home/xvhaocheng/work'); p=w/'iommu-platform/out/stage117-packet-protocol-adapter-20261013'; i=p/'implementation'
def read(n): return json.loads((p/n).read_text())
def save(n,d): (p/n).write_text(json.dumps(d,ensure_ascii=False,indent=2)+'\n')
def sha(f): return hashlib.sha256(f.read_bytes()).hexdigest()
checks=[]
def check(name,ok):
 checks.append({'check':name,'passed':bool(ok)})
 if not ok: raise AssertionError(name)
for f in read('implementation/artifact-sha256.json')['files']:
 q=i/f['path'];check('artifact '+f['path'],q.stat().st_size==f['bytes'] and sha(q)==f['sha256'])
for f in read('implementation/actual-compiled-dependencies.json')['files']:check('compiled input '+f['path'],sha(Path(f['path']))==f['sha256'])
for f in read('root/prior-assets.json'):check('prior '+f['path'],sha(w/f['path'])==f['sha256'])
for f in read('root/dirty-files-before.json'):
 if f['repo']=='iommu-platform' and f['path']=='docs/validation.md':continue
 check('preserved dirty '+f['repo']+'/'+f['path'],sha(w/f['repo']/f['path'])==f['sha256'])
for f in read('before/manifest.json'):
 q=p/'before'/f['path'];check('before '+f['path'],sha(q)==f['sha256'])
unit=read('implementation/results/protocol-unit.json'); cd=read('implementation/checker/delivery.json'); ledger=read('implementation/complete-execution-ledger.json')
check('17 units',unit['pass'] and unit['count']==17 and all(t['pass'] for t in unit['tests']))
check('independent checker and 14 negative plus 1 control',cd['passed'] and cd['checker_selftests']['negative_tamper_tests']==14 and cd['checker_selftests']['positive_executor_flag_independence_control']==1)
check('two runs once same binary',len(ledger['RTL_integrations'])==2 and len({r['binary_sha256'] for r in ledger['RTL_integrations']})==1 and ledger['RTL_execution_retries']==0)
for case in cd['accepted_cases']:
 r=read('implementation/results/'+case+'/run.json');check(case+' boundaries',r['pass'] and r['payload_bytes']==1639 and r['record_bytes']==1675 and r['application_RTL_latency'] is None and r['cache_RTL_latency'] is None and r['software_RTL_latency'] is None and r['checked_teardown'])
for wit in cd['real_peer_backend_witnesses']:check('peer witness '+str(wit['consumed_packet']),wit['backend_accepted']<=wit['consumer_cycle']<wit['backend_response'])
a={'schema':'root-packet-protocol-acceptance-v1','accepted':True,'scope':'Controlled original-fixture packet protocol function only','cache_backend':'MODEL_UNCACHED_ASSUMPTION','packet_count':3,'payload_bytes':1639,'record_bytes':1675,'exact_spans_bytes':[60,65,503,1011],'protocol_units':unit,'independent_checker_selftests':cd['checker_selftests'],'accepted_cases':cd['accepted_cases'],'peer_backend_witnesses':cd['real_peer_backend_witnesses'],'raw_per_case':cd['raw_per_case'],'logical_memory_bytes_per_case':cd['logical_memory_bytes_per_case'],'artifact_hash_count':214,'actual_compiled_dependency_count':160,'critical_source_review':'root/critical-review.json','execution_ledger':'implementation/complete-execution-ledger.json','costs':'implementation/cost-and-parameters.json','limitations':cd['limits'],'application_RTL_latency':None,'cache_RTL_latency':None,'software_RTL_latency':None,'host_scratch_peak_bytes':None,'native_guest_validated':False,'real_cache_handoff_validated':False,'production_persistent_quarantine_validated':False,'performance_claim':False,'new_G0':False,'innovation_claim':False,'paper_complete':False,'next_gate':'Minimal actual CVA6 cache ownership handoff witness under unchanged original workload; then source-backed delivery target and complete ordinary comparisons for one G0','actual_model_observation':'root/model-runtime-observation.json','goal_registration':'root/goal-registration.json'}
save('root-result-acceptance.json',a)
review=read('root/critical-review.json');review['status']='accepted after direct critical-source review and final artifact/input custody verification';review['acceptance_remaining']=[];save('root/critical-review.json',review)
report='''# stage117逐包协议模型验收（2026-10-13）

原公开接口缺少peer仍活跃时的完整包交接、前缀发布、消费及归还合同。本轮按用户选择先实现协议模型，受控原始工作负载的功能验收通过。缓存后端为`MODEL_UNCACHED_ASSUMPTION`，没有实际CVA6 CPU/cache模型，因此不能据此声称缓存安全、应用加速或独立创新；论文未完成。

进入条件是已有具名软件适配范围、原来源和冻结任务，且原硬件支持精确span和逐engine公开NEXT/DONE。结束条件是实际实现、定向故障测试、两个新案例、独立原始证据核验及状态/计划/交接/验证完整交付。本轮不改生产driver、ABI或RTL，不上板，不恢复关闭分支，不重跑旧实验。

原三帧60/65/1514B、1639B载荷、1675B记录及四span（60/65/503/1011B）保持不变。完成收据绑定engine、span、packet、cookie、ID和世代；包3必须两段完整退休。公开DONE匹配后先启动已准备的合法后继，再处理可发布前缀。交接凭据绑定不可变16B目的容量闭包，每次记录和局部guard读取都检查范围。先校验完整记录，再发布原前缀；源A退休后覆盖复用，实际consumer通过原B cookie读完整链，消费后才归还B。全池与表快照仅在最终已验证quiet读取。

root直接检查协议、控制器、测试和独立检查器代码，发现最初凭据未强制地址闭包，要求在首次RTL构建前补齐逐次读取范围检查及peer/全池/溢出拒绝。修订已纳入最终编译输入。

17项协议测试通过，覆盖部分包、前缀、陈旧/重复收据与凭据、提前回收、未知接受、超时和范围。一次RTL构建及single/parallel两个新案例各一次通过，同一二进制SHA256为`1343401f25c0d66cf25fa0f20b2874700067319b0a6e35fd52835c3c38b31b8f`。独立检查不信执行器PASS标志，从原JSON、公开MMIO、R/W/LAST/B、实际host访问及后端transport重建；14项篡改拒绝和1项执行器标志独立性正控制通过。检查器有离线修订/重复读取，完整次数见账本，未额外运行RTL。

| 双DMA功能见证 | consumer事件位置 | peer实际后端活跃区间 |
|---|---:|---:|
| 包1完整消费 | 853 | 包3 span2：[92,1414) |
| 包2完整消费 | 1458 | 包3 span3：[1422,1619) |

两次消费前本包R/W/B均已退休。这些位置只证明因果顺序；host校验/消费不推进RTL时钟，不能当作真实应用周期。旧single1161/parallel1601有限G0仍关闭，本轮不产生新性能比较或440周期归因。

每案AR/AW/B各6，读总线1672B，有效W字节1639B且恰一次；翻译请求/响应12，lookup12/hit11，PTW_AR2，DDTW_AR分别1/2。每案39项host操作：预先写header36B；发布前读记录1675B和guard133B；consumer另读1675B和guard133B；覆盖源1639B；最终全池读16384B、表读12288B。初始化28672B计入记录。双DMA活动资源2对单DMA1；每DMA 4KiB缓存，FIFO8、AXI在途8，DEFAULT每engine一个公开job，驻留共享2MiB Sv39映射、16KiB逻辑池及12KiB表。全部具体参数、MMIO及源持有记录见成本文件。

RTL构建宿主耗时6.096376秒，两个案例进程分别0.034319/0.036391秒，只是执行成本。单元首次编译的两项缩进警告修正后通过；包装依赖时间字段解析失败也保留。无RTL重试。软件、缓存、应用RTL时延和host scratch峰值仍为null；metadata仅host heap，真实PA/IOVA alias未绑定，逻辑隔离不证明进程退出后的生产物理隔离。

主会话实际Sol/ultra，实现席Astra/ultra，独立检查席Sol/ultra；Codex-only，原生上下文，无额外token预算。模型与执行证据完整保留，没有以配置覆盖冒充实际窗口。

下一关口：在原任务和布局下做最小真实CVA6缓存交接见证，核物理alias、metadata归属、完成收据、完整包读取与cache清空/重填成本。实际接口功能通过后，再冻结来源支持的交付目标，纳入CPU、单DMA、普通多DMA及合法驻留/大页/批量，对照一次G0。不能用更多本模型轮数替代真实缓存验证。

[稳定接受](../out/stage117-packet-protocol-adapter-20261013/root-result-acceptance.json)／[证据导航](../out/stage117-packet-protocol-adapter-20261013/README.md)／[成本与参数](../out/stage117-packet-protocol-adapter-20261013/implementation/cost-and-parameters.json)／[完整执行账本](../out/stage117-packet-protocol-adapter-20261013/implementation/complete-execution-ledger.json)／[最终交付核验](../out/stage117-packet-protocol-adapter-20261013/root/delivery-verification.json)。
'''
(w/'iommu-platform/docs/stage117-packet-protocol-adapter-20261013.md').write_text(report)
(p/'README.md').write_text('''# stage117逐包协议模型交付

功能已接受；MODEL_UNCACHED_ASSUMPTION，实际缓存/应用性能未验证，论文未完成。

- [报告](../../docs/stage117-packet-protocol-adapter-20261013.md)
- [root接受](root-result-acceptance.json)、[亲核记录](root/critical-review.json)
- [实现导航](implementation/delivery.json)、[实际编译依赖](implementation/actual-compiled-dependencies.json)、[214项哈希](implementation/artifact-sha256.json)
- [17项测试](implementation/results/protocol-unit.json)、[两个新案例及原始证据](implementation/results/)
- [独立checker](implementation/checker/delivery.json)、[15项自检](implementation/checker/checker-selftests.json)
- [完整成本](implementation/cost-and-parameters.json)、[执行账本](implementation/complete-execution-ledger.json)
- [实际模型记录](root/model-runtime-observation.json)、[goal登记](root/goal-registration.json)
- [交付检查](root/delivery-verification.json)、[goal完成回执](root/goal-completion.json)
- [原文快照清单](before/manifest.json)

下一步为最小实际CVA6缓存交接见证；不重跑本轮或旧关闭实验。
''')
summary='''stage117逐包协议模型已实现并接受：17项协议测试、两个同二进制新增RTL案例各一次通过；独立原JSON/总线/实际访问核验及14项篡改拒绝、1项独立性控制通过。包1/2消费时peer实际后端仍活跃，本包R/W/B已退休。原1639B/1675B、四span及布局不变，生产driver/ABI/RTL未改，旧实验未重跑。MODEL_UNCACHED_ASSUMPTION，实际CVA6缓存、应用时延、生产持久隔离未验证，未知成本null，无新G0/收益/创新声明，论文未完成。下一步是原工作负载下最小真实CVA6缓存交接见证，闭合alias、metadata和cache成本后再冻结一次G0普通对照；原两周窗口继续累计。'''
status=w/'CURRENT_STATUS.md';old=status.read_text();tail=old[old.index('- 独立Linux贡献：'):];status.write_text('# 当前状态 stage117逐包协议模型已接受 2026年10月13日\n\n'+summary+'\n\n[报告](iommu-platform/docs/stage117-packet-protocol-adapter-20261013.md)／[稳定接受](iommu-platform/out/stage117-packet-protocol-adapter-20261013/root-result-acceptance.json)／[交付核验](iommu-platform/out/stage117-packet-protocol-adapter-20261013/root/delivery-verification.json)。实际goal在完整同步核验后才完成；回执见输出导航。\n\n'+tail)
for name in ['HANDOFF.md','iommu-platform/docs/research-execution-plan-20261011.md','iommu-platform/docs/validation.md']:
 q=w/name;prefix='' if name=='HANDOFF.md' else '../../' if name.endswith('research-execution-plan-20261011.md') else '../../'
 link='iommu-platform/docs/stage117-packet-protocol-adapter-20261013.md' if name=='HANDOFF.md' else 'stage117-packet-protocol-adapter-20261013.md'
 q.write_text('## 2026-10-13 stage117逐包协议模型功能接受\n\n'+summary+'\n\nroot实际Sol/ultra、实现Astra/ultra、checker Sol/ultra；直接关键源码审查及范围修正、实际编译依赖160项、产物214项已核。宿主耗时仅执行成本，软件/cache/application RTL时延null。首次单元编译警告和包装字段解析失败均保留；一次RTL构建、两次案例、零重试。\n\n[报告]('+link+')。完整成本、原始记录、实际goal回执和交付核验从报告导航。以下完整保留前轮记录，当前功能接受不改变旧有限G0裁决。\n\n'+q.read_text())
plan=w/'研究计划.md';old=plan.read_text();plan.write_text('# 多 DMA 共享 IOMMU 研究总计划\n\n'+summary+'\n\n[本轮报告](iommu-platform/docs/stage117-packet-protocol-adapter-20261013.md)／[稳定接受](iommu-platform/out/stage117-packet-protocol-adapter-20261013/root-result-acceptance.json)。旧阶段文字保留为历史裁决；其中“本轮不实现适配”指此前静态关口，已由本轮模型实现推进，实际公开Linux缓存接口仍未实现。\n\n## 前轮静态关口及总规则（历史保留）\n\n'+old.split('\n',1)[1])
for name in ['HANDOFF.md','iommu-platform/docs/research-execution-plan-20261011.md','iommu-platform/docs/validation.md']:
 check('history preserved '+name,(w/name).read_bytes().endswith((p/'before'/name).read_bytes()))
check('plan old body retained',(p/'before/研究计划.md').read_text().split('\n',1)[1] in plan.read_text())
check('status independent contributions retained',tail==status.read_text()[status.read_text().index('- 独立Linux贡献：'):])
for q in [status,plan,w/'HANDOFF.md',w/'iommu-platform/docs/research-execution-plan-20261011.md',w/'iommu-platform/docs/validation.md']:check('sync '+str(q), 'stage117逐包协议模型' in q.read_text() and '论文未完成' in q.read_text())
save('root/delivery-verification.json',{'passed':True,'checks_count':len(checks),'checks':checks,'scope':'New delivery custody and documentation integration only; no model/test/checker replay','goal_completion_pending':True})
save('root/integration-ledger.json',{'author':'root Codex','actions':['Direct source review and bounded closure fix request before RTL build','Final 214 artifact and 160 compiled input hash custody checks','Prior assets and preexisting dirty files preserved','Report, navigator, total/local plans, status, handoff and validation synchronized','New integration check only; no simulation/unit/independent checker rerun'],'external_models':False,'paper_complete':False})
print(json.dumps({'passed':True,'checks':len(checks),'report':str(w/'iommu-platform/docs/stage117-packet-protocol-adapter-20261013.md')},ensure_ascii=False))
