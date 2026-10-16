# Nonblock engineering pilot — frozen run plan (2026-10-04)

Status (2026-10-04): original-serial **15/15 completed**, no data errors, taint=0.
The two banked FPGA versions are still compiling; no banked hardware result or
novelty claim is made. The original boot required one documented startup-script
interrupt before any experiment began.

Runner: `scripts/mac_nonblock_pilot.py`.

---

## 1. Scope

A single, fully frozen engineering pilot that answers one question only:

> under a fresh boot of one BIT, what do the P4M hardware window cycle count and
> the number of A jobs completed inside that window look like across a fixed
> Solo / A1 / A8 / A16 footprint ladder?

Everything else (selection, tuning, calibration, statistics, novelty) is out of
scope and must not be produced from this data until the results exist.

Three labels are planned, one runner invocation each, one BIT flashed per
invocation because a fresh boot resets the policy:

| label             | role of the BIT                          | legacy cost run |
|-------------------|------------------------------------------|-----------------|
| `original-serial` | original serial reference                | no              |
| `banked-serial`   | banked serial reference                  | no              |
| `banked-hit`      | banked target                            | no              |

The original role-2 legacy cost run is **not** part of this pilot.

Firmware is already the SD kernel `00093`. The runner never writes the SD card
and never copies firmware.

---

## 2. Frozen case plan (15 cases)

Every case is one `idma_experiment --p4m` run with

* `--chunk 4096`
* `--victim-pages 1` (victim DMA instance 0, `/dev/idma0`)
* `--a-gap-cycles 0`
* `--timeout-ms 60000`
* aggressor `none` (Solo) or DMA instance `1` with 1/8/16 pages

### 2.1 Short group — `transfers=64`, `warmup=16`

| # | case id      | condition | aggressor       |
|---|--------------|-----------|-----------------|
| 1 | `short-solo` | Solo      | none            |
| 2 | `short-a1`   | A1        | id 1, pages 1   |
| 3 | `short-a8`   | A8        | id 1, pages 8   |

### 2.2 Long group — `transfers=4096`, `warmup=256`, three rounds

Condition order is fixed per round and is never re-selected:

| round | order                     |
|-------|---------------------------|
| 1     | Solo, A1, A8, A16 (base)  |
| 2     | A16, A8, A1, Solo (reverse) |
| 3     | Solo, A1, A8, A16 (base)  |

| # | case id         | round | condition | aggressor       |
|---|-----------------|-------|-----------|-----------------|
| 4 | `long-r1-solo`  | 1     | Solo      | none            |
| 5 | `long-r1-a1`    | 1     | A1        | id 1, pages 1   |
| 6 | `long-r1-a8`    | 1     | A8        | id 1, pages 8   |
| 7 | `long-r1-a16`   | 1     | A16       | id 1, pages 16  |
| 8 | `long-r2-a16`   | 2     | A16       | id 1, pages 16  |
| 9 | `long-r2-a8`    | 2     | A8        | id 1, pages 8   |
|10 | `long-r2-a1`    | 2     | A1        | id 1, pages 1   |
|11 | `long-r2-solo`  | 2     | Solo      | none            |
|12 | `long-r3-solo`  | 3     | Solo      | none            |
|13 | `long-r3-a1`    | 3     | A1        | id 1, pages 1   |
|14 | `long-r3-a8`    | 3     | A8        | id 1, pages 8   |
|15 | `long-r3-a16`   | 3     | A16       | id 1, pages 16  |

Total = 3 + 12 = **15 cases**, executed in the table order, once each.

Exact CLI form per case:


## 2026-10-04 N停滞后的恢复与排除记录

短A8没有退出后，保存原始目录并停止对应Mac runner，JTAG恢复已知可用原版BIT。实际UART确认kernel00093、boot956fa8d0-0bf3-4104-a8b1-2af736624a01、taint0，记录out/n-prototype-20261004/restored-boot.json；没有重跑任何样本。

Codex短仿真检查了“跨银行AW重排直接堵住W”的候选解释：当前axi_demux_simple在前一银行W未完成时阻止切换AW银行，两个模式的512写beat/两个B均完成，aw-order-repro.log保留。该候选不成立；不能据此排除读写数据依赖/共享DDR响应停滞。正在由DeepSeek起草有限读写缓冲＋共享返回路径测试和接收计数非零guard，Codex继续核心诊断。计数作用域接线修复尚未重编。

## 2026-10-04 v1当前进展

- 已修复计数generate作用域接线；Codex补读写依赖约束，S/N使用同一规则，旧控制器默认行为不变。
- DeepSeek起草读写依赖模型、脚本与诊断；Codex连接真实模块接口、审查/修改关键RTL并执行。共享返回模型3种发起顺序36场景、原控制器12对照、原有联合/控制器测试、计数非零guard全部通过。模型尚未包括实际DMA后端、DDR以及同一任务两段的全部重叠。
- 两个v1 BIT完整构建正在运行；旧v0失败构建保存在out/n-prototype-20261004/failed-v0-builds/，原逻辑路径映射见RELOCATION.json，不覆盖失败证据。
- FPGA已恢复原版Linux00093，boot956fa8d0-0bf3-4104-a8b1-2af736624a01，taint0。无新正式样本、无旧样本重跑，无SD更新。编译/必要最终时序完成后JTAG继续两版各15项；原版15项已完成不重跑。
- 根因边界、科学影响和原始路径见iommu-platform/docs/nonblock-dependency-diagnosis-20261004.md；不能称N有性能收益或新颖性。


