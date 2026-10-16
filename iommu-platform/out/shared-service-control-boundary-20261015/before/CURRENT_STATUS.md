> actualgoal1791990424已实际complete；五页同步及334项新交付检查先于complete。工具累计480,283 tokens／39分40秒，无额外token预算。[原样回执](iommu-platform/out/local-read-reclaim-20261015/root/goal-completion.json)。LR1归普通源阶段接口，0新性能条件，论文未完成。

# 当前状态：LR1归普通阶段接口，未进入性能条件（2026-10-15）

- actualgoal1791990424已实际登记，无额外token预算；技术裁决稳定，五页同步与必要新交付检查后才actualcomplete。论文未完成、原立题窗口累计。
- 新LR1私有原型八组122项／八完整trace通过：源全R且old owner退休、PTE/cache/inval/targetorder完成后归还；CPU覆盖sourcepage4096B，旧目的oldbytes完整写完，无关DMA继续。untimed参考，不是native缓存/PTW/DDR/FPGA。
- 三独立ordinary交付；phased/targeted完整事件、值及正常R/W/B复现LR1，phased还复现角色交换/错误终结。同选定primitive/controller逻辑成本差0，没有本版独有动作或完整可实现成本优势，性能主张关闭。0四性能条件，native窗口/面积/Fmax/CPU/PTE-CQ-cacheline/应用收益null。
- 原生最终R→最后W/B的因果偏序成立，不需人为延迟；新ACK开销后是否仍有收益窗口未验。封口按原JobFIFO已接受parent，未发第二burst仍计旧debt并继续正常drain，不撤heldVALID、不以depth8偷看未来任务。
- Intel官方确有domain/page范围相关drain，但RC取完data不同于DMA全页Rreceipt。source-page ACK不冒globalIOFENCE；现CQ PR/PW TODO且CQH先于decode、现source RW/launcher仅64B/精确失效等缺接口，补标准能力不算创新。64Bfullbuffer无例外，原4096资格保持。
- root亲核11files/22原文及primary；新source席核35files/118原文。root+两新席actual Sol6.1/ultra/native475000/fork-none，Codex-only；0旧实验/模型/validator/trace/build回放、关闭分支/旧席恢复、生产RTL-driver-ABI/FPGA或压力/容量/walker增加。
- 后继不自动开源核/模型/工作流/RTL。继续方法须具体需求超出ordinary phase+selective drain及新动作/完整实现成本；不重命名相同阶段脚本、不以未补native baseline制造差异，不自动转页交换/表征。SDH1/BR1/RCS1等原裁决及8/9月成果保留。

[完整报告](iommu-platform/docs/local-read-reclaim-20261015.md)／[实际goal导航](iommu-platform/out/local-read-reclaim-20261015/README.md)／[根裁决](iommu-platform/out/local-read-reclaim-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/local-read-reclaim-20261015/next-goal-contract.json)。旧五页逐字归档，以下独立维护原文保留。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
