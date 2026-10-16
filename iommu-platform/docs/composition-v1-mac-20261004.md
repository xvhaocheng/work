# composition-v1 交付与上板说明（2026-10-04）

## 2026-10-07 现行BIT：隔离替换历史，定向22/22通过

- 当前Mac BIT：`tools/composition-isolated-20261007/ariane_xilinx.bit`；Dell对应[route-opt BIT](../out/composition-isolated-20261007/route-opt/ariane_xilinx.bit)。WNS +0.022ns/WHS +0.051ns，门槛通过。
- Linux仍为`6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty`，仍从SD启动；boot_id `d61e7517-2b15-4702-86f2-3f338ddd5345`。本轮只JTAG重载一次，不写SD；网口用于控制和实验。Linux保持运行、UART已释放。
- 定向入口`tools/composition-isolated-20261007/mac_composition_history.py`复用原runner，冻结short4+explore18共22；需`--through-phase explore`。原172入口与本入口区分。SSH helper仍用`tools/composition-clean-20261006/mac_update_reboot.py`，凭据仍由`scripts/run_research2_mac.py`在内存读取。
- 已完成Mac `results/composition-isolated-20261007-formal01`，启动记录`results/composition-isolated-20261007-boot01`；已全部回传Dell本轮`mac-records/`。结果目录不能覆盖或resume，不自动重复已完成实验。
- [本轮报告](composition-history-isolation-20261007.md)：A8旧P/PN A miss5117/6582，隔离后2048/2048，双方绝对耗时降低约21%；PN相对P仍慢12.67%～12.79%。剩余差额未归因，不合并不同BIT/boot为同一次实验。

下方clean172与ptwfix条目均为历史版本；当前BIT以上节为准。

## 2026-10-06 现行入口：裁剪 BIT + cpn3-clean（172/172通过）

- 用户最新授权的一轮172已于23:38 JST完成：Mac `results/composition-clean-20261006-formal01`，short16/noise12/explore144全部通过，exit0/failed0、逐项数据错误/taint/恢复状态0；boot_id全程不变，原始JSON/SSH/UART日志全部回传。[新BIT板测验收](../out/composition-clean-20261006-board/board-acceptance.json)。本轮直接attach，无重编/写卡/重载/预跑；Linux保持运行、UART已释放，不自动重复实验。这是功能结论，未声明性能或创新收益。
- Mac BIT：`tools/composition-clean-20261006/ariane_xilinx.bit`；工具：`tools/composition-clean-20261006/mac_update_reboot.py`；配套Linux：`6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty`。最终WNS+0.169ns/WHS+0.038ns，整板LUT156215。[构建与部署验收](../out/composition-clean-20261006/acceptance.json)。
- 已完成网传、32MiB第一分区写入同步、新BIT JTAG加载、SSH新内核/新boot_id确认；boot_id `05f32544-9e7d-4a2b-bc56-9b5dcabb8138`。原始结果Mac `results/composition-clean-20261006-boot02/`，Dell [回传目录](../out/composition-clean-20261006/mac-records/)。Linux保持运行，UART已释放，无需再写卡或重载。
- 网络/登录/1MHz JTAG参数沿用下方既有流程。将来需要重载时仍用既有helper，显式指定本节BIT和`--expected-kernel`及新的`--out`；没有软件更新需求时省略`--payload`。不要直接复制下面旧版本命令加载旧BIT。
- 软件更新阶段第一次旧内核`wc`软锁死后保留现场并重载旧BIT恢复，第二次更新成功；根因未确认。该阶段未增加hash/回读/实验；上条是用户随后授权的新BIT独立172结果，不沿用旧ptwfix的172结论。

下方均为先前版本交接和命令，保留复现依据；当前版本以上节为准。

## 2026-10-05 现行入口：composition-ptwfix route-opt + net4trace（172/172 通过）

> 本节是当前操作入口；下方“composition-trim route-opt + net3 + 172”一节及其后的串口 SD 上板 / cpn1 示例均标为历史，只保留原始记录。本轮完整 172 已通过功能验收；性能和研究收益仍需单独分析。

