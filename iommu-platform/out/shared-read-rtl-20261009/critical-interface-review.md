# §50：共享长读接真实 iDMA / IOMMU 的关键接口审核

作者：Codex；2026-10-09。只读核源和纸面并发推演；本席没有修改 RTL、构建或运行 DUT。根席负责候选实现及实际验证。本审核采用根席收窄后的首版：SameBankDmasEnable=1、每流一个正常 2KiB source raw parent、单页静态 DRAM 映射、输入在 copy epoch 内不修改、CONF129 同用于候选和强普通控制、q 不超过该流实际私有 payload 容量。

## 结论与最小可用位置

这个窄合同可以接，但不能只把现有 completion AR 接口换成双播器。最小需要三个真实接口改动：①每流 raw iDMA AW/W/B 出口、全局 AXI mux **之前**放透明 q-W adapter；②用新的两 parent read controller 维护各自 logical R cursor，消费各自真实 TL READ recipe，并在现有 direct completion AR/R 位置发一笔 PA 相同的共享长 R；③直接向两流 raw iDMA response 端返回独立 R，不向未接受这些 source AR 的 TC 注入 R。

此处“可接”不代表当前三份 RTL 已有这些能力。现 producer 单 child / 单 owner / 单返回路由，不能用连线直接得到两 flow 的独立 R 和暂停。当前 READ recipe 不授予 destination WRITE。原长 raw AW 已被 q adapter 私有接受后，其原 B 仍是未完成义务；有限共享通路释放不能冒称整个原 job 或 backend 已退休。

首版不能把 raw CONF129 等同于实际物理 AxCACHE=2：**direct 源 AR 有保留属性的 override；普通 TC AR 与物理 AW 标量打包仍丢 cache。** 下文给出准确位置。

## 1. 两个 source context 必须分别有真实 READ recipe

源：[hit_bypass_parent_translation.sv](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/hit_bypass_parent_translation.sv:71)。请求键是原 parent 的 DID、IOVA、PID、PID-valid、privilege、bank，访问类型明确是 READ：

```systemverilog
k.did = parent_did_i[f][n]; k.iova = parent_addr_i[f][n];
k.pid = parent_pid_i[f][n]; k.pid_valid = parent_pid_valid_i[f][n];
k.priv = parent_priv_i[f][n]; k.bank = parent_bank_i[f][n];
k.ttype = rv_iommu::UNTRANSLATED_R;
```

同文件 103–107 的 direct eligibility 是 still-live generation 的成功、可读、非 ignore、非 MRIF 结果，不能用 prepared bit 替代成功：

```systemverilog
cache_valid_o[f][n] = enable_i && !flush_i && parent_valid_i[f][n] &&
    valid_q[f][n] && seq_q[f][n]==parent_seq_i[f][n] &&
    !value_q[f][n].error && !value_q[f][n].ignore && !value_q[f][n].is_mrif &&
    (value_q[f][n].r || value_q[f][n].bypass);
cache_pa_o[f][n] = value_q[f][n].spaddr;
```

成功 fill 的请求键、row generation、live 条件还在同文件 218–234 核验。因此首版可用 `cache_valid + cache_pa + live parent identity/generation` 作为 **该 source parent** 的 READ recipe；A 不能继承 V 的授权。两流各自成功后，在同页范围内比较整个将共享区间的 PA 起点和长度，并要求兼容的 raw AXI 属性，才合并 physical R。首版正常 immutable DRAM 排除了 MMIO/read-side-effect 和重读值改变问题。

真实限定来自 [multi_actual_dma_top.sv](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/multi_actual_dma_top.sv:1017)：当前 helper 只接 len255 / size3 / INCR / 无 lock / parent 页内起点≤2048 / data 的 2KiB parent；direct 还排除 ATS。不要把这个接口宣传为通用短 parent、跨页、ATS 或 destination recipe。helper 的 `value.w` 来自 source READ 结果，既不是目标 IOVA 的翻译，也不授权目标 WRITE。

[multi_banked_top.sv:811](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/multi_banked_top.sv:811) 的 flush 只显式接 IOTINVAL / IODIRINVAL；本审核只接受静态配置。动态 DDTP/context 变更、invalidation 后已 offer R、暂停后 recipe 重用的有效性协议需要另行定义，不能免费继承该接口。

## 2. 现 physical direct 插口能发 PA R，但不含双 logical route

AxID 更正（本席后续直接核实例）：两 DMA raw AxID 均为0；输入 mux 加 flow bit 后原 source ID 为0/4。SameBank 的 bank 选择是bit3，bit2区分 flow；direct 标签才额外使用bit0。不能把某条下游观察 ID 当作第二 DMA 的 raw AxID=6。

