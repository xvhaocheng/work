#!/usr/bin/env python3
"""Isolated fixtures for run import; production incremental input stays untouched."""
import copy
import hashlib
import json
from pathlib import Path
import statistics
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import import_board_runs as importer
import extract_results as tables

HERE = Path(__file__).resolve().parent
ROOT = tables.ROOT
checks = []


def require(name, value):
    if not value:
        raise AssertionError(name)
    checks.append({"check": name, "passed": True})


def write(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(importer.json_text(value))


historical_path = ROOT / "iommu-platform/out/mac-path-main-20260916-164633-run-vvguros8/records.json"
historical = json.loads(historical_path.read_text())
prefix_seed = ROOT / "paper/ispass2027/board/runs/g1-20261017-01/uart/command-e4178cb1140e49eaad4ad2db9df13e8d.json"
prefix_command = json.loads(prefix_seed.read_text()) if prefix_seed.is_file() else None
prefix_text = prefix_command["stdout"].replace("\r", "") if prefix_command else ""
prefix_at = prefix_text.find('{"schema":"idma.batch.v1"')
real_prefix = prefix_text[prefix_text.rfind("\n", 0, prefix_at)+1:prefix_at] if prefix_at >= 0 else ""
source_by_mode = {}
for item in historical:
    mode = "Solo" if item["record"]["result"]["aggressor"] is None else "A"+str(item["record"]["input"]["aggressor_pages"])
    source_by_mode.setdefault(mode, item)


def collector_metrics(item):
    obs, result = item["observer"], item["record"]["result"]
    time = obs["time"]
    metrics = {"hardware_ms": time["cycles"]/50000,
               "hardware_MiB_s": result["victim"]["completed_bytes"]/1048576/(time["cycles"]/50e6),
               "hardware_cycles": time["cycles"], "h_cycles": time["h"], "gap_cycles": time["gap"],
               "overlap_percent": obs["axi_v"]["both"]*100/time["cycles"],
               "software_ms": result["victim_elapsed_ns"]/1e6}
    for lane in ("victim", "aggressor"):
        for metric in ("iotlb_miss", "s1_ptw", "s2_ptw"):
            metrics[lane+"_"+metric] = result[lane][metric] if result[lane] else None
    for key, values in item["path"].items():
        metrics["path_"+key+"_mean_cycles"] = values["sum"]/values["full"] if values["full"] else None
        metrics["path_"+key+"_max_cycles"] = values["max"] if values["full"] else None
    return metrics


def review(records, phase):
    groups = {}
    for condition in importer.PHASES[phase][1]:
        items = [r for r in records if r["condition"] == condition]
        if not items:
            continue
        metrics = {}
        for field in items[0]["metrics"]:
            values = [i["metrics"][field] for i in items if i["metrics"][field] is not None]
            metrics[field] = {"median": statistics.median(values) if values else None,
                              "min": min(values) if values else None,
                              "max": max(values) if values else None}
        groups[condition] = {"n": len(items), "boot_ids": sorted({i["boot_id"] for i in items}), "metrics": metrics}
    result = {"groups": groups, "count": len(records),
              "sample_count_complete": all(g["n"] == importer.PHASES[phase][0] for g in groups.values())}
    if phase == "g1":
        result["passed"] = len(records) == 9
    return result


def fixture(directory, phase, suffix=0):
    repeat, conditions, ds, blocks = importer.PHASES[phase]
    directory.mkdir(parents=True)
    boot = f"00000000-0000-0000-0000-{1+list(importer.PHASES).index(phase)+suffix:012x}"
    bit = ("a" if ds == "50/50" else "b")*64
    load = {"status": "boot_identity_verified", "kernel_release": "fixture-kernel",
            "boot_id": boot, "bit_sha256": bit, "program_file_sha256_verified": True,
            "configuration_readback_verified": False}
    write(directory/"load.json", load)
    plan = [(block, condition) for block in blocks for condition in
            conditions[block % len(conditions):]+conditions[:block % len(conditions)]]
    records = []
    for ordinal, (block, condition) in enumerate(plan):
        original = source_by_mode[condition if condition in source_by_mode else "A8"]
        item = {"record": copy.deepcopy(original["record"]), "observer": copy.deepcopy(original["observer"]),
                "path": {p["role"]+p["channel"]+str(p["stage"]): {k:p[k] for k in
                         ("valid", "entered", "full", "carry", "sum", "max", "start", "end", "area")}
                         for p in original["path"]} if phase != "g1" else {},
                "sequence": 100+ordinal+1, "boot_id": boot, "case": ordinal+1,
                "condition": condition, "block": block, "ds": ds}
        if condition not in {"Solo", "A1", "A8"}:
            item["record"]["input"]["aggressor_pages"] = int(condition[1:])
            argv = item["record"]["argv"]
            argv[argv.index("--aggressor-pages")+1] = condition[1:]
        # Deliberately vary raw cycle fields, not summary prose, to test medians.
        delta = block*1000
        item["observer"]["time"]["cycles"] += delta
        item["observer"]["time"]["h"] += delta
        item["observer"]["axi_v"]["neither"] += delta
        item["record"]["result"]["victim_elapsed_ns"] += delta*20
        item["metrics"] = collector_metrics(item)
        item["raw_command_path"] = str(directory/"uart"/f"command-case{ordinal+1:04d}.json")
        records.append(item)
        cli = json.dumps(item["record"], separators=(",", ":"))
        text = f"KERNEL=fixture-kernel\nBOOT={boot}\nTAINT=0\nPRE_SEQ={item['sequence']-1}\n{cli}\nCASE_EXIT=0\nTAINT_AFTER=0\n"
        write(directory/"uart"/f"command-case{ordinal+1:04d}.json", {"exit_code": 0, "stdout": text, "command": "synthetic importer fixture only", "raw_path": item["raw_command_path"]})
    write(directory/"records.json", records)
    write(directory/"review.json", review(records, phase))
    manifest = {"phase": phase, "load_manifest": str(directory/"load.json"),
                "load_manifest_sha256": importer.sha_bytes((directory/"load.json").read_bytes()),
                "kernel_release": "fixture-kernel", "bit_sha256": bit, "status": "passed",
                "recorded_cases": len(records), "planned_cases": plan, "synthetic_fixture": True}
    write(directory/"manifest.json", manifest)
    return records


def materialize(files, output):
    output.mkdir(parents=True)
    for name, text in files.items():
        (output/name).write_text(text)


def runner_checks(work, board):
    import contextlib
    import io
    import runpy
    import types

    runner = runpy.run_path(str(board/"run_baseline100.py"))
    calls = []
    class Result:
        returncode = 0
        def check_returncode(self):
            pass
    def fake_run(argv, **kwargs):
        calls.append(argv)
        return Result()
    runner["main"].__globals__["subprocess"] = types.SimpleNamespace(run=fake_run)
    runner["main"].__globals__["print"] = lambda *a, **k: None
    write_receipt = work/"software-write.json"
    write(write_receipt, {"exit_code": 0})
    def invoke(label, initial=None):
        calls.clear()
        old_argv = sys.argv
        try:
            sys.argv = ["run_baseline100.py", "--output", str(work/label), "--credential-helper", str(work/"unused-helper"), "--software-write", str(write_receipt)]
            if initial is not None:
                sys.argv.extend(["--initial-load-manifest", str(initial)])
            with contextlib.redirect_stderr(io.StringIO()):
                runner["main"]()
        finally:
            sys.argv = old_argv
        return list(calls), json.loads((work/label/"campaign.json").read_text())

    default_calls, _ = invoke("runner")
    loads = [a for a in default_calls if a[1].endswith("program_and_boot.py")]
    collections = [a for a in default_calls if a[1].endswith("collect.py")]
    require("Future runner loads twice and collects DS50-all then DS0-all", len(loads) == len(collections) == 2 and [a[a.index("--phase")+1] for a in collections] == list(importer.FORMAL_AB))
    require("Runner selects distinct DS50/DS0 BITs without a software rewrite", loads[0][loads[0].index("--bit")+1] != loads[1][loads[1].index("--bit")+1] and len(default_calls) == 4)

    delivery = json.loads((board/"packages/baseline100-20261017/manifest.json").read_text())
    receipt = {"status": "boot_identity_verified", "kernel_release": delivery["kernel_release"],
               "bit_sha256": delivery["artifacts"]["ds50_bit"]["sha256"],
               "boot_id": "00000000-0000-0000-0000-000000000001"}
    initial = work/"existing-ds50-load.json"
    write(initial, receipt)
    original = initial.read_bytes()
    reused_calls, campaign = invoke("runner-reused", initial)
    reused_loads = [a for a in reused_calls if a[1].endswith("program_and_boot.py")]
    reused_collections = [a for a in reused_calls if a[1].endswith("collect.py")]
    require("Reusing initial DS50 receipt leaves only one DS0 program call", len(reused_calls) == 3 and len(reused_loads) == 1 and reused_loads[0][reused_loads[0].index("--bit")+1] == loads[1][loads[1].index("--bit")+1] and [a[a.index("--phase")+1] for a in reused_collections] == list(importer.FORMAL_AB))
    require("First collection uses exact existing load receipt for its boot check", reused_collections[0][reused_collections[0].index("--load-manifest")+1] == str(initial.resolve()))
    first = campaign["phases"][0]
    require("Campaign records reuse without inventing a load receipt or changing source", campaign["initial_load_manifest"] == str(initial.resolve()) and first["load"]["action"] == "reuse_existing_receipt" and not first["load"]["new_program_or_boot"] and not (work/"runner-reused/ds50-all-load").exists() and initial.read_bytes() == original)
    for field, value in [("bit_sha256", "f"*64), ("kernel_release", "wrong-kernel"), ("status", "failed")]:
        invalid = work/("invalid-"+field+".json")
        write(invalid, {**receipt, field: value})
        label = "runner-invalid-"+field
        try:
            invoke(label, invalid)
        except SystemExit as error:
            require("Initial receipt rejects mismatched "+field+" before any process or output", error.code == 2 and not calls and not (work/label).exists())
        else:
            raise AssertionError("Mismatched initial receipt accepted: "+field)


def phase_layout_checks():
    """Exercise only the phase extension, with all board I/O replaced locally."""
    import re
    import runpy
    import types

    before = importer.OFFICIAL.read_bytes()
    with tempfile.TemporaryDirectory(prefix=".phase-layout-fixture-", dir=HERE) as temporary:
        work = Path(temporary)
        for phase in importer.FORMAL+importer.FORMAL_AB:
            fixture(work/phase, phase)
        aba = [work/p for p in importer.FORMAL]
        ab = [work/p for p in importer.FORMAL_AB]
        _, _, old = importer.prepare(aba, work/"aba-import", "fixture-aba")
        require("Existing ABA accepts 25+50+25 and three actual boots", old["formal100"]["complete"] and old["formal100"]["accepted_points"] == 100 and old["formal100"]["observed_boot_count"] == 3)
        pooled = [g for g in old["groups"] if g["aggregation"] == "pooled_first_and_last_with_explicit_boots"]
        require("Existing ABA still pools ten DS50 trials across two boots", len(pooled) == 5 and all(g["n_trials"] == 10 and g["observed_boot_count"] == 2 for g in pooled))
        files, _, new = importer.prepare(ab, work/"ab-import", "fixture-ab")
        again, _, _ = importer.prepare(reversed(ab), work/"ab-import", "fixture-ab")
        require("AB import is deterministic in argument order", files == again)
        require("AB accepts 50+50 with two actual boots", new["formal100"]["complete"] and new["formal100"]["accepted_points"] == 100 and new["formal100"]["observed_boot_count"] == 2 and new["formal100"]["layout"] == "AB_50_50")
        require("Each AB condition has ten trials in one boot without pooling", len(new["groups"]) == 10 and all(g["n_trials"] == 10 and g["observed_boot_count"] == 1 and g["aggregation"] == "within_one_run_phase_and_boot" for g in new["groups"]))
        for phase in importer.FORMAL_AB:
            _, _, half = importer.prepare([work/phase], work/(phase+"-half-import"), "fixture-half-"+phase)
            require("Missing other AB half rejects completeness: "+phase, not half["formal100"]["complete"] and half["formal100"]["accepted_points"] == 50 and 0 in half["formal100"]["phase_run_counts"].values())
        duplicate = work/"duplicate-all"
        fixture(duplicate, "ds50-all", suffix=20)
        _, _, duplicated = importer.prepare(ab+[duplicate], work/"ab-duplicate", "fixture-ab-duplicate")
        require("AB repeated phase points stay visible and block completeness", not duplicated["formal100"]["complete"] and len(duplicated["formal100"]["duplicate_points"]) == 50)
        _, _, mixed = importer.prepare(ab+[aba[0]], work/"mixed", "fixture-mixed")
        require("AB and ABA cannot be mixed into one formal100", not mixed["formal100"]["complete"] and bool(mixed["formal100"]["layout_issues"]))

        board = ROOT/"paper/ispass2027/board"
        load = work/"collector-load.json"
        write(load, {"status": "boot_identity_verified", "kernel_release": "fixture-kernel", "boot_id": "00000000-0000-0000-0000-000000000001", "bit_sha256": "a"*64})

        def planned_execution(path, phase, destination):
            commands = []
            class FakeConsole:
                def __init__(self, *args):
                    pass
                def close(self):
                    pass
                def command(self, command, timeout):
                    commands.append(command)
                    return {"exit_code": 0, "stdout": command, "raw_path": str(destination/"synthetic-command.json")}
            def fake_parse(text, expected_kernel, expect_path):
                solo = "--aggressor none " in text
                pages = int(re.search(r"--aggressor-pages (\d+)", text)[1])
                condition = "Solo" if solo else "A"+str(pages)
                return {"boot_id": "00000000-0000-0000-0000-000000000001", "metrics": {"hardware_ms": {"Solo":56.5,"A1":107.0,"A8":155.0}.get(condition, 200.0)},
                        "record": {"input": {"aggressor_pages": pages}, "result": {"aggressor": None if solo else {}}}}
            uart = types.ModuleType("uart")
            uart.Console, uart.DEFAULT_PORT = FakeConsole, "synthetic-no-port"
            old_uart, old_argv = sys.modules.get("uart"), sys.argv
            sys.modules["uart"] = uart
            try:
                module = runpy.run_path(str(path))
                module["main"].__globals__["parse"] = fake_parse
                module["main"].__globals__["print"] = lambda *a, **k: None
                sys.argv = [str(path), "--phase", phase, "--output", str(destination), "--expected-kernel", "fixture-kernel", "--load-manifest", str(load)]
                module["main"]()
            finally:
                sys.argv = old_argv
                if old_uart is None:
                    del sys.modules["uart"]
                else:
                    sys.modules["uart"] = old_uart
            return commands, json.loads((destination/"records.json").read_text()), json.loads((destination/"review.json").read_text())

        snapshot = board/"manifests/collect.baseline100-as-dispatched.py"
        for phase in ("g1",)+importer.FORMAL:
            previous = planned_execution(snapshot, phase, work/(phase+"-old-collector"))
            current = planned_execution(board/"collect.py", phase, work/(phase+"-new-collector"))
            select = lambda items: [(x["block"], x["condition"], x["ds"]) for x in items]
            require("Old phase commands/order/DS/review unchanged: "+phase, previous[0] == current[0] and select(previous[1]) == select(current[1]) and previous[2] == current[2])
        for phase in importer.FORMAL_AB:
            commands, items, reviewed = planned_execution(board/"collect.py", phase, work/(phase+"-collector"))
            count, conditions, ds, blocks = importer.PHASES[phase]
            plan = [(b,c) for b in blocks for c in conditions[b % len(conditions):]+conditions[:b % len(conditions)]]
            require("New collector phase has rotated fifty-point plan and review n=10: "+phase, len(commands) == 50 and [(x["block"],x["condition"]) for x in items] == plan and {x["ds"] for x in items} == {ds} and reviewed["sample_count_complete"] and all(g["n"] == count == 10 for g in reviewed["groups"].values()))

        runner_checks(work, board)
    require("Phase fixtures leave production incremental input unchanged", importer.OFFICIAL.read_bytes() == before)
    report = {"schema_version": 1, "status": "passed", "scope": "Focused AB/ABA phase fixtures; collector parsing stubbed, all UART/subprocess operations stubbed; no production run changed", "checks_passed": len(checks), "checks": checks}
    write(HERE/"phase-layout-validation.json", report)
    print(json.dumps({"status": "passed", "checks_passed": len(checks), "board_operations": 0}))


if "--phase-layout-only" in sys.argv:
    phase_layout_checks()
    raise SystemExit(0)


if "--runner-reuse-only" in sys.argv:
    with tempfile.TemporaryDirectory(prefix=".runner-reuse-fixture-", dir=HERE) as temporary:
        runner_checks(Path(temporary), ROOT/"paper/ispass2027/board")
    write(HERE/"runner-reuse-validation.json", {"schema_version": 1, "status": "passed", "scope": "Host receipt checks and stubbed runner subprocess only; no board operations or production run changes", "checks_passed": len(checks), "checks": checks})
    print(json.dumps({"status": "passed", "checks_passed": len(checks), "board_operations": 0}))
    raise SystemExit(0)


production_before = importer.sha_bytes(importer.OFFICIAL.read_bytes())
with tempfile.TemporaryDirectory(prefix=".importer-fixture-", dir=HERE) as temporary:
    work = Path(temporary)
    runs = []
    raw = {}
    for phase in ("g1",)+importer.FORMAL:
        run = work/phase
        raw[phase] = fixture(run, phase)
        runs.append(run)
    output = work/"complete-import"
    files, config, summary = importer.prepare(runs, output, "fixture-complete")
    materialize(files, output)
    second, _, _ = importer.prepare(reversed(runs), output, "fixture-complete")
    require("Exact deterministic import independent of input argument order", files == second)
    command = [sys.executable, str(HERE/"import_board_runs.py"), "--name", "fixture-complete", "--output", str(output), "--check"]
    for run in runs:
        command.extend(["--run", str(run)])
    result = subprocess.run(command, check=True, capture_output=True, text=True)
    require("Actual importer CLI --check replays the snapshot without writing", json.loads(result.stdout)["status"] == "passed")
    require("Formal 25+50+25 contains exactly 100 accepted points", summary["formal100"]["complete"] and summary["formal100"]["accepted_points"] == 100)
    require("G1 nine points remain a separate series", summary["g1"]["accepted_points"] == 9 and summary["g1"]["all_named_runs_passed"])
    require("Three phase boots are recorded, never ten independent restarts", summary["formal100"]["observed_boot_count"] == 3 and summary["formal100"]["independent_restart_repetitions"] is None)
    pooled = [g for g in summary["groups"] if g["aggregation"] == "pooled_first_and_last_with_explicit_boots"]
    require("DS50 pooled groups expose ten trials across two actual boots", len(pooled) == 5 and all(g["n_trials"] == 10 and g["observed_boot_count"] == 2 and g["metrics"]["hardware_ms"]["n"] == 10 for g in pooled))
    require("Matched release is not promoted to identical software payload", all(g["software_payload_equivalence"] == "not_inferred_from_kernel_release" for g in pooled))
    for group in summary["groups"]:
        if group["aggregation"] != "within_one_run_phase_and_boot":
            continue
        expected = [r["observer"]["time"]["cycles"]/50000 for r in raw[group["phases"][0]] if r["condition"] == group["condition"]]
        require("Raw-cycle median agrees: "+group["group_id"], group["metrics"]["hardware_ms"]["median"] == statistics.median(expected))
    require("All new scalar specs have the exact incremental schema", all(set(r) == set(tables.FIELDS)-{"value"} for r in config["rows"]))
    integration = work/"integration"
    integration.mkdir()
    target = integration/"incremental_sources.json"
    importer.install(config, target)
    installed = target.read_bytes()
    importer.install(config, target)
    require("Repeated identical installation is idempotent", target.read_bytes() == installed)
    install_command = ["--install" if word == "--check" else word for word in command]
    install_command.extend(["--incremental", str(target)])
    result = subprocess.run(install_command, check=True, capture_output=True, text=True)
    require("Existing reviewed snapshot can be installed via CLI without overwriting", json.loads(result.stdout)["installed"] and target.read_bytes() == installed)
    original_here = tables.HERE
    try:
        tables.HERE = integration
        extraction = tables.Extraction()
        tables.add_incremental(extraction)
        require("Every imported scalar locator resolves through actual extraction", len(extraction.board) == len(config["rows"]))
        require("Absent aggressor quantities stay not_applicable unknown", any(r["metric"] == "aggressor_completed" and r["value"] == "unknown" and r["value_status"] == "not_applicable" for r in extraction.board))
        require("Genuine zero miss remains observed zero", any(r["metric"] == "victim_iotlb_miss" and r["value"] == 0 and r["statistic"] == "sample" for r in extraction.board))
        victim_run = work/"ds50-first"/"records.json"
        preserved = victim_run.read_bytes()
        victim_run.write_bytes(preserved+b"\n")
        try:
            tables.add_incremental(tables.Extraction())
        except AssertionError:
            checks.append({"check": "Original source byte tampering fails imported hash check", "passed": True})
        else:
            raise AssertionError("Changed original source accepted")
        victim_run.write_bytes(preserved)
    finally:
        tables.HERE = original_here
    try:
        importer.install(config, importer.OFFICIAL)
    except ValueError as error:
        require("Synthetic fixture cannot enter production incremental configuration", "Synthetic fixture" in str(error))
    else:
        raise AssertionError("Fixture production installation unexpectedly succeeded")
    # A real-shaped failed collector attempt is represented by its original raw
    # command, while only the saved valid records contribute to statistics.
    failed = work/"failed-first"
    failed_raw = fixture(failed, "ds50-first", suffix=10)
    write(failed/"records.json", failed_raw[:3])
    for path in (failed/"uart").glob("command-*.json"):
        if path.name > "command-case0004.json":
            path.unlink()
    command = json.loads((failed/"uart/command-case0004.json").read_text())
    command.update(exit_code=None, error="synthetic timeout")
    write(failed/"uart/command-case0004.json", command)
    manifest = json.loads((failed/"manifest.json").read_text())
    manifest.update(status="failed", error="TimeoutError: fixture failure", recorded_cases=3)
    write(failed/"manifest.json", manifest)
    (failed/"review.json").unlink()
    _, failed_config, failed_summary = importer.prepare([failed, work/"ds0-middle", work/"ds50-last"], work/"failed-import", "fixture-failure")
    run = next(r for r in failed_summary["runs"] if r["phase"] == "ds50-first")
    require("Failed raw UART attempt is retained without becoming an accepted point", run["attempted_points_with_evidence"] == 4 and run["accepted_points"] == 3 and run["failed_or_unretained_points"] == 1)
    require("Early failure and missing review cannot claim formal100 complete", not failed_summary["formal100"]["complete"] and any("review.json missing" in x for x in run["issues"]))
    require("Failed attempt scalar rows expose unknown without zero substitution", any("failed_or_unretained" in r["sample_set"] and r["value_status"] == "unknown" for r in failed_config["rows"]))
    duplicate = work/"duplicate-first"
    fixture(duplicate, "ds50-first", suffix=20)
    _, _, duplicate_summary = importer.prepare(runs+[duplicate], work/"duplicates-import", "fixture-duplicate")
    require("Duplicate phase/block/condition points are not silently counted as extra repetitions", not duplicate_summary["formal100"]["complete"] and len(duplicate_summary["formal100"]["duplicate_points"]) == 25)
    # Distinct recorded BITs block cross-boot DS50 pooling.
    last_load = work/"ds50-last/load.json"
    data = json.loads(last_load.read_text()); data["bit_sha256"] = "c"*64; write(last_load, data)
    last_manifest = work/"ds50-last/manifest.json"
    data = json.loads(last_manifest.read_text()); data["bit_sha256"] = "c"*64
    data["load_manifest_sha256"] = importer.sha_bytes(last_load.read_bytes()); write(last_manifest, data)
    _, _, mismatch_summary = importer.prepare(runs, work/"mismatch-import", "fixture-mismatch")
    require("Different DS50 BIT hashes block pooling and completion acceptance", not mismatch_summary["formal100"]["complete"] and bool(mismatch_summary["formal100"]["pooling_issues"]) and not any(g["aggregation"] == "pooled_first_and_last_with_explicit_boots" for g in mismatch_summary["groups"]))
    # Remove an actual source field instead of substituting a made-up zero.
    missing = work/"missing-counter"
    fixture(missing, "g1", suffix=30)
    missing_records = json.loads((missing/"records.json").read_text())
    del missing_records[0]["record"]["result"]["victim"]["iotlb_miss"]
    write(missing/"records.json", missing_records)
    _, missing_config, missing_summary = importer.prepare([missing], work/"missing-import", "fixture-missing")
    value = missing_summary["runs"][0]["attempts"][0]["metrics"]["victim_iotlb_miss"]
    require("Missing raw counter stays null with unknown status", value["value"] is None and value["value_status"] == "unknown")
    require("Missing scalar uses an explicit generated null locator", any(r["row_id"].endswith("case0001.victim_iotlb_miss.sample") and r["raw_path"].endswith("summary.json") and r["value_status"] == "unknown" for r in missing_config["rows"]))
    # The reported median is never the numerical source for newly built stats.
    forged_review = work/"g1/review.json"
    data = json.loads(forged_review.read_text())
    data["groups"]["A1"]["metrics"]["hardware_ms"]["median"] = 999999
    write(forged_review, data)
    evidence = work/"software-evidence.json"
    write(evidence, {"kernel_release": "fixture-kernel", "payload_sha256": "d"*64, "synthetic_fixture": True})
    _, _, original_summary = importer.prepare([work/"g1"], work/"forged-review-import", "fixture-original-values", [evidence])
    group = next(g for g in original_summary["groups"] if g["condition"] == "A1")
    require("Reported summary prose/numbers do not replace original cycle values", group["metrics"]["hardware_ms"]["median"] == statistics.median(r["observer"]["time"]["cycles"]/50000 for r in raw["g1"] if r["condition"] == "A1"))
    require("Review disagreement is surfaced rather than silently accepted", any("review median mismatch" in x for x in original_summary["runs"][0]["issues"]))
    require("Software evidence retains a hashed pointer without certifying loaded bytes", original_summary["software_evidence"][0]["raw_path"] == importer.relative(evidence) and original_summary["software_payload_equivalence"] == "not_inferred")
    require("Real printk prefix seed exists without a preceding newline", bool(real_prefix) and prefix_at > 0 and prefix_text[prefix_at-1] != "\n")
    actual_documents = importer.batch_documents(prefix_text)
    require("Actual original prefixed CLI JSON decodes completely", len(actual_documents) == 1 and actual_documents[0]["result"]["victim"]["completed"] == 4096 and actual_documents[0]["result"]["victim"]["iotlb_miss"] == 0)
    recovered_new = work/"recovered-new"
    recovered_raw = fixture(recovered_new, "g1", suffix=40)
    recovered_old = work/"recovered-old"
    recovered_old.mkdir()
    oldest = work/"recovered-original/manifest.json"
    write(oldest, {"phase": "g1", "status": "failed", "recorded_cases": 0,
                   "error": "initial parser failure", "synthetic_fixture": True})
    old_manifest = json.loads((recovered_new/"manifest.json").read_text())
    old_manifest.update(status="failed", recorded_cases=0, error="AssertionError: exactly one original CLI JSON")
    old_manifest.update(recovered_from=str(oldest), recovered_manifest_sha256=importer.sha_bytes(oldest.read_bytes()))
    write(recovered_old/"manifest.json", old_manifest)
    new_first = recovered_new/"uart/command-case0001.json"
    recovered_command = json.loads(new_first.read_text())
    recovered_command["stdout"] = recovered_command["stdout"].replace('{"schema":"idma.batch.v1"', real_prefix+'{"schema":"idma.batch.v1"', 1)
    old_command = recovered_old/"uart/command-original.json"
    recovered_command["raw_path"] = str(old_command)
    write(old_command, recovered_command)
    new_first.unlink()
    recovered_raw[0]["raw_command_path"] = str(old_command)
    write(recovered_new/"records.json", recovered_raw)
    recovery_manifest = json.loads((recovered_new/"manifest.json").read_text())
    recovery_manifest.update(recovered_from=str(recovered_old/"manifest.json"),
        recovered_manifest_sha256=importer.sha_bytes((recovered_old/"manifest.json").read_bytes()),
        recovered_raw_commands=[str(old_command)], recovered_cases=1)
    write(recovered_new/"manifest.json", recovery_manifest)
    _, recovered_config, recovered_summary = importer.prepare([recovered_new], work/"recovered-import", "fixture-recovery")
    recovered_run = recovered_summary["runs"][0]
    require("Nine records bind to eight new commands plus one recovered original", len(list((recovered_new/"uart").glob("command-*.json"))) == 8 and recovered_run["attempted_points_with_evidence"] == recovered_run["accepted_points"] == 9 and recovered_run["failed_or_unretained_points"] == 0 and recovered_run["phase_complete"])
    require("Recovered prefix sample has the exact original command path once", recovered_run["attempts"][0]["command_path"] == importer.relative(old_command) and sum(a["recovered_original_command"] for a in recovered_run["attempts"]) == 1)
    require("Old failed parser manifest stays in recovery provenance without becoming a new failed trial", recovered_run["recovery"]["prior_manifest_status"] == "failed" and recovered_run["recovery"]["matched_recovered_cases"] == 1 and not recovered_summary["software_evidence"])
    require("Two previous failed manifests remain in the recursive recovery hash chain", len(recovered_run["recovery"]["manifest_chain"]) == 2 and all(m["status"] == "failed" for m in recovered_run["recovery"]["manifest_chain"]))
    source_paths = {s["raw_path"] for s in recovered_config["source_files"]}
    require("Recovered original command and failed manifest both enter scalar source hash chain", {importer.relative(old_command), importer.relative(recovered_old/"manifest.json")} <= source_paths)
    program_failed = work/"program-failed/load.json"
    write(program_failed, {"status": "failed", "error": "KeyboardInterrupt", "synthetic_fixture": True})
    (program_failed.parent/"program.log").write_text("synthetic program returned\n")
    auth = work/"authentication-receipt.json"
    write(auth, {"status": "shell_prompt_observed_identity_still_required", "password_saved": False})
    live_load = recovered_new/"load.json"
    live_load_data = json.loads(live_load.read_text())
    live_load_data.update(resumed_from=str(program_failed), original_receipt_sha256=importer.sha_bytes(program_failed.read_bytes()), authentication_receipt=str(auth))
    write(live_load, live_load_data)
    recovery_manifest["load_manifest_sha256"] = importer.sha_bytes(live_load.read_bytes())
    write(recovered_new/"manifest.json", recovery_manifest)
    _, auth_config, auth_summary = importer.prepare([recovered_new], work/"auth-recovery-import", "fixture-auth-chain")
    evidence_chain = auth_summary["runs"][0]["identity"]["load_evidence"]
    require("Original program failure and authentication receipt are retained under load identity", any(e["role"] == "resumed_from" and e["status"] == "failed" for e in evidence_chain) and any(e["role"] == "authentication_receipt" for e in evidence_chain) and any(e["role"] == "program.log" for e in evidence_chain) and not auth_summary["software_evidence"])
    wrong_path_records = copy.deepcopy(recovered_raw)
    wrong_path_records[0]["raw_command_path"] = wrong_path_records[1]["raw_command_path"]
    write(recovered_new/"records.json", wrong_path_records)
    _, _, wrong_path_summary = importer.prepare([recovered_new], work/"wrong-command-import", "fixture-wrong-command")
    require("Explicit raw_command_path mismatch is rejected rather than matched elsewhere", not wrong_path_summary["runs"][0]["attempts"][0]["accepted"] and any(e["field"] == "/raw_command_path" for e in wrong_path_summary["runs"][0]["attempts"][0]["errors"]))
    write(recovered_new/"records.json", recovered_raw)
    original_failed_manifest = (recovered_old/"manifest.json").read_bytes()
    (recovered_old/"manifest.json").write_bytes(original_failed_manifest+b"\n")
    try:
        importer.prepare([recovered_new], work/"bad-recovery-hash", "fixture-bad-recovery")
    except ValueError as error:
        require("Recorded recovered_manifest_sha256 detects original failed manifest changes", "hash disagrees" in str(error))
    else:
        raise AssertionError("Recovered original manifest hash mismatch accepted")
    (recovered_old/"manifest.json").write_bytes(original_failed_manifest)

require("Production incremental_sources.json is byte-for-byte unchanged", importer.sha_bytes(importer.OFFICIAL.read_bytes()) == production_before)
report = {"schema_version": 1, "status": "passed", "scope": "Isolated synthetic collector-shaped fixtures seeded by archived raw fields; no board operation and no official result installation",
          "generator_sha256": importer.sha_bytes((HERE/"import_board_runs.py").read_bytes()),
          "extractor_sha256": importer.sha_bytes((HERE/"extract_results.py").read_bytes()),
          "test_sha256": importer.sha_bytes(Path(__file__).read_bytes()),
          "historical_seed": {"raw_path": str(historical_path.relative_to(ROOT)), "sha256": importer.sha_bytes(historical_path.read_bytes())},
          "actual_printk_prefix_seed": {"raw_path": str(prefix_seed.relative_to(ROOT)), "sha256": importer.sha_bytes(prefix_seed.read_bytes())},
          "production_incremental_sha256": production_before,
          "checks": checks, "checks_passed": len(checks), "temporary_fixture_inputs_removed": True}
write(HERE/"importer-validation.json", report)
print(json.dumps({"status": "passed", "checks_passed": len(checks), "production_incremental_unchanged": True}, ensure_ascii=False))
