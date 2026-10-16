# §50：目标 WRITE recipe 与提前恢复 fence 的关键审核

作者：Codex；2026-10-09。直接只读审核当前 out 副本的 shared_pretl_read、parent_translation_reuse、multi_actual_dma_top、multi_banked_top；因 row1 生存期直接依赖 q writer，另读 q_write_splitter。没有执行构建、DUT、脚本或改 RTL。运行数字/失败现场由根席提供，本席未重复统计日志。最后一次文稿同步依据根席提供的 build06 两项 guard 变化与 run12/13 状态，按要求没有再次读 RTL 或运行。

## 审核结论

接受新增四 recipe 的门在**每次 reset 仅两个正常、page-aligned 2KiB job，SameBank=1，静态配置/映射、正常许可成功**的范围内切断 run02 所见“长 source R held → destination DDTW 在其后 → 下一 q W 无法供应 → 长 R 无法前进”的循环。关键增量不是只预热 IOTLB：每流真实 source READ 和真实 destination WRITE 的结果分别存在 still-live row 中，所有后续同页 q/suffix WRITE 由该 row 返回 PA，因而不再依赖目标页留在 IOTLB。

也接受新 `detached && !V_done_R` 的 A 新 AW fence。它阻止提前恢复时 A 的剩余 long AW 排到仍需读数据的 V 剩余 long AW 前；门只作用新 child 创建，已有 AW/W VALID 不因该门撤销。

上述是源码与受限因果审核，不是通用 fault/ATS/invalidation/multiple-job/跨页持续进度定理。根席报告新 fence 后 run12/13 双方 done、完整 byte checks 通过；最终 physical AW 统计为 A254/256 而未通过，严格计数验收仍待修正。因此这里不将 run12/13 标为完整 PASS。

## 1. row1 是真实目标 WRITE 键，不是 source.w

[multi_actual_dma_top.sv:966](/home/xvhaocheng/work/iommu-platform/out/shared-read-rtl-20261009/multi_actual_dma_top.sv:966) 明确从每流 q writer 私有接受的原 AW 取 row1：

```systemverilog
assign raw_aw = gen_no_pool.gen_q_writer[f].i_q.parent_q;
assign reuse_parent_write[f][n] = n == 1;
assign reuse_parent_valid[f][n] = i_pre.active_q[f] &&
    ((n == 0 && i_pre.pa_q[f] == 1 && i_pre.done_q[f] == 0) ||
     (n == 1 && gen_no_pool.gen_q_writer[f].i_q.parent_live_q));
```

976–980 的地址/DID/PID/PID-valid/privilege 对 row1 全取 `raw_aw`，对 row0 才取 source AR。974–975 的 row1 bank 在 SameBank 下为 0；真实 TC 也选 bit3 为 bank。helper [hit_bypass_parent_translation.sv:71](/home/xvhaocheng/work/iommu-platform/out/shared-read-rtl-20261009/hit_bypass_parent_translation.sv:71) 按 row 类型生成 `UNTRANSLATED_W` 或 `UNTRANSLATED_R`，完整键仍含 DID、PID、privilege、bank；并经 [multi_banked_top.sv:813](/home/xvhaocheng/work/iommu-platform/out/shared-read-rtl-20261009/multi_banked_top.sv:813) 接到真实 TL 的 memo 请求接口。

helper222–230 只在原请求键、live parent、generation 一致且真实返回无 error/ignore/MRIF、低12bit一致时 fill；103–106 的成功证书按 row 选 `w` 或 `r`，或者允许架构 bypass：

```systemverilog
!value_q[f][n].error && !value_q[f][n].ignore && !value_q[f][n].is_mrif &&
((parent_write_i[f][n] ? value_q[f][n].w : value_q[f][n].r) || value_q[f][n].bypass)
```

所以 row1 WRITE 权限是实际 destination WRITE 翻译结果；A/V 各自取得，source 结果的 w 位没有被当作目标授权。`prepared_q` 只表示尝试，不能满足成功门。WRITE 失败没有有效证书，首共享 R 不会发；当前 controller 只对 row0 failed 显式 fatal，不提供通用 WRITE fault 完成处理，不能称为 fault 模式通过。

