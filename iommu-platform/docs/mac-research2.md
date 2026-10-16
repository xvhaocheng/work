# Mac 两 BIT 顺序执行工具 mac_research2

## 1. 目的与范围

本工具在 Mac 上顺序执行一份共用 Linux 的两 BIT research2 交付。它只负责：

- 只读 preflight：校验 manifest、授权包内文件 SHA256、openFPGALoader、pyserial、串口空闲；不执行板端命令，不查询板端 CLI，不做 --caps2。
- 顺序 flash image role=1 / role=2，每个 BIT 只调用一次 openFPGALoader，不自动重试。
- 每次 flash 前启动串口 reader，完整追加 UART 字节到 console.raw.log。
- 每个 case 通过一条板端 CLI + argv 执行，采集 record.json、stderr、exit、taint before/after、dmesg before/after、uname before/after。
- case 目录在板端打包 tar.gz，再 base64 回传；Mac 校验 SHA256 并安全解包。
- 第一段每个 case 的 raw 已回传、落盘并重新哈希验证后，才允许 sync 并 flash 第二 BIT。
- 技术失败首错停止；科学阴性由 analyzer 判断，runner 不判。
- 不自动重跑、不剔除、不 resume、不修改原始文件、不覆盖已存在的输出目录。

不在范围：

- 不裁剪/修改 SD 卡，不格式化，不写 bootrom。
- 不执行真实板操作作为本交付测试。
- 不实现 research2_plan / research2_results 内部逻辑。
- 不在 runner 里做任何收益/显著性判断。

## 2. 依赖与模块加载

Mac 需预装 Python 3.8+、pyserial、openFPGALoader。

`research2_plan.py` 与 `research2_results.py` 由 `importlib.util.spec_from_file_location`
按文件路径加载，脚本目录不会被加入 `sys.path`，因此本地同名文件不会遮蔽标准库模块
（历史故障：`scripts/platform.py` 遮蔽标准库 `platform`，导致 `import uuid` 失败）。
runner 自身也不再使用 `uuid`，随机标识统一使用 `secrets.token_hex`。

接口（与本交付一致）：

```python
from research2_plan import build_plan, select_configs, resolve_case
from research2_results import validate_record, metrics
```

`build_plan(stage)` 接受 `e2` / `method`；`resolve_case(case, selection)` 固定两个参数，
runner 不做任何 `TypeError` 回退，接口不符即技术失败。

## 3. Manifest 结构

schema 固定为 `research2-delivery-v1`。示例：

```json
{
  "schema": "research2-delivery-v1",
  "software": {
    "kernel_release": "<uname -r>",
    "cli_sha256": "<64 hex>",
    "board_cli_path": "/usr/bin/idma_research"
  },
  "images": [
    {"role": 1, "bit": "artifacts/role1.bit", "sha256": "<64 hex>"},
    {"role": 2, "bit": "artifacts/role2.bit", "sha256": "<64 hex>"}
  ],
  "scripts": {
    "plan": "scripts/research2_plan.py",
    "analyzer": "scripts/research2_results.py",
    "plan_sha256": "<64 hex>",
    "analyzer_sha256": "<64 hex>"
  }
}
```

路径必须相对 package root，拒绝绝对路径、`..`、反斜杠、symlink 逃逸。
`software.cli_sha256` 不用于本地 preflight 的板端查询；本地 preflight 只校验 manifest 中记录的值和包内本地文件。每次 flash 并启动后，`_collect_boot_info` 才用 `sha256sum <board_cli_path>` 核对板端 CLI，并运行 `--caps2`；两者都必须在任何 case 之前通过。

## 4. 使用

### 4.1 只读 preflight

```bash
python3 scripts/mac_research2.py --manifest DELIVERY.json --package-root . \
  --serial /dev/tty.usbserial-XXXX --preflight
```

preflight 不创建输出目录，不执行板端命令，不跑板端测试，也不查询板端 CLI/caps，只打印 JSON。

### 4.2 正式执行

```bash
python3 scripts/mac_research2.py --manifest DELIVERY.json --package-root . \
  --out /tmp/research2-run-01 --serial /dev/tty.usbserial-XXXX \
  --stage both --startup-timeout 300
```

