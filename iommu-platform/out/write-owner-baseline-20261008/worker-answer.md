# 1. 逐行核证：SourceReadGateEnable / FrontierEnable / FrontierScanEnable / HeaderParkMode

## 1.1 参数在哪里声明、被谁消费（真实接口）

| 参数 | 声明位置 | 唯一消费者 | 备注 |
|---|---|---|---|
| `SourceReadGateEnable` | `tests/nonblock_actual_dma_top.sv:54` | `nonblock_completion_pool_late`（`tests/nonblock_actual_dma_top.sv:629`） | **只**进 late pool |
| `FrontierEnable` | `tests/nonblock_actual_dma_top.sv:55` | `nonblock_completion_pool_late`（`:630`） | **只**进 late pool |
| `FrontierScanEnable` | `tests/nonblock_actual_dma_top.sv:56`（默认 `1`） | `nonblock_completion_pool_late`（`:631`） | **只**进 late pool |
| `HeaderParkMode` | `tests/nonblock_actual_dma_top.sv:57` | `nonblock_banked_top`（`:795`） | 生产 RTL **没有**此参数 |
| (`HeaderParkDepth`/`HeaderDirectEnable`) | `:58` / `:59` | `nonblock_banked_top`（`:795`/`:796`） | 同上 |

系统层副本（本轮只读）同样转发：`out/natural-write-order-20261008/system_top.sv:85-90`，实例化在 `:1130-1134`；`read_admission` 内部用 `HeaderParkMode` 决定 tag 队列索引模式（`system_top.sv:1287` → `tests/nonblock_read_admission.sv:23`）。

**关键事实（必须先说）**：`SourceReadGateEnable`/`FrontierEnable`/`FrontierScanEnable` 只出现在 `gen_late_pool` 分支里（`tests/nonblock_actual_dma_top.sv:623-676`），而该分支的选择条件是 `SharedPoolMode >= 4`（`:556` / `:578` / `:623`）。本轮基线命令 `out/natural-write-order-20261008/build-write-limit-command.json:14` 是 `-GSharedPoolMode=0`，走 `gen_no_pool`（`:556-577`）：**这三个参数在当前构建里完全没有消费者，恒等于无效**；`gen_no_pool` 直接把 `dbg_frontier_*`、`dbg_e2_*` 全部置 0（`:571-577`）。

## 1.2 与生产 `rv_iommu_banked_ctl` 及 `TEST_COMPLETION_GATE` 的关系

- 生产 `rv_iommu_banked_ctl`（`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:50-58`）的参数表里**只有** `RVIOMMUCfg / E3Enable / BankIdBit / 类型`。它没有 `CompletionGateEnable`、没有 `HeaderParkMode/Depth/DirectEnable`、没有 `SourceReadGateEnable/Frontier*`。这些**全部是 test-only**。
- `tests/nonblock_banked_top.sv:641-648`：`` `ifdef TEST_COMPLETION_GATE `` 时例化 `nonblock_completion_banked_ctl` 并把 `HeaderParkMode/HeaderParkDepth/HeaderDirectEnable` 传进去（`:642-645`）；否则例化生产 `rv_iommu_banked_ctl`（`:647`，且**不带任何 HeaderPark 端口**）。
- 反向保护：`tests/nonblock_banked_top.sv:635-637`——`` `ifndef TEST_COMPLETION_GATE `` 且 `CompletionGateEnable` 为真 ⇒ `$fatal`。也就是说 **`CompletionGateEnable` 必须依赖 `TEST_COMPLETION_GATE` 宏才允许打开**。
- `HeaderParkMode != 0` 的实现在 `tests/nonblock_completion_banked_ctl.sv:448-587`（test-only 文件），且 `:463-465` 自带 `$fatal`：必须 `CompletionGateEnable` 且 `HeaderParkMode ∈ {1,2}` 且 `HeaderParkDepth >= 1`。

