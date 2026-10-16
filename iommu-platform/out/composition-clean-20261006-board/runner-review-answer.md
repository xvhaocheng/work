# 有界执行证据核查（compose V9 BIT + cpn3-clean，冻结 172）

结论先行：**对 observer V9（`IDMA_OBS_MAGIC_V9`）而言，`scripts/mac_composition.py` 与 `scripts/composition_plan.py` 都不需要修改**，因为两者只校验 composition 自己的 bank magic（`COMPOSITION_MAGIC`）与 tool ABI，从不读取/校验 observer 接口 magic，也不依赖被裁掉的 path-timer 字段。下面给可导航的 file:line 证据。

---

## 1) 两个脚本是否需要因 observer V9 改动

### 1.1 两个 magic 确实不同（用户提示已确认）

- composition bank magic（脚本唯一校验的那个）：
  - `scripts/composition_plan.py:62` `COMPOSITION_MAGIC = 0x49444D4143504E01`
  - 校验点：`scripts/composition_plan.py:368-369` → `raw[RAW_MAGIC] != COMPOSITION_MAGIC`（`RAW_MAGIC = 0`，见 `:72`）
- observer 接口 magic（V9，新增）：在补丁与内核驱动里，与上面完全无关：
  - `out/debug-cleanup-20261006/production-cleanup.patch:393-395`（`TrimmedComposition ? 64'h49444d414f425309 : …`）
  - `out/debug-cleanup-20261006/production-cleanup.patch:406`、`:417-418`、`:419-420`、`:429-430`（驱动侧新增/接受 V9）

两者字面值不同（`0x49444D4143504E01` vs `0x49444d414f425309`），且**脚本侧从未引用 observer magic**。

### 1.2 脚本对 observer 的依赖面

- `scripts/mac_composition.py` 对板子的校验只在 `collect_identity()`：
  - `scripts/mac_composition.py:387-404` 只读 `--abi` 并检查 `schema=='idma-composition-tool-v1'`、`record_schema=='idma-composition-v1'`、`composition_abi==1`（`:399-404`）。
  - 全部 CLI/内核对齐点：`:363-385`（`uname -r`、`boot_id`、`/proc/sys/kernel/tainted`）。
  - dmesg 仅原样落盘，从不解析：`:407-416`。
- `scripts/composition_plan.py` 的记录校验只碰 `result.raw` / `result.p4m.window` / `result.p4m.observation` / `result.experiment`，见 `:260-323`；对 `p4m.observer_status` 只要求等于 `P4M_OBSERVER_STATUS_OK=4`（`:65`、`:316-317`），**不涉及 observer magic**。
- 被 V9 裁掉的正是 "old path timers"，脚本已把它们标注为不参与科学判定：
  - `scripts/mac_composition.py:586` `'old HPM fields are raw_only, not a validated banked-mode metric'`
  - `composition_plan.py:478` `'hpm': 'raw_only, not a validated metric'`

### 1.3 小幅观察（非阻塞）

- `scripts/composition_plan.py:64` `P4M_BANK_MAGIC = 0x49444D4150344D01` 已定义，但在本文件校验逻辑中**未被引用**（validator 只用 `COMPOSITION_MAGIC`）。不影响结论。
- 补丁把 `PathEnable` 改为 `InclIOMMU && (IOMMUNonblockMode != 3 || IOMMULegacyE3Observer)`（`production-cleanup.patch:162-163`），但保持 `P4MEnable(InclIOMMU)`、`ResearchEnable(InclIOMMU)`（`:166`）——即 P4M window/observation 生成路径保留，脚本期望的 `P4M_WINDOW_WORDS=16`、`P4M_OBS_WORDS=35`（`composition_plan.py:66-67`）不受影响。

**结论：不需要改 `mac_composition.py` / `composition_plan.py`。** 这是提案性判断，见下文“假设/未决”。

---

## 2) through-phase explore 参数、失败停止行为、完整结果验收字段/文件

### 2.1 through-phase 参数

- 相序常量：`scripts/mac_composition.py:45` `PHASES = ('short','noise','explore')`
- 开关：`scripts/mac_composition.py:694` `--through-phase`，choices 同上，**默认 `'short'`**
- 选择逻辑：`scripts/mac_composition.py:645` `selected = self.plan.cases_through_phase(plan, self.through)`；实现 `composition_plan.py:181-186`（`PHASES[:index+1]` 集合过滤）。

