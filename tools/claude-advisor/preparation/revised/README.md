# claude-advisor bridge（模板 / 未接通）

## 调用策略（先读）

- Claude 只在**用户当前主动声明**“让 Claude / Anthropic 参与”时使用，用于计划、创造力、交叉讨论与审计。
- 用户没有点名时，Codex 与 DeepSeek 按日常分工继续工作；不要因为任务重要、复杂或“值得复审”就自动调用 Claude。
- 一个简单触发例：`这次让 Claude、Codex、DeepSeek 交叉审论文主线`。
- 本桥只是工作约定与追溯工具，**不能验证用户真实授权**。调用方不能凭“任务重要/值得复审”捏造 `user_request`。

本目录包含一个**小而可审阅**的桥接脚本，用于让上层 Agent 在用户当次明确点名时向官方
Anthropic Messages API 发起一次**单轮、无工具**的咨询。

## 当前状态（请先读完）

- **未配置**：`config.json` 尚未创建，`model`、`max_tokens` 均为 `null`。
- **未接通**：尚未进行任何真实 API 调用，没有选择具体 Claude 模型，也没有验证过
  任何模型的 thinking 支持情况。
- **MCP 模板禁用**：`codex-mcp.example.toml` 只是模板，`enabled = false`，本轮不会
  写入或修改任何全局 Codex 设置，也不会注册。
- **未安装**：没有任何全局安装动作。
- **离线测试**：前版 36 个离线测试已通过；本次修订后的测试尚未执行，待 Codex 运行验证。没有真实 Claude 调用、没有模型、没有连通性验证。

在用户完成密钥与模型配置、且真实调用验证通过之前，**不要启动 `--serve`**，也不要
把模板复制进全局配置。

## 这个桥不是自主 Agent

它不做以下任何事情：

- 不调用工具、不执行命令、不写文件、不访问网络（除了一次向官方端点的 POST）
- 不自动重试、不跟随 HTTP 重定向、不做流式输出
- 不读取工作区源码，除非调用方显式传入 `--files` 并给出绝对 `--root`
- 不打印、不记录、不落盘 API key
- 不在用户未当次点名 Claude/Anthropic 时自动发起咨询；每次外发都必须在当次用户请求范围内

它只做一件事：把调用方给出的任务与显式上下文发给模型，然后把**完整原始回复**
（含 `content` 块、`usage`、`stop_reason`）以 JSON 返回。CLI 结果是 stdout 上的完整
JSON，调用方自行重定向到本地 artifact。

## 文件

| 文件 | 用途 |
| --- | --- |
| `bridge.py` | 桥本体，仅使用 Python 3 标准库 |
| `config.example.json` | 配置模板，复制为 `config.json` 后填写 |
| `codex-mcp.example.toml` | MCP 配置模板，**保持禁用** |
| `test_bridge.py` | 离线单元测试，全部使用假 opener，不联网 |
| `README.md` | 本文件 |

## 配置

复制模板并填写：

```
cp config.example.json config.json
```

允许的字段只有以下这些；出现任何其它字段（尤其是 `api_key`、`token`、`secret`
之类的明文字段）配置会被**直接拒绝**，不会静默忽略：

- `base_url`：必须严格等于 `https://api.anthropic.com`。第三方代理与重定向一律不接受。
- `model`：**当前为 `null`**。等用户确定具体模型后再填。不要从 DeepSeek 等其它
  供应商照搬模型名。
- `api_key_env`：默认 `ANTHROPIC_API_KEY`。
- `api_key_file`：**当前为 `null`**。可选用；见下方密钥规则。
- `api_version`：默认 `2023-06-01`。
- `workspace_id`：默认 `null`。多 workspace key 时可能需要。
- `max_tokens`：**当前为 `null`**。必须由用户按所选模型确定，不要猜。真实调用要求
  它是正整数。
- `thinking`：**当前为 `null`**。等模型确定、并确认该模型支持后再设置。只允许
  `null`、`{"type": "adaptive"}`、或 `{"type": "enabled", "budget_tokens": N}`，其中
  `N` 为不小于 1024 的整数且小于 `max_tokens`。**禁止静默降级**：不满足条件会直接
  报错，不会自动改小。
- `timeout_seconds`：默认 `600`。

### 密钥规则

密钥只有两个来源，二选一：

1. 环境变量（名字由 `api_key_env` 指定）；
2. 仓库外的私有文件（`api_key_file`）。

对密钥文件的要求（任一不满足都直接拒绝）：绝对路径、真实普通文件、不是符号链接、
权限 `0600`（组/其他人无任何权限）、且**不在工作区目录树内**。工作区目录由桥所在
目录向上推导，因此不要试图把 key 放在本仓库或其上层工作目录里。

无论来自环境变量还是文件，key 若包含 CR/LF 或不能安全放进 HTTP header 的字符，
都会在创建请求前被拒绝，且不会回显 key 内容。

`--status` 只检查环境变量**是否存在且非空**，不读取密钥内容，也不打印任何密钥或
密钥文件路径。`--status` 不做网络请求，没有 key 时依然以退出码 0 结束。若
`api_key_env` 不是合法字符串，`--status` 不会因为环境变量映射报错，而是给出安全
错误、`call_ready` 为 false，并仍然退出 0。

## 用法

状态（本地、无网络、无 key 也退出 0）：

```
python3 bridge.py --status
```

