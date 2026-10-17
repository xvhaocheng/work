#include "Vfrontend_tap_top.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

static uint64_t checks=0, comparisons=0, cases=0, states=0;
static uint64_t sim_half_cycles=0;
double sc_time_stamp(){return static_cast<double>(sim_half_cycles);}
static void ck(bool ok,const std::string &message){++checks;if(!ok)throw std::runtime_error(message);}
struct Bench {
    Vfrontend_tap_top d;
    std::string name;
    uint64_t cycle=0,due=0,copies=0,done=0,retires=0,stop_edges=0;
    bool stalled=false;
    explicit Bench(std::string label):name(label){
        d.rst_ni=0;d.valid_i=d.write_i=d.complete_i=0;d.addr_i=d.wdata_i=0;d.wstrb_i=255;
        tick();d.rst_ni=1;tick();
    }
    void eval(){
        d.clk_i=0;d.ready_i=!stalled;d.complete_i=due&&cycle==due;d.backend_idle_i=!due;d.eval();
    }
    void sample(){
        ck(d.gold_match_o,name+": tap on/default old interface/state differs from dd1a gold");++comparisons;
        ck(d.default_zero_o,name+": omitted PnTapEnable outputs not zero");
        ck(bool(d.stop_o)==bool(d.valid_i&&d.write_i&&d.wstrb_i==255&&d.addr_i==0x88&&d.wdata_i==2),
           name+": STOP qualifier");
        ck(bool(d.quiet_o)==bool(d.state_o==0&&!d.offer_o&&!d.raw_launch_o&&!d.active_o),name+": local frontend quiet formula");
        if(d.rst_ni){states|=1U<<d.state_o;if(d.state_o)ck(!d.quiet_o,name+": COUNT/WAIT/COOLDOWN not quiet");}
    }
    void tick(){
        eval();sample();
        if(d.rst_ni){
            std::cout<<"{\"fixture_only\":true,\"source_board_cases\":0,\"case\":\""<<name<<"\",\"cycle\":"<<cycle
                <<",\"state\":"<<unsigned(d.state_o)<<",\"next\":"<<unsigned(d.next_state_o)<<",\"offer\":"<<unsigned(d.offer_o)
                <<",\"ready\":"<<unsigned(d.ready_i)<<",\"complete\":"<<unsigned(d.complete_i)<<",\"raw_launch\":"<<unsigned(d.raw_launch_o)
                <<",\"issue\":"<<unsigned(d.issue_o)<<",\"done\":"<<unsigned(d.done_o)<<",\"retire\":"<<unsigned(d.retire_o)
                <<",\"stop_tap\":"<<unsigned(d.stop_o)<<",\"quiet_tap\":"<<unsigned(d.quiet_o)<<",\"gap_remaining\":"<<d.gap_remaining_o
                <<",\"cfg_valid\":"<<unsigned(d.valid_i)<<",\"cfg_write\":"<<unsigned(d.write_i)<<",\"addr\":"<<unsigned(d.addr_i)
                <<",\"wstrb\":"<<unsigned(d.wstrb_i)<<",\"wdata\":"<<d.wdata_i<<"}\n";
            if(d.offer_o&&d.ready_i){ck(!due,name+": synthetic backend only one pending");++copies;due=cycle+6;}
            if(d.complete_i){ck(d.state_o==2&&d.done_o,name+": real frontend COPY completion in WAIT");++done;due=0;}
            if(d.retire_o)++retires;
            if(d.stop_o)++stop_edges;
        }
        ++sim_half_cycles;d.clk_i=1;d.eval();++sim_half_cycles;++cycle;eval();sample();
    }
    uint64_t access(unsigned addr,bool write=false,uint64_t data=0,unsigned strobe=255){
        d.addr_i=addr;d.write_i=write;d.wdata_i=data;d.wstrb_i=strobe;d.valid_i=1;eval();sample();
        ck(d.ready_o,name+": control ready");const auto value=d.rdata_o;tick();d.valid_i=0;d.wstrb_i=255;eval();sample();return value;
    }
    void configure(unsigned count,unsigned gap=0){
        access(0x88,true,0);access(0,true,0x10000);access(8,true,0x100000);
        access(16,true,4096);access(24,true,0);access(0x90,true,count);access(0x98,true,1);
        access(0xb0,true,gap);access(0x88,true,1);
        ck(d.quiet_o,name+": enabled but idle frontend is locally quiet");
    }
    void start(){access(0x28);ck(d.state_o==1&&!d.quiet_o,name+": start enters COUNT");}
    void until(unsigned state){for(unsigned i=0;i<100&&d.state_o!=state;++i)tick();ck(d.state_o==state,name+": bounded requested state");}
    void at_done(){for(unsigned i=0;i<100&&cycle!=due;++i)tick();eval();ck(d.complete_i&&d.state_o==2,name+": true completion edge");}
    void finish(unsigned expected){
        for(unsigned i=0;i<200&&(d.state_o||due);++i)tick();
        ck(d.state_o==0&&!due&&d.quiet_o,name+": fully retired frontend locally quiet");
        ck(copies==expected&&done==expected&&retires==1,name+": exact finite COPY/command accounting");
        ck(access(0xa0)==expected&&access(0xa8)==expected,name+": original count read windows");
        ck(access(0x30)!=0,name+": original command DONE id read");
        ++cases;std::cout<<"{\"case\":\""<<name<<"\",\"status\":\"passed\",\"fixture_only\":true,\"source_board_cases\":0,\"copies\":"<<copies<<",\"stop_edges\":"<<stop_edges<<"}\n";
    }
};

