#!/usr/bin/env python3
"""New RCS1 conditional FIFO-service engine; no production code imported.

Each execution is a finite cold episode, not a replay or parameter search.
See frozen-input.json and limitations.md for the conditional clock/service model.
"""
from __future__ import annotations

import argparse
import collections
import datetime
import functools
import hashlib
import json
import pathlib
import sys
import traceback

HERE = pathlib.Path(__file__).resolve().parent
POLICIES = ["PLRU", "pin", "oldest", "SJF", "fraction", "minrefs", "fractionInsert", "RCS1"]
KEYS = ["a", "b"] + [f"p{i}" for i in range(14)] + ["q", "r"]
KINDEX = {k: i for i, k in enumerate(KEYS)}
COPY_BYTES = 64
BEATS = 8
EPOCH = 1


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def jwrite(path, obj):
    path.write_text(json.dumps(obj, indent=2, sort_keys=True) + "\n")


def va(key, block):
    return 0x40000000 + 4096 * KINDEX[key] + 64 * block


def pa(key, block):
    return (0x20000 + KINDEX[key]) * 4096 + 64 * block


def payload_value(key, block, beat):
    return (KINDEX[key] + 1) * 1000000 + block * 1000 + beat


class PLRU:
    """Native shared binary-tree bits: hit points away; no invalid-first."""
    def __init__(self):
        self.bits = [0] * 15
        self.ways = [None] * 16
        self.contents = [None] * 16
        self.generation = 0

    def victim(self, mask):
        assert mask
        node, low, high = 0, 0, 16
        while high - low > 1:
            middle = (low + high) // 2
            left = any(w in mask for w in range(low, middle))
            right = any(w in mask for w in range(middle, high))
            choose_right = right and (not left or self.bits[node])
            if choose_right:
                node, low = 2 * node + 2, middle
            else:
                node, high = 2 * node + 1, middle
        return low

    def touch(self, way):
        node, low, high = 0, 0, 16
        while high - low > 1:
            middle = (low + high) // 2
            right = way >= middle
            self.bits[node] = int(not right)
            if right:
                node, low = 2 * node + 2, middle
            else:
                node, high = 2 * node + 1, middle
        self.generation += 1

    def resident(self):
        return set(k for k in self.ways if k is not None)

    def fill(self, way, key, content):
        assert key not in self.resident()
        self.ways[way] = key
        self.contents[way] = content.copy()
        self.generation += 1


def copies_for(condition):
    def cp(sk, sb, dk, db, publication=False):
        return dict(src_key=sk, src_block=sb, dst_key=dk, dst_block=db,
                    bytes=64, publication=publication)
    prefix = [cp("a", 0, "a", 1)]
    prefix += [cp(f"p{i}", 0, f"p{i}", 1) for i in range(7)]
    prefix += [cp("b", 0, "b", 1)]
    prefix += [cp(f"p{i}", 0, f"p{i}", 1) for i in range(7, 14)]
    prefix += [cp("p13", 1, "q", 1, publication=True)]
    tail = list(range(14)) if condition != "C4" else list(reversed(range(14)))
    producer = prefix + [cp(f"p{i}", 1, f"p{i}", 2) for i in tail]
    producer += [cp("r", 0, "r", 2)]
    big = [cp(f"p{i}", 1, f"p{i}", 3) for i in range(14)]
    big += [cp("r", 4, "r", 3)]
    return {"P": producer, "B": big,
            "S2": [cp("a", 1, "q", 4), cp("a", 1, "q", 5)],
            "S3": [cp("b", 1, "q", 6), cp("b", 1, "q", 7)]}


def make_input():
    obj = {
        "version": "RCS1-four-conditions-input-v1", "author": "Codex",
        "conditions": ["C1", "C2", "C3", "C4"], "policies": POLICIES,
        "keys": KEYS, "ways": 16, "walker_count": 1,
        "source_data_buffer_capacity_beats": {"0": 512, "1": 512},
        "native_active_copies_per_source": 1, "copy_bytes": COPY_BYTES,
        "shared_IOTLB_lookup_acceptances_per_tick": 1,
        "shared_TC_translation_response_deliveries_per_tick": 1,
        "lookup_selection": "two sources RR, advance only on legitimate accepted lookup; busy miss cannot consume walker/hit grant",
        "fast_hit_window": "other bank only while single PTW actively need_PTE/PTE_wait; no new probe during choice/result-held; one original owned fast result",
        "payload_read_bytes": 3264, "payload_write_bytes": 3264,
        "registration_common_ready_tick": 0,
        "registration_preparation_software_cycles": None,
        "service": {"type": "conditional_shared_FIFO", "beat_bytes": 8,
                    "beats_per_tick": 1, "pte_levels": 3,
                    "PTE_requests": "dependent on returned previous PTE contents",
                    "PTE_walk_fixed_latency": None,
                    "selection_pipeline_ticks_all_policies": 1,
                    "hit_permission_ticks": 1, "full_result_permission_ticks": 1,
                    "B_ordinary_response_ticks": 1,
                    "producer_stage_receipt_ticks_after_B": 1,
                    "final_consumer_owner_receipt_ticks_after_B": 1,
                    "same_tick_enqueue_order": ["walker_next_PTE", "data_AR_source0_source1", "data_W_native_accepted_AW_order"],
                    "tick_phase_order": ["due_B_receipts", "due_translation_permissions",
                        "new_copy_and_R_W_request_creation", "bank_lookup_source0_source1",
                        "walker_next_PTE_enqueue", "data_enqueue_and_native_AR_AW_receipts_source0_source1",
                        "walker_selection_output_generation_check", "one_FIFO_beat_service"]},
        "tree_PLRU": {"initial_bits": [0] * 15, "initial_ways": [None] * 16,
                      "invalid_first_added": False, "post_fill_hit_touch": True,
                      "bypass_touch": False, "duplicate_shared_history": False},
        "conditions_contract": {
            "C1": {"registry_feedback": True, "producer_tail_order": "p0..p13,r",
                   "AW_translation_after": "all eight R beats"},
            "C2": {"registry_feedback": False, "all_policies_fallback": "PLRU",
                   "paid_choice_ticks": 1, "work_dependency_ledger_remains": True},
            "C3": {"registry_feedback": True,
                   "ID_age_by_role": {"P": [2, 2], "B": [1, 1],
                                      "S2": [0, 0], "S3": [3, 3]},
                   "role_work_source_and_dependencies_unchanged": True},
            "C4": {"registry_feedback": True, "producer_tail_order": "p13..p0,r",
                   "AW_translation_after": "first returned R beat",
                   "W_requires_actual_R": True}},
        "default_ID_age_by_role": {"P": [0, 0], "B": [1, 1],
                                    "S2": [2, 2], "S3": [3, 3]},
        "source_by_role": {"P": 0, "B": 1, "S2": 1, "S3": 1},
        "source1_task_order": ["B", "S2", "S3"],
        "copies_by_condition": {c: copies_for(c) for c in ["C1", "C2", "C3", "C4"]},
        "formal_key": "S1 / Sv39 / same trusted address-space and epoch / 4KiB / nonglobal / RW / U=1,A=1,D=1; requests priv=0",
        "trusted_shared_address_space_assumption": "sources retain DIDs10/11 but explicitly configured same PSCID/root/current epoch; stock distinct-DID defaults do not establish shared cache identity",
        "full_buffer_read_certificate": "actual active aligned64B source job / 8beats <= original512beats; original FullBufferDID10+source exception; not synthesized from cache hit",
        "possible_current_software_supply": {"writes_per_copy":4,"NEXT_ID_reads_per_copy":1,
            "copies":51,"configuration_writes":204,"trigger_reads":51,
            "total_MMIO_operations":255,"nominal_register_bytes":2040,
            "actual_MMIO_bus_bytes":None,"polls":None,"CPU_ticks":None},
        "epoch": EPOCH, "errors_injected": 0, "all_responses": "OKAY",
        "measured_area": None, "measured_Fmax": None, "physical_CPU_cycles": None,
        "GPU_AI_application_gain": None,
        "fixed_rule_inputs": ["root/mechanism-card.md", "root/four-conditions-contract.md",
                              "control/fixed-program.md"]}
    return obj


