## What the patch changes (mapped to the four findings)

| Finding | Change | Where |
|---|---|---|
| 1a — top-level `doc.errors` still left `trusted=true` and still produced deltas | new `input_source_errors()` turns every input-level reason into a per-sample `source_errors` list; `trusted` now also requires `not source_errors`, so `build_cross_sample` (which resets `prev` on any untrusted sample) emits no pairs; reader reasons are carried **verbatim** (`reasons = [str(e) for e in input_errors]`, `result["input_errors"]` unchanged) | constants, `input_source_errors`, `decode_sample`, `main`, `_emit` |
| 1b — `idcode_masked_match` never re-checked/enforced | source is re-derived: `_int_like(doc["idcode"]) & 0x0FFFFFFF` must equal `0x03651093` **and** `idcode_masked_match is True` (strict `is True`, so `1`/`"true"` fail); the boolean alone is never accepted | `input_source_errors` |
| 2 — no samples ⇒ `all_samples_trusted=True`, exit 0 | added `bool(result["samples"])` to the trust conjunction; the no-samples message now also lands in `decoder_errors` so the non-zero exit is explained on stderr ("no bus evidence") | `main` |
| 3 — bit 41 treated as unconditional constant marker; diag bit 0 "subset" wording | layout note / error rewritten as *ready is 1 only while `ack == request`* (`hang_snapshot.sv:56-62`); diag bit 0 wording is now only "fewer conditions than the n_window term", no subset/superset and no invalidate-exclusion claim | `decode_header`, `decode_sample`, `NONBLOCK_DIAG_BITS[0]`, `decode_peripherals` notes |
| 4 — no new framework | all changes are local edits inside the existing ~600-line structure; bit maps, offline behaviour, read-only open, stdlib-only are untouched | whole patch |

Behaviour after patch: exit 0 ⇔ `bool(samples) and no untrusted sample and no input_errors and no source_errors and no decoder_errors`. `source_errors` is a new key in the top-level object and in every sample (schema name left at `hang_snapshot_decode/v1` — see open items).

---

## 1. Unified diff — `out/composition-fix-20261005/snapshot-decoder-proposal.py`

Hunk line numbers are computed from the supplied listing; `git apply` / `patch` tolerate small offsets since context is unique.

