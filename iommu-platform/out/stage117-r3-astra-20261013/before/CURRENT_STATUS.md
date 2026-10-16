# 当前状态 stage117-r2 2026年10月13日 规则修订并重做已交付

- 按用户“困难就重订规则并重做stage117”，完成stage118决定性接口审查后转向：C117图像需载荷/consumer适配，当前投影条件归档，不继续适配链。新规则先查任务必要性与普通原地/不复制对照，再查现平台接口；GPU/AI在可行来源中优先。[新规则](iommu-platform/out/stage117-r2-platform-first-20261013/revised-stage117-rules.json)／[完整报告](iommu-platform/docs/stage117-r2-platform-first-20261013.md)。
- 重做只选C117R2 DPDK DMA复制转发，冻结v25.11／ed957165eadbe60a47d5ec223578cdd1c13d0bd9。虽匹配内存复制方向，原包无后续保留consumer，合法独占mbuf原地改MAC并TX可保留有用输出；copy benchmark不足以建立任务必须独立复制的需求。本卡不准入基线/G0/G1，未声称普通对照实测更快。[稳定接受](iommu-platform/out/stage117-r2-platform-first-20261013/root-result-acceptance.json)／[具体普通对照](iommu-platform/out/stage117-r2-platform-first-20261013/ordinary-control-decision.json)。
- 实际goal仍stage115 paused、完整原目标未完成；stage118/117-r2按已授权合同执行但未注册goal，无状态伪改。9份DPDK原始源码及关键硬件证据已核；无新席、外部模型、实现、构建、板测或旧重跑。完整成本未知项保留null；论文未完成。[交付核验](iommu-platform/out/stage117-r2-platform-first-20261013/delivery-verification.json)。
- 唯一下一合同stage117-r3：必要搬运/真实所有权依据优先，仅选一个未关闭来源并核现平台接口；不从复制benchmark反推需求，不恢复C117/C117R2或其他关闭卡。未登记/未执行，不自动启动。[下一合同](iommu-platform/out/stage117-r2-platform-first-20261013/next-goal-contract.json)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
