# 本轮关键原文

## intel_drained_definition
RC取完主存返回数据是drain，不是DMA整页receipt
/home/xvhaocheng/work/iommu-platform/out/local-read-reclaim-20261015/controls/sources/intel-vtd-spec.txt
行 8129 至 8142
```text
8129: 6.5.4           Draining of Requests to Memory
8130: Requests from devices that are already processed by the remapping hardware, but queued within the
8131: Root-Complex to be completed to memory are referred as non-committed requests. Draining refers to
8132: hardware pushing (committing) these requests to the global ordering point. Hardware
8133: implementations report support for draining through the Capability Registers.
8134: 
8135: A write request to system memory is considered drained when the effects of the write are visible to
8136: processor accesses to addresses targeted by the write request. A read request to system memory is
8137: considered drained when the Root-Complex has finished fetching all of its read response data from
8138: memory.
8139: 
8140: Requirements for draining are described below:
8141:  • Draining applies only to requests to memory and do not guarantee draining of requests to peer
8142:    destinations.
```

## intel_page_domain_scope
官方domain/page范围相关drain保证
/home/xvhaocheng/work/iommu-platform/out/local-read-reclaim-20261015/controls/sources/intel-vtd-spec.txt
行 8169 至 8186
```text
8169:  • Read and write draining of untranslated requests are performed on IOTLB invalidation requests
8170:    specifying Drain Read (DR) and Drain Write (DW) flags respectively. For IOTLB invalidations
8171:    submitted through the IOTLB Invalidate Register (IOTLB_REG), draining must be completed
8172:    before hardware clears the IVT field in the register (which indicates invalidation completed). For
8173:    IOTLB invalidations submitted through the queued invalidation interface, draining must be
8174:    completed before the next Invalidation Wait Descriptor (inv_wait_dsc) is completed by hardware.
8175:        — For global IOTLB invalidation requests specifying DMA read/write draining, all non-committed
8176:          DMA read/write requests queued within the Root-Complex are drained.
8177:        — For domain-selective IOTLB invalidation requests specifying read/write draining, hardware
8178:          only guarantees draining of non-committed read/write requests to the domain specified in the
8179:          invalidation request.
8180:        — For page-selective IOTLB invalidation requests specifying read/write draining, hardware only
8181:          guarantees draining of non-committed read/write requests with untranslated address
8182:          overlapping the address range specified in the invalidation request and to the specified
8183:          domain.
8184:  • Read and write draining of untranslated requests are performed on all PASID based IOTLB
8185:    invalidation requests, where draining is completed before the next Invalidation Wait Descriptor
8186:    (inv_wait_dsc) is completed by hardware.
```

## tc_internal_handoff
COMPLETE按内部comp ready释放raw owner
/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv
行 716 至 726
```text
716:             COMPLETE: begin
717: 
718:                 complete_read = is_read;
719:                 complete_write = is_write;
720: 
721:                 if ((axi_comp_resp.ar_ready && is_read) ||
722:                     (axi_comp_resp.aw_ready && is_write)) begin
723:                     aw_ready    = is_write;
724:                     ar_ready    = is_read;
725:                     state_n     = IDLE;
726:                 end
```

## tc_following_spills
SpillAr/SpillR有原owner，内部handoff不同外部DDR接受
/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv
行 549 至 557
```text
549:             .axi_resp_t     (axi_comp_resp_t),
550: 
551:             .NoMstPorts     (32'd2),
552:             .AxiLookBits    (RVIOMMUCfg.AxiIdWidth),
553:             .SpillAw        (1'b1),
554:             .SpillW         (1'b1),
555:             .SpillB         (1'b1),
556:             .SpillAr        (1'b1),
557:             .SpillR         (1'b1)
```

