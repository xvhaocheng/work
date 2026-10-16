```diff
--- a/tests/nonblock_actual_dma_test.cpp
+++ b/tests/nonblock_actual_dma_test.cpp
@@ -80,6 +80,8 @@
 static const uint64_t V_SRC_IOVA = 0x10000000ULL;
 static const uint64_t V_DST_IOVA = 0x10001000ULL;
 static const uint64_t V_SRC_PA   = 0x80000000ULL;
 static const uint64_t V_DST_PA   = 0x80001000ULL;
+static const uint64_t V_COLD_DST_IOVA = 0x10002000ULL;
+static const uint64_t V_COLD_DST_PA   = 0x80002000ULL;
 
 static const uint64_t A_IOVA_BASE   = 0x20000000ULL;
 
@@ -188,6 +190,7 @@
     // ---------------- run-time knobs ------------------------------------
     bool     nonblock_enable   = true;
     bool     policy_partition  = false;
+    bool     v_cold_dst        = false;
     uint32_t ptw_latency       = 1;
     uint32_t a_pages           = 8;
     uint32_t v_gap_cycles      = 1000;
@@ -436,7 +439,8 @@
         // V page tables: root 0x20000, L1V 0x40000, L0V 0x50000.
         write64(0x20000, 0x0000000000010001ULL);
         write64(0x40400, 0x0000000000014001ULL);
         write64(0x50000, 0x00000000200000DFULL); // Vsrc -> 0x80000000
         write64(0x50008, 0x00000000200004DFULL); // Vdst -> 0x80001000
+        write64(0x50010, ((V_COLD_DST_PA >> 12) << 10) | 0xDFULL); // Vcold-dst -> 0x80002000
 
         // A page tables: root 0x30000, L1A 0x60000, L0A 0x70000.
         write64(0x30000, 0x0000000000018001ULL);
@@ -452,6 +456,7 @@
         // V source content + unique destination pre-fill.
         for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
             write64(V_SRC_PA + (uint64_t)i * 8, page_word(0, i));
             write64(V_DST_PA + (uint64_t)i * 8, page_old(0, i));
+            write64(V_COLD_DST_PA + (uint64_t)i * 8, page_old(0, i));
         }
         // A source content + unique destination pre-fill.
         for (uint32_t p = 0; p < a_pages; ++p) {
@@ -1103,6 +1108,15 @@
     uint64_t cfg_rd(bool sel, uint64_t addr) {
         return cfg_xact(sel, false, addr, 0, 0x00);
     }
 
+    // Measurement destination address selected by +v-cold-dst.  Warm-up
+    // deliberately keeps using the original V_DST_IOVA/V_DST_PA.
+    uint64_t v_measure_dst_iova() const {
+        return v_cold_dst ? V_COLD_DST_IOVA : V_DST_IOVA;
+    }
+    uint64_t v_measure_dst_pa() const {
+        return v_cold_dst ? V_COLD_DST_PA : V_DST_PA;
+    }
+
     // program ONE batch-off 64-bit copy
     void program_single(bool sel, uint64_t src, uint64_t dst) {
         cfg_wr(sel, REG_CONF, 0x0);
@@ -1464,6 +1478,8 @@
         std::printf(
             "ACTUAL-DMA-WINDOW-JSON {"
             "\"schema\":\"actual-dma-window-v1\","
-            "\"mode\":\"%s\",\"runtime_N\":%d,\"policy_enable\":%d,"
+            "\"mode\":\"%s\",\"v_cold_dst\":%d,"
+            "\"v_dst_iova\":%llu,\"v_dst_pa\":%llu,"
+            "\"runtime_N\":%d,\"policy_enable\":%d,"
             "\"a_pages\":%u,\"v_gap\":%u,\"v_transfers\":%u,\"warmup\":%u,"
             "\"ptw_latency\":%u,\"a_target\":%llu,"
             "\"W_cycles\":%llu,\"win_open_cycle\":%ld,"
@@ -1496,4 +1512,7 @@
-            name, (int)enable, (int)policy_partition,
+            name, (int)v_cold_dst,
+            (unsigned long long)v_measure_dst_iova(),
+            (unsigned long long)v_measure_dst_pa(),
+            (int)enable, (int)policy_partition,
             a_pages, v_gap_cycles, v_transfers, warmup, ptw_latency,
             (unsigned long long)a_target,
             (unsigned long long)w_cycles, win_open_cycle, win_close_cycle,
@@ -1850,5 +1869,5 @@
                         A_IOVA_BASE + (uint64_t)a_pages * 0x1000,
                         a_target, a_pages, 0);
         // V victim: single page, --transfers jobs, --v-gap gap.
-        configure_batch(SEL_V, V_SRC_IOVA, V_DST_IOVA,
+        configure_batch(SEL_V, V_SRC_IOVA, v_measure_dst_iova(),
                         (uint64_t)v_transfers, 1, v_gap_cycles);
 
         // ---- 3. A NEXT_ID then V NEXT_ID -------------------------------
@@ -1928,8 +1947,22 @@
         // ---- 6. exact byte verification of every touched page ----------
         for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
             CHECK(read64(V_SRC_PA + (uint64_t)i * 8) == page_word(0, i),
                   "V source page was modified");
-            CHECK(read64(V_DST_PA + (uint64_t)i * 8) == page_word(0, i),
-                  "V destination bytes do not match the copied source");
+            CHECK(read64(v_measure_dst_pa() + (uint64_t)i * 8) == page_word(0, i),
+                  "V measurement destination bytes do not match the copied source");
+        }
+        if (v_cold_dst) {
+            // Warm-up, when it runs, still targets V_DST_PA only.
+            for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
+                const uint64_t expected = (warmup > 0) ? page_word(0, i)
+                                                       : page_old(0, i);
+                CHECK(read64(V_DST_PA + (uint64_t)i * 8) == expected,
+                      "V warm destination page does not match its expected state");
+            }
+        } else {
+            // The new cold page is reserved and must not be touched on a hot run.
+            for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
+                CHECK(read64(V_COLD_DST_PA + (uint64_t)i * 8) == page_old(0, i),
+                      "unused cold V destination page was modified");
+            }
         }
         for (uint32_t p = 0; p < a_pages; ++p) {
             for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
                 CHECK(read64(A_SRC_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8)
@@ -2074,6 +2107,7 @@
     uint32_t    transfers= 64;
     uint32_t    warmup   = 256;
     uint32_t    ptw_lat  = 31;
     uint64_t    a_target = 1048576ULL;
+    bool        v_cold_dst = false;
 #ifdef ACTUAL_DMA_SYSTEM
     bool        cpu_writes = false;
     bool        ds_priority = false;
@@ -2097,6 +2131,16 @@
         if (const char* v = val("transfers")) transfers = (uint32_t)std::strtoul(v, nullptr, 0);
         if (const char* v = val("warmup"))    warmup = (uint32_t)std::strtoul(v, nullptr, 0);
         if (const char* v = val("ptw-latency")) ptw_lat = (uint32_t)std::strtoul(v, nullptr, 0);
         if (const char* v = val("a-target"))  a_target = std::strtoull(v, nullptr, 0);
+        if (const char* v = val("v-cold-dst")) {
+            char* end = nullptr;
+            const unsigned long parsed = std::strtoul(v, &end, 0);
+            if (end == v || *end != '\0' || parsed > 1) {
+                std::fprintf(stderr,
+                    "nonblock_actual_dma_test: invalid +v-cold-dst='%s' (expected 0|1)\n", v);
+                return 2;
+            }
+            v_cold_dst = (parsed != 0);
+        }
 #ifdef ACTUAL_DMA_SYSTEM
         if (const char* v = val("cpu-writes")) cpu_writes = (std::strtoul(v, nullptr, 0) != 0);
         if (const char* v = val("ds-priority")) ds_priority = (std::strtoul(v, nullptr, 0) != 0);
@@ -2188,6 +2232,7 @@
         tb.v_transfers     = transfers;
         tb.warmup          = warmup;
         tb.ptw_latency     = ptw_lat;
         tb.a_target        = a_target;
+        tb.v_cold_dst      = v_cold_dst;
 #ifdef ACTUAL_DMA_SYSTEM
         tb.cpu_writes      = cpu_writes;
         tb.ds_priority     = ds_priority;
```

