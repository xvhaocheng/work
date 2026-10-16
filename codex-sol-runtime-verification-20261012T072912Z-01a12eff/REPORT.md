# 本次新 Sol 主会话与子 agent 核验

结论：两席通过。本次实际日志均为 `gpt-6.1-sol / ultra`，原生元数据默认 `500000`，运行有效窗口 `475000`。

采样时间：2026-10-12T16:34:34.367300+09:00（日本时间）。所有数值均来自本次线程，未用旧会话或 API 最大容量替代验收。

| 席位／线程ID | 实际模型 | 实际推理强度 | 原生默认窗口 | 运行有效窗口 | 是否通过 |
|---|---|---|---:|---:|---|
| 主会话 `01a12eff-c971-7c62-8071-a08b646a4b61` | gpt-6.1-sol | ultra | 500000 | 475000 | 通过 |
| 子 agent `01a12f02-ac3f-7261-b553-9f70031b111b` | gpt-6.1-sol | ultra | 500000 | 475000 | 通过 |

## 当前配置

当前 `/home/xvhaocheng/.codex/config.toml` 第1、2行指定 `gpt-6.1-sol / ultra`；第53–55行的子 agent 默认也相同。检查全文所有55行、系统与工作目录祖先配置路径，以及活动 app-server 的启动参数/相关环境：未发现 `model_context_window`、`model_auto_compact_token_limit` 或 `model_catalog_json` 活跃覆盖。活动启动项只有 `features.code_mode_host=true`。配置采样前后哈希相同。

详见 [配置证据](config-observation.json)、[配置完整性](config-integrity.json)。仅保存相关配置行，不复制密钥或无关 MCP 配置。旧备份和未被引用的自定义目录文件没有被当作当前配置。

## 原生模型元数据

原生模型目录：当前 OpenAI provider 的 `models_cache.json`；已保存字节一致的只读快照。`slug=gpt-6.1-sol` 位于快照第8行，`context_window=500000` 位于第118行，`max_context_window=872000` 位于第119行，`effective_context_window_percent=95` 位于第121行。缓存时间 2026-10-12T07:28:34.331334386Z，客户端版本 0.162.1。

默认窗口 `500000`、最大支持窗口 `872000`、有效窗口 `475000` 分别记录。`500000 × 95% = 475000` 只解释元数据关系；实际有效容量仍由各席本次 `token_count` 独立证明。

## 本次原始运行证据

### 主会话

- 原日志：`/home/xvhaocheng/.codex/sessions/2026/10/12/rollout-2026-10-12T16-24-37-01a12eff-c971-7c62-8071-a08b646a4b61.jsonl`。创建时间（JST）`2026-10-12T16:24:37.619000+09:00`；CLI `0.162.1`，provider `openai`。
- `session_meta`：原第1行；线程身份和父子关系。`context_window` 只含 `window_id`，不作为容量证据。
- `turn_context`：原第8行，时间 `2026-10-12T07:24:39.794Z`；实际 `model=gpt-6.1-sol`，`effort=ultra`。
- 最新 `token_count`：原第96行，时间 `2026-10-12T07:31:13.370Z`；`info.model_context_window=475000`。
- 原始记录副本：`root-runtime.raw.jsonl`；逐行映射、记录 SHA256 和原日志采样字节前缀见 `runtime-provenance.json`。

### 子 agent

- 原日志：`/home/xvhaocheng/.codex/sessions/2026/10/12/rollout-2026-10-12T16-27-46-01a12f02-ac3f-7261-b553-9f70031b111b.jsonl`。创建时间（JST）`2026-10-12T16:27:46.751000+09:00`；CLI `0.162.1`，provider `openai`。
- `session_meta`：原第1行；线程身份和父子关系。`context_window` 只含 `window_id`，不作为容量证据。
- `turn_context`：原第8行，时间 `2026-10-12T07:27:48.972Z`；实际 `model=gpt-6.1-sol`，`effort=ultra`。
- 最新 `token_count`：原第45行，时间 `2026-10-12T07:30:26.548Z`；`info.model_context_window=475000`。
- 子席完成：原第46行，时间 `2026-10-12T07:30:26.551Z`。root 在其完成后直接读取原日志，不依赖创建参数或子席自述。`fork_turns=none`、指定模型和强度的请求另存。
- 原始记录副本：`child-runtime.raw.jsonl`；逐行映射、记录 SHA256 和原日志采样字节前缀见 `runtime-provenance.json`。

## Tokens 使用与容量的区别

| 席位 | 最近请求输入 | 最近请求总 tokens | 累计请求总 tokens | 精确实时已占窗口 |
|---|---:|---:|---:|---|
| 主会话 | 55162 | 56154 | 474152 | 未验实 |
| 子 agent | 37351 | 38845 | 134681 | 未验实 |

`last_token_usage` 是最近一次请求的输入/输出统计；`total_token_usage` 是多次请求累积，可能反复计入相同上下文。二者均不冒充精确实时窗口占用。主会话数值为报告生成前的采样，不包含随后报告和最终答复的所有 tokens。完整输入、缓存输入、输出、推理输出字段保存在验收 JSON 和原始 `token_count` 中。

## 验收边界与任务结束

每席均直接有本次 `turn_context` 与 `token_count` 原始证据，并与当前未覆写的原生元数据对应；两席满足全部目标。没有需要解释的目标不符。没有查询不可见的服务端实际路由，也不据此猜测。

本轮仅使用 Codex，创建恰好一个独占子 agent，`fork_turns=none`。未接收研究交接、未登记 goal、未进入研究执行；未修改配置、缓存、数据库或历史记录，未重启服务。只新建本独立核验产物目录。
