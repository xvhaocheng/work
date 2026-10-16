仅依据所给源码做定向接口/寄存器证据整理；未运行命令、未写文件，不声称任何测试通过。

## 导航摘要

1. 当前 TL wrapper 内实际 cache：DDTC、PDTC、IOTLB、MRIFC；实际 walker：DDTW、PDTW、主 PTW、MSI PTW。cache 的 tags/content/PLRU 与 walker 的 `state_q`/PTE 进度寄存器是不同模块、不同 `always_ff`，没有同一物理 slot 的证据。
2. IOTLB tag 不含 DID。命中键是 VPN+PSCID/GSCID+stage+valid；DID 只出现在 DDTC/PDTC tag 和 IOTLB 的 policy fill/owner 选择里。fastprobe 强制 `en_1S=1, en_2S=0, gscid=0`，所以只可能命中 stage1-only 条目。
3. 生产 `rv_iommu_ptw.sv` 是单 FSM、单未完成 walk；接口和状态里没有 PWC/coalescing/multiple-miss 队列。`out/dual-write-prefix-20261008/multi_actual_dma_top.sv` 里有一个 test-only `pretl_owner_fragment`/`reuse_cache_*` 直通路径，但它不在 `rv_iommu_tl_wrap.sv` 的生产实例里。
4. 两个现有 fixture 的 V/A roots 分离、PSCID 不同、IOVA 不同；本轮不能把跨 DID 共享命中归到这些旧实验。合法同 context 共享只能作为构造假设列出，不能称跑过。
5. 当前接口没有 permission generation/epoch/version 字段；新增 cache 的权限世代比较所需接口在给定源码中不存在。

---

## 1. 当前实例的 cache 与 PTW；active 状态是否同一存储

### 实例清单（`rv_iommu_tl_wrap.sv`）

- DDTC：`rv_iommu_ddtc i_rv_iommu_ddtc`，`rv_iommu_tl_wrap.sv:502-520`。
  - 实际 tag：`device_id + valid`，`rv_iommu_ddtc.sv:48-51`。
  - 实际内容：`dc_t [NumDdtcEntries-1:0] content_q/content_n`，`rv_iommu_ddtc.sv:54`。
  - 实际状态：`tags_q/content_q/plru_tree_q`，`rv_iommu_ddtc.sv:171-182`。
- DDTW：`i_rv_iommu_ddtw`，`rv_iommu_tl_wrap.sv:525-560`。
- PDTC：仅 `InclPC`，`i_rv_iommu_pdtc`，`rv_iommu_tl_wrap.sv:584-599`。
  - tag：`device_id + process_id + valid`，`rv_iommu_pdtc.sv:43-47`。
  - content：`pc_t [NumPdtcEntries-1:0]`，`rv_iommu_pdtc.sv:50`。
  - 状态：`tags_q/content_q/plru_tree_q`，`rv_iommu_pdtc.sv:173-184`。
- PDTW：`i_rv_iommu_pdtw`，`rv_iommu_tl_wrap.sv:604-640`。
- IOTLB：`i_rv_iommu_iotlb`，`rv_iommu_tl_wrap.sv:657-685`。
  - tag：`vpn2/vpn1/vpn0 + pscid + gscid + en_1S + en_2S + valid`，`rv_iommu_iotlb.sv:58-68`。
  - content：`iotlb_content_t [NumIotlbEntries-1:0]`，`rv_iommu_iotlb.sv:71`。
  - 状态：`tags_q/content_q/plru_tree_q`，`rv_iommu_iotlb.sv:464-473`。
- 主 PTW：`i_rv_iommu_ptw`，`rv_iommu_tl_wrap.sv:759-804`。
  - `active_o = (state_q != IDLE)`，`rv_iommu_ptw.sv:100`。
  - 实际 walk 状态：`state_q/ptw_stage_q/main_lvl_q/s1_lvl_q/iova_q/gpaddr_q/gpa_x_q/ptw_pptr_q/leaf_1Spte_q/...`，`rv_iommu_ptw.sv:661-696`。
