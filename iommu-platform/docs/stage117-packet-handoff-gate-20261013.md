# stage117 逐包消费可达性裁决：2026-10-13

裁决为**需要具名有界软件适配**：源任务要求及时交付和回收完成包，平台有并发DMA与精确搬运原语，但现有公开接口尚未把完整包完成、CPU缓存交接、发布、消费和B归还串起来。本轮没有实现该新合同，也没有新性能结果。不能据此认定硬件不支持并发或必须新增RTL。

本轮问题是：另一DMA仍活跃时，CPU能否通过已有接口安全取得一个完整包并归还其缓冲？已有证据为DPDK v25.11本地VM→VM来源、原三帧适配及上一轮四案。进入条件是同一来源、原字节/布局、可定位的实际完成与缓存路径；结束条件是源需求、已有接口、新增能力、未知量分别裁决，root接受后同步总/局部计划、状态、交接和验证，交付检查通过后才实际goal complete。

研究平台仍是Genesys2/CVA6＋四iDMA＋RISC-V IOMMU＋Linux；Dell为主机支持端。上一轮有限G0 single/parallel全部排空1161/1601模型周期的普通覆盖保留，未重跑；440周期差额不解释本轮尚未实现的逐包性能。C117R3未固定为论文主场景，论文未完成。

## 来源支持什么

来源保持冻结commit `ed957165eadbe60a47d5ec223578cdd1c13d0bd9`。`vhost_lib.rst:511–515`明确要求及时poll完成包通知guest，持续submit不poll会填满DMA ring并最终丢包。这是完成回收的真实资源依赖，不提供本平台到达率、期限或已有性能损失数值。

`virtio_net.c:91–138`将包完成标志关联到最后一个copy；`:2277–2343`只取连续完成前缀、减少inflight、release发布used并通知guest，无全局DMA静止条件。`:1201–1329`在异步payload提交前由CPU构造header。示例`main.c:1358–1369`在下一burst提交前先poll；`:1022–1040`释放返回的host mbuf。普通批量poll/供给仍是强对照，不要求一包一次poll。

host mbuf释放是A侧复用，不等于B guest已消费并重新发布其buffer。源码证明B的used/通知边界，实际guest执行和B归还时刻未观测；受控适配必须分别记录。包3的503B与1011B两段齐才是1514B完整包，不能用部分DMA字节冒充完整有用包。[13组root原文及源哈希](../out/stage117-packet-handoff-gate-20261013/source/evidence.json)。

## 平台已有原语与缺项

tracked `idma.c:719–740`确有两路独立poll/refill，V完成可早于A。非batch DEFAULT支持精确SRC/DST/NUM_BYTES和逐lane NEXT/DONE；不把实验ioctl的4096整除规则或batch限制扩展成硬件长度限制。当前公开接口接受内部pattern实验/研究请求；`:2622–2652`仍在两lane drain后做CPU缓存交接和校验，`:2927–2932`的fops无payload read/write/mmap，没有完整包/cookie/acquire/consumer/return入口。历史R107有限模块的allidle只约束那版接口，不代表整个平台只能串行。

正常DEFAULT完成沿R→buffer→W→最后B响应→backend rsp_valid→DONE。`dma_core_wrap.sv:319`明确NO_ERROR_HANDLING，`idma_backend.sv:521`将error绑零，所以DONE不能单独证明字节成功。IOMMU fault queue现按cause/DID/iotval记录并限频打印；observer核ID与握手守恒，未捕获逐次RRESP/BRESP，也没有包/transfer-ID关联的成功或失败收据。unknown NEXT/timeout已有retain/quarantine基础。

受控成功路径可以保留完整实际字节、guard及事务退休验证，并计入额外CPU读取/源保留成本；这不等于生产异常安全回执。完整生产恢复不是所有G0的前置要求，是否需要额外硬件错误可见性尚未决定。[36条平台原文与调用链](../out/stage117-packet-handoff-gate-20261013/platform/evidence.json)／[平台裁决](../out/stage117-packet-handoff-gate-20261013/platform/result.json)。

## 缓存和原布局的具体边界

