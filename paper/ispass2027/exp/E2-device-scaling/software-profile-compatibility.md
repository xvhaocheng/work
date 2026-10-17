# E2 common software compatibility

## Material Passport

- Scope: E2 host tools, finite identity profiles and synthetic validation. Codex-only; DeepSeek was not used.
- Status: both exact software tuples accepted using their actual passed manifests; no new board cases, builds, JTAG/UART/SD operation.
- Original failed five-point prefix, failure tail, frozen paper output and old tool/source commits remain unchanged. They are separate old-version evidence; this change does not migrate or resume them.

## Identity and role contract

`e2_software_profiles.py` contains exactly two profiles. Both require `status=passed`, the unchanged scaling CLI source SHA256 `b1433570…`, and `bin/idma_scaling` in the actual completed build manifest.

| Profile | Linux | Software platform | Software CVA6 reference | Kernel release |
| --- | --- | --- | --- | --- |
| `p0-multi-v1` | `acece3a3ab7d72ba1f936c1984b82a4a55f28d46` | `52c5fdd028bbf3b07deaf1d0f7e13e3a271b29d0` | `8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8` | `6.5.0-rc2-ispass2027-p0-multi-v1` |
| `full-e3-v1` | `65b427946152e3aca27babab0a437821729805d4` | `ec99648e3c9a1e48c1e9721e3662bb632eb439bd` | `1d587125eee690e623e6ffae13e70f9bbe989adc` | `6.5.0-rc2-ispass2027-full-e3-v1` |

Both use exact OpenSBI `e8d21bfdc2dd753314627968e496c3a1188f6dfb`. The new actual manifest has SHA256 `7415dcc4d076fa434bc22294f4309582af84086964f1b47bb05c695555f0bcc4`.

E2 continues to require the original DS0 BIT `e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6`, actual CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069` and IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`. The existing baseline100 source record remains the hardware provenance object, with its original platform `885ed0a7…` and Linux `45bf76b1…` fields preserved as historical build references. The E5 `d934846e…` BIT is explicitly rejected by E2.

`bind_identity()` records actual runtime Linux/software-platform/OpenSBI independently from actual loaded CVA6/IOMMU. `software_build_reference` and `source_roles` explain the software CVA6 reference and old hardware source record. It does not rewrite either original manifest or substitute a hardware reference for running Linux.

The collector, full exporter and prefix validator use this same helper; pairs consume the strict full exporter. New runs retain original source/software receipt byte hashes, the profile/helper hash, runtime component identities and existing collector/load/boot identities. Recovery rejects a change in any of these. The full/prefix/pairs paths require the current frozen collector and helper source identities. Old five-point snapshots remain replayable with their archived tool commit; current tools do not weaken source/hash gates to recertify them.

`collect_multi.py --check-only` validates metadata without an output directory or UART import. `--plan-only` also remains host-only. Actual startup rejects recursive explicit fixture markers before creating output or importing UART. For real new-profile collection Root must explicitly use `--expected-kernel 6.5.0-rc2-ispass2027-full-e3-v1`, the original DS0 hardware source record, the actual passed common software manifest and a matching new verified boot/load receipt. Collection still requires `--ds 0/0` and a new output directory.

## Preserved protocol and finite verification

The original `parse`, `plan`, `command`, `review` and `validate_receipt` function source bytes are unchanged; their hashes are in the [focused identity receipt](software-profile-host01/identity/validation.json). The exact rotating90-point order, work/roles, taint0, lifecycle/drain/data, raw command, observer/PATH/private/shared accounting, sequence/boot and fixture publication checks remain. Prefixes stay partial; full/pairs still require complete passed90 and ten actual blocks per cell.

| Evidence | Actual result |
| --- | --- |
| [Actual old/new software identity](software-profile-host01/identity/validation.json) | passed42; actual passed manifests, distinct runtime/reference roles, unknown/mixed fields, changed CLI, wrong BIT, hardware-source mismatches, startup fixture refusal and recovery identity refusals |
| [Collector](software-profile-host01/collector.json) | passed30 plus90 explicit synthetic points |
| [Old full](software-profile-host01/full-old-verified/validation.json), [new full](software-profile-host01/full-new-verified/validation.json) | passed25 each; includes E6 public-entry regression and five new profile/helper/runtime/source/software hash refusal cases |
| [Old prefix](software-profile-host01/prefix-old-verified/validation.json), [new prefix](software-profile-host01/prefix-new-verified/validation.json) | passed49 each; original partial/tail/race/fixture gates plus five new identity refusal cases |
| [Old pairs](software-profile-host01/pairs-old-verified/validation.json), [new pairs](software-profile-host01/pairs-new-verified/validation.json) | passed39 each; original complete90/pairing/raw/fixture gates plus five new identity refusal cases |

All campaigns above are explicitly synthetic with zero source board cases. The first shared E6 entry test exposed local helper import resolution when loading the E2 exporter through `importlib`; its actual stderr is preserved in [failure receipt](software-profile-host01/public-import-failure01.json). The E2 exporter now resolves its own local helper directory; E6 source files were not changed. Final full tests passed for both profiles and both public entries. Generated replay trees remain local and ignored; no old checkpoint or result file was overwritten.
