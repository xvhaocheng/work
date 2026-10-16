# 有界来源裁决：保留机制，下一步只验可达边界

**来源准入建议：YES，作为公开模拟器中实际实现的FTL/GC机制；性能/信息价值准入：UNKNOWN。** 此判断只给root一个候选事实卡，不替root选择D路线或声称新意。

最强可信版本：固定正常写工作负载在已分配数据上进行覆盖写，自然产生无效页和空闲块压力；在所有host命令/hostDMA完成后的有限时间窗，若真实执行仍有内部GC搬页/擦除，它可改变下一普通读的payload供应时间。写历史、quiet时长和下一请求均事先固定；不能观察GC/目标PTW相位后挑选起点或调参追正例。

来源直接确定的是状态生成、独立内部队列、维修完成清理、资源与请求供给调用链。以下均还缺：全主机排空后残留活动GC的可达轨迹；同一完整H0下可达的不同内部状态；新增Z是否区别于普通读延迟/历史推断；Z的现实读取费用；相对普通等待/QD/pacing/试选的收益；共享IOMMU的P/PN动作如何进入这个因果链。公开初态、固定seed、完整调度与命令历史若可重建全部状态，模拟器不能自动提供“隐藏信息”。只有未观测且来源真实的初始布局/其他租户/控制器随机执行等造成同H0多状态时才有信息问题；本次未证明此条件。

## 必须包含的普通可执行对照

| 对照 | 要解释的替代原因与完整成本 |
|---|---|
| 持久host DMA映射、普通P固定模式 | host IOVA保持已映射可消除/减少翻译压力；要计pin资源和占用寿命，不因SSD有GC就假定新增IOMMU动作有效 |
| QD上限、按完成补发、固定pacing | 已可改变控制器占用与GC机会；计吞吐牺牲、host CPU、完整完成时间 |
| 普通等待/idle drain、等时间P | 有限GC本来会完成；新增动作配置或Z读取期间也在维修，必须匹配同样等待时间 |
| 固定小读探测及近期读延迟/主机完整历史推断 | 可能已揭示服务慢态；计探测读、CPU、数据/映射cache扰动和由等待带来的状态变化 |
| 该控制器已提供的遥测 | 本次只核到MQSim内部统计/日志；真实设备的SMART/vendor日志含不含及时GC信息及费用未核，不能假定已有或没有 |
| controller内部调度/资源分区 | MQSim已有user/GC调度、CMT与channel/chip资源配置，需区分可部署控制与仅模型选项；不能把新增接口缺项当创新本身 |

## 明确否证条件

1. 固定正常工作负载里，host命令完成且hostDMA真正排空后始终无内部GC活动，则“这一工作负载的残留GC窗口”版本不成立；FTL持久状态本身仍保留。
2. 一旦GC/dirty destage全面排空，纯读既有映射无新增维护，而所谓残留影响消失，就不能把此前影响说成跨任意quiet的持续debt。
3. 相同完整H0只能到达同一状态，或可用便宜的完成历史/探测/已有遥测等价预测，就不能把新增Z提供独有信息作为该版本贡献。
4. 等时间普通P等待或固定QD/pacing达到全部完成成本同样或更好，且N无成功/无独有动作效果，则停该收益主张；不由单个控制否定全部架构思想。
5. 残留GC与目标访问不同物理资源、cache已命中，或全部翻译长期命中，因果链相应截断；不得把device latency变化直接升级为共享IOMMU收益。

## 最小后续执行检查（仅建议，未实施/未运行）

在固定commit的MQSim内，选择已核的PAGE_LEVEL + OUT_OF_ORDER，预登记工作负载、seed和完整资源配置。先完成正常数据填充/覆盖写；下一只读访问限定已写LPA，避免`online_create_entry_for_reads`补建捷径。禁止按目标PTW相位注入状态，禁止把自动precondition生成分布当作运行中观测。

只添加只读事件记录接口，不改调度：记录host request/CQE/payload完成、所有host queue与PCIe in-flight、GC队列/进行中搬页写/erase/locked LPA、dirty cache/mapping写回和chip/channel busy。分别在host全面排空、固定idle后和内部全面排空三个边界记完整状态；普通下一读记录command、内部read排队/issue/finish、payload DMA边界。最先只判“残留活动存在且进入后续读因果链”能否成立；不运行旧CS1，不添加Z选策，不急于接共享IOMMU。

若成立，保存完整自然历史、原始事件和代码hash，再由root决定是否值得比较同H0可达状态与等时间等待等普通控制。若不成立，保留真实FTL状态事实，按上述具体范围停止该有限GC窗口版本。不能把zero-cost内部oracle当候选接口，不能将配置数值冒充真实器件成本。

## 执行与来源账本

- 实际：读取root/platform AGENTS、CURRENT_STATUS与上一轮decision-analysis；使用ARS fact-check/source-verification及PDF定向阅读。官方repo README/tree定位后，源码按3件以内小批获取；没有clone，没有全树正文扫描。
- 留存：12个固定ref源码/配置/README，均核Git blob与API fixed-ref tree记录；1份作者PDF及SHA256；README初从master下载，随后以fixed-ref tree的blob校验绑定。完整来源见`sources.json`。
- 获取异常：USENIX网页/PDF浏览器访问成功，终端下载403；改取作者ETH原文，未循环重试。一次tree校验误以commit-response的sha为tree-object sha而触发本地AssertionError，已按API实际返回字段分列并对各blob核实；不隐去两字段差异。一次可选fitz模块不存在，改用已装Poppler抽取/渲染，无安装。
- 未做：0 simulator build/run、0真实设备操作、0旧实验恢复、0RTL/Linux/ABI改动、0外部模型。当前主canonical文档由root统一更新，本席只写`ssd/`。
- 请求模型/强度为Sol/high；工具未向本席提供可核的实际运行元数据，两字段在账本记unknown，不以请求配置冒充实运行。
- 来源质量：代码是公开实现的直接证据（ARS VII描述性原始实现，不是人群试验评级）；论文是作者的非随机受控器件比较（III），不覆盖本文候选因果命题；来源并非独立交叉验证，硬件型号、当前commit验证和商业固件状态均未核。