## lr1_alias_scope
不同context/IOVA相同PA别名拒绝，而非一般多alias成功回收
/home/xvhaocheng/work/iommu-platform/out/local-read-reclaim-20261015/prototype/reclaim_protocol.py
行 250 至 277
```text
250:     def snapshot(self):
251:         jobs = self.target_jobs()
252:         old = sum(1 for t in self.tickets.values() if t['epoch']==self.target_epoch)
253:         for packet in list(self.held.values())+list(self.inflight.values()):
254:             j = self.jobs[packet['source'],packet['job']]
255:             old += int(self.is_target(j))
256:         # Count existing accepted-parent intervals not yet handed to transport;
257:         # closing admission cannot make their byte obligations disappear.
258:         for j in jobs:
259:             for burst in (0,1):
260:                 key = (j.source,j.job,burst)
261:                 if key not in self.accepted_ar and key not in self.held:
262:                     old += 1
263:         target = next(x for x in self.jobs.values() if x.source==self.target_source)
264:         aliases_closed = not any(j.src==target.src and
265:                                  (j.context,j.src_iova)!=(target.context,target.src_iova)
266:                                  for j in self.jobs.values())
267:         no_target_writer = not any(j.dst==target.src for j in self.jobs.values())
268:         return dict(request_epoch=self.target_epoch, current_epoch=self.current_epoch,
269:                     readonly=self.readonly and no_target_writer, nonoverlap=self.nonoverlap,
270:                     registry_complete=self.registry_complete, alias_safe=self.alias_safe and aliases_closed,
271:                     all_sources_sealed=bool(jobs) and all((j.source,j.job,j.epoch) in self.sealed for j in jobs),
272:                     all_readers_complete=bool(jobs) and all(j.read==4096 for j in jobs),
273:                     read_error=any(j.error for j in jobs), old_obligations=old,
274:                     authorization_revoked=self.revoked, cache_synced=self.synced,
275:                     invalidation_committed=self.invalidated, target_ordering_proven=self.ordered,
276:                     whole_copy_complete=bool(jobs) and all(all((j.source,j.job,b) in self.b for b in (0,1)) for j in jobs),
277:                     request_active=self.request_active)
```

## lr1_paid_adapter
普通选择不产生读者/receipt/hardware seal/PTE-cache-CQ事实
/home/xvhaocheng/work/iommu-platform/out/local-read-reclaim-20261015/controls/ordinary_policy.py
行 1 至 8
```text
1: """Independent, untimed LR1 ordinary control policies.
2: 
3: The frozen snapshot contains results of *paid, public* target primitives.
4: None of these policies discovers readers, measures real R acceptance, seals
5: hardware, or performs PTE/cache/CQ operations.  The common adapter must do so.
6: Only ``ack`` authorizes successful page release; ``error`` quarantines it.
7: Extra snapshot fields are ignored and cannot provide a private oracle.
8: """
```

## lr1_phased_policy
具名stage policy使用同付费snapshot
/home/xvhaocheng/work/iommu-platform/out/local-read-reclaim-20261015/controls/ordinary_policy.py
行 84 至 118
```text
84: def phased(snapshot):
85:     """Ordinary source-phase join followed by target-only unmap.
86: 
87:     The join is exact real-R receipt for every registered old-generation reader,
88:     plus retirement of all raw/walk/helper/OUT/held/accepted obligations.
89:     It can finish while nonoverlapping destination W/B continues.
90:     """
91:     facts = _validated(snapshot)
92:     if facts is None or not facts["request_active"]:
93:         return "wait"
94:     if facts["request_epoch"] != facts["current_epoch"]:
95:         return "wait"
96:     if facts["read_error"]:
97:         if not facts["all_sources_sealed"]:
98:             return "seal"
99:         if facts["old_obligations"] != 0:
100:             return "wait"
101:         return "error"
102:     if not facts["readonly"] or not facts["nonoverlap"]:
103:         return "wait"
104:     if not facts["registry_complete"] or not facts["alias_safe"]:
105:         return "wait"
106:     if not facts["all_sources_sealed"]:
107:         return "seal"
108:     if facts["old_obligations"] != 0 or not facts["all_readers_complete"]:
109:         return "wait"
110:     if not facts["authorization_revoked"]:
111:         return "revoke"
112:     if not facts["cache_synced"]:
113:         return "sync"
114:     if not facts["invalidation_committed"]:
115:         return "invalidate"
116:     if not facts["target_ordering_proven"]:
117:         return "order"
118:     return "ack"
```

