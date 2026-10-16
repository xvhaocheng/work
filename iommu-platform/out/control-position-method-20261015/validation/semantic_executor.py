#!/usr/bin/env python3
"""Execute frozen HERO source semantics. No clock, AXI, cache or RTL model.

One logical step atomically executes one eligible DAG node. Publication FIFO
is an explicit allowed completion order, not a hardware ordering guarantee.
Root owns the frozen cases, policy interpretation, invocation and acceptance.
"""

from __future__ import annotations

import argparse
from collections import Counter, deque
from dataclasses import dataclass, field
import hashlib
import json
from pathlib import Path
import struct
import sys
from typing import Any


WIDTH = HEIGHT = 32
STRIPE = 16
SLOT_BYTES = 2048
ARRAY_BYTES = 4096
MASK = (1 << 32) - 1
SLOTS = ("a0", "a1", "b0", "b1", "c0", "c1")
CASE_IDS = ("P", "N", "H1", "H2")
POLICIES = ("source_A_first", "source_current_need")
KINDS = (
    "map_in", "alloc", "publish", "dma_complete", "wait", "compute",
    "join", "free", "map_out", "visibility", "compare", "host_free",
)
JOB_ORDER = ("A0", "A1", "B00", "B01", "B10", "B11", "C0", "C1")
JOB_SPECS = {
    "A0": ("A", 0, "a0", "h2d"),
    "A1": ("A", 1, "a1", "h2d"),
    "B00": ("B", 0, "b0", "h2d"),
    "B01": ("B", 1, "b1", "h2d"),
    "B10": ("B", 0, "b0", "h2d"),
    "B11": ("B", 1, "b1", "h2d"),
    "C0": ("C", 0, "c0", "d2h"),
    "C1": ("C", 1, "c1", "d2h"),
}


class ContractError(RuntimeError):
    pass


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ContractError(message)


def digest(data: bytes | bytearray) -> str:
    return hashlib.sha256(data).hexdigest()


def pack(values: list[int]) -> bytearray:
    return bytearray(struct.pack("<" + "I" * len(values), *values))


def matrix_reference(a: bytearray, b: bytearray) -> bytearray:
    """Independent full CPU reference, without the tiled kernel's indexing."""
    av = struct.unpack("<1024I", a)
    bv = struct.unpack("<1024I", b)
    out = []
    for row in range(HEIGHT):
        for column in range(WIDTH):
            total = 0
            for inner in range(WIDTH):
                total = (total + av[row * WIDTH + inner] *
                         bv[column * WIDTH + inner]) & MASK
            out.append(total)
    return pack(out)


def normalized_job_details(details: dict[str, Any]) -> tuple[str, str, int, str, str]:
    job = details.get("job_id")
    require(job in JOB_SPECS, f"unknown job_id {job!r}")
    array, stripe, slot, direction = JOB_SPECS[job]
    require(str(details.get("host_array", "")).upper() == array,
            f"{job}: host_array differs from source")
    require(type(details.get("stripe")) is int and details["stripe"] == stripe,
            f"{job}: stripe differs from source")
    require(str(details.get("slot", "")).lower() == slot,
            f"{job}: slot differs from source")
    require(details.get("direction") == direction,
            f"{job}: direction differs from source")
    require(details.get("bytes", SLOT_BYTES) == SLOT_BYTES,
            f"{job}: each copy must contain 2048 bytes")
    return job, array, stripe, slot, direction


def compute_coordinates(node: dict[str, Any]) -> tuple[int, int]:
    details = node["details"]
    s, t = details.get("s"), details.get("t")
    require(type(s) is int and type(t) is int and s in (0, 1) and t in (0, 1),
            f"{node['id']}: compute requires s,t in 0..1")
    for key, expected in (("a_slot", f"a{s}"), ("b_slot", f"b{t}"),
                          ("c_slot", f"c{s}")):
        require(str(details.get(key, expected)).lower() == expected,
                f"{node['id']}: {key} violates fixed six-slot layout")
    return s, t


