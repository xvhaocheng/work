# §56：已有数据闭包与读 membership 退出

Codex 独立关键推演与实际源码核查。仅读源码、写本稿；没有运行、编译或改功能 RTL。根在本次核查期间接入了新门，以下采用核到的新 `promised_end/next_end` 接口与代码，运行结果由根另行审定。

**裁决：新退出式可以解除 A 首 WRITE parent 只有 1 拍时的旧环，且不需要等待 A 旧 B。** 成立范围是当前单顺序 2KiB DEFAULT copy、所有地址 8B 对齐、全字节有效、真实 READ/WRITE 授权、V 全长 sponsor、每流只有一个不可撤 qW child、正常最终服务。退出表示不再需要未来 A logical R；它不表示 A 的 W/B、原 parent 或 backend 已退休。强普通共同裁剪 R/W 也能消除此死锁，必须保留该对照。

## 1. 实际 R/W 握手能不能当“已有数据”

记 job 全局拍号：`r=returnedA`，`w=raw_writtenA`；都是已握手前缀末端，不是计划量。

- source 控制器只在 `s_r_valid_o && s_r_ready_i` 时增加 `returned_q`，并清当前 member 的 skid 位：`shared_pretl_read.sv:238–242`。top 把 ready 接到 `mux_mst_req.r_ready`，把该 R 接回 mux 响应，再由 qwriter 原样传回 DMA：`multi_actual_dma_top.sv:949–959`、`:659–667`、`q_write_splitter.sv:42–43`。本次 top 的 mux 实例不覆盖 SpillR：`:833–846`。
- IOMMU vendor `cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:55` 默认 `SpillR=0`；`:446–462` 的 bypass 把当前 RID 对应的 DMA R-ready 直接接回。另一份 `cva6/vendor/pulp-platform/axi/src/axi_mux.sv:54,370–386` 也相同；两份源码都直接核过，不能混用行号。因此这里 controller 的 logical R 握手就是实际 iDMA 接受 R 的事件，不能随意推广到新增 R spill 的接口。
- `idma_axi_transport_layer.sv:255–266`：`axi_req_o.r_ready=in_ready & r_dp_ready_i`；DEFAULT 模式下，同一次真实 R 握手对所有有效 byte lane 置 `buffer_in_valid`。`:232–233` 只按 shift 对齐。当前 src/page 对齐、dst offset 0/3072/4088 都是 8B 对齐，长度 2KiB，因此每次 accepted R 都是完整 8B、shift 为 0。`:188–219` 的首尾 mask 在此合同不丢 byte。
- 实际 payload 保存在 `idma_buffer.sv:33–50` 每 byte lane 的 FIFO；没有 flush。`idma_axi_transport_layer.sv:343–357` 仅当整拍所需 lane 都有效且 raw W-ready 真握手时 pop。因此不能把 WVALID、AW offer 或物理 W 观察器当作该 FIFO 已消费量。
- qwriter 的 upstream W-ready 与 downstream W-ready 使用相同条件，`q_write_splitter.sv:47–50`；`:95–98` 的 `got_q++` 正好对应 raw W 握手。`:29–30` 的 `raw_written=parentBase+offset+(child_live?got:0)` 因而数的是已从 DMA 取出的 payload，并非等 DDR 物理 W 才增加。

所以在当前全字节顺序 copy 中，尚留在 DMA 的数据恰为连续区间 **`[w,r)`**，库存为 `r-w` 拍。已经交给 TC/spill 的 `[0,w)` 数据不再属于 DMA 库存，但仍在履行旧 W 义务；这不影响退出条件。

## 2. 为什么必须数本地 firstVALID child

qwriter 新 child 在 `:83–90` 注册 `child_q/count_q`，同时置 `child_live_q=1,offered_q=1`。次周期 `m_req_o.aw_valid=offered_q`（`:45–46`），即使 mux 尚未选中它，此 AW 已首次 VALID，不能撤回。全局 mux 的 `w_fifo_push` 只在它被 mux 首次选择时出现，不能覆盖这段本地义务。

新接口 `promised_end=parentBase+offset+(child_live?count:0)`（`:31–32`）正确包含 offered、accepted、部分 W 已送出及全部 W 已送出但 B 未回的同一个 child。记其绝对末端 `E`。未送出的旧 payload 数是 `E-w`；若 `r>=E`，则

