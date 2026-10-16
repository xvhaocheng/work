#!/usr/bin/env python3
"""Read only selected local Codex model/agent evidence; write sanitized artifacts here."""
import collections
import datetime
import hashlib
import json
import pathlib
import sqlite3

ROOT = "01a12b52-38c4-78c2-a2a4-b6c9d070a362"
CODEX_DIR = pathlib.Path("/home/xvhaocheng/.codex")
OUT = pathlib.Path(__file__).resolve().parent
VISIBLE = {
    "/root": "running",
    "/root/ai_page_lifecycle_source_20261012": "pending_init",
    "/root/model_tree_inventory_20261012": "running",
    "/root/platform_mapping_constraints_20261012": "pending_init",
}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def canonical(data):
    return json.dumps(data, ensure_ascii=False, sort_keys=True, separators=(",", ":")).encode()


def write(name, data):
    p = OUT / name
    p.write_text(json.dumps(data, ensure_ascii=False, indent=2) + "\n")
    return {"path": str(p), "sha256": digest(p.read_bytes())}


def evidence(path, line, data, selected):
    return {"file": str(path), "line": line, "timestamp": data.get("timestamp"),
            "record_sha256": digest(canonical(data)), "selected": selected}


def inspect_rollout(thread):
    p = pathlib.Path(thread["rollout_path"])
    if not p.is_file():
        return {"missing": True, "path": str(p)}
    before = p.stat()
    # A live rollout may append after inspection. Hash and cite only the bounded prefix read.
    with p.open("rb") as f:
        data = f.read(before.st_size)
    metas, contexts, windows, lifecycle = [], [], [], []
    counts, invalid = collections.Counter(), []
    last_usage, last_token = None, None
    for n, line in enumerate(data.splitlines(), 1):
        try:
            item = json.loads(line)
        except json.JSONDecodeError:
            invalid.append(n)
            continue
        kind, pay = item.get("type"), item.get("payload", {})
        counts[kind] += 1
        if not isinstance(pay, dict):
            continue
        if kind == "session_meta":
            selected = {k: pay[k] for k in ["id", "timestamp", "cwd", "originator", "cli_version",
                        "source", "thread_source", "model_provider", "parent_thread_id", "forked_from_id",
                        "agent_path", "agent_nickname", "agent_role", "context_window"] if k in pay}
            metas.append(evidence(p, n, item, selected))
        elif kind == "turn_context":
            selected = {k: pay[k] for k in ["turn_id", "model", "effort", "reasoning_effort",
                        "model_context_window", "context_window"] if k in pay}
            cm = pay.get("collaboration_mode")
            if isinstance(cm, dict):
                selected["collaboration_mode"] = {"mode": cm.get("mode")}
                settings = cm.get("settings")
                if isinstance(settings, dict):
                    selected["collaboration_mode"]["settings"] = {
                        k: settings[k] for k in ["model", "reasoning_effort"] if k in settings}
            contexts.append(evidence(p, n, item, selected))
        elif kind == "event_msg":
            typ = pay.get("type")
            if typ == "token_count" and isinstance(pay.get("info"), dict):
                last_token = evidence(p, n, item, {
                    "model_context_window": pay["info"].get("model_context_window")})
                if not windows or windows[-1]["selected"] != last_token["selected"]:
                    windows.append(last_token)
            elif typ in ["task_started", "task_complete", "turn_aborted", "shutdown_complete"]:
                lifecycle.append(evidence(p, n, item, {"type": typ, "turn_id": pay.get("turn_id")}))
        elif kind == "token_usage_record":
            # This record does not carry the server's resolved model; keep keys only plus turn id.
            last_usage = evidence(p, n, item, {
                "payload_keys": list(pay), "turn_id": pay.get("turn_id"),
                **{k: pay[k] for k in ["model", "model_name", "model_context_window", "reasoning_effort"]
                   if k in pay}})
    changes = []
    for record in contexts:
        selected = {k: v for k, v in record["selected"].items() if k != "turn_id"}
        if not changes or changes[-1]["selected"] != selected:
            changes.append({**record, "selected": selected})
    after = p.stat()
    return {
        "path": str(p), "prefix_bytes": len(data), "prefix_sha256": digest(data),
        "size_at_start": before.st_size, "size_at_end": after.st_size,
        "mtime_ns_at_start": before.st_mtime_ns, "mtime_ns_at_end": after.st_mtime_ns,
        "appended_or_changed_during_read": (before.st_size, before.st_mtime_ns) != (after.st_size, after.st_mtime_ns),
        "parse_error_lines": invalid, "record_counts": dict(counts),
        "session_metadata": metas, "turn_context_count": len(contexts),
        "latest_turn_context": contexts[-1] if contexts else None,
        "model_effort_mode_changes": changes, "context_window_changes": windows,
        "latest_token_count": last_token, "latest_token_usage_record": last_usage,
        "latest_lifecycle": lifecycle[-1] if lifecycle else None,
    }


