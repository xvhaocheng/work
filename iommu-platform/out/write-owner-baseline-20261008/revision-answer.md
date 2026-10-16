# 聚焦修订（rev-2）：§1.5 更正 + 新 out 目标 + 准备脚本重写

根目录：`/home/xvhaocheng/work/iommu-platform`（沿用 `out/natural-write-order-20261008/build-write-limit-command.json:25` 的绝对前缀）。
本轮**只出准备脚本，不构建、不运行、不改 SV**。`codex-baseline-spec.md` / `ordinary_copy_owner_gate.sv` / `owner_actual_dma_top.sv` / `worker-answer.md` / `production-before.json` 视为既有输入，脚本**必须读到、写不到**。

---

## A. 修订点清单（相对上一答）

1. **撤回** `worker-answer.md:60` 的“四个开关的全部可见判据只有单一 ID 位相等”。`tests/nonblock_completion_pool_late.sv:94-106` 的 `can_drain` 确有 descriptor 偏移 / len / size 严格匹配（`:102-105`）。
2. 由此更正 §1.5 的论证链：结论（不能用旧开关证明普通方法负结果/创新）不变，但理由换成**固定 4 KiB / Depth8 / len255 几何不适用 + Mode0 不例化**。
3. 闭合原 `worker-answer.md:54` 的最大证据缺口：源读门控就在 `nonblock_completion_pool_late.sv:264-269`（`source_read_eligible` → `m_req_o[b].ar_valid` / `s_rsp_o[b].ar_ready`）。
4. 新增关键更正：该门控是 **sticky 释放**（`:327-328`），不是逐 AR 检查（原答隐含“每个 AR 都过判据”是错的）。
5. `future_drain` 仅 `Mode==6||7`（`:113`）；`frontier_drain` 仅 `Mode==7`（`:162`）——Mode4 下二者惰性。
6. 新 out 的 build 命令改为**单模型 `obj-owner`**：`-GSourceReadGateEnable=1`（新接口，见 `codex-baseline-spec.md:5`），其余诊断全 0，无 `+define+TEST_COMPLETION_GATE`。
7. `tests/nonblock_actual_dma_top.sv` → 新 out `owner_actual_dma_top.sv`（唯一替换），并在其**前**插入新 out `ordinary_copy_owner_gate.sv`，脚本内断言 `index(gate)+1 == index(top)`。
8. TC 副本、`system_top.sv`、C++ driver、`--Mdir` **全部迁入新 out**；源文件 sha 双记录（src 与 dst 必须相等）。
9. 默认 run 只有 **CONF0 × {v-first, a-first} × 32768 B**；CONF1 只准备 driver 副本 + relink + 非默认 run（几何不满足，`codex-baseline-spec.md:9`）。
10. 脚本**拒绝覆盖任何已存在文件**（内容/哈希不一致即 abort，且在任何写入之前 abort）。
11. 补充：`axi_mux.sv` 现已提供，原 §1.3 的 `id[2]` vs `w_fifo_data[0]` 疑问可在本轮收口（见 C.3）。
12. 不触碰任何模型/运行时设置（`max 393216` 保持不变）。

---

## B. §1.5 更正（替换 `worker-answer.md:56-63`）

> 原文：“这四个开关的全部可见判据只有单一 ID 位的相等（`id[2]`）加上 W-FIFO 的‘非空/头’标志（`:607-614`）。**owner 相等不是字节/地址保证。**”

**不成立，撤回。** 补齐 `tests/nonblock_completion_pool_late.sv` 后，准入函数 `can_drain`（`:94-106`）除 owner 位（`:99-100`）外还要求：

- `ar.addr >= src_q[b] && aw_addr_q[b][n] >= dst_q[b]`（`:102`）
- `(ar.addr - src_q[b]) == (aw_addr_q[b][n] - dst_q[b])` —— 源读偏移与 virtual AW 目的偏移**逐字节相等**（`:103`）
- `ar.len == aw_len_q[b][n]`（`:104`）
- `ar.size == aw_size_q[b][n]`（`:104`），且 `ar.burst == 2'b01 && ar.size == 3'd3`（`:105`）
- 前置计数：`paw_count_q[b] > vw_done_q[b] && pw_done_q[b] == vw_done_q[b]`（`:101`），`job_active_q[b] && n < Depth && aw_count_q[b] > vw_done_q[b]`（`:98`）

`aw_*_q` 在 virtual AW 首次 offer 时捕获（写 `:416-424`；端口见 `tests/nonblock_actual_dma_top.sv:606-615`）。所以“owner 相等之外没有任何对齐约束”的论断必须删除。

**但真正的限制是固定几何，而不是判据缺失：**

