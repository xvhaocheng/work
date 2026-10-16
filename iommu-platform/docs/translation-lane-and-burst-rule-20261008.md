# 翻译返回对照与新准入规则审定（2026-10-08，§29）

用户要求创新。本轮将“专用翻译线路”降为已有方法对照；Codex独立提出动态源读分段G和条件进展预留H，并核原文、真实连接和最强简单规则。**目前没有证实G/H是独立创新，不进入生产RTL或BIT构建。** 可用普通原语实现不自动否定论文贡献，但不同术语、不同决定和没有搜到同款论文也都不是创新证明。

## 1. 原平台在哪里共享

生产四iDMA经`ariane_peripherals_xilinx.sv:1228–1252`的mux进入IOMMU。数据由comp口返回系统互连（:1385–1387、:1451–1456）；DS经单独延迟器（:1361–1380）进入另一发起端口。顶层`ariane_xilinx.sv:850–851`已有独立comp/DS发起端口，最终仍汇入同一个DRAM目标、时钟转换器及MIG接口（:934–946、:1006–1090）。发起端口不同，不等于物理DDR带宽不同；单R端口也不自动意味着MIG必须采用当前行为模型的FIFO顺序。

`rv_iommu_ds_if.sv:81–109、227–273`在DS内部合并PTW/DDTW等并共用一个返回级。**DS返回与DMA payload共享后级DDR服务，但没有共用本研究的payload byte-credit池。** MaxSlvTrans/MaxMstTrans=8是事务上限，不能当作八份完整payload储存。

测试当前是两个真实iDMA、两bank；合成CPU在slv0、debug闲置slv1、comp/slv2、DS/slv3。DMA配置直连寄存器，不是第四路DDR负载。生产四DMA不能据此写成已跑四真实DMA实验。

## 2. 仅两次新增运行：旧优先级是否已经够用

复用[冻结命令与日志](../out/translation-lane-check-20261008/README.md)，旧二进制未重编。相对§26 serial仅把`+ds-priority=0`改为1，CSV输出路径改到本轮目录。64-bit同一R口最多一拍/周期，共享队列上限16不变，无第二DDR端口。既有C++仅在未保持RVALID时把最老DS移到队头；同ID顺序保留，既已VALID的数据不改。

两项exit0/PASS，暖场与各自旧对照逐字相同，V/A各64次4KiB，DMA数据AR各256、DS完成各93；PN FAST62、P FAST0，字节和终止排空检查通过。

| DDR行为模型 | P cycles | PN cycles | PN−P |
|---|---:|---:|---:|
| §26原FIFO/队头串行准备 | 92109 | 92374 | +265（0.288%） |
| 本轮DS返回优先 | 88989 | 91547 | +2558（2.875%） |

优先级使P耗时降低3.387%、PN降低0.895%。本轮两者DS从DDR AR接受到首R的均值/最大值都32 cycles，A PTW-active都13489。**这个局部延迟相同后PN仍慢，说明该模型中的DDR页表排队差不是唯一因素；不等于全过程翻译时间、准备时序已相同，更不是新方法失败/成功或实板结论。** DS额外50/50固定延迟仍存在。

本轮互斥读服务账本PN−P：empty +1312、setup +992、transfer +256、backpressure −2，总计2558。合成CPU读221/253，多32笔，每笔8拍与31准备周期，恰好对应transfer/setup差额；empty差也等于32×41，但**没有做因果干预，不能说CPU造成全部差额**。CPU是等待一次响应结束后才发下一次的背景流（system_top :660–685），不是实际Linux轮询。相同DMA工作量不表示整个DDR工作量相同；不得据此认定内存带宽饱和。

## 3. 原文已经覆盖什么

