from pathlib import Path
import hashlib,json,datetime
root=Path('/home/xvhaocheng/work');plat=root/'iommu-platform';out=plat/'out/r107-bounded-decision-20261012'
files=[(root/'CURRENT_STATUS.md','CURRENT_STATUS.md'),(root/'HANDOFF.md','HANDOFF.md'),(root/'研究计划.md','研究计划.md'),(plat/'docs/validation.md','validation.md'),(plat/'docs/research-execution-plan-20261011.md','research-execution-plan-20261011.md')]
for p,n in files:
    baseline=out/'before'/('concurrent-'+n)
    if not baseline.is_file(): baseline=out/'before'/n
    assert p.read_bytes()==baseline.read_bytes(),f'further concurrent documentation change: {p}'

current='''# 当前状态 stage108 2026年10月12日 R107有限合同与有界立题裁决完成

- 整体仍为第一阶段“问题与方法立题”。本轮实际goal已登记、无额外token预算；R107条件保留有限合同资产，没有新G0／G1、创新或净收益验收。[稳定裁决](iommu-platform/out/r107-bounded-decision-20261012/root-result-acceptance.json)。
- 固定vLLM v0.1.0＋官方OPT-125m完整12层／12heads，TP2分片为明确研究选择。一次源函数／CPU全字节回放通过：3唯一槽、144完整恢复、1,769,472B；此前换出另计同量。无原生请求trace／GPU／FPGA执行。[有限合同与数据](iommu-platform/out/r107-bounded-decision-20261012/instance/finite-contract.md)。
- 两序列共享前缀、同可信owner，允许授权单DID合并四池；普通连续run可减至96笔而完整字节不变。live3、恢复保守预留5均容于8槽；不主张容量强迫。663,552B仅为CPU checkpoint代理标记，不是native attention必读量；真实消费时刻未知。
- 普通四完整4MiB／order10池在当前内核大小上合法（只拒order>MAX_ORDER）；八独立2MiB块和合法4KiB回退均保留。四池逻辑9MiB、准备backing16MiB、目标8叶；实际allocation/map/leaf/cache/完整成本未测，8叶<16IOTLB项不证明零miss。[普通合同与成本](iommu-platform/out/r107-bounded-decision-20261012/platform/ordinary-baseline-contract.md)。
- 硬件preissue SRC/DST/LEN/NEXT/DONE已存在；Linux helper缺有限manifest／稳定池／层事件普通adapter，不能当科学损失。logical free不撤整池映射；源码event/refcount已有安全能力。没有普通强对照未覆盖的翻译损失，重复未变源码／CPU回放到此停止。[本轮报告](iommu-platform/docs/r107-bounded-decision-20261012.md)。
- 下一步只在实际Image/DTB/bit/mode/时序身份及明确普通adapter范围成立后，做同一实例一次普通准备／完整完成采证，保留大页／预映射／驻留／批量／授权单DMA。无新输入或普通覆盖则归档此卡；残余可避免翻译损失成立才登记最小G0，之后才G1。下一合同未激活、未登记后继goal。[下一合同](iommu-platform/out/r107-bounded-decision-20261012/next-goal-contract.json)。
- 主平台仍Genesys2／CVA6＋四iDMA＋RISC-V IOMMU＋Linux，服务器辅助。stage106八案和私有credit修复保留、生产未合入；mode0每路3beats／源码mode1前两路512／旧私有512分列，live身份与时序未核。本批root＋两Sol／ultra独占席，零DeepSeek／Claude／Astra、新DUT/build/板测/物理分配/生产修改；root643项、平台200项通过，73旧输入不变。[账本](iommu-platform/out/r107-bounded-decision-20261012/execution-ledger.json)。
- 本新主会话和本轮之前的独占核验子席实际gpt-6.1-sol／ultra、原生500,000／有效475,000已验实；最大支持872,000分列，未改配置。旧258,400历史记录原样保留，不冒称旧席窗口已变。[本次核验](codex-sol-runtime-verification-20261012T072912Z-01a12eff/REPORT.md)。
- 独立上游贡献DS／PTW：用户网页PR52／PR51待审；独立AXI边界修复commit57eb562及816,064模块回归保留，无新板测，CVA6／sources.lock指针未更新。它们不作本轮KV结果。[PR裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)／[AXI验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
'''
unrelated=[line for line in (root/'CURRENT_STATUS.md').read_text().splitlines() if line.startswith('- 独立')]
current='\n'.join(line for line in current.splitlines() if not line.startswith('- 独立上游贡献DS'))+'\n\n'+'\n\n'.join(unrelated)+'\n'
(root/'CURRENT_STATUS.md').write_text(current)
entry='''## 2026-10-12 stage108 R107有限恢复合同与有界立题裁决

实际create_goal登记执行，无额外token预算；按stage107 next-goal-contract完成一份有限合同、普通准备/成本及稳定裁决。决定：条件保留R107有限合同资产，无新G0/G1。具体实例和source-level控制点已成立，缺实例/不可达归档条件未触发；actual普通准备与残余翻译损失仍未知，不能据CPU回放推进方法，也不能声称科学阴性。

完整OPT-125m官方配置固定revision27dcfa74d334bc871f3234de431e71c6eeba5dd6；L12/Htotal12、TP2/Hshard6、D64、B16、fp16。研究选定双序列共享完整前缀＋两不同尾槽，3唯一槽、ref2/1/1、COW0；槽8而live3/保守预留5，无容量强迫。原source类/形状函数＋CPU代理仅一次实际核验：144完整恢复、1,769,472B，既往swapout另计同量；24worker-layer和源保持/未使用槽guard/完整padding通过。六个池原始数据保存，不是native GPU/trace/FPGA完成。

663,552B是代理checkpoint标记字节；root亲核worker最后token于get_len()-1重送与attention等待后更新再读，Sequence长度不证明native KV-valid/必读量。源refcount、同stream与layer-event已做安全，logical free不撤整池映射；双向同期swap禁止。同可信owner允许授权单DMA四池，连续run96笔保留所有完整字节与层屏障。

四池逻辑9,437,184B。普通4×完整4MiB/order10或8×2MiB/order9，均16MiB backing/8目标叶；当前内核order>MAX_ORDER才拒，旧10MiB/order12限制不套本例。4KiB回退、驻留、预映射、批量及单DMA都保留。actual allocation/map/leaf/cache/fence/全成本与liveImage时序未测；IOTLB16/8叶不证明零miss。Linux helper缺普通manifest/stablepool/layer-event glue，硬件实际preissue地址/长度/NEXT/DONE控制已有；未实现adapter/RTL。

root亲核20源及原始旧summary边界，独立raw/源/保护643项通过；平台200项一次通过；73本轮冻结旧输入不变，stage106八案不重跑。旧校验器通用断言/hash实际复用，旧ready0和six-tensor专用events/output不套新KV。来源两web open失败后实际一次revision查找＋一次固定config下载；只新增此primary，不取权重/trace。代理字节scope标签修订未重跑CPU，初次验收恢复保存明确说明非事前快照。

下一合同未激活：实际镜像/时序身份与明确ordinary adapter范围先成立，才做同实例一次普通prepare/full completion采证；无新输入或普通覆盖就归档，不扩场景/压力或猜计算。具体translation残余成立才新G0，G0支持后G1。root＋两独占Sol/ultra席，fork none；无DeepSeek/Claude/Astra/DUT/build/GPU/板连接/真实分配/map/生产改动。新主会话及先前核验子席500k/475k已验实，旧窗口历史不改。本轮结束不代表论文完成。

[报告](iommu-platform/docs/r107-bounded-decision-20261012.md)／[稳定裁决](iommu-platform/out/r107-bounded-decision-20261012/root-result-acceptance.json)／[有限合同](iommu-platform/out/r107-bounded-decision-20261012/instance/finite-contract.md)／[原始数据](iommu-platform/out/r107-bounded-decision-20261012/instance/raw-manifest.json)／[普通成本](iommu-platform/out/r107-bounded-decision-20261012/platform/cost-inventory.json)／[账本](iommu-platform/out/r107-bounded-decision-20261012/execution-ledger.json)／[下一合同](iommu-platform/out/r107-bounded-decision-20261012/next-goal-contract.json)。

'''
def prepend_section(path,text):
 old=path.read_text();cut=old.find('\n')+1
 path.write_text(text+old if old.startswith('## ') else old[:cut]+'\n'+text+old[cut:].lstrip('\n'))