### 当前硬件与状态

- 现行 Mac BIT：`tools/composition-ptwfix-20261005/ariane_xilinx.bit`；Dell 验收对应 [out/composition-ptwfix-20261005/route-opt/ariane_xilinx.bit](../out/composition-ptwfix-20261005/route-opt/ariane_xilinx.bit)。不要用旧 composition-trim/net3 BIT 代替当前 BIT。
- 验收记录 [acceptance.json](../out/composition-ptwfix-20261005/acceptance.json)：route-opt BIT 最终时序通过，WNS +0.031 ns、WHS +0.019 ns、WPWS +0.062 ns；276241/276241 线路布通、errors 0；native BIT DRC errors 0、组合环 / 锁存环 / 无时钟 0。完整板级 IO/CDC 签核仍未完成。
- RTL 修复：沿用 DS 写通道来源对齐修复，并修正 PTW 合法叶分支（`PTW_legal_leaf_branching`）。针对性仿真 [targeted-test-acceptance.json](../out/composition-ptwfix-20261005/targeted-test-acceptance.json) 由旧失败转为 7/0 通过；旧 legal 4KiB 读、写被分别误报为 cause 13、cause 15。该范围只覆盖 PTW 接口与既有 TL 回归，不等于板级 PN 验收。
- 已加载当前 BIT 并启动 Linux/SSH：boot_id `2695d848-8589-491e-bf7f-1de7af5ea9cb`；此前失败的 PN4096 实际 1/1 实板通过（V_completed 4096、A_completed 7244、V/A data errors 0、taint 0）。
- 完整冻结 172 已在 `results/composition-ptwfix-20261005-formal01` 一次完成：172/172 通过、0 失败（short 16、noise 12、explore 144），原始目录已回传。[板测验收](../out/composition-ptwfix-20261005/board-acceptance.json)。旧 net3 和 DS-only 两轮各 52/1/119 的记录保留，未并入本次结果。

### 当前内核与网络

- 现行内核：`6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty`。
- Mac `192.168.77.1`，FPGA `192.168.77.2`，USB 网卡百兆，JTAG `1000000`。SD 常驻板卡，正常工作走以太网/SSH；UART 只作故障诊断，同一时间只能有一个采集者。

### Mac 项目根目录

```sh
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
```

### 日常：同步并重载当前 BIT（不带 `--payload` = 不写 SD）

- 执行前先确认并结束已有 XVC / 其他 JTAG 占用，确保同一时间只有一个 JTAG 使用者。

```sh
tools/method88-venv/bin/python scripts/mac_update_reboot.py \
  --run --transport ssh \
  --credential-script scripts/run_research2_mac.py \
  --host-ip 192.168.77.1 \
  --board-host 192.168.77.2 \
  --bit tools/composition-ptwfix-20261005/ariane_xilinx.bit \
  --expected-kernel "6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty" \
  --out "results/composition-ptwfix-$(date +%Y%m%d-%H%M%S)-reload" \
  --boot-timeout 900 \
  --jtag-hz 1000000
```

- 固定流程：正常工作走以太网、SD 留板卡；仅 RTL 改变时 sync + JTAG 重载指定 BIT、不写 SD；只有 Linux 改变时才加 `--payload <fw_payload.bin>`，此时 `--expected-kernel` 必须换成该软件实际交付的 `uname -r` 版本，不沿用旧 net4trace；网传后只写 32 MiB 第一原始分区并 sync，再用 JTAG 重载已验收 BIT，不覆盖整卡。
- 不带 `--payload` 时工具仍先 sync 再 JTAG 重载；默认 dry-run，只有 `--run` 才执行；`--out` 必须每次新目录，不覆盖、不 resume。
- 不加入常规 payload hash、写后回读、重复 build 或额外预跑；保留工具已有身份核对与退出码。

### 异常卡死：先保存证据，再释放 JTAG 后手动恢复（此前已确认完全卡死的 DS 恢复例外，不是默认流程）

先保存可取得的故障输出；UART 诊断保留且只能有一个采集者。确认既有 JTAG XVC 已正常退出后，使用下方 openFPGALoader 手动加载同一已验收 BIT；这不是脚本现有 rescue CLI 开关：