```diff
--- a/out/composition-fix-20261005/snapshot-decoder-proposal.py
+++ b/out/composition-fix-20261005/snapshot-decoder-proposal.py
@@ -12,9 +12,15 @@
   * every decoded bus value is re-derived from the raw 136-byte frame
     ("rawframehex"), never from the reader's own convenience fields
     ("header", "payload_words", "payload_hex");
-  * exit status is 0 only while every sample is trusted: raw frame length/hex
-    OK, magic/version/valid/marker OK, reqtoggle == acksync, header and
-    payload_words agree with rawframehex, and the reader reported no errors.
+  * exit status is 0 only while every sample is trusted and the JTAG source is
+    verified: raw frame length/hex OK, magic/version/valid OK, ready (bit 41,
+    1 only while ack == request) OK, reqtoggle == acksync, header and
+    payload_words agree with rawframehex, the reader reported no errors, and
+    idcode[27:0] == 0x03651093 with 'idcode_masked_match' strictly true;
+  * a document with no samples, or with a non-empty top-level 'errors', or
+    whose JTAG source cannot be re-verified, yields no trusted sample: every
+    sample is marked untrusted, cross-sample deltas are suppressed and the
+    exit status is non-zero.
 
 Bit-layout authority (RTL, not the reader's fields):
   * cva6/corev_apu/fpga/src/hang_snapshot.sv:53-62
@@ -46,7 +52,12 @@
 SEQ_MASK = 0xFFFFF
 CYCLE_MOD = 1 << 32
 COUNTER_MOD = 1 << 16
 
+# JTAG source identity, re-derived from the raw 'idcode' value; the reader's
+# 'idcode_masked_match' field alone is not accepted as proof of the source.
+IDCODE_LOW28_MASK = 0x0FFFFFFF
+IDCODE_LOW28_EXPECTED = 0x03651093
+
 AXI_RESP_NAMES = {0: "OKAY", 1: "EXOKAY", 2: "SLVERR", 3: "DECERR"}
 
 HEADER_FIELDS = ("raw", "magic", "version", "valid", "ready",
@@ -61,8 +72,8 @@
 # nonblock_diag bits. bit 0 is the TL-wrapper override, bits 1..15 come from
 # rv_iommu_nonblock_hit.diag_o (rv_iommu_tl_wrap.sv:307-308,
 # rv_iommu_nonblock_hit.sv:114-132).
 NONBLOCK_DIAG_BITS = [
-    "tl_wrap: state==PT && e3_ptw_active && en_1S_q && !en_2S_q (TL wrapper override; subset of n_window)",
+    "tl_wrap: state==PT && e3_ptw_active && en_1S_q && !en_2S_q (TL wrapper override; has fewer conditions than the n_window term)",
     "nonblock_hit: window_i",
     "nonblock_hit: window_i && candidate_valid_i",
     "nonblock_hit: start",
@@ -202,12 +213,13 @@
         "reqtoggle": _bits(header_int, 42, 1),
         "acksync": _bits(header_int, 43, 1),
         "sequence": _bits(header_int, 44, 20),
         "layout_note": (
             "bits[31:0]=magic, [39:32]=8'd1 (version), [40]=valid_q, "
-            "[41]=constant 1'b1 (the reader labels it 'ready'), [42]=request_q, "
+            "[41]=1 only while acknowledge_sync_q == request_q at capture, "
+            "else 0 (the reader labels it 'ready'), [42]=request_q, "
             "[43]=acknowledge_sync_q, [63:44]=sequence "
-            "(hang_snapshot.sv:58-59)"
+            "(hang_snapshot.sv:56-62)"
         ),
     }
 
 
@@ -243,8 +255,8 @@
             "diag bit 0 is produced by the TL wrapper (rv_iommu_tl_wrap.sv:307-308), "
             "bits 1..15 come from rv_iommu_nonblock_hit.diag_o "
             "(rv_iommu_nonblock_hit.sv:114-132)",
-            "diag bit 0 is a subset of the TL-wrapper n_window term: it omits the "
-            "bank/implicit-translation/debug/invalidate exclusions of "
-            "rv_iommu_tl_wrap.sv:303-306",
+            "diag bit 0 has fewer conditions than the TL-wrapper n_window term "
+            "(rv_iommu_tl_wrap.sv:303-308): n_window applies additional "
+            "conditions that bit 0 does not",
         ],
     }
 
@@ -417,18 +429,19 @@
 # ---------------------------------------------------------------------------
 # Sample-level decode
 # ---------------------------------------------------------------------------
-def decode_sample(raw_sample, fallback_index):
+def decode_sample(raw_sample, fallback_index, source_errors=()):
     out = {
         "index": fallback_index,
         "input_timestamp": None,
         "input_timestamp_iso": None,
+        "source_errors": [str(x) for x in source_errors],
         "trusted": False,
         "decoder_frame_checks_ok": False,
         "errors": [],
         "input_validation_errors": [],
         "blocked_flags": [],
         "blocked_flags_note": BLOCKED_NOTE,
     }
     if not isinstance(raw_sample, dict):
         out["errors"].append("sample entry is not a JSON object")
         return out
@@ -504,8 +517,9 @@
     if header["version"] != VERSION:
         out["errors"].append("bad version %d, expected %d" % (header["version"], VERSION))
     if not header["valid"]:
         out["errors"].append("valid bit is 0: the frame holds no acknowledged sample")
     if not header["ready"]:
-        out["errors"].append("marker bit (bit 41, labelled 'ready' by the reader) is 0")
+        out["errors"].append(
+            "ready bit (bit 41) is 0: bit 41 is 1 only while ack == request at "
+            "capture (hang_snapshot.sv:57-62), so no acknowledged frame was captured")
     if header["reqtoggle"] != header["acksync"]:
         out["errors"].append("reqtoggle %d != acksync %d"
                              % (header["reqtoggle"], header["acksync"]))
@@ -522,9 +536,33 @@
     ]
     out["blocked_flags"] = collect_blocked_flags(out["axi_records"])
 
     out["decoder_frame_checks_ok"] = not out["errors"]
-    out["trusted"] = out["decoder_frame_checks_ok"] and not out["input_validation_errors"]
+    out["trusted"] = (out["decoder_frame_checks_ok"]
+                      and not out["input_validation_errors"]
+                      and not out["source_errors"])
     return out
 
+
+def input_source_errors(doc, input_errors):
+    """Reasons that make every sample in this document unusable as evidence.
+
+    The reader's own 'errors' are carried over verbatim. The JTAG source is
+    re-derived from the raw 'idcode' value, and the reader's
+    'idcode_masked_match' field must additionally be strictly True.
+    """
+    reasons = [str(e) for e in input_errors]
+    idcode = _int_like(doc.get("idcode"))
+    if idcode is None:
+        reasons.append("input 'idcode' is missing or not integer-like: %r"
+                       % (doc.get("idcode"),))
+    elif (idcode & IDCODE_LOW28_MASK) != IDCODE_LOW28_EXPECTED:
+        reasons.append(
+            "input 'idcode' low 28 bits are 0x%07X, expected 0x%07X"
+            % (idcode & IDCODE_LOW28_MASK, IDCODE_LOW28_EXPECTED))
+    if doc.get("idcode_masked_match") is not True:
+        reasons.append("input 'idcode_masked_match' is not strictly true: %r"
+                       % (doc.get("idcode_masked_match"),))
+    return reasons
+
 
 # ---------------------------------------------------------------------------
 # Output / main
@@ -603,14 +641,16 @@
 def _emit(result, code):
     json.dump(result, sys.stdout, indent=2)
     sys.stdout.write("\n")
     if code != 0:
-        lines = list(result.get("input_errors", [])) + list(result.get("decoder_errors", []))
+        lines = (list(result.get("decoder_errors", []))
+                 + list(result.get("source_errors", [])))
         for s in result.get("samples", []):
             if not s.get("trusted"):
                 lines.extend("sample %s: %s" % (s.get("index"), e) for e in s.get("errors", []))
                 lines.extend("sample %s (reader): %s" % (s.get("index"), e)
                              for e in s.get("input_validation_errors", []))
         sys.stderr.write("decode_hang_snapshot: untrusted input; not valid bus evidence:\n")
         for line in lines:
             sys.stderr.write("  %s\n" % line)
     return code
@@ -626,13 +666,14 @@
     args = parser.parse_args(argv)
 
     result = {
         "tool": "scripts/decode_hang_snapshot.py",
         "schema": "hang_snapshot_decode/v1",
         "input_file": args.input_json,
         "input_errors": [],
+        "source_errors": [],
         "input_metadata": {},
         "decoder_errors": [],
         "samples_decoded": 0,
         "untrusted_sample_indices": [],
         "all_samples_trusted": False,
         "exit_status": None,
@@ -668,29 +709,33 @@
     raw_input_errors = doc.get("errors")
     if raw_input_errors is None:
         raw_input_errors = []
     elif not isinstance(raw_input_errors, list):
         raw_input_errors = [str(raw_input_errors)]
     result["input_errors"] = [str(x) for x in raw_input_errors]
 
     result["input_metadata"] = {
         key: doc.get(key) for key in
         ("host", "port", "idcode_only", "advertised_bytes", "max_bits",
          "actual_period_ns", "idcode", "idcode_masked_match")
     }
+    result["source_errors"] = input_source_errors(doc, result["input_errors"])
 
     samples_in = doc.get("samples")
     if samples_in is None:
         result["decoder_errors"].append("input JSON has no 'samples' field")
         samples_in = []
     elif not isinstance(samples_in, list):
         result["decoder_errors"].append("'samples' is not a list")
         samples_in = []
 
-    result["samples"] = [decode_sample(raw, i) for i, raw in enumerate(samples_in)]
+    result["samples"] = [decode_sample(raw, i, result["source_errors"])
+                         for i, raw in enumerate(samples_in)]
     result["samples_decoded"] = len(result["samples"])
     if not result["samples"]:
-        result["warnings"].append(
-            "input contains no samples (e.g. the reader ran with --idcode-only): "
-            "there is no bus evidence to decode")
+        no_bus_evidence = (
+            "input contains no samples (e.g. the reader ran with --idcode-only): "
+            "there is no bus evidence to decode")
+        result["warnings"].append(no_bus_evidence)
+        result["decoder_errors"].append(no_bus_evidence)
 
     result["cross_sample"] = build_cross_sample(result["samples"])
@@ -697,9 +742,11 @@
 
     untrusted = [s["index"] for s in result["samples"] if not s.get("trusted")]
     result["untrusted_sample_indices"] = untrusted
-    result["all_samples_trusted"] = (not untrusted
-                                     and not result["input_errors"]
-                                     and not result["decoder_errors"])
+    result["all_samples_trusted"] = (bool(result["samples"])
+                                     and not untrusted
+                                     and not result["input_errors"]
+                                     and not result["source_errors"]
+                                     and not result["decoder_errors"])
     code = 0 if result["all_samples_trusted"] else 1
     result["exit_status"] = code
     return _emit(result, code)
```

