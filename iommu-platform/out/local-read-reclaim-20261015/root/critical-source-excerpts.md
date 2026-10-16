# LR1 root direct critical excerpts, no native execution

## cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:254
```
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
```

## cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:343
```
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

## cva6/corev_apu/iDMA/src/idma_backend.sv:471
```
471:     // data path, meta channels, and last queues have to be ready for the legalizer to be ready
472:     assign r_ready = r_dp_req_in_ready & ar_ready;
473:     assign w_ready = w_dp_req_in_ready & aw_ready & w_last_ready;
474: 
```

## cva6/corev_apu/iDMA/src/idma_backend.sv:519
```
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
534:         assign busy_o.eh_cnt_busy = 1'b0;
```

## cva6/corev_apu/iDMA/src/idma_stream_fifo.sv:105
```
105:         stream_fifo #(
106:             .DEPTH        ( Depth  ),
107:             .T            ( type_t ),
108:             // Full 4 KiB N/S byte lanes use the existing FPGA RAM queue.
109:             // Small legacy queues and other payload types keep their storage.
110:             .FORCE_FPGA_RAM ( Depth == 32'd512 && $bits(type_t) == 8 )
```

## cva6/vendor/pulp-platform/common_cells/src/stream_fifo.sv:12
```
12: 
13: module stream_fifo #(
14:     /// FIFO is in fall-through mode
15:     parameter bit          FALL_THROUGH = 1'b0,
16:     /// Default data width if the fifo is of type logic
17:     parameter int unsigned DATA_WIDTH   = 32,
18:     /// Depth can be arbitrary from 0 to 2**32
19:     parameter int unsigned DEPTH        = 8,
20:     parameter type         T            = logic [DATA_WIDTH-1:0],
21:     // DO NOT OVERWRITE THIS PARAMETER
22:     parameter int unsigned ADDR_DEPTH  = (DEPTH > 1) ? $clog2(DEPTH) : 1,
23:     parameter bit          FORCE_FPGA_RAM = 1'b0
24: ) (
```

## cva6/vendor/pulp-platform/common_cells/src/stream_fifo.sv:43
```
43:     assign push    = valid_i & ~full;
44:     assign pop     = ready_i & ~empty;
45:     assign ready_o = ~full;
46:     assign valid_o = ~empty;
47: 
48:     fifo_v3 #(
49:         .FALL_THROUGH   (FALL_THROUGH),
50:         .DATA_WIDTH     (DATA_WIDTH),
51:         .DEPTH          (DEPTH),
52:         .dtype(T),
53:         .FORCE_FPGA_RAM (FORCE_FPGA_RAM)
```

## cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:15
```
15:     logic eligible;
16:     assign eligible = CapacityBytes >= 4096 && normal_copy_i &&
17:                       length_i == 64'd4096 && src_i[11:0] == 0 && dst_i[11:0] == 0;
18:     assign permit_o = pending_q && eligible_q && !fault_q && normal_copy_i;
19: 
```

## cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:173
```
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

## cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:306
```
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
```

## cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:253
```
253:     // Exactly 64-byte, DEFAULT, aligned, batch-disabled, idle-backend transfer with
254:     // an all-zero configuration. CONF is compared per reg_pkg field to avoid packed
255:     // aggregate comparison ambiguities.
256:     assign launch_cfg_ok = (dma_reg2hw.conf.ttype.q     == idma_pkg::DEFAULT) &&
257:                            (dma_reg2hw.conf.pv.q         == 1'b0) &&
258:                            (dma_reg2hw.conf.cache.q      == 4'b0) &&
259:                            (dma_reg2hw.conf.decouple.q   == 1'b0) &&
260:                            (dma_reg2hw.conf.deburst.q    == 1'b0) &&
261:                            (dma_reg2hw.conf.serialize.q  == 1'b0) &&
262:                            (dma_reg2hw.num_bytes.q       == 64'd64) &&
263:                            (dma_reg2hw.src_addr.q[5:0]   == '0) &&
264:                            (dma_reg2hw.dst_addr.q[5:0]   == '0) &&
265:                            !batch_enable_q &&
266:                            backend_idle_i;
```

## cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:519
```
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
```

## cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:275
```
275: 
276:             REGISTER: begin
277:                 if (mem_resp_i.r_valid) begin
278:                     mem_req_o.r_ready   = 1'b1;
279:                     if (mem_resp_i.r.last) begin
280:                         cmd_n[127:64]   = mem_resp_i.r.data[((64*1) &
281:                                                 (RVIOMMUCfg.AxiDataWidth-1))+:64];
282:                         cq_head_o       = (cq_head_i + 1) & idx_mask;
283:                         state_n         = DECODE;
284:                     end
285:                     else cmd_n[63:0]    = mem_resp_i.r.data[((64*0) &
286:                                                 (RVIOMMUCfg.AxiDataWidth-1))+:64];
287: 
288:                     // Check for AXI transmission errors
289:                     if (mem_resp_i.r.resp != axi_pkg::RESP_OKAY) begin
290:                         state_n         = ERROR;
291:                         cqcsr_mf_o      = 1'b1;
292:                         cqcsr_mf_wen_o  = 1'b1;
293:                     end
294:                 end
295:             end
296: 
297:             DECODE: begin
298: 
```

## cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:312
```
312:                         iotinval_o.av       = cmd_iotinval.av;
313:                         iotinval_o.gv       = cmd_iotinval.gv;
314:                         iotinval_o.pscv     = cmd_iotinval.pscv;
315:                         iotinval_o.vpn      = cmd_iotinval.addr[(rv_iommu::GPPNW39-1):0];
316:                         iotinval_o.gscid    = cmd_iotinval.gscid;
317:                         iotinval_o.pscid    = cmd_iotinval.pscid;
```

## cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:348
```
348:                     rv_iommu::IOFENCE: begin
349: 
350:                         // From Spec:
351:                         // A command is determined to be illegal if a reserved bit is set to 1
352:                         if ((|cmd_iofence.reserved_1) || (|cmd_iofence.reserved_2)  ||
353:                             (cmd_iofence.func3 != rv_iommu::IOFENCE_C)) begin
354:                             cqcsr_ill_o     = 1'b1;
355:                             cqcsr_ill_wen_o = 1'b1;
356:                             state_n         = ERROR;
357:                         end
358: 
359:                         // Valid IOFENCE.C command
360:                         else begin
361: 
362:                             // TODO: Add IOFENCE support with PR and PW using R/W counters
363: 
364:                             // From Spec:
365:                             // If AV=1, the IOMMU writes DATA to memory at a 4-byte aligned address ADDR[63:2] * 4
366:                             if(cmd_iofence.av) begin
367:                                 cq_pptr_n   = {cmd_iofence.addr[PLEN-1-2:0], 2'b0};
368:                                 wr_state_n  = AW_REQ;
369:                                 state_n     = WRITE;
370:                             end
371: 
372:                             // Set cqcsr.fence_w_ip if IOMMU supports and uses WSIs
373:                             if (cmd_iofence.wsi && wsi_en_i) begin
374:                                 cqcsr_fence_o       = 1'b1;
375:                                 cqcsr_fence_wen_o   = 1'b1;
376:                             end
377: 
378:                             // Wait for ATS invalidations to complete before proceeding.
379:                             if(atsinval_inflight_i) begin
380:                                ats_fence_valid_o = 1'b1;
381:                                // The ATS synch request has completed.
382:                                if(ats_fence_ready_i) begin
383:                                   state_n = IDLE;
384:                                // An Invalidation timed out.
385:                                end else if(atsinval_to_i) begin
```

## cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:84
```
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
```

## cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:157
```
157:         LEAF: begin
158:           if (accept_o) begin
159:             resp_q <= '0;
160:             resp_q.bank <= req_q.bank;
161:             resp_q.spaddr <= {iotlb_content_i.content_1S.ppn[PPNW-1:0],
162:                               req_q.iova[11:0]};
163:             resp_q.r <= iotlb_content_i.content_1S.r;
164:             resp_q.w <= iotlb_content_i.content_1S.w;
165:             resp_q.x <= iotlb_content_i.content_1S.x;
166:             resp_q.u <= iotlb_content_i.content_1S.u;
167:             resp_q.g <= iotlb_content_i.content_1S.g;
168:             state_q <= OUT;
169:           end else state_q <= REJECT;
170:         end
171:         OUT: if (response_ready_i) state_q <= IDLE;
```

## linux/drivers/misc/idma.c:270
```
270: static int idma_iommu_map_range(struct iommu_domain *domain,
271: 				unsigned long iova, phys_addr_t phys,
272: 				size_t size, size_t pgsize)
273: {
274: 	size_t off;
275: 	int ret;
276: 
277: 	for (off = 0; off < size; off += pgsize) {
278: 		ret = iommu_map(domain, iova + off, phys + off, pgsize,
279: 				IOMMU_READ | IOMMU_WRITE, GFP_KERNEL);
```

## linux/drivers/misc/idma.c:2489
```
2489: 	e.hpm_configured = 1;
2490: 	/*
2491: 	 * Platform workaround: this CVA6 has no DMA snoop of the WT data cache.
2492: 	 * Its FENCE.I drains CPU writes and invalidates D-cache as well as I-cache.
2493: 	 * Hand initialized buffers to DMA before warmup; never flush per transfer.
2494: 	 */
2495: 	local_flush_icache_all();
```

## linux/drivers/iommu/riscv/iommu.c:359
```
359: static inline void riscv_iommu_cmd_iofence(struct riscv_iommu_command *cmd)
360: {
361: 	cmd->dword0 = FIELD_PREP(RISCV_IOMMU_CMD_OPCODE, RISCV_IOMMU_CMD_IOFENCE_OPCODE) |
362: 	    FIELD_PREP(RISCV_IOMMU_CMD_FUNC, RISCV_IOMMU_CMD_IOFENCE_FUNC_C);
363: 	cmd->dword1 = 0;
```

## linux/drivers/iommu/riscv/iommu.c:560
```
560: static bool riscv_iommu_iofence_sync(struct riscv_iommu_device *iommu)
561: {
562: 	struct riscv_iommu_command cmd;
563: 	riscv_iommu_cmd_iofence(&cmd);
564: 	return riscv_iommu_post_sync(iommu, &cmd, true);
565: }
```

## linux/drivers/iommu/riscv/iommu.c:1730
```
1730: static void riscv_iommu_flush_iotlb_range(struct iommu_domain *iommu_domain,
1731: 					  unsigned long *start, unsigned long *end,
1732: 					  size_t *pgsize)
1733: {
1734: 	struct riscv_iommu_domain *domain = iommu_domain_to_riscv(iommu_domain);
1735: 	struct riscv_iommu_command cmd;
1736: 	struct riscv_iommu_endpoint *endpoint;
1737: 	// unsigned long iova;
1738: 	unsigned long payload;
1739: 
1740: 	if (domain->mode == RISCV_IOMMU_DC_FSC_MODE_BARE)
1741: 		return;
1742: 
1743: 	/* Domain not attached to an IOMMU! */
1744: 	BUG_ON(!domain->iommu);
1745: 
1746: 	if (start && end) {
1747: 		payload = riscv_iommu_ats_inval_payload(*start, *end, true);
1748: 	} else {
1749: 		payload = riscv_iommu_ats_inval_all_payload(true);
1750: 	}
1751: 
1752: 	if (domain->g_stage) {
1753: 		riscv_iommu_cmd_inval_gvma(&cmd);
1754: 		riscv_iommu_cmd_inval_set_gscid(&cmd, domain->id);
1755: 	} else {
1756: 		riscv_iommu_cmd_inval_vma(&cmd);
1757: 		// riscv_iommu_cmd_inval_set_pscid(&cmd, domain->id);
1758: 	}
1759: 
1760: 	// if (start && end && pgsize) {
1761: 	// 	/* Cover only the range that is needed */
1762: 	// 	for (iova = *start; iova <= *end; iova += *pgsize) {
1763: 	// 		printk("Sending IOTINVAL with ADDR");
1764: 	// 		riscv_iommu_cmd_inval_set_addr(&cmd, iova);
1765: 	// 		riscv_iommu_post(domain->iommu, &cmd);
1766: 	// 	}
1767: 	// } else {
1768: 		riscv_iommu_post(domain->iommu, &cmd);
1769: 	// }
1770: 	riscv_iommu_iofence_sync(domain->iommu);
1771: 
```
