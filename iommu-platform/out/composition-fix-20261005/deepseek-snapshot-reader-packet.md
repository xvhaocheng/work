## Deliverable

Single-file unified diff for `scripts/read_hang_snapshot.py`. Only `reset_tap` (5→6 ones), `shift_ir`, `shift_dr` (per-phase packet split) and the stale top-level note are touched. No new imports, no frame-format change, no retries/transport change; public names/signatures/return types and all error-recording / `healthy` socket-cleanup logic are untouched.

```diff
diff --git a/scripts/read_hang_snapshot.py b/scripts/read_hang_snapshot.py
--- a/scripts/read_hang_snapshot.py
+++ b/scripts/read_hang_snapshot.py
@@ -3,7 +3,8 @@
 read_hang_snapshot.py
 
 Bounded XVC diagnostic reader for a single verified Kintex-7 xc7k325t TAP.
-Standard library only. Proposal only; no execution performed by author.
+Standard library only. Drafted by a worker; the packet-split behavior below
+was executed and verified on hardware by Codex.
 
 References:
 - AMD UG470 Table 10-2 for documented JTAG primitives.
@@ -225,7 +226,11 @@
 def reset_tap(client):
-    """Reset TAP: >=5 TMS=1 then TMS=0. Leaves TAP in IDLE."""
-    tms = [1] * 5 + [0]
+    """Reset TAP: 6 TMS=1 then TMS=0. Leaves TAP in IDLE.
+
+    6 ones (instead of the JTAG minimum 5) matches the measured
+    openFPGALoader behavior verified on hardware by Codex.
+    """
+    tms = [1] * 6 + [0]
     tdi = [0] * len(tms)
     xvc_shift(client, tms, tdi)
 
@@ -237,39 +242,46 @@
 def shift_ir(client, ir_value, ir_len=IR_LEN, idle_clocks=16):
-    """Assume TAP is in IDLE. Shift IR, end in IDLE.
-    Returns TDO bits (unused)."""
-    entry_tms = [1, 1, 0, 0]       # Select-DR, Select-IR, Capture-IR, Shift-IR
-    entry_tdi = [0, 0, 0, 0]
+    """Assume TAP is in IDLE. Shift IR, end in IDLE.
+
+    Each TAP phase is issued as its own XVC shift transaction. The
+    openFPGALoader XVC server mis-samples TDO when the state entry,
+    data, exit and idle clocks are packed into a single shift, so they
+    are split here (verified on hardware by Codex). Returns the TDO
+    bits of the IR data packet (unused)."""
+    # Entry: Select-DR, Select-IR, Capture-IR, Shift-IR
+    xvc_shift(client, [1, 1, 0, 0], [0, 0, 0, 0])
+    # Data: ir_len IR bits LSB-first; final bit carries TMS=1 to leave Shift-IR
     data_tms = [0] * ir_len
     if ir_len > 0:
-        data_tms[-1] = 1            # exit1
+        data_tms[-1] = 1            # Exit1-IR
     data_tdi = [(ir_value >> i) & 1 for i in range(ir_len)]
-    exit_tms = [1, 0]               # Update-IR, Idle
-    exit_tdi = [0, 0]
-    idle_tms = [0] * idle_clocks
-    idle_tdi = [0] * idle_clocks
-    tms = entry_tms + data_tms + exit_tms + idle_tms
-    tdi = entry_tdi + data_tdi + exit_tdi + idle_tdi
-    return xvc_shift(client, tms, tdi)
+    tdo = xvc_shift(client, data_tms, data_tdi)
+    # Exit: Update-IR, Idle
+    xvc_shift(client, [1, 0], [0, 0])
+    # Idle clocks (TMS=0, TDI=0)
+    xvc_shift(client, [0] * idle_clocks, [0] * idle_clocks)
+    return tdo
 
 def shift_dr(client, data_bits, data_len, idle_clocks=16):
-    """Assume TAP is in IDLE. Shift DR, end in IDLE.
-    Returns TDO bits for the data_len data bits."""
+    """Assume TAP is in IDLE. Shift DR, end in IDLE.
+
+    Each TAP phase is issued as its own XVC shift transaction (see
+    shift_ir for rationale). The data packet keeps TMS=0 for every bit
+    except the last (Exit1-DR); xvc_shift still chunks it automatically
+    to respect the advertised XVC buffer size. Returns the full TDO of
+    the data packet, so the result is exactly data_len bits with no
+    entry offset to strip."""
     if len(data_bits) != data_len:
         raise XVCProtocolError("data_bits length mismatch")
-    entry_tms = [1, 0, 0]           # Select-DR, Capture-DR, Shift-DR
-    entry_tdi = [0, 0, 0]
+    # Entry: Select-DR, Capture-DR, Shift-DR
+    xvc_shift(client, [1, 0, 0], [0, 0, 0])
+    # Data: data_len bits; final bit carries TMS=1 to leave Shift-DR
     data_tms = [0] * data_len
     if data_len > 0:
-        data_tms[-1] = 1            # exit1
+        data_tms[-1] = 1            # Exit1-DR
     data_tdi = data_bits[:]
-    exit_tms = [1, 0]               # Update-DR, Idle
-    exit_tdi = [0, 0]
-    idle_tms = [0] * idle_clocks
-    idle_tdi = [0] * idle_clocks
-    tms = entry_tms + data_tms + exit_tms + idle_tms
-    tdi = entry_tdi + data_tdi + exit_tdi + idle_tdi
-    tdo = xvc_shift(client, tms, tdi)
-    # Extract data TDO bits
-    data_start = len(entry_tms)
-    data_tdo = tdo[data_start:data_start+data_len]
-    return data_tdo
+    data_tdo = xvc_shift(client, data_tms, data_tdi)
+    # Exit: Update-DR, Idle
+    xvc_shift(client, [1, 0], [0, 0])
+    # Idle clocks (TMS=0, TDI=0)
+    xvc_shift(client, [0] * idle_clocks, [0] * idle_clocks)
+    return data_tdo
```

