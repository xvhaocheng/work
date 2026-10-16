# Read-only evidence report — formal01 item `explore-b1-g1000-a8-mpn` (F:15, DID 11)

Scope note: this is evidence organization only. I did **not** run anything, did not modify code, and do **not** assert a root cause. Every claim below is either a raw log/record value or a `file:line` code citation. Anything inferred is labelled **assumption**.

---

## 0. What the artifacts positively establish (one paragraph)

The failed trial is composition sequence **54** in `out/composition-dsfix-20261005/mac-records/results/composition-dsfix-20261005-formal01/cases/explore-b1-g1000-a8-mpn/record.json` (`argv`: `--mode 3 --a-pages 8 --v-gap 1000 --transfers 4096 --warmup 256`, record.json:2–17), returning `status = -5` / `errno = 5` / `success = false` (record.json:19–20,65,281,296). The log ends with 10 hardware fault records `F: 15 | DID: 11` (dmesg.failure.txt:706–735). The immediately preceding sequence in the *same* run is **53**, which matches `cases/explore-b1-g1000-a1-mn/record.json` exactly (`p4m.observation[0]=8419261`, `sequence=53`, record.json a1-mn:91,128), and that one is `success=true`. A same-shape (a_pages=8) passing case exists in a different run dir: `composition-dsfix-20261005-n01/cases/explore-b1-g1000-a8-mn/record.json`. The single positive “why” that can be read out of the driver is that `observer_status = 0x4000000004` violates the driver’s `status == 4` requirement, so the observer is declared **invalid** and `idma_run_session2` returns `-EIO` before data verification runs.

---

## 1. F:15 / DID 11 / iotval — exact list, and deltas vs the passing cases

### 1.1 Fault records (verbatim, dmesg.failure.txt:706–735)

All records are `F: 15 | DID: 11`, `iotval2: 0`, 4 KiB aligned, all inside the 2 MiB-aligned region `0x3fffe00000` + 0x{b,c,d,e}000:

| # | lines | iotval | iotval2 |
|---|-------|--------|---------|
| 1 | 706–708 | `0x3fffe0e000` | 0 |
| 2 | 709–711 | `0x3fffe0d000` | 0 |
| 3 | 712–714 | `0x3fffe0c000` | 0 |
| 4 | 715–717 | `0x3fffe0b000` | 0 |
| 5 | 718–720 | `0x3fffe0d000` | 0 |
| 6 | 721–723 | `0x3fffe0c000` | 0 |
| 7 | 724–726 | `0x3fffe0b000` | 0 |
| 8 | 727–729 | `0x3fffe0b000` | 0 |
| 9 | 730–732 | `0x3fffe0c000` | 0 |
| 10 | 733–735 | `0x3fffe0b000` | 0 |

Distinct IOVAs: `{0x3fffe0b000, 0x3fffe0c000, 0x3fffe0d000, 0x3fffe0e000}`. In the RTL the fault payload is built in `rv_iommu_tl_wrap.sv:436–445` (`cause_code = error_code_q`, `iova = req_data.iova`, `did = req_data.did`). **Not provided:** the module that formats `F:/DID:/iotval:` and hence the exact meaning of `iotval` (iova vs gpaddr) and `iotval2` here.

Snippet (dmesg.failure.txt:705–711):
```
idma_batch_start version=1 seq=54 method=irq_pair v_did=10 a_did=11 ... a_id=7915 v_id=10039 ... a_to_v_ns=639240 error=-5
riscv,iommu 50010000.iommu: F: 15 | DID: 11
riscv,iommu 50010000.iommu: iotval: 3fffe0e000
riscv,iommu 50010000.iommu: iotval2: 0
riscv,iommu 50010000.iommu: F: 15 | DID: 11
riscv,iommu 50010000.iommu: iotval: 3fffe0d000
riscv,iommu 50010000.iommu: iotval2: 0
```

Timing: the 10 fault prints (start 703.57 s) occur **after** the failed composition already froze/returned (`idma_batch_obs seq=54` at 703.36 s line 686; `idma_batch_start seq=54` at 703.55 s line 705). So the fault-queue drain is after the observation close; this is not a per-cycle ordering proof.

