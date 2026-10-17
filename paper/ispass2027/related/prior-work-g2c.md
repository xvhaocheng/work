D1：部分重叠；D2：部分重叠；D3：已被覆盖（合计与个体性能并报的方法）。检索截至2026-10-18（JST），裁决限于下列来源及实际阅读范围，未决全文不视为已排除。

# G2c：附加延迟、失效范围与合计吞吐的增量前作核查

## 1. 范围与证据口径

本报告只核D1–D3，并追取指定未决全文；不重做[G2](prior-work.md)或[G2b](prior-work-g2b.md)，不改两者已有结论。主窗口为2016年至检索日，必要时追溯早期IOMMU失效工作。检索、取得原件、结构预检、读过目标节、读完论文和复现实验分开记录。本次不运行实验、仿真、工件或构建；只在`related/`新增本文并给两份旧报告追加链接。

对照沿[逐实验总账](../../../docs/experiment-ledger/README.md)及[后续修正](../../../docs/experiment-ledger/corrections.md)定位。本文核对的是原报告的数字与含义，不重新归约原始实验，不为旧版本补写身份、时序或因果证明。页码均指从1开始的PDF物理页；有结构告警的文件仅按已核章节/图表引用。

| 主张 | 本项目原证据及核准含义 | 本文不接受的外推 |
|---|---|---|
| D1 | [四点统计](../data/ds-sweep-summary.md)与[执行报告“完成结果与证据”](../board/ds-sweep.md)：每档五条件各10次，DS0/50复用原100次，DS25/100新增100次；同DS/同block计算`T_V(Ak)/T_V(A1)-1`再取中位 | DS是数据结构访存通路每方向附加周期，包含延迟器的通路效应，不是隔离出的单次PTW成本；DS0为直连，正值启用诊断结构。四点不证明跨平台线性定律或真实CXL部署效果 |
| D2 | [宽/细失效README“干扰大小”“开销”](../../../iommu-platform/out/invalidation-interference-20261017/README.md)、[strict/FQ/ORACLE README§1–3](../../../iommu-platform/out/dead-translation-20261017/README.md)及[IOFENCE更正](../../../iommu-platform/out/iofence-interference-20261017/README.md)：两套命令序列的局部RTL/行为DDR仿真，CPU/Linux未运行 | 14.8%–17.6%属于本地/开发分支宽失效序列，不能称Linux主线默认代价；旧A增时不证明PR/PW drain。FQ与ORACLE相等不等于FQ相对strict没有收益，详见§3 |
| D3 | [统一100次补充分析§1及“口径不确定性”](../analysis/baseline100-extra.md)：同一V硬件窗口内A的B响应折算16MiB，严格W字节界限为±4096B；合计/Solo按同phase、同block配对后取中位 | 它是payload完成量proxy，不是物理DDR总字节；合计不单独证明公平，也不能独立归因翻译成本。A1的105.579%不是严格守恒 |

## 2. D1：附加DS延迟与远端页表推论

**裁决：部分重叠。** 远端PTE访问延长walk、持续占用walker并增加排队的链条，MGvm已有明确原文；内存放置、翻译局部性与负载平衡也有前史。本文可保留的是独立DMA共享RISC-V IOMMU、同档A1控制下的四点FPGA敏感性。它不能直接升级为Chiplet/CXL系统的一般性能结论，CATO全文缺口尤其限制这一外推。