Notes on the diff:
* `build_cross_sample` is unchanged — it already resets `prev` on an untrusted sample, so once finding 1a makes samples untrusted the deltas disappear. No extra guard was added (minimal patch).
* `_emit` now prints `source_errors` instead of `input_errors`; the reader's strings still appear byte-for-byte (they are the first elements of `source_errors`), so nothing is lost and nothing is duplicated.
* Bit positions, frame geometry, read-only open, stdlib-only and "observations only" wording are untouched.

---

## 2. Acceptance script 1 (finding 1a) — constructed frames + top-level error

`out/composition-fix-20261005/accept_source_errors_block_evidence.py`

```python
#!/usr/bin/env python3
"""Acceptance 1 (finding 1a): a non-empty top-level 'errors' list must make
every sample untrusted and must suppress cross-sample deltas.

Offline only: the frames are constructed here, no board and no JTAG.
Run:  python3 accept_source_errors_block_evidence.py
"""
import json
import os
import subprocess
import sys
import tempfile

DECODER = os.environ.get(
    "DECODER",
    os.path.join(os.path.dirname(os.path.abspath(__file__)),
                 "snapshot-decoder-proposal.py"))

MAGIC = 0x48414E47
IDCODE = 0x03651093          # low 28 bits of the reader's 'idcode'
REASON = "reader: JTAG returned 1360 bits, advertised 1088"


def build_frame(sequence, cycle, cpu_ar, cpu_r_last, dram_ar, dram_r_last,
                ready=1, reqtoggle=1, acksync=1):
    words = [0] * 32
    words[4] = cycle & 0xFFFFFFFF
    words[6] = (cpu_ar & 0xFFFF) | ((cpu_r_last & 0xFFFF) << 16)
    words[7] = (dram_ar & 0xFFFF) | ((dram_r_last & 0xFFFF) << 16)
    header = ((MAGIC & 0xFFFFFFFF) | (1 << 32)          # version 1
              | (1 << 40)                               # valid
              | ((ready & 1) << 41)
              | ((reqtoggle & 1) << 42)
              | ((acksync & 1) << 43)
              | ((sequence & 0xFFFFF) << 44))
    raw = header.to_bytes(8, "little") + b"".join(
        w.to_bytes(4, "little") for w in words)
    assert len(raw) == 136, len(raw)
    return raw


def sample_from_frame(raw, index):
    h = int.from_bytes(raw[:8], "little")
    payload = raw[8:]
    return {
        "index": index,
        "timestamp": float(index),
        "timestamp_iso": "1970-01-01T00:00:00Z",
        "header": {
            "raw": "0x%016X" % h,
            "magic": "0x%08X" % (h & 0xFFFFFFFF),
            "version": (h >> 32) & 0xFF,
            "valid": bool((h >> 40) & 1),
            "ready": bool((h >> 41) & 1),
            "reqtoggle": (h >> 42) & 1,
            "acksync": (h >> 43) & 1,
            "sequence": (h >> 44) & 0xFFFFF,
        },
        "payload_words": [int.from_bytes(payload[4 * i:4 * i + 4], "little")
                          for i in range(32)],
        "payload_hex": payload.hex(),
        "rawframehex": raw.hex(),
        "validation_errors": [],
    }


def make_doc(errors):
    frames = [build_frame(100, 1000, 7, 6, 5, 5),
              build_frame(101, 1016, 9, 6, 6, 5)]
    return {
        "host": "127.0.0.1",
        "port": 3121,
        "idcode_only": False,
        "advertised_bytes": 136,
        "max_bits": 1088,
        "actual_period_ns": 50.0,
        "idcode": "0x%08X" % IDCODE,
        "idcode_masked_match": True,
        "errors": list(errors),
        "samples": [sample_from_frame(f, i) for i, f in enumerate(frames)],
    }


def run_decoder(doc):
    fd, path = tempfile.mkstemp(suffix="-hang.json")
    try:
        with os.fdopen(fd, "w", encoding="utf-8") as fh:
            json.dump(doc, fh)
        proc = subprocess.run([sys.executable, DECODER, path],
                              capture_output=True, text=True)
    finally:
        os.unlink(path)
    try:
        decoded = json.loads(proc.stdout)
    except ValueError:
        print("FAIL: decoder did not print JSON (exit %d)" % proc.returncode)
        print(proc.stdout)
        print(proc.stderr)
        sys.exit(2)
    return proc.returncode, decoded, proc.stderr


def main():
    failures = 0

    def check(ok, what):
        nonlocal failures
        if ok:
            print("ok: %s" % what)
        else:
            print("FAIL: %s" % what)
            failures += 1

    # Positive control: same frames, no top-level error.
    code, decoded, _ = run_decoder(make_doc([]))
    check(code == 0, "control: exit status 0 without top-level errors")
    check(decoded["all_samples_trusted"] is True, "control: all samples trusted")
    check(len(decoded["cross_sample"]["pairs"]) == 1,
          "control: exactly one cross-sample pair")
    if decoded["cross_sample"]["pairs"]:
        deltas = decoded["cross_sample"]["pairs"][0].get("deltas") or {}
        check(deltas.get("cycle_q_delta_mod_2_32") == 16,
              "control: cycle_q delta is 16")
        check(deltas.get("cpu_ar_count_delta_mod_2_16") == 2,
              "control: cpu_ar_count delta is 2")

    # Finding 1a: the reader reported a source error.
    code, decoded, err = run_decoder(make_doc([REASON]))
    check(code != 0, "exit status non-zero when doc.errors is non-empty")
    check(decoded["input_errors"] == [REASON],
          "top-level error string is kept verbatim")
    check(decoded["all_samples_trusted"] is False, "all_samples_trusted is false")
    check(all(s["trusted"] is False for s in decoded["samples"]),
          "every sample is untrusted")
    check(all(REASON in s.get("source_errors", []) for s in decoded["samples"]),
          "the reader reason is attached to every sample, unchanged")
    check(decoded["cross_sample"]["pairs"] == [],
          "no cross-sample deltas are emitted")
    check(REASON in err, "the reason is reported on stderr")

    print("FAILURES: %d" % failures)
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
```

