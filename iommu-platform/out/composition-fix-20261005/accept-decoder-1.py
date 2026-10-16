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
