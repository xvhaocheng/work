#!/usr/bin/env python3
"""Read-only verification of the saved frozen-source RTL audit.

This checks source bytes, excerpts and recorded build identities. It does not
run Git, a build, a simulation or board commands, and does not validate measured
behavior. Historical Git-blob SHA claims are compared with the frozen bytes;
the commit IDs are cross-checked against the saved manifests, not Git objects.
"""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import sys


class VerificationError(Exception):
    """A required source or recorded identity could not be verified."""


def require(condition: bool, message: str) -> None:
    if not condition:
        raise VerificationError(message)


def digest(raw: bytes) -> str:
    return hashlib.sha256(raw).hexdigest()


class Verifier:
    def __init__(self, source_root: Path, audit: dict) -> None:
        self.root = source_root.resolve()
        self.audit = audit
        self.cache: dict[str, bytes] = {}
        self.manifests: dict[str, dict] = {}
        self.counts = {
            "evidence_entries": 0,
            "reference_excerpts": 0,
            "rtl_identity_checks": 0,
            "software_checks": 0,
            "source_manifest_checks": 0,
            "build_manifest_checks": 0,
        }

    def read(self, relative: str) -> bytes:
        path = Path(relative)
        require(not path.is_absolute() and ".." not in path.parts,
                f"source path must be relative to --source-root: {relative}")
        if relative not in self.cache:
            full = self.root / path
            try:
                self.cache[relative] = full.read_bytes()
            except FileNotFoundError as exc:
                raise VerificationError(
                    f"missing required evidence/frozen source: {full}; "
                    "supply --source-root containing the original manifests "
                    "and frozen build trees"
                ) from exc
        return self.cache[relative]

    def document(self, relative: str) -> dict:
        if relative not in self.manifests:
            self.manifests[relative] = json.loads(self.read(relative))
        return self.manifests[relative]

    def file_hash(self, relative: str, expected: str) -> bytes:
        raw = self.read(relative)
        actual = digest(raw)
        require(actual == expected,
                f"SHA256 mismatch: {relative}: expected {expected}, got {actual}")
        return raw

    def reference(self, ref: dict, *, compile_excerpt: bool = False) -> None:
        raw = self.file_hash(ref["path"], ref["file_sha256_now"])
        lines = raw.decode("utf-8").splitlines()
        first, last = ref["line"], ref["end_line"]
        require(1 <= first <= last <= len(lines),
                f"invalid cited line range: {ref['path']}:{first}-{last}")
        cited = "\n".join(lines[first - 1:last])
        if compile_excerpt:
            # This one audit entry intentionally saves the two selected path
            # tokens from a long read_verilog line, not a contiguous excerpt.
            require(first == last and cited.startswith("## read_verilog -sv"),
                    f"expected a read_verilog source-list line: {ref['path']}:{first}")
            selected = " ".join(
                token for token in cited.split()
                if token.endswith(("/stream_delay.sv", "/axi_delayer.sv"))
            )
            require(selected == ref["excerpt"],
                    f"selected compile-path excerpt mismatch: {ref['path']}:{first}")
        else:
            require(cited == ref["excerpt"],
                    f"source excerpt mismatch: {ref['path']}:{first}-{last}")
        self.counts["reference_excerpts"] += 1

    def build_manifest(self, relative: str) -> dict:
        manifest = self.document(relative)
        if not manifest.get("_rtl_audit_checked"):
            require(manifest["source_manifest_sha256"] ==
                    self.audit["source_manifest_sha256_now"],
                    f"source-manifest identity mismatch: {relative}")
            for field, component in (("selected_cva6", "cva6"),
                                     ("selected_iommu", "iommu")):
                require(manifest[field] == self.audit["commits"][component],
                        f"{field} mismatch: {relative}")
            # This marker is in memory only; no manifest is written.
            manifest["_rtl_audit_checked"] = True
            self.counts["build_manifest_checks"] += 1
        return manifest

    def rtl_identity(self, item: dict, source_manifest: dict) -> None:
        raw = self.file_hash(item["path"], item["sha256_now"])
        self.reference(item["manifest_ref"])
        require(item["manifest_ref"]["path"] == item["manifest"],
                f"manifest citation points elsewhere: {item['path']}")
        manifest = self.build_manifest(item["manifest"])
        key = item["path"].split("/src/", 1)[1]
        require(manifest["sources"].get(key) == item["sha256_now"] and
                item["matches_manifest"] is True,
                f"build sources identity mismatch: {item['path']}")
        require(manifest["variant"] == item["variant"],
                f"build variant mismatch: {item['path']}")
        component = "iommu" if "/riscv-iommu/" in key else "cva6"
        require(item["commit"] == self.audit["commits"][component],
                f"recorded commit mismatch: {item['path']}")
        if item["origin"] == "exact_selected_commit_blob":
            require(digest(raw) == item["git_blob_sha256"],
                    f"recorded blob SHA differs from frozen bytes: {item['path']}")
        elif item["origin"] == "selected_commit_plus_three_DS_localparam_overlay_only":
            self.top_overlay(item, raw, manifest, source_manifest)
        else:
            raise VerificationError(f"unrecognized source origin: {item['origin']}")
        self.counts["rtl_identity_checks"] += 1

    def top_overlay(self, item: dict, raw: bytes, manifest: dict,
                    source_manifest: dict) -> None:
        key = "corev_apu/fpga/src/ariane_xilinx.sv"
        require(item["git_blob_sha256"] ==
                source_manifest["measurement_overlay_files"][key],
                f"top base-blob SHA differs from source manifest: {item['path']}")
        bases = [entry for entry in self.audit["identity_checks"]
                 if entry["variant"] == "ds0" and entry["path"].endswith("/" + key)]
        require(len(bases) == 1, "expected one DS0 top-level reference")
        baseline = self.file_hash(bases[0]["path"], item["git_blob_sha256"])
        pattern = (r"(localparam (?:bit|int unsigned) "
                   r"IOMMUDS(?:DelayEnable|FixedDelayInput|FixedDelayOutput)\s*=\s*)\d+")
        normalize = lambda text: re.sub(pattern, r"\g<1>0", text)
        text = raw.decode("utf-8")
        require(normalize(text) == normalize(baseline.decode("utf-8")),
                f"top differs beyond the three DS parameters: {item['path']}")
        parameters = manifest["parameters"]
        expected = {
            "DelayEnable": int(parameters["ds_delay_enable"]),
            "FixedDelayInput": parameters["ds_cycles"][0],
            "FixedDelayOutput": parameters["ds_cycles"][1],
        }
        for suffix, value in expected.items():
            found = re.findall(r"localparam (?:bit|int unsigned) IOMMUDS" +
                               suffix + r"\s*=\s*(\d+)\s*;", text)
            require(found == [str(value)],
                    f"DS parameter mismatch ({suffix}): {item['path']}")

    def software(self, item: dict) -> None:
        raw = self.file_hash(item["path"], item["sha256_now"])
        kernel = "/src/linux/" in item["path"]
        component = "linux" if kernel else "platform_software_inputs"
        require(item["commit"] == self.audit["commits"][component],
                f"software commit mismatch: {item['path']}")
        if "git_blob_sha256" in item:
            require(digest(raw) == item["git_blob_sha256"],
                    f"recorded software blob SHA mismatch: {item['path']}")
        ref = item["manifest_ref"]
        if ref is not None:
            self.reference(ref)
            manifest = self.document(ref["path"])
            section = "kernel_critical_sources" if kernel else "handwritten_inputs"
            key = item["path"].split("/src/linux/" if kernel else "/inputs/", 1)[1]
            require(manifest[section].get(key) == item["sha256_now"] and
                    item["matches_manifest"] is True,
                    f"software build-source mismatch: {item['path']}")
            field = "selected_linux" if kernel else "selected_platform_software_inputs"
            require(manifest[field] == item["commit"],
                    f"software build commit mismatch: {item['path']}")
            require(manifest["source_manifest_sha256"] ==
                    self.audit["source_manifest_sha256_now"],
                    f"software source-manifest mismatch: {item['path']}")
        else:
            require(item.get("matches_selected_commit") is True,
                    f"unexplained software entry without a manifest reference: {item['path']}")
        self.counts["software_checks"] += 1

    def run(self) -> dict:
        require(self.audit["schema"] == "interference-model-rtl-audit-v1",
                "unsupported RTL audit schema")
        self.file_hash(self.audit["source_manifest"],
                       self.audit["source_manifest_sha256_now"])
        source = self.document(self.audit["source_manifest"])
        for field, component in (("selected_cva6", "cva6"), ("iommu", "iommu"),
                                 ("linux", "linux")):
            require(source[field] == self.audit["commits"][component],
                    f"source-manifest commit mismatch: {field}")
        self.counts["source_manifest_checks"] += 1
        ids: set[str] = set()
        for evidence in self.audit["evidence"]:
            require(evidence["id"] not in ids, f"duplicate evidence ID: {evidence['id']}")
            ids.add(evidence["id"])
            require(bool(evidence["refs"]), f"no references: {evidence['id']}")
            for ref in evidence["refs"]:
                self.reference(ref, compile_excerpt=evidence["id"] == "COMPILE-resolve")
            self.counts["evidence_entries"] += 1
        for item in self.audit["identity_checks"]:
            self.rtl_identity(item, source)
        for item in self.audit["software_checks"]:
            self.software(item)
        return {
            "status": "passed",
            "scope": "saved source-byte, excerpt and build-manifest consistency only",
            **self.counts,
            "distinct_files_hashed": len(self.cache),
            "git_objects_reopened": False,
            "build_simulation_or_board_validation": False,
            "limitations": [
                "Recorded commit IDs and blob SHA claims are checked against saved "
                "manifests/frozen bytes; Git objects are not independently reopened.",
                "Unlisted Linux headers have current-byte hashes and saved commit "
                "claims, but no separate historical build-manifest entry.",
                "This does not prove historical loaded BIT identity, dynamic "
                "per-PTE latency, measured behavior or model fit accuracy.",
            ],
        }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source-root", required=True, type=Path,
                        help="repository root containing original manifests and frozen build trees")
    parser.add_argument("--audit", type=Path,
                        default=Path(__file__).resolve().parents[1] / "interference-model-rtl.json")
    args = parser.parse_args()
    try:
        audit = json.loads(args.audit.read_bytes())
        result = Verifier(args.source_root, audit).run()
    except (VerificationError, OSError, ValueError, KeyError, IndexError, TypeError) as exc:
        print(json.dumps({"status": "failed", "error": str(exc)}, ensure_ascii=False),
              file=sys.stderr)
        return 1
    print(json.dumps(result, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
