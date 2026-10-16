## Findings summary

1. **`tests/banked_ctl_test.cpp`**
   - Failures 1–4 and 6 are caused by `expect_write()` / `expect_read()` deriving the expected translation DID from the AXI ID. The test’s `drive_write()` / `drive_read()` use a separate DID (`0x5`, `0x7`, `0x9`), so the model compares against the wrong value. Fix: pass an explicit expected DID for every translation entry.
   - Failures 9–13 are caused by the completion model requiring an accepted completion AW before it will accept any completion W beat. AXI legally allows W before AW, and the DUT does exactly that while `comp_aw_ready` is low. Fix: capture completion W beats into an independent FIFO and associate them only in completion-AW order with exact `data`/`strb`/`last` matching. This also removes the old payload-search-across-AWs behaviour that could mask AW/W swaps.
   - No test file change should weaken underflow, drain, identity, or byte checks.

2. **`tests/nonblock_tl_test.cpp`**
   - The VMA invalidation scenario uses `{gv=1, av=0, pscv=0}`. In `rv_iommu_iotlb.sv` that selects case `3'b100`, which only invalidates entries with `en_2S && en_1S`. The warmed V leaf is S1-only (`en_2S=0, en_1S=1`), so it is not removed and the fast path can still hit it. Fix: use `gv=0, av=0, pscv=0` (case `3'b000`), which invalidates host S1-only entries, forcing a real PTW refill.
   - Keep the existing `latched_req_fire` handling intact.

3. **`scripts/platform.py` builder**
   - Restrict `--workspace-overlay` to `action == fpga` and the two new N profiles only.
   - Reject empty overlay lists.
   - Validate overlay profile config: `nonblock_mode` in `(1,2)`, `legacy_e3_observer is False`, `research_role == 0`.
   - Do **not** bypass the old `check()` pre-check. Instead, teach `check()` to allow only the explicitly overlaid paths to be dirty.
   - Path validation: split raw string on `/`, reject empty / `.` / `..` segments before any normalization, reject backslashes, require `cva6/` prefix, reject Linux paths.
   - Reject symlink components for both source and destination.
   - At copy time, read the source bytes once, compute SHA-256 and size in memory, compare against the manifest-time values, and write exactly those bytes to the destination. No second read-back.
   - Mode preservation is optional and not required for RTL.
   - Reuse the two config files from the previous proposal unchanged.

---

## Test diff 1 — `tests/banked_ctl_test.cpp`

