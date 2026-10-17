# CARRV initial draft

Start with [main.pdf](main.pdf), [claims.md](claims.md), and [todo.md](todo.md). This is an English writing draft with visible placeholders, not a submission-ready manuscript. The latest located official CFP is CARRV 2024; [venue.md](venue.md) records the fallback and its ACM two-column, six-body-page limit. [self-review.md](self-review.md) gives the requested reviewer assessment. Unverified citations are listed in [to-verify.md](to-verify.md).

All files created by this writing task stay in `paper/carrv/`. No experiment, hardware build, Linux run, or production-code modification is part of this task. Replotting only reads completed result CSVs. The mainline/dead-translation results were added when that completed source directory was explicitly supplied; pending IOFENCE experiments are not presumed complete.

## Rebuild

Requirements: XeLaTeX, latexmk, BibTeX, the ACM `acmart` class and `ACM-Reference-Format.bst`, xeCJK, Noto Serif/Sans CJK SC fonts (visible draft TODOs), Poppler, and Python packages in [requirements.txt](requirements.txt). The original CSVs must remain at their repository paths. The diagram's layout is schematic; its numerical workload labels come from the existing invalidation CSV, and its topology is cited to the original RTL/README in its script and claim rows.

From the workspace root, the existing result-analysis interpreter can reproduce the artifacts without installing anything:

```sh
PAPER_PYTHON="$PWD/iommu-platform/out/invalidation-interference-20261017/runner/.venv-analysis/bin/python" \
  bash paper/carrv/scripts/build.sh
```

Alternatively, create a virtual environment inside this directory, install `requirements.txt`, and set `PAPER_PYTHON` to its Python interpreter. The build regenerates numerical macros, all figures, the table, derived CSVs, PDF, and claim/TODO indexes. It does not invoke any experimental runner. Intermediate TeX logs and rendered page checks go in ignored `build/`; publication figures and derived tables remain versioned.

The main text uses invisible `\claim{ID}` markers. The index builder checks their source mappings and expands generated number macros; this is a structural check, not a guarantee of semantic validity. Original source passages, raw result checks, and PDF rendering were also reviewed. Model work was Codex-only under the project's standing instruction; DeepSeek was not used.
