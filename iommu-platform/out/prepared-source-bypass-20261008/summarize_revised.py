#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""§39 普通已准备源读物理旁路 —— 离线证据汇总（脚本提案，作者未执行）。

约束：
1. 只读本目录下 6 个新运行的产物与顶层 build 产物；不运行 DUT、不改源码文件、不改 CSV。
2. 六场景（off/cache/wait × v/a）全部保留；失败场景与缺失产物不得从输出中消失。
3. SV 事件 cycle（RAW4/PREP4/POSTTL/DIRECT39-AR.cycle）与 TB issue/done cycle
   （PLACEMENT-METRICS-JSON.flow.*.issue_to_done_cycles）不同钟原点，
   脚本内不做任何跨域算术；均值与旧值比较只用 TB issue/done cycle。
4. 致命（fatal）条件：DIRECT39-STATS offered==0、busy==0；DIRECT39-HOLD held==0；
   BYPASS38-STATS held==0；DIRECT39-AR 行数==direct（含 direct==0）；
   DIRECT39-AR state2/write1 计数==write_wait；顶层 build exit_code==0、reused is False。
5. held 属性与 R 路由关键断言由 Codex RTL/C++ 验证；本脚本不重跑、不冒称作者。
"""

from __future__ import annotations

import hashlib
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = Path(__file__).resolve().parent
REF38 = OUT.parent / "multi-parent-preparation-20261008"

MODES = (("off", 0), ("cache", 1), ("wait", 2))   # 场景名 -> +physical-bypass=N
FLOWS = ("v", "a")

TOTAL_SUB_SLICES = 64          # direct + fallback
EXPECTED_POSTTL_PARENTS = 32   # 源原父
EXPECTED_POSTTL_CHILDREN = 64  # 物理源子片
EXPECTED_PAR_TOTAL = 64
EXPECTED_PAW_TOTAL = 34        # 目的 AW
EXPECTED_BEATS_PER_DMA = 4096  # 各路 4096 R/W
TL_ACCEPT_BASELINE_38 = 66     # 只对比、只报告，不猜性能原因
TL_RESP_BASELINE_38 = 66
RAW4_FUNCTIONAL_BITS = 3317    # 3316 + 1 固定 route
PREP4_FUNCTIONAL_BITS = 1281
DECLARED_TOTAL_BITS = RAW4_FUNCTIONAL_BITS + PREP4_FUNCTIONAL_BITS  # 4598，仅声明状态

EXPECTED_HELD = 0              # DIRECT39-HOLD / BYPASS38-STATS 的 held 期望值
BUILD_RESULT = OUT / "build-result.json"
BUILD_COMMAND = OUT / "build-command.json"

# 外部给定参考值（§38 审核报告），本脚本不重算、不重跑旧 log
REF38_BEST_ORDINARY_MEAN = 24751
REF38_POST_MEAN = 18824

AR_FIELDS = ("cycle", "owner", "parent", "addr", "tc_state", "tc_write")
DIRECT39_FIELDS = ("mode", "direct", "fallback", "write_wait", "offered", "busy", "observation_bits")

TOKEN_RE = re.compile(r"(\w+)=(\S+)")


# ---------------------------------------------------------------- 基础读取

def read_json_file(path):
    if not path.is_file():
        return None, f"缺少文件: {path}"
    try:
        return json.loads(path.read_text(encoding="utf-8")), None
    except Exception as exc:  # pragma: no cover
        return None, f"JSON 解析失败: {path}: {exc}"


def read_text_file(path):
    if not path.is_file():
        return None, f"缺少文件: {path}"
    try:
        return path.read_text(encoding="utf-8", errors="replace"), None
    except Exception as exc:  # pragma: no cover
        return None, f"读取失败: {path}: {exc}"


def all_lines(text, prefix):
    return [ln for ln in text.splitlines() if ln.startswith(prefix + " ")]


def tokens(line):
    out = {}
    for key, value in TOKEN_RE.findall(line):
        out[key] = int(value) if re.fullmatch(r"\d+", value) else value
    return out


def sha256_hex(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def signed(x):
    """带符号整数显示（如 +33）；非整数原样转字符串。"""
    if isinstance(x, bool):
        return "%+d" % int(x)
    if isinstance(x, int):
        return "%+d" % x
    return str(x)


def fmt1(x):
    if isinstance(x, bool):
        return str(x)
    if isinstance(x, (int, float)):
        return "%.1f" % float(x)
    return str(x)


def num(x):
    """数值写进文本：整数不带小数点，非整浮点用 repr（可回读精度）。"""
    if isinstance(x, bool):
        return str(int(x))
    if isinstance(x, float):
        return str(int(x)) if x.is_integer() else repr(x)
    return str(x)


# ---------------------------------------------------------------- 运行记录

class Run:
    def __init__(self, name, mode, flow, physical_bypass):
        self.name = name
        self.mode = mode
        self.flow = flow
        self.physical_bypass = physical_bypass
        self.checks = []
        self.anomalies = []       # 非致命，只报告
        self.facts = {}
        self.source_lines = {}    # 前缀 -> 原始行，作为字段级证据
        self.fatal = []           # 致命失败 id
        self.missing = []         # 缺失产物路径
        self.status = "no_evidence"

    def check(self, cid, ok, detail, fatal=True):
        ok = bool(ok)
        self.checks.append({"id": cid, "ok": ok, "fatal": bool(fatal), "detail": detail})
        if not ok:
            if fatal:
                self.fatal.append(cid)
            else:
                self.anomalies.append({"id": cid, "detail": detail})

    def missing_artifact(self, path, why):
        self.missing.append(str(path))
        self.check("artifact:" + Path(path).name, False, why)

    def finalize(self):
        if self.fatal:
            self.status = "missing_evidence" if self.missing else "fail"
        else:
            self.status = "evidence_ok"

    def to_json(self):
        return {
            "name": self.name,
            "mode": self.mode,
            "flow": self.flow,
            "physical_bypass": self.physical_bypass,
            "status": self.status,
            "fatal_failures": self.fatal,
            "missing_artifacts": self.missing,
            "anomalies": self.anomalies,
            "checks": self.checks,
            "facts": self.facts,
            "source_lines": self.source_lines,
        }


def take_stat(run, text, prefix):
    found = all_lines(text, prefix)
    if len(found) != 1:
        run.check(f"{prefix} 行数==1", False, f"{prefix} 行数={len(found)}")
        return None
    run.source_lines[prefix] = found[0]
    return tokens(found[0])


def take_json(run, text, prefix):
    found = all_lines(text, prefix)
    if len(found) != 1:
        run.check(f"{prefix} 行数==1", False, f"{prefix} 行数={len(found)}")
        return None
    run.source_lines[prefix] = found[0]
    try:
        return json.loads(found[0][len(prefix) + 1:])
    except Exception as exc:
        run.check(f"{prefix} JSON 可解析", False, f"{prefix} JSON 解析失败: {exc}")
        return None


# ---------------------------------------------------------------- 单次运行

def analyze_run(mode, physical_bypass, flow):
    name = f"{mode}-{flow}-32768"
    run = Run(name, mode, flow, physical_bypass)

    # --- result.json
    result, err = read_json_file(OUT / f"{name}-result.json")
    if result is None:
        run.missing_artifact(OUT / f"{name}-result.json", err)
    else:
        run.facts["result"] = result
        run.check("result.exit_code==0", result.get("exit_code") == 0,
                  f"exit_code={result.get('exit_code')!r}")
        run.check("result.reused is False", result.get("reused") is False,
                  f"reused={result.get('reused')!r}")

    # --- command.json
    cmd, err = read_json_file(OUT / f"{name}-command.json")
    if cmd is None:
        run.missing_artifact(OUT / f"{name}-command.json", err)
    elif not isinstance(cmd, list):
        run.check("command.is_list", False, f"command.json 不是 argv 数组: {type(cmd).__name__}")
    else:
        argv = [str(x) for x in cmd]
        run.facts["argv"] = argv
        for tok in (f"+physical-bypass={physical_bypass}", "+reuse=2", "+prepare-order=0"):
            run.check(f"command has {tok}", tok in argv, f"argv 中缺少 {tok}")
        for tok in ("+mode=s", "+natural=1", "+natural-bytes=32768",
                    "+warmup=0", "+read-admit=none", "+ptw-latency=31"):
            run.check(f"command inherits {tok}", tok in argv,
                      f"沿用 §38 bypass-aw-first 的参数 {tok} 未出现（沿用口径需人工确认）",
                      fatal=False)

    # --- .log
    text, err = read_text_file(OUT / f"{name}.log")
    if text is None:
        run.missing_artifact(OUT / f"{name}.log", err)
        run.finalize()
        return run

    lines = text.splitlines()
    pass_count = lines.count("nonblock_actual_dma_test: PASS")
    run.facts["dut_pass_line_count"] = pass_count
    run.check("log PASS 行恰好 1 条", pass_count == 1,
              f"nonblock_actual_dma_test: PASS 行数={pass_count}")

    raw_decl = take_stat(run, text, "RAW4-DECL")
    prep_decl = take_stat(run, text, "PREP4-DECL")
    raw_stats = take_stat(run, text, "RAW4-STATS")
    prep_stats = take_stat(run, text, "PREP4-STATS")
    posttl = take_stat(run, text, "POSTTL-STATS")
    direct_stats = take_stat(run, text, "DIRECT39-STATS")
    direct_hold = take_stat(run, text, "DIRECT39-HOLD")
    bypass38_stats = take_stat(run, text, "BYPASS38-STATS")
    nat = take_json(run, text, "NATURAL-JSON")
    pl = take_json(run, text, "PLACEMENT-METRICS-JSON")

    # --- RAW4-DECL：3317 = 3316 + 固定 route 1
    if raw_decl is not None:
        run.facts["raw4_decl"] = raw_decl
        run.check("RAW4-DECL functional_bits==3317",
                  raw_decl.get("functional_bits") == RAW4_FUNCTIONAL_BITS,
                  f"functional_bits={raw_decl.get('functional_bits')}（期望 {RAW4_FUNCTIONAL_BITS}）")
        run.facts["raw4_observation_bits"] = raw_decl.get("observation_bits")

    # --- PREP4-DECL：1281（观察位独立声明，不参与控制）
    if prep_decl is not None:
        run.facts["prep4_decl"] = prep_decl
        run.check("PREP4-DECL functional_bits==1281",
                  prep_decl.get("functional_bits") == PREP4_FUNCTIONAL_BITS,
                  f"functional_bits={prep_decl.get('functional_bits')}（期望 {PREP4_FUNCTIONAL_BITS}）")
        run.facts["prep4_observation_bits"] = prep_decl.get("observation_bits")

    # --- RAW4-STATS：父槽 live 0/0，高水位 <= 4
    if raw_stats is not None:
        run.facts["raw4_stats"] = raw_stats
        run.check("RAW4 live_v==0", raw_stats.get("live_v") == 0, f"live_v={raw_stats.get('live_v')}")
        run.check("RAW4 live_a==0", raw_stats.get("live_a") == 0, f"live_a={raw_stats.get('live_a')}")
        hwv, hwa = raw_stats.get("highwater_v"), raw_stats.get("highwater_a")
        run.check("RAW4 highwater_v<=4", isinstance(hwv, int) and hwv <= 4, f"highwater_v={hwv}")
        run.check("RAW4 highwater_a<=4", isinstance(hwa, int) and hwa <= 4, f"highwater_a={hwa}")

    # --- PREP4-STATS：enabled/order；slow-hit-valid-offer 全 0；reused 只报告不设 64
    if prep_stats is not None:
        run.facts["prep4_stats"] = prep_stats
        run.check("PREP4-STATS enabled==1", prep_stats.get("enabled") == 1,
                  f"enabled={prep_stats.get('enabled')}")
        run.check("PREP4-STATS order==0", prep_stats.get("order") == 0,
                  f"order={prep_stats.get('order')}")
        for key in ("slow", "hit", "valid", "offer"):
            run.check(f"PREP4-STATS {key}==0", prep_stats.get(key) == 0,
                      f"{key}={prep_stats.get(key)}")
        run.facts["prep4_source_reuse_reported"] = {
            "reused": prep_stats.get("reused"),
            "prepared": prep_stats.get("prepared"),
            "forwarded": prep_stats.get("forwarded"),
            "filled": prep_stats.get("filled"),
            "note": "缓存直通后 source reuse 数不再要求 64，只报告",
        }

    # --- POSTTL-STATS：源原父 32 / 物理源子片 64
    if posttl is not None:
        run.facts["posttl"] = posttl
        run.check("POSTTL parents==32", posttl.get("parents") == EXPECTED_POSTTL_PARENTS,
                  f"parents={posttl.get('parents')}")
        run.check("POSTTL children==64", posttl.get("children") == EXPECTED_POSTTL_CHILDREN,
                  f"children={posttl.get('children')}")

    # --- NATURAL-JSON：nat var 是正常 TC 入口子片数(=fallback)，不硬性要求 64
    if nat is not None:
        run.facts["natural"] = nat
        run.check("NATURAL completed && verified",
                  bool(nat.get("completed")) and bool(nat.get("verified")),
                  f"completed={nat.get('completed')} verified={nat.get('verified')}")
        run.check("NATURAL conf==1", nat.get("conf") == 1, f"conf={nat.get('conf')}")
        run.check("NATURAL src_offset==0", nat.get("src_offset") == 0,
                  f"src_offset={nat.get('src_offset')}")
        run.check("NATURAL dst_offset==3072", nat.get("dst_offset") == 3072,
                  f"dst_offset={nat.get('dst_offset')}")
        run.check("NATURAL par_total==64", nat.get("par_total") == EXPECTED_PAR_TOTAL,
                  f"par_total={nat.get('par_total')}")
        run.check("NATURAL paw_total==34", nat.get("paw_total") == EXPECTED_PAW_TOTAL,
                  f"paw_total={nat.get('paw_total')}")
        for key in ("v_src_r_beats", "a_src_r_beats", "v_dst_w_beats", "a_dst_w_beats"):
            run.check(f"NATURAL {key}==4096", nat.get(key) == EXPECTED_BEATS_PER_DMA,
                      f"{key}={nat.get(key)}")
        for key in ("vw_unattributed", "vw_len_mismatch", "vaw_queue_remaining"):
            run.check(f"NATURAL {key}==0", nat.get(key) == 0, f"{key}={nat.get(key)}")
        run.facts["nat_var_normal_tc_entry_sub_slices"] = nat.get("var")

    # --- PLACEMENT：TL 数量直接报告；偏离 66 报出，不猜原因；TB 周期单独成域
    tb = None
    if pl is not None:
        run.facts["placement"] = pl
        tl = {
            "n_tl_accept": pl.get("n_tl_accept"),
            "n_tl_resp": pl.get("n_tl_resp"),
            "n_iotlb_lookup": pl.get("n_iotlb_lookup"),
            "n_iotlb_hit": pl.get("n_iotlb_hit"),
            "n_iotlb_miss": pl.get("n_iotlb_miss"),
            "n_ddtw_ar": pl.get("n_ddtw_ar"),
            "n_ptw_ar": pl.get("n_ptw_ar"),
            "n_fast": pl.get("n_fast"),
        }
        run.facts["tl_counts"] = tl
        lookup, hit, miss = tl["n_iotlb_lookup"], tl["n_iotlb_hit"], tl["n_iotlb_miss"]
        if all(isinstance(x, int) for x in (lookup, hit, miss)):
            run.check("PLACEMENT hit+miss==lookup", hit + miss == lookup,
                      f"{hit}+{miss}!={lookup}")
        else:
            run.check("PLACEMENT hit/miss/lookup 为整数", False,
                      f"lookup={lookup!r} hit={hit!r} miss={miss!r}")
        for key, baseline in (("n_tl_accept", TL_ACCEPT_BASELINE_38),
                              ("n_tl_resp", TL_RESP_BASELINE_38)):
            val = tl.get(key)
            if val != baseline:
                run.check(f"PLACEMENT {key} 偏离 §38 基线 {baseline}", False,
                          f"{key}={val}（偏离基线 {baseline}，只报告、不猜性能原因）",
                          fatal=False)
        flow_map = pl.get("flow") or {}
        tb = {}
        for side in ("v", "a"):
            side_map = flow_map.get(side) or {}
            tb[side] = side_map.get("issue_to_done_cycles")
        for side in ("v", "a"):
            run.check(f"PLACEMENT flow.{side}.issue_to_done_cycles 为整数",
                      isinstance(tb[side], int),
                      f"flow.{side}.issue_to_done_cycles={tb[side]!r}")
        run.facts["tb_issue_to_done_cycles"] = dict(tb)
        run.facts["tb_cycles_clock_domain"] = "TB issue/done cycles（与 SV 事件 cycle 不同钟原点）"
    run.facts["tb_cycles"] = tb

    # --- DIRECT39-STATS：direct/fallback 划分 + write_wait/offered/busy 报告
    d = (direct_stats or {}).get("direct")
    fb = (direct_stats or {}).get("fallback")
    if direct_stats is not None:
        run.facts["direct39_stats"] = direct_stats
        run.facts["direct39_clock_domain"] = "SV 事件 cycle 域（不得与 TB issue/done 混算）"
        for key in DIRECT39_FIELDS:
            if key not in direct_stats:
                run.check(f"DIRECT39-STATS 含 {key}", False,
                          f"缺少字段 {key}（接口列出的字段）", fatal=False)
        mode_field = direct_stats.get("mode")
        if isinstance(mode_field, str):
            run.check("DIRECT39-STATS mode 与场景一致", mode_field == mode,
                      f"DIRECT39 mode={mode_field} 场景={mode}", fatal=False)
        if isinstance(d, int) and isinstance(fb, int):
            run.check("DIRECT39 direct+fallback==64", d + fb == TOTAL_SUB_SLICES,
                      f"direct={d} fallback={fb} 合计={d + fb}（期望 {TOTAL_SUB_SLICES}）")
        else:
            run.check("DIRECT39 direct/fallback 为整数", False,
                      f"direct={d!r} fallback={fb!r}")
        run.check("DIRECT39-STATS offered==0", direct_stats.get("offered") == 0,
                  f"offered={direct_stats.get('offered')}")
        run.check("DIRECT39-STATS busy==0", direct_stats.get("busy") == 0,
                  f"busy={direct_stats.get('busy')}")
        nat_var = (nat or {}).get("var")
        if isinstance(nat_var, int) and isinstance(fb, int):
            run.check("NATURAL var==DIRECT39 fallback（正常 TC 入口子片数）", nat_var == fb,
                      f"nat.var={nat_var} DIRECT39.fallback={fb}")
        if mode == "off":
            run.check("off: direct==0", d == 0, f"direct={d}")
            run.check("off: fallback==64", fb == TOTAL_SUB_SLICES, f"fallback={fb}")
        else:
            run.check(f"{mode}: 开关启用 direct>0", isinstance(d, int) and d > 0, f"direct={d}")
        run.facts["write_wait"] = direct_stats.get("write_wait")
        run.facts["write_wait_note"] = (
            "write_wait=真实 TC 等待写翻译期间发出的直接 AR（只报告，不设阈值）")

    # --- DIRECT39-HOLD / BYPASS38-STATS：held 必须为 0（致命）
    if direct_hold is not None:
        run.facts["direct39_hold"] = direct_hold
        run.check("DIRECT39-HOLD held==0", direct_hold.get("held") == EXPECTED_HELD,
                  f"held={direct_hold.get('held')}（期望 {EXPECTED_HELD}）")
    if bypass38_stats is not None:
        run.facts["bypass38_stats"] = bypass38_stats
        run.check("BYPASS38-STATS held==0", bypass38_stats.get("held") == EXPECTED_HELD,
                  f"held={bypass38_stats.get('held')}（期望 {EXPECTED_HELD}）")

    # --- DIRECT39-AR：SV 域逐条事件
    ar_lines = all_lines(text, "DIRECT39-AR")
    if ar_lines:
        run.source_lines["DIRECT39-AR"] = ar_lines[0]
    ars, bad = [], []
    for ln in ar_lines:
        tk = tokens(ln)
        entry = {key: tk.get(key) for key in AR_FIELDS}
        missing = [key for key in AR_FIELDS if key not in tk]
        if missing:
            bad.append((ln, f"缺字段 {missing}"))
        else:
            if not isinstance(entry["cycle"], int):
                bad.append((ln, "cycle 非整数"))
            if entry["owner"] not in (0, 1):
                bad.append((ln, f"owner={entry['owner']} 非 0/1"))
            if entry["tc_write"] not in (0, 1):
                bad.append((ln, f"tc_write={entry['tc_write']} 非 0/1"))
        ars.append(entry)
    run.facts["direct39_ar_count"] = len(ars)
    run.facts["direct39_ar_first"] = ars[0] if ars else None
    run.facts["direct39_ar_tc_write_true"] = sum(1 for x in ars if x.get("tc_write") == 1)
    run.facts["direct39_ar_tc_state2_write1"] = sum(
        1 for x in ars if x.get("tc_state") == 2 and x.get("tc_write") == 1)
    if bad:
        run.check("DIRECT39-AR 字段完整且取值合法", False,
                  f"异常行数={len(bad)}，示例: {bad[0][0]} | {bad[0][1]}", fatal=False)
    # 行数必须等于 direct（direct==0 时也必须为 0），致命
    if isinstance(d, int):
        run.check("DIRECT39-AR 行数==direct（含 direct==0）",
                  len(ars) == d,
                  f"AR 行数={len(ars)} DIRECT39.direct={d}")
    # state2/write1 计数必须等于 write_wait，致命
    if direct_stats is not None:
        ww = direct_stats.get("write_wait")
        s2w1 = run.facts["direct39_ar_tc_state2_write1"]
        if isinstance(ww, int):
            run.check("DIRECT39-AR state2/write1 计数==write_wait", s2w1 == ww,
                      f"state2/write1={s2w1} write_wait={ww}")
        else:
            run.check("DIRECT39-AR state2/write1 计数==write_wait", False,
                      f"state2/write1={s2w1}；write_wait={ww!r} 非整数，无法核对")

    # --- off 必须与 §38 对应 CSV 逐字节相同
    if mode == "off":
        new_csv = OUT / f"{name}.csv"
        ref_csv = REF38 / f"bypass-aw-first-{flow}-32768.csv"
        if new_csv.is_file() and ref_csv.is_file():
            nb, rb = new_csv.read_bytes(), ref_csv.read_bytes()
            run.facts["csv_path"] = str(new_csv)
            run.facts["csv_sha256"] = sha256_hex(nb)
            run.facts["ref38_csv_path"] = str(ref_csv)
            run.facts["ref38_csv_sha256"] = sha256_hex(rb)
            run.check("off.csv 与 §38 bypass-aw-first.csv 逐字节相同", nb == rb,
                      f"{new_csv.name}({len(nb)}B, sha256={run.facts['csv_sha256'][:16]}) "
                      f"vs {ref_csv}({len(rb)}B, sha256={run.facts['ref38_csv_sha256'][:16]})")
        else:
            run.check("off.csv 可比较", False,
                      f"缺少 {new_csv if not new_csv.is_file() else ref_csv}，无法逐字节比较",
                      fatal=False)

    run.finalize()
    return run


# ---------------------------------------------------------------- 顶层 build

def analyze_build():
    checks = []
    facts = {}
    missing = []

    def check(cid, ok, detail, fatal=True):
        checks.append({"id": cid, "ok": bool(ok), "fatal": bool(fatal), "detail": detail})

    result, err = read_json_file(BUILD_RESULT)
    if result is None:
        missing.append(str(BUILD_RESULT))
        check("build-result.json 存在", False, err)
    else:
        facts["result"] = result
        check("build exit_code==0", result.get("exit_code") == 0,
              f"exit_code={result.get('exit_code')!r}")
        check("build reused is False", result.get("reused") is False,
              f"reused={result.get('reused')!r}")

    cmd, err = read_json_file(BUILD_COMMAND)
    if cmd is None:
        check("build-command.json 存在", False, err, fatal=False)
    elif not isinstance(cmd, list):
        check("build-command.json 是 argv 数组", False,
              f"type={type(cmd).__name__}", fatal=False)
    else:
        facts["argv"] = [str(x) for x in cmd]

    fatal = [c["id"] for c in checks if not c["ok"] and c["fatal"]]
    status = "missing_evidence" if missing else ("fail" if fatal else "evidence_ok")
    return {"status": status, "missing": missing, "fatal_failures": fatal,
            "checks": checks, "facts": facts}


# ---------------------------------------------------------------- 条件判断

def conditional_note(mode_name, physical_bypass, mean):
    return "普通强对照的性能比较；独立创新判定见 Codex 报告。"


# ---------------------------------------------------------------- 汇总

def main():
    runs = {}
    for mode, physical_bypass in MODES:
        for flow in FLOWS:
            run = analyze_run(mode, physical_bypass, flow)
            runs[run.name] = run

    build = analyze_build()

    comparisons, mirror = [], []
    for mode, physical_bypass in MODES:
        v_run, a_run = runs[f"{mode}-v-32768"], runs[f"{mode}-a-32768"]
        tv = v_run.facts.get("tb_cycles") or {}
        ta = a_run.facts.get("tb_cycles") or {}
        vv, va = tv.get("v"), tv.get("a")
        av, aa = ta.get("v"), ta.get("a")

        # A 先：只做镜像一致性检验，不进入均值
        entry = {"mode": mode,
                 "v_first_run": v_run.name,
                 "a_first_run": a_run.name,
                 "a_first_excluded_from_mean": True}
        if all(isinstance(x, int) for x in (vv, va, av, aa)):
            entry["mirror_ok"] = (vv == aa) and (va == av)
            entry["v_first"] = {"v": vv, "a": va}
            entry["a_first"] = {"v": av, "a": aa}
            entry["after_swap_delta"] = {"v": aa - vv, "a": av - va}
            entry["v_first_mean"] = (vv + va) / 2
            entry["a_first_mean"] = (av + aa) / 2
            entry["mean_delta"] = entry["a_first_mean"] - entry["v_first_mean"]
            entry["detail"] = ("检验 V-first(v,a) 是否等于 A-first(a,v)；"
                               "after_swap_delta=(aa-vv, av-va)；A-first 不参与均值")
        else:
            entry["mirror_ok"] = None
            entry["detail"] = "缺少可比较的 TB issue_to_done_cycles，无法做镜像检验（失败场景不消失）"
        mirror.append(entry)

        # V 先：计算 V/A 双方均值（仅 TB 域）
        item = {"mode": mode,
                "mean_source": "V-first 同次实验的 V/A 两条 TB issue→done 周期",
                "unit": "TB issue/done cycles（非 SV 事件 cycle，二者不同钟原点）"}
        if isinstance(vv, int) and isinstance(va, int):
            mean = (vv + va) / 2
            item.update({"v_cycles": vv, "a_cycles": va, "mean_cycles": mean})
            for label, ref in (("old38_best_ordinary", REF38_BEST_ORDINARY_MEAN),
                               ("old_post_window4_depth4", REF38_POST_MEAN)):
                delta = mean - ref
                item["vs_" + label] = {
                    "reference_cycles": ref,
                    "delta_cycles": delta,
                    "delta_percent": (100.0 * delta / ref) if ref else None,
                    "delta_percent_denominator": f"旧参考值 {ref}",
                }
            ref_post = REF38_POST_MEAN
            delta_post = mean - ref_post
            original_gap = REF38_BEST_ORDINARY_MEAN - REF38_POST_MEAN
            gap_reduction = original_gap - delta_post
            if "vs_old_post_window4_depth4" in item:
                item["vs_old_post_window4_depth4"].update({
                    "percent_of_mean": (100.0 * delta_post / mean) if mean else None,
                    "percent_of_mean_denominator": "本次 V-first TB 均值（与旧参考值分母不同，不可混用）",
                    "original_gap_cycles": original_gap,
                    "gap_reduction_cycles": gap_reduction,
                    "gap_reduction_percent": (100.0 * gap_reduction / original_gap) if original_gap else None,
                    "gap_reduction_percent_denominator": f"旧差距 {original_gap} cycles",
                })
            item["conditional_note"] = conditional_note(mode, physical_bypass, mean)
        else:
            item.update({"v_cycles": vv, "a_cycles": va, "mean_cycles": None,
                         "conditional_note": "缺少 V-first TB issue_to_done_cycles，无法计算均值（失败场景不消失）"})
        comparisons.append(item)

    anomalies = []
    for name, run in runs.items():
        for anom in run.anomalies:
            anomalies.append({"run": name, "id": anom["id"], "detail": anom["detail"]})
    for m in mirror:
        if m.get("mirror_ok") is False:
            vf = m.get("v_first") or {}
            af = m.get("a_first") or {}
            d = m.get("after_swap_delta") or {}
            detail = (
                f"mirror_ok=False：V-first(v,a)={vf.get('v')}/{vf.get('a')}，"
                f"A-first(v,a)={af.get('v')}/{af.get('a')}；"
                f"交换后两流逐项差 Δv={signed(d.get('v'))}、Δa={signed(d.get('a'))} cycles；"
                f"V-first mean={fmt1(m.get('v_first_mean'))}，"
                f"A-first mean={fmt1(m.get('a_first_mean'))}，"
                f"mean Δ={fmt1(m.get('mean_delta'))}；不构成镜像复制，A-first 不进入均值。")
            anomalies.append({"run": f"{m['mode']} (mirror)",
                              "id": "a_first_mirror_ok=False", "detail": detail})

    report = {
        "schema": "prepared-source-bypass-39-summary-v1",
        "script": "summarize_revised.py（离线证据汇总；不运行 DUT、不改其他文件、不重跑旧 log）",
        "root": str(ROOT),
        "out": str(OUT),
        "reference_38_dir": str(REF38),
        "build": build,
        "clock_domains": {
            "sv_event_cycles": {
                "sources": ["RAW4-*", "PREP4-*", "POSTTL-*", "DIRECT39-AR.cycle"],
                "note": "RTL 侧 SV 事件 cycle",
            },
            "tb_issue_done_cycles": {
                "sources": ["PLACEMENT-METRICS-JSON.flow.{v,a}.issue_to_done_cycles"],
                "note": "TB issue/done cycle",
            },
            "rule": "两域不同钟原点：均值与 §38/旧 post 比较只用 TB issue/done cycle，SV 事件 cycle 不参与任何跨域算术。",
        },
        "declared_only": {
            "raw4_functional_bits": RAW4_FUNCTIONAL_BITS,
            "prep4_functional_bits": PREP4_FUNCTIONAL_BITS,
            "declared_total_bits": DECLARED_TOTAL_BITS,
            "note": "3317=3316+固定 route 1；3317+1281=4598 仅为声明状态，不是综合面积或总成本。",
            "observation_stats_independent_not_in_control": True,
        },
        "external_reference_values": {
            "old38_best_ordinary_mean_cycles": REF38_BEST_ORDINARY_MEAN,
            "old_post_window4_depth4_mean_cycles": REF38_POST_MEAN,
            "note": "外部给定参考值（§38 审核报告）；本脚本不重算、不重跑旧 log，旧 log 只作字段示例。",
        },
        "verification_ownership": ("所有 held 属性与 R 路由关键断言由 Codex RTL/C++ 验证；"
                                   "本脚本只做离线证据汇总，不重跑验证、不冒称作者。"),
        "all_six_scenarios_present": len(runs) == 6,
        "scenarios": {name: run.to_json() for name, run in runs.items()},
        "v_first_comparisons": comparisons,
        "a_first_mirror_check": mirror,
        "anomalies": anomalies,
        "failed_scenarios": [
            {"run": name, "status": run.status,
             "fatal_failures": run.fatal, "missing_artifacts": run.missing}
            for name, run in runs.items() if run.status != "evidence_ok"
        ],
    }

    (OUT / "analysis.json").write_text(
        json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    (OUT / "analysis.md").write_text(render_md(report), encoding="utf-8")
    return report


def render_md(report):
    out = []
    add = out.append
    add("# §39 普通已准备源读物理旁路：离线证据汇总（脚本提案输出）")
    add("")
    add("本文件由 `summarize_revised.py` 生成，仅做离线证据汇总；不代表 RTL 验证，也不构成独立创新结论。")
    add("")
    add("- 输出目录：`%s`" % report["out"])
    add("- §38 参照目录：`%s`" % report["reference_38_dir"])
    add("- 钟域：SV 事件 cycle 与 TB issue/done cycle 不同钟原点，脚本不做跨域算术。")
    add("- 六场景全部收录：%s" % ("是" if report["all_six_scenarios_present"] else "否"))
    add("- 声明状态：RAW4 functional_bits=%d、PREP4 functional_bits=%d、合计 %d（仅声明，非综合面积/总成本）。"
        % (RAW4_FUNCTIONAL_BITS, PREP4_FUNCTIONAL_BITS, DECLARED_TOTAL_BITS))
    b = report.get("build") or {}
    br = (b.get("facts") or {}).get("result") or {}
    add("- 顶层构建：status=%s，exit_code=%s，reused=%s"
        % (b.get("status"), br.get("exit_code"), br.get("reused")))
    for check in b.get("checks", []):
        if not check["ok"]:
            add("  - 构建检查 `%s`：%s" % (check["id"], check["detail"]))
    add("")
    add("## 六个运行")
    add("")
    add("| 运行 | physical-bypass | 状态 | direct | fallback | nat var | write_wait | TL accept/resp | V-first TB V/A/mean |")
    add("| --- | --- | --- | --- | --- | --- | --- | --- | --- |")
    comp_by_mode = {c["mode"]: c for c in report["v_first_comparisons"]}
    for mode, physical_bypass in MODES:
        for flow in FLOWS:
            run = report["scenarios"][f"{mode}-{flow}-32768"]
            facts = run["facts"]
            ds = facts.get("direct39_stats") or {}
            nm = facts.get("natural") or {}
            tl = facts.get("tl_counts") or {}
            mean_txt = "-"
            if flow == "v" and comp_by_mode.get(mode, {}).get("mean_cycles") is not None:
                c = comp_by_mode[mode]
                mean_txt = "%d/%d/%.1f" % (c["v_cycles"], c["a_cycles"], c["mean_cycles"])
            add("| %s | %s | %s | %s | %s | %s | %s | %s/%s | %s |" % (
                run["name"], run["physical_bypass"], run["status"],
                ds.get("direct", "-"), ds.get("fallback", "-"), nm.get("var", "-"),
                ds.get("write_wait", "-"), tl.get("n_tl_accept", "-"), tl.get("n_tl_resp", "-"),
                mean_txt))
    add("")
    add("## V-first 比较（TB issue/done cycle，A-first 不进入均值）")
    add("")
    for c in report["v_first_comparisons"]:
        add("- **%s**：mean=%s cycles（单位：TB issue/done）" % (
            c["mode"], "N/A" if c["mean_cycles"] is None else "%.1f" % c["mean_cycles"]))
        for key in ("vs_old38_best_ordinary", "vs_old_post_window4_depth4"):
            cmp = c.get(key)
            if not cmp:
                continue
            add("  - %s：ref=%s cycles，Δ=%s cycles" % (
                key, num(cmp["reference_cycles"]), num(cmp["delta_cycles"])))
            add("    - 分母=旧参考值 %s：100*%s/%s = %r%%" % (
                num(cmp["reference_cycles"]), num(cmp["delta_cycles"]),
                num(cmp["reference_cycles"]), cmp["delta_percent"]))
            if key == "vs_old_post_window4_depth4" and cmp.get("percent_of_mean") is not None:
                add("    - 分母=本次均值 %s（与上一行分母不同，不是同一个数）：100*%s/%s = %r%%" % (
                    num(c["mean_cycles"]), num(cmp["delta_cycles"]),
                    num(c["mean_cycles"]), cmp["percent_of_mean"]))
                add("    - 旧差距 %s cycles 缩小：残差 %s cycles，100*%s/%s = %r%%（分母=旧差距）" % (
                    num(cmp["original_gap_cycles"]), num(cmp["delta_cycles"]),
                    num(cmp["gap_reduction_cycles"]), num(cmp["original_gap_cycles"]),
                    cmp["gap_reduction_percent"]))
        add("  - 条件判断：%s" % c["conditional_note"])
    add("")
    add("## A-first 镜像一致性检验（不混平均）")
    add("")
    for m in report["a_first_mirror_check"]:
        add("- %s：mirror_ok=%s" % (m["mode"], m.get("mirror_ok")))
        vf = m.get("v_first") or {}
        af = m.get("a_first") or {}
        if vf and af:
            add("  - V-first(v,a)=%s/%s；A-first(v,a)=%s/%s" % (
                vf.get("v"), vf.get("a"), af.get("v"), af.get("a")))
        d = m.get("after_swap_delta") or {}
        if d:
            add("  - 交换后两流逐项差：Δv=%s，Δa=%s cycles" % (
                signed(d.get("v")), signed(d.get("a"))))
        if m.get("v_first_mean") is not None and m.get("a_first_mean") is not None:
            add("  - 均值：V-first %.1f，A-first %.1f，mean Δ=%s" % (
                m["v_first_mean"], m["a_first_mean"], fmt1(m["mean_delta"])))
        add("  - %s" % m["detail"])
    add("")
    add("## 异常与失败证据")
    add("")
    if not report["failed_scenarios"] and not report["anomalies"]:
        add("- 无记录到的失败/异常（仍以 Codex 复核为准）。")
    for item in report["failed_scenarios"]:
        add("- 失败场景 `%s`：status=%s，致命检查=%s，缺失产物=%s" % (
            item["run"], item["status"],
            ", ".join(item["fatal_failures"]) or "无",
            ", ".join(item.get("missing_artifacts") or []) or "无"))
        run = report["scenarios"][item["run"]]
        for check in run["checks"]:
            if not check["ok"]:
                add("  - 检查 `%s`：%s" % (check["id"], check["detail"]))
        for prefix, line in run["source_lines"].items():
            add("  - 证据 `%s`: `%s`" % (prefix, line))
    for anom in report["anomalies"]:
        add("- 异常 `%s` @ `%s`：%s" % (anom["id"], anom["run"], anom["detail"]))
    add("")
    return "\n".join(out) + "\n"


if __name__ == "__main__":
    main()
