# Mac 连接 Genesys 2：上板准备

**最新：formal60 未完成——24通过，第25次日志计数检查停止，35未运行。** [停止原因](results/experiments/20260920-215406-run-cicggil4/FAILURE_HANDOFF.md)，[完整已采集结果](results/experiments/20260920-215406-run-cicggil4/analysis/RESULTS.md)，[Dell回执](results/experiments/20260920-215406-run-cicggil4/handoff-receipt.json)。原因是dmesg环形覆盖后总条数不增加，触发继承的检查；不是已发现DMA数据错误。全部原始证据已回Dell，未重试，Linux保持运行、Mac串口采集已关闭。下面“参数未冻结”等为历史状态。

**DeepSeek 全局接口1.4.0已安装。** 支持监督式终端/文件/会话工具请求，仍由Codex审核与现有工具执行；不改变系统审批。见[安装回执](tools/deepseek-global/INSTALLATION.md)。

**最新：Linux00089 的 4 短＋8 对照已全部通过交付 runner 并回传 Dell。** [完整结果](results/experiments/20260920-211940-run-46nvmwgi/RESULTS.md)，[回传回执](results/experiments/20260920-211940-run-46nvmwgi/upload-receipt.json)。12/12、exit0、taint/data_errors/quarantined0，无重跑；B/P硬件吞吐中位108.813/108.858 MiB/s，4对周期比0.999414～0.999996，仅描述性接近。短A1的启动跨度及hrtimer警告保留。Dell HANDOFF/validation已更新，串口已关闭；正式参数未冻结，以下写卡等待状态为历史。

**最新：P4M start-control / Linux00089 已写 SD，待插回 FPGA。** 只更新 payload，复用 P4M v7 BIT；原启动区备份 `results/sd/update-wvxp4i78/`，未改第二分区，按要求跳过哈希和写后回读。下一步已授权4短＋8对照、遇错停止，详见 [上板交接](P4M_START_SD.md)。以下00088记录为历史。

**最新：21 次 P4M 预试已完成（4096 transfers / warmup 256）。** 7 条件各 3 次，全部 exit=0、data_errors/quarantined/taint=0，未重试；原始结果已回传 Dell `out/mac-p4m-pilot-20260920-202441-run-9_mfua8v`。见 [执行记录](results/experiments/20260920-202441-run-9_mfua8v/EXECUTION.md)。A8 gap0 第三次与前两次有差异，不能据此宣称 batch/P4M 等价；正式实验未启动。以下是历史记录。

完整 [21 次结果表与建议](results/experiments/20260920-202441-run-9_mfua8v/RESULTS.md) 已回传，Dell HANDOFF/validation 已更新，见 [回执](results/experiments/20260920-202441-run-9_mfua8v/handoff-receipt.json)。

本轮可复用脚本：[linux_p4m_pilot21.sh](scripts/linux_p4m_pilot21.sh)。仅在另获运行授权后，向同版 00088/P4M v7 板卡传入脚本，确认 `path_observe=Y`，执行 `sh /tmp/p4m-pilot21.sh /tmp/新的独立结果目录`；输出目录必须不存在。脚本固定交错顺序、V0/A1、21 次、遇错停止，并将 JSON、退出码与完整 dmesg 输出至串口；必须先开启串口记录。不要用旧的四组 short runner 代替，不自动补跑或扩大条件。原始数据与版本信息每轮回传 Dell，并更新交接。

**最新：P4M 四组交付短实验已按用户本轮明确指令完成。** Solo/A1gap0/A8gap0/A8gap1000板端全部成功、taint/data_errors/quarantined0；完整结果已回传Dell并更新交接。见 [指标表](results/experiments/20260920-180713-run-l7cin11d/METRICS.md)。仅4组，未扩矩阵或冻结正式参数；以下“仅启动”是此前状态。

**最新（2026-09-20）：P4M 新 BIT 与 Linux00088 已启动到登录界面，未运行验收或实验。** 启动证据已回传Dell并更新交接，见 `results/experiments/20260920-175705-run-rsn6mvks/upload-receipt.json`。当前等待明确实验范围；以下写卡准备状态为历史。

