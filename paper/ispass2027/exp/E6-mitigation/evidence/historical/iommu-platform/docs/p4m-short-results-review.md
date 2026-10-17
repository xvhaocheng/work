# P4M 短验收结果复核（Solo / A1 gap0 / A8 gap0 / A8 gap1000）

**结论：四例短验收在功能层面通过；本报告是短验收复核，不是正式收益结论。** 实际执行：DeepSeek 起草复核/文档，Codex 审阅原始记录、UART、driver/observer 并做科学决策与实际复核。复核脚本正常退出与自检：正常复核 exit 0，自测 PASS，原始输入哈希未变。

## 关键表（n=1，描述性，不代表显著性）

| 条件 | V 周期 | V 硬件耗时（ms） | 相对 Solo | 窗口内 A 完成数 | A 完成速率（MiB/s） |
|---|---:|---:|---:|---:|---:|
| solo | 44279 | 0.885580 | 1.0000× | 0 | 0.000 |
| a1-gap0 | 83938 | 1.678760 | 1.8957× | 64 | 148.919 |
| a8-gap0 | 120679 | 2.413580 | 2.7254× | 64 | 103.581 |
| a8-gap1000 | 85276 | 1.705520 | 1.9259× | 32 | 73.291 |

时间换算按 50 MHz；速率统计完整 copy 的完成字节数，不是 AXI 总线吞吐。

| 条件 | 软件耗时（ms） | 完成到 CPU 读取（ms） | 整轮 A 完成数 | 开窗前开始且窗内完成的 A |
|---|---:|---:|---:|---:|
| solo | 1.959840 | 1.000640 | — | 0 |
| a1-gap0 | 69.479280 | 67.128260 | 4961 | 1 |
| a8-gap0 | 22.188640 | 19.367660 | 1496 | 1 |
| a8-gap1000 | 2.835120 | 0.983640 | 61 | 1 |

A8 gap1000 相对 A8 gap0：V 时间 -29.33650428%，A 完成速率 -29.24210798%；A 完成数 32 vs 64，但窗口更短，因此速率不是简单减半。gap1000 的 V 变快/A 完成速率下降是预期取舍，不是本轮异常。

## 已验证事实

