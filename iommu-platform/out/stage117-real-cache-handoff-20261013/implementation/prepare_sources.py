#!/usr/bin/env python3
import pathlib,json,hashlib
H=pathlib.Path(__file__).resolve().parent;R=H.parents[2];O=R/'out/stage117-packet-protocol-adapter-20261013/implementation'
for n in ['inherited_harness.hpp','finite_hooks.inc','packet_protocol.hpp','packet_dma.cpp','nonblock_actual_dma_system_top.sv']:(H/n).write_bytes((O/n).read_bytes())
# Additional test-only scalar ports, all direct views of real cache interfaces.
ports='''    input logic c_req,c_we,c_tag_valid,c_kill,c_fence,
    input logic [63:0] c_addr,c_wdata,
    input logic [7:0] c_be,
    input logic [1:0] c_size,
    output logic c_gnt,c_rvalid,
    output logic [63:0] c_rdata,
    output logic c_flush_i,c_flush_d,c_flush_ack,c_halt,c_wb_empty,c_mshr,c_mshr1,c_dmiss,c_imiss,c_i_flush_en,c_d_flush_en,
'''
for c in ['ar','aw']:
 ports+=f'    output logic c_{c}_valid,c_{c}_ready,\n    output logic [63:0] c_{c}_addr,\n    output logic [3:0] c_{c}_id,\n    output logic [7:0] c_{c}_len,\n    output logic [2:0] c_{c}_size,\n    output logic [1:0] c_{c}_burst,\n'
ports+='''    output logic c_w_valid,c_w_ready,c_w_last,c_r_valid,c_r_ready,c_r_last,c_b_valid,c_b_ready,
    output logic [63:0] c_w_data,c_r_data,
    output logic [7:0] c_w_strb,
    output logic [3:0] c_r_id,c_b_id,
    output logic [1:0] c_r_resp,c_b_resp,
'''
p=H/'nonblock_actual_dma_system_top.sv';s=p.read_text().replace('    output logic  f_front_valid0,',ports+'    output logic  f_front_valid0,',1)
block='''    // Stage117 test-only deterministic CPU ports into the unmodified CVA6 WT cache.
    // No CPU instruction/MMU execution is claimed. Cache AXI shares existing xbar slv0.
    slv_req_t real_cache_req;
    ariane_pkg::dcache_req_i_t [2:0] real_cpu_req;
    ariane_pkg::dcache_req_o_t [2:0] real_cpu_rsp;
    always_comb begin
      real_cpu_req = '0;
      real_cpu_req[1].address_index = c_addr[ariane_pkg::DCACHE_INDEX_WIDTH-1:0];
      real_cpu_req[1].address_tag = c_addr[riscv::PLEN-1:ariane_pkg::DCACHE_INDEX_WIDTH];
      real_cpu_req[1].data_req = c_req && !c_we;
      real_cpu_req[1].data_be = c_be;
      real_cpu_req[1].data_size = c_size;
      real_cpu_req[1].tag_valid = c_tag_valid;
      real_cpu_req[1].kill_req = c_kill;
      real_cpu_req[2] = real_cpu_req[1];
      real_cpu_req[2].data_req = c_req && c_we;
      real_cpu_req[2].data_we = 1'b1;
      real_cpu_req[2].data_wdata = c_wdata;
    end
    assign c_gnt = c_we ? real_cpu_rsp[2].data_gnt : real_cpu_rsp[1].data_gnt;
    assign c_rvalid = real_cpu_rsp[1].data_rvalid;
    assign c_rdata = real_cpu_rsp[1].data_rdata;
    controller i_real_fence_controller (
      .clk_i,.rst_ni,.v_i(1'b0),.set_pc_commit_o(),.flush_if_o(),.flush_unissued_instr_o(),
      .flush_id_o(),.flush_ex_o(),.flush_bp_o(),.flush_icache_o(c_flush_i),.flush_dcache_o(c_flush_d),
      .flush_dcache_ack_i(c_flush_ack),.flush_tlb_o(),.flush_tlb_vvma_o(),.flush_tlb_gvma_o(),
      .halt_csr_i(1'b0),.halt_o(c_halt),.eret_i(1'b0),.ex_valid_i(1'b0),.set_debug_pc_i(1'b0),
      .resolved_branch_i('0),.flush_csr_i(1'b0),.fence_i_i(c_fence),.fence_i(1'b0),
      .sfence_vma_i(1'b0),.hfence_vvma_i(1'b0),.hfence_gvma_i(1'b0),.flush_commit_i(1'b0)
    );
    wt_cache_subsystem #(.ArianeCfg(ariane_pkg::ArianeDefaultConfig),.AxiAddrWidth(64),.AxiDataWidth(64),
       .AxiIdWidth(4),.axi_req_t(slv_req_t),.axi_rsp_t(slv_resp_t)) i_real_cache (
      .clk_i,.rst_ni,.icache_en_i(1'b1),.icache_flush_i(c_flush_i),.icache_miss_o(c_imiss),
      .icache_areq_i('0),.icache_areq_o(),.icache_dreq_i('0),.icache_dreq_o(),
      .dcache_enable_i(1'b1),.dcache_flush_i(c_flush_d),.dcache_flush_ack_o(c_flush_ack),.dcache_miss_o(c_dmiss),
      .dcache_amo_req_i('0),.dcache_amo_resp_o(),.dcache_req_ports_i(real_cpu_req),.dcache_req_ports_o(real_cpu_rsp),
      .wbuffer_empty_o(c_wb_empty),.wbuffer_not_ni_o(),.axi_req_o(real_cache_req),.axi_resp_i(slv_resps[0])
    );
    assign c_mshr = i_real_cache.i_wt_dcache.i_wt_dcache_missunit.mshr_vld_q;
    assign c_mshr1 = i_real_cache.i_wt_dcache.i_wt_dcache_missunit.mshr_vld_q1;
    assign c_i_flush_en = i_real_cache.i_cva6_icache.flush_en;
    assign c_d_flush_en = i_real_cache.i_wt_dcache.i_wt_dcache_missunit.flush_en;
    initial begin
      if (ariane_pkg::DCACHE_LINE_WIDTH != 128 || ariane_pkg::DCACHE_SET_ASSOC != 8 || ariane_pkg::DCACHE_INDEX_WIDTH != 12)
        $fatal(1,"Genesys2 WT16B configuration conflict");
    end
'''
for c in ['ar','aw']:
 for field in ['valid','addr','id','len','size','burst']:
  source=f'real_cache_req.{c}_valid' if field=='valid' else f'real_cache_req.{c}.{field}'
  block+=f'    assign c_{c}_{field} = {source};\n'
 block+=f'    assign c_{c}_ready = slv_resps[0].{c}_ready;\n'