→ **跑满 172 必须显式传 `--through-phase explore`**（传 'short' 只跑 16，传 'noise' 只跑 28）。

### 2.2 explore 阶段冻结参数（不改）

- `composition_plan.py:43-45` `EXPLORE_TRANSFERS=4096`、`EXPLORE_WARMUP=256`、`EXPLORE_BLOCKS=3`
- `composition_plan.py:34-35` `A_PAGE_CHOICES=(0,1,8,16)`、`V_GAP_CHOICES=(0,1000,4000)`
- `composition_plan.py:28` `MODE_ORDER=(S,N,PN…)` 实为 `(0,1,2,3)`；旋转派序 `:156-165`
- 固定量：`PROTECTED_DID=10`（`:52`）、`DEFAULT_TIMEOUT_MS=15000`（`:49`）、`CHUNK_BYTES=4096`（`:55`）、`PLATFORM_HZ=50000000`（`:56`）、`CYCLE_NS=20`（`:57`）
- 用例计数（`composition_plan.py:166-178`）：short 16 + noise 12 + explore 144 = **172**。
- 每 case 的 argv 形状：`composition_plan.py:189-198`（`--device/--mode/--a-pages/--v-gap/--transfers/--warmup/--timeout-ms`）。

### 2.3 失败停止行为（无重试、无恢复、无重跑）

- 单 case 出现任一 problem 即抛错：`mac_composition.py:577-578` `raise CompositionError(...)`。
- 顶层捕获 → 写 `STOPPED.txt`、`dmesg.failure.txt`、`summary.json` status=`stopped`，然后 re-raise：
  - `mac_composition.py:663-667`
  - `_stop()` 内容：`:324-335`（`stopped_at / reason / phase / case`）
  - `KeyboardInterrupt` 分支：`:658-662`（status=`interrupted`）
- 清单显式关闭自动重跑/恢复：`mac_composition.py:623-624`（`auto_rerun: False`、`resume: False`）、文档串 `:13-14`。
- 输出目录不允许复用/覆盖：`:736-737`（`--out` 已存在即 `SystemExit`）；每 case 目录 `exist_ok=False`：`:436`。
- 每 case 完成后立即刷新 summary：`:575` `self.write_summary('running')`。
- 技术失败即停，但“科学负（fast==0/miss==0/P-PN 无收益）”不算 technical 失败：`composition_plan.py:11-13, 422`。

### 2.4 完整结果验收字段/文件

**权威汇总 `summary.json`（schema `composition-summary-v1`）**，写于 `mac_composition.py:591-603`：
- `schema`、`run_id`、`status`、`through_phase`、`updated_at`
- `planned`、`attempted`、`completed`、`failed`
- `cases[]`：每项 `id, phase, mode, a_pages, v_gap_cycles, status, exit, taint_after, dir, v_window_cycles, a_completed_jobs_window, problems`（`:561-574`）
- `notes[]`（`:584-590`）

**`manifest.json`（`composition-manifest-v1`）**：`mac_composition.py:619-637`（`mode='attach-only'`、`through_phase`、`expected_kernel`、`board_cli`、`device`、`plan_generation`、`planned`、`phases`、transport 字段 `:123-137`）。

**`plan.json`（`idma-composition-plan-v1`）**：`composition_plan.py:169-178`。

**`boot.json`（attach 身份）**：由 `collect_identity()` 产出，字段 `collected_at, uname_r, boot_id, taint, tool_abi_exit, tool_abi_raw, tool_abi`（`mac_composition.py:361-405`），写入 `:641`。

**每 case 目录 `cases/<id>/`**：
- `record.raw.json`（CLI stdout 原始字节）：`:477-478`
- `record.json`：`:491-492`
- `metrics.json`：`:524-528`
- `stderr.txt`：`:479-481`
- `meta.json`（schema `composition-case-meta-v1`）：字段见 `:532-559`（含 `argv, command, framed_exit, exit, taint_before/after, boot_id, boot_after, uname_r, board_cli, board_cli_abi, problems`）

**运行级其它文件**：`run.log`（`:312-319`）、`console.raw.log`/`console.events.log`（`:760-761, 769-770`）、`dmesg.before.txt`（`:644`）、`dmesg.<phase>.end.txt`（`:653`）、失败时 `dmesg.failure.txt`（`:660, 665`）+ `STOPPED.txt`。

