#!/usr/bin/env bash
set -euo pipefail
artifact_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cd "$artifact_root"
common=source/hero-tools/sw/hero-driver/common
# Same actual production C for both binaries; only OS/hardware calls are faked.
gcc -std=gnu11 -O1 -g -Wall -Wextra -Wno-unused-parameter \
  -fsanitize=address,undefined -fno-omit-frame-pointer \
  -Itests/kernel-stubs -I"$common" "$common/hero_iommu.c" \
  tests/hero_iommu_faults.c -o evidence/hero-iommu-faults
timeout 20 env ASAN_OPTIONS=detect_leaks=1:handle_segv=0 evidence/hero-iommu-faults
gcc -std=gnu11 -O1 -g -Wall -Wextra -Wno-unused-parameter \
  -fsanitize=address,undefined -fno-omit-frame-pointer -DTEST_DEFAULT_GATES \
  -Itests/kernel-stubs -I"$common" "$common/hero_iommu.c" \
  "$common/hero_platform_gate.c" tests/hero_iommu_faults.c \
  -o evidence/hero-iommu-default-gate
timeout 20 env ASAN_OPTIONS=detect_leaks=1:handle_segv=0 evidence/hero-iommu-default-gate
