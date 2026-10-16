"""Validation for the ``idma.e3_controlled.v1`` report (E3 controlled, v6).

The report is printed by ``references/idma_e3_controlled_00087.c``
(``print_json``, lines 314-348) and carries one fixed 64 B controlled pair: two
rounds, each with the E3 bank v2 words (256), the per-lane driver statistics and
the raw 32-word launcher record.

``validate_pair(report, expected)`` returns a JSON-ready summary.  It raises
``ValueError`` only when the document cannot be trusted as a report of this
experiment (schema, types, bool where an integer is required, uint64 range,
array lengths, fixed 64 B inputs, ``expected`` vs ``input`` mismatch, a pair that
never ran, or E3 bank structure that ``scripts/e3_queue_fields.py`` already
treats as impossible).  Everything the CLI itself classifies -- a failed
launcher record, a probe miss, a missing A COMP/RETURN, no trigger, a truncated
insert region, a V first-query miss -- is reported per round as ``reasons`` /
``comparability_reasons`` so one bad round never hides the other.

Deliberate limits of this batch:

* ``e3_words[96:256]`` (internal window stage / write lanes) is validated with
  the layout and conservation of ``scripts/assess_e3.py:108-125``, revised for
  the controlled experiment (a V lane full count is 1, not ``2 * transfers``,
  and a solo run requires zero A lanes).  See ``_validate_internal_region``.
* no throughput/MiB quantity is derived from the 64 B diagnostics;
* no paired D128-D0/delta or causal claim is produced (``paired`` is ``None``);
* timestamps are compared only with other *present* timestamps of the same clock
  domain (driver clock vs. launcher tick domain are never mixed).
"""

from __future__ import annotations

from collections.abc import Mapping, Sequence
from typing import Any, Dict, List, NamedTuple, Optional

__all__ = ["validate_pair"]

SCHEMA = "idma.e3_controlled.v1"

UINT64_MAX = (1 << 64) - 1
INT32_MIN = -(1 << 31)
INT32_MAX = (1 << 31) - 1

E3_WORD_COUNT = 256
MAPPING_WORDS = 8
LAUNCHER_WORDS = 32
PRIME_COUNT = 2
ROUND_COUNT = 2

OBSERVER_MAGIC_V6 = 0x49444D414F425306
E3_BANK_V2 = 0x49444D4145330002
LAUNCH_MAGIC = 0x49444D414C4E4301

CTRL_STAGE_DONE = 8
PREP_COLD = 0
PREP_HOT = 1
PAIR_COMPLEMENTARY = 0
PAIR_ZERO = 1

FIXED_VICTIM_ID = 0
FIXED_VICTIM_DID = 10
FIXED_VICTIM_PAGES = 1
FIXED_AGGRESSOR_PAGES = 1
FIXED_CHUNK_BYTES = 64
FIXED_TRANSFERS = 1
FIXED_WARMUP = 0
TIMEOUT_MIN_MS = 1
TIMEOUT_MAX_MS = 60000
A_IOVA_MAPPING_INDEX = 4

V_ID = 0
V_DID = 10
A_ID = 4
A_DID = 11
VALID_ID_DID = frozenset({(V_ID, V_DID), (A_ID, A_DID)})

# ``IDMA_EXPERIMENT_NO_AGGRESSOR`` (linux/include/uapi/linux/idma.h).  The CLI
# prints exactly this value for ``--aggressor none``; it is a confirmed UAPI
# constant, not an input-derived sentinel.
NO_AGGRESSOR = 0xFFFFFFFF

# --- e3_words indices: mirror of scripts/e3_queue_fields.py:23-50 -----------
IDX_MAGIC = 0
IDX_ERROR = 1
IDX_TRIGGER = 2
IDX_CONFIG_D = 3
IDX_RESP_RECV = 4
IDX_RESP_FIRST = 5
IDX_RESP_CONSUMED = 6
IDX_ACTUAL_D = 7
IDX_V_SEQ = 10
IDX_A_SEQ = 11
IDX_INSERT_TOTAL = 12
IDX_ARM_FREEZE = 16
IDX_FINAL_HIT = 17
IDX_FINAL_MISS = 21
IDX_GATE_BASE = 25
IDX_A_RECORD = 32
IDX_INSERT1 = 46
IDX_INSERT2 = 54
IDX_INSERT_TRUNC = 62
IDX_INSERT_COUNT = 63
IDX_V_RECORD = 64
IDX_PRE_HIT = 78
IDX_PRE_MISS = 82
IDX_PRE_TR = 86
IDX_PRE_COMP = 90
IDX_HEAD = 94
IDX_HEAD_IOVA = 95

# --- internal window stage / write lanes: e3_words[96:256] ------------------
# Stage lanes: 96 + lane*32 + stage*8, eight words per block, four lanes and
# four stages (scripts/assess_e3.py:114-119).  Write lanes: 224 + lane*16
# (scripts/assess_e3.py:120-123).
INTERNAL_STAGE_BASE = 96
INTERNAL_STAGE_STRIDE = 32
INTERNAL_STAGE_WORDS = 8
INTERNAL_STAGE_STAGES = 4
INTERNAL_STAGE_LANES = 4
INTERNAL_V_LANES = 2
INTERNAL_WRITE_BASE = 224
INTERNAL_WRITE_STRIDE = 16
INTERNAL_WRITE_WORDS = 16
INTERNAL_WRITE_LANES = 2
INTERNAL_WRITE_RESERVED = 2
INTERNAL_V_FULL = 1
INTERNAL_STAGE_FIELDS = (
    "entered",
    "full",
    "carry",
    "sum",
    "max",
    "start",
    "end",
    "area",
)

# Request record offsets (base 32 for A, base 64 for V).
REC_SEQ = 0
REC_ID = 1
REC_DID = 2
REC_IOVA = 3
REC_EVENT = 4
REC_TR = 5
REC_SELECTED = 6
REC_TRANS_REQ = 7
REC_FIRST_QUERY = 8
REC_TRANS_RESP = 9
REC_COMP = 10
REC_RETURN = 11
REC_FIRST_HIT = 12
REC_WRITE = 13

# Intervening record offsets (base 46 and 54).
INS_SEQ = 0
INS_ID = 1
INS_DID = 2
INS_IOVA = 3
INS_WRITE = 4
INS_TR = 5
INS_SELECTED = 6
INS_HIT = 7

# Event bitmap bits follow the seven timestamp fields in REC_TR..REC_RETURN.
EVENT_BITS = 0x7F

# The trigger point is e3_words[4] (response received), never the A record's
# first-query field.  TR / selected / trans request / trans response are
# required to read the round; first query, comp and return may legitimately be
# outside the V window and are then classified as comparability, not failure.
A_REQUIRED_EVENT_BITS = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 4)
A_COMP_BIT = 1 << 5
A_RETURN_BIT = 1 << 6

RECORD_TIMESTAMP_FIELDS = (
    "tr",
    "selected",
    "trans_request",
    "first_query",
    "trans_response",
    "comp",
    "return",
)

# Readings from the delivery document (E3_CONTROLLED.md:297-301):
#   A remaining service  = w41 - w4   (A trans response - trigger)
#   V remaining wait     = w70 - w4   (V selected - trigger)
#   V full queue         = w70 - w69  (V selected - V TR)
#   V own translation    = w73 - w71  (V trans response - V trans request)
A_TRANS_RESP_WORD = IDX_A_RECORD + REC_TRANS_RESP
V_TR_WORD = IDX_V_RECORD + REC_TR
V_SELECTED_WORD = IDX_V_RECORD + REC_SELECTED
V_TRANS_REQ_WORD = IDX_V_RECORD + REC_TRANS_REQ
V_TRANS_RESP_WORD = IDX_V_RECORD + REC_TRANS_RESP