def validate_case(case: dict[str, Any]) -> dict[str, Any]:
    require(case.get("id") in CASE_IDS, "case id must be P,N,H1,H2")
    require(type(case.get("retain_B", False)) is bool, "retain_B must be boolean")
    retained = case.get("retain_B", False)
    require(retained == (case["id"] == "H1"), "only H1 uses retained B")
    nodes = case.get("nodes")
    require(isinstance(nodes, list) and nodes, "case needs frozen nodes")
    by_id: dict[str, dict[str, Any]] = {}
    for position, original in enumerate(nodes):
        require(isinstance(original, dict), "node must be an object")
        node = dict(original)
        node_id = node.get("id")
        require(isinstance(node_id, str) and node_id and node_id not in by_id,
                f"invalid/duplicate node id {node_id!r}")
        require(node.get("kind") in KINDS, f"{node_id}: unsupported kind")
        require(isinstance(node.get("predecessors"), list),
                f"{node_id}: predecessors must be explicit")
        require(all(isinstance(item, str) for item in node["predecessors"]),
                f"{node_id}: predecessor must be a node id")
        require(len(set(node["predecessors"])) == len(node["predecessors"]),
                f"{node_id}: duplicate predecessor")
        require("owner" in node, f"{node_id}: owner must be explicit")
        require(isinstance(node.get("details"), dict),
                f"{node_id}: details must be an object")
        node["position"] = position
        by_id[node_id] = node
    for node in by_id.values():
        require(all(item in by_id and item != node["id"]
                    for item in node["predecessors"]),
                f"{node['id']}: unknown/self predecessor")
    remaining = set(by_id)
    ordered: set[str] = set()
    while remaining:
        ready = {item for item in remaining
                 if set(by_id[item]["predecessors"]) <= ordered}
        require(bool(ready), "frozen DAG contains a cycle")
        ordered.update(ready)
        remaining.difference_update(ready)

    def ancestors(node_id: str) -> set[str]:
        found: set[str] = set()
        stack = list(by_id[node_id]["predecessors"])
        while stack:
            item = stack.pop()
            if item not in found:
                found.add(item)
                stack.extend(by_id[item]["predecessors"])
        return found

    required_ids = {
        "host_init", "host_reference", "map_in", "allocate_six", "K00", "K01",
        "K10", "K11", "wait_C0", "pub_C1", "done_C1", "wait_C1",
        "parallel_join", "free_six", "kernel_return", "host_compare", "free_host",
    }
    require(required_ids <= set(by_id), "missing source-lifecycle node ids")
    require("host_init" in ancestors("host_reference"), "reference before host init")
    require("host_reference" in ancestors("map_in"), "map_in omits reference prefix")
    require("map_in" in ancestors("allocate_six"), "allocation omits map_in prefix")
    require("K11" in ancestors("wait_C0"), "final C0 wait must follow final compute")
    suffix = ("wait_C0", "pub_C1", "done_C1", "wait_C1", "parallel_join",
              "free_six", "kernel_return")
    for left, right in zip(suffix, suffix[1:]):
        require(left in ancestors(right), f"mandatory suffix edge {left}->{right} absent")
    for node_id, kind in (("host_init", "join"), ("host_reference", "join"),
                          ("map_in", "map_in"), ("allocate_six", "alloc"),
                          ("parallel_join", "join"), ("free_six", "free"),
                          ("kernel_return", "join"), ("host_compare", "compare"),
                          ("free_host", "host_free")):
        require(by_id[node_id]["kind"] == kind, f"{node_id}: wrong lifecycle kind")
    singletons = {}
    for kind in ("map_in", "alloc", "free", "map_out", "visibility", "compare", "host_free"):
        matching = [node["id"] for node in by_id.values() if node["kind"] == kind]
        require(len(matching) == 1, f"need exactly one {kind} event")
        singletons[kind] = matching[0]
    for left, right in (("kernel_return", singletons["map_out"]),
                        (singletons["map_out"], singletons["visibility"]),
                        (singletons["visibility"], "host_compare"),
                        ("host_compare", "free_host")):
        require(left in ancestors(right), f"missing visibility/consumer edge {left}->{right}")

    expected_jobs = set(JOB_ORDER) - ({"B10", "B11"} if retained else set())
    publications: dict[str, str] = {}
    completions: dict[str, str] = {}
    waits: dict[str, str] = {}
    retained_checks: dict[str, str] = {}
    computes: dict[tuple[int, int], str] = {}
    for node in by_id.values():
        kind, details = node["kind"], node["details"]
        if kind == "publish":
            job, *_ = normalized_job_details(details)
            require(job in expected_jobs and job not in publications,
                    f"duplicate/unexpected publication {job}")
            publications[job] = node["id"]
        elif kind in ("dma_complete", "wait"):
            if kind == "wait" and "retained_slot" in details:
                slot = str(details["retained_slot"]).lower()
                require(retained and slot in ("b0", "b1") and
                        "job_id" not in details and slot not in retained_checks,
                        "retained checks cannot wait a nonexistent job")
                retained_checks[slot] = node["id"]
            else:
                job = details.get("job_id")
                collection = completions if kind == "dma_complete" else waits
                require(job in expected_jobs and job not in collection,
                        f"duplicate/unexpected {kind} {job!r}")
                collection[job] = node["id"]
        elif kind == "compute":
            st = compute_coordinates(node)
            require(st not in computes and node["id"] == f"K{st[0]}{st[1]}",
                    "compute identities must match source tiles")
            computes[st] = node["id"]
    require(set(publications) == set(completions) == set(waits) == expected_jobs,
            "each expected real job needs one publication, completion and wait")
    require(set(computes) == {(0, 0), (0, 1), (1, 0), (1, 1)}, "four source tiles required")
    require(set(retained_checks) == ({"b0", "b1"} if retained else set()),
            "retained B needs exactly two ownership checks")
    for job in expected_jobs:
        require(publications[job] in ancestors(completions[job]),
                f"{job}: completion lacks publication predecessor")
        require(completions[job] in ancestors(waits[job]),
                f"{job}: wait lacks actual completion predecessor")
    prefix = case.get("prefix", [])
    require(isinstance(prefix, list) and len(set(prefix)) == len(prefix) and
            all(item in by_id for item in prefix), "bad explicit prefix")
    common = ["host_init", "host_reference", "map_in", "allocate_six", "pub_A0"]
    if case["id"] in ("P", "H1"):
        require(prefix == common, "P/H1 source prefix must end after A0 publication")
    if case["id"] == "N":
        require(prefix == common + ["pub_A1"], "N must preserve A0,A1 publication prefix")
    until = case.get("prefix_until")
    if case["id"] == "H2":
        require(until == "K11" and case.get("prefix_policy") == "source_A_first",
                "H2 prefix must be naturally generated by source_A_first through K11")
    else:
        require(until is None, "only H2 uses prefix_until")
    return {"case": case, "nodes": by_id, "jobs": expected_jobs,
            "publications": publications, "completions": completions,
            "waits": waits, "retained_checks": retained_checks}


