#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""P4M 形式化60（原25 + 离线追认6 + 新启动29）只读审计与描述统计。

只读输入（不修改、不重跑、不访问设备、不联网，仅标准库）：
    out/p4m-formal60-recovery-20260920/evidence/p4m-formal60-215406   原25
    out/p4m-complete-review-20261002/retained6                        离线追认6
    out/mac-p4m-remaining29-20261002/collection/unpacked              新启动29

输出（只写本审阅目录 out/p4m-complete-review-20261002/）：
    trials.csv    每 trial 一行
    trials.json   每 trial 的完整记录（含逐项校验结果）
    summary.json  审计检查 / 样本数 / 描述统计 / 配对量 / 分段 / 事实 / 警告
    RESULTS.md    中文表格报告（数字全部来自程序计算）

用法：
    python3 out/p4m-complete-review-20261002/analyze.py
    python3 out/p4m-complete-review-20261002/analyze.py --self-test
主分析始终使用 strict 计时口径，不提供放宽开关；--out-dir 只允许写到本审阅目录或其子目录。

本程序不生成也不支持这些结论：同一次启动60次在线通过、P99/WCET、最优 gap、
统计显著性/功效、把整轮 A miss 总数下降称为 miss 率下降、把 CPU victim_elapsed_ns
当作硬件窗口。
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import math
import re
import sys
from dataclasses import dataclass, field
from datetime import datetime, timezone
from pathlib import Path
from statistics import median

# ---------------------------------------------------------------------------
# 常量与目录
# ---------------------------------------------------------------------------
ROOT = Path(__file__).resolve().parents[2]
REVIEW_DIR = Path(__file__).resolve().parent

MIB = 1 << 20
CHUNK = 4096
TRANSFERS = 4096
V_BYTES = CHUNK * TRANSFERS
PLATFORM_HZ = 50_000_000
CYCLE_NS = 20
A_RATE_REL_TOL = 1e-6
A_RATE_ABS_TOL = 1e-3
GAPS = (0, 1000, 2000)
# 与 bench/idma_experiment.c 的 validate_p4m() 一致的真实 bank magic（非夹具猜值）。
P4M_BANK_MAGIC = 0x49444D4150344D01
P4M_FLAGS_KNOWN = 0x7
REQUIRED_KERNEL_RELEASE = "6.5.0-rc2-00089-gefd9461d6459"

TS_RE = re.compile(r"\[\s*(\d+\.\d+)\s*\]")
KV_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")

DIR_CONF = {
    "orig25": {
        "path": ROOT / "out" / "p4m-formal60-recovery-20260920" / "evidence" / "p4m-formal60-215406",
        "acq": "acq25_origboot",
        "power": "pre_powercycle",
        "label": "原25：b01..b08 24例在线通过 + b09-g2000 离线追认",
    },
    "retained6": {
        "path": REVIEW_DIR / "retained6",
        "acq": "acq6_retained",
        "power": "pre_powercycle",
        "label": "离线追认6：b09-g0/g1000、b10 三例、b11-g2000（resume35 前段）",
    },
    "remaining29": {
        "path": ROOT / "out" / "mac-p4m-remaining29-20261002" / "collection" / "unpacked",
        "acq": "acq29_newboot",
        "power": "post_powercycle_newboot",
        "label": "新启动补缺29：b11-g1000/g0 起至 b20-g0",
    },
}
AUDIT_ALIAS_DIR = ROOT / "out" / "mac-p4m-remaining29-20261002" / "audit"
ACQ_ORDER = ("acq25_origboot", "acq6_retained", "acq29_newboot")
POWER_ORDER = ("pre_powercycle", "post_powercycle_newboot")
OFFLINE_IN_ACQ25 = {"formal-b09-g2000"}


def _expected_layout():
    """(block_no, gap) -> dir_key。这是冻结矩阵与交接口径的声明式表达。"""
    layout = {}
    for b in range(1, 9):
        for g in GAPS:
            layout[(b, g)] = "orig25"
    layout[(9, 2000)] = "orig25"
    layout[(9, 0)] = "retained6"
    layout[(9, 1000)] = "retained6"
    for g in GAPS:
        layout[(10, g)] = "retained6"
    layout[(11, 2000)] = "retained6"
    layout[(11, 0)] = "remaining29"
    layout[(11, 1000)] = "remaining29"
    for b in range(12, 21):
        for g in GAPS:
            layout[(b, g)] = "remaining29"
    return layout


EXPECTED_LAYOUT = _expected_layout()


def expected_manifest():
    expected = {}
    for (block_no, gap), dir_key in sorted(EXPECTED_LAYOUT.items()):
        expected.setdefault(dir_key, []).append("formal-b%02d-g%d.json" % (block_no, gap))
    return expected


def manifest_check(actual_by_dir):
    """纯函数：比较三目录 formal-*.json 实际清单与冻结矩阵；不需要真实输入即可自检。"""
    expected = expected_manifest()
    errors = []
    seen = {}
    for dir_key in sorted(DIR_CONF):
        names = actual_by_dir.get(dir_key)
        want = set(expected.get(dir_key, []))
        if names is None:
            errors.append({"check": "manifest.dir_missing",
                           "detail": "目录 %s 不存在或未提供清单" % DIR_CONF[dir_key]["path"]})
            continue
        have = set()
        for name in names:
            if name in have:
                errors.append({"check": "manifest.duplicate_in_dir",
                               "detail": "%s 在 %s 内重复" % (name, dir_key)})
            have.add(name)
            if name in seen:
                errors.append({"check": "manifest.duplicate_case",
                               "detail": "%s 同时在 %s 与 %s" % (name, seen[name], dir_key)})
            else:
                seen[name] = dir_key
            if name in want:
                continue
            elsewhere = sorted(k for k, v in expected.items() if name in v)
            if elsewhere:
                errors.append({"check": "manifest.misplaced_case",
                               "detail": "%s 在 %s，期望 %s" % (name, dir_key, ",".join(elsewhere))})
            else:
                errors.append({"check": "manifest.unexpected_case",
                               "detail": "%s 不在冻结矩阵中" % name})
        for name in sorted(want - have):
            errors.append({"check": "manifest.missing_case",
                           "detail": "%s 应从 %s 缺失" % (name, dir_key)})
    return (not errors), errors


def _params_kv(lines):
    kv = {}
    for line in lines or []:
        if "=" in line:
            key, _, value = line.partition("=")
            kv[key.strip()] = value.strip()
    return kv


def validate_segment_metadata(metadata):
    required = []
    missing = []
    for key, entry in sorted(metadata.items()):
        params = entry.get("params.txt")
        if not params:
            required.append({"check": "metadata.params_present_%s" % key,
                             "detail": "params.txt 缺失，未读取"})
            missing.append({"case": None, "item": "metadata_%s_params.txt" % key,
                            "detail": "文件缺失，未读取"})
            continue
        kv = _params_kv(params)
        release = kv.get("kernel_release", "")
        uname = kv.get("uname", "")
        if "00089" not in release or "00089" not in uname:
            required.append({"check": "metadata.kernel_00089_%s" % key,
                             "detail": "kernel_release=%r uname=%r 未含 00089" % (release, uname)})
        if release != REQUIRED_KERNEL_RELEASE:
            required.append({"check": "metadata.kernel_exact_%s" % key,
                             "detail": "kernel_release=%r 期望 %s" % (release, REQUIRED_KERNEL_RELEASE)})
        if kv.get("taint_baseline") != "0":
            required.append({"check": "metadata.taint_baseline_%s" % key,
                             "detail": "taint_baseline=%r" % kv.get("taint_baseline")})
        if DIR_CONF[key]["power"] == "post_powercycle_newboot":
            if kv.get("new_boot_segment") != "1":
                required.append({"check": "metadata.postboot_new_boot_segment_%s" % key,
                                 "detail": "new_boot_segment=%r" % kv.get("new_boot_segment")})
        elif kv.get("new_boot_segment") == "1":
            required.append({"check": "metadata.preboot_new_boot_segment_%s" % key,
                             "detail": "pre_powercycle 段不应标记新启动"})
        taint = entry.get("taint.txt")
        if taint is None:
            required.append({"check": "metadata.taint_present_%s" % key,
                             "detail": "taint.txt 缺失，未读取"})
            missing.append({"case": None, "item": "metadata_%s_taint.txt" % key,
                            "detail": "文件缺失，未读取"})
        elif taint != ["0"]:
            required.append({"check": "handover.taint_%s" % key,
                             "detail": "taint=%r，期望 0" % taint})
    return required, missing

STAT_METRICS = (
    ("v_cycles", "V窗口cycles"),
    ("hardware_window_ns", "硬件窗口ns"),
    ("v_mib_s", "V速率MiB/s"),
    ("a_window_done_jobs", "A窗口完成作业"),
    ("a_window_bytes", "A窗口字节"),
    ("a_window_mib_s", "A窗口速率MiB/s"),
    ("a_flowboundary_jobs", "边界携入作业"),
    ("a_flowboundary_bytes", "边界携入字节"),
    ("a_issue_window", "A开窗提交"),
    ("a_done_window", "A窗口完成"),
    ("a_opening_inflight_id", "开窗在途ID"),
    ("p4m_flags", "P4M flags"),
    ("v_whole_miss", "V整轮iotlb_miss"),
    ("v_whole_s1", "V整轮s1_ptw"),
    ("v_whole_s2", "V整轮s2_ptw"),
    ("a_whole_miss", "A整轮iotlb_miss"),
    ("a_whole_s1", "A整轮s1_ptw"),
    ("a_whole_s2", "A整轮s2_ptw"),
    ("a_whole_completed", "A整轮完成数"),
    ("irq_span_ns", "启动irq_span_ns"),
    ("a_to_v_ns", "启动a_to_v_ns"),
    ("done_to_read", "batch_time done_to_read cycles"),
    ("done_to_read_ns", "batch_time done_to_read ns"),
)

PAIR_QUANTITIES = (
    ("v_rate_ratio", "V速率比"),
    ("v_rate_improvement_pct", "V吞吐提升%"),
    ("v_time_decrease_pct", "V时间下降%"),
    ("a_rate_ratio", "A速率比"),
    ("a_rate_loss_pct", "A同窗速率损失%"),
)

CSV_COLUMNS = (
    ("case_id", "case_id"),
    ("block_id", "block_id"),
    ("gap_cycles", "gap_cycles"),
    ("dir_key", "dir_key"),
    ("acquisition_segment", "acquisition_segment"),
    ("power_segment", "power_segment"),
    ("audit_identity", "audit_identity"),
    ("seq_dmesg", "seq_dmesg_not_block"),
    ("startup_ts_s", "startup_dmesg_ts_s"),
    ("a_begin_ns", "startup_a_begin_ns"),
    ("a_to_v_ns", "startup_a_to_v_ns"),
    ("irq_span_ns", "startup_irq_span_ns"),
    ("v_cycles", "v_cycles"),
    ("hardware_window_ns", "hardware_window_ns"),
    ("v_bytes", "v_bytes"),
    ("v_mib_s", "v_mib_s"),
    ("a_window_done_jobs", "a_window_done"),
    ("a_window_bytes", "a_window_bytes"),
    ("a_window_mib_s", "a_window_mib_s"),
    ("a_flowboundary_jobs", "a_flowboundary_jobs"),
    ("a_flowboundary_bytes", "a_flowboundary_bytes"),
    ("a_issue_window", "a_issue_window"),
    ("a_done_window", "a_done_window"),
    ("a_inflight_at_issue", "a_inflight_at_issue"),
    ("a_inflight_at_done", "a_inflight_at_done"),
    ("a_opening_inflight_id", "a_opening_inflight_id"),
    ("a_closing_inflight_id", "a_closing_inflight_id"),
    ("p4m_flags", "p4m_flags"),
    ("has_a", "p4m_has_a"),
    ("closed", "p4m_closed"),
    ("conservation", "p4m_conservation"),
    ("strict_preopening_inflight", "p4m_strict_preopening_inflight"),
    ("v_whole_submitted", "v_whole_submitted"),
    ("v_whole_completed", "v_whole_completed"),
    ("v_whole_miss", "v_whole_iotlb_miss"),
    ("v_whole_s1", "v_whole_s1_ptw"),
    ("v_whole_s2", "v_whole_s2_ptw"),
    ("v_whole_active_cycles", "v_whole_active_cycles"),
    ("a_whole_submitted", "a_whole_submitted"),
    ("a_whole_completed", "a_whole_completed"),
    ("a_whole_miss", "a_whole_iotlb_miss"),
    ("a_whole_s1", "a_whole_s1_ptw"),
    ("a_whole_s2", "a_whole_s2_ptw"),
    ("a_whole_active_cycles", "a_whole_active_cycles"),
    ("cpu_victim_elapsed_ns", "cpu_victim_elapsed_ns_NOT_hw_window"),
    ("hpm_snapshot_before_ns", "hpm_snapshot_before_ns"),
    ("hpm_snapshot_after_ns", "hpm_snapshot_after_ns"),
    ("drain_ns", "drain_ns"),
    ("done_to_read", "batch_time_done_to_read_cycles"),
    ("done_to_read_ns", "batch_time_done_to_read_ns"),
    ("json_a_rate_bps", "json_a_completed_bytes_per_second"),
    ("recomputed_a_rate_bps", "recomputed_a_window_bytes_per_second"),
    ("a_rate_abs_diff", "a_rate_abs_diff"),
    ("a_rate_rel_diff", "a_rate_rel_diff"),
    ("timing_source", "timing_source"),
    ("json_path", "json_path"),
    ("startup_path", "startup_path"),
    ("batchtime_path", "batchtime_path"),
    ("required_failures", "required_failure_count"),
    ("advisory_warnings", "advisory_warning_count"),
)


