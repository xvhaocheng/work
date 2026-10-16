# D-BOUNDARY-SOURCE-2：SPDK 持久供给与可恢复边界

## Material Passport

- ARS-Codex / experiment-agent / plan，限定真实外部源码核查及研究投影。
- 日期：2026-10-15。建议与请求 `gpt-6.1-sol/high`；本席未取得独立运行模型/强度元数据，实际值 `unknown`。
- 本席 Codex-only。仅写本目录；0 网络取件、程序/模型/实验运行、生产修改、板操作、goal 操作。未调用 DeepSeek/Claude。
- 外部来源固定 SPDK v24.09，commit `19524ad45373e35d5fdc06f4e081444d87ceed80`。原件与精确摘录见 [source-evidence.md](source-evidence.md)，hash/阅读范围见 [source-manifest.json](source-manifest.json)。
- Verification Status：以下生命周期和分支为直接源码事实；部署、实际状态、时间和收益未测。源码是正常程序来源，不是本机 native trace。本结论仅限固定commit的所选perf PCIe路径，不外推其他程序或所有平台。

**结论：正常持续的完成→补交确实保留 task/buffer；但 stock perf 没有“联合排空、保留 pool、配置后继续同一任务”的现成阶段。自然阶段只有连续运行中的单笔完成、可选预热转正式统计，以及终止排空。前两者不保证全 owner quiet；第三者会释放 task/buffer 并销毁 qpair。** 所以严格的“stock 自然可恢复 quiet episode”来源卡不成立；可以明确新增普通 admission adapter 后研究持久阶段，不能把新增接口写成原程序已有能力。

## 1. 这三个 task 不能混叫

| 对象 | 源码身份与边界 | 持久性/结束 |
| --- | --- | --- |
| `perf_task` | `perf.c:170–184`；含 ns_ctx、iovs、metadata、submit_tsc、direction | 是可复用 slot。每次 callback 后同一对象再次提交，故一次 callback 不是这个对象寿命结束 |
| 一笔逻辑 IO | `1471–1529,1533–1600`；一次成功 API 提交到对应最终 callback | 成功提交才占应用在途/提交计数；CQ 状态与普通完成计数分开，错误 callback 也加 io_completed |
| 一次有限 workload/invocation | `1713–1871,3333–3398`；每 `(worker,namespace)` 发 QD，运行到 count/time/signal 等条件，再 drain/cleanup | `-d N` 是每线程每 namespace 的附加退出条件，仍强制 `-t`；N≥QD 且不可配 `-a`（`1920,2833–2837,2886–2893`）。不是现成可循环有限 episode API |

namespace 是存储地址空间；`ns_worker_ctx` 是 namespace×worker 的应用供给上下文。`3104–3170` 可让同 namespace 在多个 worker 出现、或一 worker 含多个 namespace，不能仅按 NSID 判全 owner 完成。每 context 还有多个 qpair（`1001–1069`）。研究 owner 若采用 context、设备或进程，须明确枚举全部受控制源。

## 2. 自然边界裁定

| 自然点 | 原文 | 是否保留 payload/task | 是否已经 all-owner quiet | 是否 stock 可继续 |
| --- | --- | --- | --- | --- |
| 普通成功 IO callback | `1539–1573` | 是，未 drain 则同 task 直接补交 | 不保证。QD>1通常尚有其他在途，且callback内马上补交；其他context/worker独立继续 | 是，连续补交 |
| warmup→measurement | `1748–1753,1811–1828` | 是，未重新 allocate/free | 否。只清 stats/histogram；没有停补交、drain 或跨 worker barrier | 是，持续同一供给循环 |
| count 到额 | `1522–1529` | 已在途对象暂存；后续 callback 逐个释放 | 设置 is_draining 当时否；其余请求与其他 context 仍可能在途 | 否，submit_single_io 断言非 draining，主循环/cleanup 无恢复分支 |
| 时间/exit→drain | `1843–1869` | 否，callback free payload/iovs/metadata/task | 只证明本 worker 各 context 的应用在途数归零；不是硬件共享通路 quiet 证书 | 否，紧接 qpair/poll-group cleanup 与 worker 返回 |
| main 等所有 worker | `3354–3390` | payload task 已走终止释放；qpair 已清理 | 全体 worker 结束；仍不能用此替代拟议 IOMMU/AXI/PTW quiet ack | 否，输出、detach、env_fini、退出 |

**QD=1 例外也不是免费现成边界。** callback 减在途后、补交前可能短暂为0；这只是一处可以插钩子的程序位置。源码无停留、全 owner 集合确认、共享 quiet 或配置 ack。QD>1 的正常无错不 drain 路径通常只是 QD→QD−1→QD，逐个 callback 补交，不会因一个 poll 批次完成了多个旧请求就自动形成持久空队列。硬件中可能暂时无请求也不等于软件获得了可安全切换的承诺。

