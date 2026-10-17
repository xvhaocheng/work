# DISCOVERY-REUSE-1：名义容量、重复walk与命中旁路的完成代价

## Material Passport

- ARS-Codex `experiment-agent / run`及原始数据归约；2026-10-17 JST；输入`main@67a3f66eb11132450c153fdb56c7529ebc0ecdfd`。
- 状态：**8个预登记条件实际完成，逐parent与独立原始事件核验通过；未独立复跑，不作统计显著性或原生收益声明。**用户在具体合同完成后回复“继续”，授权本轮实施/运行。
- [冻结计划](problem-discovery-contract-20261017.md)／[执行入口与复现](../out/problem-discovery-run-20261017/README.md)／[root接受](../out/problem-discovery-run-20261017/acceptance.json)／[完整归约](../out/problem-discovery-run-20261017/analysis/findings.md)。

## 1. 决定

**本轮取得了能改变机制设计的新事实，但不启动新控制器：当前普通替换存在容量利用局限，而合法普通大页组织已消除本专用arena中的重复walk和P/PN完成差；这批结果归为问题发现与普通优化依据，未形成独立论文机制。**

不把它压缩成“又一个候选失败”：本轮没有运行待证明的新机制。它纠正了两个会误导设计的前提——“足迹小于分区配额就只剩冷miss”和“让hit提前进入数据通路必然改善完整任务”。First-PTE、前三机制的停止范围不变，也不由本轮否定所有4KiB受限部署、替换组织或翻译—数据协同机制。

## 2. 实际执行合同与完整结果

两真实iDMA各64个normal、aligned4096、batch-OFF COPY；每lane一个未完成parent。V源/目的固定，A源固定、目的按本lane序号`j % R`轮转。初始配置/触发A后V，循环V poll/refill后A，双方独立完成64次后排空。没有warm、人工内部状态、启动delay或选择性前缀删除。