- MSI PTW / MRIFC：`i_rv_iommu_msiptw`，`rv_iommu_tl_wrap.sv:846-878`；`i_rv_iommu_mrifc`，`rv_iommu_tl_wrap.sv:899-917`，仅 MRIF 配置。

### 是否同一物理存储/slot

不是同一物理存储或 slot，给定源码中的证据：

- IOTLB 的 tag/content/PLRU 只在 `rv_iommu_iotlb.sv:464-473` 的 `always_ff` 更新。
- 主 PTW 的 walk 进度只在 `rv_iommu_ptw.sv:661-696` 的 `always_ff` 更新。
- PTW 对 IOTLB 的唯一“写”路径是 `update_o` 最终条目，`rv_iommu_ptw.sv:190-248`，并在 tl_wrap 中接入 `iotlb_update`，`rv_iommu_tl_wrap.sv:268-280`。
- PTW 的 active 只作为 `active_o` 输出，`rv_iommu_ptw.sv:100`；在 tl_wrap 中用于 `n_window`、`research_o`、`e3_o`、`policy_quiet_o` 等观察/门控，`rv_iommu_tl_wrap.sv:295,303-308,725-740,771`，没有被写入 IOTLB/DDTC/PDTC 的 tag/content。
- IOTLB 的 lookup 端口不接收 PTW active：`rv_iommu_iotlb` 端口没有 `active_i`，只有 `lookup_i/update_i/inval_i`，`rv_iommu_iotlb.sv:32-56`。
- DDTC/PDTC 也没有 PTW active 输入；DDTC 端口见 `rv_iommu_ddtc.sv:30-45`，PDTC 端口见 `rv_iommu_pdtc.sv:25-40`。

### “结构没有接口” vs “自然发生争用”

- 结构没有接口：IOTLB/DDTC/PDTC 的 tag/content 与 PTW 的 `state_q/ptw_pptr_q` 无直接连接；cache 不知道 PTW active 的内部进度，PTW 也不读 cache 的 tag/content 内部数组。
- 自然发生争用：在 tl_wrap 层只有逻辑串行/端口级竞争，不是同一存储竞争：
  - 主 FSM 一次处理一个请求状态：`IDLE/DDT/PDT/PT/MSI/...`，`rv_iommu_tl_wrap.sv:143-151,1003-1573`。
  - `ptw_init = (state_q == PT) & ~iotlb_lu_hit`，`rv_iommu_tl_wrap.sv:375`。
  - 请求仲裁器在 normal/debug/ATS 之间，`rv_iommu_tl_wrap.sv:181-214`。
  - DDTW/PDTW/PTW/MSI PTW 在 tl_wrap 中各自有 AXI 端口，`rv_iommu_tl_wrap.sv:108-118`，连接见 `547,622,751/787,876`。若外部 xbar 合并这些端口，争用发生在外部互连，不在本文件证明的 cache/PTW slot 上。
  - test wrapper 也把 ddtw/ptw 作为分离端口再导出，`tests/nonblock_actual_dma_top.sv:886-912`；`out/dual-write-prefix-20261008/multi_actual_dma_top.sv:1142-1168` 同样。

---

## 2. IOTLB/fastprobe 跨 DID 共享条件

### IOTLB tag 与 hit 条件

Tag 字段，`rv_iommu_iotlb.sv:58-68`：

```systemverilog
logic [10:0] vpn2;
logic [8:0]  vpn1;
logic [8:0]  vpn0;
pscid_t pscid;
gscid_t gscid;
logic en_1S;
logic en_2S;
logic valid;
```

没有 DID 字段。

Lookup/hit，`rv_iommu_iotlb.sv:101-139`：

