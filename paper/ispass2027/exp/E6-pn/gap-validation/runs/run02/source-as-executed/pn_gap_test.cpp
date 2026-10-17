#include "Vpn_gap_top.h"
#include "verilated.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static uint64_t default_cycles=0,enabled_cycles=0,cases=0;
static std::ofstream events("events.jsonl");
static void ck(bool value,const std::string& text){if(!value)throw std::runtime_error(text);}
struct Bench {
    Vpn_gap_top d;
    std::string name;
    uint64_t cycle=0,due=0,length=4096,functional_id=0;
    unsigned pages=1,retired=0,done_events=0;
    bool stall=false,backend_available=true,compare_enabled=false,batch=true;
    std::vector<uint64_t> issues,completions;
    explicit Bench(std::string label):name(std::move(label)){
        d.rst_ni=0;d.valid_i=d.write_i=d.complete_i=d.endpoint_force_i=0;
        d.addr_i=d.wdata_i=d.endpoint_count_i=0;d.wstrb_i=255;tick();d.rst_ni=1;tick();
    }
    void eval(){
        d.clk_i=0;d.ready_i=!stall;d.complete_i=due&&cycle==due;
        d.backend_idle_i=backend_available&&!due;d.eval();
    }
    void gold(){
        ck(d.default_gold_match_o,name+": default-disabled output differs from8aac gold");default_cycles++;
        if(compare_enabled){ck(d.enabled_gold_match_o,name+": enabled/gap0 output differs from8aac gold");enabled_cycles++;}
    }
    void tick(){
        eval();gold();
        if(d.rst_ni&&(d.valid_i||d.request_o||d.complete_i||d.retire_o||d.endpoint_force_i)){
            events<<"{\"case\":\""<<name<<"\",\"cycle\":"<<cycle<<",\"state\":"<<unsigned(d.state_o)
                <<",\"next_state\":"<<unsigned(d.next_state_o)<<",\"gap\":"<<d.gap_config_o<<",\"remaining\":"<<d.remaining_o
                <<",\"next_remaining\":"<<d.next_remaining_o<<",\"offer\":"<<unsigned(d.request_o)<<",\"ready\":"<<unsigned(d.ready_i)
                <<",\"true_done\":"<<unsigned(d.complete_i)<<",\"obs_done\":"<<unsigned(d.done_o)<<",\"command_retire\":"<<unsigned(d.retire_o)
                <<",\"mmio\":"<<unsigned(d.valid_i)<<",\"write\":"<<unsigned(d.write_i)<<",\"addr\":"<<unsigned(d.addr_i)
                <<",\"wdata\":"<<d.wdata_i<<",\"wstrb\":"<<unsigned(d.wstrb_i)<<",\"error\":"<<unsigned(d.error_o)
                <<",\"forced_endpoint\":"<<unsigned(d.endpoint_force_i)<<"}\n";
        }
        if(d.rst_ni&&d.request_o&&d.ready_i){
            ck(!due,name+": more than one task outstanding");
            ck(d.length_o==length,name+": actual descriptor length changed");
            uint64_t offset=batch?(issues.size()%pages)*4096ULL:0;
            ck(d.src_o==0x10000+offset&&d.dst_o==0x100000+offset,name+": actual descriptor ring offset differs");
            if(batch)ck(d.issue_o&&d.issue_id_o==issues.size()+1,name+": issue ID/event differs");
            issues.push_back(cycle);due=cycle+7;
        }
        if(d.complete_i){
            ck(d.state_o==2&&d.done_o,name+": true completion must be a WAIT COPY done");
            completions.push_back(cycle);due=0;
        }
        if(d.done_o&&d.rst_ni)done_events++;
        if(d.retire_o&&d.rst_ni)retired++;
        d.clk_i=1;d.eval();cycle++;eval();gold();
    }
    uint64_t access(unsigned addr,bool write=false,uint64_t value=0,bool error=false,unsigned strobe=255){
        d.addr_i=addr;d.write_i=write;d.wdata_i=value;d.wstrb_i=strobe;d.valid_i=1;eval();
        ck(d.ready_o&&bool(d.error_o)==error,name+": MMIO ready/error addr="+std::to_string(addr));
        uint64_t result=d.rdata_o;tick();d.valid_i=0;d.wstrb_i=255;eval();return result;
    }
    void gap(uint32_t value){access(0xb0,true,value);ck(access(0xb0)==value,name+": gap readback");}
    void configure(uint64_t count,unsigned geometry=1){
        batch=true;length=4096;pages=geometry;
        access(0x88,true,0);access(0,true,0x10000);access(8,true,0x100000);
        access(16,true,4096);access(24,true,0);access(0x90,true,count);access(0x98,true,pages);access(0x88,true,1);
    }
    void start(){functional_id=access(0x28);ck(functional_id!=0,name+": command ID nonzero");}
    void drain(unsigned budget=100000){
        for(unsigned i=0;i<budget&&(d.state_o||due);i++)tick();
        ck(d.state_o==0&&!due,name+": bounded drain failed");
    }
    void until_done(unsigned count){for(unsigned i=0;i<100000&&completions.size()<count;i++)tick();ck(completions.size()==count,name+": completion budget");}
    void at_true_done(){for(unsigned i=0;i<100&&cycle!=due;i++)tick();eval();ck(d.complete_i&&d.state_o==2,name+": true-done edge reached");}
    void finished(unsigned count,bool stopped=false){
        drain();ck(issues.size()==count&&completions.size()==count&&done_events==count,name+": exact COPY issue/done accounting");
        ck(retired==1,name+": command must retire once");
        if(batch){ck(access(0xa0)==count&&access(0xa8)==count,name+": exact registered issued/completed");
            auto status=access(0x88);ck(((status&7)==(stopped?5U:1U)),name+": active/stopped state");}
        ck(access(0x30)==functional_id,name+": actual idgen DONE must reach command ID");
        events<<"{\"case\":\""<<name<<"\",\"status\":\"passed\",\"cycles\":"<<cycle<<",\"copies\":"<<count<<",\"COPY_done_events\":"<<done_events<<",\"command_retires\":"<<retired<<"}\n";cases++;
    }
};