结论：`HeaderParkMode` 要生效需要 **同时**满足 `TEST_COMPLETION_GATE` 宏 + `SharedPoolMode >= 4`（即 `CompletionGateEnable=1`，`tests/nonblock_actual_dma_top.sv:794`）。本轮命令两者都不满足 ⇒ **`HeaderParkMode` 在本轮基线上是惰性参数**。

另外，`tests/nonblock_actual_dma_top.sv:500-501` 有一条 `initial $fatal`：`SameBankDmasEnable && 0 < SharedPoolMode < 4` 直接终止。本轮 `SharedPoolMode=0`，不触发。

## 1.3 owner ID bit 与 physical/virtual W head 的真实使用

`pool` 的 owner/W-head 端口（`tests/nonblock_actual_dma_top.sv:606-615`，late 变体 `:647-658`）：

| 端口 | 实际驱动 | 位 |
|---|---|---|
| `aw_bank_i` | `i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2]` | **硬编码 `[2]`**（`:607`/`:648`） |
| `v_whead_valid_i` | `!i_dma_iommu_tr_mux.gen_mux.w_fifo_empty` | mux W-FIFO 空标志（`:609`/`:650`） |
| `v_whead_bank_i` | `i_dma_iommu_tr_mux.gen_mux.w_fifo_data[0]` | **硬编码 `[0]`**（`:610`/`:651`） |
| `p_aw_fire_i/p_aw_bank_i` | `comp_aw_valid && comp_aw_ready` / `comp_aw_id[2]` | **硬编码 `[2]`**（`:612`/`:653`） |
| `p_whead_valid_i` | `!i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_empty` | 生产 `axi_mux` 内部信号（`:613`/`:654`） |
| `p_whead_bank_i` | `physical_w_stream`（late）或 `...w_fifo_data[0]`（原始） | `:614`/`:655`，`physical_w_stream` 定义 `:482-499` |

要点：

1. **owner/stream 位是硬编码 `id[2]`，不是 `BankIdBit`。** `BankIdBit` 在别处：`rv_iommu_banked_ctl.sv:243-244`（`aw_sel/ar_sel = id[BankIdBit]`）、`tests/nonblock_banked_top.sv:797` 与 `system_top.sv:1282` 都是 `SameBankDmasEnable ? 3 : 2`。本实验用 `BankIdBit=3` 把两个真实 DMA 都放进 bank 0，而 `id[2]`（mux 端口位）此时充当 DMA 身份位。**“bank”在这个 pool 里实际是“stream/DMA 身份”，不是 DDR bank。**
2. `v_whead_*` = 译址**前**的 DMA 目的写（虚拟 W）；`p_whead_*` = 译址**后**的 completion 写（物理 W），来自生产 `rv_iommu_banked_ctl` 内部 `i_out_axi_mux`（`rv_iommu_banked_ctl.sv:418-449`）的**层次化内部信号**，不是生产端口。
3. **未决缺口**：`v_whead_bank_i` 取的是 `...w_fifo_data[0]`（bit 0），而同一文件的物理侧却写 `id[2]`。没有 `axi_mux` 源码（本包未提供），我**无法确认** `w_fifo_data[0]` 是否真的携带 stream 位；这需要 Codex 用 `axi_mux` 源或加一个只读 tap 单独核实。这是本轮最脆弱的观测量之一，必须标注而不当结论使用。

## 1.4 进入 AR 通道的位置

pool 位于**两个真实 DMA 的 translation-request（IOVA）侧**与 `axi_mux` 之间：

- slave 侧 AR：`tests/nonblock_actual_dma_top.sv:636`（`.s_req_i(pool_s_req)`，`:595`），`pool_s_req` 由 `dma0/1_mst_req` 组成（`:516-521`）。
- master 侧 AR：`.m_req_o(pool_m_req)`（`:596`），接到 `slv_reqs[0]/[1]`（`:544-546`），再进真实 4 入 `axi_mux`（`:680-705`），输出经 `unpack_mux_mst`（`:711-748`）成为 `tr_ar_*`，最终进入 `nonblock_banked_top`（`:817-829`）。

