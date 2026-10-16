# stage117续行：三个实际结果

同一C117R3、同一generated三帧：适配正确性、Dell直接CPU、真实单iDMA＋IOMMU RTL均通过；普通方法已功能满足，未建立可避免共享翻译残余，不进入G0/G1。

- [完整报告](../../docs/stage117-three-results-20261013.md)
- [root亲核接受](root/critical-review.json)与[跨两席13项整合核验](root/integration-verification.json)
- [结果1：适配](result1-adapter.json)；[代码](cpu/adapter_cpu.cpp)；[共用输入](common/fixture.json)与[独立consumer](common/fixture.hpp)
- [结果2：CPU](result2-cpu.json)；[实际run](cpu/run.json)；[编译/环境/hash/诊断账本](cpu/execution-ledger.json)
- [结果3：真实DMA完整交付](dma/results/delivery.json)；[实际run](dma/results/run.json)；[原DDR快照与trace独立核验](dma/results/artifact-verification.json)
- [本次文档交付检查](delivery-verification.json)；[改前五份文档](before/manifest.json)

CPU29,830ns是本机一次有限生命周期，含observer；RTL1227周期仅是模型条件下配置→quiet，使用`ptw_latency=1`、DS50/50、DDR行为内存且无CPU/cache。二者不构成速度比。初次ASan失败、后续有界handler-off诊断通过分别保存。真实Linux映射/缓存、native应用、物理大页和FPGA成本未测；生产driver/RTL/ABI未改。

实际goal已complete：[稳定接受](root-result-acceptance.json)／[完成回执](root/goal-completion.json)。