```sh
/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 \
  tools/composition-ptwfix-20261005/ariane_xilinx.bit
```

这一步不写 SD；恢复后用 SSH 读取实际内核、boot_id 与网络状态。恢复成功不代表失败实验已通过；免 sync 仅适用于此前已确认完全卡死的 DS 恢复历史例外（本次 PTW 上板走正常 sync 流程），不能固化为默认流程。

### 跑冻结 172（纯网口 SSH attach；以下供今后明确启动新一轮使用）

```sh
tools/method88-venv/bin/python tools/composition-v1-20261004/mac_composition.py \
  --run --transport ssh \
  --board-host 192.168.77.2 \
  --credential-script scripts/run_research2_mac.py \
  --ssh-helper scripts/mac_update_reboot.py \
  --through-phase explore \
  --expected-kernel "6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty" \
  --attach-timeout 60 \
  --out "results/composition-ptwfix-$(date +%Y%m%d-%H%M%S)"
```

- `--through-phase explore` 才跑冻结 172，一次执行已含前序阶段；`--out` 必须不存在。一次只执行一遍 172。
- 失败即停：技术失败保留现场后停止并定位，不自动重跑；禁止自动改 172 计划或阈值、禁止用重试掩盖失败；科学阴性（fast==0、miss==0、无收益）不是技术失败。
- 结束把原始目录（含失败现场）回传 Dell，释放 UART/JTAG 占用而 Linux 保持运行。
- 进入正式 172 前关闭 trace 是当前内核要求（`echo N > /sys/module/idma/parameters/composition_trace`）；单次 PN4096 仅属本轮排错，不是 172 的一部分，也不作为每次 172 的常规额外预跑。
- 凭据只通过现有 `scripts/run_research2_mac.py` helper；不读、不输出密码或密钥；不要把 `root@` 写进 `--board-host`。

### 本轮全 172 验收结果

- `status=complete`、`planned=attempted=completed=172`、`failed=0`，全部 case 的 problems 为空；每项 taint=0、恢复状态=0、参与传输的数据错误=0。40 项 solo 没有 A，不把空 A 当成缺失结果。新 BIT 上 PN 单项和本轮正式 172 使用同一 boot，单项不计入 172。
- UART/JTAG 占用已释放，Linux 保持运行；本轮 JTAG 加载 1 次、SD 写入 0 次。这是功能验收，不等于性能收益、机制创新或完整板级 IO/CDC 签核。

---

## 2026-10-05 历史入口：composition-trim route-opt + net3 + 172

> 以下为历史 net3 一轮，记录保留，不再作为当前入口。

> 本节覆盖旧的串口上板入口；旧历史说明保留在下方。日常传输和操作走网口；当前开发实验保留 UART 记录现场，并保留 JTAG 恢复。net3 内核从原 SD 启动，本轮不写 SD。

### 当前硬件与网络

- 用户确认03:06时序通过的 `tools/composition-trim-20261005/route-opt/ariane_xilinx.bit`；首次加载后net3启动，实验boot_id为`29d710b2-4949-402d-8055-8734cbaa5220`。内核为`6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty`。
- 纯SSH一次运行计划172；short16、noise12、explore24完成，共52。第53项`explore-b1-g1000-a8-mpn`超时后停止，119未运行，失败CLI JSON/exit/boot.after缺失；`boot id changed: None`不能证明重启。完整172未验收，不自动重跑。[结果记录](../out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/RESULTS.md)。
- 用户已接回UART，原端口115200身份查询仍无字节；Mac侧`en11`为`192.168.77.1/24`且active，板端ping/TCP22均无响应。根因未确认，重新接线不能补回此前的UART输出。
- 现场保存后，JTAG成功重载同一BIT，net3/SSH已恢复，boot_id `476dcf54-3dfb-441f-b95c-3440589736f5`、taint0、carrier1，UART启动日志已保存。本请求累计JTAG2次、SD写0次；[恢复结果](../out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-03-rescue/result.json) · [执行记录](../out/composition-board-net3-20261005/execution-state.json)。

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

