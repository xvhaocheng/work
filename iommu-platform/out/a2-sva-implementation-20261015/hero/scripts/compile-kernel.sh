#!/usr/bin/env bash
set -euo pipefail
artifact_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
kernel_src="$artifact_root/source/linux-e4b0725a5089f6cc0fc5afec06f3091d98dffa68"
kernel_build="$artifact_root/build/linux-e4b0725"
driver_src="$artifact_root/source/hero-tools/sw/hero-driver/carfield"
mkdir -p "$kernel_build" "$artifact_root/logs"
# Compile profile only: retain pinned kernel config, clear external initramfs,
# disable force-unload and debug-symbol generation. No other kernel is accepted.
python3 - "$kernel_src/Makefile" <<'CHECK'
import sys
from pathlib import Path
s = Path(sys.argv[1]).read_text().splitlines()
v = {l.split('=')[0].strip(): l.split('=')[1].strip() for l in s[:8] if '=' in l}
assert [v[k] for k in ('VERSION','PATCHLEVEL','SUBLEVEL')] == ['6','9','0'], v
CHECK
cp "$artifact_root/config/linux64_defconfig_iommu" "$kernel_build/.config"
"$kernel_src/scripts/config" --file "$kernel_build/.config" \
  --set-str INITRAMFS_SOURCE "" --disable MODULE_FORCE_UNLOAD \
  --disable DEBUG_INFO --enable DEBUG_INFO_NONE
make -C "$kernel_src" O="$kernel_build" ARCH=riscv CROSS_COMPILE=riscv64-linux-gnu- olddefconfig
cp "$kernel_build/.config" "$artifact_root/config/linux-6.9.compile-resolved.config"
make -C "$kernel_src" O="$kernel_build" ARCH=riscv CROSS_COMPILE=riscv64-linux-gnu- -j8 modules_prepare
# Compile real changed units and link their relocatable module object. A full
# .ko/modpost additionally requires the exact kernel's Module.symvers/vmlinux.
make -C "$kernel_src" O="$kernel_build" ARCH=riscv CROSS_COMPILE=riscv64-linux-gnu- \
  M="$driver_src" HERO_PLATFORM=1 -j4 carfield.o
cp "$driver_src/carfield.o" "$artifact_root/evidence/carfield-linux-6.9.o"