int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
    for(unsigned gap:{0U,4U}){
        Bench b("finite-gap"+std::to_string(gap));b.configure(3,gap);
        // IDLE NEXT_ID launch precedes the per-copy obs_issue by a state edge.
        b.d.addr_i=0x28;b.d.write_i=0;b.d.valid_i=1;b.eval();b.sample();
        ck(b.d.raw_launch_o&&!b.d.issue_o&&!b.d.offer_o&&!b.d.quiet_o,"raw launch prevents false quiet");
        b.tick();b.d.valid_i=0;b.stalled=true;b.eval();
        const auto src=b.d.src_o,dst=b.d.dst_o;
        for(unsigned i=0;i<5;++i){ck(b.d.offer_o&&!b.d.quiet_o&&b.d.src_o==src&&b.d.dst_o==dst,"held COUNT offer stable/not quiet");b.tick();}
        b.stalled=false;b.finish(3);
    }
    {
        Bench b("STOP-held-COUNT");b.configure(10,4);b.start();b.stalled=true;
        b.access(0x88,true,2);ck(b.d.offer_o&&!b.d.quiet_o,"STOP cannot erase held offer");
        for(unsigned i=0;i<3;++i)b.tick();b.stalled=false;b.finish(1);ck(b.stop_edges==1,"one actual STOP edge");
    }
    {
        Bench b("STOP-WAIT-before-done");b.configure(10,4);b.start();b.tick();
        ck(b.d.state_o==2,"WAIT reached");b.access(0x88,true,2);b.finish(1);
    }
    for(unsigned gap:{0U,4U}){
        Bench b("STOP-same-done-gap"+std::to_string(gap));b.configure(10,gap);b.start();b.tick();b.at_done();
        b.access(0x88,true,2);ck(!b.d.quiet_o,"same-edge STOP/done still COUNT or COOLDOWN");
        // Preserve dd1a behavior: gap0 has already entered the next COUNT;
        // nonzero gap can stop in COOLDOWN before a second task is offered.
        b.finish(gap?1:2);
    }
    for(bool expiry:{false,true}){
        Bench b(expiry?"STOP-COOLDOWN-expiry":"STOP-COOLDOWN-middle");
        b.configure(10,4);b.start();b.until(3);
        if(expiry){while(b.d.gap_remaining_o)b.tick();b.eval();}
        ck(b.d.state_o==3&&!b.d.quiet_o&&!b.d.offer_o&&!b.d.done_o,"cooldown has no offer but is not quiet");
        b.access(0x88,true,2);ck(b.d.state_o==0&&b.d.quiet_o,"accepted cooldown STOP returns to idle");
        b.finish(1);
    }
    for(unsigned kind=0;kind<5;++kind){
        Bench b("non-STOP-qualifier-"+std::to_string(kind));b.configure(1);b.start();b.stalled=true;
        if(kind==0)b.access(0x88,true,2,0x7f);
        if(kind==1)b.access(0x89,true,2);
        if(kind==2)b.access(0x88,false,2);
        if(kind==3)b.access(0x88,true,3);
        if(kind==4){b.d.addr_i=0x88;b.d.write_i=1;b.d.wdata_i=2;b.d.valid_i=0;b.eval();b.sample();b.tick();}
        ck(b.stop_edges==0,"partial/read/invalid-address/value/no-valid never STOP pulse");
        b.stalled=false;b.finish(1);
    }
    {
        Bench b("IDLE-repeated-STOP");b.access(0x88,true,2);b.access(0x88,true,2);
        ck(b.stop_edges==2&&b.d.quiet_o,"each full accepted STOP is an event, even if flag already1");
        ++cases;
    }
    ck(states==15,"all four frontend states actually observed");
    std::cout<<"{\"summary\":\"passed\",\"fixture_only\":true,\"source_board_cases\":0,\"cases\":"<<cases<<",\"checks\":"<<checks<<",\"gold_comparisons\":"<<comparisons<<",\"states_mask\":"<<states<<"}\n";
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}return 0;}
