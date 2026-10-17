// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vvendor_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
static Vvendor_top t;static FILE*trace_file;static unsigned cyc=0,evs=0,checks=0,cases=0,aw=0,ar=0,w=0,br=0,rr=0;static const char*phase="reset";
static void ck(bool x,const char*s){checks++;if(!x){fprintf(stderr,"FAIL c=%u phase=%s kind=%u %s\n",cyc,phase,t.kind_i,s);exit(1);}}
static void ev(){t.eval();evs++;ck(t.equal_on_o&&t.equal_off_o,"complete struct old vendor gold differs");ck(!t.off_idle_o,"defaultoff not0");if(t.kind_i>=2)ck(!t.pn_idle_o,"unsupported inventory falsely idle");if(t.pn_idle_o)ck(t.aw_empty_o&&t.ar_empty_o&&!t.w_open_o&&!t.aw_lock_o&&!t.ar_lock_o&&!t.aw_chan_o&&!t.aw_sel_o&&!t.ar_chan_o&&!t.ar_sel_o&&!t.w_cut_o&&!t.b_cut_o&&!t.r_cut_o,"idle with existing inventory/lock/spill");
 fprintf(trace_file,"%u,%u,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,",cyc,evs,phase,t.clk_i,t.rst_ni,t.kind_i,t.equal_on_o,t.equal_off_o,t.pn_idle_o,t.aw_empty_o,t.ar_empty_o,t.w_open_o,t.aw_lock_o,t.ar_lock_o,t.aw_chan_o,t.aw_sel_o,t.ar_chan_o,t.ar_sel_o,t.w_cut_o,t.b_cut_o,t.r_cut_o,t.aw_valid_i,t.aw_ready_o,t.ar_valid_i,t.ar_ready_o,t.w_valid_i,t.w_ready_o,t.w_last_i,t.b_valid_o,t.b_ready_i,t.r_valid_o,t.r_ready_i,t.r_last_o,t.mst_aw_valid_o,t.mst_aw_ready_i,t.mst_ar_valid_o,t.mst_ar_ready_i,t.mst_b_valid_i,t.mst_r_valid_i);fprintf(trace_file,"%u,%u,%u\n",t.ar_push_o,t.inject_o,t.ar_pop_o);}