因此“准入/门控源读”的挂点在 **pool 的 AR slave→master 通路**（即 DMA 读请求进入 IOMMU 译址前）。**pool 内部的具体门控级行号无法给出**：`nonblock_completion_pool_late.sv` 本包未提供 —— 这是本报告最大的证据缺口。

## 1.5 它们能/不能保证“源 R 长度落在当前具体写段范围内”

**不能。** 明确断言如下：

- 这四个开关的全部可见判据只有**单一 ID 位的相等**（`id[2]`）加上 W-FIFO 的“非空/头”标志（`:607-614`）。**owner 相等不是字节/地址保证。**
- 反例就在本轮见证里：V 的源读 `0x80002800, 2048 B`（`docs/natural-write-order-boundary-20261008.md:49,55`），其中**只有前 1024 B** 对应当前 V 写段，后 1024 B 对应排在 A 之后的 V 写段（`...md:55`；`out/natural-write-order-20261008/codex-i-refinement.md:5`：“后一个 V physical AW 已经翻译准备，不能绕过 A”）。**owner 完全相同（V），却已越过当前可写段。**
- 代码本身也承认缺口：“仅检查‘目的译址 ready’会漏掉这个空洞”（`...md:55`）；“不能凭 PA 地址范围猜 owner/copy”（`codex-i-refinement.md:15`）。
- 另外，如上所述，`SourceReadGateEnable`/`Frontier*` 在 `SharedPoolMode=0` 下**即使置 1 也无任何效果**；`HeaderParkMode` 在无 `TEST_COMPLETION_GATE` 时同样无效。所以本轮**无法**用这些开关去“证明”任何 ordinary 强对照的负结果。

## 1.6 本轮最小同资源强普通对照的建议编译参数

直接复用 `out/natural-write-order-20261008/build-write-limit-command.json` 的参数集，**只改 out 目录与 driver**，不新增、不打开任何门控参数：

```
-GPolicyEnable=1 -GDmaBufferDepth=3 -GSharedPoolMode=0 -GPoolSlots=1
-GSameBankDmasEnable=1 -GInterleaveAwEnable=0
-GSourceReadGateEnable=0 -GFrontierEnable=0 -GHeaderParkMode=0
-GReverseReadOrderEnable=0
+define+ASSERTS_OFF          # 故意不加 +define+TEST_COMPLETION_GATE
```

注：基线里没有 `-GFrontierScanEnable`（其默认 `1` 惰性，因为 `FrontierEnable=0`）；也没有 `-GHeaderParkDepth/-GHeaderDirectEnable`（默认 `2`/`1`，同样惰性）。**不要**为了“补全”而添加它们。

---

# 2. 准备脚本 `prepare_owner_baseline.py`

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""prepare_owner_baseline.py

Prepare ONLY the minimal same-resource strong ORDINARY baseline command set for
the two-real-DMA / same-bank write-order study (research sec.31 follow-up).

This script PREPARES ARTIFACTS ONLY:
  * it does not build, does not run the DUT;
  * it never modifies production RTL, tests/, or any existing file;
  * it reads only from  <root>/out/natural-write-order-20261008
    and writes only under <root>/out/write-owner-baseline-20261008.

Hard invariants (a violation exits non-zero BEFORE any write):
  * every required source path must exist;
  * each substituted element must occur EXACTLY once in the reused source order;
  * TEST_COMPLETION_GATE must be absent, ReverseReadOrderEnable and
    InterleaveAwEnable must be 0 -> otherwise refuse (do not silently edit);
  * no inapplicable parameter (SourceReadGateEnable / FrontierEnable /
    FrontierScanEnable / HeaderParkMode / ...) is auto-enabled.

The script does NOT invent a new mechanism. The optional virtual-W-head gate /
HeaderPark candidates need TEST_COMPLETION_GATE and/or new RTL and are therefore
refused here so that Codex decides them explicitly.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
import sys
from pathlib import Path


# ---------------------------------------------------------------------------
# Layout constants (previous round -> this round)
# ---------------------------------------------------------------------------
OLD_REL = "out/natural-write-order-20261008"
NEW_REL = "out/write-owner-baseline-20261008"

