#!/usr/bin/env python3
"""
Four-DMA boundary abstract BFS model proposal.

This is a single-file, standard-library-only Python 3 script.  It does not
execute commands, use the network, or read project files.  It only writes
results.json and traces.json under --output.

The model is the abstract AR-header parking/dispatch progress model described
by the parent task.  It is NOT an RTL proof, NOT an occupancy model, and NOT a
production-reachability model.  It intentionally omits TLB, PTW cycles, FAST,
AXI VALID cycle stability, full iDMA internals, payload/control buffering,
original AR FIFO occupancy, and source-held descriptor storage.
"""

import argparse
import json
import os
from collections import deque


# ---------------------------------------------------------------------------
# Constants and basic helpers
# ---------------------------------------------------------------------------

# Fixed virtual-W obligation order.
WRITE_ORDER = (
    (0, 0), (0, 1),
    (2, 0), (2, 1),
    (1, 0), (1, 1),
    (3, 0), (3, 1),
)

BANK_STREAMS = {
    0: (0, 2),
    1: (1, 3),
}

BEATS = (0, 1)

FIXED_BANK0 = ((2, 0), (2, 1), (0, 0), (0, 1))
FIXED_BANK1 = ((1, 0), (1, 1), (3, 0), (3, 1))


def require(cond, msg):
    """Explicit self-check.  Raises AssertionError on invariant violation."""
    if not cond:
        raise AssertionError(msg)


def tag_to_list(tag):
    return [tag[0], tag[1]]


def bank_of(tag):
    return tag[0] & 1


def interleavings(seq1, seq2):
    """
    Yield all interleavings of two sequences that preserve the order inside
    each sequence.  Used to generate the 6 bank input orders per bank.
    """
    if not seq1:
        yield tuple(seq2)
    elif not seq2:
        yield tuple(seq1)
    else:
        for rest in interleavings(seq1[1:], seq2):
            yield (seq1[0],) + rest
        for rest in interleavings(seq1, seq2[1:]):
            yield (seq2[0],) + rest


def generate_bank_orders(bank):
    streams = BANK_STREAMS[bank]
    seqs = [tuple((d, beat) for beat in BEATS) for d in streams]
    return list(interleavings(seqs[0], seqs[1]))


def generate_cases():
    b0_orders = generate_bank_orders(0)
    b1_orders = generate_bank_orders(1)
    cases = []
    for i, b0 in enumerate(b0_orders):
        for j, b1 in enumerate(b1_orders):
            case_id = f"case_b0_{i}_b1_{j}"
            cases.append({
                "case_id": case_id,
                "bank_orders": [b0, b1],
                "is_fixed_sample": (b0 == FIXED_BANK0 and b1 == FIXED_BANK1),
            })
    require(any(c["is_fixed_sample"] for c in cases), "fixed sample was not generated")
    return cases


def policy_list():
    policies = [("local2", {})]
    for H in range(1, 7):
        policies.append(("globalH", {"H": H}))
    for H in (2, 3):
        policies.append(("owner-reserve2-H", {"H": H}))
    return policies


# ---------------------------------------------------------------------------
# State helpers
# ---------------------------------------------------------------------------

# State is:
#   (cursors, ready, pool, writecursor)
# where:
#   cursors = (c0, c1) int per bank
#   ready   = (r0, r1) bool per bank
#   pool    = tuple of tags, canonical sorted by (source, beat)
#   writecursor = int index into WRITE_ORDER


def current_write(state):
    wc = state[3]
    if wc < len(WRITE_ORDER):
        return WRITE_ORDER[wc]
    return None


def current_owner_bank(state):
    tag = current_write(state)
    if tag is None:
        return None
    return bank_of(tag)


def park_allowed(policy_name, params, state, b):
    """
    Capacity rule for parking bank b's currently translated head.
    Direct is handled separately and is always allowed when its own
    preconditions hold.
    """
    pool = state[2]
    total = len(pool)

    if policy_name == "local2":
        cnt = sum(1 for t in pool if bank_of(t) == b)
        return cnt < 2

    if policy_name == "globalH":
        H = params["H"]
        return total < H

    if policy_name == "owner-reserve2-H":
        H = params["H"]
        owner = current_owner_bank(state)
        if owner is None:
            return False
        if b == owner:
            # Current owner bank: only total < H.
            return total < H
        # Non-owner bank: reserve 2 slots for the owner bank.
        # Interpretation used here: the non-owner bank's own parked count
        # must stay below H-2.  Thus H=2 reserves both slots for the owner,
        # and H=3 allows at most one non-owner parked head.
        cnt_b = sum(1 for t in pool if bank_of(t) == b)
        return total < H and cnt_b < (H - 2)

    raise ValueError(f"unknown policy: {policy_name}")


