### 片段 1：插入 `docs/composition-v1-mac-20261004.md` 顶部的最新纯网口章节

## 2026-10-05 最新纯网口（SSH）入口：composition-trim route-opt + net3 + 172

> 本节覆盖旧的串口上板入口；旧历史说明保留在下方。日常不再接 UART，仍保留 JTAG。net3 内核从原 SD 启动，本轮不写 SD。

### 当前硬件与网络

- 用户已确认（03:06）使用 `tools/composition-trim-20261005/route-opt/ariane_xilinx.bit`；已加载新 BIT。`execution-state.json` 记录本轮 JTAG 加载 1 次、SD 写 0 次、科学实验 0 次（该文件更新时；之后 172 已启动，以 runner 原始输出为准）。
- 加载后 net3 从原 SD 启动：`uname -r = 6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty`，`boot_id = 29d710b2-4949-402d-8055-8734cbaa5220`。
- 用户已拔 UART，网卡换到原 UART USB 口，实际 `UART_REMOVED`；Mac 侧 `en11` 为 `192.168.77.1/24` 且 active，板卡为 `192.168.77.2`。
- JTAG 保留；Linux/网口完全失效时可能需要人工 SD 恢复，不能承诺所有故障自动恢复。

### Mac 项目根目录操作

以下命令均在 Mac 项目根目录执行：

```sh
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
```

### 只重载 BIT（不带 `--payload` = 不写 SD）

```sh
tools/method88-venv/bin/python scripts/mac_update_reboot.py \
  --run --transport ssh \
  --credential-script scripts/run_research2_mac.py \
  --host-ip 192.168.77.1 \
  --board-host 192.168.77.2 \
  --bit tools/composition-trim-20261005/route-opt/ariane_xilinx.bit \
  --expected-kernel "6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty" \
  --out "results/composition-routeopt-net3-$(date +%Y%m%d-%H%M%S)-reload" \
  --boot-timeout 600 \
  --jtag-hz 1000000
```

- 不带 `--payload`：只 JTAG 重载，不写 SD。
- 本轮 reload 实际结果在 `mac-records/results/composition-routeopt-net3-20261005-02-reload/result.json`。
- 下次 `--out` 必须用新目录，不能覆盖旧结果。

### 跑 172（纯网口 SSH attach）

```sh
tools/method88-venv/bin/python tools/composition-v1-20261004/mac_composition.py \
  --run --transport ssh \
  --board-host 192.168.77.2 \
  --credential-script scripts/run_research2_mac.py \
  --ssh-helper scripts/mac_update_reboot.py \
  --through-phase explore \
  --expected-kernel "6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty" \
  --attach-timeout 60 \
  --out "results/composition-routeopt-net3-$(date +%Y%m%d-%H%M%S)"
```

- `--through-phase explore` 才会跑冻结的 172；`--out` 必须不存在。本轮已用 `results/composition-routeopt-net3-20261005-01`，下次必须用新的时间戳 fresh dir。
- `mac_composition.py` 已补 SSH 传输，继承原 `SerialConsole` 的 framer，沿用同一个 `AttachRunner` 和冻结 172，不新增实验框架。
- 现有密码认证 helper 只在 Mac 内存里做 AST 提取；不得列出/保存密码，不得添加免密 key。SSH 复用为 ControlMaster auto、ControlPersist 60、自身短临时 socket，最终 close，并保留原始帧。

### 安全与失败边界

- 失败即停：技术失败保留现场后停止，不自动重跑；科学阴性（如 fast==0、miss==0、无收益）不是技术失败，不因此停止。
- 首次 SSH 加载 01 在 sync 段因 PTY EOF 先于 waitpid 正常结束被误判失败（9 秒停止，未到 JTAG）；Codex 修正仅退出等待，02 已通过。不要把这次误报当 Linux sync 实际超时。
- 不做 payload hash、SD 回读、再短跑全量、完整重编；回传沿用 Mac tar over SSH 到 Dell `out/composition-board-net3-20261005`。
- 不要夸大板端完整 IO/CDC 签核，也不要写成机制创新或性能收益。DeepSeek 起草 SSH/tests/docs；Codex 负责关键退出竞态、架构决定、源码审查、工具执行与实板验收。

依据：`iommu-platform/out/composition-board-net3-20261005/execution-state.json:2-25,39-75`；`iommu-platform/out/composition-board-net3-20261005/ssh-smoke-result.json:1-16`；`iommu-platform/docs/software-update-20261004.md:137-199`；`iommu-platform/docs/composition-v1-mac-20261004.md:57-89`。

