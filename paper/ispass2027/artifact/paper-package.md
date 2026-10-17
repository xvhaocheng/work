# Anonymous paper-source package

2026-10-17 JST. The local source archive was independently unpacked and compiled successfully to **8 pages**. This preparation does not publish the manuscript or its research artifact.

## Accepted local products

Attempt: `artifact/packages/paper-source-20261017-02/` (ignored, local only).

| File | Bytes | SHA256 |
| --- | ---: | --- |
| `paper-source.tar.gz` | 241735 | `46ffd18dd94db8310d0feb77e780b786a82a577f3fa0647262229a531d2465cd` |
| `compiled.pdf` (derived) | 314972 | `0def1b22d971e4f78f6fb17aa001bc378b1f1d9cf6151e426b35697cb9adea36` |

`validation.json` beside the archive records all input hashes, the actual independent extraction directory, commands, tool versions, TeX input closure, PDF metadata and rendering disposition. Logs and review PNGs remain in its named local temporary directory, outside the archive and Git.

## Source identity and transformation

Source commit at snapshot: `afb6a3b287ec53493d375b0eb2e73aea170b6428`. Builder SHA256: `6f1045f6407f61941e9c9cf971ae37b5f9eba423aae092c7bdd5171fcd0bb3d1`.
The original `tex/main.pdf` remains 315070 bytes, SHA256 `826fcac934583211872bacf468a7a568fe96b9ad85cafa9eea39b8e5e4cff843`. The derived PDF above is a separate artifact and does not replace that identity.

The builder copies only `main.tex`, the seven section sources, `references.bib`, `numbers.tex`, ten generated figure PDFs, unmodified IEEEtran class/style and the system LPPL text. It adds a package-local README, Makefile, third-party notice and neutral manifest. The only change to a derived manuscript source is replacing the two `../analysis/figures/` path prefixes with `figures/`; the original TeX files are not edited.

The archive contains 27 regular files. Tar owner/group names and IDs and modification times are neutral/fixed, so they reveal no local account. The package manifest uses only relative file names and SHA256 values; source commit and private source paths stay in the external validation report. All selected inputs, including the original manuscript PDF, remained byte-identical across preparation.

Internal claims, raw paths, source/citation audits, experiment records, paid publisher PDFs, compilation intermediates/logs, rendered review images and virtual environments are excluded. Invisible no-op claim IDs already in the manuscript remain usable without the private claim register. Normal third-person bibliography citations are retained.

## Build and verification

Preparation command from the workspace root:

```sh
python3 paper/ispass2027/artifact/build_paper_bundle.py \
  --output paper/ispass2027/artifact/packages/paper-source-20261017-02
```

The script creates a new local temporary extraction and actually runs `make` there. The bundled recipe uses latexmk with `pdflatex -no-shell-escape`, nonstop interaction and halt-on-error. It used the bundled `IEEEtran.cls` and `IEEEtran.bst`; all 104 external recorded TeX inputs were within the checked system TeX/font paths, with no dependency on the development workspace. TeX search-path environment overrides were removed for this check. No Python environment or raw experiment source is required for the package build.

| Tool | Observed version |
| --- | --- |
| latexmk | Latexmk, John Collins, 20 November 2021. Version 4.76 |
| pdflatex | pdfTeX 3.141592653-2.6-1.40.22 (TeX Live 2022/dev/Debian) |
| bibtex | BibTeX 0.99d (TeX Live 2022/dev/Debian) |
| make | GNU Make 4.3 |

The package README lists GNU Make, latexmk, pdfLaTeX, BibTeX, the standard LaTeX packages and Times/Nimbus Roman/AMS fonts. On Debian/Ubuntu it identifies the corresponding TeX Live package groups. The bundled IEEEtran files are unmodified and retain their notices; the LPPL text and source-distribution links are included.

Validation found no unresolved reference/citation, rerun request or overfull box. The pre-existing two underfull horizontal and two underfull vertical boxes remain ordinary justification/float whitespace. The compiled document is US Letter, two columns, 10pt, with `Anonymous Authors`, empty PDF Author metadata and page numbers 1–8. Text/metadata scans found no development-account path or numerical pending placeholder.

Poppler rendered all eight pages at 100 dpi. Codex inspected the full contact sheet, the tall-chart page 6 and reference/declaration page 8, with no clipping or overlap observed. Images are included at their original column width, preserving the figure pipeline's checked minimum 8pt text. The source archive was identical in two local preparation runs; derived PDF bytes may differ with compilation metadata and TeX versions.

