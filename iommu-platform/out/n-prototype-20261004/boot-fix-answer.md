# Focused patch proposal — `scripts/mac_nonblock_pilot.py`

Three bounded changes, no interface redesign:

1. **`ROOT_PROMPT_RE`** now accepts a line-start standalone `#` (the real Buildroot prompt), keeps `/ #`, and forces the `root@…` branch to actually reach `#` so a hostname colon alone can never be read as a prompt. The pre-JTAG `drain_input()` (line 620) and the `collect_boot_identity()` kernel-`00093`/boot-id-different criteria (lines 709–743) are untouched.
2. **`--recover-startup-from DIR`**: reuse a previously stopped run's successful flash, skip JTAG, never re-hash and never tar, prove the source is sample-free, then run the normal wait/kernel/caps/mode/taint path.
3. **`--interrupt-stalled-seed`** (default `false`): one — and only one — CTRL-C, only after a real `Linux version` banner, only on a visible `Saving random seed` stall with no root prompt for 30 s, never at the SD loader stage and never as a repeat probe.

---

## Unified diff

```diff
--- a/scripts/mac_nonblock_pilot.py
+++ b/scripts/mac_nonblock_pilot.py
@@ -64,6 +64,17 @@
 EXPECTED_UNAME_R = '6.5.0-rc2-00093-gc0f5e13ea81f'
 BOOT_ID_RE = re.compile(r'^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-'
                         r'[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$')
 
+# --recover-startup-from: the original-serial-02 run stopped at the Buildroot
+# "Saving random seed" step.  Its pre-flash boot_id (the boot that ran before
+# the BIT was flashed) is known from its console/run log and is reused only so
+# the recovered boot can be shown to be a different boot.
+RECOVER_KNOWN_PRE_FLASH_BOOT_ID = '236b6316-cca3-49be-b52b-48f3963fa515'
+RECOVER_MANUAL_CTRL_C_NOTE = (
+    'the stopped boot was unblocked by a single manual operator CTRL-C at the '
+    'Buildroot "Saving random seed" step; no experiment was re-run')
+RECOVER_PROMPT_ELICIT_DELAY = 1.0
+
+# --interrupt-stalled-seed: a CTRL-C is only ever sent after a real fresh Linux
+# banner, and only on a visible "Saving random seed" stall.
+STALLED_SEED_MARKER = 'Saving random seed'
+STALLED_SEED_SILENCE = 30.0
+
 # struct idma_experiment_caps is eight __u32 followed by reserved[8] __u32, i.e.
 # exactly 64 bytes.  The caps block describes that structure and must never be
@@ -149,8 +160,12 @@
 # Root-shell / login evidence for the post-flash wait.  The shell prompt is not
 # newline terminated, so it is matched against the accumulated drain buffer.
-ROOT_PROMPT_RE = re.compile(r'/ #|root@[^\s]*[:#]')
+# Buildroot's real root shell prints a bare '# ' at the start of a line, which
+# is the form the board shows; '/ #' and the 'root@host:path#' form are kept.
+# The root@ branch must contain its trailing '#' so a hostname colon on its own
+# can never be mistaken for a prompt.
+ROOT_PROMPT_RE = re.compile(r'(?m)^#(?: |$)|^/ #|root@\S*#')
 LOGIN_PROMPT_RE = re.compile(r'login:\s*$')
 
 _DQUOTE = chr(34)
@@ -525,6 +540,8 @@ class PilotRunner:
         self.last_helper_seq: Optional[int] = None
         self.started_at = utc_now()
         self.stop_written = False
+        self.recovery: Optional[Dict[str, Any]] = None
+        self._recovery_boot_id_source = 'recorded-constant'
 
     # ---- logging / writing -------------------------------------------------
 
@@ -642,6 +659,124 @@ class PilotRunner:
             raise NonblockError('openFPGALoader rc=' + str(proc.returncode)
                                 + ': ' + document['stderr'][-400:])
         return document
 
+    def _recover_pre_flash_boot_id(self, source: pathlib.Path) -> str:
+        '''Best evidence of the pre-flash boot_id of the stopped run.
+
+        Preference order: the source run.log 'pre-flash boot_id=' line, then
+        the operator-recorded constant.  Only a valid UUID is accepted; the
+        value is used solely by the boot_id-different check on the recovered
+        boot and is never treated as a gate on its own.
+        '''
+        self._recovery_boot_id_source = 'recorded-constant'
+        log_path = source / 'run.log'
+        if log_path.is_file():
+            text = log_path.read_text(encoding='utf-8', errors='replace')
+            match = re.search(r'pre-flash boot_id=([0-9a-fA-F-]{36})', text)
+            if match:
+                value = match.group(1).strip().lower()
+                if BOOT_ID_RE.fullmatch(value):
+                    self._recovery_boot_id_source = 'run.log'
+                    return value
+        return RECOVER_KNOWN_PRE_FLASH_BOOT_ID
+
+    def recover_startup(self) -> Dict[str, Any]:
+        '''Reuse the flash record of a previously stopped run.
+
+        Read-only against the source; the current output must still not exist
+        (checked by main).  This never re-flashes the BIT, never re-hashes the
+        BIT, never tars anything and never re-runs an experiment.  It only
+        proves the previous run flashed the same BIT/label and collected no
+        sample, records the old pre-flash boot_id and marks this boot as a
+        recovered startup.  Every later gate (uname/kernel, boot_id-different,
+        caps, reader mode, taint) still runs unchanged.
+        '''
+        source = pathlib.Path(self.args.recover_startup_from).resolve()
+        if not source.is_dir():
+            raise NonblockError('--recover-startup-from is not a directory: '
+                                + str(source))
+        if source == self.out_dir.resolve():
+            raise NonblockError('--recover-startup-from must differ from --out')
+        if not (source / 'STOPPED.txt').is_file():
+            raise NonblockError('recovery source has no STOPPED.txt: ' + str(source))
+
+        flash_path = source / 'flash.json'
+        if not flash_path.is_file():
+            raise NonblockError('recovery source has no flash.json: ' + str(source))
+        try:
+            flash = json.loads(flash_path.read_text(encoding='utf-8',
+                                                    errors='replace'))
+        except ValueError as exc:
+            raise NonblockError('recovery flash.json is not JSON: ' + repr(exc))
+        if not isinstance(flash, dict):
+            raise NonblockError('recovery flash.json is not an object')
+        if flash.get('returncode') != 0:
+            raise NonblockError('recovery source flash did not succeed: '
+                                + repr(flash.get('returncode')))
+        want_bit = pathlib.Path(self.args.bit).resolve()
+        try:
+            have_bit = pathlib.Path(str(flash.get('bit'))).resolve()
+        except Exception:
+            have_bit = None
+        if have_bit != want_bit:
+            raise NonblockError('recovery source flashed a different BIT: '
+                                + repr(flash.get('bit'))
+                                + ' != ' + repr(str(want_bit)))
+
+        manifest_path = source / 'manifest.json'
+        if not manifest_path.is_file():
+            raise NonblockError('recovery source has no manifest.json: '
+                                + str(source))
+        try:
+            manifest = json.loads(manifest_path.read_text(encoding='utf-8',
+                                                          errors='replace'))
+        except ValueError as exc:
+            raise NonblockError('recovery manifest.json is not JSON: ' + repr(exc))
+        if not isinstance(manifest, dict):
+            raise NonblockError('recovery manifest.json is not an object')
+        if manifest.get('label') != self.args.label:
+            raise NonblockError('recovery source label '
+                                + repr(manifest.get('label'))
+                                + ' != ' + repr(self.args.label))
+        if manifest.get('cases') != []:
+            raise NonblockError('recovery source already holds samples; refusing: '
+                                + repr(manifest.get('cases')))
+        if manifest.get('completed') not in (0, None) \
+                or manifest.get('failed') not in (0, None):
+            raise NonblockError('recovery source manifest is not empty: completed='
+                                + repr(manifest.get('completed')) + ' failed='
+                                + repr(manifest.get('failed')))
+        if manifest.get('bit') is not None:
+            try:
+                have_mbit = pathlib.Path(str(manifest.get('bit'))).resolve()
+            except Exception:
+                have_mbit = None
+            if have_mbit != want_bit:
+                raise NonblockError('recovery source manifest BIT mismatch: '
+                                    + repr(manifest.get('bit')))
+
+        boot_id_before = self._recover_pre_flash_boot_id(source)
+        self.boot_id_before = boot_id_before
+
+        recovered_flash = dict(flash)
+        recovered_flash['recovered_startup'] = True
+        recovered_flash['recovery_source'] = str(source)
+        recovered_flash['reused_from'] = str(flash_path)
+        recovered_flash['reflashed'] = False
+        recovered_flash['bit_sha256_reused'] = flash.get('bit_sha256')
+        recovered_flash['manual_ctrl_c'] = True
+        recovered_flash['manual_ctrl_c_note'] = RECOVER_MANUAL_CTRL_C_NOTE
+        recovered_flash['recorded_at'] = utc_now()
+        self._write_json('flash.json', recovered_flash)
+
+        self.recovery = {
+            'schema': 'nonblock-pilot-recovery-v1',
+            'source_dir': str(source),
+            'flash_reused': True,
+            'reflashed': False,
+            'bit_sha256_reused': flash.get('bit_sha256'),
+            'manual_ctrl_c': True,
+            'boot_id_before': boot_id_before,
+            'boot_id_before_source': self._recovery_boot_id_source,
+            'note': RECOVER_MANUAL_CTRL_C_NOTE,
+        }
+        self._write_json('boot-recovery.json', {
+            'schema': 'nonblock-pilot-boot-recovery-v1',
+            'recovered': True,
+            'source_dir': str(source),
+            'interrupt': 'manual CTRL-C (operator) during the previous run',
+            'count': 1,
+            'note': (RECOVER_MANUAL_CTRL_C_NOTE
+                     + '; this run re-flashed nothing and re-ran no experiment'),
+            'recovered_at': utc_now(),
+        })
+        self._log('recover-startup-from ' + str(source)
+                  + ': flash reused, pre-flash boot_id=' + repr(boot_id_before)
+                  + ' (' + self._recovery_boot_id_source + ')')
+        return recovered_flash
+
     def _wait_for_root_shell(self, timeout: Optional[float] = None,
                              already_booted: bool = False) -> None:
         '''Wait for a usable root shell without blind post-flash writes.
@@ -690,6 +775,90 @@ class PilotRunner:
                 login_sent = True
             time.sleep(0.2)
         raise NonblockError('root shell not reached within ' + str(total) + 's')
*** END OF STALE HUNK REFERENCE — see replacement hunk below ***
```