```diff
--- a/tests/banked_ctl_test.cpp
+++ b/tests/banked_ctl_test.cpp
@@ -158,6 +158,7 @@ struct Sim {
     // completion slave + scoreboard
     std::deque<CawEnt> caw_q;                // accepted comp AW awaiting their W
+    std::deque<CmpWRec> cw_q;                // comp W beats captured independently of AW
     std::deque<BBeat>  b_q;
     std::deque<RBeat>  r_q;
 
@@ -204,12 +205,14 @@ static void add_xlate(uint64_t iova, uint64_t paddr, bool error, uint32_t did) {
 }
 
 static void expect_write(uint32_t id, uint64_t iova, uint64_t paddr,
-                         bool error, const std::vector<WBeat> &beats) {
-    add_xlate(iova, paddr, error, id);
+                         bool error, uint32_t did,
+                         const std::vector<WBeat> &beats) {
+    add_xlate(iova, paddr, error, did);
     ExpWr e; e.id = id; e.paddr = paddr; e.beats = beats;
     g_exp_wr.push_back(e);
 }
 static void expect_read(uint32_t id, uint64_t iova, uint64_t paddr,
-                        uint32_t len, bool error) {
-    add_xlate(iova, paddr, error, id);
+                        uint32_t len, bool error, uint32_t did) {
+    add_xlate(iova, paddr, error, did);
     ExpRd e; e.id = id; e.paddr = paddr; e.len = len;
     g_exp_rd.push_back(e);
 }
@@ -503,7 +506,7 @@ static void test_write_bank0_multi() {
     beats.push_back(beat(0x1111222233334444ull, false));
     beats.push_back(beat(0xAAAABBBBCCCCDDDDull, true));
 
-    expect_write(0x0, 0x1000, 0x80000000ull, false, beats);
+    expect_write(0x0, 0x1000, 0x80000000ull, false, 0x5, beats);
     drive_write(0x0, 0x1000, 1, 0x5, beats);
     run_cycles(140);
 
@@ -530,8 +533,8 @@ static void test_two_banks_distinct_ids() {
     std::vector<WBeat> b0; b0.push_back(beat(0xB0B0B0B0B0B0B0B0ull, true));
     std::vector<WBeat> b1; b1.push_back(beat(0xB1B1B1B1B1B1B1B1ull, true));
-    expect_write(0x1, 0x1000, 0x40000000ull, false, b0);
-    expect_write(0x4, 0x2000, 0x40001000ull, false, b1);
+    expect_write(0x1, 0x1000, 0x40000000ull, false, 0x1, b0);
+    expect_write(0x4, 0x2000, 0x40001000ull, false, 0x4, b1);
 
     drive_write(0x1, 0x1000, 0, 0x1, b0);
     run_cycles(20);
@@ -555,7 +558,7 @@ static void test_error_write_bank1() {
     std::vector<WBeat> beats; beats.push_back(beat(0xE1E1E1E1E1E1E1E1ull, true));
-    expect_write(0x4, 0x2000, 0xDEADBEE0ull, true, beats);   // translation error
+    expect_write(0x4, 0x2000, 0xDEADBEE0ull, true, 0x7, beats);   // translation error
     drive_write(0x4, 0x2000, 0, 0x7, beats);
     run_cycles(100);
 
@@ -578,8 +581,8 @@ static void test_two_bank_reorder() {
     std::vector<WBeat> b0; b0.push_back(beat(0xA0A0A0A0A0A0A0A0ull, true));
     std::vector<WBeat> b1; b1.push_back(beat(0xA1A1A1A1A1A1A1A1ull, true));
-    expect_write(0x0, 0x1000, 0x50000000ull, false, b0);
-    expect_write(0x4, 0x2000, 0x50001000ull, false, b1);
+    expect_write(0x0, 0x1000, 0x50000000ull, false, 0x0, b0);
+    expect_write(0x4, 0x2000, 0x50001000ull, false, 0x4, b1);
 
     drive_write(0x0, 0x1000, 0, 0x0, b0);
     run_cycles(20);
@@ -611,9 +614,9 @@ static void test_same_id_order_and_capacity() {
     std::vector<WBeat> w0; w0.push_back(beat(0xD0D0D0D0D0D0D0D0ull, true));
     std::vector<WBeat> w1; w1.push_back(beat(0xD1D1D1D1D1D1D1D1ull, true));
     std::vector<WBeat> w2; w2.push_back(beat(0xD2D2D2D2D2D2D2D2ull, true));
-    expect_write(0x0, 0x1000, 0x60000000ull, false, w0);
-    expect_write(0x0, 0x2000, 0x60001000ull, false, w1);
-    expect_write(0x0, 0x3000, 0x60002000ull, false, w2);
+    expect_write(0x0, 0x1000, 0x60000000ull, false, 0x0, w0);
+    expect_write(0x0, 0x2000, 0x60001000ull, false, 0x0, w1);
+    expect_write(0x0, 0x3000, 0x60002000ull, false, 0x0, w2);
 
     g_up_aw_stall = 0;
     drive_write(0x0, 0x1000, 0, 0x0, w0);
@@ -653,7 +656,7 @@ static void test_output_backpressure_recovery() {
     std::vector<WBeat> beats;
     beats.push_back(beat(0xC0C0C0C0C0C0C0C0ull, false));
     beats.push_back(beat(0xC1C1C1C1C1C1C1C1ull, true));
-    expect_write(0x0, 0x1000, 0x70000000ull, false, beats);
+    expect_write(0x0, 0x1000, 0x70000000ull, false, 0x0, beats);
     drive_write(0x0, 0x1000, 1, 0x0, beats);
 
     run_cycles(40);
@@ -688,7 +691,7 @@ static void test_w_before_aw() {
     std::vector<WBeat> beats; beats.push_back(beat(0xE0E0E0E0E0E0E0E0ull, true));
-    expect_write(0x0, 0x1000, 0x90000000ull, false, beats);
+    expect_write(0x0, 0x1000, 0x90000000ull, false, 0x0, beats);
 
     up.wq.push_back(beats[0]);
     up.aw_valid = false;
@@ -711,7 +714,7 @@ static void test_read_bank0_single() {
 
-    expect_read(0x0, 0x4000, 0xCAFEF000ull, 0, false);
+    expect_read(0x0, 0x4000, 0xCAFEF000ull, 0, false, 0x9);
     drive_read(0x0, 0x4000, 0, 0x9);
     run_cycles(100);
 
@@ -736,7 +739,7 @@ static void test_read_bank1_multi() {
 
-    expect_read(0x4, 0x4000, 0xC0FFEE00ull, 1, false);
+    expect_read(0x4, 0x4000, 0xC0FFEE00ull, 1, false, 0x4);
     drive_read(0x4, 0x4000, 1, 0x4);
     run_cycles(160);
 
@@ -760,7 +763,7 @@ static void test_error_read_bank0() {
 
-    expect_read(0x0, 0x4000, 0x0ull, 0, true);   // translation error
+    expect_read(0x0, 0x4000, 0x0ull, 0, true, 0x9);   // translation error
     drive_read(0x0, 0x4000, 0, 0x9);
     run_cycles(100);
 
@@ -386,33 +386,41 @@ static void service() {
     }
 
     if (dut->comp_w_valid && dut->comp_w_ready) {
         L_cmp_w.push_back({g_cycle, dut->comp_w_data, dut->comp_w_strb, (bool)dut->comp_w_last});
-        // Associate this beat with the outstanding AW whose next expected beat
-        // it matches (payload/strb/last).  Unique payload tags keep this
-        // unambiguous, so a corrupted or duplicated beat matches nothing.
-        int match = -1;
-        for (size_t i = 0; i < sim.caw_q.size(); ++i) {
-            CawEnt &c = sim.caw_q[i];
-            if (!c.have_exp || c.got >= c.exp.size()) continue;
-            if (c.exp[c.got].data == dut->comp_w_data &&
-                c.exp[c.got].strb == (uint64_t)dut->comp_w_strb &&
-                c.exp[c.got].last == (bool)dut->comp_w_last) { match = (int)i; break; }
-        }
-        if (match < 0) {
-            expect(false, "comp W beat matches no outstanding comp AW");
-        } else {
-            CawEnt &c = sim.caw_q[(size_t)match];
-            c.got++;
-            if (dut->comp_w_last) {
-                expect_eq((uint64_t)c.got, (uint64_t)c.len + 1, "comp W burst length != len+1");
-                BBeat bb; bb.id = c.id; bb.resp = 0;
-                sim.b_q.push_back(bb);
-                if (c.exp_ptr) c.exp_ptr->b_hits++;
-                sim.caw_q.erase(sim.caw_q.begin() + match);
-            }
-        }
+        sim.cw_q.push_back({g_cycle, dut->comp_w_data, (uint64_t)dut->comp_w_strb, (bool)dut->comp_w_last});
+    }
+    while (!sim.caw_q.empty() && !sim.cw_q.empty()) {
+        CawEnt &c = sim.caw_q.front();
+        CmpWRec &w = sim.cw_q.front();
+        if (!c.have_exp) {
+            expect(false, "comp AW without expected beats");
+            sim.caw_q.pop_front();
+            continue;
+        }
+        if (c.got >= c.exp.size()) {
+            expect(false, "comp AW has more W beats than expected");
+            sim.caw_q.pop_front();
+            continue;
+        }
+        const WBeat &e = c.exp[c.got];
+        if (w.data != e.data || w.strb != e.strb || w.last != e.last) {
+            expect(false, "comp W beat mismatch (data/strb/last) for oldest outstanding comp AW");
+            sim.cw_q.pop_front();
+            continue;
+        }
+        c.got++;
+        sim.cw_q.pop_front();
+        if (w.last) {
+            expect_eq((uint64_t)c.got, (uint64_t)c.len + 1, "comp W burst length != len+1");
+            BBeat bb; bb.id = c.id; bb.resp = 0;
+            sim.b_q.push_back(bb);
+            if (c.exp_ptr) c.exp_ptr->b_hits++;
+            sim.caw_q.pop_front();
+        }
     }
 
     // (3) start new model outputs
```