# --- launcher record indices (references/idma_e3_controlled_00087.c:44-67) --
LW_MAGIC = 0
LW_ERROR = 1
LW_STATUS = 2
LW_TICKS = 3
LW_GOA_TS = 4
LW_ASELECT_TS = 5
LW_GOV_TS = 6
LW_VISSUE_TS = 7
LW_AISSUE_TS = 8
LW_VDONE_TS = 9
LW_ADONE_TS = 10
LW_ISSUE_ID_V = 11
LW_ISSUE_ID_A = 12
LW_DONE_ID_V = 13
LW_DONE_ID_A = 14
LW_ISSUED_MASK = 15
LW_DONE_MASK = 16
LW_CANCEL_MASK = 17
LW_SEEN = 18
LW_EXPECTED = 19
LW_SELECT_ID = 20
LW_SELECT_DID = 21
LW_SELECT_IOVA = 22
LW_SELECT_WRITE = 23
LW_ISSUE_CNT_V = 24
LW_ISSUE_CNT_A = 25
LW_DONE_CNT_V = 26
LW_DONE_CNT_A = 27
LW_HAS_A = 28
LW_HELD = 29
LW_ACTIVE = 30
LW_ZERO = 31

LAUNCH_STATUS_RUNNING = 1 << 1
LAUNCH_STATUS_FROZEN = 1 << 2
LAUNCH_STATUS_ABORTED = 1 << 3

LAUNCHER_TIMESTAMP_NAMES = {
    LW_GOA_TS: "goA",
    LW_ASELECT_TS: "Aselect",
    LW_GOV_TS: "goV",
    LW_VISSUE_TS: "Vissue",
    LW_AISSUE_TS: "Aissue",
    LW_VDONE_TS: "Vdone",
    LW_ADONE_TS: "Adone",
}

# Fields of struct idma_experiment_stats as printed by print_stats().
STATS_FIELDS = (
    "submitted",
    "completed",
    "completed_bytes",
    "active_cycles",
    "iotlb_miss",
    "s1_ptw",
    "s2_ptw",
    "refill_gap_ns_sum",
    "refill_gap_ns_max",
    "refill_gap_samples",
    "in_flight_at_start",
    "in_flight_at_end",
    "data_errors",
    "quarantined",
)

INPUT_KEYS = (
    "victim_id",
    "aggressor_id",
    "victim_pages",
    "aggressor_pages",
    "chunk_bytes",
    "transfers",
    "warmup",
    "timeout_ms",
    "first_delay",
    "prep_mode",
    "prep",
    "pair_mode",
    "pair",
    "flags",
)


class _Expected(NamedTuple):
    aggressor: Any
    has_aggressor: bool
    prep: str
    pair: str
    first_delay: int


# --------------------------------------------------------------------------
# generic shape helpers
# --------------------------------------------------------------------------


def _mapping(value: Any, label: str) -> Mapping:
    if not isinstance(value, Mapping):
        raise ValueError(f"{label} must be a JSON object, got {type(value).__name__}")
    return value


def _field(obj: Mapping, key: str, label: str) -> Any:
    if key not in obj:
        raise ValueError(f"{label}.{key} is missing")
    return obj[key]


def _uint64(value: Any, label: str) -> int:
    if isinstance(value, bool) or not isinstance(value, int):
        raise ValueError(f"{label} must be an integer, got {value!r}")
    if value < 0 or value > UINT64_MAX:
        raise ValueError(f"{label} is outside uint64: {value!r}")
    return value


def _int32(value: Any, label: str) -> int:
    if isinstance(value, bool) or not isinstance(value, int):
        raise ValueError(f"{label} must be an integer, got {value!r}")
    if value < INT32_MIN or value > INT32_MAX:
        raise ValueError(f"{label} is outside C int range: {value!r}")
    return value


def _boolean(value: Any, label: str) -> bool:
    if not isinstance(value, bool):
        raise ValueError(f"{label} must be a JSON boolean, got {value!r}")
    return value


def _string(value: Any, label: str) -> str:
    if not isinstance(value, str):
        raise ValueError(f"{label} must be a JSON string, got {value!r}")
    return value


def _uint_field(obj: Mapping, key: str, label: str) -> int:
    return _uint64(_field(obj, key, label), f"{label}.{key}")


def _uint_list(value: Any, count: int, label: str) -> List[int]:
    if not isinstance(value, (list, tuple)):
        raise ValueError(f"{label} must be a list, got {type(value).__name__}")
    if len(value) != count:
        raise ValueError(f"{label} length {len(value)} != {count}")
    return [_uint64(item, f"{label}[{i}]") for i, item in enumerate(value)]


# --------------------------------------------------------------------------
# statistics mirrors (references/idma_e3_controlled_00087.c:122-148)
# --------------------------------------------------------------------------


def _stats(value: Any, label: str) -> Dict[str, int]:
    obj = _mapping(value, label)
    return {key: _uint_field(obj, key, label) for key in STATS_FIELDS}


def _stats_all_zero(stats: Mapping) -> bool:
    return all(value == 0 for value in stats.values())


def _lane_reasons(stats: Mapping, label: str) -> List[str]:
    """Mirror of lane_exact_1() (references/idma_e3_controlled_00087.c:132)."""
    reasons: List[str] = []
    if stats["submitted"] != 1:
        reasons.append(f"{label}_submitted_not_1")
    if stats["completed"] != 1:
        reasons.append(f"{label}_completed_not_1")
    if stats["completed_bytes"] != FIXED_CHUNK_BYTES:
        reasons.append(f"{label}_completed_bytes_not_64")
    if stats["data_errors"]:
        reasons.append(f"{label}_data_errors")
    if stats["quarantined"]:
        reasons.append(f"{label}_quarantined")
    if stats["s2_ptw"]:
        reasons.append(f"{label}_s2_ptw")
    if stats["in_flight_at_end"]:
        reasons.append(f"{label}_in_flight_at_end")
    return reasons


def _probe_reasons(stats: Mapping) -> List[str]:
    """Mirror of probe_exact_1() (references/idma_e3_controlled_00087.c:142)."""
    reasons = _lane_reasons(stats, "probe")
    if stats["iotlb_miss"]:
        reasons.append("probe_iotlb_miss")
    if stats["s1_ptw"]:
        reasons.append("probe_s1_ptw")
    return reasons


# --------------------------------------------------------------------------
# launcher mirrors (references/idma_e3_controlled_00087.c:150-204)
# --------------------------------------------------------------------------


