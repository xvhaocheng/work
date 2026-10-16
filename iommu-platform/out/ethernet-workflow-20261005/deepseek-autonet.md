以下为只读分析后的可应用提案；我未执行任何命令，也未声称测试通过。补丁基于你提供的快照编写；若工作区中 `configs/board-network.interfaces`、`configs/board-dhcpcd.conf` 已存在且含其他脏修改，不要使用 `/dev/null` 段覆盖，只把对应新增行追加进去。

## 证据与现状

- `scripts/rootfs.py:25-27`：`DATA` 只有 `etc/platform-research-plan.tsv`，且 `scripts/rootfs.py:79`、`:101` 把所有 `DATA` 固定为 `0644`。因此不能把 `authorized_keys` 放进去。
- `scripts/rootfs.py:58-108`：repack 已保留设备节点、跳过 `LEGACY_NAMES`、按 basename 替换 benchmark、按 path 替换 data，并在写输出前做去重和模式校验。当前没有目录项、没有 `0600` 受保护数据、没有 SSH key 校验。
- `scripts/rootfs.py:110`：当前入口无参数。
- `scripts/build_update_software.py:24-27`：`BASE/DELIVERY/PREVIOUS/STATE_PATH` 硬编码 `out/software-update-20261004`。
- `scripts/build_update_software.py:29`：`LOCAL_VERSION = '-cpn2-net1'`。
- `scripts/build_update_software.py:51-60`：`DELIVERY_SOURCES` 当前不含静态网络配置和公开 SSH key。
- `scripts/build_update_software.py:62-92`：`INPUT_PATTERNS` 当前不含 `configs/board-network.interfaces`、`configs/board-dhcpcd.conf`、`configs/authorized_keys`。
- `scripts/build_update_software.py:112-116`、`:128-148`、`:215-225`、`:228-270`：状态/previous/delivery/main 都直接引用全局硬编码路径。
- `tests/test_rootfs.py:27-34`：测试 fixture 从 `EXECUTABLES` 和 `DATA` 生成；新增 `PROTECTED_DATA` / `DIRECTORIES` 后需要 fixture 和断言更新。
- `tests/test_rootfs.py:57-88`、`:187-196`：需要把新目录和新受保护文件纳入去重、模式、精确集合断言。
- `out/ethernet-workflow-20261005/base-network-init.txt:52-65`：Buildroot `S40network` 执行 `/sbin/ifup -a`。
- `out/ethernet-workflow-20261005/base-network-init.txt:85-100`：`S41dhcpcd` 用 `dhcpcd -f /etc/dhcpcd.conf` 启动。
- `out/ethernet-workflow-20261005/base-network-init.txt:151-155`：当前 `etc/network/interfaces` 只有 `lo`，没有 `eth0` 静态配置。
- `out/ethernet-workflow-20261005/base-network-init.txt:1-49`：当前 `etc/dhcpcd.conf` 没有 `denyinterfaces eth0`。

## 统一 diff

