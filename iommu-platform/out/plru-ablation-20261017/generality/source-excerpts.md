# 只读源码片段

行号对应原始文件和同字节快照，不是本文件行号。

## cva6_sv32

原件：`iommu-platform/cva6/core/mmu_sv32/cva6_tlb_sv32.sv`；SHA-256 `16edf37ca9504399afa1b0f0bb1f503420dd5a93855b0562c31306b8b9b1d33e`。

原件 L18–L57：

```text
18: //              This module is an adaptation of the Sv39 TLB developed
19: //              by Florian Zaruba and David Schaffenrath to the Sv32 standard.
20: //
21: // =========================================================================== //
22: // Revisions  :
23: // Date        Version  Author       Description
24: // 2020-02-17  0.1      S.Jacq       TLB Sv32 for CV32A6
25: // =========================================================================== //
26: 
27: module cva6_tlb_sv32 import ariane_pkg::*; #(
28:       parameter int unsigned TLB_ENTRIES = 4,
29:       parameter int unsigned ASID_WIDTH  = 1
30:   )(
31:     input  logic                    clk_i,    // Clock
32:     input  logic                    rst_ni,   // Asynchronous reset active low
33:     input  logic                    flush_i,  // Flush signal
34:     // Update TLB
35:     input  tlb_update_sv32_t        update_i,
36:     // Lookup signals
37:     input  logic                    lu_access_i,
38:     input  logic [ASID_WIDTH-1:0]   lu_asid_i,
39:     input  logic [riscv::VLEN-1:0]  lu_vaddr_i,
40:     output riscv::pte_sv32_t        lu_content_o,
41:     input  logic [ASID_WIDTH-1:0]   asid_to_be_flushed_i,
42:     input  logic [riscv::VLEN-1:0]  vaddr_to_be_flushed_i,
43:     output logic                    lu_is_4M_o,
44:     output logic                    lu_hit_o
45: );
46: 
47:     // Sv32 defines two levels of page tables
48:     struct packed {
49:       logic [8:0]            asid; //9 bits wide
50:       logic [9:0]            vpn1; //10 bits wide
51:       logic [9:0]            vpn0; //10 bits wide
52:       logic                  is_4M;
53:       logic                  valid;
54:     } [TLB_ENTRIES-1:0] tags_q, tags_n;
55: 
56:     riscv::pte_sv32_t [TLB_ENTRIES-1:0] content_q, content_n;
57:     logic [9:0] vpn0, vpn1;
```

原件 L119–L139：

```text
119: 	          	tags_n[i].valid = 1'b0;
120:                 // the entry is flushed if it's not global, and the asid matches and vaddr is 0. ("SFENCE.VMA 0 asid" case)
121: 	        else if ((!content_q[i].g) && (vaddr_to_be_flushed_is0) && (asid_to_be_flushed_i == tags_q[i].asid) && (!asid_to_be_flushed_is0))
122: 	        	  tags_n[i].valid = 1'b0;
123:             // normal replacement
124:             end else if (update_i.valid & replace_en[i]) begin
125:                 // update tag array
126:                 tags_n[i] = '{
127:                     asid:  update_i.asid,
128:                     vpn1:  update_i.vpn [19:10],
129:                     vpn0:  update_i.vpn [9:0],
130:                     is_4M: update_i.is_4M,
131:                     valid: 1'b1
132:                 };
133:                 // and content as well
134:                 content_n[i] = update_i.content;
135:             end
136:         end
137:     end
138: 
139:     // -----------------------------------------------
```

原件 L141–L216：

```text
141:     // -----------------------------------------------
142:     logic[2*(TLB_ENTRIES-1)-1:0] plru_tree_q, plru_tree_n;
143:     always_comb begin : plru_replacement
144:         plru_tree_n = plru_tree_q;
145:         // The PLRU-tree indexing:
146:         // lvl0        0
147:         //            / \
148:         //           /   \
149:         // lvl1     1     2
150:         //         / \   / \
151:         // lvl2   3   4 5   6
152:         //       / \ /\/\  /\
153:         //      ... ... ... ...
154:         // Just predefine which nodes will be set/cleared
155:         // E.g. for a TLB with 8 entries, the for-loop is semantically
156:         // equivalent to the following pseudo-code:
157:         // unique case (1'b1)
158:         // lu_hit[7]: plru_tree_n[0, 2, 6] = {1, 1, 1};
159:         // lu_hit[6]: plru_tree_n[0, 2, 6] = {1, 1, 0};
160:         // lu_hit[5]: plru_tree_n[0, 2, 5] = {1, 0, 1};
161:         // lu_hit[4]: plru_tree_n[0, 2, 5] = {1, 0, 0};
162:         // lu_hit[3]: plru_tree_n[0, 1, 4] = {0, 1, 1};
163:         // lu_hit[2]: plru_tree_n[0, 1, 4] = {0, 1, 0};
164:         // lu_hit[1]: plru_tree_n[0, 1, 3] = {0, 0, 1};
165:         // lu_hit[0]: plru_tree_n[0, 1, 3] = {0, 0, 0};
166:         // default: begin /* No hit */ end
167:         // endcase
168:         for (int unsigned i = 0; i < TLB_ENTRIES; i++) begin
169:             automatic int unsigned idx_base, shift, new_index;
170:             // we got a hit so update the pointer as it was least recently used
171:             if (lu_hit[i] & lu_access_i) begin
172:                 // Set the nodes to the values we would expect
173:                 for (int unsigned lvl = 0; lvl < $clog2(TLB_ENTRIES); lvl++) begin
174:                   idx_base = $unsigned((2**lvl)-1);
175:                   // lvl0 <=> MSB, lvl1 <=> MSB-1, ...
176:                   shift = $clog2(TLB_ENTRIES) - lvl;
177:                   // to circumvent the 32 bit integer arithmetic assignment
178:                   new_index =  ~((i >> (shift-1)) & 32'b1);
179:                   plru_tree_n[idx_base + (i >> shift)] = new_index[0];
180:                 end
181:             end
182:         end
183:         // Decode tree to write enable signals
184:         // Next for-loop basically creates the following logic for e.g. an 8 entry
185:         // TLB (note: pseudo-code obviously):
186:         // replace_en[7] = &plru_tree_q[ 6, 2, 0]; //plru_tree_q[0,2,6]=={1,1,1}
187:         // replace_en[6] = &plru_tree_q[~6, 2, 0]; //plru_tree_q[0,2,6]=={1,1,0}
188:         // replace_en[5] = &plru_tree_q[ 5,~2, 0]; //plru_tree_q[0,2,5]=={1,0,1}
189:         // replace_en[4] = &plru_tree_q[~5,~2, 0]; //plru_tree_q[0,2,5]=={1,0,0}
190:         // replace_en[3] = &plru_tree_q[ 4, 1,~0]; //plru_tree_q[0,1,4]=={0,1,1}
191:         // replace_en[2] = &plru_tree_q[~4, 1,~0]; //plru_tree_q[0,1,4]=={0,1,0}
192:         // replace_en[1] = &plru_tree_q[ 3,~1,~0]; //plru_tree_q[0,1,3]=={0,0,1}
193:         // replace_en[0] = &plru_tree_q[~3,~1,~0]; //plru_tree_q[0,1,3]=={0,0,0}
194:         // For each entry traverse the tree. If every tree-node matches,
195:         // the corresponding bit of the entry's index, this is
196:         // the next entry to replace.
197:         for (int unsigned i = 0; i < TLB_ENTRIES; i += 1) begin
198:             automatic logic en;
199:             automatic int unsigned idx_base, shift, new_index;
200:             en = 1'b1;
201:             for (int unsigned lvl = 0; lvl < $clog2(TLB_ENTRIES); lvl++) begin
202:                 idx_base = $unsigned((2**lvl)-1);
203:                 // lvl0 <=> MSB, lvl1 <=> MSB-1, ...
204:                 shift = $clog2(TLB_ENTRIES) - lvl;
205: 
206:                 // en &= plru_tree_q[idx_base + (i>>shift)] == ((i >> (shift-1)) & 1'b1);
207:                 new_index =  (i >> (shift-1)) & 32'b1;
208:                 if (new_index[0]) begin
209:                   en &= plru_tree_q[idx_base + (i>>shift)];
210:                 end else begin
211:                   en &= ~plru_tree_q[idx_base + (i>>shift)];
212:                 end
213:             end
214:             replace_en[i] = en;
215:         end
216:     end
```

## cva6_sv39

原件：`iommu-platform/cva6/core/mmu_sv39/tlb.sv`；SHA-256 `d03a3cd0b6f004c20777ca92d55298f61173a067566a4fa336ffa2ddb18164fe`。

原件 L18–L57：

