`default_nettype none
//=============================================================================
// dma_research_policy.sv
//
// Standalone research Method control policy block (bank E00..E5F).
// NEW FILE - no existing RTL is modified.
//
// Implements docs/research-two-bit-v2.md Method control (independent module,
// bank E00 onwards).
//
// Address decode
// --------------
//   index_i is the FULL peripheral index of the E bank, i.e. 0xE00..0xE5F.
//   index_i[12:8] must equal 5'h0E (bank E); index_i[7:0] is the in-bank
//   offset 0x00..0x5F.  Read and write use exactly the same decode; no offset
//   is added or subtracted anywhere.
//
// Register map (in-bank offset)
// ---------------------------------------------------------------------------
//  E00 r  magic     0x49444d414d435431  (IDMAMCT1)
//  E01 r  version   1
//  E02 r  error     sticky, see E_* bits
//  E03 r  status    bit0 armed / bit1 frozen / bit2 abort
//  E04 w  command   1 ARM / 2 ABORT / 3 FREEZE / 4 CLEAR   (reads 0)
//      FREEZE keeps the abort flag and the sticky error diagnostics: a normal
//      FREEZE reads E03=2, a FREEZE of an aborted run reads E03=6.  Only a
//      legal CLEAR (or reset) drops the diagnostics.
//  E05 rw mode                    default 0
//  E06 rw window_cycles           default 16384    (legal 1..WINDOW_MAX)
//  E07 rw target_bytes            default 51540
//  E08 rw deadband_bytes          default 4096
//  E09 rw initial_budget          default 8000
//  E0A rw min_budget              default 1
//  E0B rw max_budget              default 32768
//  E0C rw budget_step             default 512
//  E0D rw credit_cap              default 65536
//  E0E rw hit_weight              default 1
//  E0F rw miss_weight             default 100
//  E10 rw ptw_weight              default 100
//  --- statistics (read only, CounterWidth wide) -----------------------------
//  E20 complete (fully V-active) billing windows
//  E21 V done events
//  E22 V done bytes (4096 per done)
//  E23 windows whose observed bytes < target
//  E24 total shortfall bytes (target - observed)
//  E25 permission_closed_cycles : cycles with policy_enable_o=1 and the
//      admission permission closed by the registered error/abort state or by
//      the balance/feedback deny.  There is no a_wait input, so this is NOT
//      claimed to be actual ADMIT-blocked cycles.
//  E26 total charge
//  E27 total replenish
//  E28 A issue events
//  E29 A done events
//  E2A cycles with non-zero charge
//  E2B A-owned lookup hit events
//  E2C A-owned lookup miss events
//  E2D A-owned PTW return events (ctl_did==DID_A and ptw_r_fire)
//  E2E cycles with ctl_did==DID_A (A-owner cycles)
//  E2F A select fires
//  E30 A-owned lookup events (hit+miss)
//  E31 A-owned lookup miss events
//  E32 A REQ_TRANS/AWAIT_RESP cycles
//  E33 window boundaries closed
//  E34 window boundaries that were NOT fully V-active
//  E35 feedback steps that increased b
//  E36 feedback steps that decreased b
//  E37 mode6 gap loads
//  E38 mode7 stale charges applied
//  E39 cycles with armed=1 and frozen=0
//  E3A final signed balance C (two's complement, 64 bit)
//  E3B minimum b seen
//  E3C maximum b seen
//  E3D final b
//  E3E longest V-active run with no V done (cycles, 64 bit)
//  E3F reserved, reads 0
//  E40..E5F V-done-per-complete-active-window histogram, 32 buckets:
//          bucket k (k=0..30) = windows with k V done, bucket 31 = >=31
// ---------------------------------------------------------------------------
//  Cost per mode:
//    0 unlimited/no charge | 1 A issue = 4096 B | 2 A select = 1
//    3 A lookup miss = 1   | 4 weighted (hit/miss/ptw) | 5 A REQ_TRANS/
//    AWAIT_RESP cycle = 1  | 6 V-only feedback gap (no per-cycle charge)
//    7 stale actual (previous complete fixed window A REQ_TRANS/AWAIT_RESP
//      cycles charged at the next window end)
//=============================================================================

module dma_research_policy #(
    parameter bit          Enabled      = 1'b1,
    parameter int unsigned CounterWidth = 64,
    parameter logic [23:0] DID_A        = 24'd11,   // A translation id (0x0B)
    parameter logic [2:0]  ST_REQ_TRANS = 3'd1,
    parameter logic [2:0]  ST_AWAIT_RESP= 3'd2,
    parameter logic [2:0]  ST_COMPLETE  = 3'd4,     // never charged as actual
    parameter logic [31:0] WINDOW_MAX   = 32'd16_777_215
)(
    input  wire logic        clk_i,
    input  wire logic        rst_ni,

    input  wire logic        cfg_valid_i,
    input  wire logic        cfg_write_i,
    input  wire logic [12:0] index_i,
    input  wire logic [63:0] wdata_i,
    output logic [63:0] rdata_o,

    input  wire logic        quiet_i,
    input  wire logic        supported_i,

    input  wire logic        v_active_i,
    input  wire logic        a_issue_i,
    input  wire logic        a_done_i,
    input  wire logic        v_done_i,
    input  wire logic [2:0]  ctl_state_i,
    input  wire logic [23:0] ctl_did_i,
    input  wire logic        select_fire_i,
    input  wire logic [23:0] select_did_i,
    input  wire logic        lookup_i,
    input  wire logic        lookup_hit_i,
    input  wire logic [23:0] lookup_did_i,
    input  wire logic        ptw_r_fire_i,
    input  wire logic        response_error_i,

    output logic        policy_enable_o,
    output logic        policy_allow_o,
    output logic        policy_abort_o
);

    //=========================================================================
    // 1. Constants
    //=========================================================================
    localparam logic [63:0] MAGIC      = 64'h49444d414d435431; // IDMAMCT1
    localparam logic [63:0] VERSION    = 64'd1;
    localparam logic [63:0] BYTES_DONE = 64'd4096;

    localparam logic [4:0]  BANK_E     = 5'h0E;   // index_i[12:8] for E00..E5F

    // full counter mask; an increment larger than the counter range is still
    // detected (see cnt_ovf).
    localparam logic [63:0] CNT_MASK =
        (CounterWidth >= 64) ? 64'hFFFF_FFFF_FFFF_FFFF
                             : ((64'h1 << CounterWidth[5:0]) - 64'h1);

    // in-bank offsets (0x00..0x5F)
    localparam logic [7:0] R_MAGIC    = 8'h00;
    localparam logic [7:0] R_VERSION  = 8'h01;
    localparam logic [7:0] R_ERROR    = 8'h02;
    localparam logic [7:0] R_STATUS   = 8'h03;
    localparam logic [7:0] R_COMMAND  = 8'h04;
    localparam logic [7:0] R_MODE     = 8'h05;
    localparam logic [7:0] R_WINDOW   = 8'h06;
    localparam logic [7:0] R_TARGET   = 8'h07;
    localparam logic [7:0] R_DEADBAND = 8'h08;
    localparam logic [7:0] R_INITBUD  = 8'h09;
    localparam logic [7:0] R_MINBUD   = 8'h0A;
    localparam logic [7:0] R_MAXBUD   = 8'h0B;
    localparam logic [7:0] R_STEP     = 8'h0C;
    localparam logic [7:0] R_CAP      = 8'h0D;
    localparam logic [7:0] R_WHIT     = 8'h0E;
    localparam logic [7:0] R_WMISS    = 8'h0F;
    localparam logic [7:0] R_WPTW     = 8'h10;
    localparam logic [7:0] R_LASTWR   = 8'h10;

    localparam logic [7:0] S_BALANCE   = 8'h3A;
    localparam logic [7:0] S_BMIN      = 8'h3B;
    localparam logic [7:0] S_BMAX      = 8'h3C;
    localparam logic [7:0] S_BFINAL    = 8'h3D;
    localparam logic [7:0] S_NODONE    = 8'h3E;
    localparam logic [7:0] S_HIST_BASE = 8'h40;
    localparam logic [7:0] S_HIST_LAST = 8'h5F;

    // sticky error bits (E02)
    localparam int E_BADADDR  = 0;  // bad index / wrong bank / high bits set
    localparam int E_BADCMD   = 1;  // unknown command code
    localparam int E_UNSUP    = 2;  // ARM/ABORT while !supported_i or Enabled=0
    localparam int E_CFGLOCK  = 3;  // config write while armed
    localparam int E_CNTOVF   = 4;  // statistics counter overflow
    localparam int E_BALOVF   = 5;  // signed balance overflow during charge/refill
    localparam int E_CLEARILL = 6;  // illegal CLEAR (armed or not quiet)
    localparam int E_FRZILL   = 7;  // FREEZE while not quiet
    localparam int E_ARMSTATE = 8;  // ARM/ABORT/FREEZE in a wrong state
    localparam int E_OBS      = 9;  // observation error while armed
    localparam int E_BADCFG   = 10; // config does not satisfy ARM predicates
    localparam int E_SUPDROP  = 11; // supported_i deasserted while armed

    // statistics counter indices -> E20..E39
    localparam int ST_WIN       = 0;  // E20
    localparam int ST_VDONE     = 1;  // E21
    localparam int ST_VBYTES    = 2;  // E22
    localparam int ST_SHORTW    = 3;  // E23
    localparam int ST_SHORTB    = 4;  // E24
    localparam int ST_PERMCLOSE = 5;  // E25 permission_closed_cycles
    localparam int ST_CHARGE    = 6;  // E26
    localparam int ST_REPLEN    = 7;  // E27
    localparam int ST_AISSUE    = 8;  // E28
    localparam int ST_ADONE     = 9;  // E29
    localparam int ST_CHARGECYC = 10; // E2A
    localparam int ST_HIT       = 11; // E2B
    localparam int ST_MISS      = 12; // E2C
    localparam int ST_PTW       = 13; // E2D
    localparam int ST_AOWN      = 14; // E2E
    localparam int ST_SEL       = 15; // E2F
    localparam int ST_LUTA      = 16; // E30
    localparam int ST_LUTAMISS  = 17; // E31
    localparam int ST_ACTUALCYC = 18; // E32
    localparam int ST_WINALL    = 19; // E33
    localparam int ST_WINPART   = 20; // E34
    localparam int ST_FBUP      = 21; // E35
    localparam int ST_FBDN      = 22; // E36
    localparam int ST_GAPL      = 23; // E37
    localparam int ST_STALE     = 24; // E38
    localparam int ST_ARMEDCYC  = 25; // E39
    localparam int NSTAT        = 26;
    localparam int NHIST        = 32;

    // reset-time defaults
    localparam logic [31:0] DEF_MODE     = 32'd0;
    localparam logic [31:0] DEF_WINDOW   = 32'd16384;
    localparam logic [31:0] DEF_TARGET   = 32'd51540;
    localparam logic [31:0] DEF_DEADBAND = 32'd4096;
    localparam logic [31:0] DEF_INITBUD  = 32'd8000;
    localparam logic [31:0] DEF_MINBUD   = 32'd1;
    localparam logic [31:0] DEF_MAXBUD   = 32'd32768;
    localparam logic [31:0] DEF_STEP     = 32'd512;
    localparam logic [31:0] DEF_CAP      = 32'd65536;
    localparam logic [31:0] DEF_WHIT     = 32'd1;
    localparam logic [31:0] DEF_WMISS    = 32'd100;
    localparam logic [31:0] DEF_WPTW     = 32'd100;

    //=========================================================================
    // 2. State
    //=========================================================================
    logic        armed_q, frozen_q, aborted_q;
    logic [63:0] err_q;

    logic [31:0] mode_q, window_q, target_q, deadband_q, initbud_q,
                 minbud_q, maxbud_q, step_q, cap_q,
                 whit_q, wmiss_q, wptw_q;

    logic signed [63:0] balance_q;
    logic [63:0]        b_q, bmin_q, bmax_q;
    logic [63:0]        prev_main_q, win_main_q;
    logic [63:0]        win_cnt_q, win_vdone_q;
    logic               win_allact_q;
    logic [63:0]        nodone_run_q, nodone_max_q;
    logic [63:0]        m6_cd_q;

    logic [CounterWidth-1:0] stat_q [0:NSTAT-1];
    logic [CounterWidth-1:0] hist_q [0:NHIST-1];

    logic [63:0] stat_inc [0:NSTAT-1];
    logic [64:0] stat_sum [0:NSTAT-1];
    logic [63:0] hist_inc [0:NHIST-1];
    logic [64:0] hist_sum [0:NHIST-1];

    //=========================================================================
    // 3. Helper functions
    //=========================================================================
    // zero-extend a CounterWidth value to 64 bit
    function automatic logic [63:0] zext64(input logic [CounterWidth-1:0] v);
        logic [63:0] r;
        begin
            r = 64'd0;
            for (int i = 0; i < CounterWidth; i++) r[i] = v[i];
            zext64 = r;
        end
    endfunction

    // exact (CounterWidth + 1)-bit sum; the full value is kept so that an
    // increment larger than CNT_MASK is still detected below.
    function automatic logic [64:0] cadd(input logic [CounterWidth-1:0] c,
                                         input logic [63:0]          inc);
        logic [64:0] w;
        begin
            w    = {1'b0, zext64(c)} + {1'b0, inc};
            cadd = w;
        end
    endfunction

    //=========================================================================
    // 4. Event qualifiers (peripheral clk domain)
    //=========================================================================
    logic ev_sel_a, ev_lut_a, ev_lut_hit_a, ev_lut_miss_a, ev_ptw_a,
          ev_actual_a, ev_aown;

    assign ev_sel_a      = select_fire_i && (select_did_i == DID_A);
    assign ev_lut_a      = lookup_i      && (lookup_did_i == DID_A);
    assign ev_lut_hit_a  = ev_lut_a &&  lookup_hit_i;
    assign ev_lut_miss_a = ev_lut_a && !lookup_hit_i;
    assign ev_ptw_a      = ptw_r_fire_i  && (ctl_did_i == DID_A);
    // actual cost: only REQ_TRANS / AWAIT_RESP.  COMPLETE is NOT charged.
    assign ev_actual_a   = (ctl_did_i == DID_A) &&
                           ((ctl_state_i == ST_REQ_TRANS) ||
                            (ctl_state_i == ST_AWAIT_RESP)) &&
                           (ctl_state_i != ST_COMPLETE);
    assign ev_aown       = (ctl_did_i == DID_A);

    //=========================================================================
    // 5. Window control
    //=========================================================================
    logic        win_run, win_last, act_all_now, win_complete;
    logic [63:0] vdone_now, vdone_bytes_now, wnd_m1;

    assign wnd_m1       = {32'd0, window_q} - 64'd1;
    assign win_run      = armed_q & ~frozen_q;
    assign win_last     = win_run & (window_q != 32'd0) & (win_cnt_q == wnd_m1);
    assign act_all_now  = win_allact_q & v_active_i;   // first and last cycle
    assign vdone_now    = win_vdone_q + (v_done_i ? 64'd1 : 64'd0);
    assign win_complete = win_last & act_all_now;

    // win_vdone_q <= window_q <= WINDOW_MAX < 2^64, so 64-bit is sufficient.
    assign vdone_bytes_now = vdone_now * BYTES_DONE;

    //=========================================================================
    // 6. Cost / charge / balance
    //=========================================================================
    logic [63:0] cost_ev, charge_now, refill_now, main_inc, shortfall_bytes;
    logic        stale_cyc, refill_en;

    always_comb begin
        cost_ev = 64'd0;
        case (mode_q)
            32'd0: cost_ev = 64'd0;
            32'd1: cost_ev = a_issue_i ? BYTES_DONE : 64'd0;
            32'd2: cost_ev = ev_sel_a  ? 64'd1 : 64'd0;
            32'd3: cost_ev = ev_lut_miss_a ? 64'd1 : 64'd0;
            32'd4: cost_ev = (ev_lut_hit_a  ? {32'd0, whit_q}  : 64'd0)
                           + (ev_lut_miss_a ? {32'd0, wmiss_q} : 64'd0)
                           + (ev_ptw_a      ? {32'd0, wptw_q}  : 64'd0);
            32'd5: cost_ev = ev_actual_a ? 64'd1 : 64'd0;
            default: cost_ev = 64'd0;   // 6 and 7 carry no per-cycle cost
        endcase
    end

    assign stale_cyc  = win_last & (mode_q == 32'd7);
    assign charge_now = (win_run ? cost_ev : 64'd0) + (stale_cyc ? prev_main_q : 64'd0);

    // mode7 accumulates A REQ_TRANS/AWAIT_RESP cycles (ev_actual_a), NOT all
    // A-owner cycles.
    assign main_inc = (win_run & (mode_q == 32'd7) & ev_actual_a) ? 64'd1 : 64'd0;

    // Refill happens at every complete fixed billing window end (win_last),
    // even when V was inactive or the window was only partially active, so A
    // cannot dead-lock on an empty budget.  Only feedback and the histogram
    // are restricted to fully V-active windows.
    assign refill_en  = win_last & (mode_q != 32'd0) & (mode_q != 32'd6);
    assign refill_now = refill_en ? b_q : 64'd0;

    logic signed [65:0] bal_s, bal_c, bal_r, bal_r_clamped;
    logic signed [65:0] cap_s;
    logic               bal_c_ovf, bal_r_ovf, bal_ovf;
    logic signed [63:0] bal_next;

    assign bal_s = {{2{balance_q[63]}}, balance_q};
    assign bal_c = bal_s - $signed({2'b00, charge_now});
    assign bal_r = bal_c + $signed({2'b00, refill_now});
    assign cap_s = $signed({2'b00, cap_q});

    always_comb begin
        // positive side only is capped by credit_cap; debt is never forgiven
        if (refill_en && (bal_r > cap_s)) bal_r_clamped = cap_s;
        else                              bal_r_clamped = bal_r;
    end

    // signed underflow is caught on the charge itself, before refill/cap could
    // mask it
    assign bal_c_ovf = (bal_c[65:63] != 3'b000) && (bal_c[65:63] != 3'b111);
    assign bal_r_ovf = (bal_r_clamped[65:63] != 3'b000) &&
                       (bal_r_clamped[65:63] != 3'b111);
    assign bal_ovf   = bal_c_ovf | bal_r_ovf;

    assign bal_next = bal_ovf
                    ? (bal_r_clamped[65] ? {1'b1, {63{1'b0}}} : {1'b0, {63{1'b1}}})
                    : bal_r_clamped[63:0];

    //=========================================================================
    // 7. Adaptive feedback
    //=========================================================================
    logic signed [65:0] obs_s, tgt_s, db_s;
    logic               fb_inc, fb_dec;
    logic [63:0]        b_next, b_inc_val, b_dec_val;

    assign obs_s = $signed({2'b00, vdone_bytes_now});
    assign tgt_s = $signed({34'd0, target_q});
    assign db_s  = $signed({34'd0, deadband_q});

    always_comb begin
        // Negative feedback, direction unchanged:
        //   billing modes: V above target -> raise A budget so V drops.
        //   mode 6: V below target -> raise the gap so V recovers.
        if (mode_q == 32'd6) begin
            fb_inc = (obs_s + db_s < tgt_s);   // V slow -> gap up
            fb_dec = (obs_s > tgt_s + db_s);
        end else begin
            fb_inc = (obs_s > tgt_s + db_s);   // V fast -> budget up
            fb_dec = (obs_s + db_s < tgt_s);
        end
    end

    logic signed [64:0] b_up_s, b_dn_s;
    assign b_up_s = $signed({1'b0, b_q}) + $signed({33'd0, step_q});
    assign b_dn_s = $signed({1'b0, b_q}) - $signed({33'd0, step_q});
    assign b_inc_val = (b_up_s > $signed({33'd0, maxbud_q})) ? {32'd0, maxbud_q}
                                                             : b_up_s[63:0];
    assign b_dec_val = (b_dn_s < $signed({33'd0, minbud_q})) ? {32'd0, minbud_q}
                                                             : b_dn_s[63:0];
    assign b_next    = fb_inc ? b_inc_val : (fb_dec ? b_dec_val : b_q);

    assign shortfall_bytes = {32'd0, target_q} - vdone_bytes_now;

    //=========================================================================
    // 8. mode6 gap countdown
    //=========================================================================
    logic [63:0] m6_cd_dec, m6_cd_next;
    logic        gap_load;

    assign m6_cd_dec = (win_run && (m6_cd_q != 64'd0)) ? (m6_cd_q - 64'd1) : m6_cd_q;

    always_comb begin
        m6_cd_next = m6_cd_dec;
        gap_load   = 1'b0;
        // Every A done reloads the gap.  The first copy is admitted before any
        // A done, so it never sees a gap; the first A done already loads one.
        if (win_run && (mode_q == 32'd6) && a_done_i) begin
            m6_cd_next = b_q;
            gap_load   = 1'b1;
        end
    end

    //=========================================================================
    // 9. Statistics increments and overflow
    //=========================================================================
    logic [4:0] hist_idx;
    assign hist_idx = (vdone_now >= 64'd31) ? 5'd31 : vdone_now[4:0];

    always_comb begin
        for (int i = 0; i < NSTAT; i++) stat_inc[i] = 64'd0;
        for (int i = 0; i < NHIST; i++) hist_inc[i] = 64'd0;

        stat_inc[ST_ARMEDCYC]  = win_run ? 64'd1 : 64'd0;
        stat_inc[ST_PERMCLOSE] = perm_closed ? 64'd1 : 64'd0;
        stat_inc[ST_CHARGE]    = charge_now;
        stat_inc[ST_CHARGECYC] = (charge_now != 64'd0) ? 64'd1 : 64'd0;
        stat_inc[ST_REPLEN]    = refill_now;

        stat_inc[ST_VDONE]     = (win_run && v_done_i)  ? 64'd1 : 64'd0;
        stat_inc[ST_VBYTES]    = (win_run && v_done_i)  ? BYTES_DONE : 64'd0;
        stat_inc[ST_AISSUE]    = (win_run && a_issue_i) ? 64'd1 : 64'd0;
        stat_inc[ST_ADONE]     = (win_run && a_done_i)  ? 64'd1 : 64'd0;
        stat_inc[ST_HIT]       = (win_run && ev_lut_hit_a)  ? 64'd1 : 64'd0;
        stat_inc[ST_MISS]      = (win_run && ev_lut_miss_a) ? 64'd1 : 64'd0;
        stat_inc[ST_PTW]       = (win_run && ev_ptw_a)      ? 64'd1 : 64'd0;
        stat_inc[ST_AOWN]      = (win_run && ev_aown)       ? 64'd1 : 64'd0;
        stat_inc[ST_SEL]       = (win_run && ev_sel_a)      ? 64'd1 : 64'd0;
        stat_inc[ST_LUTA]      = (win_run && ev_lut_a)      ? 64'd1 : 64'd0;
        stat_inc[ST_LUTAMISS]  = (win_run && ev_lut_miss_a) ? 64'd1 : 64'd0;
        stat_inc[ST_ACTUALCYC] = (win_run && ev_actual_a)   ? 64'd1 : 64'd0;

        stat_inc[ST_WIN]     = win_complete ? 64'd1 : 64'd0;
        stat_inc[ST_WINALL]  = win_last ? 64'd1 : 64'd0;
        stat_inc[ST_WINPART] = (win_last && !act_all_now) ? 64'd1 : 64'd0;
        stat_inc[ST_GAPL]    = gap_load ? 64'd1 : 64'd0;
        stat_inc[ST_STALE]   = stale_cyc ? 64'd1 : 64'd0;

        stat_inc[ST_SHORTW]  = (win_complete && (obs_s < tgt_s)) ? 64'd1 : 64'd0;
        stat_inc[ST_SHORTB]  = (win_complete && (obs_s < tgt_s)) ? shortfall_bytes
                                                                 : 64'd0;
        stat_inc[ST_FBUP]    = (win_complete && (mode_q != 32'd0) && fb_inc) ? 64'd1 : 64'd0;
        stat_inc[ST_FBDN]    = (win_complete && (mode_q != 32'd0) && fb_dec) ? 64'd1 : 64'd0;

        if (win_complete) hist_inc[hist_idx] = 64'd1;
    end

    logic cnt_ovf;

    always_comb begin
        cnt_ovf = 1'b0;
        for (int i = 0; i < NSTAT; i++) stat_sum[i] = cadd(stat_q[i], stat_inc[i]);
        for (int i = 0; i < NHIST; i++) hist_sum[i] = cadd(hist_q[i], hist_inc[i]);
        // Compare against the full counter mask so an increment larger than
        // the counter range is reported even when the low bits wrap.
        for (int i = 0; i < NSTAT; i++)
            if (stat_sum[i] > {1'b0, CNT_MASK}) cnt_ovf = 1'b1;
        for (int i = 0; i < NHIST; i++)
            if (hist_sum[i] > {1'b0, CNT_MASK}) cnt_ovf = 1'b1;
    end

    //=========================================================================
    // 10. Register write decode (combinational)
    //=========================================================================
    logic        wr_any, wr_ok, bank_sel, obs_err_now, sup_drop_now;
    logic [63:0] new_err, err_add;
    logic        do_arm, do_abort, do_freeze, do_clear, cfg_wr;
    logic        cfg_bad;

    assign bank_sel = (index_i[12:8] == BANK_E);

    assign wr_any = cfg_valid_i & cfg_write_i;
    assign wr_ok  = wr_any
                  & bank_sel
                  & (index_i[7:0] >= R_COMMAND)
                  & (index_i[7:0] <= R_LASTWR)
                  & (wdata_i[63:32] == 32'd0);

    assign cfg_bad = (mode_q > 32'd7)
                   | (window_q == 32'd0)
                   | (window_q > WINDOW_MAX)
                   | ({32'd0, minbud_q} > {32'd0, maxbud_q})
                   | ({32'd0, initbud_q} < {32'd0, minbud_q})
                   | ({32'd0, initbud_q} > {32'd0, maxbud_q})
                   | (step_q == 32'd0)
                   | ({32'd0, cap_q} < {32'd0, maxbud_q})
                   | (((mode_q != 32'd0) && (mode_q != 32'd6)) && (minbud_q == 32'd0))
                   | ((mode_q == 32'd4) &&
                      (({32'd0, whit_q} + {32'd0, wmiss_q} + {32'd0, wptw_q}) == 64'd0));

    always_comb begin
        new_err   = 64'd0;
        do_arm    = 1'b0;
        do_abort  = 1'b0;
        do_freeze = 1'b0;
        do_clear  = 1'b0;
        cfg_wr    = 1'b0;

        if (wr_any && !wr_ok) new_err[E_BADADDR] = 1'b1;

        if (wr_ok) begin
            case (index_i[7:0])
                R_COMMAND: begin
                    case (wdata_i[31:0])
                        32'd1: begin // ARM
                            if (!Enabled || !supported_i)
                                new_err[E_UNSUP]    = 1'b1;
                            else if (armed_q || frozen_q || !quiet_i)
                                new_err[E_ARMSTATE] = 1'b1;
                            else if (cfg_bad)
                                new_err[E_BADCFG]   = 1'b1;
                            else
                                do_arm              = 1'b1;
                        end
                        32'd2: begin // ABORT
                            if (!armed_q || frozen_q)
                                new_err[E_ARMSTATE] = 1'b1;
                            else if (!Enabled || !supported_i)
                                new_err[E_UNSUP]    = 1'b1;
                            else
                                do_abort            = 1'b1;
                        end
                        32'd3: begin // FREEZE (requires fully drained -> quiet)
                            if (!armed_q || frozen_q)
                                new_err[E_ARMSTATE] = 1'b1;
                            else if (!quiet_i)
                                new_err[E_FRZILL]   = 1'b1;
                            else
                                do_freeze           = 1'b1;
                        end
                        32'd4: begin // CLEAR
                            if (!quiet_i || armed_q)
                                new_err[E_CLEARILL] = 1'b1;
                            else
                                do_clear            = 1'b1;
                        end
                        default: new_err[E_BADCMD] = 1'b1;
                    endcase
                end
                default: begin
                    cfg_wr = 1'b1;
                    if (armed_q) new_err[E_CFGLOCK] = 1'b1;
                end
            endcase
        end
    end

    assign obs_err_now  = armed_q & ~frozen_q & response_error_i;
    assign sup_drop_now = armed_q & ~supported_i;

    assign err_add = (obs_err_now  ? (64'd1 << E_OBS)     : 64'd0)
                   | (sup_drop_now ? (64'd1 << E_SUPDROP) : 64'd0)
                   | (cnt_ovf      ? (64'd1 << E_CNTOVF)  : 64'd0)
                   | (bal_ovf      ? (64'd1 << E_BALOVF)  : 64'd0);

    //=========================================================================
    // 11. Policy outputs
    //=========================================================================
    logic enable_core, allow_core, err_any_reg, err_any_now, abort_eff,
          block_all, perm_closed;

    assign enable_core = Enabled & armed_q & ~frozen_q;
    assign err_any_reg = (err_q != 64'd0);
    // any sticky or same-cycle protocol error blocks new admission and aborts
    assign err_any_now = (new_err != 64'd0) | obs_err_now | sup_drop_now
                       | cnt_ovf | bal_ovf;
    // ABORT closes admission the same cycle it is decoded; it does not stop
    // in-flight billing (enable_core stays set until FREEZE).
    assign abort_eff   = aborted_q | do_abort;
    assign block_all   = err_any_reg | err_any_now;

    always_comb begin
        case (mode_q)
            32'd0:   allow_core = 1'b1;                  // unlimited
            32'd6:   allow_core = (m6_cd_next == 64'd0); // gap countdown
            default: allow_core = (bal_next >= 0);       // balance after charge
        endcase
    end

    // E25 uses only the registered error/abort state and the balance decision,
    // never the same-cycle counter overflow, so statistics cannot feed back
    // into the outputs.
    assign perm_closed = enable_core & (abort_eff | err_any_reg | ~allow_core);

    assign policy_enable_o = enable_core;
    assign policy_allow_o  = enable_core & ~abort_eff & ~block_all & allow_core;
    // Only registered errors terminate the frontend.  Its completion/retire
    // logic feeds observation counters, so sending same-cycle overflow back
    // through abort creates a combinational feedback path after synthesis.
    // Admission is still closed immediately above; in-flight work drains and
    // the sticky error terminates the frontend from the next cycle onward.
    assign policy_abort_o  = enable_core & (abort_eff | err_any_reg);

    //=========================================================================
    // 12. Register read mux
    //=========================================================================
    always_comb begin
        rdata_o = 64'd0;
        if (bank_sel) begin
            case (index_i[7:0])
                R_MAGIC:    rdata_o = MAGIC;
                R_VERSION:  rdata_o = VERSION;
                R_ERROR:    rdata_o = err_q;
                R_STATUS:   rdata_o = {61'd0, aborted_q, frozen_q, armed_q};
                R_COMMAND:  rdata_o = 64'd0;
                R_MODE:     rdata_o = {32'd0, mode_q};
                R_WINDOW:   rdata_o = {32'd0, window_q};
                R_TARGET:   rdata_o = {32'd0, target_q};
                R_DEADBAND: rdata_o = {32'd0, deadband_q};
                R_INITBUD:  rdata_o = {32'd0, initbud_q};
                R_MINBUD:   rdata_o = {32'd0, minbud_q};
                R_MAXBUD:   rdata_o = {32'd0, maxbud_q};
                R_STEP:     rdata_o = {32'd0, step_q};
                R_CAP:      rdata_o = {32'd0, cap_q};
                R_WHIT:     rdata_o = {32'd0, whit_q};
                R_WMISS:    rdata_o = {32'd0, wmiss_q};
                R_WPTW:     rdata_o = {32'd0, wptw_q};

                8'h20: rdata_o = zext64(stat_q[ST_WIN]);
                8'h21: rdata_o = zext64(stat_q[ST_VDONE]);
                8'h22: rdata_o = zext64(stat_q[ST_VBYTES]);
                8'h23: rdata_o = zext64(stat_q[ST_SHORTW]);
                8'h24: rdata_o = zext64(stat_q[ST_SHORTB]);
                8'h25: rdata_o = zext64(stat_q[ST_PERMCLOSE]);
                8'h26: rdata_o = zext64(stat_q[ST_CHARGE]);
                8'h27: rdata_o = zext64(stat_q[ST_REPLEN]);
                8'h28: rdata_o = zext64(stat_q[ST_AISSUE]);
                8'h29: rdata_o = zext64(stat_q[ST_ADONE]);
                8'h2A: rdata_o = zext64(stat_q[ST_CHARGECYC]);
                8'h2B: rdata_o = zext64(stat_q[ST_HIT]);
                8'h2C: rdata_o = zext64(stat_q[ST_MISS]);
                8'h2D: rdata_o = zext64(stat_q[ST_PTW]);
                8'h2E: rdata_o = zext64(stat_q[ST_AOWN]);
                8'h2F: rdata_o = zext64(stat_q[ST_SEL]);
                8'h30: rdata_o = zext64(stat_q[ST_LUTA]);
                8'h31: rdata_o = zext64(stat_q[ST_LUTAMISS]);
                8'h32: rdata_o = zext64(stat_q[ST_ACTUALCYC]);
                8'h33: rdata_o = zext64(stat_q[ST_WINALL]);
                8'h34: rdata_o = zext64(stat_q[ST_WINPART]);
                8'h35: rdata_o = zext64(stat_q[ST_FBUP]);
                8'h36: rdata_o = zext64(stat_q[ST_FBDN]);
                8'h37: rdata_o = zext64(stat_q[ST_GAPL]);
                8'h38: rdata_o = zext64(stat_q[ST_STALE]);
                8'h39: rdata_o = zext64(stat_q[ST_ARMEDCYC]);

                S_BALANCE: rdata_o = balance_q;
                S_BMIN:    rdata_o = bmin_q;
                S_BMAX:    rdata_o = bmax_q;
                S_BFINAL:  rdata_o = b_q;
                S_NODONE:  rdata_o = nodone_max_q;

                default: begin
                    if (index_i[7:0] >= S_HIST_BASE && index_i[7:0] <= S_HIST_LAST)
                        rdata_o = zext64(hist_q[index_i[4:0]]);
                    else
                        rdata_o = 64'd0;
                end
            endcase
        end
    end

    //=========================================================================
    // 13. V-active-without-done run length
    //=========================================================================
    logic [63:0] nodone_run_next, nodone_max_next;
    assign nodone_run_next = (win_run && v_active_i && !v_done_i)
                           ? (nodone_run_q + 64'd1) : 64'd0;
    assign nodone_max_next = (nodone_run_next > nodone_max_q)
                           ? nodone_run_next : nodone_max_q;

    //=========================================================================
    // 14. Sequential update
    //=========================================================================
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            armed_q   <= 1'b0;
            frozen_q  <= 1'b0;
            aborted_q <= 1'b0;
            err_q     <= 64'd0;

            mode_q     <= DEF_MODE;
            window_q   <= DEF_WINDOW;
            target_q   <= DEF_TARGET;
            deadband_q <= DEF_DEADBAND;
            initbud_q  <= DEF_INITBUD;
            minbud_q   <= DEF_MINBUD;
            maxbud_q   <= DEF_MAXBUD;
            step_q     <= DEF_STEP;
            cap_q      <= DEF_CAP;
            whit_q     <= DEF_WHIT;
            wmiss_q    <= DEF_WMISS;
            wptw_q     <= DEF_WPTW;

            balance_q   <= {32'd0, DEF_INITBUD};
            b_q         <= {32'd0, DEF_INITBUD};
            bmin_q      <= {32'd0, DEF_INITBUD};
            bmax_q      <= {32'd0, DEF_INITBUD};
            prev_main_q <= 64'd0;
            win_main_q  <= 64'd0;
            win_cnt_q   <= 64'd0;
            win_vdone_q <= 64'd0;
            win_allact_q<= 1'b0;
            nodone_run_q<= 64'd0;
            nodone_max_q<= 64'd0;
            m6_cd_q     <= 64'd0;

            for (int i = 0; i < NSTAT; i++) stat_q[i] <= {CounterWidth{1'b0}};
            for (int i = 0; i < NHIST; i++) hist_q[i] <= {CounterWidth{1'b0}};
        end else begin
            //-----------------------------------------------------------------
            // statistics
            //-----------------------------------------------------------------
            for (int i = 0; i < NSTAT; i++)
                stat_q[i] <= do_clear ? {CounterWidth{1'b0}}
                                      : stat_sum[i][CounterWidth-1:0];
            for (int i = 0; i < NHIST; i++)
                hist_q[i] <= do_clear ? {CounterWidth{1'b0}}
                                      : hist_sum[i][CounterWidth-1:0];

            //-----------------------------------------------------------------
            // datapath (armed, not frozen)
            //-----------------------------------------------------------------
            if (win_run) begin
                balance_q <= bal_next;

                if (win_last) begin
                    win_cnt_q    <= 64'd0;
                    win_allact_q <= 1'b1;
                    win_vdone_q  <= 64'd0;
                end else begin
                    win_cnt_q    <= win_cnt_q + 64'd1;
                    win_allact_q <= act_all_now;
                    win_vdone_q  <= vdone_now;
                end

                // mode7 stale base rotation: every complete fixed window,
                // independent of V activity
                if (win_last) begin
                    prev_main_q <= win_main_q + main_inc;
                    win_main_q  <= 64'd0;
                end else begin
                    win_main_q  <= win_main_q + main_inc;
                end

                // feedback + b seen tracking (fully V-active windows only)
                if (win_complete && (mode_q != 32'd0) && (fb_inc || fb_dec)) begin
                    b_q <= b_next;
                    if (b_next < bmin_q) bmin_q <= b_next;
                    if (b_next > bmax_q) bmax_q <= b_next;
                end

                m6_cd_q <= m6_cd_next;
            end

            nodone_run_q <= nodone_run_next;
            nodone_max_q <= nodone_max_next;

            //-----------------------------------------------------------------
            // commands (win_run is deasserted whenever ARM/CLEAR can fire)
            //-----------------------------------------------------------------
            if (do_arm) begin
                armed_q     <= 1'b1;
                aborted_q   <= 1'b0;
                frozen_q    <= 1'b0;
                balance_q   <= {32'd0, initbud_q};   // no clamping
                b_q         <= {32'd0, initbud_q};
                bmin_q      <= {32'd0, initbud_q};
                bmax_q      <= {32'd0, initbud_q};
                prev_main_q <= 64'd0;
                win_main_q  <= 64'd0;
                win_cnt_q   <= 64'd0;
                win_allact_q<= 1'b1;
                win_vdone_q <= 64'd0;
                m6_cd_q     <= 64'd0;
                nodone_run_q<= 64'd0;
            end

            if (do_abort) aborted_q <= 1'b1;

            if (do_freeze) begin
                // FREEZE (quiet) leaves armed=0 / frozen=1 so the next legal
                // CLEAR is accepted.  aborted_q is deliberately NOT cleared:
                // abort / failure diagnostics are retained until CLEAR, so a
                // frozen aborted run still reads status 6 (frozen|aborted).
                armed_q  <= 1'b0;
                frozen_q <= 1'b1;
            end

            if (do_clear) begin
                armed_q     <= 1'b0;
                aborted_q   <= 1'b0;
                frozen_q    <= 1'b0;
                balance_q   <= {32'd0, initbud_q};
                b_q         <= {32'd0, initbud_q};
                bmin_q      <= {32'd0, initbud_q};
                bmax_q      <= {32'd0, initbud_q};
                prev_main_q <= 64'd0;
                win_main_q  <= 64'd0;
                win_cnt_q   <= 64'd0;
                win_allact_q<= 1'b0;
                win_vdone_q <= 64'd0;
                m6_cd_q     <= 64'd0;
                nodone_run_q<= 64'd0;
                nodone_max_q<= 64'd0;
            end

            //-----------------------------------------------------------------
            // configuration writes (rejected while armed)
            //-----------------------------------------------------------------
            if (cfg_wr && !armed_q) begin
                case (index_i[7:0])
                    R_MODE:     mode_q     <= wdata_i[31:0];
                    R_WINDOW:   window_q   <= wdata_i[31:0];
                    R_TARGET:   target_q   <= wdata_i[31:0];
                    R_DEADBAND: deadband_q <= wdata_i[31:0];
                    R_INITBUD:  initbud_q  <= wdata_i[31:0];
                    R_MINBUD:   minbud_q   <= wdata_i[31:0];
                    R_MAXBUD:   maxbud_q   <= wdata_i[31:0];
                    R_STEP:     step_q     <= wdata_i[31:0];
                    R_CAP:      cap_q      <= wdata_i[31:0];
                    R_WHIT:     whit_q     <= wdata_i[31:0];
                    R_WMISS:    wmiss_q    <= wdata_i[31:0];
                    R_WPTW:     wptw_q     <= wdata_i[31:0];
                    default: ;
                endcase
            end

            //-----------------------------------------------------------------
            // sticky errors: reset or a legal CLEAR are the only recovery
            //-----------------------------------------------------------------
            if (do_clear) err_q <= 64'd0;
            else          err_q <= err_q | new_err | err_add;
        end
    end

endmodule

`default_nettype wire