def _launcher_timestamp_reasons(launcher: Sequence[int], *, has_a: bool) -> List[str]:
    """Timestamp bounds/timing of the raw launcher record.

    E3_CONTROLLED.md:110/134: ticks run from PREP to the end of drain and then
    freeze, and goA/goV store ``ticks + 1`` sampled on the frontend edge, so no
    timestamp may exceed ``ticks + 1``.  E3_CONTROLLED.md:59: once the A read is
    actually selected the V go is issued on the next registered cycle, i.e.
    ``goV == Aselect + 1``.  Only the launcher tick domain is compared here.
    """
    reasons: List[str] = []
    bound = launcher[LW_TICKS] + 1
    for index, name in LAUNCHER_TIMESTAMP_NAMES.items():
        if launcher[index] > bound:
            reasons.append(f"launcher_ts_bound_{name}")

    if not launcher[LW_GOV_TS] or not launcher[LW_VISSUE_TS] or not launcher[LW_VDONE_TS]:
        reasons.append("launcher_v_timestamp_zero")
    if not (
        launcher[LW_GOV_TS]
        <= launcher[LW_VISSUE_TS]
        <= launcher[LW_VDONE_TS]
    ):
        reasons.append("launcher_v_timestamp_order")

    if has_a:
        if not (
            launcher[LW_GOA_TS]
            and launcher[LW_ASELECT_TS]
            and launcher[LW_AISSUE_TS]
            and launcher[LW_ADONE_TS]
        ):
            reasons.append("launcher_a_timestamp_zero")
        if launcher[LW_GOV_TS] != launcher[LW_ASELECT_TS] + 1:
            reasons.append("launcher_gov_not_aselect_plus_1")
        if (
            launcher[LW_GOA_TS] > launcher[LW_AISSUE_TS]
            or launcher[LW_AISSUE_TS] > launcher[LW_ASELECT_TS]
            or launcher[LW_AISSUE_TS] > launcher[LW_ADONE_TS]
        ):
            reasons.append("launcher_a_timestamp_order")
    elif (
        launcher[LW_GOA_TS]
        or launcher[LW_ASELECT_TS]
        or launcher[LW_AISSUE_TS]
        or launcher[LW_ADONE_TS]
    ):
        reasons.append("launcher_a_timestamp_without_aggressor")

    return reasons


def _launcher_reasons(
    launcher: Sequence[int], *, has_a: bool, a_iova: int
) -> List[str]:
    """Mirror of launcher_ok() (references/idma_e3_controlled_00087.c:150)."""
    reasons: List[str] = []
    expected = 3 if has_a else 1
    a_expected = 1 if has_a else 0

    if launcher[LW_MAGIC] != LAUNCH_MAGIC:
        reasons.append("launcher_magic")
    if launcher[LW_ERROR] != 0:
        reasons.append("launcher_error")
    if launcher[LW_STATUS] != LAUNCH_STATUS_FROZEN:
        reasons.append("launcher_status_not_frozen")
    if launcher[LW_HELD] or launcher[LW_ACTIVE] or launcher[LW_ZERO]:
        reasons.append("launcher_not_released")

    for index, name in (
        (LW_EXPECTED, "expected"),
        (LW_SEEN, "seen"),
        (LW_ISSUED_MASK, "issued_mask"),
        (LW_DONE_MASK, "done_mask"),
    ):
        if launcher[index] != expected:
            reasons.append(f"launcher_{name}")

    if launcher[LW_CANCEL_MASK] != 0:
        reasons.append("launcher_cancel_mask")

    if not launcher[LW_ISSUE_ID_V] or launcher[LW_ISSUE_ID_V] != launcher[LW_DONE_ID_V]:
        reasons.append("launcher_v_issue_done_id")
    if launcher[LW_ISSUE_CNT_V] != 1 or launcher[LW_DONE_CNT_V] != 1:
        reasons.append("launcher_v_count")

    if has_a:
        if not launcher[LW_ISSUE_ID_A] or launcher[LW_ISSUE_ID_A] != launcher[LW_DONE_ID_A]:
            reasons.append("launcher_a_issue_done_id")
    elif launcher[LW_ISSUE_ID_A] or launcher[LW_DONE_ID_A]:
        reasons.append("launcher_a_id_without_aggressor")

    if (
        launcher[LW_ISSUE_CNT_A] != a_expected
        or launcher[LW_DONE_CNT_A] != a_expected
    ):
        reasons.append("launcher_a_count")
    if launcher[LW_HAS_A] != a_expected:
        reasons.append("launcher_has_a")

    if has_a:
        if launcher[LW_SELECT_ID] != A_ID:
            reasons.append("launcher_select_id")
        if launcher[LW_SELECT_DID] != A_DID:
            reasons.append("launcher_select_did")
        if launcher[LW_SELECT_IOVA] != a_iova:
            reasons.append("launcher_select_iova")
        if launcher[LW_SELECT_WRITE] != 0:
            reasons.append("launcher_select_write")
    elif (
        launcher[LW_SELECT_ID]
        or launcher[LW_SELECT_DID]
        or launcher[LW_SELECT_IOVA]
        or launcher[LW_SELECT_WRITE]
    ):
        reasons.append("launcher_select_without_aggressor")

    reasons.extend(_launcher_timestamp_reasons(launcher, has_a=has_a))
    return reasons


# --------------------------------------------------------------------------
# E3 bank v2 (adapted from scripts/e3_queue_fields.py:107-219, 222-538)
# --------------------------------------------------------------------------


def _validate_record_timestamps(
    words: Sequence[int],
    base: int,
    *,
    required_bits: int,
    require_all_bits: bool,
    label: str,
) -> None:
    """Copy of e3_queue_fields.py:107 with the same bit semantics.

    Only *present* bits take part in the monotonic comparison, and only present
    bits are bounded by the ARM freeze counter (same clock domain).
    """
    event = words[base + REC_EVENT]
    if event & ~EVENT_BITS:
        raise ValueError(f"{label} event bitmap has reserved bits set: {event:#x}")
    if require_all_bits and (event & EVENT_BITS) != EVENT_BITS:
        raise ValueError(f"{label} event bitmap incomplete: {event:#x}")
    if (event & required_bits) != required_bits:
        raise ValueError(f"{label} event bitmap missing required bits: {event:#x}")

    timestamps: List[int] = []
    for bit in range(7):
        value = words[base + REC_TR + bit]
        present = bool(event & (1 << bit))
        if present and value > words[IDX_ARM_FREEZE]:
            raise ValueError(
                f"{label} timestamp bit {bit} {value} > ARM freeze "
                f"{words[IDX_ARM_FREEZE]}"
            )
        timestamps.append(value)

    previous = 0
    for bit in range(7):
        if event & (1 << bit):
            if timestamps[bit] < previous:
                raise ValueError(f"{label} timestamps are not monotonic")
            previous = timestamps[bit]


def _validate_intervening(
    words: Sequence[int],
    count: int,
    *,
    a_trans_resp: int,
    v_selected: int,
    arm_freeze: int,
) -> List[Dict[str, int]]:
    """Copy of e3_queue_fields.py:140 (sequence/id/did/direction identity)."""
    records: List[Dict[str, int]] = []
    for index in range(count):
        base = IDX_INSERT1 if index == 0 else IDX_INSERT2
        seq = words[base + INS_SEQ]
        ident = words[base + INS_ID]
        did = words[base + INS_DID]
        iova = words[base + INS_IOVA]
        write = words[base + INS_WRITE]
        tr = words[base + INS_TR]
        selected = words[base + INS_SELECTED]
        hit = words[base + INS_HIT]
        label = f"intervening[{index}]"

        if (ident, did) not in VALID_ID_DID:
            raise ValueError(f"{label} invalid AXI ID/DID {ident}/{did}")
        if seq <= 0:
            raise ValueError(f"{label} sequence number must be positive")
        if write not in (0, 1):
            raise ValueError(f"{label} direction {write} is not 0 or 1")
        if hit not in (0, 1, 2):
            raise ValueError(f"{label} invalid first-hit result {hit}")
        if tr == 0 or selected == 0:
            raise ValueError(f"{label} TR/selected timestamp zero")
        if tr > selected:
            raise ValueError(f"{label} TR {tr} > selected {selected}")
        if selected < a_trans_resp:
            raise ValueError(
                f"{label} selected {selected} before A response {a_trans_resp}"
            )
        if selected > v_selected:
            raise ValueError(
                f"{label} selected {selected} after V selected {v_selected}"
            )
        if tr > arm_freeze or selected > arm_freeze:
            raise ValueError(f"{label} timestamp beyond ARM freeze {arm_freeze}")

        if (seq, ident, did, write) == (
            words[IDX_A_RECORD + REC_SEQ],
            A_ID,
            A_DID,
            words[IDX_A_RECORD + REC_WRITE],
        ):
            raise ValueError(f"{label} duplicates trigger A request")
        if (seq, ident, did, write) == (
            words[IDX_V_RECORD + REC_SEQ],
            V_ID,
            V_DID,
            words[IDX_V_RECORD + REC_WRITE],
        ):
            raise ValueError(f"{label} duplicates target V request")

        records.append(
            {
                "seq": seq,
                "id": ident,
                "did": did,
                "iova": iova,
                "write": write,
                "tr": tr,
                "selected": selected,
                "hit": hit,
            }
        )

    seen = set()
    for record in records:
        key = (record["seq"], record["id"], record["did"], record["write"])
        if key in seen:
            raise ValueError("duplicate intervening record identity")
        seen.add(key)
    if len(records) == 2 and records[0]["selected"] > records[1]["selected"]:
        raise ValueError("intervening records selected times are out of order")
    return records