- `Depth = 8`（`:46`），AW ring 8 槽（`:62-64`），越界 → `fault_o[13]`（`:361`、`:365`）。
- 单 job 必须 `job_length_i[b] == 4096 && job_src_i[b][11:0] == 0 && job_dst_i[b][11:0] == 0`（`:403-404`）。
- 源 AR 必须 `ar.len == 255 && ar.size == 3 && ar.burst == 1 && ar.id == 0`，且 `ar.addr - src_q[b] < 4096`（`:443-446`）。
- virtual AW 必须 `aw_addr_i - dst_q[b] < 4096 && aw_size_i == 3`（`:424`）。
- 单 slot（`:54`，`:87-91`）。

⇒ **4 KiB aligned / Depth8 / len255 是本轮真实、未消除的约束**，不能用于 32 KiB 错位任务。原结论（不得用旧开关宣称普通方法负结果、不得宣称创新）仍成立，但理由改为：**几何不适用 + `SharedPoolMode=0` 下该池根本不例化**（`tests/nonblock_actual_dma_top.sv:556-577`）。

**门控进入 AR 的确切位置（原最大缺口，现闭合）：**

```
nonblock_completion_pool_late.sv:264-265
  source_read_eligible[b] = !SourceReadGateEnable || source_read_release_q[b] ||
                            can_drain(b, s_req_i[b].ar) || future_drain(b, s_req_i[b].ar);
nonblock_completion_pool_late.sv:266-269
  m_req_o[b].ar_valid   = s_req_i[b].ar_valid && req_seq_q[b]-tx_seq_q[b] < Depth && source_read_eligible[b];
  s_rsp_o[b].ar_ready   = m_rsp_i[b].ar_ready && req_seq_q[b]-tx_seq_q[b] < Depth && source_read_eligible[b];
```

**新更正：sticky 而非逐 AR。** `source_read_release_q[b]` 在一旦 `m_req_o[b].ar_valid` 后被置位（`:327`），此后 `:264` 无条件放行，直到 `meta_in[b]`（`:328`）或 job_fire（`:408`）清零。因此**不能**声称“每个源 AR 都被整段准备判据检查过”。

**原反例的正确表述：** V 的 2048 B 源读（`docs/natural-write-order-boundary-20261008.md:49,55`）在本 gate 下不会仅凭 owner 相等放行——只要该 AR 的 `len` 与 ring 中当前 virtual AW 的 `len` 不等（`:104`），`can_drain` 即为 0。但这**不能反向证明该 gate 正确**：`aw_len_q` 捕获的是 virtual AW 的 len，iDMA 实际 AR 拍数与其是否一一对应，本轮未核证（见 C）。

---

## C. 新 out 的两份 SV（只读，不改）与关键未证点

### C.1 引用
- `codex-baseline-spec.md:3` — 新实现只放 out；no-pool Mode0 保持生产 `banked_ctl`、容量副本、BUF3、逆读/交错全关。
- `codex-baseline-spec.md:5` — `ordinary_copy_owner_gate.sv` 位于每 DMA 源 AR 与四输入 mux 之间，仅门控 AR VALID/READY，AW/W/B/R 直通；`SourceReadGateEnable` 在本轮 out 的 `owner_actual_dma_top.sv` Mode0 分支选择该门；**是新普通对照接口，不能叫旧开关直接生效**。
- `codex-baseline-spec.md:9` — 严格整段准备规则；CONF0 联合分段可满足；**CONF1 首 R 2 KiB vs 首 W 1 KiB 不满足**。
- `codex-baseline-spec.md:11` — 首 AR VALID 后 sticky hold 到 AR 握手；每流一个 normal active job、固定 AXI input ID0、64 bit INCR。
- 生产 `rv_iommu_banked_ctl` 与 `tests/nonblock_banked_top.sv:641-648` 的关系不变：无 `TEST_COMPLETION_GATE` 时例化生产 `rv_iommu_banked_ctl`，且 `tests/nonblock_banked_top.sv:635-637` 反向保护不变。**本轮不得定义该宏**。

### C.2 `ordinary_copy_owner_gate.sv` 关键未证 / 需 Codex 复核