### v2当前进展：整份4KiB缓冲、有限模型通过、BIT构建中

DMA0/1 BUFFER_DEPTH512，DMA2/3仍3；S/N相同，数据净增8144B、MetaFifoDepth11→520，实际综合资源待测。batch硬件要求4096B和源/目的页对齐、一次一个job等完成。本次新增depth512+overlap+rr模型banked12/12、legacy12/12通过；不是完整实际DMA/DDR验证。原depth3 rr banked8/12失败/legacy12通过，证据保留。两v2 BIT已开始完整构建，未上板；现有Linux00093/reader/ABI与SD沿用。原版15不重跑，S/N各15仍按冻结计划；原版→S差异含布局+缓冲，S→N同容量比较才评估绕过增量，不称创新或已取得性能收益。


<!-- APPEND to docs/nonblock-pilot-plan-20261004.md -->

## v4 pilot append — matched S/N v4, 15 cases each, `--diagnostics` (2026-10-04)

This section is appended only; it does not rewrite the frozen plan or historical results. No new execution is claimed here.

- Status: v4 code is implemented; two BIT builds are in progress; **not on board**. Existing runs are not rerun. No Linux/SD change.
- v3 technical status: 45 technical passes across original15 and v3 S/N15 each; all N fast = 0. This is **not a hit-rate** and **not evidence of no opportunities**. Do not turn it into a bypass-benefit or no-opportunity conclusion.
- New matched plan: run S and N v4 as a matched pair, **fifteen cases each**, in the 15-case default sequence, with `--diagnostics`. No existing reruns; no Linux/SD changes. Technical first-error stop applies. A `fast0` result is a scientific negative, **not a technical stop**.
- Analysis order: inspect actual candidate / reject / accept counts before cause or benefit. Do not speculate on unknown build resource/timing or new novelty.
- Actual runtime context: live DDR/DC for DID 10/11 is flat MSI mode1 (`out/n-prototype-20261004/v4-live-context-evidence.json:23-27`). The v3 MSI guard is mode0-only static exclusion; DDTP mode2 is supported (`rv_iommu_nonblock_hit.sv:91-93`). There is no reason to assume the U bit is unset by default; leaf acceptance uses the actual `content_1S.u` (`:94-95`).
- v4 MSI guard: input match uses `VLEN39`/`VPNW39`; cached S1 PPN uses `GPPNW39`; the leaf check uses captured `dc_q` (`rv_iommu_nonblock_hit.sv:62-79`). Fast path allows only mode0 or flat1 ordinary on both input and leaf; reads/writes in the MSI range and reserved modes fall back. Original request / canonical / Sv39 / Bare / 4 KiB / permission / stability guards remain (`:84-103`).
- Passive observer: new 16-word bank is observer-local indices16..31 (research D10..D1F, frontend raw F10..F1F); the old 0..15 (0x00..0x0F, F00..F0F) ABI and word15 error are unchanged (`dma_nonblock_observer.sv:196-232`). Seen bits: 0 base S1PTW incl implicit/debug; 1 full window; 2 candidate; 3 start; 4 DC; 5 DC good; 6 LEAF; 7 leaf good; 8 DC MSI reject; 9 leaf MSI reject; 10 leaf miss; 11 actual accept; 12 DC miss; 13 DC other; 14 leaf other; 15 enable (`rv_iommu_nonblock_hit.sv:115-131`, `dma_nonblock_observer.sv:223`).
- Diag counters: diag bank offsets 7..15 (absolute words 23..31) count events bits 3..11 (`dma_nonblock_observer.sv:164-175`, `:223-232`). Counts are frozen over `(open, close]`; the diag bank reuses the same core seq/cycles; cross-window partial probes are legal, so do **not** assert full lifecycle conservation. `diag fast` must equal the core fast sum (validator `:29`).
- Existing evidence: TL exit0; RR 4 KiB proxy 12/12 exit0; MSI actual pass7 / reject10 / accept8 / responses8 incl BP / touches8 (`out/n-prototype-20261004/v4-msi-regression.log:1`); observer actual Mode1/2 exit0 (`out/n-prototype-20261004/v4-observer-regression-fixed.log:16,27`); validator 21 exit0 (`out/n-prototype-20261004/v4-diag-validator-regression.log:42-44`); static C crosscompile `-static -Wall -Wextra -Werror` exit0.
- Verilator 4.038 compatibility uses a temporary source that removes **only** the input default already explicitly wired in the top; production is unchanged (`out/n-prototype-20261004/v4-observer-regression-fixed.log:1-5`).
- Capacity/layout: S/N share the same 4 KiB buffer for DMA0/1; old DMA2/3 are unchanged; local RAM. Original 3 vs S 512 mixes structural and buffer variation and cannot be used to isolate bypass increment. v3 archive mapping is `v3-accepted-builds/RELOCATION.json`.
- Boundary: finite unit/proxy only; not full DMA integration; not a universal deadlock proof. Attribution: DeepSeek proposal; Codex critical RTL, apply, and actual test.


### 2026-10-04 v5 后续入口

v4两版30项通过但N候选/探测/快速接收0。v5资格读入口实现已通过有限主机验证，两版FPGA构建中；最新规格、适用范围与冻结板测计划见 [nonblock-v5-qualified-read-plan-20261004.md](nonblock-v5-qualified-read-plan-20261004.md)。实际状态以 `out/n-prototype-20261004/v5-build-state.json` 和工作区CURRENT_STATUS.md为准。
