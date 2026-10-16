# Evidence: current production TL / nonblock-hit / IOTLB / DDTC / banked-controller behavior

Scope note: below is static source evidence only, from the files supplied. No build/test/run was performed; nothing here is a test claim. Line numbers refer to the supplied numbered excerpts. Where a needed file was not supplied I say so explicitly and do not infer.

## Navigation summary

| # | Question | Short answer | Primary evidence |
|---|---|---|---|
| 1 | Active walks / bypass inflight+output slots / duplicate merge | 1 main TL pipeline (one FSM), 1 walker per table (DDTW/PDTW/PTW/MSIPTW), plus 1 single-slot fast hit path; no dedup/coalesce | `rv_iommu_tl_wrap.sv:143-152,1003`; `rv_iommu_nonblock_hit.sv:36-37,47-50`; `rv_iommu_banked_ctl.sv:366-392` |
| 2 | IOTLB/DDTC tags, context eligibility, cross-DID sharing | IOTLB key = {VPN, PSCID, GSCID, stages, valid} (no DID/PID); DDTC key = DID only; PDTC key = {DID,PID} | `rv_iommu_iotlb.sv:59-68,106-137`; `rv_iommu_ddtc.sv:48-51,75`; `rv_iommu_tl_wrap.sv:567-572` |
| 3 | Response v/r paths, per-bank routing, blocked-recipient hold | Single shared TL response port; banked_ctl routes by `.bank` and returns the matching bank's ready; a blocked bank de-asserting its ready holds the TL COMPLETE state | `rv_iommu_tl_wrap.sv:1545-1568,346-353`; `rv_iommu_banked_ctl.sv:400-408` |
| 4 | Fanout / key / multiple-waiter metadata | None. `bank` is the only lifecycle tag; no waiter list, no key, no broadcast | `rv_iommu_banked_ctl.sv:42-46,366-377`; `rv_iommu_tl_wrap.sv:462-478` |
| 5 | Invalidate/fault tagging + unsupported cases | Inval keyed by GSCID/PSCID/AV/GV (IOTLB) or DID/PID (DDTC/PDTC); fault tagged with did/pid/priv/ttype; several hard `$fatal` and fall-through gaps | `rv_iommu_pkg.sv:307-313,341-351`; `rv_iommu_iotlb.sv:198-315`; `rv_iommu_tl_wrap.sv:436-445,1515-1542`; `rv_iommu_banked_ctl.sv:143-148` |

---

## (1) Number of active walks, read-hit bypass inflight/output slots, duplicate merge

### Main translation pipeline = exactly one
The TL wrapper is built around a single FSM. Only one request is in translation at a time.

```systemverilog
// rv_iommu_tl_wrap.sv:143-152
typedef enum logic [2:0] {
    IDLE=3'b000, DDT=3'b001, PDT=3'b010, PT=3'b011, MSI=3'b100,
    ERROR=3'b101, COMPLETE=3'b110
} tl_wrap_state_t;
tl_wrap_state_t state_q, state_n;
```
```systemverilog
// rv_iommu_tl_wrap.sv:1003
unique case (state_q)
```
The whole `translation_ctl_comb` block (`rv_iommu_tl_wrap.sv:950-1574`) operates on the single `req_data` and single `state_q`.

### One walker per table, launched only from the single FSM
- DDTW: `rv_iommu_ddtw` instance `rv_iommu_tl_wrap.sv:525-560`; init `assign ddtw_init = ((state_q == DDT) | ats_ddtc_valid_i) & ~ddtc_lu_hit;` (`:360`).
- PDTW (only if `RVIOMMUCfg.InclPC`): instance `:604-640`, in generate `gen_pdt_modules` `:562-564`; init `assign pdtw_init = pdtc_lu & ~pdtc_lu_hit;` (`:575`) with `pdtc_lu = (state_q == PDT)` (`:570`).
- PTW: instance `:759-804`; init `assign ptw_init = (state_q == PT) & ~iotlb_lu_hit;` (`:375`).
- MSIPTW (only if MSI enabled): instance `:846-878`; init `assign msi_ptw_init = (state_q == MSI) & ~iotlb_lu_hit & ~mrifc_lu_hit;` (`:830`).