咨询（需要用户当次明确点名 Claude/Anthropic；需要已配置的 `config.json`，且 `model` / `max_tokens` 已填）：

```
python3 bridge.py --task "审查这份改动计划" --role review --user-request "这次让 Claude 参与审查"
python3 bridge.py --task "给出论文主线" --role paper --context "..." --user-request "这次让 Claude 给论文主线"
python3 bridge.py --task "读这两处代码" --role discuss --root /abs/project --files src/a.py src/b.py --user-request "这次让 Claude 和 DeepSeek 交叉讨论"
```

角色：`plan`、`discuss`、`review`、`ideas`、`paper`、`research`。

### 传入文件

- `--files` 只接受**显式相对路径**，且必须配合**绝对** `--root`。
- `--root` 不能是 `/`，也不能是用户主目录。
- 拒绝：绝对路径、任何 `..`、任何隐藏路径段（`.` 开头，含 `.git`、`.env`）、
  名字中包含 `secret` / `credential` / `apikey` / `api_key` / `id_rsa` /
  `id_ed25519` / `password` / `token` 之类的文件、指向外部的符号链接。
- 所有文件按 UTF-8 读取，最多 128 个、合计最多 8 MiB；累计字节按实际读入内容计算，读取时最多多读 1 字节以检测文件增长。
- 不传 `--files` 时，桥不会去读取任何仓库源码；但提供 `--root` 时仍会校验 root 合法性。
- `--files` 只接受字符串列表；`str`、`dict`、`tuple` 等类型不会静默当作空列表，会直接报参数错误。

### 结果

成功时 stdout 是一个 JSON 对象，包含 `model`、`role`、`answer`、`content`
（完整原始回复块）、`usage`、`stop_reason`、`truncated`、`user_request` 等字段。
`user_request` 原样保留调用方给出的用户引用文本，只用于工作约定与追溯，**不能证明
用户真实授权**。`provider_response` 保留服务端原始 JSON 中有用的部分，但不会包含
任何 header 或密钥。

`truncated` 在 `stop_reason == "max_tokens"` **或**回复中没有文本块时为 `true`。

失败时 stdout 是 `{"ok": false, "error": {"category": ..., "message": ...}}`，
退出码为 1。HTTP 错误只报告状态码与安全分类，不回显服务端 body，也不打印 URL 中
可能携带的敏感片段。

## 给调用方的要求

- **调用方必须自行筛选敏感内容。** 这个桥无法检测所有密钥、凭证或隐私数据，
  它只做有限的路径与文件名检查。不要把不该外发的材料放进 `--context` 或 `--files`。
- **只在用户当次明确点名 Claude/Anthropic 时调用。** 调用方不能因为“任务重要/值得
  复审”而自行调用，也不能捏造 `user_request`；该字段只是约定与追溯，不能验证真实授权。
- 不是每个小操作都需要调用本桥。Codex 保留关键架构决策、复杂诊断、最终审阅权。
- 需要讨论连续性时，请把历史作为明确的 `--context` 文本传入。本版本**不实现**会话
  数据库。

## 对模型的要求（system 提示中已写死）

区分证据 / 推断 / 假设 / 建议；缺失信息直说；引用代码用 `path:line`；引用论文
必须真实；不得伪造浏览、命令、测试结果或引用。

角色约定：`review` 给致命问题 / 改进 / 最小验证 / 终止条件；`research` 最多 3 个
有判据的方向；`paper` 按 problem-mechanism-gap-method-baselines-evidence-limitations
组织主线。

## 可选的交叉讨论流程（不是固定轮次）

当用户明确要求 Claude、Codex、DeepSeek 交叉讨论时，可以使用以下**可选**流程，不要求
每次固定三方五轮：

1. 独立提案：各方先基于**同一证据包**独立给出提案，独立阶段不提前互抄。
2. 交换质疑：互相阅读后提出质疑与反例。
3. 修订：根据质疑修订方案。
4. 可检验方案：给出可检验的下一步与验收条件。

注意：共同历史会影响独立性，需要在记录中标注；最终记录分歧、未决问题与验收条件；
模型之间的共识不是证据。

## MCP（模板，暂不启用）

`codex-mcp.example.toml` 提供两个工具：`claude_status` 与 `claude_consult`，走
stdio JSON-RPC，不污染 stdout。协议版本支持 2024-11-05 / 2025-03-26 / 2025-06-18 /
2025-11-25 / 2026-07-28，未知版本回落到 2025-11-25。`claude_consult` 的
`user_request` 是**必需**参数，必须传入用户当次明确点名 Claude/Anthropic 的原话。

**在真实密钥与模型配置并验证完成之前，不要启用、也不要注册本 MCP。**

## 官方来源

- Anthropic Messages API：https://docs.anthropic.com/en/api/messages
- Anthropic API overview：https://docs.anthropic.com/en/api/overview
- Anthropic Thinking：https://docs.anthropic.com/en/docs/build-with-claude/extended-thinking
- Codex MCP：https://github.com/openai/codex/blob/main/docs/config.md#mcp

以上链接用于说明协议与配置来源；本轮没有实际连通性验证。

## 测试

```
python3 -m unittest test_bridge
```

测试全部使用进程内假 opener，不会发起任何真实网络请求。前版 36 个离线测试已由
Codex 执行通过；本次修订后的测试尚未执行，待 Codex 运行。没有真实 Claude 调用、
没有模型、没有真实连通性验证。