# ---------------------------------------------------------------------------
# 通用小工具
# ---------------------------------------------------------------------------
def read_text(path):
    return path.read_text(encoding="utf-8", errors="replace")


def as_int(value):
    if isinstance(value, bool):
        return None
    if isinstance(value, int):
        return value
    if isinstance(value, float):
        return int(value) if float(value).is_integer() else None
    if isinstance(value, str):
        text = value.strip()
        for base in (0, 10):
            try:
                return int(text, base)
            except ValueError:
                continue
    return None


def is_number(value):
    return isinstance(value, (int, float)) and not isinstance(value, bool)


def kernel_ts(line):
    match = TS_RE.search(line)
    if not match:
        return None
    try:
        return float(match.group(1))
    except ValueError:
        return None


def parse_kv(line):
    out = {}
    for token in line.split():
        if "=" not in token:
            continue
        key, _, value = token.partition("=")
        if KV_RE.match(key):
            out[key] = value
    return out


def sha256_file(path):
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for block in iter(lambda: handle.read(65536), b""):
            digest.update(block)
    return digest.hexdigest()


def hash_files(paths):
    out = {}
    for path in sorted({str(p) for p in paths}):
        p = Path(path)
        if not p.is_file():
            out[path] = {"present": False, "sha256": None, "bytes": None}
            continue
        out[path] = {"present": True, "sha256": sha256_file(p), "bytes": p.stat().st_size}
    return out


def _quantile_type7(sorted_values, q):
    n = len(sorted_values)
    if n == 0:
        return None
    if n == 1:
        return float(sorted_values[0])
    pos = (n - 1) * q
    lo = int(math.floor(pos))
    hi = int(math.ceil(pos))
    if lo == hi:
        return float(sorted_values[lo])
    frac = pos - lo
    return float(sorted_values[lo]) * (1.0 - frac) + float(sorted_values[hi]) * frac


def mm(values, with_iqr=True):
    vals = sorted(v for v in values if is_number(v))
    n_total = len(list(values))
    if not vals:
        return {"n": 0, "n_missing": n_total, "min": None, "median": None, "max": None,
                "q1": None, "q3": None, "iqr": None}
    out = {"n": len(vals), "n_missing": n_total - len(vals), "min": vals[0],
           "median": median(vals), "max": vals[-1], "q1": None, "q3": None, "iqr": None}
    if with_iqr and len(vals) >= 2:
        q1 = _quantile_type7(vals, 0.25)
        q3 = _quantile_type7(vals, 0.75)
        out["q1"] = q1
        out["q3"] = q3
        out["iqr"] = q3 - q1
    return out


def _hist_key(value):
    try:
        return (0, float(value))
    except (TypeError, ValueError):
        return (1, str(value))


def hist_and_offmodal(trials, key):
    buckets = {}
    for t in trials:
        buckets.setdefault(str(t.derived.get(key)), []).append(t.case_id)
    items = sorted(buckets.items(), key=lambda kv: _hist_key(kv[0]))
    modal = None
    if items:
        modal = sorted(buckets.items(), key=lambda kv: (-len(kv[1]), _hist_key(kv[0])))[0][0]
    off = sorted(c for k, v in buckets.items() if k != modal for c in v)
    return {"n": sum(len(v) for v in buckets.values()),
            "histogram": {k: len(v) for k, v in items},
            "modal": modal,
            "off_modal_cases": off}


# ---------------------------------------------------------------------------
# 数据模型
# ---------------------------------------------------------------------------
@dataclass
class Trial:
    case_id: str
    block_no: int
    block: str
    gap: int
    acq: str
    power: str
    audit: str
    dir_key: str
    json_path: Path | None = None
    json_obj: dict = field(default_factory=dict)
    startup_path: Path | None = None
    startup_text: str | None = None
    startup_kv: dict | None = None
    batchtime_path: Path | None = None
    batchtime_text: str | None = None
    batchtime_kv: dict | None = None
    timing_source: str = "missing"
    missing_notes: list = field(default_factory=list)
    derived: dict = field(default_factory=dict)
    required_errors: list = field(default_factory=list)
    advisory_warnings: list = field(default_factory=list)
    missing_items: list = field(default_factory=list)


class CaseCheck:
    __slots__ = ("required", "advisory", "missing")

    def __init__(self):
        self.required = []
        self.advisory = []
        self.missing = []

    def req(self, ok, name, detail=""):
        if not ok:
            self.required.append({"check": name, "detail": str(detail)})

    def adv(self, ok, name, detail=""):
        if not ok:
            self.advisory.append({"check": name, "detail": str(detail)})

    def miss(self, name, detail=""):
        self.missing.append({"item": name, "detail": str(detail)})


def audit_identity(acq, case_id):
    if acq == "acq6_retained":
        return "offline_retained"
    if acq == "acq25_origboot" and case_id in OFFLINE_IN_ACQ25:
        return "offline_retained"
    return "online_pass"


# ---------------------------------------------------------------------------
# 读取与计时记录解析
# ---------------------------------------------------------------------------
def extract_record_line(text, kind, seq):
    marker = "idma_batch_start" if kind == "startup" else "idma_batch_obs"
    hits = []
    for line in text.splitlines():
        if marker not in line:
            continue
        kv = parse_kv(line)
        if kind == "batch_time" and kv.get("part") != "time":
            continue
        if kv.get("seq") != str(seq):
            continue
        hits.append(line)
    if len(hits) == 1:
        return hits[0], None
    return None, "按 seq=%s 匹配到 %d 条记录（要求恰好 1 条）" % (seq, len(hits))


def parse_record_kv(text, kind):
    if not text:
        return None
    for line in text.splitlines():
        if kind == "startup" and "idma_batch_start" in line:
            kv = parse_kv(line)
            kv["_ts_s"] = kernel_ts(line)
            return kv
        if kind == "batch_time" and "idma_batch_obs" in line:
            kv = parse_kv(line)
            if kv.get("part") == "time":
                kv["_ts_s"] = kernel_ts(line)
                return kv
    return None


def resolve_record(case_id, block_no, gap, dirpath, suffix, kind, seq):
    """返回 (text, source_label, [files_read], missing_note)。

    顺序：同目录派生文件 -> 同目录 dmesg.raw 按唯一 seq 提取 -> audit 别名文件。
    """
    direct = dirpath / (case_id + suffix)
    if direct.is_file():
        return read_text(direct), "derived_file", [direct], None

    dmesg_note = None
    for raw_name in (case_id + ".dmesg.raw", case_id + ".dmesg"):
        raw = dirpath / raw_name
        if not raw.is_file():
            continue
        if seq is None:
            dmesg_note = "%s 存在，但 JSON 缺 sequence，无法按 seq 提取" % raw.name
            break
        line, err = extract_record_line(read_text(raw), kind, seq)
        if line is not None:
            return line + "\n", "dmesg_raw_extract", [raw], None
        dmesg_note = "%s 提取失败：%s" % (raw.name, err)
        break

    alias_stem = "b%02dg%d" % (block_no, gap)
    alias = AUDIT_ALIAS_DIR / (alias_stem + suffix)
    if alias.is_file():
        return read_text(alias), "audit_alias", [alias], None

    note = "缺少 %s%s，且无 dmesg.raw 可提取、无 audit 别名（%s%s）" % (
        case_id, suffix, alias_stem, suffix)
    if dmesg_note:
        note += "；" + dmesg_note
    return None, "missing", [], note


def read_segment_metadata(files_read):
    meta = {}
    for key, conf in DIR_CONF.items():
        entry = {}
        for name in ("params.txt", "matrix.txt", "input.txt", "taint.txt"):
            path = conf["path"] / name
            if not path.is_file():
                entry[name] = None
                continue
            files_read.append(path)
            entry[name] = read_text(path).strip().splitlines()
        meta[key] = entry
    return meta


# ---------------------------------------------------------------------------
# 派生指标
# ---------------------------------------------------------------------------
def derive_metrics(t):
    j = t.json_obj if isinstance(t.json_obj, dict) else {}
    p4m = j.get("p4m") if isinstance(j.get("p4m"), dict) else {}
    res = j.get("result") if isinstance(j.get("result"), dict) else {}
    vic = res.get("victim") if isinstance(res.get("victim"), dict) else {}
    agg = res.get("aggressor") if isinstance(res.get("aggressor"), dict) else {}
    skv = t.startup_kv if isinstance(t.startup_kv, dict) else {}

    d = {}
    d["seq"] = as_int(p4m.get("sequence"))
    d["startup_ts_s"] = skv.get("_ts_s")
    d["a_begin_ns"] = as_int(skv.get("a_begin_ns"))
    d["a_to_v_ns"] = as_int(skv.get("a_to_v_ns"))
    d["irq_span_ns"] = as_int(skv.get("irq_span_ns"))
    bkv = t.batchtime_kv if isinstance(t.batchtime_kv, dict) else {}
    d["done_to_read"] = as_int(bkv.get("done_to_read"))
    d["done_to_read_ns"] = (d["done_to_read"] * CYCLE_NS) if d["done_to_read"] is not None else None

    cycles = as_int(p4m.get("cycles"))
    hw = as_int(p4m.get("hardware_window_ns"))
    d["v_cycles"] = cycles
    d["hardware_window_ns"] = hw
    d["v_bytes"] = as_int(vic.get("completed_bytes"))
    d["v_mib_s"] = (V_BYTES / (hw / 1e9) / MIB) if (cycles and hw) else None

    jobs = as_int(p4m.get("a_completed_jobs_window"))
    abytes = as_int(p4m.get("a_progress_window_bytes"))
    d["a_window_done_jobs"] = jobs
    d["a_window_bytes"] = abytes
    d["a_window_mib_s"] = (abytes / (hw / 1e9) / MIB) if (abytes is not None and hw) else None
    d["a_flowboundary_jobs"] = as_int(p4m.get("a_started_before_window_completed"))
    d["a_flowboundary_bytes"] = as_int(p4m.get("a_started_before_window_bytes"))
    d["a_issue_window"] = as_int(p4m.get("a_issue_window"))
    d["a_done_window"] = as_int(p4m.get("a_done_window"))
    d["a_inflight_at_issue"] = as_int(p4m.get("a_inflight_at_issue"))
    d["a_inflight_at_done"] = as_int(p4m.get("a_inflight_at_done"))
    d["a_opening_inflight_id"] = as_int(p4m.get("a_opening_inflight_id"))
    d["a_closing_inflight_id"] = as_int(p4m.get("a_closing_inflight_id"))
    d["a_first_done_id"] = as_int(p4m.get("a_first_done_id"))
    d["a_last_done_id"] = as_int(p4m.get("a_last_done_id"))
    d["p4m_flags"] = as_int(p4m.get("flags"))
    d["has_a"] = p4m.get("has_a")
    d["closed"] = p4m.get("closed")
    d["conservation"] = p4m.get("conservation")
    d["strict_preopening_inflight"] = p4m.get("strict_preopening_inflight")
    d["bank_magic"] = as_int(p4m.get("bank_magic"))

    d["v_whole_submitted"] = as_int(vic.get("submitted"))
    d["v_whole_completed"] = as_int(vic.get("completed"))
    d["v_whole_miss"] = as_int(vic.get("iotlb_miss"))
    d["v_whole_s1"] = as_int(vic.get("s1_ptw"))
    d["v_whole_s2"] = as_int(vic.get("s2_ptw"))
    d["v_whole_active_cycles"] = as_int(vic.get("active_cycles"))
    d["a_whole_submitted"] = as_int(agg.get("submitted"))
    d["a_whole_completed"] = as_int(agg.get("completed"))
    d["a_whole_miss"] = as_int(agg.get("iotlb_miss"))
    d["a_whole_s1"] = as_int(agg.get("s1_ptw"))
    d["a_whole_s2"] = as_int(agg.get("s2_ptw"))
    d["a_whole_active_cycles"] = as_int(agg.get("active_cycles"))

    d["cpu_victim_elapsed_ns"] = as_int(res.get("victim_elapsed_ns"))
    d["hpm_snapshot_before_ns"] = as_int(res.get("snapshot_before_ns"))
    d["hpm_snapshot_after_ns"] = as_int(res.get("snapshot_after_ns"))
    d["drain_ns"] = as_int(res.get("drain_ns"))

    d["json_a_rate_bps"] = p4m.get("a_completed_bytes_per_second")
    if abytes is not None and hw:
        rec = abytes / (hw / 1e9)
        d["recomputed_a_rate_bps"] = rec
        try:
            got = float(p4m.get("a_completed_bytes_per_second"))
            d["a_rate_abs_diff"] = abs(got - rec)
            d["a_rate_rel_diff"] = abs(got - rec) / rec if rec else None
        except (TypeError, ValueError):
            d["a_rate_abs_diff"] = None
            d["a_rate_rel_diff"] = None
    else:
        d["recomputed_a_rate_bps"] = None
        d["a_rate_abs_diff"] = None
        d["a_rate_rel_diff"] = None

    d["json_a_jobs_rate"] = p4m.get("a_completed_jobs_per_second")
    d["recomputed_a_jobs_rate"] = (jobs / (hw / 1e9)) if (jobs is not None and hw) else None

    t.derived = d
    return d


