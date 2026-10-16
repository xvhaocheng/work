## 0. Bottom line

- **Linux payload:** build with `scripts/build_update_software.py` using a distinct `--output-dir` and `--local-version`. It runs `platform.kernel()` + `platform.firmware()`, captures `linux/drivers/misc/idma.c`, skips pinned `check()`, and never runs Vivado/fpga.  
- **Bitstream:** build with `scripts/platform.py fpga --profile migration-4dma-composition --fpga-stage ...`. If the legacy-debug cleanup is only in the dirty working tree, `--workspace-overlay` is the intended bypass — but `platform.py`’s overlay cannot express **deletions**. That is a genuine missing interface if `hang_snapshot.sv` / `hang_snapshot.xdc` must be removed from the staged cva6 tree.  
- **Mac update path for old healthy V8 → trimmed V9:** use `scripts/mac_update_reboot.py --transport ssh` with `--payload <new fw_payload.bin>`, `--bit <trimmed V9 BIT>`, and `--expected-kernel "$(cat .../kernel-release)"`. This does board-side HTTP fetch → write raw 32 MiB SD p1 → `sync` → JTAG reload → SSH identity verification.  
- **Experiments:** `mac_update_reboot.py` is **boot/update-only**. It never runs experiments. Bounded P/PN causal validation needs a separate experiment runner/plan; it is not automatic here.  
- **Final timing/routing gates:** `scripts/finalize_research2_postroute.py` is the host-side finalizer found, but it is specifically for `out/two-bit-20261003/method-direct-arm-postroute` role-2 post-route optimization. Exact routed-timing thresholds are in `scripts/package_research2.py`, which was not supplied.

---

## 1. Authorized evidence map

| Topic | File | Key lines |
|---|---|---|
| Software build harness | `scripts/build_update_software.py` | 5–12, 25–31, 54–65, 67–101, 200–223, 226–237, 240–289 |
| Mac update/reboot runner | `scripts/mac_update_reboot.py` | 3–65, 92–126, 190–231, 447–501, 670–696, 756–788, 790–820, 837–895, 898–930, 962–1031 |
| Platform/fpga/finalizer entry | `scripts/platform.py` | 176–205, 228–297, 339–359, 361–414 |
| Post-route finalizer | `scripts/finalize_research2_postroute.py` | 27–37, 63–109, 181–211, 214–321, 324–343 |
| net3 Mac update example | `docs/software-update-20261004.md` | 137–199 |
| Current/old composition Mac entry | `docs/composition-v1-mac-20261004.md` | 3–13, 30–44, 58–69, 106–119, 186–218 |
| Cleanup acceptance state | `out/debug-cleanup-20261006/acceptance.json` | 48–58, 61–73 |

---

## 2. Step 1 — package Linux with V9 `idma.c`

### 2.1 Command

Run from the repository root that contains `scripts/` and `configs/`:

```sh
cd /path/to/iommu-platform

python3 scripts/build_update_software.py \
  --output-dir=out/software-update-trimv9-20261006 \
  --local-version=-cpn2-trimv9
```

### 2.2 Why this is the correct existing path

- `--output-dir` and `--local-version` are the only build-software options.  
  Evidence: `scripts/build_update_software.py:240–246`.
- `LOCALVERSION` is set into `platform.ENV` before the kernel build.  
  Evidence: `scripts/build_update_software.py:264–265`.
- The script calls `platform.kernel(JOBS)` then `platform.firmware(JOBS)`.  
  Evidence: `scripts/build_update_software.py:272–273`.
- It deliberately does **not** run `check()` and does **not** run `fpga()`/Vivado.  
  Evidence: `scripts/build_update_software.py:5–7, 255–256`.
- `linux/drivers/misc/idma.c` is explicitly captured in the input manifest.  
  Evidence: `scripts/build_update_software.py:70–73`.
- Network configs are preserved and delivered unchanged:
  - `configs/board-network.interfaces`
  - `configs/board-dhcpcd.conf`  
  Evidence: `scripts/build_update_software.py:63–64, 90–93`.

The underlying kernel/firmware build is `platform.kernel()` / `platform.firmware()`.  
Evidence: `scripts/platform.py:176–205`.

### 2.3 Outputs

Default base becomes:

```text
out/software-update-trimv9-20261006/
  software-update-state.json
  delivery/
    Image
    platform.dtb
    rootfs.cpio
    fw_payload.bin
    platform-sd-update
    linux.config
    kernel-release
    mac_update_reboot.py
    board-network.interfaces
    board-dhcpcd.conf
```

Evidence: `scripts/build_update_software.py:25–28, 54–65, 121–126, 227–237`.