def _record_view(words: Sequence[int], base: int) -> Dict[str, Any]:
    """Record fields; timestamp fields are reported only when their bit is set."""
    event = words[base + REC_EVENT]
    view: Dict[str, Any] = {
        "seq": words[base + REC_SEQ],
        "id": words[base + REC_ID],
        "did": words[base + REC_DID],
        "iova": words[base + REC_IOVA],
        "event": event,
        "write": words[base + REC_WRITE],
        "first_hit": words[base + REC_FIRST_HIT],
    }
    for bit, name in enumerate(RECORD_TIMESTAMP_FIELDS):
        view[name] = words[base + REC_TR + bit] if event & (1 << bit) else None
    return view


def _validate_internal_region(
    words: Sequence[int], *, has_a: bool
) -> Dict[str, Any]:
    """Internal window stage / write lanes, ``e3_words[96:256]``.

    Layout and conservation follow ``scripts/assess_e3.py:108-125`` with the
    E3-controlled revisions: a V lane full count is one 64 B formal per round
    (``INTERNAL_V_FULL``), not ``2 * transfers`` as in the queue experiment, and
    a solo run (no aggressor) requires the A lanes to be zero.  The V lanes are
    populated whenever the victim issues, so they are checked unconditionally.

    Violations are collected as ``reasons`` -- a true execution failure, not a
    comparability note -- so one bad round never hides the other.
    """
    reasons: List[str] = []
    stage: List[Dict[str, int]] = []
    write: List[List[int]] = []

    # --- stage lanes: 96 + lane*32 + stage*8 (assess_e3.py:114-119) ------
    for lane in range(INTERNAL_STAGE_LANES):
        lane_base = INTERNAL_STAGE_BASE + lane * INTERNAL_STAGE_STRIDE
        for step in range(INTERNAL_STAGE_STAGES):
            off = lane_base + step * INTERNAL_STAGE_WORDS
            values = [words[off + i] for i in range(INTERNAL_STAGE_WORDS)]
            entered, full, carry, total, maximum, start, end, area = values
            label = f"internal_stage[{lane}][{step}]"

            # scripts/assess_e3.py:18-21 (field_checks)
            if start + entered != full + carry + end:
                reasons.append(f"{label}_conservation")
            if maximum > total:
                reasons.append(f"{label}_sum_max")
            if not full and (total or maximum):
                reasons.append(f"{label}_sum_max_zero")
            if full and total > full * maximum:
                reasons.append(f"{label}_max_bound")
            # scripts/assess_e3.py:117 adjacency between consecutive stages
            if (
                step < INTERNAL_STAGE_STAGES - 1
                and full + carry != words[off + INTERNAL_STAGE_WORDS]
            ):
                reasons.append(f"{label}_adjacency")

            if lane < INTERNAL_V_LANES:
                # scripts/assess_e3.py:118, ``full == 2 * transfers`` -> 1
                if full != INTERNAL_V_FULL:
                    reasons.append(f"{label}_v_full")
                if carry or start or end:
                    reasons.append(f"{label}_v_carry_start_end")
                if area != total:
                    reasons.append(f"{label}_v_area_sum")
            elif not has_a and any(values):
                # scripts/assess_e3.py:119 (solo: A lanes all zero)
                reasons.append(f"{label}_solo_a_nonzero")

            stage.append(
                {
                    "lane": lane,
                    "stage": step,
                    **dict(zip(INTERNAL_STAGE_FIELDS, values)),
                }
            )

    # --- write lanes: 224 + lane*16 (assess_e3.py:120-123) --------------
    for lane in range(INTERNAL_WRITE_LANES):
        off = INTERNAL_WRITE_BASE + lane * INTERNAL_WRITE_STRIDE
        values = [words[off + i] for i in range(INTERNAL_WRITE_WORDS)]
        label = f"internal_write[{lane}]"

        if values[13] + values[0] != values[1] + values[11]:
            reasons.append(f"{label}_conservation")
        if values[1] != values[2] + values[3]:
            reasons.append(f"{label}_split")
        if any(values[INTERNAL_WRITE_WORDS - INTERNAL_WRITE_RESERVED :]):
            reasons.append(f"{label}_reserved")
        if lane == 0:
            if (
                values[0] != INTERNAL_V_FULL
                or values[1] != INTERNAL_V_FULL
                or values[2] != INTERNAL_V_FULL
            ):
                reasons.append(f"{label}_v_count")
            if values[3] or values[11] or values[13]:
                reasons.append(f"{label}_v_zero")
        elif not has_a and any(values):
            reasons.append(f"{label}_solo_a_nonzero")

        write.append(values)

    return {
        "validated": not reasons,
        "words": [INTERNAL_STAGE_BASE, 256],
        "stage_lanes": INTERNAL_STAGE_LANES,
        "stages": INTERNAL_STAGE_STAGES,
        "v_lanes": INTERNAL_V_LANES,
        "v_full": INTERNAL_V_FULL,
        "write_lanes": INTERNAL_WRITE_LANES,
        "stage": stage,
        "write": write,
        "reasons": reasons,
    }


def _delta(words: Sequence[int], high: int, low: int, label: str, name: str) -> int:
    value = words[high] - words[low]
    if value < 0:
        raise ValueError(f"{label} {name} delta {value} is negative")
    return value