An attempted reuse of the final output directory exited 2 before any write; every existing output file retained its hash. Choose a new explicit output path for another attempt. The script also restricts output to `artifact/packages/` and does not touch board, kernel, FPGA or repository branches.

## Remaining work and scope

The working abstract, DS25/DS100 gap, active-device matrix, independent workload/size tests, verified page-size comparison, request-latency distribution and repaired-baseline mitigation rerun remain incomplete. Author contributions, funding and conflict-of-interest facts remain visibly pending. Capacity and invalidation results remain separate RTL simulations; no new experiment or scientific validation is implied by successful TeX compilation.

The package is a portable anonymous manuscript compilation source, not a complete data artifact or a submission-ready claim. Publication remains deferred until acceptance. No upload, external model call or hardware operation occurred in this packaging task. Source changes and readable reporting are for Root to commit; local archives/PDFs/logs are excluded from routine Git synchronization.

## Input content hashes

Original input hashes below are copied from the actual snapshot report. `main.tex` is the original input hash; the neutral archive manifest separately hashes its relative-image-path derivative.

| Input | SHA256 |
| --- | --- |
| `main.tex` | `d2017e9510912b220271a0b2010430d4d1522e06e62f90cab808fa2bca0467f5` |
| `references.bib` | `a1c4112036379946614dfd7f119ca3266f754e584a2cf467c6f13337de016ee3` |
| `numbers.tex` | `09e8262295d87e593ca136598f04f7f25a201d238da3fe1d60303049ad72482c` |
| `sections/introduction.tex` | `fcffd295bf0987b8fa3ecfe66a86df7b30f244e3c464c692f73d6e60057586c2` |
| `sections/background.tex` | `cb11abd04acb57bd4dbf3e2565faeb3621a2debd8e2ba7c7ca083cc5a9d4e1fe` |
| `sections/methodology.tex` | `128eac95d126af87c436c0db9c5021561bea73f8d386dbece29c6d71e2fd3d5c` |
| `sections/results.tex` | `5244086159e1d166baf604dec997bf165a461b815a7f5de234d782ad40a4ea1c` |
| `sections/discussion.tex` | `50a836d6a63194417be0afb91b42d7a0dd49ae2470699986147f5b7419342c49` |
| `sections/related.tex` | `eda90b089534d3f8e03288f167a1a9202dda033e2de457e9f7e6a67c4695dce3` |
| `sections/conclusion.tex` | `bd63a5cbb210d273bf097d64badfa9f0c8d13442c10254ca3672033b1284896e` |
| `figures/fig01-throughput.pdf` | `94bed5baf2ca3895581e32ea6c35243f069cc0e876ff31016817f9306c60ea68` |
| `figures/fig02-path-miss.pdf` | `983b8cabde5e76e53adff7bb8902a03bc4eb71dd98c6519bc82dea1f25ddedf9` |
| `figures/fig03-ds-sweep.pdf` | `2b755089bd45e5a593181d230d0a36a590530f415df906dae6a9085fc6a5b9f9` |
| `figures/fig04-device-count.pdf` | `13114925d521f91c9ffb2aafad791fcd4f69253fbddf68bbbc602002999d0ae9` |
| `figures/fig05-workload-size.pdf` | `a12d21d0d48b5117b30ed076516d68059fbbff0ae9654fe7889fb8b4b7cac8d7` |
| `figures/fig06-page-size.pdf` | `4fa3a0fba9517ab27d85bdb69fdfcb686acf2da3275269ad275a413f5d37e399` |
| `figures/fig07-latency-cdf.pdf` | `2fa85598823c70f25a9cbaf5a216bbab2b51505a868a5c930ee5a7575ca4e59c` |
| `figures/fig08-mitigation-history.pdf` | `37d71519e22fb9afa93113846ba55853bebaffbb629b959e3863d0077f6de32e` |
| `figures/fig09-capacity.pdf` | `51ad68eac55711aa4b8eede44c3cb0383d42a50bf49defa2f01b44ea5c9812b0` |
| `figures/fig10-invalidation.pdf` | `ff30764eebabac7ad2b1aaa8ceaca4f233c5115577501879e76054975be19249` |
| `IEEEtran.cls` | `da751920a317ed318b7b5cd7fa585a6cc7d28502d457856382e9be24b10a3bd7` |
| `IEEEtran.bst` | `314f0ece704568faf827011bac498650691b2b5ee06320720830e782416d5a5f` |
| `LPPL.txt (system compressed source)` | `814ca1a01ad5f8a2fbec2b19d9381e30012d36944ebccb24e49eb1850ce8afa5` |
