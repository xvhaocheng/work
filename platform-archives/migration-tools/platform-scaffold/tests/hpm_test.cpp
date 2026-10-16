#include "Vhpm_test_top.h"
#include "verilated.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    Vhpm_test_top t;
    auto tick = [&]() { t.clk=0; t.eval(); t.clk=1; t.eval(); };
    auto expect = [&](uint64_t value, const char *name) {
        if (t.misses != value) {
            std::cerr << "FAIL " << name << ": got " << t.misses << ", expected " << value << '\n';
            std::exit(1);
        }
    };
    t.rst_n=0; t.valid=0; t.inhibit=0; t.filter_did=0; t.did=10;
    tick(); tick(); t.rst_n=1;
    for(int i=0;i<8;i++) tick(); expect(0,"idle after reset");
    t.valid=1; tick(); t.valid=0;
    for(int i=0;i<10;i++) tick(); expect(1,"single pulse then idle");
    t.valid=1; for(int i=0;i<12;i++) tick(); expect(2,"held-valid event counted once");
    t.valid=0; for(int i=0;i<5;i++) tick(); expect(2,"no stale valid");
    t.filter_did=1; t.did=11; t.valid=1; tick(); t.valid=0;
    for(int i=0;i<5;i++) tick(); expect(2,"foreign DID excluded");
    t.did=10; t.valid=1; tick(); t.valid=0;
    for(int i=0;i<5;i++) tick(); expect(3,"selected DID counted");
    t.inhibit=1; t.valid=1; tick(); t.valid=0;
    for(int i=0;i<5;i++) tick(); expect(3,"inhibited counter");
    t.inhibit=0; for(int i=0;i<5;i++) tick(); expect(3,"no replay on reenable");
    if(t.cycles<40) return 1;
    std::cout << "PASS HPM: pulse, held-valid, idle, DID filter, inhibit; cycles=" << t.cycles << '\n';
    t.final();
}
