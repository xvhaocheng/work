#!/usr/bin/env python3
"""Replay historical stdout through collect.parse/review; never opens UART."""
import collections
import copy
import hashlib
import json
from pathlib import Path
import re
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
COLLECTOR = ROOT / "paper/ispass2027/board/collect.py"
SOURCE = COLLECTOR.read_text()
SOURCE_LINES = SOURCE.splitlines()
sys.path.insert(0, str(COLLECTOR.parent))
NS = {"__file__": str(COLLECTOR), "__name__": "collector_compatibility_under_test"}
exec(compile(SOURCE, str(COLLECTOR), "exec"), NS)
BOOT = "00000000-0000-0000-0000-000000000000"
OUT = ROOT / "iommu-platform/out"
report = {"schema_version": 1,
          "scope": "Offline replay of original CLI and observer lines; injected required identity envelope is test-only, not historical identity evidence",
          "PRE_SEQ_fixture_note": "Historical fixtures simulate the new envelope with PRE_SEQ=the genuine current observer sequence minus one. This marker was not present in the historical CLI output. Stale-observer negative probes retain the current case watermark.",
          "collector_sha256": hashlib.sha256(SOURCE.encode()).hexdigest(),
          "hardware_operations": 0, "inputs": [], "campaigns": {}, "review_tests": {}, "association_probes": []}


def read(path):
    report["inputs"].append({"path": str(path.relative_to(ROOT)),
                             "sha256": hashlib.sha256(path.read_bytes()).hexdigest()})
    return json.loads(path.read_text())


def mode(record):
    return "Solo" if record["result"]["aggressor"] is None else "A"+str(record["input"]["aggressor_pages"])


def observe_lines(serial):
    return [(int(match.group(1)), line) for line in serial.replace("\r", "").splitlines()
            if (match := re.search(r"idma_(?:batch|path)_obs seq=(\d+)", line))]


def stdout(cli_line, observer_lines, kernel, pre_seq):
    return "\n".join(["KERNEL="+kernel, "BOOT="+BOOT, "TAINT=0", "PRE_SEQ="+str(pre_seq), cli_line,
                      "CASE_EXIT=0", *observer_lines, "TAINT_AFTER=0", ""])


def attempt(text, kernel, paths):
    try:
        value = NS["parse"](text, kernel, paths)
        return {"accepted": True, "sequence": value["sequence"],
                "path_count": len(value["path"]), "hardware_ms": value["metrics"]["hardware_ms"]}, value
    except (AssertionError, KeyError) as exc:
        tb = exc.__traceback__
        relevant = None
        while tb:
            if tb.tb_frame.f_code.co_filename == str(COLLECTOR):
                relevant = tb.tb_lineno
            tb = tb.tb_next
        return {"accepted": False, "error_type": type(exc).__name__, "error": str(exc),
                "collector_line": relevant,
                "collector_expression": SOURCE_LINES[relevant-1].strip() if relevant else None}, None