| # | 位置 | 未证点 |
|---|---|---|
| 1 | `:45-46` vs `codex-baseline-spec.md:9` | spec 说“对应目标 AW 已到 **physical completion**”；代码只查 `p_aw_q[s] > v_done_q[s]`（physical AW **已 offer**），无 B/completion 计数。**spec 与实现措辞不等价**。 |
| 2 | `:43`,`:49`,`:52` | `n = v_done_q[s][2:0]` 作 ring 索引，validity 只靠 AW offer 时 `:113-114` 的越界 fault 保护；若 AR 迟迟不到，ring 可能已回绕。未证。 |
| 3 | `:52-55` | 只比 `addr/src` 与 `aw_addr/dst` 的偏移相等 + `len`/`size` 相等；**不检查段间连续性、不检查 `aw_addr_q[s][n] + bytes <= dst_q[s]+length`**。 |
| 4 | `:90-92` | `p_aw_q[s] <= v_aw_q[s]` 是假设（physical AW 不领先 virtual AW），代码内只 fatal 不证明。 |
| 5 | `:84-86` | `held_q` 只在“已 valid 未 ready”时置位；上游一旦撤回 VALID 直接 `$fatal`，即**协议假设**而非证明。 |
| 6 | `:44` | `read_bytes = (64'(len)+1) << 3` 只在 `size==3`（8 B/beat）成立，与 `:55` 一致；若上游出非 INCR/非 8 B 拍即无效。 |
| 7 | `:103` | 新 job fire 时若同一 cycle 该流有 virtual AW offer 就 fatal —— 依赖 top 的 job/AW 时序，未见 `owner_actual_dma_top.sv` 源码，无法核证。 |
| 8 | 全局 | 完全不做 W/R payload 观察，也不核 `r.last` 与 `len` 对账；gate 只保证 AR 侧放行条件。 |
| 9 | 全局 | 若 `owner_actual_dma_top.sv` 的 Mode0 分支没有按 spec 例化该 gate（该文件未提供），则本轮 `-GSourceReadGateEnable=1` **完全无效**。这是本脚本最大的外部依赖，必须由 Codex 打开该文件核验。 |

**结论口径：** 以上任一未证点若成立，失败/通过都**不能**被解释为“等价普通方法不行”，也不能作为 I 方案创新证据。本轮脚本只准备命令，不做任何此类主张。

### C.3 原 §1.3 疑问（`id[2]` vs `w_fifo_data[0]`）现可收口

`cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:329` — `w_fifo` 入 `mst_aw_chan.id[SlvAxiIDWidth+:MstIdxBits]`；`:331` 出 `w_fifo_data`；`:353` 用它选 W slave。即 `w_fifo_data` = **slave port index**。对 4 入 mux（`MstIdxBits=2`）`w_fifo_data[0]` 是 port index bit0；pool 相关端口 0/1 下 bit0 恰为流号。故 `v_whead_*`（bit0）与 physical 侧 `comp_aw_id[2]`（pool 的 4 bit master id 高位）**在 pool 端口 0/1 上一致**；port 2/3 不参与，不作为结论使用。原 §1.3 “最脆弱观测量”降级为已解释，但仍建议 Codex 复核 `nonblock_actual_dma_top.sv:610/651` 的实际位选。

---

## D. 完整修订脚本

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""prepare_owner_baseline.py -- revision 2 (§32 owner-gate build prep).

PREPARE ARTIFACTS ONLY.
  * does NOT build, does NOT run the DUT;
  * never touches production RTL, tests/, or any pre-existing artifact;
  * reads <root>/out/natural-write-order-20261008,
    reads + never overwrites <root>/out/write-owner-baseline-20261008.

Round-2 deltas vs revision 1 (§31):
  * single model "owner", --Mdir <new_out>/obj-owner;
  * TC copy + system_top copy + C++ drivers + Mdir all MOVE into <new_out>
    (rev-1 kept the TC reference pointing into the old out/);
  * tests/nonblock_actual_dma_top.sv -> <new_out>/owner_actual_dma_top.sv,
    and <new_out>/ordinary_copy_owner_gate.sv is inserted immediately BEFORE it;
  * -GSourceReadGateEnable is flipped 0 -> 1 exactly once. In this round it is
    the NEW Mode0 gate select inside the out-owned top (codex-baseline-spec.md:5),
    NOT the old late-pool switch (that one only existed under SharedPoolMode>=4);
  * all other diagnostics stay 0; +define+TEST_COMPLETION_GATE stays absent;
  * default run set = CONF0 x {v-first, a-first} x 32768 B only;
    CONF1 artifacts go to optional/ and are never default (its first R = 2 KiB
    vs first W = 1 KiB does not satisfy the strict whole-segment rule,
    codex-baseline-spec.md:9);
  * the five pre-existing <new_out> artifacts are REQUIRED to exist, are hashed
    into manifest.json, and are never written or overwritten;
  * NO file is ever overwritten: a pre-existing file must be byte-identical
    (text) or hash-identical (copy), otherwise the script aborts BEFORE any write.

Hard invariants (violation exits non-zero BEFORE any write):
  * every required source path exists;
  * every substituted element occurs EXACTLY once;
  * +define+TEST_COMPLETION_GATE absent, and the define set is exactly
    {+define+ASSERTS_OFF};
  * the post-substitution argv contains ONLY -G flags from REQUIRED_FLAGS, so no
    HeaderPark / late-pool / frontier diagnostic can be auto-enabled;
  * <new_out>/ordinary_copy_owner_gate.sv is immediately before
    <new_out>/owner_actual_dma_top.sv in the final argv.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
import sys
from pathlib import Path

# ------------------------------------------------------------------ layout --
OLD_REL = "out/natural-write-order-20261008"
NEW_REL = "out/write-owner-baseline-20261008"

