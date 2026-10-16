#include "Ve3_queue_controller_top.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>
#include <initializer_list>

struct Bench {
    Ve3_queue_controller_top d;
    unsigned cycles = 0;
    void eval() { d.clk_i = 0; d.eval(); }
    void tick() { eval(); d.clk_i = 1; d.eval(); ++cycles; eval(); }
    Bench() {
        d.rst_ni = 0; tick(); d.rst_ni = 1;
        d.trans_ready_i = 1; d.walker_ready_i = 1;
        d.ar_addr_i = 0x1000; d.aw_addr_i = 0x2000;
        tick();
    }
    void count(unsigned n) { assert(d.waiting_o == (d.enabled_o ? n : 0)); assert(!d.queue_error_o); }
    void read(unsigned id, unsigned did) {
        d.ar_id_i = id; d.ar_did_i = did; d.ar_valid_i = 1;
        eval(); assert(d.ar_ready_o); tick(); d.ar_valid_i = 0; eval();
    }
    void select(unsigned id, unsigned did, bool write = false) {
        eval(); assert(d.state_o == 0 && d.select_o);
        assert(d.select_id_o == (id & 255) && d.select_did_o == did && d.select_write_o == write);
        tick(); assert(d.state_o == 1); assert(d.request_o);
        assert(d.request_did_o == did); tick(); assert(d.state_o == 2 && d.response_ready_o);
    }
    void finish() {
        assert(d.state_o == 2); d.response_valid_i = 1; tick(); d.response_valid_i = 0;
        assert(d.state_o == 4);
        for (unsigned i = 0; d.state_o != 0 && i < 20; ++i) tick();
        assert(d.state_o == 0);
    }
    void a_in_service() { read(4, 11); select(4, 11); assert(d.head_id_o == 4 && d.head_did_o == 11); }
};

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    unsigned cases = 0;
    // The key regression: the physical head remains A while V is waiting behind it.
    {
        Bench b; b.a_in_service(); b.count(0); b.read(0, 10); b.count(1);
        for (unsigned i = 0; i < 8; ++i) {
            assert(b.d.head_valid_o && b.d.head_id_o == 4 && b.d.head_did_o == 11);
            assert(!b.d.select_o && b.d.state_o == 2); b.count(1); b.tick();
        }
        b.finish(); b.count(1); assert(b.d.head_id_o == 0 && b.d.head_did_o == 10);
        b.select(0, 10); b.count(0);
        for (unsigned i = 0; i < 8; ++i) {
            assert(b.d.head_valid_o && b.d.head_id_o == 0 && b.d.head_did_o == 10);
            b.count(0); b.tick();
        }
        b.finish(); b.count(0); ++cases;
    }
    // Count changes at selection even when translation request is backpressured.
    {
        Bench b; b.read(0, 10); b.count(1); b.d.trans_ready_i = 0; b.tick();
        assert(b.d.state_o == 1 && b.d.request_o); b.count(0);
        for (unsigned i = 0; i < 5; ++i) { b.tick(); b.count(0); assert(b.d.head_valid_o); }
        b.d.trans_ready_i = 1; b.tick(); b.finish(); ++cases;
    }
    // AW wins when both FIFOs become nonempty, without removing the queued V read.
    {
        Bench b;
        b.d.ar_id_i = 0; b.d.ar_did_i = 10; b.d.ar_valid_i = 1;
        b.d.aw_id_i = 4; b.d.aw_did_i = 11; b.d.aw_valid_i = 1;
        b.eval(); assert(b.d.ar_ready_o && b.d.aw_ready_o); b.tick();
        b.d.ar_valid_i = b.d.aw_valid_i = 0; b.eval(); b.count(1);
        b.select(4, 11, true); b.count(1); b.finish(); b.count(1);
        b.select(0, 10); b.count(0); b.finish(); ++cases;
    }
    // New V acceptance and an already-queued V selection occur on the same edge.
    {
        Bench b; b.read(0, 10); b.count(1);
        b.d.ar_valid_i = 1; b.eval(); assert(b.d.select_o && b.d.ar_ready_o);
        b.tick(); b.d.ar_valid_i = 0; b.eval(); b.count(1);
        assert(b.d.state_o == 1); b.tick(); b.finish();
        b.select(0, 10); b.count(0); b.finish(); ++cases;
    }
    // Both identity dimensions and high ID bits must filter acceptance and selection.
    for (unsigned n = 0; n < 3; ++n) {
        Bench b; unsigned id = n == 0 ? 0 : (n == 1 ? 4 : 256), did = n == 0 ? 11 : 10;
        b.read(id, did); b.count(0); b.select(id, did); b.count(0); b.finish(); ++cases;
    }
    // The queue is continuously maintained: delay ARM must not reset it.
    {
        Bench b; b.a_in_service(); b.read(0, 10); b.count(1);
        b.d.delay_arm_i = 1; b.tick(); b.d.delay_arm_i = 0; b.count(1);
        b.finish(); b.select(0, 10); b.count(0); b.finish(); ++cases;
    }
    // Physical FIFO full: one selected A plus seven queued V; stalled input is not counted.
    {
        Bench b; b.a_in_service();
        for (unsigned n = 1; n <= 7; ++n) { b.read(0, 10); b.count(n); }
        b.d.ar_valid_i = 1; b.eval(); assert(!b.d.ar_ready_o);
        for (unsigned i = 0; i < 4; ++i) { b.tick(); b.count(7); }
        b.d.ar_valid_i = 0; b.finish();
        for (unsigned n = 7; n; --n) { b.select(0, 10); b.count(n - 1); b.finish(); }
        ++cases;
    }
    // Real delay register consumes the real controller count; A miss predicate is modeled.
    for (unsigned delay : {0U, 128U}) {
        Bench b; b.d.delay_enable_i = 1; b.d.delay_arm_i = 1; b.tick(); b.d.delay_arm_i = 0;
        b.d.active_i = b.d.a_miss_i = 1; b.d.delay128_i = delay != 0;
        b.a_in_service(); b.read(0, 10); b.d.memory_valid_i = 1; b.eval();
        assert(b.d.memory_ready_o); assert(bool(b.d.capture_o) == bool(b.d.enabled_o));
        assert(b.d.head_id_o == 4); b.tick(); b.d.memory_valid_i = 0;
        unsigned waited = 0;
        while (!b.d.walker_valid_o && waited < 140) { assert(!b.d.consume_o); b.tick(); ++waited; }
        assert(b.d.walker_valid_o && waited == (b.d.enabled_o ? delay : 0));
        assert(b.d.extra_o == (b.d.enabled_o ? delay : 0));
        assert(bool(b.d.available_o) == bool(b.d.enabled_o)); b.tick();
        // A second response under identical eligibility is not selected again.
        b.d.memory_valid_i = 1; b.eval(); assert(!b.d.capture_o); b.tick(); b.d.memory_valid_i = 0;
        assert(b.d.walker_valid_o); b.tick(); assert(!b.d.delay_error_o);
        b.finish(); b.select(0, 10); b.finish(); ++cases;
    }
    // V already selected remains the AR head but must never qualify a response.
    {
        Bench b; b.d.delay_enable_i = b.d.active_i = b.d.a_miss_i = 1;
        b.read(0, 10); b.select(0, 10); b.d.memory_valid_i = 1; b.eval();
        assert(b.d.head_valid_o && b.d.head_id_o == 0 && !b.d.capture_o); b.count(0); ++cases;
    }
    // Error injection belongs to the isolated production queue: impossible valid controller traces.
    for (bool push : {false, true}) {
        Bench b; b.d.leaf_push_i = push; b.d.leaf_select_i = 1; b.tick();
        assert(b.d.leaf_error_o && b.d.leaf_waiting_o == 0);
        b.d.leaf_push_i = b.d.leaf_select_i = 0; b.tick(); assert(b.d.leaf_error_o); ++cases;
    }
    {
        Bench b; b.d.leaf_push_i = 1;
        for (unsigned n = 1; n <= 8; ++n) { b.tick(); assert(b.d.leaf_waiting_o == n && !b.d.leaf_error_o); }
        b.tick(); assert(b.d.leaf_waiting_o == 8 && b.d.leaf_error_o); ++cases;
    }
    std::printf("PASS %u controller/queue/delay cases; E3Enable=%u; real controller/FIFO/demux, modeled translation engine\n",
                cases, unsigned(Bench{}.d.enabled_o));
}
