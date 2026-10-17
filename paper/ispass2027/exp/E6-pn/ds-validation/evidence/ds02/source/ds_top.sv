// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Test-only flattening and state taps; no production feedback.
`include "axi/typedef.svh"
module ds_top import rv_iommu::*; (
    input logic clk_i,rst_ni,msi_mode_i,msi_init_i,
    input logic [8:0] ar_valid_i,aw_valid_i,w_valid_i,r_ready_i,b_ready_i,w_last_i,
    input logic [8:0][63:0] ar_addr_i,aw_addr_i,w_data_i,
    input logic [8:0][7:0] ar_len_i,aw_len_i,w_strb_i,
    input logic ds_ar_ready_i,ds_aw_ready_i,ds_w_ready_i,ds_r_valid_i,ds_b_valid_i,
    input logic [3:0] ds_r_id_i,ds_b_id_i,
    input logic [63:0] ds_r_data_i,
    input logic ds_r_last_i,
    input logic [1:0] ds_r_resp_i,ds_b_resp_i,
    input logic unit_read_issue_i,unit_read_last_i,unit_write_issue_i,unit_write_done_i,
    output logic equal_on_o,equal_off_o,buffers_quiet_o,transport_quiet_o,off_quiet_o,
    output logic [8:0] ar_ready_o,aw_ready_o,w_ready_o,r_valid_o,b_valid_o,
    output logic ds_ar_valid_o,ds_aw_valid_o,ds_w_valid_o,ds_r_ready_o,ds_b_ready_o,
    output logic [3:0] ds_ar_id_o,ds_aw_id_o,
    output logic [63:0] ds_ar_addr_o,ds_aw_addr_o,ds_w_data_o,
    output logic [7:0] ds_ar_len_o,ds_aw_len_o,ds_w_strb_o,
    output logic ds_w_last_o,
    output logic [7:0] read_debt_o,write_debt_o,unit_read_o,unit_write_o,
    output logic bad_o,unit_bad_o,unit_quiet_o,owner_empty_o,
    output logic ar_spill_o,aw_spill_o,w_spill_o,r_spill_o,b_spill_o,
    output logic msi_idle_o,msi_fill_o,msi_error_o
);
    typedef logic [63:0] addr_t;
    typedef logic [55:0] paddr_t;
    `AXI_TYPEDEF_ALL(axi,addr_t,logic[3:0],logic[63:0],logic[7:0],logic[0:0])
    axi_req_t prod[9],ds[3],msi_req;
    axi_resp_t rsp[3][9],ds_rsp;
    localparam int ArId[9]='{1,2,0,4,5,3,0,6,0};
    localparam int AwId[9]='{0,0,0,0,2,0,1,4,3};
    always_comb begin
        for(int k=0;k<9;k++) begin
            prod[k]='0;
            prod[k].ar_valid=ar_valid_i[k];prod[k].ar.id=4'(ArId[k]);
            prod[k].ar.addr=ar_addr_i[k];prod[k].ar.len=ar_len_i[k];
            prod[k].ar.size=3;prod[k].ar.burst=axi_pkg::BURST_INCR;
            prod[k].aw_valid=aw_valid_i[k];prod[k].aw.id=4'(AwId[k]);
            prod[k].aw.addr=aw_addr_i[k];prod[k].aw.len=aw_len_i[k];
            prod[k].aw.size=3;prod[k].aw.burst=axi_pkg::BURST_INCR;
            prod[k].w_valid=w_valid_i[k];prod[k].w.data=w_data_i[k];
            prod[k].w.strb=w_strb_i[k];prod[k].w.last=w_last_i[k];
            prod[k].r_ready=r_ready_i[k];prod[k].b_ready=b_ready_i[k];
        end
        if(msi_mode_i) prod[3]=msi_req;
        ds_rsp='0;
        ds_rsp.ar_ready=ds_ar_ready_i;ds_rsp.aw_ready=ds_aw_ready_i;ds_rsp.w_ready=ds_w_ready_i;
        ds_rsp.r_valid=ds_r_valid_i;ds_rsp.r.id=ds_r_id_i;ds_rsp.r.data=ds_r_data_i;
        ds_rsp.r.last=ds_r_last_i;ds_rsp.r.resp=ds_r_resp_i;
        ds_rsp.b_valid=ds_b_valid_i;ds_rsp.b.id=ds_b_id_i;ds_rsp.b.resp=ds_b_resp_i;
    end
    `define PARAMS .aw_chan_t(axi_aw_chan_t),.w_chan_t(axi_w_chan_t),.b_chan_t(axi_b_chan_t),.ar_chan_t(axi_ar_chan_t),.r_chan_t(axi_r_chan_t),.axi_req_t(axi_req_t),.axi_resp_t(axi_resp_t)
    `define PORTS .clk_i,.rst_ni,.ds_resp_i(ds_rsp),.ds_req_o(ds[i]),.ddtw_req_i(prod[0]),.ddtw_resp_o(rsp[i][0]),.pdtw_req_i(prod[1]),.pdtw_resp_o(rsp[i][1]),.ptw_req_i(prod[2]),.ptw_resp_o(rsp[i][2]),.msiptw_req_i(prod[3]),.msiptw_resp_o(rsp[i][3]),.mrif_handler_req_i(prod[4]),.mrif_handler_resp_o(rsp[i][4]),.cq_req_i(prod[5]),.cq_resp_o(rsp[i][5]),.fq_req_i(prod[6]),.fq_resp_o(rsp[i][6]),.pq_req_i(prod[7]),.pq_resp_o(rsp[i][7]),.msi_ig_req_i(prod[8]),.msi_ig_resp_o(rsp[i][8])
    for(genvar i=0;i<3;i++) begin : lanes
        if(i==0) begin : enabled
            rv_iommu_ds_if #(`PARAMS,.PnEnable(1)) dut (`PORTS,
                .pn_buffers_quiet_o(buffers_quiet_o),.pn_transport_quiet_o(transport_quiet_o));
        end else if(i==1) begin : disabled
            rv_iommu_ds_if #(`PARAMS) dut (`PORTS,.pn_buffers_quiet_o(),.pn_transport_quiet_o(off_quiet_o));
        end else begin : original
            rv_iommu_ds_if_gold #(`PARAMS) dut (`PORTS);
        end
    end
    always_comb begin
        equal_on_o=(ds[0]===ds[2]);equal_off_o=(ds[1]===ds[2]);
        for(int k=0;k<9;k++) begin
            equal_on_o &= (rsp[0][k]===rsp[2][k]);equal_off_o &= (rsp[1][k]===rsp[2][k]);
            ar_ready_o[k]=rsp[0][k].ar_ready;aw_ready_o[k]=rsp[0][k].aw_ready;
            w_ready_o[k]=rsp[0][k].w_ready;r_valid_o[k]=rsp[0][k].r_valid;b_valid_o[k]=rsp[0][k].b_valid;
        end
    end
    assign ds_ar_valid_o=ds[0].ar_valid,ds_aw_valid_o=ds[0].aw_valid,ds_w_valid_o=ds[0].w_valid;
    assign ds_r_ready_o=ds[0].r_ready,ds_b_ready_o=ds[0].b_ready;
    assign ds_ar_id_o=ds[0].ar.id,ds_aw_id_o=ds[0].aw.id;
    assign ds_ar_addr_o=ds[0].ar.addr,ds_aw_addr_o=ds[0].aw.addr,ds_w_data_o=ds[0].w.data;
    assign ds_ar_len_o=ds[0].ar.len,ds_aw_len_o=ds[0].aw.len,ds_w_strb_o=ds[0].w.strb,ds_w_last_o=ds[0].w.last;
    assign read_debt_o=lanes[0].enabled.dut.gen_pn_quiet.i_debt.read_q;
    assign write_debt_o=lanes[0].enabled.dut.gen_pn_quiet.i_debt.write_q;
    assign bad_o=lanes[0].enabled.dut.gen_pn_quiet.i_debt.bad_q;
    assign owner_empty_o=lanes[0].enabled.dut.w_fifo_empty;
    assign ar_spill_o=ds[0].ar_valid,aw_spill_o=ds[0].aw_valid,w_spill_o=ds[0].w_valid;
    assign r_spill_o=lanes[0].enabled.dut.r_valid,b_spill_o=lanes[0].enabled.dut.b_valid;
    rv_iommu_ds_debt unit_dut (.clk_i,.rst_ni,.read_issue_i(unit_read_issue_i),
        .read_last_i(unit_read_last_i),.write_issue_i(unit_write_issue_i),.write_done_i(unit_write_done_i),.quiet_o(unit_quiet_o));
    assign unit_read_o=unit_dut.read_q,unit_write_o=unit_dut.write_q,unit_bad_o=unit_dut.bad_q;
    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg='{default:'0,PAddrWidth:56,AxiDataWidth:64,AxiIdWidth:4,MSITrans:rv_iommu_cfg::MSI_BT_MRIF,IGS:rv_iommu_cfg::BOTH};
    iotlb_up_t iu; mrifc_up_t mu;
    rv_iommu_msiptw #(.RVIOMMUCfg(Cfg),.addr_t(addr_t),.paddr_t(paddr_t),.axi_req_t(axi_req_t),.axi_resp_t(axi_resp_t)) walker (
        .clk_i,.rst_ni,.init_msi_ptw_i(msi_init_i),.error_o(msi_error_o),.cause_code_o(),.ignore_o(),
        .req_iova_i(64'h28000000),.pscid_i('0),.gscid_i('0),.en_1S_i(1'b0),.is_rx_i(1'b0),
        .msiptp_ppn_i(44'h20),.msi_addr_mask_i('0),.content_1S_i('0),.iotlb_update_o(iu),.mrifc_update_o(mu),
        .mem_resp_i(rsp[0][3]),.mem_req_o(msi_req));
    assign msi_idle_o=(walker.bt_state_q==0)&&(walker.gen_mrif_support.mrif_state_q==0);
    assign msi_fill_o=iu.update;
    `undef PARAMS
    `undef PORTS
endmodule