The `_wait_for_root_shell` hunk above is only a location marker. Replace the **whole** current body (original lines 646–692) with the following; the surrounding context is otherwise preserved:

```diff
     def _wait_for_root_shell(self, timeout: Optional[float] = None,
-                             already_booted: bool = False) -> None:
+                             already_booted: bool = False,
+                             recovered: bool = False) -> None:
         '''Wait for a usable root shell without blind post-flash writes.
 
         While the SD loader and the guest boot, the UART is left alone except
-        for one 'root' answer to a login prompt; no framed command is typed
-        until a root prompt ('/ #', 'root@...#') is actually seen.  The root
-        prompt is not newline terminated, so the reader buffer is drained and
-        accumulated (never discarded).  Exactly one framed 'true' probe then
-        confirms the shell answers.
+        for one 'root' answer to a login prompt; no framed command is typed
+        until a root prompt ('# ', '/ #', 'root@...#') is actually seen.  The
+        root prompt is not newline terminated, so the reader buffer is drained
+        and accumulated (never discarded).  Exactly one framed 'true' probe
+        then confirms the shell answers.
+
+        recovered=True: the BIT was already flashed by a previous run, so the
+        board is already booted and its prompt was printed before this reader
+        started; a single bare newline is then allowed to elicit the prompt.
+        No Ctrl-C and no framed command is sent before the prompt is seen.
+
+        interrupt_stalled_seed (fresh boot only): after a real 'Linux version'
+        banner has been seen, a boot that stalls at 'Saving random seed' with
+        no root prompt for STALLED_SEED_SILENCE seconds gets exactly one
+        CTRL-C, recorded in boot-recovery.json.  A CTRL-C is never sent before
+        the Linux banner, so the SD loader stage is never poked, and nothing is
+        re-probed afterwards except the one framed 'true'.
         '''
         if already_booted:
             # Optional pre-flash probe: the guest is already up, so one probe
             # is allowed without waiting for a prompt.
             self.console.disable_echo()
             budget = self.args.boot_timeout if timeout is None else timeout
             result = self.console.run_shell('true', timeout=budget)
             if result.exit_code != 0:
                 raise NonblockError('pre-flash readiness probe exit='
                                     + str(result.exit_code))
             return
 
         total = self.args.boot_timeout if timeout is None else timeout
         deadline = time.monotonic() + total
+        started = time.monotonic()
         tail = ''
         login_sent = False
+        newline_sent = False
+        saw_linux = False
+        seed_seen_at: Optional[float] = None
+        interrupt_sent = False
         while time.monotonic() < deadline:
             chunk = text_of(self.console.drain_input())
             if chunk:
                 tail = (tail + chunk)[-8192:]
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
             if not login_sent and LOGIN_PROMPT_RE.search(tail):
                 self.console.write_line('root')
                 login_sent = True
+                continue
+            if recovered:
+                # The recovered board printed its prompt before this reader
+                # started, so elicit it once with a bare newline.  This is not
+                # a command and is sent at most once.
+                if (not login_sent and not newline_sent
+                        and time.monotonic() - started >= RECOVER_PROMPT_ELICIT_DELAY):
+                    self.console._write(bytes([10]))
+                    newline_sent = True
+            elif self.args.interrupt_stalled_seed:
+                if not saw_linux and 'Linux version ' in tail:
+                    saw_linux = True
+                if (saw_linux and not interrupt_sent
+                        and STALLED_SEED_MARKER in tail and seed_seen_at is None):
+                    seed_seen_at = time.monotonic()
+                if (seed_seen_at is not None and not interrupt_sent
+                        and time.monotonic() - seed_seen_at >= STALLED_SEED_SILENCE):
+                    self._send_stalled_seed_interrupt()
+                    interrupt_sent = True
+                    seed_seen_at = None
             time.sleep(0.2)
         raise NonblockError('root shell not reached within ' + str(total) + 's')
 
+    def _send_stalled_seed_interrupt(self) -> None:
+        '''Send exactly one CTRL-C to a boot that stalled after the Linux banner.
+
+        Never sent during the SD loader stage; never repeated; never a blind
+        command.  Only records the recovery and returns to the same wait loop.
+        '''
+        self._log('interrupt-stalled-seed: one CTRL-C after '
+                  + repr(STALLED_SEED_MARKER))
+        self.console._write(bytes([3]))
+        self._write_json('boot-recovery.json', {
+            'schema': 'nonblock-pilot-boot-recovery-v1',
+            'recovered': False,
+            'interrupt': 'CTRL-C',
+            'count': 1,
+            'source': ('fresh boot: Linux banner seen, then "'
+                       + STALLED_SEED_MARKER + '" with no root prompt for '
+                       + str(STALLED_SEED_SILENCE) + 's'),
+            'sd_loader_untouched': True,
+            'sent_at': utc_now(),
+            'note': ('the startup-script interruption is a host recovery '
+                     'action, not re-sampling; the booted kernel is unchanged'),
+        })
+
     def probe_boot_id_before_flash(self) -> None:
```

