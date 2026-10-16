# MQSim LPA 屏障释放：最小私有修复合同

2026-10-15；任务范围仅为机制设计与源码核查。没有修改 MQSim、构建或运行仿真。

## Material Passport / 裁决边界

- 请求配置：`gpt-6.1-sol / xhigh`，不是 ultra；本席没有可独立核对的 provider/runtime 元数据，实际模型与强度留 root 按运行记录核对。
- 基线：`51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16`。下文 `S/` 为 `/home/xvhaocheng/work/iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/`。
- 原合同：[MQ-IDENTITY-1](../root/identity-contract.json)；四冻结输入及 workload 说明：[MQ-BOUNDARY-1](../../d-persistent-source-20261015/root/boundary-contract.json)。输入、seed、cache、几何和时序均不因修复调整。
- 证据类型：原始源码 + 已冻结输入合同。本席交付前 root 报告单次 identity replay 已将缺失 UID 绑定到屏障删除，且原 XML 与滤去新增 identity 记录后的全部旧 boundary 事件一致；本席直接核对了 [missing-request-events.json](../root/missing-request-events.json) 的8个精确身份事件，未重复运行、未独立重算整份动态账本。
- 本文结论：**对 root 已报告的同一缺失 UID 删除链，可接受的最小候选是：保留原 child，先摘掉该 LPA 的屏障等待索引，解锁，再用局部指针 list 调用正常 `Translate_lpa_to_ppa_and_dispatch`。这只是当前整页写输入上的私有语义变体；是否接受仍取决于实际因果检查。** 不主张上游通用修复、数据字节正确性、论文新意或 P/PN 收益；动态根因的正式裁决与修复有效性由 root 接受记录承担。

## 1. 已直接核对的源码事实

| 决定性位置 | 原始位置与含义 |
|---|---|
| child 创建与 owner | `S/src/ssd/Host_Interface_NVMe.cpp:170-214`：按页分段；WRITE 在 `206-208` 创建 `USERIO` 原 child 并 `push_back` 到 `User_Request::Transaction_list`。`S/src/ssd/User_Request.h:25` 声明此 owner 完成列表。 |
| 初始未映射状态 | `S/src/ssd/NVM_Transaction_Flash_WR.cpp:25-30`、`S/src/ssd/NVM_Transaction_Flash.cpp:6-10`：正常 USERIO WRITE 的 `PPA=NO_PPA`、`RelatedRead=NULL`、`Physical_address_determined=false`。 |
| 两个进入屏障的入口 | `S/src/ssd/Address_Mapping_Unit_Page_Level.cpp:480-489` 在任何 query/allocate 之前查 LPA 锁；`1701-1703,1716-1718` 在 mapping 返回后重查锁。后者在 `1711,1728` 去掉原 unmapped 索引。 |
| 屏障只保存指针 | 同文件 `1890-1897` 把原 ptr 加入 read/write multimap，不操作 owner 列表、不设置物理地址、不分配用户页、不注册 program、不提交 TSU。成员声明位于 `.h:104-113`。 |
| 候选错误释放路径 | 同文件 `1797-1821` 解锁后对 child 直接调用 AMU 自身 handler 然后 `delete`；该 handler 在 `1665-1670` 对非 `MAPPING` 立即返回。此处不是 PHY 广播，也不会执行 cache 的 USERIO owner 完成回调。 |
| 正常翻译与提交 | 同文件 `510-578` 处理 CMT hit/miss/等待；`584-607` READ 取得 PPA 并注册读，WRITE 分配 plane、查 Stop、分配用户页，最后置 physical=true；`492-506` 仅对 physical=true 的 child 提交 TSU，必要时一并提交 RelatedRead。 |
| 正常写分配语义 | 同文件 `1146-1195`：获取当前映射，失效旧 PPA；若部分覆盖则创建 update-read；用户新页由 `1191` 分配；`1193-1195` 记录 child PPA 并立即更新映射。`248-256,65-75` 表明更新发生在 GMT/CMT 的 allocation 时刻。 |
| 真实介质后再广播 | `S/src/nvm_chip/flash_memory/Flash_Chip.cpp:161-169,191-199` 在模拟 program 完成时写页面 metadata 并发 ready；`S/src/ssd/NVM_PHY_ONFI_NVDDR2.cpp:573-587` 在 program ready 路径广播该真实 child。`NVM_PHY_ONFI.cpp:16-22` 调用所有已注册 handler，最后删除该 child。 |
| 正常 owner 完成 | 冻结配置实际用 ADVANCED manager。`S/src/ssd/Data_Cache_Manager_Flash_Advanced.cpp:357-373,440-448`：USERIO 广播事务完成，TURNED_OFF WRITE 从 owner 列表 remove；仅列表为空且 cache sectors=0（`Data_Cache_Manager_Base.h:62`）才发 request serviced；`Host_Interface_NVMe.cpp:140-141` 再走正常 CQE/释放。Simple manager 同样的关键路径在 `214-248`，但不是本轮实例。 |