| 强前作／原始来源 | 已有机制与本轮限制 |
|---|---|
| [AMD IOMMU规范3.09](https://www.amd.com/content/dam/amd/en/documents/processor-tech-docs/specifications/48882_IOMMU.pdf)，§2.1 | 表访问使用专用等时VC避免依赖死锁；VC不是独立物理DDR。不能把“翻译单独一路”当首创。 |
| [MASK，ASPLOS2018](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)，§5.4 | 翻译Golden队列优先于数据Silver/Normal，还按并发walk/阻塞warp调节配额。单纯翻译优先不是创新。 |
| [VDC硕士论文2011](https://www1.chapman.edu/~arasteh/publications/VDC_11.pdf)，§4.2、印刷p33 | 根据页/行边界及FIFO剩余容量生成可行最长burst；是读到显示FIFO，不能冒称完整双端IOMMU copy机制，但已否定宽泛“动态burst”首创。 |
| [TaPaSCo/H2RC2019](https://www.esa.informatik.tu-darmstadt.de/assets/publications/materials/2019/2019_H2RC_MIO.pdf)，§IV-B、p4 | 已讨论多加速器读反压与写占用互相堵塞；用完整读返回容量和完整W数据后发写解除。仅发现这条环或整预约不新。 |
| [Arm DMA-350规范Issue04](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2)，§5.8.1、p87 | 读前留完整返回空间，写前备齐整个burst的数据，明确为避免读写循环依赖。是必须保留的强普通工程基线。 |
| 已取得iDMA2024、CTC2009、TAPA2023、XDMA2025 | 几何合法化、信用耗尽结束包、运行时地址合并与超时、配置/数据分离分别已有。不能自动认定它们已发表G的完整机制，也不能只重新组合就确认创新。 |

本轮成功新增VDC2011和Arm DMA350两份PDF/TXT；TaPaSCo/H2RC2019下载遇代理503，已通过网页PDF核§IV-B原文，未计本地取得全文。见[来源](../out/translation-lane-check-20261008/papers/sources.json)。另核TI2025[Adaptive Burst原专利](https://patents.google.com/patent/US20250291754A1/en)：有FIFO/优先级事件控制进行中burst结束/延伸的合同；当前标准AXI已给定ARLEN的事务不能照搬。未取得2026 adaptive-ECC DMA正文，仅二手题录，不用它作机制重叠结论。CN114780466全文未成功访问，列为检索缺口。

## 4. Codex新规则是什么，为什么还不能升级贡献

[原始独立草案G](../out/translation-lane-check-20261008/codex-new-rule.md)改变**源读请求生成的边界**，不是只在请求生成后调优先级：

- 从copy当前逻辑字节开始，p是目的译址、虚拟/物理写顺序及独立写服务共同保证可连续消费的长度。TLB命中或瞬时WREADY不够。
- 在虚拟AR首次VALID前选择并冻结源读L，必须满足`max(0,L−p)≤free_credit`，不能截断已经接受的AR；后缀按实际消费释放预约。
- p覆盖合法Lmax时合成长R；p只覆盖前缀时先切在p处。p=0时仍允许安全整返回预约回退，不能把“不发”作为绝对规则制造新环。
- 实现需合法copy/准备信息接口，并同步更新legalizer字节数、剩余地址、tailer；不能只改ARLEN，不能用测试层次窥视免费获得信息。

**审定：** 当credit=0或p已覆盖Lmax时，这与普通最长可行`L=min(Lmax,p+free_credit)`一致。credit>0且0<p<Lmax时G更保守，两者决定不同，但尚无可信真实场景证明G更好。强普通方法也能做准确后缀预约、动态分段和当前W-owner直通。初版G降为比较候选，不标“新算法成立”。

Codex随后提出H：只有存在具体跨copy依赖时，才保留另一copy当前W义务所需最小进展资源，并据此在offer前切短源R；没有依赖就允许更长R。与固定per-bank预留的拟议区别是条件与预约量，而不是检测等待环本身。**H是未证候选：受限对齐场景中，当前W-owner可能已有p>0，普通切短＋直通就不需额外byte预留；此时H没有独有决定。** 还没有证明非零的最小保护量、合法可达见证、面积或吞吐收益。

DeepSeek给出的反例把PTE返回信用与payload池混在一起，且把p=0有安全回退误写成一律禁止。Codex拒绝用它证明平台可达；“预算先已扣除保留量却又被抢掉”也不是完整资源账本。worker建议先实现G/新增共享PTE-payload池及自定5%面积门槛没有采纳。不能为造创新反例把硬件改成更差基线。

## 5. 下一步唯一验收物：能区别强方法的决定，而非更多优先级扫描

性能/成本主线不变。先在真实独立源/目的分段、有效copy关系、现有DS/payload分离下，找出**同信息、同资源**时：
1. 普通最长可行＋准确后缀池＋当前owner直通无法达到的有效决定，或者需要明显更多状态/资源才能达到的保证；
2. 候选G/H对应的确切L、预约量、写顺序和进展见证；
3. 在固定短读、固定长读、普通动态分段、完整读返回预约＋W数据齐备门、固定预留、DS优先之间的差异。

没有这个见证就停止G/H主贡献推荐，不先改生产RTL、构建BIT或扩矩阵。现CONF0对齐旧任务不产生独立分段错配，不能拿新候选解释旧PN退化。若进度/成本证据成立，再做一个真实iDMA最小PoC；常规修复与对照工程保存，但不冒充论文创新。当前未认证创新或二区水平。

本轮：新增真实RTL仿真2/PASS2；新代码/编译/Linux/BIT/JTAG/SD/板卡动作0。DeepSeek两次提供连接证据、前作抽取和独立质疑，总340752 provider tokens，thinking/high/393216、完整未截断；Codex独立设计G/H，直接核关键原文/源码、执行仿真、拒绝无效反例并审定。完整答复、原日志与失败的资料访问保留。
