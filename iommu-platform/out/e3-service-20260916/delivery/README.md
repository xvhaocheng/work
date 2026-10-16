# E3配对实验交付

2026-09-16：软件与FPGA完整构建均退出0，manifest passed，BIT/MCS非空；定向测试通过。尚未上板。成功构建日志仅保存，未读取日志或时序报告。

- `ariane_xilinx.bit`：新DS50/50、observer v4，经已有Mac/JTAG流程配置。
- `fw_payload.bin`：配套SD启动软件，按既有SD原始分区流程更新；不是将整个交付目录复制到卡。
- `idma_e3_pair`：新配对程序，已内置rootfs；旧`idma_experiment`也保留。
- [E3_SERVICE.md](E3_SERVICE.md)：实验目的、操作顺序、全部字段及比较边界。
- `delivery.json`、两份manifest、`sources.lock.json`、`profile.json`、`validation.json`、`SHA256SUMS`：版本、构建及产物证据。

必须配套更新BIT和SD软件。内核 `6.5.0-rc2-00084-g94a519eebf07`；旧BIT不能运行新E3配对命令。固定V=DMA0/DID10、A=DMA1/DID11，仍为硬件自主4KiB批量、CPU轮询整批。

先按说明做3项配对短验收：Solo8/warm0、A1 64/warm32、A8 64/warm0。每个命令内部各运行D0/D128一轮，共6子轮；第一轮失败则第二轮不运行。通过后仅10对A8和2对A1，4096项/warm256，首轮D交替。无触发不补采，真实错误立即停止并保留记录。

新stdout为一条 `idma.e3_pair.v1` JSON，包含两轮结果及原始E3字段；按其中两个sequence分别收齐每轮3条batch+16条path，保存完整UART、退出码和taint。旧采集器不能把它当作一条旧batch结果，也不能忽略无触发/首次V缺失/次序改变的样本。

软件通过只表示测量检查通过。主比较是目标V在干预触发后的剩余等待和完整排队/自身服务，不以整批吞吐差或“最后hit”直接证明HOL。原时钟/CDC/IO审查边界保留。

锁定源码：构建起点顶层bea8f57，CVA6 44068498、IOMMU1519510、Linux94a519eebf07。构建期间仅文档/测试提交e4d6161；组件/lock未变，最后说明提交f8a0374。未推送、刷卡或JTAG操作，旧实验产物未覆盖。

MCS保留在平台 `out/migration-4dma-e3/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.mcs`；当前Mac流程使用BIT。构建日志位于本交付目录的上一级 `software.log` 与 `fpga.log`。
