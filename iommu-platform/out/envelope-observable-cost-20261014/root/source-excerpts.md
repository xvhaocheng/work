# 可观察输入与最小成本：本轮关键原文

定向读取与原文身份，非实验/旧trace或硬件重新绑定。

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:155

SHA256 `613f044b4cd2519128dab4b5ac44d1edfb9d53cd84a0c3c4f84d1ea2062bd852`；actual per-bank split, input no-spill ownership, output W queue parameters。

```text
155:     end
156: 
157:     //=========================================================================
158:     // Per-bank configuration: NumOutstandingTrans and WFifoDepth are SPLIT,
159:     // everything else (caches, fault/MRIF FIFOs, widths, ...) is unchanged.
160:     // MSITrans is preserved (no MRIF); InclATS is forced low.
161:     //=========================================================================
162:     localparam rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfgBank = rv_iommu_cfg::rv_iommu_cfg_t'{
163:         NumOutstandingTrans : RVIOMMUCfg.NumOutstandingTrans / 2,
164:         WFifoDepth          : RVIOMMUCfg.WFifoDepth / 2,
165:         MrifFifoDepth       : RVIOMMUCfg.MrifFifoDepth,
166:         FaultFifoDepth      : RVIOMMUCfg.FaultFifoDepth,
167:         NumIotlbEntries     : RVIOMMUCfg.NumIotlbEntries,
168:         NumDdtcEntries      : RVIOMMUCfg.NumDdtcEntries,
169:         NumPdtcEntries      : RVIOMMUCfg.NumPdtcEntries,
170:         NumMrifcEntries     : RVIOMMUCfg.NumMrifcEntries,
171:         InclPC              : RVIOMMUCfg.InclPC,
172:         InclAxiBC           : RVIOMMUCfg.InclAxiBC,
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:236

SHA256 `613f044b4cd2519128dab4b5ac44d1edfb9d53cd84a0c3c4f84d1ea2062bd852`；actual per-bank split, input no-spill ownership, output W queue parameters。

```text
236:     //   * select is the IMMUTABLE ORIGINAL AXI ID bit BankIdBit (AW / AR)
237:     //   * no extra spills, no ID modification (IDs stay ORIGINAL width)
238:     //   * MaxTrans = original NumOutstandingTrans*2  (documented tracker
239:     //     capacity for the two banks together), UniqueIds = 0
240:     //=========================================================================
241:     logic aw_sel;
242:     logic ar_sel;
243:     assign aw_sel = tr_axi_req_i.aw.id[BankIdBit];
244:     assign ar_sel = tr_axi_req_i.ar.id[BankIdBit];
245: 
246:     // The input demux keeps W ownership until LAST. A source read from the
247:     // other bank can otherwise fill its DMA's small R->W FIFO while its AW
248:     // cannot enter that demux. The shared memory R channel then cannot return
249:     // the current owner's source data or page-table reads: a circular wait.
250:     // Delay SELECTING such reads, rather than withdrawing an AXI/TL VALID.
251:     // Writes and already-selected transactions retain their original protocol.
252:     // A visible new AW also reserves admission before its input handshake.
253:     // W may legally finish before a stalled AW is accepted, hence the signed
254:     // balance (minimum -1 for that reserved head), rather than an unsigned
255:     // AW-minus-LAST counter that could underflow.
256:     logic signed [31:0] w_unfilled_q;
257:     logic w_owner_q, read_owner, restrict_reads;
258:     logic [1:0] bank_read_admit;
259:     assign restrict_reads = (w_unfilled_q > 0) || tr_axi_req_i.aw_valid;
260:     assign read_owner = (w_unfilled_q > 0) ? w_owner_q : aw_sel;
261:     // If a new AW enters this bank on this edge, its FIFO head only becomes
262:     // visible on the next cycle. Do not select an older queued AR one cycle
263:     // ahead of that AW. When the AW FIFO is full (no AW handshake), allow the
264:     // current owner to read and drain already-prepared writes instead.
265:     assign bank_read_admit[0] = ((w_unfilled_q > 0) ? !read_owner : !restrict_reads)
266:                                 && !(aw_accept && !aw_sel);
267:     assign bank_read_admit[1] = ((w_unfilled_q > 0) ?  read_owner : !restrict_reads)
268:                                 && !(aw_accept &&  aw_sel);
269:     always_ff @(posedge clk_i or negedge rst_ni) begin
270:         if (!rst_ni) begin
271:             w_unfilled_q <= '0;
272:             w_owner_q <= 1'b0;
273:         end else begin
274:             w_unfilled_q <= w_unfilled_q + (aw_accept ? 32'sd1 : 32'sd0)
275:                                            - (w_last_accept ? 32'sd1 : 32'sd0);
276:             if (aw_accept && w_unfilled_q <= 0) w_owner_q <= aw_sel;
277:         end
278:     end
279: 
280:     axi_demux #(
281:         .AxiIdWidth     (RVIOMMUCfg.AxiIdWidth),
282:         .AtopSupport    (1'b1),
283:         .aw_chan_t      (axi_tr_aw_chan_t),
284:         .w_chan_t       (axi_tr_w_chan_t),
285:         .b_chan_t       (axi_tr_b_chan_t),
286:         .ar_chan_t      (axi_tr_ar_chan_t),
287:         .r_chan_t       (axi_tr_r_chan_t),
288:         .axi_req_t      (axi_tr_req_t),
289:         .axi_resp_t     (axi_tr_resp_t),
290:         .NoMstPorts     (32'd2),
291:         .MaxTrans       (RVIOMMUCfg.NumOutstandingTrans * 2),
292:         .AxiLookBits    (RVIOMMUCfg.AxiIdWidth),
293:         .UniqueIds      (1'b0),
294:         .SpillAw        (1'b0),
295:         .SpillW         (1'b0),
296:         .SpillB         (1'b0),
297:         .SpillAr        (1'b0),
298:         .SpillR         (1'b0)
299:     ) i_in_axi_demux (
300:         .clk_i,
301:         .rst_ni,
302:         .test_i          (1'b0),
303:         .slv_req_i       (tr_axi_req_i),
304:         .slv_aw_select_i (aw_sel),
305:         .slv_ar_select_i (ar_sel),
306:         .slv_resp_o      (tr_axi_resp_o),
307:         .mst_reqs_o      (bank_tr_req),
308:         .mst_resps_i     (bank_tr_resp)
309:     );
310: 
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:409

SHA256 `613f044b4cd2519128dab4b5ac44d1edfb9d53cd84a0c3c4f84d1ea2062bd852`；actual per-bank split, input no-spill ownership, output W queue parameters。

```text
409: 
410:     //=========================================================================
411:     // Outgoing completion mux
412:     //   slave ports: ORIGINAL completion types / ORIGINAL ID width
413:     //   master port: ORIGINAL_ID_WIDTH + 1 (bank bit prepended as MSB)
414:     //=========================================================================
415:     comp_req_wide_t  mux_mst_req;
416:     comp_resp_wide_t mux_mst_resp;
417: 
418:     axi_mux #(
419:         .SlvAxiIDWidth (RVIOMMUCfg.AxiIdWidth),
420:         .slv_aw_chan_t (axi_comp_aw_chan_t),
421:         .mst_aw_chan_t (comp_aw_chan_wide_t),
422:         .w_chan_t      (axi_comp_w_chan_t),
423:         .slv_b_chan_t  (axi_comp_b_chan_t),
424:         .mst_b_chan_t  (comp_b_chan_wide_t),
425:         .slv_ar_chan_t (axi_comp_ar_chan_t),
426:         .mst_ar_chan_t (comp_ar_chan_wide_t),
427:         .slv_r_chan_t  (axi_comp_r_chan_t),
428:         .mst_r_chan_t  (comp_r_chan_wide_t),
429:         .slv_req_t     (axi_comp_req_t),
430:         .slv_resp_t    (axi_comp_resp_t),
431:         .mst_req_t     (comp_req_wide_t),
432:         .mst_resp_t    (comp_resp_wide_t),
433:         .NoSlvPorts    (32'd2),
434:         .MaxWTrans     (RVIOMMUCfg.NumOutstandingTrans),
435:         .FallThrough   (1'b0),
436:         .SpillAw       (1'b0),
437:         .SpillW        (1'b0),
438:         .SpillB        (1'b0),
439:         .SpillAr       (1'b0),
440:         .SpillR        (1'b0)
441:     ) i_out_axi_mux (
442:         .clk_i,
443:         .rst_ni,
444:         .test_i      (1'b0),
445:         .slv_reqs_i  (bank_comp_req),
446:         .slv_resps_o (bank_comp_resp),
447:         .mst_req_o   (mux_mst_req),
448:         .mst_resp_i  (mux_mst_resp)
449:     );
450: 
451:     //-------------------------------------------------------------------------
452:     // Master -> external request conversion.
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv:140

SHA256 `0e25cd8fd1a406bbaf7c941f876a530f10016f445301d836abe14b3bf7784781`；IOMMU vendor: no bank switch with w_open, first-offer reservation and true WLAST。

```text
140:       slv_resp_o.aw_ready = 1'b0;
141:       aw_valid     = 1'b0;
142:       // `lock_aw_valid`, used to be protocol conform as it is not allowed to deassert
143:       // a valid if there was no corresponding ready. As this process has to be able to inject
144:       // an AXI ID into the counter of the AR channel on an ATOP, there could be a case where
145:       // this process waits on `aw_ready` but in the mean time on the AR channel the counter gets
146:       // full.
147:       lock_aw_valid_d = lock_aw_valid_q;
148:       load_aw_lock    = 1'b0;
149:       // AW ID counter and W FIFO
150:       w_cnt_up        = 1'b0;
151:       // ATOP injection into ar counter
152:       atop_inject     = 1'b0;
153:       // we had an arbitration decision, the valid is locked, wait for the transaction
154:       if (lock_aw_valid_q) begin
155:         aw_valid = 1'b1;
156:         // transaction
157:         if (aw_ready) begin
158:           slv_resp_o.aw_ready    = 1'b1;
159:           lock_aw_valid_d = 1'b0;
160:           load_aw_lock    = 1'b1;
161:           // inject the ATOP if necessary
162:           atop_inject     = slv_req_i.aw.atop[axi_pkg::ATOP_R_RESP] & AtopSupport;
163:         end
164:       end else begin
165:         // An AW can be handled if `i_aw_id_counter` and `i_counter_open_w` are not full.  An ATOP that
166:         // requires an R response can be handled if additionally `i_ar_id_counter` is not full (this
167:         // only applies if ATOPs are supported at all).
168:         if (!aw_id_cnt_full && (w_open != {IdCounterWidth{1'b1}}) &&
169:             (!(ar_id_cnt_full && slv_req_i.aw.atop[axi_pkg::ATOP_R_RESP]) ||
170:              !AtopSupport)) begin
171:           // There is a valid AW vector make the id lookup and go further, if it passes.
172:           // Also stall if previous transmitted AWs still have active W's in flight.
173:           // This prevents deadlocking of the W channel. The counters are there for the
174:           // Handling of the B responses.
175:           if (slv_req_i.aw_valid &&
176:                 ((w_open == '0) || (w_select == slv_aw_select_i)) &&
177:                 (!aw_select_occupied || (slv_aw_select_i == lookup_aw_select))) begin
178:             // connect the handshake
179:             aw_valid     = 1'b1;
180:             // push arbitration to the W FIFO regardless, do not wait for the AW transaction
181:             w_cnt_up     = 1'b1;
182:             // on AW transaction
183:             if (aw_ready) begin
184:               slv_resp_o.aw_ready = 1'b1;
185:               atop_inject  = slv_req_i.aw.atop[axi_pkg::ATOP_R_RESP] & AtopSupport;
186:             // no AW transaction this cycle, lock the decision
187:             end else begin
188:               lock_aw_valid_d = 1'b1;
189:               load_aw_lock    = 1'b1;
190:             end
191:           end
192:         end
193:       end
194:     end
195: 
196:     // lock the valid signal, as the selection gets pushed into the W FIFO on first assertion,
197:     // prevent further pushing
198:     `FFLARN(lock_aw_valid_q, lock_aw_valid_d, load_aw_lock, '0, clk_i, rst_ni)
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv:233

SHA256 `0e25cd8fd1a406bbaf7c941f876a530f10016f445301d836abe14b3bf7784781`；IOMMU vendor: no bank switch with w_open, first-offer reservation and true WLAST。

```text
233:     // `w_select` determines, which handshaking is connected.
234:     // AWs are only forwarded, if the counter is empty, or `w_select_q` is the same as
235:     // `slv_aw_select_i`.
236:     counter #(
237:       .WIDTH           ( IdCounterWidth ),
238:       .STICKY_OVERFLOW ( 1'b0           )
239:     ) i_counter_open_w (
240:       .clk_i,
241:       .rst_ni,
242:       .clear_i    ( 1'b0                  ),
243:       .en_i       ( w_cnt_up ^ w_cnt_down ),
244:       .load_i     ( 1'b0                  ),
245:       .down_i     ( w_cnt_down            ),
246:       .d_i        ( '0                    ),
247:       .q_o        ( w_open                ),
248:       .overflow_o ( /*not used*/          )
249:     );
250: 
251:     `FFLARN(w_select_q, slv_aw_select_i, w_cnt_up, select_t'(0), clk_i, rst_ni)
252:     assign w_select       = (|w_open) ? w_select_q : slv_aw_select_i;
253:     assign w_select_valid = w_cnt_up | (|w_open);
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv:410

SHA256 `0e25cd8fd1a406bbaf7c941f876a530f10016f445301d836abe14b3bf7784781`；IOMMU vendor: no bank switch with w_open, first-offer reservation and true WLAST。

```text
410:     assign ar_ready = ar_valid & mst_resps_i[slv_ar_select_i].ar_ready;
411:     assign aw_ready = aw_valid & mst_resps_i[slv_aw_select_i].aw_ready;
412: 
413:     // process that defines the individual demuxes and assignments for the arbitration
414:     // as mst_reqs_o has to be drivem from the same always comb block!
415:     always_comb begin
416:       // default assignments
417:       mst_reqs_o  = '0;
418:       slv_resp_o.w_ready = 1'b0;
419:       w_cnt_down  = 1'b0;
420: 
421:       for (int unsigned i = 0; i < NoMstPorts; i++) begin
422:         // AW channel
423:         mst_reqs_o[i].aw       = slv_req_i.aw;
424:         mst_reqs_o[i].aw_valid = 1'b0;
425:         if (aw_valid && (slv_aw_select_i == i)) begin
426:           mst_reqs_o[i].aw_valid = 1'b1;
427:         end
428: 
429:         //  W channel
430:         mst_reqs_o[i].w       = slv_req_i.w;
431:         mst_reqs_o[i].w_valid = 1'b0;
432:         if (w_select_valid && (w_select == i)) begin
433:           mst_reqs_o[i].w_valid = slv_req_i.w_valid;
434:           slv_resp_o.w_ready           = mst_resps_i[i].w_ready;
435:           w_cnt_down            = slv_req_i.w_valid & mst_resps_i[i].w_ready & slv_req_i.w.last;
436:         end
437: 
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_demux_simple.sv:140

SHA256 `0e25cd8fd1a406bbaf7c941f876a530f10016f445301d836abe14b3bf7784781`；CVA6 counterpart: same decisive W exclusion; not assumed whole module identical。

```text
140:       slv_resp_o.aw_ready = 1'b0;
141:       aw_valid     = 1'b0;
142:       // `lock_aw_valid`, used to be protocol conform as it is not allowed to deassert
143:       // a valid if there was no corresponding ready. As this process has to be able to inject
144:       // an AXI ID into the counter of the AR channel on an ATOP, there could be a case where
145:       // this process waits on `aw_ready` but in the mean time on the AR channel the counter gets
146:       // full.
147:       lock_aw_valid_d = lock_aw_valid_q;
148:       load_aw_lock    = 1'b0;
149:       // AW ID counter and W FIFO
150:       w_cnt_up        = 1'b0;
151:       // ATOP injection into ar counter
152:       atop_inject     = 1'b0;
153:       // we had an arbitration decision, the valid is locked, wait for the transaction
154:       if (lock_aw_valid_q) begin
155:         aw_valid = 1'b1;
156:         // transaction
157:         if (aw_ready) begin
158:           slv_resp_o.aw_ready    = 1'b1;
159:           lock_aw_valid_d = 1'b0;
160:           load_aw_lock    = 1'b1;
161:           // inject the ATOP if necessary
162:           atop_inject     = slv_req_i.aw.atop[axi_pkg::ATOP_R_RESP] & AtopSupport;
163:         end
164:       end else begin
165:         // An AW can be handled if `i_aw_id_counter` and `i_counter_open_w` are not full.  An ATOP that
166:         // requires an R response can be handled if additionally `i_ar_id_counter` is not full (this
167:         // only applies if ATOPs are supported at all).
168:         if (!aw_id_cnt_full && (w_open != {IdCounterWidth{1'b1}}) &&
169:             (!(ar_id_cnt_full && slv_req_i.aw.atop[axi_pkg::ATOP_R_RESP]) ||
170:              !AtopSupport)) begin
171:           // There is a valid AW vector make the id lookup and go further, if it passes.
172:           // Also stall if previous transmitted AWs still have active W's in flight.
173:           // This prevents deadlocking of the W channel. The counters are there for the
174:           // Handling of the B responses.
175:           if (slv_req_i.aw_valid &&
176:                 ((w_open == '0) || (w_select == slv_aw_select_i)) &&
177:                 (!aw_select_occupied || (slv_aw_select_i == lookup_aw_select))) begin
178:             // connect the handshake
179:             aw_valid     = 1'b1;
180:             // push arbitration to the W FIFO regardless, do not wait for the AW transaction
181:             w_cnt_up     = 1'b1;
182:             // on AW transaction
183:             if (aw_ready) begin
184:               slv_resp_o.aw_ready = 1'b1;
185:               atop_inject  = slv_req_i.aw.atop[axi_pkg::ATOP_R_RESP] & AtopSupport;
186:             // no AW transaction this cycle, lock the decision
187:             end else begin
188:               lock_aw_valid_d = 1'b1;
189:               load_aw_lock    = 1'b1;
190:             end
191:           end
192:         end
193:       end
194:     end
195: 
196:     // lock the valid signal, as the selection gets pushed into the W FIFO on first assertion,
197:     // prevent further pushing
198:     `FFLARN(lock_aw_valid_q, lock_aw_valid_d, load_aw_lock, '0, clk_i, rst_ni)
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_demux_simple.sv:233

SHA256 `0e25cd8fd1a406bbaf7c941f876a530f10016f445301d836abe14b3bf7784781`；CVA6 counterpart: same decisive W exclusion; not assumed whole module identical。

```text
233:     // `w_select` determines, which handshaking is connected.
234:     // AWs are only forwarded, if the counter is empty, or `w_select_q` is the same as
235:     // `slv_aw_select_i`.
236:     counter #(
237:       .WIDTH           ( IdCounterWidth ),
238:       .STICKY_OVERFLOW ( 1'b0           )
239:     ) i_counter_open_w (
240:       .clk_i,
241:       .rst_ni,
242:       .clear_i    ( 1'b0                  ),
243:       .en_i       ( w_cnt_up ^ w_cnt_down ),
244:       .load_i     ( 1'b0                  ),
245:       .down_i     ( w_cnt_down            ),
246:       .d_i        ( '0                    ),
247:       .q_o        ( w_open                ),
248:       .overflow_o ( /*not used*/          )
249:     );
250: 
251:     `FFLARN(w_select_q, slv_aw_select_i, w_cnt_up, select_t'(0), clk_i, rst_ni)
252:     assign w_select       = (|w_open) ? w_select_q : slv_aw_select_i;
253:     assign w_select_valid = w_cnt_up | (|w_open);
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_demux_simple.sv:410

SHA256 `0e25cd8fd1a406bbaf7c941f876a530f10016f445301d836abe14b3bf7784781`；CVA6 counterpart: same decisive W exclusion; not assumed whole module identical。

```text
410:     assign ar_ready = ar_valid & mst_resps_i[slv_ar_select_i].ar_ready;
411:     assign aw_ready = aw_valid & mst_resps_i[slv_aw_select_i].aw_ready;
412: 
413:     // process that defines the individual demuxes and assignments for the arbitration
414:     // as mst_reqs_o has to be drivem from the same always comb block!
415:     always_comb begin
416:       // default assignments
417:       mst_reqs_o  = '0;
418:       slv_resp_o.w_ready = 1'b0;
419:       w_cnt_down  = 1'b0;
420: 
421:       for (int unsigned i = 0; i < NoMstPorts; i++) begin
422:         // AW channel
423:         mst_reqs_o[i].aw       = slv_req_i.aw;
424:         mst_reqs_o[i].aw_valid = 1'b0;
425:         if (aw_valid && (slv_aw_select_i == i)) begin
426:           mst_reqs_o[i].aw_valid = 1'b1;
427:         end
428: 
429:         //  W channel
430:         mst_reqs_o[i].w       = slv_req_i.w;
431:         mst_reqs_o[i].w_valid = 1'b0;
432:         if (w_select_valid && (w_select == i)) begin
433:           mst_reqs_o[i].w_valid = slv_req_i.w_valid;
434:           slv_resp_o.w_ready           = mst_resps_i[i].w_ready;
435:           w_cnt_down            = slv_req_i.w_valid & mst_resps_i[i].w_ready & slv_req_i.w.last;
436:         end
437: 
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:164

SHA256 `a8c330c2efe8d758071b0c183fa7d2385171d85c2f81cc9734b7f83ab07b27ba`；IOMMU vendor: W slot reserved on AW firstVALID, stores only bank/switch index。

```text
164:     mst_r_chan_t  [NoSlvPorts-1:0] slv_r_chans;
165:     logic         [NoSlvPorts-1:0] slv_r_valids,  slv_r_readies;
166: 
167:     // These signals are all ID prepended
168:     // AW channel
169:     mst_aw_chan_t   mst_aw_chan;
170:     logic           mst_aw_valid, mst_aw_ready;
171: 
172:     // AW master handshake internal, so that we are able to stall, if w_fifo is full
173:     logic           aw_valid,     aw_ready;
174: 
175:     // FF to lock the AW valid signal, when a new arbitration decision is made the decision
176:     // gets pushed into the W FIFO, when it now stalls prevent subsequent pushing
177:     // This FF removes AW to W dependency
178:     logic           lock_aw_valid_d, lock_aw_valid_q;
179:     logic           load_aw_lock;
180: 
181:     // signals for the FIFO that holds the last switching decision of the AW channel
182:     logic           w_fifo_full,  w_fifo_empty;
183:     logic           w_fifo_push,  w_fifo_pop;
184:     switch_id_t     w_fifo_data;
185: 
186:     // W channel spill reg
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:282

SHA256 `a8c330c2efe8d758071b0c183fa7d2385171d85c2f81cc9734b7f83ab07b27ba`；IOMMU vendor: W slot reserved on AW firstVALID, stores only bank/switch index。

```text
282: 
283:     // control of the AW channel
284:     always_comb begin
285:       // default assignments
286:       lock_aw_valid_d = lock_aw_valid_q;
287:       load_aw_lock    = 1'b0;
288:       w_fifo_push     = 1'b0;
289:       mst_aw_valid    = 1'b0;
290:       aw_ready        = 1'b0;
291:       // had a downstream stall, be valid and send the AW along
292:       if (lock_aw_valid_q) begin
293:         mst_aw_valid = 1'b1;
294:         // transaction
295:         if (mst_aw_ready) begin
296:           aw_ready        = 1'b1;
297:           lock_aw_valid_d = 1'b0;
298:           load_aw_lock    = 1'b1;
299:         end
300:       end else begin
301:         if (!w_fifo_full && aw_valid) begin
302:           mst_aw_valid = 1'b1;
303:           w_fifo_push = 1'b1;
304:           if (mst_aw_ready) begin
305:             aw_ready = 1'b1;
306:           end else begin
307:             // go to lock if transaction not in this cycle
308:             lock_aw_valid_d = 1'b1;
309:             load_aw_lock    = 1'b1;
310:           end
311:         end
312:       end
313:     end
314: 
315:     `FFLARN(lock_aw_valid_q, lock_aw_valid_d, load_aw_lock, '0, clk_i, rst_ni)
316: 
317:     fifo_v3 #(
318:       .FALL_THROUGH ( FallThrough ),
319:       .DEPTH        ( MaxWTrans   ),
320:       .dtype        ( switch_id_t )
321:     ) i_w_fifo (
322:       .clk_i     ( clk_i                                     ),
323:       .rst_ni    ( rst_ni                                    ),
324:       .flush_i   ( 1'b0                                      ),
325:       .testmode_i( test_i                                    ),
326:       .full_o    ( w_fifo_full                               ),
327:       .empty_o   ( w_fifo_empty                              ),
328:       .usage_o   (                                           ),
329:       .data_i    ( mst_aw_chan.id[SlvAxiIDWidth+:MstIdxBits] ),
330:       .push_i    ( w_fifo_push                               ),
331:       .data_o    ( w_fifo_data                               ),
332:       .pop_i     ( w_fifo_pop                                )
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:350

SHA256 `a8c330c2efe8d758071b0c183fa7d2385171d85c2f81cc9734b7f83ab07b27ba`；IOMMU vendor: W slot reserved on AW firstVALID, stores only bank/switch index。

```text
350:     // W Channel
351:     //--------------------------------------
352:     // multiplexer
353:     assign mst_w_chan = slv_w_chans[w_fifo_data];
354:     always_comb begin
355:       // default assignments
356:       mst_w_valid   = 1'b0;
357:       slv_w_readies = '0;
358:       w_fifo_pop    = 1'b0;
359:       // control
360:       if (!w_fifo_empty) begin
361:         // connect the handshake
362:         mst_w_valid                = slv_w_valids[w_fifo_data];
363:         slv_w_readies[w_fifo_data] = mst_w_ready;
364:         // pop FIFO on a last transaction
365:         w_fifo_pop = slv_w_valids[w_fifo_data] & mst_w_ready & mst_w_chan.last;
366:       end
367:     end
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_mux.sv:96

SHA256 `71b6b0187138fe1a659154f939678a92b1b36c3db7a5b74f7b64329beb107e97`；CVA6 counterpart: same W reservation/pop semantics。

```text
96:     // AW master handshake internal, so that we are able to stall, if w_fifo is full
97:     logic           aw_valid,     aw_ready;
98: 
99:     // FF to lock the AW valid signal, when a new arbitration decision is made the decision
100:     // gets pushed into the W FIFO, when it now stalls prevent subsequent pushing
101:     // This FF removes AW to W dependency
102:     logic           lock_aw_valid_d, lock_aw_valid_q;
103:     logic           load_aw_lock;
104: 
105:     // signals for the FIFO that holds the last switching decision of the AW channel
106:     logic           w_fifo_full,  w_fifo_empty;
107:     logic           w_fifo_push,  w_fifo_pop;
108:     switch_id_t     w_fifo_data;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_mux.sv:208

SHA256 `71b6b0187138fe1a659154f939678a92b1b36c3db7a5b74f7b64329beb107e97`；CVA6 counterpart: same W reservation/pop semantics。

```text
208:     always_comb begin
209:       // default assignments
210:       lock_aw_valid_d = lock_aw_valid_q;
211:       load_aw_lock    = 1'b0;
212:       w_fifo_push     = 1'b0;
213:       mst_aw_valid    = 1'b0;
214:       aw_ready        = 1'b0;
215:       // had a downstream stall, be valid and send the AW along
216:       if (lock_aw_valid_q) begin
217:         mst_aw_valid = 1'b1;
218:         // transaction
219:         if (mst_aw_ready) begin
220:           aw_ready        = 1'b1;
221:           lock_aw_valid_d = 1'b0;
222:           load_aw_lock    = 1'b1;
223:         end
224:       end else begin
225:         if (!w_fifo_full && aw_valid) begin
226:           mst_aw_valid = 1'b1;
227:           w_fifo_push = 1'b1;
228:           if (mst_aw_ready) begin
229:             aw_ready = 1'b1;
230:           end else begin
231:             // go to lock if transaction not in this cycle
232:             lock_aw_valid_d = 1'b1;
233:             load_aw_lock    = 1'b1;
234:           end
235:         end
236:       end
237:     end
238: 
239:     `FFLARN(lock_aw_valid_q, lock_aw_valid_d, load_aw_lock, '0, clk_i, rst_ni)
240: 
241:     fifo_v3 #(
242:       .FALL_THROUGH ( FallThrough ),
243:       .DEPTH        ( MaxWTrans   ),
244:       .dtype        ( switch_id_t )
245:     ) i_w_fifo (
246:       .clk_i     ( clk_i                                     ),
247:       .rst_ni    ( rst_ni                                    ),
248:       .flush_i   ( 1'b0                                      ),
249:       .testmode_i( test_i                                    ),
250:       .full_o    ( w_fifo_full                               ),
251:       .empty_o   ( w_fifo_empty                              ),
252:       .usage_o   (                                           ),
253:       .data_i    ( mst_aw_chan.id[SlvAxiIDWidth+:MstIdxBits] ),
254:       .push_i    ( w_fifo_push                               ),
255:       .data_o    ( w_fifo_data                               ),
256:       .pop_i     ( w_fifo_pop                                )
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_mux.sv:276

SHA256 `71b6b0187138fe1a659154f939678a92b1b36c3db7a5b74f7b64329beb107e97`；CVA6 counterpart: same W reservation/pop semantics。

```text
276:     // multiplexer
277:     assign mst_w_chan = slv_w_chans[w_fifo_data];
278:     always_comb begin
279:       // default assignments
280:       mst_w_valid   = 1'b0;
281:       slv_w_readies = '0;
282:       w_fifo_pop    = 1'b0;
283:       // control
284:       if (!w_fifo_empty) begin
285:         // connect the handshake
286:         mst_w_valid                = slv_w_valids[w_fifo_data];
287:         slv_w_readies[w_fifo_data] = mst_w_ready;
288:         // pop FIFO on a last transaction
289:         w_fifo_pop = slv_w_valids[w_fifo_data] & mst_w_ready & mst_w_chan.last;
290:       end
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux.sv:119

SHA256 `3c165a36d2efd88e0714d2e20dcea9375c0e211b0091252e6b082b884b9fd23b`；actual vendor packet/select spills and W channel cut。

```text
119:   spill_register #(
120:     .T       ( w_chan_t  ),
121:     .Bypass  ( ~SpillW   )
122:   ) i_w_spill_reg (
123:     .clk_i,
124:     .rst_ni,
125:     .valid_i ( slv_req_i.w_valid    ),
126:     .ready_o ( slv_resp_o.w_ready   ),
127:     .data_i  ( slv_req_i.w          ),
128:     .valid_o ( slv_req_cut.w_valid  ),
129:     .ready_i ( slv_resp_cut.w_ready ),
130:     .data_o  ( slv_req_cut.w        )
131:   );
132:   spill_register #(
133:     .T       ( ar_chan_t  ),
134:     .Bypass  ( ~SpillAr   )
135:   ) i_ar_spill_reg (
136:     .clk_i,
137:     .rst_ni,
138:     .valid_i ( slv_req_i.ar_valid    ),
139:     .ready_o ( slv_ar_ready_chan     ),
140:     .data_i  ( slv_req_i.ar          ),
141:     .valid_o ( slv_ar_valid_chan     ),
142:     .ready_i ( slv_resp_cut.ar_ready ),
143:     .data_o  ( slv_req_cut.ar        )
144:   );
145:   spill_register #(
146:     .T       ( select_t ),
147:     .Bypass  ( ~SpillAr )
148:   ) i_ar_sel_spill_reg (
149:     .clk_i,
150:     .rst_ni,
151:     .valid_i ( slv_req_i.ar_valid    ),
152:     .ready_o ( slv_ar_ready_sel      ),
153:     .data_i  ( slv_ar_select_i       ),
154:     .valid_o ( slv_ar_valid_sel      ),
155:     .ready_i ( slv_resp_cut.ar_ready ),
156:     .data_o  ( slv_ar_select         )
157:   );
158: 
159:   assign slv_resp_o.ar_ready  = slv_ar_ready_chan & slv_ar_ready_sel;
160:   assign slv_req_cut.ar_valid = slv_ar_valid_chan & slv_ar_valid_sel;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/vendor/pulp-platform/axi/src/axi_demux.sv:119

SHA256 `3c165a36d2efd88e0714d2e20dcea9375c0e211b0091252e6b082b884b9fd23b`；counterpart spill semantics; no credit/workload inference from copy location。

```text
119:   spill_register #(
120:     .T       ( w_chan_t  ),
121:     .Bypass  ( ~SpillW   )
122:   ) i_w_spill_reg (
123:     .clk_i,
124:     .rst_ni,
125:     .valid_i ( slv_req_i.w_valid    ),
126:     .ready_o ( slv_resp_o.w_ready   ),
127:     .data_i  ( slv_req_i.w          ),
128:     .valid_o ( slv_req_cut.w_valid  ),
129:     .ready_i ( slv_resp_cut.w_ready ),
130:     .data_o  ( slv_req_cut.w        )
131:   );
132:   spill_register #(
133:     .T       ( ar_chan_t  ),
134:     .Bypass  ( ~SpillAr   )
135:   ) i_ar_spill_reg (
136:     .clk_i,
137:     .rst_ni,
138:     .valid_i ( slv_req_i.ar_valid    ),
139:     .ready_o ( slv_ar_ready_chan     ),
140:     .data_i  ( slv_req_i.ar          ),
141:     .valid_o ( slv_ar_valid_chan     ),
142:     .ready_i ( slv_resp_cut.ar_ready ),
143:     .data_o  ( slv_req_cut.ar        )
144:   );
145:   spill_register #(
146:     .T       ( select_t ),
147:     .Bypass  ( ~SpillAr )
148:   ) i_ar_sel_spill_reg (
149:     .clk_i,
150:     .rst_ni,
151:     .valid_i ( slv_req_i.ar_valid    ),
152:     .ready_o ( slv_ar_ready_sel      ),
153:     .data_i  ( slv_ar_select_i       ),
154:     .valid_o ( slv_ar_valid_sel      ),
155:     .ready_i ( slv_resp_cut.ar_ready ),
156:     .data_o  ( slv_ar_select         )
157:   );
158: 
159:   assign slv_resp_o.ar_ready  = slv_ar_ready_chan & slv_ar_ready_sel;
160:   assign slv_req_cut.ar_valid = slv_ar_valid_chan & slv_ar_valid_sel;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:174

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`；credits do not equal remaining bytes; raw physical packet, source W FIFO and AW-first read selection。

```text
174:     typedef logic [$clog2(RVIOMMUCfg.NumOutstandingTrans):0] w_cnt_t;
175:     w_cnt_t w_cnt_q;
176:     logic incr_w_cnt;
177:     logic decr_w_cnt;
178:     assign incr_w_cnt = (axi_comp_req.aw_valid & axi_comp_resp.aw_ready);
179:     assign decr_w_cnt = (w_valid & w_ready & w_data.last);
180:     logic w_cnt_ovf;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:220

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`；credits do not equal remaining bytes; raw physical packet, source W FIFO and AW-first read selection。

```text
220:     assign w_ready = (w_cnt_q != '0) ? (axi_comp_resp.w_ready) : (1'b0);
221: 
222:     stream_fifo #(
223:         .DEPTH(RVIOMMUCfg.WFifoDepth),
224:         .T    (axi_tr_w_chan_t)
225:     ) i_w_stream_fifo (
226:         .clk_i      (clk_i),
227:         .rst_ni     (rst_ni),
228: 
229:         .flush_i    ('0),
230:         .testmode_i ('0),
231:         .usage_o    (  ),
232: 
233:         .data_i     (tr_axi_req_i.w),
234:         .valid_i    (tr_axi_req_i.w_valid),
235:         .ready_o    (tr_w_ready),
236: 
237:         .data_o     (w_data),
238:         .valid_o    (w_valid),
239:         .ready_i    (w_ready)
240:     );
241: 
242:     counter #(
243:         .WIDTH              ($clog2(RVIOMMUCfg.NumOutstandingTrans)+1),
244:         .STICKY_OVERFLOW    (1'b0)
245:     ) i_w_counter (
246:         .clk_i      (clk_i),
247:         .rst_ni     (rst_ni),
248:         .clear_i    (1'b0),
249:         .en_i       (incr_w_cnt ^ decr_w_cnt),
250:         .load_i     (1'b0),
251:         .down_i     (decr_w_cnt),
252:         .d_i        ('0),
253:         .q_o        (w_cnt_q),
254:         .overflow_o (w_cnt_ovf)
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:326

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`；credits do not equal remaining bytes; raw physical packet, source W FIFO and AW-first read selection。

```text
326:     logic complete_read;
327:     logic complete_write;
328: 
329:     always_comb begin : completion_bus
330: 
331:         /* Request */
332:         axi_comp_req.aw.id      = aw_data.id;
333:         axi_comp_req.aw.addr    = addr_t'(trans_resp_data_q.spaddr);
334:         axi_comp_req.aw.len     = aw_data.len;
335:         axi_comp_req.aw.size    = aw_data.size;
336:         axi_comp_req.aw.burst   = aw_data.burst;
337:         axi_comp_req.aw.lock    = aw_data.lock;
338:         axi_comp_req.aw.cache   = aw_data.cache;
339:         axi_comp_req.aw.prot    = aw_data.prot;
340:         axi_comp_req.aw.qos     = aw_data.qos;
341:         axi_comp_req.aw.region  = aw_data.region;
342:         axi_comp_req.aw.atop    = aw_data.atop;
343:         axi_comp_req.aw.user    = aw_data.user;
344:         axi_comp_req.aw_valid   = complete_write;
345: 
346:         axi_comp_req.ar.id      = ar_data.id;
347:         axi_comp_req.ar.addr    = addr_t'(trans_resp_data_q.spaddr);
348:         axi_comp_req.ar.len     = ar_data.len;
349:         axi_comp_req.ar.size    = ar_data.size;
350:         axi_comp_req.ar.burst   = ar_data.burst;
351:         axi_comp_req.ar.lock    = ar_data.lock;
352:         axi_comp_req.ar.cache   = ar_data.cache;
353:         axi_comp_req.ar.prot    = ar_data.prot;
354:         axi_comp_req.ar.qos     = ar_data.qos;
355:         axi_comp_req.ar.region  = ar_data.region;
356:         axi_comp_req.ar.user    = ar_data.user;
357:         axi_comp_req.ar_valid   = complete_read;
358: 
359:         axi_comp_req.w          = w_data;
360:         axi_comp_req.w_valid    = w_valid & (w_cnt_q != '0);
361:         axi_comp_req.r_ready    = tr_axi_req_i.r_ready;
362:         axi_comp_req.b_ready    = tr_axi_req_i.b_ready;
363: 
364:         /* Response */
365:         tr_axi_resp_o           = axi_comp_resp;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:540

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`；credits do not equal remaining bytes; raw physical packet, source W FIFO and AW-first read selection。

```text
540:         // AXI demux
541:         axi_demux #(
542:             .AxiIdWidth     (RVIOMMUCfg.AxiIdWidth),
543:             .aw_chan_t      (axi_comp_aw_chan_t),
544:             .w_chan_t       (axi_comp_w_chan_t),
545:             .b_chan_t       (axi_comp_b_chan_t),
546:             .ar_chan_t      (axi_comp_ar_chan_t),
547:             .r_chan_t       (axi_comp_r_chan_t),
548:             .axi_req_t      (axi_comp_req_t),
549:             .axi_resp_t     (axi_comp_resp_t),
550: 
551:             .NoMstPorts     (32'd2),
552:             .AxiLookBits    (RVIOMMUCfg.AxiIdWidth),
553:             .SpillAw        (1'b1),
554:             .SpillW         (1'b1),
555:             .SpillB         (1'b1),
556:             .SpillAr        (1'b1),
557:             .SpillR         (1'b1)
558:         ) i_iommu_axi_demux_no_mrif (
559:             .clk_i          (clk_i),
560:             .rst_ni         (rst_ni),
561:             .test_i         (1'b0),
562:             .slv_aw_select_i(slv_idx),
563:             .slv_ar_select_i(slv_idx),
564:             .slv_req_i      (axi_comp_req),
565:             .slv_resp_o     (axi_comp_resp),
566:             /* { 1: comp IF, 0: error slave } */
567:             .mst_reqs_o     ({comp_axi_req_o, axi_err_req}),
568:             .mst_resps_i    ({comp_axi_resp_i, axi_err_resp})
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:625

SHA256 `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`；credits do not equal remaining bytes; raw physical packet, source W FIFO and AW-first read selection。

```text
625:             IDLE: begin
626:                 // Write request received
627:                 // Stall writes if W FIFO is full
628:                 if (aw_valid && !w_cnt_ovf) begin
629: 
630:                     // Tags
631:                     trans_req_data_n.iova       = aw_data.addr;
632:                     trans_req_data_n.did        = aw_data.stream_id;
633:                     trans_req_data_n.pid_valid  = aw_data.ss_id_valid;
634:                     trans_req_data_n.pid        = aw_data.substream_id;
635:                     trans_req_data_n.ttype      = aw_data.mmu_atst && !aw_data.mmu_valid ?
636:                                                   rv_iommu::TRANSLATED_W :
637:                                                   rv_iommu::UNTRANSLATED_W;
638:                     // AxPROT[0] indicates privileged transaction when set
639:                     trans_req_data_n.priv       = aw_data.prot[0];
640:                     trans_req_data_n.is_debug   = 1'b0;
641: 
642:                     state_n = REQ_TRANS;
643:                 end
644: 
645:                 // Read request received
646:                 else if (ar_valid && (!ReadAdmissionEnable || read_admit_i ||
647:                          (fullbuffer_job_i && ar_data.stream_id == FullBufferDID))) begin
648:                     // Tags
649:                     trans_req_data_n.iova       = ar_data.addr;
650:                     trans_req_data_n.did        = ar_data.stream_id;
651:                     trans_req_data_n.pid_valid  = ar_data.ss_id_valid;
652:                     trans_req_data_n.pid        = ar_data.substream_id;
653:                     if(ar_data.mmu_atst && !ar_data.mmu_valid) begin
654:                        // ARPROT[2] indicates data access (r) when LOW, instruction access (rx) when HIGH
655:                        trans_req_data_n.ttype   = (ar_data.prot[2]) ?
656:                                                   (rv_iommu::TRANSLATED_RX) :
657:                                                   (rv_iommu::TRANSLATED_R);
658:                     end else begin
659:                        // ARPROT[2] indicates data access (r) when LOW, instruction access (rx) when HIGH
660:                        trans_req_data_n.ttype   = (ar_data.prot[2]) ?
661:                                                   (rv_iommu::UNTRANSLATED_RX) :
662:                                                   (rv_iommu::UNTRANSLATED_R);
663:                     end
664:                     // AxPROT[0] indicates privileged transaction when set
665:                     trans_req_data_n.priv       = ar_data.prot[0];
666:                     trans_req_data_n.is_debug   = 1'b0;
667: 
668:                     state_n = REQ_TRANS;
669:                 end
670:             end
671: 
672:             // Perform BC check
673:             // Request translation
674:             REQ_TRANS: begin
675: 
676:                 if (check_valid) begin
677: 
678:                     trans_req_valid_o = 1'b1;
679: 
680:                     if (trans_req_ready_i) begin
681:                         state_n     = AWAIT_RESP;
682:                     end
683:                 end
684: 
685:                 // Boundary violation
686:                 else begin
687:                     state_n = COMPLETE;
688:                     trans_resp_data_n.error = 1'b1;
689:                 end
690:             end
691: 
692:             // Await for a translation response
693:             AWAIT_RESP: begin
694: 
695:                 trans_resp_ready_o = 1'b1;
696: 
697:                 // Translation finished
698:                 if (trans_resp_valid_i) begin
699:                     trans_resp_data_n   = trans_resp_data_i;
700:                     state_n             = (trans_resp_data_i.is_mrif &&
701:                                            !trans_resp_data_i.error) ? (MRIF) : (COMPLETE);
702:                 end
703:             end
704: 
705:             // Trigger MRIF Handler
706:             MRIF: begin
707: 
708:                 mrif_req_valid_d = 1'b1;
709:                 if (mrif_req_ready_q) begin
710:                     state_n = COMPLETE;
711:                 end
712:             end
713: 
714:             // Complete transaction
715:             // Select slave
716:             COMPLETE: begin
717: 
718:                 complete_read = is_read;
719:                 complete_write = is_write;
720: 
721:                 if ((axi_comp_resp.ar_ready && is_read) ||
722:                     (axi_comp_resp.aw_ready && is_write)) begin
723:                     aw_ready    = is_write;
724:                     ar_ready    = is_read;
725:                     state_n     = IDLE;
726:                 end
727:             end
728: 
729:             default: begin
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:110

SHA256 `b1800c757d28f9c21b0a4a484d888313145c0805194dd7c7766e337f88c57df7`；exact33-bit job ordinal, event-only IDs, source options and sole-job lifetime。

```text
110:     logic observer_access;
111:     logic batch_access, locked_write;
112:     logic [7:0] full_addr;
113:     logic batch_enable_q, batch_stop_q, batch_stopped_q, batch_error_q;
114:     logic [32:0] batch_target_q, batch_issued_q, batch_completed_q;
115:     logic [12:0] batch_pages_q;
116:     logic        batch_reverse_q;
117:     logic [63:0] batch_id_q;
118:     logic batch_cfg_ok, batch_task_issue, batch_task_done;
119:     logic [31:0] gap_cycles_q;
120:     logic [31:0] gap_countdown_q, gap_countdown_n;
121:     logic gap_access;
122:     logic batch_stop_write;
123: 
124:     // Controlled-launch state
125:     logic launcher_access;
126:     logic launch_busy;
127:     logic launch_cfg_ok;
128:     logic launch_next_id_read;
129:     logic launch_base_write;
130:     logic launch_prepare_err;
131:     logic launch_locked_write;
132:     logic launch_ctrl_q, launch_ctrl_n;
133:     logic [DmaRegisterWidth-1:0] launch_id_q, launch_id_n;
134: 
135:     assign full_addr = 8'(dma_ctrl_req_i.addr);
136:     // The gap slots are exact BatchPort decodes and must be carved out before
137:     // the launcher window, otherwise d0/d8 would alias launcher b0..ff.
138:     assign gap_access = BatchPort && dma_ctrl_req_i.valid &&
139:                         (full_addr == GapCyclesOffset || full_addr == GapCapOffset);
140:     // Launcher window is carved out before the batch high-bank decode so 0xb0..0xff
141:     // never aliases the batch bank, except for the exact gap slots above.
142:     assign launcher_access = LaunchPort && ObserverPort && dma_ctrl_req_i.valid &&
143:                              (full_addr >= LauncherBase) && !gap_access;
144:     assign batch_access = BatchPort && dma_ctrl_req_i.valid && full_addr[7] && !launcher_access;
145:     assign batch_active_o = BatchPort && batch_enable_q && state_q != IDLE;
146:     // Passive observation of actual inserted wait; no control feedback.
147:     assign batch_gap_active_o = BatchPort && batch_enable_q && state_q == GAP;
148:     // Covers software jobs, queued backend work and a same-cycle launch read.
149:     assign job_quiet_o = state_q == IDLE && backend_idle_i && !valid_o &&
150:                          !issue && !dma_reg2hw.next_id.re;
151:     assign locked_write = batch_active_o && dma_ctrl_req_i.valid &&
152:                           dma_ctrl_req_i.write && !batch_access && !observer_access && !launcher_access;
153:     assign batch_cfg_ok = !batch_error_q && batch_target_q != 0 && batch_target_q <= 33'h100000000 &&
154:                           batch_pages_q != 0 && batch_pages_q <= 4096 &&
155:                           dma_reg2hw.num_bytes.q == 4096 &&
156:                           dma_reg2hw.conf.ttype.q == idma_pkg::DEFAULT &&
157:                           dma_reg2hw.src_addr.q[11:0] == 0 && dma_reg2hw.dst_addr.q[11:0] == 0;
158:     assign batch_task_issue = batch_active_o && state_q == COUNT && ready_i;
159:     assign batch_task_done = batch_active_o && state_q == WAIT && trans_complete_i;
160: 
161:     // The high 128-byte bank is explicitly decoded; the old register file remains 7-bit.
162:     // Configuration is immutable while active. STOP drains the currently offered task.
163:     always_ff @(posedge clk_i or negedge rst_ni) begin
164:         if (!rst_ni) begin
165:             batch_enable_q <= 0; batch_stop_q <= 0; batch_stopped_q <= 0;
166:             batch_error_q <= 0; batch_target_q <= 0; batch_pages_q <= 0;
167:             batch_issued_q <= 0; batch_completed_q <= 0; batch_id_q <= 0;
168:             batch_reverse_q <= 0;
169:             gap_cycles_q <= 0;
170:         end else if (BatchPort) begin
171:             if (batch_enable_q && issue) begin
172:                 batch_issued_q <= 0; batch_completed_q <= 0;
173:                 batch_stop_q <= 0; batch_stopped_q <= 0; batch_id_q <= next_id;
174:                 batch_reverse_q <= batch_reverse;
175:             end
176:             if (batch_task_issue) batch_issued_q <= batch_issued_q + 1'b1;
177:             if (batch_task_done) begin
178:                 batch_completed_q <= batch_completed_q + 1'b1;
179:                 if (batch_stop_q) batch_stopped_q <= 1;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:279

SHA256 `b1800c757d28f9c21b0a4a484d888313145c0805194dd7c7766e337f88c57df7`；exact33-bit job ordinal, event-only IDs, source options and sole-job lifetime。

```text
279:     // ordinary job, so it is a preflight "frontend busy" indicator. It is
280:     // intentionally wider than launch_ctrl_q (the controlled job only); use
281:     // launch_held_o / launch_busy to distinguish the controlled job.
282:     assign launch_active_o    = LaunchPort && (state_q == HOLD || state_q == COUNT ||
283:                                                state_q == WAIT || state_q == GAP ||
284:                                                state_q == ADMIT);
285: 
286:     assign obs_issue_o = batch_enable_q ? batch_task_issue :
287:                          (LaunchPort && (launch_ctrl_q || launch_hold_i)) ?
288:                             (launch_ctrl_q && state_q == COUNT && ready_i) : issue;
289:     assign obs_issue_id_o = batch_enable_q ? 64'(batch_issued_q) + 1 :
290:                             (LaunchPort && (launch_ctrl_q || launch_hold_i)) ?
291:                                (launch_ctrl_q ? launch_id_q : next_id) : next_id;
292:     assign obs_done_o = batch_enable_q ? batch_task_done : trans_complete;
293:     // For a controlled job report the reserved ID; otherwise keep the raw generator
294:     // behaviour (previous observer semantics).
295:     assign obs_done_id_o = batch_enable_q ? 64'(batch_completed_q) + 1 :
296:                            ((LaunchPort && launch_ctrl_q && trans_complete) ? launch_id_q :
297:                             ((&done_id) ? 64'd2 : done_id + 64'd1));
298:     assign obs_poll_o = dma_ctrl_req_i.valid && !dma_ctrl_req_i.write &&
299:                        !batch_access && !observer_access && !launcher_access &&
300:                        full_addr == 8'h30 && dma_ctrl_rsp_o.ready;
301:     assign obs_poll_id_o = batch_enable_q ?
302:         ((state_q == IDLE && done_id == batch_id_q) ? 64'(batch_completed_q) : 64'd0) : dma_ctrl_rsp_o.rdata;
303: 
304:     /*
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:321

SHA256 `b1800c757d28f9c21b0a4a484d888313145c0805194dd7c7766e337f88c57df7`；exact33-bit job ordinal, event-only IDs, source options and sole-job lifetime。

```text
321: 
322:         /*** TRANSFER PARAMETERS ***/
323:         burst_req_o                     = '0;
324: 
325:         burst_req_o.length              = dma_reg2hw.num_bytes.q;
326:         burst_req_o.src_addr            = dma_reg2hw.src_addr.q;
327:         burst_req_o.dst_addr            = dma_reg2hw.dst_addr.q;
328: 
329:         ttype_o                         = dma_reg2hw.conf.ttype.q;
330:         wdata_o                         = dma_reg2hw.wdata.q;
331: 
332:             // Current backend only supports one ID
333:         burst_req_o.opt.axi_id             = AxID;
334:             // Device ID
335:         burst_req_o.opt.device_id          = DEVICE_ID;
336:             // Process ID
337:         burst_req_o.opt.pv                 = dma_reg2hw.conf.pv.q;
338:         burst_req_o.opt.process_id         = (dma_reg2hw.conf.pv.q) ?
339:                                              (PROCESS_ID) :
340:                                              ('0);
341:             // DMA only supports incremental burst
342:         burst_req_o.opt.src.burst          = axi_pkg::BURST_INCR;
343:             // this frontend currently does not support cache variations
344:         burst_req_o.opt.src.cache          = dma_reg2hw.conf.cache.q;
345:             // AXI4 does not support locked transactions, use atomics
346:         burst_req_o.opt.src.lock           = '0;
347:             // unpriviledged, secure, data access
348:         burst_req_o.opt.src.prot           = '0;
349:             // not participating in qos
350:         burst_req_o.opt.src.qos            = '0;
351:             // only one region
352:         burst_req_o.opt.src.region         = '0;
353:             // DMA only supports incremental burst
354:         burst_req_o.opt.dst.burst          = axi_pkg::BURST_INCR;
355:             // this frontend currently does not support cache variations
356:         burst_req_o.opt.dst.cache          = dma_reg2hw.conf.cache.q;
357:             // AXI4 does not support locked transactions, use atomics
358:         burst_req_o.opt.dst.lock           = '0;
359:             // unpriviledged, secure, data access
360:         burst_req_o.opt.dst.prot           = '0;
361:             // not participating in qos
362:         burst_req_o.opt.dst.qos            = '0;
363:             // only one region in system
364:         burst_req_o.opt.dst.region         = '0;
365:             // ensure coupled AW to avoid deadlocks
366:         burst_req_o.opt.beo.decouple_aw    = '0;
367:         burst_req_o.opt.beo.decouple_rw    = dma_reg2hw.conf.decouple.q;
368:             // this frontend currently only supports completely debursting
369:         burst_req_o.opt.beo.src_max_llen   = '0;
370:             // this frontend currently only supports completely debursting
371:         burst_req_o.opt.beo.dst_max_llen   = '0;
372:         burst_req_o.opt.beo.src_reduce_len = dma_reg2hw.conf.deburst.q;
373:         burst_req_o.opt.beo.dst_reduce_len = dma_reg2hw.conf.deburst.q;
374: 
375:             // serialization no longer supported
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:517

SHA256 `b1800c757d28f9c21b0a4a484d888313145c0805194dd7c7766e337f88c57df7`；exact33-bit job ordinal, event-only IDs, source options and sole-job lifetime。

```text
517:             end
518: 
519:             COUNT: begin
520: 
521:                 // Send request to FIFO. VALID stays asserted (and addresses stable)
522:                 // regardless of later cancel/hold/go until the backend is ready.
523:                 valid_o              = 1'b1;
524: 
525:                 burst_req_o.src_addr = dma_reg2hw.src_addr.q + transf_offset_q;
526:                 burst_req_o.dst_addr = dma_reg2hw.dst_addr.q + transf_offset_q;
527: 
528:                 if (ready_i) begin
529:                     state_n          = WAIT;
530:                     transf_counter_n = transf_counter_q - 1;
531:                     if (batch_enable_q && batch_reverse_q) begin
532:                         if (transf_offset_q == 0)
533:                             transf_offset_n = (64'(batch_pages_q) - 64'd1) << 12;
534:                         else
535:                             transf_offset_n = transf_offset_q - 13'h1000;
536:                     end else begin
537:                         transf_offset_n  = transf_offset_q + 13'h1000;
538:                         if (batch_enable_q && transf_offset_n == (64'(batch_pages_q) << 12))
539:                             transf_offset_n = 0;
540:                     end
541:                 end
542:             end
543: 
544:             WAIT: begin
545: 
546:                 if (trans_complete_i) begin
547:                     if (launch_ctrl_q) begin
548:                         // Controlled job completion.
549:                         trans_complete = 1'b1;
550:                         id_retire      = 1'b1;
551:                         launch_ctrl_n  = 1'b0;
552:                         launch_id_n    = '0;
553:                         state_n        = IDLE;
554:                     end else if ((transf_counter_q == '0) ||
555:                             (!batch_enable_q && dma_reg2hw.conf.ttype.q == idma_pkg::DEFAULT) ||
556:                             (batch_enable_q && (batch_stop_q || policy_abort))) begin
557:                         trans_complete = 1'b1;
558:                         id_retire      = 1'b1;
559:                         state_n = IDLE;
560:                     end else if (batch_enable_q && gap_cycles_q != '0) begin
561:                         // Non-final batch completion: wait before offering the
562:                         // next task.  The first task is never delayed by gap.
563:                         state_n         = GAP;
564:                         gap_countdown_n = gap_cycles_q;
565:                     end else if (batch_enable_q && policy_enable) begin
566:                         state_n = ADMIT;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:1

SHA256 `7db52bf22bc9b8694698e9091dd390f1082c72a5b31ccf248b278315571a1b79`；native4KiB/512 eligibility and sole pending job, not a future W grant。

```text
1: // SPDX-License-Identifier: Solderpad-0.51
2: // Permission for one complete, aligned 4 KiB copy in a sufficiently large
3: // data buffer. Samples accepted frontend jobs and retired backend jobs only;
4: // it never changes either handshake. The register frontend serializes jobs.
5: // Unexpected overlap/retirement revokes this new permission until reset.
6: module idma_fullbuffer_certificate #(
7:     parameter int unsigned CapacityBytes = 0
8: ) (
9:     input logic clk_i, rst_ni,
10:     input logic job_fire_i, job_done_i, normal_copy_i,
11:     input logic [63:0] length_i, src_i, dst_i,
12:     output logic permit_o
13: );
14:     logic pending_q, eligible_q, fault_q;
15:     logic eligible;
16:     assign eligible = CapacityBytes >= 4096 && normal_copy_i &&
17:                       length_i == 64'd4096 && src_i[11:0] == 0 && dst_i[11:0] == 0;
18:     assign permit_o = pending_q && eligible_q && !fault_q && normal_copy_i;
19: 
20:     always_ff @(posedge clk_i or negedge rst_ni) begin
21:         if (!rst_ni) begin
22:             pending_q <= 1'b0;
23:             eligible_q <= 1'b0;
24:             fault_q <= 1'b0;
25:         end else begin
26:             case ({job_fire_i, job_done_i})
27:                 2'b10: begin
28:                     if (pending_q) fault_q <= 1'b1;
29:                     pending_q <= 1'b1;
30:                     eligible_q <= eligible;
31:                 end
32:                 2'b01: begin
33:                     if (!pending_q) fault_q <= 1'b1;
34:                     pending_q <= 1'b0;
35:                     eligible_q <= 1'b0;
36:                 end
37:                 2'b11: begin
38:                     // Replace a completed sole job; an immediately completed
39:                     // job with no predecessor needs no read permission.
40:                     pending_q <= pending_q;
41:                     eligible_q <= pending_q && eligible;
42:                 end
43:                 default: ;
44:             endcase
45:         end
46:     end
47: endmodule
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_legalizer.sv:140

SHA256 `f559a0b0b1d39a9c3ae02b064bf7a67f2b0872551f983d0cf22a4a94ce7c12e6`；mutable length is unissued request work; normal non-deburst two256-beat bursts。

```text
140:     always_comb begin : proc_read_addr_width
141:         // should the "virtual" page be reduced? e.g. the transfers split into
142:         // smaller chunks than the AXI page size?
143:         r_page_addr_width = OffsetWidth + (opt_tf_q.src_reduce_len ? opt_tf_q.src_max_llen : 'd8);
144:         // a page can be a maximum of 4kB (12 bit)
145:         r_page_addr_width = r_page_addr_width > PageAddrWidth ? PageAddrWidth : r_page_addr_width;
146:     end
147:     // calculate the page size in byte
148:     assign r_page_size = (1 << r_page_addr_width);
149: 
150:     // this is written very confusing due to system verilog not allowing variable
151:     // length ranges.
152:     // the goal is to get 'r_tf_q.addr[PageAddrWidth-1:0]' where PageAddrWidth is
153:     // r_page_addr_width and dynamically changing
154:     always_comb begin : proc_read_range_select
155:         r_page_offset = '0;
156:         for (int i = 0; i < PageAddrWidth; i++) begin
157:             r_page_offset[i] = r_page_addr_width > i ? r_tf_q.addr[i] : 1'b0;
158:         end
159:     end
160: 
161:     // calculate the number of bytes left in the page (number of bytes until
162:     // we reach the page boundary (bp)
163:     assign r_num_bytes_to_pb = r_page_size - r_page_offset;
164: 
165: 
166:     //--------------------------------------
167:     // write page boundary check
168:     //--------------------------------------
169:     // calculate the page with in bits
170:     always_comb begin : proc_write_addr_width
171:         // should the "virtual" page be reduced? e.g. the transfers split into
172:         // smaller chunks than the AXI page size?
173:         w_page_addr_width = OffsetWidth + (opt_tf_q.dst_reduce_len ? opt_tf_q.dst_max_llen : 'd8);
174:         // a page can be a maximum of 4kB (12 bit)
175:         w_page_addr_width = w_page_addr_width > PageAddrWidth ? PageAddrWidth : w_page_addr_width;
176:     end
177:     // calculate the page size in byte
178:     assign w_page_size = (1 << w_page_addr_width);
179: 
180:     // this is written very confusing due to system verilog not allowing variable
181:     // length ranges.
182:     // the goal is to get 'r_tf_q.addr[PageAddrWidth-1:0]' where PageAddrWidth is
183:     // r_page_addr_width and dynamically changing
184:     always_comb begin : proc_write_range_select
185:         w_page_offset = '0;
186:         for (int i = 0; i < PageAddrWidth; i++) begin
187:             w_page_offset[i] = w_page_addr_width > i ? w_tf_q.addr[i] : 1'b0;
188:         end
189:     end
190: 
191:     // calculate the number of bytes left in the page (number of bytes until
192:     // we reach the page boundary (bp)
193:     assign w_num_bytes_to_pb = w_page_size - w_page_offset;
194: 
195: 
196:     //--------------------------------------
197:     // page boundary check
198:     //--------------------------------------
199:     // how many transfers are remaining when concerning both r/w pages?
200:     // take the boundary that is closer
201:     assign c_num_bytes_to_pb = (r_num_bytes_to_pb > w_num_bytes_to_pb) ?
202:                                 w_num_bytes_to_pb : r_num_bytes_to_pb;
203: 
204: 
205:     //--------------------------------------
206:     // Synchronized R/W process
207:     //--------------------------------------
208:     // max num bytes readable in page
209:     assign r_num_bytes_possible = opt_tf_q.decouple_rw ?
210:                                   r_num_bytes_to_pb : c_num_bytes_to_pb;
211: 
212:     // max num bytes writable in page
213:     assign w_num_bytes_possible = opt_tf_q.decouple_rw ?
214:                                   w_num_bytes_to_pb : c_num_bytes_to_pb;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_legalizer.sv:224

SHA256 `f559a0b0b1d39a9c3ae02b064bf7a67f2b0872551f983d0cf22a4a94ce7c12e6`；mutable length is unissued request work; normal non-deburst two256-beat bursts。

```text
224:         r_tf_d   = r_tf_q;
225:         w_tf_d   = w_tf_q;
226:         opt_tf_d = opt_tf_q;
227: 
228:         // default: not done
229:         r_done = 1'b0;
230:         w_done = 1'b0;
231: 
232:         //--------------------------------------
233:         // Legalize read transaction
234:         //--------------------------------------
235:         // more bytes remaining than we can read
236:         if (r_tf_q.length > r_num_bytes_possible) begin
237:             r_num_bytes = r_num_bytes_possible;
238:             // calculate remainder
239:             r_tf_d.length = r_tf_q.length - r_num_bytes_possible;
240:             // next address
241:             r_tf_d.addr = r_tf_q.addr + r_num_bytes;
242: 
243:         // remaining bytes fit in one burst
244:         end else begin
245:             r_num_bytes = r_tf_q.length[PageAddrWidth:0];
246:             // finished
247:             r_tf_d.valid = 1'b0;
248:             r_done = 1'b1;
249:         end
250: 
251:         //--------------------------------------
252:         // Legalize write transaction
253:         //--------------------------------------
254:         // more bytes remaining than we can write
255:         if (w_tf_q.length > w_num_bytes_possible) begin
256:             w_num_bytes = w_num_bytes_possible;
257:             // calculate remainder
258:             w_tf_d.length = w_tf_q.length - w_num_bytes_possible;
259:             // next address
260:             w_tf_d.addr = w_tf_q.addr + w_num_bytes;
261: 
262:         // remaining bytes fit in one burst
263:         end else begin
264:             w_num_bytes = w_tf_q.length[PageAddrWidth:0];
265:             // finished
266:             w_tf_d.valid = 1'b0;
267:             w_done = 1'b1;
268:         end
269: 
270:         //--------------------------------------
271:         // Kill
272:         //--------------------------------------
273:         if (kill_i) begin
274:             // kill the current state
275:             r_tf_d =  '0;
276:             r_done = 1'b1;
277:             w_tf_d =  '0;
278:             w_done = 1'b1;
279:         end
280: 
281:         //--------------------------------------
282:         // Refill
283:         //--------------------------------------
284:         // new request is taken in if both r and w machines are ready.
285:         if (ready_o & valid_i) begin
286: 
287:             // load all three mutable objects (source, destination, option)
288:             // source or read
289:             r_tf_d = '{
290:                 length: req_i.length,
291:                 addr:   req_i.src_addr,
292:                 valid:  is_read
293:             };
294:             // destination or write
295:             w_tf_d = '{
296:                 length: req_i.length,
297:                 addr:   req_i.dst_addr,
298:                 valid:  is_write
299:             };
300:             // options
301:             opt_tf_d = '{
302:                 shift:          req_i.src_addr[OffsetWidth-1:0] - req_i.dst_addr[OffsetWidth-1:0],
303:                 decouple_rw:    req_i.opt.beo.decouple_rw,
304:                 decouple_aw:    req_i.opt.beo.decouple_aw,
305:                 src_max_llen:   req_i.opt.beo.src_max_llen,
306:                 dst_max_llen:   req_i.opt.beo.dst_max_llen,
307:                 src_reduce_len: req_i.opt.beo.src_reduce_len,
308:                 dst_reduce_len: req_i.opt.beo.dst_reduce_len,
309:                 axi_id:         req_i.opt.axi_id,
310:                 device_id:      req_i.opt.device_id,
311:                 pv:             req_i.opt.pv,
312:                 process_id:     req_i.opt.process_id,
313:                 src_axi_opt:    req_i.opt.src,
314:                 dst_axi_opt:    req_i.opt.dst,
315:                 super_last:     req_i.opt.last
316:             };
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_legalizer.sv:324

SHA256 `f559a0b0b1d39a9c3ae02b064bf7a67f2b0872551f983d0cf22a4a94ce7c12e6`；mutable length is unissued request work; normal non-deburst two256-beat bursts。

```text
324:     if (Protocol == idma_pkg::AXI) begin : gen_axi_ar_aw_req
325:         // assign the signals for the read meta channel
326:         assign r_req_o.ar_req = '{
327:             id:             opt_tf_q.axi_id,
328:             stream_id:      opt_tf_q.device_id,
329:             ss_id_valid:    opt_tf_q.pv,
330:             substream_id:   opt_tf_q.process_id,
331:             mmu_flow:       '0,
332:             mmu_atst:       1'b0,
333:             mmu_valid:      1'b0,
334:             mmu_secsid:     1'b0,
335:             addr:           { r_tf_q.addr[AddrWidth-1:OffsetWidth], {{OffsetWidth}{1'b0}} },
336:             len:            ((r_num_bytes + r_addr_offset - 'd1) >> OffsetWidth),
337:             size:           axi_pkg::size_t'(OffsetWidth),
338:             burst:          opt_tf_q.src_axi_opt.burst,
339:             lock:           opt_tf_q.src_axi_opt.lock,
340:             cache:          opt_tf_q.src_axi_opt.cache,
341:             prot:           opt_tf_q.src_axi_opt.prot,
342:             qos:            opt_tf_q.src_axi_opt.qos,
343:             region:         opt_tf_q.src_axi_opt.region,
344:             user:           '0
345:         };
346: 
347:         // assign the signals for the write meta channel
348:         assign w_req_o.aw_req = '{
349:             id:             opt_tf_q.axi_id,
350:             stream_id:      opt_tf_q.device_id,
351:             ss_id_valid:    opt_tf_q.pv,
352:             substream_id:   opt_tf_q.process_id,
353:             mmu_flow:       '0,
354:             mmu_atst:       1'b0,
355:             mmu_valid:      1'b0,
356:             mmu_secsid:     1'b0,
357:             addr:           { w_tf_q.addr[AddrWidth-1:OffsetWidth], {{OffsetWidth}{1'b0}} },
358:             len:            ((w_num_bytes + w_addr_offset - 'd1) >> OffsetWidth),
359:             size:           axi_pkg::size_t'(OffsetWidth),
360:             burst:          opt_tf_q.dst_axi_opt.burst,
361:             lock:           opt_tf_q.dst_axi_opt.lock,
362:             cache:          opt_tf_q.dst_axi_opt.cache,
363:             prot:           opt_tf_q.dst_axi_opt.prot,
364:             qos:            opt_tf_q.dst_axi_opt.qos,
365:             region:         opt_tf_q.dst_axi_opt.region,
366:             user:           '0,
367:             atop:           '0
368:         };
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_legalizer.sv:432

SHA256 `f559a0b0b1d39a9c3ae02b064bf7a67f2b0872551f983d0cf22a4a94ce7c12e6`；mutable length is unissued request work; normal non-deburst two256-beat bursts。

```text
432:     // last burst in generic 1D transfer?
433:     assign w_req_o.last = w_done;
434: 
435:     // last burst indicated by midend
436:     assign w_req_o.super_last = opt_tf_q.super_last;
437: 
438:     // assign aw decouple flag
439:     assign w_req_o.decouple_aw = opt_tf_q.decouple_aw;
440: 
441:     // busy output
442:     assign r_busy_o = r_tf_q.valid;
443:     assign w_busy_o = w_tf_q.valid;
444: 
445: 
446:     //--------------------------------------
447:     // Flow Control
448:     //--------------------------------------
449:     // only advance to next state if:
450:     // * rw_coupled: both machines advance
451:     // * rw_decoupled: either machine advances
452:     always_comb begin : proc_legalizer_flow_control
453:         if (opt_tf_q.decouple_rw) begin
454:             r_tf_ena  = (r_ready_i & !flush_i) | kill_i;
455:             w_tf_ena  = (w_ready_i & !flush_i) | kill_i;
456: 
457:             r_valid_o = r_tf_q.valid & r_ready_i & !flush_i;
458:             w_valid_o = w_tf_q.valid & w_ready_i & !flush_i;
459:         end else begin
460:             r_tf_ena  = (r_ready_i & w_ready_i & !flush_i) | kill_i;
461:             w_tf_ena  = (r_ready_i & w_ready_i & !flush_i) | kill_i;
462: 
463:             r_valid_o = r_tf_q.valid & w_ready_i & r_ready_i & !flush_i;
464:             w_valid_o = w_tf_q.valid & r_ready_i & w_ready_i & !flush_i;
465:         end
466:     end
467: 
468:     // load next idma request: if both machines are done!
469:     assign ready_o = r_done & w_done & r_ready_i & w_ready_i & !flush_i;
470: 
471: 
472:     //--------------------------------------
473:     // State
474:     //--------------------------------------
475:     `FF(opt_tf_q, opt_tf_d, '0, clk_i, rst_ni)
476:     `FFL(r_tf_q, r_tf_d, r_tf_ena, '0, clk_i, rst_ni)
477:     `FFL(w_tf_q, w_tf_d, w_tf_ena, '0, clk_i, rst_ni)
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:238

SHA256 `4447bec83a3335ee106a627177a1113730c27c9587d30780892eaaa972f7d80a`；DEFAULT read data is real source W data, per-burst counters and B separate。

```text
238:     //--------------------------------------
239:     // controls the next state of the read flag
240:     always_comb begin : proc_first_read
241:         // sticky is first bit for read
242:         if (!axi_rsp_i.r.last & axi_rsp_i.r_valid & axi_req_o.r_ready) begin
243:             // new transfer has started
244:             first_r_d = 1'b0;
245:         end else if (axi_rsp_i.r.last & axi_rsp_i.r_valid & axi_req_o.r_ready) begin
246:             // finish read burst
247:             first_r_d = 1'b1;
248:         end else begin
249:             // no change
250:             first_r_d = first_r_q;
251:         end
252:     end
253: 
254:     // the buffer can be pushed to if all the masked FIFO buffers (mask_in) are ready.
255:     assign in_ready = &(buffer_in_ready | ~mask_in);
256:     // the read can accept data if the buffer is ready and the response channel is ready
257:     assign axi_req_o.r_ready = in_ready & r_dp_ready_i;
258: 
259:     // Independent read
260:     logic indep_read;
261:     assign indep_read = (ttype_i != idma_pkg::DEFAULT);
262: 
263:     // once valid data is applied, it can be pushed in all the selected (mask_in) buffers
264:     // be sure the response channel is ready
265:     assign in_valid        = axi_rsp_i.r_valid & in_ready & r_dp_ready_i;
266:     assign buffer_in_valid = (in_valid && !indep_read) ? mask_in : '0;
267: 
268:     // r_dp_ready_o is triggered by the last element arriving from the read
269:     assign r_dp_ready_o = r_dp_valid_i & r_dp_ready_i &
270:                           axi_rsp_i.r.last & axi_rsp_i.r_valid & in_ready;
271: 
272:     // Send to rdata register
273:     assign rdata_valid_o   = axi_rsp_i.r.last & axi_rsp_i.r_valid & axi_req_o.r_ready;
274:     assign rdata_o         = axi_rsp_i.r.data;
275: 
276:     // connect r_dp response payload
277:     assign r_dp_rsp_o.resp  = axi_rsp_i.r.resp;
278:     assign r_dp_rsp_o.last  = axi_rsp_i.r.last;
279:     assign r_dp_rsp_o.first = first_r_q;
280: 
281:     // r_dp_valid_o is triggered once the last element is here or an error occurs
282:     assign r_dp_valid_o = axi_rsp_i.r_valid & in_ready & (axi_rsp_i.r.last | (|axi_rsp_i.r.resp));
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:288

SHA256 `4447bec83a3335ee106a627177a1113730c27c9587d30780892eaaa972f7d80a`；DEFAULT read data is real source W data, per-burst counters and B separate。

```text
288:     // connect the aw requests to the AXI bus
289:     assign axi_req_o.aw       = aw_req_i;
290:     assign axi_req_o.aw_valid = aw_valid_i;
291:     assign aw_ready_o         = axi_rsp_i.aw_ready;
292: 
293: 
294:     //--------------------------------------
295:     // Out mask generation -> (wstrb mask)
296:     //--------------------------------------
297:     // only pop the data actually needed for write from the buffer,
298:     // determine valid data to pop by calculation the wstrb
299:     always_comb begin : proc_out_mask_generator
300:         // default case: all ones
301:         mask_out = '1;
302:         // is first word: some bytes at the beginning may be invalid
303:         mask_out = first_w ? (mask_out & w_first_mask) : mask_out;
304:         // is last word in write burst: some bytes at the end may be invalid
305:         if (w_dp_req_i.tailer != '0 & last_w) begin
306:             mask_out = mask_out & w_last_mask;
307:         end
308:     end
309: 
310: 
311:     //--------------------------------------
312:     // Write control
313:     //--------------------------------------
314:     // write is decoupled from read, due to misalignment in the read/write
315:     // addresses, page crossing can be encountered at any time.
316:     // To handle this efficiently, a 2-to-1 or 1-to-2 mapping of r/w beats
317:     // is required. The write unit needs to keep track of progress through
318:     // a counter and cannot use `r last` for that.
319: 
320:     // Once buffer contains a full line -> all FIFOs are non-empty push it out.
321: 
322:     // use wdata instead of the buffered data
323:     logic use_wdata;
324:     assign use_wdata = (ttype_i == idma_pkg::WRITE) | (ttype_i == idma_pkg::PARALLEL);
325: 
326:     logic write_d, write_q;
327:     always_comb begin : independent_write
328:         
329:         write_d = write_q;
330: 
331:         // Set WVALID if the write is independent
332:         if (aw_valid_i && aw_ready_o && use_wdata) begin
333:             write_d = 1'b1;
334:         end
335: 
336:         // Clear WVALID for independent writes
337:         if (write_q && axi_rsp_i.w_ready) begin
338:             write_d = 1'b0;
339:         end
340:     end
341: 
342:     // all elements needed (defined by the mask) are in the buffer and the buffer is non-empty
343:     assign ready_to_write = ((buffer_out_valid & mask_out) == mask_out) & (buffer_out_valid != '0);
344: 
345:     // data needed by the first mask is available in the buffer -> r_first happened for sure
346:     // this signal can be high during a transfer as well, it needs to be masked
347:     assign first_possible = (((buffer_out_valid & w_first_mask) == w_first_mask) &
348:                              (buffer_out_valid != '0)) | write_q;
349: 
350:     // the buffer is completely empty and idle
351:     assign buffer_clean = &(~buffer_out_valid);
352: 
353:     // write happening: both the bus (w_ready) and the buffer (ready_to_write) is high
354:     assign write_happening = ready_to_write & axi_rsp_i.w_ready;
355: 
356:     // the main buffer is conditionally to the write mask popped
357:     assign buffer_out_ready = write_happening ? mask_out : '0;
358: 
359:     // signal the bus that we are ready
360:     assign axi_req_o.w_valid = (ready_to_write | write_q);
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:390

SHA256 `4447bec83a3335ee106a627177a1113730c27c9587d30780892eaaa972f7d80a`；DEFAULT read data is real source W data, per-burst counters and B separate。

```text
390:     end
391: 
392:     // the w last signal should only be applied to the bus if an actual transfer happens
393:     assign axi_req_o.w.last = (last_w & ready_to_write) | write_q;
394: 
395:     // we are ready for the next transfer internally, once the w last signal is applied
396:     assign w_dp_ready_o = last_w & (write_happening | write_q);
397: 
398:     // the write process: keeps track of remaining beats in burst
399:     always_comb begin : proc_write_control
400:         // defaults:
401:         // beat counter
402:         w_num_beats_d   = w_num_beats_q;
403:         w_cnt_valid_d   = w_cnt_valid_q;
404:         // mask control
405:         first_w      = 1'b0;
406:         last_w       = 1'b0;
407: 
408:         // differentiate between the burst and non-burst case. If a transfer
409:         // consists just of one beat the counters are disabled
410:         if (w_dp_req_i.is_single) begin
411:             // in the single case the transfer is both first and last.
412:             first_w = 1'b1;
413:             last_w  = 1'b1;
414: 
415:         // in the burst case the counters are needed to keep track of the progress of sending
416:         // beats. The w_last_o depends on the state of the counter
417:         end else begin
418:             // first transfer happens as soon as a) the buffer is ready for a first transfer and b)
419:             // the counter is currently invalid
420:             first_w = first_possible & ~w_cnt_valid_q;
421: 
422:             // last happens as soon as a) the counter is valid and b) the counter is now down to 1
423:             last_w  = w_cnt_valid_q & (w_num_beats_q == 8'h01);
424: 
425:             // load the counter with data in a first cycle, only modifying state if bus is ready
426:             if (first_w && write_happening) begin
427:                 w_num_beats_d = w_dp_req_i.num_beats;
428:                 w_cnt_valid_d = 1'b1;
429:             end
430: 
431:             // if we hit the last element, invalidate the counter, only modifying state
432:             // if bus is ready
433:             if (last_w && write_happening) begin
434:                 w_cnt_valid_d = 1'b0;
435:             end
436: 
437:             // count down the beats if the counter is valid and valid data is written to the bus
438:             if (w_cnt_valid_q && write_happening) w_num_beats_d = w_num_beats_q - 8'h01;
439:         end
440:     end
441: 
442: 
443:     //--------------------------------------
444:     // Write response
445:     //--------------------------------------
446:     // connect w_dp response payload
447:     assign w_dp_rsp_o.resp = axi_rsp_i.b.resp;
448:     assign w_dp_rsp_o.user = axi_rsp_i.b.user;
449: 
450:     // w_dp_valid_o is triggered once the write answer is here
451:     assign w_dp_valid_o = axi_rsp_i.b_valid;
452: 
453:     // create back pressure on the b channel if the higher parts of the DMA cannot accept more
454:     // write responses
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_backend.sv:518

SHA256 `1043573590bf91d831839edca60cb2e6497aaf047a5ad1e9b653d022cfbda5f2`；NO_ERROR_HANDLING and B-side last flag lifetime。

```text
518:         );
519:     end else if (ErrorCap == idma_pkg::NO_ERROR_HANDLING) begin : gen_no_error_handler
520:         // bypass the signals, assign their neutral values
521:         assign idma_rsp.error     = 1'b0;
522:         assign idma_rsp.pld       = 1'b0;
523:         assign idma_rsp.last      = w_super_last;
524:         assign rsp_valid          = (ttype_i == idma_pkg::READ) ? 
525:                                     (r_dp_rsp_valid) : 
526:                                     (w_dp_rsp_valid & w_last_burst);
527:         assign eh_req_ready_o     = 1'b0;
528:         assign legalizer_flush    = 1'b0;
529:         assign legalizer_kill     = 1'b0;
530:         assign dp_poison          = 1'b0;
531:         assign r_dp_rsp_ready     = rsp_ready;
532:         assign w_dp_rsp_ready     = rsp_ready;
533:         assign busy_o.eh_fsm_busy = 1'b0;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_backend.sv:632

SHA256 `1043573590bf91d831839edca60cb2e6497aaf047a5ad1e9b653d022cfbda5f2`；NO_ERROR_HANDLING and B-side last flag lifetime。

```text
632:     if (1'b1) begin : gen_last_flag_fifo
633:         idma_stream_fifo #(
634:             .Depth        ( MetaFifoDepth ),
635:             .type_t       ( logic [1:0]   ),
636:             .PrintInfo    ( PrintFifoInfo )
637:         ) i_w_last (
638:             .clk_i,
639:             .rst_ni,
640:             .testmode_i,
641:             .flush_i   ( 1'b0                            ),
642:             .usage_o   ( /* NOT CONNECTED */             ),
643:             .data_i    ( {w_req.super_last, w_req.last}  ),
644:             .valid_i   ( w_valid & w_ready               ),
645:             .ready_o   ( w_last_ready                    ),
646:             .data_o    ( {w_super_last, w_last_burst}    ),
647:             .valid_o   ( /* NOT CONNECTED */             ),
648:             .ready_i   ( w_dp_rsp_valid & w_dp_rsp_ready )
649:         );
650:     end else if (Protocol == idma_pkg::AXI_LITE) begin : gen_last_flag_bypass
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:163

SHA256 `07bd3c1288ee3e53a0924c0cc44967ced960e90493824b7709c0d5fefae85f31`；actual frontend job acceptance/retirement and unchanged backend mode。

```text
163:       .ready_i          ( be_ready_d        ),
164:       .backend_idle_i   ( ~|idma_busy       ),
165:       .trans_complete_i ( be_trans_complete ),
166: 
167:       .ttype_o          ( ttype             ),
168:       .wdata_o          ( wdata             ),
169:       .rdata_i          ( rdata             ),
170:       .rdata_valid_i    ( rdata_valid       )
171:     );
172: 
173:     idma_fullbuffer_certificate #(
174:         .CapacityBytes (BufferDepth * (AxiDataWidth / 8))
175:     ) i_fullbuffer_certificate (
176:         .clk_i, .rst_ni,
177:         .job_fire_i (be_valid_d && be_ready_d),
178:         .job_done_i (idma_rsp_valid && idma_rsp_ready),
179:         .normal_copy_i (ttype == idma_pkg::DEFAULT),
180:         .length_i (64'(burst_req_d.length)),
181:         .src_i (64'(burst_req_d.src_addr)),
182:         .dst_i (64'(burst_req_d.dst_addr)),
183:         .permit_o (fullbuffer_job_o)
184:     );
185: 
186:     stream_fifo #(
187:         .FALL_THROUGH ( 1'b0         ),
188:         .DATA_WIDTH   ( AxiDataWidth ),
189:         .DEPTH        ( JobFifoDepth ),
190:         .T            ( idma_req_t   )
191:       ) i_stream_fifo_jobs_oned (
192:         .clk_i,
193:         .rst_ni,
194:         .testmode_i,
195:         .flush_i    ( 1'b0                ),
196:         .usage_o    ( /* NOT CONNECTED */ ),
197:         .data_i     ( burst_req_d         ),
198:         .valid_i    ( be_valid_d          ),
199:         .ready_o    ( be_ready_d          ),
200:         .data_o     ( burst_req           ),
201:         .valid_o    ( be_valid            ),
202:         .ready_i    ( be_ready            )
203:     );
204: 
205: 
206:     assign be_trans_complete = idma_rsp_valid;
207:     assign idma_rsp_ready    = 1'b1;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:306

SHA256 `07bd3c1288ee3e53a0924c0cc44967ced960e90493824b7709c0d5fefae85f31`；actual frontend job acceptance/retirement and unchanged backend mode。

```text
306:   idma_backend #(
307:     .DataWidth           ( AxiDataWidth                ),
308:     .AddrWidth           ( AxiAddrWidth                ),
309:     .UserWidth           ( AxiUserWidth                ),
310:     .AxiIdWidth          ( AxiIdWidth                  ),
311:     .NumAxInFlight       ( NumAxInFlight               ),
312:     .BufferDepth         ( BufferDepth                 ),
313:     .TFLenWidth          ( TFLenWidth                  ),
314:     .RAWCouplingAvail    ( RAWCouplingAvail            ),
315:     .MaskInvalidData     ( 1'b0                        ),
316:     .HardwareLegalizer   ( 1'b1                        ),
317:     .RejectZeroTransfers ( 1'b1                        ),
318:     .MemSysDepth         ( MemSysDepth                 ),
319:     .ErrorCap            ( idma_pkg::NO_ERROR_HANDLING ),
320:     .idma_req_t          ( idma_req_t                  ),
321:     .idma_rsp_t          ( idma_rsp_t                  ),
322:     .idma_eh_req_t       ( idma_pkg::idma_eh_req_t     ),
323:     .idma_busy_t         ( idma_pkg::idma_busy_t       ),
324:     .protocol_req_t      ( axi_mst_req_t               ),
325:     .protocol_rsp_t      ( axi_mst_rsp_t               ),
326:     .aw_chan_t           ( axi_mst_aw_chan_t           ),
327:     .ar_chan_t           ( axi_mst_ar_chan_t           )
328:   ) i_idma_backend (
329:     .clk_i,
330:     .rst_ni,
331:     .testmode_i,
332: 
333:     .idma_req_i    ( burst_req         ),
334:     .req_valid_i   ( be_valid          ),
335:     .req_ready_o   ( be_ready          ),
336: 
337:     .idma_rsp_o    ( idma_rsp          ),
338:     .rsp_valid_o   ( idma_rsp_valid    ),
339:     .rsp_ready_i   ( idma_rsp_ready    ),
340: 
341:     .idma_eh_req_i ( '0                ), // No error handling
342:     .eh_req_valid_i( 1'b1              ),
343:     .eh_req_ready_o( /*NOT CONNECTED*/ ),
344: 
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:165

SHA256 `d3ab23374604d16d4cccbfafb012c65fbd5e55c738d94672b7b515a9a1fa0aae`；slow req owner is stable; other-bank N hit and actual accepted response。

```text
165:     pscid_t n_iotlb_pscid;
166:     logic cache_ddtc_hit, cache_iotlb_hit;
167:     logic cache_iotlb_en1, cache_iotlb_en2;
168:     trans_req_data_t    arb_req_data;
169:     logic               arb_req_valid;
170:     logic               arb_req_ready;
171:     trans_req_data_t    req_data;
172:     logic               req_valid;
173:     logic               req_ready;
174:     assign req_ready = (debug_resp_valid_o & debug_resp_ready_i) |
175:                        (slow_resp_valid & slow_resp_ready) |
176:                        (ats_trans_resp_valid_o & ats_trans_resp_ready_i) ;
177: 
178:     // Priority is given to normal transactions
179:     // We can keep the arbitrer in any case, as data_i and valid_i are '0
180:     // when debug/ATS are unsupported
181:     stream_arbiter #(
182:         .DATA_T   (trans_req_data_t),
183:         .N_INP    (3),
184:         .ARBITER  ("prio")
185:     ) i_req_arbiter (
186:         .clk_i          (clk_i),
187:         .rst_ni         (rst_ni),
188: 
189:         .inp_data_i     ({ats_trans_req_data_i,     debug_req_data_i,     trans_req_data_i }),
190:         .inp_valid_i    ({ats_trans_req_valid_i && normal_gate,
191:                           debug_req_valid_i && normal_gate,
192:                           trans_req_valid_i && normal_gate}),
193:         .inp_ready_o    ({ats_trans_req_ready_o,    debug_req_ready_o,    normal_trans_ready}),
194: 
195:         .oup_data_o     (arb_req_data),
196:         .oup_valid_o    (arb_req_valid),
197:         .oup_ready_i    (arb_req_ready)
198:     );
199: 
200:     spill_register #(
201:         .T          (trans_req_data_t),
202:         .Bypass     (1'b0)
203:     ) i_req_spill_register (
204:         .clk_i      (clk_i),
205:         .rst_ni     (rst_ni),
206: 
207:         .data_i     (arb_req_data),
208:         .valid_i    (arb_req_valid),
209:         .ready_o    (arb_req_ready),
210: 
211:         .data_o     (req_data),
212:         .valid_o    (req_valid),
213:         .ready_i    (req_ready)
214:     );
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:280

SHA256 `d3ab23374604d16d4cccbfafb012c65fbd5e55c738d94672b7b515a9a1fa0aae`；slow req owner is stable; other-bank N hit and actual accepted response。

```text
280:                               (ptw_iotlb_update);
281: 
282:     // Two lifecycle tags (one outstanding request per bank), one response port.
283:     // A rejected probe never handshakes: the original arbiter later accepts it.
284:     assign normal_gate = !NonblockEnable ||
285:                          (state_q == IDLE && !req_valid && !n_intercept);
286:     assign trans_req_ready_o = n_intercept ? n_accept : (normal_trans_ready && normal_gate);
287:     assign nonblock_fire_o = n_accept;
288:     assign nonblock_busy_o = n_busy;
289:     // All lifecycle storage must be empty; incoming VALID also excludes a
290:     // same-edge configuration. Never reset or discard a pending response.
291:     assign policy_quiet_o = state_q == IDLE && !req_valid && !arb_req_valid &&
292:         !trans_req_valid_i && !ats_trans_req_valid_i && !debug_req_valid_i &&
293:         !ats_ddtc_valid_i && !n_busy && !n_intercept && !n_accept &&
294:         !fast_resp_valid && !slow_resp_valid && !ats_trans_resp_valid_o &&
295:         !debug_resp_valid_o && !fault_valid_o && !e3_ptw_active &&
296:         !ddtw_axi_req_o.ar_valid && !pdtw_axi_req_o.ar_valid &&
297:         !ptw_axi_req_o.ar_valid && !msiptw_axi_req_o.ar_valid &&
298:         !ddtw_axi_resp_i.r_valid && !pdtw_axi_resp_i.r_valid &&
299:         !ptw_axi_resp_i.r_valid && !msiptw_axi_resp_i.r_valid &&
300:         !iotlb_update.update && !ddtc_update.update &&
301:         !iotinval_i.inval_vma && !iotinval_i.inval_gvma &&
302:         !iodirinval_i.inval_ddtc && !iodirinval_i.inval_pdtc;
303:     assign n_window = state_q == PT && e3_ptw_active && en_1S_q &&
304:                       !en_2S_q && !ddt_imp_trans_q && !pdt_imp_trans_q &&
305:                       !req_data.is_debug && trans_req_data_i.bank != req_data.bank &&
306:                       !ats_ddtc_valid_i && !debug_req_valid_i && !ats_trans_req_valid_i;
307:     assign nonblock_diag_o = {n_diag[15:1],
308:         (state_q == PT && e3_ptw_active && en_1S_q && !en_2S_q)};
309:     if (NonblockEnable) begin : gen_nonblock_hit
310:         rv_iommu_nonblock_hit #(
311:             .PAddrWidth(PLEN),
312:             .HasMsiContext(RVIOMMUCfg.MSITrans != rv_iommu_cfg::MSI_DISABLED),
313:             .dc_t(dc_t), .req_t(trans_req_data_t),
314:             .resp_t(trans_resp_data_t), .ddtp_t(iommu_reg2hw_ddtp_reg_t)
315:         ) i_hit (
316:             .clk_i, .rst_ni, .enable_i(nonblock_enable_i),
317:             .window_i(n_window),
318:             .candidate_i(trans_req_data_i), .candidate_valid_i(trans_req_valid_i),
319:             .normal_fire_i(trans_req_valid_i && normal_trans_ready && normal_gate),
320:             .invalidate_i(iotinval_i.inval_vma || iotinval_i.inval_gvma ||
321:                           iodirinval_i.inval_ddtc || iodirinval_i.inval_pdtc),
322:             .ddtp_i(ddtp_i), .intercept_o(n_intercept), .accept_o(n_accept),
323:             .busy_o(n_busy), .ddtc_lookup_o(n_ddtc_lu), .ddtc_touch_o(n_ddtc_touch),
324:             .ddtc_did_o(n_ddtc_did), .ddtc_hit_i(cache_ddtc_hit), .ddtc_dc_i(ddtc_dc),
325:             .iotlb_lookup_o(n_iotlb_lu), .iotlb_touch_o(n_iotlb_touch),
326:             .iotlb_vpn_o(n_iotlb_vpn), .iotlb_pscid_o(n_iotlb_pscid),
327:             .iotlb_hit_i(cache_iotlb_hit), .iotlb_content_i(iotlb_lu_content),
328:             .response_o(fast_resp_data), .response_valid_o(fast_resp_valid),
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:454

SHA256 `d3ab23374604d16d4cccbfafb012c65fbd5e55c738d94672b7b515a9a1fa0aae`；slow req owner is stable; other-bank N hit and actual accepted response。

```text
454:     assign debug_resp_data_o.ppn            = {{56-PLEN{1'b0}}, spaddr_q[PLEN-1:12]};
455: 
456:     //----------------------
457:     // Translation response
458:     //----------------------
459:     logic ignore_q, ignore_n;
460:     logic is_mrif_q, is_mrif_n;
461:     rv_iommu::mrifc_content_t mrif_q, mrif_n;
462:     assign slow_resp_data.bank = req_data.bank;
463:     assign ats_trans_resp_data_o.bank = req_data.bank;
464:     assign slow_resp_data.error      = error_q;
465:     assign slow_resp_data.ignore     = ignore_q;
466:     assign slow_resp_data.spaddr     = spaddr_q;
467:     assign slow_resp_data.is_mrif    = is_mrif_q;
468:     assign slow_resp_data.mrif_data  = mrif_q;
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:46

SHA256 `657f277a023c29c0ae4821fdf0b49f9535cfd2d36d66c4a8c880e0074f902010`；native N accepts real read/write hits with cache/context/permission checks。

```text
46: 
47:   assign start = state_q == IDLE && enable_i && window_i &&
48:                  candidate_valid_i && !invalidate_i;
49:   assign busy_o = state_q != IDLE && state_q != REJECT;
50:   assign intercept_o = start || busy_o;
51:   assign ddtc_lookup_o = state_q == DC || state_q == LEAF;
52:   assign ddtc_did_o = req_q.did;
53:   assign iotlb_lookup_o = state_q == LEAF;
54:   assign iotlb_vpn_o = req_q.iova[GPLEN39-1:12];
55:   assign iotlb_pscid_o = dc_q.ta.pscid;
56:   assign is_write = req_q.ttype == UNTRANSLATED_W;
57:   assign is_read = req_q.ttype == UNTRANSLATED_R;
58:   assign canonical = req_q.iova[63:39] == {25{req_q.iova[38]}};
59: 
60:   // The actual cached DC belongs to this request. No active-walker context
61:   // or saved inferred PSCID is used. All excluded cases fall through.
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:80

SHA256 `657f277a023c29c0ae4821fdf0b49f9535cfd2d36d66c4a8c880e0074f902010`；native N accepts real read/write hits with cache/context/permission checks。

```text
80:   end else begin : gen_no_msi
81:     assign msi_safe = 1'b1;
82:     assign msi_leaf_safe = 1'b1;
83:   end
84:   assign dc_ok = msi_safe && ddtc_hit_i && ddtc_dc_i.tc.v &&
85:       !ddtc_dc_i.tc.pdtv && !ddtc_dc_i.tc.sxl &&
86:       !ddtc_dc_i.tc.t2gpa && !ddtc_dc_i.tc.en_ats &&
87:       !ddtc_dc_i.tc.en_pri && !req_q.pid_valid && !req_q.priv &&
88:       !req_q.is_debug && (is_write || is_read) && canonical &&
89:       ddtc_dc_i.fsc.mode == ModeSv39 &&
90:       ddtc_dc_i.iohgatp.mode == ModeBare &&
91:       ((ddtp_i.iommu_mode.q == 2 && req_q.did[23:6] == 0) ||
92:        (ddtp_i.iommu_mode.q == 3 && req_q.did[23:15] == 0) ||
93:        ddtp_i.iommu_mode.q == 4);
94:   assign leaf_ok = msi_leaf_safe && iotlb_hit_i && iotlb_content_i.content_1S.v &&
95:       iotlb_content_i.content_1S.u &&
96:       !iotlb_content_i.content_1S.is_1G &&
97:       !iotlb_content_i.content_1S.is_2M &&
98:       (is_write ? iotlb_content_i.content_1S.w :
99:                   iotlb_content_i.content_1S.r);
100:   assign accept_o = state_q == LEAF && enable_i &&
101:       candidate_valid_i && candidate_i == req_q &&
102:       !invalidate_i && ddtp_i == ddtp_q && dc_ok &&
103:       ddtc_dc_i == dc_q && leaf_ok;
104:   // Probes do not change replacement state; only an actually accepted hit
105:   // touches both real caches, exactly once.
106:   assign ddtc_touch_o = accept_o;
107:   assign iotlb_touch_o = accept_o;
108:   assign response_o = resp_q;
109:   assign response_valid_o = state_q == OUT;
110: 
111:   assign probe_stable = enable_i && candidate_valid_i && candidate_i == req_q &&
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:384

SHA256 `d24c688b32022865fec625d3d53e4afb0a0c55dac0d6d1417435a13ad1979212`；future leaf/permission and remaining walk cannot be free inputs。

```text
384:             // Perform memory access with address hold in ptw_pptr_q
385:             MEM_ACCESS: begin
386:                 mem_req_o.ar_valid = 1'b1;
387:                 if (mem_resp_i.ar_ready) begin
388:                     state_n = PROC_PTE;
389:                 end
390:             end
391: 
392:             // Process PTEs
393:             PROC_PTE: begin
394: 
395:                 if (mem_resp_i.r_valid) begin
396: 
397:                     mem_req_o.r_ready   = 1'b1;
398:                         
399:                     if (pte.g && ptw_stage_q == STAGE_1)
400:                         global_mapping_n = 1'b1;
401: 
402:                     // From Spec:
403:                     // If pte.v = 0, or if pte.r = 0 and pte.w = 1, or if any bits or encodings that are reserved for
404:                     // future standard use are set within pte, stop and raise a page-fault exception corresponding
405:                     // to the original access type.
406:                     if (!pte.v || (!pte.r && pte.w) ||
407:                         (pte.g && (ptw_stage_q != STAGE_1))) begin
408:                         pf_excep_n    = 1'b1;
409:                         state_n         = ERROR;
410:                     end
411: 
412:                     // Valid PTE
413:                     else begin : valid_pte
414:                         state_n = IDLE;
415: 
416:                         // Leaf PTE
417:                         if (pte.r || pte.x) begin : leaf_pte
418:                             unique case (ptw_stage_q)
419:                                 
420:                                 // Leaf first-stage PTE
421:                                 STAGE_1: begin
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:480

SHA256 `d24c688b32022865fec625d3d53e4afb0a0c55dac0d6d1417435a13ad1979212`；future leaf/permission and remaining walk cannot be free inputs。

```text
480:                             if ((ptw_stage_q == STAGE_2_FINAL) || (!en_2S_i)) begin
481:                                     if (!implicit_trans_q)
482:                                         update_o.update = 1'b1;
483:                                     else
484:                                         imp_done_o = 1'b1;
485:                             end
486: 
487:                             // From Spec:
488:                             // (1): If i > 0 and pte.vpn[i − 1 : 0] != 0, this is a misaligned superpage.
489:                             //      Stop and raise a page-fault exception corresponding to the original access type.
490:                             // (2): When a virtual page is accessed and the A bit is clear, or is written and the D bit is clear,
491:                             //      a page-fault exception is raised.
492:                             // (3): For G-stage address translation, all memory accesses are considered to be user-level accesses,
493:                             //      as though executed in U-mode.
494:                             if ((main_lvl_q == LVL1 && |pte.ppn[17:0] != 1'b0   ) ||       // 1G
495:                                 (main_lvl_q == LVL2 && |pte.ppn[8:0] != 1'b0    ) ||       // 2M
496:                                 (!pte.a || !pte.r || (is_store_i && !pte.d)     ) ||
497:                                 (ptw_stage_q != STAGE_1 && !pte.u              )) begin
498:                                 
499:                                 pf_excep_n          = 1'b1;
500:                                 state_n             = ERROR;
501:                                 ptw_stage_n         = ptw_stage_q;
502:                                 update_o.update     = 1'b0;
503:                                 imp_done_o          = 1'b0;
504:                             end
505:                         end
506:                         
507:                         // Non-leaf PTE
508:                         else begin : non_leaf_pte
509: 
510:                             state_n = MEM_ACCESS;
511:                             
512:                             if (main_lvl_q == LVL1) begin
513: 
514:                                 main_lvl_n = LVL2;
515:                                 unique case (ptw_stage_q)
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/tb/ariane_soc_pkg.sv:109

SHA256 `88227b92e8796df4a7bedef59208b963fe09a64cb3a5e1255b5f55521f978d55`；declared global8/32, IoTLB16, widths; no new bit/clock binding。

```text
109: 
110:   localparam rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfg = rv_iommu_cfg::rv_iommu_cfg_t'{
111:     NumOutstandingTrans : 32'd8,
112:     WFifoDepth          : 32'd32,
113:     MrifFifoDepth       : 32'd8,
114:     FaultFifoDepth      : 32'd8,
115:     NumIotlbEntries     : 32'd16,
116:     NumDdtcEntries      : 32'd8,
117:     NumPdtcEntries      : 32'd8,
118:     NumMrifcEntries     : 32'd8,
119:     InclPC              : 1'b1,
120:     InclAxiBC           : 1'b1,
121:     InclDbg             : 1'b1,
122:     InclATS             : 1'b0,
123:     MSITrans            : rv_iommu_cfg::MSI_BT_MRIF,
124:     IGS                 : rv_iommu_cfg::BOTH,
125:     NumIntVec           : 32'd16,
126:     NumHpmCounters      : 32'd31,
127:     PAddrWidth          : 32'd56,
128:     AxiAddrWidth        : 32'd64,
129:     AxiDataWidth        : 32'd64,
130:     AxiIdWidth          : IdWidth,
131:     AxiProgIdWidth      : IdWidthSlave,
132:     AxiUserWidth        : ariane_pkg::AXI_USER_WIDTH,
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1227

SHA256 `9eaa9edbadef37c5323e15f3c2b201723af1cc58806517ab5f36f92c0d591666`；original4DMA source mux, fullbuffer5120/1 and actual copied config。

```text
1227: 
1228:         axi_mux #(
1229:             .SlvAxiIDWidth ( DmaAxiIdWidth        ),
1230:             .slv_aw_chan_t ( axi_dma_tr_aw_chan_t ),
1231:             .mst_aw_chan_t ( axi_tr_aw_chan_t     ),
1232:             .w_chan_t      ( axi_dma_tr_w_chan_t  ),
1233:             .slv_b_chan_t  ( axi_dma_tr_b_chan_t  ),
1234:             .mst_b_chan_t  ( axi_tr_b_chan_t      ),
1235:             .slv_ar_chan_t ( axi_dma_tr_ar_chan_t ),
1236:             .mst_ar_chan_t ( axi_tr_ar_chan_t     ),
1237:             .slv_r_chan_t  ( axi_dma_tr_r_chan_t  ),
1238:             .mst_r_chan_t  ( axi_tr_r_chan_t      ),
1239:             .slv_req_t     ( axi_dma_tr_req_t     ),
1240:             .slv_resp_t    ( axi_dma_tr_resp_t    ),
1241:             .mst_req_t     ( axi_tr_req_t         ),
1242:             .mst_resp_t    ( axi_tr_resp_t        ),
1243:             .NoSlvPorts    ( NumDMADevices        ),
1244:             .MaxWTrans     ( 8                    )
1245:         ) i_dma_iommu_tr_mux (
1246:             .clk_i,
1247:             .rst_ni,
1248:             .test_i      ( 1'b0              ),
1249:             .slv_reqs_i  ( axi_dma_tr_req    ),
1250:             .slv_resps_o ( axi_dma_tr_resp   ),
1251:             .mst_req_o   ( axi_iommu_tr_req  ),
1252:             .mst_resp_i  ( axi_iommu_tr_resp )
1253:         );
1254: 
1255:         for (genvar i = 0; i < NumDMADevices; i++) begin : gen_dma_inst
1256:             dma_core_wrap_intf #(
1257:                 .AXI_ADDR_WIDTH     ( AxiAddrWidth             ),
1258:                 .AXI_DATA_WIDTH     ( AxiDataWidth             ),
1259:                 .AXI_ID_WIDTH       ( DmaAxiIdWidth            ),
1260:                 .AXI_SLV_ID_WIDTH   ( ariane_soc::IdWidthSlave ),
1261:                 .AXI_USER_WIDTH     ( AxiUserWidth             ),
1262:                 .JOB_FIFO_DEPTH     ( 8                        ),
1263:                 // N v2: a frozen batch job is one aligned 4 KiB copy. Keep
1264:                 // its full source data until the paired write can drain, so
1265:                 // a read-ahead burst cannot block the shared R return path.
1266:                 // S and N use the same capacity; other DMA/legacy modes keep
1267:                 // the existing three-beat buffer. Larger/multi-job streams
1268:                 // are outside this prototype's dependency argument.
1269:                 .BUFFER_DEPTH       ( IOMMUNonblockMode != 0 && i < 2 ? 512 : 3 ),
1270:                 .NUM_AX_IN_FLIGHT   ( 8                        ),
1271:                 .MEM_SYS_DEPTH      ( 0                        ),
1272:                 .RAW_COUPLING_AVAIL ( 0                        ),
1273:                 .IS_TWO_D           ( 0                        ),
1274:                 .BATCH_PORT (1'b1),
1275:                 .OBSERVER_PORT      ( i == 0                   ),
1276:                 .LAUNCH_PORT        ( InclIOMMU && i < 2       ),
1277:                 .RESEARCH_PORT      ( InclIOMMU && IOMMUResearchRole != 0 && i == 1 ),
1278: 
1279:                 .DEVICE_ID          ( 24'(10 + i)              ),
1280:                 .PROCESS_ID         ( 20'(20 + i)              ),
1281:                 .AxID               ( '0                       )
1282:             ) i_dma (
1283:                 .obs_issue_o(obs_issue[i]), .obs_done_o(obs_done[i]), .obs_poll_o(obs_poll[i]),
1284:                 .batch_active_o(batch_active[i]),
1285:                 .batch_gap_active_o(batch_gap_active[i]), .job_quiet_o(job_quiet[i]),
1286:                 .fullbuffer_job_o(fullbuffer_jobs[i]),
1287:                 .policy_enable_i(i == 1 && policy_enable),
1288:                 .policy_allow_i(i == 1 && policy_allow),
1289:                 .policy_abort_i(i == 1 && policy_abort),
```

## iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1393

SHA256 `9eaa9edbadef37c5323e15f3c2b201723af1cc58806517ab5f36f92c0d591666`；original4DMA source mux, fullbuffer5120/1 and actual copied config。

```text
1393:         // Linux image. The banked prototype excludes MRIF, not flat MSI.
1394:         function automatic rv_iommu_cfg::rv_iommu_cfg_t nonblock_cfg();
1395:             rv_iommu_cfg::rv_iommu_cfg_t cfg = ariane_soc::RVIOMMUCfg;
1396:             if (IOMMUNonblockMode != 0) cfg.MSITrans = rv_iommu_cfg::MSI_BT_ONLY;
1397:             return cfg;
1398:         endfunction
1399:         initial begin
1400:             if (IOMMUNonblockMode > 3 ||
1401:                 (IOMMUNonblockMode != 0 && (IOMMUResearchRole != 0 || IOMMULegacyE3Observer)))
1402:                 $fatal(1, "Banked prototype cannot use legacy E3/COST observers");
1403:         end
1404:         rv_iommu_top #(
1405:             .RVIOMMUCfg   (nonblock_cfg()),
1406:             .BankedEnable(IOMMUNonblockMode != 0),
1407:             .NonblockEnable(IOMMUNonblockMode != 0),
1408:             .PolicyEnable(IOMMUNonblockMode == 3),
1409:             .BankIdBit(DmaAxiIdWidth),
1410:             .E3Enable(InclDMA),
1411: 
1412:             .axi_tr_aw_chan_t       (axi_tr_aw_chan_t),
1413:             .axi_tr_w_chan_t        (axi_tr_w_chan_t),
1414:             .axi_tr_b_chan_t        (axi_tr_b_chan_t),
1415:             .axi_tr_ar_chan_t       (axi_tr_ar_chan_t),
```

## iommu-platform/tests/test_nonblock_banked.py:21

SHA256 `7e07fc10b831f32937eeb1ae6f4152cf5e4032f7f067ec1d6ea2814ae089f65f`；read-only retained runner module selection; no tests invoked。

```text
21: SOURCES = [
22:     # packages
23:     VENDOR / "common_cells" / "src" / "cf_math_pkg.sv",
24:     VENDOR / "axi" / "src" / "axi_pkg.sv",
25:     IOMMU / "packages" / "rv_iommu_cfg_pkg.sv",
26:     IOMMU / "packages" / "rv_iommu_reg_pkg.sv",
27:     IOMMU / "packages" / "rv_iommu_pkg.sv",
28: 
29:     # common_cells
30:     VENDOR / "common_cells" / "src" / "lzc.sv",
31:     VENDOR / "common_cells" / "src" / "counter.sv",
32:     VENDOR / "common_cells" / "src" / "delta_counter.sv",
33:     VENDOR / "common_cells" / "src" / "fifo_v3.sv",
34:     VENDOR / "common_cells" / "src" / "spill_register_flushable.sv",
35:     VENDOR / "common_cells" / "src" / "spill_register.sv",
36:     VENDOR / "common_cells" / "src" / "rr_arb_tree.sv",
37:     VENDOR / "common_cells" / "src" / "stream_fifo.sv",
38:     VENDOR / "common_cells" / "src" / "stream_arbiter.sv",
39:     VENDOR / "common_cells" / "src" / "stream_arbiter_flushable.sv",
40: 
41:     # AXI
42:     VENDOR / "axi" / "src" / "axi_demux_simple.sv",
43:     VENDOR / "axi" / "src" / "axi_demux.sv",
44:     VENDOR / "axi" / "src" / "axi_mux.sv",
45:     VENDOR / "axi" / "src" / "axi_id_prepend.sv",
46:     VENDOR / "axi" / "src" / "axi_err_slv.sv",
```

## iommu-platform/out/pte-safe-stage-causality-20261014/ar-cause/ar-cause.md:9

SHA256 `0c4f52920fbad9a9dfc6963443beffe37b32ee14852d1fa5583b973e08b5adb5`；retained navigation for mixed vendor path, no old trace parsing。

```text
9: `tests/test_nonblock_actual_dma.py:409–415` 先收 sibling SOURCES，再收 iDMA sources、生产 FIFO 与 SYSTEM extra。`tests/test_nonblock_banked.py:21–45` 明确选择 IOMMU vendor 的 axi_pkg、rr_arb_tree、stream_arbiter、spill_register、counter、axi_mux、axi_demux、axi_demux_simple。SYSTEM extra 在 `tests/test_nonblock_actual_dma.py:173–192` 选择 CVA6 vendor 的 stream_delay、axi_delayer、axi_cut、axi_multicut、axi_xbar_unmuxed、axi_xbar，再加入真实 rv_iommu_ds_if。这是一条混合但各模块唯一的源码链；不能用另一个副本的 mux／demux 推断其实际额度。
10: 
11: `critical-excerpts.txt` 保存所有决定性范围的原文与行号；`source-sha256.json` 保存完整相关文件 SHA256、读取范围和生成时提交。没有编译执行。这里“实际编译副本”指当前 runner 明确列入 SOURCES 的模块副本，不表示本轮重新编译过。
12: 
13: ## 2. 原生通路与每个握手的含义
14: 
15: | 边界 | 源码与实际动作 | payload 能直接影响什么 |
```

## iommu-platform/out/pte-admission-full-rwb-20261014/root/streaming-completion-bound.md:1

SHA256 `950c4756313b3a2bee24c90f18c434a80c2bbc782545ea4704a53096360d380d`；parent conditional formula/assumptions and scope; not rerun old model。

```text
1: # 完整流式写入与B终点：条件边界及正向设计
2: 
3: actualgoal1791949728。本页是root亲自手推，不是运行模型或实验。**完整R/W/B加入后，R-only门限不能直接保持最优；然而下面的正反方向仍依赖未由实际共享DDR建立的服务假设，不准入microbenchmark。** 五个预先冻结普通规则见frozen-controls.json；其端点语义澄清见frozen-controls-endpoints.json。
4: 
5: ## 1. 实际路径必须保留的完成关系
6: 
7: 对策略s和已拥有job j，定义D[j,k,s]为第k份写数据经原R接收、byte buffer与W FIFO后真正对原W路径可用的时刻；H[j,k,s]为原input/output W owner、AW tracker和credit允许此拍继续前进的时刻。它不是“同bank所以可写”，也不要求免费增加MIG AW-before-W限制：AXI的AW/W可能解耦，原生授权与endpoint必须分开。
8: 
9: 令Phi_W[s](t)给出在真实该策略共享服务下、所有原约束满足且数据从t起可用时，这份W实际接受/服务完成的时刻。保持原已经承诺的W顺序：
10: 
11: `T_W[j,k,s] = Phi_W[s](max(D[j,k,s], H[j,k,s], T_W[previous native W,s]))`。
12: 
13: Phi包含已有packet/owner、通道pipeline、反压及共享服务机会；它不是可免费输入的内部DDR状态，也不先假定与s无关。B另记真实对应写burst的终结函数Psi_B[s]，输入至少有该burst的AW拥有/接受、真实WLAST与原错误路径。job正常完成C[j,s]来自最后B与backend last-burst metadata的真实匹配；quiet还包括其他已拥有R/W/B/错误/队列及原生后续供给。
14: 
15: 这样保留了R/W streaming：每份D可以在R-last之前出现，W不能被人为拖到全部R完成后。W owner通常在原WLAST事件推进，不必等B；B仍是完成义务，不能用释放W owner代替它。
16: 
17: ## 2. 用于手算的条件流式写包络
18: 
19: 以下仅用于说明“写入终点能改变选择”，不是实际MIG服务模型。附加条件全部显式：固定正常两当前job与共用前缀；原可证明W顺序为B job后A job；AW/credit已能合法支持各自数据；每job剩余N_j份数据不超过原buffer容量；R数据依次可用、无额外queue/backpressure改变样本；W可用服务以常数w_j推进且跨策略不变；两job末B相对最后W的正常终结间隔为同一个固定z。实际DDR没有建立这些读写独立/固定机会及B law，本轮不给它们数值。
20: 
21: 给定R数据可用时间向量d_j(k)，在这个条件域中：
22: 
23: `U_j = max_k [d_j(k) + (N_j-k+1)*w_j]`。
24: 
25: 第一job W结束为`max(common_W_end+N_B*w_B,U_B)`；第二job还必须等原第一job W顺序完成：
26: 
27: `W_A_end = max(W_B_end+N_A*w_A, U_A)`。
28: 
29: 所有AW授权/原pipeline下界若不能由已证data路径覆盖，要另放入max，不能删除。若d_j(k)=t_j+k*r_j+sigma_j，sigma是原native传播延迟而非人为新增尾部，则
30: 
31: `U_j = t_j + sigma_j + max(r_j+N_j*w_j, N_j*r_j+w_j)`。
32: 
33: 这是真正的流式包络：慢写时首份数据与持续W负载占主导，快写时最后读数据占主导。它不是“R-last加固定写尾巴”。第一、最后B仍各自保留C_B=W_B_end+z与C_A=W_A_end+z；由于原W顺序与N_A*w_A>0，本条件域C_A>C_B，max=C_A，sum=C_A+C_B。
34: 
35: ## 3. 一个保持相同工作量的条件正反方向
36: 
37: 仍取上轮的L/P/Q读交换符号，但明确仅在current P最终为正常叶、其后实际已拥有A数据Q的合法释放gap h跨两策略不变的条件子域使用。当前P是否最终完成、是否fault只能真实返回后确认；调度只能用历史估计，不能预读未来PTE。一般完整walk必须回到第1节，后续PTE会使h内生，不能套本式。
38: 
39: 移去相同已接受前缀；本节另要求共同W前缀已结束、B原W owner与相关授权合法保留，初始DS当前VALID未出现、L新下游尚未VALID。B的L剩余读服务a>0，当前P准备gap g>0/P服务b>0，A数据Q在P服务后h释放。ADMIT读顺序L→P→Q，WAIT为P→L→Q，始终不免费把Q提到已承诺L之前。取a>g且h>a，则B数据流在WAIT整体后移g+b；A的Q读开始分别为a+b+h和g+b+h。
40: 
41: 令U为ADMIT下B剩余流式W结束；A从其Q读开始到流式W需求结束的包络为H；定义
42: 
43: `D = U + N_A*w_A; E = b+h+H`。
44: 
45: 完整A job终点为：
46: 
47: `C_A(admit) = max(D,a+E)+z`；
48: 
49: `C_A(wait) = max(D+g+b,g+E)+z`。
50: 
51: B终点分别U+z和U+g+b+z，原任务字节和全部W/B义务未变。
52: 
53: - **读依赖主导条件：D+b<=E。** 两max中数据释放项主导，ADMIT最晚完成比WAIT晚a-g>0，应WAIT。
54: - **写排空主导相邻边界：D>=a+E。** 原B写入顺序主导，ADMIT最晚完成比WAIT早g+b>0，应ADMIT。
55: 
56: 两条件固定a/g/h及原任务工作，只在已有W服务包络/可用性上区分；没有运行任何延迟变体或增加W压力。它们是符号条件，不是两个已发现的合法平台prefix，更不是面积/物理完成数据。
57: 
58: 普通选择比较另以必要历史有效、各估计在同一声明端点恰好匹配这些条件参数为前提；当前来源并没有取得这组历史，不免费注入它们。在两条件中R-only双门限因min(a,h)>g都WAIT；earlyPause与cheapDebt也WAIT。若L真实直接供应原B W owner，owner/current规则都ADMIT。五冻结普通没有一个作为统一在线规则精确复现两个条件。但这仅是上述假设域的选择比较，不证明真实共享DDR域的独有收益。
59: 
60: ## 4. 一个明确的便宜实现边界，不拼接对照
61: 
62: 完整条件式本身可由常数个加法/max实现：比较
63: 
64: `F_admit=max(D,a+E)`与`F_wait=max(D+g+b,g+E)`，小者优先；相等时B完成更早的ADMIT使sum更小。
65: 
66: 这是本轮推导出的**O-envelope-derived**，不是五个预先冻结在线对照之一，不加入它们逐场景选最优。它在同一个声明域内是一条完整、统一、可执行的闭式规则，准确复现本条件candidate选择；公共paid输入相同，选择逻辑只需要上述两score，未包含两份逐义务rollout。因此“这对条件需要完整预测器才可选择”被这个具体实现否决；未证明真实物理周期或工程数字成本等价。
67: 
68: 对真实路径，N、current W顺序与剩余可用data必须来自原packet/owner；a/g/h/H/U只能由同端点有效服务历史和已拥有依赖形成估计，未知/overflow回退，held与安全先行。部署时还要冻结完整R/W/B service law、history class/width与snapshot，不能凭该纸面公式直接给新microbenchmark准入。
69: 
70: ## 5. 正向产物及停止范围
71: 
72: 留下的正向机制问题是：**用原W顺序和逐拍数据可用性构造完整任务完成包络，能否用小成本准入逻辑正确区分“继续排空当前写owner”与“提前服务翻译释放另一任务”？** 本页已有有界闭式选择和明确失效条件，比单纯PTE等待更接近完整任务；独立创新/物理收益均未接受。
73: 
74: 接受本页：native终点关系；流式W包络的条件定理；五冻结规则在该条件对子上的非统一覆盖；O-envelope-derived对此闭式candidate的统一选择覆盖。
75: 
76: 不接受：条件假设在实际共享MIG成立、自然accepted prefix/有效在线历史、整个原V/A窗口固定双方工作、source-bound full-RWB positive、净收益或新颖性。不能据此关闭完整准入家族/F1全族。下一轮必须有新原始证据让服务/来源假设成立或有明确新机制修订；不自动再次延长本纸面模型。
```
