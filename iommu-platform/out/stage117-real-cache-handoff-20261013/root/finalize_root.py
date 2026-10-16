from pathlib import Path
import json,hashlib,re,subprocess
w=Path('/home/xvhaocheng/work');p=w/'iommu-platform/out/stage117-real-cache-handoff-20261013';i=p/'implementation';c=p/'checker'
def get(q):return json.loads(q.read_text())
def put(q,d):q.write_text(json.dumps(d,ensure_ascii=False,indent=2)+'\n')
def sha(q):return hashlib.sha256(q.read_bytes()).hexdigest()
checks=[]
def ck(n,v):
 checks.append({'check':n,'passed':bool(v)})
 if not v:raise AssertionError(n)
delivery=get(i/'delivery.json');cd=get(c/'delivery.json');ck('stable independent path',delivery['independent_checker_acceptance'] is not None)
manifest=get(i/'artifact-sha256.json')
for f in manifest:ck('artifact '+f['path'],sha(i/f['path'])==f['sha256'] and (i/f['path']).stat().st_size==f['bytes'])
deps=get(i/'actual-compiled-dependencies.json')['files']
for f in deps:ck('compiled '+f['path'],sha(Path(f['path']))==f['sha256'])
for f in get(i/'build-inputs.json')['files']:ck('declared compile '+f['path'],sha(w/'iommu-platform'/f['path'])==f['sha256'])
for f in get(p/'root/prior-assets.json'):ck('prior '+f['path'],sha(w/f['path'])==f['sha256'])
for f in get(p/'root/dirty-files-before.json'):
 if f['repo']=='iommu-platform' and f['path']=='docs/validation.md':continue
 ck('dirty preserved '+f['repo']+'/'+f['path'],sha(w/f['repo']/f['path'])==f['sha256'])
for f in get(p/'before/manifest.json'):ck('before '+f['path'],sha(p/'before'/f['path'])==f['sha256'])
for name,h in cd['core_source_sha256'].items():ck('checker source '+name,sha(c/name)==h)
for name,h in cd['evidence_sha256'].items():ck('checker evidence '+name,sha(c/name)==h)
reports={}
for name,d in cd['reports'].items():
 q=Path(d['path']);ck('checker report '+name,sha(q)==d['sha256']);reports[name]=get(q);ck('passed '+name,reports[name]['passed'])
for name,d in get(c/'trace-sha256.json')['cases'].items():
 for filename,h in d['artifacts'].items():ck('raw custody '+name+'/'+filename,sha(Path(d['directory'])/filename)==h)
for f in get(p/'root/critical-source-review.json')['new_sources_direct_review']:ck('root reviewed '+f['path'],sha(p/f['path'])==f['sha256'])
for f in get(i/'toolchain/provenance.json'):
 ck('tool native '+f['tag'],sha(i/f['native_binary'])==f['native_binary_sha256']);ck('tool archive '+f['tag'],sha(i/f['source_archive'])==f['source_archive_sha256'])
ledger=get(i/'execution-ledger.json');binary=sha(i/delivery['same_binary']);ck('three new executions eachonce samebinary',len(ledger)==3 and all(r['attempt']==1 and r['returncode']==0 and r['binary_sha256']==binary for r in ledger))
ck('eight cache rejects plusflag control',cd['passed_bounded_cache_specific_checker_controls']==9 and get(c/'checker-selftests.json')['passed'])
ck('consumer overlap accurately false',not reports['parallel_FIFO_ahead']['real_backend_active_peer_during_actual_cache_consumer'])
ck('early handoff whilepeer positive',bool(reports['parallel_FIFO_ahead']['earlier_acquire_or_used_publication_peer_witnesses']))
for name in ['single_FIFO_ahead','parallel_FIFO_ahead']:
 d=reports[name];ck(name+' correct costs and limits',d['costs']['cache_cached16B_refill_bus_bytes']==18480 and d['costs']['cache_uncached8B_read_bus_bytes']==12288 and d['costs']['application_latency'] is None and not d['full_CPU_instruction_execution'] and d['release_quiet']['all_postscan_cache_DDR_RB_retired'])
