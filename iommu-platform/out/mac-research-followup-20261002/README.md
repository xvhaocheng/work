# Solo修复版00091：cost-followup 11/11在线完成

2026-10-02，运行一次 `platform-research cost-followup /tmp/research-followup-01`，11项全部通过交付runner在线判据，final_exit=0、FOLLOWUP_EXIT=0、collection_failed=0。包括短Solo/A8gap0各1项，主Solo/A8gap0/A8gap2000各3项。Solo未再出现原EINVAL拒绝。

Linux：6.5.0-rc2-00091-g9ad05d00cca7；boot_id：db14364a-c046-4e6a-8ae7-f4dea007724a。复用本地next-experiment-20261002/delivery/ariane_xilinx.bit研究BIT。初次串口停于SPI初始化，1MHz重新加载同一BIT后正常从SD启动；未改SD、重新编译或改动被测代码。本轮完整UART保留最初停顿与恢复过程。

这是新软件、同次启动的独立11项对照，不与旧00090的非Solo拼接成减速比例。旧84项的80成功/2失败/2未运行记录不改写；旧E2未重跑。

当前只完成原runner在线检查和原始结果采集，没有独立离线语义验收、性能分析或统计结论。没有增加试次或失败重跑。完整JSON/stderr/退出码/dmesg/计划等保留于collection/，Linux保持运行。

Dell目标：/home/xvhaocheng/work/iommu-platform/out/mac-research-followup-20261002/。本轮按用户要求直接执行已交付工具，未调用DeepSeek。
