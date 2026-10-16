# 共享 IOMMU 研究内容与证据导航

2026年10月15日。研究对象是多个 DMA 设备共享 IOMMU 时，地址翻译、载荷传输和软件供给怎样相互影响，以及应该在什么位置干预，才能改善完整任务的结果。现有成果包括实验平台、局部因果证据、完整任务评价和一批机制边界；当前主线是把这些积累发展为可检验的控制位置与部署判别方法。

**研究已有扎实的实验和实现积累，方法论文的独立贡献与真实整任务收益仍需验证。** 当前工作树还比状态页更进一步：SVA 适配代码和软件构建记录已经存在，但新内核运行、真实 DMA 和完整性能验收没有完成。以下将这几个层次分别列明。

## 研究问题及其联系

DMA 负责搬数据，IOMMU 负责把设备使用的地址翻译成可访问的物理地址。翻译查表本身也要访问存储系统，因此“让数据更早通过”可能同时改变后来页表请求的等待。应用还要经历准备、发布、缓存交接、实际消费和资源释放；某一步变快，未必使任务更早结束。

```mermaid
flowchart LR
  SW[软件准备与发布] --> DMA[多个 DMA]
  DMA --> IO[IOMMU 地址翻译]
  IO -->|页表请求| MEM[共享互连与内存]
  MEM -->|页表返回| IO
  IO -->|翻译后的载荷请求| MEM
  MEM --> END[数据与写响应完成]
  END --> CPU[缓存交接与结果消费]
  CPU --> FREE[缓冲和映射释放]
```

据现有计划，问题可以整理为三层，而不是按大量阶段编号理解：

1. **干扰来自哪里？** 区分翻译缓存替换、共享翻译服务等待、载荷与页表的内存竞争，以及软件供给或发现完成的延迟。
2. **什么位置还有合法选择？** 区分尚未发布、已经给出且不能撤回的请求，以及已经接受、必须完成的工作。源头、翻译器和内存入口看到的信息与能采取的动作不同。
3. **干预对完整任务是否有价值？** 在相同信息和资源下，与具名普通方法比较双方完成、真正消费者完成、资源释放及全部额外费用。

