## Material Passport

- Origin Skill: ARS-Codex experiment-agent；仅取实验边界规划，不执行实验或选择新策略。
- Origin Mode: plan / bounded interface source audit。
- Origin Date: 2026-10-15。
- Verification Status: 源码语义已核；以下 hook、driver 和轨迹判据尚未实现/运行。
- Version Label: d_dram_boundary_interface_v1。
- Requested runtime: gpt-6.1-sol / high；actual model、actual effort 均 unknown，未把请求参数当运行证明。
- Source: Ramulator2 v2.0a，commit `be93be78055d922aa1d4d33e15bcc8f2b0c61a9d`。复用既有19份官方原字节，没有重新下载或调研论文。当前重新核对的文件、行段、SHA-256、Git blob 见 `source-read-ranges.json`。
- 独占范围：有限 read 前史在 GenericDRAM 物理入口的完成/排空边界、cap PRE、所有 rank 刷新完成和最小只读观测；未改 source、未构建/执行、未写 canonical 文档。root 负责最终实验合同与接受。

## 结论和边界定义

**应在一次完整 `GenericDRAMSystem::tick()` 返回后检查排空，不能在最后一个 read callback 内检查。有限 reads 已完成且 controller 五类容器都空、没有已发刷新对应的 future action、各 bank 不处于 Refreshing，可以证明该 tick 的请求和维护已经排空；它不要求开行、cap 计数、时序 deadline 或下次周期刷新消失。**

**所有 rank 的一次刷新已结束，还须证明每个 channel/rank 都有本轮 REFab issue 和对应 end。只见一个 REF、只见 priority 空或一时所有 bank Closed 都不够。** 本版本 `REFab_end` 在 `t_REF+nRFC−1` 发生，而 ACT/PREA 的许可为 `t_REF+nRFC`，所以“状态机结束”和“下一 ACT 已 ready”必须分栏，至少存在源码定义的一拍差别。

以 `S(t)` 表示第 t 个 memory tick 全部执行完毕后的快照，定义：

```text
reads_finished(S) := 所有本阶段成功 accepted 的 read ID 恰好回调一次
                    AND completed_count == accepted_count == planned_count
                    AND 外部没有计划中尚未成功接受的请求/重试

controller_empty(S) := 对每个channel
    read.size == 0 AND write.size == 0 AND active.size == 0
    AND priority.size == 0 AND pending.size == 0

maintenance_idle_now(S) :=
    DRAM.m_future_actions.empty()
    AND 每个bank.state != Refreshing
    AND 没有本tick到期而尚未处理的refresh enqueue
    AND 对每个channel refresh.next_refresh_cycle > refresh.clock

Q_drain(S) := reads_finished(S) AND controller_empty(S)
              AND maintenance_idle_now(S)
```

最后两项由“完整 memory tick 后采样”与 refresh 快照共同保障：GenericDRAM `:75–80` 先 DRAM tick，再逐 controller tick；controller `:180–204` 先完成读、再周期刷新、调度、row policy、plugins、发命令。callback 里只能登记完成，禁止重入 send/tick、禁止更改地址/类型/回调，边界检查等 tick 返回后进行。

`Q_drain` 是**当下**无请求/已排队维护/在途刷新，不是以后永远没有维护。AllBank 的下一次周期到期是独立字段 `m_next_refresh_cycle`，不在 `m_future_actions` 中；该字段应保留并记录。继续 tick 会按正常规则产生后续刷新。禁止禁用/延后刷新、清事件或无限等待“未来所有周期事件也不存在”。如果预算内始终没有满足点，报告无合法边界，不伪造排空。

## 请求到底何时完成

