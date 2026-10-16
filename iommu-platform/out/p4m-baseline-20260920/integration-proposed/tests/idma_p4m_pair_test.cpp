// SPDX-License-Identifier: Solderpad-0.51
// Bounded P4M frontend/observer integration test driven by an independent,
// per-cycle event oracle.
//
// The synthetic backends (6 cycles on V, 9 cycles on A) prove wiring and the
// contract's edge accounting only.  They are not an AXI timing model, and no
// datapath or continuous-performance claim is made anywhere in this file.
//
// The oracle consumes only the exported frontend event taps (issue/done plus
// their IDs) and rebuilds the 16 P4M bank words.  It never inspects, mirrors or
// re-implements the observer's internal phase machine or counters.
#include "Vidma_p4m_pair_top.h"
#include "verilated.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

constexpr uint64_t BANK_MAGIC   = 0x49444d4150344d01ULL;
constexpr uint64_t OBS_MAGIC_V7 = 0x49444d414f425307ULL;
constexpr uint64_t GAP_MAGIC    = 0x49444d4147415001ULL;
// The A batch is deliberately far longer than any V window under test so STOP
// always has to retire a still-running (typically GAP-resident) A batch.
constexpr unsigned A_TARGET     = 4096;

void fail(const std::string &m) { throw std::runtime_error(m); }
void ck(bool b, const char *m) { if (!b) fail(m); }
void ck_eq(uint64_t got, uint64_t want, const char *m)
{
    if (got != want)
        fail(std::string(m) + ": got " + std::to_string(got) +
             " want " + std::to_string(want));
}

// ---------------------------------------------------------------------------
// Independent oracle.
//   * the window opens on the actual V issue and closes on the Nth V done;
//   * A issue/done sums cover (open, close] (opening edge excluded, closing
//     edge included) and the A stock is sampled after each edge's events;
//   * "V issued"/"V done" include the initial V issue/done of the batch, so in
//     the standard flow word13 == word14 == the batch length;
//   * the strict pre-opening cohort is cleared when the carried job's own done
//     event is counted.
// ---------------------------------------------------------------------------
struct Oracle {
    unsigned target = 1;
    bool has_a = false;
    bool armed = false, open = false, closed = false;
    long cyc = 0, t_open = -1, t_close = -1;

    bool a_task = false;          // A lane task in flight, tracked from reset
    uint64_t a_id = 0;            // A lane current task ID
    uint64_t a_issue = 0, a_done = 0;
    bool have_first = false;
    uint64_t first_done_id = 0, last_done_id = 0;
    uint64_t open_stock = 0, open_id = 0, close_stock = 0, close_id = 0;
    bool open_issue = false, carry = false, carry_done = false;
    uint64_t v_issued = 0, v_done = 0;

    std::vector<uint64_t> a_issue_ids, a_done_ids;

    void arm(unsigned n, bool with_a) { target = n; has_a = with_a; armed = true; }

    void step(bool v_iss, uint64_t v_iid, bool v_dn, uint64_t v_did,
              bool a_iss, uint64_t a_iid, bool a_dn, uint64_t a_did)
    {
        ++cyc;
        (void)v_iid; (void)v_did;
        const bool a_issue_evt = has_a && a_iss;
        const bool a_done_evt  = has_a && a_dn;
        // The observer compares the carried job's done ID against the registered
        // current ID, so capture the pre-update value.
        const uint64_t a_id_before = a_id;

        // Continuous per-lane tracking: retire first, so a same-cycle DONE plus
        // new ISSUE is a legal hand-off rather than an overlap.
        if (a_done_evt) a_task = false;
        if (a_issue_evt) { a_task = true; a_id = a_iid; }

        // Window accumulation over (open, close].  "open" is still false on the
        // opening cycle (its events are excluded) and "closed" is only raised
        // after this block (so the closing cycle's events are included).
        if (armed && open && !closed) {
            if (a_issue_evt) { ++a_issue; a_issue_ids.push_back(a_iid); }
            if (a_done_evt) {
                if (!have_first) { first_done_id = a_did; have_first = true; }
                last_done_id = a_did;
                a_done_ids.push_back(a_did);
                ++a_done;
                if (carry && a_did == a_id_before) { carry_done = true; carry = false; }
            }
        }

        // V activity of the observed batch, initial V issue included.
        if (armed && !closed) {
            if (v_iss) ++v_issued;
            if (v_dn)  ++v_done;
        }

        // Opening edge: the actual V issue.
        if (armed && !open && v_iss) {
            open = true; t_open = cyc;
            open_stock = a_task ? 1 : 0;
            open_id    = a_task ? a_id : 0;
            open_issue = a_issue_evt;
            carry      = a_task && !a_issue_evt;
            carry_done = false;
        }

        // Closing edge: the Nth V done; that cycle stays inside the window.
        if (open && !closed && v_dn && v_done == target) {
            closed = true; t_close = cyc;
            close_stock = a_task ? 1 : 0;
            close_id    = a_task ? a_id : 0;
        }
    }