class Episode:
    def __init__(self, condition, policy, input_obj, event_path):
        self.condition, self.policy, self.input = condition, policy, input_obj
        self.events = event_path.open("x")
        self.event_path = event_path
        self.tick, self.event_count = 0, 0
        self.cache = PLRU()
        self.feedback = input_obj["conditions_contract"][condition]["registry_feedback"]
        mapping = input_obj["conditions_contract"]["C3"]["ID_age_by_role"] if condition == "C3" else input_obj["default_ID_age_by_role"]
        self.tasks = {}
        self.remaining = {}
        self.user_generation = 0
        self.memory = {}
        self.pte_memory = {0x10000*4096 + 8: (0x10001<<10)|1,
                           0x10001*4096: (0x10002<<10)|1}
        for key,index in KINDEX.items():
            self.pte_memory[0x10002*4096+8*index] = ((0x20000+index)<<10)|0xD7
        self.expected_memory = {}
        self.copy_expected = {}
        self.done_copies = []
        self.counts = collections.Counter()
        self.service_queue = collections.deque()
        self.service_serial = 0
        self.timers = []
        self.requests = [None, None]
        self.active = [None, None]
        self.native_aw_ledger = collections.deque()
        self.native_aw_max = 0
        self.walker = None
        self.publication_tick = None
        self.stage_receipt_tick = None
        self.choice_snapshots = []
        self.first_key_lookups = []
        self.prefix_receipts = []
        self.max_payload_held = [0, 0]
        self.validations = collections.Counter()
        self.invalidations = 0
        self.lookup_port_reserved = False
        self.response_RR_prefer_fullwalk = True
        self.lookup_RR_source = 0
        self.epoch = EPOCH
        for role, cps in input_obj["copies_by_condition"][condition].items():
            identity, age = mapping[role]
            self.tasks[role] = dict(role=role, id=identity, age=age,
                source=input_obj["source_by_role"][role], copies=cps,
                next_copy=0, completion=None, consumer_receipt=None,
                owner_release=None, live=True, translation_retired_tick=None)
            self.remaining[role] = collections.Counter()
            for ci, cp in enumerate(cps):
                self.remaining[role][cp["src_key"]] += 1
                self.remaining[role][cp["dst_key"]] += 1
                for beat in range(8):
                    addr = pa(cp["src_key"], cp["src_block"]) + 8 * beat
                    self.memory[addr] = payload_value(cp["src_key"], cp["src_block"], beat)
        # Serial role-owned functional reference. Publication producer prefixes
        # precede all dependent consumers; disjoint destination blocks commute.
        self.expected_memory = dict(self.memory)
        for role in ["P", "B", "S2", "S3"]:
            for ci, cp in enumerate(self.tasks[role]["copies"]):
                data = [self.expected_memory[pa(cp["src_key"], cp["src_block"]) + 8*i] for i in range(8)]
                self.copy_expected[(role, ci)] = data
                for i, value in enumerate(data):
                    self.expected_memory[pa(cp["dst_key"], cp["dst_block"]) + 8*i] = value
        self.log("episode_start", condition=condition, policy=policy,
                 common_ready=0, cold=True, registry_feedback=self.feedback,
                 task_mapping={r: [t["id"], t["age"], t["source"]] for r,t in self.tasks.items()},
                 native_buffer_capacity_beats=[512, 512])
        self.log("registration_paid_boundary", tasks=4, key_records=18,
                 preparation_software_cycles=None, registration_port_area=None)

    def log(self, event_name, **details):
        obj = dict(tick=self.tick, seq=self.event_count, event=event_name, **details)
        self.events.write(json.dumps(obj, sort_keys=True) + "\n")
        self.event_count += 1

    def nonempty_U(self, prospective=None):
        out = {}
        for role, task in self.tasks.items():
            if not task["live"]:
                continue
            counter = self.remaining[role].copy()
            if prospective and prospective["role"] == role:
                assert counter[prospective["key"]] > 0
                counter[prospective["key"]] -= 1
            U = {k for k,n in counter.items() if n > 0}
            if U:
                out[role] = U
        return out

    def snapshot(self, req):
        U = self.nonempty_U(req)
        resident = self.cache.resident()
        return dict(tick=self.tick, req_id=req["id"], key=req["key"],
                    request_role=req["role"], task_id=req["task_id"],
                    prospective_current_private_handoff=True,
                    current_request_not_data_task_or_buffer_released=True,
                    user_generation=self.user_generation,
                    cache_generation=self.cache.generation, epoch=self.epoch,
                    resident=sorted(resident), ways=list(self.cache.ways),
                    tree=list(self.cache.bits),
                    U={r: sorted(u) for r,u in U.items()},
                    ids={r: self.tasks[r]["id"] for r in U},
                    ages={r: self.tasks[r]["age"] for r in U},
                    missing={r: sorted(u-resident) for r,u in U.items()})

    def select(self, snapshot):
        key = snapshot["key"]
        U = {r: set(u) for r,u in snapshot["U"].items()}
        resident = set(snapshot["resident"])
        refs = lambda k: {r for r,u in U.items() if k in u}
        legal = set(range(16))
        masked_victim = lambda protection: self.cache.victim({w for w in legal if self.cache.ways[w] not in protection} or legal)
        detail = {}
        if not self.feedback or self.policy == "PLRU":
            return dict(action="insert", way=self.cache.victim(legal), effective="PLRU", detail=detail)
        if self.policy == "pin":
            protect = set().union(*U.values()) if U else set()
            way = masked_victim(protect)
            detail = {"protected_keys": sorted(protect)}
        elif self.policy in ("oldest", "SJF", "fraction", "fractionInsert"):
            if not U:
                if self.policy == "fraction":
                    return dict(action="bypass", way=None, effective=self.policy,
                                detail={"reason": "no future registered incoming-key use"})
                return dict(action="insert", way=self.cache.victim(legal), effective=self.policy, detail={"target": None})
            if self.policy == "oldest":
                target = min(U, key=lambda r: (snapshot["ages"][r], snapshot["ids"][r]))
            elif self.policy == "SJF":
                target = min(U, key=lambda r: (len(U[r]-resident), len(U[r]), snapshot["ages"][r], snapshot["ids"][r]))
            else:
                Q = [r for r in U if key in U[r]]
                if self.policy == "fraction" and (not Q or all(5*len(U[r]&resident) < len(U[r]) for r in Q)):
                    return dict(action="bypass", way=None, effective=self.policy,
                                detail={"reason": "Q empty or all Q fractions below fixed 1/5"})
                def compare(a, b):
                    delta = len(U[a]&resident)*len(U[b]) - len(U[b]&resident)*len(U[a])
                    if delta:
                        return -1 if delta > 0 else 1
                    A = (len(U[a]-resident), len(U[a]), snapshot["ages"][a], snapshot["ids"][a])
                    B = (len(U[b]-resident), len(U[b]), snapshot["ages"][b], snapshot["ids"][b])
                    return (A>B) - (A<B)
                target = sorted(U, key=functools.cmp_to_key(compare))[0]
            protection = U[target]&resident
            way = masked_victim(protection)
            detail = dict(target=target, target_id=snapshot["ids"][target], protected_keys=sorted(protection))
        elif self.policy == "minrefs":
            scores = {w: len(refs(self.cache.ways[w])) for w in legal}
            best = min(scores.values())
            bypass = len(refs(key))
            if bypass < best:
                return dict(action="bypass", way=None, effective=self.policy,
                            detail={"minimum_insert_refs": best, "bypass_refs": bypass})
            way = self.cache.victim({w for w,n in scores.items() if n == best})
            detail = {"minimum_insert_refs": best, "bypass_refs": bypass,
                      "way_refcounts": scores}
        else:
            assert self.policy == "RCS1"
            H = set().union(*U.values()) if U else set()
            M = H-resident
            eligible = {r for r,u in U.items() if (u-resident) <= {key}}
            bypass_score = (sum(u<=resident for u in U.values()), len(M))
            scores = {}
            for w in legal:
                users = refs(self.cache.ways[w])
                scores[w] = (len(eligible)-len(users&eligible), len(M-{key})+int(bool(users)))
            best = max(scores.values(), key=lambda x: (x[0], -x[1]))
            if (bypass_score[0], -bypass_score[1]) > (best[0], -best[1]):
                return dict(action="bypass", way=None, effective=self.policy,
                            detail={"E_q": sorted(eligible), "scores": scores,
                                    "bypass_score": bypass_score})
            way = self.cache.victim({w for w,s in scores.items() if s == best})
            detail = {"E_q": sorted(eligible), "scores": scores,
                      "bypass_score": bypass_score,
                      "algebra": "exact filtered-refs; no serial 17-action service"}
        return dict(action="insert", way=way, effective=self.policy, detail=detail)

    def enqueue(self, kind, **fields):
        self.service_serial += 1
        op = dict(serial=self.service_serial, kind=kind, accepted_tick=self.tick, **fields)
        self.service_queue.append(op)
        self.log("service_enqueue", queue_depth=len(self.service_queue), **op)

    def timer(self, kind, due, **fields):
        self.timers.append(dict(kind=kind, due=due, **fields))

    def new_request(self, active, access):
        cp = active["cp"]
        key = cp["src_key"] if access == "R" else cp["dst_key"]
        block = cp["src_block"] if access == "R" else cp["dst_block"]
        task = self.tasks[active["role"]]
        req = dict(id=f"{active['role']}:{active['ci']}:{access}", role=active["role"],
                   task_id=task["id"], source=task["source"], ci=active["ci"],
                   access=access, key=key, block=block, va=va(key, block),
                   epoch=self.epoch, owner=f"source{task['source']}-copy{active['ci']}",
                   priv=False, status="held", created_tick=self.tick)
        assert self.requests[req["source"]] is None
        self.requests[req["source"]] = req
        if access == "W":
            self.native_aw_ledger.append((req["source"], req["role"], req["ci"]))
            self.native_aw_max = max(self.native_aw_max,len(self.native_aw_ledger))
            assert len(self.native_aw_ledger) <= 2
            self.counts["native_input_AW"] += 1
            self.log("native_input_AW_order_accept", req_id=req["id"], source=req["source"],
                     AW_order=list(self.native_aw_ledger), uses_original_inflight_capacity=True)
        else:
            certified = req["va"]%64 == 0 and active["cp"]["bytes"] == 64 and BEATS<=512
            assert certified
            self.counts["native_input_AR"] += 1
            self.counts["full_buffer_read_certificates"] += 1
            restricted = bool(self.native_aw_ledger)
            if restricted and self.native_aw_ledger[0][0] != req["source"]:
                self.counts["other_bank_read_fullbuffer_guard_exceptions"] += 1
            self.log("native_read_SELECT_admission", req_id=req["id"], source=req["source"],
                     native_read_guard_restricted=restricted,
                     W_owner=None if not self.native_aw_ledger else self.native_aw_ledger[0][0],
                     fullbuffer_job_certificate=True, actual_job_bytes=64, actual_R_beats=8,
                     capacity_beats=512, DID=10+req["source"], FullBufferDID=10+req["source"],
                     certificate_from_actual_job_not_cache_hit=True)
        self.counts["translation_requests"] += 1
        self.log("translation_request_held", **req)
        return req

    def retire_use(self, req):
        role,key = req["role"], req["key"]
        assert self.remaining[role][key] > 0
        self.remaining[role][key] -= 1
        self.user_generation += 1
        self.counts["private_permission_handoffs"] += 1
        if not any(n>0 for n in self.remaining[role].values()):
            self.tasks[role]["translation_retired_tick"] = self.tick
        self.log("last_use_feedback", req_id=req["id"], role=role, key=key,
                 remaining_same_key_uses=self.remaining[role][key],
                 formal_key_left_U=self.remaining[role][key]==0,
                 task_data_owner_released=False, generation=self.user_generation)

    def translation_delivery(self, item):
        req = item["req"]
        source = req["source"]
        assert self.requests[source] is req
        assert req["epoch"] == self.epoch and req["owner"] == item["owner"]
        assert item["key"] == req["key"] and item["physical_address"] == pa(req["key"], req["block"])
        assert item["permissions"] == "RW" and item["response"] == "OKAY"
        assert self.active[source]["role"] == req["role"] and self.active[source]["ci"] == req["ci"]
        if item["fullwalk"]:
            assert self.walker["req"] is req
            assert self.walker["state"] == "result_held"
            if item["inserted_way"] is not None:
                assert self.cache.ways[item["inserted_way"]] == req["key"]
                self.cache.touch(item["inserted_way"])
                self.lookup_port_reserved = True
                self.log("native_post_fill_hit_touch", way=item["inserted_way"], key=req["key"], tree=list(self.cache.bits))
            self.walker = None
        self.requests[source] = None
        act = self.active[source]
        act[req["access"]+"_permission"] = item["physical_address"]
        act[req["access"]+"_private_req"] = req
        self.counts["result_owner_identity_checks"] += 1
        self.counts["permission_epoch_checks"] += 1
        self.log("private_permission_handoff", req_id=req["id"], source=source,
                 role=req["role"], access=req["access"], fullwalk=item["fullwalk"],
                 physical_address=item["physical_address"], response="OKAY", result_owner=req["owner"])

    def handle_timers(self):
        due = [x for x in self.timers if x["due"] <= self.tick]
        self.timers = [x for x in self.timers if x["due"] > self.tick]
        priority = {"B": 0, "stage_receipt": 1, "final_receipt": 1, "translation": 2}
        due.sort(key=lambda x: (priority[x["kind"]], x.get("source", 0)))
        trans = [x for x in due if x["kind"] == "translation"]
        if len(trans) > 1:
            # Native TL shares one response port, with ordinary RR between
            # full-walk slow output and fast output. No new response slot.
            preferred = next((x for x in trans if (x["response_channel"] == "slow") == self.response_RR_prefer_fullwalk), trans[0])
            for item in trans:
                if item is not preferred:
                    due.remove(item)
                    item["due"] = self.tick+1
                    self.timers.append(item)
                    self.counts["translation_response_port_hold_ticks"] += 1
                    if item["fullwalk"]:
                        self.counts["full_result_holder_ticks"] += 1
                    self.log("translation_response_held_stable", req_id=item["req"]["id"],
                             source=item["source"], fullwalk=item["fullwalk"],
                             physical_address=item["physical_address"], owner=item["owner"],
                             shared_response_port_busy=True)
        for item in due:
            kind = item["kind"]
            if kind == "translation":
                self.translation_delivery(item)
                self.response_RR_prefer_fullwalk = item["response_channel"] != "slow"
            elif kind == "stage_receipt":
                assert self.publication_tick is not None and self.tick == self.publication_tick + 1
                self.stage_receipt_tick = self.tick
                self.counts["publication_stage_receipts"] += 1
                self.log("producer_stage_receipt", producer_B_tick=self.publication_tick,
                         dependents_now_issue_eligible=True, receipt_cost_ticks=1)
            elif kind == "final_receipt":
                task = self.tasks[item["role"]]
                assert self.active[task["source"]] is None
                assert task["next_copy"] == len(task["copies"])
                assert not any(n>0 for n in self.remaining[item["role"]].values())
                task["completion"] = task["consumer_receipt"] = task["owner_release"] = self.tick
                task["live"] = False
                self.user_generation += 1
                self.counts["consumer_receipts"] += 1
                self.counts["owner_releases"] += 1
                self.log("consumer_receipt_owner_release", role=item["role"], task_id=task["id"],
                         final_B_tick=item["B_tick"], receipt_cost_ticks=1,
                         payload_correct=True, release_after_R_W_B=True)
            else:
                assert kind == "B"
                source, act = item["source"], self.active[item["source"]]
                assert act["W_serviced"] == act["R_received"] == 8
                assert act["W_queued"] == 8 and act["R_permission"] is not None and act["W_permission"] is not None
                role,ci,cp = act["role"],act["ci"],act["cp"]
                actual = [self.memory[pa(cp["dst_key"], cp["dst_block"])+8*i] for i in range(8)]
                assert actual == self.copy_expected[(role,ci)]
                self.counts["data_B"] += 1
                self.counts["copy_payload_checks"] += 1
                self.done_copies.append((role,ci))
                self.log("B_OKAY_copy_complete", role=role, ci=ci, source=source,
                         R_beats=8, W_beats=8, response="OKAY", payload_correct=True,
                         W_last_service_tick=item["last_W_tick"], ordinary_B_response_ticks=1)
                self.active[source] = None
                task = self.tasks[role]
                if role == "P" and ci < 16:
                    self.prefix_receipts.append(dict(ci=ci, tick=self.tick,
                        key=cp["src_key"], resident=sorted(self.cache.resident()), ways=list(self.cache.ways)))
                if cp["publication"]:
                    self.publication_tick = self.tick
                    self.timer("stage_receipt", self.tick+1)
                    self.log("producer_publication_B", output_key="q", output_block=1,
                             consumer_issue_eligible=False)
                if task["next_copy"] == len(task["copies"]):
                    self.timer("final_receipt", self.tick+1, role=role, B_tick=self.tick)

    def issue_copies_and_requests(self):
        for source in [0, 1]:
            if self.active[source] is None:
                role = "P" if source == 0 else next((r for r in ["B", "S2", "S3"] if self.tasks[r]["completion"] is None), None)
                if role is None:
                    continue
                task = self.tasks[role]
                ready = source == 0 or self.stage_receipt_tick is not None
                if ready and task["next_copy"] < len(task["copies"]):
                    ci = task["next_copy"]
                    if ci:
                        assert (role,ci-1) in self.done_copies
                    task["next_copy"] += 1
                    self.active[source] = dict(role=role, ci=ci, cp=task["copies"][ci],
                        R_permission=None, W_permission=None, R_requested=False,
                        W_requested=False, AR=False, AW=False, R_received=0,
                        R_serviced=0, W_queued=0, W_serviced=0, data=[], started=self.tick)
                    self.counts["copies_started"] += 1
                    self.log("copy_start", role=role, task_id=task["id"], source=source, ci=ci,
                             producer_stage_receipt=self.stage_receipt_tick,
                             one_active_copy_per_source=True)
            act = self.active[source]
            if act is None:
                continue
            if not act["R_requested"]:
                self.new_request(act, "R")
                act["R_requested"] = True
            can_write = act["R_received"] >= (1 if self.condition == "C4" else 8)
            if can_write and not act["W_requested"] and self.requests[source] is None:
                self.new_request(act, "W")
                act["W_requested"] = True

    def lookup_banks(self):
        for source in [self.lookup_RR_source, 1-self.lookup_RR_source]:
            req = self.requests[source]
            if req is None:
                continue
            # Held identity is checked every tick, including blocked misses.
            assert req["va"] == va(req["key"], req["block"])
            assert req["task_id"] == self.tasks[req["role"]]["id"] and req["epoch"] == self.epoch
            self.validations["held_request_identity_tick_checks"] += 1
            if req["status"] != "held":
                continue
            if self.lookup_port_reserved:
                self.counts["shared_lookup_port_hold_ticks"] += 1
                continue
            key = req["key"]
            fast = self.walker is not None
            if fast:
                active_window = self.walker["state"] in ("need_PTE", "PTE_wait")
                other_bank = source != self.walker["req"]["source"]
                fast_owned = any(x["kind"] == "translation" and x.get("response_channel") == "fast" for x in self.timers)
                if not active_window or not other_bank or fast_owned:
                    self.counts["native_fast_window_or_capacity_hold_ticks"] += 1
                    continue
            if key in self.cache.resident():
                way = self.cache.ways.index(key)
                content = self.cache.contents[way]
                assert content["epoch"] == req["epoch"] and content["RW"] is True
                self.cache.touch(way)
                req["status"] = "hit_permission_pending"
                self.counts["translation_hits"] += 1
                if self.walker:
                    self.counts["other_bank_hits_while_PTW_busy"] += 1
                self.timer("translation", self.tick+1, source=source, req=req,
                    key=key, owner=req["owner"], physical_address=content["PPN"]*4096+(req["va"]&4095),
                    permissions="RW", response="OKAY", fullwalk=False, inserted_way=None,
                    response_channel="fast" if fast else "slow")
                self.log("TL_hit_permission_pending", req_id=req["id"], way=way,
                         PTW_busy=self.walker is not None, tree=list(self.cache.bits), permission_ticks=1)
                self.lookup_port_reserved = True
                self.lookup_RR_source = 1-source
            elif self.walker is None:
                req["status"] = "walker_owned"
                self.walker = dict(req=req, state="need_PTE", level=2,
                                   table_ppn=0x10000, pte_inflight=False,
                                   start=self.tick, result=None)
                self.counts["walks_started"] += 1
                if key not in self.first_key_lookups:
                    self.first_key_lookups.append(key)
                self.log("fullwalk_start", req_id=req["id"], key=key, source=source,
                         level=2, root_ppn=0x10000, single_walker=True)
                self.lookup_port_reserved = True
                self.lookup_RR_source = 1-source
            else:
                self.counts["miss_hold_ticks_PTW_busy"] += 1

    def walker_progress(self, choice_output):
        walker = self.walker
        if walker is None:
            return
        req = walker["req"]
        if choice_output and walker["state"] == "choice_pending" and walker["choice_due"] == self.tick:
            snap = walker["snapshot"]
            current = (self.cache.generation, self.user_generation, self.epoch)
            saved = (snap["cache_generation"], snap["user_generation"], snap["epoch"])
            stale = current != saved
            if stale:
                action = dict(action="insert", way=self.cache.victim(set(range(16))),
                              effective="PLRU_generation_fallback", detail={"saved_generations": saved,"current_generations": current})
                self.counts["stale_snapshot_fallbacks"] += 1
            else:
                action = walker["action"]
            way = action["way"]
            victim = None if way is None else self.cache.ways[way]
            if action["action"] == "insert":
                self.cache.fill(way, req["key"], walker["result_content"])
                self.counts["cache_insertions"] += 1
            else:
                self.counts["cache_bypasses"] += 1
            record = dict(snapshot=snap, planned_action=walker["action"], applied_action=action,
                          output_tick=self.tick, victim_key=victim, stale_fallback=stale,
                          selection_ticks=1)
            self.choice_snapshots.append(record)
            self.log("policy_choice_apply", **record)
            walker["state"] = "result_held"
            walker["result"] = dict(req_id=req["id"], owner=req["owner"], epoch=self.epoch,
                key=req["key"], physical_address=walker["result_content"]["PPN"]*4096+(req["va"]&4095),
                permissions="RW", response="OKAY", full_content=walker["result_content"].copy())
            self.counts["paid_choice_ticks"] += 1
            self.counts["full_result_holder_ticks"] += 1
            self.log("normal_full_result_held", **walker["result"], result_permission_ticks=1,
                     bypass=way is None, no_new_PTW_until_owner_receipt=True)
            self.timer("translation", self.tick+1, source=req["source"], req=req,
                       key=req["key"], owner=req["owner"],
                       physical_address=walker["result"]["physical_address"], permissions="RW",
                       response="OKAY", fullwalk=True, inserted_way=way, response_channel="slow")
        if walker["state"] == "need_PTE" and not walker["pte_inflight"]:
            shift = 12 + 9 * walker["level"]
            index = (req["va"] >> shift) & 511
            addr = walker["table_ppn"]*4096+8*index
            self.enqueue("PTE_R", req_id=req["id"], source=req["source"],
                         level=walker["level"], address=addr, bytes=8)
            self.counts["PTE_AR"] += 1
            walker["pte_inflight"] = True
            walker["state"] = "PTE_wait"
            self.log("PTE_AR_accepted", req_id=req["id"], level=walker["level"], address=addr,
                     dependent_previous_response=True, beats=1)

    def data_enqueue(self):
        for source in [0, 1]:
            act = self.active[source]
            if act is None:
                continue
            if act["R_permission"] is not None and not act["AR"]:
                act["AR"] = True
                self.retire_use(act["R_private_req"])
                self.counts["data_AR"] += 1
                self.log("data_AR_accepted", role=act["role"], ci=act["ci"], source=source,
                         address=act["R_permission"], beats=8, ID=source)
                for beat in range(8):
                    self.enqueue("data_R", source=source, role=act["role"], ci=act["ci"],
                                 beat=beat, address=act["R_permission"]+8*beat, bytes=8)
            if act["W_permission"] is not None and not act["AW"]:
                act["AW"] = True
                self.retire_use(act["W_private_req"])
                self.counts["data_AW"] += 1
                self.log("data_AW_accepted", role=act["role"], ci=act["ci"], source=source,
                         address=act["W_permission"], beats=8, ID=source,
                         actual_R_received=act["R_received"], streaming=self.condition=="C4")
            held = act["R_received"]-act["W_serviced"]
            assert 0 <= held <= 8 and held <= 512
            self.max_payload_held[source] = max(self.max_payload_held[source], held)
        # AWs may be prepared by both banks; W belongs to the oldest accepted
        # native input AW until WLAST is accepted into this FIFO abstraction.
        while self.native_aw_ledger:
            source,role,ci = self.native_aw_ledger[0]
            act = self.active[source]
            assert act is not None and (act["role"],act["ci"]) == (role,ci)
            if act["AW"]:
                while act["W_queued"] < act["R_received"]:
                    beat = act["W_queued"]
                    assert beat < len(act["data"])
                    value = act["data"][beat]
                    self.enqueue("data_W", source=source, role=act["role"], ci=act["ci"],
                                 beat=beat, address=act["W_permission"]+8*beat, bytes=8,
                                 value=value, corresponding_R_already_received=True)
                    act["W_queued"] += 1
            if act["W_queued"] == 8:
                self.native_aw_ledger.popleft()
                self.counts["native_WLAST_owner_releases"] += 1
                self.log("native_WLAST_input_owner_release", source=source, role=role, ci=ci,
                         WLAST_FIFO_accepted=True, DDR_WLAST_need_not_have_serviced=True,
                         B_independent=True, remaining_AW_order=list(self.native_aw_ledger))
            else:
                break

    def service_one(self):
        if not self.service_queue:
            self.counts["FIFO_idle_ticks"] += 1
            return
        op = self.service_queue.popleft()
        self.counts["service_beats"] += 1
        self.log("service_beat", **op, response="OKAY", queue_remaining=len(self.service_queue))
        if op["kind"] == "PTE_R":
            walker = self.walker
            assert walker["state"] == "PTE_wait" and walker["req"]["id"] == op["req_id"]
            assert walker["level"] == op["level"]
            pte = self.pte_memory[op["address"]]
            if op["level"] > 0:
                assert pte&1 and not pte&6
                walker["table_ppn"] = (pte >> 10) & ((1<<44)-1)
                walker["level"] -= 1
                walker["state"] = "need_PTE"
            else:
                assert pte&0xD7 == 0xD7
                assert ((pte >> 10) << 12) + (walker["req"]["va"] & 4095) == pa(walker["req"]["key"],walker["req"]["block"])
                walker["result_content"] = dict(PPN=pte>>10,RW=True,V=True,A=True,D=True,
                    U=True,G=False,page_bytes=4096,stage="S1",epoch=self.epoch,
                    returned_leaf_PTE=pte)
                self.counts["successful_full_walks"] += 1
                walker["state"] = "choice_pending"
                walker["choice_due"] = self.tick+1
                walker["snapshot"] = self.snapshot(walker["req"])
                walker["action"] = self.select(walker["snapshot"])
                self.log("policy_choice_input", snapshot=walker["snapshot"], planned_action=walker["action"],
                         paid_choice_ticks=1, walk_actual_service_ticks=self.tick-walker["start"]+1)
            walker["pte_inflight"] = False
            self.counts["PTE_R"] += 1
            self.counts["PTE_bytes"] += 8
            self.log("PTE_R_OKAY", req_id=op["req_id"], level=op["level"],
                     address=op["address"], returned_content=pte,
                     next_level_uses_this_response=op["level"]>0)
        else:
            source = op["source"]
            act = self.active[source]
            assert act is not None and (act["role"],act["ci"]) == (op["role"],op["ci"])
            if op["kind"] == "data_R":
                assert op["beat"] == act["R_received"]
                value = self.memory[op["address"]]
                assert value == self.copy_expected[(act["role"],act["ci"])][op["beat"]]
                act["data"].append(value)
                act["R_received"] += 1
                self.counts["data_R"] += 1
                self.counts["data_read_bytes"] += 8
                self.log("R_OKAY_owner_receipt", role=act["role"], ci=act["ci"], source=source,
                         beat=op["beat"], value=value, ID=source)
            else:
                assert op["kind"] == "data_W"
                assert op["beat"] == act["W_serviced"] and op["beat"] < act["R_received"]
                assert op["value"] == act["data"][op["beat"]]
                self.memory[op["address"]] = op["value"]
                act["W_serviced"] += 1
                self.counts["data_W"] += 1
                self.counts["data_write_bytes"] += 8
                self.log("W_OKAY", role=act["role"], ci=act["ci"], source=source,
                         beat=op["beat"], value=op["value"], ID=source,
                         corresponding_R_received=True)
                if act["W_serviced"] == 8:
                    self.timer("B", self.tick+1, source=source, last_W_tick=self.tick)
            self.max_payload_held[source] = max(self.max_payload_held[source],act["R_received"]-act["W_serviced"])

    def quiet(self):
        return all(t["completion"] is not None for t in self.tasks.values()) and not any(self.active) and not any(self.requests) and self.walker is None and not self.service_queue and not self.timers and not self.native_aw_ledger

    def run(self):
        for self.tick in range(100000):
            self.lookup_port_reserved = False
            self.handle_timers()
            self.issue_copies_and_requests()
            self.lookup_banks()
            self.walker_progress(choice_output=False)
            self.data_enqueue()
            self.walker_progress(choice_output=True)
            self.service_one()
            assert len({k for k in self.cache.ways if k is not None}) == len([k for k in self.cache.ways if k is not None])
            if self.quiet():
                break
        else:
            raise RuntimeError("finite episode failed to reach quiet; no parameter adaptation")
        c = self.counts
        assert c["copies_started"] == c["data_AR"] == c["data_AW"] == c["data_B"] == 51
        assert c["data_R"] == c["data_W"] == 408
        assert c["data_read_bytes"] == c["data_write_bytes"] == 3264
        assert c["translation_requests"] == c["private_permission_handoffs"] == 102
        assert c["PTE_AR"] == c["PTE_R"] == 3*c["successful_full_walks"]
        assert c["walks_started"] == c["successful_full_walks"]
        assert c["paid_choice_ticks"] == c["successful_full_walks"]
        assert c["service_beats"] == c["PTE_R"]+c["data_R"]+c["data_W"]
        assert c["consumer_receipts"] == c["owner_releases"] == 4
        assert c["publication_stage_receipts"] == 1
        assert c["copy_payload_checks"] == 51
        assert c["native_input_AR"] == c["native_input_AW"] == c["native_WLAST_owner_releases"] == c["full_buffer_read_certificates"] == 51
        assert len(self.done_copies) == 51 and len(set(self.done_copies)) == 51
        assert self.first_key_lookups[:16] == ["a"]+[f"p{i}" for i in range(7)]+["b"]+[f"p{i}" for i in range(7,14)]
        dest_checks = 0
        for t in self.tasks.values():
            for cp in t["copies"]:
                for i in range(8):
                    addr = pa(cp["dst_key"],cp["dst_block"])+8*i
                    assert self.memory[addr] == self.expected_memory[addr]
                    dest_checks += 1
        qrecords = [x for x in self.choice_snapshots if x["snapshot"]["req_id"] == "P:16:W"]
        assert len(qrecords) == 1
        qsnap = qrecords[0]["snapshot"]
        expected_U = {"P": sorted([f"p{i}" for i in range(14)]+["r"]),
                      "B": sorted([f"p{i}" for i in range(14)]+["r"]),
                      "S2": ["a","q"], "S3": ["b","q"]}
        assert qsnap["U"] == expected_U
        self.log("episode_quiet", all_data_complete=True, all_consumer_receipts=True,
                 all_owner_released=True, FIFO_empty=True, PTW_empty=True,
                 held_requests_empty=True, policy_result_empty=True,
                 error_count=0, error_injections=0, error_termination_validated=False,
                 final_destination_beat_checks=dest_checks)
        self.events.close()
        result = dict(condition=self.condition, policy=self.policy, status="passed",
            model_ticks=dict(common_ready=0, completion_max=max(t["completion"] for t in self.tasks.values()),
                             completion_sum=sum(t["completion"] for t in self.tasks.values()),
                             quiet=self.tick),
            tasks={r: {k:t[k] for k in ["role","id","age","source","completion",
                "consumer_receipt","owner_release","translation_retired_tick"]} for r,t in self.tasks.items()},
            actual_counts=dict(c), AXI_counts=dict(AR=c["data_AR"]+c["PTE_AR"], AW=c["data_AW"],
                R=c["data_R"]+c["PTE_R"], W=c["data_W"], B=c["data_B"],
                payload_AR=c["data_AR"], payload_AW=c["data_AW"], payload_R=c["data_R"],
                payload_W=c["data_W"], payload_B=c["data_B"], PTE_AR=c["PTE_AR"], PTE_R=c["PTE_R"]),
            prefix=dict(first_unique_fullwalk_keys=self.first_key_lookups[:16],
                        real_payload_copy_receipts=self.prefix_receipts,
                        extra_warmup_copies=0, cache_initialization=False,
                        final_first16_B_tick=self.prefix_receipts[-1]["tick"]),
            producer_publication_B_tick=self.publication_tick,
            producer_stage_receipt_tick=self.stage_receipt_tick,
            q_choice=qrecords[0], all_choice_records=self.choice_snapshots,
            checks=dict(all_data_complete=True, payload_destination_checks=dest_checks,
                        consumer_receipts=4, owner_releases=4, quiet=True,
                        source_max_payload_held_beats=self.max_payload_held,
                        source_buffer_capacity_beats=[512,512],
                        no_cache_preload=True, no_extra_work=True,
                        request_identity_tick_checks=self.validations["held_request_identity_tick_checks"],
                        native_AW_order_preserved=True,native_W_owner_released_at_input_WLAST=True,
                        native_max_AW_order_depth=self.native_aw_max,
                        original_fullbuffer_read_guard_exception=True,
                        dependent_PTE_responses=True, one_PT_Walker=True,
                        same_ID_in_source_serial=True, all_responses_OKAY=True,
                        error_count=0, error_injections=0, injected_error_termination_validated=False),
            events=dict(path=str(self.event_path), count=self.event_count, sha256=sha(self.event_path)),
            physical_area=None, Fmax=None, physical_CPU_cycles=None, application_gain=None)
        return result


