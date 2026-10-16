# method88 主机运行器（Mac）与离线复核说明

本文件说明 `scripts/mac_method88.py`（Mac 主机，同一 role2 BIT 的两次 fresh boot）和
`scripts/research_method88.py`（冻结计划 + 离线分析）的用法。

本交付的边界：

- **不改** Linux、驱动、RTL、BIT，**不重写** SD 卡，**不编译** 任何 Linux 或
  BIT 产物；
- 只在板上已经是当前基线内核 `6.5.0-rc2-00093-gc0f5e13ea81f` 时运行；
- 首错停；不自动重跑、不续跑、不覆盖已有输出目录；
- 同一个 role2 BIT 在同一输出目录里连续启动两次（每次 44 项），共 88 项。

## 1. 交付包（method88-delivery-v1）

清单 `DELIVERY.json` 使用新 schema `method88-delivery-v1`，与旧的
`research2-delivery-v1` 不同，**只要求 role 2**：

```json
{
  "schema": "method88-delivery-v1",
  "images": [
    {"role": 2, "bit": "artifacts/role2.bit",
     "sha256": "31e18ee96e6edd47d444ed3cf9f4279cfa1ba494b53497d37183a3d7d0c33651"}
  ],
  "software": {
    "kernel_release": "6.5.0-rc2-00093-gc0f5e13ea81f",
    "cli_sha256": "3af284790ba91c9536a23d754f9e22de855b230d071322b84798b92c0043a0ef",
    "board_cli_path": "/usr/bin/idma_research"
  },
  "frozen": {"root": "frozen"},
  "files": {
    "scripts/mac_research2.py": "<64hex>",
    "scripts/research2_plan.py": "<64hex>",
    "scripts/research2_results.py": "<64hex>",
    "scripts/research_results.py": "<64hex>",
    "scripts/research_method88.py": "<64hex>",
    "scripts/mac_method88.py": "<64hex>",
    "frozen/plan.json": "<64hex>",
    "frozen/plan.sha256": "<64hex>",
    "frozen/selection.json": "<64hex>",
    "frozen/selection.sha256": "<64hex>",
    "artifacts/role2.bit": "<64hex>",
    "docs/mac-method88.md": "<64hex>"
  }
}
```

校验器（`scripts/mac_method88.py`）：

- `software` 三个字段必须**逐字**等于当前基线，否则拒绝；
- `images` 必须**恰好一项且 role=2**；
- `images[0].bit` 必须精确为 `artifacts/role2.bit`，`images[0].sha256` 必须精确为基线常量 `ROLE2_BIT_SHA256`，且 `files["artifacts/role2.bit"]` 必须等于该 `image.sha256`；
- `frozen.root` 必须精确为 `frozen`（不允许改用未哈希的其他目录）；
- `files` 必须是上面这份清单的**全集**，每一项都是小写 64 位十六进制，
  路径经 `safe_package_relpath` / `resolve_package_path`（禁止绝对路径、
  `..`、反斜杠、越界软链）；
- `frozen.root` 必须是包内目录；
- 冻结计划用 `selection.json` + `plan.json` 里的 `seed` 经
  `research_method88.build_plan` **重生成**，`plan_sha256` 也用
  `research_method88.plan_digest` 重算，**整份计划必须完全一致**；
- `selection_sha256`（规范哈希）、`selection.sha256` 里的原始文件哈希与规范
  哈希都要对得上。

本步**不重做标定**：selection 直接来自既有冻结文档，`freeze` 原样拷贝源文件。

## 2. 解包、工作目录与预检（只读）

归档 `method88-20261004.tar.gz` 的外层目录是 `method88-20261004/`，其中包含
`README.md`、`SHA256SUMS` 和 `delivery/`；`delivery/` 内包含 `DELIVERY.json`、
`scripts/`、`frozen/`、`artifacts/`、`docs/`。运行时命令必须在
`method88-20261004/delivery` 下执行：

```sh
tar -xzf method88-20261004.tar.gz
cd method88-20261004/delivery
```

预检（只读）：

```sh
python3 scripts/mac_method88.py \
  --manifest DELIVERY.json \
  --package-root . \
  --serial /dev/tty.usbserial-AV0KRCHT \
  --preflight
```

预检**只**做：清单与包内所有文件/镜像哈希校验、冻结计划重生成比对、
`openFPGALoader` 与 `pyserial` 是否存在、串口的打开与关闭。

预检**不**创建 `--out` 目录，**不**运行板端 CLI，**不**发 `--caps2`，**不**跑任何
用例。若某串口已经被占用，或任一哈希不符，预检直接失败。

## 3. 正式运行

仍须在 `method88-20261004/delivery` 下执行（若尚未解包，先按第 2 节执行
`tar -xzf method88-20261004.tar.gz` 和 `cd method88-20261004/delivery`）。
`--out` 必须是不存在的新目录，且必须位于不可变包之外；从 `delivery/` 起算的
`../../results/method88-20261004-01` 即包外示例（必须尚未存在）：

