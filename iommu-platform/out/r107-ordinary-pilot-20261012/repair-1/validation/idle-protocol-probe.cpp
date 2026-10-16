#include "Vdma_composition_control.h"
#include "verilated.h"
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <iostream>
static Vdma_composition_control *dut;
struct idma_device {bool quarantined; unsigned char *reg;};
static uint64_t regs[4][32] = {};
static idma_device devices[4];
static int missing=-1,refs=0,checks=0;
#define IDMA_STATUS_REG_OFFSET 0x20
#define IDMA_BATCH_CONTROL_OFFSET 0x88
#define READ_ONCE(x) (x)
static idma_device *idma_get_instance(unsigned i) {if ((int)i==missing)return nullptr; refs++;return &devices[i];}
static void idma_put_instance(idma_device*) {refs--;}
static uint64_t readq(const unsigned char *p) {uint64_t x;std::memcpy(&x,p,8);return x;}
static uint64_t idma_composition_read(idma_device*,unsigned i) {dut->index_i=i;dut->eval();return dut->rdata_o;}
static bool old_idma_finite_quiet(struct idma_device *d)
{
	unsigned int i;
	bool quiet = true;

	for (i = 0; i < 4; i++) {
		struct idma_device *other = idma_get_instance(i);

		if (!other)
			return false;
		if (READ_ONCE(other->quarantined) ||
		    readq(other->reg + IDMA_STATUS_REG_OFFSET) ||
		    (readq(other->reg + IDMA_BATCH_CONTROL_OFFSET) & 2))
			quiet = false;
		idma_put_instance(other);
	}
	return quiet && idma_composition_read(d, 5) == 1;
}

static bool idma_finite_engines_idle(void)
{
	unsigned int i;
	bool quiet = true;

	for (i = 0; i < 4; i++) {
		struct idma_device *other = idma_get_instance(i);

		if (!other)
			return false;
		if (READ_ONCE(other->quarantined) ||
		    readq(other->reg + IDMA_STATUS_REG_OFFSET) ||
		    (readq(other->reg + IDMA_BATCH_CONTROL_OFFSET) & 2))
			quiet = false;
		idma_put_instance(other);
	}
	return quiet;
}

/* Configuration idle additionally excludes an ARMED/ACTIVE observation.
 * Keep it for preflight, mapping barriers, output ownership and final release.
 * Within our exclusive finite phase, a batch is allowed after ARM and between
 * retired batches: backend status plus batch ACTIVE is checked below, and
 * known whole-batch DONE proves the owner's previously accepted work retired.
 * Initial configuration idle and sole software issuer are required throughout;
 * this is not an arbitrary cross-owner frontend/AXI-drain API.
 */
static bool idma_finite_quiet(struct idma_device *d)
{
	return idma_finite_engines_idle() && idma_composition_read(d, 5) == 1;
}


static void need(bool x,const char *s) {checks++;if(!x)throw std::runtime_error(s);if(refs)throw std::runtime_error("reference imbalance");}
static void tick() {dut->clk_i=0;dut->eval();dut->clk_i=1;dut->eval();dut->clk_i=0;dut->eval();}
static void state_checks(bool config_idle,const char *name) {
 need(old_idma_finite_quiet(&devices[0])==config_idle,name);
 need(idma_finite_engines_idle(),"idle engines remain legal during an owned observation");
 need(idma_finite_quiet(&devices[0])==config_idle,"strict config/fence/release idle remains distinct");
}
int main(int argc,char **argv) {
 Verilated::commandArgs(argc,argv);Vdma_composition_control top;dut=&top;
 for(int i=0;i<4;i++)devices[i]={false,(unsigned char*)regs[i]};
 dut->rst_ni=0;dut->quiet_i=1;dut->supported_i=1;tick();dut->rst_ni=1;tick();
 state_checks(true,"initial configuration idle");
 dut->arm_i=1;tick();dut->arm_i=0;dut->eval();state_checks(false,"ARM reproduces old pre-NEXT rejection");
 dut->open_i=1;tick();dut->open_i=0;dut->eval();state_checks(false,"ACTIVE permits retired owned batch boundary");
 for(int i=0;i<4;i++) {
  regs[i][IDMA_STATUS_REG_OFFSET/8]=1;need(!idma_finite_engines_idle(),"backend not idle cannot pass");regs[i][IDMA_STATUS_REG_OFFSET/8]=0;
  regs[i][IDMA_BATCH_CONTROL_OFFSET/8]=2;need(!idma_finite_engines_idle(),"batch ACTIVE cannot pass");regs[i][IDMA_BATCH_CONTROL_OFFSET/8]=0;
  devices[i].quarantined=true;need(!idma_finite_engines_idle(),"quarantined lane cannot pass");devices[i].quarantined=false;
  missing=i;need(!idma_finite_engines_idle(),"missing lane cannot pass");missing=-1;
 }
 dut->close_i=1;tick();dut->close_i=0;dut->eval();state_checks(true,"FROZEN restores strict full-idle requirement");
 dut->quiet_i=0;dut->eval();need(!idma_finite_quiet(&devices[0]),"global config busy still blocks barriers/release");
 std::cout<<"{\"checks\":"<<checks<<",\"passed\":true,\"original_ARM_false_rejection_reproduced\":true,\"actual_frozen_composition_RTL\":true,\"exact_old_and_repaired_C_helpers\":true,\"actual_DMA_or_FPGA\":false}"<<std::endl;return 0;
}
