// Real controller/FIFO/demux integration; translation engine is externally driven.
`include "axi-iommu/typedef.svh"
module e3_queue_controller_top #(
    parameter bit E3Enable = 1,
    // Nine bits exercise IDs whose low eight bits alias the observed victim ID.
    parameter int IdWidth = 9
) (
    input logic clk_i, rst_ni,
    input logic ar_valid_i, aw_valid_i,
    input logic [IdWidth-1:0] ar_id_i, aw_id_i,
    input logic [23:0] ar_did_i, aw_did_i,
    input logic [63:0] ar_addr_i, aw_addr_i,
    input logic trans_ready_i, response_valid_i,
    output logic ar_ready_o, aw_ready_o, request_o, response_ready_o,
    output logic [23:0] request_did_o,
    output logic [63:0] request_addr_o,
    output logic [31:0] waiting_o,
    output logic queue_error_o, select_o, select_write_o, head_valid_o,
    output logic [7:0] select_id_o, head_id_o,
    output logic [23:0] select_did_o, head_did_o,
    output logic [2:0] state_o,
    output logic enabled_o,
    // Actual response register, qualified by an externally modeled A S1 miss.
    input logic delay_enable_i, delay_arm_i, active_i, a_miss_i, delay128_i,
    input logic memory_valid_i, walker_ready_i,
    output logic memory_ready_o, walker_valid_o, capture_o, available_o, consume_o,
    output logic delay_error_o,
    output logic [7:0] extra_o,
    // Separate real queue instance permits impossible-controller error injection.
    input logic leaf_push_i, leaf_select_i,
    output logic [31:0] leaf_waiting_o,
    output logic leaf_error_o
);
    typedef logic [63:0] addr_t;
    typedef logic [55:0] paddr_t;
    typedef logic [IdWidth-1:0] id_t;
    typedef logic [63:0] data_t;
    typedef logic [7:0] strb_t;
    typedef logic user_t;
    `AXI_TYPEDEF_EXT_ALL(tr, addr_t, id_t, data_t, strb_t, user_t,
                        rv_iommu::device_id_t, logic, rv_iommu::process_id_t)
    `AXI_TYPEDEF_ALL(comp, addr_t, id_t, data_t, strb_t, user_t)
    typedef struct packed {
        addr_t iova;
        rv_iommu::device_id_t did;
        logic pid_valid;
        rv_iommu::process_id_t pid;
        rv_iommu::ttype_t ttype;
        logic priv, is_debug;
    } trans_req_t;
    typedef struct packed {
        logic error, ignore;
        paddr_t spaddr;
        logic is_mrif;
        rv_iommu::mrifc_content_t mrif_data;
    } trans_resp_t;
    function automatic rv_iommu_cfg::rv_iommu_cfg_t config_value();
        rv_iommu_cfg::rv_iommu_cfg_t cfg = rv_iommu_cfg::NullCfg;
        cfg.NumOutstandingTrans = 8;
        cfg.WFifoDepth = 32;
        cfg.MrifFifoDepth = 8;
        cfg.AxiIdWidth = IdWidth;
        cfg.AxiDataWidth = 64;
        cfg.AxiAddrWidth = 64;
        cfg.PAddrWidth = 56;
        cfg.MSITrans = rv_iommu_cfg::MSI_DISABLED;
        cfg.InclAxiBC = 1;
        return cfg;
    endfunction
    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg = config_value();
    tr_req_t tr_req;
    tr_resp_t tr_resp;
    comp_req_t comp_req;
    comp_resp_t comp_resp;
    trans_req_t trans_req;
    trans_resp_t trans_resp;
    rv_iommu::e3_ctl_t ctl;
    logic read_pending_q;
    id_t read_id_q;
    always_comb begin
        tr_req = '0;
        tr_req.ar_valid = ar_valid_i;
        tr_req.ar.id = ar_id_i;
        tr_req.ar.stream_id = ar_did_i;
        tr_req.ar.addr = ar_addr_i;
        tr_req.ar.size = 3;
        tr_req.ar.burst = axi_pkg::BURST_INCR;
        tr_req.aw_valid = aw_valid_i;
        tr_req.aw.id = aw_id_i;
        tr_req.aw.stream_id = aw_did_i;
        tr_req.aw.addr = aw_addr_i;
        tr_req.aw.size = 3;
        tr_req.aw.burst = axi_pkg::BURST_INCR;
        tr_req.r_ready = 1;
        tr_req.b_ready = 1;
        comp_resp = '0;
        comp_resp.ar_ready = !read_pending_q;
        comp_resp.aw_ready = 1;
        comp_resp.w_ready = 1;
        comp_resp.r_valid = read_pending_q;
        comp_resp.r.id = read_id_q;
        comp_resp.r.last = 1;
        trans_resp = '0;
        trans_resp.spaddr = 56'h80001000;
    end
    // A bounded, legal one-beat read responder behind the production AXI demux.
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin read_pending_q <= 0; read_id_q <= 0; end
        else begin
            if (read_pending_q && comp_req.r_ready) read_pending_q <= 0;
            if (comp_req.ar_valid && comp_resp.ar_ready) begin
                read_pending_q <= 1;
                read_id_q <= comp_req.ar.id;
            end
        end
    end
    rv_iommu_trans_ctl_wrap #(
        .RVIOMMUCfg(Cfg), .E3Enable(E3Enable), .addr_t(addr_t), .paddr_t(paddr_t),
        .trans_req_data_t(trans_req_t), .trans_resp_data_t(trans_resp_t), .fault_data_t(logic),
        .axi_tr_aw_chan_t(tr_aw_chan_t), .axi_tr_w_chan_t(tr_w_chan_t),
        .axi_tr_ar_chan_t(tr_ar_chan_t), .axi_tr_req_t(tr_req_t), .axi_tr_resp_t(tr_resp_t),
        .axi_comp_aw_chan_t(comp_aw_chan_t), .axi_comp_w_chan_t(comp_w_chan_t),
        .axi_comp_b_chan_t(comp_b_chan_t), .axi_comp_ar_chan_t(comp_ar_chan_t),
        .axi_comp_r_chan_t(comp_r_chan_t), .axi_comp_req_t(comp_req_t),
        .axi_comp_resp_t(comp_resp_t), .axi_ds_req_t(comp_req_t), .axi_ds_resp_t(comp_resp_t)
    ) dut (
        .clk_i, .rst_ni, .tr_axi_req_i(tr_req), .tr_axi_resp_o(tr_resp),
        .comp_axi_resp_i(comp_resp), .comp_axi_req_o(comp_req),
        .mrif_axi_req_o(), .mrif_axi_resp_i('0), .trans_req_data_o(trans_req),
        .trans_req_valid_o(request_o), .trans_req_ready_i(trans_ready_i),
        .trans_resp_data_i(trans_resp), .trans_resp_valid_i(response_valid_i),
        .trans_resp_ready_o(response_ready_o), .mrif_fault_data_o(),
        .mrif_fault_valid_o(), .mrif_fault_ready_i(1'b1), .in_flight_o(), .e3_o(ctl)
    );
    assign ar_ready_o = tr_resp.ar_ready;
    assign aw_ready_o = tr_resp.aw_ready;
    assign request_did_o = trans_req.did;
    assign request_addr_o = trans_req.iova;
    assign waiting_o = ctl.victim_waiting;
    assign queue_error_o = ctl.queue_error;
    assign select_o = ctl.select_valid;
    assign select_write_o = ctl.select_write;
    assign select_id_o = ctl.select_id;
    assign select_did_o = ctl.select_did;
    assign head_valid_o = ctl.head_valid;
    assign head_id_o = ctl.head_id;
    assign head_did_o = ctl.head_did;
    assign state_o = ctl.state;
    assign enabled_o = E3Enable;

    typedef struct packed { logic r_ready; } delay_req_t;
    typedef struct packed { logic [63:0] data; logic last; } delay_r_t;
    typedef struct packed { delay_r_t r; logic r_valid; } delay_resp_t;
    delay_req_t walker_req, memory_req;
    delay_resp_t walker_resp, memory_resp;
    assign walker_req.r_ready = walker_ready_i;
    assign memory_resp = '{'{64'h123456789abcdef0, 1'b1}, memory_valid_i};
    rv_iommu_e3_delay #(.req_t(delay_req_t), .resp_t(delay_resp_t)) i_delay (
        .clk_i, .rst_ni, .enable_i(delay_enable_i), .arm_i(delay_arm_i),
        .eligible_i(active_i && a_miss_i && ctl.state == 2 && trans_req.did == 11 &&
                    ctl.victim_waiting != 0 && !ctl.queue_error),
        .delay128_i, .walker_req_i(walker_req), .walker_resp_o(walker_resp),
        .memory_req_o(memory_req), .memory_resp_i(memory_resp),
        .capture_o, .available_o, .consume_o, .error_o(delay_error_o), .extra_o
    );
    assign memory_ready_o = memory_req.r_ready;
    assign walker_valid_o = walker_resp.r_valid;
    rv_iommu_e3_queue #(.Depth(8)) i_leaf (
        .clk_i, .rst_ni, .ar_fire_i(leaf_push_i), .ar_id_i(8'd0), .ar_did_i(24'd10),
        .select_i(leaf_select_i), .select_write_i(1'b0), .select_id_i(8'd0),
        .select_did_i(24'd10), .waiting_o(leaf_waiting_o), .error_o(leaf_error_o)
    );
endmodule
