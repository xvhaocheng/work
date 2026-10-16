# FIRST-PTE-PAIRED-LOCAL-1 runner

This directory contains an isolated derivative of the actual-DMA SYSTEM C++
fixture and an independent build entry. Only `run_paired()` is the active
entry. The inherited DDR queue service, AR latency/return order, memory model,
real AXI register-master access and DMA scoreboards are retained. No new
memory service model, production source edit, old runner invocation or board
build is performed.

## Fixed software program

Each cold executable invocation starts from reset, quiet and populated memory.
Each hot invocation additionally executes exactly one normal batch-OFF A COPY
and one normal batch-OFF V COPY (same fixed A-first sequence) and drains both.
The warm history uses the static policy being tested, with no cache reset or
M escape/reset before measurement. Its cycles are separately reported. At
quiet, both destination pages are poisoned with their original distinct
sentinels so warm-written data cannot satisfy the measured data check. Sources,
page tables, contexts and effective DMA IOVA/PA mappings stay unchanged.

Measured `t0` is captured at quiet **before** configuring either DMA. A is
configured, V is configured, A NEXT_ID is read, V NEXT_ID is read. No software
delay is inserted. Each configured request has `CONF=0`, `NB=4096`, aligned
source and destination, and batch remains disabled. Both exact returned IDs
are polled to DONE; the queues then drain to protocol quiet. The complete MMIO
configuration/start prefix is included in T_V/T_A/T_data/T_return. The common
DS50/50 and DS-first/F4 shared selector are supplied by the parent's overlay;
the old C++ `ds_priority` queue rotation stays disabled. CPU/write/AMO/admission
proxy and all old G0 controls stay disabled.

Per lane the runner checks one hardware issue/DONE, one timestamp entry,
4096 read bytes, 512 returned 64-bit R beats, 4096 write bytes, **two original
256-beat write AWs and two Bs**, response conservation, exact DONE ID, batch
OFF and byte equality/source immutability. It also checks total history+measure
issue/DONE counts and all queues empty at quiet. Read child counts remain
observable rather than fixed because O changes read length. DDR AR/AW must
remain 64-bit INCR and in frozen production DRAM `[0x80000000,0xc0000000)`.

This is local RTL with behavioral DDR. The MMIO path is the fixture's genuine
register-master FSM and real DMA frontend, but bypasses the board CPU/fabric
configuration path. No CPU, real DDR controller, FPGA timing or native program
claim follows from these results.

## Relocated tables

The C++ copy uses these physical roots. The parent must apply the matching SV
DDTP and DDR decode changes; unchanged SV defaults do not satisfy this copy.

| Object | PA |
| --- | --- |
| DDT | `0xA0010000` (`ddtp.ppn = 0xA0010`) |
| V S1 root / L1 / L0 | `0xA0020000` / `0xA0040000` / `0xA0050000` |
| A S1 root / L1 / L0 | `0xA0030000` / `0xA0060000` / `0xA0070000` |

The context SATP words are `0x80000000000A0020` and
`0x80000000000A0030`. All nonleaf PTE PPNs are relocated consistently. Every
old payload IOVA/PA is retained (`V: 0x10000000/0x10001000 ->
0x80000000/0x80001000`; `A: 0x20000000/0x20001000 ->
0x90000000/0x90100000`). No table aliases these payloads or CPU scratch.

## Independent build

`--overlay` mirrors paths relative to `iommu-platform/`, such as
`tests/nonblock_actual_dma_system_top.sv` and
`cva6/vendor/pulp-platform/axi/src/axi_mux.sv`. Overlay files replace only their
matching source; remaining input files are read from the workspace.
`tests/m_first_pte_gate.sv` is automatically added when present. Additional
modules can be listed as relative strings in `sources-extra.json` or passed
with repeated `--extra-sv` (must reside under the overlay).

The source lists are imported from the two existing Python helpers without
calling their `main()` functions. Every IOMMU-vendor AXI/common_cells path and
include directory is converted to its CVA6 production-vendor counterpart;
there is one module/package version per source. The ariane test package,
production FPGA-aware FIFO family and AsyncDpRam are retained. The same CVA6
common_cells revision also requires `src/deprecated/fifo_v2.sv` for its
stream/fall-through registers; this adapter is explicitly included and depends
only on the already included `fifo_v3`. This avoids the
old fixture's mixed IOMMU-vendor/CVA6-vendor source list. The complete ordered
source/hash manifest and exact argument vector are saved per build.

```sh
python3 iommu-platform/out/first-pte-paired-local-20261016/runner/build.py \
  --overlay iommu-platform/out/first-pte-paired-local-20261016/combined/overlay \
  --out iommu-platform/out/first-pte-paired-local-20261016/build-policy0 \
  --policy 0
```

The entry only builds. `--policy 0..4` sets static SV `PairPolicy` and the C++
label `PAIRED_POLICY`: base / M / O-tail-local / O-tail-prepared / O-job128.
All builds use `PolicyEnable=1`; C++ drives nonblock and partition enabled.
Use `--dry-run` to prepare manifests without running Verilator, `--lint-only`
for RTL elaboration, and `--cpp PATH` to build a parent copy with added taps.
The parent runs cold/hot once per approved static policy, with alive/timeout
monitoring, for example:

```sh
iommu-platform/out/first-pte-paired-local-20261016/build-policy0/obj/Vnonblock_actual_dma_system_top \
  +history=cold +policy=0 +event-trace=/absolute/path/to/approved/new/cold.csv
```

Only `+history=cold|hot`, `+policy=<compiled policy>` and `+event-trace=PATH`
are accepted. No launch phase/length/delay or service sweep is exposed. The
machine-readable terminal record begins `FIRST-PTE-PAIRED-JSON`; all its cycle
arrays use absolute fixture cycles, while the trace CSV uses the measured t0
(the explicit warm `history_cfg/next` entries use their own history epoch).

## Root trace callback

`paired_pre_edge()` is called once per `tick()` immediately after
`top->clk_i=0; top->eval()` and before the rising edge. It may inspect added
outputs without changing the DUT. The `ROOT_TRACE_CALLBACK` comment marks the
insertion point. `full_era` distinguishes measured observations;
`paired_epoch_name` distinguishes setup/history/measurement; `full_t0` and
`paired_epoch_t0` give the relevant epoch. Root should copy `paired_dma.cpp`
to its own combined area and pass `--cpp` rather than editing/regenerating the
owned copy. `prepare_cpp.py` regenerates this original derived copy and
updates its hash provenance; it would overwrite local changes to that copy.

The existing full-era trace emits true raw hardware issue/done, DDR and PTW
activity. The explicit cfg/next event names start with `history_` or `measurement_`
and use the corresponding epoch. Main result accounting is independent
of the legacy `win_open` interval.

## Checks actually executed here

- Python syntax check and source-manifest dry run: passed (83 inputs before
  parent adds policy modules/overlays).
- C++17 syntax-only check against an existing generated SYSTEM interface:
  passed; no old executable was run.
- No integrated Verilator build, RTL run or synthesis performed by this seat;
  these are owned by root and cannot be inferred from the checks above.
- Read-only tool lookup: `/usr/bin/verilator` available;
  `/tools/Xilinx/Vivado/2023.1/bin/vivado` and `settings64.sh` present;
  `yosys` absent from PATH and standard `/usr/bin`/`/usr/local/bin` entries.
  Vivado license, standalone synthesis success and cost numbers are unknown.
