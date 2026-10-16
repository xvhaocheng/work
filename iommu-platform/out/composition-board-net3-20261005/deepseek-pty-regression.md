以下补丁只新增 `tests/test_mac_update_reboot.py` 中的有界回归测试，不修改生产逻辑。测试直接驱动真实的 `mac._run_ssh_with_password`，使用真实子进程和 PTY；子进程输出 `Password:`、读取虚拟口令、关闭 fd 0/1/2、短暂 sleep 后再退出，从而确定性地制造“master 先收到 EIO/EOF，child 退出状态稍后才到”的顺序。修复前该测试会得到 `UpdateError: ssh password command timed out`；修复后应返回真实 `returncode`。

```diff
diff --git a/tests/test_mac_update_reboot.py b/tests/test_mac_update_reboot.py
--- a/tests/test_mac_update_reboot.py
+++ b/tests/test_mac_update_reboot.py
@@ -674,6 +674,47 @@ class MacUpdateRebootTests(unittest.TestCase):
         proc = Stubborn()
         mac._stop_process(proc, timeout=0.01)
         self.assertTrue(proc.killed)
 
+    def _password_pty_child_argv(self, returncode):
+        """Real child: prompt, consume the dummy password, drop the PTY, delay."""
+        child = '\n'.join([
+            'import os, signal, sys, time, tty',
+            'signal.signal(signal.SIGHUP, signal.SIG_IGN)',
+            'tty.setraw(0)',
+            'sys.stdout.write("Password: ")',
+            'sys.stdout.flush()',
+            'password = b"dummy-password\\n"',
+            'received = b""',
+            'while len(received) < len(password):',
+            '    chunk = os.read(0, len(password) - len(received))',
+            '    if not chunk:',
+            '        break',
+            '    received += chunk',
+            'for fd in (0, 1, 2):',
+            '    try:',
+            '        os.close(fd)',
+            '    except OSError:',
+            '        pass',
+            'time.sleep(1.0)',
+            'os._exit(int(sys.argv[1]))',
+        ])
+        return [sys.executable, '-c', child, str(returncode)]
+
+    def _run_password_pty_child(self, returncode):
+        result = mac._run_ssh_with_password(
+            self._password_pty_child_argv(returncode),
+            b'dummy-password\n',
+            10.0)
+        self.assertIn(b'Password:', result.stdout)
+        return result
+
+    def test_password_pty_eof_before_exit_is_not_reported_as_timeout(self):
+        result = self._run_password_pty_child(0)
+        self.assertEqual(result.returncode, 0)
+
+    def test_password_pty_eof_before_nonzero_exit_preserves_returncode(self):
+        result = self._run_password_pty_child(7)
+        self.assertEqual(result.returncode, 7)
+
     def test_invalid_timeouts_are_refused_without_actions(self):
         for value in ('nan', 'inf', '-1', '0'):
             out_dir = self.tmp / ('bad-timeout-' + value.replace('-', 'neg'))
```

运行命令建议：

```bash
# 只跑新增的两个回归测试
python3 tests/test_mac_update_reboot.py -v -k password_pty

# 或跑整个测试文件
python3 tests/test_mac_update_reboot.py -v
```

如果项目把 `tests` 当作包运行，也可使用：

```bash
python3 -m unittest discover -s tests -p 'test_mac_update_reboot.py' -v -k password_pty
```

假设：

- Codex 已按诊断方案修改 `_run_ssh_with_password`：在 `os.read` 收到 `EIO` 或 EOF 后，若 `proc.poll()` 仍为 `None`，在原始 deadline 的剩余时间内 `proc.wait`；只有真正的 `subprocess.TimeoutExpired` 才报 `ssh password command timed out`。
- 测试只使用真实 PTY + 真实 Python 子进程，不启动 `ssh`，不访问网络、Mac、板卡或实际凭据；口令是字面量 `b'dummy-password\n'`。
- 子进程忽略 `SIGHUP`，避免关闭控制终端时提前被挂断信号终止，从而保证 sleep 期间父进程仍能看到 `proc.poll() is None`。
- 测试要求 POSIX PTY 语义；项目本身已无条件导入 `pty`/`termios`，因此 Linux/macOS 兼容，Windows 不在目标内。
- 子进程 sleep 1.0 秒、父进程超时 10.0 秒，用于稳定让 `EIO`/EOF 先于退出状态到达；实际板卡 9 秒误报场景不需要在单测中复现。

限制：

- 该测试直接调用私有函数 `mac._run_ssh_with_password`，而不是走完整 `main()`；这是为了使用真实 PTY 且完全不接触 `ssh`/网络。完整 runner 的编排早有 fake ssh 测试覆盖。
- 如果某平台在 slave fd 全关后返回 `b''` 而不是 `EIO`，测试仍覆盖同一代码路径，因为生产代码对 `not chunk` 和 `errno.EIO` 都走 break。
- 测试断言的是可观察行为：不误报 timeout、`returncode` 正确；不镜像实现内部的 mock 调用顺序。
- 若 CI 极慢导致 Python 子进程启动超过 10 秒，可把 `_run_password_pty_child` 中的 `10.0` 调大，但这会略微增加最坏用例时长。