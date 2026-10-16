# 当前状态 stage115 2026年10月13日 原生证据包获取未完成

- stage115实际goal已登记并保持active，无额外预算。目标是取得并核同一C113原生NVMe运行证据包；当前包未取得，不把来源入口检查标成goal complete。整体仍阶段一立题，C113条件保留/当前FPGA不准入，无G0/G1；R107/C112与先前关闭保持。[实际active快照](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-checkpoint-active.json)。
- stage115首轮已核两个同源入口：官方旧linkedcommit有NVMe模板但无关联run；SC22作者归档两份ZeRO-Infinity日志实际CPUoffload、version0.5.7、nvme_pathnull，不能套旁边NVMe模板或当v0.16.4。原ZIP 5,129,504B/MD5与metadata匹配，两log成员bytes亲核；不是科学阴性或全球无日志声明。[入口报告与原证据](iommu-platform/docs/stage115-c113-native-bundle-20261013.md)。
- 仍需关联同一已有nativebundle的version/launcher/config、path→FS/block/PCI/IOMMU/domain/commonunit/naturalpeer、N/B/p/H/pages、数值/成功byte/DONE/quiet/释放及ordinary完整成本残余。已异步询问资料路径/链接，保存时未回复；无新输入不重复已核入口、不重写未变C113卡、不另建小goal或部署补证。[同goal继续合同](iommu-platform/out/stage115-c113-native-bundle-20261013/continuation-contract.json)。
- 当前goal同一外部缺项审计第2轮；本轮无新资料路径/链接或匹配入口，未达到连续3轮blocked门槛，未complete/paused/提前blocked。前轮来源裁决已完成，不冒充本轮取包完成；收到新证据继续同goal，重复3轮且无独立进展才用实际工具blocked。[审计](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-blocking-audit.json)。
- Codex-only，root按Sol/ultra要求、一个必要新Sol/ultra/fork-none席独占AE中的zeroInfinity原run身份；不恢复旧席、改原生context或声称旧席释放。零DeepSeek/Claude/Astra、部署/build/RTL/driver/FPGAadapter/board/新测试/旧已验重跑/生产configDBhistory改动。旧stage111–114资产与原状态完整保存。[账本导航](iommu-platform/out/stage115-c113-native-bundle-20261013/README.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
