# Private observation bring-up ledger

All attempts below concern newly added passive observation logic; no scientific workload has run.

1. Make prerequisite: RISCV unset; no FPGA build or workload.
2. New full build diagnosed source trace RAM reset/write style, fixed solely in observer. Source reset/reuse checks passed; source OOC synthesis passed.
3. Generic observer unable to meet UI5ns. Bound implementation to already verified generated MIG global FIFO behavior and explicitly reject contrary order. Old generic source, unit checks and timing retained.
4. FIFO version full synthesis passed, placement stopped before any load/workload when lossless event storage was redesigned for actual200MHz/50MHz held intervals. No functional parameter, pressure or workload changed.
5. v2 Rfire+held19 lossless codec,1536 passive trace entries, native/source bank relocation within existing13-bit INDEX.104 focused checks,29 mailbox checks,5 decoder tests pass. OOC estimated WNS−0.086ns; full routed timing remains decisive. Inputs frozen in build-inputs-attempt-5.json, no new timing exceptions.

Only a routed setup/hold/pulse/loop/route/DRC accepted BIT may be loaded. Failed bring-up or invalid data is never zero debt, never performance evidence. Every actual workload dispatch is protected by an exclusive one-attempt marker. No old experiment, matrix, phase scan or model replay.
