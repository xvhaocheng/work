# E3 v6 独立受控有限实验：构建与主机验证交付

完整软件与 FPGA 构建均已通过。本目录包含配套产物、实验协议和验证记录；新 v6 尚未上板。

**必须同时使用本目录的新 BIT 和 SD payload（同一实验的产物），不可与其他轮次的 BIT/payload 混用。**

- `ariane_xilinx.bit`：observer 外层 magic v6（受控 `0x49444d414f425306`），E3 bank v2 不变；DMA0 的配置接口将 `0xb0..0xff` 接入 launcher；DMA0/DMA1 前端支持保留与释放任务。
- `fw_payload.bin`：内核 `6.5.0-rc2-00087-g2de370cd344e`，已内置 rootfs；在旧系统上只替换 CLI 不够。
- `ariane_xilinx.mcs`：非空；标准 manifest 只记录 BIT、未列 MCS，故其 SHA256 单独记录。
- `idma_e3_controlled`、`platform-e3-controlled`（板卡 runner）、`idma.h`：新程序与配套接口。
- [E3_CONTROLLED.md](E3_CONTROLLED.md)：本轮协议、寄存器、CLI 与固定矩阵。
- [docs/e3-queue-results-review.md](docs/e3-queue-results-review.md)：v5 队列配对（13 对 / 26 子轮）上板复核，须与本轮 v6 结果分开阅读。
- 本轮被引用的协议文档 `E3_QUEUE_PAIR.md`, `E3_SERVICE.md`, `E3_DIAGNOSTIC.md`；协议交付副本中的验证与构建状态链接已指向本目录。

设计要点：本轮为独立受控——每轮 formal 只发一份 64 B 的 V 副本（有 A 时再发一份 64 B 的 A 副本），各一次读、一次写，由实际计数器核对；launcher 在硬件内做受控时序，CPU 只做**有限完成轮询**（非中断）。固定 DS 50/50、4 DMA、IOTLB 16、50 MHz、无 endless A、无 CPU refill。

板端建议命令：

```
platform-e3-controlled all /tmp/e3-controlled-v6-01
```

runner 逐 case 打印标记，把 CLI stdout JSON 存文件并 `cat` 到 UART、stderr 存文件并 `cat` 到 stderr；CLI 非零立即停止，并用 `trap` 始终保存 dmesg 与 taint 终态（保留原始退出码）。runner 不执行烧写 SD、联网或系统升级，不重试 ioctl，不隐藏失败。UART 在 Mac 上捕获、保存并记录版本后回传 Dell（常设做法）。

矩阵：短验收 3 对 + 正式 8 对 = **11 对、22 子轮**；若每轮仍为 19 条观测，合计**预期 418 条 UART 观测——本轮尚未测量**，不得写成已得结果。实际出错立即停止，不自动重试、不扩矩阵、不补采。

SD 更新仍按**原有原始分区流程**刷写 payload（写入既有 payload 分区）。本目录不提供、也不发明新的写卡步骤或分区偏移。

版本与完整性（全部取自本机文件，无估计值）：

- 构建起点平台提交：软件 `9839f447d4db2bf3f90704d67cdec4a658748a95`，FPGA `57b3bcf1c0a64731ff1bedd007def70c71372ed3`；manifest 记录的平台提交：软件 `9839f447d4db2bf3f90704d67cdec4a658748a95`，FPGA `8299b3b1402f4453a34047cdb861892f51fe6e3a`（文档提交可能使二者不同）。
- 组件：CVA6 `2b04936d2b4fe970bba4c97ab99f0a46a00eec06`，IOMMU `137f5e5fb1f2cc65a1346359226b16247c3f8445`，Linux `2de370cd344ed2b6bb1b0fa5b42bfe4eeb96214b`（取自 `sources.lock.json`）。
- BIT `b55f5afbba667a7540ed003dd724d078e0e6e4bff5d30fe7e796abba5b7f2ecc`；MCS `ca84dd80255bd84aabb971f76b508c6ad1822a5be0c11249fabe779c627fc655`；payload `3ec5ff7f5e376e94862f148739c10a0a2f487f7ca5e8afa755e08bf21f40a914`；Image `59ea0f100674c3f4b9ed28a9d35552b4fbb801d0c911b0aa3bfa651698b09ccf`；`platform.dtb` `0439c6c944dbb83af0ee3c3f637c0e952d8a75bfedda9cbc3e06c31fe7542f4a`；`sources.lock.json` `b7bc9f3525eb16585db45d37fea1d69fb8e97f16442978a4cb3c8d9f07d0be85`。
- 工作树脏状态如实记录：软件构建起点 True（有未提交改动）；FPGA 构建起点 True（有未提交改动）；manifest 记录：软件 True（有未提交改动）、FPGA True（有未提交改动）。原文档脏改动原样保留，本交付**不声称工作树干净**。
- 主机验证：`validation.json` 全部检查 `exit_code=0`、`board_tested=false`；其 `sources_sha256` 逐项重新核对，`cva6/*` 与 `out/migration-4dma-e3-controlled/fpga-src/` 中实际导出源码逐项比对（构建器有意改写的顶部文件 `cva6/corev_apu/fpga/src/ariane_xilinx.sv`、`cva6/corev_apu/tb/ariane_soc_pkg.sv` 按策略不计入 staged 逐字节比对，清单见 `delivery.json`）。
- `SHA256SUMS` 递归列出本目录全部文件（不含自身），生成后已逐项复核；`delivery.json` 记录版本、哈希、逐项验证结论、跨动作清单项漂移说明与 runner 来源。

边界与说明：**新 v6 尚未上板**；构建与主机测试成功**不等于硬件正确**；时钟/CDC/IO 审查边界维持原样，本轮**未新增时序签核**；成功日志只保存、**未审查**。分析文中的原始数据链接指向原工程路径，**原始 v5 数据与 `v5-reviewed-facts.json` 仍在 Dell，未随本目录复制**。

署名：DeepSeek 整理原始证据并起草 CLI/桩/测试与文档；Codex 独立复核原始数据与关键代码，负责 RTL/驱动、设计、关键修复、应用审查提案、测试与编译。
