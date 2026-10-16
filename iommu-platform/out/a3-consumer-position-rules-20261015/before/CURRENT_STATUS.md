# 当前状态 共享IOMMU A2软件适配

2026年10月15日。A2最小代码、软件构建及分层功能检查已交付；新内核/真实DMA/板端全链仍未验，完整时间预测尚未准入。A继续，论文未完成，MIG观测仍暂停。

论文创新分析建议聚焦A“真实消费者约束下的控制位置选择”：由共享翻译依赖、owner及不可撤回义务推出可核部署判断。新核RTAS2026已覆盖FIFO外调度/合法分块/整体响应分析；A须补真实任务中的不同部署决定、可复用规则与事前完整预测。此为分析建议，新颖性及性能未验收，原执行关口不变。[详细分析](iommu-platform/docs/research-novelty-assessment-20261015.md)。

- 当前Linux6.5已实现私有同步ABI、单mm固定pin/Sv39/noPASID共享CPU页表；普通IOVA、同VA独立shadow使用相同三包四span/8页/1639B载荷与1675B文件consumer。CPU copy和原buffer writev均有实际程序，不加buffer或小传输许可。
- 生命周期包含map回滚、owner/domain恢复、CQ失效完成证据、unmap、dirty-unpin及mm释放；不确定失败保留资源并阻塞，没有硬件abort/有界故障返回/热拔插保证。C2另要求同8页先mlock再触碰，并限制CPU PTE老化路径；普通方案不承担此额外限制或费用。
- 用户程序16/16、实际驱动C边界20/20及配置检查1项通过；实际页表析构47项通过（原版9失败）；生产CQ本体新4案通过。各层有边界替身，不是native DMA。旧13/14/17记录及首失败保留，旧实验不回放。
- 实际当前内核对象/vmlinux/Image、静态RV64程序、rootfs及DTB已构建；最终Image SHA c9bb2f937405aeb02a1ff0318597021b98c7bfe70b0a6fd605423849c4a02aa2。完整源码/.config/工具/产物锁已存，rootfs实核新程序与固定输入；未加载。新接口默认关闭。
- Hero固定Linux6.9驱动对象与精确LLVM d8完整AXPY host/device编译链接通过，冷/热独立consumer、末DMA wait和map生命周期已补；旧EH重定位编号冲突经受限透明适配解决，首exit0但错误ELF已否决。Ubuntu交叉sysroot不冒充论文准确配置。Hero专用排空/失效完成/cache三个hook仍未实现，默认映射前拒绝，不能说只剩板验。
- 当前三包文件输出已可由原buffer直接writev完成；只停止其“功能交付独有收益”主张，不推导native速度或否定A。当前同步C2损失用户态前缀重叠。下一版绑定真实设备位置/consumer或CPU预算，再比较普通独立页表prefix、同步borrowed与付费内核consumer；未冻结时间四案，不扫压力/相位。
- 本轮0新内核运行/真实DMA/整平台DMA RTL/FPGA构建/板操作，生产RTL未改；仅新增CQ本体功能测试。原MIG1792023314 paused、build6 WNS−0.391ns/598setup失败、0加载采集、实际债务unknown保持。Codex-only；两周10月15—28日与历史用时保留。

[实施报告](iommu-platform/docs/a2-sva-implementation-20261015.md)／[稳定接受](iommu-platform/out/a2-sva-implementation-20261015/root-result-acceptance.json)／[补丁/构建入口](iommu-platform/out/a2-sva-implementation-20261015/README.md)／[native验收合同](iommu-platform/out/a2-sva-implementation-20261015/validation/full-native-contract.json)／[下一决定](iommu-platform/out/a2-sva-implementation-20261015/next-decision-contract.json)／[总计划](研究计划.md)。旧状态完整存本轮before，以下独立事项保持。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
