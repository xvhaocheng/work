# DMA 缓存同步修正与复测

日期：2026-09-16。承接 [上一版诊断](DMA_SMOKE_FIX.md)。结论：实板首错支持 CPU 读到旧缓存；本轮补上当前 CVA6 平台所需的 DMA 前后同步，并修正软件物理地址查询。**本轮修正版尚未上板，不能宣布数据故障已解决或唯一根因已确定。**

## 新实板证据

收到 `DMA_DIAGNOSTIC.md`，归档在 `out/dma-cache-sync-20260916/DMA_DIAGNOSTIC-received.md`。服务器已收到两次调用的 JSON、首错行和摘要，完整串口日志仍在 Mac。

- 诊断内核 `6.5.0-rc2-00079-g53d3113dee74`、SC.W 修复 BIT：新启动第一次 DMA0 Solo、1 页、4096 chunk、8 次、warmup=32 通过。
- 同一次启动随后执行完整 smoke，第一组 warmup=0 失败，8/8 完成但 `status=-84` / `data_errors=1`，后续 13 组未执行。Linux 正常，taint=0。
- 首错偏移 912（`0x390`）：源始终为 `14`；目标从 `eb` 经 FENCE.I 变为期望 `14`。`eb` 恰是目标初始化值 `~14`，符合 DMA 更新内存、CPU 保留初始化缓存行的解释。仍未单独排除额外等待等因素，不能仅凭这个探针认定唯一根因。
- 原始回传：`out/dma-smoke-debug-20260916/mac-20260916-015253-run-jlcizoxg/`（通过）和 `mac-20260916-020422-run-_hjma4td/`（失败）。

两轮中 warmup=0 和 32 都各有通过和失败，不能把问题直接归因预热次数。

## 最小软件修正

当前平台 Linux 将 DMA 视作 coherent，实际 CVA6 WT D-cache 没有外部 DMA 写入失效通知。针对现有单核平台，在 `idma_run_experiment` 增加两次 `local_flush_icache_all()`：

1. 两路缓冲初始化/映射及 HPM 配置完成后、任何 DMA 提交和预热前，排空 CPU 写入并清缓存。
2. 测量结束、两路安全排空后、任何数据校验前，清缓存，令 CPU 重新读取 DMA 结果。

该调用有效是因为**当前 CVA6 的 FENCE.I 同时清 D-cache**，并非通用 RISC-V 保证，也没有修复整个 Linux DMA 子系统的 coherent 声明。没有每次搬运刷新，没有新增 ABI、运行模式或 RTL。遇到不确定排空仍保留原隔离规则；真正的数据错误仍返回 EILSEQ，已有失败探针不把重读正确改成 PASS。

两次同步均在测量时间窗之外，但开始测量时的 CPU 缓存状态会改变。修复前后性能样本不能混用，功能通过后需用新版本重新校准 HPM 和测量开销。

## 软件页表查询的确定错误

旧 `riscv_iommu_iova_to_phys()` 把 `iova & PAGE_MASK`（IOVA 页号）拼进物理地址，并丢掉页内偏移，导致诊断字段出现异常大地址。例如本次源字节实际 PA `0x82d10390`，旧查询却打印 `0x3ffff10000`；目标实际 PA `0x82d49390`，旧查询打印 `0x3ffff49000`。

修正为按实际叶子大小拼入页内偏移，覆盖 4 KiB、2 MiB、1 GiB。主机执行真实查询函数可精确复现旧日志地址；16 个场景旧版失败 12 个，修复版全部通过。**这是软件查询错误，不能把旧 `*_pte_pa` 当作硬件实际搬运地址或 IOTLB 内容。** 本轮没有据此修改硬件翻译路径。

## 本机验证与交付

- 调度/诊断原 16 场景、96 个小 chunk 组合通过；新增 Solo、双路旧缓存视图及真实内存破坏 3 场景通过，真实破坏仍报错。
- 分别删除 DMA 前同步、DMA 后同步，新增缓存回归均能检出错误。该模型检查软件同步位置及校验语义，不代替 CVA6 硬件验证。
- 软件物理地址查询 16 场景通过；两个改动内核目标交叉编译、差异格式检查和 checkpatch 通过。
- 证据目录：`out/dma-cache-sync-20260916/`。`make check` 和完整软件构建退出 0；21 个组件干净、gitlink/lock 一致。保留原 DTS interrupt provider 缺 `#address-cells` 警告。已有研究/计划文档脏改动未代为提交，manifest 如实记录顶层 dirty。
- Linux 提交 `3b47ab1645e412095b601e3be68b6347b9fad213`，构建顶层 `76de9ee73656cc90bdae5331be7020b5f905e205`，内核 `6.5.0-rc2-00080-g3b47ab1645e4`。
- 独立交付：[更新及板上命令](out/dma-cache-sync-20260916/delivery/README.md)。payload 为 29363656 字节，SHA256 `6f52ad85c4d2fc87e97301d98f1394216b843cd8be601b2965eca429caf74053`。rootfs 三个入口内容/权限、两个 ELF 的 RISC-V 架构、内核版本/诊断字符串、manifest 产物哈希和交付目录 SHA256 全部核对通过。SC.W 修复 BIT 哈希未变；旧诊断交付与旧实验镜像保留。

## 上板复测顺序

新版需要替换 SD 原始分区中的 Linux payload，并按 Mac 已确认流程回读核验；只替换用户态程序不能得到内核修正。沿用 SC.W 修复 BIT，SHA256 `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3`，本轮不重建 FPGA。

先在新启动中运行 warmup=32 的 DMA0 单页短试次，再运行现有 14 组 `platform-smoke`，复现本次调用顺序。全部通过后，在另一新启动直接运行完整 smoke，覆盖原来的 warmup=0→32 次序及四路、未触及页、双路。任一组失败即停，保留 JSON、首错整行和内核日志，不继续性能矩阵。各次保存内核版本和最终退出状态。

全部通过仍只是功能 smoke；随后才做 per-DID HPM 校准与 E1 先导实验。原时钟/CDC/IO 约束待办不因软件修正消失。
