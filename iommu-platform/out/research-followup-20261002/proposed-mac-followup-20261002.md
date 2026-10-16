# Mac follow-up, 2026-10-02: FPGA bring-up handoff

Scope: this note is a working handoff between the Dell build/analysis host and the
Mac that owns the FPGA board.  It records the current state, the next concrete Mac
actions and the hard limits of this round.  It does not claim that any new build,
test or transfer has already happened.

## Roles

* **Dell host** - source control, compilation of kernel/driver/loader, offline
  analysis of captured runs (`scripts/research_results.py`) and delivery staging.
* **Mac host** - FPGA board, SD card, UART console and JTAG.  The FPGA never leaves
  the Mac; nothing is moved to Dell for execution.

## Current state (carry over, do not redo)

* Linux build `00090` with the original research BIT: the research run recorded
  **80 passed, 2 failed, 2 not run**.
* The original 80 passing cases are **kept as-is and must not be rerun or edited**.
  The global `failed` / `incomplete` status stays; it is not rewritten to complete.
* A new software package is pending.  Do not start the full 84-case matrix or any
  new long run before that package and the short gate below are cleared.

## Recommended next experiment (not yet executed)

Once the new package is available, on the **same BIT**:

1. A new-version **short Solo gate** first.
2. Then **Solo / A8g0 / A8g2000** on the same version and the same run segment,
   **three repetitions each**.
3. Do not start the complete 84-case matrix from this note; that is a separate,
   explicitly scheduled decision, and is deliberately not started here.

## Image and console budget

* The current image payload is about **28 MiB** (`payload_MiB` ~ 28.0).
* The bootrom reads the **whole first GPT partition (32 MiB)** regardless of the
  payload size.
* UART is **115200 8N1**; the theoretical lower bound to read 32 MiB is about
  **42.5 minutes**, and there is currently **no receiver** on the Mac side.  Plan
  the window for that worst case, not for the payload size.

## JTAG

* JTAG can **probe the connection**, but the **speed is not verified** and the
  **bootable path through JTAG is not verified**.
* Only after a **separate maintenance window is scheduled on the Mac** may anyone
  `halt` / `reset` over JTAG.
* **Never write into the RAM of a running Linux.**  No live memory writes.

## Out of scope this round

* No kernel size reduction and no kernel trimming.
* No SD card, bootrom or RTL changes.
* No PPT or slides.

## Concrete Mac next steps

1. **Save the current state first**: capture the SD image, the current BIT and the
   UART/JTAG inventory, and confirm that **no experiment has been executed** in this
   session.
2. Record the currently attached **USB and JTAG hardware** and whether **OpenOCD**
   is installed and usable.
3. Do **not** reflash the BIT; the original BIT stays in place for the pending run.

## Dell delivery path

* Delivery is staged at `out/research-followup-20261002/delivery`.
* If that directory does not exist yet, the delivery is **pending**; nothing in it
  may be announced as compiled, tested or sent until it does.