**最新准备（2026-09-20）：P4M / Linux00088 已写 SD 并安全推出。** 配套新 BIT 已下载，待加载 `migration-4dma-p4m`；不可沿用 E3 v6 BIT。旧启动分区备份：`results/sd/update-v6_09g7q/boot-before.bin`，第二分区未改。按用户要求未跑验收/回归、未计算下载哈希、未做写后回读。新交付仅有 `platform-p4m short` 四组工程短验收，正式参数尚未冻结；不自动执行短验收或发明正式矩阵。等 SD 插回断电 FPGA 并接好后继续；实验范围待用户回复。

复用下载入口 `python3 scripts/fetch_p4m.py`；写卡入口 `sudo /opt/homebrew/bin/python3 scripts/sd_card.py update --disk /dev/disk5 --release p4m-baseline-20260920 --skip-payload-hash --skip-readback`（每次重新确认设备编号）。加载新 BIT：`python3 scripts/fpga.py program --profile migration-4dma-p4m`。本轮适配由 Codex 完成；DeepSeek 请求长时间未返回，停止等待，未采用其提案。

**v6 交付已完成：完整 16 文件已回传 Dell，逐文件远端回读一致，远端 HANDOFF/validation 已更新并确认。** 本地回执：`results/experiments/20260918-164635-run-rk48bhte/upload-receipt.json` 和 `handoff-receipt.json`。本轮不再追加实验。

**最新实板结果：v6 / Linux00087 已完成固定 3 短 + 8 正式对，共 22 轮。** 全部板端成功，taint/data_errors/quarantine 为 0，418 条完整观测；结果与 PTW/miss/周期表见 [E3_CONTROLLED_RESULTS.md](E3_CONTROLLED_RESULTS.md)。冷态正式 12/12 触发、V miss 全为 0；仍报告基线和前置时间差，不直接宣布纯 D128 因果证明。当前状态及回传回执在 `results/experiments/20260918-164635-run-rk48bhte/`。下方准备说明为历史状态。

新版离线收集：`python3 scripts/collect_e3_controlled.py results/experiments/20260918-164635-run-rk48bhte --phase short`，正式阶段将 `short` 改为 `main`。本轮上传入口 `python3 scripts/return_e3_controlled.py --upload` 拒绝覆盖已有远端目录；已有回执时不要重复上传。测试需要 pytest，可在隔离环境执行 `python -m pytest tests scripts/test_e3_queue_fields.py scripts/test_e3_diag_fields.py scripts/test_collect_e3_controlled.py -q`。这不会重跑 FPGA 实验。

**最新准备：v6 / Linux00087 已写入 SD 并安全推出。** 仅更新第一启动分区，未写第二分区；旧启动分区备份 `results/sd/update-vfcfn3na/boot-before.bin`。按要求跳过下载哈希和写后回读。新 BIT 已下载但尚未加载，必须选择 `migration-4dma-e3-controlled`，不可混用 v5。交付手册：[v6 README](artifacts/software/e3-controlled-20260918/delivery/README.md)。本次未运行实验。

本次可复用入口：`python3 scripts/fetch_e3_controlled.py`；SD 身份核对后 `sudo /opt/homebrew/bin/python3 scripts/sd_card.py update --disk /dev/disk5 --release e3-controlled-20260918 --skip-payload-hash --skip-readback`（设备编号每次重新确认）。SD 插回断电板卡并接好后，再开启串口记录和加载新 BIT。后续手册范围为 3 短 + 8 正式对，尚未执行；结果照常回传 Dell。

**最新交付：E3 v5 本轮完整结果已回传 Dell，并更新远端 HANDOFF 与 validation。** 回执为 `results/experiments/20260918-130545-run-d01lc9wp/upload-receipt.json`；用户已持续授权以后每轮默认回传，规则写入本项目 `AGENTS.md`。下文“尚未回传”为此前状态。

**最新：E3 v5 / Linux00086 实板完成，3短+10正式配对、26子轮全部通过执行及字段验收。** 正式 A8 16/16 触发、A1 0/4；但 V 首次 miss 和插入记录截断使本轮不能证明纯 D128 因果效应。结果见 [E3_QUEUE_RESULTS.md](E3_QUEUE_RESULTS.md)，原始证据已本地保存，尚未回传 Dell。以下为历史阶段。