第一层提供事实，第二层形成当前方法，第三层决定它能否成为有意义的论文贡献。依据：[当前计划](../../研究计划.md)、[具体方法与判断规则](control-position-method-20261015.md#具体方法)。

## 仓库的五类研究内容

| 内容 | 已积累的工作 | 在论文中的作用 |
| --- | --- | --- |
| 实验平台 | Genesys 2、CVA6、四 iDMA、RISC-V IOMMU、Linux；观测、驱动、构建和版本追溯 | 提供能够控制变量、定位事件和复现实验的基础 |
| 干扰表征与因果定位 | E1/E4 软件与硬件时间分离、batch 并发、E2 替换、E3 局部等待传播 | 说明真实问题及其成立条件 |
| 缓解与组合机制探索 | 节流、容量分区 P、命中旁路 N、PN 组合，以及准备、分段、所有权交接等候选 | 形成强对照、反例和设计边界 |
| 完整工作流评价 | M0 双方固定工作量、R107 普通恢复、stage117 包交付与缓存/consumer 链 | 防止以局部指标或遗漏成本制造收益 |
| 当前方法与接入 | A 的控制位置合同与源语义检查；A2 的地址/context/生命周期绑定及 SVA 适配 | 将前述经验变成事前可检查、可验证的部署判断 |

硬件修复和上游补丁是独立工程成果。它们能提高实验可信度，但其正确性回归不能直接充当调度机制的性能或新意证据。[历史成果账本](../out/research-reappraisal-20261015/history/asset-ledger.md)、[历史进度](research-progress-from-july-20261014.md)。

## 已有证据支持到哪里

| 证据 | 可以说明什么 | 必须保留的边界 |
| --- | --- | --- |
| E4 与自主 batch | 早期逐项供给共跑缺少 AXI 重叠；自主 batch 才形成约 99.7% 在途重叠并测到硬件慢化 | 总慢化不能直接归因 IOMMU；无 miss 共跑也可能有数据路径竞争 |
| E2 正式 64 项 | 特定 16 项 IOTLB 布局中，后台替换前台条目，导致首次再用 miss；正向探测中位数为 696/1483 周期 | 是该布局与首次再用的结果，不是全部稳态损失的解释 |
| E3 受控 v6 | 四对 64B cold 配对中，后台剩余服务和前台 hit 剩余等待同增 127、129、129、130 周期 | 单次启动、四对样本；支持本实现局部等待传播，不推广为普适 HOL 或自然 4KiB 全部慢化 |
| P/PN 实板 | 隔离替换历史后，PN 仍比 P 慢约 12.67%—12.79%，说明组合优化未必更快 | 剩余实板因果尚未闭合 |
| M0 完整模型 | 每路各 64 次自然完成；行为 DDR 中普通 DS 优先使 P/PN 的 3689 周期差额消失 | 是模型服务规则的干预，不是实际内存控制器已有能力，也不是 FPGA 加速结果 |
| A 四案八次源语义 | 1412 条原始事件覆盖发布次序、已发布前缀、删除冗余搬运及完整消费/释放 | 已验证逻辑与字节正确，时间预测仍未知；结构留出不能改称时间留出 |

直接入口：[E4](e4-results-review.md)、[batch](batch-results-review.md)、[E2 结果与原验收](../out/e2-results-review-20261003/RESULTS.md)、[E3 配对表](e3-controlled-results-review.md#41-cold-互补对的-d128--d0)、[PN 实板](composition-history-isolation-20261007.md)、[M0 原裁决](../out/workflow-g0-m0-20261010/root-result-acceptance.json)、[A 源语义原核验](../out/control-position-method-20261015/validation/source-event-verification.json)。

这些结果共同支撑“需要区分干扰来源、控制权限和最终任务结果”的研究动机；它们并不共同证明某个新控制策略已经有效。旧报告中较早的“E2 未证”“E3 未上板”等语句须按当时日期理解，不能覆盖后续验收。

## 当前主线 A

A 的核心问题可以用一句话表达：**给定真实任务和硬件接口，判断在哪里、何时还来得及改变什么，以及这种改变能否影响完整任务。**

它输入任务依赖、地址和上下文、有限容量、资源归属、已经承担的完成义务、控制器实际可见的信息、允许动作、服务条件和全部费用。输出应区分四种情况：普通入口足够、确实需要更早控制、某项动作已被既有承诺挡住、条件不足而无法判断。

例如，未来预取 A1 和当前计算所需 B00 都尚未发布时，普通发布程序可以决定先后；A1 已发布以后，后来程序不能把 B00 插回过去。这个事实只说明局部次序的权限。要进一步声称“必须把控制前移才能改善完整任务”，还要证明当前位置所有可实现策略均不满足目标，而付出费用的前移策略能满足。后继计算、输出或消费者也可能隐藏局部收益。[方法规则](control-position-method-20261015.md#具体方法)。

A 已有可执行源合同检查器，能查依赖、合法前缀、发布限制、冗余任务消除和消费闭包。通用协议、缓存和有限资源的完整时序后端仍未完成。因而当前最准确的成熟度是：**方法与源语义验证已有产物，真实部署和完整时间预测尚待验收。** [设计](../out/control-position-method-20261015/method/design.md)、[检查器](../out/control-position-method-20261015/method/analyze_contract.py)、[预测记录](../out/control-position-method-20261015/method/predictions.json)。

## A2 接入进展及状态差异

A2 为 A 提供真实地址、发布、完成和资源归还的实现边界。已定位的 DATE 关联 HeroSDK 使用显式 pin 和独立 IOVA 映射；这不能直接称为借用同一进程页表。当前平台另发展受限的单进程 pinned-mm 方案，并以普通 IOVA、同址独立 shadow 页表作对照。[来源与设计](a2-native-sva-binding-20261015.md)。

本次查档发现两层不同步的记录：

| 层级 | 实际看到的状态 | 应采用的表述 |
| --- | --- | --- |
| 已发布状态与 A2 来源验收 | 仍记录“新 ABI/driver 未实现” | 这是来源绑定阶段的验收快照 |
| 当前工作树 | 已有 `idma_sva.h`、`idma_sva_task.h`、用户任务程序、ioctl 和构建/rootfs 接入 | 适配代码已经落盘，不能再笼统说完全未实现 |
| 实施目录的记录 | 软件构建完成；13 项用户任务主机检查、17 案驱动替身检查、4 案 CQ 本体 RTL 检查 | 各层记录分别有效，不相加成 native 通过数；本次没有复跑 |
| 最终验收 | 查档时实施目录无 `root-result-acceptance.json`，报告草稿仍有待填字段 | 不能代替实施任务作最终接受，也不能宣布真实 DMA 或性能完成 |

本次核对了构建锁中的五份归档产物（Image、vmlinux、rootfs、任务程序、DTB），其存在性与 SHA256 均匹配。驱动 SHA 在首次核对时与 17 案生命周期记录相符，交付核对时工作树又发生变化；该检查结果只属于记录中的版本，不能直接继承给最新源码。构建锁明确记录新内核运行、真实 DMA、FPGA 加载均为 false。这是对既有记录及产物的核对，不是重新构建或运行。

入口：[实际 ioctl 接入](../linux/drivers/misc/idma.c)、[私有 ABI](../linux/include/uapi/linux/idma_sva.h)、[任务驱动](../linux/drivers/misc/idma_sva_task.h)、[用户程序](../bench/idma_sva_task.c)、[构建锁](../out/a2-sva-implementation-20261015/kernel/build-lock.json)、[主机检查](../out/a2-sva-implementation-20261015/task/host-checks.json)、[生命周期记录](../out/a2-sva-implementation-20261015/task/lifecycle/results.json)、[CQ 记录](../out/a2-sva-implementation-20261015/model/cq-check-results.json)。[实施报告草稿](../out/a2-sva-implementation-20261015/root/report-draft.md)仅作进度导航。

功能输入是八页 arena、三包四段搬运，载荷 1639B，文件输出 1675B。普通 CPU 或原 buffer 直接 writev 也能满足这个输出合同；因此它适合验证功能与生命周期，不能仅凭成功输出证明 DMA 或新方法有独有价值。当前同步实现也没有建立前缀消费与另一 lane 并行的完整性能案例。[输入合同](../out/a2-native-sva-binding-20261015/design/native-task-input.json)、[实施草稿的范围与后继](../out/a2-sva-implementation-20261015/root/report-draft.md)。

## 备选 B1 和历史分支

B1 保留的问题是：未来源地址已经知道、旧 payload buffer 仍被计算占用时，能否有限地保存提前翻译结果，并在真实释放后安全移交，获得有意义的能力或完整成本差异。普通提前翻译已有前作，比较对象还包括有限 pin、prepared-PA 队列、已有缓存以及可合法复用的元数据。B1 当前是备选重设计，不是已通过的新机制。[B 候选卡](../out/research-reappraisal-20261015/candidates/b-development.md)。

历史分支应按失败类型保留价值：method88 是固定 p5 未胜强对照；R107 是普通准备已满足那张有限恢复卡；R2 是具体同预算合同没有动作/逻辑成本优势；RCS1 是原条件 engine 的完整性能负例，且有 native 许可范围勘误。它们分别约束具体主张，不否定整个共享翻译方向。[关闭范围表](../out/research-reappraisal-20261015/history/closure-scope.md)。

MIG 观测属于工程暂停：第六次构建时序未通过，没有取得新的有效实板债务记录。因此实际债务未知。这里不能把“未采到”写成“没有”，也不因本次整理恢复任务。[暂停记录](../out/mig-pte-debt-observation-20261015/root/goal-pause.json)。

## 最接近文献怎样约束贡献

以下复用仓库已有全文与逐篇证据卡；本次仅定向复核决定性段落和部分原始文献入口，不声称重新完成系统性文献综述。

| 文献 | 已有能力及原文位置 | 对本研究的要求 |
| --- | --- | --- |
| [Bermuda 2025](https://arxiv.org/abs/2508.19670v1) <!--ref:bermuda2025--><!--anchor:section:III-V--> | 商用共享 IOMMU 干扰实测，§III—V | 再次表征干扰之外，要增加可执行干预或有预测力的解释 |
| [MASK 2018](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf) <!--ref:mask2018--><!--anchor:section:5.2-5.4--> | TLB、页表缓存与翻译/数据联合调度，§5.2—5.4 | “优先翻译”或“同时考虑数据”本身不能作为新增贡献 |
| [DWS 2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) <!--ref:walk_stealing2021--><!--anchor:section:V-VI--> | walker 分区与借用、局部交错限制，§V—VI | 需要说明跨边界、有限资源与完整任务之间的实质推进 |
| [Orchestrated 2023](https://libingyao.github.io/assets/files/DAC2023.pdf) <!--ref:orchestrated2023--><!--anchor:section:IV-V--> | 调度、TLB 分区与共享，已有完整应用评价，§IV—V | “局部指标改善不等于应用改善”也不是独立的新发现 |
| [MMU-aware DMA 2018](https://arxiv.org/abs/1808.09751v1) <!--ref:socdma2018--><!--anchor:section:IV-A--> | 进度感知、有界、无 payload 访问的翻译预取，§IV-A | B 要比较结果跨 owner 保存/提交的具体能力和费用 |
| [Accelerator-Centric Translation 2017](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf) <!--ref:accelerator_translation2017--><!--anchor:section:IV-B-IV-D--> | 分级 TLB、hit-under-miss、host PWC/cache，§IV-B—D | 普通缓存、驻留和复用必须获得公平机会 |
| [RISC-V SVA 2025](https://arxiv.org/abs/2502.17398v1) <!--ref:riscvsva2025--><!--anchor:section:III-IV--> | 类似 RISC-V 平台上的页表缓存与 payload 绕行，§III—IV | 区分 kernel 与完整任务；计入映射、供给及缓存交接 |

进一步的 AXI 响应时间分析、AXI-ICRT 和 AXI-REALM 已有有界服务、队列选择、外置调节及 AW/W 成本分析。A 不能以事件图、最长路、在途工作或外置控制这些一般构件命名新意。[七篇原文证据卡 A](../out/research-reappraisal-20261015/literature/a-cards.md)、[证据卡 B](../out/research-reappraisal-20261015/literature/b-cards.md)、[关键分析差异与精确定位](../out/control-position-method-20261015/literature/analysis-delta.md)。

可保留的条件贡献论证是：**已有研究解决了翻译/数据联合调度、walker 共享和互连服务分析；本研究拟进一步从真实调用、地址上下文、发布承诺及资源归属中抽取可部署控制集合，判断何处仍有合法作用机会，并预测完整任务及费用。如果这种抽取和判断能在未用于设计的真实实例中导出有意义的新部署决定或结果差异，就形成具体的方法推进。** 现有源语义、接口绑定与局部实验为其提供基础，完整预测和相对最近分析方法的实质增量仍待验证。

## 下一步应围绕什么收敛

现有计划的关键依赖可整理为以下顺序，实施状态须先与正在形成的 A2 最终交付同步：

1. **确认真实消费者的必要需求。** 为什么需要 DMA 目标位置、设备消费或分段提前交付；哪些工作不能由原 buffer 直接消费或更简单程序消除。
2. **完成对应功能接入的验收。** 固定软件/硬件身份，核 DMA 完成、缓存可见、映射失效、异常排空及资源归还。代码和构建已有进展，缺的是相应原生运行证据；Hero 平台另有专用完成 gate 的实现缺口。
3. **冻结一个决定性完整比较。** 具名普通对照获得相同信息、资源和合法优化；服务与费用有依据后，再固定正例、相邻反例和两个时间留出，事前写明预测与失败规则。
4. **用结果决定贡献。** 有部署能力或完整成本增量则进入关键硬件验证和论文组织；普通方案满足同一需求则保留功能基线，重设计该具体任务/主张。

这里最大的待解问题是“真实任务为何需要这个控制，并且方法能预测出什么新增结果”。补齐工程链是检验它的必要条件，不能替代它。[总计划](../../研究计划.md)、[A2 时间验证合同](../out/a2-native-sva-binding-20261015/validation/full-task-contract.json)。

## 从哪里继续阅读

| 想了解什么 | 优先入口 |
| --- | --- |
| 当前正式状态与待办 | [CURRENT_STATUS](../../CURRENT_STATUS.md)；同时留意上文工作树增量 |
| 主线为什么这样选择 | [研究计划](../../研究计划.md)、[成果重评](research-reappraisal-20261015.md) |
| A 怎样作判断 | [具体方法](control-position-method-20261015.md) |
| 当前软件和接口进展 | [A2 来源绑定](a2-native-sva-binding-20261015.md)、[实施草稿](../out/a2-sva-implementation-20261015/root/report-draft.md) |
| 历史结果及其证明范围 | [从七月起的回顾](research-progress-from-july-20261014.md)、[资产账本](../out/research-reappraisal-20261015/history/asset-ledger.md) |
| 原始证据与验收 | 各报告链接的 `out/` 原件、[验证记录](validation.md)、[阶段交接](../../HANDOFF.md) |

本梳理由 Codex 使用 academic-research-suite 的证据核对与综合方式完成，一席 Sol/ultra 独占核查历史实证边界；未调用 DeepSeek 或 Claude。核对范围为指定文档、选定原始验收、当前实现入口和构建产物，不是全仓代码审计或新一轮实验。没有复跑旧测试、启动构建/板测或修改生产代码；文献复用状态及当前验收缺口保留在正文。AI 整理结果尚未经用户逐项确认。[本次范围与核对记录](../out/research-content-map-20261015/review-scope.json)。