`run()`:

```diff
@@ -1203,12 +1372,17 @@ class PilotRunner:
             'cases': self.case_plan,
         })
         self._write_manifest()
+        recovered = bool(self.args.recover_startup_from)
         try:
-            self.flash_once()
-            self._wait_for_root_shell()
+            if recovered:
+                self.recover_startup()
+            else:
+                self.flash_once()
+            self._wait_for_root_shell(recovered=recovered)
             identity = self.collect_boot_identity()
+            if recovered:
+                identity['recovered_startup'] = self.recovery
             self._write_json('boot.json', identity)
             self._log('boot uname=' + repr(identity['uname_r'])
-                      + ' boot_id=' + repr(identity['boot_id']))
+                      + ' boot_id=' + repr(identity['boot_id'])
+                      + (' recovered_startup' if recovered else ''))
             caps = self.check_caps()
             self._write_json('caps.json', caps)
             if self._reader_enabled():
```

`_write_manifest()`:

```diff
@@ -1231,6 +1405,10 @@ class PilotRunner:
             'failed': failed,
             'remaining': len(self.case_plan) - len(self.results),
+            'recovered_startup': bool(self.args.recover_startup_from),
+            'recovery_source': (str(pathlib.Path(self.args.recover_startup_from).resolve())
+                                if self.args.recover_startup_from else None),
             'cases': self.results,
         })
```

