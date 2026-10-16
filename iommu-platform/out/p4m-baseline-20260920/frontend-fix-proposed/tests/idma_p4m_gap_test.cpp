#include "Vidma_p4m_gap_top.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

static void ck(bool b, const char* s) {
    if (!b) throw std::runtime_error(s);
}

struct P4MBench {
    Vidma_p4m_gap_top d;
    uint64_t cycle = 0;
    bool differential = false;
    bool batch_mode = false;

    bool prod_valid = false, prod_write = false;
    uint8_t prod_addr = 0;
    uint64_t prod_wdata = 0, prod_wstrb = 0xff;
    bool prod_stall = false;
    uint64_t prod_due = 0;
    std::vector<uint64_t> prod_src;
    unsigned prod_completions = 0;
    uint64_t prod_last_done = 0;
    uint64_t prod_last_done_id = 0;
    bool prod_have_done = false;
    std::vector<uint64_t> prod_gaps;
    std::vector<uint64_t> prod_issue_ids;
    bool prod_issue_prev = false;

    bool gold_valid = false, gold_write = false;
    uint8_t gold_addr = 0;
    uint64_t gold_wdata = 0;
    bool gold_stall = false;
    uint64_t gold_due = 0;
    std::vector<uint64_t> gold_src;
    unsigned gold_completions = 0;
    uint64_t gold_last_done_id = 0;
    std::vector<uint64_t> gold_issue_ids;
    bool gold_issue_prev = false;

    // Controlled-launch inputs, driven identically into both frontends.
    bool launch_hold = false, launch_go = false, launch_cancel = false;

    unsigned pages = 1;

    void set_stall(bool s) {
        prod_stall = s;
        gold_stall = s;
    }

    void eval() {
        d.clk_i = 0;
        d.prod_valid_i = prod_valid;
        d.prod_write_i = prod_write;
        d.prod_addr_i = prod_addr;
        d.prod_wdata_i = prod_wdata;
        d.prod_wstrb_i = prod_wstrb;
        d.prod_ready_i = !prod_stall;
        d.prod_complete_i = prod_due && cycle == prod_due;

        d.prod_launch_hold_i = launch_hold;
        d.prod_launch_go_i = launch_go;
        d.prod_launch_cancel_i = launch_cancel;

        d.gold_valid_i = gold_valid;
        d.gold_write_i = gold_write;
        d.gold_addr_i = gold_addr;
        d.gold_wdata_i = gold_wdata;
        d.gold_ready_i = !gold_stall;
        d.gold_complete_i = gold_due && cycle == gold_due;
        d.gold_launch_hold_i = launch_hold;
        d.gold_launch_go_i = launch_go;
        d.gold_launch_cancel_i = launch_cancel;
        d.eval();
    }

    void check_differential() {
        ck(d.prod_request_o == d.gold_request_o, "differential request");
        ck(d.prod_length_o == d.gold_length_o, "differential length");
        ck(d.prod_type_o == d.gold_type_o, "differential type");
        ck(d.prod_src_o == d.gold_src_o, "differential src");
        ck(d.prod_dst_o == d.gold_dst_o, "differential dst");
        ck(d.prod_issue_o == d.gold_issue_o, "differential issue event");
        ck(d.prod_issue_id_o == d.gold_issue_id_o, "differential issue ID");
        ck(d.prod_done_o == d.gold_done_o, "differential done event");
        ck(d.prod_done_id_o == d.gold_done_id_o, "differential done ID");
        ck(d.prod_batch_active_o == d.gold_batch_active_o, "differential batch active");
        ck(d.prod_rdata_o == d.gold_rdata_o, "differential rdata");
        ck(d.prod_ready_o == d.gold_ready_o, "differential ready");
        ck(d.prod_error_o == d.gold_error_o, "differential error");
    }