`--out` 必须不存在。`--stage` 可选 `e2`、`method`、`both`。

## 5. 串口框架（marker framing）

每条命令都按如下模式发送（同一行，禁含换行）：

```sh
b0=__R2B; e0=__R2E; m1=<n1>; m2=<n2>; bm="${b0}_${m1}_${m2}__"; em="${e0}_${m1}_${m2}__"; printf '%s\n' "$bm"; <命令>; ec=$?; printf '%s%s\n' "$em" "$ec"
```

要点：

- marker 由分片拼接，命令原文里不存在 marker 字面量，终端回显、shell prompt、启动日志都不会被误当作 marker。
- 变量引用一律使用花括号（`${n1}` 而不是 `$n1_`），避免 `$n1_` 被解析成变量 `n1_`。
- begin marker 之前的一切（回显、prompt、启动噪声）都被丢弃；begin 与 end 之间是命令输出；end 之后是退出码。
- 读行时容忍 CRLF，容忍串口分块到达。
- 等待 root 只反复执行无害的 `true`，不会重跑任何实验。
- 首次框架前先盲发 `stty -echo`（失败可忽略），因为框架本身对回显免疫。
- `run_shell` 传入 `cwd` 时，整条命令被包成 `{ cd <cwd> && { <command>; }; }`；`cd` 失败会跳过所有用 `;` 分隔的子命令，不会在错误目录继续执行。

## 6. 每段流程

1. flash 前先 `sync`（非首段），sync 失败即停止，不会 flash 下一个 BIT。
2. flash role BIT。
3. 等待 root shell。
4. postboot 采集 `uname -r`、板端 CLI SHA256、`--caps2`、`/proc/sys/kernel/random/boot_id`；这些不是本地 preflight 的一部分。
5. 校验 kernel、CLI SHA256、caps schema/success/magic/version/role/features/observer_magic；
   并校验 boot_id 与上一段不同。
6. `build_plan(stage)`，写出 `<stage>/plan.json` 与 plan 哈希，之后才开始跑 case。
7. 逐 case 执行；method 段在第一个需要 selection 的 case 之前冻结 `selection.json`。
8. 全段 case 落盘后 `_verify_stage_saved` 重新校验所有 SHA，失败即保留第一段并阻止下一次 flash。

caps 数字既可能是 JSON 数字，也可能是 `0x` 十六进制字符串，解析器两种都接受，但不做任何
正则猜测：非 JSON、schema 不符、success 非 true、magic/version/role/features/observer
不匹配一律技术失败。role 1 期望 features=0b0111，role 2 期望 features=0b1111。

## 7. 板端 case 命令

```sh
{ cd <board_case_dir> && { <board_cli> <argv...> > record.json 2> stderr.txt; ec=$?; echo "$ec" > exit.txt; }; }
```

说明：runner 的 `run_shell(cwd=...)` 会把整条命令包成 `{ cd <cwd> && { ...; }; }`，所以 `cd` 失败时不会执行后面的 CLI、重定向或 `exit.txt` 写入。

argv 来自 plan，逐项 `shlex.quote`；case id 必须匹配 `^[a-z0-9][a-z0-9-]{0,127}$`。
每个 case 之前/之后都采集 taint、dmesg、uname，任何一条采集命令非零退出即技术失败。

## 8. 回传与安全解包

板端依次执行并逐条检查退出码：`sha256sum record.json|stderr.txt|exit.txt`、
`tar -czf case.tar.gz -C case_dir .`、`sha256sum case.tar.gz`、`base64 case.tar.gz`。

Mac 端严格按顺序：

1. base64 原始字节**先落盘** `archive.b64`（即使退出码非零也保留）。
2. base64 decode，校验 gzip 的 SHA256。
3. 落盘 `archive.tar.gz` 与 `archive.sha256`。
4. 安全解包到 `board/`，拒绝绝对路径、`..`、反斜杠/NUL、重复条目、symlink/hardlink、
   device/fifo、嵌套路径，以及不在 allowlist 中的条目
   （allowlist：record.json / stderr.txt / exit.txt / argv.txt / taint.* / dmesg.* / uname.*）。
   因此归档无法覆盖 SAVED、metadata.json、archive.*。
