# E4 ABI4 driver batch — fixed source, host validation only

Linux commit **e3ca456a15d1952b97a2d5bde3a1021e27511893** on independent
`ispass/a3-pagesize` follows accepted leaf-query commit
`e61315397bd2bc925f74d76f7b8173f2b3256943` (it follows frozen common P0/multi
`acece3a3ab7d72ba1f936c1984b82a4a55f28d46`). The actual checkout is
`/home/xvhaocheng/work-a3-pagesize-linux`; it is clean after this commit.
This batch has not built an Image/payload or run on the board. Root alone owns hardware.

[Design](mapping-design.md), [real leaf query and allocator evidence](leaf-query-batch.md),
[one-commit patch](pages-driver.patch), [minimal incremental bundle](pages-driver.bundle),
[bundle verification](pages-bundle-verify.log).
No allocator, CMA, RTL, constraints, old UAPI header or HPM code changed.

## Behavior and exact boundaries

ABI4 adds CAPS `0x0d` and RUN_BATCH_PAGES `0x0e` while preserving ABI1/2/3.
The native and RV64 layouts are caps64, request984, status offset80,
embedded ABI1 experiment offset144 and four128-byte buffer proofs offset472
(`include/uapi/linux/idma_pages.h:16,30,60,92`).
The full request/proofs and two IOVA ownership flags live in one heap context
(`drivers/misc/idma.c:88,1855`); there is no new large request on the kernel stack.

V is device0/DID10 and A device1/DID11. Source and destination of **each** device
are fixed2MiB, totalDMA buffers8MiB, COPY4096, a512-page ring, V4096 measured tasks,
256 software warmup tasks, and A's existing long target until STOP after V.
Only explicit IOMMU alias mapping granularity changes (4096 or2097152).
Allocation size, flags, physical/IOVA alignment requirements, address order,
pattern, warmup, copy length, ring and victim work are identical.
A final completed work is measured and may differ between conditions.
This is a20-trial plan (two conditions×ten), with no extra Solo/mixed-leaf cases.

`idma_stream_allocate:291` extracts the existing allocator/pattern code, retaining
`DMA_ATTR_FORCE_CONTIGUOUS`; legacy prepare `:320` retains its PAGE_SIZE alias map.
E4 `idma_pages_contiguous:413` requires physical2MiB alignment and every4KiB physical
page contiguous. ENOMEM, alignment or continuity errors fail with their actual
errno before DMA. There is no retry, smaller allocation, 4K fallback, or CMA change.

`idma_pages_prepare:450` obtains two empty2MiB-aligned IOVA ranges and performs an
E4-only leaf-query API/domain preflight before mapping. `idma_pages_map_buffer:434`
uses one `iommu_map` call per requested leaf size and records only successful
prefix bytes. It never discards the prefix on failure. Each buffer is verified
before DMA by `idma_pages_verify_buffer:358`: all512 4KiB bases plus final byte,
actual rawPTE, actual leaf shift12/21, V/R/W, physical offset and leaf IOVA base.
Successful pre/post mask bits each represent the complete buffer proof.

The original pair execution body is now `idma_run_experiment_mapped:1215`, with a
NULL context supplied by the unchanged-signature legacy/P0 wrapper `:1464`.
Only E4 chooses the new prepare/cleanup and leaf reads. Existing warmup, cache
handoff, observation ARM, autonomous A-before-V start, HPM bracket, STOP, drain,
final COPY/unwritten-data checks and quarantine handling remain in that body.
E4 requires selected PATH and records its sequence. Pre-STOP A control and issued
are **sequential software reads**, not a simultaneous snapshot or exact A work
inside the V hardware window. Active/no-error/not-exhausted is required on success.
HPM remains a non-atomic software bracket and is not board-calibrated by these tests.

Post-leaf proof runs only after **both lanes** are neither in-flight nor retained.
A retained/unknown lane suppresses all post-leaf proof. A partial preparation
failure may still report a post proof for a fully mapped buffer while both lanes
are quiet; the original trial error remains failure. The proof fields identify
what was checked, not a successful experiment by themselves.

`idma_pages_cleanup:504` handles A then V. It unmaps every known prefix granule
with the matching requested page size, records returned byte counts, continues
both buffers after an error, and queries every4KiB address of the entire2MiB range
to require ENOENT. Any short/oversized unmap, residual leaf or query error retains
**both original allocations of that lane**, sets quarantine and prevents further
experiments until reboot. A quiet other lane can still be cleaned. In-flight or
already-retained lanes are never unmapped/freed. Primary trial errors precede
cleanup errors. Empty aliases mean no leaf; the pre-existing allocator's empty
page-table leak remains documented separately and was not repaired.

