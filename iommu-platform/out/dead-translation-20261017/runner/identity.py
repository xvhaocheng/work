#!/usr/bin/env python3
"""Record bounded, non-secret experiment inputs before compiler or runner use."""
from __future__ import annotations

import datetime as dt
import hashlib
import json
import os
from pathlib import Path
import subprocess

HERE = Path(__file__).resolve().parent
TASK = HERE.parent
PLATFORM = TASK.parents[1]
WORK = PLATFORM.parent


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def utc() -> str:
    return dt.datetime.now(dt.timezone.utc).isoformat()


def git(repo: Path, *args: str) -> str:
    return subprocess.check_output(
        ["git", "-C", str(repo), *args],
        env=dict(os.environ, GIT_OPTIONAL_LOCKS="0"), timeout=180,
    ).decode("utf-8", errors="replace")


def record(output: Path, phase: str, consumed_inputs: list[Path]) -> dict:
    """Save relevant diffs only; enumerate unrelated tracked dirty paths, not bytes."""
    output = output.resolve()
    if not output.is_relative_to(TASK) or output.exists():
        raise ValueError("identity directory must be new and strictly within this task")
    output.mkdir(parents=True)
    frozen_path = TASK / "frozen/source-manifest.json"
    frozen = json.loads(frozen_path.read_text())
    repositories = []
    roots = [("work", WORK), ("platform", PLATFORM), ("cva6", PLATFORM / "cva6"),
             ("iommu", PLATFORM / "cva6/corev_apu/riscv-iommu"),
             ("linux", PLATFORM / "linux")]
    for name, repo in roots:
        relevant = []
        if name in ("work", "platform"):
            prefix = TASK.relative_to(repo).as_posix()
            relevant = [prefix + "/runner", prefix + "/frozen", prefix + "/frozen-copy.json"]
        tracked_status = git(repo, "status", "--short", "--untracked-files=no")
        row = dict(name=name, path=str(repo), checkout_branch=git(repo, "branch", "--show-current").strip() or None,
                   checkout_commit=git(repo, "rev-parse", "HEAD").strip(),
                   tracked_status=tracked_status.splitlines(),
                   tracked_dirty=bool(tracked_status.strip()), relevant_paths=relevant,
                   unrelated_dirty_bytes_captured=False,
                   untracked_scope="task source paths only; unrelated untracked files deliberately not enumerated")
        if relevant:
            row["relevant_untracked_files"] = git(repo, "ls-files", "--others", "--exclude-standard", "--", *relevant).splitlines()
            for label, args in [("unstaged", ["diff", "--binary", "--"]),
                                ("staged", ["diff", "--cached", "--binary", "--"])]:
                patch = output / f"{name}-{label}.patch"
                patch.write_text(git(repo, *args, *relevant))
                row[label + "_patch"] = dict(path=str(patch), sha256=sha256(patch), bytes=patch.stat().st_size)
        else:
            row["relevant_untracked_files"] = []
            row["dirty_diff_note"] = "No active checkout files are compiler inputs; frozen sources and task overlays are captured separately."
        if name == "platform":
            row["checkout_child_gitlinks"] = git(repo, "ls-tree", "HEAD", "cva6", "linux", "opensbi").splitlines()
        if name == "cva6":
            row["checkout_child_gitlink"] = git(repo, "ls-tree", "HEAD", "corev_apu/riscv-iommu").splitlines()
            row["selected_ref"] = "pinned platform-corrected CVA6 commit"
            row["selected_commit"] = frozen["cva6"]
            row["selected_child_gitlink"] = git(repo, "ls-tree", frozen["cva6"], "corev_apu/riscv-iommu").splitlines()
        if name == "iommu":
            row["selected_ref"] = "pinned platform-corrected IOMMU F5 commit"
            row["selected_commit"] = frozen["iommu"]
        if name == "linux":
            row["role"] = "source reference only; no CPU/Linux execution or compilation"
            row["selected_commit"] = row["checkout_commit"]
        repositories.append(row)
    inputs = [dict(path=str(p.resolve()), sha256=sha256(p), bytes=p.stat().st_size)
              for p in sorted(set(consumed_inputs + [frozen_path]))]
    lock = PLATFORM / "sources.lock.json"
    payload = dict(schema="dead-translation-identity-v1", phase=phase, recorded_utc=utc(),
                   repositories=repositories, consumed_inputs=inputs,
                   sources_lock=dict(path=str(lock), sha256=sha256(lock),
                                     note="checkout lock is provenance only; actual compile closure is frozen/source-manifest.json"),
                   actual_baseline={key: frozen[key] for key in ("cva6", "iommu", "register_interface", "axi_slice")},
                   actual_source_note="Independent frozen corrected baseline plus explicit task-local handwritten inputs and declared overlays. Active research checkout is not compiled.",
                   native_cpu=False, fpga=False)
    path = output / "identity.json"
    path.write_text(json.dumps(payload, indent=2, ensure_ascii=False) + "\n")
    return dict(path=str(path), sha256=sha256(path))
