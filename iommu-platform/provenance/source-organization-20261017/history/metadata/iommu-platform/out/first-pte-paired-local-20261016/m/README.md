# M-ID f1 local RTL overlay

## Material Passport

- Origin: ARS-Codex experiment-agent, authorized bounded implementation of `FIRST-PTE-PAIRED-LOCAL-1`; no pipeline restart.
- Date: 2026-10-16 JST. Codex-only; requested Sol/xhigh; independently observable model/effort metadata unknown.
- Status: authored and source-reviewed overlay; isolated lint and eight finite gate+actual-arbiter RTL protocol cases passed. Integrated elaboration, DMA simulation and cost acceptance belong to root and are not claimed here.
- Scope: only this `m/` directory was written. No production source, ABI, old output, hardware/MIG work, or performance matrix.

`files/` contains complete replacements at paths relative to `iommu-platform/`; `overlay.patch` describes exactly these replacements against their current original bytes. `manifest.json` binds both versions. Do not apply to production: root assembles a separate selected test tree. The current component inputs were CVA6 HEAD `a1b08934391833aed21e6ad63b545634040808ac` and IOMMU HEAD `57eb5621802ef9ed8d77c684b9a03205c6630391`, with existing worktree edits retained in the copies. `sources.lock.json` still names older IOMMU `238a39e66a07b27a67fb5749df40691682555027`; these overlays bind bytes rather than claiming that the lock or an old bitstream is updated.

## Selected source and interfaces

Use one `cva6/vendor/pulp-platform` AXI/common_cells tree. The old fixture's IOMMU-vendor `axi_mux`/`rr_arb_tree` definitions must be replaced, not added alongside these. The additional module is `files/tests/m_first_pte_gate.sv`. No change to `axi_xbar_unmuxed.sv` is necessary: all new control goes from `axi_xbar` directly into the selected existing `gen_mst_port_mux[0].i_axi_mux`, after the existing ingress/decode/multicut.

The five new TL outputs at `rv_iommu_tl_wrap.sv:135` are direct same-clock wires:

| TL output | Meaning / root wiring |
| --- | --- |
| `m_normal_s1_start_o` | Real accepted normal-S1 PTW occurrence, from the PTW state/edge condition at `rv_iommu_ptw.sv:154`; observation output. |
| `m_ptw_active_o` | Existing PTW active, no additional flop; wire to xbar `m_ptw_active_i`. |
| `m_qualified_start_o` | Start plus valid normal untranslated write, nondebug, nonimplicit/nonMSI, actual root page in the frozen DRAM range; wire to xbar `m_start_i`. |
| `m_start_bank_o` | `req_data.bank` from the same held request; wire to xbar `m_start_bank_i`. |
| `m_root_dram_o` | Observation of the actual root-page predicate. |

Root adds ports through `nonblock_banked_top -> nonblock_actual_dma_top -> nonblock_actual_dma_system_top`. Do not add source-stage registers or mailboxes. The gate receiver record captures on the very same edge as the actual start, giving f1. `start` and `active` must not acquire different delays.

Root configures the test `axi_xbar` as follows:

```systemverilog
.MFirstPteEnable(1'b1),       // compile-time M candidate; runtime enable can select its bypass
.MFirstPteMstPort(0),
.MFirstPteH(64),
.CommonDsFirstEnable(1'b1)    // common paid policy for ALL comparison policies
```

Inputs are `m_enable_i` (true only for the M run), `m_pair_qualified_i` (quiet-established static pair lifetime), `m_start_i`, `m_ptw_active_i`, and `m_start_bank_i`. Constant-disabled controls are ignored in other mux instances. For separate cost elaborations, set `MFirstPteEnable=0` for the ordinary design; the runtime-disabled M build still physically contains M logic and is not a valid incremental-cost baseline.

The xbar outputs `m_state_o[15:0]`, `m_p1_commit_o`, `m_arm_o`, `m_timeout_o`, `m_masked_o`, and `m_ds_quota_o[2:0]` need no additional receiving registers for correctness. They are observation endpoints. `m_state_o` fields are:

