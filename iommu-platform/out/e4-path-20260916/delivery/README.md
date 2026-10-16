# 分段路径观测交付

2026-09-16：代码及定向验证完成，软件和FPGA完整构建均退出0、manifest passed，BIT/MCS非空。
内核 `6.5.0-rc2-00083-g45bf76b15e36`。新版尚未上板。按用户要求成功日志仅保存，未另读日志或时序报告。

- `ariane_xilinx.bit`：通过已有Mac/JTAG流程配置FPGA，DS50/50、observer v3。
- `fw_payload.bin`：按照已有SD原始分区流程更新的软件；不是把全部文件复制到卡。
- `idma_experiment`：独立程序副本，已内置启动镜像。
- `E4_PATH.md`：分段含义、字段、有效性检查和短测命令。
- `delivery.json`、两份manifest、`sources.lock.json`、`validation.json`、`SHA256SUMS`：源码、验证和产物证据。

必须配套更新BIT和SD软件。新Linux批量默认要求v3；旧v2 BIT会在提交前报错。
仍用 `--batch`，DMA硬件自主接续4KiB、CPU轮询整批。没有改成中断，没有改变DS延迟。

固定V=DMA0/A=DMA1：先8项Solo连续两次，再64项A8一次，均warmup0。
每次保留JSON、退出码、同seq的3条batch与16条path记录；检查valid/status、任务及事务守恒、数据错误/quarantine/taint。
旧采集脚本不能只保存三条observer，必须保留全部19条。异常立即停止，保留完整dmesg。

三次短测通过后，只做Solo/A1/A8各10次、4096项/warmup256，轮换条件顺序，所有miss模式均保留。
原E1/E4/批量结果和旧产物未覆盖。新增观测只是定位边界，尚不能证明HOL或纯PTW代价。
各阶段及读写累计周期不能直接相加当整批耗时。原时钟/CDC/IO审查待办保留。

构建源码顶层 `a2e8475`、CVA6 `a1572926`、Linux `45bf76b15e36`，文档完成提交 `562e266`。
主机测试为RTL事件模型/MMIO模型，具体通过项目见validation.json；未执行刷卡、JTAG或板上实验。
MCS保留在 `out/migration-4dma-path/fpga-src/cva6/corev_apu/fpga/work-fpga/`，本次Mac流程使用BIT。
