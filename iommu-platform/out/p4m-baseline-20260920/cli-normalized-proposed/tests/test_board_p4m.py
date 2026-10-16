#!/usr/bin/env python3
"""Host-only checks for the P4M board runner script.

These tests do not run FPGA code, DMA, HPM, or any board command. They only
validate script syntax, argument gating, and fail-stop behavior when required
commands are absent.
"""

from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / "tests" / "board-p4m.sh"


class BoardP4mScriptTest(unittest.TestCase):
    def run_script(self, *args):
        return subprocess.run(["sh", str(SCRIPT), *args], text=True,
                              capture_output=True, check=False)

    def test_shell_syntax(self):
        result = subprocess.run(["sh", "-n", str(SCRIPT)], text=True,
                                capture_output=True, check=False)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_requires_short_mode(self):
        for args in ((), ("bogus",)):
            result = self.run_script(*args)
            self.assertEqual(result.returncode, 2, result)
            self.assertIn("Usage: platform-p4m", result.stderr)

    def test_rejects_existing_output_directory(self):
        with tempfile.TemporaryDirectory() as temporary:
            out = Path(temporary) / "existing"
            out.mkdir()
            result = self.run_script("short", str(out))
            self.assertEqual(result.returncode, 2, result)
            self.assertIn("already exists", result.stderr)

    def test_missing_commands_fail_stop_and_keep_logs(self):
        with tempfile.TemporaryDirectory() as temporary:
            out = Path(temporary) / "out"
            result = self.run_script("short", str(out))
            self.assertEqual(result.returncode, 127, result)
            self.assertIn("missing command", result.stderr)
            self.assertTrue(out.is_dir())
            self.assertTrue((out / "input.txt").is_file())
            self.assertTrue((out / "matrix.txt").is_file())
            self.assertTrue((out / "taint.txt").is_file())
            matrix = (out / "matrix.txt").read_text()
            self.assertIn("Solo", matrix)
            self.assertIn("A1 gap0", matrix)
            self.assertIn("A8 gap1000", matrix)

    def test_no_full_pilot_execution(self):
        text = SCRIPT.read_text()
        self.assertIn("4096", text)
        self.assertIn("256", text)
        self.assertNotIn("run_main", text)


if __name__ == "__main__":
    unittest.main()