## Validation receipts

- [Final actual-function host receipt](pages-host-final/validation.json): **89 E4**
  cases, **38 legacy/P0 +57 multi** cases for both baseline and final source,
  non-PIE ASan+UBSan, all pass. The ordered legacy/P0 register-operation digest
  agrees exactly: `5869995487082302522:195665`. Production function bodies are
  extracted unchanged; allocation/MMIO/mapping/HPM/observer/cache are finite stubs.
  This tests orchestration and accounting, not real IOMMU/RTL/cache/event behavior.
- E4 cases cover BTC1/BTC2 ×4K/2M success, all four allocation and PA continuity
  failures, selected map-prefix failures across buffers, residual mapping despite
  map failure, unavailable/malformed/missing leaf pre/post, post final-byte offset,
  HPM/ARM/setup errors, warmup and batch zero IDs, drain failure, COPY failure,
  early/exhausted A, every buffer's2M short-unmap (with and without residual),
  every buffer's4K last-granule short-unmap, cleanup query error, original-error
  precedence, user-copy errors and caps. Retained buffers are only reclaimed by
  the host model's explicit simulated reboot between independent cases.
- [CLI receipt](pages-cli-validation.json), [45 raw mock results](pages-cli-cases.json):
  native and RV64 static builds; syscall wrappers never open hardware. Both leaf
  sizes, bad options/caps/results/proofs, returned errno/partial cleanup, and legal
  PTE A/D-only change are covered. Native and RV64 static assertions check ABI bytes.
- [Final RV64 kernel receipt](pages-kernel-validation.json), [raw log](pages-kernel-final.log),
  [stack usage](pages-idma-stack-usage.txt): `drivers/misc/idma.o` passes. Core944B,
  prepare224B, cleanup224B, proof160B; existing observer finish1488B and multi1312B.
  [First compile](pages-kernel-first.json) is preserved separately: its source was
  earlier than final preflight/CONFIG guards and is not the final validation identity.
- [Driver checkpatch](pages-driver-checkpatch.log): zero errors, two false-positive
  constant-placement warnings for `PAGE_SIZE != SZ_4K` (both are compile-time constants).
  [New UAPI checkpatch](pages-header-checkpatch.log): zero errors/warnings.
- `pages-host01` preserves an initial harness-only misleading-indentation build
  failure; `pages-host02` passed83 cases before six additional lifecycle cases.
  These are not substituted for the final89-case receipt.

[Actual page-table walker](leaf-query-batch.md) has independent accepted65-case /
7232-query evidence and the recorded PIE ASan startup limitation. This driver batch
uses non-PIE from the start; it does not reclassify the previous PIE failure.

## CLI and integration

`idma_pages.c` runs one bounded ABI4 trial at `/dev/idma0`:

```text
idma_pages --mapping-page-bytes 4096 --timeout-ms 60000
idma_pages --mapping-page-bytes 2097152 --timeout-ms 60000
idma_pages --caps
```

It emits full JSON `schema=idma.pages.run.v1`, including original/returned inputs,
all caps, all four proofs and embedded complete ABI1 results even on ioctl failure.
It validates the fixed work, roles, observed A active/target, final completion/bytes,
pre/post leaf evidence and fully empty/freed aliases before marking success.
It has no legacy fallback or direct MMIO. CLI source SHA256:
`264e75c6dc20d597e97831038c15b87b9d8274440661dbfb75505c9cedbe5314`.

**This standalone e3ca batch accepts PATHv3 only in CAPS_PAGES.** It must not be
called OBS4-ready. The future common E4/E5 integration must merge accepted E5
Linux `22975965c89285f5510cb89fd3a3c865b91c0d36` and change CAPS_PAGES to the
accepted `idma_observer_has_path` v3/v4 helper, then verify OBS4 caps and the complete
E4 path plus original v3 compatibility. Root reviews the fixed result before a
single future common Image/payload. Current acece E2/P0 delivery stays frozen.

The subsequent collector will group by `mapping_page_bytes`/`condition`, block1..10,
retain command/boot/sequence/identity/raw outputs and mapping failures, and attach
accepted E5 OBS4 histogram parsing to the same20 trials. There is no board result
or paper CSV claim in this batch.
