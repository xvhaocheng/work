#!/usr/bin/env python3
"""Build source-bound ISPASS tables without changing historical evidence.

Only the Python standard library is used. source_locator is an RFC 6901 JSON
pointer, or a JSON expression with explicit source operands for derived ratios.
"""
from __future__ import annotations

import argparse
import collections
import csv
import hashlib
import io
import json
import math
from pathlib import Path
import re
import statistics

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
OUT = ROOT / "iommu-platform/out"
FIELDS = ["row_id", "experiment", "condition", "metric", "statistic", "value",
          "unit", "n", "bitstream_id", "kernel_id", "raw_path", "source_locator",
          "evidence_level", "value_status", "identity_status", "boot_id", "sample_set"]
STATS = ("min", "median", "max", "q1", "q3", "iqr")


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def encoded(value):
    return json.dumps(value, ensure_ascii=False, indent=2, sort_keys=True) + "\n"


def pointer(*parts):
    return "/" + "/".join(str(p).replace("~", "~0").replace("/", "~1") for p in parts)


def locate(obj, ptr):
    if ptr == "":
        return obj
    assert ptr.startswith("/"), ptr
    for part in ptr[1:].split("/"):
        key = part.replace("~1", "/").replace("~0", "~")
        obj = obj[int(key)] if isinstance(obj, list) else obj[key]
    return obj


def slug(text):
    return re.sub(r"[^a-zA-Z0-9_.-]+", "-", str(text)).strip("-")


def unit(metric):
    low = metric.lower()
    if "mib_s" in low:
        return "MiB/s"
    if "percent" in low or "pct" in low:
        return "%"
    if "ratio" in low or "slowdown" in low or "over_m0" in low:
        return "ratio"
    if low.endswith("_ms") or "_ms_" in low:
        return "ms"
    if low.endswith("_ns") or "_ns_" in low:
        return "ns"
    if "bytes" in low:
        return "bytes"
    if low == "cq_entry_cycles":
        return "command_simulation_steps"
    if low in {"t_a", "t_b", "t_end", "a_final_flush_end", "cq_total_waiting"}:
        return "simulation_steps"
    if "cycle" in low or low in {"w", "a_remaining", "v_remaining", "v_wait", "v_own"}:
        return "cycles"
    return "count"



def csv_text(rows):
    output = io.StringIO(newline="")
    writer = csv.DictWriter(output, FIELDS, lineterminator="\n")
    writer.writeheader()
    writer.writerows(sorted(rows, key=lambda row: row["row_id"]))
    return output.getvalue()
