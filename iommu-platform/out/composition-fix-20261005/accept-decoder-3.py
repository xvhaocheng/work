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
