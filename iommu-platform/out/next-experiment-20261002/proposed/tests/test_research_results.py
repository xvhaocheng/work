#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Tests for scripts/research_results.py.

The fixtures fabricate coherent raw captures (snapshots, committed update
records, first-probe records) so the analyzer's independent re-derivation is
checked against evidence that is self-consistent by construction.  No hardware
is involved and no board result is claimed.
"""

from __future__ import annotations

import hashlib
import importlib.util
import json
import re
import tempfile
import unittest
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
ANALYZER_PATH = REPO_ROOT / "scripts" / "research_results.py"
RTL_PATH = REPO_ROOT / "cva6" / "corev_apu" / "fpga" / "src" / "dma_research_observer.sv"


def load_module(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


rr = load_module(ANALYZER_PATH, "research_results_under_test")

DID_V = 10
DID_A = 11
PHASE_WARM = 1
PHASE_A = 2
PHASE_PROBE = 3

TARGET0_VPN = 0x100
TARGET1_VPN = 0x200
OTHER_VPN = 0x300


def ctx(vpn, pscid=7, gscid=3, valid=True, stage1=True, stage2=False, global_=False):
    lo = vpn & ((1 << 29) - 1)
    lo |= (pscid & ((1 << 20) - 1)) << 29
    lo |= (1 << 49) if stage1 else 0
    lo |= (1 << 50) if stage2 else 0
    lo |= (1 << 51) if valid else 0
    lo |= (1 << 56) if global_ else 0
    return (lo, gscid & 0xFFFF)


def plain(vpn):
    return ctx(vpn)


def to_hex_words(values):
    return ["0x%016x" % (value & 0xFFFFFFFFFFFFFFFF) for value in values]


def state_list(table):
    return [table.get(slot, (0, 0)) for slot in range(16)]


def apply_events(state, events):
    out = dict(state)
    for event in events:
        out[event["slot"]] = event["new"]
    return out


def build_meta(slot, phase, did, old, new, targets):
    meta = (1 << slot) | (slot << 16) | (phase << 21) | (did << 24) | (1 << 48)
    if old == new:
        meta |= 1 << 49
    for index in (0, 1):
        if rr.removes_target(old, new, rr.ident_of(targets[index])):
            meta |= 1 << (52 + index)
    return meta


def build_e2(*, targets, warm, deadline, freeze, probe_state, probe_times,
             events=(), mapping=None, astart=10_000, window=5_000_000,
             a_count=112, probestart=None, freeze_ts=None, phase=4,
             snapshot_valid=7, event_count=None, update_count=None,
             a_issue=None, a_done=None, magic=True, meta_override=None,
             a_first=None, a_last=None):
    words = [0] * 4096
    if magic:
        words[0x000] = rr.IDMA_RESEARCH_MAGIC
        words[0x001] = rr.OBSERVER_MAGIC
    words[0x003] = 0
    words[0x006] = phase
    words[0x007] = astart
    words[0x008] = window
    words[0x01E] = astart + window
    words[0x010], words[0x011] = targets[0]
    words[0x012], words[0x013] = targets[1]
    words[0x01C] = 3
    words[0x01D] = 3
    words[0x021] = snapshot_valid
    words[0x025] = a_count if a_issue is None else a_issue
    words[0x026] = a_count if a_done is None else a_done
    if a_count:
        words[0x027] = (astart + 10) if a_first is None else a_first
        words[0x028] = (astart + 200) if a_last is None else a_last
    words[0x032] = 3
    for name, table in (("warm", warm), ("deadline", deadline), ("freeze", freeze)):
        base = dict(rr.SNAPSHOT_REGIONS)[name]
        for slot in range(16):
            lo, hi = table.get(slot, (0, 0))
            words[base + 2 * slot] = lo
            words[base + 2 * slot + 1] = hi
    for index in (0, 1):
        state = state_list(probe_state[index])
        ident = rr.ident_of(targets[index])
        other_ident = rr.ident_of(targets[1 - index])
        live = rr.live_bitmap(state, ident)
        other = rr.live_bitmap(state, other_ident)
        hit = rr.present_ordinary(state, ident)
        words[0x014 + 4 * index] = probe_times[index]
        words[0x015 + 4 * index] = (1 << 32 if hit else 0) | (other << 16) | live
        words[0x016 + 4 * index], words[0x017 + 4 * index] = targets[index]
    words[0x01F] = probestart if probestart is not None else (astart + window + 10)
    words[0x020] = freeze_ts if freeze_ts is not None else (max(probe_times) + 10)
    for index, event in enumerate(events):
        base = rr.EVENT_BASE + index * rr.EVENT_WORDS
        meta = build_meta(event["slot"], event["phase"], event["did"],
                          event["old"], event["new"], targets)
        if meta_override is not None:
            meta = meta_override(index, meta)
        words[base] = event["ts"]
        words[base + 1] = meta
        words[base + 2], words[base + 3] = event["old"]
        words[base + 4], words[base + 5] = event["new"]
    words[0x00A] = len(events) if event_count is None else event_count
    words[0x00E] = len(events) if update_count is None else update_count
    if mapping is None:
        mapping = ((targets[0][0] & ((1 << 29) - 1)) << 12,
                   (targets[1][0] & ((1 << 29) - 1)) << 12, 0, 0)
    return to_hex_words(words), list(mapping)


def build_cost(*, window_cycles=1000, arm_main=4000, v_done=4096, a_done=100,
               samples=(), sample_count=None, eligible=None, sample_ovf=0,
               arm_done_all=None, carry_in=1, carry_out=0, cost_err=0,
               session_state=0b110, session_cycles=200000):
    words = [0] * 4096
    words[0xC00] = rr.COST_MAGIC
    words[0xC01] = session_state
    words[0xC02] = cost_err
    words[0xC03] = session_cycles
    words[0xC04] = arm_main
    words[0xC05] = 7
    words[0xC06] = 5
    words[0xC07] = 4
    words[0xC0F] = 10
    words[0xC10] = a_done
    words[0xC11] = v_done
    words[0xC12] = v_done
    words[0xC13] = a_done if arm_done_all is None else arm_done_all
    words[0xC14] = (len(samples) * 16) if eligible is None else eligible
    words[0xC15] = len(samples) if sample_count is None else sample_count
    words[0xC16] = sample_ovf
    words[0xC17] = carry_in
    words[0xC18] = carry_out
    words[0xC19] = window_cycles
    words[0xC1A] = 10
    words[0xC1B] = 5
    words[0xC1C] = 4
    words[0xC1D] = 3
    for index, sample in enumerate(samples):
        base = rr.SAMPLE_BASE + index * rr.SAMPLE_WORDS
        words[base] = 0xAA00 + index
        words[base + 1] = (sample.get("main", 3) << 32) | sample.get("complete", 2)
        words[base + 2] = (sample.get("req", 4) << 32) | sample.get("miss", 1)
        words[base + 3] = (sample.get("duration", 100) << 32) | sample.get("flags", 0x4)
    return to_hex_words(words)


def cost_p4m(window_cycles=1000, a_done=100, flags=0x3):
    window = [0] * 16
    window[0] = rr.P4M_WINDOW_MAGIC
    window[1] = window_cycles
    window[3] = a_done
    window[4] = a_done
    window[15] = flags
    return {
        "aggressor_gap_cycles": 0,
        "sequence": "0x%016x" % 1,
        "observer_status": "0x%016x" % 4,
        "observation": ["0x%016x" % window_cycles] + ["0x%016x" % 0] * 34,
        "window": ["0x%016x" % value for value in window],
        "experiment": {"status": 0},
    }


def cost_case(condition="a8g0", apages=8, vcount=4096, warmup=256, stage="main"):
    return {
        "id": "cost-b01-%s" % condition,
        "ordinal": 1,
        "stage": stage,
        "mode": "cost",
        "condition": condition,
        "apages": apages,
        "acount": 0,
        "gap": 0,
        "vcount": vcount,
        "warmup": warmup,
        "window": 0,
        "timeout": 10000,
    }


def e2_case(condition="w1", apages=1, stage="short", ordinal=2):
    return {
        "id": "short-e2-%s" % condition,
        "ordinal": ordinal,
        "stage": stage,
        "mode": "e2",
        "condition": condition,
        "apages": apages,
        "acount": 0 if apages == 0 else 112,
        "gap": 0,
        "vcount": 1,
        "warmup": 2,
        "window": 5_000_000,
        "timeout": 2000,
    }


class RawWordTests(unittest.TestCase):
    def test_exact64_strings_are_accepted_and_preserved(self):
        words = ["0x%016x" % 0] * rr.RAW_WORDS
        words[7] = "0x00000000FFFFFFFF"
        words[8] = "0x00000001ABCDEF01"
        raw = rr.parse_raw_words(words)
        self.assertEqual(raw[7], 0xFFFFFFFF)
        self.assertEqual(raw[8], 0x1ABCDEF01)
        self.assertEqual(raw.hex(8), "0x00000001ABCDEF01")
        self.assertEqual(len(raw.to_json()), rr.RAW_WORDS)

    def test_uppercase_exact64_is_preserved_verbatim(self):
        words = ["0x%016x" % 0] * rr.RAW_WORDS
        words[1] = "0x00000000000000AB"
        raw = rr.parse_raw_words(words)
        self.assertEqual(raw.hex(1), "0x00000000000000AB")

    def test_short_word_is_rejected(self):
        words = ["0x%016x" % 0] * rr.RAW_WORDS
        words[5] = "0x1"
        with self.assertRaises(rr.AnalysisError):
            rr.parse_raw_words(words)

    def test_wrong_length_is_rejected(self):
        with self.assertRaises(rr.AnalysisError):
            rr.parse_raw_words(["0x%016x" % 0] * (rr.RAW_WORDS - 1))

    def test_non_string_and_bool_are_rejected(self):
        words = ["0x%016x" % 0] * rr.RAW_WORDS
        words[3] = 17
        with self.assertRaises(rr.AnalysisError):
            rr.parse_raw_words(words)
        words[3] = True
        with self.assertRaises(rr.AnalysisError):
            rr.parse_raw_words(words)


class EntryDecodeTests(unittest.TestCase):
    def test_ordinary_s1_only_4k_entry(self):
        entry = ctx(TARGET0_VPN)
        self.assertTrue(rr.entry_is_ordinary(entry[0], entry[1]))
        decoded = rr.decode_entry(entry[0], entry[1])
        self.assertEqual(decoded["vpn"], TARGET0_VPN)
        self.assertTrue(decoded["en1_stage1"])
        self.assertFalse(decoded["en2_stage2"])

    def test_global_or_two_stage_entries_are_not_ordinary(self):
        self.assertFalse(rr.entry_is_ordinary(*ctx(TARGET0_VPN, global_=True)))
        self.assertFalse(rr.entry_is_ordinary(*ctx(TARGET0_VPN, stage2=True)))
        self.assertFalse(rr.entry_is_ordinary(*ctx(TARGET0_VPN, valid=False)))

    def test_identity_ignores_geometry_and_valid_bits(self):
        a = ctx(TARGET0_VPN)
        b = ctx(TARGET0_VPN, valid=False)
        self.assertEqual(rr.ident_of(a), rr.ident_of(b))


class E2AnalyzerTests(unittest.TestCase):
    """Fabricated but coherent E2 captures."""

    def setUp(self):
        self.t0 = ctx(TARGET0_VPN)
        self.t1 = ctx(TARGET1_VPN)
        self.other = plain(OTHER_VPN)
        self.targets = (self.t0, self.t1)
        self.case = e2_case("w1", 1)
        self.probe_times = (5_010_020, 5_010_040)

    def analyze(self, words, mapping=None, case=None):
        case = case or self.case
        raw = rr.parse_raw_words(words)
        problems = []
        unknowns = []
        evidence = rr.analyze_e2(raw, case, mapping if mapping is not None else None,
                                 problems, unknowns)
        return evidence, problems, unknowns

    def coherent(self, *, a_events=(), probe_events=(), probe_state_builder=None,
                 warm=None, events=None):
        warm = warm if warm is not None else {0: self.t0, 1: self.t1, 5: self.other}
        a_events = list(a_events)
        probe_events = list(probe_events)
        deadline = apply_events(warm, a_events)
        if probe_state_builder is None:
            probe_state = {0: deadline, 1: apply_events(deadline, probe_events)}
        else:
            probe_state = probe_state_builder(deadline, probe_events)
        freeze = apply_events(probe_state[1], [])
        events = list(events) if events is not None else a_events + probe_events
        words, mapping = build_e2(
            targets=self.targets,
            warm=warm,
            deadline=deadline,
            freeze=freeze,
            probe_state=probe_state,
            probe_times=self.probe_times,
            events=events,
            astart=10_000,
            window=5_000_000,
            a_count=self.case["acount"],
        )
        return words, mapping, deadline, freeze, probe_state

    # ------------------------------------------------------------------ cases
    def test_resident_hit_negative(self):
        a_event = {"ts": 10_100, "slot": 5, "old": self.other, "new": plain(0x400),
                   "phase": PHASE_A, "did": DID_A}
        words, mapping, _, _, _ = self.coherent(a_events=[a_event])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertEqual(problems, [])
        self.assertEqual([t["classification"] for t in evidence["targets"]],
                         ["resident_hit_negative", "resident_hit_negative"])
        self.assertTrue(evidence["replay"]["deadline_snapshot_reproduced"])
        self.assertTrue(evidence["replay"]["freeze_snapshot_reproduced"])

    def test_a_evicted_then_first_miss(self):
        a_event = {"ts": 10_100, "slot": 0, "old": self.t0, "new": plain(0x500),
                   "phase": PHASE_A, "did": DID_A}
        probe_event = {"ts": 5_010_030, "slot": 7, "old": self.other, "new": self.t0,
                       "phase": PHASE_PROBE, "did": DID_V}
        words, mapping, _, _, _ = self.coherent(a_events=[a_event], probe_events=[probe_event])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertEqual(problems, [])
        target0, target1 = evidence["targets"]
        self.assertEqual(target0["classification"], "A_evicted_then_first_miss")
        self.assertIn(a_event["ts"], [0] + [event["ts"] for event in []])
        self.assertEqual(target0["A_removal_events"], [0])
        self.assertEqual(target1["classification"], "resident_hit_negative")

    def test_probe_induced_loss_by_other_probe_refill(self):
        a_event = {"ts": 10_100, "slot": 0, "old": self.t0, "new": plain(0x500),
                   "phase": PHASE_A, "did": DID_A}
        probe_event = {"ts": 5_010_030, "slot": 1, "old": self.t1, "new": self.t0,
                       "phase": PHASE_PROBE, "did": DID_V}
        words, mapping, _, _, _ = self.coherent(a_events=[a_event], probe_events=[probe_event])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertEqual(problems, [])
        target0, target1 = evidence["targets"]
        self.assertEqual(target0["classification"], "A_evicted_then_first_miss")
        self.assertEqual(target1["classification"], "probe_induced_loss")
        self.assertIn("other_probe_refill", target1["cause"])
        self.assertTrue(target1["resident_deadline"])

    def test_unresolved_when_nothing_explains_the_miss(self):
        warm = {0: self.t0, 1: self.t1}
        deadline = dict(warm)
        # The probe record claims the target vanished although no record evicted it.
        probe_state = {0: {1: self.t1}, 1: {1: self.t1}}
        words, mapping = build_e2(
            targets=self.targets, warm=warm, deadline=deadline, freeze=probe_state[1],
            probe_state=probe_state, probe_times=self.probe_times, events=[],
            astart=10_000, window=5_000_000, a_count=self.case["acount"])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertEqual(evidence["targets"][0]["classification"], "other_unresolved")
        self.assertTrue(problems)
        self.assertTrue(any("live bitmap" in item for item in problems))

    def test_warm_phase_events_are_excluded_from_post_warm_replay(self):
        warm_event = {"ts": 9_500, "slot": 2, "old": plain(0x600), "new": self.other,
                      "phase": PHASE_WARM, "did": DID_V}
        warm = {0: self.t0, 1: self.t1, 2: self.other}
        words, mapping, _, _, _ = self.coherent(warm=warm, events=[warm_event])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertEqual(problems, [])
        self.assertEqual(evidence["warm_phase_events"], 1)
        self.assertEqual(evidence["post_warm_events"], 0)
        self.assertTrue(evidence["replay"]["deadline_snapshot_reproduced"])

    def test_same_edge_update_follows_the_probe(self):
        a_event = {"ts": 10_100, "slot": 0, "old": self.t0, "new": plain(0x500),
                   "phase": PHASE_A, "did": DID_A}
        # A probe-phase fill at exactly the first probe timestamp must not be seen
        # by that probe (pre-edge sample), but is visible later.
        same_edge = {"ts": self.probe_times[0], "slot": 6, "old": plain(0x700),
                     "new": plain(0x701), "phase": PHASE_PROBE, "did": DID_V}
        words, mapping, _, _, _ = self.coherent(a_events=[a_event], probe_events=[same_edge])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertEqual(problems, [])
        self.assertEqual(evidence["targets"][0]["first_probe"]["same_edge_update_events"], [1])
        self.assertEqual(evidence["targets"][1]["first_probe"]["hit"], True)

    def test_invalid_magic_is_flagged(self):
        words, mapping, _, _, _ = self.coherent()
        words[0] = "0x0000000000000000"
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertTrue(any("research magic" in item for item in problems))

    def test_error_flag_is_flagged(self):
        words, mapping, _, _, _ = self.coherent()
        words[0x00D] = "0x%016x" % 0x2
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertTrue(any("e2_err" in item for item in problems))

    def test_wrong_owner_event_is_flagged(self):
        a_event = {"ts": 10_100, "slot": 5, "old": self.other, "new": plain(0x400),
                   "phase": PHASE_A, "did": DID_V}
        words, mapping, _, _, _ = self.coherent(a_events=[a_event])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertTrue(any("wrong owner" in item for item in problems))

    def test_non_one_hot_mask_and_slot_mismatch_are_flagged(self):
        a_event = {"ts": 10_100, "slot": 5, "old": self.other, "new": plain(0x400),
                   "phase": PHASE_A, "did": DID_A}
        words, mapping, _, _, _ = self.coherent(a_events=[a_event])

        def bad_mask(index, meta):
            return meta | 0x1  # second bit set -> not one-hot

        words, mapping, _, _, _ = self.coherent(a_events=[a_event])
        broken, broken_mapping = build_e2(
            targets=self.targets,
            warm={0: self.t0, 1: self.t1, 5: self.other},
            deadline={0: self.t0, 1: self.t1, 5: plain(0x400)},
            freeze={0: self.t0, 1: self.t1, 5: plain(0x400)},
            probe_state={0: {0: self.t0, 1: self.t1}, 1: {0: self.t0, 1: self.t1}},
            probe_times=self.probe_times,
            events=[a_event],
            a_count=self.case["acount"],
            meta_override=bad_mask,
        )
        evidence, problems, unknowns = self.analyze(broken, broken_mapping)
        self.assertTrue(any("one-hot" in item for item in problems))

    def test_event_count_overflow_is_flagged(self):
        words, mapping, _, _, _ = self.coherent()
        words[0x00A] = "0x%016x" % 300
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertTrue(any("overflow" in item or "exceeds" in item for item in problems))

    def test_deadline_snapshot_mismatch_is_flagged(self):
        a_event = {"ts": 10_100, "slot": 5, "old": self.other, "new": plain(0x400),
                   "phase": PHASE_A, "did": DID_A}
        warm = {0: self.t0, 1: self.t1, 5: self.other}
        deadline = apply_events(warm, [a_event])
        words, mapping = build_e2(
            targets=self.targets, warm=warm, deadline=dict(warm), freeze=deadline,
            probe_state={0: deadline, 1: deadline}, probe_times=self.probe_times,
            events=[a_event], astart=10_000, window=5_000_000, a_count=self.case["acount"])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertTrue(any("deadline snapshot" in item for item in problems))

    def test_w0_anomaly_is_flagged_and_kept(self):
        case = e2_case("w0", 0)
        warm = {0: self.t0, 1: self.t1}
        words, mapping = build_e2(
            targets=self.targets, warm=warm, deadline={1: self.t1}, freeze={1: self.t1},
            probe_state={0: {1: self.t1}, 1: {1: self.t1}}, probe_times=self.probe_times,
            events=[], astart=10_000, window=5_000_000, a_count=0)
        evidence, problems, unknowns = self.analyze(words, mapping, case=case)
        self.assertTrue(evidence["no_a_anomaly"])
        self.assertTrue(any("W0" in item for item in unknowns))


class CostAnalyzerTests(unittest.TestCase):
    def setUp(self):
        self.case = cost_case("a8g0", 8)

    def analyze(self, words, p4m=None, rates=None):
        raw = rr.parse_raw_words(words)
        problems = []
        unknowns = []
        evidence = rr.analyze_cost(raw, self.case, p4m if p4m is not None else cost_p4m(),
                                   rates, problems, unknowns)
        return evidence, problems, unknowns

    def test_rate_uses_common_window_not_whole_arm_main(self):
        words = build_cost(window_cycles=1000, arm_main=999_999, v_done=4096, a_done=100)
        evidence, problems, unknowns = self.analyze(words)
        self.assertEqual(problems, [])
        rates = evidence["rates"]
        self.assertEqual(rates["denominator_word"], "0x%03x" % rr.w("window_cycles"))
        self.assertAlmostEqual(
            rates["v_mib_per_s"], 4096 * 4096 * 50_000_000 / 1000 / (1024.0 * 1024.0))
        self.assertAlmostEqual(
            rates["a_common_mib_per_s"], 100 * 4096 * 50_000_000 / 1000 / (1024.0 * 1024.0))
        self.assertAlmostEqual(rates["a_common_jobs_per_s"], 100 * 50_000_000 / 1000)
        self.assertNotAlmostEqual(rates["v_mib_per_s"],
                                  rates["v_mib_per_s_with_whole_arm_main_denominator"])

    def test_whole_arm_and_common_window_are_reported_separately(self):
        words = build_cost(window_cycles=1000, arm_main=4000, v_done=4096, a_done=100)
        evidence, problems, unknowns = self.analyze(words)
        self.assertEqual(evidence["whole_arm"]["cycles"], 200000)
        self.assertEqual(evidence["whole_arm"]["main_v"], 4000)
        self.assertEqual(evidence["common_window"]["cycles"], 1000)
        self.assertEqual(evidence["common_window"]["main_v"], 10)
        self.assertIn("whole arm", evidence["separation_note"])

    def test_sample_decode_and_grouping(self):
        samples = [
            {"main": 3, "complete": 2, "req": 4, "miss": 1, "duration": 100, "flags": 0x4},
            {"main": 5, "complete": 2, "req": 4, "miss": 1, "duration": 300, "flags": 0x4},
            {"main": 5, "complete": 2, "req": 4, "miss": 1, "duration": 200, "flags": 0x5},
        ]
        words = build_cost(samples=samples)
        evidence, problems, unknowns = self.analyze(words)
        self.assertEqual(evidence["sample_count"], 3)
        decoded = evidence["samples"]
        self.assertEqual([item["duration"] for item in decoded], [100, 300, 200])
        self.assertEqual(decoded[0]["main"], 3)
        self.assertEqual(decoded[0]["miss"], 1)
        self.assertTrue(decoded[0]["fully_contained"])
        self.assertEqual(evidence["carry"], {"carry_in": 1, "carry_out": 0})
        groups = rr.group_samples([("a8g0", "main", item) for item in decoded])
        self.assertEqual(len(groups), 1)
        self.assertEqual(groups[0]["n"], 3)
        self.assertEqual(groups[0]["min"], 100)
        self.assertEqual(groups[0]["max"], 300)
        self.assertEqual(groups[0]["median"], 200)
        self.assertEqual(groups[0]["stages"], ["main"])

    def test_zero_samples_are_legitimate(self):
        words = build_cost(samples=[], eligible=0, sample_count=0)
        evidence, problems, unknowns = self.analyze(words)
        self.assertEqual(problems, [])
        self.assertEqual(evidence["sample_count"], 0)

    def test_sample_overflow_and_count_mismatch_are_flagged(self):
        words = build_cost(samples=[], eligible=32, sample_count=0, sample_ovf=1)
        evidence, problems, unknowns = self.analyze(words)
        self.assertTrue(any("overflow" in item for item in problems))
        self.assertTrue(any("sample count" in item for item in problems))

    def test_a_done_counters_must_agree(self):
        words = build_cost(samples=[], arm_done_all=5)
        evidence, problems, unknowns = self.analyze(words)
        self.assertTrue(any("disagree" in item for item in problems))

    def test_p4m_window_mismatch_is_flagged(self):
        words = build_cost(window_cycles=1000)
        p4m = cost_p4m(window_cycles=1234, a_done=100)
        evidence, problems, unknowns = self.analyze(words, p4m=p4m)
        self.assertTrue(any("p4m.window[1]" in item for item in problems))

    def test_solo_with_a_activity_is_flagged_but_kept(self):
        self.case = cost_case("solo", 0)
        words = build_cost(window_cycles=1000, v_done=64, a_done=0)
        words = list(words)
        words[0xC0F] = "0x%016x" % 4
        words[0xC10] = "0x%016x" % 4
        words[0xC13] = "0x%016x" % 4
        p4m = cost_p4m(window_cycles=1000, a_done=0, flags=0x2)
        evidence, problems, unknowns = self.analyze(words, p4m=p4m)
        self.assertTrue(any("no-A" in item or "solo" in item for item in unknowns))


class FakePlan:
    """Minimal stand-in for scripts/research_plan.py."""

    def __init__(self, cases):
        self.cases = cases
        self.__file__ = "<fake-plan-generator>"

    def to_tsv(self, plan):
        lines = ["\t".join(rr.FROZEN_COLUMNS)]
        for case in self.cases:
            lines.append("\t".join(str(case[column]) for column in rr.FROZEN_COLUMNS))
        return "\n".join(lines) + "\n"

    def build_plan(self):
        tsv = self.to_tsv(None)
        return {
            "cases": self.cases,
            "counts": {},
            "tsv_sha256": hashlib.sha256(tsv.encode("utf-8")).hexdigest(),
        }


def short_cases():
    cases = [
        e2_case("w0", 0, "short", 1),
        e2_case("w1", 1, "short", 2),
        e2_case("w16", 16, "short", 3),
        cost_case("solo", 0, 64, 8, "short"),
        cost_case("a8g0", 8, 64, 8, "short"),
        cost_case("a16g0", 16, 64, 8, "short"),
    ]
    for case in cases:
        case["id"] = case["id"].replace("cost-b01-", "short-cost-")
        if case["mode"] == "e2":
            case["argv"] = ["--e2", "--a-pages", str(case["apages"]),
                            "--a-transfers", str(case["acount"]),
                            "--window-cycles", str(case["window"]),
                            "--warmup", str(case["warmup"]),
                            "--timeout-ms", str(case["timeout"])]
        else:
            case["argv"] = ["--cost", "--a-pages", str(case["apages"]),
                            "--a-gap-cycles", str(case["gap"]),
                            "--transfers", str(case["vcount"]),
                            "--warmup", str(case["warmup"]),
                            "--timeout-ms", str(case["timeout"])]
    return cases


def e2_doc(case, words, mapping):
    return {
        "schema": "idma-research-v1",
        "success": True,
        "validated": True,
        "mode": "e2",
        "input": {
            "a_pages": case["apages"],
            "a_transfers": case["acount"],
            "window_cycles": case["window"],
            "warmup": case["warmup"],
            "transfers": case["vcount"],
            "gap_cycles": case["gap"],
            "timeout_ms": case["timeout"],
        },
        "status": 0,
        "flush_status": 0,
        "stage": 5,
        "raw_captured": rr.RAW_WORDS,
        "observer_magic": "0x%016x" % rr.OBSERVER_MAGIC,
        "mapping": ["0x%016x" % value for value in mapping],
        "raw": words,
        "phase_stats": [{"submitted": 2, "completed": 2}, {"submitted": 112},
                        {"submitted": 1}],
        "errors": [],
        "syscall_rc": 0,
        "errno": 0,
    }


def cost_doc(case, words, p4m, rates=None):
    return {
        "schema": "idma-research-v1",
        "success": True,
        "validated": True,
        "mode": "cost",
        "input": {
            "a_pages": case["apages"],
            "a_transfers": case["acount"],
            "window_cycles": case["window"],
            "warmup": case["warmup"],
            "transfers": case["vcount"],
            "gap_cycles": case["gap"],
            "timeout_ms": case["timeout"],
        },
        "status": 0,
        "flush_status": 0,
        "stage": 5,
        "raw_captured": rr.RAW_WORDS,
        "observer_magic": "0x%016x" % rr.OBSERVER_MAGIC,
        "mapping": ["0x%016x" % 0] * 4,
        "raw": words,
        "phase_stats": [{}, {}, {}],
        "p4m": p4m,
        "rates": rates or {},
        "errors": [],
        "syscall_rc": 0,
        "errno": 0,
    }


def idle_e2_words():
    t0 = ctx(TARGET0_VPN)
    t1 = ctx(TARGET1_VPN)
    words, mapping = build_e2(
        targets=(t0, t1), warm={0: t0, 1: t1}, deadline={0: t0, 1: t1},
        freeze={0: t0, 1: t1}, probe_state={0: {0: t0, 1: t1}, 1: {0: t0, 1: t1}},
        probe_times=(5_010_020, 5_010_040), events=[], astart=10_000,
        window=5_000_000, a_count=112)
    return words, mapping, t0, t1


def write_run_dir(root, cases, docs, *, omit_ids=(), mutate_copy=False,
                  stopped=False, drop_cases_rows=()):
    root.mkdir()
    generator = FakePlan(cases)
    tsv = generator.to_tsv(None)
    rows = tsv.splitlines()
    copy_text = tsv
    if mutate_copy:
        copy_text = tsv.replace("short-e2-w1", "short-e2-x1", 1)
    (root / "plan.copy.tsv").write_text(copy_text, encoding="utf-8")
    (root / "plan.selected.tsv").write_text("\n".join(rows[1:]) + "\n", encoding="utf-8")
    case_lines = ["\t".join(rr.CASES_COLUMNS)]
    argv_lines = []
    for case in cases:
        if case["id"] in drop_cases_rows:
            continue
        case_lines.append("\t".join([case["id"], str(case["ordinal"]), case["stage"],
                                      case["mode"], case["condition"], "0", "0", "0"]))
        argv_lines.append("%s\t%s" % (case["id"], " ".join(case.get("argv", []))))
    (root / "cases.tsv").write_text("\n".join(case_lines) + "\n", encoding="utf-8")
    (root / "argv.tsv").write_text("\n".join(argv_lines) + "\n", encoding="utf-8")
    for case_id, doc in docs.items():
        if case_id in omit_ids:
            continue
        (root / (case_id + ".json")).write_text(json.dumps(doc), encoding="utf-8")
        (root / (case_id + ".exit")).write_text("0\n", encoding="utf-8")
        (root / (case_id + ".err")).write_text("", encoding="utf-8")
    if stopped:
        (root / "STOPPED.txt").write_text("status=stopped\nexit=1\nmode=short\n", encoding="utf-8")
    return generator


class RunnerDirectoryTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name) / "run"
        self.out = Path(self.tmp.name) / "out"
        self.cases = short_cases()
        words, mapping, _, _ = idle_e2_words()
        self.docs = {}
        for case in self.cases:
            if case["mode"] == "e2":
                case_words, case_mapping = build_e2(
                    targets=(ctx(TARGET0_VPN), ctx(TARGET1_VPN)),
                    warm={0: ctx(TARGET0_VPN), 1: ctx(TARGET1_VPN)},
                    deadline={0: ctx(TARGET0_VPN), 1: ctx(TARGET1_VPN)},
                    freeze={0: ctx(TARGET0_VPN), 1: ctx(TARGET1_VPN)},
                    probe_state={0: {0: ctx(TARGET0_VPN), 1: ctx(TARGET1_VPN)},
                                 1: {0: ctx(TARGET0_VPN), 1: ctx(TARGET1_VPN)}},
                    probe_times=(5_010_020, 5_010_040), events=[], astart=10_000,
                    window=5_000_000, a_count=case["acount"])
                self.docs[case["id"]] = e2_doc(case, case_words, case_mapping)
            else:
                case_words = build_cost(window_cycles=1000, v_done=case["vcount"], a_done=10)
                self.docs[case["id"]] = cost_doc(case, case_words,
                                                  cost_p4m(window_cycles=1000, a_done=10))

    def run_analyze(self, **kwargs):
        generator = write_run_dir(self.root, self.cases, self.docs, **kwargs)
        report = rr.analyze("short", str(self.root), str(self.out), plan_generator=generator)
        return report

    def test_complete_run_is_ok_and_writes_both_artifacts(self):
        report = self.run_analyze()
        self.assertEqual(report["status"], "ok")
        self.assertEqual(report["exit_status"], 0)
        results = json.loads((self.out / "results.json").read_text(encoding="utf-8"))
        self.assertEqual(results["schema"], rr.REPORT_SCHEMA)
        self.assertTrue((self.out / "report.zh.txt").read_text(encoding="utf-8"))
        self.assertEqual(
            report["report_zh_sha256"],
            hashlib.sha256((self.out / "report.zh.txt").read_bytes()).hexdigest())

    def test_cli_booleans_are_not_trusted(self):
        self.docs["short-e2-w1"] = dict(self.docs["short-e2-w1"])
        bad_words = list(self.docs["short-e2-w1"]["raw"])
        bad_words[0x00D] = "0x%016x" % 0x4
        self.docs["short-e2-w1"]["raw"] = bad_words
        report = self.run_analyze()
        record = [item for item in report["cases"] if item["id"] == "short-e2-w1"][0]
        self.assertEqual(record["cli_claims"]["success"], True)
        self.assertEqual(record["status"], "failed")
        self.assertEqual(report["exit_status"], 1)

    def test_mutated_plan_copy_is_flagged(self):
        report = self.run_analyze(mutate_copy=True)
        self.assertFalse(report["plan"]["verified"])
        self.assertTrue(any("plan.copy.tsv does not match" in item
                            for item in report["plan"]["problems"]))
        self.assertEqual(report["exit_status"], 1)

    def test_missing_case_is_listed_and_not_retried(self):
        report = self.run_analyze(omit_ids=("short-cost-a16g0",),
                                  drop_cases_rows=("short-cost-a16g0",))
        self.assertIn("short-cost-a16g0", report["summary"]["missing"])
        self.assertEqual(report["exit_status"], 1)
        self.assertFalse(report["limits"]["no_retry"] is False)

    def test_input_directory_is_never_mutated(self):
        before = sorted(str(path.relative_to(self.root)) for path in self.root.rglob("*"))
        report = self.run_analyze()
        after = sorted(str(path.relative_to(self.root)) for path in self.root.rglob("*"))
        self.assertEqual(before, after)
        self.assertTrue(report["input_hashes"]["unchanged"])
        self.assertEqual(report["input_hashes"]["changed"], [])

    def test_existing_output_directory_is_refused(self):
        self.root.mkdir()
        self.out.mkdir()
        with self.assertRaises(rr.UsageError):
            rr.analyze("short", str(self.root), str(self.out))

    def test_output_inside_input_is_refused(self):
        self.root.mkdir()
        with self.assertRaises(rr.UsageError):
            rr.analyze("short", str(self.root), str(self.root / "nested"))

    def test_no_hardware_claims_and_no_global_e2_claim(self):
        report = self.run_analyze()
        self.assertEqual(report["conclusions"]["hardware_claims"], "none")
        self.assertEqual(report["e2"]["global_claim"], "not_established")
        self.assertIn("engineering repetition", report["cost"]["repetition_note"])
        text = (self.out / "report.zh.txt").read_text(encoding="utf-8")
        self.assertIn("不构成任何硬件结论", text)
        self.assertIn("未建立", text)

    def test_stopped_run_is_incomplete_not_fatal(self):
        report = self.run_analyze(omit_ids=("short-e2-w16", "short-cost-solo",
                                            "short-cost-a8g0", "short-cost-a16g0"),
                                  drop_cases_rows=("short-e2-w16", "short-cost-solo",
                                                   "short-cost-a8g0", "short-cost-a16g0"),
                                  stopped=True)
        self.assertEqual(report["status"], "incomplete")
        self.assertEqual(report["exit_status"], 1)
        self.assertEqual(report["summary"]["expected_cases"], 6)
        self.assertEqual(len(report["summary"]["missing"]), 4)


class SvMapTests(unittest.TestCase):
    READ_MUX_RE = re.compile(r"13'h([0-9A-Fa-f]{4}):\s*rdata_o = (.+?);")

    def test_embedded_raw_map_matches_the_read_mux(self):
        if not RTL_PATH.is_file():
            self.skipTest("dma_research_observer.sv is not available")
        text = RTL_PATH.read_text(encoding="utf-8")
        mux = {int(index, 16): re.sub(r"\s+", "", rhs)
               for index, rhs in self.READ_MUX_RE.findall(text)}
        self.assertTrue(mux, "the read mux was not parsed")
        for name, (index, rhs) in rr.RAW_MAP.items():
            self.assertIn(index, mux, "%s (0x%03x) is absent from the read mux" % (name, index))
            self.assertEqual(
                mux[index], re.sub(r"\s+", "", rhs),
                "%s (0x%03x): table says %s, RTL says %s" % (name, index, rhs, mux[index]))


if __name__ == "__main__":
    unittest.main()
