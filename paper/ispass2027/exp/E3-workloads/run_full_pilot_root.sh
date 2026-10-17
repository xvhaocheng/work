#!/bin/bash
# Root only. No load/write operation; collects with an already verified full-E3 BIT.
set -euo pipefail
: "${E3_LOAD_MANIFEST:?Set the actual verified full-E3 load manifest}"
: "${E3_RUN_OUTPUT:?Set a new collection output directory}"
: "${E3_SETUP_SECONDS:?Set actual full-E3 BIT/load/boot setup seconds}"
E3_COLLECTION_SECONDS=$(python3 - "$E3_SETUP_SECONDS" <<'PY'
import math,sys
value=float(sys.argv[1])
if not math.isfinite(value) or not 0<=value<21470:
    raise SystemExit('actual finite setup seconds must leave more than130s within21600s')
print(21600-value)
PY
)
E3_PAPER_ROOT=/home/xvhaocheng/work-root-a/paper/ispass2027
E3_COLLECT_ARGS=(--max-cases 30)
if [[ -n "${E3_RESUME_RUN:-}" ]]; then
    # Root decides whether150 fits after inspecting the real30-point pilot.
    E3_COLLECT_ARGS=(--max-cases 150 --resume-run "$E3_RESUME_RUN")
fi
exec python3 "$E3_PAPER_ROOT/exp/E3-workloads/collect_workload.py" \
  --output "$E3_RUN_OUTPUT" --board-tools "$E3_PAPER_ROOT/board" \
  --load-manifest "$E3_LOAD_MANIFEST" \
  --fpga-manifest /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/full-e3-ds0-resume01/build-manifest.json \
  --software-manifest "$E3_PAPER_ROOT/exp/E6-mitigation/software-source/build01/build-manifest.json" \
  --p0-backend "$E3_PAPER_ROOT/exp/E3-workloads/backend-validation/runs/run03/summary.json" \
  --long-backend "$E3_PAPER_ROOT/exp/E3-workloads/test-results/long-backend04-summary.json" \
  --packet-backend "$E3_PAPER_ROOT/exp/E3-workloads/test-results/packet-backend01-summary.json" \
  --expected-kernel 6.5.0-rc2-ispass2027-g-v1 \
  --max-wall-seconds "$E3_COLLECTION_SECONDS" "${E3_COLLECT_ARGS[@]}"
