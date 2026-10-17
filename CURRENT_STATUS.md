# 当前状态：问题发现完成，替换局限与已承诺读债务已定位

2026年10月17日（JST）。用户在具体合同后回复“继续”，DISCOVERY-REUSE-1已实际完成一次局部基座编译、6个基础＋2个触发诊断，1024个parent逐笔与独立原始事件核验通过。**取得新事实，当前归普通优化依据，未立新机制主线。**总入口为[计划](研究计划.md)与[导航](研究导航.md)。

## 全局安排与本轮裁决

- 两真实iDMA各64次normal4096 COPY，只变A目的再用R1/8/16；普通P/PN共同DS-first/F4，非stock；冷前缀、配置/补发、双方B/DONE与quiet全部保留。
- R8时A足迹9页却有39次walk。源码推导为masked tree-PLRU热源及不等子树偏置；页级偶数反复miss/奇数仅冷miss精确吻合，内部victim未直接采样。名义14项mask不等于14项有效驻留保证。
- 4KiB下P/PN同walk数，PN完整时间分别慢0.70%/18.03%/28.39%；R16具体后续PTE在已承诺V载荷后等待461步，DS-first无法改这个前驱。局部排队不当作全部可回收收益；软件供给、读腿、写腿分别报告。
- 普通A专用池2MiB诊断均完整75797步/3初始walk；P/PN parents记录字节相同，PN仅多2个未接受的物理命中probe。本arena普通组织已足够；普通替换修正仅作未执行的后继校准建议，不自动开发新接口/控制器或补更多case。
- RECONVERGENCE-1三个具体版本、First-PTE base=M4481/1749及mask0等历史裁决保持。B1/I/D/A5/A2、mapping/p5/D-CS1与MIG暂停不变，原10月15—28日窗口不重启；MIG WNS−0.391ns/598setup/0newloads/debtunknown。

[发现结果](iommu-platform/docs/problem-discovery-results-20261017.md)／[实际输出与复现](iommu-platform/out/problem-discovery-run-20261017/README.md)／[root接受](iommu-platform/out/problem-discovery-run-20261017/acceptance.json)／[冻结合同](iommu-platform/docs/problem-discovery-contract-20261017.md)／[原三个机制裁决](iommu-platform/docs/research-reconvergence-20261016.md)。

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮仅新独立测试副本、只读观测、一次局部Verilator编译和8次新仿真；无失败条件、旧实验重跑、生产修改、综合/布线或板测。行为DDR时间非FPGA/原生时间，NO_ERROR_HANDLING缺项不变；已接受原始事件gzip逐字节可恢复。Codex-only；C++请求Sol/xhigh、观察/归约Sol/high，实际独立模型metadata unknown。MIG继续暂停。

## 独立工程事项

- GitHub 同步：已建立并上传公开仓库 [xvhaocheng/work](https://github.com/xvhaocheng/work)，分支 `main`；803,869个Git条目，16个大文件无损分片。26项同步工具测试通过，远端提交／文件树与本地一致，原内部仓库元数据保持。源码、研究记录与历史归档保留，构建产物排除；不改变研究安排。 用户已授权以后项目修改完成并检查后同步提交推送到 `work/main`，详见 [持续同步规则](AGENTS.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
