#!/usr/bin/env python3
"""Derive independent passive top / simulation-only ORACLE overlays from prior frozen inputs."""
from pathlib import Path
import difflib
BASE=Path(__file__).resolve().parents[2]/'invalidation-interference-20261017'
RUN=Path(__file__).resolve().parents[1]/'runner'
def repl(s,a,b,n=1):
    assert s.count(a)==n,(a,s.count(a));return s.replace(a,b)
ports='''
`ifndef SYNTHESIS
    // Simulation-only ORACLE: omniscient immediate removal, no software interface.
    input logic oracle_remove_i,
    input logic [19:0] oracle_pscid_i,
    input logic [26:0] oracle_vpn_i,
`endif
'''
conn='''
`ifndef SYNTHESIS
            .oracle_remove_i (oracle_remove_i),
            .oracle_pscid_i (oracle_pscid_i),
            .oracle_vpn_i (oracle_vpn_i),
`endif
'''
paths=['cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv','cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv','cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv']
for rel in paths:
    original=(BASE/'frozen'/rel).read_text();s=original
    if rel.endswith('rv_iommu_top.sv'):
        s=repl(s,'    // Translation Request Interface (Slave)',ports+'\n    // Translation Request Interface (Slave)')
        s=repl(s,') i_rv_iommu_tl_wrap (',') i_rv_iommu_tl_wrap ('+conn,2)
    elif rel.endswith('rv_iommu_tl_wrap.sv'):
        s=repl(s,'    // Transaction controller request port',ports+'\n    // Transaction controller request port')
        s=repl(s,') i_rv_iommu_iotlb (',') i_rv_iommu_iotlb ('+conn)
    else:
        s=repl(s,'    // Flush',ports+'\n    // Flush')
        a='''        end
    end

    //-----------------------------------------------
    // PLRU - Pseudo Least Recently Used Replacement'''
        b='''        end
`ifndef SYNTHESIS
        // Last assignment wins, including a same-edge fill of this exact key.
        // This is deliberately not an architected IOTINVAL/CQ implementation.
        if (oracle_remove_i) begin
            for (int unsigned i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
                if (tags_n[i].valid && tags_n[i].en_1S && !tags_n[i].en_2S &&
                    tags_n[i].pscid == oracle_pscid_i &&
                    {tags_n[i].vpn2[8:0], tags_n[i].vpn1, tags_n[i].vpn0} == oracle_vpn_i)
                    tags_n[i].valid = 1'b0;
            end
        end
`endif
    end

    //-----------------------------------------------
    // PLRU - Pseudo Least Recently Used Replacement'''
        s=repl(s,a,b)
    out=RUN/'rtl-overlay'/rel;out.parent.mkdir(parents=True,exist_ok=True);out.write_text(s)
    diff=out.with_suffix('.sv.diff');diff.write_text(''.join(difflib.unified_diff(original.splitlines(True),s.splitlines(True),fromfile='frozen/'+rel,tofile='rtl-overlay/'+rel)))
