# Root decision03：known next-edge MSI pending work

2026-10-18 JST / ARS experiment-agent. Root read actual original FQ117–119,
257–269; SW source IRQ routing/intp477–480; IPSR latch1701–1764 in the fixed5b7
SW tree. The worker's run04 first exposed this finite endpoint; original failed
run/production identity must remain preserved. A complete raw transition trace
is requested; this decision does not claim the corrected version has run.

FQ retiresB and updates tail when returningIDLE. Its `ipsr_fip_o=fqcsr_ie_i` and
`ipsr_fip_wen_o=(tail!=maskedhead)|mf|of` can become active from that new tail.
The IRQ register latches the event at the next edge; prior registered intp is
still0 for a cycle. MSI child may then appear locally idle although known
hardware IRQ update is queued. No outstanding AXI at that instant is a weaker
statement than no known future SW-domain work. A similar boundary exists for
CQ/HPM sources routed by the original three-entry intp vector.

Root authorizes one additional **readonly combinational** predicate in the
already modified SW wrapper, no additional file/state/ports:

```text
if IGS supports MSI and original fctl.wsi==0:
  pending_irq_raise = OR over original intp sources cip/fip/pmip:
                     hw2reg.ipsr.X.de && hw2reg.ipsr.X.d
                     && !reg2hw.ipsr.X.q
  pn_idle additionally requires !pending_irq_raise
else:
  predicate's aggregate identity is1
```

The compile branch must use the original config; do not instantiate MSI under
WSI_ONLY, modify IRQ/regmap/MSI behavior, add pip to original intp, force flags,
or require sticky IRQ/de zero. Once registered IRQ is high and already edged,
this predictive term no longer blocks, allowing natural idle after real MSI
retirement. Existing pending/edged and bus predicates remain authoritative.
Current program writes already block SW quiet, so prediction is conservative
around software clear without becoming a state update or feedback path.

Freeze FQ state/tail/head, real hw2reg.d/de, reg2hw.IPSR, MSI pending/edged/state,
newidle around the original gap before modifying the predicate. Then complete
real MSI-error→FQ chain to expected MSI B1/FQ B2 rather than ending at a transient
local quiet. New finite evidence must show the predictive gap stays nonquiet,
the later natural complete drain is quiet, and all original old outputs retain
fullgold equality. No board/physical/full-stack result follows from this design.
