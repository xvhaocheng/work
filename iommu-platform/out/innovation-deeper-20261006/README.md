# 实际路径与更弱完成条件：审查记录

日期：2026-10-06。最终结论见[审定说明](../../docs/innovation-deeper-20261006.md)。本轮没有生产代码改动、新测试／仿真／构建、板测或限速实验。没有 Claude 或新 Codex 子 agent 调用。

## 结果与证据

实际顶层关闭 R→AW coupler，普通 copy 可以先发目的物理 AW、随后才完成源读翻译；banked 的读准入不排除这个前缀。当前 CQ PR/PW 排空仍为 TODO，没有证明完整更新等待环。进一步给出限定物理请求排空模型内的普通“读返回信用＋写数据依赖准入”条件，说明不必总在读之前拿齐两端翻译；不是新机制或性能验收。

| 委派 | 输入 | 原始返回 | 可读正文 | provider tokens |
|---|---|---|---|---:|
| 请求／完成链及成本清单 | [输入](path-review-input.json) | [原始](path-review-raw.json) | [正文](path-review.md) | 169028 |
| 补实际配置、coupler、banked并定向纠正 | [输入](revision-input.json) | [原始](revision-raw.json) | [正文](revision.md) | 139523 |

两次均 `deepseek-flash`、thinking enabled/high、requested/effective maximum393216、truncated=false、finish_reason=stop。合计308551 provider tokens，不是Codex用量或价格。DeepSeek仅分析给定文件；Codex执行工具、直接核关键源码／规范并裁定。

[audit.json](audit.json)保留纠正，模型原文不是全篇接受：同拍虚拟请求不等于同时翻译；不能把控制器一／两项请求状态等同K个配对记录；配对不必新增AXI tag或锁住TLB；部分取消不要求保留失效结果。第二份答复仍混淆K与控制器槽的表格已明确拒绝。cqh提前推进仅为未闭合观察，未扩大为本轮bug结论。

[source-evidence.json](source-evidence.json)保存10份关键当前源码的SHA256、必要行窗口；不冒充新的构建快照或当前板端逐字节匹配验收。[web-evidence.json](web-evidence.json)保存检索与读取，搜索命中并非都被采信。

## 新核主来源

- [RISC-V IOMMU 20260222命令规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)：区分缓存失效、CQ完成、已处理数据请求的PR/PW有序性。规范不要求本题假定的全局暂停翻译锁。
- [Intel DSA 341204-006US Rev3，2025-06](https://cdrdv2-public.intel.com/857060/341204-006-intel-data-streaming-accelerator-spec.pdf)：直接核§3.12–3.13、§4.3–4.4、§8.2.4。已有翻译、故障部分完成、buffer和在途限制，不能把这些宽泛组合当空白；不等于本文特定AXI撤销依赖的现成证明。
- [iDMA官方仓库](https://github.com/pulp-platform/iDMA)：用于实现来源；具体判断以本地授权平台的实际配置和源代码为准。

专利聚合页、教程、非官方镜像与搜索摘要没有成为“已证创新／已完全覆盖”的依据；没有声称穷尽前作。主文献库123项计数未改，网页阅读不当新全文入库。

单分块时序公式是Codex的说明性条件推演，不是实测，不等价于超过流水化成对预翻译。全系统完成性、真实动态更新工作流和新信息增量仍未确认。
