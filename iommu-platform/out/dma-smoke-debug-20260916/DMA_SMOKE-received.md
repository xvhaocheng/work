# DMA smoke：失败记录（2026-09-16）

本轮 DMA smoke **未通过**。Linux 启动/登录/proc 的上一轮通过结果仍成立，不能等同于 DMA 全功能通过。

## 结果

同一运行中的 `migration-4dma-scfix`，未重载 BIT、未复位、未写 SD/Flash。重新 root 登录成功，内核版本 `6.5.0-rc2-00078-g031e91dbadc1`；四个设备节点存在，taint 前后均为 0，完整 dmesg 未出现新的 Oops/BUG 或原 proc 故障。

1. 原 `/usr/bin/platform-smoke` 在首个用例报 `timeout: applet not found`，退出 127，未提交 DMA。板上 BusyBox 列表及 `command -v timeout` 均确认缺少该功能。
2. 临时兼容版保留全部 14 组原用例及参数，只将 BusyBox timeout 换为 shell/setsid 进程组超时保护。实板自测成功退出码 0、失败退出码 7、1 秒超时终止码 143；进程列表确认无遗留测试/守护进程。
3. 兼容版首组通过，第二组失败后按 `set -e` 自动停止，退出 1。剩余 12 组未运行，未运行双路并行用例。

| 用例 | 提交/完成 | 数据错误 | 状态 | IOTLB miss / S1 PTW |
|---|---|---|---|---|
| DMA0/DID10，1 页，chunk 4096，8 次，warmup=0 | 8/8 | 0 | 0，通过 | 2 / 2 |
| DMA0/DID10，1 页，chunk 4096，8 次，warmup=32 | 8/8 | 1 | -84，失败 | 0 / 0 |

两组均报告 32768 完成字节、measurement_complete=1、in_flight_at_end=0、quarantined=0。完成计数不能替代内容正确性校验。HPM 数值只是原始观察，未完成 per-DID 校准，也不证明 cold/warm 状态或请求重叠。

dell 只读源码核对：Linux 提交 `031e91dbadc115dafac957067135002b74035a10`；`linux/drivers/misc/idma.c` 的 `idma_verify()` 在源数据或目标预期内容首次不匹配时，增加 data_errors 并返回 `-EILSEQ`（-84）。因此终端的 “Invalid or incomplete multibyte or wide character” 是 errno 文案，不是串口编码原因；计数 1 不表示仅一个字节错误。当前结果未包含具体偏移/实际值，不能据此确定软件、缓存一致性、IOMMU 或 DMA RTL 中哪一层是根因。

## 文件与复现

- 原始串口：`results/serial/20260916-012105-console-aqvoxg_h/console.raw.log`；38,648 bytes，SHA256 `7bad255ae8d51189aee17f0b474443d7dba8b7516f4e1521c2ab740a416045f8`。
- 实验：`results/experiments/20260916-012053-run-70e00pz2/run.json`；两条原始 JSON 已提取到同目录 `dma-records.json`。
- 原交付 smoke：SHA256 `da462723891ca1c8313f90ea7f2257885721cee5c7e466b3d2fca1df5a50e0c4`；idma_experiment SHA256 `788a59a4979400a240308a9174bb7d4f85657e4f3a1022cfc2d2d952286801a8`。板上与 Mac 独立校验一致，原文件未改。
- `scripts/linux_dma_smoke_check.sh`：记录原入口的退出码、taint 和 dmesg；当前镜像仍会遇到缺 timeout 问题。
- `scripts/run_bounded.sh`：临时 Linux 超时函数；不使用 killall，命令和守护各有独立进程组。
- `scripts/platform_smoke_compat.sh`：内嵌上述函数的兼容副本，SHA256 `0c2509e2b19918b4cc562e933881210534fed6c384678e8b2b1b8fa02ef8c2a8`。本次经串口 here-document 执行，没有安装或覆盖板上文件。
- `scripts/summarize_dma_smoke.py`：在 Mac 重新读取原始串口 JSON，输出证据汇总，不自动宣称校准通过。

Mac 日志解析入口：

```sh
python3 scripts/summarize_dma_smoke.py results/serial/20260916-012105-console-aqvoxg_h/console.raw.log
```

兼容脚本只能在 FPGA Linux 执行，不是在 Mac 执行。重试前先开独立串口日志；本次已有数据校验失败，不建议盲目重复完整矩阵。下一步应先定位预热用例的首个不匹配；正式修复还需补齐 rootfs 的 timeout 依赖。本轮未修改 dell 源码、未重建软件/RTL，也未把本轮新日志上传 dell。

结束时已退出 root，观察到登录提示并关闭采集。板卡保持 Linux 运行。
