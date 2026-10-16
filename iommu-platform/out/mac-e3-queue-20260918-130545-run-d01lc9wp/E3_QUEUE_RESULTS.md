# E3 v5 / Linux 00086 队列配对结果

日期：2026-09-18。范围：v5 `migration-4dma-e3-queue`，内核 `6.5.0-rc2-00086-gd2cd32d78918`。正式结果只含 10 对主实验；3 对短验收（含 Solo）只做执行/观察验收，不并入正式统计，也无 Solo 正式配对。

## 执行与数据正确性

- `short-assessment.json`：`passed=true`，`errors=[]`，`cases_seen=3`。
- `main-assessment.json`：`passed=true`，`errors=[]`，`cases_seen=10`。
- `run.json`：`status=completed_local_only`，`main_passed=true`，`main_pairs=10`，`total_rounds=26`，`unique_observation_lines=494`，`taint_after=0`，`causal_claim_supported=false`。原始 `short-records.json` / `main-records.json` 中全部 26 轮 `data_errors=0`、`quarantined=0`。
- 正式 A8 触发 16/16；A1 0/4。硬件 MiB/s 中位数 A8=109.29，A1=149.51（无 Solo 正式配对）。收尾 taint=0，printk 恢复 `7 4 1 7`，串口已关闭，无重编译、无补采/重跑。实验归档保留采集时状态，后续回传状态以本地 `upload-receipt.json` 和 Dell 最新交接为准。
- 版本：`run.json` Linux `6.5.0-rc2-00086-gd2cd32d78918`；协议 `E3 v5: 3 short + 10 formal pairs, no retry`（`run.json`）。`run.json` 同时记录 `bit.hash_verified=false`、`sd_readback_verified=false`。
- 状态以 `run.json` 为准；`main-summary.json` 顶层 `run` 已刷新为 `completed_local_only` 嵌套快照；`short-summary.json` 顶层 `run` 仍是较早快照，不作为最终状态。
- 主机侧新脚本：`linux_e3_queue.sh`、`collect_e3_queue.py`、`e3_queue_fields.py`、`assess_e3.py`；`host_field_tests_passed=35`（`run.json`）。

## 正式 10 对结果

表内“硬件 MiB/s”保留 2 位小数；“A miss/PTW”两侧同为 `a_miss=a_ptw`。“触发/中间”格式为“首轮触发/中间条数；次轮触发/中间条数”，`—` 表示 `target=null`。来源：`main-summary.json`，按 case 与 sequence 对应各行。

| 案例 | A 页 | D 首/次 | 硬件 MiB/s 首/次 | V miss 首/次 | A miss/PTW 首/次 | 触发/中间 |
|---:|---:|---:|---:|---:|---:|---:|
| 1 | 8 | 0 / 0 | 109.19 / 110.16 | 2 / 2 | 5142 / 5133 | 1/5 ; 1/1 |
| 2 | 8 | 0 / 128 | 109.18 / 110.09 | 2 / 2 | 5142 / 5133 | 1/5 ; 1/1 |
| 3 | 8 | 128 / 0 | 109.11 / 110.12 | 2 / 2 | 5142 / 5135 | 1/5 ; 1/1 |
| 4 | 1 | 0 / 128 | 149.11 / 149.58 | 0 / 0 | 0 / 0 | 0/— ; 0/— |
| 5 | 8 | 0 / 128 | 109.18 / 109.22 | 2 / 2 | 5142 / 5142 | 1/5 ; 1/5 |
| 6 | 8 | 0 / 0 | 109.30 / 110.08 | 2 / 2 | 5142 / 5133 | 1/5 ; 1/1 |
| 7 | 8 | 128 / 0 | 109.18 / 110.02 | 2 / 2 | 5142 / 5135 | 1/5 ; 1/1 |
| 8 | 8 | 0 / 128 | 109.27 / 110.05 | 2 / 2 | 5142 / 5135 | 1/5 ; 1/1 |
| 9 | 1 | 128 / 0 | 149.43 / 149.62 | 0 / 0 | 0 / 0 | 0/— ; 0/— |
| 10 | 8 | 128 / 0 | 109.21 / 110.09 | 2 / 2 | 5142 / 5133 | 1/5 ; 1/1 |

