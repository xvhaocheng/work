// P4M observer contract regression: P4MEnable=1, PathEnable=0.
// Independently driven clock and event schedule; no board or formal claim.
#include "Vdma_observer.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

static unsigned cases;
static void check(bool condition, const char *message) {
    if (!condition) throw std::runtime_error(message);
}

struct Bench {
    Vdma_observer d;
    void tick(unsigned n=1) { while(n--) {d.clk_i=0;d.eval();d.clk_i=1;d.eval();d.clk_i=0;d.eval();} }
    void reset() {
        d.cfg_valid_i=d.cfg_write_i=0;d.cfg_addr_i=0;d.cfg_wdata_i=0;d.cfg_wstrb_i=255;
        d.issue_i=d.done_i=d.poll_i=d.batch_active_i=0;
        d.ar_valid_i=d.aw_valid_i=d.ar_ready_i=d.aw_ready_i=0;
        d.rlast_fire_i=d.b_fire_i=d.bad_id_i=0;
        d.shared_ar_valid_i=d.shared_aw_valid_i=d.shared_ar_ready_i=d.shared_aw_ready_i=0;
        for(int i=0;i<4;i++)d.issue_id_i[i]=d.done_id_i[i]=d.poll_id_i[i]=0;
        d.rst_ni=0;tick(2);d.rst_ni=1;tick();
    }
    Bench(){reset();}
    void write(unsigned addr,uint64_t v){d.cfg_valid_i=d.cfg_write_i=1;d.cfg_addr_i=addr;d.cfg_wdata_i=v;tick();d.cfg_valid_i=d.cfg_write_i=0;d.eval();}
    uint64_t read(unsigned addr){d.cfg_addr_i=addr;d.eval();return d.cfg_rdata_o;}
    uint64_t status(){return read(0x60);}
    uint64_t count(unsigned i){write(0x70,i);return read(0x78);}
    uint64_t bank(unsigned i){write(0x70,448+i);return read(0x78);}
    // P4M arm: bit30 set, batch required, victim must be 0, optional aggressor must be 1.
    void arm_p4m(unsigned victim,int aggressor,unsigned target){
        d.batch_active_i=3;
        write(0x68,target);
        uint64_t w=1ULL|(1ULL<<30)|(1ULL<<25)|(uint64_t(victim)<<8);
        if(aggressor>=0)w|=(uint64_t(aggressor)<<16)|(1ULL<<24);
        write(0x60,w);
        check(status()==1,"P4M arm");
    }
    void issue(unsigned v,uint64_t id){d.issue_i=1<<v;d.issue_id_i[v]=id;tick();d.issue_i=0;}
    void done(unsigned v,uint64_t id){d.done_i=1<<v;d.done_id_i[v]=id;tick();d.done_i=0;}
    void poll(unsigned v,uint64_t id){d.poll_i=1<<v;d.poll_id_i[v]=id;tick();d.poll_i=0;}
    void freeze(unsigned v,uint64_t last){check(status()==2,"P4M awaits final poll");poll(v,last);check(status()==4,"P4M frozen valid");}
};

