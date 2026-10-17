#include "Vdma_observer.h"
#include <verilated.h>
#include <cassert>
#include <cstdint>
#include <cstdio>

struct Bench {
    Vdma_observer d;
    Bench() { d.rst_ni=0; tick(); d.rst_ni=1; tick(); }
    void tick() { d.clk_i=0; d.eval(); d.clk_i=1; d.eval(); }
    void write(unsigned a, uint64_t v) {
        d.cfg_valid_i=1; d.cfg_write_i=1; d.cfg_wstrb_i=255;
        d.cfg_addr_i=a; d.cfg_wdata_i=v; tick(); d.cfg_valid_i=0; d.cfg_write_i=0;
    }
    uint64_t reg(unsigned a) {d.cfg_addr_i=a;d.eval();return d.cfg_rdata_o;}
    uint64_t get(unsigned lane,unsigned stage,unsigned field) {
        write(0x70,64+lane*32+stage*8+field);return reg(0x78);
    }
    uint64_t hist(unsigned lane,unsigned stage,unsigned bin) {
        write(0x70,256+lane*128+stage*32+bin); return reg(0x78);
    }
    void check_hist() {
        write(0x70,192); assert(reg(0x78)==0x0040000020202002ULL);
        for(unsigned lane=0;lane<4;lane++) for(unsigned stage=0;stage<4;stage++) {
            uint64_t total=0; for(unsigned bin=0;bin<32;bin++) total+=hist(lane,stage,bin);
            assert(total==get(lane,stage,1));
            std::printf("HIST lane=%u stage=%u full=%llu total=%llu\n",lane,stage,
                        (unsigned long long)get(lane,stage,1),(unsigned long long)total);
        }
    }
    void arm(unsigned v=0,unsigned a=1,bool pair=false) {
        write(0x60,4);write(0x68,1);write(0x60,1|(1u<<25)|(1u<<26)|(v<<8)|(a<<16)|(pair?1u<<24:0));
        assert(reg(0x60)==1);
    }
    void open(unsigned v=0) {d.issue_i=1<<v;d.issue_id_i[v]=1;tick();d.issue_i=0;}
    void close(unsigned v=0) {
        d.done_i=1<<v;d.done_id_i[v]=1;tick();d.done_i=0;
        d.poll_i=1<<v;d.poll_id_i[v]=1;tick();d.poll_i=0;
        if(reg(0x60)==4) check_hist();
    }
    void event(unsigned port,unsigned ch,unsigned step) {
        if(step==0) {if(ch) {d.aw_valid_i=1<<port;d.aw_ready_i=1<<port;}
                    else {d.ar_valid_i=1<<port;d.ar_ready_i=1<<port;}}
        if(step==1) {d.tr_id_i[ch]=port<<2;if(ch) d.shared_aw_valid_i=d.shared_aw_ready_i=1;
                    else d.shared_ar_valid_i=d.shared_ar_ready_i=1;}
        if(step==2) {d.out_id_i[ch]=port<<2;d.out_valid_i=1<<ch;}
        if(step==3) {d.out_id_i[ch]=port<<2;d.out_valid_i=1<<ch;d.out_ready_i=1<<ch;}
        if(step==4) {d.return_id_i[ch]=port<<2;d.return_i=1<<ch;
                    if(ch)d.b_fire_i=1<<port;else d.rlast_fire_i=1<<port;}
        tick();
        d.aw_valid_i=d.aw_ready_i=d.ar_valid_i=d.ar_ready_i=0;
        d.shared_ar_valid_i=d.shared_ar_ready_i=d.shared_aw_valid_i=d.shared_aw_ready_i=0;
        if(step!=2) d.out_valid_i=0;
        d.out_ready_i=d.return_i=d.b_fire_i=d.rlast_fire_i=0;
    }
};

