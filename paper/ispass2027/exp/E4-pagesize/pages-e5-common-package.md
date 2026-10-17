# E4/E5 common software and20-point collection entry

The **single** common software build passed on its first complete execution.
Kernel release is **6.5.0-rc2-ispass2027-e4-e5-v1**; payload29363656B,
headroom4190776B below the existing32MiB budget. No board operation was performed
by A3. Root owns all loading, updates and collection and will use only the final
accepted E5 OBS4 BIT after the earlier P0/E2 schedule.

## Fixed components and build

- Linux **da6b69fc007ad5e671dc288dd8038d02e3ebe43d**, independent
  `/home/xvhaocheng/work-a3-pagesize-linux`, branch`ispass/a3-pagesize`, clean.
  It is accepted E4`e3ca456a15d1952b97a2d5bde3a1021e27511893`, a true merge
  (`656eb561ca64c7067ff5da713c5c9156aac8f82c`) of accepted E5
  `22975965c89285f5510cb89fd3a3c865b91c0d36`, then one CAPS_PAGES line using
  `idma_observer_has_path` for both v3/v4. No other production changes.
- Platform **a68603f0fc668771bc0c9e3a86fcefe289bfa17b**, independent
  `/home/xvhaocheng/work-a3-pagesize-platform`, same branch, clean. Parent52c5fdd;
  only actual Linux/CVA6 gitlinks and matching`sources.lock.json` changed.
- CVA6 **6c5c666d45979d3a03fb98cff89ba7ad095f6125** (A1 frozen E5 RTL),
  IOMMU **85b157ebe71a623e326e51fff7b7126fe77e8eb9**,
  OpenSBI **e8d21bfdc2dd753314627968e496c3a1188f6dfb**.
- Linux[patch](pages-e5-integration.patch), [bundle](pages-e5-linux.bundle),
  [verification](pages-e5-bundle-verify.log); platform[patch](pages-e5-platform.patch),
  [bundle](pages-e5-platform.bundle),[verification](pages-e5-platform-bundle-verify.log).
  The bundle requires accepted leaf-query e613; it includes E4+E5 source ancestry.

The actual complete build is local:
`/home/xvhaocheng/work-a3-pagesize/paper/ispass2027/exp/E4-pagesize/artifacts/e4-e5-software01`.
Use its **passed build-manifest.json directly** as the software delivery receipt.
[Exact manifest copy](pages-software-build-manifest.json),
[outer commands](pages-software-command-receipt.json),
[full raw build log](pages-software-build.log), [launch result](pages-software-launch01.log),
[source bindings](pages-e5-source.json).

[Builder](build_pages_software.py) copies A2's accepted prepare/finalize flow and
its existing helper[source](source/build_baseline.as-reused.py), changes only task
schema, release suffix, selected E5 CVA6 and the extra critical source list.
It archives immutable Linux/OpenSBI commits, preserves exact original
`idma_experiment`, `platform-timeout`, `platform-sd-update` binaries and adds
`idma_p0`, `idma_scaling`, `idma_pages` from frozen inputs. It includes final kernel,
DTB/rootfs/loader/OpenSBI payload and matching SPI recovery modules.
`modules_prepare` runs before external modules, then vmlinux.symvers is copied to
Module.symvers. There was no module.lds failure, repair or suppressed modpost.
Both modules'[actual vermagic](pages-spi-vermagic.log) matches the new release.
The current acece E2/P0 package and the E5 Vivado archive were untouched.

## Finite validation boundaries

[Final common-source lifecycle receipt](pages-e5-host-final/validation.json):
**97 E4** cases plus **38 legacy/P0 +57 multi** for both old and final source,
non-PIE ASan+UBSan. The original v3 suite is retained; OBS4 adds full E4
orchestration for BTC1/BTC2×4K/2M, CAPS v1..4 and rejection before allocation when
PATH is absent. The exact legacy/P0 MMIO digest remains
`5869995487082302522:195665`. This lifecycle model **stubs observer/MMIO/cache/HPM**.