**Assumption (needs record support, not present):** the faulting IOVAs `0x3fffe0{b..e}000` are storable addresses of the aggressor (DID 11). Deriving this requires the composition mapping (src_iova/dst_iova), which the composition record does **not** carry (only the E3-pair path fills `pair->mapping[]`, idma.c:2454–2463). “STORE” implies the DMA dst side (`is_store` = write beat).

### 1.2 Failing vs passing observation/window data

`p4m.observation[i]` is `idma_observer_indexed(o, i)` (idma.c:1400–1401); the same words are logged as `idma_batch_obs ... part=axi_v/axi_a` (idma.c:1139–1150). Field order: 0 cycles, 1 h, 2 pending, 3 gap, 4 hmax, 6 gapmax, 7 issued, 8 done, 9 polled, 10 neither, 11 v_only, 12 a_only, 13 both, 14/15/16/17/18/19 = V ar/aw/rlast/b/ar_wait/aw_wait, 20..25 = A ar/aw/rlast/b/ar_wait/aw_wait, 26..29 = shared ar/aw/ar_wait/aw_wait, 30/32 start, 31/33 end, 34 read_delay.

| word | a1-mn (formal01 seq53, PASS) | a8-mn (n01 seq1, PASS) | **a8-mpn (formal01 seq54, FAIL)** |
|---|---|---|---|
| observer_status | 4 | 4 | **0x4000000004** |
| 0 cycles | 8419261 | 9454156 | 9474077 |
| 1 h | 4320166 | 5355061 | 5374982 |
| 3 gap | 4099095 | 4099095 | 4099095 |
| 4 hmax | 1361 | 2792 | 2623 |
| 7 issued / 8 done | 4096 / 4096 | 4096 / 4096 | 4096 / 4096 |
| 10 neither | 8190 | 2728 | **1493** |
| 11 v_only | 8192 | 8192 | **8189** |
| 12 a_only | 4095001 | 4100463 | **4101698** |
| 13 both | 4307878 | 5342773 | **5362697** |
| 14–17 V ar/aw/rlast/b | 8192 each | 8192 each | 8192 each |
| 18/19 V ar_wait/aw_wait | 0/0 | 0/0 | **1/1** |
| 20–23 A ar/aw/rlast/b | 16382 each | 10920 each | **9682 each** |
| 24/25 A ar_wait/aw_wait | 0/0 | 0/0 | **3/3** |
| 26/27 shared ar/aw | 24574 | 19112 | **17874** |
| 29 shared_aw_wait | 4126942 | 5104844 | 5131406 |
| 34 done_to_read | 1740559 | 885 | 770323 |

Cross-checks (`idma_observation_finish` inv. (idma.c:1079–1080)) hold in all three, so the counters are self-consistent:
- seq53: `8190+8192+4095001+4307878 = 8419261`; seq54: `1493+8189+4101698+5362697 = 9474077`.
- shared_ar = V_ar + A_ar in every case (24574=8192+16382; 17874=8192+9682).

**Same-run delta (a1-mn seq53 → a8-mpn seq54), read/write AXI channels only:**
- V lane: ar/aw/rlast/b **unchanged 8192**; V ar_wait 0→1, aw_wait 0→1.
- A lane: ar/aw/rlast/b **16382→9682 (−6700)**; A ar_wait 0→3, aw_wait 0→3.
- shared ar/aw 24574→17874 (**−6700** = exactly the A-lane drop).
- cycle classification: neither 8190→1493 (**−6697**), a_only 4095001→4101698 (**+6697**), both 4307878→5362697 (+1054819), cycles +1054816.

**Same-shape delta (a8-mn n01 → a8-mpn formal01):** A ar/aw/rlast/b 10920→9682 (−1238), shared ar/aw 19112→17874 (−1238), neither 2728→1493 (−1235), a_only +1235, both +19924, V ar/aw unchanged 8192, V ar_wait/aw_wait 0→1.

Signal sources for these counters are `dma_observer.sv:375–386` (per-lane `bump(14+role*6 …)`, `bump(26..29 shared_*)`) and the “neither/v_only/a_only/both” cycle classification is not in the supplied RTL (it is an observer bank word outside `dma_observer.sv`) — see §5.

### 1.3 The four things that must not be conflated

