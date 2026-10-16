# 8 GB 启动卡：检查、备份、更新与恢复

本流程更新现有原始启动分区，使用 `scripts/sd_card.py`。卡的身份和布局已登记；磁盘编号可能随重新插入而变化。

## 本次实际状态

**2026-09-15 已完成更新并安全推出 SD 卡。** 在用户已授权的系统终端运行同一更新脚本，状态为 `readback_verified`，新版 payload 和完整 32 MiB 启动区回读均通过。主/备 GPT CRC 正常，磁盘首尾保护区与写前一致，第二分区未被写入。FPGA 实际启动仍待验证。

- 完整备份及记录：`results/sd/update-6curphb9/`。
- 旧启动区 SHA256：`7cefdfde8eb6707aa20f5d0900eef8c8729a34ef760fb4c3c434af4989b5e97b`。
- 新 32 MiB 启动区（payload 后补零）SHA256：`90dcd0bd70cd55a6679d01d5154cdd6c758a34822ad0ca5930a21f1eb0b408ba`。
- payload 回读 SHA256 与下表固定值一致。旧备份归档后也再次校验通过。

此前后台管理员进程读取 `/dev/rdisk5` 被 macOS 拒绝，失败记录保留供排错；那些尝试均未写卡。此机器请优先在 macOS 自带“终端”执行 sudo 命令。若以后遇到 `Operation not permitted`，检查“系统设置 → 隐私与安全性 → 完全磁盘访问权限”中终端的授权。管理员密码与应用磁盘访问权限是两层控制。参考 [Apple 文件访问控制说明](https://support.apple.com/en-ca/guide/security/secddd1d86a6/web)。

## 已核对的目标与启动文件

| 项目 | 值 |
|---|---|
| 本次识别的整卡 | `/dev/disk5`，USB 外置物理卡 |
| 整卡容量 | 7,817,134,080 字节，标称 8 GB |
| 第一分区 | 32 MiB，偏移 1 MiB，LBA 2048～67583 |
| 第一分区 UUID | `46298C61-BAC2-44C9-A8FF-8FA61CF2BEE0` |
| 第一分区类型 | `7412F7D5-A156-4B13-81DC-867174929325` |
| 第二分区 UUID | `0AF525DD-76E6-44E8-A5CD-4A66DC5697B8` |
| 第二分区 | Linux Filesystem，7,782,514,176 字节；保留 |
| 新启动内容 | 已校验的 `fw_payload.bin`，29,363,656 字节 |
| payload SHA256 | `c596f516b33e54f25bd62dec76ae00eccfb02939a4cf3029b044db689b0a3776` |

项目 Boot ROM 说明使用上述 32 MiB 第一分区。`gpt.c` 读取 GPT 第一项，将整个分区读入 DDR；`main.c` 指定加载并跳转到 `0x80000000`。OpenSBI 的 `FW_TEXT_START` 同为 `0x80000000`，构建脚本将 lloader、Linux 和设备树装入 `fw_payload.bin`。新版 Linux 内置 rootfs，因此更新范围是第一分区。

本轮定向核对了服务器源码与 FPGA 导出目录：`bootrom_64.sv` SHA256 均为 `4f36641d7672d98a5a7bb7daf5a215bcd57935c687566e34e66501f1fd28bf13`，`main.c` 和 `gpt.c` 的对应哈希也一致。这些是源文件/构建入口的核对，实际启动仍需上板验证。

## 命令

在当前工作文件夹打开终端。重新插卡后先运行 `diskutil list`，将下面的 `/dev/disk5` 替换为实际整卡编号。脚本还会核对容量、两个分区的 UUID、偏移和大小，避免只凭编号选择目标。

仅预览更新计划（不会打开原始设备写入）：

```sh
python3 scripts/sd_card.py update --disk /dev/disk5 --dry-run
```

检查 GPT、备份原始启动区并记录旧内容标记（仅从卡读取）：

```sh
sudo /opt/homebrew/bin/python3 scripts/sd_card.py inspect --disk /dev/disk5
```

在获得磁盘访问权限的系统终端中，备份并更新启动分区：

```sh
sudo /opt/homebrew/bin/python3 scripts/sd_card.py update --disk /dev/disk5
```

备选系统认证窗口入口（启动它的应用同样需要磁盘访问权限）：

```sh
python3 scripts/run_sd_admin.py --disk /dev/disk5
```

本机上述后台管理员方式曾被拒绝，已经实测成功的是系统终端方式。也可用 `osascript scripts/update_sd_terminal.applescript` 打开新终端并填入本次 `/dev/disk5` 更新命令；仍由 Python 脚本核对卡身份。磁盘编号改变时，应使用带实际 `--disk` 参数的命令。

系统要求管理员密码时在系统窗口或自己的终端输入。终端密码输入通常不显示字符；无需把密码发送到对话。

`run_sd_admin.py` 先完成预览，再将脚本和已校验的软件复制到 `/private/tmp/genesys-sd-*`，通过系统认证运行同一个更新脚本，最后将备份和记录复制回当前工作文件夹。这样管理员进程只访问临时工作区和 SD 设备，无需读取受 macOS 隐私保护的桌面目录。授权与归档日志在 `results/sd-admin/`，临时副本保留供排错。取消认证不会启动写卡。

每次操作在 `results/sd/<操作>-<唯一编号>/` 保存：

- `boot-before.bin`：原先的完整 32 MiB 第一分区。
- `disk-head-1MiB.bin`、`disk-tail-1MiB.bin`：磁盘首尾保护区，包含主/备 GPT。
- `status.json`：设备身份、起止时间、备份 SHA256、写入状态和回读结果。

更新先卸载分区并复核身份，检查主/备 GPT CRC，完成本地备份并校验后才写第一分区。新 payload 后面的剩余空间填零，得到确定的 32 MiB 启动区内容。写入后同步并重新打开原始设备，核对完整分区及 payload 的 SHA256，比较磁盘首尾保护区与写前是否一致。第二分区和整盘设备不会被打开用于写入；首尾比较不是对第二分区全部内容做哈希。

`status: readback_verified` 表示写卡与回读完成，不等于 FPGA 已启动。若操作中断或验证失败，保留目录中的旧分区备份，停止后续启动步骤并排查；脚本不会自动重试写入。

## 恢复原启动区

只在需要回退时运行，`--backup` 填原更新操作的备份目录：

```sh
sudo /opt/homebrew/bin/python3 scripts/sd_card.py restore --disk /dev/disk5 \
  --backup '/此处填写原备份目录的完整路径'
```

恢复校验备份的大小/哈希和目标 UUID，再备份当前第一分区，恢复原 32 MiB 内容并回读校验。此命令不修复已损坏的 GPT，也不是整卡恢复工具；若身份或 GPT 校验失败会停止。

本次可用于回退的备份目录为 `results/sd/update-6curphb9/`；需要回退时将其完整路径传入 `--backup`。不要选择此前只有失败状态、没有 `boot-before.bin` 的目录。

## 写卡后

完成后安全推出 SD 卡：

```sh
diskutil eject /dev/disk5
```

将 SD 卡插入断电的 Genesys 2，接好电源、JTAG 和串口。按主 README 先开启串口采集，再加载 `.bit`，观察启动输出。Linux 启动确认后运行 `/usr/bin/platform-smoke` 并保存日志，再做 E0/HPM 校准。
