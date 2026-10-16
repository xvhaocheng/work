// SPDX-License-Identifier: Solderpad-0.51
// Composition observer linkage regression.
//
// Exercises the production dma_observer (V8: P4MEnable/ResearchEnable/
// PathEnable = 1) wired to the real dma_composition_control behind the same
// absolute observer index bank used by ariane_peripherals_xilinx.sv for
// IOMMUNonblockMode == 3.  The DUT is driven only through the observer register
// interface and the real ARM/OPEN/CLOSE plumbing; no internal DUT register is
// read and no AXI bus event is fabricated.  Therefore this test makes no claim
// about AXI stock conservation, driver end-to-end validity, or any board result.
#include "Vcomposition_observer_top.h"
#include "verilated.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

static unsigned cases;

static void check(bool condition, const char *message)
{
    if (!condition)
        throw std::runtime_error(message);
}

// Independent ABI constants (never derived from the DUT).
static const uint64_t OBS_MAGIC_V8   = 0x49444d414f425308ULL;
static const uint64_t P4M_BANK_MAGIC = 0x49444d4150344d01ULL;
static const uint64_t COMP_MAGIC     = 0x49444d4143504e01ULL;
static const uint64_t COMP_ABI       = 0x1ULL;
static const uint64_t COMP_PROT_MASK = 0x3ULL;
static const uint32_t COMP_BASE      = 512u + 0xd20u; /* absolute obs index */

struct Bench {
    Vcomposition_observer_top d;

    Bench() { reset(); }

    void tick(unsigned n = 1)
    {
        while (n--) {
            d.clk_i = 0; d.eval();
            d.clk_i = 1; d.eval();
            d.clk_i = 0; d.eval();
        }
    }

    void reset()
    {
        d.cfg_valid_i = 0; d.cfg_write_i = 0; d.cfg_addr_i = 0;
        d.cfg_wdata_i = 0; d.cfg_wstrb_i = 0xff;
        d.issue_i = 0; d.done_i = 0; d.poll_i = 0; d.batch_active_i = 0;
        d.ar_valid_i = 0; d.aw_valid_i = 0; d.ar_ready_i = 0; d.aw_ready_i = 0;
        d.rlast_fire_i = 0; d.b_fire_i = 0; d.bad_id_i = 0;
        d.shared_ar_valid_i = 0; d.shared_aw_valid_i = 0;
        d.shared_ar_ready_i = 0; d.shared_aw_ready_i = 0;
        d.out_valid_i = 0; d.out_ready_i = 0; d.return_i = 0;
        d.e3_data_i = 0; d.e3_error_i = 0;
        d.lookup_i = 0; d.lookup_hit_i = 0; d.normal_i = 0;
        d.stage1_i = 0; d.stage2_i = 0;
        d.lookup_did_i = 0; d.lookup_pscid_i = 0;
        d.accept_i = 0; d.fast_i = 0; d.fast_did_i = 0; d.gap_active_i = 0;
        d.update_i = 0; d.invalidate_i = 0; d.update_did_i = 0;
        d.replace_mask_i = 0; d.plru_i = 0;
        d.entries_seed_i = 0; d.entries_next_seed_i = 0;
        d.composition_iommu_quiet_i = 1; d.supported_i = 1;
        d.job_quiet_i = 0xf; d.launch_active_i = 0; d.launch_held_i = 0;
        for (int i = 0; i < 4; i++)
            d.issue_id_i[i] = d.done_id_i[i] = d.poll_id_i[i] = 0;
        for (int i = 0; i < 2; i++)
            d.tr_id_i[i] = d.out_id_i[i] = d.return_id_i[i] = 0;

        d.rst_ni = 0; tick(2); d.rst_ni = 1; tick();
    }

    void obs_write(unsigned addr, uint64_t value)
    {
        d.cfg_valid_i = 1; d.cfg_write_i = 1; d.cfg_addr_i = addr;
        d.cfg_wdata_i = value; d.cfg_wstrb_i = 0xff;
        tick();
        d.cfg_valid_i = 0; d.cfg_write_i = 0; d.eval();
    }

    uint64_t obs_read(unsigned addr)
    {
        d.cfg_addr_i = addr; d.eval();
        return d.cfg_rdata_o;
    }

