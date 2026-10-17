# PN compiled AXI source boundary

Root source check2026-10-18 JST, no build performed. FixedCVA6
8aac3fcd345af7686a55af1867783fd6b91a0069 Makefile228–230 compiles
`vendor/pulp-platform/axi/src/axi_demux_simple.sv` and `axi_demux.sv`.
It does not compile the corresponding `corev_apu/riscv-iommu/vendor/axi` copies.
`git ls-tree` proves the former is a flattened normal tree inCVA6, whereas the
latter is IOMMU's actual vendor gitlink e72230ef6c8cd24387b788b69d02c10174decd45.

Root compared actual fixed Git bytes directly with the locked vendor original:

| File | SHA256 in both original copies |
| --- | --- |
| axi_demux.sv | 3c165a36d2efd88e0714d2e20dcea9375c0e211b0091252e6b082b884b9fd23b |
| axi_demux_simple.sv | 0e25cd8fd1a406bbaf7c941f876a530f10016f445301d836abe14b3bf7784781 |

After the independent AXI readonly export batch is accepted, finalCVA6 component
integration must copy exactly those two accepted committed vendor files into
its compiled flattened paths and record that source commit/SHA in the existing
source/build manifest. Keep the IOMMU actual vendor commit/pin and incremental
bundle too. Do not edit only a build export, compile duplicate module sets,
rewrite unrelated vendor files, upgrade dependencies or claim the unused copy
was the compiled source. This is pending final integration, not a claim that
new vendor code has already passed or that CVA6 has been modified. Default-off
extra outputs preserve other original callers of the shared module definitions.
