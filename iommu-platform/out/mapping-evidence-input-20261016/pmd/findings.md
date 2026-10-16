# MAP-EVIDENCE-INPUT-1：DMA_PMD 现成输入核验

结论：本席未取得可准入固定网络切面的 L0 原始输入，也没有完整 CPU 输入。RFC 00/22 的现成内容是作者自报的 ARM 双向吞吐量及 SMMU 聚合计数，缺少逐次 B map、source/config episode、运行身份及 CPU 执行边界。不能把这些数值重构为跨 recycle 再用率或完整 CPU；原生统计保持 null，不写成零。此结论仅覆盖以下明确来源；lore 正文因访问验证未取得，第二条 PMC 外链由 root 独占核查，均不据此宣称全球不存在数据。

本席请求 Sol/high，实际独立模型与推理强度元数据 unknown。共享研究 Codex-only，未用 DeepSeek/Claude。采用 ARS deep-research/fact-check 的来源核查方法，只判断所查工程证据的身份及准入，不套医学分级或期刊掠夺性检查。作者报告自身实现和结果，属于自报聚合证据；本轮未独立复现或新增交叉性能验证。

## 已取得输入及判定

唯一含运行数字的来源为旧目录的 [rfc00.txt](/home/xvhaocheng/work/iommu-platform/out/mapping-organization-source-20261016/dma-pmd/rfc00.txt:92)，原始归档是 [RFC 00/22](https://lists.openwall.net/linux-kernel/2026/10/03/1066)。本轮复核全文 1–264 行、正文全部外链以及表格/patch 清单；没有重读全部 patch 或驱动。HTML 原始 17,359 bytes，SHA256 `d54460e2a532bd265088e9e35adf32c2c4739e8d52e03a30356e2446757c8655`，txt 13,087 bytes，SHA256 `7c37f40c618a4ef5c1dd32a0d86796cefe39a0eff519a685f97eb73cb666d064`，均与前轮 manifest 一致，未重复下载。

| 具体输入 | 实际提供 | 本次准入 |
|---|---|---|
| rfc00.txt:94–112 的作者性能样例 | large ARM、2×200G NIC、双向流量；DMA-FQ 约440Gbps、严格 DMA 上 DMA_PMD 约745Gbps；各自 PMCG event0x80 lookups 与 event0x81 misses | 仅可作该作者聚合样例参考。无逐次 map 成功、PFN/netmem/source 代际或配对；无完整 CPU；不准入 L0/CPU |
| rfc00.txt:132 的 lore 引用 | 明确背景邮件 URL；本次仅得到访问验证 HTML | 正文、构建、driver/workload、字段、损失/删失及配对均 unknown；不准入，并保持未核 |
| rfc00.txt:133 的 PMC 引用 | 明确背景论文 URL | root 独占，PMD 席未访问；不重复论文席或扩大调研 |
| rfc00.txt:205 的 patch22 名称 | statistics/debugfs 的代码功能指针 | 封面未提供该功能输出的运行 dump；功能名不等原始输入，本轮未打开新 patch |

封面写明 larger TX segments 可以减少 lookups（:108–109）。故这些 lookup/miss 与吞吐量变化不单独归因于 B map CPU，也不能识别 A→B 页面跨 recycle 行为。补丁列表中出现 mlx5e（:202），不足以证明性能样例使用 mlx5e，更不足以建立 v6.12/native-XDP/cyclic/linear/order0 身份。

## 固定身份的缺项

冻结身份为 Linux v6.12 mlx5e native-XDP cyclic linear order0，A 接收后经 FIB 到 B redirect，或条件本地 UDP，非 XSK/clone/retry。该封面没有给出运行 kernel commit/config、准确 NIC/driver/firmware、设备/domain/backend、XDP/RQ 几何、PAGE_SIZE、流量生成/协议/FIB 路径及启动/终止窗口。

L0 缺少归属 B 调用点的成功/失败 map 记录、PFN/netmem 与 pool/source/config epoch、事件唯一性/时钟/位宽、丢失统计和左右删失。完整 CPU 还缺少调用点 entry/return、CPU 执行时钟、IRQ/抢占及 tracing 开销，以及 map/unmap/sync、setup/lookup/fallback、供给/退役/worker/Q/尾部与有效完成口径。因此不计算 r、完整 CPU、有限 K 收益或净收益。

## 一层引用的实际访问结果

正文只有两条外链，来自封面 :132–133；[cover-body-links.json](cover-body-links.json) 保存枚举结果。没有发现该封面正文中的作者 repo、独立原始日志、数据包或可见附件链接；这是对该保存归档的检查，不是原始 MIME 全附件或全网存在性证明。文中 Documentation/core-api/dma-pmd.rst 是系列内源码路径（:120–122），不构成已提供的原始运行数据。

lore 邮件 [HTML](https://lore.kernel.org/all/4b42f2eb-dc29-153e-ace9-5584ea2e5070@redhat.com/) 的 web.open 返回不可访问，工具未提供 HTTP 状态。随后对同一邮件 HTML 及其 `/raw` 表示各做一次直接 GET，HTTP 均为 200，但 Content-Type 为 text/html、响应为 Anubis 验证页，不是邮件。两份响应均 7,688 bytes；SHA256 分别为 `02d5f7375a1e410dd2a9bb6d04cbd4a9b553994e1b065b7f32e1501f7edbc5c3` 与 `b1ddadc5787941bc1df060520e52efb805890dd6ee3afb8bf317b36ccf80b7ec`。收据明确区分 transport_success 与 source_content_obtained=false。

仅核了验证页标题/标志及内容类型，没有执行页面脚本、通过挑战、追第三方链接或寻找新文献。邮件内部是否另有原始数据/作者 repo 保持 unknown；访问失败不等于没有数据。

[输入卡](input-card.json) 按具体输入记录准入理由和字段缺项，[来源清单](source-manifest.json) 记录旧 raw 与新响应 bytes/hash、来源链及实际检查区间，[HTTP 收据](http-receipts.json) 保留每次真实结果。新增 1 次 web 尝试、2 次直接 HTTP GET；0 artifact 执行、0 clone、0 本机 probe/采集/驱动/collector/模型/构建/板测。只写本轮 pmd/，canonical 与旧 raw 均未编辑。
