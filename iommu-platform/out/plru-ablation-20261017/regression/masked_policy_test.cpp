// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Independent recency oracle: pointer-based recursive trees (V0/V1), an MRU
// linked-order list (V2). Test only; no hierarchical force or private state read.
#include "Vcomposition_iotlb_top.h"
#include "verilated.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
static Vcomposition_iotlb_top* tp;
#define t (*tp)
static int variant, checks;
static unsigned seed=0xc0ffee33;
static unsigned rnd(){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static void need(bool ok,const char* why){checks++;if(!ok){std::fprintf(stderr,"FAIL variant=%d check=%d %s\n",variant,checks,why);std::exit(1);}}
struct Node {
 int lo, hi; bool right=false; std::unique_ptr<Node> l,r;
 Node(int a,int b):lo(a),hi(b){if(b-a>1){int m=(a+b)/2;l.reset(new Node(a,m));r.reset(new Node(m,b));}}
 int victim()const{if(!l)return lo;return right?r->victim():l->victim();}
 void touch(int x){if(!l)return;if(x<l->hi){right=true;l->touch(x);}else{right=false;r->touch(x);}}
 void reset(){right=false;if(l){l->reset();r->reset();}}
};
struct Entry{bool valid=false;unsigned vpn=0,pscid=0;};
static std::array<Entry,16> e;
static std::vector<int> order;
static Node prot(0,2), compact(2,16);
static unsigned tree=0;
static int v0victim(unsigned mask){int n=0,lo=0,hi=16;while(hi-lo>1){int mid=(lo+hi)/2;unsigned lm=mask & (((1u<<mid)-1)^((1u<<lo)-1));unsigned rm=mask & (((1u<<hi)-1)^((1u<<mid)-1));bool right=rm&&(!lm||((tree>>n)&1));if(right){n=2*n+2;lo=mid;}else{n=2*n+1;hi=mid;}}return lo;}
static void v0touch(int way){int n=0,lo=0,hi=16;unsigned own=(way<2)?3:0xfffc;while(hi-lo>1){int mid=(lo+hi)/2;unsigned lm=own & (((1u<<mid)-1)^((1u<<lo)-1));unsigned rm=own & (((1u<<hi)-1)^((1u<<mid)-1));bool right=way>=mid;if(lm&&rm){tree=(tree&~(1u<<n))|((!right)?(1u<<n):0);}if(right){n=2*n+2;lo=mid;}else{n=2*n+1;hi=mid;}}}
static int expected_victim(unsigned mask){if(!mask)return -1;if(variant==2){for(int i=0;i<16;i++)if((mask&(1u<<i))&&!e[i].valid)return i;for(int i:order)if(mask&(1u<<i))return i;}
 if(variant==1)return mask==3?prot.victim():compact.victim();return v0victim(mask);}
static void touch(int i){if(variant==2){order.erase(std::find(order.begin(),order.end(),i));order.push_back(i);}else if(variant==1){if(i<2)prot.touch(i);else compact.touch(i);}else v0touch(i);}
static void clear_model(){for(auto& x:e)x.valid=false;tree=0;prot.reset();compact.reset();order.resize(16);std::iota(order.begin(),order.end(),0);}
static void idle(){t.lookup_i=0;t.touch_i=0;t.update_update_i=0;t.policy_clear_i=0;t.inval_vma_i=0;t.inval_gvma_i=0;t.en_1S_i=1;t.en_2S_i=0;}
static void snap(){for(int i=0;i<16;i++){t.tap_idx=i;t.eval();uint64_t lo=t.a_q_lo;need(bool((lo>>51)&1)==e[i].valid,"valid bitmap mismatch");if(e[i].valid){need((lo&0x1fffffff)==e[i].vpn,"resident VPN mismatch");need(((lo>>29)&0xfffff)==e[i].pscid,"resident PSCID mismatch");}}}
// Every edge checks pre-state lookup, pre-state victim and all post-state tags.
static void edge(unsigned mask,bool fill,unsigned vpn,int look,bool do_touch,bool clear=false,bool inv=false){idle();t.policy_mask_i=mask;t.policy_clear_i=clear;t.inval_vma_i=inv;t.inval_gv_i=0;t.inval_av_i=0;t.inval_pscv_i=0;t.update_update_i=fill;t.update_vpn_i=vpn;t.update_pscid_i=11;t.update_gscid_i=0;
 t.up_c1s_ppn_i=0x40000+vpn;t.up_c1s_v_i=1;t.up_c1s_r_i=1;t.up_c1s_w_i=1;t.up_c1s_g_i=0;
 if(look>=0){t.lookup_i=1;t.touch_i=do_touch;t.lu_vpn_i=e[look].valid?e[look].vpn:0x1f00000+look;t.lu_pscid_i=e[look].pscid;t.lu_gscid_i=0;}
 t.clk_i=0;t.eval();int v=expected_victim(mask);unsigned m=v<0?0:1u<<v;need(t.a_replace_mask==m,"pre-edge victim mismatch");need(bool(t.a_lu_hit_o)==(look>=0&&e[look].valid),"lookup result mismatch");
 bool touched=look>=0&&e[look].valid&&do_touch;
 if(touched)touch(look);
 if(clear){clear_model();}else if(inv){for(auto& x:e)x.valid=false;}else if(fill&&v>=0){e[v]={true,vpn,11};if(variant==2)touch(v);}
 t.clk_i=1;t.eval();t.clk_i=0;t.eval();snap();}
int main(int argc,char**argv){Verilated::commandArgs(argc,argv);variant=argc>1?std::atoi(argv[1]):0;tp=new Vcomposition_iotlb_top;t.rst_ni=0;t.policy_mask_i=0xfffc;idle();clear_model();for(int i=0;i<3;i++){t.clk_i=0;t.eval();t.clk_i=1;t.eval();}t.clk_i=0;t.rst_ni=1;t.eval();snap();unsigned vpn=0x10000;
 // Cold fills, policy clear, both exact masks; touches belong to resident way,
 // including touching one partition while the other mask is selected.
 for(int i=0;i<70;i++){unsigned mask=(i%7==0)?3:0xfffc;edge(mask,true,vpn++,-1,false);int recent=-1;for(int k=0;k<16;k++)if(e[k].valid&&e[k].vpn==vpn-1)recent=k;edge(mask,false,0,recent,true);}
 edge(0xfffc,true,vpn++,2,true,true); // clear wins over simultaneous hit + fill
 // Deterministic randomized legal unique-tag fills and explicit touches.
 for(int i=0;i<12000;i++){unsigned r=rnd(),mask=(r&1)?3:0xfffc;bool fill=(r&6)!=0;int look=(r&8)?int((r>>8)&15):-1;bool clear=(i%431)==0;bool inv=!clear&&(i%307)==0;edge(mask,fill,vpn++,look,bool(r&16),clear,inv);}
 // Empty mask cannot allocate; V1's compact domain excludes empty mask, so
 // this assertion is exercised on original and exact-LRU variants only.
 if(variant!=1)edge(0,true,vpn++,-1,false);
 std::printf("PASS masked-policy V%d: %d checks; fixed masks, cold/full occupancy, owner-independent touch, disabled touch, clear, invalidation, same-edge hit/fill\n",variant,checks);return 0;}
