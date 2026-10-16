# 共享IOMMU成果重审与立题方案

2026年10月15日。**主线继续，优先发展能预测控制位置与完整任务结果的方法A；有限翻译准备B保留为备选重设计。** 当前早期walk意图只是A可能采用的一个接口，不再作为所有新想法的唯一入口。

此次重审复用七篇已存全文及历史验收，补齐方法、信息、保证、评价和成本比较；因A涉及互连分析，另定向核三项AXI近邻。得到两张具体候选卡和四项冻结预测，并执行一个全新的2×2copy完整RTL/行为内存pilot。这个固定任务没有触发当前头竞争，两种仲裁的完整事件与完成向量相同；四项任务级预测仍未验收，独立方法贡献仍需前瞻证据。

## 已有成果怎样支撑下一步

| 成果 | 可以继承的认识或能力 | 下一步怎样使用 |
| --- | --- | --- |
| 多DMA平台、E4与batch | 软件发现完成与硬件执行分开；自主batch建立真实重叠；无miss共跑也有数据竞争 | 复用供给、硬件计时、任务身份与守恒，不用软件总时间直接归因IOMMU |
| E2 | 特定布局下替换导致首次再用miss | 将容量效应与服务等待区分，比较P/N/PN时计真实工作量变化 |
| E3 v6 | 受控64B中A服务延长向后来V-hit等待传播 | 用局部因果说明控制链，不外推自然4KiB全部损失 |
| PN与M0 | 旧实板约12.7%组合退化；另一完整模型合同中3689周期差额被普通返回服务规则消除 | 研究优化怎样组合和选择控制位置；不能把模型返回重排当作MIG外部能力 |
| method88、R107 | 固定p5未胜强对照；一个有限恢复任务已被普通大页/驻留/准备满足 | 复用强普通基线、完整供给与准备成本，不重跑同版寻找胜例 |
| R2、RCS1、stage117 | 所有权/成本比较、完整完成反例和真实cache/consumer资产 | 合法回收阶段字段，验证全部完成；RCS1旧64B豁免只属其条件engine |

完整账本列16项资产、原始记录、正式验收和可核行号：[成果账本](../out/research-reappraisal-20261015/history/asset-ledger.md)／[结构化证据](../out/research-reappraisal-20261015/history/asset-ledger.json)。[关闭范围表](../out/research-reappraisal-20261015/history/closure-scope.md)区分性能负结果、普通构造等价、文献覆盖、缺需求与工程失败。8月逐日日期缺口保留，9月保存日期不回填为8月开发日期。

## 最接近论文的实质对照

