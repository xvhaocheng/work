# 创新审查再推进：证据与审定

日期：2026-10-06。最终结论以 [Codex 审定](../../docs/innovation-followup-20261006.md) 为准。模型原答复保留用于溯源，不代表其中每个判断被接受。

## 工作范围

- 成功读取 ShareMMU 官方 v3 artifact 的相关压缩字节区间并提取 20 份源文件，未下载整包、未构建或执行它。
- 核查替换计分、翻译请求／返回、memory↔scratchpad DMA、在途限制与完成回调；Codex 直接核论文、关键源码及当前 IOMMU AW/AR 路径。
- 原读返回循环被普通返回空间预留排除；新的提前 AW 循环仍是条件假设。普通逐块成对预翻译是必须比较的强基线，没有确认新机制。
- 没有修改生产 RTL/Linux、运行新测试／仿真／构建、操作板卡或重做限速。没有调用 Claude 或新 Codex 子 agent。

## 模型记录

| 任务 | 完整输入 | 原始返回 | 可读答复 | provider tokens |
|---|---|---|---|---:|
| artifact 替换与 DMA 路径 | [JSON](artifact-review-input.json) | [JSON](artifact-review-raw.json) | [正文](artifact-review.md) | 117002 |
| 完成回调及基线核查 | [JSON](completion-review-input.json) | [JSON](completion-review-raw.json) | [正文](completion-review.md) | 171072 |
| 两个条件环与普通配对方案的窄修订 | [JSON](pair-review-input.json) | [JSON](pair-review-raw.json) | [正文](pair-review.md) | 86827 |

三次均为 `deepseek-flash`、thinking enabled/high，requested/effective maximum 393216，`truncated=false`、`finish_reason=stop`。合计 374901 provider tokens（含输入、输出，具体 reasoning 见原始返回），不是 Codex token 用量或价格。DeepSeek 没有自行执行、联网或修改文件。根执行工具并作关键审查和最终接受决定。

[audit.json](audit.json) 保留用量与重要纠正。特别是：回调参数顺序并无已证错误；`pendingOpposite` 单独不构成环；同／不同 VPN 不是有效的新信息反事实；单个 copy 不等于只能有一个潜在翻译 miss；部分配对预留也必须可取消或避免引入另一等待环。

## 来源与局限

[web-evidence.json](web-evidence.json) 保存本轮检索与官方页面读取。检索命中不自动计入接受的证据；最终报告只采用直接有关的主来源。[artifact-manifest.json](artifact-manifest.json) 记录提取文件、字节数和本地 SHA256。原始代码位于 [artifact/](artifact/)。保留上游原文件与版权声明。

官方概念 DOI：<https://doi.org/10.5281/zenodo.21543532>；本轮解析记录：<https://zenodo.org/api/records/22681559>。ZIP 名 `sharemmu-micro59-ae-v3.zip`，metadata 大小 132141444 B。元数据／中心目录复用[上轮记录](../innovation-focus-20261006/README.md)。本次官方 content 入口对以下范围均返回 HTTP 206（inclusive offsets）：

| 内容组 | 开始 | 结束 |
|---|---:|---:|
| ProtectionMemobj.cc | 77766526 | 77768877 |
| DMAController | 77733709 | 77740174 |
| ProtectionMemobj.hh/Protectiontable.py | 77768878 | 77772834 |
| NetworkInterrupt | 77823299 | 77839480 |
| LCAccDevice/commands | 77746253 | 77758907 |
| DMAEngine/SimicsInterface/prefetcher | 77884888 | 77893722 |
| Ruby System | 77530160 | 77542699 |
| TransferDescription | 77730318 | 77731454 |

没有验证整包 MD5，也不把本地 SHA256 称为上游独立认证。源码范围审查不能排除未审变体或其他路径；未运行论文 artifact、未复现其性能，也没有证明论文或当前板卡死锁。

收尾核对：新审定正文和本记录的本地引用均可达；20 份提取文件的大小／CRC 与 ZIP 中心目录一致。[证据检查](evidence-check.json)只确认提取和文档可用，不是平台测试或创新验收。

补核的最强近邻包括 MMU-aware DMA 2018 的预锁／重放、Arm DTI 的进度要求、HotNets 2022 的 IOTLB 延迟与有限 PCIe 信用，以及 MASK 的既有关键性措施。主文献库的 123 项计数保持，网页读取不冒充新增全文入库。