accepted_for_review = []
for label, directory, kernel, expect_path in [
        ("batch_main", "mac-batch-main-20260916-112954-run-9sa69rhc", "6.5.0-rc2-00082-gb7979a1a06ac", False),
        ("path_main", "mac-path-main-20260916-164633-run-vvguros8", "6.5.0-rc2-00083-g45bf76b15e36", True)]:
    folder = OUT / directory
    records = read(folder / "records.json")
    metadata = read(folder / "summary.json")
    review_script = folder / "server-review.py"
    report["inputs"].append({"path": str(review_script.relative_to(ROOT)),
                             "sha256": hashlib.sha256(review_script.read_bytes()).hexdigest()})
    serial = metadata["raw_serial"].replace("\r", "")
    cli_lines = [line for line in serial.splitlines() if line.startswith('{"schema":"idma.batch.v1"')]
    observations = observe_lines(serial)
    sequences = sorted({seq for seq, line in observations if "idma_batch_obs" in line})
    rows, own_inputs = [], []
    for index, item in enumerate(records):
        record = item["record"]
        sequence = item.get("sequence", sequences[index])
        matching = [line for line in cli_lines if json.loads(line) == record]
        assert len(matching) == 1, (label, item["case"], len(matching))
        own = [line for seq, line in observations if seq == sequence]
        cumulative = [line for seq, line in observations if seq <= sequence]
        parsed, value = attempt(stdout(matching[0], own, kernel, sequence-1), kernel, expect_path)
        accumulated, accumulated_value = attempt(stdout(matching[0], cumulative, kernel, sequence-1), kernel, expect_path)
        entry = {"case": item["case"], "device": record["input"]["device"],
                 "condition": mode(record), "expected_sequence": sequence,
                 "single_sequence": parsed, "cumulative_dmesg": accumulated,
                 "cumulative_max_sequence": max(seq for seq, line in observations if seq <= sequence)}
        entry["expected_fixed_role_rejection"] = record["input"]["device"] != "/dev/idma0"
        if expect_path:
            # These are the exact conditions in the historical server-review.
            entry["A_area_not_equal_sum_fields"] = [
                {k: p[k] for k in ("role", "channel", "stage", "entered", "full", "carry", "start", "end", "area", "sum")}
                for p in item["path"] if p["role"] == "a" and p["area"] != p["sum"]]
            for p in item["path"]:
                assert p["start"]+p["entered"] == p["full"]+p["carry"]+p["end"]
                assert p["carry"] <= p["start"] and p["full"] <= p["entered"]
                assert p["max"] <= p["sum"] <= p["full"]*p["max"]
                assert p["area"] >= p["sum"]
                if p["role"] == "v":
                    assert p["area"] == p["sum"] and p["start"] == p["end"] == p["carry"] == 0
                if p["stage"] < 3:
                    following = next(q for q in item["path"] if (q["role"], q["channel"], q["stage"]) == (p["role"], p["channel"], p["stage"]+1))
                    assert p["full"]+p["carry"] == following["entered"]
            entry["historical_path_conservation_passed"] = True
            entry["nonempty_zero_duration_path_fields"] = [
                {k:p[k] for k in ("role", "channel", "stage", "full", "sum", "max")}
                for p in item["path"] if p["full"] > 0 and p["sum"] == 0]
        if value:
            assert parsed["sequence"] == sequence
            assert value["observer"] == item["observer"]
            assert accumulated_value["observer"] == item["observer"]
            assert value["metrics"]["hardware_cycles"] == item["observer"]["time"]["cycles"]
            if expect_path:
                assert set(value["path"]) == {p["role"]+p["channel"]+str(p["stage"]) for p in item["path"]}
                for p in item["path"]:
                    key = "path_"+p["role"]+p["channel"]+str(p["stage"])+"_mean_cycles"
                    assert value["metrics"][key] == (p["sum"]/p["full"] if p["full"] else None)
            value.update(condition=mode(record))
            if label == "batch_main":
                accepted_for_review.append(value)
        own_inputs.append((record, sequence, matching[0], own))
        rows.append(entry)
    report["campaigns"][label] = {
        "cases": len(rows), "accepted": sum(r["single_sequence"]["accepted"] for r in rows),
        "expected_fixed_role_rejections": sum(r["expected_fixed_role_rejection"] for r in rows),
        "unexpected_rejections": sum(not r["single_sequence"]["accepted"] and not r["expected_fixed_role_rejection"] for r in rows),
        "accepted_sequences_match_own_record": True,
        "case_results": rows}
    # Association failure probe: preserve both genuine sources' fields but omit
    # the current sequence. No fabricated counter value is introduced.
    for later in own_inputs:
        previous = next((p for p in own_inputs if p[1] < later[1] and p[0]["input"]["device"] == later[0]["input"]["device"] == "/dev/idma0" and mode(p[0]) == mode(later[0]) and p[0]["result"]["victim_elapsed_ns"] != later[0]["result"]["victim_elapsed_ns"]), None)
        if previous:
            result, _ = attempt(stdout(later[2], previous[3], kernel, later[1]-1), kernel, expect_path)
            report["association_probes"].append({"campaign": label,
                "probe": "current original CLI plus earlier same-condition genuine observer lines; current observer deliberately absent",
                "current_sequence": later[1], "supplied_observer_sequence": previous[1], **result})
            break


def g1_input(n, factor=None):
    selected = []
    for condition in NS["REFERENCE_MS"]:
        group = [copy.deepcopy(r) for r in accepted_for_review if r["condition"] == condition][:n]
        assert len(group) == n
        if factor is not None:
            for row in group:
                row["metrics"]["hardware_ms"] = NS["REFERENCE_MS"][condition]*factor
        selected.extend(group)
    return selected


for name, records in [("historical_three_per_condition", g1_input(3)),
                      ("two_per_condition", g1_input(2)),
                      ("missing_A8", [r for r in g1_input(3) if r["condition"] != "A8"]),
                      ("synthetic_exact_upper_two_percent_boundary", g1_input(3, 1.02)),
                      ("synthetic_exact_lower_two_percent_boundary", g1_input(3, .98)),
                      ("synthetic_inside_tolerance", g1_input(3, 1.0199)),
                      ("synthetic_outside_tolerance", g1_input(3, 1.0201))]:
    report["review_tests"][name] = NS["review"](records, 3, True)