Because each init is gated by `state_q`, only the walker for the current state can be active. `active_o` is explicitly unconnected for DDTW/PDTW (`:539`, `:616`); only PTW's active is captured as `e3_ptw_active` (`:771`).

DDTC lookup can additionally be driven by the ATS page-request port independent of the FSM state:
```systemverilog
// rv_iommu_tl_wrap.sv:249-251
assign ddtc_lu = (state_q == DDT) | ats_ddtc_valid_i | n_ddtc_lu;
assign ddtc_lu_did = n_ddtc_lu ? n_ddtc_did :
                     ats_ddtc_valid_i ? ats_ddtc_did_i : req_data.did;
```
The DDTC and IOTLB are single lookup ports time-shared across FSM, ATS, and fast path:
```systemverilog
// rv_iommu_tl_wrap.sv:271-277
assign iotlb_lu       = (state_q == PT) | (state_q == MSI) | n_iotlb_lu;
assign iotlb_lu_vpn   = n_iotlb_lu ? n_iotlb_vpn : req_data.iova[GPLEN39-1:12];
assign iotlb_lu_pscid = n_iotlb_lu ? n_iotlb_pscid : pscid_q;
...
assign iotlb_lu_hit = cache_iotlb_hit && !n_iotlb_lu;
```
```systemverilog
// rv_iommu_nonblock_hit.sv:4
// The DDTC and IOTLB ports below are time-shared with the original walker.
```

### Read-hit bypass (nonblock hit path): one request slot, one response slot
The bypass is a single small FSM, not a banked set of registers:
```systemverilog
// rv_iommu_nonblock_hit.sv:36-41
typedef enum logic [2:0] {IDLE, DC, LEAF, OUT, REJECT} state_t;
state_t state_q;
req_t req_q;
dc_t dc_q;
ddtp_t ddtp_q;
resp_t resp_q;
```
Only one candidate is tracked; acceptance requires the probe to be identical to the latched request:
```systemverilog
// rv_iommu_nonblock_hit.sv:100-107
assign accept_o = state_q == LEAF && enable_i &&
    candidate_valid_i && candidate_i == req_q &&
    !invalidate_i && ddtp_i == ddtp_q && dc_ok &&
    ddtc_dc_i == dc_q && leaf_ok;
assign ddtc_touch_o = accept_o;
assign iotlb_touch_o = accept_o;
```
There is exactly one output slot:
```systemverilog
// rv_iommu_nonblock_hit.sv:108-109
assign response_o = resp_q;
assign response_valid_o = state_q == OUT;
```
```systemverilog
// rv_iommu_nonblock_hit.sv:171
OUT: if (response_ready_i) state_q <= IDLE;
```
The "two-bank" characterization comes from the enable window: the bypass only opens against the *other* bank while the main pipeline is in `PT` and the PTW is active:
```systemverilog
// rv_iommu_tl_wrap.sv:303-306
assign n_window = state_q == PT && e3_ptw_active && en_1S_q &&
                  !en_2S_q && !ddt_imp_trans_q && !pdt_imp_trans_q &&
                  !req_data.is_debug && trans_req_data_i.bank != req_data.bank &&
                  !ats_ddtc_valid_i && !debug_req_valid_i && !ats_trans_req_valid_i;
```
```systemverilog
// rv_iommu_tl_wrap.sv:282-286
// Two lifecycle tags (one outstanding request per bank), one response port.
// A rejected probe never handshakes: the original arbiter later accepts it.
assign normal_gate = !NonblockEnable ||
                     (state_q == IDLE && !req_valid && !n_intercept);
assign trans_req_ready_o = n_intercept ? n_accept : (normal_trans_ready && normal_gate);
```
The fast response and the normal ("slow") response share one RR arbiter:
```systemverilog
// rv_iommu_tl_wrap.sv:346-353
stream_arbiter #(.DATA_T(trans_resp_data_t), .N_INP(2), .ARBITER("rr"))
    i_nonblock_response_arbiter (
        .clk_i, .rst_ni, .inp_data_i({fast_resp_data, slow_resp_data}),
        .inp_valid_i({fast_resp_valid, slow_resp_valid}),
        .inp_ready_o({fast_resp_ready, slow_resp_ready}),
        .oup_data_o(trans_resp_data_o), .oup_valid_o(trans_resp_valid_o),
        .oup_ready_i(trans_resp_ready_i)
    );
```

