# P4M pilot21 静态复核结果（DS 未执行板卡命令）

## 状态一句话

21/21 功能层通过，上传记录与去重观测一致；但 **legacy B8 vs P4M A8gap0 的 gap0 等价性未解决**，尤其 block3 P4M A8gap0 seq24 是“更快但不同初始相位”的样本。Gap500 是真实非单调反例，必须保留。n=3 不能建立等价性、正式性能或新颖性。下一步不应冻结正式矩阵，先做最小有界判别动作。

## 1. 功能通过边界

- 21/21：`success=true`、`result.status=0`、V issued/done=4096、data_errors/quarantined=0、final taint=0、collection_failed=0。
- 证据：`records.json` 全部 21 条；`observations-unique.log:631-634`；`run.json:22-33`。
- 限制：这不是独立主机验收，不是 BIT/SD hash 验证。`run.json:4-17` 显示 BIT sha256=null、hash_verified=false、software_sha256=null；`run.json:33` 明确 independent_host_acceptance 未运行。

## 2. 关键配对：B8 legacy batch vs P4M A8gap0

| block | B8 trial/seq | B8 cycles | A8g0 trial/seq | A8g0 cycles | ratio A8g0/B8 | diff cycles | B8 V miss/PTW | A8g0 V miss/PTW | B8 A_whole miss/completed | A8g0 A_whole miss/completed | A8g0 window jobs |
|---|---:|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|
| B1 | t03/seq7 | 7745642 | t04/seq8 | 7751680 | 1.00078 | +6038 | 0/0 | 0/0 | 6160/5507 | 6160/5479 | 4096 |
| B2 | t13/seq17 | 7744503 | t12/seq16 | 7748692 | 1.00054 | +4189 | 0/0 | 0/0 | 6160/5517 | 6160/4168 | 4096 |
| B3 | t19/seq23 | 7745970 | t20/seq24 | 7248503 | 0.93578 | -497467 | 0/0 | 2/2 | 6160/4142 | 5141/5998 | 4096 |

证据：B8 `records.json:435-564,2574-2703,3813-3942`；A8g0 `records.json:566-790,2471-2569,3944-4168`；observer seq `observations-unique.log:140,166,374,401,558,584`。

## 3. 第三个 gap0 是更快，不是单纯更慢

seq24 的 V cycles=7248503，低于 seq8/seq16 的 7751680/7748692；V derived 约 110.368 MiB/s，高于约 103.2 MiB/s。但它不是同一初始相位：

| 样本 | opening A ID | first_done ID | last_done ID | closing ID | window jobs | whole A completed | whole excess | V miss/PTW | A whole miss/PTW | done_to_read |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| seq8 | 4 | 4 | 4099 | 4100 | 4096 | 5479 | 1383 | 0/0 | 6160/6160 | 932416 |
| seq16 | 4 | 4 | 4099 | 4100 | 4096 | 4168 | 72 | 0/0 | 6160/6160 | 26767 |
| seq24 | 1448 | 1448 | 5543 | 5544 | 4096 | 5998 | 1902 | 2/2 | 5141/5141 | 295647 |

这说明：seq24 在窗口打开时 A 已经推进到 ID 1448，而 seq8/seq16 是 ID 4；whole A completed 与 miss 也不同。`whole-run A/HPM` 不等于窗口 A 计数。

## 4. 窗口 stage waits 变化（描述性）

seq8/seq16/seq24 的 `idma_path_obs` 在 A/V 各 stage sum 上有变化。例如：

| 样本 | V r stage1 | V r stage3 | V w stage1 | V w stage3 | A r stage1 | A r stage3 | A w stage1 | A w stage3 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| seq8 | 4931732 | 9024047 | 2204496 | 11931927 | 8816822 | 5139745 | 6090442 | 8047602 |
| seq16 | 4928128 | 9021527 | 2204780 | 11925572 | 8812866 | 5138387 | 6090384 | 8042296 |
| seq24 | 3884210 | 9067041 | 2857198 | 10274536 | 9115746 | 3833813 | 2098704 | 11031511 |

证据：`observations-unique.log:169-184,377-392,587-602`。这支持“相位/占用改变”的假设，但不能无 RTL 做因果诊断。

## 5. Gap500 非单调，保留三个样本

