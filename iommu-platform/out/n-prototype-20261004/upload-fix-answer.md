## What this patch does (scope)

Only `scripts/mac_nonblock_pilot.py`, only the raw‑UART upload path:

- Adds two upload‑tuning constants.
- Rewrites `_upload_reader()` to: raise only `_ser.write_timeout` (never `read_timeout`) for the transfer, restore it in `finally`; confirm the header by reading the real begin marker before any payload byte; clear `PS2` for the heredoc and restore it afterwards; pace one payload line at a time; bound the whole transfer by a single `--upload-timeout`; on any failure stop immediately with bounded `upload-failure.json` (bytes/lines attempted, phase, error — never ELF/base64); on success run only `chmod 700 && test -s`.

No changes to reader/MMIO code, hardware, kernel, the manifest/case flow, or any other function. No in‑function CTRL‑C, retry, resume, hash/read‑back or untar is added.

Key facts used (from the supplied files): `mac_research2.SerialConsole.start()` builds the port with `factory(port, baud, 0.1)` → `read_timeout = write_timeout = 0.1` (`scripts/mac_research2.py:359-365`, `322-332`); `_write` does `write()+flush()` under the lock (`:402-405`); `read_until/read_line` block on the reader condition (`:427-459`). The crash trace confirms the failing loop is `_upload_reader` line 1085 writing to `_ser.write` with the 0.1 s write timeout.

---

## Unified diff

