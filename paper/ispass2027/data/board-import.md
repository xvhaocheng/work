# 新板测导入与正式 100 点核账

入口是 [import_board_runs.py](import_board_runs.py)。它只读取显式传入的采集目录及必要身份/软件证据，生成本目录下的新快照包；不打开 UART、不运行采集器、不重启或加载板卡。正式增量清单已安装真实 G1 与统一100点ABA结果，见[100点简表](baseline100-summary.md)；隔离 fixture 保持独立，没有虚构测量结果。

## 原始输入

每个 `--run` 目录应有 `collect.py` 的 `manifest.json` 和 `records.json`，完成时另有 `review.json`；加载身份从本目录 `load.json` 或 manifest 的 `load_manifest` 取得。可同时输入多个 `--run`，顺序不影响生成字节。

`manifest.json` 必须是六个受支持 phase 之一：`g1`、`ds50-first`、`ds0-middle`、`ds50-last`、`ds50-all`、`ds0-all`。失败时可以没有 `records.json/review.json`；已有样本、manifest 错误和 `uart/command-*.json` 原始命令仍保留。缺失文件不会补造。若只有一个未写入 records 的 UART 命令，依据采集器遇到首个失败即停的控制流关联到下一计划点；多个无法关联的命令保留为未知顺序，不能冒认属于某次完成试验。

恢复运行使用 collector 的 `recovered_from`、`recovered_manifest_sha256`、`recovered_cases` 和 `recovered_raw_commands`。导入器递归核验旧 manifest 的原始哈希，保留失败状态；每项 `raw_command_path` 精确绑定原始命令 JSON、完整 CLI 对象、PRE_SEQ 与 boot。旧命令和新目录命令去重读取，恢复样本只计一次，不把旧解析失败再算成额外硬件试次。JSON 前同一行的 printk 前缀通过 `raw_decode` 处理；原始命令保持不改。恢复来源属于测量证据，不能冒充软件证据。

`--software-evidence` 可重复指定 software-build、构建 manifest、SD 写入/回读或已有软件身份链的 JSON。导入器记录原文件和哈希；不根据相同 kernel release 推断 payload 相同，也不把主机 payload 哈希当成已实际启动的 SD 字节认证。load 的 program-file 核验与 configuration readback 标志分别原样保留。

load 已有的 `resumed_from/original_receipt_sha256`、`authentication_receipt`、`sd_evidence/raw_receipt` 链会保留在身份来源中，并把原 program 日志/脚本纳入哈希；只引用现有回执，不读取系统凭据，不把认证中断改写成新的硬件失败或新的加载。

## 先准备，再安装

下面目录名是调用示例，应以 root 提供的实际完成目录替换。输出须在 `paper/ispass2027/data/` 下，首次准备要求新目录。

```sh
python3 paper/ispass2027/data/import_board_runs.py \
  --run paper/ispass2027/board/runs/g1-20261017-01 \
  --output paper/ispass2027/data/board_imports/g1-20261017-01 \
  --name g1-20261017-01
```

当前已启动的 ABA 正式 100 点同时传入三个采集段，沿用其启动时的 25/50/25 计划；不为调整后续流程重做。G1 可以单独导入，也可同时传入但始终保持独立系列：

```sh
python3 paper/ispass2027/data/import_board_runs.py \
  --run paper/ispass2027/board/runs/baseline-ds50-first-01 \
  --run paper/ispass2027/board/runs/baseline-ds0-middle-01 \
  --run paper/ispass2027/board/runs/baseline-ds50-last-01 \
  --software-evidence paper/ispass2027/board/software-build.json \
  --software-evidence paper/ispass2027/board/manifests/software-host-verification.json \
  --output paper/ispass2027/data/board_imports/baseline100-01 \
  --name baseline100-01
```

今后的默认 runner 使用 AB 两阶段：同一软件 payload，先完成 `ds50-all` 的 50 点，再完成 `ds0-all` 的 50 点，每套 BIT 只加载一次，不再回切。导入时改为两个完整目录：

