```python
#!/usr/bin/env python3
"""
frontier_model.py

Finite normal 4K translation frontier model for the §45 contract.
This is a mathematical finite-graph model only.  It is not a DUT,
not a memory-cycle model, and not a whole-architecture minimal-state
result.  It must not be used to establish a global lower bound.
"""

import argparse
import heapq
import json
import os
import sys

DATA_BITS = [0, 44, 44, 44, 30, 74, 74, 74, 30, 74, 74, 74, 33, 77, 77, 0]
FAMILIES = ["whole_restart", "full_current", "semantic_restart", "mixed_indexed", "semantic_chunks"]
Q_VALUES = list(range(1, 16))
B_VALUES = list(range(0, 86))
TOTAL_NODES = 16 * 16 * 2  # 512

# ---------------------------------------------------------------------------
# family helpers
# ---------------------------------------------------------------------------

def b_semantic(p):
    """Semantic restart b(p) from contract line 21."""
    if p < 12:
        return 4 * (p // 4)
    if p <= 14:
        return 12
    return 15  # terminal, not used as a saved value

def semantic_token_bits(b):
    """Token bits for semantic_restart saved value b."""
    if b == 0:
        return 0
    if b in (4, 8):
        return DATA_BITS[b] + 3  # 30 + 3 = 33
    if b == 12:
        return DATA_BITS[b] + 3  # 33 + 3 = 36
    raise ValueError(f"invalid semantic b {b}")

def direct_save_set(family, B):
    """Positions 1..14 that can be directly saved.  0 and 15 are always present."""
    if family == "whole_restart":
        return set()
    if family == "full_current":
        return {p for p in range(1, 15) if DATA_BITS[p] + 5 <= B}
    if family == "semantic_restart":
        return {b for b in (4, 8, 12) if semantic_token_bits(b) <= B}
    if family == "mixed_indexed":
        return {p for p in range(1, 15) if DATA_BITS[p] + 4 <= B}
    if family == "semantic_chunks":
        return {p for p in (3, 4, 7, 8, 11, 12) if DATA_BITS[p] + 3 <= B}
    raise ValueError(family)

_save_choices_cache = {}
def save_choices(family, p_after, B):
    """Legal saved positions for a non-terminal execution to p_after."""
    key = (family, p_after, B)
    if key in _save_choices_cache:
        return _save_choices_cache[key]
    choices = {0}  # 0 is always a legal restart/fallback
    if family == "whole_restart":
        pass
    elif family == "full_current":
        if DATA_BITS[p_after] + 5 <= B:
            choices.add(p_after)
    elif family == "semantic_restart":
        b = b_semantic(p_after)
        if semantic_token_bits(b) <= B:
            choices.add(b)
    elif family == "mixed_indexed":
        if DATA_BITS[p_after] + 4 <= B:
            choices.add(p_after)
        b = b_semantic(p_after)
        if b > 0 and DATA_BITS[b] + 4 <= B:
            choices.add(b)
    elif family == "semantic_chunks":
        if p_after in (3, 4, 7, 8, 11, 12) and DATA_BITS[p_after] + 3 <= B:
            choices.add(p_after)
    else:
        raise ValueError(family)
    _save_choices_cache[key] = choices
    return choices

_token_bits_cache = {}
def token_bits_for_saved(family, s):
    """Bits in the saved token for position s (0..14)."""
    key = (family, s)
    if key in _token_bits_cache:
        return _token_bits_cache[key]
    if s == 0:
        tb = 0
    elif family == "whole_restart":
        tb = 0  # only 0 can be saved
    elif family == "full_current":
        tb = DATA_BITS[s] + 5
    elif family == "semantic_restart":
        tb = semantic_token_bits(s)
    elif family == "mixed_indexed":
        tb = DATA_BITS[s] + 4
    elif family == "semantic_chunks":
        tb = DATA_BITS[s] + 3
    else:
        raise ValueError(family)
    _token_bits_cache[key] = tb
    return tb

# ---------------------------------------------------------------------------
# graph neighbours (tuple state, with info, for path reconstruction)
# ---------------------------------------------------------------------------

def neighbors(state, family, q, B):
    pA, pV, turn = state
    if pA == 15 and pV == 15:
        return []
    if pA == 15 and pV < 15:
        return [((15, 15, 0), 15 - pV, {
            "turn": "V", "from": pV, "x": 15 - pV, "to": 15,
            "saved_to": 15, "lost": 0, "token_bits": 0,
            "continuous_finish": True
        })]
    if pV == 15 and pA < 15:
        return [((15, 15, 0), 15 - pA, {
            "turn": "A", "from": pA, "x": 15 - pA, "to": 15,
            "saved_to": 15, "lost": 0, "token_bits": 0,
            "continuous_finish": True
        })]
    if turn == 0:
        pT, pO = pA, pV
        T_name, O_idx = "A", 1
        T_idx = 0
    else:
        pT, pO = pV, pA
        T_name, O_idx = "V", 0
        T_idx = 1
    max_x = min(q, 15 - pT)
    res = []
    for x in range(1, max_x + 1):
        p_after = pT + x
        if p_after == 15:
            new_pA = 15 if T_idx == 0 else pA
            new_pV = 15 if T_idx == 1 else pV
            new_state = (new_pA, new_pV, O_idx)
            info = {
                "turn": T_name, "from": pT, "x": x, "to": 15,
                "saved_to": 15, "lost": 0, "token_bits": 0
            }
            res.append((new_state, x, info))
        else:
            for s in save_choices(family, p_after, B):
                if T_idx == 0:
                    new_state = (s, pO, O_idx)
                else:
                    new_state = (pO, s, O_idx)
                lost = p_after - s
                tb = token_bits_for_saved(family, s)
                info = {
                    "turn": T_name, "from": pT, "x": x, "to": p_after,
                    "saved_to": s, "lost": lost, "token_bits": tb
                }
                res.append((new_state, x, info))
    return res

# ---------------------------------------------------------------------------
# fast integer-index neighbours and Dijkstra for bulk enumeration
# ---------------------------------------------------------------------------

def state_to_idx(state):
    pA, pV, turn = state
    return (pA * 16 + pV) * 2 + turn

def idx_to_state(idx):
    turn = idx & 1
    pV = (idx >> 1) % 16
    pA = (idx >> 1) // 16
    return (pA, pV, turn)

def neighbors_idx(idx, family, q, B):
    pA, pV, turn = idx_to_state(idx)
    if pA == 15 and pV == 15:
        return []
    if pA == 15 and pV < 15:
        return [(state_to_idx((15, 15, 0)), 15 - pV)]
    if pV == 15 and pA < 15:
        return [(state_to_idx((15, 15, 0)), 15 - pA)]
    if turn == 0:
        pT, pO = pA, pV
        T_idx = 0
        O_idx = 1
    else:
        pT, pO = pV, pA
        T_idx = 1
        O_idx = 0
    max_x = min(q, 15 - pT)
    res = []
    for x in range(1, max_x + 1):
        p_after = pT + x
        if p_after == 15:
            new_pA = 15 if T_idx == 0 else pA
            new_pV = 15 if T_idx == 1 else pV
            res.append((state_to_idx((new_pA, new_pV, O_idx)), x))
        else:
            for s in save_choices(family, p_after, B):
                if T_idx == 0:
                    new_state = (s, pO, O_idx)
                else:
                    new_state = (pO, s, O_idx)
                res.append((state_to_idx(new_state), x))
    return res

def dijkstra_fast(family, q, B):
    start = state_to_idx((0, 0, 0))
    INF = 10**9
    dist = [INF] * TOTAL_NODES
    dist[start] = 0
    heap = [(0, start)]
    visited = [False] * TOTAL_NODES
    reachable_count = 0
    while heap:
        d, idx = heapq.heappop(heap)
        if visited[idx]:
            continue
        visited[idx] = True
        reachable_count += 1
        pA, pV, _ = idx_to_state(idx)
        if pA == 15 and pV == 15:
            return d, reachable_count
        for nxt, w in neighbors_idx(idx, family, q, B):
            nd = d + w
            if nd < dist[nxt]:
                dist[nxt] = nd
                heapq.heappush(heap, (nd, nxt))
    return None, reachable_count

# ---------------------------------------------------------------------------
# path/cycle helpers
# ---------------------------------------------------------------------------

def dijkstra_path(family, q, B):
    start = (0, 0, 0)
    dist = {start: 0}
    prev = {}
    heap = [(0, start)]
    visited = set()
    while heap:
        d, state = heapq.heappop(heap)
        if state in visited:
            continue
        visited.add(state)
        if state[0] == 15 and state[1] == 15:
            path = []
            cur = state
            while cur in prev:
                pstate, info = prev[cur]
                path.append((pstate, cur, info))
                cur = pstate
            path.reverse()
            return d, path, len(visited)
        for nxt, w, info in neighbors(state, family, q, B):
            nd = d + w
            if nxt not in dist or nd < dist[nxt]:
                dist[nxt] = nd
                prev[nxt] = (state, info)
                heapq.heappush(heap, (nd, nxt))
    return None, None, len(visited)

def find_cycle(family, q, B, start=(0, 0, 0)):
    color = {start: 1}
    parent = {}
    parent_info = {}
    dfs_stack = [(start, iter(neighbors(start, family, q, B)))]
    while dfs_stack:
        node, it = dfs_stack[-1]
        try:
            nxt, w, info = next(it)
        except StopIteration:
            color[node] = 2
            dfs_stack.pop()
            continue
        if nxt in color:
            if color[nxt] == 1:
                edges = []
                cur = node
                while cur != nxt:
                    p = parent[cur]
                    edges.append((p, cur, parent_info[cur]))
                    cur = p
                edges.reverse()
                edges.append((node, nxt, info))
                return edges
            continue
        color[nxt] = 1
        parent[nxt] = node
        parent_info[nxt] = info
        dfs_stack.append((nxt, iter(neighbors(nxt, family, q, B))))
    return None

def format_path(path):
    if path is None:
        return None
    return [
        {"from_state": list(pstate), "to_state": list(nstate), "info": info}
        for pstate, nstate, info in path
    ]

def format_cycle(cycle):
    if cycle is None:
        return None
    return [
        {"from_state": list(fstate), "to_state": list(tstate), "info": info}
        for fstate, tstate, info in cycle
    ]

def maxgap_positions(S):
    S = sorted(S)
    return max(S[i + 1] - S[i] for i in range(len(S) - 1))

# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="Finite normal 4K translation frontier model")
    parser.add_argument("--output", default="out/translation-progress-frontier-20261008/frontier_report.json",
                        help="Output JSON path")
    args = parser.parse_args()

    # Direct save sets / maxgap prediction per family and B
    direct_info = {}
    for fam in FAMILIES:
        direct_info[fam] = {}
        for B in B_VALUES:
            S = {0, 15} | direct_save_set(fam, B)
            G = maxgap_positions(S)
            direct_info[fam][B] = {
                "S": sorted(S),
                "maxgap": G,
                "predicted_min_q": G
            }

    # Enumerate all finite-graph configurations
    all_results = {fam: {} for fam in FAMILIES}
    mismatches = []
    for fam in FAMILIES:
        for B in B_VALUES:
            all_results[fam][B] = {}
            for q in Q_VALUES:
                min_work, reach_nodes = dijkstra_fast(fam, q, B)
                reachable = min_work is not None
                all_results[fam][B][q] = {
                    "q": q,
                    "B": B,
                    "reachable": reachable,
                    "min_work": min_work,
                    "reachable_nodes": reach_nodes,
                    "unreachable_nodes": TOTAL_NODES - reach_nodes
                }
                pred = q >= direct_info[fam][B]["maxgap"]
                if pred != reachable:
                    mismatches.append({
                        "family": fam, "q": q, "B": B,
                        "reason": "reachability mismatch",
                        "predicted_reachable": pred,
                        "actual_reachable": reachable,
                        "maxgap": direct_info[fam][B]["maxgap"],
                        "direct_S": direct_info[fam][B]["S"]
                    })
                elif reachable and min_work != 30:
                    mismatches.append({
                        "family": fam, "q": q, "B": B,
                        "reason": "min_work != 30",
                        "predicted_reachable": pred,
                        "actual_reachable": reachable,
                        "min_work": min_work,
                        "direct_S": direct_info[fam][B]["S"]
                    })

    # B bands per family
    bands = {}
    for fam in FAMILIES:
        fam_bands = []
        current = None
        for B in B_VALUES:
            direct_S = direct_info[fam][B]["S"]
            G = direct_info[fam][B]["maxgap"]
            pred_min_q = direct_info[fam][B]["predicted_min_q"]
            actual_min_q = None
            for q in Q_VALUES:
                if all_results[fam][B][q]["reachable"]:
                    actual_min_q = q
                    break
            key = (tuple(direct_S), G, pred_min_q, actual_min_q)
            if current is None or current["key"] != key:
                if current is not None:
                    fam_bands.append(current["band"])
                current = {
                    "key": key,
                    "band": {
                        "B_min": B,
                        "B_max": B,
                        "direct_S": list(direct_S),
                        "maxgap": G,
                        "predicted_min_q": pred_min_q,
                        "actual_min_q": actual_min_q
                    }
                }
            else:
                current["band"]["B_max"] = B
        if current is not None:
            fam_bands.append(current["band"])
        bands[fam] = fam_bands

    # Representative q/B matrix
    rep_B = [36, 77, 78, 79, 81, 82]
    rep_q = [1, 2, 3, 4]
    rep_paths = {}
    for fam in FAMILIES:
        for q in rep_q:
            for B in rep_B:
                key = f"{fam}_q{q}_B{B}"
                min_work, path, reach_nodes = dijkstra_path(fam, q, B)
                if min_work is not None:
                    rep_paths[key] = {
                        "family": fam, "q": q, "B": B,
                        "reachable": True,
                        "min_work": min_work,
                        "recomputed_work": min_work - 30,
                        "reachable_nodes": reach_nodes,
                        "path": format_path(path)
                    }
                else:
                    cycle = find_cycle(fam, q, B)
                    rep_paths[key] = {
                        "family": fam, "q": q, "B": B,
                        "reachable": False,
                        "min_work": None,
                        "recomputed_work": None,
                        "reachable_nodes": reach_nodes,
                        "unreachable_nodes": TOTAL_NODES - reach_nodes,
                        "cycle": format_cycle(cycle) if cycle else None
                    }

    # Specific required cases
    specific = {}
    def add_specific(name, fam, q, B):
        min_work, path, reach_nodes = dijkstra_path(fam, q, B)
        entry = {
            "family": fam, "q": q, "B": B,
            "reachable": min_work is not None,
            "min_work": min_work,
            "recomputed_work": (min_work - 30) if min_work is not None else None,
            "reachable_nodes": reach_nodes,
            "unreachable_nodes": TOTAL_NODES - reach_nodes
        }
        if min_work is not None:
            entry["path"] = format_path(path)
        else:
            cycle = find_cycle(fam, q, B)
            entry["cycle"] = format_cycle(cycle) if cycle else None
        specific[name] = entry

    add_specific("semantic_restart_q3_B36_failure_cycle", "semantic_restart", 3, 36)
    add_specific("semantic_restart_q4_B36_no_reread_path", "semantic_restart", 4, 36)
    add_specific("semantic_chunks_q3_B77_no_reread_path", "semantic_chunks", 3, 77)
    add_specific("whole_q14_B0_unreachable", "whole_restart", 14, 0)
    add_specific("whole_q15_B0_success", "whole_restart", 15, 0)

    # Compact all-config table
    all_configs = {
        "fields": ["q", "B", "reachable", "min_work", "reachable_nodes", "unreachable_nodes"]
    }
    for fam in FAMILIES:
        fam_list = []
        for B in B_VALUES:
            for q in Q_VALUES:
                r = all_results[fam][B][q]
                fam_list.append([
                    r["q"], r["B"], r["reachable"], r["min_work"],
                    r["reachable_nodes"], r["unreachable_nodes"]
                ])
        all_configs[fam] = fam_list

    report = {
        "metadata": {
            "model": "finite normal 4K translation frontier model",
            "contract": "§45 codex-model-contract.md",
            "families": FAMILIES,
            "q_values": Q_VALUES,
            "B_values": B_VALUES,
            "total_configurations": len(FAMILIES) * len(Q_VALUES) * len(B_VALUES),
            "total_graph_nodes": TOTAL_NODES,
            "data_bits": DATA_BITS,
            "chain": "G1,G2,G3,S1,G1,G2,G3,S2,G1,G2,G3,S3,G1,G2,G3",
            "all_configs_fields": all_configs["fields"],
            "notes": [
                "Finite mathematical model only; not DUT, not memory-cycle, not whole-architecture minimal state.",
                "0 and 15 need no extra frame; C/identity/active/terminal shared costs are excluded from B.",
                "No area output.",
                "Allows voluntary early yield and restart; does not force filling q.",
                "PWC/denser tags/cache optimizations are not used here to establish any global lower bound.",
                "Unreachability is determined by full finite-graph Dijkstra; a reported cycle is only a non-progress trace, not a proof that all strategies fail.",
                "min_work is null when the finite graph cannot reach both-done state.",
                "recomputed_work = min_work - 30; 30 is the total unique PTE work for two requests (15+15)."
            ]
        },
        "bands": bands,
        "representative_paths": rep_paths,
        "specific_cases": specific,
        "mismatches": mismatches,
        "exit_code": 1 if mismatches else 0,
        "all_configs": all_configs
    }

    out_dir = os.path.dirname(os.path.abspath(args.output))
    if out_dir:
        os.makedirs(out_dir, exist_ok=True)
    with open(args.output, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=2, sort_keys=False)

    print(f"wrote {args.output}")
    if mismatches:
        print(f"WARNING: {len(mismatches)} mismatches found", file=sys.stderr)
        sys.exit(1)
    else:
        print("all finite-graph checks matched direct maxgap prediction")
        sys.exit(0)

if __name__ == "__main__":
    main()
```