- 不带 `--payload`：同步并JTAG重载，不写SD。
- 本轮 reload 实际结果在 `mac-records/results/composition-routeopt-net3-20261005-02-reload/result.json`。
- 下次 `--out` 必须用新目录，不能覆盖旧结果。

### Linux已经失联时：直接JTAG恢复

上面的SSH入口会先读取旧启动身份并同步；板端已失联时无法通过这一步。先保存可取得的故障输出，独占UART采集启动日志，再直接加载同一已验收BIT：

```sh
/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 \
  tools/composition-trim-20261005/route-opt/ariane_xilinx.bit
```

这一步不写SD。待Linux启动后再通过SSH读取实际内核、boot_id和网络状态；恢复成功也不代表失败实验已通过。纯SSH实验入口本身不会采集UART，另一个采集进程只能独占串口；不要同时启动两个串口读者。

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
- 现有密码认证 helper 从现有Mac脚本在内存里做AST提取，沿用既有登录方式。SSH 复用为 ControlMaster auto、ControlPersist 60、自身短临时 socket，最终 close，并保留原始帧。

### 安全与失败边界

- 失败即停：技术失败保留现场后停止，不自动重跑；科学阴性（如 fast==0、miss==0、无收益）不是技术失败，不因此停止。
- 首次 SSH 加载 01 在 sync 段因 PTY EOF 先于 waitpid 正常结束被误判失败（9 秒停止，未到 JTAG）；Codex 修正仅退出等待，02 已通过。不要把这次误报当 Linux sync 实际超时。
- 不做 payload hash、SD 回读、再短跑全量、完整重编；回传沿用 Mac tar over SSH 到 Dell `out/composition-board-net3-20261005`。
- 不要夸大板端完整 IO/CDC 签核，也不要写成机制创新或性能收益。DeepSeek 起草 SSH/tests/docs；Codex 负责关键退出竞态、架构决定、源码审查、工具执行与实板验收。


[实际加载与执行状态](../out/composition-board-net3-20261005/execution-state.json) · [纯网口实板连接验收](../out/composition-board-net3-20261005/ssh-smoke-result.json)。

---

一次人工写 SD + 一次加载 BIT；启动后四个模式 S / N / P / PN（mode 0/1/2/3）在**同一次 boot、同一个 BIT** 内由驱动 ioctl 0x11 写 live config（offset 2）运行时切换。切换不重写 SD、不重载 BIT、不重启；全部 172 用例在一次启动内跑完。

## 交付文件（平铺到 `tools/composition-v1-20261004/`）

| 文件 | 说明 |
|---|---|
| `ariane_xilinx.bit` | FPGA bitstream（只加载一次） |
| `fw_payload.bin` | 写入 SD 第一分区的 payload（29,363,656 字节，只写一次） |
| `mac_composition.py` | Mac attach-only runner |
| `composition_plan.py` | 172 用例冻结计划（runner 导入） |
| `mac_research2.py` | 提供 `SerialConsole`（runner 导入） |
| `DELIVERY.json` | 交付清单；`fw_payload.bin` 的 sha256 为 null，按用户要求跳过哈希核对 |
| `SPEC.md` | 现有规格说明 |
| `RESEARCH_REVIEW.md` | 现有研究评审 |
| `README.md` | 本说明 |

`mac_composition.py` 从**自身所在目录**加载 `mac_research2.py`、`composition_plan.py`，这三个 `.py` 必须平铺在同一目录。

运行依赖：`tools/method88-venv/bin/python`（含 pyserial）+ 同目录 `mac_research2.py`(SerialConsole) / `composition_plan.py`；板卡侧默认 `/usr/bin/idma_composition`、`/dev/idma0`。

## 1. SD 写卡（人工；Codex 不代做）

写卡脚本 `scripts/sd_card.py`（**已确认存在**；本次装审查过的 composition-v1 release）。它**只接受已登记的这张卡**：

