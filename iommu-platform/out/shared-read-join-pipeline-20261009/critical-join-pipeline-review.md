# §67 MaxOutstanding 1/2：join closure 的必要协议审查

作者：Codex 独立关键源审。只读本目录实际 RTL、必要 AXI mux/iDMA 路径；没有构建、运行、改 RTL，也没有读取待跑结果。2026-10-09。§65 build02 的 20 个新配置完整 copy PASS 是 root 告知的上一阶段结果，不能支持本目录 Max2。

**结论：在本文限定合同下，没有发现 Max2 为 phase1–3 新增的协议或数据进展阻断。Max2 允许两个待 B 义务，仍只允许一个未送完 payload 的 child；这一区别使旧 closure / peer-W 证明可以延续。原 parent 仍等全部真实 child B 聚合、原 B 握手后才释放。接受范围必须保留完整目标 AW/W 最终产生 B 的正常服务假设；本文没有 cycle bound 或生产通用证明。普通写流水线不是创新。**

## 1. 被审核合同与实际位置

- [设计合同](codex-design-before-run.md:3)：§65 fixed source，Q2/B3，单个 static/immutable 正常 2 KiB job，mode3 V 独立 bootstrap、两 context 分别成功的 READ 和目的 WRITE recipe，first/second WRITE-page prefetch；无 fault/invalidation/pause/多 job。
- [q_write_splitter.sv:21](q_write_splitter.sv:21) 仅一份 `child_q/count_q/got_q`；[96–109 的入口](q_write_splitter.sv:96) 要求 `!child_live_q`，Max2 的额外能力是 `b_pending_q < 2`。不能把它解释为两份尚缺 R 的 W child。
- [multi_actual_dma_top.sv:730](multi_actual_dma_top.sv:730) 把 `WriteOutstanding` 传给 splitter；source phase 和 permission 合同没有因这一参数变更。
- phase1 新 V child 裁到 J：[top:711](multi_actual_dma_top.sv:711)；phase3 A-prefix 裁到 J、joint 新起点不超过 peer 实际 raw W：[top:718](multi_actual_dma_top.sv:718)。hold 只影响尚未注册的 child：[top:732](multi_actual_dma_top.sv:732)。

## 2. 不可撤 AW、旧 B 与 parent 生命期

关键实际代码：

```systemverilog
// q_write_splitter.sv:96–104
if (parent_live_q && !child_live_q && !b_valid_q && offset_q < total_q &&
    b_pending_q < MaxOutstanding && next_count != 0 && !hold_i && ...) begin
    child_q <= parent_q; ...
    child_live_q <= 1; offered_q <= 1; aw_accepted_q <= 0;
end
// :118–122：只有实际 raw WLAST 才释放 payload child
offset_q <= offset_q+count_q;
got_q <= 0; child_live_q <= 0; aw_accepted_q <= 0;
// :145–146：只有原 B 握手才释放原 parent
if (s_rsp_o.b_valid && s_req_i.b_ready) begin
    b_valid_q <= 0; parent_live_q <= 0;
end
```

