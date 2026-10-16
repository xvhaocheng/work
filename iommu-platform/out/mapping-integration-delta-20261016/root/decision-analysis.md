# 局部整合差项裁决：保留V1-N/K，取消非必需的逐TX关联责任

2026-10-16 JST。OWNER-BINDING-INTEGRATION-DELTA-1的字段、调用、锁和退出责任比较已完成。**当前最值得保留的具体设计是V1-N/K：固定A→B和规定权限，K个预留关联槽，首次使用建图，槽满走普通短map；空闲关联不pin页，不提供独立idle eviction，仅在源真正退出或B停新并排空后撤图。** 这使既有frame/全部片段引用可保护在途使用，不再强制每TX token或完成记录中的binding handle。

这是本轮获得的设计简化与可执行比较，未实现驱动或证明性能。**未确认独立论文贡献；低成本源退出接口与原生路径整合的研究空间保留。** O3使用同一付费接口可完全等价，只判策略增量0。具名普通构造的等价也不证明它已经发表、实现或拥有相同实测完整成本。

## 1. 比上轮具体前进了什么

| 上轮保留项 | 本轮定向核查后的具体变化 | 证据与边界 |
| --- | --- | --- |
| 每次B使用持独立binding token | 无独立eviction、固定配置且撤图只在owner退出/目标drain时，可用正常frame/pp_ref保护；通用G版才保留token | `helpers.h:227–323`、`page_pool.c:866–873`、`en_rx.c:274–302`；一页可有多个frame，不能单帧CQ就撤图 |
| 每源页预建owner wrapper | pool级hook/ctx＋K个lazy sparse记录；未接纳页无独立wrapper，退出hook查无关联就原路结束 | 设计投影，需索引出版/失败/CPU回收；普通适配获得同一优化 |
| 完成FIFO三槽换handle | 窄版CQ只归frame，不读binding；拟议持久mode可只保留mode＋xdpf两个逻辑项 | stock `xdp.c:673–700,925–931`；未编译，不能称结构字节或缓存行减少 |
| dma-buf可能每page新对象 | DB-SLOT-K允许K个固定PAGE_SIZE对象预export/attach并正确重绑，或仅需求页lazy export；span进一步聚合 | 原文确证export/attach对象及睡眠上下文；slot/range网络适配是条件构造，非现成driver |
| 首miss、退出是否必须睡眠/Q未区分 | 标准dma-buf map/unmap入口有might_sleep；直接DMA API在既有NAPI/return位置已有同前端依据；target关闭竞争仍可能要Q | 不是所有backend时限或新锁协议证明；标准框架热cache不必调用这些睡眠入口 |

详细的[普通owner适配表](../owner/integration-analysis.md)、[dma-buf适配表](../dmabuf/adapter-analysis.md)均区分实际代码和条件设计。原文来自已存官方v6.12 `adc218676eef25575469234709c2d87185ca223a`与独立DMA_PMD RFC；没有把两者拼成可运行内核。

## 2. 各方案到底需要什么

以下名字是本轮可审查构造，不是假称已有同名系统。T仅条件部署投影，必须授权完整实际granule及同owner期间后续用途；S逐对象撤权另列，不能混用。固定当前单buffer普通page-backed路径；保留真实fragment bias和最后引用。

| 阶段 | V1-N/K与普通R-OB4K-T-N共同部分 | 标准DB-SLOT-K／SPAN，及私有range适配差项 |
| --- | --- | --- |
| 控制面/source join | pool cookie、release hook、source索引、B可枚举集合、K槽及退出节点；不覆盖A的pp字段 | 使用export有anon file/dma_buf/resv，attachment为对象×目标关系；允许slot重用/span/共享resv，不要求用户fd或每包引用 |
| 第一次B需求 | 当前frame保护源页；claim并登记CREATING，DMA API建图，地址先于LIVE出版；冲突/不足合法O0 | 标准core map不直接放NAPI；预备好则命中，否则O0当前工作＋worker prepare，或合法copy/defer。worker与源退出/配置竞争须计费 |
| 热命中/提交 | 读ready/DMA base，按当前offset/长度同步；原SQ接受条件照旧；不预测接受结果 | 自有cache可走同样热路径；不可把所有hot hit强制收费为core map。write fences必须服从；不能立即禁用的访问原文是should设fence，不强收窄版每TX fence |
| 失败/CQ | 当前责任结束前保留原frame；持久mode CQ只return frame，之后不再使用record；O0 mode正常unmap | 合法同表示；不强制每包file引用/SG分配/token或额外FIFO项；标准SG map/unmap必须正确配对 |
| source真退出 | 先关源门/找关联，完成旧B撤图，再release_cnt/清pp_info/put；不等pool最终销毁 | 标准unmap在可睡worker，softirq退出要正确Q；DB-PAGE对象级notify精确到slot，SPAN标准通知是整对象。私有range可局部处理，但需新增协议 |
| B关闭 | 禁新并排CPU发布者、真实DMA drain、唯一unmapper、配置寿命及CPU metadata回收 | 同义务；标准detach/file销毁不替代source真实退出；不能拿get_device、RCU或move_notify返回冒充DMA完成 |

**R-OB4K-T-N**把DMA_PMD的owner→domain追踪和最后free撤图思想适配到普通page_pool，改用现有按页DMA API。它不免费继承PMD物理池、整区IOVA仿射窗口、RW或coherent假设；同样支付hook、K索引、地址记录、权限/同步和关闭。按本轮具体构造，未找出V1必须少做而R必须多做的原语。这个结果约束“上述组织能力独占”主张，不推出其已在网络上实现，也不推出所有实现CPU相同。

