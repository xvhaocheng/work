# Native choice 关键源原文
只读源快照；本文件由 Codex 机械抽取，非模型/仿真。每项 SHA256 为完整源文件，摘录保留原始行号。

## tests/nonblock_actual_dma_top.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_top.sv`

完整文件 SHA256：`81d9566595e20f208a22db27b9b4e9e90678f8394aedb1be13f2a89fd2275c5a`

### 43-60

```systemverilog
   43:     parameter bit          NonblockEnable       = 1,
   44:     parameter int unsigned NumOutstandingTrans = 8,
   45:     parameter int unsigned WFifoDepth          = 32,
   46:     // Test-only elaboration switch, forwarded verbatim to the harness.
   47:     parameter bit          PolicyEnable        = 0,
   48:     parameter int unsigned DmaBufferDepth      = 512,
   49:     parameter int unsigned SharedPoolMode      = 0,
   50:     parameter bit InterleaveAwEnable = 0,
   51:     parameter int unsigned InterleaveAwDelay = 64,
   52:     parameter bit SameBankDmasEnable = 0,
   53:     parameter bit ReverseReadOrderEnable = 0,
   54:     parameter bit SourceReadGateEnable = 0,
   55:     parameter bit FrontierEnable = 0,
   56:     parameter bit FrontierScanEnable = 1,
   57:     parameter int unsigned HeaderParkMode = 0,
   58:     parameter int unsigned HeaderParkDepth = 2,
   59:     parameter bit HeaderDirectEnable = 1,
   60:     parameter int unsigned PoolSlots           = 1
```

### 352-468

```systemverilog
  352:     dma_core_wrap #(
  353:         .AxiAddrWidth     ( AxiAddrWidth    ),
  354:         .AxiDataWidth     ( AxiDataWidth    ),
  355:         .AxiIdWidth       ( DmaAxiIdWidth   ),
  356:         .AxiUserWidth     ( AxiUserWidth    ),
  357:         .AxiSlvIdWidth    ( CfgBusIDWidth   ),
  358:         .NumAxInFlight    ( 8               ),
  359:         .MemSysDepth      ( 0               ),
  360:         .JobFifoDepth     ( 8               ),
  361:         .BufferDepth      ( DmaBufferDepth  ),
  362:         .RAWCouplingAvail ( 1'b0            ),
  363:         .ObserverPort     ( 1'b1            ),   // only V
  364:         .BatchPort        ( 1'b1            ),
  365:         .LaunchPort       ( 1'b1            ),
  366:         .ResearchPort     ( 1'b0            ),   // only A
  367:         .IsTwoD           ( 1'b0            ),
  368:         .axi_mst_req_t    ( axi_tr2_req_t   ),
  369:         .axi_mst_rsp_t    ( axi_tr2_resp_t  ),
  370:         .axi_slv_req_t    ( cfg_req_t       ),
  371:         .axi_slv_rsp_t    ( cfg_resp_t      ),
  372:         .DEVICE_ID        ( 24'd10          ),
  373:         .PROCESS_ID       ( 20'd20          ),
  374:         .AxID             ( 2'd0            )
  375:     ) i_dma0 (
  376:         .clk_i, .rst_ni,
  377:         .testmode_i     ( 1'b0              ),
  378:         .obs_issue_o    ( d0_issue          ),
  379:         .obs_done_o     ( d0_done           ),
  380:         .obs_poll_o     ( d0_poll           ),
  381:         .batch_active_o ( d0_batch_active   ),
  382:         .batch_gap_active_o ( d0_batch_gap  ),
  383:         .job_quiet_o    ( d0_job_quiet      ),
  384:         .fullbuffer_job_o ( d0_fullbuffer   ),
  385:         .policy_enable_i( 1'b0              ),
  386:         .policy_allow_i ( 1'b1              ),
  387:         .policy_abort_i ( 1'b0              ),
  388:         .batch_reverse_i( 1'b0              ),
  389:         .launch_hold_i  ( 1'b0              ),
  390:         .launch_go_i    ( 1'b0              ),
  391:         .launch_cancel_i( 1'b0              ),
  392:         .launch_held_o  ( d0_launch_held    ),
  393:         .launch_active_o( d0_launch_active  ),
  394:         .launch_cancelled_o ( /* open */    ),
  395:         .launch_error_o ( d0_launch_err     ),
  396:         .obs_issue_id_o ( /* open */        ),
  397:         .obs_done_id_o  ( /* open */        ),
  398:         .obs_poll_id_o  ( /* open */        ),
  399:         .obs_cfg_valid_o( /* open */        ),
  400:         .obs_cfg_write_o( /* open */        ),
  401:         .obs_cfg_addr_o ( /* open */        ),
  402:         .obs_cfg_wdata_o( /* open */        ),
  403:         .obs_cfg_wstrb_o( /* open */        ),
  404:         .obs_cfg_rdata_i( 64'd0             ),
  405:         .axi_mst_req_o  ( dma0_mst_req      ),
  406:         .axi_mst_rsp_i  ( dma0_mst_rsp      ),
  407:         .axi_slv_req_i  ( cfg_req[0]        ),
  408:         .axi_slv_rsp_o  ( cfg_rsp[0]        )
  409:     );
  410: 
  411:     dma_core_wrap #(
  412:         .AxiAddrWidth     ( AxiAddrWidth    ),
  413:         .AxiDataWidth     ( AxiDataWidth    ),
  414:         .AxiIdWidth       ( DmaAxiIdWidth   ),
  415:         .AxiUserWidth     ( AxiUserWidth    ),
  416:         .AxiSlvIdWidth    ( CfgBusIDWidth   ),
  417:         .NumAxInFlight    ( 8               ),
  418:         .MemSysDepth      ( 0               ),
  419:         .JobFifoDepth     ( 8               ),
  420:         .BufferDepth      ( DmaBufferDepth  ),
  421:         .RAWCouplingAvail ( 1'b0            ),
  422:         .ObserverPort     ( 1'b0            ),   // only V
  423:         .BatchPort        ( 1'b1            ),
  424:         .LaunchPort       ( 1'b1            ),
  425:         .ResearchPort     ( 1'b1            ),   // only A
  426:         .IsTwoD           ( 1'b0            ),
  427:         .axi_mst_req_t    ( axi_tr2_req_t   ),
  428:         .axi_mst_rsp_t    ( axi_tr2_resp_t  ),
  429:         .axi_slv_req_t    ( cfg_req_t       ),
  430:         .axi_slv_rsp_t    ( cfg_resp_t      ),
  431:         .DEVICE_ID        ( 24'd11          ),
  432:         .PROCESS_ID       ( 20'd21          ),
  433:         .AxID             ( 2'd0            )
  434:     ) i_dma1 (
  435:         .clk_i, .rst_ni,
  436:         .testmode_i     ( 1'b0              ),
  437:         .obs_issue_o    ( d1_issue          ),
  438:         .obs_done_o     ( d1_done           ),
  439:         .obs_poll_o     ( d1_poll           ),
  440:         .batch_active_o ( d1_batch_active   ),
  441:         .batch_gap_active_o ( d1_batch_gap  ),
  442:         .job_quiet_o    ( d1_job_quiet      ),
  443:         .fullbuffer_job_o ( d1_fullbuffer   ),
  444:         .policy_enable_i( 1'b0              ),
  445:         .policy_allow_i ( 1'b1              ),
  446:         .policy_abort_i ( 1'b0              ),
  447:         .batch_reverse_i( 1'b0              ),
  448:         .launch_hold_i  ( 1'b0              ),
  449:         .launch_go_i    ( 1'b0              ),
  450:         .launch_cancel_i( 1'b0              ),
  451:         .launch_held_o  ( d1_launch_held    ),
  452:         .launch_active_o( d1_launch_active  ),
  453:         .launch_cancelled_o ( /* open */    ),
  454:         .launch_error_o ( d1_launch_err     ),
  455:         .obs_issue_id_o ( /* open */        ),
  456:         .obs_done_id_o  ( /* open */        ),
  457:         .obs_poll_id_o  ( /* open */        ),
  458:         .obs_cfg_valid_o( /* open */        ),
  459:         .obs_cfg_write_o( /* open */        ),
  460:         .obs_cfg_addr_o ( /* open */        ),
  461:         .obs_cfg_wdata_o( /* open */        ),
  462:         .obs_cfg_wstrb_o( /* open */        ),
  463:         .obs_cfg_rdata_i( 64'd0             ),
  464:         .axi_mst_req_o  ( dma1_mst_req      ),
  465:         .axi_mst_rsp_i  ( dma1_mst_rsp      ),
  466:         .axi_slv_req_i  ( cfg_req[1]        ),
  467:         .axi_slv_rsp_o  ( cfg_rsp[1]        )
  468:     );
```

### 513-520

```systemverilog
  513:     assign ar_fixture_block = ReverseReadOrderEnable && ar_fixture_active_q &&
  514:         ar_fixture_age_q < 64 && !ar_fixture_released_q;
  515:     always_comb begin
  516:         pool_s_req = {dma1_mst_req, dma0_mst_req};
  517:         pool_s_req[1].ar_valid = dma1_mst_req.ar_valid && !ar_fixture_block;
  518:         dma0_mst_rsp = pool_s_rsp[0];
  519:         dma1_mst_rsp = pool_s_rsp[1];
  520:         dma1_mst_rsp.ar_ready = pool_s_rsp[1].ar_ready && !ar_fixture_block;
```

