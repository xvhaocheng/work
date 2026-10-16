Below is a bounded proposal: complete drop‑in diffs for the two existing scripts plus one new offline test file. **I did not execute anything**, so nothing here is claimed as passing; the acceptance commands at the end are for Codex to run.

Assumptions I had to make (flagging explicitly):

- The reader’s “read_snapshot” function is `request_and_read_sample`; I added its optional `expected_version` parameter there.
- Frame geometry is unchanged (1088 bits / 136 bytes, header 64 bits, 32 payload words); only the version byte and payload word meanings differ. I did not touch XVC framing, TAP steps, cleanup, IDCODE rev mask, sequence or raw-evidence logic.
- v2 word mapping taken verbatim from `ariane_xilinx.sv:850-915`: words 0..23 unchanged, words 24..27 = `HANG_AXI_RECORD(dram)` (post‑atomic‑adapter, pre‑clock‑converter SoC‑clock AXI), word 28 = `hang_amo_state`, word 29 = `hang_mux_state`, word 30 = `hang_aw_meta`, word 31 = `HANG_LRSC.w_addr_q[31:0]`.
- State enum names come from `axi_riscv_amos.sv:148-164` and `axi_riscv_lrsc.sv:174-179`.

---

## 1. `scripts/read_hang_snapshot.py` — unified diff

```diff
--- a/scripts/read_hang_snapshot.py
+++ b/scripts/read_hang_snapshot.py
@@ -45,6 +45,11 @@
 PAYLOAD_BITS = 1024
 PAYLOAD_WORDS = 32
 MAGIC = 0x48414E47
 VERSION = 1
+# Frame versions this reader accepts for --expected-version. Both share the
+# 1088-bit / 136-byte frame and the header field positions; only the version
+# byte and the meaning of the payload words differ.
+SUPPORTED_VERSIONS = (1, 2)
 
 SEQ_MASK = 0xFFFFF  # 20-bit sequence field
 MIN_ADVERTISED_BYTES = 16
@@ -301,7 +306,7 @@
-def request_and_read_sample(client, index):
+def request_and_read_sample(client, index, expected_version=VERSION):
     """Perform one USER4 sample request and read. Returns dict with frame data
     and a list of validation errors. Does not raise on validation errors;
     caller records the sample and then fails."""
@@ -334,8 +339,8 @@
     if magic != MAGIC:
         errors.append(f"bad magic 0x{magic:08X}, expected 0x{MAGIC:08X}")
-    if version != VERSION:
-        errors.append(f"bad version {version}, expected {VERSION}")
+    if version != expected_version:
+        errors.append(f"bad version {version}, expected {expected_version}")
     if not valid:
         errors.append("valid bit not set")
     if not ready:
@@ -382,6 +387,10 @@
     parser.add_argument("--samples", type=int, default=DEFAULT_SAMPLES,
                         help=f"Number of samples (default {DEFAULT_SAMPLES}, min {MIN_SAMPLES}, max {MAX_SAMPLES})")
+    parser.add_argument("--expected-version", type=int,
+                        choices=list(SUPPORTED_VERSIONS), default=VERSION,
+                        help=f"Expected frame header version (default {VERSION}; "
+                             f"choices {', '.join(str(v) for v in SUPPORTED_VERSIONS)})")
     args = parser.parse_args()
 
     if args.samples < MIN_SAMPLES or args.samples > MAX_SAMPLES:
@@ -390,6 +399,7 @@
     result = {
         "host": args.host,
         "port": args.port,
         "idcode_only": args.idcode_only,
+        "expected_version": args.expected_version,
         "advertised_bytes": None,
         "max_bits": None,
         "actual_period_ns": None,
@@ -425,7 +435,7 @@
         if not args.idcode_only:
             prev_seq = None
             for i in range(args.samples):
-                sample = request_and_read_sample(client, i)
+                sample = request_and_read_sample(client, i, args.expected_version)
                 result["samples"].append(sample)
                 if sample["validation_errors"]:
                     raise ValidationError(
```

