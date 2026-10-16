# 实现前创新审查记录

日期：2026-10-06。审定正文：[novelty-gate](../../docs/novelty-gate-20261006.md)。当前 A/B 宽泛方案未通过进入机制实现的预审；A1/B1 是收窄的待证问题，不是创新或发表保证。

## 调用与证据

| DeepSeek任务 | 完整输入 | 原始返回 | 可读答复 | provider tokens |
|---|---|---|---|---:|
| 最近前作证据 | [input](prior-input.json) | [raw](prior-raw.json) | [report](prior.md) | 195261 |
| 旧policy与实验边界 | [input](internal-input.json) | [raw](internal-raw.json) | [report](internal.md) | 52702 |
| 定向纠正/补近邻 | [input](revision-input.json) | [raw](revision-raw.json) | [report](revision.md) | 131889 |
| 两个收窄问题的反证卡 | [input](extension-input.json) | [raw](extension-raw.json) | [report](extension.md) | 172651 |

四次均为 deepseek-flash、thinking enabled/high、requested/effective max_tokens=393216，finish_reason=stop、truncated=false；合计552503 provider tokens，包含输入/输出，不是费用或Codex用量。完整字段见[audit.json](audit.json)。前三份raw保留MCP封装，extension-raw保存解出的完整payload；未删除回答中的不采纳意见。DeepSeek仅返回文本证据/草稿；实际检索、下载、源码核读、文档应用和裁定由Codex完成。

`web-1.json`至`web-8.json`保留前段检索；`web-extension-1.json`至`web-extension-8.json`保存继续收窄时的检索及原始工具结果，含抓取失败。搜索结果只作发现入口，正文论据由原始论文/作者或机构来源支持。网页工具行号与本地PDF提取行号不是同一坐标。

## 原始文献及核查深度

- 本轮关键原文核查：TCWS/TA-CCWS §7.2、CCWS §3.3、DAWS §5、MASK §5.2–5.4，以及IOTLB Wall、Bermuda、KPart、DWS等直接涉及当前主张的段落。复用现有专题库和广域地图，未重复下载它们。
- 新近边界：Dead on Arrival v2按2026预印本处理；CounterPoint按论文原文定位为微架构假设检验方法，不能称在线配置控制器。
- [mNPUsim作者PDF](papers/mnpusim2023.pdf)及[提取文本](papers/mnpusim2023.txt)：IISWC2023；根核§3.1、§4.1.3、§4.3–4.4；DeepSeek提取两张问题卡所需证据。文本含宽列/换页符，引用应按节号并核`nl -ba`物理行。
- [NeuMMU作者稿PDF](papers/neummu2019-author.pdf)及[提取文本](papers/neummu2019-author.txt)：2019 arXiv作者版本；根核§III-C、§IV-B，尤其DMA限流损失并行性及多加速器QoS不在该文范围。此份未逐字对照最终出版版，不将其未来工作当作2026空白。
- 新PDF/text来源、字节数和SHA256见audit。主专题库123项/既有增补批次统计未在本轮重算；本目录是定向补核，不能称整个领域已穷尽或所有全文已精读。

## Codex审定时不采纳或纠正的内容

1. MASK venue是ASPLOS2018；其L2 bypass不是当前N的PTW忙时另一bank命中服务，填充token也不是固定条目保护P。原答复保留，审定文档已更正。
2. 未在若干论文中发现某机制，不等于无人做；TCWS已直接用虚拟页标签/复用调度源端，DAWS已有预测式占用控制。
3. 页预算不等于IOTLB Wall的大页/回收；DWS不是抢占walker，CLAP不是预取器。控制点不同须说明新增约束，不能简单互相覆盖或仅因平台不同宣称创新。
4. p5阴性限定旧固定A8、目标、版本及四格对照，不推导全部限速无用；当前172不抹去P4M/method88历史。p4 PTW项是返回握手事件，不是完整walk完成。未把worker源码推测升级为新bug。
5. mNPUsim已有RAW依赖、计算重叠与FCFS共享；其预生成请求列表不证明所有应用可获得完整未来。worker关于有限前视/消费者时刻的叙述仅为假设，没有真实轨迹支持。普通EDF/slack/局部性组合仍可能足够。
6. worker建议“固定PTW负载”不能直接作为P/N因果比较约束；PTW负载、miss、实际吞吐可能被机制改变。应匹配外生工作与边界，分析中介和完成驱动供给，而非锁死结果变量。
7. 完整净收益、干预和留出验证是后续证明要求，不应全部前置成允许最低成本概念验证的条件。当前未过的是具体前作差异/工作流/可证伪预测，不是因为尚无最终论文数据。

## 本轮没有发生的操作

没有RTL/Linux/实验程序改动、构建、新板测、SD写入或重启；无Claude调用或新Codex子agent。只更新研究结论与交接。既有172/172功能验收及method88阴性结论不变。不自动恢复旧formal60或旧“优先实现A”的安排。

## 后续：用户再问推进空间

新增一次DeepSeek证据整理：[输入](room-evidence-request.json)、[完整MCP返回](room-evidence-response.json)、[原答复](room-evidence-answer.md)。deepseek-flash、thinking/high、默认393216、stop、truncated=false；prompt153641＋completion62017＝215658 provider tokens。与上方原4次分开，不更改旧统计。

Codex直接执行[六份原记录字段核对及P/PN算术](room-metrics-audit.json)、[worker部分恒等式/交互复算](room-worker-spotcheck.json)，核[关键RTL快照与新PDF身份](room-source-audit.json)。新增官方[DAAT-MCS全文](papers/daat-mcs2026.pdf)与[text](papers/daat-mcs2026.txt)、[网页核查](room-web-new.json)；补读CVA6-VMRT官方作者HTML，未运行其代码。SP-IMPact官方DOI为10.4230/OASIcs.NG-RES.2025.5，纠正主库catalog.csv/references.bib/INDEX.md，历史worker稿保持原样。

审定见正文§8。未采纳worker的全库新颖性断言、A完成数因精确相同即疑似截断、只剩独立性缺口、单次跨boot复现即可支持创新等推断；A1/gap0 block1加性交互实际−0.184%，不是其手算+0.31%。fast与normal分开计数、A独立done事件、raw_replace_pscid含义由既有规格/源代码说明，不能称其无定义。未接受“6 cell”却列8条件的复测计划。没有新增实现、仿真或上板；共享PLRU只是已核源码中的解释候选，不是已经证明的根因或创新。