def run_all(args):
    input_path = HERE / "frozen-input.json"
    input_obj = json.loads(input_path.read_text())
    manifest_path = HERE / "execution-ledger.json"
    existing = None
    if manifest_path.exists():
        if not args.resume_after_failure:
            raise SystemExit("execution ledger already exists: refused unlogged repeated execution")
        existing = json.loads(manifest_path.read_text())
        assert existing["status"] == "implementation_failure_stopped"
        assert existing["input_sha256"] == sha(input_path), "recovery cannot modify frozen input"
        history = HERE / f"execution-ledger-before-recovery{len(existing.get('recoveries',[]))+1}.json"
        assert not history.exists()
        history.write_bytes(manifest_path.read_bytes())
    gate_path = pathlib.Path(args.gate).resolve()
    gate = json.loads(gate_path.read_text())
    assert gate.get("status") == "pass_for_exactly_four_new_conditional_model_conditions", "root gate must explicitly authorize model entry"
    assert gate.get("runs_per_condition") == 8 and gate.get("new_conditions_limit") == 4
    for item in gate["files"]:
        frozen_file = HERE.parent / item["path"]
        assert sha(frozen_file) == item["sha256"], "root gate file SHA mismatch: "+item["path"]
    assert args.expected_input_sha == sha(input_path), "frozen input hash mismatch"
    ledger = existing or dict(version="RCS1-execution-ledger-v1", author="Codex", model="gpt-6.1-sol",
        reasoning_effort="ultra", experiment="new conditional engine, no old replay",
        started_UTC=datetime.datetime.now(datetime.timezone.utc).isoformat(),
        input_sha256=sha(input_path), engine_sha256=sha(pathlib.Path(__file__)),
        gate_path=str(gate_path), gate_sha256=sha(gate_path), argv=sys.argv,
        planned_conditions=input_obj["conditions"], planned_policies=input_obj["policies"],
        sweep=False, old_assets_executed=False, runs=[], status="running")
    if existing:
        ledger.setdefault("recoveries",[]).append(dict(
            time_UTC=datetime.datetime.now(datetime.timezone.utc).isoformat(),
            engine_sha256=sha(pathlib.Path(__file__)), argv=sys.argv,
            reason="specific implementation failure; preserved first ledger and raw events; no input/rule/timing tuning"))
        ledger["status"] = "running"
    jwrite(manifest_path, ledger)
    results = [json.loads(pathlib.Path(r["result_path"]).read_text())
               for r in ledger["runs"] if r["status"] == "passed"]
    passed_cases = {(r["condition"],r["policy"]) for r in ledger["runs"] if r["status"] == "passed"}
    failed = False
    for condition in input_obj["conditions"]:
        for policy in input_obj["policies"]:
            if (condition,policy) in passed_cases:
                continue
            attempt = 1+sum(r["condition"]==condition and r["policy"]==policy for r in ledger["runs"])
            start = datetime.datetime.now(datetime.timezone.utc).isoformat()
            suffix = "" if attempt == 1 else f".retry{attempt}"
            event_path = HERE / f"{condition}-{policy}{suffix}.events.jsonl"
            run_entry = dict(condition=condition, policy=policy, attempt=attempt,
                             engine_sha256=sha(pathlib.Path(__file__)),
                             started_UTC=start, event_path=str(event_path))
            episode = None
            try:
                episode = Episode(condition, policy, input_obj, event_path)
                result = episode.run()
                result_path = HERE / f"{condition}-{policy}.result.json"
                jwrite(result_path, result)
                run_entry.update(status="passed", result_path=str(result_path), result_sha256=sha(result_path))
                results.append(result)
            except Exception as exc:
                if episode and not episode.events.closed:
                    episode.log("implementation_failure", error_type=type(exc).__name__, message=str(exc))
                    episode.events.close()
                failure_path = HERE / f"{condition}-{policy}{suffix}.failure.txt"
                failure_path.write_text(traceback.format_exc())
                run_entry.update(status="failed", failure_path=str(failure_path), error=str(exc))
                failed = True
            run_entry["finished_UTC"] = datetime.datetime.now(datetime.timezone.utc).isoformat()
            ledger["runs"].append(run_entry)
            jwrite(manifest_path, ledger)
            print(json.dumps({k:run_entry[k] for k in ["condition","policy","status"]}), flush=True)
            if failed:
                break
        if failed:
            break
    if not failed:
        c2 = [r for r in results if r["condition"] == "C2"]
        reference = c2[0]
        for result in c2[1:]:
            assert result["model_ticks"] == reference["model_ticks"]
            assert result["actual_counts"] == reference["actual_counts"]
        summary = dict(version="RCS1-conditional-results-v1", input_sha256=sha(input_path),
            results=[dict(condition=r["condition"], policy=r["policy"], model_ticks=r["model_ticks"],
                tasks=r["tasks"], actual_counts=r["actual_counts"], AXI_counts=r["AXI_counts"],
                q_action=r["q_choice"]["applied_action"], q_victim=r["q_choice"]["victim_key"],
                q_resident=r["q_choice"]["snapshot"]["resident"], checks=r["checks"])
                for r in results],
            C2_all_programs_equal=True, physical_area=None, Fmax=None,
            physical_CPU_cycles=None, GPU_AI_application_gain=None,
            conclusions="conditional model evidence only; root accepts or closes frozen hypothesis")
        jwrite(HERE / "results-summary.json", summary)
        ledger.update(status="complete", executed_runs=len(ledger["runs"]),
                      successful_policy_episodes=len(results),
                      conditions_executed_once=not any(r["attempt"]>1 for r in ledger["runs"]),
                      each_policy_episode_completed_once=True,
                      checks_passed=True, summary_sha256=sha(HERE/"results-summary.json"))
    else:
        ledger.update(status="implementation_failure_stopped", executed_runs=len(ledger["runs"]),
                      conditions_executed_once=False, checks_passed=False)
    ledger["finished_UTC"] = datetime.datetime.now(datetime.timezone.utc).isoformat()
    jwrite(manifest_path, ledger)
    return 1 if failed else 0


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--prepare-input", action="store_true")
    parser.add_argument("--run", action="store_true")
    parser.add_argument("--gate")
    parser.add_argument("--expected-input-sha")
    parser.add_argument("--resume-after-failure", action="store_true")
    args = parser.parse_args()
    if args.prepare_input:
        path = HERE / "frozen-input.json"
        if path.exists():
            raise SystemExit("input already exists: freeze replacement requires explicit tracked correction")
        jwrite(path, make_input())
        jwrite(HERE/"frozen-input-sha.json", dict(path=str(path),sha256=sha(path),
                                               prepared_without_model_run=True))
        print(sha(path))
        return 0
    if args.run and args.gate and args.expected_input_sha:
        return run_all(args)
    parser.error("use --prepare-input, or --run --gate ROOT_GATE --expected-input-sha HASH")


if __name__ == "__main__":
    sys.exit(main())