共同基座是**P/PN＋已实现的承诺前DS-first/F4**，不是stock；C++返回提升、queued startup overlap、M/O及CPU背景关闭。一个elaboration/可执行文件完成全部条件。DDR为原FIFO、head-only startup31、DS50/50、R/W独立的行为模型；生产RTL/Linux未改。源码和实际命令以[build manifest](../out/problem-discovery-run-20261017/build01/build-manifest.json)、[C++](../out/problem-discovery-run-20261017/runner/discovery_dma.cpp#L203)及[实现说明](../out/problem-discovery-run-20261017/runner/implementation-note.md)为准。

时间单位均为**从配置前quiet起点t0计的RTL仿真时钟步数**，不是FPGA周期、真实DDR或应用CPU时间。`T_V/T_A`是各lane最终硬件DONE；`T_return`还包括软件完成发现、最终检查访问和quiet。

| A叶 / R | 普通模式 | T_V | T_A | T_return | walk | N真正接受 |
| --- | --- | ---: | ---: | ---: | ---: | ---: |
| 4KiB / 1 | P | 76300 | 75726 | 76331 | 4 | 0 |
| 4KiB / 1 | PN | 76843 | 76269 | 76869 | 4 | 4 |
| 4KiB / 8 | P | 90261 | 89749 | 90290 | 39 | 0 |
| 4KiB / 8 | PN | 106542 | 106030 | 106570 | 39 | 74 |
| 4KiB / 16 | P | 101305 | 100793 | 101334 | 67 | 0 |
| 4KiB / 16 | PN | 130078 | 129566 | 130104 | 67 | 130 |
| 2MiB / 16 | P | 75770 | 75196 | 75797 | 3 | 0 |
| 2MiB / 16 | PN | 75770 | 75196 | 75797 | 3 | 0 |

每条件双方总计128个parent，全部8条件共1024个parent；每lane均262144B读/写、32768个R/W beat、128AR/128AW/128B、64issue/DONE、错误0。逐W strobe数据与逐parent字节覆盖在运行中检查；独立归约再从原始AW/W/B/R和身份事件核验，最终内存比较只是其中一项检查。所有条件首次运行通过，未重跑旧实验或扩大矩阵。

## 3. 新事实一：9页并未利用好14项配额

**实际轨迹事实：**R8时A只使用源1页＋目的8页，P/PN两者却都发生39次walk；其中V两页各1次、A源1次、A目的36次。目的0/2/4/6各walk8次，目的1/3/5/7各1次。R16为每目的4次，总67次walk。不能再把R8当成“首填后保证无walk”的容量控制。

**源码推导：**实际IOTLB是masked tree-PLRU，而非14项LRU；[eligible/victim逻辑](../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv#L365)只看mask与tree位，未优先选择无效way。[partition-local touch](../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv#L384)隔离V/A历史，却没有平衡A的6/8项根子树。

本次冷历史中，目的先填way2、源随后填way8的源码推导，使反复源hit把后续目的victim推向左6项；其2项/4项子树交替选择，R8偶数目的4页在2项中轮换，奇数4页可驻留，精确预测`32＋4＋1＋2=39`及上述页级模式。[详细推导与真实行号](../out/problem-discovery-run-20261017/analysis/replacement-source.md)给出每一步。

**没有直接记录内部victim/occupancy快照**，上述way分配仍是源码推导与页级轨迹相符，不升级为实测内部状态。它是当前普通替换实现的利用局限，未证明错译或功能bug。invalid-first、LRU或其他普通替换是否改善完整结果未在本轮运行，不把它们称为已测强基线，也不把此现象直接认领新算法。

## 4. 新事实二：hit更早读完，可以更晚完成

4KiB下PN与P的walk数逐档相同，但完整`T_return`分别多538、16280、28770步（约0.70%、18.03%、28.39%）。不能用“PN多做了walk”解释这些差额。

以R16的V全64个parent的非重叠阶段之和为例：

| 阶段总和 | P | PN | PN−P |
| --- | ---: | ---: | ---: |
| ready→issue | 1993 | 1691 | −302 |
| backend→首个本地R | 34473 | 11387 | −23086 |
| 全部源R收齐→最后B | 29767 | 81928 | ＋52161 |

其余issue→backend、首R→全部源R、最后B→DONE总和相同，三项差额正好解释该lane完整DONE多28773步。这是**完整时间的阶段定位**，不是证明某个控制器能够回收52161步。尤其不能把后台/前台重叠或同时成立的AW flags再相加。

更具体的必要前驱来自A第16个parent的第二笔PTE：[精确ID/occurrence证据](../out/problem-discovery-run-20261017/analysis/committed-debt-example.json)。

| 实际事件 | P | PN |
| --- | ---: | ---: |
| 第二PTE（ID48）被DDR接受 | 27750 | 35006 |
| 成为FIFO head | 27751 | 35468 |
| PTE R返回 | 27782 | 35499 |
| 接受边沿后的排队步数 | 0 | 461 |

PN中同一PTE之前已有V第16个parent的两笔payload（ID32，occ127/128）：分别在34790/34796接受；两笔R直到35180/35467才结束。后来的PTE不能越过这些已承诺读。**DS-first只能选择尚未承诺的请求，不能改变此时的FIFO前驱。**这里确认的是指定PTE在行为模型中的实际前驱责任，不把两模式时间点当同一到达日历，也不把局部461步扩成整个PN差额的纯因果估计。

软件供给并非没有成本，但V的ready→issue总和在R16约两千步，不能独自解释完整差额；源收齐后的写腿/依赖需要保留。所有parent、最长V与最后完成parent的固定切片见[归约报告](../out/problem-discovery-run-20261017/analysis/findings.md)，没有选择性删除首填。

## 5. 普通组织的实际干预与费用边界

R16已出现环回后的目的walk及必要写翻译链：P下A j16在27460接受写翻译，27463 miss/start，27609源数据收齐，27984 walk结束、27985响应，27990才有目的DDR AW，28507完成。因此按冻结规则追加两项2MiB诊断；[预触发原始证据](../out/problem-discovery-run-20261017/analysis/diagnostic-trigger.json)与[root准入记录](../out/problem-discovery-run-20261017/diagnostic-acceptance.json)可复核。

两项只将A映射改成普通Sv39 2MiB叶，COPY序列、物理地址/内容、供给和buffer均不变，V仍4KiB。P的`T_return`少25537步，PN少54307步；两者全部parents CSV字节一致，均仅3次初始walk，N接受0。PN仍有2个物理命中probe，它们没有被接受，不能用probe hit冒称旁路成功。

这证明**该专用arena允许普通大页时，消除大量重复walk的组织改动具有完整效果**。P配对没有改变N使能；PN还涉及N资格/路径变化，均不把整个差额命名为纯walk成本或新调度收益。没有执行“只换替换规则”“只换PTE排队规则”的消融，二者各自全局净影响仍未知。

资源分别核账：每条件相同1MiB有效payload读＋写流量；PTE/DDT额外流量另列。A专用arena始终预留2MiB，R16有效payload物理页19（A17＋V2），元数据始终预留七个4KiB页，包括2MiB方案未使用的A L0；实际稀疏字典初始化字节另记。没有OS物理分配/不可回收RAM测量，配置MMIO步数也不是CPU费用或consumer/free生命周期收益。

## 6. 本轮研究裁决与唯一后继边界

| 问题 | 本轮推进到哪里 | 不能推出什么 |
| --- | --- | --- |
| 重复翻译是否真实 | 逐页/parent观察成立；2MiB普通组织改善完整结果 | 所有正常应用都存在相同压力，或新替换算法成立 |
| 普通DS-first之后是否还有债务 | 指定后续PTE确实排在已承诺payload后；承诺边界已定位 | 全部PN损失都能被新接口消除，或First-PTE M应恢复 |
| 供给是否就是全部问题 | 所核输入的供给阶段不足以解释主要差额，写腿明显暴露 | 真实CPU/消费者瓶颈已被排除 |
| 普通方法是否足够 | 本arena的合法2MiB组织已取得相同完成结果 | 普通替换/分段所有实现已测，所有4KiB受限任务已解决 |
| 论文机制是否成立 | 新约束/前驱事实已取得；当前归普通优化依据 | 新意、实现成本优势或可推广主张已成立 |

**不把下一步设为新的早意图协议或全系统开发。**若继续这个输入，最小有判别力的后继只应是一个普通替换校准：R8/P下检查“有无效eligible way时先填它”能否消除首轮提前覆盖及后续重复walk，并记录完整COPY结果和实际新增逻辑。通过则归普通优化；失败则说明该普通修正不充分，回到剩余实际victim/义务分析，仍不能自动认领新机制。此建议未执行、未排队，不加到本轮8条件，也不把它本身当论文创新；允许直接结束此arena范围。

若以后要以“后续PTE已承诺债务”为论文问题，需有匹配正常来源说明普通合法大页/替换组织后仍有必要walk，并给已有分段/服务规则同信息、同接口的机会；不能为了保留矛盾禁止可用普通方案。宽泛的翻译—数据协同仍受[已核MASK/REALM/iDMA前史](../../references/research-reconvergence-20261016.md)限制，本轮没有重复查新或宣称发现此前未研究的问题。

本轮只有一次局部基座编译及8次新条件仿真；没有生产改动、综合/布线、旧实验重跑或板测。B1/I/D/A5/A2、mapping/p5/D-CS1及MIG暂停、原10月15—28日窗口均保留。Codex-only；C++实现请求Sol/xhigh，观测/归约请求Sol/high，实际独立模型元数据不可得，不声称切换root。
