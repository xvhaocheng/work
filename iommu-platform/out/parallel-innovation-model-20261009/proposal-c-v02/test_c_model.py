"""Meaningful unittest checks for the c-model contract."""
from __future__ import annotations

import unittest

from c_event_model import (
    Fixture,
    build_case,
    simulate,
    STATUS_OK,
    STATUS_INFEASIBLE,
    STATUS_INVALID,
)
from c_policies import (
    policy_group_first,
    policy_hot_page,
    policy_closure_ack_density,
    policy_max_closure,
    policy_setup_density,
    policy_marginal_unlock_proxy,
)

# FIXTURE_IDS/POLICIES/PROFILES are exported by the runner, not by c_policies.
from run_c_model import FIXTURE_IDS, POLICIES, PROFILES


_MATRIX_CACHE = None


def _matrix_runs():
    """Plan + execute the full 6x2x6 matrix once, cached for reuse.

    Returns a list of (context, case, result) tuples.
    """
    global _MATRIX_CACHE
    if _MATRIX_CACHE is None:
        runs = []
        for fid in FIXTURE_IDS:
            case = build_case(fid)
            for pname, pfunc in POLICIES:
                for profile, overlap in PROFILES:
                    order = pfunc(case, overlap=overlap)
                    res = simulate(case, order, overlap=overlap, drain=True)
                    runs.append((f"{fid}/{pname}/{profile}", case, res))
        _MATRIX_CACHE = runs
    return _MATRIX_CACHE


class TestHandSerial(unittest.TestCase):
    def test_four_L10_x_first_hand_timing(self):
        case = build_case("four_L10")
        order = [0, 1, 3, 5, 2, 4, 6]  # X, Y1, Y2, Y3, Z1, Z2, Z3
        res = simulate(case, order, overlap=False, drain=True)
        self.assertEqual(res["status"], STATUS_OK)
        self.assertEqual(res["ready"], [11, 35, 37, 38])
        self.assertEqual(res["ack"], [12, 36, 38, 39])
        self.assertEqual(res["metrics"]["source_walks"], 3)
        self.assertEqual(res["metrics"]["total_copies"], 7)
        self.assertLessEqual(res["metrics"]["peak_buffer"], 4)
        self.assertEqual(res["metrics"]["makespan"], 39)

    def test_four_L10_yz_first_hand_timing(self):
        case = build_case("four_L10")
        order = [1, 3, 5, 2, 4, 6, 0]  # Y1, Y2, Y3, Z1, Z2, Z3, X
        res = simulate(case, order, overlap=False, drain=True)
        self.assertEqual(res["status"], STATUS_OK)
        by_name = {case.groups[g][0]: res["ready"][g] for g in range(4)}
        ack_by_name = {case.groups[g][0]: res["ack"][g] for g in range(4)}
        self.assertEqual(by_name, {"X": 38, "G1": 24, "G2": 26, "G3": 27})
        self.assertEqual(ack_by_name, {"X": 39, "G1": 25, "G2": 27, "G3": 28})
        self.assertEqual(res["metrics"]["makespan"], 39)
        self.assertEqual(res["metrics"]["source_walks"], 3)
        self.assertEqual(res["metrics"]["total_copies"], 7)
        self.assertLessEqual(res["metrics"]["peak_buffer"], 4)

    def test_closure_ack_density_matches_yz_first_on_four_serial(self):
        case = build_case("four_L10")
        order_closure = policy_closure_ack_density(case, overlap=False)
        res_closure = simulate(case, order_closure, overlap=False, drain=True)
        res_yz = simulate(case, [1, 3, 5, 2, 4, 6, 0], overlap=False, drain=True)
        self.assertEqual(res_closure["status"], STATUS_OK)
        self.assertEqual(res_closure["ready"], res_yz["ready"])
        self.assertEqual(res_closure["ack"], res_yz["ack"])

    def test_eight_prefix_hand_timing(self):
        case = build_case("eight_L10")
        # H1..H4 are groups 4..7. W1=7, W2=9, W3=11, U1=8.
        res = simulate(case, [7, 9, 11, 8], overlap=False, drain=True)
        self.assertEqual(res["status"], STATUS_OK)
        self.assertEqual(res["ready"][4], 24)
        self.assertEqual(res["ack"][4], 25)

        # Y1=1, Y2=3, Y3=5, Z1=2, Z2=4, Z3=6.
        res2 = simulate(case, [1, 3, 5, 2, 4, 6], overlap=False, drain=True)
        self.assertEqual(res2["status"], STATUS_OK)
        self.assertEqual(res2["ack"][1], 25)
        self.assertEqual(res2["ack"][2], 27)
        self.assertEqual(res2["ack"][3], 28)