Nothing else changes: XVC packet splits, TAP reset/idle timing, cleanup BYPASS gating, IDCODE mask tolerance, sequence continuity and the raw evidence (`header`, `payload_words`, `rawframehex`, `payload_hex`, `validation_errors`) are untouched. The JSON now also carries a top‑level `"expected_version"` metadata field.

---

## 2. `scripts/decode_hang_snapshot.py` — unified diff

### 2a. Docstring authority/behaviour additions

```diff
--- a/scripts/decode_hang_snapshot.py
+++ b/scripts/decode_hang_snapshot.py
@@ -12,6 +12,11 @@
   * every decoded bus value is re-derived from the raw 136-byte frame
     ("rawframehex"), never from the reader's own convenience fields
     ("header", "payload_words", "payload_hex");
+  * the header version is decoded as 1 or 2 (SUPPORTED_VERSIONS): version 1
+    keeps the original six 4-word AXI records; version 2 decodes only the first
+    four AXI records plus the post-atomic-adapter 'dram' record (words 24..27)
+    and decodes words 28..31 as the AMO/LRSC and DDR-crossbar-mux observables;
+    any other version is rejected without assuming an AXI layout;
   * exit status is 0 only while every sample is trusted and the JTAG source is
     verified: raw frame length/hex OK, magic/version/valid OK, ready (bit 41,
@@ -29,6 +34,9 @@
   * cva6/corev_apu/fpga/src/ariane_xilinx.sv:837-851   hang_sample[1023:0]
   * cva6/corev_apu/fpga/src/ariane_xilinx.sv:842-845   `HANG_AXI_RECORD
+  * cva6/corev_apu/fpga/src/ariane_xilinx.sv:850-910   frame v2 words 24..31
+      (post-atomic 'dram' record, AMO/LRSC/xbar-mux observables)
   * cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:952-959  hang_diag_o[127:0]
```

### 2b. Constants

```diff
@@ -45,6 +53,7 @@
 # ---------------------------------------------------------------------------
 MAGIC = 0x48414E47
 VERSION = 1
+SUPPORTED_VERSIONS = (1, 2)
 FRAME_BYTES = 136
 HEADER_BYTES = 8