`_write_summary()`:

```diff
@@ -1257,6 +1435,28 @@ class PilotRunner:
         except OSError:
             content = ''
             for line in content.splitlines():
                 if any(pattern.search(line) for pattern in WARNING_PATTERNS):
                     warnings_found.append(line.strip())
+        notes = [
+            'metrics are limited to the P4M window cycles and the A jobs '
+            'completed inside the window',
+            'raw HPM counters are retained per case but are not a validated '
+            'metric under this nonblock design',
+            'no selection, no tuning, no retry, no resume',
+        ]
+        if self.args.recover_startup_from:
+            notes.append(
+                'recovered_startup: reusing the flash record of '
+                + str(pathlib.Path(self.args.recover_startup_from).resolve())
+                + '; no BIT was re-flashed, no BIT was re-hashed and no '
+                'experiment was re-run before this run')
+        if (self.args.recover_startup_from
+                or (self.out_dir / 'boot-recovery.json').is_file()):
+            notes.append(
+                'startup interruption limitation: the Buildroot startup was '
+                'unblocked by a single CTRL-C at "Saving random seed"; this is '
+                'a host-side recovery action, not re-sampling, and the booted '
+                'kernel is unchanged.  The exact stalled sub-step is not '
+                'asserted (it is not claimed to be an entropy shortage)')
         self._write_json('summary.json', {
             'schema': 'nonblock-pilot-summary-v1',
             'run_id': self.run_id,
             'label': self.args.label,
             'status': status,
             'finished_at': utc_now(),
             'planned': len(self.case_plan),
             'completed': sum(1 for e in self.results if e['status'] == 'completed'),
             'failed': sum(1 for e in self.results if e['status'] == 'failed'),
             'cases': self.results,
             'unexpected_warnings': warnings_found,
-            'notes': [
-                'metrics are limited to the P4M window cycles and the A jobs '
-                'completed inside the window',
-                'raw HPM counters are retained per case but are not a validated '
-                'metric under this nonblock design',
-                'no selection, no tuning, no retry, no resume',
-            ],
+            'notes': notes,
         })
```

