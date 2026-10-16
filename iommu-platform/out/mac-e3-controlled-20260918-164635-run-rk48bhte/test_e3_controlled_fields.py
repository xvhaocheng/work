"""Small tests for scripts/e3_controlled_fields.py.

Run from the repository root, e.g.::

    python -m pytest tests/test_e3_controlled_fields.py -q
"""

from __future__ import annotations

import json
import sys
from pathlib import Path

import pytest

# Works whether this file lives in <root>/tests/ or <root>/scripts/.
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))

import e3_controlled_fields as m  # noqa: E402

EVENT_ALL = 0x7F
# Raw value the CLI prints for --aggressor none; the module never hardcodes it,
# it only requires input.aggressor_id == round.aggressor_did for that case.
NO_AGGRESSOR = 0xFFFFFFFF

AGGRESSOR_EXPECTED = (1, "cold", "complementary", 0)
HOT_EXPECTED = (1, "hot", "complementary", 128)
SOLO_EXPECTED = ("none", "cold", "complementary", 0)


# --------------------------------------------------------------------------
# fixture builders
# --------------------------------------------------------------------------


def stats_lane(**overrides):
    stats = {
        "submitted": 1,
        "completed": 1,
        "completed_bytes": 64,
        "active_cycles": 10,
        "iotlb_miss": 0,
        "s1_ptw": 0,
        "s2_ptw": 0,
        "refill_gap_ns_sum": 0,
        "refill_gap_ns_max": 0,
        "refill_gap_samples": 0,
        "in_flight_at_start": 0,
        "in_flight_at_end": 0,
        "data_errors": 0,
        "quarantined": 0,
    }
    stats.update(overrides)
    return stats


def stats_zero():
    return {key: 0 for key in m.STATS_FIELDS}