## 2. 原父生存足以覆盖全部后续 q / resume suffix

[q_write_splitter.sv:56](/home/xvhaocheng/work/iommu-platform/out/shared-read-rtl-20261009/q_write_splitter.sv:56) 原 AW handshake 后复制完整 header 到 `parent_q` 并设 `parent_live_q`。直到所有子 B 后产生原 B，且原 B 被 iDMA 接受时，88–89 才清 parent_live。它在暂停时不会因 quiet 清原父。

所以 A 在退出后仍有未返回 source suffix 时，A 原长 WRITE 必然尚未完成；row1 继续 live。V 返回最后 source beat 后，row0 可失效，但 row1 要继续保留到目标 W/原 B 完成。source controller 的 detached 后独立补读分支没有再检查 row1 成功；在此单一父、静态无 flush 的合同内，第一次共享 AR 已要求其成功，而父生存维持其有效，故可用。若允许 invalidation、disable/reconfigure 或换 generation，这个推导不成立，不能仅靠一次启动门。

top963/968 当前 head/seq 均固定 0。shared read 的 pa/returned/done、q writer group 计数也没有在第二个 job fire 重新初始化，因此接受范围是**本轮每次 reset 的两个单 job**；“每 job 一个 parent”不能被误读为此实现已支持连续多个 job 或 epoch 自动更新。

## 3. 同页 offset reuse 返回的 PA 与真实 TC 子请求一致

[hit_bypass_parent_translation.sv:125](/home/xvhaocheng/work/iommu-platform/out/shared-read-rtl-20261009/hit_bypass_parent_translation.sv:125) 的 WRITE 命中：请求除 IOVA 外的完整键相同、live generation 相同、IOVA 在同一 4KiB 页。182–184 为 TC response 增加实际 offset：

```systemverilog
hit_resp_q <= unpack_normal(value_q[reuse_owner][reuse_index]);
hit_resp_q.spaddr <= value_q[reuse_owner][reuse_index].spaddr +
    56'(s_req_i.iova-parent_addr_i[reuse_owner][reuse_index]);
```

首版 q writer57–59 强制原目的 AW 页起点为 0、len255/size3/INCR/cache2/无 lock；后续 child 从原 AW 加 offset，count 为 q 或 detached 后 remaining。因此全部目标 byte 区间在同一页的前2048B。prepared PA 的低12bit也必须等于原 IOVA，故 `PA_base + delta` 给出各 q 起点，末端不跨该4KiB页；resume long suffix 同样成立。真实 TC 每笔 q AW 的 header/W/B 义务还在，只有它的翻译 response 由成功 recipe 命中返回，未绕过实际 TC 的 AW/W/B 路径。

最终 guard 同步：根席在 build06 将 reuse predicate 从 `valid_q[f][n]` 改为 `cache_valid_o[f][n]`。因此首共享 R 的四证书门与每次 TC response reuse 都检查正常 READ/WRITE 对应权限，不再只靠启动门或保存位。q splitter 的 raw AW 捕获还显式拒绝 `mmu_atst`、`atop`、`prot[2]`，收窄到正常 untranslated data WRITE、无 atomic 的合同；这两项最终变化由根席提供，本席没有重复核源。实际 [idma_legalizer.sv:354](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:354) 也把 destination mmu_atst/mmu_valid 均置0。通用 translated-W 不匹配该 row，不能承诺无后续 miss。若原基址不在页起点，同页判定也不能免费推广上述非负 delta / parent 范围证明。

## 4. held background / slow 与 WR hit 没有被新增接口吞掉

helper153–159：held `prep_offer_q` 的 m_req 固定为 slow_req，不依赖 hit_q 撤回；当存在 background slow 时，成功 row 的 normal hit 可独立进入 hit_resp。background response 被内部 ready 消费，`slow_prepare_q` 抑制其直接返回 TC；hit_q 则稳定保存其 TC response。179–184 不覆盖 slow_req，218–230 按 slow ownership 填对应 row。现有60–61 assertion 也检查 held background request 不变。

