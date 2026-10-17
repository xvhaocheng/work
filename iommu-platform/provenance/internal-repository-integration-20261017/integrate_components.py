#!/usr/bin/env python3
"""One-time source-preserving integration of the already organized Git chains.

This records existing working files, merges existing feature histories and pins
the integrated child. It never builds, runs experiments, resets or deletes files.
The before-state guards intentionally prevent an unreviewed repeat execution.
"""
import hashlib
import json
from pathlib import Path
import subprocess

HERE = Path(__file__).resolve().parent
PLATFORM = HERE.parents[1]
ROOT = PLATFORM.parent
BRANCH = "codex/organized-current-20261017"
TARGETS = {
    "iommu": ("cva6/corev_apu/riscv-iommu", "0b7939445298a23ea3e131ffd4e003cebb15ac59"),
    "linux": ("linux", "32b96fe373e56611c9eee787580c1e475d7b52e9"),
    "opensbi": ("opensbi", "9ed1f82340fe0db0352f0a14ec1caada5aef0c2a"),
    "cva6": ("cva6", "991074e7fca9eddd33c61d72767c3729f573dc3e"),
}


def git(repo, *args):
    return subprocess.check_output(["git", "--no-optional-locks", "-C", str(repo), *args])


def oid(repo, rev):
    return git(repo, "rev-parse", rev).decode().strip()


def identity(path):
    if path.is_symlink():
        return ("symlink", str(path.readlink()))
    return ("regular", hashlib.sha256(path.read_bytes()).hexdigest(), path.stat().st_mode & 0o777)


def main():
    result_path = HERE / "component-integration.json"
    if result_path.exists():
        raise RuntimeError("Integration receipt already exists; do not repeat mutations")
    before = {r["path"]: r for r in json.loads((HERE / "before.json").read_text())["repositories"]}
    results = []
    # Guard every repository before mutating any repository.
    for name, (relative, target) in TARGETS.items():
        repo = PLATFORM / relative
        original = before[str(repo.relative_to(ROOT))]
        assert oid(repo, "HEAD") == original["head"], name
        assert git(repo, "branch", "--show-current").decode().strip() == original["branch"], name
        assert not git(repo, "diff", "--cached", "--binary"), name
        assert BRANCH not in git(repo, "branch", "--format=%(refname:short)").decode().splitlines(), name
        assert oid(repo, target + "^{tree}"), name
    for name, (relative, target) in TARGETS.items():
        repo = PLATFORM / relative
        original = before[str(repo.relative_to(ROOT))]
        changed = [p.decode() for p in git(repo, "diff", "--name-only", "-z", original["head"], target).split(b"\0") if p]
        regular = [p for p in changed if p != "corev_apu/riscv-iommu" or name != "cva6"]
        fingerprints = {p: identity(repo / p) for p in regular}
        git(repo, "switch", "--quiet", "-c", BRANCH)
        git(repo, "add", "--", *regular)
        if name == "cva6":
            # First capture exactly the reviewed feature tip. The child checkout
            # is already integrated and has the same source; bind that new
            # history explicitly in a separate commit below.
            git(repo, "update-index", "--cacheinfo", "160000,0b7939445298a23ea3e131ffd4e003cebb15ac59,corev_apu/riscv-iommu")
        expected_tree = oid(repo, target + "^{tree}")
        assert oid(repo, "HEAD") == original["head"]
        assert git(repo, "write-tree").decode().strip() == expected_tree, name
        git(repo, "commit", "--quiet", "-m", f"Capture existing {name} source before integrating organized history")
        capture = oid(repo, "HEAD")
        assert oid(repo, "HEAD^{tree}") == expected_tree
        git(repo, "merge", "--quiet", "--no-ff", "-m", f"Integrate function-separated {name} history with identical source", target)
        merge = oid(repo, "HEAD")
        assert oid(repo, "HEAD^{tree}") == expected_tree, name
        git(repo, "merge-base", "--is-ancestor", original["head"], "HEAD")
        git(repo, "merge-base", "--is-ancestor", target, "HEAD")
        if name == "cva6":
            child = PLATFORM / "cva6/corev_apu/riscv-iommu"
            child_head = oid(child, "HEAD")
            git(repo, "add", "--", "corev_apu/riscv-iommu")
            git(repo, "commit", "--quiet", "-m", "Pin integrated IOMMU history without changing RTL source")
            pin = git(repo, "ls-tree", "HEAD", "--", "corev_apu/riscv-iommu").decode()
            assert pin.startswith("160000 commit " + child_head + "\t")
            delta = git(repo, "diff", "--name-only", target, "HEAD").decode().splitlines()
            assert delta == ["corev_apu/riscv-iommu"]
        for path, fingerprint in fingerprints.items():
            assert identity(repo / path) == fingerprint, (name, path)
        assert not git(repo, "status", "--porcelain=v1", "--untracked-files=all"), name
        refs = set(git(repo, "for-each-ref", "--format=%(refname) %(objectname)").decode().splitlines())
        assert set(original["all_refs"]).issubset(refs), name
        assert git(repo, "remote", "-v").decode().splitlines() == original["remotes"], name
        results.append({"component": name, "path": relative, "original_head": original["head"],
                        "original_branch_preserved": original["branch"], "active_branch": BRANCH,
                        "feature_endpoint": target, "capture_commit": capture, "merge_commit": merge,
                        "active_commit": oid(repo, "HEAD"), "tree": oid(repo, "HEAD^{tree}"),
                        "changed_ordinary_files_checked": len(fingerprints), "source_bytes_modes_preserved": True,
                        "old_refs_and_remotes_preserved": True, "status": "clean"})
        (HERE / "component-progress.json").write_text(json.dumps(results, indent=2) + "\n")
        print(json.dumps(results[-1]), flush=True)
    result_path.write_text(json.dumps({"schema": "active-component-integration-v1", "components": results,
                                       "no_source_behavior_changes": True, "builds_or_experiments_run": 0}, indent=2) + "\n")


if __name__ == "__main__":
    main()