### 2.4 Acceptance gates

The script fails closed on:

1. Required kernel configs must equal `y`:
   - `CONFIG_RISCV`
   - `CONFIG_BLK_DEV_INITRD`
   - `CONFIG_SPI_XILINX`
   - `CONFIG_MMC`
   - `CONFIG_MMC_BLOCK`
   - `CONFIG_MMC_SPI`
   - `CONFIG_LOWRISC_ETH`
   - `CONFIG_PHYLIB`
   - `CONFIG_MDIO_BITBANG`
   - `CONFIG_REALTEK_PHY`  
   Evidence: `scripts/build_update_software.py:103–114, 200–204`.

2. `CONFIG_INITRAMFS_SOURCE` must equal `"<ROOT>/out/rootfs.cpio"`.  
   Evidence: `scripts/build_update_software.py:205–209`.

3. `fw_payload.bin` must exist, be non-empty, and be `<= 33554432` bytes.  
   Evidence: `scripts/build_update_software.py:217–223`.

4. Every delivery artifact must exist and be non-empty.  
   Evidence: `scripts/build_update_software.py:230–232`.

5. Final state must be `status == "complete"` and `exit_code == 0`.  
   Evidence: `scripts/build_update_software.py:280–281, 287–289`.

### 2.5 Hash / final artifact inspection

The state file is the manifest. It stores per-artifact `size` and `sha256`.  
Evidence: `scripts/build_update_software.py:235–236, 278`.

Read it directly:

```sh
cat out/software-update-trimv9-20261006/software-update-state.json
cat out/software-update-trimv9-20261006/delivery/kernel-release
shasum -a 256 out/software-update-trimv9-20261006/delivery/*
```

Do **not** hard-code `uname -r`. Use `delivery/kernel-release` as the source of truth.  
Evidence: docs/software-update-20261004.md:24, 150.

**Uncertainty:** the build script does not create a `.tar.gz`. The docs mention a tar archive, but no tar step exists in `scripts/build_update_software.py`. If tar packaging is required, that is a missing next step/file.

---

## 3. Step 2 — build full trimmed composition BIT

### 3.1 Intended platform action

The platform supports profile `migration-4dma-composition` and action `fpga`.  
Evidence: `scripts/platform.py:364–365, 409`.

Fresh stage command:

```sh
python3 scripts/platform.py fpga \
  --profile migration-4dma-composition \
  --fpga-stage out/trimv9-20261006/fpga-src
```

This is the cleanest “output-tag” option: the stage sits under `out/trimv9-20261006/fpga-src`, avoiding an existing `out/migration-4dma-composition/fpga-src`.  
Evidence: `scripts/platform.py:234–246`.

Resulting BIT path:

```text
out/trimv9-20261006/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit
```

Evidence: `scripts/platform.py:234–235, 245, 294–297, 345`.

### 3.2 What `platform.py fpga` enforces

`fpga()` validates:

- `cfg['num_dma'] == 4` and `cfg['device_ids'] == [10,11,12,13]`.
- `cfg['board'] == 'genesys2'`.
- `cfg['cpu_hz'] == 50000000`.
- `cfg['timebase_hz'] == 25000000`.
- `research_role` in `{0,1,2}`.
- `nonblock_mode` in `{0,1,2,3}`.
- `nonblock_mode == 3` only for `migration-4dma-composition`.
- For nonblock profiles: `legacy_e3_observer == false`, `research_role == 0`, `ds_delay_input == 50`, `ds_delay_output == 50`, `iotlb_entries == 16`.

Evidence: `scripts/platform.py:228–269`.

After Vivado, it checks:

```text
ariane_xilinx.bit
ariane_xilinx.mcs
```

must be present and non-empty.  
Evidence: `scripts/platform.py:294–297`.

Manifest is written to:

```text
out/manifests/fpga-migration-4dma-composition.json
```

Evidence: `scripts/platform.py:339–359`.

### 3.3 Dirty-tree / legacy-debug-cleanup problem

If the legacy debug wiring cleanup is only in the dirty working tree, `platform.py fpga` will call `check()` and fail on dirty components.  
Evidence: `scripts/platform.py:401`; `check()` at `scripts/platform.py:116–137`.

The intended bypass is:

```sh
python3 scripts/platform.py fpga \
  --profile migration-4dma-composition \
  --fpga-stage out/trimv9-20261006/fpga-src \
  --workspace-overlay <existing-overlay.json>
```

`--workspace-overlay` is valid only with `fpga`, only for the migrated nonblock profiles, and must be a non-empty JSON list of `cva6/...` paths.  
Evidence: `scripts/platform.py:366, 372–385`.