**静态可推出的入屏障状态，不冒充动态快照：** 当前正常创建、从未成功映射的 USERIO full-page WRITE，入屏障时应是 physical=false、PPA=NO_PPA、RelatedRead=NULL，原 ptr 在其 owner 列表中恰好一次，尚未有 user program-issued、TSU submit 或 PHY active membership。Address 字段没有有效用户物理归属，不能用它反推 GC 的旧 block。若 observer 显示不满足，必须重新定位，不可通过清位/reset 伪装。

本席核过的动态摘录：host UID1777（native device ID1776、CID53、LBA2912、16 sectors）绑定 child UID2841 / USERIO WRITE / LPA182 / creation PPA=NO_PPA；原始日志行66955创建、66956入barrier，67100释放、67102进入AMU callback、67104删除。创建/入队时刻2118228724ns，释放/删除2125524421ns。摘录携带raw log SHA256 `6d8cfadbd6e3b3ab141075874909c491c6af0598d2dfcd999efb1b6df945e32f`。这些字段直接支持身份链；physical/owner/完整日志中无其他完成等事实仍分别由上面的源码与root完整账本支持，不能仅从8条筛选事件自称已全局排除。

`CACHE` 也会被错误释放代码丢掉正常回调，但 CACHE writeback 可来自独立列表且 `UserIORequest=NULL`（如 Simple `156-158`）；WRITE_CACHE 还可提前从 owner 列表移除 user child（`174`）。因此不能把“所有 USERIO/CACHE child 均有活 owner”当通用不变量。本轮 `TURNED_OFF` 没有这个所有权分叉，建议验收中检查 CACHE child 数确为零。

## 2. 解锁时 GC 仍拥有什么

1. `GC_and_WL_Unit_Page_Level.cpp:148-150` 先标记 old block Has_ongoing_gc_wl、加入 Ongoing_erase_operations，并为其中有效 LPA 设置屏障（AMU `1768-1793`）。这不是把等待的 user child 绑定到 old block。
2. 非 copyback GC read 与 write 是另外两个 `GC_WL` 对象；GC write 的 `RelatedErase` 指向 old-block erase，见 `GC_and_WL_Unit_Page_Level.cpp:172-181`。GC read 完成后先核 current PPA 仍为其读出的 old PPA，再设置 GC write 的 LPA/bitmap，分配 **GC 新页**并提交（`GC_and_WL_Unit_Base.cpp:125-136`）。
3. AMU `757-827` / `1146-1195` 的 is_for_gc 路径在 GC write 发出前即失效 old PPA、把映射更新为 GC 新 PPA。GC write 真正 program 完成后，`GC_and_WL_Unit_Base.cpp:143-151` 才调用 Remove_lpa_barrier；此时 `transaction->Address` 属 GC 新页，`RelatedErase->Address` 属 old block，不能混用。
4. **Remove_lpa_barrier 返回前，该 GC write 仍在 old erase 的 Page_movement_activities 内。** `151` 才 remove 它；Has_ongoing_gc_wl 和 Ongoing_erase_operations 要等 erase 完成路径 `153-160` 才清掉。TSU 对 erase 的 ready 条件是 Page_movement_activities.empty（`TSU_Base.h:111-112`）。修复不能提前删 movement、提前解除旧 block GC ownership 或人工结束 erase。
5. GC 已 program 的是先前版本的搬运页，不能代表新用户写已 program。重试的 USERIO write 必须从当前映射取得 GC 新页作为其 old PPA，按正常流程将其失效并分配另一用户 PPA。

