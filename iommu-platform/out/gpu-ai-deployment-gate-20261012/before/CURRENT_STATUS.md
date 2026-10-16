# 当前状态 stage103 2026年10月12日真实双场景预筛结束

- 整体仍在第一阶段“问题与方法立题”；按用户选择调整筛选范围，新一轮真实场景预筛结束，未进入可执行G0/G1，方法论文未完成。[最新报告](iommu-platform/docs/real-dma-scenario-screen-20261012.md)／[总计划](研究计划.md)。
- 网络/存储与周期采集/独立后台各补核三项，共六原始材料，得到真实程序与任务生命周期；两项都缺匹配运行实例的实际共同翻译路径及完整事件/完成/成本对应，无选定场景或G0运行登记。
- [网络卡](iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/demand-card.json)：iperf端点＋SPDK PCIe提交/复用/排空已核；研究共跑须明示。VFIO group不证明NIC/SSD共用cache/PTW；随机LBA不等于新IOVA，command/receiver完成不能冒称iDMA DONE。
- [采集卡](iommu-platform/out/real-dma-scenario-screen-20261012/periodic_capture/demand-card.json)：官方SDI/YAVTA入口已核；正常10次捕获仅后三次允许保存，实际buffer/stride/bytesused/周期未知。默认2025.1 DTS关闭SMMU不代表历史运行image；独立有限后台未确定。DDR环无需塞入R2 SPM。
- 根已核原文、完成调用链、NEXT/DONE/STOP及模型范围；两条条件失效假设与相邻反例保留，未测普通失败或接受新机制。无deadline非否定理由，合法大页/稳定pool/提前准备/发布前优先与反馈均须公平计价。
- 到限结束本轮，不追加第三场景、不扫强度/扩后台、不自动构建时序/模型/RTL/FPGA。重开需选定实例的共享路径＋独立任务buffer/arrival/有效completion/mapping记录，才能冻结最小G0。stage98—102及E2/E3/P4M/method88旧资产保留。
- [根裁决](iommu-platform/out/real-dma-scenario-screen-20261012/root-result-acceptance.json)／[选择](iommu-platform/out/real-dma-scenario-screen-20261012/selection.json)／[根假设](iommu-platform/out/real-dma-scenario-screen-20261012/root-hypotheses.json)／[旧状态](iommu-platform/out/real-dma-scenario-screen-20261012/before/CURRENT_STATUS.md)。仅Codex，无DeepSeek/Claude；本轮无新性能实验，旧源/原始产物及脏工作树保留。