**暂停 poll 也不能代替暂停 admission。** 既有命令仍可执行；CQ 未消费不会使之前的 payload 失效，也不会给出全 owner 已排空证明。`-C` 只限每轮 completion 处理量；应用 submit 返回0与 trace SUBMIT 都不是硬件全链接受/退休凭据。库 trace COMPLETE 在 driver 在途减数与 callback 之前，qd=1 不能当 app quiet（`nvme_pcie_common.c:681–712`）。

`is_draining` 在所选 perf 全文件的赋值搜索中只有置 true；context 的初始 false 来自 calloc（`3108`）。终止时 `1566–1570` 直接 free 对象，`1867–1869→1087–1097` 释放 qpair。故把 true 改回 false 也不能恢复已经释放的 pool。重新调用 allocate/重启属于新寿命；allocator 恰好复用同地址不是同 buffer/mapping generation 的保证。应用 free 与底层 memseg unmap 不是同一事件，沿用既有映射审查，不据 free 假造逐 task IOMMU unmap。

## 3. 一次准备、每笔准备、阶段准备

| 范围 | 源码实际工作 | 判定 |
| --- | --- | --- |
| 一次 invocation 初始化 | env、worker、controller、关联 context（`3265–3337`）；每 context 建 qpair/poll group、等待连接（`1001–1069`） | stock 强制路径/条件分支依配置；成本不能靠报表窗口抹掉 |
| 每个 slot 初次准备 | worker 启动 barrier 后开始计时，QD 次 `allocate_task→setup_payload→submit`（`1737–1758,1603–1628`）；DMA allocation/填 pattern/iovs（`804–844`） | 对每个初始 slot 一次；正常 callback 不重分配、不重新 memset 整个 payload |
| 每笔 IO | offset/LBA 生成、submit_tsc、direction、qpair选择、API submit（`847–904,1471–1529`）；metadata场景有DIF准备/验证 | 普通运行的实际软件工作，不能把所有命令准备都算成初始化一次 |
| 可选 invocation warmup | 静态未显式初始化的 `g_warmup_time_in_sec` 为0（`245`）；`-a` 可设非零（`1889,2562–2564`） | time模式可不 warm 或 warm；warm 时连续做真实IO，结束只重置 stats/histogram（`1811–1828`）；并非每笔/每阶段强制 warm |
| count模式 | `2886–2893` 明确拒绝 `-d` 配 `-a`，拒绝 N<QD | 不应虚构“SPDK count每阶段先warm再N次”的 stock合同；可另定义计费投影 |
| 研究 quiet/config/clear/warm | 本次所核 perf 分支没有本平台模式与这套承诺 | 由root/接口席核当前平台；本来源不把它们设为0或当已有能力 |

SPDK 可无 `-a` 的事实**不改变固定 M0 的 warm256、COPY、reset或费用合同**。若另立持久 source 投影，须单独冻结其准备规则并重新验收。初始化 `memset` 不是设备 DMA 翻译预热；warmup 阶段也没有硬件状态唯一化/保持到下一阶段的保证。

启动 barrier `1737` 只发生一次、在初始 task 分配前。warm phase 由各 worker 本地时钟判断，没有同一 barrier 的循环使用；它不是一个全局 phase 切换协议。

## 4. 普通公开信息与物理状态

这里的 O_host 是源软件依法可获取的信息集合，O_phys 是绑定物理服务后更细的事务/资源状态；两者都不是“本席目前拿到的文件列表”。可获取但尚未记录的字段不能因此被隐去。读出、保留全历史及计算有成本时，候选与基线按相同规则付费。

| 信息 | 最早可得位置/可作的推理 | 不可自动升级成什么 |
| --- | --- | --- |
| 命令/配置 | QD、IO大小、namespace/context/qpair、方向、顺序/随机规则；已生成 offset/LBA 在 API submit 前可见（`1471–1507`） | 随机 LBA 不是随机 host IOVA；未来服务时间不是命令字段 |
| buffer布局 | task/iovs在初次 setup后已知；同 slot 后继复用可推知；mapping、leaf、页覆盖在真实取得后应公开给基线 | VA、设备IOVA、CPU PA与DRAM bank/row不是同一字段；源码指针不是实际PA |
| 完成历史 | 软件能知道已提交、最终 CQ 状态、poll/callback时刻、当前在途与offset；可以做时间序列/队深/命令感知预测 | io_completed含失败；聚合延迟不是完整成功史；未来callback不可由事后trace免费提供 |
| cache/translation历史 | 已知页覆盖与完整合法访问/失效历史可以供普通可执行cache/冲突预测；明确确定模型可重建的状态应给强基线 | pool常驻不保证IOTLB常驻；不能仅凭没日志宣布cache状态信息论不可知 |
| 物理DRAM与共享结构 | 若有真实PA布局、所有相关流量、refresh与控制器规则，可尝试重建/估计行状态；应逐项验模型充分性 | host提交/完成史一般没有直接给每笔payload、PTE、仲裁、row-buffer的实际服务序列；stock schema也无这些字段 |

