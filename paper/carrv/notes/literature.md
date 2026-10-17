# CARRV draft: bounded literature and background evidence

Prepared 2026-10-17, Codex-only. This is a reuse and targeted source check of the authorized local corpus, not a new literature search or exhaustive novelty review. No experiments, external-model calls, downloads, or human-read attestations were made. Paths below are relative to the workspace root `/home/xvhaocheng/work`.

## Corpus and verification scope

The five required scholarly identities were previously checked in `iommu-platform/out/plru-ablation-20261017/literature/`. Its `README.md:3` explicitly limits that earlier operation to metadata and local-original identity. This drafting pass directly checked only the technical passages listed below in the four existing texts; it does not upgrade any source to a new full-text or human-read status. Original PDF hashes and version identities remain in that directory's `metadata.json` and `verification.json`.

| BibTeX key | Verified identity and version | Content coverage used here |
| --- | --- | --- |
| `riommu2015` | Malka, Amit, Ben-Yehuda, Tsafrir; ASPLOS 2015, 355–368; DOI `10.1145/2694344.2694355` | Local author PDF/text, abstract mechanism statement only; no replication or borrowed performance claim. |
| `socdma2018` | Kurth, Vogel, Marongiu, Benini; ICCD 2018, 292–300; DOI `10.1109/ICCD.2018.00052` | Existing arXiv v1 author manuscript, abstract and §IV-C. IEEE metadata identifies the published work; the local bytes are not asserted to be the IEEE typeset version. |
| `iotlb_bottleneck2010` | Amit, Ben-Yehuda, Yassour; WIOSCA 2010 author manuscript, HAL `inria-00493752v1` | Local text §4.2–4.4: IOVA offsets, capacity/associativity, superpages and protection granularity. No DOI is assigned to this manuscript. |
| `zero_day_iommu2023` | Rodríguez, Costa, Sá, Pinto; RISC-V Summit Europe 2023, two-page extended abstract | Metadata only. The official abstract's title, authors, venue, and two-page extent were previously observed. No local body or technical result is available in the allowed evidence. |
| `mask2018` | Ausavarungnirun, Miller, Landgraf, Ghose, Gandhi, Jog, Rossbach, Mutlu; ASPLOS 2018, 503–518; DOI `10.1145/3173162.3173169` | Local text mechanism overview and §5.4; GPU-specific control points, without a claim of direct DMA equivalence. |
| `riscv_iommu_spec2026` | RISC-V International, RISC-V IOMMU Architecture Specification v1.0.1, 2026-02-22, ratified | Frozen official PDF cover and §4.1.1 Table 11/source text; local release source commit `054005f4ffe30838a4ffe144a01ea3933ffea6f2`. |
| `linux_riscv_mainline2026` | Descriptive source-code citation to `torvalds/linux@58161ead29e50d70a317ea26b472e0c7508e69da` | Frozen driver source §§listed below; not a paper, a runtime result, or a continuously current HEAD. No publication year or author was invented. |

Exact metadata authorities: `iommu-platform/out/plru-ablation-20261017/literature/README.md:7,15,23,31,39`; three publisher-submitted DOI records in its `raw/*.crossref.json`; Zero-Day field observations in `browser-metadata-observations.json:6–20`. The RISC-V release identity is on `iommu-platform/out/invalidation-interference-20261017/sources/spec-rtl/spec/riscv-iommu-v20260222.txt:1–3`; hash/version provenance is in that audit's `findings.md:9–12`. Linux snapshot identities are in `sources/linux/findings.md:9–15` and the per-version manifests.

## Proposed short related-work prose

The following is a bounded writing proposal, not a novelty verdict:

> IOTLB pressure and the cost of translation management have a substantial prior history. Amit et al. examine IOVA offsets, IOTLB capacity and associativity, and superpages as ways to reduce misses [iotlb_bottleneck2010]. rIOMMU exploits sequential ring-buffer access with a circular flat translation table and amortizes explicit invalidation over I/O bursts [riommu2015]. For heterogeneous SoCs, Kurth et al. combine translation prefetching and parallel software miss handling with DMA burst tracking and retry [socdma2018]. MASK addresses GPU multi-application interference through TLB-fill control, translation-aware cache bypassing, and memory scheduling [mask2018]. These works motivate separating replacement, translation service, and invalidation scope in an implementation-specific study. Rodríguez et al. provide a public two-page extended abstract entitled *Open-source RISC-V Input/Output Memory Management Unit (IOMMU) IP* [zero_day_iommu2023]; the verified metadata establishes that document's identity, but supplies no technical comparison for this draft.

The last sentence may be placed with platform provenance instead of the related-work paragraph to keep the limited reading scope unobtrusive. Do not turn it into a design/evaluation summary. The paper's own finding, scope and contribution must be grounded by the result/provenance agent's evidence, rather than supplied by these citations.

## Claim-to-original locators

| Proposed claim | Direct original locator | Boundary |
| --- | --- | --- |
| rIOMMU uses a circular flat table for predictable ring traffic; its described organization needs one IOTLB entry and explicit invalidations at burst boundaries. | `iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt:23–31` (abstract). | Ring-specific mechanism; no general multi-DMA isolation or present-platform speedup claim. |
| Kurth et al. use compiler-generated prefetch helpers, parallel miss helpers and DMA changes. | `iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt:25–36` (abstract). | Hybrid software-managed IOMMU; not evidence that this platform implements those helpers. |
| Their DMA retirement buffer stores burst identity and permits retry after translation misses, avoiding locking all pages for an entire transfer. | Same `socdma2018.txt:371–412` (§IV-C); especially `381–402` and `393–412`. | Original is external-memory/SPM and has recoverable failure/retry semantics. It does not prove any proposed arbitrary two-endpoint protocol equivalent or new. |
| Early IOTLB work considers distinct per-device IOVA offsets without hardware changes. | `iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_bottleneck2010.txt:457–470` (§4.2; two-column extraction). | Set-index assumptions and the 2010 environment are explicit in the original. Do not claim identical replacement behavior in this IOTLB. |
| More entries/associativity and larger pages are ordinary options; superpages trade translation reach against protection granularity. | Same `iotlb_bottleneck2010.txt:465–471,510–544` (§4.3–4.4). | Does not establish legal hugepage use for arbitrary mixed-owner allocations. |
| MASK regulates shared-TLB fills, uses a bypass cache, and separates translation/data memory scheduling. | `iommu-platform/out/shared-translation-literature-20261006/papers/mask2018.txt:660–678` (overview); `881–924` (§5.4). | GPU proposal; no claim it implements this RISC-V CQ, DMA API, or local partition tree. |
| Zero-Day abstract identity only. | `iommu-platform/out/plru-ablation-20261017/literature/browser-metadata-observations.json:6–12`; identity caveat in `README.md:31–35`. | Metadata receipt records title/authors/footer, not body passages. No architecture, area, speed, compliance or evaluation conclusion follows. |

## Source-supported RISC-V and Linux background

Suggested wording:

> RISC-V IOMMU commands synchronize page-table updates with translation-cache state. For host first-stage translation, `IOTINVAL.VMA` can select all host address spaces, a PSCID, or leaf translations covering an address; PSCID-qualified forms exclude global mappings [riscv_iommu_spec2026]. Thus separate address-space identifiers alone do not prevent a command that selects all host address spaces from invalidating unrelated translations.

Original support: `iommu-platform/out/invalidation-interference-20261017/sources/spec-rtl/spec/iommu_in_memory_queues.adoc:172–192,208–233`, §4.1.1/Table 11 (printed page 49, PDF page 53). The final sentence is a direct inference from the all-host versus PSCID rows, not a separate measured result. `GV=0` makes the second stage Bare and ignores GSCID; `AV=1,PSCV=0` is address-selective across all host PSCIDs, rather than device-selective. Keep non-global mappings explicit when claiming PSCID isolation.