- 整卡 7,817,134,080 字节、512 字节块、外置物理盘、`GUID_partition_scheme`；
- 第一分区：32 MiB、偏移 1 MiB、LBA 2048–67583、类型 `7412F7D5-A156-4B13-81DC-867174929325`、UUID `46298C61-BAC2-44C9-A8FF-8FA61CF2BEE0`；
- 第二分区：UUID `0AF525DD-76E6-44E8-A5CD-4A66DC5697B8`，保留不动。

这些 UUID 是这张卡的固有身份，用于核对并**继续沿用**；任一项不符即拒绝。写入对象是**整卡原始第一分区**（payload 后补零到 32 MiB），**不是普通 FAT 文件拷贝**，第二分区不写。

```sh
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
diskutil list
```

从 `diskutil list` 读出整卡编号，把下面的 `/dev/diskN` 换成实际值（例如 `/dev/disk5`）。本次实际编号未知，**必须替换，不得沿用旧编号**。

```sh
sudo tools/method88-venv/bin/python scripts/sd_card.py update \
  --disk /dev/diskN \
  --release composition-v1-20261004 \
  --skip-payload-hash --skip-readback
```

写完安全弹出：

```sh
diskutil eject /dev/diskN
```

- `--skip-payload-hash`：`DELIVERY.json` 里 `fw_payload.bin` 的 sha256 为 null，按用户要求跳过，不设为阻塞项。
- `--skip-readback`：按用户要求跳过写后回读。
- 本批显式选择 `update` 与 `composition-v1-20261004`，不要求下载哈希、回读或解包。

## 2. 上板（Codex 通过 ssh 到 Mac 执行；SSH 目标是 Mac，不是板上的 Linux）

1. SD 插入**断电的** Genesys 2，接好串口与 JTAG，再上电。
2. 先做串口采集（记录启动输出）。
3. 加载一次 BIT：

```sh
/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 'tools/composition-v1-20261004/ariane_xilinx.bit'
```

4. 等 Linux 起到 root shell；保存启动日志并关闭临时串口采集，再启动下方 runner，避免串口同时被占用。

加载 BIT（flash）与 attach runner 是两步，**不写进同一自动函数**。

## 3. 运行全部 172（Codex 执行）

```sh
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
tools/method88-venv/bin/python tools/composition-v1-20261004/mac_composition.py \
  --run \
  --through-phase explore \
  --out results/composition-v1-20261004-01 \
  --expected-kernel '6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty'
```

- 不用先跑 `--plan`：`--run` 会先生成完整 `plan.json` 再执行，不要为看计划单独跑一次。
- `--through-phase` 默认 `short`（只 16）；要跑全 172 必须 `explore`。直接一次 `explore`，不要先 `short` 再全量（会重复 short）。
- `--expected-kernel` 用本次实际 release `6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty`。runner 先 `uname -r`，不等即在任何用例前停止。
- `--out` 必须**不存在**（`results/composition-v1-20261004-01`）；已存在会直接拒绝，不覆盖、不 resume、不自动重跑。
- 失败即停：技术性失败写 `STOPPED.txt` 并停止，不自动重跑。科学阴性（fast==0、miss==0、P/PN 无收益）不算技术失败，不因此停止。
- 串口被占用时用只读 `lsof` 检查并拒绝，不杀进程。
- runner 只 attach：不写 BIT、不写 SD、不重启。

## 4. 结果回传

完成与失败的原始结果**全部**回传 Dell（本批目的地，不覆盖已有数据）：

```
xvhaocheng@dell:/home/xvhaocheng/work/iommu-platform/out/mac-composition-v1-20261004-01/
```

跑完 Linux 保持运行，不关机 / 不重启 / 不重加载 BIT / payload。

## 5. 状态

构建状态：Linux已完成；BIT待构建完成后更新。尚未上板。

## 6. 一句话操作清单

1. 人工：`diskutil list` → 替换 `/dev/diskN` → `sd_card.py update`（含 `--skip-payload-hash --skip-readback`）→ `diskutil eject`。
2. SD 插入断电板卡，接串口/JTAG，上电；Codex 串口采集后加载 BIT，等 root shell。
3. Codex 一次 `--through-phase explore` 跑完 172。
4. 结果（含失败）回传 Dell；Linux 继续运行。
