# E6 最后优先级设计，待Root定范围

本批只确认来源、接口冲突与一个有限实现方案。未移植、未编译、未启动BIT构建，A1不访问硬件。
当前已通过的E5 DS0 BIT和共同E4/E5软件不受影响；不等待本设计才交付E5。

## 基座和要回答的有限问题

原版来源链固定为CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069` /
IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9` 必要修复基座；
实际拟移植起点使用其已接受P0/multi后继上的E5被动观测CVA6
`6c5c666d45979d3a03fb98cff89ba7ad095f6125`，IOMMU仍为85b。
Linux使用已接受共同E4/E5 `da6b69fc007ad5e671dc288dd8038d02e3ebe43d`，不另建旧P4M payload线。
这些是本方案的冻结候选，后续不得默默取当前HEAD；Root若指定后继应另记完整SHA与差异。

只刻画已知缓解的取舍：固定A供给间隔能换取多少V窗口改善、付出多少A完成工作/速率代价；
固定容量P、合格命中旁路N及PN在同一结构下的实际效果。历史PN负收益必须照实展示。
不以PN是否胜过P作为继续/停止判别，不新增下游优先级、读准入策略或机制修复。
首选DS0直连、50MHz、4KiB COPY、V0/DID10一页、A1/DID11八页、N4096/W256；
历史DS50数据保留为旧条件，不与新的DS0点混成一个统计总体。
本批不合入完整E3的BTC3/OBS5/Depth64或扩大工作负载集合。

## 结构比较与固定容量

| 配置 | IOTLB/替换 | 控制器与供给 | 必须报告的比较边界 |
| --- | --- | --- | --- |
| 已接受原版+被动观测 | 16条共享PLRU | 原控制器、默认3拍DMA数据FIFO | 已有E5 BIT，结构锚；旧研究S不能替代 |
| G：原版+P4M gap | 同原版16条 | 仍原控制器/3拍FIFO；仅两任务间插gap | gap0需有限逐周期等价；物理BIT不同，不能声称原版BIT本身 |
| M：banked S | 16条共享PLRU | 两bank、同一TL/PTW、各512拍DMA0/1 FIFO | 是付出结构开销后的S对照，明确命名banked S |
| M：N | 同banked S | 同容量，只开合格命中旁路 | 与banked S同BIT/boot配对 |
| M：P | 仍总16条，保护DID分2条、其余14条，局部PLRU历史 | 同banked S；N关闭 | 与同结构S/N/PN比较，不能把512拍缓冲收益归给容量分区 |
| M：PN | 同P | 同结构且N开启 | 与P的负向/零向结果全部保留 |

历史banked容量并非无限并行：全局`NumOutstandingTrans=8`、`WFifoDepth=32`按两bank分为4/16，
DDTC仍8条、IOTLB仍16条；每bank最多一个翻译生命周期，normal慢路径和fast路径共享响应口，
不是两个PTW。输入demux另有`MaxTrans=2*8=16`跟踪器；不能把这个16写成翻译并行度或宣称所有额外库存为0。
DMA0/1的FIFO由3×8=24字节增到512×8=4096字节，每lane增加4072字节数据容量，
两lane共8144字节，另有指针/metadata/两bank FIFO等开销。其余DMA不启用本有限方法合同。
两bank按不可变原AXI ID位选择，不能用DID充当生命周期tag；回包ID/每ID顺序必须保持。
banked MRIF不支持，历史实例使用MSI_BT_ONLY、ATS关闭；原版MSI_BT_MRIF与此差异也必须列明。

Full-copy permission只对一个真实接受且尚未退休、源/目的页对齐、长度恰4096、容量至少4096的普通copy有效。
未知重叠/无前驱退休会撤销许可；没有许可时保留原non-owner read admission。
禁止为了得到速度而去掉读准入、伪造permission、增加翻译tag或采用旧M0介入。
N仅在另一bank的stage1 PTW忙窗口尝试缓存命中：真实DDTC和IOTLB同时合格，Sv39/4KiB/S2 Bare、
无ATS/PRI/PID/priv/debug、权限与MSI排除匹配、候选和DDTP稳定且无失效。
不合格/未命中不accept，回原慢路径；只在真实accept时touch两缓存，fast响应需在背压时稳定。
这不是任意页大小/任意DID并行翻译方法。

