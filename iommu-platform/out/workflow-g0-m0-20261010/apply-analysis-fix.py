#!/usr/bin/env python3
"""Targeted patch for out/workflow-g0-m0-20261010/report-block-0.py.

Applies exact-string replacements to the offline analyzer:

1. Port classification by the RTL slave field slv = (tag >> 4) & 3 instead of
   the per-tag table that wrongly mapped tag 36 to the DS port.
2. results-table.csv batch_complete now derives from both channel records
   (V and A): batch_complete == 1 and not stopped.
3. End-of-run contract check: every case_id is printed and any tail miss,
   unmatched R beat, same-id order violation, beat index mismatch, unaccounted
   R beats or non-consistent PTW result exits 1 after the diagnostics JSON has
   been written.
4. results.json records must be duplicate-free and exactly equal the
   baseline-register.json case set, with matching a_pages / scenario /
   ds_priority metadata.

No simulation is run and no other file is touched.

Usage:
    python3 apply-report-block-0-patch.py            # patch in place (+ .bak)
    python3 apply-report-block-0-patch.py --check    # dry check only
"""

import argparse
import sys
from pathlib import Path


DEFAULT_TARGET = Path("out/workflow-g0-m0-20261010/report-block-0.py")


PATCHES = [
    # 1. slv-based port map
    (
        "slv_port_map",
        '''PORT_BY_TAG = {
    32: "port2_payload",
    36: "port3_ds",
}''',
        '''# Tag -> port mapping follows the RTL slave encoding slv = (tag >> 4) & 3.
# Verified on the A8 root traces: tag 32 (slv2, V payload), tag 36 (slv2,
# A payload) and tag 48 (slv3, DS).  Tag 36 is NOT a DS tag.
# Ports are classified by slv; unknown slv/tag pairs keep an explicit label
# instead of being folded silently into a known port.
PORT_BY_SLV = {
    2: "port2_payload",
    3: "port3_ds",
}


def port_for_tag(tag):
    slv = (tag >> 4) & 3
    name = PORT_BY_SLV.get(slv)
    if name is None:
        return f"unknown_slv{slv}_tag{tag}"
    return name''',
    ),
    # 2. preset the known ports
    (
        "preset_ports",
        '''    for port in ("port2_payload", "port3_ds"):
        per_port[port]''',
        '''    for port in sorted(PORT_BY_SLV.values()):
        per_port[port]''',
    ),
    # 3. latency attribution uses the slv-derived port
    (
        "port_lookup",
        '''                    port = PORT_BY_TAG.get(tag, f"tag{tag}")''',
        '''                    port = port_for_tag(tag)''',
    ),
    # 4. per-channel completion helper
    (
        "channel_complete_helper",
        '''def channel_map(record):
    return {channel["id"]: channel for channel in record.get("channels", [])}''',
        '''def channel_map(record):
    return {channel["id"]: channel for channel in record.get("channels", [])}


def channel_complete(channel):
    """True only when the channel reports batch_complete == 1 and is not stopped."""
    return channel.get("batch_complete") == 1 and not bool(channel.get("stopped"))''',
    ),
    # 5. batch_complete from both channels instead of the missing top-level field
    (
        "batch_complete_cell",
        '''            "batch_complete": record.get("batch_complete"),''',
        '''            "batch_complete": int(channel_complete(v) and channel_complete(a)),''',
    ),
    # 6. registry/records/metadata validation + contract failure collection
    (
        "validation_helpers",
        '''def main():
    parser = argparse.ArgumentParser()''',
        '''def index_records(results, cases):
    """Index results.json records by case_id; reject duplicates and set drift."""
    records = results.get("records")
    if not isinstance(records, list):
        die("results.json: records must be a list")

    registry_ids = {case["id"] for case in cases}
    records_by_case = {}
    duplicates = []
    for record in records:
        case_id = record.get("case_id")
        if not case_id:
            die(f"results.json: record without case_id: {record!r}")
        if case_id in records_by_case:
            duplicates.append(case_id)
            continue
        records_by_case[case_id] = record

    if duplicates:
        die(f"results.json: duplicate case records: {sorted(set(duplicates))}")

    missing = sorted(registry_ids - set(records_by_case))
    if missing:
        die(f"results.json missing case records: {missing}")

    unexpected = sorted(set(records_by_case) - registry_ids)
    if unexpected:
        die(f"results.json records not in baseline-register.json: {unexpected}")

    return records_by_case


def check_record_metadata(cases, records_by_case):
    """baseline-register.json metadata must match the results.json record.

    A field absent on one side is not reported here; the record-level checks
    already fail earlier if a required field is missing there.
    """
    problems = []
    for case in cases:
        case_id = case["id"]
        record = records_by_case[case_id]

        base_a_pages = case.get("a_pages")
        rec_a_pages = record.get("a_pages")
        if (
            base_a_pages is not None
            and rec_a_pages is not None
            and base_a_pages != rec_a_pages
        ):
            problems.append(
                f"{case_id}: a_pages baseline={base_a_pages!r} results={rec_a_pages!r}"
            )

        base_scenario = case.get("scenario", case.get("mode"))
        rec_scenario = record.get("scenario", record.get("mode"))
        if (
            base_scenario is not None
            and rec_scenario is not None
            and base_scenario != rec_scenario
        ):
            problems.append(
                f"{case_id}: scenario baseline={base_scenario!r} results={rec_scenario!r}"
            )

        base_ds = case.get("ds_priority")
        rec_ds = record.get("ds_priority")
        if base_ds is not None and rec_ds is not None and bool(base_ds) != bool(rec_ds):
            problems.append(
                f"{case_id}: ds_priority baseline={base_ds!r} results={rec_ds!r}"
            )

    if problems:
        die(
            "baseline-register.json / results.json metadata mismatch: "
            + "; ".join(problems)
        )


PTW_CONSISTENT = "consistent"


def collect_case_failures(trace_stats_by_case):
    """Return {case_id: [reason, ...]} for every case breaking the contract."""
    failures = {}
    for case_id, case_stats in trace_stats_by_case.items():
        reasons = []

        if not case_stats["all_requests_tail_complete"]:
            reasons.append("tail_incomplete")

        unmatched_r = case_stats["unmatched_r_beats"]
        if unmatched_r:
            reasons.append(f"unmatched_r_beats={unmatched_r}")

        order_violations = case_stats["same_id_order_violations"]
        if order_violations:
            reasons.append(f"same_id_order_violations={order_violations}")

        beat_mismatch = case_stats["beat_index_mismatches"]
        if beat_mismatch:
            reasons.append(f"beat_index_mismatches={beat_mismatch}")

        if not case_stats["all_r_beats_accounted"]:
            reasons.append("r_beats_not_accounted")

        ptw = case_stats["ptw"]
        ptw_consistency = ptw["consistency"]
        if ptw_consistency != PTW_CONSISTENT:
            ptw_matched = ptw["matched_total_cycles"]
            ptw_summary = ptw["summary_ptw_active_cycles"]
            ptw_start = ptw["unmatched_start"]
            ptw_end = ptw["unmatched_end"]
            ptw_fullt0 = ptw["fullt0_events"]
            reasons.append(
                f"ptw_consistency={ptw_consistency} matched={ptw_matched} "
                f"summary={ptw_summary} unmatched_start={ptw_start} "
                f"unmatched_end={ptw_end} fullt0_events={ptw_fullt0}"
            )

        if reasons:
            failures[case_id] = reasons
    return failures


def main():
    parser = argparse.ArgumentParser()''',
    ),
    # 7. use the strict index + metadata checks
    (
        "main_records_index",
        '''    records_by_case = {}
    for record in results.get("records", []):
        case_id = record.get("case_id")
        if case_id:
            records_by_case[case_id] = record

    missing = [case["id"] for case in cases if case["id"] not in records_by_case]
    if missing:
        die(f"results.json missing case records: {missing}")''',
        '''    records_by_case = index_records(results, cases)
    check_record_metadata(cases, records_by_case)''',
    ),
    # 8. end-of-run contract gate (diagnostics JSON already written above)
    (
        "end_of_run_gate",
        '''    bad_tails = [
        case_id
        for case_id, case_stats in trace_stats_by_case.items()
        if not case_stats["all_requests_tail_complete"]
    ]
    if bad_tails:
        print(f"ERROR: incomplete request tails for: {bad_tails}", file=sys.stderr)
        sys.exit(1)''',
        '''    failures = collect_case_failures(trace_stats_by_case)
    for case_id in trace_stats_by_case:
        reasons = failures.get(case_id, [])
        if reasons:
            detail = "; ".join(reasons)
            print(f"FAIL {case_id}: {detail}", file=sys.stderr)
        else:
            print(f"OK {case_id}")

    if failures:
        failed_ids = ", ".join(failures)
        print(
            f"ERROR: {len(failures)}/{len(trace_stats_by_case)} cases violate the "
            f"registration contract: {failed_ids}",
            file=sys.stderr,
        )
        print(f"diagnostics kept in {stats_path}", file=sys.stderr)
        sys.exit(1)

    print("all cases satisfy the registration contract")''',
    ),
]