### Can duplicate translation requests be merged today? No.
The banked controller's "merge" is round-robin arbitration, not deduplication; it presents one bank request at a time and overrides only `.bank`, preserving every other field:
```systemverilog
// rv_iommu_banked_ctl.sv:366-377
// Translation request merge (round-robin, stable while valid)
//   The bank-local request is copied and ONLY `.bank` is overwritten, so
//   every other header field (iova/did/pid/ttype/...) is preserved bit-exact.
trans_req_data_t [1:0] bank_req_data;
always_comb begin : bank_req_tag
    bank_req_data[0]       = bank_trans_req_data[0];
    bank_req_data[0].bank  = 1'b0;
    bank_req_data[1]       = bank_trans_req_data[1];
    bank_req_data[1].bank  = 1'b1;
end
```
```systemverilog
// rv_iommu_banked_ctl.sv:379-392
stream_arbiter #(.DATA_T(trans_req_data_t), .N_INP(2), .ARBITER("rr"))
    i_trans_req_arb ( ... .oup_data_o(trans_req_data_o), ... );
```
There is no table keyed by {iova,did,pid,...}, no MSHR-style pending list, and the single TL pipeline + single IOTLB lookup port serializes any genuine duplicate. Two identical requests arriving on the two banks are both arbitrated in, looked up twice, and answered twice.

---

## (2) IOTLB/DDTC tags, context eligibility, cross-DID sharing, per-request differences

### IOTLB tag and content
```systemverilog
// rv_iommu_iotlb.sv:59-68
struct packed {
    logic [10:0]        vpn2;
    logic [8:0]         vpn1;
    logic [8:0]         vpn0;
    rv_iommu::pscid_t   pscid;
    rv_iommu::gscid_t   gscid;
    logic               en_1S;
    logic               en_2S;
    logic               valid;
} [RVIOMMUCfg.NumIotlbEntries-1:0] tags_q, tags_n;
```
Tag has **no device_id and no process_id**. Widths: `pscid` = 20 bits, `gscid` = 16 bits (`rv_iommu_pkg.sv:85-86`).

Match logic:
```systemverilog
// rv_iommu_iotlb.sv:106,109,126,129-136
match_pscid[i] = (((lu_pscid_i == tags_q[i].pscid) || content_q[i].content_1S.g) && en_1S_i) || !en_1S_i;
match_gscid[i] = (lu_gscid_i == tags_q[i].gscid && en_2S_i) || !en_2S_i;
match_stage[i] = (tags_q[i].en_2S == en_2S_i) && (tags_q[i].en_1S == en_1S_i);
if (tags_q[i].valid && match_pscid[i] && match_gscid[i] && match_stage[i] && (vpn2 == tags_q[i].vpn2)) begin
    if (is_1G[i] || ((vpn1 == tags_q[i].vpn1) && (is_2M[i] || vpn0 == tags_q[i].vpn0))) begin
        lu_content_o = content_q[i]; lu_hit_o = 1'b1; lu_hit[i] = 1'b1;
    end
end
```
Two notable eligibility rules: a **global first-stage entry (`content_1S.g == 1`) matches any PSCID** when `en_1S` is set; when a stage is disabled its match terms collapse to `1`.

Content carries per-stage permissions but not request attributes:
```systemverilog
// rv_iommu_pkg.sv:316-330
typedef struct packed { logic is_1G; logic is_2M; ppn_t ppn; logic g,u,x,w,r,v; } iotlb_stage_content_t;
typedef struct packed { iotlb_stage_content_t content_1S; iotlb_stage_content_t content_2S; } iotlb_content_t;
```

### DDTC and PDTC tags
```systemverilog
// rv_iommu_ddtc.sv:48-51
struct packed { rv_iommu::device_id_t device_id; logic valid; } [RVIOMMUCfg.NumDdtcEntries-1:0] tags_q, tags_n;
```
```systemverilog
// rv_iommu_ddtc.sv:75
if (tags_q[i].valid && tags_q[i].device_id == lu_did_i) begin
```
DDTC is strictly keyed by DID; content is the whole Device Context (`dc_t`). PDTC (seen only through the wrapper ports) is keyed by DID+PID:
```systemverilog
// rv_iommu_tl_wrap.sv:570-572
assign pdtc_lu      = (state_q == PDT);
assign pdtc_lu_did  = req_data.did;
assign pdtc_lu_pid  = req_data.pid;
```
Content is `pc_t` (`rv_iommu_pkg.sv:238-241`).

