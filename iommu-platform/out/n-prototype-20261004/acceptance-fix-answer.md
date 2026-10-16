# Findings and proposed focused patches

Three files were named, but only **two pilot auxiliary scripts need changes**:

- `scripts/mac_nonblock_pilot.py` — three bounded fixes (login/password, seed‑stall silence, recover‑startup started‑case rejection).
- `out/n-prototype-20261004/analyze_nonblock_pilot.proposal.py` — qualified folding + direct record/identity validation.
- `scripts/mac_method88.py` — **no change**: `_wait_for_root` (lines 428‑457) already answers `login:` and `Password:` with `root`, using a `cursor` over new bytes and a 6‑attempt cap. It is the precedent to mirror, not to touch. (Its `attempts > 6` retry loop does *not* belong in the nonblock runner, which must be strict "one login + one Password, no guessing/retries".)

Everything below is a proposal; I could not execute anything. Line numbers refer to the numbered listings you supplied.

---

## Patch 1 — `scripts/mac_nonblock_pilot.py`

### 1a. New Password prompt regex

```diff
@@ -168,7 +168,8 @@
 # can never be mistaken for a prompt.
 ROOT_PROMPT_RE = re.compile(r'(?m)^#(?: |$)|^/ #|root@\S*#')
 LOGIN_PROMPT_RE = re.compile(r'login:\s*$')
+PASSWORD_PROMPT_RE = re.compile(r'Password:\s*$')
 
 _DQUOTE = chr(34)
 _B64_WS = frozenset(bytes([32, 9, 13, 10, 11, 12]))
 _INT_RE = re.compile(r'-?[0-9]+')
```

### 1b. `recover_startup`: reject any started case dir (manifest `cases=[]` is not proof)

```diff
@@ -750,6 +751,19 @@
             raise NonblockError('recovery source manifest is not empty: completed='
                                 + repr(manifest.get('completed')) + ' failed='
                                 + repr(manifest.get('failed')))
+        # manifest.cases == [] alone does not prove that no sample was started:
+        # a half-written case directory can exist while the manifest still
+        # reads empty.  Reject the source if its cases/ directory holds
+        # anything.  This lists that one directory only (no tree walk).
+        cases_dir = source / 'cases'
+        if cases_dir.is_dir():
+            try:
+                started = sorted(entry.name for entry in cases_dir.iterdir())
+            except OSError as exc:
+                raise NonblockError('recovery source cases/ cannot be listed: '
+                                    + repr(exc))
+            if started:
+                raise NonblockError(
+                    'recovery source already has case directories (a case '
+                    'started); refusing: ' + repr(started[:8]))
         if manifest.get('bit') is not None:
             try:
                 have_mbit = pathlib.Path(str(manifest.get('bit'))).resolve()
```

### 1c. `_wait_for_root_shell`: docstring

```diff
@@ -805,21 +805,24 @@
-        '''Wait for a usable root shell without blind post-flash writes.
-
-        While the SD loader and the guest boot, the UART is left alone except
-        for one 'root' answer to a login prompt; no framed command is typed
-        until a root prompt ('# ', '/ #', 'root@...#') is actually seen.  The
-        root prompt is not newline terminated, so the reader buffer is drained
-        and accumulated (never discarded).  Exactly one framed 'true' probe
-        then confirms the shell answers.
-
-        recovered=True: the BIT was already flashed by a previous run, so the
-        board is already booted and its prompt was printed before this reader
-        started; a single bare newline is then allowed to elicit the prompt.
-        No Ctrl-C and no framed command is sent before the prompt is seen.
-
-        interrupt_stalled_seed (fresh boot only): after a real 'Linux version'
-        banner has been seen, a boot that stalls at 'Saving random seed' with
-        no root prompt for STALLED_SEED_SILENCE seconds gets exactly one
-        CTRL-C, recorded in boot-recovery.json.  A CTRL-C is never sent before
-        the Linux banner, so the SD loader stage is never poked, and nothing is
-        re-probed afterwards except the one framed 'true'.
-        '''
+        '''Wait for a usable root shell without blind post-flash writes.
+
+        While the SD loader and the guest boot, the UART is left alone except
+        for the two guest login answers: exactly one 'root' at the first
+        'login:' prompt (username) and exactly one 'root' at the following
+        'Password:' prompt.  Each answer is sent only for a prompt that has
+        just arrived, never for a stale prompt already sitting in the tail, and
+        there is no password guessing and no retry: at most two prompt answers
+        are ever written.  No framed command is typed until a root prompt
+        ('# ', '/ #', 'root@...#') is actually seen.  The root prompt is not
+        newline terminated, so the reader buffer is drained and accumulated
+        (never discarded).  Exactly one framed 'true' probe then confirms the
+        shell answers.
+
+        recovered=True: the BIT was already flashed by a previous run, so the
+        board is already booted and its prompt was printed before this reader
+        started; a single bare newline is then allowed to elicit the prompt.
+        No Ctrl-C and no framed command is sent before the prompt is seen.
+
+        interrupt_stalled_seed (fresh boot only): after a real 'Linux version'
+        banner has been seen, a 'Saving random seed' line that is followed by
+        NO further UART output for STALLED_SEED_SILENCE seconds gets exactly
+        one CTRL-C, recorded in boot-recovery.json.  Progress is judged from
+        the time of the last non-empty drain (not from when the marker was
+        first seen), so a boot that keeps printing is never interrupted.  Once
+        a login, Password or root prompt appears the seed interrupt is turned
+        off, so a completed startup is never poked by the old marker, and the
+        SD loader stage (no Linux banner) is never poked at all.
+        '''
```

