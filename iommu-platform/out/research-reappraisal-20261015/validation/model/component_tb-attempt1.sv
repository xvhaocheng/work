module research_ar_component_tb;
  typedef struct packed {
    logic [5:0] id;
    logic [63:0] addr;
    logic [7:0] len;
    logic [3:0] qos;
  } ar_t;
  logic clk=0, rst_n=0;
  logic [3:0] req, grant;
  ar_t [3:0] data_i;
  ar_t data_o;
  logic ready, valid;
  logic [1:0] index_o;
  integer mode, bad_held;
  research_ar_selector #(.DataType(ar_t)) dut (
    .clk_i(clk),.rst_ni(rst_n),.req_i(req),.gnt_o(grant),.data_i(data_i),
    .gnt_i(ready),.req_o(valid),.data_o(data_o),.idx_o(index_o)
  );
  task automatic cycle;
    #4; clk=1; #1; clk=0; #4;
  endtask
  task automatic expect_source(input int source);
    #1;
    if (!valid || index_o != source || data_o != data_i[source])
      $fatal(1,"component expected source %0d, got %0d",source,index_o);
    cycle();
  endtask
  initial begin
    if (!$value$plusargs("ar-ds-first=%d",mode)) mode=0;
    if (!$value$plusargs("bad-held=%d",bad_held)) bad_held=0;
    req=0;ready=0;
    for (int i=0;i<4;i++) begin
      data_i[i]='0;
      data_i[i].id=6'(i*16);
      data_i[i].addr=64'(4096*(i+1));
    end
    cycle();cycle();rst_n=1;
    if (mode==1 && !bad_held) begin
      req=4'b1111;ready=1;
      repeat (4) expect_source(3);
      expect_source(0); // Four competitive accepts buy one non-DS escape.
      req=4'b0111;
      expect_source(1);expect_source(2);expect_source(0);
      req=0;ready=0;cycle();
    end
    req=4'b0100;ready=0;
    expect_source(2); // First offer is now held, not accepted.
    req=4'b1100;
    if (bad_held) data_i[2].qos=4'b1000;
    expect_source(2);
    expect_source(2);
    ready=1;expect_source(2);
    req=4'b1000;expect_source(3);
    req=0;ready=1;cycle();
    $display("COMPONENT-PASS mode=%0d",mode);
    $finish;
  end
endmodule