review=get(p/'root/critical-source-review.json');review['status']='root direct critical source/raw evidence and final custody accepted';put(p/'root/critical-source-review.json',review)
a={'schema':'root-stage117-real-cache-acceptance-v1','accepted':True,'actual_cache_function':True,'three_microtests':reports['microtests']['tests'],'two_integration_cases':['single_FIFO_ahead','parallel_FIFO_ahead'],'original_fixture_payload_bytes':1639,'original_record_bytes':1675,'original_spans_bytes':[60,65,503,1011],'models':'root and both new seats gpt-6.1-sol/ultra; native effective475000 observed','same_binary_sha256':binary,'executions':ledger,'compiler_attempts':8,'failed_compiler_attempts':6,'successful_compiler_attempts':2,'actual_compiled_dependencies':len(deps),'implementation_artifacts':len(manifest),'new_checker_controls':9,'full_checker_evaluations':cd['full_checker_evaluations'],'physical_CPU_DMA_alias_validated':True,'actual_physical_metadata_validated':True,'packet_RWB_and_public_DONE_verified':reports['parallel_FIFO_ahead']['actual_packet_RWB_and_public_DONE_verified'],'earlier_peer_active_stages':reports['parallel_FIFO_ahead']['earlier_acquire_or_used_publication_peer_witnesses'],'actual_consumer_peer_overlap':False,'overlap_decision':'not observed for controlled original schedule; no workload/schedule tuning or rerun','costs_by_case':{n:reports[n]['costs'] for n in ['single_FIFO_ahead','parallel_FIFO_ahead']},'full_CPU_instruction_execution':False,'virtual_CPU_MMU_alias_validation':False,'production_driver_API_or_persistent_quarantine_validation':False,'application_latency':None,'full_CPU_latency':None,'board_latency':None,'new_G0':False,'performance_or_innovation_claim':False,'limits':cd['limits'],'next_gate':'Freeze source-backed delivery target and complete lawful ordinary CPU/single-DMA/multi-DMA comparisons for one G0; do not repeat cache/protocol tests or demand a preproved residual','paper_complete':False,'goal_registration':'root/goal-registration.json','critical_review':'root/critical-source-review.json','checker_delivery':'checker/delivery.json','costs':'implementation/costs-and-overlap.json'}
put(p/'root-result-acceptance.json',a)
nextc={'schema':'next-goal-preparation-contract-v1','status':'next scope only; not a frozen or registered G0 execution','current_gate':'controlled actualcache/controller function accepted; actualconsumer peer overlap absent','next_action':'source-backed delivery metric and full ordinary comparison contract; then one G0','required':['original ownership/task dependencies/byte/address/cookie/DID/mapping conserved','include lawful direct CPU copy and continued single/multiDMA supply/poll/completeprefix delivery','choose lawful minimum handoff permethod; do not force directCPU copy to pay unnecessary DMA cache flush','retain available resident/large mappings and batching when exact original work permits; no artificial one-child constraint','separate packet acquire/used publication/source reuse/actualBconsumer/Breturn and allocation release','charge initialization,cache flush/refill,verification,source holds,CPU supply/consumer and release; audits separately identified','model CPU port costs are not fullCPU or application time; native/board unknown null','causal IOMMU interference and novelty separate from finite performance vector; residual required only to enterG1 after G0'],'forbidden':['old same-input replay','new pressure/deadline/arrival/ringexhaustion invention','closed R107/C112 or other branch resurrection','consumer-peer overlap manufactured by reordering current validation run','new RTL/driver system without a specific accepted need'],'model_policy':'gpt-6.1-sol/ultra allseats, native context, noAstra/DeepSeek/Claude; no old seats resumed','paper_complete':False}
put(p/'next-goal-contract.json',nextc)
report='''# stage117真实缓存交接验证（2026-10-13）

真实WT缓存和控制路径的受控功能验收通过。三项新缓存微测试、single/parallel两个新增案例均通过独立原始证据检查。包1缓存取得和发布时peer仍活跃，但实际消费时peer已完成：本轮消费重叠投影未成立，不调整顺序或压力重跑。论文未完成，无新G0、性能收益或创新声明。

原问题是已接受的逐包协议仍使用不缓存的host直接消费，不能证明缓存可见性。进入条件为原三帧1639B载荷/1675B记录、四span60/65/503/1011B、地址/cookie/DID及驻留映射不变，既有硬件接口可表达有界交接。结束条件为实际缓存/控制接口实现、定向微测试与两个集成案例、原始字节/所有权/成本核验及完整状态交付；root接受和同步后才完成goal。

实际源码绑定Genesys2 WT_DCACHE和cv64a6_imafdc_sv39，Dcache128bit即16B缓存行、8way。实际wt_cache_subsystem与controller接到原互连CPU入口，共享原DDR backing。CPU直接物理端口0x80000000+offset；DMA原IOVA0x10000000+offset经驻留2MiB leaf到同一PA。metadata为池外0x80004000起每包64B实际缓存读写，仍是可信同owner模型，没有虚拟CPU/MMU、guest隔离或生产ABI承诺。

池、表、header和metadata原始初始化均经实际CPU缓存端口；host仅预分配零backing。公开NEXT/DONE调度，匹配后先启动合法后继，再交接完成前缀。WT普通fence不清Dcache，实际fence_i_i输入请求I/D失效，真实Dcache ack和控制器halt释放才完成；I/D各256行完整清空均从实际row信号检查。实际cacheline refill必须位于已交接包容量闭包或本包metadata，peer活跃时不扫全池。A源覆写和metadata标记均等待WT B退休；B原cookie真实缓存消费完成才归还。最后全池/表/metadata读取后重新排空CPUcache和DDR，再释放逻辑映射。

| 微测试 | 实际原始证据 |
|---|---|
| 缺失交接 | DMA已退休，CPU仍返回旧A5缓存字11936128518282651045 |
| 正确交接 | 实际控制确认后返回原JSON预期新字2207613190144 |
| 待完成写／MSHR | 写等待11周期、MSHR等待10周期后才进入清空；完整清空后确认 |

三个微测试在一次新micro运行中完成，另两个集成案例各一次，三次运行同一二进制：`edb148dd8796b1f55b446a1ca79ade2b917d52417316df2f24a6d485c7179fe4`。独立checker不信PASS或统计，重建原JSON、CPU grant/response、cache和DDR全部AXI、公开完成收据、物理metadata与最终内存。8项新缓存篡改被拒绝，1项PASS独立性控制通过；完整checker共12次评估，定向成本标签刷新另外记录，无RTL复跑或旧17项协议测试重放。

| parallel包1阶段 | 共同模型时钟位置 | peer包3 span2 |
|---|---:|---|
| 缓存取得完成 | 12507 | 活跃区间[11488,12810) |
| used发布 | 12637 | 仍活跃 |
| 实际consumer缓存请求/响应 | [12848,12911] | 已完成 |

包2/3实际consumer区间为[13456,13523]/[16056,16487]，同样无peer重叠。包1发布后到源复用完成的211周期内peer结束；这是所测校验/覆写/消费顺序的边界，不证明所有合法消费顺序都不能重叠，也不证明cache fence必须等待所有DMA静止。

每案实际DMA AR/AW/B各6，R总线1672B，有效W1639B恰一次，翻译请求/响应12，lookup12/hit11，PTW_AR2，DDTW_AR为1/2。实际CPU共7927次操作（4088读、3839写），有用读32576B、写30659B，包含初始化和全部最终扫描。实际cache AXI读取2691笔：1155笔16B缓存重填18480B，另1536笔8B不缓存表读取12288B；不能把30768B总读取全当重填。WT AW/B各3870。

每包交接258模型周期，三包共774；启动交接302，终末扫描前交接258，总计1334。reset另有一次清空，I/D清空分别1536周期。单/双DMA验证分别116/132/1576周期；源覆写及退休metadata分别112/99/653与211/99/653周期；consumer含metadata/guard/确认分别77/81/445周期。启动至共同就绪11419周期；最终核验扫描与排空36804周期独立列账，释放位置53416/53320。所有这些是实际端口驱动模型成本；完整CPU指令、软件、应用、Linux和FPGA时间仍null，不将单/双之间的位置差解释为收益或翻译因果。

构建有8次调用（6失败、2成功）：重复测试包、旧工具随机语法限制、5.006内部错误、缺少依赖及5.040 gate优化器错误均保留。隔离的官方5.040加-fno-gate通过；工具5.006/5.040源码编译约30.06/31.00秒，未替换系统工具或修改原RTL。成功构建7约7.81秒，构建8为新增micro释放观测的增量编译约1.61秒；micro进程约0.177秒，两案各约0.700秒，只作宿主执行成本。早期失败构建输入快照未完整保存，重建argv明确标识；最终成功构建8输入快照及实际编译依赖均保留，不能用当前快照冒充历史快照。

root直接审查关键实际源码、顶层、端口驱动、独立checker和原始微测试，要求补齐真实初始化、Icache完整清空、WT store退休与最终读取后的新排空证明。全部工作由root与两个新Sol/ultra席完成，运行观察均gpt-6.1-sol、有效窗口475000，无额外预算或上下文覆盖，不用Astra/DeepSeek/Claude。无生产driver/ABI/RTL改动、板操作或关闭实验复跑。

下一步固定来源支持的交付目标和CPU/单DMA/普通多DMA完整强对照，再一次G0；不重做缓存功能检查，不先要求实证残余再重复同题。直接CPU对照使用自身合法最小交接，不能人为强制同样的DMA全cache清空。继续计校验、源持有、cache重填、供给和释放成本；只有G0得到具体可达可避免翻译损失才进入G1。原两周立题窗口继续累计，本轮结束不代表论文完成。

[稳定裁决](../out/stage117-real-cache-handoff-20261013/root-result-acceptance.json)／[导航](../out/stage117-real-cache-handoff-20261013/README.md)／[完整分阶段成本](../out/stage117-real-cache-handoff-20261013/implementation/costs-and-overlap.json)／[独立checker](../out/stage117-real-cache-handoff-20261013/checker/delivery.json)／[后继范围合同](../out/stage117-real-cache-handoff-20261013/next-goal-contract.json)／[交付核验](../out/stage117-real-cache-handoff-20261013/root/delivery-verification.json)。
'''
(w/'iommu-platform/docs/stage117-real-cache-handoff-20261013.md').write_text(report)
(p/'README.md').write_text('''# stage117真实缓存交接交付

受控实际WT缓存/controller功能已接受；consumer-peer重叠未观察到，完整CPU/应用/FPGA未验证，论文未完成。

- [报告](../../docs/stage117-real-cache-handoff-20261013.md)、[root稳定接受](root-result-acceptance.json)
- [实现导航](implementation/delivery.json)、[分阶段成本](implementation/costs-and-overlap.json)
- [源码及原始来源绑定](implementation/source-and-oracle-binding.json)、[实际编译依赖](implementation/actual-compiled-dependencies.json)、[产物哈希](implementation/artifact-sha256.json)
- [构建账本](implementation/build-ledger.json)、[三个新运行账本](implementation/execution-ledger.json)、[工具来源](implementation/toolchain/provenance.json)
- [独立checker交付](checker/delivery.json)、[9项定向控制](checker/checker-selftests.json)、[checker完整账本](checker/execution-ledger.json)
- [root亲核](root/critical-source-review.json)、[微测试关口](root/microtest-direct-review.json)、[实际模型](root/model-runtime-observation.json)
- [当前合同](implementation-contract.json)、[下一关口](next-goal-contract.json)
- [goal登记](root/goal-registration.json)、[goal完成回执](root/goal-completion.json)、[最终交付检查](root/delivery-verification.json)
- [五份原文快照](before/manifest.json)
''')
summary='stage117真实缓存交接功能已接受：实际WT缓存/controller、共享CPU/DMA物理backing及metadata闭合；3项微测试、两个同二进制新案例各一次通过，独立8项篡改拒绝＋1项PASS独立性控制通过。包1取得/发布时peer活跃，真正消费重叠未观察到，不加压或调序重跑。每包缓存交接258模型周期，重填18480B与不缓存表读取12288B分开计；完整CPU/应用/FPGA成本null。无新G0/收益/创新声明，论文未完成。'
nexttext='下一步固定来源支持交付目标与合法CPU/单DMA/普通多DMA强对照，完整计成本后一次G0；不重复缓存关口、不恢复关闭分支，原两周窗口继续累计。'
q=w/'CURRENT_STATUS.md';old=q.read_text();tail=old[old.index('- 独立Linux贡献：'):];q.write_text('# 当前状态 stage117真实缓存交接已接受 2026年10月13日\n\n'+summary+'\n\n'+nexttext+'\n\nroot及两新增席均gpt-6.1-sol/ultra，有效窗口475000；无Astra/DeepSeek/Claude、额外预算或旧席恢复。完整同步及交付核验后才完成实际goal。\n\n[报告](iommu-platform/docs/stage117-real-cache-handoff-20261013.md)／[稳定接受](iommu-platform/out/stage117-real-cache-handoff-20261013/root-result-acceptance.json)／[下一关口](iommu-platform/out/stage117-real-cache-handoff-20261013/next-goal-contract.json)／[交付导航](iommu-platform/out/stage117-real-cache-handoff-20261013/README.md)。\n\n'+tail)
for n in ['HANDOFF.md','iommu-platform/docs/research-execution-plan-20261011.md','iommu-platform/docs/validation.md']:
 q=w/n;link='iommu-platform/docs/stage117-real-cache-handoff-20261013.md' if n=='HANDOFF.md' else 'stage117-real-cache-handoff-20261013.md';q.write_text('## 2026-10-13 stage117真实缓存交接接受\n\n'+summary+'\n\n'+nexttext+'\n\n关键边界：实际端口驱动、controller输入，未执行完整CPU指令；peer-active缓存交接/发布与实际消费分别裁决。CPU真实初始化、每行清空、WT B退休、最终扫描后release_quiet均亲核。生产driver/ABI/RTL未改，三运行各一次，6失败/2成功构建及隔离工具成本保留。root与两席全Sol/ultra。完整交付核验后才实际goal complete。\n\n[报告]('+link+')提供原始证据、成本、实际goal回执和交付导航。以下完整保留前轮历史，不修改旧有限G0裁决。\n\n'+q.read_text())
