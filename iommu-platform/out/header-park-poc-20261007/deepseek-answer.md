## Findings summary

I analysed the supplied sources and produced a bounded test-only patch for the two files. The per-stream FAST-tag path is gated behind a new OPTIONAL parameter `ReadTagPerStreamEnable` (default `1'b0`) and a new input `bank_read_stream_i`; in default configuration the FIFO index, pop index, stored value, overflow/underflow fault bits and timing reduce **exactly** to the current expressions. The `$fatal` reorder is only armed in the optional mode.

### `ar_data` lifetime confirmation (from the supplied TC source)

`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`:

- `ar_data` is the head of the AR stream FIFO `i_ar_stream_fifo` (`:141-164`), valid while `ar_valid` is set (`:142`, `:161`).
- The completion AR is built directly from it: `axi_comp_req.ar.id = ar_data.id;` (`:346`), i.e. the completion AR ID equals the bank's accepted AR ID.
- The FIFO is popped (`ar_ready = is_read`) **only** in the `COMPLETE` state and **only** in the cycle the completion AR is accepted downstream: `if ((axi_comp_resp.ar_ready && is_read) || ...) begin ... ar_ready = is_read; state_n = IDLE; end` (`:721-726`). Therefore `ar_data` is held stable across `IDLE → REQ_TRANS → AWAIT_RESP → COMPLETE`, and advances one cycle *after* the completion handshake.
- `is_read` is a registered function of `trans_req_data_q.ttype` (`:266-273`), so the bank's `is_read` and `ar_data` describe the same in-flight transaction at the completion handshake.

On the harness side (`tests/nonblock_actual_dma_system_top.sv`), `u_comp_ar_chan.id = u_comp_ar_id` (`:500`), `u_comp_ar_id` is the completion AR ID re-exported by the inner harness (`.comp_ar_id (u_comp_ar_id)`, `:1141`) and the proxy consumes it as `.s_ar_chan (u_comp_ar_chan)` (`:1256`). Hence `s_ar_chan.id[StreamIdBit]` and `gen_bank[b].i_bank.ar_data.id[2]` are the same AR-ID bit of the same read, provided the `bank_trans_resp_valid_i[b] & bank_trans_resp_ready_i[b]` handshake coincides with the TC's completion-AR acceptance (i.e. the banked arbiter does not add a register stage between the TC completion and `bank_trans_resp`) — see Unresolved/risks.

### Design notes / assumptions

- `ReadTagPerStreamEnable` only changes the **FIFO index** used for push/pop. `capture_bank = s_ar_chan.id[BankIdBit]` remains exclusively for `buf_bank_q`/`restricted`/`exempt`. `fast_resp_bank_i` remains the value source (`tag_push_fast[..] = fast_resp_valid_i && fast_resp_ready_i && (fast_resp_bank_i == b)`).
- The push/pop index is the stream only when `ReadTagPerStreamEnable` is set; `ReadTagQueueEnable` still gates the FF block, so when `ReadTagQueueEnable=0` the optional feature is inert and the `fast_pending_q` path is unchanged.
- Depth stays 8; `dbg_tag_fault[0]` (overflow) and `dbg_tag_fault[1]` (underflow) are preserved verbatim. The duplicate-push watchdog reuses `dbg_tag_fault[2]` and additionally `$fatal`s, so **no new port** is added. Only `$fatal`/`$error`-style test code, consistent with “NOT PRODUCTION RTL”.
- `StreamIdBit = 2` default and the system-top wiring `ar_data.id[2]` match. I did **not** author the `HeaderParkMode` parameter (Codex owns it); the system-top hunk references it only in the `.ReadTagPerStreamEnable(HeaderParkMode != 0)` binding.