| 源码位置（source/ 下） | 已核事实 | driver 含义 |
| --- | --- | --- |
| `src/memory_system/impl/generic_DRAM_system.cpp:50–72` | apply mapper 后把请求送给选定 channel；仅 `is_success` 时统计成功请求 | 物理入口 accepted 是 `send==true`。拒绝请求不能算 accepted 或 completion；记录失败重试与成功次序。 |
| `src/dram_controller/impl/generic_dram_controller.cpp:113–160` | 普通请求进入 read/write；同地址可从 write buffer 转发 read 至 pending，depart=下一拍 | 本轮只注入 reads 且初始无 writes，以排除转发。controller 的统计计数在入队前递增，不可拿它当 accepted 数。 |
| 同文件 `:206–220` | 最终 RD issue 后设 depart=`m_clk+m_read_latency`，复制进 pending，再移除原buffer；ACT 后可能迁入 active | 普通队列空不能排除 active/pending；final RD issue 不是 read callback 完成。 |
| `src/dram/impl/DDR4.cpp:434–435` | read latency=`nCL+nBL` | 回调表示模型中的读数据到达；本模型不执行真实数据内容校验。 |
| controller `:298–317` | 每tick只检查 pending 首项一次；depart<=clk时先 callback，后 pop_front | callback 时原请求尚在 pending，且同tick之后还可能 enqueue/issue维护。真实回调tick可能晚于depart，需同时记录 depart、callback tick、pending retire后的快照。 |
| controller `:211–214` | WR最后命令后直接移除，无写完成callback | 本轮限定 reads。不能将此处队列空解释为 AXI B 或真实 DDR 写入完成。 |

因此本轮完成域是“GenericDRAM 模型的 read 数据到达及 controller retire”，不声称覆盖真实系统 cache、互连、IOMMU 或 host CQ 的完成域。

## 普通 warm 与 cap=4 的 PRE

OpenRowPolicy 在 `basic_rowpolicies.cpp:10–21` 不主动动作。ClosedRowPolicy 在 `:72–117` 仅对本tick已经选定的命令更新：PRE/REF清计数；RD/WR给目标bank计数+1，到 cap 就创建 close-row 请求，经 `priority_send` enqueue PRE，再立即把计数置0。

严格顺序为：

```text
controller tick(t):
  callback/retire之前已到depart的旧read
  refresh.tick()
  schedule选出本tick第四次RD
  rowpolicy.update(): 计数3→4，enqueue PRE，计数→0
  plugins.update()      # 官方TraceRecorder在这里；尚未issue
  DRAM.issue_command(RD)
  read进pending，depart=t+(nCL+nBL)，原buffer移除

后续tick(s):
  维护PRE真正check_ready并被选中，issue PRE后bank Closed/row map清空
  这可能在最后RD的callback之前、同一tick或之后，不能预设顺序
```

证据：controller `:185–214`；rowpolicy `:103–115`；DDR4 `:484–486`；action `:17–26`。PRE earliest 受到最近 ACT+nRAS 和 RD+nRTP 等全部实际限制，以及 active/priority 调度影响。对默认 DDR4_2400R，`nCL+nBL=20`，`nRTP=9`，`nRAS=39`（DDR4 `:38`）；这些是源级参数，不是本席运行数据，也不能仅凭公式替代实际 issue 记录。

必须分别记录以下里程碑：

1. warm 的 read accepted / callback / retire。
2. cap触发 PRE 请求（目标bank、触发RD、tick）。
3. `priority_send` 的现有返回值为 true，实际 PRE 入队。
4. DRAM实际 PRE issue。
5. 完整tick后的 bank Closed 且 row map 空。
6. 若合同要求 PRE 后可 ACT，另检查全部路径上的 ACT timing ready；不能把 PRE issue 即当 nRP 已过。

特别注意：rowpolicy `:113` 忽略 `priority_send` 返回值，然后清计数/增统计。因此 `m_col_accesses=0` 或 `num_close_reqs++` **不能证明 PRE 已成功入队或发出**。低占用测试也应记录既有返回值，出现失败就拒绝该次cap完整性判断，不修原实现后假装原规则成立。

一个只完成1–3次列访问且没有cap/PRE/PREA/REF介入的bank，可在 `Q_drain` 下仍 Opened、保留 row map 和非零cap计数。warm若是实际读，本身参与计数、可触发第4次关行，还推进时钟/刷新；它不是不扰动准备。若warm之后状态洗去，应按事件解释，不能保留“warm前”快照作为“warm后”状态。

## 所有 rank 的刷新完成

