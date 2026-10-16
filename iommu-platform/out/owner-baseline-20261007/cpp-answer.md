## 结论

10 个唯一、可独立审查的替换，仅改动 `iommu-platform/tests/nonblock_actual_dma_test.cpp`：

| # | 目标 | 原行定位 |
|---|---|---|
| 1 | `Tb` 增加 `owner_only` / `measurement_phase` runtime 旋钮 | L193 之后（`v_cold_dst` 旋钮旁，L191-193 块内） |
| 2 | `tick()` 每周期驱动 `top->owner_only_i`（`#ifdef` 之外，两种 top 都生效） | L578-579 之后 |
| 3 | `run_scenario()` 入口显式 `measurement_phase=false`（放在 `reset()` 前，使复位 tick 也拉低） | L1839-1840 |
| 4 | WARM-SUMMARY 之后、configure 之前 `measurement_phase=true`，直到终态不清除 | L1863-1864 之后 |
| 5/6 | `ACTUAL-DMA-WINDOW-JSON` 增加 `owner_only` 字段 + 对应实参（`owner_only && measurement_phase`） | L1479-1481、L1512-1513 |
| 7 | 启动打印一行新状态（两 top 通用） | L2221 之后 |
| 8/9/10 | main 的 `owner_only` 变量、`+owner-only=0\|1` 严格 strtoul 校验、`tb.owner_only` 赋值 | L2112、L2144-2145、L2237 |

未触碰：`reset()/warm_phase()/drain_a()/WARM-SUMMARY` 本体、数据/quiet、代理账本检查、`read-admit` 映射与验证、任何 schema 字符串、Python runner。

```json
{
  "replacements": [
    {
      "old": "    bool     v_cold_dst        = false;",
      "new": "    bool     v_cold_dst        = false;\n    // Owner-only admission gate.  BOTH tops expose owner_only_i; tick()\n    // drives owner_only_i = owner_only && measurement_phase every cycle, so\n    // this knob alone is inert until the post-warm measurement phase.\n    bool     owner_only        = false;\n    // Raised after the warm phase and held through the terminal checks (see\n    // run_scenario()); cleared at scenario start so repeated scenarios are\n    // well defined.  Never set or cleared by win_open.\n    bool     measurement_phase = false;"
    },
    {
      "old": "        top->nonblock_enable_i  = nonblock_enable  ? 1 : 0;\n        top->policy_partition_i = policy_partition ? 1 : 0;",
      "new": "        top->nonblock_enable_i  = nonblock_enable  ? 1 : 0;\n        top->policy_partition_i = policy_partition ? 1 : 0;\n        // Present on BOTH tops: suppress fullbuffer_jobs_i admission\n        // permission only while owner_only is requested and the post-warm\n        // measurement phase is active.  Buffer size is unchanged.\n        top->owner_only_i       = (owner_only && measurement_phase) ? 1 : 0;"
    },
    {
      "old": "        setup_memory(a_pages);\n        reset(enable);",
      "new": "        setup_memory(a_pages);\n        // Repeated-scenario hygiene: enter with the owner-only measurement\n        // phase OFF so the reset ticks and the whole warm phase drive\n        // owner_only_i low.  Warm caches and already-selected requests are\n        // untouched.\n        measurement_phase = false;\n        reset(enable);"
    },
    {
      "old": "                    warm_a_drain_cycles,\n                    warm_end_cycle - warm_start_cycle);",
      "new": "                    warm_a_drain_cycles,\n                    warm_end_cycle - warm_start_cycle);\n\n        // ---- 1b. owner-only measurement phase --------------------------\n        // Raised after warm + drain and their summary, before either batch is\n        // configured or triggered, and held through STOP A, check_quiet, the\n        // byte verification and the terminal checks.  Deliberately NOT gated\n        // on win_open: closing the measurement window does not release it.\n        measurement_phase = true;"
    },
    {
      "old": "            \"ACTUAL-DMA-WINDOW-JSON {\"\n            \"\\\"schema\\\":\\\"actual-dma-window-v1\\\",\"\n            \"\\\"mode\\\":\\\"%s\\\",\\\"v_cold_dst\\\":%d,\"",
      "new": "            \"ACTUAL-DMA-WINDOW-JSON {\"\n            \"\\\"schema\\\":\\\"actual-dma-window-v1\\\",\"\n            \"\\\"mode\\\":\\\"%s\\\",\\\"v_cold_dst\\\":%d,\\\"owner_only\\\":%d,\""
    },
    {
      "old": "            name, (int)v_cold_dst,\n            (unsigned long long)v_measure_dst_iova(),",
      "new": "            name, (int)v_cold_dst,\n            (int)(owner_only && measurement_phase),\n            (unsigned long long)v_measure_dst_iova(),"
    },
    {
      "old": "    std::printf(\"NOTE: ptw latency is a plain model delay (DDL/DDR), NOT MIG-matched.\\n\");",
      "new": "    std::printf(\"NOTE: ptw latency is a plain model delay (DDL/DDR), NOT MIG-matched.\\n\");\n    std::printf(\"nonblock_actual_dma_test: owner_only=%d\\n\", (int)owner_only);"
    },
    {
      "old": "    bool        v_cold_dst = false;",
      "new": "    bool        v_cold_dst = false;\n    bool        owner_only = false;"
    },
    {
      "old": "            v_cold_dst = (parsed != 0);\n        }",
      "new": "            v_cold_dst = (parsed != 0);\n        }\n        if (const char* v = val(\"owner-only\")) {\n            char* end = nullptr;\n            const unsigned long parsed = std::strtoul(v, &end, 0);\n            if (end == v || *end != '\\0' || parsed > 1) {\n                std::fprintf(stderr,\n                    \"nonblock_actual_dma_test: invalid +owner-only='%s' (expected 0|1)\\n\", v);\n                return 2;\n            }\n            owner_only = (parsed != 0);\n        }"
    },
    {
      "old": "        tb.v_cold_dst      = v_cold_dst;",
      "new": "        tb.v_cold_dst      = v_cold_dst;\n        tb.owner_only      = owner_only;"
    }
  ]
}
```