# ---------------------------------------------------------------------------
# 逐例校验
# ---------------------------------------------------------------------------
def _argv_ok(argv, gap):
    if not isinstance(argv, list) or not argv:
        return False, "argv 缺失或类型错误"
    expected = [("--device", "/dev/idma0"), ("--victim-pages", "1"), ("--chunk", "4096"),
                ("--transfers", "4096"), ("--warmup", "256"), ("--timeout-ms", "60000"),
                ("--aggressor", "1"), ("--aggressor-pages", "8"), ("--a-gap-cycles", str(gap))]
    problems = []
    for flag, want in expected:
        try:
            idx = argv.index(flag)
        except ValueError:
            problems.append("%s 缺失" % flag)
            continue
        if idx + 1 >= len(argv) or str(argv[idx + 1]) != want:
            got = argv[idx + 1] if idx + 1 < len(argv) else "<无>"
            problems.append("%s=%s 期望 %s" % (flag, got, want))
    if "--p4m" not in argv:
        problems.append("--p4m 缺失")
    return (not problems), "; ".join(problems)


def _close(got, want, rel_tol=A_RATE_REL_TOL, abs_tol=A_RATE_ABS_TOL):
    if got is None or want is None:
        return False, "got=%r want=%r" % (got, want)
    try:
        g = float(got)
        w = float(want)
    except (TypeError, ValueError):
        return False, "got=%r want=%r" % (got, want)
    diff = abs(g - w)
    tol = max(abs_tol, rel_tol * abs(w))
    return diff <= tol, "got=%.9g want=%.9g absdiff=%.3g tol=%.3g" % (g, w, diff, tol)


def validate_case(t, check, strict_timing=True):
    j = t.json_obj if isinstance(t.json_obj, dict) else {}
    if not j:
        check.req(False, "json.readable", "JSON 缺失或不可解析：%s" % (t.json_path or "<无路径>"))
        return
    p4m = j.get("p4m") if isinstance(j.get("p4m"), dict) else {}
    res = j.get("result") if isinstance(j.get("result"), dict) else {}
    inp = j.get("input") if isinstance(j.get("input"), dict) else {}
    vic = res.get("victim") if isinstance(res.get("victim"), dict) else {}
    agg = res.get("aggressor") if isinstance(res.get("aggressor"), dict) else {}

    check.req(j.get("schema") == "idma.p4m.v1", "json.schema", "got=%r" % (j.get("schema"),))
    check.req(j.get("success") is True, "json.success", "got=%r" % (j.get("success"),))
    check.req(j.get("error") is None, "json.error_null", "got=%r" % (j.get("error"),))
    check.req(as_int(j.get("errno")) == 0, "json.errno_0", "got=%r" % (j.get("errno"),))
    check.req(j.get("stage") == "run", "json.stage_run", "got=%r" % (j.get("stage"),))
    check.req(j.get("run_attempted") is True, "json.run_attempted", "got=%r" % (j.get("run_attempted"),))

    check.req(inp.get("caps_only") is False, "input.caps_only_false", "got=%r" % (inp.get("caps_only"),))
    check.req(inp.get("device") == "/dev/idma0", "input.device", "got=%r" % (inp.get("device"),))
    check.req(as_int(inp.get("victim_pages")) == 1, "input.victim_pages", "got=%r" % (inp.get("victim_pages"),))
    check.req(as_int(inp.get("aggressor_id")) == 1, "input.aggressor_id", "got=%r" % (inp.get("aggressor_id"),))
    check.req(as_int(inp.get("aggressor_pages")) == 8, "input.aggressor_pages", "got=%r" % (inp.get("aggressor_pages"),))
    check.req(as_int(inp.get("chunk_bytes")) == CHUNK, "input.chunk_4096", "got=%r" % (inp.get("chunk_bytes"),))
    check.req(as_int(inp.get("victim_transfers")) == TRANSFERS, "input.transfers_4096", "got=%r" % (inp.get("victim_transfers"),))
    check.req(as_int(inp.get("warmup_transfers")) == 256, "input.warmup_256", "got=%r" % (inp.get("warmup_transfers"),))
    check.req(as_int(inp.get("run_timeout_ms")) == 60000, "input.timeout_60000", "got=%r" % (inp.get("run_timeout_ms"),))
    check.req(as_int(inp.get("a_gap_cycles")) == t.gap, "input.a_gap_cycles", "got=%r want=%d" % (inp.get("a_gap_cycles"), t.gap))
    check.req(as_int(inp.get("flags")) == 0, "input.flags_0", "got=%r" % (inp.get("flags"),))

    argv_ok, argv_detail = _argv_ok(j.get("argv"), t.gap)
    check.req(argv_ok, "json.argv_matches_condition", argv_detail)

    check.req(as_int(res.get("status")) == 0, "result.status_0", "got=%r" % (res.get("status"),))
    check.req(as_int(res.get("hpm_configured")) == 1, "result.hpm_configured_1", "got=%r" % (res.get("hpm_configured"),))
    check.req(as_int(res.get("measurement_started")) == 1, "result.measurement_started_1", "got=%r" % (res.get("measurement_started"),))
    check.req(as_int(res.get("measurement_complete")) == 1, "result.measurement_complete_1", "got=%r" % (res.get("measurement_complete"),))
    check.req(as_int(res.get("victim_did")) == 10, "result.victim_did_10", "got=%r" % (res.get("victim_did"),))
    check.req(as_int(res.get("aggressor_did")) == 11, "result.aggressor_did_11", "got=%r" % (res.get("aggressor_did"),))

    check.req(as_int(vic.get("submitted")) == TRANSFERS, "result.victim.submitted_4096", "got=%r" % (vic.get("submitted"),))
    check.req(as_int(vic.get("completed")) == TRANSFERS, "result.victim.completed_4096", "got=%r" % (vic.get("completed"),))
    check.req(as_int(vic.get("completed_bytes")) == V_BYTES, "result.victim.completed_bytes_16MiB", "got=%r" % (vic.get("completed_bytes"),))
    check.req(as_int(vic.get("data_errors")) == 0, "result.victim.data_errors_0", "got=%r" % (vic.get("data_errors"),))
    check.req(as_int(vic.get("quarantined")) == 0, "result.victim.quarantined_0", "got=%r" % (vic.get("quarantined"),))
    for key in ("iotlb_miss", "s1_ptw", "s2_ptw"):
        value = as_int(vic.get(key))
        check.req(value is not None and value >= 0, "result.victim.%s_nonneg_int" % key,
                  "got=%r" % (vic.get(key),))

    a_sub = as_int(agg.get("submitted"))
    a_comp = as_int(agg.get("completed"))
    check.req(a_sub is not None and a_comp is not None and a_sub == a_comp and a_sub > 0,
              "result.aggressor.submitted_eq_completed", "submitted=%r completed=%r" % (agg.get("submitted"), agg.get("completed")))
    check.req(a_sub is not None and as_int(agg.get("completed_bytes")) == a_sub * CHUNK,
              "result.aggressor.completed_bytes_eq_submitted_x_chunk", "got=%r" % (agg.get("completed_bytes"),))
    check.req(as_int(agg.get("data_errors")) == 0, "result.aggressor.data_errors_0", "got=%r" % (agg.get("data_errors"),))
    check.req(as_int(agg.get("quarantined")) == 0, "result.aggressor.quarantined_0", "got=%r" % (agg.get("quarantined"),))
    for key in ("iotlb_miss", "s1_ptw", "s2_ptw"):
        value = as_int(agg.get(key))
        check.req(value is not None and value >= 0, "result.aggressor.%s_nonneg_int" % key,
                  "got=%r" % (agg.get(key),))

    check.req(as_int(p4m.get("requested_gap_cycles")) == t.gap, "p4m.requested_gap_cycles",
              "got=%r want=%d" % (p4m.get("requested_gap_cycles"), t.gap))
    seq = as_int(p4m.get("sequence"))
    check.req(seq is not None and seq > 0, "p4m.sequence_positive", "got=%r" % (p4m.get("sequence"),))
    check.req(as_int(p4m.get("observer_status")) == 4, "p4m.observer_status_4", "got=%r" % (p4m.get("observer_status"),))
    check.req(as_int(p4m.get("v_issued")) == TRANSFERS, "p4m.v_issued_4096", "got=%r" % (p4m.get("v_issued"),))
    check.req(as_int(p4m.get("v_done")) == TRANSFERS, "p4m.v_done_4096", "got=%r" % (p4m.get("v_done"),))
    check.req(p4m.get("has_a") is True, "p4m.has_a_true", "got=%r" % (p4m.get("has_a"),))
    check.req(p4m.get("closed") is True, "p4m.closed_true", "got=%r" % (p4m.get("closed"),))
    check.req(p4m.get("conservation") is True, "p4m.conservation_true", "got=%r" % (p4m.get("conservation"),))
    check.req(isinstance(p4m.get("strict_preopening_inflight"), bool), "p4m.strict_preopening_inflight_bool",
              "got=%r" % (p4m.get("strict_preopening_inflight"),))
    check.req(as_int(p4m.get("bank_magic")) == P4M_BANK_MAGIC, "p4m.bank_magic_bench_constant",
              "got=%r want=%r" % (p4m.get("bank_magic"), P4M_BANK_MAGIC))
    flags = as_int(p4m.get("flags"))
    check.req(flags is not None and (flags & ~P4M_FLAGS_KNOWN) == 0, "p4m.flags_known_bits",
              "got=%r" % (p4m.get("flags"),))
    start_stock = as_int(p4m.get("a_inflight_at_issue"))
    end_stock = as_int(p4m.get("a_inflight_at_done"))
    issue = as_int(p4m.get("a_issue_window"))
    done = as_int(p4m.get("a_done_window"))
    carry = as_int(p4m.get("a_started_before_window_completed"))
    carry_bytes = as_int(p4m.get("a_started_before_window_bytes"))
    opening = as_int(p4m.get("a_issue_opening_edge"))
    jobs = as_int(p4m.get("a_completed_jobs_window"))
    for name, value in (("a_inflight_at_issue", start_stock), ("a_inflight_at_done", end_stock),
                        ("a_issue_opening_edge", opening), ("a_started_before_window_completed", carry)):
        check.req(value is not None and value in (0, 1), "p4m.%s_in_0_1" % name,
                  "got=%r" % (p4m.get(name),))
    flow_inputs = (start_stock, end_stock, issue, done)
    recomputed = None
    if all(v is not None for v in flow_inputs):
        recomputed = (start_stock + issue == done + end_stock)
    check.req(recomputed is not None, "p4m.flow_conservation_inputs",
              "start=%r end=%r issue=%r done=%r" % (p4m.get("a_inflight_at_issue"),
                                                    p4m.get("a_inflight_at_done"),
                                                    p4m.get("a_issue_window"), p4m.get("a_done_window")))
    check.req(isinstance(p4m.get("conservation"), bool) and recomputed is not None
              and p4m.get("conservation") == recomputed, "p4m.flow_conservation_recomputed",
              "json=%r recomputed=%r" % (p4m.get("conservation"), recomputed))
    check.req(jobs is not None and done is not None and jobs == done, "p4m.jobs_eq_done_window",
              "jobs=%r done=%r" % (p4m.get("a_completed_jobs_window"), p4m.get("a_done_window")))
    check.req(carry is not None and carry_bytes is not None and carry_bytes == carry * CHUNK,
              "p4m.carry_bytes_eq_carry_x_chunk",
              "carry=%r bytes=%r" % (carry, p4m.get("a_started_before_window_bytes")))
    strict_flag = p4m.get("strict_preopening_inflight")
    check.req(isinstance(strict_flag, bool) and start_stock is not None and opening is not None
              and strict_flag == bool(start_stock and not opening),
              "p4m.strict_eq_start_and_not_opening_edge",
              "strict=%r start=%r opening=%r" % (strict_flag, start_stock, opening))
    check.req(not (opening and not start_stock), "p4m.opening_edge_requires_start_stock",
              "opening=%r start=%r" % (opening, start_stock))
    check.req(carry is not None and done is not None and isinstance(strict_flag, bool)
              and carry == (1 if (strict_flag and done) else 0),
              "p4m.carry_matches_strict_cohort",
              "carry=%r strict=%r done=%r" % (carry, strict_flag, done))
    if flags is not None:
        check.req(isinstance(p4m.get("has_a"), bool) and p4m.get("has_a") == bool(flags & 0x1),
                  "p4m.has_a_matches_flags", "has_a=%r flags=%r" % (p4m.get("has_a"), flags))
        check.req(isinstance(p4m.get("closed"), bool) and p4m.get("closed") == bool(flags & 0x2),
                  "p4m.closed_matches_flags", "closed=%r flags=%r" % (p4m.get("closed"), flags))
        check.req(isinstance(strict_flag, bool) and strict_flag == bool(flags & 0x4),
                  "p4m.strict_matches_flags", "strict=%r flags=%r" % (strict_flag, flags))
    check.req(as_int(p4m.get("task_error_mask")) == 0, "p4m.task_error_mask_0", "got=%r" % (p4m.get("task_error_mask"),))
    check.req(as_int(p4m.get("platform_hz")) == PLATFORM_HZ, "p4m.platform_hz_50MHz", "got=%r" % (p4m.get("platform_hz"),))
    check.req(as_int(p4m.get("cycle_ns")) == CYCLE_NS, "p4m.cycle_ns_20", "got=%r" % (p4m.get("cycle_ns"),))

    cycles = as_int(p4m.get("cycles"))
    hw = as_int(p4m.get("hardware_window_ns"))
    check.req(cycles is not None and cycles > 0, "p4m.cycles_positive", "got=%r" % (p4m.get("cycles"),))
    check.req(cycles is not None and hw is not None and hw == cycles * CYCLE_NS,
              "p4m.hardware_window_ns_eq_cycles_x_20", "cycles=%r window=%r" % (cycles, hw))

    jobs = as_int(p4m.get("a_completed_jobs_window"))
    abytes = as_int(p4m.get("a_progress_window_bytes"))
    check.req(jobs is not None and jobs >= 0, "p4m.a_completed_jobs_window_nonneg", "got=%r" % (p4m.get("a_completed_jobs_window"),))
    check.req(jobs is not None and abytes is not None and abytes == jobs * CHUNK,
              "p4m.a_progress_window_bytes_eq_jobs_x_chunk", "jobs=%r bytes=%r" % (jobs, abytes))

    fb_jobs = as_int(p4m.get("a_started_before_window_completed"))
    fb_bytes = as_int(p4m.get("a_started_before_window_bytes"))
    check.req(fb_jobs in (0, 1), "p4m.flowboundary_jobs_in_0_1", "got=%r" % (p4m.get("a_started_before_window_completed"),))
    check.req(fb_bytes is not None and 0 <= fb_bytes <= CHUNK, "p4m.flowboundary_bytes_within_chunk", "got=%r" % (p4m.get("a_started_before_window_bytes"),))
    check.adv(fb_jobs is not None and fb_bytes is not None and fb_bytes == fb_jobs * CHUNK,
              "advisory.flowboundary_bytes_eq_jobs_x_chunk", "jobs=%r bytes=%r" % (fb_jobs, fb_bytes))

    if abytes is not None and hw:
        ok_bytes, detail_bytes = _close(p4m.get("a_completed_bytes_per_second"), abytes / (hw / 1e9))
        check.req(ok_bytes, "p4m.a_completed_bytes_per_second_recomputed", detail_bytes)
        if jobs is not None:
            ok_jobs, detail_jobs = _close(p4m.get("a_completed_jobs_per_second"), jobs / (hw / 1e9))
            check.req(ok_jobs, "p4m.a_completed_jobs_per_second_recomputed", detail_jobs)
    else:
        check.req(False, "p4m.a_rate_inputs", "缺少 cycles/hardware_window_ns 或 A 窗口字节")

    _validate_timing(t, check, strict_timing, seq, cycles, p4m)