5. 校验 record.json 对板端 `sha256sum` 的结果，复制到 `runs/<id>/record.json`，写 `record.sha256`。

## 9. 失败语义

- 板端任一 setup/capture/hash/tar/base64 命令非零退出：技术失败。
- 归档 SHA 缺失或与解包结果不符：技术失败，且停止时保留已取得的原始字节。
- CLI exit != 0：**先完成归档/回传/落盘**，再写 metadata.json 并停止，不额外补实验。
- exit.txt 与框架退出码不一致：技术失败。
- taint after != 0：技术失败。
- `validate_record` 返回非空 errors：技术失败（success/validated/errors 由 analyzer 判定）。
- 串口 timeout / 读超时：保留日志，写 STOPPED.txt，退出。
- 失败时仍保留 `board-command.txt`、`archive.b64`（如有）、metadata.json。
- 不自动重跑、不剔除、不修改原始文件。

## 10. 输出目录

```text
OUT/
  run.json                 # run_id / stage / manifest 哈希 / kernel / CLI 哈希
  console.raw.log          # 全量 UART 原始字节（全局保留）
  console.events.log
  run.log
  STOPPED.txt              # 仅失败时
  COMPLETE.json            # 仅全部选定段成功时
  e2/
    plan.json
    plan.sha256.json
    boot.json
    flash.json
    runs/<case_id>/
      record.json  record.sha256  metadata.json
      archive.tar.gz  archive.sha256  archive.b64
      board-command.txt  board/...  metrics.json  SAVED
    stage-done.json
  method/
    plan.json  plan.sha256.json  boot.json  flash.json
    selection.json         # select_configs 的直接返回值，无包装
    selection.audit.json   # 仅校准输入文件哈希 + 冻结时间
    runs/<case_id>/...
    stage-done.json
```

`e2/` 与 `method/` 各自可独立作为 analyzer 输入：

```bash
python3 scripts/research2_results.py analyze OUT/e2 --output OUT-e2-analysis
python3 scripts/research2_results.py analyze OUT/method --output OUT-method-analysis
```

metadata.json 字段：argv、command、exit、framed_exit、taint_before/after、boot_id、
kernel_release、board_cli、board_cli_sha256、bit、bit_sha256、record_sha256、archive_sha256。

## 11. 参数冻结

- 只有 phase 为 `calibration` 的 plan case 会进入 `select_configs`；没有
  “用已完成 case 兜底”的分支，因此 formal/boundary/holdout 数据不会泄漏进选择。
- `selection.json` 是 `select_configs` 的直接返回值；输入文件哈希写在单独的
  `selection.audit.json`，不嵌套进 selection。
- 冻结发生在正式 case 之前；缺校准记录即停止，不会带着空 selection 继续。

## 12. 测试

```bash
python3 -m unittest tests/test_mac_research2.py
```

测试不接触真实板、不导入真实 pyserial：

- `ShellMockSerial` 用宿主 `/bin/sh` **真正执行**每条框架命令，验证 marker 语法、
  退出码、cwd 失败、CRLF、分块输出，以及“回显不被误当输出”。
- 假板端 CLI 输出真实 caps JSON（hex 数字形式）并按 BIT 产生新的 boot_id。
- 覆盖真实 `build_plan` 的 64 / 462 清单、`resolve_case` 严格双参接口、
  校准冻结只吃 calibration 记录、哈希缺失/不匹配阻止第二次 flash、
  技术失败仍先归档再停止。

## 13. 已知限制

- 未在真实 Mac + Genesys2 上执行；本文档与测试都是主机侧方案。
- 板端需要 posix 的 `sha256sum`、`tar`、`base64`、`dmesg`、`stty`。
- `--caps2` 需要板端 CLI 支持；caps 解析只接受严格 JSON。
- 未实现最终 halt/reboot；JTAG flash 本身 reset FPGA，Linux 状态不维持。
- 板端 `/tmp` 目录默认保留以便取证，不会自动清理。