started = datetime.datetime.now(datetime.timezone.utc).isoformat()
c = sqlite3.connect("file:" + str(CODEX_DIR / "state_5.sqlite") + "?mode=ro", uri=True)
c.row_factory = sqlite3.Row
c.execute("PRAGMA query_only=ON")
c.execute("BEGIN")
schema = [dict(r) for r in c.execute("SELECT name,sql FROM sqlite_master WHERE type='table' ORDER BY name")]
cols = ["id", "rollout_path", "created_at", "updated_at", "source", "model_provider", "cwd",
        "archived", "archived_at", "cli_version", "agent_nickname", "agent_role", "model",
        "reasoning_effort", "agent_path", "thread_source", "created_at_ms", "updated_at_ms", "originator"]
query = "WITH RECURSIVE tree(id,depth) AS (SELECT ?,0 UNION ALL SELECT e.child_thread_id,t.depth+1 " \
        "FROM thread_spawn_edges e JOIN tree t ON e.parent_thread_id=t.id) SELECT " + \
        ",".join("th."+x for x in cols) + ",tree.depth FROM threads th JOIN tree ON th.id=tree.id " \
        "ORDER BY tree.depth,th.created_at"
threads = [dict(r) for r in c.execute(query, (ROOT,))]
ids = {r["id"] for r in threads}
edges = [dict(r) for r in c.execute(
    "WITH RECURSIVE tree(id) AS (SELECT ? UNION SELECT e.child_thread_id FROM thread_spawn_edges e "
    "JOIN tree t ON e.parent_thread_id=t.id) SELECT e.* FROM thread_spawn_edges e JOIN tree t "
    "ON e.parent_thread_id=t.id ORDER BY e.parent_thread_id,e.child_thread_id", (ROOT,))]
source_only_children = []
for row in threads:
    source_children = c.execute(
        "SELECT id,agent_path,model,reasoning_effort,source FROM threads WHERE "
        "CASE WHEN json_valid(source) THEN json_extract(source,'$.subagent.thread_spawn.parent_thread_id') END=?",
        (row["id"],)).fetchall()
    source_only_children.extend(dict(r) for r in source_children if r["id"] not in ids)
db_evidence = {
    "path": str(CODEX_DIR / "state_5.sqlite"), "open_uri_mode": "ro", "query_only": 1,
    "transaction": "single read transaction for schema/tree/source-link checks", "schema": schema,
    "schema_sha256": digest(canonical(schema)), "threads": threads, "edges": edges,
    "rows_sha256": digest(canonical({"threads": threads, "edges": edges})),
    "source_linked_children_missing_from_edge_tree": source_only_children,
    "sql": {"tree_query": query, "bound_root_thread_id": ROOT},
}
c.rollback()
c.close()
db_ref = write("sqlite-tree-evidence.json", db_evidence)

rollouts = []
for thread in threads:
    record = inspect_rollout(thread)
    path = thread["agent_path"] or "/root"
    last = (record.get("latest_lifecycle") or {}).get("selected", {}).get("type")
    visible = VISIBLE.get(path)
    visibility = ("harness_running" if visible == "running" else
                  "harness_pending_init_not_yet_live_verified" if visible == "pending_init" else
                  "historical_persisted_not_visible_in_current_harness")
    rollouts.append({"thread_id": thread["id"], "agent_path": path, "depth": thread["depth"],
                     "harness_status": visible, "visibility_class": visibility,
                     "last_recorded_lifecycle_type": last, "rollout": record})
rollout_ref = write("rollout-model-evidence.json", {"root_thread_id": ROOT, "threads": rollouts})

cache_evidence = []
for name in ["models_cache.json", "models_cache.before-default-verification-20261012T054800Z.json"]:
    p = CODEX_DIR / name
    if not p.is_file():
        cache_evidence.append({"path": str(p), "missing": True})
        continue
    raw = p.read_bytes()
    data = json.loads(raw)
    sol = []
    others = []
    for m in data.get("models", []):
        if m.get("slug") == "gpt-6.1-sol":
            sol.append({k: m[k] for k in ["slug", "default_reasoning_level", "supported_reasoning_levels",
                        "context_window", "max_context_window", "effective_context_window_percent",
                        "supports_experimental_context", "supports_reasoning_effort_updates",
                        "multi_agent_reasoning_effort"] if k in m})
        else:
            others.append({k: m[k] for k in ["slug", "context_window", "max_context_window",
                           "effective_context_window_percent", "default_reasoning_level"] if k in m})
    cache_evidence.append({"path": str(p), "sha256": digest(raw),
                           "fetched_at": data.get("fetched_at"), "client_version": data.get("client_version"),
                           "sol": sol, "other_model_catalog_values_without_asserting_no_override": others})
