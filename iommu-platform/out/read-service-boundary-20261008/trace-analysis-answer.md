下面给出一份**未执行**的静态实现提案：单文件 Python3 标准库脚本，CLI 为 `--root DIR --output FILE`。它只读取 `serial-p.log`、`serial-pn.log`、`serial-p-events.csv`、`serial-pn-events.csv`，不修改项目，写出 JSON。脚本会在硬不匹配时直接报错；FIFO 初始 pending 未知处只假设 0，并在不验证时输出 diagnostic，不伪造初始值。

已按给定日志手工核对到的服务账本关系：

- P：`serial-p.log:22` READ-SERVICE-WINDOW-JSON：`empty=4351 setup=19638 transfer=67901 backpressure=219`，sum=`92109`=`W_cycles`；`serial-p.log:11` window：`ddr_r=67901 ddr_r_stall=219 fast=0 fast_bank0=0 fast_bank1=0`。
- PN：`serial-pn.log:22`：`empty=4592 setup=19607 transfer=67893 backpressure=282`，sum=`92374`=`W_cycles`；`serial-pn.log:11` window：`ddr_r=67893 ddr_r_stall=282 fast=62 fast_bank0=62 fast_bank1=0`。
- PN-P 四类差：`empty +241, setup -31, transfer -8, backpressure +63`，sum=`265`=`Wdiff 92374-92109`。注意：这是服务账本差值，不是独立因果结论。
- FAST 终端：PN `READ-ADMIT-WINDOW-JSON` 的 `fastread_captures=62`，与 `window.fast=62` 一致；P 为 0。脚本也会同时保留 `READ-TAG-TERMINAL-JSON`。

## 主要假设与边界

1. CSV 的 `cycle` 视为相对 `win_open`；窗口取 `0 < cycle <= W`，opening cycle 0 不计，closing cycle W 计入。
2. `newcaptured` 的 source 取 `tag & 0x0F`；`bit4` 为 bank，`bit5` 为 FAST。
3. `arfire` 的 `tag` 直接作为 original source id。
4. `ddr_ar` 物理数据读：`port = (tag >> 4) & 3`，`low4 = tag & 0x0F`；仅 `port==2 and low4 in (0,4)` 计入 DMA data。
5. `fast_response` 的 `tag` 按 bank 处理；脚本要求 bank0/bank1 计数与 window 一致。
6. FAST 配对按同源 FAST newcaptured 出现顺序与 bank0 fast_response 顺序一一配对；时间非负检查采用 `fast_response.cycle <= newcaptured.cycle <= arfire.cycle <= ddr_ar.cycle`。
7. FIFO empty 重建使用全部 `ddr_ar` 与 `ddr_r_last` 事件，按 cycle 分组、按文件内同 cycle 顺序处理；假设窗口第一 cycle 前 pending=0。若 pending<0 或重建 empty 数 != READ-SERVICE-WINDOW.empty，则 `verified=false` 并输出 diagnostic。
8. 事件 cycle 差仅表示观察到的寄存器/事件时序差，不直接当成 copy 耗时，也不据此断言 IOMMU/DMA 瓶颈。

## 建议脚本