## 3. 局部 list 重译的可行性与顺序范围

### 可接受的释放操作

执行前先验证输入与待释放元素的合同；然后：

1. 为 **指定 stream + LPA** 分别取得两个 multimap 的 `equal_range(lpa)`；将这一代 waiters 的原 ptr 放入局部 `std::list<NVM_Transaction*>` 并从两个 barrier map 摘掉对应节点。先把两组都摘完，再调用任何可能重入的翻译/调度操作。
2. 擦除该 LPA 的 Locked_LPAs entry。不要清全部锁，不碰其他 LPA 的待办。
3. 局部 list 非空时只调用一次正常 `Translate_lpa_to_ppa_and_dispatch(local)`。list 只保存借用指针，销毁 list 节点不 delete child。
4. 不修改 owner 列表、child UID、Source、Type、原数据/bitmap/timestamp，不主动置 physical=true/false，不在 release 中完成或删除 child，不重新构造 child，不发送 CQE，不调用任何“补完成”handler/broadcast。

**为何局部 list 必要：** 把整个 `owner->Transaction_list` 重传会连同已经映射/排队的兄弟 child 再次 query/allocate/submit。局部 list 只能包含本次已从 barrier 索引移交出来的原 ptr；owner 列表同时保留完成记账引用，直到真实 PHY 回调自行 remove。队列引用和 owner 记账不是两个可各自 delete 的拥有者。

**为何先 detach 两组：** `query_cmt → allocate_user_page → Check_gc_required` 可同步创建 GC/新屏障，mapping miss 也可同步生成其他 flash 事务。若边迭代原 multimap 边递归处理，后来新加的同 LPA waiter 可能被当前循环再吃掉，或与旧迭代器/删除逻辑混淆。detach 形成固定批次；新一代只能留在新屏障索引中。每个 child 在重译入口仍由 `480-489` 再查锁。

### 同 LPA 并发 full-page WRITE：允许，不伪造无冲突前提

冻结 trace 随机覆写同一组 512 LPAs，允许同 LPA 有多个尚未完成的 write。本轮没有 read/fence；本合同不添加“按 host 提交顺序完成/覆盖”的新保证，只保持源模型 **按实际 allocation 次序更新映射** 的规则。

- 同 LPA 的每个原 child 分别分配 PPA，不能合并成一次 GC/user program，也不能因后写覆盖了前写就取消前写的真实 program 与完成。
- 局部 list 保持原 write multimap 对该 key 的现有迭代顺序。以后若有 overfull 等待，原 `std::set<ptr>` 恢复顺序本就不同于 host 提交顺序；不把它说成 NVMe FIFO 保证，也不为了恢复 FIFO 扩写调度器。
- W1 allocation 发布 P1；W2 allocation 再把 P1 标 invalid、发布 P2，允许 W1 的真实 program 当时尚未完成。这与普通写现有规则相同。`Flash_Block_Manager.cpp:20-33` 给每个 user allocation 增加其实际 block 的 Ongoing_user_program_count；Base `213-217` 仅在真实 user program 回调减数。
- 本轮 RGA 在 Page_Level `69-76` 调用 `is_safe_gc_wl_candidate`；Base `222-239` 拒绝 write frontier、ongoing user program>0、已有 GC 的 block。因此即使 P1 被后写标 invalid，也不能在 W1 program 之前据此 erase/reuse。W1/W2 最终各自经 PHY 完成；AMU handler 对 USERIO 返回且 cache completion不改映射，旧 child 晚完成不会把当前映射回滚成 P1。
- root 必须用实际 allocation/PHY/GC 证据检查这条性质，不能只用“两个 CQE 都出现”代替。