def _bank_summary(
    words: Sequence[int], label: str, *, has_a: bool, prep: str
) -> Dict[str, Any]:
    """E3 bank v2 checks with the E3-controlled relaxations."""
    if words[IDX_MAGIC] != E3_BANK_V2:
        raise ValueError(
            f"{label}.e3_words[0] magic {words[IDX_MAGIC]:#x} != {E3_BANK_V2:#x}"
        )
    if words[IDX_ERROR] != 0:
        raise ValueError(
            f"{label}.e3_words[1] error bits nonzero: {words[IDX_ERROR]:#x}"
        )

    trigger = words[IDX_TRIGGER]
    if trigger not in (0, 1):
        raise ValueError(f"{label} trigger count {trigger} is not 0 or 1")

    config_d = words[IDX_CONFIG_D]
    if config_d not in (0, 128):
        raise ValueError(f"{label} config D {config_d} is not 0 or 128")

    g25, g26, g27, g28, g29, g30, g31 = words[IDX_GATE_BASE : IDX_GATE_BASE + 7]
    if g25 < g26:
        raise ValueError(f"{label} gate 25 {g25} < gate 26 {g26}")
    if g26 != g27 + g28:
        raise ValueError(f"{label} gate 26 {g26} != gate 27 {g27} + gate 28 {g28}")
    if g29 > g27:
        raise ValueError(f"{label} gate 29 {g29} > gate 27 {g27}")
    if g30 < g27:
        raise ValueError(f"{label} gate 30 {g30} < gate 27 {g27}")
    if g27 > 0 and trigger != 1:
        raise ValueError(f"{label} gate 27 {g27} nonzero but trigger is {trigger}")
    if g27 == 0 and trigger != 0:
        raise ValueError(f"{label} gate 27 zero but trigger is {trigger}")
    if trigger == 0 and g31 != 0:
        raise ValueError(f"{label} no trigger but gate 31 is {g31}")
    if not has_a:
        if g26 != 0:
            raise ValueError(
                f"{label} no aggressor but gate 26 (A-class PTW) is {g26}"
            )
        if trigger != 0:
            raise ValueError(f"{label} no aggressor but trigger is {trigger}")

    head_raw = words[IDX_HEAD]
    if head_raw >> 33:
        raise ValueError(f"{label} head94 has bits above bit32 set: {head_raw:#x}")
    head_valid = (head_raw >> 32) & 1
    head_did = (head_raw >> 8) & 0xFFFFFF
    head_id = head_raw & 0xFF
    head_is_v = bool(head_valid and head_id == V_ID and head_did == V_DID)
    if head_valid and (head_id, head_did) not in VALID_ID_DID:
        raise ValueError(f"{label} head94 invalid identity {head_id}/{head_did}")
    if g31 != (1 if head_is_v else 0):
        raise ValueError(
            f"{label} gate 31 {g31} != head_is_v {int(head_is_v)} "
            f"(head_valid={head_valid}, id={head_id}, did={head_did})"
        )

    for lane in range(4):
        pre_hit = words[IDX_PRE_HIT + lane]
        final_hit = words[IDX_FINAL_HIT + lane]
        pre_miss = words[IDX_PRE_MISS + lane]
        final_miss = words[IDX_FINAL_MISS + lane]
        if pre_hit > final_hit:
            raise ValueError(
                f"{label} pre-hit lane {lane} {pre_hit} > final-hit {final_hit}"
            )
        if pre_miss > final_miss:
            raise ValueError(
                f"{label} pre-miss lane {lane} {pre_miss} > final-miss {final_miss}"
            )

    insert_total = words[IDX_INSERT_TOTAL]
    expected_trunc = 1 if insert_total > 2 else 0
    expected_count = min(insert_total, 2)
    if words[IDX_INSERT_TRUNC] != expected_trunc:
        raise ValueError(
            f"{label} insert trunc {words[IDX_INSERT_TRUNC]} != {expected_trunc} "
            f"(total {insert_total})"
        )
    if words[IDX_INSERT_COUNT] != expected_count:
        raise ValueError(
            f"{label} insert count {words[IDX_INSERT_COUNT]} != {expected_count} "
            f"(total {insert_total})"
        )
    if trigger == 0 and insert_total != 0:
        raise ValueError(f"{label} no trigger but insert total {insert_total} nonzero")

    intervening: List[Dict[str, int]] = []
    target: Optional[Dict[str, Any]] = None
    readings: Dict[str, Any] = {
        "a_remaining_service": None,
        "v_remaining_wait": None,
        "v_full_queue": None,
        "v_own_translation": None,
    }
    comparability: List[str] = []

    if trigger == 1:
        if words[IDX_ACTUAL_D] != config_d:
            raise ValueError(
                f"{label} actual D {words[IDX_ACTUAL_D]} != config D {config_d}"
            )
        if words[IDX_RESP_FIRST] - words[IDX_RESP_RECV] != config_d + 1:
            raise ValueError(
                f"{label} response first - received "
                f"{words[IDX_RESP_FIRST] - words[IDX_RESP_RECV]} != "
                f"config D + 1 ({config_d + 1})"
            )
        if words[IDX_RESP_CONSUMED] < words[IDX_RESP_FIRST]:
            raise ValueError(
                f"{label} response consumed {words[IDX_RESP_CONSUMED]} < "
                f"first {words[IDX_RESP_FIRST]}"
            )
        if not (
            words[IDX_RESP_RECV]
            <= words[IDX_RESP_FIRST]
            <= words[IDX_RESP_CONSUMED]
            <= words[IDX_ARM_FREEZE]
        ):
            raise ValueError(f"{label} response timestamps outside ARM freeze range")

        _validate_record_timestamps(
            words,
            IDX_A_RECORD,
            required_bits=A_REQUIRED_EVENT_BITS,
            require_all_bits=False,
            label=f"{label} A record",
        )
        if words[IDX_A_RECORD + REC_ID] != A_ID or words[IDX_A_RECORD + REC_DID] != A_DID:
            raise ValueError(
                f"{label} A record identity is not ID{A_ID}/DID{A_DID}: "
                f"{words[IDX_A_RECORD + REC_ID]}/{words[IDX_A_RECORD + REC_DID]}"
            )
        if words[IDX_A_RECORD + REC_SEQ] <= 0:
            raise ValueError(f"{label} A record sequence number must be positive")
        if words[IDX_A_RECORD + REC_WRITE] not in (0, 1):
            raise ValueError(
                f"{label} A record direction {words[IDX_A_RECORD + REC_WRITE]} "
                "is not 0 or 1"
            )
        if words[IDX_A_SEQ] != words[IDX_A_RECORD + REC_SEQ]:
            raise ValueError(
                f"{label} e11 A sequence {words[IDX_A_SEQ]} != A record sequence "
                f"{words[IDX_A_RECORD + REC_SEQ]}"
            )

        _validate_record_timestamps(
            words,
            IDX_V_RECORD,
            required_bits=EVENT_BITS,
            require_all_bits=True,
            label=f"{label} V record",
        )
        if words[IDX_V_RECORD + REC_ID] != V_ID or words[IDX_V_RECORD + REC_DID] != V_DID:
            raise ValueError(
                f"{label} V record identity is not ID{V_ID}/DID{V_DID}: "
                f"{words[IDX_V_RECORD + REC_ID]}/{words[IDX_V_RECORD + REC_DID]}"
            )
        if words[IDX_V_RECORD + REC_SEQ] <= 0:
            raise ValueError(f"{label} V record sequence number must be positive")
        if words[IDX_V_RECORD + REC_WRITE] != 0:
            raise ValueError(f"{label} V record is a write")
        if words[IDX_V_RECORD + REC_FIRST_HIT] not in (0, 1):
            raise ValueError(
                f"{label} V record first-hit {words[IDX_V_RECORD + REC_FIRST_HIT]} "
                "not in {0,1}"
            )
        if words[IDX_V_SEQ] != words[IDX_V_RECORD + REC_SEQ]:
            raise ValueError(
                f"{label} e10 V sequence {words[IDX_V_SEQ]} != V record sequence "
                f"{words[IDX_V_RECORD + REC_SEQ]}"
            )

        a_trans_req = words[IDX_A_RECORD + REC_TRANS_REQ]
        trigger_time = words[IDX_RESP_RECV]
        a_trans_resp = words[IDX_A_RECORD + REC_TRANS_RESP]
        v_tr = words[IDX_V_RECORD + REC_TR]
        v_selected = words[IDX_V_RECORD + REC_SELECTED]

        if trigger_time < a_trans_req:
            raise ValueError(
                f"{label} trigger {trigger_time} is not after A trans request "
                f"{a_trans_req}"
            )
        if trigger_time < v_tr:
            raise ValueError(
                f"{label} trigger {trigger_time} is not after V TR {v_tr}"
            )
        if trigger_time > a_trans_resp:
            raise ValueError(
                f"{label} trigger {trigger_time} is not before A response "
                f"{a_trans_resp}"
            )
        if trigger_time > v_selected:
            raise ValueError(
                f"{label} trigger {trigger_time} is not before V selected {v_selected}"
            )
        if a_trans_resp < words[IDX_RESP_CONSUMED]:
            raise ValueError(
                f"{label} A response {a_trans_resp} before response consumed "
                f"{words[IDX_RESP_CONSUMED]}"
            )
        if v_selected < a_trans_resp:
            raise ValueError(
                f"{label} V selected {v_selected} before A response {a_trans_resp}"
            )

        intervening = _validate_intervening(
            words,
            words[IDX_INSERT_COUNT],
            a_trans_resp=a_trans_resp,
            v_selected=v_selected,
            arm_freeze=words[IDX_ARM_FREEZE],
        )

        a_event = words[IDX_A_RECORD + REC_EVENT]
        v_event = words[IDX_V_RECORD + REC_EVENT]

        if not a_event & A_COMP_BIT:
            comparability.append("a_comp_missing")
        if not a_event & A_RETURN_BIT:
            comparability.append("a_return_missing")
        if words[IDX_A_RECORD + REC_FIRST_HIT] != 0 and prep == "cold":
            comparability.append("cold_a_not_miss")
        if words[IDX_V_RECORD + REC_FIRST_HIT] != 1:
            comparability.append("v_first_miss")

        if a_event & (1 << 4):
            readings["a_remaining_service"] = _delta(
                words, A_TRANS_RESP_WORD, IDX_RESP_RECV, label, "a_remaining_service"
            )
        if v_event & (1 << 1):
            readings["v_remaining_wait"] = _delta(
                words, V_SELECTED_WORD, IDX_RESP_RECV, label, "v_remaining_wait"
            )
        if v_event & (1 << 0) and v_event & (1 << 1):
            readings["v_full_queue"] = _delta(
                words, V_SELECTED_WORD, V_TR_WORD, label, "v_full_queue"
            )
        if v_event & (1 << 2) and v_event & (1 << 4):
            readings["v_own_translation"] = _delta(
                words, V_TRANS_RESP_WORD, V_TRANS_REQ_WORD, label, "v_own_translation"
            )

        target = {
            "a": _record_view(words, IDX_A_RECORD),
            "v": _record_view(words, IDX_V_RECORD),
        }

    if trigger == 0:
        classification = "nontrigger"
        comparability.append("no_trigger")
    elif words[IDX_INSERT_TRUNC]:
        classification = "truncated"
        comparability.append("inserts_truncated")
    else:
        classification = "triggered"

    return {
        "magic": words[IDX_MAGIC],
        "error": words[IDX_ERROR],
        "trigger": trigger,
        "classification": classification,
        "config_d": config_d,
        "aggressor_present": bool(has_a),
        "gates": {
            "ptw_handshakes": g25,
            "trigger_class": g26,
            "with_waiting_v": g27,
            "without_waiting_v": g28,
            "old_head_not_v": g29,
            "waiting_cycles": g30,
            "trigger_head_is_v": g31,
        },
        "head": {
            "raw": head_raw,
            "valid": head_valid,
            "did": head_did if head_valid else None,
            "id": head_id if head_valid else None,
            "iova": words[IDX_HEAD_IOVA] if head_valid else None,
            "is_v": head_is_v,
        },
        "history": {
            "pre_hit": list(words[IDX_PRE_HIT : IDX_PRE_HIT + 4]),
            "pre_miss": list(words[IDX_PRE_MISS : IDX_PRE_MISS + 4]),
            "pre_tr": list(words[IDX_PRE_TR : IDX_PRE_TR + 4]),
            "pre_comp": list(words[IDX_PRE_COMP : IDX_PRE_COMP + 4]),
            "final_hit": list(words[IDX_FINAL_HIT : IDX_FINAL_HIT + 4]),
            "final_miss": list(words[IDX_FINAL_MISS : IDX_FINAL_MISS + 4]),
        },
        "insert_total": insert_total,
        "inserted": words[IDX_INSERT_COUNT],
        "truncated": bool(words[IDX_INSERT_TRUNC]),
        "intervening": intervening,
        "timings": {
            "response_received": words[IDX_RESP_RECV],
            "response_first": words[IDX_RESP_FIRST],
            "response_consumed": words[IDX_RESP_CONSUMED],
            "actual_added": words[IDX_ACTUAL_D],
            "arm_freeze": words[IDX_ARM_FREEZE],
            "a_translation_response": (
                words[IDX_A_RECORD + REC_TRANS_RESP] if trigger else None
            ),
            "v_selected": words[IDX_V_RECORD + REC_SELECTED] if trigger else None,
        },
        "target": target,
        "readings": readings,
        "internal_region": _validate_internal_region(words, has_a=has_a),
        "comparability_reasons": comparability,
    }