```text
18: module tlb import ariane_pkg::*; #(
19:       parameter int unsigned TLB_ENTRIES = 4,
20:       parameter int unsigned ASID_WIDTH  = 1
21:   )(
22:     input  logic                    clk_i,    // Clock
23:     input  logic                    rst_ni,   // Asynchronous reset active low
24:     input  logic                    flush_i,  // Flush signal
25:     // Update TLB
26:     input  tlb_update_t             update_i,
27:     // Lookup signals
28:     input  logic                    lu_access_i,
29:     input  logic [ASID_WIDTH-1:0]   lu_asid_i,
30:     input  logic [riscv::VLEN-1:0]  lu_vaddr_i,
31:     output riscv::pte_t             lu_content_o,
32:     input  logic [ASID_WIDTH-1:0]   asid_to_be_flushed_i,
33:     input  logic [riscv::VLEN-1:0]  vaddr_to_be_flushed_i,
34:     output logic                    lu_is_2M_o,
35:     output logic                    lu_is_1G_o,
36:     output logic                    lu_hit_o
37: );
38: 
39:     // SV39 defines three levels of page tables
40:     struct packed {
41:       logic [ASID_WIDTH-1:0] asid;
42:       logic [riscv::VPN2:0]  vpn2;
43:       logic [8:0]            vpn1;
44:       logic [8:0]            vpn0;
45:       logic                  is_2M;
46:       logic                  is_1G;
47:       logic                  valid;
48:     } [TLB_ENTRIES-1:0] tags_q, tags_n;
49: 
50:     riscv::pte_t [TLB_ENTRIES-1:0] content_q, content_n;
51:     logic [8:0] vpn0, vpn1;
52:     logic [riscv::VPN2:0] vpn2;
53:     logic [TLB_ENTRIES-1:0] lu_hit;     // to replacement logic
54:     logic [TLB_ENTRIES-1:0] replace_en; // replace the following entry, set by replacement strategy
55:     //-------------
56:     // Translation
57:     //-------------
```

原件 L129–L150：

```text
129: 				          	tags_n[i].valid = 1'b0;
130:                 // the entry is flushed if it's not global, and the asid matches and vaddr is 0. ("SFENCE.VMA 0 asid" case)
131: 				        else if ((!content_q[i].g) && (vaddr_to_be_flushed_is0) && (asid_to_be_flushed_i == tags_q[i].asid) && (!asid_to_be_flushed_is0))
132: 				        	  tags_n[i].valid = 1'b0;
133:             // normal replacement
134:             end else if (update_i.valid & replace_en[i]) begin
135:                 // update tag array
136:                 tags_n[i] = '{
137:                     asid:  update_i.asid,
138:                     vpn2:  update_i.vpn [18+riscv::VPN2:18],
139:                     vpn1:  update_i.vpn [17:9],
140:                     vpn0:  update_i.vpn [8:0],
141:                     is_1G: update_i.is_1G,
142:                     is_2M: update_i.is_2M,
143:                     valid: 1'b1
144:                 };
145:                 // and content as well
146:                 content_n[i] = update_i.content;
147:             end
148:         end
149:     end
150: 
```

原件 L152–L228：

```text
152:     // PLRU - Pseudo Least Recently Used Replacement
153:     // -----------------------------------------------
154:     logic[2*(TLB_ENTRIES-1)-1:0] plru_tree_q, plru_tree_n;
155:     always_comb begin : plru_replacement
156:         plru_tree_n = plru_tree_q;
157:         // The PLRU-tree indexing:
158:         // lvl0        0
159:         //            / \
160:         //           /   \
161:         // lvl1     1     2
162:         //         / \   / \
163:         // lvl2   3   4 5   6
164:         //       / \ /\/\  /\
165:         //      ... ... ... ...
166:         // Just predefine which nodes will be set/cleared
167:         // E.g. for a TLB with 8 entries, the for-loop is semantically
168:         // equivalent to the following pseudo-code:
169:         // unique case (1'b1)
170:         // lu_hit[7]: plru_tree_n[0, 2, 6] = {1, 1, 1};
171:         // lu_hit[6]: plru_tree_n[0, 2, 6] = {1, 1, 0};
172:         // lu_hit[5]: plru_tree_n[0, 2, 5] = {1, 0, 1};
173:         // lu_hit[4]: plru_tree_n[0, 2, 5] = {1, 0, 0};
174:         // lu_hit[3]: plru_tree_n[0, 1, 4] = {0, 1, 1};
175:         // lu_hit[2]: plru_tree_n[0, 1, 4] = {0, 1, 0};
176:         // lu_hit[1]: plru_tree_n[0, 1, 3] = {0, 0, 1};
177:         // lu_hit[0]: plru_tree_n[0, 1, 3] = {0, 0, 0};
178:         // default: begin /* No hit */ end
179:         // endcase
180:         for (int unsigned i = 0; i < TLB_ENTRIES; i++) begin
181:             automatic int unsigned idx_base, shift, new_index;
182:             // we got a hit so update the pointer as it was least recently used
183:             if (lu_hit[i] & lu_access_i) begin
184:                 // Set the nodes to the values we would expect
185:                 for (int unsigned lvl = 0; lvl < $clog2(TLB_ENTRIES); lvl++) begin
186:                   idx_base = $unsigned((2**lvl)-1);
187:                   // lvl0 <=> MSB, lvl1 <=> MSB-1, ...
188:                   shift = $clog2(TLB_ENTRIES) - lvl;
189:                   // to circumvent the 32 bit integer arithmetic assignment
190:                   new_index =  ~((i >> (shift-1)) & 32'b1);
191:                   plru_tree_n[idx_base + (i >> shift)] = new_index[0];
192:                 end
193:             end
194:         end
195:         // Decode tree to write enable signals
196:         // Next for-loop basically creates the following logic for e.g. an 8 entry
197:         // TLB (note: pseudo-code obviously):
198:         // replace_en[7] = &plru_tree_q[ 6, 2, 0]; //plru_tree_q[0,2,6]=={1,1,1}
199:         // replace_en[6] = &plru_tree_q[~6, 2, 0]; //plru_tree_q[0,2,6]=={1,1,0}
200:         // replace_en[5] = &plru_tree_q[ 5,~2, 0]; //plru_tree_q[0,2,5]=={1,0,1}
201:         // replace_en[4] = &plru_tree_q[~5,~2, 0]; //plru_tree_q[0,2,5]=={1,0,0}
202:         // replace_en[3] = &plru_tree_q[ 4, 1,~0]; //plru_tree_q[0,1,4]=={0,1,1}
203:         // replace_en[2] = &plru_tree_q[~4, 1,~0]; //plru_tree_q[0,1,4]=={0,1,0}
204:         // replace_en[1] = &plru_tree_q[ 3,~1,~0]; //plru_tree_q[0,1,3]=={0,0,1}
205:         // replace_en[0] = &plru_tree_q[~3,~1,~0]; //plru_tree_q[0,1,3]=={0,0,0}
206:         // For each entry traverse the tree. If every tree-node matches,
207:         // the corresponding bit of the entry's index, this is
208:         // the next entry to replace.
209:         for (int unsigned i = 0; i < TLB_ENTRIES; i += 1) begin
210:             automatic logic en;
211:             automatic int unsigned idx_base, shift, new_index;
212:             en = 1'b1;
213:             for (int unsigned lvl = 0; lvl < $clog2(TLB_ENTRIES); lvl++) begin
214:                 idx_base = $unsigned((2**lvl)-1);
215:                 // lvl0 <=> MSB, lvl1 <=> MSB-1, ...
216:                 shift = $clog2(TLB_ENTRIES) - lvl;
217: 
218:                 // en &= plru_tree_q[idx_base + (i>>shift)] == ((i >> (shift-1)) & 1'b1);
219:                 new_index =  (i >> (shift-1)) & 32'b1;
220:                 if (new_index[0]) begin
221:                   en &= plru_tree_q[idx_base + (i>>shift)];
222:                 end else begin
223:                   en &= ~plru_tree_q[idx_base + (i>>shift)];
224:                 end
225:             end
226:             replace_en[i] = en;
227:         end
228:     end
```

## cva6_sv39x4

原件：`iommu-platform/cva6/core/mmu_sv39x4/cva6_tlb_sv39x4.sv`；SHA-256 `bd0c0aef36b704978f5bc70923e804bf79c3a5a412d32e5ea8ba858900a4d4d7`。

原件 L18–L78：

```text
18: 
19: 
20: module cva6_tlb_sv39x4 import ariane_pkg::*; #(
21:       parameter int unsigned TLB_ENTRIES = 4,
22:       parameter int unsigned ASID_WIDTH  = 1,
23:       parameter int unsigned VMID_WIDTH  = 1
24:   )(
25:     input  logic                    clk_i,    // Clock
26:     input  logic                    rst_ni,   // Asynchronous reset active low
27:     input  logic                    flush_i,  // Flush normal translations signal
28:     input  logic                    flush_vvma_i,  // Flush vs stage signal
29:     input  logic                    flush_gvma_i,  // Flush g stage signal
30:     input  logic                    s_st_enbl_i,  // s-stage enabled
31:     input  logic                    g_st_enbl_i,  // g-stage enabled
32:     input  logic                    v_i,  // virtualization mode
33:     // Update TLB
34:     input  tlb_update_sv39x4_t      update_i,
35:     // Lookup signals
36:     input  logic                    lu_access_i,
37:     input  logic [ASID_WIDTH-1:0]   lu_asid_i,
38:     input  logic [VMID_WIDTH-1:0]   lu_vmid_i,
39:     input  logic [riscv::VLEN-1:0]  lu_vaddr_i,
40:     output logic [riscv::GPLEN-1:0] lu_gpaddr_o,
41:     output riscv::pte_t             lu_content_o,
42:     output riscv::pte_t             lu_g_content_o,
43:     input  logic [ASID_WIDTH-1:0]   asid_to_be_flushed_i,
44:     input  logic [VMID_WIDTH-1:0]   vmid_to_be_flushed_i,
45:     input  logic [riscv::VLEN-1:0]  vaddr_to_be_flushed_i,
46:     input  logic [riscv::GPLEN-1:0] gpaddr_to_be_flushed_i,
47:     output logic                    lu_is_2M_o,
48:     output logic                    lu_is_1G_o,
49:     output logic                    lu_hit_o
50: );
51: 
52:     // SV39 defines three levels of page tables
53:     struct packed {
54:       logic [ASID_WIDTH-1:0] asid;
55:       logic [VMID_WIDTH-1:0] vmid;
56:       logic [riscv::GPPN2:0] vpn2;
57:       logic [8:0]            vpn1;
58:       logic [8:0]            vpn0;
59:       logic                  is_s_2M;
60:       logic                  is_s_1G;
61:       logic                  is_g_2M;
62:       logic                  is_g_1G;
63:       logic                  s_st_enbl;   // s-stage translation
64:       logic                  g_st_enbl;   // g-stage translation
65:       logic                  v;           // virtualization mode
66:       logic                  valid;
67:     } [TLB_ENTRIES-1:0] tags_q, tags_n;
68: 
69:     struct packed {
70:       riscv::pte_t pte;
71:       riscv::pte_t gpte;
72:     } [TLB_ENTRIES-1:0] content_q, content_n;
73: 
74:     logic [8:0] vpn0, vpn1;
75:     logic [riscv::GPPN2:0] vpn2;
76:     logic [TLB_ENTRIES-1:0] lu_hit;     // to replacement logic
77:     logic [TLB_ENTRIES-1:0] replace_en; // replace the following entry, set by replacement strategy
78:     logic [TLB_ENTRIES-1:0] match_vmid;
```