### 544-558

```systemverilog
  544:     assign pool_m_rsp = {slv_resps[1], slv_resps[0]};
  545:     assign slv_reqs[0] = pool_m_req[0];
  546:     assign slv_reqs[1] = pool_m_req[1];
  547:     assign slv_reqs[2] = '0;
  548:     assign slv_reqs[3] = '0;
  549:     assign dbg_e2_mode = SharedPoolMode;
  550:     assign dbg_e2_slots = (SharedPoolMode == 0 || SharedPoolMode == 7) ? 0 : PoolSlots;
  551:     assign dbg_e2_dma_depth = DmaBufferDepth;
  552:     // Production fullbuffer certificates continue to report real private
  553:     // capacity. The extra test permission is per reserved response, not job.
  554:     assign harness_capacity_permission = SharedPoolMode == 0 ?
  555:         (fullbuffer_jobs_o & {2{!owner_only_i}}) : pool_permission;
  556:     if (SharedPoolMode == 0) begin : gen_no_pool
  557:         assign pool_m_req = pool_s_req;
  558:         assign pool_s_rsp = pool_m_rsp;
```

### 680-704

```systemverilog
  680:     axi_mux #(
  681:         .SlvAxiIDWidth ( DmaAxiIdWidth      ),
  682:         .slv_aw_chan_t ( axi_tr2_aw_chan_t  ),
  683:         .mst_aw_chan_t ( axi_tr4_aw_chan_t  ),
  684:         .w_chan_t      ( axi_tr2_w_chan_t   ),
  685:         .slv_b_chan_t  ( axi_tr2_b_chan_t   ),
  686:         .mst_b_chan_t  ( axi_tr4_b_chan_t   ),
  687:         .slv_ar_chan_t ( axi_tr2_ar_chan_t  ),
  688:         .mst_ar_chan_t ( axi_tr4_ar_chan_t  ),
  689:         .slv_r_chan_t  ( axi_tr2_r_chan_t   ),
  690:         .mst_r_chan_t  ( axi_tr4_r_chan_t   ),
  691:         .slv_req_t     ( axi_tr2_req_t      ),
  692:         .slv_resp_t    ( axi_tr2_resp_t     ),
  693:         .mst_req_t     ( axi_tr4_req_t      ),
  694:         .mst_resp_t    ( axi_tr4_resp_t     ),
  695:         .NoSlvPorts    ( 4                  ),
  696:         .MaxWTrans     ( 8                  )
  697:     ) i_dma_iommu_tr_mux (
  698:         .clk_i,
  699:         .rst_ni,
  700:         .test_i      ( 1'b0             ),
  701:         .slv_reqs_i  ( slv_reqs         ),
  702:         .slv_resps_o ( slv_resps        ),
  703:         .mst_req_o   ( mux_mst_req      ),
  704:         .mst_resp_i  ( mux_mst_resp     )
```

### 711-772

```systemverilog
  711:     always_comb begin : unpack_mux_mst
  712:         tr_aw_valid      = mux_mst_req.aw_valid;
  713:         tr_aw_id         = mux_mst_req.aw.id;
  714:         tr_aw_addr       = mux_mst_req.aw.addr;
  715:         tr_aw_len        = mux_mst_req.aw.len;
  716:         tr_aw_size       = mux_mst_req.aw.size;
  717:         tr_aw_burst      = mux_mst_req.aw.burst;
  718:         tr_aw_prot       = mux_mst_req.aw.prot;
  719:         tr_aw_user       = mux_mst_req.aw.user;
  720:         tr_aw_did        = mux_mst_req.aw.stream_id;
  721:         tr_aw_ssid_valid = mux_mst_req.aw.ss_id_valid;
  722:         tr_aw_subid      = mux_mst_req.aw.substream_id;
  723:         tr_aw_mmu_atst   = mux_mst_req.aw.mmu_atst;
  724:         tr_aw_mmu_valid  = mux_mst_req.aw.mmu_valid;
  725: 
  726:         tr_ar_valid      = mux_mst_req.ar_valid;
  727:         tr_ar_id         = mux_mst_req.ar.id;
  728:         tr_ar_addr       = mux_mst_req.ar.addr;
  729:         tr_ar_len        = mux_mst_req.ar.len;
  730:         tr_ar_size       = mux_mst_req.ar.size;
  731:         tr_ar_burst      = mux_mst_req.ar.burst;
  732:         tr_ar_prot       = mux_mst_req.ar.prot;
  733:         tr_ar_user       = mux_mst_req.ar.user;
  734:         tr_ar_did        = mux_mst_req.ar.stream_id;
  735:         tr_ar_ssid_valid = mux_mst_req.ar.ss_id_valid;
  736:         tr_ar_subid      = mux_mst_req.ar.substream_id;
  737:         tr_ar_mmu_atst   = mux_mst_req.ar.mmu_atst;
  738:         tr_ar_mmu_valid  = mux_mst_req.ar.mmu_valid;
  739: 
  740:         tr_w_valid       = mux_mst_req.w_valid;
  741:         tr_w_data        = mux_mst_req.w.data;
  742:         tr_w_strb        = mux_mst_req.w.strb;
  743:         tr_w_last        = mux_mst_req.w.last;
  744:         tr_w_user        = mux_mst_req.w.user;
  745: 
  746:         tr_b_ready       = mux_mst_req.b_ready;
  747:         tr_r_ready       = mux_mst_req.r_ready;
  748:     end
  749: 
  750:     // Harness response bits -> mux master response
  751:     always_comb begin : pack_mux_mst_resp
  752:         mux_mst_resp = '0;
  753:         mux_mst_resp.aw_ready = tr_aw_ready;
  754:         mux_mst_resp.ar_ready = tr_ar_ready;
  755:         mux_mst_resp.w_ready  = tr_w_ready;
  756:         mux_mst_resp.b_valid  = tr_b_valid;
  757:         mux_mst_resp.b.id     = tr_b_id;
  758:         mux_mst_resp.b.resp   = tr_b_resp;
  759:         mux_mst_resp.b.user   = '0;
  760:         mux_mst_resp.r_valid  = tr_r_valid;
  761:         mux_mst_resp.r.id     = tr_r_id;
  762:         mux_mst_resp.r.data   = tr_r_data;
  763:         mux_mst_resp.r.resp   = tr_r_resp;
  764:         mux_mst_resp.r.last   = tr_r_last;
  765:         mux_mst_resp.r.user   = '0;
  766:     end
  767: 
  768:     // ========================================================================
  769:     // Certificate-driven full-buffer permit (NOT a static 2'b11).
  770:     // Bit b == bank b full-buffer permit; bank0 = V = DMA0, bank1 = A = DMA1.
  771:     // ========================================================================
  772:     assign fullbuffer_jobs_o = { d1_fullbuffer, d0_fullbuffer };
```

### 788-797

```systemverilog
  788:     nonblock_banked_top #(
  789:         .NonblockEnable      ( NonblockEnable      ),
  790:         .BankedEnable        ( 1'b1                ),
  791:         .NumOutstandingTrans ( NumOutstandingTrans ),
  792:         .WFifoDepth          ( WFifoDepth          ),
  793:         .PolicyEnable        ( PolicyEnable        ),
  794:         .CompletionGateEnable( SharedPoolMode >= 4 ),
  795:         .HeaderParkMode(HeaderParkMode), .HeaderParkDepth(HeaderParkDepth),
  796:         .HeaderDirectEnable(HeaderDirectEnable),
  797:         .BankIdBit(SameBankDmasEnable ? 3 : 2)
```

### 970-1066