report["real_prefix_cases"] = []
actual_failed = ROOT / "paper/ispass2027/board/runs/g1-20261017-01"
for path in sorted((actual_failed / "uart").glob("command-*.json")):
    command = read(path)
    text = command.get("stdout", "").replace("\r", "")
    marker = text.find('{"schema":"idma.batch.v1"')
    if marker < 0 or marker == 0 or text[marker-1] == "\n":
        continue
    kernel = re.search(r"^KERNEL=(.+)$", text, re.M).group(1)
    document, end = json.JSONDecoder().raw_decode(text, marker)
    parsed, value = attempt(text, kernel, False)
    duplicate, _ = attempt(text+"\n"+text[marker:end]+"\n", kernel, False)
    result = {"raw_path": str(path.relative_to(ROOT)), "schema_offset": marker,
              "same_line_prefix": text[text.rfind("\n", 0, marker)+1:marker],
              "actual_output_without_envelope_changes": True,
              "parse": parsed, "duplicate_CLI_negative": duplicate}
    if value:
        assert value["record"] == document
        assert value["sequence"] == 1 and value["metrics"]["hardware_ms"] == 56.8054
        assert value["record"]["result"]["victim"]["completed"] == 4096
        assert value["record"]["result"]["victim"]["iotlb_miss"] == 0
    report["real_prefix_cases"].append(result)

actual_run = ROOT / "paper/ispass2027/board/runs/g1-20261017-03"
actual_manifest = read(actual_run/"manifest.json")
actual_records = read(actual_run/"records.json")
report["actual_G1_nine_raw_commands"] = []
for item in actual_records:
    path = Path(item["raw_command_path"])
    if not path.is_absolute():
        path = ROOT/path
    command = read(path)
    text = command["stdout"].replace("\r", "")
    parsed, value = attempt(text, actual_manifest["kernel_release"], False)
    result = {"case": item["case"], "condition": item["condition"],
              "raw_path": str(path.relative_to(ROOT)), "parse": parsed,
              "live_part_t_fragment_before_snapshot": bool(re.search(r"part=t(?:\s|$)", text.split("\nCASE_EXIT=0\n", 1)[0]))}
    if value:
        assert value["record"] == item["record"]
        assert value["observer"] == item["observer"]
        assert value["metrics"] == item["metrics"]
        assert value["sequence"] == item["sequence"]
    report["actual_G1_nine_raw_commands"].append(result)

report["collector_unchanged_during_check"] = hashlib.sha256(COLLECTOR.read_bytes()).hexdigest() == report["collector_sha256"]
report["acceptance_checks"] = {
    "expected_90_case_outcomes": all(v["unexpected_rejections"] == 0 for v in report["campaigns"].values()),
    "both_stale_observer_probes_rejected": len(report["association_probes"]) == 2 and all(not p["accepted"] for p in report["association_probes"]),
    "three_per_condition_historical_G1_passed": report["review_tests"]["historical_three_per_condition"]["passed"],
    "too_few_and_missing_condition_rejected": not report["review_tests"]["two_per_condition"]["passed"] and not report["review_tests"]["missing_A8"]["passed"],
    "inclusive_two_percent_boundaries_accepted": all(report["review_tests"][name]["passed"] for name in ("synthetic_exact_upper_two_percent_boundary", "synthetic_exact_lower_two_percent_boundary")),
    "inside_accepted_outside_rejected": report["review_tests"]["synthetic_inside_tolerance"]["passed"] and not report["review_tests"]["synthetic_outside_tolerance"]["passed"],
    "real_printk_prefix_accepted_and_duplicate_CLI_rejected": bool(report["real_prefix_cases"]) and all(r["parse"]["accepted"] and not r["duplicate_CLI_negative"]["accepted"] for r in report["real_prefix_cases"]),
    "all_nine_actual_G1_commands_match_recovered_records": len(report["actual_G1_nine_raw_commands"]) == 9 and all(r["parse"]["accepted"] for r in report["actual_G1_nine_raw_commands"]),
    "collector_unchanged": report["collector_unchanged_during_check"]}
report["status"] = "passed" if all(report["acceptance_checks"].values()) else "needs_revision"
prior_path = HERE / "collector-compatibility.json"
if prior_path.exists():
    prior = json.loads(prior_path.read_text())
    report["previous_runs"] = prior.get("previous_runs", [])+[
        {k:prior[k] for k in ("collector_sha256", "campaigns", "review_tests", "association_probes", "collector_unchanged_during_check")}]
(HERE / "collector-compatibility.json").write_text(json.dumps(report, indent=2, ensure_ascii=False)+"\n")
print(json.dumps({"status": report["status"], "collector_sha256": report["collector_sha256"],
    "campaigns": {k: {n: v[n] for n in ("cases", "accepted", "expected_fixed_role_rejections", "unexpected_rejections")} for k,v in report["campaigns"].items()},
    "review_passed": {k:v["passed"] for k,v in report["review_tests"].items()},
    "association_probes": report["association_probes"],
    "collector_unchanged_during_check": report["collector_unchanged_during_check"]}, ensure_ascii=False, indent=2))
