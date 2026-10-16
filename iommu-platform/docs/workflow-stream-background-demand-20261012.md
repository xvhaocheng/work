# B工作流真实后台需求补证与裁决 stage102

2026-10-12。按用户批准计划执行，优先核真实后台需求。全程Codex根执行；最多两项直接关联新材料已用完，没有新模型、构建、校验器或实板运行。

**裁决：本次有界取证结束，真实后台需求仍未决；不登记下一轮时序构建，不进入G1。** 原8MiB受控后台和stage100/101结果保留。方法论文总体目标未完成。

## 两项补证及来源边界

固定HERO提交 `c8481a930a15a385d4f58f519bed83d2e75e2a73`：

- [官方应用说明](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/openmp-examples/README.md)第4—17行提供各应用构建及Polybench线程、存储、DMA和offload选项，没有所选独立后台的到达/完成合同。计算线程并行不能直接当跨设备后台。
- [官方测试入口](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/openmp-examples/testset.cfg)第5—15行登记四个子测试，没有描述共跑关系。未读取测试调度器，不断言测试一概串行或绝不并发；测试列表本身不足以提供应用轨迹。

两文件均保存原始字节、SHA256和与已固定官方树一致的Git blob身份。[来源清单](../out/workflow-stream-background-demand-20261012/source-manifest.json)。未沿其外链扩大应用类别。

复用既有原文/源码作关键核验：原SP描述一个规则分块应用的输入/输出与计算；HERO的A/B/C双缓冲属于同一个矩阵结果。矩阵程序第351—405行明确说明device(0)/device(1)对应同一个物理加速器，同时使用不安全；当前活动路径为copy-based，SVM段被禁用。因此不能把两个逻辑device号改称独立设备背景。子测试配置的timeout属于测试限时，单位本轮未核，不是来源应用的首笔/逐笔期限。[带原始行号的核证](../out/workflow-stream-background-demand-20261012/source-evidence.json)。

## 后台需求卡

| 必要项 | 来源应用后台 | 既有受控后台 |
|---|---|---|
| 独立任务身份 | 未取得；A/B/C为同一结果的搬运 | 研究追加的独立iDMA copy任务 |
| 到达关系 | 无原生触发/到达记录 | 逻辑t0就绪，策略等待照计 |
| 有效工作量 | 独立后台工作量未知 | 2048×4096B＝8MiB，16页环 |
| 自然结束 | 未取得 | 固定全部工作完成，不截断/延长 |
| 完成目标 | 未取得 | 完整完成与共同观察窗内有效进度 |
| 共享翻译路径 | 未识别后台，尚无法对应 | stage100已验收的实际DMA/IOMMU模型路径 |
| 期限 | 没有已核期限 | 没有期限，不补首笔阈值 |
| 适配成本 | 未识别具体任务，无法闭合 | 原stage100成本和限制保持 |

[机器可读需求卡](../out/workflow-stream-background-demand-20261012/background-demand-card.json)逐项标记来源事实、实验设定和未知。共同启动、有限工作量可作为明确的实验设计；本轮不要求所有实验数字都必须原论文提供。主张实际应用需求时，必须另有可核的程序、实际需求规格或记录。没有deadline也能成立，不能因缺deadline而否定有效进度目标。

## 接受与下一关口

此次未建立独立后台身份及到达、工作、结束、目标、共享路径的对应。按两项材料和一天上限停止；无新G0失效、无候选、无实际计算时序构建。源码未提供该合同属于需求未决，不能称B科学阴性或领域失败。

有具体新任务合同后再重开：说明独立有用工作、到达/依赖、有限工作及完成目标，并核其实际共享服务；来源未知项与实验选择分列。通过后才规划实际计算/搬运的共同时间基测量；强普通策略仍有关键路径损失才进入最多两条G1规则。本轮不反复补查同一材料，不加后台长度或新期限来恢复候选。

[根裁决](../out/workflow-stream-background-demand-20261012/root-result-acceptance.json)／[执行账本](../out/workflow-stream-background-demand-20261012/execution-ledger.json)／[stage101条件复核](workflow-stream-platform-review-20261012.md)／[stage100结果](workflow-stream-g0-results-20261012.md)。仅Codex，未调用DeepSeek或Claude；未修改生产接口、源码或旧原始数据。