@dataclass
class Slot:
    data: bytearray = field(default_factory=lambda: bytearray([0xA5] * SLOT_BYTES))
    allocated: bool = False
    resident: str | None = None
    dma_owner: str | None = None
    uses: set[str] = field(default_factory=set)
    produced_columns: set[int] = field(default_factory=set)
    written_bytes: list[int] = field(default_factory=lambda: [0] * SLOT_BYTES)
    output_retired: bool = False


class Executor:
    def __init__(self, validated: dict[str, Any], contract: dict[str, Any],
                 policy: str, output: Path, contract_sha256: str):
        self.case = validated["case"]
        self.nodes = validated["nodes"]
        self.expected_jobs = validated["jobs"]
        self.contract = contract
        self.policy = policy
        self.output = output
        self.contract_sha256 = contract_sha256
        self.kind_rank = {kind: index for index, kind in enumerate(contract["kind_order"])}
        self.done: set[str] = set()
        self.slots = {name: Slot() for name in SLOTS}
        self.host: dict[str, bytearray] = {}
        self.external: dict[str, bytearray] = {}
        self.input_hashes: dict[str, str] = {}
        self.reference: bytearray | None = None
        self.observed_c: bytes | None = None
        self.target_written = [0] * ARRAY_BYTES
        self.jobs: dict[str, dict[str, Any]] = {}
        self.fifo: deque[str] = deque()
        self.waited: set[str] = set()
        self.retained_checked: set[str] = set()
        self.computed: set[tuple[int, int]] = set()
        self.copy_bytes = Counter()
        self.macs = 0
        self.consumer_elements = 0
        self.mapped_in = self.mapped_out = self.visible = self.compared = False
        self.host_freed = False
        self.step = self.record_number = 0
        self.phase = "prefix"
        self.prefix_end_step: int | None = None
        self.h2_debt: bool | None = None
        self.h2_pending_jobs: list[str] | None = None
        self.max_ready_publications = 0
        self.events = (output / "events.jsonl").open("x", encoding="utf-8")

    def record(self, event: str, **details: Any) -> None:
        self.record_number += 1
        row = {"record": self.record_number, "logical_step": self.step,
               "phase": self.phase, "case": self.case["id"],
               "policy": self.policy, "event": event, **details}
        self.events.write(json.dumps(row, ensure_ascii=False, sort_keys=True) + "\n")
        self.events.flush()

    def slot_event(self, operation: str, slot: str, owner: str, **details: Any) -> None:
        self.record("slot_owner", operation=operation, slot=slot, owner=owner, **details)

    def ready_nodes(self) -> list[dict[str, Any]]:
        ready = []
        for node in self.nodes.values():
            if node["id"] in self.done or not set(node["predecessors"]) <= self.done:
                continue
            if node["kind"] == "dma_complete":
                job = node["details"]["job_id"]
                if not self.fifo or self.fifo[0] != job:
                    continue
            ready.append(node)
        publications = sum(node["kind"] == "publish" for node in ready)
        self.max_ready_publications = max(self.max_ready_publications, publications)
        require(publications <= self.contract["pending_descriptor_capacity"],
                "ready publications exceed explicitly paid pending descriptor slots")
        return ready

    def publication_rank(self, node: dict[str, Any], policy: str) -> tuple[int, int]:
        job = node["details"]["job_id"]
        source_rank = JOB_ORDER.index(job)
        if policy == "source_A_first":
            return 0, source_rank
        # Frozen source information only. This does not inspect future service,
        # derive a winning schedule, or optimize against the reference result.
        return (node["details"]["first_use_rank"],
                self.contract["current_need_tie_order"].index(job))

    def choose(self, ready: list[dict[str, Any]], policy: str) -> dict[str, Any]:
        require(bool(ready), "no legal ready node; dependency/FIFO deadlock")
        return min(ready, key=lambda node: (
            self.kind_rank[node["kind"]],
            self.publication_rank(node, policy) if node["kind"] == "publish" else (0, 0),
            node["position"],
        ))

    def uses_for_job(self, job: str) -> set[str]:
        if job.startswith("A"):
            return {f"K{job[1]}0", f"K{job[1]}1"}
        if job.startswith("B"):
            return ({f"K0{job[2]}", f"K1{job[2]}"} if self.case.get("retain_B", False)
                    else {f"K{job[1]}{job[2]}"})
        return set()

    def assert_inputs_unchanged(self) -> None:
        for name in ("A", "B"):
            require(digest(self.host[name]) == self.input_hashes[name], f"host {name} changed")
            if self.mapped_in:
                require(digest(self.external[name]) == self.input_hashes[name],
                        f"mapped source {name} changed")

    def execute(self, node: dict[str, Any]) -> dict[str, Any]:
        node_id, kind, details = node["id"], node["kind"], node["details"]
        if kind == "join":
            if node_id == "host_init":
                require(not self.host, "host initialization repeated")
                self.host["A"] = pack([i * WIDTH + j for i in range(HEIGHT) for j in range(WIDTH)])
                self.host["B"] = pack([2 if i == j else 0 for i in range(HEIGHT) for j in range(WIDTH)])
                # Source memset length is width*height bytes. Remaining bytes are
                # deliberately poisoned; complete production must overwrite them.
                self.host["C"] = bytearray([0xA5] * ARRAY_BYTES)
                self.host["C"][:WIDTH * HEIGHT] = bytes(WIDTH * HEIGHT)
                self.host["D"] = bytearray([0xA5] * ARRAY_BYTES)
                self.host["D"][:WIDTH * HEIGHT] = bytes(WIDTH * HEIGHT)
                self.input_hashes = {name: digest(self.host[name]) for name in ("A", "B")}
                return {"initialized_source_bytes": 2 * ARRAY_BYTES,
                        "partial_C_memset_bytes": WIDTH * HEIGHT}
            if node_id == "host_reference":
                require(bool(self.host) and self.reference is None, "invalid host reference lifetime")
                self.reference = matrix_reference(self.host["A"], self.host["B"])
                self.host["D"][:] = self.reference
                return {"reference_MACs": 32768, "reference_bytes": ARRAY_BYTES,
                        "reference_sha256": digest(self.reference)}
            if node_id == "parallel_join":
                require(len(self.computed) == 4 and self.waited == self.expected_jobs,
                        "parallel join before all real source waits/compute")
            if node_id == "kernel_return":
                require(all(not slot.allocated for slot in self.slots.values()),
                        "kernel returned before six slots free")
            if node_id == "task_complete":
                require(self.host_freed, "task_complete before final host free")
            return {"logical_synchronization": True}
        if kind == "map_in":
            require(self.reference is not None and not self.mapped_in, "bad map_in")
            self.external = {name: bytearray(self.host[name]) for name in ("A", "B")}
            self.external["C"] = bytearray([0xA5] * ARRAY_BYTES)
            self.mapped_in = True
            return {"logical_visibility": True, "A_B_bytes_available": 2 * ARRAY_BYTES,
                    "logical_map_copy_bytes": 2 * ARRAY_BYTES,
                    "actual_runtime_bus_bytes": None, "runtime_latency": None}
        if kind == "alloc":
            require(self.mapped_in and all(not slot.allocated for slot in self.slots.values()),
                    "bad six-slot allocation")
            for name, slot in self.slots.items():
                slot.allocated = True
                self.slot_event("acquire", name, "allocation", bytes=SLOT_BYTES)
            return {"allocated_slots": list(SLOTS), "payload_storage_bytes": 6 * SLOT_BYTES}
        if kind == "publish":
            job, array, stripe, name, direction = normalized_job_details(details)
            require(job not in self.jobs, f"{job}: published twice")
            slot = self.slots[name]
            require(slot.allocated and slot.dma_owner is None, f"{job}: slot unavailable")
            if direction == "h2d":
                require(not slot.uses, f"{job}: overwrites unconsumed {sorted(slot.uses)}")
                if slot.resident is not None:
                    self.slot_event("release", name, f"resident:{slot.resident}",
                                    reason="all real compute uses consumed")
                slot.resident = None
                self.slot_event("acquire", name, job, mode="DMA_write")
            else:
                require(slot.produced_columns == {0, 1} and
                        all(count == 1 for count in slot.written_bytes),
                        f"{job}: source C stripe not completely produced")
                require(not slot.output_retired, f"{job}: C output already retired")
                self.slot_event("acquire", name, job, mode="DMA_read")
            slot.dma_owner = job
            self.jobs[job] = {"published_at_step": self.step, "completed_at_step": None,
                              "slot": name, "direction": direction,
                              "semantic_handle": f"{self.case['id']}:{len(self.jobs)+1}:{job}"}
            self.fifo.append(job)
            return {"job_id": job, "semantic_handle": self.jobs[job]["semantic_handle"],
                    "FIFO_after": list(self.fifo), "logical_descriptor_started": True,
                    "actual_hardware_job_id": None}
        if kind == "dma_complete":
            job = details["job_id"]
            require(self.fifo and self.fifo[0] == job, f"{job}: violates publication FIFO")
            require(job in self.jobs and self.jobs[job]["completed_at_step"] is None,
                    f"{job}: missing/duplicate completion")
            array, stripe, name, direction = JOB_SPECS[job]
            slot = self.slots[name]
            require(slot.allocated and slot.dma_owner == job, f"{job}: lost DMA owner")
            offset = stripe * SLOT_BYTES
            if direction == "h2d":
                require(not slot.uses, f"{job}: destination still has old users")
                data = bytes(self.external[array][offset:offset + SLOT_BYTES])
                slot.data[:] = data
                slot.uses = self.uses_for_job(job)
                slot.resident = job
                self.slot_event("write", name, job, bytes=SLOT_BYTES, sha256=digest(data))
                self.slot_event("release", name, job, mode="DMA_write")
                self.slot_event("acquire", name, f"resident:{job}", uses=sorted(slot.uses))
            else:
                data = bytes(slot.data)
                require(all(count == 0 for count in self.target_written[offset:offset + SLOT_BYTES]),
                        f"{job}: duplicate target C output bytes")
                self.external["C"][offset:offset + SLOT_BYTES] = data
                self.target_written[offset:offset + SLOT_BYTES] = [1] * SLOT_BYTES
                slot.output_retired = True
                self.slot_event("read", name, job, bytes=SLOT_BYTES, sha256=digest(data))
                self.slot_event("release", name, job, mode="DMA_read")
            slot.dma_owner = None
            self.jobs[job]["completed_at_step"] = self.step
            self.fifo.popleft()
            self.copy_bytes[direction] += SLOT_BYTES
            self.copy_bytes["total"] += SLOT_BYTES
            return {"job_id": job, "copied_bytes": SLOT_BYTES, "direction": direction,
                    "data_sha256": digest(data), "FIFO_after": list(self.fifo),
                    "AR_R_AW_W_B_counts": None, "latency": None}
        if kind == "wait":
            if "retained_slot" in details:
                name = str(details["retained_slot"]).lower()
                slot = self.slots[name]
                original_job = f"B0{name[1]}"
                require(name not in self.retained_checked and slot.allocated and
                        slot.dma_owner is None and slot.resident == original_job and
                        original_job in self.waited,
                        f"{name}: retained B lacks original valid owned receipt")
                begin = int(name[1]) * SLOT_BYTES
                require(slot.data == self.external["B"][begin:begin + SLOT_BYTES],
                        f"{name}: retained B content changed")
                self.retained_checked.add(name)
                self.slot_event("read", name, "retention_check", bytes=SLOT_BYTES,
                                semantic_check=True, actual_CPU_read_bytes=None)
                return {"retained_slot": name, "new_DMA_job": False,
                        "check_latency": None}
            job = details["job_id"]
            require(job in self.jobs and self.jobs[job]["completed_at_step"] is not None and
                    job not in self.waited, f"{job}: wait before real completion or duplicate")
            self.waited.add(job)
            return {"job_id": job, "completion_observed": True, "poll_operations": None}
        if kind == "compute":
            s, t = compute_coordinates(node)
            require((s, t) not in self.computed, "compute repeated")
            a_name, b_name, c_name = f"a{s}", f"b{t}", f"c{s}"
            a, b, c = (self.slots[name] for name in (a_name, b_name, c_name))
            a_job = f"A{s}"
            b_job = f"B0{t}" if self.case.get("retain_B", False) else f"B{s}{t}"
            require(a_job in self.waited and b_job in self.waited,
                    f"{node_id}: source DMA waits incomplete")
            if self.case.get("retain_B", False) and s == 1:
                require(b_name in self.retained_checked,
                        f"{node_id}: missing retained-owner check")
            for name, slot, resident in ((a_name, a, a_job), (b_name, b, b_job)):
                require(slot.allocated and slot.dma_owner is None and slot.resident == resident and
                        node_id in slot.uses, f"{node_id}: wrong/unavailable {name} input")
                self.slot_event("acquire", name, node_id, mode="compute_read")
                self.slot_event("read", name, node_id, logical_operand_reads=8192,
                                logical_operand_bytes=8192 * 4,
                                actual_memory_bus_bytes=None, content_sha256=digest(slot.data))
            require(c.allocated and c.dma_owner is None and t not in c.produced_columns and
                    not c.output_retired, f"{node_id}: C still owned or already produced")
            self.slot_event("acquire", c_name, node_id, mode="compute_write")
            if c.resident is None:
                self.slot_event("acquire", c_name, f"resident:C{s}",
                                reason="first real output production")
            av = struct.unpack("<512I", a.data)
            bv = struct.unpack("<512I", b.data)
            for row in range(STRIPE):
                for column in range(STRIPE):
                    total = 0
                    for inner in range(WIDTH):
                        total = (total + av[row * WIDTH + inner] *
                                 bv[column * WIDTH + inner]) & MASK
                    offset = (row * WIDTH + t * STRIPE + column) * 4
                    require(all(count == 0 for count in c.written_bytes[offset:offset + 4]),
                            f"{node_id}: duplicate compute output")
                    c.data[offset:offset + 4] = struct.pack("<I", total)
                    c.written_bytes[offset:offset + 4] = [1] * 4
            c.produced_columns.add(t)
            c.resident = f"C{s}"
            for name, slot in ((a_name, a), (b_name, b)):
                slot.uses.remove(node_id)
                self.slot_event("release", name, node_id, mode="compute_read",
                                remaining_uses=sorted(slot.uses))
            self.slot_event("write", c_name, node_id, bytes=STRIPE * STRIPE * 4,
                            column_tile=t, content_sha256=digest(c.data))
            self.slot_event("release", c_name, node_id, mode="compute_write",
                            payload_still_owned=True)
            self.computed.add((s, t))
            self.macs += STRIPE * STRIPE * WIDTH
            return {"s": s, "t": t, "MACs": 8192,
                    "C_output_elements": 256, "end_barrier": True, "compute_time": None}
        if kind == "free":
            require(len(self.computed) == 4 and not self.fifo and
                    self.waited == self.expected_jobs, "slots freed before full compute/DMA waits")
            for name, slot in self.slots.items():
                require(slot.allocated and slot.dma_owner is None and not slot.uses,
                        f"{name}: free before owner/consumer retirement")
                if name.startswith("c"):
                    require(slot.output_retired, f"{name}: output not retired before free")
                if slot.resident is not None:
                    self.slot_event("release", name, f"resident:{slot.resident}",
                                    reason="all data consumers and DMA owners retired")
                slot.resident = None
                slot.allocated = False
                self.slot_event("free", name, "allocation", bytes=SLOT_BYTES)
            return {"freed_slots": list(SLOTS)}
        if kind == "map_out":
            require(not self.mapped_out and not self.fifo and
                    all(count == 1 for count in self.target_written) and
                    all(not slot.allocated for slot in self.slots.values()),
                    "map_out before kernel storage/output closure")
            self.host["C"][:] = self.external["C"]
            self.mapped_out = True
            return {"logical_C_copy_bytes": ARRAY_BYTES,
                    "actual_runtime_bus_bytes": None, "runtime_latency": None}
        if kind == "visibility":
            require(self.mapped_out and not self.visible, "invalid visibility event")
            self.visible = True
            return {"logical_visibility": True, "WTcache_implementation": False,
                    "cache_latency": None, "CPU_instruction_time": None}
        if kind == "compare":
            require(self.visible and not self.compared and self.reference is not None,
                    "consumer before visibility/reference or repeated")
            self.assert_inputs_unchanged()
            require(self.host["D"] == self.reference, "host reference memory D changed")
            for index in range(WIDTH * HEIGHT):
                offset = index * 4
                actual = struct.unpack_from("<I", self.host["C"], offset)[0]
                expected = struct.unpack_from("<I", self.host["D"], offset)[0]
                require(actual == expected, f"C element {index}: {actual} != {expected}")
                self.consumer_elements += 1
            self.compared = True
            self.observed_c = bytes(self.host["C"])
            self.record("consumer_receipt", elements=self.consumer_elements,
                        C_logical_read_bytes=ARRAY_BYTES, D_logical_read_bytes=ARRAY_BYTES,
                        C_sha256=digest(self.observed_c), actual_CPU_time=None)
            return {"matched_elements": self.consumer_elements, "full_C_bytes": ARRAY_BYTES}
        if kind == "host_free":
            require(self.compared and self.consumer_elements == 1024 and not self.host_freed and
                    not self.fifo and all(not slot.allocated for slot in self.slots.values()),
                    "host free before final consumer or live device ownership")
            self.assert_inputs_unchanged()
            self.host_freed = True
            return {"freed_host_regions": ["A", "B", "C", "D"],
                    "semantic_release_only": True}
        raise ContractError(f"unsupported event {kind}")

    def step_node(self, node: dict[str, Any], ready: list[dict[str, Any]]) -> None:
        require(node["id"] in {item["id"] for item in ready},
                f"{node['id']}: selected node is not eligible")
        self.step += 1
        self.record("event_begin", node_id=node["id"], kind=node["kind"],
                    owner=node["owner"], predecessors=node["predecessors"],
                    details=node["details"], ready=[item["id"] for item in ready])
        result = self.execute(node)
        self.done.add(node["id"])
        self.record("event_complete", node_id=node["id"], kind=node["kind"], result=result)

    def final_checks(self) -> None:
        require(self.done == set(self.nodes), "not every frozen event executed")
        require(set(self.jobs) == self.expected_jobs and self.waited == self.expected_jobs and
                all(job["completed_at_step"] is not None for job in self.jobs.values()),
                "job publication/completion/wait conservation failed")
        expected_copies = 6 if self.case.get("retain_B", False) else 8
        require(len(self.jobs) == expected_copies and
                self.copy_bytes["total"] == expected_copies * SLOT_BYTES,
                "complete copied-byte count failed")
        require(self.copy_bytes["d2h"] == ARRAY_BYTES and
                self.copy_bytes["h2d"] == (expected_copies - 2) * SLOT_BYTES,
                "directional copied-byte count failed")
        require(self.macs == 32768 and len(self.computed) == 4, "compute coverage failed")
        require(not self.fifo and all(not slot.allocated and slot.dma_owner is None and
                                     not slot.uses for slot in self.slots.values()),
                "final slot/DMA owner not quiet")
        require(self.host_freed and self.compared and self.consumer_elements == 1024 and
                self.observed_c == bytes(self.reference or b""), "final consumer/hostfree failed")
        self.assert_inputs_unchanged()
        if self.case["id"] == "P" and self.policy == "source_current_need":
            require(self.jobs["B00"]["published_at_step"] < self.jobs["A1"]["published_at_step"],
                    "P ordinary current need failed frozen B00-before-A1 action")
        if self.case["id"] == "N":
            require(self.jobs["A1"]["published_at_step"] < self.jobs["B00"]["published_at_step"],
                    "N rewrote committed publication prefix")

    def result(self, passed: bool, error: str | None = None) -> dict[str, Any]:
        return {
            "schema": "hero-source-semantic-result-v1", "pass": passed,
            "case": self.case["id"], "policy": self.policy, "error": error,
            "input_contract_sha256": self.contract_sha256,
            "logical_steps": self.step, "event_records": self.record_number,
            "prefix_end_step": self.prefix_end_step,
            "executed_events": len(self.done), "frozen_events": len(self.nodes),
            "job_receipts": self.jobs, "source_job_waits": sorted(self.waited),
            "copied_bytes": dict(self.copy_bytes), "kernel_MACs": self.macs,
            "source_external_payload_bytes": {
                "read_from_host": self.copy_bytes["h2d"],
                "write_to_host": self.copy_bytes["d2h"],
                "actual_runtime_or_AXI_bus_bytes": None,
            },
            "logical_map_copy_bytes": {"map_in_A_B": 2 * ARRAY_BYTES if self.mapped_in else 0,
                                       "map_out_C": ARRAY_BYTES if self.mapped_out else 0},
            "host_reference_MACs": 32768 if self.reference is not None else 0,
            "consumer_elements": self.consumer_elements,
            "output_C_bytes": len(self.observed_c) if self.observed_c is not None else None,
            "output_C_sha256": digest(self.observed_c) if self.observed_c is not None else None,
            "retained_B_checks": sorted(self.retained_checked),
            "max_ready_publications": self.max_ready_publications,
            "final_slots_free": {name: not slot.allocated for name, slot in self.slots.items()},
            "host_freed_after_consumer": self.host_freed and self.compared,
            "final_publication_FIFO": list(self.fifo),
            "H2_C0_debt_at_K11": self.h2_debt,
            "H2_pending_jobs_at_K11": self.h2_pending_jobs,
            "H2_debt_subcondition_covered": self.h2_debt,
            "H2_suffix_checked": passed if self.case["id"] == "H2" else None,
            "timing": None, "CPU_time": None, "cache_latency": None,
            "AR_R_AW_W_B_counts": None, "area_Fmax": None,
            "native_DDR_or_RTL_validation": False,
            "original_HERO_SVM_execution": False,
            "performance_prediction_accepted": False,
            "meaning": "finite source semantics only; event order is not elapsed time",
        }

    def run(self) -> dict[str, Any]:
        try:
            self.record("run_begin", resource_order="publication_fifo",
                        kind_order=self.contract["kind_order"],
                        actual_hardware_identity=None, timing=None)
            for node_id in self.case.get("prefix", []):
                self.step_node(self.nodes[node_id], self.ready_nodes())
            if self.case["id"] == "P":
                require({"pub_A1", "pub_B00"} <=
                        {node["id"] for node in self.ready_nodes()},
                        "P prefix does not enable both unpublished descriptors")
                self.record("P_choice_receipt", enabled=["pub_A1", "pub_B00"],
                            both_unpublished=True, native_coexistence=None,
                            scope="source_DAG_readiness_only")
            until = self.case.get("prefix_until")
            if until:
                while until not in self.done:
                    ready = self.ready_nodes()
                    self.step_node(self.choose(ready, self.case["prefix_policy"]), ready)
                self.h2_pending_jobs = list(self.fifo)
                self.h2_debt = "C0" in self.jobs and self.jobs["C0"]["completed_at_step"] is None
                self.record("H2_prefix_receipt", through=until, C0_unretired=self.h2_debt,
                            pending_jobs=self.h2_pending_jobs, unforced=True)
            self.prefix_end_step = self.step
            self.record("prefix_complete", prefix_end_step=self.step)
            self.phase = "policy_suffix"
            while self.done != set(self.nodes):
                ready = self.ready_nodes()
                self.step_node(self.choose(ready, self.policy), ready)
            self.final_checks()
            self.record("semantic_acceptance", complete_source_semantics=True,
                        performance_or_native_acceptance=False)
            result = self.result(True)
        except (ContractError, KeyError, TypeError, ValueError, struct.error) as exc:
            self.record("execution_failure", error_type=type(exc).__name__, error=str(exc),
                        completed_nodes=sorted(self.done), pending_jobs=list(self.fifo))
            result = self.result(False, f"{type(exc).__name__}: {exc}")
            (self.output / "failure.json").write_text(json.dumps(result, indent=2) + "\n")
        finally:
            self.events.close()
        if self.observed_c is not None:
            (self.output / "output-c.bin").write_bytes(self.observed_c)
        (self.output / "result.json").write_text(json.dumps(result, indent=2) + "\n")
        return result


