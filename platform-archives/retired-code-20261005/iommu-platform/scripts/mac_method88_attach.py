#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""mac_method88_attach.py - one existing live boot + one fresh role-2 boot.

This is a small deployment-amendment wrapper around the delivered
``scripts/mac_method88.py`` runner.  It keeps the frozen 88-case plan and the
44 / 44 selection unchanged, but the first 44 cases ATTACH to the current
existing live role-2 boot (no JTAG, no reboot), while the second 44 cases run
after the original role-2 BIT is flashed by JTAG and a genuinely fresh boot is
waited for.  The run is therefore one existing boot plus one fresh boot, never
two fresh boots.

Load order:
  * ``MAC_METHOD88`` may hold an absolute path to the delivered
    ``mac_method88.py``; otherwise the sibling file next to this script is used.
  * The delivered runner and its siblings are loaded exactly as before; this
    file only subclasses ``Method88Runner`` and reuses its per-case capture,
    validation, sync, BusyBox archive console and 44-case verification.

No preflight or package checks are added here.  The canonical runner's own
gates remain in force.
"""
from __future__ import annotations

import argparse
import importlib.util
import json
import os
import pathlib
import sys


HERE = pathlib.Path(__file__).resolve()
ENV_MAC_METHOD88 = "MAC_METHOD88"
EXISTING_BOOT_ID = "93406a68-fc8a-4040-9dc6-ae4e4a670d36"


def _load_delivered_mac_method88():
    env_path = os.environ.get(ENV_MAC_METHOD88)
    if env_path:
        path = pathlib.Path(env_path)
    else:
        path = HERE.parent / "mac_method88.py"
    if not path.is_file():
        raise SystemExit(
            "cannot find the delivered mac_method88.py; set "
            + ENV_MAC_METHOD88 + " to its absolute path or place it next to "
            + str(HERE)
        )
    spec = importlib.util.spec_from_file_location(
        "method88_attach_delivered_mac_method88", path
    )
    if spec is None or spec.loader is None:
        raise SystemExit("cannot load " + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


mac88 = _load_delivered_mac_method88()
mac2 = mac88.mac2
method88 = mac88.method88
results2 = mac88.results2

AMENDMENT_SCHEMA = getattr(
    method88, "DEPLOYMENT_AMENDMENT_SCHEMA",
    "idma-method88-deployment-amendment-v1"
)
AMENDMENT_NOTE = getattr(
    method88, "DEPLOYMENT_AMENDMENT_NOTE",
    "第一次复用已有启动，第二次新启动；有既往462项活动，不能当成两次全新启动"
)


class Method88AttachRunner(mac88.Method88Runner):
    """Run boot 1 by attaching to the existing live role-2 boot.

    Boot 2 is delegated to the delivered Method88Runner, which flashes the same
    role-2 BIT and waits for a fresh boot.
    """

    boot_provenance = {1: "existing", 2: "fresh"}

    def _write_json(self, rel, obj):
        text = str(rel)
        if (isinstance(obj, dict) and obj.get("schema") == mac2.METADATA_SCHEMA
                and text.endswith("/metadata.json")):
            obj = dict(obj)
            provenance = self.boot_provenance.get(self.plan_boot)
            if provenance is not None:
                obj["boot_provenance"] = provenance
        if (isinstance(obj, dict) and obj.get("schema") == mac88.COMPLETE_SCHEMA
                and text == "COMPLETE.json"
                and getattr(self, "attach_amendment", None)):
            obj = dict(obj)
            obj["deployment_amendment"] = {
                "existing_firstboot": True,
                "first_boot_id": self.attach_amendment.get("first_boot_id"),
                "boots_are_two_fresh": False,
                "second_boot_fresh_required": True,
                "note": self.attach_amendment.get("note"),
            }
        super()._write_json(rel, obj)

    def _run_boot(self, boot_number, cases):
        if boot_number != 1:
            return super()._run_boot(boot_number, cases)

        self.plan_boot = 1
        image = self._image_for_role(mac88.ROLE)
        self._log(
            "boot 1: attaching to the existing live role " + str(mac88.ROLE)
            + " boot without JTAG or reboot"
        )
        # No _flash() and no flashed_roles append: this is the existing boot.
        mac2.Research2Runner._wait_for_root(self, self.args.startup_timeout)
        info = self._collect_boot_info(mac88.ROLE)
        actual_boot_id = info.get("boot_id")
        if actual_boot_id != EXISTING_BOOT_ID:
            raise mac2.TechnicalFailure(
                "existing boot_id mismatch: " + repr(actual_boot_id)
                + " != " + repr(EXISTING_BOOT_ID)
            )
        self._record_boot(1, info)
        self.boot = info
        self._write_json("method/boots/1.json", info)

        amendment = {
            "schema": AMENDMENT_SCHEMA,
            "created_at": mac2.utc_now(),
            "existing_firstboot": True,
            "first_boot_id": actual_boot_id,
            "boots_are_two_fresh": False,
            "second_boot_fresh_required": True,
            "prior_activity": "method462 completed on this boot",
            "initial_flash_skipped": True,
            "first_boot_attached_without_jtag": True,
            "first_boot_rebooted": False,
            "bit_sha256": image["sha256"],
            "same_bit_sha256": True,
            "plan_sha256": self.frozen_plan.get("plan_sha256"),
            "selection_sha256": self.frozen_plan.get("selection_sha256"),
            "cases_per_boot": mac88.CASES_PER_BOOT,
            "total_cases": mac88.TOTAL_CASES,
            "boot_provenance": {"1": "existing", "2": "fresh"},
            "note": AMENDMENT_NOTE,
        }
        self.attach_amendment = amendment
        self._write_json("method/DEPLOYMENT_AMENDMENT.json", amendment)

        for case in cases:
            self.current_case = case.get("id")
            self._run_one_case("method", mac88.ROLE, case)


def main(argv=None):
    args = mac88.build_arg_parser().parse_args(argv)
    manifest_path = pathlib.Path(args.manifest)
    package_root = (pathlib.Path(args.package_root)
                    if args.package_root else manifest_path.parent)
    if args.preflight:
        try:
            result = mac88.run_preflight(
                manifest_path, package_root, args.serial, args.manifest_sha256
            )
        except (mac2.PreflightError, mac2.ManifestError) as exc:
            print("mac_method88_attach: preflight failed: " + str(exc),
                  file=sys.stderr)
            return 2
        print(json.dumps(result, indent=2, ensure_ascii=False, sort_keys=True))
        return 0
    if not args.out:
        raise SystemExit("--out is required unless --preflight")
    out_dir = pathlib.Path(args.out)
    if out_dir.exists():
        raise SystemExit("--out already exists: " + str(out_dir))
    args.stage = "method"
    manifest = mac88.load_manifest(manifest_path, args.manifest_sha256)
    mac88.verify_package_files(manifest, package_root)
    frozen = mac88.verify_frozen(manifest, package_root)
    manifest_sha256 = mac2.sha256_file(manifest_path)
    console = mac88.Method88Console(
        args.serial, out_dir / "console.raw.log", out_dir / "console.events.log"
    )
    programmer = mac2.OpenFPGALoaderProgrammer(freq=args.jtag_freq)
    runner = Method88AttachRunner(
        args, manifest, package_root, out_dir, console, programmer,
        method88, results2,
        frozen_plan=frozen["plan"], frozen_selection=frozen["selection"],
        manifest_sha256=manifest_sha256,
    )
    return runner.run()


if __name__ == "__main__":
    sys.exit(main())