Overlay validation:

- POSIX separators.
- No empty / `.` / `..` segments.
- Must start with `cva6/`.
- Must not reference `linux`.
- No duplicates.
- Source must be a regular file.
- No symlinks.
- No NUL bytes.
- UTF-8 text.

Evidence: `scripts/platform.py:37–80`.

The overlay copies files into the staged tree.  
Evidence: `scripts/platform.py:82–111`.

### 3.4 Genuine missing interface: overlay cannot express deletions

The cleanup acceptance records deleted files:

```text
cva6/corev_apu/fpga/src/hang_snapshot.sv
cva6/corev_apu/fpga/constraints/hang_snapshot.xdc
tests/hang_snapshot_cdc_tb.sv
```

Evidence: `out/debug-cleanup-20261006/acceptance.json:48–52`.

But `platform.py`’s workspace overlay only copies/overwrites files. It has no deletion operation.  
Evidence: `scripts/platform.py:82–111`.

Therefore, if those files must be absent from the staged cva6 tree, the existing `--workspace-overlay` workflow is insufficient. Root must either:

- commit the cleanup into `cva6` and update `sources.lock.json` so `check()` passes, or
- extend/modify the staging workflow to support deletions.

This is a genuine missing interface; no code edit is proposed here.

### 3.5 Final timing/routing gates after full build

The only finalizer in the authorized set is:

```sh
python3 scripts/finalize_research2_postroute.py
```

Defaults:

```text
repo root: parent of scripts/
optimization dir: out/two-bit-20261003/method-direct-arm-postroute
```

Evidence: `scripts/finalize_research2_postroute.py:27–30, 328–331`.

It requires:

```text
input.json
execution.json
optimized.dcp
ariane_xilinx.bit
ariane_xilinx.mcs
utilization.rpt
timing.rpt
```

Evidence: `scripts/finalize_research2_postroute.py:27–30, 184–188`.

It parses optimized timing and calls:

```python
pkg.validate_routed_timing(...)
```

Evidence: `scripts/finalize_research2_postroute.py:204–211`.

It writes `manifest.json` only if every routed-timing gate passes.  
Evidence: `scripts/finalize_research2_postroute.py:226–228, 241–313`.

**Uncertainties:**

- Exact numeric WNS/WHS/WPWS thresholds are in `scripts/package_research2.py`, not supplied.
- The finalizer is specific to research2 role 2 and the `out/two-bit-20261003/method-direct-arm-postroute` directory. If the trimmed composition fullbuild produces a different postroute directory, pass `--optimization-dir` or use the correct finalizer for that build.
- It does not itself run Vivado. It only validates already-produced outputs.

---

## 4. Step 3 — Mac pure-Ethernet old healthy V8 → trimmed V9

### 4.1 Assumptions

- Old V8 board is healthy, boots Linux, and is reachable by SSH.
- Mac host IP: `192.168.77.1`.
- Board IP: `192.168.77.2`.
- JTAG is free; only one JTAG user.
- UART is not needed for `--transport ssh`; the runner never opens the serial port.  
  Evidence: `scripts/mac_update_reboot.py:34–35, 670–671, 952`.
- Board-side `platform-fetch-update` and `platform-sd-update` exist.
  - `platform-sd-update` is delivered by the software build.  
    Evidence: `scripts/build_update_software.py:59`.
  - `platform-fetch-update` is invoked by the runner, but is **not** in `DELIVERY_SOURCES`.  
    Evidence: `scripts/mac_update_reboot.py:770`; `scripts/build_update_software.py:54–65`.
    This is a dependency/uncertainty: confirm it is in the rootfs or already on the board.

### 4.2 Required local files

| Item | Path / note |
|---|---|
| Runner | `scripts/mac_update_reboot.py` |
| Credential script | `scripts/run_research2_mac.py` — must contain exactly one `_write(bytes)` call |
| New payload | `out/software-update-trimv9-20261006/delivery/fw_payload.bin` |
| New kernel release | `out/software-update-trimv9-20261006/delivery/kernel-release` |
| Trimmed V9 BIT | `out/trimv9-20261006/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit` |
| JTAG loader | `openFPGALoader` on PATH or `/opt/homebrew/bin/openFPGALoader` |
| Python | Docs use `tools/method88-venv/bin/python` |

Evidence: `scripts/mac_update_reboot.py:190–199, 202–231, 898–930`; docs/software-update-20261004.md:163, 167–172.

### 4.3 Command