BASE_CMD_NAME = "build-write-limit-command.json"
OLD_OBJ_NAME = "obj-write-limit"
NEW_OBJ_NAME = "obj-ordinary"
SYSTEM_TOP_NAME = "system_top.sv"
TC_NAME = "trans_ctl_write_limit.sv"
OLD_DRIVER_NAME = "natural_test.cpp"

CONF0_DRIVER = "natural_coupled_test.cpp"   # CONF=0, common read/write boundary
CONF1_DRIVER = "natural_queue_test.cpp"     # CONF=1, decouple_rw
CONF_DRIVER = {"conf0": CONF0_DRIVER, "conf1": CONF1_DRIVER}

FORBIDDEN_DEFINE = "TEST_COMPLETION_GATE"

REQUIRED_FLAGS = {
    "-GPolicyEnable=1":       "policy elaboration (production-like)",
    "-GDmaBufferDepth=3":     "small iDMA byte FIFO (same-resource baseline)",
    "-GSharedPoolMode=0":     "private-buffer; gate pool NOT instantiated",
    "-GPoolSlots=1":          "single slot",
    "-GSameBankDmasEnable=1": "put the two real DMAs on one bank (BankIdBit=3)",
    "-GInterleaveAwEnable=0": "forced AW interleave disabled",
    "-GSourceReadGateEnable=0": "source read gate disabled",
    "-GFrontierEnable=0":     "frontier disabled",
    "-GHeaderParkMode=0":     "header park disabled",
    "-GReverseReadOrderEnable=0": "reverse read order disabled",
}

TOP_MODULE = "nonblock_actual_dma_system_top"
TRACE_HEADER = "cycle,event,region,id,addr,len,extra"


# ---------------------------------------------------------------------------
# Small helpers
# ---------------------------------------------------------------------------
def die(msg: str) -> "None":
    print(f"prepare_owner_baseline: ERROR: {msg}", file=sys.stderr)
    sys.exit(1)


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as fh:
        for blk in iter(lambda: fh.read(1 << 20), b""):
            h.update(blk)
    return h.hexdigest()


def count_exact(argv, token: str) -> int:
    return sum(1 for a in argv if a == token)


def replace_exactly_once(argv, old: str, new: str, what: str):
    n = count_exact(argv, old)
    if n != 1:
        die(f"expected exactly 1 {what} element {old!r} in the reused command; found {n}")
    out = list(argv)
    idx = out.index(old)
    out[idx] = new
    return out, idx


def require_flag(argv, flag: str, why: str) -> None:
    if count_exact(argv, flag) != 1:
        die(f"required control flag {flag!r} ({why}) is missing or duplicated")


