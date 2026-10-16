# D：SPDK来源合同与模型准入裁决

2026-10-15。**推进完成驱动的源语义原型；暂不启动P/PN性能模型或RTL适配。** 已核实SPDK有真实的完成→同task/buffer补发规则，取得映射实现链和库事件schema；未取得实际事件轨迹、物理共享绑定或服务态。这里的推进是把可检验合同变成执行对象，不是接受D的新意或净收益。

## Material Passport

- ARS-Codex：延续定向来源核查及experiment-agent/plan；不启动完整论文pipeline，不恢复用户已删除的个人研究方法。
- 输入：固定SPDK v24.09 `19524ad45373e35d5fdc06f4e081444d87ceed80`、其DPDK gitlink、明确标为参考实现的Linux v6.12、既有M0/平台边界。来源原件和hash见[映射索引](../mapping/sources.json)、[事件索引](../events/sources.json)。
- 验证范围：源码事实、明确投影的clockless状态机及其独立回放；无统计样本、设备计时或原生性能。最终执行结果以[root接受](root-result-acceptance.json)及[原型回执](../model/README.md)为准。
- Codex-only。证据席请求Sol/high、设计席Sol/xhigh、普通实现席Sol/medium；独立实际运行元数据未取得，不以请求值冒充实际值。不因新配置重跑旧实验。

## 1. 真实来源提供的研究空间

`perf.c:1507–1529,1533–1573,1603–1630`支持有限在途task：成功提交增加计数；应用callback退休本次请求；未drain时复用同一个task和buffer补发。源反馈确实存在，因此动作一旦改变完成顺序，就可能改变后续提交与buffer代际。**另一动作必须重新计算后继，不能沿用一次运行的固定到达时间表。** 源码规则不证明某次实际运行出现了共享翻译干扰。

普通起点已包含常驻buffer池及完成驱动复用。随机LBA不是随机host IOVA；pin/map常驻也不保证IOTLB永久驻留。`spdk_dma_zmalloc_socket → rte_zmalloc_socket`、DPDK memseg/VFIO映射及PRP/SGL路径均已核。4KiB IO、PRP粒度、CPU hugepage、SPDK软件2MiB查表和IOMMU leaf分别登记，实际leaf未知，不能把4KiB页当原部署必然条件。[完整证据](../mapping/findings.md)。

## 2. 已有事件能证明到哪里

SPDK库SUBMIT包含QID、request、ctx、CID和部分命令字段，时点在SQ写入前；COMPLETE在软件poll处理时记录，并先于错误/重试/应用callback判定。它们不暴露payload DMA服务、IOTLB/PTW/DDR归属、buffer IOVA、BDF或完整drain。

取得的是**实际实现的schema，实际trace文件为0、联合trace为0**。stock perf源码未见显式trace初始化，`-L/-LL`只有直方图；不据此宣称任何链接环境都不可能启动tracing，也不宣称互联网没有公开trace。NVMe成功必须解码`SC=0 && SCT=0`，phase位为1仍可成功；`io_completed`还包含错误callback。worker汇总elapsed在最终drain之前截取，不能作为本研究全任务时间。[字段裁定](../events/findings.md)。

## 3. 准入分三层，避免把缺部署误当缺研究空间

| 层次 | 本轮决定 | 可支持的结论 |
| --- | --- | --- |
| S：源语义及无计时执行 | 准入并实施小原型；所有事件显式synthetic | 有限task、代际、复用、补发、错误分类与尾排空合同可执行 |
| M：显式研究架构的闭环服务模型 | 条件准入规则已设计，当前性能版本未准入 | provider、动作、观测、成本、普通对照冻结后，可给假设范围内的因果/信息边界 |
| N：实际部署与原生应用效果 | 未准入、未运行 | 仍需实际Requester/映射/共享结构、完整成功轨迹、费用和部署接口 |

