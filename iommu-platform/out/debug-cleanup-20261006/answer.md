# IOMMU_HANG_DIAG 完全移除：依赖证据 + 最小补丁方案

> 说明：以下为**代码级核查结论与待审查补丁方案**，并非已执行操作。我未运行 rg、未编译、未删除任何文件；所有「需 root rg 确认」项已显式标出。数据类（`out/composition-hangdiag-20261005/...` 等）本次**不动**，待用户答复。

---

## 1. 分类结论（按依赖，不按名称）

### 类别 A —— 旧卡死 USER4 快照 / hangdiag 临时诊断（DS/PTW 解决后废弃）→ 可完全移除

判定依据：该功能只由 `IOMMU_HANG_DIAG` 宏开启，数据出口仅一个 JTAG USER4 采样端口，不进入任何功能路径；`docs/composition-trim-20261005.md` 已记录 composition 裁剪后不需要它，且 composition 用 `research_data` 路径（`ariane_peripherals_xilinx.sv:1018-1021`）而非 `hang_diag_o`。

| 文件 | 行 | 证据 |
|---|---|---|
| `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv` | 41-43 | 端口声明 `` `ifdef IOMMU_HANG_DIAG `` / `output logic [127:0] hang_diag_o,` / `` `endif `` |
| 同上 | 952-960 | `gen_dma` 内 `` `ifdef `` / 953-959 对 `hang_diag_o[127:0]` 的 4 条 `assign` / `` `endif `` |
| 同上 | 1331-1333 | `gen_dma_disabled` 内 `` `ifdef `` / `assign hang_diag_o = '0;` / `` `endif `` |
| `cva6/corev_apu/fpga/src/ariane_xilinx.sv` | 810-916 | 整块 `` `ifdef IOMMU_HANG_DIAG ``：811 `hang_peripherals`；812 `hang_sample`；813-814 计数字号；815-836 `always_ff`；837-839 采样装配；842-845 `` `HANG_AXI_RECORD ``；846-853 6 条 AXI/`dram` 记录；854 `` `undef ``；855-857 `` `HANG_AMO/`HANG_LRSC/`HANG_WMUX ``；858-906 `always_comb`（引用 `i_axi_riscv_atomics.*`、`i_axi_xbar.*` 层次路径）；907-910 装配；911-913 `` `undef ``；914-915 `hang_snapshot #(.FrameVersion(8'd2)) i_hang_diag (...)` |
| 同上 | 946-948 | `i_ariane_peripherals` 例化端口 `` `ifdef `` / `.hang_diag_o ( hang_peripherals ),` / `` `endif `` |
| `cva6/corev_apu/fpga/src/hang_snapshot.sv` | 72-93 | 模块 `hang_snapshot` 整体在 `` `ifdef IOMMU_HANG_DIAG `` 内（`BSCANE2`+`BUFG`+`hang_snapshot_cdc`） |
| 同上 | 6-70 | `hang_snapshot_cdc` **不在** ifdef 内，属无条件定义；但其唯一消费者是 `hang_snapshot`（72-93）与 `tests/hang_snapshot_cdc_tb.sv:16` |
| `cva6/corev_apu/fpga/constraints/hang_snapshot.xdc` | 1-9 | 整文件；4-9 引用层次路径 `i_hang_diag/i_bscan/TCK`、`i_hang_diag/i_cdc/...`（只对 `hang_snapshot` 例化存在时有意义） |
| `cva6/corev_apu/fpga/scripts/run.tcl` | 45-49 | 注释 + `` if {[info exists ::env(IOMMU_HANG_DIAG)] ...} ``：`verilog_define IOMMU_HANG_DIAG` + `add_files ... constraints/hang_snapshot.xdc` |
| `scripts/read_hang_snapshot.py` | 1-489 | 整个 XVC/USER4 读取器（`USER4_IR = 0x23`@41、`MAGIC`@47、`FRAME_BITS=1088`@43） |
| `scripts/decode_hang_snapshot.py` | 1-1061 | 离线解码器，注释 30-40 直接引用 `hang_snapshot.sv:53-62`、`ariane_xilinx.sv:837-910`、`ariane_peripherals_xilinx.sv:952-959` |
| `tests/test_hang_snapshot_v2.py` | 1-337 | 直接 `import` reader/decoder（27-28、44-45），并依赖真实数据 `out/composition-hangdiag-20261005/n4096-hang-snapshot.json`（29-30，**数据本体不删，待答复**） |
| `tests/hang_snapshot_cdc_tb.sv` | 1-80 | 直接例化 `hang_snapshot_cdc`（16） |

**关联但仍需 root rg 确认的文件（不在本次上下文内）：**
- `cva6/corev_apu/fpga/scripts/add_sources.tcl`（`run.tcl:43` source 它）——若含固定文件清单 `src/hang_snapshot.sv`，需同步删该行。
- 任何 Vivado 工程 / `*.prj` / `*.tcl` 中显式列出 `hang_snapshot.sv` 或 `hang_snapshot.xdc` 的位置。
- `scripts/platform.py` 全文件是否出现字面量 `IOMMU_HANG_DIAG`（本次所给内容 1-416 **未见**；`fpga()` 只对 `IOMMUDSFixedDelayInput/Output`、`IOMMULegacyE3Observer`、`IOMMUResearchRole`、`IOMMUNonblockMode` 做正则替换，270-278）。
- `scripts/platform.py:207-226` `stage_repo` 用 `git archive <commit>` 整仓导出，**无固定文件清单**，故 `hang_snapshot.sv/xdc` 是随 cva6 commit 一起被带入——移除必须落在 cva6 仓库提交本身，而非仅工作区。
- `scripts/platform.py:290-291` 对 `fpga_root.glob('xilinx/*/tcl/run.tcl')` 前置 prologue；该 glob 路径（`xilinx/*/tcl/`）与本次要改的 `scripts/run.tcl` **不同**，但需 rg 确认不存在第二份 `run.tcl` 含同类分支。
- docs/*.md、其他脚本对 `read_hang_snapshot` / `decode_hang_snapshot` / `hang_snapshot` 的引用。
- `hang_snapshot_cdc` 是否有第三处消费者（除 `hang_snapshot` 与 tb）。

### 类别 B —— 旧 research / nonblock 观察器（composition profile 已禁用；其他历史 profile 仍引用）→ **本次不删**

| 模块/位置 | 行 | 依赖说明 |
|---|---|---|
| `gen_legacy_research` / `dma_research_observer` | `ariane_peripherals_xilinx.sv:1069-1098`（例化 1070），守卫 `IOMMUNonblockMode != 3` | 被 `research_role!=0` 的历史 profile 引用；`research_reverse_o` 回灌 `dma_core_wrap_intf`（1299）。其输出经 `research_set_data` 复用器（1018-1021）与 `dma_observer ResearchEnable`（978-982, 162）相连 |
| `dma_research_cost_groups` | 同文件 1605-1828 | 被 `dma_research_observer` 内部例化（369-393） |
| `dma_research_e2_timing` | 402-430（例化） | 外部模块，仅 research 路径使用 |
| `gen_nonblock_observer` / `dma_nonblock_observer` | `ariane_peripherals_xilinx.sv:1024-1035`（例化 1025），守卫 `IOMMUNonblockMode != 0 && != 3` | 被 `n-serial`(1) / `n-hit`(2) profile 引用；经 `nonblock_data` 复用（1019, 1028, 1033-1034） |

结论：这些是**历史 profile 的功能性观测**，composition 只是在自己的分支里不例化它们（`docs/composition-trim-20261005.md:14`）。它们仍被 `scripts/platform.py:365` 所列 profile 引用，**不能凭「research/nonblock/debug」字样判定为废弃**。

### 类别 C —— 当前必需（composition bank / IOTLB 被动 tap / 有效 observer）→ **保留，禁改**

| 模块/信号 | 行 | 作用 |
|---|---|---|
| `dma_observer` | `ariane_peripherals_xilinx.sv:978-1010` | PathEnable/P4MEnable/ResearchEnable/E3Enable 复用一个平台观测器 |
| `dma_path_observer` | `dma_observer.sv:112-124` | path 统计（C1 诊断路径） |
| `dma_e3_observer` | `ariane_peripherals_xilinx.sv:1173-1190` | E3 观测 |
| `dma_controlled_launcher` | 1142-1165 | 受控启动 |
| `dma_composition_control` | 1036-1059 | composition 128 rawwords / 开-关快照 |
| IOTLB 被动 tap：`research` / `research_entries[_next]` / `plru` | 890, 951, 961-964, 1058, 1078, 1474 | C1 四例共享 PLRU 诊断直接依赖；由 `rv_iommu_top` 的 `.research_o(research)`（1474）产生 |

关键事实：`hang_diag_o`（953-959）只是**再次采样** `obs_*` / `e3_tl` / `nonblock_*` / `composition_*`。删除它**不会**动到底层信号或 composition 的 `research_data` 路径（1018-1021）。

---

## 2. 最小 unified diff（单块；大文件删除以路径清单表示，不展开）

```diff
--- a/cva6/corev_apu/fpga/src/hang_snapshot.sv
+++ /dev/null
@@ -1,94 +0,0 @@
-<删除整文件>
-  · 6-70   hang_snapshot_cdc（无条件定义；消费者仅为下方 hang_snapshot 与
-          tests/hang_snapshot_cdc_tb.sv:16）
-  · 72-93  `ifdef IOMMU_HANG_DIAG module hang_snapshot (BSCANE2 + BUFG + cdc)

--- a/cva6/corev_apu/fpga/constraints/hang_snapshot.xdc
+++ /dev/null
@@ -1,9 +0,0 @@
-<删除整文件；4-9 的 create_clock / set_false_path / set_max_delay 均指向
- i_hang_diag/i_bscan 与 i_hang_diag/i_cdc，仅在 hang_snapshot 例化存在时有效>

--- a/scripts/read_hang_snapshot.py
+++ /dev/null
@@ -1,489 +0,0 @@
-<删除整文件（XVC/USER4 读取器）>

--- a/scripts/decode_hang_snapshot.py
+++ /dev/null
@@ -1,1061 +0,0 @@
-<删除整文件（离线解码器）>

--- a/tests/test_hang_snapshot_v2.py
+++ /dev/null
@@ -1,337 +0,0 @@
-<删除整文件（reader/decoder 离线测试；仅删除测试代码，不删其引用的 out/ 数据）>

--- a/tests/hang_snapshot_cdc_tb.sv
+++ /dev/null
@@ -1,80 +0,0 @@
-<删除整文件（hang_snapshot_cdc 异步时钟契约测试台）>

--- a/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv
+++ b/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv
@@ -38,10 +38,7 @@ module ariane_peripherals #(
     input  logic       clk_i           , // Clock
     input  logic       clk_200MHz_i    ,
     input  logic       rst_ni          , // Asynchronous reset active low
-`ifdef IOMMU_HANG_DIAG
-    output logic [127:0] hang_diag_o   ,
-`endif
     AXI_BUS.Slave      plic            ,
     AXI_BUS.Slave      uart            ,
     AXI_BUS.Slave      spi             ,
@@ -949,14 +949,5 @@ module ariane_peripherals #(
         logic research_write, research_dma_quiet, research_arm, research_open, research_close;
         logic [127:0] research_entries [16], research_entries_next [16];
-`ifdef IOMMU_HANG_DIAG
-        assign hang_diag_o[31:0] = {e3_tl.did[3:0], nonblock_did[3:0],
-            e3_tl.ptw_active, e3_tl.error, nonblock_fire, nonblock_trans_accept,
-            composition_iommu_quiet, composition_clear, composition_mode, nonblock_diag};
-        assign hang_diag_o[63:32] = {obs_poll, obs_done, obs_issue, launch_active,
-            batch_gap_active, batch_active, job_quiet, fullbuffer_jobs};
-        assign hang_diag_o[95:64] = {obs_done_id[0][15:0], obs_issue_id[0][15:0]};
-        assign hang_diag_o[127:96] = {obs_done_id[1][15:0], obs_issue_id[1][15:0]};
-`endif
         for (genvar r = 0; r < 16; r++) begin : gen_research_entries
             assign research_entries[r] = research.entries[r];
             assign research_entries_next[r] = research.entries_next[r];
@@ -1328,9 +1328,6 @@ module ariane_peripherals #(
     else begin : gen_dma_disabled
 
         assign fullbuffer_jobs = '0;
-`ifdef IOMMU_HANG_DIAG
-        assign hang_diag_o = '0;
-`endif
 		// AXI Bus between System Interconnect (Mst) and iDMA Configuration Port (Slv)
 		ariane_axi_soc::req_slv_t axi_dma_cfg_req;
 		ariane_axi_soc::resp_slv_t axi_dma_cfg_resp;

--- a/cva6/corev_apu/fpga/src/ariane_xilinx.sv
+++ b/cva6/corev_apu/fpga/src/ariane_xilinx.sv
@@ -810,107 +810,0 @@ module ariane_xilinx (
-`ifdef IOMMU_HANG_DIAG
-<删除 811-915 全部 105 行：hang_peripherals/hang_sample 声明、815-836 计数器
- always_ff、837-853 采样装配、842-845 `HANG_AXI_RECORD、855-857 `HANG_AMO/
- `HANG_LRSC/`HANG_WMUX、858-906 always_comb（含 i_axi_riscv_atomics.* 与
- i_axi_xbar.* 层次引用）、907-910 装配、911-913 `undef、914-915 i_hang_diag
- 例化>
-`endif
 
 ariane_peripherals #(
@@ -943,9 +943,6 @@ module ariane_xilinx (
     `elsif VCU118
     .InclSPI      ( 1'b0         ),
     .InclEthernet ( 1'b0         )
     `endif
 ) i_ariane_peripherals (
-`ifdef IOMMU_HANG_DIAG
-    .hang_diag_o  ( hang_peripherals             ),
-`endif
     .clk_i        ( clk                          ),
     .clk_200MHz_i ( ddr_clock_out                ),
     .rst_ni       ( ndmreset_n                   ),

--- a/cva6/corev_apu/fpga/scripts/run.tcl
+++ b/cva6/corev_apu/fpga/scripts/run.tcl
@@ -43,9 +43,6 @@
 source scripts/add_sources.tcl
 
-# Explicit diagnostic build only; ordinary profiles contain no USER4 monitor.
-if {[info exists ::env(IOMMU_HANG_DIAG)] && $::env(IOMMU_HANG_DIAG) eq "1"} {
-    set_property verilog_define [concat [get_property verilog_define [current_fileset]] IOMMU_HANG_DIAG] [current_fileset]
-    add_files -fileset constrs_1 -norecurse constraints/hang_snapshot.xdc
-}
 
 set_property top ${project}_xilinx [current_fileset]
```

**可选清理（非必需）**：上述 `run.tcl` 删除后 44 与 50 行会留下两个连续空行；如需消除，可在同一 hunk 内再删一行空行（不改变行为）。

**同时需在 `add_sources.tcl`（或等价的固定文件清单）删除对应行**：若其中存在 `src/hang_snapshot.sv` 与 `constraints/hang_snapshot.xdc` 的显式条目。该文件未在上下文中提供，属**待 root rg 确认**。

---

## 3. 其他 observer 退出旧 profile —— 范围建议（**不自动改历史 profile 功能**）

仅作规划，需等「旧 profile 是否退役」定论后再动：

1. `dma_nonblock_observer`（`dma_nonblock_observer.sv`）与 `gen_nonblock_observer`（`ariane_peripherals_xilinx.sv:1024-1035`）：仅当 `n-serial`/`n-hit`（`platform.py:365`）同时退役，才可连同 `nonblock_data` 复用分支（1019, 1033-1034）一并删除。
2. `dma_research_observer`（含 `dma_research_cost_groups`、`dma_research_e2_timing` 例化）与 `gen_legacy_research`（1069-1098）：仅当所有 `research_role!=0` 的 profile 退役后，才可连同 `research_reverse`（1074, 1099, 1299）、`research_data` 复用（1018-1021）与 `dma_observer` 的 `ResearchEnable` 分支（981, 162, 450-455, 476, 491）一起撤除。
3. 上述任一删除都会触碰 `composition` 所依赖的共享 `dma_observer` 接口，属跨模块重构 —— 需要 Codex 主导，本轮不做。

---

## 4. 禁改清单（本轮不动）

- DDR 功能：`ariane_xilinx.sv:1044-1056`（`axi_riscv_atomics_wrap`）、`1116-1200`（`xlnx_axi_clock_converter_ddr`）、`1220-1288`/`1483-1544`（DDR 控制器）。
- XBAR：`ariane_xilinx.sv:291-320`。`dram` 网络（1042）在删除诊断块后仍被功能路径使用（1055/1119），不得误删。
- DS/PTW 功能：`ariane_peripherals_xilinx.sv:1363-1480`（`axi_delayer`、`i_riscv_iommu`、ds/comp/prog AXI 连接）。
- 时钟/时序约束主文件：`constraints/genesys-2.xdc`、`kc705.xdc`、`vc707.xdc`，以及 `run.tcl:19-27, 74` 的 board 约束加载。
- 有效 observer：`dma_observer`、`dma_path_observer`、`dma_e3_observer`、`dma_controlled_launcher`、`dma_composition_control`，以及 IOTLB `research`/`plru` tap。
- 数据/资产：`out/composition-hangdiag-20261005/`、formal 172/method88 原始数据、已接受的 BIT/来源快照、文献与模型审计 —— 本次不删，待用户答复旧产物范围。

---

## 5. 建议的 root rg 核查（我未执行）

```
rg -n "IOMMU_HANG_DIAG|hang_diag_o|hang_peripherals|hang_sample|hang_snapshot" --glob '!out/**'
rg -n "hang_snapshot" cva6/corev_apu/fpga/scripts cva6/corev_apu/fpga/xilinx
rg -n "read_hang_snapshot|decode_hang_snapshot|test_hang_snapshot_v2" .
rg -n "hang_snapshot\.sv|hang_snapshot\.xdc" cva6/corev_apu/fpga/scripts cva6/corev_apu/fpga/xilinx
# run.tcl 是否存在第二份：
fd -H 'run\.tcl' cva6/corev_apu/fpga
# 历史 profile 对类别 B 模块的引用：
rg -n "dma_research_observer|dma_nonblock_observer|research_role|nonblock_mode" configs scripts/platform.py
```

---

## 6. 验收检查（供 Codex 执行验证）

1. `rg "IOMMU_HANG_DIAG" .` → 除候选 docs 外 0 命中；`rg "hang_diag_o|hang_snapshot|HANG_AXI_RECORD|HANG_AMO|HANG_LRSC|HANG_WMUX" cva6/ scripts/ tests/` → 0 命中。
2. `ariane_peripherals` 端口列表中不再有 `hang_diag_o`，且 `ariane_xilinx` 例化不再连接该端口；两端一致，无悬空端口。
3. `run.tcl` 不再定义 `IOMMU_HANG_DIAG`、不再 `add_files` `hang_snapshot.xdc`。
4. 默认（非 `IOMMU_HANG_DIAG`）Vivado 综合仍通过；`migration-4dma-composition`(mode3)、`n-serial`(1)、`n-hit`(2)、`e3`、`research-*` profile 的 `platform.py fpga` 参数替换（270-278）与例化路径均不受影响。
5. `add_sources.tcl`（或其固定清单）中已无 `hang_snapshot.sv`/`hang_snapshot.xdc` 条目；文件删除发生在其**所属 cva6 提交**内（因 `stage_repo` 走 `git archive`）。
6. composition 观测路径未变：`research_data` 复用（1018-1021）、`dma_composition_control`（1036-1059）、`dma_observer`（978-1010）保持原样。

## 7. 未决 / 假设

- 假设 `hang_snapshot_cdc` 无第三处消费者（除被删的 `hang_snapshot` 与 tb）——**待 rg 确认**；若希望保留通用 CDC 助手，可仅删 `hang_snapshot` 模块（72-93），保留 6-70，此时 `tests/hang_snapshot_cdc_tb.sv` 可保留。此为设计选择点，需 Codex/用户裁定。
- 假设 `add_sources.tcl` 等未提供的构建清单结构如 `run.tcl:43` 所示；实际条目需 root rg。
- `docs/composition-trim-20261005.md` 等文档对 hangdiag 的叙述属历史记录，未列入删除清单。
