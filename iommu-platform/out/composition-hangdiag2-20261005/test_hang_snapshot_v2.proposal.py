#!/usr/bin/env python3
"""Offline acceptance tests for the hang-snapshot frame reader/decoder (v1/v2).

These tests are fully offline and synthetic where needed:
  * no network, no XVC server, no JTAG cable, no FPGA build;
  * the version-1 case replays the real recorded JSON under out/;
  * the version-2 cases build a SYNTHETIC 136-byte raw frame by hand, mark it
    clearly as synthetic and keep header/payload_words in sync with
    rawframehex so the decoder's re-derivation is exercised.

Run:
    python3 -m unittest discover -s tests -p 'test_hang_snapshot_v2.py' -v
or
    python3 tests/test_hang_snapshot_v2.py
"""

import contextlib
import importlib.util
import io
import json
import os
import tempfile
import unittest
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
DECODER_PATH = REPO_ROOT / "scripts" / "decode_hang_snapshot.py"
READER_PATH = REPO_ROOT / "scripts" / "read_hang_snapshot.py"
V1_INPUT = (REPO_ROOT / "out" / "composition-hangdiag-20261005"
            / "n4096-hang-snapshot.json")

FRAME_BYTES = 136
PAYLOAD_WORDS = 32
IDCODE = "0x43651093"


