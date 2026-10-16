# mac_nonblock_pilot.py — focused diff + short doc delta

Proposals only. No hardware runs, no compilation, no test execution is claimed. Line numbers in hunk headers were derived by hand from the supplied file; `patch`/`git apply` should be run by the parent and offsets regenerated if any drift exists.

## Assumptions (flag for Codex)

1. `struct idma_experiment_caps` = 8×`__u32` + `reserved[8]`×`__u32` = **64 bytes**; `input.struct_size` is `sizeof(struct idma_experiment)` (a different struct). The current `caps_block['struct_size'] != inp['struct_size']` check is therefore always-invalid and is removed.
2. Caps JSON keys are exactly the ones emitted by `report()` in `bench/idma_experiment.c:535-544`: `abi_version`, `struct_size`, `flags` (=`c->capabilities`), `device_id`, `did`, `max_pages`, `page_bytes`, `max_run_timeout_ms`, `did_hpm_supported`, `software_refill_supported`, `reserved[8]`.
3. Helper JSON contract assumed (helper compiled later): object with `abi` (or `schema`) == `idma-nonblock-window-v1` and `window` == 16 integers. Change the parse if the actual helper differs.
4. Helper invocation assumed `'<elf>' <mode>` (mode 1=`banked-serial`, 2=`banked-hit`). Align with the ELF's real argv when built.
5. Window conservation assumed `window[14] == sum(window[6..13])` (fast `Vread,Vwrite,Aread,Awrite,Other` + normal `V,A,Other`). Flagged, not proven.
6. `--reader` upload uses the inherited protected `SerialConsole._write` (raw, un-framed) exactly as instructed; no ELF bytes are ever written to `console.events.log` or `run.log`.
7. `/dev/mem` character-device check is applied unconditionally after caps, per instruction.

## Unified diff — `scripts/mac_nonblock_pilot.py`

