本次新子席已验实：`gpt-6.1-sol / ultra`，模型元数据默认窗口 `500000`，实际运行有效窗口 `475000`。

- 自身 `CODEX_THREAD_ID=01a12f02-ac3f-7261-b553-9f70031b111b`；`CODEX_SESSION_ID=01a12eff-c971-7c62-8071-a08b646a4b61`。
- [子席 session_meta](/home/xvhaocheng/.codex/sessions/2026/10/12/rollout-2026-10-12T16-27-46-01a12f02-ac3f-7261-b553-9f70031b111b.jsonl:1)：创建时间 `2026-10-12T07:27:46.751Z`，CLI `0.162.1`；`source.subagent.thread_spawn` 记载父线程为上述主会话、`depth=1`、`agent_path=/root/sol_runtime_verification`。`context_window` 只含 `window_id=01a12f02-ac3f-7261-b553-9f8d162231d8`，不是容量字段。
- [实际 turn_context](/home/xvhaocheng/.codex/sessions/2026/10/12/rollout-2026-10-12T16-27-46-01a12f02-ac3f-7261-b553-9f70031b111b.jsonl:8)：时间 `2026-10-12T07:27:48.972Z`，`turn_id=01a12f02-ac6e-7903-b04e-8544cc2328af`，`model=gpt-6.1-sol`，`effort=ultra`。
- [模型元数据](/home/xvhaocheng/.codex/models_cache.json:118)：`context_window=500000`；第119行 `max_context_window=872000`；第121行 `effective_context_window_percent=95`。第34行明确支持 `ultra`；完整支持值为 `low/medium/high/xhigh/max/ultra`。缓存时间在[第2行](/home/xvhaocheng/.codex/models_cache.json:2)：`2026-10-12T07:28:34.331334386Z`；第4行客户端版本 `0.162.1`。
- [配置](/home/xvhaocheng/.codex/config.toml:1)：第1、2行分别为 `gpt-6.1-sol`、`ultra`；[第53行起](/home/xvhaocheng/.codex/config.toml:53) 的 `[agents]` 默认子席同为 `gpt-6.1-sol / ultra`。全文55行未发现 `model_context_window`、`model_auto_compact_token_limit`、`model_catalog_json` 的活跃配置或非注释文本出现。

读取时最新 [token_count](/home/xvhaocheng/.codex/sessions/2026/10/12/rollout-2026-10-12T16-27-46-01a12f02-ac3f-7261-b553-9f70031b111b.jsonl:31) 时间为 `2026-10-12T07:29:01.008Z`，原始 `info` 完整如下：

```json
{
  "total_token_usage": {
    "input_tokens": 57350,
    "cached_input_tokens": 36096,
    "cache_write_input_tokens": 0,
    "output_tokens": 2006,
    "reasoning_output_tokens": 807,
    "total_tokens": 59356
  },
  "last_token_usage": {
    "input_tokens": 32958,
    "cached_input_tokens": 24192,
    "cache_write_input_tokens": 0,
    "output_tokens": 1421,
    "reasoning_output_tokens": 516,
    "total_tokens": 34379
  },
  "model_context_window": 475000
}
```

`475000` 有实际运行记录支持，也与 `500000 × 95%` 一致。`872000` 是最大支持窗口；`last_token_usage` 是最近一次请求统计，`total_token_usage` 是累积调用统计，均不代表精确实时窗口占用。

本子席只做本地只读核验，没有修改文件、调用 DeepSeek/Claude、创建 goal 或额外子席；主会话运行值由 root 独立验收。
