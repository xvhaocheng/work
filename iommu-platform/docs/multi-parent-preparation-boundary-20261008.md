# 多父准备边界：普通缓存命中直通对照与条件性独立创新判据

2026-10-08，§38，接 §37。Codex 亲自设计准入、旁路对照和预翻译时机，直接检查关键源码及前作，执行构建／仿真并最终验收；DeepSeek 起草和修订结果汇总。实验代码修改限于 out 副本。

## 本轮改变了什么判断

本轮新增 5 次构建、18 次新运行，18 PASS：8 次普通模式、4 次读优先、2 次观察运行、4 次命中直通。`analysis.md` 只覆盖最初 8 项；[codex-raw-audit.json](../out/multi-parent-preparation-20261008/codex-raw-audit.json) 覆盖全部 18 项。另 10 项是新增 DUT 运行（即 8+10=18），不是另加 10 项根审计。首次汇总解析失败（VAR32 与 64 混淆、header0x 误读）是解析／汇总问题，不是 DUT 失败；原始 summary.py 初稿保留，summary_revised.py 经 Codex 字面 mdir 修正后 8 项全部接受。未重跑旧 DUT，未综合，未上板，未改 production/tests/Linux/BIT/SD/board。

已完成的“最强普通缓存命中直通”为 AW 优先 V 先启动：V=23380、A=26122、均值=24751。它只绕过后台 TL 串行等待；源子片仍经过共享 TC，正式子片在共享 TC 等待写翻译时不能处理这个命中读。把它叫“普通 TC 旁路”是错的。旧 post window4 的 V 先启动为 V=17485、A=20163、均值=18824，均值降低 23.946507%，绝对 5927 周期。这个差距不能叫“约 27% 独有规则收益”，也不是新规则证明。真正下一强对照是“已准备源片直接通过物理 AR、绕过繁忙 TC”，见 [codex-next-control.md](../out/multi-parent-preparation-20261008/codex-next-control.md)；该对照尚未实现。

prepare0／prepare1／prepare2 的双流均值均为 25869.5，但 prepare0 与 prepare2 的 CSV 和 prepseq 相同，prepare1 的 CSV／prepseq 不同而终值均值相同；不能把这三组称为 3 个独立性能点。

读优先不是统一最好：串行 lazy 均值=25630、串行 prepare0 均值=25847.5、缓存命中直通 read 均值=24992，均未优于 AW 优先的 24751。

命中直通期间，后台译址请求的VALID独立保持，后台响应可在内部接收；正式未命中请求仍串行；完整正常响应83位（不含MRIF内容102位）、慢请求完整上下文118位、待返回命中响应185位 保留；声明功能状态1281位不变。实际重叠命中为 AW 优先 28、read 优先 27（V 先启动）。观察运行两组 CSV 与对应串行读优先逐字节相同，观察本身没有控制结果。

## 实验表：全部普通条件与旧 post window4

表中 V／A 为同一次 V 先启动运行的 issue→done 周期；A 先结果逐路对换，是确定性镜像，均值相同，不再逐行列 20 行。观察 lazy／观察 prepare0 的 V 先启动 CSV 与对应串行读优先逐字节相同，不作为独立性能条件。

| 条件 | V | A | 均值 | 备注 |
|---|---:|---:|---:|---|
| lazy | 24084 | 27340 | 25712.0 | 32/32 复用；TL66／命中32／未命中34／DDTW2／PTW102／FAST0 |
| prepare0 | 24464 | 27275 | 25869.5 | prepared32／filled32／reused64／future15+15 |
| prepare1 | 24464 | 27275 | 25869.5 | prepared32／filled32／reused64／future15+15 |
| prepare2 | 24464 | 27275 | 25869.5 | prepared32／filled32／reused64／future15+15 |
| 读优先 lazy | 24002 | 27258 | 25630.0 | 串行读优先 lazy |
| 读优先 prepare0 | 24442 | 27253 | 25847.5 | 串行读优先 prepare |
| 缓存命中直通 AW 优先 | 23380 | 26122 | 24751.0 | 重叠命中 28，保持 0 |
| 缓存命中直通 read 优先 | 23621 | 26363 | 24992.0 | 重叠命中 27，保持 0 |
| 旧 post window4 | 17485 | 20163 | 18824.0 | 旧 post 参考 |

