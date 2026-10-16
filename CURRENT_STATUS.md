# 当前状态：首PTE与legalizer成对设计完成，建议一次局部实现判别

2026年10月16日（JST）。FIRST-PTE-INTERFACE-COST-1已完成源码驱动的两个具体设计。**建议只将M-ID同拍捕获与复用TC保持的O-tail-prepared作一次局部成对实现；结束“票据天然少状态、更便宜”的论证。**未选论文主线，未实现生产RTL，未取得新性能或成本测量。[研究计划](研究计划.md)为唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一判别

- M-ID以真实normal-S1 write start、同钟固定传播及既有ID关联P1；P1最早C3到共享selector，f1在C0生效。f2身份仍安全但可能错过L；held不可撤，H到期只停gate不清关联。静态资格绑定实际独立COPY及DRAM内S1根页表，不以bank/busy或timeout替代证明。
- 普通legalizer可复用cap、metadata FIFO及AR holder完成paced读，local版可无独立pacing状态；frontend最后B不是读credit。强O-tail-prepared提前翻译并用既有TC保存L1，只新增1个完成sticky与回线，不需新header FIFO。实际TL等待/回授与M跨层线时序不同，尚无净胜结论。
- 原70模拟／82检查结果只复用、未重跑：cold普通2066／票据2060／split零暴露2058；hot2050，short票据慢1，long快9，late失效。a_split占共享server，不是多AR固定拍数；新接线改变次序时不能在旧数加常数。同接口等价只排除独占调度主张。
- 唯一后继提案FIRST-PTE-PAIRED-LOCAL-1：复用完整COPY夹具，局部实现与同约束控制锥成本比较，允许O合法提前准备；本轮未执行、不自动启动。若O以不高于M完整成本覆盖服务，或固定正常程序没有可用窗口，结束这个COPY/M-ID成本版本，不转成新协议链或自动升位其他方向。
- mapping、B1/I/D/A5/A2及独立工程成果保留各自边界。MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；原10月15—28日窗口不重启。

[六段比较与裁决](iommu-platform/docs/first-pte-interface-cost-20261016.md)／[M设计](iommu-platform/out/first-pte-interface-cost-20261016/m/source-design.md)／[O设计](iommu-platform/out/first-pte-interface-cost-20261016/o/source-design.md)／[root接受](iommu-platform/out/first-pte-interface-cost-20261016/root/root-result-acceptance.json)／[唯一后继合同](iommu-platform/out/first-pte-interface-cost-20261016/root/next-minimal-contract.json)／[本轮检查](iommu-platform/out/first-pte-interface-cost-20261016/root/verification.json)／[原条件模型](iommu-platform/out/mechanism-construction-20261016/model/README.md)

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮仅定向源码/已有原文核查、状态机与偏序设计、只读结构/算术辅助；新性能模型、旧70重跑、RTL仿真、生产RTL/Linux/ABI修改、构建、综合、hostprobe、板测均0。源级设计不等于协议/活性验证；现有error强制0是两侧共同缺项。当前源码overlay/组件HEAD与lock差异已绑定，不代表既有bitstream身份。Codex-only；M/O请求Sol/xhigh，结果/前作核对请求Sol/high；实际独立model/effort metadata unknown，未声称切换root。MIG暂停与上述实际时序边界保持。

## 独立工程事项

- GitHub 同步：已建立并上传公开仓库 [xvhaocheng/work](https://github.com/xvhaocheng/work)，分支 `main`；803,869个Git条目，16个大文件无损分片。26项同步工具测试通过，远端提交／文件树与本地一致，原内部仓库元数据保持。源码、研究记录与历史归档保留，构建产物排除；不改变研究安排。 用户已授权以后项目修改完成并检查后同步提交推送到 `work/main`，详见 [持续同步规则](AGENTS.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