def _validate_timing(t, check, strict_timing, seq, cycles, p4m):
    if t.startup_kv is None or t.batchtime_kv is None:
        note = "计时记录不可用（timing_source=%s）" % t.timing_source
        if t.missing_notes:
            note += "；" + "；".join(t.missing_notes)
        check.miss("timing_record", note)
        if strict_timing:
            check.req(False, "timing.record_available", note)
        else:
            check.adv(False, "timing.record_available", note)
        return

    skv = t.startup_kv
    bkv = t.batchtime_kv
    check.req(as_int(skv.get("version")) == 1, "timing.startup_version_1", "got=%r" % (skv.get("version"),))
    check.req(as_int(skv.get("seq")) == seq, "timing.startup_seq_eq_p4m_sequence",
              "startup=%r json=%r" % (skv.get("seq"), seq))
    check.req(skv.get("method") == "irq_pair", "timing.startup_method_irq_pair", "got=%r" % (skv.get("method"),))
    check.req(as_int(skv.get("attempted")) == 1, "timing.startup_attempted_1", "got=%r" % (skv.get("attempted"),))
    check.req(as_int(skv.get("has_a")) == 1, "timing.startup_has_a_1", "got=%r" % (skv.get("has_a"),))
    check.req(as_int(skv.get("a_triggered")) == 1, "timing.startup_a_triggered_1", "got=%r" % (skv.get("a_triggered"),))
    check.req(as_int(skv.get("v_triggered")) == 1, "timing.startup_v_triggered_1", "got=%r" % (skv.get("v_triggered"),))
    check.req(as_int(skv.get("error")) == 0, "timing.startup_error_0", "got=%r" % (skv.get("error"),))
    check.req(as_int(skv.get("v_did")) == 10, "timing.startup_v_did_10", "got=%r" % (skv.get("v_did"),))
    check.req(as_int(skv.get("a_did")) == 11, "timing.startup_a_did_11", "got=%r" % (skv.get("a_did"),))
    a_id = as_int(skv.get("a_id"))
    v_id = as_int(skv.get("v_id"))
    check.req(a_id is not None and a_id > 0, "timing.startup_a_id_positive", "got=%r" % (skv.get("a_id"),))
    check.req(v_id is not None and v_id > 0, "timing.startup_v_id_positive", "got=%r" % (skv.get("v_id"),))

    keys = ("a_begin_ns", "a_end_ns", "v_begin_ns", "v_end_ns", "irq_span_ns", "a_to_v_ns")
    times = {k: as_int(skv.get(k)) for k in keys}
    absent = [k for k, v in times.items() if v is None]
    check.req(not absent, "timing.startup_integer_fields", "缺失/非整数: %s" % (absent,))
    if not absent:
        a0, a1 = times["a_begin_ns"], times["a_end_ns"]
        v0, v1 = times["v_begin_ns"], times["v_end_ns"]
        check.req(a0 <= a1 <= v0 <= v1, "timing.startup_time_order", "a0=%d a1=%d v0=%d v1=%d" % (a0, a1, v0, v1))
        check.req(times["a_to_v_ns"] == v0 - a0, "timing.a_to_v_ns_eq_v_begin_minus_a_begin",
                  "got=%d want=%d" % (times["a_to_v_ns"], v0 - a0))
        check.req(times["irq_span_ns"] == v1 - a0, "timing.irq_span_ns_eq_v_end_minus_a_begin",
                  "got=%d want=%d" % (times["irq_span_ns"], v1 - a0))
        check.req(times["irq_span_ns"] > 0, "timing.irq_span_positive", "got=%d" % times["irq_span_ns"])

    check.req(bkv.get("part") == "time", "timing.batch_time_part_time", "got=%r" % (bkv.get("part"),))
    check.req(as_int(bkv.get("seq")) == seq, "timing.batch_time_seq_eq_p4m_sequence",
              "batch_time=%r json=%r" % (bkv.get("seq"), seq))
    check.req(as_int(bkv.get("valid")) == 1, "timing.batch_time_valid_1", "got=%r" % (bkv.get("valid"),))
    check.req(as_int(bkv.get("status")) == 4, "timing.batch_time_status_0x4", "got=%r" % (bkv.get("status"),))
    check.req(as_int(bkv.get("v_did")) == 10, "timing.batch_time_v_did_10", "got=%r" % (bkv.get("v_did"),))
    check.req(as_int(bkv.get("a_did")) == 11, "timing.batch_time_a_did_11", "got=%r" % (bkv.get("a_did"),))
    bt_cycles = as_int(bkv.get("cycles"))
    check.req(bt_cycles is not None and cycles is not None and bt_cycles == cycles,
              "timing.batch_time_cycles_eq_p4m_cycles", "batch_time=%r p4m=%r" % (bt_cycles, cycles))
    check.req(as_int(bkv.get("issued")) == TRANSFERS, "timing.batch_time_issued_4096", "got=%r" % (bkv.get("issued"),))
    check.req(as_int(bkv.get("done")) == TRANSFERS, "timing.batch_time_done_4096", "got=%r" % (bkv.get("done"),))
    check.req(as_int(bkv.get("batch_polled")) == 1, "timing.batch_time_batch_polled_1", "got=%r" % (bkv.get("batch_polled"),))

    raw_window = p4m.get("raw_window")
    if isinstance(raw_window, list) and len(raw_window) >= 16:
        pairs = ((0, "bank_magic"), (1, "cycles"), (2, "a_issue_window"), (3, "a_done_window"),
                 (4, "a_inflight_at_issue"), (5, "a_inflight_at_done"),
                 (6, "a_started_before_window_completed"), (7, "a_issue_opening_edge"),
                 (8, "a_opening_inflight_id"), (9, "a_closing_inflight_id"),
                 (10, "a_first_done_id"), (11, "a_last_done_id"), (15, "flags"))
        for idx, name in pairs:
            want = as_int(p4m.get(name))
            got = as_int(raw_window[idx])
            check.adv(got is not None and want is not None and got == want,
                      "advisory.raw_window[%d]==%s" % (idx, name), "got=%r want=%r" % (got, want))
    else:
        check.adv(False, "advisory.raw_window_shape", "raw_window 缺失/长度不足/类型不符")

    raw_obs = p4m.get("raw_observation")
    if isinstance(raw_obs, list) and len(raw_obs) >= 35:
        check.adv(as_int(raw_obs[0]) == cycles, "advisory.raw_observation[0]==cycles",
                  "got=%r want=%r" % (raw_obs[0], cycles))
        anchors = ((1, "h"), (3, "gap"), (4, "hmax"), (6, "gapmax"))
        for idx, name in anchors:
            want = as_int(bkv.get(name))
            got = as_int(raw_obs[idx])
            check.adv(got is not None and want is not None and got == want,
                      "advisory.raw_observation[%d]==batch_time.%s" % (idx, name), "got=%r want=%r" % (got, want))
        want_last = as_int(bkv.get("done_to_read"))
        got_last = as_int(raw_obs[-1])
        check.adv(got_last is not None and want_last is not None and got_last == want_last,
                  "advisory.raw_observation[-1]==batch_time.done_to_read", "got=%r want=%r" % (got_last, want_last))
    else:
        check.adv(False, "advisory.raw_observation_shape", "raw_observation 缺失/长度不足/类型不符")


# ---------------------------------------------------------------------------
# 数据集装载
# ---------------------------------------------------------------------------
def load_dataset(strict_timing=True):
    trials = []
    files_read = []
    layout_errors = []
    actual_by_dir = {}
    for key, conf in sorted(DIR_CONF.items()):
        path = conf["path"]
        actual_by_dir[key] = sorted(p.name for p in path.glob("formal-*.json")) if path.is_dir() else None
    ok_manifest, manifest_errors = manifest_check(actual_by_dir)
    if not ok_manifest:
        layout_errors.extend(manifest_errors)
    for (block_no, gap), dir_key in sorted(EXPECTED_LAYOUT.items()):
        case_id = "formal-b%02d-g%d" % (block_no, gap)
        conf = DIR_CONF[dir_key]
        dirpath = conf["path"]
        json_path = dirpath / (case_id + ".json")
        json_obj = None
        if json_path.is_file():
            files_read.append(json_path)
            try:
                json_obj = json.loads(read_text(json_path))
            except Exception as exc:  # noqa: BLE001 - 需要把解析失败当成数据问题上报
                layout_errors.append({"case": case_id, "check": "json.parse",
                                      "detail": "%s: %s" % (json_path, exc)})
        else:
            elsewhere = [k for k, c in sorted(DIR_CONF.items()) if (c["path"] / (case_id + ".json")).is_file()]
            layout_errors.append({"case": case_id, "check": "json.missing_in_expected_dir",
                                  "detail": "期望目录 %s 缺少 %s；其它目录中存在: %s"
                                            % (dir_key, case_id, ", ".join(elsewhere) or "无")})

        trial = Trial(case_id=case_id, block_no=block_no, block="b%02d" % block_no, gap=gap,
                      acq=conf["acq"], power=conf["power"],
                      audit=audit_identity(conf["acq"], case_id), dir_key=dir_key,
                      json_path=json_path if json_path.is_file() else None,
                      json_obj=json_obj if isinstance(json_obj, dict) else {})

        seq = None
        if isinstance(json_obj, dict) and isinstance(json_obj.get("p4m"), dict):
            seq = as_int(json_obj["p4m"].get("sequence"))

        st_text, st_source, st_used, st_note = resolve_record(
            case_id, block_no, gap, dirpath, ".startup", "startup", seq)
        bt_text, bt_source, bt_used, bt_note = resolve_record(
            case_id, block_no, gap, dirpath, ".batch_time", "batch_time", seq)
        files_read.extend(st_used)
        files_read.extend(bt_used)
        trial.startup_path = st_used[0] if st_used else None
        trial.startup_text = st_text
        trial.startup_kv = parse_record_kv(st_text, "startup") if st_text else None
        trial.batchtime_path = bt_used[0] if bt_used else None
        trial.batchtime_text = bt_text
        trial.batchtime_kv = parse_record_kv(bt_text, "batch_time") if bt_text else None
        trial.timing_source = "startup:%s|batch_time:%s" % (st_source, bt_source)
        trial.missing_notes = [n for n in (st_note, bt_note) if n]

        check = CaseCheck()
        derive_metrics(trial)
        validate_case(trial, check, strict_timing=strict_timing)
        trial.required_errors = check.required
        trial.advisory_warnings = check.advisory
        trial.missing_items = list(check.missing)
        if trial.missing_notes:
            trial.missing_items.extend({"item": "timing_file", "detail": n} for n in trial.missing_notes)
        trials.append(trial)
    return trials, files_read, layout_errors