int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
    for(unsigned pages:{1U,8U})for(unsigned count:{1U,2U,9U}){
        Bench b("gap0-gold-p"+std::to_string(pages)+"-n"+std::to_string(count));b.compare_enabled=true;b.configure(count,pages);
        ck(b.access(0x80)==0x49444d4142544301ULL,"BTC1 unchanged");b.start();b.stall=true;
        for(unsigned i=0;i<5;i++){ck(b.d.request_o,"held initial offer");b.tick();}
        b.stall=false;b.finished(count);
        for(unsigned i=1;i<count;i++)ck(b.issues[i]-b.completions[i-1]==1,"gap0 original one COUNT cycle");
    }
    for(uint32_t gap:{1U,1000U,4000U})for(unsigned pages:{1U,8U}){
        Bench b("gap-real-"+std::to_string(gap)+"-p"+std::to_string(pages));b.gap(gap);b.configure(4,pages);b.start();
        auto start_cycle=b.cycle;b.finished(4);
        uint64_t refill=0;
        for(unsigned i=1;i<4;i++){auto interval=b.issues[i]-b.completions[i-1];ck(interval==uint64_t(gap)+1,"exact g extra cycles plus original COUNT");refill+=interval;}
        ck(b.issues[0]==start_cycle,"no gap before first offer");
        ck(refill==3*(uint64_t(gap)+1),"sum of done-to-next-accept intervals");
        ck(b.cycle-b.completions.back()<10,"no final cooldown");
        events<<"{\"case\":\""<<b.name<<"\",\"done_to_next_accept_sum\":"<<refill<<",\"intentional_added_cycles\":"<<3ULL*gap<<"}\n";
    }
    for(uint32_t gap:{0U,1U,1000U}){
        Bench b("WAIT-done-same-edge-STOP-g"+std::to_string(gap));
        if(gap)b.gap(gap);else b.compare_enabled=true;
        b.configure(6);b.start();b.at_true_done();b.access(0x88,true,2);b.finished(gap?1:2,true);
        events<<"{\"case\":\""<<b.name<<"\",\"boundary\":\"gap0 keeps original already-offered next COUNT;g>0 has no next offer\"}\n";
    }
    for(uint32_t gap:{1U,1000U,4000U}){
        Bench b("expiry-same-edge-STOP-g"+std::to_string(gap));b.gap(gap);b.configure(3);b.start();b.until_done(1);
        while(b.d.remaining_o)b.tick();ck(b.d.state_o==3&&!b.d.request_o,"expiry is still unoffered cooldown");
        b.access(0x88,true,2);b.finished(1,true);
    }
    for(uint32_t gap:{0U,1U,1000U}){
        Bench b("held-COUNT-STOP-g"+std::to_string(gap));if(gap)b.gap(gap);else b.compare_enabled=true;
        b.configure(5);b.start();b.stall=true;b.access(0x88,true,2);
        for(unsigned i=0;i<5;i++){ck(b.d.request_o&&b.d.src_o==0x10000&&b.d.length_o==4096,"STOP must not withdraw held descriptor");b.tick();}
        b.stall=false;b.finished(1,true);
    }
    for(unsigned stage:{1U,2U,3U}){
        Bench b("active-gap-write-state"+std::to_string(stage));b.gap(1000);b.configure(3);b.start();
        if(stage==2)b.tick();if(stage==3)b.until_done(1);
        ck(b.d.state_o==stage,"chosen active state");b.access(0xb0,true,7,true);
        ck(b.access(0xb0)==1000,"active gap immutable");ck(b.access(0x88)>>32,"active write sticky batch_error");b.finished(3);
    }
    for(unsigned bad:{0U,1U,2U,3U}){
        Bench b("idle-invalid-gap-write-"+std::to_string(bad));b.gap(17);
        if(bad==0)b.access(0xb0,true,3,true,15);
        if(bad==1)b.access(0xb0,true,1ULL<<32,true);
        if(bad==2){b.backend_available=false;b.access(0xb0,true,3,true);b.backend_available=true;}
        if(bad==3)b.access(0xb8,true,0,true);
        ck(b.access(0xb0)==17,"invalid write preserves old gap");ck(b.access(0x88)>>32,"invalid write sticky batch_error");
        b.access(0x88,true,0);ck((b.access(0x88)>>32)==0,"disable clears existing error");ck(b.access(0xb0)==17,"disable preserves gap");
        ck(b.access(0xb8)==0x49444d4147415001ULL,"readonly gap cap");cases++;
    }
    for(uint64_t length:{64ULL,1536ULL,4096ULL,65536ULL}){
        Bench b("batchOFF-warmup-gMAX-len"+std::to_string(length));b.gap(UINT32_MAX);b.access(0x88,true,0);
        ck(b.access(0xb0)==UINT32_MAX,"disable retains UINT_MAX gap");b.compare_enabled=true;b.batch=false;b.length=length;
        b.access(0,true,0x10000);b.access(8,true,0x100000);b.access(16,true,length);b.access(24,true,0);
        b.start();b.finished(1);ck(b.d.state_o==0&&b.cycle<40,"ordinary warmup ignores huge gap");
    }
    for(unsigned addr:{0U,8U,16U,0x90U,0x98U}){
        Bench b("old-active-write-gold-"+std::to_string(addr));b.compare_enabled=true;b.configure(2);b.start();b.stall=true;
        auto old=b.access(addr);b.access(addr,true,99,addr<0x80);ck(b.access(addr)==old,"old active configuration immutable");
        b.stall=false;b.finished(2);
    }
    {Bench b("UINT_MAX-real-entry-and-STOP");b.gap(UINT32_MAX);b.configure(3);b.start();b.until_done(1);
        ck(b.d.state_o==3&&b.d.remaining_o==UINT32_MAX-1,"UINT_MAX loads g-1 without overflow");
        for(unsigned i=0;i<3;i++){auto q=b.d.remaining_o;b.tick();ck(b.d.remaining_o==q-1&&!b.d.request_o,"real UINT_MAX decrement");}
        b.access(0x88,true,2);b.finished(1,true);}
    {Bench b("UINT_MAX-forced-counter-endpoints");b.gap(UINT32_MAX);b.configure(3);b.start();b.until_done(1);
        b.d.endpoint_force_i=1;
        for(uint32_t q:{UINT32_MAX,UINT32_MAX-1,1U,0U}){
            b.d.endpoint_count_i=q;b.eval();ck(b.d.remaining_o==q,"explicit counter force applied");
            ck(b.d.next_state_o==(q?3:1),"actual DUT counter endpoint next-state");
            ck(q==0||b.d.next_remaining_o==q-1,"actual DUT counter endpoint decrement");
            events<<"{\"case\":\""<<b.name<<"\",\"forced_endpoint\":true,\"remaining\":"<<q<<",\"next_state\":"<<unsigned(b.d.next_state_o)<<",\"next_remaining\":"<<b.d.next_remaining_o<<"}\n";
        }
        b.access(0x88,true,2);b.d.endpoint_force_i=0;b.eval();b.finished(1,true);}
    {Bench b("reset-gap-to-zero");b.gap(4000);b.d.rst_ni=0;b.tick();b.d.rst_ni=1;b.tick();
        ck(b.access(0xb0)==0&&b.access(0xb8)==0x49444d4147415001ULL,"reset gap0 and cap");cases++;}
    events.close();
    std::cout<<"PASS "<<cases<<" finite actual frontend scenarios; default_gold_cycle_checks="<<default_cycles
        <<" enabled_gap0_gold_cycle_checks="<<enabled_cycles<<"; synthetic completions;0 board/backend/IOMMU/timing claims\n";
    return 0;
}catch(const std::exception& error){std::cerr<<"FAIL "<<error.what()<<"\n";return 1;}}
