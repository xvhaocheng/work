# Formal PN runner

`collect_formal.py` is separate from accepted S15. It reads existing
matrix235-v1.csv: `--phase main`150, `gap`30, `ds50`40. All required arguments:

```text
python3 collect_formal.py --phase main --source-root /home/xvhaocheng/work-root-a --load-manifest /actual/load.json --source-manifest /actual/pn-source.json --software-manifest /actual/original/software-build.json --s15-gate /actual/accepted/gate.json --output /new/run --stop-file /tmp/pn-stop
```

Root alone selects/loads the qualified phase BIT and installs accepted
`pn-config`/`pn-control` in the target PATH. No build/load/SD action exists here.
The actual PN source manifest uses the same fields as S15:
selected_cva6/iommu/linux; the accepted gate binds those component identities.
DS0 load must match S15 BIT; DS50 must be distinct and have accepted S15.
Legacy load receipts have no DS field: Root supplies the correct phase receipt.

Per point: guarded kernel/boot/taint check → one set-mode (including same-mode
flush) → one set-gap → unchanged original batch command → pn-control read.
Writer success/full raw mode, gap and one functional attempt are checked.
Any failure retains raw and accepted prefix, stops without retry/restore.
Stop-file is checked only between complete points and marks preempted.
Cleanup failure marks failed. Source, plan and existing receipt paths/hashes
are recorded; raw is kept only once by the UART helper, with no copied tree.

The runner reuses gate.parser's actual outer PATH0 shared leave checks. Only
this invocation's private adapted parse namespace replaces the original
gap comparison by `gap==(4096-1)*(configured_gap+1)`. No S15/gate/helper source
changes. The fixed8aac observer `dma_observer.sv:194–204,222–231` enters REFILL
on a nonfinal done and increments gap every REFILL cycle through next issue;
accepted gap frontend finite evidence (`gap-validation/README.md:50–56`)
measures done→next-accept1+g, no initial/final gap. Therefore g0 remains4095;
g1000/4000 expect4095×1001/4001. This is a retained hard check, not a dropped
cadence assertion or modified UART text/CLI JSON.

Post-read requires199==7 (frozen/A/union valid; all error/reserved flags zero),
positive197 and198≥both197/V cycles, exact full193/gap/caps and194 eligibility/
quiet/no-fault.195/196 are real u32 N accepts: N-disabled modes require0;
N-enabled modes retain actual zero or positive counts independently of label.
A final bytes must equal completed×4096. A own rate uses A final bytes/197;
union rate uses (V+A final bytes)/198, all at50MHz. Original HPM/PATH windows
remain unchanged. A is still continuous target2^32, STOP/drain after Vdone.

Preparation checked syntax, CSV cell/block legality/counts150/30/40 and actual
accepted parser compilation only. No extra fixture matrix, raw parsing,
collection, board case or hardware access occurred. Root source review precedes
use; actual capture/whole-work results are not claimed by this preparation.
