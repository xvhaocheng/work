```diff
--- a/scripts/platform.py
+++ b/scripts/platform.py
@@ -33,6 +33,82 @@ def sha(path):
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
 
@@ -157,19 +157,31 @@ def fpga(profile):
     for item in records():
         if item['path'] == 'cva6' or item['path'].startswith('cva6/'):
             stage_repo(item, stage/item['path'])
+    copy_workspace_overlay(stage)
     cva6 = stage/'cva6'
     top = cva6/'corev_apu/fpga/src/ariane_xilinx.sv'
     data = top.read_text()
     legacy_e3_observer = cfg.get('legacy_e3_observer', True)
     if not isinstance(legacy_e3_observer, bool):
         raise RuntimeError('legacy_e3_observer must be a JSON boolean')
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
@@ -256,15 +256,33 @@ def main():
     global BUILD_CONTEXT
     parser=argparse.ArgumentParser()
     parser.add_argument('action',choices=['doctor','check','bench','rootfs','kernel','firmware','software','hpm-test','fpga','bundle'])
-    parser.add_argument('--profile',default='migration-4dma',choices=['migration-4dma','baseline-4dma','migration-4dma-scfix','migration-4dma-observe','migration-4dma-batch','baseline-4dma-batch','migration-4dma-path','migration-4dma-e3','migration-4dma-e3-queue','migration-4dma-e3-controlled','migration-4dma-p4m','migration-4dma-research-v1','migration-4dma-research-e2-v2','migration-4dma-research-method-v2'])
+    parser.add_argument('--profile',default='migration-4dma',choices=['migration-4dma','baseline-4dma','migration-4dma-scfix','migration-4dma-observe','migration-4dma-batch','baseline-4dma-batch','migration-4dma-path','migration-4dma-e3','migration-4dma-e3-queue','migration-4dma-e3-controlled','migration-4dma-p4m','migration-4dma-research-v1','migration-4dma-research-e2-v2','migration-4dma-research-method-v2','migration-4dma-n-serial','migration-4dma-n-hit'])
+    parser.add_argument('--workspace-overlay', type=Path, default=None)
     parser.add_argument('--jobs',type=int,default=12)
     args=parser.parse_args()
-    BUILD_CONTEXT = {'timestamp':datetime.now(timezone.utc).isoformat(),
-                     'platform_commit':capture(['git','rev-parse','HEAD']),
-                     'platform_dirty':bool(capture(['git','status','--porcelain'])),
-                     'sources_lock_sha256':sha(ROOT/'sources.lock.json'),
-                     'profile_sha256':sha(ROOT/'configs'/f'{args.profile}.json')}
+    if args.workspace_overlay is not None:
+        if args.action != 'fpga':
+            parser.error('--workspace-overlay is only valid with the fpga action')
+        if args.profile not in ('migration-4dma-n-serial', 'migration-4dma-n-hit'):
+            parser.error('--workspace-overlay is only valid with the new N profiles')
+        cfg = json.loads((ROOT/'configs'/f'{args.profile}.json').read_text())
+        if cfg.get('nonblock_mode') not in (1, 2) or cfg.get('legacy_e3_observer') is not False or cfg.get('research_role') != 0:
+            parser.error('--workspace-overlay requires nonblock_mode 1/2, legacy_e3_observer=false, research_role=0')
+        overlay = load_workspace_overlay(args.workspace_overlay)
+    else:
+        overlay = []
+    BUILD_CONTEXT = {'timestamp':datetime.now(timezone.utc).isoformat(),
+                     'platform_commit':capture(['git','rev-parse','HEAD']),
+                     'platform_dirty':bool(capture(['git','status','--porcelain'])),
+                     'sources_lock_sha256':sha(ROOT/'sources.lock.json'),
+                     'profile_sha256':sha(ROOT/'configs'/f'{args.profile}.json'),
+                     'workspace_overlay':overlay}
+    if overlay:
+        BUILD_CONTEXT['check_bypassed'] = 'explicit_workspace_snapshot'
     try:
         if args.action not in ['doctor','bundle']:
-            check()
+            if overlay:
+                print('WARNING: --workspace-overlay used; bypassing pinned-source check() for explicit workspace snapshot', file=sys.stderr)
+            else:
+                check()
         if args.action=='doctor': doctor()
         elif args.action=='bench': bench()
         elif args.action=='rootfs': rootfs()
```