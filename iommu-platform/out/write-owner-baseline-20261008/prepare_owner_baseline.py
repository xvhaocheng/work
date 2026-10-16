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