```diff
--- a/scripts/mac_nonblock_pilot.py
+++ b/scripts/mac_nonblock_pilot.py
@@ -58,6 +58,23 @@
 DEFAULT_BOOT_ID_PATH = '/proc/sys/kernel/random/boot_id'
 DEFAULT_TAINT_PATH = '/proc/sys/kernel/tainted'
 DEFAULT_BAUDRATE = 115200
 DEFAULT_JTAG_FREQ = 1000000
 
+# The pilot only ever targets the one frozen engineering kernel; the value is
+# checked, not merely recorded.
+EXPECTED_UNAME_R = '6.5.0-rc2-00093-gc0f5e13ea81f'
+BOOT_ID_RE = re.compile(r'^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-'
+                        r'[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$')
+
+# struct idma_experiment_caps is eight __u32 followed by reserved[8] __u32, i.e.
+# exactly 64 bytes.  The caps block describes that structure and must never be
+# compared with input.struct_size, which is sizeof(struct idma_experiment).
+CAPS_ABI_VERSION = 1
+CAPS_STRUCT_SIZE = 64
+CAPS_DEVICE_ID = 0
+CAPS_DID = 10
+# CAP_DID_HPM | CAP_HW_BATCH | CAP_P4M
+CAPS_REQUIRED_FLAGS = (1 << 0) | (1 << 2) | (1 << 7)
+CAPS_RESERVED_WORDS = 8
+DEV_MEM_PATH = '/dev/mem'
+
 DEFAULT_BOOT_TIMEOUT = 300.0
@@ -65,6 +82,7 @@
 DEFAULT_FLASH_TIMEOUT = 180.0
 PRE_FLASH_PROBE_TIMEOUT = 20.0
+DEFAULT_UPLOAD_TIMEOUT = 300.0
 
 DEFAULT_CHUNK = 4096
 DEFAULT_TIMEOUT_MS = 60000
@@ -72,6 +90,10 @@
 
 LABELS = ('original-serial', 'banked-serial', 'banked-hit')
+# --reader is mandatory for the two banked labels (only those bitstreams have
+# the RO hardware) and forbidden for original-serial.
+READER_REQUIRED_LABELS = ('banked-serial', 'banked-hit')
+READER_FORBIDDEN_LABELS = ('original-serial',)
 
 P4M_SCHEMA = 'idma.p4m.v1'
 CAPS_SCHEMA = 'idma.experiment.v1'
@@ -79,6 +101,20 @@
 P4M_OBS_WORDS = 35
 P4M_OBSERVER_STATUS_OK = 4
 
+# Static RISC-V ELF helper, ABI idma-nonblock-window-v1.  The ELF is built
+# later; this runner only uploads it and validates the JSON it prints.
+READER_ABI = 'idma-nonblock-window-v1'
+READER_WINDOW_WORDS = 16
+READER_MAGIC = 0x49444D414E424C01
+READER_VERSION = 1
+READER_FLAGS_FROZEN = 4          # window[3]
+READER_ERRORS_OK = 0             # window[15]
+READER_MODE_FOR_LABEL = {'banked-serial': 1, 'banked-hit': 2}
+READER_FAST_WORDS = (6, 7, 8, 9, 10)   # Vread, Vwrite, Aread, Awrite, Other
+READER_NORMAL_WORDS = (11, 12, 13)     # V, A, Other
+UPLOAD_CHUNK = 1024
+HELPER_FILES = ('helper.json', 'helper.stderr.txt', 'helper.exit.txt')
+
 SHORT_TRANSFERS = 64
 SHORT_WARMUP = 16
@@ -108,6 +144,11 @@
     re.compile(r'\bKernel panic\b', re.IGNORECASE),
 )
 
+# Root-shell / login evidence for the post-flash wait.  The shell prompt is not
+# newline terminated, so it is matched against the accumulated drain buffer.
+ROOT_PROMPT_RE = re.compile(r'/ #|root@[^\s]*[:#]')
+LOGIN_PROMPT_RE = re.compile(r'login:\s*$')
+
 _DQUOTE = chr(34)
 _B64_WS = frozenset(bytes([32, 9, 13, 10, 11, 12]))
@@ -351,6 +392,46 @@
         'hpm': 'raw-only, incomplete under nonblock; not a validated metric',
     }
 
+
+def validate_helper_window(window: Any, label: str, expected_cycles: Any,
+                           previous_seq: Optional[int]) -> List[str]:
+    '''Validate the static-ELF helper window (ABI idma-nonblock-window-v1).
+
+    The full 16-word window is kept verbatim elsewhere; nothing here is
+    reinterpreted as an HPM counter.
+    '''
+    problems: List[str] = []
+    if not isinstance(window, list) or len(window) != READER_WINDOW_WORDS:
+        return ['helper window is not ' + str(READER_WINDOW_WORDS) + ' words']
+    if any(isinstance(value, bool) or not isinstance(value, int)
+           for value in window):
+        return ['helper window contains non-integer words']
+    if window[0] != READER_MAGIC:
+        problems.append('helper window[0] magic ' + repr(window[0]))
+    if window[1] != READER_VERSION:
+        problems.append('helper window[1] version ' + repr(window[1]))
+    expected_mode = READER_MODE_FOR_LABEL.get(label)
+    if window[2] != expected_mode:
+        problems.append('helper window[2] mode ' + repr(window[2])
+                        + ' != ' + repr(expected_mode) + ' for ' + label)
+    if window[3] != READER_FLAGS_FROZEN:
+        problems.append('helper window[3] flags ' + repr(window[3])
+                        + ' != ' + str(READER_FLAGS_FROZEN))
+    if window[5] != expected_cycles:
+        problems.append('helper window[5] cycles ' + repr(window[5])
+                        + ' != p4m.raw_window[1] ' + repr(expected_cycles))
+    if window[15] != READER_ERRORS_OK:
+        problems.append('helper window[15] errors ' + repr(window[15]))
+    fast = sum(window[i] for i in READER_FAST_WORDS)
+    normal = sum(window[i] for i in READER_NORMAL_WORDS)
+    if window[14] != fast + normal:
+        problems.append('helper accepted ' + repr(window[14]) + ' != fast '
+                        + repr(fast) + ' + normal ' + repr(normal))
+    if previous_seq is not None and window[4] <= previous_seq:
+        problems.append('helper window[4] sequence ' + repr(window[4])
+                        + ' did not increase past ' + repr(previous_seq))
+    return problems
+
 
 # --------------------------------------------------------------------------
@@ -396,12 +477,33 @@
         problems.append('caps max_run_timeout_ms ' + repr(max_timeout)
                         + ' < ' + str(DEFAULT_TIMEOUT_MS))
-    if caps_block.get('abi_version') != inp.get('abi_version'):
-        problems.append('caps abi_version does not match input abi_version')
-    if caps_block.get('struct_size') != inp.get('struct_size'):
-        problems.append('caps struct_size does not match input struct_size')
-    # CAP_P4M / CAP_HW_BATCH are not printed as booleans by --caps; the per-case
-    # CLI enforces them and exits non-zero when they are missing.  See doc A3.
+    # The caps block describes struct idma_experiment_caps, whose size (64) is a
+    # different struct from the input struct_size; comparing the two is invalid
+    # and is deliberately not done here.
+    if caps_block.get('abi_version') != CAPS_ABI_VERSION:
+        problems.append('caps abi_version ' + repr(caps_block.get('abi_version'))
+                        + ' != ' + str(CAPS_ABI_VERSION))
+    if caps_block.get('struct_size') != CAPS_STRUCT_SIZE:
+        problems.append('caps struct_size ' + repr(caps_block.get('struct_size'))
+                        + ' != ' + str(CAPS_STRUCT_SIZE))
+    if caps_block.get('device_id') != CAPS_DEVICE_ID:
+        problems.append('caps device_id ' + repr(caps_block.get('device_id'))
+                        + ' != ' + str(CAPS_DEVICE_ID))
+    if caps_block.get('did') != CAPS_DID:
+        problems.append('caps did ' + repr(caps_block.get('did'))
+                        + ' != ' + str(CAPS_DID))
+    flags = caps_block.get('flags')
+    if isinstance(flags, bool) or not isinstance(flags, int) or \
+            (flags & CAPS_REQUIRED_FLAGS) != CAPS_REQUIRED_FLAGS:
+        problems.append('caps flags ' + repr(flags) + ' lack required bits '
+                        + hex(CAPS_REQUIRED_FLAGS))
+    reserved = caps_block.get('reserved')
+    if not isinstance(reserved, list) or len(reserved) != CAPS_RESERVED_WORDS \
+            or any(value != 0 for value in reserved):
+        problems.append('caps reserved is not ' + str(CAPS_RESERVED_WORDS)
+                        + ' zero words: ' + repr(reserved))
+    # CAP_DID_HPM / CAP_HW_BATCH / CAP_P4M are checked through the printed
+    # flags word above; the per-case CLI enforces them too.  See doc A3.
     return problems
@@ -421,6 +523,7 @@
         self.results: List[Dict[str, Any]] = []
         self.current_case: Optional[str] = None
         self.boot_id_before: Optional[str] = None
+        self.last_helper_seq: Optional[int] = None
         self.started_at = utc_now()
         self.stop_written = False
 
@@ -538,31 +641,52 @@
             raise NonblockError('openFPGALoader rc=' + str(proc.returncode)
                                 + ': ' + document['stderr'][-400:])
         return document
 
-    def _wait_for_root_shell(self, timeout: Optional[float] = None) -> None:
-        '''Wait for a usable root shell, answering a login prompt if one appears.'''
-        total = self.args.boot_timeout if timeout is None else timeout
-        deadline = time.monotonic() + total
-        last: Optional[str] = None
-        self.console.disable_echo()
-        while time.monotonic() < deadline:
-            tail = text_of(self.console.drain_input()).lower()
-            if tail:
-                if 'password:' in tail:
-                    self.console.write_line('')          # root has no password
-                    self.console.disable_echo()
-                elif tail.rstrip().endswith('login:'):
-                    self.console.write_line('root')
-                    self.console.disable_echo()
-            budget = max(1.0, min(5.0, deadline - time.monotonic()))
-            try:
-                result = self.console.run_shell('true', timeout=budget)
-                if result.exit_code == 0:
-                    return
-                last = 'readiness probe exit=' + str(result.exit_code)
-            except Exception as exc:
-                last = repr(exc)
-            time.sleep(0.5)
-        raise NonblockError('root shell not reached within ' + str(total)
-                            + 's: ' + str(last))
+    def _wait_for_root_shell(self, timeout: Optional[float] = None,
+                             already_booted: bool = False) -> None:
+        '''Wait for a usable root shell without blind post-flash writes.
+
+        While the SD loader and the guest boot, the UART is left alone except
+        for one 'root' answer to a login prompt; no framed command is typed
+        until a root prompt ('/ #', 'root@...#') is actually seen.  The root
+        prompt is not newline terminated, so the reader buffer is drained and
+        accumulated (never discarded).  Exactly one framed 'true' probe then
+        confirms the shell answers.
+        '''
+        if already_booted:
+            # Optional pre-flash probe: the guest is already up, so one probe
+            # is allowed without waiting for a prompt.
+            self.console.disable_echo()
+            budget = self.args.boot_timeout if timeout is None else timeout
+            result = self.console.run_shell('true', timeout=budget)
+            if result.exit_code != 0:
+                raise NonblockError('pre-flash readiness probe exit='
+                                    + str(result.exit_code))
+            return
+
+        total = self.args.boot_timeout if timeout is None else timeout
+        deadline = time.monotonic() + total
+        tail = ''
+        login_sent = False
+        while time.monotonic() < deadline:
+            chunk = text_of(self.console.drain_input())
+            if chunk:
+                tail = (tail + chunk)[-8192:]
+            if ROOT_PROMPT_RE.search(tail):
+                self.console.disable_echo()
+                budget = max(1.0, min(5.0, deadline - time.monotonic()))
+                try:
+                    result = self.console.run_shell('true', timeout=budget)
+                except Exception as exc:
+                    raise NonblockError('root shell did not answer one framed '
+                                        'probe: ' + repr(exc))
+                if result.exit_code != 0:
+                    raise NonblockError('root shell framed probe exit='
+                                        + str(result.exit_code))
+                return
+            if not login_sent and LOGIN_PROMPT_RE.search(tail):
+                self.console.write_line('root')
+                login_sent = True
+            time.sleep(0.2)
+        raise NonblockError('root shell not reached within ' + str(total) + 's')
@@ -569,6 +693,6 @@
         '''Best effort only: never fatal, never a gate.'''
         try:
-            self._wait_for_root_shell(timeout=PRE_FLASH_PROBE_TIMEOUT)
+            self._wait_for_root_shell(timeout=PRE_FLASH_PROBE_TIMEOUT,
+                                      already_booted=True)
             result = self.console.run_shell('cat ' + DEFAULT_BOOT_ID_PATH,
                                             timeout=self.args.command_timeout)
@@ -586,6 +710,10 @@
         if result.exit_code != 0:
             raise NonblockError('uname -r exit=' + str(result.exit_code))
         info['uname_r'] = text_of(result.stdout).strip()
+        info['uname_expected'] = EXPECTED_UNAME_R
+        if info['uname_r'] != EXPECTED_UNAME_R:
+            raise NonblockError('uname -r ' + repr(info['uname_r'])
+                                + ' != ' + repr(EXPECTED_UNAME_R))
 
         result = self.console.run_shell('cat ' + DEFAULT_BOOT_ID_PATH,
                                         timeout=self.args.command_timeout)
@@ -594,6 +722,8 @@
         info['boot_id'] = text_of(result.stdout).strip()
         if not info['boot_id']:
             raise NonblockError('boot_id is empty')
+        if not BOOT_ID_RE.fullmatch(info['boot_id']):
+            raise NonblockError('boot_id is not a UUID: ' + repr(info['boot_id']))
 
         result = self.console.run_shell('cat ' + DEFAULT_TAINT_PATH,
                                         timeout=self.args.command_timeout)
@@ -626,6 +756,189 @@
         problems = validate_caps(parsed, self.args.device)
         if problems:
             raise NonblockError('caps validation failed: ' + '; '.join(problems))
         return {'raw': raw, 'parsed': parsed, 'collected_at': utc_now()}
 
+    def _check_dev_mem(self) -> None:
+        '''The static helper reads MMIO through /dev/mem; require it present.'''
+        result = self.console.run_shell('test -c ' + DEV_MEM_PATH,
+                                        timeout=self.args.command_timeout)
+        if result.exit_code != 0:
+            raise NonblockError(DEV_MEM_PATH + ' is not a character device')
+
+    def _reader_enabled(self) -> bool:
+        return bool(self.args.reader) and self.args.label in READER_REQUIRED_LABELS
+
+    def _reader_target(self) -> str:
+        return self.board_root + '/nonblock-reader.elf'
+
+    def _reader_command(self) -> str:
+        # Assumed helper invocation; align with the ELF once it is compiled.
+        mode = READER_MODE_FOR_LABEL.get(self.args.label)
+        return shlex.quote(self._reader_target()) + ' ' + str(mode)
+
+    def _upload_reader(self) -> Dict[str, Any]:
+        '''Raw UART upload of the static helper into /tmp.
+
+        The ELF is base64 encoded on the host and typed to the board as a shell
+        here-document through the raw writer (run_shell forbids newlines).  The
+        begin/end markers are built from shell parts so an echoed line can never
+        be mistaken for a marker.  There is no automatic retry: the end marker
+        is expected exactly once and the final check is one framed command.
+        '''
+        source = pathlib.Path(self.args.reader).resolve()
+        if not source.is_file():
+            raise NonblockError('--reader ELF not found: ' + str(source))
+        data = source.read_bytes()
+        encoded = base64.b64encode(data)
+        lines = [encoded[i:i + UPLOAD_CHUNK].decode('ascii')
+                 for i in range(0, len(encoded), UPLOAD_CHUNK)]
+        target = self._reader_target()
+        n1 = secrets.token_hex(8)
+        n2 = secrets.token_hex(8)
+        begin = 'NBB_' + n1 + '_' + n2 + '__'
+        end = 'NBE_' + n1 + '_' + n2 + '__'
+        delim = 'NBD_' + n1 + '_' + n2 + '__'
+        nl = chr(92) + 'n'
+
+        script: List[str] = [
+            'stty -echo 2>/dev/null',
+            'mkdir -p ' + shlex.quote(self.board_root),
+            'b0=NBB; e0=NBE; m1=' + n1 + '; m2=' + n2
+            + '; bm="${b0}_${m1}_${m2}__"; em="${e0}_${m1}_${m2}__"',
+            "printf '%s" + nl + "' " + _DQUOTE + '$bm' + _DQUOTE,
+            '{ base64 -d > ' + shlex.quote(target) + '; __nb_rc=$?; printf '
+            + "'%s %s" + nl + "' " + _DQUOTE + '$em' + _DQUOTE + ' '
+            + _DQUOTE + '$__nb_rc' + _DQUOTE + '; } <<' + "'" + delim + "'",
+        ]
+        script.extend(lines)
+        script.append(delim)
+
+        for line in script:
+            self.console._write((line + chr(10)).encode('utf-8'))
+
+        try:
+            self.console.read_until(begin.encode('ascii'),
+                                    self.args.upload_timeout)
+            self.console.read_until(end.encode('ascii'),
+                                    self.args.upload_timeout)
+            raw_rc = self.console.read_line(self.args.upload_timeout)
+        except Exception as exc:
+            raise NonblockError('reader upload framing failed: ' + repr(exc))
+        rc_text = text_of(raw_rc).strip()
+        if not _INT_RE.fullmatch(rc_text):
+            raise NonblockError('reader upload exit marker is not an integer: '
+                                + repr(raw_rc))
+        if int(rc_text) != 0:
+            raise NonblockError('reader base64 decode exit=' + rc_text)
+
+        check = self.console.run_shell(
+            'chmod 700 ' + shlex.quote(target) + ' && test -s '
+            + shlex.quote(target), timeout=self.args.command_timeout)
+        if check.exit_code != 0:
+            raise NonblockError('reader chmod/verify exit=' + str(check.exit_code))
+
+        record = {
+            'schema': 'nonblock-pilot-reader-upload-v1',
+            'reader_path': target,
+            'reader_mode': READER_MODE_FOR_LABEL.get(self.args.label),
+            'elf_bytes': len(data),
+            'transfer_bytes': len(encoded),
+            'chunk_bytes': UPLOAD_CHUNK,
+            'chunk_count': len(lines),
+            'mode': '700',
+            'hash_readback': False,
+            'uploaded_at': utc_now(),
+        }
+        self._write_json('reader-upload.json', record)
+        self._log('reader uploaded: ' + target + ' bytes=' + str(len(encoded))
+                  + ' mode=' + str(record['reader_mode']))
+        return record
+
+    def _run_helper_case(self, cid: str, board_dir: str, local_dir: pathlib.Path,
+                         record: Any) -> Tuple[List[str], Optional[List[int]]]:
+        '''Run the uploaded helper once and validate its frozen window.
+
+        Returns (problems, full_window).  Reached only when the CLI exited 0 and
+        the kernel is untainted, so MMIO is never read from a busy or failed run.
+        '''
+        problems: List[str] = []
+        command = (self._reader_command() + ' > helper.json 2> helper.stderr.txt; '
+                   'ec=$?; echo ' + _DQUOTE + '$ec' + _DQUOTE
+                   + ' > helper.exit.txt')
+        framed_exit: Optional[int] = None
+        try:
+            result = self.console.run_shell(command, timeout=self.args.case_timeout,
+                                            cwd=board_dir, exit_var='ec')
+            framed_exit = result.exit_code
+        except Exception as exc:
+            problems.append('helper command raised: ' + repr(exc))
+        for name in HELPER_FILES:
+            try:
+                self._export_file(board_dir + '/' + name,
+                                  local_dir / (name + '.b64'),
+                                  local_dir / name)
+            except Exception as exc:
+                problems.append('export ' + name + ': ' + repr(exc))
+
+        helper_exit: Optional[int] = None
+        exit_path = local_dir / 'helper.exit.txt'
+        if exit_path.is_file():
+            value = exit_path.read_text(encoding='utf-8',
+                                        errors='replace').strip()
+            if _INT_RE.fullmatch(value):
+                helper_exit = int(value)
+        if helper_exit is None:
+            problems.append('helper.exit.txt is missing or not an integer')
+        elif helper_exit != 0:
+            problems.append('helper exit ' + str(helper_exit))
+        elif framed_exit is not None and framed_exit != helper_exit:
+            problems.append('helper framed exit ' + str(framed_exit)
+                            + ' != helper.exit.txt ' + str(helper_exit))
+
+        window: Optional[List[int]] = None
+        helper_doc: Any = None
+        json_path = local_dir / 'helper.json'
+        if json_path.is_file():
+            try:
+                helper_doc = json.loads(json_path.read_text(encoding='utf-8',
+                                                            errors='replace'))
+            except ValueError as exc:
+                problems.append('helper.json is not valid JSON: ' + repr(exc))
+        if isinstance(helper_doc, dict):
+            abi = helper_doc.get('abi', helper_doc.get('schema'))
+            if abi != READER_ABI:
+                problems.append('helper ABI ' + repr(abi) + ' != ' + READER_ABI)
+            candidate = helper_doc.get('window')
+            if isinstance(candidate, list):
+                window = candidate
+        else:
+            problems.append('helper.json is not a JSON object')
+
+        expected_cycles: Any = None
+        if isinstance(record, dict):
+            p4m = record.get('p4m')
+            if isinstance(p4m, dict):
+                raw = p4m.get('raw_window')
+                if isinstance(raw, list) and len(raw) > 1:
+                    expected_cycles = raw[1]
+        if expected_cycles is None:
+            problems.append('no p4m.raw_window[1] for the helper cycles')
+
+        if window is None:
+            problems.append('helper window is missing')
+        else:
+            window_problems = validate_helper_window(
+                window, self.args.label, expected_cycles, self.last_helper_seq)
+            problems.extend(window_problems)
+            if not window_problems:
+                self.last_helper_seq = window[4]
+            self._write_json('cases/' + cid + '/helper-window.json', {
+                'schema': 'nonblock-pilot-helper-window-v1',
+                'id': cid,
+                'label': self.args.label,
+                'abi': READER_ABI,
+                'mode': READER_MODE_FOR_LABEL.get(self.args.label),
+                'window': window,
+                'note': 'full frozen 16-word window saved verbatim; not HPM',
+            })
+        return problems, window
+
     # ---- one case ----------------------------------------------------------
@@ -706,6 +1019,16 @@
         evidence, export_problems = self._collect_case_evidence(board_dir, local_dir)
         record = evidence['record']
         exit_code = evidence['exit']
 
-        problems: List[str] = list(export_problems)
+        helper_problems: List[str] = []
+        helper_window: Optional[List[int]] = None
+        if self._reader_enabled() and exit_code == 0 \
+                and evidence['taint_after'] == '0':
+            # Only touch MMIO through the helper when the CLI exited 0 and the
+            # kernel is untainted; never probe a busy or failed board.
+            helper_problems, helper_window = self._run_helper_case(
+                cid, board_dir, local_dir, record)
+
+        problems: List[str] = list(export_problems) + helper_problems
         if framed_error is not None:
             problems.append('framed command raised: ' + repr(framed_error))
@@ -749,6 +1072,8 @@
             'finished_at': utc_now(),
             'problems': problems,
             'metrics': metrics,
+            'helper_window': helper_window,
+            'helper_problems': helper_problems,
         }
         self._write_json('cases/' + cid + '/case.json', case_document)
 
@@ -764,6 +1089,7 @@
             'v_window_cycles': (metrics or {}).get('v_window_cycles'),
             'a_completed_jobs_window': (metrics or {}).get('a_completed_jobs_window'),
             'problems': problems,
+            'helper_window': helper_window,
         })
         self._write_manifest()
@@ -849,6 +1175,9 @@
             caps = self.check_caps()
             self._write_json('caps.json', caps)
+            self._check_dev_mem()
+            if self._reader_enabled():
+                self._upload_reader()
             self._capture_dmesg('dmesg.baseline.txt')
             for case in self.case_plan:
                 self.run_one_case(case)
@@ -896,6 +1225,10 @@
     parser.add_argument('--flash-timeout', type=float, default=DEFAULT_FLASH_TIMEOUT)
     parser.add_argument('--jtag-freq', type=int, default=DEFAULT_JTAG_FREQ)
+    parser.add_argument('--reader', default=None,
+                        help='static RISC-V ELF helper; required for '
+                             'banked-serial/banked-hit, forbidden for original-serial')
+    parser.add_argument('--upload-timeout', type=float, default=DEFAULT_UPLOAD_TIMEOUT)
     parser.add_argument('--pre-flash-boot-id', action='store_true',
                         help='best effort: read the running boot_id over the UART '
                              'before flashing, so the new boot_id can be shown to differ')
@@ -911,6 +1244,13 @@
         raise SystemExit('--bit not found: ' + args.bit)
     if not pathlib.Path(args.openfpgaloader).is_file():
         raise SystemExit('--openfpgaloader not found: ' + args.openfpgaloader)
+    if args.label in READER_REQUIRED_LABELS and not args.reader:
+        raise SystemExit('--reader is required for label ' + args.label)
+    if args.reader:
+        if args.label in READER_FORBIDDEN_LABELS:
+            raise SystemExit('--reader is forbidden for label ' + args.label
+                             + ' (no RO hardware on that bitstream)')
+        if not pathlib.Path(args.reader).is_file():
+            raise SystemExit('--reader not found: ' + args.reader)
 
     check_uart_ownership(args.serial)                 # read-only, never kills
@@ -922,6 +1262,14 @@
 
     runner = PilotRunner(args, console, out_dir)
     if args.pre_flash_boot_id:
-        runner.probe_boot_id_before_flash()
+        try:
+            runner.probe_boot_id_before_flash()
+        except BaseException:
+            # A failed pre-flash probe must release the UART as well.
+            try:
+                console.close()
+            except Exception:
+                pass
+            raise
     return runner.run()
```

