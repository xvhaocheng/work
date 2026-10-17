# P_shared：独立后批的最小运行时 history 接口提案

2026-10-18 JST。当前已提交的 N 核心 `755cd723b20457418734700fae9e8d6f91d988c8` 及其 top 归档 `06598c3f7270483d3bcd746505a251e0538a19ea` 不含这项修改。本文件只响应最新用户追加的 P_shared 需求，生产 RTL、寄存器、综合均未开始。原 plan/source-map/已接受 P/N 小批按原时间保留。

## 最小生产改动

当前 IOTLB `rtl/translation_logic/rv_iommu_iotlb.sv:355–358` 的 `partition_local_active` 只在编译 P-local、16entries 和固定0003/fffc mask下为1；`:387–393` 以 resident way归属限制 touch。建议只在此 predicate 上加一个条件：

```text
partition_local_active = existing_accepted_predicate &&
                         (!HistoryControlEnable || history_isolate_i)
```

`HistoryControlEnable` 编译默认0，`history_isolate_i` 隔离语义默认1。TL只传这一路到IOTLB，沿用原`PartitionLocalHistory(PolicyEnable)`。不修改 `allocation_mask` / eligible tree / masked victim / fill owner / lookup / tag content / permission / invalidation。共享时只回到已经存在的 `!partition_local_active` 全树touch分支；当前真实 shared-history反例使用的也是这条分支，复用它作reference，不重写PLRU算法。

TL公开输入可保留 `history_isolate_i=1'b1`。内部 cache 端口建议显式连接、不依赖输入默认初始化：N批已遇到 Verilator5.040 对内联默认值cache输入的ATTROF内部故障，去掉内部默认并显式连接后通过。编译`HistoryControlEnable=0`时忽略输入、恒保持原隔离语义，旧实例不连接也不会变成共享行为；新启用实例显式给 `!history_shared_q`。这只涉及工具兼容，不改变reset/default合同。

默认隔离时原P配置与已接受P逐拍相同；S/N fullmask原`existing_predicate=0`，共享控制对它们不起作用。P_shared仍旧 single-controller/原BufferDepth3支路，绝不按bit2选择N/fullbuffer支路。N branch只看bit0，P allocation只看bit1。

## 193控制字与非法写（待Root锁定）

赞同复用index193 bit2=`history_shared`、reset0；低2bit仍S/N/P/PN，其余0。最小已授权公开值建议只允许 **0、1、2、3、6**：

| 实际控制字 | 低2bit | bit2 | 处理/支路 |
| --- | --- | --- | --- |
| 0 | S | 0 | 原single-controller/3beat、fullmask |
| 1 | N | 0 | N bank/fullbuffer、fullmask |
| 2 | P | 0 | 原single-controller/3beat、2/14 local-history |
| 3 | PN | 0 | N bank/fullbuffer、2/14 local-history |
| 6 | P | 1 | 原single-controller/3beat、2/14 shared whole-tree touch |

值4/5无P而要求shared，必须拒绝。值7虽有Pbit，用户当前仅追加P_shared板点，没有PN_shared；建议本轮也拒绝7，避免公开未验证的额外配置。如果Root希望沿“Pbit=1即可”接纳7，需要明确把PN_shared列入有限验证，不能悄悄等同用户已经要求它。

`HistoryControlEnable=0`时仅0..3合法，bit2读回0，写6/7拒绝，不静默清bit接受。使能时允许6。原193写控制继续要求全WSTRB、真正全quiet：DMA/frontend/job/backend、所有AXI/W-owner/response、两ctl/TL/walkers/cache更新和invalidation、原OBS及新capture tracker均无live义务。**同一接受边沿**原子更新全部3bit并invalidate tags/clear PLRU；同值合法写也flush。非法或忙拒绝保持原mode/history/cache/counter/phase不动，沿194既有拟定reject状态报告；194只增加一个常量`history_control_supported`能力bit（具体bit仍Root统一冻结），不新增地址/observer/身份。

后续输出应同时保留193原始word与decoded低2bit/history_shared，不能把word6截成mode2后丢掉历史触碰政策。重置S仍原word0，S15仍无MODE/GAP写、无额外flush，原CLI路径/原参数不变。

## 资源、时序与接受条件

新增运行时状态只有控制字的一个FF（以及既有读回mux/合法性比较），没有第二棵PLRU、FIFO或cache条目。共享状态只影响hit→PLRU D更新条件，不改AR/R/W/B ready/valid、TL candidate gate、P mask/victim组合路径；但该控制位会扇出到多层PLRU更新选择，可能落在lookup→PLRU FF关键路径，因此不能凭RTL行数宣称时序无代价。

最小评估顺序：独立小批只IOTLB/TL参数+predicate、真实DUT有限证据；后续wrapper控制批才新增193 bit2/194常量/flush。无完整build前，直接审展开逻辑和实际有限回执：

1. 编译关闭以及启用+`history_isolate=1`逐拍对照当前P-local，包含tags/content/q/n PLRU/victim、clear/同拍fill-touch；fullmask继续raw85b原gold。使能/关闭参数不改S数据路径。
2. `history_isolate=0`逐拍对照已存在的whole-tree shared参考：mask持续0003/fffc、fill不越区、真实Vtouch改变共享A历史反例可达；resident-way局部归属在隔离模式保持；N probe仍仅trueaccept touch。
3. 控制批另核0/1/2/3/6全quiet读回，0↔6及2↔6、同值flush，busy/非法值/部分WSTRB拒绝不变、无旧事务跨模式；禁编译时6拒绝/bit2读0。这里不在N批伪造已完成MMIO证明。

若展开审查已有明确高扇出/关键路径风险，采用编译关闭fallback，只保留已接受P-local。本任务仍只安排一次DS0首构；若含开关，沿既定50MHz约束、WNS>=0/setup/hold/pulse失败端点0/routing0，至多一次历史post-route物理优化。失败即停，不为开关反复build、改约束或降低频率；fallback不能冒充该失败版本已timing通过。实际是否采用开关由Root在完整build前据diff/有限证据决定。

## 最新独立样本数

包含P_shared时，总数 **235**：S15准入15 + 原S/N/P/PN主矩阵120 + 新P_shared×A1/A8/A64×10=30 + P/PN A8 gap0/1000/4000各5共30 + DS50 A8四模式各10共40。各phase独立、不复用gap0或S15试次。DS0主矩阵所有gap0；Vgap仍仅独立最小cooldown。DS50不追加P_shared点。若明确放弃runtime history开关，则隔离only计划仍205，P_shared30点标未运行/原因，不补齐或推造数据。

共同硬门不变：DS0 routed合格后先S Solo/A1/A8各5；逐cell相对统一DS0硬件median绝对偏差<=0.5%，不合格即停；**只有S15实际板测通过才允许同源DS50构建**。此文没有任何板测n或面积/时序结果。