## 3. Acceptance script 2 (finding 2) — no samples, no bus evidence

`out/composition-fix-20261005/accept_no_samples_not_trusted.py`

```python
#!/usr/bin/env python3
"""Acceptance 2 (finding 2): a document without samples must not be trusted,
must exit non-zero and must name the missing bus evidence. Offline only.
Run:  python3 accept_no_samples_not_trusted.py
"""
import json
import os
import subprocess
import sys
import tempfile

DECODER = os.environ.get(
    "DECODER",
    os.path.join(os.path.dirname(os.path.abspath(__file__)),
                 "snapshot-decoder-proposal.py"))

IDCODE = 0x03651093


def base_doc():
    return {
        "host": "127.0.0.1",
        "port": 3121,
        "idcode_only": True,
        "advertised_bytes": 0,
        "max_bits": 1088,
        "actual_period_ns": 50.0,
        "idcode": "0x%08X" % IDCODE,
        "idcode_masked_match": True,
        "errors": [],
    }


def run_decoder(doc):
    fd, path = tempfile.mkstemp(suffix="-hang.json")
    try:
        with os.fdopen(fd, "w", encoding="utf-8") as fh:
            json.dump(doc, fh)
        proc = subprocess.run([sys.executable, DECODER, path],
                              capture_output=True, text=True)
    finally:
        os.unlink(path)
    try:
        decoded = json.loads(proc.stdout)
    except ValueError:
        print("FAIL: decoder did not print JSON (exit %d)" % proc.returncode)
        print(proc.stdout)
        print(proc.stderr)
        sys.exit(2)
    return proc.returncode, decoded, proc.stderr


def main():
    failures = 0

    def check(ok, what):
        nonlocal failures
        if ok:
            print("ok: %s" % what)
        else:
            print("FAIL: %s" % what)
            failures += 1

    # case A: explicit empty sample list
    doc = base_doc()
    doc["samples"] = []
    code, decoded, err = run_decoder(doc)
    check(code != 0, "empty 'samples' list: exit status non-zero")
    check(decoded["all_samples_trusted"] is False,
          "empty 'samples' list: all_samples_trusted is false")
    check(decoded["samples"] == [], "empty 'samples' list: no samples decoded")
    check(decoded["untrusted_sample_indices"] == [],
          "empty 'samples' list: nothing to mark untrusted")
    check(any("no bus evidence" in w for w in decoded["warnings"]),
          "empty 'samples' list: warning names the missing bus evidence")
    check(any("no bus evidence" in e for e in decoded["decoder_errors"]),
          "empty 'samples' list: decoder_errors names the missing bus evidence")
    check(decoded["cross_sample"]["pairs"] == [],
          "empty 'samples' list: no cross-sample deltas")
    check("no bus evidence" in err,
          "empty 'samples' list: stderr explains the non-zero exit")

    # case B: 'samples' field absent altogether
    doc = base_doc()
    code, decoded, err = run_decoder(doc)
    check(code != 0, "missing 'samples' field: exit status non-zero")
    check(decoded["all_samples_trusted"] is False,
          "missing 'samples' field: all_samples_trusted is false")
    check(any("no 'samples' field" in e for e in decoded["decoder_errors"]),
          "missing 'samples' field: reported in decoder_errors")
    check(any("no bus evidence" in e for e in decoded["decoder_errors"]),
          "missing 'samples' field: names the missing bus evidence")

    print("FAILURES: %d" % failures)
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
```

