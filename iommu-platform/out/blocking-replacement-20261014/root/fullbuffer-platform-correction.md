# Source-proven fullbuffer mapping correction

2026-10-14, newly found in BR1 legality work; root directly inspected the certificate and its production call chain. No old experiment/model/validator/trace was rerun.

The production certificate is not a general test that a transfer fits a512-beat buffer. `cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:16-18` requires CapacityBytes>=4096, DEFAULT normal copy, length==4096, and both src/dst aligned to a4KiB page. `dma_core_wrap.sv:173-183` samples the actual accepted frontend job and actual backend response; the FPGA wiring passes only DMA0/1 permits to banked TC. `rv_iommu_trans_ctl_wrap.sv:645-647` needs this actual permit plus the matching DID for the read-admission exception.

For an aligned64B copy, the required length predicate is false even though512beats could physically hold it. Its native permit is0 and the ordinary read-admission guard applies. No newly enlarged certificate or extra permission is authorized by BR1.

The old RCS1 engine explicitly gave its64B copies a full-buffer read-admission exemption. That is an additional conditional-model capability; its claimed identity with the existing native certificate/guard was too broad. The32 recorded outcomes, payload/PTE/event integrity checks and conditional fixed-SJF comparison remain unchanged evidence about that engine. They are not established native guarded-platform outcomes. No corrected native performance values are available, and the old outcomes must not be relabeled or replayed to fabricate them.

RCS1's frozen engine did not support its full-benefit claim; that closure is not reversed. A corrected native comparison could have different timings, but is unperformed. This source correction does not restore RCS1, R2, CO1, R107/C112 or any other closed branch; it prevents BR1 from inheriting a free64B guard exception. Historical report body/raw/model files are preserved; a report-front erratum and current plans must explicitly carry this boundary.

The initial source collector also had an out-of-range end770 against the749-line TC file, failed before writing any evidence, and was corrected to749. A later certificate path guess was corrected by a targeted file listing. Neither mechanical correction executed science or changed source.