# --------------------------------------------------------------------------
# expected tuple and inputs
# --------------------------------------------------------------------------


def _parse_expected(expected: Any) -> _Expected:
    if not isinstance(expected, (tuple, list)) or len(expected) != 4:
        raise ValueError(
            "expected must be a 4-tuple (aggressor, prep, pair, first_delay)"
        )
    aggressor, prep, pair, first_delay = expected

    if isinstance(aggressor, str) and aggressor == "none":
        has_aggressor = False
    elif not isinstance(aggressor, bool) and aggressor in (1, "1"):
        has_aggressor = True
    else:
        raise ValueError(
            f"expected aggressor is {aggressor!r}, expected 'none' or 1"
        )

    if prep not in ("cold", "hot"):
        raise ValueError(f"expected prep is {prep!r}, expected 'cold' or 'hot'")
    if pair not in ("complementary", "zero"):
        raise ValueError(
            f"expected pair is {pair!r}, expected 'complementary' or 'zero'"
        )
    if isinstance(first_delay, bool) or first_delay not in (0, 128):
        raise ValueError(
            f"expected first_delay is {first_delay!r}, expected 0 or 128"
        )

    return _Expected(aggressor, has_aggressor, prep, pair, int(first_delay))


def _expected_delay(exp: _Expected, index: int) -> int:
    """Mirror of the expected_delay expression in controlled_round_ok()."""
    if exp.pair == "zero":
        return 0
    return (128 - exp.first_delay) if index else exp.first_delay


