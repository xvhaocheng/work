# 真实多DMA两场景预筛与裁决 stage103

2026-10-12。用户选择重新筛真实并发场景并批准计划，范围由已结束A/B调整为网络/存储、周期采集/独立后台两类。整体仍在第一阶段“问题与方法立题”；本批是新一轮需求预筛，未执行G0或进入G1。Codex根负责关键源码、控制时机、成本和裁决，两必要Codex席独占取证。

**裁决：两张需求卡接受为来源证据，两项均未通过可执行G0入口，本轮预筛结束。** 新事实建立真实程序和任务生命周期，但缺匹配运行实例的共同翻译路径、完整事件/完成/成本对应；当前没有选定场景、G0运行登记或新候选。不追加第三场景，方法论文总体目标未完成。

## 两场景得到什么

| 场景 | 已取得的正向事实 | 完成边界 | 决定性缺口 |
|---|---|---|---|
| 网络收发＋独立本地存储 | 固定iperf3端点及有限字节/块数控制；固定SPDK PCIe NVMe提交、回调补发、buffer复用与最终排空；VFIO配置 | receiver成功接收与NVMe命令完成分别报告；不是内存copy完成，也不自动代表持久化 | 同运行实例NIC/SSD translated mode、Requester/domain与实际共用cache/PTW；任务事件/映射生命周期和端到端成本 |
| 周期采集＋独立后台 | 官方SDI→VDMA/Framebuffer Write→DDR入口；固定YAVTA的QBUF/DQBUF/消费/归还/停流 | 采集buffer ready、CPU消费、重新QBUF、文件完成分别报告 | 匹配capture/GEM配置的启用共同SMMU路径、协商帧字节与实际周期；独立有限后台仍未确定 |

网络侧是两个真实官方基准程序的拟议研究组合，未取得原生联合服务需求；组合实验本身可以合法明示，本次不将其改称原生轨迹。采集输出自己的文件写入属于前台消费，不能当独立后台。没有期限也可评价双方完成/进度；本轮没有新增阈值或延长后台。

[网络存储卡](../out/real-dma-scenario-screen-20261012/network_storage/demand-card.json)／[周期采集卡](../out/real-dma-scenario-screen-20261012/periodic_capture/demand-card.json)／[选择裁决](../out/real-dma-scenario-screen-20261012/selection.json)。

## 根直接核到的关键边界

网络侧固定SPDK v24.09 commit `19524ad45373e35d5fdc06f4e081444d87ceed80`、iperf3 3.17.1 commit `2acfcfe94e928e74542c9f107e02aa6dd4748a79`。SPDK正常完成复用相同task/buffer，随机LBA不代表新host IOVA。`io_complete`检查错误后仍可计入`io_completed`；打印在排空之后，elapsed在排空前捕获。以后若比较固定窗口有效字节，需独立成功完成事件，不能直接套报表计数。

[SPDK实际完成代码](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/app/spdk_nvme_perf/perf.c)／[系统配置](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/doc/system_configuration.md)／[iperf3官方手册](https://github.com/esnet/iperf/blob/2acfcfe94e928e74542c9f107e02aa6dd4748a79/src/iperf3.1)。VFIO group给隔离/权限粒度，示例是I/OAT多通道，不证明NIC/SSD共用翻译缓存或walker；UIO的off/passthrough配置不能当翻译干扰证据。

官方SDI入口请求3buffer、捕获10帧、YUYV1920×1080、跳过前7帧保存。根核固定YAVTA源码：正常10次DQBUF仍有10次采集，只有最后3次允许保存；实际buffer数、stride/sizeimage/bytesused与索引次序须取返回值。不能直接把名义4,147,200B当实际DMA字节，也不能把支持帧率列表当这次程序的固定到达周期。

[官方SDI入口](https://xilinx-wiki.atlassian.net/wiki/plugins/viewsource/viewpagesrc.action?pageId=1090519105)／[固定YAVTA源码](https://git.ideasonboard.com/camera/yavta/raw/commit/42573311466dbeab630d084bc2d5afe88154690b/yavta.c)。官方wiki实际页面revision未知，仅固定取得的工具全文快照；固定2026 upstream YAVTA、2025.1默认DTS与历史示例没有组成匹配同image。具体[DTS](https://github.com/Xilinx/linux-xlnx/blob/0a0f70e531c7c36d7ab42b374286b67b9fc2c660/arch/arm64/boot/dts/xilinx/zynqmp.dtsi)的SMMUdisabled、GEM iommus注释已核，但不据此断言所有视频运行绕IOMMU。

采集DDR环无需放进R2私有96KiB SPM；名义环大小不是否决理由。真正待核的是DDR/PTE授权、生产者与CPU消费/ownership、软件供给及其时序。现诊断模型只有两iDMA和生产翻译模块/行为DDR，没有真实NIC、SSD、采集源、协议/媒介、CPU cache和MIG；寄存器cfg绕生产CPU互连。因此阶段投影须说明边界和成本，不能免费当完整应用。

[根逐段原文核证](../out/real-dma-scenario-screen-20261012/root-critical-source-review.json)／[实际控制点与适配边界](../out/real-dma-scenario-screen-20261012/root-control-and-adaptation-boundaries.json)。

## 两条待检验假设与普通对照

网络/存储：若NIC映射周转与存储常驻pool确实争用同一翻译资源，相同字节率和在途预算下，页局部性/映射寿命是否仍造成普通byte pacing难预测的receiver等待？必须保留合法大页、buffer复用、布局/准备及完整成本；F&S严格map/unmap/cache失效语义不能换成当前静态DDT/Sv39实验。若这些普通方法已满足双方目标，则关闭该假设。

采集：若实际生产/消费时序留有有限slot复用余量，已发布后台copy的剩余翻译服务是否让采集超余量，使下一copy门控来不及？比较合法ring准备、页大小、发布前优先、反馈和有界分段，计所有成本。若普通控制已覆盖余量并完成后台，则关闭该假设。当前余量、共同路径和完整后台未建立，不填有利参数或宣称实测失败。

[根条件假设与反例](../out/real-dma-scenario-screen-20261012/root-hypotheses.json)。两条都是条件问题，没有接受创新规则；不能据此说前作已被击败或普通策略已失效。

## 本轮终点与执行记录

每场景三项新原始材料，总六项到限；检索/版本元数据只用于定位，未作为额外应用证明。网络侧一次过时路径404后改正确路径；采集官方页直接请求202空体、REST404，改用官方页工具全文快照；原失败记录保留。没有再扩大来源、场景、强度或期限。

两卡都未达到可执行G0条件，不作适配成本排名或伪造数值成本，也不生成占位实验矩阵。重开需要一个选定运行实例的实际共享路径和独立任务buffer/arrival/有效completion/mapping记录；然后固定相关普通基线和最小判别。当前结束的是本轮预筛，原E2/E3及stage98—102受控/工程/有限阴性结果完整保留。

[根裁决](../out/real-dma-scenario-screen-20261012/root-result-acceptance.json)／[网络来源与账本](../out/real-dma-scenario-screen-20261012/network_storage/source-manifest.json)／[采集来源与账本](../out/real-dma-scenario-screen-20261012/periodic_capture/source-manifest.json)。全Codex根及两个复用席，未调用DeepSeek/Claude；本轮零性能模型、构建、应用运行或FPGA，无生产修改。worker各做一次来源静态验收，root另做最终来源/文档验收，不能把它们称实验运行。
