#include "Vembedding_top.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

static uint64_t checks=0, comparisons=0, cases=0, half_cycles=0;
static bool pn,history,taps,path;
double sc_time_stamp(){return static_cast<double>(half_cycles);}
static void ck(bool ok,const std::string &label){++checks;if(!ok)throw std::runtime_error(label);}
struct Bench {
    Vembedding_top d; std::string name; uint64_t cycle=0,arms=0,targets=0,flushes=0;
    std::array<unsigned,4> reads{},writes{}; unsigned bad=0,mode=0; bool rejected=false,trace=false;
    explicit Bench(std::string label):name(label){
        d.clk_i=0;d.rst_ni=0;d.cfg_valid_i=d.cfg_write_i=0;d.cfg_addr_i=d.cfg_wdata_i=0;d.cfg_wstrb_i=255;
        d.issue_i=d.done_i=d.poll_i=d.batch_active_i=0;d.event_id_i=0;
        d.ar_i=d.aw_i=d.rlast_i=d.b_i=d.bad_id_i=0;d.ar_ready_i=d.aw_ready_i=15;
        d.shared_i=d.out_i=d.return_i=0;d.axi_port_i=0;
        d.dataflow_quiet_i=1;d.certificate_fault_i=d.n_accept_i=0;d.n_did_i=0;d.stop_i=0;
        tick();d.rst_ni=1;tick();
    }
    bool eligible()const{return d.dataflow_quiet_i&&d.phase_o!=1&&!d.active_o&&d.phase_o!=6&&
        !d.capture_busy_o&&!d.certificate_fault_i&&!d.arm_o;}
    bool legal()const{return d.cfg_wdata_i==0||d.cfg_wdata_i==1||d.cfg_wdata_i==2||d.cfg_wdata_i==3||(history&&d.cfg_wdata_i==6);}
    bool intent()const{return pn&&d.cfg_valid_i&&d.cfg_write_i&&d.cfg_addr_i==0x78&&d.index_o==193;}
    bool accept()const{return intent()&&d.cfg_wstrb_i==255&&legal()&&eligible();}
    void eval(){d.clk_i=0;d.eval();}
    void sample(){
        ck(d.gold_match_o,name+": old scalar/state/ID/stock/read diff outside approved PN differences");
        ck(d.path_match_o,name+": old full PATH state differs");
        ck(d.tap_match_o,name+": original fact taps differ");++comparisons;
        bool quiet=pn&&!bad&&!d.ar_i&&!d.aw_i;
        for(unsigned i=0;i<4;++i)quiet=quiet&&!reads[i]&&!writes[i];
        ck(bool(d.private_quiet_o)==quiet,name+": exact four-port stock fact");
        ck(d.mode_o==(pn?mode:0),name+": full atomic mode/history word");
        ck(bool(d.clear_o)==bool(pn&&d.rst_ni&&accept()),name+": policy clear qualifier");
        if(!pn)ck(!d.capture_busy_o&&!d.status_o&&!d.aown_o&&!d.union_o&&!d.n10_o&&!d.n11_o,name+": disabled child outputs zero");
        if(pn&&d.cfg_addr_i==0x78&&d.index_o>=192&&d.index_o<=199){
            uint64_t expected=0;
            switch(d.index_o){
                case 192:expected=UINT64_C(0x49444d41504e0001);break;
                case 193:expected=mode;break;
                case 194:expected=(uint64_t(history)<<63)|eligible()|(uint64_t(rejected)<<1)|
                    (uint64_t(d.certificate_fault_i)<<2)|(uint64_t(d.capture_busy_o)<<3)|
                    (uint64_t(d.dataflow_quiet_i)<<4)|(uint64_t(d.active_o||d.phase_o==6)<<5);break;
                case 195:expected=d.n10_o;break;case 196:expected=d.n11_o;break;
                case 197:expected=d.aown_o;break;case 198:expected=d.union_o;break;case 199:expected=d.status_o;break;
            }
            ck(d.rdata_o==expected,name+": exact PN DATA word");
        }
        if(d.cfg_addr_i==0x70)ck(d.rdata_o==d.index_o,name+": INDEX read not masked by PN bank");
        if(d.cfg_addr_i==0x58)ck(d.rdata_o==(path?UINT64_C(0x49444d414f425303):UINT64_C(0x49444d414f425302)),name+": old Magic read not masked");
        if(d.cfg_addr_i==0x60)ck((d.rdata_o&7)==uint64_t((d.phase_o==1)|((d.active_o||d.phase_o==6)<<1)|((d.phase_o==5)<<2)),name+": original status low bits");
    }
    void tick(){
        eval();sample();
        if(trace)std::cout<<"{\"fixture_only\":true,\"source_board_cases\":0,\"case\":\""<<name<<"\",\"cycle\":"<<cycle
            <<",\"arm\":"<<unsigned(d.arm_o)<<",\"active\":"<<unsigned(d.active_o)<<",\"target\":"<<unsigned(d.target_o)
            <<",\"issue\":"<<unsigned(d.issue_i)<<",\"done\":"<<unsigned(d.done_i)<<",\"stop\":"<<unsigned(d.stop_i)
            <<",\"batch\":"<<unsigned(d.batch_active_i)<<",\"quiet\":"<<unsigned(d.dataflow_quiet_i)
            <<",\"n_accept\":"<<unsigned(d.n_accept_i)<<",\"n_did\":"<<d.n_did_i<<",\"phase\":"<<unsigned(d.phase_o)
            <<",\"capture_busy\":"<<unsigned(d.capture_busy_o)<<",\"flags\":"<<d.status_o<<",\"aown\":"<<d.aown_o
            <<",\"union\":"<<d.union_o<<",\"n10\":"<<d.n10_o<<",\"n11\":"<<d.n11_o<<"}\n";
        if(d.rst_ni){
            arms+=d.arm_o;targets+=d.target_o;flushes+=d.clear_o;
            if(intent()){if(accept()){mode=unsigned(d.cfg_wdata_i);rejected=false;}else rejected=true;}
            for(unsigned p=0;p<4;++p){
                const bool ar=(d.ar_i&d.ar_ready_i)&(1U<<p),rr=d.rlast_i&(1U<<p);
                const bool aw=(d.aw_i&d.aw_ready_i)&(1U<<p),br=d.b_i&(1U<<p);
                if(ar&&!rr){if(reads[p]==255)bad|=1U<<p;reads[p]=(reads[p]+1)&255;}
                if(!ar&&rr){if(!reads[p])bad|=1U<<p;reads[p]=(reads[p]-1)&255;}
                if(aw&&!br){if(writes[p]==255)bad|=1U<<p;writes[p]=(writes[p]+1)&255;}
                if(!aw&&br){if(!writes[p])bad|=1U<<p;writes[p]=(writes[p]-1)&255;}
            }
            bad|=d.bad_id_i;
        }
        ++half_cycles;d.clk_i=1;d.eval();++half_cycles;++cycle;eval();sample();
    }
    uint64_t cfg(unsigned addr,bool write=false,uint64_t value=0,unsigned strobe=255){
        d.cfg_valid_i=1;d.cfg_write_i=write;d.cfg_addr_i=addr;d.cfg_wdata_i=value;d.cfg_wstrb_i=strobe;
        eval();sample();const auto result=d.rdata_o;tick();d.cfg_valid_i=d.cfg_write_i=0;d.cfg_wstrb_i=255;eval();sample();return result;
    }
    uint64_t word(unsigned i){cfg(0x70,true,i);return cfg(0x78);}
    void mode_write(uint64_t value,unsigned strobe=255){cfg(0x70,true,193);cfg(0x78,true,value,strobe);if(pn){word(193);word(194);}}
    static uint64_t command(unsigned v,unsigned a,bool hasa,bool batch,bool pathbit){return 1ULL|(uint64_t(v)<<8)|(uint64_t(a)<<16)|
        (uint64_t(hasa)<<24)|(uint64_t(batch)<<25)|(uint64_t(pathbit)<<26);}
    void arm(unsigned v,unsigned a,bool hasa,unsigned count=1,bool batch=true){
        cfg(0x68,true,count);d.cfg_valid_i=d.cfg_write_i=1;d.cfg_addr_i=0x60;d.cfg_wdata_i=command(v,a,hasa,batch,path&&batch);
        eval();sample();ck(d.arm_o&&d.victim_o==v&&d.aggressor_o==a&&bool(d.has_a_o)==hasa,name+": accepted ARM uses same-edge new roles");
        tick();d.cfg_valid_i=d.cfg_write_i=0;eval();sample();if(pn)ck(d.capture_busy_o&&d.status_o==0&&!d.n10_o&&!d.n11_o,name+": actual ARM clears new capture");
    }
    uint64_t event(unsigned issue,unsigned done,unsigned poll,uint64_t id){
        d.issue_i=issue;d.done_i=done;d.poll_i=poll;d.event_id_i=id;const auto at=cycle;tick();
        d.issue_i=d.done_i=d.poll_i=0;eval();sample();return at;
    }
    void chain(unsigned port,bool write){
        d.axi_port_i=port;const unsigned lane=1U<<port,ch=write?2:1;
        if(write)d.aw_i=lane;else d.ar_i=lane;d.shared_i=ch;tick();d.aw_i=d.ar_i=d.shared_i=0;
        d.out_i=ch;tick();d.out_i=0;
        d.return_i=ch;if(write)d.b_i=lane;else d.rlast_i=lane;tick();d.return_i=d.b_i=d.rlast_i=0;eval();sample();
    }
    void finish(){++cases;std::cout<<"{\"case\":\""<<name<<"\",\"status\":\"passed\",\"fixture_only\":true,\"source_board_cases\":0,"
        <<"\"arms\":"<<arms<<",\"targets\":"<<targets<<",\"flush_beats\":"<<flushes<<"}\n";}
};

