# stage117 重做第2版：先核任务必要性和现平台接口

已按用户“如果不好做，就重新制定stage117的规则重新stage117”执行。stage118确认C117需载荷输入与真实图像consumer适配，当前投影条件归档；随后修订规则并只重选一个来源C117R2：DPDK DMAdev数据包复制转发。新来源被普通对照提前筛出，未准入基线或G0/G1。本轮完成规则修订与一次重新筛选，论文未完成。

## 为什么改规则

C117的普通DDR copy会增加原RDMA输入没有的源读。现iDMA虽有WRITE模式，WDATA仅提供一个64位寄存器数据字，没有接收帧载荷的stream接口；批处理与受控launcher又只接受DEFAULT copy。不能在运行时任意改WDATA冒充有背压保证的payload流，也没有本轮验证多拍WRITE。配置为单次8字节、保持参数到完成，可以描述一个寄存器供数原语，但这不构成图像接收应用。实际legalizer把copy的读写都发给同一设备/PID，mmu标志均固定；没有已有源读单独绕过翻译的寄存器开关。identity地址映射也仍会发翻译请求，不能当作消除了源读。

原图像consumer还包括RAW10解包、黑电平、整帧直方图/白平衡及后续demosaic；seedbyte checker不能替代。9个关键硬件文件与冻结FPGA源码逐字节相同，本轮只核源码，不重做板端身份检查。stage118据此给出needs_named_adaptation，用户转向后终止该适配链，未登记stage119或实施任何改动。[stage118决定性证据](../out/stage118-c117-interface-reachability-20261013/README.md)。

## 新规则

1. 先写有用最终输出与所有权，亲核原地处理、不复制、CPU路径和同owner单DMA等普通对照。一个程序专门做copy benchmark，不能单凭此证明真实任务必须复制。
2. 再核现平台地址、读写方向、payload、完成与释放接口。区分RTL具备原语和driver已经暴露可运行接口；需要新载荷通路、大型consumer/runtime或新翻译语义时，先结束该投影。
3. 给source-backed完整字节、依赖、生命周期和资源界限。真实任务是转发时验包内容/头部变换/所有权；任务是数值计算时验数值输出。未知成本留null，完整数值和性能由后续普通基线取得。
4. 原生系统主张仍需原生requester/启用翻译/共同单元证据。明确限于现FPGA的有限投影可复用已接受的共享路径，但必须保存因果映射与遗漏，不冒称原生应用或速度收益；新性能依然需要对应实测。完整外部运行包不再作为所有选源的前置门槛。
5. 来源与接口通过后，才另立有界普通基线合同；有具体可达可避免的翻译残余后进入G0，G0支持后才G1。不能靠强制copy、隔离、小页、压力、期限或缩小普通对照救候选。

GPU/AI在可适配来源中仍优先；本轮C117已有GPU来源实践，用户明确要求换规则后，允许选择平台更适配的非GPU任务。每轮只冻结一个新来源，遇到明确失败即停，不在同轮追加第二个凑候选。规则文件为[revised-stage117-rules.json](../out/stage117-r2-platform-first-20261013/revised-stage117-rules.json)；旧stage116/117记录原样保留。

## 本次重新选择及核实

选DPDK `examples/dma/dmafwd.c`，限定IOAT memory-to-memory实现。标签v25.11，commit `ed957165eadbe60a47d5ec223578cdd1c13d0bd9`；保存tag/commit/tree及9份必要原始文件，全部核git blob和SHA256。在线25.11文档当前显示25.11.3，裁决以冻结v25.11源码为准。vhost检索只作入口导航，没有选择为第二工作流。

任务链是RX包→分配dst/复制metadata→按src/dst IOVA复制data_len→取得DMA成功完成→释放源mbuf→目标进rx_to_tx ring→CPU改MAC→NIC TX。每RX queue分配一个dmadev，source/dst指针按copy返回索引保留。源自带SW memcpy对照；接口类型为MEM_TO_MEM，与现iDMA DEFAULT的方向匹配。实际原生拓扑、启用IOMMU和同时流量未观察，不能从多个逻辑队列推出共同翻译单元。

读取实际driver还发现：现有研究API要求chunk整除4096，而且没有通用packet payload/consumer接口。底层copy方向匹配不等于DPDK已经能在板上运行。C117R2在更早的任务必要性关已经失败，因此没有继续建设软件适配。

