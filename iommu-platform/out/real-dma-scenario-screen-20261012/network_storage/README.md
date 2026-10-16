三份新原文已冻结。结论：取得真实 iperf3 与 SPDK local-PCIe perf 入口、各自完成规则及 mapping 条件，但同 host 共跑只是拟议的研究组合，没有取得原生并发服务的进度需求或共同物理翻译路径；本席建议保留需求/路径缺证，由 root 裁决，不进入新模型。本轮没有科学失效结论。

- [需求卡](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/demand-card.json)：source fact / experiment setting / unknown 分列，含可反驳条件、普通对照和最小适配成本。
- [证据导航](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/evidence.json)：E1–E17 与 root_must_read 精确原文行；摘要不替代原文。
- [完整来源清单](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/source-manifest.json)：三份新材料完整 bytes、版本、commit、URL、SHA256；F&S/地图/平台源码复用。
- [实际账本](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/execution-ledger.json)：一次错误旧路径 404、一次有目的路径纠正；3份成功材料后停止，没有 build/run。

root 必须直接读：

- [iperf3 官方手册](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/sources/iperf3.1:59)，另读125–132、321–328：端点统计、生成数据与结束条件。
- [SPDK perf](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/sources/spdk_nvme_perf.c:1532)，另读1737–1859、1892–1921：buffer reuse、error 仍进入完成计数、closed-loop/排空、PCIe/NVMe-oF 和数量终止。
- [SPDK 配置](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/sources/spdk_system_configuration.md:5)，另读49–59、89–112：translated VFIO、group 语义、全内存 map/pin 成本。
- [F&S 原文抽取](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt:968)，另读997–1011：远程链与已有 Rx/Tx IOTLB 残余，均非独立 NIC+SSD 共跑证据。
- [模型边界](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_system_top.sv:46)：无真实设备/协议/CPUcache/MIG，DMA DONE 只覆盖内存阶段。

所有新来源保持固定版本，不采用搜索返回的 master/latest 摘要作证。并发程序组合可以作为研究实验，但本轮优先真实场景，仍缺 service owner 的双方可接受进度规格。无 deadline 不构成否决。

作者与实际执行：Codex gpt-6.1-sol / ultra；没有调用 DeepSeek 或 Claude。仅做来源下载、定向关键源码审查、JSON/导航写入及静态文件校验。