Separately, A1's accepted actual observer function harness was run against the
**same final source**: [48 checks](e5-observer-host-final/summary.json),
[manifest](e5-observer-host-final/manifest.json),
[raw fixture output](e5-observer-host-final/fixture.log).
It extracts actual prepare/hist-finish/finish/path-capability functions unchanged
and uses mocked frozen MMIO. This verifies their v3/v4 and malformed-hist handling;
it does not turn the lifecycle model into an actual-RTL integration simulation.

[Final RV64 object receipt](pages-e5-kernel-validation.json),
[raw compile log](pages-e5-kernel-final.log),[stack usage](pages-e5-idma-stack-usage.txt).
The full common Image/payload build additionally compiled these actual sources.
The prior45 CLI mock/static layout cases still bind the identical frozen CLI and
headers; the common builder also compiled that exact CLI with final archived UAPI.
None of these host/build checks establishes board HPM calibration or measured speed.

## Transport, Root only

[Transport metadata](pages-software-transport.json) binds the existing passed
manifest and source artifact hashes. Deterministic gzip uses level6, mtime0 and
no embedded filename; a host decompression byte comparison passed. Original
payload/module hashes are reused from the build manifest.

- `artifacts/e4-e5-software01/fw_payload.bin.gz`:10795535B.
- `artifacts/e4-e5-software01/spi-recovery-e4-e5.tar.gz`:8869B, exactly
  `spi-bitbang.ko` then`spi-xilinx.ko`, both for the **new e4-e5 kernel**.

Use existing UART gzip/bytesack handling. The current acece kernel requires its
own P0/multi modules while writing the new software. The new module archive is
only for a later update under the new kernel. Formal experiments load no modules.
BusyBox decompression commands are recorded in the metadata; no tar`-z` is assumed.
No SD write, readback, extra image hashing or board verification was added here.

## Collector contract

[collect_pages.py](collect_pages.py) plans exactly20 trials on one DS0 OBS4 boot:
each block1..10 runs leaf4k then leaf2m. `planned_cases` is an ordered array of
`{'block':1,'condition':'leaf4k','mapping_page_bytes':4096}` (then leaf2m2097152).
Each accepted item has schema`ispass2027.e4_pagesize.record.v1`, case1-based and the
same three planned fields. `item.record` is the original full CLI JSON with
schema`idma.pages.run.v1`; the real CLI`input.mapping_page_bytes` is checked against
both the plan and actual pre/post shift/PTE proofs. [Saved plan](pages-plan.json).

The collector verifies complete CLI/caps/role/fixed-work/bytes/quiet/unmap/free
proofs, same boot/kernel and consecutive fresh observer sequence. It retains all
existing private stock/flow and16 PATH checks. Shared accepted counts use selected
PATH0 full+carry and exact two-device equality. Signed private-tap residuals can
be negative across the mux spill boundary; no unsupported private/first-offer
stock identity was introduced. See[shared-count derivation](../../board/observer-shared-counts.md).

Only after those checks does it call accepted E5
`hist_records.attach_from_output(raw.stdout,item)`, requiring INFO1/OBS4 and all16
same-sequence full-count histograms with32bins. `item.histogram` stores the result.
The original complete post-command dmesg boundary, raw command, CLI output and
failure remain saved. First error stops; there is no retry/fallback or4K replacement
for failed2M mapping. Allocation/cleanup failures remain failed raw trials.

The campaign manifest records status/running→passed or failed, actual receipts,
boot, plan and helper hashes. `--recover-run` only restores a complete successful
raw prefix on the exact same immutable identities/boot and resumes missing points.
Wrong command, timeout/partial command, sequence gap, relabelled point or identity
change rejects recovery. Host fixtures cannot start actual collection.