**最新：E3 v5 / Linux00086 已写入 SD 并安全推出；新 BIT 已下载但尚未加载。必须换用 migration-4dma-e3-queue BIT，不能沿用 v4。** 见 [E3_QUEUE_SD_UPDATE.md](E3_QUEUE_SD_UPDATE.md)。按要求跳过下载哈希和写后回读，旧启动分区已备份。以下为历史阶段。

**最新：00085 D0/D0诊断已完成，3短+6正式配对共18轮通过。** 两轮A预热数量/末页相同，预热miss不同；有一次16.92ms软件启动间隔。结果 [E3_DIAG_RESULTS.md](E3_DIAG_RESULTS.md)。已获用户批准，完整记录已回传dell并更新交接。

**最新：00085 D0/D0诊断软件已写入SD并安全推出，未重新编译，现有E3 BIT不变。** 见 [E3_DIAG_SD_UPDATE.md](E3_DIAG_SD_UPDATE.md)，尚未上板。

**最新：E3配对采样完成，3短验收+12正式配对全部执行通过；但正式A8双轮均触发为0/10，暂不能作配对因果结论。** 详情 [E3_STAGE_RESULTS.md](E3_STAGE_RESULTS.md)，未补采或修改RTL。

**最新准备：E3配对版Linux00084已写SD并安全推出，新BIT已下载，等待插回FPGA。** 配套profile为migration-4dma-e3；详见 [E3_SD_UPDATE.md](E3_SD_UPDATE.md)。E3尚未上板，不复用旧版单轮采集器。

**当前：v3 分段路径实验已完成，3短验收+30正式全部通过。** Solo/A1/A8硬件吞吐283.32/149.31/103.28 MiB/s，详见 [PATH_STAGE_RESULTS.md](PATH_STAGE_RESULTS.md)。下方各结果为历史阶段。

**最新：50/50 BIT 回切复测 30/30 通过**。Solo/A1/A8 硬件吞吐 283.33/149.51/103.31 MiB/s，A8 性能恢复；详见 [DS50_RETEST_RESULTS.md](DS50_RETEST_RESULTS.md)。

**最新：DS0/0正式对照60/60通过**，固定DMA0/DMA1，Solo/A1/A8/A16/A32/A64。A8～A64硬件吞吐较旧DS50/50提高约31%～36%，详见 [DS0_BATCH_RESULTS.md](DS0_BATCH_RESULTS.md)。已回传dell。

最新准备：**DS0/0 batch 新BIT**，Linux沿用00082；本次SD写入已完成，见 [DS0_BATCH_UPDATE.md](DS0_BATCH_UPDATE.md)。新加载profile为baseline-4dma-batch，尚未上板。

**最新：固定DMA0受害/DMA1干扰，A16/A32/A64正式30/30通过**。按用户要求未短校准、未交换角色；中位硬件吞吐98.27/95.95/94.84MiB/s。见 [BATCH_EXT_RESULTS.md](BATCH_EXT_RESULTS.md)，已同步dell。

**最新：batch正式矩阵60/60通过（加短校准共74/74）**。Solo/A1/A8硬件吞吐约283/149/103 MiB/s，结果、PTW/miss表及保留异常样本见 [BATCH_MAIN_RESULTS.md](BATCH_MAIN_RESULTS.md)。已同步dell；下面保留历史阶段。

工作位置：`/Users/jojo/Desktop/在mac链接pfga(codex)`。
Mac 负责 JTAG 下载和串口；dell 负责 FPGA/Linux 编译。

最新：**硬件批量版 Linux00082 + migration-4dma-batch 已上板，14/14短校准通过**，真实双路在途重叠约99.7%～99.8%。结果与脚本用法见 [BATCH_SHORT_RESULTS.md](BATCH_SHORT_RESULTS.md)，下载/写卡见 [BATCH_SD_UPDATE.md](BATCH_SD_UPDATE.md)。正式性能样本尚未运行；它与旧E4观测版不是同一模式，不能沿用旧H/P/R验收口径。

