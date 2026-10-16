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
import os
import re
import subprocess
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
    words[0] = rr.IDMA_RESEARCH_MAGIC
    words[1] = rr.OBSERVER_MAGIC
    words[2] = (1 << 21) | (1 << 22) | (1 << 12) | (1 << 14) | (1 << 15)
    words[4] = 2
    words[0xC01] = session_state
    words[0xC02] = cost_err
    words[0xC03] = session_cycles
    words[0xC04] = arm_main
    words[0xC05] = 7
    words[0xC06] = 5
    words[0xC07] = 4
    words[0xC0F] = a_done
    words[0xC10] = a_done
    words[0xC11] = v_done
    words[0xC12] = v_done
    words[0xC13] = a_done if arm_done_all is None else arm_done_all
    words[0xC2D] = rr.DID_CONSTANTS
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
    window[2] = a_done
    window[15] = flags
    return {
        "aggressor_gap_cycles": 0,
        "sequence": "0x%016x" % 1,
        "observer_status": "0x%016x" % 4,
        "observation": ["0x%016x" % window_cycles] + ["0x%016x" % 0] * 34,
        "window": ["0x%016x" % value for value in window],
        "experiment": {
            "status": 0,
            "victim": {"submitted": 4096, "completed": 4096, "completed_bytes": 4096 * 4096,
                       "data_errors": 0, "quarantined": 0},
            "aggressor": {"submitted": a_done, "completed": a_done, "completed_bytes": a_done * 4096,
                          "data_errors": 0, "quarantined": 0},
        },
    }


def set_raw_word(words, index, value):
    words[index] = "0x%016x" % (value & 0xFFFFFFFFFFFFFFFF)


def apply_fullstats(words, groups, *, error=0, version=1, group_count=6, stride=16,
                    magic=rr.FULLSTATS_MAGIC, total=None, header_reserved=()):
    """Overlay a fullstats block on a raw word list (mutates in place)."""
    set_raw_word(words, rr.FULLSTATS_BASE, magic)
    set_raw_word(words, rr.FULLSTATS_BASE + 1, version)
    set_raw_word(words, rr.FULLSTATS_BASE + 2, group_count)
    set_raw_word(words, rr.FULLSTATS_BASE + 3, stride)
    set_raw_word(words, rr.FULLSTATS_BASE + 4, error)
    total_value = sum(group.get("count", 0) for group in groups) if total is None else total
    set_raw_word(words, rr.FULLSTATS_BASE + 5, total_value)
    for offset in range(6, 16):
        set_raw_word(words, rr.FULLSTATS_BASE + offset, 1 if offset in header_reserved else 0)
    for index, group in enumerate(groups):
        base = rr.FULLSTATS_GROUP_BASE + index * rr.FULLSTATS_GROUP_STRIDE
        values = [
            group.get("count", 0),
            group.get("sum_main", 0),
            group.get("sum_complete", 0),
            group.get("sum_duration", 0),
            group.get("sum_req", 0),
            group.get("sum_miss", 0),
            (group.get("max_main", 0) << 32) | group.get("min_main", 0),
            (group.get("max_complete", 0) << 32) | group.get("min_complete", 0),
            (group.get("max_duration", 0) << 32) | group.get("min_duration", 0),
        ]
        values.extend(group.get("reserved", [0] * (rr.FULLSTATS_GROUP_STRIDE - 9)))
        for offset, value in enumerate(values):
            set_raw_word(words, base + offset, value)