s=(BASE/'runner/invalidation_dma_top.sv').read_text()
s=repl(s,'    input logic rst_ni,','    input logic rst_ni,'+ports)
s=repl(s,'    output logic dbg_ptw_edge_trigger\n','''    output logic dbg_ptw_edge_trigger,
    // AXI mask bits: ARV,ARR,RV,RR,RL,AWV,AWR,WV,WR,BV,BR (bit0 first).
    output logic [2:0] dbg_cq_next_state,
    output logic [1:0] dbg_cq_wr_state,
    output logic dbg_cq_cmd_complete,
    output logic dbg_iofence_decode,
    output logic dbg_iofence_complete,
    output logic dbg_iofence_av,
    output logic dbg_iofence_pr,
    output logic dbg_iofence_pw,
    output logic dbg_tl_active,
    output logic [23:0] dbg_tl_active_did,
    output logic [63:0] dbg_tl_active_iova,
    output logic [3:0] dbg_tl_active_ttype,
    output logic [2:0] dbg_tl_next_state,
    output logic dbg_tl_resp_valid,
    output logic dbg_tl_resp_ready,
    output logic [10:0] dbg_cq_bus,
    output logic [10:0] dbg_ptw_bus,
    output logic [10:0] dbg_ddtw_bus,
    output logic [10:0] dbg_ds_bus,
    output logic [10:0] dbg_comp_bus,
    output logic [10:0] dbg_dma0_bus,
    output logic [10:0] dbg_dma1_bus,
    output logic [63:0] dbg_cq_ar_addr,
    output logic [63:0] dbg_ptw_ar_addr,
    output logic [63:0] dbg_ddtw_ar_addr,
    output logic [63:0] dbg_ds_ar_addr,
    output logic [63:0] dbg_comp_ar_addr,
    output logic [63:0] dbg_comp_aw_addr,
    output logic [3:0] dbg_ds_ar_id,
    output logic [3:0] dbg_ds_r_id,
    output logic [3:0] dbg_comp_ar_id,
    output logic [3:0] dbg_comp_aw_id,
    output logic [3:0] dbg_comp_r_id,
    output logic [3:0] dbg_comp_b_id,
    output logic dbg_iotlb_update,
    output logic dbg_iotlb_fill,
    output logic [26:0] dbg_iotlb_update_vpn,
    output logic [19:0] dbg_iotlb_update_pscid,
    output logic [15:0] dbg_iotlb_replace_en,
    output logic [15:0] dbg_iotlb_lu_hit_vec,
    output logic [14:0] dbg_iotlb_plru
''')
s=repl(s,'  ) i_iommu(.clk_i,.rst_ni,.tr_req_i(tr_req),.tr_resp_o(tr_rsp),','''  ) i_iommu(.clk_i,.rst_ni,
`ifndef SYNTHESIS
    .oracle_remove_i,.oracle_pscid_i,.oracle_vpn_i,
`endif
    .tr_req_i(tr_req),.tr_resp_o(tr_rsp),''')
