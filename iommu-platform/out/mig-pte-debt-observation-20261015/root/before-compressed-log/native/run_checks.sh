#!/usr/bin/env bash
set -euo pipefail
native_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
verilator --version > "$native_dir/tool-version.txt"
verilator --lint-only -Wall --top-module mig_read_debt_observer \
  "$native_dir/mig_read_debt_observer.sv" > "$native_dir/lint.log" 2>&1
verilator --cc --exe --build --assert -Wall --top-module mig_read_debt_observer \
  -CFLAGS '-DTEST_LIVE_SLOTS=32 -DTEST_EVENT_DEPTH=1024' \
  --Mdir "$native_dir/obj_default" \
  "$native_dir/mig_read_debt_observer.sv" "$native_dir/check_native.cpp" \
  > "$native_dir/unit-build.log" 2>&1
"$native_dir/obj_default/Vmig_read_debt_observer" | tee "$native_dir/unit-result.txt"