AllBank `all_bank_refresh.cpp:40–55` 每次时钟等于 next 时，先 `next+=nREFI`，再为每个rank enqueue一个 all-bank-refresh。每个controller独立覆盖该channel的所有rank；示例为1channel、2ranks，而不是只有rank0。

REF请求在 `preq.h:45–69` 若有开bank先要求 PREA。PREA (`action.h:74–93`) 清各bank row map；随后 REFab (`:95–102`) 把各bank置 Refreshing；结束动作 (`:104–111`) 把各bank置 Closed。**这套 rank action 不给 rank 节点本身写 Refreshing，rank初态又是 PowerUp（DDR4 `:159–165`）。必须遍历所有bank，不能依赖rank.m_state；可保留rank state作原始记录。** REFab_end本身不清row map，合法REF路径的PREA已清；应核实际row map，不能把理想语义补到源码。

REFab issue 当tick移除priority请求，却在 DDR4 `:220–243` 留下 `FutureAction{cmd=REFab, addr, clk=t_REF+nRFC−1}`；后续 DDR4 tick正好等于该clk时执行 end并删除（`:183–193`）。FutureAction记录的cmd仍为REFab，不是REFab_end。

对第 g 个已到期刷新burst，建立只读事件账：

```text
每个(channel, rank):
  refresh_enqueue(g, accepted=true)
  actual_REFab_issue(g, t_REF)
  actual_REFab_end(g, t_END)
  require t_END == t_REF+nRFC−1

all_ranks_ended(g,S) := 每个配置中的channel/rank都有上述三事件
                      AND 当前没有该g残余排队/未来action/Refreshing bank

refresh_state_boundary(g,S) := all_ranks_ended(g,S) AND Q_drain(S)

refresh_ready_boundary(g,S) := refresh_state_boundary(g,S)
                              AND 每个bank的ACT check_ready(...)为true
```

generation g不必写回模型Request：audit sidecar按每个channel/rank保存已接受REF的FIFO，将实际REF issue匹配最早尚未issue者，将end按地址和预定end tick匹配。若关联不唯一/遗漏则拒绝验收；不要给原Request加影响调度的字段。对所有due bursts都记账，不能只为最后一个挑结果。`Q_drain` 可排除当前在途维护，但不能单独证明“已跨过至少一个完整刷新burst”。

REF→ACT/PREA deadline 在 DDR4 `:471` 是 `t_REF+nRFC`；end函数不调用update_timing。所以end tick上即使future vector空且所有bank Closed，ACT仍可能尚未ready一拍。后续若要求所有旧前史有效timing已成熟，可加单独的 `timing_matured` 判据：选明目标命令集合和地址路径，或扫描所有节点所有命令 `ready_clk<=now`（未设置−1忽略）。这是更强条件，不属于 `Q_drain` 必要条件。扫描后仍须记录 next_refresh，防止新REF在等待期间重新产生约束。

## 最小只读 hook 与记录项

优先采用私有实验 `audit` sink；各实现文件对已有字段/局部返回值只读emit，由统一tick末尾汇聚。无需向模拟请求加入额外调度能力，无需reset/clear/private-state overwrite。建议位置：