BASE_CMD_NAME   = "build-write-limit-command.json"
OLD_OBJ_NAME    = "obj-write-limit"
NEW_OBJ_NAME    = "obj-owner"
SYSTEM_TOP_NAME = "system_top.sv"
TC_NAME         = "trans_ctl_write_limit.sv"
OLD_DRIVER_NAME = "natural_test.cpp"          # element present in the reused argv
CONF0_DRIVER    = "natural_coupled_test.cpp"  # copied verbatim, default build driver
CONF1_DRIVER    = "natural_queue_test.cpp"    # optional copy this round

NEW_GATE_NAME = "ordinary_copy_owner_gate.sv"
NEW_TOP_NAME  = "owner_actual_dma_top.sv"
OLD_TOP_REL   = "tests/nonblock_actual_dma_top.sv"

PRESERVED = (
    "codex-baseline-spec.md",
    "ordinary_copy_owner_gate.sv",
    "owner_actual_dma_top.sv",
    "worker-answer.md",
    "production-before.json",
)

FORBIDDEN_DEFINE = "TEST_COMPLETION_GATE"
ALLOWED_DEFINES  = ["+define+ASSERTS_OFF"]

TOP_MODULE = "nonblock_actual_dma_system_top"

SRC_GATE_OLD = "-GSourceReadGateEnable=0"
SRC_GATE_NEW = "-GSourceReadGateEnable=1"

# Every -G flag that may appear in the FINAL argv. Anything else -> abort.
REQUIRED_FLAGS = {
    "-GPolicyEnable=1":           "policy elaboration (production-like)",
    "-GDmaBufferDepth=3":         "small iDMA byte FIFO (BUF3 same-resource baseline)",
    "-GSharedPoolMode=0":         "no completion pool; production banked_ctl path",
    "-GPoolSlots=1":              "single slot (inert at Mode0)",
    "-GSameBankDmasEnable=1":     "two real DMAs on one bank (BankIdBit=3)",
    "-GInterleaveAwEnable=0":     "forced AW interleave off",
    "-GSourceReadGateEnable=1":   "NEW out-only Mode0 gate select (NOT the old late-pool switch)",
    "-GFrontierEnable=0":         "frontier off",
    "-GHeaderParkMode=0":         "header park off",
    "-GReverseReadOrderEnable=0": "reverse read order off",
}

# Testbench modules that must stay in the file list exactly once, untouched.
STILL_ONCE_REL = (
    "tests/nonblock_completion_pool.sv",
    "tests/nonblock_completion_pool_late.sv",
    "tests/nonblock_banked_top.sv",
)

TRACE_HEADER = "cycle,event,region,id,addr,len,extra"


# ----------------------------------------------------------------- helpers --
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


def replace_exactly_once(argv, old: str, new: str, what: str, counts: dict):
    n = count_exact(argv, old)
    if n != 1:
        die(f"expected exactly 1 {what} element {old!r} in the reused command; found {n}")
    out = list(argv)
    out[out.index(old)] = new
    counts[what] = counts.get(what, 0) + 1
    return out


def require_flag(argv, flag: str, why: str) -> None:
    n = count_exact(argv, flag)
    if n != 1:
        die(f"required control flag {flag!r} ({why}) appears {n} times, expected 1")


def require_absent_define(argv, name: str) -> None:
    for a in argv:
        if name in a:
            die(f"forbidden macro {name!r} present in reused command ({a!r}); "
                "this round must not define TEST_COMPLETION_GATE")


