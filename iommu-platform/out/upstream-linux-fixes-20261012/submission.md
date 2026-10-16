# Linux contribution preparation — 2026-10-12

Target: malejo97/linux, wip/riscv-iommu, d371fb0f95d5bae20af8135f976ffd8a06bfaa23.

Both changes remain in the actual platform upstream branch. Its master lacks these legacy files; the newer tjeznach/linux riscv_iommu.next HPM macro is already parenthesized, and current Linux mainline has a different page-table implementation. These are legacy platform contributions, not demonstrated new mainline bugs.

Both independent branches were pushed successfully to xvhaocheng/linux_iommudirver. Each contains one commit and one source file relative to the verified upstream base. Authorship xvhaocheng <xvhaocheng@gmail.com>. No DMA cache workarounds, experimental HPM API, or current dirty Linux changes included. No Signed-off-by certification added.

Actual tests: IOVA baseline 16 cases/12 failures, patched 16/0. HPM baseline 32 cases/14 failures, patched 32/0. Checkpatch --no-signoff --no-tree: both 0 errors/0 warnings; diff checks passed. Host extracted production functions/macros only, not full kernel build or new hardware test.

PR creation attempted against malejo97/linux: GitHub integration returned 403 Resource not accessible by integration. No PR created. Second identical creation not retried. Use the prefilled upstream comparison forms below; these are creation forms, NOT existing PR URLs.

## IOVA

Commit: 8e3a7406fadb
Head: xvhaocheng/linux_iommudirver:fix/iova-leaf-offset
Title: iommu/riscv: Preserve the offset within the translated leaf

## Problem
`riscv_iommu_iova_to_phys()` ORs `iova & PAGE_MASK` into the physical frame base. This copies IOVA page-number bits into the physical address and discards the byte offset, affecting ordinary 4 KiB mappings as well as huge pages.

## Fix
Return the leaf shift from `riscv_iommu_pt_walk_fetch()` and combine the physical frame with only the offset within that leaf. Update the recursive and lookup callers; unmap passes NULL. Covers 4 KiB, 2 MiB and 1 GiB leaves.

## Scope and provenance
One commit, only `drivers/iommu/riscv/io_pgtable.c`, based on `wip/riscv-iommu` at d371fb0f95d5bae20af8135f976ffd8a06bfaa23. Isolated from my platform fix 3b47ab1645e412095b601e3be68b6347b9fad213. CVA6 DMA cache workarounds and research changes are excluded. This targets the legacy platform driver, not a claim that the current Linux mainline implementation has this defect.

## Verification actually run
A host C harness compiles the actual walk and lookup functions using a software Sv39 PTE model. 16 cases cover 4 KiB offsets, 2 MiB and 1 GiB interior/end offsets, missing entries and identity lookup: baseline 12 failures; patched 0 failures. `git diff --check` and checkpatch (--no-signoff --no-tree) pass. No full kernel build or new FPGA test performed.

