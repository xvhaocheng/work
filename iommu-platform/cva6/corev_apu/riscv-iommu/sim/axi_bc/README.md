# AXI 4 KiB boundary regression

Run from the IOMMU repository:

```sh
python3 sim/axi_bc/run.py
python3 sim/axi_bc/run.py --focus fixed-tail,incr-cross-half
python3 sim/axi_bc/run.py --source /absolute/path/rv_iommu_axi_bc.before.sv --out /tmp/axi-bc-before
```

Requires Python 3, a C++ compiler and Verilator (tested with 4.038). Default
outputs go to a fresh temporary directory; `--out` selects a retained build.
The runner saves `build.log` and `run.log` and propagates failure status.

The real combinational RTL is wrapped with a 64-bit address. The reference
walks individual AXI beats and compares the page of each beat's first and
last accessed bytes. It does not reuse the DUT's end-of-burst calculation.
The addressing rules follow Arm IHI 0022H sections A3.4.1 and A3.4.2:
https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/IHI0022H_amba_axi_protocol_spec.pdf

The default 816,064 checks cover every page offset, AxSIZE 0..7, FIXED/INCR/WRAP,
AxLEN {0,1,3,7,15,31,127,255}, disabled checks, reserved bursts, invalid WRAP
lengths/alignment, and sampled addresses with bit 40 set. This is boundary
checking, not a complete AXI protocol validator; FIXED length limits and bus
width legality are outside its contract. It does not exhaust every AxLEN.

Named cases include the independently reported page-end error: FIXED at
0xFF8, size 3, length 0 must allow bytes 0xFF8..0xFFF. A distinct INCR case
at 0xFF0, size 3, length 2 must reject a three-beat burst that crosses the
page; the old half-length formula incorrectly allows it. A one-byte page-tail
access and an unaligned first beat must also be accepted.

On 2026-10-12, HEAD 238a39e66a07b27a67fb5749df40691682555027 failed 112,514
of 816,064 checks, including the two named cases above. The final repair
passed all 816,064 checks and all five named cases. The repair also avoids
3-bit exponent-sum overflow for larger WRAP windows. The old RTL passes
the named one-byte case on this Verilator version; no universal size-zero
failure is claimed.

These results are standalone boundary-checker simulation, not DMA payload,
transaction-controller integration, FPGA timing, or board validation.
DeepSeek authored the initial test proposal and runner revision; Codex
reviewed/applied them, added the half-length counterexample, authored the
RTL repair, and executed the tests.
