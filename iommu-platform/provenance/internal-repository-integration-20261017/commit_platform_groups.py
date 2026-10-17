#!/usr/bin/env python3
"""One-time commit of inventoried platform work in dependency order; no builds."""
import hashlib
import json
from pathlib import Path
import subprocess

HERE = Path(__file__).resolve().parent
PLATFORM = HERE.parents[1]
BRANCH = "codex/organized-current-20261017"


def git(*args):
    return subprocess.check_output(["git", "--no-optional-locks", "-C", str(PLATFORM), *args])


def main():
    inventory = json.loads((HERE / "platform-inventory.json").read_text())
    receipt = HERE / "platform-commits.json"
    assert not receipt.exists(), "Do not repeat platform commits"
    assert git("rev-parse", "HEAD").decode().strip() == inventory["repository_HEAD"]
    assert not git("diff", "--cached", "--binary")
    sources = {}
    for group in inventory["functional_source_groups"]:
        for relative in group["paths"]:
            path = PLATFORM / relative
            assert path.is_file() and not path.is_symlink(), relative
            sources[relative] = {"sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
                                 "mode": path.stat().st_mode & 0o777}
    (HERE / "platform-source-bytes-before.json").write_text(json.dumps(sources, indent=2) + "\n")
    git("switch", "--quiet", "-c", BRANCH)
    groups = [dict(id="COMPONENT-PINS", paths=["cva6", "linux", "opensbi", "sources.lock.json"],
                   title="Pin integrated component histories and synchronize source lock")]
    for group in inventory["functional_source_groups"]:
        groups.append(dict(id=group["group_id"], paths=group["paths"],
                           title={
                               "S01_IOMMU_FIX_REGRESSIONS": "Record targeted DS ownership and PTW leaf regression sources",
                               "S02_P4M_SUPPLY_OBSERVER": "Record P4M supply observation and recovery tools",
                               "S03_NONBLOCK_COMPOSITION_RTL": "Record banked admission and P N composition RTL fixtures",
                               "S04_COMPOSITION_HOST_TOOLS": "Record composition profiles, software and host tools",
                               "S05_ACTUAL_DMA_M0": "Record actual DMA and complete COPY fixtures",
                               "S06_A2_SVA_USER_TASK": "Record fixed SVA user task and input fixture",
                               "S07_BOARD_NETWORK_UPDATE": "Record board network and software update workflow",
                               "S08_BUILD_ROOTFS_INTEGRATION": "Record shared platform and rootfs integration",
                           }[group["group_id"]]))
    for group_id, title in [
        ("D_HISTORY_MIGRATION", "Preserve historical plan relocation and revised archive targets"),
        ("D_RESEARCH_REPORTS", "Record existing research reports and bounded conclusions"),
        ("D_ENGINEERING_NAVIGATION", "Record engineering navigation and preserved validation artifacts"),
        ("P_SOURCE_ORGANIZATION_PACKAGE", "Record original baselines, feature stacks and historical source evidence"),
    ]:
        paths = list(inventory["groups"][group_id]["paths"])
        if group_id == "D_ENGINEERING_NAVIGATION":
            paths.extend([".gitignore", "tight_setup_hold_pins.txt"])
        groups.append(dict(id=group_id, paths=paths, title=title))
    results = []
    for group in groups:
        assert not git("diff", "--cached", "--binary")
        git("add", "--", *group["paths"])
        staged = git("diff", "--cached", "--no-renames", "--name-only", "-z").split(b"\0")
        staged = [x.decode() for x in staged if x]
        assert set(staged) == set(group["paths"]), (group["id"], staged)
        git("commit", "--quiet", "-m", group["title"])
        results.append({"group": group["id"], "commit": git("rev-parse", "HEAD").decode().strip(),
                        "paths": group["paths"], "source_only": True,
                        "independent_build_or_runtime_verified": False})
        receipt.write_text(json.dumps(results, indent=2) + "\n")
        print(json.dumps({"group": group["id"], "commit": results[-1]["commit"], "paths": len(staged)}), flush=True)
    for relative, expected in sources.items():
        path = PLATFORM / relative
        assert hashlib.sha256(path.read_bytes()).hexdigest() == expected["sha256"], relative
        assert path.stat().st_mode & 0o777 == expected["mode"], relative


if __name__ == "__main__":
    main()
