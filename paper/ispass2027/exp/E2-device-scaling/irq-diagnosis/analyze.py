#!/usr/bin/env python3
"""Read preserved evidence; write only this diagnosis's derived artifacts."""
import hashlib
import json
from pathlib import Path
import re

BASE = Path(__file__).resolve().parent


def main():
    for entry in json.loads((BASE / "snapshot-manifest.json").read_text())["files"]:
        data = (BASE / entry["snapshot"]).read_bytes()
        assert len(data) == entry["read_bytes"] == entry["cutoff_bytes"]
        assert hashlib.sha256(data).hexdigest() == entry["sha256"]
    source_entries = json.loads((BASE / "source-manifest.json").read_text())
    for entry in source_entries:
        data = (BASE / entry["snapshot"]).read_bytes()
        assert hashlib.sha256(data).hexdigest() == entry["sha256"]

    raw = (BASE / "00-uart.raw.log").read_bytes().decode("utf-8", "replace")
    lines = raw.split("\n")  # Match file:line locators; retain CR bytes in original.
    samples = []
    for i, line in enumerate(lines):
        match = re.search(r"\[\s*([\d.]+)\] watchdog: BUG: soft lockup .*?for (\d+)s! \[(.*?):(\d+)\]", line)
        if not match:
            continue
        block = "\n".join(lines[i:i + 29])
        epc = re.search(r"epc : ([0-9a-f]{16}) ra :", block)
        cause = re.search(r"cause: ([0-9a-f]{16})", block)
        pending = re.search(r"s11:\s*([0-9a-f]{16})", block)
        assert epc and cause and pending
        samples.append(dict(line=i + 1, timestamp_s=float(match[1]), duration_s=int(match[2]),
                            task=match[3], pid=int(match[4]), epc=epc[1], cause=cause[1],
                            pending_softirq_mask=int(pending[1], 16)))
    assert len(samples) == 7
    assert all(s["epc"] == "ffffffff808a4e18" and s["cause"] == "8000000000000005"
               and s["pending_softirq_mask"] == 2 for s in samples)

    console = (BASE / "console02/command-0514e2d6078b4acf9f600372033bbaf1.stdout.txt").read_text()
    def vals(pattern):
        found = [int(x) for x in re.findall(pattern, console, re.M)]
        assert len(found) == 2, (pattern, found)
        return found
    now = vals(r"^now at (\d+) nsecs$")
    jiffies = vals(r"^jiffies: (\d+)$")
    irqs = vals(r"^\s*11:\s*(\d+).*riscv-timer$")
    events = vals(r"^\s*\.nr_events\s*:\s*(\d+)$")
    retries = vals(r"^\s*\.nr_retries\s*:\s*(\d+)$")
    hangs = vals(r"^\s*\.nr_hangs\s*:\s*(\d+)$")
    max_hang = vals(r"^\s*\.max_hang_time\s*:\s*(\d+)$")
    idle = vals(r"^\s*\.idle_sleeptime\s*:\s*(\d+) nsecs$")
    ctxt = vals(r"^ctxt (\d+)$")
    delta_s = (now[1] - now[0]) / 1e9
    old_stdout = (BASE / "03-command-fa2f354e990b4f75a77692b55a203131.stdout.txt").read_text()
    decoder = json.JSONDecoder()
    old, _ = decoder.raw_decode(old_stdout[old_stdout.index('{"schema":"idma.multi_batch.v1"'):])
    old_result = old["result"]
    hw_cycles, done_to_read_cycles = 13105262, 1136467216
    assert f"cycles={hw_cycles} " in old_stdout
    assert f"done_to_read={done_to_read_cycles}" in old_stdout
    new_manifest = json.loads((BASE / "01-manifest.json").read_text())
    assert new_manifest["runtime_commits"]["linux"] == "65b427946152e3aca27babab0a437821729805d4"
    assert new_manifest["recorded_cases"] == 5 and new_manifest["status"] == "failed"
    result = {
        "scope": "offline source/log diagnosis only; no hardware commands or experiment rerun",
        "snapshot_validation": "all captured byte lengths and SHA256 match",
        "fixed_source_files_validated": len(source_entries),
        "new_watchdog_samples": samples,
        "new_unique_hrtimer_warning_ns": sorted(set(int(x) for x in re.findall(r"hrtimer: interrupt took (\d+) ns", raw))),
        "post_ctrl_c": {
            "now_ns": now, "jiffies": jiffies, "timer_irq": irqs, "nr_events": events,
            "nr_retries": retries, "nr_hangs": hangs, "max_hang_time_ns_as_exported": max_hang,
            "idle_sleeptime_ns": idle, "context_switches": ctxt,
            "delta_timer_list_now_s": delta_s,
            "jiffies_delta": jiffies[1] - jiffies[0],
            "jiffies_per_timer_list_second": (jiffies[1] - jiffies[0]) / delta_s,
            "timer_irq_delta": irqs[1] - irqs[0],
            "nr_events_delta": events[1] - events[0],
            "nr_hangs_delta": hangs[1] - hangs[0],
            "nr_retries_delta": retries[1] - retries[0],
            "idle_delta_s": (idle[1] - idle[0]) / 1e9,
            "context_switch_delta": ctxt[1] - ctxt[0],
            "not_atomic": True,
            "no_independent_host_wallclock_frequency_calibration": True
        },
        "old_failed_point": {
            "victim_hw_cycles": hw_cycles, "victim_hw_s_at_50MHz": hw_cycles / 50e6,
            "victim_sw_s": old_result["victim_elapsed_ns"] / 1e9,
            "done_to_read_s_at_50MHz": done_to_read_cycles / 50e6,
            "aggressor_completions": [s["completed"] for s in old_result["stats"][1:]],
            "status": old_result["status"],
            "accepted": False
        },
        "limits": ["No fault-window IRQ entry/rearm timing or CPU AXI latency trace exists here",
                   "Hrtimer retry counters are cumulative, not attributable solely to N4_A8",
                   "Post-CTRL-C counters do not prove STOP/drain/data completion",
                   "Timer_list now and jiffies share the same time source; host wallclock calibration remains absent"]
    }
    assert retries[0] == retries[1] and hangs[0] == hangs[1]
    (BASE / "calculations.json").write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n")
    print(json.dumps({"validation": "passed", "source_files": len(source_entries),
                      "watchdog_samples": len(samples), "recovered_jiffies_hz": result["post_ctrl_c"]["jiffies_per_timer_list_second"],
                      "hangs_delta": hangs[1] - hangs[0]}, indent=2))


if __name__ == "__main__":
    main()