`E-w <= r-w`。

全部旧 child 的未送出 payload 已经在 DMA，或者已被取走交给 TC。剩余动作只有合法 AW/W/B 的继续服务，不再依赖任何未来 A R。这里 B 是否回来没有参与判断，正是应该删除的假依赖。

反之，只看 global coverage 会漏掉还在 mux 前 held 的本地 AW：可有本地 A qW2 已 offer、global A coverage 仍 0、A 尚未收到数据。若据 coverage 0 清掉 A R 位，该 AW 后来进入共同 W 队首，却没有 A 数据；V 私有 FIFO 会因该 W 队首阻塞而填满，A fill 又被 V RLAST fence 挡住。所有权限正常、memory 公平也无法修这个闭环。`promised_end` 修的是这个确切承诺窗口。

## 3. F1/q2 的可达事件链

当前 A dst offset4088 的首 raw AW 只有 1 拍，V 首 qAW 为 2 拍；设共同 W 顺序为 V2 在 A1 前。

1. V2/A1 已本地 offer、随后全局 offer；真实 WRITE recipe 均成功。source 仅广播 R0，两个 logical R 都真实入各自 FIFO。A `r=1`、`E=1`，A 的 W0 因 V W 队首尚缺 R1 而等待。
2. 旧规则等 A B，R1 又禁止给越过首 WRITE 前沿的 A，因而 `A B → V WLAST → V R1 → A detach → A B` 成环。
3. 新 `shared_pretl_read.sv:168–182` 先置 pending，再在 `!a_skid && r>=E` 时清未来 A R 位，不等待 A B。R0 的 held logical 返回仍要真实握手；当前 A skid 位没清前不会退出。
4. 退出只改变后续 physical R 的 recipient mask（`:89–90`）；旧 AR 与已 offer 的 logical R 都不变。V 能接受 R1，V2 的 WLAST/B 得到正常服务，随后 A1 的 W/B 得到正常服务。
5. A 首 raw parent 的 B 返回后可捕获次 raw AW。`idma_axi_transport_layer.sv:410–438` 的 WLAST 由自己 W 长度计数，`:451–455` 的 B 接受不等 logical whole-AR RLAST；`idma_backend.sv:519–532` 的 NO_ERROR_HANDLING 路径允许非最后 W parent B 正常消耗。因此 A 次页 preparation 不被整个 logical AR256 的 RLAST 人为锁住。

该链证明删除了 payload/B 环，不是有限 cycle 上界。后续 TL/PTW、DDR W/B 仍需正常最终服务；跨 RID R 的时隙策略另由根的 actual driver 验收。

## 4. 退出后新 W 的例外是安全的，但须用绝对末端

根最新 top 的门为 `!WRITE_recipe_valid || (A && source_a_hold && next_end*8>source_returned)`，`multi_actual_dma_top.sv:698–705`。`q_write_splitter.sv:33–37` 以当前实际 parent base、offset 和将要发出的 count 算 `next_end`；`:83–84` 又要求 nonzero count。因此允许的是 **完整新 child 的 payload 已有**，不是仅“FIFO 非空”或 `count<=returned`。

在无 child 时，前一 child 的 B 已返回，`w=start=parentBase+offset`。所以 `next_end<=r` 等价于新 child 长度不超过真实剩余库存 `r-w`，不会重复使用 `[0,w)` 已消费数据。新 child 成为 firstVALID 后，其末端仍不超过 r，退出条件继续成立。

这也封住“退出和新 child 生成同一时钟边沿”的竞态：pending 时原 `a_hold` 已为 1（`shared_pretl_read.sv:80–81`），同拍新 child 若被例外放行，必有 `next_end<=r`，下一拍新的 promised_end 仍被已返回数据覆盖。单 child 限制 `!child_live&&!b_valid` 防止并发再加第二笔义务。

若 detached 时 `keep_q=0` 把 child 放大为整个剩余 parent，现门会针对这个**放大后的** `next_end` 检查；不能拿 nominal q2 的末端准许实际 long AW。当前 `next_count` 计算已满足此条件。

## 5. held、B、跨 parent 的边界

