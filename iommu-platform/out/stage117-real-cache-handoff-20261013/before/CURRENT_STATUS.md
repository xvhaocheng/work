# 当前状态 stage117逐包协议模型已接受 2026年10月13日

本轮完整交付检查通过后，实际goal已complete：工具记录673,693 tokens、30分05秒，无额外token预算；论文未完成。

stage117逐包协议模型已实现并接受：17项协议测试、两个同二进制新增RTL案例各一次通过；独立原JSON/总线/实际访问核验及14项篡改拒绝、1项独立性控制通过。包1/2消费时peer实际后端仍活跃，本包R/W/B已退休。原1639B/1675B、四span及布局不变，生产driver/ABI/RTL未改，旧实验未重跑。MODEL_UNCACHED_ASSUMPTION，实际CVA6缓存、应用时延、生产持久隔离未验证，未知成本null，无新G0/收益/创新声明，论文未完成。下一步是原工作负载下最小真实CVA6缓存交接见证，闭合alias、metadata和cache成本后再冻结一次G0普通对照；原两周窗口继续累计。

[报告](iommu-platform/docs/stage117-packet-protocol-adapter-20261013.md)／[稳定接受](iommu-platform/out/stage117-packet-protocol-adapter-20261013/root-result-acceptance.json)／[交付核验](iommu-platform/out/stage117-packet-protocol-adapter-20261013/root/delivery-verification.json)。实际完成回执见输出导航。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
