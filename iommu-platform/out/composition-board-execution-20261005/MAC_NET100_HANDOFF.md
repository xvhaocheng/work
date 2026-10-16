## 2026-10-05 最新：百兆网口更新Linux和板端SD写入已完成

实际net2内核 `6.5.0-rc2-cpn2-net2-00093-gc0f5e13ea81f-dirty`，boot_id `f7e78b10-b4a5-44b5-9114-10d0aa21251a`。Mac en11/192.168.77.1，板端eth0/192.168.77.2，100Mbps全双工/carrier1；Linux运行、UART释放。SD全程留在FPGA，未改RTL/BootROM，未运行实验。旧已通过BIT `tools/nonblock-pilot-20261004/banked-hit-v5.bit` 保持。

Linux驱动已开放100/1000全双工；32KB模块经UART先引导联网，随后系统SSH复用原run_research2_mac登录响应，执行platform-fetch-update --write：29363656B网传，32MiB第一分区/mmcblk0p1写入及同步成功、exit0。1MHz JTAG同BIT再加载，最终net2从SD实际启动。新交付：artifacts/software/software-update-net100-20261005/delivery；完整结果：results/ethernet-update-20261005-net100-update-02及results/ethernet-update-20261005-net100-final-boot。均已回Dell。

UART输入溢出曾损坏命令，最终4B/10ms分段成功；模块程序额外dmesg命令exit1与首次SSH无控制TTY认证失败保留。实际网传写卡exit0、最终启动exit0另行证明成功。传输+写卡约24.3分钟，速度仍需优化。当前开机仍需UART net-up；永久拔UART需补自动IP/调整重启工具，现有mac_update_reboot.py空密码假设不适用。原composition编译failed，无新BIT；172未跑。此状态覆盖更早cpn1/net1/等待编译快照。

DeepSeek整理RTL证据、起草修订模块上传片段；Codex负责关键驱动及实际执行/验收。临时HTTP服务完成后停止，Linux保留运行。