- `match_pscid[i] = (((lu_pscid_i == tags_q[i].pscid) || content_q[i].content_1S.g) && en_1S_i) || !en_1S_i;`，`rv_iommu_iotlb.sv:106`。
- `match_gscid[i] = (lu_gscid_i == tags_q[i].gscid && en_2S_i) || !en_2S_i;`，`rv_iommu_iotlb.sv:109`。
- `match_stage[i] = (tags_q[i].en_2S == en_2S_i) && (tags_q[i].en_1S == en_1S_i);`，`rv_iommu_iotlb.sv:126`。
- 完整命中：`valid && match_pscid && match_gscid && match_stage && vpn2 == tags_q[i].vpn2`，然后 1G 或 `vpn1` 匹配且 2M 或 `vpn0` 匹配，`rv_iommu_iotlb.sv:129-136`。
- `is_1G`/`is_2M` 使用 `rv_iommu_pkg.sv:618-651` 的函数。

GV/PSCV/SCID 在 hit 条件中的位置：

- `gv/pscv` 不是 IOTLB lookup 输入；它们只在 invalidation 输入 `iotlb_inval_t` 中，`rv_iommu_pkg.sv:342-351`，并在 invalidation 逻辑中使用：`rv_iommu_iotlb.sv:218,294`。
- hit 中出现的 SCID 只有 PSCID/GSCID；GV 不参与命中。

### fastprobe 跨 DID 共享的精确条件

fastprobe 通过 `n_iotlb_lu` 走非阻塞 hit 路径，`rv_iommu_tl_wrap.sv:271-277`：

```systemverilog
assign iotlb_lu         = (state_q == PT) | (state_q == MSI) | n_iotlb_lu;
assign iotlb_lu_vpn     = n_iotlb_lu ? n_iotlb_vpn : req_data.iova[...];
assign iotlb_lu_pscid   = n_iotlb_lu ? n_iotlb_pscid : pscid_q;
assign iotlb_lu_gscid   = n_iotlb_lu ? '0 : gscid_q;
assign cache_iotlb_en1  = n_iotlb_lu ? 1'b1 : en_1S_q;
assign cache_iotlb_en2  = n_iotlb_lu ? 1'b0 : en_2S_q;
assign iotlb_lu_hit     = cache_iotlb_hit && !n_iotlb_lu;
```

因此 fastprobe 在 IOTLB 侧要命中：

- 被命中条目的 `en_1S=1, en_2S=0`，因为 `match_stage` 是精确相等，`rv_iommu_iotlb.sv:126`。
- `lu_gscid_i=0`，且 `en_2S_i=0`，所以 GSCID 条件被 `!en_2S_i` 短路，`rv_iommu_iotlb.sv:109`。
- `lu_pscid_i` 必须等于条目的 `pscid`，或条目 `content_1S.g=1`，`rv_iommu_iotlb.sv:106`。
- VPN 匹配同上。
- DID 不在命中条件里。所以从 IOTLB 侧看，跨 DID 共享命中在结构上可能，只要 fastprobe 提供给 IOTLB 的 PSCID/VPN/stage 与 resident 条目匹配。
- 但 fastprobe 的 `n_iotlb_pscid` 由未提供的 `rv_iommu_nonblock_hit` 产生，`rv_iommu_tl_wrap.sv:325-327`；给定文件不能证明它如何从 candidate DID 的 DDTC/PC 推导 PSCID。这是关键未知。

注意：`iotlb_lu_hit` 在 `n_iotlb_lu` 时被强制为 0，`rv_iommu_tl_wrap.sv:277`，所以 fastprobe 的命中由 `i_hit` 直接消费 `cache_iotlb_hit`，不是主 PTW 路径的 `iotlb_lu_hit`，见实例连接 `rv_iommu_tl_wrap.sv:327`。

### DID 是 key 还是 only fill policy