S→M不要求原来的PC/SSD已经原生提供拟议P/PN硬件，也不要求先测出反号。新硬件研究可以明确提出接口。反过来，接口缺项本身不是创新，假设模型也不能补写真实状态分布、持久性、时序或性能。

不提前进入M的原因是服务provider和跨quiet状态仍未固定，而非要求“先证明收益才允许建模”。原型不含共享翻译/服务计时，不能代替这个决定问题。详细接口及两条模型路径见[设计合同](../design/proposal.md)。

## 4. 本轮最小可执行交付

先冻结[语义合同](semantic-prototype-contract.json)，再实现一个worker/namespace、纯read、无metadata、有限N=4/Q=2的clockless子集。N/Q仅是验证常量；opaque buffer不是实际VA/IOVA；completion次序遍历是接口允许的过近似，不是自然NVMe样本。只在应用callback层建模，不模拟驱动重试、PCIe、SQ/CQ流量、设备服务或硬件安全完成。

生成器和独立事件回放器分别实现。验证在途额度、准确工作量、generation/buffer所有权、drain后无新增提交及全部释放；成功状态解码单独覆盖phase=1与SC/SCT错误。早复用、drain后额外提交、错误计为成功的变异必须被拒绝。另用不同合法callback次序展示补发序列跟随自身反馈。实际检查数与运行记录留在原型产物，不把合成事件计作实测。

## 5. 最有价值的下一决定题

**在完成驱动供给中，已有公开结构与完整host历史之外，是否还存在可跨quiet/clear/warm预测、且会改变P/PN完整效果的共享服务信息？** 若没有，保留普通结构规则或信息充分性结果；不人为隐藏QD、映射、复用、扇出或burst制造缺口。

下一批只需先冻结一份服务合同：两个明确的完成驱动owner、NVMe read的device→host payload方向、研究映射和实际资源约束、命令/服务/payload/CQ/poll因果、普通TL与安全fast许可、完整有限终点、provider状态转移及跨边界保留/重置、决策前可得O0和候选付费Z。可选符号参数域并给条件结论，不凭空填真实延迟、hidden state或天然相位；provider不能查询希望哪个模式获胜。保持旧M0与guard不变。

目标事前固定为双方相同成功工作量的完整makespan，另报各方完成及quiet/free；配置、clear/warm、观测与下一invocation准备费用不能漏掉。若另研究前台期限目标，要另冻结后台工作与期限，不能看结果改指标。每个动作独立演化供给与环境队列。

普通对照必须有固定P/PN、完整O0规则、合法大页/常驻池/准备、队深与poll、固定池的普通admission/pacing、付费试选。stock QD变化同时改变task池与翻译footprint；固定池admission是需明确实现和计费的普通变体，不能假装stock已有，也不能只给研究动作。新Z也交普通规则，分别裁定接口价值与选择算法价值。

如果正常provider不能产生反号、O0已足够、clear/warm消除预测、Z太迟/太贵，或普通试选同成本达到相同完整结果，则停止该合同对应增量。停止范围不扩大为PN或整个D思想族无效。

## 6. 新意边界与保留事项

最近工作比较继续沿用[六族比较§2.2–2.4](../../../docs/independent-research-directions-20261015.md)：已有遥测/采样/反馈选择，不能再认领宽泛“少观测选策略”。**如果**源保真的有限闭环证明完整O0确有动作不可辨识边界，且及时的少量硬件信息经全部成本后改善未见任务选择，**才**可围绕这项具体边界、接口和完整决策证据发展贡献。当前只补了来源链、可得性与执行语义，没有新增论文覆盖或novelty接受结论；语义原型不是贡献证明。

固定M0旧来源门不通过的裁决及原六日志不变，不调gap、不放松防护、不重建覆盖历史目录。I仍轻量not_ready，不自动提升或退A；A5/A2及并发工程成果保留。MIG暂停、旧时序失败及实际债务unknown不变；0原生SPDK/DMA、Linux/RTL/UAPI修改、FPGA或板操作。