```diff
--- a/scripts/mac_nonblock_pilot.py
+++ b/scripts/mac_nonblock_pilot.py
@@ -130,6 +130,12 @@
 READER_FAST_WORDS = (6, 7, 8, 9, 10)   # Vread, Vwrite, Aread, Awrite, Other
 READER_NORMAL_WORDS = (11, 12, 13)     # V, A, Other
 UPLOAD_CHUNK = 1024
+# Raw-UART heredoc upload tuning.  At 115200 8N1 one 1024-byte base64 line
+# costs about 89 ms of wire time, so each payload line is followed by a short
+# pause to keep the host write queue drained.  The write timeout is raised only
+# for this upload and is always restored; read_timeout is never touched.
+UPLOAD_WRITE_TIMEOUT = 5.0
+UPLOAD_LINE_DELAY = 0.15
 HELPER_FILES = ('helper.json', 'helper.stderr.txt', 'helper.exit.txt')
 
 SHORT_TRANSFERS = 64
@@ -1044,83 +1050,175 @@
         mode = READER_MODE_FOR_LABEL.get(self.args.label)
         return shlex.quote(self._reader_target())
 
     def _upload_reader(self) -> Dict[str, Any]:
-        '''Raw UART upload of the static helper into /tmp.
-
-        The ELF is base64 encoded on the host and typed to the board as a shell
-        here-document through the raw writer (run_shell forbids newlines).  The
-        begin/end markers are built from shell parts so an echoed line can never
-        be mistaken for a marker.  There is no automatic retry: the end marker
-        is expected exactly once and the final check is one framed command.
-        '''
+        '''Raw UART upload of the static helper into /tmp.
+
+        The ELF is base64 encoded on the host and typed to the board as a shell
+        here-document through the raw writer (run_shell forbids newlines).  The
+        begin/end markers are built from shell parts so an echoed line can never
+        be mistaken for a marker.
+
+        The write timeout is raised only for this transfer and restored in a
+        finally block; read_timeout is never changed.  The payload is paced one
+        line at a time (UPLOAD_LINE_DELAY): 1024 base64 bytes cost about 89 ms
+        of wire time at 115200 8N1, so without pacing the host write queue fills
+        and pyserial raises SerialTimeoutException.  PS2 is cleared for the
+        heredoc (to suppress the secondary prompt on every payload line) and
+        restored afterwards.  The whole transfer is bounded by one
+        --upload-timeout; the header is confirmed by reading the real begin
+        marker before any payload byte is sent, and exactly one begin/end marker
+        pair is used.  On any failure the transfer stops immediately (no retry,
+        no resume, no partial helper run) and bounded metadata (bytes/lines
+        attempted, phase, error) is written to upload-failure.json; the ELF and
+        its base64 are never stored there.
+        '''
         source = pathlib.Path(self.args.reader).resolve()
         if not source.is_file():
             raise NonblockError('--reader ELF not found: ' + str(source))
         data = source.read_bytes()
         encoded = base64.b64encode(data)
         lines = [encoded[i:i + UPLOAD_CHUNK].decode('ascii')
                  for i in range(0, len(encoded), UPLOAD_CHUNK)]
         target = self._reader_target()
         n1 = secrets.token_hex(8)
         n2 = secrets.token_hex(8)
         begin = 'NBB_' + n1 + '_' + n2 + '__'
         end = 'NBE_' + n1 + '_' + n2 + '__'
         delim = 'NBD_' + n1 + '_' + n2 + '__'
         nl = chr(92) + 'n'
 
-        script: List[str] = [
+        # Header through the heredoc opener.  The begin marker is printed before
+        # the heredoc, so reading it proves the header ran before any payload
+        # byte is sent.  PS2 is saved and cleared so the heredoc does not echo
+        # '> ' for every payload line; it is restored once the upload finishes.
+        header: List[str] = [
             'stty -echo 2>/dev/null',
             'mkdir -p ' + shlex.quote(self.board_root),
+            '__nb_ps2="${PS2-}"; PS2=',
             'b0=NBB; e0=NBE; m1=' + n1 + '; m2=' + n2
             + '; bm="${b0}_${m1}_${m2}__"; em="${e0}_${m1}_${m2}__"',
             "printf '%s" + nl + "' " + _DQUOTE + '$bm' + _DQUOTE,
             '{ base64 -d > ' + shlex.quote(target) + '; __nb_rc=$?; printf '
             + "'%s %s" + nl + "' " + _DQUOTE + '$em' + _DQUOTE + ' '
             + _DQUOTE + '$__nb_rc' + _DQUOTE + '; } <<' + "'" + delim + "'",
         ]
-        script.extend(lines)
-        script.append(delim)
 
-        for line in script:
-            self.console._write((line + chr(10)).encode('utf-8'))
+        old_write_timeout: Optional[float] = None
+        try:
+            old_write_timeout = self.console._ser.write_timeout
+            self.console._ser.write_timeout = UPLOAD_WRITE_TIMEOUT
+        except Exception as exc:
+            old_write_timeout = None
+            self._log('reader upload: cannot raise write_timeout: ' + repr(exc))
 
-        try:
-            self.console.read_until(begin.encode('ascii'),
-                                    self.args.upload_timeout)
-            self.console.read_until(end.encode('ascii'),
-                                    self.args.upload_timeout)
-            raw_rc = self.console.read_line(self.args.upload_timeout)
+        deadline = time.monotonic() + self.args.upload_timeout
+        lines_attempted = 0
+        bytes_attempted = 0
+        phase = 'write-header'
+        upload_error: Optional[BaseException] = None
+        try:
+            for line in header:
+                self.console._write((line + chr(10)).encode('utf-8'))
+
+            phase = 'await-begin'
+            remaining = deadline - time.monotonic()
+            if remaining <= 0:
+                raise NonblockError('reader upload header timed out')
+            self.console.read_until(begin.encode('ascii'), remaining)
+
+            phase = 'write-payload'
+            for line in lines:
+                remaining = deadline - time.monotonic()
+                if remaining <= 0:
+                    raise NonblockError(
+                        'reader upload payload timed out after '
+                        + str(lines_attempted) + ' lines')
+                self.console._write((line + chr(10)).encode('utf-8'))
+                lines_attempted += 1
+                bytes_attempted += len(line)
+                time.sleep(UPLOAD_LINE_DELAY)
+            self.console._write((delim + chr(10)).encode('utf-8'))
+
+            phase = 'await-end'
+            remaining = deadline - time.monotonic()
+            if remaining <= 0:
+                raise NonblockError('reader upload end marker timed out')
+            self.console.read_until(end.encode('ascii'), remaining)
+
+            phase = 'check-exit'
+            remaining = deadline - time.monotonic()
+            if remaining <= 0:
+                raise NonblockError('reader upload exit marker timed out')
+            raw_rc = self.console.read_line(remaining)
+            rc_text = text_of(raw_rc).strip()
+            if not _INT_RE.fullmatch(rc_text):
+                raise NonblockError(
+                    'reader upload exit marker is not an integer: '
+                    + repr(raw_rc))
+            if int(rc_text) != 0:
+                raise NonblockError('reader base64 decode exit=' + rc_text)
+
+            phase = 'verify'
+            check = self.console.run_shell(
+                'PS2="${__nb_ps2-}"; chmod 700 ' + shlex.quote(target)
+                + ' && test -s ' + shlex.quote(target),
+                timeout=self.args.command_timeout)
+            if check.exit_code != 0:
+                raise NonblockError('reader chmod/verify exit='
+                                    + str(check.exit_code))
         except Exception as exc:
-            raise NonblockError('reader upload framing failed: ' + repr(exc))
-        rc_text = text_of(raw_rc).strip()
-        if not _INT_RE.fullmatch(rc_text):
-            raise NonblockError('reader upload exit marker is not an integer: '
-                                + repr(raw_rc))
-        if int(rc_text) != 0:
-            raise NonblockError('reader base64 decode exit=' + rc_text)
-
-        check = self.console.run_shell(
-            'chmod 700 ' + shlex.quote(target) + ' && test -s '
-            + shlex.quote(target), timeout=self.args.command_timeout)
-        if check.exit_code != 0:
-            raise NonblockError('reader chmod/verify exit=' + str(check.exit_code))
+            upload_error = exc
+            failure = {
+                'schema': 'nonblock-pilot-reader-upload-failure-v1',
+                'reader_path': target,
+                'reader_mode': READER_MODE_FOR_LABEL.get(self.args.label),
+                'elf_bytes': len(data),
+                'transfer_bytes': len(encoded),
+                'chunk_bytes': UPLOAD_CHUNK,
+                'chunk_count': len(lines),
+                'lines_attempted': lines_attempted,
+                'bytes_attempted': bytes_attempted,
+                'phase': phase,
+                'error': repr(exc),
+                'auto_retry': False,
+                'resumed': False,
+                'failed_at': utc_now(),
+            }
+            try:
+                self._write_json('upload-failure.json', failure)
+            except OSError as write_exc:
+                self._log('cannot write upload-failure.json: '
+                          + repr(write_exc))
+        finally:
+            if old_write_timeout is not None:
+                try:
+                    self.console._ser.write_timeout = old_write_timeout
+                except Exception as exc:
+                    self._log('reader upload: cannot restore write_timeout: '
+                              + repr(exc))
+
+        if upload_error is not None:
+            self._log('reader upload failed in phase ' + phase + ': '
+                      + repr(upload_error) + ' lines=' + str(lines_attempted)
+                      + ' bytes=' + str(bytes_attempted))
+            raise NonblockError('reader upload failed: ' + repr(upload_error))
 
         record = {
             'schema': 'nonblock-pilot-reader-upload-v1',
             'reader_path': target,
             'reader_mode': READER_MODE_FOR_LABEL.get(self.args.label),
             'elf_bytes': len(data),
             'transfer_bytes': len(encoded),
             'chunk_bytes': UPLOAD_CHUNK,
             'chunk_count': len(lines),
             'mode': '700',
             'hash_readback': False,
             'uploaded_at': utc_now(),
         }
         self._write_json('reader-upload.json', record)
         self._log('reader uploaded: ' + target + ' bytes=' + str(len(encoded))
                   + ' mode=' + str(record['reader_mode']))
         return record
 
     def _run_helper_case(self, cid: str, board_dir: str, local_dir: pathlib.Path,
                          record: Any) -> Tuple[List[str], Optional[List[int]]]:
```