- IOTLB：DID 不是 tag/key；tag 无 DID，`rv_iommu_iotlb.sv:58-68`。
- DDTC：DID 是 key，`tags_q[i].device_id == lu_did_i`，`rv_iommu_ddtc.sv:75`。
- PDTC：DID+PID 是 key，`rv_iommu_pdtc.sv:71`。
- IOTLB 的 policy owner：tl_wrap 里 `policy_mask_i` 用 `req_data.did == policy_did_i` 选择 fill mask，`rv_iommu_tl_wrap.sv:669-673`；注释明确“Owner is the stable main PTW/MSI request, never a fast probe DID.”，`rv_iommu_tl_wrap.sv:669`。
- 所以 DID 对 IOTLB 是 DDTC 查找键/填充策略 owner，不是 IOTLB 命中键。

### 同 context mapping/invalidation 必要前提

从 tag/hit 和 invalidation 源码可推出的必要条件：

- 同 stage tuple：`en_1S/en_2S` 精确相同，`rv_iommu_iotlb.sv:126`。
- 同 VPN：`vpn2` 必须相同，再按 1G/2M/4K 匹配 `vpn1/vpn0`，`rv_iommu_iotlb.sv:129-132`。
- 同 PSCID，或 S1 `content_1S.g=1` 允许 PSCID 不同，`rv_iommu_iotlb.sv:106`。
- 若 `en_2S=1`，必须同 GSCID，`rv_iommu_iotlb.sv:109`。
- fastprobe 只走 stage1-only：`en_1S=1, en_2S=0`，`rv_iommu_tl_wrap.sv:275-276`。

Invalidation 必要前提：

- IOTINVAL.VMA 由 `{gv,av,pscv}` 选择条目，`rv_iommu_iotlb.sv:218-277`。关键点：
  - `3'b001`：2S disabled、1S enabled、PSCID 匹配，排除 global，`rv_iommu_iotlb.sv:225-229`。
  - `3'b010`：2S disabled、1S enabled、IOVA 匹配，包含 global，`rv_iommu_iotlb.sv:231-237`。
  - `3'b011`：PSCID+IOVA，排除 global，`rv_iommu_iotlb.sv:238-244`。
  - `3'b100/101/110/111`：涉及 GSCID、PSCID、IOVA，`rv_iommu_iotlb.sv:246-276`。
- IOTINVAL.GVMA 由 `{gv,av}` 选择，`rv_iommu_iotlb.sv:294-314`。
- 因为 IOTLB 条目无 DID，失效 PSCID/GSCID/VPN 会作用于所有共享该键的 DID。若两个 DID 想共享，失效域不能比共享键更细；否则无法只失效一个 DID 而不影响另一个，或会漏失效。
- 对 S1 global 条目，`pscv=1` 的 VMA 失效排除 global，`rv_iommu_iotlb.sv:225-229,238-244`；若依赖 global 共享，必须用覆盖 global 的失效形式。

### policy fillmask/lookup/PLRU historical isolation 是否允许共享命中

- `policy_mask_i` 只进入 replacement/allocation：`allocation_mask = PolicyEnable ? policy_mask_i : '1`，`rv_iommu_iotlb.sv:366`；`replace_en[i] = en && allocation_mask[i]`，`rv_iommu_iotlb.sv:422`。
- `policy_clear_i` 清 valid 和 PLRU，`rv_iommu_iotlb.sv:215-216,424`。
- `partition_local_active` 只影响 PLRU touch 更新，`rv_iommu_iotlb.sv:359-362,384-403`。
- Lookup 段 `rv_iommu_iotlb.sv:101-139` 不读 `PolicyEnable/policy_mask_i/partition_local_active`。
- 结论：fillmask/PLRU historical isolation 不拒绝共享命中；任何 resident 条目，只要 tag/hit 条件满足，都可被命中，无论其 fill owner。Owner 只影响填充到哪些 way、PLRU 历史如何隔离，不改变 hit key。
- owner 用途在 tl_wrap 是 fill mask 选择，`rv_iommu_tl_wrap.sv:669-673`；测试中的 fill checker 也检查 DID10 fill 在 protected ways、DID11 fill 不在 protected ways，`tests/composition_tl_test.cpp:375-391`。这检查的是 fill owner，不是 hit owner。