```systemverilog
  970:         end else begin
  971:             unique case (cfg_state_q)
  972:                 C_IDLE: begin
  973:                     if (cfg_valid) begin
  974:                         cfg_addr_q  <= cfg_addr;
  975:                         cfg_wdata_q <= cfg_wdata;
  976:                         cfg_wstrb_q <= cfg_wstrb;
  977:                         cfg_sel_q   <= cfg_sel;
  978:                         cfg_write_q <= cfg_write;
  979:                         cfg_err_q   <= 1'b0;
  980:                         cfg_aw_done_q <= 1'b0;
  981:                         cfg_w_done_q  <= 1'b0;
  982:                         cfg_state_q <= cfg_write ? C_AW : C_AR;
  983:                     end
  984:                 end
  985:                 C_AW: begin
  986:                     // Offer AW and W independently: the register converter
  987:                     // may withhold AWREADY until WVALID is present.
  988:                     if (cfg_drive_req.aw_valid && cfg_rsp[cfg_sel_q].aw_ready)
  989:                         cfg_aw_done_q <= 1'b1;
  990:                     if (cfg_drive_req.w_valid && cfg_rsp[cfg_sel_q].w_ready)
  991:                         cfg_w_done_q <= 1'b1;
  992:                     if ((cfg_aw_done_q || cfg_rsp[cfg_sel_q].aw_ready) &&
  993:                         (cfg_w_done_q || cfg_rsp[cfg_sel_q].w_ready))
  994:                         cfg_state_q <= C_B;
  995:                 end
  996:                 C_B: begin
  997:                     if (cfg_rsp[cfg_sel_q].b_valid) begin
  998:                         cfg_err_q   <= (cfg_rsp[cfg_sel_q].b.resp != axi_pkg::RESP_OKAY);
  999:                         cfg_rdata_q <= '0;
 1000:                         cfg_state_q <= C_HOLD;
 1001:                     end
 1002:                 end
 1003:                 C_AR: begin
 1004:                     if (cfg_rsp[cfg_sel_q].ar_ready) cfg_state_q <= C_R;
 1005:                 end
 1006:                 C_R: begin
 1007:                     if (cfg_rsp[cfg_sel_q].r_valid) begin
 1008:                         cfg_rdata_q <= cfg_rsp[cfg_sel_q].r.data;
 1009:                         cfg_err_q   <= (cfg_rsp[cfg_sel_q].r.resp != axi_pkg::RESP_OKAY);
 1010:                         cfg_state_q <= C_HOLD;
 1011:                     end
 1012:                 end
 1013:                 C_HOLD: begin
 1014:                     // Hold ready until the external master drops cfg_valid so a
 1015:                     // held request can never be re-issued.
 1016:                     if (!cfg_valid) cfg_state_q <= C_IDLE;
 1017:                 end
 1018:                 default: cfg_state_q <= C_IDLE;
 1019:             endcase
 1020:         end
 1021:     end
 1022: 
 1023:     // Drive the selected slave port; the other port stays completely idle.
 1024:     always_comb begin : cfg_drive
 1025:         cfg_drive_req = '0;
 1026:         cfg_drive_req.aw.id    = '0;
 1027:         cfg_drive_req.aw.addr  = cfg_addr_q;
 1028:         cfg_drive_req.aw.len   = 8'd0;                 // single beat
 1029:         cfg_drive_req.aw.size  = 3'd3;                 // 8 bytes
 1030:         cfg_drive_req.aw.burst = axi_pkg::BURST_INCR;
 1031:         cfg_drive_req.aw.user  = '0;
 1032:         cfg_drive_req.w.data   = cfg_wdata_q;
 1033:         cfg_drive_req.w.strb   = cfg_wstrb_q;
 1034:         cfg_drive_req.w.last   = 1'b1;
 1035:         cfg_drive_req.w.user   = '0;
 1036:         cfg_drive_req.ar.id    = '0;
 1037:         cfg_drive_req.ar.addr  = cfg_addr_q;
 1038:         cfg_drive_req.ar.len   = 8'd0;
 1039:         cfg_drive_req.ar.size  = 3'd3;
 1040:         cfg_drive_req.ar.burst = axi_pkg::BURST_INCR;
 1041:         cfg_drive_req.ar.user  = '0;
 1042:         // Response-ready defaults are LOW.  A response may only be consumed in
 1043:         // the state that expects it, so B/R can never be accepted early/lost.
 1044:         cfg_drive_req.b_ready  = 1'b0;
 1045:         cfg_drive_req.r_ready  = 1'b0;
 1046: 
 1047:         unique case (cfg_state_q)
 1048:             // Hold each channel until its own handshake, without waiting for
 1049:             // the other channel or duplicating an already accepted request.
 1050:             C_AW: begin
 1051:                      cfg_drive_req.aw_valid = !cfg_aw_done_q;
 1052:                      cfg_drive_req.w_valid  = !cfg_w_done_q;
 1053:             end
 1054:             C_B:     cfg_drive_req.b_ready  = 1'b1;
 1055:             C_AR:    cfg_drive_req.ar_valid = 1'b1;
 1056:             C_R:     cfg_drive_req.r_ready  = 1'b1;
 1057:             default: ;
 1058:         endcase
 1059: 
 1060:         cfg_req[0] = '0;
 1061:         cfg_req[1] = '0;
 1062:         if (cfg_sel_q) cfg_req[1] = cfg_drive_req;
 1063:         else           cfg_req[0] = cfg_drive_req;
 1064:     end
 1065: 
 1066:     assign cfg_ready = (cfg_state_q == C_HOLD);
```

## tests/nonblock_actual_dma_system_top.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_system_top.sv`

完整文件 SHA256：`36be950d57fc8db2e02246c1f1e9dc61fa7feed8a28ca13fb48a94d5b6d28f01`

### 71-96

```systemverilog
   71:     parameter bit          NonblockEnable       = 1,
   72:     parameter bit          DdrAtomics           = 0,
   73:     // Test-only: add the synthetic CPU plain-write / AMO stimulus.  Only
   74:     // effective together with DdrAtomics (--system-atomics --cpu-writes).
   75:     parameter bit          CpuWrites            = 0,
   76:     parameter int unsigned NumOutstandingTrans = 8,
   77:     parameter int unsigned WFifoDepth          = 32,
   78:     parameter bit          PolicyEnable        = 0,
   79:     parameter int unsigned DmaBufferDepth      = 512,
   80:     parameter int unsigned SharedPoolMode      = 0,
   81:     parameter bit InterleaveAwEnable = 0,
   82:     parameter int unsigned InterleaveAwDelay = 64,
   83:     parameter bit SameBankDmasEnable = 0,
   84:     parameter bit ReverseReadOrderEnable = 0,
   85:     parameter bit SourceReadGateEnable = 0,
   86:     parameter bit FrontierEnable = 0,
   87:     parameter bit FrontierScanEnable = 1,
   88:     parameter int unsigned HeaderParkMode = 0,
   89:     parameter int unsigned HeaderParkDepth = 2,
   90:     parameter bit HeaderDirectEnable = 1,
   91:     parameter int unsigned PoolSlots           = 1,
   92:     // Test-only: cycles between the synthetic CPU read offers.
   93:     parameter int unsigned CpuIssueInterval    = 32,
   94:     // Test-only: DS path delay (board constants).
   95:     parameter int unsigned DsFixedDelayInput   = 50,
   96:     parameter int unsigned DsFixedDelayOutput  = 50
```

### 883-899

```systemverilog
  883:     end
  884: 
  885:     localparam axi_pkg::xbar_cfg_t XbarCfg = '{
  886:         NoSlvPorts:         32'd4,
  887:         NoMstPorts:         32'd2,
  888:         MaxMstTrans:        32'd8,
  889:         MaxSlvTrans:        32'd8,
  890:         FallThrough:        1'b0,
  891:         LatencyMode:        axi_pkg::CUT_ALL_PORTS,
  892:         PipelineStages:     32'd1,
  893:         AxiIdWidthSlvPorts: 32'd4,
  894:         AxiIdUsedSlvPorts:  32'd4,
  895:         UniqueIds:          1'b0,
  896:         AxiAddrWidth:       32'd64,
  897:         AxiDataWidth:       32'd64,
  898:         NoAddrRules:        32'd2
  899:     };
```

### 1102-1119

```systemverilog
 1102:     nonblock_actual_dma_top #(
 1103:         .NonblockEnable      (NonblockEnable),
 1104:         .NumOutstandingTrans (NumOutstandingTrans),
 1105:         .WFifoDepth          (WFifoDepth),
 1106:         .PolicyEnable        (PolicyEnable),
 1107:         .DmaBufferDepth      (DmaBufferDepth),
 1108:         .SharedPoolMode      (SharedPoolMode),
 1109:         .InterleaveAwEnable(InterleaveAwEnable),
 1110:         .InterleaveAwDelay(InterleaveAwDelay),
 1111:         .SameBankDmasEnable(SameBankDmasEnable),
 1112:         .ReverseReadOrderEnable(ReverseReadOrderEnable),
 1113:         .SourceReadGateEnable(SourceReadGateEnable),
 1114:         .FrontierEnable(FrontierEnable),
 1115:         .FrontierScanEnable(FrontierScanEnable),
 1116:         .HeaderParkMode(HeaderParkMode), .HeaderParkDepth(HeaderParkDepth),
 1117:         .HeaderDirectEnable(HeaderDirectEnable),
 1118:         .PoolSlots           (PoolSlots)
 1119:     ) i_dma (
```

## tests/nonblock_banked_top.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/tests/nonblock_banked_top.sv`

完整文件 SHA256：`927c421b5df615a2886f45b0d4206f22446f825eb275bc03c7d9d45ace60fdcc`

### 419-451

