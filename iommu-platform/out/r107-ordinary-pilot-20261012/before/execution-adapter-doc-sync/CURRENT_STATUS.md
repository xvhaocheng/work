# 当前状态 stage111 2026年10月12日 R107普通恢复关正在执行

- 真实goal已登记、active、无额外预算；整体仍阶段一立题。R107条件保留，尚无普通FPGA恢复／新G0／G1，论文未完成。[部署前接受](iommu-platform/out/r107-ordinary-pilot-20261012/predeployment-acceptance.json)。
- 必要ordinary四池/实际leaf/cache/checked IOFENCE/NEXT-DONE/全字节/层依赖与释放接口已实现、root亲核；一次干净构建59.066s，new kernel 6.5.0-rc2-r107-ordinary-stage111、payload29,363,656B/ade1bd4a…。静态RV64与host校验1正26反通过，非板测。[报告](iommu-platform/docs/r107-ordinary-pilot-20261012.md)。
- 原32MiB首分区已完整备份Dell，SHA21ba2506…两端一致；新payload板端hash及existing updater真实dry-run通过。原读/上传timeout和base64恢复全部留证，不假报原wrapper成功。当前旧boot7fd964e5…/f9优化bit/S0仍运行，未写SD或重启。[最终部署方案](iommu-platform/out/r107-ordinary-pilot-20261012/deployment-proposal.json)。
- 最终新SD写入许可已询问，尚未收到；此前只批准f9易失bit和重启。批准后只写/dev/mmcblk0p1、核新boot身份与独立计时，再同实例一次完整准备/换出/恢复/释放采证。保留大页/4KiB/premap/residency/batch/单DMA；不重跑stage106/108、不扩场景、新RTL0。
- 实际分配/leaf/cache/fence/完整成本及普通控制外翻译残余仍未测；技术错误不作科学阴性。具体可达可避免loss成立才G0；普通覆盖只归档R107卡、回阶段一新显式需求，不恢复关闭分支。
- 181旧主产物与内核输入不变，生产/config/cache/历史保留；UART已释放，专用以太网临时地址与task HTTP保留供部署使用。全Codex三独占Sol/ultra，无DeepSeek/Claude。[账本](iommu-platform/out/r107-ordinary-pilot-20261012/execution-ledger.json)。
- 主会话与初始独占核验席Sol/ultra，原生500000／有效475000，最大支持872000分列已验；未改模型配置或原运行记录。[模型验收](codex-sol-runtime-verification-20261012T072912Z-01a12eff/REPORT.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
