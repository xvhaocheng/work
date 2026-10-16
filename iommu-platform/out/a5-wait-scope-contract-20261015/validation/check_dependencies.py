#!/usr/bin/env python3
"""Check two frozen source dependency cuts; no cycles, bus simulation or payload execution.

The graph is a manual source/API abstraction, not extracted RTL or an AXI
reachability proof. R_all/W_all/B denote aggregate job facts, not one burst.
"""
import hashlib
import json
from collections import defaultdict, deque
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def jobs_for(tiles):
    jobs = [("RX", 0), ("RY", 0)]
    for t in range(tiles):
        if t + 1 < tiles:
            jobs += [("RX", t + 1), ("RY", t + 1)]
        jobs.append(("WY", t))
    return jobs


def graph(tiles, episodes, selective):
    pred = defaultdict(set)

    def edge(a, b):
        pred[a]
        pred[b].add(a)

    edge("prepare", "map")
    jobs = jobs_for(tiles)
    index = {job: i for i, job in enumerate(jobs)}
    phases = ("call", "FE_offer", "accept", "return_ID", "R_all", "W_all", "B_retired")
    previous = "map"
    for ep in range(episodes):
        base = f"e{ep}"
        edge(previous, f"{base}:restore")
        edge(f"{base}:restore", f"{base}:cache_device")
        edge(f"{base}:cache_device", f"{base}:target")
        for rank, job in enumerate(jobs):
            p = f"{base}:j{rank}"
            for a, b in zip(phases, phases[1:]):
                # return_ID and payload may progress independently after accept.
                if a == "return_ID":
                    edge(f"{p}:accept", f"{p}:{b}")
                else:
                    edge(f"{p}:{a}", f"{p}:{b}")
            if rank:
                edge(f"{base}:j{rank-1}:return_ID", f"{p}:call")
                edge(f"{base}:j{rank-1}:B_retired", f"{p}:B_retired")
            else:
                edge(f"{base}:target", f"{p}:call")
        for t in range(tiles):
            k, end = f"{base}:K{t}:start", f"{base}:K{t}:end"
            edge(k, end)
            for kind in ("RX", "RY"):
                edge(f"{base}:j{index[kind,t]}:B_retired", k)
            if t:
                edge(f"{base}:j{index['WY',t-1]}:return_ID", k)
                if not (selective and t == tiles - 1):
                    edge(f"{base}:j{index['WY',t-1]}:B_retired", k)
            if t + 1 < tiles:
                edge(k, f"{base}:j{index['RX',t+1]}:call")
                edge(f"{base}:j{index['RY',t+1]}:return_ID", end)
            edge(end, f"{base}:j{index['WY',t]}:call")
        edge(f"{base}:j{len(jobs)-1}:return_ID", f"{base}:tail_wait_all")
        edge(f"{base}:j{len(jobs)-1}:B_retired", f"{base}:tail_wait_all")
        edge(f"{base}:tail_wait_all", f"{base}:fence_barrier_DONE")
        edge(f"{base}:fence_barrier_DONE", f"{base}:cache_cpu")
        edge(f"{base}:cache_cpu", f"{base}:consume")
        previous = f"{base}:consume"
    edge(previous, "unmap_invalidate_complete")
    edge("unmap_invalidate_complete", "unpin")
    edge("unpin", "free")
    return pred, jobs, index


def topological_count(pred):
    succ = defaultdict(set)
    deg = {n: len(ps) for n, ps in pred.items()}
    for n, ps in pred.items():
        for p in ps:
            succ[p].add(n)
    q = deque(n for n, d in deg.items() if d == 0)
    count = 0
    while q:
        n = q.popleft()
        count += 1
        for s in succ[n]:
            deg[s] -= 1
            if not deg[s]:
                q.append(s)
    return count


def prefix(case, tiles):
    ep = case["episode"]
    b = f"e{ep}"
    done = {"prepare", "map", f"{b}:restore", f"{b}:cache_device", f"{b}:target"}
    for r in range(case["accepted_prefix_exclusive"]):
        done.update(f"{b}:j{r}:{s}" for s in ("call", "FE_offer", "accept", "return_ID"))
    for r in range(case["completed_prefix_exclusive"]):
        done.update(f"{b}:j{r}:{s}" for s in ("R_all", "W_all", "B_retired"))
    for r in case["extra_payload_read_done_ranks"]:
        done.add(f"{b}:j{r}:R_all")
    for t in range(tiles - 1):
        done.update((f"{b}:K{t}:start", f"{b}:K{t}:end"))
    return done