[multi_actual_dma_top.sv:572](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/multi_actual_dma_top.sv:572) 用低 ID bit 识别 direct R：

```systemverilog
direct_ar_valid=pre_ar_valid && pre_direct;
pre_raw_r_valid=pre_direct ? (comp_r_valid && comp_r_id[0]) : tr_r_valid;
tc_comp_r_valid=comp_r_valid && !comp_r_id[0];
comp_r_ready=comp_r_id[0] ? (pre_direct && pre_r_ready) : tc_comp_r_ready;
comp_ar_id=direct_ar_valid ? (pre_ar.id | 4'd1) : tc_comp_ar_id;
```

595–600 的 assertion 明确要求 direct/TC source AR 不同时出现、direct R 必须有 `i_pre.busy_q` 的唯一 captured child route。973–980 仍把 producer 的一个 `s_r` 接全局 mux 的一个 ready；928–943 也只有一条 `pre_r` 返回。因而一个单独放在该插口后的合并器无法替第二个 parent 生成原始 R：它既没有第二个有效 parent route，也不能把不存在的 TC source 请求伪装成已接受。

[compact_pretl_owner_fragment.sv:95](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/compact_pretl_owner_fragment.sv:95) 的发读选择只支持当前 virtual/physical W owner 和 seq 相同的一个 owner，计算该 owner 已 physical-accepted W prefix；adaptive=0 时 cap128 beat。283–287 latch 唯一 `child_owner_q`；299–313 每个物理 R handshake 只增加这个 owner 的 `returned_q`，parent RLAST 才增加 `done_q`：

```systemverilog
s = int'(child_owner_q); n = p_index(done_q[s]);
returned_q[s][n] <= returned_q[s][n]+1;
if (s_r_o.last) done_q[s] <= done_q[s]+1;
if (m_r_i.last) begin busy_q <= 0; child_direct_q<=0; end
```

因此 root 的新 controller 必须替换该 single-child 路由部分，分别保存 V/A 的 raw header、generation、实际返 R cursor 和 original-parent RLAST。共享 physical R 的 RLAST 不能提前退休暂停 A 的 logical parent；A 的未返回 suffix 仍是 live parent，resume 才能依其仍有效的 recipe 从自己的 cursor 补读。现 helper 的 TC reuse 只匹配 base 或 base+1024（122–131），任意 q-offset 补读应使用仍有效 direct recipe + offset；不能声称现 reuse hit 规则已覆盖所有 resume 点。

## 3. q-W 必须在全局 first offer 前，原 B 必须合并

[vendor axi_mux.sv:301](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:301) 首次 AW VALID 就 push W owner，先于 downstream AW accept：

```systemverilog
if (!w_fifo_full && aw_valid) begin
  mst_aw_valid = 1'b1;
  w_fifo_push = 1'b1;
  if (mst_aw_ready) aw_ready = 1'b1;
  else lock_aw_valid_d = 1'b1;
end
```

所以 q-W adapter 的位置是每流 raw output 到 mux input 之间。它先私有保存原 AW，再向 mux offer q-child AW，按 q-child 重写 downstream WLAST，收齐所有 child B 后只返原 AW 的一个 B，并合并错误。它给 iDMA 的原 W handshake / 原 WLAST / 原 B 语义必须维持原长 parent；在 physical AW 端才缩短不能撤销已占全局 W owner 的长原 AW。

现 vanilla [axi_burst_splitter.sv:187](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_burst_splitter.sv:187) 已具体实现只在最后子 B 返回原 B、累计错误与 BVALID hold；其 336/338 的断言只允许 single-beat output，不能直接称为所需 q=2 splitter。可以把上述语义当实现验收基准。

首版 SameBank 避免了异 bank AW 等待前 bank WLAST 的附加锁。Root 的 virtual first-offer coverage 合同不要求 A physical qAW 已到才给 A raw R：A 可把最多 q beats 停在自己 buffer；待 V 当前 q 的 W 前缀排空后，A 的 q W 可进入 TC。这个合同可以替换旧 producer 的 physical-W coverage，但必须有实际 `B_private >= q`、held R / AW 稳定、不重复送同一 flow 的 beat，以及每流最多一笔未退休 q group。quiet 要等待属于 A 的已 offer q 的 W 与 child B/TC spill 义务全部排空；原 AW/原 B 和 raw AR suffix 可私有保留。暂停并不允许撤销已首 offer 的 qAW、已首 VALID 的该 flow R。

## 4. 迟原 B 不阻首个 raw AR；多 parent 不能据此类推