```sh
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'

tools/method88-venv/bin/python scripts/mac_update_reboot.py \
  --run --transport ssh \
  --credential-script scripts/run_research2_mac.py \
  --board-host 192.168.77.2 \
  --host-ip 192.168.77.1 \
  --payload out/software-update-trimv9-20261006/delivery/fw_payload.bin \
  --bit out/trimv9-20261006/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit \
  --expected-kernel "$(cat out/software-update-trimv9-20261006/delivery/kernel-release)" \
  --out "results/trimv9-$(date +%Y%m%d-%H%M%S)" \
  --boot-timeout 900 \
  --update-timeout 3600 \
  --jtag-hz 1000000
```

### 4.4 Flag map

| Flag | Meaning | Evidence |
|---|---|---|
| `--run` | Actually execute; default is dry-run | `scripts/mac_update_reboot.py:902–903, 966–967` |
| `--transport ssh` | Pure Ethernet; serial not used | `scripts/mac_update_reboot.py:904–905, 952` |
| `--credential-script` | AST-extract `_write(bytes)`; never execute script | `scripts/mac_update_reboot.py:202–231, 916–918` |
| `--board-host` | SSH target; required for ssh | `scripts/mac_update_reboot.py:914–915, 988–992` |
| `--host-ip` | Mac IP for temporary HTTP server | `scripts/mac_update_reboot.py:912–913, 988–992` |
| `--payload` | Local `fw_payload.bin` served over HTTP; board runs `platform-fetch-update <url> --write` | `scripts/mac_update_reboot.py:756–773, 870–872, 910–911` |
| `--bit` | Local BIT; required, non-empty | `scripts/mac_update_reboot.py:974–984` |
| `--expected-kernel` | Exact post-reboot `uname -r` | `scripts/mac_update_reboot.py:976–977, 645–650, 808–816` |
| `--out` | Fresh output dir; must not exist | `scripts/mac_update_reboot.py:985–987` |
| `--boot-timeout` | Post-reboot SSH wait | `scripts/mac_update_reboot.py:928` |
| `--update-timeout` | Board fetch/sync timeout | `scripts/mac_update_reboot.py:924–926` |
| `--jtag-hz` | openFPGALoader JTAG clock | `scripts/mac_update_reboot.py:922–923, 779–788` |

### 4.5 Flow and safety

The SSH runner does:

1. Pre-reboot SSH identity capture.  
   Evidence: `scripts/mac_update_reboot.py:865–869`.

2. If `--payload` is given:
   - start temporary read-only HTTP server bound to `--host-ip`;
   - board runs `platform-fetch-update <url> --write`.  
   Evidence: `scripts/mac_update_reboot.py:756–773`.

3. Board `sync`.  
   Evidence: `scripts/mac_update_reboot.py:775–777`.

4. Local `openFPGALoader -b genesys2 --freq <jtag_hz> <bit>`.  
   Evidence: `scripts/mac_update_reboot.py:779–788`.

5. Wait for SSH identity and verify:
   - post-reboot `uname -r == --expected-kernel`;
   - canonical `boot_id` before and after;
   - `boot_id` changed after reboot.  
   Evidence: `scripts/mac_update_reboot.py:790–820, 855–859, 645–650`.

It writes `result.json` at each phase (`running`, `pre_identity`, `downloading_and_writing`, `syncing`, `loading_bit`, `waiting_for_network`, `complete`/`failed`).  
Evidence: `scripts/mac_update_reboot.py:822–825, 891–895`.

It does **not**:

- write the whole SD card;
- write SD directly from the Mac;
- run experiments;
- auto-retry.

The board-side raw 32 MiB p1 write is done by `platform-sd-update`.  
Evidence: docs/software-update-20261004.md:13–20, 85; delivery source at `scripts/build_update_software.py:59`.

### 4.6 Hash caveat

The Mac runner invokes:

```text
platform-fetch-update <url> --write
```

Evidence: `scripts/mac_update_reboot.py:770`.

The docs state `--write` re-downloads and skips the manual hash comparison, so the explicit two-stage manual method is the hash-checked path.  
Evidence: docs/software-update-20261004.md:22, 70–87.

Therefore:

- The software build records SHA256 in `software-update-state.json`.  
  Evidence: `scripts/build_update_software.py:235–236, 278`.
- The Mac SSH runner itself does **not** perform an independent payload SHA256 check before write.
- If a payload-hash gate is mandatory, use the manual two-stage method or confirm that `platform-fetch-update --write` performs it board-side. This is an uncertainty.

---

## 5. Experiments / bounded P/PN causal validation

