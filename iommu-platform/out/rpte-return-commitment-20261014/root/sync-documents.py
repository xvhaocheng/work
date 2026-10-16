"""One authorized documentation synchronization, retaining exact entry archives."""
from pathlib import Path
import hashlib
import json

W=Path('/home/xvhaocheng/work')
O=W/'iommu-platform/out/rpte-return-commitment-20261014'
manifest=json.loads((O/'before/manifest.json').read_text())
for row in manifest:
    old=(O/row['archive']).read_bytes()
    assert hashlib.sha256(old).hexdigest()==row['sha256'],row['path']
    assert (W/row['path']).read_bytes()==old,('input changed since archive',row['path'])

current="""# 当前状态：R/PTE真实返回封口实例未准入（2026-10-14）

- source gate已完成：正常1D RREADY/本源W drain、fresh NormalTL/下一PTE/heldR、full source身份和完整R/W/B退休已亲核。
- private sole对齐4KiB/512拍不能以自身容量溢出制造heldR；生产小源3+明确RTL8槽仅条件上界。DEFAULT共同fresh前缀unknown，无实际四源或板端deadlock声明。
- 同bank `[危险READ2, owner0所需READ0]` 条件cut在两bank AB/BA下均无finite全部完成；唯一安全头先由冻结O-envelope-select表达。实际timing/数字成本等价未证，F1全族/双安全头服务窗口域仍开放。
- 0新模型/四条件/仿真/RTL/test/build/综合/板、0旧实验/validator重跑，无生产变更、额外压力/容量或关闭分支恢复。新增成本/面积/Fmax/应用null。
- 下一门只接两安全头PTE出生/服务窗口的新source事实和一个完整普通控制不能覆盖的完整收益见证；通过才私有原型≤4条件，再GPU/AI工作流与FPGA。8/9月资产保留，原窗口累计，论文未完成。
- actual goal无额外预算，root+2新独占席实际6.1-sol/ultra/native有效475000、forknone/Codex-only；五文档与交付核验完成后才actualcomplete，以原样工具回执为准。

[本轮报告](iommu-platform/docs/rpte-return-commitment-decision-20261014.md)／[稳定裁决](iommu-platform/out/rpte-return-commitment-20261014/root-result-acceptance.json)／[证据导航](iommu-platform/out/rpte-return-commitment-20261014/README.md)／[下一门](iommu-platform/out/rpte-return-commitment-20261014/next-goal-contract.json)／[交付核验](iommu-platform/out/rpte-return-commitment-20261014/root/delivery-verification.json)／[完成原回执](iommu-platform/out/rpte-return-commitment-20261014/root/goal-completion.json)。入口原件在本轮before目录。

"""
marker='- 独立Linux贡献：'
old=(O/'before/CURRENT_STATUS.md').read_text()
assert old.count(marker)==1
tail=old[old.index(marker):]
(W/'CURRENT_STATUS.md').write_text(current+tail)

