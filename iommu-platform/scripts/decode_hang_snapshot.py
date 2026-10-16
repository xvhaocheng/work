#!/usr/bin/env python3
"""decode_hang_snapshot.py -- read-only offline decoder for read_hang_snapshot.py JSON.

Usage:
    python3 scripts/decode_hang_snapshot.py <read_hang_snapshot.json> > decoded.json

Properties:
  * standard library only;
  * the input path is opened read-only; it is never written, truncated or moved;
  * no network access, no shell, no subprocesses;
  * decoded result goes to stdout as JSON;
  * every decoded bus value is re-derived from the raw 136-byte frame
    ("rawframehex"), never from the reader's own convenience fields
    ("header", "payload_words", "payload_hex");
  * the header version is decoded as 1 or 2 (SUPPORTED_VERSIONS): version 1
    keeps the original six 4-word AXI records; version 2 decodes only the first
    four AXI records plus the post-atomic-adapter 'dram' record (words 24..27)
    and decodes words 28..31 as the AMO/LRSC and DDR-crossbar-mux observables;
    any other version is rejected without assuming an AXI layout;
  * exit status is 0 only while every sample is trusted and the JTAG source is
    verified: raw frame length/hex OK, magic/version/valid OK, ready (bit 41,
    1 only while ack == request) OK, reqtoggle == acksync, header and
    payload_words agree with rawframehex, the reader reported no errors, and
    idcode[27:0] == 0x03651093 with 'idcode_masked_match' strictly true;
  * a document with no samples, or with a non-empty top-level 'errors', or
    whose JTAG source cannot be re-verified, yields no trusted sample: every
    sample is marked untrusted, cross-sample deltas are suppressed and the
    exit status is non-zero.

Legacy bit-layout authority (RTL, not the reader's fields):
The USER4 circuitry was removed from current RTL on 2026-10-06. The paths and
line numbers below refer to the accepted composition-ptwfix-20261005 source
snapshot under out/composition-ptwfix-20261005/fpga-src/, retained for old data.
  * cva6/corev_apu/fpga/src/hang_snapshot.sv:53-62
      shift_q <= {snapshot_q, sequence_q, acknowledge_sync_q, request_q,
                  1'b1, valid_q, 8'd1, 32'h48414e47};
  * cva6/corev_apu/fpga/src/ariane_xilinx.sv:837-851   hang_sample[1023:0]
  * cva6/corev_apu/fpga/src/ariane_xilinx.sv:842-845   `HANG_AXI_RECORD
  * cva6/corev_apu/fpga/src/ariane_xilinx.sv:850-910   frame v2 words 24..31
      (post-atomic 'dram' record, AMO/LRSC/xbar-mux observables)
  * cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:952-959  hang_diag_o[127:0]
  * cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:307-308
  * cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:114-132

This tool reports observations only. It does not decide a root cause and it
cannot prove a deadlock.
"""

import argparse
import json
import sys

# ---------------------------------------------------------------------------
# Constants (mirrors scripts/read_hang_snapshot.py:43-50)
# ---------------------------------------------------------------------------
MAGIC = 0x48414E47
VERSION = 1
SUPPORTED_VERSIONS = (1, 2)
FRAME_BYTES = 136
HEADER_BYTES = 8
PAYLOAD_BYTES = 128
PAYLOAD_WORDS = 32
SEQ_MASK = 0xFFFFF
CYCLE_MOD = 1 << 32
COUNTER_MOD = 1 << 16

# JTAG source identity, re-derived from the raw 'idcode' value; the reader's
# 'idcode_masked_match' field alone is not accepted as proof of the source.
IDCODE_LOW28_MASK = 0x0FFFFFFF
IDCODE_LOW28_EXPECTED = 0x03651093

AXI_RESP_NAMES = {0: "OKAY", 1: "EXOKAY", 2: "SLVERR", 3: "DECERR"}

HEADER_FIELDS = ("raw", "magic", "version", "valid", "ready",
                 "reqtoggle", "acksync", "sequence")

BLOCKED_NOTE = (
    "each entry is a static valid && !ready observation of one sample instant; "
    "ordinary transient AXI backpressure also shows valid && !ready, so this "
    "listing is not by itself evidence of a stall or of a deadlock"
)

# nonblock_diag bits. bit 0 is the TL-wrapper override, bits 1..15 come from
# rv_iommu_nonblock_hit.diag_o (rv_iommu_tl_wrap.sv:307-308,
# rv_iommu_nonblock_hit.sv:114-132).
NONBLOCK_DIAG_BITS = [
    "tl_wrap: state==PT && e3_ptw_active && en_1S_q && !en_2S_q (TL wrapper override; has fewer conditions than the n_window term)",
    "nonblock_hit: window_i",
    "nonblock_hit: window_i && candidate_valid_i",
    "nonblock_hit: start",
    "nonblock_hit: state==DC",
    "nonblock_hit: state==DC && dc_ok && probe_stable",
    "nonblock_hit: state==LEAF",
    "nonblock_hit: state==LEAF && leaf_ok",
    "nonblock_hit: state==DC && ddtc_hit_i && !msi_safe",
    "nonblock_hit: state==LEAF && iotlb_hit_i && !msi_leaf_safe",
    "nonblock_hit: state==LEAF && !iotlb_hit_i",
    "nonblock_hit: accept_o",
    "nonblock_hit: state==DC && !ddtc_hit_i",
    "nonblock_hit: state==DC && ddtc_hit_i && msi_safe && (!dc_ok || !probe_stable)",
    "nonblock_hit: state==LEAF && iotlb_hit_i && msi_leaf_safe && !accept_o",
    "nonblock_hit: enable_i",
]