q=w/'研究计划.md';old=q.read_text();q.write_text('# 多 DMA 共享 IOMMU 研究总计划\n\n'+summary+'\n\n'+nexttext+'\n\n[本轮报告](iommu-platform/docs/stage117-real-cache-handoff-20261013.md)／[下一关口合同](iommu-platform/out/stage117-real-cache-handoff-20261013/next-goal-contract.json)。此前“实际缓存待验”已由受控缓存端口验证推进；实际CPU程序、原生应用与生产接口仍未实现。C117R3仍为暂定载体，未固定论文主场景。\n\n## 前轮关口与总规则（完整历史保留）\n\n'+old.split('\n',1)[1])
for n in ['HANDOFF.md','iommu-platform/docs/research-execution-plan-20261011.md','iommu-platform/docs/validation.md']:ck('history '+n,(w/n).read_bytes().endswith((p/'before'/n).read_bytes()))
ck('plan history',(p/'before/研究计划.md').read_text().split('\n',1)[1] in (w/'研究计划.md').read_text());ck('contributions preserved',(w/'CURRENT_STATUS.md').read_text().endswith(tail))
for n in ['CURRENT_STATUS.md','HANDOFF.md','研究计划.md','iommu-platform/docs/research-execution-plan-20261011.md','iommu-platform/docs/validation.md']:ck('shared synchronized '+n,summary in (w/n).read_text() and nexttext in (w/n).read_text())
for repo in get(p/'root/worktree-before.json'):
 q=w/repo['repo'];ck('HEAD unchanged '+repo['repo'],subprocess.check_output(['git','rev-parse','HEAD'],cwd=q,text=True).strip()==repo['head'])
