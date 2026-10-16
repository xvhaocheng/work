# 当前状态：AW准备完整因果裁决已形成，按准确缺项结束（2026-10-14）

- 同一实际goal1791959885，无额外token预算；用户恢复后actual root gpt-6.1-sol/ultra/native有效475000，0新席/旧席恢复。原medium/blocked回执保留；工具入口仍记blocked，未伪造active或另起goal。
- 决定性纠正：原upstream mux默认AW spill两槽，private AW单槽不是到input demux的全部容量。coupled第二AR需DMA源側AW握手，可能早于input AW接受/B raw WLAST；初稿跨端点不等式已撤回并完整归档。
- 新入口必要条件：同currentA的A.AW0若在原AW序列排在B.AW1前，normal256beat/18beat preoutput容量界限使B2/A0不可达。原B.AW1-before-currentA.AW0及actual options/cache/job/P snapshot未取得，不人为延后A造入口。
- 完整AW源/输入/target/output、两段R、raw/output W、各B/lastmetadata/错误/退休与原后继供给已写入因果合同。samebank write不能用N穿过自身active读P；FullBuffer不能证明accepted decouple/deburst。
- 局部next-PTE准备重叠的WAIT已与frozen dualGap动作匹配；AW遮蔽只接受局部因果，未作统一全任务ordinary覆盖。未建立完整独有收益，不判论文创新/全族失败。
- 本轮按准确缺项E1–E6结束：原快照、有序供给/完整history/共享RWB反馈/独立B/完整原工作/费用及完整选择差异未闭合。未来AW包络/F1/完整准入全族open，全部既有关闭结论保持。
- 费用包括原生两槽前瞻的读取/关联、所有AW端点/未发工作/history/端口/算术/maxsum/held公平/延迟/CDC/原CPU供给。68bit只组件小计；完整数字cost/area/Fmax/actual完成/netgain均null。
- 0新观测/model/experiment/build/test/旧trace-validator replay/生产RTL/driver/ABI/FPGA/板/压力容量walker并发时相延迟。未进入新四条件，microbenchmark不准入，8/9月资产保留。
- 下一门只接新同源完整原始证据包或明确新合法能力；不自动扩大同纸面图或source hunt，source-only授权保持。论文未完成，原立题窗口累计。
- 稳定产物及五文档同步已形成；仅完成本轮必要新交付/需求核验后写actual complete，当前尚未complete。

[报告](iommu-platform/docs/aw-barrier-causal-20261014.md)／[证据导航](iommu-platform/out/aw-barrier-causal-20261014/README.md)／[裁决](iommu-platform/out/aw-barrier-causal-20261014/root-result-acceptance.json)／[准确缺项](iommu-platform/out/aw-barrier-causal-20261014/root/missing-evidence.json)／[下一门](iommu-platform/out/aw-barrier-causal-20261014/next-goal-contract.json)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。