| hook位置 | 必须记录 | 理由 |
| --- | --- | --- |
| GenericDRAM `send:51–72`，mapper后/已得返回值 | full address、mapped vector、type、stable外部ID、success、memory clock及同tick序号 | 精确区分拒绝/接受与controller物理入口；不能仅记host submit。 |
| controller `priority_send:163–168` | addr、type/finalcmd、success、tick | 维护实际入队与失败。 |
| controller `tick:223–225` 结束处；GenericDRAM `:80–81`全部controller后 | controller clock、read/write/active/priority/pending各大小；必要时每项ID/type/address/final/next command/arrive/depart；write_mode；最终聚合快照 | 五容器必须逐项读；现有queue统计省略active，且是累计量，不能代用。callback内不是采样点。 |
| rowpolicy `:72–118`结束/返回前 | cap、每bank `m_col_accesses`；触发命令和cap enqueue事件 | 私有计数无法从外层completion直接读出；只在快照读取，不运行update以求观测。 |
| refresh `:40–57`完成后 | local clock、nREFI、next_refresh、num_ranks；本轮每rank enqueue结果 | 区别已排队维护与未来周期。字段private，原接口不能直接取得。 |
| DDR4 `issue_command:210–218`执行完毕 | 真正issue tick/command/address、future vector；可记录pre/post bank快照 | 官方TraceRecorder在issue之前，只证明选中命令；此处证明真正改变状态。 |
| DDR4 `handle_future_action:232–243` REFab_end后；DDR4 tick `:191–193`删除action后 | end event及其原REF地址、deadline、完成后状态/剩余future | end不是controller选中命令，TraceRecorder不会代为记录。 |
| DDR4统一snapshot（在GenericDRAM tick末聚合时调用） | 全部channel/rank/bank的state、row map；每层ready_clk/history；m_future_actions；DRAMclock | bank row map与所有层时序是持久来源；future和Refreshing是真正未完成维护。 |

这些是观测位置/接口约束，非已写patch。可在新私有header定义只读snapshot结构/observe函数，或给相关接口补const debug snapshot；具体机械接口由实施席与root选择。**private字段不可由普通插件假称直接可读**，尤其controller容器、ClosedRowPolicy计数、AllBank next。DDR4具体类定义也在.cpp中，外部仅持IDRAM指针时不能臆造downcast访问节点；本地snapshot hook可绕过这一可见性问题。

现有 `IDRAM` 对 future vector、timing tables、check_ready/check_rowbuffer_hit开放C++内部接口（`dram.h:32–59,83–124`），不意味着生产主机/硬件可以免费读取这些状态。只读 `check_ready` 也只查 timing：`node.h:209–232` 不核state或队列，须与state和get_preq一起解释；bank Closed时RD可能timing-ready，但实际下一命令仍是ACT。

不要调用 `FRFCFS::get_best_request()` 当“只读查询”，它会给每个req写 `command`（scheduler `:41–48`）。直接读snapshot，用const逻辑统计；不可通过调度一次来观测。单纯 `check_ready`、`get_preq_command` 在本轮所核固定实现中只读，但禁止调用issue/update/tick“探测状态”。

## 最小frontend/factory初始化链

补读另一实施席取得的同commit完整archive（绝对路径见source-read-ranges.json），无需自建frontend类。官方 `src/frontend/impl/external_wrapper/gem5_frontend.cpp:11–24` 的GEM5 frontend：init/tick为空、external receive直接转memory.send；它不注入cache/translation请求。`IFrontEnd::get_num_cores()`默认1（frontend.h:46），满足Generic controller按core建统计数组；外部request可用合法source_id=0，唯一ID保留在callback capture。

```cpp
// YAML中 Frontend.impl = GEM5；MemorySystem保留root冻结的原配置。
auto* frontend = Factory::create_frontend(config);
auto* memory = Factory::create_memory_system(config);
frontend->connect_memory_system(memory);
memory->connect_frontend(frontend);
// 随后driver自行 memory->send(Request(addr, Read, 0, callback));
// memory->tick(); 最后返回后采样。不由frontend自动发请求。
```

这保留 `main.cpp:83–92` 的完整初始化/connect顺序：Factory创建并gather children（factory.cpp:139–162），注册构造宏会init（base.h:262–277），connect再按组件列表setup（frontend.h:24–30；memory_system.h:24–30）。不要重新调用init冒充冷启动。Generic的setup先得到DRAM/设置bank index/核心数，之后refresh/rowpolicy的setup才能依赖它。

**不要沿用官方main的退出条件。** GEM5.is_finished恒true（gem5_frontend.cpp:23），main.cpp:105–106因此会立即退出。LoadStoreTrace也不合适作为完成证据：`:41–47,96–99` 只按成功send条数完成，不能证明pending数据完成。私有driver应按本卡Q_drain决定何时停止，继续推进维护事件。

