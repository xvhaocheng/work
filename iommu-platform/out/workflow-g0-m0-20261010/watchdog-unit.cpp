#include <cassert>
static bool full_watchdog_fault(long now, long last_progress, bool issued,
                                long supply_wait, long gap) {
    const long elapsed = now - last_progress;
    return issued ? (elapsed > gap) : (elapsed > supply_wait);
}
int main(){assert(full_watchdog_fault(1000,0,false,500,200));assert(!full_watchdog_fault(200,0,false,500,200));assert(full_watchdog_fault(501,200,true,500,200));assert(!full_watchdog_fault(250,200,true,500,200));}
