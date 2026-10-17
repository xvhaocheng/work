#!/usr/bin/env python3
"""Export one pinned source profile. Never builds or runs an experiment."""
import argparse
import hashlib
import importlib.util
import json
import subprocess
import sys
from pathlib import Path

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
PLATFORM = HERE.parents[1]


def git(repo, *args):
    return subprocess.check_output(["git", "-C", str(repo), *args])


def component_path(relative):
    path = Path(relative)
    if path.is_absolute() or ".." in path.parts:
        raise ValueError(f"Invalid component path: {relative}")
    repo = PLATFORM / path
    actual = Path(git(repo, "rev-parse", "--show-toplevel").decode().strip()).resolve()
    if actual != repo.resolve():
        raise ValueError(f"Not an independent component repository: {relative}")
    return repo


def resolve(profile):
    """Follow each selected tree's own gitlinks, including old vendor pins."""
    explicit = {c["path"]: c["commit"] for c in profile["components"]}
    pending = list(explicit.items())
    resolved = {}
    while pending:
        relative, commit = pending.pop(0)
        if relative in resolved:
            if resolved[relative]["commit"] != commit:
                raise ValueError(f"Conflicting component pin: {relative}")
            continue
        repo = component_path(relative)
        actual = git(repo, "rev-parse", f"{commit}^{{commit}}").decode().strip()
        if actual != commit:
            raise ValueError(f"Profile must use full immutable commit IDs: {relative}")
        tree = git(repo, "rev-parse", f"{commit}^{{tree}}").decode().strip()
        resolved[relative] = {"path": relative, "commit": commit, "tree": tree}
        for entry in git(repo, "ls-tree", "-r", "-z", commit).split(b"\0"):
            if not entry.startswith(b"160000 "):
                continue
            header, name = entry.split(b"\t", 1)
            child_commit = header.split()[2].decode()
            child = (Path(relative) / name.decode()).as_posix()
            if child in explicit and explicit[child] != child_commit:
                raise ValueError(f"Explicit pin disagrees with parent tree: {child}")
            pending.append((child, child_commit))
    return sorted(resolved.values(), key=lambda c: (len(Path(c["path"]).parts), c["path"]))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--profile")
    parser.add_argument("--output", type=Path)
    parser.add_argument("--check-only", action="store_true")
    parser.add_argument("--list", action="store_true")
    args = parser.parse_args()
    registry_path = HERE / "source-profiles.json"
    registry = json.loads(registry_path.read_text())
    profiles = {p["id"]: p for p in registry["profiles"]}
    if args.list:
        print("\n".join(profiles))
        return
    if args.profile not in profiles:
        parser.error("Choose --profile from: " + ", ".join(profiles))
    profile = profiles[args.profile]
    components = resolve(profile)
    result = {"schema": "organized-source-export-v1", "profile": args.profile,
              "classification": profile["classification"], "components": components,
              "source_only": True, "built_or_run": False,
              "registry_sha256": hashlib.sha256(registry_path.read_bytes()).hexdigest(),
              "export_script_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest()}
    if args.check_only:
        print(json.dumps(result, indent=2))
        return
    if args.output is None:
        parser.error("--output must be a new directory, or use --check-only")
    output = args.output.resolve()
    if output.exists():
        parser.error(f"Refusing to overwrite an existing output directory: {output}")
    # Reuse the platform's existing, archive-only staging function. This import
    # does not call its main(), pinned-build checks, Make, or hardware targets.
    spec = importlib.util.spec_from_file_location("pinned_platform_archive", PLATFORM / "scripts/platform.py")
    platform = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(platform)
    output.mkdir(parents=True)
    for component in components:
        platform.stage_repo(component, output / component["path"])
    (output / "source-export.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps({"output": str(output), "components": len(components),
                      "profile": args.profile, "source_only": True}))


if __name__ == "__main__":
    main()
