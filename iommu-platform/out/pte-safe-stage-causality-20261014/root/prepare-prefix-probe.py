"""Create a NEW passive native-prefix probe. Never invoke an old scenario or runner."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json

W=Path('/home/xvhaocheng/work');P=W/'iommu-platform';O=P/'out/pte-safe-stage-causality-20261014';M=O/'model'
assert not (M/'safe_pte_stage_top.sv').exists()
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
admission={'schema':'safe-passive-probe-admission-v1','created_utc':datetime.now(timezone.utc).isoformat(),'author':'root Codex architecture/source admission','admitted':'one passive native-prefix legality/phase probe, not a F1/ABBA performance experiment','why':'Source gives exact conditional dualREAD capture but fixed native prefix phase is unknown. Native module/traffic configuration is legal; passive observation resolves that necessary fact without granting a choice or manipulating the state.','native_config':{'one_job_each':True,'bytes_each':4096,'buffer_beats_each':512,'DEFAULT':True,'cpu':False,'PolicyEnable':0,'DdrAtomics':0,'SharedPoolMode':0,'SameBankDmasEnable':False,'ReverseReadOrderEnable':False,'owner_only':False,'read_admit_proxy_mode':0,'DS_input_output_delay':[50,50],'DDR_head_prepare_latency':31,'DDR_queue_capacity':16},'actions':'program both descriptors then ordinary serial NEXT_ID trigger source0 followed source1; original NormalTL and AR/R/W/B RTL arbitration untouched; no warmup/batch/stimulus, no phase search, no conditional kick using debug data','R_policy':'approved DSpriority window extends through ALL two jobs and every owned R/W/B/faultquiet; identical original head-only preparation and held/sameID semantics, no latency-overlap or tail pause','source_gate_status':'conditional fresh state only; ABBA action/online prediction remains not admitted unless actual fresh safe pair captured','probe_limit':{'build':'one new changed wrapper+main','native_prefix_run':1,'phase_retries':0,'old_runs':0},'stop':'absent fresh safe pair => no forced choice/ABBA or four-condition performance; record this fixed prefix only, global reachability unknown','paper_complete':False}
(O/'root/probe-admission.json').write_text(json.dumps(admission,ensure_ascii=False,indent=2)+'\n')

top=P/'tests/nonblock_actual_dma_system_top.sv';s=top.read_text()
assert s.count('module nonblock_actual_dma_system_top')==1
s=s.replace('module nonblock_actual_dma_system_top','module safe_pte_stage_top',1)
signals={
'pair':('logic [1:0]','i_dma.i_sys.gen_banked.i_banked.bank_trans_req_valid'),
'mvalid':('logic','i_dma.i_sys.trans_req_valid'),
'mready':('logic','i_dma.i_sys.trans_req_ready'),
'read0':('logic','i_dma.i_sys.gen_banked.i_banked.gen_bank[0].i_bank.is_read'),
'read1':('logic','i_dma.i_sys.gen_banked.i_banked.gen_bank[1].i_bank.is_read'),
'capacity':('logic [1:0]','i_dma.fullbuffer_jobs_o'),
'head_did0':('logic [23:0]','i_dma.i_sys.gen_banked.i_banked.bank_trans_req_data[0].did'),
'head_did1':('logic [23:0]','i_dma.i_sys.gen_banked.i_banked.bank_trans_req_data[1].did'),
'head_id0':('logic [3:0]','i_dma.i_sys.gen_banked.i_banked.gen_bank[0].i_bank.ar_data.id'),
'head_id1':('logic [3:0]','i_dma.i_sys.gen_banked.i_banked.gen_bank[1].i_bank.ar_data.id'),
'head_iova0':('logic [63:0]','i_dma.i_sys.gen_banked.i_banked.bank_trans_req_data[0].iova'),
'head_iova1':('logic [63:0]','i_dma.i_sys.gen_banked.i_banked.bank_trans_req_data[1].iova'),
'owner_bank':('logic','i_dma.i_sys.i_tl.req_data.bank'),
'owner_did':('logic [23:0]','i_dma.i_sys.i_tl.req_data.did'),
'owner_iova':('logic [63:0]','i_dma.i_sys.i_tl.req_data.iova'),
'owner_type':('logic [3:0]','i_dma.i_sys.i_tl.req_data.ttype'),
'pte_level':('logic [1:0]','i_dma.i_sys.i_tl.i_rv_iommu_ptw.main_lvl_q'),
'pte_stage':('logic [1:0]','i_dma.i_sys.i_tl.i_rv_iommu_ptw.ptw_stage_q'),
'pte_valid':('logic','u_ptw_ar_valid'),
'pte_ready':('logic','ptw_resp.ar_ready'),
'pte_addr':('logic [63:0]','u_ptw_ar_addr'),
'pte_rvalid':('logic','ptw_resp.r_valid'),
'pte_rready':('logic','u_ptw_r_ready'),
'pte_rdata':('logic [63:0]','ptw_resp.r.data'),
'ds_valid':('logic','ds_req.ar_valid'),
'ds_ready':('logic','ds_resp.ar_ready'),
'ds_id':('logic [3:0]','ds_req.ar.id'),
'ds_addr':('logic [63:0]','ds_req.ar.addr'),
'ds_delayed_valid':('logic','ds_delayed_req.ar_valid'),
'ds_delayed_ready':('logic','ds_delayed_resp.ar_ready'),
'ds_delayed_id':('logic [3:0]','ds_delayed_req.ar.id'),
'ds_delayed_addr':('logic [63:0]','ds_delayed_req.ar.addr'),
'mux_reqs':('logic [3:0]','i_xbar.gen_mst_port_mux[0].i_axi_mux.gen_mux.slv_ar_valids'),
'mux_valid':('logic','i_xbar.gen_mst_port_mux[0].i_axi_mux.gen_mux.ar_valid'),
'mux_ready':('logic','i_xbar.gen_mst_port_mux[0].i_axi_mux.gen_mux.ar_ready'),
'mux_id':('logic [5:0]','i_xbar.gen_mst_port_mux[0].i_axi_mux.gen_mux.mst_ar_chan.id'),
'mux_addr':('logic [63:0]','i_xbar.gen_mst_port_mux[0].i_axi_mux.gen_mux.mst_ar_chan.addr'),
}
ports='\n'.join('    output '+typ+' probe_'+name+',' for name,(typ,expr) in signals.items())+'\n'
s=s.replace('    input  logic        clk_i,',ports+'    input  logic        clk_i,',1)
assert s.count('\nendmodule')==1
s=s.replace('\nendmodule','\n// Passive views only; no signal is fed back into native scheduling.\n'+'\n'.join('    assign probe_'+name+' = '+expr+';' for name,(typ,expr) in signals.items())+'\nendmodule',1)
(M/'safe_pte_stage_top.sv').write_text(s)

orig=P/'tests/nonblock_actual_dma_test.cpp';cpp=orig.read_text()
cpp=cpp[:cpp.index('\nint main(int argc, char** argv)')]
cpp=cpp.replace('Vnonblock_actual_dma_system_top','Vsafe_pte_stage_top')
start=cpp.index('        const bool ds_window = full_batch\n')
end=cpp.index('\n        if (ds_priority',start)
cpp=cpp[:start]+'        const bool ds_window = true; // NEW approved complete-task priority window.\n'+cpp[end:]
fields=[(n,'top->probe_'+n) for n in signals]+[(n,'top->'+n) for n in ['ddr_ar_valid','ddr_ar_ready','ddr_ar_id','ddr_ar_addr','ddr_ar_len','ddr_r_valid','ddr_r_ready','ddr_r_id','ddr_r_resp','ddr_r_last','ddr_aw_valid','ddr_aw_ready','ddr_aw_id','ddr_aw_len','ddr_w_valid','ddr_w_ready','ddr_w_last','ddr_b_valid','ddr_b_ready','ddr_b_id','ddr_b_resp','dbg_dma_job_quiet','dbg_in_flight']]+[('memq','mem_q.size()'),('prior_mheld','probe_mheld')]
header='cycle,'+','.join(n for n,e in fields)+'\n'
method='''    FILE* probe_file = nullptr;
    bool probe_record = false, probe_mheld = false;
    uint64_t probe_safe_fresh=0, probe_any_fresh=0, probe_both=0, probe_Rheld=0, probe_peak=0;
    uint64_t probe_ar[2]={0,0}, probe_aw[2]={0,0}, probe_R[2]={0,0}, probe_W[2]={0,0}, probe_B[2]={0,0}, probe_err=0;
    void probe_emit() {
        if (!probe_record) return;
        std::fprintf(probe_file,"%ld",g_cycle);
'''+''.join('        std::fprintf(probe_file,",%llu",(unsigned long long)('+e+'));\n' for n,e in fields)+'''        std::fputc('\n',probe_file);
        const bool fresh=top->probe_pair==3 && !probe_mheld;
        if (top->probe_pair==3) ++probe_both;
        if (fresh) ++probe_any_fresh;
        if (fresh && top->probe_read0 && top->probe_read1 && top->probe_capacity==3) ++probe_safe_fresh;
        probe_mheld=top->probe_mvalid && !top->probe_mready;
        if (top->ddr_r_valid && !top->ddr_r_ready) ++probe_Rheld;
        probe_peak=std::max(probe_peak,(uint64_t)mem_q.size());
        if (top->ddr_ar_valid && top->ddr_ar_ready && (top->ddr_ar_id>>4)==2) ++probe_ar[(top->ddr_ar_id>>2)&1];
        if (top->ddr_aw_valid && top->ddr_aw_ready && (top->ddr_aw_id>>4)==2) ++probe_aw[(top->ddr_aw_id>>2)&1];
        if (top->ddr_r_valid && top->ddr_r_ready && (top->ddr_r_id>>4)==2) { ++probe_R[(top->ddr_r_id>>2)&1]; probe_err += top->ddr_r_resp!=0; }
        if (top->ddr_w_valid && top->ddr_w_ready && !caw_q.empty() && (caw_q.front().id>>4)==2) ++probe_W[(caw_q.front().id>>2)&1];
        if (top->ddr_b_valid && top->ddr_b_ready && (top->ddr_b_id>>4)==2) { ++probe_B[(top->ddr_b_id>>2)&1]; probe_err += top->ddr_b_resp!=0; }
    }
'''
# Literal C++ backslash-n is required, rather than a newline embedded inside a char literal.
method=method.replace("std::fputc('\n',probe_file);","std::fputc('\\n',probe_file);")
cpp=cpp.replace('    void tick() {',method+'\n    void tick() {',1)
cpp=cpp.replace('        // ================= pre-edge snapshot ==============================','        probe_emit();\n        // ================= pre-edge snapshot ==============================',1)
cpp+='''
int main(int argc,char**argv) {
    if (argc!=2) return 2;
    try {
        Tb tb;
        tb.a_pages=1; tb.warmup=0; tb.v_transfers=1; tb.a_target=1;
        tb.cpu_enable=false; tb.cpu_writes=false; tb.full_batch=false;
        tb.policy_partition=false; tb.owner_only=false;
        tb.ds_priority=true; tb.read_latency_overlap=false; tb.write_tail_pause=0;
        tb.read_admit_mode_val=0; tb.read_admit_measure_only=false;
        tb.ptw_latency=31;
        tb.setup_memory(1); tb.reset(true);
        tb.program_single(SEL_V,V_SRC_IOVA,V_DST_IOVA);
        tb.program_single(SEL_A,A_IOVA_BASE,A_IOVA_BASE+0x1000);
        tb.probe_file=std::fopen(argv[1],"w"); CHECK(tb.probe_file,"new probe CSV open");
        std::fputs(HEADER_LITERAL,tb.probe_file);
        tb.probe_record=true; const long t0=tb.g_cycle;
        const uint64_t vID=tb.trigger(SEL_V), aID=tb.trigger(SEL_A);
        tb.wait_done(SEL_V,vID,"new-prefix-V"); const long vdone=tb.g_cycle;
        tb.wait_done(SEL_A,aID,"new-prefix-A"); const long adone=tb.g_cycle;
        tb.check_quiet("new-passive-prefix");
        for (uint32_t i=0;i<PAGE_BYTES;i++) {
            CHECK(tb.mem[V_SRC_PA+i]==tb.mem[V_DST_PA+i],"source0 bytes mismatch");
            CHECK(tb.mem[A_SRC_PA_BASE+i]==tb.mem[A_DST_PA_BASE+i],"source1 bytes mismatch");
        }
        for(int c=0;c<2;c++) {
            CHECK(tb.probe_ar[c]==2 && tb.probe_aw[c]==2,"actual burst count");
            CHECK(tb.probe_R[c]==512 && tb.probe_W[c]==512 && tb.probe_B[c]==2,"actual bytes/response conservation");
        }
        CHECK(tb.probe_err==0,"payload response error");
        std::fclose(tb.probe_file);tb.probe_file=nullptr;
        std::printf("SAFE-PASSIVE-PREFIX {\\"t0\\":%ld,\\"DONE_poll\\":[%ld,%ld],\\"quiet\\":%ld,\\"safe_fresh\\":%llu,\\"any_fresh\\":%llu,\\"both_valid_ticks\\":%llu,\\"Rheld_ticks\\":%llu,\\"memq_peak\\":%llu,\\"bytes_R_each\\":4096,\\"bytes_W_each\\":4096,\\"AR_each\\":2,\\"AW_each\\":2,\\"B_each\\":2,\\"errors\\":0,\\"pass\\":true}\\n",
            t0,vdone-t0,adone-t0,tb.g_cycle-t0,(unsigned long long)tb.probe_safe_fresh,(unsigned long long)tb.probe_any_fresh,(unsigned long long)tb.probe_both,(unsigned long long)tb.probe_Rheld,(unsigned long long)tb.probe_peak);
        return 0;
    } catch(const std::exception&e) { std::fprintf(stderr,"NEW-PREFIX-FAIL %s\\n",e.what());return 1; }
}
'''.replace('HEADER_LITERAL',json.dumps(header))
(M/'safe_pte_stage_probe.cpp').write_text(cpp)
(M/'prepare-provenance.json').write_text(json.dumps({'schema':'new-passive-copy-provenance-v1','originals':[{'path':str(p),'sha256':sha(p)} for p in [top,orig]],'new_copies':[{'path':str(p),'sha256':sha(p)} for p in [M/'safe_pte_stage_top.sv',M/'safe_pte_stage_probe.cpp']],'native_arbitration_unchanged':True,'new_taps_only':list(signals),'controller_does_not_read_probe_data':True,'old_main_and_scenarios_not_executed':True,'priority_change':'only approved complete-task scope, original memory shape unchanged'},ensure_ascii=False,indent=2)+'\n')
print('Prepared one new passive native-prefix probe; no simulation or build run yet.')