def accesses(kind, tile, size):
    x = ("L1_X", tile % 2 * size, size)
    y = ("L1_Y", tile % 2 * size, size)
    hx, hy = ("HOST_X", tile * size, size), ("HOST_Y", tile * size, size)
    return {"RX": [(hx, "R"), (x, "W")], "RY": [(hy, "R"), (y, "W")],
            "WY": [(y, "R"), (hy, "W")], "K": [(x, "R"), (y, "R"), (y, "W")]}[kind]


def conflicts(a, b):
    return any(ra[0] == rb[0] and max(ra[1], rb[1]) < min(ra[1]+ra[2], rb[1]+rb[2])
               and (ma == "W" or mb == "W") for ra, ma in a for rb, mb in b)


def run():
    data = (ROOT / "case-contract.json").read_bytes()
    freeze = json.loads((ROOT / "case-freeze.json").read_text())
    assert hashlib.sha256(data).hexdigest() == freeze["contract_sha256"]
    c = json.loads(data)
    cfg = c["input"]["fixed_config"]
    tiles, episodes = cfg["tiles_per_episode"], cfg["episodes"]
    pg0, jobs, idx = graph(tiles, episodes, False)
    pg1, _, _ = graph(tiles, episodes, True)
    assert len(jobs) == cfg["jobs_per_episode"] == 192
    assert topological_count(pg0) == len(pg0)
    assert topological_count(pg1) == len(pg1)
    rows = []
    for case in c["cases"]:
        done = prefix(case, tiles)
        gate = f"e{case['episode']}:K{tiles-1}:start"
        missing = {n: sorted(pg0[n] - done) for n in done if pg0[n] - done}
        assert not missing, missing  # Prefix is an order ideal, not a native trace.
        needs = [idx[k, tiles-1] for k in ("RX", "RY")]
        rank = max(needs)
        outstanding = [j for r, j in enumerate(jobs[:case["accepted_prefix_exclusive"]])
                       if r >= case["completed_prefix_exclusive"] and r not in needs]
        safe_omissions = all(not conflicts(accesses("K", tiles-1, cfg["tile_bytes"]),
                                          accesses(k, t, cfg["tile_bytes"])) for k, t in outstanding)
        rule_gate = case["completed_prefix_exclusive"] > rank and safe_omissions
        standard0, standard1 = pg0[gate] <= done, pg1[gate] <= done
        assert rule_gate == standard1 == case["prediction"]["public_input_gate"]
        assert standard0 == case["prediction"]["original_wait_all_gate"]
        assert "free" not in done and "e1:consume" not in done
        # Final unpin/free remains dominated by both episodes' consumers and DMA tails.
        assert pg0["free"] == pg1["free"] == {"unpin"}
        rows.append({"case":case["id"],"source_prefix_is_order_ideal":True,
                     "minimal_required_relative_rank":rank,"safe_pending_omissions":safe_omissions,
                     "rule_gate":rule_gate,"same_information_DAG_ID_gate":standard1,
                     "original_wait_all_gate":standard0,"actual_cycle_reachability":None,
                     "full_consumer_time":None})
    result = {"kind":"two frozen source dependency cuts only","contract_sha256":freeze["contract_sha256"],
              "script_sha256":hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
              "graph_nodes_per_program":len(pg0),"episodes_in_graph":episodes,"jobs_in_graph":episodes*len(jobs),
              "cases":rows,"all_source_predictions_match":True,
              "not_proved":["RTL/buffer-occupancy/protocol reachability","native data/error/cache correctness",
                            "simultaneous IOMMU requests or contention","any cycle, service or complete time gain",
                            "new solver or exclusive method value"],
              "previous_A1_A3_experiments_replayed":False}
    (ROOT / "validation/dependency-results.json").write_text(json.dumps(result,ensure_ascii=False,indent=2)+"\n")
    print(json.dumps(result,ensure_ascii=False,indent=2))


if __name__ == "__main__":
    run()