导航与假设：

- `tests/nonblock_actual_dma_test.cpp:80-84`：新增 `V_COLD_DST_IOVA=0x10002000`、`V_COLD_DST_PA=0x80002000`。
- `tests/nonblock_actual_dma_test.cpp:189-192`：`Tb` 新增运行时开关 `bool v_cold_dst=false`。
- `tests/nonblock_actual_dma_test.cpp:436-441`：在 V leaf 表 `0x50010` 增加 cold 目的页映射，PTE 用 `((PA>>12)<<10)|0xDF`。
- `tests/nonblock_actual_dma_test.cpp:452-457`：整页初始化 `V_COLD_DST_PA` 为 `page_old(0,i)`，原两页保留。
- `tests/nonblock_actual_dma_test.cpp:1103-1119`：新增测量目标地址选择 helper；warm 路径仍使用原 `V_DST_IOVA/V_DST_PA`。
- `tests/nonblock_actual_dma_test.cpp:1464-1518`：`ACTUAL-DMA-WINDOW-JSON` 增加 `v_cold_dst`、`v_dst_iova`、`v_dst_pa`；schema 保持 `actual-dma-window-v1`，字段用 number 序列化。
- `tests/nonblock_actual_dma_test.cpp:1853`：仅测量 `configure_batch(SEL_V,...)` 的目的 IOVA 改为 `v_measure_dst_iova()`；pages=1、transfers、源地址、4 KiB 长度、A 调度、窗口/DDR/CPU 逻辑未改。
- `tests/nonblock_actual_dma_test.cpp:1928-1948`：数据检查改为检查实际测量目的页；cold=true 时检查 warm 目的页在 `warmup>0` 为 `page_word(0,i)`、`warmup==0` 为 `page_old(0,i)`；cold=false 时检查新 cold 页仍为 `page_old(0,i)`；A 全页检查保持。
- `tests/nonblock_actual_dma_test.cpp:2077`、`2097-2110`、`2191`：新增 `+v-cold-dst=0|1` 严格解析，非 0/1 返回 2，并接入 `Tb`。