[Open prefilled upstream PR form](https://github.com/malejo97/linux/compare/wip%2Friscv-iommu...xvhaocheng:linux_iommudirver:fix%2Fiova-leaf-offset?expand=1&title=iommu%2Friscv%3A%20Preserve%20the%20offset%20within%20the%20translated%20leaf&body=%23%23%20Problem%0A%60riscv_iommu_iova_to_phys()%60%20ORs%20%60iova%20%26%20PAGE_MASK%60%20into%20the%20physical%20frame%20base.%20This%20copies%20IOVA%20page-number%20bits%20into%20the%20physical%20address%20and%20discards%20the%20byte%20offset%2C%20affecting%20ordinary%204%20KiB%20mappings%20as%20well%20as%20huge%20pages.%0A%0A%23%23%20Fix%0AReturn%20the%20leaf%20shift%20from%20%60riscv_iommu_pt_walk_fetch()%60%20and%20combine%20the%20physical%20frame%20with%20only%20the%20offset%20within%20that%20leaf.%20Update%20the%20recursive%20and%20lookup%20callers%3B%20unmap%20passes%20NULL.%20Covers%204%20KiB%2C%202%20MiB%20and%201%20GiB%20leaves.%0A%0A%23%23%20Scope%20and%20provenance%0AOne%20commit%2C%20only%20%60drivers%2Fiommu%2Friscv%2Fio_pgtable.c%60%2C%20based%20on%20%60wip%2Friscv-iommu%60%20at%20d371fb0f95d5bae20af8135f976ffd8a06bfaa23.%20Isolated%20from%20my%20platform%20fix%203b47ab1645e412095b601e3be68b6347b9fad213.%20CVA6%20DMA%20cache%20workarounds%20and%20research%20changes%20are%20excluded.%20This%20targets%20the%20legacy%20platform%20driver%2C%20not%20a%20claim%20that%20the%20current%20Linux%20mainline%20implementation%20has%20this%20defect.%0A%0A%23%23%20Verification%20actually%20run%0AA%20host%20C%20harness%20compiles%20the%20actual%20walk%20and%20lookup%20functions%20using%20a%20software%20Sv39%20PTE%20model.%2016%20cases%20cover%204%20KiB%20offsets%2C%202%20MiB%20and%201%20GiB%20interior%2Fend%20offsets%2C%20missing%20entries%20and%20identity%20lookup%3A%20baseline%2012%20failures%3B%20patched%200%20failures.%20%60git%20diff%20--check%60%20and%20checkpatch%20(--no-signoff%20--no-tree)%20pass.%20No%20full%20kernel%20build%20or%20new%20FPGA%20test%20performed.)

## HPM

Commit: 77805904b3c0
Head: xvhaocheng/linux_iommudirver:fix/hpm-macro-arguments
Title: iommu/riscv: Parenthesize HPM register index arguments

## Problem
`RISCV_IOMMU_REG_IOHPMCTR(_n)` and `RISCV_IOMMU_REG_IOHPMEVT(_n)` multiply an unparenthesized argument by the 8-byte register stride. For `lane=1`, `lane * 3 + 1` yields BASE + 11 instead of BASE + 32, selecting the wrong address.

## Fix
Parenthesize `_n` in both macros. Plain index arguments keep their existing results.

## Scope and provenance
One commit, exactly two macro replacements in `drivers/iommu/riscv/iommu-bits.h`, based on `wip/riscv-iommu` at d371fb0f95d5bae20af8135f976ffd8a06bfaa23. Extracted from my platform change affbfbf6adda41d5b33f7b36880279d8ee8f48f7; no experimental HPM API or benchmark changes. The newer `tjeznach/linux:riscv_iommu.next` header already has argument parentheses, so this is a fix for the legacy platform branch, not a new mainline regression.

## Verification actually run
Extracted the actual four #define lines from each header and compiled a host C test with -Wall -Wextra -Werror. Eight lanes, plain and compound indices, both macros: 32 cases; baseline 14 failures; patched 0 failures. Compound indices stay within 0..31. `git diff --check` and checkpatch (--no-signoff --no-tree) pass. No full kernel build or new hardware test.

[Open prefilled upstream PR form](https://github.com/malejo97/linux/compare/wip%2Friscv-iommu...xvhaocheng:linux_iommudirver:fix%2Fhpm-macro-arguments?expand=1&title=iommu%2Friscv%3A%20Parenthesize%20HPM%20register%20index%20arguments&body=%23%23%20Problem%0A%60RISCV_IOMMU_REG_IOHPMCTR(_n)%60%20and%20%60RISCV_IOMMU_REG_IOHPMEVT(_n)%60%20multiply%20an%20unparenthesized%20argument%20by%20the%208-byte%20register%20stride.%20For%20%60lane%3D1%60%2C%20%60lane%20*%203%20%2B%201%60%20yields%20BASE%20%2B%2011%20instead%20of%20BASE%20%2B%2032%2C%20selecting%20the%20wrong%20address.%0A%0A%23%23%20Fix%0AParenthesize%20%60_n%60%20in%20both%20macros.%20Plain%20index%20arguments%20keep%20their%20existing%20results.%0A%0A%23%23%20Scope%20and%20provenance%0AOne%20commit%2C%20exactly%20two%20macro%20replacements%20in%20%60drivers%2Fiommu%2Friscv%2Fiommu-bits.h%60%2C%20based%20on%20%60wip%2Friscv-iommu%60%20at%20d371fb0f95d5bae20af8135f976ffd8a06bfaa23.%20Extracted%20from%20my%20platform%20change%20affbfbf6adda41d5b33f7b36880279d8ee8f48f7%3B%20no%20experimental%20HPM%20API%20or%20benchmark%20changes.%20The%20newer%20%60tjeznach%2Flinux%3Ariscv_iommu.next%60%20header%20already%20has%20argument%20parentheses%2C%20so%20this%20is%20a%20fix%20for%20the%20legacy%20platform%20branch%2C%20not%20a%20new%20mainline%20regression.%0A%0A%23%23%20Verification%20actually%20run%0AExtracted%20the%20actual%20four%20%23define%20lines%20from%20each%20header%20and%20compiled%20a%20host%20C%20test%20with%20-Wall%20-Wextra%20-Werror.%20Eight%20lanes%2C%20plain%20and%20compound%20indices%2C%20both%20macros%3A%2032%20cases%3B%20baseline%2014%20failures%3B%20patched%200%20failures.%20Compound%20indices%20stay%20within%200..31.%20%60git%20diff%20--check%60%20and%20checkpatch%20(--no-signoff%20--no-tree)%20pass.%20No%20full%20kernel%20build%20or%20new%20hardware%20test.)

Model attribution: DeepSeek supplied initial description/test suggestions; corrected revision still used incorrect module/macro names and was rejected (complete revision saved in deepseek-revision.json). Codex directly verified source, isolated original user changes, corrected text, authored small macro test after bounded worker failure, executed checks and pushed. No Claude or Codex subagents.