class TestSourceCacheLRU(unittest.TestCase):
    def _abac_case(self):
        # cap=2, one group, dispatch order A,B,A,C (ids 0,1,2,3).
        return Fixture(
            id="lru_cap2_ABAC",
            groups=[("T", ["A", "B", "A", "C"])],
            L=1.0,
            d=1.0,
            consumer=1.0,
            B=4,
            cap=2,
        )

    def test_lru_evicts_least_recently_used_on_cap2(self):
        # With cap 2 and order A,B,A,C: A is re-dispatched (a hit) after B, so
        # the LRU victim when C is walked must be B (least recently used),
        # leaving cache [A, C]. A FIFO cache (no recency refresh on hit) would
        # instead evict A and leave [B, C]. Checked on BOTH profiles.
        for profile, overlap in (("serial", False), ("overlap", True)):
            case = self._abac_case()
            res = simulate(case, [0, 1, 2, 3], overlap=overlap, drain=True)
            self.assertEqual(res["status"], STATUS_OK, profile)
            self.assertEqual(sorted(res["completed_ids"]), [0, 1, 2, 3], profile)
            self.assertEqual(res["cache"], ["A", "C"], profile)

            evicted = [e["key"] for e in res["trace"] if e["event"] == "cache_evict"]
            self.assertIn("B", evicted, f"{profile}: B should be evicted")
            self.assertNotIn("A", evicted, f"{profile}: A is MRU, not the victim")

            # Every descriptor dispatched exactly once as well.
            starts = [
                e["descriptor"] for e in res["trace"] if e["event"] == "copy_start"
            ]
            self.assertEqual(sorted(starts), [0, 1, 2, 3], profile)