### Where PSCID/GSCID come from and when they are context
`pscid_q/gscid_q` are registered per translation (`rv_iommu_tl_wrap.sv:223-224`, written `:1623-1624`). They are set from the DC or PC depending on path:
- 2nd-stage-only (translated/T2GPA): `gscid_n = ddtc_dc.iohgatp.gscid;` (`:1137`).
- No process context: `gscid_n = ddtc_dc.iohgatp.gscid; pscid_n = ddtc_dc.ta.pscid;` (`:1160-1161`).
- With process context: `gscid_n = dc_q.iohgatp.gscid; pscid_n = pdtc_lu_content.ta.pscid;` (`:1257-1258`).

### Cross-DID sharing: what is safe and what differs per request
- **Can two DIDs share one IOTLB entry today? Yes, if they resolve to the same {VPN, PSCID, GSCID, stage config}.** The IOTLB key contains no DID/PID, and global S1 entries match any PSCID (`rv_iommu_iotlb.sv:106`). This is the normal PSCID/GSCID-sharing contract.
- **DDTC/PDTC are not shared across DIDs.** DDTC is one DC per DID (`rv_iommu_ddtc.sv:75`); the DC (including `ta.pscid`) is per-DID.
- **Per-request attributes that are *not* part of the IOTLB tag and are re-evaluated each translation** (so they can differ between two requests hitting the same entry):
  - privilege / user-vs-supervisor and SUM: `!req_data.priv && !iotlb_lu_content.content_1S.u` and `req_data.priv && ...content_1S.u && (!pc_q.ta.sum || is_rx)` (`rv_iommu_tl_wrap.sv:1374-1378`); `sum_q` comes from `pc_q.ta.sum` (`:1364`).
  - operation type: `is_store`/`is_rx` gating W/X (`:1392-1404`, `:415-419`).
  - `is_debug` (debug response path), `t2gpa` (`:1365`), `pid_valid`/`dpe` handling (`:411-413`, `:1120-1122`).
- **The bypass path only passes a very restricted context** (exactly one translation context, 4KiB S1-only):
```systemverilog
// rv_iommu_nonblock_hit.sv:84-99
assign dc_ok = msi_safe && ddtc_hit_i && ddtc_dc_i.tc.v &&
    !ddtc_dc_i.tc.pdtv && !ddtc_dc_i.tc.sxl &&
    !ddtc_dc_i.tc.t2gpa && !ddtc_dc_i.tc.en_ats &&
    !ddtc_dc_i.tc.en_pri && !req_q.pid_valid && !req_q.priv &&
    !req_q.is_debug && (is_write || is_read) && canonical &&
    ddtc_dc_i.fsc.mode == ModeSv39 &&
    ddtc_dc_i.iohgatp.mode == ModeBare &&
    ((ddtp_i.iommu_mode.q == 2 && req_q.did[23:6] == 0) ||
     (ddtp_i.iommu_mode.q == 3 && req_q.did[23:15] == 0) ||
     ddtp_i.iommu_mode.q == 4);
assign leaf_ok = msi_leaf_safe && iotlb_hit_i && iotlb_content_i.content_1S.v &&
    iotlb_content_i.content_1S.u &&
    !iotlb_content_i.content_1S.is_1G &&
    !iotlb_content_i.content_1S.is_2M &&
    (is_write ? iotlb_content_i.content_1S.w : iotlb_content_i.content_1S.r);
```

---

## (3) Response valid/ready paths, per-bank routing, blocked-recipient hold

