# 当前状态：问题发现合同已准备，实验尚未运行

2026年10月17日（JST）。已按用户修正分开“问题发现、机制原型、论文贡献”：**尚无创新机制不禁止有价值、可区分且有上限的探索。**DISCOVERY-REUSE-1已形成具体合同和源码可行性：固定源/供给，只改变A目的再用距离，P/PN共6个基础条件，最多2个有条件普通大页诊断。用户材料明确此建议尚非新实验授权，因此本轮未改夹具、未运行。总入口为[计划](研究计划.md)与[导航](研究导航.md)。

## 全局安排与本轮裁决

- 唯一待执行提案：两真实iDMA各64个normal4096、batch-OFF COPY，源固定；V目的不变，A目的环R=1/8/16；普通P与PN同享承诺前DS-first/F4。初始冷历史及全部配置/补发/B/quiet纳入，不假装stock或原生CPU/DDR。
- 竞争解释为翻译链、已承诺payload/回压、供给/写腿/完成发现；逐parent身份和真实承诺端点取证，重叠不相加为可回收收益。仅在R16已有重复walk/必要等待证据时，最多加P/PN各一A专用池2MiB叶诊断；不为凑数补点。
- 可行性核查不是执行：paired原入口只有一对COPY，需新独立测试补发器/逐parent核验及只读观察；legacy aw_wait_change在旧paired full-era不落盘，不能冒称已有owner因果链。所核E2/E3/M0/旧70/六case均非本合同同输入。
- RECONVERGENCE-1三个版本停止范围保持：前两项机制增量不足，第三项问题尚未证实，不合并为三次机制实验失败。First-PTE仍为base=M4481/1749、A3507早于arm3514、mask0，不运行M翻案；旧负结果不改写。
- 无论文主线成立结论；发现结果才决定缩小范围、普通优化或围绕一项差异推导机制。B1/I/D/A5/A2、mapping/p5/D-CS1与MIG暂停保持，原10月15—28日窗口不重启；MIG WNS−0.391ns/598setup/0newloads/debtunknown。

[问题发现合同](iommu-platform/docs/problem-discovery-contract-20261017.md)／[夹具可行性](iommu-platform/out/problem-discovery-plan-20261017/fixture-feasibility.md)／[已有覆盖与观察缺口](iommu-platform/out/problem-discovery-plan-20261017/existing-coverage.md)／[三道门规则](docs/agent-policy.md#问题发现机制原型与论文贡献)／[前轮三个机制裁决](iommu-platform/docs/research-reconvergence-20261016.md)／[First-PTE结果](iommu-platform/docs/first-pte-paired-local-20261016.md)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮仅已有合同/源码核查、地址算术及计划/规则修改；夹具实现、编译、模型/仿真、综合、生产修改、加载/板测均0。前轮局部RTL/OOC结果原样保留；NO_ERROR_HANDLING缺项不变。Codex-only；两个独占核查席请求Sol/high，实际独立metadata unknown，未声称切换root。MIG暂停不因问题发现方法修正而解除。

## 独立工程事项

- GitHub 同步：已建立并上传公开仓库 [xvhaocheng/work](https://github.com/xvhaocheng/work)，分支 `main`；803,869个Git条目，16个大文件无损分片。26项同步工具测试通过，远端提交／文件树与本地一致，原内部仓库元数据保持。源码、研究记录与历史归档保留，构建产物排除；不改变研究安排。 用户已授权以后项目修改完成并检查后同步提交推送到 `work/main`，详见 [持续同步规则](AGENTS.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
