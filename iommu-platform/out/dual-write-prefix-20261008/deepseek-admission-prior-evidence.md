## 0. Scope and provenance

- Frozen AXI‑REALM sources are identified in `sources.json:2` as commit `50153a346b753dc2bc7723c446656a43db35d02d`; file list at `sources.json:3-45`.
- `axi_isolate.sv` is not from the AXI‑REALM repo; it is from the `axi` dependency at `sources.json:29-32`, commit `fccffb5953ec8564218ba05e20adbedec845e014`.
- The supplied AXI‑REALM modules are `axi_rt_unit.sv`, `axi_rt_unit_counter.sv`, `axi_gran_burst_splitter.sv`, `axi_gran_burst_splitter_ax_chan.sv`, `axi_write_buffer.sv`; `axi_isolate.sv` is the supplied dependency.
- Stage 41 critical acceptance is `out/fragment-drain-boundary-20261008/codex-critical-acceptance.md`; relevant lines cited below.
- This is static source reading only. No build, run, test, BIT, SD, board, or simulation evidence is claimed.

## 1. Navigable evidence matrix

| ID | Question | Exact evidence | Finding | Caveat |
|---|---|---|---|---|
| E1 | Where is read budget charged? | `axi_rt_unit.sv:279,281`; `axi_rt_unit.sv:352-368`; `axi_rt_unit_counter.sv:84-87` | On AR handshake `fwd_req.ar_valid & fwd_resp.ar_ready`, by `ar_bytes = ({1'b0, fwd_req.ar.len} + 9'h001) << fwd_req.ar.size`; one decrement per downstream AR burst. | `fwd_req` for AR is after the splitter; `axi_write_buffer` bypasses AR/R. |
| E2 | Where is write budget charged? | `axi_rt_unit.sv:278,281`; `axi_rt_unit.sv:370-386`; `axi_rt_unit_counter.sv:84-87` | On AW handshake `fwd_req.aw_valid & fwd_resp.aw_ready`, by `aw_bytes`; one decrement per downstream AW burst. | `fwd_req.aw` is the write-buffer output, so an AW can be buffered before it is charged. |
| E3 | Does `budget_left` influence outstanding txn count or only isolation? | `axi_rt_unit.sv:219-220,364,382`; `axi_rt_unit_counter.sv:90` | Only isolation. `budget_spent_o = (budget_left_o == '0) \| budget_overflow` drives `r_isolate`/`w_isolate`; those OR into `global_isolate`, which drives head and tail `axi_isolate`. `budget_left_o` is exported but is not connected to `NumPending`, `MaxReadTxns`, `MaxWriteTxns`, or `NumOutstanding`. | Software could use exported `budget_left_o` externally, but that is not in these files. |
| E4 | What does `NumPending` mean? | `axi_rt_unit.sv:16,198,227-228,257,394`; `axi_isolate.sv:41-42,191-202,265-266,331`; `axi_write_buffer.sv:76-79,108-110`; `axi_gran_burst_splitter_ax_chan.sv:56-75` | A transaction/burst-count parameter: max pending requests per isolate channel, max read/write txns in the splitter, AW FIFO depth and W-last queue depth in the write buffer. It is not a byte count and not pending bytes. | `axi_gran_burst_splitter_counters` is not supplied; exact counter allocation semantics are inferred from ports and the ax_chan comment. |
| E5 | Does metadata include source-to-dest mapping or cross-manager queue prefix? | `axi_rt_unit.sv:290-322,343-387`; `axi_gran_burst_splitter_ax_chan.sv:14-15,36,63-67`; `axi_write_buffer.sv:76-91,108-123,130-146`; `axi_isolate.sv:191-202` | No source-to-dest mapping. Address decoding produces only `aw_region`/`ar_region` for budget counters. Splitter counters are keyed by AXI ID. Write buffer is FIFO order. Isolate counters are aggregate per channel. | `rt_rule_t` and `addr_decode_dync` are opaque/not supplied; vendor `axi_mux` owner queue is external per `codex-critical-acceptance.md:43`. |
| E6 | How are held AW/AR and source future reads handled? | `axi_isolate.sv:182-187,260-302,328-366,318-323,240-243,255` | `Hold` preserves existing VALID until ready; `Drain` cuts future AW/AR, waits pending counters to zero, lets existing W/B/R continue; `RREADY` follows master; `Isolate` silences. | Tail isolate adds a second stage; top `isolated_o` is OR of head and tail, `axi_rt_unit.sv:415`. |
| E7 | Does full W buffering make R→W continuation irrelevant? | `axi_write_buffer.sv:130-146,76-123,95-102,170-177`; `axi_rt_unit.sv:256-272`; `codex-critical-acceptance.md:65,69` | Not established as a property of the frozen code. W ingress is gated by finite `WBufferDepth` FIFO ready. Full W buffering can decouple destination W drain only up to that depth and only if AW/last/B/isolation permit. | R path bypasses `axi_write_buffer` (`axi_write_buffer.sv:59-65`); R storage is elsewhere. |
| E8 | 2023 paper per-beat wording vs frozen revision? | `sources.json:2,11-45`; `codex-critical-acceptance.md:47`; `axi_rt_unit.sv:278-282`; `axi_rt_unit_counter.sv:84-87` | Frozen code charges full downstream Ax burst at Ax handshake; no per-beat budget decrement in supplied code. | Paper text is not in the bundle; stage 41 explicitly says not to map paper per-beat wording to this code and that frozen code is not proven equal to the paper evaluation revision. |
| E9 | Obvious ordinary baselines? | `axi_isolate.sv:265-266,331`; `axi_write_buffer.sv:76-79,130-146`; `axi_gran_burst_splitter_ax_chan.sv:14-15,56-75`; `axi_rt_unit_counter.sv:63-90`; `axi_gran_burst_splitter.sv:80-87` | Ordinary caps/bookkeeping: per-channel pending-transaction cap; FIFO occupancy caps; per-AXI-ID burst-length counters; per-region byte budget/period token bucket; outstanding-txn cap. | No novelty claim. Vendor `axi_mux` owner queue is external per stage 41. |

