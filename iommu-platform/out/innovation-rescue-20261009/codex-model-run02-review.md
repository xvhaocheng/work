# §49 run02：根级诊断与最小修订合同

实际 child 输出 306 scenarios / 6835 checks / 143 failed。run02 JSON、log 均保留。记录包装器误把 ledger 对象当列表，发生在 child 完成之后；未重跑 child 恢复计时，ledger 明示没有保存其 start/elapsed/process-return-code，而保留结果报告 exit1。

## 1. 合法补读误报（139 项）

`eval_scenario:ingress_no_phantom_member` 将所有 `A_exited` 的 A 入队均算违规。但 `solo` 补读恰恰要求 A 保持 exited（不重新加入旧 shared R）且恢复可运行。只对 shared policy 的新 required mask 拒绝 exited A；所有 policy 仍需各 flow 的 offered_end 覆盖 beat。不能清除 A.exited 来让检查通过，也不能放宽 shared 冻结 mask。

## 2. 强普通对照 early_resume 的真实模型死锁（4 项）

`ordinary_tile_exit N16/B3/q2/P2/resume6`：t3 pause，t4 V first-offer W[2,16)，t5 A 原 W[0,2) 完成。t6 `do_resume` 清掉 pause，尚未来得及 `exit_check`，又让 A 接 common W[2,4)、shared R[2,4)。V 已不可撤销的长 W 等待 index4；A 下一短 AW 又等 V 完成当前 band，形成循环。完整 trace 由 Codex 直接执行/读取，控制须纠正，不能将这个损坏对照当新机制优势。

根架构决定：一旦 survivor 的 long W first-offer 已在 common band 之外展开，就不能因 early resume 重新加入 common tiled 读。把 resume 视为请求：先保持关闭新 A AW，完成旧 W 和已冻结 R 的义务，再正式 EXIT；如果 resume 已请求，EXIT 后立即转可运行 solo fill。未展开且未越过 A cursor 时可取消 pause 回 common。同规则用于 open/ordinary；不能修改任何 held AW/R header/mask，不允许提前丢弃旧 A debt。

实现可用显式 `detach_pending`，或在 do_resume / exit_check 中保存等价状态。`A.resumed` 只标记请求收到，不等于可以立刻 offer。V 长写是否已提交以实际 first-VALID header 为准，不仅 accepted。`read_idle` 等旧 physical child + skid 排空后才补读。两者的旧 Wband 未完成时也不许把 A 凭空当作 quiet。

## 3. 修正记账名称与完整完成检查

`aw_outstanding` 当前实际是 AW first-VALID 尚未 accepted 的计数；不能称它完整 unretired。真正 `aw_unretired_max` 用 `aw_first_valid - b_count` 在 first offer 时累积，one band 结构仍保证 <=1。每个已经 done 的 flow 须显式 `AWfirst == AWaccepted == WLAST == B == bands_completed`；未完成 flow保留先后不等式。

## 4. 补两个有界相互作用实例

在真实 Sim（open 与 ordinary）注入 A RREADY 低直到 t8、resume t4，以及 A AWREADY 低直到 t8、resume t4；N16/B3/q2/P0。两 flow 完成，payload/cursor/held header/mask 正确。实际 pause→drain→resume 的 deferred 情况，不能只测独立 toy 状态。

接受门：所有已有 306 场景不删除；增加上述 4 场景；所有 invariant 和 expected negative assertions 均 fatal。检查器只修错判，不改变 payload/physical counts。保留 run02 与原代码，返回小 patch，不重复返回整份代码/巨量 JSON 结果。