历史已验f9冻结源的实际链是add_sources选择`cv64a6_imafdc_sv39`、Genesys2全局WT_DCACHE、DcacheLineWidth128bit，即16B缓存线。Linux `local_flush_icache_all`发fence.i；CVA6等待CPU写缓冲/MSHR，清I/D-cache并确认。WT清valid不做脏整行writeback，没有“所有DMA idle”输入。因此全cache操作本身不能推出所有DMA必须停止；它也没有自动建立逐包所有权合同。

普通MMIO屏障不消除陈旧payload缓存；通用dma_sync名称或CONFIG_ZICBOM不能证明该CVA6已有按range维护能力。原DTS宣称rv64imafdch，实际路径仍需绑定平台缓存操作、single-hart、CPU PA/IOVA别名和metadata归属。

原fixture布局按16B行计算如下，均为对齐base内半开区间；没有增加padding或改变原跨度。

| 包 | 源缓存行闭包 | 目的descriptor容量闭包，含头和guard | header与payload共享行起点 |
|---|---|---|---:|
| 1 | [128,192) | [8320,8432) | 8336 |
| 2 | [384,464) | [8592,8720) | 8608 |
| 3 | [784,1296)、[3792,4816) | [9024,9552)、[12048,13088) | 9040 |

跨group闭包交集为空，同包header/payload仍共享行。提交前需排空属于DMA包的CPU写；交接前必须完整包全部跨度退休，执行对应缓存交接，再按完成前缀publish/consume。CPU不能在peer-active时扫描整16KiB池或peer guard。行不相交只是必要范围事实，不证明当前已经实现安全交接；metadata实际布局未指定。[静态布局原始计算](../out/stage117-packet-handoff-gate-20261013/platform/cacheline-layout.json)。

13/14个关键tracked↔f9文件相同，top模式不同：历史f9 NonblockMode3/E3Observer0，tracked为0/1，因而DMA0/1缓冲选型512与3 beat不同。当前部署身份、当前板逐包交接和板性能均未新核，不能把历史源或tracked默认冒充当前运行实例。本轮未恢复R107。

## 接受的有界适配方向

root接受后续方向为同一可信owner的有限packet ownership/cache glue：原三包1639B/1675B记录、四精确span、原地址、同域DID10/11和2MiB驻留映射。复用现DEFAULT与WT原语，新增packet/span/cookie输入、完整包join、CPU acquire、used发布、原B消费后归还及不确定状态保留。它是普通基础能力，没有新颖性或收益声明，本轮不实现。

A读退休、host staging归还、B used发布和B实际消费归还分开。逻辑buffer回收不能撤销仍覆盖peer的驻留leaf；最终停止issuer、核完整quiet与checked释放后才unmap/free。无法证明退休的allocation不访问、不归还，保持retain/quarantine。

成本必须包括逐span配置、poll/供给、全I/D缓存失效和重填、头/used排序、真实consumer、源持有延长、可选完整校验及最终drain/map释放；实际时间、资源峰值、板端面积/频率/功耗保持null。实际映射/别名和metadata行布局也未闭合。[适配范围、条件及取舍](../out/stage117-packet-handoff-gate-20261013/adaptation-scope-card.json)。

因此本轮不冻结新G0合同或启动后继goal。下一步若继续同源，应先实现并验证这份有界软件合同，再固定来源支持的交付目标与CPU/单DMA/普通多DMA等可执行对照做一次G0；无需先证明残余。保留大页/驻留/批量/布局控制，不人为加压、造期限、换源或移植完整VM/vhost系统。

root亲核关键原文、缓存实现、完成链和现接口并接受稳定裁决；新平台席实际Astra/ultra，主会话实际Sol/ultra，均按原生上下文运行，未设置额外预算或上下文覆盖。当前主会话有效窗口观察475000、平台席258400分别记录，速度倍数未验实。没有DeepSeek/Claude，没有新实验、构建、板探测/部署或生产ABI/RTL修改。[稳定root接受](../out/stage117-packet-handoff-gate-20261013/root-result-acceptance.json)／[证据导航](../out/stage117-packet-handoff-gate-20261013/README.md)。

本轮已在root稳定接受、全部计划/状态/交接/验证同步与246项交付检查通过后，由实际update_goal返回complete；工具记录308,316 tokens／29分13秒，无额外token预算，论文未完成。[实际完成](../out/stage117-packet-handoff-gate-20261013/root/goal-completion.json)／[root交付核验](../out/stage117-packet-handoff-gate-20261013/root/delivery-verification.json)。
