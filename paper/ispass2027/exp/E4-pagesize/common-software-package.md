# E4/E5 package using the passed common software

## Material Passport

- Date/stage: 2026-10-18; bounded host-only packaging and reproducibility validation.
- Status: new independent package ready for Root to copy; no board load/boot receipt or board cases created.
- Ownership: Codex-only. No DeepSeek, software/RTL build, FPGA programming, UART or SD operation by this batch.

The exact new profile `full-e3-v1` is registered: Linux `65b427946152e3aca27babab0a437821729805d4`, software platform `ec99648e3c9a1e48c1e9721e3662bb632eb439bd`, software CVA6 reference `1d587125eee690e623e6ffae13e70f9bbe989adc`, OpenSBI `e8d21bfdc2dd753314627968e496c3a1188f6dfb`, release `6.5.0-rc2-ispass2027-full-e3-v1`. The old exact `e4-e5-v1` profile remains accepted. No `46df97c…` intermediate profile was registered.

Actual loaded E4/E5 hardware remains CVA6 `6c5c666d45979d3a03fb98cff89ba7ad095f6125`, IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`, BIT `d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c`, OBS4/INFO1/Depth4/50MHz/DS0. The original FPGA manifest's Linux reference remains in its original source manifest and is not rewritten as the running Linux. Actual runtime and software/FPGA build-reference roles appear in [prepared source](common-software-host01/prepared-source.json).

## Immutable inputs and package interface

The accepted metadata inputs are Root's byte-preserved copies:

| Input | SHA256 |
| --- | --- |
| `E3-workloads/common-v2-software-build-manifest.json` | `7415dcc4d076fa434bc22294f4309582af84086964f1b47bb05c695555f0bcc4` |
| `E3-workloads/common-v2-software-transport.json` | `babd0036df89b9b78191f170c72f7c49729d610959e282619ebc699177944f31` |
| Original E5 `build/completed02/build-manifest.json` | `94fff46f7f87d6687f431f96f570c934a84f0dcc9b3726fd9c36893b611ceb5c` |

`prepare_package.py --build-root` locates existing binaries independently of the metadata-copy directory. It must equal the original delivery-manifest directory recorded by the accepted transport metadata, and that original manifest must equal the provided software manifest byte-for-byte. Without this option, the original manifest-parent default remains. Root's metadata copy directory is deliberately refused as a binary directory.

The new profile pins all three accepted metadata hashes, the seven accepted artifact sizes, matching payload/member hashes, exact two SPI members, member sizes/vermagic, and transport paths/sizes. BIT/Image/payload/gzip/module bytes are hardlinked from their accepted original build/transport paths; binary hashes are reused, not recomputed. The actual new payload is29363656 bytes, Image24130560, gzip11068218, SPI archive8871, modules14008/17640 and E5 BIT11443722. Original software, transport and FPGA manifests are copied byte-for-byte.

The local package is `/home/xvhaocheng/work-a3-pagesize/paper/ispass2027/exp/E4-pagesize/artifacts/full-e3-e4-e5-package01`. Its binaries come from A2 `E3-workloads/artifacts/full-e3-software01` and the original E5 BIT, while metadata inputs came from Root. See the [package receipt](common-software-host01/package-receipt.json) and [prepared manifest](common-software-host01/prepared-manifest.json). Root will copy metadata and hardlink binaries into a new Root board package; collection/export must use those Root-local paths, rather than cross this A3 source root. The old `artifacts/e4-e5-package01`, old `pages-package-*.json` and all old raw evidence remain unchanged.

## Actual finite evidence

| Receipt | Actual result |
| --- | --- |
| [New package/profile validation](common-software-host01/validation.json) | passed22; actual accepted preparation, wrong default/explicit root, unknown profile, software/FPGA metadata changes, payload/gzip/archive/member hash and size/release/member-set refusals, software/hardware role-mixing refusals, check-only and fixture-start refusal before UART/output |
| [New-profile export](common-software-host01/export-new/validation.json) | passed31, full synthetic20 and all original raw/identity/leaf/histogram/public-fixture gates |
| [Old-profile export](common-software-host01/export-old/validation.json) | passed31, full synthetic20 using the same frozen protocol |
| [Old identity](common-software-host01/legacy-identity.json) | passed14 |
| [Old package/profile regression](software-profile-host04/validation.json) | passed20; original default package interface still works |

These fixtures have zero source board cases. No runtime parser, plan, command, leaf/PTE/PA/lifecycle, taint, PATH/private/shared accounting, OBS4/INFO1 or histogram rule changed. The collector/full export still bind exact profiles and current helper hashes; the exporter fixture harness now derives software/kernel/reference metadata from either actual accepted manifest while keeping its20-point raw protocol checks.