def is_success(state, bank_orders):
    cursors, ready, pool, wc = state
    return (
        wc == len(WRITE_ORDER)
        and cursors == (len(bank_orders[0]), len(bank_orders[1]))
        and not any(ready)
        and len(pool) == 0
    )


def successors(state, bank_orders, policy_name, params):
    """
    All enabled abstract transitions from state.
    Every transition is one atomic step.
    """
    cursors, ready, pool, wc = state
    succ = []

    # translate(b): one shared translation engine, one bank head per step.
    for b in (0, 1):
        if not ready[b] and cursors[b] < len(bank_orders[b]):
            new_ready = list(ready)
            new_ready[b] = True
            succ.append((("translate", b), (cursors, tuple(new_ready), pool, wc)))

    # park(b): move current head into pool if capacity allows.
    for b in (0, 1):
        if ready[b]:
            tag = bank_orders[b][cursors[b]]
            if park_allowed(policy_name, params, state, b):
                if tag in pool:
                    raise AssertionError(f"attempt to park duplicate tag {tag}")
                new_cursors = list(cursors)
                new_cursors[b] += 1
                new_pool = tuple(sorted(pool + (tag,), key=lambda t: (t[0], t[1])))
                new_ready = list(ready)
                new_ready[b] = False
                succ.append((
                    ("park", b, tag),
                    (tuple(new_cursors), tuple(new_ready), new_pool, wc),
                ))

    # direct(b): current W obligation, no earlier same-source parked.
    cw = current_write(state)
    for b in (0, 1):
        if ready[b]:
            tag = bank_orders[b][cursors[b]]
            if cw is not None and tag == cw:
                d, beat = tag
                earlier = (d, 0) if beat == 1 else None
                if earlier is not None and earlier in pool:
                    continue
                new_cursors = list(cursors)
                new_cursors[b] += 1
                new_ready = list(ready)
                new_ready[b] = False
                succ.append((
                    ("direct", b, tag),
                    (tuple(new_cursors), tuple(new_ready), pool, wc + 1),
                ))

    # dispatch-pool: exact current W obligation in pool, same-source prior retired.
    if cw is not None and cw in pool:
        d, beat = cw
        earlier = (d, 0) if beat == 1 else None
        if earlier is not None and earlier in pool:
            raise AssertionError(
                f"dispatch {cw} while earlier same-source {earlier} is still in pool"
            )
        new_pool = tuple(t for t in pool if t != cw)
        succ.append((("dispatch", cw), (cursors, ready, new_pool, wc + 1)))

    # Deterministic ordering for reproducibility.
    def key(item):
        action, _ = item
        kind = action[0]
        order = {"translate": 0, "park": 1, "direct": 2, "dispatch": 3}
        return (order[kind],) + action[1:]

    succ.sort(key=key)
    return succ


# ---------------------------------------------------------------------------
# Invariant self-checks
# ---------------------------------------------------------------------------