# -------------------------------------------------------------------- main --
def main(argv=None) -> int:
    ap = argparse.ArgumentParser(prog="prepare_owner_baseline.py")
    ap.add_argument("project_root",
                    help="absolute path to the iommu-platform project root")
    ap.add_argument("--model", default="owner",
                    help="single model name; only 'owner' is supported this round")
    ap.add_argument("--bytes", default="32768",
                    help="+natural-bytes per default run (default 32768)")
    ap.add_argument("--first-orders", default="v,a",
                    help="comma list of +natural-first values (v|a)")
    args = ap.parse_args(argv)

    if args.model != "owner":
        die(f"this round prepares only the single model 'owner'; got {args.model!r}. "
            "HeaderPark / late-pool variants need TEST_COMPLETION_GATE and/or "
            "new RTL and are explicitly out of scope; Codex decides them.")

    if not args.bytes.isdigit() or int(args.bytes) <= 0:
        die(f"--bytes must be a positive integer; got {args.bytes!r}")

    firsts = [f.strip() for f in args.first_orders.split(",") if f.strip()]
    if not firsts:
        die("--first-orders must name at least one order")
    for f in firsts:
        if f not in ("v", "a"):
            die(f"--first-orders entries must be v|a; got {f!r}")

    # ---- absolute root + path pre-checks (BEFORE any write) ---------------
    root = Path(args.project_root)
    if not root.is_absolute():
        die("project_root must be an absolute path")
    if not root.is_dir():
        die(f"project_root missing or not a directory: {root}")

    old_out = root / OLD_REL
    new_out = root / NEW_REL

    if not old_out.is_dir():
        die(f"previous-round out dir missing: {old_out}")
    if not new_out.is_dir():
        die(f"round-2 target dir must already exist with the preserved artifacts: {new_out}")

    # ---- the five pre-existing round-2 artifacts: MUST exist, never written
    preserved_sha = {}
    for name in PRESERVED:
        p = new_out / name
        if not p.is_file():
            die(f"required pre-existing round-2 artifact missing: {p}")
        preserved_sha[name] = sha256_file(p)

    # ---- required sources -------------------------------------------------
    base_cmd_path = old_out / BASE_CMD_NAME
    old_systop    = old_out / SYSTEM_TOP_NAME
    old_tc        = old_out / TC_NAME
    old_driver0   = old_out / CONF0_DRIVER
    old_driver_ref = old_out / OLD_DRIVER_NAME
    old_driver1   = old_out / CONF1_DRIVER          # optional this round

    for p, what in (
        (base_cmd_path,  "previous-round build command JSON"),
        (old_systop,     "previous-round read-only system_top.sv"),
        (old_tc,         "previous-round capacity-fixed TC copy"),
        (old_driver0,    "previous-round CONF0 driver (coupled)"),
        (old_driver_ref, "driver element referenced by the reused command"),
    ):
        if not p.is_file():
            die(f"{what} not found: {p}")

    conf1_available = old_driver1.is_file()

    # ---- load + sanity-check the reused command ---------------------------
    try:
        base_argv = json.loads(base_cmd_path.read_text(encoding="utf-8"))
    except Exception as exc:  # noqa: BLE001
        die(f"cannot parse {base_cmd_path}: {exc}")
    if not isinstance(base_argv, list) or not all(isinstance(x, str) for x in base_argv):
        die(f"{base_cmd_path} must contain a JSON array of strings")
    if not base_argv:
        die(f"{base_cmd_path} is empty")

    require_absent_define(base_argv, FORBIDDEN_DEFINE)

    # exactly one source-read-gate element, flipped 0 -> 1 (value change, not add)
    if count_exact(base_argv, SRC_GATE_OLD) != 1:
        die(f"expected exactly 1 {SRC_GATE_OLD!r} in the reused command; "
            f"found {count_exact(base_argv, SRC_GATE_OLD)}")
    argv = list(base_argv)
    argv[argv.index(SRC_GATE_OLD)] = SRC_GATE_NEW

    # every other -G flag must already be present exactly once with the round-2 value
    for flag, why in REQUIRED_FLAGS.items():
        require_flag(argv, flag, why)

    # no diagnostic may be auto-added
    extra_g = sorted({a for a in argv if a.startswith("-G")} - set(REQUIRED_FLAGS))
    if extra_g:
        die(f"unexpected -G flag(s) in the derived command (no auto-enable allowed): {extra_g}")

    defines = sorted(a for a in argv if a.startswith("+define+"))
    if defines != ALLOWED_DEFINES:
        die(f"define set must be exactly {ALLOWED_DEFINES}; got {defines}")

    if count_exact(argv, "--exe") != 1:
        die("reused command must have exactly one --exe element")

    # ---- exact, single-occurrence substitutions ---------------------------
    counts: dict = {}

    argv = replace_exactly_once(
        argv, (old_out / OLD_OBJ_NAME).as_posix(),
        (new_out / NEW_OBJ_NAME).as_posix(), "Mdir", counts)

    argv = replace_exactly_once(
        argv, old_systop.as_posix(),
        (new_out / SYSTEM_TOP_NAME).as_posix(), "system_top.sv", counts)

    argv = replace_exactly_once(
        argv, old_tc.as_posix(),
        (new_out / TC_NAME).as_posix(), "TC copy", counts)

    argv = replace_exactly_once(
        argv, old_driver_ref.as_posix(),
        (new_out / CONF0_DRIVER).as_posix(), "driver cpp", counts)

    # top replacement + gate insertion immediately before it
    argv = replace_exactly_once(
        argv, (root / OLD_TOP_REL).as_posix(),
        (new_out / NEW_TOP_NAME).as_posix(), "top module", counts)

    top_str  = (new_out / NEW_TOP_NAME).as_posix()
    gate_str = (new_out / NEW_GATE_NAME).as_posix()
    i_top = argv.index(top_str)
    argv.insert(i_top, gate_str)
    if argv[i_top] != gate_str or argv[i_top + 1] != top_str:
        die("internal error: gate is not immediately before the new top")
    counts["gate_insert_before_top"] = 1

    # ---- post-conditions --------------------------------------------------
    if count_exact(argv, (root / OLD_TOP_REL).as_posix()) != 0:
        die("old tests/nonblock_actual_dma_top.sv still present after substitution")
    if count_exact(argv, gate_str) != 1:
        die(f"expected exactly 1 gate source {gate_str!r}")
    if count_exact(argv, top_str) != 1:
        die(f"expected exactly 1 new top {top_str!r}")

    for rel in STILL_ONCE_REL:
        if count_exact(argv, (root / rel).as_posix()) != 1:
            die(f"expected exactly 1 untouched testbench source {rel!r}")

    leftovers = [a for a in argv
                 if a.startswith(old_out.as_posix() + "/")
                 and a != (new_out / NEW_OBJ_NAME).as_posix()]
    if leftovers:
        die(f"unsubstituted previous-round paths remain: {leftovers}")

    new_argv = argv  # canonical: CONF0 driver, Mdir=obj-owner

    # ---- derived C++ relink commands --------------------------------------
    model_dir = new_out / "models" / args.model

    exe_for = {c: (new_out / "bin" / f"owner-baseline-{c}").as_posix()
               for c in ("conf0", "conf1")}

    relink = {}
    # conf0: canonical argv already references <new_out>/natural_coupled_test.cpp
    relink["conf0"] = list(new_argv) + ["-o", exe_for["conf0"]]
    # conf1: swap the driver element only, and only if the copy is available
    if conf1_available:
        cmd = replace_exactly_once(
            list(new_argv), (new_out / CONF0_DRIVER).as_posix(),
            (new_out / CONF1_DRIVER).as_posix(), "conf1 driver cpp", {})
        relink["conf1"] = cmd + ["-o", exe_for["conf1"]]

    # ---- run commands: default = CONF0 only, 32 KiB, v-first and a-first --
    def make_run(conf: str, first: str, is_default: bool) -> dict:
        tag = f"{conf}.{first}-first.{args.bytes}"
        trace = (new_out / "traces"  / f"{tag}.csv").as_posix()
        return {
            "tag": tag,
            "default": is_default,
            "argv": [
                exe_for[conf],
                "+mode=s",
                "+natural=1",
                f"+natural-bytes={args.bytes}",
                f"+natural-first={first}",
                f"+natural-trace={trace}",
            ],
            "cwd": new_out.as_posix(),
            "stdout_log": (new_out / "logs" / f"{tag}.stdout.log").as_posix(),
            "stderr_log": (new_out / "logs" / f"{tag}.stderr.log").as_posix(),
            "exit_code_file": (new_out / "results" / f"{tag}.rc").as_posix(),
            "result_json": (new_out / "results" / f"{tag}.result.json").as_posix(),
            "trace_csv": trace,
            "trace_header": TRACE_HEADER,
            "notes": (
                "default CONF0 run" if is_default else
                "CONF1 NOT a default run: first R = 2 KiB vs first W = 1 KiB does "
                "not satisfy the strict whole-segment readiness rule "
                "(codex-baseline-spec.md:9). Run only if Codex explicitly re-plans "
                "the geometry."
            ),
        }

    default_runs = {}
    for first in firsts:
        r = make_run("conf0", first, True)
        default_runs[r["tag"]] = r

    optional_runs = {}
    if conf1_available:
        for first in firsts:
            r = make_run("conf1", first, False)
            optional_runs[r["tag"]] = r

    # ---- build the in-memory artifact set (nothing written yet) -----------
    params = sorted(a for a in new_argv if a.startswith("-G"))
    defines_final = sorted(a for a in new_argv if a.startswith("+define+"))

    plan_text: dict = {}
    plan_copy: list = []

    plan_text[(new_out / BASE_CMD_NAME)] = json.dumps(new_argv, indent=2) + "\n"
    plan_text[model_dir / "build-command.json"] = json.dumps(new_argv, indent=2) + "\n"
    for conf, cmd in relink.items():
        plan_text[model_dir / f"relink.{conf}.json"] = json.dumps(cmd, indent=2) + "\n"
    for tag, r in default_runs.items():
        plan_text[model_dir / f"run.{tag}.json"] = json.dumps(r, indent=2) + "\n"
    for tag, r in optional_runs.items():
        plan_text[model_dir / "optional" / f"run.{tag}.json"] = json.dumps(r, indent=2) + "\n"

    plan_text[model_dir / "params.json"] = json.dumps({
        "model": args.model,
        "note": ("explicit control parameters this round; nothing inapplicable "
                 "(HeaderPark / late-pool / frontier diagnostics) is auto-enabled"),
        "G_flags": params,
        "defines": defines_final,
        "forbidden_macro": FORBIDDEN_DEFINE,
        "source_read_gate_note": (
            "-GSourceReadGateEnable=1 selects the NEW out-only Mode0 gate "
            "(out/write-owner-baseline-20261008/ordinary_copy_owner_gate.sv, "
            "see codex-baseline-spec.md:5). It is NOT the old late-pool switch "
            "(tests/nonblock_completion_pool_late.sv:264), which was only "
            "reachable under SharedPoolMode>=4 and is inert here."),
        "top_module": TOP_MODULE,
        "obj_dir": (new_out / NEW_OBJ_NAME).as_posix(),
        "top_source": (new_out / NEW_TOP_NAME).as_posix(),
        "gate_source": (new_out / NEW_GATE_NAME).as_posix(),
    }, indent=2) + "\n"

    plan_text[model_dir / "drivers.json"] = json.dumps({
        "conf0": {"role": "CONF=0 common read/write boundary (default build driver)",
                  "path": (new_out / CONF0_DRIVER).as_posix()},
        "conf1": {"role": "CONF=1 decouple_rw (optional, NOT a default run)",
                  "path": (new_out / CONF1_DRIVER).as_posix() if conf1_available else None},
        "source_conf0": old_driver0.as_posix(),
        "source_conf1": old_driver1.as_posix() if conf1_available else None,
        "exe": exe_for,
        "run_trace_header": TRACE_HEADER,
    }, indent=2) + "\n"

    plan_copy += [
        (old_systop,   new_out / SYSTEM_TOP_NAME),
        (old_tc,       new_out / TC_NAME),
        (old_driver0,  new_out / CONF0_DRIVER),
    ]
    if conf1_available:
        plan_copy.append((old_driver1, new_out / CONF1_DRIVER))

    # ---- NO-OVERWRITE pre-scan (still before any write) -------------------
    conflicts = []
    for dst, text in plan_text.items():
        if dst.exists():
            try:
                cur = dst.read_text(encoding="utf-8")
            except Exception:  # noqa: BLE001
                conflicts.append(f"{dst} exists and is not readable as UTF-8 text")
                continue
            if cur != text:
                conflicts.append(f"{dst} exists with different content")
    for src, dst in plan_copy:
        if dst.exists():
            if sha256_file(dst) != sha256_file(src):
                conflicts.append(f"{dst} exists with a different hash than {src}")
    if conflicts:
        die("refusing to overwrite pre-existing files:\n  - " + "\n  - ".join(conflicts))

    # ---- (only now) create directories and write --------------------------
    for d in (new_out / "bin", new_out / "traces", new_out / "logs",
              new_out / "results", model_dir, model_dir / "optional"):
        d.mkdir(parents=True, exist_ok=True)

    copy_hashes = {}
    for src, dst in plan_copy:
        if not dst.exists():
            shutil.copy2(src, dst)
        h_src, h_dst = sha256_file(src), sha256_file(dst)
        if h_src != h_dst:
            die(f"verbatim copy of {src} -> {dst} did not reproduce the source hash")
        copy_hashes[dst.as_posix()] = {"src": src.as_posix(), "sha256": h_dst}

    text_written = {}
    for dst, text in plan_text.items():
        if not dst.exists():
            dst.write_text(text, encoding="utf-8")
            text_written[dst.as_posix()] = "written"
        else:
            text_written[dst.as_posix()] = "identical-existing"

    # ---- manifest ---------------------------------------------------------
    manifest = {
        "schema": "write-owner-baseline-prep-v2",
        "root": root.as_posix(),
        "old_out": old_out.as_posix(),
        "new_out": new_out.as_posix(),
        "model": args.model,
        "obj_dir": (new_out / NEW_OBJ_NAME).as_posix(),
        "top_module": TOP_MODULE,
        "bytes": args.bytes,
        "first_orders": firsts,
        "preserved_artifact_sha256": preserved_sha,
        "source_sha256": {
            "base_command_json": {"path": base_cmd_path.as_posix(),
                                  "sha256": sha256_file(base_cmd_path)},
            "system_top":        {"path": old_systop.as_posix(),
                                  "sha256": sha256_file(old_systop)},
            "tc_copy":           {"path": old_tc.as_posix(),
                                  "sha256": sha256_file(old_tc)},
            "conf0_driver":      {"path": old_driver0.as_posix(),
                                  "sha256": sha256_file(old_driver0)},
            "conf1_driver":      ({"path": old_driver1.as_posix(),
                                   "sha256": sha256_file(old_driver1)}
                                  if conf1_available else None),
        },
        "copied_files": copy_hashes,
        "text_files": text_written,
        "replacement_counts": counts,
        "argv_insert": {
            "gate": gate_str,
            "index": i_top,
            "immediately_before": top_str,
        },
        "params": {"G_flags": params, "defines": defines_final},
        "forbidden_macro": FORBIDDEN_DEFINE,
        "default_runs": sorted(default_runs.keys()),
        "optional_runs_not_default": sorted(optional_runs.keys()),
        "relink_prepared": sorted(relink.keys()),
        "expected_exe": exe_for,
        "trace_header": TRACE_HEADER,
        "not_built": True,
        "not_run": True,
    }
    man_path = new_out / "manifest.json"
    if man_path.exists():
        if man_path.read_text(encoding="utf-8") != json.dumps(manifest, indent=2) + "\n":
            die(f"refusing to overwrite pre-existing {man_path}")
    else:
        man_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")

    print("prepare_owner_baseline: OK")
    print(f"  new out dir   : {new_out.as_posix()}")
    print(f"  canonical cmd : {(new_out / BASE_CMD_NAME).as_posix()}")
    print(f"  model dir     : {model_dir.as_posix()}")
    print(f"  obj dir       : {(new_out / NEW_OBJ_NAME).as_posix()}")
    print(f"  gate before   : {gate_str}  (index {i_top})")
    print(f"  default runs  : {sorted(default_runs.keys())}")
    if optional_runs:
        print(f"  optional runs : {sorted(optional_runs.keys())} (NOT default)")
    print(f"  preserved     : {', '.join(PRESERVED)} (hashed, untouched)")
    print("  no build, no DUT run, production/tests untouched")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