原件 L246–L267：

```text
246:             // normal replacement
247:             end else if (update_i.valid & replace_en[i]) begin
248:                 // update tag array
249:                 tags_n[i] = '{
250:                     asid:  update_i.asid,
251:                     vmid:  update_i.vmid,
252:                     vpn2:  update_i.vpn[18+riscv::GPPN2:18],
253:                     vpn1:  update_i.vpn [17:9],
254:                     vpn0:  update_i.vpn [8:0],
255:                     s_st_enbl:  s_st_enbl_i,
256:                     g_st_enbl:  g_st_enbl_i,
257:                     v:  v_i,
258:                     is_s_1G: update_i.is_s_1G,
259:                     is_s_2M: update_i.is_s_2M,
260:                     is_g_1G: update_i.is_g_1G,
261:                     is_g_2M: update_i.is_g_2M,
262:                     valid: 1'b1
263:                 };
264:                 // and content as well
265:                 content_n[i].pte = update_i.content;
266:                 content_n[i].gpte = update_i.g_content;
267:             end
```

原件 L272–L348：

```text
272:     // PLRU - Pseudo Least Recently Used Replacement
273:     // -----------------------------------------------
274:     logic[2*(TLB_ENTRIES-1)-1:0] plru_tree_q, plru_tree_n;
275:     always_comb begin : plru_replacement
276:         plru_tree_n = plru_tree_q;
277:         // The PLRU-tree indexing:
278:         // lvl0        0
279:         //            / \
280:         //           /   \
281:         // lvl1     1     2
282:         //         / \   / \
283:         // lvl2   3   4 5   6
284:         //       / \ /\/\  /\
285:         //      ... ... ... ...
286:         // Just predefine which nodes will be set/cleared
287:         // E.g. for a TLB with 8 entries, the for-loop is semantically
288:         // equivalent to the following pseudo-code:
289:         // unique case (1'b1)
290:         // lu_hit[7]: plru_tree_n[0, 2, 6] = {1, 1, 1};
291:         // lu_hit[6]: plru_tree_n[0, 2, 6] = {1, 1, 0};
292:         // lu_hit[5]: plru_tree_n[0, 2, 5] = {1, 0, 1};
293:         // lu_hit[4]: plru_tree_n[0, 2, 5] = {1, 0, 0};
294:         // lu_hit[3]: plru_tree_n[0, 1, 4] = {0, 1, 1};
295:         // lu_hit[2]: plru_tree_n[0, 1, 4] = {0, 1, 0};
296:         // lu_hit[1]: plru_tree_n[0, 1, 3] = {0, 0, 1};
297:         // lu_hit[0]: plru_tree_n[0, 1, 3] = {0, 0, 0};
298:         // default: begin /* No hit */ end
299:         // endcase
300:         for (int unsigned i = 0; i < TLB_ENTRIES; i++) begin
301:             automatic int unsigned idx_base, shift, new_index;
302:             // we got a hit so update the pointer as it was least recently used
303:             if (lu_hit[i] & lu_access_i) begin
304:                 // Set the nodes to the values we would expect
305:                 for (int unsigned lvl = 0; lvl < $clog2(TLB_ENTRIES); lvl++) begin
306:                   idx_base = $unsigned((2**lvl)-1);
307:                   // lvl0 <=> MSB, lvl1 <=> MSB-1, ...
308:                   shift = $clog2(TLB_ENTRIES) - lvl;
309:                   // to circumvent the 32 bit integer arithmetic assignment
310:                   new_index =  ~((i >> (shift-1)) & 32'b1);
311:                   plru_tree_n[idx_base + (i >> shift)] = new_index[0];
312:                 end
313:             end
314:         end
315:         // Decode tree to write enable signals
316:         // Next for-loop basically creates the following logic for e.g. an 8 entry
317:         // TLB (note: pseudo-code obviously):
318:         // replace_en[7] = &plru_tree_q[ 6, 2, 0]; //plru_tree_q[0,2,6]=={1,1,1}
319:         // replace_en[6] = &plru_tree_q[~6, 2, 0]; //plru_tree_q[0,2,6]=={1,1,0}
320:         // replace_en[5] = &plru_tree_q[ 5,~2, 0]; //plru_tree_q[0,2,5]=={1,0,1}
321:         // replace_en[4] = &plru_tree_q[~5,~2, 0]; //plru_tree_q[0,2,5]=={1,0,0}
322:         // replace_en[3] = &plru_tree_q[ 4, 1,~0]; //plru_tree_q[0,1,4]=={0,1,1}
323:         // replace_en[2] = &plru_tree_q[~4, 1,~0]; //plru_tree_q[0,1,4]=={0,1,0}
324:         // replace_en[1] = &plru_tree_q[ 3,~1,~0]; //plru_tree_q[0,1,3]=={0,0,1}
325:         // replace_en[0] = &plru_tree_q[~3,~1,~0]; //plru_tree_q[0,1,3]=={0,0,0}
326:         // For each entry traverse the tree. If every tree-node matches,
327:         // the corresponding bit of the entry's index, this is
328:         // the next entry to replace.
329:         for (int unsigned i = 0; i < TLB_ENTRIES; i += 1) begin
330:             automatic logic en;
331:             automatic int unsigned idx_base, shift, new_index;
332:             en = 1'b1;
333:             for (int unsigned lvl = 0; lvl < $clog2(TLB_ENTRIES); lvl++) begin
334:                 idx_base = $unsigned((2**lvl)-1);
335:                 // lvl0 <=> MSB, lvl1 <=> MSB-1, ...
336:                 shift = $clog2(TLB_ENTRIES) - lvl;
337: 
338:                 // en &= plru_tree_q[idx_base + (i>>shift)] == ((i >> (shift-1)) & 1'b1);
339:                 new_index =  (i >> (shift-1)) & 32'b1;
340:                 if (new_index[0]) begin
341:                   en &= plru_tree_q[idx_base + (i>>shift)];
342:                 end else begin
343:                   en &= ~plru_tree_q[idx_base + (i>>shift)];
344:                 end
345:             end
346:             replace_en[i] = en;
347:         end
348:     end
```

## local_iotlb

原件：`iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv`；SHA-256 `2868d6de26c8d98d2a750a0d78f6ce96cb896b9253b82b070a3e3772f37e9d26`。

原件 L22–L55：

```text
22: 
23: module rv_iommu_iotlb #(
24:     /// RISC-V IOMMU configuration struct
25:     parameter rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfg = rv_iommu_cfg::NullCfg,
26:     parameter bit TouchEnable = 0,
27:     parameter bit PolicyEnable = 0,
28:     // Composition experiment: independent history for fixed ways 0-1 / 2-15.
29:     // Other geometries/masks and the default instance retain shared PLRU.
30:     parameter bit PartitionLocalHistory = 0
31: )(
32:     input  logic                        clk_i,
33:     input  logic                        rst_ni,
34: 
35:     // Flush
36:     input  rv_iommu::iotlb_inval_t      inval_i,
37: 
38:     // Experimental replacement allocation only; no lookup/permission change.
39:     input logic policy_clear_i,
40:     input logic [RVIOMMUCfg.NumIotlbEntries-1:0] policy_mask_i,
41: 
42:     // Update
43:     input  rv_iommu::iotlb_up_t         update_i,
44: 
45:     // Lookup
46:     input  logic                            lookup_i,
47:     input  logic                            touch_i,
48:     input  logic [rv_iommu::GPPNW39-1:0]    lu_vpn_i,
49:     input  rv_iommu::pscid_t                lu_pscid_i,
50:     input  rv_iommu::gscid_t                lu_gscid_i,
51:     input  logic                            en_1S_i,
52:     input  logic                            en_2S_i,
53:     output logic                            lu_hit_o,
54:     output rv_iommu::iotlb_content_t        lu_content_o,
55:     output rv_iommu::research_t            research_o
```

原件 L103–L136：

