# 当前状态 stage111 2026年10月12日 R107普通恢复关执行中

- 整体仍阶段一立题；R107条件保留，尚无普通恢复成功／新G0／G1，论文未完成。原stage111 goal已登记、无额外预算；工具仍显示blocked且未提供恢复接口，用户本次要求按已审方案执行，原完整目标继续。[恢复合同](iommu-platform/out/r107-ordinary-pilot-20261012/resume-execution-contract.json)。
- 普通四池/实际leaf/cache/checked IOFENCE/NEXT-DONE/全字节/层依赖与释放接口、一次干净构建59.066s、静态RV64与host校验已接受，非板测。[报告](iommu-platform/docs/r107-ordinary-pilot-20261012.md)。
- 用户本次执行指令按上下文授权此前已展示的具体SD更新与重启；原备份/payload/f9 bit本地哈希及新鲜旧boot/payload/updater/分区/未挂载核查均通过。一次updater已返回PASS与rc0（含fsync/close），正在同次整分区SHA检查；尚未重启或RUN。[授权原文与解释](iommu-platform/out/r107-ordinary-pilot-20261012/live/new-SD-user-approval.json)／[单次写入状态](iommu-platform/out/r107-ordinary-pilot-20261012/live/SD-write-attempt.json)。
- 下一步：整分区hash匹配后，同f9易失bit重启；新boot/config/notes/DTB接受后核独立计时，再同实例一次完整准备/换出/恢复/释放。保留大页/4KiB/premap/residency/batch/单DMA，不重跑stage106/108、不扩场景、新RTL0。观察timeout不重复提交写入或RUN。
- CPU/TIME原始计数、kernel及board userspace名义ns、独立Dell阶段墙钟分列；固定分频源码已亲核，运行连续性/漂移/SI准确度不冒称板验。实际普通完整成本与控制外可达可避免翻译残余仍待一次采证；技术错误不作科学阴性。具体残余成立才G0，普通覆盖只结束R107卡并回阶段一新显式需求。
- 生产/config/cache/原历史保留，task HTTP只绑定专用192.168.77.1:18080；全Codex独占Sol/ultra，无DeepSeek/Claude。主会话与初始核验席原生500000／有效475000、最大支持872000分列已验。[模型证据](codex-sol-runtime-verification-20261012T072912Z-01a12eff/REPORT.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