```diff
diff --git a/scripts/rootfs.py b/scripts/rootfs.py
--- a/scripts/rootfs.py
+++ b/scripts/rootfs.py
@@ -25,3 +25,21 @@
 DATA={
     'etc/platform-research-plan.tsv': 'tests/research-plan.tsv',
+    'etc/network/interfaces': 'configs/board-network.interfaces',
+    'etc/dhcpcd.conf': 'configs/board-dhcpcd.conf',
 }
+
+DIRECTORIES={
+    'root/.ssh': 0o700,
+}
+
+PROTECTED_DATA={
+    'root/.ssh/authorized_keys': 'configs/authorized_keys',
+}
+
+PUBLIC_KEY_PREFIXES=(
+    'ssh-ed25519',
+    'ssh-rsa',
+    'ecdsa-sha2-',
+    'sk-ssh-ed25519@openssh.com',
+    'sk-ecdsa-sha2-',
+)
@@ -53,5 +71,27 @@
     out.extend(b'070701'+''.join(f'{v:08x}' for v in fields).encode()+encoded)
     out.extend(b'\0'*((-len(out))%4))
     out.extend(body)
     out.extend(b'\0'*((-len(out))%4))
 
+def validate_authorized_keys(path, body):
+    if not body:
+        raise ValueError(f'Empty authorized_keys file: {path}')
+    try:
+        text = body.decode('utf-8')
+    except UnicodeDecodeError as exc:
+        raise ValueError(f'authorized_keys is not UTF-8 text: {path}') from exc
+    lines = [line.strip() for line in text.splitlines() if line.strip() and not line.lstrip().startswith('#')]
+    if not lines:
+        raise ValueError(f'authorized_keys has no public key: {path}')
+    valid = False
+    for line in lines:
+        if 'PRIVATE KEY' in line or 'BEGIN ' in line:
+            raise ValueError(f'authorized_keys contains private key material: {path}')
+        fields = line.split()
+        if len(fields) < 2:
+            raise ValueError(f'authorized_keys line is not a public key: {path}')
+        if any(field.startswith(PUBLIC_KEY_PREFIXES) for field in fields):
+            valid = True
+    if not valid:
+        raise ValueError(f'authorized_keys has no supported public key: {path}')
+
@@ -58,53 +98,91 @@
-def repack(root=ROOT):
-    entries=list(read_newc((root/'assets/rootfs-base.cpio').read_bytes()))
-    binaries={name:(root/path).read_bytes() for name,path in EXECUTABLES.items()}
-    data={path:(root/source).read_bytes() for path,source in DATA.items()}
-    output=bytearray()
-    ino=max(f[0] for _,f,_ in entries)+1
-    # Remove every prior copy, including root-level and duplicate managed entries.
-    replaced_names=LEGACY_NAMES | EXECUTABLES.keys()
-    replaced_data_paths={str(Path(path)) for path in DATA}
-    for name,fields,body in entries:
-        normalized=str(Path(name))
-        if Path(name).name in replaced_names:
-            continue
-        if normalized in replaced_data_paths:
-            continue
-        write_entry(output,name,fields,body)
-    for name,body in binaries.items():
-        fields=[ino,stat.S_IFREG|0o755,0,0,1,0,0,0,0,0,0,0,0]
-        write_entry(output,'usr/bin/'+name,fields,body)
-        ino+=1
-    for path,body in data.items():
-        fields=[ino,stat.S_IFREG|0o644,0,0,1,0,0,0,0,0,0,0,0]
-        write_entry(output,path,fields,body)
-        ino+=1
-    write_entry(output,'TRAILER!!!',[0]*13,b'')
-    output.extend(b'\0'*((-len(output))%512))
-    # Validate before replacing the output. Path normalizes leading './' aliases.
-    packed={}
-    for name,fields,body in read_newc(output):
-        normalized=str(Path(name))
-        if normalized in packed:
-            raise ValueError(f'Duplicate rootfs entry: {normalized}')
-        if Path(name).name in LEGACY_NAMES:
-            raise ValueError(f'Legacy benchmark in rootfs: {name}')
-        packed[normalized]=(fields,body)
-    for name,body in binaries.items():
-        actual_fields,actual_body=packed['usr/bin/'+name]
-        if actual_body != body or actual_fields[1] != (stat.S_IFREG|0o755):
-            raise ValueError(f'Executable verification failed: {name}')
-        if sum(Path(path).name == name for path in packed) != 1:
-            raise ValueError(f'Duplicate executable in rootfs: {name}')
-    for path,body in data.items():
-        actual_fields,actual_body=packed[path]
-        if actual_body != body or actual_fields[1] != (stat.S_IFREG|0o644):
-            raise ValueError(f'Data verification failed: {path}')
-        if sum(normalized == path for normalized in packed) != 1:
-            raise ValueError(f'Duplicate data in rootfs: {path}')
-    dest=root/'out/rootfs.cpio'
-    dest.parent.mkdir(parents=True,exist_ok=True)
-    dest.write_bytes(output)
-    print('PASS: rootfs contains current experiment/smoke/watchdog; device nodes preserved')
-
-if __name__=='__main__': repack()
+def repack(root=ROOT, authorized_keys=None):
+    entries=list(read_newc((root/'assets/rootfs-base.cpio').read_bytes()))
+    binaries={name:(root/path).read_bytes() for name,path in EXECUTABLES.items()}
+    data={path:(root/source).read_bytes() for path,source in DATA.items()}
+    protected={}
+    for path,source in PROTECTED_DATA.items():
+        source_path = root/source
+        if authorized_keys is not None:
+            source_path = Path(authorized_keys)
+            if not source_path.is_absolute():
+                source_path = root / source_path
+        body=source_path.read_bytes()
+        validate_authorized_keys(source_path, body)
+        protected[path]=body
+    output=bytearray()
+    ino=max(f[0] for _,f,_ in entries)+1
+    # Remove every prior copy, including root-level and duplicate managed entries.
+    replaced_names=LEGACY_NAMES | EXECUTABLES.keys()
+    replaced_data_paths={str(Path(path)) for path in DATA}
+    replaced_data_paths |= {str(Path(path)) for path in PROTECTED_DATA}
+    replaced_data_paths |= {str(Path(path)) for path in DIRECTORIES}
+    for name,fields,body in entries:
+        normalized=str(Path(name))
+        if Path(name).name in replaced_names:
+            continue
+        if normalized in replaced_data_paths:
+            continue
+        write_entry(output,name,fields,body)
+    for path,mode in DIRECTORIES.items():
+        fields=[ino,stat.S_IFDIR|mode,0,0,2,0,0,0,0,0,0,0,0]
+        write_entry(output,path,fields,b'')
+        ino+=1
+    for name,body in binaries.items():
+        fields=[ino,stat.S_IFREG|0o755,0,0,1,0,0,0,0,0,0,0,0]
+        write_entry(output,'usr/bin/'+name,fields,body)
+        ino+=1
+    for path,body in data.items():
+        fields=[ino,stat.S_IFREG|0o644,0,0,1,0,0,0,0,0,0,0,0]
+        write_entry(output,path,fields,body)
+        ino+=1
+    for path,body in protected.items():
+        fields=[ino,stat.S_IFREG|0o600,0,0,1,0,0,0,0,0,0,0,0]
+        write_entry(output,path,fields,body)
+        ino+=1
+    write_entry(output,'TRAILER!!!',[0]*13,b'')
+    output.extend(b'\0'*((-len(output))%512))
+    # Validate before replacing the output. Path normalizes leading './' aliases.
+    packed={}
+    for name,fields,body in read_newc(output):
+        normalized=str(Path(name))
+        if normalized in packed:
+            raise ValueError(f'Duplicate rootfs entry: {normalized}')
+        if Path(name).name in LEGACY_NAMES:
+            raise ValueError(f'Legacy benchmark in rootfs: {name}')
+        packed[normalized]=(fields,body)
+    for name,body in binaries.items():
+        actual_fields,actual_body=packed['usr/bin/'+name]
+        if actual_body != body or actual_fields[1] != (stat.S_IFREG|0o755):
+            raise ValueError(f'Executable verification failed: {name}')
+        if sum(Path(path).name == name for path in packed) != 1:
+            raise ValueError(f'Duplicate executable in rootfs: {name}')
+    for path,body in data.items():
+        actual_fields,actual_body=packed[path]
+        if actual_body != body or actual_fields[1] != (stat.S_IFREG|0o644):
+            raise ValueError(f'Data verification failed: {path}')
+        if sum(normalized == path for normalized in packed) != 1:
+            raise ValueError(f'Duplicate data in rootfs: {path}')
+    for path,mode in DIRECTORIES.items():
+        actual_fields,actual_body=packed[path]
+        if actual_body != b'' or actual_fields[1] != (stat.S_IFDIR|mode):
+            raise ValueError(f'Directory verification failed: {path}')
+        if sum(normalized == path for normalized in packed) != 1:
+            raise ValueError(f'Duplicate directory in rootfs: {path}')
+    for path,body in protected.items():
+        actual_fields,actual_body=packed[path]
+        if actual_body != body or actual_fields[1] != (stat.S_IFREG|0o600):
+            raise ValueError(f'Protected data verification failed: {path}')
+        if sum(normalized == path for normalized in packed) != 1:
+            raise ValueError(f'Duplicate protected data in rootfs: {path}')
+    dest=root/'out/rootfs.cpio'
+    dest.parent.mkdir(parents=True,exist_ok=True)
+    dest.write_bytes(output)
+    print('PASS: rootfs contains current experiment/smoke/watchdog; static network/SSH key; device nodes preserved')
+
+if __name__=='__main__':
+    import argparse
+    parser=argparse.ArgumentParser(description=__doc__)
+    parser.add_argument('--authorized-keys', default=None,
+                        help='public authorized_keys file (default: configs/authorized_keys)')
+    args=parser.parse_args()
+    repack(authorized_keys=args.authorized_keys)
diff --git a/tests/test_rootfs.py b/tests/test_rootfs.py
--- a/tests/test_rootfs.py
+++ b/tests/test_rootfs.py
@@ -31,4 +31,8 @@
         for packed_path, source in rootfs.DATA.items():
             path = self.root / source
             path.parent.mkdir(parents=True, exist_ok=True)
             path.write_bytes(('fixture for ' + packed_path).encode())
+        for packed_path, source in rootfs.PROTECTED_DATA.items():
+            path = self.root / source
+            path.parent.mkdir(parents=True, exist_ok=True)
+            path.write_bytes(b'ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAITestFixtureKeyForRootfsPackaging root@test\n')
@@ -61,8 +65,12 @@
         for packed_path in rootfs.DATA:
             parent, _, leaf = packed_path.rpartition('/')
             names += [packed_path, f'./{packed_path}']
             if parent:
                 names.append(f'{parent}/./{leaf}')
             else:
                 names.append(f'././{leaf}')
+        for packed_path in rootfs.PROTECTED_DATA:
+            names += [packed_path, f'./{packed_path}']
+        for packed_path in rootfs.DIRECTORIES:
+            names += [packed_path, f'./{packed_path}']
         self.base([(name, self.file, b'old copy') for name in names])
@@ -72,12 +80,20 @@
         expected = {'dev/idma0', 'etc/banner'}
         expected |= {f'usr/bin/{name}' for name in rootfs.EXECUTABLES}
         expected |= set(rootfs.DATA)
+        expected |= set(rootfs.DIRECTORIES)
+        expected |= set(rootfs.PROTECTED_DATA)
         self.assertEqual(set(packed), expected)
         for name, source in rootfs.EXECUTABLES.items():
             fields, body = packed['usr/bin/' + name]
             self.assertEqual(fields[1], stat.S_IFREG | 0o755)
             self.assertEqual(body, (self.root / source).read_bytes())
         for path, source in rootfs.DATA.items():
             fields, body = packed[path]
             self.assertEqual(fields[1], stat.S_IFREG | 0o644)
             self.assertEqual(body, (self.root / source).read_bytes())
+        for path, source in rootfs.PROTECTED_DATA.items():
+            fields, body = packed[path]
+            self.assertEqual(fields[1], stat.S_IFREG | 0o600)
+            self.assertEqual(body, (self.root / source).read_bytes())
+        for path, mode in rootfs.DIRECTORIES.items():
+            fields, body = packed[path]
+            self.assertEqual(fields[1], stat.S_IFDIR | mode)
+            self.assertEqual(body, b'')
@@ -193,8 +209,30 @@
         for name in rootfs.EXECUTABLES:
             self.assertEqual(sum(Path(path).name == name for path, _, _ in entries), 1)
         for path in rootfs.DATA:
             self.assertEqual(sum(str(Path(name)) == path for name, _, _ in entries), 1)
+        for path in rootfs.PROTECTED_DATA:
+            self.assertEqual(sum(str(Path(name)) == path for name, _, _ in entries), 1)
+        for path in rootfs.DIRECTORIES:
+            self.assertEqual(sum(str(Path(name)) == path for name, _, _ in entries), 1)
+
+    def test_empty_authorized_keys_rejected_before_output_replaced(self):
+        self.base()
+        output = self.root / 'out/rootfs.cpio'
+        output.write_bytes(b'previous rootfs')
+        (self.root / 'configs/authorized_keys').write_bytes(b'')
+        with self.assertRaisesRegex(ValueError, 'authorized_keys'):
+            self.pack()
+        self.assertEqual(output.read_bytes(), b'previous rootfs')
+
+    def test_missing_authorized_keys_preserves_existing_output(self):
+        self.base()
+        output = self.root / 'out/rootfs.cpio'
+        output.write_bytes(b'previous rootfs')
+        (self.root / 'configs/authorized_keys').unlink()
+        with self.assertRaises(FileNotFoundError):
+            self.pack()
+        self.assertEqual(output.read_bytes(), b'previous rootfs')
+
 
 if __name__ == '__main__':
     unittest.main()
diff --git a/scripts/build_update_software.py b/scripts/build_update_software.py
--- a/scripts/build_update_software.py
+++ b/scripts/build_update_software.py
@@ -14,7 +14,8 @@
 from __future__ import annotations
 
+import argparse
 import importlib.util
 import json
 import shutil
 from datetime import datetime, timezone
 from pathlib import Path
@@ -24,6 +25,7 @@
-BASE = ROOT / 'out' / 'software-update-20261004'
-DELIVERY = BASE / 'delivery'
-PREVIOUS = BASE / 'previous'
-STATE_PATH = BASE / 'software-update-state.json'
-
-LOCAL_VERSION = '-cpn2-net1'
+DEFAULT_OUTPUT_DIR = 'out/software-update-20261004'
+DEFAULT_LOCAL_VERSION = '-cpn2-net1'
+BASE = ROOT / DEFAULT_OUTPUT_DIR
+DELIVERY = BASE / 'delivery'
+PREVIOUS = BASE / 'previous'
+STATE_PATH = BASE / 'software-update-state.json'
+LOCAL_VERSION = DEFAULT_LOCAL_VERSION
+JOBS = 12
@@ -30,2 +32,11 @@
 JOBS = 12
+
+
+def resolve_base(output_dir):
+    base = Path(output_dir)
+    if not base.is_absolute():
+        base = ROOT / base
+    return base
+
+
 MAX_PAYLOAD = 33554432  # 32 MiB OpenSBI payload window
@@ -51,10 +62,13 @@
 DELIVERY_SOURCES = {
     'Image': KERNEL_IMAGE,
     'platform.dtb': PLATFORM_DTB,
     'rootfs.cpio': ROOTFS,
     'fw_payload.bin': FW_PAYLOAD,
     'platform-sd-update': SD_TOOL,
     'linux.config': KERNEL_CONFIG,
     'kernel-release': KERNEL_RELEASE,
     'mac_update_reboot.py': MAC_UPDATE_REBOOT,
+    'board-network.interfaces': ROOT / 'configs' / 'board-network.interfaces',
+    'board-dhcpcd.conf': ROOT / 'configs' / 'board-dhcpcd.conf',
+    'authorized_keys': ROOT / 'configs' / 'authorized_keys',
 }
@@ -84,2 +98,7 @@
     'rootscripts': ['scripts/rootfs.py'],
+    'network': [
+        'configs/board-network.interfaces',
+        'configs/board-dhcpcd.conf',
+    ],
+    'ssh_authorized_keys': ['configs/authorized_keys'],
     'board_helpers': [
@@ -112,5 +131,6 @@
-def write_state(state):
-    BASE.mkdir(parents=True, exist_ok=True)
-    tmp = STATE_PATH.with_suffix(STATE_PATH.suffix + '.tmp')
-    tmp.write_text(json.dumps(state, indent=2) + '\n')
-    tmp.replace(STATE_PATH)
+def write_state(state, base=BASE):
+    base.mkdir(parents=True, exist_ok=True)
+    state_path = base / 'software-update-state.json'
+    tmp = state_path.with_suffix(state_path.suffix + '.tmp')
+    tmp.write_text(json.dumps(state, indent=2) + '\n')
+    tmp.replace(state_path)
@@ -128,21 +148,22 @@
-def ensure_previous(sha):
-    manifest_path = PREVIOUS / 'previous-manifest.json'
-    if PREVIOUS.exists():
+def ensure_previous(sha, base=BASE):
+    previous = base / 'previous'
+    manifest_path = previous / 'previous-manifest.json'
+    if previous.exists():
         if manifest_path.is_file():
             return json.loads(manifest_path.read_text())
         return {'existing': True}
-    PREVIOUS.mkdir(parents=True, exist_ok=False)
+    previous.mkdir(parents=True, exist_ok=False)
     manifest = {}
     for name, source in PREVIOUS_SOURCES.items():
         entry = {'source': str(source), 'copied': False}
         if source.is_file() and source.stat().st_size:
-            destination = PREVIOUS / name
+            destination = previous / name
             shutil.copy2(source, destination)
             entry['copied'] = True
             entry['size'] = destination.stat().st_size
             entry['sha256'] = sha(destination)
         else:
             entry['missing'] = True
         manifest[name] = entry
     manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
     return manifest
@@ -215,11 +236,12 @@
-def deliver(sha):
-    DELIVERY.mkdir(parents=True, exist_ok=True)
+def deliver(sha, base=BASE):
+    delivery = base / 'delivery'
+    delivery.mkdir(parents=True, exist_ok=True)
     manifest = {}
     for name, source in DELIVERY_SOURCES.items():
         if not source.is_file() or source.stat().st_size == 0:
             raise RuntimeError('delivery artifact missing or empty: ' + str(source))
-        destination = DELIVERY / name
+        destination = delivery / name
         shutil.copy2(source, destination)
         manifest[name] = {'source': str(source), 'size': destination.stat().st_size,
                           'sha256': sha(destination)}
     return manifest
@@ -228,43 +250,52 @@
-def main():
-    BASE.mkdir(parents=True, exist_ok=True)
+def main(argv=None):
+    parser = argparse.ArgumentParser(description=__doc__)
+    parser.add_argument('--output-dir', default=DEFAULT_OUTPUT_DIR,
+                        help='delivery base directory (default: %(default)s)')
+    parser.add_argument('--local-version', default=DEFAULT_LOCAL_VERSION,
+                        help='kernel LOCALVERSION (default: %(default)s)')
+    args = parser.parse_args(argv)
+    base = resolve_base(args.output_dir)
+    local_version = args.local_version
+    base.mkdir(parents=True, exist_ok=True)
     state = {
         'started_at': utc_now(),
         'source_mode': 'authorized_working_tree',
         'check_bypassed': 'authorized_software_working_tree',
         'fpga': 'not_run',
-        'localversion': LOCAL_VERSION,
+        'localversion': local_version,
+        'base': str(base),
         'secrets_excluded': True,
         'status': 'running',
     }
-    write_state(state)
+    write_state(state, base)
     try:
         platform = load_platform()
-        platform.ENV['LOCALVERSION'] = LOCAL_VERSION
+        platform.ENV['LOCALVERSION'] = local_version
         state['platform_commit'] = platform.capture(['git', 'rev-parse', 'HEAD'])
         state['platform_dirty'] = bool(platform.capture(['git', 'status', '--porcelain']))
 
-        state['previous'] = ensure_previous(platform.sha)
+        state['previous'] = ensure_previous(platform.sha, base)
         state['inputs'] = capture_inputs(platform.sha)
 
         platform.kernel(JOBS)
         platform.firmware(JOBS)
 
         state['config'] = check_config(read_config())
         state['kernel_release'] = read_kernel_release()
         state['fw_payload_size'] = check_payload()
-        state['artifacts'] = deliver(platform.sha)
+        state['artifacts'] = deliver(platform.sha, base)
 
         state['status'] = 'complete'
         state['exit_code'] = 0
     except BaseException as exc:
         state['status'] = 'failed'
         state['exit_code'] = int(getattr(exc, 'returncode', None) or 1)
         state['error'] = type(exc).__name__ + ': ' + str(exc)
         raise
     finally:
         state['finished_at'] = utc_now()
-        write_state(state)
+        write_state(state, base)
 
 
 if __name__ == '__main__':
     main()
diff --git a/configs/board-network.interfaces b/configs/board-network.interfaces
new file mode 100644
--- /dev/null
+++ b/configs/board-network.interfaces
@@ -0,0 +1,10 @@
+# interface file auto-generated by buildroot
+
+auto lo
+iface lo inet loopback
+
+# Static link to the Mac helper. Keep it local: no gateway.
+auto eth0
+iface eth0 inet static
+    address 192.168.77.2
+    netmask 255.255.255.0
diff --git a/configs/board-dhcpcd.conf b/configs/board-dhcpcd.conf
new file mode 100644
--- /dev/null
+++ b/configs/board-dhcpcd.conf
@@ -0,0 +1,51 @@
+# A sample configuration for dhcpcd.
+# See dhcpcd.conf(5) for details.
+
+# Allow users of this group to interact with dhcpcd via the control socket.
+#controlgroup wheel
+
+# Inform the DHCP server of our hostname for DDNS.
+#hostname
+
+# Use the hardware address of the interface for the Client ID.
+#clientid
+# or
+# Use the same DUID + IAID as set in DHCPv6 for DHCPv4 ClientID as per RFC4361.
+# Some non-RFC compliant DHCP servers do not reply with this set.
+# In this case, comment out duid and enable clientid above.
+duid
+
+# Persist interface configuration when dhcpcd exits.
+persistent
+
+# Do not let DHCP touch the static Mac link.
+denyinterfaces eth0
+
+# vendorclassid is set to blank to avoid sending the default of
+# dhcpcd-<version>:<os>:<machine>:<platform>
+vendorclassid
+
+# A list of options to request from the DHCP server.
+option domain_name_servers, domain_name, domain_search
+option classless_static_routes
+# Respect the network MTU. This is applied to DHCP routes.
+option interface_mtu
+
+# Request a hostname from the network
+option host_name
+
+# Most distributions have NTP support.
+#option ntp_servers
+
+# Rapid commit support.
+# Safe to enable by default because it requires the equivalent option set
+# on the server to actually work.
+option rapid_commit
+
+# A ServerID is required by RFC2131.
+require dhcp_server_identifier
+
+# Generate SLAAC address using the Hardware Address of the interface
+#slaac hwaddr
+# OR generate Stable Private IPv6 Addresses based from the DUID
+slaac private
```

