# Accepted scope correction: original deferred DDTP state

Root directly checked original regmap lines3198–3289 and busy field645–683,
then accepted one additional passive file. The original nine-file proposal,
source-map.json and source-verification.json remain as-read historical evidence.
Current scope is ten files; source-map-v2.json locks their original85b/5b7 bytes.

`rv_iommu_regmap` appends PnEnable0 and pn_idle_o. Defaultoff is constant0;
enabled is exactly `rst_ni && !write_l_q && !write_h_q && !reg_req_i.valid`.
SW passes PnEnable and ANDs this real child output. No field, busy, state,
READY/VALID, write deferral or in_flight behavior changes. No hierarchical
production reference or shadow FF is added.

A real program low32 or high32 DDTP write with original in_flight1 may return B
before commit. Bridge/current-reg-valid can be0 while pending_q remains1.
When in_flight first falls0, the next edge commits the deferred write; both
before and during that edge eligibility must remain0. After the original
pending flag clears, quiet may recover if every other SW debt is empty.
The actual complete SW/program dual-gold finite must cover both halves and
these pre/post edges through public AXI CSR inputs, without forcing state.

Status: implementation/test preparation; no finite pass is claimed here.