def load_contract(path: Path) -> tuple[dict[str, Any], list[dict[str, Any]], str]:
    raw = path.read_bytes()
    contract = json.loads(raw)
    require(contract.get("resource_order") == "publication_fifo",
            "resource_order must explicitly be publication_fifo")
    require(contract.get("pending_descriptor_capacity") == 2,
            "contract must explicitly retain two pending descriptor slots")
    order = contract.get("kind_order")
    require(isinstance(order, list) and len(order) == len(KINDS) and set(order) == set(KINDS),
            "kind_order must freeze every supported kind exactly once")
    source_input = contract.get("input", {})
    dimensions = {key: source_input.get(key)
                  for key in ("height", "width", "stripe_height", "n_stripes")}
    require(dimensions == {"height": 32, "width": 32, "stripe_height": 16, "n_stripes": 2},
            "only frozen height32,width32,stripe16,n2 dimensions supported")
    require(contract.get("policies") == list(POLICIES), "two fixed source policies required")
    tie_order = contract.get("current_need_tie_order")
    require(isinstance(tie_order, list) and len(tie_order) == len(JOB_ORDER) and
            set(tie_order) == set(JOB_ORDER), "invalid frozen current-need tie order")
    require(contract.get("payload_slots") == 6 and contract.get("payload_slot_bytes") == SLOT_BYTES
            and contract.get("added_payload_bytes") == 0, "six original slots only")
    require(contract.get("fullbuffer_native_permit_for_2048B") is False,
            "2048B has no native FullBuffer permit")
    require(contract.get("maximum_complete_semantic_episodes") == 8 and
            contract.get("maximum_timed_or_RTL_episodes") == 0 and
            contract.get("performance_gate", {}).get("admitted") is False,
            "contract must preserve source-only execution boundary")
    cases = contract.get("cases")
    require(isinstance(cases, list) and len(cases) == 4 and
            {case.get("id") for case in cases} == set(CASE_IDS), "need exactly P,N,H1,H2")
    validated = [validate_case(case) for case in cases]
    for case in validated:
        frozen = case["case"]
        copies = 6 if frozen.get("retain_B", False) else 8
        require(frozen.get("resource_order") == "publication_fifo" and
                frozen.get("expected_jobs") == copies and
                frozen.get("expected_dma_copied_bytes") == copies * SLOT_BYTES and
                frozen.get("expected_external_read_bytes") == (copies - 2) * SLOT_BYTES and
                frozen.get("expected_external_write_bytes") == ARRAY_BYTES,
                f"{frozen['id']}: source byte/job expectation mismatch")
        for node in case["nodes"].values():
            if node["kind"] == "publish":
                require(type(node["details"].get("first_use_rank")) is int and
                        node["details"]["first_use_rank"] >= 0,
                        "publication needs declared static first_use_rank")
    return contract, validated, digest(raw)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--contract", type=Path, required=True)
    parser.add_argument("--output-dir", type=Path, required=True)
    parser.add_argument("--case", action="append", choices=CASE_IDS,
                        help="Optional exact subset for a specifically authorized invocation")
    parser.add_argument("--policy", action="append", choices=POLICIES,
                        help="Optional exact subset; never an optimizer or policy search")
    args = parser.parse_args(argv)
    try:
        contract, validated, contract_hash = load_contract(args.contract)
        selected_cases = args.case or list(CASE_IDS)
        selected_policies = args.policy or list(POLICIES)
        require(len(set(selected_cases)) == len(selected_cases) and
                len(set(selected_policies)) == len(selected_policies), "duplicate selector")
        require(not args.output_dir.exists(), "output directory already exists; refusing overwrite/replay")
        args.output_dir.mkdir(parents=True)
        results = []
        for case in validated:
            case_id = case["case"]["id"]
            if case_id not in selected_cases:
                continue
            for policy in selected_policies:
                output = args.output_dir / case_id / policy
                output.mkdir(parents=True)
                executor = Executor(case, contract, policy, output, contract_hash)
                result = executor.run()
                results.append(result)
                summary = {"schema": "hero-source-semantic-summary-v1",
                           "input_contract_sha256": contract_hash,
                           "executor_sha256": digest(Path(__file__).read_bytes()),
                           "pass": all(item["pass"] for item in results),
                           "requested_runs": len(selected_cases) * len(selected_policies),
                           "executed_runs": len(results),
                           "no_automatic_retries": True, "results": results,
                           "timing": None, "native_or_performance_acceptance": False}
                (args.output_dir / "summary.json").write_text(json.dumps(summary, indent=2) + "\n")
                print(json.dumps({"case": case_id, "policy": policy,
                                  "semantic_pass": result["pass"], "timing": None}))
                if not result["pass"]:
                    return 1
        return 0
    except (ContractError, OSError, KeyError, TypeError, ValueError) as exc:
        print(json.dumps({"semantic_pass": False, "error_type": type(exc).__name__,
                          "error": str(exc), "no_execution_or_partial_execution": True}), file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
