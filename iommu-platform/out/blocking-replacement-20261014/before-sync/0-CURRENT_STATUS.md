> actualgoal1791976511已实际complete；五页同步、308项新事件核验及133项交付检查先于complete。工具累计523,102 tokens／44分30秒，无额外token预算。[原样回执](iommu-platform/out/rcs1-completion-set-20261014/root/goal-completion.json)。仅关闭RCS1本版性能主张，论文未完成。

# 当前状态：RCS1四条件完成，当前版本性能主张关闭（2026-10-14）

- actualgoal1791976511的技术交付已闭合；五页同步和必要新核验后调用actual complete，无额外token预算。root及两个新席实际6.1-sol／ultra／native有效475000，Codex-only。论文仍在立题。
- **具体新规则有真实选择差异，但完整收益未成立。** RCS1以E_q过滤逐页refs，在18-key冷工作中换p3；普通minrefs/pin/fractionInsert换a、SJF换b。单缺key17-key草案由blind minrefs全子族覆盖、未跑性能。
- 四新条件×八固定规则，32完整新episode。RCS1最后完成C1/C3=921、C4=907；同一SJF=920、900；C2所有规则923相同。关闭本版完整性能主张，保留集合规则和反例，不推全家族普通覆盖或改论文定位。
- 每项51copy，模型R/W各3264B、408R/408W/51B，四consumer/owner收据、实际值和quiet通过。完整共有成本、single lookup/response、原guard/AW/WLAST及direct-full-result界面已计入；面积/Fmax/真实CPU/GPU应用／硬件gain未知null，未注错／失效／取消。
- 1个tick0日志失败原件保留，仅修该错误重试并执行其余未跑项。root独立核116750新事件、308checks pass；没有旧实验/validator/trace/build回放、成功项重跑、生产/RTL/driver/FPGA改变或压力/预热/相位寻例。单PTW/cache16和原队列保持；所有关闭分支与旧席不恢复。
- 下一关需超出既有普通控制的新活owner路径／实际R/W用途有符号动作；不自动调本版或转工作流/FPGA。未来-only延期填入仅未批准方向，须与普通同动作对照。8/9月资产继续继承；CO1/R2/R107/C112与旧AW停止有效，C未自动启动。

[报告](iommu-platform/docs/rcs1-completion-set-20261014.md)／[产物及actual登记导航](iommu-platform/out/rcs1-completion-set-20261014/README.md)／[稳定根裁决](iommu-platform/out/rcs1-completion-set-20261014/root-result-acceptance.json)／[后继合同](iommu-platform/out/rcs1-completion-set-20261014/next-goal-contract.json)。本轮结束不代表论文完成；当前页此前原件保存在本轮before与before-sync。以下独立维护原文保留。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