```text
103:             for (int unsigned i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
104:         
105:                 // PSCID match
106:                 match_pscid[i] = (((lu_pscid_i == tags_q[i].pscid) || content_q[i].content_1S.g) && en_1S_i) || !en_1S_i;
107: 
108:                 // GSCID match
109:                 match_gscid[i] = (lu_gscid_i == tags_q[i].gscid && en_2S_i) || !en_2S_i;
110: 
111:                 is_1G[i] = rv_iommu::is_trans_1G(   en_1S_i,
112:                                                     en_2S_i,
113:                                                     content_q[i].content_1S.is_1G,
114:                                                     content_q[i].content_2S.is_1G
115:                                                 );
116: 
117:                 is_2M[i] = rv_iommu::is_trans_2M(   en_1S_i,
118:                                                     en_2S_i,
119:                                                     content_q[i].content_1S.is_1G,
120:                                                     content_q[i].content_1S.is_2M,
121:                                                     content_q[i].content_2S.is_1G,
122:                                                     content_q[i].content_2S.is_2M
123:                                                 );
124: 
125:                 // Stage match
126:                 match_stage[i] = (tags_q[i].en_2S == en_2S_i) && (tags_q[i].en_1S == en_1S_i);
127:                 
128:                 // Full match
129:                 if (tags_q[i].valid && match_pscid[i] && match_gscid[i] && match_stage[i] && (vpn2 == tags_q[i].vpn2)) begin
130:                     
131:                     // 1G match | 2M match | 4k match
132:                     if (is_1G[i] || ((vpn1 == tags_q[i].vpn1) && (is_2M[i] || vpn0 == tags_q[i].vpn0))) begin
133:                         lu_content_o    = content_q[i];
134:                         lu_hit_o        = 1'b1;
135:                         lu_hit[i]       = 1'b1;
136:                     end
```

原件 L303–L332：

```text
303:                         if(tags_q[i].en_2S && tags_q[i].gscid == inval_i.gscid) begin
304:                             tags_n[i].valid = 1'b0;
305:                         end
306:                     end
307:                     2'b11: begin
308:                         // 2S enabled, 1S don't care, GSCID match, IOVA match
309:                         if(tags_q[i].en_2S && tags_q[i].gscid == inval_i.gscid && 
310:                            (gpaddr_4k_match[i] || gpaddr_2M_match[i] || gpaddr_1G_match[i])) begin
311:                             tags_n[i].valid = 1'b0;
312:                         end
313:                     end
314:                 endcase
315:             end
316: 
317:             // update
318:             else if (update_i.update && replace_en[i]) begin
319: 
320:                 tags_n[i] = '{
321:                     pscid:      update_i.pscid,
322:                     gscid:      update_i.gscid,
323:                     vpn2:       update_i.vpn[rv_iommu::GPPNW39-1:18],
324:                     vpn1:       update_i.vpn[17:9],
325:                     vpn0:       update_i.vpn[8:0],
326:                     en_1S:      en_1S_i,
327:                     en_2S:      en_2S_i,
328:                     valid:      1'b1
329:                 };
330:                 
331:                 content_n[i] = update_i.content;
332:             end
```

原件 L336–L425：

```text
336:     //-----------------------------------------------
337:     // PLRU - Pseudo Least Recently Used Replacement
338:     //-----------------------------------------------
339:     logic[(RVIOMMUCfg.NumIotlbEntries-2):0] plru_tree_q, plru_tree_n;
340: 
341:     function automatic logic [2*RVIOMMUCfg.NumIotlbEntries-2:0] history_nodes(
342:         input logic [RVIOMMUCfg.NumIotlbEntries-1:0] owners
343:     );
344:         logic [2*RVIOMMUCfg.NumIotlbEntries-2:0] nodes;
345:         nodes = '0;
346:         for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++)
347:             nodes[RVIOMMUCfg.NumIotlbEntries-1+i] = owners[i];
348:         for (int i = RVIOMMUCfg.NumIotlbEntries-2; i >= 0; i--)
349:             nodes[i] = nodes[2*i+1] | nodes[2*i+2];
350:         return nodes;
351:     endfunction
352: 
353:     localparam logic [RVIOMMUCfg.NumIotlbEntries-1:0] ProtectedWays =
354:         RVIOMMUCfg.NumIotlbEntries'(16'h0003);
355:     localparam logic [2*RVIOMMUCfg.NumIotlbEntries-2:0] ProtectedNodes =
356:         history_nodes(ProtectedWays);
357:     localparam logic [2*RVIOMMUCfg.NumIotlbEntries-2:0] BackgroundNodes =
358:         history_nodes(~ProtectedWays);
359:     logic partition_local_active;
360:     assign partition_local_active = PartitionLocalHistory && PolicyEnable &&
361:         RVIOMMUCfg.NumIotlbEntries == 16 &&
362:         ((policy_mask_i == ProtectedWays) || (policy_mask_i == ~ProtectedWays));
363: 
364:     logic [2*RVIOMMUCfg.NumIotlbEntries-2:0] eligible_nodes;
365:     logic [RVIOMMUCfg.NumIotlbEntries-1:0] allocation_mask;
366:     assign allocation_mask = PolicyEnable ? policy_mask_i : '1;
367:     always_comb begin
368:         eligible_nodes = '0;
369:         for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++)
370:             eligible_nodes[RVIOMMUCfg.NumIotlbEntries-1+i] = allocation_mask[i];
371:         for (int i = RVIOMMUCfg.NumIotlbEntries-2; i >= 0; i--)
372:             eligible_nodes[i] = eligible_nodes[2*i+1] | eligible_nodes[2*i+2];
373:     end
374: 
375:     always_comb begin : plru_replacement
376:         plru_tree_n = plru_tree_q;
377:         
378:         for (int unsigned i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
379:             automatic int unsigned idx_base, shift, new_index;
380:             idx_base    = 0;
381:             shift       = 0;
382:             new_index   = 0;
383: 
384:             if (lu_hit[i] && (!TouchEnable || touch_i)) begin
385:                 for (int unsigned lvl = 0; lvl < $clog2(RVIOMMUCfg.NumIotlbEntries); lvl++) begin
386:                     idx_base = $unsigned((2**lvl)-1);
387:                     shift = $clog2(RVIOMMUCfg.NumIotlbEntries) - lvl;
388:                     new_index =  ~((i >> (shift-1)) & 32'b1);
389:                     // Fill ownership belongs to the main request, but a hit
390:                     // may belong to a different fast request. Derive touch
391:                     // ownership from the resident way, never policy_mask_i.
392:                     // A branch used by only one child of this partition is
393:                     // forced by the allocation mask and needs no history.
394:                     if (!partition_local_active ||
395:                         (i < 2 ?
396:                          (ProtectedNodes[2*(idx_base + (i >> shift))+1] &&
397:                           ProtectedNodes[2*(idx_base + (i >> shift))+2]) :
398:                          (BackgroundNodes[2*(idx_base + (i >> shift))+1] &&
399:                           BackgroundNodes[2*(idx_base + (i >> shift))+2])))
400:                         plru_tree_n[idx_base + (i >> shift)] = new_index[0];
401:                 end
402:             end
403:         end
404: 
405:         for (int unsigned i = 0; i < RVIOMMUCfg.NumIotlbEntries; i += 1) begin
406:             automatic logic en, choose_right;
407:             automatic int unsigned idx_base, shift, new_index;
408:             en = 1'b1;
409: 
410:             for (int unsigned lvl = 0; lvl < $clog2(RVIOMMUCfg.NumIotlbEntries); lvl++) begin
411:                 idx_base = $unsigned((2**lvl)-1);
412:                 shift = $clog2(RVIOMMUCfg.NumIotlbEntries) - lvl;
413:                 new_index =  (i >> (shift-1)) & 32'b1;
414:                 // Follow tree-PLRU whenever that child has eligible leaves.
415:                 // A full mask reproduces the original victim bit for bit.
416:                 choose_right = eligible_nodes[2*(idx_base + (i>>shift))+2] &&
417:                     (!eligible_nodes[2*(idx_base + (i>>shift))+1] ||
418:                      plru_tree_q[idx_base + (i>>shift)]);
419:                 if (new_index[0]) en &= choose_right;
420:                 else en &= ~choose_right;
421:             end
422:             replace_en[i] = en && allocation_mask[i];
423:         end
424:         if (PolicyEnable && policy_clear_i) plru_tree_n = '0;
425:     end
```

## linux_dma_iommu

原件：`iommu-platform/linux/drivers/iommu/dma-iommu.c`；SHA-256 `68862ded252075324f542f2a66db4aceaa6179480cbdbd24f5400394ff2ea176`。

原件 L613–L621：

```text
613: 	ret = iova_domain_init_rcaches(iovad);
614: 	if (ret)
615: 		goto done_unlock;
616: 
617: 	/* If the FQ fails we can simply fall back to strict mode */
618: 	if (domain->type == IOMMU_DOMAIN_DMA_FQ &&
619: 	    (!device_iommu_capable(dev, IOMMU_CAP_DEFERRED_FLUSH) || iommu_dma_init_fq(domain)))
620: 		domain->type = IOMMU_DOMAIN_DMA;
621: 
```

原件 L658–L754：

