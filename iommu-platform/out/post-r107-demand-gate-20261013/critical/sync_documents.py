#!/usr/bin/env python3
"""One final stable documentary reconciliation; retain all older history."""
from pathlib import Path
import json, hashlib

W = Path('/home/xvhaocheng/work')
P = W/'iommu-platform'
O = P/'out/post-r107-demand-gate-20261013'
manifest = json.loads((O/'before/document-manifest.json').read_text())
for r in manifest:
    assert hashlib.sha256(Path(r['path']).read_bytes()).hexdigest() == r['sha256'], ('concurrent modification',r['path'])

status_path = W/'CURRENT_STATUS.md'
old = status_path.read_text()
marker = '\n- 独立Linux贡献'
assert marker in old
independent = old[old.index(marker):]
status = '''# 当前状态 stage112 2026年10月13日 新增消费需求裁决完成

- 整体仍阶段一立题。真实goal已登记、无额外预算，本轮交付齐备；归档C112逐层归并的当前FPGA翻译缺口投影，R107原有限卡继续归档，无G0/G1，方法论文未完成。[稳定裁决](iommu-platform/out/post-r107-demand-gate-20261013/root-result-acceptance.json)。
- 新核固定vLLM67d96c29的RowParallel/GraphAllReduce：12层两处归并、持久GPU buffer复用，完整容量M未给。真实任务存在，但原板joint layer仅全恢复后CPU字节检查，不是attention/collective；新的partial数据、物理requester/共享翻译路径与完成接口未成立。[来源、合同与边界](iommu-platform/docs/post-r107-demand-gate-20261013.md)。
- 同两序列decode的源码形状为8 padded行×768×2B＝每rank12,288B，两活动prefix24,576B；24次rank逻辑输入合计589,824B，独有有效归并结果73,728B。wire/DMA/计算/到达/期限均未验。若新合同授权既有GPU池backing slack，可放在原leaf内，无新增叶；只作普通布局见证，未执行，也不覆盖未知M/全模型资源。
- 既有实板ordinary资产保留：4×4MiB/8真实2MiB叶；priorout/restore各1,769,472B，恢复1728hit/0miss/0walk、完整字节/quiet/checked释放通过。旧成本、假设和来源界限不改；不推C112性能。[原始接受](iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/validation/actual-finite-pilot-validation.json)。
- 归档后路线回phase1来源/需求选择：一个明确的自然独立搬运、实际共享路径及普通控制外新事实先成立，再G0；不扩当前参数/压力、不恢复关闭场景、不建collective模拟器。[inactive下一合同](iommu-platform/out/post-r107-demand-gate-20261013/next-goal-contract.json)。
- 旧三席历史FINAL均已完成、12关键产物入口保持原路径，当前仍pending_init；当前无close接口，未中断/归档/关闭或声称释放，占用原因未知。新席spawn失败一次后root完成核查；若后续确需独占席，先保存交接进入新主会话，保持Sol/ultra。[原始生命周期核验](iommu-platform/out/post-r107-demand-gate-20261013/validation/agent-lifecycle-review.json)。
- 19旧关键输入保留；四个直接依赖取自原官方archive。零生产/config/cache/DB/history改动、板命令、新build/RTL/旧回放/校验器重跑。Codex-only，无DeepSeek/Claude/Astra；本轮未重新核板端身份，最后已验状态见stage111。[账本](iommu-platform/out/post-r107-demand-gate-20261013/execution-ledger.json)。
'''
status_path.write_text(status+independent)
assert status_path.read_text().endswith(independent)