| 前作及链接 | 平台 | 机制 | 实验 | 结论 | 与本文差异 |
|---|---|---|---|---|---|
| [Designing Virtual Memory System of MCM GPUs（MGvm），MICRO2022](https://doi.org/10.1109/MICRO56248.2022.00036)，[作者稿](https://www.csa.iisc.ac.in/~arkapravab/papers/MICRO22_MGvm.pdf) | 4-chiplet GPU仿真、15工作负载（§I，PDF2） | 动态home-slice映射及leaf PTE局部放置；必要时细化映射平衡压力（§I，PDF2） | 私有/共享TLB，PTW数/容量与互连延迟敏感性；延迟16/32/64ns（§VI-C/Fig.12–13，PDF12–13） | **远端PTE使walk更长、walker占用更久，继而增加PTW/TLB排队**（§I，PDF2）；延迟变化改变相对收益（§VI-C） | 已直接覆盖D1通用放大解释和延迟敏感性思想；不是独立DMA共享IOMMU、同档all-hit A1控制下的FPGA曲线，也没有证明跨平台固定线性系数 |
| [Mitosis: Transparently Self-Replicating Page-Tables for Large-Memory Machines，ASPLOS2020](https://doi.org/10.1145/3373376.3378468)，[作者稿](https://apanwariisc.github.io/publications/asplos-2020-mitosis/mitosis-asplos20.pdf) | 四socket Xeon E7-4850 v3、512GB、Linux NUMA实机（§2.2，PDF3） | 页表复制/迁移；关键对照独立控制页表与数据放置（§3.2/Table2，PDF5） | 7组local/remote及STREAM带宽干扰配置、4KiB/2MiB、perf执行与walk cycles（§3.2，PDF5–6） | 页表远端放置与带宽竞争已联合实测；2MiB仍可能受远端页表影响（§3.2/Fig.6） | 是D1更早的放置/竞争控制前史；CPU NUMA而非DMA/IOMMU，非CXL；文中合并表述的3.3×不能当单独的干扰增量，也不是本文DS线性系数 |
| [Barre Chord: Efficient Virtual Memory Translation for Multi-Chip-Module GPUs，ISCA2024](https://doi.org/10.1109/ISCA59077.2024.00065)，[作者稿](https://seonjinna.github.io/assets/pdf/BarreChord_isca24.pdf) | MCM GPU经PCIe ATS访问host IOMMU，MGPUSim，16 PTW基线（§II-A/Fig.3、§III-A/B，PDF2–3） | 跨chiplet翻译合并/抑制，减少PCIe与PTW压力（§IV-A/B，PDF4） | 2/4/8/16 chiplets及8/16/32 PTW敏感性（§VII-H1/H2，PDF11） | 更大MCM带来更多PCIe/PTW竞争，减少翻译需求可缓和（同节） | 已有加速器IOMMU共享压力评价；同一GPU的chiplets不同于两个独立DMA；所核节未评本文每方向四档延迟的实板控制 |
| [Leveraging Chiplet-Locality for Efficient Memory Mapping in Multi-Chip Module GPUs（CLAP），MICRO2025](https://doi.org/10.1145/3725843.3756090)，[作者稿](https://zoon17.github.io/pdfs/CLAP_micro25.pdf) | 四chiplet GPGPU-Sim v4.2，4KiB/64KiB/2MiB（§3.1–3.2/Table1） | 联合chiplet局部性与内存映射（§3.3–3.5、§5相关段） | 页大小、数据布局、远端访问及性能（§3.3/Fig.6–8）；结构UNAVAILABLE，只用节/图锚 | 大页扩大reach却可因放置变粗增加远端**数据**访问，抵消翻译收益（§3.3） | 不是纯PTE延迟sweep；Fig.6远端率是memory instructions，不能改写为PTE专属比例；非独立DMA或真实CXL |
| [PIMFY: Eliminating Remote Page Walks in MCM GPUs，ICCD2025](https://doi.org/10.1109/ICCD65941.2025.00037)，[机构摘要](https://yonsei.elsevierpure.com/en/publications/pimfy-eliminating-remote-page-walks-in-mcm-gpus/) | **仅摘要**：MCM GPU，具体模拟器/配置未核 | 依据kernel内静态映射，在chiplets复制页表（Abstract） | 摘要列walk延迟与性能评价，未得正文 | 摘要将远端PTE与NUMA性能损失联系；完整竞争/敏感性评估未知 | D1直接摘要级近邻；不能因摘要未写独立DMA就排除正文覆盖，需学校或作者原稿补查 |
| [Clover: Storage-Efficient Page Table Replication in Wafer-Scale GPUs，CAL2026](https://doi.org/10.1109/LCA.2026.3718473)，[作者摘要](https://skku-compasslab.github.io/compasslab/publications/papers/260728-cal-clover/) | **仅摘要**：6×6 wafer GPU mesh、16 irregular应用，具体评估实现未核（Abstract） | 拓扑感知k-hop页表副本，缩短leaf PTE跨mesh访问（Abstract） | 摘要比较全复制/受限复制的时间和存储；正文未得 | 已将远端PTE路径/复制与性能联系，完整排队分析未知 | 非独立DMA；作者公开目录只有摘要/配图，无PDF。不能据摘要排除正文的竞争评价，也不采用模板里被注释的无关DOI |
| [Mitigating Address Translation Overhead in Near-Data Processing on CXL Memory（NDSP），Gwangin Kim，DGIST硕论2026](https://doi.org/10.22677/THESIS.200000944685)，[机构记录](https://scholar.dgist.ac.kr/handle/20.500.11750/59726) | **仅机构摘要**：CXL NDP，全系统模拟 | 设备ShadowPT与OS notifier同步，减少ATS/I/O PF/PCIe往返（Abstract） | 摘要列OLAP/KV/graph及4KiB/2MiB；未读正文 | 远端/host翻译往返的成本与设备端翻译已被研究 | 不能写成真实CXL板测；独立设备数、共跑及延迟sweep未知。[公开viewer](http://dgist.dcollection.net/common/orgView/200000944685)可访问，但本轮未取得PDF；不是已证必须校园权限 |
| [CATO作者概述](https://skku-compasslab.github.io/compasslab/publications/papers/260710-micro-cato/)；[MICRO2026题录](https://www.microarch.org/micro59/program/) | local DRAM/CXL二层服务器；方案完整评估的实机/模型分工尚未核 | 动态翻译卸载、CMM中的末级PTE hash副本（作者概述“Proposed Solution”） | 概述报告无竞争/有竞争下页表放置对比；未得论文方法和结果节 | 概述指出拥塞下本地DRAM可比CMM慢，卸载翻译还可减轻数据竞争（概述“Problem”“Impact”） | **仅作者概述，非论文全文**；已足以要求保留“远端可能反而更快”的条件，不能核定其是否覆盖本文双DMA控制/曲线或算法收益。全文仍未决，详见§5 |

### 2.1 本项目四点究竟支持什么

[DS执行报告“完成结果与证据”](../board/ds-sweep.md)给出的相对同档A1额外慢化如下；这是配对比值的中位数，每点n=10，不是两个中位数之比。

| DS，拍/方向 | A8 | A16 | A64 |
|---|---:|---:|---:|
| 0 | 10.3962% | 13.7155% | 16.3512% |
| 25 | 24.3199% | 31.4304% | 36.8189% |
| 50 | 44.7831% | 52.0975% | 57.6313% |
| 100 | 76.0753% | 88.7220% | 98.1181% |

原报告已经区分形状：A64的分段斜率接近，A8/A16高延迟段边际增长趋缓；不能笼统把全部负载写成已验证的近线性定律。Solo/A1各档中位数接近，是当前样本的负控制观察，不是统计上证明“绝不受影响”。50MHz下100拍是每方向2µs，不能直接当成某个CXL链路的访问延迟；本轮也没有CXL协议、实际远端页表放置/复制、异构缓存与带宽变化。DS0直连与正值诊断结构的区别、分boot采集和IQR的描述性含义均须保留（同报告“固定输入”“统计口径”；四点统计“Sources and limits”）。

MGvm说明等待放大的机制已有；CATO作者概述说明放置改变拥塞时，远端不必然更慢。因而论文可提出**需要把共享翻译争用纳入远端页表设计评价**，不能据这一延迟器实验声称已经验证“Chiplet/CXL部署会显著放大跨设备干扰”，也不能宣称线性斜率可迁移。

**建议中文（一句）：** 在所测FPGA共享IOMMU配置中，将数据结构通路每方向附加延迟从0增至100拍，使A8/A64下相对零miss共跑控制的额外慢化分别由10.4%/16.4%增至76.1%/98.1%，提示远端页表设计应评估共享翻译干扰。

**Suggested English (one sentence):** On our FPGA platform, increasing the added data-structure-path delay from 0 to 100 cycles per direction raises the victim's extra slowdown over the co-running all-hit control from 10.4% to 76.1% for A8 and from 16.4% to 98.1% for A64, motivating evaluation of shared-translation interference when considering remote page tables.

### 2.2 CATO追取结果

G2/G2b保留的旧作者URL为404；本轮从作者公开仓库找到多一层`papers/`的[正确页面](https://skku-compasslab.github.io/compasslab/publications/papers/260710-micro-cato/)，并直接读[固定提交中的概述原件](https://github.com/SKKU-COMPASSLAB/compasslab/blob/c76259fd469010086c44b66046e639a6c2211b48/content/publications/papers/260710-micro-CATO/index.md)。该目录只有概述与配图，无目标PDF；正文前的`pub_url`只是被注释的其他论文示例，不能冒用为CATO出版号。MICRO官方Session8B确认题名/作者，但当前题录没有论文链接。作者网页日期也不是已经正式出版的证明。

本轮解除的是**作者概述不可达**，不是论文全文缺口；未改写旧报告的历史访问状态或裁决。论文的DMA/设备并发形式、页表/缓存参数、延迟与竞争如何独立控制、模型及硬件证据分工、局限与相关工作仍需正文核查。不得从概述填出这些未报告参数。

## 3. D2：失效范围、发起方成本与死条目

**裁决：部分重叠。** 范围与命令数的取舍、deferred/FQ摊销，以及失效影响仍运行DMA的翻译，均有直接IOMMU前作。可保留的是具名RISC-V IOMMU/双iDMA模型中的同/异域、宽/细范围及双方完成时间比较，另加特定替换实现下ORACLE的负结果；不能把这些条件百分比当新失效方法。

| 前作及链接 | 平台 | 机制 | 实验 | 结论 | 与本文差异 |
|---|---|---|---|---|---|
| [Fast & Safe IO Memory Protection，SOSP2024](https://www.qizhecai.com/papers/fands.pdf) | Linux6.0.3、Mellanox NIC；同NIC Tx/Rx共享IOVA空间（§2.1–2.2） | 保留无需失效的PTcache，连续IOVA使descriptor可一次range invalidation（§3/Fig.4–6） | 测Tx/Rx、IOTLB/PTcache miss、吞吐及机制消融（§2.2/Fig.2–3；§4.1/4.3） | **Tx ACK的unmap清共享PTcache，使Rx翻译miss增加**；失效相关跨DMA影响与范围/缓存保留优化已有实测（§2.2） | 最强近邻；原文是同设备两方向/同IOVA空间，不是本文不同PSCID的宽失效矩阵，但不能据此忽略机制重叠。结构UNAVAILABLE，仅用节/图锚，见§6 |
| [rIOMMU，ASPLOS2015](https://doi.org/10.1145/2694344.2694355)，[作者稿](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf) | NIC/Linux与拟议ring翻译组织 | 每ring有限rIOTLB驻留、隐式替换旧翻译、burst末失效（§4，PDF6、8） | Table1测strict/deferred失效费用；§5.1以CPU busy-wait模拟拟议硬件失效成本（PDF5、9） | 失效摊销及安全回收已有；拟议硬件非实际rIOMMU芯片 | 与本文都含模型证据，不能因“仿真”排除；其模型不评共享RTL CQ/DDR取数争用，也没有本文PSCID/地址矩阵 |
| [Utilizing the IOMMU Scalably，ATC2015](https://www.usenix.org/conference/atc15/technical-session/presentation/peleg)，[作者稿](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf) | Linux3.17.2、16核与10Gbps NIC（§6.1，PDF9） | per-core deferred队列，降低共享队列锁争用；flush完成前保留IOVA/页表（§3.2、§5，PDF4–5、9） | 软件伸缩性及系统性能（§6，PDF9、12） | 250项或10ms批刷及整刷清仍有效项的代价已有；该版本阈值不是当前Linux阈值（§3.2） | 直接FQ前史；本文增加硬件模型里的邻居完成时间与命令取数分解，不是发明延迟刷新 |
| [Amit，iommu/amd: Tailored gather logic for AMD，2021-06-07原始补丁邮件](https://lkml.rescloud.iu.edu/hypermail/linux/kernel/2106.0/08115.html) | AMD native/vIOMMU软件路径 | 依硬件特性选择gather/flush范围 | 原始工程说明与新增函数注释；非双设备性能论文 | 邮件说明及函数注释**明确权衡较宽flush带来更多miss与更少flush次数**；虚拟化的PTE同步费用会改变选择 | 直接覆盖D2一般取舍；没有本文双方完整周期百分比。技术披露可作前史，不当作已证普遍最优策略 |
| [IOMMU Deferred Invalidation Vulnerability: Exploit and Defense，DATE2024](https://doi.org/10.23919/DATE58400.2024.10546528)，[会场原件](https://past.date-conference.com/proceedings-archive/2024/DATA/734_pdf_upload.pdf) | 硬件strict/deferred背景测量；两设备/两domain的QEMU攻击与提案 | unmap时按IOVA/sourceID使旧项不可用，继续deferred flush（§III–IV，PDF2–5） | TableI为硬件背景；HW-INV提案性能在QEMU（§V–VII/Fig.4–5，PDF4–6） | deferred窗口有生命周期安全风险；即时阻止旧项使用已有机制 | 旧项不可用不等于为替换腾槽，安全隔离也不等于时间隔离；不能用本文ORACLE零收益否定该安全方案，不能把QEMU提案收益写成硬件实现 |
| [Amit/Ben-Yehuda/Yassour，IOMMU: Strategies for Mitigating the IOTLB Bottleneck，WIOSCA2010](https://nadav.amit.zone/publications/pdfs/amit2010iommu.pdf) | 含两E1000并发、各自domain；轨迹/IOTLB模型（§3.2，PDF5） | transient映射的Eviction Hints及替换策略（§4.1，PDF5） | Fig.12与§4.8–4.9（PDF10–11） | hints收益小，多NIC时可降低hit率；最佳替换也不代替prefetch | “瞬态占用可驱逐不等于有可恢复收益”已有近邻；其hint操作不同于本文unmap后清valid，未做PSCID/address失效完成时间矩阵 |
| [IOTLB Wall，PeerJ CS2023](https://doi.org/10.7717/peerj-cs.1385)，[作者稿](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) | Linux5.15网络、deferred失效部署（方法，PDF8–9） | IOTLB覆盖、buffer局部性与软件配置 | 数据面吞吐/miss/CPU防瓶颈控制；G2b已读，本轮复核方法 | 数据面miss、CPU费用和模式必须区分 | 未给本文scope矩阵；它的deferred默认不能替代当前RISC-V strict来源判断，不重复G2b反馈线 |
| [Intel VT-d Rev5.0](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf)；[RISC-V IOMMU v1.0.1/20260222](https://docs.riscv.org/reference/iommu/v20260222/_attachments/riscv-iommu.pdf) | 架构规范，无实验 | Intel domain/page/PASID、IH及范围drain；RISC-V PSCID/address、范围扩展 | Intel §6.5.2.3–4/PDF116–117、119–120，§6.5.4/PDF136–137；RISC-V §4.1.1–2/Table11/PDF52–55及[Address Range Invalidation](https://docs.riscv.org/reference/iommu/v20260222/iommu_extensions.html) | 细粒度失效及有关完成语义已存在；实现可以更粗 | 否决接口新颖性，不代替实现性能测量；Intel最低drain保证不证明不等待其他域，更不能补全本文PR/PW模型前沿 |

### 3.1 三组数字必须分开

1. **旧宽/细范围轮次。** M0是含A容量/替换压力的无失效基线，不是无A Solo；M1使B完整时间由199740/204875变成234869/235189拍，即17.587%/14.796%。M2仅在异PSCID保护B；同域仍受影响。M3域内按地址可保护不重叠翻译。0/8是定向条件的总walk数，**仅失效归因miss归零**，不是所有翻译都命中（宽/细失效README“干扰大小”）。
2. **旧发起方代价。** A完整操作含最后unmap/fence，M2异域/M3为213675–214261拍，相对M1的194749拍慢9.718%–10.019%；两组命令数相同。后继诊断将增量主要定位到CQ命令取数；旧路径PR=PW=0、两条fence，不能叫已测payload drain（宽/细README“开销”；IOFENCE报告“旧数据的归因”）。
3. **主线来源行为模型。** 固定Linux `58161ead29e5`的strict对应局部RTL中，B为204205/209940拍，相对M0慢2.235%/2.472%。FQ为199740/204875，**相对strict快2.19%/2.41%**；ORACLE与FQ完全相同。故“FQ没有收益”过宽：零增益指ORACLE相对FQ；3%旧判断阈值未通过不等于真实差值为0（dead-translation README§1、§3）。

FQ这轮128条入队，没有达到255条容量阈值，工作窗口又短于10ms/500000拍；刷新尾部至501475拍才排空，原报告单列。FQ的A工作完成不能当包含全部回收尾部的完成时间；这些结果不覆盖timer反复触发、并发flush或多CPU部署。ORACLE将死驻留占比从FQ的平均36%–47%降到近零仍无收益，但原PLRU不优先挑invalid空项；结论限该实现，不是“死项普遍无害”（同README§1、§3）。

两轮均为真实双iDMA/CQ/MMIO RTL与行为DDR，28配置各一次，没有运行CPU/Linux或上板；主机页表写未完整计CPU费用。PR/PW测试前沿只计COMP AR/AW已接受者，可能漏掉已翻译、尚未越过COMPLETE/demux的请求；旧组是否触发未知，不能冒称完整规范实现。后续26组计划未运行，本文不恢复它（IOFENCE报告“模型…覆盖边界”及“未测”行）。

### 3.2 可保留的论文表述

截至检索日在上述已读段落中，未发现本文这组共享RISC-V IOMMU、同/异PSCID、范围序列和双方完成时间的同配置评价；FBI仍无全文，故此判断有未决边界。跨DMA失效影响、scoped invalidation与批处理原则本身已有上述前作。

**建议中文（一句）：** 在所评估的RISC-V IOMMU局部RTL模型中，将失效收窄至不覆盖邻居翻译的范围可消除失效引起的邻居miss，但可能增加发起方完整操作时间，具体代价取决于所建模的Linux命令序列。

**Suggested English (one sentence):** Within the evaluated RISC-V IOMMU RTL model, restricting invalidations to exclude the neighbor's translations removes invalidation-induced misses but can increase the initiator's full-operation time, depending on the modeled Linux command sequence.

## 4. D3：合计吞吐与个体份额

**裁决：已被覆盖，针对指标与比较思路。** 同时检查个体性能、总资源利用和总量损失已有直接先例；本项目的增量是具名DMA配置的量化与窗口记账，不是一个新指标。不同论文的IPC、网络payload和物理内存流量不能直接互换。

| 前作及链接 | 平台 | 机制 | 实验 | 结论 | 与本文差异 |
|---|---|---|---|---|---|
| [hostCC，SIGCOMM2023](https://doi.org/10.1145/3603269.3604878)，[作者稿](https://www.cs.cornell.edu/~ragarwal/pubs/hostcc.pdf) | Cascade Lake/100Gbps NIC/PCIe/DDR（§2.2，PDF3） | MBA调节MApp份额，联合主机及网络拥塞控制（§4.2，PDF8） | Fig.9并报NetApp-T与MApp吞吐、二者内存带宽堆叠；§5.1分析总利用率（PDF8–9） | 提高一方可能过度压低另一方；level3→4，NetApp-T多5.2GB/s而MApp少13.8GB/s，作者据此指出粗粒度分配导致总利用率损失（§5.1，PDF9） | 直接否决“看双方总量以区别转移与损失”作为新方法；它未采用本文固定COPY字节/同V窗口/Solo配对公式。应用吞吐与内存负载折算不同，原文明确说明（§4.2） |
| [Understanding Host Interconnect Congestion，HotNets2022](https://doi.org/10.1145/3563766.3564110)，[会场稿](https://conferences.sigcomm.org/hotnets/2022/papers/hotnets22_sagarwal.pdf) | NIC/IOMMU/CPU内存互连 | 分别改变IOMMU与Stream竞争（§3.1–3.2） | Fig.6同时绘网络payload吞吐、总内存带宽与丢包，IOMMU开/关（PDF5） | 共享内存竞争会降低网络利用；与翻译竞争联合时退化更大（§3.2，PDF5–6） | 是资源占用与有效吞吐并报的先例，不是双方DMA payload相加的精确同式指标；不能将其DDR带宽列当本文COPY吞吐 |
| [MASK，ASPLOS2018](https://doi.org/10.1145/3173162.3173169)，[作者稿](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf) | 多应用GPU仿真（§6，PDF8–9） | 共享翻译与内存层次调度 | §6“Evaluation Metrics”（PDF9）明确`Σ IPC_shared/IPC_alone`及最大slowdown；分别衡量系统性能和个体不公平 | 合计归一化进度与个体慢化须分别评价，已有标准做法 | IPC weighted speedup不是本文共同Solo分母下的payload总量比；只作指标思想前史，不能声称两式一般等价 |

### 4.1 本项目的解释边界

据原补充分析§1，DS0 A1/A8/A64合计/Solo中位数分别为**105.579%/95.665%/90.766%**；对应A1中V约占Solo的52.790%。它支持“零miss共跑下主要是份额减少，miss压力下又出现低于单流参考的payload总量”这一描述。因为同窗A响应记账量与V都为16MiB，合计列近似为V列的两倍；不是第二份独立因果证据。

可将指标写为`G_sum=(B_V/W+B_A/W)/g_Solo`，同时给出V与A各自的速率；`B_A`必须注明响应记账近似及跨窗界限。**总量约等于Solo并不推出公平**，还须看两路份额；**总量低于Solo不独立证明全部损失由翻译产生**，还须结合A1控制、miss/路径观测和配置一致性。COPY只记一次有效payload，不能把读与写再次相加。上述均是对本项目记账和推断的限制，原报告“路径、HPM与因果限制”亦未把总损失全归到IOMMU某阶段。

**建议中文（一句）：** 我们在同一受害者硬件窗口内并报双方各自及合计的有效载荷吞吐近似值，并与配对Solo比较，以区分近似共享容量与完成总量的进一步下降。

**Suggested English (one sentence):** We report per-stream and aggregate payload-throughput proxies over the same victim window, normalized to a paired Solo run, to distinguish approximately shared capacity from a reduction in total completed payload.

## 5. 未决全文与学校下载清单

**指定追取的LATPC、NB-Walker、TRANS-FETCH、王镇道等2024，本轮均未新增取得论文正文；CATO新增的是作者概述。** 日期均为2026-10-18 JST。未得全文不等于无论文或确定付费，也不按摘要/题名排除机制覆盖。表中DOI仅填已核身份；软件DOI不充论文DOI。

| 完整题名／作者与身份 | 可供学校检索的入口 | 实际缺口与本轮结果 | 影响 |
|---|---|---|---|
| **Don't Get Stuck in Traffic: A Case for Contention-Aware Translation Offloading（CATO）**；Osang Kwon等5人；MICRO2026；**论文DOI未核到** | [会场Session8B](https://www.microarch.org/micro59/program/)、[作者概述](https://skku-compasslab.github.io/compasslab/publications/papers/260710-micro-cato/) | **仅作者概述，非论文全文**；正确路径恢复后仍无PDF/正式单篇出版链接。需原始论文正文、方法/结果/局限；不能根据题录或网页日期假定学校现在一定可下载 | D1最高优先；也可能影响G2b C1的数据/翻译竞争解释，保留风险而不改旧裁决 |
| **LATPC: Accelerating GPU Address Translation Using Locality-Aware TLB Prefetching and MSHR Compression**；Yeonan Ha、Jiho Park、Hanna Cha、Jiwon Lee、Joonsung Kim、Won Woo Ro、Youngsok Kim；MICRO2025，418–431；**10.1145/3725843.3756069** | [ACM正式页](https://dl.acm.org/doi/10.1145/3725843.3756069)、[PDF](https://dl.acm.org/doi/pdf/10.1145/3725843.3756069) | **仅摘要**；正式发表及题录已核，普通PDF返回403/HTML。第一作者、导师及ESCal列表均只链DOI；需要学校ACM取得完整正文，元数据页码不等于已核PDF页数 | 翻译预取/批量PTW的资源竞争，影响G2b C1及D1完整覆盖判断；不能据摘要排除 |
| **NB-Walker: Non-blocking Page Table Walker to Enhance Address Translation in NUMA GPUs**；Zihang Chen、Lieven Eeckhout、Hongyuan Liu、Tianao Ge、Xinkai Wang、Jiayi Huang；MICRO2026；**论文DOI未核到** | [会场Session8B](https://www.microarch.org/micro59/program/)、[第一作者论文页](https://maxkev1n.github.io/publications/micro-2026/) | **仅题录及工件README**；作者页无PDF，公开v2归档176项无论文PDF。软件记录[10.5281/zenodo.21488827](https://zenodo.org/records/21488827)不是论文DOI；学校按完整题名查proceedings/early access | D1的NUMA/PTW排队与G2b C1的non-blocking关系；尚不能核机制或实验覆盖 |
| **TRANS-FETCH: Driving Translation Prefetching with Deep Lookahead from L2 Data Prefetch Events**；Cansong Zhou、Xiaohang Wang、Peng Liu；MICRO2026；**论文DOI未核到** | [会场Session6A](https://www.microarch.org/micro59/program/)；未核到publisher单篇页 | **仅题录**；同名匿名[软件候选](https://zenodo.org/records/22091080)未能与三作者严格绑定；其归档只有结果图和依赖库PDF，无论文稿，未用作机制证据。学校按题名与作者查proceedings/early access | G2b C1翻译提前准备/预取及D1的压力敏感性；无摘要/正文，不作覆盖裁决 |
| **基于RISC-V的IOMMU设计**；王镇道、班贵龙、胡锦、焦旭峰；《湖南大学学报（自然科学版）》2024，51(6)：187–194；**10.16339/j.cnki.hdxbzkb.2024238** | [出版社文章](https://cjournal.hep.com.cn/1674-2974/CN/10.16339/j.cnki.hdxbzkb.2024238)、[原站PDF](https://cjournal.hep.com.cn/1674-2974/CN/PDF/10.16339/j.cnki.hdxbzkb.2024238) | **仅摘要**；本轮直接核原站四作者/刊期/页码与PDF链接，正常PDF请求超时，未收到文件。前轮登录/验证码与本轮超时分开；需要该期刊完整8页 | G2同题系统近邻及D1/D2具体实验覆盖仍未核；不可拿同名学位论文或CPU MMU文章补正文 |
| **FBI: Framework for Efficient Batching of IOMMU TLB Invalidations**；Jiho Han、Won Woo Ro；ICEIC2025 PS5-03；**论文DOI未知** | [会议program公开副本](https://lab.hanbat.ac.kr/powervision/conferences/document/3118/download)，PDF27/印刷94 | **仅题录**；program确认作者与场次，精确标题及IEEE限域/Crossref查询未核到对应DOI或正文。尚不能确认独立出版论文页；需学校按会名/题名查证，不能编造单篇链接 | D2批处理的未排除近邻；题名不能证明有本文PSCID/邻居完成时间矩阵 |

优先通过学校取得**LATPC和中文2024**：两篇正式出版身份明确，但本环境未得正文。CATO、NB-Walker和TRANS-FETCH已有接受题录，尚未核到正式单篇入口，学校权限也可能不足以解决尚未发布的问题；可用完整题名/作者查找作者可分享稿或后续early access。本任务没有联系作者或登录学校账号。

其他D1摘要/概述级缺口见该项表：Clover、PIMFY及NDSP不因相邻标题被默认为已排除；其中学位论文公开viewer入口不等于本轮已取得其论文文件。DATE2024则已取得正文并核§III–VII，不再在本报告列为缺全文。

## 6. 检索与阅读记录

### 6.1 数据库、实际渠道与失败

以下记录只对应本轮增量，不把G2/G2b的历史查询重计为新检索。先按`references/catalog.json`、相关`references/summaries/`及`iommu-platform/out/shared-translation-literature-20261006/`定位已有原件；旧“worker读完”记录不是本轮主会话全文重读声明。日期统一为**2026-10-18 JST**；外部网页索引的domain/`site:`限定不等于站内完整检索。

| 来源 | 实际使用 | 成败与可支持范围 |
|---|---|---|
| arXiv | 外部限域；原生网页查`Contention-Aware Translation Offloading`；原生标题查询`ti:LATPC OR ti:"NB-Walker" OR ti:"TRANS-FETCH"` | 两项原生查询正常200、各0结果；只表示本查询未命中，不排除其他题名或未索引版本 |
| IEEE Xplore | 外部domain/`site:`限定；CATO原生`queryText=Contention-Aware Translation Offloading`；FBI标题/作者补检 | CATO正常HTTP418/HTML，无可用结果集；未核到FBI单篇DOI。不能写成IEEE全库已排除 |
| ACM DL | 外部限域；CATO原生`AllField=Contention-Aware Translation Offloading`；LATPC正式PDF；D1原件的作者版补检 | CATO与LATPC普通请求403/HTML，非PDF；作者公开PDF/机构页分别按实际状态使用，未绕账号或验证码 |
| dblp | CATO原生`q=Contention-Aware Translation Offloading`，作者/题录入口补检 | 正常HTTP202/HTML，未得到可用该题搜索集；外部索引只作身份线索 |
| Google Scholar | CATO原生完整副题查询；外部索引辅助 | 原生429/sorry；未绕验证码，不能将失败当无论文 |
| 作者、机构与会场 | COMPASSLAB公开仓和网页、Yonsei/SKKU、Yeonan/Youngsok/ESCal、NB作者、MICRO程序、HEP、DATE会场、Cornell/USENIX、IISc等 | 原件/作者概述/摘要/题录分级；CATO路径已恢复、FBI只有program、中文PDF超时，具体缺项见§5 |
| 规范与原始工程记录 | Intel/RISC-V官方规范、Amit原始邮件；复用同hash原件与预检 | 用于scope、完成与取舍的事实，不把规范支持当已实现或已证明性能隔离 |
| 公开元数据与软件记录 | Crossref精确DOI/标题核名、Zenodo公开记录/归档目录；D1另用OpenAlex/Semantic Scholar逐DOI核名与找公开稿 | 元数据不是正文。NB标题top3无精确匹配；TRANS的Crossref查询429，不是无记录。归档只查论文文件/README，没有执行工件；身份API不当科学结果来源 |

实际失败补充：LATPC普通PDF响应为5525B HTML，未运行“论文PDF”预检；第一作者/导师/实验室条目仅给DOI。SKKU ScholarX的一个普通HTTPS请求有证书校验失败，未关闭TLS验证。中文PDF本轮为连接/超时、web打开失败，未声称本轮拿到登录页；旧轮登录/验证码继续只作为历史状态。Amit早期openwall入口超时后，关键取舍采用实际成功读取的6月原始邮件。未使用学校会话、付费全文或联系作者。

### 6.2 已读但未作为直接覆盖证明的条目

- **LATPC、NB-Walker、TRANS-FETCH、中文2024、FBI**：分别为摘要/题录/文档范围，继续留在未决清单，不按可访问性判为无关。
- **NB-Walker软件v2**：只列归档、读README，未运行；软件版本和论文标题复数变体不另计独立论文，不用代码补论文评估。
- **同名Trans-Fetch匿名软件**：未核严格作者绑定，归档没有论文；结果图、通用ChampSim说明和zlib文档不当论文原文。
- **中文相邻学位论文及CPU MMU设计文章**：仅搜索题录/片段，未确认为王镇道等的同一篇，不替代目标全文。
- **CPU TLB shootdown、SITE等**：本轮未新增全文核读；不以CPU失效论文直接证明多DMA IOMMU性能结论。
- **CATO同名政治/交通/语言教学结果**：搜索噪声，未用于学术覆盖判断；未把搜索无命中或同名页面当作不存在证据。
- **DATE2024安全攻击与D2 ORACLE**：目标节已读，纳入相邻前作；安全撤销、旧项不可用与腾槽性能是不同问题。
- **旧G2/G2b百分比或停止阈值**：只作来源导航；未拿未过3%门槛替代“真实差值为0”，不启动旧门或新实验。

### 6.3 实际阅读范围、版本与结构状态

本轮新增阅读是目标章节核验，不将“取得完整PDF”升级成“全文逐字读完”。关键图表作了视觉核对；结构预检仅检查PDF结构，不证明机制或数值正确。F&S的15页均可枚举/读取，但active xref未覆盖object421，状态为**UNAVAILABLE**；CLAP的18页均可枚举/读取，但active xref未覆盖object2094，亦为**UNAVAILABLE**，下面保留原状态并用章节锚，不重新序列化掩盖告警。

| 原件／版本 | 实际阅读范围 | 结构及SHA256 |
|---|---|---|
| mgvm2022作者稿 | PDF2/3/6/10/12相关段、4–5局部；主会话复核PDF2/12及图，非全文 | PASS，19页；`83761d948fcb10a787dc762d611bac89e9c7bb881fad63975eb2d9ea50695dc8` |
| barre2024作者稿 | PDF1–4及11相关段、9局部；主会话复核PDF11/§VII-H与图 | PASS，14页；`027228e356ac8a2a62d58337f148d46d6fba4f2c9ace7402572be09083fa3695` |
| clap2025作者稿 | §3.1–3.5/Table1/Fig6–8及§5/5.1局部；主会话视觉核Fig6–8 | UNAVAILABLE，18页；`6661f5c8b66a99e855585fef60b78000d88348ac1b43924887257a3f0eda0975` |
| mitosis2020作者稿 | PDF3/5/6相关段；主会话复核PDF5–6及Table2，非全文 | PASS，18页；`9e9928282ee64ca25ce21670176d4f4bc4bf73b1630661de7f5de26e6d30e121` |
| rIOMMU2015 | PDF5-6,8-9, p1 metadata; Table1 visual | PASS，14页；`70a58dd66ee033036c821b1dfa6e19eba62e45c806cf8fb30ee019322e450be3` |
| Scalably2015 | PDF3-4, PDF5 visible beginning/end only, PDF9,12; Figure2 visual; PDF5 middle truncation disclosed | PASS，14页；`2fe319d9f089d6db3d02bf49172b86f0656798ada5c36ff7bb597c1c56d10df1` |
| F&S2024 | Sections2.2,3,4.1,4.3,4.4; physicalPDF6-10,12-14 actual read; cite sections/figures due UNAVAILABLE; Fig6 visual | UNAVAILABLE，15页；`42d11d59ba23b65f719578311c6cf9be7bbec9cf201e377fef124cd5449eab02` |
| Amit等2010 | PDF2,4-5,10-11; p1 metadata; Figure12 visual | PASS，12页；`5889c11577f41f13026198dbd264f429f98a7f3cf1d9180ceb0320bbec6af3b6` |
| IOTLB Wall2023 | G2b scope PDF3,8-12,22 reused; D2 targeted method PDF8-9 reviewed; no repeated whole-paper review | PASS，34页；`4abb682f1facf103c035f27dbe08f7857b7c38cd34c23d3c541e62e4ec734b76` |
| DATE2024会场稿 | PDF2-6 sectionsIII-VII; p1 metadata/abstract primary web extract; Figure5 visual | PASS，6页；`8f40b309632ab155e36cb7cc62bb61cb5edb45afdc66e8df0e586795bc8ed4e5` |
| VT-d Rev5.0 | PDF116-117,119-120,136-137; scoped invalidation and drain only; PDF137 visual | PASS，354页；`a136448f2a14d49aa524106f58a139a5add90f78eef8a6abaabcc917ebc753b6` |
| RISC-V IOMMU/20260222 | PDF52-55 sections4.1.1-4.1.2; Table11/PDF53 visual | PASS，108页；`be2134b407a1a3ce5a9da1779cbdc4b4c641ee50ab32676e0a858e5a04ff2b8e` |
| FBI所在会议program（非论文） | PDF27/printed94 only, poster PS5-03; visual; paper full text and DOI unknown | PASS，34页；`73c56a4d4bf77cbb8e3feecfd4abe273db5b4f994c4b207e86693e0ed588dff1` |
| hostcc2023作者/会场稿 | 复用G2b；本轮复核PDF8/§4.2/Fig9及PDF9/§5.1，Fig9视觉核；§2.2配置沿旧读域 | PASS，13页；`2d20ba935a906d5dd23e2bd87f3bf519b3ba8f70c6a3592def3cd4fb09bcf767` |
| hotnets2022作者/会场稿 | 复用G2b；本轮复核PDF4–6/§3.1–3.2，非全文重读 | PASS，7页；`0863e360b0e9089c0c45a041d5cf8824e6d12678763b6daf13479bfc6c87cc89` |
| MASK作者稿 | 复用G2b同hash PASS；本轮只复核§6 Evaluation Metrics/PDF9，非重复全文核读 | PASS，16页；`f72e642e761582784d7508dabc743b5e5176f1bb2cb11282260bef18545c41fb` |

主会话另直接读CATO固定提交的概述、F&S §2.2/§3、Amit 2021 gather原文及VT-d PDF137，并核本项目DS/合计吞吐/失效/后继更正报告；其他目标段由有界Codex席直接读原件，主会话据关键原文作接受判断。CATO、Clover等网页摘要及§5条目没有论文PDF预检或伪造页锚；摘要标题/TOC不等于正文已读。

### 6.4 实际查询字符串

下列查询全部于2026-10-18 JST执行；相同渠道和限制下的重复查询只保留一次。R为主会话CATO/D3，L为D1来源追查，I为D2，F为指定全文追取。除另行注明外均是外部网页索引；原生入口及失败含义见§6.1。

**R组**

- R01：`"Don't Get Stuck in Traffic: A Case for Contention-Aware Translation Offloading"`
- R02：`"CATO" "translation" "Kwon"`
- R03：`"Contention-Aware Translation Offloading" pdf`
- R04：`"Seokin Hong" publications MICRO 2026 CATO`
- R05：`"Contention-Aware" "Translation" "Kwon"`
- R06：`"CATO" "translation offloading"`
- R07：`"Osang Kwon" MICRO 2026 translation pdf`
- R08：`"Don’t Get Stuck in Traffic" MICRO`
- R09：`"Osang Kwon" GPU`
- R10：`"Contention-Aware Translation" site:arxiv.org`
- R11：`"Translation Offloading" site:dl.acm.org`
- R12：`"CATO" "page table"`
- R13：`"memory bandwidth" "aggregate throughput" fairness partitioning solo`
- R14：`"IOMMU" "aggregate throughput" interference`
- R15：`"Host Congestion Control" "memory bandwidth" throughput`
- R16：`"MASK" "weighted speedup" "fairness" 2018 GPU`
- R17：`"translation offloading" MICRO 2026 pdf -site:scribd.com -site:researchgate.net`
- R18：`"CATO" "CXL" "translation"`
- R19：`"Contention-Aware Translation Offloading"`；domain：`dl.acm.org`, `ieeexplore.ieee.org`, `arxiv.org`, `dblp.org`
- R20：`"Osang Kwon" "CATO" pdf`

**L组**

- L01：`"Clover" "Storage-Efficient Page Table Replication"`
- L02：`"PIMFY" "Remote Page Walks"`
- L03：`"remote page table" "GPU" "CXL" 2025 2026`
- L04：`"Barre Chord" "CLAP" "latency"`
- L05：`"Clover" "Page Table" Sungbin pdf`
- L06：`"PIMFY" pdf -site:eurekamag.com -site:lacuna.tiptreesystems.com`
- L07：`"page table" "CXL" latency contention -CATO -cato -site:researchgate.net`
- L08：`"Clover" "Storage-Efficient" pdf -site:scholarx.skku.edu`
- L09：`"Clover" site:skku-compasslab.github.io`
- L10：`"PIMFY" "Yonsei" pdf`
- L11：`"PIMFY" site:yonsei.elsevierpure.com`
- L12：`"Clover: Storage-Efficient" filetype:pdf`
- L13：`"PIMFY: Eliminating Remote Page Walks" filetype:pdf`
- L14：`"Mitigating Address Translation Overhead in Near" "Kim"`
- L15：`"remote page table" "accelerator" latency contention -CATO`

**I组**

- I01：`id/title filters: riommu; scalable; iommu; fbi; deferred; invalid; flush`；渠道：`local_catalog_summaries`
- I02：`IOMMU "deferred" "DATE" "2024"`
- I03：`IOMMU "FBI" invalidation`
- I04：`"IOMMU" "PASID" "invalidation" performance range`
- I05：`"IOMMU" "flush queue" paper Linux`
- I06：`"FBI: Framework for Efficient Batching of IOMMU TLB Invalidations"`
- I07：`"IOMMU Deferred Invalidation Vulnerability" pdf`
- I08：`"IOMMU" "invalidation" "range" "performance" paper`
- I09：`"Amit" "IOMMU" "invalidation" selective global`
- I10：`"FBI" "IOMMU" "Han" "Ro"`
- I11：`"Framework for Efficient Batching" IOMMU DOI`
- I12：`"IOMMU TLB Invalidations" ICEIC 2025 doi`
- I13：`"FBI" "IOMMU" pdf site:yonsei.ac.kr`
- I14：`https://api.crossref.org/works?query.title=FBI+Framework+for+Efficient+Batching+of+IOMMU+TLB+Invalidations&rows=5`；渠道：`crossref_rest`
- I15：`"FBI" "IOMMU" site:ieeexplore.ieee.org`
- I16：`"Framework for Efficient Batching" "10.1109"`
- I17：`"Jiho Han" "Won Woo Ro" "2025"`
- I18：`"IOMMU" "invalidation" "Amit" "2021" optimization`
- I19：`https://lkml.rescloud.iu.edu/hypermail/linux/kernel/2106.0/08115.html`；渠道：`primary_link_follow`
- I20：`https://lkml.rescloud.iu.edu/hypermail/linux/kernel/2106.0/08114.html`；渠道：`primary_link_follow`
- I21：`https://lkml.rescloud.iu.edu/hypermail/linux/kernel/2106.0/08109.html`；渠道：`primary_link_follow`
- I22：`https://docs.riscv.org/reference/iommu/iommu_extensions.html`；渠道：`primary_official_spec`
- I23：`https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf`；渠道：`primary_official_spec`

**F组**

- F01：`"LATPC" "3725843.3756069" pdf`
- F02：`"NB-Walker" "Page Table" pdf`
- F03：`"TRANS-FETCH" "Translation"`
- F04：`"10.16339/j.cnki.hdxbzkb.2024238"`
- F05：`"LATPC" "Yeonan" pdf`
- F06：`"NB-Walker" "Eeckhout"`
- F07：`"TRANS-FETCH" "Cansong"`
- F08：`"基于RISC-V的IOMMU设计" 王镇道 pdf`
- F09：`"TRANS-FETCH" MICRO2026`
- F10：`"Cansong Zhou" "Xiaohang Wang" "Peng Liu"`
- F11：`"LATPC" "Youngsok Kim" pdf`
- F12：`"TRANS-FETCH" "Deep Lookahead"`
- F13：`"基于RISC-V的IOMMU设计" "焦旭峰"`
- F14：`"Lieven Eeckhout" publications NB-Walker`
- F15：`"Driving Translation Prefetching"`
- F16：`"Cansong Zhou" homepage`
- F17：`"NB-Walker"`；domain：`biblio.ugent.be`, `arxiv.org`, `dl.acm.org`
- F18：`"LATPC"`；domain：`youngsok.github.io`, `arxiv.org`
- F19：`"基于RISC-V的IOMMU设计" pdf`；domain：`edu.cn`
- F20：`"LATPC" pdf`；domain：`yonsei.ac.kr`, `skku.edu`, `github.io`
- F21：`"TRANS-FETCH" "Translation" pdf`；domain：`zju.edu.cn`, `arxiv.org`, `dl.acm.org`

原生查询补充：Crossref对MGvm用`query.title=Designing Virtual Memory System of MCM GPUs&rows=1`，对LATPC用精确DOI，对NB完整题名取top3，对TRANS完整题名请求429；FBI完整题名取top5仅无关匹配。D1 OpenAlex逐DOI查Clover/PIMFY，Semantic Scholar以`paper/DOI:{doi}?fields=title,authors,year,venue,openAccessPdf,externalIds`查MGvm/Barre/CLAP/Clover/PIMFY均200，NDSP404；这些只作身份/公开稿入口核对，不做引文网络穷尽检索或科学结果来源。Zenodo对TRANS用`q=title:"TRANS-FETCH"&size=5`，另直接读NB v2记录；arXiv及四数据库原生失败的查询见§6.1。

## 7. 增量结论与AI辅助说明

D1保留统一FPGA、独立DMA和同档A1参照下的延迟曲线；一般远端PTE/排队放大认识已有MGvm、Mitosis等，不把延迟器当真实CXL验证。D2保留具体命令序列/范围的双方完成时间和有边界的ORACLE负结果；范围、批处理、失效影响其他DMA翻译已有F&S、Amit等前史。D3放在指标定义与结果解释，不单列方法创新。

最大风险是把已知机制关系升级为新通用发现，以及CATO等正文缺口。本文的差项是已核原文范围内的实现级量化，不是“全领域无人做过”或无条件的新颖性保证；也不重裁G2/G2b。仅摘要/作者概述候选可构成风险，不能被未检出完整稿自动排除。

本轮使用ARS定向文献核验与PDF流程，由Codex主会话及三个有界Codex任务完成；主会话直接复核上述决定性原文、关键图和本项目结果口径后整合本文。未调用DeepSeek或Claude，未向人工作者添加已读标记，未重跑论文工件、RTL、硬件或归约实验。临时取证之外，仓库只新增本文并给G2/G2b追加链接；查询、版本、读域、失败与未决项均在本文内保留，不依赖临时收据才能理解裁决。