# ---------------------------------------------------------------------------
# 矩阵与全局核验
# ---------------------------------------------------------------------------
def check_layout_purity(trials):
    errors = []
    seen = {}
    duplicates = []
    for t in trials:
        key = (t.block_no, t.gap)
        if key in seen:
            duplicates.append("%s 与 %s" % (seen[key].case_id, t.case_id))
        seen[key] = t
    if duplicates:
        errors.append({"check": "matrix.duplicate_block_gap",
                       "detail": "重复 (block,gap)：%s" % "; ".join(duplicates)})
    missing = sorted(k for k in EXPECTED_LAYOUT if k not in seen)
    if missing:
        errors.append({"check": "matrix.missing_case",
                       "detail": "缺少 %d 例：%s" % (len(missing), ", ".join("b%02d-g%d" % k for k in missing))})
    extra = sorted(k for k in seen if k not in EXPECTED_LAYOUT)
    if extra:
        errors.append({"check": "matrix.unexpected_case",
                       "detail": "多出 %d 例：%s" % (len(extra), ", ".join("b%02d-g%d" % k for k in extra))})
    for gap in GAPS:
        n = sum(1 for k in seen if k[1] == gap)
        if n != 20:
            errors.append({"check": "matrix.gap_count_%d" % gap, "detail": "n=%d，期望 20" % n})
    return (not errors), errors


def global_checks(trials, layout_errors, metadata):
    required = []
    advisory = []
    missing = []

    required.extend(layout_errors)
    ok, errors = check_layout_purity(trials)
    required.extend(errors)

    for t in trials:
        for item in t.required_errors:
            required.append({"case": t.case_id, **item})
        for item in t.advisory_warnings:
            advisory.append({"case": t.case_id, **item})
        for item in t.missing_items:
            missing.append({"case": t.case_id, **item})

    # 目录与声明布局一致
    for t in trials:
        want = EXPECTED_LAYOUT.get((t.block_no, t.gap))
        if want is not None and want != t.dir_key:
            required.append({"case": t.case_id, "check": "matrix.dir_key_mismatch",
                             "detail": "实际=%s 期望=%s" % (t.dir_key, want)})

    online = [t.case_id for t in trials if t.audit == "online_pass"]
    offline = [t.case_id for t in trials if t.audit == "offline_retained"]
    if len(online) != 53:
        required.append({"check": "audit.online_count", "detail": "在线身份 n=%d，期望 53" % len(online)})
    if len(offline) != 7:
        required.append({"check": "audit.offline_count", "detail": "离线追认 n=%d，期望 7" % len(offline)})

    # 采集段内 seq 唯一
    for acq in ACQ_ORDER:
        seqs = {}
        for t in trials:
            if t.acq != acq:
                continue
            s = t.derived.get("seq")
            if s is None:
                continue
            seqs.setdefault(s, []).append(t.case_id)
        dup = {s: v for s, v in seqs.items() if len(v) > 1}
        if dup:
            required.append({"check": "segment.seq_unique_%s" % acq,
                             "detail": "重复 seq：%s" % dup})

    # 段内 seq 连续性与启动时间单调（事实性，advisory）
    seq_facts = {}
    for power in POWER_ORDER:
        sel = [t for t in trials if t.power == power and t.derived.get("seq") is not None]
        seqs = sorted(t.derived["seq"] for t in sel)
        seq_facts[power] = {"n": len(seqs),
                            "min": seqs[0] if seqs else None,
                            "max": seqs[-1] if seqs else None,
                            "contiguous": bool(seqs) and seqs == list(range(seqs[0], seqs[0] + len(seqs)))}
        ordered = sorted(sel, key=lambda t: t.derived["seq"])
        bad = []
        prev = None
        for t in ordered:
            cur = t.derived.get("a_begin_ns")
            if prev is not None and cur is not None and cur <= prev:
                bad.append(t.case_id)
            prev = cur if cur is not None else prev
        seq_facts[power]["a_begin_monotonic_by_seq"] = (not bad)
        seq_facts[power]["a_begin_nonmonotonic_cases"] = bad
        if not seq_facts[power]["contiguous"]:
            advisory.append({"check": "warn.seq_not_contiguous_%s" % power,
                             "detail": "n=%s min=%s max=%s contiguous=False" % (
                                 seq_facts[power]["n"], seq_facts[power]["min"],
                                 seq_facts[power]["max"])})
        if seq_facts[power]["a_begin_nonmonotonic_cases"]:
            advisory.append({"check": "warn.a_begin_not_monotonic_%s" % power,
                             "detail": "乱序样本：%s" % ", ".join(seq_facts[power]["a_begin_nonmonotonic_cases"])})

    # bank_magic 一致性（只比较一致性，不对具体 magic 值做断言）
    magics = {}
    for t in trials:
        magics.setdefault(str(t.derived.get("bank_magic")), []).append(t.case_id)
    if len(magics) > 1:
        advisory.append({"check": "fact.bank_magic_consistency",
                         "detail": "出现多个 bank_magic 取值：%s" % {k: len(v) for k, v in magics.items()}})

    # 交接口径对照（正常事实不进入 advisory；缺项与真实异常才上报）
    handover = {}
    rem = metadata.get("remaining29", {}).get("params.txt")
    if rem:
        handover["remaining29_params"] = rem
    ret = metadata.get("retained6", {}).get("params.txt")
    if ret:
        handover["retained6_params"] = ret
    orig = metadata.get("orig25", {}).get("params.txt")
    if orig:
        handover["orig25_params"] = orig
    meta_required, meta_missing = validate_segment_metadata(metadata)
    required.extend(meta_required)
    missing.extend(meta_missing)
    for key, entry in metadata.items():
        for name in ("params.txt", "matrix.txt", "input.txt"):
            if entry.get(name) is None:
                missing.append({"case": None, "item": "metadata_%s_%s" % (key, name),
                                "detail": "文件缺失，未读取"})

    return required, advisory, missing, seq_facts, handover


# ---------------------------------------------------------------------------
# 描述统计与配对
# ---------------------------------------------------------------------------
def stats_for(trials):
    out = {}
    for key, _label in STAT_METRICS:
        out[key] = mm([t.derived.get(key) for t in trials])
    return out


def describe(trials):
    return {
        "n": len(trials),
        "gap_counts": {str(g): sum(1 for t in trials if t.gap == g) for g in GAPS},
        "audit_counts": {
            "online_pass": sum(1 for t in trials if t.audit == "online_pass"),
            "offline_retained": sum(1 for t in trials if t.audit == "offline_retained"),
        },
        "by_gap": {str(g): stats_for([t for t in trials if t.gap == g]) for g in GAPS},
        "overall": stats_for(trials),
    }


def build_pairs(trials):
    blocks = {}
    for t in trials:
        blocks.setdefault(t.block_no, {})[t.gap] = t
    rows = []
    for block_no in sorted(blocks):
        per = blocks[block_no]
        g0 = per.get(0)
        if g0 is None:
            continue
        for gap in (1000, 2000):
            cur = per.get(gap)
            if cur is None:
                continue
            row = {
                "block": "b%02d" % block_no,
                "gap": gap,
                "gap0_case": g0.case_id,
                "case": cur.case_id,
                "gap0_acquisition_segment": g0.acq,
                "case_acquisition_segment": cur.acq,
                "gap0_power_segment": g0.power,
                "case_power_segment": cur.power,
                "same_acquisition_segment": cur.acq == g0.acq,
                "same_power_segment": cur.power == g0.power,
            }
            v0 = g0.derived.get("v_cycles")
            v1 = cur.derived.get("v_cycles")
            if v0 and v1:
                row["v_cycles_ratio"] = v1 / v0
                row["v_time_decrease_pct"] = (1.0 - v1 / v0) * 100.0
            r0 = g0.derived.get("v_mib_s")
            r1 = cur.derived.get("v_mib_s")
            if r0 and r1:
                row["v_rate_ratio"] = r1 / r0
                row["v_rate_improvement_pct"] = (r1 / r0 - 1.0) * 100.0
            a0 = g0.derived.get("a_window_mib_s")
            a1 = cur.derived.get("a_window_mib_s")
            if a0 and a1:
                row["a_rate_ratio"] = a1 / a0
                row["a_rate_loss_pct"] = (1.0 - a1 / a0) * 100.0
            rows.append(row)
    views = {
        "same_acquisition_segment": [r for r in rows if r["same_acquisition_segment"]],
        "same_power_segment": [r for r in rows if r["same_power_segment"]],
        "flagged_cross_power_segment": [r for r in rows if not r["same_power_segment"]],
    }
    return rows, views


def aggregate_pairs(rows):
    out = {}
    for gap in (1000, 2000):
        sel = [r for r in rows if r["gap"] == gap]
        agg = {"n_pairs": len(sel), "blocks": [r["block"] for r in sel]}
        for key, _label in PAIR_QUANTITIES:
            vals = [r.get(key) for r in sel]
            agg[key] = mm(vals) if any(is_number(v) for v in vals) else mm(vals)
        out[str(gap)] = agg
    return out


# ---------------------------------------------------------------------------
# 事实性变化与 pre/post 描述性差值
# ---------------------------------------------------------------------------
def facts_section(trials):
    hist_keys = ("p4m_flags", "a_opening_inflight_id", "a_flowboundary_jobs", "a_flowboundary_bytes",
                 "a_inflight_at_issue", "a_inflight_at_done", "strict_preopening_inflight",
                 "bank_magic", "done_to_read")
    facts = {}
    for key in hist_keys:
        by_gap = {}
        for gap in GAPS:
            per_gap = [t for t in trials if t.gap == gap]
            by_gap[str(gap)] = {power: hist_and_offmodal([t for t in per_gap if t.power == power], key)
                                for power in POWER_ORDER}
        facts[key] = {"by_gap": by_gap}
    facts["issue_vs_done_mismatch_cases"] = sorted(
        t.case_id for t in trials
        if t.derived.get("a_issue_window") is not None and t.derived.get("a_done_window") is not None
        and t.derived["a_issue_window"] != t.derived["a_done_window"])
    facts["v_miss_nonzero_cases"] = []
    for t in trials:
        d = t.derived
        miss = d.get("v_whole_miss")
        if miss:
            facts["v_miss_nonzero_cases"].append({
                "case_id": t.case_id, "block": t.block, "gap": t.gap,
                "power_segment": t.power, "acquisition_segment": t.acq,
                "seq": d.get("seq"), "v_whole_miss": miss, "v_whole_s1": d.get("v_whole_s1"),
                "v_whole_s2": d.get("v_whole_s2"), "v_mib_s": d.get("v_mib_s"),
                "v_cycles": d.get("v_cycles"), "hardware_window_ns": d.get("hardware_window_ns"),
                "a_whole_miss": d.get("a_whole_miss"), "a_opening_inflight_id": d.get("a_opening_inflight_id"),
                "a_to_v_ns": d.get("a_to_v_ns"), "irq_span_ns": d.get("irq_span_ns"),
                "done_to_read": d.get("done_to_read"), "done_to_read_ns": d.get("done_to_read_ns"),
                "json_path": str(t.json_path) if t.json_path else None,
                "startup_path": str(t.startup_path) if t.startup_path else None,
                "batchtime_path": str(t.batchtime_path) if t.batchtime_path else None})
    facts["startup_irq_span_ns"] = {
        str(g): {power: mm([t.derived.get("irq_span_ns") for t in trials if t.gap == g and t.power == power])
                 for power in POWER_ORDER} for g in GAPS}
    facts["startup_a_to_v_ns"] = {
        str(g): {power: mm([t.derived.get("a_to_v_ns") for t in trials if t.gap == g and t.power == power])
                 for power in POWER_ORDER} for g in GAPS}
    return facts


def _bucket(trials, key):
    out = {}
    for t in trials:
        out.setdefault(str(t.derived.get(key)), []).append(t.case_id)
    return out


def median_diff_pre_post(trials):
    pre = [t for t in trials if t.power == "pre_powercycle"]
    post = [t for t in trials if t.power == "post_powercycle_newboot"]
    out = {}
    for key, label in STAT_METRICS:
        per_gap = {}
        for gap in GAPS:
            a = mm([t.derived.get(key) for t in pre if t.gap == gap])
            b = mm([t.derived.get(key) for t in post if t.gap == gap])
            diff = None
            if is_number(a["median"]) and is_number(b["median"]):
                diff = b["median"] - a["median"]
            per_gap[str(gap)] = {"pre": a, "post": b, "median_diff_post_minus_pre": diff}
        out[key] = {"label": label, "by_gap": per_gap}
    return out


