# DMA0 单次诊断版上板流程（2026-09-16）

当前结果：诊断内核正常启动，指定的一次 warmup=32 Solo 通过；用户随后确认运行完整 14 组 smoke，但第一组 warmup=0 就出现数据校验失败，正式脚本自动停止，剩余 13 组未运行。首错显示目标字节在 FENCE.I 后从 `eb` 变为期望的 `14`。两轮结果及完整首错行均已回传 dell，数据故障尚未修复。

本次独立实验：`results/experiments/20260916-015253-run-jlcizoxg/`；串口：`results/serial/20260916-015329-console-k9ylodj8/console.raw.log`。保持单次测试范围，不把此前旧内核的结果混入本轮。

## 实板结果及回传

两轮均使用 SC.W 修复 BIT 和诊断内核 `6.5.0-rc2-00079-g53d3113dee74`，未在两轮之间重启/重新配置。板上的平台超时程序、实验程序、完整 smoke 脚本哈希均与新交付一致。31 项 Mac 离线测试通过。

| 顺序 | 范围 | 实测结果 |
|---|---|---|
| 1 | 用户指定的一次 DMA0 Solo，warmup=32、1 页、8 次、chunk4096 | success=true，8/8 完成，32768 bytes，data_errors=0，退出 0，无首错行 |
| 2 | 用户追加授权完整 14 组 smoke | 首组 DMA0 warmup=0：8/8 完成，但 data_errors=1、status=-84、退出 1；0 组通过、1 组失败、13 组未执行 |

第 1 轮不能证明故障修复，也没有触发错误后的刷新诊断。第 2 轮得到源值 `14`、刷新后 `14`、期望 `14`；目标值 `eb`、刷新后 `14`、期望 `14`，首错偏移 912（0x390）。目标旧值与正确值互补，刷新后恢复正确，支持旧缓存视图假设；仍不是完整根因证明。软件页表查询值仅原样保存，不把它等同于硬件 IOTLB 或据其单独诊断地址转换。

第 2 轮启动时控制台实时打印的首错行损坏/截断；随后原脚本 dmesg 和独立只读 grep 均取得相同的完整行，没有为补日志重跑 DMA。提取脚本检查全部 18 个字段并去重，保存完整版本。

完整首错行：

```text
[  272.307762] zdl,idma 50000000.master: experiment data mismatch: id=0 did=10 offset=912 touched_end=4096 size=4096 chunk=4096 src=14 src_after_fence_i=14 expected_src=14 dst=eb dst_after_fence_i=14 expected_dst=14 src_iova=0x3fffe00390 dst_iova=0x3fffe01390 src_pa=0x0000000082d10390 dst_pa=0x0000000082d49390 src_pte_pa=0x0000003ffff10000 dst_pte_pa=0x0000003ffff49000
```

本地：

- 单次：`results/experiments/20260916-015253-run-jlcizoxg/diagnostic-result.json`；串口 25,234 bytes，SHA256 `75fa4680629b6a194857c4df14283d641031193f2f9460fc01c5a218f33a733f`。
- smoke 首组失败：`results/experiments/20260916-020422-run-_hjma4td/diagnostic-result.json`、同目录 `mismatch.txt`；串口 `results/serial/20260916-020427-console-bm0agd4f/console.raw.log`，9,096 bytes，SHA256 `b2786f55ecb276670b6446bbcf21ef234942133a2f067aef2e02e702adca5aef`。

dell 已创建两个独立目录，各含 `dma-diagnostic.json`、`mismatch.txt`、`summary.json`，远端写后哈希与 Mac 一致；单次成功的 mismatch.txt 为空：

- `/home/xvhaocheng/work/iommu-platform/out/dma-smoke-debug-20260916/mac-20260916-015253-run-jlcizoxg/`
- `/home/xvhaocheng/work/iommu-platform/out/dma-smoke-debug-20260916/mac-20260916-020422-run-_hjma4td/`

回传入口 `scripts/return_dma_diagnostic.py <diagnostic-result.json> --upload`，默认仅预览，目标目录必须不存在；不要重复上传到已存在目录。仅回传要求的 JSON/首错行/版本摘要，未上传完整串口或历史研究文档。

两轮 taint 前后均为 0，没有原 proc/Oops 崩溃，进程列表未见遗留实验/超时进程。已正常退出 root、关闭串口，Linux 保持在登录提示。未做性能/HPM/完整双路矩阵或进一步修复。

## 本次 SD 实际结果