brief = 'stage112新增需求裁决完成：归档C112当前FPGA翻译缺口投影，R107继续归档，无G0/G1。固定vLLM TP2每层attention输出/MLP输出共24次RowParallel归并、GraphAllReduce持久buffer已亲核；同两序列decode形状每rank活动12,288B/两prefix24,576B，授权普通布局可放既有backing slack原leaf，未执行且完整M未知。原板24worker-layer/12joint只是全恢复后CPU字节检查，新的partial输出/collective完成与实际共享requester未成立。保留合法大页/premap/residency/batch/singleDMA及完整准备、计算、归并、释放成本；wire/compute/arrival/deadline和新translation残余未知，不填0。不扩场景/压力、不重跑旧回放/旧校验器，不做实现/build/板动作。实际goal登记无额外预算；本轮交付齐备，论文仍立题。'
agent = '用户补充顺序已核：旧r109_artifact_identity/r111_build_staging/r111_ordinary_interface_plan原始FINAL分别09:07:32/12:13:23/12:10:57UTC，历史任务完成且无后续dispatch，12关键产物保留。当前三席仍pending_init，协作close不可用；archive不当作close，零interrupt/archive/关闭/释放断言，无占用原因猜测或配置修改。root已完成关键任务，本轮无需换主会话；真需独占席时以保存交接进入新主会话，继续Sol/ultra。'
handoff = W/'HANDOFF.md'
handoff_text = '# 2026年10月13日 stage112新增消费需求／普通边界裁决\n\n'+brief+'\n\n'+agent+'\n\n[报告](iommu-platform/docs/post-r107-demand-gate-20261013.md)／[裁决](iommu-platform/out/post-r107-demand-gate-20261013/root-result-acceptance.json)／[源码与原数据](iommu-platform/out/post-r107-demand-gate-20261013/critical/root-source-review.json)／[字节合同](iommu-platform/out/post-r107-demand-gate-20261013/source/finite-consumer-byte-contract.json)／[旧席原证据](iommu-platform/out/post-r107-demand-gate-20261013/validation/agent-lifecycle-review.json)／[新主会话入口](iommu-platform/out/post-r107-demand-gate-20261013/new-primary-handoff.md)／[下一合同](iommu-platform/out/post-r107-demand-gate-20261013/next-goal-contract.json)。\n\n下面历史内容原样保留。\n\n'
handoff.write_text(handoff_text+handoff.read_text())

validation = P/'docs/validation.md'
validation.write_text('# stage112新增消费合同与FPGA进入边界（2026年10月13日JST）\n\n'+brief+'\n\n'+agent+'\n\n[报告](post-r107-demand-gate-20261013.md)／[root直接核源](../out/post-r107-demand-gate-20261013/critical/root-source-review.json)／[原始R107字段](../out/post-r107-demand-gate-20261013/critical/existing-raw-evidence.json)／[原始E3有限因果对](../out/post-r107-demand-gate-20261013/critical/existing-e3-boundary.json)／[交付/保护核验](../out/post-r107-demand-gate-20261013/validation/delivery-check.json)／[账本](../out/post-r107-demand-gate-20261013/execution-ledger.json)。以下旧验收原样保留。\n\n'+validation.read_text())

plan = W/'研究计划.md'
txt = plan.read_text();lines = txt.splitlines()
assert lines[4].startswith('当前有界进展stage111：')
lines[2] = '更新：2026-10-13（2026-10-11发起批次）。目标：有独立增量的首篇方法论文；先用约两周完成立题判断，通过后在现有 FPGA 上闭合关键验证。'
lines[4] = '当前有界进展stage112：'+brief+' 原生collective需求合同保留为资产，这项当前FPGA方法投影结束；下一回phase1有界来源/需求选择，以真正新增搬运/共享路径与普通控制外事实为入口。旧三席任务已完成、产物保留，close接口不可用，未释放/重建，不降Sol/ultra。[报告](iommu-platform/docs/post-r107-demand-gate-20261013.md)／[稳定裁决](iommu-platform/out/post-r107-demand-gate-20261013/root-result-acceptance.json)／[下一合同](iommu-platform/out/post-r107-demand-gate-20261013/next-goal-contract.json)。'
txt = '\n'.join(lines)+'\n'
stale = next(s for s in txt.splitlines() if s.startswith('stage110的下一合同已由stage111真实goal承接：'))
txt = txt.replace(stale,'历史stage110→111的部署合同已实际完成：已授权写SD、匹配r1真实boot、同有限任务一次完整普通恢复及全成本通过，R107卡归档；不存在当前待批SD或当前blocked状态。stage112已在其后完成新增消费需求裁决，C112当前FPGA投影也结束。后续回阶段一的有界来源/需求选择，不自动恢复旧卡、增加压力或搭新系统。[stage111真实交付](iommu-platform/docs/r107-ordinary-pilot-20261012.md)／[stage112当前结果](iommu-platform/docs/post-r107-demand-gate-20261013.md)。')
plan.write_text(txt)

