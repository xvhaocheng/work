# 映射再用机会裁决：正常再用可达，现成事件只支持有限映射出现证据

2026-10-16 JST。**MAP-REUSE-OPPORTUNITY-1已完成来源、观测、配对与CPU费用合同。** 最重要的新事实是：固定mlx5e native-XDP **cyclic、linear、order0子切面**后，每次RX allocation是一页一个packet位置；B映射重复须跨实际recycle/再次alloc，不能靠同次分配中的小包packing制造。源码支持正常再用和正常受限两类事件顺序，但尚无匹配的再用频率或CPU费用记录。

当前保留V1-N/K为低成本整合假设。**没有得到净收益、独立创新或共享IOMMU干扰实证；也没有把资料不足裁成思想族失败。** O3同付费接口等价仍只判策略增量，普通R为本轮构造而非已发表网络实现。当前不开发驱动、采集系统、RTL、状态/性能模型或板测。

## 1. 正常问题来源比上一轮更具体

| 已核事实 | 来源与实际含义 | 边界 |
| --- | --- | --- |
| XDP linear stride不小于PAGE_SIZE；当前cyclic线性子切面恰为PAGE_SIZE、num_frags=1、mask=0 | `en/params.c:256–281,296–317,693–707`，经`en_main.c:911–912,604–640`连到实际页分区 | 是本轮选择的源码支持子切面，不声称每个部署都用cyclic；不混striding/multibuffer/其他driver |
| 正常refill每小批先free旧WQE，再alloc新WQE | `en_rx.c:457–478`；cache从栈顶取页`page_pool.c:422–436` | B consumer先归还、driver再drain bias且p为该小批最后成功direct入cache页时，紧接alloc可取p；是条件偏序，不是频率/任意gap |
| consumer尚未归还时，drain driver bias不使页可回pool | `en_rx.c:274–302`、`helpers.h:245–323`、`page_pool.c:866–873` | source需用别的cache/ring页或慢分配；不一定新增一页，也不证明p以后不再用 |
| 跨CPU回流不等于source退出；ring满、实际NUMA变化等可结束source代际 | `page_pool.c:370–419,884–902,1139–1150`及RX正常NAPI连线 | 跨CPU不等于跨NUMA；退出原因存在不证明谁占主导 |
| pool_size是回流ring容量，非全部source物理页上限 | `page_pool.c:194–210`及pool类型/driver参数 | 不能用cache+ring容量直接推出live页数、复用次数或候选K |

[source机会分析](../reuse/source-opportunity.md)给出完整正向/限制序列、bias责任和r的分层计数。普通source代际g跨正常recycle不更换；真正离pool后，即使同PFN被再次分配，也必须另记g。B CQ还帧、bulk flush、last-pp-ref、recycle、source退出、buddy free继续分开。

此配置纠正了一个潜在高估：小packet不意味着native RX同页装多个packet；不能把generic fragment支持当本切面额外B再用。该事实也可供copy/紧凑池比较复用，但本轮不自动展开其他候选或据此推总RAM节省。

## 2. 现成事件能取得什么

新取官方v6.12固定commit的三份trace定义，原有`kernel/dma/mapping.c`复用；没有开始采集。[观测合同](../observation/observation-contract.md)和[字段矩阵](../observation/event-matrix.json)列出逐字段、触发点与缺口。

| 层级 | 现成来源 | 当前可支持范围 |
| --- | --- | --- |
| **L0：同source代际内B map出现** | pool hold/release含pool、netmem/PFN与全局序数；DMA map含dev_name、phys/offset、DMAaddr、size/dir/attrs | 在epoch、配置、完整性及本链来源已确定时，可数B映射出现/成功；不是全尝试覆盖，也不是接受/CQ |
| **L1：逐frame真正接受** | redirect含路由/错误，devmap_xmit含每bulk sent/drops/err | 无frame/page/SQ稳定关联；不能把相邻map或batch sent自动配成逐frame接受 |
| **L2：CQ／清理、最后引用及退出配对** | 实际driver有CQ、批归还、pp最后门；pool release可结束owner episode | 现成这些事件缺逐frame/WQE/CQE和正常last-ref关联；CQ reclaim/错误/stop cleanup、有效工作结果须分别记 |

**合并几何与L0后的最小粗筛推论：** 若已证明本切面一allocation一packet、单次FIB到B、无clone/软件重发，且两条成功B map确属该调用链和同一完整source/config episode，那么第二条map就蕴含至少一次跨recycle/再次alloc。可先利用合格现成L0记录筛映射调用的重复机会，**不必先开发完整逐alloc/CQ采集器**。该结论不成立于来源混杂、重复记录、跨epoch误合并或同allocation可多map的其他切面；也不证明两帧被接受或完成。若连这些条件都未证，只有地址重复而不能下此推论。