```text
658: static dma_addr_t iommu_dma_alloc_iova(struct iommu_domain *domain,
659: 		size_t size, u64 dma_limit, struct device *dev)
660: {
661: 	struct iommu_dma_cookie *cookie = domain->iova_cookie;
662: 	struct iova_domain *iovad = &cookie->iovad;
663: 	unsigned long shift, iova_len, iova = 0;
664: 
665: 	if (cookie->type == IOMMU_DMA_MSI_COOKIE) {
666: 		cookie->msi_iova += size;
667: 		return cookie->msi_iova - size;
668: 	}
669: 
670: 	shift = iova_shift(iovad);
671: 	iova_len = size >> shift;
672: 
673: 	dma_limit = min_not_zero(dma_limit, dev->bus_dma_limit);
674: 
675: 	if (domain->geometry.force_aperture)
676: 		dma_limit = min(dma_limit, (u64)domain->geometry.aperture_end);
677: 
678: 	/* Try to get PCI devices a SAC address */
679: 	if (dma_limit > DMA_BIT_MASK(32) && !iommu_dma_forcedac && dev_is_pci(dev))
680: 		iova = alloc_iova_fast(iovad, iova_len,
681: 				       DMA_BIT_MASK(32) >> shift, false);
682: 
683: 	if (!iova)
684: 		iova = alloc_iova_fast(iovad, iova_len, dma_limit >> shift,
685: 				       true);
686: 
687: 	return (dma_addr_t)iova << shift;
688: }
689: 
690: static void iommu_dma_free_iova(struct iommu_dma_cookie *cookie,
691: 		dma_addr_t iova, size_t size, struct iommu_iotlb_gather *gather)
692: {
693: 	struct iova_domain *iovad = &cookie->iovad;
694: 
695: 	/* The MSI case is only ever cleaning up its most recent allocation */
696: 	if (cookie->type == IOMMU_DMA_MSI_COOKIE)
697: 		cookie->msi_iova -= size;
698: 	else if (gather && gather->queued)
699: 		queue_iova(cookie, iova_pfn(iovad, iova),
700: 				size >> iova_shift(iovad),
701: 				&gather->freelist);
702: 	else
703: 		free_iova_fast(iovad, iova_pfn(iovad, iova),
704: 				size >> iova_shift(iovad));
705: }
706: 
707: static void __iommu_dma_unmap(struct device *dev, dma_addr_t dma_addr,
708: 		size_t size)
709: {
710: 	struct iommu_domain *domain = iommu_get_dma_domain(dev);
711: 	struct iommu_dma_cookie *cookie = domain->iova_cookie;
712: 	struct iova_domain *iovad = &cookie->iovad;
713: 	size_t iova_off = iova_offset(iovad, dma_addr);
714: 	struct iommu_iotlb_gather iotlb_gather;
715: 	size_t unmapped;
716: 
717: 	dma_addr -= iova_off;
718: 	size = iova_align(iovad, size + iova_off);
719: 	iommu_iotlb_gather_init(&iotlb_gather);
720: 	iotlb_gather.queued = READ_ONCE(cookie->fq_domain);
721: 
722: 	unmapped = iommu_unmap_fast(domain, dma_addr, size, &iotlb_gather);
723: 	WARN_ON(unmapped != size);
724: 
725: 	if (!iotlb_gather.queued)
726: 		iommu_iotlb_sync(domain, &iotlb_gather);
727: 	iommu_dma_free_iova(cookie, dma_addr, size, &iotlb_gather);
728: }
729: 
730: static dma_addr_t __iommu_dma_map(struct device *dev, phys_addr_t phys,
731: 		size_t size, int prot, u64 dma_mask)
732: {
733: 	struct iommu_domain *domain = iommu_get_dma_domain(dev);
734: 	struct iommu_dma_cookie *cookie = domain->iova_cookie;
735: 	struct iova_domain *iovad = &cookie->iovad;
736: 	size_t iova_off = iova_offset(iovad, phys);
737: 	dma_addr_t iova;
738: 
739: 	if (static_branch_unlikely(&iommu_deferred_attach_enabled) &&
740: 	    iommu_deferred_attach(dev, domain))
741: 		return DMA_MAPPING_ERROR;
742: 
743: 	size = iova_align(iovad, size + iova_off);
744: 
745: 	iova = iommu_dma_alloc_iova(domain, size, dma_mask, dev);
746: 	if (!iova)
747: 		return DMA_MAPPING_ERROR;
748: 
749: 	if (iommu_map(domain, iova, phys - iova_off, size, prot, GFP_ATOMIC)) {
750: 		iommu_dma_free_iova(cookie, iova, size, NULL);
751: 		return DMA_MAPPING_ERROR;
752: 	}
753: 	return iova + iova_off;
754: }
```

原件 L1015–L1087：

```text
1015: static dma_addr_t iommu_dma_map_page(struct device *dev, struct page *page,
1016: 		unsigned long offset, size_t size, enum dma_data_direction dir,
1017: 		unsigned long attrs)
1018: {
1019: 	phys_addr_t phys = page_to_phys(page) + offset;
1020: 	bool coherent = dev_is_dma_coherent(dev);
1021: 	int prot = dma_info_to_prot(dir, coherent, attrs);
1022: 	struct iommu_domain *domain = iommu_get_dma_domain(dev);
1023: 	struct iommu_dma_cookie *cookie = domain->iova_cookie;
1024: 	struct iova_domain *iovad = &cookie->iovad;
1025: 	dma_addr_t iova, dma_mask = dma_get_mask(dev);
1026: 
1027: 	/*
1028: 	 * If both the physical buffer start address and size are
1029: 	 * page aligned, we don't need to use a bounce page.
1030: 	 */
1031: 	if (dev_use_swiotlb(dev, size, dir) &&
1032: 	    iova_offset(iovad, phys | size)) {
1033: 		void *padding_start;
1034: 		size_t padding_size, aligned_size;
1035: 
1036: 		if (!is_swiotlb_active(dev)) {
1037: 			dev_warn_once(dev, "DMA bounce buffers are inactive, unable to map unaligned transaction.\n");
1038: 			return DMA_MAPPING_ERROR;
1039: 		}
1040: 
1041: 		aligned_size = iova_align(iovad, size);
1042: 		phys = swiotlb_tbl_map_single(dev, phys, size, aligned_size,
1043: 					      iova_mask(iovad), dir, attrs);
1044: 
1045: 		if (phys == DMA_MAPPING_ERROR)
1046: 			return DMA_MAPPING_ERROR;
1047: 
1048: 		/* Cleanup the padding area. */
1049: 		padding_start = phys_to_virt(phys);
1050: 		padding_size = aligned_size;
1051: 
1052: 		if (!(attrs & DMA_ATTR_SKIP_CPU_SYNC) &&
1053: 		    (dir == DMA_TO_DEVICE || dir == DMA_BIDIRECTIONAL)) {
1054: 			padding_start += size;
1055: 			padding_size -= size;
1056: 		}
1057: 
1058: 		memset(padding_start, 0, padding_size);
1059: 	}
1060: 
1061: 	if (!coherent && !(attrs & DMA_ATTR_SKIP_CPU_SYNC))
1062: 		arch_sync_dma_for_device(phys, size, dir);
1063: 
1064: 	iova = __iommu_dma_map(dev, phys, size, prot, dma_mask);
1065: 	if (iova == DMA_MAPPING_ERROR && is_swiotlb_buffer(dev, phys))
1066: 		swiotlb_tbl_unmap_single(dev, phys, size, dir, attrs);
1067: 	return iova;
1068: }
1069: 
1070: static void iommu_dma_unmap_page(struct device *dev, dma_addr_t dma_handle,
1071: 		size_t size, enum dma_data_direction dir, unsigned long attrs)
1072: {
1073: 	struct iommu_domain *domain = iommu_get_dma_domain(dev);
1074: 	phys_addr_t phys;
1075: 
1076: 	phys = iommu_iova_to_phys(domain, dma_handle);
1077: 	if (WARN_ON(!phys))
1078: 		return;
1079: 
1080: 	if (!(attrs & DMA_ATTR_SKIP_CPU_SYNC) && !dev_is_dma_coherent(dev))
1081: 		arch_sync_dma_for_cpu(phys, size, dir);
1082: 
1083: 	__iommu_dma_unmap(dev, dma_handle, size);
1084: 
1085: 	if (unlikely(is_swiotlb_buffer(dev, phys)))
1086: 		swiotlb_tbl_unmap_single(dev, phys, size, dir, attrs);
1087: }
```

原件 L1588–L1603：

```text
1588: static const struct dma_map_ops iommu_dma_ops = {
1589: 	.flags			= DMA_F_PCI_P2PDMA_SUPPORTED,
1590: 	.alloc			= iommu_dma_alloc,
1591: 	.free			= iommu_dma_free,
1592: 	.alloc_pages		= dma_common_alloc_pages,
1593: 	.free_pages		= dma_common_free_pages,
1594: 	.alloc_noncontiguous	= iommu_dma_alloc_noncontiguous,
1595: 	.free_noncontiguous	= iommu_dma_free_noncontiguous,
1596: 	.mmap			= iommu_dma_mmap,
1597: 	.get_sgtable		= iommu_dma_get_sgtable,
1598: 	.map_page		= iommu_dma_map_page,
1599: 	.unmap_page		= iommu_dma_unmap_page,
1600: 	.map_sg			= iommu_dma_map_sg,
1601: 	.unmap_sg		= iommu_dma_unmap_sg,
1602: 	.sync_single_for_cpu	= iommu_dma_sync_single_for_cpu,
1603: 	.sync_single_for_device	= iommu_dma_sync_single_for_device,
```

## linux_iova

原件：`iommu-platform/linux/drivers/iommu/iova.c`；SHA-256 `4407089fdf9b019146167b3a4e17bb40872a3de9d19ace1261e49136d9d6b012`。

原件 L178–L236：