- 四例短验收在板上功能通过：records/raw UART/taint 一致；板卡 exit 0、final taint=0、collection_failed=0。
- P4M-1 与 P4M-3 短验收完成；P4M-2 的 legacy `--batch` 与 `--p4m` gap0 显式对照本轮未做（本轮全为 `--p4m`）；P4M-4 预试为下一步。
- 不证明正式收益；所有 n=1。
- 窗口守恒通过；carry=1 在每个 A 例出现，占窗口内完成数 1.56%、1.56%、3.125%，是边界计数效应。
- HPM 全软件区间：V miss 0；A1 miss 0；A8 gap0 miss 112；A8 gap1000 miss 72；不能当作同一硬件窗口归因。
- `raw_observation[34]` 是 done_to_read；[observer](../cva6/corev_apu/fpga/src/dma_observer.sv#L336) 在 BATCH_READ 递增直到匹配 poll；[driver](../linux/drivers/misc/idma.c#L902) 记录该值。UART 与 records 的 done_to_read 一致：50032 / 3356413 / 968383 / 49182 cycles。
- UART 中 hrtimer 16.2008 ms 警告是辅助现象；不能证明全部结果由它导致。
- 硬件功能验收与 artifact identity 分开：SD/readback/hash 未验证只保留为 provenance 警告；raw BOOT Linux 00088 行成功，P4M magic 与 success 支持功能验收，不因陈旧 `hardware_boot_verified=false` 强制重烧/重跑。

## 证明边界

- n=1，不能声称显著性、等价性或正式收益。
- P4M-2 同 BIT `--batch` vs `--p4m` gap0 未做显式比较。
- 地址映射/容量仍是未证明背景，不单独断言为干扰原因。
- done_to_read 证明完成读取延迟，不证明调度/中断是全部根因。
- artifact identity、SD 回读与 bit/payload 哈希仍有 provenance 缺口；原始 profile/manifest 中 board `not_run` 是历史快照，保持不可变。

## 在论文中的角色

P4M 短验收只支持“共同 V 硬件窗口内 A 完成量/边界在途 + 固定 gap 基线”这一测量路径在板上可运行、可记录、可复核。正式收益与取舍曲线必须由后续预试/正式采样给出，不能由 n=1 短验收推出。

## 精确 21 试次计划（pilot，非正式）

- 3 个确定性交错 block，每 block 7 trials，共 21 trials；pilot 不是正式结论。
- 固定 4096 V jobs、256 warmup、4 KiB、50 MHz、DS 50/50、V0 DID10、A1 DID11、PTW128 off。
- 每 block：P4M Solo、P4M A1gap0、legacy `--batch` A8gap0、P4M A8gap0、P4M A8gap500、A8gap1000、A8gap2000。
- 顺序：
  - block1: Solo, A1, BatchA8, P4MA8g0, g500, g1000, g2000
  - block2: Solo, g2000, g1000, g500, P4MA8g0, BatchA8, A1
  - block3: Solo, g1000, g500, g2000, BatchA8, P4MA8g0, A1
- 每 block 一次 Solo 基线（共 3 次），同工作量，block 内比较；legacy/P4M 成对相邻并做顺序反转。
- legacy 比较前确认 `/sys/module/idma/parameters/path_observe` 为 Y，与 P4M 的路径观测一致。`observe` 默认 false，但两条 batch 路径都会强制启用 observer（`idma.c:2060` 附近的 `batch || READ_ONCE(idma_observe)`），无需修改它；legacy 使用 UART `idma_batch_obs` 硬件 cycles，不用软件耗时。
- legacy 分支 driver 调同一 batch configure/write0 并 reset gap，同一窗口硬件 logging；P4M 额外启用共同窗口记录、gap 配置/回读及结果校验；主测批量供给循环相同。
- pilot gap500/1000/2000 覆盖约 1x 观察到的 shared-window A interval 1886 cycles 的分数区间，不是 active cycles/completed whole-run 723 的纯服务时间。
- 仅 pilot 点，不冻结正式参数。若端点未覆盖取舍，最多后续补一个有针对性的 4000 pilot 点，不自动扩矩阵。
- n=3 用于重复性与选点，不做等价性证明或显著性；不发明等价阈值；报告 paired ratio 与条件内范围；若 gap0 系统性差异仍在，正式前调查，不因 n=3 宣布等价。
- 功能错误、timeout、taint、quarantine、无效 bank/raw、record 冲突、缺 final marker 时停止。
- ZERO A window progress 合法：报告 zero+bounds，不停止、不排除、不当作纯零异常。
- 所有条件 V issued/done 4096，含 Solo。
- 软件 poll tail 警告保留，不因慢而拒绝；不做 cherry-picked rerun。
- 报告 median/min/max 与 paired values，描述性 pilot，无 CI/显著性；保留全部 trials。
- 本轮只确定计划，未启动预试。现有 `platform-p4m short` 仍只跑四组；下一项实施工作是准备这 21 次预试的执行与采集脚本，无需重新编译 BIT 或内核。

## 验收与停止标准

验收：21 trials 完成且无停止条件；每 block 有 Solo 基线；legacy/P4M 成对相邻且顺序反转；所有条件 V issued/done 4096；UART 的状态、角色、任务计数、时间分解与占用总和一致。legacy 从 UART 取得硬件窗口；P4M 再核对 JSON 中的同序号观测、窗口守恒及 done_to_read。预试使用适配 21 条件的复核程序，本轮短验收程序固定检查四组，不能直接用于预试。

停止：功能错误、timeout、taint、quarantine、无效 bank/raw、record 冲突、缺 final marker；legacy 比较前 path_observe 未启用；script sourcecopy mismatch；关键 evidence 缺失。

## 证据入口

- [原始 JSON](../out/mac-p4m-short-20260920-180713-run-l7cin11d/records.json)、[原始 UART](../out/mac-p4m-short-20260920-180713-run-l7cin11d/console.raw.log)、[运行记录](../out/mac-p4m-short-20260920-180713-run-l7cin11d/run.json)。
- [服务器复核结果](../out/p4m-short-review-20260920/review.json)含逐项指标、版本比较及原始文件校验值；[执行凭据](../out/p4m-short-review-20260920/acceptance.json)。
- 正式采样前补齐现有 Mac BIT/payload 文件哈希与传输记录；本轮未自动写 SD 或重烧。