### READ / 部分页更新：本版不声称通用正确

两个 multimap 分开保存 R/W，无法据此恢复原 R-W 交错序列。简单“所有 read 后所有 write”或反过来，不是对任意有序访问的证明；`TSU_Base.h:107-110` 的 read ready 无逐 LPA 写后读依赖，write 只等 RelatedRead。尤其 mapping 在 program 前已发布，后续 read 的可见版本与先前未完成写、host ordering 需要另一个合同。

普通 read 重走正常映射可以重新注册读计数，并走真实 PHY/cache 返回，比删除或借 GC 数据补完成更有定义；但它不自动证明混合 R/W 的数据可见性/顺序。本轮最小接受范围因此是 **整页 USERIO write-only、cache off、无 read/fence**。建议私有修复在遇到 barrier read、非 full-page write、CACHE 或 unexpected RelatedRead 时明确报合同不适用；不要悄悄继续并把运行叫通用修复。可采用同一 detach 骨架处理两图，但 read 图非空时当前合同不准入。

## 4. 重入、再次加锁、CMT 和 overfull 的条件

### 回调内调度本身可以复用，但有明确范围

- 引擎是串行事件处理（`S/src/sim/Engine.cpp:80-99`）；Register_sim_event 只入队（`118-123`），不是多线程同时修改 owner。
- TSU 嵌套 Prepare/Schedule 用计数配对（`TSU_Base.h:47-60`；`TSU_OutofOrder.cpp:144-160`）；不要另手写漏配或多配的 Prepare。
- 释放发生于 GC program PHY 广播中，PHY 还没 ClearCommand/减 active dies/改 chip status（NVDDR2 `573-595`）。冻结实例仅一个 chip/die/plane，此时 PHY bookkeeping status 仍 WRITING；TSU write/erase `334-419` 不会在该 chip WRITING 时发新 program/erase。因此 release 内 Schedule 可以先排队，不会合法地占用尚未清掉的同 die ActiveTransactions。回调后 `619-622` 原有 idle 通知继续驱动，不需要新增模拟事件。
- 本轮 CMD_Suspension_Support=ERASE、没有 program suspension；不得把上面的单 chip/die 判断外推到 read + program suspend、多 die/多 plane 的通用重入安全。
- 原 GC movement remove 保持在 Remove_lpa_barrier 返回之后。不要为了“立刻开始用户写”调整此顺序。

### 再次加锁

局部 list 正常进入 Translate 后，每个 child 都重新查锁；若 locked 就把同一个 physical=false child 放回 barrier，一次 release 不承诺当场 submit。若早先 child 已分配成功，而之后同一调用栈又触发 GC，正常 user program-issued 计数应保护其目的 block；不得再把已 mapped/submitted child 入 barrier。动态验收核查每个 UID 的 queue 转移及 program-issued/serviced 配对。

### CMT 缺项/异步 mapping 等待

`query_cmt` 的 miss 可将原 child 放入 `Waiting_unmapped_{read,program}_transactions`（`561-575`）；local list 销毁不能 delete 它。mapping completion `1694-1728` 负责后续再查 barrier、translate 与 submit。不应直接跳过 query 调 `translate_lpa_to_ppa` 来“确保及时完成”。

当前输入的 CMT 不是缺证的默认假设：实际 config 为 2MiB，几何为 1024 physical pages / 16 sectors per page；AMU `327-334` 算 entry size=`ceil((2*log2(1024)+16)/8)=5B`、capacity=419430 entries，远大于冻结 trace 的 512 个 LPAs。故在本轮只有这些 user LPAs 的条件下不应发生 capacity eviction。仍须实际记录 mapping miss 的类型：首次 `GTD=NO_MPPN` 的同步建项是正常（`1462-1484`），**异步 mapping fetch / eviction / MVPN barrier 是未预计路径，不能未经调查就套用本轮证明**。