def require_absent_define(argv, name: str) -> None:
    for a in argv:
        if name in a:
            die(f"forbidden macro {name!r} present in reused command ({a!r}); "
                "this round must not use TEST_COMPLETION_GATE")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def main(argv=None) -> int:
    ap = argparse.ArgumentParser(prog="prepare_owner_baseline.py")
    ap.add_argument("project_root",
                    help="absolute path to the iommu-platform project root")
    ap.add_argument("--models", default="ordinary",
                    help="model names; only 'ordinary' is supported this round")
    ap.add_argument("--bytes", default="32768", choices=("2048", "32768"),
                    help="+natural-bytes per run; default 32768")
    ap.add_argument("--first-orders", default="v,a",
                    help="comma list of +natural-first values (v|a)")
    args = ap.parse_args(argv)

    # ---- model selection (deliberately restrictive) ----------------------
    models = [m.strip() for m in args.models.split(",") if m.strip()]
    if not models:
        die("--models must name at least one model")
    unsupported = [m for m in models if m != "ordinary"]
    if unsupported:
        die(
            "this round only prepares the ordinary same-resource baseline; "
            f"requested unsupported model(s): {unsupported}. The "
            "virtual-W-head gate (SourceReadGateEnable) and the HeaderPark "
            "candidate require SharedPoolMode>=4 and/or TEST_COMPLETION_GATE "
            "+ new/changed RTL, both of which this round forbids. "
            "Codex must decide and author them explicitly."
        )

    firsts = [f.strip() for f in args.first_orders.split(",") if f.strip()]
    for f in firsts:
        if f not in ("v", "a"):
            die(f"--first-orders entries must be v|a; got {f!r}")
    if not firsts:
        die("--first-orders must name at least one order")

    # ---- absolute root + path pre-checks (BEFORE any write) --------------
    root = Path(args.project_root)
    if not root.is_absolute():
        die("project_root must be an absolute path")
    if not root.is_dir():
        die(f"project_root missing or not a directory: {root}")

    old_out = root / OLD_REL
    new_out = root / NEW_REL

    base_cmd_path = old_out / BASE_CMD_NAME
    old_systop = old_out / SYSTEM_TOP_NAME
    old_tc = old_out / TC_NAME
    old_driver0 = old_out / CONF0_DRIVER
    old_driver1 = old_out / CONF1_DRIVER

    for p, what in (
        (base_cmd_path, "previous-round build command JSON"),
        (old_systop, "previous-round read-only system_top.sv"),
        (old_tc, "previous-round capacity-fixed TC copy"),
        (old_driver0, "previous-round CONF0 driver (coupled)"),
        (old_driver1, "previous-round CONF1 driver (queue)"),
    ):
        if not p.is_file():
            die(f"{what} not found: {p}")

    try:
        base_argv = json.loads(base_cmd_path.read_text(encoding="utf-8"))
    except Exception as exc:  # noqa: BLE001
        die(f"cannot parse {base_cmd_path}: {exc}")
    if not isinstance(base_argv, list) or not all(isinstance(x, str) for x in base_argv):
        die(f"{base_cmd_path} must contain a JSON array of strings")
    if not base_argv:
        die(f"{base_cmd_path} is empty")

    # ---- forbidden-flag pre-checks ---------------------------------------
    require_absent_define(base_argv, FORBIDDEN_DEFINE)
    for flag, why in REQUIRED_FLAGS.items():
        require_flag(base_argv, flag, why)

    # ---- exact, single-occurrence substitutions --------------------------
    old_obj_str = (old_out / OLD_OBJ_NAME).as_posix()
    old_systop_str = old_systop.as_posix()
    old_driver_str = (old_out / OLD_DRIVER_NAME).as_posix()
    old_tc_str = old_tc.as_posix()

    # The pinned capacity-fixed TC copy stays a FIXED reference into the
    # previous round; it is intentionally NOT copied and NOT rewritten.
    if count_exact(base_argv, old_tc_str) != 1:
        die(f"expected exactly 1 pinned TC source {old_tc_str!r}; "
            f"found {count_exact(base_argv, old_tc_str)}")

    new_obj_str = (new_out / NEW_OBJ_NAME).as_posix()
    new_systop_str = (new_out / SYSTEM_TOP_NAME).as_posix()

    argv, _ = replace_exactly_once(base_argv, old_obj_str, new_obj_str, "--Mdir")
    argv, _ = replace_exactly_once(argv, old_systop_str, new_systop_str, "system_top.sv")
    argv, _ = replace_exactly_once(argv, old_driver_str,
                                   (new_out / CONF0_DRIVER).as_posix(), "driver cpp")

    if "--exe" not in argv:
        die("reused command has no --exe element; cannot derive relink commands")

    # Absolute-argv sanity: every path-like element that pointed into the old
    # out must now point into the new out or at the pinned TC source.
    leftovers = [a for a in argv if a.startswith(old_out.as_posix() + "/")
                 and a not in (new_obj_str, new_systop_str,
                               (new_out / CONF0_DRIVER).as_posix(), old_tc_str)]
    if leftovers:
        die(f"unsubstituted previous-round paths remain: {leftovers}")

    # ---- build the in-memory artifact set (nothing written yet) ----------
    params = sorted(a for a in argv if a.startswith("-G"))
    defines = sorted(a for a in argv if a.startswith("+define+"))
    build_cmd = list(argv)   # canonical: CONF0 driver, obj dir = obj-ordinary

    exe_for = {c: (new_out / "bin" / f"owner-baseline-{c}").as_posix()
               for c in CONF_DRIVER}

    relink_cmds = {}
    for conf, drv in CONF_DRIVER.items():
        # Replace the conf0 driver element with this conf's driver, then insert
        # an absolute -o so the two drivers do not overwrite each other.
        cmd = list(build_cmd)
        drv_elems = [i for i, a in enumerate(cmd)
                     if a == (new_out / CONF0_DRIVER).as_posix()]
        if len(drv_elems) != 1:
            die(f"internal: expected 1 canonical driver element; found {len(drv_elems)}")
        i = drv_elems[0]
        cmd[i] = (new_out / drv).as_posix()
        exe_i = cmd.index("--exe")
        cmd = cmd[:exe_i + 1] + ["-o", exe_for[conf]] + cmd[exe_i + 1:]
        relink_cmds[conf] = cmd

    run_cmds = {}
    for conf in CONF_DRIVER:
        for first in firsts:
            tag = f"{conf}.{first}-first"
            trace = (new_out / "traces" / f"{tag}.{args.bytes}.csv").as_posix()
            run_cmds[tag] = [
                exe_for[conf],
                "+mode=s",
                "+natural=1",
                f"+natural-bytes={args.bytes}",
                f"+natural-first={first}",
                f"+natural-trace={trace}",
            ]

    # ---- compute hashes before writing -----------------------------------
    hashes = {
        "pinned_tc_source": {"path": old_tc_str, "sha256": sha256_file(old_tc)},
        "base_command_json": {"path": base_cmd_path.as_posix(),
                              "sha256": sha256_file(base_cmd_path)},
        "source_system_top": {"path": old_systop_str, "sha256": sha256_file(old_systop)},
        "source_conf0_driver": {"path": old_driver0.as_posix(),
                                "sha256": sha256_file(old_driver0)},
        "source_conf1_driver": {"path": old_driver1.as_posix(),
                                "sha256": sha256_file(old_driver1)},
    }

    # ---------------------------------------------------------------------
    # All pre-checks passed. Now (and only now) create the new output tree.
    # ---------------------------------------------------------------------
    (new_out / "models" / "ordinary_baseline").mkdir(parents=True, exist_ok=True)
    (new_out / "traces").mkdir(parents=True, exist_ok=True)
    (new_out / "bin").mkdir(parents=True, exist_ok=True)

    copies = {
        SYSTEM_TOP_NAME: old_systop,
        CONF0_DRIVER: old_driver0,
        CONF1_DRIVER: old_driver1,
    }
    copy_hashes = {}
    for name, src in copies.items():
        dst = new_out / name
        shutil.copy2(src, dst)
        if sha256_file(dst) != hashes[
                {"system_top.sv": "source_system_top",
                 "natural_coupled_test.cpp": "source_conf0_driver",
                 "natural_queue_test.cpp": "source_conf1_driver"}[name]]["sha256"]:
            die(f"verbatim copy of {name} did not reproduce the source hash")
        copy_hashes[name] = {"src": src.as_posix(), "sha256": sha256_file(dst)}

    # Canonical previous-round command, fully substituted (CONF0 driver).
    # NOTE: the pinned TC copy is still referenced from the previous out/.
    (new_out / BASE_CMD_NAME).write_text(
        json.dumps(build_cmd, indent=2) + "\n", encoding="utf-8")

    model_dir = new_out / "models" / "ordinary_baseline"
    (model_dir / "build-command.json").write_text(
        json.dumps(build_cmd, indent=2) + "\n", encoding="utf-8")
    for conf, cmd in relink_cmds.items():
        (model_dir / f"relink.{conf}.json").write_text(
            json.dumps(cmd, indent=2) + "\n", encoding="utf-8")
    for tag, cmd in run_cmds.items():
        (model_dir / f"run.{tag}.json").write_text(
            json.dumps(cmd, indent=2) + "\n", encoding="utf-8")

    (model_dir / "params.json").write_text(json.dumps({
        "note": "control parameters/driver paths are explicit for Codex review; "
                "nothing inapplicable is auto-enabled",
        "G_flags": params,
        "defines": defines,
        "forbidden_macro": FORBIDDEN_DEFINE,
        "top_module": TOP_MODULE,
        "obj_dir": new_obj_str,
        "expected_default_exe_assumption":
            f"{new_obj_str}/V{TOP_MODULE} (verilator default; verify)",
        "pinned_tc_source": old_tc_str,
    }, indent=2) + "\n", encoding="utf-8")

    (model_dir / "drivers.json").write_text(json.dumps({
        "conf0": {"role": "CONF=0 common read/write boundary",
                  "path": (new_out / CONF0_DRIVER).as_posix()},
        "conf1": {"role": "CONF=1 decouple_rw",
                  "path": (new_out / CONF1_DRIVER).as_posix()},
        "exe": exe_for,
        "run_trace_header": TRACE_HEADER,
    }, indent=2) + "\n", encoding="utf-8")

    (new_out / "candidates.md").write_text(
        "# Candidate models (this round)\n\n"
        "Prepared (1): **ordinary same-resource baseline** -- reuse of "
        f"`{base_cmd_path.name}` with only Mdir / system_top.sv / driver "
        "re-pointed; capacity-fixed TC copy pinned by reference + hash.\n\n"
        "NOT prepared (Codex decision required):\n"
        "  * virtual-W-head source-read gate -- the only existing switch "
        "(`-GSourceReadGateEnable`) is consumed ONLY by "
        "`nonblock_completion_pool_late`, which this round's "
        "`-GSharedPoolMode=0` never instantiates; enabling it needs "
        "SharedPoolMode>=4 (=> CompletionGateEnable => TEST_COMPLETION_GATE), "
        "which is forbidden this round, and its internal source is not part of "
        "the reviewed file set.\n"
        "  * HeaderPark / bypass -- `-GHeaderParkMode` is inert without "
        "TEST_COMPLETION_GATE (tests/nonblock_banked_top.sv:641-648).\n",
        encoding="utf-8")

    (new_out / "manifest.json").write_text(json.dumps({
        "schema": "write-owner-baseline-prep-v1",
        "root": root.as_posix(),
        "old_out": old_out.as_posix(),
        "new_out": new_out.as_posix(),
        "models": models,
        "bytes": args.bytes,
        "first_orders": firsts,
        "hashes": hashes,
        "copies": copy_hashes,
        "replacement_counts": {
            "Mdir": 1,
            "system_top.sv": 1,
            "driver_cpp": 1,
            "pinned_tc_source": 1,
        },
        "runs_prepared": sorted(run_cmds.keys()),
        "relinks_prepared": sorted(relink_cmds.keys()),
        "not_built": True,
        "not_run": True,
    }, indent=2) + "\n", encoding="utf-8")

    print("prepare_owner_baseline: OK")
    print(f"  new out dir : {new_out.as_posix()}")
    print(f"  build cmd   : {(new_out / BASE_CMD_NAME).as_posix()}")
    print(f"  model dir   : {model_dir.as_posix()}")
    print(f"  pinned TC   : {old_tc_str} (sha256 {hashes['pinned_tc_source']['sha256'][:16]}...)")
    print(f"  prepared runs: {sorted(run_cmds.keys())}")
    print("  no build, no DUT run, production/tests untouched")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
