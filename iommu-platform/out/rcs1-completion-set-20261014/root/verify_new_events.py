#!/usr/bin/env python3
"""Read new RCS1 event records; no engine import, simulation or old replay."""
import collections
import hashlib
import json
import pathlib

O = pathlib.Path(__file__).resolve().parents[1]
M = O / "model"
checks = []

def read(p):
    return json.loads(p.read_text())

def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()

def ck(label, value):
    checks.append({"check": label, "pass": bool(value)})
    if not value:
        raise AssertionError(label)

def address(keys, key, block, beat=0):
    return (0x20000 + keys.index(key)) * 4096 + block * 64 + beat * 8

inp = read(M / "frozen-input.json")
ledger = read(M / "execution-ledger.json")
gate = read(O / "root/entry-gate.json")
ck("exact four fixed conditions and eight fixed programs", inp["conditions"] == ["C1","C2","C3","C4"] and len(inp["policies"]) == 8)
ck("actual input unchanged", sha(M / "frozen-input.json") == ledger["input_sha256"] == "5945288d97bb3fd61b72db67d6cc4feb702b40e656c2190db1d499793b650a2e")
for item in gate["files"]:
    ck("pre-run freeze " + item["path"], sha(O / item["path"]) == item["sha256"])
passes = [r for r in ledger["runs"] if r["status"] == "passed"]
failures = [r for r in ledger["runs"] if r["status"] == "failed"]
ck("32 unique completed episodes", len(passes) == 32 and len({(r["condition"],r["policy"]) for r in passes}) == 32)
ck("one specific startup failure preserved", len(failures) == 1 and failures[0]["condition"] == "C1" and failures[0]["policy"] == "PLRU" and pathlib.Path(failures[0]["failure_path"]).exists())
failed_events = [json.loads(l) for l in pathlib.Path(failures[0]["event_path"]).open()]
ck("startup failure before any PTE/data service", not any(e["event"] == "service_beat" for e in failed_events) and all(e["tick"] == 0 for e in failed_events))
ck("all successful runs use the final reviewed code", all(r["engine_sha256"] == sha(M / "conditional_engine.py") for r in passes))
results = {}
totals = collections.Counter()
for run in passes:
    result = read(pathlib.Path(run["result_path"]))
    tag = result["condition"] + "/" + result["policy"]
    evpath = pathlib.Path(result["events"]["path"])
    ck(tag + " raw result/event hashes", sha(pathlib.Path(run["result_path"])) == run["result_sha256"] and sha(evpath) == result["events"]["sha256"])
    events = [json.loads(l) for l in evpath.open()]
    ck(tag + " event sequence", len(events) == result["events"]["count"] and [e["seq"] for e in events] == list(range(len(events))) and [e["tick"] for e in events] == sorted(e["tick"] for e in events))
    count = collections.Counter(e["event"] for e in events)
    expected = {"copy_start":51,"data_AR_accepted":51,"data_AW_accepted":51,
                "R_OKAY_owner_receipt":408,"W_OKAY":408,"B_OKAY_copy_complete":51,
                "private_permission_handoff":102,"last_use_feedback":102,
                "consumer_receipt_owner_release":4,"producer_stage_receipt":1,
                "native_WLAST_input_owner_release":51,"native_read_SELECT_admission":51,"episode_quiet":1}
    ck(tag + " raw payload/request/retirement conservation", all(count[k] == v for k,v in expected.items()))
    copies = inp["copies_by_condition"][result["condition"]]
    keys = inp["keys"]
    remaining = {r:collections.Counter(k for c in cs for k in [c["src_key"],c["dst_key"]]) for r,cs in copies.items()}
    memory = {}
    for cs in copies.values():
        for c in cs:
            for beat in range(8):
                memory[address(keys,c["src_key"],c["src_block"],beat)] = (keys.index(c["src_key"])+1)*1000000 + c["src_block"]*1000 + beat
    queue = collections.deque()
    aw_order = collections.deque()
    accepted, permissions, requests = set(), {}, {}
    active = {}
    reads, writes, Bs = {}, {}, {}
    pte_level = {}
    pte_previous = {}
    pte_count = 0
    port_ticks = collections.Counter()
    response_ticks = collections.Counter()
    source_next = {0:[],1:[]}
    publish = None
    receipt = None
    endpoints = {}
    for e in events:
        kind = e["event"]
        if kind == "copy_start":
            src, role, ci = e["source"],e["role"],e["ci"]
            assert src not in active
            assert role == "P" or receipt is not None
            active[src] = (role,ci)
            source_next[src].append((role,ci))
        elif kind == "translation_request_held":
            assert e["id"] not in requests
            assert active[e["source"]] == (e["role"],e["ci"])
            requests[e["id"]] = e
        elif kind == "private_permission_handoff":
            r = requests[e["req_id"]]
            assert (r["role"],r["source"],r["access"],r["owner"]) == (e["role"],e["source"],e["access"],e["result_owner"])
            assert e["response"] == "OKAY"
            assert e["physical_address"] == address(keys,r["key"],r["block"])
            permissions[e["req_id"]] = e
            response_ticks[e["tick"]] += 1
        elif kind == "fullwalk_start":
            assert e["req_id"] not in pte_level
            pte_level[e["req_id"]] = 2
            port_ticks[e["tick"]] += 1
        elif kind in ("TL_hit_permission_pending","native_post_fill_hit_touch"):
            port_ticks[e["tick"]] += 1
        elif kind == "PTE_AR_accepted":
            rid = e["req_id"]
            assert e["level"] == pte_level[rid]
            if e["level"] < 2:
                assert rid in pte_previous and e["tick"] > pte_previous[rid]["tick"]
                req = requests[rid]
                index = (req["va"] >> (12+9*e["level"])) & 511
                assert e["address"] == (pte_previous[rid]["returned_content"] >> 10)*4096 + 8*index
        elif kind == "PTE_R_OKAY":
            assert e["level"] == pte_level[e["req_id"]]
            pte_level[e["req_id"]] -= 1
            pte_previous[e["req_id"]] = e
            pte_count += 1
        elif kind == "service_enqueue":
            queue.append(e)
        elif kind == "service_beat":
            op = queue.popleft()
            assert e["serial"] == op["serial"] and e["kind"] == op["kind"] and e["address"] == op["address"]
            assert e["bytes"] == 8 and e["response"] == "OKAY"
            if e["kind"] == "data_R":
                cp = copies[e["role"]][e["ci"]]
                assert e["address"] == address(keys,cp["src_key"],cp["src_block"],e["beat"])
                reads[(e["role"],e["ci"],e["beat"])] = memory[e["address"]]
            elif e["kind"] == "data_W":
                cp = copies[e["role"]][e["ci"]]
                assert e["address"] == address(keys,cp["dst_key"],cp["dst_block"],e["beat"])
                assert e["value"] == reads[(e["role"],e["ci"],e["beat"])]
                memory[e["address"]] = e["value"]
                writes[(e["role"],e["ci"],e["beat"])] = e["tick"]
        elif kind == "native_input_AW_order_accept":
            rid = e["req_id"]
            r = requests.get(rid)
            # Native-input header event precedes the held-request trace on this edge.
            role,ci,_ = rid.split(":")
            aw_order.append((role,int(ci)))
        elif kind == "native_WLAST_input_owner_release":
            assert aw_order.popleft() == (e["role"],e["ci"])
        elif kind == "last_use_feedback":
            rid = e["req_id"]
            assert rid in permissions and rid not in accepted
            accepted.add(rid)
            remaining[e["role"]][e["key"]] -= 1
            assert remaining[e["role"]][e["key"]] == e["remaining_same_key_uses"] >= 0
        elif kind in ("data_AR_accepted","data_AW_accepted"):
            access = "R" if kind == "data_AR_accepted" else "W"
            rid = f'{e["role"]}:{e["ci"]}:{access}'
            assert rid in accepted and e["address"] == permissions[rid]["physical_address"]
        elif kind == "R_OKAY_owner_receipt":
            assert e["value"] == reads[(e["role"],e["ci"],e["beat"])]
        elif kind == "B_OKAY_copy_complete":
            role,ci,src = e["role"],e["ci"],e["source"]
            assert active.pop(src) == (role,ci)
            assert all((role,ci,b) in writes for b in range(8))
            assert e["tick"] == writes[(role,ci,7)] + 1
            Bs[(role,ci)] = e["tick"]
        elif kind == "producer_publication_B":
            publish = e["tick"]
            assert publish == Bs[("P",16)]
        elif kind == "producer_stage_receipt":
            receipt = e["tick"]
            assert receipt == publish + 1
        elif kind == "consumer_receipt_owner_release":
            assert e["tick"] == Bs[(e["role"],len(copies[e["role"]])-1)] + 1
            endpoints[e["role"]] = e["tick"]
        elif kind == "policy_choice_input":
            s = e["snapshot"]
            req = requests[s["req_id"]]
            expected_u = {}
            for role, counter in remaining.items():
                cp = counter.copy()
                if role == req["role"]:
                    cp[req["key"]] -= 1
                u = sorted(k for k,n in cp.items() if n > 0)
                if u:
                    expected_u[role] = u
            assert s["U"] == expected_u
            if result["policy"] == "RCS1" and result["condition"] != "C2":
                u = [set(v) for v in s["U"].values()]
                R = set(s["resident"])
                scores = []
                for w in range(16):
                    post = (R-{s["ways"][w]})|{s["key"]}
                    scores.append((sum(t <= post for t in u), -len(set().union(*(t-post for t in u)) if u else set())))
                bypass = (sum(t <= R for t in u), -len(set().union(*(t-R for t in u)) if u else set()))
                a = e["planned_action"]
                assert (bypass if a["action"] == "bypass" else scores[a["way"]]) == max(scores+[bypass])
    ck(tag + " independent FIFO/real values/permissions/use retirement/ownership/dependent PTE checks", not queue and not aw_order and not active and len(reads)==len(writes)==408 and len(Bs)==51 and len(accepted)==102 and all(n == -1 for n in pte_level.values()))
    ck(tag + " original single lookup/response budgets", max(port_ticks.values()) <= 1 and max(response_ticks.values()) <= 1)
    ck(tag + " full completion recounted", endpoints == {r:t["completion"] for r,t in result["tasks"].items()} and max(endpoints.values()) == result["model_ticks"]["completion_max"] and sum(endpoints.values()) == result["model_ticks"]["completion_sum"] and events[-1]["event"] == "episode_quiet" and events[-1]["tick"] == result["model_ticks"]["quiet"])
    ck(tag + " actual bytes and PTE counts", pte_count == result["actual_counts"]["PTE_R"] == 3*count["fullwalk_start"] and result["actual_counts"]["data_read_bytes"] == result["actual_counts"]["data_write_bytes"] == 408*8 and result["actual_counts"]["PTE_bytes"] == pte_count*8)
    ck(tag + " complete fixed same-source work", source_next[0] == [("P",i) for i in range(32)] and source_next[1] == [(r,i) for r in ["B","S2","S3"] for i in range(len(copies[r]))])
    ck(tag + " unknown physical costs and untested errors preserved", result["physical_area"] is None and result["Fmax"] is None and result["physical_CPU_cycles"] is None and result["application_gain"] is None and result["checks"]["error_count"] == 0 and not result["checks"]["injected_error_termination_validated"])
    totals["events"] += len(events)
    totals["successful_episodes"] += 1
    totals["PTE_beats"] += pte_count
    results[(result["condition"],result["policy"])] = result

for c in inp["conditions"]:
    r = results[(c,"RCS1")]
    if c != "C2":
        ck(c + " actual q witness reached without preload", r["q_choice"]["snapshot"]["tree"] == [0]*15 and len(r["q_choice"]["snapshot"]["resident"])==16 and r["q_choice"]["victim_key"] == "p3")
    s = results[(c,"SJF")]
    ck(c + " RCS1 complete time does not beat frozen SJF", r["model_ticks"]["completion_max"] >= s["model_ticks"]["completion_max"])
ck("C2 all fixed programs exactly agree", len({json.dumps(results[("C2",p)]["model_ticks"],sort_keys=True) for p in inp["policies"]})==1)
out = {"status":"passed","kind":"new raw-event delivery validation; no engine execution or old trace/validator replay","checks":checks,"passed":len(checks),"failed":0,"totals":dict(totals)}
(O / "root/event-verification.json").write_text(json.dumps(out,indent=2)+"\n")
print(json.dumps({k:v for k,v in out.items() if k != "checks"}))
