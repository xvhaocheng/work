# 当前讨论：共享翻译协作候选静态预筛完成（2026-10-14）

## 本会话执行：一次自然入口观测（2026-10-14，actualgoal1791965564）

- 此goal独立于下方A–H静态讨论；实际root Sol6.1／ultra／native475000、独自、Codex-only，无额外预算。新授权的一次采集已完成；实际goal已complete，工具回执191168 tokens／30分17秒，无额外预算；论文未完成。
- 沿用原两路DEFAULT／sole4096B／buffer512、source0→source1触发；accepted decouple_rw0／deburst0，原两槽AW spill峰值2。source1第二AR第73拍早于首input AW3242及source0首raw WLAST2954，取得端点纠正的原始见证。
- 两job存活的B2/A0有1695拍，加A读P后429拍（1557–1985）；此间B物理载荷L始终未就绪。B首物理AR2430晚于A最后PTE消费1985，完整入口未见。本固定冷前缀停止，future-AW/F1全族open，原关闭结论保持。
- 各源实际4096B／2AR／2AW／512R／512W／2独立B、12PTE及最终quiet正常；硬件完成3190／4145、原轮询返回quiet4151，含6拍原发现／返回，均为本次基线而非gain。super_last0不当作job末次metadata，内部last-burst FIFO未直接导出。
- 仅1新隔离build／1新被动capture，未重跑旧矩阵／validator／trace或改生产／板；无供给、压力、容量、相位变动。journal4151行／618列／6.7MB，tap宽度和4874bit仅simulation接口；完整在线cost／area／Fmax／gain仍null。
- 完整入口门false，不执行性能四条件；采集额度已用完，不自动第二次、补预热或改相位／映射。后继只接已存在原供给自身产生的B源读/context历史及完整L/P证据，或独立新能力合同；不扩写同纸面图。下方协作候选建议与未准入状态保留，8/9月成果及累计窗口不变。

[观测报告](iommu-platform/docs/aw-natural-entry-observation-20261014.md)／[新原始证据](iommu-platform/out/aw-natural-entry-observation-20261014/README.md)／[裁决](iommu-platform/out/aw-natural-entry-observation-20261014/root-result-acceptance.json)／[下一合同](iommu-platform/out/aw-natural-entry-observation-20261014/next-goal-contract.json)。


- 用户新提出A–H协作/意图/拥塞控制候选；本轮已作有界源码与关键前作预筛，保存root裁决与三份独占分析。当前是机制候选讨论，没有新研究goal、模型或性能实验准入，论文仍在立题。
- 建议先核H/D/G：把未完成后继的页使用与buffer释放义务用于新页准入；基础页级credit+hit/merge通行作强普通基线。另一保留卡是C完成驱动的局部撤销/覆盖保持。两卡均未证明独有收益/完整成本，不把建议当用户已选定主线。
- A宽泛多层在途共享由Neighborhood-aware MICRO2018直接覆盖；B简单主动ATC push与D descriptor预翻译已有技术披露；C连续映射面临Direct Segments/CoLT；G只作协调标签，不替代上下文/权限。E需超出现有N的真实能力，shared reply端口不自动是HOL；F需面对16项精确tag和完整更新/replay费用。
- 原平台源码为四DMA/两bank/单正常TL与PTW/IOTLB16，事务outstanding8不作8个walk。源码InclATS=0，寄存器前端有ADMIT/HOLD，新增TFID/deadline/uTLB接口与当前启动配置仍须绑定；不把源码能力冒充加载bit。
- 上轮AW因果分析已actual complete，E1–E6准确缺项及future-AW/F1 open保持；初稿端点错误与原medium/blocked记录保留。新候选不恢复p5、P/PN差额、R107/C112及其他关闭收益主张。
- 0新实验/model/RTL/driver/ABI/build/板测/旧矩阵或validator回放；本轮仅新增静态候选/证据/文档，Codex主席实质设计与核原文，三席独占关键边界，无DeepSeek/Claude。原约两周立题窗口继续累计，论文未完成。
- 下一判别题：选定一张卡，写清一个可达动作或完整成本差异、同信息强普通对照、正/邻反例及实际接口；差异存活才另立有限模型合同。当前不自动搭完整FLOW架构或恢复旧试验。

[本轮报告](iommu-platform/docs/flow-iommu-candidate-review-20261014.md)／[根裁决](iommu-platform/out/flow-iommu-candidates-20261014/root-result-acceptance.json)／[原始源码与前作入口](iommu-platform/out/flow-iommu-candidates-20261014/root/evidence.json)／[前轮AW裁决](iommu-platform/docs/aw-barrier-causal-20261014.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