for c,fs in [('w',['valid','data','strb','last']),('r',['ready']),('b',['ready'])]:
 for f in fs:block+=f'    assign c_{c}_{f} = real_cache_req.{c}_{f if f in ["valid","ready"] else ""}' + (f';\n' if f in ['valid','ready'] else f'\b')
# Replace generated W fields cleanly (structure member spelling).
block=block[:block.index('    assign c_w_valid')] + '''    assign c_w_valid=real_cache_req.w_valid;
    assign c_w_data=real_cache_req.w.data;
    assign c_w_strb=real_cache_req.w.strb;
    assign c_w_last=real_cache_req.w.last;
    assign c_w_ready=slv_resps[0].w_ready;
    assign c_r_ready=real_cache_req.r_ready;
    assign c_b_ready=real_cache_req.b_ready;
    assign c_r_valid=slv_resps[0].r_valid;
    assign c_r_data=slv_resps[0].r.data;
    assign c_r_id=slv_resps[0].r.id;
    assign c_r_last=slv_resps[0].r.last;
    assign c_r_resp=slv_resps[0].r.resp;
    assign c_b_valid=slv_resps[0].b_valid;
    assign c_b_id=slv_resps[0].b.id;
    assign c_b_resp=slv_resps[0].b.resp;
    assign slv_reqs[0] = real_cache_req;
'''
s=s.replace('    assign slv_reqs[0] = cpu_req;',block,1);p.write_text(s)
# Single observation point before the actual common rising edge.
p=H/'inherited_harness.hpp';s=p.read_text().replace('#include "finite_hooks.inc"','#include "finite_hooks.inc"\n#include "cache_hooks.inc"',1).replace('        finite_observe();','        finite_observe();\n        cache_observe();',1);p.write_text(s)
# DMA oracle is independent of CPU source0; retain all DMA conditions unchanged.
p=H/'finite_hooks.inc';s=p.read_text().replace('else{CHECK(port==3,"unexpected DDR read source");','else if(port==3){').replace('CHECK(top->ddr_aw_size==3 && top->ddr_aw_burst==1 && ((top->ddr_aw_id>>4)&3)==2,"unexpected DDR AW");','CHECK(top->ddr_aw_burst==1,"unexpected DDR AW burst");\n        if(((top->ddr_aw_id>>4)&3)==2) {\n        CHECK(top->ddr_aw_size==3,"unexpected DMA DDR AW size");').replace('finite_event("payload_aw",int((id>>2)&1),-1,top->ddr_aw_addr,(uint64_t(top->ddr_aw_len)+1)*8,id);','finite_event("payload_aw",int((id>>2)&1),-1,top->ddr_aw_addr,(uint64_t(top->ddr_aw_len)+1)*8,id);\n        }').replace('if(top->ddr_w_valid && top->ddr_w_ready) {','if(top->ddr_w_valid && top->ddr_w_ready && !caw_q.empty() && ((caw_q.front().id>>4)&3)==2) {').replace('} else finite_event("table_r",-1,-1,p,8,r.id,top->ddr_r_data);','} else if(((r.id>>4)&3)==3) finite_event("table_r",-1,-1,p,8,r.id,top->ddr_r_data);').replace('if(top->ddr_r_last)finite_event','if(top->ddr_r_last && ((r.id>>4)&3)!=0)finite_event').replace('if(top->ddr_b_valid && top->ddr_b_ready) {','if(top->ddr_b_valid && top->ddr_b_ready && ((top->ddr_b_id>>4)&3)==2) {');p.write_text(s)
# Tokens are issued only after actual acknowledged controller acquire by caller.
p=H/'packet_protocol.hpp';s=p.read_text().replace('No CPU/cache or application latency model.','Actual cache acquire receipt required; no CPU instruction/application model.').replace('MODEL_UNCACHED_ASSUMPTION','ACTUAL_CVA6_WT16B_CONTROLLER_INPUT').replace('Token acquire(unsigned pid,uint64_t generation){','Token acquire(unsigned pid,uint64_t generation,uint64_t completed_fence){').replace('// The model directly accesses behavioral memory. This does not execute fence.i.','require(completed_fence!=0,"missing actual acknowledged controller fence receipt");');p.write_text(s)
p=H/'interface-plan.json';a=json.loads(p.read_text());a['interfaces']['initialization']='Backing allocation/zero default pre-reset only. Actual frozen pool, all fixed tables, metadata and headers written through real CPU cache ports before startup acquire/common-ready.';p.write_text(json.dumps(a,indent=2)+'\n')
