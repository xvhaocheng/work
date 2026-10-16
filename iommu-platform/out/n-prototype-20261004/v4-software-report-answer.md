Below are bounded proposals only. I could not run or edit anything; no test result is claimed.

## 1) `bench/idma_nonblock_read.c` — optional `--diagnostics`

```diff
diff --git a/bench/idma_nonblock_read.c b/bench/idma_nonblock_read.c
--- a/bench/idma_nonblock_read.c
+++ b/bench/idma_nonblock_read.c
@@ -30,6 +30,9 @@
 #define NB_WORDS     16
 #define NB_MAGIC     0x49444D414E424C01ULL
 #define NB_VERSION   1ULL
+#define DIAG_MAGIC   0x49444D414E444701ULL
+#define DIAG_VERSION 1ULL
+#define DIAG_FLAGS_FROZEN 4ULL
 
 /*
  * Register-block index base of the non-block window bank.
@@ -38,6 +41,7 @@
  * Task specification: index = 512 + 0xD00 + i  (== 0xF00 + i).
  */
 #define NB_INDEX_BASE (512ULL + 0xD00ULL)
+#define DIAG_INDEX_BASE (NB_INDEX_BASE + 16ULL)
 
 #if defined(__riscv)
 #define NB_BARRIER() __asm__ __volatile__("fence iorw, iorw" ::: "memory")
@@ -63,7 +67,9 @@
     return val;
 }
 
-static void emit_json(const uint64_t w[NB_WORDS])
+static void emit_json(const uint64_t w[NB_WORDS], int diagnostics,
+                      const uint64_t d[NB_WORDS], uint64_t core_fast_sum)
 {
     unsigned i;
 
@@ -79,10 +85,18 @@
     printf("],\"words\":[");
     for (i = 0; i < NB_WORDS; i++)
         printf("%s%" PRIu64, i == 0 ? "" : ",", w[i]);
-    printf("]}\n");
+    printf("]");
+    if (diagnostics) {
+        printf(",\"diagnostic\":{\"words\":[");
+        for (i = 0; i < NB_WORDS; i++)
+            printf("%s%" PRIu64, i == 0 ? "" : ",", d[i]);
+        printf("],\"core_fast_sum\":%" PRIu64 "}", core_fast_sum);
+    }
+    printf("}\n");
 }
 
-int main(void)
+static void usage(const char *prog)
+{
+    fprintf(stderr, "usage: %s [--diagnostics]\n", prog);
+}
+
+int main(int argc, char **argv)
 {
     uint64_t words[NB_WORDS];
+    uint64_t diag[NB_WORDS];
     uint64_t orig_index, status, flags, sum, tmp;
+    uint64_t core_fast_sum = 0, core_seq_before, core_seq_after;
+    uint64_t core_flags_before, core_flags_after;
     void *map;
-    int fd, rc = 0, ovf;
-    unsigned i;
+    int fd, rc = 0, ovf, diagnostics = 0;
+    unsigned i, argi;
+
+    for (argi = 1; argi < (unsigned)argc; argi++) {
+        if (strcmp(argv[argi], "--diagnostics") == 0) {
+            diagnostics = 1;
+        } else {
+            usage(argv[0]);
+            return 2;
+        }
+    }
+    memset(diag, 0, sizeof(diag));
 
     fd = open("/dev/mem", O_RDWR | O_SYNC);
     if (fd < 0) {
@@ -166,8 +180,76 @@
         rc = 4;
     }
 
+    if (rc == 0 && diagnostics) {
+        core_seq_before = words[4];
+        core_flags_before = words[3];
+        for (i = 0; i < NB_WORDS; i++) {
+            nb_write(REG_INDEX, DIAG_INDEX_BASE + (uint64_t)i);
+            diag[i] = nb_read(REG_DATA);
+        }
+        nb_write(REG_INDEX, NB_INDEX_BASE + 4ULL);
+        core_seq_after = nb_read(REG_DATA);
+        nb_write(REG_INDEX, NB_INDEX_BASE + 3ULL);
+        core_flags_after = nb_read(REG_DATA);
+
+        if (diag[0] != DIAG_MAGIC) {
+            fprintf(stderr, "bad diagnostic magic: 0x%016" PRIx64 "\n",
+                    diag[0]);
+            rc = 4;
+        }
+        if (diag[1] != DIAG_VERSION) {
+            fprintf(stderr, "bad diagnostic version: %" PRIu64 "\n", diag[1]);
+            rc = 4;
+        }
+        if (diag[2] != words[2]) {
+            fprintf(stderr,
+                    "diagnostic mode %" PRIu64 " != core mode %" PRIu64 "\n",
+                    diag[2], words[2]);
+            rc = 4;
+        }
+        if (diag[3] != DIAG_FLAGS_FROZEN) {
+            fprintf(stderr, "bad diagnostic flags: 0x%016" PRIx64 "\n",
+                    diag[3]);
+            rc = 4;
+        }
+        if (diag[4] != words[4]) {
+            fprintf(stderr,
+                    "diagnostic seq %" PRIu64 " != core seq %" PRIu64 "\n",
+                    diag[4], words[4]);
+            rc = 4;
+        }
+        if (diag[5] != words[5]) {
+            fprintf(stderr,
+                    "diagnostic cycles %" PRIu64 " != core cycles %" PRIu64
+                    "\n", diag[5], words[5]);
+            rc = 4;
+        }
+        if (core_seq_after != core_seq_before) {
+            fprintf(stderr,
+                    "core seq changed during diagnostic read: %" PRIu64
+                    " -> %" PRIu64 "\n", core_seq_before, core_seq_after);
+            rc = 4;
+        }
+        if (core_flags_after != core_flags_before) {
+            fprintf(stderr,
+                    "core flags changed during diagnostic read: 0x%016" PRIx64
+                    " -> 0x%016" PRIx64 "\n",
+                    core_flags_before, core_flags_after);
+            rc = 4;
+        }
+
+        core_fast_sum = 0;
+        ovf = 0;
+        for (i = 6; i <= 10; i++) {
+            if (__builtin_add_overflow(core_fast_sum, words[i], &tmp)) {
+                ovf = 1;
+                break;
+            }
+            core_fast_sum = tmp;
+        }
+        if (ovf) {
+            fprintf(stderr, "diagnostic core fast sum overflows 64 bits\n");
+            rc = 4;
+        } else if (core_fast_sum != diag[15]) {
+            fprintf(stderr,
+                    "diagnostic fast_accepts %" PRIu64
+                    " != sum(core fast 6..10) %" PRIu64 "\n",
+                    diag[15], core_fast_sum);
+            rc = 4;
+        }
+    }
+
     if (rc == 0)
-        emit_json(words);
+        emit_json(words, diagnostics, diag, core_fast_sum);
 
 out_restore:
     nb_write(REG_INDEX, orig_index);
```

