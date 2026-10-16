module e3_observer_top (
 input logic clk_i,rst_ni,arm_i,clear_i,active_i,open_i,close_i,abort_i,delay128_i,
 input logic select_i,write_i,request_i,response_i,lookup_i,hit_i,capture_i,available_i,consume_i,
 input logic [3:0] id_i,
 input logic [7:0] extra_i,index_i,
 input logic [1:0] tr_fire_i,out_valid_i,out_ready_i,return_i,
 input logic [3:0] tr_id_i[2],out_id_i[2],return_id_i[2],
 input logic wlast_i,head_a_i,ptw_r_i,a_miss_i,queue_bad_i,arm_dirty_i,
 output logic [63:0] data_o,
 output logic error_o
);
 rv_iommu::e3_ctl_t ctl;
 rv_iommu::e3_tl_t tl;
 logic [31:0] waiting; logic queue_error;
 rv_iommu_e3_queue #(.Depth(4)) i_queue(.clk_i,.rst_ni,
  .ar_fire_i(tr_fire_i[0]),.ar_id_i(8'(tr_id_i[0])),.ar_did_i(tr_id_i[0]==0?24'd10:24'd11),
  .select_i,.select_write_i(write_i),.select_id_i(8'(id_i)),.select_did_i(id_i==0?24'd10:24'd11),
  .waiting_o(waiting),.error_o(queue_error));
 always_comb begin
  ctl='0;tl='0;
  ctl.select_valid=select_i;ctl.select_write=write_i;ctl.select_id=8'(id_i);
  ctl.select_did=id_i==0?10:11;ctl.select_iova=64'h1000;
  ctl.head_valid=1;ctl.head_did=head_a_i?11:10;ctl.head_id=head_a_i?4:0;ctl.head_iova=64'h1000;ctl.state=2;
  ctl.victim_waiting=waiting+(arm_dirty_i?1:0);ctl.queue_error=queue_error||queue_bad_i;
  ctl.request_fire=request_i;ctl.response_fire=response_i;
  tl.lookup=lookup_i;tl.hit=hit_i;tl.normal=1;tl.did=id_i==0?10:11;
  tl.ptw_r_fire=capture_i||ptw_r_i;tl.a_s1_miss=capture_i||a_miss_i;
  tl.capture=capture_i;tl.available=available_i;tl.consume=consume_i;tl.extra=extra_i;
  tl.level=1;tl.pte_address=64'h80001000;
 end
 dma_e3_observer dut(.clk_i,.rst_ni,.arm_i,.clear_i,.active_i,.open_i,.close_i,.abort_i,.delay128_i,
  .ctl_i(ctl),.tl_i(tl),.tr_fire_i,.out_valid_i,.out_ready_i,.return_i,.tr_id_i,.out_id_i,.return_id_i,
  .tr_address_i('{64'h1000,64'h1000}),.wlast_i,.index_i,.data_o,.error_o);
endmodule
