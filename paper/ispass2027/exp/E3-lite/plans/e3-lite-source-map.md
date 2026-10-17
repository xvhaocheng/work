# E3-lite initial fixed-source map

This is the pre-implementation map accompanying the
[approved bounded plan](e3-lite-readonly-design.md). Status: planned worktrees,
no candidate source commit or new implementation result yet. Codex-only;
Root owns all hardware access. This batch permits source and finite Verilator
checks only. **No Vivado invocation, including module/OOC synthesis.** PN's
complete build has priority. The failed full-E3 implementation stays stopped.

## Repository boundary

| Repository | Fixed parent | New independent worktree / branch |
| --- | --- | --- |
| work | `main` as resolved when created (record actual parent in execution receipt) | `/home/xvhaocheng/work-e3-lite`, `ispass/e3-lite` |
| CVA6 | `1d587125eee690e623e6ffae13e70f9bbe989adc` | `/home/xvhaocheng/work-e3-lite-cva6`, `ispass/e3-lite` |
| platform | `dfebcc0425bab25567e8514d156650a608530be1` | `/home/xvhaocheng/work-e3-lite-platform`, `ispass/e3-lite` |

Preserve all existing worktrees and branches. The fixed platform already
points to CVA6 `1d587125eee690e623e6ffae13e70f9bbe989adc`, Linux
`da6b69fc007ad5e671dc288dd8038d02e3ebe43d` and OpenSBI
`e8d21bfdc2dd753314627968e496c3a1188f6dfb`. Linux is a hardware source-reference
pin, not a declaration that this software contains ABI5. CVA6's unchanged
IOMMU child is `85b157ebe71a623e326e51fff7b7126fe77e8eb9` at
`corev_apu/riscv-iommu`. No Linux/frontend/backend/top/constraint changes are
part of this batch. Component commits precede any platform pointer/lock update.

## Actual critical source at CVA6 1d587125

All lines below refer to the fixed parent, not to a future edited line number.
The paths are relative to its actual CVA6 repository.

| File and lines | Contract / allowed action |
| --- | --- |
| `corev_apu/fpga/src/dma_path_observer.sv:19–29` | Only production file allowed. Depth64 timestamp+born storage can become one 33-bit memory; preserve pointer/count/statistic widths. |
| same `:41–56` | Histogram update/saturation and HistEnable0 outputs unchanged. Sticky histogram overflow is not a new memory-write gate. |
| same `:59–77` | Preserve `matched`, combinational `bad`, empty age0/current-active, old head age/whole, and all output encoding. |
| same `:79–122` | Original arrays already have no reset assignment. Depth64 storage writes move to a separate posedge process with enable exactly `rst_ni && !clear_i && run_i && !bad && enter_i`. Old Depth4 writes remain in their original reset/clear/run control branch. Metadata/statistics updates unchanged. |
| same `:178–184` | Sixteen stage instances, depth propagation, clear/ARM and stage output connections unchanged. |
| `corev_apu/fpga/src/dma_observer.sv:34–41,63–72` | Unchanged Depth64 requires PATH+hist, OBS5, INFO2 with depth64, stage layout. |
| `corev_apu/fpga/src/ariane_peripherals_xilinx.sv:32,925–926` | Existing queue-depth generic / observer connection unchanged. |
| `corev_apu/fpga/src/ariane_xilinx.sv:15,817` | Existing top generic and forwarding unchanged. |

The last three files are source-boundary controls, not edit targets.

| Fixed parent file | SHA256 |
| --- | --- |
| `dma_path_observer.sv` | `9694a8953532b753d960e6c4f4c856e603769b55d25a0e45abe75ed74dc8de1c` |
| `dma_observer.sv` | `bd7cae915af86596ddb83cf80ecda94428fd67fcd0c9ae0f4d059f2beb05ec06` |
| `ariane_peripherals_xilinx.sv` | `b0495b1c6f030aa8eac1318230ed56cc7042c6db2575c330a01c08d74bd68c4f` |
| `ariane_xilinx.sv` | `a32ac826c9ed7322bcb1652208b9580a3a0bac6dbff53921e8e898c5f80bebe8` |

## Finite acceptance scope

Run the actual candidate RTL beside renamed modules made from the exact
1d587125 gold bytes. Compare all outputs before and after every driven edge
for Depth4 and Depth64, HistEnable0/1. Required directed cases: full queue
simultaneous enter+leave reads the old head; empty simultaneous enter+leave;
pointer wraps; open/close/carry; reset/clear/run0; unmatched leave and full
overflow; histogram and statistic overflow; continued legal writes after
sticky errors. Exercise arbitrary stale memory via prior transactions/reset
and different initialization seeds; occupied entries must have a valid
earlier enter. Preserve raw source snapshots, commands, logs and original
failed attempts. No Python behavioral mirror substitutes for actual RTL.

Successful finite equivalence alone makes no RAM-inference, resource, fit,
timing or board-data claim. Later synthesis needs separate authorization;
any later BIT needs all original 50MHz timing/routing/DRC gates and an honest
new source/platform/artifact identity.
