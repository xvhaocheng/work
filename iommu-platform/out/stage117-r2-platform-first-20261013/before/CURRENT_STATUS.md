# 当前状态 stage117 2026年10月13日 单一GPU来源预筛已交付

- C117 Holoscan Sensor Bridge双目IMX274来源条件保留，冻结2.7.0／1df133d4c78a6921249be4b147d65554aa8efa68；22份源码带来持续MR、tensor分配所有权、GPU/host注册分支及write-only输入等新增事实。[报告](iommu-platform/docs/stage117-holoscan-stereo-prescreen-20261013.md)／[稳定接受](iommu-platform/out/stage117-holoscan-stereo-prescreen-20261013/root-result-acceptance.json)。
- 来源任务结构成立，实际backend/requester/共享启用翻译路径未证实；现FPGA复制的额外源读、真实图像consumer、逐槽handoff与有依据预算仍待判。分配寿命不等于防覆写；RDMA MR base0不等于host IOVA0；代码4K默认与文档1080分列。来源/接口关未通过，未准入普通基线或G0/G1；完整合法普通对照和未知成本保留。[需求卡](iommu-platform/out/stage117-holoscan-stereo-prescreen-20261013/demand-interface-card.json)。
- 唯一下一合同stage118：静态裁决有限写入接口及真实图像consumer可达性，尚未登记/执行；不实现、部署、仿真、构建或上板，不追加第二workflow/恢复关闭卡。[下一合同](iommu-platform/out/stage117-holoscan-stereo-prescreen-20261013/next-goal-contract.json)。
- 实际工具goal仍为stage115 paused、完整原目标未完成；stage117是已授权但未注册独立任务，未冒称goal complete。来源→普通基线→G0/G1的stage116关口政策保持。Codex-only/Sol-ultra原生默认，无新席/外部模型/旧重跑；论文未完成。[验证记录](iommu-platform/out/stage117-holoscan-stereo-prescreen-20261013/delivery-verification.json)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
