## Root cause (confirmed by the supplied log)

`tests/test_nonblock_actual_dma.py` builds `idma_sources()` on top of `sibling.SOURCES` from `tests/test_nonblock_banked.py`, which supplies the **IOMMU vendor** `common_cells` copies:

- `cva6/corev_apu/riscv-iommu/vendor/common_cells/src/fifo_v3.sv` (no `FORCE_FPGA_RAM` pin, no `ariane_pkg`, no `AsyncDpRam`)
- `…/common_cells/src/stream_fifo.sv` (same)

But the production iDMA `idma_stream_fifo.sv:105-110` overrides `.FORCE_FPGA_RAM (Depth == 32'd512 && $bits(type_t) == 8)`. The resolved `stream_fifo` has no such pin → `%Error: …idma_stream_fifo.sv:110:14: Parameter pin not found: 'FORCE_FPGA_RAM'`.

Fix = replace exactly those two modules in the *actual* test's source list with the production `cva6/vendor/pulp-platform/common_cells` ones, add `fpga-support/rtl/AsyncDpRam.sv`, and compile a **test-only** `ariane_pkg` with `FPGA_EN = 1'b0` (matching the production global value) *first*, so `fifo_v3.sv:41` (`ariane_pkg::FPGA_EN || FORCE_FPGA_RAM`) resolves. Nothing else is touched; sibling/component tests and production RTL stay byte-identical.

Net storage behaviour after the repair:
- real 512×8 DMA buffer FIFO → `FORCE_FPGA_RAM == 1` → `gen_fpga_queue` / real `AsyncDpRam`;
- every generic queue → `FORCE_FPGA_RAM == 0` + `FPGA_EN == 0` → `gen_asic_queue`;
- one definition each of `ariane_pkg`, `fifo_v3`, `stream_fifo`, `AsyncDpRam`.

---

## File 1 (new): `tests/nonblock_actual_dma_pkg.sv`

Complete content:

```systemverilog
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// tests/nonblock_actual_dma_pkg.sv
//
// TEST-ONLY ariane_pkg stub for tests/test_nonblock_actual_dma.py.
//
// The production cva6/vendor/pulp-platform/common_cells fifo_v3.sv selects
// its FPGA RAM storage with
//
//     localparam bit UseFpgaRam = ariane_pkg::FPGA_EN || FORCE_FPGA_RAM;
//
// Pulling in the real CVA6 ariane_pkg would drag in the whole SoC
// configuration.  The production global value is FPGA_EN == 0, so this stub
// makes that value explicit: every generic queue (FORCE_FPGA_RAM == 0) stays
// on the non-RAM storage branch, while the real 512 x 8 bit DMA buffer FIFO
// (FORCE_FPGA_RAM == 1) still opts into AsyncDpRam.

package ariane_pkg;
    localparam bit FPGA_EN = 1'b0;
endpackage : ariane_pkg
```

As a unified diff:

```diff
--- /dev/null
+++ b/tests/nonblock_actual_dma_pkg.sv
@@ -0,0 +1,20 @@
+// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
+//
+// tests/nonblock_actual_dma_pkg.sv
+//
+// TEST-ONLY ariane_pkg stub for tests/test_nonblock_actual_dma.py.
+//
+// The production cva6/vendor/pulp-platform/common_cells fifo_v3.sv selects
+// its FPGA RAM storage with
+//
+//     localparam bit UseFpgaRam = ariane_pkg::FPGA_EN || FORCE_FPGA_RAM;
+//
+// Pulling in the real CVA6 ariane_pkg would drag in the whole SoC
+// configuration.  The production global value is FPGA_EN == 0, so this stub
+// makes that value explicit: every generic queue (FORCE_FPGA_RAM == 0) stays
+// on the non-RAM storage branch, while the real 512 x 8 bit DMA buffer FIFO
+// (FORCE_FPGA_RAM == 1) still opts into AsyncDpRam.
+
+package ariane_pkg;
+    localparam bit FPGA_EN = 1'b0;
+endpackage : ariane_pkg
```

(Same `localparam bit FPGA_EN = 1'b0;` form already used at `tests/nonblock_fifo_ram_top.sv:27`, so Verilator 4.038 compatibility is established.)

