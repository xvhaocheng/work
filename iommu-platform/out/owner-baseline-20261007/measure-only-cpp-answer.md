```json
{
  "replacements": [
    {
      "old": "    std::string read_admit_mode_str = \"none\";\n    uint32_t    read_admit_mode_val = 0;",
      "new": "    std::string read_admit_mode_str = \"none\";\n    uint32_t    read_admit_mode_val = 0;\n    // +read-admit-measure-only=0|1 (default 0).  While it is 1 and the post-warm\n    // measurement phase is NOT active (reset ticks, warm-up and the warm A\n    // drain), tick() forces read_admit_mode_i to 1, i.e. the SAME single-slot\n    // proxy buffer used by +read-admit=buffer.  From measurement_phase onward\n    // the requested read_admit_mode_val is driven unchanged, so the measured\n    // window keeps the requested admission mode while the warm phase runs the\n    // buffer.  Default false reproduces the previous behaviour exactly.\n    bool        read_admit_measure_only = false;"
    },
    {
      "old": "        top->read_admit_mode_i = read_admit_mode_val;",
      "new": "        top->read_admit_mode_i =\n            (read_admit_measure_only && !measurement_phase) ? 1 : read_admit_mode_val;"
    },
    {
      "old": "    uint32_t    read_period = 256;",
      "new": "    uint32_t    read_period = 256;\n    bool        read_admit_measure_only = false;"
    },
    {
      "old": "        if (const char* v = val(\"read-period\")) read_period = (uint32_t)std::strtoul(v, nullptr, 0);",
      "new": "        if (const char* v = val(\"read-period\")) read_period = (uint32_t)std::strtoul(v, nullptr, 0);\n        if (const char* v = val(\"read-admit-measure-only\")) {\n            char* end = nullptr;\n            const unsigned long parsed = std::strtoul(v, &end, 0);\n            if (end == v || *end != '\\0' || parsed > 1) {\n                std::fprintf(stderr,\n                    \"nonblock_actual_dma_test: invalid +read-admit-measure-only='%s' (expected 0|1)\\n\", v);\n                return 2;\n            }\n            read_admit_measure_only = (parsed != 0);\n        }"
    },
    {
      "old": "        tb.read_admit_mode_str = read_admit;\n        tb.read_admit_mode_val = read_admit_val;",
      "new": "        tb.read_admit_mode_str = read_admit;\n        tb.read_admit_mode_val = read_admit_val;\n        tb.read_admit_measure_only = read_admit_measure_only;"
    },
    {
      "old": "            \"\\\"mode\\\":\\\"%s\\\",\\\"read_admit\\\":\\\"%s\\\",\\\"budget\\\":%u,\\\"period\\\":%u,\"\n            \"\\\"captures\\\":%llu,\\\"offer_handshakes\\\":%llu,\\\"firstoffers\\\":%llu,\"",
      "new": "            \"\\\"mode\\\":\\\"%s\\\",\\\"read_admit\\\":\\\"%s\\\",\\\"budget\\\":%u,\\\"period\\\":%u,\"\n            \"\\\"measure_only\\\":%d,\"\n            \"\\\"captures\\\":%llu,\\\"offer_handshakes\\\":%llu,\\\"firstoffers\\\":%llu,\""
    },
    {
      "old": "            name, read_admit_mode_str.c_str(), read_budget, read_period,\n            (unsigned long long)w_ra_captures,",
      "new": "            name, read_admit_mode_str.c_str(), read_budget, read_period,\n            (int)read_admit_measure_only,\n            (unsigned long long)w_ra_captures,"
    }
  ]
}
```

Notes / assumptions (single file `iommu-platform/tests/nonblock_actual_dma_test.cpp`, 7 edits, no other files):

- Edit 1: `Tb::read_admit_measure_only` added inside the `#ifdef ACTUAL_DMA_SYSTEM` runtime read-admit knob block (right after `read_admit_mode_val`, before `read_budget`); default `false`.
- Edit 2: `tick()` drives `read_admit_mode_i = (read_admit_measure_only && !measurement_phase) ? 1 : read_admit_mode_val;`. With the default `false` this is exactly `read_admit_mode_val`, so old behavior is bit-for-bit preserved; `1` is the existing single-slot buffer mode. Other knob drives (`read_budget_i`, `read_period_i`), phase gating, `owner_only_i`, algorithm/proxy and budgets are untouched.
- Edit 3–5: `main` bool (inside `#ifdef ACTUAL_DMA_SYSTEM`), strict parse of `+read-admit-measure-only` using `strtoul` with `endptr` (`end == v || *end != '\0' || parsed > 1` → exit 2, same style as `+v-cold-dst` / `+owner-only`), and `tb.read_admit_measure_only = read_admit_measure_only;` alongside the other read-admit assignments.
- Edit 6–7: `READ-ADMIT-WINDOW-JSON` gains exactly one new field `"measure_only":%d` placed after `"period":%u` and before `"captures"`, with the matching arg inserted at the same position (`period` → `measure_only` → `w_ra_captures`). The value is the requested runtime flag `(int)read_admit_measure_only`, not the effective `mode_i` (effective `mode_i` is already 0/gated during `measurement_phase`, and the JSON is printed at window close). Schema string, all pre-existing fields, all other data/quiet/check logic and the terminal `READ-ADMIT-TERMINAL-JSON` / `if (read_admit_mode_val != 0)` guard block are unchanged.
- Assumption flagged for Codex: only the raw flag is reported (`measure_only` = 1 even when `+read-admit=none`, i.e. the "warm uses buffer, measurement uses none" combination you said will not be run yet). If you instead want the *effective* mode gating in JSON, that arg would become `(int)(read_admit_measure_only && !measurement_phase)` — not done here per "用实际flag".
- No extra console print (e.g. extending the `read_admit=%s budget=%u period=%u` line) was added, since it was not requested; JSON carries the new field.

Suggested acceptance checks (parent to run):
1. Rebuild/recompile only this C++ file (no other artifact changed); confirm compile clean.
2. Default run (no `+read-admit-measure-only`) with a non-zero `+read-admit` (e.g. `walk`): WARM-SUMMARY phase counters/PN lines must be byte-identical to the previous buffer-mode warm run (warm still runs the requested mode when the flag is 0).
3. One or two `+mode=n +read-admit=walk +read-admit-measure-only=1` cold-page runs, compared against the same run without the flag: warm phase numbers should match the old `+read-admit=buffer` warm numbers (bar `measure_only` in JSON), while the measured window should match `walk`; expect `READ-ADMIT-WINDOW-JSON` to contain `"measure_only":1` at the specified position and all other fields byte-identical.
4. Strict-parse check: `+read-admit-measure-only=2`, `=x`, `=` → exit code 2 with the new error line.