[38 protocol/recovery checks](pages-collector-host.json),
[14 identity/check-only checks](pages-identity-host.json). All constructed transcripts
in[fixtures](pages-collector-fixtures.json) are explicitly synthetic, never board
measurements. Closing spill/private residual−1 is accepted with every conservation
check intact; changing shared total is rejected. Host tests and `--plan-only` /
`--check-only` do not import UART. The identity test includes the actual passed
software manifest and synthetic FPGA/load/package receipts; final real BIT/load
binding remains pending Root's accepted build and boot receipts.

After the final E5 BIT and Root package/load receipts exist:

```text
python3 collect_pages.py --check-only --package PACKAGE --load-manifest LOAD.json \
  --fpga-manifest E5_BUILD.json --software-manifest SOFTWARE/build-manifest.json
python3 collect_pages.py --output NEW_E4_RUN --package PACKAGE --load-manifest LOAD.json \
  --fpga-manifest E5_BUILD.json --software-manifest SOFTWARE/build-manifest.json
```

The default E5 helper directory is the sibling accepted E5-latency directory;
`--e5-dir` can point at the integrated copy. This collector does not load BITs,
write SD or create a package. Package/source/load formats reuse existing fields
including artifacts.ds0_bit, source.actual_commits and boot_identity_verified.
A1's accepted interval analysis groups explicitly by mapping_page_bytes, rebinds
raw command/CLI/PATH/512bins and handles full-only A samples separately from carry.
There is no additional standalone E5 campaign and no real E4 result/CSV yet.

A1 independently exercised this exact frozen protocol once through parse→raw
rebind→interval analysis: leaf4k/leaf2m/closing-spill and a one-point internal
20-plan prefix CSV export all passed, explicitly fixture-only/source_board_cases0.
All public publication paths rejected the fixtures. Accepted evidence is E5
commit`6eaa15b10dd0ed6158bfa29255704cbe1cc82b0e`,
[compatibility receipt](../E5-latency/protocol-compat01/receipt.json).

## Focused recursive startup guard revision

Root requested the accepted E5 `synthetic_markers` recursion at actual collection
startup. The collector now uses those exact marker semantics for every nesting
level of package/source/load/FPGA/software JSON, after host check-only returns but
before output-directory creation or UART import. This changes only the explicit
synthetic-evidence startup boundary; parsing, plan, recovery and production Linux
are unchanged. [Focused receipt](pages-nested-guard-host.json) has11 checks,
including five actual-main subprocesses, one nested evidence_level marker in each
identity source, all refusing collection with no directory and zero UART import
attempts. The prior38 protocol/14 identity receipts remain byte-for-byte preserved
for the earlier7efc8fe collector revision; no unchanged driver suite was rerun.

## OBS4 experiment qualification remains fixed

The shared histogram helper's accepted OBS5 extension does not widen E4. After
attachment, E4 explicitly requires info.version4/schema1 with no depth field.
[Two focused checks](pages-hist-version-host.json) accept the unchanged OBS4 case
and reject a self-consistent OBS5/schema2/depth64 case that the shared helper
correctly accepts. No driver suite or hardware run was added.

## Ready host package after final E5 BIT passed

[Preparation receipt](pages-package-preparation.json),
[manifest copy](pages-package-manifest.json),[source copy](pages-package-source.json)
bind the accepted final E5 BIT SHA
`d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c`
and the already accepted common software. The ready local directory is
`/home/xvhaocheng/work-a3-pagesize/paper/ispass2027/exp/E4-pagesize/artifacts/e4-e5-package01`.
Its small metadata are copied from original passed manifests; binary artifacts
are hardlinks to the exact frozen files. Inode/size equality was checked and all
binary hashes reused from existing build/transport receipts; no binary rehash,
load record, FPGA operation or new board sample was created.

Root may copy these small manifest/source files and link binaries into its formal
board/packages directory. Formal collector FPGA/software arguments should use
Root's own exact metadata copies (E5/build/completed02/build-manifest.json and
E4/pages-software-build-manifest.json), so complete paper export can constrain all
original metadata/raw inputs to the Root worktree source root. Keep the original
package/source identity bytes unchanged; place results in their separate run
directory. The package is ready_for_collection, not a successful boot or result.
