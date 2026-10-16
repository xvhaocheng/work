# composition-v1 交付与上板运行说明（2026-10-04）

## 0. 先讲清楚：一次写 SD + 一次加载 BIT，之后四模式运行时切换

本交付在硬件上只做两个一次性动作：

1. 人工把新的 `fw_payload.bin` 写入 SD 卡的**第一个 GPT 分区**（原样覆盖启动分区，只写一次）。
2. 加载一次新的 `ariane_xilinx.bit`，让 Linux 起来（只加载一次）。

Linux 起来之后，四个模式 **S / N / P / PN**（mode 0 / 1 / 2 / 3）都在**同一个 BIT、同一次 boot** 内，由驱动通过 ioctl 0x11 写 live config（offset 2）在**运行时**切换。切换不需要重写 SD、不需要重新加载 BIT、不需要重启。

所以：SD 与 BIT 一次性准备好，四模式是运行时的；全部 172 个用例在一次启动内跑完。

## 1. 交付文件（平铺到 `/Users/jojo/Desktop/在mac链接pfga(codex)/tools/composition-v1-20261004`）

| 文件 | 说明 |
|---|---|
| `ariane_xilinx.bit` | FPGA bitstream（只加载一次） |
| `fw_payload.bin` | 写入 SD 第一分区的 payload（只写一次） |
| `mac_composition.py` | Mac 主机 attach-only runner（串口帧封装 + 计划 + 校验） |
| `composition_plan.py` | 冻结的 172 用例计划与记录校验（被 runner 导入） |
| `mac_research2.py` | 提供 `SerialConsole`（被 runner 复用） |
| `README.md` | 本说明 |

注意：`mac_composition.py` 运行时会从**自身所在目录**加载 `mac_research2.py`、`composition_plan.py`，所以这三个 `.py` 必须平铺在同一目录。

## 2. 依赖（只做识别，不额外验证）

- Python：`/Users/jojo/Desktop/在mac链接pfga(codex)/tools/method88-venv/bin/python`
- pyserial：`mac_research2.py` 内 `import serial`（SerialConsole 用到）
- openFPGALoader：`/opt/homebrew/bin/openFPGALoader`（BIT 加载步骤用，不属于本 runner）
- runner 需要 `mac_research2.py` 暴露 `SerialConsole`，且具备 `start`/`close`/`run_shell`/`drain_input`/`disable_echo`
- 板卡侧默认值：板卡 CLI `/usr/bin/idma_composition`、设备 `/dev/idma0`、`/proc/sys/kernel/random/boot_id`

## 3. SD 卡：payload 写到哪个分区、怎样更新

依据旧 Mac SD 流程（`out/paper-route-review-20261004/mac-sd-card.md`）：

**写入目标（GPT 布局）**
- **整卡的第一个 GPT 分区**：32 MiB，偏移 1 MiB，LBA 2048–67583。
- 该分区的分区类型 GUID：`7412F7D5-A156-4B13-81DC-867174929325`。
- 旧登记卡该分区 UUID：`46298C61-BAC2-44C9-A8FF-8FA61CF2BEE0`（用于身份核对，不作为本次可沿用值）。
- **第二分区（Linux Filesystem，约 7,782,514,176 字节）保留，不写入。**
- 原因：Boot ROM 读 GPT 第一项，把整个第一分区读入 DDR 并跳转 `0x80000000`；OpenSBI 的 `FW_TEXT_START` 也是 `0x80000000`，构建把 lloader/Linux/dtb 一起打进 `fw_payload.bin`。因此新 payload 就是原样覆盖第一分区（payload 后补零到 32 MiB）。

**怎样更新（旧流程做法）**
- 重新插卡后先 `diskutil list`，确认**真实整卡编号**（不要沿用旧编号）。
- 用旧流程的写卡脚本以 update 方式写入第一分区：它先卸载分区、复核身份（容量/UUID/偏移/大小）、校验主备 GPT CRC，备份第一分区后才写；写入后同步。
- 写卡完成后 `diskutil eject /dev/diskN`。

**不得沿用 / 不得假设**
- **不得沿用旧 disk 号 `/dev/disk5`**；编号会随重新插入变化。
- 不得沿用旧 payload SHA256、旧备份目录（`results/sd/update-6curphb9/`）、旧日期。旧文件里的值都属于旧包，不适用新包。
- 本任务不要求下载哈希、不要求写后回读、不要求解包验收。