body="""## 2026-10-14 R/PTE真实返回封口具名实例裁决

执行批准的source gate并登记actualgoal，无额外token预算。正常1D Rready消费端恒1，buffer仅本完整source W握手排空；production四源ID0/4/8/12（DDR32/36/40/44）、bank0={0,2}/bank1={1,3}，区别private两源。每bank原生4 outstanding/16 W FIFO，RAWcoupler关闭；DEFAULT成对metadata不是firstR→AW或任意全局前缀证明。DSpriority每拍可提升已DDR接受DS，不能提升未生成PTE或撤回heldR。

K0 private sole对齐4KiB/512拍排除自身容量溢出held正例；不缩buffer造收益。K1生产3拍+明确RTL8返回槽仅条件容量上界；bank0 `[READ2, 解锁owner0所需READ0]` cut下AB/BA均不能完成全部R/W/B/faultquiet，另bank先TL/R不是完整收益。实际DEFAULT共同fresh前缀仍unknown，无实际生产或板端deadlock声明。K2唯一安全头先由单个完整O-envelope-select表达，缺证不当危险；实际select时延/数字成本未冻结，不宣称逐拍/全部费用等价或整个F1发表覆盖。

本轮F1具名held-return实例未准入；全族及两安全头服务窗口域保留。完整ID、同初态AB/BA偏序、carried owner债务、各级PTE接受/首次RVALID/held、R/W/B/error/quiet与所有unknown已落档。原九在线普通合同/37费用分类复用，normal准备/envelope/reservation/coupler从合法同origin完整计费；当前新增数字成本/面积/Fmax/应用null。0新模型/microbenchmark/仿真/test/RTL/build/综合/板，0旧实验/validator重跑，无生产变更、额外压力/容量，不恢复R2/R107/C112或旧席。

root及两个新独占席实际gpt-6.1-sol/ultra/native有效475000、forknone/Codex-only。下一门只接新source支持的两安全头PTE出生/服务窗口完整选择差异，与已冻结short/debt/owner/release/two-stage等单个完整在线规则比较；通过才一原型≤4新条件，再核GPU/AI共享路径和FPGA。不自动新goal/扩矩阵/更改论文定位，8/9月资产保留，原立题窗口累计，论文未完成。

REPORT_LINKS

稳定root裁决和五文档同步后，先做本轮新交付核验，再actualgoal complete；实际工具用量见原样回执。以下原样保留入口全部历史，旧“下一步”只指当时状态。

"""
roots={'HANDOFF.md':'# 阶段交接：R/PTE真实返回封口裁决',
       '研究计划.md':'# 研究总计划：R/PTE真实返回封口裁决',
       'iommu-platform/docs/research-execution-plan-20261011.md':'# 当前执行：R/PTE真实返回封口裁决',
       'iommu-platform/docs/validation.md':'# 最新验证记录：R/PTE真实返回封口裁决'}
for row in manifest:
    path=row['path']
    if path=='CURRENT_STATUS.md':continue
    if path.startswith('iommu-platform/docs/'):
        links='[本轮报告](rpte-return-commitment-decision-20261014.md)／[稳定裁决](../out/rpte-return-commitment-20261014/root-result-acceptance.json)／[证据导航](../out/rpte-return-commitment-20261014/README.md)／[下一门](../out/rpte-return-commitment-20261014/next-goal-contract.json)／[交付核验](../out/rpte-return-commitment-20261014/root/delivery-verification.json)／[actualgoal原回执](../out/rpte-return-commitment-20261014/root/goal-completion.json)。'
    else:
        links='[本轮报告](iommu-platform/docs/rpte-return-commitment-decision-20261014.md)／[稳定裁决](iommu-platform/out/rpte-return-commitment-20261014/root-result-acceptance.json)／[证据导航](iommu-platform/out/rpte-return-commitment-20261014/README.md)／[下一门](iommu-platform/out/rpte-return-commitment-20261014/next-goal-contract.json)／[交付核验](iommu-platform/out/rpte-return-commitment-20261014/root/delivery-verification.json)／[actualgoal原回执](iommu-platform/out/rpte-return-commitment-20261014/root/goal-completion.json)。'
    prefix=roots[path]+'\n\n'+body.replace('REPORT_LINKS',links)
    (W/path).write_bytes(prefix.encode()+(O/row['archive']).read_bytes())

# Focused navigation correction after the root initially quoted a nonexisting old filename.
costpath=O/'root/observation-ownership-cost-card.json'
cost=json.loads(costpath.read_text())
cost['existing_full_cost_reference']='../tl-memory-feedback-20261014/ordinary-cost/cost-interface-card.md and ../r2-ownership-cost-20261013/root/full-cost-comparison.json (37 categories only; no old candidate architecture or numeric total inherited)'
costpath.write_text(json.dumps(cost,ensure_ascii=False,indent=2)+'\n')
(O/'root/document-sync.json').write_text(json.dumps({'schema':'rpte-document-sync-v1','documents':[r['path'] for r in manifest],'archives_exact':True,'four_detailed_documents_keep_old_bytes_as_suffix':True,'CURRENT_independent_tail_exact':True,'actual_goal_complete_called':False},ensure_ascii=False,indent=2)+'\n')
print('Five documents synchronized once; entry archives and independent status tail retained.')
