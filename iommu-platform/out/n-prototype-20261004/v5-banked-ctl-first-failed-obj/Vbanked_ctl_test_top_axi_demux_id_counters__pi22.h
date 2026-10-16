// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vbanked_ctl_test_top.h for the primary calling header

#ifndef _VBANKED_CTL_TEST_TOP_AXI_DEMUX_ID_COUNTERS__PI22_H_
#define _VBANKED_CTL_TEST_TOP_AXI_DEMUX_ID_COUNTERS__PI22_H_  // guard

#include "verilated_heavy.h"

//==========

class Vbanked_ctl_test_top__Syms;

//----------

VL_MODULE(Vbanked_ctl_test_top_axi_demux_id_counters__pi22) {
  public:
    
    // PORTS
    VL_IN8(clk_i,0,0);
    VL_IN8(rst_ni,0,0);
    VL_IN8(lookup_axi_id_i,3,0);
    VL_OUT8(lookup_mst_select_o,0,0);
    VL_OUT8(lookup_mst_select_occupied_o,0,0);
    VL_OUT8(full_o,0,0);
    VL_IN8(push_axi_id_i,3,0);
    VL_IN8(push_mst_select_i,0,0);
    VL_IN8(push_i,0,0);
    VL_IN8(inject_axi_id_i,3,0);
    VL_IN8(inject_i,0,0);
    VL_IN8(pop_axi_id_i,3,0);
    VL_IN8(pop_i,0,0);
    
    // LOCAL SIGNALS
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        SData/*15:0*/ __PVT__mst_select_q;
        CData/*0:0*/ __PVT__gen_counters__BRA__0__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__0__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__0__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__1__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__1__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__1__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__2__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__2__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__2__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__3__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__3__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__3__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__4__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__4__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__4__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__5__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__5__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__5__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__6__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__6__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__6__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__7__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__7__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__7__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__8__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__8__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__8__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__9__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__9__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__9__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__10__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__10__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__10__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__11__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__11__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__11__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__12__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__12__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__12__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__13__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__13__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__13__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__14__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__14__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__14__KET____DOT__cnt_delta;
        CData/*0:0*/ __PVT__gen_counters__BRA__15__KET____DOT__cnt_en;
        CData/*0:0*/ __PVT__gen_counters__BRA__15__KET____DOT__cnt_down;
        CData/*2:0*/ __PVT__gen_counters__BRA__15__KET____DOT__cnt_delta;
        CData/*3:0*/ __PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__0__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__1__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__2__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__3__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__4__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__5__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__6__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_q;
    };
    struct {
        CData/*3:0*/ __PVT__gen_counters__BRA__7__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__8__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__9__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__10__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__11__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__12__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__13__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__14__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        CData/*3:0*/ __PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_q;
        CData/*3:0*/ __PVT__gen_counters__BRA__15__KET____DOT__i_in_flight_cnt__DOT__counter_d;
        SData/*15:0*/ __PVT__push_en;
        SData/*15:0*/ __PVT__inject_en;
        SData/*15:0*/ __PVT__pop_en;
        SData/*15:0*/ __PVT__occupied;
        SData/*15:0*/ __PVT__cnt_full;
    };
    
    // INTERNAL VARIABLES
  private:
    Vbanked_ctl_test_top__Syms* __VlSymsp;  // Symbol table
  public:
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vbanked_ctl_test_top_axi_demux_id_counters__pi22);  ///< Copying not allowed
  public:
    Vbanked_ctl_test_top_axi_demux_id_counters__pi22(const char* name = "TOP");
    ~Vbanked_ctl_test_top_axi_demux_id_counters__pi22();
    
    // INTERNAL METHODS
    void __Vconfigure(Vbanked_ctl_test_top__Syms* symsp, bool first);
    void _combo__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__10(Vbanked_ctl_test_top__Syms* __restrict vlSymsp);
    void _combo__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__7(Vbanked_ctl_test_top__Syms* __restrict vlSymsp);
    void _combo__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__8(Vbanked_ctl_test_top__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    void _sequent__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__5(Vbanked_ctl_test_top__Syms* __restrict vlSymsp);
    void _sequent__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__6(Vbanked_ctl_test_top__Syms* __restrict vlSymsp);
    void _settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__2(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    void _settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_ar_id_counter__DOT__i_ar_id_counter__4(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    void _settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__1(Vbanked_ctl_test_top__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    void _settle__TOP__banked_ctl_test_top__DOT__i_dut__DOT__i_in_axi_demux__DOT__i_demux_simple__DOT__genblk1__DOT__gen_aw_id_counter__DOT__i_aw_id_counter__3(Vbanked_ctl_test_top__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