    void tick() {
        eval();
        if (differential) check_differential();

        if (d.rst_ni) {
            bool prod_now = d.prod_issue_o;
            if (prod_now && !prod_issue_prev) prod_issue_ids.push_back(d.prod_issue_id_o);
            prod_issue_prev = prod_now;
            bool gold_now = d.gold_issue_o;
            if (gold_now && !gold_issue_prev) gold_issue_ids.push_back(d.gold_issue_id_o);
            gold_issue_prev = gold_now;
        }

        if (d.rst_ni && d.prod_request_o && d.prod_ready_i) {
            ck(!prod_due, "one production task outstanding");
            ck(d.prod_length_o == 4096 && d.prod_type_o == 0, "DEFAULT 4K copies");
            if (prod_have_done) {
                prod_gaps.push_back(cycle - prod_last_done - 1);
            }
            prod_src.push_back(d.prod_src_o);
            if (batch_mode) {
                // The batch frontend reports a 1-based issued-job counter and
                // raises the issue event on the backend accept.
                ck(d.prod_issue_o && d.prod_issue_id_o == prod_src.size(),
                   "per-task production issue event");
            }
            prod_due = cycle + 7;
        }
        if (d.rst_ni && d.gold_request_o && d.gold_ready_i) {
            ck(!gold_due, "one golden task outstanding");
            gold_src.push_back(d.gold_src_o);
            gold_due = cycle + 7;
        }
        if (d.prod_complete_i) {
            ck(d.prod_done_o, "per-task production retirement event");
            if (batch_mode) {
                ck(d.prod_done_id_o == prod_completions + 1, "batch retirement ID");
            } else {
                // Ordinary jobs report the raw ID generator value on the MMIO
                // path: the first completed job is reported as raw ID 2.
                ck(d.prod_done_id_o == prod_completions + 2, "legacy raw retirement ID");
            }
            prod_completions++;
            prod_last_done = cycle;
            prod_last_done_id = d.prod_done_id_o;
            prod_have_done = true;
            prod_due = 0;
        }
        if (d.gold_complete_i) {
            gold_completions++;
            gold_last_done_id = d.gold_done_id_o;
            gold_due = 0;
        }

        if (differential) {
            ck(prod_due == gold_due, "differential completion scheduling");
        }

        d.clk_i = 1; d.eval();
        cycle++;
        eval();
    }

    P4MBench() {
        d.rst_ni = 0;
        prod_valid = gold_valid = 0;
        for (int i = 0; i < 2; i++) {
            eval();
            d.clk_i = 1; d.eval();
            d.clk_i = 0; d.eval();
        }
        d.rst_ni = 1;
        eval();
        d.clk_i = 1; d.eval();
        d.clk_i = 0; d.eval();
    }

    uint64_t access(unsigned addr, bool wr = false, uint64_t val = 0, uint8_t strb = 0xff) {
        prod_valid = 1;
        prod_write = wr;
        prod_addr = addr;
        prod_wdata = val;
        prod_wstrb = strb;
        eval();
        ck(d.prod_ready_o, "MMIO ready");
        uint64_t r = d.prod_rdata_o;
        tick();
        prod_valid = 0;
        prod_wstrb = 0xff;
        eval();
        return r;
    }

    uint64_t legacy_access(unsigned addr, bool wr = false, uint64_t val = 0) {
        prod_valid = 1; gold_valid = 1;
        prod_write = wr; gold_write = wr;
        prod_addr = addr; gold_addr = addr;
        prod_wdata = val; gold_wdata = val;
        prod_wstrb = 0xff;
        eval();
        ck(d.prod_ready_o && d.gold_ready_o, "legacy MMIO ready");
        ck(d.prod_rdata_o == d.gold_rdata_o, "legacy read data");
        ck(d.prod_error_o == d.gold_error_o, "legacy error");
        if (!wr && addr == 0x28 && !batch_mode) {
            // Ordinary (non-batch) jobs raise the raw issue event on the MMIO ID
            // reservation, not on the later backend accept.
            ck(d.prod_issue_o && d.gold_issue_o, "legacy issue event on reservation");
        }
        uint64_t r = d.prod_rdata_o;
        tick();
        prod_valid = 0; gold_valid = 0;
        eval();
        return r;
    }