| 样本 | cycles | window jobs | V miss/PTW | A whole miss/PTW | A whole completed |
|---|---:|---:|---:|---:|---:|
| seq9 t05 | 8157735 | 4095 | 0/0 | 7184/7184 | 4120 |
| seq15 t11 | 8156880 | 4095 | 0/0 | 7184/7184 | 4153 |
| seq21 t17 | 8157023 | 4095 | 0/0 | 7184/7184 | 4151 |

同 block 比较：gap500 比 gap0 慢约 5.24%、5.27%、12.53%（第三受 seq24 异常 gap0 影响）；A window jobs 少 1 个。只描述观察范围；不证明所有工作负载都不利，也不黑名单该点。

## 6. 每条件 V cycles 与同 block Solo 归一化

| 条件 | cycles r1/r2/r3 | ratio vs same-block Solo |
|---|---:|---:|
| S | 2822099 / 2821580 / 2821786 | 1.000 |
| A1g0 | 5368703 / 5356688 / 5346832 | 1.90238 / 1.89847 / 1.89484 |
| B8 | 7745642 / 7744503 / 7745970 | 2.74464 / 2.74474 / 2.74506 |
| A8g0 | 7751680 / 7748692 / 7248503 | 2.74678 / 2.74622 / 2.56876 |
| A8g500 | 8157735 / 8156880 / 8157023 | 2.89066 / 2.89089 / 2.89073 |
| A8g1000 | 5497730 / 5502872 / 5506360 | 1.94810 / 1.95028 / 1.95137 |
| A8g2000 | 4667603 / 4668975 / 4667331 | 1.65395 / 1.65474 / 1.65403 |

说明：这是逐 block 配对比值，不是 ratio of medians。

## 7. gap1000/gap2000 的 V 时间减少与 A 窗口吞吐代价

使用同 block gap0 基线：

| gap | block | V time reduction | A window jobs gap0→gap | A window throughput cost |
|---|---:|---:|---:|---:|
| 1000 | B1 | 29.08% | 4096→2048 | 29.50% |
| 1000 | B2 | 28.98% | 4096→2048 | 29.59% |
| 1000 | B3 | 24.03% | 4096→2048 | 34.18% |
| 2000 | B1 | 39.79% | 4096→1365 | 44.66% |
| 2000 | B2 | 39.75% | 4096→1365 | 44.69% |
| 2000 | B3 | 35.61% | 4096→1364 | 48.28% |

注意：V time reduction ≠ V throughput increase；A throughput cost 使用同 block 基线，不能混用不同 block 或 ratio of medians。B3 gap0 异常会影响其 paired 值。

## 8. 基线与停止判断

- B8 无 P4M window bank，A window 进度 N/A；不能把 B8 whole A 统计当 window 统计。
- B1/B2 B8 vs P4M gap0 很接近；B3 出现 phase 差异。因此 P4M-2 的 gap0 显式对照仍未解决。
- 所有上传检查均为已有报告/记录的重分析，不是新的独立回归或 UART 全量验证。

## 9. 下一步最小有界判别动作（建议，不自动执行）

在现有 BIT/kernel 下，不重烧、不重编译、不新增正式矩阵，只做 balanced ABBA 的 B8 vs P4M A8gap0 小配对：

- 条件仅两个：legacy B8、P4M A8g0；参数保持 4096 transfers、256 warmup、4KiB、no gap。
- 建议顺序：B8, A8g0, A8g0, B8，重复两轮 = 8 trials；或 4 对 ABBA。
- 必看字段：opening_inflight_id、first/last done ID、carry、window cycles、A window jobs、whole A miss/completed、done_to_read。
- 目的：判别 B3 gap0 差异是启动/初始相位次序效应，还是 P4M vs legacy 的持续差异。
- 若 opening ID 仍系统性跳变，先不要冻结正式矩阵；若差异消失，再考虑正式预注册。

## 10. 缺失证据与 RTL 边界

需要 parent/Codex 读 RTL 与驱动才能做因果诊断：observer open/close、P4M bank 语义、batch gap backend、A/V launch order、warmup/reset/order。现有 `idma.c` 仅能说明软件顺序和校验逻辑：`idma_batch_window` A 在 V 前提交（`idma.c:658-667`），`idma_run_session` warmup/批量配置/gap/arm/window（`idma.c:2114-2184`），`idma_p4m_finish` 的 carry/ID 校验（`idma.c:1142-1248`）。

缺失实际证据：完整 `console.raw.log`（本批已排除）、RTL、BIT/SD hash、独立主机验收、HPM 校准、seq24 与 t10 drain 异常的原始时间戳/上下文。