## 2) `scripts/mac_nonblock_pilot.py` — optional `--diagnostics`

```diff
diff --git a/scripts/mac_nonblock_pilot.py b/scripts/mac_nonblock_pilot.py
--- a/scripts/mac_nonblock_pilot.py
+++ b/scripts/mac_nonblock_pilot.py
@@ -129,6 +129,15 @@
 READER_MODE_FOR_LABEL = {'banked-serial': 1, 'banked-hit': 2}
 READER_FAST_WORDS = (6, 7, 8, 9, 10)   # Vread, Vwrite, Aread, Awrite, Other
 READER_NORMAL_WORDS = (11, 12, 13)     # V, A, Other
+
+# Optional second 16-word read-only diagnostic bank (index 16..31).  It is
+# read only when the helper is invoked with --diagnostics; the default 16-word
+# ABI above is unchanged.
+READER_DIAG_WORDS = 16
+READER_DIAG_MAGIC = 0x49444D414E444701
+READER_DIAG_VERSION = 1
+READER_DIAG_FLAGS_FROZEN = 4
+READER_DIAG_FAST_ACCEPT_WORD = 15
 UPLOAD_CHUNK = 1024
 # Raw-UART heredoc upload tuning.  At 115200 8N1 one 1024-byte base64 line
 # costs about 89 ms of wire time, so each payload line is followed by a short
@@ -499,6 +508,54 @@
         problems.append('helper window[4] sequence ' + repr(window[4])
                         + ' did not increase past ' + repr(previous_seq))
     return problems
 
 
+def validate_helper_diagnostic(window: Any, diagnostic: Any,
+                               label: str) -> List[str]:
+    '''Validate the optional second 16-word diagnostic bank.
+
+    The helper validates this bank before printing it; this runner repeats the
+    structural checks so a malformed helper.json can never be recorded as a
+    completed case.  The diagnostic bank is passive: its words are not HPM
+    counters and never replace the frozen 16-word core ABI.
+    '''
+    problems: List[str] = []
+    if not isinstance(window, list) or len(window) != READER_WINDOW_WORDS:
+        return ['diagnostic check needs the core 16-word window']
+    if not isinstance(diagnostic, dict):
+        return ['helper diagnostic is not an object']
+    words = diagnostic.get('words')
+    if not isinstance(words, list) or len(words) != READER_DIAG_WORDS:
+        return ['helper diagnostic.words is not '
+                + str(READER_DIAG_WORDS) + ' words']
+    if any(isinstance(value, bool) or not isinstance(value, int)
+           for value in words):
+        return ['helper diagnostic.words contains non-integer words']
+    if words[0] != READER_DIAG_MAGIC:
+        problems.append('helper diagnostic[0] magic ' + repr(words[0]))
+    if words[1] != READER_DIAG_VERSION:
+        problems.append('helper diagnostic[1] version ' + repr(words[1]))
+    expected_mode = READER_MODE_FOR_LABEL.get(label)
+    if words[2] != expected_mode:
+        problems.append('helper diagnostic[2] mode ' + repr(words[2])
+                        + ' != ' + repr(expected_mode) + ' for ' + label)
+    if words[3] != READER_DIAG_FLAGS_FROZEN:
+        problems.append('helper diagnostic[3] flags ' + repr(words[3])
+                        + ' != ' + str(READER_DIAG_FLAGS_FROZEN))
+    if words[4] != window[4]:
+        problems.append('helper diagnostic[4] seq ' + repr(words[4])
+                        + ' != core window[4] ' + repr(window[4]))
+    if words[5] != window[5]:
+        problems.append('helper diagnostic[5] cycles ' + repr(words[5])
+                        + ' != core window[5] ' + repr(window[5]))
+    total = 0
+    for index in READER_FAST_WORDS:
+        total += window[index]
+    if total > 0xFFFFFFFFFFFFFFFF:
+        problems.append('helper diagnostic core fast sum overflows 64 bits')
+    elif words[READER_DIAG_FAST_ACCEPT_WORD] != total:
+        problems.append('helper diagnostic[15] fast_accepts '
+                        + repr(words[READER_DIAG_FAST_ACCEPT_WORD])
+                        + ' != sum(core fast 6..10) ' + repr(total))
+    return problems
+
+
 # --------------------------------------------------------------------------
 # capability gate (--caps)
 # --------------------------------------------------------------------------
@@ -1089,7 +1147,10 @@
     def _reader_command(self) -> str:
         # Assumed helper invocation; align with the ELF once it is compiled.
-        mode = READER_MODE_FOR_LABEL.get(self.args.label)
-        return shlex.quote(self._reader_target())
+        command = shlex.quote(self._reader_target())
+        if self.args.diagnostics:
+            command += ' --diagnostics'
+        return command
 
     def _upload_reader(self) -> Dict[str, Any]:
         '''Raw UART upload of the static helper into /tmp.
@@ -1344,6 +1405,25 @@
             self._write_json('cases/' + cid + '/helper-window.json', {
                 'schema': 'nonblock-pilot-helper-window-v1',
                 'id': cid,
                 'label': self.args.label,
                 'abi': READER_ABI,
                 'mode': READER_MODE_FOR_LABEL.get(self.args.label),
                 'window': window,
                 'note': 'full frozen 16-word window saved verbatim; not HPM',
             })
+            if self.args.diagnostics:
+                diagnostic = (helper_doc.get('diagnostic')
+                              if isinstance(helper_doc, dict) else None)
+                problems.extend(validate_helper_diagnostic(
+                    window, diagnostic, self.args.label))
+                if isinstance(diagnostic, dict):
+                    self._write_json('cases/' + cid + '/diag-window.json', {
+                        'schema': 'nonblock-pilot-diag-window-v1',
+                        'id': cid,
+                        'label': self.args.label,
+                        'abi': READER_ABI,
+                        'mode': READER_MODE_FOR_LABEL.get(self.args.label),
+                        'diagnostic': diagnostic,
+                        'note': ('full second 16-word diagnostic bank saved '
+                                 'verbatim; passive only, not HPM'),
+                    })
         return problems, window
@@ -1664,6 +1744,11 @@
     parser.add_argument('--reader', default=None,
                         help='static RISC-V ELF helper; required for '
                              'banked-serial/banked-hit, forbidden for original-serial')
+    parser.add_argument('--diagnostics', action='store_true',
+                        help='read the optional second 16-word diagnostic bank '
+                             'through the uploaded reader; requires a banked '
+                             'label and --reader')
     parser.add_argument('--upload-timeout', type=float, default=DEFAULT_UPLOAD_TIMEOUT)
     parser.add_argument('--pre-flash-boot-id', action='store_true',
                         help='best effort: read the running boot_id over the UART '
@@ -1699,6 +1784,12 @@
         if not pathlib.Path(args.reader).is_file():
             raise SystemExit('--reader not found: ' + args.reader)
+    if args.diagnostics:
+        if args.label not in READER_REQUIRED_LABELS:
+            raise SystemExit('--diagnostics requires a banked label with RO '
+                             'hardware: ' + ', '.join(READER_REQUIRED_LABELS))
+        if not args.reader:
+            raise SystemExit('--diagnostics requires --reader')
     if args.recover_startup_from:
         if args.interrupt_stalled_seed:
             raise SystemExit('--interrupt-stalled-seed does not apply with '
```

