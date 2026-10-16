// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Independent Verilator regression driver for rv_iommu_axi_bc (AXI4 4-kiB
// boundary checker).
//
// The expected result is produced by a golden model that walks the AXI beats
// of the transaction and applies the AXI4 rule "a burst must never cross a
// 4-kiB boundary": every beat of the burst must live in the very same 4-kiB
// page.  The DUT's own end-of-burst arithmetic is deliberately NOT reused.
//
// Usage:
//   axi_bc_test [--focus <name>]... [--max-failures N] [--list-tests]
//
// Exit status: 0 = every check matched the golden model, 1 = mismatch(es),
//              2 = usage error.

#include "verilated.h"
#include "Vaxi_bc_tb.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------
const unsigned kPageShift        = 12;          // 4 kiB pages
const uint64_t kPageSize         = 1ull << kPageShift;
const unsigned kDefaultMaxPrints = 16;

// AxBURST encodings, as in vendor/axi/src/axi_pkg.sv
const unsigned BURST_FIXED = 0;
const unsigned BURST_INCR  = 1;
const unsigned BURST_WRAP  = 2;
const unsigned BURST_RSVD  = 3;

const char* burst_name(unsigned burst) {
    switch (burst) {
        case BURST_FIXED: return "FIXED";
        case BURST_INCR:  return "INCR";
        case BURST_WRAP:  return "WRAP";
        default:          return "RSVD";
    }
}

// ---------------------------------------------------------------------------
// DUT handle and port access helpers
// ---------------------------------------------------------------------------
Vaxi_bc_tb* g_dut = 0;

// Verilator stores a 64-bit port either in a scalar word (QData, uint64_t) or
// in a wide vector (VlWide<2>), depending on version and options: support both.
template <typename T>
typename std::enable_if<std::is_class<T>::value>::type
write_addr(T& sig, uint64_t v) {
    sig[0] = static_cast<uint32_t>(v & 0xffffffffull);
    sig[1] = static_cast<uint32_t>(v >> 32);
}

template <typename T>
typename std::enable_if<!std::is_class<T>::value>::type
write_addr(T& sig, uint64_t v) {
    sig = v;
}

// Drive one AXI request into the checker and return allow_request_o.
bool dut_eval(uint64_t addr, unsigned burst, unsigned len, unsigned size, bool check) {
    write_addr(g_dut->addr_i, addr);
    g_dut->check_i        = static_cast<unsigned char>(check ? 1u : 0u);
    g_dut->burst_type_i   = static_cast<unsigned char>(burst & 0x3u);
    g_dut->burst_length_i = static_cast<unsigned char>(len & 0xffu);
    g_dut->n_bytes_i      = static_cast<unsigned char>(size & 0x7u);
    g_dut->eval();
    return g_dut->allow_request_o != 0;
}