### TL response generation and hold
Normal responses are produced in `COMPLETE` and held until ready; ERROR holds for fault handshake:
```systemverilog
// rv_iommu_tl_wrap.sv:1545-1568
COMPLETE: begin
    if (req_data.is_debug) begin
        debug_resp_valid_o  = 1'b1;
        if (debug_resp_ready_i) begin state_n = IDLE; end
    end
    else if(req_data.ttype == rv_iommu::PCIE_ATS_TRANS_REQ) begin
        ats_trans_resp_valid_o  = 1'b1;
        if (ats_trans_resp_ready_i) begin state_n = IDLE; end
    end
    else begin
        slow_resp_valid  = 1'b1;
        if (slow_resp_ready) begin state_n = IDLE; end
    end
end
```
```systemverilog
// rv_iommu_tl_wrap.sv:1515-1542 (ERROR)
error_n = 1'b1;
fault_valid_o = 1'b1;
if (fault_ready_i) begin state_n = COMPLETE; end
```
`slow_resp_ready` is the RR arbiter's granted ready, whose `oup_ready_i` is `trans_resp_ready_i`:
```systemverilog
// rv_iommu_tl_wrap.sv:348-352
.inp_valid_i({fast_resp_valid, slow_resp_valid}),
.inp_ready_o({fast_resp_ready, slow_resp_ready}),
.oup_data_o(trans_resp_data_o), .oup_valid_o(trans_resp_valid_o),
.oup_ready_i(trans_resp_ready_i)
```
Also, new requests cannot be latched while a response is outstanding:
```systemverilog
// rv_iommu_tl_wrap.sv:174-176
assign req_ready = (debug_resp_valid_o & debug_resp_ready_i) |
                   (slow_resp_valid & slow_resp_ready) |
                   (ats_trans_resp_valid_o & ats_trans_resp_ready_i) ;
```

### Per-bank routing: shared bus, routed by `.bank`
```systemverilog
// rv_iommu_banked_ctl.sv:400-408
always_comb begin : bank_resp_route
    bank_trans_resp_data[0] = trans_resp_data_i;
    bank_trans_resp_data[1] = trans_resp_data_i;
    bank_trans_resp_valid[0] = trans_resp_valid_i && (trans_resp_data_i.bank == 1'b0);
    bank_trans_resp_valid[1] = trans_resp_valid_i && (trans_resp_data_i.bank == 1'b1);
end
assign trans_resp_ready_o = trans_resp_data_i.bank ? bank_trans_resp_ready[1]
                                                   : bank_trans_resp_ready[0];
```
Both banks receive the same payload; only the tagged bank sees `valid`. The ready returned upstream is the **matching bank's** ready. At most one translation per bank is in flight, so `bank` is an unambiguous tag (`rv_iommu_banked_ctl.sv:42-46`). There is no separate per-bank response channel to the TL — one shared bus and one shared TL response port.

### Can a blocked recipient hold the TL/PTW slot? Structurally yes.
Chain: if `bank_trans_resp_ready[b]` is low for the tagged bank → `trans_resp_ready_o` is low (`banked_ctl:407-408`) → the RR arbiter does not grant slow → `slow_resp_ready` stays low → the TL stays in `COMPLETE` (`tl_wrap:1564`) → `trans_req_ready_o`/`req_ready` do not accept a new request (`tl_wrap:174-176,286`). The single main pipeline slot and thus the next bank's forward progress are held by the blocked return. The same hold exists for the fault path (`tl_wrap:1531-1539` and `:1538`) and for the fast path's `OUT` slot (`nonblock_hit.sv:171`).

**Limitation:** whether `bank_trans_resp_ready[b]` can actually de-assert under completion backpressure is determined inside `rv_iommu_trans_ctl_wrap.sv`, which was **not supplied**. The structural blocking path above is citable from the supplied files; the trigger condition is not.

---

## (4) Outcome fanout / key / multiple-waiter metadata today

None exists. The only lifecycle tag is the single-bit `.bank`:
```systemverilog
// rv_iommu_banked_ctl.sv:42-46
//   * The downstream translation engine must echo the `bank` field it received
//     in `trans_req_data_o` back in `trans_resp_data_i`.  At most one
//     translation is in flight per bank, which makes `bank` an unambiguous
//     lifecycle tag and guarantees the full bank-local header ...
```
```systemverilog
// rv_iommu_tl_wrap.sv:462-478 (slow response fields — no waiter/key fields)
assign slow_resp_data.bank = req_data.bank;
assign slow_resp_data.error = error_q;
assign slow_resp_data.ignore = ignore_q;
assign slow_resp_data.spaddr = spaddr_q;
assign slow_resp_data.is_mrif = is_mrif_q;
assign slow_resp_data.mrif_data = mrif_q;
assign slow_resp_data.range = range_q;
assign slow_resp_data.bypass = bypass_q;
assign slow_resp_data.fault_code = error_code_q;
assign slow_resp_data.w = w_q; ... .r = r_q; ... .x = x_q; ...
assign slow_resp_data.sum = sum_q; ... .g = g_q; ... .u = u_q; ...
assign slow_resp_data.t2gpa = t2gpa_q;
```
The observer structures are ABI taps only and do not participate in arbitration or carry any waiter list:
```systemverilog
// rv_iommu_pkg.sv:24
// Bounded FPGA experiment taps. No observer signal participates in arbitration.
```
`research_t` (`rv_iommu_pkg.sv:52-67`) and `e3_tl_t` (`rv_iommu_pkg.sv:42-50`) carry per-lookup fields (did/iova/pscid/gscid/stage), not fanout. They are driven in `rv_iommu_tl_wrap.sv:712-740` and `rv_iommu_iotlb.sv:429-461`.