    uint64_t obs_indexed(unsigned index)
    {
        obs_write(0x70, index);
        return obs_read(0x78);
    }

    uint64_t comp_read(unsigned word)
    {
        obs_write(0x70, COMP_BASE + word);
        return obs_read(0x78);
    }

    // Program composition word 2 through the observer absolute bank.  Returns
    // whether the same-cycle protected clear pulse was observed.
    bool comp_program(uint32_t mode, uint32_t did)
    {
        obs_write(0x70, COMP_BASE + 2);
        uint64_t cfg = ((uint64_t)did << 8) | (uint64_t)mode;
        d.cfg_valid_i = 1; d.cfg_write_i = 1; d.cfg_addr_i = 0x78;
        d.cfg_wdata_i = cfg; d.cfg_wstrb_i = 0xff;
        d.eval();
        bool clear = d.composition_clear_o != 0;
        d.clk_i = 1; d.eval();
        d.clk_i = 0; d.eval();
        d.cfg_valid_i = 0; d.cfg_write_i = 0; d.eval();
        return clear;
    }

    uint64_t obs_status() { return obs_read(0x60); }

    uint64_t p4m_word(unsigned i)
    {
        obs_write(0x70, 448 + i);
        return obs_read(0x78);
    }

    // Solo P4M arm: bit30 (P4M), bit26 (path) and bit25 (batch), exactly the
    // command idma_observation_arm() builds for a P4M run.  PathEnable is
    // compiled into the DUT, so bit26 is legal here and must stay in the frame.
    void arm_p4m_solo(unsigned target)
    {
        obs_write(0x68, target);
        uint64_t command = 1ULL | (1ULL << 25) | (1ULL << 26) | (1ULL << 30);
        obs_write(0x60, command);
        check((obs_status() & 7ULL) == 1ULL, "P4M solo ARM accepted");
    }

    void issue(unsigned port, uint64_t id)
    { d.issue_i = 1u << port; d.issue_id_i[port] = id; tick(); d.issue_i = 0; }

    void done(unsigned port, uint64_t id)
    { d.done_i = 1u << port; d.done_id_i[port] = id; tick(); d.done_i = 0; }

    void poll(unsigned port, uint64_t id)
    { d.poll_i = 1u << port; d.poll_id_i[port] = id; tick(); d.poll_i = 0; }

    void pulse_lookup(uint32_t did, unsigned hit)
    {
        d.lookup_i = 1; d.lookup_did_i = did; d.lookup_hit_i = hit ? 1 : 0;
        d.normal_i = 1; d.stage1_i = 1; d.stage2_i = 0;
        tick();
        d.lookup_i = 0; d.normal_i = 0; d.stage1_i = 0;
    }

    void pulse_fast(uint32_t did, unsigned accept)
    {
        d.fast_i = 1; d.fast_did_i = did; d.accept_i = accept ? 1 : 0;
        tick();
        d.fast_i = 0; d.accept_i = 0;
    }

    void pulse_update(uint32_t did, uint16_t mask)
    {
        d.update_i = 1; d.update_did_i = did; d.replace_mask_i = mask;
        tick();
        d.update_i = 0; d.replace_mask_i = 0;
    }