---

## File 2 (edit): `tests/test_nonblock_actual_dma.py`

Three hunks, ascending, old-line numbers as in the supplied file.

```diff
--- a/tests/test_nonblock_actual_dma.py
+++ b/tests/test_nonblock_actual_dma.py
@@ -12,7 +12,14 @@
 axi packages come from EXACTLY ONE vendor tree (the one that runner already
 uses); the iDMA and register_interface files are appended, never a second
 copy of common_cells/axi.  Two extra include roots are added: the IOMMU
 "include" directory (axi-iommu/*.svh) and the register_interface include
 directory (register_interface/*.svh).
+
+One bounded exception: the two IOMMU vendor FIFO modules that lack the
+FORCE_FPGA_RAM pin are replaced by the production
+cva6/vendor/pulp-platform/common_cells fifo_v3.sv / stream_fifo.sv (plus
+fpga-support/rtl/AsyncDpRam.sv and a TEST-ONLY ariane_pkg with FPGA_EN == 0),
+so the iDMA 512 x 8 bit DMA buffer FIFO still opts into the FPGA RAM storage
+branch while every generic queue stays on the non-RAM branch.
 
 No Vivado, no hardware.  The object directory is independent:
 out/nonblock-actual-dma/obj.
@@ -127,7 +134,36 @@
         REGI / "src" / "axi_to_reg.sv",
         REGI / "src" / "axi_lite_to_reg.sv",
     ]
 
+# The iDMA production stream FIFO selects FPGA RAM storage through the
+# FORCE_FPGA_RAM parameter.  That parameter does not exist in the IOMMU vendor
+# copies of common_cells fifo_v3.sv / stream_fifo.sv (the sibling runner's
+# single common_cells tree), so those two modules are replaced by the
+# production cva6/vendor/pulp-platform/common_cells versions, which also need
+# fpga-support/rtl/AsyncDpRam.sv and ariane_pkg::FPGA_EN.
+IOMMU_CC       = IOMMU / "vendor" / "common_cells" / "src"
+PROD_CC        = CVA6 / "vendor" / "pulp-platform" / "common_cells" / "src"
+PROD_FPGA      = CVA6 / "vendor" / "pulp-platform" / "fpga-support" / "rtl"
+ARIANE_PKG_SRC = ROOT / "tests" / "nonblock_actual_dma_pkg.sv"
+
+# Sibling sources that must NOT be compiled: the IOMMU vendor FIFO family has
+# no FORCE_FPGA_RAM pin, which would break idma_stream_fifo.sv.
+REPLACED_BY_PRODUCTION = {
+    IOMMU_CC / "fifo_v3.sv",
+    IOMMU_CC / "stream_fifo.sv",
+}
+
+
+def production_fifo_sources():
+    # Production FIFO family (FORCE_FPGA_RAM aware) + its RAM primitive.
+    return [
+        PROD_CC   / "fifo_v3.sv",
+        PROD_CC   / "stream_fifo.sv",
+        PROD_FPGA / "AsyncDpRam.sv",
+    ]
+
+
 def idma_incdirs():
     # IOMMU/include carries axi-iommu/*.svh; the register_interface include
     # roots carry register_interface/*.svh.  The single vendored axi/ and
@@ -165,10 +200,17 @@
     sibling = load_sibling()
     tb = ROOT / "tests" / "nonblock_actual_dma_test.cpp"
 
-    # common_cells/axi come from the sibling's tree ONLY (no duplicate vendor).
+    # common_cells/axi come from the sibling's tree ONLY (no duplicate vendor),
+    # except the FIFO family, which is taken from the production tree so that
+    # idma_stream_fifo's FORCE_FPGA_RAM pin resolves and AsyncDpRam is real.
+    # The TEST-ONLY ariane_pkg is compiled FIRST: fifo_v3.sv reads
+    # ariane_pkg::FPGA_EN and Verilator 4.038 needs the package declared first.
     sources = dedup(
-        [require(p) for p in sibling.SOURCES]
+        [require(ARIANE_PKG_SRC)]
+        + [require(p) for p in sibling.SOURCES
+           if p not in REPLACED_BY_PRODUCTION]
         + [require(p) for p in idma_sources()]
+        + [require(p) for p in production_fifo_sources()]
         + [require(ROOT / "tests" / "nonblock_actual_dma_top.sv")]
     )
     incdirs = dedup(list(sibling.INCDIRS) + idma_incdirs())
```