1. **`data_errors = 0` — NOT evidence of correct data in the failing run.** `idma_verify()` runs only when the earlier stages returned 0 (`idma.c:2638–2649`), and `data_errors` is incremented only inside `idma_verify()` (`idma.c:806`). In this run `ret` is already `-EIO` when the verify block is reached (see §2/§4), so `idma_verify` is skipped and `data_errors=0` is the `memset` default (`idma.c:2170–2171`). The passing a1-mn/a8-mn records *did* reach `idma_verify`, so their `data_errors=0` means “verified”.
2. **Completion counts.** Victim: `submitted=4096`, `completed=4096`, `completed_bytes=16777216` (failed record.json:69–82); batch engine `issued=4096 done=4096` (dmesg:686). Aggressor: `submitted=5767`, `completed=5767`, `in_flight_at_end=1` (failed record.json:42–56,47). So the V copy count is complete; the A lane is short and still in flight at window close.
3. **AXI error / translation fault.** No AXI `SLVERR/DECERR` evidence exists in these artifacts; the only transport-side error signal present is the observer’s path error (bit 38, §2). The translation fault is the hardware `F: 15 | DID: 11` records (STORE_PAGE_FAULT, §3).
4. **Observer invalid.** The observer is invalidated by `status != 4` (idma.c:1076, 1406). That is a *record-validity* judgement, not a data or AXI result. Additionally the path lane chain `a w stage0 entered (9682) → a w stage1 entered (9659)` violates `p[1]+p[2] != next.entered` (idma.c:1106), so the path bank is also internally inconsistent. Note the log prints `valid=0` on all 16 `idma_path_obs` lines (dmesg:689–704) because that `valid` is the **single aggregate** variable reused in the loop (idma.c:1151–1157), not a per-lane flag.

Aggressor HPM (marked `raw_only`, record.json:46): `iotlb_miss=6128`, `s1_ptw=6128`, `s2_ptw=0` (failed). Compare a8-mn n01: `iotlb_miss=4135`; a1-mn: `iotlb_miss=0`. These are raw-only fields (`semantics.hpm`, record.json:289) and are not validated metrics.

---

## 2. `observer_status = 0x4000000004` bit38 → real RTL source

`0x4000000004` = bit2 (`0x4`) + bit38 (`0x4000000000`). Bit2 is `FROZEN`.

- Bit38 is produced **only** here:
  `cva6/corev_apu/fpga/src/dma_observer.sv:153`
  ```
  cfg_rdata_o[38] = error_q[6] || (path_q && path_error);
  ```
  (inside the `7'h60` status read, dma_observer.sv:148–155.)
- `error_q[6]` is never assigned anywhere in `dma_observer.sv` (only `error_n[0]…[5]` and `error_n[8]` are written — 445, 448, 454, 459–461, 464, 523, 525, 539; plus `error_q[2]` overflow at 140, `error_q[3]` at 355). Reset clears all (`dma_observer.sv:545`). So for this design bit38 = `path_q && path_error`.
- `path_q` is set at ARM from `cfg_wdata_i[26]` (`path_n = cfg_wdata_i[26]` dma_observer.sv:498; `path_q <= path_n` :583). The driver always sets that bit when `o->path` (`idma.c:1027`), and `o->path = o->p4m || o->e3 || (o->batch && idma_path_observe)` (`idma.c:996`), which is true for this composition run.
- `path_error` is the `error_o` of the path observer instance:
  `dma_observer.sv:123` → `.error_o(path_error)` (instance at :113–124).
- The path observer’s error definition:
  `cva6/corev_apu/fpga/src/dma_path_observer.sv:122`
  ```
  assign error_o = bad_q || |stage_error;
  ```
  - `bad_q` sources: `:158` (`arm_i && !idle_i`), `:164` (`&time_q`), `:167` (held in_valid dropped), `:170` (held out_valid/ID changed), `:176` (private AXI ID low bits non-zero on tr/out/return).
  - `stage_error[lane*4+stage]` comes from `dma_path_stage` (`:137–143`); its sources are `dma_path_observer.sv:59` (`bad`, defined `:30` with `matched` `:29`), `:72` (area overflow), `:75` (entered overflow), `:82` (full/Σ overflow), `:85` (carry overflow).

So: **bit38 ⇒ `path_error` ⇒ (`bad_q` or some `stage_error`) in `dma_path_observer`/`dma_path_stage`.** Which of the enumerated sub-sources fired is **not** recorded anywhere in the log/record — only the aggregate bit.