---

## 3. PTE 访问是否有 PWC/PTW coalescing/crossflowprefixcache；单 walker 是否能 accept 多个 miss

### 生产 `rv_iommu_ptw.sv`

给定源码中，PTE 访问是一个单 FSM：

- 状态只有 `IDLE/MEM_ACCESS/PROC_PTE/ERROR`，`rv_iommu_ptw.sv:91-97`。
- `active_o = (state_q != IDLE)`，`rv_iommu_ptw.sv:100`。
- `init_ptw_i` 是边沿触发控制：`edge_trigger_n` 防止 `init_ptw_i` 持续时重复初始化，`rv_iommu_ptw.sv:150-158`；IDLE 仅在 `init_ptw_i && !edge_trigger_q` 时启动，`rv_iommu_ptw.sv:325`。
- MEM_ACCESS 只发一个 AR：`mem_req_o.ar_valid = 1'b1;`，`rv_iommu_ptw.sv:385-390`。
- PROC_PTE 只处理一个 `mem_resp_i.r_valid`，`rv_iommu_ptw.sv:395-397`。
- 没有 miss 队列、PWC 数组、coalescing 表、cross-flow prefix cache 结构；内部只保存单个 walk 的 `ptw_pptr_q/leaf_1Spte_q/gpa_x_q` 等，`rv_iommu_ptw.sv:127-135,661-696`。
- `update_o` 是单个 `iotlb_up_t`，`rv_iommu_ptw.sv:190-248`，只输出最终一个 VPN/content。
- 所以在这个文件内，没有“同时 accept 多个 miss”的接口或存储。`ptw_pptr_q` 能跨 MEM_ACCESS/PROC_PTE 保持一个未完成 walk 的进度，`rv_iommu_ptw.sv:135,302,687`，但不是多个 miss 的 progress 表。

### tl_wrap 层

- 主 PTW 启动：`ptw_init = (state_q == PT) & ~iotlb_lu_hit`，`rv_iommu_tl_wrap.sv:375`。主 FSM 一次只在一个 state。
- fastprobe 可在主 PTW active 时接受一个 bank-different 的 fast hit：`n_window = state_q == PT && e3_ptw_active && en_1S_q && !en_2S_q && ... && trans_req_data_i.bank != req_data.bank && ...`，`rv_iommu_tl_wrap.sv:303-306`。
- 这不是第二个 PTW miss/coalescing；它是 fast hit 路径，由 `rv_iommu_nonblock_hit` 产生 `fast_resp_*`，`rv_iommu_tl_wrap.sv:309-330`，再经 response arbiter，`rv_iommu_tl_wrap.sv:346-353`。
- 测试 `fast_prove_under_cold_a` 证明的是持有 cold A walk 时 V 的 fast hit，不是 V 发起第二个 walk，`tests/composition_tl_test.cpp:442-462`。
- MSI PTW 是另一个独立 walker，`rv_iommu_tl_wrap.sv:846-878`；但未提供其源码，不能对其内部 coalescing 下定论。
- DDTW/PDTW 也未提供源码；它们是 DDT/PDT walker，不是 PTE walker。

### out/dual-write-prefix test wrapper

`out/dual-write-prefix-20261008/multi_actual_dma_top.sv` 中有一个 test-only `pretl_owner_fragment` 与 `reuse_cache_*` 直通路径：

- `reuse_cache_valid/reuse_cache_failed/reuse_cache_pa` 声明，`out/dual-write-prefix-20261008/multi_actual_dma_top.sv:563-564`。
- `pretl_owner_fragment i_pre` 使用 `cache_valid_i(reuse_cache_valid)`、`cache_eligible_i(direct_eligible)`、`cache_pa_i(reuse_cache_pa)`，`out/dual-write-prefix-20261008/multi_actual_dma_top.sv:973-998`。
- `pre_direct` 选择 `comp_ar` direct 或正常 `tr_ar`，`out/dual-write-prefix-20261008/multi_actual_dma_top.sv:572-587,903-924`。
- 这个结构不在 `rv_iommu_tl_wrap.sv` 的生产 cache/PTW 实例内；它是测试 wrapper 层。不能把它当作生产 PTE PWC/coalescing 证据。