## 假设、接口与未验证项

- 假设 `configs/board-network.interfaces`、`configs/board-dhcpcd.conf` 当前不存在。若已存在，请勿应用 `/dev/null` 新文件段；只追加：
  - `configs/board-network.interfaces`：`auto eth0`、`iface eth0 inet static`、`address 192.168.77.2`、`netmask 255.255.255.0`，且不要加网关。
  - `configs/board-dhcpcd.conf`：`denyinterfaces eth0`。
- 未提供 `configs/authorized_keys`。接口固定为 `configs/authorized_keys`；也可用 `python3 scripts/rootfs.py --authorized-keys /path/to/authorized_keys` 覆盖。该文件必须是有效 SSH 公钥文本；不得放私钥、口令或空文件。空/缺失/无支持的 key 类型会在写 `out/rootfs.cpio` 前失败。
- `scripts/rootfs.py` 默认把 `root/.ssh` 打成 `0700` 目录、`root/.ssh/authorized_keys` 打成 `0600`，不会走 `DATA` 的 `0644`。
- builder 无参数默认仍是：
  - `--output-dir out/software-update-20261004`
  - `--local-version -cpn2-net1`
- 本批计划版本 `-cpn2-net3`，应由执行者显式传入，例如：
  - `python3 scripts/build_update_software.py --local-version=-cpn2-net3`
  - 如需单独输出目录，再加 `--output-dir=out/software-update-<date>`。
