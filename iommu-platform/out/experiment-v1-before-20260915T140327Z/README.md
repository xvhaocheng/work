# 实验 v1 软件交付

软件构建与主机控制逻辑测试通过；尚未上板运行或完成 HPM 校准。

- `fw_payload.bin`：OpenSBI 加载 lloader、Linux 和设备树的组合产物；Linux 内置新版 rootfs。
- `Image`、`platform.dtb`、`rootfs.cpio`、`linux-rv64-cva6.bin`：同时保留各阶段产物，便于核查。
- `idma_experiment`、`platform-smoke`：已包含在 rootfs 中；独立副本用于对照。
- `software-manifest.json`、`sources.lock.json`、`delivery.json`、`build.log`：构建版本、配置、校验与日志。

这些文件保留构建时的名称。当前没有 SD 卡，尚未确认板上启动器要求的卡布局；本目录不是可直接整盘写入的 SD 镜像，也不表示所有文件都要拷入卡。准备介质时按实际启动器确定加载方式。

硬件逻辑本轮没有修改。沿用 migration-4dma 的 DS 50/50 bitstream，原位置和 SHA256 在 delivery.json 中；它不是 DS 0/0 正式基线。

新版启动后先运行 `/usr/bin/platform-smoke` 并保存串口日志，检查设备、数据错误和计数归属；命令成功还不等于 HPM 校准或请求重叠成立。

在本目录运行 `sha256sum -c SHA256SUMS` 可验证交付文件。指标含义见平台 `docs/experiment-v1.md`。
