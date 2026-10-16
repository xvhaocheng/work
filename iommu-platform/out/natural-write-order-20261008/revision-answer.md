## 摘要

交付 `apply_worker_revision.py`：只读改写一个文件 `out/natural-write-order-20261008/apply_natural.py`，用 **5 个 anchor**（覆盖要求的 4 项改动）做精确替换，每个 anchor 必须先 `count == 1` 才写盘；任一 anchor 不为 1 就整体抛错、**不写任何东西**（重复执行会显式失败而不是二次打补丁）。脚本本身不读/写 C++ 源、不生成产物、不做备份（Codex 自行保存原件），并会把 unified diff 打到 stdout 供复核。

改动落点（当前文件行号）：

| 改动 | anchor 位置 | 内容 |
|---|---|---|
| 1 | 830–831 | `nat_vaw_valid` 自检由 `!= 3` 改为 `!= 2` + 准确注释（port decl + assign lhs；rhs 是 `i_dma.tr_aw_valid`），自检保留 |
| 2 | 699–700 | parser 内 `strtoul/strtoull` **endptr** 核：`+natural` 严格 `0\|1`（拒绝 `2`/`0x1`/`1x`/空），`+natural-bytes` 必须是完整十进制（拒绝 trailing 垃圾）；原有 `2048\|32768` 与 `natural-first/trace` 校验原样保留 |
| 3a | 713（`if (natural) {` 之后） | `+natural=1` 仅允许 `+mode=s`、`read_admit=="none"`、无 `+cpu-writes`，否则 stderr + `return 2` |
| 3b | 384 注释行与 `scenario_start` 之间 | `run_natural()` 在 `reset(false)` 之后强制 `policy_partition = false; owner_only = false;`，消除与 main `tb.policy_partition = policy;` 的矛盾 |
| 4 | 472–479 | 终验新增 `n_nat_vw_unattr == 0`、`n_nat_vw_mismatch == 0` 两个 CHECK，并把 WLAST/队列/长度类失败信息统一改为 “**observer cannot pair legal W-before-AW; not a DUT verdict**”，不宣称 DUT 违约；byte 校验、DDR 规则、任务长短/IOVA/PA/tick 采样、`run_scenario` 路径完全不动 |

## 完整脚本

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
apply_worker_revision.py

Bounded revision of exactly ONE file:

    out/natural-write-order-20261008/apply_natural.py

(Codex saves its own copy of the original first; this script makes no backup,
reads/creates/modifies no generated artifact and no source file.)