## 可执行的普通对照为何成立

冻结源码中，原包在copy完成后即被释放，后续只有复制包改MAC并发送；未见要求保留原头部/内容的其他consumer。对有效、可写、独占(refcnt=1)的RX mbuf，可以把原mbuf直接放入同一个rx_to_tx ring，沿用相同MAC变换和TX queue。入ring失败则释放并计drop；TX接受的包交由PMD持有，不能提前释放；TX未接受的包按原规则释放/计drop。RX API会补充接收描述符缓冲，TX API接受同类pool mbuf并在真正完成后回收。若有真实共享/不可变/保留原内容合同，则保留copy分支；不能为了候选临时增加这种合同。

输出仍是同样长度、同样12字节MAC字段变换、其余payload原样、相同每队列顺序与合法drop规则。时间、实际drop率和最终吞吐可能不同，未运行、未测量，因此没有“零复制一定更快”的结论。原路径copy期间持有src+dst，之后持有dst直到TX；原地路径持有一个mbuf直到TX，完整驻留/队列/回收成本仍须计算。

这是从同一程序与API构造的普通对照，未声称选定sample本身已实现它。它不等价于以“必须执行copy次数”为定义的复制基准；正因如此，该benchmark本身不足以证明应用级独立复制需求。当前卡不能支持通过新增方法优化这些可省copy的共享翻译损失。NIC RX/TX的翻译活动仍存在，不把省去copy引擎说成消除了整个IOMMU问题。[具体对照及条件](../out/stage117-r2-platform-first-20261013/ordinary-control-decision.json)。

## 字节、成本及边界

本轮保留force_min_copy_size=0，禁止为压力扩大复制。单段packet的L_i=data_len；native copy附加读ΣL_i、写ΣL_i，metadata CPU复制另计。普通原地路径仅消除这一copy引擎载荷，NIC收发、头部CPU修改、ring与descriptor等成本保留。source使用data_len，不能默认为多段packet也完整正确。

源默认burst32、DMA ring2048、RX/TX ring各1024、最小65536个mbuf；保留原pool公式及默认值，不冒称为FPGA实际资源或“必须凑64笔”。实际包长/数量/到达、成功bytes、DONE/quiet/释放、wire完成、RAM和时序均未知。全部准备、供给、映射、copy/CPU、ring/drop、MAC consumer、NIC TX、cache交接、排空释放与有限投影适配成本列入[字节资源成本卡](../out/stage117-r2-platform-first-20261013/bytes-resources-costs.json)。DMA完成不等于wire完成；TX返回入描述符数也不能代替最终完成。

如果新原始任务证据确立必须保留独立输出、原数据不可改或真实所有权约束，且仍不能被普通路径满足，才值得新的有界来源裁决。这是相邻正条件，不是本轮已存在的条件。不宣称DPDK示例无效、所有copy都多余或共享翻译研究失败。

## 执行与下一步

仅源码/接口审查、规则修订和文档交付；无新agent、DeepSeek/Claude、部署、RTL/driver修改、仿真、构建或旧实验重跑。实际goal仍是stage115 paused、完整原目标未完成，工具不允许覆盖未完成goal；stage118与117-r2按授权合同执行，均未冒称新goal登记或旧goal complete。

唯一下一合同为stage117-r3：先有真实的必要搬运/所有权证据、再核现平台接口，仅选一个来源，GPU/AI可行者优先；不重新跑这两张卡，不为copy benchmark追加约束。合同未登记/未执行、不自动启动。[下一合同](../out/stage117-r2-platform-first-20261013/next-goal-contract.json)。

[原始来源](../out/stage117-r2-platform-first-20261013/source/file-manifest.json)／[关键原文与行号](../out/stage117-r2-platform-first-20261013/critical-source-review.json)／[需求卡](../out/stage117-r2-platform-first-20261013/demand-interface-card.json)／[稳定接受](../out/stage117-r2-platform-first-20261013/root-result-acceptance.json)／[实际账本](../out/stage117-r2-platform-first-20261013/execution-ledger.json)／[交付核验](../out/stage117-r2-platform-first-20261013/delivery-verification.json)。
