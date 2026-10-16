#!/bin/sh
# Builds the natural write-order experiment with the command stored next to it.
set -eu
cd /home/xvhaocheng/work/iommu-platform
python3 - <<'PY'
import json, subprocess, sys
argv = json.load(open("out/natural-write-order-20261008/build-natural-command.json"))
print("+ " + " ".join(argv), file=sys.stderr)
sys.exit(subprocess.call(argv))
PY