`scripts/mac_update_reboot.py` is **boot/update-only**. It has no experiment calls. Its phases are identity, optional fetch/write, sync, BIT load, and post-boot identity.  
Evidence: `scripts/mac_update_reboot.py:34–51, 837–895`.

Therefore:

- It does **not** automatically run P/PN.
- It does **not** run the 172 matrix.
- It can reboot and verify the new kernel/BIT only.

Bounded P/PN causal validation requires a separate experiment runner/plan. The authorized docs show only the full composition runner path:

```sh
tools/method88-venv/bin/python tools/composition-v1-20261004/mac_composition.py \
  --run --transport ssh \
  --board-host 192.168.77.2 \
  --credential-script scripts/run_research2_mac.py \
  --ssh-helper scripts/mac_update_reboot.py \
  --through-phase explore \
  --expected-kernel "<actual kernel-release>" \
  --attach-timeout 60 \
  --out "results/composition-trimv9-$(date +%Y%m%d-%H%M%S)"
```

Evidence: docs/composition-v1-mac-20261004.md:61–69, 139–147.

But `--through-phase explore` is the full 172 path. The task explicitly says not to repeat the old 172 matrix. So a **bounded** P/PN plan/driver is not present in the authorized files. That is a missing next file/interface if bounded P/PN is required.

Do **not** claim the update runner runs experiments. It does not.

---

## 6. Uncertainties and only-missing-next-file suggestions

| Missing / uncertain item | Why it matters | Suggested next file |
|---|---|---|
| Exact routed timing gates | `finalize_research2_postroute.py` calls `pkg.validate_routed_timing(...)` but thresholds are not shown | `scripts/package_research2.py` |
| Board-side `platform-fetch-update` | Runner calls it, but it is not in `DELIVERY_SOURCES` | `scripts/rootfs.py`, `tests/board-fetch-update.sh` |
| Credential script shape | Runner requires exactly one `_write(bytes)` call | `scripts/run_research2_mac.py` |
| Nonblock profile config | `platform.py fpga` requires `nonblock_mode=3`, `legacy_e3_observer=false`, `research_role=0` | `configs/migration-4dma-composition.json` |
| Dirty cva6 cleanup | `check()` fails on dirty components; overlay cannot express deletions | `sources.lock.json`, cva6 git state, existing overlay JSON |
| Fullbuild finalizer applicability | `finalize_research2_postroute.py` is research2 role-2 specific | correct finalizer for trimmed composition fullbuild, if different |
| Tar packaging | Docs mention `software-update-20261004.tar.gz`, but the build script does not tar | tar/package step |
| Bounded P/PN | Update runner is boot-only; bounded plan/driver not in authorized files | bounded composition plan/runner |
| Payload hash before SD write | Mac runner uses `platform-fetch-update --write`, which docs say skips manual hash | board-side fetch tool or manual two-stage procedure |
| JTAG exclusivity / loader presence | Runner requires `openFPGALoader` and exclusive JTAG | local tool state, no file |

---

## 7. Recommended safe sequence for root

1. Ensure V9 `linux/drivers/misc/idma.c` is in the working tree.
2. Build software:

```sh
python3 scripts/build_update_software.py \
  --output-dir=out/software-update-trimv9-20261006 \
  --local-version=-cpn2-trimv9
```

3. Verify state and kernel release:

```sh
cat out/software-update-trimv9-20261006/software-update-state.json
cat out/software-update-trimv9-20261006/delivery/kernel-release
shasum -a 256 out/software-update-trimv9-20261006/delivery/*
```

4. Build BIT with a fresh stage, after resolving the dirty-tree/overlay/deletion issue:

```sh
python3 scripts/platform.py fpga \
  --profile migration-4dma-composition \
  --fpga-stage out/trimv9-20261006/fpga-src
```

5. Run Mac pure-Ethernet update/reload from old healthy V8:

```sh
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
tools/method88-venv/bin/python scripts/mac_update_reboot.py \
  --run --transport ssh \
  --credential-script scripts/run_research2_mac.py \
  --board-host 192.168.77.2 \
  --host-ip 192.168.77.1 \
  --payload out/software-update-trimv9-20261006/delivery/fw_payload.bin \
  --bit out/trimv9-20261006/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit \
  --expected-kernel "$(cat out/software-update-trimv9-20261006/delivery/kernel-release)" \
  --out "results/trimv9-$(date +%Y%m%d-%H%M%S)" \
  --boot-timeout 900 \
  --update-timeout 3600 \
  --jtag-hz 1000000
```

6. Treat success only as:

```text
result.json status == complete
post-reboot uname -r == expected
boot_id canonical and changed
```

No experiment/P/PN result is produced by this update runner.