允许 overlap 的是后台 offer/后台 slow 与缓存命中；formal normal slow 仍串行，不能把“hit 可绕 background”理解为任意两笔未记账的 TL normal 请求同时进入。当前四个成功证书出现后，长 R 才会成为 DDR head：后续 WR 同页命中无需再走真实 TL，即使 IOTLB 替换也不会引入第二次 DDTW。flush、外来 requester、跨页或改变请求类型不在这条闭包里；仅“内存公平”不能替代证明。

## 5. 提前恢复 fence 的顺序证明与不可撤义务

根席报告 run08/09：resume 先清 pause，A 提前 offer 剩余 long AW，排在 V long W 前；但 A fill R 要等 V source R 完成，V 小 FIFO 因 A W head 无法排空，出现真实 watchdog。当前 [shared_pretl_read.sv:54](/home/xvhaocheng/work/iommu-platform/out/shared-read-rtl-20261009/shared_pretl_read.sv:54) 新门是：

```systemverilog
assign a_hold_o = pause_i || (detach_pending_q && !detached_q) ||
                  (detached_q && done_q[0] == 0);
```

必要事件链是：V 最后 logical R 可交入 iDMA → coverageV 已达到2048B → V 剩余 long AW 已在全局 mux first offer 并 push owner FIFO → V done_R 后才准 A 创建新 long AW → A 的新 first offer 晚于 V。coverage 只由全局 virtual AW first offer 增加（source controller121–123、top939–943），R ingress逐 beat检查 coverage（68–71），done_R 仅在 V 自己最后 logical R handshake 设置（181–185）。因此等待 logical RLAST 不是等待原 job/B 全退休，而是用真实数据依赖强制 V long AW 先取得 W 序；SameBank 的本地 AW FIFO/normal spill 保序使 physical W 序同样 V 在 A 前。V 最后 logical R 交入私有 FIFO时，其物理 RLAST已取走，旧长读也不再占 memory head。

q writer63–69 的 hold 只禁止创建新 child；29–34 的 offered AW / 已准入 W/B 都不看 hold。pause 前已经注册或首 offer 的 A q 因而仍兑现，只有其退休且无 A skid 时才 detach。detach_pending 门与 detached/V-done 门相接，没有提前 resume 后的一拍放行空隙。该 fence 不要求暂停方撤销已首 VALID 的 AR/R/AW/W，也不把 A 原 parent 或原 B 当成已完成。

## 6. 原 AxID 表述更正及验收范围

两真实 DMA 的 AxID 实际均为 **0**（当前 top390/449）；输入 mux 添 flow bit 后原 source ID 是 **0/4**，不是第二 DMA raw AxID=6。SameBank 选择 **bit3**（top995），因此两流均 bank0；bit2仍区分 owner。direct physical source AR 才额外 OR bit0，返回路由再去该 bit。此前如果把观察到的某个 completion ID 当作原 raw AxID，应以本处及实际实例为准。

根席提供 recipe 修复后的 no-pause 和 pause-copy PASS，只说明相应运行；本席未运行。新 fence 的 run12/13 在 delayed B / 周期 READY backpressure 下已双方 done、完整 byte checks 通过，但严格 physical AW 统计仍失败：根席定位 pre-edge observer 只有 `fW && !caw_q.empty()` 才计数，会漏同周期新 AW 加首 W；posted-edge memory 的 AW push→W 处理顺序正确。DeepSeek 正在定向修复 C++ 观察器，保留严格256验收；这里不把观察器修复前的 run12/13 写成完整 PASS，也没有替根席核日志。当前未给出通用 fault 恢复、invalidation 后 recipe 生命周期、多个 parent/多个 job、跨页目标准备或跨 bank 顺序保证。候选/强普通 q-read 使用同样四 recipe 和 q-W adapter，新增目标准备/保存成本必须双方同计。