def die(message):
    raise SystemExit(f"ERROR: {message}")


def apply_patches(text, patches):
    for name, old, new in patches:
        occurrences = text.count(old)
        if occurrences != 1:
            die(
                f"anchor {name!r} matched {occurrences} times in the target file; "
                "refusing to patch"
            )
        text = text.replace(old, new, 1)
    return text


def read_text(path):
    with path.open("r", encoding="utf-8", newline="") as f:
        return f.read()


def write_text(path, text):
    with path.open("w", encoding="utf-8", newline="") as f:
        f.write(text)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--target", type=Path, default=DEFAULT_TARGET)
    parser.add_argument(
        "--check",
        action="store_true",
        help="verify that every anchor matches and the result compiles, without writing",
    )
    parser.add_argument(
        "--no-backup",
        action="store_true",
        help="do not write <target>.bak before patching",
    )
    args = parser.parse_args()

    target = args.target
    if not target.is_file():
        die(f"target script not found: {target}")

    original = read_text(target)
    patched = apply_patches(original, PATCHES)

    try:
        compile(patched, str(target), "exec")
    except SyntaxError as exc:
        die(f"patched source does not compile: {exc}")

    if args.check:
        print(f"{target}: all {len(PATCHES)} anchors matched; patched source compiles")
        return

    if not args.no_backup:
        backup = target.parent / (target.name + ".bak")
        write_text(backup, original)
        print(f"wrote backup {backup}")

    write_text(target, patched)
    print(f"patched {target} ({len(PATCHES)} replacements)")


if __name__ == "__main__":
    main()