# ---------------------------------------------------------------------------
# 报告渲染
# ---------------------------------------------------------------------------
def fmt(value, nd=3):
    if value is None:
        return "n/a"
    if isinstance(value, bool):
        return "true" if value else "false"
    if isinstance(value, float):
        if value != 0 and (abs(value) >= 1e7 or abs(value) < 1e-4):
            return "%.5g" % value
        return ("%%.%df" % nd) % value
    return str(value)


def md_table(headers, rows):
    lines = ["| " + " | ".join(headers) + " |",
             "|" + "|".join(["---"] * len(headers)) + "|"]
    for row in rows:
        lines.append("| " + " | ".join(fmt(c) if not isinstance(c, str) else c for c in row) + " |")
    return "\n".join(lines)


def metric_table(desc):
    headers = ["指标"]
    for gap in GAPS:
        headers.extend(["gap%d min" % gap, "gap%d 中位" % gap, "gap%d max" % gap])
    rows = []
    for key, label in STAT_METRICS:
        row = [label]
        for gap in GAPS:
            m = desc["by_gap"][str(gap)][key]
            row.extend([m["min"], m["median"], m["max"]])
        rows.append(row)
    return md_table(headers, rows)


def pair_table(agg):
    headers = ["gap", "n"]
    for _key, label in PAIR_QUANTITIES:
        headers.extend([label + " 中位", label + " min", label + " max"])
    rows = []
    for gap in (1000, 2000):
        entry = agg[str(gap)]
        row = [gap, entry["n_pairs"]]
        for key, _label in PAIR_QUANTITIES:
            m = entry[key]
            row.extend([m["median"], m["min"], m["max"]])
        rows.append(row)
    return md_table(headers, rows)


def render_md(summary):
    parts = []
    parts.append("# P4M 60 例（25 + 离线追认6 + 新启动29）只读审计与描述统计\n")
    parts.append("生成时间：%s\n" % summary["generated_utc"])
    parts.append("程序：`out/p4m-complete-review-20261002/analyze.py`（只读；本报告数字全部由程序计算）\n")
    g = summary["descriptive"]["all60"]["by_gap"]
    pair = summary["pairs"]["aggregates"]
    p1000 = pair["same_acquisition_segment"]["1000"]
    p2000 = pair["same_acquisition_segment"]["2000"]
    ps2000 = pair["same_power_segment"]["2000"]
    parts.append("\n## 0. 一页中文摘要（数字全部来自本程序计算）\n")
    parts.append("- 60 例按 gap 各 20；V 中位速率（MiB/s）在 gap0/1000/2000 分别为 %s / %s / %s；"
                 "A 同窗中位速率为 %s / %s / %s。"
                 % (fmt(g["0"]["v_mib_s"]["median"]), fmt(g["1000"]["v_mib_s"]["median"]),
                    fmt(g["2000"]["v_mib_s"]["median"]), fmt(g["0"]["a_window_mib_s"]["median"]),
                    fmt(g["1000"]["a_window_mib_s"]["median"]), fmt(g["2000"]["a_window_mib_s"]["median"])))
    parts.append("- 主要严格同采集段配对：gap1000 n=%s，V 吞吐中位提升 %s%%、V 耗时中位下降 %s%%、"
                 "A 同窗速率中位损失 %s%%；gap2000 n=%s，对应为 %s%% / %s%% / %s%%。"
                 % (p1000["n_pairs"], fmt(p1000["v_rate_improvement_pct"]["median"]),
                    fmt(p1000["v_time_decrease_pct"]["median"]), fmt(p1000["a_rate_loss_pct"]["median"]),
                    p2000["n_pairs"], fmt(p2000["v_rate_improvement_pct"]["median"]),
                    fmt(p2000["v_time_decrease_pct"]["median"]), fmt(p2000["a_rate_loss_pct"]["median"])))
    parts.append("- 同电源段敏感性：gap2000 n=%s，V 吞吐中位提升 %s%%、V 耗时下降 %s%%、A 损失 %s%%，"
                 "与同采集段结果近似一致；这是描述，不是统计等价声明。"
                 % (ps2000["n_pairs"], fmt(ps2000["v_rate_improvement_pct"]["median"]),
                    fmt(ps2000["v_time_decrease_pct"]["median"]), fmt(ps2000["a_rate_loss_pct"]["median"])))
    nonzero_v_miss_cases = summary["nonzero_v_miss_cases"]
    if nonzero_v_miss_cases:
        cases = "、".join(
            "%s（V miss=%s/S1=%s，A 整轮 miss=%s，V 速率 %s MiB/s）" % (
                c["case_id"], c["v_whole_miss"], c["v_whole_s1"], c["a_whole_miss"], fmt(c["v_mib_s"]))
            for c in nonzero_v_miss_cases)
        parts.append("- V 整轮 miss 非零样本共 %d 例：%s；详见第 6 节。V 最快不等于 miss 加速，"
                     "不从同时出现推断因果，也不剔除该样本。" % (len(nonzero_v_miss_cases), cases))
    else:
        parts.append("- V 整轮 miss 非零样本共 0 例；详见第 6 节。V 最快不等于 miss 加速，"
                     "不从同时出现推断因果，也不剔除该样本。")
    parts.append("- 在线/离线身份沿用既有审计：在线通过 %s 例、离线追认 %s 例；本程序不验证缺失的原始 exit 记录。"
                 % (summary["sample_counts"]["online_pass"], summary["sample_counts"]["offline_retained"]))
    parts.append("- 内核：params 记录要求精确 00089（%s）；硬件窗口按固定 50 MHz、20 ns/cycle 假设由 cycles 换算。"
                 % REQUIRED_KERNEL_RELEASE)

    parts.append("## 1. 数据来源与分段\n")
    rows = []
    for key, conf in DIR_CONF.items():
        n = sum(1 for t in summary["trials_index"] if t["dir_key"] == key)
        gaps = {str(g): sum(1 for t in summary["trials_index"] if t["dir_key"] == key and t["gap"] == g)
                for g in GAPS}
        rows.append([key, str(conf["path"]), conf["acq"], conf["power"], n, str(gaps)])
    parts.append(md_table(["目录", "路径", "采集段", "电源段", "例数", "gap 分布"], rows))
    parts.append("")
    parts.append("- 在线通过身份 %d 例，离线追认身份 %d 例；**不得**称为同一次启动 60 次在线通过。"
                 % (summary["sample_counts"]["online_pass"], summary["sample_counts"]["offline_retained"]))
    parts.append("- `pre_powercycle` = 25 + 6 = %d 例这一归组只依据日志事实（seq 连续性、启动时间单调）与交接说明；"
                 "相同内核串不作为同 boot 的证明。"
                 % summary["sample_counts"]["pre_powercycle"])
    parts.append("- 跨采集段区块：%s" % ", ".join(summary["sample_counts"]["cross_segment_blocks"] or ["无"]))

    parts.append("\n## 2. 审计结果\n")
    parts.append("- 必需检查失败：%d 条" % len(summary["audit"]["required_failures"]))
    parts.append("- 缺项（不影响数据有效性但需说明）：%d 条" % len(summary["audit"]["missing_items"]))
    parts.append("- 警告（advisory）：%d 条" % len(summary["audit"]["advisory_warnings"]))
    parts.append("- 输入文件 SHA256 复核：%s"
                 % ("未变" if summary["inputs"]["unchanged"] else "**发生变化**"))
    if summary["audit"]["required_failures"]:
        parts.append("\n前 20 条必需失败：\n")
        rows = [[e.get("case") or "-", e.get("check"), e.get("detail")]
                for e in summary["audit"]["required_failures"][:20]]
        parts.append(md_table(["case", "检查", "细节"], rows))
    if summary["audit"]["missing_items"]:
        parts.append("\n缺项清单：\n")
        rows = [[e.get("case") or "-", e.get("item"), e.get("detail")]
                for e in summary["audit"]["missing_items"]]
        parts.append(md_table(["case", "缺项", "说明"], rows))

    parts.append("\n## 3. 全 60 描述统计（每条件 n=20；min/中位/max；含 IQR 见 summary.json）\n")
    parts.append(metric_table(summary["descriptive"]["all60"]))

    parts.append("\n## 4. 分段描述统计\n")
    for key in ACQ_ORDER:
        desc = summary["descriptive"]["by_acquisition_segment"][key]
        parts.append("\n### 采集段 %s（每 gap n=%s）\n" % (key, desc["gap_counts"]))
        parts.append(metric_table(desc))
    for key in POWER_ORDER:
        desc = summary["descriptive"]["by_power_segment"][key]
        parts.append("\n### 电源段 %s（每 gap n=%s）\n" % (key, desc["gap_counts"]))
        parts.append(metric_table(desc))

    parts.append("\n### pre31 与 post29 的描述性差值（中位差，不做检验）\n")
    rows = []
    for key, entry in summary["pre_post_median_diff"].items():
        row = [entry["label"]]
        for gap in GAPS:
            cell = entry["by_gap"][str(gap)]
            row.append("%s / %s / %s" % (fmt(cell["pre"]["median"]), fmt(cell["post"]["median"]),
                                          fmt(cell["median_diff_post_minus_pre"])))
        rows.append(row)
    parts.append(md_table(["指标", "gap0 pre/post/差", "gap1000 pre/post/差", "gap2000 pre/post/差"], rows))
    parts.append("")
    parts.append("- 两侧样本数不等（pre 10/10/11，post 10/10/9）；"
                 "中位相近**不能**作为统计等价的证据，上表只是事实陈述。")

    parts.append("\n## 5. 配对效果（相对同 block 的 gap0）\n")
    parts.append("- 先算每个 block 的比值，再取这些比值的中位；**不是**两个中位数之比。")
    parts.append("- 视图 A＝同一采集段配对；视图 B＝同一 pre/post 电源段配对（B ⊇ A）；"
                 "两者之外的跨电源配对单列，不作为主要配对（其中含 b11 的 gap2000/g0）。")
    for view in ("same_acquisition_segment", "same_power_segment", "flagged_cross_power_segment"):
        parts.append("\n### %s\n" % view)
        parts.append(pair_table(summary["pairs"]["aggregates"][view]))
    parts.append("\n### 逐 block 明细\n")
    rows = []
    for row in summary["pairs"]["rows"]:
        rows.append([row["block"], row["gap"], row["case"], row["gap0_case"],
                     "是" if row["same_acquisition_segment"] else "否",
                     "是" if row["same_power_segment"] else "否",
                     row.get("v_rate_ratio"), row.get("v_rate_improvement_pct"),
                     row.get("v_time_decrease_pct"), row.get("a_rate_ratio"), row.get("a_rate_loss_pct")])
    parts.append(md_table(["block", "gap", "case", "gap0", "同采集段", "同电源段",
                           "V速率比", "V吞吐提升%", "V时间下降%", "A速率比", "A同窗速率损失%"], rows))

    parts.append("\n## 6. 事实性变化记录（按 gap，再按 pre/post 分组；不推断统计等价）\n")
    facts = summary["facts"]
    hist_keys = ("p4m_flags", "a_opening_inflight_id", "a_flowboundary_jobs", "a_flowboundary_bytes",
                 "a_inflight_at_issue", "a_inflight_at_done", "strict_preopening_inflight",
                 "done_to_read")
    for key in hist_keys:
        rows = []
        for gap in GAPS:
            for power in POWER_ORDER:
                entry = facts[key]["by_gap"][str(gap)][power]
                rows.append(["gap%d" % gap, power, str(entry["histogram"]), entry["modal"],
                             ", ".join(entry["off_modal_cases"]) or "无"])
        parts.append("\n### %s（先 gap 后 pre/post）\n" % key)
        parts.append(md_table(["gap", "电源段", "取值分布", "众数", "偏离众数的样本"], rows))
    parts.append("\n- `a_issue_window != a_done_window` 的样本：%s"
                 % (", ".join(facts["issue_vs_done_mismatch_cases"]) or "无"))
    parts.append("- bank_magic 已按 bench 常量 %d 校验；不再把正常取值分布列为警告。" % P4M_BANK_MAGIC)
    parts.append("\n### V 整轮 miss 非零样本（整轮计数，非硬件窗内计数）\n")
    rows = []
    for item in facts["v_miss_nonzero_cases"]:
        rows.append([item["case_id"], item["gap"], item["power_segment"], item["v_whole_miss"],
                     item["v_whole_s1"], item["v_whole_s2"], item["v_mib_s"], item["a_whole_miss"],
                     item["a_opening_inflight_id"], item["a_to_v_ns"], item["irq_span_ns"],
                     item["done_to_read"], item["done_to_read_ns"]])
    parts.append(md_table(["case", "gap", "电源段", "V整轮miss", "V s1", "V s2", "V速率MiB/s",
                           "A整轮miss", "开窗在途ID", "a_to_v_ns", "irq_span_ns",
                           "done_to_read cycles", "done_to_read ns"], rows) if rows else "无")
    b15 = [i for i in facts["v_miss_nonzero_cases"] if i["case_id"] == "formal-b15-g0"]
    if b15:
        b = b15[0]
        ms = (b["done_to_read_ns"] / 1e6) if b["done_to_read_ns"] else 0.0
        parts.append("")
        parts.append("b15-g0 突出事实：V 整轮 iotlb_miss=%s、s1_ptw=%s；硬件窗口 %s ns、cycles=%s、"
                     "V 速率 %s MiB/s；A 整轮 miss=%s；开窗在途 ID=%s；a_to_v_ns=%s、irq_span_ns=%s、"
                     "done_to_read=%s cycles（%s ns ≈ %.4f ms）。以上均为整轮或启动记录原值，"
                     "这里只取得整轮 miss 计数，无法确定其发生在硬件窗口内还是窗口外，不能定位具体相位；JSON 无逐条目轨迹，不得据此推断替换、启动跨度或轮询迟读的因果。"
                     % (b["v_whole_miss"], b["v_whole_s1"], b["hardware_window_ns"], b["v_cycles"],
                        fmt(b["v_mib_s"]), b["a_whole_miss"], b["a_opening_inflight_id"],
                        b["a_to_v_ns"], b["irq_span_ns"], b["done_to_read"], b["done_to_read_ns"], ms))
        parts.append("来源：`%s`、`%s`、`%s`。" % (b["json_path"], b["startup_path"], b["batchtime_path"]))
    parts.append("\n- 电源段启动时间统计（按 gap，再 pre/post；irq_span_ns / a_to_v_ns）：%s"
                 % {"irq_span_ns": facts["startup_irq_span_ns"], "a_to_v_ns": facts["startup_a_to_v_ns"]})

    parts.append("\n## 7. 限度与禁止的宣称\n")
    for item in summary["limitations"]:
        parts.append("- %s" % item)
    return "\n".join(parts) + "\n"


