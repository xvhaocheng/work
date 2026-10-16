# Supporting Address Translation for Accelerator-Centric Architectures

稳定ID：`accelerator_translation2017` · 类型：`research_paper` · 主题：`accelerators`

作者：Jason Cong; Zhenman Fang; Yuchen Hao; Glenn Reinman；年份：2017；出处：IEEE International Symposium on High Performance Computer Architecture (HPCA)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：私有小TLB加共享L2 TLB，并将页表遍历卸载到宿主核MMU，以减少页表遍历次数和延迟。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：加速器与宿主核处于同一应用虚拟地址空间/进程ID匹配；TLB shootdown和page fault可支持；使用4KB页进行主评估；加速器访存规则、批量、适合数据tiling；以IOMMU作为正确性和安全基线

限制：PARADE/gem5全系统仿真，无真实硬件；未评估不可信加速器间权限隔离；未讨论A/D位；宿主页表遍历依赖宿主MMU端口和请求队列，可能成为瓶颈；大页、TLB-aware tiling、banked shared TLB留作未来工作

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：accelerators主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf)
- [来源2](https://doi.org/10.1109/HPCA.2017.19)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/accelerator_translation2017.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.pdf) · SHA-256 `ec6e10d718e228c16ec211d4509bd56c9d9010d577c88885fe588c6141d15964`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:3](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt:104](../../iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt:135](../../iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt:499](../../iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt:604](../../iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt:725](../../iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/dvm2018.txt:723](../../iommu-platform/out/shared-translation-literature-20261006/papers/dvm2018.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-literature-review-20261004.md:19](../../iommu-platform/docs/research-literature-review-20261004.md)：

| Supporting Address Translation for Accelerator-Centric Architectures（Cong, Fang, Hao, Reinman；2017；HPCA） | https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf | `out/method88-literature-20261004/web-evidence.json`；原文 pp.5-7（L569-L576、L600-L654、L638-L640、L660-L744） | 16-32 entry、LRU 的 private TLB 减 30.4% page walk、性能从 12.3% 升至 23.3%（P5 L588-L598）；512-entry shared L2 TLB（mostly-inclusive、LRU）累计减 76.8%，infinite private TLB 下仍减 41.7%（P6 L629-L713）；**private TLB 与 shared L2 TLB 都明确支持 non-blocking hit-under-miss**（P5 L569-L576；P6 L638-L640） | 面向 accelerator-centric 的仿真研究，IOTLB-only 作 baseline；不是共享 IOMMU 上多 DMA 流的实时保护，未涉及准入计费 | 重叠：hit-under-miss、private/shared TLB、page walk 削减。区别：让 hit 越过 miss 不是新基本思想；本课题只可能落在保护位置/容量与顺序边界的设计取舍 |

[iommu-platform/docs/translation-continuation-frontier-20261008.md:15](../../iommu-platform/docs/translation-continuation-frontier-20261008.md)：

- **最强前作**：Cong 等, HPCA 2017, DOI 10.1109/HPCA.2017.19（[作者 PDF](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf)）：不同步加速器的翻译复用、共享 TLB 过滤重复 miss、hit-under-miss、结果路由；官方 gem5 coalescer 固定 revision `392c1ced53827198652f5eda58e1874246b024f4`（committer 日期 2020-09-10），[tlb_coalescer.cc](https://gem5.googlesource.com/public/gem5/+/392c1ced53827198652f5eda58e1874246b024f4/src/gpu-compute/tlb_coalescer.cc) cc 95–124 / 140–211 / 211–222。

[iommu-platform/docs/research-content-map-20261015.md:106](../../iommu-platform/docs/research-content-map-20261015.md)：

| [Accelerator-Centric Translation 2017](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf) <!--ref:accelerator_translation2017--><!--anchor:section:IV-B-IV-D--> | 分级 TLB、hit-under-miss、host PWC/cache，§IV-B—D | 普通缓存、驻留和复用必须获得公平机会 |

[iommu-platform/docs/paper-logic.md:173](../../iommu-platform/docs/paper-logic.md)：

- [Supporting Address Translation for Accelerator-Centric Architectures (HPCA 2017)](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf)：第 IV-A/IV-B 明确支持私有/共享 TLB 的 non-blocking hit-under-miss。因此“让 hit 绕过 miss”不是新的基本思想。

[返回总目录](../README.md)