### 1d. `_wait_for_root_shell`: state init + wait loop

```diff
@@ -837,49 +840,70 @@
         total = self.args.boot_timeout if timeout is None else timeout
         deadline = time.monotonic() + total
         started = time.monotonic()
         tail = ''
+        # Only newly arrived bytes are examined for the two guest login
+        # prompts; a stale 'login:'/'Password:' left in the accumulated tail is
+        # never answered twice.  prompt_pos is the offset already examined.
+        prompt_buf = ''
+        prompt_pos = 0
         login_sent = False
+        password_sent = False
         newline_sent = False
         saw_linux = False
-        seed_seen_at: Optional[float] = None
+        seed_stall_armed = False
+        last_output_at = started
         interrupt_sent = False
         while time.monotonic() < deadline:
             chunk = text_of(self.console.drain_input())
             if chunk:
                 tail = (tail + chunk)[-8192:]
+                combined = prompt_buf + chunk
+                dropped = max(0, len(combined) - 4096)
+                prompt_buf = combined[dropped:]
+                prompt_pos = max(0, prompt_pos - dropped)
+                last_output_at = time.monotonic()
             if ROOT_PROMPT_RE.search(tail):
                 self.console.disable_echo()
                 budget = max(1.0, min(5.0, deadline - time.monotonic()))
                 try:
                     result = self.console.run_shell('true', timeout=budget)
                 except Exception as exc:
                     raise NonblockError('root shell did not answer one framed '
                                         'probe: ' + repr(exc))
                 if result.exit_code != 0:
                     raise NonblockError('root shell framed probe exit='
                                         + str(result.exit_code))
                 return
-            if not login_sent and LOGIN_PROMPT_RE.search(tail):
+            fresh = prompt_buf[prompt_pos:]
+            match = LOGIN_PROMPT_RE.search(fresh)
+            if not login_sent and match:
                 self.console.write_line('root')
                 login_sent = True
+                prompt_pos += match.end()
+                continue
+            match = PASSWORD_PROMPT_RE.search(fresh)
+            if not password_sent and match:
+                self.console.write_line('root')
+                password_sent = True
+                prompt_pos += match.end()
                 continue
             if recovered:
                 # The recovered board printed its prompt before this reader
                 # started, so elicit it once with a bare newline.  This is not
                 # a command and is sent at most once.
                 if (not login_sent and not newline_sent
                         and time.monotonic() - started >= RECOVER_PROMPT_ELICIT_DELAY):
                     self.console._write(bytes([10]))
                     newline_sent = True
-            elif self.args.interrupt_stalled_seed:
-                if not saw_linux and 'Linux version ' in tail:
+            elif self.args.interrupt_stalled_seed and not (login_sent or password_sent):
+                if 'Linux version ' in tail:
                     saw_linux = True
-                if (saw_linux and not interrupt_sent
-                        and STALLED_SEED_MARKER in tail and seed_seen_at is None):
-                    seed_seen_at = time.monotonic()
-                if (seed_seen_at is not None and not interrupt_sent
-                        and time.monotonic() - seed_seen_at >= STALLED_SEED_SILENCE):
-                    self._send_stalled_seed_interrupt()
-                    interrupt_sent = True
-                    seed_seen_at = None
+                if saw_linux and not interrupt_sent:
+                    marker_pos = tail.rfind(STALLED_SEED_MARKER)
+                    linux_pos = tail.rfind('Linux version ')
+                    if marker_pos != -1 and marker_pos > linux_pos:
+                        seed_stall_armed = True
+                    if (seed_stall_armed
+                            and time.monotonic() - last_output_at
+                            >= STALLED_SEED_SILENCE):
+                        self._send_stalled_seed_interrupt()
+                        interrupt_sent = True
             time.sleep(0.2)
         raise NonblockError('root shell not reached within ' + str(total) + 's')
```

