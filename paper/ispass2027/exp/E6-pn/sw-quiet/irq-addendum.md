# Accepted known-next-edge MSI-source closure

The original ten-file candidate and complete diagnostic remain in
`runs/run05-before-irq-fix/`; all ten production files there are byte-identical
to run04. `irq-gap-before-fix.json` extracts the actual original window, retains
the raw SHA, and binds those ten source files. This diagnostic passes its own
bounded data/retirement checks, but it **exposes the pre-prediction idle gap**;
it is not a successful fixed-quiet qualification.

At cycle49 post, the real first FQ B has retired and tail becomes1 while head0.
The original FIP hardware update is de=d=1 but the IPSR FIP q is still0.
At cycle50 pre, every prior child reports idle and old SW idle is1. At the next
edge FIP q becomes1, and then the real MSI generator enters WRITE/sets edged.
This is known work about to enter the old MSI input, even though no AXI debt
was accepted during the gap. Root directly checked the real FQ→SW→regmap
latch→MSI chain and accepted the following passive correction.

Only SW's new tap changes. When MSI is actually compiled and the original
routing selects MSI (`!reg2hw.fctl.wsi.q`), each of cip/fip/pmip contributes
`hw2reg.ipsr.X.de && hw2reg.ipsr.X.d && !reg2hw.ipsr.X.q`. Any such known
next-edge rising source prevents new SW idle. No FF, field, interrupt, FSM,
READY/VALID or legacy output changes. When q is already1 the term is0; sticky
edged IRQ and continuously asserted FQ de therefore do not block forever.
WSI_ONLY or current WSI routing does not acquire this additional MSI term.

`irq-prediction-before.sv` and `irq-prediction.patch` preserve the explicit
before/after production addition. Final actual main run06 and omitted run07
both test the final ten component bytes. The independent raw audit matches
every original output in the before and after MSI-error case, while cycle50
pre changes only new SW idle1→0. Known FIP work closes without changing the
original MSI B1/FQ B2 natural retirements. Final main cases also retain sticky
edged IRQ recovery, masked pending, and WSI-route idle; the extra configuration
preserves compile-omitted MSI/debug idle identities.

The finite stimulus produces the critical FIP path. All three source terms
are in the reviewed production expression, but this package does not claim
three separately isolated CIP/FIP/PMIP IRQ-generation campaigns.
