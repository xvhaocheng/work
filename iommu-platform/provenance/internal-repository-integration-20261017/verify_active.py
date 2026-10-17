#!/usr/bin/env python3
"""Verify active internal repositories using the existing pure Git checker."""
import argparse
from contextlib import redirect_stdout
from datetime import datetime, timezone
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import subprocess
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
PLATFORM = HERE.parents[1]
ROOT = PLATFORM.parent


def git(repo, *args):
    return subprocess.check_output(["git", "--no-optional-locks", "-C", str(repo), *args])


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    assert not args.output.exists(), "Refusing to replace a previous receipt"
    spec = importlib.util.spec_from_file_location("existing_platform_checker", PLATFORM / "scripts/platform.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    captured = io.StringIO()
    with redirect_stdout(captured):
        module.check()  # No main(), manifest, build, tests or hardware action.
    assert not git(PLATFORM, "status", "--porcelain=v1", "--untracked-files=all")
    before = json.loads((HERE / "before.json").read_text())
    for old in before["repositories"]:
        repo = ROOT / old["path"]
        now = set(git(repo, "for-each-ref", "--format=%(refname) %(objectname)").decode().splitlines())
        assert set(old["all_refs"]).issubset(now), old["path"]
        assert git(repo, "remote", "-v").decode().splitlines() == old["remotes"], old["path"]
        git(repo, "merge-base", "--is-ancestor", old["head"], "HEAD")
        assert git(repo, "branch", "--show-current").decode().strip() == "codex/organized-current-20261017"
    integrated = json.loads((HERE / "component-integration.json").read_text())
    for row in integrated["components"]:
        repo = PLATFORM / row["path"]
        assert git(repo, "rev-parse", "HEAD").decode().strip() == row["active_commit"]
        git(repo, "merge-base", "--is-ancestor", row["feature_endpoint"], "HEAD")
        assert git(repo, "rev-parse", "HEAD^{tree}").decode().strip() == row["tree"]
        assert not git(repo, "status", "--porcelain=v1", "--untracked-files=all")
    sources = json.loads((HERE / "platform-source-bytes-before.json").read_text())
    for relative, expected in sources.items():
        path = PLATFORM / relative
        assert sha(path) == expected["sha256"], relative
        assert path.stat().st_mode & 0o777 == expected["mode"], relative
    retention = json.loads((HERE / "local-retention.json").read_text())
    for row in retention["files"]:
        assert sha(PLATFORM / row["path"]) == row["sha256"], row["path"]
    for row in json.loads((PLATFORM / "sources.lock.json").read_text())["components"]:
        repo = PLATFORM / row["path"]
        assert not git(repo, "status", "--porcelain=v1", "--untracked-files=all"), row["path"]
    bundles = json.loads((HERE / "component-bundles.json").read_text())
    bundle_receipt = json.loads((HERE / "component-bundle-verification.json").read_text())
    assert bundles["component_integration_sha256"] == sha(HERE / "component-integration.json")
    assert bundle_receipt["manifest_sha256"] == sha(HERE / "component-bundles.json")
    for row in bundles["components"]:
        assert sha(HERE / row["bundle"]) == row["sha256"], row["component"]
    result = {"schema": "internal-active-repository-verification-v1", "status": "passed",
              "checked_utc": datetime.now(timezone.utc).isoformat(),
              "platform_head": git(PLATFORM, "rev-parse", "HEAD").decode().strip(),
              "active_branch": "codex/organized-current-20261017",
              "existing_checker_output": captured.getvalue().strip(),
              "clean_repository_count": len(module.records()) + 1,
              "old_refs_remotes_and_ancestry_preserved": True,
              "feature_chains_in_active_ancestry": True,
              "platform_source_files_preserved": len(sources),
              "retained_file_hashes_preserved": len(retention["files"]),
              "component_bundle_heads_verified": bundle_receipt["total_heads"],
              "component_bundle_new_objects_verified": bundle_receipt["total_new_objects"],
              "sources_lock_sha256": sha(PLATFORM / "sources.lock.json"),
              "new_builds_tests_simulations_experiments_or_board_runs": 0,
              "limits": "Git/source identity only; no new runtime or original performance baseline validation."}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