(The `@@ -1044,83 +1050,175 @@` header is the correct line arithmetic for the shown file: old region 1044‑1126 = 83 lines, new = 175; git/patch recomputes nothing, so if you prefer, replace the whole function body instead of trusting the counts.)

---

## Interface / behavior constraints the parent should preserve

- **Serial object contract** (unchanged, re‑used from `mac_research2.SerialConsole`): `self.console._ser` is a pyserial `Serial` with `.write_timeout`; `self.console._write(bytes)` writes+flushes under the console lock; `read_until(marker_bytes, timeout)` / `read_line(timeout)` block on the reader condition. Only `.write_timeout` is touched; `.timeout` (read, 0.1) must stay 0.1.
- **Public return**: on success returns the same `reader-upload.json` dict as before (`schema=nonblock-pilot-reader-upload-v1`, `hash_readback=False`); on failure raises `NonblockError` (so `PilotRunner.run` writes `STOPPED.txt`/summary and does not continue to cases).
- **New artifact**: `upload-failure.json` (`schema=nonblock-pilot-reader-upload-failure-v1`) with `lines_attempted`, `bytes_attempted`, `phase` ∈ {`write-header`,`await-begin`,`write-payload`,`await-end`,`check-exit`,`verify`}, `error`, `auto_retry=false`, `resumed=false`; sizes only — no ELF/base64 content.
- **Markers**: still exactly one `begin` / one `end`; the header line order is `begin printf` then heredoc opener, so the guest is already inside the heredoc only after the begin marker has been read.
- **Guest side effects**: `stty -echo` (already the boot state), `PS2` saved to `__nb_ps2` then cleared, restored inside the verify command (`PS2="${__nb_ps2-}"` before the `&&`). No CTRL‑C, no partial‑run of the helper.