实际映射、actual leaf、Requester→物理共享IOTLB/PTW的缺项直接沿用 [mapping/findings.md:72](/home/xvhaocheng/work/iommu-platform/out/d-spdk-contract-20261015/mapping/findings.md:72) 第72–95行；本席未重调VFIO、未探机器。不把group/container/domain名称当共享缓存证明。原文件阅读范围在manifest中可核。

stock trace 的库schema有命令片段、ctx/CID/QID、qd/status，缺payload VA/IOVA和应用phase/drain/free等关系；既有 [events/findings.md:41](/home/xvhaocheng/work/iommu-platform/out/d-spdk-contract-20261015/events/findings.md:41) 第41–56行已分类。本席未取得实际trace，也未由缺trace推论原始状态不存在或永不可推断。

一个可检验但**未确认有效**的 source relation 是：上一阶段“哪个成功IO完成/哪个持久slot待补交”与“该owner payload在另一owner页表服务等待期间实际获得/占住共享服务”的关联，而非只统计LBA或总miss。前半是公开源历史；后半需要绑定后的硬件观测/新探针。它只有在给出具体部署或明示模型、排除由同O_host/布局可重建、证明跨真正quiet与规定准备仍影响下一阶段后，才可能提供增量；不预设持续内部row/cache状态，更不写已观测正例。

## 5. 最小可检验研究投影：保留 pool 的 admission adapter

**以下均未实现，属于可提交root裁决的研究接口；不是stock或当前平台已经支持。** 可复用 [design/proposal.md](../../d-spdk-contract-20261015/design/proposal.md) 的源保真层次，但将终止drain与可恢复phase分开，不让旧终止语义原型自动取得pause能力。

1. 保留真正的 `is_draining` 作为最终终止/错误路径。另加 `RUNNING / PAUSE_REQUESTED / QUIESCED / RESUMING` 与 `idle_tasks`，在callback退休成功代际后，暂停时将task置idle，既不补交也不free。失败仍报失败，不凭完成计数充成功字节。
2. 每个owner显式封住新admission，保留 QD容量和全部pool；已accept的请求继续处理CQ/poll/callback。owner内 `inflight=0 && pending-submit=0`、所有slot均idle、无错误；所有受控owner集合ack后再求共享通路quiet。不得仅暂停某一个worker或只关一个namespace。
3. shared quiet 必须覆盖该研究通路的payload、命令/CQ、TL/PTW、AXI在途/held offer与返回归属等，并区分源ack与硬件ack；具体条件由接口席核。若PCIe qpair仍有待doorbell的延迟提交或driver queued request，应用callback计数之外也须纳入adapter合同。保持poll直到安全边界，不能靠停止poll骗quiet。
4. 读取公开历史/付费信号，执行研究动作配置/clear/准备并确认完成，再对全部owner发布resume；用同task/buffer生成新代际和新命令，沿各自动作实际完成顺序补交。源被暂停的等待时间、重新启动相位与状态扰动全计；不能照抄另一动作callback日历。
5. 有限phase的N只用于明确的新phase工作预算；结束phase不free，最终invocation终止才按原生命周期free。若边界请求按自然应用phase而非任意N产生，需另给真实调用方来源；perf本身未提供这种反复业务phase。这一投影是人为定义的研究分段，不改称自然原生episode。

新增费单：pause/resume消息与协调、所有owner尾部/慢者等待、idle队列/代际状态、保留pool/锁页与qpair资源、CQ/poll/CPU、quiet证明及模式配置/clear/warm、观测读出/计算/存储、恢复burst与干扰、最终free/detach。全部为 `unknown`，不能填0。一次环境/映射初始化可以单列或按预先固定的episode数分摊，但最终报告保留总额；无限流没有自然有限makespan，需事前定义有限观测/完成目标。

普通同接口对照必须也能停止补交、保留pool、按公开命令/布局/成功历史预测或付费pilot，并支付同样边界费。接口能力或策略可被普通程序复现分别裁决；没有新意/收益接受结论。

## 6. 接受与否决范围

- 接受：真实普通循环保留slot/payload；自然completion驱动后继；warmup可选且continuous；terminal drain释放对象并cleanup，stock没有保pool恢复分支。
- 否决当前来源版本：把单task完成、warmup统计切面或terminal drain直接当“已具备全owner quiet、保pool、下一阶段可选mode”的自然边界。原因是直接原文分支冲突，不是缺实测的泛化否定。
- 条件保留：明示新增且计费的持久pool/admission adapter来源投影。明确模型假设后可做语义/机制证明，不要求先有外部真实jointtrace；若声称原生可部署、真实持续状态、真实收益，则需对应运行/拓扑/测量。
- 本席无权接受平台模式切换、clear、warm合同或D优先级；交root合并。实际trace、phys mapping、跨边界持续性、O_host下预测缺口和完整正反收益仍未知。