1. `offered_q` 是本流注册后的 first-VALID 承诺，比全局 mux 选择更早。hold/phase 变化不撤掉它：[q:56](q_write_splitter.sv:56)、[q:78](q_write_splitter.sv:78)、[q:111](q_write_splitter.sv:111)。`promised_end_o` 已计入该 live child 全长，不能只统计 accepted AW。
2. `b_pending_q` 在实际 splitter 输出 AW 握手增加、真实返回 B 握手减少；同拍二者发生计数不变：[q:125](q_write_splitter.sv:125)、[q:133](q_write_splitter.sv:133)。每流同一 captured parent/ID，B 计数加 error joining 足够表达这些旧义务；本文正常成功合同不扩展 error 聚合优先级。
3. 注册新 child 时 `pending<Max`，且此后不会再注册第二个 payload child；所以即使该 AW 暂未接受，`pending + offered <= Max`。同拍新 AW/B 不产生漏债。首 AW 和零延迟 B 也不能错误计入：pending0 时 BREADY 为0，合法 B 需保持到可握手。
4. `raw_written` 在真实 raw WLAST 时从旧 `offset+got` 连续变成新 offset；`promised_end` 同样从当前 child end 变成已消费前缀：[q:37](q_write_splitter.sv:37)、[q:40](q_write_splitter.sv:40)、[q:114](q_write_splitter.sv:114)。旧 pending B 代表的 payload 已被真实 raw W 消费，不属于未来 R 数据债。
5. 最后 payload 完成后，原 B 仅在 pending 全清时产生：[q:130](q_write_splitter.sv:130)、[q:141](q_write_splitter.sv:141)。最后 W 与最后 B 同拍，判断使用旧 `child_live/offset`，次拍 fallback 补出原 B；不会丢 B，也不会提前释放 parent。AW2 capture 的 AWREADY 必须等原 parent/B 释放：[q:55](q_write_splitter.sv:55)。因此旧 B 不会被新 WRITE parent 的地址/len/context 覆写。
6. 页间 B bubble 保留旧 total/header；下一 parent 的 job-relative base 恰为旧 F，cursor 不回零：[q:33](q_write_splitter.sv:33)。实际 prefetch slot1/2 是 job-life 固定键，并非用提前捕获 AW2 伪造 WRITE 成功；[top:1045](multi_actual_dma_top.sv:1045) 仍核原 WRITE context。[top:1055](multi_actual_dma_top.sv:1055) 的 full256 recipe 仍需 A 两页成功。

`quiet_o = !child_live && pendingB==0` [q:32](q_write_splitter.sv:32) **不是 parent metadata quiet**：原 BVALID 尚未被 iDMA 接收时它可为真。当前 phase1 数据 closure 没有用它冒充 parent 释放。`pending_o` [q:51](q_write_splitter.sv:51) 现在明确是 accepted 待 B 加本地未接受 first-VALID；不是未耗尽 payload child 数。

## 3. 真正的 W 顺序和两层退休

input [axi_mux.sv:264](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:264) 使用 LockIn；[301](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:301) 在 mux 第一次 offer 时就 push W-owner FIFO，不等外部 AWREADY；[353](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:353) 仅让 FIFO head 的 W 握手，并在该 head raw WLAST pop。top [865](multi_actual_dma_top.sv:865) 的 MaxWTrans8 对此窄两流最多4个未 B 完成的 child 足够；B 等待不重新插回已 pop 的 W-owner token。

这里的 raw AWREADY/raw WLAST 不等于 DDR 外侧 AW/W 退休。已有正常 same-bank TC/FIFO/spill 可以保存旧 AW/W；其物理发送可能落后于 input pop，但仍保序。本文不把 TC COMPLETE 内部握手当成外部 physical AW acceptance。Max2 没有提供跨 bank/order inversion 的新证明。

iDMA 对原 logical R 的真实握手才 push payload FIFO，并以原 logical RLAST 退休 read metadata：[transport:254](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:254)。原 W metadata 在真实原 WLAST 推进：[transport:392](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:392)；原 B 经 splitter 聚合后才到 iDMA：[transport:450](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:450)。NO_ERROR_HANDLING 正常 copy 的 job response 仍由最后 WRITE B 决定：[backend:519](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:519)、[637](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:637)。logical RLAST 允许早于 WRITE1 原 B，不能早报 whole-copy 完成。

## 4. phase1–3 的必要进展条件

**phase1。** start 的 J 已覆盖本地 registered promise：[source:103](shared_pretl_read.sv:103)。同沿 start 冻结新缺 payload V child；仍可创建的全 payload child，其 end≤returned≤J，故不会把 promise 推出 J。后续新 V child 截到 J，odd J 的最后 q1 合法。closure 条件是无 skid、`returnedV=rawWrittenV=J` 且 promise≤J：[source:245](shared_pretl_read.sv:245)。此时 pendingB 可为1或2，但旧 child 已没有 R 数据债。若 pendingB 满而仍未写到 J，新 child 会暂等旧 B；这是需要正常 B 最终服务的停顿，不能据公平 R/W 仲裁声称固定 cycle bound。

