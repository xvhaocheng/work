# 当前状态：首PTE局部RTL判别完成，冻结COPY中的收益版本结束

2026年10月16日（JST）。FIRST-PTE-PAIRED-LOCAL-1已完成真实DMA/翻译模块＋行为DDR的局部RTL执行及小模块综合。**M与普通不分段基线的完整结果和已记录事务事件相同；V写walk启动时A源已全部读完，没有实际门控机会。结束这份固定程序/PN/cold-hot历史的M-ID收益主张，不调相位续造胜例。**接口与普通分段实现保留，不外推整个早意图家族或共享IOMMU方向无价值。未选论文主线；[总计划](研究计划.md)与[导航](研究导航.md)继续为统一入口。

## 全局安排与本轮裁决

- 三个policy实际运行cold/hot：普通不分段与M最终quiet均4481/1749；强O-tail-prepared为4519/1794。单位是RTL仿真clock steps，非FPGA/MIG/原生应用时间。每lane4096B读写、512R beat、2AW/2B、1issue/1DONE、数据及quiet全部通过。
- M cold：A源最后RLAST3507，V目的walk/M arm3514，P1 shared接受3570，实际mask0；hot无walk。M比主动分段快不构成贡献，因为普通O0已完全复现M。同一正常程序从配置前quiet出发，不强装旧模型前缀；本轮cold全冷12PTE/hot0PTE，未测试所有选择性冷历史。
- O-prepared真实提前翻译并复用TC持有：L1 wait2939、L0真RLAST3379、done3380、L1共享首次offer3383；新增1sticky、无新header/payload FIFO。M有限RTL单测覆盖held/READY、P1、H/escape与F4；不是一般协议/恢复证明。tail-local/job源码保留、未跑性能，不继续凑矩阵。
- 同锥O局部综合prepared比base +80 SliceLUT/+1FF/+14CARRY4；M helper为9FF/28原语LUT，范围不同不能作全成本排名。20ns仅综合后分析，无布局布线；完整route/CPU/物理内存收益unknown，不因此延长本版开发。
- 本轮任务结束，无自动下一gate、协议或生产任务。旧70及短gap/长gap/late条件结果保留未重跑。B1/I/D/A5/A2、mapping/p5/D-CS1保持；MIG1792023314 paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；原10月15—28日窗口不重启。

[六段结果与裁决](iommu-platform/docs/first-pte-paired-local-20261016.md)／[原始结果分析](iommu-platform/out/first-pte-paired-local-20261016/root/paired-analysis.json)／[实际输入](iommu-platform/out/first-pte-paired-local-20261016/root/input-contract.json)／[root接受](iommu-platform/out/first-pte-paired-local-20261016/root/root-result-acceptance.json)／[局部成本](iommu-platform/out/first-pte-paired-local-20261016/cost/execution-results.md)／[验收记录](iommu-platform/out/first-pte-paired-local-20261016/root/verification.json)／[上一轮源码设计](iommu-platform/docs/first-pte-interface-cost-20261016.md)

## 各分支最新进展

- A5保留源级完成域/等待证书：HERO AXPY四槽、32768元素/64tiles、cold/hot两episode。RX/RY外侧R与本地B分开，单DMA流水不证明实际共享翻译竞争，同SID0/SSID0及xbar同ID跨目的约束仍须准确处理；O-T1只等最后RY一个ID，O-G双ID/Y复用guard开放。变体未应用/编译。[A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md)。
- A5冻结两源级切面，新增一次离线依赖核对含两episode/384jobs及最终释放，与同信息标准DAG一致；并非周期/原生性能正反例，完整时间null，1正1反2时间留出未冻结。下一局部决定仍核xbar/接收能力与自然窗口；不自动恢复A全局优先。[A5接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[下一合同](iommu-platform/out/a5-wait-scope-contract-20261015/next-decision-contract.json)。
- A2构建/分层检查保留，新内核/真实DMA未运行；Hero三个硬件hook仍缺实现/default deny。B1/Q/G及旧负结果按具体版本保留，经研究导航定位，不自动恢复旧实验。本批写入前的A5状态全文在[快照](iommu-platform/out/research-plan-implementation-20261015/before-final-write/CURRENT_STATUS.md)。

## 硬件与执行状态

本轮执行6个固定局部RTL case、一次有限M单测及4次小控制锥OOC综合调用（2次综合成功后报告阶段失败、修正后2次报告完整）；无生产RTL/Linux/ABI修改、全板构建、place/route、加载或板测，无新性能模型/旧70重跑。复用行为DDR，R/W独立推进及每request startup为夹具规则，不能套旧2060/2058或称原生性能。现行NO_ERROR_HANDLING共同缺项保留。Codex-only；M/O请求Sol/xhigh、runner请求Sol/high，实际独立运行metadata unknown，未声称切换root。硬件暂停与既有时序边界保持。

## 独立工程事项

- GitHub 同步：已建立并上传公开仓库 [xvhaocheng/work](https://github.com/xvhaocheng/work)，分支 `main`；803,869个Git条目，16个大文件无损分片。26项同步工具测试通过，远端提交／文件树与本地一致，原内部仓库元数据保持。源码、研究记录与历史归档保留，构建产物排除；不改变研究安排。 用户已授权以后项目修改完成并检查后同步提交推送到 `work/main`，详见 [持续同步规则](AGENTS.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

- 历史进度导航：[从7月起的研究进度回顾](iommu-platform/docs/research-progress-from-july-20261014.md)。已串联多DMA扩展、9月因果测量、10月方法筛选及当前AW证据缺项；8月逐日记录仍有日期缺口。本次仅回顾留存，不改变当前研究裁决或后继授权。
