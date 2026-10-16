以下补丁仅改 `scripts/mac_composition.py` 的 SSH console 生命周期与 `_run_framed`，并为其新增 2 个必要测试；UART 路径、计划/验收、framer/password 注入逻辑均不变。未执行任何测试。

```diff
diff --git a/scripts/mac_composition.py b/scripts/mac_composition.py
--- a/scripts/mac_composition.py
+++ b/scripts/mac_composition.py
@@ -27,5 +27,6 @@
 import secrets
 import shlex
 import subprocess
 import sys
+import tempfile
 import time
@@ -156,16 +157,65 @@
         def __init__(self, raw_log_path, events_log_path, board_host,
                      credential_response):
             super().__init__(None, raw_log_path, events_log_path)
             self.board_host = board_host
             self._credential_response = credential_response
+            self._ssh_dir = None
+            self._control_path = None
 
-        def start(self, timeout=10.0):
-            self.raw_log_path.parent.mkdir(parents=True, exist_ok=True)
-            self.events_log_path.parent.mkdir(parents=True, exist_ok=True)
-            self._log_event('ssh console start host=' + str(self.board_host))
-
-        def close(self):
-            self._stop.set()
+        def _cleanup_ssh_dir(self):
+            directory = self._ssh_dir
+            self._ssh_dir = None
+            self._control_path = None
+            if directory is None:
+                return
+            try:
+                directory.cleanup()
+            except Exception:
+                self._log_event('ssh control dir cleanup failed')
+
+        def _ensure_ssh_dir(self):
+            if self._ssh_dir is not None:
+                return
+            directory = tempfile.TemporaryDirectory(dir='/tmp', prefix='comp-ssh-')
+            self._ssh_dir = directory
+            self._control_path = pathlib.Path(directory.name) / 's'
+            try:
+                pathlib.Path(directory.name).chmod(0o700)
+            except OSError:
+                self._log_event('ssh control dir chmod failed')
+
+        def _append_raw(self, data):
+            if not data:
+                return
+            self.raw_log_path.parent.mkdir(parents=True, exist_ok=True)
+            with open(self.raw_log_path, 'ab') as handle:
+                handle.write(data)
+                handle.flush()
+
+        def start(self, timeout=10.0):
+            self.raw_log_path.parent.mkdir(parents=True, exist_ok=True)
+            self.events_log_path.parent.mkdir(parents=True, exist_ok=True)
+            self._ensure_ssh_dir()
+            self._log_event('ssh console start host=' + str(self.board_host))
+
+        def close(self):
+            self._stop.set()
+            path = self._control_path
+            try:
+                if path is not None and path.exists():
+                    argv = ['ssh', '-S', str(path), '-O', 'exit',
+                            'root@' + str(self.board_host)]
+                    try:
+                        proc = subprocess.run(argv, timeout=5.0, check=False,
+                                              capture_output=True)
+                        if getattr(proc, 'returncode', None) != 0:
+                            self._log_event('ssh master close exit='
+                                            + str(proc.returncode))
+                    except Exception as exc:
+                        self._log_event('ssh master close failed: '
+                                        + type(exc).__name__)
+            finally:
+                self._cleanup_ssh_dir()
 
         def disable_echo(self):
             return
@@ -176,10 +226,17 @@
         def _run_framed(self, full_command, begin, end, timeout):
+            self._ensure_ssh_dir()
             argv = build_ssh_argv(key=None, password_mode=True,
                                   board_host=self.board_host,
                                   remote_command=full_command)
+            argv = list(argv[:-2]) + [
+                '-o', 'ControlMaster=auto',
+                '-o', 'ControlPersist=60',
+                '-o', 'ControlPath=' + str(self._control_path),
+            ] + list(argv[-2:])
             self._log_event('ssh send host=' + str(self.board_host))
             proc = run_ssh(argv, timeout, self._credential_response)
             transcript = bytes(getattr(proc, 'stdout', b'') or b'')
+            self._append_raw(transcript)
             returncode = getattr(proc, 'returncode', None)
             if returncode != 0:
                 raise CompositionError('ssh transport failed rc=' + str(returncode))
diff --git a/tests/test_mac_composition_ssh.py b/tests/test_mac_composition_ssh.py
--- a/tests/test_mac_composition_ssh.py
+++ b/tests/test_mac_composition_ssh.py
@@ -124,6 +124,8 @@
         ssh_module = types.SimpleNamespace(
             build_ssh_argv=helper.build_ssh_argv,
             default_ssh_runner=runner)
         console_cls = mac.build_ssh_console_class(mac_research2, ssh_module)
         raw = self.tmp / ('console-%d.raw.log' % self._counter)
         events = self.tmp / ('console-%d.events.log' % self._counter)
-        return console_cls(raw, events, BOARD_HOST, CREDENTIAL)
+        console = console_cls(raw, events, BOARD_HOST, CREDENTIAL)
+        self.addCleanup(console.close)
+        return console
@@ -168,6 +170,29 @@
         self.assertIsNone(console.disable_echo())
         self.assertEqual(console.drain_input(), b'')
         console.close()
         self.assertTrue(console._stop.is_set())
 
+    def test_ssh_console_owns_and_cleans_its_control_dir(self):
+        console = self.build_console(RecordingRunner(b''))
+        console.start()
+        control_path = console._control_path
+        self.assertIsNotNone(control_path)
+        self.assertEqual(control_path.name, 's')
+        control_dir = control_path.parent
+        self.assertTrue(control_dir.is_dir())
+        self.assertEqual(control_dir.stat().st_mode & 0o777, 0o700)
+        self.assertTrue(str(control_dir).startswith('/tmp/comp-ssh-'))
+        # A stand-in for the Unix socket: close must target only this path.
+        control_path.write_bytes(b'')
+        with mock.patch.object(mac.subprocess, 'run') as run:
+            run.return_value = types.SimpleNamespace(returncode=0)
+            console.close()
+        self.assertFalse(control_path.exists())
+        self.assertEqual(run.call_count, 1)
+        argv = run.call_args[0][0]
+        self.assertEqual(argv, ['ssh', '-S', str(control_path), '-O', 'exit',
+                                'root@' + BOARD_HOST])
+        self.assertEqual(run.call_args[1]['timeout'], 5.0)
+        self.assertFalse(run.call_args[1]['check'])
+
     # -- banner / password prompt / CRLF stripping --------------------------
@@ -232,7 +257,18 @@
     def test_ssh_exit_marker_without_line_end_fails(self):
         transcript = b'__R2B_a_b__\r\nout\r\n__R2E_a_b__0'
         console = self.build_console(RecordingRunner(transcript))
         with self.assertRaises(mac.CompositionError):
             console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)
 
+    def test_ssh_failure_transcript_is_saved_to_raw_log(self):
+        transcript = (b"root@board.local's password: \r\n"
+                      b"Permission denied (publickey,password).\r\n")
+        console = self.build_console(RecordingRunner(transcript, returncode=255))
+        console.start()
+        with self.assertRaises(mac.CompositionError):
+            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)
+        raw = console.raw_log_path.read_bytes()
+        self.assertIn(transcript, raw)
+        self.assertNotIn(CREDENTIAL, raw)
+
     # -- main() entry: ssh never touches UART / serial ----------------------
```