- `capture_inputs` 新增哈希 `configs/board-network.interfaces`、`configs/board-dhcpcd.conf`、`configs/authorized_keys`；`deliver` 会把这三个公开输入复制进 delivery 目录。
- 未修改 Linux 驱动、RTL、bench、`platform_sd_update.c`。
- 未执行任何测试或构建；以下验收需由执行者运行。

## 最小有意义验收

1. 公钥缺失/空文件负向：
   - 临时移走 `configs/authorized_keys` 或写入空文件；
   - 运行 `python3 scripts/rootfs.py`；
   - 应分别抛出 `FileNotFoundError` 或 `ValueError`，且不覆盖既有 `out/rootfs.cpio`。
2. 放置一个有效公开 `configs/authorized_keys` 后：
   - 运行 `python3 scripts/rootfs.py`；
   - 用 `scripts/rootfs.read_newc` 或测试断言检查：
     - `root/.ssh` 是 `stat.S_IFDIR | 0o700`；
     - `root/.ssh/authorized_keys` 是 `stat.S_IFREG | 0o600`；
     - `etc/network/interfaces` 与 `etc/dhcpcd.conf` 是 `0644`；
     - `dev/idma0` 仍保留；
     - 无重复项、无 `LEGACY_NAMES`。
3. 现有回归：
   - 运行 `python3 tests/test_rootfs.py`；
   - 应覆盖新增的 `PROTECTED_DATA`、`DIRECTORIES`、空/缺失公钥用例。
4. builder 接口：
   - 运行 `python3 scripts/build_update_software.py --help`，确认两个新选项和无参数默认值；
   - 实际构建时使用 `--local-version=-cpn2-net3`，并确认状态 JSON 中 `localversion` 为 `-cpn2-net3`，且 `inputs`/`artifacts` 含新静态配置和公开 key。