class TestNegativeAndInvariants(unittest.TestCase):
    def test_reject_full_pool_unfinished_head_prefix(self):
        case = build_case("eight_L10")
        # W1,W2,W3,W4 then U1: after W4 the pool is full with no pending ACK.
        res = simulate(case, [7, 9, 11, 13, 8], overlap=False, drain=False)
        self.assertEqual(res["status"], STATUS_INFEASIBLE)

    def test_detect_duplicate_descriptor(self):
        case = build_case("four_L10")
        res = simulate(case, [0, 0], overlap=False, drain=False)
        self.assertEqual(res["status"], STATUS_INVALID)

    def test_no_future_ack_borrowing(self):
        case = build_case("four_L10")
        res = simulate(case, [0, 1, 3, 5, 2, 4, 6], overlap=False, drain=True)
        # Z2 is descriptor 4. It cannot dispatch before G1 ACK at 36.
        self.assertGreaterEqual(res["copy_start"][4], res["ack"][1])
        self.assertEqual(res["ack"][1], 36)

    def test_same_overlap_opportunity_for_controls(self):
        case = build_case("four_L10")
        order = [1, 3, 5, 2, 4, 6, 0]
        serial = simulate(case, order, overlap=False, drain=True)
        overlap = simulate(case, order, overlap=True, drain=True)
        self.assertEqual(serial["status"], STATUS_OK)
        self.assertEqual(overlap["status"], STATUS_OK)
        self.assertLessEqual(overlap["last_copy_time"], serial["last_copy_time"])

    def test_all_fixtures_profiles_policies_are_legal_and_complete(self):
        for ctx, case, res in _matrix_runs():
            self.assertEqual(res["status"], STATUS_OK, ctx)
            self.assertEqual(
                len(res["completed_ids"]),
                len(case.descs),
                f"{ctx}: not all descriptors copied once",
            )
            self.assertEqual(
                sorted(res["completed_ids"]),
                list(range(len(case.descs))),
                f"{ctx}: duplicate or missing descriptor",
            )
            self.assertTrue(
                all(x is not None for x in res["ready"]),
                f"{ctx}: some group not ready",
            )
            self.assertTrue(
                all(x is not None for x in res["ack"]),
                f"{ctx}: some group not ACKed",
            )
            self.assertLessEqual(
                res["metrics"]["peak_buffer"],
                case.B,
                f"{ctx}: peak buffer exceeds B",
            )
            for g, ids in case.group_desc_ids.items():
                self.assertLessEqual(
                    len(ids),
                    case.B,
                    f"{ctx}: group {g} exceeds B",
                )
            # No release before ACK: every ACK time must not precede ready time.
            for g in range(len(case.groups)):
                self.assertGreaterEqual(
                    res["ack"][g],
                    res["ready"][g],
                    f"{ctx}: ACK before ready for group {g}",
                )

    def test_source_authority_present_at_dispatch(self):
        # The copy_start trace must record the actual source key, and that key
        # must be genuinely resident (statically legal same-domain authority;
        # hot target page) in the authority snapshot at dispatch time.
        for ctx, case, res in _matrix_runs():
            saw_dispatch = False
            for ev in res["trace"]:
                if ev["event"] != "copy_start":
                    continue
                saw_dispatch = True
                key = ev["source_key"]
                snap = ev["cache_snapshot"]
                self.assertEqual(
                    key,
                    case.descs[ev["descriptor"]].key,
                    f"{ctx}: source_key does not match descriptor key",
                )
                self.assertIn(
                    key,
                    snap,
                    f"{ctx}: dispatched key {key!r} was not resident at dispatch",
                )
            self.assertTrue(saw_dispatch, f"{ctx}: no copy dispatch recorded")

    def test_trace_occupancy_peak_and_fullcopy_exactly_once(self):
        # Independently rebuild live-input occupancy from the copy_start/ack
        # trace events (not the internal counter). A future ACK can never be
        # borrowed, so occupancy must never exceed B; it must dip to 0 only via
        # real ACKs; and every descriptor must be copied exactly once.
        for ctx, case, res in _matrix_runs():
            occ = 0
            peak = 0
            starts = {}
            ends = {}
            for ev in res["trace"]:
                if ev["event"] == "copy_start":
                    occ += 1
                    peak = max(peak, occ)
                    self.assertLessEqual(
                        occ,
                        case.B,
                        f"{ctx}: live inputs depth {occ} > B (future ACK borrowed)",
                    )
                    did = ev["descriptor"]
                    starts[did] = starts.get(did, 0) + 1
                elif ev["event"] == "copy_end":
                    did = ev["descriptor"]
                    ends[did] = ends.get(did, 0) + 1
                elif ev["event"] == "ack":
                    occ -= ev["released"]
                    self.assertGreaterEqual(
                        occ, 0, f"{ctx}: negative occupancy after ACK"
                    )
            self.assertEqual(
                peak, res["metrics"]["peak_buffer"], f"{ctx}: reconstructed peak"
            )
            self.assertEqual(occ, 0, f"{ctx}: live inputs not fully released")
            self.assertEqual(len(starts), len(case.descs), f"{ctx}: not all copied")
            self.assertTrue(
                all(c == 1 for c in starts.values()),
                f"{ctx}: a descriptor was copied more than once",
            )
            self.assertEqual(
                sorted(starts),
                sorted(ends),
                f"{ctx}: copy_start/copy_end mismatch",
            )
            self.assertTrue(
                all(c == 1 for c in ends.values()),
                f"{ctx}: copy_end not exactly once",
            )


if __name__ == "__main__":
    unittest.main()
