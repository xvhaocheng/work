**有部分重叠需要讨论（截至2026-10-17，JST）**：已核实Procedia2025的RISC-V IOMMU多IOATC共享IOATS并行性能评价（§2、§5.3），以及Hur/Ro2025的RISC-V处理器配置＋SMMU模型多客户端并发与缓存策略评价（§III–IV）；两篇均是直接性能前作。已读原文没有完整覆盖本文的固定共跑基线、零miss邻居等待传播及缓解组合证据，差异见§5.3、§7；不能主张首次研究RISC-V系统中的多客户端共享翻译性能／干扰。

# G2：ISPASS 2027 前作新颖性判别

**G2裁决：有条件通过，限于§7的具体刻画增量。** 用户随后提供两篇出版社PDF，已完成全文及关键图表核读，原先“缺全文”两项已解除并改列实质重叠前作。通过依据是已核方法／结果未提供本文的零miss共跑控制链、具名PTW响应对邻居首次命中的局部干预，以及P/PN条件性负结果；平台更换本身不是通过依据。条件是正文明确引用这两篇、保留机制前史与各证据适用范围，不使用领域级首次；中文2024设计等仅摘要候选及检索覆盖限制仍保留，故不是无条件的排他性新颖性保证。此裁决不触发新机制、实验或停止审查。