Corroborating time-ordering clue (evidence, not root cause): the composition bank word6 in the record is `4` (failed record.json:155, and driver expects `p->raw[6] == 4` at idma.c:2317), while the live `IDMA_OBS_CONTROL` read is `0x4000000004` (`idma.c:1399`, failed record.json:127). So bit38 was set after the word6 snapshot the composition bank holds. **Needs the composition-bank RTL to confirm what word6 latches and at which edge.**

---

## 3. `STORE_PAGE_FAULT (15)` — every producing branch, field sources, N/PN sharing/protection files

Cause 15 = `STORE_PAGE_FAULT`; the design emits it in exactly **two** places (both stage‑1, both selected by `is_store`):

### 3.1 IOTLB-hit permission fault (fast, no walk)
`rv_iommu_tl_wrap.sv:1366–1384` (state `PT`, `iotlb_lu_hit`):
```
1373  if (en_1S_q && (
1374      (is_store && !iotlb_lu_content.content_1S.w)            ||   // (1) W permission
1375      (is_rx && !…content_1S.r && !…content_1S.x)             ||   // (2) RX
1376      (!req_data.priv && !…content_1S.u)                      ||   // (3) U
1377      (req_data.priv && …content_1S.u && (!pc_q.ta.sum || is_rx))  // (4) SUM
1378  )) begin
1379      state_n = (dc_q.tc.dtf) ? (COMPLETE) : (ERROR);
1380      if (is_store)
1381          error_code_n = rv_iommu::STORE_PAGE_FAULT;   // <-- cause 15
1382      else
1383          error_code_n = rv_iommu::LOAD_PAGE_FAULT;
```
Note this branch checks **W/U/R/X/SUM only — there is no D (dirty) or A check on the IOTLB path**; A/D are only enforced during the walk (§3.2). `dtf` (`dc_q.tc.dtf`) can turn the fault into a plain completion.

### 3.2 PTW page fault (propagated through `ptw_cause`)
- `rv_iommu_ptw.sv` raises `pf_excep_n` for: invalid PTE `!pte.v || (!pte.r && pte.w) || (pte.g && stage≠STAGE_1)` (`:406–410`); **leaf** checks containing the dirty/access/misaligned/user set (`:490–500`):
  ```
  490  if ((main_lvl_q == LVL1 && |pte.ppn[17:0] != 1'b0) ||
  491      (main_lvl_q == LVL2 && |pte.ppn[8:0]  != 1'b0) ||
  492      (!pte.a || !pte.r || (is_store_i && !pte.d))   ||
  493      (ptw_stage_q != STAGE_1 && !pte.u)) begin
  ```
  non-leaf with `pte.a||pte.d||pte.u` (`:586–590`); reserved bits `[63:54]` (`:595–601`); `pte.ppn[43:GPPNW39]!=0` on STAGE_1 with en_2S (`:605–612`).
- Cause selection at `ERROR`:
  ```
  636  else if (pf_excep_q) begin
  641      if (is_store_i) cause_code_o = rv_iommu::STORE_GUEST_PAGE_FAULT;  // stage≠1
  ...
  647      else if (is_store_i) cause_code_o = rv_iommu::STORE_PAGE_FAULT;   // <-- cause 15, STAGE_1
  ```
  AXI `r.resp != OKAY` is a separate cause, `PT_DATA_CORRUPTION` (`:615–621`, `:632–634`), **not** a page fault.
- Propagation into `error_code_q`/`fault_data_o`: `rv_iommu_tl_wrap.sv:1300–1309` (`error_code_n = ptw_cause`, plus `error_2S_n/error_2S_int_n/error_gpaddr_n` at `:1302–1304`), then `fault_data_o.cause_code = error_code_q` at `:437`.

### 3.3 Exception-field sources (`rv_iommu_tl_wrap.sv:436–445`)
| fault_data_o field | source |
|---|---|
| `trans_type` | `req_data.ttype` (:436) |
| `cause_code` | `error_code_q` (:437) |
| `iova` | `req_data.iova` (:438) |
| `gpaddr` | `error_gpaddr_q` (:439) |
| `did` | `req_data.did` (:440) |
| `pv` / `pid` | `req_data.pid_valid` / `req_data.pid` (:441–442) |
| `is_supervisor` | `req_data.priv` (:443) |
| `is_guest_pf` | `error_2S_q` (:444) |
| `is_implicit` | `error_2S_int_q` (:445) |