static void tick(){t.clk_i=0;ev();if(t.rst_ni){aw+=bool(t.mst_aw_valid_o&t.mst_aw_ready_i);ar+=bool(t.mst_ar_valid_o&t.mst_ar_ready_i);w+=bool(t.mst_w_valid_o&t.mst_w_ready_i);br+=t.b_valid_o&&t.b_ready_i;rr+=t.r_valid_o&&t.r_ready_i;}t.clk_i=1;ev();t.clk_i=0;ev();cyc++;}
static void idle(unsigned n=4){while(n--)tick();}
static void mark(const char*s){phase=s;cases++;}
static void reset(unsigned k){t.kind_i=k;t.aw_valid_i=t.ar_valid_i=t.w_valid_i=0;t.b_ready_i=t.r_ready_i=1;t.w_last_i=1;t.aw_id_i=t.ar_id_i=t.b_id_i=t.r_id_i=0;t.aw_addr_i=0x1000;t.ar_addr_i=0x2000;t.w_data_i=0x1234;t.r_data_i=0x5678;t.aw_len_i=t.ar_len_i=0;t.atop_i=0;t.aw_select_i=t.ar_select_i=0;t.r_last_i=1;t.mst_aw_ready_i=t.mst_ar_ready_i=t.mst_w_ready_i=7;t.mst_b_valid_i=t.mst_r_valid_i=0;t.rst_ni=0;idle(2);t.rst_ni=1;idle(3);if(k<2)ck(t.pn_idle_o,"empty supported notidle");}
static void aw_offer(unsigned p,unsigned id=1,unsigned len=0,unsigned atop=0){t.aw_select_i=p;t.aw_id_i=id;t.aw_len_i=len;t.atop_i=atop;t.aw_valid_i=1;for(unsigned n=0;;n++){ev();ck(n<30,"AW timeout");bool f=t.aw_ready_o;tick();if(f)break;}t.aw_valid_i=0;idle(3);}
static void ar_offer(unsigned p,unsigned id=2,unsigned len=0){t.ar_select_i=p;t.ar_id_i=id;t.ar_len_i=len;t.ar_valid_i=1;for(unsigned n=0;;n++){ev();ck(n<30,"AR timeout");bool f=t.ar_ready_o;tick();if(f)break;}t.ar_valid_i=0;idle(3);}
static void w_offer(bool last){t.w_valid_i=1;t.w_last_i=last;for(unsigned n=0;;n++){ev();ck(n<30,"W timeout");bool f=t.w_ready_o;tick();if(f)break;}t.w_valid_i=0;idle(3);}
static void b_send(unsigned p,unsigned id=1){t.b_id_i=id;t.mst_b_valid_i=1u<<p;for(unsigned n=0;;n++){ev();ck(n<30,"B timeout");bool f=t.mst_b_ready_o&(1u<<p);tick();if(f)break;}t.mst_b_valid_i=0;idle(3);}
static void r_send(unsigned p,unsigned id=2,bool last=true){t.r_id_i=id;t.r_last_i=last;t.mst_r_valid_i=1u<<p;for(unsigned n=0;;n++){ev();ck(n<30,"R timeout");bool f=t.mst_r_ready_o&(1u<<p);tick();if(f)break;}t.mst_r_valid_i=0;idle(3);}
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);ck(argc==2,"trace arg");trace_file=fopen(argv[1],"w");ck(trace_file,"trace open");fprintf(trace_file,"cycle,eval,phase,clk,rst,kind,equal_on,equal_off,idle,awempty,arempty,wopen,awlock,arlock,awchan,awsel,archan,arsel,wcut,bcut,rcut,awvalid,awready,arvalid,arready,wvalid,wready,wlast,bvalid,bready,rvalid,rready,rlast,mawvalid,mawready,marvalid,marready,mbvalid,mrvalid,ar_push,atop_inject,ar_pop\n");
 for(unsigned k=0;k<4;k++){
  mark("read-debt-noVALID-and-held-Rspill");reset(k);unsigned p=k==2?0:1;ar_offer(p,2,1);idle(20);if(k<2)ck(!t.pn_idle_o&&!t.ar_empty_o,"accepted read empty gap falselyidle");r_send(p,2,false);if(k<2)ck(!t.pn_idle_o,"nonlast retired read");t.r_ready_i=0;
  if(k==0){t.r_id_i=2;t.r_last_i=1;t.mst_r_valid_i=1u<<p;idle(3);ck(!t.pn_idle_o,"held bypassR idle");t.r_ready_i=1;tick();t.mst_r_valid_i=0;idle();}
  else{r_send(p,2,true);if(k<2)ck(t.ar_empty_o&&t.r_cut_o&&!t.pn_idle_o,"Rspill after internal retired notguarded");t.r_ready_i=1;idle();}
  if(k<2)ck(t.pn_idle_o,"read didnot drain");
  mark("write-WLAST-and-held-Bspill");aw_offer(p,1,1);w_offer(false);if(k<2)ck(!t.pn_idle_o&&t.w_open_o==1,"nonlast lostWopen");w_offer(true);idle(20);if(k<2)ck(!t.pn_idle_o&&!t.aw_empty_o&&t.w_open_o==0,"WLAST lost owedB");t.b_ready_i=0;
  if(k==0){t.b_id_i=1;t.mst_b_valid_i=1u<<p;idle(3);ck(!t.pn_idle_o,"held bypassB idle");t.b_ready_i=1;tick();t.mst_b_valid_i=0;idle();}
  else{b_send(p);if(k<2)ck(t.aw_empty_o&&t.b_cut_o&&!t.pn_idle_o,"Bspill after internal retired notguarded");t.b_ready_i=1;idle();}
  if(k<2)ck(t.pn_idle_o,"write didnot drain");
  mark("ATOP-real-existing-read-injection");aw_offer(p,3,0,0x20);w_offer(true);if(k<2)ck(!t.ar_empty_o&&!t.aw_empty_o,"ATOP no real dual debt");r_send(p,3,true);if(k<2)ck(!t.pn_idle_o&&!t.aw_empty_o,"ATOP R erasedB debt");b_send(p,3);if(k<2)ck(t.pn_idle_o,"ATOP didnotdrain");
 }
 for(unsigned k=0;k<2;k++){
  mark("AW-lock-data-select-and-W-before-AW");reset(k);t.mst_aw_ready_i=0;t.aw_select_i=0;t.aw_id_i=1;t.aw_valid_i=1;t.w_valid_i=1;t.w_last_i=1;bool af=false,wf=false;
  for(unsigned n=0;n<8;n++){ev();bool a=t.aw_valid_i&&t.aw_ready_o,b=t.w_valid_i&&t.w_ready_o;tick();if(a){t.aw_valid_i=0;af=true;}if(b){t.w_valid_i=0;wf=true;}}
  ck(t.aw_lock_o&&!t.pn_idle_o,"actual lockedAW notguarded");t.mst_aw_ready_i=7;
  for(unsigned n=0;!af||!wf;n++){ck(n<30,"lockedAW/W didnotresume");ev();bool a=t.aw_valid_i&&t.aw_ready_o,b=t.w_valid_i&&t.w_ready_o;tick();if(a){t.aw_valid_i=0;af=true;}if(b){t.w_valid_i=0;wf=true;}}
  idle();b_send(0);ck(t.pn_idle_o,"lockedAW/W didnotdrain");
  mark("AR-lock-data-select-and-two-ID-inventory");t.mst_ar_ready_i=0;t.ar_select_i=0;t.ar_id_i=4;t.ar_valid_i=1;bool f=false;
  for(unsigned n=0;n<8;n++){ev();bool a=t.ar_valid_i&&t.ar_ready_o;tick();if(a){t.ar_valid_i=0;f=true;}}
  ck(t.ar_lock_o&&!t.pn_idle_o,"actual lockedAR notguarded");t.mst_ar_ready_i=7;
  while(!f){ev();bool a=t.ar_ready_o;tick();if(a){t.ar_valid_i=0;f=true;}}idle();ar_offer(1,5);r_send(0,4);ck(!t.ar_empty_o&&!t.pn_idle_o,"one ID retired allinventory");r_send(1,5);ck(t.pn_idle_o,"twoID didnotdrain");
  mark("same-edge-real-ARpush-ATOPinject-Rpop");reset(k);ar_offer(0,7);
  t.aw_select_i=t.ar_select_i=0;t.aw_id_i=t.ar_id_i=7;t.aw_len_i=t.ar_len_i=0;t.atop_i=0x20;t.aw_valid_i=t.ar_valid_i=1;
  if(k==1){tick();t.aw_valid_i=t.ar_valid_i=0;}
  t.mst_r_valid_i=1;t.r_id_i=7;t.r_last_i=1;ev();ck(t.ar_push_o&&t.inject_o&&t.ar_pop_o,"did not exercise true 3-way ledger event");tick();t.aw_valid_i=t.ar_valid_i=0;t.mst_r_valid_i=0;idle(3);w_offer(true);
  r_send(0,7);ck(!t.ar_empty_o&&!t.pn_idle_o,"3-wayevent lost second owedR");r_send(0,7);b_send(0,7);ck(t.pn_idle_o,"3-wayevent didnot drain");
 }
 fprintf(stdout,"PASS vendor cases=%u cycles=%u evals=%u checks=%u AW=%u AR=%u W=%u B=%u R=%u; old-byte gold, no board\n",cases,cyc,evs,checks,aw,ar,w,br,rr);fclose(trace_file);
}