## Short doc delta (append to the pilot engineering note; not a full file)

```markdown
### Nonblock pilot runner delta (mac_nonblock_pilot.py)

* **Caps gate fixed.** `--caps` prints a *caps* struct (`struct idma_experiment_caps`,
  exactly 64 bytes). The old check compared `capabilities.struct_size` with
  `input.struct_size`, which is `sizeof(struct idma_experiment)` — different structs,
  so the comparison was invalid. Now: `capabilities.abi_version == 1`,
  `capabilities.struct_size == 64`, `device_id == 0`, `did == 10`,
  `flags & ((1<<0)|(1<<2)|(1<<7))` set (DID_HPM|HW_BATCH|P4M; the printed `flags`
  word equals `c->capabilities`), and `reserved[8]` all zero. `input.struct_size`
  is no longer compared with the caps size.

* **Boot identity is validated, not merely recorded.** `uname -r` must equal
  `6.5.0-rc2-00093-gc0f5e13ea81f`; `boot_id` must match the UUID form; and when the
  optional pre-flash `boot_id` was read it must differ (unchanged rule).

* **Post-flash wait no longer blind-writes.** During SD-loader/kernel boot nothing
  is typed. The reader buffer is drained and accumulated (the root prompt is not
  newline-terminated) until `/ #` or `root@...#` or a `login:` prompt is seen;
  `root` is sent only on the login prompt (no password line is sent). After a root
  prompt is visible, exactly one framed `true` probe confirms the shell. The
  optional pre-flash probe uses `already_booted=True` (one immediate probe) and the
  UART is now closed if that probe raises.