P采用P2已修正的2/14局部历史。fill所有者取稳定主PTW请求DID，touch所有者取命中常驻way，
不能使用当前fill mask误更新另一分区历史；全maskS/N需与原PLRU逐拍等价。
仅全quiet接受配置/清IOTLB并递增generation；busy/非法写没有模式/cache/generation副作用。
模式和保护DID应在prepare/warmup前设置，同一试次锁存；排空后恢复S并记录restore结果。

## P4M的合法间隔与完整工作

只移植历史P4M的有限gap语义：非最后一个batch task完成后装载32bit倒计数，
在COUNT再次offer新task前等待GAP；首task不延迟，最后task不插尾gap。
gap=0走原WAIT→COUNT路径；gap并非固定“发起间隔”，实际间隔还含真实服务/握手周期。
STOP在GAP和倒计数到期同拍时优先，不能多发一个任务；已offer且背压的task不得撤回。
active期间配置不可改；非法宽度、wstrb、保留地址写仍报错。

候选只用历史可复现的A gap `0/1000/2000` cycles，即0/20/40µs，V gap固定0。
旧pilot另有500/4000，但本批不扫描它们，不根据新性能样本重新挑最优点；
只有Root在采集前明确更改问题时才另记计划。32bit全范围只作有限边界测试，不排板点。

推荐新有限成本合同为V、A各固定4096个4KiB copy，自然完成后再排空/校验，
warmup256单列；不在V关窗时截断A预算。V窗口仍为真实首V issue到第4096 V done，
只在`(open,close]`计A issue/done和carry。A可能在窗外继续，它的全任务首issue到第4096 done
需独立硬件计时并在A done冻结；同命令软件起止、poll、drain与验证时间另列。
这样每lane完整字节固定16MiB，A whole吞吐分母是A自身完整执行时间，不能拿V窗口去除全部A字节。
若Root选择保持旧background-A/STOP协议以减少移植，则必须明确其A完整工作量随V窗口变化：
记录实际issued/completed、STOP时未决/排空、完整完成字节和对应实测时间；
只能称“本次实际完成工作/速率”，不能声称等量A完整任务。此替代合同不能与固定A自然完成混合配对。

每个接受样本同时保留：V/A计划及实际完整count/bytes、各自硬件elapsed、完整软件elapsed、
同V窗口A实际completed bytes/rate、窗口issue/done/start/end/carry、实际gap占用、HPM原始区间、
数据错误/排空/隔离/taint。窗口守恒按开始在途+窗口issue−窗口done=结束在途，
carry单列；零A窗内完成是合法0+边界，不能删点或重跑。
V改善与A代价按同block真实配对比及各条件Type7描述统计报告，不用W减名义gap冒充独立服务时延，
不把整轮HPM解释成窗口HPM，不将各组median之比说成配对比median。

## 必须先解决的接口冲突

1. 旧P4M OBS7的448..463已被E5 hist256..767覆盖，不能直接移植旧observer decode。
   保留OBS4全部字段与bins；提议在当前10bit索引的768以上放独立capability门控E6 bank。
   一种可审布局是768..783 capability/版本/容量/控制，784..799旧16word窗口口径，
   800..927组合128word控制/原始快照，928..959全任务计时/预算；其余保留0且写拒绝。
   这是待分配布局，不是已冻结接口。可以裁掉不必要快照以降低实测面积，不能谎称其值仍可用。
   E5解析器只验证原512桶；E6解析器另验额外bank并绑定同一sequence/generation。
2. 历史gap d0/d8在8aeb上空闲，但A2已接受BTC3用d0表示columns，d8..f8明确reserved/write→error。
   本批不能把d0当未来共享gap口。候选e0/e8只可在独立E6 gap capability存在时重新定义，
   默认禁用实例保持BTC2/3原保留地址行为；最终地址/版本由Root分配。
3. 旧P4M ioctl0x0d与共同Linux的E4 CAPS_PAGES冲突；旧header绝不能整体覆盖。
   保留现0x06..0x0e全部字节，新的独立E6 request若确有必要由Root分配空号与固定结构；
   仅在现代pair core增加受capability门控的可选参数，原wrapper传默认0、原E2/P0/multi/E4行为保持。
   不通过重用reserved输入给旧调用注入新含义。旧OBS7检查也不能简单改成“版本>=”。