```diff
diff --git a/tests/nonblock_read_admission.sv b/tests/nonblock_read_admission.sv
--- a/tests/nonblock_read_admission.sv
+++ b/tests/nonblock_read_admission.sv
@@ -16,5 +16,12 @@ module nonblock_read_admission #(
     parameter int unsigned AddrWidth = 64,
     parameter int unsigned LenWidth  = 8,
     parameter bit ReadTagQueueEnable = 1'b0,
+    // Test-only: when set (with ReadTagQueueEnable) the completed-read tag
+    // FIFOs are indexed by SOURCE DMA STREAM instead of by bank, so reads from
+    // two DMA IDs in the same bank may be reordered.  Default 0 keeps the
+    // original bank-indexed behaviour and timing.
+    parameter bit ReadTagPerStreamEnable = 1'b0,
+    // AR id bit selecting the source DMA stream (DMA0/DMA1).
+    parameter int unsigned StreamIdBit = 2,
     parameter type ar_chan_t = logic
 ) (
@@ -39,4 +39,7 @@
     input  logic        fast_resp_valid_i,
     input  logic        fast_resp_ready_i,
     input  logic        fast_resp_bank_i,
+    // Test-only: source DMA stream (AR id bit StreamIdBit) of each bank's
+    // current successful read response.
+    input  logic [1:0]  bank_read_stream_i,
     input  logic [1:0]  bank_is_read_i,
@@ -98,8 +98,47 @@
     logic [1:0]  fast_pending_q;
     logic tag_q [2][8];
     logic [2:0] tag_rd_q [2], tag_wr_q [2];
     logic [3:0] tag_count_q [2];
-    wire [1:0] tag_push = bank_trans_resp_valid_i & bank_trans_resp_ready_i &
-        bank_is_read_i & ~bank_trans_resp_error_i;
+    wire [1:0] tag_push_bank = bank_trans_resp_valid_i & bank_trans_resp_ready_i &
+        bank_is_read_i & ~bank_trans_resp_error_i;
+    logic [1:0] tag_push;
+    logic [1:0] tag_push_fast;
+    logic       tag_push_dup;
+
+    // Default: the tag FIFO write index IS the bank (original behaviour).
+    // ReadTagPerStreamEnable: a bank's successful read pushes into the FIFO of
+    // the source DMA stream that issued it (bank_read_stream_i).  The stored
+    // FAST value stays derived from the bank-based fast_resp_bank_i comparison.
+    always_comb begin : tag_push_comb
+        tag_push      = 2'b00;
+        tag_push_fast = 2'b00;
+        tag_push_dup  = 1'b0;
+        if (ReadTagPerStreamEnable) begin
+            for (int b = 0; b < 2; b++) begin
+                if (tag_push_bank[b]) begin
+                    if (tag_push[bank_read_stream_i[b]])
+                        tag_push_dup = 1'b1;
+                    tag_push[bank_read_stream_i[b]]      = 1'b1;
+                    tag_push_fast[bank_read_stream_i[b]] =
+                        fast_resp_valid_i && fast_resp_ready_i &&
+                        (fast_resp_bank_i == 1'(b));
+                end
+            end
+        end else begin
+            tag_push = tag_push_bank;
+            tag_push_fast[0] = fast_resp_valid_i && fast_resp_ready_i &&
+                               (fast_resp_bank_i == 1'b0);
+            tag_push_fast[1] = fast_resp_valid_i && fast_resp_ready_i &&
+                               (fast_resp_bank_i == 1'b1);
+        end
+    end
+
+    // Test-only safety net: bank assignment is immutable at request time, so in
+    // per-stream mode two banks must never complete a read for the SAME source
+    // stream in the SAME cycle.  Flag it and stop the simulation.
+    always_comb begin : tag_push_dup_check
+        if (ReadTagQueueEnable && ReadTagPerStreamEnable && tag_push_dup)
+            $fatal(1, "nonblock_read_admission: two banks pushed the same source stream in one cycle");
+    end
 
     logic [31:0] first_offers_q;
@@ -116,4 +116,9 @@
     wire capture_bank = s_ar_chan.id[BankIdBit];
-    wire capture_fast = ReadTagQueueEnable ? tag_q[capture_bank][tag_rd_q[capture_bank]] :
+    // Capture/pop index: bank in default mode; source DMA stream when the
+    // per-stream reorder is enabled.  capture_bank itself stays bank-based, so
+    // budget/exempt accounting is unchanged.
+    wire capture_stream  = s_ar_chan.id[StreamIdBit];
+    wire capture_tag_idx = ReadTagPerStreamEnable ? capture_stream : capture_bank;
+    wire capture_fast = ReadTagQueueEnable ? tag_q[capture_tag_idx][tag_rd_q[capture_tag_idx]] :
         fast_pending_q[capture_bank];
     wire capture_fire = (mode_i != RA_NONE) && (state_q == ST_EMPTY) && s_ar_valid;
@@ -214,17 +214,17 @@
         end else if (ReadTagQueueEnable) begin
             for (int b = 0; b < 2; b++) begin
                 tag_count_q[b] <= tag_count_q[b] + 4'(tag_push[b]) -
-                    4'(capture_fire && capture_bank == 1'(b));
+                    4'(capture_fire && capture_tag_idx == 1'(b));
                 if (tag_push[b]) begin
-                    tag_q[b][tag_wr_q[b]] <= fast_resp_valid_i && fast_resp_ready_i &&
-                        fast_resp_bank_i == 1'(b);
+                    tag_q[b][tag_wr_q[b]] <= tag_push_fast[b];
                     tag_wr_q[b] <= tag_wr_q[b] + 1;
-                    if (tag_count_q[b] == 8 && !(capture_fire && capture_bank == 1'(b)))
+                    if (tag_count_q[b] == 8 && !(capture_fire && capture_tag_idx == 1'(b)))
                         dbg_tag_fault[0] <= 1'b1;
                 end
-                if (capture_fire && capture_bank == 1'(b)) begin
+                if (capture_fire && capture_tag_idx == 1'(b)) begin
                     tag_rd_q[b] <= tag_rd_q[b] + 1;
                     if (tag_count_q[b] == 0) dbg_tag_fault[1] <= 1'b1;
                 end
             end
+            if (tag_push_dup) dbg_tag_fault[2] <= 1'b1;
         end
     end
diff --git a/tests/nonblock_actual_dma_system_top.sv b/tests/nonblock_actual_dma_system_top.sv
--- a/tests/nonblock_actual_dma_system_top.sv
+++ b/tests/nonblock_actual_dma_system_top.sv
@@ -1240,8 +1240,9 @@
     nonblock_read_admission #(
         .BankIdBit (SameBankDmasEnable ? 3 : 2),
         .IdWidth   (4),
         .AddrWidth (64),
         .LenWidth  (8),
         .ReadTagQueueEnable(SharedPoolMode >= 4),
+        .ReadTagPerStreamEnable(HeaderParkMode != 0),
         .ar_chan_t (slv_ar_chan_t)
     ) i_read_admission (
@@ -1264,4 +1264,8 @@
         .fast_resp_valid_i (dbg_fast_resp_valid),
         .fast_resp_ready_i (dbg_fast_resp_ready),
         .fast_resp_bank_i  (dbg_fast_resp_bank),
+        .bank_read_stream_i ({
+            i_dma.i_sys.gen_banked.i_banked.gen_bank[1].i_bank.ar_data.id[2],
+            i_dma.i_sys.gen_banked.i_banked.gen_bank[0].i_bank.ar_data.id[2]
+        }),
         .bank_is_read_i ({
```