    std::array<uint64_t, 16> expected() const
    {
        std::array<uint64_t, 16> e{};
        e[0]  = BANK_MAGIC;
        e[1]  = (open && closed) ? uint64_t(t_close - t_open) : 0;
        e[2]  = a_issue;
        e[3]  = a_done;
        e[4]  = open_stock;
        e[5]  = close_stock;
        e[6]  = carry_done ? 1 : 0;
        e[7]  = open_issue ? 1 : 0;
        e[8]  = open_id;
        e[9]  = close_id;
        e[10] = have_first ? first_done_id : 0;
        e[11] = have_first ? last_done_id : 0;
        e[12] = 0;                                    // no P4M violation expected
        e[13] = v_issued;
        e[14] = v_done;
        e[15] = (has_a ? 1ULL : 0ULL) | (closed ? 2ULL : 0ULL) | (carry ? 4ULL : 0ULL);
        return e;
    }
};

void check_bank(const std::array<uint64_t,16> &bank,
                const std::array<uint64_t,16> &exp, const std::string &ctx)
{
    static const char *names[16] = {
        "magic","cycles","A_issue","A_done","A_start","A_end","carry_done",
        "open_issue","open_id","close_id","first_done_id","last_done_id",
        "error","V_issued","V_done","flags"};
    for (unsigned i = 0; i < 16; ++i)
        if (bank[i] != exp[i])
            fail(ctx + " bank word " + std::to_string(i) + " (" + names[i] +
                 ") got " + std::to_string(bank[i]) +
                 " want " + std::to_string(exp[i]));
}

