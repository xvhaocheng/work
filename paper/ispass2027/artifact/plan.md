# Minimal artifact and anonymous-release plan

Status: local preparation and independent build checks, 2026-10-17 JST. The paper-source package and all three E7 capacity configurations now compile from new extraction directories; the data-only analysis package reproduces the accepted statistics and figures. See the [prepared-artifact index](prepared-artifacts.md), [paper-package.md](paper-package.md), [analysis-package.md](analysis-package.md) and [e7-package.md](e7-package.md). The selected-input closure for numerical replay is documented in [replay-input-audit.md](replay-input-audit.md). Archives, binaries and logs stay under ignored local `packages/`; builders and readable validation reports are synchronized to the task branch. No review artifact is uploaded or submitted. The manuscript promises release **upon acceptance**. Existing public development history is not an anonymous artifact link.

The smallest useful artifact has two entry points: replay the accepted measurements into tables/figures/PDF without hardware, and optionally rebuild/run the documented FPGA configuration. The first is the mandatory reproducibility path; the second requires the board, licensed FPGA tools and the existing explicit board workflow. A successful offline replay is not a repeated board measurement.

## Minimum file set

| Package | Include | Why / boundary |
| --- | --- | --- |
| Anonymous paper | `tex/main.tex`, `tex/sections/`, `tex/references.bib`, `tex/numbers.tex`, figure PDFs, short build instructions | Compilable 10pt IEEE double-column draft; exclude internal claim paths and author-identifying metadata from the review upload. |
| Analysis entry | `analysis/build.sh`, pinned `requirements.txt`, `analysis/scripts/`, `baseline100-extra.json`, `inputs/baseline100-source-audit.json`, `history-number-inputs.json` | Deterministic statistics and actual source-counter semantics, including byte-proxy limits; source-export identities retain original commits and lines. |
| Provenance | `data/source_manifest.json`, stable row IDs/locators, `data/board_imports/`, `data/incremental_sources.json`, baseline package `source.json` and used build/run manifests; `analysis/figure-trace.json`, `figure-statistics.json`, `numbers-provenance.json`, `merge-manifest.json` | Every figure point and numerical macro joins back to an accepted source. Row counts are not trial counts; base IDs and evidence levels stay explicit. |
| Accepted measurement inputs | The exact raw `records.json`, per-run manifests/load receipts and source-counter audit used by the baseline calculation; the canonical historical long table, selected history snapshots and accepted validation summaries | Numerical replay needs no UART log or full `out/` tree. Keep the original complete evidence locally, including failures and recovery chains; never overwrite it. The compact replay package reproduces curated numerical inputs, rather than rerunning historical extraction from all raw sources. |
| Simulation supplement | `data/invalidation_sim_results.csv` plus its selected-input manifests and raw summaries; accepted `exp/E7-capacity-sim/` source/commands/results/import receipt | Invalidation and capacity have separate simulation base IDs. Behavioral memory and MMIO supply are disclosed; no FPGA time conversion. Pending configurations are excluded until accepted. |
| Fixed executable sources | Necessary platform adaptation/fix patches; fixed Git commits/bundles/locks from baseline package metadata; matching driver/UAPI, autonomous frontend and observer sources | Do not substitute the current research checkout or a branch name for the actual experiment inputs. Preserve component provenance and licenses. |
| Optional board assets | Locally archived actual Linux payload and DS0/DS50 BIT files, associated delivery manifests, tool/version instructions and board skill | Binary images remain outside Git under the established artifact-storage rule. Match the existing manifests; do not introduce another identity framework. |
| Internal audit only | `tex/claims.md`, `claim-intent.json`, `citation-audit.md`, `check_claims.py`, build/visual-check report | Keep full provenance internally. Before an anonymous artifact is supplied, create a separate scrubbed copy with stable anonymous locators; never alter the original evidence to anonymize it. |

Do not redistribute the user-supplied publisher PDFs with the artifact. Keep their DOI/version/hash and exact claim locators; readers can obtain the originals through the publisher or institutional author versions. The manuscript's ordinary third-person citations remain intact.

## Offline replay