def validate_state(state, bank_orders, policy_name, params):
    """
    Substantive conservation checks.

    Checks:
      - cursors/ready/writecursor ranges
      - unique parked and ready requests
      - read capture == parked union retired
      - pool subset of write order and disjoint from retired
      - retired is exactly a prefix of write order
      - ready heads are not already read/parked/retired
      - same-stream order
      - policy capacity bounds
      - terminal consistency
    """
    cursors, ready, pool, wc = state

    for b in (0, 1):
        require(0 <= cursors[b] <= len(bank_orders[b]),
                f"cursor out of range: bank={b}, cursors={cursors}")
    require(0 <= wc <= len(WRITE_ORDER),
            f"writecursor out of range: {wc}")

    heads = []
    for b in (0, 1):
        if ready[b]:
            require(cursors[b] < len(bank_orders[b]),
                    f"ready but cursor at end: bank={b}, state={state}")
            heads.append(bank_orders[b][cursors[b]])

    require(len(set(pool)) == len(pool),
            f"duplicate pool tags: {pool}")
    require(len(set(heads)) == len(heads),
            f"duplicate ready heads: {heads}")
    require(not (set(pool) & set(heads)),
            f"tag both parked and ready: {set(pool) & set(heads)}")

    read_captured = set()
    for b in (0, 1):
        read_captured.update(bank_orders[b][:cursors[b]])
    retired = set(WRITE_ORDER[:wc])

    require(read_captured == (set(pool) | retired),
            f"read-capture mismatch: read={read_captured}, pool={set(pool)}, retired={retired}")
    require(set(pool) <= set(WRITE_ORDER),
            f"pool contains unknown tag: {set(pool) - set(WRITE_ORDER)}")
    require(not (set(pool) & retired),
            f"pool intersects retired: {set(pool) & retired}")
    require(retired == set(WRITE_ORDER[:wc]),
            f"retired is not exactly a write-order prefix: {retired}")

    for h in heads:
        require(h not in read_captured,
                f"ready head already read: {h}")
        require(h not in set(pool),
                f"ready head already parked: {h}")
        require(h not in retired,
                f"ready head already retired: {h}")

    for b in (0, 1):
        if not ready[b] and cursors[b] < len(bank_orders[b]):
            tag = bank_orders[b][cursors[b]]
            require(tag not in read_captured,
                    f"untranslated head already read: {tag}")
            require(tag not in set(pool),
                    f"untranslated head already parked: {tag}")
            require(tag not in retired,
                    f"untranslated head already retired: {tag}")

    for d in range(4):
        t0 = (d, 0)
        t1 = (d, 1)
        if t1 in read_captured:
            require(t0 in read_captured,
                    f"same-stream order violation: {t1} read before {t0}")
        if t1 in retired:
            require(t0 in retired,
                    f"same-stream order violation: {t1} retired before {t0}")
        if t1 in set(pool):
            require(t0 in read_captured,
                    f"same-stream order violation: {t1} parked before {t0} read")
        if t1 in heads:
            require(t0 in read_captured,
                    f"same-stream order violation: {t1} ready before {t0} read")

    if policy_name == "local2":
        for b in (0, 1):
            cnt = sum(1 for t in pool if bank_of(t) == b)
            require(cnt <= 2,
                    f"local2 bank{b} pool overflow: {cnt}")
        require(len(pool) <= 4,
                f"local2 total pool overflow: {len(pool)}")

    elif policy_name == "globalH":
        H = params["H"]
        require(len(pool) <= H,
                f"globalH pool overflow: {len(pool)} > {H}")

    elif policy_name == "owner-reserve2-H":
        H = params["H"]
        require(len(pool) <= H,
                f"owner-reserve2 pool overflow: {len(pool)} > {H}")
        owner = current_owner_bank(state)
        for b in (0, 1):
            if owner is not None and b != owner:
                cnt = sum(1 for t in pool if bank_of(t) == b)
                require(cnt <= H - 2,
                        f"owner-reserve2 non-owner bank{b} overflow: {cnt} > {H - 2}")

    else:
        raise ValueError(f"unknown policy: {policy_name}")

    if wc == len(WRITE_ORDER):
        require(cursors == (len(bank_orders[0]), len(bank_orders[1])),
                f"terminal writecursor but cursors={cursors}")
        require(not any(ready),
                f"terminal writecursor but ready={ready}")
        require(len(pool) == 0,
                f"terminal writecursor but pool={pool}")

    return True


# ---------------------------------------------------------------------------
# Serialization helpers for traces
# ---------------------------------------------------------------------------

def state_to_dict(state, bank_orders):
    cursors, ready, pool, wc = state
    heads = []
    for b in (0, 1):
        if ready[b]:
            tag = bank_orders[b][cursors[b]]
            heads.append({
                "bank": b,
                "tag": tag_to_list(tag),
                "source": tag[0],
                "beat": tag[1],
            })
    cw = current_write(state)
    return {
        "cursors": [cursors[0], cursors[1]],
        "ready": [bool(ready[0]), bool(ready[1])],
        "pool": [tag_to_list(t) for t in pool],
        "pool_bank0": [tag_to_list(t) for t in pool if bank_of(t) == 0],
        "pool_bank1": [tag_to_list(t) for t in pool if bank_of(t) == 1],
        "writecursor": wc,
        "current_write": tag_to_list(cw) if cw is not None else None,
        "owner_bank": current_owner_bank(state),
        "ready_heads": heads,
    }