## 4. Acceptance script 3 (findings 1b and 3) — source identity, ready/ack, convenience fields

`out/composition-fix-20261005/accept_source_identity_and_frame_fields.py`

```python
#!/usr/bin/env python3
"""Acceptance 3 (findings 1b and 3): the JTAG source must be re-verified from
the raw 'idcode' (low 28 bits) together with a strictly-true
'idcode_masked_match'; bit 41 is 1 only while ack == request; reader
convenience fields that disagree with the raw frame untrust the sample.

Offline only: frames are constructed here, no board and no JTAG.
Run:  python3 accept_source_identity_and_frame_fields.py
"""
import json
import os
import subprocess
import sys
import tempfile

DECODER = os.environ.get(
    "DECODER",
    os.path.join(os.path.dirname(os.path.abspath(__file__)),
                 "snapshot-decoder-proposal.py"))

MAGIC = 0x48414E47
IDCODE_LOW28 = 0x03651093
WRONG_IDCODE = 0x03651092       # one bit away from the expected low 28 bits
MASKED_GOOD_IDCODE = 0x13651093  # version nibble set, low 28 bits still match


def build_frame(sequence, cycle=0, cpu_ar=0, cpu_r_last=0, dram_ar=0,
                dram_r_last=0, ready=1, reqtoggle=1, acksync=1):
    words = [0] * 32
    words[4] = cycle & 0xFFFFFFFF
    words[6] = (cpu_ar & 0xFFFF) | ((cpu_r_last & 0xFFFF) << 16)
    words[7] = (dram_ar & 0xFFFF) | ((dram_r_last & 0xFFFF) << 16)
    header = ((MAGIC & 0xFFFFFFFF) | (1 << 32)
              | (1 << 40)
              | ((ready & 1) << 41)
              | ((reqtoggle & 1) << 42)
              | ((acksync & 1) << 43)
              | ((sequence & 0xFFFFF) << 44))
    raw = header.to_bytes(8, "little") + b"".join(
        w.to_bytes(4, "little") for w in words)
    assert len(raw) == 136, len(raw)
    return raw


def sample_from_frame(raw, index):
    h = int.from_bytes(raw[:8], "little")
    payload = raw[8:]
    return {
        "index": index,
        "timestamp": float(index),
        "timestamp_iso": "1970-01-01T00:00:00Z",
        "header": {
            "raw": "0x%016X" % h,
            "magic": "0x%08X" % (h & 0xFFFFFFFF),
            "version": (h >> 32) & 0xFF,
            "valid": bool((h >> 40) & 1),
            "ready": bool((h >> 41) & 1),
            "reqtoggle": (h >> 42) & 1,
            "acksync": (h >> 43) & 1,
            "sequence": (h >> 44) & 0xFFFFF,
        },
        "payload_words": [int.from_bytes(payload[4 * i:4 * i + 4], "little")
                          for i in range(32)],
        "payload_hex": payload.hex(),
        "rawframehex": raw.hex(),
        "validation_errors": [],
    }


def make_doc(idcode, masked, frames, mutate=None):
    samples = [sample_from_frame(f, i) for i, f in enumerate(frames)]
    if mutate is not None:
        mutate(samples)
    return {
        "host": "127.0.0.1",
        "port": 3121,
        "idcode_only": False,
        "advertised_bytes": 136,
        "max_bits": 1088,
        "actual_period_ns": 50.0,
        "idcode": "0x%08X" % idcode,
        "idcode_masked_match": masked,
        "errors": [],
        "samples": samples,
    }


def run_decoder(doc):
    fd, path = tempfile.mkstemp(suffix="-hang.json")
    try:
        with os.fdopen(fd, "w", encoding="utf-8") as fh:
            json.dump(doc, fh)
        proc = subprocess.run([sys.executable, DECODER, path],
                              capture_output=True, text=True)
    finally:
        os.unlink(path)
    try:
        decoded = json.loads(proc.stdout)
    except ValueError:
        print("FAIL: decoder did not print JSON (exit %d)" % proc.returncode)
        print(proc.stdout)
        print(proc.stderr)
        sys.exit(2)
    return proc.returncode, decoded, proc.stderr


def main():
    failures = 0

    def check(ok, what):
        nonlocal failures
        if ok:
            print("ok: %s" % what)
        else:
            print("FAIL: %s" % what)
            failures += 1

    good_frames = [
        build_frame(500, cycle=1000, cpu_ar=7, cpu_r_last=6,
                    dram_ar=5, dram_r_last=5),
        build_frame(501, cycle=1016, cpu_ar=9, cpu_r_last=6,
                    dram_ar=6, dram_r_last=5),
    ]

    def doc(idcode=IDCODE_LOW28, masked=True, frames=good_frames, mutate=None):
        return make_doc(idcode, masked, frames, mutate)

    # (a) raw idcode low 28 bits mismatch, convenience flag says True:
    #     the flag alone must not be accepted as proof.
    code, decoded, _ = run_decoder(doc(idcode=WRONG_IDCODE, masked=True))
    check(code != 0, "(a) exit status non-zero for a mismatching idcode")
    check(not decoded["all_samples_trusted"], "(a) all_samples_trusted is false")
    check(all(not s["trusted"] for s in decoded["samples"]),
          "(a) every sample is untrusted")
    check(any("low 28 bits" in e for s in decoded["samples"]
              for e in s["source_errors"]),
          "(a) reason names the idcode low 28 bits")
    check(decoded["cross_sample"]["pairs"] == [],
          "(a) no cross-sample deltas")

    # (b) low 28 bits match (version nibble ignored) but the flag is not a bool
    code, decoded, _ = run_decoder(doc(idcode=MASKED_GOOD_IDCODE, masked="true"))
    check(code != 0, "(b) exit status non-zero when the flag is not strictly true")
    check(all(not s["trusted"] for s in decoded["samples"]),
          "(b) every sample is untrusted")
    check(any("not strictly true" in e for s in decoded["samples"]
              for e in s["source_errors"]),
          "(b) reason names the strict-True requirement")

    # (c) bit 41 == 0 because ack != request
    bad_frames = [
        build_frame(500, cycle=1000, cpu_ar=7, cpu_r_last=6,
                    dram_ar=5, dram_r_last=5, ready=0, reqtoggle=1, acksync=0),
        build_frame(501, cycle=1016, cpu_ar=9, cpu_r_last=6,
                    dram_ar=6, dram_r_last=5, ready=0, reqtoggle=1, acksync=0),
    ]
    code, decoded, _ = run_decoder(doc(frames=bad_frames))
    check(code != 0, "(c) exit status non-zero")
    check(all(not s["trusted"] for s in decoded["samples"]),
          "(c) every sample is untrusted")
    check(all(any("ready bit (bit 41) is 0" in e and "ack == request" in e
                  for e in s["errors"]) for s in decoded["samples"]),
          "(c) the error says bit 41 is 1 only while ack == request")
    check(decoded["cross_sample"]["pairs"] == [], "(c) no cross-sample deltas")

    # (d) reader convenience field 'payload_words' disagrees with rawframehex
    def bump_word(samples):
        samples[0]["payload_words"][4] += 1

    code, decoded, _ = run_decoder(doc(mutate=bump_word))
    check(code != 0, "(d) exit status non-zero")
    check(not decoded["samples"][0]["trusted"] and decoded["samples"][1]["trusted"],
          "(d) only the mutated sample is untrusted")
    check(any("payload_words[4] disagrees" in e
              for e in decoded["samples"][0]["errors"]),
          "(d) the disagreement is reported")
    check(decoded["cross_sample"]["pairs"] == [],
          "(d) no deltas across the broken pair")

    # (e) reader convenience field 'payload_hex' disagrees with rawframehex
    def break_hex(samples):
        samples[1]["payload_hex"] = "00" * 128

    code, decoded, _ = run_decoder(doc(mutate=break_hex))
    check(code != 0, "(e) exit status non-zero")
    check(any("'payload_hex' disagrees" in e
              for e in decoded["samples"][1]["errors"]),
          "(e) the disagreement is reported")

    # (f) verified source and frames: positive control
    code, decoded, _ = run_decoder(doc(idcode=MASKED_GOOD_IDCODE, masked=True))
    check(code == 0, "(f) exit status 0 for a verified source")
    check(decoded["all_samples_trusted"] is True, "(f) all samples trusted")
    pairs = decoded["cross_sample"]["pairs"]
    check(len(pairs) == 1, "(f) exactly one cross-sample pair")
    if pairs and pairs[0].get("deltas"):
        d = pairs[0]["deltas"]
        check(d["cycle_q_delta_mod_2_32"] == 16, "(f) cycle_q delta is 16")
        check(d["cpu_ar_count_delta_mod_2_16"] == 2, "(f) cpu_ar_count delta is 2")
        check(d["dram_ar_count_delta_mod_2_16"] == 1, "(f) dram_ar_count delta is 1")

    print("FAILURES: %d" % failures)
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
```