**最新：E4新版实板105/105通过**（9短测+36开关开销+60主样本），结果已回传dell。V的H约56ms基本不变，共跑增加主要在完成→DONE读取的P段；60主样本AXI重叠均0，不能归为纯CPU/IOMMU或HOL。见 [E4_STAGE_RESULTS.md](E4_STAGE_RESULTS.md)。observe已关闭、串口退出，下文为历史阶段。

**最新准备：E4新BIT与Linux00081已下载，SD已更新并安全推出。** 按用户要求未做下载哈希/写后回读，旧版保留。新版尚未上板，必须配套observe BIT；见 [E4_SD_UPDATE.md](E4_SD_UPDATE.md)。

**最新：当前双路E1完成，144/144通过**（16项HPM短测+48敏感性+80主样本）。A1页耗时约Solo的1.73倍，A8/32页约1.79倍；无数据错误，结果已同步dell。见 [E1_STAGE_RESULTS.md](E1_STAGE_RESULTS.md) 和 [HPM_REMAINING.md](HPM_REMAINING.md)。这是现有接口、DS50/50下的软件计时E1，不是完整E0/P2或E2/E3验收；以下为历史阶段。

最新性能进度：经用户同意跳过剩余校准，已完成 **18/18 探索性样本**。Solo约27.3 MiB/s，双路被测通道约15.2–15.3 MiB/s，软件观测耗时约1.79倍；无数据错误、taint=0，结果已回传dell。见 [PERFORMANCE_EXPLORE.md](PERFORMANCE_EXPLORE.md)。不是纯硬件延迟或正式校准后的性能结论。以下各节“未进入性能矩阵”描述此前阶段。

HPM 最新进度：双路 **12/12 + 6 空闲对照通过**，无数据错误，taint=0，状态已恢复、结果已回传 dell；见 [HPM_DUAL_DID.md](HPM_DUAL_DID.md)。此前单路 **32/32 + 8 空闲对照通过**，见 [HPM_SINGLE_DID.md](HPM_SINGLE_DID.md)。双路 aggressor 完整调用128、窗口126是不同范围的观测，未证明精确边界。完整 HPM 计数语义/溢出/开销等仍待验证，未进入性能矩阵。

最新：缓存同步修正版 `…00080-g3b47ab1645e4` 已完成两次新启动复测，**29/29 组通过**（短试次+14组，再新启动直接14组），data_errors=0、taint前后=0；结果已回传 dell。见 [DMA_CACHE_SYNC_RETEST.md](DMA_CACHE_SYNC_RETEST.md)。未进行 HPM 校准或性能矩阵。用户要求后续下载/写卡阶段不额外跑测试、文件哈希核对或写后回读，以速度优先；仍识别目标卡并备份，明确记录未验证边界，不把功能验收等同于文件完整性验证。

历史诊断内核 `…00079` 的失败见 [DMA_DIAGNOSTIC.md](DMA_DIAGNOSTIC.md)：一次 warmup=32 Solo 通过，后续 smoke 首组 warmup=0 失败，偏移912的目标值 `eb` 经 FENCE.I 后变为 `14`。该失败已回传并促成缓存同步修正版；不要与上面的新版通过结果混淆。

此前旧软件 DMA smoke 也未通过：原脚本缺少 BusyBox timeout 依赖，临时兼容版 warmup=0 通过、warmup=32 失败；历史记录见 [DMA_SMOKE.md](DMA_SMOKE.md)。新诊断软件已补齐超时依赖，但数据故障仍可复现。

首次上板状态及日志索引见 [FIRST_BOARD.md](FIRST_BOARD.md)。本机 JTAG 默认使用已验证的 1 MHz。

历史旧 BIT：能进入新版 Linux，但 `root` 登录触发 `generic_permission` 内核异常，见 FIRST_BOARD.md。最新修复版已成功登录并完成 100 轮 proc 检查，见下。

2026-09-16 最新实测：SC.W 修复版 BIT 已以 1 MHz 加载，STAT `0x501079fc`，DONE/EOS/GWE 正常。使用同一 SD 正常启动并登录 root，100 轮 proc 读取/子进程退出检查通过，taint 前后均为 0，未复现原 proc/Oops 异常。板卡保持 Linux 运行，串口采集已结束。最新结果、脚本及日志见 [SCFIX_BOOT.md](SCFIX_BOOT.md)，RTL 定位/54 场景回归见 [LINUX_BOOT_FIX.md](LINUX_BOOT_FIX.md)。