## model_ds_priority
行为服务通过rotate已接受mem_q选DS，非已部署MIG接口
/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp
行 679 至 719
```text
679:         top->ddr_r_valid = 0;
680:         if (mem_q.empty()) return;
681:         // AXI allows R interleaving across different IDs. Promote only the
682:         // oldest DS request, retaining order within every ID and preserving
683:         // a VALID payload held from the prior tick. No DUT arbitration changes.
684:         // Each request retains its original latency/beat state after rotation.
685:         // The priority WINDOW is the legacy first-V-issue window in the legacy
686:         // path and the full-era V-completion count in full-batch mode
687:         // (v_transfers == 64 in the approved full configuration).  The held-R
688:         // guard applies to BOTH: a beat currently held MUST NOT be displaced.
689:         const bool ds_window = full_batch
690:             ? (full_era && full_done_n[0] < (uint32_t)v_transfers)
691:             : win_open;
692:         if (ds_priority && !ddr_r_held && ds_window &&
693:             ((mem_q.front().id >> 4) & 3) != 3) {
694:             auto ds = std::find_if(mem_q.begin(), mem_q.end(), [](const MemRead& r) {
695:                 return ((r.id >> 4) & 3) == 3;
696:             });
697:             if (ds != mem_q.end()) std::rotate(mem_q.begin(), ds, ds + 1);
698:         }
699:         MemRead& h = mem_q.front();
700:         ddr_setup_port = (h.id >> 4) & 3u;
701:         if (read_latency_overlap && measurement_phase) {
702:             // Same isolated-request delay as the old model: AR accepted at
703:             // t first returns at t + latency + 1. Queued requests age from
704:             // acceptance, allowing their preparation delays to overlap.
705:             if (g_cycle <= h.accepted_cycle + (long)ptw_latency) {
706:                 ddr_setup_wait = true;
707:                 return;
708:             }
709:         } else if (h.latency > 0) {
710:             --h.latency;
711:             ddr_setup_wait = true;
712:             return;
713:         }
714: 
715:         top->ddr_r_valid = 1;
716:         top->ddr_r_data  = read64(h.addr + (uint64_t)h.beat * 8);
717:         top->ddr_r_id    = (uint8_t)h.id;   // 6-bit shared-DDR ID echoed verbatim
718:         top->ddr_r_resp  = 0;
719:         top->ddr_r_last  = (h.beat == h.len) ? 1 : 0;
```

## native_fullbuffer
必须正常精确4096对齐，无64B例外
/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv
行 14 至 18
```text
14:     logic pending_q, eligible_q, fault_q;
15:     logic eligible;
16:     assign eligible = CapacityBytes >= 4096 && normal_copy_i &&
17:                       length_i == 64'd4096 && src_i[11:0] == 0 && dst_i[11:0] == 0;
18:     assign permit_o = pending_q && eligible_q && !fault_q && normal_copy_i;
```

## co1_equivalent_origin
OC1-min在读CO1卡后按普通模块构造，不是前存发表基线
/home/xvhaocheng/work/iommu-platform/out/completion-obligation-design-20261014/control/min-rule.md
行 1 至 13
```text
1: # OC1-min：固定的最小阶段引用计数 pin 对照
2: 
3: 2026-10-14，版本 `OC1-min-frozen-v1`。这是 root 点名“TTL/rank 是否必要”的有界后续任务：在 OC1-v1 保存冻结并读取 CO1 卡之后，按普通模块构造一份更小固定程序。v1 原件与配对保留，不挑场景赢家，不声称这份完整组合已被发表。
4: 
5: 结论：标准阶段引用计数 pin 不需要 per-key TTL 或 structural critical rank。下述程序用同一登记账本、阶段退休、release 布尔、登记年龄、RR 与一个唯一 defer token，实现 CO1 的有限两源合法动作及共同进度合同。两源限定确保“被越过者下次服务”指唯一另一个 head。
6: 
7: ## 固定范围和共同 paid 接口
8: 
9: `N<=4` 登记 task，`K<=16` 正式 page key，`B<=2` 真实有 owner 的数据 buffer，原 cache16，单正常 TL/PTW；仅 native source0/1，每源至多一个实际 eligible preVALID head。task 槽可顺序复用 source0/1；不增四源并发或改变 native buffer512/3、in-flight 容量。
10: 
11: 实际 HOLD 仅严格 aligned 64B DEFAULT；任意范围的两源合法软件 pre-kick/generic gate 属共同新增付费接口。未握手 VALID 也不能被登记当许可。形式 key/context/epoch、actual resident/invalid/fill、最后翻译用途 handoff、完整 R/W/B、CPU consumer/cache ownership、buffer owner/release、source/functional-job/task-slot 关联均按 `rule.md` 的相同严格含义计费。
12: 
13: 同一固定逻辑实现可容纳每 task 多页和重复使用，但必须有完整登记及精确最后用途证明；没有该证明则双方禁用提前解除。这里不是未来 PTE/DDR 查询、不是新 prepared translation pool。
```

## legacy_first_R_is_handshake

/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp

```text
1166:             if (f_ddr_ar) trace_event("ddr_ar", s_ddr_ar_i, s_ddr_ar_a, s_ddr_ar_l);
1167:             if (f_ddr_aw) trace_event("ddr_aw", s_ddr_aw_i, s_ddr_aw_a, s_ddr_aw_l);
1168:             if (f_ddr_r && mem_q.front().beat == 0)
1169:                 trace_event("ddr_r_first", mem_q.front().id, mem_q.front().addr,
1170:                             g_cycle - mem_q.front().accepted_cycle);
1171:             if (f_ddr_r && top->ddr_r_last) {
1172:                 const MemRead& r = mem_q.front();
1173:                 trace_event("ddr_r_last", r.id, r.addr, g_cycle - r.accepted_cycle);
```
