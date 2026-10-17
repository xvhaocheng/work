#!/usr/bin/env python3
"""Check source organization artifacts and preserved checkouts; no builds/runs."""
import datetime
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
PLATFORM = HERE.parents[1]
OUT = PLATFORM / "out/source-organization-20261017"


def git(repo, *args, env=None):
    return subprocess.check_output(["git", "-C", str(repo), *args], env=env)


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def load(relative):
    return json.loads((HERE / relative).read_text())


def main():
    checks = {}
    initial = load("initial-state.json")
    components = {"iommu": PLATFORM / "cva6/corev_apu/riscv-iommu",
                  "cva6": PLATFORM / "cva6", "linux": PLATFORM / "linux",
                  "opensbi": PLATFORM / "opensbi"}
    preserved = []
    for before in initial["repositories"]:
        if before["path"] not in {str(p.relative_to(ROOT)) for p in components.values()}:
            continue  # Top-level documentation is intentionally updated.
        repo = ROOT / before["path"]
        assert git(repo, "rev-parse", "HEAD").decode().strip() == before["head"]
        assert git(repo, "symbolic-ref", "--short", "HEAD").decode().strip() == before["symbolic_head"]
        now = set(git(repo, "for-each-ref", "--format=%(refname) %(objectname)",
                      "refs/heads", "refs/tags").decode().splitlines())
        assert set(before["heads_and_tags"]).issubset(now)
        assert hashlib.sha256(git(repo, "diff", "--binary", "HEAD")).hexdigest() == before["diff_sha256"]
        preserved.append(before["path"])
    checks["original_component_heads_branches_old_refs_and_diffs"] = preserved
    for name in ("iommu", "linux"):
        frozen = load(f"{name}/frozen-input.json")
        gitdir = Path(git(components[name], "rev-parse", "--absolute-git-dir").decode().strip())
        assert digest(gitdir / "index") == frozen["index_sha256"]
    assert git(components["cva6"], "ls-files", "--stage") == (HERE / "cva6/original-index.txt").read_bytes()
    assert not git(components["opensbi"], "diff", "--cached", "--binary")
    checks["original_source_component_indices_preserved"] = True

    packaged = {}
    for name in ("iommu", "cva6", "linux"):
        manifest_path = HERE / name / "artifact-sha256.json"
        artifacts = json.loads(manifest_path.read_text())
        rows = ([dict(info, path=path) for path, info in artifacts.items()]
                if isinstance(artifacts, dict) else artifacts)
        for item in rows:
            path = HERE / name / item["path"]
            assert digest(path) == item["sha256"], path
            expected_size = item.get("size", item.get("bytes"))
            if expected_size is not None:
                assert path.stat().st_size == expected_size, path
        steps = load(f"{name}/steps.json")
        if isinstance(steps, dict):
            steps = steps["steps"]
        for step in steps:
            repo = components[name]
            assert git(repo, "rev-parse", step["commit"] + "^{tree}").decode().strip() == step["tree"]
            assert git(repo, "rev-parse", step["commit"] + "^").decode().strip() == step["parent"]
            assert digest(HERE / name / step["patch"]) == step["patch_sha256"]
            if step.get("ref"):
                assert git(repo, "rev-parse", step["ref"]).decode().strip() == step["commit"]
        packaged[name] = {"artifacts_checked": len(rows), "commits_checked": len(steps)}
    checks["component_package_hashes_and_actual_git_objects"] = packaged

    # Supplementary firmware change: replay in a temporary index, then import
    # the bundle in a fresh bare repo. Never touch the original checkout/index.
    firmware = load("opensbi/manifest.json")
    repo = components["opensbi"]
    assert digest(HERE / "opensbi/source.bundle") == firmware["bundle_sha256"]
    assert digest(HERE / "opensbi/platform.c") == firmware["source"]["sha256"]
    assert hashlib.sha256(git(repo, "show", firmware["new_commit"] + ":" + firmware["source"]["path"])).hexdigest() == firmware["source"]["sha256"]
    OUT.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="firmware-check-", dir=OUT) as temp:
        temp = Path(temp)
        env = dict(os.environ, GIT_INDEX_FILE=str(temp / "index"))
        git(repo, "read-tree", firmware["base_commit"], env=env)
        git(repo, "apply", "--cached", str(HERE / "opensbi/0001-reset-registration.patch"), env=env)
        assert git(repo, "write-tree", env=env).decode().strip() == firmware["tree"]
        bare = temp / "restored.git"
        subprocess.run(["git", "init", "--bare", "--quiet", str(bare)], check=True)
        git(bare, "fetch", "--quiet", str(repo), firmware["base_commit"])
        git(bare, "fetch", "--quiet", str(HERE / "opensbi/source.bundle"), firmware["ref"])
        assert git(bare, "rev-parse", "FETCH_HEAD").decode().strip() == firmware["new_commit"]
        assert git(bare, "rev-parse", "FETCH_HEAD^{tree}").decode().strip() == firmware["tree"]
    checks["opensbi_patch_tree_and_bundle_exact_restore"] = True
    history = load("history/verification.json")
    assert history["status"] == "passed"
    assert digest(HERE / "history/index.json") == history["index_sha256"]
    assert digest(HERE / "history/core-build-sources.json") == history["core_sources_sha256"]
    checks["historical_index_matches_completed_static_receipt"] = True
    exports = load("source-export-verification.json")
    assert exports["status"] == "passed"
    checks["source_export_receipt"] = "source-export-verification.json"
    result = {"schema": "source-organization-package-verification-v1",
              "checked_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
              "status": "passed", "checks": checks,
              "no_new_builds_tests_simulations_or_experiments": True,
              "limits": ["Static identity does not establish compilation or runtime correctness.",
                         "Existing experiment outcomes are not attributed to new commits.",
                         "Top-level documentation changes are intentional; concurrent work is excluded from the commit."]}
    (HERE / "verification.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