* **New optional `--reader <elf>`.** Required for labels `banked-serial`/`banked-hit`
  (the only bitstreams with the RO hardware), forbidden for `original-serial`.
  The static RISC-V ELF is uploaded over UART into `/tmp` only — no SD-card write,
  no kernel change. Upload is a raw here-document (`base64 -d > <target>`,
  ≤1024-char lines, shell-built begin/end markers, echo off, no retry, end marker
  expected once), then one framed `chmod 700` + `test -s` check. Recorded:
  transfer bytes, fixed root path, reader mode; no hash and no read-back. ELF bytes
  are never written to the event log or run log. `$HOME` is never set.

* **`/dev/mem` precondition.** After boot/caps and before any case, `/dev/mem` must
  be a character device.

* **Per-case helper window (ABI `idma-nonblock-window-v1`).** After each CLI run the
  helper runs *only* when the CLI exited 0 and `taint.after == 0`; its raw window
  JSON, stderr and exit are captured next to the five existing files
  (`helper.json`, `helper.stderr.txt`, `helper.exit.txt`). The full 16-word window is
  saved verbatim (not reinterpreted as HPM) and validated: `word0 magic 0x49444D414E424C01`,
  `word1 version 1`, `word2 mode` matches the label, `word3 flags 4`,
  `word4 sequence` increases within the bank boot, `word5 cycles == p4m.raw_window[1]`,
  `word15 errors 0`, and `word14 total accepted == sum(word6..word13)` (conservation
  **assumed**; confirm against the compiled helper). `words 6..10` = fast counts
  Vread/Vwrite/Aread/Awrite/Other; `words 11..13` = normal V/A/Other.