    void pulse_gap(unsigned lane)
    {
        d.gap_active_i = (uint8_t)(1u << lane);
        tick();
        d.gap_active_i = 0;
    }
};

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);
    try {
        Bench b;

        // Case 1: V8 observer magic and P4M bank capability, PathEnable bank live.
        check(b.obs_read(0x58) == OBS_MAGIC_V8, "observer magic is V8");
        check(b.p4m_word(0) == P4M_BANK_MAGIC, "P4M bank capability magic");
        b.obs_write(0x70, 64);
        check(b.obs_read(0x78) == 0ULL,
              "path bank (index 64) is readable with PathEnable=1");
        cases++;

        // Case 2: composition magic/ABI and an accepted configure through the
        // absolute 512+D20+2 index, with the protected clear pulse.
        check(b.comp_read(0) == COMP_MAGIC, "composition magic via absolute index");
        check(b.comp_read(1) == COMP_ABI, "composition ABI version");
        check(b.comp_read(5) == 1ULL, "composition reports quiet before configure");
        check(b.comp_read(3) == 0ULL, "composition generation starts at zero");
        bool clear = b.comp_program(1, 10);
        check(clear, "accepted configure pulses the protected clear");
        check(b.comp_read(3) == 1ULL, "generation increments once");
        check(b.comp_read(2) == ((10ULL << 8) | 1ULL), "mode/did stored in word 2");
        check(b.comp_read(4) == 0ULL, "configure status is ok");
        cases++;

        // Case 3: the real ARM path (bit30|bit26|bit25, target 1) is accepted.
        b.arm_p4m_solo(1);
        check((b.obs_status() & 7ULL) == 1ULL, "observer reports ARMED");
        check((b.comp_read(6) & 1ULL) != 0ULL, "composition reports ARMED");
        cases++;

        // Case 4: V issue opens the composition window; pre-issue snapshot is
        // stable and the opening edge samples entries_i and plru_i.
        b.d.entries_seed_i      = 0x0000000000001000ULL;
        b.d.entries_next_seed_i = 0x0000000000002000ULL;
        b.d.plru_i              = 0x5a5a1234ULL;
        check((b.comp_read(6) & 3ULL) == 1ULL, "ARMED and not yet ACTIVE before issue");
        check(b.comp_read(8) == 0ULL, "zero window cycles before issue");
        b.issue(0, 1000);
        check((b.comp_read(6) & 2ULL) != 0ULL, "composition ACTIVE after V issue");
        check((b.comp_read(6) & 4ULL) == 0ULL, "composition not frozen during the window");
        check(b.comp_read(64) == (0x1000000000000000ULL + 0ULL),
              "entries_open[0] low from entries_i");
        check(b.comp_read(65) == (0x0000000000001000ULL + 0ULL),
              "entries_open[0] high from entries_i");
        check(b.comp_read(36) == 0x5a5a1234ULL, "plru_open captured at the opening edge");
        cases++;

        // Case 5: real lookup/hit/miss, fast, update and gap events in the window.
        b.pulse_lookup(10, 1);
        b.pulse_lookup(10, 0);
        b.pulse_lookup(11, 1);
        b.pulse_lookup(11, 0);
        b.pulse_lookup(99, 1);
        b.pulse_fast(10, 1);
        b.pulse_update(10, 1);
        b.pulse_gap(0);
        check(b.comp_read(12) == 2ULL && b.comp_read(13) == 1ULL && b.comp_read(14) == 1ULL,
              "V lookup/hit/miss counters");
        check(b.comp_read(15) == 2ULL && b.comp_read(16) == 1ULL && b.comp_read(17) == 1ULL,
              "A lookup/hit/miss counters");
        check(b.comp_read(18) == 1ULL && b.comp_read(19) == 1ULL && b.comp_read(20) == 0ULL,
              "O lookup/hit/miss counters");
        check(b.comp_read(21) == 1ULL, "fast V counter");
        check(b.comp_read(26) == 1ULL, "update fill V counter");
        check(b.comp_read(24) == 1ULL && b.comp_read(25) == 0ULL, "gap V/A counters");
        check(b.comp_read(29) == 0ULL && b.comp_read(30) == 0ULL &&
              b.comp_read(31) == 0ULL && b.comp_read(32) == 0ULL && b.comp_read(33) == 0ULL,
              "no repl/part/inval/first_bad/fast_n events");
        cases++;

        // Case 6: V done closes the window; entries_close samples entries_next,
        // then the final poll freezes the observer.
        b.d.plru_i = 0x0badf00dULL;
        b.done(0, 1000);
        check((b.comp_read(6) & 4ULL) != 0ULL, "composition FROZEN after V done");
        check(b.comp_read(96) == (0x2000000000000000ULL + 0ULL),
              "entries_close[0] low from entries_next");
        check(b.comp_read(97) == (0x0000000000002000ULL + 0ULL),
              "entries_close[0] high from entries_next");
        check(b.comp_read(37) == 0x0badf00dULL, "plru_close captured at the closing edge");
        b.poll(0, 1000);
        check((b.obs_status() & 7ULL) == 4ULL, "observer FROZEN after the final poll");
        check((b.obs_status() >> 32) == 0ULL, "no observer error at freeze");
        cases++;

        // Case 7: full 128-word readback and cross-consistency with the P4M bank.
        std::array<uint64_t, 128> raw;
        for (unsigned w = 0; w < 128; w++)
            raw[w] = b.comp_read(w);
        check(raw[0] == COMP_MAGIC && raw[1] == COMP_ABI, "raw magic/ABI");
        check(raw[2] == ((10ULL << 8) | 1ULL), "raw configured mode/did");
        check(raw[3] == 1ULL, "raw generation");
        check(raw[4] == 0ULL, "raw status zero");
        check(raw[5] == 1ULL, "raw quiet observation set");
        check(raw[6] == 4ULL, "raw state flags FROZEN only");
        check(raw[7] == 1ULL, "raw accepted sequence");
        check(raw[8] != 0ULL && raw[8] == b.p4m_word(1),
              "composition cycles equal the frozen P4M cycle count");
        check(raw[9] == 0ULL, "raw error register clean");
        check(raw[10] == ((10ULL << 8) | 1ULL) && raw[11] == 1ULL,
              "raw arm-time window config/generation");
        check(raw[12] == raw[13] + raw[14], "V lookup conservation");
        check(raw[15] == raw[16] + raw[17], "A lookup conservation");
        check(raw[18] == raw[19] + raw[20], "O lookup conservation");
        check(raw[24] <= raw[8], "gap bounded by cycles");
        check(raw[34] == COMP_PROT_MASK, "protected mask word");
        check(raw[35] == 0ULL, "reserved word 35 zero");
        for (unsigned w = 40; w < 64; w++)
            check(raw[w] == 0ULL, "reserved composition window words zero");
        for (unsigned i = 0; i < 16; i++) {
            check(raw[64 + 2 * i]     == (0x1000000000000000ULL + i), "entries_open low word");
            check(raw[64 + 2 * i + 1] == (0x0000000000001000ULL + i), "entries_open high word");
            check(raw[96 + 2 * i]     == (0x2000000000000000ULL + i), "entries_close low word");
            check(raw[96 + 2 * i + 1] == (0x0000000000002000ULL + i), "entries_close high word");
        }
        check(raw[36] == 0x5a5a1234ULL && raw[37] == 0x0badf00dULL,
              "raw opening/closing PLRU");
        cases++;

        // Case 8: the disabled legacy banks read zero for the mode-3 profile.
        check(b.obs_indexed(512) == 0ULL,
              "legacy relative 0 (old research magic) reads zero");
        check(b.obs_indexed(512 + 0xd00) == 0ULL, "legacy relative D00 reads zero");
        cases++;

        // Case 9: a new ARM clears the previous window state.
        b.arm_p4m_solo(1);
        check((b.obs_status() & 7ULL) == 1ULL, "observer re-armed");
        check(b.comp_read(6) == 1ULL, "composition ARMED and not frozen");
        check(b.comp_read(8) == 0ULL, "window cycles cleared by the new ARM");
        check(b.comp_read(12) == 0ULL && b.comp_read(13) == 0ULL && b.comp_read(14) == 0ULL,
              "V window counters cleared by the new ARM");
        check(b.comp_read(64) == 0ULL && b.comp_read(96) == 0ULL,
              "entry snapshots cleared by the new ARM");
        cases++;

        // Case 10: a configure attempted while the observation is ARMED is
        // rejected without clear, mode/did or generation side effects.
        check((b.comp_read(6) & 1ULL) != 0ULL, "still ARMED before the busy configure");
        uint64_t gen_before = b.comp_read(3);
        uint64_t cfg_before = b.comp_read(2);
        bool busy_clear = b.comp_program(3, 11);
        check(!busy_clear, "busy configure must not pulse the protected clear");
        check(b.comp_read(4) == 1ULL, "busy configure reports status 1");
        check(b.comp_read(3) == gen_before, "busy configure leaves generation unchanged");
        check(b.comp_read(2) == cfg_before, "busy configure leaves mode/did unchanged");
        cases++;

        std::cout << "PASS " << cases
                  << " composition observer scenarios (no driver/AXI conservation claim)\n";
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "FAIL after " << cases << ": " << e.what() << "\n";
        return 1;
    }
}