### 1e. `_send_stalled_seed_interrupt`: record the real trigger

```diff
@@ -900,7 +900,7 @@
             'interrupt': 'CTRL-C',
             'count': 1,
-            'source': ('fresh boot: Linux banner seen, then \"'
-                       + STALLED_SEED_MARKER + '\" with no root prompt for '
-                       + str(STALLED_SEED_SILENCE) + 's'),
+            'source': ('fresh boot: Linux banner seen, then \"'
+                       + STALLED_SEED_MARKER + '\" followed by no UART output for '
+                       + str(STALLED_SEED_SILENCE) + 's'),
```

**Behaviour notes / assumptions**

- Order of precedence each iteration: root prompt (returns) → new `login:` → new `Password:` → recovered‑newline → seed‑stall. Password is *not* gated on `login_sent` (so a getty that prints both in one drain still gets exactly one Password answer); it is gated only on `password_sent`, i.e. still at most one, never repeated.
- `prompt_buf` is capped at 4096 chars and `prompt_pos` is shifted on truncation, so a prompt run over many small chunks still matches while an old answered prompt cannot match again.
- Seed interrupt is armed only when `Linux version ` appears **before** the marker inside the live tail, and is disarmed the moment any of login/Password/root appears. The 30 s uses `last_output_at` (last non‑empty drain), not the marker timestamp, so a progressing boot is never interrupted.
- `write_line('root')` (LF) is kept for both answers, matching the existing username path. `mac_method88` uses CR; both work with the guest tty, but I kept the existing helper to avoid changing the proven path.

---

## Patch 2 — `out/n-prototype-20261004/analyze_nonblock_pilot.proposal.py`

### 2a. New constants (record / identity)

```diff
@@ -127,6 +127,16 @@
 RAW_WINDOW_V_ISSUED_INDEX = 13
 RAW_WINDOW_V_DONE_INDEX = 14
 RAW_WINDOW_A_JOBS_INDEX: Optional[int] = 3   # == a_done_window == p4m.a_completed_jobs_window
+
+# runner validate_record() 钉死、但本脚本原先未逐字段核对的关键值
+RECORD_SCHEMA = 'idma.p4m.v1'
+P4M_OBSERVER_STATUS_OK = 4
+P4M_OBS_WORDS = 35
+# 运行身份 / 能力块的必要值
+CAPS_SCHEMA = 'idma.experiment.v1'
+BOOT_TAINT_OK = '0'
+BOOT_ID_RE = re.compile(
+    r'^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-'
+    r'[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$')
 
 # --- helper 窗口（ABI idma-nonblock-window-v1，16 字）---------------------
```

### 2b. Bounded direct record validator (mirrors `mac_nonblock_pilot.validate_record`, no runner import)

