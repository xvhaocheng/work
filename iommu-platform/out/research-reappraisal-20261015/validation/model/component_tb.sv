// Delay-free component wrapper for the installed Verilator 4.x C++ interface.
module research_ar_component_tb (
  input  logic       clk_i,
  input  logic       rst_ni,
  input  logic [3:0] req,
  input  logic       ready,
  input  logic       tamper_i,
  output logic       valid,
  output logic [3:0] grant,
  output logic [1:0] index_o
);
  typedef struct packed {
    logic [5:0] id;
    logic [63:0] addr;
    logic [7:0] len;
    logic [3:0] qos;
  } ar_t;
  ar_t [3:0] data_i;
  ar_t data_o;

  always_comb begin
    for (int unsigned source = 0; source < 4; source++) begin
      data_i[source] = '0;
      data_i[source].id = 6'(source * 16);
      data_i[source].addr = 64'(4096 * (source + 1));
      data_i[source].len = 8'd0;
    end
    data_i[2].qos = tamper_i ? 4'h8 : 4'h0;
  end

  research_ar_selector #(.DataType(ar_t)) dut (
    .clk_i (clk_i),
    .rst_ni(rst_ni),
    .req_i (req),
    .gnt_o (grant),
    .data_i(data_i),
    .gnt_i (ready),
    .req_o (valid),
    .data_o(data_o),
    .idx_o (index_o)
  );
endmodule