```

**脚本假设 / 残留风险（必须由 Codex 复核）**

- `build-command.json` 假定沿用基线“无 `-o`”的输出名约定（`<Mdir>/V<top>`）；脚本只把该假设写进 `params.json`，不写进 argv。若实际 exe 名不同，只需改 `exe_for`。
- relink 命令**新增** `-o <abs>`（插在 `--exe` 之后）以区分两个 driver；这是派生命令，不修改复用的源 argv。若 Codex 不接受新增元素，删掉该三行即可。
- 悬空的 `id[2]` vs `w_fifo_data[0]` 不确定性（见 1.3）不影响脚本，但影响任何“W-head 门”结论的可信度。
- 脚本**拒绝** `--models headerpark` 等：因为这需要本轮明确禁用的 `TEST_COMPLETION_GATE`/`SharedPoolMode>=4`。这是有意的，不是缺陷。

---

# 3. 关键验收：可核 / 不可核

## 3.1 现有 driver 能直接核的（都在两个 `natural_*_test.cpp` 里）

| 验收项 | 依据 | 说明 |
|---|---|---|
| 完整 copy + 源字节不变 | `natural_coupled_test.cpp:922-929`（同样在 `natural_queue_test.cpp`） | 逐 word 比对 pre-copy 快照 |
| 目标 guard（未写 head/tail 保持原值）| `:931-945`（`off_beats=384`，`:931-944`） | `copied` 判定区间外的必须等于 `page_old` |
| per-DMA physical R/W 几何 | `:947-959` | 每路 R/W = bytes/8，且区域不交叉 |
| NEXT-ID 严格 DONE 匹配 | `:1790-1812`（`trigger`/`wait_done`） | 只与 `trigger()` 捕获的 NEXT id 比较，绝不接受 stale DONE |
| 终止 quiet | `:1938-1974` | `dbg_in_flight==0 && dbg_dma_job_quiet==3`，且 `mem_q/caw_q/cb_q` 清空 |
| 原始 AW/WLAST 观察 | `:1247-1264`（`paw`/`pwlast`）、summary `:986-1002` | 按 `(id>>4)&3==2` 过滤真 completion master |
| read-admission VALID 保持（上/下游 AR）| `:1109-1149` | 仅覆盖读准入代理的 AR 通道 |
| 失败时真实退出码 + 原始队列快照 | `:3235-3249`（`completed:false/verified:false`、`rc=1`）、`dump_stall` `:1982-1992`（`NATURAL-QUEUE/RQ/WQ`）| 快照只打 stdout/stderr |

## 3.2 需要**新观测**或**原源审查**才能说的

- **源 R 字节 ↔ 当前可写目的段** 的逐字节对应：现有 driver 只做**区域分类**（`nat_pregion` `:134-140`），不能判定“这 1024 B 属于 A 之前还是之后的 V 段”。这正是 `...md:55`/`codex-i-refinement.md:5,15` 点出的空洞，需要新增按源偏移→目的译址→W 序的配对观测。
- **`id[2]` vs `w_fifo_data[0]` 的真实位义**：需 `axi_mux` 源或独立只读 tap（见 1.3）。
- **pool 内部的门控级**：`nonblock_completion_pool_late.sv` 未提供，`SourceReadGateEnable`/`Frontier*` 的实际进入 AR 通道位置无法给出行号。
- **同 ID 保序（completion AR）**：driver 明确声明“无法配对合法的 W-before-AW，仅为观测器限制，不是 DUT 协议裁决”（`:962-983`）；`nonblock_completion_banked_ctl.sv:558-563` 的 same-ID 顺序断言只在 `TEST_COMPLETION_GATE` 构建中存在。
- **C0 计数缺陷是否已修**：本包给出的 `trans_ctl_write_limit.sv` 已含 `w_slots_full`（`:181-185`、使用于 `:211-212`、`:633`），但这是**实验副本**；谓生产源已修仍需 Codex 复核（`...md:39` 明确“尚未应用到生产源”）。

## 3.3 明确不能主张的

- 不能因“现有强普通门（含 `SourceReadGateEnable`/`HeaderPark`）在本轮基线里存在”就断言**新工作负结果**或**创新成立**：这些开关在 `-GSharedPoolMode=0` 下**根本没有被例化**（`tests/nonblock_actual_dma_top.sv:556-577`），逻辑上不参与本轮任何一次运行。
- 不能把“同 owner”当“字节可写”的保证（1.5 已给反例）。
- 不能把读/写互堵描述为 MIG 死锁：`...md:69` 明确“不是已证明的 MIG 死锁”，DDR 只是串行 FIFO 行为模型（`...md:11`）。
- 不能声明任何性能/面积结论：本轮无窗口计数（`win_armed`/`report_window` 只在 `run_scenario` 路径使用 `:2581`，`run_natural` 不开启）。