```diff
@@ -345,6 +345,40 @@
         if any(value != 0 for value in fast_words):
             problems.append('Mode1 (banked-serial) 快速计数必须全 0，实际 %r' % (fast_words,))
     return problems
 
 
+def validate_record_fields(spec: Dict[str, Any],
+                           record: Dict[str, Any]) -> List[str]:
+    """按 runner validate_record() 源逐字段复核必要的 record 值。
+
+    只读传入对象；不导入 runner（避免 Mac/serial 依赖与副作用），不改写任何字段。
+    合法偏低/波动的数值不在此判定。
+    """
+    problems: List[str] = []
+    if record.get('schema') != RECORD_SCHEMA:
+        problems.append('record.schema %r != %s'
+                        % (record.get('schema'), RECORD_SCHEMA))
+    if record.get('success') is not True:
+        problems.append('record.success %r 不是 true' % (record.get('success'),))
+    error = record.get('error')
+    if isinstance(error, str) and error.strip():
+        problems.append('record.error %r' % (error,))
+
+    result = record.get('result')
+    if not isinstance(result, dict):
+        problems.append('record.result 不是对象')
+    else:
+        if result.get('status') != 0:
+            problems.append('record.result.status %r != 0'
+                            % (result.get('status'),))
+        for side in ('victim', 'aggressor'):
+            stats = result.get(side)
+            if side == 'aggressor' and spec['condition'] == 'Solo':
+                if stats is not None:
+                    problems.append('Solo 用例的 result.aggressor 不是 null')
+                continue
+            if not isinstance(stats, dict):
+                problems.append('result.%s 不是对象' % side)
+                continue
+            if stats.get('data_errors') != 0:
+                problems.append('result.%s.data_errors %r != 0'
+                                % (side, stats.get('data_errors')))
+            if stats.get('quarantined') != 0:
+                problems.append('result.%s.quarantined %r != 0'
+                                % (side, stats.get('quarantined')))
+    return problems
+
+
 # ---------------------------------------------------------------------------
 # 单个 case
 # ---------------------------------------------------------------------------
```

### 2c. Call it and add the missing p4m / observer checks

```diff
@@ -465,6 +465,8 @@
     elif not isinstance(record_doc, dict):
         an.q(label, case_id, 'record_not_object', 'record.json 不是 JSON 对象')
     else:
+        for message in validate_record_fields(spec, record_doc):
+            an.q(label, case_id, 'record_field', message)
         candidate = record_doc.get('p4m')
         if isinstance(candidate, dict):
             p4m = candidate
```

```diff
@@ -474,6 +476,14 @@
     if p4m is not None:
         entry['p4m_has_a'] = p4m.get('has_a')
 
+        if p4m.get('bank_magic') != P4M_BANK_MAGIC:
+            an.q(label, case_id, 'p4m_bank_magic',
+                 'p4m.bank_magic=%r != %#x'
+                 % (p4m.get('bank_magic'), P4M_BANK_MAGIC))
+        if p4m.get('observer_status') != P4M_OBSERVER_STATUS_OK:
+            an.q(label, case_id, 'p4m_observer_status',
+                 'p4m.observer_status=%r != %d'
+                 % (p4m.get('observer_status'), P4M_OBSERVER_STATUS_OK))
+
         window = p4m.get('raw_window')
         if all_ints(window) and len(window) == P4M_WINDOW_WORDS:
```

```diff
@@ -488,6 +498,17 @@
         observation = p4m.get('raw_observation')
         if isinstance(observation, list):
             entry['raw_observation_len'] = len(observation)
             if all_ints(observation):
                 entry['raw_observation'] = list(observation)
+            if len(observation) != P4M_OBS_WORDS:
+                an.q(label, case_id, 'raw_observation_shape',
+                     'raw_observation 不是 %d 字: len=%d'
+                     % (P4M_OBS_WORDS, len(observation)))
+
+        if (entry['raw_window'] is not None
+                and entry['raw_observation'] is not None
+                and len(entry['raw_observation']) > 0
+                and entry['raw_window'][RAW_WINDOW_CYCLES_INDEX]
+                != entry['raw_observation'][0]):
+            an.q(label, case_id, 'raw_window_observer_mismatch',
+                 'raw_window[%d]=%r != raw_observation[0]=%r'
+                 % (RAW_WINDOW_CYCLES_INDEX,
+                    entry['raw_window'][RAW_WINDOW_CYCLES_INDEX],
+                    entry['raw_observation'][0]))
 
         cycles = None
```

### 2d. Fold all case issues + label‑level issues into technical qualification

New helper (insert after `analyze_case`, before `summarize_label`):

