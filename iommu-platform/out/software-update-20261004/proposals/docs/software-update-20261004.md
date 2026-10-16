# 软件更新 2026-10-04 操作指南

## 状态总览
- 已实现（软件/配置）：`platform-sd-update`、`platform-net-up`/`tests/board-net-up.sh`、`tests/board-fetch-update.sh`、`scripts/mac_update_reboot.py`、内核 `-cpn2-net1` 交付、1G full 以太网驱动等。
- 上板状态：待验证，未上板。以软件构建状态和验证记录为准，尚未上板。
- 作者/责任：DeepSeek 起草；Codex 复核并负责真实测试；最终结果由 Codex 更新。
- 当前启动为 initramfs RAM，raw boot 分区未挂载；完整 worker/test 日志在 `out/software-update-20261004` 下。

## 范围与安全
- 只更新 `/dev/mmcblk0p1` 这个 32 MiB boot 分区，不是整张 SD 卡。
- 分区尾部剩余空间写零；GPT、固定 type/UUID、其他分区不变，由 updater 校验；这些目前是软件/mock 测试结论，上板未验证。
- updater 固定校验的 boot 分区 type GUID 为 `7412F7D5-A156-4B13-81DC-867174929325`，unique GUID 为 `46298C61-BAC2-44C9-A8FF-8FA61CF2BEE0`。
- 无 A/B/自动回滚；更新中断或坏更新可能需要手动 SD 恢复。
- 失败即停止，不自动重试、不自动重启。
- 本文不新增用户审批要求，也不要求执行本指南之外的外部动作。

## 已实现的软件/配置
- `platform-sd-update`：root 运行；拒绝符号链接；目标必须是 `/dev/mmcblk<数字>p1`；校验 `BLKGETSIZE64` 为 32 MiB；sysfs `partition=1`、`start=2048`、`size=65536`；512B 逻辑扇区；只读校验 GPT；拒绝已挂载；`flock` 独占；先把镜像读入内存再打开可写描述符；只写分区；`fsync`；不自动重启。
- `platform-net-up` / `tests/board-net-up.sh`：只配置命令行指定的一个接口；静态地址；默认 dry-run，只有 `--apply` 才改状态；不启动 DHCP。
- `tests/board-fetch-update.sh`：只接受 `http://`；先下载到 `.partial`；成功后默认 dry-run；`--write` 会重新下载并跳过手动 hash 对比，因此建议用下面的显式两段式。
- `scripts/mac_update_reboot.py`：默认 dry-run；`--run` 必须显式给 `--port`、`--bit`、`--expected-kernel`、`--out`；通过 UART 确认 shell 存活，用 JTAG 加载 BIT，等待登录，读取 `uname -r` 和 `/proc/sys/kernel/random/boot_id`；失败停止，不重试。
- 内核：`LOCALVERSION=-cpn2-net1`，实际内核 release 从 `delivery/kernel-release` 读取，不要硬编码 hash。
- 以太网：第一版驱动只支持 1G full；`lowrisc_ethernet.c` 按实际 RTL 对齐 64 buffer、NAPI、phylib MDIO addr 1、timer TX、软件 RX FCS 检查/移除。
- 构建器待修订版本会交付 `linux.config`、`kernel-release`、`mac_update_reboot.py`，并要求 SD/PHY/ETH 相关配置为 `y`。

## 硬件限制与外部 JTAG 重启路径
- 当前硬件/DT 没有原生 CPU-driven reset 路径；OpenSBI 的 `fdt_reset_init()` 只注册 DT 中实际可用的 reset 设备，当前 DT 没有，因此现阶段不能声明原生 reboot 可用。
- 现阶段重启靠外部 JTAG：保持 JTAG 和串口连接，用 Mac helper 重新加载同一个已验证 BIT，从 SD 卡重新启动。
- Mac helper 还有待修订项：no-newline prompts、`sync && echo`、before/after UUID 必须变化。
- 不要盲目执行普通 `reboot` 命令。