def _validate_inputs(inputs: Mapping, exp: _Expected) -> int:
    """Fixed 64 B experiment inputs; returns the raw aggressor_id."""
    if _uint_field(inputs, "victim_id", "input") != FIXED_VICTIM_ID:
        raise ValueError(f"input.victim_id is not the fixed {FIXED_VICTIM_ID}")
    aggressor_id = _uint_field(inputs, "aggressor_id", "input")
    if _uint_field(inputs, "victim_pages", "input") != FIXED_VICTIM_PAGES:
        raise ValueError(f"input.victim_pages is not the fixed {FIXED_VICTIM_PAGES}")
    if _uint_field(inputs, "aggressor_pages", "input") != FIXED_AGGRESSOR_PAGES:
        raise ValueError(
            f"input.aggressor_pages is not the fixed {FIXED_AGGRESSOR_PAGES}"
        )
    if _uint_field(inputs, "chunk_bytes", "input") != FIXED_CHUNK_BYTES:
        raise ValueError(
            f"input.chunk_bytes is not the fixed {FIXED_CHUNK_BYTES}"
        )
    if _uint_field(inputs, "transfers", "input") != FIXED_TRANSFERS:
        raise ValueError(f"input.transfers is not the fixed {FIXED_TRANSFERS}")
    if _uint_field(inputs, "warmup", "input") != FIXED_WARMUP:
        raise ValueError(f"input.warmup is not the fixed {FIXED_WARMUP}")

    timeout = _uint_field(inputs, "timeout_ms", "input")
    if not TIMEOUT_MIN_MS <= timeout <= TIMEOUT_MAX_MS:
        raise ValueError(
            f"input.timeout_ms {timeout} outside "
            f"{TIMEOUT_MIN_MS}..{TIMEOUT_MAX_MS}"
        )
    _uint_field(inputs, "flags", "input")

    first_delay = _uint_field(inputs, "first_delay", "input")
    prep_mode = _uint_field(inputs, "prep_mode", "input")
    pair_mode = _uint_field(inputs, "pair_mode", "input")
    prep = _string(_field(inputs, "prep", "input"), "input.prep")
    pair = _string(_field(inputs, "pair", "input"), "input.pair")

    if prep_mode not in (PREP_COLD, PREP_HOT) or prep != (
        "hot" if prep_mode == PREP_HOT else "cold"
    ):
        raise ValueError(
            f"input.prep/prep_mode {prep!r}/{prep_mode} are inconsistent"
        )
    if pair_mode not in (PAIR_COMPLEMENTARY, PAIR_ZERO) or pair != (
        "zero" if pair_mode == PAIR_ZERO else "complementary"
    ):
        raise ValueError(
            f"input.pair/pair_mode {pair!r}/{pair_mode} are inconsistent"
        )
    if first_delay not in (0, 128):
        raise ValueError(
            f"input.first_delay is {first_delay}, expected 0 or 128"
        )
    if pair_mode == PAIR_ZERO and first_delay != 0:
        raise ValueError("input.pair is zero but input.first_delay is nonzero")

    if exp.has_aggressor:
        if aggressor_id != 1:
            raise ValueError(
                f"expected aggressor 1 but input.aggressor_id is {aggressor_id}"
            )
    else:
        if prep_mode != PREP_COLD:
            raise ValueError("solo run (aggressor none) must use cold prep")
        if aggressor_id != NO_AGGRESSOR:
            raise ValueError(
                f"expected aggressor 'none' but input.aggressor_id is "
                f"{aggressor_id}, expected {NO_AGGRESSOR}"
            )

    if prep != exp.prep:
        raise ValueError(
            f"expected prep {exp.prep!r} but input.prep is {prep!r}"
        )
    if pair != exp.pair:
        raise ValueError(
            f"expected pair {exp.pair!r} but input.pair is {pair!r}"
        )
    if first_delay != exp.first_delay:
        raise ValueError(
            f"expected first_delay {exp.first_delay} but input.first_delay is "
            f"{first_delay}"
        )
    return aggressor_id


# --------------------------------------------------------------------------
# per-round validation
# --------------------------------------------------------------------------


def _validate_round(
    raw: Any,
    index: int,
    *,
    exp: _Expected,
    aggressor_id: int,
    mapping: Sequence[int],
) -> Dict[str, Any]:
    label = f"rounds[{index}]"
    entry = _mapping(raw, label)

    expected_delay = _expected_delay(exp, index)
    delay = _uint_field(entry, "delay", label)
    if delay != expected_delay:
        raise ValueError(
            f"{label}.delay is {delay}, expected {expected_delay} for "
            f"prep={exp.prep} pair={exp.pair} first_delay={exp.first_delay} "
            f"round {index}"
        )

    sequence = _uint_field(entry, "sequence", label)
    status = _int32(_field(entry, "status", label), f"{label}.status")
    hpm_configured = _uint_field(entry, "hpm_configured", label)
    measurement_started = _uint_field(entry, "measurement_started", label)
    measurement_complete = _uint_field(entry, "measurement_complete", label)
    victim_did = _uint_field(entry, "victim_did", label)
    aggressor_did = _uint_field(entry, "aggressor_did", label)
    victim_elapsed_ns = _uint_field(entry, "victim_elapsed_ns", label)
    snapshot_before_ns = _uint_field(entry, "snapshot_before_ns", label)
    snapshot_after_ns = _uint_field(entry, "snapshot_after_ns", label)
    drain_ns = _uint_field(entry, "drain_ns", label)

    victim_stats = _stats(_field(entry, "victim", label), f"{label}.victim")
    aggressor_stats = _stats(_field(entry, "aggressor", label), f"{label}.aggressor")

    controlled = _mapping(
        _field(entry, "controlled", label), f"{label}.controlled"
    )
    flush_status = _int32(
        _field(controlled, "flush_status", f"{label}.controlled"),
        f"{label}.controlled.flush_status",
    )
    prep_complete = _uint_field(controlled, "prep_complete", f"{label}.controlled")
    stage = _uint_field(controlled, "stage", f"{label}.controlled")
    observer_magic = _uint_field(
        controlled, "observer_magic", f"{label}.controlled"
    )
    v_hw_cycles = _uint_field(controlled, "v_hw_cycles", f"{label}.controlled")

    prime_raw = _field(controlled, "prime", f"{label}.controlled")
    if not isinstance(prime_raw, (list, tuple)) or len(prime_raw) != PRIME_COUNT:
        raise ValueError(f"{label}.controlled.prime must hold exactly 2 statistics")
    prime = [
        _stats(prime_raw[i], f"{label}.controlled.prime[{i}]")
        for i in range(PRIME_COUNT)
    ]
    probe = _stats(
        _field(controlled, "probe", f"{label}.controlled"),
        f"{label}.controlled.probe",
    )
    launcher = _uint_list(
        _field(controlled, "launcher", f"{label}.controlled"),
        LAUNCHER_WORDS,
        f"{label}.controlled.launcher",
    )

    words = _uint_list(
        _field(entry, "e3_words", label), E3_WORD_COUNT, f"{label}.e3_words"
    )
    bank = _bank_summary(words, label, has_a=exp.has_aggressor, prep=exp.prep)

    reasons: List[str] = []

    # --- controlled_round_ok (references/idma_e3_controlled_00087.c:206) ----
    if not sequence:
        reasons.append("sequence_zero")
    if status:
        reasons.append("status_nonzero")
    if not measurement_started:
        reasons.append("measurement_not_started")
    if not measurement_complete:
        reasons.append("measurement_not_complete")
    if not hpm_configured:
        reasons.append("hpm_not_configured")
    if not prep_complete:
        reasons.append("prep_not_complete")
    if flush_status:
        reasons.append("flush_status_nonzero")
    if stage != CTRL_STAGE_DONE:
        reasons.append("stage_not_done")
    if observer_magic != OBSERVER_MAGIC_V6:
        reasons.append("observer_magic")
    if words[IDX_CONFIG_D] != expected_delay:
        reasons.append("config_d_mismatch")
    if not v_hw_cycles:
        reasons.append("v_hw_cycles_zero")

    if victim_did != FIXED_VICTIM_DID:
        reasons.append("victim_did")
    if exp.has_aggressor:
        if aggressor_did != A_DID:
            reasons.append("aggressor_did")
    elif aggressor_did == 1 or aggressor_did == A_DID:
        reasons.append("aggressor_did_not_absent")
    elif aggressor_did != aggressor_id:
        reasons.append("aggressor_did_sentinel_mismatch")

    reasons.extend(_lane_reasons(victim_stats, "victim"))
    if exp.has_aggressor:
        reasons.extend(_lane_reasons(aggressor_stats, "aggressor"))
    elif not _stats_all_zero(aggressor_stats):
        reasons.append("aggressor_stats_without_aggressor")

    reasons.extend(_lane_reasons(prime[0], "prime_v"))
    if exp.prep == "hot":
        reasons.extend(_lane_reasons(prime[1], "prime_a"))
    elif not _stats_all_zero(prime[1]):
        reasons.append("prime_a_without_hot_prep")

    reasons.extend(_probe_reasons(probe))
    reasons.extend(
        _launcher_reasons(
            launcher,
            has_a=exp.has_aggressor,
            a_iova=mapping[A_IOVA_MAPPING_INDEX],
        )
    )
    reasons.extend(bank["internal_region"]["reasons"])

    summary: Dict[str, Any] = {
        "index": index,
        "sequence": sequence,
        "delay": delay,
        "status": status,
        "hpm_configured": hpm_configured,
        "measurement_started": measurement_started,
        "measurement_complete": measurement_complete,
        "victim_did": victim_did,
        "aggressor_did": aggressor_did,
        "victim_elapsed_ns": victim_elapsed_ns,
        "snapshot_before_ns": snapshot_before_ns,
        "snapshot_after_ns": snapshot_after_ns,
        "drain_ns": drain_ns,
        "stats": {"victim": victim_stats, "aggressor": aggressor_stats},
        "controlled": {
            "flush_status": flush_status,
            "prep_complete": prep_complete,
            "stage": stage,
            "prime": prime,
            "probe": probe,
            "launcher": launcher,
            "observer_magic": observer_magic,
            "v_hw_cycles": v_hw_cycles,
        },
        "trigger": bank["trigger"],
        "classification": bank["classification"],
        "gates": bank["gates"],
        "head": bank["head"],
        "history": bank["history"],
        "insert_total": bank["insert_total"],
        "inserted": bank["inserted"],
        "truncated": bank["truncated"],
        "intervening": bank["intervening"],
        "timings": bank["timings"],
        "target": bank["target"],
        "readings": bank["readings"],
        "internal_region": bank["internal_region"],
        "round_ok": not reasons,
        "reasons": reasons,
        "comparability_reasons": list(bank["comparability_reasons"]),
        "comparable": (not reasons) and not bank["comparability_reasons"],
    }
    return summary


