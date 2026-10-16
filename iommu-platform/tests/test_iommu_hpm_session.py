#!/usr/bin/env python3
"""Run production HPM session functions against host-controlled MMIO/locks.

Only kernel includes are removed; the production function bodies are compiled
unchanged. This checks driver control logic, not RTL behavior or board counters.
"""

import hashlib
import os
from pathlib import Path
import re
import resource
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[1]


def without_includes(text):
    return re.sub(r"^\s*#include[^\n]*", "", text, flags=re.MULTILINE)


def main():
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    source = ROOT / "linux/drivers/iommu/riscv/iommu-hpm.c"
    contents = source.read_text()
    with tempfile.TemporaryDirectory(prefix="iommu-hpm-session-") as temporary:
        temp = Path(temporary)
        (temp / "production_hpm.c").write_text(without_includes(contents))
        (temp / "production_hpm_api.h").write_text(without_includes(
            (ROOT / "linux/include/linux/riscv_iommu_hpm.h").read_text()))
        # Use the driver's register definitions without unrelated queue structs.
        definitions = []
        for line in (ROOT / "linux/drivers/iommu/riscv/iommu-bits.h").read_text().splitlines():
            if re.match(r"#define\s+RISCV_IOMMU_", line) and (
                    "HPM" in line or "IOCOUNT" in line or
                    re.match(r"#define\s+RISCV_IOMMU_REG_CAP\s", line) or
                    re.match(r"#define\s+RISCV_IOMMU_EVENT_(TLB_MISS|S1_PTW|S2_PTW)\s", line)):
                assert not line.endswith("\\"), "update extraction for continued register macro"
                definitions.append(line)
        (temp / "production_hpm_bits.h").write_text("\n".join(definitions) + "\n")
        binary = temp / "hpm-session-test"
        subprocess.run([
            os.environ.get("HOSTCC", "cc"), "-std=gnu11", "-O2", "-g", "-Wall", "-Wextra",
            "-Werror", "-I", str(temp), str(ROOT / "tests/test_iommu_hpm_session.c"),
            "-o", str(binary),
        ], check=True)
        subprocess.run([str(binary)], check=True)
    print(f"Production iommu-hpm.c SHA256: {hashlib.sha256(contents.encode()).hexdigest()}")
    print("Host control-logic checks only; no RTL/HPM event/FPGA calibration result.")


if __name__ == "__main__":
    main()