```sh
python3 paper/ispass2027/data/import_board_runs.py \
  --run paper/ispass2027/board/runs/baseline100-next/ds50-all \
  --run paper/ispass2027/board/runs/baseline100-next/ds0-all \
  --output paper/ispass2027/data/board_imports/baseline100-next \
  --name baseline100-next
```

按实际需要附加既有 `--software-evidence`，不因使用新布局增加 SD 回读或额外软件验收。

`run_baseline100.py` 可选传入 `--initial-load-manifest <真实DS50加载目录/load.json>`。仅在确认板端仍处于该回执对应的 DS50 启动时使用：runner 在主机核对回执的 `status=boot_identity_verified`、kernel release 和 package DS50 BIT 哈希，匹配后直接采集 `ds50-all`，只在切 `ds0-all` 时加载一次。不增加 UART 身份探测；collector 在实际采集时仍核对原回执 boot ID。campaign 的 `initial_load_manifest` 和首段 `load.action=reuse_existing_receipt` 指向原文件，不生成替代加载回执。未传此参数时仍正常加载两次；回执的状态、内核或 BIT 不匹配会在启动任何进程前拒绝。

快照包包含：

| 文件 | 内容 |
| --- | --- |
| `summary.json` | 原始来源的逐试次标量、失败证据指针、按采集段/boot/条件分层的统计、显式 DS50 合并及 100 点完整性 |
| `incremental_sources.json` | 可供主提取器使用的 scalar locator 行；没有手填 `value`，另含全部 `source_files` 的 SHA256 |
| `manifest.json` | 输入/输出哈希、脚本哈希、正式计数与 G1 结果状态 |

使用完全相同的参数追加 `--check`，要求每个生成文件字节一致；不会写文件。检查包内数据后，使用相同参数追加 `--install`，先重新核对包，再合并到正式 `data/incremental_sources.json`。已有相同导入可重复安装；相同 `row_id` 内容不同或相同来源哈希不同会拒绝，不静默替换旧快照。测试可用 `--incremental <data下的隔离文件>`，fixture 不能安装到正式清单。

安装后运行：

```sh
python3 paper/ispass2027/data/extract_results.py
python3 paper/ispass2027/data/extract_results.py --check
```

正式增量清单支持可选 `source_files=[{"raw_path":"…","sha256":"…"}]`。主提取器在读取标量之前验证这些原始输入和派生 summary 的哈希，并在构建结束再次核对全部来源，避免只有派生表还在而原始链已经变化的情况。同次重建的 JSON 只读取一次，以免长表每一行重复读取大 summary。

## 100 点、重复次数与 boot

正式计划支持两种完整布局：当前 ABA 为 DS50-first 25 点、DS0-middle 50 点、DS50-last 25 点，分别为五种条件各 5、10、5 次；今后默认 AB 为 DS50-all 50 点、DS0-all 50 点，两段均为五种条件各 10 次。每段沿用按 block 轮转条件的顺序。每 phase 保留实际单次启动身份。G1 为三种条件各 3 次，另用 `board_g1` 系列；正式数据用 `board_baseline100`，不把 9 点 G1 计入正式 100 点。

所有运行首先按 run、phase、实际 boot、条件分层。DS50-first 与 DS50-last 各只出现一个命名 run，且报告的 BIT 哈希、kernel release 一致、没有重复计划点时，才另生成 `pooled_first_and_last_with_explicit_boots`。它显式列出两个来源 phase 和实际 boot IDs；正常完整组的 `n=10` 是 10 次试验，`observed_boot_count=2` 是两个实际 boot，绝不称为 10 次独立重启。软件 payload 是否等价仍保留未推断状态和证据指针。

AB 的 DS50-all、DS0-all 各自已包含每条件 10 次，始终使用 `within_one_run_phase_and_boot`，不标为 pooled；正常完整布局记录两个实际 boot。出现任意 `*-all` phase 时按 AB 判断，缺少另一半不能通过；把 AB 和 ABA 采集段混在一起也不能通过。