CLI + `main`:

```diff
@@ -1236,6 +1458,17 @@ def build_arg_parser() -> argparse.ArgumentParser:
     parser.add_argument('--pre-flash-boot-id', action='store_true',
                         help='best effort: read the running boot_id over the '
                              'UART before flashing, so the new boot_id can be shown to differ')
+    parser.add_argument('--recover-startup-from', default=None,
+                        help='reuse the flash record of a previously stopped '
+                             'run with the same BIT/label and no samples; '
+                             'never re-flashes and never re-hashes the BIT')
+    parser.add_argument('--interrupt-stalled-seed', action='store_true',
+                        help='optional: after a real fresh Linux banner, send '
+                             'exactly one CTRL-C if the boot stalls at '
+                             '"Saving random seed" with no root prompt for '
+                             '30s (default off; never used at the SD loader)')
     return parser
 
 
@@ -1257,6 +1490,14 @@ def main(argv: Optional[Sequence[str]] = None) -> int:
             raise SystemExit('--reader is forbidden for label ' + args.label
                              + ' (no RO hardware on that bitstream)')
         if not pathlib.Path(args.reader).is_file():
             raise SystemExit('--reader not found: ' + args.reader)
+    if args.recover_startup_from:
+        if args.interrupt_stalled_seed:
+            raise SystemExit('--interrupt-stalled-seed does not apply with '
+                             '--recover-startup-from (no fresh boot is started)')
+        if not pathlib.Path(args.recover_startup_from).is_dir():
+            raise SystemExit('--recover-startup-from is not a directory: '
+                             + args.recover_startup_from)
 
     check_uart_ownership(args.serial)                 # read-only, never kills
```

---

## What each change does, against the supplied evidence