    void configure(unsigned n, unsigned w, uint32_t gap) {
        pages = w;
        batch_mode = true;
        access(0x00, 1, 0x10000);
        access(0x08, 1, 0x100000);
        access(0x10, 1, 4096);
        access(0x18, 1, 0);
        access(0x88, 1, 0);
        access(0x90, 1, n);
        access(0x98, 1, w);
        access(0xd0, 1, gap);
        ck(access(0xd8) == 0x49444d4147415001ULL, "gap capability magic");
        ck(access(0xd0) == gap, "gap readback");
        access(0x88, 1, 1);
    }

    // Batch configuration for the pinned-gold comparison.  The new d0/d8 gap
    // slots are deliberately never touched: they do not exist in the pinned
    // prechange frontend, so this is the only excluded traffic.
    void configure_legacy_batch(unsigned n, unsigned w) {
        pages = w;
        batch_mode = true;
        legacy_access(0x00, 1, 0x10000);
        legacy_access(0x08, 1, 0x100000);
        legacy_access(0x10, 1, 4096);
        legacy_access(0x18, 1, 0);
        legacy_access(0x88, 1, 0);
        legacy_access(0x90, 1, n);
        legacy_access(0x98, 1, w);
        legacy_access(0x88, 1, 1);
    }

    void start() {
        uint64_t id = access(0x28);
        ck(id == 2, "one functional ID per batch");
        tick();
        ck(prod_src.size() == 1, "first job offered without gap");
    }