---

## 4. 两个现有 A/V 映射 fixture

### Fixture A：`tests/composition_tl_test.cpp`

来源：`tests/composition_tl_test.cpp:342-372`。

| 项 | DID10 / V | DID11 / A |
|---|---|---|
| DDT 条目 | 1-level base 0x10000，DID10 -> 0x10280，注释 `tests/composition_tl_test.cpp:343-348` | DID11 -> 0x102C0，`tests/composition_tl_test.cpp:350-353` |
| PSCID | 10（ta 写入 0xA000，注释 pscid=10），`tests/composition_tl_test.cpp:347` | 11（ta 0xB000），`tests/composition_tl_test.cpp:352` |
| GSCID | 0，因为 iohgatp=Bare，`tests/composition_tl_test.cpp:346` | 0，`tests/composition_tl_test.cpp:351` |
| S-root | fsc Sv39, ppn=0x20 -> phys 0x20000，`tests/composition_tl_test.cpp:348,355` | fsc Sv39, ppn=0x30 -> phys 0x30000，`tests/composition_tl_test.cpp:353,363` |
| G-root | 无 / Bare，`tests/composition_tl_test.cpp:346,351` | 无 / Bare，同上 |
| translation mode | Sv39 stage1 only | Sv39 stage1 only |
| IOVA -> PA | V0 0x10000000 -> 0x80000000；V1 0x10001000 -> 0x80001000；Vcold 0x10002000 -> 0x80002000，`tests/composition_tl_test.cpp:358-361` | A page n: 0x20000000+n*0x1000 -> 0x90000000+n*0x1000，n=0..15，`tests/composition_tl_test.cpp:366-371` |
| Page table phys | V: root 0x20000, L1V 0x40000, L0V 0x50000，`tests/composition_tl_test.cpp:355-362` | A: root 0x30000, L1A 0x60000, L0A 0x70000，`tests/composition_tl_test.cpp:363-371` |
| 关键 PTE 地址 | root PTE 0x20000 -> L1V；L1 PTE 0x40400 -> L0V；leaf V0 0x50000, V1 0x50008, Vro 0x50010, Vcold 0x50018，`tests/composition_tl_test.cpp:356-361` | root PTE 0x30000 -> L1A；L1 PTE 0x60800 -> L0A；leaf A n at 0x70000+n*8，`tests/composition_tl_test.cpp:364-371` |

### Fixture B：`tests/nonblock_actual_dma_test.cpp` / `tests/nonblock_actual_dma_top.sv`

来源：`tests/nonblock_actual_dma_test.cpp:453-503`，DID 常量 `tests/nonblock_actual_dma_test.cpp:77-78`，DMA 参数 `tests/nonblock_actual_dma_top.sv:372-373,431-432`。