## 3) `tests/nonblock_observer_top.sv` — connect forthcoming `diag_events`

```diff
diff --git a/tests/nonblock_observer_top.sv b/tests/nonblock_observer_top.sv
--- a/tests/nonblock_observer_top.sv
+++ b/tests/nonblock_observer_top.sv
@@ -20,7 +20,8 @@
     input  logic        accept,
     input  logic        fast,
     input  logic [23:0] did,
-    input  logic [5:0]  ttype
+    input  logic [5:0]  ttype,
+    input  logic [15:0] diag_events
 );
     dma_nonblock_observer #(.Mode(`NB_MODE_VAL)) dut (
@@ -33,6 +34,7 @@
         .fast   (fast),
         .did    (did),
-        .ttype  (ttype)
+        .ttype  (ttype),
+        .diag_events(diag_events)
     );
 endmodule
```

## 4) `tests/nonblock_observer_test.cpp` — bounded diag exercise, legacy 16-word tests preserved

```diff
diff --git a/tests/nonblock_observer_test.cpp b/tests/nonblock_observer_test.cpp
--- a/tests/nonblock_observer_test.cpp
+++ b/tests/nonblock_observer_test.cpp
@@ -20,7 +20,7 @@
 static void idle() {
     dut->arm = 0; dut->open = 0; dut->close = 0;
     dut->accept = 0; dut->fast = 0;
-    dut->did = 0; dut->ttype = 0;
+    dut->did = 0; dut->ttype = 0; dut->diag_events = 0;
 }
 
 static uint64_t rd(int idx) {
@@ -182,6 +182,100 @@
     idle(); dut->close = 1; tick();
     idle(); dut->eval();
     check("close-edge frozen", (rd(3) >> 2) & 1, 1);
     check("close-edge cycles", rd(5), 1);
 
+    // ---- optional second diagnostic bank (index 16..31) --------------------
+    // Fresh reset/ARM/open/close for a bounded diagnostic window.  The core
+    // 16-word checks above stay untouched; this section only reads the
+    // separate passive bank.  No ordering relation between the probe counters
+    // is asserted: a probe can legitimately cross the window boundary.
+    dut->rst = 1; idle(); tick();
+    dut->rst = 0; idle(); dut->eval();
+
+    idle(); dut->arm = 1; tick();
+    idle(); dut->eval();
+    check("diag magic",     rd(16), 0x49444D414E444701ULL);
+    check("diag version",   rd(17), 1);
+    check("diag mode",      rd(18), mode);
+    check("diag arm flags", rd(19), 0x1);
+    check("diag arm seq",   rd(20), 1);
+    check("diag arm cycles", rd(21), 0);
+    check("diag arm seen",  rd(22), 0);
+    for (int i = 23; i <= 31; i++)
+        check("diag arm counter", rd(i), 0);
+
+    idle(); dut->open = 1; tick();
+    idle(); dut->eval();
+    check("diag open flags", rd(19), 0x2);
+    check("diag open seen",  rd(22), 0);
+    for (int i = 23; i <= 31; i++)
+        check("diag open counter", rd(i), 0);
+
+    // Probe-stage seen bits and their counters.  These are independent
+    // observations, not a lifecycle ordering assertion.
+    idle(); dut->diag_events = (1u << 3) | (1u << 4) | (1u << 5); tick();
+    idle(); dut->diag_events = (1u << 6) | (1u << 7); tick();
+    idle(); dut->diag_events = (1u << 8) | (1u << 9) | (1u << 10); tick();
+
+    // One real fast accept with its fast-accept diagnostic event.
+    idle();
+    dut->accept = 1; dut->fast = 1; dut->did = 10; dut->ttype = 0x2;
+    dut->diag_events = (1u << 11);
+    tick();
+
+    // Remaining seen-only bits: no core fast count and no probe counter.
+    idle();
+    dut->diag_events = (1u << 0) | (1u << 1) | (1u << 2) |
+                       (1u << 12) | (1u << 13) | (1u << 14) | (1u << 15);
+    tick();
+
+    // Closing edge: still ACTIVE at the edge, so both the core fast event
+    // and its diagnostic event are counted.
+    idle();
+    dut->accept = 1; dut->fast = 1; dut->did = 11; dut->ttype = 0x2;
+    dut->diag_events = (1u << 11); dut->close = 1;
+    tick();
+    idle(); dut->eval();
+
+    check("diag frozen flags", rd(19), 0x4);
+    check("diag seen all",     rd(22), 0xFFFF);
+    check("diag start",        rd(23), 1);
+    check("diag dc eval",      rd(24), 1);
+    check("diag dc good",      rd(25), 1);
+    check("diag leaf eval",    rd(26), 1);
+    check("diag leaf good",    rd(27), 1);
+    check("diag dc msi rej",   rd(28), 1);
+    check("diag leaf msi rej", rd(29), 1);
+    check("diag leaf miss",    rd(30), 1);
+    check("diag fast accept",  rd(31), 2);
+    check("diag cycles",       rd(21), 6);
+    check("diag seq same",     rd(20), rd(4));
+    check("diag cycles same",  rd(21), rd(5));
+
+    uint64_t core_fast_sum = 0, core_fast_tmp = 0;
+    int core_fast_ovf = 0;
+    for (int i = 6; i <= 10; i++) {
+        if (__builtin_add_overflow(core_fast_sum, rd(i), &core_fast_tmp)) {
+            core_fast_ovf = 1;
+            break;
+        }
+        core_fast_sum = core_fast_tmp;
+    }
+    check("diag core fast sum overflow", core_fast_ovf, 0);
+    check("diag fast accept == core fast sum", rd(31), core_fast_sum);
+    check("diag overflow bit", (rd(19) >> 63) & 1, 0);
+
     delete dut;
     if (g_fail) {
         std::fprintf(stderr, "%d check(s) failed\n", g_fail);
```

## 5) Complete Chinese `RESULTS_REVIEW.md` fence

```markdown
# RESULTS_REVIEW.md（中文 · 完整修订版 · 仅基于已提供原始证据）

> 本文替换此前约 3341 字符的 fenced 草稿。替换动作：
> - “布局 Δ” → “S/原版变化（结构+缓冲）”
> - “绕过 Δ” → “N/S变化（本轮未触发绕过）”
> - 不再把 N/S 数值差写成实际绕过效果：本轮 N 的全部 fast 计数为 0，不能据此声称绕过生效或未生效。
>
> 口径：离线只读汇总，不重跑、不改原始文件、不实现硬件。单位沿用 50 MHz 假设，1 cycle = 20 ns（推导量）。A 速度只用同一窗口 A 完成字节速率，不是总线吞吐。

## 0. 数据状态与边界

- original / banked-serial / banked-hit 三份 pilot 均为 15/15 complete、0 failed、taint 0，数据有效。
- 现有 raw pilots 合计 45 例全部 complete；N 侧 `fastcounts=[0,0,0,0,0]`。
- 本轮不因 fast=0 直接归因，不宣称 novelty，不宣称已有 miss baseline 下的 prior-work hit。
- 旧 raw HPM / miss / PTW 字段在 nonblock 结构下不作为机制证据。
- 不做显著性、不剔除合法样本、不重跑、不重放旧数据。

## 1. 运行身份与完整度

| label | 目录 | BIT | run_id | 15/15 | taint | helper |
|---|---|---|---|---|---|---|
| original-serial | `nonblock-pilot-20261004-original-03` | `original-serial.bit` | `a72442e2e112f401` | 是 | 0 | 无 |
| banked-serial | `nonblock-pilot-20261004-serial-v3-01` | `banked-serial-v3.bit` | `3ae491ffaa3df550` | 是 | 0 | mode 1 |
| banked-hit | `nonblock-pilot-20261004-hit-v3-01` | `banked-hit-v3.bit` | `cc371e91173ce50a` | 是 | 0 | mode 2 |

来源：三份 `summary.json` 的 `status:"complete"`、`completed:15/failed:0`；`analysis-v3-pilot/nonblock-pilot-analysis.json` 的 `completeness.all_cases_complete:true`。

## 2. 长组结果（每条件三轮；median[min,max]）

### Solo（A jobs 恒 0）

| label | cycles median[min,max] | V MiB/s |
|---|---|---|
| original | 2828763 [2828343, 2832793] | 282.809 [282.407, 282.851] |
| serial | 2825110 [2820368, 2825919] | 283.175 [283.094, 283.651] |
| hit | 2825984 [2825890, 2831598] | 283.087 [282.526, 283.097] |

### A1

| label | cycles median[min,max] | V = A MiB/s | A jobs |
|---|---|---|---|
| original | 5359179 [5358650, 5360287] | 149.277 [149.246, 149.291] | 4096×3 |
| serial | 5584177 [5581750, 5585686] | 143.262 [143.223, 143.324] | 4096×3 |
| hit | 5584794 [5579282, 5586215] | 143.246 [143.210, 143.388] | 4096×3 |

### A8

| label | cycles median[min,max] | V = A MiB/s | A jobs |
|---|---|---|---|
| original | 7349054 [7347789, 7353256] | 108.858 [108.795, 108.876] | 4096×3 |
| serial | 7667658 [7666937, 7669765] | 104.334 [104.306, 104.344] | 4096×3 |
| hit | 7667630 [7667627, 7669858] | 104.335 [104.304, 104.335] | 4096×3 |

### A16

| label | cycles median[min,max] | V = A MiB/s | A jobs |
|---|---|---|---|
| original | 7945520 [7942861, 7946297] | 100.686 [100.676, 100.719] | 4096×3 |
| serial | 8274495 [8272332, 8275137] | 96.683 [96.675, 96.708] | 4096×3 |
| hit | 8275122 [8272399, 8277440] | 96.675 [96.648, 96.707] | 4096×3 |

说明：A 条件里同窗口 A 完成为 4096，字节 = 4096×4096 = 16777216 B，与 V 固定工作量相同，故 A MiB/s 数值上等于 V MiB/s。这只是“同窗口完成量相同”，不是命中率，也不是总线吞吐。

## 3. 两段差值（仅描述性，禁止越界解释）

| 条件 | S/原版变化（结构+缓冲） = serial − original | N/S变化（本轮未触发绕过） = hit − serial |
|---|---|---|
| Solo | +0.366 MiB/s（+0.13%） | −0.088（−0.03%） |
| A1 | −6.015（−4.03%） | −0.016（−0.01%） |
| A8 | −4.523（−4.16%） | +0.0004（+0.0004%） |
| A16 | −4.003（−3.98%） | −0.007（−0.008%） |

对应周期数差：A 条件 S/原版方向约 +4.1%〜+4.3%；N/S 方向 |Δ| ≤ 0.02%。

**强制限制（必须随数字一起写）**

- original→S 同时包含银行化 + 缓冲存储/容量/布局；两次各自新鲜启动、物理分配不同、固定顺序、n=3 ⇒ 不得声称显著性、机制因果或性能收益。
- N/S 差值在本轮 **不构成绕过效果证据**：N 的全部 fast 计数为 0，本轮未触发可观测绕过；该差值只能作为描述性窗口周期差保留。
- 只有 S 与 N 同为 512 缓冲时，S→N 的差才可能分离绕过增量；即便如此，本轮 S→N 差异非常接近 0，且 fast 计数全 0。
- A 速度只用同一窗口 A 吞吐评价，不用跨窗口 A 完成数比较。
- 不宣称 novelty；在已知 miss baseline 下不宣称 prior-work hit。

## 4. Live read-only DC/MMIO 事实（只读快照）

已提供的只读 live DC/MMIO 证据：

- `out/n-prototype-20261004/nonblock-live-dc-20261004/mmio.json`：
  - `0x50010000` caps = `0x000001F8E6420210` → 与 CAPMSIFLAT=1 相符。
  - `0x50010010` ddtp = `0x0000000020AC4002`。
- `out/n-prototype-20261004/nonblock-live-dc-20261004/dc.json`：
  - `ddtp` = 548159490 = `0x20AC4002`；mode = 2；root = `0x82b10000`；stride = 64 B。
  - DID10：`tc=0x1`、`iohgatp=0x0`、`ta=0x1000`、`fsc=0x8000000000082B89`、`msiptp=0x1000000000082B88`、`msi_addr_mask=0xFF`、`msi_addr_pattern=0x28000`。
  - DID11：`tc=0x1`、`iohgatp=0x0`、`ta=0x2000`、`fsc=0x8000000000082B8D`、`msiptp=0x1000000000082B8B`、`msi_addr_mask=0xFF`、`msi_addr_pattern=0x28000`。

解读边界：

- 上述快照确认 CAPMSIFLAT=1、DDTP mode 2、root `0x82b10000`、extended 64 B stride、DID10/11 的 `msiptp.mode=1`。
- 与旧 fast 守卫所要求的 DDTP mode 0 静态条件相比，当前 live DC 的 DDTP mode 2 构成静态配置不相容。这是静态配置事实，不等于运行时缓存 DC 一定精确等于 RAM，也不等于已证明所有候选机会，更不等于唯一原因。
- DID10/11 的 `iohgatp=0`（Bare）、`tc=1`（V）与 flat MSI 快路径的若干静态前提相容，但不能据此宣称快路径必然命中。

## 5. Linux 页表 U 位修正：移除此前不支持的候选

- `linux/arch/riscv/include/asm/pgtable.h:157`：
  ```c
  #define _PAGE_BASE (_PAGE_PRESENT | _PAGE_ACCESSED | _PAGE_USER)
  ```
- `linux/arch/riscv/include/asm/pgtable-bits.h:15`：
  ```c
  #define _PAGE_USER (1 << 4)
  ```
- 因此源码默认即置 U 位；此前“IOMMU 页表可能未置 U 位”的候选缺乏已提供源码支持，应从不确定性中移除。
- 这不代表所有门都成立；只说明 U 位这一条不再作为未决阻塞候选。

## 6. 主 FPGA 实际配置事实

- `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1322-1327`：
  - 非 block 模式下 `nonblock_cfg()` 将 `cfg.MSITrans` 覆盖为 `rv_iommu_cfg::MSI_BT_ONLY`，保留 flat MSI。
- 同文件 `:1332-1334`：
  - 若 `IOMMUNonblockMode > 2` 或 nonblock 与 legacy E3/COST observers 并用，会 `$fatal`。
- 同文件 `:1204`：
  - nonblock 模式下 iDMA 0/1 的 `BUFFER_DEPTH` 为 512；S/N 使用相同 512 缓冲容量。
- 同文件 `:991-1002`：
  - `dma_nonblock_observer` 已连接到 `nonblock_data`，并接收 `nonblock_diag` 作为 `diag_events`。
- 同文件 `:1390-1392`：
  - `rv_iommu_top` 输出 `nonblock_fire_o`、`trans_accept_o`、`nonblock_diag_o`、`trans_did_o`、`trans_type_o`。

## 7. 诊断 ABI 与旧 16 字 ABI 的关系

旧 16 字 ABI 保持不变；旧 word15 仍是 sticky error，不得挪用。

此前“把 sticky flags 放进 err15 未用高位”的提案无效：旧 helper 对 `words[15] != 0` 一律拒绝，因此不能用 err15 承载诊断位。

选定的诊断 ABI 为独立第二 bank，index 16..31，寄存器索引：

```text
INDIRECT raw NB_INDEX_BASE + 16
NB_INDEX_BASE = 512 + 0xD00 = 0xF00
extra bank base = 0xF10
```

第二 bank 精确 16 字：

| diag word | index | 含义 |
|---|---|---|
| 0 | 16 | magic `0x49444D414E444701` |
| 1 | 17 | version 1 |
| 2 | 18 | mode（与 core 相同） |
| 3 | 19 | flags：core status low3 + bit63 diag counter overflow error（必须 0；运行时 exact 4） |
| 4 | 20 | seq（与 core 相同） |
| 5 | 21 | cycles（与 core 相同） |
| 6 | 22 | seen 16-bit mask |
| 7 | 23 | probe_starts |
| 8 | 24 | dc_evaluations |
| 9 | 25 | dc_good |
| 10 | 26 | leaf_evaluations |
| 11 | 27 | leaf_good |
| 12 | 28 | dc_msi_rejects |
| 13 | 29 | leaf_msi_rejects |
| 14 | 30 | leaf_lookup_misses |
| 15 | 31 | fast_accepts（必须等于 core words 6..10 之和；同一窗口精确相等） |

计数采样规则：

- 采样窗口同为 `(open, close]`。
- reset / ARM clear；close freeze。
- 跨窗口的部分 probe 合法：start/eval/leaf 计数之间不强制完整生命周期关系。
- 全部诊断为 passive，不反馈 DMA/AXI。

`seen` 位映射：

| bit | 含义 |
|---|---|
| 0 | normal base PTW active（S1 only） |
| 1 | existing full fast window predicate |
| 2 | candidate valid in that window |
| 3 | start |
| 4 | DC eval |
| 5 | DC passes stable stage |
| 6 | LEAF eval |
| 7 | leaf eligibility |
| 8 | DC MSI reject（if DDTC hit） |
| 9 | leaf MSI reject（if IOTLB hit） |
| 10 | leaf lookup miss |
| 11 | fast accept |
| 12 | DC lookup miss |
| 13 | DC other reject |
| 14 | leaf other reject |
| 15 | runtime enable |

## 8. 限制与不可主张项

1. original→S 被混淆：同时含银行化 + 缓冲存储/容量/布局；两次各自新鲜启动、物理分配不同、固定顺序、n=3 ⇒ 不得声称显著性、机制因果、性能收益。
2. N fast 未被本次仪表观测到：15/15 `fastcounts=[0,0,0,0,0]`，`normalcounts` 为正，word15 错误位 0。该事实不等于路径损坏、不等于零命中率、不等于零 legacy 资格；快速路径事件数不是命中率，也不是全部 miss 数。
3. 不使用旧 raw HPM/miss/PTW：`record.json` 中 `victim/aggressor.iotlb_miss/s1_ptw/s2_ptw` 属旧字段，在 nonblock 结构下非有效 metric。
4. 启动数据 priming 限制：original-03 使用恢复启动，未重新烧写 BIT，复用 original-02 的 flash 记录；Buildroot 在 “Saving random seed” 处由单次人工 CTRL-C 解除阻塞；内核与 bit 哈希未变，但精确停滞子步骤未被断言。
5. 无 SD 更新、Linux 00093 未改：本次不需要重编 Linux；后续若不改 ABI/接口，不应提议重编。
6. 每 label 一次运行一次启动，跨 label 差异含运行间波动；不做方差分析、不剔除合法样本、不重跑。
7. 只读 live DC 是单点快照；不得声称运行时 cache DC 精确等于 RAM，也不得声称已覆盖全部候选机会或唯一原因。
8. 不宣称 novelty；在已知 miss baseline 下不宣称 prior-work hit。

## 9. 下一步计划（有界，不实现硬件补丁）

- 不因“fast 计数为 0”直接修路径；先取证。
- 不提议 Linux 重编；维持 Linux 00093、无 SD 更新。
- 采用 v4 BOTH S/N 各 15 例新 pilot，使用新 observer；S/N 同为 512 缓冲；不复用旧数据，不做 short-only。
- 被动 F10 诊断 bank 与旧 16 字 ABI 并存；旧 helper 继续可用。
- 计划修复范围只针对 flat MSI=1 下的普通输入 / cached physical non-MSI 地址；unsupported MSI / reserved mode 回退 normal。
- 候选 overlap / benefit 仍待观察；本轮不预判。
- 一次短序列，遇技术第一错即停，保留全部 raw，不重试、不重跑。

## 10. 验收检查（草案）

- C helper：默认无参数输出与旧 JSON 字段/值完全一致；`--diagnostics` 只额外增加 `diagnostic` 对象；未知参数打印 usage 且非零退出；所有错误路径恢复原 INDEX。
- C helper：`diagnostic.words` 为 16 个 uint64；`words[0]/[1]/[2]/[3]/[4]/[5]` 分别校验 magic/version/mode/flags/seq/cycles；`words[15] == sum(core words 6..10)` 且和检查溢出；不伪造数据。
- Python runner：`--diagnostics` 仅对 banked label + `--reader` 有效；`_reader_command` 只在 flag 为真时追加 `--diagnostics`；默认 15 例计划/超时/stop 行为不变；diagnostic 校验失败使 case fail；`diag-window.json` 只在 diagnostic 存在时保存。
- Observer tests：顶层 `diag_events[15:0]` 已连到 `dma_nonblock_observer.diag_events`；cpp 保留旧 16 字检查；新增 diag bank 检查 ARM/open/close/freeze、seen mask、各计数器、word31 与 core fast 6..10 之和相等（含 closing edge）；不对部分 probe 窗口强加顺序断言。
- 不包含硬件 observer 实现；observer RTL 由 Codex 负责。

## 11. 不确定性 / 未决

- D-1：`nonblock_diag` 在 `rv_iommu_top` 内部的实际事件语义需 Codex 确认；本报告只确认顶层已接到 `dma_nonblock_observer.diag_events`。
- D-2：诊断 bank 的 RTL 实现由 Codex 完成；测试与 runner 假设 index 16..31 与上述 ABI 一致。
- D-3：已移除“U 位未置”候选；但 leaf/dc 其它门是否成立仍需运行时诊断确认。
- D-4：候选 overlap 频率与 benefit 尚未观察；live DC 是单点只读快照。
- D-5：不得声称运行时 cache DC 精确等于 RAM、覆盖全部候选机会或为唯一原因。
- D-6：本 worker 未执行、未编辑、未烧写；以上均为提案与静态证据汇总。
```

## Acceptance checks

- C helper: compile with warnings enabled; no-arg JSON remains exactly the old schema/fields/values; `--diagnostics` adds only `diagnostic`; unknown arg prints usage and exits non-zero; original INDEX restored on all MMIO error paths.
- Python runner: `py_compile`; argparse rejects unknown args; `--diagnostics` rejected for `original-serial`; default plan remains 15 cases; `_reader_command` appends the flag only when true; diagnostic validation failure fails the case; `diag-window.json` is written only when diagnostic object exists.
- Verilator tests: top connects `diag_events`; legacy 16-word tests unchanged; new diag checks pass for both Mode 1/Mode 2 builds; no partial-probe ordering assertions.
- Hardware/RTL: Codex must supply the actual `dma_nonblock_observer` diag bank at index 16..31 and the `diag_events[15:0]` source semantics.

## Uncertainties

- I did not run any tool, compile, lint, or hardware test; none of the above is claimed as executed.
- The observer RTL is intentionally not patched here; the tests assume Codex’s forthcoming `diag_events` and second bank exist with the ABI described.
- Live DC facts are read-only single-snapshot evidence; they do not prove runtime cache equals RAM, cover all candidate opportunities, or establish sole cause.
- The N/S numeric difference remains descriptive only while all N fast counts are zero.