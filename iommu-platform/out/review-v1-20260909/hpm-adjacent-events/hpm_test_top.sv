module hpm_test_top (
    input logic clk, rst_n, valid, inhibit, filter_did,
    input logic [23:0] did,
    input logic [8:0] event_mask,
    input logic [14:0] event_id,
    output logic [63:0] misses,
    output logic [62:0] cycles
);
    import rv_iommu_reg_pkg::*;
    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg = '{NumHpmCounters: 1, default: '0};
    rv_iommu::hpm_event_t event_data;
    iommu_reg2hw_iocountinh_reg_t inh;
    iommu_reg2hw_iohpmcycles_reg_t cyc_in;
    iommu_reg2hw_iohpmctr_reg_t [0:0] ctr_in;
    iommu_reg2hw_iohpmevt_reg_t [0:0] evt_in;
    iommu_hw2reg_iohpmcycles_reg_t cyc_out;
    iommu_hw2reg_iohpmctr_reg_t [0:0] ctr_out;
    iommu_hw2reg_iohpmevt_reg_t [0:0] evt_out;
    always_comb begin
        event_data = '0;
        event_data.valid = valid;
        event_data.etype_msk = event_mask;
        event_data.filters.did = did;
        inh = '0;
        inh.hpm.q[0] = inhibit;
        cyc_in = '0;
        cyc_in.counter.q = cycles;
        ctr_in = '0;
        ctr_in[0].counter.q = misses;
        evt_in = '0;
        evt_in[0].eventid.q = event_id;
        evt_in[0].dv_gscv.q = filter_did;
        evt_in[0].did_gscid.q = 10;
    end
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            misses <= '0;
            cycles <= '0;
        end else begin
            if (ctr_out[0].counter.de) misses <= ctr_out[0].counter.d;
            if (cyc_out.counter.de) cycles <= cyc_out.counter.d;
        end
    end
    rv_iommu_hpm #(.RVIOMMUCfg(Cfg)) dut (
        .clk_i(clk), .rst_ni(rst_n), .events_i(event_data),
        .iocountinh_i(inh), .iohpmcycles_i(cyc_in), .iohpmctr_i(ctr_in), .iohpmevt_i(evt_in),
        .iohpmcycles_o(cyc_out), .iohpmctr_o(ctr_out), .iohpmevt_o(evt_out),
        .ipsr_pmip_o(), .ipsr_pmip_wen_o()
    );
endmodule