```python
#!/usr/bin/env python3
import argparse
import csv
import json
import os
import re
import statistics
from collections import defaultdict

SERVICE_CATS = ("empty", "setup", "transfer", "backpressure")
OBS_EVENTS = {
    "fast_response", "newcaptured", "arfire", "aw_wait_change",
    "ptw_start", "ptw_end", "dma_done",
}
NEAR = 5


def die(msg):
    raise SystemExit("ERROR: " + msg)


def parse_log(path):
    out = {}
    rx = re.compile(r'^([A-Z0-9-]+-JSON)\s+(\{.*\})\s*$')
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.rstrip("\n")
            m = rx.match(line)
            if m:
                key = m.group(1)
                try:
                    out[key] = json.loads(m.group(2))
                except json.JSONDecodeError as e:
                    die(f"{path}: bad JSON for {key}: {e}")
    return out


def read_events(path):
    rows = []
    with open(path, newline="", encoding="utf-8", errors="replace") as f:
        r = csv.DictReader(f)
        if r.fieldnames != ["cycle", "event", "tag", "address", "value"]:
            die(f"{path}: unexpected header {r.fieldnames}")
        for i, row in enumerate(r, start=2):
            try:
                rows.append({
                    "cycle": int(row["cycle"]),
                    "event": row["event"].strip(),
                    "tag": int(row["tag"]),
                    "address": int(row["address"]),
                    "value": int(row["value"]),
                })
            except Exception as e:
                die(f"{path}:{i}: bad row {row!r}: {e}")
    return rows


def stats(vals):
    vals = sorted(vals)
    if not vals:
        return {"count": 0, "min": None, "median": None, "max": None}
    return {
        "count": len(vals),
        "min": vals[0],
        "median": statistics.median(vals),
        "max": vals[-1],
    }


def get_log(log, key, path):
    if key not in log:
        die(f"{path}: missing {key}")
    return log[key]


def service_check(rsw, adw, path):
    W = adw.get("W_cycles")
    win = adw.get("window")
    if not isinstance(W, int) or not isinstance(win, dict):
        die(f"{path}: ACTUAL-DMA-WINDOW-JSON missing W_cycles/window")
    s = sum(rsw[c] for c in SERVICE_CATS)
    if s != W:
        die(f"{path}: service sum {s} != W_cycles {W}")
    if rsw["transfer"] != win.get("ddr_r"):
        die(f"{path}: transfer {rsw['transfer']} != window.ddr_r {win.get('ddr_r')}")
    if rsw["backpressure"] != win.get("ddr_r_stall"):
        die(f"{path}: backpressure {rsw['backpressure']} != window.ddr_r_stall {win.get('ddr_r_stall')}")
    return {
        "W_cycles": W,
        "sum": s,
        "sum_equals_W": True,
        "transfer_equals_window_ddr_r": True,
        "backpressure_equals_window_ddr_r_stall": True,
    }


def is_phys_data_ddr_ar(ev):
    tag = ev["tag"]
    return ((tag >> 4) & 3) == 2 and (tag & 0x0F) in (0, 4)


def match_source(events, source, W, label):
    news, ars, ddrs = [], [], []
    for ev in events:
        if not (0 < ev["cycle"] <= W):
            continue
        if ev["event"] == "newcaptured":
            if (ev["tag"] & 0x0F) == source:
                news.append(ev)
        elif ev["event"] == "arfire":
            if (ev["tag"] & 0x0F) == source:
                ars.append(ev)
        elif ev["event"] == "ddr_ar" and is_phys_data_ddr_ar(ev):
            if (ev["tag"] & 0x0F) == source:
                ddrs.append(ev)

    if not (len(news) == len(ars) == len(ddrs)):
        die(f"{label} source {source}: count mismatch "
            f"newcaptured={len(news)} arfire={len(ars)} data_ddr_ar={len(ddrs)}")

    pairs = []
    for i, (n, a, d) in enumerate(zip(news, ars, ddrs)):
        if not (n["address"] == a["address"] == d["address"]):
            die(f"{label} source {source} idx {i}: address mismatch "
                f"{n['address']} {a['address']} {d['address']}")
        if not (n["value"] == a["value"] == d["value"] == 255):
            die(f"{label} source {source} idx {i}: burst len mismatch "
                f"{n['value']} {a['value']} {d['value']}")
        if not (n["cycle"] <= a["cycle"] <= d["cycle"]):
            die(f"{label} source {source} idx {i}: non-monotonic cycles "
                f"{n['cycle']} {a['cycle']} {d['cycle']}")
        pairs.append({"new": n, "ar": a, "ddr": d})
    return pairs


def pair_stats(pairs):
    return {
        "capture_to_arfire": stats([p["ar"]["cycle"] - p["new"]["cycle"] for p in pairs]),
        "capture_to_ddr_ar": stats([p["ddr"]["cycle"] - p["new"]["cycle"] for p in pairs]),
        "arfire_to_ddr_ar": stats([p["ddr"]["cycle"] - p["ar"]["cycle"] for p in pairs]),
    }


def fast_pairing(events, W, v_pairs, win, label):
    fast_resp = [ev for ev in events if 0 < ev["cycle"] <= W and ev["event"] == "fast_response"]
    other = [ev for ev in fast_resp if ev["tag"] not in (0, 1)]
    if other:
        die(f"{label}: fast_response unexpected bank tags: "
            f"{json.dumps(other[:5], ensure_ascii=False)}")

    bank0 = [ev for ev in fast_resp if ev["tag"] == 0]
    bank1 = [ev for ev in fast_resp if ev["tag"] == 1]
    if len(bank1) != win.get("fast_bank1", 0):
        die(f"{label}: fast_response bank1 count {len(bank1)} != "
            f"window.fast_bank1 {win.get('fast_bank1')}")
    if len(bank0) != win.get("fast_bank0", 0):
        die(f"{label}: fast_response bank0 count {len(bank0)} != "
            f"window.fast_bank0 {win.get('fast_bank0')}")

    fast_new = [
        p for p in v_pairs
        if ((p["new"]["tag"] >> 5) & 1) == 1 and ((p["new"]["tag"] >> 4) & 1) == 0
    ]
    if len(fast_new) != len(bank0):
        die(f"{label}: FAST newcaptured count {len(fast_new)} != "
            f"fast_response bank0 count {len(bank0)}")

    paired = []
    missing = []
    for i, (fr, p) in enumerate(zip(bank0, fast_new)):
        n, a, d = p["new"], p["ar"], p["ddr"]
        deltas = {
            "to_newcaptured": n["cycle"] - fr["cycle"],
            "to_arfire": a["cycle"] - fr["cycle"],
            "to_ddr_ar": d["cycle"] - fr["cycle"],
        }
        if min(deltas.values()) < 0:
            missing.append({
                "idx": i,
                "fast_response": fr,
                "new": n,
                "ar": a,
                "ddr": d,
                "deltas": deltas,
            })
        paired.append({"idx": i, "fast_response": fr, "new": n, "ar": a, "ddr": d, **deltas})

    if missing:
        die(f"{label}: FAST pairing has negative deltas/missing: "
            f"{json.dumps(missing[:5], ensure_ascii=False)}")

    first = [p for p in paired if p["new"]["address"] == 0x80000000]
    second = [p for p in paired if p["new"]["address"] == 0x80000800]
    if len(first) + len(second) != len(paired):
        die(f"{label}: FAST newcaptured has unexpected address "
            f"(expected 0x80000000/0x80000800)")

    def gstats(group):
        return {
            "to_newcaptured": stats([p["to_newcaptured"] for p in group]),
            "to_arfire": stats([p["to_arfire"] for p in group]),
            "to_ddr_ar": stats([p["to_ddr_ar"] for p in group]),
        }

    return {
        "fast_response_total": len(fast_resp),
        "fast_response_bank0": len(bank0),
        "fast_response_bank1": len(bank1),
        "fast_newcaptured_bank0": len(fast_new),
        "paired": len(paired),
        "all": gstats(paired),
        "first_burst": gstats(first),
        "second_burst": gstats(second),
    }


def reconstruct_fifo(events, W, measured_empty, label):
    by = defaultdict(list)
    for ev in events:
        if 0 < ev["cycle"] <= W and ev["event"] in ("ddr_ar", "ddr_r_last"):
            by[ev["cycle"]].append(ev)

    cycles = sorted(by)
    pending = 0
    intervals = []
    empty_start = None
    cur = 1
    diag = []
    negative = False

    for c in cycles:
        if pending == 0:
            if empty_start is None:
                empty_start = cur
        else:
            if empty_start is not None:
                intervals.append((empty_start, cur - 1))
                empty_start = None

        for ev in by[c]:
            if ev["event"] == "ddr_ar":
                pending += 1
            elif ev["event"] == "ddr_r_last":
                pending -= 1
            if pending < 0:
                diag.append(
                    f"pending<0 at cycle {c} after {ev['event']} "
                    f"tag={ev['tag']}: {pending}"
                )
                negative = True
                break

        if negative:
            break

        if pending > 0 and empty_start is not None:
            intervals.append((empty_start, c))
            empty_start = None
        cur = c + 1

    if not negative:
        if pending == 0:
            if empty_start is None:
                empty_start = cur
            if empty_start <= W:
                intervals.append((empty_start, W))
        else:
            if empty_start is not None:
                intervals.append((empty_start, cur - 1))

    total_empty = sum(b - a + 1 for a, b in intervals)
    if total_empty != measured_empty:
        diag.append(
            f"reconstructed empty {total_empty} != measured empty {measured_empty}"
        )

    verified = (not negative) and (total_empty == measured_empty)
    obs = [ev for ev in events if ev["event"] in OBS_EVENTS]
    longest = []
    for a, b in sorted(intervals, key=lambda x: (x[1] - x[0] + 1, -x[0]), reverse=True)[:8]:
        near = [ev for ev in obs if a - NEAR <= ev["cycle"] <= b + NEAR]
        longest.append({
            "start": a,
            "end": b,
            "length": b - a + 1,
            "nearby_events": near,
        })

    return {
        "initial_pending_assumption": 0,
        "boundary": "window cycles 1..W inclusive; opening cycle0 excluded; "
                    "pre-edge state before events at cycle",
        "reconstructed_empty_cycles": total_empty,
        "measured_empty": measured_empty,
        "verified": verified,
        "diagnostics": diag,
        "longest_empty_intervals": longest,
        "note": "If verified=false, do not use this reconstruction as causal evidence; "
                "service counts above remain valid.",
    }


def load_one(root, tag):
    log_path = os.path.join(root, f"serial-{tag}.log")
    csv_path = os.path.join(root, f"serial-{tag}-events.csv")

    log = parse_log(log_path)
    events = read_events(csv_path)

    rsw = get_log(log, "READ-SERVICE-WINDOW-JSON", log_path)
    adw = get_log(log, "ACTUAL-DMA-WINDOW-JSON", log_path)
    admw = log.get("READ-ADMIT-WINDOW-JSON")
    rtt = log.get("READ-TAG-TERMINAL-JSON")
    win = adw["window"]

    if admw is not None and "fastread_captures" in admw:
        if admw["fastread_captures"] != win.get("fast"):
            die(f"{log_path}: READ-ADMIT-WINDOW fastread_captures "
                f"{admw['fastread_captures']} != window.fast {win.get('fast')}")

    sc = service_check(rsw, adw, log_path)
    W = sc["W_cycles"]

    v_pairs = match_source(events, 0, W, tag)
    a_pairs = match_source(events, 4, W, tag)
    fast = fast_pairing(events, W, v_pairs, win, tag)
    fifo = reconstruct_fifo(events, W, rsw["empty"], tag)

    return {
        "log_path": log_path,
        "events_path": csv_path,
        "read_service_window": rsw,
        "actual_dma_window": adw,
        "fast_tag_terminal": {
            "READ-ADMIT-WINDOW-JSON": admw,
            "READ-TAG-TERMINAL-JSON": rtt,
        },
        "service_check": sc,
        "event_matching": {
            "source_0_V": {"count": len(v_pairs), "stats": pair_stats(v_pairs)},
            "source_4_A": {"count": len(a_pairs), "stats": pair_stats(a_pairs)},
            "v_delay_stats": pair_stats(v_pairs),
        },
        "fast_pairing": fast,
        "fifo_empty_reconstruction": fifo,
    }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", required=True)
    ap.add_argument("--output", required=True)
    args = ap.parse_args()

    p = load_one(args.root, "p")
    pn = load_one(args.root, "pn")

    diff = {
        c: pn["read_service_window"][c] - p["read_service_window"][c]
        for c in SERVICE_CATS
    }
    Wdiff = pn["service_check"]["W_cycles"] - p["service_check"]["W_cycles"]
    if sum(diff.values()) != Wdiff:
        die(f"service diff sum {sum(diff.values())} != Wdiff {Wdiff}")

    out = {
        "schema": "read-service-boundary-v1",
        "notes": [
            "READ-SERVICE-WINDOW categories are a service ledger; "
            "transfer==window.ddr_r and backpressure==window.ddr_r_stall are equality checks, "
            "not independent causal proofs.",
            "Event-cycle differences are observed event timing, not copy duration.",
            "Only ddr_ar port2 low4bit 0/4 are treated as physical DMA data reads; "
            "CPU/PTW are not counted as DMAdata.",
            "FIFO empty reconstruction assumes initial pending=0; if pending<0 or "
            "reconstructed empty != measured empty, treat as unverified diagnostic, not causal.",
        ],
        "p": p,
        "pn": pn,
        "pn_minus_p_service_diff": {
            "categories": diff,
            "sum": sum(diff.values()),
            "Wdiff": Wdiff,
            "sum_equals_Wdiff": sum(diff.values()) == Wdiff,
            "note": "service ledger difference, not independent causal attribution",
        },
        "pn_minus_p_v_delay_stats": {
            k: {
                "pn_minus_p_median": (
                    pn["event_matching"]["v_delay_stats"][k]["median"]
                    - p["event_matching"]["v_delay_stats"][k]["median"]
                )
                if p["event_matching"]["v_delay_stats"][k]["median"] is not None
                and pn["event_matching"]["v_delay_stats"][k]["median"] is not None
                else None
            }
            for k in ("capture_to_arfire", "capture_to_ddr_ar", "arfire_to_ddr_ar")
        },
    }

    with open(args.output, "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=2)
    print(f"wrote {args.output}")


if __name__ == "__main__":
    main()
```

## 建议验收检查

1. 运行：
   ```bash
   python3 this_script.py \
     --root out/read-service-boundary-20261008 \
     --output out/read-service-boundary-20261008/read-service-boundary.json
   ```
2. 预期服务检查通过：
   - P sum 92109，transfer=67901，backpressure=219。
   - PN sum 92374，transfer=67893，backpressure=282。
   - PN-P diff sum 265。
3. 预期 source FIFO 匹配：
   - P/PN 每个 source 的 `newcaptured`、`arfire`、物理 data `ddr_ar` 计数应一致；若不一致脚本报具体 count mismatch。
   - 地址、burst len=255、`newcaptured.cycle <= arfire.cycle <= ddr_ar.cycle` 任一不满足即报错。
4. FAST：
   - P 期望 0；PN 期望 bank0=62、bank1=0、FAST newcaptured bank0=62。
   - FAST 配对时间若出现负值，脚本输出具体 idx/事件，不静默修正。
5. FIFO empty：
   - 若初始 pending=0 假设正确，`reconstructed_empty_cycles` 应等于 READ-SERVICE-WINDOW.empty：P=4351，PN=4592。
   - 若 pending 变负或 empty 不等，输出 `verified=false` 和 diagnostics；不要把该重建用于因果结论。