**1 — prompt.** Current regex `r'/ #|root@[^\s]*[:#]'` (line 151) misses the bare `# ` that actually appears (e.g. `console.raw.log:1` starts `# __R2B_…`) and lets `root@host:` match on the colon. New regex `r'(?m)^#(?: |$)|^/ #|root@\S*#'`:
- `(?m)^#(?: |$)` matches the line-start standalone `# ` the board really prints;
- `^/ #` keeps the old `/ #` form;
- `root@\S*#` requires the trailing `#`, so `root@host:` cannot match without a `#`.

`LOGIN_PROMPT_RE` unchanged. The pre-JTAG `drain_input()` (line 620) and `collect_boot_identity()` (kernel `EXPECTED_UNAME_R`, `boot_id != boot_id_before`, `taint == 0`) are unchanged, as required.

**2 — recovery.** `recover_startup()` validates: `STOPPED.txt` present, `flash.json.returncode == 0`, resolved BIT path equals `--bit`, `manifest.label == --label`, `manifest.cases == []`, `completed/failed` in `{0,None}`, manifest BIT matches. It sets `boot_id_before` from the source `run.log` (`pre-flash boot_id=…`, matching `console.raw.log:4`) else the recorded constant `236b6316-cca3-49be-b52b-48f3963fa515`, writes a reused `flash.json` tagged `recovered_startup/recovery_source/manual_ctrl_c`, writes `boot-recovery.json`, and returns. No `sha256_file()` call, no tar, no re-flash. The normal `run()` flow then still executes `collect_boot_identity()` (kernel + boot-id-different + taint), `check_caps()`, reader mode checks — nothing is bypassed. Both `mac_nonblock_pilot` labels are affected identically.

**3 — stalled seed.** Guarded by `saw_linux` (so the SD loader stage is never interrupted), the marker, a 30 s silence timer and a one-shot `interrupt_sent` latch. One CTRL-C, recorded in `boot-recovery.json`, then the same loop continues to look for the prompt and the single framed `true`. No blind command, no repeat probe.

---

## Assumptions / unresolved

- **Pre-flash boot id.** The task states the value is known from the console log *or* recorded as `236b6316-…`. The patch prefers a source `run.log` `pre-flash boot_id=` line and otherwise uses that constant, recording which was used (`boot_id_before_source`). If you want the constant *always* (ignoring `run.log`), drop the `run.log` branch.
- **Recovered board state.** Recovery assumes the board is already at a root shell or a `login:` prompt (per `console.raw.log`: after the one manual CTRL-C `Welcome to Buildroot`/`login:` appeared). A single bare `\n` (not a command) is used once to elicit the prompt because the prompt was printed before the reader thread started. If you consider even that too intrusive, set `RECOVER_PROMPT_ELICIT_DELAY` handling aside and instead require an operator-typed newline.
- **Both flags together** raise `SystemExit` — recovery starts no fresh boot, so `--interrupt-stalled-seed` has no target. Flag this if you want them silently combinable instead.
- **Reason not asserted.** The patch deliberately does not claim entropy shortage; the notes say the stalled sub-step is unknown, matching the instruction.
- Not exercised on hardware here — no test evidence is claimed.

## Suggested acceptance checks (for Codex to run)

1. `python3 -m py_compile scripts/mac_nonblock_pilot.py`.
2. Regex units: `'# '`, `'x\n# '`, `'/ #'`, `'root@buildroot:~# '` match; `'root@host:1234'`, `'no prompt here'` do not.
3. Recovery happy path against `out/n-prototype-20261004/nonblock-pilot-20261004-original-02`: run with a fresh `--out`; assert no `openFPGALoader` is invoked, `flash.json.recovered_startup == true` and `flash.json.reused_from` points at the source, `boot.json.recovered_startup.source_dir` set, `summary.json` carries the two new notes.
4. Recovery negatives (must refuse, output dir untouched): source with a non-empty `cases`, `flash.json.returncode != 0`, different `--bit`/`--label`, missing `STOPPED.txt`, `--out` already existing.
5. `--interrupt-stalled-seed`: with a fake console replaying `Linux version …` then `Saving random seed:` and no prompt, assert exactly one `b'\x03'` write and one `boot-recovery.json`; assert no `\x03` before the Linux banner; assert a prompt arriving inside 30 s yields zero writes.