- `hold_i` 和 `child_quantum_i` 只作用于新 child 构造。已存在 child 的 AWVALID、WVALID、WREADY、BREADY 不引用这两个门（qwriter `:45–50`、`:83–98`），故退出、前沿更新或 quantum 变 0 不撤已 held AW/W。`:65–70` 的 held 检查可抓稳定性问题，但本稿没有运行该检查。
- source `!a_skid` 而不是只看数量确保已经接收、尚欠 A 的 logical R 不会被丢弃。若 skid 只剩 V pending，退出不修改其 payload、owner 或 RLAST；物理 RREADY 原先低、退组后升高是合法的，不能改变 memory 已 held 的 RVALID/data。
- child 全 W 已送出但 B 未回时 `got=count`；`raw_written=promised_end=E`。退出安全，但 W/B 在 TC、memory 或响应路径尚未退休，不能把 detached 当 qwriter quiet、physical quiet 或 backend idle。当前调试 quiet 仍另与 `q_quiet` 相与，top `:647–648`。
- child B 边沿后 `offset+=count, child_live=0`（qwriter `:100–106`），raw_written 从 `base+oldoffset+got` 变成 `base+newOffset`，数值不退。actual parent B 期间仍保留旧 base/offset；`:108–109` 后才清 parent live。下一顺序 raw AW 的 base 等于前 parent end，offset 归 0（`:80–81`），全局 raw_written 继续相同前沿，不把新 parent 局部 0 错当 job 全局 0。
- 同时 raw B+下一 raw AW capture 被 `aw_ready=!parent_live&&!b_valid`（`:44`）排除；这是当前 generation/key 的一拍无效窗口，不能为了流水化消掉而仍用相同 seq。此稿不复做 helper ABA 审核，只保留该前提。
- 上述库存等式依赖 actual legalizer 顺序、full-byte copy 与单 job epoch。已有 qwriter 检查不独立验证所有 parent 地址严格连续。多 job、未对齐 byte mask、错误/flush、独立 WRITE data、跨 job cursor 复用不在证明范围；这些情况不能只用拍数 `r-w` 代替逐 byte 数据覆盖。

## 6. 强普通反例与可接受增量

最新普通 mode2 使用各 qwriter 的 job 全局 start，裁到 `common_end=min(FV,FA)`；若 `common_end<=start`，child quantum 置 0，top 的 `write_child_quantum` 块及 qwriter `:37,:84` 防止零长下溢。source mode2 同时把物理 R child 裁到两成功 WRITE 前沿。

F1 时普通先发 V1/A1，而不是 V2/A1；R0 满足两 WLAST/B，首 parent B 后准备 A 次页，再继续共享源 R。这样没有旧环，且保持原 logical parent/RLAST/B 语义；V 第一段是原 raw AW 的内部子段，不是伪造 job completion。qwriter `:96–97` 检查 raw WLAST，`:48` 另产生 child WLAST，`:106` 只在整 actual parent 最后 child B 才合成 raw B。

按 q2 且共同边界只在拍 1 处错开，普通 V 的分段是 `1+127*2+1`，129 个 AW；候选 V 是 128 个 q2 AW。普通完整共享两段源可只读 N 拍，候选若 A 在拍1退出、V旧 N 拍继续、A最后补 N−1 拍，则共 `2N−1=511` 拍。普通少重复 R，候选能让 V 不等 A 下一 WRITE recipe；这里存在应实测的 V 进展与整体工作量取舍，不能只拿旧 fixed-q2 死锁对照宣传改进。

因为该共同裁剪普通控制能消除同一环，“取消等 B、按已有数据退组”单独不构成已经证实的新算法。若实际同信息、同完整成本比较显示它在 V/A 进展与重复工作之间达到普通共同裁剪、普通 prefix/suffix 同数据退组无法达到的具体点，才有相应微架构/调度增量可论证；当前只接受本轮安全接口修复与可达机制，不预告实验收益。

## 7. 本次必须保留的验收项

本稿未执行这些项。根的运行至少应分别确认：旧 F1/B-quiet 的实际环；新 mode1 V/A 完整 copy 与 bytechecks；普通 mode2 同时裁 R/W 后完整 copy；全局 promised/raw_written 跨 A1→A255 parent 不倒退；退出时 A skid 已空、promised_end<=returned；held AW/W/R 稳定；所有旧 B 得到真实响应且 raw B 仅一次。性能结论还须区分 source payload、AR、各流 AW/PTW、V/A done，不能以 detached 一位宣称全资源释放。
