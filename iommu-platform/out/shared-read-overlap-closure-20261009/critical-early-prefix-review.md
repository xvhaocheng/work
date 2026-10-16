# §74：V 写数据债排空前启动 A prefix 的关键审核

Codex 直接审读本目录的 design、功能 diff、当前 `shared_pretl_read.sv`、top 的 W gate 和 `q_write_splitter.sv` 的承诺/消费口。仅作源码并发推演，未构建、执行或修改 RTL。本稿不给正在进行的构建/运行预先标 PASS。

## 结论与必要条件

在 mode3、两个单次静态正常 2KiB job、Q2/private B3、真实完整授权、非零 AR→R 延迟的原合同内，当前提前移交未见新增 header、ID/LAST 路由或内部数据债死锁阻断。它把“V 所有已承诺 W 的 payload 已到”与“V raw W 已实际交出”分开，前者即可释放 main R 元数据供独立 A prefix 使用。旧 V R 仍在 oldV route；旧 AW/W/B 全部保留。

两个条件必须同时保留：移交时实际 `rV==J && rV>=promisedV && !skid`，以及移交后任何新 W 的 `nextEnd<=实际 returned`。第一条件覆盖 already firstVALID 而尚未进入 global mux 的 qchild；第二条件禁止 A 用新缺 payload 的 W 头挡住 V 排空。当前源码与 qwriter 接口满足这两项。

## 1. 提前移交时没有未跟踪的 V R 或写承诺

源 `shared_pretl_read.sv:324–328`：

```systemverilog
if (EarlyPrefix && cutover_phase_q == 1 && !early_moved_q && !skid_q &&
    returned_q[0] == v_cut_q && returned_q[0] >= promised_write_end_i[0]) begin
    old_v_busy_q <= busy_q;
    busy_q <= 0;
    early_moved_q <= 1;
end
```

`returnedV` 在源 `:475–479` 的真实 logical R 握手才增加。qwriter `:37–42` 的 `raw_written` 用 job-relative parent base+offset+当前 child 已握手 got；`promised_end` 则使用当前 child 完整 count，包含其注册创建但尚未 AWREADY/尚未 global offer 的尾端。child 在 `:96–104` 创建即 `child_live=offered=1`；payload 只有 `:114–122` 实际 W 握手才消费。

因此 `rV>=promisedV` 不把仅完成译址、仅收到物理 R 或仅 qchild idle 当 payload 到达。可能尚有 V 数据留在 FIFO/输出流水级或一个 held W，但它已经不需要未来 source R。`rawWV<J` 只说明这些已到数据还未全部交给写路径，不说明其丢失。

phase1 接收 V R 的末端是 J（源 `:187`）。移交沿 `!skid` 排除任何尚未交 DMA 的已接受 beat，rV 又已到 J，所以该沿不会再接受 index J 的 V beat。正在内存中 hold 的 index J RVALID 不被撤回，之后仍由同一个 oldV route 接收。J/E 在 cutover 时保存（`:318–320`），不会被新的 A metadata 改写。

同沿创建一个 V qchild不会漏承诺：top `:728–733` 先把所有新 V child 截至固定 J，即使 source 读到的是沿前 promise，新 promise 也不能超 J。既有 firstVALID child 不能被组合 quantum 缩短；原 start-cutover 的 J>=promised 门仍在 `:135–140`。

## 2. 同沿 phase2 与 A busy 的所有权

phase1→phase2 仍要求真实 `rawWV==J`、rV==J、无 skid、promise<=J（源 `:330–336`）。新增 `!early_moved_q` 决定是否还需要移动 main busy：

```systemverilog
if (ParallelPrefixRepair && !early_moved_q) begin
    old_v_busy_q <= busy_q;
    busy_q <= 0;
end
```

分两种情况：

- 首次 early handoff 与 phase2 同沿：两块读取同一个沿前 V busy，并向 oldVbusy/busy 写相同值。A builder 的 early 分支读取旧 `early_moved=0`，该沿不能建立新 A header。不存在把新 A busy 复制为旧 V 的竞态。
- 已提前 handoff 后：early_moved 已经为 1；A 可以 offered、accepted、busy。后来的 phase2 transition 不写 main busy/oldVbusy。即使该沿接受 A AR（`:443–447`）或 A physical LAST（`:468–472`），只由对应实际握手更新 A route，不会被 phase2 覆盖。

physical A R skid 与 logical A R held 也保持：无 skid 才能进入 phase2；接受新 A R 的同沿可正常建立 skid，phase1/phase2 对它均要求 member A。原 logical header/id/LAST 由同一个原 parent 与 A cursor 产生（`:193–197`），没有随阶段翻转。

## 3. 元数据复用和真实 LAST

EarlyPrefix 只在 `phase1 && early_moved` 扩展已有双路解析（源 `:156–166`）：返回 ID 为 V 且 oldVbusy 时走 `incoming_old_v`，beat index 是实际 rV；其余已授权 main child 走 A offset+got。ID 合同仍为原 V0/A4，完成接口加 direct bit，返回再清 direct bit。没有给双方复用一个 ID。

当 early_moved 后且仍 phase1，V 路由强制留在 J（`:187`），A 路由不受 J 限制，可以读取 immutable `[0,E)`。A builder `:357–370` 要求 A 自己 READ 有效、WRITE256；当 choice 尚未绑定时 `repair_shared_active=1`，固定 prefix 的终点仍 E。V 原物理 header 已被下游接受，改 main `child_q` 不会改旧内存 header。