s=repl(s,'`undef TL\n','''  // These signals only leave this independent simulation wrapper.
  assign dbg_cq_next_state=`CQ.state_n;
  assign dbg_cq_wr_state=`CQ.wr_state_q;
  assign dbg_cq_cmd_complete=((`CQ.state_q==3)||(`CQ.state_q==4))&&(`CQ.state_n==0);
  assign dbg_iofence_decode=(`CQ.state_q==3)&&(`CQ.cmd_q[6:0]==7'd2);
  assign dbg_iofence_complete=dbg_cq_cmd_complete&&(`CQ.cmd_q[6:0]==7'd2);
  assign dbg_iofence_av=`CQ.cmd_iofence.av;
  assign dbg_iofence_pr=`CQ.cmd_iofence.pr;
  assign dbg_iofence_pw=`CQ.cmd_iofence.pw;
  assign dbg_tl_active=`TL.req_valid;
  assign dbg_tl_active_did=`TL.req_data.did;
  assign dbg_tl_active_iova=`TL.req_data.iova;
  assign dbg_tl_active_ttype=`TL.req_data.ttype;
  assign dbg_tl_next_state=`TL.state_n;
  assign dbg_tl_resp_valid=i_iommu.trans_resp_valid;
  assign dbg_tl_resp_ready=i_iommu.trans_resp_ready;
`define OBS_AXI_MASK(req,rsp) {req.b_ready,rsp.b_valid,rsp.w_ready,req.w_valid,rsp.aw_ready,req.aw_valid,rsp.r.last,req.r_ready,rsp.r_valid,rsp.ar_ready,req.ar_valid}
  assign dbg_cq_bus=`OBS_AXI_MASK(i_iommu.cq_axi_req,i_iommu.cq_axi_resp);
  assign dbg_ptw_bus=`OBS_AXI_MASK(i_iommu.ptw_axi_req,i_iommu.ptw_axi_resp);
  assign dbg_ddtw_bus=`OBS_AXI_MASK(i_iommu.ddtw_axi_req,i_iommu.ddtw_axi_resp);
  assign dbg_ds_bus=`OBS_AXI_MASK(ds_req,ds_rsp);
  assign dbg_comp_bus=`OBS_AXI_MASK(comp_req,comp_rsp);
  assign dbg_dma0_bus=`OBS_AXI_MASK(dma_req[0],dma_rsp[0]);
  assign dbg_dma1_bus=`OBS_AXI_MASK(dma_req[1],dma_rsp[1]);
`undef OBS_AXI_MASK
  assign dbg_cq_ar_addr=i_iommu.cq_axi_req.ar.addr;
  assign dbg_ptw_ar_addr=i_iommu.ptw_axi_req.ar.addr;
  assign dbg_ddtw_ar_addr=i_iommu.ddtw_axi_req.ar.addr;
  assign dbg_ds_ar_addr=ds_req.ar.addr;
  assign dbg_comp_ar_addr=comp_req.ar.addr;
  assign dbg_comp_aw_addr=comp_req.aw.addr;
  assign dbg_ds_ar_id=ds_req.ar.id;
  assign dbg_ds_r_id=ds_rsp.r.id;
  assign dbg_comp_ar_id=comp_req.ar.id;
  assign dbg_comp_aw_id=comp_req.aw.id;
  assign dbg_comp_r_id=comp_rsp.r.id;
  assign dbg_comp_b_id=comp_rsp.b.id;
  assign dbg_iotlb_update=`IOTLB.update_i.update;
  assign dbg_iotlb_fill=`IOTLB.update_i.update&&!`IOTLB.inval_i.inval_vma&&!`IOTLB.inval_i.inval_gvma;
  assign dbg_iotlb_update_vpn=`IOTLB.update_i.vpn[26:0];
  assign dbg_iotlb_update_pscid=`IOTLB.update_i.pscid;
  assign dbg_iotlb_replace_en=`IOTLB.replace_en;
  assign dbg_iotlb_lu_hit_vec=`IOTLB.lu_hit;
  assign dbg_iotlb_plru=`IOTLB.plru_tree_q;
`undef TL
''')
(RUN/'invalidation_dma_top.sv').write_text(s)
print('top and three simulation-only overlays written')

# Version 2: bounded AV0/ATS-off PR/PW completion in the independent simulation.
# Done inputs are provided by the model's explicit device-request cohort tracker.
fence_ports='''
`ifndef SYNTHESIS
    input logic sim_fence_read_done_i,
    input logic sim_fence_write_done_i,
`endif
'''
fence_conn='''
`ifndef SYNTHESIS
        .sim_fence_read_done_i (sim_fence_read_done_i),
        .sim_fence_write_done_i (sim_fence_write_done_i),
`endif
'''
rel='cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv'
p=RUN/'rtl-overlay'/rel;s=p.read_text()
s=repl(s,'    // Translation Request Interface (Slave)',fence_ports+'\n    // Translation Request Interface (Slave)')
s=repl(s,') i_rv_iommu_sw_if_wrap (',') i_rv_iommu_sw_if_wrap ('+fence_conn)
p.write_text(s)
rel='cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_sw_if_wrap.sv'
s=(BASE/'frozen'/rel).read_text()
s=repl(s,'    // From Prog IF',fence_ports+'\n    // From Prog IF')
s=repl(s,') i_rv_iommu_cq_handler (',') i_rv_iommu_cq_handler ('+fence_conn)
p=RUN/'rtl-overlay'/rel;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(s)
rel='cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv'
s=(BASE/'frozen'/rel).read_text()
s=repl(s,'    // cqb',fence_ports+'\n    // cqb')
s=repl(s,'        ERROR           // 101','''        ERROR           // 101
`ifndef SYNTHESIS
        , WAIT_PRPW      // 110: model cohort has not reached the global point
`endif''')
s=repl(s,'                        cq_head_o       = (cq_head_i + 1) & idx_mask;','''`ifndef SYNTHESIS
                        // Low 64b was latched on the first R beat. Defer only
                        // PR/PW IOFENCE completion visibility; legacy PR=PW=0
                        // retains the original REGISTER CQH advance.
                        if (!(cmd_q[6:0] == rv_iommu::IOFENCE && (cmd_q[12] || cmd_q[13])))
`endif
                        cq_head_o       = (cq_head_i + 1) & idx_mask;''')