void run_case(bool solo, unsigned a_pages, unsigned n, unsigned gap,
              unsigned lead, bool warmup)
{
    const std::string ctx =
        std::string("case{solo=") + (solo ? "1" : "0") +
        ",a_pages=" + std::to_string(a_pages) +
        ",n=" + std::to_string(n) +
        ",gap=" + std::to_string(gap) +
        ",lead=" + std::to_string(lead) +
        ",warmup=" + (warmup ? "1" : "0") + "}";

    Vidma_p4m_pair_top d;
    Oracle o;

    d.clk_i = 0; d.rst_ni = 0; d.valid_i = 0; d.write_i = 0;
    d.port_i = 0; d.addr_i = 0; d.wdata_i = 0;

    // Sampled once per clock cycle, immediately after the posedge has settled,
    // so the oracle sees exactly the combinational event values that the
    // observer registers at the NEXT posedge.
    auto sample = [&]() {
        o.step((d.issue_o & 0x1) != 0, d.issue_id_o[0],
               (d.done_o  & 0x1) != 0, d.done_id_o[0],
               (d.issue_o & 0x2) != 0, d.issue_id_o[1],
               (d.done_o  & 0x2) != 0, d.done_id_o[1]);
    };
    auto tick = [&]() { d.clk_i = 1; d.eval(); d.clk_i = 0; d.eval(); sample(); };
    auto access = [&](unsigned port, unsigned addr, bool wr, uint64_t val) -> uint64_t {
        d.port_i = port; d.addr_i = (uint8_t)addr; d.write_i = wr;
        d.wdata_i = val; d.valid_i = 1; d.eval();
        ck(d.ready_o != 0, (ctx + " register access not ready").c_str());
        ck(d.error_o == 0, (ctx + " register access reported an error").c_str());
        const uint64_t r = d.rdata_o;
        tick();
        d.valid_i = 0; d.eval();
        return r;
    };
    auto drain_a = [&]() {
        unsigned t = 0;
        while (((d.running_o >> 1) & 1) && ++t < 20000) tick();
        ck(t < 20000, (ctx + " A lane did not drain").c_str());
    };

    d.rst_ni = 0; tick();
    d.rst_ni = 1; tick();

    // Optional ordinary (non-batch) single copy on the A lane BEFORE ARM, to
    // move the functional IDs and prove that tracking is not reset by ARM.
    if (warmup) {
        access(1, 0x00, true, 0x10000);
        access(1, 0x08, true, 0x20000);
        access(1, 0x10, true, 4096);
        access(1, 0x18, true, 0);
        const uint64_t wid = access(1, 0x28, false, 0);
        ck(wid != 0, (ctx + " A warm-up copy returned ID 0").c_str());
        unsigned t = 0;
        while (access(1, 0x30, false, 0) != wid && ++t < 4000) {}
        ck(t < 4000, (ctx + " A warm-up copy did not retire").c_str());
        ck(((d.running_o >> 1) & 1) == 0, (ctx + " A lane busy after warm-up").c_str());
    }

    // Batch configuration: V is lane 0 with gap forced to zero, A is lane 1
    // with the requested gap and page set.
    for (unsigned p = 0; p < 2; ++p) {
        if (solo && p == 1) break;
        access(p, 0x00, true, 0x10000);
        access(p, 0x08, true, 0x100000);
        access(p, 0x10, true, 4096);
        access(p, 0x18, true, 0);
        access(p, 0x90, true, p == 0 ? n : A_TARGET);
        access(p, 0x98, true, p == 0 ? 1u : a_pages);
        access(p, 0xd0, true, p == 0 ? 0u : gap);
        access(p, 0x88, true, 1);
    }

    // The 0xd0 slot is serviced inside the BatchPort frontend (never forwarded
    // to the observer); 0xd8 is its read-only capability.  Read both back before
    // any launch, as the driver contract requires.
    ck_eq(access(0, 0xd8, false, 0), GAP_MAGIC, (ctx + " gap capability V").c_str());
    if (!solo)
        ck_eq(access(1, 0xd8, false, 0), GAP_MAGIC, (ctx + " gap capability A").c_str());
    ck_eq(access(0, 0xd0, false, 0), 0, (ctx + " V gap readback must be zero").c_str());
    if (!solo)
        ck_eq(access(1, 0xd0, false, 0), gap, (ctx + " A gap readback").c_str());
    ck_eq(access(0, 0x58, false, 0), OBS_MAGIC_V7, (ctx + " observer v7 magic").c_str());

    // ARM before starting either lane.  V is port 0, A is port 1; path bit26 is
    // left clear because PathEnable=0 here.
    access(0, 0x68, true, n);
    const uint64_t arm = 1ULL | (1ULL << 30) | (1ULL << 25) |
                         (solo ? 0ULL : (1ULL << 24)) | (1ULL << 16);
    o.arm(n, !solo);
    access(0, 0x60, true, arm);

    uint64_t a_batch_id = 0;
    if (!solo) {
        a_batch_id = access(1, 0x28, false, 0);
        ck(a_batch_id != 0, (ctx + " A batch ID must be non-zero").c_str());
        for (unsigned i = 0; i < lead; ++i) tick();
    }

    const uint64_t v_batch_id = access(0, 0x28, false, 0);
    ck(v_batch_id != 0, (ctx + " V batch ID must be non-zero").c_str());

    // Poll the raw V DONE functional ID until the batch retires.
    unsigned tries = 0;
    while (access(0, 0x30, false, 0) != v_batch_id && ++tries < 40000) {}
    ck(tries < 40000, (ctx + " V batch did not retire").c_str());

    // Reads must be frozen by that poll, and P4M must not raise the main
    // observer status error bit (error_q[8] = status bit 40).
    const uint64_t obs_status = access(0, 0x60, false, 0);
    ck(((obs_status >> 2) & 1) != 0, (ctx + " observer did not freeze after the V CPU poll").c_str());
    ck(((obs_status >> 40) & 1) == 0, (ctx + " observer main status error bit is set").c_str());

    // STOP the A batch and drain it, then prove that the RAW functional A DONE
    // ID retires even when STOP lands in GAP (not a derived observer value).
    if (!solo) {
        access(1, 0x88, true, 2);
        drain_a();
        ck_eq(access(1, 0x30, false, 0), a_batch_id,
              (ctx + " raw A DONE functional batch ID must retire after STOP").c_str());
    }

    // Frozen window snapshots: read the bank twice and require bit-identical
    // results after the V poll and the A drain.
    std::array<uint64_t, 16> bank{}, bank_again{};
    for (unsigned i = 0; i < 16; ++i) { access(0, 0x70, true, 448u + i); bank[i] = access(0, 0x78, false, 0); }
    for (unsigned i = 0; i < 16; ++i) { access(0, 0x70, true, 448u + i); bank_again[i] = access(0, 0x78, false, 0); }
    ck(bank == bank_again, (ctx + " P4M bank moved after the V poll and A drain").c_str());

    check_bank(bank, o.expected(), ctx);

    // The 35 legacy words are validated with conservation and bucket-sum
    // invariants that never require modelling observer-internal state.
    std::array<uint64_t, 35> leg{};
    for (unsigned i = 0; i < 35; ++i) { access(0, 0x70, true, i); leg[i] = access(0, 0x78, false, 0); }
    ck_eq(leg[0], bank[1], (ctx + " legacy count0 must equal the P4M cycle count").c_str());
    ck_eq(leg[10] + leg[11] + leg[12] + leg[13], leg[0], (ctx + " legacy pending buckets").c_str());
    ck_eq(leg[8], bank[14], (ctx + " legacy V done count must match the P4M V done count").c_str());
    ck_eq(leg[9], 1, (ctx + " legacy poll-found count").c_str());
    for (unsigned lane = 0; lane < 2; ++lane) {
        ck_eq((leg[30 + lane] >> 16) + leg[14 + lane * 6],
              (leg[32 + lane] >> 16) + leg[16 + lane * 6],
              (ctx + " read conservation across the opening edge").c_str());
        ck_eq((leg[30 + lane] & 0xffff) + leg[15 + lane * 6],
              (leg[32 + lane] & 0xffff) + leg[17 + lane * 6],
              (ctx + " write conservation across the opening edge").c_str());
    }
    if (solo) {
        for (unsigned i = 20; i <= 25; ++i)
            ck_eq(leg[i], 0, (ctx + " solo lane-1 counters must stay zero").c_str());
        ck_eq(leg[31], 0, (ctx + " solo lane-1 opening stock").c_str());
        ck_eq(leg[33], 0, (ctx + " solo lane-1 closing stock").c_str());
    }
}

} // namespace

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);
    try {
        unsigned cases = 0;

        // Solo: V only, gap must be zero, A lane never started.
        for (unsigned n : {1u, 8u, 64u}) {
            run_case(true, 1, n, 0, 1, false);
            ++cases;
        }

        // Co-run: A page set 1 or 8, V length 1/8/64, A gap 0/1/7/1000 and a
        // 1..12 cycle start-up lead between A starting and the V issue.
        for (unsigned pages : {1u, 8u})
            for (unsigned n : {1u, 8u, 64u})
                for (unsigned gap : {0u, 1u, 7u, 1000u})
                    for (unsigned lead = 1; lead <= 12; ++lead) {
                        run_case(false, pages, n, gap, lead, false);
                        ++cases;
                    }

        // Warm-up variants: an ordinary copy on A before ARM.
        run_case(false, 1, 8, 1, 3, true);  ++cases;
        run_case(false, 8, 64, 0, 5, true); ++cases;

        std::cout << "PASS " << cases
                  << " P4M frontend/observer configurations (synthetic backend; "
                     "no AXI timing or datapath claims)\n";
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "FAIL " << e.what() << "\n";
        return 1;
    }
}
