// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Real public AXI/CSR stimuli. Test-only taps never drive internal state.
#include "Vsw_quiet_top.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

static std::ofstream events;
static uint64_t all_cycles=0,all_checks=0,gold_checks=0;
static unsigned cases=0;
static bool omitted=false;
static void require(bool value,const std::string& text){++all_checks;if(!value)throw std::runtime_error(text);}
template<class T> static uint64_t word64(const T& v,unsigned lane){return uint64_t(v[lane*2])|(uint64_t(v[lane*2+1])<<32);}
template<class T> static void put64(T& v,unsigned lane,uint64_t x){v[lane*2]=uint32_t(x);v[lane*2+1]=uint32_t(x>>32);}
static std::string hexwords(const VlWide<256>& v){std::ostringstream s;s<<std::hex<<std::setfill('0');for(int i=255;i>=0;--i)s<<std::setw(8)<<v[i];return s.str();}
struct Request{uint64_t addr;unsigned id,len,size,index=0;};
struct Reply{unsigned id,resp;};
struct Port{
    bool ar=true,aw=true,w=true,r=true,b=true;
    bool b_error_once=false,r_error_once=false;
    std::deque<Request> reads,writes;
    std::deque<Reply> replies;
    unsigned ars=0,aws=0,rs=0,ws=0,bs=0;
    unsigned previous_stall=0;
    std::array<std::string,3> previous_payload;
};
struct Tick{bool aw,w,ar,b,rv;unsigned bresp,rresp,bid,rid;uint64_t rdata;};
struct Sim{
    VerilatedContext context;
    Vsw_quiet_top d{&context};
    std::array<Port,4> mem;
    std::map<uint64_t,uint8_t> bytes;
    uint64_t cycle=0;
    std::string name;
    explicit Sim(std::string n):name(std::move(n)){
        d.clk_i=0;d.rst_ni=0;d.eval();
        d.p_w_last_i=1;d.p_w_strb_i=0xff;d.p_aw_size_i=d.p_ar_size_i=2;
        d.p_aw_id_i=42;d.p_ar_id_i=53;d.p_b_ready_i=d.p_r_ready_i=1;
        d.fault_cause_i=260;d.dbg_req_ready_i=1;
        events<<"{\"event\":\"case\",\"name\":\""<<name<<"\"}\n";
        for(unsigned i=0;i<4;i++)step();
        d.rst_ni=1;step();settle();
    }
    uint64_t load(uint64_t addr)const{
        uint64_t v=0;for(unsigned i=0;i<8;i++){auto it=bytes.find(addr+i);if(it!=bytes.end())v|=uint64_t(it->second)<<(8*i);}return v;
    }
    void store(uint64_t addr,uint64_t value,unsigned strb=255){for(unsigned i=0;i<8;i++)if(strb&(1u<<i))bytes[addr+i]=uint8_t(value>>(8*i));}
    void drive_memory(){
        d.m_ar_ready_i=d.m_aw_ready_i=d.m_w_ready_i=d.m_r_valid_i=d.m_b_valid_i=d.m_r_last_i=0;
        d.m_r_id_i=d.m_b_id_i=d.m_r_resp_i=d.m_b_resp_i=0;
        for(unsigned i=0;i<4;i++){
            auto& p=mem[i];put64(d.m_r_data_i,i,0);
            if(p.ar&&p.reads.size()<8)d.m_ar_ready_i|=1u<<i;
            if(p.aw&&p.writes.size()<8)d.m_aw_ready_i|=1u<<i;
            if(p.w&&!p.writes.empty())d.m_w_ready_i|=1u<<i;
            if(p.r&&!p.reads.empty()){
                const auto& q=p.reads.front();d.m_r_valid_i|=1u<<i;
                if(q.index==q.len)d.m_r_last_i|=1u<<i;
                d.m_r_id_i|=q.id<<(4*i);put64(d.m_r_data_i,i,load(q.addr+(uint64_t(q.index)<<q.size)));
                if(p.r_error_once)d.m_r_resp_i|=2u<<(2*i);
            }
            if(p.b&&!p.replies.empty()){
                d.m_b_valid_i|=1u<<i;d.m_b_id_i|=p.replies.front().id<<(4*i);d.m_b_resp_i|=p.replies.front().resp<<(2*i);
            }
        }
    }
    void check(const char* phase){
        require(d.equal_o==3,name+": old output/gold mismatch at "+phase);
        for(unsigned i=0;i<256;i++){
            require(d.old_on_o[i]==d.old_gold_o[i],name+": enabled old word mismatch");
            require(d.old_off_o[i]==d.old_gold_o[i],name+": disabled old word mismatch");
        }
        gold_checks+=2;
        require(!(d.sw_idle_o&2)&&!(d.prog_idle_o&2),name+": PnEnable0 tap is not0");
        if(!d.rst_ni)require(!d.sw_idle_o&&!d.prog_idle_o,name+": reset tap not0");
        if(d.rst_ni&&d.ddtp_pending_o)require(!d.regmap_idle_o&&!(d.sw_idle_o&1),name+": deferred DDTP wrongly idle");
        events<<"{\"event\":\"sample\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle<<",\"phase\":\""<<phase<<"\",\"reset\":"<<unsigned(!d.rst_ni)
            <<",\"eq\":"<<unsigned(d.equal_o)<<",\"sw\":"<<unsigned(d.sw_idle_o)<<",\"prog\":"<<unsigned(d.prog_idle_o)<<",\"child\":"<<unsigned(d.child_idle_o)<<",\"regmap_idle\":"<<unsigned(d.regmap_idle_o)
            <<",\"cq\":"<<unsigned(d.cq_state_o)<<",\"cq_wr\":"<<unsigned(d.cq_write_state_o)<<",\"cqh\":"<<d.cq_head_o<<",\"cqt\":"<<d.cq_tail_o
            <<",\"fq\":"<<unsigned(d.fq_state_o)<<",\"fqh\":"<<d.fq_head_o<<",\"fqt\":"<<d.fq_tail_o<<",\"pq\":"<<unsigned(d.pq_state_o)
            <<",\"msi\":"<<unsigned(d.msi_state_o)<<",\"pending\":"<<d.msi_pending_o<<",\"edged\":"<<unsigned(d.msi_edged_o)<<",\"irq\":"<<unsigned(d.irq_pending_o)
            <<",\"debug\":"<<unsigned(d.dbg_state_o)<<",\"go\":"<<unsigned(d.dbg_go_o)<<",\"debug_offer\":"<<unsigned(d.dbg_req_valid_o)
            <<",\"bridge\":"<<unsigned(d.bridge_state_o)<<",\"bridge_valids\":"<<d.bridge_valids_o<<",\"psel\":"<<unsigned(d.bridge_psel_o)<<",\"penable\":"<<unsigned(d.bridge_penable_o)
            <<",\"fault_inputs\":"<<unsigned(d.fault_valid_i)<<",\"fault_spill\":"<<unsigned(d.fault_spill_o)<<",\"fault_fifo\":"<<unsigned(d.fault_fifo_o)
            <<",\"ddtp_pending\":"<<unsigned(d.ddtp_pending_o)<<",\"in_flight\":"<<unsigned(d.in_flight_i)<<",\"reg_valid\":"<<unsigned(d.reg_valid_o)
            <<",\"old_on\":\""<<hexwords(d.old_on_o)<<"\",\"old_off\":\""<<hexwords(d.old_off_o)<<"\",\"old_gold\":\""<<hexwords(d.old_gold_o)<<"\"}\n";
    }
    void settle(){drive_memory();d.clk_i=0;d.eval();}
    Tick step(){
        require(cycle<20000&&all_cycles<200000,name+": finite cycle budget exceeded");settle();check("pre");
        Tick t{bool(d.p_aw_valid_i&&d.p_aw_ready_o),bool(d.p_w_valid_i&&d.p_w_ready_o),bool(d.p_ar_valid_i&&d.p_ar_ready_o),bool(d.p_b_valid_o&&d.p_b_ready_i),bool(d.p_r_valid_o&&d.p_r_ready_i),unsigned(d.p_b_resp_o),unsigned(d.p_r_resp_o),unsigned(d.p_b_id_o),unsigned(d.p_r_id_o),d.p_r_data_o};
        if(d.rst_ni){
            for(unsigned i=0;i<4;i++){
                auto& p=mem[i];unsigned flags=((d.m_ar_valid_o>>i)&1)|(((d.m_aw_valid_o>>i)&1)<<1)|(((d.m_w_valid_o>>i)&1)<<2);
                unsigned ready=((d.m_ar_ready_i>>i)&1)|(((d.m_aw_ready_i>>i)&1)<<1)|(((d.m_w_ready_i>>i)&1)<<2);
                std::array<std::string,3> payload{
                    std::to_string(word64(d.m_ar_addr_o,i))+":"+std::to_string((d.m_ar_id_o>>(4*i))&15)+":"+std::to_string((d.m_ar_len_o>>(8*i))&255)+":"+std::to_string((d.m_ar_size_o>>(3*i))&7),
                    std::to_string(word64(d.m_aw_addr_o,i))+":"+std::to_string((d.m_aw_id_o>>(4*i))&15)+":"+std::to_string((d.m_aw_len_o>>(8*i))&255)+":"+std::to_string((d.m_aw_size_o>>(3*i))&7),
                    std::to_string(word64(d.m_w_data_o,i))+":"+std::to_string((d.m_w_strb_o>>(8*i))&255)+":"+std::to_string((d.m_w_last_o>>i)&1)};
                for(unsigned ch=0;ch<3;ch++)if(p.previous_stall&(1u<<ch))require((flags&(1u<<ch))&&payload[ch]==p.previous_payload[ch],name+": stalled DS payload changed");
                p.previous_stall=flags&~ready;p.previous_payload=payload;
                if((d.m_r_valid_i&d.m_r_ready_o)&(1u<<i)){
                    auto& q=p.reads.front();events<<"{\"event\":\"R\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle<<",\"port\":"<<i<<",\"id\":"<<q.id<<",\"data\":"<<word64(d.m_r_data_i,i)<<",\"last\":"<<unsigned(q.index==q.len)<<",\"resp\":"<<((d.m_r_resp_i>>(2*i))&3)<<"}\n";
                    ++p.rs;p.r_error_once=false;if(q.index++==q.len)p.reads.pop_front();
                }
                if((d.m_b_valid_i&d.m_b_ready_o)&(1u<<i)){
                    events<<"{\"event\":\"B\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle<<",\"port\":"<<i<<",\"id\":"<<p.replies.front().id<<",\"resp\":"<<p.replies.front().resp<<"}\n";++p.bs;p.replies.pop_front();
                }
                if((d.m_ar_valid_o&d.m_ar_ready_i)&(1u<<i)){
                    Request q{word64(d.m_ar_addr_o,i),unsigned((d.m_ar_id_o>>(4*i))&15),unsigned((d.m_ar_len_o>>(8*i))&255),unsigned((d.m_ar_size_o>>(3*i))&7)};
                    require(q.size<=3,name+": DS read size");p.reads.push_back(q);++p.ars;
                    events<<"{\"event\":\"AR\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle<<",\"port\":"<<i<<",\"addr\":"<<q.addr<<",\"id\":"<<q.id<<",\"len\":"<<q.len<<",\"size\":"<<q.size<<"}\n";
                }
                if((d.m_aw_valid_o&d.m_aw_ready_i)&(1u<<i)){
                    Request q{word64(d.m_aw_addr_o,i),unsigned((d.m_aw_id_o>>(4*i))&15),unsigned((d.m_aw_len_o>>(8*i))&255),unsigned((d.m_aw_size_o>>(3*i))&7)};
                    require(q.size<=3,name+": DS write size");p.writes.push_back(q);++p.aws;
                    events<<"{\"event\":\"AW\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle<<",\"port\":"<<i<<",\"addr\":"<<q.addr<<",\"id\":"<<q.id<<",\"len\":"<<q.len<<",\"size\":"<<q.size<<"}\n";
                }
                if((d.m_w_valid_o&d.m_w_ready_i)&(1u<<i)){
                    require(!p.writes.empty(),name+": W without AW");auto& q=p.writes.front();bool last=(d.m_w_last_o>>i)&1;
                    require(last==(q.index==q.len),name+": original/legal-domain WLAST does not match AWLEN");
                    unsigned strb=(d.m_w_strb_o>>(8*i))&255;uint64_t data=word64(d.m_w_data_o,i),addr=q.addr+(uint64_t(q.index)<<q.size);
                    store(addr&~uint64_t(7),data,strb);++p.ws;
                    events<<"{\"event\":\"W\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle<<",\"port\":"<<i<<",\"addr\":"<<addr<<",\"data\":"<<data<<",\"strb\":"<<strb<<",\"last\":"<<unsigned(last)<<"}\n";
                    if(q.index++==q.len){p.replies.push_back({q.id,p.b_error_once?2u:0u});p.b_error_once=false;p.writes.pop_front();}
                }
            }
        }
        if(t.aw||t.w||t.ar||t.b||t.rv)events<<"{\"event\":\"program\",\"case\":\""<<name<<"\",\"cycle\":"<<cycle<<",\"aw\":"<<t.aw<<",\"w\":"<<t.w<<",\"ar\":"<<t.ar<<",\"b\":"<<t.b<<",\"r\":"<<t.rv<<",\"awaddr\":"<<d.p_aw_addr_i<<",\"wdata\":"<<d.p_w_data_i<<",\"araddr\":"<<d.p_ar_addr_i<<",\"rdata\":"<<t.rdata<<"}\n";
        d.clk_i=1;d.eval();check("post");context.timeInc(2);++cycle;++all_cycles;return t;
    }
    void ticks(unsigned n){while(n--)step();settle();}
    void until(const std::function<bool()>& predicate,const std::string& reason){
        for(unsigned i=0;i<2000;i++){settle();if(predicate())return;step();}throw std::runtime_error(name+": timeout "+reason);
    }
    void idle(){until([&]{return (d.sw_idle_o&1)&&(d.prog_idle_o&1);},"natural SW/program quiet");}
    void write(uint64_t addr,uint64_t value,unsigned size=2,int skew=0,unsigned hold_b=0){
        d.p_aw_addr_i=addr;d.p_aw_size_i=size;d.p_aw_len_i=0;
        d.p_w_data_i=size==2&&((addr&7)==4)?value<<32:value;
        d.p_w_strb_i=size==3?255:((addr&7)==4?240:15);d.p_w_last_i=1;
        bool ad=false,wd=false,done=false;unsigned held=0;
        for(unsigned i=0;i<2000&&!done;i++){
            d.p_aw_valid_i=!ad&&(skew>=0||i>=unsigned(-skew));
            d.p_w_valid_i=!wd&&(skew<=0||i>=unsigned(skew));
            d.p_b_ready_i=held>=hold_b;
            settle();if(d.p_b_valid_o&&!d.p_b_ready_i){++held;require(!(d.prog_idle_o&1),name+": held program B wrongly quiet");}
            Tick t=step();ad|=t.aw;wd|=t.w;
            if(t.b){require(t.bresp==0&&t.bid==42,name+": program B response/ID");done=true;}
        }
        require(ad&&wd&&done,name+": program write did not retire");d.p_aw_valid_i=d.p_w_valid_i=0;d.p_b_ready_i=1;settle();
    }
    uint64_t read(uint64_t addr,unsigned size=2,unsigned hold_r=0){
        d.p_ar_addr_i=addr;d.p_ar_size_i=size;d.p_ar_len_i=0;bool sent=false,done=false;uint64_t result=0;unsigned held=0;
        for(unsigned i=0;i<2000&&!done;i++){
            d.p_ar_valid_i=!sent;d.p_r_ready_i=held>=hold_r;settle();
            if(d.p_r_valid_o&&!d.p_r_ready_i){++held;require(!(d.prog_idle_o&1),name+": held program R wrongly quiet");}
            Tick t=step();sent|=t.ar;if(t.rv){require(t.rresp==0&&t.rid==53,name+": program R response/ID");result=t.rdata;done=true;}
        }
        require(sent&&done,name+": program read did not retire");d.p_ar_valid_i=0;d.p_r_ready_i=1;settle();
        return size==2?(result>>((addr&4)?32:0))&0xffffffffu:result;
    }
    void setup_cq(){write(8,2);write(0x18,(0x10u<<10)|3);write(0x48,1);idle();}
    void setup_fq(bool ie=true){write(8,2);write(0x28,(0x20u<<10)|3);write(0x4c,ie?3:1);idle();}
    void command(uint64_t low,uint64_t high){store(0x10000,low);store(0x10008,high);write(0x24,1);}
    void faults(unsigned rounds){
        for(unsigned n=0;n<rounds;n++){
            unsigned pending=3;d.fault_serial_i=n;
            for(unsigned i=0;i<2000&&pending;i++){
                d.fault_valid_i=pending;settle();unsigned accepted=pending&d.fault_ready_o;step();pending&=~accepted;
            }
            require(!pending,name+": fault source backpressure timeout");d.fault_valid_i=0;
        }
        settle();
    }
    void one_fault(){d.fault_serial_i=71;d.fault_valid_i=1;until([&]{return d.fault_ready_o&1;},"fault ready");step();d.fault_valid_i=0;settle();}
    void complete(){
        idle();for(unsigned i=0;i<4;i++)require(mem[i].reads.empty()&&mem[i].writes.empty()&&mem[i].replies.empty(),name+": memory debt at end");
        ++cases;events<<"{\"event\":\"case_pass\",\"name\":\""<<name<<"\",\"cycles\":"<<cycle<<"}\n";std::cout<<"PASS "<<name<<" cycles="<<cycle<<"\n";
    }
};
static void run_cases(){
    {Sim s("empty_psel1_hpm_nonzero");require(s.d.bridge_psel_o==1,"PSEL must retain original1");s.ticks(9);s.write(0x5c,0);s.ticks(12);require(s.read(0x60,3)>0,"real HPM cycles not advanced");s.complete();}
    {Sim s("program_aw_only_and_held_b");s.write(0x18,0x4003,2,7,8);s.complete();}
    {Sim s("program_w_before_aw_and_held_r");s.write(0x18,0x4003,2,-7,0);require(s.read(0x18,2,8)==0x4003,"real CQB readback");s.complete();}
    {Sim s("program_64bit_halves");s.write(0x18,0x100004003ull,3);require(s.read(0x18,3)==0x100004003ull,"real64 CQB readback");s.complete();}
    for(unsigned half=0;half<2;half++){
        Sim s(half?"ddtp_deferred_high32":"ddtp_deferred_low32");s.d.in_flight_i=1;uint32_t value=half?0x2abcde:0x4401;
        s.write(0x10+4*half,value);require(s.d.ddtp_pending_o&&s.d.prog_idle_o==1&&!(s.d.sw_idle_o&1),"deferred after program B");s.ticks(4);
        s.d.in_flight_i=0;s.settle();require(s.d.ddtp_pending_o&&!s.d.regmap_idle_o&&!(s.d.sw_idle_o&1),"pending before original commit edge");s.step();s.settle();
        require(!s.d.ddtp_pending_o&&s.d.regmap_idle_o,"original DDTP commit did not clear pending");require(s.read(0x10+4*half)==value,"deferred DDTP readback");s.complete();
    }
    {Sim s("cq_head_equal_in_decode");s.setup_cq();s.mem[0].ar=false;s.command(1,0);require(!(s.d.sw_idle_o&1),"queued CQ wrongly quiet");s.mem[0].ar=true;
        s.until([&]{return s.d.cq_state_o==3;},"CQ actual DECODE");require(s.d.cq_head_o==s.d.cq_tail_o&&!(s.d.child_idle_o&1),"head equal must not bypass DECODE");s.complete();require(s.mem[0].ars==1&&s.mem[0].rs==2,"real command fetch count");}
    {Sim s("cq_write_delayed_b_stale_substate_irq");s.setup_cq();s.write(0x48,3);s.mem[0].b=false;s.command((uint64_t(0x1234)<<32)|2|(1u<<10)|(1u<<11),0x90000>>2);
        s.until([&]{return !s.mem[0].replies.empty();},"CQ real W completion");s.ticks(8);require(s.d.cq_state_o==4&&s.d.cq_head_o==s.d.cq_tail_o&&!(s.d.sw_idle_o&1),"CQ B debt");
        s.mem[0].b=true;s.complete();require(s.d.cq_write_state_o==2&&s.d.cq_state_o==0,"stale CQ B_RESP must remain original");require(s.d.wsi_o!=0&&s.load(0x90000)==0x1234,"CQ WSI/data");}
    {Sim s("cq_error_original_csr_recovery");s.setup_cq();s.command(0x7f,0);s.until([&]{return s.d.cq_state_o==5;},"CQ ERROR");require(!(s.d.sw_idle_o&1),"CQ ERROR quiet");s.write(0x48,0x401);s.complete();}
    {Sim s("fault_spills_fifo_fq_b_then_unread_ring");s.setup_fq();s.mem[1].b=false;s.faults(6);s.ticks(5);
        require((s.d.fault_spill_o||s.d.fault_fifo_o)&&!s.d.fault_valid_i&&!(s.d.sw_idle_o&1),"stored fault inventory not visible");s.mem[1].b=true;s.complete();
        require(s.mem[1].aws==12&&s.mem[1].ws==48&&s.mem[1].bs==12,"real twelve FQ records");require(s.d.fq_tail_o==12&&s.d.fq_head_o==0&&s.d.wsi_o,"unread FQ ring/IRQ should not prevent quiet");}
    {Sim s("fq_error_original_csr_recovery");s.setup_fq(false);s.mem[1].b_error_once=true;s.one_fault();s.until([&]{return s.d.fq_state_o==2;},"FQ ERROR");require(!(s.d.sw_idle_o&1),"FQ ERROR quiet");s.write(0x4c,0x101);s.complete();}
    if(!omitted){
        {Sim s("debug_go_held_request_ongoing_response");s.write(0x258,0x12345000);s.write(0x25c,0);s.write(0x264,0x5600);s.d.dbg_req_ready_i=0;s.write(0x260,9);s.ticks(5);
            require(s.d.dbg_go_o&&s.d.dbg_state_o==0&&s.d.dbg_req_valid_o&&!(s.d.sw_idle_o&1),"GO held in IDLE");require(s.d.dbg_is_debug_o&&s.d.dbg_did_o==0x56&&s.d.dbg_iova_o==0x12345000,"real debug fields");
            s.d.dbg_req_ready_i=1;s.step();s.ticks(7);require(s.d.dbg_state_o==1&&!s.d.dbg_req_valid_o&&!(s.d.sw_idle_o&1),"ONGOING gap must not be quiet");
            s.d.dbg_ppn_i=0xabc;s.d.dbg_resp_valid_i=1;s.step();s.d.dbg_resp_valid_i=0;s.settle();require(!s.d.dbg_go_o,"original hardware GO clear");s.complete();}
        for(unsigned variant=0;variant<3;variant++){
            Sim s(variant==0?"msi_sticky_edged_irq":variant==1?"msi_masked_pending_disable_unmask":"msi_error_fault_real_fq_chain");s.setup_fq();s.write(0x300,0xa0000);s.write(0x304,0);s.write(0x308,0x6543);
            s.write(0x30c,variant==1?1:0);s.write(8,0);if(variant==2)s.mem[3].b_error_once=true;else if(variant==0)s.mem[3].b=false;
            s.one_fault();
            if(variant==1){s.until([&]{return s.d.msi_pending_o!=0;},"real masked pending");require(s.d.msi_state_o==0&&!(s.d.sw_idle_o&1),"masked pending not busy");s.write(8,2);s.ticks(4);require(s.d.msi_pending_o&&!(s.d.sw_idle_o&1),"disable cannot discard pending");s.write(0x30c,0);}
            if(variant==0){s.until([&]{return !s.mem[3].replies.empty();},"MSI real W completion");s.ticks(7);require(s.d.msi_state_o==1&&!(s.d.sw_idle_o&1),"MSI B debt");s.mem[3].b=true;}
            s.complete();require(s.mem[3].aws==1&&s.mem[3].ws==1&&s.mem[3].bs==1,"MSI must naturally retire once");require(s.load(0xa0000)==0x6543,"real MSI memory data");
            if(variant==2)require(s.mem[1].aws==2&&s.d.fq_tail_o==2,"MSI fault must become second real FQ record");
            else require(s.d.irq_pending_o&&s.d.msi_edged_o,"sticky IRQ must remain edged while idle");
        }
    }else{
        Sim s("omitted_features_idle_identity");require((s.d.child_idle_o&0x30)==0x30,"omitted child idle must be1");s.complete();
    }
}
int main(int argc,char**argv){
    try{
        require(argc==3,"arguments events.jsonl target|omitted");events.open(argv[1]);require(bool(events),"events output open");omitted=std::string(argv[2])=="omitted";
        run_cases();events<<"{\"event\":\"summary\",\"status\":\"passed\",\"cases\":"<<cases<<",\"cycles\":"<<all_cycles<<",\"checks\":"<<all_checks<<",\"gold_pre_post\":"<<gold_checks<<",\"board_trials\":0}\n";
        std::cout<<"SUMMARY cases="<<cases<<" cycles="<<all_cycles<<" checks="<<all_checks<<" gold_pre_post="<<gold_checks<<" board=0\n";return 0;
    }catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<" cases_completed="<<cases<<" cycles="<<all_cycles<<"\n";return 1;}
}