## 2. Detailed excerpts

### 2.1 Budget charge point and amount

`axi_rt_unit.sv:278-282`:

```systemverilog
278:   assign aw_bytes = ({1'b0, fwd_req.aw.len} + 9'h001) << (fwd_req.aw.size);
279:   assign ar_bytes = ({1'b0, fwd_req.ar.len} + 9'h001) << (fwd_req.ar.size);
280: 
281:   assign aw_happening = fwd_req.aw_valid & fwd_resp.aw_ready;
282:   assign ar_happening = fwd_req.ar_valid & fwd_resp.ar_ready;
```

`axi_rt_unit.sv:352-386`:

```systemverilog
352:     ax_rt_unit_counter #(
353:       .PeriodWidth ( PeriodWidth ),
354:       .BudgetWidth ( BudgetWidth ),
355:       .ax_bytes_t  ( ax_bytes_t  )
356:     ) i_ax_rt_unit_counter_read (
357:       .clk_i,
358:       .rst_ni,
359:       .ax_bytes_i     ( ar_bytes                         ),
360:       .ax_happening_i ( ar_happening & r_region_selected ),
361:       .enable_i       ( imtu_enable_i                    ),
362:       .budget_i       ( r_budget_i      [r]              ),
363:       .budget_left_o  ( r_budget_left_o [r]              ),
364:       .budget_spent_o ( r_isolate       [r]              ),
365:       .period_i       ( r_period_i      [r]              ),
366:       .period_left_o  ( r_period_left_o [r]              ),
367:       .period_abort_i ( imtu_abort_i                     )
368:     );
369: 
370:     ax_rt_unit_counter #(
371:       .PeriodWidth ( PeriodWidth ),
372:       .BudgetWidth ( BudgetWidth ),
373:       .ax_bytes_t  ( ax_bytes_t  )
374:     ) i_ax_rt_unit_counter_write (
375:       .clk_i,
376:       .rst_ni,
377:       .ax_bytes_i     ( aw_bytes                         ),
378:       .ax_happening_i ( aw_happening & w_region_selected ),
379:       .enable_i       ( imtu_enable_i                    ),
380:       .budget_i       ( w_budget_i      [r]              ),
381:       .budget_left_o  ( w_budget_left_o [r]              ),
382:       .budget_spent_o ( w_isolate       [r]              ),
383:       .period_i       ( w_period_i      [r]              ),
384:       .period_left_o  ( w_period_left_o [r]              ),
385:       .period_abort_i ( imtu_abort_i                     )
386:     );
```

`axi_rt_unit_counter.sv:63-90`:

```systemverilog
63:   budget_t bytes_spent;
64:   logic    budget_en;
65:   logic    budget_overflow;
66: 
67:   delta_counter #(
68:     .WIDTH           ( BudgetWidth ),
69:     .STICKY_OVERFLOW ( 1'b0        )
70:   ) i_delta_counter_budget (
71:     .clk_i,
72:     .rst_ni,
73:     .clear_i   ( 1'b0            ),
74:     .en_i      ( budget_en       ),
75:     .load_i    ( period_over     ),
76:     .down_i    ( 1'b1            ),
77:     .delta_i   ( bytes_spent     ),
78:     .d_i       ( budget_i        ),
79:     .q_o       ( budget_left_o   ),
80:     .overflow_o( budget_overflow )
81:   );
82: 
83:   // explicit cast
84:   assign bytes_spent = budget_t'(ax_bytes_i);
85: 
86:   // enable budget counter
87:   assign budget_en = enable_i & !budget_spent_o & ax_happening_i;
88: 
89:   // no more budget left :(
90:   assign budget_spent_o = (budget_left_o == '0) | budget_overflow;
```

Interpretation:

- Read budget is charged on the `fwd_req.ar_valid & fwd_resp.ar_ready` handshake, using entire `ar_bytes` of the downstream AR burst.
- Write budget is charged on the `fwd_req.aw_valid & fwd_resp.aw_ready` handshake, using entire `aw_bytes` of the downstream AW burst.
- There is no per-beat budget charge in `axi_rt_unit_counter`; it has one `ax_bytes_i` input and one `ax_happening_i` enable.
- Because the splitter can fragment bursts, `fwd_req` is the downstream Ax burst after splitting. So the charge is per downstream Ax burst, not per original upstream burst and not per R/W data beat.
- For writes, `fwd_req.aw` is the `axi_write_buffer` master output. An AW can be accepted into the write buffer before this charge point.