`error_code_q` write sites: defaults :979–983; `TRANS_TYPE_DISALLOWED` :1020/:1071/:1207/:1274/:1467/:1510; DDTW cause :1088; PTW cause :1305; store/load page fault :1381/:1383; store/load guest :1400/:1402. `error_gpaddr_q` only set for guest faults (:1304/:1397). `fault_valid_o` is asserted in state `ERROR` (:1529,:1536).

Access-type derivation: `is_store` = `(&req_data.ttype[1:0]) & ~req_data.ttype[3]` (`rv_iommu_tl_wrap.sv:416`), `is_rx` = `(:419)`, and both are fanned to the PTW (`is_store_i`/`is_rx_i`, `:780–781`). **Not provided:** the DMA-side store/read pair mapping (which DMA burst is a store), and PTE contents for the four faulting IOVAs — so we cannot say which of §3.1 vs §3.2 fired, nor which of the sub-conditions (W / A / D / U / SUM / misaligned / reserved / GPA) applied.

### 3.4 N / PN sharing and protection-difference files (Fast path “N” vs policy-partitioned path)

If “N” = the nonblocking fast-hit path and “PN” = the policy/protected variant of it (naming not defined inside the supplied tree — see Assumption A2), the relevant code is:

- **Shared lookup sideband (N and main walker time-share the same IOTLB/DDTC ports):**
  - `rv_iommu_tl_wrap.sv:249–253` — `ddtc_lu = (state_q==DDT) | ats_ddtc_valid_i | n_ddtc_lu`, DID mux at `:250–251`, `ddtc_lu_hit = cache_ddtc_hit && !n_ddtc_lu` `:252`.
  - `rv_iommu_tl_wrap.sv:271–277` — `iotlb_lu = … | n_iotlb_lu` (`:271`), `iotlb_lu_vpn/pscid = n_iotlb_lu ? n_* : req_data.*` (`:272–273`), `iotlb_lu_gscid = n_iotlb_lu ? '0 : gscid_q` (`:274`), `cache_iotlb_en1 = n_iotlb_lu ? 1 : en_1S_q`, `cache_iotlb_en2 = n_iotlb_lu ? 0 : en_2S_q` (`:275–276`), `iotlb_lu_hit = cache_iotlb_hit && !n_iotlb_lu` (`:277`).
  - Connections: `rv_iommu_tl_wrap.sv:674–681` (`lookup_i`, `touch_i(!n_iotlb_lu || n_iotlb_touch)` :675, `lu_vpn_i/lu_pscid_i/lu_gscid_i`, `en_1S_i/en_2S_i`).
  - `rv_iommu_nonblock_hit.sv:2–4` (header: “DDTC and IOTLB ports … time-shared”), `:26–29` (ports), `:51–55` (`ddtc_lookup_o = state_q==DC||LEAF`, `iotlb_lookup_o = state_q==LEAF`, `iotlb_vpn_o = req_q.iova[…12]`, `iotlb_pscid_o = dc_q.ta.pscid`), `:100–107` (`accept_o`, `ddtc_touch_o/iotlb_touch_o = accept_o`), `:143–174` (IDLE→DC→LEAF→OUT/REJECT FSM).
- **Protection/partition difference:** `rv_iommu_tl_wrap.sv:668–672` sets the IOTLB allocation mask from **the main (`req_data`) DID**, not the probe:
  ```
  668  // Owner is the stable main PTW/MSI request, never a fast probe DID.
  669  .policy_mask_i (PolicyEnable && policy_partition_i ?
  670      (req_data.did == policy_did_i ?
  671       RVIOMMUCfg.NumIotlbEntries'(16'h0003) :   // 2 entries (protected DID)
  672       RVIOMMUCfg.NumIotlbEntries'(16'hfffc)) : '1),
  ```
  Consumed in `rv_iommu_iotlb.sv`: `policy_clear_i` `:36`, `policy_mask_i` `:37`, `allocation_mask = PolicyEnable ? policy_mask_i : '1` `:340`, `eligible_nodes` `:341–347`, `replace_en = en && allocation_mask[i]` `:385`, and tag write `:315–329` (which writes `en_1S: en_1S_i, en_2S: en_2S_i`, i.e. the **lookup** enables of that cycle, `:323–324`). PLRU touch uses `lu_hit[i] && (!TouchEnable || touch_i)` `:358`.
  Probe vs main tag constraints: because the probe forces `en_2S=0` and `gscid='0` (`tl_wrap:274–276`), IOTLB match for the probe degenerates to `match_gscid==1` and `match_stage = (tags.en_2S==0)&&(tags.en_1S==1)` (`rv_iommu_iotlb.sv:106,123`), i.e. **the probe can only hit S1-only entries and ignores GSCID**, while the main walker supplies real `gscid/en_2S`.
