#!/usr/bin/env python3
"""Assemble test-local overlays; production inputs are read-only."""
from pathlib import Path
import hashlib,json,shutil

RUN=Path(__file__).resolve().parents[1]
PLATFORM=RUN.parents[1]
OVER=RUN/'combined/overlay'

def replace(s,old,new,n=1):
    assert s.count(old)==n,(old,s.count(old),n)
    return s.replace(old,new)

def put(rel,s):
    p=OVER/rel;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(s)

def main():
    OVER.mkdir(parents=True,exist_ok=True)
    origins=[]
    for task in ['m','o']:
        for f in sorted((RUN/task/'files').rglob('*.sv')):
            rel=f.relative_to(RUN/task/'files')
            if rel.parts[0]=='iommu-platform':rel=Path(*rel.parts[1:])
            dst=OVER/rel;dst.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(f,dst)
            origins.append({'source':str(f.relative_to(RUN)),'output':str(rel),'sha256':hashlib.sha256(f.read_bytes()).hexdigest()})
    bundle='''    output logic m_normal_s1_start_o, m_ptw_active_o,
    output logic m_qualified_start_o, m_start_bank_o, m_root_dram_o,
'''
    connections='''        .m_normal_s1_start_o, .m_ptw_active_o, .m_qualified_start_o,
        .m_start_bank_o, .m_root_dram_o,
'''
    p='tests/nonblock_banked_top.sv';s=(PLATFORM/p).read_text()
    s=replace(s,'    parameter bit NonblockEnable = 1,','    parameter int unsigned FirstPteReadPolicy = 0,\n    parameter bit NonblockEnable = 1,')
    s=replace(s,'    input  logic        clk_i,',bundle+'    input logic first_pte_l0_done_i,\n    input  logic        clk_i,')
    s=replace(s,"ddtp_i.ppn.q        = 44'h10;  // 0x10000 >> 12","ddtp_i.ppn.q        = 44'hA0010; // test tables in actual DRAM window")
    s=replace(s,'    rv_iommu_banked_ctl #(\n','    rv_iommu_banked_ctl #(\n        .FirstPteReadPolicy(FirstPteReadPolicy), .FirstPteAId(4),\n')
    s=replace(s,'    ) i_banked (\n','    ) i_banked (\n        .first_pte_l0_done_i,\n')
    s=replace(s,'    ) i_tl (\n','    ) i_tl (\n'+connections)
    put(p,s)

    p='tests/nonblock_actual_dma_top.sv';s=(PLATFORM/p).read_text()
    s=replace(s,'    parameter bit          NonblockEnable       = 1,','    parameter int unsigned PairPolicy = 0,\n    parameter bit          NonblockEnable       = 1,')
    s=replace(s,'    input  logic clk_i,',bundle+'    output logic first_pte_l0_done_o,\n    input  logic clk_i,')
    # Static configuration; no new software policy interface or counter.
    for i in [0,1]:
        at=s.index('    dma_core_wrap #(',s.index('    dma_core_wrap #(')+1) if i else s.index('    dma_core_wrap #(')
        expression='(PairPolicy == 4 ? 3 : 0)' if i==0 else '(PairPolicy == 2 ? 1 : PairPolicy == 3 ? 2 : PairPolicy == 4 ? 3 : 0)'
        where=at+len('    dma_core_wrap #(\n')
        s=s[:where]+f'        .FirstPteReadPolicy({expression}),\n'+s[where:]
    s=replace(s,'    ) i_dma0 (\n','    ) i_dma0 (\n        .first_pte_l0_done_o(),\n')
    s=replace(s,'    ) i_dma1 (\n','    ) i_dma1 (\n        .first_pte_l0_done_o,\n')
    s=replace(s,'    nonblock_banked_top #(\n','    nonblock_banked_top #(\n        .FirstPteReadPolicy(PairPolicy == 2 ? 1 : PairPolicy == 3 ? 2 : PairPolicy == 4 ? 3 : 0),\n')
    s=replace(s,'    ) i_sys (\n','    ) i_sys (\n'+connections+'        .first_pte_l0_done_i(first_pte_l0_done_o),\n')
    put(p,s)

    p='tests/nonblock_actual_dma_system_top.sv';s=(PLATFORM/p).read_text()
    s=replace(s,'    parameter bit          NonblockEnable       = 1,','    parameter int unsigned PairPolicy = 0,\n    parameter bit          NonblockEnable       = 1,')
    ports='''    output logic dbg_m_raw_start, dbg_m_start, dbg_m_active,
    output logic dbg_m_bank, dbg_m_root_dram,
    output logic [15:0] dbg_m_state,
    output logic dbg_m_arm, dbg_m_commit, dbg_m_timeout, dbg_m_masked,
    output logic [2:0] dbg_common_ds_quota,
    output logic dbg_sel_valid, dbg_sel_ready,
    output logic [5:0] dbg_sel_id,
    output logic [63:0] dbg_sel_addr,
    output logic [7:0] dbg_sel_len,
    output logic dbg_raw_a_valid,
    output logic [63:0] dbg_raw_a_addr,
    output logic [7:0] dbg_raw_a_len,
    output logic dbg_o_done, dbg_o_wait, dbg_o_l1_offer,
    output logic [1:0] dbg_src_drained, dbg_backend_fire,
    output logic [1:0] dbg_local_ar_valid, dbg_local_ar_ready,
    output logic [63:0] dbg_local_ar_addr[2],
    output logic [7:0] dbg_local_ar_len[2],
    output logic [1:0] dbg_local_r_fire, dbg_local_r_last,
    output logic dbg_ptw_local_valid, dbg_ptw_local_ready,
'''
    s=replace(s,'    input  logic        clk_i,',ports+'    input  logic        clk_i,')
    s=replace(s,"start_addr: 64'h0000_0000_0000_0000,\n                        end_addr:   64'h0000_0001_0000_0000}; // 0 .. 4 GiB -> DDR", "start_addr: 64'h0000_0000_8000_0000,\n                        end_addr:   64'h0000_0000_c000_0000}; // actual board DRAM window")
    s=replace(s,'    axi_xbar #(\n','    axi_xbar #(\n        .MFirstPteEnable(PairPolicy == 1), .MFirstPteMstPort(0),\n        .MFirstPteH(64), .CommonDsFirstEnable(1),\n')
    s=replace(s,'    ) i_xbar (\n','''    ) i_xbar (
        .m_enable_i(PairPolicy == 1), .m_pair_qualified_i(1'b1),
        .m_start_i(dbg_m_start), .m_ptw_active_i(dbg_m_active), .m_start_bank_i(dbg_m_bank),
        .m_state_o(dbg_m_state), .m_arm_o(dbg_m_arm), .m_p1_commit_o(dbg_m_commit),
        .m_timeout_o(dbg_m_timeout), .m_masked_o(dbg_m_masked), .m_ds_quota_o(dbg_common_ds_quota),
''')
    s=replace(s,'    nonblock_actual_dma_top #(\n','    nonblock_actual_dma_top #(\n        .PairPolicy(PairPolicy),\n')
    s=replace(s,'    ) i_dma (\n','''    ) i_dma (
        .m_normal_s1_start_o(dbg_m_raw_start), .m_ptw_active_o(dbg_m_active),
        .m_qualified_start_o(dbg_m_start), .m_start_bank_o(dbg_m_bank), .m_root_dram_o(dbg_m_root_dram),
        .first_pte_l0_done_o(dbg_o_done),
''')
    # Observation only. No hierarchically forced state or handshake controls.
    trace='''
    `define PAIR_AR i_xbar.gen_mst_port_mux[0].i_axi_mux.gen_mux
    assign dbg_sel_valid = `PAIR_AR.ar_valid;
    assign dbg_sel_ready = `PAIR_AR.ar_ready;
    assign dbg_sel_id = `PAIR_AR.mst_ar_chan.id;
    assign dbg_sel_addr = `PAIR_AR.mst_ar_chan.addr;
    assign dbg_sel_len = `PAIR_AR.mst_ar_chan.len;
    assign dbg_raw_a_valid = `PAIR_AR.slv_ar_valids[2] &&
                            ((`PAIR_AR.slv_ar_chans[2].id & 6'h3c) == 6'h24);
    assign dbg_raw_a_addr = `PAIR_AR.slv_ar_chans[2].addr;
    assign dbg_raw_a_len = `PAIR_AR.slv_ar_chans[2].len;
    `undef PAIR_AR
    `define PAIR_TC i_dma.i_sys.gen_banked.i_banked.gen_bank[1].i_bank
    assign dbg_o_wait = (`PAIR_TC.state_q == 3'b100) && `PAIR_TC.first_pte_hold_l1;
    assign dbg_o_l1_offer = `PAIR_TC.first_pte_l1 && `PAIR_TC.axi_comp_req.ar_valid;
    `undef PAIR_TC
    // Raw drain predicates; C++ separately tracks actual backend parent accept.
    // Fullbuffer alone does not exclude the parent waiting in the job FIFO.
    assign dbg_src_drained[0] =
        !i_dma.i_dma0.i_idma_backend.busy_o.r_leg_busy &&
        !i_dma.i_dma0.i_idma_backend.r_dp_req_out_valid;
    assign dbg_src_drained[1] =
        !i_dma.i_dma1.i_idma_backend.busy_o.r_leg_busy &&
        !i_dma.i_dma1.i_idma_backend.r_dp_req_out_valid;
    assign dbg_backend_fire[0] = i_dma.i_dma0.be_valid && i_dma.i_dma0.be_ready;
    assign dbg_backend_fire[1] = i_dma.i_dma1.be_valid && i_dma.i_dma1.be_ready;
    assign dbg_local_ar_valid = {i_dma.dma1_mst_req.ar_valid, i_dma.dma0_mst_req.ar_valid};
    assign dbg_local_ar_ready = {i_dma.dma1_mst_rsp.ar_ready, i_dma.dma0_mst_rsp.ar_ready};
    assign dbg_local_ar_addr = '{i_dma.dma0_mst_req.ar.addr, i_dma.dma1_mst_req.ar.addr};
    assign dbg_local_ar_len = '{i_dma.dma0_mst_req.ar.len, i_dma.dma1_mst_req.ar.len};
    assign dbg_local_r_fire[0] = i_dma.dma0_mst_req.r_ready && i_dma.dma0_mst_rsp.r_valid;
    assign dbg_local_r_fire[1] = i_dma.dma1_mst_req.r_ready && i_dma.dma1_mst_rsp.r_valid;
    assign dbg_local_r_last = {i_dma.dma1_mst_rsp.r.last, i_dma.dma0_mst_rsp.r.last};
    assign dbg_ptw_local_valid = u_ptw_ar_valid;
    assign dbg_ptw_local_ready = ptw_resp.ar_ready;
'''
    s=replace(s,'endmodule',trace+'\nendmodule')
    put(p,s)
    (RUN/'combined/assembly.json').write_text(json.dumps({'origins':origins,'wrappers':[str(OVER/'tests'/x) for x in ['nonblock_banked_top.sv','nonblock_actual_dma_top.sv','nonblock_actual_dma_system_top.sv']]},indent=2)+'\n')
    print(OVER)

if __name__=='__main__':main()