---

### 片段 2：CURRENT_STATUS 最新短段

## CURRENT_STATUS（2026-10-05）

- 当前入口：composition-v1 172 走纯网口/SSH；板卡 net3 原 SD 启动，`boot_id=29d710b2-4949-402d-8055-8734cbaa5220`，`uname -r=6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty`。
- UART 已拔，实际 `UART_REMOVED`；Mac `en11=192.168.77.1` active，板 `192.168.77.2`；JTAG 保留。
- 新 `composition-trim route-opt` BIT 已确认并加载；`execution-state.json` 记录本轮 JTAG 1 次、SD 写 0 次、科学实验 0 次（更新时）。之后 172 已启动，以 runner 原始输出为准。
- 172 runner 已启动；早期进度快照 failed=0，但可能过期，本文不写固定计数。完整 172 实验验收未结束。
- 首次 01 的 sync 失败是 PTY EOF 退出等待误判，非 Linux sync 实际超时；02 已通过。
- 证据目录：`out/composition-board-net3-20261005`；reload 证据：`mac-records/results/composition-routeopt-net3-20261005-02-reload/result.json`。

---

### 片段 3：HANDOFF 可追加短段

## HANDOFF（2026-10-05 追加）

- 接手时先确认：板卡 `192.168.77.2`、Mac `en11=192.168.77.1`、`uname -r` 为 net3、`boot_id` 是否变化；日常不接 UART，JTAG 保留。
- 需要重载 BIT：用上面 `mac_update_reboot.py --run --transport ssh ...`，不带 `--payload`；这表示只 JTAG 重载、不写 SD。Linux/网口完全失效可能需人工 SD 恢复，不要承诺全自动恢复。
- 需要跑实验：用上面 `mac_composition.py --run --transport ssh ... --through-phase explore ...`；沿用冻结 172，不新增实验框架。本轮 out 已用 `results/composition-routeopt-net3-20261005-01`；下次用新时间戳 fresh dir。
- 回传：沿用 Mac tar over SSH 到 Dell `out/composition-board-net3-20261005`；不做 payload hash、SD 回读、再短跑全量、完整重编。
- 失败：技术失败保留后停，不自动重跑；科学阴性不是技术失败。
- 凭据红线：不得列出/保存密码，不得添加免密 key，不改账号权限；SSH helper 内存 AST 提取；ControlMaster auto/ControlPersist 60/自身短临时 socket 最终 close，保留原始帧。
- 责任：DeepSeek 起草 SSH/tests/docs；Codex 负责关键退出竞态、架构决定、源码审查、工具执行与实板验收。

---

### 片段 4：validation 可追加短段

## validation（2026-10-05 追加）

- SSH 接口检查：13 项初稿检查通过；最终 15 项接口检查通过；原 composition 入口 5 项通过；原加载器 21 项 + 新增真实 PTY 2 项通过。
- 双身份 SSH 实板复用通过：5.244 秒；第二条无口令提示；自身连接清理；ControlMaster auto/ControlPersist 60；原始帧已保存。`ssh-smoke-result.json` 记录 `passed=true`、`exit_codes=[0,0]`、`control_socket_created=true`、`control_path_cleaned=true`、`uart_opened=false`、`scientific_cases=0`。
- credential guard 误报已确认：命中字符串只在标准 SSH 认证提示里，没有回显凭据行。
- 首次 SSH 加载 01：sync 段因 PTY EOF 先于 waitpid 正常结束被误判失败，9 秒停止，未到 JTAG；Codex 修正仅退出等待，02 已通过。不是 Linux sync 实际超时。
- reload 结果：`mac-records/results/composition-routeopt-net3-20261005-02-reload/result.json`；总证据目录：`out/composition-board-net3-20261005`。
- 未完成：完整 172 实验验收未结束；不要声称板端完整 IO/CDC 签核，也不要声称机制创新或性能收益。

依据：`iommu-platform/out/composition-board-net3-20261005/execution-state.json:39-75`；`iommu-platform/out/composition-board-net3-20261005/ssh-smoke-result.json:1-16`。

---

仍待 172 原始数据：完整 172 实验的最终通过/失败与逐项结果、科学结论（含阴性）以及 route-opt/net3 组合的实际收益/机制结论，均需以 `mac_composition.py` 原始输出和回传证据为准。