prepend_section(root/'HANDOFF.md',entry)
validation='''## 2026年10月12日 stage108 R107有限合同／普通路径与有界裁决

条件保留有限合同，无新G0/G1、普通失败或净收益验收。实际create_goal登记，无额外预算；完整OPT-125m fixed model + vLLM方法CPU回放一次，3unique slots/144 full jobs/1,769,472B完整恢复及同量prior swapout通过，24worker-layer、源和guard/padding核验。663,552B仅proxy checkpoint有效标记；最后token重送并在attention前覆盖的原文边界已亲核，不作nativeuse/compute/时序证据。

普通4×full4MiB/order10与8×2MiB/order9均大小上可达，16MiB backing/8目标leaf、source IOTLB16；实际allocator/leaf/cache/fence/全成本未测。4KiB、premap/residency/batch/coalescing96jobs、授权singleDID完整合并保留。现helper任意KV/stablepool/layer-event接口缺口是ordinary glue，真实硬件preissue寄存器控制已有；没有新adapter/RTL。CPU、原生GPU与FPGA执行严格分列。

root源/记录/保护643项与platform结构200项均一次通过；73旧输入不变、stage106八案不重跑。原validate_g0_records通用assert/hash实际复用，shape-specific event/data接口因ready0/six-tensor硬编码未套用，无伪造hardwareevents。检查耗时不是硬件performance。零新DUT/build/board/GPU/实际allocation/map/生产改动；root＋两独占Sol/ultra，零DeepSeek/Claude/Astra。一次标签范围澄清原始执行记录保留，无未变CPU重跑。

本批交付结束，next合同未激活：仅新actual身份与ordinary准备数据能解除保留，残余翻译loss成立才G0；本轮不代表论文完成。此前独立新主会话/核验席的500,000原生、475,000有效已验实，[核验报告](../../codex-sol-runtime-verification-20261012T072912Z-01a12eff/REPORT.md)；下方旧258,400记录保留为历史，不改旧运行证据。

[报告](r107-bounded-decision-20261012.md)／[根裁决](../out/r107-bounded-decision-20261012/root-result-acceptance.json)／[应用验收与原始数据](../out/r107-bounded-decision-20261012/instance/acceptance.json)／[root直接验收](../out/r107-bounded-decision-20261012/critical/root-evidence-acceptance.json)／[全成本](../out/r107-bounded-decision-20261012/platform/cost-inventory.json)／[账本](../out/r107-bounded-decision-20261012/execution-ledger.json)／[下一合同](../out/r107-bounded-decision-20261012/next-goal-contract.json)。

'''
prepend_section(plat/'docs/validation.md',validation)
plan=plat/'docs/research-execution-plan-20261011.md';old=plan.read_text();start=old.index('## stage107');end=old.index('## stage106',start)
section='''## stage108有限KV合同与有界裁决（当前入口）

R107条件保留合同资产，没有新G0/G1。完整OPT125m形状与source方法一次CPU核验闭合3唯一槽、144完整恢复/1,769,472B、同量此前swapout和24worker-layer；这不是原生trace/GPU/FPGA。663,552B只proxy标记，nativeuse与时序未验。真实硬件preissue控制已有，Linux普通manifest/stablepool/layer-event glue尚未实现；实现缺口不作科学失败。

普通四完整4MiB/order10或八2MiB/order9、16MiB backing/8目标叶在源码大小上合法；保留4KiB/premap/residency/batch及同owner singleDMA/coalescing96笔。actual分配/leaf/cache/fence/完整成本及ordinary残余翻译loss仍未知，8<16不证明零miss。root643项、平台200项、73旧输入保护通过，旧八案不重跑；零新DUT/build/GPU/board/allocation/map/生产修改，全Codex。

下一仅有前提的同实例普通prepare/full completion采证合同，尚未登记后继goal。先核actual镜像/模式/时序身份与必要ordinary adapter范围，再保存全成本/真实叶/缓存/完成/排空/残余归因；没有这些新输入或普通覆盖就归档R107，不重复源/CPU回放，不加场景/压力/猜计算。具体可避免翻译loss成立才最小G0，G0支持后最多两G1方向。本轮完成不是论文完成。

[报告](r107-bounded-decision-20261012.md)／[稳定裁决](../out/r107-bounded-decision-20261012/root-result-acceptance.json)／[合同和数据](../out/r107-bounded-decision-20261012/instance/finite-contract.md)／[普通成本](../out/r107-bounded-decision-20261012/platform/cost-inventory.json)／[下一合同](../out/r107-bounded-decision-20261012/next-goal-contract.json)／[账本](../out/r107-bounded-decision-20261012/execution-ledger.json)。stage107原文与输入保留在本轮before及原ai-platform-constraints目录；其源级事实未改。

以下stage106及更早条目为历史记录，其中“当前／下一”指当轮；执行状态以本节与CURRENT_STATUS为准。

'''
plan.write_text(old[:start]+section+old[end:])
plan=root/'研究计划.md';old=plan.read_text();start=old.index('当前stage107');end=old.find('\n## ',start)
if end==-1:end=len(old)
# Replace only the two current-stage paragraphs, preserving later sections.
previous='''当前stage107完成平台大页／合并约束与AI恢复依赖审查。现存整槽generic分配超限，但普通二十独立2MiB extent路线几何可达；真实分配／map/cache/完整成本仍未测。固定vLLM的稳定池、完整唯一KV恢复和layer use有来源，logical free不撤映射，现有顺序/event不作新贡献。R107条件保留，无新G0/G1。[本轮裁决与成本](iommu-platform/docs/ai-platform-constraints-20261012.md)。

下一只落实这一卡的一份有限任务／所有权／生命周期合同及普通准备路径；保留合法大页、预映射、批量、驻留／授权单DMA合并，明确研究回放、完整目标和成本。缺具体实例或合法控制点则归档；有准确普通对照未覆盖的可达翻译相关有效路径损失才登记最小G0，随后比较最多两条规则。不猜计算／期限，不扩第三场景或压力，不先新RTL，不恢复服务器GPU主线。[下一goal合同](iommu-platform/out/ai-platform-constraints-20261012/next-goal-contract.json)。'''
# Existing punctuation includes a Chinese enumeration mark between residence and authorization.
lines=old.splitlines(keepends=True);indexes=[i for i,l in enumerate(lines) if l.startswith(('当前stage107','下一只落实这一卡'))]
assert len(indexes)==2,indexes
lines[indexes[0]]='当前stage108完成R107完整OPT125m有限研究回放／所有权／生命周期／字节合同与普通准备接口核验，条件保留合同资产，无新G0/G1。一次source/CPU全字节核验144笔、1,769,472B恢复及同量此前swapout通过；nativeuse/时序及actual FPGA成本仍未知。普通4×4MiB/order10或8×2MiB/order9、premap/residency/batch/coalescing及授权单DMA均保留。[本轮裁决与成本](iommu-platform/docs/r107-bounded-decision-20261012.md)。\n'
lines[indexes[1]]='下一只在actual镜像/时序身份与明确ordinary adapter范围成立后，做同实例一次普通准备／完整完成采证；没有这些新输入或ordinary覆盖即归档，不重复未变source/CPU回放，不扩第三场景/压力或猜计算。残余可避免翻译loss成立才登记最小G0，G0支持后才设计最多两G1方向。下一合同未激活，本轮不代表方法论文完成。[下一合同](iommu-platform/out/r107-bounded-decision-20261012/next-goal-contract.json)。\n'
plan.write_text(''.join(lines))
record={'stage':108,'updated_at_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'files':[{'path':str(p),'before':str(out/'before'/n),'before_sha256':hashlib.sha256((out/'before'/n).read_bytes()).hexdigest(),'after_sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p,n in files],'standalone_report':str(plat/'docs/r107-bounded-decision-20261012.md'),'stable_acceptance_written_before_documentation':True,'existing_history_preserved':'HANDOFF and validation current bodies including concurrent Linux contribution entries retained unchanged; all independent CURRENT_STATUS bullets retained verbatim. Initial and concurrent before snapshots saved. Execution plan stage107 section kept in initial before; total plan changed only two current paragraphs.'}
(out/'documentation-write-record.json').write_text(json.dumps(record,ensure_ascii=False,indent=2)+'\n')
print('SYNCHRONIZED',len(files),'shared documents plus standalone report')