| 前作与原文位置 | 已做到什么 | 本研究仍可推进的具体问题 |
| --- | --- | --- |
| [Bermuda RTCSA2025](https://arxiv.org/pdf/2508.19670v1)，PDF3–10，§III–V | 商用SMMUv2共享翻译干扰实测；缓解方向主要留在讨论 | 可执行干预和完整任务收益，不只是换平台再次表征 |
| [MASK ASPLOS2018](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)，PDF7–9，§5.2–6；PDF12成本 | TLB填充、PTE缓存与MC三队列联合设计；同时处理翻译和data | 固定外部接口下控制何时仍有效，已承诺义务怎样限制保护及费用 |
| [DWS HPCA2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)，PDF5–8／会刊630–633，§V–VI | walker软分区与借用，局部跨租户交错约束 | 将局部服务性质连接到共享内存、有限buffer及真实任务完成 |
| [Orchestrated DAC2023](https://libingyao.github.io/assets/files/DAC2023.pdf)，PDF4–5，§IV–V | 调度、TLB分区和共享；已测完整应用，已有局部hit收益不兑现的例子 | 必须增加事前判别与部署决定，不能只重述局部指标不足 |
| [MMU-aware DMA ICCD2018](https://arxiv.org/pdf/1808.09751)，PDF4–6，§IV-A/C；PDF7–9评价 | worker进度指导有限距离纯翻译预取，burst metadata与失败重放 | 成功准备结果跨真实payload所有权边界的有限保存、提交及完整费用 |
| [Accelerator-Centric Translation HPCA2017](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf)，PDF6–9，§IV-B–D | 小私有TLB、共享TLB、hit-under-miss、host MMU/PWC/cache | 与普通分级缓存和结果队列比较接口组织及成本 |
| [RISC-V SVA DATE2025](https://arxiv.org/pdf/2502.17398v1)，PDF2–5，§III–IV | 页表共享LLC、payload绕行，实测降低所测kernel翻译残余 | 核查剩余需求；统一含准备、offload、cache交接的任务计时 |

每篇的假设、算法/协议、保证、评估口径、成本、短原文、页码/行号、版本与哈希见[A四篇卡](../out/research-reappraisal-20261015/literature/a-cards.md)及[B三篇卡](../out/research-reappraisal-20261015/literature/b-cards.md)。这些卡取代本轮判断所涉旧摘要口径，不改写旧库历史。

定向补充表明，层次AXI响应时间分析已考虑流水、在途上限和完整任务；AXI-ICRT已有可选择缓冲及可组合调度；AXI-REALM已有外置分段、预算、先收W后发AW和事务阶段监控。因此“外置控制、不修改MC、考虑在途、可组合”本身也不能支撑A新意。[三项近邻比较及原件](../out/research-reappraisal-20261015/analysis/adjacent-assessment.md)。

本轮纠正三项会影响判断的旧阅读口径：Orchestrated本地稿为六页DAC2023稿，旧不存在行号及所谓2024引文不采用；MASK直接原文写64个并发walk threads，与DWS对它的“无限walker”复述有未解版本冲突；Bermuda不同图表和正文的数值/单位差异不混合成完整任务成本。

## 优先候选A

### 具体贡献目标

构造控制权约束的反事实事件图：从共同合法任务前缀出发，把信息产生/传递、本owner首次offer、接受、返回、写完成、consumer及buffer释放分开；保留已经不可撤回的边，只改变该控制器实际拥有的新选择。重建每级PTE和后续载荷的依赖到达，在具名服务假设下预测完整任务。

方法输出四类可核部署结论：现有普通控制足够；需要更早控制；该动作已因承诺发生而无效；关键条件未知。普通DAG最长路、max-plus、carry-in与服务曲线是采用的工具。贡献须来自共享翻译反馈下新的判别/部署能力和前瞻结果，不能来自术语或图本身。

### 可核查的新意论证

MASK已经在可修改GPU存储层次中联合管理翻译和数据，DWS已经控制walker内交错，AXI时序和调节工作已处理流水、在途与边界控制。我们拟推进的问题是：依赖翻译产生的新流量、以及翻译完成释放的payload，怎样在逐级不可撤回、有限资源的路径中改变控制效果和任务完成。如果分析能预测未用于设计的案例，并导出有意义的新部署决定或完整成本差异，就可以形成方法贡献。当前E2/E3、PN/M0及源码支持这一问题；真实前瞻结果、适用范围和相对最近分析方法的实质增量仍待证。

### 实施与验证合同

[A候选卡](../out/research-reappraisal-20261015/candidates/a-development.md)和[方法规范](../out/research-reappraisal-20261015/analysis/a-method.md)固定输入/输出、控制位置、合法性及费用。四项[前瞻预测](../out/research-reappraisal-20261015/analysis/prospective-validation.json)为：当前PTE头可见时普通DS-first足够；共同旧债务不能被后门消除；传播太迟会失去机会；局部翻译提前可能被后继等待隐藏。后两项不得用已有结果调节后再称留出。

**决定：推进A的预测方法设计，重设计完整验证入口。** 已在私有测试副本接入真正的current-VALID DS-first/F4，删除返回提升，用真实iDMA/PTW从冷reset完成两路各两笔4KiB。RR/DS两案V均7009、A均6497、quiet均7019条件周期；28次offer中双头竞争和held-late-DS均0。PTE在行为FIFO遇到两次前序payload债务（487/940拍），但这个控制点没有新的竞争选择，按预定规则结束该pilot、不调相位或压力。这个源绑定的部署边界是正向材料，不是收益正例或对A全族的否定。[完整结果与原始入口](../out/research-reappraisal-20261015/analysis/a-pilot-results.md)。原生产及board接口没有集成，实际MIG债务仍未知。

## 备选候选B

### 自然需求与具体协议

固定HERO `mm-large.c`的双缓冲循环中，t轮计算仍读取当前B槽，而t+2外存源地址已可推导；该槽须等计算完成才能覆盖。其源级先行窗口有依据，持续时间、miss和可避免任务等待尚未实测。stage117原交付没有重复物理占槽，不能充当这个正例。

保留的最小协议是：有限prepare不产生payload请求、不取得旧buffer；结果按正式地址空间和准确owner保存；真实release及原事务资格满足后commit；已发布请求正常排空。稳定映射与异步失效合同分开，不为尚未出现的remap需求扩范围。

### 新意与决定

ICCD2018已覆盖进度感知、有限距离、无payload访问的提前翻译，因此停止把这些动作本身作为B0的独立新意。B1保留“准备结果跨真实payload-slot所有权边界的有限保存与交接”问题，比较同信息有限pin、普通prepared-PA队列、合法字段复用及PWC/LLC。

**决定：B1保留重设计，尚不进入模型。** 当前没有相对上述具名普通方案的独立动作或完整成本差，不能靠未知物理成本制造差异；同接口普通等价也不能证明完整协议已发表或架构无价值。与R2/O-B5S的具体承诺时点区别、完整成本、HERO源码和正反条件见[B候选卡](../out/research-reappraisal-20261015/candidates/b-development.md)。

## 两周推进与下一关口

两周窗口为2026年10月15—28日（Asia/Tokyo），原累计研究用时保留；当前实际推进以证据而非经过了几天计，提前交付不意味着后面的验证已经通过。

| 阶段 | 交付关口 | 当前实际状态 |
| --- | --- | --- |
| 第1—3天 | 关键历史账本、七篇实质对照及准确关闭范围 | 已交付；额外补核三项直接相关分析/互连前作 |
| 第4—5天 | A/B两卡、实际接口/需求、完整成本目录与条件新意 | 已交付；A优先，B1备选重设计 |
| 第6—10天 | 一条候选、一个正例/相邻反例及两项冻结留出，完整新模型验证 | A先完成2个新完整pilot；该固定来源未覆盖作用机会，四项任务级预测仍未验收 |
| 第11—14天 | 核前瞻结果、费用和实质贡献，推进实现或明确重设计 | 已有方向决定与具体重设计点；未把预测正确或论文立题记为已完成 |

下一轮先把A的方法变成可检查的分析结果：以新pilot为有来源的“债务存在但此处无作用机会”边界，结合准确前作，给出能从协议/任务约束推出控制位置选择的规则及适用条件。只有另有任务事实提供新的可改变选择，才冻结一个完整正例及原四项预测的验证合同；不自动复跑ACV-2x2、不扫相位、加压力或重跑旧成功矩阵寻找胜例。若仍只得到标准DAG/AXI分析的直接实例，则收窄具体方法主张，而不是关闭共享IOMMU主问题。

A若证明必要部署能力或完整成本点，下一阶段做一个真实GPU/AI来源的共享路径，再按[RTL/FPGA验收合同](../out/research-reappraisal-20261015/validation/rtl-fpga-contract.md)开展关键验证；时序、功能与测量身份须匹配。B只有取得其具名普通对照的具体不足才替补。原MIG观测暂停不因重审而恢复。

本轮有一个私有完整任务模型构建和两次新完整执行，另有普通组件的协议验证；其命令、失败和结果独立留存。生产RTL/Linux/driver/ABI及板操作均无变化，原暂停回执与旧科学结果保留。唯一当前接受与后继条件见[root-result-acceptance.json](../out/research-reappraisal-20261015/root-result-acceptance.json)及[next-decision-contract.json](../out/research-reappraisal-20261015/next-decision-contract.json)。