    void drain() {
        for (unsigned i = 0; i < 200000 && (d.prod_batch_active_o || prod_due); i++) {
            tick();
        }
        ck(!d.prod_batch_active_o && !prod_due, "batch eventually idle");
    }
};

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    try {
        unsigned cases = 0;

        for (uint32_t gap : {0u, 1u, 7u, 1000u}) {
            P4MBench b;
            b.configure(4, 1, gap);
            b.start();
            for (unsigned i = 0; i < 20000 && b.prod_completions < 4; i++) b.tick();
            ck(b.prod_completions == 4, "gap sweep bounded completion");
            ck(b.prod_src.size() == 4, "exact finite count");
            ck(b.prod_completions == 4, "exact completions");
            ck(b.prod_gaps.size() == 3, "three inter-job gaps");
            for (uint64_t g : b.prod_gaps) ck(g == gap, "exact extra gap cycles");
            ck(!b.d.prod_batch_active_o, "idle after batch");
            cases++;
        }

        {
            P4MBench b;
            b.configure(4, 1, 1000);
            b.start();
            ck(b.prod_src.size() == 1, "firstjob no gap");
            for (unsigned i = 0; i < 20000 && b.prod_completions < 4; i++) b.tick();
            ck(b.prod_completions == 4, "firstjob bounded completion");
            ck(b.prod_gaps.size() == 3, "firstjob then gaps");
            for (uint64_t g : b.prod_gaps) ck(g == 1000, "firstjob gap exact");
            cases++;
        }

        {
            P4MBench b;
            b.configure(1, 1, 1000);
            b.start();
            for (unsigned i = 0; i < 20000 && b.prod_completions < 1; i++) b.tick();
            ck(b.prod_completions == 1, "target1 bounded completion");
            ck(b.prod_gaps.empty(), "target1 no next offer");
            ck(!b.d.prod_batch_active_o, "target1 no trailing gap");
            ck(b.prod_completions == 1, "target1 one completion");
            cases++;
        }

        {
            P4MBench b;
            b.configure(8, 1, 20);
            b.start();
            for (unsigned i = 0; i < 2000 && b.prod_completions < 1; i++) b.tick();
            ck(b.prod_completions == 1, "GAP bounded completion");
            ck(b.d.prod_batch_active_o, "GAP active");
            ck(b.d.prod_launch_active_o, "GAP keeps launch active");
            uint64_t done_id = b.prod_last_done_id;
            b.access(0x88, 1, 2);
            b.tick();
            ck(!b.d.prod_batch_active_o, "STOP in GAP returns IDLE");
            ck(!b.d.prod_launch_active_o, "STOP in GAP clears launch active");
            ck(b.prod_src.size() == 1, "STOP in GAP no extra offer");
            ck(b.prod_completions == 1, "STOP in GAP no extra done");
            ck(b.prod_last_done_id == done_id, "STOP in GAP raw done ID unchanged");
            ck((b.access(0x88) & 7) == 5, "STOP in GAP stopped status");
            cases++;
        }

        {
            P4MBench b;
            b.configure(8, 1, 1);
            b.start();
            for (unsigned i = 0; i < 2000 && b.prod_completions < 1; i++) b.tick();
            ck(b.prod_completions == 1, "GAP expiry bounded completion");
            ck(b.d.prod_batch_active_o, "GAP expiry active");
            ck(b.d.prod_launch_active_o, "GAP expiry keeps launch active");
            uint64_t done_id = b.prod_last_done_id;
            b.access(0x88, 1, 2);
            b.tick();
            ck(!b.d.prod_batch_active_o, "STOP expiry returns IDLE");
            ck(!b.d.prod_launch_active_o, "STOP expiry clears launch active");
            ck(b.prod_src.size() == 1, "STOP expiry no extra offer");
            ck(b.prod_completions == 1, "STOP expiry no extra done");
            ck(b.prod_last_done_id == done_id, "STOP expiry raw done ID unchanged");
            cases++;
        }

        {
            P4MBench b;
            b.configure(64, 8, 0);
            // Batch begin requires ready_i high, so NEXT_ID is reserved first and
            // only then is the backend backpressured; the stall legally blocks
            // the first offer.
            uint64_t id = b.access(0x28);
            ck(id == 2, "stalled batch functional ID");
            b.set_stall(true);
            b.eval();
            ck(b.d.prod_request_o, "stalled request valid");
            uint64_t src0 = b.d.prod_src_o;
            uint64_t dst0 = b.d.prod_dst_o;
            uint64_t len0 = b.d.prod_length_o;
            b.access(0x88, 1, 2);
            for (int i = 0; i < 8; i++) {
                b.eval();
                ck(b.d.prod_request_o, "STOP cannot withdraw stalled valid");
                ck(b.d.prod_src_o == src0 && b.d.prod_dst_o == dst0 &&
                   b.d.prod_length_o == len0, "STOP cannot change stalled payload");
                b.tick();
            }
            b.set_stall(false);
            b.drain();
            ck(b.prod_src.size() == 1 && b.prod_completions == 1,
               "stalled STOP safe completion");
            cases++;
        }

        {
            P4MBench b;
            b.access(0x88, 1, 0);
            b.access(0xd0, 1, 0x1234);
            ck(b.access(0xd0) == 0x1234, "gap programmed");
            b.access(0xd0, 1, 0x100000000ULL);
            ck((b.access(0x88) >> 32) != 0, "upper gap bits rejected");
            b.access(0x88, 1, 0);
            ck((b.access(0x88) >> 32) == 0, "disable clears error");
            b.access(0xd0, 1, 0x55);
            b.access(0xd0, 1, 0x66, 0x0f);
            ck((b.access(0x88) >> 32) != 0, "partial strobe rejected");
            ck(b.access(0xd0) == 0x55, "gap unchanged on bad strobe");
            b.access(0x88, 1, 0);
            b.configure(8, 1, 0x77);
            b.start();
            b.access(0xd0, 1, 0x88);
            ck((b.access(0x88) >> 32) != 0, "active gap write rejected");
            ck(b.access(0xd0) == 0x77, "gap unchanged while active");
            b.drain();
            b.access(0x88, 1, 0);
            b.access(0xd8, 1, 0);
            ck((b.access(0x88) >> 32) != 0, "d8 write rejected");
            cases++;
        }

        {
            P4MBench b;
            b.access(0x88, 1, 0);
            b.access(0xd0, 1, 0x1234);
            ck(b.access(0xd0) == 0x1234, "gap set");
            b.access(0x88, 1, 0);
            ck(b.access(0xd0) == 0, "disable zeroes gap");
            cases++;
        }

        {
            P4MBench b;
            b.prod_valid = 1; b.prod_write = 1; b.prod_addr = 0xb0;
            b.prod_wdata = 0; b.prod_wstrb = 0xff;
            b.eval();
            ck(b.d.prod_cfg_valid_o, "launcher b0 forwarded");
            ck(b.d.prod_cfg_addr_o == 0xb0, "launcher b0 address");
            b.tick();
            b.prod_valid = 0; b.eval();

            b.prod_valid = 1; b.prod_write = 1; b.prod_addr = 0xd0;
            b.prod_wdata = 1; b.prod_wstrb = 0xff;
            b.eval();
            ck(!b.d.prod_cfg_valid_o, "gap d0 not forwarded to launcher");
            b.tick();
            b.prod_valid = 0; b.eval();
            cases++;
        }

        {
            // Controlled active gap write: while a controlled launch job is
            // held, a write to the d0 gap slot must be rejected (AXI error)
            // without poisoning batch_error (bit 32 of the 0x88 status word).
            P4MBench b;
            b.access(0x00, 1, 0x10000);
            b.access(0x08, 1, 0x100000);
            b.access(0x10, 1, 64);
            b.access(0x18, 1, 0);
            ck((b.access(0x88) & 1) == 0, "launch preflight requires batch disabled");
            ck((b.access(0x88) >> 32) == 0, "no batch error before launch hold");

            b.launch_hold = true;
            b.prod_valid = 1; b.prod_write = 0; b.prod_addr = 0x28;
            b.prod_wdata = 0; b.prod_wstrb = 0xff;
            b.eval();
            ck(b.d.prod_ready_o && !b.d.prod_error_o, "launch prepare accepted");
            b.tick();
            b.prod_valid = 0; b.eval();
            b.launch_hold = false;
            ck(b.d.prod_launch_held_o && b.d.prod_launch_active_o, "launch hold entered");

            b.prod_valid = 1; b.prod_write = 1; b.prod_addr = 0xd0;
            b.prod_wdata = 0x1234; b.prod_wstrb = 0xff;
            b.eval();
            ck(b.d.prod_ready_o && b.d.prod_error_o, "held gap write rejected");
            ck(b.d.prod_launch_error_o, "held gap write flagged as launch error");
            b.tick();
            b.prod_valid = 0; b.eval();
            ck((b.access(0x88) >> 32) == 0, "held gap write does not poison batch_error");
            ck(b.access(0xd0) == 0, "held gap write did not change gap_cycles");

            b.launch_cancel = true;
            b.tick();
            b.launch_cancel = false;
            b.eval();
            ck(!b.d.prod_launch_held_o && !b.d.prod_launch_active_o,
               "launch cancel returns to IDLE");
            cases++;
        }

        // Ordinary (non-batch) legacy differential: corrected raw ID semantics -
        // the issue event fires on the MMIO ID reservation and the first
        // completed job is reported as raw ID 2, incrementing per job.
        {
            P4MBench b;
            b.differential = true;
            b.legacy_access(0x00, 1, 0x10000);
            b.legacy_access(0x08, 1, 0x100000);
            b.legacy_access(0x10, 1, 4096);
            b.legacy_access(0x18, 1, 0);
            for (int t = 0; t < 3; t++) {
                uint64_t id = b.legacy_access(0x28);
                ck(id == 2 + (uint64_t)t, "legacy raw reserved ID sequence");
                for (int i = 0; i < 200 && b.prod_completions <= (unsigned)t; i++) {
                    b.tick();
                }
                ck(b.prod_completions == (unsigned)t + 1, "legacy production completion");
                ck(b.gold_completions == (unsigned)t + 1, "legacy golden completion");
                ck(b.prod_last_done_id == b.gold_last_done_id, "legacy raw done ID match");
            }
            ck(b.prod_src.size() == 3 && b.gold_src.size() == 3, "legacy count");
            for (size_t i = 0; i < b.prod_src.size(); i++) {
                ck(b.prod_src[i] == b.gold_src[i], "legacy source sequence");
            }
            ck(b.prod_issue_ids.size() == 3 && b.gold_issue_ids.size() == 3,
               "legacy raw issue event count");
            for (size_t i = 0; i < b.prod_issue_ids.size(); i++) {
                ck(b.prod_issue_ids[i] == b.gold_issue_ids[i], "legacy raw issue ID match");
                if (i > 0) ck(b.prod_issue_ids[i] == b.prod_issue_ids[i - 1] + 1,
                              "legacy raw issue IDs increment");
            }
            cases++;
        }

        // Cycle-for-cycle legacy BATCH gap0 comparison against the pinned
        // prechange frontend: both BatchPort/LaunchPort 1, identical configs,
        // identical backend stalls and completion timing.  Only the new d0/d8
        // gap slots are excluded (they do not exist in the pinned golden).
        for (unsigned w : {1u, 8u}) {
            P4MBench b;
            b.differential = true;
            b.configure_legacy_batch(4, w);
            uint64_t id = b.legacy_access(0x28);
            ck(id == 2, "differential batch functional ID");
            b.legacy_access(0x88);
            b.legacy_access(0xa0);
            b.legacy_access(0xa8);
            for (unsigned i = 0; i < 4000 && b.prod_completions < 4; i++) b.tick();
            ck(b.prod_completions == 4 && b.gold_completions == 4,
               "differential batch completions");
            ck(b.prod_src.size() == 4 && b.gold_src.size() == 4,
               "differential batch job count");
            for (size_t i = 0; i < b.prod_src.size(); i++) {
                ck(b.prod_src[i] == b.gold_src[i], "differential batch source");
            }
            ck(b.prod_issue_ids.size() == 4 && b.gold_issue_ids.size() == 4,
               "differential batch issue events");
            ck(b.prod_gaps.size() == 3, "differential batch gap0 count");
            for (uint64_t g : b.prod_gaps) ck(g == 0, "differential batch gap0 exact");
            ck(!b.d.prod_batch_active_o && !b.d.gold_batch_active_o,
               "differential batch idle");
            cases++;
        }

        // Same comparison with the offer backpressured in COUNT and STOP issued
        // while the stalled offer must stay valid and unchanged.
        {
            P4MBench b;
            b.differential = true;
            b.configure_legacy_batch(64, 8);
            uint64_t id = b.legacy_access(0x28);
            ck(id == 2, "differential stalled batch functional ID");
            b.set_stall(true);
            b.eval();
            ck(b.d.prod_request_o && b.d.gold_request_o, "differential stalled offer");
            uint64_t src0 = b.d.prod_src_o, dst0 = b.d.prod_dst_o, len0 = b.d.prod_length_o;
            for (int i = 0; i < 4; i++) {
                ck(b.d.prod_src_o == src0 && b.d.prod_dst_o == dst0 &&
                   b.d.prod_length_o == len0, "differential stalled payload stable");
                b.tick();
            }
            b.legacy_access(0x88, 1, 2);
            for (int i = 0; i < 8; i++) {
                ck(b.d.prod_request_o, "differential STOP cannot withdraw stalled valid");
                ck(b.d.prod_src_o == src0 && b.d.prod_dst_o == dst0 &&
                   b.d.prod_length_o == len0, "differential STOP stalled payload stable");
                b.tick();
            }
            b.set_stall(false);
            b.drain();
            for (unsigned i = 0; i < 200 && (b.prod_completions < 1 || b.gold_completions < 1); i++) {
                b.tick();
            }
            ck(b.prod_src.size() == 1 && b.gold_src.size() == 1,
               "differential stalled STOP single job");
            ck(b.prod_completions == 1 && b.gold_completions == 1,
               "differential stalled STOP single completion");
            ck(!b.d.prod_batch_active_o && !b.d.gold_batch_active_o,
               "differential stalled STOP idle");
            cases++;
        }

        std::cout << "PASS " << cases << " P4M gap scenarios\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FAIL " << e.what() << "\n";
        return 1;
    }
}