```python
def finalize_label_qualification(an: Analysis, label: str,
                                 info: Dict[str, Any]) -> None:
    '''把该 label 的全部质量 issue 折进每个 case 的技术资格与 label 汇总。

    该 case 的任何质量 issue（含 a_jobs/exit_txt/metrics/helper 交叉核对）或
    该 label 的 plan/boot/flash/caps/summary 等必要问题，都使受影响 case 技术上
    不合格；合法偏低/波动样本不产生 issue，仍保留并参与汇总。
    '''
    label_level = [issue for issue in an.quality
                   if issue['scope'] == label and issue['case'] is None]
    for case_entry in info['cases']:
        merged = list(case_entry.get('qualified_reasons') or [])
        for issue in an.quality:
            if issue['scope'] != label:
                continue
            if issue['case'] not in (case_entry['id'], None):
                continue
            text = '%s: %s' % (issue['code'], issue['detail'])
            if text not in merged:
                merged.append(text)
        case_entry['qualified_reasons'] = merged
        case_entry['qualified'] = not merged
    info['label_quality_issues'] = [
        '%s: %s' % (issue['code'], issue['detail']) for issue in label_level]
    info['technical_qualified'] = (
        not label_level
        and all(case_entry['qualified'] for case_entry in info['cases']))
```

Call it on both exit paths of `summarize_label`:

```diff
@@ -760,10 +760,13 @@
         'plan_ids': None,
         'cases': [],
+        'label_quality_issues': [],
+        'technical_qualified': None,
     }
 
     if not info['exists']:
         an.q(label, None, 'input_dir_missing',
              'runner 输出目录不存在: ' + directory.as_posix())
         for spec in plan:
             info['cases'].append(analyze_case(an, label, spec,
                                               directory / 'cases' / spec['id']))
+        finalize_label_qualification(an, label, info)
         return info
```

```diff
@@ -906,6 +909,9 @@
         if previous is not None and current[2] <= previous[2]:
             an.q(label, current[1], 'helper_sequence',
                  'helper words[4] 序号 %d 未超过前一有效窗口 %s 的 %d'
                  % (current[2], previous[1], previous[2]))
         previous = current
 
+    finalize_label_qualification(an, label, info)
+
     return info
```

`compute_effects` then only publishes when every label's `rounds_qualified` is true, which now requires all three rounds of that condition to be free of *any* per‑case or label‑level issue. `compute_long`/`compute_short` read `qualified`/`qualified_reasons`, so they inherit the fold automatically (both run after `summarize_label`).

### 2e. Necessary plan / boot / flash / caps checks

```diff
@@ -839,6 +842,9 @@
     elif not isinstance(plan_doc, dict):
         an.q(label, None, 'plan_not_object', 'plan.json 不是 JSON 对象')
     else:
+        if plan_doc.get('schema') != PLAN_SCHEMA:
+            an.q(label, None, 'plan_schema',
+                 'plan.json schema=%r != %s' % (plan_doc.get('schema'), PLAN_SCHEMA))
         listed = plan_doc.get('cases')
         if isinstance(listed, list):
```

```diff
@@ -851,8 +857,30 @@
     boot, err = load_json_file(directory / 'boot.json')
-    if err is None and isinstance(boot, dict):
+    if err is not None:
+        an.q(label, None, 'boot_unreadable', err)
+    elif not isinstance(boot, dict):
+        an.q(label, None, 'boot_not_object', 'boot.json 不是 JSON 对象')
+    else:
         info['boot'] = {
             'uname_r': boot.get('uname_r'),
             'boot_id': boot.get('boot_id'),
             'taint': boot.get('taint'),
             'boot_id_before': boot.get('boot_id_before'),
         }
+        uname_r = boot.get('uname_r')
+        if not isinstance(uname_r, str) or not uname_r.strip():
+            an.q(label, None, 'boot_uname_missing', 'boot.uname_r 缺失')
+        boot_id = boot.get('boot_id')
+        if not isinstance(boot_id, str) or not BOOT_ID_RE.fullmatch(boot_id.strip()):
+            an.q(label, None, 'boot_id_invalid',
+                 'boot.boot_id=%r 不是 UUID' % (boot_id,))
+        if boot.get('taint') != BOOT_TAINT_OK:
+            an.q(label, None, 'boot_taint',
+                 'boot.taint=%r != %r' % (boot.get('taint'), BOOT_TAINT_OK))
+        before = boot.get('boot_id_before')
+        if (isinstance(before, str) and before.strip()
+                and isinstance(boot_id, str)
+                and before.strip() == boot_id.strip()):
+            an.q(label, None, 'boot_id_unchanged',
+                 'boot_id == boot_id_before=%r（flash 后 boot 未更换）' % (boot_id,))
 
     flash, err = load_json_file(directory / 'flash.json')
```

