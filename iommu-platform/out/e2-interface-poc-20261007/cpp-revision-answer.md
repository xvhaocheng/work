{
  "replacements": [
    {
      "old": "        top->eval();\n\n#ifdef ACTUAL_DMA_SYSTEM\n        // ================= read-admission AXI stability (pre-edge) =========",
      "new": "        top->eval();\n\n        CHECK(!top->rst_ni || top->dbg_e2_mode == 0 || top->dbg_dma_fullbuffer == 0,\n              (std::string(\"E2 mode reported fullbuffer after reset (dbg_e2_fault=\") +\n               std::to_string((unsigned)top->dbg_e2_fault) + \")\").c_str());\n\n#ifdef ACTUAL_DMA_SYSTEM\n        // ================= read-admission AXI stability (pre-edge) ========="
    }
  ]
}