旧 V physical LAST 使用保存的 E（`:454–457`）；A physical LAST 使用自己的 `child_beats/child_got`（`:458–460`），各自只清自己的 busy（`:468–472`）。以后 independent V continuation 才使用端点 256。logical LAST 始终用各自原 len255 与各自 returned cursor（`:196–197,475–479`），没有把 E 误当 copy 的 logical LAST。

若 J==E<256，最后 physical R 已进入/交出 skid 后才满足无 skid 的 handoff，沿前 main busy 已为 0；复制 oldVbusy=0 是正确的，不需等待一个不存在的新 RLAST。phase1 只继续排空已到的 V W，随后进入原 phase2。若 E==256 且选 burst-end closure，原 `<256` 门禁止该 cutover，按原路径完成，不把 terminal 当另一个可接力端点。

本次没有加入在-flight cohort membership：A prefix 仍是独立物理读取；真正共享 suffix 仍须旧 V 真 RLAST、双方实际 R/W==E、无 skid/main offer/busy，和双方 READ/WRITE/PA/属性匹配。这次移交不能用于绕过这些 join 门。

## 4. 为什么 held A R 不会困住 V 的缺 payload W

移交时所有已不可撤 V W 的 payload 已实际返回；phase1 top 再禁止新 V W 超 J（`:728–733`）。EarlyPrefix 打开 A 借两 private slots（源 `:177–183`）和 A 独立 prefix R，但 top 同时把 **所有新 W** 限制为 payload 已到：

```systemverilog
// multi_actual_dma_top.sv:749–751
hold_i(!current_write_recipe_valid[f] ||
 (ParallelPrefixRepair &&
  (phase==2 || (EarlyPrefix && phase==1 && early_moved)) &&
  next_end*8 > source_returned[f]) || ...)
```

所以 global W 头即便是 A，也已有完整 qchild 数据；即便头是 V，其旧承诺已被 rV 覆盖。held A R 可以等待 FIFO/W 空间，但这些 W 的消费不需要该 held beat 或新的 V R。这排除了本次变化针对的 `held A R → V 缺 payload W → W头 → A FIFO` 环。

反过来，若某实现允许移交时遗留超 J 的 firstVALID V child，可取 J10、rV10、rawWV10、V 当前 qchild[10,12)：V 在 J 禁止 R10/R11，而该 W 要它们，phase2 的 promise 门又拒绝推进。这个状态的 AXI 义务本身合法，但被本版 **start 的 J>=promise 和新 child 剪 J** 提前排除，handoff 的 `rV>=promise` 再次检查；不能声称只删 handoff 比较、保留另外两门，就能从本版正常可达状态制造此环。必要的是整个 `promisedV<=J<=rV` 不变量，不是某一个冗余比较单独不可删除。

若删除 resident NEW W 门，可让 A 新 qW[2,4)在全局 W 头但 A 只收到2个 beat，下一 A R 在 index2；DDR 当前 held V R indexJ。V 要等自己的 W 排空才放开 J 处 R，但 V W 被缺 A payload 的 W 头挡住，A R又不能越过 held V R。当前 `nextEnd<=returnedA` 禁止 A qW[2,4) firstVALID，故此环不能建立。

若 DDR 在 J 处先 hold 旧 V R，则 A AR 可能被接受却暂时得不到数据；V 已到数据的 W/B 在正常有限服务下独立排空，phase2 后 V R 继续，这不是依赖 A 数据的环。不能据此保证 RR 一定给 A 提前 R，也不能保证 early 功能必有 cycle 收益。AXI 本身无 W/B 服务周期上界；结论依赖正常写数据与 B 服务能够推进，而非用永远关闭 READY 制造反例。

奇数 E 与 A 原生奇数 F 没有改变上述逻辑：新 A W 先剪 E（top `:719–725`），仍不得越过原 parent 的 total；qwriter 保留完整 native parent header，所有子 B 聚合后才交原 B，下一原 AW 才能重捕获（`:125–147`）。真实 READ/WRITE prefetch 证书仍是单 job 固定 key，两页 success 不替代 raw parent B 的正常退休。phase1 提前返回 A terminal 的许可仍以完整 WRITE256/private容量为前提，copy done 仍由原 DMA 写完成路径决定。

## 5. 观察与接受范围

`early_moved=1` 只证明 main metadata 曾提前移交，不能证明 A AR 实际在 V 写闭合前被接受。源 `:449–450` 的 `parallel_overlap_ars_q` 仍只累计 phase2 AR 接受，漏计这次新增的 phase1 A AR。因此评估 early overlap 应看实际 source AR 握手沿与当时 `rawWV<J`；不能用该旧 observer 的 0 宣称无提前 AR，也不能只用 early_moved 宣称真正隐藏了延迟。通用 child_count 仍按单物理 AR 实际握手计数，功能没有消费它。

新增实际功能状态是一个 early_moved bit，phase gates/比较也有组合成本；没有新 payload buffer。它只改变独立 A prefix 的最早 offer/read 时机，工作量公式仅在同 E/FA 与相同最终 route 下保持，不能宣称所有配置固定同 AR 数或收益。

因为这个动作是已到数据债与旧 R route 分离后进行普通 AR 重叠，单独的提前 AR 不能作为新算法贡献；本稿不提前接受未来 in-flight join。当前正常合同以外的 physical READ error 未证明：新增 phase1 A 读取时，源 `:462` 的 fatal 只在 phase>=2 生效。零周期 AR/R、重映射/撤销、多 job epoch、错误恢复、持续到达的公平界、面积或板上行为均未验证。
