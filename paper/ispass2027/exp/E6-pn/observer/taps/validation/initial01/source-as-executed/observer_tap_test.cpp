#include "Vobserver_tap_top.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
static uint64_t checks=0, comparisons=0, cases=0;
static void ck(bool ok,const std::string&m){++checks;if(!ok)throw std::runtime_error(m);}
struct Bench {
    Vobserver_tap_top d;std::string name;uint64_t cycle=0,arms=0,targets=0;unsigned v=0,a=1;
    explicit Bench(std::string label):name(label){
        d.rst_ni=0;d.cfg_valid_i=d.cfg_write_i=0;d.cfg_addr_i=d.cfg_wdata_i=0;d.cfg_wstrb_i=255;
        d.issue_i=d.done_i=d.poll_i=d.batch_active_i=0;d.event_id_i=0;d.axi_stage_i=d.axi_port_i=d.axi_write_i=0;
        tick();d.rst_ni=1;tick();
    }
    void eval(){d.clk_i=0;d.eval();}
    void sample(){
        ck(d.gold_match_o,name+": old cfg/counter/state/ID/stock differs from8aac gold");++comparisons;
        ck(d.default_zero_o,name+": default-disabled taps not all zero");
        ck(bool(d.armed_o)==(d.phase_o==1),name+": actual ARMED fact");
        ck(bool(d.active_o)==(d.phase_o==2||d.phase_o==3||d.phase_o==4),name+": actual active excludes read tail");
        ck(bool(d.busy_o)==(d.active_o||d.phase_o==6),name+": original busy fact");
    }
    void tick(){
        eval();sample();
        if(d.rst_ni){
            arms+=d.arm_o;targets+=d.target_o;
            std::cout<<"{\"fixture_only\":true,\"source_board_cases\":0,\"case\":\""<<name<<"\",\"cycle\":"<<cycle
                <<",\"phase\":"<<unsigned(d.phase_o)<<",\"error\":"<<d.error_o<<",\"arm\":"<<unsigned(d.arm_o)
                <<",\"victim\":"<<unsigned(d.victim_o)<<",\"aggressor\":"<<unsigned(d.aggressor_o)<<",\"has_a\":"<<unsigned(d.has_a_o)
                <<",\"active\":"<<unsigned(d.active_o)<<",\"target\":"<<unsigned(d.target_o)<<",\"armed\":"<<unsigned(d.armed_o)
                <<",\"busy\":"<<unsigned(d.busy_o)<<",\"issue\":"<<unsigned(d.issue_i)<<",\"done\":"<<unsigned(d.done_i)
                <<",\"poll\":"<<unsigned(d.poll_i)<<",\"event_id\":"<<d.event_id_i<<",\"cfg_write\":"<<unsigned(d.cfg_write_i)
                <<",\"cfg_addr\":"<<unsigned(d.cfg_addr_i)<<",\"cfg_wdata\":"<<d.cfg_wdata_i<<",\"cfg_rdata\":"<<d.cfg_rdata_o<<"}\n";
        }
        d.clk_i=1;d.eval();++cycle;eval();sample();
    }
    uint64_t cfg(unsigned addr,bool write=false,uint64_t data=0,unsigned strobe=255){
        d.cfg_valid_i=1;d.cfg_write_i=write;d.cfg_addr_i=addr;d.cfg_wdata_i=data;d.cfg_wstrb_i=strobe;
        eval();sample();const auto result=d.cfg_rdata_o;tick();d.cfg_valid_i=0;d.cfg_write_i=0;d.cfg_wstrb_i=255;eval();sample();return result;
    }
    static uint64_t command(unsigned victim,unsigned aggressor,bool hasa,bool batch,bool path){
        return 1ULL|(uint64_t(victim)<<8)|(uint64_t(aggressor)<<16)|(uint64_t(hasa)<<24)|(uint64_t(batch)<<25)|(uint64_t(path)<<26);
    }
    void arm(unsigned victim,unsigned aggressor,bool hasa,bool batch,bool path,unsigned target){
        v=victim;a=aggressor;cfg(0x68,true,target);
        d.cfg_valid_i=d.cfg_write_i=1;d.cfg_addr_i=0x60;d.cfg_wdata_i=command(v,a,hasa,batch,path);eval();sample();
        ck(d.arm_o&&d.victim_o==v&&d.aggressor_o==a&&bool(d.has_a_o)==hasa,name+": accepted roles valid on same ARM edge");
        tick();d.cfg_valid_i=d.cfg_write_i=0;eval();sample();
        ck(d.armed_o&&!d.active_o&&!d.busy_o&&!d.arm_o,name+": ARM distinct from active/busy");
        d.batch_active_i=hasa?((1U<<v)|(1U<<a)):(1U<<v);
    }
    void event(unsigned issue,unsigned done,unsigned poll,uint64_t id){
        d.issue_i=issue;d.done_i=done;d.poll_i=poll;d.event_id_i=id;eval();sample();tick();
        d.issue_i=d.done_i=d.poll_i=0;eval();sample();
    }
    uint64_t count(unsigned index){cfg(0x70,true,index);return cfg(0x78);}
    void done_case(){++cases;std::cout<<"{\"case\":\""<<name<<"\",\"status\":\"passed\",\"fixture_only\":true,\"source_board_cases\":0,\"arm_events\":"<<arms<<",\"target_events\":"<<targets<<"}\n";}
};

