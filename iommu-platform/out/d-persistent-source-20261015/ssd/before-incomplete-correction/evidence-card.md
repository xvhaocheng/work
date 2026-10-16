# MQSim：写历史留下的FTL状态与有限GC窗口

2026-10-15，ARS deep-research / fact-check，source_verification 单阶段；共享研究 Codex-only。仅核 MQSim 官方源码与 FAST18 作者原文，不做跨方向新意排名。

**结论：找到实际公开实现的持续服务机制。** 正常覆盖写使旧页失效并消耗空闲块；低空闲块触发独立的搬页读、搬页写、擦除和地址屏障；这些内部活动与主机请求完成是不同状态机。它支持“主机命令已完成不等于SSD内部维修已完成”这一来源明确的候选机制，但本次未运行，尚无“全部主机工作已完成且GC仍活动”的实际轨迹，也没有同完整主机历史的信息缺口、真实设备收益或P/PN收益证明。

固定源码为 [CMU-SAFARI/MQSim commit 51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16](https://github.com/CMU-SAFARI/MQSim/commit/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16)，提交日期2025-10-09。选读12个源码/配置/README文件；原字节、SHA256、Git blob校验、URL和阅读行段见 [sources.json](sources.json)。代码证据对应这个公开模拟器，不代表商用SSD固件。

| 核查点 | 源码事实和固定行段 | 判定边界 |
|---|---|---|
| 自然状态生成 | `Address_Mapping_Unit_Page_Level.cpp:1146–1195`：覆盖写先将old PPA标invalid，再分配新页；`Flash_Block_Manager.cpp:20–36,111–120`：valid/free/invalid计数与写frontier更新 | 从普通写请求生成，不需要按目标PTW相位注入债务 |
| GC触发 | `Flash_Block_Manager.cpp:29–33,49–52,101–105`；`GC_and_WL_Unit_Base.cpp:21–29`将阈值乘**块数**；`GC_and_WL_Unit_Page_Level.cpp:43–50,142–150`检查free block pool、有效候选和并发上限后设屏障 | README说free pages，但决定实现为free **blocks**；无invalid页时该候选返回，不可只因空间满就强造GC |
| 独立维护事务 | `GC_and_WL_Unit_Page_Level.cpp:157–188`：GC_WL事务`UserIORequest`为空，搬页依赖和erase单独排队；`GC_and_WL_Unit_Base.cpp:105–165`：读完成后分配新页并提交搬页写，写完成解屏障，erase完成才清ongoing并归还free pool | 内部维修不是未完成主机命令的另一名称；但是否跨越全主机排空边界要用实际轨迹确认 |
| 后续读供给路径 | `TSU_OutofOrder.cpp:162–201,227–329,404–417`：GC/user/mapping分队列，共用chip/channel；urgent GC可先于user读，busy program/erase可阻塞读；`TSU_Base.cpp:34–65`由NAND idle信号继续调度；`Host_Interface_NVMe.cpp:98–140,372–375`：读被服务后发送payload | 因果路径是控制器内部排队/flash完成→host payload发送变迟，不能直接称为IOMMU竞争或译前供给已测变化 |
| 主机完成不全局排GC | `Host_Interface_NVMe.cpp:98–141`减少On_the_fly、发送读数据、处理CQ并发CQE，未检查GC队列；`GC_and_WL_Unit_Base.cpp:153–165`仅由erase完成归还块 | 源码结构证明两条件不同；“本机某次host outstanding=0且GC>0”未执行验证 |
| 纯读限制 | `Address_Mapping_Unit_Page_Level.cpp:588–604`：已分配页的读走正常读，不走覆盖写；但未分配LPA有`online_create_entry_for_reads`，其`1381–1383`会分配user-write页 | 纯读已有数据不会自动生出写历史GC。未写LPA的模拟器补建映射、历史dirty CMT写回与真实写入必须分开 |
| 初始化/复位 | `SSD_Device.cpp:114,185,273,305`构造FTL/块管理/AMU/GC；`401–417`可选precondition与cache warmup；`FTL.cpp:47–49`开启预处理 | 没有核得主机reset/power-cycle/sanitize的完整语义；新建模拟器对象不是现实掉电复位实验，不声明状态跨掉电存活 |
| 可观测性 | `FTL.cpp:755–888`输出累计GC次数、平均搬页数、CMT hit/miss；`TSU_OutofOrder.cpp:89–137`有各队列统计；README:56–57有epoch响应时间日志 | 这些是模拟器内部/结束报告，不能当作真实主机已有的即时GC遥测接口，更不能默认免费 |

**quiet必须写清是哪一层。** 主机quiet至少要有所有命令完成、SQ/CQ处理与host payload/CQE DMA排空，而不是只看`On_the_fly_requests`（该计数在发送读数据之前就减）。这仍可容许内部GC活动。若quiet进一步要求GC队列/搬页/擦除/映射写回/dirty destage全部排空，则活动GC窗口结束；free/invalid分布与LPA→PPA布局仍存留，但不能给之后的只读阶段凭空延长GC。此时后续写可以再次消耗资源并触发GC；后续只读的差异须另证CMT、数据布局等具体路径。

论文来源：[Tavakkol et al., FAST18, MQSim](https://people.inf.ethz.ch/omutlu/pub/MQSim-SSD-simulation-framework_fast18.pdf)，定向读§2.3、§3.2、§4.2.3/4.3和Appendix A.1/A.2；p7/p16渲染核对，未声称全文精读。§4.2.3说明模拟器实现GC与调度；A.1/A.2报告四块2016–2017年设备的预处理和响应时间对照。它支持该模拟器的校准背景，**不验证当前commit、idle后GC残留、同H0反号或新增观测价值**。作者自验证与代码同一家族，不算独立复现；设备未公开精确型号。这一来源范围内无伪造/撤稿迹象核实，未做出版诚信数据库全面排查。

**成本只能按实现计，不再造冷却单位。** 当前样例`ssdconfig.xml:52–61`给read=75,000、program=750,000、erase=3,800,000以及suspend费用；README定义为ns。这是示例模拟参数，非本机SSD测量。一次有v个valid页的非copyback GC产生v个读、v个搬页写、1次擦除，另含映射/屏障/排队与通道成本；并行使总时间不能直接把所有延迟相加。新遥测成本、PCIe/CPU读取成本和观测新鲜度均unknown。

**实现限制必须保留。** 此卡的调度细节限定`OUT_OF_ORDER`（`SSD_Device.cpp:139–150`有真实选择分支）；仓库样例31行选的是`PRIORITY_OUT_OF_ORDER`，本次未核该策略，不把二者混同。AMU:1770假设反向LPA信息可DRAM直取；1805–1814允许屏障后的请求借用搬页数据并直接完成。这些简化影响成本和时延，不能拿内部oracle读出当现实已有接口。busy-chip suspend分支存在fall-through，当前卡不以其实际可用性作正例。