```text
178: static int __alloc_and_insert_iova_range(struct iova_domain *iovad,
179: 		unsigned long size, unsigned long limit_pfn,
180: 			struct iova *new, bool size_aligned)
181: {
182: 	struct rb_node *curr, *prev;
183: 	struct iova *curr_iova;
184: 	unsigned long flags;
185: 	unsigned long new_pfn, retry_pfn;
186: 	unsigned long align_mask = ~0UL;
187: 	unsigned long high_pfn = limit_pfn, low_pfn = iovad->start_pfn;
188: 
189: 	if (size_aligned)
190: 		align_mask <<= fls_long(size - 1);
191: 
192: 	/* Walk the tree backwards */
193: 	spin_lock_irqsave(&iovad->iova_rbtree_lock, flags);
194: 	if (limit_pfn <= iovad->dma_32bit_pfn &&
195: 			size >= iovad->max32_alloc_size)
196: 		goto iova32_full;
197: 
198: 	curr = __get_cached_rbnode(iovad, limit_pfn);
199: 	curr_iova = to_iova(curr);
200: 	retry_pfn = curr_iova->pfn_hi;
201: 
202: retry:
203: 	do {
204: 		high_pfn = min(high_pfn, curr_iova->pfn_lo);
205: 		new_pfn = (high_pfn - size) & align_mask;
206: 		prev = curr;
207: 		curr = rb_prev(curr);
208: 		curr_iova = to_iova(curr);
209: 	} while (curr && new_pfn <= curr_iova->pfn_hi && new_pfn >= low_pfn);
210: 
211: 	if (high_pfn < size || new_pfn < low_pfn) {
212: 		if (low_pfn == iovad->start_pfn && retry_pfn < limit_pfn) {
213: 			high_pfn = limit_pfn;
214: 			low_pfn = retry_pfn + 1;
215: 			curr = iova_find_limit(iovad, limit_pfn);
216: 			curr_iova = to_iova(curr);
217: 			goto retry;
218: 		}
219: 		iovad->max32_alloc_size = size;
220: 		goto iova32_full;
221: 	}
222: 
223: 	/* pfn_lo will point to size aligned address if size_aligned is set */
224: 	new->pfn_lo = new_pfn;
225: 	new->pfn_hi = new->pfn_lo + size - 1;
226: 
227: 	/* If we have 'prev', it's a valid place to start the insertion. */
228: 	iova_insert_rbtree(&iovad->rbroot, new, prev);
229: 	__cached_rbnode_insert_update(iovad, new);
230: 
231: 	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
232: 	return 0;
233: 
234: iova32_full:
235: 	spin_unlock_irqrestore(&iovad->iova_rbtree_lock, flags);
236: 	return -ENOMEM;
```

原件 L439–L493：

```text
439: alloc_iova_fast(struct iova_domain *iovad, unsigned long size,
440: 		unsigned long limit_pfn, bool flush_rcache)
441: {
442: 	unsigned long iova_pfn;
443: 	struct iova *new_iova;
444: 
445: 	/*
446: 	 * Freeing non-power-of-two-sized allocations back into the IOVA caches
447: 	 * will come back to bite us badly, so we have to waste a bit of space
448: 	 * rounding up anything cacheable to make sure that can't happen. The
449: 	 * order of the unadjusted size will still match upon freeing.
450: 	 */
451: 	if (size < (1 << (IOVA_RANGE_CACHE_MAX_SIZE - 1)))
452: 		size = roundup_pow_of_two(size);
453: 
454: 	iova_pfn = iova_rcache_get(iovad, size, limit_pfn + 1);
455: 	if (iova_pfn)
456: 		return iova_pfn;
457: 
458: retry:
459: 	new_iova = alloc_iova(iovad, size, limit_pfn, true);
460: 	if (!new_iova) {
461: 		unsigned int cpu;
462: 
463: 		if (!flush_rcache)
464: 			return 0;
465: 
466: 		/* Try replenishing IOVAs by flushing rcache. */
467: 		flush_rcache = false;
468: 		for_each_online_cpu(cpu)
469: 			free_cpu_cached_iovas(cpu, iovad);
470: 		free_global_cached_iovas(iovad);
471: 		goto retry;
472: 	}
473: 
474: 	return new_iova->pfn_lo;
475: }
476: EXPORT_SYMBOL_GPL(alloc_iova_fast);
477: 
478: /**
479:  * free_iova_fast - free iova pfn range into rcache
480:  * @iovad: - iova domain in question.
481:  * @pfn: - pfn that is allocated previously
482:  * @size: - # of pages in range
483:  * This functions frees an iova range by trying to put it into the rcache,
484:  * falling back to regular iova deallocation via free_iova() if this fails.
485:  */
486: void
487: free_iova_fast(struct iova_domain *iovad, unsigned long pfn, unsigned long size)
488: {
489: 	if (iova_rcache_insert(iovad, pfn, size))
490: 		return;
491: 
492: 	free_iova(iovad, pfn);
493: }
```

原件 L697–L718：

```text
697: static unsigned long iova_magazine_pop(struct iova_magazine *mag,
698: 				       unsigned long limit_pfn)
699: {
700: 	int i;
701: 	unsigned long pfn;
702: 
703: 	/* Only fall back to the rbtree if we have no suitable pfns at all */
704: 	for (i = mag->size - 1; mag->pfns[i] > limit_pfn; i--)
705: 		if (i == 0)
706: 			return 0;
707: 
708: 	/* Swap it to pop it */
709: 	pfn = mag->pfns[i];
710: 	mag->pfns[i] = mag->pfns[--mag->size];
711: 
712: 	return pfn;
713: }
714: 
715: static void iova_magazine_push(struct iova_magazine *mag, unsigned long pfn)
716: {
717: 	mag->pfns[mag->size++] = pfn;
718: }
```

原件 L839–L888：

```text
839: static unsigned long __iova_rcache_get(struct iova_rcache *rcache,
840: 				       unsigned long limit_pfn)
841: {
842: 	struct iova_cpu_rcache *cpu_rcache;
843: 	unsigned long iova_pfn = 0;
844: 	bool has_pfn = false;
845: 	unsigned long flags;
846: 
847: 	cpu_rcache = raw_cpu_ptr(rcache->cpu_rcaches);
848: 	spin_lock_irqsave(&cpu_rcache->lock, flags);
849: 
850: 	if (!iova_magazine_empty(cpu_rcache->loaded)) {
851: 		has_pfn = true;
852: 	} else if (!iova_magazine_empty(cpu_rcache->prev)) {
853: 		swap(cpu_rcache->prev, cpu_rcache->loaded);
854: 		has_pfn = true;
855: 	} else {
856: 		spin_lock(&rcache->lock);
857: 		if (rcache->depot_size > 0) {
858: 			iova_magazine_free(cpu_rcache->loaded);
859: 			cpu_rcache->loaded = rcache->depot[--rcache->depot_size];
860: 			has_pfn = true;
861: 		}
862: 		spin_unlock(&rcache->lock);
863: 	}
864: 
865: 	if (has_pfn)
866: 		iova_pfn = iova_magazine_pop(cpu_rcache->loaded, limit_pfn);
867: 
868: 	spin_unlock_irqrestore(&cpu_rcache->lock, flags);
869: 
870: 	return iova_pfn;
871: }
872: 
873: /*
874:  * Try to satisfy IOVA allocation range from rcache.  Fail if requested
875:  * size is too big or the DMA limit we are given isn't satisfied by the
876:  * top element in the magazine.
877:  */
878: static unsigned long iova_rcache_get(struct iova_domain *iovad,
879: 				     unsigned long size,
880: 				     unsigned long limit_pfn)
881: {
882: 	unsigned int log_size = order_base_2(size);
883: 
884: 	if (log_size >= IOVA_RANGE_CACHE_MAX_SIZE)
885: 		return 0;
886: 
887: 	return __iova_rcache_get(&iovad->rcaches[log_size], limit_pfn - size);
888: }
```

## linux_riscv_iommu

原件：`iommu-platform/linux/drivers/iommu/riscv/iommu.c`；SHA-256 `62c137d53cfaeac5374aefebd84f08457a362ddfde681192f550aed6a608ef0c`。

原件 L1198–L1210：

```text
1198: static bool riscv_iommu_capable(struct device *dev, enum iommu_cap cap)
1199: {
1200: 	switch (cap) {
1201: 	case IOMMU_CAP_CACHE_COHERENCY:
1202: 	case IOMMU_CAP_PRE_BOOT_PROTECTION:
1203: 		return true;
1204: 
1205: 	default:
1206: 		break;
1207: 	}
1208: 
1209: 	return false;
1210: }
```

原件 L1777–L1817：

```text
1777: static void riscv_iommu_flush_iotlb_range(struct iommu_domain *iommu_domain,
1778: 					  unsigned long *start, unsigned long *end,
1779: 					  size_t *pgsize)
1780: {
1781: 	struct riscv_iommu_domain *domain = iommu_domain_to_riscv(iommu_domain);
1782: 	struct riscv_iommu_command cmd;
1783: 	struct riscv_iommu_endpoint *endpoint;
1784: 	// unsigned long iova;
1785: 	unsigned long payload;
1786: 
1787: 	if (domain->mode == RISCV_IOMMU_DC_FSC_MODE_BARE)
1788: 		return;
1789: 
1790: 	/* Domain not attached to an IOMMU! */
1791: 	BUG_ON(!domain->iommu);
1792: 
1793: 	if (start && end) {
1794: 		payload = riscv_iommu_ats_inval_payload(*start, *end, true);
1795: 	} else {
1796: 		payload = riscv_iommu_ats_inval_all_payload(true);
1797: 	}
1798: 
1799: 	if (domain->g_stage) {
1800: 		riscv_iommu_cmd_inval_gvma(&cmd);
1801: 		riscv_iommu_cmd_inval_set_gscid(&cmd, domain->id);
1802: 	} else {
1803: 		riscv_iommu_cmd_inval_vma(&cmd);
1804: 		// riscv_iommu_cmd_inval_set_pscid(&cmd, domain->id);
1805: 	}
1806: 
1807: 	// if (start && end && pgsize) {
1808: 	// 	/* Cover only the range that is needed */
1809: 	// 	for (iova = *start; iova <= *end; iova += *pgsize) {
1810: 	// 		printk("Sending IOTINVAL with ADDR");
1811: 	// 		riscv_iommu_cmd_inval_set_addr(&cmd, iova);
1812: 	// 		riscv_iommu_post(domain->iommu, &cmd);
1813: 	// 	}
1814: 	// } else {
1815: 		riscv_iommu_post(domain->iommu, &cmd);
1816: 	// }
1817: 	riscv_iommu_iofence_sync(domain->iommu);
```