def bank_words(
    *,
    config_d,
    trigger=1,
    a_event=EVENT_ALL,
    v_event=EVENT_ALL,
    a_first_hit=0,
    v_first_hit=1,
    insert_total=0,
    arm_freeze=1000,
    gates=None,
    head_raw=None,
):
    words = [0] * m.E3_WORD_COUNT
    words[m.IDX_MAGIC] = m.E3_BANK_V2
    words[m.IDX_ERROR] = 0
    words[m.IDX_TRIGGER] = trigger
    words[m.IDX_CONFIG_D] = config_d
    words[m.IDX_ARM_FREEZE] = arm_freeze

    if gates is None:
        gates = (2, 2, 1, 1, 1, 5, 1) if trigger else (0, 0, 0, 0, 0, 0, 0)
    words[m.IDX_GATE_BASE : m.IDX_GATE_BASE + 7] = list(gates)

    if head_raw is None:
        head_raw = ((1 << 32) | (m.V_DID << 8) | m.V_ID) if trigger else 0
    words[m.IDX_HEAD] = head_raw
    words[m.IDX_HEAD_IOVA] = 0x4000

    words[m.IDX_INSERT_TOTAL] = insert_total
    words[m.IDX_INSERT_COUNT] = min(insert_total, 2)
    words[m.IDX_INSERT_TRUNC] = 1 if insert_total > 2 else 0

    words[m.IDX_FINAL_MISS + 1] = 1

    # e3_words[96:256]: internal window stage/write lanes.  V is the victim and
    # always issues, so the two V lanes and write lane 0 are populated even when
    # the round does not trigger; the A lanes and write lane 1 stay zero.
    for lane in range(m.INTERNAL_V_LANES):
        for step in range(m.INTERNAL_STAGE_STAGES):
            off = (
                m.INTERNAL_STAGE_BASE
                + lane * m.INTERNAL_STAGE_STRIDE
                + step * m.INTERNAL_STAGE_WORDS
            )
            words[off : off + m.INTERNAL_STAGE_WORDS] = [1, 1, 0, 1, 1, 0, 0, 1]
    words[
        m.INTERNAL_WRITE_BASE : m.INTERNAL_WRITE_BASE + m.INTERNAL_WRITE_WORDS
    ] = [1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

    if not trigger:
        return words

    recv = 100
    words[m.IDX_RESP_RECV] = recv
    words[m.IDX_RESP_FIRST] = recv + config_d + 1
    words[m.IDX_RESP_CONSUMED] = recv + config_d + 2
    words[m.IDX_ACTUAL_D] = config_d

    a_trans_resp = recv + config_d + 2
    v_selected = a_trans_resp + 3

    a_base = m.IDX_A_RECORD
    words[a_base + m.REC_EVENT] = a_event
    words[a_base + m.REC_SEQ] = 1
    words[a_base + m.REC_ID] = m.A_ID
    words[a_base + m.REC_DID] = m.A_DID
    words[a_base + m.REC_IOVA] = 0x9000
    words[a_base + m.REC_TR] = 50
    words[a_base + m.REC_SELECTED] = 60
    words[a_base + m.REC_TRANS_REQ] = 61
    words[a_base + m.REC_FIRST_QUERY] = 62
    words[a_base + m.REC_TRANS_RESP] = a_trans_resp
    words[a_base + m.REC_COMP] = a_trans_resp + 5
    words[a_base + m.REC_RETURN] = a_trans_resp + 10
    words[a_base + m.REC_FIRST_HIT] = a_first_hit
    words[a_base + m.REC_WRITE] = 0
    words[m.IDX_A_SEQ] = 1

    v_base = m.IDX_V_RECORD
    words[v_base + m.REC_EVENT] = v_event
    words[v_base + m.REC_SEQ] = 2
    words[v_base + m.REC_ID] = m.V_ID
    words[v_base + m.REC_DID] = m.V_DID
    words[v_base + m.REC_IOVA] = 0x4000
    words[v_base + m.REC_TR] = 40
    words[v_base + m.REC_SELECTED] = v_selected
    words[v_base + m.REC_TRANS_REQ] = v_selected + 1
    words[v_base + m.REC_FIRST_QUERY] = v_selected + 2
    words[v_base + m.REC_TRANS_RESP] = v_selected + 12
    words[v_base + m.REC_COMP] = v_selected + 17
    words[v_base + m.REC_RETURN] = v_selected + 25
    words[v_base + m.REC_FIRST_HIT] = v_first_hit
    words[v_base + m.REC_WRITE] = 0
    words[m.IDX_V_SEQ] = 2
    return words


def add_intervening(words, index, *, seq, ident, did, iova, write, tr, selected, hit):
    base = m.IDX_INSERT1 if index == 0 else m.IDX_INSERT2
    words[base + m.INS_SEQ] = seq
    words[base + m.INS_ID] = ident
    words[base + m.INS_DID] = did
    words[base + m.INS_IOVA] = iova
    words[base + m.INS_WRITE] = write
    words[base + m.INS_TR] = tr
    words[base + m.INS_SELECTED] = selected
    words[base + m.INS_HIT] = hit


def launcher_words(*, has_a, a_iova=0x5000, status=None, gov=None):
    words = [0] * m.LAUNCHER_WORDS
    expected = 3 if has_a else 1
    words[m.LW_MAGIC] = m.LAUNCH_MAGIC
    words[m.LW_ERROR] = 0
    words[m.LW_STATUS] = (
        m.LAUNCH_STATUS_FROZEN if status is None else status
    )
    words[m.LW_TICKS] = 500
    if has_a:
        words[m.LW_GOA_TS] = 10
        words[m.LW_ASELECT_TS] = 200
        words[m.LW_GOV_TS] = 201 if gov is None else gov
        words[m.LW_VISSUE_TS] = 202
        words[m.LW_AISSUE_TS] = 11
        words[m.LW_VDONE_TS] = 300
        words[m.LW_ADONE_TS] = 250
    else:
        words[m.LW_GOV_TS] = 1
        words[m.LW_VISSUE_TS] = 2
        words[m.LW_VDONE_TS] = 300

    words[m.LW_ISSUE_ID_V] = 1
    words[m.LW_DONE_ID_V] = 1
    if has_a:
        words[m.LW_ISSUE_ID_A] = 5
        words[m.LW_DONE_ID_A] = 5

    words[m.LW_ISSUED_MASK] = expected
    words[m.LW_DONE_MASK] = expected
    words[m.LW_CANCEL_MASK] = 0
    words[m.LW_SEEN] = expected
    words[m.LW_EXPECTED] = expected

    if has_a:
        words[m.LW_SELECT_ID] = m.A_ID
        words[m.LW_SELECT_DID] = m.A_DID
        words[m.LW_SELECT_IOVA] = a_iova
        words[m.LW_SELECT_WRITE] = 0

    words[m.LW_ISSUE_CNT_V] = 1
    words[m.LW_DONE_CNT_V] = 1
    words[m.LW_ISSUE_CNT_A] = 1 if has_a else 0
    words[m.LW_DONE_CNT_A] = 1 if has_a else 0
    words[m.LW_HAS_A] = 1 if has_a else 0
    words[m.LW_HELD] = 0
    words[m.LW_ACTIVE] = 0
    words[m.LW_ZERO] = 0
    return words


def make_round(*, delay, has_a, prep="cold", trigger=None, probe=None, **word_kwargs):
    if trigger is None:
        trigger = 1 if has_a else 0
    words = bank_words(config_d=delay, trigger=trigger, **word_kwargs)
    controlled = {
        "flush_status": 0,
        "prep_complete": 1,
        "stage": m.CTRL_STAGE_DONE,
        "prime": [stats_lane(), stats_lane() if prep == "hot" else stats_zero()],
        "probe": probe if probe is not None else stats_lane(),
        "launcher": launcher_words(has_a=has_a),
        "observer_magic": m.OBSERVER_MAGIC_V6,
        "v_hw_cycles": 12345,
    }
    return {
        "delay": delay,
        "sequence": 1,
        "status": 0,
        "hpm_configured": 1,
        "measurement_started": 1,
        "measurement_complete": 1,
        "victim_did": m.FIXED_VICTIM_DID,
        "aggressor_did": m.A_DID if has_a else NO_AGGRESSOR,
        "victim_elapsed_ns": 1000,
        "snapshot_before_ns": 10,
        "snapshot_after_ns": 1010,
        "drain_ns": 5,
        "victim": stats_lane(),
        "aggressor": stats_lane() if has_a else stats_zero(),
        "e3_words": words,
        "controlled": controlled,
    }


def make_report(expected, *, rounds=None):
    aggressor, prep, pair, first_delay = expected
    has_a = aggressor != "none"
    if rounds is None:
        delay0 = 0 if pair == "zero" else first_delay
        delay1 = 0 if pair == "zero" else 128 - first_delay
        rounds = [
            make_round(delay=delay0, has_a=has_a, prep=prep),
            make_round(delay=delay1, has_a=has_a, prep=prep),
        ]
    return {
        "schema": m.SCHEMA,
        "success": True,
        "stage": "run",
        "errno": 0,
        "run_attempted": True,
        "status": 0,
        "rounds_attempted": 2,
        "rounds_completed": 2,
        "scope": {"hpm": "GO_to_both_drained", "observer": "V_window"},
        "input": {
            "victim_id": 0,
            "aggressor_id": 1 if has_a else NO_AGGRESSOR,
            "victim_pages": 1,
            "aggressor_pages": 1,
            "chunk_bytes": 64,
            "transfers": 1,
            "warmup": 0,
            "timeout_ms": 5000,
            "first_delay": first_delay,
            "prep_mode": 1 if prep == "hot" else 0,
            "prep": prep,
            "pair_mode": 1 if pair == "zero" else 0,
            "pair": pair,
            "flags": 0,
        },
        "mapping": [0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000, 0x8000],
        "rounds": rounds,
    }


# --------------------------------------------------------------------------
# tests
# --------------------------------------------------------------------------


def test_proper_fixture_summary():
    report = make_report(AGGRESSOR_EXPECTED)
    out = m.validate_pair(report, AGGRESSOR_EXPECTED)

    assert out["schema"] == m.SCHEMA
    assert out["pair"]["execution_ok"] is True
    assert out["pair"]["rounds_ok"] is True
    assert out["pair"]["reasons"] == []
    assert out["paired"] is None
    assert out["comparability_reasons"] == []
    json.dumps(out)  # must stay JSON-ready

    r0, r1 = out["rounds"]
    assert r0["sequence"] == 1 and r0["delay"] == 0
    assert r1["delay"] == 128
    assert r0["trigger"] == 1 and r0["classification"] == "triggered"
    assert r0["round_ok"] and r1["round_ok"]
    assert r0["comparable"] and r1["comparable"]
    assert r0["comparability_reasons"] == []
    assert "launcher" in r0["controlled"] and len(r0["controlled"]["launcher"]) == 32
    assert len(r0["stats"]["victim"]) == len(m.STATS_FIELDS)
    assert r0["internal_region"]["validated"] is True
    assert r0["internal_region"]["reasons"] == []
    assert len(r0["internal_region"]["stage"]) == 16
    assert len(r0["internal_region"]["write"]) == 2

    # Readings (E3_CONTROLLED.md:297-301), only from present bits.
    assert r0["readings"]["a_remaining_service"] == 2
    assert r1["readings"]["a_remaining_service"] == 130
    assert r0["readings"]["v_remaining_wait"] == 5
    assert r0["readings"]["v_full_queue"] == 65
    assert r0["readings"]["v_own_translation"] == 11
    assert (
        r0["readings"]["v_full_queue"]
        == r0["target"]["v"]["selected"] - r0["target"]["v"]["tr"]
    )


def test_hot_prep_fixture():
    report = make_report(HOT_EXPECTED)
    out = m.validate_pair(report, HOT_EXPECTED)
    assert out["pair"]["execution_ok"] is True
    assert out["rounds"][0]["delay"] == 128
    assert out["rounds"][1]["delay"] == 0


@pytest.mark.parametrize(
    "path",
    [
        ("rounds", 0, "controlled", "launcher"),
        ("rounds", 0, "e3_words"),
        ("mapping",),
    ],
)
def test_bool_is_rejected_where_integer_is_required(path):
    report = make_report(AGGRESSOR_EXPECTED)
    target = report
    for key in path[:-1]:
        target = target[key]
    target[path[-1]][0] = True
    with pytest.raises(ValueError):
        m.validate_pair(report, AGGRESSOR_EXPECTED)


def test_uint64_range_is_enforced():
    for bad in (-1, 1 << 64):
        report = make_report(AGGRESSOR_EXPECTED)
        report["rounds"][0]["controlled"]["launcher"][m.LW_TICKS] = bad
        with pytest.raises(ValueError):
            m.validate_pair(report, AGGRESSOR_EXPECTED)


def test_launcher_timing_failure_is_reported_not_raised():
    report = make_report(AGGRESSOR_EXPECTED)
    launcher = report["rounds"][0]["controlled"]["launcher"]
    launcher[m.LW_GOV_TS] = launcher[m.LW_ASELECT_TS] + 2
    report["success"] = False
    report["errno"] = 71  # EPROTO, as the CLI would report

    out = m.validate_pair(report, AGGRESSOR_EXPECTED)
    r0, r1 = out["rounds"]
    assert r0["round_ok"] is False
    assert "launcher_gov_not_aselect_plus_1" in r0["reasons"]
    assert r1["round_ok"] is True  # rounds stay independent
    assert out["pair"]["execution_ok"] is False
    assert "success_false" in out["pair"]["reasons"]


def test_launcher_status_failure_is_reported():
    report = make_report(AGGRESSOR_EXPECTED)
    launcher = report["rounds"][1]["controlled"]["launcher"]
    launcher[m.LW_STATUS] = m.LAUNCH_STATUS_FROZEN | m.LAUNCH_STATUS_RUNNING
    out = m.validate_pair(report, AGGRESSOR_EXPECTED)
    assert "launcher_status_not_frozen" in out["rounds"][1]["reasons"]
    assert out["rounds"][0]["round_ok"] is True


def test_probe_error_is_reported_not_raised():
    report = make_report(AGGRESSOR_EXPECTED)
    report["rounds"][1]["controlled"]["probe"] = stats_lane(iotlb_miss=1)
    report["success"] = False
    report["errno"] = 71

    out = m.validate_pair(report, AGGRESSOR_EXPECTED)
    assert "probe_iotlb_miss" in out["rounds"][1]["reasons"]
    assert out["rounds"][1]["round_ok"] is False
    assert out["rounds"][0]["round_ok"] is True


def test_solo_nontrigger_is_accepted():
    report = make_report(SOLO_EXPECTED)
    out = m.validate_pair(report, SOLO_EXPECTED)

    r0, r1 = out["rounds"]
    for item in (r0, r1):
        assert item["trigger"] == 0
        assert item["classification"] == "nontrigger"
        assert item["round_ok"] is True  # no trigger is not an execution failure
        assert item["comparability_reasons"] == ["no_trigger"]
        assert item["comparable"] is False
        assert item["readings"]["a_remaining_service"] is None
        assert item["target"] is None
    assert out["pair"]["execution_ok"] is True
    assert out["comparability_reasons"] == [
        "round0:no_trigger",
        "round1:no_trigger",
    ]


def test_internal_region_write_lane_is_validated():
    report = make_report(AGGRESSOR_EXPECTED)
    words = report["rounds"][0]["e3_words"]
    words[m.INTERNAL_WRITE_BASE + 14] = 1  # reserved word of the V write lane

    out = m.validate_pair(report, AGGRESSOR_EXPECTED)
    r0, r1 = out["rounds"]
    assert r0["round_ok"] is False
    assert r0["internal_region"]["validated"] is False
    assert "internal_write[0]_reserved" in r0["reasons"]
    assert r1["round_ok"] is True  # rounds stay independent
    assert out["pair"]["execution_ok"] is False


def test_internal_region_v_lane_full_is_validated():
    report = make_report(AGGRESSOR_EXPECTED)
    words = report["rounds"][0]["e3_words"]
    words[m.INTERNAL_STAGE_BASE + 1] = 4  # V lane full must be exactly 1

    out = m.validate_pair(report, AGGRESSOR_EXPECTED)
    r0 = out["rounds"][0]
    assert r0["round_ok"] is False
    assert "internal_stage[0][0]_v_full" in r0["reasons"]
    assert out["pair"]["execution_ok"] is False


def test_internal_region_solo_a_lanes_must_be_zero():
    report = make_report(SOLO_EXPECTED)
    words = report["rounds"][0]["e3_words"]
    words[m.INTERNAL_STAGE_BASE + 2 * m.INTERNAL_STAGE_STRIDE] = 1

    out = m.validate_pair(report, SOLO_EXPECTED)
    r0 = out["rounds"][0]
    assert r0["round_ok"] is False
    assert "internal_stage[2][0]_solo_a_nonzero" in r0["reasons"]
    assert out["pair"]["execution_ok"] is False


def test_no_aggressor_sentinel_is_enforced():
    report = make_report(SOLO_EXPECTED)
    report["input"]["aggressor_id"] = 2  # neither 1 nor 0xFFFFFFFF
    with pytest.raises(ValueError):
        m.validate_pair(report, SOLO_EXPECTED)


def test_solo_aggressor_did_must_be_the_sentinel():
    report = make_report(SOLO_EXPECTED)
    report["rounds"][0]["aggressor_did"] = 0

    out = m.validate_pair(report, SOLO_EXPECTED)
    assert out["rounds"][0]["round_ok"] is False
    assert "aggressor_did_sentinel_mismatch" in out["rounds"][0]["reasons"]
    assert out["pair"]["execution_ok"] is False


def test_a_return_and_comp_missing_are_comparability():
    report = make_report(AGGRESSOR_EXPECTED)
    words = report["rounds"][0]["e3_words"]
    words[m.IDX_A_RECORD + m.REC_EVENT] = EVENT_ALL & ~(
        (1 << 5) | (1 << 6)
    )
    words[m.IDX_A_RECORD + m.REC_COMP] = 0
    words[m.IDX_A_RECORD + m.REC_RETURN] = 0

    out = m.validate_pair(report, AGGRESSOR_EXPECTED)
    r0 = out["rounds"][0]
    assert r0["round_ok"] is True
    assert "a_comp_missing" in r0["comparability_reasons"]
    assert "a_return_missing" in r0["comparability_reasons"]
    assert r0["comparable"] is False
    assert r0["readings"]["a_remaining_service"] == 2
    assert r0["target"]["a"]["comp"] is None
    assert r0["target"]["a"]["return"] is None
    assert out["rounds"][1]["comparable"] is True


def test_a_trans_response_missing_is_malformed():
    report = make_report(AGGRESSOR_EXPECTED)
    words = report["rounds"][0]["e3_words"]
    words[m.IDX_A_RECORD + m.REC_EVENT] = EVENT_ALL & ~(1 << 4)
    with pytest.raises(ValueError):
        m.validate_pair(report, AGGRESSOR_EXPECTED)


def test_v_first_miss_is_preserved():
    report = make_report(AGGRESSOR_EXPECTED)
    words = report["rounds"][0]["e3_words"]
    words[m.IDX_V_RECORD + m.REC_FIRST_HIT] = 0

    out = m.validate_pair(report, AGGRESSOR_EXPECTED)
    r0 = out["rounds"][0]
    assert r0["round_ok"] is True
    assert "v_first_miss" in r0["comparability_reasons"]
    assert r0["comparable"] is False


def test_truncated_inserts_are_preserved():
    report = make_report(AGGRESSOR_EXPECTED)
    words = report["rounds"][0]["e3_words"]
    words[m.IDX_INSERT_TOTAL] = 3
    words[m.IDX_INSERT_COUNT] = 2
    words[m.IDX_INSERT_TRUNC] = 1
    add_intervening(
        words, 0, seq=2, ident=m.A_ID, did=m.A_DID, iova=0x9100, write=0,
        tr=90, selected=103, hit=1,
    )
    add_intervening(
        words, 1, seq=3, ident=m.A_ID, did=m.A_DID, iova=0x9200, write=1,
        tr=91, selected=104, hit=1,
    )

    out = m.validate_pair(report, AGGRESSOR_EXPECTED)
    r0 = out["rounds"][0]
    assert r0["classification"] == "truncated"
    assert r0["truncated"] is True
    assert "inserts_truncated" in r0["comparability_reasons"]
    assert r0["round_ok"] is True
    assert len(r0["intervening"]) == 2
    assert r0["intervening"][1]["write"] == 1


def test_success_true_but_round_failed_is_reported():
    report = make_report(AGGRESSOR_EXPECTED)
    launcher = report["rounds"][0]["controlled"]["launcher"]
    launcher[m.LW_GOV_TS] += 5  # success flag left true on purpose
    out = m.validate_pair(report, AGGRESSOR_EXPECTED)
    assert "success_true_but_round_failed" in out["pair"]["reasons"]
    assert out["pair"]["execution_ok"] is False


def test_expected_mismatch_raises():
    report = make_report(AGGRESSOR_EXPECTED)
    with pytest.raises(ValueError):
        m.validate_pair(report, (1, "hot", "complementary", 0))


@pytest.mark.parametrize(
    "mutate",
    [
        lambda r: r.pop("schema"),
        lambda r: r.__setitem__("schema", "idma.e3_controlled.v2"),
        lambda r: r.__setitem__("success", "true"),
        lambda r: r["rounds"][0].pop("e3_words"),
        lambda r: r["rounds"][0].__setitem__("e3_words", r["rounds"][0]["e3_words"][:255]),
        lambda r: r["rounds"][0]["controlled"].__setitem__(
            "launcher", r["rounds"][0]["controlled"]["launcher"][:31]
        ),
        lambda r: r.__setitem__("mapping", r["mapping"][:7]),
        lambda r: r.__setitem__("rounds", r["rounds"][:1]),
        lambda r: r.__setitem__("rounds_attempted", 0),
        lambda r: r.__setitem__("run_attempted", False),
        lambda r: r["input"].__setitem__("chunk_bytes", 128),
        lambda r: r["input"].__setitem__("warmup", 1),
        lambda r: r["input"].__setitem__("timeout_ms", 0),
    ],
)
def test_malformed_documents_raise(mutate):
    report = make_report(AGGRESSOR_EXPECTED)
    mutate(report)
    with pytest.raises(ValueError):
        m.validate_pair(report, AGGRESSOR_EXPECTED)