for q in [p/'README.md',w/'iommu-platform/docs/stage117-real-cache-handoff-20261013.md']:
 for target in re.findall(r'\]\(([^)]+)\)',q.read_text()):
  f=(q.parent/target).resolve();ck('navigation '+target,f.exists() or f in [p/'root/goal-completion.json',p/'root/delivery-verification.json'])
put(p/'root/delivery-verification.json',{'passed':True,'checks_count':len(checks),'checks':checks,'scope':'New custody/integration only; no simulations/unit/checker replay','goal_completion_pending':True,'paper_complete':False})
put(p/'root/integration-ledger.json',{'author':'root gpt-6.1-sol/ultra','simulations_by_root':0,'old_tests_replayed':0,'independent_checker_replays_by_root':0,'root_raw_micro_review':1,'actions':['actual source/interface/fence and cache-lifecycle critical review','required CPUinit/row-sweep/Bretire/postscanquiet revisions','direct original micro raw proof and independent results acceptance','compiled/artifact/checker/raw/original-state custody','shared five docs and next-scope synchronization'],'performance_or_innovation_claim':False})
print(json.dumps({'passed':True,'checks':len(checks),'implementation_artifacts':len(manifest),'actual_compiled_inputs':len(deps),'report':str(w/'iommu-platform/docs/stage117-real-cache-handoff-20261013.md')},ensure_ascii=False))
