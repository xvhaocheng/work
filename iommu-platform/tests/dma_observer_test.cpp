// Production observer regression, independently driven clock/event schedule.
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
    void arm(unsigned v=0,int a=-1,unsigned n=2,bool batch=false){write(0x68,n);write(0x60,1|(batch?(1ULL<<25):0)|(uint64_t(v)<<8)|(a<0?0:((uint64_t(a)<<16)|(1ULL<<24))));check(status()==1,"arm");}
    void issue(unsigned v,uint64_t id){d.issue_i=1<<v;d.issue_id_i[v]=id;tick();d.issue_i=0;}
    void done(unsigned v,uint64_t id){d.done_i=1<<v;d.done_id_i[v]=id;tick();d.done_i=0;}
    void poll(unsigned v,uint64_t id){d.poll_i=1<<v;d.poll_id_i[v]=id;tick();d.poll_i=0;}
    void body(unsigned v,uint64_t id,unsigned h,unsigned p){tick(h-1);done(v,id);tick(p-1);poll(v,id);}
    std::array<uint64_t,34> result(){std::array<uint64_t,34>r;for(unsigned i=0;i<34;i++){write(0x70,i);r[i]=read(0x78);}return r;}
    void finish(unsigned n){check(status()==4,"valid frozen status");auto r=result();check(r[7]==n&&r[8]==n&&r[9]==n,"task counts");check(r[0]==r[1]+r[2]+r[3],"H/P/R identity");check(r[0]==r[10]+r[11]+r[12]+r[13],"occupancy identity");for(int k=0;k<2;k++){auto start=r[30+k],end=r[32+k];check((start>>16)+r[14+6*k]==(end>>16)+r[16+6*k],"read conservation");check((start&65535)+r[15+6*k]==(end&65535)+r[17+6*k],"write conservation");}cases++;}
};
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
    Bench b;
    check(b.read(0x58)==0x49444d414f425302ULL,"version");
    if(argc>1 && std::string(argv[1])=="small") {
        b.arm(0,-1,1);b.issue(0,2);b.body(0,2,300,1);
        check((b.status()>>32)&4,"timing overflow must invalidate");cases++;
        b.reset();b.d.ar_valid_i=b.d.ar_ready_i=2;b.tick(8);b.d.ar_valid_i=b.d.ar_ready_i=0;
        b.arm(0,1,1);b.issue(0,2);b.body(0,2,2,1);
        check((b.status()>>32)&8,"outstanding overflow must invalidate");cases++;
    } else {
        for(unsigned v=0;v<4;v++) for(int a : {-1,int((v+1)%4)}) {
            b.reset();b.arm(v,a);b.issue(v,100);b.body(v,100,7,5);b.tick(8);b.issue(v,101);b.body(v,101,3,2);b.finish(2);
            auto r=b.result();check(r[0]==26&&r[1]==10&&r[2]==7&&r[3]==9,"known time intervals");
            check(r[4]==7&&r[5]==5&&r[6]==9,"interval maxima");auto saved=r;b.tick(10);check(b.result()==saved,"frozen results");
        }
        // Ignore stale DONE values, including a read concurrent with retirement.
        b.reset();b.arm(0,-1,1);b.issue(0,2);b.poll(0,1);b.d.poll_i=1;b.d.poll_id_i[0]=1;b.done(0,2);b.d.poll_i=0;b.poll(0,1);b.poll(0,2);b.finish(1);
        check(b.result()[1]==2&&b.result()[2]==2,"stale polling boundaries");
        // A has a warmup request in flight before ARM; include it in overlap.
        b.reset();b.d.ar_valid_i=b.d.ar_ready_i=2;b.tick();b.d.ar_valid_i=b.d.ar_ready_i=0;
        b.arm(0,1,1);b.issue(0,2);b.d.ar_valid_i=1;b.tick(3); // offered but not accepted
        b.d.ar_ready_i=1;b.tick();b.d.ar_valid_i=b.d.ar_ready_i=0;
        b.d.rlast_fire_i=3;b.tick();b.d.rlast_fire_i=0;b.done(0,2);b.poll(0,2);b.finish(1);
        {auto r=b.result();check(r[31]==65536&&r[33]==0,"carry warmup outstanding");check(r[13]>=4&&r[18]==3,"offered overlap and backpressure");}
        // Accepted requests on the start edge belong in start stock, not window flow.
        b.reset();b.arm(0,-1,1);b.d.ar_valid_i=b.d.ar_ready_i=1;b.issue(0,2);b.d.ar_valid_i=b.d.ar_ready_i=0;b.d.rlast_fire_i=1;b.tick();b.d.rlast_fire_i=0;b.done(0,2);b.poll(0,2);b.finish(1);
        check(b.result()[30]==65536,"same-edge opening stock");
        // Multiple transactions, simultaneous increments/retirements and write path.
        b.reset();b.arm(0,1,1);b.issue(0,2);b.d.ar_valid_i=b.d.ar_ready_i=b.d.aw_valid_i=b.d.aw_ready_i=3;b.tick(3);
        b.d.rlast_fire_i=b.d.b_fire_i=3;b.tick(2);b.d.ar_valid_i=b.d.ar_ready_i=b.d.aw_valid_i=b.d.aw_ready_i=0;b.tick(3);b.d.rlast_fire_i=b.d.b_fire_i=0;
        b.done(0,2);b.poll(0,2);b.finish(1);check(b.result()[14]==5&&b.result()[17]==5,"multi-transaction accounting");
        // Shared ingress accounting is separate from private-port admission.
        b.reset();b.arm(1,0,1);b.issue(1,3);b.d.shared_ar_valid_i=1;b.d.shared_aw_valid_i=1;b.tick(4);b.d.shared_ar_ready_i=b.d.shared_aw_ready_i=1;b.tick(2);b.d.shared_ar_valid_i=b.d.shared_aw_valid_i=0;b.done(1,3);b.poll(1,3);b.finish(1);
        {auto r=b.result();check(r[26]==2&&r[27]==2&&r[28]==4&&r[29]==4,"shared counts");}
        // In-flight A may remain at freeze; subsequent drain cannot alter snapshot.
        b.reset();b.arm(0,1,1);b.issue(0,2);b.d.aw_valid_i=b.d.aw_ready_i=2;b.tick();b.d.aw_valid_i=b.d.aw_ready_i=0;b.done(0,2);b.poll(0,2);b.finish(1);auto frozen=b.result();check(frozen[33]==1,"closing stock");b.d.b_fire_i=2;b.tick();b.d.b_fire_i=0;check(b.result()==frozen,"drain excluded");
        b.arm(0,1,1);b.issue(0,3);b.body(0,3,2,1);b.finish(1);check(b.result()[31]==0,"drain tracked while frozen");
        // Bad IDs, underflow and sequencing remain failures.
        b.reset();b.arm(0,-1,1);b.issue(0,2);b.done(0,3);b.poll(0,2);check((b.status()>>32)&2,"wrong retired ID");cases++;
        b.reset();b.arm(0,-1,1);b.issue(0,2);b.issue(0,3);check((b.status()>>32)&2,"overlapping descriptors");cases++;
        b.reset();b.d.b_fire_i=2;b.tick();b.d.b_fire_i=0;b.arm(0,1,1);b.issue(0,2);b.body(0,2,2,1);check((b.status()>>32)&8,"pre-arm underflow is not hidden");cases++;
        b.reset();b.arm(0,-1,1);b.issue(0,2);b.d.bad_id_i=1;b.tick();b.d.bad_id_i=0;b.body(0,2,2,1);check((b.status()>>32)&8,"AXI identity error");cases++;
        b.reset();b.arm(0,-1,1);b.write(0x60,2);check(b.status()==(4|(1ULL<<36)),"abort armed");cases++;
        b.reset();b.arm(0,-1,1);b.issue(0,2);b.write(0x60,2);check(b.status()==(4|(1ULL<<36)),"abort active");cases++;
        b.reset();b.write(0x68,0);b.write(0x60,1);check(b.status()==(1ULL<<32),"zero target rejected");cases++;
        b.reset();b.d.cfg_wstrb_i=1;b.write(0x68,1);check(b.status()==(1ULL<<32),"partial write rejected");cases++;
        b.reset();b.arm(0,-1,1);b.issue(0,2);b.write(0x60,1);check((b.status()>>32)&1,"rearm active rejected");cases++;
    }
    // Autonomous batches freeze at hardware completion, then measure one DONE read delay.
    for(unsigned role=0;role<2;role++) {
        b.reset(); b.d.batch_active_i=3; b.arm(role,1-role,2,true);
        b.issue(role,1); b.tick(4); b.done(role,1); b.tick(2);
        b.issue(role,2); b.tick(6); b.done(role,2);
        check(b.status()==2,"batch awaits final read");
        auto frozen=b.result(); b.tick(9); b.poll(role,2);
        check(b.status()==4,"batch frozen valid");
        auto r=b.result();
        check(r[0]==frozen[0]&&r[0]==r[1]+r[3]&&r[2]==0,"batch HW window excludes final polling");
        check(r[7]==2&&r[8]==2&&r[9]==1,"batch hardware counts and one poll");
        b.write(0x70,34); check(b.read(0x78)>9,"batch final read delay"); cases++;
    }
    b.reset(); b.d.batch_active_i=1; b.arm(0,1,1,true); b.issue(0,1); b.done(0,1); b.poll(0,1);
    check((b.status()>>32)&32,"A starvation marks batch invalid");cases++;
    b.reset(); b.arm(0,-1,1,true);b.issue(0,1);b.done(0,1);b.write(0x60,2);
    check((b.status()>>32)&16,"abort while awaiting batch read");cases++;
    std::cout<<"PASS "<<cases<<" production observer scenarios (no board claim)\n";return 0;
}catch(const std::exception&e){std::cerr<<"FAIL after "<<cases<<": "<<e.what()<<"\n";return 1;}}