**缺信息（需确认，不在此杜撰）**
- 本次交付包平铺文件里**没有写卡脚本**（旧流程的 `scripts/sd_card.py`）。它在本机是否仍存在、路径如何，需要确认。
- 本次 SD 整卡的**实际 disk 编号未知**，必须先 `diskutil list`。
- 本次实际 SD 卡的容量 / 两个分区 UUID 未在本证据中给出；需在写卡前核对。

## 4. 上板前的一次性动作

1. Mac 上 `diskutil list` 确认真实整卡编号。
2. 将 `fw_payload.bin` 写入第一分区（见 §3），同步后 `diskutil eject`。
3. SD 插入**断电的** Genesys 2，接好串口与 JTAG，再上电。
4. 加载 BIT（一次性，由 Codex 执行）：`openFPGALoader -b genesys2 --freq 1000000 <ariane_xilinx.bit>`（参考 `mac_research2.py` 里 flash 的调用形式；这不是本 runner 的动作）。等待 Linux 起 root shell。

## 5. 运行全部 172（实际命令）

先出计划（不会打开串口）：

```sh
cd /Users/jojo/Desktop/在mac链接pfga(codex)/tools/composition-v1-20261004
/Users/jojo/Desktop/在mac链接pfga(codex)/tools/method88-venv/bin/python mac_composition.py \
  --plan \
  --out ./results/plan-20261004 \
  --expected-kernel '6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty'
```

正式跑全部 172（short → noise → explore，按序门控）：

```sh
/Users/jojo/Desktop/在mac链接pfga(codex)/tools/method88-venv/bin/python mac_composition.py \
  --run \
  --out ./results/run-20261004-01 \
  --expected-kernel '6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty' \
  --through-phase explore
```

要点：
- `--expected-kernel` 取自本次构建状态：`kernel_release = 6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty`（**不是**旧 P4M 的 `6.5.0-rc2-00089-...`）。runner 先 `uname -r`，不等于该值即在任何用例前停止。
- `--through-phase explore` 才会跑完整 172。**默认是 `short`（只 16）**：不要先用默认 short 跑一遍、再另外起一次全量——那样会把 short 跑两次。应当直接一次 `--through-phase explore` 跑完（runner 内部按 short→noise→explore 顺序执行，short/noise 技术失败会立即停止、不会进入 explore）。
- `--out` 目录必须**不存在**；已存在会直接拒绝，因此不会覆盖、不会 resume、不会自动重跑。要再来一轮必须换新的结果目录。
- 失败即停：任意用例出现技术性失败（退出码、taint、boot_id 变化、记录校验不通过等）会写 `STOPPED.txt` 并停止，**不自动重跑**。
- 科学阴性（fast==0、miss==0、P/PN 无收益）不算技术失败，不会因此停止；探索本身不等于确认。
- 若串口被占用，runner 用只读 `lsof` 检查并拒绝，不会杀进程。

## 6. 结果与回传

- 结果目录内容：`plan.json`、`manifest.json`、`boot.json`、`run.log`、`console.raw.log`、`console.events.log`、`cases/<id>/{record.raw.json,record.json,metrics.json,meta.json,stderr.txt}`、`dmesg.*.txt`、`summary.json`；技术失败时另有 `STOPPED.txt`。
- **完成与失败的原始结果全部回传 Dell**：Dell `~/work/iommu-platform/out/` 下独立目录，保留既有数据、不覆盖不同内容；失败、无触发、不可比样本一并回传，不只回传成功摘要。
- **Linux 保持运行**：跑完不关机、不重启、不重新加载 BIT/payload，便于继续实验。

## 7. 局限与未完成（不得声称上板通过）

- FPGA **仍在编译**：`ariane_xilinx.bit` 的最终性未在证据中确认，**板测尚未开始**，本文件不声称任何上板通过。
- Linux 构建状态为 complete（软件状态是证据），但「构建成功」≠「板上启动成功」。
- 未提供新的 BIT 时序或上板验收；不得编造。
- 五轮评审当前实施状态：关键 RTL/新 ioctl 已写入工作树，BIT 与 Linux 本批尚未完成编译，板测未开始。
- 探索性质：单 boot、单 FPGA、单 IOMMU；explore 非确认；n=3 不构成显著性或发表依据。
- 本 runner 是 attach-only：不写 BIT、不写 SD、不重启。

## 8. 用户操作清单（一句话版）

1. 确认已写好 SD（第一分区，见 §3）并插入断电的 Genesys 2。
2. 确认串口线、JTAG 已连，上电。
3. 等 Codex 加载一次 BIT、Linux 起 root shell。
4. 由 Codex 执行 §5 的 `--through-phase explore` 一次跑完 172。
5. 结果（含失败）回传 Dell；Linux 继续运行。