| Bits | Meaning |
| --- | --- |
| 0 | pending first-PTE association |
| 1 | active new-request suppression |
| 2 | escape outstanding |
| 9:3 | remaining H, 7 bits |
| 10 | enable and pair qualified |
| 11 | accepted arm occurrence on this edge |
| 12 | exact P1 selector acceptance on this edge |
| 13 | H reaches zero on this edge |
| 14 | raw A head requests a mask; held winner may legally ignore it |
| 15 | pending walk has retired |

The existing pre-output-spill selector can be observed at `i_xbar.gen_mst_port_mux[0].i_axi_mux.gen_mux.{ar_valid,ar_ready,mst_ar_chan}`; raw candidates are `slv_ar_valids/slv_ar_chans`. These are observation taps only. No hierarchical reference controls the implementation. `m_p1_commit_o` is this selector's acceptance, not DDR/atomics/MIG acceptance.

## Frozen gate and safety boundary

The fixed source contract is V DMA0/bank0 and A DMA1/bank1. Existing full PTW ID is `{2'b11,4'b0}=48`; exact A prefix is `(full_id & 6'h3c)==6'h24`, accepting local IDs 0..3 only from payload source2 and original DMA1. This does not identify a unique last AR within that parent. M can defer any new AR from this exact A source while active. Root records which actual A AR was affected.

The source predicate forms the real root base using the same `iosatp_ppn_q[PPNW-1:0]` bits as the PTW and requires `0x80000000 <= base <= 0xbffff000`, so its aligned 4KiB page is entirely within the actual `[0x80000000,0xc0000000)` DRAM decode. This comparison is new logic and must be counted. Root still owns immutable context/root/route, physically independent COPY ranges, and lifecycle qualification. A different bank alone is insufficient. Static E0 applies only to this fixed quiet/reset-delimited test pair; there is no claim of dynamic pair reconfiguration or deployment qualification at zero cost.

H=64 was frozen by root before measurement from the existing forward50 plus three forward cuts and 11 cycles of fixed bound margin. It is not tuned from results. It bounds eligibility suppression and does not promise a P1 response in 64 cycles. P1 may be delayed by existing accepted work. At H expiry, disable or pair invalidation, remaining clears but pending survives. Only exact full-ID48 selector acceptance or actual PTW retirement clears pending. P2 cannot be mistaken for P1 under the fixed f1/root-target/serial-walker contract. No control replay or renewal occurs.

`rr_arb_tree.sv:203` applies `req_allow_i` only when the existing lock is clear. A locked request set and winner are retained using the original `lock_q/req_q`, including when a new M start arrives, H expires, or qualification changes. The leaf grant equations at `:322` qualify READY with the effective `req_d` for both mask and DS-first modes. Thus a masked raw VALID cannot falsely handshake. Original data/ARID/RID and spill contents are unchanged. Default-disabled instances retain the original ready behavior.

M masks only the exact A head at the shared merged payload port. That head can prevent V payload requests behind it from reaching the selector. This is a real head-of-line effect, not an independence or source-done proof. P1 remains on the separate DS path; the gate ends at P1 acceptance or H, not at whole-walk completion. Correctness and any useful completion difference require integrated traces.

Escape requires one non-DS acceptance at the same selector after an exit. A simultaneous non-DS acceptance can satisfy escape. New starts use old pending/escape state, so an occurrence skipped while busy or escaping is never resurrected later. With no non-DS traffic, ordinary DS service continues; M remains unarmed.

## Common current-VALID DS-first/F4

`rr_arb_tree.sv:157` implements the common comparison policy, enabled only on this DRAM AR tree. Source3 wins whenever it is currently valid and fewer than four consecutive DS requests have been accepted. Its quota saturates at four. At four, an eligible non-DS request wins if one exists; any accepted non-DS resets the quota. DS continues if no eligible non-DS is present. An already held winner always completes before a new policy decision. Among non-DS requests the existing RR selection remains in use. The 3-bit quota is common cost; DS-first/F4 is not claimed as stock platform behavior.