> In the audited local Linux DMA-IOMMU path, mapping allocates an IOVA and installs a translation; successful region mapping invokes the driver's map synchronization callback. An unmap that is not queued synchronizes the IOTLB before returning the IOVA. This local RISC-V driver uses a host-wide invalidation because its PSCID setter and address loop are disabled. The fixed mainline snapshot already sets PSCID before either address-specific invalidation or its broader fallback [linux_riscv_mainline2026].

These are source facts, not observations of a running kernel. Direct locators, all under `iommu-platform/out/invalidation-interference-20261017/sources/linux/`:

| Fact | Direct source locator |
| --- | --- |
| Local IOVA allocation and mapping. | `C-platform/drivers/iommu/dma-iommu.c:730–753`. |
| Success calls `.iotlb_sync_map` once per mapped region. | `C-platform/drivers/iommu/iommu.c:2402–2417`; callback implementation `C-platform/drivers/iommu/riscv/iommu.c:1848–1858`. |
| Nonqueued unmap performs gather sync, then IOVA release. | `C-platform/drivers/iommu/dma-iommu.c:708–727`. |
| Local host path disables PSCID and address qualifiers. | `C-platform/drivers/iommu/riscv/iommu.c:1799–1817`; construction/reset/setters at `333–369` establish AV=PSCV=GV=0. |
| Mainline retains PSCID even at the label named `global`. | `A-mainline/drivers/iommu/riscv/iommu.c:1005–1048`, particularly `1014–1015`. |
| Mainline fallback depends on command count, levels and capabilities. | `A-mainline/drivers/iommu/riscv/iommu.c:961–1002,1017–1048`. It is not an unconditional “512 pages means system-wide flush” rule. |
| Mainline recognizes generic_pt domains as supporting deferred-flush queues. | `A-mainline/drivers/iommu/dma-iommu.c:652–658`; initialized path is recorded in `findings.md:33`. |

Do not describe that local driver behavior as a current Linux-mainline flaw. Audited identities: A/mainline `58161ead29e50d70a317ea26b472e0c7508e69da`; B/development `d371fb0f95d5bae20af8135f976ffd8a06bfaa23`; C/local platform `f215540ad19dfe88c91b43b82c09cf40c514f323`. Source comparison does not establish that any benchmark invoked the complete Linux map/unmap sequence; the CQ simulation and Linux API execution remain distinct evidence levels.

## Drafting constraints relevant to the main argument

- Attribute the observed partition-aware tree-PLRU behavior to the exact local research version unless a separate original-code comparison establishes otherwise. This literature check does not establish an upstream implementation flaw. Nominal mask capacity, occupied entries, actual victims and repeat walks require experiment/source evidence, not a related-work citation.
- Quantify the configured victim/replacement choices, invalidation qualifiers and completion tradeoffs using the existing result data. Avoid field-wide claims such as “first shared-IOMMU interference study” or “existing systems lack coordinated translation/data scheduling.” MASK alone rules out the latter formulation.
- Large pages, IOVA layout, replacement policies and precise invalidation have ordinary antecedents. An observed benefit of one of them can support a bounded characterization result without proving a new mechanism.
- No comprehensive search, retraction check, literature ranking, hardware replication or domain-wide novelty certification was performed here. Unresolved identity and content checks are in `literature-to-verify.md`.

## Later supplied standard

The completed prior-work audit supplied Intel VT-d Rev.5.0. Its official PDF cover identifies August 2024, Intel Corporation, and order D51397-017. The directly checked scope passage is `iommu-platform/out/iofence-interference-20261017/prior/sources/intel-vtd-rev5.txt:8169-8183` (§6.5.4, printed6-46 / PDF137). `intel_vtd2024` was added for domain/page-selective drain guarantees only. No vendor timing or noninterference property is inferred from the minimum guarantee. The final bibliography therefore has eight entries.

The Linux source citation uses an explicit `n.d.` absence-of-date notation and a `Linux source` sorting key to avoid the installed ACM BibTeX style's malformed nested-bracket fallback. Neither field invents a publication year or named author. The audited commit and date remain in the note.