假设/边界：

- 只改 `tests/nonblock_actual_dma_test.cpp`；SV、生产、驱动、Python runner/tests 不动。
- `+v-cold-dst` 默认 `false`，因此默认 hot case 应保持原行为；新增 `0x50010` 映射和 cold 页初始化不应影响旧地址映射。
- 新增 JSON 字段只出现在 `#ifdef ACTUAL_DMA_SYSTEM` 的 `ACTUAL-DMA-WINDOW-JSON`。
- 终点 PASS 仍由 quiet + exact byte checks 共同决定；`report_window()` 仍先打印，但不会把 report 当 PASS。

建议验收：

1. 默认 hot：不传 `+v-cold-dst` 或传 `+v-cold-dst=0`，与旧 93487 同参数跑一次，确认仍 PASS，JSON 中 `"v_cold_dst":0`，`v_dst_iova=268439552`（0x10001000），`v_dst_pa=2147487744`（0x80001000）。
2. cold 短病例：例如 `+mode=p +v-cold-dst=1 +warmup=0` 与 `+mode=pn +v-cold-dst=1 +warmup=2`，确认 PASS，JSON 中 `"v_cold_dst":1`，`v_dst_iova=268443648`（0x10002000），`v_dst_pa=2147487744+4096`（0x80002000），且 warm 目的页/未用页检查符合条件。
3. 最后确认 `SCENARIO-FAIL`/`STALL`/`WAIT` 未出现，且 byte checks / quiet 均通过后打印 PASS。
