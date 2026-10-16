#include "Vresearch_ar_component_tb.h"
#include "verilated.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

namespace {
int plusarg(int argc, char** argv, const char* prefix, int fallback) {
    const std::size_t width = std::strlen(prefix);
    for (int i = 1; i < argc; ++i) {
        if (std::strncmp(argv[i], prefix, width) == 0)
            return std::atoi(argv[i] + width);
    }
    return fallback;
}

void clock_cycle(Vresearch_ar_component_tb& top) {
    top.clk_i = 0;
    top.eval();
    top.clk_i = 1;
    top.eval();
    top.clk_i = 0;
    top.eval();
}

void expect_cycle(Vresearch_ar_component_tb& top, unsigned mask, bool ready,
                  int expected_source, bool tamper, const char* label) {
    top.req = mask;
    top.ready = ready;
    top.tamper_i = tamper;
    top.clk_i = 0;
    top.eval();
    if (expected_source >= 0) {
        const unsigned expected_grant = ready ? (1u << expected_source) : 0u;
        if (!top.valid || top.index_o != expected_source || top.grant != expected_grant) {
            throw std::runtime_error(std::string(label) +
                ": unexpected valid/index/grant: " + std::to_string(top.valid) +
                "/" + std::to_string(top.index_o) + "/" + std::to_string(top.grant));
        }
    } else if (top.valid || (top.grant & mask)) {
        throw std::runtime_error(std::string(label) + ": idle has a valid request/accept");
    }
    clock_cycle(top);
}
}  // namespace

int main(int argc, char** argv) {
    // $fatal deliberately aborts negative cases; keep its evidence unbuffered.
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    Verilated::commandArgs(argc, argv);
    auto top = std::make_unique<Vresearch_ar_component_tb>();
    const int mode = plusarg(argc, argv, "+ar-ds-first=", 0);
    const bool bad_held = plusarg(argc, argv, "+bad-held=", 0) != 0;
    try {
        top->clk_i = 0;
        top->rst_ni = 0;
        top->req = 0;
        top->ready = 0;
        top->tamper_i = 0;
        top->eval();  // Invalid mode must fail in the selector initial block.
        clock_cycle(*top);
        clock_cycle(*top);
        top->rst_ni = 1;
        top->eval();

        if (mode == 1 && !bad_held) {
            for (int n = 0; n < 4; ++n)
                expect_cycle(*top, 0xf, true, 3, false, "competitive DS accept");
            expect_cycle(*top, 0xf, true, 0, false, "F4 escape to source0");
            expect_cycle(*top, 0x7, true, 1, false, "nonDS RR source1");
            expect_cycle(*top, 0x7, true, 2, false, "nonDS RR source2");
            expect_cycle(*top, 0x7, true, 0, false, "nonDS RR wrap source0");
            expect_cycle(*top, 0x0, false, -1, false, "clear idle");
        }

        expect_cycle(*top, 0x4, false, 2, false, "first nonDS held offer");
        expect_cycle(*top, 0xc, false, 2, bad_held, "late DS while source2 held");
        expect_cycle(*top, 0xc, false, 2, false, "held source2 second stalled cycle");
        expect_cycle(*top, 0xc, true, 2, false, "accept unchanged held source2");
        expect_cycle(*top, 0x8, true, 3, false, "accept DS after held source2");
        expect_cycle(*top, 0x0, true, -1, false, "final quiet");
        top->final();
        std::printf("COMPONENT-PASS mode=%d\n", mode);
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "COMPONENT-FAIL: %s\n", error.what());
        top->final();
        return 1;
    }
}
