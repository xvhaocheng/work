# 有界剩余覆盖/收敛审计（第 3 轮，只做 routine 覆盖；不重做 20 项、不扩表）

## 0. 一句话结论
以"**还有什么会改变研究决策**"为准则，剩余优先缺口 **8 类**，全部集中在**真实 CPU-GPU-NIC 流水线证据、GPU 驱动 RDMA/存储、MR 注册/撤销、ATS 失效语义、任务运行时、翻译测量/sim-to-real、应用存在性**。**没有任何一类足以判定候选已成立**；两类（1、8）缺的是"真实系统/真实应用是否真的存在该状态"，缺证据 ≠ 创新证明。其余为**全库未见**或**只查到题录**。无法浏览，BN 新题录一律不新增。

---

## 1. 四桶覆盖状态（先分清证据等级）

| 桶 | 判定 | 证据 |
|---|---|---|
| **已收缺全文** | 主目录 19 项 `fulltext_available=false` | catalog.csv `fulltext_available` 列；集中于 activeforward2018(L3)、cato2026(L11)、cdfd2026(L12)、clover2026(L15)、cryptommu2023(L20)、ctl2026(L21)、helm2025(L44)、lamp2025(L55)、latpc2025(L56)、libra2026(L59)、maverick2026(L62)、nbwalker2026(L69)、ontable2017(L77)、sagepte2026(L89)、shadowupdate2026(L90)、shared1993(L91)、snakebyte2023(L97)、synergistic2010(L108)、transfetch2026(L110)。与 gap doc L41 "主目录仍有19项"一致 |
| **已下载尚未深读** | 20 项中已取全文但 `read_depth=primary_metadata_or_abstract_…pending` | verified-gaps.json: asm2015(L33)、mise2013(L48)、parbs2008(L63)、rmm2015(L78)、twizzler2020(L93)、mind2021(L108)、tinylfu2015(L123)、leases1989(L138)、deadentry2026(L168)、virtuoso2025(L183)、atlas2010(L228)；另 cats2015(L243)为 `metadata_plus_related_author_talk` |
| **全库未见** | 123 项 + 20 项均无 | GPUDirect RDMA/Storage、MR cache/rcache 协议、ATS.INVAL 专文、Binoculars 等翻译观测、GPU 任务运行时翻译集成、IOMMU sim-to-real 标定（见 §3、§4） |
| **未验证应用存在** | 候选 1–4 均为假设 | translation-opportunities-20261006.md L3、L11、L25、L37、L51；旧地图 L186 "平台不能直接支持…真实多 GPU UVM 等结论" |

---

## 2. 最多 8 类优先缺口（每类：现有覆盖 / 欠缺 / 应核 / 完成条件）

### 类 1｜真实 CPU-GPU-NIC 三设备端到端流水线证据
- **现有**：io 线 DAMN(L24)、F&S(L33)、IOMMU Scalably(L51)、IOTLB wall(L53)、ODRP(L76)、rIOMMU(L87)、vIOMMU(L120)、TeRM(L109)；gpu 线 MASK(L61)、walk_scheduling(L123)、walk_stealing(L124)、STAR(L105)。旧地图 L28/L30 分述 I/O 与 GPU 共享。
- **欠缺**：**没有任何一篇把 CPU+GPU+NIC 同一流水线的翻译干扰一起测**；io 线是纯 I/O，gpu 线是纯 GPU/多 GPU。
- **应核**：是否存在把翻译竞争放在三设备流水线上的真实系统工作。
- **完成条件**：取得≥1 篇/生产报告显示该流水线存在翻译竞争；否则记为全库未见，候选 1 保持假设。

### 类 2｜GPU 驱动发起的 RDMA/存储网络应用
- **现有**：rdmaodp2021(L85)、odrp2025(L76)、term2024(L109)、bypassd2024(L9)、iotlb_wall2023(L53)——均为 **CPU 侧发起**。
- **欠缺**：GPU/加速器驱动发起的 RDMA（GPUDirect RDMA）或存储（GPUDirect Storage/可组合存储）路径全库未见；旧地图 **L188 明确"RDMA 注册缓存/撤销"仍未覆盖**。
- **应核**：GPU 发起路径上的翻译/失效成本公开工作。
- **完成条件**：找到或**明确记录缺失**（缺失不等于前作无覆盖）。