a='''                            // TODO: Add IOFENCE support with PR and PW using R/W counters'''
b='''                            // TODO: Add IOFENCE support with PR and PW using R/W counters
`ifndef SYNTHESIS
                            // The independent test model supplies the completion
                            // of a fixed pre-fence device-request cohort.
                            // AV=1 + PR/PW and ATS are outside this test hook.
`endif'''
s=repl(s,a,b)
a='''                            end

                        end
                    end

                    /*
                        IODIR.INVAL_DDT'''
b='''                            end
`ifndef SYNTHESIS
                            if (cmd_iofence.pr || cmd_iofence.pw) begin
                                if (cmd_iofence.av || atsinval_inflight_i) begin
                                    cqcsr_ill_o = 1'b1;
                                    cqcsr_ill_wen_o = 1'b1;
                                    state_n = ERROR;
                                end else if ((cmd_iofence.pr && !sim_fence_read_done_i) ||
                                             (cmd_iofence.pw && !sim_fence_write_done_i)) begin
                                    state_n = WAIT_PRPW;
                                end else begin
                                    cq_head_o = (cq_head_i + 1) & idx_mask;
                                    state_n = IDLE;
                                end
                            end
`endif

                        end
                    end

                    /*
                        IODIR.INVAL_DDT'''
s=repl(s,a,b)
a='''            // When an error occurs, the CQ stops processing commands until SW clear all error bits'''
b='''`ifndef SYNTHESIS
            // DECODE runs once; only cohort completion is polled here.
            WAIT_PRPW: begin
                if ((!cmd_iofence.pr || sim_fence_read_done_i) &&
                    (!cmd_iofence.pw || sim_fence_write_done_i)) begin
                    cq_head_o = (cq_head_i + 1) & idx_mask;
                    state_n = IDLE;
                end
            end
`endif

            // When an error occurs, the CQ stops processing commands until SW clear all error bits'''
s=repl(s,a,b)
p=RUN/'rtl-overlay'/rel;p.write_text(s)
# Regenerate all overlay diffs against the immutable frozen originals.
for p in sorted((RUN/'rtl-overlay').rglob('*.sv')):
    rel=p.relative_to(RUN/'rtl-overlay');original=(BASE/'frozen'/rel).read_text();s=p.read_text()
    p.with_suffix('.sv.diff').write_text(''.join(difflib.unified_diff(original.splitlines(True),s.splitlines(True),fromfile='frozen/'+str(rel),tofile='rtl-overlay/'+str(rel))))
p=RUN/'invalidation_dma_top.sv';s=p.read_text()
s=repl(s,'    input logic rst_ni,','    input logic rst_ni,'+fence_ports)
s=repl(s,'    output logic dbg_iofence_decode,','    output logic dbg_iofence_wait,\n    output logic dbg_iofence_decode,')
s=repl(s,'    .oracle_remove_i,.oracle_pscid_i,.oracle_vpn_i,','    .oracle_remove_i,.oracle_pscid_i,.oracle_vpn_i,\n    .sim_fence_read_done_i,.sim_fence_write_done_i,')
s=repl(s,'  assign dbg_cq_cmd_complete=((`CQ.state_q==3)||(`CQ.state_q==4))&&(`CQ.state_n==0);','  assign dbg_cq_cmd_complete=((`CQ.state_q==3)||(`CQ.state_q==4)||(`CQ.state_q==6))&&(`CQ.state_n==0);')
s=repl(s,'  assign dbg_iofence_decode=', '  assign dbg_iofence_wait=(`CQ.state_q==6);\n  assign dbg_iofence_decode=')
p.write_text(s)
print('PR/PW hook and five overlays written; legacy PR=PW=0 path preserved')