模式定义：模式0先按父请求距当前头的顺序，再按V／A；模式 1 匹配双 W 头按流最旧优先；模式 2 流轮询最旧优先。准备未来 15／每路，最多提前 3 个父事务，即 30/32；实际准备32次／填入32次／复用64次；lazy 32/0。TL66／lookup66／命中32／未命中34／DDTW2／PTW102／FAST0；物理 AR64／AW34；每路 4096 读／写；FIFO48B；未加载荷。4 个每路原窗口从 AR accept 到 parent RLAST，pre pa/done 0..4，实际高水位 4/4，helper valid tags 结束时 0。

## 因果见证与普通旁路边界

`observer-prepare0` V 先启动的 SV 本地时钟：源 V0 缓存填充 1019；写 REQUEST 2421，写 RESPONSE 2861，同一 pending 持续 440 周期等待写；源命中接受 2868 发生在写完成之后，不能据此说 2868 时 TC 仍忙写。该窗口才是“等待写”证据；`2868` 不能当“写仍忙”的证据。`codex-raw-audit.json` 只给这些 SV 本地时钟，不给 TB 周期。

观察计数：串行读优先 prepare 待处理14896／写等待7507／读等待64；lazy 待处理15341／写等待7955／读等待7136；缓存命中直通 read 待处理14006／写等待7523／读等待64。它们是 reset／drain 重叠，不是延迟分解，也不能与 flow completion 求和；整个模型不只 IOMMU。通用缓存在未命中下返回命中是已知普通行为；约束被标为 old helper baseline prototype gate，由 Codex 判断，不是 production PN 缺陷。

## 声明状态、路径与回归边界

声明功能位：pre raw 3316 + helper 1281 = 4597；post 4602。二者未综合，不是等面积，不声明面积优势。删除重复偏移也改变了组合计算，未核LUT或时序。pre raw header 推导中 addr-src 的偏移重复 512 bit 已移除，active job src 保持稳定。完整正常响应83位（不含MRIF内容102位）、慢请求完整上下文118位、待返回命中响应185位 保留；generation seq32。

76 个 production SV 哈希不变；production RTL、tests、Linux、BIT、SD、FPGA 板均未改。无故障／失效／世代通用验收、无四 DMA／MIG、无旧 PN 根因结论。调用用量只保留在 raw 证据，不在 docs 元数据中汇总。

## 独立创新条件

因为 Arm LTI 已定义 translation before ordering 与 lookaside（1.1／1.2.2）、同 page+context share 及 completion deadline 6.2.2，仅缓存读绕过 TC 或背景未命中不能称为独立创新。下一最强普通对照必须让已准备源片物理 AR 在 TC 写未命中期间通过，并带 full permission／lifetime／BC／error／routing，且同 original window／saved metadata／payload；该对照尚未实现。下一优先检验普通物理AR旁路。

如果普通完整 TC 旁路在同进度与同成本下成立，则位置／组合规则不能称为独立创新。如果联合依赖前沿准入／保留规则能在同预算下处理该旁路，并在故障／失效／生命周期约束下相对强前作仍展示独特进度／成本，则能称为独立机制。当前静态普通尚未验证世代／故障，只有 active flush cancel out 原型，没有 LTI 协议实现／LC 完成语义，所以不能把推测执行／提前后台当作生产安全。没有证据证明世界上无人拥有 完全相同的规则；不作期刊级声明，也不声明独立成立。下一强门槛仍是故障／失效／生命周期未验证；这些未验证项不自动等于是新问题，需强前作对照。

Coffman W spill 已知。[PULP axi_xbar 原设计说明](https://pulp-platform.github.io/axi/xbar_parameters/module.axi_xbar.html) 的 W 顺序／spill 等待环见该页；[Arm LTI IHI0089A](https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/IHI0089A_amba_lti.pdf?hash=091C20253C83E0BB218FD393595E051F343E8A52&la=en&revision=ec4c2359-5838-4f79-86ab-e1354236830a) 见官方 PDF。Codex 今日已亲自核对该官方材料，并核到 [primary-source-review.md](../out/multi-parent-preparation-20261008/primary-source-review.md) 新增的 [US20250217302A1](https://patents.google.com/patent/US20250217302A1/en) 前史：外部 ATC／PCIe 等待环与省重复地址已有描述；它不是本平台 AXI DMA 的同一个环，不声称精确覆盖本原型的 AXI 周期，也不当当前 RTL 根因或已完全覆盖候选联合规则。

详见 [analysis.md](../out/multi-parent-preparation-20261008/analysis.md)、[codex-raw-audit.json](../out/multi-parent-preparation-20261008/codex-raw-audit.json)。