int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
    Bench b;
    check(b.read(0x58)==0x49444d414f425307ULL,"P4M version magic");
    check(b.bank(0)==0x49444d4150344d01ULL,"P4M bank capability");
    cases++;
    if(argc>1 && std::string(argv[1])=="small") {
        // Counter overflow inside a P4M window must invalidate the frozen bank.
        b.arm_p4m(0,-1,1);
        b.issue(0,0x41);
        b.tick(300);
        b.done(0,0x41);
        b.freeze(0,0x41);
        check(((b.status()>>32)&4)!=0,"timing overflow");
        check(((b.status()>>40)&1)!=0,"overflow invalidates P4M");
        check((b.bank(12)&8)!=0,"overflow evidence in bank word 12");
        check(b.bank(1)==b.count(0),"P4M cycles equal frozen count0");
        cases++;
    } else {
        // Solo run: no aggressor, two victim jobs, opening edge excluded, closing edge included.
        b.reset();
        b.arm_p4m(0,-1,2);
        b.issue(0,101);
        b.tick(4);
        b.done(0,101);
        b.tick(3);
        b.issue(0,102);
        b.tick(2);
        b.done(0,102);
        b.freeze(0,102);
        check(b.bank(1)==12,"solo window cycles");
        check(b.bank(2)==0&&b.bank(3)==0&&b.bank(4)==0&&b.bank(5)==0,"solo no aggressor flow");
        check(b.bank(6)==0&&b.bank(7)==0&&b.bank(8)==0&&b.bank(9)==0,"solo no aggressor ids");
        check(b.bank(10)==0&&b.bank(11)==0&&b.bank(12)==0,"solo clean bank");
        check(b.bank(13)==1&&b.bank(14)==2,"solo victim flow");
        check(b.bank(15)==2,"solo flags: closed without aggressor");
        cases++;

        // Aggressor carry-in: issued strictly before the opening edge, retired inside the window.
        b.reset();
        b.arm_p4m(0,1,2);
        b.issue(1,900);
        b.issue(0,201);
        b.tick(5);
        b.done(1,900);
        b.tick(3);
        b.issue(1,901);
        b.tick(2);
        b.done(0,201);
        b.tick(2);
        b.issue(0,202);
        b.tick(2);
        b.done(1,901);
        b.tick(1);
        b.done(0,202);
        b.freeze(0,202);
        check(b.bank(1)==21,"carry window cycles");
        check(b.bank(2)==1&&b.bank(3)==2,"in-window aggressor flow");
        check(b.bank(4)==1&&b.bank(5)==0,"opening/closing stock");
        check(b.bank(6)==1,"carry done counted once");
        check(b.bank(7)==0,"no opening-edge aggressor issue");
        check(b.bank(8)==900&&b.bank(9)==0,"opening/closing id");
        check(b.bank(10)==900&&b.bank(11)==901,"first/last aggressor done id");
        check(b.bank(12)==0,"carry run clean");
        check(b.bank(13)==1&&b.bank(14)==2,"carry victim flow");
        check(b.bank(15)==3,"carry cleared, closed, has aggressor");
        check(b.bank(4)+b.bank(2)==b.bank(3)+b.bank(5),"aggressor conservation");
        check(b.bank(6)<=b.bank(4)&&b.bank(6)<=b.bank(3)&&b.bank(6)<=1,"carry bound");
        cases++;

        // Opening edge: aggressor done (retiring the carry) and aggressor issue are excluded.
        b.reset();
        b.arm_p4m(0,1,1);
        b.issue(1,910);
        d.issue_i=3; d.issue_id_i[0]=300; d.done_i=2; d.done_id_i[1]=910; d.issue_id_i[1]=911;
        b.tick(); d.issue_i=0; d.done_i=0;
        b.tick(4);
        b.done(1,911);
        b.tick(2);
        b.done(0,300);
        b.freeze(0,300);
        check(b.bank(2)==0&&b.bank(3)==1,"opening-edge aggressor events excluded");
        check(b.bank(4)==1&&b.bank(7)==1&&b.bank(8)==911,"post-edge opening stock and edge issue");
        check(b.bank(5)==0&&b.bank(6)==0&&b.bank(9)==0,"retire-and-replace leaves no carry");
        check(b.bank(12)==0,"opening-edge run clean");
        cases++;

        // Simultaneous prior-job done and new issue inside the window: retire, then accept.
        b.reset();
        b.arm_p4m(0,1,1);
        b.issue(0,400);
        b.tick(3);
        b.issue(1,500);
        b.tick(2);
        d.done_i=2; d.done_id_i[1]=500; d.issue_i=2; d.issue_id_i[1]=501;
        b.tick(); d.done_i=0; d.issue_i=0;
        b.tick(2);
        b.done(0,400);
        b.freeze(0,400);
        check(b.bank(2)==2&&b.bank(3)==1,"retire then new counts both");
        check(b.bank(4)==0&&b.bank(5)==1&&b.bank(9)==501,"new job is the closing stock");
        check(b.bank(6)==0&&b.bank(7)==0&&b.bank(8)==0,"no carry cohort");
        check(b.bank(10)==500&&b.bank(11)==500,"first/last done id");
        check(b.bank(12)==0,"hand-off is not an overlap");
        cases++;

        // Closing edge: victim done, aggressor done and aggressor issue are all included.
        b.reset();
        b.arm_p4m(0,1,1);
        b.issue(0,600);
        b.tick(3);
        b.issue(1,700);
        b.tick(2);
        d.done_i=3; d.done_id_i[0]=600; d.done_id_i[1]=700; d.issue_i=2; d.issue_id_i[1]=701;
        b.tick(); d.done_i=0; d.issue_i=0;
        b.freeze(0,600);
        check(b.bank(2)==2&&b.bank(3)==1,"closing-edge aggressor events included");
        check(b.bank(4)==0&&b.bank(5)==1&&b.bank(9)==701,"closing stock after edge");
        check(b.bank(13)==0&&b.bank(14)==1,"closing-edge victim done included");
        check(b.bank(12)==0,"closing-edge run clean");
        cases++;

        // Aggressor idle gap across the window plus a multi-job refill sequence.
        b.reset();
        b.arm_p4m(0,1,3);
        b.issue(0,1);
        b.tick(2);
        b.issue(1,50);
        b.tick(6);
        b.done(1,50);
        b.tick(4);
        b.done(0,1);
        b.tick(2);
        b.issue(0,2);
        b.tick(3);
        b.done(0,2);
        b.tick(3);
        b.issue(0,3);
        b.tick(2);
        b.done(0,3);
        b.freeze(0,3);
        check(b.bank(13)==2&&b.bank(14)==3,"multi-job victim flow across refills");
        check(b.bank(2)==1&&b.bank(3)==1,"aggressor flow across an idle gap");
        check(b.bank(4)==0&&b.bank(5)==0&&b.bank(6)==0,"aggressor idle at opening and close");
        check(b.bank(12)==0,"idle gap run clean");
        cases++;

        // Frozen bank survives post-freeze drain and AXI activity.
        b.reset();
        b.arm_p4m(0,1,1);
        b.issue(0,800);
        b.tick(3);
        b.issue(1,801);
        b.tick(2);
        b.done(0,800);
        b.freeze(0,800);
        std::array<uint64_t,16> frozen;
        for(unsigned i=0;i<16;i++)frozen[i]=b.bank(i);
        b.d.issue_i=2; b.d.issue_id_i[1]=999; b.tick(); b.d.issue_i=0;
        b.done(1,801);
        b.d.issue_i=2; b.d.issue_id_i[1]=1000; b.tick(); b.d.issue_i=0;
        b.d.done_i=2; b.d.done_id_i[1]=1000; b.tick(); b.d.done_i=0;
        b.d.ar_valid_i=b.d.aw_valid_i=b.d.ar_ready_i=b.d.aw_ready_i=3; b.tick(4);
        b.d.ar_valid_i=b.d.aw_valid_i=b.d.ar_ready_i=b.d.aw_ready_i=0;
        for(unsigned i=0;i<16;i++)check(b.bank(i)==frozen[i],"frozen P4M bank after drain");
        check(b.count(0)==frozen[1],"frozen count0 matches bank cycles");
        cases++;

        // Aggressor lane violations.
        b.reset(); b.arm_p4m(0,1,1); b.issue(0,9); b.tick(2); b.issue(1,60); b.tick(2);
        b.done(1,61);
        check((b.bank(12)&2)!=0,"aggressor done with wrong id");
        check(((b.status()>>40)&1)!=0,"wrong id invalidates P4M");
        cases++;
        b.reset(); b.arm_p4m(0,1,1); b.issue(0,10); b.tick(2); b.done(1,70);
        check((b.bank(12)&2)!=0,"aggressor done without issue");
        cases++;
        b.reset(); b.arm_p4m(0,1,1); b.issue(0,11); b.tick(2); b.issue(1,71); b.tick(1); b.issue(1,72);
        check((b.bank(12)&4)!=0,"overlapping aggressor descriptors");
        check(((b.status()>>40)&1)!=0,"overlap invalidates P4M");
        cases++;
        b.reset(); b.arm_p4m(0,1,1); b.issue(0,12); b.tick(2); b.issue(1,0);
        check((b.bank(12)&1)!=0,"zero aggressor id");
        cases++;

        // Abort while the window is open: frozen for evidence, never valid.
        b.reset(); b.arm_p4m(0,-1,2); b.issue(0,20); b.tick(3);
        b.write(0x60,2);
        check(b.status()==(4|(1ULL<<36)|(1ULL<<40)),"aborted P4M is frozen and invalid");
        check((b.bank(15)&2)==0,"aborted window is not closed");
        check((b.bank(12)&16)!=0,"abort evidence in bank word 12");
        cases++;

        // Illegal P4M arm combinations.
        b.reset(); b.write(0x68,1); b.write(0x60,1ULL|(1ULL<<30)|(1ULL<<25)|(1ULL<<27)); check(b.status()==(1ULL<<32),"P4M forbids the E3 bit");
        b.reset(); b.write(0x68,1); b.write(0x60,1ULL|(1ULL<<30)|(1ULL<<25)|(1ULL<<28)); check(b.status()==(1ULL<<32),"P4M forbids the delay bit");
        b.reset(); b.write(0x68,1); b.write(0x60,1ULL|(1ULL<<30)|(1ULL<<25)|(1ULL<<29)); check(b.status()==(1ULL<<32),"P4M forbids the finite bit");
        b.reset(); b.write(0x68,1); b.write(0x60,1ULL|(1ULL<<30)); check(b.status()==(1ULL<<32),"P4M requires batch");
        b.reset(); b.write(0x68,1); b.write(0x60,1ULL|(1ULL<<30)|(1ULL<<25)|(1ULL<<8)); check(b.status()==(1ULL<<32),"P4M requires victim 0");
        b.reset(); b.write(0x68,1); b.write(0x60,1ULL|(1ULL<<30)|(1ULL<<25)|(1ULL<<24)|(2ULL<<16)); check(b.status()==(1ULL<<32),"P4M requires aggressor 1");
        b.reset(); b.write(0x68,1); b.write(0x60,1ULL|(1ULL<<30)|(1ULL<<25)|(1ULL<<26)); check(b.status()==(1ULL<<32),"path bit needs PathEnable");
        b.reset(); b.write(0x70,464); check(b.status()==(1ULL<<32),"P4M index range ends at 463");
        cases+=8;

        // Legacy arm modes stay available when P4MEnable is 1.
        b.reset(); b.d.batch_active_i=3; b.write(0x68,1); b.write(0x60,1ULL|(1ULL<<25));
        check(b.status()==1,"legacy batch arm retained");
        b.issue(0,1); b.tick(2); b.done(0,1);
        check(b.status()==2,"legacy batch awaits the final poll");
        b.poll(0,1);
        check(b.status()==4,"legacy batch frozen");
        check(((b.status()>>40)&1)==0,"legacy batch leaves P4M validity alone");
        cases++;
    }
    std::cout<<"PASS "<<cases<<" P4M observer scenarios (no board claim)\n";return 0;
}catch(const std::exception&e){std::cerr<<"FAIL after "<<cases<<": "<<e.what()<<"\n";return 1;}}