```systemverilog
  419:     // -------------------------------------------------------------------------
  420:     // Pack upstream request
  421:     // -------------------------------------------------------------------------
  422:     always_comb begin : pack_tr_req
  423:         tr_req = '0;
  424: 
  425:         tr_req.aw_valid = tr_aw_valid;
  426:         tr_req.aw.id    = tr_aw_id;
  427:         tr_req.aw.addr  = tr_aw_addr;
  428:         tr_req.aw.len   = tr_aw_len;
  429:         tr_req.aw.size  = tr_aw_size;
  430:         tr_req.aw.burst = tr_aw_burst;
  431:         tr_req.aw.prot  = tr_aw_prot;
  432:         tr_req.aw.user  = tr_aw_user;
  433:         tr_req.aw.stream_id    = tr_aw_did;
  434:         tr_req.aw.ss_id_valid  = tr_aw_ssid_valid;
  435:         tr_req.aw.substream_id = tr_aw_subid;
  436:         tr_req.aw.mmu_atst     = tr_aw_mmu_atst;
  437:         tr_req.aw.mmu_valid    = tr_aw_mmu_valid;
  438: 
  439:         tr_req.ar_valid = tr_ar_valid;
  440:         tr_req.ar.id    = tr_ar_id;
  441:         tr_req.ar.addr  = tr_ar_addr;
  442:         tr_req.ar.len   = tr_ar_len;
  443:         tr_req.ar.size  = tr_ar_size;
  444:         tr_req.ar.burst = tr_ar_burst;
  445:         tr_req.ar.prot  = tr_ar_prot;
  446:         tr_req.ar.user  = tr_ar_user;
  447:         tr_req.ar.stream_id    = tr_ar_did;
  448:         tr_req.ar.ss_id_valid  = tr_ar_ssid_valid;
  449:         tr_req.ar.substream_id = tr_ar_subid;
  450:         tr_req.ar.mmu_atst     = tr_ar_mmu_atst;
  451:         tr_req.ar.mmu_valid    = tr_ar_mmu_valid;
```

### 640-656