```

---

## E. 绝对路径 / 调用 / 验收 manifest 接口

调用（Codex 侧工具执行，本轮不执行）：

```
python3 prepare_owner_baseline.py /home/xvhaocheng/work/iommu-platform \
        --model owner --bytes 32768 --first-orders v,a
```

预期产出（全部在 `/home/xvhaocheng/work/iommu-platform/out/write-owner-baseline-20261008/`）：

| 路径 | 角色 | 默认 |
|---|---|---|
| `build-write-limit-command.json` | 规范化 build argv（CONF0 driver，`--Mdir=.../obj-owner`） | — |
| `models/owner/build-command.json` | 同上副本 | — |
| `models/owner/relink.conf0.json` | CONF0 relink（argv + `-o bin/owner-baseline-conf0`） | 是 |
| `models/owner/relink.conf1.json` | CONF1 relink（仅当 `natural_queue_test.cpp` 存在） | 否（只准备、不跑） |
| `models/owner/run.conf0.v-first.32768.json` | run ① | **是** |
| `models/owner/run.conf0.a-first.32768.json` | run ② | **是** |
| `models/owner/optional/run.conf1.*.json` | CONF1 几何不满足，非默认 | 否 |
| `models/owner/params.json` | `-G` 集合 / defines / forbidden macro / gate 说明 | — |
| `models/owner/drivers.json` | driver 路径、角色、exe 路径、trace 表头 | — |
| `manifest.json` | `schema=write-owner-baseline-prep-v2`：preserved sha、src sha、copy sha、替换计数、gate 插入位、`not_built/not_run` | — |
| `bin/`, `traces/`, `logs/`, `results/` | 空目录，供 run 时写 exe / CSV / stdout / rc | — |

每个 run JSON 内含：`argv`（含 `+natural-trace=<abs>.csv`）、`cwd`、`stdout_log`、`stderr_log`、`exit_code_file`、`result_json`、`trace_csv`、`trace_header`。**run 不执行**；`result_json` 由 Codex 的执行工具在跑完后写入。

### 验收检查（脚本自带，abort 前 0 次写入）

1. 5 个既有 artifact 必须存在（否则 abort），且 sha 进 manifest，脚本永不写它们。
2. `tests/nonblock_actual_dma_top.sv` 恰好 1 次 → 换成新 out top；gate 插在其前一格，脚本断言 `argv[idx]==gate && argv[idx+1]==top`。
3. `-GSourceReadGateEnable=0` 恰好 1 次 → 改成 `=1`；改后 `-G` 集合必须**严格等于** `REQUIRED_FLAGS` 的键（任何 HeaderPark / latepool / frontier 新增即 abort）。
4. define 集合必须严格等于 `["+define+ASSERTS_OFF"]`（无 `TEST_COMPLETION_GATE`）。
5. 替换后 argv 中不得再有 `out/natural-write-order-20261008/` 前缀元素。
6. 无覆盖：目标已存在且内容/哈希不同 → abort。

### 假设 / 未决（交 Codex）

- **`owner_actual_dma_top.sv` 未提供**：`-GSourceReadGateEnable=1` 是否真的在 Mode0 分支例化新 gate，只能由 Codex 打开该文件核验（`codex-baseline-spec.md:5`）。若未接线，本轮任何 run 都无意义。
- **driver 参数约定**沿用上一轮 `+mode=s +natural=1 +natural-bytes= +natural-first= +natural-trace=`；本轮未提供 driver 源码，无法核证。若实际参数不同，只改 `make_run`。
- **`-o <abs exe>`** 追加在 argv 末尾（Verilator 位置不限，但属派生命令，非复用源 argv）；若 Codex 不接受新增元素，删掉 `+ ["-o", exe]` 两处即可。
- **`+natural-first=v` 先跑、`a` 后跑** 只是 run 命名约定，不代表执行顺序；任务只要求两个默认 run。
- **脚本不生成仿真结果解析器**（任务允许省略）：CSV/日志/rc 文件名已在 run JSON 中固定，Codex 的执行工具直接用。
- `ordinary_copy_owner_gate.sv` 的 9 项未证点见 C.2；任何基于该 gate 的负结果/创新主张都需先消除它们。
- 未触碰模型/运行时配置；`max 393216` 等保持不变。