### Assumptions
- The finite model follows §45 exactly: two requests, chain of 15 PTEs, `data_bits` as given, and the five primitive families.
- `0` is always a legal fallback save (whole restart) when a family cannot save the current position within `B`.
- For `mixed_indexed`, saving semantic `b` uses the same 4-bit tag, so its token cost is `data_bits[b] + 4`.
- For `semantic_restart`, `b = 4*floor(p/4)` for `p < 12`, `b = 12` for `p in [12,14]`; token costs are `0` for `b=0`, `33` for `b=4/8`, `36` for `b=12`.
- Terminal `15` and initial `0` need no extra frame; C/identity/active/terminal shared costs are excluded from `B`.
- Unreachability is decided by complete finite-graph Dijkstra; a cycle is only a non-progress trace and not by itself a proof that all policies fail.
- The script uses only the Python standard library and is intended to be run once by root.

### Unresolved / Notes
- The model does **not** prove the general maxgap proposition; it only cross-checks the finite 6450 configurations.
- No area, PWC, denser tag, or cache-optimization comparison is included.
- If any mismatch is found, the script stores diagnostics in `mismatches`, sets `exit_code: 1`, and exits nonzero.
- The representative path matrix is 5 families × 4 q values × 6 B values = 120 entries. Adjust `rep_q` / `rep_B` if a different representative set is desired.
- The finite 4K model must not be used to assert a global lower bound after allowing legal PWC/denser tags/cache optimizations.

### Suggested acceptance checks
1. Run:
   ```bash
   python frontier_model.py --output out/translation-progress-frontier-20261008/frontier_report.json
   ```
2. Confirm JSON has `"exit_code": 0` and `"mismatches": []`.
3. Verify `specific_cases` contains:
   - `semantic_restart_q3_B36_failure_cycle` with a non-progress cycle,
   - `semantic_restart_q4_B36_no_reread_path` with `min_work == 30`,
   - `semantic_chunks_q3_B77_no_reread_path` with `min_work == 30`,
   - `whole_q14_B0_unreachable` with `reachable == false`,
   - `whole_q15_B0_success` with `min_work == 30`.
4. Check `bands` shows the expected min-q thresholds, e.g. `whole_restart` always 15; `full_current` bands around `B=35,38,79,82`; `mixed_indexed` around `B=34,37,78,81`; `semantic_restart` around `B=33,36`; `semantic_chunks` around `B=33,36,47,77`.
5. Check `all_configs` has 6450 entries per family and `min_work` is `null` for unreachable cases.
6. Ensure no `inf` values appear in the JSON; unreachable `min_work` must be `null`.