截断轮（9/16）：1 首、2 首、3 首、5 两轮、6 首、7 首、8 首、10 首；其余 7 轮为 triggered。

## 触发覆盖、前史与截断

- A8 `v_first_hit=0` 为 16/16（`run.json`；各 A8 轮次 `target.v_first_hit` 均为 0），说明“V 首次 miss”在正式 A8 全覆盖。
- 9/16 截断（`run.json`）：截断轮 `classification=truncated`，`truncated=true`，`insert_total` 大于实际 `inserted`，最多只保存前两条 intervening 记录。例：`main-summary.json` 中 case1 seq7 `insert_total=5, inserted=2, truncated=true`；case5 seq16 同为 5/2/true。不得把 `inserted` 当完整插入轨迹。
- 旧队头 A 证据 9/16：A8 触发轮中 9 轮 `trigger_head_is_v=0` 且 `head.is_v=false`，身份为 ID4/DID11，例如 `main-summary.json` 中 case1 seq7、case5 seq16、case7 seq19。这 9 个触发轮是旧队头 A 条件的直接覆盖证据，而非仅候选机会；但单独 9 条仍不证明 HOL。
- 前史/身份/插入差异：D0/D0 的 case1（seq7 vs seq8）和 case6（seq17 vs seq18）两轮自然不同。case1 的 `pre_hit/pre_miss/pre_tr/pre_comp`、旧队头、intervening、截断状态均不同（`main-summary.json` 中 case1 seq7/seq8）；case6 同样不同（`main-summary.json` 中 case6 seq17/seq18）。这属于 D0/D0 自然轮差证据，不允许把不同前史/身份的 D128−D0 当因果增量。
- 结论：数据正确性和观测通过；A8 触发覆盖与 V 首次 miss 可报告；因果未建立，尤其没有可配平的 D128−D0。没有对未匹配案例声称 D128 差值。

## 重新解析命令（只解析已有 UART，不触发硬件）

仅用以下命令重新解析已有 UART 原始日志；不触发硬件，不重跑 `idma_e3_pair`，也不刷新短验收原始快照。

```sh
python3 scripts/collect_e3_queue.py results/experiments/20260918-130545-run-d01lc9wp --phase main
```

## 证据索引

- [run.json](results/experiments/20260918-130545-run-d01lc9wp/run.json)
- [main-summary.json](results/experiments/20260918-130545-run-d01lc9wp/main-summary.json)
- [main-records.json](results/experiments/20260918-130545-run-d01lc9wp/main-records.json)
- [main-assessment.json](results/experiments/20260918-130545-run-d01lc9wp/main-assessment.json)
- [short-assessment.json](results/experiments/20260918-130545-run-d01lc9wp/short-assessment.json)
- [short-records.json](results/experiments/20260918-130545-run-d01lc9wp/short-records.json)
- 串口原始日志：[console.raw.log](results/serial/20260918-130407-console-49jwu1pv/console.raw.log)
- JTAG：[20260918-130413-program-vhjdluu7](results/jtag/20260918-130413-program-vhjdluu7)
- 协议与字段：[E3_QUEUE_PAIR.md](artifacts/software/e3-followup-20260918/delivery/E3_QUEUE_PAIR.md)

## 后续讨论（未实现）

有限请求序列或硬件共同启动/共同起点只作为未来控制选项讨论；本轮未实现，也未自动追加或补采。

归属：DeepSeek 整理诊断草稿与证据；Codex 复核原始数据、修正语义错误、补主机测试并执行上板与验证。
