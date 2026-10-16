# DMA 缓存同步版交付（2026-09-16）

本版补上当前 CVA6 平台在 DMA 开始前和安全结束后的缓存同步，并修复软件 IOVA→PA 查询的偏移拼接错误。主机回归、完整软件构建和产物校验通过；**修正版尚未上板，数据故障是否解决仍需复测。**

- 内核：`6.5.0-rc2-00080-g3b47ab1645e4`。
- Linux 提交：`3b47ab1645e412095b601e3be68b6347b9fad213`；构建顶层：`76de9ee73656cc90bdae5331be7020b5f905e205`。
- `fw_payload.bin`：29363656 字节，SHA256 `6f52ad85c4d2fc87e97301d98f1394216b843cd8be601b2965eca429caf74053`。
- 沿用 SC.W 修复 BIT，SHA256 `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3`。本轮无新 BIT，不能换回原来有 SC.W 错误的 BIT。

## Mac 操作顺序

1. 下载本目录的 `fw_payload.bin` 和 `SHA256SUMS`，用 `shasum -a 256 fw_payload.bin` 核对对应行。
2. 按已经确认的 SD 原始分区更新流程替换 payload，并回读核验；该文件不是整盘镜像。仅替换用户程序不能获得内核修正。
3. 配合相同 SC.W 修复 BIT，从 SD 重新启动。先检查 `uname -a` 是上面的新内核版本，保存完整串口记录。
4. 在 FPGA Linux 执行下面的短试次；通过后运行完整 smoke，覆盖本次先 warmup=32 再 warmup=0 的顺序。

```sh
uname -a
platform-timeout 120 idma_experiment --device /dev/idma0 \
  --aggressor none --victim-pages 1 --chunk 4096 --transfers 8 \
  --warmup 32 --timeout-ms 10000
rc=$?
echo "short test exit=$rc"
dmesg | tail -80
if [ "$rc" -eq 0 ]; then
    platform-smoke
    smoke_rc=$?
    echo "full smoke exit=$smoke_rc"
fi
```

任何失败都停止并保存 JSON、首错整行、退出码和内核日志，不开始性能矩阵。若上述全部通过，在另一次新启动直接执行 `platform-smoke`，再保存完整输出，覆盖 warmup=0→32 原顺序。

完整 smoke 含 14 组：四路各 3 组 Solo，另有 2 组双路；遇错即停，其余组保持“未运行”。验收需同时检查返回状态、data_errors、完成数、内核日志和 taint，不能只看最后打印或 DONE ID。通过后再做 per-DID HPM 校准。

缓存同步在测量窗口外，但改变初始 CPU 缓存状态，旧版性能样本不能与本版混用。真实数据错误仍报 EILSEQ；失败后 FENCE.I 重读正确也不会改为 PASS。`*_pte_pa` 为软件查询，不是硬件 IOTLB。

`platform-timeout` 保留上一版实现：只对直接子进程发送 TERM，超时返回 124；不能取消硬件 DMA 或恢复内核卡死。

详细依据：`/home/xvhaocheng/work/iommu-platform/DMA_CACHE_SYNC.md`。本目录与旧诊断/实验交付分开；`delivery.json`、`host-checks.json` 和日志记录源码版本及验证边界。

完整下载后可用 `shasum -a 256 -c SHA256SUMS`（服务器为 `sha256sum -c SHA256SUMS`）核验。
