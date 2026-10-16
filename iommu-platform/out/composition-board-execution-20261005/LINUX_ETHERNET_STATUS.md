# 2026-10-05 Linux 百兆网口更新已跑通

**实际完成：Mac 网传 payload → FPGA Linux 只写 SD 的32MiB第一分区并同步 → 1MHz JTAG重载相同旧BIT → net2 Linux从SD启动。SD全程未拔，未改RTL/BootROM，未运行实验。**

- 当前内核：`6.5.0-rc2-cpn2-net2-00093-gc0f5e13ea81f-dirty`；boot_id `f7e78b10-b4a5-44b5-9114-10d0aa21251a`。[实际最终启动](ethernet-update-20261005-net100-final-boot/boot-state.json)，JTAG exit0，Linux身份命令/net-up均exit0，carrier1，Linux保留运行、UART释放。
- Mac网卡：USB 10_100 LAN，VID:PID0bda:8152，en11/192.168.77.1；板端eth0/192.168.77.2，100Mbps全双工。鼠标原USB口识别到该网卡，原USB口未枚举；不是Linux驱动未绑定。
- net1仅广告千兆；Linux驱动已放开100/1000全双工，现有RTL自适应速率/MII收发链保留，本轮实际网传证明旧N v5 BIT可用。临时32KB模块先经UART引导联网；net2采用内建修正驱动，后续无需再注入模块。
- [实际网传写卡回执](ethernet-update-20261005-net100-update-02/update-state.json)：29363656B全部下载；PASS写33554432B到`/dev/mmcblk0p1`（image29363656B、padding4190776B），同步成功，SSHexit0。完整输出同目录`ssh-update.raw.log`。传输及写卡总计24.34分钟，实际速度仍待优化，不把协商百兆当实际传输线速。
- 新软件交付：Dell `out/software-update-net100-20261005/delivery`；Mac `artifacts/software/software-update-net100-20261005/delivery`。复用原构建器，内核/固件构建exit0；未重编BIT。当前旧BIT为Mac `tools/nonblock-pilot-20261004/banked-hit-v5.bit`。
- UART曾有`ttyS0 input overrun`，丢字符后shell进入未闭合命令的`>`提示，Linux仍能SSH访问。不同串口不会因此相互抢字符；本轮无同端口多程序占用。最终改为4B/10ms分段发送后启动确认成功。模块引导程序最后dmesg命令exit1、首次无控制tty的SSH认证失败均保留，未抹成全流程一次通过。
- 当前每次启动仍由UART调用`platform-net-up`配置地址；永久拔UART之前需补自动静态IP并调整现有JTAG重启工具。普通reboot没有本次验证；本次按用户选择用JTAG。源位置：Linux在`linux/`，RTL在`cva6/`及其硬件组件，快照/日志/临时编译副本在`out/`。
- 原composition-v1恢复编译最终failed/exit1/gates0，WNS−3.268ns、无BIT/MCS；本轮未修复/重编它，172项未启动。

DeepSeek提取RTL百兆证据、起草并定向修订串口引导片段；Codex负责关键驱动、设备归属/解绑审查、实际执行与最终启动确认。原始回复、失败与成功记录全部保留。