```

### 2c. Two AXI record lists instead of one

```diff
@@ -91,22 +100,42 @@
-# Six 4-word AXI records, hang_sample[383:256] .. hang_sample[1023:896].
-AXI_RECORDS = [
+# Frame v1: six 4-word AXI records, hang_sample[383:256] .. hang_sample[1023:896].
+AXI_RECORDS_V1 = [
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
 
+# Frame v2: the first four records (words 8..23) are identical in meaning to v1;
+# word 24..27 is the post-atomic-adapter / pre-clock-converter SoC-clock AXI bus
+# 'dram'. Words 28..31 are NOT an AXI record in v2.
+AXI_RECORDS_V2 = [
+    {"name": "slave_CVA6", "word_base": 8,
+     "rtl": "slave[ariane_soc::CVA6]",
+     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:846"},
+    {"name": "slave_IOMMU_COMP", "word_base": 12,
+     "rtl": "slave[ariane_soc::IOMMU_COMP]",
+     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:847"},
+    {"name": "slave_IOMMU_MEM", "word_base": 16,
+     "rtl": "slave[ariane_soc::IOMMU_MEM] (IOMMU data-structure port)",
+     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:848"},
+    {"name": "master_DRAM", "word_base": 20,
+     "rtl": "master[ariane_soc::DRAM] (pre-atomic-adapter)",
+     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:849"},
+    {"name": "dram_postatomics", "word_base": 24,
+     "rtl": ("dram (post-atomic-adapter, pre-clock-converter SoC-clock AXI; "
+             "i_axi_riscv_atomics mst port)"),
+     "citation": "cva6/corev_apu/fpga/src/ariane_xilinx.sv:853"},
+]
+
```

### 2d. New v2 word 28..31 decoders (inserted after `AXI_RECORD_FIELDS`)

```diff
@@ -140,6 +169,178 @@
 )
 
+# ---------------------------------------------------------------------------
+# Frame v2 words 28..31: AMO / LRSC / DDR-crossbar-mux observables.
+#
+# All of these are passive samples: ariane_xilinx.sv drives hang_sample from
+# existing FSM/register state and nothing in the functional SoC reads it back.
+# They are raw register/wire levels at one sample instant; this tool reports
+# observations only, does not decide a root cause and cannot prove a deadlock.
+#
+# Enum names and widths: axi_riscv_amos.sv:148-164, axi_riscv_lrsc.sv:174-179.
+# ---------------------------------------------------------------------------
+
+AMO_AW_STATE_NAMES = {0: "FEEDTHROUGH_AW", 1: "WAIT_RESULT_AW", 2: "SEND_AW"}
+AMO_W_STATE_NAMES = {0: "FEEDTHROUGH_W", 1: "WAIT_DATA_W", 2: "WAIT_RESULT_W",
+                     3: "WAIT_CHANNEL_W", 4: "SEND_W"}
+AMO_B_STATE_NAMES = {0: "FEEDTHROUGH_B", 1: "WAIT_COMPLETE_B",
+                     2: "WAIT_CHANNEL_B", 3: "SEND_B"}
+AMO_AR_STATE_NAMES = {0: "FEEDTHROUGH_AR", 1: "WAIT_CHANNEL_AR", 2: "SEND_AR"}
+AMO_R_STATE_NAMES = {0: "FEEDTHROUGH_R", 1: "WAIT_DATA_R",
+                     2: "WAIT_CHANNEL_R", 3: "SEND_R"}
+AMO_ATOP_VALID_NAMES = {0: "NONE", 1: "INVALID", 2: "LOAD", 3: "STORE"}
+
+LRSC_W_STATE_NAMES = {0: "AW_IDLE", 1: "W_FORWARD", 2: "W_BYPASS",
+                      3: "W_WAIT_ART_CLR", 4: "W_DROP", 5: "B_FORWARD",
+                      6: "B_INJECT"}
+LRSC_R_STATE_NAMES = {0: "R_IDLE", 1: "R_WAIT_AR", 2: "R_WAIT_R"}
+
+
+def _enum_entry(name, bits, value, table, source):
+    return {"name": name, "bits": bits, "value": value,
+            "enum_name": table.get(value), "source": source}
+
+
+def _flag_entry(name, bits, value, source):
+    return {"name": name, "bits": bits, "value": bool(value), "source": source}
+
+
+def _vector_entry(name, bits, value, width, source):
+    return {"name": name, "bits": bits, "value": value,
+            "hex": _hex(value, width),
+            "per_port": {"port%d" % i: bool((value >> i) & 1)
+                         for i in range(width)},
+            "source": source}
+
+
+def decode_v2_amo_state(payload):
+    """Word 28 = hang_amo_state (ariane_xilinx.sv:858-875, assigned at 907)."""
+    w = _bits(payload, 32 * 28, 32)
+    aw = _bits(w, 0, 2)
+    ws = _bits(w, 2, 3)
+    bs = _bits(w, 5, 2)
+    ar = _bits(w, 7, 2)
+    rs = _bits(w, 9, 2)
+    atop_valid = _bits(w, 28, 2)
+    return {
+        "word_index": 28,
+        "raw_word": _hex(w, 32),
+        "rtl": ("hang_sample[927:896] = hang_amo_state from "
+                "i_axi_riscv_atomics.i_atomics.i_amos "
+                "(ariane_xilinx.sv:855-875, 907)"),
+        "fields": [
+            _enum_entry("aw_state_q", "[1:0]", aw, AMO_AW_STATE_NAMES,
+                        "axi_riscv_amos.sv:148-149"),
+            _enum_entry("w_state_q", "[4:2]", ws, AMO_W_STATE_NAMES,
+                        "axi_riscv_amos.sv:151-152"),
+            _enum_entry("b_state_q", "[6:5]", bs, AMO_B_STATE_NAMES,
+                        "axi_riscv_amos.sv:154-155"),
+            _enum_entry("ar_state_q", "[8:7]", ar, AMO_AR_STATE_NAMES,
+                        "axi_riscv_amos.sv:157-158"),
+            _enum_entry("r_state_q", "[10:9]", rs, AMO_R_STATE_NAMES,
+                        "axi_riscv_amos.sv:160-161"),
+            _flag_entry("w_cnt_q", "[11]", _bits(w, 11, 1),
+                        "axi_riscv_amos.sv:188 (AXI_MAX_WRITE_TXNS=1 -> 1 bit)"),
+            _flag_entry("w_cnt_req_q", "[12]", _bits(w, 12, 1),
+                        "axi_riscv_amos.sv:189"),
+            _flag_entry("w_cnt_inj_q", "[13]", _bits(w, 13, 1),
+                        "axi_riscv_amos.sv:190"),
+            _flag_entry("r_d_valid_q", "[14]", _bits(w, 14, 1),
+                        "axi_riscv_amos.sv:186"),
+            _flag_entry("w_d_valid_q", "[15]", _bits(w, 15, 1),
+                        "axi_riscv_amos.sv:185"),
+            {"name": "id_q", "bits": "[21:16]", "value": _bits(w, 16, 6),
+             "source": "axi_riscv_amos.sv:169"},
+            {"name": "atop_q", "bits": "[27:22]", "value": _bits(w, 22, 6),
+             "source": "axi_riscv_amos.sv:172"},
+            _enum_entry("atop_valid_q", "[29:28]", atop_valid,
+                        AMO_ATOP_VALID_NAMES, "axi_riscv_amos.sv:163-164"),
+            _flag_entry("adapter_ready", "[30]", _bits(w, 30, 1),
+                        "axi_riscv_amos.sv:217-222"),
+            _flag_entry("transaction_collision", "[31]", _bits(w, 31, 1),
+                        "axi_riscv_amos.sv:257-261"),
+        ],
+        "notes": [
+            "passive observation of the atomic-adapter FSM/registers; nothing "
+            "here feeds back into the SoC",
+            "raw register levels at one sample instant; not by itself a stall "
+            "or deadlock verdict",
+        ],
+    }
+
+
+def decode_v2_xbar_w_mux(payload):
+    """Word 29 = hang_mux_state (ariane_xilinx.sv:857, 877-893, 908)."""
+    w = _bits(payload, 32 * 29, 32)
+    empty = _bits(w, 0, 1)
+    return {
+        "word_index": 29,
+        "raw_word": _hex(w, 32),
+        "rtl": ("hang_sample[959:928] = hang_mux_state from "
+                "i_axi_xbar.i_xbar.gen_mst_port_mux[0].i_axi_mux.gen_mux "
+                "(ariane_xilinx.sv:857, 877-893, 908)"),
+        "fields": [
+            _flag_entry("w_fifo_empty", "[0]", empty,
+                        "ariane_xilinx.sv:878"),
+            _flag_entry("w_fifo_full", "[1]", _bits(w, 1, 1),
+                        "ariane_xilinx.sv:879"),
+            {"name": "w_fifo_data", "bits": "[3:2]", "value": _bits(w, 2, 2),
+             "source": "ariane_xilinx.sv:880",
+             "meaningful": not empty,
+             "meaningful_when": "w_fifo_empty == 0",
+             "note": ("2-bit head word of the W FIFO; only meaningful while "
+                      "w_fifo_empty is 0. With w_fifo_empty == 1 these bits are "
+                      "stale FIFO content and must not be read as a value")},
+            _flag_entry("lock_aw_valid_q", "[4]", _bits(w, 4, 1),
+                        "ariane_xilinx.sv:881"),
+            _flag_entry("mst_w_valid", "[5]", _bits(w, 5, 1),
+                        "ariane_xilinx.sv:882"),
+            _flag_entry("mst_w_ready", "[6]", _bits(w, 6, 1),
+                        "ariane_xilinx.sv:883"),
+            _flag_entry("mst_w_chan_last", "[7]", _bits(w, 7, 1),
+                        "ariane_xilinx.sv:884"),
+            _vector_entry("slv_w_valids", "[11:8]", _bits(w, 8, 4), 4,
+                          "ariane_xilinx.sv:885"),
+            _vector_entry("slv_w_readies", "[15:12]", _bits(w, 12, 4), 4,
+                          "ariane_xilinx.sv:886"),
+            _vector_entry("slv_aw_valids", "[19:16]", _bits(w, 16, 4), 4,
+                          "ariane_xilinx.sv:887"),
+            _vector_entry("slv_aw_readies", "[23:20]", _bits(w, 20, 4), 4,
+                          "ariane_xilinx.sv:888"),
+            {"name": "i_w_fifo.status_cnt_q", "bits": "[27:24]",
+             "value": _bits(w, 24, 4), "hex": _hex(_bits(w, 24, 4), 4),
+             "source": "ariane_xilinx.sv:889"},
+            _flag_entry("w_fifo_push", "[28]", _bits(w, 28, 1),
+                        "ariane_xilinx.sv:890"),
+            _flag_entry("w_fifo_pop", "[29]", _bits(w, 29, 1),
+                        "ariane_xilinx.sv:891"),
+            _flag_entry("aw_valid", "[30]", _bits(w, 30, 1),
+                        "ariane_xilinx.sv:892"),
+            _flag_entry("aw_ready", "[31]", _bits(w, 31, 1),
+                        "ariane_xilinx.sv:893"),
+        ],
+        "notes": [
+            "passive observation of the DDR crossbar W-channel mux; nothing "
+            "here feeds back into the SoC",
+            "per_port entries mark which of the 4 slave ports of this mux "
+            "assert the corresponding valid/ready bit",
+        ],
+    }
+
+
+def decode_v2_dram_aw_lrsc(payload, dram_record):
+    """Word 30 = hang_aw_meta (ariane_xilinx.sv:895-905, 909).
+
+    The master_DRAM.aw_* bits are latched wire levels and are meaningful only
+    while master[ariane_soc::DRAM].aw_valid is 1 (word 20..23 record).
+    """
+    w = _bits(payload, 32 * 30, 32)
+    aw_valid = None
+    if isinstance(dram_record, dict) and isinstance(dram_record.get("aw_valid"), bool):
+        aw_valid = dram_record["aw_valid"]
+    meaningful = aw_valid is True
+    return {
+        "word_index": 30,
+        "raw_word": _hex(w, 32),
+        "rtl": ("hang_sample[991:960] = hang_aw_meta "
+                "(ariane_xilinx.sv:895-905, 909)"),
+        "master_dram_aw_valid": aw_valid,
+        "fields": [
+            {"name": "master_DRAM.aw_id", "bits": "[5:0]",
+             "value": _bits(w, 0, 6), "meaningful": meaningful,
+             "source": "ariane_xilinx.sv:896"},
+            {"name": "master_DRAM.aw_len", "bits": "[13:6]",
+             "value": _bits(w, 6, 8), "meaningful": meaningful,
+             "source": "ariane_xilinx.sv:897"},
+            {"name": "master_DRAM.aw_size", "bits": "[16:14]",
+             "value": _bits(w, 14, 3), "meaningful": meaningful,
+             "source": "ariane_xilinx.sv:898"},
+            {"name": "master_DRAM.aw_atop", "bits": "[22:17]",
+             "value": _bits(w, 17, 6), "meaningful": meaningful,
+             "source": "ariane_xilinx.sv:899"},
+            {"name": "master_DRAM.aw_lock", "bits": "[23]",
+             "value": bool(_bits(w, 23, 1)), "meaningful": meaningful,
+             "source": "ariane_xilinx.sv:900"},
+            _enum_entry("lrsc.w_state_q", "[26:24]", _bits(w, 24, 3),
+                        LRSC_W_STATE_NAMES, "axi_riscv_lrsc.sv:177-179"),
+            _enum_entry("lrsc.r_state_q", "[28:27]", _bits(w, 27, 2),
+                        LRSC_R_STATE_NAMES, "axi_riscv_lrsc.sv:174-175"),
+            _flag_entry("int_axi_w_valid", "[29]", _bits(w, 29, 1),
+                        "ariane_xilinx.sv:903"),
+            _flag_entry("int_axi_w_ready", "[30]", _bits(w, 30, 1),
+                        "ariane_xilinx.sv:904"),
+            _flag_entry("int_axi_w_last", "[31]", _bits(w, 31, 1),
+                        "ariane_xilinx.sv:905"),
+        ],
+        "notes": [
+            "the master_DRAM.aw_* fields are meaningful only while "
+            "master[ariane_soc::DRAM].aw_valid is 1 (word 20..23 record); with "
+            "aw_valid == 0 those bits are raw idle-AW-channel wire levels and "
+            "must not be read as a transaction",
+            "int_axi_w_* are the atomic adapter's internal W signals "
+            "(ariane_xilinx.sv:903-905)",
+        ],
+    }
+
+
+def decode_v2_lrsc_saved_w_addr(payload):
+    """Word 31 = HANG_LRSC.w_addr_q[31:0] (ariane_xilinx.sv:910)."""
+    w = _bits(payload, 32 * 31, 32)
+    return {
+        "word_index": 31,
+        "raw_word": _hex(w, 32),
+        "lrsc_w_addr_q_low32": _hex(w, 32),
+        "rtl": ("hang_sample[1023:992] = `HANG_LRSC.w_addr_q[31:0], i.e. "
+                "i_axi_riscv_atomics.i_atomics.i_lrsc.w_addr_q "
+                "(ariane_xilinx.sv:910)"),
+        "notes": [
+            "w_addr_q is saved LR/SC state: it is latched from slv_aw_addr_i when "
+            "LR/SC accepts a new AW (axi_riscv_lrsc.sv:298-305, registered at "
+            "480-488). It is therefore the last accepted AW address, not "
+            "necessarily the current AW address and not necessarily valid for "
+            "the sampled cycle",
+            "only the low 32 bits are captured",
+        ],
+    }
+
+
+def decode_v2_post_atomics(payload, dram_record):
+    return {
+        "note": ("frame version 2 words 28..31: passive AMO/LRSC and DDR "
+                 "crossbar-mux observables; these are NOT a sixth AXI record"),
+        "amo_state": decode_v2_amo_state(payload),
+        "xbar_w_mux_state": decode_v2_xbar_w_mux(payload),
+        "dram_aw_lrsc_meta": decode_v2_dram_aw_lrsc(payload, dram_record),
+        "lrsc_saved_w_addr": decode_v2_lrsc_saved_w_addr(payload),
+    }
+
+
```

### 2e. Version handling and record selection in `decode_sample`

```diff
@@ -512,28 +683,60 @@
     # Frame semantics.
     if header["magic"] != _hex(MAGIC, 32):
         out["errors"].append("bad magic %s, expected %s"
                              % (header["magic"], _hex(MAGIC, 32)))
-    if header["version"] != VERSION:
-        out["errors"].append("bad version %d, expected %d" % (header["version"], VERSION))
+    version = header["version"]
+    if version not in SUPPORTED_VERSIONS:
+        out["errors"].append(
+            "unsupported header version %d: only versions %s are decoded"
+            % (version, ", ".join(str(v) for v in SUPPORTED_VERSIONS)))
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
-    out["axi_records"] = [
-        decode_axi_record(rec["name"], rec["rtl"], rec["citation"],
-                          rec["word_base"], payload_int)
-        for rec in AXI_RECORDS
-    ]
+    out["axi_layout_version"] = version
+    if version == 1:
+        out["axi_records"] = [
+            decode_axi_record(rec["name"], rec["rtl"], rec["citation"],
+                              rec["word_base"], payload_int)
+            for rec in AXI_RECORDS_V1
+        ]
+        out["axi_records_note"] = (
+            "frame version 1: six 4-word AXI records at words 8, 12, 16, 20, 24 "
+            "and 28 (the last two are master_DMA_CFG and master_IOMMU_CFG)")
+        out["post_atomics"] = None
+    elif version == 2:
+        out["axi_records"] = [
+            decode_axi_record(rec["name"], rec["rtl"], rec["citation"],
+                              rec["word_base"], payload_int)
+            for rec in AXI_RECORDS_V2
+        ]
+        out["axi_records_note"] = (
+            "frame version 2: five 4-word AXI records at words 8, 12, 16, 20 and "
+            "24 (the last is 'dram', the post-atomic-adapter / pre-clock-converter "
+            "SoC-clock AXI); words 28..31 are NOT a sixth AXI record, they carry "
+            "the AMO/LRSC and DDR-crossbar-mux state decoded under 'post_atomics'")
+        out["post_atomics"] = decode_v2_post_atomics(
+            payload_int,
+            next((r for r in out["axi_records"] if r["name"] == "master_DRAM"),
+                 None))
+    else:
+        out["axi_records"] = []
+        out["axi_records_note"] = (
+            "frame version %d is not decoded; no AXI layout is assumed" % version)
+        out["post_atomics"] = None
     out["blocked_flags"] = collect_blocked_flags(out["axi_records"])
```

### 2f. Surface reader metadata `expected_version`

```diff
@@ -715,7 +918,8 @@
     result["input_metadata"] = {
         key: doc.get(key) for key in
         ("host", "port", "idcode_only", "advertised_bytes", "max_bits",
-         "actual_period_ns", "idcode", "idcode_masked_match")
+         "actual_period_ns", "idcode", "idcode_masked_match",
+         "expected_version")
     }
```

Everything else is unchanged: raw-frame parse, header/payload agreement, `payload_hex` check, magic/valid/ready/reqtoggle, IDCODE low‑28 re‑derivation, `idcode_masked_match is True`, no-samples handling, cross-sample modulo deltas, exit status.

---

## 3. New file `tests/test_hang_snapshot_v2.py`

```python
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
```

---

## 4. Assumptions / unresolved

- The synthetic v2 payload words 0..23 are placeholders; the test asserts only words 24..31 plus the record count/name set, so it does not depend on the real peripheral/counter values.
- `_vector_entry` uses `_hex(value, width)` with `width == 4` only (the 4‑bit mux vectors); no nibble‑misaligned `_hex` is used for the 6‑/3‑bit fields.
- The decoder’s new version check turns an unknown version into an `errors` entry, which makes `trusted == False` and the exit status non‑zero. No AXI layout is assumed for unknown versions (`axi_records == []`).
- I assumed the “current AW meta meaningful” gate is the **pre‑atomic** `master_DRAM` record’s `aw_valid` (words 20..23), not the post‑atomic `dram` record; that matches `ariane_xilinx.sv:896-900` which samples `master[ariane_soc::DRAM]`.
- Requirement "reader `read_snapshot` param" is implemented on `request_and_read_sample`; if the intended symbol is different, only that one signature call site needs renaming.
- Cross‑module refactors, RTL authority confirmation and on‑hardware v2 capture remain Codex’s job.

## 5. Acceptance commands (to be run by Codex; I did not run them)

```bash
# 1) syntax check
python3 -m py_compile scripts/read_hang_snapshot.py \
                     scripts/decode_hang_snapshot.py \
                     tests/test_hang_snapshot_v2.py

# 2) offline tests (real v1 fixture + synthetic v2, no net/board/FPGA)
python3 -m unittest discover -s tests -p 'test_hang_snapshot_v2.py' -v
# or: python3 tests/test_hang_snapshot_v2.py

# 3) v1 regression must still exit 0 with six AXI records
python3 scripts/decode_hang_snapshot.py \
    out/composition-hangdiag-20261005/n4096-hang-snapshot.json > /tmp/decoded_v1.json
echo "exit=$?"     # expect 0
python3 - <<'PY'
import json; d=json.load(open('/tmp/decoded_v1.json'))
print([len(s['axi_records']) for s in d['samples']], d['all_samples_trusted'])
PY

# 4) reader option surface
python3 scripts/read_hang_snapshot.py --help | grep -A1 -- '--expected-version'

# 5) (hardware, later, not part of this bounded change)
# python3 scripts/read_hang_snapshot.py --expected-version 2 --output out/v2.json
# python3 scripts/decode_hang_snapshot.py out/v2.json > /tmp/decoded_v2.json
```