```systemverilog
  640:     if (BankedEnable) begin : gen_banked
  641: `ifdef TEST_COMPLETION_GATE
  642:     nonblock_completion_banked_ctl #(
  643:         .CompletionGateEnable(CompletionGateEnable),
  644:         .HeaderParkMode(HeaderParkMode), .HeaderParkDepth(HeaderParkDepth),
  645:         .HeaderDirectEnable(HeaderDirectEnable),
  646: `else
  647:     rv_iommu_banked_ctl #(
  648: `endif
  649:         .RVIOMMUCfg         (Cfg),
  650:         .E3Enable           (1'b0),
  651:         .BankIdBit          (BankIdBit),
  652:         .addr_t             (addr_t),
  653:         .paddr_t            (paddr_t),
  654:         .trans_req_data_t   (trans_req_data_t),
  655:         .trans_resp_data_t  (trans_resp_data_t),
  656:         .fault_data_t       (fault_data_t),
```

## cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv`

完整文件 SHA256：`07bd3c1288ee3e53a0924c0cc44967ced960e90493824b7709c0d5fefae85f31`

### 131-207

```systemverilog
  131:   if (!IsTwoD) begin : gen_one_d
  132:     /*
  133:      * DMA Frontend
  134:      */
  135:     idma_reg64_frontend #(
  136:       .ObserverPort ( ObserverPort ),
  137:       .BatchPort ( BatchPort ),
  138:       .LaunchPort ( LaunchPort ),
  139:       .ResearchPort ( ResearchPort ),
  140:       .dma_regs_req_t  ( dma_regs_req_t ),
  141:       .dma_regs_rsp_t  ( dma_regs_rsp_t ),
  142:       .burst_req_t     ( idma_req_t     ),
  143:       .DEVICE_ID       ( DEVICE_ID      ),
  144:       .PROCESS_ID      ( PROCESS_ID     ),
  145:       .AxiIdWidth      ( AxiIdWidth     ),
  146:       .AxID            ( AxID           )
  147:     ) i_dma_frontend (
  148:       .obs_issue_o, .obs_done_o, .obs_poll_o, .batch_active_o, .batch_gap_active_o, .job_quiet_o,
  149:       .policy_enable_i, .policy_allow_i, .policy_abort_i, .batch_reverse_i,
  150:       .launch_hold_i, .launch_go_i, .launch_cancel_i,
  151:       .launch_held_o, .launch_active_o, .launch_cancelled_o, .launch_error_o,
  152:       .obs_issue_id_o, .obs_done_id_o, .obs_poll_id_o,
  153:       .obs_cfg_valid_o, .obs_cfg_write_o, .obs_cfg_addr_o,
  154:       .obs_cfg_wdata_o, .obs_cfg_wstrb_o, .obs_cfg_rdata_i,
  155:       .clk_i,
  156:       .rst_ni,
  157:       // AXI slave: control port
  158:       .dma_ctrl_req_i   ( dma_regs_req      ),
  159:       .dma_ctrl_rsp_o   ( dma_regs_rsp      ),
  160:       // Backend control
  161:       .burst_req_o      ( burst_req_d       ),
  162:       .valid_o          ( be_valid_d        ),
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

### 309-350

```systemverilog
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
  345:     .protocol_req_o( axi_mst_req_o     ),
  346:     .protocol_rsp_i( axi_mst_rsp_i     ),
  347:     .busy_o        ( idma_busy         ),
  348: 
  349:     .ttype_i       ( ttype             ),
  350:     .wdata_i       ( wdata             ),
```

## cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv`

完整文件 SHA256：`b1800c757d28f9c21b0a4a484d888313145c0805194dd7c7766e337f88c57df7`

### 253-267

```systemverilog
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
  267: 
```

### 323-373

```systemverilog
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
```

### 470-569

```systemverilog
  470:             IDLE: begin
  471: 
  472:                 if (LaunchPort && launch_hold_i && launch_next_id_read) begin
  473:                     // Controlled preparation: reserve an ID and enter HOLD, or
  474:                     // reject (invalid config / simultaneous cancel) without
  475:                     // allocating an ID.
  476:                     if (launch_cancel_i || !launch_cfg_ok) begin
  477:                         dma_ctrl_rsp_o.ready = 1;
  478:                         dma_ctrl_rsp_o.error = 1;
  479:                     end else begin
  480:                         issue               = 1'b1;
  481:                         dma_hw2reg.next_id.d = next_id;
  482:                         dma_ctrl_rsp_o.ready = 1;
  483:                         dma_ctrl_rsp_o.error = 0;
  484:                         launch_id_n         = next_id;
  485:                         launch_ctrl_n       = 1'b1;
  486:                         transf_counter_n    = dma_reg2hw.n_trans.q;
  487:                         transf_offset_n     = '0;
  488:                         state_n             = HOLD;
  489:                     end
  490:                 end else if (dma_reg2hw.next_id.re) begin
  491:                     if (batch_enable_q && !batch_cfg_ok) begin
  492:                         dma_ctrl_rsp_o.ready = 1;
  493:                         dma_ctrl_rsp_o.error = 1;
  494:                     end else if (dma_reg2hw.num_bytes.q != '0) begin
  495:                         issue = 1'b1;
  496:                         dma_hw2reg.next_id.d = next_id;
  497:                         transf_counter_n = batch_enable_q ? 64'(batch_target_q) : dma_reg2hw.n_trans.q;
  498:                         if (batch_enable_q && policy_enable) begin
  499:                             // First policy-gated batch copy: reserve the functional ID
  500:                             // and wait for backend idle plus allow before offering VALID.
  501:                             dma_ctrl_rsp_o.ready = 1'b1;
  502:                             transf_offset_n = (batch_enable_q && batch_reverse)
  503:                                 ? ((64'(batch_pages_q) - 64'd1) << 12) : '0;
  504:                             state_n = ADMIT;
  505:                         end else begin
  506:                             dma_ctrl_rsp_o.ready = ready_i;
  507:                             transf_offset_n = (batch_enable_q && batch_reverse)
  508:                                 ? ((64'(batch_pages_q) - 64'd1) << 12) : '0;
  509:                             state_n = COUNT;
  510:                             if (batch_enable_q && !ready_i) begin
  511:                                 issue = 0;
  512:                                 state_n = IDLE;
  513:                             end
  514:                         end
  515:                     end
  516:                 end
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
  567:                     end else begin
  568:                         state_n = COUNT;
  569:                     end
```

## cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv`

完整文件 SHA256：`7db52bf22bc9b8694698e9091dd390f1082c72a5b31ccf248b278315571a1b79`

### 1-47

```systemverilog
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

## cva6/corev_apu/iDMA/src/idma_pkg.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_pkg.sv`

完整文件 SHA256：`0532f59df15e002ee592e722b3c2d3e8f62f0b22330d12bba32852e2cafa6959`

### 107-113

```systemverilog
  107:     function automatic int unsigned determineMaxBeatsPerBurst(protocol_e protocol);
  108:         case (protocol)
  109:         AXI:        return 256;
  110:         // AXI-Lite and OBI do not have bursts
  111:         default:    return 1;
  112:         endcase
  113:     endfunction
```

## cva6/corev_apu/iDMA/src/idma_legalizer.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv`

完整文件 SHA256：`f559a0b0b1d39a9c3ae02b064bf7a67f2b0872551f983d0cf22a4a94ce7c12e6`

### 75-85

```systemverilog
   75:     localparam int unsigned MaxBeatsPerBurst = idma_pkg::determineMaxBeatsPerBurst(Protocol);
   76: 
   77:     /// Stobe width
   78:     localparam int unsigned StrbWidth     = DataWidth / 8;
   79:     /// Offset width
   80:     localparam int unsigned OffsetWidth   = $clog2(StrbWidth);
   81:     /// The size of a page in byte
   82:     localparam int unsigned PageSize      = (MaxBeatsPerBurst * StrbWidth > 4096) ?
   83:                                             4096 :  MaxBeatsPerBurst * StrbWidth;
   84:     /// The width of page offset byte addresses
   85:     localparam int unsigned PageAddrWidth = $clog2(PageSize);
```

### 130-178

```systemverilog
  130:     // Transaction type
  131:     logic is_read;
  132:     logic is_write;
  133:     assign is_read = (ttype_i != idma_pkg::WRITE);
  134:     assign is_write = (ttype_i != idma_pkg::READ);
  135: 
  136:     //--------------------------------------
  137:     // read page boundary check
  138:     //--------------------------------------
  139:     // calculate the page with in bits
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
```

### 203-267

```systemverilog
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
  215: 
  216:     // calculate the address offsets aligned to transfer sizes.
  217:     assign r_addr_offset = r_tf_q.addr[OffsetWidth-1:0];
  218:     assign w_addr_offset = w_tf_q.addr[OffsetWidth-1:0];
  219: 
  220:     // legalization process -> read and write is coupled together
  221:     always_comb begin : proc_read_write_transaction
  222: 
  223:         // default: keep state
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
```

### 282-317

```systemverilog
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
  317:         end
```

### 324-400

```systemverilog
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
  369:     end else if (Protocol == idma_pkg::AXI_LITE) begin : gen_axi_lite_ar_aw_req
  370:         // assign the signals for the read meta channel
  371:         assign r_req_o.ar_req = '{
  372:             addr:   { r_tf_q.addr[AddrWidth-1:OffsetWidth], {{OffsetWidth}{1'b0}} },
  373:             prot:   opt_tf_q.src_axi_opt.prot
  374:         };
  375: 
  376:         // assign the signals for the write meta channel
  377:         assign w_req_o.aw_req = '{
  378:             addr:   { w_tf_q.addr[AddrWidth-1:OffsetWidth], {{OffsetWidth}{1'b0}} },
  379:             prot:   opt_tf_q.dst_axi_opt.prot
  380:         };
  381:     end else if (Protocol == idma_pkg::OBI) begin : gen_obi_ar_aw_req
  382:         assign r_req_o.ar_req = '{
  383:                 addr:   { r_tf_q.addr[AddrWidth-1:OffsetWidth], {{OffsetWidth}{1'b0}} },
  384:                 be:     '1, //TODO: Placeholder, read everything
  385:                 we:     1'b0,
  386:                 wdata:  '0
  387:             };
  388: 
  389:         // assign the signals for the write meta channel
  390:         assign w_req_o.aw_req = '{
  391:             addr:   { w_tf_q.addr[AddrWidth-1:OffsetWidth], {{OffsetWidth}{1'b0}} },
  392:             be:     '0,
  393:             we:     1,
  394:             wdata: '0
  395:         };
  396:     end else begin : gen_ar_aw_req_error
  397:         `IDMA_NONSYNTH_BLOCK(
  398:         $fatal(1, "Legalizer: `AR-AW_req` not implemented for requested protocol!");
  399:         )
  400:     end
```

### 449-477

```systemverilog
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

## cva6/corev_apu/iDMA/src/idma_backend.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv`

完整文件 SHA256：`1043573590bf91d831839edca60cb2e6497aaf047a5ad1e9b653d022cfbda5f2`

### 471-473

```systemverilog
  471:     // data path, meta channels, and last queues have to be ready for the legalizer to be ready
  472:     assign r_ready = r_dp_req_in_ready & ar_ready;
  473:     assign w_ready = w_dp_req_in_ready & aw_ready & w_last_ready;
```

### 519-532

```systemverilog
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
```

### 554-570

```systemverilog
  554:     idma_stream_fifo #(
  555:         .Depth        ( NumAxInFlight ),
  556:         .type_t       ( r_dp_req_t    ),
  557:         .PrintInfo    ( PrintFifoInfo )
  558:     ) i_r_dp_req (
  559:         .clk_i,
  560:         .rst_ni,
  561:         .testmode_i,
  562:         .flush_i   ( 1'b0                ),
  563:         .usage_o   ( /* NOT CONNECTED */ ),
  564:         .data_i    ( r_req.r_dp_req      ),
  565:         .valid_i   ( r_valid             ),
  566:         .ready_o   ( r_dp_req_in_ready   ),
  567:         .data_o    ( r_dp_req_out        ),
  568:         .valid_o   ( r_dp_req_out_valid  ),
  569:         .ready_i   ( r_dp_req_out_ready  )
  570:     );
```

### 590-625

```systemverilog
  590:     end else begin : gen_w_dp_fifo
  591:         idma_stream_fifo #(
  592:             .Depth        ( NumAxInFlight ),
  593:             .type_t       ( w_dp_req_t    ),
  594:             .PrintInfo    ( PrintFifoInfo )
  595:         ) i_w_dp_req (
  596:             .clk_i,
  597:             .rst_ni,
  598:             .testmode_i,
  599:             .flush_i   ( 1'b0                ),
  600:             .usage_o   ( /* NOT CONNECTED */ ),
  601:             .data_i    ( w_req.w_dp_req      ),
  602:             .valid_i   ( w_valid             ),
  603:             .ready_o   ( w_dp_req_in_ready   ),
  604:             .data_o    ( w_dp_req_out        ),
  605:             .valid_o   ( w_dp_req_out_valid  ),
  606:             .ready_i   ( w_dp_req_out_ready  )
  607:         );
  608:     end
  609: 
  610:     // Add fall-through register to allow the input to be ready if the output is not. This
  611:     // does not add a cycle of delay
  612:     fall_through_register #(
  613:         .T          ( ar_chan_t )
  614:     ) i_ar_fall_through_register (
  615:         .clk_i,
  616:         .rst_ni,
  617:         .testmode_i,
  618:         .clr_i      ( 1'b0         ),
  619:         .valid_i    ( r_valid      ),
  620:         .ready_o    ( ar_ready     ),
  621:         .data_i     ( r_req.ar_req ),
  622:         .valid_o    ( ar_valid_dp  ),
  623:         .ready_i    ( ar_ready_dp  ),
  624:         .data_o     ( ar_req_dp    )
  625:     );
```

### 631-649

```systemverilog
  631:     //if (Protocol == idma_pkg::AXI) begin : gen_last_flag_fifo
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
```

### 837-857

```systemverilog
  837:     end else begin : gen_r_aw_bypass
  838: 
  839:         // Add fall-through register to allow the input to be ready if the output is not. This
  840:         // does not add a cycle of delay
  841:         fall_through_register #(
  842:             .T          ( aw_chan_t )
  843:         ) i_aw_fall_through_register (
  844:             .clk_i,
  845:             .rst_ni,
  846:             .testmode_i,
  847:             .clr_i      ( 1'b0          ),
  848:             .valid_i    ( w_valid       ),
  849:             .ready_o    ( aw_ready      ),
  850:             .data_i     ( w_req.aw_req  ),
  851:             .valid_o    ( aw_valid_dp   ),
  852:             .ready_i    ( aw_ready_dp   ),
  853:             .data_o     ( aw_req_dp     )
  854:         );
  855: 
  856:         // no unit: not busy
  857:         assign busy_o.raw_coupler_busy = 1'b0;
```

## cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv`

完整文件 SHA256：`4447bec83a3335ee106a627177a1113730c27c9587d30780892eaaa972f7d80a`

### 196-202

```systemverilog
  196:     //--------------------------------------
  197:     // Read meta channel
  198:     //--------------------------------------
  199:     // connect the ar requests to the AXI bus
  200:     assign axi_req_o.ar       = ar_req_i;
  201:     assign axi_req_o.ar_valid = ar_valid_i;
  202:     assign ar_ready_o         = axi_rsp_i.ar_ready;
```

### 254-280

```systemverilog
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
```

### 288-291

```systemverilog
  288:     // connect the aw requests to the AXI bus
  289:     assign axi_req_o.aw       = aw_req_i;
  290:     assign axi_req_o.aw_valid = aw_valid_i;
  291:     assign aw_ready_o         = axi_rsp_i.aw_ready;
```

### 343-360

```systemverilog
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

### 390-396

```systemverilog
  390:     end
  391: 
  392:     // the w last signal should only be applied to the bus if an actual transfer happens
  393:     assign axi_req_o.w.last = (last_w & ready_to_write) | write_q;
  394: 
  395:     // we are ready for the next transfer internally, once the w last signal is applied
  396:     assign w_dp_ready_o = last_w & (write_happening | write_q);
```

## cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv`

完整文件 SHA256：`613f044b4cd2519128dab4b5ac44d1edfb9d53cd84a0c3c4f84d1ea2062bd852`

### 158-164

```systemverilog
  158:     // Per-bank configuration: NumOutstandingTrans and WFifoDepth are SPLIT,
  159:     // everything else (caches, fault/MRIF FIFOs, widths, ...) is unchanged.
  160:     // MSITrans is preserved (no MRIF); InclATS is forced low.
  161:     //=========================================================================
  162:     localparam rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfgBank = rv_iommu_cfg::rv_iommu_cfg_t'{
  163:         NumOutstandingTrans : RVIOMMUCfg.NumOutstandingTrans / 2,
  164:         WFifoDepth          : RVIOMMUCfg.WFifoDepth / 2,
```

### 243-277

```systemverilog
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
```

### 280-308

```systemverilog
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
```

### 314-356

```systemverilog
  314:     for (genvar b = 0; b < 2; b++) begin : gen_bank
  315:         rv_iommu_trans_ctl_wrap #(
  316:             .RVIOMMUCfg         (RVIOMMUCfgBank),
  317:             .E3Enable           (1'b0),   // legacy E3 counters are per-owner only
  318:             .ReadAdmissionEnable(1'b1),
  319:             .FullBufferDID      (24'(10 + b)),
  320:             .addr_t             (addr_t),
  321:             .paddr_t            (paddr_t),
  322:             .trans_req_data_t   (trans_req_data_t),
  323:             .trans_resp_data_t  (trans_resp_data_t),
  324:             .fault_data_t       (fault_data_t),
  325:             .axi_tr_aw_chan_t   (axi_tr_aw_chan_t),
  326:             .axi_tr_w_chan_t    (axi_tr_w_chan_t),
  327:             .axi_tr_ar_chan_t   (axi_tr_ar_chan_t),
  328:             .axi_tr_req_t       (axi_tr_req_t),
  329:             .axi_tr_resp_t      (axi_tr_resp_t),
  330:             .axi_comp_aw_chan_t (axi_comp_aw_chan_t),
  331:             .axi_comp_w_chan_t  (axi_comp_w_chan_t),
  332:             .axi_comp_b_chan_t  (axi_comp_b_chan_t),
  333:             .axi_comp_ar_chan_t (axi_comp_ar_chan_t),
  334:             .axi_comp_r_chan_t  (axi_comp_r_chan_t),
  335:             .axi_comp_req_t     (axi_comp_req_t),
  336:             .axi_comp_resp_t    (axi_comp_resp_t),
  337:             .axi_ds_req_t       (axi_ds_req_t),
  338:             .axi_ds_resp_t      (axi_ds_resp_t)
  339:         ) i_bank (
  340:             .clk_i,
  341:             .rst_ni,
  342:             .read_admit_i       (bank_read_admit[b]),
  343:             .fullbuffer_job_i   (fullbuffer_jobs_i[b]),
  344:             .tr_axi_req_i       (bank_tr_req[b]),
  345:             .tr_axi_resp_o      (bank_tr_resp[b]),
  346:             .comp_axi_resp_i    (bank_comp_resp[b]),
  347:             .comp_axi_req_o     (bank_comp_req[b]),
  348:             .mrif_axi_resp_i    (mrif_axi_resp_i),
  349:             .trans_req_data_o   (bank_trans_req_data[b]),
  350:             .trans_req_valid_o  (bank_trans_req_valid[b]),
  351:             .trans_req_ready_i  (bank_trans_req_ready[b]),
  352:             .trans_resp_data_i  (bank_trans_resp_data[b]),
  353:             .trans_resp_valid_i (bank_trans_resp_valid[b]),
  354:             .trans_resp_ready_o (bank_trans_resp_ready[b]),
  355:             .mrif_fault_ready_i (mrif_fault_ready_i),
  356:             .in_flight_o        (bank_busy[b])
```

### 365-391

```systemverilog
  365:     //=========================================================================
  366:     // Translation request merge (round-robin, stable while valid)
  367:     //   The bank-local request is copied and ONLY `.bank` is overwritten, so
  368:     //   every other header field (iova/did/pid/ttype/...) is preserved bit-exact.
  369:     //=========================================================================
  370:     trans_req_data_t [1:0] bank_req_data;
  371: 
  372:     always_comb begin : bank_req_tag
  373:         bank_req_data[0]       = bank_trans_req_data[0];
  374:         bank_req_data[0].bank  = 1'b0;
  375:         bank_req_data[1]       = bank_trans_req_data[1];
  376:         bank_req_data[1].bank  = 1'b1;
  377:     end
  378: 
  379:     stream_arbiter #(
  380:         .DATA_T  (trans_req_data_t),
  381:         .N_INP   (2),
  382:         .ARBITER ("rr")
  383:     ) i_trans_req_arb (
  384:         .clk_i,
  385:         .rst_ni,
  386:         .inp_data_i  (bank_req_data),
  387:         .inp_valid_i (bank_trans_req_valid),
  388:         .inp_ready_o (bank_trans_req_ready),
  389:         .oup_data_o  (trans_req_data_o),
  390:         .oup_valid_o (trans_req_valid_o),
  391:         .oup_ready_i (trans_req_ready_i)
```

### 549-586

```systemverilog
  549:     assign aw_accept        = tr_axi_req_i.aw_valid && tr_axi_resp_o.aw_ready;
  550:     assign ar_accept        = tr_axi_req_i.ar_valid && tr_axi_resp_o.ar_ready;
  551:     assign w_accept         = tr_axi_req_i.w_valid  && tr_axi_resp_o.w_ready;
  552:     assign w_nonlast_accept = w_accept && !tr_axi_req_i.w.last;
  553:     assign w_last_accept    = w_accept &&  tr_axi_req_i.w.last;
  554:     assign b_return         = tr_axi_resp_o.b_valid && tr_axi_req_i.b_ready;
  555:     assign r_last_return    = tr_axi_resp_o.r_valid && tr_axi_req_i.r_ready && tr_axi_resp_o.r.last;
  556: 
  557:     logic [31:0] inflight_aw_q, inflight_ar_q, inflight_w_q;
  558:     logic        w_partial_q;
  559: 
  560:     always_ff @(posedge clk_i or negedge rst_ni) begin : inflight_counters
  561:         if (!rst_ni) begin
  562:             inflight_aw_q <= 32'd0;
  563:             inflight_ar_q <= 32'd0;
  564:             inflight_w_q  <= 32'd0;
  565:             w_partial_q   <= 1'b0;
  566:         end else begin
  567:             inflight_aw_q <= inflight_aw_q
  568:                            + (aw_accept     ? 32'd1 : 32'd0)
  569:                            - (b_return      ? 32'd1 : 32'd0);
  570:             inflight_ar_q <= inflight_ar_q
  571:                            + (ar_accept     ? 32'd1 : 32'd0)
  572:                            - (r_last_return ? 32'd1 : 32'd0);
  573:             inflight_w_q  <= inflight_w_q
  574:                            + (w_last_accept ? 32'd1 : 32'd0)
  575:                            - (b_return      ? 32'd1 : 32'd0);
  576:             w_partial_q   <= w_last_accept    ? 1'b0 :
  577:                              w_nonlast_accept ? 1'b1 :
  578:                              w_partial_q;
  579:         end
  580:     end
  581: 
  582:     assign in_flight_o = (inflight_aw_q != 32'd0) ||
  583:                          (inflight_ar_q != 32'd0) ||
  584:                          (inflight_w_q  != 32'd0) ||
  585:                          w_partial_q ||
  586:                          bank_busy[0] || bank_busy[1];
```

## cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`

完整文件 SHA256：`4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f`

### 113-163

```systemverilog
  113:     //----------
  114:     // AX FIFOs
  115:     //----------
  116:     axi_tr_aw_chan_t aw_data;
  117:     logic aw_valid;
  118:     logic aw_ready;
  119:     logic tr_aw_ready;
  120: 
  121:     stream_fifo #(
  122:         .DEPTH(RVIOMMUCfg.NumOutstandingTrans),
  123:         .T    (axi_tr_aw_chan_t)
  124:     ) i_aw_stream_fifo (
  125:         .clk_i      (clk_i),
  126:         .rst_ni     (rst_ni),
  127: 
  128:         .flush_i    ('0),
  129:         .testmode_i ('0),
  130:         .usage_o    (  ),
  131: 
  132:         .data_i     (tr_axi_req_i.aw),
  133:         .valid_i    (tr_axi_req_i.aw_valid),
  134:         .ready_o    (tr_aw_ready),
  135: 
  136:         .data_o     (aw_data),
  137:         .valid_o    (aw_valid),
  138:         .ready_i    (aw_ready)
  139:     );
  140: 
  141:     axi_tr_ar_chan_t ar_data;
  142:     logic ar_valid;
  143:     logic ar_ready;
  144:     logic tr_ar_ready;
  145: 
  146:     stream_fifo #(
  147:         .DEPTH(RVIOMMUCfg.NumOutstandingTrans),
  148:         .T    (axi_tr_ar_chan_t)
  149:     ) i_ar_stream_fifo (
  150:         .clk_i      (clk_i),
  151:         .rst_ni     (rst_ni),
  152: 
  153:         .flush_i    ('0),
  154:         .testmode_i ('0),
  155:         .usage_o    (  ),
  156: 
  157:         .data_i     (tr_axi_req_i.ar),
  158:         .valid_i    (tr_axi_req_i.ar_valid),
  159:         .ready_o    (tr_ar_ready),
  160: 
  161:         .data_o     (ar_data),
  162:         .valid_o    (ar_valid),
  163:         .ready_i    (ar_ready)
```

### 174-180

```systemverilog
  174:     typedef logic [$clog2(RVIOMMUCfg.NumOutstandingTrans):0] w_cnt_t;
  175:     w_cnt_t w_cnt_q;
  176:     logic incr_w_cnt;
  177:     logic decr_w_cnt;
  178:     assign incr_w_cnt = (axi_comp_req.aw_valid & axi_comp_resp.aw_ready);
  179:     assign decr_w_cnt = (w_valid & w_ready & w_data.last);
  180:     logic w_cnt_ovf;
```

### 266-273

```systemverilog
  266:     logic is_read;
  267:     assign is_read = ((trans_req_data_q.ttype == rv_iommu::UNTRANSLATED_RX) ||
  268:                       (trans_req_data_q.ttype == rv_iommu::UNTRANSLATED_R ) ||
  269:                       (trans_req_data_q.ttype == rv_iommu::TRANSLATED_R )   ||
  270:                       (trans_req_data_q.ttype == rv_iommu::TRANSLATED_R )   );
  271:     logic is_write;
  272:     assign is_write = (trans_req_data_q.ttype == rv_iommu::UNTRANSLATED_W ||
  273:                        trans_req_data_q.ttype == rv_iommu::TRANSLATED_W   );
```

### 329-357

```systemverilog
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
```

### 620-745

```systemverilog
  620:         unique case (state_q)
  621: 
  622:             // Monitor incoming requests
  623:             // Forward AW before AR so read-to-write DMA can open the W drain
  624:             // before source read data fills the write-data FIFO.
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
  730:                 state_n = IDLE;
  731:             end
  732:         endcase
  733:     end
  734: 
  735:     always_ff @(posedge clk_i or negedge rst_ni) begin : transaction_ctl_seq
  736:         if (!rst_ni) begin
  737:             state_q             <= IDLE;
  738:             trans_req_data_q    <= '0;
  739:             trans_resp_data_q   <= '0;
  740:         end
  741: 
  742:         else begin
  743:             state_q             <= state_n;
  744:             trans_req_data_q    <= trans_req_data_n;
  745:             trans_resp_data_q   <= trans_resp_data_n;
```

## cva6/vendor/pulp-platform/axi/src/axi_mux.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/vendor/pulp-platform/axi/src/axi_mux.sv`

完整文件 SHA256：`71b6b0187138fe1a659154f939678a92b1b36c3db7a5b74f7b64329beb107e97`

### 43-54

```systemverilog
   43:   parameter int unsigned NoSlvPorts    = 32'd0, // Number of slave ports
   44:   // Maximum number of outstanding transactions per write
   45:   parameter int unsigned MaxWTrans     = 32'd8,
   46:   // If enabled, this multiplexer is purely combinatorial
   47:   parameter bit          FallThrough   = 1'b0,
   48:   // add spill register on write master ports, adds a cycle latency on write channels
   49:   parameter bit          SpillAw       = 1'b1,
   50:   parameter bit          SpillW        = 1'b0,
   51:   parameter bit          SpillB        = 1'b0,
   52:   // add spill register on read master ports, adds a cycle latency on read channels
   53:   parameter bit          SpillAr       = 1'b1,
   54:   parameter bit          SpillR        = 1'b0
```

### 188-290

```systemverilog
  188:     rr_arb_tree #(
  189:       .NumIn    ( NoSlvPorts    ),
  190:       .DataType ( mst_aw_chan_t ),
  191:       .AxiVldRdy( 1'b1          ),
  192:       .LockIn   ( 1'b1          )
  193:     ) i_aw_arbiter (
  194:       .clk_i  ( clk_i           ),
  195:       .rst_ni ( rst_ni          ),
  196:       .flush_i( 1'b0            ),
  197:       .rr_i   ( '0              ),
  198:       .req_i  ( slv_aw_valids   ),
  199:       .gnt_o  ( slv_aw_readies  ),
  200:       .data_i ( slv_aw_chans    ),
  201:       .gnt_i  ( aw_ready        ),
  202:       .req_o  ( aw_valid        ),
  203:       .data_o ( mst_aw_chan     ),
  204:       .idx_o  (                 )
  205:     );
  206: 
  207:     // control of the AW channel
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
  257:     );
  258: 
  259:     spill_register #(
  260:       .T       ( mst_aw_chan_t ),
  261:       .Bypass  ( ~SpillAw      ) // Param indicated that we want a spill reg
  262:     ) i_aw_spill_reg (
  263:       .clk_i   ( clk_i               ),
  264:       .rst_ni  ( rst_ni              ),
  265:       .valid_i ( mst_aw_valid        ),
  266:       .ready_o ( mst_aw_ready        ),
  267:       .data_i  ( mst_aw_chan         ),
  268:       .valid_o ( mst_req_o.aw_valid  ),
  269:       .ready_i ( mst_resp_i.aw_ready ),
  270:       .data_o  ( mst_req_o.aw        )
  271:     );
  272: 
  273:     //--------------------------------------
  274:     // W Channel
  275:     //--------------------------------------
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

### 332-364

```systemverilog
  332:     //--------------------------------------
  333:     rr_arb_tree #(
  334:       .NumIn    ( NoSlvPorts    ),
  335:       .DataType ( mst_ar_chan_t ),
  336:       .AxiVldRdy( 1'b1          ),
  337:       .LockIn   ( 1'b1          )
  338:     ) i_ar_arbiter (
  339:       .clk_i  ( clk_i           ),
  340:       .rst_ni ( rst_ni          ),
  341:       .flush_i( 1'b0            ),
  342:       .rr_i   ( '0              ),
  343:       .req_i  ( slv_ar_valids   ),
  344:       .gnt_o  ( slv_ar_readies  ),
  345:       .data_i ( slv_ar_chans    ),
  346:       .gnt_i  ( ar_ready        ),
  347:       .req_o  ( ar_valid        ),
  348:       .data_o ( mst_ar_chan     ),
  349:       .idx_o  (                 )
  350:     );
  351: 
  352:     spill_register #(
  353:       .T       ( mst_ar_chan_t ),
  354:       .Bypass  ( ~SpillAr      )
  355:     ) i_ar_spill_reg (
  356:       .clk_i   ( clk_i               ),
  357:       .rst_ni  ( rst_ni              ),
  358:       .valid_i ( ar_valid            ),
  359:       .ready_o ( ar_ready            ),
  360:       .data_i  ( mst_ar_chan         ),
  361:       .valid_o ( mst_req_o.ar_valid  ),
  362:       .ready_i ( mst_resp_i.ar_ready ),
  363:       .data_o  ( mst_req_o.ar        )
  364:     );
```

## cva6/vendor/pulp-platform/axi/src/axi_demux_simple.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/vendor/pulp-platform/axi/src/axi_demux_simple.sv`

完整文件 SHA256：`0e25cd8fd1a406bbaf7c941f876a530f10016f445301d836abe14b3bf7784781`

### 154-193

```systemverilog
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
```

### 232-253

```systemverilog
  232:     // This counter steers the demultiplexer of the W channel.
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

### 299-343

```systemverilog
  299:       slv_resp_o.ar_ready    = 1'b0;
  300:       ar_valid        = 1'b0;
  301:       // `lock_ar_valid`: Used to be protocol conform as it is not allowed to deassert `ar_valid`
  302:       // if there was no corresponding `ar_ready`. There is the possibility that an injection
  303:       // of a R response from an `atop` from the AW channel can change the occupied flag of the
  304:       // `i_ar_id_counter`, even if it was previously empty. This FF prevents the deassertion.
  305:       lock_ar_valid_d = lock_ar_valid_q;
  306:       load_ar_lock    = 1'b0;
  307:       // AR id counter
  308:       ar_push         = 1'b0;
  309:       // The process had an arbitration decision in a previous cycle, the valid is locked,
  310:       // wait for the AR transaction.
  311:       if (lock_ar_valid_q) begin
  312:         ar_valid = 1'b1;
  313:         // transaction
  314:         if (ar_ready) begin
  315:           slv_resp_o.ar_ready    = 1'b1;
  316:           ar_push         = 1'b1;
  317:           lock_ar_valid_d = 1'b0;
  318:           load_ar_lock    = 1'b1;
  319:         end
  320:       end else begin
  321:         // The process can start handling AR transaction if `i_ar_id_counter` has space.
  322:         if (!ar_id_cnt_full) begin
  323:           // There is a valid AR, so look the ID up.
  324:           if (slv_req_i.ar_valid && (!ar_select_occupied ||
  325:              (slv_ar_select_i == lookup_ar_select))) begin
  326:             // connect the AR handshake
  327:             ar_valid     = 1'b1;
  328:             // on transaction
  329:             if (ar_ready) begin
  330:               slv_resp_o.ar_ready = 1'b1;
  331:               ar_push      = 1'b1;
  332:             // no transaction this cycle, lock the valid decision!
  333:             end else begin
  334:               lock_ar_valid_d = 1'b1;
  335:               load_ar_lock    = 1'b1;
  336:             end
  337:           end
  338:         end
  339:       end
  340:     end
  341: 
  342:     // this ff is needed so that ar does not get de-asserted if an atop gets injected
  343:     `FFLARN(lock_ar_valid_q, lock_ar_valid_d, load_ar_lock, '0, clk_i, rst_ni)
```

## cva6/vendor/pulp-platform/common_cells/src/stream_fifo.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/vendor/pulp-platform/common_cells/src/stream_fifo.sv`

完整文件 SHA256：`54d952e7ef71db75abdb44c113a61f982312e75105a5a84fe2a6a158858ad85b`

### 14-65

```systemverilog
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
   25:     input  logic                  clk_i,      // Clock
   26:     input  logic                  rst_ni,     // Asynchronous reset active low
   27:     input  logic                  flush_i,    // flush the fifo
   28:     input  logic                  testmode_i, // test_mode to bypass clock gating
   29:     output logic [ADDR_DEPTH-1:0] usage_o,    // fill pointer
   30:     // input interface
   31:     input  T                      data_i,     // data to push into the fifo
   32:     input  logic                  valid_i,    // input data valid
   33:     output logic                  ready_o,    // fifo is not full
   34:     // output interface
   35:     output T                      data_o,     // output data
   36:     output logic                  valid_o,    // fifo is not empty
   37:     input  logic                  ready_i     // pop head from fifo
   38: );
   39: 
   40:     logic push, pop;
   41:     logic empty, full;
   42: 
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
   54:     ) fifo_i (
   55:         .clk_i,
   56:         .rst_ni,
   57:         .flush_i,
   58:         .testmode_i,
   59:         .full_o     (full),
   60:         .empty_o    (empty),
   61:         .usage_o,
   62:         .data_i,
   63:         .push_i     (push),
   64:         .data_o,
   65:         .pop_i      (pop)
```

## cva6/vendor/pulp-platform/common_cells/src/fall_through_register.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/vendor/pulp-platform/common_cells/src/fall_through_register.sv`

完整文件 SHA256：`2ba886585636e1f47d8420b408b5b40a468149ad80a71fd4ba782f1859936661`

### 21-56

```systemverilog
   21:     input  logic    testmode_i,     // Test mode to bypass clock gating
   22:     // Input port
   23:     input  logic    valid_i,
   24:     output logic    ready_o,
   25:     input  T        data_i,
   26:     // Output port
   27:     output logic    valid_o,
   28:     input  logic    ready_i,
   29:     output T        data_o
   30: );
   31: 
   32:     logic   fifo_empty,
   33:             fifo_full;
   34: 
   35:     fifo_v2 #(
   36:         .FALL_THROUGH   (1'b1),
   37:         .DATA_WIDTH     ($size(T)),
   38:         .DEPTH          (1),
   39:         .dtype          (T)
   40:     ) i_fifo (
   41:         .clk_i          (clk_i),
   42:         .rst_ni         (rst_ni),
   43:         .flush_i        (clr_i),
   44:         .testmode_i     (testmode_i),
   45:         .full_o         (fifo_full),
   46:         .empty_o        (fifo_empty),
   47:         .alm_full_o     ( ),
   48:         .alm_empty_o    ( ),
   49:         .data_i         (data_i),
   50:         .push_i         (valid_i & ~fifo_full),
   51:         .data_o         (data_o),
   52:         .pop_i          (ready_i & ~fifo_empty)
   53:     );
   54: 
   55:     assign ready_o = ~fifo_full;
   56:     assign valid_o = ~fifo_empty;
```

## cva6/vendor/pulp-platform/common_cells/src/stream_arbiter.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/vendor/pulp-platform/common_cells/src/stream_arbiter.sv`

完整文件 SHA256：`727cfd2af43220b85c91f4098f9372d10aca1a0115cd614fe98a8730fb5bb1f8`

### 16-47

```systemverilog
   16: module stream_arbiter #(
   17:     parameter type      DATA_T = logic,   // Vivado requires a default value for type parameters.
   18:     parameter integer   N_INP = -1,       // Synopsys DC requires a default value for parameters.
   19:     parameter           ARBITER = "rr"    // "rr" or "prio"
   20: ) (
   21:     input  logic              clk_i,
   22:     input  logic              rst_ni,
   23: 
   24:     input  DATA_T [N_INP-1:0] inp_data_i,
   25:     input  logic  [N_INP-1:0] inp_valid_i,
   26:     output logic  [N_INP-1:0] inp_ready_o,
   27: 
   28:     output DATA_T             oup_data_o,
   29:     output logic              oup_valid_o,
   30:     input  logic              oup_ready_i
   31: );
   32: 
   33:   stream_arbiter_flushable #(
   34:     .DATA_T   (DATA_T),
   35:     .N_INP    (N_INP),
   36:     .ARBITER  (ARBITER)
   37:   ) i_arb (
   38:     .clk_i        (clk_i),
   39:     .rst_ni       (rst_ni),
   40:     .flush_i      (1'b0),
   41:     .inp_data_i   (inp_data_i),
   42:     .inp_valid_i  (inp_valid_i),
   43:     .inp_ready_o  (inp_ready_o),
   44:     .oup_data_o   (oup_data_o),
   45:     .oup_valid_o  (oup_valid_o),
   46:     .oup_ready_i  (oup_ready_i)
   47:   );
```

## cva6/vendor/pulp-platform/common_cells/src/stream_arbiter_flushable.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/vendor/pulp-platform/common_cells/src/stream_arbiter_flushable.sv`

完整文件 SHA256：`8a6ffd292fde633bcfba55ca5a0e89f367aa65406978d5947914979b6096a6d4`

### 16-53

```systemverilog
   16: module stream_arbiter_flushable #(
   17:     parameter type      DATA_T = logic,   // Vivado requires a default value for type parameters.
   18:     parameter integer   N_INP = -1,       // Synopsys DC requires a default value for parameters.
   19:     parameter           ARBITER = "rr"    // "rr" or "prio"
   20: ) (
   21:     input  logic              clk_i,
   22:     input  logic              rst_ni,
   23:     input  logic              flush_i,
   24: 
   25:     input  DATA_T [N_INP-1:0] inp_data_i,
   26:     input  logic  [N_INP-1:0] inp_valid_i,
   27:     output logic  [N_INP-1:0] inp_ready_o,
   28: 
   29:     output DATA_T             oup_data_o,
   30:     output logic              oup_valid_o,
   31:     input  logic              oup_ready_i
   32: );
   33: 
   34:   if (ARBITER == "rr") begin : gen_rr_arb
   35:     rr_arb_tree #(
   36:       .NumIn      (N_INP),
   37:       .DataType   (DATA_T),
   38:       .ExtPrio    (1'b0),
   39:       .AxiVldRdy  (1'b1),
   40:       .LockIn     (1'b1)
   41:     ) i_arbiter (
   42:       .clk_i,
   43:       .rst_ni,
   44:       .flush_i,
   45:       .rr_i   ('0),
   46:       .req_i  (inp_valid_i),
   47:       .gnt_o  (inp_ready_o),
   48:       .data_i (inp_data_i),
   49:       .gnt_i  (oup_ready_i),
   50:       .req_o  (oup_valid_o),
   51:       .data_o (oup_data_o),
   52:       .idx_o  ()
   53:     );
```

## cva6/vendor/pulp-platform/common_cells/src/rr_arb_tree.sv

绝对入口：`/home/xvhaocheng/work/iommu-platform/cva6/vendor/pulp-platform/common_cells/src/rr_arb_tree.sv`

完整文件 SHA256：`c7361280318a605131345932ab3325bc53f8fc76e67c5579b991b96a5b4ffeed`

### 140-196

```systemverilog
  140:     // the final arbitration decision can be taken from the root of the tree
  141:     assign req_o        = req_nodes[0];
  142:     assign data_o       = data_nodes[0];
  143:     assign idx_o        = index_nodes[0];
  144: 
  145:     if (ExtPrio) begin : gen_ext_rr
  146:       assign rr_q       = rr_i;
  147:       assign req_d      = req_i;
  148:     end else begin : gen_int_rr
  149:       idx_t rr_d;
  150: 
  151:       // lock arbiter decision in case we got at least one req and no acknowledge
  152:       if (LockIn) begin : gen_lock
  153:         logic  lock_d, lock_q;
  154:         logic [NumIn-1:0] req_q;
  155: 
  156:         assign lock_d     = req_o & ~gnt_i;
  157:         assign req_d      = (lock_q) ? req_q : req_i;
  158: 
  159:         always_ff @(posedge clk_i or negedge rst_ni) begin : p_lock_reg
  160:           if (!rst_ni) begin
  161:             lock_q <= '0;
  162:           end else begin
  163:             if (flush_i) begin
  164:               lock_q <= '0;
  165:             end else begin
  166:               lock_q <= lock_d;
  167:             end
  168:           end
  169:         end
  170: 
  171:         // pragma translate_off
  172:         `ifndef VERILATOR
  173:           lock: assert property(
  174:             @(posedge clk_i) LockIn |-> req_o &&
  175:                              (!gnt_i && !flush_i) |=> idx_o == $past(idx_o)) else
  176:                 $fatal (1, "Lock implies same arbiter decision in next cycle if output is not \
  177:                             ready.");
  178: 
  179:           logic [NumIn-1:0] req_tmp;
  180:           assign req_tmp = req_q & req_i;
  181:           lock_req: assume property(
  182:             @(posedge clk_i) LockIn |-> lock_d |=> req_tmp == req_q) else
  183:                 $fatal (1, "It is disallowed to deassert unserved request signals when LockIn is \
  184:                             enabled.");
  185:         `endif
  186:         // pragma translate_on
  187: 
  188:         always_ff @(posedge clk_i or negedge rst_ni) begin : p_req_regs
  189:           if (!rst_ni) begin
  190:             req_q  <= '0;
  191:           end else begin
  192:             if (flush_i) begin
  193:               req_q  <= '0;
  194:             end else begin
  195:               req_q  <= req_d;
  196:             end
```