```diff
@@ -860,12 +888,34 @@
     flash, err = load_json_file(directory / 'flash.json')
-    if err is None and isinstance(flash, dict):
+    if err is not None:
+        an.q(label, None, 'flash_unreadable', err)
+    elif not isinstance(flash, dict):
+        an.q(label, None, 'flash_not_object', 'flash.json 不是 JSON 对象')
+    else:
         info['flash'] = {
             'schema': flash.get('schema'),
             'bit': flash.get('bit'),
             'bit_sha256': flash.get('bit_sha256'),
             'returncode': flash.get('returncode'),
             'note': 'sha256 由 runner 记录；本脚本未重新计算任何哈希',
         }
         if flash.get('schema') != FLASH_SCHEMA:
             an.q(label, None, 'flash_schema',
                  'flash.schema=%r != %s' % (flash.get('schema'), FLASH_SCHEMA))
+        if flash.get('returncode') != 0:
+            an.q(label, None, 'flash_returncode',
+                 'flash.returncode=%r != 0' % (flash.get('returncode'),))
 
     caps, err = load_json_file(directory / 'caps.json')
-    info['caps_present'] = isinstance(caps, dict)
+    if err is not None:
+        info['caps_present'] = False
+        an.q(label, None, 'caps_unreadable', err)
+    elif not isinstance(caps, dict):
+        info['caps_present'] = False
+        an.q(label, None, 'caps_not_object', 'caps.json 不是 JSON 对象')
+    else:
+        info['caps_present'] = True
+        parsed = caps.get('parsed')
+        if not isinstance(parsed, dict):
+            an.q(label, None, 'caps_parsed_missing', 'caps.json.parsed 不是对象')
+        else:
+            if parsed.get('schema') != CAPS_SCHEMA:
+                an.q(label, None, 'caps_schema',
+                     'caps.parsed.schema=%r != %s'
+                     % (parsed.get('schema'), CAPS_SCHEMA))
+            if parsed.get('success') is not True:
+                an.q(label, None, 'caps_success',
+                     'caps.parsed.success=%r 不是 true' % (parsed.get('success'),))
```

### 2f. Docstring / uncertainties / boundary notes

```diff
@@ -51,6 +51,15 @@
 * 布局效应 = banked-serial − original-serial；
   绕过效应 = banked-hit − banked-serial；
   original 与 banked-hit 的直接差值不用于归因绕过。
+* 发布前的技术资格会汇合**该 case 的全部质量 issue**（含 a_jobs / exit_txt /
+  metrics / helper 交叉核对）以及该 label 的 plan / boot / flash / caps /
+  summary 必要质量问题；任一存在即该 case（及受影响 label）技术上不合格，
+  compute_effects 不发布。合法偏低或波动样本不记为 quality failure，不剔除。
+* 直接按 runner validate_record() 的源逐实际字段复核 record.schema /
+  success / error / result.status / victim|aggressor.data_errors /
+  quarantined，以及 p4m.bank_magic / observer_status / raw_window 与
+  raw_observation 的一致性；不导入 runner、不写设备、不重算哈希。
 * helper 窗口是 ``idma-nonblock-window-v1``，16 字，计数区间为 (open, close]；
   序号一律取 ``words[4]``；快速路径事件数不是命中率，也不是全部 miss 数。
```

```diff
@@ -1456,6 +1465,12 @@
         '只按冻结计划的 15 个 case id 打开确切路径，未枚举目录；'
         '因此计划外的额外 case 目录不会被发现。',
+        '技术资格（qualified）已汇合该 case 的全部质量 issue 与该 label 的 '
+        'plan/boot/flash/caps/summary 必要质量问题；合法偏低或波动样本不产生 '
+        'issue，仍保留并参与汇总，但任一真实 issue 都会阻止效应发布。',
         '不做哈希校验、不从设备读回、不重跑、不修改任何原始文件。',
         '旧 HPM / COST / E3 数据不是本设计的机制证据；'
         'helper 快速路径事件数不是命中率，也不是全部 miss 数。',
```

