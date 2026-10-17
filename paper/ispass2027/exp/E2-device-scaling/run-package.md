# E2 DS0 execution package

Prepared, not run. **Root alone owns the board/JTAG/UART.** Complete the existing old-software experiments and E6 role swap first; then use the one common new P0+multi software payload. E2 keeps the original DS0 BIT. No E2 RTL build, extra Solo, automatic retries, parameter probes or intermediate reboot.

The [fixed90-point plan](multi-plan.json) rotates nine conditions across ten blocks: V DMA0, A DMA1 / DMA1+2 / DMA1+2+3, selected A DMA1. Each A has source/destination pages1,8, or total6 per direction (6/3/2 per A). All points COPY4096×4096, warmup256; same total-footprint controls use the same DMA work. Preserve all failed raw trials and stop; do not silently replace them.

Required existing identities:

- DS0 load receipt with verified current boot/kernel and `bit_sha256=e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6`.
- Original [baseline100 DS0 source record](../../board/packages/baseline100-20261017/source.json) for actual BIT provenance. Its old Linux field is retained as the historical baseline, not presented as the new software identity.
- Completed A2 common software `build-manifest.json`: selected Linux `acece3a3ab7d72ba1f936c1984b82a4a55f28d46`, kernel `6.5.0-rc2-ispass2027-p0-multi-v1`, included `idma_scaling`, matching frozen CLI source hash. Current builder output is `/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/p0-software01/build-manifest.json`.

Use the existing board skill/package update and boot record. The collector compares these existing receipts and does not rehash BIT/payload or add board checks. The prior pathv3 boot receipt cannot describe the new kernel boot.

The plan can be rendered locally without opening UART:

```sh
python3 paper/ispass2027/exp/E2-device-scaling/collect_multi.py --plan-only
```

Root's collection command, after the common payload is loaded and the existing load receipt records the current DS0 boot:

```sh
python3 paper/ispass2027/exp/E2-device-scaling/collect_multi.py \
  --ds 0/0 \
  --expected-kernel 6.5.0-rc2-ispass2027-p0-multi-v1 \
  --load-manifest /absolute/current-ds0-load/load.json \
  --source-record paper/ispass2027/board/packages/baseline100-20261017/source.json \
  --software-manifest /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/p0-software01/build-manifest.json \
  --output /absolute/new-e2-output
```

The collector saves complete raw UART command JSON before parsing, saves each accepted record, then writes a nine-group review with median/min/max and explicit count completeness. CLI failure, observer mismatch, data errors, unfinished drain, taint or boot change stops collection and retains the failure. `--recover-run /absolute/old-output` is only for an already complete, valid prefix on the same boot and identities; it reparses every old command, requires consecutive observer sequences, then runs only the unexecuted suffix. An incomplete or failed raw command refuses recovery. Output directories must be new.

Validation keeps the original private AXI stock/flow and every selected PATH conservation equation. Same-window shared AR minus `vr0.full+vr0.carry+ar0.full+ar0.carry` is the combined unselected shared acceptance count; AW uses vw0/aw0. It is exactly0 with two devices and nonnegative with three/four. Private subtraction remains signed and may be negative due the enabled mux spill. See [source derivation](../../board/observer-shared-counts.md).

V hardware cycles/throughput use the unchanged precise V observer window. A final counts/bytes include startup and drain; per-A exact window completion counts remain N/A. HPM is a sequence of software snapshots, not atomic hardware-window counts; retain real board event values without calling calibration passed. Overlap and PATH refer only to V0/A1. DMA2/3 PATH remains N/A; combined unselected shared traffic does not provide per-device PATH.

[Offline collector checks](multi-collector-host.json) pass29 focused checks plus all90 synthetic campaign records, including every condition, role/byte/drain/active/sequence failures, same-window negative shared rejection, positive unselected totals, and a closing-spill fixture with all stock/flow and stage equations preserved. [Explicit synthetic transcripts](multi-collector-fixtures.json) include that valid private residual−1 case and the rejected tampered shared count. These are protocol fixtures, not board measurements. Software implementation/build evidence is in [multi-driver-batch.md](multi-driver-batch.md).