### 类 3｜内存注册/注销与 MR cache 生命周期
- **现有**：rdmaodp2021(L85) 覆盖 InfiniBand ODP（按需注册）；通用副本有效期/准入为 leases1989(verified-gaps L138)、tinylfu2015(L123)、mind2021(L108)。
- **欠缺**：显式注册（`ibv_reg_mr`）与**注册缓存（rcache）失效协议**全库未见；旧地图 L188 列为开放范围。
- **应核**：MR 撤销与 IOMMU 撤权语义的关系；注册缓存失效判定。
- **完成条件**：≥1 篇或权威文档覆盖 MR cache 失效；否则标注未覆盖范围。

### 类 4｜ATS/PRI/PASID 失效语义与设备侧翻译缓存
- **现有**：官方 RISC-V IOMMU 队列（旧地图 L134，**非论文**）；iommu_scalable2015(L51) 延迟/per-core 失效；riommu2015(L87) burst 失效；site2017(L95) 自失效；hatric2017(L42)/latr2018(L57)/UNITD2010(L114) 翻译一致性；didi2011(L26)/ptlbmalloc2020(L83)/shootdown1989(L94)/shoot4u2016(L93)/fpr2024(L34) shootdown；riscvsva2025(L88) RISC-V IOMMU SVA 评估。
- **欠缺**：**无专文测 PCIe ATS.INVAL / PASID-scoped 失效成本与完成语义**；device TLB 失效确认无独立测量。须区分 **IOMMU IOTLB 失效** vs **device ATS cache 失效**。
- **应核**：PCIe ATS 规范、ATS.INVAL 完成语义、device TLB invalidate 延迟测量。
- **完成条件**：取得 ATS 聚焦论文或规范级测量计划。

### 类 5｜跨设备同步与任务运行时的翻译感知调度
- **现有**：catalog walk_scheduling2018(L123)、walk_stealing2021(L124)；verified-gaps BIS(L17)、Criticality Stacks(L213)、CATS(L243)、PAR-BS(L63)、ATLAS(L228)；旧地图 GPS(L103)/WIC(L106) 但**不在 123 项**。
- **欠缺**：**无 GPU 任务运行时（CUDA stream/graph、HIP、MPS/MIG 调度器）与翻译调度的集成工作**；20 项的关键性/运行时工作是 CPU/通用。
- **应核**：CUDA graph/stream-ordered 与翻译服务顺序；MPS/MIG 下翻译共享（STAR 只覆盖 MIG TLB）。
- **完成条件**：找到运行时依赖信息驱动翻译调度的工作，或写明候选 1 的新信息不能由现有运行时廉价获得。

### 类 6｜翻译性能测量与 HEC 观测归因（Binoculars 等）
- **现有**：counterpoint2026 全文（counterpoint2026.txt L1–1223；案例 Haswell MMU）；verified-gaps coz2015(L2)、mise2013(L47)、asm2015(L32)；旧地图微基准 iotlb_bottleneck2010(L67 §A)。
- **欠缺（全库未见，来源=CounterPoint 参考文献，非网上新检索）**：见 §3。
- **应核**：Binoculars（page walker contention 的 HEC 刻画）作为候选 3 的直接测量前作。
- **完成条件**：把 Binoculars + 1–2 项观测工作纳入前作矩阵，定义候选 3 的观测集合/噪声/可判别对象/动作收益。

