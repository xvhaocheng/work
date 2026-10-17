# E4 software profile compatibility: host-only preparation

This page records the initial `fa945ec` framework stage. The accepted common-software follow-up and new independent package are recorded in [common-software-package.md](common-software-package.md); the original stage evidence below is retained.

## Material Passport

- Stage: bounded experiment-tool maintenance and host reproducibility checks.
- Scope: E4 files only; no FPGA programming, UART, SD update, software/RTL build, or board campaign.
- Status: legacy exact profile accepted; no new ABI5 profile registered. New board cases: **0**.
- Model: Codex-only, as required by the shared IOMMU project. DeepSeek was not used.

## Accepted interface and refusal conditions

`software_profiles.py` is the only finite software-profile registry. `validate_software(software, expected_kernel)` returns a profile name only when the complete Linux/platform/software-CVA6/kernel-release tuple matches a registered entry. It retains the passed-status, unchanged `idma_pages.c` SHA256, required program and matching SPI recovery release checks. The current registry contains only the original `e4-e5-v1` tuple (Linux `da6b69fc…`, platform `a68603f0…`, software-CVA6 `6c5c666d…`, release `6.5.0-rc2-ispass2027-e4-e5-v1`).

`validate_bound_software(bound, software, source, fpga)` binds actual runtime Linux, software platform and OpenSBI to the passed software receipt, while independently requiring loaded E5 CVA6 `6c5c666d…`, IOMMU `85b157eb…`, and the accepted `d934846e…` BIT. The old immutable package remains valid without new role annotations. Any future registered profile requires `software_build_reference` and `fpga_build_reference` annotations: software CVA6 is a build reference, and any Linux mentioned by the original FPGA source manifest is also a build reference. Neither overrides the actual runtime components.

The collector and exporter share these checks. Actual collection records the profile and profile-helper SHA256; prefix recovery additionally compares both fields and still compares boot, source, software, FPGA, load, package, collector and observation-helper identities. Complete export requires these current source hashes and still requires passed20, the exact frozen order, original raw commands, unchanged identity sources and full original protocol validation. Unknown or mixed tuples, changed CLI, mismatched releases/components/BIT, cross-profile/source/boot recovery and changed helper hashes are rejected.

`prepare_package.py` uses the accepted software platform's own source lock. Its CVA6 lock comparison is against the software build reference; `source.actual_commits.cva6/iommu` remain the loaded E5 hardware. It copies the original software/FPGA manifests byte-for-byte and hardlinks existing artifacts using accepted manifest hashes. It does not rewrite or rehash binaries. The original `artifacts/e4-e5-package01` and all old `pages-*.json` receipts remain unchanged.

The 20-point plan, exact CLI input/hash, leaf/PA/PTE/lifecycle proofs, private/shared accounting, PATH equations, OBS4/INFO1/Depth4, histograms and recursive fixture gates remain in place. E5 files and the Linux/CLI implementations were not changed.

## Actual FPGA reference correction

The original E4 package's `metadata/fpga-build-manifest.json` equals E5 `build/completed02/build-manifest.json`, SHA256 `94fff46f7f87d6687f431f96f570c934a84f0dcc9b3726fd9c36893b611ceb5c`. Its actual top-level fields are `selected_platform=2f19f6c2de5530760f521bf771b6217066bc54d5`, CVA6 `6c5c666d45979d3a03fb98cff89ba7ad095f6125` and IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`; it has no `selected_linux` field.

Its linked E5 `build/e5-fpga-source.json` names Linux `acece3a3ab7d72ba1f936c1984b82a4a55f28d46` with `linux_reference_only=true`. The new source annotation retains the original FPGA source-manifest path and hash rather than inventing a `selected_linux` field. The initial host preparation failed on that absent field; its exact subprocess stderr and failed status are retained in [first-attempt validation](software-profile-host01/validation.json). The partial host-only package is retained locally under the ignored `artifacts/software-profile-host01/` directory and has no ready manifest.

## Finite host evidence

| Receipt | Actual result | Coverage |
| --- | --- | --- |
| [Profile/package validation](software-profile-host03/validation.json) | passed20 checks | Actual legacy receipt, unknown/mixed tuple refusal, unchanged CLI/SPI gates, actual unknown-profile collector refusal before output/UART, actual metadata-copy and hardlink verification, all five runtime-component mismatches, fixed BIT |
| [Prepared source](software-profile-host03/prepared-source.json), [package receipt](software-profile-host03/package-receipt.json) | legacy host-only package | Explicit software/hardware roles; original build-manifest copies and accepted binary hash reuse |
| [Identity check](software-profile-host02/identity.json) | passed14 checks | Check-only path and explicit fixture actual-start refusal |
| [Collector protocol](software-profile-host02/collector.json) | passed41 checks | Full synthetic20 protocol; same-boot prefix acceptance; profile/helper/source/boot and malformed-prefix refusals |
| [Nested fixture guard](software-profile-host02/nested-guard.json) | passed11 checks | Five actual startup refusals, zero UART import attempts |
| [Complete export](software-profile-host02/export-final/validation.json) | passed31 checks | Full synthetic20, profile relabel/helper-hash refusals, prior complete/raw/identity/histogram/public-fixture gates |

All these campaign fixtures remain explicitly synthetic with zero source board cases. The nested-guard harness needed its script directory placed on `sys.path` when using `runpy` after introducing a local profile module; its final run passed without any UART import. Generated export build trees remain local and ignored; validators, locked input fixtures and receipts are retained for replay.

## Next bounded batch

Root paused the full-E3 build while diagnosing the E2 multi-device scheduling failure. The final new Linux may therefore differ from `46df97c…`. Do not register the old proposed Linux alone, the planned release or any arbitrary 40-hex platform. Root must supply the final complete passed software tuple and accepted manifest/transport paths. Then register exactly that tuple, run focused new-profile role/mismatch checks, and create a new E4/E5 package; preserve this old package and all original manifests.