新 BIT 在 `artifacts/migration-4dma-scfix/ariane_xilinx.bit`，SHA256 `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3`。使用以下入口（不会覆盖旧 BIT）；没有完整构建记录或校验失败时脚本会拒绝配置：

```sh
python3 scripts/fetch_scfix.py
python3 scripts/fpga.py program --profile migration-4dma-scfix --dry-run
# 实际配置前先开启串口日志
python3 scripts/fpga.py program --profile migration-4dma-scfix
python3 scripts/fpga.py status
python3 scripts/fpga.py new-run --profile migration-4dma-scfix --label scfix-boot
```

注意：不带 `--profile` 的旧 `program` 命令仍选择历史 migration-4dma BIT，仅用于明确的旧版对照。修复验证必须显式选择 `migration-4dma-scfix`。

正式时序 WNS 0.177 ns、WHS 0.010 ns、WPWS 0.062 ns，已设置的时序约束满足；仍有 6 项时钟 Critical Warning，IO/CDC 审查未完成。构建状态及原始报告在 `results/build-scfix-20260916/`。Mac 离线测试 23/23 通过。新 BIT 的 Linux 登录/proc 复测已通过；DMA smoke 已尝试但失败，独立原子指令复测和 HPM 仍未验收，SD 本轮未重写。

## 文件位置

| 目录 | 内容 |
|---|---|
| `artifacts/migration-4dma/` | 已固定 SHA256 的 `ariane_xilinx.bit` |
| `artifacts/software/` | 新版完整软件交付、下载校验记录和说明；已写入 SD 并回读校验 |
| `references/dell/incoming-*/` | 每次独立下载快照，含服务器原路径层级和 `download.json` |
| `scripts/fpga.py` | 本地检查、获取文件、JTAG 和串口工具；仅依赖 Python 标准库 |
| `results/preflight/` | Mac 工具、板卡支持、BIT 校验记录 |
| `results/jtag/` | JTAG 命令输出、退出码及 BIT 版本 |
| `results/serial/` | 原始串口接收日志及会话时间/参数 |
| `results/experiments/` | 每次实验的版本、命令、结果和备注 |
| `scripts/sd_card.py` / `results/sd/` | 已登记 SD 卡的备份、启动分区更新、回读验证和恢复记录 |

每次操作生成独立目录。下载快照的 `download.json` 只有 `status: verified` 表示完整下载并通过 BIT 校验，`incomplete` 表示未完成。资料文件另记录 SHA256；BIT 的固定哈希来自交接并在服务器端复核。

## 未连接板卡时

在此目录的终端运行：

```sh
python3 scripts/fpga.py check
python3 scripts/fpga.py program --dry-run
```

需要重新下载固定版本和资料时：

```sh
python3 scripts/fpga.py fetch
```

下载已核对的 2026-09-15 软件交付：

```sh
python3 scripts/fetch_software.py
```

每次保存到 `artifacts/software/20260915T140327Z-*/experiment-v1/`，上层 `download.json` 的 `status: verified` 表示完整下载及校验通过。脚本固定交付清单哈希，并检查全部文件的 SHA256、产物大小及所配 BIT；服务器再次更新版本后应重新核对清单。

使用已有 SSH 别名 `dell`（经 `ras` 跳转），采用已有密钥认证和主机密钥校验。脚本不会保存凭据。BIT 已存在时先校验；遇到不同文件停止，保留现场。新硬件版本需另行核对并更新固定大小/哈希。

## 连接板卡后

SD 卡已识别为现有 8 GB GPT 启动卡，本次编号 `/dev/disk5`。完整的脚本操作与恢复说明见 [SD_CARD.md](SD_CARD.md)。更新入口如下，每次先以 `diskutil list` 确认实际编号：

```sh
python3 scripts/sd_card.py update --disk /dev/disk5 --dry-run
sudo /opt/homebrew/bin/python3 scripts/sd_card.py update --disk /dev/disk5
```

脚本自动备份原 32 MiB 启动分区、写新版 payload 并回读校验，保留第二分区。确认 `status.json` 为 `readback_verified` 后安全推出，再放入板卡。

