#!/usr/bin/env bash
set -euo pipefail
paper_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
cd -- "$paper_dir"
export PYTHONDONTWRITEBYTECODE=1
export MPLCONFIGDIR="$paper_dir/.cache/matplotlib"
paper_python="${PAPER_PYTHON:-python3}"
mkdir -p build
"$paper_python" scripts/prepare_data.py
"$paper_python" scripts/draw_platform.py
"$paper_python" scripts/plot_invalidation.py
"$paper_python" scripts/plot_plru.py
"$paper_python" scripts/plot_mainline.py
latexmk -xelatex -interaction=nonstopmode -halt-on-error -outdir=build main.tex >build/compile.stdout 2>&1
cp -- build/main.pdf main.pdf
"$paper_python" scripts/build_indexes.py
pdfinfo main.pdf | sed -n '/^Pages:/p'