Run from the artifact root after restoring the manifest-listed selected inputs. The compact numerical closure contains 17 data files and 10 control files before the package entry, notices and provenance mappings. It reproduces the current statistics, tables and figures without all historical `out/` inputs; it is not a complete raw-data release or a hardware reproduction. Derived path/identity redactions must be explicit and separately hash-bound to the untouched original inputs.

```sh
python3 -m venv paper/ispass2027/analysis/.venv
paper/ispass2027/analysis/.venv/bin/pip install -r paper/ispass2027/analysis/requirements.txt
python3 paper/ispass2027/analysis/scripts/baseline_extra.py --check
bash paper/ispass2027/analysis/build.sh
make -C paper/ispass2027/tex check
make -C paper/ispass2027/tex
```

Expected outputs are the reviewed baseline statistics, ten figure slots with explicit missing-data status where applicable, stable trace/number provenance and a compilable PDF. `build.sh` validates its analysis inputs and refuses incompatible bases; it must not silently replace missing measurements with synthetic values. Exact PDF bytes may vary by LaTeX/font versions; numeric outputs, stable IDs, source hashes and declared tolerances are the acceptance surface. Rendering must still be inspected for layout defects.

The data extractor's `--check` additionally verifies the source-linked long tables where the full selected raw-data set is present. This is a separate audit, outside the compact replay closure:

```sh
python3 paper/ispass2027/data/extract_results.py --check
```

This command is read-only with respect to the historical experiment inputs. Hardware collectors, synthesis and simulator runs are separate entry points; none is part of the offline figure rebuild.

## Optional rebuild and board reproduction

Use the baseline package's `metadata/BUILD.md`, source lock and saved command manifests for software/FPGA rebuilding. Use `board/skill/genesys2-iommu-board/SKILL.md` only when a hardware run is explicitly requested, with a new output directory. Reuse valid existing configurations and the established transfer/load/collect flow; retain built-in checks and stop on errors. Do not add repeat hashing, SD readback, unsolicited reboot loops or retrospective identity claims.

Reproduction reports must keep separate:

- G1 historical-image reproduction and the repaired baseline campaign;
- DS0 original wiring and added-delay sensitivity;
- per-trial repeats and actual boot count, including the original 25/50/25 order;
- hardware timing, non-atomic software HPM snapshots and per-trial path means;
- victim progress, aggressor response-accounted payload proxy, its boundary error, and whole-task completion;
- historical mitigation versions, new repaired-baseline measurements and behavioral-model simulations.

## Anonymous preparation and acceptance

1. Stage a new review copy. Retain raw evidence unchanged in the private master. Build the PDF with `Anonymous Authors`, empty PDF author metadata and page numbers; scan extracted text and embedded link targets for names, local account paths, hostnames and identifying repository URLs.
2. Remove author/funding/acknowledgment identities from the review copy as required by the venue, while retaining normal third-person citations to related platform work. Do not invent funding, conflict-of-interest or author-contribution facts; obtain these before submission. Preserve the actual AI-assistance disclosure required by the venue.
3. For any later anonymous artifact, map private paths to stable neutral relative paths and include a private reverse map. Scrub UART host/user/network identifiers only in derivative public copies. Label redactions and keep command parameters, timestamps needed for sequence/boot grouping, observed values and failure reasons reproducible.
4. Resolve symlinks into a portable archive, verify that every selected JSON pointer and figure/number operand resolves, rebuild in a clean environment, and compare the generated numeric artifacts with the accepted manifest. Confirm that pending experiments remain marked pending and every simulation axis retains its unit.
5. After acceptance, complete the author-owned declarations, choose the release license consistently with component licenses, restore the intended non-anonymous documentation, and publish the source/data archive through the authorized release workflow. Record a persistent version and checksum. This plan does not perform that publication.

## Current gaps

The current ten-figure draft is not a complete artifact release: active-device scaling, independent workload/size control, verified large-page comparisons, request-latency distributions and repaired-baseline mitigation reruns have gaps. DS25/DS100 may enter only through complete accepted manifests. The E7 capacity sweep now has a complete accepted 12-cell simulation receipt; its evidence remains separate from board results. Some historical loaded-BIT/software identity and HPM calibration checks were not recorded, and cannot be reconstructed by a later hash or archive commit. These limitations travel with the released evidence rather than being silently ``fixed'' during packaging.