原件 L1841–L1846：

```text
1841: static void riscv_iommu_iotlb_sync(struct iommu_domain *iommu_domain,
1842: 				   struct iommu_iotlb_gather *gather)
1843: {
1844: 	riscv_iommu_flush_iotlb_range(iommu_domain, &gather->start, &gather->end,
1845: 				      &gather->pgsize);
1846: }
```

## linux_idma

原件：`iommu-platform/linux/drivers/misc/idma.c`；SHA-256 `5d36f9c0a55288001cf9e698a5b306e08914c77065a9747420637a81d2457dc9`。

原件 L242–L268：

```text
242: static int idma_find_iova(struct iommu_domain *domain, size_t size,
243: 			  unsigned long *src_iova, unsigned long *dst_iova)
244: {
245: 	unsigned long start = domain->geometry.aperture_start;
246: 	unsigned long end = domain->geometry.aperture_end;
247: 	unsigned long iova;
248: 	size_t span = size * 2;
249: 
250: 	if (end < start || span < size || span - 1 > end - start)
251: 		return -EBUSY;
252: 
253: 	iova = ALIGN_DOWN(end - span + 1, SZ_2M);
254: 	for (;;) {
255: 		if (!idma_iova_range_free(domain, iova, span)) {
256: 			if (iova < start + SZ_2M)
257: 				break;
258: 			iova -= SZ_2M;
259: 			continue;
260: 		}
261: 
262: 		*src_iova = iova;
263: 		*dst_iova = iova + size;
264: 		return 0;
265: 	}
266: 
267: 	return -EBUSY;
268: }
```

原件 L354–L399：

```text
354: static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
355: 			       u32 pages, u32 chunk,
356: 			       struct idma_experiment_stats *stats)
357: {
358: 	struct page *src_page, *dst_page;
359: 	size_t i;
360: 	int ret;
361: 
362: 	s->idma = idma;
363: 	s->dev = idma->miscdev.parent;
364: 	s->stats = stats;
365: 	s->domain = iommu_get_domain_for_dev(s->dev);
366: 	if (!s->domain || s->domain->type == IOMMU_DOMAIN_IDENTITY)
367: 		return -EOPNOTSUPP;
368: 	s->size = (size_t)pages * PAGE_SIZE;
369: 	s->chunk = chunk;
370: 	s->src = dma_alloc_attrs(s->dev, s->size, &s->src_dma, GFP_KERNEL,
371: 				 DMA_ATTR_FORCE_CONTIGUOUS);
372: 	if (!s->src)
373: 		return -ENOMEM;
374: 	s->dst = dma_alloc_attrs(s->dev, s->size, &s->dst_dma, GFP_KERNEL,
375: 				 DMA_ATTR_FORCE_CONTIGUOUS);
376: 	if (!s->dst)
377: 		return -ENOMEM;
378: 	for (i = 0; i < s->size / sizeof(u64); i++) {
379: 		((u64 *)s->src)[i] = idma_pattern(s, i);
380: 		((u64 *)s->dst)[i] = ~idma_pattern(s, i);
381: 	}
382: 	src_page = idma_buffer_page(s->src);
383: 	dst_page = idma_buffer_page(s->dst);
384: 	if (!src_page || !dst_page)
385: 		return -EFAULT;
386: 	ret = idma_find_iova(s->domain, s->size, &s->src_iova, &s->dst_iova);
387: 	if (ret)
388: 		return ret;
389: 	ret = idma_iommu_map_range(s->domain, s->src_iova,
390: 				   page_to_phys(src_page), s->size, PAGE_SIZE);
391: 	if (ret)
392: 		return ret;
393: 	s->mapped_src = true;
394: 	ret = idma_iommu_map_range(s->domain, s->dst_iova,
395: 				   page_to_phys(dst_page), s->size, PAGE_SIZE);
396: 	if (ret)
397: 		return ret;
398: 	s->mapped_dst = true;
399: 	return 0;
```

## historical_idma

原件：`iommu-platform/out/dma-cache-sync-20260916/idma-before.c`；SHA-256 `b63560df143d2fe58be5eff8620755b44ca81c0b7ccaaf96883374444e417305`。

原件 L157–L182：

```text
157: static int idma_find_iova(struct iommu_domain *domain, size_t size,
158: 			  unsigned long *src_iova, unsigned long *dst_iova)
159: {
160: 	unsigned long start = domain->geometry.aperture_start;
161: 	unsigned long end = domain->geometry.aperture_end;
162: 	unsigned long iova;
163: 	size_t span = size * 2;
164: 
165: 	if (end < start || span < size || span - 1 > end - start)
166: 		return -EBUSY;
167: 
168: 	iova = ALIGN_DOWN(end - span + 1, SZ_2M);
169: 	for (;;) {
170: 		if (!idma_iova_range_free(domain, iova, span)) {
171: 			if (iova < start + SZ_2M)
172: 				break;
173: 			iova -= SZ_2M;
174: 			continue;
175: 		}
176: 
177: 		*src_iova = iova;
178: 		*dst_iova = iova + size;
179: 		return 0;
180: 	}
181: 
182: 	return -EBUSY;
```

原件 L264–L309：

```text
264: static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
265: 			       u32 pages, u32 chunk,
266: 			       struct idma_experiment_stats *stats)
267: {
268: 	struct page *src_page, *dst_page;
269: 	size_t i;
270: 	int ret;
271: 
272: 	s->idma = idma;
273: 	s->dev = idma->miscdev.parent;
274: 	s->stats = stats;
275: 	s->domain = iommu_get_domain_for_dev(s->dev);
276: 	if (!s->domain || s->domain->type == IOMMU_DOMAIN_IDENTITY)
277: 		return -EOPNOTSUPP;
278: 	s->size = (size_t)pages * PAGE_SIZE;
279: 	s->chunk = chunk;
280: 	s->src = dma_alloc_attrs(s->dev, s->size, &s->src_dma, GFP_KERNEL,
281: 				 DMA_ATTR_FORCE_CONTIGUOUS);
282: 	if (!s->src)
283: 		return -ENOMEM;
284: 	s->dst = dma_alloc_attrs(s->dev, s->size, &s->dst_dma, GFP_KERNEL,
285: 				 DMA_ATTR_FORCE_CONTIGUOUS);
286: 	if (!s->dst)
287: 		return -ENOMEM;
288: 	for (i = 0; i < s->size / sizeof(u64); i++) {
289: 		((u64 *)s->src)[i] = idma_pattern(s, i);
290: 		((u64 *)s->dst)[i] = ~idma_pattern(s, i);
291: 	}
292: 	src_page = idma_buffer_page(s->src);
293: 	dst_page = idma_buffer_page(s->dst);
294: 	if (!src_page || !dst_page)
295: 		return -EFAULT;
296: 	ret = idma_find_iova(s->domain, s->size, &s->src_iova, &s->dst_iova);
297: 	if (ret)
298: 		return ret;
299: 	ret = idma_iommu_map_range(s->domain, s->src_iova,
300: 				  page_to_phys(src_page), s->size, PAGE_SIZE);
301: 	if (ret)
302: 		return ret;
303: 	s->mapped_src = true;
304: 	ret = idma_iommu_map_range(s->domain, s->dst_iova,
305: 				  page_to_phys(dst_page), s->size, PAGE_SIZE);
306: 	if (ret)
307: 		return ret;
308: 	s->mapped_dst = true;
309: 	return 0;
```

## linux_iova_mismatch

原件：`iommu-platform/out/dma-smoke-debug-20260916/mac-20260916-020422-run-_hjma4td/mismatch.txt`；SHA-256 `66f53f3a88800efdb7e13c1e2b5730dc2a4ba3cec092a532be6aa38afe44d8b6`。

原件 L1–L1：

```text
1: [  272.307762] zdl,idma 50000000.master: experiment data mismatch: id=0 did=10 offset=912 touched_end=4096 size=4096 chunk=4096 src=14 src_after_fence_i=14 expected_src=14 dst=eb dst_after_fence_i=14 expected_dst=14 src_iova=0x3fffe00390 dst_iova=0x3fffe01390 src_pa=0x0000000082d10390 dst_pa=0x0000000082d49390 src_pte_pa=0x0000003ffff10000 dst_pte_pa=0x0000003ffff49000
```

## upstream_iotlb

官方固定版本见 upstream-source-receipt.json。

原件 L14–L68：

