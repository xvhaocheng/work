from pathlib import Path
p=Path(__file__).resolve().parent
ports=['input logic clk_i,rst_ni','input logic [1:0] mode_i','input logic history_i,clear_i','input logic [1:0] permit_i','output logic quiet_o,nfire_o','output logic [23:0] ndid_o','output logic same_off_o,same_gold_o','output logic [15:0] wsi_o','input logic [23:0] t_did']
req={'aw_valid':1,'aw_addr':64,'aw_id':4,'aw_len':8,'aw_size':3,'aw_atop':6,'w_valid':1,'w_data':64,'w_strb':8,'w_last':1,'b_ready':1,'ar_valid':1,'ar_addr':64,'ar_id':4,'ar_len':8,'ar_size':3,'r_ready':1}
rsp={'aw_ready':1,'w_ready':1,'b_valid':1,'b_resp':2,'b_id':4,'ar_ready':1,'r_valid':1,'r_resp':2,'r_id':4,'r_last':1,'r_data':64}
assign=[]
for name in ['t','p','c','d']:
 slave=name in ['t','p']
 for group,d in [('req',req),('rsp',rsp)]:
  for f,w in d.items():
   if name=='p' and f in ['aw_id','ar_id','b_id','r_id']:w=6
   inp=slave==(group=='req');ports.append(f'{"input" if inp else "output"} logic '+(f'[{w-1}:0] ' if w>1 else '')+f'{name}_{f}')
   sf=f.replace('_','.',1) if f.startswith(('aw_','ar_','w_','b_','r_')) and f not in ['aw_valid','ar_valid','w_valid','b_ready','r_ready','b_valid','r_valid','aw_ready','ar_ready','w_ready'] else f
   obj=f'{name}_req' if group=='req' else f'{name}_rsp[0]' if slave else f'{name}_rsp'
   assign.append(f'{obj}.{sf}={name}_{f};' if inp else f'assign {name}_{f}={obj}.{sf};')
