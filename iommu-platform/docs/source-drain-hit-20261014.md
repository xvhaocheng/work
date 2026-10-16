# SDH1：源级写排空命中旁路与双页交换，2026-10-14

**结论：主候选交付了可执行私有功能原型，但具名普通 O-NBO 在相同信息、资源和付费接口下复现其动作；本版没有独有动作或完整成本优势，不进入四个性能条件。** 双页交换交付实际值正确的参考程序及有条件源码链，保留为DMA协作设计卡；缺原生AW前门、准确读收据和故障清理，不宣称IOMMU贡献或应用净收益。

actual goal1791986539已登记，无额外token预算，最多两个工作日；root与两新独占席实际Sol6.1/ultra/fork-none，原生有效窗口475000，未设置手动覆盖。源核席异常结束后root补完关键原文与交付，没有恢复旧席或新增第三席。实际complete须在五页同步和新交付检查之后，回执见[产物入口](../out/source-drain-hit-20261014/README.md)。本轮结束不代表论文完成，原立题窗口累计。

## 具体问题与新规则

生产N在normal PTW忙时只支持异bank受限hit；TC每bank只保存一个active翻译生命周期。完整ID0/8同bank，4/12同bank。若另一源的必要read hit处于同bank，当前通道不能交付它。只放宽bank比较会把它的PA错交activeTC，因而本轮改变的是请求前进与所有权路由。

冻结规则见[机制卡](../out/source-drain-hit-20261014/root/mechanism-card.md)：每完整源最老未完成AR、异于active miss source、真实受限N权限hit，且属于准确当前W头同job/needed字节区间、匹配physicalAW已经接受、前序W独立可排空，才能使用现有N单OUT。用fullsource/原槽/generation关联原AR，保持header及PA直到physicalAR握手；normal owner与其原结果寄存器独立保留。正式失效/normal完成优先，原型一walk最多一次grant，普通可以使用同样规则。

保持单PTW、16IoTLB、每bank原4AR/4AW、总32TC W拍及DMA buffers[512,512,3,3]。64B没有原生fullbuffer许可；许可只页对齐4096B DEFAULT且容量足够。hit并不自动提供数据返回安全许可。来源见[root16files/36原文](../out/source-drain-hit-20261014/root/source-evidence.json)。

## 可观察输入与源端可达边界

完整source、raw slot和generation、源内顺序、currentW producer/job/cursor、真实physicalAW身份及accept、已供给前序W与当前读区间均是有主状态；新增读取、记录和跨模块关联收费。现有bank readowner不是所需证书。DDTC/IOTLB只能通过现有共享probe依次查当前请求的真实内容，不免费并行获得四个head的命中，也不借active miss的DC。DDTP/DC稳定、权限、MSI排除和失效优先沿用现有N限制。

[原生链](../out/source-drain-hit-20261014/source/native-chain.md)给出新的source-compatible冷偏序：B的24B页内copy先正常准备并接受目的AW，建立有用的同页合法leaf；另一同bank源A的下一AW被原AW-before-AR规则选择并发生miss；B最老AR仍需提供当前W的数据。没有添加预热/压力/时相/延迟或walker。**此偏序未经真实frontend/native执行**；source0/2没有共同硬件launch口，供给与共同逻辑就绪成本未知。私有原型使用显式已观察权限fixture，不执行这条冷链，不能将fixture算成免费命中建立。

## 私有功能原型与具体普通

[protocol.py](../out/source-drain-hit-20261014/prototype/protocol.py)实现原两bank各4槽的稳定索引、代数防旧结果、源内最老、normal pinned owner、N单OUT direct-route、锁定physicalAR、真实R/W/B归属、字节/值守恒、sticky error receipt及consumer/quiet。正常PA由原TC结果独立保存；fast PA由显式叶PPN与IOVA低12位生成，并核可信copy绑定，不能直接用期望job地址构造翻译结果。原型permission是源码谓词的规范化已观察输入，**不是真实cache/DDTC/PTW、生产RTL或DDR性能模型**。Python age/gen为无界整数，硬件宽度/回绕规则未实现、必须计费。一般生产失效、ATS/MSI/stage2、取消和错误发布仍未验。

[独立普通程序](../out/source-drain-hit-20261014/controls/ordinary_policy.py)不调用候选：找当前已承诺W的唯一数据生产者，检查其最老AR和同样安全资格，然后用通用single-PTW nonblocking lane处理。完整tag、同一原header、directOUT和physicalAR锁均付费。它不被强迫新增MSHR/PA表，也不被禁止合法字段复用或同bank跨ID选择。

覆盖理由是具体的：有效currentW确定唯一source/job；源内最老确定唯一AR。候选资格集合与O-NBO条件一致，故相同可信snapshot返回同tag或同None；共同adapter的发布/接受/退休步骤归纳得到相同事件。该证明限于本冻结规则和同adapter，不等于所有非阻塞族或发表前作全文覆盖，也不证明原生N已具有这些接口。

