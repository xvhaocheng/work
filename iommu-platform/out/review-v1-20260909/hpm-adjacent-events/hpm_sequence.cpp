#include "Vhpm_test_top.h"
#include "verilated.h"
#include <iostream>
int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv); Vhpm_test_top t;
  auto tick=[&](){ t.clk=0;t.eval();t.clk=1;t.eval();};
  t.rst_n=0;t.valid=0;t.inhibit=0;t.filter_did=1;t.did=10;t.event_id=5;t.event_mask=0;
  tick();tick();t.rst_n=1;tick();
  // Legal translation FSM progression: IDLE UT_REQ then a cold DDT lookup.
  t.valid=1;t.event_mask=1<<1;tick();
  t.event_mask=1<<5;for(int i=0;i<6;i++) tick();
  t.valid=0;for(int i=0;i<4;i++) tick();
  std::cout << "UT_REQ then held DDTW without invalid cycle: actual=" << t.misses << ", expected=1\n";
  bool reproduced=t.misses==0;
  // Control: the identical DDTW signal with an invalid cycle before it counts once.
  t.valid=1;t.event_mask=1<<5;for(int i=0;i<6;i++) tick();
  t.valid=0;for(int i=0;i<4;i++) tick();
  std::cout << "Separated held DDTW control: incremental actual=" << t.misses << ", expected=1\n";
  t.final(); return reproduced && t.misses==1 ? 0 : 1;
}