execution = P/'docs/research-execution-plan-20261011.md'
txt=execution.read_text();anchor='## stage111普通实板恢复／全成本裁决'
assert anchor in txt
idx=txt.index(anchor)
section='## stage112新增消费需求关（2026年10月13日JST，交付齐备）\n\n'+brief+'\n\n'+agent+'\n\n[报告](post-r107-demand-gate-20261013.md)／[根裁决](../out/post-r107-demand-gate-20261013/root-result-acceptance.json)／[新增依赖原文](../out/post-r107-demand-gate-20261013/source/new-primary-source-manifest.json)／[下一进入合同](../out/post-r107-demand-gate-20261013/next-goal-contract.json)。下面stage111及更早条目是历史记录，当前以本节和CURRENT_STATUS为准。\n\n'
execution.write_text(txt[:idx]+section+txt[idx:])

handoff_new = '''# 后续确需独占席时的新主会话交接

当前root已完成stage112关键核查，不需要为补例行复审换主会话。本文件只在后续确有独占关键问题且原主会话无法释放席位时使用；不自动创建会话或研究goal。

- 工作区/home/xvhaocheng/work；先读适用AGENTS和CURRENT_STATUS；全Codex，Sol gpt-6.1-sol/ultra，fork_turns=none提供独占任务必要上下文，不调用DeepSeek/Claude，不降强度，不改模型/上下文配置。
- 整体阶段一立题，方法论文未完成。R107有限恢复归档：普通8个真实2MiB叶，恢复1728hit/0walk，全字节/DONE/quiet/checked release已接受；不重跑stage106/108或身份审计。
- stage112亲核固定vLLM67d96c29的RowParallel/GraphAllReduce：TP2每层两个归并位置，持久GPU buffer；真实消费者不是原FPGA CPU检查代理。当前C112 FPGA翻译缺口投影归档，未进入G0/G1，未实现/测新的collective。
- 只有新的自然独立搬运/实际共享路径、完整owner/lifetime/bytes/ready/cost和普通大页/premap/residency/batch/合法singleDMA未覆盖事实成立，才进入最小G0；不恢复关闭场景或人为增加容量/设备隔离/时序压力。
- 三旧席历史任务完成、12关键产物保留；当前pending_init原因未知。本接口无close能力，interrupt不是close，archive未证明释放。零线程关闭/释放实证；真需新席时在新主会话明确指定Sol/ultra，再直接核实际执行证据，不仅看创建参数。
- 原board最后已验r1boot370a9dd6-7d1a-4190-8395-acaf3e25f69c/kernel6.5.0-rc2-r107-ordinary-stage111-r1/f9bit/S0；本轮没有访问板，不把这个旧状态说成当前新读取。不得自动重启/刷写或开始新任务。

导航：本目录root-result-acceptance.json、next-goal-contract.json、source/finite-consumer-byte-contract.json、critical/root-source-review.json、critical/existing-raw-evidence.json、validation/agent-lifecycle-review.json及../../../docs/post-r107-demand-gate-20261013.md。完整旧记录原路径保留；摘要不能代替关键原文/原数据。
'''
# Relative docs route from this artifact root is ../../docs, not ../../../docs.
handoff_new=handoff_new.replace('../../../docs/','../../docs/')
(O/'new-primary-handoff.md').write_text(handoff_new)

accept=json.loads((O/'root-result-acceptance.json').read_text());accept['deliveries']['shared_doc_sync']=True
(O/'root-result-acceptance.json').write_text(json.dumps(accept,ensure_ascii=False,indent=2)+'\n')
ledger=json.loads((O/'execution-ledger.json').read_text());ledger['shared_doc_sync']=True;ledger['shared_document_initial_write_batches']=1
(O/'execution-ledger.json').write_text(json.dumps(ledger,ensure_ascii=False,indent=2)+'\n')
print('five shared docs and focused handoff saved; independent CURRENT_STATUS tail retained')