### 2.2 `budget_left` influences isolation only

`axi_rt_unit.sv:219-220`:

```systemverilog
219:   assign global_isolate = ((|r_isolate) | (|w_isolate)) & imtu_enable_i;
220:   assign isolate_o      = global_isolate;
```

`axi_rt_unit.sv:214` and `axi_rt_unit.sv:410`:

```systemverilog
214:     .isolate_i  ( global_isolate | byp_isolate ),
...
410:     .isolate_i  ( global_isolate ),
```

`axi_rt_unit_counter.sv:90`:

```systemverilog
90:   assign budget_spent_o = (budget_left_o == '0) | budget_overflow;
```

Interpretation:

- `budget_left_o` itself is exported as `r_budget_left_o` / `w_budget_left_o`.
- Internally, the only value derived from `budget_left_o` is `budget_spent_o`.
- `budget_spent_o` becomes `r_isolate[r]` / `w_isolate[r]`, which OR into `global_isolate`.
- `global_isolate` drives both head `i_axi_isolate` and tail `i_axi_isolate_tail`.
- `budget_left_o` is not connected to `NumPending`, `MaxReadTxns`, `MaxWriteTxns`, or `NumOutstanding`.
- Therefore, in the supplied RTL, budget_left affects admission/stopping only through isolation. It does not change the configured outstanding-transaction limits.
- The isolation is global: any region’s read or write `budget_spent_o` ORs into one `global_isolate`. It is not per-region isolation in this top module.

### 2.3 What `NumPending` means

Parameter and usages in `axi_rt_unit.sv`:

```systemverilog
16:   parameter int unsigned NumPending         = 32'd0,
...
198:     .NumPending           ( NumPending   ),
...
227:     .MaxReadTxns   ( NumPending         ),
228:     .MaxWriteTxns  ( NumPending         ),
...
257:     .NumOutstanding ( NumPending   ),
...
394:     .NumPending           ( NumPending   ),
```

`axi_isolate.sv:41-42`:

```systemverilog
41:   /// Maximum number of pending requests per channel
42:   parameter int unsigned NumPending = 32'd16,
```

`axi_isolate.sv:191-202`:

