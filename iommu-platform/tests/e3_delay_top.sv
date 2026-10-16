module e3_delay_top (
    input logic clk_i,rst_ni,enable_i,arm_i,eligible_i,delay128_i,
    input logic in_valid_i,out_ready_i,in_last_i,
    input logic [63:0] in_data_i,
    input logic [3:0] in_id_i,
    output logic in_ready_o,out_valid_o,out_last_o,
    output logic [63:0] out_data_o,
    output logic [3:0] out_id_o,
    output logic capture_o,available_o,consume_o,error_o,
    output logic [7:0] extra_o
);
    typedef struct packed {logic [63:0] data;logic[3:0]id;logic last;} r_t;
    typedef struct packed {logic r_ready;} req_t;
    typedef struct packed {r_t r;logic r_valid;} resp_t;
    req_t req,mem_req;resp_t resp,mem_resp;
    assign req.r_ready=out_ready_i;
    assign mem_resp='{r:'{data:in_data_i,id:in_id_i,last:in_last_i},r_valid:in_valid_i};
    assign in_ready_o=mem_req.r_ready;
    assign out_valid_o=resp.r_valid;
    assign out_data_o=resp.r.data;
    assign out_id_o=resp.r.id;
    assign out_last_o=resp.r.last;
    rv_iommu_e3_delay #(.req_t(req_t),.resp_t(resp_t)) dut (
        .clk_i,.rst_ni,.enable_i,.arm_i,.eligible_i,.delay128_i,
        .walker_req_i(req),.walker_resp_o(resp),.memory_req_o(mem_req),.memory_resp_i(mem_resp),
        .capture_o,.available_o,.consume_o,.error_o,.extra_o
    );
endmodule