// ---------------------------------------------------------------------------
// Golden reference model
// ---------------------------------------------------------------------------
// Beat addresses per AXI4 (compare axi_pkg::beat_addr / axi_pkg::wrap_boundary):
//   Number_Bytes    = 1 << AxSIZE
//   Burst_Length    = AxLEN + 1
//   Aligned_Address = (INT(Start_Address / Number_Bytes)) * Number_Bytes
//
//   INCR : beat 0 is Start_Address; beat N (N > 0) is
//          Aligned_Address + N * Number_Bytes.
//   FIXED: every beat uses Start_Address; the valid byte lanes end at
//          Aligned_Address + Number_Bytes - 1 (A3-51 / A3-52).
//   WRAP : only AxLEN in {1,3,7,15} (2/4/8/16 beats) and a Number_Bytes
//          aligned start address are legal; beats walk
//          Aligned_Address + N * Number_Bytes and wrap back to
//          Wrap_Boundary = INT(Start_Address / Window) * Window with
//          Window = Number_Bytes * Burst_Length.
//   AxBURST = 2'b11 is reserved and never a legal transaction.
//
// A transaction is allowed iff every beat is fully inside one 4-kiB page and
// all beats agree on that page (first beat, last beat and every beat between).
bool golden(uint64_t addr, unsigned burst, unsigned len, unsigned size, bool check) {
    if (!check) return false;                  // checker disabled / AxVALID low
    if (burst == BURST_RSVD) return false;     // reserved burst encoding

    const uint64_t nb      = 1ull << (size & 0x7u);        // Number_Bytes
    const uint64_t nbeats  = static_cast<uint64_t>(len) + 1ull;
    const uint64_t aligned = addr & ~(nb - 1ull);          // Aligned_Address

    uint64_t ref_page = 0;
    bool have_ref = false;

    // Register one beat [lo, hi]; hi is the last byte touched by the beat.
    auto beat_ok = [&](uint64_t lo, uint64_t hi) -> bool {
        const uint64_t pl = lo >> kPageShift;
        const uint64_t ph = hi >> kPageShift;
        if (pl != ph) return false;                        // beat spans two pages
        if (!have_ref) {
            have_ref = true;
            ref_page = pl;
            return true;
        }
        return pl == ref_page;                             // beat in another page
    };

    switch (burst) {
        case BURST_FIXED: {
            for (uint64_t i = 0; i < nbeats; ++i) {
                if (!beat_ok(addr, aligned + nb - 1ull)) return false;
            }
            return true;
        }
        case BURST_INCR: {
            if (!beat_ok(addr, aligned + nb - 1ull)) return false;
            for (uint64_t i = 1; i < nbeats; ++i) {
                const uint64_t a = aligned + i * nb;
                if (!beat_ok(a, a + nb - 1ull)) return false;
            }
            return true;
        }
        case BURST_WRAP: {
            if (len != 1 && len != 3 && len != 7 && len != 15) return false;
            if ((addr & (nb - 1ull)) != 0) return false;   // start not size aligned
            const uint64_t window = nb * nbeats;
            const uint64_t wrap_boundary = (addr / window) * window;
            for (uint64_t i = 0; i < nbeats; ++i) {
                uint64_t a = addr + i * nb;
                if (a >= wrap_boundary + window) a -= window;
                if (!beat_ok(a, a + nb - 1ull)) return false;
            }
            return true;
        }
        default:
            return false;
    }
}

// ---------------------------------------------------------------------------
// Bookkeeping and reporting
// ---------------------------------------------------------------------------
const unsigned G_SWEEP = 0;
const unsigned G_GATE  = 1;
const unsigned G_RSVD  = 2;
const unsigned G_ALIAS = 3;
const unsigned G_FOCUS = 4;
const unsigned G_COUNT = 5;

const char* const kGroupTag[G_COUNT] = {
    "sweep", "gate", "rsvd", "alias", "focus"
};
const char* const kGroupDesc[G_COUNT] = {
    "offset sweep: FIXED/INCR/WRAP x size 0..7 x len {0,1,3,7,15,31,127,255} x offset 0..4095",
    "check_i = 0 must reject",
    "reserved AxBURST = 2'b11 must reject",
    "same page offset at a high address (bit 40)",
    "named focus cases"
};

unsigned long long g_checks[G_COUNT] = {0};
unsigned long long g_fails[G_COUNT]  = {0};
unsigned g_printed    = 0;
unsigned g_max_prints = kDefaultMaxPrints;

void report_fail(unsigned group, const char* tag, uint64_t addr, unsigned burst,
                 unsigned len, unsigned size, bool check, int got, bool expect) {
    ++g_fails[group];
    if (g_printed >= g_max_prints) return;
    ++g_printed;
    if (got < 0) {
        std::printf("FAIL [%s:%s] addr=0x%012llx burst=%s len=%u size=%u check=%u : "
                    "hard-coded expectation disagrees with the golden model\n",
                    kGroupTag[group], tag, static_cast<unsigned long long>(addr),
                    burst_name(burst), len, size, check ? 1u : 0u);
        return;
    }
    std::printf("FAIL [%s:%s] addr=0x%012llx burst=%s len=%u size=%u check=%u : "
                "dut=%s golden=%s\n",
                kGroupTag[group], tag, static_cast<unsigned long long>(addr),
                burst_name(burst), len, size, check ? 1u : 0u,
                got ? "ALLOW" : "REJECT", expect ? "ALLOW" : "REJECT");
}