int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
    {
        Bench b("batch-PATH-same-edge-roles-full-read-windows");
        b.event(4,0,0,77);b.event(0,4,0,77); // OFF/warmup must not ARM/target/count.
        ck(!b.arms&&!b.targets&&b.count(7)==0&&b.count(8)==0,"warmup excluded");
        b.arm(2,3,true,true,true,2);
        b.d.issue_i=4;b.d.event_id_i=1;b.eval();ck(!b.d.active_o&&!b.d.target_o,"opening issue excluded from active");
        b.tick();b.d.issue_i=0;b.eval();ck(b.d.active_o,"active opens after issue");
        for(bool write:{false,true}){
            b.d.axi_write_i=write;b.d.axi_port_i=write?3:2;
            for(unsigned stage=1;stage<=3;++stage){b.d.axi_stage_i=stage;b.tick();}
            b.d.axi_stage_i=0;b.eval();
        }
        b.event(0,4,0,1);ck(!b.targets,"non-final done not target");b.event(4,0,0,2);
        b.d.done_i=4;b.d.event_id_i=2;b.eval();b.sample();ck(b.d.active_o&&b.d.target_o,"final target is actual done/close edge");
        b.tick();b.d.done_i=0;b.eval();b.sample();ck(!b.d.active_o&&b.d.busy_o&&!b.d.target_o&&b.d.phase_o==6,"read tail distinct from final done");
        // Every original scalar and all128 PATH read indices are compared at every half-edge.
        for(unsigned idx=0;idx<35;++idx)b.count(idx);
        for(unsigned idx=64;idx<192;++idx)b.count(idx);
        ck(b.d.error_o==0,"coherent synthetic PATH/private transactions preserve old checks");
        ck(b.count(7)==2&&b.count(8)==2&&b.targets==1,"original issue/completion sequence preserved");
        b.event(0,0,4,99);ck(b.d.phase_o==6&&!b.d.target_o,"wrong poll cannot freeze or target");
        b.event(0,0,4,2);ck(b.d.phase_o==5&&!b.d.busy_o&&!b.d.active_o,"only original matching poll freezes");
        ck((b.cfg(0x60)&7)==4,"original frozen/status window");
        b.cfg(0x70,true,192);ck(b.d.error_o&1,"old invalid INDEX192 remains invalid");
        b.done_case();
    }
    {
        Bench b("PATH-off-ARM-and-final-target");b.arm(1,0,false,true,false,1);
        b.event(2,0,0,1);b.d.done_i=2;b.d.event_id_i=1;b.eval();ck(b.d.target_o,"target without PATH still pulses");
        b.tick();b.d.done_i=0;b.event(0,0,2,1);ck(b.arms==1&&b.targets==1&&b.d.phase_o==5,"general ARM not conditional path_arm");b.done_case();
    }
    {
        Bench b("legacy-nonbatch-target-never-pulses");b.arm(0,1,false,false,false,2);
        b.event(1,0,0,1);b.event(0,1,0,1);ck(b.d.phase_o==3&&!b.targets,"legacy done enters poll wait, not batch target");
        b.event(0,0,1,1);b.event(1,0,0,2);b.event(0,1,0,2);b.event(0,0,1,2);
        ck(b.d.phase_o==5&&!b.targets&&b.count(9)==2,"legacy freezes by two polls without hardware batch target pulse");b.done_case();
    }
    for(unsigned kind=0;kind<4;++kind){
        Bench b("rejected-ARM-"+std::to_string(kind));b.cfg(0x68,true,kind==0?0:1);
        uint64_t cmd=Bench::command(0,1,true,true,false);
        if(kind==2)cmd|=1ULL<<63;
        if(kind==3)cmd=Bench::command(1,1,true,true,false);
        b.cfg(0x60,true,cmd,kind==1?15:255);
        ck(!b.arms&&!b.d.armed_o&&(b.d.error_o&1),"bad target/strobe/reserved/roles not accepted");b.done_case();
    }
    {
        Bench b("ARM-busy-rejection-does-not-change-roles");b.arm(2,3,true,true,false,1);b.event(4,0,0,1);
        b.cfg(0x60,true,Bench::command(0,1,false,true,false));
        ck(b.arms==1&&b.d.victim_o==2&&b.d.aggressor_o==3&&b.d.has_a_o,"rejected ARM keeps existing roles");
        b.cfg(0x60,true,2);ck(b.d.phase_o==5&&(b.d.error_o&16)&&!b.targets,"abort is not target done");b.done_case();
    }
    {
        Bench b("anomalous-final-ID-retains-original-target-semantics");b.arm(0,1,false,true,false,1);b.event(1,0,0,1);
        b.d.done_i=1;b.d.event_id_i=99;b.eval();ck(b.d.target_o,"original final-count transition still pulses on anomalous ID");
        b.tick();b.d.done_i=0;b.eval();ck((b.d.error_o&2)&&b.d.phase_o==6&&b.targets==1,"old ID error independently invalidates sample");
        b.event(0,0,1,1);ck((b.cfg(0x60)>>32)&2,"original public error gate retained; target alone is not success");b.done_case();
    }
    std::cout<<"{\"summary\":\"passed\",\"fixture_only\":true,\"source_board_cases\":0,\"cases\":"<<cases<<",\"checks\":"<<checks<<",\"gold_comparisons\":"<<comparisons<<"}\n";
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}return 0;}