**本次 SD 更新已在系统终端完成，完整回读校验通过，并已安全推出。** 旧启动区备份和结果在 `results/sd/update-6curphb9/`。后台管理员方式曾受权限限制，这台 Mac 请优先使用系统终端；详细说明见 `SD_CARD.md`。

1. 板卡接电，并将 Mac 数据线接到板上 USB-JTAG 接口。
2. 识别 JTAG：

   ```sh
   python3 scripts/fpga.py detect
   ```

3. 将串口 USB 接到 Mac，列出设备：

   ```sh
   python3 scripts/fpga.py ports
   ```

   根据插拔前后变化选择实际串口；蓝牙等候选不能当成板卡。JTAG 与串口按板卡接口标识分别接线。USB 连接应由 macOS 持有。

4. 在另一个终端打开串口，把命令中的设备路径换成实际路径：

   ```sh
   python3 scripts/fpga.py console /dev/cu.实际串口设备名
   ```

   固定 115200、8N1、无流控。接收数据逐段写入 `console.raw.log`；按 `Ctrl+]` 退出，其余键盘输入发送到板卡。限时采集可加 `--capture-seconds 60`。日志包含终端收到的字节，不包含本地未回显的键盘输入。串口须先开启，再启动/复位板卡，才能捕获早期启动信息。

5. 在原终端校验并加载 FPGA：

   ```sh
   python3 scripts/fpga.py program --profile migration-4dma-scfix
   python3 scripts/fpga.py status
   ```

   使用 `openFPGALoader -b genesys2 <bit路径>`，只做易失配置，断电后重新加载。`status` 读取 STAT，检查 DONE/EOS/GWE 和 CRC/ID 错误。工具返回成功但读到未就绪状态时，脚本记录 `startup_incomplete`、返回 2；Linux 启动另看串口。`program` 若没有附带 STAT 输出，仍需运行 `status` 确认。

   板上 LD15 是电源正常灯，LD14/DONE 是 FPGA 配置完成灯。实际 UART 本次为 `/dev/cu.usbserial-AV0KRCHT`；`200300BD51480/1` 属于 Digilent JTAG USB 芯片，不用作 Linux 串口。

   本机默认 6 MHz 首次下载后 DONE 未就绪，1 MHz 重试后配置通过并开始从 SD 加载软件。因此脚本现默认 **1 MHz**，约需 90 秒传输；也可显式指定 `--freq 1000000`。这是一组实际对照记录，尚未确定 6 MHz 失败的具体原因。

6. SD 制卡和 Linux 启动确认后，创建实验记录目录：

   ```sh
   python3 scripts/fpga.py new-run --label first-smoke
   ```

   在 FPGA Linux 串口运行最新版 `/usr/bin/platform-smoke`，将串口日志位置、软件哈希、命令和结果写入该目录。`run.json` 初始状态为未开始，验收字段留空，需根据真实结果填写。之后再做 E0/HPM 校准。

## 当前边界

- 当前硬件为 `migration-4dma-scfix`，DS 输入/输出延迟 50/50 cycles；DS 0/0 的 `baseline-4dma` 尚未构建。
- 当前 BIT：11,443,722 bytes；SHA256 `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3`。历史 BIT 独立保留。
- 2026-09-15 新软件交付已包含小 chunk 校验修复和多页部分覆盖 smoke，详见 `artifacts/software/README.md`。现有 SD 卡布局与项目 32 MiB 原始 GPT 启动分区约定一致，更新流程见 `SD_CARD.md`。
- 板卡已连接，USB/JTAG、1 MHz 易失下载、UART/SD、Linux 启动/登录和 proc 检查已通过；最新结果见 `SCFIX_BOOT.md`。DMA smoke、HPM、时钟/CDC/IO 约束审查仍待办。
# 最新实验：v3 分段路径观测

新版本刷卡、配套 BIT 和上板顺序见 [PATH_SD_UPDATE.md](PATH_SD_UPDATE.md)。本轮固定 DMA0/DMA1，3 次新观测验收通过后再运行 Solo/A1/A8 各 10 次；不要使用旧版仅采集三行的脚本。