**DB-SPAN-RANGE**允许普通方案自建range_acquire/map/revoke及部分slot状态，但不是标准dma-buf partial API：immutable size、完整backing/SG、整对象notify及同步合同仍需履行；带洞的单页表不能冒充完整span映射。若绕过core映射使用私有DMA API，应准确记为新增付费range适配。不能一面给它V1同等接口，一面把该接口当作前作已发表的能力。

## 3. 执行上下文与关闭的决定性边界

root直接核了`dma-mapping.h:380–396`：single map调用page map；single unmap调用page unmap。B原CQ `xdp.c:683–684`、A原source return `page_pool.c:661–665`已经用这些接口。因此在有效同backend、无关闭竞争及新增等待锁的稳定B分支，源退出同步B unmap具有现有调用位置依据，不应一概先加worker/Q。实际大小、attrs、同步、映射错误和时限仍需满足合同；无硬件结果。

标准dma-buf `dma-buf.c:1118,1221`的might_sleep位于入口，trylock不改变该合同；它可把控制面准备和退出交worker，热hit用自有cache。这是具体执行位置差异，**不是V1独有能力证据**：普通R及私有range cache同样可直接使用DMA API。

B实际禁XDP→同步NAPI→关channel/SQ的顺序已定向核到`en_main.c:3197–3216,3403–3418,2077–2086`和`xdp.h:138–150`。真实FW/DMA停止链没有新增核读，所以不把synchronize_net当硬件排空证明。

root给出一个明确的关闭反例：target先claim唯一unmapper再等CPU grace，source在该读侧内退出并等待target完成，形成自等待环。处理必须排好禁新/reader drain/claim顺序或安全Q；不能给任一方案“先free、后台补unmap”。[关闭与K库存合同](close-and-capacity-contract.md)固定责任和条件反例，未运行状态模型或证明并发实现。

## 4. 完整费用与资源归因

设实际已接纳记录数R≤K，所有CREATING/LIVE/CLOSING/Q共用该预算，撤图和必要回收结束才返slot。每slot最多对应一个order0源页，因此关联Q的库存≤K页；**不代表总RAM差≤K页或有界排空时间**。CPU侧读取、索引、prepare失败、短map fallback、每次sync、关闭/worker/回收全部计入，不能只数省下的map调用。

标准DB-SLOT-K可只建K个可重用wrapper和目标关系，不能按每packet、每源页总数P或每generation重复export收费。SPAN减少wrapper数，同时要付backing/slot/range维护；实际sizeof、配置统计和争用时长未测。没有凭原语数量推吞吐。

物理payload继续按本策略实际`S∪P∪C∪Q`去重，元数据/PTE/IOVA另列。**相对stock O0，V1不天然省payload RAM**：stock也不让idle B cache长期pin；避免pin所省的替代分配只可与真的pin-cache比较。V1-N与普通R可P=0，Q仍可能非0。若copy fallback，目标C和copy带宽另记；早归原frame不能直接称省总RAM。各策略自己导出真实recycle/allocation，不锁同一PFN再用序列。

源池原有保留、B延后退出和替代分配的归因仍按上轮规则；相同工作/服务下CPU或不可回收内存都可作主要贡献，不再等待用户抽象认可。当前没有实际成本/收益数字。

## 5. 一段可核查的贡献论证与本轮裁决

最接近的DMA_PMD已具owner持域映射状态、首次建图和最后free撤图，dma-buf动态attachment已有owner通知与无条件长期pin的替代；原生page_pool本身也已对A复用映射。当前具体推进是：把普通基础页source退出、固定B配置和既有frame/片段责任组合为一个K有界、可原子首接纳、无逐TX独立token的原生路径整合合同，并准确处理失败与关闭。其意义可能是减少逐包map/unmap及通用框架维护，保持既有source布局/消费者；但普通R的具体构造可覆盖同能力，合理dma-buf适配可避掉许多臆定收费。本轮没有证实独立方法或完整成本优势，**保留其作为低成本接口/实现候选，不认领独占组织或新算法，也不据普通构造关闭思想族**。若后续同有效工作/服务下真实整合数据证明CPU/资源或部署价值，才据该具体增量推进论文；不能以缺源码接口或几个新字段代替证据。

决定性预测仍可检验：T合法、固定B、无独立eviction、所有fragment持至CQ/drain时，B图可跨正常source recycle，hot/CQ无新增token，真正source退出前撤图。普通R同享预测；预测成功只验证可行性。加入独立idle eviction或活跃期换domain却不补使用保护，已有另一个fragment在DMA时就可能撤图，是相邻失败条件。高再用/低退出才可能摊销，来源分布未知不包装为已有胜例。

## 6. 下一最小动作

当前字段/调用/锁/责任任务已完成，不再重列同表或补旧累计阈值数字。下一转为[MAP-REUSE-OPPORTUNITY-1](next-minimal-contract.json)：只在这一固定链上说明什么正常来源会产生可复用mapping、哪些事件能在合法观察时机得到、完整CPU费用如何与真正的source退出配对，形成可取得的证据合同及条件盈亏范围。它不要求用户先给保证成立痛点，也不要求先有原生胜例；没有原生身份时明确保留条件分析，不用M0/PMD说明数字冒充网络结果。只有具体证据需求和可达身份成立后再决定最小采集/实现，当前不自动启动。

本轮零新下载、零新模型/旧阈值账、零原生/构建/驱动RTL/设备操作。B1/I/D/A5/A2、独立工程事项、MIG暂停及10月15—28日窗口保持，仍未选论文主线。