## 可审小批及构建顺序

| 批 | 独占范围与接受证据 | 完成后才允许的动作 |
| --- | --- | --- |
| 0（本批） | 来源/容量/接口/采样合同，无生产改动 | Root决定gap先行或是否仍需M、选A自然完成或旧STOP合同、分配接口 |
| G-RTL | 从6c5/85b独立组件worktree移植最小gap与同窗口/全任务被动计数，保留Depth4 | 真实P0 frontend gap0逐拍等价、STOP/offer、有限边界及16段hist守恒通过，Root读实际diff |
| G-Linux | 从da6共同Linux独立分支增加E6小接口；复用prepare/cache/drain/verify | 实际函数host-MMIO正反例、ABI静态检查、旧48 observer与E4有限回归、目标对象编译通过 |
| G-build | 固定全部完整SHA/source manifest，输出仅独立worktree artifacts | 一次DS0/50MHz实现，WNS≥0、setup/hold/pulse failing0、routing errors0；失败停报，不改约束 |
| M-RTL（Root仍需要时） | 在G共同基座窄移植B1/N1/P2、fullbuffer、quiet配置，默认开关保持原版；不移植无关E3/策略 | 全mask/局部历史、真实双DMA/TL/DS背压、ID/回包/失效/配置/有限权限反例证据，Root读实际diff |
| M-build | 同一M BIT支持banked S/N/P/PN，全模式同容量/同软件，DS0 | 单次50MHz实现与完整资源/时序验收，再排Root板时 |

G有限测试应覆盖gap0与真实P0的issue/done/ID/旧MMIO逐周期一致，0/1/1000/2000/32bit端点，
STOP+到期同拍、COUNT背压、active写、首/尾task、同拍done/issue、窗口开闭carry、完整计时溢出。
E6只允许4KiB，保留P0三层拒零与原zero-length backend负向证据；不修生产backend。
M用真实iDMA backend+banked/TL+DS有限harness，所有S/N/P/PN各有限工作并核真实AR/AW/W/R/B与bytes、
权限/失效回退、accepted-touch一次、响应背压稳定、warmup/排空/数据；不拿自写命中/突发公式替代DUT。
最多固定64个测量任务/role，编译与仿真各有限预算，不启长仿真或机制参数搜索。
历史12.7%板上差额不是这些仿真必须复现的验收条件。

E5已实测LUT166334、register142751、Slice50581/50950（99.28%）；
M增加结构可能不适配现面积/时序。这是本次选择M前的具体实现风险，不能把旧裁剪bit的合格结果沿用。
不为让M通过而撤销已接受被动计数、深度错误或原功能检查；需要裁剪时先交实际范围给Root。
组件完成后分别commit/lock与最小incremental bundle，顶层再归档，不能只保存顶层patch冒充组件历史。

## 必要板点上限候选

优先复用Root已排E4/E3软件切换，所有硬件由Root串行。此处是待审上限，不是执行授权或已冻结计划。

- G：10个block，每block Solo、A8 gap0、A8 gap1000、A8 gap2000，共40点；
  gap0有同BIT锚、Solo用于V slowdown，非零gap仅两点。block轮换三个A条件次序，采集前冻结。
- M若保留：10个block，每block A8 banked S/N/P/PN，共40点，四mode同BIT/boot；
  直接报告同结构配对完成时间/完整A代价，不另加M Solo或全A页数矩阵。
- 总新增上限80点、至多G/M两种新BIT；不重复原角色互换30点，也不重复历史DS50/P4M60/172组合矩阵。
  原版E5现有数据可作跨BIT结构开销背景，不能冒充G/M同BIT配对；
  若论文只需P4M取舍，Root可只选G40点并保留历史P/N/PN负结果，不启动M。

技术错误停止并保留原失败，无自动重试/删慢点/补造。科学上的0进度或PN更慢不触发技术停止。
checkpoint用真实完成区组冻结raw/manifest/records独立快照，明确n/complete=false，
逐试次长表落本任务目录；未完成点不补齐，最终再冻结完整真实样本。
