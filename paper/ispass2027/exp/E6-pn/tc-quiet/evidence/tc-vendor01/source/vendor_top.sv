// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
`include "axi/typedef.svh"
module vendor_top (
 input logic clk_i,rst_ni,
 input logic [1:0] kind_i,aw_select_i,ar_select_i,
 input logic aw_valid_i,ar_valid_i,w_valid_i,b_ready_i,r_ready_i,w_last_i,
 input logic [3:0] aw_id_i,ar_id_i,b_id_i,r_id_i,
 input logic [63:0] aw_addr_i,ar_addr_i,w_data_i,r_data_i,
 input logic [7:0] aw_len_i,ar_len_i,
 input logic [5:0] atop_i,
 input logic r_last_i,
 input logic [2:0] mst_aw_ready_i,mst_ar_ready_i,mst_w_ready_i,mst_b_valid_i,mst_r_valid_i,
 output logic aw_ready_o,ar_ready_o,w_ready_o,b_valid_o,r_valid_o,
 output logic [2:0] mst_aw_valid_o,mst_ar_valid_o,mst_w_valid_o,mst_b_ready_o,mst_r_ready_o,
 output logic [3:0] b_id_o,r_id_o,
 output logic r_last_o,
 output logic equal_on_o,equal_off_o,pn_idle_o,off_idle_o,
 output logic aw_empty_o,ar_empty_o,aw_lock_o,ar_lock_o,
 output logic [7:0] w_open_o,
 output logic aw_chan_o,aw_sel_o,ar_chan_o,ar_sel_o,w_cut_o,b_cut_o,r_cut_o
);
 typedef logic [63:0] addr_t;
 `AXI_TYPEDEF_ALL(axi,addr_t,logic[3:0],logic[63:0],logic[7:0],logic[0:0])
 axi_req_t req;
 always_comb begin
  req='0;req.aw_valid=aw_valid_i;req.aw.id=aw_id_i;req.aw.addr=aw_addr_i;req.aw.len=aw_len_i;req.aw.atop=atop_i;req.aw.size=3;req.aw.burst=1;
  req.ar_valid=ar_valid_i;req.ar.id=ar_id_i;req.ar.addr=ar_addr_i;req.ar.len=ar_len_i;req.ar.size=3;req.ar.burst=1;
  req.w_valid=w_valid_i;req.w.data=w_data_i;req.w.strb='1;req.w.last=w_last_i;req.b_ready=b_ready_i;req.r_ready=r_ready_i;
 end
 axi_resp_t [3:0] selected;
 logic[3:0] eq_on,eq_off,idle_on,idle_off,ae,re,al,rl,ac,as_,rc,rs,wc,bc,rrc;
 logic[3:0][7:0] wo;
 logic[3:0][2:0] av,rv,wv,br,rr;
 for(genvar k=0;k<4;k++)begin:configurations
  localparam int N=(k==1)?3:((k==2)?1:2);
  localparam bit Spill=(k!=0),U=(k==3);
  axi_req_t stimulus;
  axi_resp_t [N-1:0] mrsp;
  axi_req_t [N-1:0] mreq[3];
  axi_resp_t srsp[3];
  assign stimulus=(kind_i==k)?req:'0;
  for(genvar p=0;p<N;p++)begin:ports_
   always_comb begin
    mrsp[p]='0;mrsp[p].aw_ready=mst_aw_ready_i[p];mrsp[p].ar_ready=mst_ar_ready_i[p];mrsp[p].w_ready=mst_w_ready_i[p];
    mrsp[p].b_valid=(kind_i==k)&&mst_b_valid_i[p];mrsp[p].b.id=b_id_i;
    mrsp[p].r_valid=(kind_i==k)&&mst_r_valid_i[p];mrsp[p].r.id=r_id_i;mrsp[p].r.data=r_data_i;mrsp[p].r.last=r_last_i;
   end
  end
  `define PARAMS .AxiIdWidth(4),.AtopSupport(1),.aw_chan_t(axi_aw_chan_t),.w_chan_t(axi_w_chan_t),.b_chan_t(axi_b_chan_t),.ar_chan_t(axi_ar_chan_t),.r_chan_t(axi_r_chan_t),.axi_req_t(axi_req_t),.axi_resp_t(axi_resp_t),.NoMstPorts(N),.MaxTrans(8),.AxiLookBits(4),.UniqueIds(U),.SpillAw(Spill),.SpillAr(Spill),.SpillW(Spill),.SpillB(Spill),.SpillR(Spill)
  `define PORTS .clk_i,.rst_ni,.test_i(1'b0),.slv_req_i(stimulus),.slv_aw_select_i(((k==2)?1:((k==1)?2:1))'(aw_select_i)),.slv_ar_select_i(((k==2)?1:((k==1)?2:1))'(ar_select_i))
  axi_demux #(`PARAMS,.PnEnable(1)) dut_on (`PORTS,.slv_resp_o(srsp[0]),.mst_reqs_o(mreq[0]),.mst_resps_i(mrsp),.pn_idle_o(idle_on[k]));
  axi_demux #(`PARAMS) dut_off (`PORTS,.slv_resp_o(srsp[1]),.mst_reqs_o(mreq[1]),.mst_resps_i(mrsp),.pn_idle_o(idle_off[k]));
  axi_demux_gold #(`PARAMS) gold (`PORTS,.slv_resp_o(srsp[2]),.mst_reqs_o(mreq[2]),.mst_resps_i(mrsp));
  assign selected[k]=srsp[0];assign eq_on[k]=(srsp[0]===srsp[2])&&(mreq[0]===mreq[2]);assign eq_off[k]=(srsp[1]===srsp[2])&&(mreq[1]===mreq[2]);
  always_comb begin
   av[k]='0;rv[k]='0;wv[k]='0;br[k]='0;rr[k]='0;
   for(int p=0;p<N;p++)begin
    av[k][p]=mreq[0][p].aw_valid;rv[k][p]=mreq[0][p].ar_valid;wv[k][p]=mreq[0][p].w_valid;br[k][p]=mreq[0][p].b_ready;rr[k][p]=mreq[0][p].r_ready;
   end
  end
  if(k<2)begin:taps
   assign ae[k]=dut_on.i_demux_simple.genblk1.pn_aw_empty;
   assign re[k]=dut_on.i_demux_simple.genblk1.pn_ar_empty;
   assign al[k]=dut_on.i_demux_simple.genblk1.lock_aw_valid_q;
   assign rl[k]=dut_on.i_demux_simple.genblk1.lock_ar_valid_q;
   assign wo[k]=8'(dut_on.i_demux_simple.genblk1.w_open);
  end else begin:no_taps
   assign ae[k]=0,re[k]=0,al[k]=0,rl[k]=0,wo[k]=0;
  end
  assign ac[k]=dut_on.slv_aw_valid_chan,as_[k]=dut_on.slv_aw_valid_sel,rc[k]=dut_on.slv_ar_valid_chan,rs[k]=dut_on.slv_ar_valid_sel;
  assign wc[k]=dut_on.slv_req_cut.w_valid,bc[k]=dut_on.slv_resp_o.b_valid,rrc[k]=dut_on.slv_resp_o.r_valid;
  `undef PARAMS
  `undef PORTS
 end
 assign aw_ready_o=selected[kind_i].aw_ready,ar_ready_o=selected[kind_i].ar_ready,w_ready_o=selected[kind_i].w_ready,b_valid_o=selected[kind_i].b_valid,r_valid_o=selected[kind_i].r_valid;
 assign b_id_o=selected[kind_i].b.id,r_id_o=selected[kind_i].r.id,r_last_o=selected[kind_i].r.last;
 assign equal_on_o=&eq_on,equal_off_o=&eq_off,pn_idle_o=idle_on[kind_i],off_idle_o=idle_off[kind_i];
 assign mst_aw_valid_o=av[kind_i],mst_ar_valid_o=rv[kind_i],mst_w_valid_o=wv[kind_i],mst_b_ready_o=br[kind_i],mst_r_ready_o=rr[kind_i];
 assign aw_empty_o=ae[kind_i],ar_empty_o=re[kind_i],aw_lock_o=al[kind_i],ar_lock_o=rl[kind_i],w_open_o=wo[kind_i];
 assign aw_chan_o=ac[kind_i],aw_sel_o=as_[kind_i],ar_chan_o=rc[kind_i],ar_sel_o=rs[kind_i],w_cut_o=wc[kind_i],b_cut_o=bc[kind_i],r_cut_o=rrc[kind_i];
endmodule