---

## Acceptance checks

Static / host‑only (no board):
1. `python3 -m py_compile scripts/mac_nonblock_pilot.py` — syntax ok.
2. With a stub `_ser` recording calls and a stub console whose `read_until/read_line` return the expected markers/`'0'`:
   - `_ser.write_timeout` equals `UPLOAD_WRITE_TIMEOUT` (=5.0) at every `_write`, and equals 0.1 again after `_upload_reader` returns (and after a forced `read_until`/`_write` exception).
   - `_ser.timeout` (read) is still 0.1 before and after.
   - `read_until(begin)` is invoked before the first payload `_write`.
   - number of raw `_write` calls == `len(header) + len(payload_lines) + 1` (header + payload + `delim`).
   - On a forced failure, `upload-failure.json` exists with `phase`, `lines_attempted`, `bytes_attempted`, `error`; assert none of the ELF bytes / base64 appear in it.

Timing:
3. For 576 base64 lines (415 KiB ELF → ~576 KiB base64): payload wall time ≈ 576 × 0.15 ≈ 86 s (in the 85–100 s band); total including 6 header lines and marker reads ≪ 300 s `--upload-timeout`.

Live (Codex, on hardware):
4. Re‑run with `--reader` (separate `--out …-03`, reusing the serial‑01 flash): upload completes in ~90 s, no `SerialTimeoutException`; after the end marker the guest prompt is a single `#` with no residual `> ` continuation line and no `0x11/0x13`; `chmod 700 && test -s` exit 0; `reader-upload.json` written, no `upload-failure.json`.
5. Failure drill (e.g. unplug/pause): run stops immediately, `upload-failure.json` present and free of ELF/base64, and **no** helper executed. The stuck heredoc is recovered by Codex’s explicit single CTRL‑C + separate serial‑03 launcher sourcing the serial‑01 flash — this is intentionally **not** in this function.

---

## Assumptions / uncertainties / out of scope

- `UPLOAD_LINE_DELAY = 0.15` is chosen so 576 lines land at ~86 s (top of the requested 0.12–0.15 s range, needed to also meet the 85–100 s total). If the real ELF yields a different line count, the per‑line band and the 85–100 s total can disagree; verify the count and, if needed, retune this single constant.
- `UPLOAD_WRITE_TIMEOUT = 5.0` matches “5 s 以上”; a 1025‑byte line is ~89 ms so it should never approach it.
- Assumes the reused console exposes `_ser`/`_write` exactly as in `mac_research2.py` and that the guest shell supports `PS2`, `<<'DELIM'` and `base64 -d`.
- The guest’s XON/XOFF (`0x11/0x13`) flow‑control behaviour is not addressed here (pacing + raised write timeout is the fix); if the board tty still emits them, that is a separate diagnosis for Codex.
- Not touched, by instruction: reader/MMIO code, hardware, kernel, the heredoc‑recovery path, and any hashing/read‑back/unpack of the uploaded ELF.
- This is a proposed patch; it has not been executed, so no test is claimed to have passed.