## Supporting citations

- Push condition reused unchanged: `tests/nonblock_read_admission.sv:102-103` (`bank_trans_resp_valid_i & bank_trans_resp_ready_i & bank_is_read_i & ~bank_trans_resp_error_i`).
- FAST value source kept bank-based: `tests/nonblock_read_admission.sv:219-220` (original `fast_resp_bank_i == 1'(b)`), now folded into `tag_push_fast[b]` (`:11-23` of the replacement block).
- Overflow/underflow diagnostics preserved: `tests/nonblock_read_admission.sv:222-223` and `:227`.
- Depth-8 FIFOs preserved: `:99-101`.
- Bank index for budget/exempt untouched: `:116` (`capture_bank`), `:123-129` (`restricted`/`exempt`).
- Existing tag-FIFO write/pop driver: `:207-231`.
- Completion AR ID provenance: `rv_iommu_trans_ctl_wrap.sv:346` (`axi_comp_req.ar.id = ar_data.id`), pop at `:721-726`, AR FIFO `:141-164`.
- Proxy `s_ar_chan` origin: `tests/nonblock_actual_dma_system_top.sv:500`, `:1141`, `:1256`.
- Existing hierarchy precedent for the wiring path: `tests/nonblock_actual_dma_system_top.sv:1267-1275` (`...gen_bank[b].i_bank.is_read`, `...bank_trans_resp_*`).