# Frame v1: six 4-word AXI records, hang_sample[383:256] .. hang_sample[1023:896].
AXI_RECORDS_V1 = [
    {"name": "slave_CVA6", "word_base": 8,
     "rtl": "slave[ariane_soc::CVA6]",
     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:846"},
    {"name": "slave_IOMMU_COMP", "word_base": 12,
     "rtl": "slave[ariane_soc::IOMMU_COMP]",
     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:847"},
    {"name": "slave_IOMMU_MEM", "word_base": 16,
     "rtl": "slave[ariane_soc::IOMMU_MEM] (IOMMU data-structure port)",
     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:848"},
    {"name": "master_DRAM", "word_base": 20,
     "rtl": "master[ariane_soc::DRAM]",
     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:849"},
    {"name": "master_DMA_CFG", "word_base": 24,
     "rtl": "master[ariane_soc::DMA_CFG]",
     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:850"},
    {"name": "master_IOMMU_CFG", "word_base": 28,
     "rtl": "master[ariane_soc::IOMMU_CFG]",
     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:851"},
]

# Frame v2: the first four records (words 8..23) are identical in meaning to v1;
# word 24..27 is the post-atomic-adapter / pre-clock-converter SoC-clock AXI bus
# 'dram'. Words 28..31 are NOT an AXI record in v2.
AXI_RECORDS_V2 = [
    {"name": "slave_CVA6", "word_base": 8,
     "rtl": "slave[ariane_soc::CVA6]",
     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:846"},
    {"name": "slave_IOMMU_COMP", "word_base": 12,
     "rtl": "slave[ariane_soc::IOMMU_COMP]",
     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:847"},
    {"name": "slave_IOMMU_MEM", "word_base": 16,
     "rtl": "slave[ariane_soc::IOMMU_MEM] (IOMMU data-structure port)",
     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:848"},
    {"name": "master_DRAM", "word_base": 20,
     "rtl": "master[ariane_soc::DRAM] (pre-atomic-adapter)",
     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:849"},
    {"name": "dram_postatomics", "word_base": 24,
     "rtl": ("dram (post-atomic-adapter, pre-clock-converter SoC-clock AXI; "
             "i_axi_riscv_atomics mst port)"),
     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:853"},
]

# AXI record layout inside its 128-bit slice:
#   {r_resp, 6'(b_id), 6'(r_id), 6'(ar_id), r_last, r_ready, r_valid,
#    ar_ready, ar_valid, b_ready, b_valid, w_last, w_ready, w_valid,
#    aw_ready, aw_valid, aw_addr[31:0], ar_addr}
# (ariane_xilinx.sv:842-845)
AXI_RECORD_FIELDS = (
    ("ar_addr", 0, 64),
    ("aw_addr_low32", 64, 32),
    ("aw_valid", 96, 1),
    ("aw_ready", 97, 1),
    ("w_valid", 98, 1),
    ("w_ready", 99, 1),
    ("w_last", 100, 1),
    ("b_valid", 101, 1),
    ("b_ready", 102, 1),
    ("ar_valid", 103, 1),
    ("ar_ready", 104, 1),
    ("r_valid", 105, 1),
    ("r_ready", 106, 1),
    ("r_last", 107, 1),
    ("ar_id", 108, 6),
    ("r_id", 114, 6),
    ("b_id", 120, 6),
    ("r_resp", 126, 2),
)

# ---------------------------------------------------------------------------
# Frame v2 words 28..31: AMO / LRSC / DDR-crossbar-mux observables.
#
# All of these are passive samples: ariane_xilinx.sv drives hang_sample from
# existing FSM/register state and nothing in the functional SoC reads it back.
# They are raw register/wire levels at one sample instant; this tool reports
# observations only, does not decide a root cause and cannot prove a deadlock.
#
# Enum names and widths: axi_riscv_amos.sv:148-164, axi_riscv_lrsc.sv:174-179.
# ---------------------------------------------------------------------------

AMO_AW_STATE_NAMES = {0: "FEEDTHROUGH_AW", 1: "WAIT_RESULT_AW", 2: "SEND_AW"}
AMO_W_STATE_NAMES = {0: "FEEDTHROUGH_W", 1: "WAIT_DATA_W", 2: "WAIT_RESULT_W",
                     3: "WAIT_CHANNEL_W", 4: "SEND_W"}
AMO_B_STATE_NAMES = {0: "FEEDTHROUGH_B", 1: "WAIT_COMPLETE_B",
                     2: "WAIT_CHANNEL_B", 3: "SEND_B"}
AMO_AR_STATE_NAMES = {0: "FEEDTHROUGH_AR", 1: "WAIT_CHANNEL_AR", 2: "SEND_AR"}
AMO_R_STATE_NAMES = {0: "FEEDTHROUGH_R", 1: "WAIT_DATA_R",
                     2: "WAIT_CHANNEL_R", 3: "SEND_R"}
AMO_ATOP_VALID_NAMES = {0: "NONE", 1: "INVALID", 2: "LOAD", 3: "STORE"}

LRSC_W_STATE_NAMES = {0: "AW_IDLE", 1: "W_FORWARD", 2: "W_BYPASS",
                      3: "W_WAIT_ART_CLR", 4: "W_DROP", 5: "B_FORWARD",
                      6: "B_INJECT"}
LRSC_R_STATE_NAMES = {0: "R_IDLE", 1: "R_WAIT_AR", 2: "R_WAIT_R"}


def _enum_entry(name, bits, value, table, source):
    return {"name": name, "bits": bits, "value": value,
            "enum_name": table.get(value), "source": source}


def _flag_entry(name, bits, value, source):
    return {"name": name, "bits": bits, "value": bool(value), "source": source}


def _vector_entry(name, bits, value, width, source):
    return {"name": name, "bits": bits, "value": value,
            "hex": _hex(value, width),
            "per_port": {"port%d" % i: bool((value >> i) & 1)
                         for i in range(width)},
            "source": source}


def decode_v2_amo_state(payload):
    """Word 28 = hang_amo_state (ariane_xilinx.sv:858-875, assigned at 907)."""
    w = _bits(payload, 32 * 28, 32)
    aw = _bits(w, 0, 2)
    ws = _bits(w, 2, 3)
    bs = _bits(w, 5, 2)
    ar = _bits(w, 7, 2)
    rs = _bits(w, 9, 2)
    atop_valid = _bits(w, 28, 2)
    return {
        "word_index": 28,
        "raw_word": _hex(w, 32),
        "rtl": ("hang_sample[927:896] = hang_amo_state from "
                "i_axi_riscv_atomics.i_atomics.i_amos "
                "(ariane_xilinx.sv:855-875, 907)"),
        "fields": [
            _enum_entry("aw_state_q", "[1:0]", aw, AMO_AW_STATE_NAMES,
                        "axi_riscv_amos.sv:148-149"),
            _enum_entry("w_state_q", "[4:2]", ws, AMO_W_STATE_NAMES,
                        "axi_riscv_amos.sv:151-152"),
            _enum_entry("b_state_q", "[6:5]", bs, AMO_B_STATE_NAMES,
                        "axi_riscv_amos.sv:154-155"),
            _enum_entry("ar_state_q", "[8:7]", ar, AMO_AR_STATE_NAMES,
                        "axi_riscv_amos.sv:157-158"),
            _enum_entry("r_state_q", "[10:9]", rs, AMO_R_STATE_NAMES,
                        "axi_riscv_amos.sv:160-161"),
            _flag_entry("w_cnt_q", "[11]", _bits(w, 11, 1),
                        "axi_riscv_amos.sv:188 (AXI_MAX_WRITE_TXNS=1 -> 1 bit)"),
            _flag_entry("w_cnt_req_q", "[12]", _bits(w, 12, 1),
                        "axi_riscv_amos.sv:189"),
            _flag_entry("w_cnt_inj_q", "[13]", _bits(w, 13, 1),
                        "axi_riscv_amos.sv:190"),
            _flag_entry("r_d_valid_q", "[14]", _bits(w, 14, 1),
                        "axi_riscv_amos.sv:186"),
            _flag_entry("w_d_valid_q", "[15]", _bits(w, 15, 1),
                        "axi_riscv_amos.sv:185"),
            {"name": "id_q", "bits": "[21:16]", "value": _bits(w, 16, 6),
             "source": "axi_riscv_amos.sv:169"},
            {"name": "atop_q", "bits": "[27:22]", "value": _bits(w, 22, 6),
             "source": "axi_riscv_amos.sv:172"},
            _enum_entry("atop_valid_q", "[29:28]", atop_valid,
                        AMO_ATOP_VALID_NAMES, "axi_riscv_amos.sv:163-164"),
            _flag_entry("adapter_ready", "[30]", _bits(w, 30, 1),
                        "axi_riscv_amos.sv:217-222"),
            _flag_entry("transaction_collision", "[31]", _bits(w, 31, 1),
                        "axi_riscv_amos.sv:257-261"),
        ],
        "notes": [
            "passive observation of the atomic-adapter FSM/registers; nothing "
            "here feeds back into the SoC",
            "raw register levels at one sample instant; not by itself a stall "
            "or deadlock verdict",
        ],
    }


def decode_v2_xbar_w_mux(payload):
    """Word 29 = hang_mux_state (ariane_xilinx.sv:857, 877-893, 908)."""
    w = _bits(payload, 32 * 29, 32)
    empty = _bits(w, 0, 1)
    return {
        "word_index": 29,
        "raw_word": _hex(w, 32),
        "rtl": ("hang_sample[959:928] = hang_mux_state from "
                "i_axi_xbar.i_xbar.gen_mst_port_mux[0].i_axi_mux.gen_mux "
                "(ariane_xilinx.sv:857, 877-893, 908)"),
        "fields": [
            _flag_entry("w_fifo_empty", "[0]", empty,
                        "ariane_xilinx.sv:878"),
            _flag_entry("w_fifo_full", "[1]", _bits(w, 1, 1),
                        "ariane_xilinx.sv:879"),
            {"name": "w_fifo_data", "bits": "[3:2]", "value": _bits(w, 2, 2),
             "source": "ariane_xilinx.sv:880",
             "meaningful": not empty,
             "meaningful_when": "w_fifo_empty == 0",
             "note": ("2-bit head word of the W FIFO; only meaningful while "
                      "w_fifo_empty is 0. With w_fifo_empty == 1 these bits are "
                      "stale FIFO content and must not be read as a value")},
            _flag_entry("lock_aw_valid_q", "[4]", _bits(w, 4, 1),
                        "ariane_xilinx.sv:881"),
            _flag_entry("mst_w_valid", "[5]", _bits(w, 5, 1),
                        "ariane_xilinx.sv:882"),
            _flag_entry("mst_w_ready", "[6]", _bits(w, 6, 1),
                        "ariane_xilinx.sv:883"),
            _flag_entry("mst_w_chan_last", "[7]", _bits(w, 7, 1),
                        "ariane_xilinx.sv:884"),
            _vector_entry("slv_w_valids", "[11:8]", _bits(w, 8, 4), 4,
                          "ariane_xilinx.sv:885"),
            _vector_entry("slv_w_readies", "[15:12]", _bits(w, 12, 4), 4,
                          "ariane_xilinx.sv:886"),
            _vector_entry("slv_aw_valids", "[19:16]", _bits(w, 16, 4), 4,
                          "ariane_xilinx.sv:887"),
            _vector_entry("slv_aw_readies", "[23:20]", _bits(w, 20, 4), 4,
                          "ariane_xilinx.sv:888"),
            {"name": "i_w_fifo.status_cnt_q", "bits": "[27:24]",
             "value": _bits(w, 24, 4), "hex": _hex(_bits(w, 24, 4), 4),
             "source": "ariane_xilinx.sv:889"},
            _flag_entry("w_fifo_push", "[28]", _bits(w, 28, 1),
                        "ariane_xilinx.sv:890"),
            _flag_entry("w_fifo_pop", "[29]", _bits(w, 29, 1),
                        "ariane_xilinx.sv:891"),
            _flag_entry("aw_valid", "[30]", _bits(w, 30, 1),
                        "ariane_xilinx.sv:892"),
            _flag_entry("aw_ready", "[31]", _bits(w, 31, 1),
                        "ariane_xilinx.sv:893"),
        ],
        "notes": [
            "passive observation of the DDR crossbar W-channel mux; nothing "
            "here feeds back into the SoC",
            "per_port entries mark which of the 4 slave ports of this mux "
            "assert the corresponding valid/ready bit",
        ],
    }


def decode_v2_dram_aw_lrsc(payload, dram_record):
    """Word 30 = hang_aw_meta (ariane_xilinx.sv:895-905, 909).

    The master_DRAM.aw_* bits are latched wire levels and are meaningful only
    while master[ariane_soc::DRAM].aw_valid is 1 (word 20..23 record).
    """
    w = _bits(payload, 32 * 30, 32)
    aw_valid = None
    if isinstance(dram_record, dict) and isinstance(dram_record.get("aw_valid"), bool):
        aw_valid = dram_record["aw_valid"]
    meaningful = aw_valid is True
    return {
        "word_index": 30,
        "raw_word": _hex(w, 32),
        "rtl": ("hang_sample[991:960] = hang_aw_meta "
                "(ariane_xilinx.sv:895-905, 909)"),
        "master_dram_aw_valid": aw_valid,
        "fields": [
            {"name": "master_DRAM.aw_id", "bits": "[5:0]",
             "value": _bits(w, 0, 6), "meaningful": meaningful,
             "source": "ariane_xilinx.sv:896"},
            {"name": "master_DRAM.aw_len", "bits": "[13:6]",
             "value": _bits(w, 6, 8), "meaningful": meaningful,
             "source": "ariane_xilinx.sv:897"},
            {"name": "master_DRAM.aw_size", "bits": "[16:14]",
             "value": _bits(w, 14, 3), "meaningful": meaningful,
             "source": "ariane_xilinx.sv:898"},
            {"name": "master_DRAM.aw_atop", "bits": "[22:17]",
             "value": _bits(w, 17, 6), "meaningful": meaningful,
             "source": "ariane_xilinx.sv:899"},
            {"name": "master_DRAM.aw_lock", "bits": "[23]",
             "value": bool(_bits(w, 23, 1)), "meaningful": meaningful,
             "source": "ariane_xilinx.sv:900"},
            _enum_entry("lrsc.w_state_q", "[26:24]", _bits(w, 24, 3),
                        LRSC_W_STATE_NAMES, "axi_riscv_lrsc.sv:177-179"),
            _enum_entry("lrsc.r_state_q", "[28:27]", _bits(w, 27, 2),
                        LRSC_R_STATE_NAMES, "axi_riscv_lrsc.sv:174-175"),
            _flag_entry("int_axi_w_valid", "[29]", _bits(w, 29, 1),
                        "ariane_xilinx.sv:903"),
            _flag_entry("int_axi_w_ready", "[30]", _bits(w, 30, 1),
                        "ariane_xilinx.sv:904"),
            _flag_entry("int_axi_w_last", "[31]", _bits(w, 31, 1),
                        "ariane_xilinx.sv:905"),
        ],
        "notes": [
            "the master_DRAM.aw_* fields are meaningful only while "
            "master[ariane_soc::DRAM].aw_valid is 1 (word 20..23 record); with "
            "aw_valid == 0 those bits are raw idle-AW-channel wire levels and "
            "must not be read as a transaction",
            "int_axi_w_* are the atomic adapter's internal W signals "
            "(ariane_xilinx.sv:903-905)",
        ],
    }


def decode_v2_lrsc_saved_w_addr(payload):
    """Word 31 = HANG_LRSC.w_addr_q[31:0] (ariane_xilinx.sv:910)."""
    w = _bits(payload, 32 * 31, 32)
    return {
        "word_index": 31,
        "raw_word": _hex(w, 32),
        "lrsc_w_addr_q_low32": _hex(w, 32),
        "rtl": ("hang_sample[1023:992] = `HANG_LRSC.w_addr_q[31:0], i.e. "
                "i_axi_riscv_atomics.i_atomics.i_lrsc.w_addr_q "
                "(ariane_xilinx.sv:910)"),
        "notes": [
            "w_addr_q is saved LR/SC state: it is latched from slv_aw_addr_i when "
            "LR/SC accepts a new AW (axi_riscv_lrsc.sv:298-305, registered at "
            "480-488). It is therefore the last accepted AW address, not "
            "necessarily the current AW address and not necessarily valid for "
            "the sampled cycle",
            "only the low 32 bits are captured",
        ],
    }


def decode_v2_post_atomics(payload, dram_record):
    return {
        "note": ("frame version 2 words 28..31: passive AMO/LRSC and DDR "
                 "crossbar-mux observables; these are NOT a sixth AXI record"),
        "amo_state": decode_v2_amo_state(payload),
        "xbar_w_mux_state": decode_v2_xbar_w_mux(payload),
        "dram_aw_lrsc_meta": decode_v2_dram_aw_lrsc(payload, dram_record),
        "lrsc_saved_w_addr": decode_v2_lrsc_saved_w_addr(payload),
    }


# ---------------------------------------------------------------------------
# Small helpers
# ---------------------------------------------------------------------------

def _bits(value, lsb, width):
    return (value >> lsb) & ((1 << width) - 1)


def _hex(value, width_bits):
    return "0x%0*X" % (width_bits // 4, value)


def _int_like(value):
    """Best-effort int view of a JSON scalar (bool, int, or '0x..'/'42' string)."""
    if isinstance(value, bool):
        return int(value)
    if isinstance(value, int):
        return value
    if isinstance(value, float) and value.is_integer():
        return int(value)
    if isinstance(value, str):
        s = value.strip().lower()
        try:
            return int(s, 16) if s.startswith("0x") else int(s, 10)
        except ValueError:
            return None
    return None


# ---------------------------------------------------------------------------
# Raw frame re-derivation and validation
# ---------------------------------------------------------------------------

def parse_raw_frame(raw_hex):
    """Re-derive (header_int, payload_int, payload_words, payload_bytes).

    Raises ValueError with a human-readable reason whenever the frame cannot be
    used as bus evidence.
    """
    if raw_hex is None:
        raise ValueError("sample has no 'rawframehex' field")
    if not isinstance(raw_hex, str):
        raise ValueError("'rawframehex' is not a string")
    s = raw_hex.strip()
    if len(s) != FRAME_BYTES * 2:
        raise ValueError(
            "'rawframehex' is %d hex characters, expected %d (%d bytes)"
            % (len(s), FRAME_BYTES * 2, FRAME_BYTES))
    try:
        raw = bytes.fromhex(s)
    except ValueError as exc:
        raise ValueError("'rawframehex' is not valid hex: %s" % exc) from exc
    if len(raw) != FRAME_BYTES:
        raise ValueError("frame is %d bytes, expected %d" % (len(raw), FRAME_BYTES))
    header_int = int.from_bytes(raw[:HEADER_BYTES], "little")
    payload_bytes = raw[HEADER_BYTES:HEADER_BYTES + PAYLOAD_BYTES]
    payload_int = int.from_bytes(payload_bytes, "little")
    payload_words = [
        int.from_bytes(payload_bytes[4 * i:4 * i + 4], "little")
        for i in range(PAYLOAD_WORDS)
    ]
    return header_int, payload_int, payload_words, payload_bytes


def decode_header(header_int):
    return {
        "raw": _hex(header_int, 64),
        "magic": _hex(_bits(header_int, 0, 32), 32),
        "version": _bits(header_int, 32, 8),
        "valid": bool(_bits(header_int, 40, 1)),
        "ready": bool(_bits(header_int, 41, 1)),
        "reqtoggle": _bits(header_int, 42, 1),
        "acksync": _bits(header_int, 43, 1),
        "sequence": _bits(header_int, 44, 20),
        "layout_note": (
            "bits[31:0]=magic, [39:32]=frame version (1 or 2), [40]=valid_q, "
            "[41]=1 only while acknowledge_sync_q == request_q at capture, "
            "else 0 (the reader labels it 'ready'), [42]=request_q, "
            "[43]=acknowledge_sync_q, [63:44]=sequence "
            "(hang_snapshot.sv:56-62)"
        ),
    }


# ---------------------------------------------------------------------------
# Body decode
# ---------------------------------------------------------------------------

def decode_peripherals(payload):
    w0 = _bits(payload, 0, 32)
    diag = _bits(payload, 0, 16)
    return {
        "word_index": 0,
        "rtl": "hang_sample[31:0] = hang_diag_o[31:0] "
               "(ariane_peripherals_xilinx.sv:953-955; ariane_xilinx.sv:837)",
        "nonblock_diag16": _hex(diag, 16),
        "nonblock_diag_bits": [
            {"bit": i, "value": bool((diag >> i) & 1), "name": NONBLOCK_DIAG_BITS[i]}
            for i in range(16)
        ],
        "nonblock_diag_set_bits": [i for i in range(16) if (diag >> i) & 1],
        "composition_mode": _bits(w0, 16, 2),
        "composition_clear": bool(_bits(w0, 18, 1)),
        "composition_iommu_quiet": bool(_bits(w0, 19, 1)),
        "nonblock_trans_accept": bool(_bits(w0, 20, 1)),
        "nonblock_fire": bool(_bits(w0, 21, 1)),
        "e3_tl_error": bool(_bits(w0, 22, 1)),
        "e3_tl_ptw_active": bool(_bits(w0, 23, 1)),
        "nonblock_did_low4": _hex(_bits(w0, 24, 4), 4),
        "e3_tl_did_low4": _hex(_bits(w0, 28, 4), 4),
        "notes": [
            "only bits[3:0] of nonblock_did / e3_tl.did are captured, the upper "
            "DID bits are not in the snapshot",
            "diag bit 0 is produced by the TL wrapper (rv_iommu_tl_wrap.sv:307-308), "
            "bits 1..15 come from rv_iommu_nonblock_hit.diag_o "
            "(rv_iommu_nonblock_hit.sv:114-132)",
            "diag bit 0 has fewer conditions than the TL-wrapper n_window term "
            "(rv_iommu_tl_wrap.sv:303-308): n_window applies additional "
            "conditions that bit 0 does not",
        ],
    }


def decode_dma_flag_word(payload):
    w1 = _bits(payload, 32, 32)
    specs = (
        ("fullbuffer_jobs", 0,
         "only bits[1:0] are consumed by fullbuffer_jobs_i "
         "(ariane_peripherals_xilinx.sv:1478)"),
        ("job_quiet", 4, None),
        ("batch_active", 8, None),
        ("batch_gap_active", 12, None),
        ("launch_active", 16, None),
        ("obs_issue", 20, None),
        ("obs_done", 24, None),
        ("obs_poll", 28, None),
    )
    out = {
        "word_index": 1,
        "rtl": "hang_sample[63:32] = hang_diag_o[63:32] "
               "(ariane_peripherals_xilinx.sv:956-957)",
        "fields": {},
    }
    for name, lsb, note in specs:
        v = _bits(w1, lsb, 4)
        entry = {
            "raw": v,
            "hex": _hex(v, 4),
            "per_dma": {"dma0": bool(v & 1), "dma1": bool(v & 2),
                        "dma2": bool(v & 4), "dma3": bool(v & 8)},
        }
        if note:
            entry["note"] = note
        out["fields"][name] = entry
    return out


def decode_obs_id_words(payload):
    out = {
        "word_indices": [2, 3],
        "rtl": "hang_sample[95:64] = {obs_done_id[0][15:0], obs_issue_id[0][15:0]}; "
               "hang_sample[127:96] = {obs_done_id[1][15:0], obs_issue_id[1][15:0]} "
               "(ariane_peripherals_xilinx.sv:958-959)",
        "devices": [],
        "note": "only bits[15:0] of the 64-bit obs_issue_id/obs_done_id are captured",
    }
    for dev, word_index in enumerate((2, 3)):
        word = _bits(payload, 32 * word_index, 32)
        out["devices"].append({
            "dma_device": dev,
            "obs_issue_id_low16": _hex(_bits(word, 0, 16), 16),
            "obs_done_id_low16": _hex(_bits(word, 16, 16), 16),
        })
    return out


def decode_counters(payload):
    specs = (
        ("cycle_q", 128, 32,
         "cva6/corev_apu/fpga/src/ariane_xilinx.sv:839 (hang_sample[159:128])",
         "free-running counter, +1 every clk_i edge (50 MHz), wraps at 2^32"),
        ("cpu_last_ar_addr", 160, 32,
         "cva6/corev_apu/fpga/src/ariane_xilinx.sv:839 (hang_sample[191:160])",
         "last accepted CPU-side AR address: slave[CVA6].ar_valid && ar_ready "
         "(ariane_xilinx.sv:825-826); only the low 32 bits are captured"),
        ("cpu_ar_count", 192, 16,
         "cva6/corev_apu/fpga/src/ariane_xilinx.sv:838 (hang_sample[207:192])",
         "counts slave[CVA6] ar_valid && ar_ready fires, wraps at 2^16"),
        ("cpu_r_last_count", 208, 16,
         "cva6/corev_apu/fpga/src/ariane_xilinx.sv:838 (hang_sample[223:208])",
         "counts slave[CVA6] r_valid && r_ready && r_last fires, wraps at 2^16"),
        ("dram_ar_count", 224, 16,
         "cva6/corev_apu/fpga/src/ariane_xilinx.sv:838 (hang_sample[239:224])",
         "counts master[DRAM] ar_valid && ar_ready fires, wraps at 2^16"),
        ("dram_r_last_count", 240, 16,
         "cva6/corev_apu/fpga/src/ariane_xilinx.sv:838 (hang_sample[255:240])",
         "counts master[DRAM] r_valid && r_ready && r_last fires, wraps at 2^16"),
    )
    out = {}
    for name, lsb, width, rtl, note in specs:
        v = _bits(payload, lsb, width)
        out[name] = {"value": v, "hex": _hex(v, width), "rtl": rtl, "note": note}
    return out


def decode_axi_record(name, rtl_desc, rtl_cite, word_base, payload):
    words = [_bits(payload, 32 * (word_base + i), 32) for i in range(4)]
    rec = 0
    for i, w in enumerate(words):
        rec |= w << (32 * i)
    f = {fname: _bits(rec, lsb, width) for fname, lsb, width in AXI_RECORD_FIELDS}

    def flag(n):
        return bool(f[n])

    return {
        "name": name,
        "rtl": rtl_desc,
        "rtl_citation": rtl_cite,
        "word_range": [word_base, word_base + 3],
        "raw_words": [_hex(w, 32) for w in words],

        # addresses
        "ar_addr": _hex(f["ar_addr"], 64),
        "ar_addr_meaningful": flag("ar_valid"),
        "aw_addr_low32": _hex(f["aw_addr_low32"], 32),
        "aw_addr_low32_meaningful": flag("aw_valid"),

        # AW
        "aw_valid": flag("aw_valid"),
        "aw_ready": flag("aw_ready"),
        # W
        "w_valid": flag("w_valid"),
        "w_ready": flag("w_ready"),
        "w_last": flag("w_last"),
        # B
        "b_valid": flag("b_valid"),
        "b_ready": flag("b_ready"),
        "b_id": f["b_id"],
        "b_id_meaningful": flag("b_valid"),
        # AR
        "ar_valid": flag("ar_valid"),
        "ar_ready": flag("ar_ready"),
        "ar_id": f["ar_id"],
        "ar_id_meaningful": flag("ar_valid"),
        # R
        "r_valid": flag("r_valid"),
        "r_ready": flag("r_ready"),
        "r_last": flag("r_last"),
        "r_id": f["r_id"],
        "r_id_meaningful": flag("r_valid"),
        "r_resp": f["r_resp"],
        "r_resp_name": AXI_RESP_NAMES[f["r_resp"]] if flag("r_valid") else None,
        "r_resp_meaningful": flag("r_valid"),

        "blocked_valid_not_ready": {
            "aw": flag("aw_valid") and not flag("aw_ready"),
            "w": flag("w_valid") and not flag("w_ready"),
            "b": flag("b_valid") and not flag("b_ready"),
            "ar": flag("ar_valid") and not flag("ar_ready"),
            "r": flag("r_valid") and not flag("r_ready"),
        },
        "notes": [
            "addresses, ids and response are meaningful only while the matching "
            "x_valid is 1; with x_valid == 0 they are the raw wire levels of an "
            "idle channel and must not be read as a transaction",
            "b_resp is not present in the record macro, only r_resp is captured "
            "(ariane_xilinx.sv:842-845)",
            "b_id / r_id / ar_id are 6 bits (AxiIdWidthSlaves = 4 + clog2(4))",
        ],
    }


def collect_blocked_flags(records):
    out = []
    for rec in records:
        for ch in ("aw", "w", "b", "ar", "r"):
            if rec["blocked_valid_not_ready"][ch]:
                out.append({
                    "axi_record": rec["name"],
                    "channel": ch,
                    "condition": "%s_valid && !%s_ready" % (ch, ch),
                })
    return out


# ---------------------------------------------------------------------------
# Sample-level decode
# ---------------------------------------------------------------------------
def decode_sample(raw_sample, fallback_index, source_errors=()):
    out = {
        "index": fallback_index,
        "input_timestamp": None,
        "input_timestamp_iso": None,
        "source_errors": [str(x) for x in source_errors],
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

    out["index"] = raw_sample.get("index", fallback_index)
    out["input_timestamp"] = raw_sample.get("timestamp")
    out["input_timestamp_iso"] = raw_sample.get("timestamp_iso")

    # Preserve the reader's own validation errors verbatim.
    ive = raw_sample.get("validation_errors")
    if ive is None:
        out["errors"].append("sample has no 'validation_errors' list from the reader")
    elif not isinstance(ive, list):
        out["errors"].append("'validation_errors' is not a list")
    else:
        out["input_validation_errors"] = [str(x) for x in ive]

    # Everything below is re-derived from the raw frame, not from the decoded fields.
    try:
        header_int, payload_int, payload_words, payload_bytes = parse_raw_frame(
            raw_sample.get("rawframehex"))
    except ValueError as exc:
        out["errors"].append("cannot use raw frame: %s" % exc)
        return out

    header = decode_header(header_int)
    out["header"] = header

    in_header = raw_sample.get("header")
    if not isinstance(in_header, dict):
        out["errors"].append("'header' object is missing from the sample")
    else:
        for fname in HEADER_FIELDS:
            if fname not in in_header:
                out["errors"].append("header.%s is missing in the input" % fname)
                continue
            got = _int_like(in_header[fname])
            want = _int_like(header[fname])
            if got is None:
                out["errors"].append(
                    "header.%s is not integer-like: %r" % (fname, in_header[fname]))
            elif got != want:
                out["errors"].append(
                    "header.%s disagrees with rawframehex: input %r vs raw %r"
                    % (fname, in_header[fname], header[fname]))

    in_words = raw_sample.get("payload_words")
    if not isinstance(in_words, list):
        out["errors"].append("'payload_words' is missing or not a list")
    elif len(in_words) != PAYLOAD_WORDS:
        out["errors"].append("'payload_words' has %d entries, expected %d"
                             % (len(in_words), PAYLOAD_WORDS))
    else:
        for i, got_raw in enumerate(in_words):
            got = _int_like(got_raw)
            if got is None:
                out["errors"].append("payload_words[%d] is not integer-like: %r"
                                     % (i, got_raw))
            elif got != payload_words[i]:
                out["errors"].append(
                    "payload_words[%d] disagrees with rawframehex: input %r vs raw 0x%08X"
                    % (i, got_raw, payload_words[i]))

    in_payload_hex = raw_sample.get("payload_hex")
    if isinstance(in_payload_hex, str):
        if in_payload_hex.strip().lower() != payload_bytes.hex():
            out["errors"].append("'payload_hex' disagrees with the payload inside rawframehex")

    # Frame semantics.
    if header["magic"] != _hex(MAGIC, 32):
        out["errors"].append("bad magic %s, expected %s"
                             % (header["magic"], _hex(MAGIC, 32)))
    version = header["version"]
    if version not in SUPPORTED_VERSIONS:
        out["errors"].append(
            "unsupported header version %d: only versions %s are decoded"
            % (version, ", ".join(str(v) for v in SUPPORTED_VERSIONS)))
    if not header["valid"]:
        out["errors"].append("valid bit is 0: the frame holds no acknowledged sample")
    if not header["ready"]:
        out["errors"].append(
            "ready bit (bit 41) is 0: bit 41 is 1 only while ack == request at "
            "capture (hang_snapshot.sv:57-62), so no acknowledged frame was captured")
    if header["reqtoggle"] != header["acksync"]:
        out["errors"].append("reqtoggle %d != acksync %d"
                             % (header["reqtoggle"], header["acksync"]))

    out["peripherals"] = decode_peripherals(payload_int)
    out["dma_flag_vectors"] = decode_dma_flag_word(payload_int)
    out["dma_observation_ids"] = decode_obs_id_words(payload_int)
    out["counters"] = decode_counters(payload_int)
    out["axi_layout_version"] = version
    if version == 1:
        out["axi_records"] = [
            decode_axi_record(rec["name"], rec["rtl"], rec["citation"],
                              rec["word_base"], payload_int)
            for rec in AXI_RECORDS_V1
        ]
        out["axi_records_note"] = (
            "frame version 1: six 4-word AXI records at words 8, 12, 16, 20, 24 "
            "and 28 (the last two are master_DMA_CFG and master_IOMMU_CFG)")
        out["post_atomics"] = None
    elif version == 2:
        out["axi_records"] = [
            decode_axi_record(rec["name"], rec["rtl"], rec["citation"],
                              rec["word_base"], payload_int)
            for rec in AXI_RECORDS_V2
        ]
        out["axi_records_note"] = (
            "frame version 2: five 4-word AXI records at words 8, 12, 16, 20 and "
            "24 (the last is 'dram', the post-atomic-adapter / pre-clock-converter "
            "SoC-clock AXI); words 28..31 are NOT a sixth AXI record, they carry "
            "the AMO/LRSC and DDR-crossbar-mux state decoded under 'post_atomics'")
        out["post_atomics"] = decode_v2_post_atomics(
            payload_int,
            next((r for r in out["axi_records"] if r["name"] == "master_DRAM"),
                 None))
    else:
        out["axi_records"] = []
        out["axi_records_note"] = (
            "frame version %d is not decoded; no AXI layout is assumed" % version)
        out["post_atomics"] = None
    out["blocked_flags"] = collect_blocked_flags(out["axi_records"])

    out["decoder_frame_checks_ok"] = not out["errors"]
    out["trusted"] = (out["decoder_frame_checks_ok"]
                      and not out["input_validation_errors"]
                      and not out["source_errors"])
    return out


def input_source_errors(doc, input_errors):
    """Reasons that make every sample in this document unusable as evidence.

    The reader's own 'errors' are carried over verbatim. The JTAG source is
    re-derived from the raw 'idcode' value, and the reader's
    'idcode_masked_match' field must additionally be strictly True.
    """
    reasons = [str(e) for e in input_errors]
    idcode = _int_like(doc.get("idcode"))
    if idcode is None:
        reasons.append("input 'idcode' is missing or not integer-like: %r"
                       % (doc.get("idcode"),))
    elif (idcode & IDCODE_LOW28_MASK) != IDCODE_LOW28_EXPECTED:
        reasons.append(
            "input 'idcode' low 28 bits are 0x%07X, expected 0x%07X"
            % (idcode & IDCODE_LOW28_MASK, IDCODE_LOW28_EXPECTED))
    if doc.get("idcode_masked_match") is not True:
        reasons.append("input 'idcode_masked_match' is not strictly true: %r"
                       % (doc.get("idcode_masked_match"),))
    return reasons


# ---------------------------------------------------------------------------
# Cross-sample deltas (modulo only, contiguous trusted pairs only)
# ---------------------------------------------------------------------------

def build_cross_sample(samples):
    result = {
        "method": (
            "deltas are computed only between neighbouring samples that are both "
            "'trusted' and whose 20-bit sequence field is contiguous "
            "(next == prev + 1 mod 2^20)"
        ),
        "pairs": [],
        "notes": [
            "differencing two samples yields only a modulo difference: cycle_q is "
            "mod 2^32, the four 16-bit counters are mod 2^16; a wrap inside the "
            "interval is indistinguishable from the reported value, so these "
            "numbers are not absolute elapsed cycles or absolute outstanding counts",
            "no cross-sample conclusion is drawn for a pair when a sample is not "
            "trusted or when the sequence is not contiguous",
        ],
    }
    prev = None
    for cur in samples:
        if not cur.get("trusted"):
            prev = None
            continue
        if prev is None:
            prev = cur
            continue

        seq_prev = prev["header"]["sequence"]
        seq_cur = cur["header"]["sequence"]
        expected = (seq_prev + 1) & SEQ_MASK
        entry = {
            "from_index": prev["index"],
            "to_index": cur["index"],
            "sequence_from": seq_prev,
            "sequence_to": seq_cur,
            "sequence_contiguous": seq_cur == expected,
        }
        if seq_cur != expected:
            entry["deltas"] = None
            entry["note"] = (
                "sequence %d -> %d is not contiguous (expected %d): no cross-sample "
                "variation is reported for this pair"
                % (seq_prev, seq_cur, expected))
        else:
            pc, cc = prev["counters"], cur["counters"]
            entry["deltas"] = {
                "cycle_q_delta_mod_2_32":
                    (cc["cycle_q"]["value"] - pc["cycle_q"]["value"]) % CYCLE_MOD,
                "cpu_ar_count_delta_mod_2_16":
                    (cc["cpu_ar_count"]["value"] - pc["cpu_ar_count"]["value"]) % COUNTER_MOD,
                "cpu_r_last_count_delta_mod_2_16":
                    (cc["cpu_r_last_count"]["value"] - pc["cpu_r_last_count"]["value"]) % COUNTER_MOD,
                "dram_ar_count_delta_mod_2_16":
                    (cc["dram_ar_count"]["value"] - pc["dram_ar_count"]["value"]) % COUNTER_MOD,
                "dram_r_last_count_delta_mod_2_16":
                    (cc["dram_r_last_count"]["value"] - pc["dram_r_last_count"]["value"]) % COUNTER_MOD,
            }
            entry["note"] = (
                "modulo differences only: cycle_q mod 2^32 and 16-bit counters "
                "mod 2^16; this pair does not prove that nothing wrapped and does "
                "not give an absolute backlog")
        result["pairs"].append(entry)
        prev = cur
    return result


# ---------------------------------------------------------------------------
# Output / main
# ---------------------------------------------------------------------------

def _emit(result, code):
    json.dump(result, sys.stdout, indent=2)
    sys.stdout.write("\n")
    if code != 0:
        lines = (list(result.get("decoder_errors", []))
                 + list(result.get("source_errors", [])))
        for s in result.get("samples", []):
            if not s.get("trusted"):
                lines.extend("sample %s: %s" % (s.get("index"), e) for e in s.get("errors", []))
                lines.extend("sample %s (reader): %s" % (s.get("index"), e)
                             for e in s.get("input_validation_errors", []))
        sys.stderr.write("decode_hang_snapshot: untrusted input; not valid bus evidence:\n")
        for line in lines:
            sys.stderr.write("  %s\n" % line)
    return code


def main(argv=None):
    parser = argparse.ArgumentParser(
        description=("Offline read-only decoder for the JSON written by "
                     "scripts/read_hang_snapshot.py. Stdlib only; the input file "
                     "is opened read-only and never modified."))
    parser.add_argument("input_json", metavar="read_hang_snapshot.json",
                        help="JSON produced by scripts/read_hang_snapshot.py")
    args = parser.parse_args(argv)

    result = {
        "tool": "scripts/decode_hang_snapshot.py",
        "schema": "hang_snapshot_decode/v1",
        "input_file": args.input_json,
        "input_errors": [],
        "source_errors": [],
        "input_metadata": {},
        "decoder_errors": [],
        "samples_decoded": 0,
        "untrusted_sample_indices": [],
        "all_samples_trusted": False,
        "exit_status": None,
        "warnings": [
            "decoded values are raw register/wire contents from static JTAG "
            "samples; this tool does not decide a root cause and cannot prove a "
            "deadlock",
            "values flagged *_meaningful == false (x_valid == 0) are raw idle-channel "
            "wire levels and must not be read as a transaction",
            "blocked_valid_not_ready entries can be ordinary transient AXI "
            "backpressure; a single snapshot is not a stall verdict",
            "cross-sample deltas are modulo differences (cycle mod 2^32, counters "
            "mod 2^16); an undetected wrap cannot be excluded",
        ],
        "samples": [],
        "cross_sample": {"pairs": [], "notes": []},
    }

    try:
        with open(args.input_json, "r", encoding="utf-8") as fh:
            doc = json.load(fh)
    except OSError as exc:
        result["decoder_errors"].append("cannot read input file: %s" % exc)
        return _emit(result, 1)
    except json.JSONDecodeError as exc:
        result["decoder_errors"].append("input file is not valid JSON: %s" % exc)
        return _emit(result, 1)

    if not isinstance(doc, dict):
        result["decoder_errors"].append("top-level JSON value is not an object")
        return _emit(result, 1)

    raw_input_errors = doc.get("errors")
    if raw_input_errors is None:
        raw_input_errors = []
    elif not isinstance(raw_input_errors, list):
        raw_input_errors = [str(raw_input_errors)]
    result["input_errors"] = [str(x) for x in raw_input_errors]

    result["input_metadata"] = {
        key: doc.get(key) for key in
        ("host", "port", "idcode_only", "advertised_bytes", "max_bits",
         "actual_period_ns", "idcode", "idcode_masked_match",
         "expected_version")
    }
    result["source_errors"] = input_source_errors(doc, result["input_errors"])

    samples_in = doc.get("samples")
    if samples_in is None:
        result["decoder_errors"].append("input JSON has no 'samples' field")
        samples_in = []
    elif not isinstance(samples_in, list):
        result["decoder_errors"].append("'samples' is not a list")
        samples_in = []

    result["samples"] = [decode_sample(raw, i, result["source_errors"])
                         for i, raw in enumerate(samples_in)]
    result["samples_decoded"] = len(result["samples"])
    if not result["samples"]:
        no_bus_evidence = (
            "input contains no samples (e.g. the reader ran with --idcode-only): "
            "there is no bus evidence to decode")
        result["warnings"].append(no_bus_evidence)
        result["decoder_errors"].append(no_bus_evidence)

    result["cross_sample"] = build_cross_sample(result["samples"])

    untrusted = [s["index"] for s in result["samples"] if not s.get("trusted")]
    result["untrusted_sample_indices"] = untrusted
    result["all_samples_trusted"] = (bool(result["samples"])
                                     and not untrusted
                                     and not result["input_errors"]
                                     and not result["source_errors"]
                                     and not result["decoder_errors"])
    code = 0 if result["all_samples_trusted"] else 1
    result["exit_status"] = code
    return _emit(result, code)


if __name__ == "__main__":
    sys.exit(main())