**Conclusion:** one request → one response. No broadcast, no keyed fanout, no multiple-waiter accounting exists in the supplied production RTL.

---

## (5) Invalidate/fault tagging and precise unsupported cases

### Invalidate tagging
```systemverilog
// rv_iommu_pkg.sv:341-351 (IOTLB invalidate)
typedef struct packed {
    logic               inval_vma;
    logic               inval_gvma;
    logic               av;
    logic               gv;
    logic               pscv;
    logic [GPPNW39-1:0] vpn;
    gscid_t             gscid;
    pscid_t             pscid;
} iotlb_inval_t;
```
Note: **no DID and no PID** in the IOTLB invalidate record — IOTLB flush is keyed by GSCID/PSCID/AV/GV/VPN only.
```systemverilog
// rv_iommu_pkg.sv:307-313 (DDTC/PDTC invalidate)
typedef struct packed {
    logic           inval_ddtc;
    logic           inval_pdtc;
    logic           dv;
    device_id_t     did;
    process_id_t    pid;
} xdtc_inval_t;
```
IOTLB flush implements all `{gv,av,pscv}` and `{gv,av}` combinations (`rv_iommu_iotlb.sv:198-315`). DDTC invalidation is all-or-by-DID:
```systemverilog
// rv_iommu_ddtc.sv:101-111
if (inval_i.inval_ddtc) begin
    if (!inval_i.dv) begin tags_n[i].valid = 1'b0; end       // DV=0: all
    else if (tags_q[i].device_id == inval_i.did) begin        // DV=1: by DID
        tags_n[i].valid = 1'b0;
    end
end
```
The bypass treats any invalidate as a kill:
```systemverilog
// rv_iommu_tl_wrap.sv:320-321
.invalidate_i(iotinval_i.inval_vma || iotinval_i.inval_gvma ||
              iodirinval_i.inval_ddtc || iodirinval_i.inval_pdtc),
```

### Fault tagging
```systemverilog
// rv_iommu_tl_wrap.sv:436-445
assign fault_data_o.trans_type    = req_data.ttype;
assign fault_data_o.cause_code    = error_code_q;
assign fault_data_o.iova          = req_data.iova;
assign fault_data_o.gpaddr        = error_gpaddr_q;
assign fault_data_o.did           = req_data.did;
assign fault_data_o.pv            = req_data.pid_valid;
assign fault_data_o.pid           = req_data.pid;
assign fault_data_o.is_supervisor = req_data.priv;
assign fault_data_o.is_guest_pf   = error_2S_q;
assign fault_data_o.is_implicit   = error_2S_int_q;
```
Fault port is single (`fault_valid_o`/`fault_ready_i`, `:121-123`); no per-bank tag on the fault record. The record struct in the package is `fq_record_t` (`rv_iommu_pkg.sv:547-556`). ATS gets a special case — page faults and `MSI_PTE_INVALID`/`PDT_ENTRY_INVALID` generate a fault, other causes complete with error only (`rv_iommu_tl_wrap.sv:1516-1534`).