真实 iDMA wrapper [dma_core_wrap.sv:306](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:306) 配 RAWCouplingAvail=0、NO_ERROR_HANDLING；top365/424 两实例 NumAxInFlight=8。backend [idma_backend.sv:610](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:610) 有独立 AR fall-through，839–858 有独立 AW fall-through；554–569 是 8-slot read datapath metadata FIFO。

[idma_axi_transport_layer.sv:201](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:201) ARVALID / ARREADY 直接来自 read metadata 和 raw AR response。451–455 的 BVALID / BREADY 只接 write datapath response。backend524–532 的正常 copy job response 是 `w_dp_rsp_valid & w_last_burst`，不把迟 B 接到当前 ARVALID。因此 q adapter 能先接受原 AW，随后原 B 延到所有 q 完成；当前 2KiB 单 parent 的 raw AR 可提交，不需要先有原 B。

仍有准确的准入边界：[idma_legalizer.sv:451](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:451) 在 RW-coupled 模式按 r_ready / w_ready 同时前进；backend472–473 的 w_ready 包含 AW metadata 与 write-last queue readiness。首次空 AR/AW fall-through 可一起收第一个 pair，q adapter 应提供原 AW 的捕获槽并持续握手。若把“第一次原 AW ready”也推迟到自己的 qB，可能制造不必要的前置循环。更长 job、第二 parent、满 write-last FIFO / AW 槽时，后续 AR 确可被耦合阻塞；本审核不作无界 AR ahead-of-W 保证。原 job done / backend_idle 仍等待最后合并 B。

## 5. AxCACHE 的实际丢失与 direct 例外

- [multi_banked_top.sv:435](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/multi_banked_top.sv:435) `pack_tr_req` 从 `tr_req='0` 开始，AW435–450 / AR452–465 打包 id、addr、len、size、burst、prot、user、context，但没有 cache；所以经此标量链进入普通 TC 的 AR/AW cache 为 0。
- [multi_system_top.sv:523](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/multi_system_top.sv:523) `comp_pack` 又从 `comp_req='0` 开始，物理 AW 的 526–535 也没有 cache。
- **例外**：同文件543–556 `comp_ar_in_pack` 在 `i_dma.direct_ar_valid` 时完整取 `pre_ar`，含 `u_comp_ar_chan.cache = i_dma.pre_ar.cache`。因此 direct 源 AR 保留 raw CONF129 的 cache2；不能称所有物理 AR 均为 0。
- TC 本体 [trans_ctl_write_limit.sv:337](/home/xvhaocheng/work/iommu-platform/out/natural-write-order-20261008/trans_ctl_write_limit.sv:337) 会从 raw header 保留 cache；丢失点在 wrapper 标量接口，不是 TC 权限逻辑。

首版若只用 raw cache2 授权 raw 层 burst 变形、物理端主动降成不可缓存属性，应写明这个事实，不能把它等同于完整透传。若候选/对照合同要求 physical AXI 属性保持原请求，则必须给上述普通与物理 AW 路径显式透传，并在真实握手端观测 cache=2；两种控制同样修正。cache0010 是 modifiable 属性，不能当作新增 cache/posted-write 收益。

## 6. 必须保留的未验证项

首版单页 destination WRITE 仍逐 q 经过真实 TC 授权，不使用 source READ recipe 绕过。SameBank / q≤私有 buffer / virtual-offer coverage 没有发现必然的两 owner 锁环，但尚未有新 RTL / memory backpressure / delayed B 的运行证据。关键待验收是：两 flow raw R READY 分开、held beat mask 不变、独立 ID/RLAST/cursor；全局 mux 前短 q 的真实 first offer；每 q W / B 退休后才允许该流下一组；原 B 迟到且不提前 job done；实际 cache 属性。

若扩到跨页或 cache 受干扰，共享长 R 可能停在下一 q-band 前，而其 destination WRITE miss 的 PTE 返回与 held data R 争用物理返回资源。旧 producer 通过只读 physical-accepted W prefix 避免了这项新的未来依赖。首版固定单页、正常许可成功只能缩小它，不能证明通用持续进度：必须核/定义目标页稳定可命中、独立 WRITE recipe 或 PTW 在 held data R 下仍可服务。不能把“内存公平”自动当成这个通路互锁已解除。

继续实现前必须直接读的关键源已在本审查读过：三个指定 out 模块；q-admission 所用 vendor axi_mux first offer；iDMA backend / legalizer / transport 的初次准入、buffer、原 R/B；multi_banked / multi_system 的 cache 打包；真实 TC normal/error 输出与内部 spill 的退休边界。这里没有宣称通用 fault / ignore / MRIF / ATS / invalidation、异 bank、多个 job 或整个系统 QoS 保证。
