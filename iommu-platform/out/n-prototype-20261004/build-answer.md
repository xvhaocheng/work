## Proposed unified diff: `scripts/platform.py`

```diff
--- a/scripts/platform.py
+++ b/scripts/platform.py
@@ -5,6 +5,6 @@
 import hashlib
 import json
 import os
-from pathlib import Path
+from pathlib import Path, PurePosixPath
 import re
 import shutil
@@ -30,9 +30,49 @@
 def sha(path):
     with Path(path).open('rb') as f:
         digest = hashlib.sha256()
         for chunk in iter(lambda: f.read(1024 * 1024), b''):
             digest.update(chunk)
         return digest.hexdigest()
 
+def load_workspace_overlay(path):
+    data = json.loads(Path(path).read_text())
+    if not isinstance(data, list):
+        raise RuntimeError('--workspace-overlay must contain a JSON list')
+    overlay = []
+    seen = set()
+    root = ROOT.resolve()
+    for raw in data:
+        if not isinstance(raw, str):
+            raise RuntimeError('Workspace overlay entries must be strings')
+        rel = PurePosixPath(raw)
+        if rel.is_absolute() or raw.startswith(('/', '\\')):
+            raise RuntimeError(f'Workspace overlay path must be relative: {raw}')
+        parts = rel.parts
+        if not parts or any(part in ('', '.', '..') for part in parts):
+            raise RuntimeError(f'Workspace overlay path has empty or traversal segment: {raw}')
+        if parts[0] != 'cva6' or len(parts) < 2:
+            raise RuntimeError(f'Workspace overlay path must be under cva6/: {raw}')
+        if any(part.lower() == 'linux' for part in parts):
+            raise RuntimeError(f'Workspace overlay path must not reference Linux: {raw}')
+        rel_str = str(rel)
+        if rel_str in seen:
+            raise RuntimeError(f'Duplicate workspace overlay path: {raw}')
+        seen.add(rel_str)
+        source = ROOT / rel
+        if source.is_symlink():
+            raise RuntimeError(f'Workspace overlay source must not be a symlink: {raw}')
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
+        overlay.append({'path': rel_str, 'sha256': sha(source), 'size': source.stat().st_size})
+    return overlay
+
 def records():
     return json.loads((ROOT / 'sources.lock.json').read_text())['components']
@@ -140,9 +140,23 @@
         if not (member.isfile() or member.isdir() or member.issym() or member.islnk()):
             raise ValueError(f'Unsupported archive member: {member.name}')
         return tarfile.tar_filter(member, directory)
     with tarfile.open(fileobj=proc.stdout, mode='r|') as archive:
         archive.extractall(dest, filter=source_filter)
     if proc.wait():
         raise RuntimeError(f"Archive failed: {item['path']}")
 
+def copy_workspace_overlay(stage):
+    for item in BUILD_CONTEXT.get('workspace_overlay', []):
+        rel = PurePosixPath(item['path'])
+        source = ROOT / rel
+        dest = stage / rel
+        if dest.is_symlink():
+            raise RuntimeError(f'Overlay destination is a symlink: {dest}')
+        if dest.exists() and dest.is_dir():
+            raise RuntimeError(f'Overlay destination is an existing directory: {dest}')
+        dest.parent.mkdir(parents=True, exist_ok=True)
+        shutil.copy2(source, dest)
+        if not dest.is_file() or dest.is_symlink():
+            raise RuntimeError(f'Overlay copy did not produce a regular file: {dest}')
+
 def fpga(profile):
@@ -157,6 +157,7 @@
     for item in records():
         if item['path'] == 'cva6' or item['path'].startswith('cva6/'):
             stage_repo(item, stage/item['path'])
+    copy_workspace_overlay(stage)
     cva6 = stage/'cva6'
     top = cva6/'corev_apu/fpga/src/ariane_xilinx.sv'
     data = top.read_text()
@@ -162,16 +162,27 @@
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
@@ -258,5 +258,8 @@
     parser=argparse.ArgumentParser()
     parser.add_argument('action',choices=['doctor','check','bench','rootfs','kernel','firmware','software','hpm-test','fpga','bundle'])
-    parser.add_argument('--profile',default='migration-4dma',choices=['migration-4dma','baseline-4dma','migration-4dma-scfix','migration-4dma-observe','migration-4dma-batch','baseline-4dma-batch','migration-4dma-path','migration-4dma-e3','migration-4dma-e3-queue','migration-4dma-e3-controlled','migration-4dma-p4m','migration-4dma-research-v1','migration-4dma-research-e2-v2','migration-4dma-research-method-v2'])
+    parser.add_argument('--profile',default='migration-4dma',choices=['migration-4dma','baseline-4dma','migration-4dma-scfix','migration-4dma-observe','migration-4dma-batch','baseline-4dma-batch','migration-4dma-path','migration-4dma-e3','migration-4dma-e3-queue','migration-4dma-e3-controlled','migration-4dma-p4m','migration-4dma-research-v1','migration-4dma-research-e2-v2','migration-4dma-research-method-v2','migration-4dma-n-serial','migration-4dma-n-hit'])
+    parser.add_argument('--workspace-overlay', type=Path, default=None)
     parser.add_argument('--jobs',type=int,default=12)
     args=parser.parse_args()
+    if args.workspace_overlay is not None and args.action != 'fpga':
+        parser.error('--workspace-overlay is only valid with the fpga action')
@@ -263,8 +263,14 @@
     BUILD_CONTEXT = {'timestamp':datetime.now(timezone.utc).isoformat(),
                      'platform_commit':capture(['git','rev-parse','HEAD']),
                      'platform_dirty':bool(capture(['git','status','--porcelain'])),
                      'sources_lock_sha256':sha(ROOT/'sources.lock.json'),
-                     'profile_sha256':sha(ROOT/'configs'/f'{args.profile}.json')}
+                     'profile_sha256':sha(ROOT/'configs'/f'{args.profile}.json'),
+                     'workspace_overlay': load_workspace_overlay(args.workspace_overlay) if args.workspace_overlay is not None else []}
+    if args.workspace_overlay is not None:
+        BUILD_CONTEXT['check_bypassed'] = 'workspace_overlay'
     try:
         if args.action not in ['doctor','bundle']:
-            check()
+            if args.workspace_overlay is None:
+                check()
+            else:
+                print('WARN: strict check bypassed by --workspace-overlay; not claiming a clean work tree', flush=True)
```