cache_ref = write("model-cache-evidence.json", cache_evidence)

root_rollout = next(r for r in rollouts if r["thread_id"] == ROOT)["rollout"]
summary_rows = []
for thread, record in zip(threads, rollouts):
    rollout = record["rollout"]
    turn = rollout.get("latest_turn_context") or {}
    token = rollout.get("latest_token_count") or {}
    summary_rows.append({
        "thread_id": thread["id"], "agent_path": record["agent_path"], "depth": thread["depth"],
        "persisted_model": thread["model"], "persisted_effort": thread["reasoning_effort"],
        "last_turn_model": turn.get("selected", {}).get("model"),
        "last_turn_effort": turn.get("selected", {}).get("effort"),
        "last_observed_effective_context": token.get("selected", {}).get("model_context_window"),
        "harness_status": record["harness_status"], "visibility_class": record["visibility_class"],
        "last_recorded_lifecycle_type": record["last_recorded_lifecycle_type"],
        "turn_context_source": {k: turn.get(k) for k in ["file", "line", "timestamp", "record_sha256"]},
        "token_count_source": {k: token.get(k) for k in ["file", "line", "timestamp", "record_sha256"]},
    })
report = {
    "schema_version": 1, "scope": "Read-only inventory of the existing root's stored tree and local rollout model evidence",
    "root_thread_id": ROOT, "collection_started_utc": started,
    "collection_finished_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
    "actions": {"state_db_opened_read_only": True, "query_only": True, "config_modified": False,
                "database_modified": False, "rollouts_modified": False, "agents_started_by_this_worker": 0,
                "api_inference_tests": 0, "credentials_read": False,
                "research_files_read": ["/home/xvhaocheng/work/AGENTS.md", "/home/xvhaocheng/work/CURRENT_STATUS.md"]},
    "harness_snapshot": {"source": "collaboration.list_agents returned during this task before collection",
                         "agents": [{"agent_name": k, "agent_status": v} for k, v in VISIBLE.items()]},
    "tree_counts": {"all_stored_nodes": len(threads), "descendant_count": len(threads)-1,
                    "maximum_depth": max(r["depth"] for r in threads),
                    "persisted_model_counts": dict(collections.Counter(r["model"] for r in threads)),
                    "harness_visible": len(VISIBLE), "harness_running": 2, "harness_pending_init": 2,
                    "persisted_not_visible": sum(r["harness_status"] is None for r in rollouts)},
    "threads": summary_rows,
    "observations": [
        "The SQLite recursive tree has root plus 17 direct children, including this inventory worker; no deeper descendants were found.",
        "All stored edge statuses are open and all archived flags are zero. Neither field proves current execution or loaded harness state.",
        "Three historical children retain gpt-6-astra/ultra records; their latest recorded task was complete or aborted and they are absent from the current harness snapshot.",
        "Current visible root is gpt-6.1-sol/ultra in latest turn_context. The two pending_init research children have only historical Sol/ultra turn evidence; current resumed execution is unverified.",
        "Current local Sol cache context_window is 272000 with effective_context_window_percent 95; its numerical product is 258400, matching latest root and worker token_count evidence.",
        "The sampled local cache and its named backup do not establish native Sol context_window=500000 or effective=475000. max_context_window=872000 is a separate field, not proof of a 500000 default.",
        "No config, database, rollout, or cache modification was performed by this worker. No hot switch or 475000 runtime acceptance is demonstrated by these pre-change local records.",
    ],
    "unknowns": [
        "Whether a future verified catalog reload or root configuration change yields native context_window=500000 is outside this snapshot.",
        "Server-resolved model identity is not present in selected latest token_usage_record fields; turn_context establishes configured runtime model and effort, not an independent API response model attestation.",
        "Models in cache are catalog values; this worker did not inspect global/project config overrides and does not assert that other models lack overrides.",
        "The harness snapshot may omit recoverable historical children; absence is not proof of deletion or intentional archival.",
    ],
    "target_vs_observed": {"requested_policy_model": "gpt-6.1-sol", "requested_policy_effort": "ultra",
                           "requested_native_context": 500000, "requested_effective_context": 475000,
                           "observed_root_effective_context": (root_rollout.get("latest_token_count") or {}).get("selected", {}).get("model_context_window"),
                           "hot_switch_verified": False},
    "evidence_artifacts": [db_ref, rollout_ref, cache_ref],
}
report_ref = write("tree-inventory.json", report)
write("artifact-manifest.json", {"files": [db_ref, rollout_ref, cache_ref, report_ref],
      "collection_script": {"path": str(pathlib.Path(__file__).resolve()), "sha256": digest(pathlib.Path(__file__).read_bytes())}})
print(json.dumps({"report": report_ref, "tree_counts": report["tree_counts"],
                  "observed_root_effective_context": report["target_vs_observed"]["observed_root_effective_context"]}, ensure_ascii=False))