| 项 | DID10 / V / DMA0 | DID11 / A / DMA1 |
|---|---|---|
| DDT 条目 | Extended 1-level base 0x10000，DID10 -> 0x10280，`tests/nonblock_actual_dma_test.cpp:457-463` | DID11 -> 0x102C0，`tests/nonblock_actual_dma_test.cpp:465-470` |
| PSCID | 10（0x10290 = 0xA000），`tests/nonblock_actual_dma_test.cpp:460` | 11（0x102D0 = 0xB000），`tests/nonblock_actual_dma_test.cpp:467` |
| GSCID | 0，iohgatp 为 Bare（0x10288=0），`tests/nonblock_actual_dma_test.cpp:459` | 0，`tests/nonblock_actual_dma_test.cpp:466` |
| S-root | Sv39, ppn=0x20 -> phys 0x20000，`tests/nonblock_actual_dma_test.cpp:461,472` | Sv39, ppn=0x30 -> phys 0x30000，`tests/nonblock_actual_dma_test.cpp:468,480` |
| G-root | 无 / Bare | 无 / Bare |
| translation mode | Sv39 stage1 only | Sv39 stage1 only |
| IOVA -> PA | V_SRC 0x10000000 -> 0x80000000；V_DST 0x10001000 -> 0x80001000；V_COLD_DST 0x10002000 -> 0x80002000，`tests/nonblock_actual_dma_test.cpp:80-85` | A_IOVA_BASE 0x20000000；A_SRC_PA_BASE 0x90000000；A_DST_PA_BASE 0x90100000，`tests/nonblock_actual_dma_test.cpp:87-89` |
| Page table phys | V: root 0x20000, L1V 0x40000, L0V 0x50000，`tests/nonblock_actual_dma_test.cpp:472-477` | A: root 0x30000, L1A 0x60000, L0A 0x70000，`tests/nonblock_actual_dma_test.cpp:479-487` |
| 关键 PTE 地址 | root 0x20000 -> 0x40000；L1 0x40400 -> 0x50000；leaf Vsrc 0x50000, Vdst 0x50008, Vcold 0x50010，`tests/nonblock_actual_dma_test.cpp:473-477` | root 0x30000 -> 0x60000；L1 0x60800 -> 0x70000；source page n leaf 0x70000+n*8；dest page n leaf 0x70000+(a_pages+n)*8，`tests/nonblock_actual_dma_test.cpp:480-487` |
| A 环 IOVA | — | source: base+p*0x1000；dest: base+(a_pages+p)*0x1000，`tests/nonblock_actual_dma_test.cpp:1786-1789`，默认 a_pages=8，`tests/nonblock_actual_dma_test.cpp:2401` |

### roots 分离与共享 claim 的边界

两个 fixture 都满足：

- V root = 0x20000，A root = 0x30000，页表根分离。
- PSCID：V=10，A=11，不同。
- IOVA：V 从 0x10000000，A 从 0x20000000，不同 VPN。
- PTEs 使用 0xDF/0xD3 等，S1 G 位为 0；因此 IOTLB `match_pscid` 不能靠 `content_1S.g=1` 绕过 PSCID 不同，`rv_iommu_iotlb.sv:106`。
- stage 都是 Sv39 stage1 only，但 PSCID 不同足以使 IOTLB 命中失败。

因此本 turn 不能把任何跨 DID 共享命中 claim 归到这些旧实验；旧实验的 V/A 是分离 context、分离 root、分离 IOVA。它们能证明的是 nonblock fast path 对同一 V 上下文的命中，例如 `tests/composition_tl_test.cpp:442-462`，不是 A/V 共享。

### 若用 same context 构造合法共享，必要 mapping/key 假设（不称跑过）

从 IOTLB 源码可列出的必要假设：

- 让两个 DID/设备得到相同 PSCID（或使用 S1 `G=1` 让 PSCID 不参与匹配，`rv_iommu_iotlb.sv:106`）。
- 若 `en_2S=1`，必须相同 GSCID，`rv_iommu_iotlb.sv:109`。
- 必须相同 stage tuple：`en_1S/en_2S` 精确相等，`rv_iommu_iotlb.sv:126`。
- 必须相同 VPN 或 superpage 覆盖：`vpn2` 匹配，再按 `is_1G/is_2M` 匹配 `vpn1/vpn0`，`rv_iommu_iotlb.sv:129-132`。
- fastprobe 场景还必须让 resident 条目为 stage1-only：`en_1S=1, en_2S=0`，`rv_iommu_tl_wrap.sv:275-276`。
- 软件语义上，相同 PSCID/GSCID 必须代表相同地址空间；若 roots 不同但 PSCID 相同，IOTLB 仍会按 tag 命中，正确性依赖 software 保证同 PSCID 同 mapping。
- 失效前提：必须用覆盖共享键的 IOTINVAL.VMA/GVMA；对 S1 global 条目，注意 `pscv=1` 排除 global，`rv_iommu_iotlb.sv:225-229,238-244`。
- 以上只是构造假设，本轮未运行，不能称已构造或已通过。

