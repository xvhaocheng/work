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
        check(b.bank(13)==2&&b.bank(13)==b.count(7),"solo V issued counts the opening job and matches legacy count7");
        check(b.bank(14)==2&&b.bank(14)==b.count(8),"solo V done matches legacy count8 and the target");
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
        check(b.bank(13)==2&&b.bank(13)==b.count(7),"carry victim flow matches legacy count7");
        check(b.bank(14)==2&&b.bank(14)==b.count(8),"carry victim done matches legacy count8");
        check(b.bank(15)==7,"immutable pre-opening cohort flag, closed, has aggressor");
        check(b.bank(4)+b.bank(2)==b.bank(3)+b.bank(5),"aggressor conservation");
        check(b.bank(6)<=b.bank(4)&&b.bank(6)<=b.bank(3)&&b.bank(6)<=1,"carry bound");
        cases++;

        // Opening edge: aggressor done (retiring the carry) and aggressor issue are excluded.
        b.reset();
        b.arm_p4m(0,1,1);
        b.issue(1,910);
        b.d.issue_i=3; b.d.issue_id_i[0]=300; b.d.done_i=2; b.d.done_id_i[1]=910; b.d.issue_id_i[1]=911;
        b.tick(); b.d.issue_i=0; b.d.done_i=0;
        b.tick(4);
        b.done(1,911);
        b.tick(2);
        b.done(0,300);
        b.freeze(0,300);
        check(b.bank(2)==0&&b.bank(3)==1,"opening-edge aggressor events excluded");
        check(b.bank(4)==1&&b.bank(7)==1&&b.bank(8)==911,"post-edge opening stock and edge issue");
        check(b.bank(5)==0&&b.bank(6)==0&&b.bank(9)==0,"retire-and-replace leaves no carry");
        check((b.bank(15)&4)==0,"opening-edge issue leaves no strict pre-opening cohort flag");
        check(b.bank(12)==0,"opening-edge run clean");
        cases++;

        // Simultaneous prior-job done and new issue inside the window: retire, then accept.
        b.reset();
        b.arm_p4m(0,1,1);
        b.issue(0,400);
        b.tick(3);
        b.issue(1,500);
        b.tick(2);
        b.d.done_i=2; b.d.done_id_i[1]=500; b.d.issue_i=2; b.d.issue_id_i[1]=501;
        b.tick(); b.d.done_i=0; b.d.issue_i=0;
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
        b.d.done_i=3; b.d.done_id_i[0]=600; b.d.done_id_i[1]=700; b.d.issue_i=2; b.d.issue_id_i[1]=701;
        b.tick(); b.d.done_i=0; b.d.issue_i=0;
        b.freeze(0,600);
        check(b.bank(2)==2&&b.bank(3)==1,"closing-edge aggressor events included");
        check(b.bank(4)==0&&b.bank(5)==1&&b.bank(9)==701,"closing stock after edge");
        check(b.bank(13)==1&&b.bank(14)==1,"closing-edge victim done included and opening issue counted");
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
        check(b.bank(13)==3&&b.bank(14)==3,"multi-job victim flow across refills counts the opening job");
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

        // An A job issued before ARM on the lane a later arm selects stays tracked: it is the strict
        // pre-opening cohort and its single done is recorded inside the window.
        b.reset();
        b.issue(1,1100);
        b.arm_p4m(0,1,1);
        b.issue(0,1101);
        b.tick(2);
        b.done(1,1100);
        b.tick(1);
        b.done(0,1101);
        b.freeze(0,1101);
        check(b.bank(4)==1&&b.bank(8)==1100,"pre-ARM A job is the opening cohort stock");
        check(b.bank(6)==1&&(b.bank(15)&4)!=0,"cohort done counted once and flag held at close");
        check(b.bank(12)==0,"pre-ARM cohort run is clean");
        cases++;

        // Malformed evidence on the lane a later arm selects must still invalidate the run.
        b.reset();
        b.issue(1,0);
        b.arm_p4m(0,1,1);
        b.issue(0,1200);
        b.tick(2);
        b.done(0,1200);
        b.freeze(0,1200);
        check((b.bank(12)&1)!=0,"pre-ARM zero id on the selected lane is reported");
        check(((b.status()>>40)&1)!=0,"selected pre-ARM error invalidates P4M");
        cases++;

        // Malformed evidence on a lane the arm does not select must be ignored.
        b.reset();
        b.issue(2,0);
        b.arm_p4m(0,-1,1);
        b.issue(0,1300);
        b.tick(3);
        b.done(0,1300);
        b.freeze(0,1300);
        check(b.bank(12)==0,"unselected lane error ignored");
        check(((b.status()>>40)&1)==0,"unselected lane error leaves P4M valid");
        check(b.bank(15)==2,"solo flags: no aggressor and closed");
        cases++;

        // Legacy roles first, then a P4M arm: only the newly selected lanes are sampled.
        b.reset();
        b.d.batch_active_i=15;
        b.write(0x68,1);
        b.write(0x60,1ULL|(1ULL<<25)|(2ULL<<8)|(1ULL<<24)|(3ULL<<16));
        check(b.status()==1,"legacy V2/A3 batch arm");
        b.issue(2,50); b.tick(2); b.done(2,50); b.poll(2,50);
        check(b.status()==4,"legacy V2/A3 run frozen");
        b.arm_p4m(0,1,1);
        b.issue(1,60);
        b.issue(0,61);
        b.tick(2);
        b.done(0,61);
        b.freeze(0,61);
        check(b.bank(4)==1&&b.bank(8)==60,"P4M samples the newly selected A lane");
        check(b.bank(2)==0&&b.bank(3)==0&&b.bank(12)==0,"legacy roles leave no P4M window evidence");
        cases++;

        // Corun then Solo without reset: Solo A fields stay zero even though the A lane still holds
        // the corun job in flight.
        b.reset();
        b.arm_p4m(0,1,1);
        b.issue(1,1300);
        b.issue(0,1301);
        b.tick(2);
        b.done(0,1301);
        b.freeze(0,1301);
        check(b.bank(5)==1&&b.bank(9)==1300,"corun closes with the A job still in flight");
        b.arm_p4m(0,-1,1);
        b.issue(0,1310);
        b.tick(3);
        b.done(0,1310);
        b.freeze(0,1310);
        check(b.bank(2)==0&&b.bank(3)==0&&b.bank(4)==0&&b.bank(5)==0,"solo opening/closing stock zero");
        check(b.bank(6)==0&&b.bank(7)==0&&b.bank(8)==0&&b.bank(9)==0,"solo id fields zero");
        check(b.bank(10)==0&&b.bank(11)==0&&b.bank(12)==0,"solo done ids and errors zero");
        check(b.bank(13)==1&&b.bank(14)==1,"solo V counts one issue and one done");
        check(b.bank(15)==2,"solo flags: closed without aggressor after corun");
        cases++;

        // Aborted window: the frozen bank must not move under late events.
        b.reset();
        b.arm_p4m(0,1,2);
        b.issue(1,1400);
        b.issue(0,1401);
        b.tick(3);
        b.done(1,1400);
        b.write(0x60,2);
        check(b.status()==(4|(1ULL<<36)|(1ULL<<40)),"aborted corun frozen and invalid");
        check((b.bank(12)&16)!=0,"abort evidence in bank word 12");
        std::array<uint64_t,16> aborted;
        for(unsigned i=0;i<16;i++)aborted[i]=b.bank(i);
        b.d.issue_i=2; b.d.issue_id_i[1]=1402; b.tick(); b.d.issue_i=0;
        b.d.done_i=2; b.d.done_id_i[1]=1403; b.tick(); b.d.done_i=0;
        b.d.issue_i=1; b.d.issue_id_i[0]=1404; b.tick(); b.d.issue_i=0;
        b.d.ar_valid_i=b.d.aw_valid_i=b.d.ar_ready_i=b.d.aw_ready_i=3; b.tick(3);
        b.d.ar_valid_i=b.d.aw_valid_i=b.d.ar_ready_i=b.d.aw_ready_i=0;
        for(unsigned i=0;i<16;i++)check(b.bank(i)==aborted[i],"aborted bank stable under late events");
        check(((b.status()>>40)&1)!=0,"aborted run stays invalid");
        cases++;

        // CLEAR drops the P4M error word and error_q[8]; cleared evidence must not re-assert.
        b.reset();
        b.arm_p4m(0,1,1);
        b.issue(0,1500);
        b.tick(2);
        b.done(1,1501);
        check((b.bank(12)&2)!=0,"unmatched A done before the clear");
        check(((b.status()>>40)&1)!=0,"unmatched A done invalidates before the clear");
        b.write(0x60,2);
        b.write(0x60,4);
        check(b.status()==0,"clear returns the observer to idle");
        check(((b.status()>>40)&1)==0,"clear drops error_q[8]");
        check(b.bank(12)==0,"clear drops the P4M error word");
        b.arm_p4m(0,1,1);
        check(((b.status()>>40)&1)==0&&b.bank(12)==0,"cleared evidence does not re-assert on the next arm");
        cases++;
    }
    std::cout<<"PASS "<<cases<<" P4M observer scenarios (no board claim)\n";return 0;
}catch(const std::exception&e){std::cerr<<"FAIL after "<<cases<<": "<<e.what()<<"\n";return 1;}}
