# Anonymous manuscript build and visual check

2026-10-17 JST. **The current working draft compiles to 8 pages including references.** It is a reviewable draft with explicit experiment/declaration gaps, not a submission-ready certificate. No hardware operation, external model call, upload or publication was performed by the manuscript task.

## Outputs and configuration

- Source: [`main.tex`](main.tex), [`sections/`](sections/), [`references.bib`](references.bib).
- Readable PDF: [`main.pdf`](main.pdf), 315070 bytes; SHA256 `826fcac934583211872bacf468a7a568fe96b9ad85cafa9eea39b8e5e4cff843`.
- Source/claim joins: [`claims.md`](claims.md), [`claim-intent.json`](claim-intent.json), [`claim-check.json`](claim-check.json), generated [`numbers-provenance.json`](numbers-provenance.json).
- Citation scope and unavailable lookups: [`citation-audit.md`](citation-audit.md).
- Minimal release and anonymous-preparation plan: [`../artifact/plan.md`](../artifact/plan.md).

The document uses `IEEEtran` conference mode, 10pt, US Letter, two columns, `Anonymous Authors`, and visible page numbers 1–8. PDF Author metadata is empty. All fonts reported by `pdffonts` are embedded Type1 or TrueType; no Type3 font is present. The English abstract is a working placeholder and must be reconciled with the final experiment set.

## Actual checks

`make` ran the local claim checker, then `latexmk -pdf -interaction=nonstopmode -halt-on-error main.tex`. Installed tools were latexmk 4.76 and pdfTeX 1.40.22 / TeX Live 2022 (Debian). Compilation exited successfully. The final log has **no undefined citation/reference, no rerun request and no overfull box**. It retains two underfull horizontal boxes and two underfull vertical boxes; the rendered pages show ordinary justification/float whitespace without clipping or overlap.

Poppler rendered all eight final pages at 100dpi. The full contact sheet and individual figure-dense pages 4/6 and reference/declaration page 8 were visually inspected. Labels, captions, footers, column boundaries and DOI wrapping are legible; no clipped chart or overlapping element was observed. Review PNGs, extracted text, layout XML and command logs remain local under ignored paths.

Each figure is included at its actual 3.5in column width with **no maximum-height shrink**. The figure pipeline checked all ten figure PDFs using `pdftohtml -xml -zoom1`; [`../analysis/build-validation.json`](../analysis/build-validation.json) records an actual minimum rendered font size of **8.0pt** for every figure. Scientific tick labels use plain strings to avoid smaller mathematical superscripts. Thus the manuscript does not achieve its page count by shrinking the tall figures 2/8.

The local checker passed with **49 registered claim IDs, eight cited bibliography entries and 14 numerical macros used in prose**. All used numerical macros have generated derivations and source operands; none renders as a pending numerical value. This verifies the declared joins, not semantic support or completeness of claim extraction. Citation placement is before sentence punctuation, and available DOI identifiers are printed as well as retained in BibTeX.

The ARS deterministic acronym check read the saved draft body with complete coverage and no findings after applying the documented domain/name/unit allowlist. The allowlist and report are saved as [`acronym-allowlist.txt`](acronym-allowlist.txt) and [`acronym-report.md`](acronym-report.md). The source-only draft snapshot remains ignored and local. This is a body-only advisory, not an abstract or scientific-integrity verdict.

Extracted PDF text contains no local account path, identifying development-repository URL or unresolved numerical placeholder. Names in ordinary third-person prior-work citations remain, as required for anonymous scholarship. Funding, author contributions and conflict-of-interest facts are visibly pending; no author-owned facts are invented.

## Page budget and remaining work

The requested allocation is retained below. Floats cross section boundaries, so these are planning budgets rather than a false claim of exact per-section pagination.

| Content | Requested pages | Current status |
| --- | ---: | --- |
| Introduction | 1.00 | Drafted; implementation-specific contribution and direct precedents |
| Background | 0.75 | Drafted; IOMMU/IOTLB/PTW/DMA and distinct delay mechanisms |
| Platform and methodology | 1.50 | Drafted; actual baseline, supply, hardware/software windows and statistics |
| Results | 4.50 | DS0/DS50 baseline, E3/E2 history, historical mitigation and two separate simulation supplements; incomplete matrices remain visible |
| Discussion, validity, conclusion and declarations | 0.75 | Drafted with author-owned declaration gaps |
| Related work | 0.50 | All seven mandatory related works discussed in third person |
| Total body budget | **9.00** | Current whole PDF is 8 pages including references; unused budget is not filled with padding |

Figure 3 currently contains DS0/DS50 anchors, with DS25/DS100 pending. Figures 4–7 explicitly lack active-device scaling, independent workload/size, verified page-size and request-distribution data. Figure 8 is historical-only and not a repaired-baseline mitigation rerun. Figure 9 uses the completed hash-bound 12-cell E7 **RTL simulation**; figure 10 is the separate invalidation **simulation**. No derived stage view is presented as a CDF.

E7 now reports capacity-sensitive disappearance of the A8 increment while the A1 co-run difference remains. Its ordinary MMIO/single-COPY supply, different warm-up, arrival-order memory service (16-cycle head startup then one 8-byte beat/cycle), absent CPU/Linux execution and trend-only calibration are explicit. Historical FPGA E2 residency/removal evidence is described separately. The residual is not split into controller blocking and memory competition from the capacity matrix alone.

The baseline distinguishes victim elapsed slowdown from the aggregate write-response payload proxy. A1's victim share is not described as a halving of system efficiency. HPM uses non-atomic software windows, per-request path means are not tails or additive elapsed shares, DS50 is not pure PTW cost, and DS0 high-pressure victim miss counts are not relabeled zero.

## Citation and workflow limits

ARS academic-paper drafting, citation and formatting guidance was used for this scoped stage; the user-supplied target, configuration and outline were reused. No full research pipeline or redundant approval interview was started. The seven required works are cited: Bermuda, DATE2025 SVA, Rodríguez2024, Procedia2025, Hur/Ro2025, HERO2017 and Kurth2018. The updated prior-work report and original publisher-PDF identities replace the obsolete missing-full-text status for Procedia and Hur/Ro.

This writer directly inspected the publisher title pages, Procedia §5.3, Hur/Ro's method/result sections and the thesis title pages; saved prior Codex full-reading reports provide the remaining stated technical scope. No claim is made that this writer reread every source page. Conference-version metadata is kept distinct from the technically read author manuscripts. Some publisher/DOI/ETH/institution endpoints were unavailable in this run, with already-verified metadata and original PDFs used as documented. Retraction-database and plagiarism-service checks were not performed. No field-wide priority, first-study, complete causal decomposition or universal performance claim is made.

The present task was performed by Codex. Historical experiment authorship, including earlier DeepSeek involvement documented in original reports, remains separate from this Codex-only manuscript task. The manuscript's final AI-use and author declarations still require author verification before submission. Source/data release is promised only upon acceptance; no content was externally published.