## What changed (mapped to supplied source)

- `shift_ir` (orig L237–252): previously concatenated entry+data+exit+idle into one `xvc_shift` (L250–252). Now four independent `xvc_shift` calls — entry `[1,1,0,0]/[0,0,0,0]`, data `[0]*(ir_len-1)+[1]` with IR bits LSB-first, exit `[1,0]/[0,0]`, idle `idle_clocks` zeros. The data packet's TDO is returned.
- `shift_dr` (orig L254–275): entry `[1,0,0]/[0,0,0]`, data N with TMS only last bit set (still auto-chunked by `xvc_shift`/`shift` since each phase now goes through the same chunking helper), exit `[1,0]/[0,0]`, idle zeros. Return is now the raw data-packet TDO of length `data_len` — the old `data_start = len(entry_tms)` offset strip (L272–274) is removed, matching the verified `0x03651093` capture.
- `reset_tap` (orig L227): `[1]*5 + [0]` → `[1]*6 + [0]`, matching hardware measurement.
- Header L6: removed the stale "Proposal only; no execution performed by author" line; states worker-drafted / Codex-verified-on-hardware.

Unchanged: `XVCClient`, `xvc_shift`, `idle_clocks()`, `read_idcode`, `request_and_read_sample`, `main` error recording, `healthy` gating and cleanup BYPASS path, JSON schema, CLI flags, `IDCODE_MASK` (0x0FFFFFFF, so the allowed revision nibble stays accepted). `idle_clocks` parameter names are preserved (note the pre-existing shadowing of the module-level `idle_clocks()` function inside these two functions is intended and unchanged).

## Evidence

Supplied artifact `out/composition-fix-20261005/snapshot-reader-split-bypass.json`: `idcode = 0x43651093` (low 28 bits = `0x03651093`), `bypass1088_ok = true`, `bad_indices = []` — consistent with the described verification. This is the provided evidence; I did not run it.

## Acceptance checks (≤5)

1. `git apply --check` on the diff is clean, and `python3 -m py_compile scripts/read_hang_snapshot.py` succeeds.
2. `reset_tap` emits exactly one shift with TMS `[1,1,1,1,1,1,0]` (7 bits); TDI all zero.
3. `shift_ir`/`shift_dr` each issue 4 discrete `xvc_shift` calls in order (entry, data, exit, idle); IR data packet is `ir_len` bits with only the last TMS=1; DR data packet is `data_len` bits with only the last TMS=1 and its returned list length equals `data_len`.
4. On the real board (Codex execution): `--idcode-only` yields an IDCODE whose low 28 bits equal `0x03651093`, and the BYPASS 1088-bit pattern loopback passes bit-by-bit (`bad_indices == []`).
5. Public API unchanged (`shift_ir(client, ir_value, ir_len=IR_LEN, idle_clocks=16)`, `shift_dr(client, data_bits, data_len, idle_clocks=16)`), `idle_clocks` params retained, and error-recording plus the `healthy`-gated cleanup BYPASS path behave as before.

Note: these acceptance points require execution (compile/board run) which I cannot perform or claim.