假设与边界：
- `mac_research2.SerialConsole` 已提供 `_log_event`、`_stop`、`raw_log_path`、`events_log_path`，当前文件已经这样使用。
- `helper.build_ssh_argv` 返回值保持 `..., 'root@host', remote_command` 的形态，因此补丁用 `argv[:-2] + options + argv[-2:]` 在 host/command 前插入三个 `-o`。
- `close` 只在 `_control_path` 存在且 `path.exists()` 时发送 `ssh -S <own path> -O exit root@host`；不会扫描或关闭其它连接。若 master 不存在或已过期，socket 不存在，则只做清理。
- 原始 transcript 在检查 `returncode` 前写入 `raw_log_path`；helper 已在写入口令前禁 ECHO，因此 raw 中可能出现 `password:` prompt，但不会出现真实口令。`close` 只记录 returncode 或异常类型，不记录 argv/stdout/stderr。

风险：
- 首次使用 `ControlMaster=auto` + `ControlPersist=60` 时，OpenSSH 可能让 master 进程在后台存活；现有 PTY runner 关闭 master_fd 时理论上可能影响后台 master。该风险需要在真实非生产连接上验证 master 复用与 `-O exit` 清理是否正常。
- `close` 的 `path.exists()` 是保护性短路；如果 OpenSSH 在异常情况下移除了 socket 但 master 仍存活，则不会被本进程关闭，只能依赖 `ControlPersist=60` idle 退出。
- 补丁行号基于当前提供的内容；若实际文件已被改动，建议用 `git apply --recount` 或先 `git apply --check`。

建议验收命令（未执行）：
```bash
python3 -m py_compile scripts/mac_composition.py tests/test_mac_composition_ssh.py
python3 -m unittest tests.test_mac_composition_ssh
# 或
python3 tests/test_mac_composition_ssh.py
```

真实 SSH 路径验收（需操作者提供真实 board/credential，且不写入本补丁测试）：
```bash
python3 scripts/mac_composition.py \
  --out /tmp/comp-ssh-run \
  --expected-kernel '<kernel>' \
  --run --transport ssh \
  --board-host '<board>' \
  --credential-script '<credential script>' \
  --ssh-helper scripts/mac_update_reboot.py \
  --through-phase short
```
检查：
- 首个 SSH 命令后不再重复出现口令提示。
- `console.raw.log` 含原始 transcript，但不含 credential bytes。
- `console.events.log` 只在 close 失败时出现简短 `ssh master close ...` 事件。
- 运行结束后 `/tmp/comp-ssh-*` 被清理。
- 原有 UART 测试与 13 个初稿测试保持通过。