// expect_override >= 0 forces a hard-coded expectation (used by the focus
// cases, which also cross-check the golden model); -1 lets the model decide.
void check_case(unsigned group, const char* tag, uint64_t addr, unsigned burst,
                unsigned len, unsigned size, bool check, int expect_override) {
    ++g_checks[group];
    const bool expect = golden(addr, burst, len, size, check);
    if (expect_override >= 0 && expect != (expect_override != 0)) {
        report_fail(group, tag, addr, burst, len, size, check, -1, expect);
        return;
    }
    const bool got = dut_eval(addr, burst, len, size, check);
    if (got != expect) {
        report_fail(group, tag, addr, burst, len, size, check, got ? 1 : 0, expect);
    }
}

// ---------------------------------------------------------------------------
// Named focus cases (fixed names, also used by the legacy red tests)
// ---------------------------------------------------------------------------
struct FocusCase {
    const char* name;
    uint64_t    addr;
    unsigned    burst;
    unsigned    len;
    unsigned    size;
    bool        check;
    int         expect;   // >= 0: hard-coded expectation, cross-checked
};

const FocusCase kFocusCases[] = {
    // 0xFF8 + 8 bytes - 1 = 0xFFF: the beat window ends on the last
    // byte of the 4-kiB page -> the burst does not cross the boundary.
    {"fixed-tail",     0x0ff8ull, BURST_FIXED, 0, 3, true, 1},
    // 4 beats x 8 bytes from 0xFF0: beat 2 starts at 0x1000 -> crosses.
    {"incr-cross",     0x0ff0ull, BURST_INCR,  3, 3, true, 0},
    // Three 8-byte beats cross the page, but a half-length formula misses it.
    {"incr-cross-half",0x0ff0ull, BURST_INCR,  2, 3, true, 0},
    // Single 1-byte beat on the last byte of the page -> legal.
    {"incr-byte-tail", 0x0fffull, BURST_INCR,  0, 0, true, 1},
    // Unaligned first beat touches 0xFF9..0xFFF of its 8-byte window -> legal.
    {"incr-unaligned", 0x0ff9ull, BURST_INCR,  0, 3, true, 1}
};
const unsigned kNumFocusCases = sizeof(kFocusCases) / sizeof(kFocusCases[0]);

int run_focus(const std::string& name) {
    for (unsigned i = 0; i < kNumFocusCases; ++i) {
        if (name == kFocusCases[i].name) {
            const FocusCase& c = kFocusCases[i];
            check_case(G_FOCUS, c.name, c.addr, c.burst, c.len, c.size, c.check, c.expect);
            return 0;
        }
    }
    return -1;
}

