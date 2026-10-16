# stage117：Holoscan 双目图像来源预筛

C117条件保留；来源任务与生命周期成立，当前FPGA投影未准入，不能启动普通性能基线或G0/G1。本轮完成来源筛选，不代表论文完成。实际工具goal仍为stage115 paused，完整原目标未完成；本轮是用户明确授权、未注册成goal的独立有界任务，未修改旧goal。

## 具体问题与关口

选择一个未关闭的GPU/AI来源，核真实任务、owner、字节、依赖、自然共享翻译路径和现平台合法接口。stage116已将完整数值与成本获取移到来源通过后的有界普通基线，本轮仍须列齐普通强对照和全部成本项目。进入条件是明确执行stage116唯一后继合同；结束条件是单一需求卡、原始来源与hash、稳定接受、一个未启动后继合同及状态/交接/验证同步。没有实测不作科学阴性。

## 冻结来源及新增事实

只选NVIDIA Holoscan Sensor Bridge的双目IMX274 RoCE接收→CSI/Bayer→图像处理→demosaic→visualizer，标签2.7.0，commit `1df133d4c78a6921249be4b147d65554aa8efa68`。22份定向源码均核对exact commit的git blob与SHA256；标签是annotated tag，已保存并解析tag对象，不把tag对象SHA冒充commit。`hololink_module`是`import hololink as hololink_module`的别名，实际RoceReceiver走C++绑定；不能拼入另一路Python接收器的槽参数。

两个接收实例分别持有接收分配与MR；源代码注册跨多帧保留。GPU DMA-BUF与pinned host device-mapped memory均有源码路径，实际运行选择未知。host分支不必执行整帧H2D复制，但GPU读host的互连与消费成本不能漏掉。张量释放回调持有整块接收内存，保证分配寿命；它不等于逐槽防覆写或最终GPU消费完成保证。源码PSN诊断不能直接作为已经发生的缺陷。

源默认是3840×2160 RAW10、配置60FPS，文档支持双目1080p；两者分列，既不声称4K默认已经成功运行，也不暗改到1080p制造合并对照收益。两个流没有已验配对同步合同；两个IB上下文/端口不证明两个PCI requester、共同IOMMU或启用翻译。CUDA图像计算不直接计作另一host DMA requester。

## 字节与资源

RAW10有效图像字节为`5WH/4`；接收帧F还包含CSI起始、行padding、光学黑行、嵌入元数据与尾部。槽stride为`round_up(F,P_transport)+128`，两个frame slots的ring为`round_up(2*stride,P_OS)`，实际raw分配另多一页作对齐。这里pages=2是帧槽，不是IOMMU页数；协议对齐与系统页不混用。正常源目标写入按F+128推导，实际成功字节、包头/重试与物理DDR流量未知。元数据CUDA复制为每次通知128字节，不等同整帧CPU复制。

代码默认W/H下，共用Bayer池请求132,710,400字节，共用RGBA池请求530,841,600字节，共计663,552,000字节；两路共享池，不能双算。这些是源码请求量，不是实际分配量或现FPGA适配证明。实际帧数、数值输出、DONE/quiet/释放、时序、资源及各成本均未知，机器卡用null保留。

## 路径与现平台裁决

原始输入是RDMA WRITE数据及metadata notification，host NIC不需要为载荷另读host源。DDR复制增加源读：若这条读也经翻译，就改变了待研究因果。MR远端base=0是在rkey标识的RDMA地址空间，不是host DMA/IOMMU IOVA=0。源码关于IOMMU映射与权限的注释不能证明实际domain/topology或所有pinned memory的通用权限；也不借用旧GPUDirect文档的限制替代当期路径证据。

结构任务可接受，但实际backend、PCI requester、启用翻译domain/共同单元与自然同时peer未证实。现平台四iDMA共享路径是既有资产，不能据此把这个native应用说成已经运行于该硬件。尚缺source-faithful写入接口、真实数值图像consumer、逐槽交接/一致性和有依据的有限资源预算。tracked driver已有pair实验入口；不把有限repair快照的顺序批次/字节checker限制推广为硬件不能并发。因这些具体缺项，来源/接口关未通过，普通性能基线未准入。

## 普通强对照与完整成本

保留源码已用的持续MR/驻留ring、合法大页/预映射/布局/稳定scratch、同owner单网络双目（只在原格式/帧率的带宽条件成立时）、host pinned device-mapped分支，以及合法批量/队列/普通调度。源提供single-network同工作流对照，不是第二候选。单DMA或CPU数值consumer只在任务/预算/依赖忠实且合法时接受；seedbyte checker不能替代图像消费。

分别计相机/firmware/config供给、分配/对齐/pin/export/MR/PD/CQ/QP准备、wire/NIC/翻译/重试/drop、metadata/events/CPU通知、CSI/光学黑/图像处理、demosaic/visualizer、槽回收与cache/依赖、停止/unconfigure/join/QP error/CQ drain、deregister/fd close/最终tensor释放，以及投影适配资源/转换/rewarm。数值未知不填0；receive ready不能替代最终consumer完成。

正条件：实际native共享启用翻译路径证据或独立明确标记的忠实有限接口成立，并有真实consumer/资源预算，才能另定普通基线。相邻反条件：只有端口、路径未知/旁路/不共享、普通持续MR已覆盖所提收益、额外源读改变因果、非翻译消费/供给成本解释慢化或预算不成立；这些限制不能直接否定共享翻译主问题。

## 唯一下一任务

冻结stage118：静态核已有iDMA读写翻译标志、issue/DONE和cache/交接，判断能否维持native目标写入翻译而不增加翻译源读，并接受真实数值图像consumer与source-supported有限预算。只裁决supported / needs_named_adaptation / not_faithful，不实现适配、实验、构建或上板。native PCI/backend未知与有限投影结论独立保留。后继未登记、未执行，需明确后续授权；不启动第二来源、不恢复关闭卡。

## 证据入口

[来源与22份文件hash](../out/stage117-holoscan-stereo-prescreen-20261013/source/file-manifest.json)、[20处关键原文及行号](../out/stage117-holoscan-stereo-prescreen-20261013/critical/root-source-review.json)、[需求接口卡](../out/stage117-holoscan-stereo-prescreen-20261013/demand-interface-card.json)、[字节资源合同](../out/stage117-holoscan-stereo-prescreen-20261013/byte-and-resource-contract.json)、[稳定接受](../out/stage117-holoscan-stereo-prescreen-20261013/root-result-acceptance.json)、[实际账本](../out/stage117-holoscan-stereo-prescreen-20261013/execution-ledger.json)、[唯一下一合同](../out/stage117-holoscan-stereo-prescreen-20261013/next-goal-contract.json)。验证只验本轮交付与来源一致性，不声称验证native应用性能。