# ---------------------------------------------------------------------------
# 主分析
# ---------------------------------------------------------------------------
def run_analysis(args):
    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    trials, files_read, layout_errors = load_dataset(strict_timing=True)
    metadata = read_segment_metadata(files_read)
    hashes_before = hash_files(files_read)

    required, advisory, missing, seq_facts, handover = global_checks(trials, layout_errors, metadata)

    # 描述统计
    descriptive = {
        "all60": describe(trials),
        "by_acquisition_segment": {seg: describe([t for t in trials if t.acq == seg]) for seg in ACQ_ORDER},
        "by_power_segment": {seg: describe([t for t in trials if t.power == seg]) for seg in POWER_ORDER},
    }
    pre31_vs_post29 = {
        "pre": describe([t for t in trials if t.power == "pre_powercycle"]),
        "post": describe([t for t in trials if t.power == "post_powercycle_newboot"]),
        "note": "样本数不等（pre 10/10/11，post 10/10/9）；只做描述性对照，不宣称等价或显著。",
    }

    pair_rows, pair_views = build_pairs(trials)
    pair_aggregates = {view: aggregate_pairs(rows) for view, rows in pair_views.items()}

    facts = facts_section(trials)
    median_diff = median_diff_pre_post(trials)

    cross_segment_blocks = sorted({t.block for t in trials
                                   if len({x.acq for x in trials if x.block == t.block}) > 1})

    limitations = [
        "本报告是 60 份既有证据的只读汇总，不是在线重跑；不含本轮 Solo。",
        "53 例在线通过 + 7 例离线追认；不得表述为同一次启动 60 次在线通过。",
        "pre_powercycle=31 的归组只依据日志事实与交接说明，相同内核串不是同 boot 证明。",
        "whole-run HPM（victim.iotlb_miss/s1_ptw/s2_ptw、aggressor 同项）是整轮软件窗口计数；"
        "不得作为同硬件窗口指标，也不得把整轮 miss 总数下降称为 miss 率下降。",
        "CPU 侧 victim_elapsed_ns 不是硬件窗口；硬件窗口一律取 p4m.hardware_window_ns。",
        "V 速率 = 4096*4096/(hardware_window_ns/1e9)/2**20 MiB/s；"
        "A 速率 = a_progress_window_bytes/(hardware_window_ns/1e9)/2**20 MiB/s，"
        "是窗口内完成作业×chunk，不是总线字节。",
        "配对视图 A（同采集段）与视图 B（同电源段）的 n 不同；不配平、不剔除异常样本、不自动重跑。",
        "b11 的 gap2000/g0 跨 boot，仅单列标记，不作为主要配对。",
        "无 P99/WCET，无最优 gap 宣称，不做显著性检验、功效分析或过度的统计推断。",
        "未对任何 miss 做每拷贝归一化；如需派生，必须限定在同整轮 A 工作窗口并写明含义。",
        "本轮只补缺并汇总；不得据此宣布某条件最优或某机制因果成立。",
        "在线/离线身份沿用既有审计口径；本程序不验证原始 exit 记录，也不把身份当作本次重新在线通过。",
        "params 元数据按 00089 内核、taint=0、postboot 新启动标记检查；缺少必需项按 required 失败上报。",
    ]

    summary = {
        "schema": "p4m.complete60.review.v1",
        "generated_utc": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "tool": "out/p4m-complete-review-20261002/analyze.py",
        "inputs": {
            "dirs": {k: str(c["path"]) for k, c in DIR_CONF.items()},
            "audit_alias_dir": str(AUDIT_ALIAS_DIR),
            "hashed_files": hashes_before,
            "hashed_file_count": len(hashes_before),
        },
        "sample_counts": {
            "total": len(trials),
            "per_gap": {str(g): sum(1 for t in trials if t.gap == g) for g in GAPS},
            "online_pass": sum(1 for t in trials if t.audit == "online_pass"),
            "offline_retained": sum(1 for t in trials if t.audit == "offline_retained"),
            "per_acquisition_segment": {seg: sum(1 for t in trials if t.acq == seg) for seg in ACQ_ORDER},
            "per_power_segment": {seg: sum(1 for t in trials if t.power == seg) for seg in POWER_ORDER},
            "pre_powercycle": sum(1 for t in trials if t.power == "pre_powercycle"),
            "post_powercycle_newboot": sum(1 for t in trials if t.power == "post_powercycle_newboot"),
            "cross_segment_blocks": cross_segment_blocks,
            "pre_powercycle_grouping_basis": "seq 连续性与启动时间单调（日志事实）+ 交接说明；"
                                             "相同内核串不作为同 boot 证明。",
        },
        "segment_metadata": {
            key: {name: entry for name, entry in meta.items()}
            for key, meta in metadata.items()
        },
        "audit": {
            "required_failures": required,
            "advisory_warnings": advisory,
            "missing_items": missing,
            "seq_facts": seq_facts,
            "handover_records": handover,
            "required_failure_count": len(required),
            "advisory_warning_count": len(advisory),
            "missing_item_count": len(missing),
            "identity_basis": "在线/离线身份沿用既有审计口径（acq25/retained6/remaining29 与 b09-g2000 离线追认）；本程序不验证缺失的原始 exit 记录。",
        },
        "descriptive": descriptive,
        "pre_post_median_diff": median_diff,
        "facts": facts,
        "nonzero_v_miss_cases": facts["v_miss_nonzero_cases"],
        "pairs": {
            "rows": pair_rows,
            "aggregates": pair_aggregates,
            "notes": [
                "同一采集段视图：gap0 与目标 gap 在同一 dir（同一采集段）。",
                "同一电源段视图：gap0 与目标 gap 在同一 pre/post 电源段，包含跨采集段但同电源段的配对。",
                "flagged_cross_power_segment：既不同采集段也不同电源段（b11 的 gap2000/g0），单列标记，不作为主要配对。",
                "先算每个 block 的比值再取中位，不把两个中位数之比称为配对中位数。",
                "n 在各视图/gap 间不同，不做配平、不剔除。",
                "配对来自同一 block 的顺序试次，不是同时测量，差异包含漂移。",
            ],
        },
        "trials_index": [
            {"case_id": t.case_id, "block": t.block, "gap": t.gap, "dir_key": t.dir_key,
             "acquisition_segment": t.acq, "power_segment": t.power, "audit_identity": t.audit,
             "seq_dmesg": t.derived.get("seq"), "timing_source": t.timing_source,
             "required_failure_count": len(t.required_errors),
             "advisory_warning_count": len(t.advisory_warnings),
             "json_path": str(t.json_path) if t.json_path else None}
            for t in trials
        ],
        "limitations": limitations,
        "warnings": [
            "缺项只单列说明，不伪造字段、不自动重跑、不剔除样本。",
            "额外 raw 数组交叉核对仅作为辅助检查；核心字段已按 bench 契约严格核验。",
            "bench 常量已校验。",
        ],
    }

    # 先完成所有读取，再复核输入未变
    hashes_after = hash_files(files_read)
    changed = sorted(p for p in hashes_before
                     if hashes_before[p]["sha256"] != hashes_after.get(p, {}).get("sha256"))
    summary["inputs"]["changed_files"] = changed
    summary["inputs"]["unchanged"] = not changed
    summary["inputs"]["hashes_after"] = hashes_after
    if changed:
        summary["audit"]["required_failures"].append(
            {"check": "inputs.unchanged", "detail": "运行期间输入文件 SHA256 变化：%s" % changed})
        summary["audit"]["required_failure_count"] = len(summary["audit"]["required_failures"])

    # trials.csv
    csv_path = out_dir / "trials.csv"
    with csv_path.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle)
        writer.writerow([header for _key, header in CSV_COLUMNS])
        for t in trials:
            writer.writerow([_csv_value(t, key) for key, _header in CSV_COLUMNS])

    # trials.json
    trials_json = []
    for t in trials:
        trials_json.append({
            "case_id": t.case_id, "block": t.block, "gap": t.gap,
            "dir_key": t.dir_key, "acquisition_segment": t.acq, "power_segment": t.power,
            "audit_identity": t.audit,
            "json_path": str(t.json_path) if t.json_path else None,
            "startup_path": str(t.startup_path) if t.startup_path else None,
            "batchtime_path": str(t.batchtime_path) if t.batchtime_path else None,
            "timing_source": t.timing_source,
            "derived": t.derived,
            "required_failures": t.required_errors,
            "advisory_warnings": t.advisory_warnings,
            "missing_items": t.missing_items,
            "json_sha256": hashes_before.get(str(t.json_path), {}).get("sha256") if t.json_path else None,
        })
    (out_dir / "trials.json").write_text(json.dumps(trials_json, indent=2, ensure_ascii=False) + "\n",
                                         encoding="utf-8")

    (out_dir / "summary.json").write_text(json.dumps(summary, indent=2, ensure_ascii=False) + "\n",
                                          encoding="utf-8")
    (out_dir / "RESULTS.md").write_text(render_md(summary), encoding="utf-8")

    print("P4M review: cases=%d gap_counts=%s online=%d offline=%d"
          % (len(trials), summary["sample_counts"]["per_gap"],
             summary["sample_counts"]["online_pass"], summary["sample_counts"]["offline_retained"]))
    print("P4M review: required_failures=%d advisory_warnings=%d missing_items=%d"
          % (len(summary["audit"]["required_failures"]),
             len(summary["audit"]["advisory_warnings"]),
             len(summary["audit"]["missing_items"])))
    print("P4M review: inputs_unchanged=%s hashed_files=%d"
          % (summary["inputs"]["unchanged"], summary["inputs"]["hashed_file_count"]))
    print("P4M review: outputs=%s" % out_dir)
    return 0 if not summary["audit"]["required_failures"] else 1


def _csv_value(t, key):
    if key == "case_id":
        return t.case_id
    if key == "block_id":
        return t.block
    if key == "gap_cycles":
        return t.gap
    if key == "dir_key":
        return t.dir_key
    if key == "acquisition_segment":
        return t.acq
    if key == "power_segment":
        return t.power
    if key == "audit_identity":
        return t.audit
    if key == "seq_dmesg":
        return t.derived.get("seq")
    if key == "timing_source":
        return t.timing_source
    if key == "json_path":
        return str(t.json_path) if t.json_path else ""
    if key == "startup_path":
        return str(t.startup_path) if t.startup_path else ""
    if key == "batchtime_path":
        return str(t.batchtime_path) if t.batchtime_path else ""
    if key == "required_failures":
        return len(t.required_errors)
    if key == "advisory_warnings":
        return len(t.advisory_warnings)
    return t.derived.get(key)


# ---------------------------------------------------------------------------
# 内存夹具与负向自检（不读写任何原始数据）
# ---------------------------------------------------------------------------
FIXTURE_MAGIC = P4M_BANK_MAGIC  # 与 bench 常量一致，不是猜值


def _fixture_cycles(block_no, gap):
    base = {0: 7_350_000, 1000: 5_310_000, 2000: 4_667_000}[gap]
    return base + block_no * 17


