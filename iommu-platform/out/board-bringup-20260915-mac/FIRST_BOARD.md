# 首次上板记录：2026-09-15

## 已验证

- Mac 识别到 Digilent Adept USB Device，序列号 `200300BD5148`；FT232R USB UART，序列号 `AV0KRCHT`。
- JTAG IDCODE `0x03651093`，芯片 `xc7k325t`，符合 Genesys 2。
- UART 使用 `/dev/cu.usbserial-AV0KRCHT`，115200、8N1、无流控；JTAG 对应的两个串口候选不作为 Linux 控制台。
- 固定 migration-4dma BIT（DS 50/50）在 **1 MHz** JTAG 频率下配置完成。STAT `0x501079fc`，DONE/EOS/GWE 均为 1，CRC/ID error 均为 0。
- Boot ROM 输出 `Hello World!`、SPI/SD 初始化完成，读出与已更新卡一致的 GPT 第一分区（LBA `0x800`～`0x107ff`）及分区 UUID，约数分钟后完成 32 MiB 启动区加载。
- OpenSBI 启动并进入本次更新的 Linux `6.5.0-rc2-00078-g031e91dbadc1`；日志显示 4 个 iDMA 设备注册。但 Linux 登录失败，不能记为可用系统验收通过。

## 当前阻塞：Linux 内核异常，无法登录

本次串口观测顺序：

1. Linux 启动约 21 秒时出现 `Dentry ... still in use`、`umount_check` 和 `VFS: Busy inodes after unmount of proc`，调用栈涉及 `proc_kill_sb`、`proc_flush_pid`、`release_task`。
2. 随后 `cat`（PID 70）、`dhcpcd`（PID 88）各触发一次 NULL pointer dereference，位置均为 `generic_permission+0x1c/0x1b2`，日志中的 `badaddr` 为 `0x60d`。
3. 出现 Buildroot 登录提示后，仅输入一次 `root`；`login`（PID 95）触发第三次同位置 Oops，调用栈涉及 `do_readlinkat` / `sys_readlinkat`。返回登录提示，没有获得可用 root shell。

因此本轮结论是 **FPGA 配置、SD 加载及进入 Linux 内核已验证；Linux 可用性验收失败**。smoke、DMA 实验和 E0/HPM 校准均未运行。串口采集已正常结束，不再占用端口。

已做有限只读核对：交付 rootfs 的 `/init`、`etc/inittab` 为常规 Buildroot 启动流程；dell 上 Linux 工作树干净，并核对了 `umount_check` 对应源码。这些检查尚不能定位根因，不能据此认定 SD 损坏、rootfs 打包错误或 FPGA 硬件错误。下一步需要结合本日志在 dell 定位内核异常；本轮没有修改服务器源码、重建或刷入其他版本。

## 下载频率的实际记录

默认 6 MHz 首次传输返回 0，但 IR DONE=0，STAT 先为 `0x500c198c`，后为 `0x5018198c`，未看到启动文本。只改变 JTAG 频率为 1 MHz 重试后，IR DONE=1，独立 STAT 检查通过并输出 Boot ROM 日志。

这说明当前组合至少在 1 MHz 可用；尚未定位 6 MHz 的具体失败原因。没有修改 BIT、写 Flash 或重建 FPGA。

`scripts/fpga.py` 已默认使用 1 MHz，并新增 `status` 命令及配置完成状态检查。若有 STAT 输出但 DONE/EOS/GWE 未就绪，记录 `startup_incomplete` 并返回 2，避免把工具退出码 0 当作配置成功。8 项相关离线测试通过。

## 证据位置

- 识别：`results/jtag/20260915-233311-detect-9s73j3xf/`。
- 6 MHz 下载：`results/jtag/20260915-233348-program-oww1gsel/`（当时旧脚本只记录工具返回 0，须结合 output.log 的 DONE=0 解读）。
- 1 MHz 下载：`results/jtag/20260915-233744-program-lthwbt0o/`。
- 1 MHz 独立 STAT：`results/jtag/20260915-233931-status-kvcoq9y_/`。
- 原始串口日志：`results/serial/20260915-233330-console-gv7d1io6/console.raw.log`。前缀包含第一次失败配置前的 62 个 NUL 字节及四个点，随后是成功重试的启动文本；原始字节保留。
- 串口日志共 27,543 bytes，SHA256 `6958a69754dc52f0e00b2a4c0f928a0ce9378cfcdf9cea19114bfaf25a1c1226`；会话状态 `capture_finished`。少量长行在接收内容中已有截断，未补写或重构调用栈。
- 本次验收记录：`results/experiments/20260915-225059-run-i8uj8bql/run.json`，状态 `linux_login_failed`，未运行的测试字段保持空值。

## 指示灯

LD15 是 power-good 电源正常指示灯，LD14/DONE 是配置完成指示灯。LD15 亮不能单独证明 FPGA 配置完成或 Linux 已启动。说明依据：[Digilent Genesys 2 手册](https://digilent.com/reference/_media/reference/programmable-logic/genesys-2/genesys2_rm.pdf)。

## 后续验收

修复并重新验证 Linux 可用登录后，才运行 smoke，随后进行 E0/HPM 校准；各项须分别记录，不能由配置完成直接推定。