- **Context gating files that decide whether “PN” runs at all:** `rv_iommu_tl_wrap.sv:136–138` (`$fatal` unless `NonblockEnable && NumIotlbEntries==16` when `PolicyEnable`), `:284–287` (`normal_gate`, `trans_req_ready_o`, `nonblock_fire_o`, `nonblock_busy_o`), `:291–302` (`policy_quiet_o`), `:303–308` (`n_window`, `nonblock_diag_o`).

Config declared for this run: **InclATS0 / MSI_Bare** (given). In RTL that implies MSI branch disabled (`rv_iommu_tl_wrap.sv:927–943`) and ATS arbitration inputs idle (`:190–193`); so path N is the S1-only, `en_1S=1,en_2S=0` nonblock fast path.

**Code oddity worth Codex review (not a root-cause claim):** `rv_iommu_tl_wrap.sv:1336` reads `if(en_2S_q && en_2S_q)`, duplicating `en_2S_q` (likely intended `en_1S_q && en_2S_q`). It makes that arm identical to the `else if (en_2S_q)` arm at `:1342`; under the current S1-only config neither arm is taken (the `else if (en_1S_q)` at `:1348` is).

---

## 4. Shared IOTLB sideband / main-request queue & liveness — what is and is not shown

Where the sharing and liveness live:
- Sideband mux and hit suppression: `rv_iommu_tl_wrap.sv:271–277` (esp. `iotlb_lu_hit = cache_iotlb_hit && !n_iotlb_lu` :277 — while the probe drives the lookup, the main FSM’s hit is **forced 0**); `:674–681`.
- Main request queue/liveness: request arbiter+spill register `rv_iommu_tl_wrap.sv:181–214`, `req_ready` term `:174–176`; `normal_gate`/intercept `:284–287`; the nonblock FSM can sit in `REJECT` (`rv_iommu_nonblock_hit.sv:36,151,172`, header note `:2–4`) and the rejected probe “never handshakes; the original arbiter later accepts it” (`tl_wrap:282–283`); probe stability/accept conditions `rv_iommu_nonblock_hit.sv:100–112`.
- The probe window is intentionally concurrent with the main PTW: `n_window = state_q==PT && e3_ptw_active && en_1S_q && !en_2S_q && … && trans_req_data_i.bank != req_data.bank …` (`tl_wrap:303–306`), i.e. the sharing is designed for exactly the S1-only config used here.

Deliberately **not** concluded (per instruction “don’t guess from names”): no causal link is asserted between the shared sideband and the observed faults. Evidence that would be required but is **not provided**:
1. No per-cycle trace of `n_iotlb_lu`, `n_accept`, `cache_iotlb_hit`, `iotlb_lu_hit`, `n_iotlb_touch`, `ptw_axi_req_o.ar_valid`, `req_valid` around the fault times.
2. No IOTLB dump at fault time: the research bank (`rv_iommu_iotlb.sv:392–424`, fields `entries[i]`/`entries_next[i]`) is never captured into the record; the composition `raw[64..127]` values exist but their field mapping is not defined in any supplied file (see §5), so they cannot be read as IOTLB entries.
3. No `fault_data_o` field dump (`trans_type`, `is_supervisor`, `is_guest_pf`, `is_implicit`, `gpaddr`, `p`, `pid`) — so §3.1 vs §3.2 and A/D vs W cannot be separated.
4. No DC/PDT contents for DID 11 (which DDTC entry served DID 11), no `ddtp.iommu_mode`, no `dc.tc.dtf`, no `fsc.mode`/`iohgatp.mode` readback.
5. No composition-mapping (`src_iova/dst_iova`) for the a8-mpn trial.
6. The fault-print source (`F:/DID:/iotval:` formatter, and the fault-queue implementation) is not in the supplied files.