// ---------------------------------------------------------------------------
// Default suite (816064 checks, i.e. below the 1e6 budget)
// ---------------------------------------------------------------------------
void run_full_suite() {
    const unsigned kBursts[]      = {BURST_FIXED, BURST_INCR, BURST_WRAP};
    const unsigned kSizes[]       = {0, 1, 2, 3, 4, 5, 6, 7};
    const unsigned kLens[]        = {0, 1, 3, 7, 15, 31, 127, 255};
    const uint64_t kGateOffsets[] = {0x0ull, 0x1ull, 0x7ffull, 0x0ff8ull, 0x0fffull};

    // 1) every offset inside a 4-kiB page, every size, the three burst types
    //    and the requested AxLEN set: 3 * 8 * 8 * 4096 = 786432 checks.
    for (unsigned b = 0; b < 3; ++b) {
        for (unsigned s = 0; s < 8; ++s) {
            for (unsigned l = 0; l < 8; ++l) {
                for (uint64_t off = 0; off < kPageSize; ++off) {
                    check_case(G_SWEEP, "offset", off, kBursts[b], kLens[l], kSizes[s], true, -1);
                }
            }
        }
    }

    // 2) check_i = 0 must always reject: 3 * 8 * 8 * 5 = 960 checks.
    for (unsigned b = 0; b < 3; ++b) {
        for (unsigned s = 0; s < 8; ++s) {
            for (unsigned l = 0; l < 8; ++l) {
                for (unsigned o = 0; o < 5; ++o) {
                    check_case(G_GATE, "gate0", kGateOffsets[o], kBursts[b], kLens[l], kSizes[s], false, 0);
                }
            }
        }
    }

    // 3) reserved burst encoding must always reject (sampled offsets):
    //    8 * 8 * 256 = 16384 checks.
    for (unsigned s = 0; s < 8; ++s) {
        for (unsigned l = 0; l < 8; ++l) {
            for (uint64_t off = 0; off < kPageSize; off += 16) {
                check_case(G_RSVD, "reserved", off, BURST_RSVD, kLens[l], kSizes[s], true, 0);
            }
        }
    }

    // 4) the same page offset at a high address (bit 40) must give the same
    //    verdict: 3 * 8 * 8 * 64 = 12288 checks.
    for (unsigned b = 0; b < 3; ++b) {
        for (unsigned s = 0; s < 8; ++s) {
            for (unsigned l = 0; l < 8; ++l) {
                for (uint64_t off = 0; off < kPageSize; off += 64) {
                    check_case(G_ALIAS, "alias", off | (1ull << 40), kBursts[b], kLens[l], kSizes[s], true, -1);
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// CLI
// ---------------------------------------------------------------------------
void usage() {
    std::printf(
        "usage: axi_bc_test [--focus <name>]... [--max-failures N] [--list-tests]\n"
        "  --focus <name>    run one named regression case (repeatable)\n"
        "  --max-failures N  print at most N failing checks (default 16)\n"
        "  --list-tests      list the named regression cases and exit\n");
}

void print_summary() {
    unsigned long long total = 0;
    unsigned long long fails = 0;
    std::printf("\n");
    std::printf("axi_bc regression summary\n");
    for (unsigned g = 0; g < G_COUNT; ++g) {
        if (g_checks[g] == 0) continue;
        std::printf("  %-6s %-78s : %8llu checks, %llu failures\n",
                    kGroupTag[g], kGroupDesc[g], g_checks[g], g_fails[g]);
        total += g_checks[g];
        fails += g_fails[g];
    }
    std::printf("  %-6s %-78s : %8llu checks, %llu failures\n",
                "TOTAL", "", total, fails);
    if (g_printed < fails) {
        std::printf("  (only the first %u failures were printed)\n", g_printed);
    }
    std::printf("axi_bc: %s (checks=%llu, failures=%llu)\n",
                fails ? "FAIL" : "PASS", total, fails);
    std::fflush(stdout);
}

}  // namespace

int main(int argc, char** argv) {
    std::vector<std::string> focus;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--focus") {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "axi_bc_test: --focus needs an argument\n");
                return 2;
            }
            focus.push_back(std::string(argv[++i]));
        } else if (arg.compare(0, 8, "--focus=") == 0) {
            focus.push_back(arg.substr(8));
        } else if (arg == "--max-failures") {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "axi_bc_test: --max-failures needs an argument\n");
                return 2;
            }
            g_max_prints = static_cast<unsigned>(std::atoi(argv[++i]));
        } else if (arg == "--list-tests") {
            for (unsigned k = 0; k < kNumFocusCases; ++k) {
                std::printf("%s\n", kFocusCases[k].name);
            }
            return 0;
        } else if (arg == "--help" || arg == "-h") {
            usage();
            return 0;
        } else {
            std::fprintf(stderr, "axi_bc_test: unknown option '%s'\n", arg.c_str());
            usage();
            return 2;
        }
    }

    g_dut = new Vaxi_bc_tb;

    if (!focus.empty()) {
        for (std::size_t i = 0; i < focus.size(); ++i) {
            if (run_focus(focus[i]) != 0) {
                std::fprintf(stderr, "axi_bc_test: unknown focus case '%s'\n",
                             focus[i].c_str());
                std::fprintf(stderr, "known cases:");
                for (unsigned k = 0; k < kNumFocusCases; ++k) {
                    std::fprintf(stderr, " %s", kFocusCases[k].name);
                }
                std::fprintf(stderr, "\n");
                g_dut->final();
                delete g_dut;
                return 2;
            }
        }
    } else {
        run_full_suite();
    }

    print_summary();

    g_dut->final();
    delete g_dut;

    unsigned long long fails = 0;
    for (unsigned g = 0; g < G_COUNT; ++g) fails += g_fails[g];
    return fails ? 1 : 0;
}
