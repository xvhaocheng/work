# FAILURE_HANDOFF — P4M formal60（run: `20260920-215406-run-cicggil4`）

- 结论：本轮在 runner 校验阶段停止，**未完成 60 trial**。
- 分工：故障原因由 Codex 核定，DeepSeek 起草本交接。
- 性质：只读交接，只记录已发生事实；本文不含任何修复动作，也不代为声明主控后续操作（串口归档、上传）的状态。

## 1. 原始错误整行（原样保留）

```
P4M: case=formal-b09-g2000 verify_failed=startup_line_count before=36 after=36 want=37
```

`run.json`：`status = stopped_runner_verification_exit5_evidence_collected`，`runner_exit = 5`，`host_exit = 5`。

## 2. 证据（Codex 直接检查 `board-evidence.tgz`，736449 bytes / 207 members）

1. `formal-b09-g2000.dmesg.before`：`seq 1..36`，36 条。
2. `formal-b09-g2000.dmesg.raw`：`seq 2..37`，36 条。
3. `scripts/linux_p4m_formal60.sh` 第 437..442 行先要求 `after_count = before_count + 1`。
4. 旧 `seq=1` 已被 dmesg 环形缓冲覆盖，新 `seq=37` 存在。
5. 校验函数（记录计数检查）原样继承 00089 runner；本轮 runner 相对 00089 新增了 formal60 矩阵，故不能笼统说“runner 未改动”，只能说该检查逻辑原样保留。
6. 25 个 CLI JSON 全部 `success=true`；最后一个 result `status=0`，V completed 4096，A completed 1369，双方 data_errors 0 / quarantined 0，measurement_complete 1，CLI exit 0，final_taint 0。
7. `cases_done=24`（gap0/gap1000/gap2000 各 8）；第 25 个 `formal-b09-g2000` 校验失败；`cases_not_run=35`。
8. Linux 仍在运行、未重试。
9. `startup_missing` 只表示缺少 runner 规范化标记，原始 `seq 37` 存在于 `.dmesg.raw`。
10. 上传状态**以 `upload-receipt.json` 与 `handoff-receipt.json` 为准**，本文不提前假定。

## 3. 判定（Codex 核定）

**不是“已发现 DMA 内容故障”。**

脚本“条数应递增（after = before + 1）”的假设与 dmesg 环形缓冲覆盖冲突：本次 before/after 均为 36 条，最早记录 `seq=1` 被覆盖、新增 `seq=37`，窗口前移而条数不变，计数断言失败。属采集脚本簿记/断言缺陷，无任何指向 DMA 数据错误的证据。

## 4. 统计口径

- 不得声称 60 完成：24/60，未运行 35。
- 不得删除或事后剔除任何样本，含第 25 个 `formal-b09-g2000`。
- `analysis/RESULTS.md` gap2000 的 `n=9` 已含第 25 个（case_end=0 者仅 8、有标记 1），**不可视作 9 个通过**。
- 通过数：8 + 8 + 8 = 24；第 25 个未通过（`no_case_end, startup_missing`）。不得把“8 合格 + 1 未通过”混为同一套统计。

## 5. 下一步（尚未授权）

可修脚本：按新 seq 定位启动标记（如认新出现的 `seq=37`），替代 `总条数 + 1`，无需新 BIT、无需重启 Linux。**修复与补跑均未授权，当前不执行。**
