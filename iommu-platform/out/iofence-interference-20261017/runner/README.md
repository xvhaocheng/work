# IOFENCE流量门：已停止的未验证代码提案

本目录仅保存任务停止前写出的独立runner提案，**未构建、未lint、未运行仿真，也没有26格结果**。root已核Intel VT-d Rev5.0 §6.5.4（印刷6-46）现有domain/page scope DR/DW，按用户“任何门失败即停止”要求终止本候选；本目录没有scoped fence机制。

完整状态、哈希、未完成项见[status](../analysis/status.json)。`invalidation_dma.cpp`是未验证的修改提案，[changes.diff](changes.diff)仅比较本轮与旧runner的CPP。`invalidation_dma_top.sv`、RTL overlays和其余驱动/分析脚本均是旧轮独立复制，尚未适配本轮合同，**不应直接用继承execute.py执行本任务**。旧报告及检查回执已另命名为`inherited-*`，仅说明复制来源，不构成本轮检查。

提案已写：A固定64×4096 COPY及strict逐页unmap、B四页持久映射/背景供给、512/2048/4096字节与0/2048间隔选项、global/none命令PRPW诊断选项、全已供给B工作收尾检查、per-fence状态/固定cohort计数和同拍DDR完成归因。所有内容均未经编译或运行验证。

root随后发现：规范already-processed边界至少应覆盖TC翻译完成响应；提案仍只快照comp接受，尚未接线TC processed计数，可能漏掉内部已翻译而待发的请求。因此不能以此提案支撑规范PRPW安全性或性能结论。前作门失败后已停止，不继续补实现。

frozen源与旧目录字节相同但无硬链接；[复制核对](frozen-copy.json)保留逐文件SHA256和inode独立检查。旧结果、生产RTL/Linux、FPGA/MIG、论文均未修改。本次由Codex编写/执行复制与状态记录；没有DeepSeek调用。