### 类 7｜sim-to-real 标定缺口
- **现有**：旧地图 L56 区分真机/仿真；IDYLL(L47) 真机剖析+MGPUSim；ObservUVM(L75) 单 GPU 真机；Revelator v2(L86) Linux 原型。
- **欠缺**：**无针对本平台（CVA6+RISC-V IOMMU）或通用 IOMMU 的仿真模型↔真机标定**；CounterPoint 是"模型 vs 计数器一致性"检查，**不等于 sim-to-real 标定**。
- **应核**：IOMMU/翻译仿真器标定（如 gem5 SMMU invalidation 延迟，CounterPoint 引 [39]）。
- **完成条件**：一篇标定研究，或记录"全库未见标定"。

### 类 8｜候选假设所需真实应用存在性证据
- **现有**：候选 1–4 均为假设（translation-opportunities L3/L11/L25/L37/L51）；旧地图 L186 平台适用边界；**未执行任何轨迹采集**。
- **欠缺**：无证据证明真实应用存在"翻译主导的跨设备依赖"（候选 1）、"高复用+高更新"同时出现（候选 2）、"相同 TLB 统计需相反动作"（候选 3）。
- **应核**：真实 GPU-NIC-storage 流水线 profile/trace。
- **完成条件**：≥1 条轨迹显示相应状态；否则候选降级为待验证。

---

## 3. 从 CounterPoint 参考文献可查的"翻译观测"先前工作（**非新遗漏，题录未联网核实**）
以下均出现在 counterpoint2026.txt 参考文献，**123 项与 20 项均无**，属"全库未见"线索，出版状态以原文引用为准：

| 短称 | CounterPoint.txt 行 | 说明 |
|---|---|---|
| Binoculars | L929、L1215–1218 | Zhao/Morrison/Fletcher/Torrellas, USENIX Security 2022；用 HEC 刻画 page walker contention |
| Lindsay & Bhattacharjee IISWC'24 | L1075–1078 | Understanding Address Translation Scaling via HECs |
| Ertmer/Dumitru/Yarom 2025 | L1052–1053 | Reverse-Engineering the Address Translation Caches |
| Peek-a-Walk | L1216–1219 | Page walk 侧信道 |
| TLB;DR | L1185–1188 | TLB desynchronized reverse engineering |
| van Schaik et al. 2017 | L1203–1206 | 侧信道反推页表缓存 |
| Merri field & Taheri 2016 | L1138–1141 | EPT 性能影响 |

另 CounterPoint 自身发现可直接用于候选 3 的观测建模：walk 合并（L807–824）、aborted walks（L841–854）、walk replay（L836–854）、root-level MMU cache（L825–839）。

## 4. ≤6 条记忆型建议（**明确未联网核实，不得写成已核新遗漏**）
1. GPUDirect RDMA / GPUDirect Storage（NVIDIA，未核）——类 2。
2. UCX/UCP memory registration cache（rcache，未核）——类 3。
3. NCCL / NVSHMEM 注册与跨设备同步（未核）——类 5。
4. PCIe ATS 规范与 ATS.INVAL / PASID-scoped 语义（未核）——类 4。
5. Linux iommufd/VFIO 设备直通撤权路径（未核）——类 4。
6. AMD HSA/ROCm IOMMUv2 或 APU ATS（未核）——类 1/4。

---

## 5. 停止广撒网 / 进入 1–2 具体问题强前作矩阵的门槛
- **可进入矩阵**当且仅当：① 类 1 或类 8 取得**真实系统/真实轨迹**显示翻译主导状态；② 候选的"新信息"**无法由现有运行时（BIS/CATS/CounterPoint）廉价获得**；③ 强前作已**取得全文（非标题）**并在**相同约束**下比较。
- **不因此判创新**：两轮没搜到 **≠** 创新证明；缺全文 **≠** 前作未覆盖；**不得**新增"无插桩"之类硬约束规避前作；**不**把 ATLAS 文件名 `HPCA16_ATLAS.pdf` 当届次冲突——**ATLAS 为 HPCA 2010**（verified-gaps L230），文件名非年份。

## 6. 边界声明
本轮只做覆盖审计，未修改文件、未运行命令。创新/架构判断归 Codex。20 项表未重生成。所有"未核"建议均已标注，未写成已核。