# 当前状态 stage117-r3 2026年10月13日 来源裁决已交付

- 唯一C117R3 DPDK vhost本地VM→VM来源，有限FPGA普通基线条件准入：接收方预发布buffer/cookie决定目的地址，完整包必须交付到该存储。允许CPU/同owner单DMA直接A→B，原A→host→B两次复制不强制；不是恢复C117R2。[报告](iommu-platform/docs/stage117-r3-astra-20261013.md)／[稳定裁决](iommu-platform/out/stage117-r3-astra-20261013/root-result-acceptance.json)。
- 现非batch DEFAULT可表达精确地址/长度；需有限包descriptor、used/consumer及全局quiet/cache交接的软件glue，尚未实现。不部署VM/完整网络栈，不冒称逐槽并发缓存、原生共同IOMMU或性能收益；无G0/G1/新机制结论。[接口与适配](iommu-platform/out/stage117-r3-astra-20261013/finite-adaptation-contract.json)。
- CPU、直接RX/producer放置、合法共享/页重映射、单DMA、大页/预映射/驻留/批量/布局/稳定池与全部成本保留；若普通已满足或适配越界即停止投影。下一合同仅同卡一次有界普通基线，inactive未执行，不自动循环选源。[下一合同](iommu-platform/out/stage117-r3-astra-20261013/next-goal-contract.json)。
- 本轮Astra/ultra执行，root亲核关键原文；本席独立goal已实际complete，[原始回执](iommu-platform/out/stage117-r3-astra-20261013/goal-completion.json)。无DeepSeek/Claude、实现、仿真、构建、板测或旧重跑；论文未完成。父stage115未主动改写；先前paused回执与后续get_goal=null分列，原因unknown。原生默认上下文未改，实际1.5倍未验实。[交付核验](iommu-platform/out/stage117-r3-astra-20261013/delivery-verification.json)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
