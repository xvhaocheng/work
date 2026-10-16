# E4 被动观测交付：本机验证与构建通过，待上板校准

保留轮询、补交次序、缓存同步和原实验 JSON。新增观测默认关闭。本目录软件和 FPGA 完整构建通过，产物哈希已核验；原 E1 数据、SC.W 修复 BIT 和缓存同步软件均保留。

## 交付配套

- FPGA：`ariane_xilinx.bit`，11,443,722 字节，SHA256 `41fa23d0aee476b603111df192c612f3d40e844067211564d2b8b175a60b9b90`。profile `migration-4dma-observe`，4 DMA、50 MHz、DS50/50，含原 SC.W 修复和新被动观测。
- SD 软件：`fw_payload.bin`，29,363,656 字节；内核应显示 `6.5.0-rc2-00081-g853e60b234c3`。
- payload SHA256：`d31fc2db4f620704563c8e4d75a50ce4a62e9e50b0a6e047b4f14cbc073d4ba6`。
- 构建源码：顶层 `228aed25ef3ea9da59b120357a7d8f00d4b0deb5`，CVA6 `540544ced5c579f7abc9d388ee6ddcb38a4110e8`，Linux `853e60b234c388ebb3d54bc3811d6d85048a4d36`。其他版本及哈希见 `delivery.json`。

## Mac / FPGA 下一步

1. 下载 BIT、payload 和 `SHA256SUMS`。只下载上板所需文件即可，其余是复核证据；`.mcs` 为附带产物，当前 JTAG BIT + SD 流程不用它。核验实际下载文件；保留旧文件和日志。
2. 按已经验证的 Mac 流程配置新 BIT，并把新 payload 写入 SD 的既有启动原始分区。继续使用当前卡布局和正确识别的目标分区；payload 不是放到普通文件目录即可启动。此说明不代替识别当前 SD 设备，也不执行刷卡。
3. 保存新一轮完整串口日志。启动后核对内核版本、`/proc/sys/kernel/tainted`，确认 `/sys/module/idma/parameters/observe` 默认是 `N`。先在关闭观测时运行下面的短 Solo，确认 JSON 成功、完成数及数据校验正常。
4. 空闲时开启观测，重复短 Solo。每次须保存 JSON、退出码和相同 `seq` 的 `part=time`、`part=axi_v`、`part=axi_a` 三条内核记录。`valid=1`、`status=4`，issued/done/polled 都应为本次目标数；H+P+R 和四种占用状态之和都等于 cycles；AXI 满足“期初+受理=完成+期末”。
5. 再验证角色互换、短 A1 共跑及 64 次窗口。V 的 AR/AW/RLAST/B 应有非零计数且期初/期末在途清空；Solo 的 A 统计应为零。硬件窗口按 20ns/周期换算应落在软件计时边界内，允许计时量化。共跑重叠为零也可能是结果，不强求它为正。
6. 全部通过后按实验卡做 36 条观测开关开销样本，再做 60 条 Solo/A1/A8 双角色主样本。出现退出非零、数据错误、无效/缺失观测、taint 或 quarantine 时停止采样并保存日志，不纳入性能 PASS。

短 Solo（在 FPGA Linux 上执行；第一次保持 observe=0，第二次先 echo 1）：

```sh
uname -r
cat /proc/sys/kernel/tainted
cat /sys/module/idma/parameters/observe
platform-timeout 120 idma_experiment --device /dev/idma0 \
  --aggressor none --victim-pages 1 --chunk 4096 \
  --transfers 8 --warmup 32 --timeout-ms 10000
rc=$?
echo "E4 short exit=$rc"
dmesg | tail -80
```

空闲时开启：`echo 1 > /sys/module/idma/parameters/observe`；关闭用 `echo 0`。开启观测需要新 BIT 和新软件同时就位；旧 BIT 会被明确拒绝，不能把错误当零值。每次试次只在入口捕获开关一次，执行中改开关不会取消 DMA。

## 解释边界

H 是任务分配 ID 到完成，P 是完成到 DONE 寄存器读被接受，R 是该读取到下次任务分配 ID。P/R 包括控制寄存器通路可能的等待，不能直接全归给 CPU；H 也不能单称 IOMMU 时间。两路在途重叠不单独证明共享队列竞争或 HOL。

布线后 WNS=0.177ns、WHS=0.027ns、WPWS=0.062ns，当前约束全部满足；观测寄存器均为50MHz，进出路径时序已检查。仍有原6项时钟方法警告及外部端口/CDC待办，Slice使用率96.55%。

本机测试和构建不能替代新版本实板验收。原时钟/CDC/外部端口约束审查待办保留。详细接口、验证记录与实验卡见平台根目录 `E4_OBSERVER.md`、`docs/e4-supply-timing-plan.md`。