---

## 5. 新增 cache 的 key/权限世代比较：接口证据与未知

给定源码没有任何 permission generation/epoch/version 接口证据：

- IOTLB tag 无 generation：`vpn2/vpn1/vpn0/pscid/gscid/en_1S/en_2S/valid`，`rv_iommu_iotlb.sv:58-68`。
- IOTLB update 无 generation：`update, vpn, pscid, gscid, content`，`rv_iommu_pkg.sv:333-339`。
- IOTLB invalidation 无 generation：`inval_vma, inval_gvma, av, gv, pscv, vpn, gscid, pscid`，`rv_iommu_pkg.sv:342-351`。
- IOTLB 更新只判断 `update_i.update && replace_en[i]`，不比较权限世代；`rv_iommu_iotlb.sv:318`。失效只清 `valid`，`rv_iommu_iotlb.sv:215-217,221-275`。
- DDTC tag 只有 DID+valid，`rv_iommu_ddtc.sv:48-51`；content 是 `dc_t`，`rv_iommu_ddtc.sv:54`；无 generation。
- PDTC tag 只有 DID+PID+valid，`rv_iommu_pdtc.sv:43-47`；content 是 `pc_t`，`rv_iommu_pdtc.sv:50`；无 generation。
- Device/Process Context 字段：
  - `dc_ta_t` 只有 `pscid`，`rv_iommu_pkg.sv:147-151`。
  - `pc_ta_t` 只有 `pscid/sum/ens/v`，`rv_iommu_pkg.sv:154-161`。
  - `tc_t` 无 permission generation，`rv_iommu_pkg.sv:171-187`。
- PTE 字段：`v/r/w/x/u/g/a/d/ppn/rsw/reserved`，无 version，`rv_iommu_pkg.sv:102-114`。
- PTW 输出 `update_o` 只有最终 VPN/content/pscid/gscid，`rv_iommu_ptw.sv:190-248`，无 generation。

因此：若新增 cache 需要“permission generation 比较”，当前接口没有可比较的世代字段；给定源码只提供 scid/vpn/stage 键和 invalidation 清 valid。未知包括：外部软件是否维护 generation、是否需要新接口、现有 invalidation 顺序是否足以替代世代比较。这里不设计机制，只报告接口缺口。

---

## 不确定处

- `rv_iommu_nonblock_hit.sv` 未提供：fastprobe 如何得到 `n_iotlb_pscid/n_iotlb_vpn`、如何使用 candidate DID/DDTC、以及 fast hit 的完整 gating 不能从给定文件完全证明。
- `rv_iommu_msiptw.sv`、`rv_iommu_mrifc.sv` 未提供：不能对其内部是否有 coalescing/PWC 下定论。
- `rv_iommu_ddtw.sv`、`rv_iommu_pdtw.sv` 未提供：不能对其内部 walker 并发/coalescing 下定论。
- `out/dual-write-prefix-20261008/multi_actual_dma_top.sv` 中的 `pretl_owner_fragment`/`reuse_cache_*` 是 test wrapper 证据，不是生产 `rv_iommu_tl_wrap.sv` 内的 IOTLB/PTW 结构。
- 两个 fixture 的 DDT 字段解码基于测试注释和写入值；未运行读取 DC 字段每一步，但注释与结构一致。

## 最多 3 个后续具体文件建议

1. `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv`：确认 fastprobe 的 PSCID/GSCID/VPN/DID 来源和 hit gating。
2. `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_msiptw.sv`：确认 MSI walker 是否单走向、是否有 coalescing/PWC。
3. `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ddtw.sv`：确认 DDT walker 的并发/挂起接口，区分 DDT/PDT walker 与 PTE walker。
