{
  "replacements": [
    {
      "old": "        top->eval();\n\n#ifdef ACTUAL_DMA_SYSTEM\n        // ================= read-admission AXI stability (pre-edge) =========",
      "new": "        top->eval();\n\n        CHECK(!top->rst_ni || top->dbg_e2_fault == 0,\n              \"E2 pool reported a fault after reset\");\n\n#ifdef ACTUAL_DMA_SYSTEM\n        // ================= read-admission AXI stability (pre-edge) ========="
    },
    {
      "old": "                    v_gap_cycles, ptw_latency);\n    }",
      "new": "                    v_gap_cycles, ptw_latency);\n\n        // ---- 8. E2 pool terminal acceptance -----------------------------\n        const uint32_t e2_mode      = (uint32_t)top->dbg_e2_mode;\n        const uint32_t e2_slots     = (uint32_t)top->dbg_e2_slots;\n        const uint32_t e2_depth     = (uint32_t)top->dbg_e2_dma_depth;\n        const uint32_t e2_occupancy = (uint32_t)top->dbg_e2_occupancy;\n        const uint32_t e2_reserved  = (uint32_t)top->dbg_e2_reserved;\n        const uint32_t e2_busy      = (uint32_t)top->dbg_e2_busy;\n        const uint32_t e2_grant_pool   = (uint32_t)top->dbg_e2_grant_pool;\n        const uint32_t e2_grant_stream = (uint32_t)top->dbg_e2_grant_stream;\n        const uint32_t e2_received  = (uint32_t)top->dbg_e2_received;\n        const uint32_t e2_replayed  = (uint32_t)top->dbg_e2_replayed;\n        const uint32_t e2_highwater = (uint32_t)top->dbg_e2_highwater;\n\n        CHECK(e2_occupancy == 0,\n              \"E2 pool occupancy is nonzero at terminal\");\n        CHECK(e2_reserved == 0,\n              \"E2 pool reserved-beat count is nonzero at terminal\");\n        CHECK(e2_busy == 0,\n              \"E2 pool pending-read mask is nonzero at terminal\");\n        CHECK(e2_received == e2_replayed,\n              \"E2 pool received/replayed beat counts differ at terminal\");\n        if (e2_mode != 0) {\n            CHECK(e2_depth == 3,\n                  \"E2 mode requires dma_buffer_depth == 3 at terminal\");\n            CHECK(top->dbg_dma_fullbuffer == 0,\n                  \"E2 mode terminal fullbuffer certificate is nonzero\");\n        } else {\n            // e2_mode 0 is the legacy bypass; passing this branch does NOT\n            // certify the new E2 pool.\n        }\n\n        std::printf(\"E2-TERMINAL-JSON {\"\n                    \"\\\"schema\\\":\\\"actual-dma-e2-terminal-v1\\\",\"\n                    \"\\\"scenario\\\":\\\"%s\\\",\"\n                    \"\\\"e2_mode\\\":%u,\\\"pool_slots\\\":%u,\\\"dma_buffer_depth\\\":%u,\"\n                    \"\\\"pool_capacity_bytes\\\":%llu,\"\n                    \"\\\"private_payload_bytes\\\":%llu,\"\n                    \"\\\"reserved_beats\\\":%u,\\\"occupancy_beats\\\":%u,\"\n                    \"\\\"pending_reads\\\":%u,\"\n                    \"\\\"grants_pool\\\":%u,\\\"grants_stream\\\":%u,\"\n                    \"\\\"received_beats\\\":%u,\\\"replayed_beats\\\":%u,\"\n                    \"\\\"highwater_beats\\\":%u,\"\n                    \"\\\"scope\\\":\\\"cumulative_including_warm_not_window\\\"}\\n\",\n                    name, (unsigned)e2_mode, (unsigned)e2_slots,\n                    (unsigned)e2_depth,\n                    (unsigned long long)((uint64_t)e2_slots * 2048ULL),\n                    (unsigned long long)((uint64_t)e2_depth * 16ULL),\n                    (unsigned)e2_reserved, (unsigned)e2_occupancy,\n                    (unsigned)e2_busy,\n                    (unsigned)e2_grant_pool, (unsigned)e2_grant_stream,\n                    (unsigned)e2_received, (unsigned)e2_replayed,\n                    (unsigned)e2_highwater);\n\n    }"
    },
    {
      "old": "        std::printf(\n            \"ACTUAL-DMA-WINDOW-JSON {\"\n            \"\\\"schema\\\":\\\"actual-dma-window-v1\\\",\"",
      "new": "        std::printf(\n            \"ACTUAL-DMA-WINDOW-JSON {\"\n            \"\\\"schema\\\":\\\"actual-dma-window-v1\\\",\"\n            \"\\\"e2_mode\\\":%u,\\\"pool_slots\\\":%u,\\\"dma_buffer_depth\\\":%u,\""
    },
    {
      "old": "            name, (int)v_cold_dst,",
      "new": "            (unsigned)top->dbg_e2_mode,\n            (unsigned)top->dbg_e2_slots,\n            (unsigned)top->dbg_e2_dma_depth,\n            name, (int)v_cold_dst,"
    }
  ],
  "assumptions": [
    "Both Vnonblock_actual_dma_top and Vnonblock_actual_dma_system_top expose dbg_e2_mode, dbg_e2_slots, dbg_e2_dma_depth, dbg_e2_occupancy, dbg_e2_reserved, dbg_e2_busy, dbg_e2_fault, dbg_e2_grant_pool, dbg_e2_grant_stream, dbg_e2_received, dbg_e2_replayed, dbg_e2_highwater as uint32 scalar outputs.",
    "dbg_dma_fullbuffer is a 2-bit mask where 0 means neither channel is full; checking == 0 is the required terminal certificate for e2_mode != 0.",
    "dbg_e2_mode is static elaboration: 0=old bypass, 1=normal full-response reserved pool, 2=E2 dependency-selection pool.",
    "top->rst_ni is deasserted (1) after reset() completes and stays 1 for the rest of the scenario; the fault check is gated on rst_ni to avoid reset-time noise.",
    "The E2 terminal checks run for all scenarios including mode0. For mode0 the depth==3 and fullbuffer==0 checks are skipped (bypass is not certified as the new feature), but occupancy/reserved/busy==0 and received==replayed still run.",
    "Optional window counters for grant/receive/replay were not added; only the required terminal cumulative counters are reported.",
    "No new CLI options are introduced and C++ does not drive any new control signals.",
    "The parent will add the named dbg_e2_* outputs to both tops before compiling."
  ],
  "checks": [
    "Compile both Vnonblock_actual_dma_top and Vnonblock_actual_dma_system_top builds.",
    "Run mode s (bypass): E2-TERMINAL-JSON must print e2_mode=0; occupancy/reserved/busy must be 0; received==replayed must hold; no depth-check failure.",
    "Run a mode with e2_mode != 0 (n/p/pn as elaborated): E2-TERMINAL-JSON must print e2_mode!=0, dma_buffer_depth=3, dbg_dma_fullbuffer=0 at terminal, occupancy/reserved/busy=0, received==replayed.",
    "Verify ACTUAL-DMA-WINDOW-JSON now contains e2_mode, pool_slots, dma_buffer_depth with the correct static elaboration values and that all prior numeric fields are unchanged.",
    "Inject dbg_e2_fault=1 after reset and verify the per-tick CHECK fails.",
    "Verify no existing ++ warm/model counters, STOP, quiet checks, window counters, or PASS/FAIL behavior were removed or altered."
  ]
}