## New complete configs

### `configs/migration-4dma-n-serial.json`

```json
{
  "description": "Four-DMA ordinary nonblocking baseline: serial nonblock traversal (not a novelty claim)",
  "board": "genesys2",
  "num_dma": 4,
  "device_ids": [10, 11, 12, 13],
  "iotlb_entries": 16,
  "ds_delay_input": 50,
  "ds_delay_output": 50,
  "legacy_e3_observer": false,
  "research_role": 0,
  "nonblock_mode": 1,
  "cpu_hz": 50000000,
  "timebase_hz": 25000000
}
```

### `configs/migration-4dma-n-hit.json`

```json
{
  "description": "Four-DMA ordinary nonblocking baseline: hit-enabled nonblock traversal (not a novelty claim)",
  "board": "genesys2",
  "num_dma": 4,
  "device_ids": [10, 11, 12, 13],
  "iotlb_entries": 16,
  "ds_delay_input": 50,
  "ds_delay_output": 50,
  "legacy_e3_observer": false,
  "research_role": 0,
  "nonblock_mode": 2,
  "cpu_hz": 50000000,
  "timebase_hz": 25000000
}
```

## Brief documentation wording (separate from patch; no full validation claim)

- The real TL targeted test passed before the latest additions; the existing IOTLB 703-comparison set reports PASS.
- The wrapper has not yet passed; the missing test model is `r_resp`.
- New full-top integration is partial; board wiring remains under review.
- No FPGA build and no board run have been performed for these configurations.
- The two configurations share the same bank layout, cache, and capacities; they are serial vs hit-enabled ordinary nonblocking baselines, not a new novelty claim.
- Old cost/research metrics are invalid here.
- P4M timing/data verification is planned; the existing Linux `00093` BIT-only pilot may work, but new metadata is a later genuine Linux API gap.
- Do not cite this as full validation or as a completed experiment.

## Assumptions

- `--workspace-overlay` points to a JSON list of strings. Each string is a POSIX-style path relative to the platform root and must start with `cva6/`.
- The overlay source files are regular UTF-8 text files. Binary files, symlinks, absolute paths, `..` traversal, `linux` components, and non-`cva6` paths are rejected.
- The destination is always `stage/<overlay path>`, i.e. `out/<profile>/fpga-src/<overlay path>`.
- For nonzero `nonblock_mode`, the supplied top source must already contain `localparam int unsigned IOMMUNonblockMode = ...` and the port connection. The script rewrites only the numeric value. Old pinned tops that lack the parameter remain valid for `nonblock_mode == 0` because the parameter is not added to the rewrite list.
- `check()` is still the default for all old builds. It is bypassed only for `fpga` when `--workspace-overlay` is explicitly supplied, and that bypass is recorded as `check_bypassed = "workspace_overlay"`.

## Suggested acceptance checks

- `python3 -m py_compile scripts/platform.py`
- `python3 scripts/platform.py --help` shows `--workspace-overlay` and both new profile names.
- `python3 scripts/platform.py fpga --profile migration-4dma-n-serial --workspace-overlay bad.json` rejects absolute paths, `..`, symlinks, `linux` paths, non-`cva6` paths, existing directories, and binary/non-UTF-8 files.
- Running `fpga` with an overlay writes `workspace_overlay` records into `out/<profile>/fpga-src/build-inputs.json` and into the final manifest’s `build_start`.
- Running `fpga` without `--workspace-overlay` still calls `check()`.
- Running any non-`fpga` action with `--workspace-overlay` is rejected by argparse.
- `configs/migration-4dma-n-serial.json` and `configs/migration-4dma-n-hit.json` parse as JSON and satisfy the `nonblock_mode` profile constraints (`legacy_e3_observer=false`, `research_role=0`, `ds_delay_input=50`, `ds_delay_output=50`, `iotlb_entries=16`).

No tests are claimed here; the above are proposed checks for the parent to execute.