本轮直接一步推进一个memory tick，并在记录中声明这一单位。GEM5没有解析`clock_ratio`，继承IFrontEnd默认1（frontend.h:21）；不声称复现示例SimpleO3的8:3 frontend/memory调度。MemorySystem原clock_ratio字段可保留；直接tick驱动的入口时间以memory clock精确约定，不自动换算主机时间。

## passive sink的最小实现约束

可在一个私有audit header中提供probe注册：各具体实现setup时注册只读lambda，捕获this、channel等ID；driver在memory.tick返回后统一调用这些probe读取私有字段，不必改现有interface。DDR4 probe在其.cpp内部定义，因而无需暴露具体Node类；rowpolicy、refresh、controller同理。register不调用tick/update、不注入请求；probe只复制标量/容器内容，不调用scheduler选择。

事件emit只复制值，不保存将失效的req iterator/reference；日志层与模型分开。fresh-instance前注销旧probe或令probe生命周期严格包含在instance内，防止第二次实例访问已销毁this。队列优先级/调用次数/时钟推进次数不得改变，`priority_send`成功状态利用已经存在的局部is_success，不额外再send一次。audit失败由driver在tick返回后拒绝结果；不能悄悄跳过日志再宣布通过。

全局post-tick快照应在一个明确入口采集；不要把各组件早先emit的片段误拼成同一时点快照。特别是DDR4.tick先结束后controller仍可issue新REF，故仅用DDR4.tick末的future空来代表memory tick末会漏计新维护。事件ledger可分别存pre/post，验收只使用统一post-memory-tick状态。

在所选DDR4中，`m_future_actions`只用于已发REF结束动作（DDR4.cpp:220–229），所以**任何剩余future action都视为Q_drain未通过**；next_refresh_cycle则是未来周期而应保留。不要把`m_cmd_ready_clk`/history当“真实队列”：它们是残留时序状态，非待完成请求，不应为了排空自动清零。换其他DRAM实现后需另核其future语义，本卡不泛化。

## 最小driver伪码与接受约束

```text
create full fresh instance using fixed original config; connect/setup normally
disable unrelated frontend injection by using the agreed physical-entry adapter
assert all clocks/queues initial and no submitted writes; use only read requests

for each phase already frozen by root:  # 本席不新增workload或策略
    accepted_ids = {}; completed_ids = {}; retry_only_unaccepted()
    send each planned read outside callback; preserve explicit same-tick order
    on callback(id,req):
        assert id in accepted_ids and id not in completed_ids
        save callback tick, req.depart; mark completed_ids  # 无重入/注入
    while within root's fixed cycle budget:
        mem.tick()                     # DRAM then every controller
        S = collect_const_snapshot()  # 只在完整tick后
        verify all clocks align, all observed enqueues accepted as required
        if phase wants drain and Q_drain(S): save S; stop drain loop
        if phase wants completed refresh and refresh_state_boundary(g,S):
            save state-end S
            if contract also wants ACT-ready:
                keep ticking normally until refresh_ready_boundary(g,S)
            stop at the required boundary
    if no target boundary: report timeout/no legal boundary, never clear queues

compare actual snapshots/event ledger with source rules
```

外部ID宜放callback capture/sidecar，不改 `source_id` 当唯一ID（它还索引per-core统计），不改地址映射或同tick顺序。失败send仍应记日志，其请求按规定重试，成功时才入accepted集合。read-only输入配空写队列排除了write forwarding；若以后混写，须重新冻结真正写完成域。

接受检查仅针对边界：请求ID不丢不重、N成功接受且N回调、五容器空、future空、所有bank非Refreshing、next仍在将来；cap触发/入队/issue/闭行可逐项勾连；每个配置channel/rank刷新都完整匹配；REF end与ACT ready一拍区别按实际迹验证。短quiet后是否保留行/计数只报告实际快照；跨REF后闭行不等于所有旧timing已成熟，保留历史数组不等于历史仍有性能影响。

无新增P/PN策略、无native trace/P/PN前置要求、无O_host信息充分性判断。固定O_phys可重放只关乎模型输入的充分性，不能据此认定O_host足够；本轮结果也不构成MIG/DDR硬件性能主张。root应直接复核上述关键源段及实际hook补丁/轨迹后冻结验收。