**phase2。** 源仅在 closure 后 sink 旧 physical child；[source:142](shared_pretl_read.sv:142) 不受 logical FIFO/B 等待限制，仍保存原 physical ID/len 和检查真正 RLAST。[source:308](shared_pretl_read.sv:308) 仅在实际 physical RLAST 清 busy，repair builder 要求 offered/busy/skid 全空：[source:249](shared_pretl_read.sv:249)。如果 phase1 末正好 E=J 且 physical busy 已0，进入 phase2 后可直接发 repair；无需等待不存在的第二个 RLAST。旧 B 可与 sink/repair 并存，不可被取消。

**phase3。** 原 peer gate 看 actual raw W，而不是新的 issue-group/B 计数：[top:726](multi_actual_dma_top.sv:726)。每流仍最多一个未耗尽 payload child，且该 child 长≤2；新 child 的 start≤peer 已消费 W，则其 end≤peerW+2。shared R 因 private lead2 停顿时，已有全局 W head 所需的≤2个 payload 可通过 R 到达或已在 FIFO；旧 pending B 不增加该 head 的缺数据长度。R skid 逐成员交完才取下一 beat，`returned−rawWritten` 与实际 FIFO 容量对应。以上是有限进度论证，仍需正常 FIFO/TC/W/B 最终服务。

该证明不要求 J 偶数，也不要求跨 A native F 后两边始终同 q2 parity。A-prefix 裁到 J，joint 的实际 peer 游标门和 q≤2 才是关键；最后 q1 由原 parent total 截短。Max2 issue-group 控制在 mode3 被 `detached_i || mode3` 旁路：[top:735](multi_actual_dma_top.sv:735)，不能把 group 数增速当作本模式的安全依据。

**F255 必须保留已有 terminal fix。** [source:131](shared_pretl_read.sv:131) 在 full-authorized phase3 允许最后 source beat 私有停车，不再要求尚不存在的 AW2 coverage。否则仍有原 §59 环：V qW[254,256) 缺 R255，A qW[254,255) 被 V head 挡，A 原 B→AW2 等旧 W，而 R255 又等 AW2。Max2 本身不能修这条环。当前代码保留修复；A 原 parent255 的全部 B 清后才可捕获 AW2，而 parked R255 供其最后 q1。

## 5. 可推翻本结论的接口变化，不是当前已发现 bug

- 如果以后允许两份尚缺 payload 的 child，却仍只用当前一份 `count_q` 计算 promise，phase1 可漏掉第二笔 first-VALID AW 的 end。例：已有 [0,2)、新增 [2,4)，closure J2 后 sink 会丢第二笔必需 R。当前 `!child_live` 入口禁止此例。
- 如果仅因最后 raw WLAST 就释放原 parent，旧 pending B 会在 AW2 capture 后被错误归属；相同 ID 的计数不能弥补被覆写的原 header/context/error 合同。当前原 B 聚合/握手门禁止此例。
- 如果恢复 phase3 的 last/frontier AWcoverage 特判，F255 可再现上述真实死锁；pendingB2 不构成解法。
- 若目标可要求后续新 child 的 R/W 才给已完整接收的旧 AW/W 返回 B，pending 满时会停住。本文正常 memory 进展合同排除这种跨 child 依赖；AXI ready/valid 规则自身不承诺有限 B 延迟。

## 6. 成本、比较和未验证项

Max1→Max2 的 `b_pending` 位宽由1增为2；每流增加1 functional bit。`peak_b_pending` 同样增加1 observation bit/flow，必须单列而不藏入控制面积。没有新增 payload child/header FIFO。完整综合面积、时序、实际 outstanding 峰值均未验证；同 J 策略两边须配置同 Max、同 observer/权限和数据容量。

普通 burst-end catch-up 也可使用这份 Max2 splitter、同 peer gate 和完整 permission合同，故流水线改善本身不构成独立贡献。只有同 E/FA、同先前 physical reads 时，最小 J 与 E 策略的 source work 仍为 `E + (256−FA)`；Max2 会改变触发时刻和实际 E/J，不能拿固定公式替代实际计数，也不能推出任一策略对 V/A 都不劣。较早 J 的原收益是否主要来自单 W/B 串行限制，留给 root 对两策略同 Max1/2 的真实 byte、AR/R、qW/B 峰值与完成周期验收。构建进行中不等于通过；本审查不引用未来仿真结果。