def _load_module(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


decoder = _load_module(DECODER_PATH, "decode_hang_snapshot")
reader = _load_module(READER_PATH, "read_hang_snapshot")


# ---------------------------------------------------------------------------
# Synthetic frame helpers (clearly synthetic, never captured from hardware)
# ---------------------------------------------------------------------------
def _build_raw_frame(version, sequence, words, valid=1, ready=1,
                     reqtoggle=1, acksync=1):
    assert len(words) == PAYLOAD_WORDS
    header = decoder.MAGIC & 0xFFFFFFFF
    header |= (version & 0xFF) << 32
    header |= (valid & 1) << 40
    header |= (ready & 1) << 41
    header |= (reqtoggle & 1) << 42
    header |= (acksync & 1) << 43
    header |= (sequence & decoder.SEQ_MASK) << 44
    raw = header.to_bytes(8, "little")
    raw += b"".join((w & 0xFFFFFFFF).to_bytes(4, "little") for w in words)
    assert len(raw) == FRAME_BYTES
    return raw, header


def _sample(raw, header, index=0, validation_errors=None):
    words = [int.from_bytes(raw[8 + 4 * i: 8 + 4 * i + 4], "little")
             for i in range(PAYLOAD_WORDS)]
    return {
        "index": index,
        "timestamp": 0.0,
        "timestamp_iso": "1970-01-01T00:00:00+00:00",
        # synthetic v2 rawframe (built by hand, header/payload kept in sync)
        "synthetic": True,
        "header": {
            "raw": "0x%016X" % header,
            "magic": "0x%08X" % decoder.MAGIC,
            "version": (header >> 32) & 0xFF,
            "valid": bool((header >> 40) & 1),
            "ready": bool((header >> 41) & 1),
            "reqtoggle": (header >> 42) & 1,
            "acksync": (header >> 43) & 1,
            "sequence": (header >> 44) & decoder.SEQ_MASK,
        },
        "payload_words": ["0x%08X" % w for w in words],
        "rawframehex": raw.hex(),
        "payload_hex": raw[8:].hex(),
        "validation_errors": [] if validation_errors is None else validation_errors,
    }


def _doc(version, samples):
    return {
        "host": "127.0.0.1",
        "port": 13721,
        "idcode_only": False,
        "advertised_bytes": 2048,
        "max_bits": 1024,
        "actual_period_ns": 1000,
        "idcode": IDCODE,
        "idcode_masked_match": True,
        "expected_version": version,
        "samples": samples,
        "errors": [],
    }


def _run_decoder(doc):
    fd, path = tempfile.mkstemp(suffix=".json")
    try:
        with os.fdopen(fd, "w", encoding="utf-8") as fh:
            json.dump(doc, fh)
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            code = decoder.main([path])
    finally:
        os.unlink(path)
    return code, json.loads(buf.getvalue())


def _field(entry, name):
    for f in entry["fields"]:
        if f["name"] == name:
            return f
    raise AssertionError("field %s not found in %r" % (name, entry))


def _v2_words():
    """SYNTHETIC v2 payload: words 0..23 are placeholders, 24..31 are crafted.

    Bit positions follow ariane_xilinx.sv:850-910 and the AXI record macro
    layout (ariane_xilinx.sv:842-845).
    """
    words = [0] * PAYLOAD_WORDS

    # words 8..23: four upstream AXI records; set master_DRAM.aw_valid:
    # record word_base 20, bit 96 of the 128-bit record -> word 23, bit 0.
    words[23] = 0x0000_0001

    # words 24..27: post-atomic 'dram' record; set aw_valid (word 27, bit 0)
    words[24] = 0x0000_0000   # ar_addr[31:0]
    words[25] = 0x0000_0000   # ar_addr[63:32]
    words[26] = 0x0000_0040   # aw_addr_low32
    words[27] = 0x0000_0001   # status word, aw_valid = bit 0

    # word 28: AMO state
    amo = 0
    amo |= (1 << 0)      # aw_state_q = WAIT_RESULT_AW
    amo |= (4 << 2)      # w_state_q  = SEND_W
    amo |= (1 << 5)      # b_state_q  = WAIT_COMPLETE_B
    amo |= (2 << 7)      # ar_state_q = SEND_AR
    amo |= (3 << 9)      # r_state_q  = SEND_R
    amo |= (1 << 11)     # w_cnt_q
    amo |= (0 << 12)     # w_cnt_req_q
    amo |= (1 << 13)     # w_cnt_inj_q
    amo |= (1 << 14)     # r_d_valid_q
    amo |= (0 << 15)     # w_d_valid_q
    amo |= (0x2A << 16)  # id_q
    amo |= (0x15 << 22)  # atop_q
    amo |= (2 << 28)     # atop_valid_q = LOAD
    amo |= (1 << 30)     # adapter_ready
    amo |= (0 << 31)     # transaction_collision
    words[28] = amo

    # word 29: DDR xbar W mux state
    mux = 0
    mux |= (0 << 0)              # w_fifo_empty = 0 (not empty)
    mux |= (1 << 1)              # w_fifo_full
    mux |= (2 << 2)              # w_fifo_data
    mux |= (1 << 4)              # lock_aw_valid_q
    mux |= (1 << 5)              # mst_w_valid
    mux |= (0 << 6)              # mst_w_ready
    mux |= (1 << 7)              # mst_w_chan.last
    mux |= (0b0100 << 8)         # slv_w_valids
    mux |= (0b0011 << 12)        # slv_w_readies
    mux |= (0b1000 << 16)        # slv_aw_valids
    mux |= (0b0001 << 20)        # slv_aw_readies
    mux |= (0x5 << 24)           # i_w_fifo.status_cnt_q
    mux |= (1 << 28)             # w_fifo_push
    mux |= (0 << 29)             # w_fifo_pop
    mux |= (1 << 30)             # aw_valid
    mux |= (0 << 31)             # aw_ready
    words[29] = mux

    # word 30: master_DRAM AW metadata + LRSC state + adapter int W signals
    meta = 0
    meta |= (0x21 << 0)    # master_DRAM.aw_id
    meta |= (0x07 << 6)    # master_DRAM.aw_len
    meta |= (0x03 << 14)   # master_DRAM.aw_size
    meta |= (0x15 << 17)   # master_DRAM.aw_atop
    meta |= (1 << 23)      # master_DRAM.aw_lock
    meta |= (6 << 24)      # lrsc.w_state_q = B_INJECT
    meta |= (2 << 27)      # lrsc.r_state_q = R_WAIT_R
    meta |= (1 << 29)      # int_axi_w_valid
    meta |= (0 << 30)      # int_axi_w_ready
    meta |= (1 << 31)      # int_axi_w_last
    words[30] = meta

    # word 31: LRSC saved w_addr low 32
    words[31] = 0xDEADBEEF
    return words


# ---------------------------------------------------------------------------
# Version-1 compatibility against the real recorded input
# ---------------------------------------------------------------------------
class V1CompatibilityTest(unittest.TestCase):
    @unittest.skipUnless(V1_INPUT.is_file(), "recorded v1 input not present")
    def test_real_v1_input_still_decodes_with_six_records(self):
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            code = decoder.main([str(V1_INPUT)])
        out = json.loads(buf.getvalue())
        self.assertEqual(code, 0)
        self.assertTrue(out["all_samples_trusted"])
        self.assertTrue(out["samples"])
        for sample in out["samples"]:
            self.assertEqual(sample["axi_layout_version"], 1)
            self.assertEqual(len(sample["axi_records"]), 6)
            names = [r["name"] for r in sample["axi_records"]]
            self.assertIn("master_DMA_CFG", names)
            self.assertIn("master_IOMMU_CFG", names)
            self.assertIsNone(sample["post_atomics"])


# ---------------------------------------------------------------------------
# Synthetic version-2 cases
# ---------------------------------------------------------------------------
class V2SyntheticTest(unittest.TestCase):
    def test_synthetic_v2_bits_and_enums(self):
        raw, header = _build_raw_frame(2, 11, _v2_words())
        code, out = _run_decoder(_doc(2, [_sample(raw, header)]))
        self.assertEqual(code, 0, out.get("decoder_errors"))
        self.assertTrue(out["all_samples_trusted"])
        s = out["samples"][0]
        self.assertTrue(s["trusted"])
        self.assertEqual(s["axi_layout_version"], 2)

        names = [r["name"] for r in s["axi_records"]]
        self.assertEqual(names, ["slave_CVA6", "slave_IOMMU_COMP",
                                 "slave_IOMMU_MEM", "master_DRAM",
                                 "dram_postatomics"])
        for rec in s["axi_records"]:
            # words 28..31 must never be decoded as a sixth AXI record
            self.assertLess(rec["word_range"][1], 28)

        pa = s["post_atomics"]
        amo = pa["amo_state"]
        self.assertEqual(_field(amo, "aw_state_q")["value"], 1)
        self.assertEqual(_field(amo, "aw_state_q")["enum_name"], "WAIT_RESULT_AW")
        self.assertEqual(_field(amo, "w_state_q")["value"], 4)
        self.assertEqual(_field(amo, "w_state_q")["enum_name"], "SEND_W")
        self.assertEqual(_field(amo, "b_state_q")["enum_name"], "WAIT_COMPLETE_B")
        self.assertEqual(_field(amo, "ar_state_q")["enum_name"], "SEND_AR")
        self.assertEqual(_field(amo, "r_state_q")["enum_name"], "SEND_R")
        self.assertEqual(_field(amo, "atop_valid_q")["value"], 2)
        self.assertEqual(_field(amo, "atop_valid_q")["enum_name"], "LOAD")
        self.assertTrue(_field(amo, "adapter_ready")["value"])
        self.assertFalse(_field(amo, "transaction_collision")["value"])
        self.assertEqual(_field(amo, "id_q")["value"], 0x2A)
        self.assertEqual(_field(amo, "atop_q")["value"], 0x15)

        mux = pa["xbar_w_mux_state"]
        data = _field(mux, "w_fifo_data")
        self.assertEqual(data["value"], 2)
        self.assertTrue(data["meaningful"])
        self.assertEqual(_field(mux, "slv_w_valids")["per_port"]["port2"], True)
        self.assertEqual(_field(mux, "slv_aw_valids")["per_port"]["port3"], True)
        self.assertEqual(_field(mux, "i_w_fifo.status_cnt_q")["value"], 0x5)
        self.assertTrue(_field(mux, "w_fifo_push")["value"])
        self.assertFalse(_field(mux, "w_fifo_pop")["value"])

        meta = pa["dram_aw_lrsc_meta"]
        self.assertTrue(meta["master_dram_aw_valid"])
        self.assertTrue(_field(meta, "master_DRAM.aw_id")["meaningful"])
        self.assertEqual(_field(meta, "master_DRAM.aw_id")["value"], 0x21)
        self.assertEqual(_field(meta, "master_DRAM.aw_len")["value"], 0x07)
        self.assertEqual(_field(meta, "master_DRAM.aw_size")["value"], 0x03)
        self.assertEqual(_field(meta, "master_DRAM.aw_atop")["value"], 0x15)
        self.assertTrue(_field(meta, "master_DRAM.aw_lock")["value"])
        self.assertEqual(_field(meta, "lrsc.w_state_q")["value"], 6)
        self.assertEqual(_field(meta, "lrsc.w_state_q")["enum_name"], "B_INJECT")
        self.assertEqual(_field(meta, "lrsc.r_state_q")["enum_name"], "R_WAIT_R")
        self.assertTrue(_field(meta, "int_axi_w_valid")["value"])
        self.assertFalse(_field(meta, "int_axi_w_ready")["value"])
        self.assertTrue(_field(meta, "int_axi_w_last")["value"])

        saved = pa["lrsc_saved_w_addr"]
        self.assertEqual(saved["lrsc_w_addr_q_low32"], "0xDEADBEEF")

    def test_v2_w_fifo_data_not_meaningful_when_empty(self):
        words = _v2_words()
        words[29] |= (1 << 0)          # w_fifo_empty = 1
        raw, header = _build_raw_frame(2, 12, words)
        code, out = _run_decoder(_doc(2, [_sample(raw, header)]))
        self.assertEqual(code, 0)
        data = _field(out["samples"][0]["post_atomics"]["xbar_w_mux_state"],
                      "w_fifo_data")
        self.assertFalse(data["meaningful"])

    def test_v2_corrupt_payload_word_is_rejected(self):
        raw, header = _build_raw_frame(2, 13, _v2_words())
        sample = _sample(raw, header)
        # deliberately corrupt one reported payload word so it disagrees with
        # rawframehex; the decoder must refuse to trust the sample.
        sample["payload_words"][28] = "0x00000000"
        code, out = _run_decoder(_doc(2, [sample]))
        self.assertNotEqual(code, 0)
        self.assertFalse(out["all_samples_trusted"])
        s = out["samples"][0]
        self.assertFalse(s["trusted"])
        joined = " ".join(s["errors"])
        self.assertIn("payload_words[28]", joined)
        self.assertIn("disagrees with rawframehex", joined)

    def test_unknown_version_is_rejected(self):
        raw, header = _build_raw_frame(3, 14, _v2_words())
        code, out = _run_decoder(_doc(3, [_sample(raw, header)]))
        self.assertNotEqual(code, 0)
        s = out["samples"][0]
        self.assertFalse(s["trusted"])
        joined = " ".join(s["errors"])
        self.assertIn("unsupported header version 3", joined)


# ---------------------------------------------------------------------------
# Reader option surface (structural only; never opens a socket)
# ---------------------------------------------------------------------------
class ReaderOptionTest(unittest.TestCase):
    def test_reader_exposes_expected_versions_1_and_2(self):
        self.assertEqual(tuple(reader.SUPPORTED_VERSIONS), (1, 2))
        self.assertEqual(reader.VERSION, 1)


if __name__ == "__main__":
    unittest.main()
