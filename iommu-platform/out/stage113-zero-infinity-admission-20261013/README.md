# stage113 单一来源／可达性裁决导航

2026年10月13日JST。本轮唯一工作流：DeepSpeed ZeRO-Infinity NVMe tiled CPU-Adam optimizer step，v0.16.4/e2dc3eeb1923073e32739596a4fd051417d4ff92。**C113源合同条件保留，当前FPGA实验不准入；没有新G0/G1。** 原R107/C112及更早关闭范围保持。未知实际需求和损失不当科学阴性。本轮不是论文完成。

1. [完整报告](/home/xvhaocheng/work/iommu-platform/docs/stage113-zero-infinity-admission-20261013.md:1)：单一需求卡、依赖、字节、普通强对照、完整成本、正／反条件、具体缺项和最终理由。
2. [稳定root裁决](root-result-acceptance.json)、[机器需求卡](demand-interface-card.json)、[字节／资源合同](byte-and-resource-contract.json)、[固定来源](source-selection.json)。symbolic bytes不冒充native运行；H/N/B/p/actual byte/timing/topology未验。
3. [root亲核关键原文](critical/root-source-review.json)：41个直接源码证据组、精确绝对路径／行号／原文／SHA256。actual FPGA四DMA路径为`fpga/src/ariane_peripherals_xilinx.sv`，通用tb wrapper不是实际四DMA证据。
4. [唯一子席完整报告](workers/nvme-boundary/report.md:14)、[57原文证据](workers/nvme-boundary/critical-quotes.json)、[40个source入口及git/blob核验](workers/nvme-boundary/source-manifest.json)、[可导航交付](workers/nvme-boundary/summary.json)。独占AIO→NVMe mapping边界，没有复审root整卡或恢复旧席。
5. [官方tag解析](source/version.json)、[初始源码与失败GET](source/manifest.json)、[root直接依赖](source/root-direct-dependencies.json)、[论文原文](source/ZeRO-Infinity-2104.07857v1.pdf)、[论文来源/hash](source/paper-manifest.json)。worker原始tree、blob匹配和冻结Linux文件在`workers/nvme-boundary/source/`；失败请求保留，没把下载失败说成源内容。
6. [原执行合同](goal-contract-input.json)、[实际goal登记后的active快照](goal-registration.json)、[实际完成回执](goal-completion.json)、[实际账本](execution-ledger.json)、[本席身份可读字段](validation/current-seat-metadata.json)。子席spawn明确Sol/ultra/fork-none；root运行model/effort/窗口字段未可读，不以旧会话证明本席窗口。
7. [交付校验](validation/delivery-check.json)、[旧关键输入hash](critical/preserved-input-hashes.json)、`before/`：本轮更新前原状态、交接、总计划与验证/执行文档完整保存。零旧实验重跑、生产/配置/cache/数据库/history改动、板动作或新构建。
8. [下一最小合同](next-goal-contract.json)：inactive、未登记／未执行。限定同一源实际配置与path→device/domain→natural peer→ordinary残余证据；不先建设新GPU/SSD系统或FPGA adapter，不自动加第二个来源。

物理平台仍Genesys2/CVA6＋四iDMA＋RISC-V IOMMU/Linux。本轮只在服务器核来源，不声明GPU或板上执行。原stage111实板结果与stage112关闭裁决原样保留，导航见`critical/preserved-input-hashes.json`及原handoff。