```text
14: // Date:    04/11/2022
15: // Acknowledges: SSRC - Technology Innovation Institute (TII)
16: //
17: // Description: IO Translation Lookaside Buffer (IOTLB) for RISC-V IOMMU.
18: //              Compliant with the Sv39x4 virtual memory scheme, as defined
19: //              in the RISC-V Privileged Specification 1.12
20: //              This module is an adaptation of the Sv39 TLB developed
21: //              by Florian Zaruba and David Schaffenrath to the Sv39x4 standard.
22: //              Each entry groups first-stage and second-stage PTE data. Second-stage data may be an MSI mapping.
23: 
24: module rv_iommu_iotlb_sv39x4 #(
25:     parameter int unsigned IOTLB_ENTRIES = 4
26: )(
27:     input  logic                    clk_i,            // Clock
28:     input  logic                    rst_ni,           // Asynchronous reset active low
29: 
30:     // Flush signals
31:     input  logic                    flush_vma_i,      // IOTINVAL.VMA
32:     input  logic                    flush_gvma_i,     // IOTINVAL.GVMA
33:     input  logic                    flush_av_i,       // ADDR valid
34:     input  logic                    flush_gv_i,       // GSCID valid
35:     input  logic                    flush_pscv_i,     // PSCID valid
36:     input  logic [riscv::GPPNW-1:0] flush_vpn_i,      // VPN to be flushed
37:     input  logic [15:0]             flush_gscid_i,    // GSCID identifier to be flushed (VM identifier)
38:     input  logic [19:0]             flush_pscid_i,    // PSCID identifier to be flushed (address space identifier)
39: 
40:     // Update signals
41:     input  logic                    update_i,
42:     input  logic                    up_1S_2M_i,
43:     input  logic                    up_1S_1G_i,
44:     input  logic                    up_2S_2M_i,
45:     input  logic                    up_2S_1G_i,
46:     input  logic                    up_is_msi_i,
47:     input  logic [riscv::GPPNW-1:0] up_vpn_i,
48:     input  logic [19:0]             up_pscid_i,
49:     input  logic [15:0]             up_gscid_i,
50:     input riscv::pte_t              up_1S_content_i,
51:     input riscv::pte_t              up_2S_content_i,
52: 
53:     // Lookup signals
54:     input  logic                    lookup_i,           // lookup flag
55:     input  logic [riscv::VLEN-1:0]  lu_iova_i,          // IOVA to look for 
56:     input  logic [19:0]             lu_pscid_i,         // PSCID to look for
57:     input  logic [15:0]             lu_gscid_i,         // GSCID to look for
58:     input  logic                    en_1S_i,            // first-stage enabled
59:     input  logic                    en_2S_i,            // second-stage enabled
60:     output logic                    lu_1S_2M_o,         // superpages     
61:     output logic                    lu_1S_1G_o,
62:     output logic                    lu_2S_2M_o,               
63:     output logic                    lu_2S_1G_o,
64:     output logic                    lu_is_msi_o,        // MSI translation
65:     output logic                    lu_hit_o,           // hit flag
66:     output logic                    lu_miss_o,          // miss flag
67:     output riscv::pte_t             lu_1S_content_o,    // first-stage PTE (GPA PPN)
68:     output riscv::pte_t             lu_2S_content_o     // second-stage PTE (SPA PPN)
```

原件 L347–L377：

```text
347:                             tags_n[i].valid = 1'b0;
348:                         end
349:                     end
350:                 endcase
351:             end
352: 
353:             // normal replacement
354:             else if (update_i && replace_en[i] && ((en_1S_i && up_1S_content_i.v) || (en_2S_i && up_2S_content_i.v))) begin
355:                 // update tags
356:                 tags_n[i] = '{
357:                     pscid:      up_pscid_i,
358:                     gscid:      up_gscid_i,
359:                     vpn2:       up_vpn_i[18+riscv::GPPN2:18],
360:                     vpn1:       up_vpn_i[17:9],
361:                     vpn0:       up_vpn_i[8:0],
362:                     en_1S:      en_1S_i,
363:                     en_2S:      en_2S_i,
364:                     is_1S_1G:   up_1S_1G_i,
365:                     is_1S_2M:   up_1S_2M_i,
366:                     is_2S_1G:   up_2S_1G_i,
367:                     is_2S_2M:   up_2S_2M_i,
368:                     is_msi:     up_is_msi_i,
369:                     valid:      1'b1
370:                 };
371:                 // and content as well
372:                 content_n[i].pte_1S = up_1S_content_i;
373:                 content_n[i].pte_2S = up_2S_content_i;
374:             end
375:         end
376:     end
377: 
```

原件 L380–L456：

```text
380:     // -----------------------------------------------
381:     
382:     logic[2*(IOTLB_ENTRIES-1)-1:0] plru_tree_q, plru_tree_n;
383:     always_comb begin : plru_replacement
384:         plru_tree_n = plru_tree_q;
385:         // The PLRU-tree indexing:
386:         // lvl0        0
387:         //            / \
388:         //           /   \
389:         // lvl1     1     2
390:         //         / \   / \
391:         // lvl2   3   4 5   6
392:         //       / \ /\/\  /\
393:         //      ... ... ... ...
394:         // Just predefine which nodes will be set/cleared
395:         // E.g. for a TLB with 8 entries, the for-loop is semantically
396:         // equivalent to the following pseudo-code:
397:         // unique case (1'b1)
398:         // lu_hit[7]: plru_tree_n[0, 2, 6] = {1, 1, 1};
399:         // lu_hit[6]: plru_tree_n[0, 2, 6] = {1, 1, 0};
400:         // lu_hit[5]: plru_tree_n[0, 2, 5] = {1, 0, 1};
401:         // lu_hit[4]: plru_tree_n[0, 2, 5] = {1, 0, 0};
402:         // lu_hit[3]: plru_tree_n[0, 1, 4] = {0, 1, 1};
403:         // lu_hit[2]: plru_tree_n[0, 1, 4] = {0, 1, 0};
404:         // lu_hit[1]: plru_tree_n[0, 1, 3] = {0, 0, 1};
405:         // lu_hit[0]: plru_tree_n[0, 1, 3] = {0, 0, 0};
406:         // default: begin /* No hit */ end
407:         // endcase
408:         for (int unsigned i = 0; i < IOTLB_ENTRIES; i++) begin
409:             automatic int unsigned idx_base, shift, new_index;
410:             // we got a hit so update the pointer as it was least recently used
411:             if (lu_hit[i] && lookup_i) begin
412:                 // Set the nodes to the values we would expect
413:                 for (int unsigned lvl = 0; lvl < $clog2(IOTLB_ENTRIES); lvl++) begin  // 3 for 8 entries
414:                     idx_base = $unsigned((2**lvl)-1);     // 0 for lvl0, 1 for lvl1, 3 for lvl2
415:                     // lvl0 <=> MSB, lvl1 <=> MSB-1, ...
416:                     shift = $clog2(IOTLB_ENTRIES) - lvl;    // 3 for lvl0, 2 for lvl1, 1 for lvl2
417:                     // to circumvent the 32 bit integer arithmetic assignment
418:                     new_index =  ~((i >> (shift-1)) & 32'b1);
419:                     plru_tree_n[idx_base + (i >> shift)] = new_index[0];
420:                 end
421:             end
422:         end
423:         // Decode tree to write enable signals
424:         // Next for-loop basically creates the following logic for e.g. an 8 entry
425:         // TLB (note: pseudo-code obviously):
426:         // replace_en[7] = &plru_tree_q[ 6, 2, 0]; //plru_tree_q[0,2,6]=={1,1,1}
427:         // replace_en[6] = &plru_tree_q[~6, 2, 0]; //plru_tree_q[0,2,6]=={1,1,0}
428:         // replace_en[5] = &plru_tree_q[ 5,~2, 0]; //plru_tree_q[0,2,5]=={1,0,1}
429:         // replace_en[4] = &plru_tree_q[~5,~2, 0]; //plru_tree_q[0,2,5]=={1,0,0}
430:         // replace_en[3] = &plru_tree_q[ 4, 1,~0]; //plru_tree_q[0,1,4]=={0,1,1}
431:         // replace_en[2] = &plru_tree_q[~4, 1,~0]; //plru_tree_q[0,1,4]=={0,1,0}
432:         // replace_en[1] = &plru_tree_q[ 3,~1,~0]; //plru_tree_q[0,1,3]=={0,0,1}
433:         // replace_en[0] = &plru_tree_q[~3,~1,~0]; //plru_tree_q[0,1,3]=={0,0,0}
434:         // For each entry traverse the tree. If every tree-node matches,
435:         // the corresponding bit of the entry's index, this is
436:         // the next entry to replace.
437:         for (int unsigned i = 0; i < IOTLB_ENTRIES; i += 1) begin
438:             automatic logic en;
439:             automatic int unsigned idx_base, shift, new_index;
440:             en = 1'b1;
441:             for (int unsigned lvl = 0; lvl < $clog2(IOTLB_ENTRIES); lvl++) begin
442:                 idx_base = $unsigned((2**lvl)-1);
443:                 // lvl0 <=> MSB, lvl1 <=> MSB-1, ...
444:                 shift = $clog2(IOTLB_ENTRIES) - lvl;
445: 
446:                 // en &= plru_tree_q[idx_base + (i>>shift)] == ((i >> (shift-1)) & 1'b1);
447:                 new_index =  (i >> (shift-1)) & 32'b1;
448:                 if (new_index[0]) begin
449:                     en &= plru_tree_q[idx_base + (i>>shift)];
450:                 end else begin
451:                     en &= ~plru_tree_q[idx_base + (i>>shift)];
452:                 end
453:             end
454:             replace_en[i] = en;
455:         end
456:     end
```