**通过判据（代码可判定）**：`summary.status=='complete'` 且 `planned==attempted==completed==172 && failed==0`，且各 case `meta.problems==[]` / `status=='completed'`（`:567, 577-578`）。内核侧：`uname_r` 必须等于 `--expected-kernel`，`taint` 必须 `'0'`，`boot_id` 必须为 UUID 且全程不变（`:367-369, 384-385, 505-506`）。板 CLI：`--abi` 三字段对齐（`:399-404`）。

---

## 3) 本轮应回传的既有原始文件 + 完成后最小事实记录

### 3.1 回传顺序（按信息量，均为运行目录已生成的原始文件）

1. `summary.json`（唯一权威汇总）
2. `manifest.json`
3. `plan.json`（证明仍是冻结 172）
4. `boot.json`（身份/ABI 证据）
5. `cases/<id>/meta.json`（逐 case）
6. `cases/<id>/record.json`
7. `cases/<id>/metrics.json`
8. `cases/<id>/record.raw.json`
9. `cases/<id>/stderr.txt`
10. `run.log`
11. `dmesg.before.txt` 与 `dmesg.<phase>.end.txt`
12. 若 STOPPED：`STOPPED.txt` + `dmesg.failure.txt`
13. （可选、大体积）`console.raw.log` / `console.events.log`

已在案、无需重复的对外证据文件：`out/composition-clean-20261006/acceptance.json`（build/boot 受理，kernel `acceptance.json:22`、boot_id `:23`）与 `out/debug-cleanup-20261006/production-cleanup.patch`。

### 3.2 完成后最小事实记录（建议固定字段）

- `run_id`（来自 `summary.run_id`）
- `through_phase`（应为 `explore`）
- `expected_kernel` 与实测 `uname_r`（应相等，`summary.json`/`boot.json`）
- `boot_id`（应等于本次 boot 的 ID）与 `taint`（须 `'0'`）
- `board_cli` + `tool_abi.{schema, record_schema, composition_abi}`
- 计数：`planned / attempted / completed / failed`（期望 172/172/172/0）
- `status`（`complete` / `stopped` / `interrupted`）
- 若停止：`STOPPED.txt` 的 `reason / phase / case`

---

## 假设 / 未决（需 Codex 验证）

1. **板端 `idma_composition` 是否接受 V9 observer**：脚本侧不需要改（已证），但采样数组 `p4m.window`/`p4m.observation` 由 P4M observer 产生。补丁保留了 `P4MEnable/ResearchEnable`（`production-cleanup.patch:166`），并让内核驱动接受 V9（`idma.c` 侧 `:419-420,:429-430`）；**但板端 CLI 二进制不在所给文件内，无法证明其接受 V9 magic 或不依赖被裁字段**。属未决依赖。
2. `scripts/mac_composition.py:16-18` 文档串称“short+noise 技术验收 gate explore”，但代码层面并未基于 short/noise 的科学结果提前终止 explore；explore 的唯一门是逐 case validator。结论按代码为准。
3. 所给 `mac_composition.py` 已支持 `--transport ssh`（`:738-762`），因此“沿用 composition-v1-20261004 脚本 + 显式 clean ssh helper”属 CLI 配置，不需改码：`--ssh-helper` 直接按路径加载（`:682-684, :743-746, :109-120`）。请核对实际所用 `mac_composition.py` 的 hash 与本内容一致。
4. 无法执行/运行，未做任何测试断言；上述均为代码/补丁阅读结论。

## 建议验收检查（供 Codex 落地）

- `summary.json`: `status=='complete' && planned==172 && attempted==172 && completed==172 && failed==0`。
- 任一 `cases/<id>/meta.json`: `problems==[]`、`taint_before=='0'`、`taint_after=='0'`、`boot_after==boot_id`、`uname_r=='6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty'`。
- `boot.json.tool_abi`: `schema=='idma-composition-tool-v1' && record_schema=='idma-composition-v1' && composition_abi==1`。
- `manifest.json`: `auto_rerun==False && resume==False && through_phase=='explore'`。
- 若本目录存在 `STOPPED.txt`：按 §2.3 立即停并保留已写出的 case 证据，不做重跑。