# --------------------------------------------------------------------------
# public entry point
# --------------------------------------------------------------------------


def validate_pair(p: Any, expected: Any) -> Dict[str, Any]:
    """Validate one ``idma.e3_controlled.v1`` report and return a summary.

    ``expected`` is ``(aggressor, prep, pair, first_delay)`` with
    ``aggressor`` = ``"none"`` or ``1``, ``prep`` = ``"cold"``/``"hot"``,
    ``pair`` = ``"complementary"``/``"zero"`` and ``first_delay`` = ``0``/``128``.
    Malformed documents, a document that does not match ``expected``, and a pair
    that never produced two completed rounds raise ``ValueError``.  Everything
    the CLI itself classifies is returned as per-round ``reasons`` /
    ``comparability_reasons``.  No paired delta and no causal claim is produced
    (``"paired"`` is ``None``).
    """
    doc = _mapping(p, "report")
    schema = _string(_field(doc, "schema", "report"), "report.schema")
    if schema != SCHEMA:
        raise ValueError(f"report.schema is {schema!r}, expected {SCHEMA!r}")

    exp = _parse_expected(expected)

    inputs = _mapping(_field(doc, "input", "report"), "input")
    aggressor_id = _validate_inputs(inputs, exp)
    mapping = _uint_list(_field(doc, "mapping", "report"), MAPPING_WORDS, "mapping")

    success = _boolean(_field(doc, "success", "report"), "report.success")
    run_attempted = _boolean(
        _field(doc, "run_attempted", "report"), "report.run_attempted"
    )
    stage = _string(_field(doc, "stage", "report"), "report.stage")
    errno_value = _int32(_field(doc, "errno", "report"), "report.errno")
    pair_status = _int32(_field(doc, "status", "report"), "report.status")
    rounds_attempted = _uint_field(doc, "rounds_attempted", "report")
    rounds_completed = _uint_field(doc, "rounds_completed", "report")

    scope = _mapping(_field(doc, "scope", "report"), "scope")
    for key, want in (("hpm", "GO_to_both_drained"), ("observer", "V_window")):
        got = _string(_field(scope, key, "scope"), f"scope.{key}")
        if got != want:
            raise ValueError(f"scope.{key} is {got!r}, expected {want!r}")

    if not run_attempted:
        raise ValueError(
            "report.run_attempted is false: the controlled ioctl never ran"
        )
    if rounds_attempted != ROUND_COUNT or rounds_completed != ROUND_COUNT:
        raise ValueError(
            f"rounds_attempted/completed {rounds_attempted}/{rounds_completed} "
            f"!= {ROUND_COUNT}/{ROUND_COUNT}"
        )

    rounds_raw = _field(doc, "rounds", "report")
    if not isinstance(rounds_raw, (list, tuple)) or len(rounds_raw) != ROUND_COUNT:
        raise ValueError(f"report.rounds must hold exactly {ROUND_COUNT} rounds")

    rounds = [
        _validate_round(
            rounds_raw[index],
            index,
            exp=exp,
            aggressor_id=aggressor_id,
            mapping=mapping,
        )
        for index in range(ROUND_COUNT)
    ]

    rounds_ok = all(item["round_ok"] for item in rounds)
    pair_reasons: List[str] = []
    if pair_status:
        pair_reasons.append("pair_status_nonzero")
    if errno_value:
        pair_reasons.append("errno_nonzero")
    if not success:
        pair_reasons.append("success_false")
    if success and not rounds_ok:
        pair_reasons.append("success_true_but_round_failed")
    if rounds_ok and not success:
        pair_reasons.append("rounds_ok_but_success_false")
    if not success and not errno_value:
        pair_reasons.append("failure_without_errno")

    comparability: List[str] = []
    for item in rounds:
        for reason in item["comparability_reasons"]:
            comparability.append(f"round{item['index']}:{reason}")

    return {
        "schema": schema,
        "expected": {
            "aggressor": exp.aggressor,
            "has_aggressor": exp.has_aggressor,
            "prep": exp.prep,
            "pair": exp.pair,
            "first_delay": exp.first_delay,
        },
        "scope": {"hpm": scope["hpm"], "observer": scope["observer"]},
        "input": {key: inputs[key] for key in INPUT_KEYS},
        "mapping": mapping,
        "pair": {
            "success": success,
            "stage": stage,
            "errno": errno_value,
            "status": pair_status,
            "run_attempted": run_attempted,
            "rounds_attempted": rounds_attempted,
            "rounds_completed": rounds_completed,
            "rounds_ok": rounds_ok,
            "execution_ok": (not pair_reasons) and rounds_ok,
            "reasons": pair_reasons,
        },
        "rounds": rounds,
        "comparability_reasons": comparability,
        # Intentionally no D128-D0 delta, no significance label and no causal
        # claim: the fixed matrix must be reviewed by Codex first.
        "paired": None,
    }