(Blank "context" lines in the diff are ordinary one-space `patch` context lines; the source lines between `]` / blank / `def idma_incdirs():` are unchanged.)

### Resulting source order (single definition per module/package)

1. `tests/nonblock_actual_dma_pkg.sv` — `ariane_pkg` first (required by `fifo_v3.sv`).
2. `sibling.SOURCES` **minus** `riscv-iommu/vendor/common_cells/src/{fifo_v3,stream_fifo}.sv` — all packages, `axi_*`, `counter/lzc/delta_counter/spill_register*/rr_arb_tree/stream_arbiter*`, IOMMU RTL, `tests/nonblock_banked_top.sv`.
3. `idma_sources()` — unchanged iDMA + `axi_to_axi_lite/burst_splitter`, IOMMU-vendor `id_queue/fall_through_register/stream_register/stream_demux/stream_fork`, register_interface prims.
4. `production_fifo_sources()` — `cva6/vendor/pulp-platform/common_cells/src/fifo_v3.sv`, `…/stream_fifo.sv`, `cva6/vendor/pulp-platform/fpga-support/rtl/AsyncDpRam.sv`.
5. `tests/nonblock_actual_dma_top.sv`.

`incdirs` unchanged (`sibling.INCDIRS` + `idma_incdirs()`); the production FIFO files use no `` `include ``.

---

## Dependencies / order of application

1. Create `tests/nonblock_actual_dma_pkg.sv` (exact content above). It must exist before the runner executes — `require(ARIANE_PKG_SRC)` exits if missing.
2. Apply the 3-hunk diff to `tests/test_nonblock_actual_dma.py`.
3. No other file changes. `tests/test_nonblock_banked.py`, `tests/nonblock_fifo_ram_top.sv`, `tests/test_nonblock_fifo_ram.py`, all production RTL, and `tests/nonblock_actual_dma_top.sv` / `tests/nonblock_actual_dma_test.cpp` are untouched (no C++/SV rewrite).

## Acceptance (Codex must execute — worker cannot run)

Compile + N64 run:

```
python3 tests/test_nonblock_actual_dma.py --mode n --transfers 64 --ptw-latency 31
```

Expected: exit 0; `out/nonblock-actual-dma/build.log` contains no `%Error` (verilator emits the model, `out/nonblock-actual-dma/obj/Vnonblock_actual_dma_top` exists, C++ harness returns 0). Optional disabled-mode sanity: `python3 tests/test_nonblock_actual_dma.py --mode s --transfers 1024`.

## Assumptions and residual uncertainty

- `sibling.SOURCES` entries are absolute `Path` objects (they are: all built from `ROOT = Path(__file__).resolve().parents[1]`), so `p not in REPLACED_BY_PRODUCTION` matches the two IOMMU-vendor FIFO files exactly.
- The supplied production `fifo_v3.sv`/`stream_fifo.sv` differ from the IOMMU-vendor copies only by the appended `FORCE_FPGA_RAM` parameter plus the internal `UseFpgaRam`/`AsyncDpRam` branches; therefore any existing named/positional parameter overrides of `fifo_v3`/`stream_fifo` inside the IOMMU RTL, iDMA, and register_interface sources remain valid. If a future build shows a *different* API incompatibility, that would be a separate named replacement — not attempted here.
- No other provided source declares `ariane_pkg`, `fifo_v3`, `stream_fifo`, or `AsyncDpRam`, so the "single definition" requirement holds. If the sibling list ever gains a production-tree copy of these, the filter/dedup set must be revisited.
- `IOMMU_CC` is defined via `IOMMU` (already `CVA6 / "corev_apu" / "riscv-iommu"`), identical to `VENDOR` used by the sibling.
- The log path in the task (`out/composition-fix-20261005/actual-dma-revision-build.log`) is an external copy; the repaired runner writes its own `out/nonblock-actual-dma/build.log`.