int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
    ck(argc==5,"explicit profile arguments");pn=std::string(argv[1])=="1";history=std::string(argv[2])=="1";
    taps=std::string(argv[3])=="1";path=std::string(argv[4])=="1";
    {
        Bench b("PN-decode-mode-and-old-addresses");
        if(pn){
            b.cfg(0x68,true,123);b.cfg(0x70,true,193);
            ck(b.cfg(0x68)==123&&b.cfg(0x70)==193,"PN index does not replace target/INDEX");
            b.cfg(0x58);b.cfg(0x60);
            for(unsigned from:{0U,1U,2U,3U,6U})for(unsigned to:{0U,1U,2U,3U,6U}){b.mode_write(from);b.mode_write(to);}
            for(uint64_t value:std::array<uint64_t,5>{4,5,7,8,UINT64_C(0x1000000000000006)})b.mode_write(value);
            b.mode_write(3,0x7f);b.mode_write(2,0);b.mode_write(1);b.mode_write(1);
            b.d.dataflow_quiet_i=0;b.mode_write(2);b.d.dataflow_quiet_i=1;
            b.d.certificate_fault_i=1;b.mode_write(2);b.d.certificate_fault_i=0;
            for(unsigned idx:{192U,194U,195U,196U,197U,198U,199U})for(unsigned s:{0U,1U,127U,255U}){
                b.cfg(0x70,true,idx);b.cfg(0x78,true,UINT64_MAX,s);b.word(194);
            }
            ck(b.d.error_o==0,"MODE/readonly PN writes never pollute old error");
        }
        for(uint64_t idx:std::array<uint64_t,7>{35,63,192,199,200,255,UINT64_C(0x100000000c0)}){
            b.cfg(0x70,true,idx);const bool allowed=pn&&(idx==192||idx==199);
            ck(bool(b.d.error_o&1)==!allowed,"full64 INDEX range, no truncation aliases");b.cfg(0x60,true,4);
        }
        b.cfg(0x70,true,192,0x7f);ck(b.d.error_o&1,"partial INDEX still old error");b.cfg(0x60,true,4);
        b.cfg(0x70,true,0);for(unsigned i=0;i<35;++i)b.word(i);
        if(path)for(unsigned i=64;i<192;++i)b.word(i);
        else {b.cfg(0x70,true,64);ck(b.d.error_o&1,"PATH off range remains invalid");}
        b.finish();
    }
    {
        Bench b("actual-ARM-window-A-tail-and-selected-STOP");b.trace=true;
        b.event(4,0,0,77);b.event(0,4,0,77);ck(!b.d.capture_busy_o,"warmup before accepted ARM excluded");
        b.arm(2,3,true,2);b.d.dataflow_quiet_i=0;b.d.batch_active_i=12;
        b.d.n_accept_i=1;b.d.n_did_i=10;const auto firstA=b.event(8,0,0,1);
        b.event(4,0,0,1);b.d.n_accept_i=0; // Both pre-open edges excluded.
        b.d.n_accept_i=1;b.d.n_did_i=10;b.tick();b.d.n_accept_i=0;
        for(unsigned p:{2U,3U})for(bool w:{false,true})b.chain(p,w);
        b.event(0,4,0,1);b.event(4,0,0,2);
        b.d.n_accept_i=1;b.d.n_did_i=11;b.d.done_i=4;b.d.event_id_i=2;b.eval();
        ck(b.d.active_o&&b.d.target_o,"real final done/close, not poll");b.tick();b.d.done_i=0;b.d.n_accept_i=0;
        b.event(0,0,4,99);ck(b.d.phase_o==6,"wrong poll not frozen");b.event(0,0,4,2);ck(b.d.phase_o==5,"correct old poll freezes old observer");
        if(pn){ck(b.d.capture_busy_o,"A capture continues after old FROZEN");b.d.dataflow_quiet_i=1;b.mode_write(2);ck(b.mode==0&&b.rejected,"capture busy prevents MODE despite old frozen");b.d.dataflow_quiet_i=0;}
        const auto lastA=b.event(0,8,0,7);b.d.batch_active_i=0;b.d.dataflow_quiet_i=1;
        b.d.stop_i=1;b.tick();b.d.stop_i=0;for(unsigned i=0;i<5;++i)b.tick();
        if(pn)ck(b.d.capture_busy_o,"unselected STOP cannot freeze capture");
        b.d.stop_i=8;b.tick();b.d.stop_i=0;for(unsigned i=0;i<7;++i)b.tick();
        if(pn){
            ck(b.d.status_o==7&&!b.d.capture_busy_o,"selected STOP+quiet yields valid frozen pair");
            ck(b.d.aown_o==lastA-firstA&&b.d.union_o==lastA-firstA,"absolute-cycle Aown/union exclude STOP+quiet tail");
            ck(b.d.n10_o==1&&b.d.n11_o==1,"original (open,close] N accepts count exact");
            b.mode_write(2);ck(b.mode==2&&!b.rejected,"MODE succeeds after capture ends");
            ck(b.word(197)==lastA-firstA&&b.word(198)==lastA-firstA&&b.word(199)==7,"PN bank preserves capture across MODE clear");
        }
        ck(b.d.error_o==0&&b.word(7)==2&&b.word(8)==2&&b.word(9)==1,"old scalar completion/read sequencing unchanged");
        for(unsigned i=0;i<35;++i)b.word(i);if(path)for(unsigned i=64;i<192;++i)b.word(i);
        b.finish();
    }
    {
        Bench b("Solo-freezes-before-old-poll-no-busy-feedback");b.arm(1,0,false);b.d.dataflow_quiet_i=0;b.d.batch_active_i=2;
        const auto first=b.event(2,0,0,1);b.tick();const auto last=b.event(0,2,0,1);
        b.d.batch_active_i=0;b.d.dataflow_quiet_i=1;b.d.stop_i=2;b.tick();b.d.stop_i=0;
        if(pn){ck(b.d.status_o==5&&!b.d.capture_busy_o&&b.d.phase_o==6,"capture freezes while old BATCH_READ still busy");
            ck(b.d.union_o==last-first&&!b.d.aown_o,"Solo absolute-cycle union and no Aown");
            b.mode_write(3);ck(b.mode==0&&b.rejected,"old BATCH_READ alone protects MODE after capture froze");}
        b.event(0,0,2,1);if(pn){b.mode_write(3);ck(b.mode==3,"after old poll MODE accepted");}
        // Next original ARM resets the new capture and latches changed roles.
        b.arm(3,2,false);if(pn)ck(b.d.status_o==0&&b.d.capture_busy_o&&!b.d.union_o,"next accepted ARM clears capture only");
        b.finish();
    }
    {
        Bench b("failed-STOP-without-target-and-old-abort");b.arm(0,1,true);
        b.d.stop_i=1;b.tick();b.d.stop_i=0;
        if(pn)ck(b.d.status_o==33&&!b.d.capture_busy_o,"failed STOP quiet freezes invalid, no invented duration");
        b.mode_write(2);if(pn)ck(b.mode==0&&b.rejected,"old ARMED protects MODE even invalid capture frozen");
        b.cfg(0x60,true,2);ck(b.d.error_o&16,"old abort error preserved");b.finish();
    }
    {
        Bench b("old-rejected-ARM-and-ID-error-plus-N-invalid");b.cfg(0x68,true,0);
        b.cfg(0x60,true,Bench::command(0,1,true,true,path));ck(!b.arms&&!b.d.capture_busy_o&&(b.d.error_o&1),"rejected ARM never starts capture");
        b.cfg(0x60,true,4);b.arm(0,1,false);b.d.batch_active_i=1;b.d.dataflow_quiet_i=0;
        b.event(1,0,0,1);b.d.done_i=1;b.d.event_id_i=99;b.d.n_accept_i=1;b.d.n_did_i=12;b.eval();
        ck(b.d.target_o,"original final count still target on wrong ID");b.tick();b.d.done_i=b.d.n_accept_i=0;
        b.event(0,0,1,1);b.d.batch_active_i=0;b.d.dataflow_quiet_i=1;b.d.stop_i=1;b.tick();b.d.stop_i=0;
        ck(b.d.error_o&2,"old ID error remains mandatory");if(pn)ck((b.d.status_o&128)&&!(b.d.status_o&6),"unknown DID invalidates both new durations");
        b.finish();
    }
    {
        Bench b("old-nonbatch-poll-sequence");b.arm(0,1,false,2,false);
        b.event(1,0,0,1);b.event(0,1,0,1);b.event(0,0,1,1);b.event(1,0,0,2);b.event(0,1,0,2);b.event(0,0,1,2);
        ck(b.d.phase_o==5&&b.targets==0&&b.word(9)==2,"nonbatch original two-poll sequence no new target");b.finish();
    }
    for(unsigned lane=0;lane<4;++lane){
        Bench b("private-all-lanes-held-stock-"+std::to_string(lane));const unsigned bit=1U<<lane;
        b.d.ar_i=b.d.aw_i=bit;b.d.ar_ready_i=b.d.aw_ready_i=0;b.tick();
        b.d.ar_i=b.d.aw_i=0;b.tick();b.d.ar_ready_i=b.d.aw_ready_i=15;
        b.d.ar_i=b.d.aw_i=bit;b.tick();b.d.ar_i=b.d.aw_i=0;b.tick();
        b.d.rlast_i=b.d.b_i=bit;b.tick();b.d.rlast_i=b.d.b_i=0;b.tick();
        b.d.ar_i=b.d.aw_i=b.d.rlast_i=b.d.b_i=bit;b.tick();b.d.ar_i=b.d.aw_i=b.d.rlast_i=b.d.b_i=0;b.tick();
        if(pn)ck(b.d.private_quiet_o,"all returned debt quiet across every lane and zero-net beat");
        b.d.bad_id_i=bit;b.tick();b.d.bad_id_i=0;b.tick();if(pn)ck(!b.d.private_quiet_o,"bad-ID sticky fact cannot be cleared by reads or ARM");
        b.cfg(0x60,true,4);if(pn)ck(!b.d.private_quiet_o,"old clear does not reset private tracker");b.finish();
    }
    for(bool write:{false,true}){
        Bench b(write?"private-AW-overflow":"private-AR-overflow");
        if(write)b.d.aw_i=8;else b.d.ar_i=8;for(unsigned i=0;i<256;++i)b.tick();
        b.d.ar_i=b.d.aw_i=0;b.tick();if(pn)ck(!b.d.private_quiet_o&&b.bad==8,"wrapped zero stock never quiet after overflow");b.finish();
    }
    {
        Bench b("private-underflow");b.d.rlast_i=2;b.d.b_i=4;b.tick();b.d.rlast_i=b.d.b_i=0;b.tick();
        if(pn)ck(!b.d.private_quiet_o&&b.bad==6,"underflow sticky on separate lanes");b.finish();
    }
    std::cout<<"{\"summary\":\"passed\",\"fixture_only\":true,\"source_board_cases\":0,\"cases\":"<<cases
        <<",\"checks\":"<<checks<<",\"gold_comparisons\":"<<comparisons<<"}\n";
}catch(const std::exception &error){std::cerr<<error.what()<<'\n';return 1;}return 0;}