---

## Test diff 2 — `tests/nonblock_tl_test.cpp`

```diff
--- a/tests/nonblock_tl_test.cpp
+++ b/tests/nonblock_tl_test.cpp
@@ -579,7 +579,7 @@ public:
         tick(); // DC -> LEAF
         CHECK_AT(!req_fire, "inval/VMA: accepted while DC");
         top->inval_vma = 1;
-        top->inval_gv  = 1;
+        top->inval_gv  = 0; // S1-only entry: gv=0,av=0,pscv=0 invalidates host S1 entries
         tick(); // LEAF sees VMA invalidation -> REJECT
         CHECK_AT(!req_fire, "inval/VMA: fast accept during LEAF VMA invalidation");
         CHECK_AT(!req_fire_fast, "inval/VMA: fast path fired during LEAF VMA invalidation");
```

---

## Builder diff — `scripts/platform.py`

This is the complete replacement builder diff against the actual `scripts/platform.py`. It does **not** bypass `check()`; it makes `check()` aware of the exact overlay paths. It does **not** claim any build or test passed.

```diff
--- a/scripts/platform.py
+++ b/scripts/platform.py
@@ -33,6 +33,69 @@ def sha(path):
         for chunk in iter(lambda: f.read(1024 * 1024), b''):
             digest.update(chunk)
         return digest.hexdigest()
 
+def load_workspace_overlay(path):
+    data = json.loads(Path(path).read_text())
+    if not isinstance(data, list):
+        raise RuntimeError('--workspace-overlay must contain a JSON list')
+    if not data:
+        raise RuntimeError('--workspace-overlay must not be empty')
+    overlay = []
+    seen = set()
+    root = ROOT.resolve()
+    for raw in data:
+        if not isinstance(raw, str):
+            raise RuntimeError('Workspace overlay entries must be strings')
+        if '\\' in raw:
+            raise RuntimeError(f'Workspace overlay path must use POSIX separators: {raw}')
+        parts = raw.split('/')
+        if not parts or any(part in ('', '.', '..') for part in parts):
+            raise RuntimeError(f'Workspace overlay path has empty or traversal segment: {raw}')
+        if parts[0] != 'cva6' or len(parts) < 2:
+            raise RuntimeError(f'Workspace overlay path must be under cva6/: {raw}')
+        if any(part.lower() == 'linux' for part in parts):
+            raise RuntimeError(f'Workspace overlay path must not reference Linux: {raw}')
+        rel_str = '/'.join(parts)
+        if rel_str in seen:
+            raise RuntimeError(f'Duplicate workspace overlay path: {raw}')
+        seen.add(rel_str)
+        source = ROOT / Path(*parts)
+        cur = ROOT
+        for part in parts:
+            cur = cur / part
+            if cur.is_symlink():
+                raise RuntimeError(f'Workspace overlay source path contains a symlink: {cur}')
+        if not source.is_file():
+            raise RuntimeError(f'Workspace overlay source is not a regular file: {raw}')
+        if not source.resolve().is_relative_to(root):
+            raise RuntimeError(f'Workspace overlay source escapes platform root: {raw}')
+        raw_bytes = source.read_bytes()
+        if b'\x00' in raw_bytes:
+            raise RuntimeError(f'Workspace overlay source contains NUL bytes: {raw}')
+        try:
+            raw_bytes.decode('utf-8')
+        except UnicodeDecodeError as exc:
+            raise RuntimeError(f'Workspace overlay source is not UTF-8 text: {raw}') from exc
+        overlay.append({'path': rel_str, 'sha256': hashlib.sha256(raw_bytes).hexdigest(), 'size': len(raw_bytes)})
+    return overlay
+
+def copy_workspace_overlay(stage):
+    overlay = BUILD_CONTEXT.get('workspace_overlay', [])
+    if not overlay:
+        return
+    stage_resolved = stage.resolve()
+    for item in overlay:
+        parts = item['path'].split('/')
+        source = ROOT / Path(*parts)
+        cur = ROOT
+        for part in parts:
+            cur = cur / part
+            if cur.is_symlink():
+                raise RuntimeError(f'Workspace overlay source path contains a symlink: {cur}')
+        if not source.is_file():
+            raise RuntimeError(f'Workspace overlay source is not a regular file: {source}')
+        raw_bytes = source.read_bytes()
+        if hashlib.sha256(raw_bytes).hexdigest() != item['sha256'] or len(raw_bytes) != item['size']:
+            raise RuntimeError(f'Workspace overlay source changed after manifest: {item["path"]}')
+        dest = stage / Path(*parts)
+        cur = stage
+        for part in parts:
+            cur = cur / part
+            if cur.is_symlink():
+                raise RuntimeError(f'Workspace overlay destination path contains a symlink: {cur}')
+        if dest.exists() and dest.is_dir():
+            raise RuntimeError(f'Overlay destination is an existing directory: {dest}')
+        if not dest.resolve().is_relative_to(stage_resolved):
+            raise RuntimeError(f'Workspace overlay destination escapes stage: {dest}')
+        dest.parent.mkdir(parents=True, exist_ok=True)
+        dest.write_bytes(raw_bytes)
+
 def records():
     return json.loads((ROOT / 'sources.lock.json').read_text())['components']
 
@@ -40,11 +103,24 @@ def records():
-def check():
+def check(allowed_dirty=None):
+    allowed_dirty = allowed_dirty or {}
     for item in records():
         p = ROOT / item['path']
         actual = capture(['git', 'rev-parse', 'HEAD'], p)
         if actual != item['commit']:
             raise RuntimeError(f"Version mismatch: {item['path']}")
-        if capture(['git', '--no-optional-locks', 'status', '--porcelain', '--untracked-files=normal'], p):
-            raise RuntimeError(f"Dirty component: {item['path']}; commit changes and update lock before building")
+        status_lines = capture(['git', '--no-optional-locks', 'status', '--porcelain', '--untracked-files=normal'], p).splitlines()
+        allowed = allowed_dirty.get(item['path'], set())
+        for line in status_lines:
+            if not line:
+                continue
+            if len(line) < 4:
+                raise RuntimeError(f"Dirty component: {item['path']}; unparseable status: {line}")
+            path = line[3:].strip()
+            if ' -> ' in path:
+                path = path.split(' -> ', 1)[1].strip()
+            if path in allowed:
+                continue
+            raise RuntimeError(f"Dirty component: {item['path']}; commit changes and update lock before building")
         alt = Path(capture(['git', 'rev-parse', '--git-path', 'objects/info/alternates'], p))
         if not alt.is_absolute():
             alt = p / alt
@@ -157,6 +193,7 @@ def fpga(profile):
     for item in records():
         if item['path'] == 'cva6' or item['path'].startswith('cva6/'):
             stage_repo(item, stage/item['path'])
+    copy_workspace_overlay(stage)
     cva6 = stage/'cva6'
     top = cva6/'corev_apu/fpga/src/ariane_xilinx.sv'
     data = top.read_text()
@@ -166,12 +203,24 @@ def fpga(profile):
     research_role = cfg.get('research_role', 0)
     if type(research_role) is not int or research_role not in (0, 1, 2):
         raise RuntimeError('research_role must be 0 (legacy), 1 (E2), or 2 (method)')
-    for name, val in [('IOMMUDSFixedDelayInput', cfg['ds_delay_input']),
-                      ('IOMMUDSFixedDelayOutput', cfg['ds_delay_output']),
-                      ('IOMMULegacyE3Observer', legacy_e3_observer),
-                      ('IOMMUResearchRole', research_role)]:
-        data, n = re.subn(r'(localparam int unsigned '+name+r'\s*=\s*)\d+', lambda m:m[1]+str(int(val)), data)
-        if n != 1: raise RuntimeError(f'Missing unique hardware parameter: {name}')
+    nonblock_mode = cfg.get('nonblock_mode', 0)
+    if type(nonblock_mode) is not int or nonblock_mode not in (0, 1, 2):
+        raise RuntimeError('nonblock_mode must be 0 (off), 1 (serial), or 2 (hit enabled)')
+    if nonblock_mode:
+        if legacy_e3_observer or research_role != 0:
+            raise RuntimeError('nonblock_mode profiles require legacy_e3_observer=false and research_role=0')
+        if cfg.get('ds_delay_input') != 50 or cfg.get('ds_delay_output') != 50 or cfg.get('iotlb_entries') != 16:
+            raise RuntimeError('nonblock_mode profiles require DS50/50 and 16 IOTLB entries')
+    params = [('IOMMUDSFixedDelayInput', cfg['ds_delay_input']),
+              ('IOMMUDSFixedDelayOutput', cfg['ds_delay_output']),
+              ('IOMMULegacyE3Observer', legacy_e3_observer),
+              ('IOMMUResearchRole', research_role)]
+    if nonblock_mode:
+        params.append(('IOMMUNonblockMode', nonblock_mode))
+    for name, val in params:
+        data, n = re.subn(r'(localparam int unsigned '+name+r'\s*=\s*)\d+', lambda m:m[1]+str(int(val)), data)
+        if n != 1: raise RuntimeError(f'Missing unique hardware parameter: {name}')
     top.write_text(data)
     soc = cva6/'corev_apu/tb/ariane_soc_pkg.sv'
     data, n = re.subn(r'(NumIotlbEntries\s*:\s*32\x27d)\d+', lambda m:m[1]+str(cfg['iotlb_entries']), soc.read_text())
@@ -256,9 +285,10 @@ def main():
     parser=argparse.ArgumentParser()
     parser.add_argument('action',choices=['doctor','check','bench','rootfs','kernel','firmware','software','hpm-test','fpga','bundle'])
-    parser.add_argument('--profile',default='migration-4dma',choices=['migration-4dma','baseline-4dma','migration-4dma-scfix','migration-4dma-observe','migration-4dma-batch','baseline-4dma-batch','migration-4dma-path','migration-4dma-e3','migration-4dma-e3-queue','migration-4dma-e3-controlled','migration-4dma-p4m','migration-4dma-research-v1','migration-4dma-research-e2-v2','migration-4dma-research-method-v2'])
+    parser.add_argument('--profile',default='migration-4dma',choices=['migration-4dma','baseline-4dma','migration-4dma-scfix','migration-4dma-observe','migration-4dma-batch','baseline-4dma-batch','migration-4dma-path','migration-4dma-e3','migration-4dma-e3-queue','migration-4dma-e3-controlled','migration-4dma-p4m','migration-4dma-research-v1','migration-4dma-research-e2-v2','migration-4dma-research-method-v2','migration-4dma-n-serial','migration-4dma-n-hit'])
+    parser.add_argument('--workspace-overlay', type=Path, default=None)
     parser.add_argument('--jobs',type=int,default=12)
     args=parser.parse_args()
+    if args.workspace_overlay is not None:
+        if args.action != 'fpga':
+            parser.error('--workspace-overlay is only valid with the fpga action')
+        if args.profile not in ('migration-4dma-n-serial', 'migration-4dma-n-hit'):
+            parser.error('--workspace-overlay is only valid with the new N profiles')
+        cfg = json.loads((ROOT/'configs'/f'{args.profile}.json').read_text())
+        if cfg.get('nonblock_mode') not in (1, 2) or cfg.get('legacy_e3_observer') is not False or cfg.get('research_role') != 0:
+            parser.error('--workspace-overlay requires nonblock_mode 1/2, legacy_e3_observer=false, research_role=0')
+    overlay = load_workspace_overlay(args.workspace_overlay) if args.workspace_overlay is not None else []
+    allowed_dirty = {}
+    for item in overlay:
+        parts = item['path'].split('/')
+        allowed_dirty.setdefault(parts[0], set()).add('/'.join(parts[1:]))
     BUILD_CONTEXT = {'timestamp':datetime.now(timezone.utc).isoformat(),
                      'platform_commit':capture(['git','rev-parse','HEAD']),
                      'platform_dirty':bool(capture(['git','status','--porcelain'])),
                      'sources_lock_sha256':sha(ROOT/'sources.lock.json'),
-                     'profile_sha256':sha(ROOT/'configs'/f'{args.profile}.json')}
+                     'profile_sha256':sha(ROOT/'configs'/f'{args.profile}.json'),
+                     'workspace_overlay':overlay}
     try:
         if args.action not in ['doctor','bundle']:
-            check()
+            check(allowed_dirty)
         if args.action=='doctor': doctor()
         elif args.action=='bench': bench()
         elif args.action=='rootfs': rootfs()
```