Two independent invalidities are visible in the driver for the failed trial (both real, order matters):
- `status != 4` at `idma.c:1076` (observer_finish) and `idma.c:1406` (p4m_finish) ⇒ `-EIO`.
- path chain check `rv_iommu_path` … `idma.c:1106`: A-write lane stage0 entered `9682` vs stage1 entered `9659` (dmesg:681–682) ⇒ also `valid=false`; plus A-write stage1 `end=4`, `full=9655` (dmesg:682).

`idma_composition_capture` (`idma.c:2304–2336`) itself passes for this record (raw[6]=4, raw[24]=4095000, raw[34]=3, raw[40..63]=0), so it returns 0; because `ret` was already `-EIO`, that does not change the outcome. `idma_verify` is skipped (`idma.c:2638`).

---

## 5. Assumptions (explicit)

- **A1.** `seq N` identifies one composition call; `batch_obs`/`path_obs` precede `batch_start` for the same `seq` because of `idma.c:2651/2656/2728–2730`. Consistent with all log lines.
- **A2.** “N”/“PN” are taken (from the case names `-mn`/`-mpn` and the RTL `NonblockEnable`/`PolicyEnable`/`policy_partition_i`) to mean the nonblocking fast path and its policy-partitioned variant. This naming is **not defined in any supplied file**; the RTL facts in §3.4 are cited independently of the naming.
- **A3.** `iotval` in the `F:` record is the faulting IOVA (stage-1 page fault); `iotval2` is a second field the printer maps to 0. The printer is unknown.
- **A4.** The composition record’s `p4m.observation[i]` corresponds 1:1 to the `idma_batch_obs … part=axi_v/axi_a` words (idma.c:1127–1150, 1400–1401); verified by exact equality of word 0 (=`cycles`) between dmesg seq53 and the a1-mn record, and by the four-way sum invariants.
- **A5.** The faulting IOVAs lie in the aggressor’s range (DID 11 = aggressor, `idma.c:2132`), which is why cause 15 on DID 11 is a store-side fault; the exact src/dst IOVA ownership is not in the record.

---

## 6. Suggested next minimal source / minimal discriminating observation (for Codex to execute)

Minimal sources to read next (not supplied here):
1. `rv_iommu.sv` (+ `rv_iommu_reg_pkg.sv` / `rv_iommu_pkg.sv`) — fault-queue field mapping and the `F:/iotval:` printer; `STORE_PAGE_FAULT` constant; `dtf`.
2. `rv_iommu_cfg` (the instantiated config) — confirm `InclATS=0`, `MSITrans`, `PolicyEnable`, `NonblockEnable`, `NumIotlbEntries`.
3. FPGA composition-bank RTL — definitions of bank word6 and word24, to explain `raw[6]=4` vs live `0x4000000004`.
4. `rv_iommu_ddtc.sv` / `rv_iommu_ddtw.sv` — DID-11 DC contents path (fields used by `dc_ok`, `nonblock_hit.sv:84–93`).

Minimal discriminating observations (one bounded capture each, no board expansion beyond the existing run):
1. Dump `fault_data_o` (`trans_type`, `cause_code`, `iova`, `is_supervisor`, `is_guest_pf`, `is_implicit`, `gpaddr`, `did`, `p`, `pid`) per fault — separates §3.1 IOTLB-hit W/U/SUM fault from §3.2 PTW A/D/W/U/R/or reserved fault.
2. Dump the IOTLB research bank (`rv_iommu_iotlb.sv:392–424`) immediately before/after the first fault, decoding `entries[i]`/`entries_next[i]` per `:399–423`, for the four faulting VPNs — separates “resident entry has wrong W/U” from “miss/walk fault”.
3. Concurrent capture of `n_iotlb_lu`, `cache_iotlb_hit`, `iotlb_lu_hit`, `n_accept`, `n_iotlb_touch`, `req_valid`, `ptw_axi_req_o.ar_valid` at the faulting `req_data.did==11`/`req_data.iova` — tests the shared-sideband concurrency hypothesis.
4. Timestamped capture of `IDMA_OBS_CONTROL` (bit38) at the composition close edge vs at the driver’s finish reads — confirms whether `path_error` latched after the bank word6 snapshot.
5. For the A-write lane, capture `dma_path_observer` per-stage `enter_i`/`leave_i`/`count_q` around the close edge — explains the 9682→9659 chain break and stage1 `end=4`.

No tests or patches are proposed here; the above are observation points only.