* The frozen 15-case plan, file layout and hash/package flows are unchanged.

* **Blocked / do not claim yet:** additional invalid-reader / MEM-stop checks remain
  blocked on the actual new kernel (possibly a user-SD boundary). No claim is made
  until the runtime is verified.
```

## Suggested acceptance checks (parent to execute)

1. `python3 -c "import ast,pathlib; ast.parse(pathlib.Path('scripts/mac_nonblock_pilot.py').read_text())"` — syntax.
2. `--caps` against the real board: JSON parses; `capabilities.struct_size==64`, `abi_version==1`, `device_id==0`, `did==10`, `flags & 0x85 == 0x85`, `reserved==[0]*8`; runner must **not** compare input vs caps struct size.
3. `collect_boot_identity` fails if `uname -r != 6.5.0-rc2-00093-gc0f5e13ea81f`; fails on a non-UUID `boot_id`; fails when `boot_id` equals the pre-flash value.
4. Boot wait: with a loader that is silent then prints `/ #`, assert **zero** framed commands are typed before the prompt appears (grep `console.events.log` for `send:` before the first prompt); exactly one framed `true` afterwards.
5. `--reader` with `--label original-serial` → exit before touching the UART; with a banked label and no `--reader` → exit; with `--reader missing.elf` → exit.
6. Upload: after success `/tmp/<run>/nonblock-reader.elf` exists, mode `700`, size == ELF size; `reader-upload.json` has no hash/readback; `console.events.log` and `run.log` contain no base64/ELF bytes.
7. Per case with `--reader`: `helper.json`/`helper.stderr.txt`/`helper.exit.txt` present; helper is skipped (and MMIO not read) whenever exit≠0 or taint≠0; `helper-window.json` holds the full 16 words; a deliberately wrong `word5`, `word2`, `word15`, a non-increasing `word4`, or `word14 != sum(word6..13)` is rejected.
8. Pre-flash probe failure path: force `probe_boot_id_before_flash` to raise and confirm `console.close()` ran (no UART leak).

## Unresolved / needs Codex

- Helper JSON key (`abi` vs `schema`) and argv shape are assumptions to be reconciled with the compiled ELF.
- `window[14]` conservation definition is a guess.
- Whether `/dev/mem` should be required for `original-serial` (currently unconditional per instruction).
- Invalid-reader / MEM-stop checks remain blocked pending the new-kernel / user-SD boundary check.