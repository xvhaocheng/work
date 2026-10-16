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
