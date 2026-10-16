# 两 BIT Mac 交付交接（2026-10-03）

## 0. 状态

- 本文件是交付交接，不是板端结果报告。
- 共用Linux00093、E2 BIT和方法BIT已完成构建/主机验证。方法版经物理优化，最终WNS +0.048ns；E2为+0.177ns。两者建立/保持/脉宽失败端点及组合回路均0。尚未上板，不能视为板端验收或性能结果。
- 所有板端结论必须来自实际运行日志与冻结 analyzer 输出。

## 1. 目标

- 一份共用 Linux：`6.5.0-rc2-00093-gc0f5e13ea81f`。
- 一份共用 payload：`artifacts/fw_payload.bin`。
- 首次 SD 更新由用户按其既有方法完成；本文不提供、也不替换 SD 写入、格式化或分区操作命令。
- SD 更新后，两个 BIT 由 `scripts/mac_research2.py` 通过 JTAG 顺序切换并自动执行；之后不再改 SD。
- 第一段 E2 64 项先完成、保存并重新哈希验证，再切第二 BIT。
- 第二段 method 462 项使用已冻结 plan 与参数选择流程。
- 技术失败首错停止；不自动重跑、不删除、不 resume、不修改原始文件。
- 保留 short gate、errorstop、allraw 语义：短验收不删减，技术错误立即停止，所有 raw console、case archive、record、metadata 原样保存。

## 2. 交付包结构

```text
DELIVERY.json
artifacts/role1.bit
artifacts/role2.bit
artifacts/fw_payload.bin
artifacts/idma_research
scripts/mac_research2.py
scripts/research2_plan.py
scripts/research2_results.py
scripts/research_results.py
plans/e2.json
plans/method.json
docs/mac-research2.md
docs/research-two-bit-v2.md
docs/research-two-bit-mac-20261003.md
```

说明：打包脚本现在还会通过正常 member hash 路径加入两份时序证据：
`provenance/timing/role1.timing_summary_routed.rpt` 与
`provenance/timing/role2.timing_summary_routed.rpt`（role2 为 authenticated post-route optimization 的 `timing.rpt`），并将解析后的
`Design Timing Summary` 数值行、显式组合环计数和报告 SHA256 记录到
`DELIVERY.json` 与 `acceptance.json`。role2 还会加入 additive
`provenance/postroute/manifest.json`、`optimize.tcl`、`execution.json` 和 `input.json`；不打包大体积 `optimized.dcp`。

- `artifacts/idma_research` 是板端 CLI；其 SHA256 与 `software.cli_sha256` 必须一致。
- `artifacts/fw_payload.bin` 是共用 payload；两个 BIT 共用同一份 Linux 用户态与内核接口。
- `plans/e2.json` 对应 E2 64 项；`plans/method.json` 对应 method 462 项。
- `scripts/research2_results.py` 是新版离线分析入口，也供 runner 校验；`scripts/research_results.py` 是其复用的旧证据校验依赖。

## 3. 首次 SD 更新

1. 用户按自己的既有 SD 更新方法，将 `artifacts/fw_payload.bin` 更新到 SD；它已包含 Linux `6.5.0-rc2-00093-gc0f5e13ea81f`，无需另复制一份 Linux Image。
2. 本文不提供任何 SD 写命令、不格式化、不操作分区、不写 bootrom。
3. SD 更新完成后，后续两个 BIT 的切换全部由 Mac 脚本经 JTAG 自动完成，不再改 SD。

## 4. 运行前本地 preflight

```bash
python3 scripts/mac_research2.py --manifest DELIVERY.json --package-root . --serial /dev/tty.usbserial-XXXX --preflight
```

本地 preflight 只做：

- 校验 manifest schema 与本地包内文件 SHA256。
- 校验 openFPGALoader、pyserial、串口空闲。
- 打印预期 kernel_release、cli_sha256、board_cli_path 与预期 feature bits。

本地 preflight 不执行板端命令，不查询板端 CLI SHA256，不执行 `--caps2`。板端 CLI SHA256 与 `--caps2` 是每次 flash 并启动后的 postboot 检查，且必须在任何 case 之前通过。

## 5. 正式运行

```bash
python3 scripts/mac_research2.py --manifest DELIVERY.json --package-root . --out /tmp/research2-run-01 --serial /dev/tty.usbserial-XXXX --stage both --startup-timeout 300
```

- `--out` 必须不存在。
- `--stage both` 的顺序固定为 role1 E2 64，然后 role2 method 462。
- 每个 BIT 只调用一次 openFPGALoader，不自动重试。
- 每次 flash 前启动串口 reader，完整追加 UART 字节到 `console.raw.log`。
- 第一段所有 case 的 record、stderr、exit、archive、hash、metadata 全部落盘并通过 `_verify_stage_saved` 后，才允许 `sync` 并 flash 第二 BIT。
- 第二段 boot_id 必须与第一段不同；kernel、CLI SHA256、caps schema/success/magic/version/role/features/observer_magic 必须通过。
- 技术失败首错停止；科学阴性由 analyzer 判断，runner 不判。

## 6. 冻结计划

- E2：64 项，按 `plans/e2.json` 与 `docs/research-two-bit-v2.md` 的冻结清单执行。
- method：462 项，按 `plans/method.json` 与 `docs/research-two-bit-v2.md` 的冻结清单执行。
- 校准选择只在 phase 为 calibration 的记录上冻结，不读取 formal、boundary 或 holdout 数据重新选参。
- 不自动重跑，不为了达到数量补跑，不删除任何原始数据。

## 7. 分析输出

两段可分别分析：

```bash
python3 scripts/research2_results.py analyze OUT/e2 --output OUT-e2-analysis
python3 scripts/research2_results.py analyze OUT/method --output OUT-method-analysis
```

运行正式分析前，建议先核对冻结 analyzer CLI：

```bash
python3 scripts/research2_results.py analyze --help
```

离线分析入口固定为新版 `scripts/research2_results.py`，不要直接用旧版 `research_results.py` 分析新清单。

## 8. 禁止事项

- 不在本文或脚本中发明 SD 写命令、分区操作或 bootrom 操作。
- 不自动重跑、不剔除、不 resume、不修改原始文件。
- 不把主机侧构建或测试当作板端证据。
- 不在没有实际运行证据时声明收益、保护匹配或显著性。
- 不覆盖已存在的输出目录。

## 9. 交付状态与板端待验收

- 交付包中的 `artifacts/role2.bit` 来自已验收的物理优化产物；其SHA256为 `31e18ee96e6edd47d444ed3cf9f4279cfa1ba494b53497d37183a3d7d0c33651`。原负时序BIT保留在Dell，不可代替交付包内BIT。全部实际哈希和版本见 `DELIVERY.json` 与 `SHA256SUMS`；优化来源见 `provenance/postroute/manifest.json`。Mac仍须执行每次启动后的kernel/CLI/caps检查。
- 板端结果未产生前，本文件不包含任何板端性能结论。
