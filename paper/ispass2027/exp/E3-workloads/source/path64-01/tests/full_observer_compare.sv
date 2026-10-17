module full_observer_compare #(parameter bit HistEnable=1) (
    input logic clk_i, rst_ni,
    input logic cfg_valid_i, cfg_write_i,
    input logic [6:0] cfg_addr_i,
    input logic [63:0] cfg_wdata_i,
    input logic [7:0] cfg_wstrb_i,
    output logic [63:0] cfg_rdata_o,
    input logic [NumPorts-1:0] issue_i, done_i, poll_i,
    input logic [NumPorts-1:0] batch_active_i,
    input logic [63:0] issue_id_i [NumPorts],
    input logic [63:0] done_id_i [NumPorts],
    input logic [63:0] poll_id_i [NumPorts],
    input logic [NumPorts-1:0] ar_valid_i, aw_valid_i, ar_ready_i, aw_ready_i,
    input logic [NumPorts-1:0] rlast_fire_i, b_fire_i, bad_id_i,
    input logic shared_ar_valid_i, shared_aw_valid_i,
    input logic shared_ar_ready_i, shared_aw_ready_i,
    input logic [AxiIdWidth-1:0] tr_id_i [2], out_id_i [2], return_id_i [2],
    input logic [1:0] out_valid_i, out_ready_i, return_i
, output logic equal_o);
logic [63:0] reference_data;
dma_observer #(.PathEnable(1),.HistEnable(HistEnable)) candidate(.*);
dma_observer_gold #(.PathEnable(1),.HistEnable(HistEnable)) reference_dut(.cfg_rdata_o(reference_data),.*);
logic [34:0] same_count;
logic [127:0] same_path;
logic [511:0] same_hist;
for(genvar i=0;i<35;i++) assign same_count[i]=candidate.count_q[i]==reference_dut.count_q[i];
for(genvar l=0;l<4;l++) for(genvar s=0;s<4;s++) begin
  for(genvar f=0;f<8;f++) assign same_path[l*32+s*8+f]=candidate.gen_path.i_path.result[l][s][f]==reference_dut.gen_path.i_path.result[l][s][f];
  for(genvar b=0;b<32;b++) assign same_hist[l*128+s*32+b]=candidate.gen_path.i_path.histogram[l][s][b]==reference_dut.gen_path.i_path.histogram[l][s][b];
end
assign equal_o=(cfg_rdata_o==reference_data)&&(&same_count)&&(&same_path)&&(&same_hist)&&
  candidate.hist_overflow==reference_dut.hist_overflow;
endmodule