### Precise unsupported / fall-through cases visible in supplied files
- No process-directory support ⇒ PDT state errors:
```systemverilog
// rv_iommu_tl_wrap.sv:1273-1275
else begin : no_pdt_state
    state_n = ERROR;
    error_code_n = rv_iommu::TRANS_TYPE_DISALLOWED;
end
```
- MSI translation disabled ⇒ MSI state errors (`rv_iommu_tl_wrap.sv:1509-1511`); MSI MRIF + debug ⇒ ERROR (`:1502-1504`).
- Banked controller hard-excludes MRIF and ATS:
```systemverilog
// rv_iommu_banked_ctl.sv:143-148
if (RVIOMMUCfg.MSITrans == rv_iommu_cfg::MSI_BT_MRIF) begin
    $fatal(1, "rv_iommu_banked_ctl: MRIF is unsupported");
end
if (RVIOMMUCfg.InclATS) begin
    $fatal(1, "rv_iommu_banked_ctl: InclATS must be disabled");
end
```
- Policy composition hard requirement:
```systemverilog
// rv_iommu_tl_wrap.sv:137-138
if (PolicyEnable && (!NonblockEnable || RVIOMMUCfg.NumIotlbEntries != 16))
    $fatal(1, "Composition requires nonblocking-capable 16-entry IOTLB");
```
- Bypass fall-through set (all excluded cases are served by the normal/fault path, never handshake here): see `rv_iommu_nonblock_hit.sv:84-99` quoted in (2). It rejects superpages, two-stage, PD, SXL, T2GPA, ATS/PRI, priv, pid_valid, debug, non-canonical, and MSI-typed ranges (guarded at `:62-83`).
- Legacy E3 counters are incompatible with two owners; only `.quiet` is meaningful:
```systemverilog
// rv_iommu_banked_ctl.sv:622-625
always_comb begin : legacy_e3
    e3_o       = '0;
    e3_o.quiet = !in_flight_o;
end
```

---

## Limitations / not established

- `trans_req_data_t` / `trans_resp_data_t` definitions were **not supplied**, so the exact field list and the declared width of `.bank` are not verified here. The supplied code uses `.bank` as a single bit (`banked_ctl:374-376,403-404` compare to `1'b0/1'b1`).
- `rv_iommu_trans_ctl_wrap.sv` was **not supplied**, so the condition under which `bank_trans_resp_ready[b]` (and thus `trans_resp_ready_o`) de-asserts is unknown. The "blocked recipient holds the TL slot" claim is structural from `tl_wrap`/`banked_ctl` only.
- `rv_iommu_pdtc.sv`, `rv_iommu_ptw.sv`, `rv_iommu_ddtw.sv`, `rv_iommu_pdtw.sv`, `rv_iommu_msiptw.sv`, `rv_iommu_mrifc.sv` were **not supplied**; PDTC tag asserted only from its wrapper ports.
- No test/build/run evidence. No claim that any of the above has been simulated.
- This is evidence only; no §44 mechanism verdict is offered.

## Next exact sources to pull (in priority order)
1. `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv` — translation-response ready/`trans_resp_ready_o` de-assert conditions, AW-before-AR gating, completion backpressure, and whether a single bank can stall the shared response bus.
2. The typedef source for `trans_req_data_t` / `trans_resp_data_t` (likely a `riscv-iommu/rtl/*typedef*.svh` or `rv_iommu_typedef.svh`) — verify `.bank` width and the complete response field set (confirm absence of any key/waiter field).
3. `rv_iommu_pdtc.sv` — confirm PDTC tag = {DID,PID,valid} and content = `pc_t`.
4. `rv_iommu_ptw.sv` — `update_o` (`ptw_iotlb_update`) timing vs `iotlb_update` mux (`tl_wrap:278-280`) and whether a PTW update can be dropped when the single IOTLB port is busy.
5. Top-level config / `rv_iommu_cfg` source — confirm `NonblockEnable`, `PolicyEnable`, `NumIotlbEntries`, `InclPC`, `MSITrans` values used in the production instance.

## Tagged hypothesis-only note (NOT evidence, NOT a proposed change here)
Any future cross-bank outcome reuse would have to supply metadata that does not exist today: a stable key (e.g., {VPN,PSCID,GSCID,stages} already present in the IOTLB tag, `iotlb.sv:59-68`), a waiter/fanout list (absent), and a response router that can return the same result to more than one `.bank` (today a single tagged `bank`, `banked_ctl:403-408`). This is recorded only to mark the gap; it is not a mechanism verdict and not derived from the supplied code as a capability.

If you want, tell me which of the five next sources above you can paste and I will turn this into precise line-cited evidence for that layer.