- 更新记录/备份：`results/sd/update-r7q8_l66/`；状态 `readback_verified`。
- UTC：2026-09-15 16:51:08～16:51:16。
- 旧 32 MiB 启动区备份 SHA256 `90dcd0bd70cd55a6679d01d5154cdd6c758a34822ad0ca5930a21f1eb0b408ba`，与上一版更新结果一致。
- 新 32 MiB（含补零）完整回读 SHA256 `281543ee99f0f98531e7df8256c6dbdd774b9abc8b431b7574042bfe15ee52aa`。
- payload 部分回读 SHA256 `58ba4d4ae9601ba8fad6064e217dabef51f897ac5461ca07fcc77eb9232836bb`，与固定交付一致。
- GPT 主备 CRC 正常、盘头/尾各 1 MiB 未变；未写第二分区。更新前后核对卡 UUID/容量/偏移通过。
- `diskutil eject /dev/disk5` 成功，用户可安全取卡。旧版本可通过原 restore 流程及本次备份恢复，不把诊断版当成故障修复版。

## 固定版本

- dell 交付：`/home/xvhaocheng/work/iommu-platform/out/dma-smoke-debug-20260916/delivery/`。
- Mac：`artifacts/software/dma-smoke-debug-20260916/delivery/`；15 个文件、14 项 SHA256 均通过。
- `delivery.json` SHA256：`1953fa06372cedbc2b1322a8b112e7cf6849dbb8325e94bad1b96a036722d0a3`。
- payload：29,363,656 bytes；SHA256 `58ba4d4ae9601ba8fad6064e217dabef51f897ac5461ca07fcc77eb9232836bb`。
- Linux：`6.5.0-rc2-00079-g53d3113dee74`；commit `53d3113dee74761515fc08b6a4f984d41227c50f`。
- BIT 仍为 `migration-4dma-scfix`，SHA256 `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3`，没有新 BIT。

诊断内核不是数据故障修复：即使刷新后读值变正确，原 EILSEQ 失败仍保留。页表查询的物理地址不代表硬件 IOTLB 状态。

## SD 更新

板卡先断电，再将原 SD 插入 Mac。每次重新检查设备编号，脚本还会核验原 UUID/容量/偏移，不允许换卡或改布局。

```sh
python3 scripts/fetch_dma_debug.py
diskutil list external physical
python3 scripts/sd_card.py update --disk /dev/disk5 --release dma-smoke-debug-20260916 --dry-run
sudo /opt/homebrew/bin/python3 scripts/sd_card.py update --disk /dev/disk5 --release dma-smoke-debug-20260916
```

这台 Mac 沿用已成功的系统 Terminal 管理员流程；启动入口为 `osascript scripts/update_sd_debug_terminal.applescript /dev/disk5`。在 Terminal 本地输入 Mac 管理员密码，不写入脚本/日志。

只备份并更新原 32 MiB 启动分区，主备 GPT 和头尾保护区检查、完整回读仍保留。默认不带 `--release` 依然是旧软件，诊断更新必须显式指定。新旧软件下载目录和历次 SD 备份均分开保留。必须看到 `readback_verified` 并核对 payload 回读哈希，才能安全推出并取卡。

## 单次上板诊断（写卡后）

安全推出 SD，放回断电的 Genesys 2，连接 JTAG/UART 后供电。断电会清除易失配置，所以需要重新加载同一 SC.W 修复版 BIT，不换回历史 BIT。

先在终端 A 开始串口采集，在终端 B 加载并确认配置：

```sh
python3 scripts/fpga.py console /dev/cu.usbserial-AV0KRCHT
# 另一个终端
python3 scripts/fpga.py program --profile migration-4dma-scfix
python3 scripts/fpga.py status
```

等待 SD 读取和 Linux 启动，登录后先核对新内核版本及 `/usr/bin/platform-timeout` 哈希。只执行 `scripts/linux_dma_diagnostic_once.sh` 的一次 DMA0 Solo：

```sh
uname -a
/usr/bin/platform-timeout 120 /usr/bin/idma_experiment \
  --device /dev/idma0 --aggressor none --victim-pages 1 \
  --chunk 4096 --transfers 8 --warmup 32 --timeout-ms 10000
rc=$?
echo "DMA diagnostic exit=$rc"
dmesg | tail -80
```

保留完整 JSON 和 `experiment data mismatch:` 整行，必要时补读完整 dmesg 但不重跑 DMA。禁止本轮自动展开全 smoke、双路、HPM 或性能矩阵。`platform-timeout` 不取消硬件 DMA，也不能恢复不可中断内核卡死。

Mac 离线回归 27/27 通过，包括新增诊断版本选择、错误版本拒绝及 dry-run 不打开设备检查；这不是实板验收结果。