def blocked_heads(state, bank_orders, policy_name, params):
    """
    Ready heads that currently cannot be direct-retired and cannot be parked.
    This is only diagnostic output; enabled transitions are authoritative.
    """
    cw = current_write(state)
    res = []
    for b in (0, 1):
        if state[1][b]:
            tag = bank_orders[b][state[0][b]]
            direct_ok = False
            if cw is not None and tag == cw:
                d, beat = tag
                earlier = (d, 0) if beat == 1 else None
                if earlier is None or earlier not in state[2]:
                    direct_ok = True
            park_ok = park_allowed(policy_name, params, state, b)
            if not direct_ok and not park_ok:
                res.append({
                    "bank": b,
                    "tag": tag_to_list(tag),
                    "source": tag[0],
                    "beat": tag[1],
                    "direct_ok": direct_ok,
                    "park_ok": park_ok,
                })
    return res


def action_to_str(action):
    kind = action[0]
    if kind == "translate":
        return f"translate(bank={action[1]})"
    if kind == "park":
        return f"park(bank={action[1]}, tag=({action[2][0]},{action[2][1]}))"
    if kind == "direct":
        return f"direct(bank={action[1]}, tag=({action[2][0]},{action[2][1]}))"
    if kind == "dispatch":
        return f"dispatch(tag=({action[1][0]},{action[1][1]}))"
    raise ValueError(f"unknown action: {action}")


def action_to_detail(action):
    kind = action[0]
    if kind == "translate":
        return {"kind": "translate", "bank": action[1]}
    if kind == "park":
        return {"kind": "park", "bank": action[1], "tag": tag_to_list(action[2])}
    if kind == "direct":
        return {"kind": "direct", "bank": action[1], "tag": tag_to_list(action[2])}
    if kind == "dispatch":
        return {"kind": "dispatch", "tag": tag_to_list(action[1])}
    raise ValueError(f"unknown action: {action}")


# ---------------------------------------------------------------------------
# BFS
# ---------------------------------------------------------------------------

