#include "Vdma_pn_control.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

static uint64_t checks = 0, beats = 0, cases = 0, half_cycles = 0;
double sc_time_stamp() { return static_cast<double>(half_cycles); }
static void ck(bool value, const std::string &label) {
    ++checks;
    if (!value) throw std::runtime_error(label);
}

struct Bench {
    Vdma_pn_control d;
    const bool enabled, history;
    unsigned mode = 0;
    bool rejected = false;
    explicit Bench(bool e, bool h) : enabled(e), history(h) {
        d.clk_i = 0; d.rst_ni = 0;
        d.cfg_valid_i = d.cfg_write_i = 0; d.cfg_addr_i = 0;
        d.cfg_wdata_i = 0; d.cfg_wstrb_i = 255; d.index_i = 0;
        guards(0);
        d.n_did10_i = 0xfedcba98; d.n_did11_i = 0x89abcdef;
        d.aown_cycles_i = UINT64_C(0x80000000fedcba98);
        d.union_cycles_i = UINT64_C(0xfffffffedcba9876);
        d.capture_flags_i = 0xa5;
        d.eval(); sample("async reset"); tick("reset edge");
        d.rst_ni = 1; d.eval(); sample("reset release");
    }
    void guards(unsigned mask) {
        d.dataflow_quiet_i = !(mask & 1);
        d.obs_armed_i = bool(mask & 2); d.obs_busy_i = bool(mask & 4);
        d.capture_busy_i = bool(mask & 8); d.certificate_fault_i = bool(mask & 16);
        d.arm_accepted_i = bool(mask & 32);
    }
    bool eligible() const {
        return d.dataflow_quiet_i && !d.obs_armed_i && !d.obs_busy_i &&
               !d.capture_busy_i && !d.certificate_fault_i && !d.arm_accepted_i;
    }
    bool legal() const {
        switch (d.cfg_wdata_i) {
            case 0: case 1: case 2: case 3: return true;
            case 6: return history;
            default: return false;
        }
    }
    bool intent() const {
        return d.cfg_valid_i && d.cfg_write_i && d.cfg_addr_i == 120 && d.index_i == 193;
    }
    bool accepted() const { return intent() && d.cfg_wstrb_i == 255 && legal() && eligible(); }
    uint64_t bank() const {
        if (!enabled) return 0;
        switch (d.index_i) {
            case 192: return UINT64_C(0x49444d41504e0001);
            case 193: return mode;
            case 194: return (uint64_t(history) << 63) | uint64_t(eligible()) |
                (uint64_t(rejected) << 1) | (uint64_t(d.certificate_fault_i) << 2) |
                (uint64_t(d.capture_busy_i) << 3) | (uint64_t(d.dataflow_quiet_i) << 4) |
                (uint64_t(d.obs_busy_i) << 5);
            case 195: return d.n_did10_i;
            case 196: return d.n_did11_i;
            case 197: return d.aown_cycles_i;
            case 198: return d.union_cycles_i;
            case 199: return d.capture_flags_i;
            default: return 0;
        }
    }
    void sample(const std::string &label) {
        ck(d.mode_word_o == (enabled ? mode : 0), label + ": atomic full mode/history word");
        ck(bool(d.policy_clear_o) == (enabled && d.rst_ni && accepted()), label + ": exact combinational policy clear");
        ck(bool(d.bank_selected_o) == (enabled && d.index_i >= 192 && d.index_i <= 199), label + ": bank selected range");
        ck(d.bank_rdata_o == bank(), label + ": complete bank word and reserved bits");
    }
    void tick(const std::string &label) {
        d.clk_i = 0; d.eval(); sample(label + " before");
        if (!d.rst_ni || !enabled) { mode = 0; rejected = false; }
        else if (intent()) {
            if (accepted()) { mode = unsigned(d.cfg_wdata_i); rejected = false; }
            else rejected = true;
        }
        ++half_cycles; d.clk_i = 1; d.eval(); sample(label + " after");
        ++half_cycles; d.clk_i = 0; d.eval(); ++beats;
    }
    void write(uint64_t word, unsigned strobe = 255, unsigned addr = 120,
               unsigned index = 193, bool valid = true, bool is_write = true) {
        d.cfg_wdata_i = word; d.cfg_wstrb_i = strobe; d.cfg_addr_i = addr;
        d.index_i = index; d.cfg_valid_i = valid; d.cfg_write_i = is_write;
        tick("request beat");
        d.cfg_valid_i = 0; d.eval(); sample("request removed");
        ck(!d.policy_clear_o, "clear does not persist without a request");
    }
    void read(unsigned index) {
        d.index_i = index; d.cfg_addr_i = 120; d.cfg_valid_i = 1; d.cfg_write_i = 0;
        tick("read beat"); d.cfg_valid_i = 0; d.eval(); sample("read removed");
    }
    void finish(const char *name) {
        ++cases;
        std::cout << "{\"case\":\"" << name << "\",\"status\":\"passed\","
                     "\"fixture_only\":true,\"source_board_cases\":0}\n";
    }
};

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    try {
        ck(argc == 3, "test requires explicit compile-profile expectations");
        const bool enabled = std::string(argv[1]) == "1", history = std::string(argv[2]) == "1";
        Bench b(enabled, history);
        b.read(192); b.read(193); b.read(194); b.finish("reset-cap-status");
        // All directed transitions include same-word flush and 0<->6 / 2<->6.
        for (unsigned from : {0U, 1U, 2U, 3U, 6U}) {
            for (unsigned to : {0U, 1U, 2U, 3U, 6U}) {
                b.guards(0); b.write(from); b.write(to); b.read(193); b.read(194);
            }
        }
        b.finish("all-mode-transitions-and-same-word-flush");
        for (unsigned word = 0; word < 256; ++word) { b.write(word); b.read(193); b.read(194); }
        b.finish("all-low-byte-values-and-shared-support");
        for (unsigned bit = 8; bit < 64; ++bit) { b.write((UINT64_C(1) << bit) | 6); b.read(193); b.read(194); }
        b.finish("every-high-reserved-bit-rejected");
        for (unsigned mask = 0; mask < 64; ++mask) {
            b.guards(0); b.write(2); b.guards(mask); b.write(3); b.read(193); b.read(194);
        }
        b.guards(0); b.finish("each-eligibility-guard-and-all-combinations");
        for (unsigned strobe = 0; strobe < 256; ++strobe) { b.write(1, strobe); b.read(193); b.read(194); }
        b.finish("all-byte-strobes");
        for (unsigned addr = 0; addr < 128; ++addr) { b.write(2, 255, addr); b.read(194); }
        b.finish("only-data-address-is-mode-intent");
        for (unsigned index = 0; index < 256; ++index) { b.write(3, 255, 120, index); b.read(index); b.read(194); }
        b.finish("all-indices-and-readonly-writes");
        b.write(4); // Establish sticky rejection even when quiet.
        for (unsigned index = 192; index <= 199; ++index) b.read(index);
        b.write(0, 255, 120, 193, false); b.read(194);
        b.write(0, 255, 120, 193, true, false); b.read(194);
        b.write(0, 255, 112, 193); b.read(194); // INDEX selector is external.
        b.write(0, 255, 120, 194); b.read(194);
        b.write(0); b.read(194); b.finish("reads-and-unrelated-access-do-not-clear-rejection");
        // No counter state exists here: arbitrary live input updates must remain
        // visible across accepted/rejected control writes and reads.
        for (unsigned trial = 0; trial < 16; ++trial) {
            b.d.n_did10_i = 0xffffffffU - trial; b.d.n_did11_i = 1U << trial;
            b.d.aown_cycles_i = (UINT64_C(1) << (trial + 32)) | trial;
            b.d.union_cycles_i = UINT64_MAX - trial; b.d.capture_flags_i = trial * 17;
            b.write(trial % 2 ? 6 : 4);
            for (unsigned index = 195; index <= 199; ++index) b.read(index);
        }
        b.finish("counter-and-duration-inputs-remain-passive-and-full-width");
        b.write(3); b.write(4); b.d.rst_ni = 0; b.mode = 0; b.rejected = false;
        b.d.cfg_valid_i = b.d.cfg_write_i = 1; b.d.cfg_addr_i = 120;
        b.d.cfg_wstrb_i = 255; b.d.cfg_wdata_i = 6; b.d.index_i = 193;
        b.d.eval(); b.sample("async reset with request");
        ck(!b.d.policy_clear_o, "reset suppresses clear even with a pending valid write");
        b.tick("reset request ignored"); b.d.cfg_valid_i = 0; b.d.rst_ni = 1;
        b.read(193); b.read(194); b.finish("async-reset-clears-mode-and-rejection");
        b.d.final();
        std::cout << "{\"summary\":\"passed\",\"fixture_only\":true,\"source_board_cases\":0,"
                     "\"enable\":" << enabled << ",\"history\":" << history << ",\"cases\":" << cases
                  << ",\"checks\":" << checks << ",\"request_clock_beats\":" << beats << "}\n";
    } catch (const std::exception &error) { std::cerr << error.what() << '\n'; return 1; }
    return 0;
}