## 第一次上板前：手动安装 raw payload
- 使用现有 Mac 写卡流程，手动选择正确的 SD 卡和 raw 32 MiB boot 分区。
- 不要整卡格式化，不要自动磁盘选择，不要提供或使用猜测 Mac 盘符的通用 `dd`。
- 这一步由人操作；本 worker 不能执行或确认上板结果。

## 上板网络示例（1Gbps）
- Mac 和板子使用 1Gbps 网卡/链路。
- 在 Mac 上手动给正确的以太网适配器设 `192.168.77.1/24`。
- 在板子上先 dry-run：
```
platform-net-up eth0 --addr 192.168.77.2/24
```
确认无误后再实际应用：
```
platform-net-up eth0 --addr 192.168.77.2/24 --apply
```
- 注意：现有 rootfs 的 `ip` 可能是 BusyBox 版本，`ip -o` 兼容性需要关注；当前脚本使用 `ip -o addr show`，测试只在 mock 下检查，尚未上板运行。
- 板子基础检查：
```
ip link show eth0
cat /sys/class/net/eth0/carrier
dmesg | grep -Ei 'mmc|mmcblk'
cat /sys/class/block/mmcblk0p1/partition
cat /sys/class/block/mmcblk0p1/start
cat /sys/class/block/mmcblk0p1/size
```
期望 `partition=1`、`start=2048`、`size=65536`（512B sector）。已有 GPT 的固定 type/UUID 由 updater 校验。

## Mac HTTP 服务
```
cd /path/to/out/software-update-20261004/delivery
python3 -m http.server 8000 --bind 192.168.77.1
```
- 只在 delivery 目录运行，不要把私有文件或目录暴露到网络。

## 板子下载与写 SD 卡
先默认 dry-run，只下载一次：
```
platform-fetch-update http://192.168.77.1:8000/fw_payload.bin
```
它会下载到 `/tmp/fw_payload.bin`，然后做 dry-run 检查。

在 Mac 端取 manifest/SHA256，在板子端对比：
```
shasum -a 256 /path/to/out/software-update-20261004/delivery/fw_payload.bin
sha256sum /tmp/fw_payload.bin
```
如果 manifest 有对应 SHA256，以 manifest 为准。必须一致才写。

显式写入，不要用 `--write` 重下载并跳过手动 hash：
```
platform-sd-update --device /dev/mmcblk0p1 --image /tmp/fw_payload.bin
```
等待输出 `PASS`，并确认 fsync 完成。失败即停止，不自动重试、不自动重启。

## 外部 JTAG 重启验证
先 dry-run（不带 `--run`）：
```
python3 scripts/mac_update_reboot.py --bit /path/to/SAMEKNOWNBIT.bit --port /dev/cu.usbserial-XXXX --expected-kernel "$(cat delivery/kernel-release)" --out fresh_reboot_dry_20261004
```
确认参数后再执行：
```
python3 scripts/mac_update_reboot.py --run --bit /path/to/SAMEKNOWNBIT.bit --port /dev/cu.usbserial-XXXX --expected-kernel "$(cat delivery/kernel-release)" --out fresh_reboot_20261004
```
- 以上是安全示例字符串，本次未执行。
- 保持 JTAG 和串口连接。失败停止，不自动重试/重启。

## 启动后验证
- `uname -r` 必须等于 `cat delivery/kernel-release`。
- 读取新的 `/proc/sys/kernel/random/boot_id`。
- 用 `platform-net-up` 重新配置板子 IP；当前文件不会跨重启持久化静态 IP。
- 校验更新后的 SD 读回 SHA 与同一镜像范围一致；使用只读方式，不要破坏性 `dd`。
- 不要盲目普通 `reboot`。

## 第一次上板验收清单
1. SD 枚举与 `platform-sd-update --dry-run`。
2. PHY link 和 ping 通。
3. 已知 payload 的 SHA256 一致。
4. 只写同一个已知可用 payload。
5. 用外部 JTAG 重载同一已验证 BIT 重启。
6. 只有真实 PASS 后，AI 远程更新才可考虑投入运行。

## 验证状态
以软件构建状态和验证记录为准，尚未上板。
Codex 更新最终结果前，本文不写 PASS 数量或未确认 hash。
