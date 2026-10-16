#!/usr/bin/env python3
"""Use the delivered runner with this board's normal root login."""
import importlib.util
import re
import sys
import time
from pathlib import Path

runner_path = Path(__file__).resolve().parents[1] / 'artifacts/software/research-two-bit-20261003/delivery/scripts/mac_research2.py'
spec = importlib.util.spec_from_file_location('delivered_mac_research2', runner_path)
module = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = module
spec.loader.exec_module(module)

class LoggedInRunner(module.Research2Runner):
    def _flash(self, image):
        self.console.drain_input()
        return super()._flash(image)

    def _wait_for_root(self, timeout):
        deadline = time.monotonic() + timeout
        cursor = 0
        attempts = 0
        fresh_boot = False
        while time.monotonic() < deadline:
            with self.console._cond:
                data = bytes(self.console._buffer)
                recent = data[cursor:]
                if b'Linux version ' in data or b'login:' in data:
                    fresh_boot = True
                if fresh_boot and re.search(rb'(?m)^#\s*$', recent):
                    break
                prompt = re.search(rb'(?:login:|Password:)\s*$', recent)
                if prompt:
                    attempts += 1
                    if attempts > 6:
                        raise module.TechnicalFailure('root login failed')
                    cursor = len(data)
                else:
                    self.console._cond.wait(timeout=min(1.0, max(0.01, deadline-time.monotonic())))
                    continue
            self.console._write(b'root\r')
        else:
            raise module.TechnicalFailure('root login startup timeout')
        remaining = deadline - time.monotonic()
        if remaining <= 0:
            raise module.TechnicalFailure('root readiness startup timeout')
        return super()._wait_for_root(remaining)

module.Research2Runner = LoggedInRunner
if __name__ == '__main__':
    raise SystemExit(module.main())