---

## Configs

The two config files from the previous proposal can be reused unchanged:

- `configs/migration-4dma-n-serial.json` — `nonblock_mode: 1`
- `configs/migration-4dma-n-hit.json` — `nonblock_mode: 2`

Both must satisfy:
- `legacy_e3_observer: false`
- `research_role: 0`
- `ds_delay_input: 50`
- `ds_delay_output: 50`
- `iotlb_entries: 16`

No additional config changes are proposed here.

---

## Assumptions and unresolved issues

- **Banked test:** The completion model now buffers W beats independently. The association loop runs every cycle after the capture blocks, so it handles both W-before-AW and AW-before-W. Mismatched W beats are reported and popped to avoid an infinite loop; the AW remains at the front so a subsequent correct beat can still complete it. This preserves underflow detection because a missing beat leaves the AW in `caw_q` and `b_hits` never increments.
- **TL test:** Changing `inval_gv` to `0` relies on the fast path still rejecting the leaf when `inval_vma` is asserted. The IOTLB definition confirms `{gv=0, av=0, pscv=0}` invalidates S1-only host entries, so the subsequent normal-path request must refill. If the fast path has its own separate invalidation semantics, the parent should confirm that `inval_vma` alone is sufficient to trigger the REJECT.
- **Builder:** The overlay source is expected to live under `ROOT/cva6/...`. `check()` is still called; it now allows only the explicitly listed overlay paths to be dirty for the `cva6` component. All other dirty files in any pinned component still fail. The overlay is rejected unless `action == fpga` and `profile` is exactly one of the two new N profiles with the required `nonblock_mode` / `legacy_e3_observer` / `research_role` values. The list must be non-empty. Source and destination symlink components are rejected. The copy reads the source bytes once, hashes in memory, compares against manifest-time SHA-256 and size, and writes exactly those bytes. No build or test has been run here.
- **Required RTL parameter:** `IOMMUNonblockMode` must already exist in the source top for `nonblock_mode` in `(1,2)`. The script rewrites only its numeric value; it does not insert the parameter.

## Suggested acceptance checks

- `python3 -m py_compile scripts/platform.py`
- `python3 scripts/platform.py --help` shows `--workspace-overlay` and both new profile names.
- `python3 scripts/platform.py fpga --profile migration-4dma-n-serial --workspace-overlay bad.json` rejects:
  - absolute paths
  - `..` / `.` / empty segments
  - backslashes
  - non-`cva6` paths
  - symlinked source files or parents
  - empty JSON list
- `python3 scripts/platform.py fpga --profile migration-4dma` with `--workspace-overlay` is rejected because the profile is not one of the new N profiles.
- `python3 scripts/platform.py check --workspace-overlay good.json` is rejected because overlay is only valid with `fpga`.
- With a valid overlay, `check()` still fails if a non-overlay file in `cva6` is dirty.
- If the overlay source changes between manifest hash and copy, the build aborts with `Workspace overlay source changed after manifest`.
- Run the two C++ tests against the patched sources; the previously failing cases should no longer fail for the wrong DID, and the completion backpressure case should no longer reject legal W-before-AW.