## 应用后形状（供 Codex 快速核对）

- `tick()` 开头（L578 附近）：
  ```
  top->nonblock_enable_i  = ...
  top->policy_partition_i = ...
  top->owner_only_i       = (owner_only && measurement_phase) ? 1 : 0;
  #ifdef ACTUAL_DMA_SYSTEM
  top->read_admit_mode_i  = ...
  ```
  `owner_only_i` 驱动位于 `#ifdef` 之外 → Plain/SYSTEM 两种 top 都编译用到该输入。
- `run_scenario()`：
  `setup_memory(a_pages);` → `measurement_phase = false;` → `reset(enable);` → warm → `drain_a()` → WARM-SUMMARY → `measurement_phase = true;` → configure A/V → trigger → … → STOP A → `check_quiet` → 字节校验 → 终态（不再清零）。
- WINDOW-JSON 前缀变为：
  `ACTUAL-DMA-WINDOW-JSON {"schema":"actual-dma-window-v1","mode":"n","v_cold_dst":0,"owner_only":1,"v_dst_iova":...}`（`owner_only` = `(int)(owner_only && measurement_phase)`；schema 字符串与其他字段全部保留）。

## 假设

1. baseline 已按题述在 `tests/nonblock_actual_dma_top.sv` 与 system top 中新增 `input owner_only_i`，且语义为“抑制 `fullbuffer_jobs_i` 许可，(buffer 不缩小，原 read-guard/owner 排空规则不变)”。若某个 top 缺该端口，会在 C++ 编译期报错（`owner_only_i` 未声明）——这是预期的保护性失败。
2. `measurement_phase=false` 放在 `reset(enable)` **之前**，以便复位的 12 个 tick 也已拉低 `owner_only_i`；`reset()`/`warm_phase()`/`drain_a()` 函数体未改动，warm 缓存与已选请求不受影响。
3. `report_window()` 的 WINDOW-JSON 只在 `ACTUAL_DMA_SYSTEM` 分支存在，因此 Plain top 只多出「启动打印」一行 `owner_only=`；WINDOW-JSON 字段仅在 SYSTEM 构建输出。若要求 Plain top 也有该 JSON 字段，需要另行指派（本轮未做，避免扩大范围）。
4. `+owner-only` 未提供时默认 0，旧命令行行为不变；`+owner-only=2`、`+owner-only=abc`、`+owner-only=`（空串）、`+owner-only=1x` 一律 `return 2`。

## 建议验收命令（由 Codex 执行，我未运行）

```
# 1) 两种 top 都能编译（Plain 与 SYSTEM），并检查端口存在
python3 tests/test_nonblock_actual_dma.py            # runner 未改，原有用例应保持 PASS
# 2) 新旋钮严格校验（期望退出码 2）
<binary> +mode=n +owner-only=2      ; echo $?   # -> 2
<binary> +mode=n +owner-only=abc    ; echo $?   # -> 2
<binary> +mode=n +owner-only=       ; echo $?   # -> 2
# 3) 语义检查：owner_only 仅在 measurement phase 生效；WINDOW-JSON 字段出现
<binary> +mode=n +owner-only=0 | grep -o '"owner_only":[01]'   # 期望 "owner_only":0
<binary> +mode=n +owner-only=1 | grep -o '"owner_only":[01]'   # 期望 "owner_only":1
# 4) 回归：不加 +owner-only 时输出与既有基线一致（除新增一行启动打印）
```

验收判据：`measurement_phase` 在 WARM-SUMMARY 之后置位且直到 `PASS` 前不再清零；`win_close` 后不解除；`check_quiet`、字节校验、read-admit 账本（`n_ra_captured == n_ra_ar_fire`、`dbg_ra_buffer_empty`、`dbg_ra_total_debt == 0`）与 quiet 判定逻辑逐字未变。

未解/待 Codex 决策：是否需要在 Plain top 也输出一个含 `owner_only` 的窗口级 JSON（本轮按要求仅保留 SYSTEM 分支的 `ACTUAL-DMA-WINDOW-JSON`）。
