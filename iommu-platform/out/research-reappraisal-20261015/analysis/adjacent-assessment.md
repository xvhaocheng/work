# 控制位置分析的邻近前作

2026年10月15日。七篇共享翻译核心论文不足以单独裁定分析方法的新意，因为在途阻塞、流水时延和接口调节本来就是实时互连研究对象。为候选A定向补核以下三项，不另起广域文献地图。

## 层次AXI的响应时间分析

Francesco Restuccia等，[Bounding Memory Access Times in Multi-Accelerator Architectures on FPGA SoCs](https://retis.sssup.it/~giorgio/paps/2023/TC23.pdf)，IEEE TC 72(1)，2023，pp154–167，DOI 10.1109/TC.2022.3214117。本轮核作者PDF的§2及§3关键分析：PDF第2–4页说明任务、互连及内存模型，§3.6–3.7在第7–8页给干扰传播与响应时间算法。全文保存在[PDF](adjacent/axi-rtanalysis.pdf)及[文本](adjacent/axi-rtanalysis.txt)。

该工作已经联合层次互连、流水、每任务在途上限和完整硬件任务响应时间，且明确采用内存控制器顺序返回的模型。不能把考虑旧在途请求、层次或任务级完成作为本课题首次贡献。其§2.1把读写传播视为独立，§2.2假定流水不满，§2.3用经文档或profiling取得的PS/MC延迟参数，§3分析周期任务的事务预算。它不是简单“忽略内存控制器”，也不因未单列IOMMU就能被判为不可用。

候选A需要实质增加的部分是：将依赖PTE返回产生的后续请求、翻译完成释放的数据流量，以及与DMA所有权/有限buffer有关的阻塞纳入一个可核的控制干预合同；检验其是否为现有分析的直接实例、保守扩展或有意义的新能力。当前只核上述关键章节，未逐条复证所有引理，不宣称已排除其扩展。

## 可随机选择缓冲事务的实时互连

Zhe Jiang、Kecheng Yang、Nathan Fisher、Ian Gray、Neil Audsley、Zheng Dong，[AXI-ICRT Towards a Real-Time AXI-Interconnect for Highly Integrated SoCs](https://userweb.cs.txstate.edu/~k_y47/webpage/pubs/tc23c.pdf)。本地保存[作者稿](adjacent/axi-icrt.pdf)与[文本](adjacent/axi-icrt.txt)；本轮不从URL文件名推断出版年份。直接核设计目标、§4架构与§6调度分析，未对完整实现及全部评估作重新验收。

它把FIFO中的physical priority inversion作为已有问题，提供随机访问缓冲事务的微架构及两层可组合调度，§6使用周期服务器与supply/demand bound做可调度性分析。故“FIFO阻挡优先级”“可组合调度”和“把队列改为可选择”都不是新的基本思想。其可修改缓冲和调度结构是实付能力，不代表固定MIG外部也能重排已接受读。

候选A的区别应落在固定接口下哪些控制仍可用、何时需要付费迁移控制位置，以及翻译反馈怎样改变完整任务；不能仅把可选择队列换名后与FIFO弱基线比较。对其架构的等价实现、完整逻辑/物理成本比较尚未完成。

## 边界流量调节与事务生命周期

[AXI-REALM Safe Modular and Lightweight Traffic Monitoring and Regulation for Heterogeneous Mixed-Criticality Systems](https://arxiv.org/html/2501.10161v1)，固定arXiv v1；本轮保存[PDF](adjacent/axi-realm.pdf)及[文本](adjacent/axi-realm.txt)。直接核§III-A的burst splitter、write buffer及预算调节，§III-B/C的适用条件和分阶段监控，并核§IV–V的成本/评价口径。

它已实现外置的burst fragmentation、按区域预算调节、先收足片段W再发布AW，以及按事务阶段/先前burst长度调节超时预算。设计有实际成本评估，并以互连进度和RR等条件为前提。因此不能声称所有前作必须改MC，不能把提前发布AW会占W资源、外置门控、细粒度分段或按既有债务计费单列新颖性。

需要继续比较的是：它的普通调节能力用于共享IOMMU和依赖PTE流量时已经足够到哪里；若DMA需要先获得目的许可才能安全读源，集成位置和必要排空读是否改变其使用条件与完整费用。本文并未据缺少IOMMU专节认定这些问题无人研究。

## 对候选A的修订

把原先过宽的“不可修改MC时需要外置保护”收窄：普通外置调节已经存在，新的方法必须解释并预测共享翻译产生的反馈、控制权及完整任务后果，给出优于直接应用既有方法的认识、能力或成本点。DAG最长路、carry-in阻塞和service-curve工具作为标准构件使用。七篇核心论文和本补充均未证明该特定方法已经完整发表，也未证明它具有足够独立贡献；下一步须用准确方法比较和前瞻结果判别。

下载URL、字节数与SHA256保存在[sources.json](adjacent/sources.json)。本轮网页工具对AXI-ICRT因文件过大读取失败，随后从同一作者原始地址取得完整PDF；RTAS25网页访问超时没有被记为成功全文阅读，也不参与本轮新增覆盖结论。