最接近的证据链包括：[Procedia2025，§2–5](https://doi.org/10.1016/j.procs.2025.08.115)的RISC-V共享IOATS多端扩展；[Hur/Ro2025，§III–IV](https://doi.org/10.1109/ICEIC64972.2025.10879735)的多client并发、IPC与缓存策略；[Rodríguez2024硕论，§6.1、§6.3、§7.1](https://hdl.handle.net/1822/99350)的同平台四iDMA性能；[Bermuda，§III–V](https://arxiv.org/pdf/2508.19670v1)的Arm多DMA干扰；[DATE2025 SVA，§III–IV](https://arxiv.org/pdf/2502.17398v1)的同开源IOMMU族FPGA／host噪声；[Kurth2018，§II(b)、§IV-C](https://arxiv.org/pdf/1808.09751v1)的miss阻塞hit前史。本文须把这些已有内容与新的固定实现实测、局部等待证据和缓解取舍逐项比较。

## 1. 判别口径与全文覆盖

- 同题按实质判断：RISC-V IOMMU、多个独立DMA流共享翻译资源、实际评价并发竞争／干扰。仿真也计入；不要求相同FPGA、OS、缓解方案或V/A命名。
- 多设备接线、顺序跨设备请求、单DMA加host噪声、核内MMU竞争分别记账；硬件支持并发不证明实验实际并发。
- 多client并发共享IOMMU及其缓存策略已经是直接问题重叠；不要求前作使用victim/aggressor名称。Hur/Ro的RISC-V处理器配置与gem5 SMMU模型分别记录，既不能称其纯Arm而忽略，也不能把它写成标准RISC-V IOMMU RTL／多DMA FPGA实验（§III–IV、Table I/II）。
- 标准RISC-V IOMMU与早期ARM host＋RISC-V accelerator／软件管理RAB区分；后者仍限制问题、方法与机制新颖性，不能因年代早而删除。若按“RISC-V加速器系统中的共享I/O翻译”宽口径判断，HERO／Kurth已构成实质重叠，不能靠“标准”一词或更换host ISA规避前作。
- 主检索窗口2024-01-01至2026-10-17，关键先例向前追溯。每项事实给章节／页码，摘要及题录明确降级。PDF页从1开始；硕论另列印刷页。未报告参数不猜补。
- 下面保留既有核查事实并合并本轮增量。阅读标记是Codex实际阅读，不是用户本人阅读，也不代表所有参考文献原作已被逐篇核查。

<a id="scope"></a>
### 两篇必读：版本与全文覆盖

| 文献 | 本轮直接阅读范围 | 原件与定位规则 |
|---|---|---|
| Costa、Martins、Pinto，Bermuda（RTCSA 2025） | arXiv **2508.19670v1**：摘要、§I–VII及致谢正文已读；图注已读，Fig.1–8实际渲染查看；参考列表浏览及定位相关条目核对，不逐条审查外部原作 | [作者 PDF](https://arxiv.org/pdf/2508.19670v1)、[本地同哈希原件](../../../references/papers/bermuda2025.pdf)；PDF共12页；以下按从1开始的PDF页序计页，不将其混成出版页码 |
| Koenig、Zelioli、Benini，ETH/DATE2025 SVA | arXiv **2502.17398v1**：摘要、§I–VII及唯一编号脚注正文已读；Table I/II、Fig.1–5、Listing 1实际渲染查看；参考列表浏览及定位相关条目核对，不逐条审查外部原作 | [作者 PDF](https://arxiv.org/pdf/2502.17398v1)、[本地同哈希原件](../../../references/papers/riscvsva2025.pdf)；PDF共7页；[ETH正式题录](https://www.research-collection.ethz.ch/entities/publication/92097a99-b995-45fa-b2a9-518e17fc35ed)核定DATE2025及DOI |

两篇原件哈希与既有ARS结构预检一致：Bermuda为12/12/12页、SVA为7/7/7页，均PASS，本轮复用。Bermuda正文与参考列表逐页读完，主会话重核Fig.7/8；SVA子任务实际读完并渲染全部7页，主会话直接核方法、结果和关键图表。下表保留全文覆盖细目；不代表用户本人已读，也不代表所有参考文献外部原作都已逐篇核验。结构PASS仅证明PDF结构，不证明科学结论。

<a id="fulltext-coverage"></a>
### 实际全文覆盖明细

| 文献 | 正文逐节定位及本轮覆盖 | 图表、脚注与参考文献边界 |
|---|---|---|
| Bermuda v1 | 摘要第1页；§I第1–2页；§II第2–3页；§III第3–5页；§IV第5–9页（A第7页、B第7–8页、C第8–9页）；§V第9–10页（含V-D）；§VI、VII及致谢第10页：以上正文均已直接阅读 | Fig.1第2页、Fig.2第4页、Fig.3–5第5页、Fig.6第6页、Fig.7第8页、Fig.8第9页及其图注均已查看。所读PDF未见另列编号脚注。第11–12页参考列表已浏览；仅核对与平台、性能/安全近邻相关的[36]–[40]、[43]–[50]与正文关联，未逐篇读取这些外部文献 |
| ETH v1 | 摘要及§I第1–2页；§II第2页；§III第2–3页；§IV第3–5页（A第3–4页、B第3–5页、C第5页）；§V第5–6页；§VI、VII第6页：以上正文均已直接阅读 | 第2页脚注1指向`carfield/tree/date_iommu_evaluation`，已读，未复现该工件。Fig.1第2页；Table I、Fig.2第3页；Table II、Fig.3/4、Listing 1第4页；Fig.5第5页及其图注均已查看。第6–7页参考列表已浏览；仅核对[7]–[10]、[18]–[26]与定位相关的引用关联，未逐篇审查这些外部原作 |

该覆盖记录是本轮Codex实际阅读记录；“全文正文已读”“关键图表已核”“参考文献原作已独立核验”是不同事项，最后一项没有被本记录泛化宣称。


**自含版本记录。** Bermuda arXiv2508.19670v1，SHA256 d85ffeaa949a3ad5f17e950871a2f85c49b61bf237e70fca813a2535bb8a51f8；SVA arXiv2502.17398v1，SHA256 4610cbf0fc9eec49e41618c3dad28c4ba609c4f596efc26d19ec6548cd98c319。正式出版正文未与作者稿逐字等同。新增PDF初检因pypdf缺失为UNAVAILABLE，临时独立依赖环境复查后，Rodríguez硕论115页（SHA256 b364a5d4d1d3a222bfbed5d56c2e0ff7d91ee92a3039788d56e5b3f77b1a41c8）、IEICE6页、ShareMMU18页、MASK16页、Kurth2018九页、Hao2017十二页均PASS；原UNAVAILABLE记录未覆盖。HERO2017、DATE海报／slides等新预检也PASS；ANDARE2018 HERO有xref覆盖告警，仍UNAVAILABLE，后文仅用章节锚。新阅读／预检记录与旧引用卡分开，不新增用户“全文已读”标记。


<a id="paper-matrix"></a>
## 2. 前作对比表

“局部”表示直接核读所列方法／评估段，不冒称全文；同一论文的预印本、出版题录、海报和工件合并。未决项没有算作已排除。

| 论文（含链接） | 平台 | IOMMU类型 | 负载与传输大小 | 观测手段 | 主要结论 | 是否评估缓解 | 与本文的具体差异 |
|---|---|---|---|---|---|---|---|
| **Rodríguez，An Open-source RISC-V Input/Output Memory Management Unit，硕论2024**（局部；[机构全文](https://repositorium.uminho.pt/bitstreams/696f58ed-10e6-4674-b750-1de29e401413/download)） | CVA6／Genesys2、四iDMA、裸机／Linux／Bao（§6.1–6.2） | 标准v1.0，单入口、共享IOATC、RR（§6.1、§7.1） | 两阶段、32 mappings、1000 transfers；正文payload未报告，相关bench为8 B但运行身份未绑定（§6.3.2；下文§5.1） | CPU cycles＋HPM；平均latency／miss（Fig.6.31/32，PDF105–106） | 已测缓存容量影响，正文报告小缓存约325%额外延迟；已认识设备争用（§7.1） | 实测DDTC/IOTLB扩容／MSI配置；分布式IOATC是future work | **最接近的平台前作**；公开关联bench逐笔等完成，正文未建立持续并发供给；实际运行版本unknown，不能绝对排除其他运行 |
| **Costa、Martins、Pinto，Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems，RTCSA2025**（全文；[v1](https://arxiv.org/pdf/2508.19670v1)，[DOI](https://doi.org/10.1109/RTCSA66114.2025.00027)） | ZCU104，FPGA victim＋FPD-DMA 8 channels／Bao | Arm MMU-500/SMMUv2，TBU5＋TCU（§III–IV） | 延迟16 B，吞吐16 B–4 KiB；V循环63页、A N=56/1992（PDF7–9） | PMU＋ILA，SMMU开／关 | 16 B吞吐降约32%，4 KiB近汇合；最大延迟至1.79×（Fig.7/8） | §V-C讨论候选，明确未完整实验评价 | 问题高度重叠，非RISC-V；没有本文逐请求PTW响应干预。不能说它没测4 KiB |
| **Koenig等，Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs，DATE2025**（全文；[v1](https://arxiv.org/pdf/2502.17398v1)，[DOI](https://doi.org/10.23919/DATE64628.2025.10992962)） | VCU128，CVA6＋单Snitch cluster／DMA，Linux | Zero-Day Labs实现族，4项IOTLB、1项device-directory cache（§III-A） | RajaPERF、tiling／double buffering；每tile DMA字节数未报告（§III-B） | 软件／kernel分层计时、平均PTW、DRAM延迟与host噪声（§IV） | gemm开销4.2%–17.6%，LLC后0.4%–0.7%；host使平均PTW慢约20%（Fig.4/5） | 实测LLC缓存host/PTW，DMA数据绕LLC | 单cluster DMA＋host噪声，未报告多个独立DMA翻译流并发评价；数据绕LLC不等于本文hit旁路 |
| **DMAAUTH，USENIX Security2024**（局部；[正式全文](https://www.usenix.org/system/files/usenixsecurity24-wang-xingkai.pdf)） | RocketChip FPGA、Linux6.1、NVMe／E1000E | 移植CVA6 IOMMU，256项IOTLB避免其成瓶颈（§6.1，印刷1089–1090） | fio四线程／1 GiB／4 KiB–4 MiB，iperf3扫描（§7.2） | bare／IOMMU／DMAAUTH保护和性能比较 | RISC-V FPGA IOMMU性能基线已有（§6.1、§7.2、Fig.12/13） | 评价认证保护方案 | 所核§7.2分别测设备，未报告跨设备solo/concurrent干扰矩阵；四线程不等于四DMA设备 |
| **Liu等，End-to-End Implementation of IOMMU-Based DMA Isolation for Trusted Execution Environments on RISC-V，IEICE2026**（局部；[题录](https://ken.ieice.org/ken/paper/20260315lct5/eng/)，[机构全文](https://www.kasahara.cs.waseda.ac.jp/achieve/pdf/y2025/LIU%E3%81%95%E3%82%93ETNET.pdf)） | VCU118，Rocket＋单NVDLA Small，Linux6.15（§3、§5.1） | 开源RISC-V IOMMU，4项IOTLB、4 KiB页（表1–3） | LeNet／ResNet；每笔DMA字节数未报告（表4） | Linux／fault日志、IOMMU开关／推理时间 | 隔离、故障约束及负载相关开销（§4–5） | 评价IOMMU保护开销 | §5.5/PDF5明确单NVDLA，多设备争用／global invalidation pressure留待未来；**作者／实验室关联前作，正常引用**，不推断与本文数据相同 |
| **Alam等，ShareMMU，MICRO2026公开camera-ready**（局部；[作者入口](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted)） | x86 gem5/PARADE，4/8/16/32 AFUs（§VI/PDF11） | 可信共享TLB＋Permission Tracker；私有TLB不可信 | PARADE应用、4 KiB页；共享TLB1024项8-way（Table II/III） | 周期仿真，设备数／回失效／多进程hit-rate扫描（§VII-B/PDF12） | 已评价多加速器共享翻译、容量竞争与隔离（§V-C、§VII） | AA-LRU、Permission Tracker、安全变体 | 非RISC-V／非FPGA，但不能写成“仅安全、无性能”；共享翻译隔离概念已有 |
| **Garofalo等，A Reliable, Time-Predictable Heterogeneous SoC for AI-Enhanced Mixed-Criticality Edge Applications，2025**（正文全读；[作者稿](https://arxiv.org/pdf/2502.18953v1)） | Carfield异构RISC-V SoC | 所核正文未评价IOMMU翻译路径 | host、system DMA、AMR/vector DMA施压HyperRAM／LLC／SPM（§III-2、Fig.6）；大小依实验 | 总体干扰与隔离配置 | 多加速器共享资源干扰／缓解已有（§II-1、§III-2） | TSU、LLC／SPM分配 | 不能称首次RISC-V DMA共享资源干扰；本文需聚焦翻译及其与数据路径区别 |
| **HERO，CARRV2017**（局部、窗口外；[正式稿](https://carrv.github.io/2017/papers/kurth-hero-carrv2017.pdf)） | ARM host＋RISC-V clusters，各cluster DMA | 软件管理RAB（§2.1） | MMM等；实测1/2/4/6/8 clusters（§3.2） | RAB逐核事件图／计数／总体性能（§3.4） | hit-under-miss已有直接展示（Fig.6b） | 翻译／并行支持；非本文消融 | 非标准RISC-V IOMMU；内部可见性、时序图和hit-under-miss均非本文首创 |
| **Kurth等，Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine，ICCD2018**（局部、窗口外；[v1](https://arxiv.org/pdf/1808.09751v1)） | Zynq7045，ARM A9 host＋multi-cluster PMCA | hybrid/shared IOMMU、软件PTW；L1 32/L2 256（§V-A） | 指针追踪／stream；实验8笔在途、burst≤2 KiB（§V-B/D） | 实系统运行时间、walk／buffer成本 | **明确miss满会阻塞hit**；锁定限制多cluster DMA（§II(b)、§IV-C） | 预取、并行walk、DMA retry | 机制前史极近；本文只能主张固定标准实现的实板定量与诊断增量 |
| **Hao等，Supporting Address Translation for Accelerator-Centric Architectures，HPCA2017**（局部；[作者稿](https://www.sfu.ca/~zhenman/files/C10-HPCA2017-ARA-Addr-Translation.pdf)） | x86 PARADE／多加速器DMA（§III） | 32项IOTLB基线及私有／共享TLB | tiling／double buffering、4 KiB页、64 outstanding/accelerator（§III） | 全系统仿真／理想翻译对照 | 私有和共享TLB已支持hit-under-miss（§IV-B/C） | 两级TLB、host MMU复用 | 非RISC-V FPGA；hit旁路不是新机制 |
| **MASK，ASPLOS2018**（局部；[作者稿](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf)） | 多应用GPU，Mosaic/GPGPU-Sim（§6） | 共享TLB／PTW层级 | GPU kernels，非DMA COPY | 翻译／数据争用及模拟消融（§4–7） | 翻译与数据服务相互影响（§4.2/4.3） | fill tokens、cache bypass、PTE优先和应用感知DRAM调度（§5.2–5.4） | 控制点不同；资源控制与协同思路不属本文首创 |
| **Zhao等，Performance optimization and research of data center-level IOMMU based on RISC-V，Procedia2025**（**仅摘要**；[出版社](https://www.sciencedirect.com/science/article/pii/S1877050925024263)） | 摘要：UVM、BC-Linux／Panshi DPU4.0 | distributed IOATC、Walk Cache IOATS（摘要） | 独立设备数、并发供给及干扰对照未核；索引中的IOATC数量表不是已核正文（§5.3） | 摘要称仿真／网络测试；续查仍未取得正文 | 不以摘要／索引数值作正式对照 | 摘要称结构优化，细节未核 | **最高优先级全文风险**；开放许可已核不等于全文已取得，不能按分布式缓存排除同题，DOI10.1016/j.procs.2025.08.115 |
| **Hur、Ro，Enhancing IOMMU Efficiency in Heterogeneous SaCs: A Study on Cache Policy Impacts，ICEIC2025**（**仅摘要**；[机构](https://yonsei.elsevierpure.com/en/publications/enhancing-iommu-efficiency-in-heterogeneous-sacs-a-study-on-cache/)；作者列表写SoCs，§5.3） | ISA／仿真或实机未核 | 机构Abstract明确多devices共享TLB | 独立DMA数、实际并发／工作集／大小未核 | 全文未得；作者公开列表没有该条PDF链接 | 机构Abstract讨论TLB／walk-cache替换与性能 | Abstract称adaptive policy，细节未核 | **高优先级未决**；不能由引用Arm／gem5等推定平台并排除，DOI10.1109/ICEIC64972.2025.10879735 |
| **王镇道等，基于RISC-V的IOMMU设计，2024**（**仅摘要**；[出版社](https://cjournal.hep.com.cn/1674-2974/CN/10.16339/j.cnki.hdxbzkb.2024238)） | RISC-V，具体平台未核 | 摘要为I/O虚拟化IP | 未核 | 全文登录／验证码，未得 | 仅确认设计／效率主题 | 未核 | Procedia所引近邻，未排除并发评估；51(6):187–194 |
| **Kumar等，Rethinking IOMMU for Future IO Devices，YArch2025**（**仅摘要／题录**；[机构](https://infoscience.epfl.ch/entities/publication/6366166d-7142-457b-b340-8df795675d9f)） | ISA／实验未核 | 摘要提出VMA粒度集中式IOVLB | 未核 | 机构PDF429/405 | 只确认覆盖率／安全设计提案 | 未核 | 机制近邻，不能把索引局部正文当全文实验结论 |
| **Han、Ro，FBI: Framework for Efficient Batching of IOMMU TLB Invalidations，ICEIC2025议程**（**仅题录**；[公开议程](https://lab.hanbat.ac.kr/powervision/conferences/document/3118/download)） | 未核 | 未核 | 未核 | Jan21 PS5/03，PDF27/印刷94 | 题名不能补造结果 | 未核 | 合批失效已有具名条目；未见摘要／正文，不能认定已覆盖本文矩阵 |

<a id="bermuda"></a>
## 3. Bermuda：已存在直接的跨 DMA 干扰实测

### 平台、负载与控制

| 维度 | v1原文事实 | 直接定位 |
|---|---|---|
| 硬件 | ZCU104/Zynq UltraScale+；四核Cortex-A53 1.2 GHz；Arm CoreLink MMU-500/SMMUv2 525 MHz；FPD-DMA 600 MHz；FPGA victim在100/150/300 MHz下测延迟 | PDF第3页§III；第6页§IV；Fig.7 |
| 翻译结构 | 微TLB实测64项；受PMU可见性限制，macro-TLB没有完整探测，使用规格中的2000项作假设 | 第3–4页§III；Fig.3 |
| victim形态 | FPGA IP经S_AXI_HP3_FPD/TBU5访问DDR；4 KiB页；循环访问63个连续页的首地址后回绕；延迟实验每次16 B；对应63个读页条目和一个写页条目 | 第6–7页§IV，Fig.6 |
| interferer | Bao上baremetal VM，四CPU各管理两条FPD-DMA channel，共8条，StreamID各异；文中每channel访问N/4页；`interf_tbu`取N=56，`interf_tcu`取N=1992 | 第7页Interference Workload |
| 可比控制 | solo、两个interference场景；SMMU启用/关闭。关闭组保留总线/内存共跑影响；没有把这些影响天然归零 | 第7–8页§IV-A/B、Fig.7 |
| 计时 | ILA获得DMA transaction周期；PMU用于结构表征。Fig.4给出AR/R handshake示意，不能由此补成包含AW/W/B及软件供给的完整COPY任务计时 | 第3页Measurement Tools；第5页Fig.4；第8页Fig.7 |

上述“每channel N/4页”及“8条不同StreamID”的计数关系按原文保留；本轮未从工件重建物理页去重或逐channel地址序列，不另行推导精确占用。`interf_tcu`是作者设计用来施压TCU的负载名称，不等于已经逐次直接观测到macro-TLB/PTW事件。读者推断的边界：PMU不完整可见时，场景名不能替代内部每级归因证据。[Costa等，§III–IV](https://arxiv.org/html/2508.19670v1) <!--ref:bermuda2025--><!--anchor:section:III-IV-->

### 关键结果与“吞吐不敏感”的条件

| 结果 | 可引用范围 | 不应改写成 |
|---|---|---|
| Fig.7，16 B，100 MHz | SMMU启用时solo平均184.82 ns；`interf_tbu`平均272.29 ns，约1.47倍 | 只有尾部变慢、平均值不受影响 |
| Fig.7，16 B，300 MHz | solo观测最大值63.33 ns，`interf_tcu`113.33 ns，约1.79倍 | 普适WCET保证，或RISC-V也必然是1.79倍 |
| §IV-C/Fig.8，16 B | 相对solo，两个干扰场景吞吐约降32% | 小传输吞吐也不敏感 |
| §IV-C/Fig.8，1 KiB | 吞吐约低10% | 所有payload都接近无干扰 |
| §IV-C/Fig.8，4 KiB | 曲线近汇合；正文solo约1440、干扰最高约1410，**单位原样为KB/s** | 任意大任务、任意工作集或任意IOMMU均无明显吞吐损失 |

§IV-C扫描的是16 B到一页4 KiB的payload；它不是多MiB连续流或真实应用的普遍结论。Fig.8没有标明所用FPGA频率，相关段落也未明确绑定；不能自动继承Fig.7的100 MHz。正文和图轴均写KB/s，并同时讨论约100 ns额外延迟及毫秒量级payload传输，单位/计时口径存在需工件核对的疑点。本笔记不默改为MB/s，也不跨论文比较绝对带宽。Fig.7表格100 MHz的TCU平均为273.35 ns，附近正文为273.85 ns；采用前者时须注明来自表格。[Costa等，PDF第8–9页§IV-B/C、Fig.7/8](https://arxiv.org/pdf/2508.19670v1) <!--ref:bermuda2025--><!--anchor:page:8-9-->

§V-C讨论按stream的QoS、静态映射/page coloring等缓解，但明确完整实验评估不在本文范围；§V-B涉及SMMUv3的解释属于讨论，实际平台仍是SMMUv2。所核§III–V中未见本文E3式“选中一个A侧PTW响应、施加有界延迟、关联具体V首次命中和等待”的实验。因此差异可落在内部可见性与具体因果链，不能写成“首次发现多DMA IOMMU干扰”或“首次想到限流/分区”。[Costa等，PDF第9–10页§V](https://arxiv.org/pdf/2508.19670v1) <!--ref:bermuda2025--><!--anchor:section:V-->

**全文其余各节对定位的补充。** 摘要及§I把贡献写为结构逆向、干扰延迟表征和公开工件；§II的集中/分布式IOTLB概述是背景，不作为本文平台内部组织已被独立验证的替代。§VI将DevIOus、Thunderclap、IOTLB-SC归入安全近邻，并把既有SMMU使用模型、页表设计、I/O内存争用调节列为性能近邻；本页只核对该文如何组织这些引用，不据其转述替外部原作背书。§VII明确将跨架构相似行为保留为需后续确认的假设，§V-D列出跨架构、预取、分区与多租户扩展方向。这些补读没有增加RISC-V实测或缓解评估，也没有消解Fig.8口径疑点。[Costa等，PDF第1–3、9–10页，摘要及§I、II、V-D、VI、VII](https://arxiv.org/pdf/2508.19670v1) <!--ref:bermuda2025--><!--anchor:section:I-II,V-D-VII-->


### “约三分之一”能否反驳Bermuda

**先纠正命题。** Bermuda不是“平均吞吐大多不敏感”的普遍结论。Fig.7平均transaction延迟增加约47%–51%；Fig.8的16 B吞吐下降约32%，1 KiB约10%，4 KiB才近汇合。§IV-C／§V-B解释为近常数翻译附加时间被较大payload摊薄；§VII的跨架构相似性是待检验假设。[原文PDF8–10，§IV-B/C、§V-B、§VII](https://arxiv.org/pdf/2508.19670v1#page=8)

| 条件差异 | 已核事实 | 解释边界 |
|---|---|---|
| 大小与任务范围 | Bermuda延迟用16 B，但吞吐也测4 KiB；本文每项4 KiB、4096项共16 MiB COPY | **不能仅以16 B vs 4 KiB解释**；需要比较任务累计范围、供给和服务组织（B §IV；本文批量报告） |
| 地址复用／容量 | B的V循环63页首地址、micro-TLB64项，A N=56/1992；本文V源／目标各一页、A改变足迹 | 可能改变驱逐、walk和闭环节奏；未做同负载跨平台复现，不写成已证原因（B PDF7；本文批量／路径报告） |
| 共享结构 | B实测MMU-500 TBU/TCU，macro/PTW可见性有限；原平台硕论§7.1为monolithic shared IOATC；本文E3定位到共享串行控制状态／AR队列 | 可改变hit等待，但不能推成“Arm都快／RISC-V都阻塞”，也不能用规范名代替实际配置（B §III/V；硕论§7.1；E3§3） |
| 非翻译共跑损失 | B有SMMU开／关对照；本文A1双方零miss时V已从282.91降到149.40 MiB/s | 约三分之一是总共跑结果，不是三倍纯翻译损失；B的非翻译控制也不可忽略（B Fig.7；本文批量／路径报告） |
| 服务模型与计时 | 本文DS50/50、DS0/0是附加延迟诊断；B图7为ns、图8标KB/s并谈毫秒payload；本文计硬件末项完成窗口和COPY有效字节 | DS不是优先级干预；B吞吐timer／单位未明时不能比较绝对带宽（B §III/IV-C；本文DS／批量报告） |

上述“可能解释”均为**读者条件推断，未作跨平台因果验证**。本轮追查作者／组织公开GitHub、论文HTML及具名仓库搜索，仍未定位Bermuda专属工件；Fig.8频率、重复数、软件供给是否入窗及单位疑点保持unknown。不能从图7倒算发表图的“正确值”，也不擅改KB/s为MB/s。

可用于论文的表述是：本文承接Bermuda的问题，在另一固定实现与持续并发负载中观察到4 KiB批量COPY仍有明显损失，并以零miss基线、分段观测和局部PTW干预限定解释。结果**限制“大payload摊薄干扰”的可外推范围**，尚不是对Bermuda原实验数值的直接反驳。

<a id="eth"></a>
## 4. ETH/DATE2025：RISC-V FPGA性能、软件成本和host干扰已有实测

### 平台、负载与计时层次

| 维度 | v1直接证据 | 直接定位 |
|---|---|---|
| 平台与IOMMU | VCU128 FPGA原型实现Cheshire/CVA6 + 8核Snitch cluster；第9核控制其DMA；IOMMU采用Zero-Day Labs实现族，4个IOTLB项、1个device-directory项，覆盖一个(device, process)对 | 第1–2页§II、III-A；Fig.1 |
| 频率和内存 | Snitch 20 MHz，host/IOMMU 50 MHz；128 KiB LLC/SPM；AXI delayer延迟R/B响应，弥补FPGA低频下相对DRAM过快的情况 | 第2页§III-A；不是实硅原生时钟结论 |
| 工作负载 | RajaPERF子集，tiling/double buffering；Table I含gemm128、gesummv512、heat3d64、axpy32768、merge sort65536；每个计算offload两次，测热指令cache | 第3页§III-B、Table I |
| 应用层成本 | §IV-A/Fig.2/3区分host copy或map、OpenMP fork/join同步、device计算 | 第3–4页；软件开销核算不是本文新增概念 |
| kernel层成本 | §IV-B/Fig.4/Table II仅测device运行，排除host offload/sync；DMA region为核等待DMA的数据等待周期，compute region为其余周期 | 第4页；不是总DMA服务时间，也不能与整批COPY吞吐直接相除 |
| host干扰 | §IV-C/Fig.5在axpy运行期间由CVA6制造随机内存流，量平均IO PTW时间 | 第5页；已含共享总线/LLC干扰，不是纯单流无干扰研究 |
| 缓解条件 | LLC只cache host/PTW，DMA经bypass保持长burst；host需self-invalidate；map紧接offload，有利于PTE留在LLC | 第4–5页Listing 1及§IV-B/C |

[Koenig等，§II–IV](https://arxiv.org/html/2502.17398v1) <!--ref:riscvsva2025--><!--anchor:section:II-IV-->

### 可稳妥引用的结果及作者稿内部冲突

gemm的图文一致结果：无LLC时，在200/1000-cycle DRAM延迟条件下IOMMU额外开销为4.2%/17.6%；有LLC时为0.4%/0.7%。Fig.5及正文还报告LLC将PTW时间平均降低约15倍，host噪声使PTW平均增加约20%。这些支持“页表路径缓存和host流量影响翻译性能”，不能作为多DMA互扰已被覆盖的证据。[Koenig等，PDF第4–5页Fig.4/5](https://arxiv.org/pdf/2502.17398v1) <!--ref:riscvsva2025--><!--anchor:page:4-5-->

本轮实际PDF检查还发现以下冲突，保留原状，不替作者修数：

- **“所有kernel、所有latency都低于2%”与Fig.4不一致。** Fig.4在200-cycle处，heat3d的IOMMU+LLC标2.7%，merge sort标4.5%。所以不能用摘要或正文的泛化句覆盖图中例外。
- **81.3%的kernel归属不一致。** §IV-B正文把1000-cycle处81.3%写给heat3d；Fig.4将81.3%标在merge sort，heat3d为53.8%。不将81.3%当作已消解冲突的heat3d数据。
- Table II的merge sort、200-cycle、IOMMU+LLC单元格印成`6.96e3`，与同列百万量级其余值及Fig.4不匹配。没有原始结果前不凭猜测修成`6.96e6`，不从该格重算结论。

这些是作者稿第4页本身的图文/表格不一致，**不是**本项目复现失败；本轮没有执行前作代码。正式出版版是否已修正未核。引用gemm以及host干扰结论时仍须带各自条件，不能扩成跨负载普遍界限。[Koenig等，PDF第4页Fig.4/Table II/§IV-B](https://arxiv.org/pdf/2502.17398v1) <!--ref:riscvsva2025--><!--anchor:page:4-->

**全文其余各节对定位的补充。** 摘要/§I从scratchpad型可编程加速器的copy成本引出SVA，不以研究多设备隔离或公平性为贡献；§II明确Snitch为本文的device/accelerator，并交代单cluster DMA。§V对照软件PTW/预取、active forwarding、TLB coalescing和复用IOMMU映射的allocator，并说明DMA bypass LLC也有ESP等平台先例；这些方向不能由本文再次认领为新概念，相关原作仍按需要另读。§VI重申LLC及DMA bypass条件，并再次出现“全部kernel低于2%”的泛化句，未提供新数据来消解Fig.4的例外。§VII为项目资助致谢，未增加评估条件。脚注1提供公开平台分支入口，本轮未运行该分支。[Koenig等，PDF第1–2、5–6页，摘要及§I、II、V、VI、VII、脚注1](https://arxiv.org/pdf/2502.17398v1) <!--ref:riscvsva2025--><!--anchor:section:I-II,V-VII-->


### 后续作者材料与工件补核

[官方DATE2025海报](https://pulp-platform.org/docs/date2025/Cyril_Koenig_IOMMU_DATE_poster.pdf)全文及单页图表已实际核读：动机、结果和结论改用“所有被测kernel低于5%”，图中仍有2.7%／4.5%。[官方slides结论](https://pulp-platform.org/docs/date2025/Cyril_Koenig_IOMMU_DATE_video_slides.pdf)（PDF27／slide14）也为低于5%。这些是同一工作后续公开作者材料，不是独立论文；不能证明IEEE最终正文已修正。安全做法是引用gemm具名条件，不沿用v1全kernel低于2%的上界。

原稿PDF2脚注1指向[Carfield分支](https://github.com/pulp-platform/carfield/tree/date_iommu_evaluation)。检查时分支为dbb4ad3380e6e81fa7eac3955e2340752f29dab0；[Bender.lock 464–468行](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/Bender.lock#L464)锁定cyrilkoe/riscv-iommu的4897be154f5e12e14d5a85bb8d94b1bbf5755b84。已读README、lock、软件入口及通用offload loop，未执行工件。通用源码不能补出论文每tile字节数，当前分支身份也不是发表实验运行身份。


<a id="closest-origin"></a>
## 5. 最接近的平台来源与更早机制前史

### 5.1 Rodríguez2024：同平台多设备性能评价已存在

[机构公开硕论](https://hdl.handle.net/1822/99350)封面January2024、记录日期2024-02-13。它不是会议论文／预印本，按公开灰文献单列，技术重叠仍限制本文定位。本轮核读§6.1–6.3、§7及相关参考，不宣称115页全部精读。§6.1–6.2（印刷76–78／PDF94–96）给出CVA6、Genesys2、四iDMA、单请求入口、RR仲裁及裸机／Linux／Bao集成。§6.3.2（印刷87–88／PDF105–106）用32映射、每设备不同VMID/GSCID、1000次传输，CPU cycles＋HPM评价缓存配置。§7.1（印刷91／PDF109）明确认识共享IOATC设备争用，提出分布式IOATC未来方向。不能写成该组此前没有研究RISC-V多设备性能或不知道竞争。[原文§6–7](https://repositorium.uminho.pt/bitstreams/696f58ed-10e6-4674-b750-1de29e401413/download#page=94)

**并发核查：** 论文参考[55]指向公开tests，未锁运行commit。最接近四DMA／1000次／32映射的373ccb86f4dfed69851a8bc5b8a69ef731a82fcb（2023-11-07）中，latency_test每轮随机挑一个DMA、配置8 B、触发、等待idma_done完成，结算后才下一轮；header为1000次、读写各16 mappings。故**该已核源码是顺序跨设备访问，非四DMA同时持续供给**。缓存驻留仍可跨顺序请求相互影响，“顺序”不等于不存在缓存竞争。[函数1033–1081行](https://github.com/zero-day-labs/riscv-iommu-tests/blob/373ccb86f4dfed69851a8bc5b8a69ef731a82fcb/iommu_tests.c#L1033)、[参数41–46行](https://github.com/zero-day-labs/riscv-iommu-tests/blob/373ccb86f4dfed69851a8bc5b8a69ef731a82fcb/inc/iommu_tests.h#L41)、[DMA数量](https://github.com/zero-day-labs/riscv-iommu-tests/blob/373ccb86f4dfed69851a8bc5b8a69ef731a82fcb/inc/idma.h#L6)

**运行身份不能替换。** 同一快照生效GSCID全相同、独立GSCID示例被注释，latency_test默认注册也被注释，与硕论独立VMID文字不一致。2024-01-09的d5bca68b6e6e4a4bfdbdaff68abad26d53ea3f20和2024-06-19的1ea9fcbb69153926694c4fe6e75b9f5f68b94440仍逐笔等待，但参数不同。它们证明检查到的源码顺序，不能冒充Fig.6.31/32实际执行身份；论文BIT／commit／配置仍unknown。当前判定为极近平台／性能前作，**未核实同时多流干扰评价，不能绝对排除未公开运行**。[上下文23–88行](https://github.com/zero-day-labs/riscv-iommu-tests/blob/373ccb86f4dfed69851a8bc5b8a69ef731a82fcb/device_contexts.c#L23)、[注册8–9行](https://github.com/zero-day-labs/riscv-iommu-tests/blob/373ccb86f4dfed69851a8bc5b8a69ef731a82fcb/test_register.c#L8)、[一月函数](https://github.com/zero-day-labs/riscv-iommu-tests/blob/d5bca68b6e6e4a4bfdbdaff68abad26d53ea3f20/iommu_tests.c#L1003)

### 5.2 HERO／Kurth限制问题、方法与机制的新颖性

HERO2017 §2.1为ARM host＋RISC-V clusters／各cluster DMA／RAB，§3.2实测cluster数，§3.4及Fig.6(b)给出逐核事件图：一个core的L1 miss进入L2搜索时，另一个core的hit仍一周期完成。内部可见性、事件时序和hit-under-miss均已有明确前史。[正式稿PDF2、4–6，§2.1、§3.2、§3.4](https://carrv.github.io/2017/papers/kurth-hero-carrv2017.pdf)

Kurth2018 §II(b)明确描述outstanding-miss／数据缓冲满时，后续即使hit也无法翻译并反压各master；§IV-C解释共享TLB锁定限制多cluster DMA并设计retry／metadata；§V评价指针追踪、stream、walk和缓冲成本。**本文E3可以是该固定RISC-V控制器中一个具体依赖链的实板诊断，不能说首次发现miss阻塞hit。** [原文PDF2、6–9，§II(b)、§IV-C、§V](https://arxiv.org/pdf/1808.09751v1)

ANDARE2018 HERO是同族另一篇文章；其§4.1实际评估ARM A9 host＋单8核cluster，L1 32／L2 1024项hybrid MMU，不与2017多cluster或ICCD2018的L2 256项混写。正式PDF结构预检有xref覆盖告警，状态UNAVAILABLE，仅采用章节锚。[机构原文§2、§3.4、§4.1–4.2](https://www.research-collection.ethz.ch/server/api/core/bitstreams/f62fdb50-8f38-4ee9-bd84-ead60430dd70/content)

<a id="procedia-hur-followup"></a>
### 5.3 Procedia与Hur/Ro：定向续查后的具体未决项

**本轮未取得两篇可逐页核读的全文，不作“已排除同题”判断。** 2026-10-17新增检索覆盖作者、机构、会议材料、公开工件及原生OA位置；合法公开入口的获取结果详见§8.5。开放获取元数据、搜索索引正文片段、另一份讲稿和完整论文是不同层次的证据。

| 同题判定所需内容 | Procedia2025 | Hur/Ro2025 |
|---|---|---|
| RISC-V IOMMU及共享翻译结构 | 出版社Abstract确认RISC-V、distributed IOATC与Walk Cache IOATS；共享组织的原图／配置未核 | 机构Abstract确认多设备共享TLB；ISA与具体翻译结构未报告于摘要 |
| 多个独立DMA流实际同时供给 | 未核。IOATC数量不自动等于独立DMA数，接线支持并发也不自动等于实验并发 | 未核。摘要的multiple devices是问题背景，不能据此补出实际并发设备数 |
| 并发共享相对基线的争用影响 | 未核。须辨别固定总工作量的分摊／扩展效率、每流服务时间和共享IOATS排队 | 未核。须取得负载混合、替换策略比较及独立设备的指标／对照 |
| 可直接比较的干扰或缓解结果 | 摘要不能回答零miss邻居、他人PTW、数据访存竞争控制和失效范围 | 摘要称自适应缓存策略可缓解性能下降；未核其覆盖哪种竞争与缓解 |
| 全文／关键图表阅读状态 | **仅摘要**；未取得原PDF，未给原论文SHA或声称结构PASS | **仅摘要**；未取得原PDF，未给原论文SHA或声称结构PASS |

表内摘要事实分别来自[Procedia出版社Abstract](https://www.sciencedirect.com/science/article/pii/S1877050925024263)、[Yonsei机构Abstract](https://yonsei.elsevierpure.com/en/publications/enhancing-iommu-efficiency-in-heterogeneous-sacs-a-study-on-cache/)，其余单元格是本次核查状态，不是论文不存在相应实验的结论。

**Procedia的新增线索及边界。** 出版商API的`coredata`明确标开放获取及CC-BY-NC-ND 4.0，但本次取得的1951字节XML没有正文；`view=FULL`返回401。OpenAlex该DOI记录仅有DOI落地页、`pdf_url=null`，这是索引状态，不能扩大为全网无全文。[出版商元数据](https://api.elsevier.com/content/article/pii/S1877050925024263)、[OpenAlex位置字段](https://api.openalex.org/works/https://doi.org/10.1016/j.procs.2025.08.115)

Scribd搜索索引露出自称§5.3／Table 1的IOATC_NUM=1/2/4及并行服务描述；**这是未核镜像的索引提示，不是已核原文实验**。正常页面只取得描述、13页占位及订阅入口，没有取得可逐页核验的表图。本笔记不采纳其中时间、大小、时钟或“无额外延迟”作为正式比较值，也不由片段推定该文只有单设备。[公开索引入口](https://www.scribd.com/document/1046293233/基於RISC-V的資料中心級IOMMU性能優化及研究)

作者报告追查找到赵顺通的openEuler官方活动回顾及其材料仓库线索；相关RVSC2024公开材料另取得郑律的X100讲稿。**该讲稿不是Procedia论文**：其PDF13页可见DMA／PCIe端经DTI连接IOATS，IOATC的数据地址走System Bus；只核该图，不能把其配置或实验移植给Procedia。[openEuler回顾“赵顺通”段及材料入口](https://www.openeuler.org/zh/news/openEuler/20241220-meetuop/20241220-meetuop.html)、[X100讲稿PDF第13页](https://github.com/cnrv/RVSC2024/blob/main/slides/0822A/029-%E9%83%91%E5%BE%8B.pdf)

**Hur/Ro的新增线索及边界。** 作者实验室公开列表的ICEIC2025条目写作“Enhancing IOMMU Efficiency in Heterogeneous **SoCs**”；Crossref／Yonsei题录为“**SaCs**”，按同作者、同会场、同题名其余部分合并检索，保留拼写差异。该作者条目未附有效PDF／slides链接，机构“Access to Document”只有DOI。[作者列表2025条目](http://escal.yonsei.ac.kr/publications.html)、[机构题录及Abstract](https://yonsei.elsevierpure.com/en/publications/enhancing-iommu-efficiency-in-heterogeneous-sacs-a-study-on-cache/)

Crossref参考文献元数据包含Arm MMU-700／SMMUv3、ChampSim、gem5、XSBench等线索；**引用不能证明被测ISA、模拟器或并发负载**，不能据此判成Arm而排除RISC-V。[Crossref该DOI的`reference`字段](https://api.crossref.org/works/10.1109/ICEIC64972.2025.10879735)

**对最终裁决的影响。** 私有IOATC不排除共享IOATS／PTW竞争，PCIe等设备的独立请求流也不因未用iDMA而排除；不要求论文使用victim/aggressor名称。若全文证实实际并发共享翻译资源并评价争用影响，即按同题／实质重叠重新比较，不能靠分布式结构、仿真或平台不同规避。若原文只评价缓存命中成本或模块数量扩展，则该结果仍是性能前作，是否覆盖本文干扰问题须由供给、工作量和指标决定。**现阶段两种情况均未被全文裁定，故G2保持待补核；候选新意1与3尤须保留这两篇的覆盖风险，不能保证其未做候选新意2的内部诊断。**

<a id="comparison"></a>
## 6. 本文可成立的具体差异及当前证据边界

本表“本文”指项目现存历史证据与拟写作范围，**不是**声称统一原版基线、所有版本或新板测已经完成。新统一基线的实际结果由其独立数据及运行身份决定，本页不预报。

| 维度 | Bermuda / ETH已覆盖什么 | 本文现有证据与可写差异 | 边界与原始入口 |
|---|---|---|---|
| 研究对象 | Bermuda为Arm共享TBU/TCU；ETH为RISC-V单cluster DMA与host噪声 | 固定实现的多个真实iDMA共用开源RISC-V IOMMU；研究翻译结构与串行服务如何影响相邻DMA | 更换ISA/平台本身不足以宣称首次；必须绑定实际RTL、配置及软件版本 |
| 吞吐负载 | Bermuda对一页以内payload作大小扫描；ETH为有计算/双缓冲的应用kernel | 9月硬件批量：每项4 KiB、4096项、V有效COPY总量16 MiB，warmup256；V固定源/目标各一页，A改变工作集 | [批量报告](../../../iommu-platform/docs/batch-results-review.md)；原始[主矩阵](../../../iommu-platform/out/mac-batch-main-20260916-112954-run-9sa69rhc/records.json)/[扩展](../../../iommu-platform/out/mac-batch-ext-20260916-114036-run-8pjv_djc/records.json) |
| 观测 | Bermuda有PMU/ILA和SMMU开关，macro/PTW可见性有限；ETH量device runtime及平均PTW | E4分入口、IOMMU内部、出口地址反压、写后段；A8相对A1内部读写经过时间增加，V计数多数/该组为命中 | [分段报告](../../../iommu-platform/docs/path-results-review.md)；分段重叠，不能直接相加成总慢化占比 |
| 局部因果 | 两篇所核评估段没有本项目这种逐请求有界PTW响应干预 | E3对选中的A侧S1 PTW响应加入128-cycle诊断延迟；4个cold互补对的A剩余服务与V剩余等待增量同为127–130周期，目标V首次命中 | [E3受控报告](../../../iommu-platform/docs/e3-controlled-results-review.md)；64 B有限诊断、n=4，不能解释全部A8慢化，也不是缓解收益 |
| 缓解与负结果 | Bermuda只讨论候选；ETH已评价LLC与DMA bypass | 项目已有IOTLB容量分区P、命中旁路N、PN组合及替换历史隔离；隔离后A miss同为2048，PN仍比P慢12.67%–12.79% | [组合历史报告](../../../iommu-platform/docs/composition-history-isolation-20261007.md)、[comparison](../../../iommu-platform/out/composition-isolated-20261007/comparison.json)；n=3描述性，分区/旁路不是新概念，扩展不是stock |
| 完成目标 | Bermuda主要是transaction延迟/吞吐；ETH分软件与kernel成本 | 严格区分硬件窗口、软件发现完成、读/写腿、双方完整任务和排空；缓解收益须保留A侧代价 | 既有宏观V窗口不能自动补成双方端到端应用收益；待统一图表按数据实际覆盖写 |

<a id="one-third"></a>
### “约三分之一”及基线损失的准确口径

批量历史数据中，V=DMA0各条件10次中位吞吐为Solo **282.91**、A1 **149.40**、A8 **103.24**、A64 **94.84 MiB/s**。用报告显示精度相除，A64/Solo约**0.3352**，A8/Solo约**0.3649**。因此“降至约三分之一”应具体绑定 **A64**，不得误标成A8。主矩阵与A64扩展并非同轮随机化实验，扩展没有同期重采baseline或交换角色。[数字及计时定义](../../../iommu-platform/docs/batch-results-review.md)

Solo→A1中双方miss为0，V已明显变慢；Solo→A64的全部损失因此不能称为“纯翻译成本”。A1→A8新增慢化伴随A翻译活动和V内部等待；E4定位与E3局部干预补强解释，但仍不能把翻译服务、数据/PTW访存和内部排队的全部份额完整分摊。HPM使用非原子软件快照，不是每个请求的原子真值；多数V miss为0也不能单凭该计数推出全部V首次查询都命中。[批量边界](../../../iommu-platform/docs/batch-results-review.md)、[路径边界](../../../iommu-platform/docs/path-results-review.md)、[E3边界](../../../iommu-platform/docs/e3-controlled-results-review.md)


### DS、缓解与失效的准确口径

- **DS是附加延迟诊断，不是已验证的页表访存优先级调整。** 三阶段50/50→0/0→50/50中，A8相对A1额外硬件耗时为44.71%→10.50%→44.72%；DS影响多类IOMMU数据结构请求，映射、内部状态、miss模式并未完全固定，不能将差值算为纯PTW成本。[DS0报告“主要发现／边界”](../../../iommu-platform/docs/ds0-batch-results-review.md)、[回切报告](../../../iommu-platform/docs/ds50-retest-results-review.md)
- **V/A取舍与组合负结果分开报告。** P4M首24个平衡样本描述限速使V更快、同窗口A有效速率下降，不能与P/PN试验拼成统一“分区＋限速”结果。[P4M§6](../../../iommu-platform/docs/p4m-formal60-recovery.md)。P/PN替换历史隔离后，A miss均2048，PN仍慢12.67%–12.79%，n=3；等miss数不等地址、walk或时间线，唯一根因未定。[组合报告§3–4](../../../iommu-platform/docs/composition-history-isolation-20261007.md)
- **失效只属仿真补充。** Verilator／行为DDR、真实双DMA/CQ，28配置各一次；宽失效使邻居完整时间增14.8%–17.6%，收窄域／地址清除失效归因miss，但发起方较宽失效慢9.7%–10.0%。CPU/Linux未执行，旧／本地驱动行为不能泛化为当前主线或板测。[失效报告“真实部署／干扰大小／开销”](../../../iommu-platform/out/invalidation-interference-20261017/README.md)
- 以上绑定历史研究／必要修复配置，不以关闭扩展开关称原版，也不把新统一基线预报为已完成。具体版本／BIT／时序边界保留。[版本导航](../../../iommu-platform/docs/versions.md)


<a id="novelty-claims"></a>
## 7. 可以合法主张的新意与不能主张的点

以下四项是**限定范围的候选贡献表述**，有已核原文差项和本项目证据支撑；尚未取得全文的近邻不在已排除集合内。不使用领域级首次，也不把待统一复测的数据写成已完成。

| 可写的具体增量 | 最近前作的具体缺项／已有内容 | 本文证据与条件 |
|---|---|---|
| 1. 固定标准开源IOMMU中，持续并发4 KiB批量DMA的总慢化、零miss共跑基线与翻译压力增量的定量刻画 | 硕论§6.3.2为缓存／平均latency、关联bench逐笔等待；Bermuda§IV-C的4 KiB近汇合；DATE§IV为应用及host噪声。已核评估没有本文这组同条件数据链 | 批量＋路径报告；不能把总损失全归PTW，也不能认领“首次分离翻译与内存竞争”；仍待§5.3两项全文核实；不能保证它们未覆盖此增量 |
| 2. 本控制器中A的具名S1 PTW响应延长如何传导到一个随后首次命中的V read的局部实板证据 | Bermuda§III–V、DATE§III–IV未采用此有界逐请求响应干预；但HERO§3.4／Kurth§II(b)已知可见性和hit-under-miss／阻塞，故不是新现象 | E3§3–5，64 B、cold互补对n=4，A剩余服务与V剩余等待增量127–130周期；不解释全部宏观慢化或构成通用HOL定理 |
| 3. 同一硬件路径普通缓解的条件性取舍：等A miss计数下PN仍比P慢，并记录保护V的A侧代价 | Bermuda§V-C未实验评价其候选；DATE§IV评LLC与DMA数据绕cache；MASK§5已有翻译／数据协同，故不认领组合思想或“组合会冲突”的普遍认识 | P/PN§3–4与P4M§6；PN n=3为描述性负结果，两套试验不能拼成统一完整矩阵，唯一根因未定 |
| 4. 特定RISC-V失效范围／驱动行为下，邻居miss与双方完整时间代价的量化 | Bermuda§IV、DATE§IV未评这类CQ失效范围矩阵；IEICE§5.5明确将global invalidation pressure置于范围外；FBI及其他失效近邻未全文排除 | 失效28配置仿真；相对M0/M1、同／异域分别报walk与双方时间，不能认领收窄／合批方法首创，不能冒充Linux实板 |

**不能主张：**

1. 首次RISC-V FPGA IOMMU集成、多iDMA或翻译性能评价：硕论§6、DATE§III–IV、DMAAUTH§6.1／§7.2、IEICE§3–5已有相应证据。
2. 首次发现多设备共享翻译干扰、hit可能被他人miss拖住：Bermuda§IV、硕论§7.1、Kurth2018§II(b)、HERO2017§3.4均有直接边界。
3. 首创分区、限速、hit-under-miss、PTE优先或翻译／数据协同：Bermuda§V-C已有候选，Hao2017§IV-B/C有hit-under-miss，MASK§5.2–5.4有资源控制与调度。本文N和DATE的数据绕LLC也不是同一种bypass。
4. 首次多加速器共享翻译／隔离性能研究：ShareMMU§V–VII构成明确反例；只能继续比较具体实现、条件、观测和代价。
5. “4 KiB可摊薄一切翻译干扰”、证明Bermuda错误、或RISC-V架构必然HOL：均超出两边已核配置。
6. DS优先级改善纯PTW成本44.7%→10.5%、等miss即等服务、失效仿真即Linux板测、保护V无A代价：均超出现有原始报告。

**建议定位句：** 本文在一个固定、公开的RISC-V IOMMU实现上，对持续并发DMA进行FPGA刻画；结合零miss共跑基线、分段观测与具名请求干预，说明翻译活动怎样影响邻居命中请求，并报告普通缓解组合及失效范围的条件性取舍。贡献是经验证的新实测认识与适用边界，不是新IOMMU或新的通用隔离机制。


<a id="search-record"></a>
## 8. 检索记录

### 8.1 日期、来源和访问边界

本轮日期**2026-10-17（JST）**，主窗口2024-01-01至当日，关键先例回溯不限于2024以后。按原文／正式元数据辨别首发、修订和未来卷期，不用“几个月前”或crawl标签代替日期。旧笔记2025–2026检索保留为历史范围，本轮实际补2024，不倒改旧收据。定向查新不冒称系统综述、全库筛查或完整召回，不虚构命中总数。

| 来源 | 实际渠道 | 访问／覆盖局限 |
|---|---|---|
| arXiv | 作者稿／HTML直接阅读；真实API按all:IOMMU、all:IOTLB、all:SMMU、all:IOMMU AND all:RISC-V查元数据；外部域检索 | API元数据不是全文检索；人工核年份 |
| IEEE Xplore | 原生入口与具名论文页尝试、DOI题录；外部site:检索 | JS验证／202空体／不可访问，改用作者机构稿，不称IEEE全库无同题 |
| ACM DL | 原生query页尝试，外部域／题名／DOI查询 | 原生不可访问，域查询弱覆盖，作者原文补检 |
| dblp | 已读Bermuda等题录及作者消歧列表，外部域检索 | API/XML有202防护，部分单页失败；题录不代替方法原文 |
| Google Scholar | 原生入口尝试，七关键词site:scholar.google.com补检 | 原生不可访问；域检索多无相关结果／漂移，**不称已完成Scholar站内检索** |
| 作者／机构／会场 | Minho/Pinto、Martins、Rodríguez列表，PULP2024–2026列表，正式稿／程序／Summit摘要 | 列表可能滞后；talk／poster无正文时维持题录／摘要 |
| 前后向引文（补充发现索引） | Bermuda参考[36]–[50]；DATE参考[7]–[10]、[18]–[26]；S2的Bermuda DOI引文4项，OpenAlex2项；DATE两版本OpenAlex引文0；S2相关查询429 | 覆盖不一致，0不证明没有后作，科学判断回原文 |
| 工件／固定源码 | tests三份commit、Carfield branch/lock；Bermuda工件定位追查 | 未运行前作；当前树不等历史运行；Bermuda专属工件未定位 |

**会场覆盖。** RTCSA核Bermuda、追2026 Wheel；RTAS以会场全称／作者／IOMMU补检，另命中2025 FPGA bus isolation题录但全文403；ECRTS核DAAT-MCS、追2024 MCU；DATE核SVA及archive、AXI-REALM背景；DAC／ISPASS／IISWC／CARRV按下列会场和域查询补检。CARRV2024官方程序直接读过，2025／2026路径不可访问；IISWC三年所试program路径不可访问，只有有限外部域补检，不称逐篇筛完proceedings。Summit以2024–2026作者／IOMMU查询及2025／2026程序／海报补检，2024旧presentations路径404。

### 8.2 已看过的非同题近邻与未决项

每行注明实际阅读级别。摘要／题录级“未决”不计为已排除。

| 条目与入口 | 实际阅读 | 有界分类／未决点 |
|---|---|---|
| [SP-IMPact，NG-RES2025](https://drops.dagstuhl.de/entities/document/10.4230/OASIcs.NG-RES.2025.5) | 正式§3–6，重点§4.1/§5 | CPU MiBench／buffer stressor，LLC／bus；§4明确本轮不评MBR。正式DOI末位.5，旧目录.2不沿用 |
| [H-MBR，NG-RES2025](https://drops.dagstuhl.de/entities/document/10.4230/OASIcs.NG-RES.2025.4) | §3–4 | CPU bus PMU计费、idle／timer恢复；所核评价不是DMA翻译流 |
| [DAAT-MCS，ECRTS2026](https://drops.dagstuhl.de/entities/document/10.4230/LIPIcs.ECRTS.2026.18) | §1、§5.1、§6–7 | 三CPU VM／LLC配置，QoS为扩展讨论；不是新的共享DMA IOMMU实测 |
| [A “World” Reality Check，2602.05002v1](https://arxiv.org/pdf/2602.05002v1) | §III-B、§V–VI、§VIII–IX | IOPMP/S-WC/M-WC、单DMA单beat测checker；不是共享翻译缓存。绑定二月预印本，不声称十一月卷期版已读 |
| [Secure “Wheels”，2410.09839v1](https://arxiv.org/pdf/2410.09839v1) | §V–VI | PMP/WID提案／路线图，相关干扰评价为未来指标 |
| [Contention Behind the Wheel，RTCSA2026](https://doi.org/10.1109/RTCSA71153.2026.00018) | **仅正式题录** | 63–74页、作者日期已核；未得primary技术正文／摘要，不据二手MCU摘要排除 |
| [Shared Resource Contention in MCUs，ECRTS2024](https://doi.org/10.4230/LIPIcs.ECRTS.2024.5) | **仅题录／引用关联** | 2024同组候选，技术范围未决，不据题名排除 |
| [AXI-REALM，2501.10161v2](https://arxiv.org/abs/2501.10161v2)及2024短稿 | **本轮仅摘要**，旧卡另存 | 摘要为互连预算窗口／fragmentation／fairness；不借摘要完成IOMMU同题排除；版本归同工作族 |
| [HeroSDK，ICCD2024](https://www.research-collection.ethz.ch/entities/publication/18e9002c-8780-4c59-b1ba-f81cc4a2e39a) | **仅官方摘要**；PDF429 | OpenMP／offload近邻，全文未决 |
| [Numpy/OpenBLAS，2504.03677v1](https://arxiv.org/pdf/2504.03677v1) | 两页全文§1–5／参考 | §4把IOMMU zero-copy列未来工作，预测不当实测 |
| [AraOS，2504.10345v1](https://arxiv.org/html/2504.10345v1) | 摘要、§2–3 | Ara VLSU共享CVA6核内MMU，不是所核多DMA IOMMU |
| [CVA6-VMRT，2504.05718v1](https://arxiv.org/html/2504.05718v1) | 摘要、§2–3 | 核内TLB／VM干扰，TLB字样不等IOMMU |
| [CVA6-RT，2606.26177v1](https://arxiv.org/html/2606.26177v1) | 摘要、§2–3 | 核内TLB/SPM与中断延迟，非所核多DMA共享翻译 |
| [Unleashing the Penguin，Summit2026](https://riscv-europe.org/summit/2026/posters) | **仅官方摘要**；B/98、BWUHVG | TT-Ascalon可编程device model/Linux验证；未得poster／全文，性能并发范围未决 |
| [SPMP-Based Virtualization，Summit2026](https://riscv-europe.org/summit/2026/presentations#P-XFYMDU) | **仅官方摘要** | SPMP层数／entry count的资源与时序；不作全文级排除 |
| [Commercial-Grade RISC-V IOMMU with Verification，2024 talk](https://10xengineers.ai/publications/) | **仅作者事件题录**；未观看录像 | 验证／实现近邻，内容未决 |
| [Let’s Get Physical，Access2025](https://doi.org/10.1109/ACCESS.2025.3636061) | **仅题录／作者列表** | 不据MMU-less题名排除全部实验 |
| [Secure Nano-UAV SoC，2401.03531](https://arxiv.org/abs/2401.03531) | **仅题录发现** | Pinto/PULP平台线索，本轮未完成技术核读，不作排除 |
| [WaferTrans，2609.06125](https://arxiv.org/abs/2609.06125) | §IV-A／Fig.6 | 已核平台AMD GPU/MGPUSim，非标准RISC-V IOMMU实板，不冒称全篇重审 |
| [Gem5-AcceSys/MatrixFlow，2603.19057](https://arxiv.org/abs/2603.19057) | §3.2、§4.1／Table5 | ARMv8/SMMU、单加速器多channels；不等多独立RISC-V设备，以arXiv身份引用 |
| [VA-RDMA ARMv8，2511.19258](https://arxiv.org/abs/2511.19258) | **仅摘要／Comments** | ARM；2017本科论文2025上传，不当新RISC-V结果 |
| [VA-RDMA page faults，2511.21018](https://arxiv.org/abs/2511.21018) | **仅摘要／Comments** | ARM；2019硕论2025修订上传，全文未核 |
| [GPUBreach，2605.03812](https://arxiv.org/abs/2605.03812) | **仅摘要** | GPU Rowhammer／页表安全；不用于性能新颖性排除 |
| [IOTLB-SC，AsiaCCS2023](https://arxiv.org/abs/2202.11623) | **本轮仅摘要**；旧卡有§7局部 | 共享IOTLB跨设备时序影响已有；不冒称本轮重读全文／分区实测 |
| [IOTLB Wall，2023](https://pmc.ncbi.nlm.nih.gov/articles/PMC10280580/) | 正文Testbed／Basic model／Buffer shuffling等相关段 | Intel/AMD服务器NIC，IOTLB／页大小成本；不认领吞吐墙／大页概念 |
| [Host Interconnect Congestion，HotNets2022](https://research.google/pubs/understanding-host-interconnect-congestion/) | **仅作者摘要** | host/NIC/IOMMU拥塞背景，不据摘要排除所有细节 |
| [Fast & Safe IO Memory Protection，SOSP2024](https://doi.org/10.1145/3694715.3695943) | **仅机构摘要／题录** | IOTLB成本／失效近邻，未核细部方案／覆盖 |
| [FPGA Bus Transactions Isolation，RTAS2025](https://www.iris.sssup.it/retrieve/73ee842a-4597-4a84-a3d4-a2bf6bf31cb6/A_Design_Flow_to_Securely_Isolate_FPGA_Bus_Transactions_in_Heterogeneous_SoCs.pdf) | **仅机构题录／检索片段**；全文403 | bus isolation近邻，平台与并发翻译评价未核 |
| [UAV Jitter Resilient Disturbance Compensation，ICCAD2026](https://doi.org/10.1109/ICCAD69956.2026.11643105) | **仅Bermuda引文题录** | 未得primary技术正文，不据UAV题名排除 |

### 8.3 本地catalog／summaries复核

定向复核Bermuda、riscvsva2025、SP-IMPact、HERO/socdma、ShareMMU、MASK、accelerator_translation2017、AXI-REALM、IOTLB Wall/SC、Linux SVA、zero-day-labs IP/demo及PULP工件等。目录是历史聚合，不是全文已读清单；论文、代码、规范、专利和提及不能混算。旧SVA“所有kernel<2%”已被Fig.4和后续海报限定，SP-IMPact DOI冲突以正式.5处理；本轮未改目录／摘要／用户已读标记。[目录](../../../references/catalog.json)、[摘要入口](../../../references/summaries/README.md)

[CBQRI IOMMU QoS规范§4.1](https://docs.riscv.org/reference/cbqri/v1.0/qos_iommu.html)和[上游IP](https://github.com/zero-day-labs/riscv-iommu)仅作为接口／来源，不能当作已经完成并发性能研究。支持多个iDMA与本次新增实测是不同命题。

### 8.4 原样查询账本

以下为实际发送的字符串，按执行范围归组，跨组重复保留。常规查询都是**外部网页索引**；site:不等于站内检索，布尔／日期操作符可能被引擎忽略。每批返回合并，不能据此伪造逐词命中数。原生API、作者页和工件读取见§8.1。本文件自含查询和结论所需入口，不依赖未提交的工具响应文件。


#### Minho／zero-day-labs作者和工件追踪（48条）

~~~text
"Bermuda" "IOMMU" Martins Pinto
"Sandro Pinto" "IOMMU" 2025 2026
"zero-day-labs" "IOMMU"
"Sandro Pinto" "IOMMU" publications
"Bermuda" "IOMMU" GitHub artifact
"IOMMU" "Manuel Rodríguez" 2024 2025 2026
Sandro Pinto publications 2026 IOMMU temporal isolation
"José Martins" "IOMMU" 2026
"Manuel Rodriguez" "IOMMU" 2025 2026
"Diogo Costa" "IOMMU" 2026
"Bermuda" "IOMMU" -site:arxiv.org -site:researchgate.net -site:dblp.org -site:rankdblp.com
"IOMMU" "Sandro Pinto" "RTAS"
"IOMMU" "Sandro Pinto" "2024"
"IOMMU" "Sandro Pinto" "2026" -site:docs.riscv.org -site:topofmind.dev
"DAAT-MCS" ECRTS 2026
"Contention Behind the Wheel" IOMMU
"Bermuda Triangle of Contention" citations -site:themoonlight.io
"Bermuda Triangle" "Costa" "github"
"Diogo21Costa" IOMMU
"Beyond the Bermuda" artifact code
"Contention Behind the Wheel" pdf
"DAAT" "IOMMU"
"Sandro Pinto" IOMMU DATE DAC ISPASS IISWC CARRV 2024 2025 2026
"Manuel Rodríguez" "IOMMU" "2024" "Summit"
"Open-Source Commercial-Grade RISC-V IOMMU with Verification"
"Sandro Pinto" "IOMMU" "QoS"
"IOMMU" "Rodriguez" "2024" "verification"
"IOMMU" "Rodríguez" "2025"
site:riscv-europe.org "IOMMU" "2024"
site:riscv-europe.org "IOMMU" "2026"
"An open-source RISC-V input/output memory management unit" 2024
"Contention Behind the Wheel" site:uminho.pt
"Open-Source Commercial-Grade" IOMMU -site:community.twstalker.com
"Bermuda" "IOMMU" "artifacts"
"Bermuda" "IOMMU" "GitHub" -site:themoonlight.io
"An open-source RISC-V input/output memory management unit" "99350"
"Manuel" "IOMMU" "2024" site:repositorium.uminho.pt
"IOMMU" "Saad Waheed"
"RISC-V" "IOMMU" "Sandro Pinto" "RTAS 2026"
"IOMMU" "Pinto" "CARRV" "2024"
"Sandro Pinto" "IOMMU" "IISWC"
"Sandro Pinto" "IOMMU" "ISPASS"
"Sandro Pinto" "IOMMU" "DATE"
"José Martins" "IOMMU" QoS partitioning
"DAAT-MCS" "IOMMU"
"Contention Behind the Wheel" site:arxiv.org
"Contention Behind the Wheel" site:repositorium.uminho.pt
"Bermuda" "IOMMU" site:github.com/Diogo21Costa
~~~

#### ETH／PULP和HERO追引（35条）

~~~text
"IOMMU" "Koenig" "2024" "2026"
"IOMMU" "Zelioli" "2026"
"IOMMU" "Benini" "2024" "2025"
"IOMMU" "Kurth" "2024" OR "2025" OR "2026"
site:pulp-platform.org IOMMU after:2024-01-01 before:2026-10-18
site:iis.ee.ethz.ch IOMMU after:2024-01-01 before:2026-10-18
"Benini" "address translation" after:2024-01-01 before:2026-10-18
"Evaluating IOMMU-Based Shared Virtual Addressing" -site:arxiv.org
"10992962" pdf
"Evaluating IOMMU-Based Shared Virtual Addressing" "tile"
site:pulp-platform.org "IOMMU" "2026"
site:pulp-platform.org "IOMMU" "2024"
"Koenig" "IOMMU" after:2024-01-01 before:2026-10-18 -site:mail-archive.com -site:opennet.ru
"Zelioli" "DMA" after:2024-01-01 before:2026-10-18
"Kurth" "IOMMU" after:2024-01-01 before:2026-10-18
site:pulp-platform.org "DMA" "2024" "Benz"
"HeroSDK" IOMMU
"Koenig" "IOMMU" "Cyril" "2026"
"Enrico Zelioli" "IOMMU" -site:researchgate.net
"Andreas Kurth" "IOMMU" "2024" OR "2025" OR "2026"
"HERO: an Open-Source Research Platform" pdf
"HERO" "RAB" "RISC-V" "clusters"
"Cyril Koenig" "IOMMU" citations -site:researchgate.net -site:themoonlight.io -site:lacuna.tiptreesystems.com -site:riscv-linux.com
"IOMMU" "Cyril Koenig" 2024 2025 2026 -site:mail-archive.com -site:researchgate.net -site:lacuna.tiptreesystems.com
"HeroSDK" "IOMMU" "2024" "2025"
"RISC-V" "IOMMU" "multi" "Benini"
"Scalable and Efficient Virtual Memory Sharing" "interference"
"HERO" "HW/SW Exploration" filetype:pdf
"HERO" "3295821" pdf
site:arxiv.org "HERO" "HW/SW Exploration"
"HeroSDK" "2024" filetype:pdf -site:researchgate.net -site:themoonlight.io
"A Reliable, Time-Predictable Heterogeneous SoC" IOMMU
"A Reliable, Time-Predictable Heterogeneous SoC" pdf
"HeroSDK" "IOMMU" "memory" "2024" -site:researchgate.net
"HERO" "3295821" "RAB"
~~~

#### 通用关键词、五数据库域和未决近邻（108条）

~~~text
"IOMMU interference" after:2024-01-01 before:2026-10-18
"IOTLB contention" after:2024-01-01 before:2026-10-18
"shared address translation" DMA after:2024-01-01 before:2026-10-18
"SMMU contention" after:2024-01-01 before:2026-10-18
"RISC-V IOMMU" performance after:2024-01-01 before:2026-10-18
"mixed-criticality" IOMMU after:2024-01-01 before:2026-10-18
"host congestion" IOTLB after:2024-01-01 before:2026-10-18
"Performance optimization and research of data center-level IOMMU based on RISC-V"
"End-to-End Implementation of IOMMU-Based DMA Isolation" RISC-V
"Unleashing the Penguin" IOMMU
"RISC-V" IOMMU contention interference performance 2024 2025 2026 -site:reddit.com
IOTLB "host" congestion 2024 2025 2026 Fast Safe
site:arxiv.org (IOMMU OR IOTLB) (contention OR interference OR performance) after:2024-01-01 before:2026-10-18
site:ieeexplore.ieee.org (IOMMU OR IOTLB) (contention OR interference OR "RISC-V") 2024 2025 2026
site:dl.acm.org (IOMMU OR IOTLB) (contention OR interference OR "RISC-V") 2024 2025 2026
site:dblp.org (IOMMU OR IOTLB) 2024 2025 2026
"10.1016/j.procs.2025.08.115" authors pdf
"data center-level IOMMU" pdf
"Unleashing the Penguin" "Goparaju"
"IOMMU-Based DMA Isolation" Waseda pdf
"Shuntong Zhao" IOMMU pdf
"Mingsen Hong" IOMMU
"Yanqing Liu" IOMMU Waseda
"Goparaju" "Piggin" IOMMU programmable
"IOMMU" site:kasahara.cs.waseda.ac.jp 2026
"Programmable Device Model" IOMMU Tenstorrent
"Shuntong" "IOMMU" China Mobile
"RISC-V" "IOMMU" "IOATS" 2025
site:dl.acm.org IOTLB 2024
site:dl.acm.org IOMMU 2025
site:dblp.org "IOMMU" "2026"
site:scholar.google.com "IOMMU" "interference"
"IOTLB" "2024" site:dl.acm.org/doi
"IOMMU" "2025" site:dl.acm.org/doi
"IOMMU" site:scholar.google.com/scholar
"IOTLB" "contention" site:arxiv.org
"Performance optimization and research" IOMMU Zhao Hong Wang filetype:pdf
"Shuntong Zhao" "Mingsen Hong"
"基于RISC-V" "数据中心级" IOMMU
"Panshi DPU4.0" IOMMU
"Enhancing IOMMU Efficiency in Heterogeneous SaCs" pdf
"IOMMU" "RTAS" "2024" "2025" "2026"
"IOMMU" "ECRTS" "2024" "2025" "2026"
"IOMMU" "ISPASS" "2024" "2025" "2026"
"IOMMU" "DAC" after:2024-01-01 before:2026-10-18
"IOMMU" "IISWC" after:2024-01-01 before:2026-10-18
"IOMMU" "CARRV" 2024 2025 2026
"IOMMU" "RTAS" -powerpc -qemu 2024 2025 2026
"Enhancing IOMMU Efficiency" filetype:pdf
"Won Hur" IOMMU github
"Won Woo Ro" IOMMU cache policy pdf
IOMMU interference after:2024-01-01 before:2026-10-18
IOTLB contention after:2024-01-01 before:2026-10-18
shared address translation DMA after:2024-01-01 before:2026-10-18
SMMU contention after:2024-01-01 before:2026-10-18
RISC-V IOMMU performance after:2024-01-01 before:2026-10-18
mixed-criticality IOMMU after:2024-01-01 before:2026-10-18
host congestion IOTLB after:2024-01-01 before:2026-10-18
IOMMU interference after:2024-01-01 before:2026-10-18
IOTLB contention after:2024-01-01 before:2026-10-18
shared address translation DMA after:2024-01-01 before:2026-10-18
SMMU contention after:2024-01-01 before:2026-10-18
RISC-V IOMMU performance after:2024-01-01 before:2026-10-18
mixed-criticality IOMMU after:2024-01-01 before:2026-10-18
host congestion IOTLB after:2024-01-01 before:2026-10-18
IOMMU interference after:2024-01-01 before:2026-10-18
IOTLB contention after:2024-01-01 before:2026-10-18
shared address translation DMA after:2024-01-01 before:2026-10-18
SMMU contention after:2024-01-01 before:2026-10-18
RISC-V IOMMU performance after:2024-01-01 before:2026-10-18
mixed-criticality IOMMU after:2024-01-01 before:2026-10-18
host congestion IOTLB after:2024-01-01 before:2026-10-18
IOMMU interference after:2024-01-01 before:2026-10-18
IOTLB contention after:2024-01-01 before:2026-10-18
shared address translation DMA after:2024-01-01 before:2026-10-18
SMMU contention after:2024-01-01 before:2026-10-18
RISC-V IOMMU performance after:2024-01-01 before:2026-10-18
mixed-criticality IOMMU after:2024-01-01 before:2026-10-18
host congestion IOTLB after:2024-01-01 before:2026-10-18
IOMMU interference after:2024-01-01 before:2026-10-18
IOTLB contention after:2024-01-01 before:2026-10-18
shared address translation DMA after:2024-01-01 before:2026-10-18
SMMU contention after:2024-01-01 before:2026-10-18
RISC-V IOMMU performance after:2024-01-01 before:2026-10-18
mixed-criticality IOMMU after:2024-01-01 before:2026-10-18
host congestion IOTLB after:2024-01-01 before:2026-10-18
"FBI: Framework for Efficient Batching" IOMMU
"Enhancing IOMMU Efficiency" "gem5"
"Won Woo Ro" "Computer Architecture" lab publications
"Unleashing the Penguin" IOMMU pdf Tenstorrent Goparaju
"FBI" "IOMMU" "Jiho"
"IOMMU" site:escal.yonsei.ac.kr
"Enhancing IOMMU Efficiency" "RISC-V"
"Unleashing the Penguin" "IOMMU" -site:riscv-europe.org
"FBI: Framework" "IOMMU" -site:lab.hanbat.ac.kr
"IOMMU" "Jiho Han" IEEE
"IOMMU" "Won Hur" pdf -site:researchgate.net
"IOMMU" "Shuntong Zhao" -site:scribd.com -site:patents.google.com
"Fast & Safe IO Memory Protection" pdf
"IOTLB-SC" site:arxiv.org/abs
"Sumeru" "cache-optimal" 2026
"Rethinking IOMMU for Future IO Devices"
"IOTLB-SC" arxiv 2202.11623
"RISC-V" "IOMMU" "Journal of Hunan" 2024
"基于RISC-V的IOMMU" 2024 湖南大学
"Design of IOMMU Based on RISC-V" 2024
"Sumeru" "Towards Understanding" author
"FBI" "IOMMU TLB Invalidations" 2025
~~~

#### 主会话补充机制／会场检索（13条）

~~~text
"Beyond the Bermuda Triangle of Contention" RTCSA 2025 doi
"IOMMU" "head-of-line" interference DMA
"IOTLB" "hit" "blocking" contention
("IOMMU" OR "address translation") ISPASS 2024 2025 2026
("IOMMU" OR "address translation") IISWC 2024 2025 2026
("IOMMU" OR "address translation") CARRV 2024 2025 2026
("IOMMU" OR "address translation") DAC 2024 2025 2026
site:ispass.org IOMMU 2024 2025 2026
site:iiswc.org IOMMU translation 2024 2025 2026
site:carrv.github.io IOMMU
site:dac.com IOMMU RISC-V 2024 2025 2026
"IOMMU" "RTAS" "2024" "2025" "2026" -PowerPC -firmware
"IOMMU" "Real-Time and Embedded Technology and Applications" contention
~~~

### 8.5 本次定向续查：公开全文获取记录（2026-10-17，JST）

本次只续查Procedia与Hur/Ro的正文及可确定其并发方法的公开材料。新增66条外部网页索引查询（Procedia28、Hur/Ro28、主会话10，重复保留），不是66次数据库站内检索，也不重写§8.4的既有记录。中文／韩文人名变体仅为检索字符串，不据猜测确认身份。所有新来源均按实际取得内容降级；没有取得这两篇的正文、原始PDF或可核实验工件，没有新增“全文已读”标记。

| 实际公开渠道／入口 | 本次获取结果与阅读范围 | 可用于本次裁决的边界 |
|---|---|---|
| [ScienceDirect目标页](https://www.sciencedirect.com/science/article/pii/S1877050925024263)、同PII的/pdf路径及via=ihub变体 | 普通请求403，返回访问拦截HTML，不是论文；网页工具也403 | 没有完成浏览器访问：Playwright依赖npx缺失，仅执行可用性检查。访问受阻不能证明未公开 |
| [Elsevier原生PII接口](https://api.elsevier.com/content/article/pii/S1877050925024263?httpAccept=text%2Fxml)、[DOI FULL接口](https://api.elsevier.com/content/article/doi/10.1016/j.procs.2025.08.115?view=FULL&httpAccept=text%2Fxml) | 前者200、1951字节，仅coredata；后者401；text/plain返回400；ScienceDirect中国域代理连接失败 | 开放许可已核，正文未得；不把题录XML写成全文 |
| [Procedia OpenAlex位置](https://api.openalex.org/works/https://doi.org/10.1016/j.procs.2025.08.115)、[S2 DOI接口](https://api.semanticscholar.org/graph/v1/paper/DOI:10.1016/j.procs.2025.08.115?fields=title,authors,openAccessPdf,url,externalIds) | OpenAlex200，仅DOI落地页、无PDF URL；S2 404 | 仅为当前索引位置；不是无作者稿的证明 |
| [ITQM2025 Program页](https://itqm-meeting.org/2025/program.html)、[官方HTTP Program](http://itqm-meeting.org/2025/files/ITQM2025%20Program.pdf) | HTTPS证书错误／网页工具502；正常官网HTTP取得Program。已提取全文，检索IOMMU／Shuntong／Mingsen等未匹配 | Program不是proceedings全文，未匹配不否定发表。三个解析器均34页但ARS对象偏移告警，状态UNAVAILABLE；不用其页锚支持论文结论 |
| [openEuler回顾](https://www.openeuler.org/zh/news/openEuler/20241220-meetuop/20241220-meetuop.html)“赵顺通”段／材料末段；[公开材料仓库](https://gitee.com/openeuler/presentations/tree/master/meetup) | 回顾正文已读，追查其讲稿；Gitee页面／公开API超时。[官方GitHub镜像目录](https://github.com/openeuler-mirror/presentations/tree/master/meetup)仅查到2024年8月材料，未找到目标报告 | 回顾是讲稿线索，不是Procedia方法；镜像可能滞后，不认定Gitee无稿 |
| [RVSC2024公开材料](https://github.com/cnrv/RVSC2024)、[郑律X100讲稿](https://github.com/cnrv/RVSC2024/blob/main/slides/0822A/029-%E9%83%91%E5%BE%8B.pdf) | 刘亚南相关报告Slides／Video为TBA；另取得郑律讲稿，子任务局部读PDF12–18页，主会话渲染核第13页；19/19/19页ARS PASS | 原图仅归此讲稿，不当Procedia全文／同版图，也不据其缺少干扰矩阵排除Procedia |
| [Scribd目标页](https://www.scribd.com/document/1046293233/基於RISC-V的資料中心級IOMMU性能優化及研究)／外部索引，[Procedia ResearchGate题录](https://www.researchgate.net/publication/394841384_Performance_optimization_and_research_of_data_center-level_IOMMU_based_on_RISC-V) | Scribd普通请求200但只得描述、13页占位及订阅入口；网页工具超时，索引露出§5.3／Table 1提示。RG为request-only | 没有获得可逐页核读的正文或原表，不从受限页面重建全文，不采纳索引参数作科学事实；未向作者发请求 |
| [Hur/Ro作者publications](http://escal.yonsei.ac.kr/publications.html)、[project](http://escal.yonsei.ac.kr/project.html)、[member](http://escal.yonsei.ac.kr/member.html)、[机构题录](https://yonsei.elsevierpure.com/en/publications/enhancing-iommu-efficiency-in-heterogeneous-sacs-a-study-on-cache/) | 公开HTTP页面200，目标列表条目、项目关联及完整机构Abstract已读；列表无有效PDF／slides；此前HTTPS失败不能再写成整站不可访问 | 目标条目中的IEEE9138941仅为注释掉的重复占位，不是目标10879735；项目／成员背景不证明论文平台 |
| [IEEE标准PDF按钮入口](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=10879735) | 普通请求418、15410字节HTML；网页工具不可读；未取得PDF | 不记全文，不据失败认定论文只有摘要 |
| [Hur/Ro Crossref](https://api.crossref.org/works/10.1109/ICEIC64972.2025.10879735)、[OpenAlex](https://api.openalex.org/works/https://doi.org/10.1109/ICEIC64972.2025.10879735)、[OpenAIRE](https://api.openaire.eu/search/publications?doi=10.1109/ICEIC64972.2025.10879735&format=json) | 均200。Crossref给17条reference；OpenAlex closed／无PDF；OpenAIRE只有DOI／dblp元数据入口，无新增稿件。Crossref另有similarity-checking用途链接，未作为一般公开稿获取 | 元数据不代替正文；来源页码1–4不等于已核PDF四页；引用Arm／模拟器不能推出方法 |
| [GitHub仓库搜索IOMMU Yonsei](https://api.github.com/search/repositories?q=IOMMU+Yonsei)、[IOMMU Hur](https://api.github.com/search/repositories?q=IOMMU+Hur)；[同组个人公开页](https://jiwon24lee.github.io/) | 两个原生查询均200、total_count=0；个人页未见目标稿／工件 | 搜索仅覆盖匹配的仓库元数据，不是全GitHub内容排除；未登录私有库或联系作者 |
| Hur/Ro的DOI直达、lab HTTPS index／member、Ro简历WonWooRo_Resume.pdf、[ResearchGate题录](https://www.researchgate.net/publication/389108151_Enhancing_IOMMU_Efficiency_in_Heterogeneous_SaCs_A_Study_on_Cache_Policy_Impacts) | 前几项网页工具不可读；lab project与publications的工具读取／find也失败，后由普通HTTP成功补读；RG索引显示No full-text available／request-only | 工具find失败不代表页面没有目标；列表目标与2024 ARM条目均已从实际HTML核到，但都未得正文 |

**新增辅助PDF身份。** X100讲稿SHA256 e7101aad13d96d0e7bf4be8a6e742d214ea637a093502748a3ae259f1b82404a；ITQM Program SHA256 dd15b258f9034db030496fbdf94d07048529e3115d524c20e7722703419cee23。前者PASS、后者UNAVAILABLE均为本次实际ARS检查；它们都不是Procedia或Hur/Ro原文，不能给两篇借用结构通过或全文阅读标记。文档已自含新查询、原件入口、接受／未决状态和必要哈希，不依赖未提交的临时收据。

#### Procedia作者／会议／公开稿查询（28条）

~~~text
"10.1016/j.procs.2025.08.115" pdf -site:scribd.com -site:patents.google.com -site:researchgate.net
"Performance optimization and research of data center-level IOMMU" proceedings
ITQM 2025 proceedings "IOMMU"
"Shuntong Zhao" IOMMU "China Mobile"
ITQM 2025 "Procedia" "266" proceedings
ITQM 2025 IOMMU Zhao paper PDF China Mobile Suzhou
"IOMMU" "洪铭森"
"IOMMU" "赵顺通"
"面向云数据中心的RISC-V IOMMU" pdf
"赵顺通" "IOMMU" pdf
"ITQM 2025" "program" pdf
"Performance optimization" "IOMMU" "pdf" "Zhao" -site:scribd.com -site:researchgate.net
"IOMMU" site:gitee.com/openeuler/presentations
"赵顺通" "面向" "云" filetype:pdf
"云数据中心" "IOMMU" "ppt"
"ITQM2025" "IOMMU" "Zhao"
"赵顺通" RISC-V IOMMU 讲稿
"面向云数据中心" "IOMMU" filetype:pdf
"赵顺通" "洪" IOMMU
"IOMMU" "Shuntong" "pdf" -site:researchgate.net -site:scribd.com -site:patents.google.com
"IOMMU" "赵顺通" "移动云"
"data center-level IOMMU" author manuscript repository
"S1877050925024263" pdf download
"IOMMU" "923–935" -site:scribd.com -site:sciencedirect.com -site:patents.google.com
"基于RISC-V" "IOMMU" "性能优化" "赵"
"IOMMU" "赵顺通" site:10086.cn
"IOATS" "赵" "洪" RISC-V
"云数据中心" "IOMMU" filetype:pdf -site:scribd.com
~~~

#### Hur/Ro作者／机构／公开稿查询（28条）

~~~text
"Enhancing IOMMU Efficiency" pdf
"Won Hur" "Won Woo Ro" IOMMU
"10879735" "pdf"
"Enhancing IOMMU" "SoCs"
"Won Hur" Yonsei computer architecture website
"Won Woo Ro" laboratory publications IOMMU
"10879735" -site:researchgate.net -site:proceedings.com -site:ftp.math.utah.edu
"Enhancing IOMMU Efficiency" -site:researchgate.net -site:yonsei.elsevierpure.com
site:escal.yonsei.ac.kr "Enhancing"
site:yonsei.ac.kr "Cache Policy Impacts"
"Won Hur" "IOMMU" "ARM"
"Enhancing IOMMU Efficiency" "pdf" -site:researchgate.net
"Enhancing IOMMU Efficiency" download
"Enhancing IOMMU Efficiency" 허원
"Won Hur" IOMMU github.io
"10879735" IEEE author pdf yonsei
"A Study on Cache Policy Impacts" -site:researchgate.net -site:yonsei.elsevierpure.com
"ICEIC" "IOMMU" "ChampSim"
"Hur" "IOMMU" "cache" "github"
"허원" "IOMMU"
"Enhancing IOMMU Efficiency in Heterogeneous SoCs"
"Enhancing IOMMU" "허"
"ICEIC 2025" "proceedings" "pdf" "cache"
"Won Hur" "IOMMU" repository OR artifact OR simulation
"Performance Implication of Hashed Page Tables" pdf -site:researchgate.net
"Enhancing IOMMU Efficiency" "code"
"Enhancing IOMMU Efficiency" "simulation"
"IOMMU" "Hur" site:github.com
~~~

#### 主会话出版商／全文定位查询（10条，重复保留）

~~~text
"S1877050925024263" pdf
"Performance optimization and research" "IOMMU" pdf
"10.1016/j.procs.2025.08.115" pdf
"S1877050925024263" "pdf.sciencedirectassets"
"Performance optimization and research of data center-level IOMMU" filetype:pdf
"Performance optimization and research" "5.2" IOMMU
"Performance optimization and research of data center-level IOMMU" "IOATC_NUM"
"S1877050925024263" "pdf"
"IOMMU" "IOATC_NUM=4"
"Performance optimization and research of data center-level IOMMU" "5.2."
~~~

<a id="open-items"></a>
## 9. 未决项与裁决适用范围

1. **本轮续查后仍未决：** Procedia2025的独立DMA流、共享IOATS细部、实际并发供给与争用对照；Hur/Ro2025的ISA、独立设备／供给及缓存策略实验矩阵（§5.3）。新增渠道没有取得两篇正文，不能写成已全文排除；G2保持待补核，不给无条件通过。解除这两项未决需可核原文的方法与结果，不能由访问失败、他文结构或参考文献代替。中文2024设计、YArch提案仍缺可验全文；这些是具名缺口，不是无限新增文献门槛。
2. 硕论Fig.6.31/32实际源码／配置unknown；公开bench串行有代码依据，不能替代论文所有运行。其同平台性能评价和共享争用认识必须引用。
3. Bermuda Fig.8单位／频率／timer／重复数与出版版差异未解；DATE正式正文是否修复<2%及表格冲突未解。只引用实际读过的版本及一致结果。
4. Wheel、HeroSDK、Penguin／2024验证talk等按阅读级别保留；缺全文不是排除依据。数据库访问限制尤其限制负面检索结论。
5. G2只涵盖本次查询与原文；不保证投稿前无新稿，不替代数据验收或构成录用预测。新统一基线按它自己的证据判断，本任务不恢复机制门或新增实验。
6. 本轮只核读论文／公开固定源码、检查历史报告并整理本文；无构建、仿真、上板或前作工件重跑，未修改其他仓库文件／历史原件。

**AI辅助说明。** 本轮检索、核读、比较与整理由Codex主会话及有界Codex子任务完成；主会话直接核决定性原文、关键公开源码与引用的历史结果报告，负责接受判断和文档检查。未调用DeepSeek或Claude。历史实验作者／执行归因保留在各原报告；不将模型一致或用户已读标记当正确性证明。

**既有投稿口径附记。** 摘要2026-12-11 AoE、全文12-18 AoE，正文9页、IEEE双栏、双盲；AoE转JST分别为12-12、12-19的20:59。按[投稿页](https://ispass.org/2027/submission/)、[日期页](https://ispass.org/2027/dates/)、[CFP](https://ispass.org/2027/cfp/)最终要求执行。关联前作正常第三人称引用，实质AI使用按实际纳入内容披露；本文是含内部路径的工作笔记，不能原样充当匿名稿。