def _fixture_json(block_no, gap, seq):
    cycles = _fixture_cycles(block_no, gap)
    window_ns = cycles * CYCLE_NS
    jobs = {0: 4096, 1000: 2048, 2000: 1364}[gap]
    a_done = jobs
    a_bytes = jobs * CHUNK
    a_rate = a_bytes / (window_ns / 1e9)
    carry_jobs = 1 if gap in (0, 1000) else 0
    flags = 7 if gap in (0, 1000) else 3
    opening = 6 if gap in (0, 1000) else 0
    closing = opening + jobs if opening else 0
    first_done = opening
    last_done = closing - 1 if closing else 0
    a_submitted = jobs + 1400
    raw_window = [FIXTURE_MAGIC, cycles, jobs, a_done,
                  1 if opening else 0, 1 if opening else 0, carry_jobs, 0,
                  opening, closing, first_done, last_done, 0, TRANSFERS, TRANSFERS, flags]
    raw_obs = [0] * 35
    raw_obs[0] = cycles
    raw_obs[1] = cycles - 4095
    raw_obs[3] = 4095
    raw_obs[4] = 2200 + block_no
    raw_obs[6] = 1
    raw_obs[7] = TRANSFERS
    raw_obs[8] = TRANSFERS
    raw_obs[34] = 4321
    return {
        "schema": "idma.p4m.v1",
        "success": True,
        "stage": "run",
        "error": None,
        "errno": 0,
        "run_attempted": True,
        "argv": ["idma_experiment", "--device", "/dev/idma0", "--victim-pages", "1",
                 "--chunk", "4096", "--transfers", "4096", "--warmup", "256",
                 "--timeout-ms", "60000", "--p4m", "--aggressor", "1",
                 "--aggressor-pages", "8", "--a-gap-cycles", str(gap)],
        "input": {"device": "/dev/idma0", "caps_only": False, "abi_version": 1,
                  "struct_size": 328, "flags": 0, "aggressor_id": 1, "victim_pages": 1,
                  "aggressor_pages": 8, "chunk_bytes": CHUNK,
                  "run_timeout_ms": 60000, "victim_transfers": TRANSFERS,
                  "warmup_transfers": 256, "a_gap_cycles": gap,
                  "reserved": [0] * 8},
        "capabilities": {"abi_version": 1, "struct_size": 64, "flags": 255, "device_id": 0,
                         "did": 10, "max_pages": 4096, "page_bytes": 4096,
                         "max_run_timeout_ms": 300000, "did_hpm_supported": True,
                         "software_refill_supported": True, "reserved": [0] * 8},
        "result": {"abi_version": 1, "struct_size": 328, "status": 0, "victim_did": 10,
                   "aggressor_did": 11, "hpm_configured": 1, "measurement_started": 1,
                   "measurement_complete": 1, "victim_elapsed_ns": 1_000_000 + block_no,
                   "snapshot_before_ns": 70_000, "snapshot_after_ns": 90_000,
                   "drain_ns": 74_000,
                   "victim": {"submitted": TRANSFERS, "completed": TRANSFERS,
                              "completed_bytes": V_BYTES, "active_cycles": cycles,
                              "iotlb_miss": 0, "s1_ptw": 0, "s2_ptw": 0,
                              "data_errors": 0, "quarantined": 0},
                   "aggressor": {"submitted": a_submitted, "completed": a_submitted,
                                 "completed_bytes": a_submitted * CHUNK,
                                 "active_cycles": cycles, "iotlb_miss": 5140 + gap,
                                 "s1_ptw": 5140 + gap, "s2_ptw": 0,
                                 "data_errors": 0, "quarantined": 0}},
        "p4m": {"requested_gap_cycles": gap, "sequence": seq, "observer_status": 4,
                "raw_window": raw_window, "raw_observation": raw_obs,
                "bank_magic": FIXTURE_MAGIC, "cycles": cycles, "hardware_window_ns": window_ns,
                "a_issue_window": jobs, "a_done_window": a_done,
                "a_inflight_at_issue": 1 if opening else 0,
                "a_inflight_at_done": 1 if opening else 0,
                "a_started_before_window_completed": carry_jobs,
                "a_issue_opening_edge": 0,
                "a_opening_inflight_id": opening, "a_closing_inflight_id": closing,
                "a_first_done_id": first_done, "a_last_done_id": last_done,
                "task_error_mask": 0, "v_issued": TRANSFERS, "v_done": TRANSFERS,
                "flags": flags, "has_a": True, "closed": True,
                "strict_preopening_inflight": bool(opening), "conservation": True,
                "a_completed_jobs_window": jobs, "a_progress_window_bytes": a_bytes,
                "a_started_before_window_bytes": carry_jobs * CHUNK,
                "a_completed_bytes_per_second": a_rate,
                "a_completed_jobs_per_second": jobs / (window_ns / 1e9),
                "platform_hz": PLATFORM_HZ, "cycle_ns": CYCLE_NS},
    }


def _fixture_timing(block_no, gap, seq):
    a_begin = 1_000_000_000_000 + seq * 5_000_000_000
    a_end = a_begin + 60_000_000
    v_begin = a_begin + 160_000_000
    v_end = v_begin + 150_000_000
    cycles = _fixture_cycles(block_no, gap)
    startup = {"version": "1", "seq": str(seq), "method": "irq_pair", "v_did": "10",
               "a_did": "11", "attempted": "1", "has_a": "1", "a_triggered": "1",
               "v_triggered": "1", "a_id": "1000", "v_id": "1001",
               "a_begin_ns": str(a_begin), "a_end_ns": str(a_end),
               "v_begin_ns": str(v_begin), "v_end_ns": str(v_end),
               "irq_span_ns": str(v_end - a_begin), "a_to_v_ns": str(v_begin - a_begin),
               "error": "0", "_ts_s": 1000.0 + seq}
    batch_time = {"seq": str(seq), "part": "time", "valid": "1", "status": "0x4",
                  "v_did": "10", "a_did": "11", "cycles": str(cycles),
                  "h": str(cycles - 4095), "gap": "4095", "hmax": str(2200 + block_no),
                  "gapmax": "1", "issued": str(TRANSFERS), "done": str(TRANSFERS),
                  "batch_polled": "1", "done_to_read": "4321", "_ts_s": 1000.0 + seq}
    return startup, batch_time


def build_fixture():
    """与真实布局同构的 60 例内存夹具（合成数值，不代表真实观测）。"""
    counters = {}
    trials = []
    for (block_no, gap), dir_key in sorted(EXPECTED_LAYOUT.items()):
        conf = DIR_CONF[dir_key]
        case_id = "formal-b%02d-g%d" % (block_no, gap)
        counters[conf["power"]] = counters.get(conf["power"], 0) + 1
        seq = counters[conf["power"]]
        trial = Trial(case_id=case_id, block_no=block_no, block="b%02d" % block_no, gap=gap,
                      acq=conf["acq"], power=conf["power"],
                      audit=audit_identity(conf["acq"], case_id), dir_key=dir_key)
        trial.json_obj = _fixture_json(block_no, gap, seq)
        skv, bkv = _fixture_timing(block_no, gap, seq)
        trial.startup_kv = skv
        trial.batchtime_kv = bkv
        trial.timing_source = "fixture"
        derive_metrics(trial)
        check = CaseCheck()
        validate_case(trial, check, strict_timing=True)
        trial.required_errors = check.required
        trial.advisory_warnings = check.advisory
        trial.missing_items = check.missing
        trials.append(trial)
    return trials


def run_self_test():
    """三个轻量负向自检，全部在内存夹具上完成，不读写任何原始数据。"""
    failures = []
    lines = []

    good = build_fixture()
    lines.append("self-test: fixture trials=%d（合成数据，未读取任何原始文件）" % len(good))
    ok, errors = check_layout_purity(good)
    if not ok:
        failures.append("T0 正常夹具不应报错：%s" % errors)
    bad_required = [(t.case_id, t.required_errors) for t in good if t.required_errors]
    if bad_required:
        failures.append("T0 正常夹具不应有必需失败：%s" % bad_required[:3])

    # 统计主逻辑：配对视图分类与计数（同一采集段 / 同一电源段 / 跨电源标记）
    _rows, views = build_pairs(good)
    counts = {view: {str(gap): sum(1 for r in rows if r["gap"] == gap) for gap in (1000, 2000)}
              for view, rows in views.items()}
    expected_counts = {"same_acquisition_segment": {"1000": 20, "2000": 18},
                       "same_power_segment": {"1000": 20, "2000": 19},
                       "flagged_cross_power_segment": {"1000": 0, "2000": 1}}
    if counts != expected_counts:
        failures.append("T0 配对视图计数不符：got=%s want=%s" % (counts, expected_counts))
    else:
        lines.append("self-test: pair views = %s" % counts)
    agg = aggregate_pairs(views["same_power_segment"])
    if agg["2000"]["n_pairs"] != 19 or agg["1000"]["n_pairs"] != 20:
        failures.append("T0 配对聚合 n 不符：%s" % {g: agg[g]["n_pairs"] for g in ("1000", "2000")})

    # T1：重复 (block,gap)
    dup = build_fixture()
    dup.append(dup[0])
    ok1, err1 = check_layout_purity(dup)
    if ok1 or not any(e["check"] == "matrix.duplicate_block_gap" for e in err1):
        failures.append("T1 重复样例未被拒绝：ok=%s errors=%s" % (ok1, err1))
    else:
        lines.append("T1 OK：重复 (block,gap) 被拒绝")

    # T2：缺少样例
    missing_fixture = build_fixture()
    missing_fixture = [t for t in missing_fixture if t.case_id != "formal-b20-g2000"]
    ok2, err2 = check_layout_purity(missing_fixture)
    checks2 = {e["check"] for e in err2}
    if ok2 or "matrix.missing_case" not in checks2 or "matrix.gap_count_2000" not in checks2:
        failures.append("T2 缺例未被拒绝：ok=%s errors=%s" % (ok2, err2))
    else:
        lines.append("T2 OK：缺例与 gap 计数被拒绝")

    # T3：错误 cycles 与错误 A 流向
    bad = build_fixture()
    target = [t for t in bad if t.case_id == "formal-b12-g1000"][0]
    target.json_obj["p4m"]["hardware_window_ns"] = target.json_obj["p4m"]["cycles"] * CYCLE_NS + 1
    target.json_obj["p4m"]["a_progress_window_bytes"] = target.json_obj["p4m"]["a_completed_jobs_window"] * CHUNK + CHUNK
    check = CaseCheck()
    validate_case(target, check, strict_timing=True)
    names = {e["check"] for e in check.required}
    if "p4m.hardware_window_ns_eq_cycles_x_20" not in names or \
       "p4m.a_progress_window_bytes_eq_jobs_x_chunk" not in names:
        failures.append("T3 错误 cycles / A 流向未被拒绝：%s" % sorted(names))
    else:
        lines.append("T3 OK：错误 cycles 与错误 A 流向均被拒绝")

    # T4：三目录正式清单与冻结矩阵一致；T5：额外/重复/放错目录必须失败
    actual_manifest = {key: sorted(v) for key, v in expected_manifest().items()}
    ok_manifest, err_manifest = manifest_check(actual_manifest)
    if not ok_manifest:
        failures.append("T4 正常清单不应报错：%s" % err_manifest)
    else:
        lines.append("T4 OK：三目录正式 JSON 清单与冻结矩阵一致")
    bad_manifest = {key: sorted(v) for key, v in expected_manifest().items()}
    bad_manifest["remaining29"] = sorted(bad_manifest["remaining29"] + ["formal-b21-g0.json"])
    bad_manifest["retained6"] = sorted(bad_manifest["retained6"] + ["formal-b01-g0.json"])
    ok_bad, err_bad = manifest_check(bad_manifest)
    names_bad = {e["check"] for e in err_bad}
    if ok_bad or "manifest.unexpected_case" not in names_bad or "manifest.misplaced_case" not in names_bad:
        failures.append("T5 额外/放错目录未被拒绝：ok=%s errors=%s" % (ok_bad, err_bad))
    else:
        lines.append("T5 OK：额外与放错目录的正式 JSON 被拒绝")

    # T6：数值流量被改但 conservation=true 仍拒绝
    bad_flow = build_fixture()
    target = [t for t in bad_flow if t.case_id == "formal-b12-g1000"][0]
    target.json_obj["p4m"]["a_done_window"] = target.json_obj["p4m"]["a_completed_jobs_window"] - 1
    target.json_obj["p4m"]["conservation"] = True
    check = CaseCheck()
    validate_case(target, check, strict_timing=True)
    names_flow = {e["check"] for e in check.required}
    if "p4m.flow_conservation_recomputed" not in names_flow:
        failures.append("T6 改数但 conservation=true 未被拒绝：%s" % sorted(names_flow))
    else:
        lines.append("T6 OK：数值流量与 conservation 不一致被拒绝")

    for line in lines:
        print(line)
    if failures:
        for item in failures:
            print("SELF-TEST FAILURE: %s" % item)
        return 1
    print("self-test: all negative checks behaved as expected; no original file was read or written")
    return 0


# ---------------------------------------------------------------------------
# 入口
# ---------------------------------------------------------------------------
def main(argv=None):
    parser = argparse.ArgumentParser(description="P4M 60 例只读审计与描述统计")
    parser.add_argument("--self-test", action="store_true",
                        help="只在内存夹具上运行负向自检，不读取、不修改任何原始数据")
    parser.add_argument("--out-dir", type=Path, default=REVIEW_DIR,
                        help="输出目录（默认本审阅目录；只允许本目录或其子目录）")
    args = parser.parse_args(argv)
    if args.self_test:
        return run_self_test()
    out_dir = Path(args.out_dir).resolve()
    review_dir = REVIEW_DIR.resolve()
    try:
        out_dir.relative_to(review_dir)
    except ValueError:
        parser.error("--out-dir 必须是 %s 或其子目录（禁止写入原始证据区域）" % review_dir)
    args.out_dir = out_dir
    return run_analysis(args)


if __name__ == "__main__":
    sys.exit(main())