## Storage, ports and paths

M adds 9 role-bearing register bits: pending1 + remaining7 + escape1. Vbank is fixed0, so no bank record is needed; E0 adds no epoch record. No source capture, PTW busy copy, held copy, per-PTE tag, ack state, new payload storage or AXI field is introduced. Five TL observation/control outputs include the three functional long wires qualified-start/active/bank; the actual root comparator and normal-write predicate are additional combinational cost. The pair qualification input and lifetime remain a separately paid shared/test-owner obligation. The common DS-first/F4 quota adds 3 bits and must be excluded from M's incremental state comparison.

Critical register paths to inspect under identical local synthesis constraints are:

1. PTW `state_q/edge_trigger_q` and TL root/request/context registers -> qualified start/root checks -> long test-wrapper wires -> gate pending/remaining D. This is the f1 long path; lint does not establish setup closure.
2. gate pending/remaining and existing candidate ARID -> mask -> DS/F4 candidate selection -> existing RR tree -> existing AR output spill D / upstream READY.
3. common quota -> DS/F4 candidates -> selected index -> quota next state.
4. existing selector VALID/READY/fullID and direct PTW active -> pending/escape D.

No physical routing, cell, FF mapping, area or Fmax claim follows from the role-bit counts. Root performs cost/complete-job comparison and acceptance.

## Verification and remaining work

`verification.json` records two isolated lint exit0 results plus the finite RTL unit result. `gate-lint.log` is empty (zero warnings). `arbiter-lint.log` retains existing common_cells/lzc warnings and command-line `-G` bit-width warnings. No integrated DMA simulation or synthesis was run by this worker. Root should verify actual normal-write/root qualification, real start-to-selector timing, payload head-of-line effect, all bytes/final Bs/quiet, and no lost accepted work in the composed system. Observing no mask in the fixed normal program is a valid negative result; do not force internal states or search a favorable phase.

Root authorized the finite protocol follow-up after the first delivery. Run it from `iommu-platform/` with `python3 out/first-pte-paired-local-20261016/m/unit/run.py`. The successful exact build/execution argv, tool version and input hashes are in `unit/result.json`; stdout is in `unit/execute.log`, compile diagnostics in `unit/build.log`. Verilator4.038 does not support `--binary`; the rejected r0 command/log/test source are retained under `unit/attempt0-unsupported-binary/`. r1 uses the supported `--cc --exe --build` plus C++ clock driver. This tool compatibility revision changed only the test driver; all six RTL overlay hashes remain identical to `manifest.json`. There were zero RTL assertion failures.

The actual gate and actual modified RR instance passed **8 scenarios / 1087 executable checks / 130 clocks**, including generic checks on every active edge for held VALID/data/index stability, one producer acceptance per selector acceptance, no grant to invalid producers, source ID preservation and bounded quota. The directed cases cover ineligible start/no replay; same-edge f1 receiver capture; exact P1 single consume and escape; all 64 timer edges, skipped busy start, passive pending retention and late P1; true active retirement with simultaneous skipped new start; held A across late intent and arriving DS; four DS accepts followed by non-DS service; held DS with quota saturated; and soft-disable passive identity. Representative raw rows are `unit/execute.log:79` (last H edge), `:88` (A offered before intent), `:90` (A still held while masked), `:93` (held A accepted), `:94` (P1 then consumed), `:95` through `:102` (four DS accepts then retained/accepted non-DS), and `:108` through `:111` (held DS survives new non-DS at quota4). The gate inputs here are producer/consumer boundary stimuli, not forced internal state. The actual PTW/TL qualification predicate and full DMA composition remain root's integrated checks. This is finite protocol evidence, not a new performance model or formal proof.

The original source design's general bank+epoch record is intentionally specialized to the authorized fixed test pair. If the test starts changing pair/context/reset independently, E0 is no longer this contract. Separate output source or transport reset while untagged AXI work is outstanding remains unsupported.
