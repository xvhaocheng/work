// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vp_iotlb_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>

static Vp_iotlb_top t;
static FILE* trace;
static unsigned evals=0, cycles=0, checks=0;
static bool unified=false;
static const char* phase="reset";
static void check(bool ok,const char* what) {
    checks++;
    if (!ok) { std::fprintf(stderr,"FAIL cycle=%u eval=%u phase=%s %s\n",cycles,evals,phase,what); std::exit(1); }
}
static void eval() {
    t.eval(); evals++;
    std::fprintf(trace,"%u,%u,%s,%u,%u,%04x,%u,%u,%u,%u,%u,%u,%04x,%04x,%04x,%04x,%04x\n",
      cycles,evals,phase,t.clk_i,t.rst_ni,t.policy_mask_i,t.policy_clear_i,t.update_i,
      t.lookup_i,t.update_vpn_i,t.lookup_vpn_i,t.p_hit_o,t.p_plru_o,t.p_plru_next_o,
      t.p_victim_o,t.shared_victim_o,t.p_valid_o);
    if (t.rst_ni) {
        check(t.off_equal_o,"PolicyEnable0 differs from 85b gold including q/n/victim");
        check(t.full_equal_o,"full-mask enabled differs from 85b gold including q/n/victim");
        if (unified) check(t.p_equal_o,"runtime full-mask P instance differs from 85b gold");
    }
}
static void tick() { t.clk_i=0;eval();t.clk_i=1;eval();t.clk_i=0;eval();cycles++; }
static void idle() {
    t.update_i=0;t.lookup_i=0;t.policy_clear_i=0;
    t.inval_vma_i=0;t.inval_gvma_i=0;t.inval_av_i=0;t.inval_gv_i=0;t.inval_pscv_i=0;
}
static void reset() {
    idle();t.en1_i=1;t.en2_i=0;t.flags_i=0x1f;t.pscid_i=1;t.gscid_i=0;
    t.update_vpn_i=0;t.lookup_vpn_i=0;t.inval_vpn_i=0;t.inval_pscid_i=0;t.inval_gscid_i=0;
    t.policy_mask_i=0xffff;t.rst_ni=0;tick();tick();t.rst_ni=1;tick();
}
static void fill(uint32_t vpn,uint16_t mask) {
    idle();t.policy_mask_i=mask;t.update_i=1;t.update_vpn_i=vpn;t.ppn_i=0x80000+vpn;
    eval(); unsigned victim=t.p_victim_o;
    check(victim && !(victim&(victim-1)) && !(victim&~mask),"fill victim not onehot/in partition");
    tick();idle();
}
static void hit(uint32_t vpn,uint16_t fillmask) {
    idle();t.policy_mask_i=fillmask;t.lookup_i=1;t.lookup_vpn_i=vpn;eval();
    check(t.p_hit_o,"expected resident hit");tick();idle();
}
static uint32_t rng=0x197a43;
static uint32_t rnd() {rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
int main(int argc,char**argv) {
    Verilated::commandArgs(argc,argv);
    check(argc==2,"need trace path");trace=std::fopen(argv[1],"w");check(trace,"trace open");
    std::fprintf(trace,"cycle,eval,phase,clk,rst,mask,clear,update,lookup,update_vpn,lookup_vpn,hit,plru_q,plru_n,victim,shared_victim,valid_q\n");
    unified=true;reset();phase="fullmask-gold";
    for(unsigned i=0;i<1800;i++) {
        idle();t.update_i=(rnd()%3)==0;t.lookup_i=rnd()&1;t.en1_i=rnd()&1;t.en2_i=rnd()&1;
        t.update_vpn_i=rnd()%96;t.lookup_vpn_i=rnd()%96;t.pscid_i=rnd()%4;t.gscid_i=rnd()%3;
        t.ppn_i=0x80000+(rnd()%512);t.flags_i=rnd()&0xff;
        unsigned inv=rnd()%31;
        t.inval_vma_i=inv==0;t.inval_gvma_i=inv==1;t.inval_av_i=rnd()&1;t.inval_gv_i=rnd()&1;
        t.inval_pscv_i=rnd()&1;t.inval_vpn_i=rnd()%96;t.inval_pscid_i=rnd()%4;t.inval_gscid_i=rnd()%3;
        tick();
    }
    unified=false;phase="resident-owner";reset();
    // Start at a known zero tree. Fill does not itself touch PLRU.
    fill(0x100,3);hit(0x100,3);fill(0x101,3);
    // Clear and recreate both V slots without retaining priming touch history.
    idle();t.policy_clear_i=1;tick();idle();
    fill(0x100,3);hit(0x100,3);fill(0x101,3);
    for(unsigned i=0;i<14;i++) {
        fill(0x200+i,0xfffc);
        hit(0x200+i,0xfffc); // Establish distinct resident pages across the eligible tree.
    }
    // Use the real surviving hits; allocation is PLRU, not invalid-first/LRU.
    t.policy_mask_i=0xfffc;eval();unsigned a_before=t.p_victim_o;
    hit(0x100,0xfffc);t.policy_mask_i=0xfffc;eval();
    check(t.p_victim_o==a_before,"V hit changed next A victim under A fill mask");
    hit(0x101,0xfffc);t.policy_mask_i=0xfffc;eval();
    check(t.p_victim_o==a_before,"other V hit changed next A victim");
    t.policy_mask_i=3;eval();unsigned v_before=t.p_victim_o;
    hit(0x20d,3);t.policy_mask_i=3;eval();
    check(t.p_victim_o==v_before,"A hit changed next V victim under V fill mask");
    phase="clear-priority";
    t.policy_clear_i=1;t.update_i=1;t.update_vpn_i=0x555;t.lookup_i=1;t.lookup_vpn_i=0x100;
    eval();check(t.p_valid_next_o==0 && t.p_plru_next_o==0,"clear did not beat same-edge fill/touch");
    tick();check(t.p_valid_o==0 && t.p_plru_o==0,"clear did not clear tags/tree");idle();
    phase="zero-mask";t.policy_mask_i=0;t.update_i=1;t.update_vpn_i=0x777;eval();
    check(t.p_victim_o==0,"empty mask selected victim");tick();check(t.p_valid_o==0,"empty mask filled entry");
    phase="disabled-policy-noise";
    for(unsigned i=0;i<400;i++) {idle();t.policy_mask_i=rnd();t.policy_clear_i=(rnd()%7)==0;
      t.lookup_i=rnd()&1;t.update_i=rnd()&1;t.lookup_vpn_i=rnd()%32;t.update_vpn_i=rnd()%32;tick();}
    std::fclose(trace);
    std::printf("PASS p_iotlb cycles=%u evals=%u checks=%u gold=85b fullmask+disabled exact; partition/clear tested\n",cycles,evals,checks);
    t.final();return 0;
}
