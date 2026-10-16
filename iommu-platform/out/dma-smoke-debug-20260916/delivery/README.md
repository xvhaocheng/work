# DMA smoke 诊断版交付（2026-09-16）

用途：修复 smoke 的 timeout 依赖；发生数据错误时保存首错位置、刷新前后值和地址，定位 CPU 缓存视图或真实搬运错误。**数据故障尚未修复或重新上板验证；缓存刷新后读对仍返回失败。**

本目录的新 `fw_payload.bin` 包含诊断内核、rootfs 和设备树。Linux 版本为 `6.5.0-rc2-00079-g53d3113dee74`，源码 `53d3113dee74761515fc08b6a4f984d41227c50f`。主机测试、完整软件构建及产物校验通过，见 `host-checks.json` / `software-manifest.json` / `delivery.json`。

## Mac 上板任务下一步

1. 下载本目录的 `fw_payload.bin` 和 `SHA256SUMS`，核对文件哈希；只下载一项时用 `shasum -a 256 fw_payload.bin` 与清单对应行比较。
2. 按 Mac 已经确认的 SD 原始分区更新流程替换 Linux payload，再回读核验。沿用原布局；本目录不是整盘 SD 镜像，不把所有文件复制到卡就当作更新完成。
3. 保持正在使用的 SC.W 修复 BIT（SHA256 `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3`）。本轮没有新 BIT，不需要换回旧 migration-4dma BIT。
4. 从 SD 重新启动新版 Linux，先只做下面的一次 DMA0 Solo 诊断，保存完整串口及首错内核行，不继续性能或双路矩阵。

在 FPGA Linux 终端执行：

```sh
uname -a
/usr/bin/platform-timeout 120 /usr/bin/idma_experiment \
  --device /dev/idma0 --aggressor none --victim-pages 1 \
  --chunk 4096 --transfers 8 --warmup 32 --timeout-ms 10000
rc=$?
echo "DMA diagnostic exit=$rc"
dmesg | tail -80
```

需要保留 `experiment data mismatch:` 整行。`src` / `dst` 是刷新前字节，`*_after_fence_i` 是刷新后字节；都与 `expected_*` 比较。`*_pte_pa` 仅是软件页表查询，不是硬件 IOTLB。一次刷新后变化只是定位线索，不能替代最终修复和复测。

`platform-timeout` 只对直接子进程发送 TERM；正常/失败返回原退出码，超时返回 124。它不取消硬件 DMA，也不能恢复不可中断的内核卡死。

服务器详细说明：`/home/xvhaocheng/work/iommu-platform/DMA_SMOKE_FIX.md`。原失败镜像仍保留在 `out/experiment-v1/`，这个新目录与它分开保存。

完整下载本目录后可运行 `sha256sum -c SHA256SUMS`（Mac 为 `shasum -a 256 -c SHA256SUMS`）。
