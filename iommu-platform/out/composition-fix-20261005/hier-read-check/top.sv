module child(input logic clk, input logic rst, output logic done);
logic [3:0] count;
always_ff @(posedge clk) if (rst) count<=0; else count<=count+1'b1;
assign done=&count;
endmodule
module top(input logic clk, input logic rst, output logic [3:0] observed, output logic done);
child u_child(.clk,.rst,.done);
assign observed=u_child.count;
endmodule
