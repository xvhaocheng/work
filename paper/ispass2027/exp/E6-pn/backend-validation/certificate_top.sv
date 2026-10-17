// Separate unit boundary, not a fabricated fullwrap/backend completion trace.
module certificate_top (
 input logic clk_i, rst_ni, job_fire_i, job_done_i, normal_copy_i,
 input logic [63:0] length_i, src_i, dst_i,
 output logic [1:0] permit_o, pending_o, fault_o,
 output logic equal_o
);
 logic [1:0] gold_permit;
 logic [1:0] same;
 for(genvar n=0;n<2;n++)begin:gen_certificate
  localparam int Capacity = n==0 ? 24 : 4096;
  idma_fullbuffer_certificate #(.CapacityBytes(Capacity)) candidate (
    .clk_i,.rst_ni,.job_fire_i,.job_done_i,.normal_copy_i,.length_i,.src_i,.dst_i,
    .permit_o(permit_o[n]),.pending_o(pending_o[n]),.fault_o(fault_o[n])
  );
  idma_fullbuffer_certificate_gold #(.CapacityBytes(Capacity)) reference_dut (
    .clk_i,.rst_ni,.job_fire_i,.job_done_i,.normal_copy_i,.length_i,.src_i,.dst_i,
    .permit_o(gold_permit[n])
  );
  assign same[n] = permit_o[n]==gold_permit[n] &&
    pending_o[n]==reference_dut.pending_q && fault_o[n]==reference_dut.fault_q;
 end
 assign equal_o=&same;
endmodule