```systemverilog
191:   cnt_t pending_aw_d,  pending_aw_q;
192:   logic update_aw_cnt;
193: 
194:   cnt_t pending_w_d,   pending_w_q;
195:   logic update_w_cnt,  connect_w;
196: 
197:   cnt_t pending_ar_d,  pending_ar_q;
198:   logic update_ar_cnt;
199: 
200:   `FFLARN(pending_aw_q, pending_aw_d, update_aw_cnt, '0, clk_i, rst_ni)
201:   `FFLARN(pending_w_q, pending_w_d, update_w_cnt, '0, clk_i, rst_ni)
202:   `FFLARN(pending_ar_q, pending_ar_d, update_ar_cnt, '0, clk_i, rst_ni)
```

`axi_isolate.sv:265-266`:

```systemverilog
265:         if (pending_aw_q >= cnt_t'(NumPending) || pending_ar_q >= cnt_t'(2*NumPending)
266:             || (pending_w_q >= cnt_t'(NumPending))) begin
```

`axi_isolate.sv:331`:

```systemverilog
331:         if (pending_ar_q >= NumPending) begin
```

`axi_write_buffer.sv:76-79` and `108-110`:

```systemverilog
76:   stream_fifo #(
77:     .DEPTH      ( NumOutstanding ),
78:     .T          ( aw_chan_t      )
79:   ) i_stream_fifo_aw (
...
108:   stream_fifo #(
109:     .DEPTH ( NumOutstanding ),
110:     .T     ( logic          )
111:   ) i_stream_fifo (
```

`axi_gran_burst_splitter_ax_chan.sv:56-75`:

```systemverilog
56:   axi_gran_burst_splitter_counters #(
57:     .MaxTxns ( MaxTxns  ),
58:     .IdWidth ( IdWidth  ),
59:     .CutPath ( CutPath  )
60:   ) i_axi_gran_burst_splitter_counters (
61:     .clk_i,
62:     .rst_ni,
63:     .alloc_id_i     ( ax_i.id       ),
64:     .alloc_len_i    ( ax_i.len      ),
65:     .alloc_req_i    ( cnt_alloc_req ),
66:     .alloc_gnt_o    ( cnt_alloc_gnt ),
67:     .cnt_id_i       ( cnt_id_i      ),
68:     .cnt_len_o      ( cnt_len_o     ),
69:     .cnt_set_err_i  ( cnt_set_err_i ),
70:     .cnt_err_o      ( cnt_err_o     ),
71:     .cnt_dec_i      ( cnt_dec_i     ),
72:     .cnt_delta_i    ( max_beats     ),
73:     .cnt_req_i      ( cnt_req_i     ),
74:     .cnt_gnt_o      ( cnt_gnt_o     )
75:   );
```

Interpretation:

- In `axi_isolate`, `NumPending` is explicitly “Maximum number of pending requests per channel” (`axi_isolate.sv:41-42`).
- The counters `pending_aw_q`, `pending_w_q`, `pending_ar_q` count outstanding transactions/AW entries: AW increment `axi_isolate.sv:216-221`, W decrement on W last `axi_isolate.sv:227-230`, B decrement `axi_isolate.sv:231-234`, AR increment `axi_isolate.sv:236-239`, R decrement on R last `axi_isolate.sv:240-243`.
- In `axi_gran_burst_splitter`, `NumPending` is passed as `MaxReadTxns` / `MaxWriteTxns`, which feed per-Ax-channel counter modules. These are transaction/counter limits, not byte limits.
- In `axi_write_buffer`, `NumPending` is passed as `NumOutstanding`, used as AW FIFO depth and W-last queue depth. It is not the W data FIFO depth; that is `WBufferDepth` (`axi_write_buffer.sv:130-134`).
- So `NumPending` is a transaction/burst-count parameter. It is not a pending-byte cap.

### 2.4 Metadata: source-to-dest mapping or cross-manager queue prefix

Address decoding in `axi_rt_unit.sv:296-321`:

```systemverilog
296:   ) i_addr_decode_dync_aw (
297:     .addr_i           ( fwd_req.aw.addr            ),
298:     .addr_map_i       ( rt_rule_i                  ),
299:     .idx_o            ( aw_region                  ),
300:     .dec_valid_o      ( /* NOT CONNECTED */        ),
301:     .dec_error_o      ( w_decode_error             ),
...
313:   ) i_addr_decode_dync_ar (
314:     .addr_i           ( fwd_req.ar.addr            ),
315:     .addr_map_i       ( rt_rule_i                  ),
316:     .idx_o            ( ar_region                  ),
...
```

Region selection in `axi_rt_unit.sv:343-350`:

```systemverilog
343:   for (genvar r = 0; r < NumAddrRegions; r++) begin : gen_regions
344:     // is the current region selected
345:     logic r_region_selected;
346:     logic w_region_selected;
347: 
348:     // the current region is selected if the index is equal to the region number
349:     assign r_region_selected = (ar_region == r);
350:     assign w_region_selected = (aw_region == r);
```

Splitter metadata comment in `axi_gran_burst_splitter_ax_chan.sv:14-15`:

```systemverilog
14: /// Store burst lengths in counters, which are associated to AXI IDs through ID queues (to allow
15: /// reordering of responses w.r.t. requests).
```

Splitter counter ID inputs in `axi_gran_burst_splitter_ax_chan.sv:63-67`:

```systemverilog
63:     .alloc_id_i     ( ax_i.id       ),
64:     .alloc_len_i    ( ax_i.len      ),
65:     .alloc_req_i    ( cnt_alloc_req ),
66:     .alloc_gnt_o    ( cnt_alloc_gnt ),
67:     .cnt_id_i       ( cnt_id_i      ),
```

Write buffer FIFO structure in `axi_write_buffer.sv:76-91`, `108-123`, `130-146`:

```systemverilog
76:   stream_fifo #(
77:     .DEPTH      ( NumOutstanding ),
78:     .T          ( aw_chan_t      )
79:   ) i_stream_fifo_aw (
...
87:     .ready_o    ( slv_resp_o.aw_ready ),
88:     .data_o     ( mst_req_o.aw        ),
89:     .valid_o    ( egress_aw_valid     ),
...
108:   stream_fifo #(
109:     .DEPTH ( NumOutstanding ),
110:     .T     ( logic          )
111:   ) i_stream_fifo (
...
121:     .valid_o    ( mgmt_valid_aw      ),
...
130:   stream_fifo #(
131:     .DEPTH      ( WBufferDepth ),
132:     .T          ( w_chan_t     ),
133:     .ADDR_DEPTH ( IdxWWidth    )
134:   ) i_stream_fifo_w (
...
139:     .usage_o    ( num_w_stored_o ),
...
141:     .valid_i    ( slv_req_i.w_valid  ),
142:     .ready_o    ( slv_resp_o.w_ready ),
143:     .data_o     ( mst_req_o.w        ),
144:     .valid_o    ( egress_w_valid     ),
```

Interpretation:

- The only mapping in `axi_rt_unit` is address → `aw_region` / `ar_region`. Those regions select budget counters.
- The splitter has an AXI ID → burst-length-counter mapping via ID queues (`axi_gran_burst_splitter_ax_chan.sv:14-15,63-67`). This is per-AXI-ID, not source-to-destination.
- `axi_write_buffer` uses a normal FIFO for AW, a normal FIFO for W, and a last-token queue. It does not carry source-to-dest metadata or cross-manager queue-prefix metadata.
- `axi_isolate` counters are aggregate per channel, not per source.
- Stage 41 describes cross-manager queue-prefix obligations as an analysis concept, not as metadata in these modules: `codex-critical-acceptance.md:61` says “两个写队列中排在所需W之前的前缀也成为义务”.
- Stage 41 also points to vendor `axi_mux` owner-push/lock as external evidence: `codex-critical-acceptance.md:43`. That file is not in this bundle.

Conclusion for E5: in the supplied frozen AXI‑REALM code, there is no source-to-dest mapping and no cross-manager queue-prefix metadata. The closest existing metadata is AXI-ID-keyed burst-length counters in the splitter and ordinary FIFO ordering in the write buffer. `rt_rule_t` and `addr_decode_dync` are opaque here; their definitions are not supplied.

### 2.5 Held AW/AR and source future reads

Isolate state and counters in `axi_isolate.sv:182-202`:

```systemverilog
182:   typedef enum logic [1:0] {
183:     Normal,
184:     Hold,
185:     Drain,
186:     Isolate
187:   } isolate_state_e;
188:   isolate_state_e state_aw_d, state_aw_q, state_ar_d, state_ar_q;
189:   logic           update_aw_state,        update_ar_state;
190: 
191:   cnt_t pending_aw_d,  pending_aw_q;
192:   logic update_aw_cnt;
193: 
194:   cnt_t pending_w_d,   pending_w_q;
195:   logic update_w_cnt,  connect_w;
196: 
197:   cnt_t pending_ar_d,  pending_ar_q;
198:   logic update_ar_cnt;
199: 
200:   `FFLARN(pending_aw_q, pending_aw_d, update_aw_cnt, '0, clk_i, rst_ni)
201:   `FFLARN(pending_w_q, pending_w_d, update_w_cnt, '0, clk_i, rst_ni)
202:   `FFLARN(pending_ar_q, pending_ar_d, update_ar_cnt, '0, clk_i, rst_ni)
```

Counter updates in `axi_isolate.sv:216-243`:

```systemverilog
216:     if (mst_req_o.aw_valid && (state_aw_q == Normal)) begin
217:       pending_aw_d++;
218:       update_aw_cnt   = 1'b1;
219:       pending_w_d++;
220:       update_w_cnt    = 1'b1;
221:       connect_w       = 1'b1;
222:       if (mst_req_o.aw.atop[axi_pkg::ATOP_R_RESP]) begin
223:         pending_ar_d++; // handle atomic with read response by injecting a count in AR
224:         update_ar_cnt = 1'b1;
225:       end
226:     end
227:     if (mst_req_o.w_valid  && mst_resp_i.w_ready && mst_req_o.w.last) begin
228:       pending_w_d--;
229:       update_w_cnt  = 1'b1;
230:     end
231:     if (mst_resp_i.b_valid  && mst_req_o.b_ready) begin
232:       pending_aw_d--;
233:       update_aw_cnt = 1'b1;
234:     end
...
236:     if (mst_req_o.ar_valid && (state_ar_q == Normal)) begin
237:       pending_ar_d++;
238:       update_ar_cnt = 1'b1;
239:     end
240:     if (mst_resp_i.r_valid  && mst_req_o.r_ready && mst_resp_i.r.last) begin
241:       pending_ar_d--;
242:       update_ar_cnt = 1'b1;
243:     end
```

AW states in `axi_isolate.sv:260-302`:

```systemverilog
260:     unique case (state_aw_q)
261:       Normal:  begin // Normal operation
...
265:         if (pending_aw_q >= cnt_t'(NumPending) || pending_ar_q >= cnt_t'(2*NumPending)
266:             || (pending_w_q >= cnt_t'(NumPending))) begin
267:           mst_req_o.aw_valid  = 1'b0;
268:           slv_resp_o.aw_ready = 1'b0;
269:           if (isolate_i) begin
270:             state_aw_d      = Drain;
271:             update_aw_state = 1'b1;
272:           end
273:         end else begin
274:           // here the AW handshake is connected normally
275:           if (slv_req_i.aw_valid && !mst_resp_i.aw_ready) begin
276:             state_aw_d      = Hold;
277:             update_aw_state = 1'b1;
278:           end else begin
279:             if (isolate_i) begin
280:               state_aw_d      = Drain;
281:               update_aw_state = 1'b1;
282:             end
283:           end
284:         end
285:       end
286:       Hold: begin // Hold the valid signal on 1'b1 if there was no transfer
287:         mst_req_o.aw_valid = 1'b1;
288:         // aw_ready normal connected
289:         if (mst_resp_i.aw_ready) begin
290:           update_aw_state = 1'b1;
291:           state_aw_d      = isolate_i ? Drain : Normal;
292:         end
293:       end
294:       Drain: begin // cut the AW channel until counter is zero
295:         mst_req_o.aw        = '0;
296:         mst_req_o.aw_valid  = 1'b0;
297:         slv_resp_o.aw_ready = 1'b0;
298:         if (pending_aw_q == '0) begin
299:           state_aw_d      = Isolate;
300:           update_aw_state = 1'b1;
301:         end
302:       end
```

W cut condition in `axi_isolate.sv:318-323`:

```systemverilog
318:     // W channel is cut as long the counter is zero and not explicitly unlocked through an AW.
319:     if ((pending_w_q == '0) && !connect_w ) begin
320:       mst_req_o.w         = '0;
321:       mst_req_o.w_valid   = 1'b0;
322:       slv_resp_o.w_ready  = 1'b0;
323:     end
```

AR states in `axi_isolate.sv:328-366`:

```systemverilog
328:     unique case (state_ar_q)
329:       Normal: begin
330:         // cut handshake if counter capacity is reached
331:         if (pending_ar_q >= NumPending) begin
332:           mst_req_o.ar_valid  = 1'b0;
333:           slv_resp_o.ar_ready = 1'b0;
334:           if (isolate_i) begin
335:             state_ar_d      = Drain;
336:             update_ar_state = 1'b1;
337:           end
338:         end else begin
339:           // here the AR handshake is connected normally
340:           if (slv_req_i.ar_valid && !mst_resp_i.ar_ready) begin
341:             state_ar_d      = Hold;
342:             update_ar_state = 1'b1;
343:           end else begin
344:             if (isolate_i) begin
345:               state_ar_d      = Drain;
346:               update_ar_state = 1'b1;
347:             end
348:           end
349:         end
350:       end
351:       Hold: begin // Hold the valid signal on 1'b1 if there was no transfer
352:         mst_req_o.ar_valid = 1'b1;
353:         // ar_ready normal connected
354:         if (mst_resp_i.ar_ready) begin
355:           update_ar_state = 1'b1;
356:           state_ar_d      = isolate_i ? Drain : Normal;
357:         end
358:       end
359:       Drain: begin
360:         mst_req_o.ar        = '0;
361:         mst_req_o.ar_valid  = 1'b0;
362:         slv_resp_o.ar_ready = 1'b0;
363:         if (pending_ar_q == '0) begin
364:           state_ar_d      = Isolate;
365:           update_ar_state = 1'b1;
366:         end
367:       end
```

Top-level isolate OR in `axi_rt_unit.sv:415`:

```systemverilog
415:   assign isolated_o = tail_isolated | isolated;
```

Interpretation:

- Held AW/AR: `Hold` state sets `mst_req_o.aw_valid = 1'b1` / `mst_req_o.ar_valid = 1'b1` and waits for downstream ready. It does not drop an already-presented VALID.
- Source future reads: in `Normal`, if `pending_ar_q >= NumPending` or `isolate_i`, `ar_valid` and `ar_ready` are cut and the FSM goes to `Drain`. In `Drain`, AR is zeroed and `ar_ready` is 0 until `pending_ar_q == 0`. In `Isolate`, AR is zeroed. So no new source AR is accepted after isolation.
- Existing reads: `pending_ar_q` decrements only on R last (`axi_isolate.sv:240-243`). `RREADY` is `mst_req_o.r_ready = slv_req_i.r_ready` by default (`axi_isolate.sv:255`); in `Drain` it is not overridden, so RREADY follows the upstream master. Stage 41 confirms: `codex-critical-acceptance.md:49` “RREADY仍取决于master”.
- Existing W: `pending_w_q` increments on accepted AW and decrements on W last. W is cut only when `pending_w_q == 0 && !connect_w` (`axi_isolate.sv:318-323`). Therefore existing W for accepted AW can continue during drain.
- Existing B/R: `Drain` does not cut B/R; `Isolate` does silence them (`axi_isolate.sv:303-314,368-379`).
- The top module has both head and tail isolation. `isolated_o` is the OR of head and tail (`axi_rt_unit.sv:415`). Stage 41 line 49 warns that tail `isolated` allows splitter parent metadata to remain, and that the top OR does not imply all parent slots are empty.

### 2.6 Full W buffering vs R→W continuation

`axi_write_buffer.sv:59-65` (AR/R bypass):

```systemverilog
59:   assign mst_req_o.ar        = slv_req_i.ar;
60:   assign mst_req_o.ar_valid  = slv_req_i.ar_valid;
61:   assign slv_resp_o.ar_ready = mst_resp_i.ar_ready;
62: 
63:   assign slv_resp_o.r        = mst_resp_i.r;
64:   assign slv_resp_o.r_valid  = mst_resp_i.r_valid;
65:   assign mst_req_o.r_ready   = slv_req_i.r_ready;
```

W FIFO in `axi_write_buffer.sv:130-146`:

```systemverilog
130:   stream_fifo #(
131:     .DEPTH      ( WBufferDepth ),
132:     .T          ( w_chan_t     ),
133:     .ADDR_DEPTH ( IdxWWidth    )
134:   ) i_stream_fifo_w (
135:     .clk_i,
136:     .rst_ni,
137:     .flush_i    ( 1'b0               ),
138:     .testmode_i ( 1'b0               ),
139:     .usage_o    ( num_w_stored_o     ),
140:     .data_i     ( slv_req_i.w        ),
141:     .valid_i    ( slv_req_i.w_valid  ),
142:     .ready_o    ( slv_resp_o.w_ready ),
143:     .data_o     ( mst_req_o.w        ),
144:     .valid_o    ( egress_w_valid     ),
145:     .ready_i    ( egress_w_ready     )
146:   );
```

AW FIFO and W-last queue gating in `axi_write_buffer.sv:76-102` and `108-123`:

```systemverilog
76:   stream_fifo #(
77:     .DEPTH      ( NumOutstanding ),
78:     .T          ( aw_chan_t      )
79:   ) i_stream_fifo_aw (
...
87:     .ready_o    ( slv_resp_o.aw_ready ),
...
95:   stream_join #(
96:     .N_INP       ( 32'd2  )
97:   ) i_stream_join_aw (
98:     .inp_valid_i ( {mgmt_valid_aw, egress_aw_valid} ),
...
100:     .oup_valid_o ( mst_req_o.aw_valid               ),
101:     .oup_ready_i ( mst_resp_i.aw_ready              )
102:   );
...
108:   stream_fifo #(
109:     .DEPTH ( NumOutstanding ),
110:     .T     ( logic          )
111:   ) i_stream_fifo (
...
121:     .valid_o    ( mgmt_valid_aw      ),
...
```

Stage 41 context: `codex-critical-acceptance.md:65`:

```text
65: 不能把整transaction完成当作所有依赖的节点：一个2beat R→2beat W、FIFO仅1beat、AW可服务的普通流式copy，R完整完成要等W腾空间、W完整完成要等R数据；粗图画成环，实际`R0/W0/R1/W1`可正常前进。后续模型必须保留beat或已可消费prefix milestone。此纸面反例由第二轮proof席提出，根同意；不是新的DUT运行。
```

Stage 41 context: `codex-critical-acceptance.md:69`:

```text
69: 这个反例还要求没有其它可达的payload停车/消耗路径；若shared W FIFO允许先收数据，该容量及其是否须回收也要纳入。不能只用名义DMA24B忽略共享W存储，声称已证明当前系统同样停滞。
```

Interpretation:

- `axi_write_buffer` has a parameterized W data FIFO of depth `WBufferDepth`. W ingress is accepted only when `slv_resp_o.w_ready` is high, which comes from `i_stream_fifo_w.ready_o` (`axi_write_buffer.sv:142`).
- Full W buffering is not guaranteed by the code; it is a parameter choice. The code does not compare `WBufferDepth` to the pending W bytes of a burst or reserve space per R.
- If `WBufferDepth` is large enough to absorb all W beats that depend on a source R burst, then for those beats the downstream W-drain dependency can be decoupled. In the stage 41 2-beat example, a 2-beat W buffer would make that particular R→W continuation irrelevant; a 1-beat buffer would not.
- However, other resources still matter: AW FIFO depth `NumOutstanding`, W-last queue depth `NumOutstanding`, downstream AW/B readiness, isolation counters, and any upstream DMA/internal storage. The write buffer bypasses AR/R, so it does not buffer read data.
- Therefore, “full W buffering makes R→W continuation irrelevant” is conditional and not established by these files alone. It is only true for the buffered W bytes if `WBufferDepth` covers them and other dependencies permit. Stage 41 line 69 explicitly says shared W FIFO capacity must be included, not ignored.

### 2.7 Frozen revision vs paper per-beat wording

`sources.json:2`:

```json
2:   "axi_rt_commit": "50153a346b753dc2bc7723c446656a43db35d02d",
```

`sources.json:29-32`:

```json
29:     {
30:       "path": "axi_isolate.sv",
31:       "url": "https://raw.githubusercontent.com/pulp-platform/axi/fccffb5953ec8564218ba05e20adbedec845e014/src/axi_isolate.sv",
32:       "sha256": "b27fda71838b4433dfa4d8d559c8eaea5ffff742894ccd875cae34e63e08cdb4",
```

`codex-critical-acceptance.md:47`:

```text
47: AXI-REALM扩展论文 arXiv:2501.10161，§III-A/III-B；官方axi_rt冻结`50153a346b753dc2bc7723c446656a43db35d02d`，依赖axi`fccffb5953ec8564218ba05e20adbedec845e014`。根直接核`axi_rt_unit.sv:197–219,278–282,352–385,393–415`与`axi_isolate.sv:245–378`、论文txt368–397/414–443。当前RTL在物理AR/AW握手按整burst bytes扣预算；不要把论文的per-beat文字直接写成该代码扣费位置。代码冻结版未证明等于论文评估revision。
```

Interpretation:

- The frozen revision is explicitly identified by commit `50153a...`.
- The supplied code charges full downstream Ax burst bytes at the AR/AW handshake (`axi_rt_unit.sv:278-282`; `axi_rt_unit_counter.sv:84-87`). There is no per-beat budget decrement in the supplied modules.
- Stage 41 explicitly warns that the paper’s per-beat wording must not be mapped onto this code’s charge location, and that the frozen code is not proven equal to the paper evaluation revision.
- The frozen code has enable/abort-style controls: `rt_enable_i` (`axi_rt_unit.sv:54`), `imtu_enable_i` (`axi_rt_unit.sv:70`), and `imtu_abort_i` (`axi_rt_unit.sv:71`). `imtu_enable_i` gates counter `enable_i` and `global_isolate` (`axi_rt_unit.sv:219,361,379`); `imtu_abort_i` reloads the period (`axi_rt_unit_counter.sv:54`). These are not per-beat throttles.
- The 2023 paper text itself is not in the provided bundle. The only supplied paper-to-code reconciliation is stage 41 line 47.

### 2.8 Ordinary baselines visible in the supplied code

These are ordinary mechanisms, not novelty claims:

1. **Per-channel pending-transaction cap**  
   `axi_isolate.sv:41-42` defines `NumPending` as “Maximum number of pending requests per channel.”  
   `axi_isolate.sv:265-266` cuts AW when `pending_aw_q >= NumPending` or `pending_ar_q >= 2*NumPending` or `pending_w_q >= NumPending`.  
   `axi_isolate.sv:331` cuts AR when `pending_ar_q >= NumPending`.

2. **FIFO occupancy caps for AW/W**  
   `axi_write_buffer.sv:76-79`: AW FIFO depth `NumOutstanding`.  
   `axi_write_buffer.sv:108-110`: W-last queue depth `NumOutstanding`.  
   `axi_write_buffer.sv:130-146`: W data FIFO depth `WBufferDepth`; `num_w_stored_o` exposes usage.  
   This is a beat/word occupancy cap, not a byte counter.

3. **Per-AXI-ID burst-length counters**  
   `axi_gran_burst_splitter_ax_chan.sv:14-15` says burst lengths are stored in counters associated to AXI IDs through ID queues.  
   `axi_gran_burst_splitter_ax_chan.sv:56-75` connects `alloc_id_i`, `alloc_len_i`, `cnt_id_i`, `cnt_len_o` to `axi_gran_burst_splitter_counters`.  
   This is ordinary per-ID queue/prefix bookkeeping. The counters module itself is not supplied.

4. **Per-region byte budget/period token bucket**  
   `axi_rt_unit_counter.sv:37-57`: period counter.  
   `axi_rt_unit_counter.sv:63-90`: budget delta counter, `bytes_spent`, `budget_en`, `budget_spent_o`.  
   This is an ordinary periodic byte-credit mechanism, charged at Ax handshake and used to trigger isolation.

5. **Outstanding transaction caps in splitter**  
   `axi_gran_burst_splitter.sv:80-87` computes `MaxTxns` from `MaxReadTxns`/`MaxWriteTxns` and passes `MaxTrans` to `axi_demux_simple`.  
   `axi_gran_burst_splitter.sv:157-179` and `289-311` pass `MaxWriteTxns`/`MaxReadTxns` to the AW/AR Ax-channel modules.  
   This is an ordinary outstanding-transaction cap.

6. **External owner/prefix baseline**  
   Stage 41 line 43 says root checked vendor `axi_mux.sv` and that it pushes owner when `!w_fifo_full && aw_valid` and locks AW when `mst_aw_ready` is 0. That is an external ordinary per-owner queue-prefix/lock baseline. The file is not in this bundle.  
   Stage 41 line 51 cites Arm DMA350 TRM full R/W-space reservation as an ordinary strong control. That is also not in the supplied RTL bundle.

## 3. Direct answer to the candidate-exclusion question

The candidate “before first irrevocable AW offer, admit only if remaining continuation burden is within configured budget” is **not implemented in the supplied frozen AXI‑REALM code**:

- Budget is charged **after** the downstream AW handshake at `fwd_req.aw_valid & fwd_resp.aw_ready` (`axi_rt_unit.sv:281`), which is the `axi_write_buffer` master output, not the upstream first AW offer.
- An AW can be accepted into the write buffer AW FIFO before that charge point (`axi_write_buffer.sv:76-91`; `axi_rt_unit.sv:256-272`).
- There is no computation of “remaining continuation burden” in these files.
- `budget_left_o` only drives `budget_spent_o` and isolation; it does not influence `NumPending`, `MaxReadTxns`, `MaxWriteTxns`, or `NumOutstanding`.
- `NumPending` is a transaction/burst count, not a pending-byte or continuation-burden budget.
- There is no source-to-dest mapping or cross-manager queue-prefix metadata in these modules.
- The paper’s per-beat wording is not evidence for this frozen code; stage 41 line 47 explicitly separates them.

The strongest ordinary existing mechanisms here are: per-region byte credit + global isolation, per-channel pending-transaction caps, FIFO occupancy caps, per-AXI-ID burst-length counters, and outstanding-transaction caps. Codex should judge equivalence and innovation.

## 4. Uncertainties and next exact files

1. `axi_gran_burst_splitter_counters.sv` is **not supplied**. Next file to read if exact per-ID counter allocation/free semantics are needed. Current evidence is only `axi_gran_burst_splitter_ax_chan.sv:14-15,56-75`.
2. `addr_decode_dync` and the actual `rt_rule_t` definition are **not supplied**. Next files to read if whether a rule type can contain source/dest metadata is needed. Current usage in `axi_rt_unit.sv:290-322` exposes only `addr_i`, `addr_map_i`, `idx_o`, `dec_error_o`, `config_ongoing_i`.
3. `stream_fifo` and `stream_join` are **not supplied**. Next files to read to confirm exact full/ready/usage semantics behind `axi_write_buffer.sv:76-177`.
4. `delta_counter` is **not supplied**. Next file to read to confirm exact load/en/overflow semantics behind `axi_rt_unit_counter.sv:37-81`.
5. Vendor `axi_mux.sv` is **not in this bundle**. Stage 41 line 43 cites `cva6/vendor/pulp-platform/axi/src/axi_mux.sv:207–235` and the IOMMU vendor copy `:287–314`. Next files to read if the external per-owner queue-prefix baseline must be quoted directly.
6. The actual 2023/2026 paper text is **not in this bundle**. Next file/step: obtain the exact arXiv revision and sections if per-beat vs full-burst wording must be compared. Stage 41 line 47 says the frozen code is not proven equal to the paper evaluation revision.
7. Top-level DMA/DUT integration files are **not in this bundle**. Next files if actual `WBufferDepth`, `NumPending`, budgets, and whether full W buffering is configured must be confirmed: stage 41 lines 28-39 list `idma_axi_transport_layer.sv`, `trans_ctl_write_limit.sv`, `dma_core_wrap.sv`, `idma_reg64_frontend.sv`, `idma_fullbuffer_certificate.sv`, `compact_pretl_owner_fragment.sv`, `multi_actual_dma_top.sv`. These are outside the frozen AXI‑REALM source bundle.
8. `axi_demux`, `axi_err_slv`, and the internals of `axi_isolate_inner` beyond `axi_isolate.sv` are not fully supplied; only the `axi_isolate.sv` file is provided.