### 满 plane：保留合法 defer，但既有恢复路径存在证明缺口

1. `translate_lpa_to_ppa:599-605` 先取得 plane，Stop_servicing_writes 为真时，在 page allocation 前返回 false，physical应仍为false。`mange_unsuccessful_translation:1909-1912` 用原 ptr 放入 per-plane set。此时不能丢指针、补成功、强行越过 Stop，也不能预占 user page/增 program count。
2. GC erase completion `153-160` 先将 erased block 归还池并结束该 old block GC，再调用 `Start_servicing_writes_for_overfull_plane`；等待本来就由已有 erase/idle 事件继续驱动，无需加轮询事件。
3. **原恢复函数 `1915-1933` 直接调用 translate，未重新查 Locked_LPAs 或 CMT accessible。** 等待期间其他正常 allocation 可启动 GC；恢复循环本身的 allocation 也可同步再启动 GC。因而“进入 overfull set 后最终又被提交”不足以证明没有越过新屏障。CMT 问题在本轮容量下可收窄，但重锁不能凭容量排除。
4. 对当前最小 release-only 版本，overfull defer **不是自动否决，也不预设为零**：必须观察每次恢复前相关 LPA unlocked 且 entry accessible；只成功分配一次、只提交一次；期间若重锁必须不发生越锁 allocation/submit。如果这些实际条件通过，本轮可在其覆盖范围内接受。
5. 若当前四输入实际出现“恢复时 locked / inaccessible”，则 release-only 修复不能通过。root 可选择另一有界补丁：先从 per-plane set 摘出一批原 ptr，统一经正常 Translate 重试，使 locked 进 barrier、missing 进 unmapped、still overfull 回新 set；不要在原 set 的迭代中调用会向同 set 插入的管理函数。此候选需要 root 独立核对触发时序并作为新补丁审查；本席不提前把它当已实现/已证正确。也可拒收本版，保留具体 UID/新锁与恢复记录。

## 5. 四冻结输入必须实际验的因果与负检查

以下是验收接口，不代表本席已运行或其他席已有这些日志。必要字段缺失应标未核，不能从总数推断。

### 全部四输入共同条件

- 原始 input/config/workload/source/instrumentation 哈希绑定；只增私有修复与必要观察，不调参数、seed、到达、几何、cache，不修改旧输出。对 512/2048/4096/8192 各一次授权条件回归，遵守原时限。
- 每个 host generation UID 唯一绑定 device request、其 child lifetime UID、device CQE 和实际 host consume。当前 full-page/cache-off 输入应每请求一个 USERIO WRITE child；每个 child 创建恰一次、正常 user page allocation恰一次、TSU submit恰一次、真实 PHY program completion恰一次、正常 owner remove恰一次、host consume恰一次。分开计 GC/MAPPING，不用它们抵 user program 数。
- 对每个 barrier child：入队时 physical=false、PPA未分配，owner仍含原ptr且未早发CQE；release仅移交索引，保持同UID/ptr/owner；之后要么 rebarrier、mapping wait、overfull wait，要么 normal map→allocate→submit；不能无归属消失。最后一次 release 后必须实际到达自己的 PHY program 完成及 owner/CQE，**GC_WL 的完成不充当该节点**。
- 每个成功 allocation 的 PPA 对应独立介质 program，若 PPA 随 block erase 被复用，按 `(block erase generation, page)` 区分，不错误要求 PPA 数值跨全程唯一。同 LPA 后写使前写页invalid时，前写仍真实完成；user outstanding count 归零之前不得将所属 block选为GC/erase，晚 completion 不回滚mapping。
- 每次 retry/defer/relock 有明确队列去向；同 UID 不同时在 barrier/unmapped/overfull/TSU 等不同活动调度状态，owner引用另计。所有新增 barrier enqueue/release 最终平衡；同 ptr 不能被两处delete。owner清除只来自正常完成。物理标志不能在失败重试后残留true而重复submit。
- 上述 overfull 恢复检查逐次成立；异步 CMT/MVPN 路径若出现，按第4节处理；barrier READ、CACHE、partial write、RelatedRead 应为0。源码 guard/assert/observer覆盖不能只靠stdout没报错代替。
- TSU Prepare/Schedule 在事件返回/引擎结束时平衡，原 GC Page_movement 链与 old block start/erase-end 配对，无program/read counter下溢、无被保护 block 提前erase、无 consistency/error消息或显式Engine stop。
- 只有 generation done、所有预期 host consume、软件 active/waiting=0、PCIe双向队列空且callback不在执行中，并在完整事件返回之后，才接受 host drained boundary。引擎正常退出还须待办队列/GC归零；不把引擎事件队列空直接当所有请求已完成。

