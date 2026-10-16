# 当前状态 stage111 R107普通恢复关：最小修复部署中（2026年10月13日JST）

- 原真实goal已恢复active，无额外预算；整体仍阶段一立题，R107条件保留。完整恢复／成功释放／全部成功成本仍未取得，无G0/G1，论文未完成。[原goal实际状态](iommu-platform/out/r107-ordinary-pilot-20261012/goal-active-observed-after-user-resume.json)。
- 首次安装与新boot身份通过，运行6.5.0-rc2-r107-ordinary-stage111，boot67ad5da4…、config/notes/DTB匹配。一次RUN确证普通4×4MiB分配、8真实2MiB叶、16MiB映射和准备边界；首批在NEXT前EBUSY，实际DMA触发／字节0，private owner隔离保留。[原始失败与根因](iommu-platform/out/r107-ordinary-pilot-20261012/failure-root-cause.json)。
- root把包含observer ARM/ACTIVE的configuration idle误当DMA quiet，源码与原始UAPI已经闭合根因；0miss不作科学阴性。旧source/失败guard/八原始文件完整保留。[字节绑定及部分接受](iommu-platform/out/r107-ordinary-pilot-20261012/validation/actual-failed-receipt-subset-acceptance.json)。
- 最小修复仅独立副本一个idma_finite.h；实际冻结RTL与原/新C helper耦合29项回归通过，严格准备/释放/未知NEXT隔离不放松。独立干净构建59.542s，r1版本/payload e674f609…；真实新payload SHA/GPT dryrun通过234.240s。按持续同任务授权部署同一可恢复首分区，未伪造新的hash特定回复；修复写入已PASS/rc0，整分区hash进行中，未新RUN。[修复源审与方案](iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/validation/root-idle-repair-source-acceptance.json)／[授权来源](iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/live/new-SD-user-approval.json)。
- 下一步仅r1新boot/真实CAPS/独立计时后，同一有限合同完整priorout＋restore＋所有字节/层proxy/checked释放采证。合法大页/4KiB/premap/residency/batch/单DMA保留，不重跑stage106/108、不扩场景、新RTL0；发生实际DMA技术错误仍停止与保留，不能当普通科学失效。
- raw CPU/TIME、名义ns、独立Dell墙钟与条件频率界分别保存。配置idle与批次完成quiet已区分；当前成功恢复/全字节/释放仍未验实。[成本及未测边界](iommu-platform/out/r107-ordinary-pilot-20261012/costs-and-boundaries.json)／[同goal继续合同](iommu-platform/out/r107-ordinary-pilot-20261012/next-goal-contract.json)。
- 181旧主产物/输入未变，生产/config/cache/原历史保留。全Codex Sol/ultra，无DeepSeek/Claude；主会话与初始核验席原生500000/有效475000、最大872000分列已验。[模型证据](codex-sol-runtime-verification-20261012T072912Z-01a12eff/REPORT.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
