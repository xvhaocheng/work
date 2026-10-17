#!/usr/bin/env bash
set -euo pipefail
analysis_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
analysis_python="${ANALYSIS_PYTHON:-$analysis_dir/.venv/bin/python}"
if [[ ! -x "$analysis_python" ]]; then
  analysis_python=python3
fi
"$analysis_python" -c 'import matplotlib, numpy' || {
  echo 'Install analysis/requirements.txt in analysis/.venv, then rerun build.sh.' >&2
  exit 1
}
"$analysis_python" "$analysis_dir/scripts/test_analysis.py"
if [[ -f "$analysis_dir/scripts/baseline_extra.py" ]]; then
  "$analysis_python" "$analysis_dir/scripts/baseline_extra.py"
fi
"$analysis_python" "$analysis_dir/scripts/merge_results.py" "$@"
"$analysis_python" "$analysis_dir/scripts/plot_figures.py"
"$analysis_python" "$analysis_dir/scripts/write_figure_notes.py"
"$analysis_python" "$analysis_dir/scripts/validate_outputs.py"