int main(int argc,char**argv) {
    Verilated::commandArgs(argc,argv);
    unsigned cases=0;
    // Every source port, both channels, exact stage durations and repeated ARM.
    for(unsigned port=0;port<4;port++) {
        Bench b;assert(b.reg(0x58)==0x49444d414f425305ULL);
        for(unsigned repeat=0;repeat<2;repeat++) {
            b.arm(port);b.open(port);
            for(unsigned ch=0;ch<2;ch++) {
                b.event(port,ch,0);b.tick();b.event(port,ch,1);
                b.tick();b.tick();b.event(port,ch,2);
                b.tick();b.event(port,ch,3);b.tick();b.tick();b.tick();b.event(port,ch,4);
            }
            b.close(port);assert(b.reg(0x60)==4);
            const unsigned duration[4]={2,3,2,4};
            for(unsigned ch=0;ch<2;ch++) for(unsigned s=0;s<4;s++) {
                assert(b.get(ch,s,0)==1 && b.get(ch,s,1)==1);
                assert(b.get(ch,s,2)==0 && b.get(ch,s,5)==0 && b.get(ch,s,6)==0);
                assert(b.get(ch,s,3)==duration[s] && b.get(ch,s,4)==duration[s]);
                assert(b.get(ch,s,7)==duration[s]);
                unsigned bin=0, age=duration[s]; while(age){++bin;age>>=1;}
                for(unsigned k=0;k<32;k++) assert(b.hist(ch,s,k)==(k==bin?1:0));
            }
            cases++;
        }
    }
    // Four requests outstanding with the same ID; ordered stage matching.
    {
        Bench b;b.arm();b.open();
        for(unsigned i=0;i<4;i++)b.event(0,0,0);
        for(unsigned i=0;i<4;i++)b.event(0,0,1);
        for(unsigned i=0;i<4;i++){b.event(0,0,2);b.event(0,0,3);}
        for(unsigned i=0;i<4;i++)b.event(0,0,4);
        b.close();assert(b.reg(0x60)==4);
        for(unsigned s=0;s<4;s++){assert(b.get(0,s,1)==4);assert(b.get(0,s,7)==b.get(0,s,3));}
        cases++;
    }
    // A carry-in and carry-out, with separate source IDs and stages.
    {
        Bench b;b.arm(0,1,true);b.d.batch_active_i=2;
        b.event(1,0,0);b.open();b.event(1,0,1);
        b.event(1,0,2);b.event(1,0,3);b.event(1,0,4);
        b.event(1,0,0);b.close();assert(b.reg(0x60)==4);
        assert(b.get(2,0,5)==1 && b.get(2,0,6)==1);
        assert(b.get(2,0,0)==1 && b.get(2,0,1)==0 && b.get(2,0,2)==1);
        for(unsigned s=1;s<4;s++)assert(b.get(2,s,1)==1);
        auto frozen=b.get(2,0,7);b.event(1,0,1);assert(b.get(2,0,7)==frozen);
        cases++;
    }
    // Simultaneous zero-cycle stages and both read/write channels.
    {
        Bench b;b.arm();b.open();
        b.d.ar_valid_i=b.d.ar_ready_i=b.d.aw_valid_i=b.d.aw_ready_i=1;
        b.d.shared_ar_valid_i=b.d.shared_ar_ready_i=b.d.shared_aw_valid_i=b.d.shared_aw_ready_i=1;
        b.d.out_valid_i=b.d.out_ready_i=b.d.return_i=3;b.d.rlast_fire_i=b.d.b_fire_i=1;b.tick();
        b.d.ar_valid_i=b.d.ar_ready_i=b.d.aw_valid_i=b.d.aw_ready_i=0;
        b.d.shared_ar_valid_i=b.d.shared_ar_ready_i=b.d.shared_aw_valid_i=b.d.shared_aw_ready_i=0;
        b.d.out_valid_i=b.d.out_ready_i=b.d.return_i=0;b.d.rlast_fire_i=b.d.b_fire_i=0;
        b.close();assert(b.reg(0x60)==4);
        for(unsigned c=0;c<2;c++)for(unsigned s=0;s<4;s++) {
            assert(b.get(c,s,1)==1);assert(b.get(c,s,3)==0);assert(b.get(c,s,7)==0);
        }cases++;
    }
    // Private address held under backpressure is offered only once; mux admission
    // may precede shared TR admission. Interleave a different device and channel.
    {
        Bench b;b.arm(2,3,true);b.d.batch_active_i=8;b.open(2);
        b.d.ar_valid_i=4;b.tick();b.tick();b.tick();
        b.d.ar_ready_i=4;b.tick();b.d.ar_valid_i=b.d.ar_ready_i=0;
        b.event(3,1,0);b.event(3,1,1);b.event(2,0,1);
        b.event(3,1,2);b.event(3,1,3);b.event(2,0,2);b.event(2,0,3);
        b.event(3,1,4);b.event(2,0,4);b.close(2);
        assert(b.reg(0x60)==4);
        assert(b.get(0,0,1)==1 && b.get(0,0,3)==6);
        assert(b.get(3,0,1)==1 && b.get(3,0,3)==1);
        cases++;
    }
    // Sticky invalid: scoreboard overflow, unmatched return, unexpected private ID,
    // held output ID changes, and ARM with outstanding private traffic.
    for(unsigned fault=0;fault<5;fault++) {
        Bench b;
        if(fault==4)b.event(0,0,0);
        if(fault==4) {b.write(0x68,1);b.write(0x60,1|(1u<<25)|(1u<<26));}
        else {b.arm();b.open();}
        if(fault==0)for(unsigned i=0;i<65;i++)b.event(0,0,0);
        if(fault==1)b.event(0,0,4);
        if(fault==2){b.d.tr_id_i[0]=1;b.d.shared_ar_valid_i=1;b.tick();}
        if(fault==3){b.event(0,0,0);b.event(0,0,1);b.event(0,0,2);b.d.out_id_i[0]=4;b.tick();}
        assert(b.reg(0x60)&(1ULL<<38));cases++;
    }
    {
        Bench b; b.write(0x70,767); assert(b.reg(0x70)==767 && b.reg(0x78)==0);
        b.write(0x70,768); assert(b.reg(0x60)&(1ULL<<32));
        b.write(0x60,4); b.write(0x70,193); assert(b.reg(0x60)&(1ULL<<32)); cases++;
    }
    // Controlled event stress, not a backend in-flight limit measurement.
    // Every physical port, role, channel and all four stages. Repeated 33-row
    // waves cross the depth64 head/tail wrap without clearing the armed window.
    for(unsigned v=0;v<4;v++)for(unsigned role=0;role<2;role++)for(unsigned ch=0;ch<2;ch++) {
        const unsigned a=(v+1)%4,port=role?a:v,lane=role*2+ch;
        Bench b;b.arm(v,a,true);b.d.batch_active_i=1u<<a;b.open(v);
        for(unsigned repeat=0;repeat<3;repeat++) {
            for(unsigned i=0;i<33;i++)b.event(port,ch,0);
            assert(!(b.reg(0x60)&(1ULL<<38)));
            for(unsigned i=0;i<33;i++)b.event(port,ch,1);
            assert(!(b.reg(0x60)&(1ULL<<38)));
            for(unsigned i=0;i<33;i++){b.event(port,ch,2);b.event(port,ch,3);}
            for(unsigned i=0;i<33;i++)b.event(port,ch,4);
        }
        b.close(v);assert(b.reg(0x60)==4);b.check_hist();
        for(unsigned s=0;s<4;s++) {
            assert(b.get(lane,s,0)==99&&b.get(lane,s,1)==99);
            assert(b.get(lane,s,2)==0&&b.get(lane,s,5)==0&&b.get(lane,s,6)==0);
            assert(b.get(lane,s,3)==b.get(lane,s,7));
            std::printf("CAP64 port=%u role=%u channel=%u stage=%u full=%llu area=%llu sum=%llu\n",
                port,role,ch,s,(unsigned long long)b.get(lane,s,1),
                (unsigned long long)b.get(lane,s,7),(unsigned long long)b.get(lane,s,3));
        }
        cases++;
    }
    std::printf("PASS path observer: %u integration scenarios\n",cases);
}
