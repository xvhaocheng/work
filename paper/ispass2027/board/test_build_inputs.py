#!/usr/bin/env python3
"""Check rejected build requests stop before creating output or spawning a build."""
import copy
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

HERE = Path(__file__).resolve().parent
SCRIPT = HERE / "build_baseline.py"
SOURCE = HERE / "manifests/source-manifest.json"


class BuildInputsTest(unittest.TestCase):
    def test_frozen_source_is_accepted_without_build(self):
        spec = importlib.util.spec_from_file_location("ispass_input_guard", SCRIPT)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        selected, components = module.validate_source(SOURCE)
        self.assertEqual(selected["selected_cva6"], module.PINNED_CVA6)
        self.assertEqual(components["cva6/corev_apu/riscv-iommu"], module.IOMMU)

    def test_bad_requests_have_no_output_side_effect(self):
        original = json.loads(SOURCE.read_text())
        with tempfile.TemporaryDirectory(prefix="ispass-build-inputs-") as name:
            temp = Path(name)
            cases = [("missing-software-source", ["software"]),
                     ("missing-fpga-source", ["fpga", "--variant", "ds0"]),
                     ("wrong-variant", ["fpga", "--variant", "other", "--source", str(SOURCE)]),
                     ("missing-variant", ["fpga", "--source", str(SOURCE)]),
                     ("zero-jobs", ["software", "--source", str(SOURCE), "--jobs", "0"]),
                     ("negative-jobs", ["fpga", "--variant", "ds0", "--source", str(SOURCE), "--jobs", "-1"]),
                     ("source-is-directory", ["software", "--source", str(temp)])]
            for field in ["linux", "opensbi", "iommu", "selected_cva6", "selected_platform"]:
                obj = copy.deepcopy(original)
                obj[field] = "0" * 40
                path = temp / (field + ".json")
                path.write_text(json.dumps(obj))
                cases.append(("wrong-" + field, ["software", "--source", str(path)]))
            foreign = copy.deepcopy(original)
            for c in foreign["components"]:
                if c["path"] == "cva6/corev_apu/riscv-iommu":
                    c["commit"] = "0" * 40
            path = temp / "foreign-child.json"
            path.write_text(json.dumps(foreign))
            cases.append(("foreign-fpga-child", ["fpga", "--variant", "ds0", "--source", str(path)]))
            malformed = copy.deepcopy(original)
            malformed["components"][0]["path"] = ["cva6"]
            path = temp / "malformed-component.json"
            path.write_text(json.dumps(malformed))
            cases.append(("malformed-component", ["software", "--source", str(path)]))
            for label, args in cases:
                with self.subTest(case=label):
                    out = temp / (label + "-out")
                    p = subprocess.run([sys.executable, str(SCRIPT), *args, "--out", str(out)],
                                       capture_output=True, text=True, timeout=10)
                    self.assertEqual(p.returncode, 2, p.stdout + p.stderr)
                    self.assertFalse(out.exists(), label)
            print("PASS 14 rejected requests before output creation; no build invocation")


if __name__ == "__main__":
    unittest.main()