```sh
python3 scripts/mac_method88.py \
  --manifest DELIVERY.json \
  --package-root . \
  --serial /dev/tty.usbserial-AV0KRCHT \
  --out ../../results/method88-20261004-01
```

可选旋钮（默认沿用旧 runner）：`--startup-timeout`、`--command-timeout`、
`--case-timeout`、`--transfer-timeout`、`--jtag-freq`、`--manifest-sha256`。

**没有** `--stage`、**没有** `--resume`、**没有** selection 调参入口。`--out`
已存在时直接拒绝。

运行流程：

1. 校验包与冻结计划/selection；
2. `--out` 新建，`console.raw.log` / `console.events.log` 在同一目录；
3. 启动串口读线程（先于任何 flash），然后第 1 次 flash 同一个 role2 BIT；
4. 等待全新启动 + root shell，采集 boot 信息（`uname -r`、CLI 哈希、
   `--caps2` role2 features `0xf`、`/proc/sys/kernel/random/boot_id`）；
5. 跑冻结计划里 `boot=1` 的 44 项；
6. `_verify_stage_saved` 通过后 `sync`，**再**第 2 次 flash 同一个 BIT；
7. 跑 `boot=2` 的 44 项，要求 boot_id 与第 1 次不同；
8. 写 `COMPLETE.json`。

输出目录布局：

```
<out>/console.raw.log
<out>/console.events.log
<out>/run.json
<out>/COMPLETE.json
<out>/STOPPED.txt            # 仅技术性失败时
<out>/method/plan.json       # 冻结计划原样拷贝
<out>/method/selection.json  # 冻结 selection 原样拷贝
<out>/method/plan.sha256
<out>/method/selection.sha256
<out>/method/boots/1.json     <out>/method/boots/1.flash.json
<out>/method/boots/2.json     <out>/method/boots/2.flash.json
<out>/method/runs/<case-id>/  # 88 个：record/metadata/metrics/archive/SAVED
```

每个 `metadata.json` 保留旧字段（`exit`、`framed_exit`、`taint_before`、
`taint_after`、`boot_id`、`kernel_release`、`board_cli`、`board_cli_sha256`、
`bit`、`bit_sha256`、`role`），并额外写入 `boot`（1 或 2）。`uname` 一律来自
板端 `uname -r`，不臆造。每个用例开始前 `taint.before` 必须为 0，否则该用例
失败并整轮停止。

## 4. 归档（BusyBox 兼容）

主板 BusyBox tar 不支持原 runner 的 `tar -czf`；`Method88Console` 只拦截这一条
命令，精确解析 6 段后替换为：

```sh
test ! -e <target>.plain.tar && test ! -e <target> \
  && tar -cf <target>.plain.tar -C <source> . \
  && gzip -c <target>.plain.tar > <target>
```

不使用 `-z`，不覆盖中间文件或最终文件；base64、sha256 校验和归档白名单逻辑
沿用旧 runner 不变。

## 5. 离线分析（可选，可在 Mac 采集回传后或 Dell 离线执行）

若仍在 `method88-20261004/delivery`，直接用同一次正式运行的输出分析到新的 `-analysis` 目录；不要重新 freeze（计划/selection 已在包内冻结）：

```sh
python3 scripts/research_method88.py analyze ../../results/method88-20261004-01/method --output ../../results/method88-20261004-01-analysis
```

离线分析不依赖 Mac 端先执行，也不影响 Mac 继续采集/完整回传；若不在 `delivery/`，先回到包内 `delivery/` 再执行同一命令。

分析只读 `RUN/method`：重生成计划并逐字段比对 `plan.json`，校验每个 run 的
record/metadata，逐启动、逐方向报告 p5 与 fixedgap/p1..p6 的配对比较，并输出
`results.json` 与中文 `report.zh.txt`。两个启动、两个方向分别报告，不合并、
不做显著性判断。

本文件里的 mock 测试只覆盖 IPC/串口框架流程（`FakeResultsModule` 不检查原始
record 内容）；原始 record 的严格校验由 `tests/test_research_method88.py`
覆盖。

## 6. 依赖

Mac 主机：Python 3.8+、`pyserial`（`serial`）、`openFPGALoader`、空闲串口。
不需要在 Mac 上安装 GNU coreutils；`sha256sum`/`base64`/`gzip`/`tar` 都在板端
BusyBox 里运行，测试里的同名替换只是主机测试替身，不是 Mac 的真实依赖。板端：
`/bin/sh`、`uname`、`sha256sum`、`tar -cf`、`gzip -c`、`base64`、`dmesg`、
`stty`、`cat`、`sync`、`/proc/sys/kernel/random/boot_id`、
`/usr/bin/idma_research`（支持 `--caps2` 且 role 2 features 为 `0xf`）。
