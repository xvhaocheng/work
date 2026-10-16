# 正常再用机会与现成观测：有界来源合同交付

2026-10-16 JST。MAP-REUSE-OPPORTUNITY-1已完成源码来源、观测、配对与CPU合同。选定cyclic、linear、native-XDP、order0子切面每次RX allocation一页一个packet位置，重复B映射须跨真实recycle/再次alloc。源码支持正常再用机会及受限/退出条件；频率、完整CPU和收益未取得，native统计为null，不能写成0。

若一allocation一packet、单次B redirect、无clone/retry、调用来源/记录唯一性及完整source/config epoch均已证明，两次成功B map可推出跨recycle再用，合格L0可先粗筛，不必先建完整逐alloc/CQ collector。L0不等accept、CQ、last-ref或CPU；redirect入队、batch sent不能逐frame配对。 hold/release不记录每次alloc/recycle，source release不等物理free。page map/unmap事件均在backend后，两post时间戳间距不是完整CPU；SG unmap在前不可混用。零丢失仍可能有IOVA复用歧义，丢失、ABA与删失须隔离；未测值为unknown/null。

本机一次只读身份检查为6.8.0-60-generic x86_64，可见物理netdev驱动e1000e/cdc_ncm、mlx5计数0，tracefs元数据读取EACCES。仅限当前可见sysfs，非全PCI/其他命名空间/远端清查；EACCES不等内核无功能，IOMMU group不证明有效translated/shared服务，当前身份不匹配固定v6.12链。

V1-N/K继续作为条件低成本原生整合假设：固定B/config、K预留lazy sparse关联、满额O0、无独立idle eviction，frame/全部片段保护在途。source最后frame/片段归还后撤旧B图；target关闭另需禁新、CPU reader drain与真实DMA drain，竞争须唯一撤图。普通O3同接口等价只判策略增量，R是条件普通构造而非已发表网络实现；未证独立创新、净收益或共享IOMMU干扰，未选论文主线。 相同工作/服务下CPU或不可回收内存改善可作主要价值；实际每策略S/P/C/Q物理并集与分配反馈、metadata/PTE、IOVA、带宽和有效完成分别报告。stock也无idle B pin，V1无天然payload RAM节省；K仅界关联库存/Q，不保证总RAM差或时限。O0轨迹不能直接作改变回收策略后的候选轨迹。

下一MAP-EVIDENCE-INPUT-1已指定、未执行：只定向定位并验收已有匹配原始输入及构建/设备配置，先核身份再算合格L0。无匹配输入保留条件界与精确缺项，不重复同合同/原语表/旧阈值，不自动开发collector、驱动、模型或板测，也不等一般授权或用户保证痛点。

| 内容 | 入口 |
| --- | --- |
| root稳定接受 | [裁决](root/decision-analysis.md)／[稳定接受](root/root-result-acceptance.json)／[证据合同](root/evidence-contract.json)／[CPU与机会界](root/cpu-and-opportunity-contract.md) |
| 来源再用 | [source分析](reuse/source-opportunity.md)／[机会卡](reuse/opportunity-card.json)／[来源清单](reuse/source-manifest.json) |
| 现成事件 | [观测合同](observation/observation-contract.md)／[事件矩阵](observation/event-matrix.json)／[来源](observation/source-manifest.json)／[3份HTTP回执](observation/download-receipts.json) |
| 输入与执行 | [下一合同](root/next-minimal-contract.json)／[只读host身份](root/host-readonly-identity.json)／[已查范围](root/scoped-asset-audit.json)／[来源核验](root/source-verification.json)／[执行账](root/execution-ledger.json)／[同步合同](root/canonical-sync-contract.json) |
| 文档回执 | [即时完整快照清单](documentation/before-write-manifest.json)／[更新账](documentation/update-ledger.json)／[验收](documentation/verification.json)／[变更清单](documentation/changed-files.json) |
| 前轮历史 | [整合差项](../mapping-integration-delta-20261016/README.md)／[寿命设计](../mapping-lifetime-design-20261016/README.md)／[接纳账](../mapping-admission-20261016/README.md)／[整页历史](../mapping-organization-source-20261016/README.md)／[B1](../b1-stripe-cost-20261016/README.md) |

本轮新增3份固定commit trace header HTTP，19项worker来源hash/58项行界核验通过，含root共16个唯一路径；只核来源定位。1次只读host元数据检查，无sudo/mount、trace enable、probe attach、流量或设备写；0原生工作负载、0新模型/旧账重跑、0构建、0生产Linux/RTL/ABI编辑及板测。 共享研究Codex-only，未用DeepSeek/Claude；请求source再用Sol/xhigh、trace源核Sol/high、文档Sol/medium，实际独立模型/强度元数据unknown，未声称切换root或改模型规则。 B1具体残余/成本证据触发、I not_ready、D既有版本边界、A5/A2及独立工程保持；MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集、实际债务unknown。10月15—28日窗口不重启，不自动升位其他分支。

文档席未重做来源研究或执行原生测量。历史全文、两AGENTS与冻结研究产物保护；14个既存历史document-target坏链照录，未修历史，也不声称原来有效。