head='''// Test-only actual complete IOMMU top, real program AXI and host AXI memory.
`include "axi/typedef.svh"
`include "axi-iommu/typedef.svh"
module pn_top_test import rv_iommu::*; # (parameter bit Gold=1)(
'''+',\n'.join(ports)+');\n'
base=Path('/home/xvhaocheng/work-e6-pn-sw-quiet/paper/ispass2027/exp/E6-pn/sw-quiet/sw_quiet_top.sv').read_text();cfg=base[base.index('    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg'):base.index('    prog_req_t program_req')];cfg=cfg.replace('!OmitFeatures','1').replace('OmitFeatures?rv_iommu_cfg::WSI_ONLY:rv_iommu_cfg::BOTH','rv_iommu_cfg::BOTH')
s=head+'typedef logic [63:0] addr_t;\n`AXI_TYPEDEF_ALL(a,addr_t,logic[3:0],logic[63:0],logic[7:0],logic[0:0])\n`AXI_TYPEDEF_ALL(p,addr_t,logic[5:0],logic[63:0],logic[7:0],logic[0:0])\n'+cfg
s+='`AXI_TYPEDEF_AW_CHAN_EXT_T(t_aw_t,addr_t,logic[3:0],logic[0:0],device_id_t,logic,process_id_t)\n`AXI_TYPEDEF_AR_CHAN_EXT_T(t_ar_t,addr_t,logic[3:0],logic[0:0],device_id_t,logic,process_id_t)\n`AXI_TYPEDEF_REQ_T(t_req_t,t_aw_t,a_w_chan_t,t_ar_t)\nt_req_t t_req;a_req_t c_req[3],d_req[3];a_resp_t t_rsp[3],c_rsp,d_rsp;p_req_t p_req;p_resp_t p_rsp[3];logic[15:0]wsi[3];logic[2:0] quiet,nfire;logic[23:0]ndid[3];\n'
# defaults and all input assignments
s+='always_comb begin t_req=\'0;p_req=\'0;c_rsp=\'0;d_rsp=\'0;\n'+ '\n'.join(a for a in assign if not a.startswith('assign ')).replace('c_req.', 'c_req[0].').replace('d_req.', 'd_req[0].')+'\n'
s+="t_req.aw.stream_id=t_did;t_req.ar.stream_id=t_did;t_req.aw.mmu_valid=1;t_req.ar.mmu_valid=1;t_req.aw.burst=1;t_req.ar.burst=1;p_req.aw.burst=1;p_req.ar.burst=1;end\n"
s+='\n'.join(a.replace('c_req.', 'c_req[0].').replace('d_req.', 'd_req[0].') for a in assign if a.startswith('assign '))+'\n'
# target DID10/11 derived AXI user bits
s+="\n" # Actual axi user mismatch! proper types use ATOP+devID?? top devID readsuser? inspect later
params='.RVIOMMUCfg(Cfg),'+','.join(f'.axi_{kind}_{ch}_chan_t({typ}_{ch}_chan_t)' for kind,typ in [('tr','a'),('comp','a'),('ds','a'),('prog','p')] for ch in ['aw','w','b','ar','r'])+','+','.join(f'.axi_{kind}_{x}_t({typ}_{x}_t)' for kind,typ in [('tr','a'),('comp','a'),('ds','a'),('prog','p')] for x in ['req','resp'])+',.axis_req_t(logic),.axis_rsp_t(logic)'
params=params.replace('.axi_tr_aw_chan_t(a_aw_chan_t)','.axi_tr_aw_chan_t(t_aw_t)').replace('.axi_tr_ar_chan_t(a_ar_chan_t)','.axi_tr_ar_chan_t(t_ar_t)').replace('.axi_tr_req_t(a_req_t)','.axi_tr_req_t(t_req_t)')
pt='.clk_i,.rst_ni,.tr_req_i(t_req),.tr_resp_o(t_rsp[i]),.comp_req_o(c_req[i]),.comp_resp_i(c_rsp),.ds_req_o(d_req[i]),.ds_resp_i(d_rsp),.prog_req_i(p_req),.prog_resp_o(p_rsp[i]),.ats_upstream_req_o(),.ats_upstream_resp_i(1\'b0),.ats_downstream_req_i(1\'b0),.ats_downstream_resp_o(),.wsi_wires_o(wsi[i])'
s+='for(genvar i=0;i<3;i++)begin:lanes\n if(i<2) begin:onoff\nrv_iommu_top #('+params+',.PnEnable(i==0),.HistoryControlEnable(i==0)) dut('+pt+',.pn_mode_i(mode_i),.pn_history_isolate_i(history_i),.pn_policy_clear_i(clear_i),.pn_fullbuffer_jobs_i(permit_i),.pn_n_fire_o(nfire[i]),.pn_n_did_o(ndid[i]),.pn_quiet_o(quiet[i]));\nend else if(Gold)begin:gold\nrv_iommu_top_gold #('+params+') dut('+pt+');\nend else begin assign t_rsp[i]=\'0;assign c_req[i]=\'0;assign d_req[i]=\'0;assign p_rsp[i]=\'0;assign wsi[i]=\'0;end\nend\n'
s+='assign same_off_o=t_rsp[0]===t_rsp[1]&&c_req[0]===c_req[1]&&d_req[0]===d_req[1]&&p_rsp[0]===p_rsp[1]&&wsi[0]===wsi[1];\nassign same_gold_o=t_rsp[0]===t_rsp[2]&&c_req[0]===c_req[2]&&d_req[0]===d_req[2]&&p_rsp[0]===p_rsp[2]&&wsi[0]===wsi[2];\nassign quiet_o=quiet[0],nfire_o=nfire[0],ndid_o=ndid[0],wsi_o=wsi[0];\nendmodule\n'
(p/'pn_top_test.sv').write_text(s)
