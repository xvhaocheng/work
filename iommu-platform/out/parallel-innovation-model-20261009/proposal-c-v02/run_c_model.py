"""Run all six fixtures x two profiles x six policies."""
from __future__ import annotations

import json
from typing import Dict, List, Any

from c_event_model import build_case, simulate
from c_policies import (
    policy_group_first,
    policy_hot_page,
    policy_closure_ack_density,
    policy_max_closure,
    policy_setup_density,
    policy_marginal_unlock_proxy,
)

FIXTURE_IDS = [
    "four_L10",
    "eight_L10",
    "four_L1",
    "eight_cap2",
    "eight_slow_consumer",
    "eight_small_pool",
]

PROFILES = [
    ("serial", False),
    ("overlap", True),
]

POLICIES = [
    ("group_first", policy_group_first),
    ("hot_page", policy_hot_page),
    ("closure_ack_density", policy_closure_ack_density),
    ("max_closure", policy_max_closure),
    ("setup_density", policy_setup_density),
    ("marginal_unlock_proxy", policy_marginal_unlock_proxy),
]


def run_all() -> Dict[str, Any]:
    out: Dict[str, Any] = {"runs": [], "hand": {}}

    for fid in FIXTURE_IDS:
        case = build_case(fid)
        for pname, pfunc in POLICIES:
            for profile, overlap in PROFILES:
                order = pfunc(case, overlap=overlap)
                res = simulate(case, order, overlap=overlap, drain=True)
                out["runs"].append({
                    "fixture": fid,
                    "policy": pname,
                    "profile": profile,
                    "status": res["status"],
                    "order": order,
                    "ready": res["ready"],
                    "ack": res["ack"],
                    "metrics": res["metrics"],
                })

    case4 = build_case("four_L10")
    hand_orders = {
        "x_first": [0, 1, 3, 5, 2, 4, 6],
        "yz_first": [1, 3, 5, 2, 4, 6, 0],
    }
    for label, order in hand_orders.items():
        res = simulate(case4, order, overlap=False, drain=True)
        out["hand"][label] = {
            "order": order,
            "status": res["status"],
            "ready": res["ready"],
            "ack": res["ack"],
            "metrics": res["metrics"],
        }

    return out


def main() -> None:
    print(json.dumps(run_all(), indent=2, sort_keys=True))


if __name__ == "__main__":
    main()