---

## Assumptions, open issues, verification

Assumptions
1. The reader always emits top-level `idcode` and `idcode_masked_match`; enforcement therefore makes a document *without* them untrusted. If an older reader build omits them, that output will now exit 1 — deliberate per instruction (1), but worth a one-line note in the tool docs.
2. `idcode[27:0] == 0x03651093` and the version nibble is ignored (script 3 case (b)/(f) uses `0x13651093` on purpose).
3. `bit41 == (bit42 == bit43)` per `hang_snapshot.sv:57-62`, so the existing `reqtoggle != acksync` check and the new ready message co-report the same capture failure (two messages, both correct).
4. `source_errors` is new output: top-level key and a per-sample key. `schema` was left at `hang_snapshot_decode/v1` because bumping it was outside the requested patch — flag for the parent if a version bump or a consumer-side note is wanted.

Not verified by me (no execution)
* The three scripts and the patch have not been run. Expected: decoder → `git apply --check` clean (hunk line numbers may be off by ±1; `git apply`/`patch` offset-adjust), then each script prints only `ok:` lines and `FAILURES: 0`, exiting 0. Failing assertions print `FAIL: ...` and exit 1; a decoder that crashes prints the failure plus its stdout/stderr and exits 2.
* Do not treat these scripts as evidence that the tool is correct on real hardware — they only exercise the offline paths named in the task.

Suggested acceptance runs

```sh
cd <repo-root>
git apply --check out/composition-fix-20261005/snapshot-decoder.diff   # or apply the file directly
python3 out/composition-fix-20261005/accept_source_errors_block_evidence.py
python3 out/composition-fix-20261005/accept_no_samples_not_trusted.py
python3 out/composition-fix-20261005/accept_source_identity_and_frame_fields.py
# DECODER=/abs/path/to/snapshot-decoder-proposal.py can override the path.
```