def cost_case(condition="a8g0", apages=8, vcount=4096, warmup=256, stage="main",
              *, ordinal=1, gap=0, case_id=None):
    return {
        "id": case_id or "cost-b01-%s" % condition,
        "ordinal": ordinal,
        "stage": stage,
        "mode": "cost",
        "condition": condition,
        "apages": apages,
        "acount": 0,
        "gap": gap,
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
        probe_event = {"ts": 5_010_030, "slot": 7, "old": (0, 0), "new": self.t0,
                       "phase": PHASE_PROBE, "did": DID_V}
        words, mapping, _, _, _ = self.coherent(a_events=[a_event], probe_events=[probe_event])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertEqual(problems, [])
        target0, target1 = evidence["targets"]
        self.assertEqual(target0["classification"], "A_evicted_then_first_miss")
        self.assertEqual(evidence["events"][0]["ts"], a_event["ts"])
        self.assertEqual(target0["A_removal_events"], [0])
        self.assertEqual(target1["classification"], "resident_hit_negative")

    def test_a_evict_then_restore_then_probe_evict_is_probe_induced(self):
        a_event1 = {"ts": 10_100, "slot": 0, "old": self.t0, "new": plain(0x500),
                    "phase": PHASE_A, "did": DID_A}
        a_event2 = {"ts": 10_200, "slot": 2, "old": (0, 0), "new": self.t0,
                    "phase": PHASE_A, "did": DID_A}
        probe_event = {"ts": 5_010_010, "slot": 2, "old": self.t0, "new": plain(0x600),
                       "phase": PHASE_PROBE, "did": DID_V}
        warm = {0: self.t0, 1: self.t1, 5: self.other}
        deadline = apply_events(warm, [a_event1, a_event2])
        probe_state = {0: apply_events(deadline, [probe_event]),
                       1: apply_events(deadline, [probe_event])}
        freeze = apply_events(probe_state[1], [])
        words, mapping = build_e2(
            targets=self.targets, warm=warm, deadline=deadline, freeze=freeze,
            probe_state=probe_state, probe_times=self.probe_times,
            events=[a_event1, a_event2, probe_event], astart=10_000,
            window=5_000_000, a_count=self.case["acount"])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertEqual(problems, [])
        target0 = evidence["targets"][0]
        self.assertEqual(target0["classification"], "probe_induced_loss")
        self.assertIn("probe_phase_event", target0["cause"])

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
        same_edge = {"ts": self.probe_times[0], "slot": 6, "old": (0, 0),
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

    def test_dst_a_evicted_then_miss_then_refill_then_src_probe_has_no_false_alarm(self):
        # Board order: the A phase evicts the dst target; the dst probe (which runs
        # first) misses; a dst refill is committed; only then does the src probe run,
        # so the src probe's cross view of the dst target is taken after that refill.
        # The presence-change check must use each target's own probe time and its own
        # probe bitmap, otherwise this later cross view raises a false alarm.
        warm = {0: self.t0, 1: self.t1, 5: self.other}
        a_event = {"ts": 10_100, "slot": 1, "old": self.t1, "new": plain(0x500),
                   "phase": PHASE_A, "did": DID_A}
        refill = {"ts": 5_010_030, "slot": 2, "old": (0, 0), "new": self.t1,
                  "phase": PHASE_PROBE, "did": DID_V}
        deadline = apply_events(warm, [a_event])
        probe_state = {0: apply_events(deadline, [refill]), 1: deadline}
        freeze = apply_events(deadline, [refill])
        words, mapping = build_e2(
            targets=self.targets, warm=warm, deadline=deadline, freeze=freeze,
            probe_state=probe_state, probe_times=(5_010_040, 5_010_020),
            events=[a_event, refill], astart=10_000, window=5_000_000,
            a_count=self.case["acount"])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertEqual(problems, [])
        self.assertEqual(unknowns, [])
        src, dst = evidence["targets"]
        self.assertEqual(src["classification"], "resident_hit_negative")
        self.assertEqual(dst["classification"], "A_evicted_then_first_miss")
        self.assertFalse(dst["first_probe"]["hit"])
        self.assertEqual(dst["A_removal_events"], [0])
        cross_check = dst["presence_cross_check"]
        self.assertEqual(cross_check["own_probe_index"], 1)
        self.assertEqual(cross_check["own_probe_time"], 5_010_020)
        self.assertEqual(cross_check["own_probe_live_bitmap"], 0)
        self.assertTrue(cross_check["probe0_live_bitmap"])
        self.assertEqual(cross_check["changing_events_between_deadline_and_own_probe"], [])
        self.assertEqual(
            cross_check["changing_events_between_deadline_and_probe"],
            cross_check["changing_events_between_deadline_and_own_probe"])

    def test_src_first_and_same_cycle_probe_orders_stay_clean(self):
        warm = {0: self.t0, 1: self.t1, 5: self.other}
        deadline = dict(warm)
        probe_state = {0: dict(deadline), 1: dict(deadline)}
        for probe_times in ((5_010_020, 5_010_040), (5_010_020, 5_010_020)):
            with self.subTest(probe_times=probe_times):
                words, mapping = build_e2(
                    targets=self.targets, warm=warm, deadline=deadline,
                    freeze=dict(deadline), probe_state=probe_state,
                    probe_times=probe_times, events=[], astart=10_000,
                    window=5_000_000, a_count=self.case["acount"])
                evidence, problems, unknowns = self.analyze(words, mapping)
                self.assertEqual(problems, [])
                self.assertEqual(unknowns, [])

    def test_unexplained_presence_change_still_warns(self):
        warm = {0: self.t0, 1: self.t1, 5: self.other}
        deadline = dict(warm)
        probe_state = {0: {1: self.t1}, 1: {1: self.t1}}
        words, mapping = build_e2(
            targets=self.targets, warm=warm, deadline=deadline,
            freeze=dict(deadline), probe_state=probe_state,
            probe_times=(5_010_020, 5_010_040), events=[], astart=10_000,
            window=5_000_000, a_count=self.case["acount"])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertEqual(evidence["targets"][0]["classification"], "other_unresolved")
        self.assertTrue(any("presence changed" in item for item in unknowns))
        self.assertTrue(any("live bitmap" in item for item in problems))

    def test_own_probe_window_event_explains_the_presence_change(self):
        warm = {0: self.t0, 1: self.t1, 5: self.other}
        deadline = dict(warm)
        evict = {"ts": 5_010_010, "slot": 0, "old": self.t0, "new": plain(0x800),
                 "phase": PHASE_PROBE, "did": DID_V}
        probe_state = {0: apply_events(deadline, [evict]),
                       1: apply_events(deadline, [evict])}
        words, mapping = build_e2(
            targets=self.targets, warm=warm, deadline=deadline,
            freeze=apply_events(deadline, [evict]), probe_state=probe_state,
            probe_times=(5_010_020, 5_010_040), events=[evict], astart=10_000,
            window=5_000_000, a_count=self.case["acount"])
        evidence, problems, unknowns = self.analyze(words, mapping)
        self.assertEqual(problems, [])
        self.assertEqual(unknowns, [])


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
        words = build_cost(window_cycles=1000, arm_main=999_999, v_done=4096, a_done=100,
                           session_cycles=2_000_000)
        evidence, problems, unknowns = self.analyze(words)
        self.assertEqual(problems, [])
        rates = evidence["rates"]
        self.assertEqual(rates["denominator_word"], "0x%03x" % rr.w("window_cycles"))
        self.assertAlmostEqual(
            rates["v_mib_per_s"], 4096 * 4096 * 50_000_000 / 1000 / (1024.0 * 1024.0))
        self.assertAlmostEqual(
            rates["a_common_mib_per_s"], 100 * 4096 * 50_000_000 / 1000 / (1024.0 * 1024.0))
        self.assertAlmostEqual(rates["a_common_jobs_per_s"], 100 * 50_000_000 / 1000)

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

    def test_lane_active_is_close_latched_information_not_capture_busy(self):
        words = list(build_cost(samples=[], eligible=0, sample_count=0))
        words[0xC2C] = "0x%016x" % 2
        evidence, problems, unknowns = self.analyze(words)
        self.assertEqual(problems, [])
        self.assertEqual(unknowns, [])
        lane = evidence["lane_active"]
        self.assertEqual(lane["value"], 2)
        self.assertFalse(lane["v_lane"])
        self.assertTrue(lane["a_lane"])
        self.assertTrue(lane["success_context_verified"])
        self.assertIn("close-latched", lane["meaning"])
        self.assertEqual(lane["carry_in"], 1)
        self.assertEqual(lane["carry_out"], 0)

    def test_lane_active_survives_a_natural_arm_carry_out(self):
        # A natural carry-out: the whole-arm A lane had issued more than it had
        # retired when the window closed, while the p4m.experiment final A/V
        # accounting shows drained, fully completed work.  This must not be turned
        # into a failure and must not raise a fabricated warning.
        words = list(build_cost(samples=[], eligible=0, sample_count=0, a_done=100))
        words[0xC0F] = "0x%016x" % 101
        words[0xC2C] = "0x%016x" % 2
        evidence, problems, unknowns = self.analyze(words)
        self.assertEqual(problems, [])
        self.assertEqual(unknowns, [])
        lane = evidence["lane_active"]
        self.assertEqual(lane["value"], 2)
        self.assertFalse(lane["v_lane"])
        self.assertTrue(lane["a_lane"])
        self.assertTrue(lane["success_context_verified"])

    def test_lane_active_does_not_mask_an_open_window(self):
        words = list(build_cost(samples=[], eligible=0, sample_count=0))
        words[0xC01] = "0x%016x" % 0b011
        words[0xC2C] = "0x%016x" % 2
        evidence, problems, unknowns = self.analyze(words)
        self.assertTrue(any("session is not" in item for item in problems))
        self.assertFalse(evidence["lane_active"]["success_context_verified"])
        self.assertEqual(unknowns, [])

    def test_lane_active_does_not_mask_incomplete_final_a_work(self):
        words = list(build_cost(samples=[], eligible=0, sample_count=0))
        words[0xC2C] = "0x%016x" % 2
        p4m = cost_p4m()
        p4m["experiment"]["aggressor"]["completed"] = 99
        evidence, problems, unknowns = self.analyze(words, p4m=p4m)
        self.assertTrue(any("P4M A completion" in item for item in problems))
        self.assertFalse(evidence["lane_active"]["success_context_verified"])

    def test_lane_active_does_not_mask_a_victim_quarantine(self):
        words = list(build_cost(samples=[], eligible=0, sample_count=0))
        words[0xC2C] = "0x%016x" % 2
        p4m = cost_p4m()
        p4m["experiment"]["victim"]["quarantined"] = 1
        evidence, problems, unknowns = self.analyze(words, p4m=p4m)
        self.assertTrue(any("quarantined" in item for item in problems))
        self.assertFalse(evidence["lane_active"]["success_context_verified"])

    def test_lane_active_does_not_mask_missing_p4m_fields(self):
        words = list(build_cost(samples=[], eligible=0, sample_count=0))
        words[0xC2C] = "0x%016x" % 2
        p4m = cost_p4m()
        del p4m["experiment"]["victim"]["completed"]
        evidence, problems, unknowns = self.analyze(words, p4m=p4m)
        self.assertTrue(any("victim completed" in item for item in problems))
        self.assertFalse(evidence["lane_active"]["success_context_verified"])

    def test_lane_active_zero_is_reported_without_a_warning(self):
        words = build_cost(samples=[], eligible=0, sample_count=0)
        evidence, problems, unknowns = self.analyze(words)
        self.assertEqual(problems, [])
        self.assertEqual(unknowns, [])
        self.assertEqual(evidence["lane_active"]["value"], 0)
        self.assertFalse(evidence["lane_active"]["v_lane"])
        self.assertFalse(evidence["lane_active"]["a_lane"])


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


class FakeFullstatsPlan(FakePlan):
    """FakePlan plus the cost-fullstats selector contract.

    The real plan generator owns the frozen 21-case selection; this fixture only
    mirrors the interface the analyzer depends on: the selector returns case
    copies that carry --require-fullstats and require_fullstats=True.
    """

    def select_cases(self, cases, mode):
        if mode != "cost-fullstats":
            return list(cases)
        selected = []
        for case in cases:
            copy = dict(case)
            copy["require_fullstats"] = True
            copy["argv"] = list(case.get("argv", [])) + ["--require-fullstats"]
            selected.append(copy)
        return selected


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
        "phase_stats": [
            {"submitted": 2, "completed": 2, "completed_bytes": 2 * 4096,
             "data_errors": 0, "quarantined": 0},
            {"submitted": case["acount"], "completed": case["acount"],
             "completed_bytes": case["acount"] * 4096, "data_errors": 0, "quarantined": 0},
            {"submitted": 1, "completed": 1, "completed_bytes": 4096,
             "data_errors": 0, "quarantined": 0},
        ],
        "errors": [],
        "syscall_rc": 0,
        "errno": 0,
    }


def cost_doc(case, words, p4m, rates=None):
    p4m["experiment"]["victim"].update(submitted=case["vcount"], completed=case["vcount"],
                                        completed_bytes=case["vcount"] * 4096)
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
        "phase_stats": [
            {"submitted": 0, "completed": 0, "completed_bytes": 0, "data_errors": 0, "quarantined": 0},
            {"submitted": 0, "completed": 0, "completed_bytes": 0, "data_errors": 0, "quarantined": 0},
            {"submitted": 0, "completed": 0, "completed_bytes": 0, "data_errors": 0, "quarantined": 0},
        ],
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


def build_docs(cases):
    docs = {}
    for case in cases:
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
            docs[case["id"]] = e2_doc(case, case_words, case_mapping)
        else:
            a_done = 10 if case["apages"] else 0
            case_words = build_cost(window_cycles=1000, v_done=case["vcount"], a_done=a_done)
            docs[case["id"]] = cost_doc(case, case_words,
                                        cost_p4m(window_cycles=1000, a_done=a_done,
                                                 flags=3 if case["apages"] else 2))
    return docs


def write_run_dir(root, cases, docs, *, omit_ids=(), mutate_copy=False,
                  stopped=False, drop_cases_rows=(), plan_tsv=None, selected_tsv=None):
    root.mkdir()
    if plan_tsv is None:
        generator = FakePlan(cases)
        tsv = generator.to_tsv(None)
    else:
        generator = None
        tsv = plan_tsv
    rows = tsv.splitlines()
    copy_text = tsv
    if mutate_copy:
        copy_text = tsv.replace("short-e2-w1", "short-e2-x1", 1)
    (root / "plan.copy.tsv").write_text(copy_text, encoding="utf-8")
    if selected_tsv is None:
        selected_tsv = "\n".join(rows[1:1+len(cases)]) + "\n"
    (root / "plan.selected.tsv").write_text(selected_tsv, encoding="utf-8")
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
    (root / "taint.txt").write_text("0\n", encoding="utf-8")
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
        self.docs = build_docs(self.cases)

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

    def test_real_plan_generator_integration(self):
        real_generator = rr.load_plan_generator()
        self.assertIsNotNone(real_generator, "real plan generator not available")
        plan = real_generator.build_plan()
        all_cases = plan["cases"]
        short_cases_real = all_cases[:6]
        tsv = real_generator.to_tsv(plan)
        selected_tsv = "\n".join(tsv.splitlines()[1:7]) + "\n"
        docs = build_docs(short_cases_real)
        write_run_dir(self.root, short_cases_real, docs,
                      plan_tsv=tsv, selected_tsv=selected_tsv)
        report = rr.analyze("short", str(self.root), str(self.out),
                            plan_generator=real_generator)
        self.assertEqual(report["status"], "ok")
        self.assertEqual(report["exit_status"], 0)

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
        generator = write_run_dir(self.root, self.cases, self.docs)
        before = sorted(str(path.relative_to(self.root)) for path in self.root.rglob("*"))
        report = rr.analyze("short", str(self.root), str(self.out), plan_generator=generator)
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


class AcceptanceTests(unittest.TestCase):
    def test_identical_rewrite_uses_mask_not_changed_index(self):
        t = E2AnalyzerTests()
        t.setUp()
        event = {"ts": 10100, "slot": 5, "old": t.other, "new": t.other,
                 "phase": PHASE_A, "did": DID_A}
        words, mapping, _, _, _ = t.coherent(a_events=[event])
        meta = int(words[0x201], 16) & ~(31 << 16)
        words[0x201] = "0x%016x" % meta
        evidence, problems, _ = t.analyze(words, mapping)
        self.assertEqual(problems, [])
        self.assertEqual(evidence["events"][0]["slot"], 5)

    def test_extreme_event_count_is_reported_without_crashing(self):
        t = E2AnalyzerTests()
        t.setUp()
        words, mapping, _, _, _ = t.coherent()
        words[0xA] = "0x000000000000ffff"
        _, problems, _ = t.analyze(words, mapping)
        self.assertTrue(any("overflow" in x for x in problems))

    def test_samples_keep_main_occupancy_separate_from_duration_and_runs(self):
        sample = {"req": 2, "miss": 1, "main": 7, "complete": 2,
                  "duration": 100, "case_id": "run1"}
        rows = [("a8g0", "main", sample),
                ("a8g0", "main", dict(sample, main=17, duration=100)),
                ("a8g0", "short", sample),
                ("a8g0", "main", dict(sample, case_id="run2"))]
        groups = rr.group_samples(rows)
        self.assertEqual(len(groups), 3)
        group = next(x for x in groups if x["n"] == 2)
        self.assertEqual(group["median"], 100)
        self.assertEqual(group["controller_main_cycles"], {"min": 7, "median": 12, "max": 17})

    def test_runner_duplicates_and_taint_cannot_pass(self):
        for corruption in ("duplicate", "order", "taint", "missing_p4m"):
            with self.subTest(corruption=corruption), tempfile.TemporaryDirectory() as tmp:
                root, out = Path(tmp) / "input", Path(tmp) / "report"
                cases = short_cases()
                docs = build_docs(cases)
                if corruption == "missing_p4m":
                    del docs["short-cost-a8g0"]["p4m"]
                gen = write_run_dir(root, cases, docs)
                if corruption in ("duplicate", "order"):
                    p = root / "cases.tsv"
                    lines = p.read_text().splitlines()
                    if corruption == "duplicate":
                        lines.append(lines[1])
                    else:
                        lines[1], lines[2] = lines[2], lines[1]
                    p.write_text("\n".join(lines) + "\n")
                elif corruption == "taint":
                    (root / "taint.txt").write_text("1\n")
                report = rr.analyze("short", root, out, plan_generator=gen)
                self.assertNotEqual(report["exit_status"], 0)

    def test_actual_board_runner_to_analyzer(self):
        # Execute the real shell runner with a fixture-producing CLI. This
        # verifies artifact/argv/plan interfaces, not physical FPGA behavior.
        with tempfile.TemporaryDirectory() as tmp:
            base = Path(tmp)
            gen = rr.load_plan_generator()
            cases = gen.build_plan()["cases"][:6]
            docs = build_docs(cases)
            replies = {" ".join(c["argv"]): docs[c["id"]] for c in cases}
            fixture = base / "replies.json"
            fixture.write_text(json.dumps(replies))
            cli = base / "fake-cli"
            cli.write_text("#!/usr/bin/env python3\nimport json,sys\n"
                           "from pathlib import Path\n"
                           "d=json.loads(Path(__file__).with_name('replies.json').read_text())\n"
                           "print(json.dumps(d[' '.join(sys.argv[1:])]))\n")
            cli.chmod(0o755)
            taint = base / "taint"
            taint.write_text("0\n")
            env = dict(os.environ, RESEARCH_PLAN=str(REPO_ROOT / "tests/research-plan.tsv"),
                       RESEARCH_CLI=str(cli), RESEARCH_TAINT=str(taint),
                       RESEARCH_DMESG="true", RESEARCH_UNAME="uname")
            root = base / "run"
            subprocess.run(["sh", str(REPO_ROOT / "tests/board-research.sh"), "short", str(root)],
                           env=env, check=True, capture_output=True, text=True)
            report = rr.analyze("short", root, base / "report")
            self.assertEqual(report["exit_status"], 0, report["runner"])
            self.assertEqual(len(report["summary"]["ok"]), 6)


FOLLOWUP_IDS = (
    "short-cost-solo",
    "short-cost-a8g0",
    "cost-b01-solo",
    "cost-b01-a8g0",
    "cost-b01-a8g2000",
    "cost-b02-a8g0",
    "cost-b02-a8g2000",
    "cost-b02-solo",
    "cost-b03-a8g0",
    "cost-b03-a8g2000",
    "cost-b03-solo",
)
FOLLOWUP_ORDINALS = (4, 5, 67, 69, 72, 74, 77, 78, 79, 82, 83)


def followup_selected_rows(plan_tsv, ordinals):
    rows = plan_tsv.splitlines()
    return "".join(rows[ordinal] + "\n" for ordinal in ordinals)


class CostFollowupAnalyzerTest(unittest.TestCase):
    """The analyzer must expect the fixed 11-case cost-followup selection.

    Host-only fixtures.  Appended as an independent class: another patch owns
    analyze_e2/analyze_cost, and the short/all expectations above are kept.
    """

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.base = Path(self.tmp.name)
        self.root = self.base / "run"
        self.out = self.base / "out"
        self.generator = rr.load_plan_generator()
        self.assertIsNotNone(self.generator, "scripts/research_plan.py is not loadable")
        self.assertTrue(
            hasattr(self.generator, "select_cases"),
            "scripts/research_plan.py must provide select_cases(cases, mode)",
        )
        self.plan = self.generator.build_plan()
        self.full_tsv = self.generator.to_tsv(self.plan)
        self.selected = self.generator.select_cases(self.plan["cases"], "cost-followup")
        self.selected_tsv = followup_selected_rows(self.full_tsv, FOLLOWUP_ORDINALS)
        self.docs = build_docs(self.selected)

    def run_analyze(self, selected_tsv=None, **kwargs):
        write_run_dir(
            self.root,
            self.selected,
            self.docs,
            plan_tsv=self.full_tsv,
            selected_tsv=self.selected_tsv if selected_tsv is None else selected_tsv,
            **kwargs,
        )
        return rr.analyze("cost-followup", str(self.root), str(self.out))

    def test_mode_table_and_selection_are_the_fixed_eleven(self):
        self.assertEqual(
            {key: rr.MODE_CASES[key] for key in ("short", "all", "cost-followup")},
            {"short": 6, "all": 84, "cost-followup": 11},
        )
        self.assertEqual(rr.MODE_CASES.get("cost-fullstats"), 21)
        self.assertEqual([c["id"] for c in self.selected], list(FOLLOWUP_IDS))
        self.assertEqual([c["ordinal"] for c in self.selected], list(FOLLOWUP_ORDINALS))
        self.assertNotEqual(
            [c["id"] for c in self.selected], [c["id"] for c in self.plan["cases"][:11]]
        )

    def test_complete_segment_is_ok_and_expects_exactly_those_rows(self):
        report = self.run_analyze()
        self.assertEqual(report["plan"]["problems"], [])
        self.assertTrue(report["plan"]["verified"])
        self.assertEqual(report["summary"]["expected_cases"], 11)
        self.assertEqual(len(report["summary"]["ok"]), 11)
        self.assertEqual([case["id"] for case in report["cases"]], list(FOLLOWUP_IDS))
        self.assertEqual(
            [case["id"] for case in report["plan"]["expected_cases"]],
            list(FOLLOWUP_IDS),
        )
        self.assertEqual(report["exit_status"], 0)
        self.assertEqual(report["status"], "ok")
        text = (self.out / "report.zh.txt").read_text(encoding="utf-8")
        self.assertIn("cost-followup", text)

    def test_first_eleven_plan_rows_are_rejected_as_the_selection(self):
        rows = self.full_tsv.splitlines()
        wrong = "".join(row + "\n" for row in rows[1:12])
        report = self.run_analyze(selected_tsv=wrong)
        self.assertFalse(report["plan"]["verified"])
        self.assertTrue(
            any("plan.selected.tsv" in item for item in report["plan"]["problems"])
        )
        self.assertNotEqual(report["exit_status"], 0)

    def test_swapped_selected_rows_are_rejected(self):
        rows = self.selected_tsv.splitlines()
        rows[0], rows[1] = rows[1], rows[0]
        report = self.run_analyze(selected_tsv="".join(row + "\n" for row in rows))
        self.assertFalse(report["plan"]["verified"])
        self.assertNotEqual(report["exit_status"], 0)

    def test_missing_followup_case_is_incomplete_and_not_retried(self):
        report = self.run_analyze(
            omit_ids=("cost-b03-solo",), drop_cases_rows=("cost-b03-solo",)
        )
        self.assertEqual(report["status"], "incomplete")
        self.assertEqual(report["exit_status"], 1)
        self.assertEqual(report["summary"]["expected_cases"], 11)
        self.assertIn("cost-b03-solo", report["summary"]["missing"])

    def test_real_runner_cost_followup_matches_the_analyzer(self):
        replies = {
            " ".join(case["argv"]): self.docs[case["id"]] for case in self.selected
        }
        (self.base / "replies.json").write_text(json.dumps(replies), encoding="utf-8")
        cli = self.base / "fake-cli"
        cli.write_text(
            "#!/usr/bin/env python3\n"
            "import json, sys\n"
            "from pathlib import Path\n"
            "d = json.loads(Path(__file__).with_name('replies.json').read_text())\n"
            "print(json.dumps(d[' '.join(sys.argv[1:])]))\n",
            encoding="utf-8",
        )
        cli.chmod(0o755)
        taint = self.base / "taint"
        taint.write_text("0\n", encoding="utf-8")
        env = dict(
            os.environ,
            RESEARCH_PLAN=str(REPO_ROOT / "tests" / "research-plan.tsv"),
            RESEARCH_CLI=str(cli),
            RESEARCH_TAINT=str(taint),
            RESEARCH_DMESG="true",
            RESEARCH_UNAME="uname",
        )
        run_dir = self.base / "runner-run"
        subprocess.run(
            ["sh", str(REPO_ROOT / "tests" / "board-research.sh"),
             "cost-followup", str(run_dir)],
            env=env,
            check=True,
            capture_output=True,
            text=True,
        )
        self.assertEqual(
            (run_dir / "plan.selected.tsv").read_text(encoding="utf-8"),
            self.selected_tsv,
        )
        self.assertEqual(
            (run_dir / "plan.copy.tsv").read_text(encoding="utf-8"), self.full_tsv
        )
        report = rr.analyze("cost-followup", run_dir, self.base / "report")
        self.assertEqual(report["plan"]["problems"], [])
        self.assertEqual(report["exit_status"], 0, report["runner"])
        self.assertEqual(len(report["summary"]["ok"]), 11)
        self.assertEqual([case["id"] for case in report["cases"]], list(FOLLOWUP_IDS))


class FullStatsValidationTests(unittest.TestCase):
    """Focused checks for the fullstats aggregation block."""

    def base_group(self, index=0, count=1):
        return {
            "count": count,
            "sum_main": 2 * count,
            "sum_complete": 1 * count,
            "sum_duration": 10 * count,
            "sum_req": 4 * count,
            "sum_miss": index * count,
            "min_main": 1,
            "max_main": 3,
            "min_complete": 1,
            "max_complete": 2,
            "min_duration": 5,
            "max_duration": 15,
        }

    def analyze(self, groups, *, case=None, error=0, magic=rr.FULLSTATS_MAGIC,
                version=1, group_count=6, stride=16, total=None, eligible=None,
                header_reserved=(), apages=8, require=True,
                win_main_a=100, win_sel_a=1000, win_miss_a=1000):
        case = case or cost_case("a8g0", apages)
        case = dict(case)
        case["require_fullstats"] = require
        if eligible is None:
            eligible = sum(group.get("count", 0) for group in groups)
        words = build_cost(window_cycles=1000, v_done=4096, a_done=3, eligible=eligible)
        # Keep the fixture coherent: the whole-arm session counters must dominate
        # the common window instead of relaxing the analyzer's separation check.
        set_raw_word(words, rr.w("arm_main_a"), max(win_main_a, 7))
        set_raw_word(words, rr.w("win_main_a"), win_main_a)
        set_raw_word(words, rr.w("win_sel_a"), win_sel_a)
        set_raw_word(words, rr.w("win_miss_a"), win_miss_a)
        apply_fullstats(words, groups, error=error, magic=magic, version=version,
                        group_count=group_count, stride=stride, total=total,
                        header_reserved=header_reserved)
        raw = rr.parse_raw_words(words)
        problems = []
        unknowns = []
        evidence = rr.analyze_cost(raw, case, cost_p4m(), None, problems, unknowns)
        return evidence, problems, unknowns

    def test_valid_fullstats_block_is_parsed_and_exposed(self):
        groups = [self.base_group(0), self.base_group(1), self.base_group(2)]
        evidence, problems, unknowns = self.analyze(groups)
        self.assertEqual(problems, [])
        self.assertEqual(unknowns, [])
        fs = evidence["fullstats"]
        self.assertIsNotNone(fs)
        self.assertTrue(fs["valid"])
        self.assertEqual(fs["total"], 3)
        self.assertEqual(fs["group_counts"], [1, 1, 1, 0, 0, 0])
        self.assertEqual(fs["totals"]["sum_miss"], 3)
        self.assertEqual(fs["magic"], "0x49444d4141474731")

    def test_group_means_are_sum_over_count_and_none_when_empty(self):
        groups = [self.base_group(0), {"count": 0}, self.base_group(2)]
        evidence, problems, unknowns = self.analyze(groups)
        self.assertEqual(problems, [])
        decoded = evidence["fullstats"]["groups"]
        self.assertEqual(decoded[0]["means"],
                         {"main": 2.0, "complete": 1.0, "duration": 10.0,
                          "req": 4.0, "miss": 0.0})
        self.assertEqual(decoded[2]["means"],
                         {"main": 2.0, "complete": 1.0, "duration": 10.0,
                          "req": 4.0, "miss": 2.0})
        self.assertEqual(decoded[1]["means"],
                         {"main": None, "complete": None, "duration": None,
                          "req": None, "miss": None})

    def test_missing_fullstats_magic_fails_when_required(self):
        groups = [self.base_group(0)]
        evidence, problems, unknowns = self.analyze(groups, magic=0)
        self.assertIsNone(evidence["fullstats"])
        self.assertTrue(any("does not carry the fullstats magic" in item for item in problems))

    def test_unknown_nonzero_magic_is_rejected(self):
        groups = [self.base_group(0)]
        evidence, problems, unknowns = self.analyze(groups, magic=0x12345678, require=False)
        self.assertIsNone(evidence["fullstats"])
        self.assertTrue(any("unknown nonzero fullstats magic" in item for item in problems))

    def test_absent_fullstats_without_requirement_is_compatible(self):
        words = build_cost(window_cycles=1000, v_done=4096, a_done=3, eligible=0)
        raw = rr.parse_raw_words(words)
        case = cost_case("a8g0", 8)
        problems = []
        unknowns = []
        evidence = rr.analyze_cost(raw, case, cost_p4m(), None, problems, unknowns)
        self.assertIsNone(evidence["fullstats"])
        self.assertEqual(problems, [])

    def test_sticky_error_word_is_rejected(self):
        groups = [self.base_group(0)]
        evidence, problems, unknowns = self.analyze(groups, error=0x1)
        self.assertTrue(any("sticky error word is nonzero" in item for item in problems))

    def test_header_reserved_words_must_be_zero(self):
        groups = [self.base_group(0)]
        evidence, problems, unknowns = self.analyze(groups, header_reserved=(6,))
        self.assertTrue(any("header reserved word" in item for item in problems))

    def test_group_reserved_words_must_be_zero(self):
        groups = [self.base_group(0)]
        groups[0]["reserved"] = [1] + [0] * 6
        evidence, problems, unknowns = self.analyze(groups)
        self.assertTrue(any("reserved words" in item for item in problems))

    def test_empty_group_with_nonzero_field_is_flagged(self):
        groups = [self.base_group(0), {"count": 0, "sum_main": 1}]
        evidence, problems, unknowns = self.analyze(groups)
        self.assertTrue(any("empty but not all fields are zero" in item for item in problems))

    def test_known_group_req_relation_is_checked(self):
        groups = [self.base_group(0)]
        groups[0]["sum_req"] = 3
        evidence, problems, unknowns = self.analyze(groups)
        self.assertTrue(any("sum_req" in item for item in problems))

    def test_known_group_miss_relation_is_checked(self):
        groups = [self.base_group(2)]
        groups[0]["sum_miss"] = 3
        evidence, problems, unknowns = self.analyze(groups)
        self.assertTrue(any("sum_miss" in item for item in problems))

    def test_group5_does_not_impose_fixed_req_or_miss(self):
        groups = [
            {"count": 0}, {"count": 0}, {"count": 0}, {"count": 0}, {"count": 0},
            {
                "count": 2,
                "sum_main": 2,
                # min_complete * count must not exceed sum_complete: min=1, count=2.
                "sum_complete": 2,
                "sum_duration": 10,
                "sum_req": 20,
                "sum_miss": 4,
                "min_main": 1,
                "max_main": 1,
                "min_complete": 1,
                "max_complete": 1,
                "min_duration": 5,
                "max_duration": 5,
            },
        ]
        evidence, problems, unknowns = self.analyze(groups)
        self.assertEqual(problems, [])
        self.assertEqual(evidence["fullstats"]["group_counts"], [0, 0, 0, 0, 0, 2])

    def test_total_sums_must_fit_common_window(self):
        groups = [self.base_group(0)]
        evidence, problems, unknowns = self.analyze(groups, win_main_a=1)
        self.assertTrue(any("exceeds common-window win_main_a" in item for item in problems))

    def test_min_mean_max_relation_is_checked_with_integer_math(self):
        groups = [self.base_group(0)]
        groups[0]["min_main"] = 5
        evidence, problems, unknowns = self.analyze(groups)
        self.assertTrue(any("min*count exceeds sum" in item for item in problems))

    def test_sum_above_max_times_count_is_flagged(self):
        groups = [self.base_group(0)]
        groups[0]["max_main"] = 1
        evidence, problems, unknowns = self.analyze(groups)
        self.assertTrue(any("sum exceeds max*count" in item for item in problems))

    def test_total_must_match_eligible_and_group_counts(self):
        groups = [self.base_group(0)]
        evidence, problems, unknowns = self.analyze(groups, total=2, eligible=1)
        self.assertTrue(any("fullstats total 2 != raw eligible 1" in item for item in problems))
        self.assertTrue(any("fullstats total 2 != sum of group counts 1" in item for item in problems))

    def test_group_sum_relations_are_checked(self):
        groups = [self.base_group(0)]
        groups[0]["sum_duration"] = 1
        evidence, problems, unknowns = self.analyze(groups)
        self.assertTrue(any("sum_main+sum_complete exceeds sum_duration" in item for item in problems))

    def test_solo_case_with_a_fullstats_is_flagged(self):
        groups = [self.base_group(0)]
        evidence, problems, unknowns = self.analyze(groups, apages=0, eligible=1)
        self.assertTrue(any("Solo case has nonzero fullstats A aggregation" in item for item in problems))

    def test_new_magic_is_validated_even_without_require_flag(self):
        groups = [self.base_group(0)]
        evidence, problems, unknowns = self.analyze(groups, require=False, error=0x2)
        self.assertIsNotNone(evidence["fullstats"])
        self.assertFalse(evidence["fullstats"]["valid"])
        self.assertTrue(any("sticky error word is nonzero" in item for item in problems))


FULLSTATS_IDS = (
    "short-cost-solo",
    "short-cost-a8g0",
    "short-cost-a16g0",
    "cost-b01-solo",
    "cost-b01-a1g0",
    "cost-b01-a8g0",
    "cost-b01-a16g0",
    "cost-b01-a8g1000",
    "cost-b01-a8g2000",
    "cost-b02-solo",
    "cost-b02-a1g0",
    "cost-b02-a8g0",
    "cost-b02-a16g0",
    "cost-b02-a8g1000",
    "cost-b02-a8g2000",
    "cost-b03-solo",
    "cost-b03-a1g0",
    "cost-b03-a8g0",
    "cost-b03-a16g0",
    "cost-b03-a8g1000",
    "cost-b03-a8g2000",
)

FULLSTATS_MAIN_CONDITIONS = (
    ("solo", 0, 0),
    ("a1g0", 1, 0),
    ("a8g0", 8, 0),
    ("a16g0", 16, 0),
    ("a8g1000", 8, 1000),
    ("a8g2000", 8, 2000),
)

# Every group block keeps mean main/complete/duration at 3/2/20 cycles, so the
# sampled mean of the same (req, miss) group is directly comparable.
FULLSTATS_FIXTURE_COUNTS = (4, 3, 3, 3, 3, 2)


def fullstats_cases():
    """The fixed 21-case cost-fullstats selection (host fixture, not the board plan)."""
    cases = []
    ordinal = 0
    for condition, apages, gap in (("solo", 0, 0), ("a8g0", 8, 0), ("a16g0", 16, 0)):
        ordinal += 1
        cases.append(cost_case(condition, apages, 64, 8, "short", ordinal=ordinal,
                               gap=gap, case_id="short-cost-%s" % condition))
    for block in ("b01", "b02", "b03"):
        for condition, apages, gap in FULLSTATS_MAIN_CONDITIONS:
            ordinal += 1
            cases.append(cost_case(condition, apages, 4096, 256, "main", ordinal=ordinal,
                                   gap=gap, case_id="cost-%s-%s" % (block, condition)))
    return cases


def fullstats_group_block(index, count):
    """One group with the fixed key requests=4 / IOTLB miss=index."""
    if not count:
        return {"count": 0}
    return {
        "count": count,
        "sum_main": 3 * count,
        "sum_complete": 2 * count,
        "sum_duration": 20 * count,
        "sum_req": 4 * count,
        "sum_miss": index * count,
        "min_main": 1,
        "max_main": 5,
        "min_complete": 1,
        "max_complete": 3,
        "min_duration": 10,
        "max_duration": 30,
    }


def fullstats_other_block(count):
    """Group 5: other request/miss combinations, real sums and no fixed key."""
    if not count:
        return {"count": 0}
    return {
        "count": count,
        "sum_main": 3 * count,
        "sum_complete": 2 * count,
        "sum_duration": 20 * count,
        "sum_req": 8 * count,
        "sum_miss": 3 * count,
        "min_main": 1,
        "max_main": 5,
        "min_complete": 1,
        "max_complete": 3,
        "min_duration": 10,
        "max_duration": 30,
    }


def fullstats_groups(counts=FULLSTATS_FIXTURE_COUNTS):
    groups = [fullstats_group_block(index, counts[index]) for index in range(5)]
    groups.append(fullstats_other_block(counts[5]))
    return groups


def fullstats_cost_doc(case):
    """A coherent cost capture carrying a valid fullstats block."""
    if case["apages"]:
        eligible = sum(FULLSTATS_FIXTURE_COUNTS)
        samples = [{"main": 3, "complete": 2, "req": 4, "miss": 0,
                    "duration": 20, "flags": 0x4}]
        groups = fullstats_groups()
        a_done = 16
        flags = 0x3
    else:
        eligible = 0
        samples = []
        groups = [{"count": 0}] * rr.FULLSTATS_GROUPS
        a_done = 0
        flags = 0x2
    words = build_cost(window_cycles=1000, v_done=case["vcount"], a_done=a_done,
                       eligible=eligible, samples=samples)
    # Keep the whole-arm session counters above the common window, and leave the
    # common window room for the fullstats totals.
    set_raw_word(words, rr.w("arm_main_a"), 100)
    set_raw_word(words, rr.w("arm_comp_a"), 100)
    set_raw_word(words, rr.w("win_main_a"), 100)
    set_raw_word(words, rr.w("win_comp_a"), 100)
    set_raw_word(words, rr.w("win_sel_a"), 200)
    set_raw_word(words, rr.w("win_miss_a"), 100)
    apply_fullstats(words, groups)
    return cost_doc(case, words, cost_p4m(window_cycles=1000, a_done=a_done, flags=flags))


def bare_cost_doc(case):
    """A valid cost capture with no fullstats block at all (old BIT)."""
    a_done = 16 if case["apages"] else 0
    words = build_cost(window_cycles=1000, v_done=case["vcount"], a_done=a_done,
                       eligible=0, samples=[])
    return cost_doc(case, words, cost_p4m(window_cycles=1000, a_done=a_done,
                                          flags=0x3 if case["apages"] else 0x2))


class CostFullstatsAnalyzerTest(unittest.TestCase):
    """The analyzer must expect and enforce the fixed 21-case cost-fullstats segment."""

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.base = Path(self.tmp.name)
        self.root = self.base / "run"
        self.out = self.base / "out"
        self.cases = fullstats_cases()
        self.generator = FakeFullstatsPlan(self.cases)
        self.selected = self.generator.select_cases(self.cases, "cost-fullstats")
        self.docs = {case["id"]: fullstats_cost_doc(case) for case in self.cases}

    def run_analyze(self, **kwargs):
        write_run_dir(self.root, self.selected, self.docs, **kwargs)
        return rr.analyze("cost-fullstats", str(self.root), str(self.out),
                          plan_generator=self.generator)

    def test_mode_selection_is_the_fixed_twenty_one(self):
        self.assertEqual(rr.MODE_CASES["cost-fullstats"], 21)
        self.assertEqual([case["id"] for case in self.cases], list(FULLSTATS_IDS))
        self.assertEqual([case["id"] for case in self.selected], list(FULLSTATS_IDS))
        self.assertTrue(all(case.get("require_fullstats") for case in self.selected))
        self.assertTrue(all("--require-fullstats" in case["argv"] for case in self.selected))

    def test_complete_segment_is_ok_with_fullstats_tables(self):
        report = self.run_analyze()
        self.assertEqual(report["plan"]["problems"], [])
        self.assertTrue(report["plan"]["verified"])
        self.assertEqual(report["summary"]["expected_cases"], 21)
        self.assertEqual(len(report["summary"]["ok"]), 21)
        self.assertEqual([case["id"] for case in report["cases"]], list(FULLSTATS_IDS))
        self.assertEqual(report["exit_status"], 0)
        self.assertEqual(report["status"], "ok")
        first = next(item for item in report["cost"]["per_case"]
                     if item["id"] == "cost-b01-a8g0")
        comparison = first["fullstats_comparison"]
        self.assertIsNotNone(comparison)
        group0 = comparison["groups"][0]
        self.assertEqual(group0["key"], "req=4,miss=0")
        self.assertEqual(group0["count"], 4)
        self.assertEqual(group0["means"], {"main": 3.0, "complete": 2.0, "duration": 20.0,
                                           "req": 4.0, "miss": 0.0})
        self.assertEqual(group0["sampled_same_group"]["n"], 1)
        self.assertEqual(group0["sampled_same_group"]["mean"]["main"], 3.0)
        group5 = comparison["groups"][5]
        self.assertFalse(group5["fixed_key"])
        self.assertEqual(group5["key"], "other")
        self.assertIsNone(group5["sampled_same_group"])
        self.assertEqual(group5["sums"]["req"], 16)
        self.assertEqual(group5["sums"]["miss"], 6)
        text = (self.out / "report.zh.txt").read_text(encoding="utf-8")
        self.assertIn("cost-fullstats", text)
        self.assertIn("req=4,miss=0", text)
        self.assertIn("组5（other）", text)
        self.assertIn("组 5 无固定 req/miss 键", text)
        self.assertIn("旧每 16 次抽样同组", text)
        self.assertIn("总体中位", text)
        self.assertIn("不据此假定统计独立", text)

    def test_missing_required_fullstats_case_fails(self):
        case = next(item for item in self.cases if item["id"] == "cost-b01-a8g0")
        self.docs[case["id"]] = bare_cost_doc(case)
        report = self.run_analyze()
        record = next(item for item in report["cases"] if item["id"] == case["id"])
        self.assertEqual(record["status"], "failed")
        self.assertTrue(any("does not carry the fullstats magic" in item
                            for item in record["problems"]))
        self.assertIn(case["id"], report["summary"]["failed"])
        self.assertEqual(report["exit_status"], 1)

    def test_real_plan_generator_declares_the_same_selection(self):
        generator = rr.load_plan_generator()
        if generator is None or not hasattr(generator, "select_cases"):
            self.skipTest("scripts/research_plan.py is not loadable")
        try:
            selected = generator.select_cases(generator.build_plan()["cases"], "cost-fullstats")
        except Exception as exc:
            self.skipTest("the plan generator does not select cost-fullstats yet: %s" % exc)
        self.assertEqual(len(selected), 21)
        self.assertTrue(all(case.get("require_fullstats") for case in selected))
        self.assertTrue(all("--require-fullstats" in case.get("argv", []) for case in selected))


if __name__ == "__main__":
    unittest.main()