hold是新页加入pool的marker，bulk中可尚未交RX；cache再次alloc/recycle不发新hold。release在A unmap之后、clear/put之前，不保证物理最后free。计数是pool全局u32序数，不是永久page epoch。PFN、pool地址、dev_name、mem_id都可能复用；缺开始/结尾的episode分别作left/right-censored。

零丢事件也未必能按IOVA相邻配对：unmap backend已归还IOVA但未发post事件，另一CPU新map可先复用地址并发事件；旧unmap随后出现会使简单活跃表误配。无call-id或被证的无歧义顺序时必须隔离ambiguous。该风险不阻止用每条map自身phys做合格L0计数，却阻止把它升级成完整生命周期。

## 3. CPU费用与条件机会界

root直接核了page API：`mapping.c:155–198`的map/unmap事件均在backend之后；map事件后还做debug。**两个post标记的间距不是map/unmap CPU费用**，其中可含SQ、设备/队列等待、CQ调度和unmap，反而漏掉前次map backend费用。SG unmap在backend前，不能混用事件侧。

[CPU与机会合同](cpu-and-opportunity-contract.md)固定了driver调用点与较窄page API边界、entry/return调用ID、失败早退、inclusive/child去重、CPU/墙钟/抢占区别以及初始化/worker/Q/尾部费用。function graph的具体口径也受选项和丢失影响，不能由开启tracer推CPU准确性；依据为[官方6.12 ftrace说明](https://docs.kernel.org/6.12/trace/ftrace.html)。本轮未启动tracer。

对配置及source代际的完整交集g，若T合法、成功map正确闭合、忽略K和全部新增成本，来源O0的重复配对机会最多为`Σ max(r_map,g−1,0)`。这是**理想化调用对数量界**，不是CPU、在线策略输入或候选闭环结果。源码只给`r_try≤实际alloc rounds`等条件关系，不给有意义的非零频率下界；数值仍unknown。

只移除B map/unmap且其余路径冻结时，直接CPU节省不超过这些调用的实际CPU费用；当前该费用及总CPU均未测。真实source供给/回收、同步、lookup/prepare、失败回退、关闭/Q和其余路径变化另计，不把此直接预算界当端到端吞吐或所有系统效果上界。若策略改变分配/Q，O0 PFN序列只作O0来源机会，不能冒充候选真实序列。

## 4. 已有材料与本机可达性

本轮只核明确列出的资产：前轮mapping-integration与lifetime材料是源码/条件设计；mapping-admission的有限账是说明PMD输入与算术，不是原生网络事件/费用。没有重跑它，也不把其数字转为本轮数据。这个范围内没有提供匹配的原始native记录；**没有声称全仓、所有论文或远端机器都不存在记录**。

root另做一次[只读host身份核对](host-readonly-identity.json)：当前release为`6.8.0-60-generic`、x86_64，物理netdev驱动e1000e/cdc_ncm，观察到的mlx5物理netdev数为0。两处tracefs metadata读取EACCES。它不足以承接固定v6.12双mlx5e链的原生费用采集，**EACCES不等于内核没有事件，网卡/IOMMU group不证明translated domain或共享服务**。没有sudo、挂载、启用trace、附着probe、发流量或改设备配置。

因此，本轮交付的是可取得性明确的证据合同和来源条件界，不是原生性能结论。没有要求用户再确认一般授权、CPU/内存论文价值或先给保证成立痛点；当前硬件缺匹配身份也不阻止有范围的条件研究。

## 5. 裁决与后继

**保留V1-N/K，停止重复相同原语表、阈值例子和无输入数字账。** 当前正向证据是具体normal recycle机会、实际配置边界和可复用L0观测基础；关键未知是同身份正常再用频率、可归属完整CPU费用、K覆盖/退出/Q及完整服务资源后果。stock无idle B pin，相对stock仍无天然payload RAM节省主张。

下一[MAP-EVIDENCE-INPUT-1](next-minimal-contract.json)从合同转向**已有匹配原始输入的定向定位/验收**：优先检查已有公开或已授权来源的有限记录及构建/设备配置，不能用更多相同合同代替输入，也不自动启动新采集/开发。若没有匹配输入，保留条件界并明确这个证据状态；这不自动关闭映射组织族或升位其他方向。独立贡献还需相对具名普通方案的实际能力/成本增量；L0重复成立本身不证明新意。

B1/I/D/A5/A2及独立工程、MIG1792023314 paused和10月15—28日窗口保持；未选论文主线。