`n` 始终是实际用于该标量统计的重复试次数。若某指标缺测，只对有数值的重复计算统计，并同时列 `n_trials`、缺测/不适用数。全缺测的统计为 JSON `null`，该指标有值重复数 `n=0`；导入主长表后为 `value=unknown`，不是零时间或零 miss。失败试次的 sample 行 `n=1` 表示一次有原始证据的尝试，不参与成功试次统计。

`formal100.complete` 只在选定布局的所有 phase 各有唯一完整命名 run、100 个计划点齐全且资格与来源核查通过时为真。重复的 `(phase, block, condition)` 原样保留并阻止完整性通过，不将重跑默认为新的合法重复，也不自动选择哪次替代。导入程序执行成功仅表示格式/来源处理成功，不能代替此标志或原板测验收。

## 标量、窗口和失败

- 硬件时间、吞吐和重叠从原始 `observer.time.cycles`、完成字节及占用重新计算，再与 collector 保存的 metrics 核对；不把 review 的汇总数当数值来源。
- 保存 V/A 完成、提交与字节，HPM miss/S1/S2，硬件窗口 issued/done，V/A AR/AW/RLAST/B、等待与占用，shared 请求以及可用的路径均值/最大值。请求等待计数单位是 cycles；start/end 是原始高 16 位读、低 16 位写的 packed outstanding 字段，不能当普通请求数量。
- 软件总时间、非原子 HPM 快照、硬件 V 窗口、DONE 到软件读取、双方包含 V 后排空的整段完成各有独立 window 标签。A 全部完成量不能除以 V 窗口当作同期吞吐。
- 原始 record 校验失败时保留具体字段、原值及错误；仍可读出的数值标为 `failed_observation`。未被 collector 写入 records 的原始失败命令保留路径和退出/超时状态，性能标量保持 unknown，不能从缺失补成 0。
- G1 性能门槛失败时，完整的 9 次原始测量仍保留，run 的 failed 状态与 `phase_complete` 传入 sample/汇总条件，不把格式正确当成通过 G1。

所有 sample/汇总有稳定 row_id。直接存在的数值指向原始 records 的精确 JSON pointer；派生或缺失值指向包内 scalar，同时列原始 operand 路径、pointer、存在性与运算。全部必要原始 JSON 都进入哈希链。

## 已运行的验证

本次布局扩展只运行 `python3 paper/ispass2027/data/test_import_board_runs.py --phase-layout-only`，结果在 [phase-layout-validation.json](phase-layout-validation.json)。隔离 fixture 检查 AB 完整接受、任一半缺失拒绝、重复／混合布局拒绝、单 boot 的 n=10；同时确认 ABA 仍完整接受并保留双 boot 合并。采集器和 runner 的离线调用以本地替身阻断全部 UART 与进程执行，核对旧四个 phase 的完整命令／轮转／DS／review 与启动时保存副本一致，以及新两阶段各 50 点、两次 BIT 加载。没有重放无关历史解析或触板。

随后加载复用优化只运行 `python3 paper/ispass2027/data/test_import_board_runs.py --runner-reuse-only`，8 项检查见 [runner-reuse-validation.json](runner-reuse-validation.json)：无参数保持两次加载；复用时首段沿用原回执、仅 DS0 新加载；campaign 如实记录来源且不写新加载回执；错误 BIT、内核或状态均在任何进程执行前拒绝。全部子进程以替身阻断，没有新建实际 100 轮采集。

运行 `python3 paper/ispass2027/data/test_import_board_runs.py`，结果在 [importer-validation.json](importer-validation.json)。fixture 由历史 raw 字段构造并明确标为 synthetic，全部在隔离临时目录生成和清理；正式增量清单前后哈希相同。

覆盖：25+50+25 与独立 G1；两个 DS50 boot 的实际重复次数；原始 cycles 的中位数和 locator 全量接入主提取器；输入顺序与字节重建；CLI `--check` 和审核后 `--install`；安装幂等；原始字节篡改拒绝；原始失败 UART 与缺失 review 保留；重复点及不同 BIT 阻止合并；缺失计数不补 0；review 数值被改动时仍从原始 cycles 计算并暴露不一致；软件证据仅保留哈希指针。未运行任何新板测。