准确前作：[Hao2018博士论文](https://escholarship.org/content/qt8p2116wz/qt8p2116wz.pdf)印刷页44–45及47明确private/shared TLB已有hit-under-miss。不能把该概念单列创新；原文并不等于本轮完整W/source/job/physicalAW协议。具体O-NBO覆盖和发表覆盖分别记录在[prior.md](../out/source-drain-hit-20261014/controls/prior.md)。RR/年龄、generic无owner、full-response预约、DS/preprepare和singleDMA范围见[普通程序表](../out/source-drain-hit-20261014/controls/ordinary-program.md)，没有拼每场景事后赢家。

## 实际新验证与成本

| 新执行 | 结果与适用范围 |
| --- | --- |
| 六组新功能检查 | pass：同bank完整source路由、heldVALID、normal结果不覆盖、role交换、AW/权限/job/区间/同ID/失效拒绝、错误与RWB/consumer/quiet、generation及全页swap值 |
| 32,768 finite snapshot pairs | pass：13布尔维度×四原ID角色对，O-NBO选择与候选相同；非穷尽生产协议状态空间 |
| 三条普通完整事件链 | pass：正常、角色交换、错误收据；完整R/W各48B、6拍、2AW/2AR/2B/2consumer，全部event等于候选参考 |
| swap/scratch实际值参考 | pass：两页旧值交换正确；swap R/W各8192B，scratch各12288B，均为参考程序统计，不是native板测 |
| 独立普通CLI/schema | 20项pass，保存原新检查记录 |

[功能原始事件](../out/source-drain-hit-20261014/prototype/functional-results.json)／[普通原始事件和有限枚举](../out/source-drain-hit-20261014/prototype/ordinary-equivalence.json)。一次新比较器把内存tuple与JSON array直接比较导致失败；统一JSON表示后通过。初版功能结果在context/叶PA审查修订前原样归档；执行账本记录这两项必要新修订，未重跑任何旧实验。

[完整成本](../out/source-drain-hit-20261014/root/full-cost.json)和[独立严格配对](../out/source-drain-hit-20261014/controls/cost.json)列出：原header选择/移除/代数与最老扫描、normal pin、新tag每保存副本、查询读口与物理输出mux、准确W/job/cursor/AW反馈、缓存端口/权限/失效/fence、错误退休、grant与选择延迟、映射/供给/真实命中建立及B/cache consumer/quiet。local slot2bit、source4bit时T=6+G；每处替换原bank1新增T−1bit。总新增位数因实际G/J/端口实现未闭合而为空，不能把符号账本称净表面积。

在共同directOUT实现中，两程序使用完全相同的上述部件与调度，所以**构造性逻辑成本差为0**；不是实测LUT/Fmax差，也不宣称最小面积。候选没有已实现的便宜独有控制。因此性能门不通过，0四条件/性能时间/PTE实测；面积/Fmax/CPU/GPU/应用/净完成收益均null。

## 双页交换的正向产物与边界

[page_swap.py](../out/source-drain-hit-20261014/prototype/page_swap.py)确实保存old(A)/old(B)至原两buffer，再写相反固定物理目标，不靠IOVA换名或消费者重新解释地址。严格成功参考统计：4AR/4AW、1024R/W拍、各8192B、4B和2consumer；scratch三copy各12288B、6AR/6AW、6B。

[源码链](../out/source-drain-hit-20261014/source/swap-chain.md)更正了AW槽：每backend一slot、source mux共享两slot；默认coupled首AW被关闸后第二AR不能生成。已有CONF.decouple=1、deburst=0能够使read independently前进；将新门放在**每源AW到source mux首次VALID之前**、初始无旧Wtoken，firstAW留原私有slot，secondAW留mutable legalizer，两完整source R可进入原buffer。R metadata/response readiness也纳入链，不只看容量。

这是一条**successful conditional源码路径**，未跑native。新AW前门、两完整4096B读收据/RRESP、可信PA对象/epoch/全部读者、故障取消清理和consumer API都不是现有done信号。NO_ERROR_HANDLING的done错误为0、DEFAULT跟lastB，不能当读阶段成功收据；swap错误前终结未闭合。

O-SW2在同付费gate/receipt接口下执行相同两阶段，现scalar READ/WRITE不能直接软件访问原整页buffer。软件门控/MMIO/CPU与硬件控制成本未测，不能据此归全族同物理成本。CPU swap、合法页重映射、scratch及相同软件门仍是强对照。少搬4096B读和写首先是DMA协作差异，尚无IOMMU方法或完整任务净收益。

## 稳定裁决与后续边界

保留普通原型、两条具体普通程序、正反功能例、冷偏序、swap真实值/流量、全部成本和接口缺项。本版SDH1独立性能主张结束；swap仅保留条件设计卡，未选为IOMMU后继。没有新增性能、生产RTL/driver/ABI、FPGA/build，未恢复BR1/RCS1/R2/R107/C112等关闭收益分支或旧席，未改原16cache/队列/缓冲或压力。

整体路线仍为机制→有界microbenchmark→一个GPU/AI实际共享路径→RTL/FPGA关键验证。若用户另选DMA合作议题，下一具体工作是实现付费前门/完整读收据及故障生命周期后比较普通O-SW2/scratch/CPU/合法重映射；当前主线不自动改题。若继续IOMMU方法，应提出超出本O-NBO的新动作或可实现完整成本差异，不靠重新打分或限制普通标签救回本候选。8/9月E1/E4/batch、E2/E3、method88/M0/stage117资产与所有历史结果保留，不重跑。

[稳定根裁决](../out/source-drain-hit-20261014/root-result-acceptance.json)／[后继合同](../out/source-drain-hit-20261014/next-goal-contract.json)／[本轮账本](../out/source-drain-hit-20261014/root/execution-ledger.json)。