```diff
@@ -1561,6 +1576,12 @@
         'boundary_notes': [
             '只读三个 runner 输出目录，只写 --out；原始文件不修改。',
             '不做目录全扫描、不做哈希、不从设备读回。',
+            '直接按 runner validate_record() 源逐字段复核 record（schema/success/'
+            'result.status/victim|aggressor.data_errors/quarantined）与 p4m '
+            'raw_window/raw_observation 一致性；不导入 runner、不写设备。',
+            'qualified 汇合该 case 全部 quality issue 与该 label 的 plan/boot/'
+            'flash/caps/summary 必要问题；有真实技术问题时 compute_effects 不发布。',
             '不剔除异常、不自动重跑、不做显著性统计。',
             '旧 HPM / COST / E3 不作 N 机制证据。',
             '快速路径事件数不是命中率，也不是全部 miss 数。',
```

---

## Acceptance checks / small tests to run (I did not execute any)

**`mac_nonblock_pilot` (unit, no hardware).** Build a fake console exposing `drain_input`/`write_line`/`_write`/`disable_echo`/`run_shell`; construct `PilotRunner` with a minimal `argparse.Namespace`. Monkeypatch `STALLED_SEED_SILENCE` to ~0.5 s to keep the test fast.

1. Emit `"login: "` → assert exactly one `write_line('root')`; then `"Password: "` → assert a second `write_line('root')`; then `"# "` → assert `run_shell('true')` is called and the function returns. Total `write_line` calls == 2.
2. Re‑emit the same `"login: "` a second time (stale) → assert no third answer.
3. Feed `"login: \nPassword: "` in one drain → assert exactly two answers (one login, one Password) and no further answers.
4. Fake clock: after `"Linux version ..."` + `"Saving random seed"`, keep emitting a char every 0.1 s → assert **no** `_write(b'\x03')`; then stop emitting → assert exactly one CTRL‑C after the silence window; then feed `"login: "` early → assert CTRL‑C disabled.
5. Feed `"Saving random seed"` without a Linux banner → assert no CTRL‑C (SD‑loader safety).

**`recover_startup` (unit).** In a tmp dir create `STOPPED.txt`, a successful `flash.json` with the matching `bit`, a `manifest.json` with `cases=[]`, `completed=0`, `failed=0`, correct `label`; then:
- add `cases/long-r1-solo/` → assert `NonblockError` mentioning case directories;
- remove it (or leave `cases/` empty / absent) → assert it proceeds.

**Analyzer (unit, synthetic tree).** For one case dir with a record that has `raw_window[3] != p4m.a_completed_jobs_window` → assert that case's `qualified is False` and a `a_jobs_mismatch` entry appears in `qualified_reasons`. Repeat for `record.schema` wrong, `success=false`, `result.status=5`, `victim.data_errors=1`, `raw_window[1] != raw_observation[0]`. Add a label‑level `flash.returncode=1` → assert every case in that label is unqualified and `compute_effects` returns `published=false` for that condition. Assert a fully clean case with a low/odd but legitimate `cycles` value stays `qualified is True` and its raw window is retained.

---

## Assumptions & things for Codex to verify

- `write_line` (LF‑terminated `root`) is acceptable for the guest Password prompt; the username path already uses it. If the guest strictly needs CR, switch both answers to `self.console._write(b'root' + bytes([13]))` — but do not add retries.
- The seed interrupt deliberately arms on the marker and fires on 30 s of *no output after it*; a boot that resumes printing keeps resetting the timer. If you want strict "stalled *at* the seed marker", additionally require the marker to be the very last non‑whitespace line before silence.
- `prompt_buf` is bounded at 4096 chars (tail is 8192); a prompt is always at the end of the live buffer, so this is enough, but it is a reduction from the full 8192 tail.
- New label‑level checks (boot/flash/caps/plan) intentionally make all cases of the label technically unqualified. For a normally `complete` run all these files exist and pass, so this only bites on stopped/degraded runs.
- All new analyzer constants/field names were taken from the runner listings (`P4M_SCHEMA`, `P4M_OBSERVER_STATUS_OK`, `P4M_OBS_WORDS`, `CAPS_SCHEMA`, `BOOT_ID_RE`) and from where the runner writes them (`boot.json` has no schema field; `caps.json` nests the parsed block under `parsed`). Confirm against a real run before trusting the caps `parsed` path.
- No input file is modified, no device is touched, no hash is recomputed in the analyzer; the running `original-03` recovered‑startup run and its artifacts are untouched by these proposals.