def analyze_case(case, policy_name, params):
    bank_orders = case["bank_orders"]
    initial = ((0, 0), (False, False), (), 0)
    validate_state(initial, bank_orders, policy_name, params)

    queue = deque([initial])
    visited = {initial}
    parent = {initial: None}

    success_states = []
    deadlock_states = []

    while queue:
        state = queue.popleft()

        if is_success(state, bank_orders):
            success_states.append(state)

        succ = successors(state, bank_orders, policy_name, params)

        if not succ and not is_success(state, bank_orders):
            deadlock_states.append(state)

        for action, ns in succ:
            validate_state(ns, bank_orders, policy_name, params)
            if ns not in visited:
                visited.add(ns)
                parent[ns] = (state, action)
                queue.append(ns)

    def reconstruct(target):
        if target is None:
            return None
        path = []
        cur = target
        while parent[cur] is not None:
            prev, action = parent[cur]
            path.append((action, cur))
            cur = prev
        path.reverse()
        return {
            "initial": state_to_dict(initial, bank_orders),
            "steps": [
                {
                    "action": action_to_str(action),
                    "action_detail": action_to_detail(action),
                    "state": state_to_dict(st, bank_orders),
                    "current_write": (
                        tag_to_list(current_write(st))
                        if current_write(st) is not None
                        else None
                    ),
                    "owner_bank": current_owner_bank(st),
                    "blocked_heads": blocked_heads(
                        st, bank_orders, policy_name, params
                    ),
                }
                for action, st in path
            ],
            "final": state_to_dict(target, bank_orders),
            "length": len(path),
        }

    success_witness = reconstruct(success_states[0]) if success_states else None
    deadlock_witness = reconstruct(deadlock_states[0]) if deadlock_states else None

    return {
        "case_id": case["case_id"],
        "is_fixed_sample": case["is_fixed_sample"],
        "success_reachable": len(success_states) > 0,
        "success_state_count": len(success_states),
        "deadlock_reachable": len(deadlock_states) > 0,
        "deadlock_state_count": len(deadlock_states),
        "state_count": len(visited),
        "has_at_least_one_dead_state": len(deadlock_states) > 0,
        "success_witness": success_witness,
        "deadlock_witness": deadlock_witness,
    }


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Four-DMA boundary abstract BFS model (not an RTL proof)"
    )
    parser.add_argument(
        "--output",
        default="out/four-dma-boundary-20261008",
        help="output directory (default: out/four-dma-boundary-20261008)",
    )
    args = parser.parse_args()

    outdir = args.output
    os.makedirs(outdir, exist_ok=True)

    cases = generate_cases()
    fixed_id = next(c["case_id"] for c in cases if c["is_fixed_sample"])
    policies = policy_list()

    results = {
        "meta": {
            "model": "four-DMA boundary abstract BFS model",
            "not_rtl_proof": True,
            "not_rtl_proof_note": (
                "This is an abstract progress model over translated AR-header "
                "parking/dispatch. It does not model TLB, PTW cycles, FAST, "
                "AXI VALID cycle stability, full iDMA, or production "
                "reachability. It provides no RTL occupancy or RTL "
                "reachability proof."
            ),
            "header_slot_counts_exclude": [
                "existing original AR FIFO",
                "source-held descriptor",
                "payload",
                "control",
            ],
            "write_order": [tag_to_list(t) for t in WRITE_ORDER],
            "fixed_sample_case_id": fixed_id,
            "fixed_sample_bank_orders": [
                [tag_to_list(t) for t in FIXED_BANK0],
                [tag_to_list(t) for t in FIXED_BANK1],
            ],
            "cases": [
                {
                    "case_id": c["case_id"],
                    "bank_orders": [
                        [tag_to_list(t) for t in c["bank_orders"][0]],
                        [tag_to_list(t) for t in c["bank_orders"][1]],
                    ],
                    "is_fixed_sample": c["is_fixed_sample"],
                }
                for c in cases
            ],
            "policies": [
                {"policy": policy_name, "params": params}
                for policy_name, params in policies
            ],
        },
        "policies": [],
    }

    traces = {
        "meta": {
            "note": (
                "Only one success witness and one deadlock witness per "
                "case/policy are emitted. Full visited state sets are not "
                "dumped."
            ),
            "not_rtl_proof": True,
            "not_rtl_proof_note": "Abstract progress model only; not an RTL proof.",
            "fixed_sample_case_id": fixed_id,
        },
        "traces": [],
    }

    for policy_name, params in policies:
        policy_result = {
            "policy": policy_name,
            "params": params,
            "case_count": len(cases),
            "cases": [],
            "aggregate": {},
        }

        for case in cases:
            res = analyze_case(case, policy_name, params)

            policy_result["cases"].append({
                "case_id": res["case_id"],
                "is_fixed_sample": res["is_fixed_sample"],
                "success_reachable": res["success_reachable"],
                "success_state_count": res["success_state_count"],
                "deadlock_reachable": res["deadlock_reachable"],
                "deadlock_state_count": res["deadlock_state_count"],
                "state_count": res["state_count"],
                "has_at_least_one_dead_state": res["has_at_least_one_dead_state"],
            })

            traces["traces"].append({
                "policy": policy_name,
                "params": params,
                "case_id": res["case_id"],
                "is_fixed_sample": res["is_fixed_sample"],
                "success_witness": res["success_witness"],
                "deadlock_witness": res["deadlock_witness"],
            })

        cases_with_success = sum(
            1 for c in policy_result["cases"] if c["success_reachable"]
        )
        cases_with_deadlock = sum(
            1 for c in policy_result["cases"] if c["deadlock_reachable"]
        )
        total_success_states = sum(
            c["success_state_count"] for c in policy_result["cases"]
        )
        total_deadlock_states = sum(
            c["deadlock_state_count"] for c in policy_result["cases"]
        )
        total_states = sum(c["state_count"] for c in policy_result["cases"])

        policy_result["aggregate"] = {
            "case_count": len(cases),
            "cases_with_success": cases_with_success,
            "cases_with_deadlock": cases_with_deadlock,
            "cases_with_at_least_one_dead_state": cases_with_deadlock,
            "all36_no_deadlock": cases_with_deadlock == 0,
            "total_success_states": total_success_states,
            "total_deadlock_states": total_deadlock_states,
            "total_states": total_states,
        }

        fs = next(
            c for c in policy_result["cases"] if c["case_id"] == fixed_id
        )
        policy_result["fixed_sample"] = dict(fs)
        results["policies"].append(policy_result)

    results_path = os.path.join(outdir, "results.json")
    traces_path = os.path.join(outdir, "traces.json")

    with open(results_path, "w", encoding="utf-8") as f:
        json.dump(results, f, indent=2, ensure_ascii=False)

    with open(traces_path, "w", encoding="utf-8") as f:
        json.dump(traces, f, indent=2, ensure_ascii=False)

    print(f"Wrote {results_path}")
    print(f"Wrote {traces_path}")
    print("NOTE: abstract progress model only; not an RTL proof.")


if __name__ == "__main__":
    main()