Five exact-anchor replacements implementing the four requested changes:

  (1) self-check fix        nat_vaw_valid is expected exactly 2 times (output
                            port declaration + assign lhs); the assign rhs is
                            "i_dma.tr_aw_valid", a different string.  The
                            self-check is kept, only its expected value and its
                            comment are corrected.
  (2) strict flag parsing   +natural must be exactly 0|1 and +natural-bytes
                            must be a complete decimal number: strtoul /
                            strtoull endptr is checked right in the argument
                            parser, so "1x", "2048x", "", "0x1" are refused.
                            The existing 2048|32768 check and the existing
                            +natural-first / +natural-trace checks stay.
  (3) natural run gate      +natural=1 is accepted only with +mode=s,
                            read_admit == "none" and no +cpu-writes, and
                            run_natural() forces policy_partition = false and
                            owner_only = false so the main-level +policy knob
                            can never contradict the natural run's reset(false).
  (4) terminal observation  nat_verify_and_report() additionally requires zero
                            unattributed virtual W beats and zero paired length
                            mismatches, and every virtual AW/W pairing failure is
                            worded as an observer limitation
                            ("observer cannot pair legal W-before-AW; not a DUT
                            verdict") instead of a DUT protocol verdict.

Untouched: DDR model rules, task length / IOVA / PA / tick sampling, the normal
tb.run_scenario() path, the byte-comparison logic, and every other anchor.

Every anchor must occur exactly once in the current target text; otherwise the
script raises and writes nothing.  A second run therefore fails loudly instead
of double-patching.
"""

import difflib
import os
import sys

ROOT   = "/home/xvhaocheng/work/iommu-platform"
TARGET = os.path.join(ROOT, "out", "natural-write-order-20261008",
                      "apply_natural.py")

# ---------------------------------------------------------------------------
# (1) accurate nat_vaw_valid self-check  (apply_natural.py lines 830-831)
# ---------------------------------------------------------------------------
A1_OLD = r'''    if top.count("nat_vaw_valid") != 3:      # port, assign lhs, assign rhs
        raise AssertionError("nat_vaw_valid count %d != 3" % top.count("nat_vaw_valid"))'''

A1_NEW = r'''    # nat_vaw_valid occurs exactly twice in the emitted top: once as the
    # output port declaration and once as the assign lhs.  The assign rhs is
    # "i_dma.tr_aw_valid", a different string that is not counted here.
    if top.count("nat_vaw_valid") != 2:      # port decl + assign lhs
        raise AssertionError("nat_vaw_valid count %d != 2" % top.count("nat_vaw_valid"))'''

# ---------------------------------------------------------------------------
# (2) strict 0|1 and complete-decimal parsing, endptr checked in the parser
#     (apply_natural.py lines 699-700, inside CPP_REPL_ARGPARSE)
# ---------------------------------------------------------------------------
A2_OLD = r'''        if (const char* v = val("natural")) natural = (std::strtoul(v, nullptr, 0) != 0);
        if (const char* v = val("natural-bytes")) natural_bytes = std::strtoull(v, nullptr, 0);'''

A2_NEW = r'''        if (const char* v = val("natural")) {
            char* end = nullptr;
            const unsigned long nv = std::strtoul(v, &end, 10);
            if (end == v || *end != '\0' || nv > 1) {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: +natural must be exactly 0 or 1\n");
                return 2;
            }
            natural = (nv != 0);
        }
        if (const char* v = val("natural-bytes")) {
            char* end = nullptr;
            natural_bytes = std::strtoull(v, &end, 10);
            if (end == v || *end != '\0') {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: +natural-bytes must be a complete "
                    "decimal number (2048|32768, no trailing characters)\n");
                return 2;
            }
        }'''

# ---------------------------------------------------------------------------
# (3a) natural may only run as the ordinary +mode=s scenario
#      (apply_natural.py line 713, top of the `if (natural) {` validation block)
# ---------------------------------------------------------------------------
A3_OLD = r'''
    if (natural) {'''

A3_NEW = r'''
    if (natural) {
        // +natural=1 drives only the ordinary single-task +mode=s scenario.
        // Refuse any combination with the optional request-shaping knobs
        // instead of silently running something else.
        if (mode != "s") {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural=1 requires +mode=s (got "
                "\"%s\")\n", mode.c_str());
            return 2;
        }
        if (read_admit != "none") {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural=1 cannot be combined with "
                "+read-admit=%s\n", read_admit.c_str());
            return 2;
        }
        if (cpu_writes) {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural=1 cannot be combined with "
                "+cpu-writes\n");
            return 2;
        }'''

# ---------------------------------------------------------------------------
# (3b) run_natural() must not inherit a main-level policy/owner selection
#      (apply_natural.py lines 384-385, right after reset(false))
# ---------------------------------------------------------------------------
A4_OLD = r'''// +mode=s semantics: nonblock off
        scenario_start'''

A4_NEW = r'''// +mode=s semantics: nonblock off
        // +mode=s: force the ordinary phase so the main-level +policy knob can
        // never make this natural run contradict its own reset(false).
        policy_partition = false;
        owner_only = false;
        scenario_start'''

# ---------------------------------------------------------------------------
# (4) terminal virtual-W observation: unattributed / len-mismatch must be
#     checked, and pairing failures are worded as observer limitations
#     (apply_natural.py lines 472-479)
# ---------------------------------------------------------------------------
A5_OLD = r'''        CHECK(n_nat_var > 0 && n_nat_vaw > 0,
              "natural: no virtual read/write request was observed");
        CHECK(n_nat_vw_beats == 2ULL * want,
              "natural: virtual W beats != both copies' destination beats");
        CHECK(n_nat_vwlast == n_nat_vaw,
              "natural: virtual WLAST count != virtual AW count");
        CHECK(nat_vaw_q.empty(),
              "natural: virtual AW / WLAST pairing queue did not drain");'''

A5_NEW = r'''        // The virtual-W observer pairs a W beat only with the head of the queue
        // of virtual AW handshakes it really saw.  A legal W-before-AW (a W beat
        // that precedes its own AW on this port) cannot be paired by this
        // observer, so the failures below are reported as OBSERVER limitations,
        // never as a DUT protocol verdict.
        CHECK(n_nat_var > 0 && n_nat_vaw > 0,
              "natural: no virtual read/write request was observed");
        CHECK(n_nat_vw_beats == 2ULL * want,
              "natural: virtual W beats != both copies' destination beats; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(n_nat_vwlast == n_nat_vaw,
              "natural: virtual WLAST count != virtual AW count; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(n_nat_vw_unattr == 0,
              "natural: unattributed virtual W beat observed; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(n_nat_vw_mismatch == 0,
              "natural: paired virtual W length mismatch; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(nat_vaw_q.empty(),
              "natural: virtual AW / WLAST pairing queue did not drain; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");'''

EDITS = [
    ("(1) self-check: nat_vaw_valid count 3 -> 2", A1_OLD, A1_NEW),
    ("(2) parse: strict 0|1 and complete-decimal endptr checks", A2_OLD, A2_NEW),
    ("(3a) validate: +natural=1 requires +mode=s / no read-admit / no cpu-writes",
     A3_OLD, A3_NEW),
    ("(3b) run_natural: force policy_partition=false, owner_only=false",
     A4_OLD, A4_NEW),
    ("(4) terminal: unattributed / len-mismatch CHECKs with observer wording",
     A5_OLD, A5_NEW),
]

# Cheap post-conditions: exact occurrence counts after the rewrite.
POST = [
    ("nat_vaw_valid count %d != 3", 0),
    ("nat_vaw_valid count %d != 2", 1),
    ("+natural must be exactly 0 or 1", 1),
    ("+natural-bytes must be a complete ", 1),
    ("policy_partition = false;", 1),
    ("owner_only = false;", 1),
    ("observer cannot pair legal W-before-AW; not a DUT verdict", 5),
]


def main():
    if not os.path.isfile(TARGET):
        raise SystemExit("apply_worker_revision: target not found: %s" % TARGET)

    with open(TARGET, "r", encoding="utf-8") as fh:
        old = fh.read()

    text = old
    for tag, anchor, repl in EDITS:
        n = text.count(anchor)
        if n != 1:
            raise AssertionError(
                "anchor %s occurs %d times in %s (expected exactly 1); "
                "nothing written" % (tag, n, TARGET))
        text = text.replace(anchor, repl, 1)

    for marker, want in POST:
        got = text.count(marker)
        if got != want:
            raise AssertionError(
                "post-check %r: %d occurrences, expected %d; nothing written"
                % (marker, got, want))

    with open(TARGET, "w", encoding="utf-8") as fh:
        fh.write(text)

    sys.stdout.write("".join(difflib.unified_diff(
        old.splitlines(keepends=True), text.splitlines(keepends=True),
        fromfile=TARGET + " (before)", tofile=TARGET + " (after)")))

    print("")
    print("apply_worker_revision: %d anchored edits applied to %s"
          % (len(EDITS), TARGET))
    print("  (1) nat_vaw_valid self-check now expects 2 (port decl + assign lhs)")
    print("  (2) +natural strictly 0|1, +natural-bytes complete decimal (endptr)")
    print("  (3) +natural=1 requires +mode=s / no read-admit / no cpu-writes;")
    print("      run_natural forces policy_partition=false, owner_only=false")
    print("  (4) terminal W observer: unattributed/mismatch checked, observer wording")
    print("")
    print("deliberately NOT done here: TEST_COMPLETION_GATE macro, counter")
    print("observation, production root-cause verdict, byte-check logic, DDR")
    print("rules, task lengths, IOVA/PA/tick sampling, normal run_scenario().")


if __name__ == "__main__":
    main()
```

## 假设 / 未决依赖（需 Codex 复核）

1. **`return 2;` 位于 parser 块**：假设 `CPP_ANCHOR_ARGPARSE` 所在的 `val(...)` 解析块与既有 `+read-period` 校验块同处 `main()`（后者已直接 `return 2;`）。若它在某个非 `int` 返回的 helper 里，编译会立刻报错，需改为置错误标志 + 在 `CPP_REPL_VALID` 里返回 2。
2. **`owner_only` 成员名**：`policy_partition` 已由脚本原文 `tb.policy_partition = policy;` 证实；`owner_only` 是从 run_natural 注释 “no owner-only gate” 推断的 Tb 成员。若实际成员名不同（或没有该成员），只需改 A4_NEW 里那一行。我无法看到 `tests/nonblock_actual_dma_test.cpp` / RTL，未做验证。
3. **未加 `ds_priority`/`batch`/`window` 的显式比较**：这些 main 侧标识符名在交付文本中不可见，按“CPU/batch/window 不引入、本轮均为 0/none”的要求，只用已知的 `mode`/`read_admit`/`cpu_writes` 做门禁，owner/policy 由 3b 强制清零。
4. 未改动 `n_nat_vw_unattr` / `n_nat_vw_mismatch` 的计数逻辑（tick 采样里已存在，仅新增终验 CHECK）。

## 建议验收步骤（我无法执行）

1. 对保存的原件与改写后文件 `diff -u`：应只有 5 个 hunk，全部落在上表 5 处；其余行不动。
2. `python3 -c "import ast;ast.parse(open('out/natural-write-order-20261008/apply_worker_revision.py').read())"` 语法自检。
3. 再跑 `python3 out/natural-write-order-20261008/apply_natural.py`：应在原 830 行处通过（count==2），继续走 CPP 阶段。注意：**上次失败发生在任何 write 之前，CPP 阶段 anchors 的“唯一性”从未被真正执行过**，此处仍可能出现第二个 anchor 失败，属可预期残余风险。
4. 跑 `build-natural.sh`，检查生成的 `natural_test.cpp` 含新解析/门禁/终验文本；负例：`+natural=2`、`+natural-bytes=2048x`、`+natural=1 +mode=<非s>`、`+read-admit=<非none>`、`+cpu-writes` 均应 stderr 报错并 `exit 2`。
5. 正例 `+mode=s +natural=1 +natural-bytes=2048|32768 +natural-first=v|a +natural-trace=<path>`：`NATURAL-SUMMARY` 中 `vw_unattributed`、`vw_len_mismatch` 为 0；若非 0，失败信息须含 “observer cannot pair legal W-before-AW; not a DUT verdict”，据此只作观察记录、不下 DUT 违约结论。
