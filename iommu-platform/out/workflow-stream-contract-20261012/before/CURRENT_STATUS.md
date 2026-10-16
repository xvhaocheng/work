# 当前状态 stage 98 2026年10月12日预筛与有界G0完成

- 方向判断：Remote PTW自身代价到多DMA共享翻译干扰的主线合理；后期围绕机制补条件的推进方式已收束。[全景审查](iommu-platform/docs/research-panorama-audit-20261011.md)／[总计划](研究计划.md)。
- [本轮实际结果](iommu-platform/docs/workflow-row-g0-results-20261011.md)：A行预取的P＋4KiB／2MiB各跑一次，477行均无迟到；max释放至真实done为3192／2175周期，min余量280／1297。只约束本固定行为DDR模型，不是Sobel计算、FPGA或硬实时验收。
- 两项均完成480笔V＋2048笔A，无STOP A；字节/B/地址覆盖/源数据/保护区/最终quiet及独立事件核验通过。固定0..1656250周期窗后台完成938／2048笔，完整尾段已报告。大页授权容量由432KiB增至6MiB，不能视为免费。
- 当前裁决：归档A“需要新增保护以满足本期限”的假设；B双缓冲因缺真实计算、块/stride与slot释放合同而延后，不能列为实验失败。没有接受的新规则，按关口不进入G1、留出、原型或FPGA构建；不否定共享翻译主问题。
- 已验收M0完整64＋64夹具保留；stage97 P/PN差额分支仍关闭。E2/E3局部机制、P4M60份（53在线＋7离线）及method88阴性均保留，不重跑历史矩阵。
- 本轮实际：私有C++编译1次、DMA模型进程2次、独立核验2案（核验器4进程、7种负测2轮均拒绝）、绘图1次；原测试源码/生产RTL/ABI/Linux未改，无新Verilator展开、FPGA构建或上板。执行前快照及109项源/产物哈希保护在场。
- 下一决定题：新的来源明确工作流事实是否显示普通方法留下有用损失；B先需一份固定kernel/块/alias合同、input/compute/output/slot事件段及实际缓冲/DMA预算。不得为凑候选调本例期限、拼人工相位或恢复旧门控。
- [具体计划](iommu-platform/docs/research-execution-plan-20261011.md)／[根裁决](iommu-platform/out/research-execution-20261011/root-result-acceptance.json)／[执行账本](iommu-platform/out/research-execution-20261011/execution-ledger.json)／[研究方法](研究方法.md)。
- 本次全部Codex；三个新增席指定gpt-6.1-sol／ultra，无DeepSeek/Claude调用。[stage97原样快照](iommu-platform/out/research-execution-20261011/before/CURRENT_STATUS.md)。