### 512：负检查

initial-fill-only 对照应继续512完整因果链；原合同下未预期GC/LPA barrier，若修复在此也改出新行为或出现虚假repair触发，须解释具体来源。无barrier输入不能因“进入修复函数也补发完成”而改变计数/调度。它不是所有输入无GC的证明。

### 2048：直接反事实链

未修复 identity replay 的缺失确切 host UID1777 / child UID2841 及删除路径已由root报告并有上述摘录。正式接受保留其最后合法状态、barrier entry、GC unlock、错误局部handler返回及删除、owner残留，并以root完整账本排除该child自己的allocation/TSU/PHY completion/CQE。本席不把筛选摘录当完整日志核账。

补丁回归对同一冻结输入中对应 generation UID，必须看见其本次创建的原child在release后实际正常allocate→TSU→自己的PHY program→owner remove→正常CQE→host consume。不要求补丁运行中的child UID数值仍为2841，也不要求后续GC/所有其他UID的时间与基线完全相同，因为增加真实用户program会改变内部时序与GC选择。只剩“2048=2048”不能通过。

### 4096 / 8192：传播与新路径

检查所有UID而非只补旧缺1/2；修复增加真实program后可以改变后续barrier、defer和GC序列，不能把旧缺失数当固定补发配额。重点核多次屏障、同LPA多child、满plane恢复、GC新页失效及old block movement/erase链。是否存在 host drained 而 GC>0 的合法边界，另按原 D 来源门记录；全部完成是正确性前提，不保证存在该 witness。

## 6. root 接受 / 否决条件与未解

**可进入实现：** identity 动态绑定成立，root核实关键原文，接受整页 USERIO write-only 的私有范围；仅修改LPA释放逻辑，不影响MVPN假设捷径、不调输入。先保留原ptr与正常路径，实施席不要自己补CQE、重建child或跳过Stop。

**四输入通过才接受本版：** 第5节所需因果/负检查在可用观察证据上成立，且所有预期命令完整完成。不要求消除同LPA并发，也不要求所有合法并发write遵循host提交顺序；要求保持本模型allocation更新规则及每个child真实program后完成。

**具体否决理由：** 缺失UID不在此路径；同ptr被重复allocate/submit/delete；未program就owner/CQE；GC完成被替代成user完成；overfull恢复越过新LPA锁；映射entry不可用仍直接translate；未完成user program所在block被GC/erase；出现不在证明范围的read/partial/cache/MVPN路径且未另验。任何一项都不能由“计数补齐”覆盖。

root已报告基线36次overfull enter/exit、637次LPA lock/unlock均配对；**配对不等于恢复时未越锁**，第4/5节的逐UID/时刻检查仍必须进行。未解项保留：补丁四输入的overfull/relock实际覆盖；新增时序是否碰到已有GC内部状态机问题；通用read/write ordering、partial update和cache lifetime；多chip/die/plane/复制回写及program suspend重入；实际数据字节正确性（MQSim现有主机模型未存真实payload）。这些均不由静态修复建议自动解决。

本席独占写入本文件；CURRENT_STATUS/HANDOFF/validation与正式root接受记录由root合并，不越权修改。
