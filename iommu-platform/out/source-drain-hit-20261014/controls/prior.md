# SDH1准确前作与本轮程序边界

2026-10-14；Codex直接浏览主来源、直接读本地关键RTL；没有调用DeepSeek/Claude，没有重跑旧验证。

主来源：[Yuchen Hao，Architectural Techniques to Enhance the Efficiency of Accelerator-Centric Architectures，UCLA博士论文，2018](https://escholarship.org/content/qt8p2116wz/qt8p2116wz.pdf)，Chapter4。论文中的章节4对应作者列表里的Cong等HPCA2017工作；本轮只核这份原文，不把另篇论文标题当独立查证。

PDF印刷页44–45（PDF页62，零基页61），§4.3.2.1明确其private TLB支持nonblocking hit-under-miss；印刷页47（PDF页64，零基页63），§4.3.3.1明确shared L2也支持hit-under-miss。关键原文片段是“non-blocking hit-under-miss support”，其目标是让miss与其他entry hit重叠。两处附近讨论TLB shootdown；private项明确扩展IOMMU向private TLB发失效，shared项要求也检查失效。提取源行1667–1690和1722–1743可导航到相应段落。

**已被原文覆盖的概念：** miss不应阻止其他合法TLB hit；不能把这个概念单独归为SDH1创新。**原文未给出的本轮具体内容：** current-W exact source/job/needed-byte qualification、external physical AW接受证书、完整source/slot/generation直接回原AR、同bank原header槽生命周期、一次grant和一个结果槽的配对成本。原文不够证明上述完整机制已经发表；也不能把其512entry shared cache/32entry private cache的实验成本搬到当前16entry/一个PTW系统。

O-NBO是本轮**独立构造并固定的普通程序**：普通nonblocking lane优先服务已承诺写的当前真实数据producer，在同安全filter下查询其源内最老hit。它不是复现Hao全文的代码，也不是仅换名调用候选。其exact coverage来自 `ordinary_policy.py` 和 `ordinary-program.md` 的唯一owner/唯一source-head映射；其同逻辑成本来自同槽、同snapshot、同接口和同adapter构造，见 `cost.json`。

因此两个裁决分别是：因为Hao确实已有hit-under-miss，不能把“miss期间pass另一个hit”作为独立概念创新；因为具体同资源O-NBO覆盖本轮SDH1具名全部选择及其adapter事件，不能把SDH1已述动作或这套逻辑资源配对作为超过该强普通程序的独有能力。这个原因不同于发表前作全文覆盖；当前面积/Fmax均未知，不能用未测物理差取代一个已成立的成本优势。

继承但未重跑：`iommu-platform/out/blocking-replacement-20261014/control/fixed-controls.md` 的具体普通程序/精确coverage方法；`iommu-platform/docs/r2-ownership-cost-20261014.md` 的正常WRITE原位复用、真实external AW和common成本；旧N路径以当前RTL为准。DS/preprepare、full-return、RR/age、single-DMA的适用条件在 `ordinary-program.md` 第4节，本文没有泛化全族否决。