## Acceptance checks (for Codex to run — I cannot execute)

1. **Elaborate/compile both test-only files** together with the Codex-authored `HeaderParkMode` parameter on `nonblock_actual_dma_system_top`. No production file may be touched.
2. **Default equivalence**: with `ReadTagPerStreamEnable=0`, `capture_tag_idx == capture_bank`, `tag_push == tag_push_bank`, and `tag_q[b][wr] <= fast_resp_valid_i && fast_resp_ready_i && (fast_resp_bank_i == 1'(b))` for both `b`. Existing nonblock tests must show bit-identical `dbg_capture_fast`/`dbg_tag_queued` and cycle timing; no new registers are added (only `always_comb`).
3. **Per-stream mode**: `ReadTagPerStreamEnable=1`, `StreamIdBit=2`. Every successful bank read (`bank_trans_resp_valid_i[b] & bank_trans_resp_ready_i[b] & bank_is_read_i[b] & ~error`) pushes into FIFO index `bank_read_stream_i[b]` with the bank-based FAST value; `capture_fire` pops FIFO `s_ar_chan.id[2]`. `dbg_tag_fault[0]/[1]` unchanged; `dbg_tag_fault[2]` never set in the normal PoC.
4. **PoC (aligned 4 KiB, one job, IDs 0/4)**: DMA0 read = `id[2]=0`, DMA1 read = `id[2]=1`; both complete in the same bank. Each completion lands in its own stream FIFO, completions are reordered within the bank across the two streams, and `dbg_capture_fast` at each proxy capture matches the FAST status of the matching DMA read. Same-DMA-ID order (per-stream FIFO order) is preserved.
5. **Duplicate watchdog**: with both banks asserting a successful read and `bank_read_stream_i[0]==bank_read_stream_i[1]`, expect `$fatal` and `dbg_tag_fault[2]==1`.
6. **No cross-bank/stream index collision in default**: `capture_bank` still drives `buf_bank_q`, `restricted`, `exempt`, `dbg_capture_bank`; only the tag FIFO index changes in the optional mode.

## Unresolved / risks (needs Codex verification)

- **`bank_trans_resp` ↔ `ar_data` coincidence.** The supplied TC confirms `ar_data` is stable through the TC completion handshake and only advances afterwards, but the supplied set does **not** include the banked module that produces `bank_trans_resp_valid/ready`. Confirm the banked arbiter does not register a stage between the TC completion and `bank_trans_resp`; if it does, `ar_data.id[2]` may already be the *next* AR at that handshake and the wiring must sample the registered ID instead.
- **Bit-collision when `StreamIdBit == BankIdBit`.** In system-top, `BankIdBit = SameBankDmasEnable ? 3 : 2`. If `SameBankDmasEnable=0` while `HeaderParkMode!=0`, then `capture_bank == capture_stream == id[2]`, i.e. bank and stream are the same bit; the per-stream feature is then degenerate. The supplied ID 0/4 PoC (bit2 = stream) implies the intended config separates bank from stream (e.g. `BankIdBit=3`). Verify the PoC configuration keeps bank/stream bits distinct.
- **`ReadTagPerStreamEnable` without `ReadTagQueueEnable`.** `HeaderParkMode!=0` with `SharedPoolMode<4` yields `ReadTagPerStreamEnable=1, ReadTagQueueEnable=0`; the feature is fully inert (only the `fast_pending_q` path is active) and no `$fatal` can fire. Confirm this is acceptable for the PoC configuration.
- **`$fatal` in `always_comb`** is test-only, non-synthesizable code; if a lint/synthesis flow elaborates this module, gate or replace it. No production change is proposed.
- I could not validate the exact hierarchy path `i_dma.i_sys.gen_banked.i_banked.gen_bank[b].i_bank.ar_data.id[2]` beyond the precedent at `tests/nonblock_actual_dma_system_top.sv:1267-1275` and the TC signals `ar_data`/`is_read`; whichever module `i_bank` is, `ar_data` must be the AR channel of the transaction that produces `bank_trans_resp`.
