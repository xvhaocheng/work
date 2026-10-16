# 当前状态 共享IOMMU A5等待范围与部署判断

2026年10月15日。依用户本轮指定继续发展A，交付AXPY真实任务、具体普通动作及有失败条件的等待范围证书。**源级部署判断已具体化；真实共享翻译竞争、原生成功链与完整时间收益仍未知。** A保留候选，不据本轮限定推进恢复跨方向最优排名。论文未完成，MIG暂停。

- 新核地址腿：RX/RY的host读过外侧IOMMU，L1写/B在集群内；WY反向。远端R结束不能代替输入本地B。读写同SID0/SSID0；单DMA不排除跨copy流水，也不证明两个请求实际竞争共享翻译资源。xbar同ID跨目的限制未核内部实现，不能把接受descriptor当已经进入IOMMU。
- root完成规则：按consumer生产者取最小必需完成前缀，检查owner/alias及下一复用guard，返回公开控制足够/WAIT/NEED_GUARD/UNKNOWN。O-T1只等最后RY一个ID、原四槽及全部末尾/host生命周期保留；一般O-G/Y回收guard继续给普通方案机会。变体未应用/编译。
- 冻结同一32768元素、cold/hot两次任务的正反源合同：同已接受RX63/RY63/WY62，输入本地写已完成则ID门槛可过；仅远端R结束、本地写未完成则须等待。两案不是周期或真实性能正反例，不要求外侧所有信号相同。
- 实际新执行仅1次小型离线依赖核对，两切面与同信息标准DAG判断一致，含全部两episode/384job及最终释放边；不是新求解器/RTL可达性/数据验收，旧四案八次与A3八项不重跑。完整时间null，1正1反2时间留出未冻结。
- 决定推进有界完成域/等待证书方法，AXPY作普通能力校准；共享竞争/完整收益版本按缺项重设计，停止仅限“此动作必须新增早接口”主张。先核固定xbar/接收能力与自然窗口，不扩场景、不修外围软件寻找胜例。
- 当前用途优先、源节流/串行/公开ID、原buffer/CPU FMA、IOVA/适用shadow/cache及DS-first同信息同资源比较，全部准备/供给/cache/CQ/consumer/锁槽/排空与损失重叠计费。未知保持未知；当前C2八页/专有mlock不能免费套到Hero大任务或强加普通方法。
- A2构建/分层检查继承；新内核/真实DMA未运行。Hero三个硬件后端仍未实现/default deny，本轮不扩工具链或补hook。0新Linux/RTL编译、周期仿真、FPGA/板操作/生产修改/旧实验回放，Codex-only。
- 原MIG1792023314 paused、build6 WNS−0.391ns/598setup失败、0新加载采集/实际债务unknown不变；两周10月15—28日及历史用时保留。

[A5简洁设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[完整正反合同](iommu-platform/out/a5-wait-scope-contract-20261015/case-contract.json)／[root规则](iommu-platform/out/a5-wait-scope-contract-20261015/method/rule.md)／[原件路径](iommu-platform/out/a5-wait-scope-contract-20261015/source/translation-legs.md)／[稳定接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一最小决定](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)／[A2接受](iommu-platform/out/a2-sva-implementation-20261015/root-result-acceptance.json)／[总计划](研究计划.md)。同步前全文在A5 before，以下独立事项保持。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
