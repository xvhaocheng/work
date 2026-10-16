# 首PTE与普通分段：局部RTL实现、完整COPY结果与裁决

## Material Passport

- ARS-Codex `experiment-agent`，有界局部实现与执行，2026-10-16 JST，`FIRST-PTE-PAIRED-LOCAL-1`。
- 状态：已执行真实DMA/翻译模块的RTL仿真、有限协议单测与局部OOC综合；**行为DDR，不是MIG、FPGA或原生应用测量**。
- Codex-only。M/O请求Sol/xhigh，runner请求Sol/high；实际独立model/effort metadata未知，未声称切换root。
- [原始结果分析](../out/first-pte-paired-local-20261016/root/paired-analysis.json)／[输入合同](../out/first-pte-paired-local-20261016/root/input-contract.json)／[root接受](../out/first-pte-paired-local-20261016/root/root-result-acceptance.json)。

## 1. 一句话决定

**结束这份冻结正常COPY程序、PN配置及cold/hot两种历史上的M-ID收益主张：M与不加意图控制的普通基线完全相同；保留可工作的接口/分段实现，不进入生产开发，不调启动相位寻找胜例。**

M比本次强分段快，不能据此认领接口收益：**本次M没有真正暂缓任何A载荷请求，普通不分段基线已经复现全部服务结果。**这项停止仅覆盖下列程序和输入，不否定其他正常来源中的早意图接口，也不将整个共享IOMMU方向判为无价值。

## 2. 两个实际局部实现

### 共同程序与控制

复用现有actual-DMA system夹具的两真实iDMA、banked TC/TL、PTW、DS合流、xbar及行为DDR。统一使用实际CVA6 vendor AXI/common_cells，避免旧夹具混用两套同名模块。各DMA运行一次batch-OFF、4KiB对齐4096B COPY；V=DMA0，A=DMA1。固定物理源/目的不相交、源在每对COPY期间不变，页表/context保持；只有这两个任务，无合成CPU。根页与DDT移入实际 `[0x80000000,0xc0000000)` DRAM译码窗口。

五种policy源码均已实现，**实际只执行普通不分段、M和O-tail-prepared**。后两项未运行策略是tail-local及whole-job128，不能称它们已通过完整RTL验收。输入在运行前固定：PN（N打开、分区打开）、A先V后、额外启动delay=0、DS前/后50/50、read startup31；所有policy共同增加current-VALID DS-first/F4，非stock免费能力。无返回队列DS优先干预，无queued startup重叠，无参数搜索。

cold是没有warm历史的正常输入，实际12笔PTE；hot先自然执行同策略的一对COPY至quiet，再poison目的数据以防旧结果冒充成功，实际0笔PTE。**没有强制装入旧模型的“V源已收齐、A末段尚待发、仅V目的冷”的内部状态。**这两种历史不等于已经穷尽所有选择性冷启动或正常任务。

时间从配置前quiet开始，包含两DMA真实MMIO配置/NEXT及等待，至两个parent DONE、所有R/W/B及M关联排空。数据初始化/poison和最终测试核验是夹具行为，没有宣称其CPU费用；不称Linux完整生命周期。[固定程序](../out/first-pte-paired-local-20261016/runner/paired_dma.cpp#L2391)中的`paired_program/run_paired`与[整合脚本](../out/first-pte-paired-local-20261016/root/integrate.py)、[观察断言](../out/first-pte-paired-local-20261016/root/instrument.py#L70)给出精确实现。

### M-ID f1/H64

真实normal-S1 start经TL的write、bank及实际DRAM根范围条件形成意图，直接同钟接至DRAM selector，f1本地捕获；固定V0/A1，不新增动态pair epoch。用既有完整PTW ID48，在selector→output-spill输入握手消费P1。pending、7bit倒计数与escape合计9bit；H64只结束抑制，关联保留至P1 commit/真实retire。原lock优先，mask与READY一致，不能撤回held。共享quota3bit属于两侧共同策略。

实现：[M接口/时序说明](../out/first-pte-paired-local-20261016/m/README.md)、[实际gate](../out/first-pte-paired-local-20261016/m/files/tests/m_first_pte_gate.sv#L53)、[TL输出](../out/first-pte-paired-local-20261016/m/files/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv#L704)、[仲裁保持](../out/first-pte-paired-local-20261016/m/files/cva6/vendor/pulp-platform/common_cells/src/rr_arb_tree.sv#L157)。不把两个bank当依赖证明；合并payload口的A头可能间接挡住后续V，真实运行会计入，不声称V完全不受影响。

### O-tail-prepared

只把A最后256beat读拆成128＋128，原prefix256、V两笔256及全部四个写burst保持。复用legalizer、metadata FIFO及AR holder；两个child可提前翻译。RAW=0时用既有metadata位标L0，A backend新增一个sticky done，真L0 RLAST才置位。A TC在L1 physical首次offer以前等待done，复用原AR head、PA和COMPLETE；VALID和退队同时门控，无新增header/payload/tag FIFO。

普通方案的提前准备**实际发生**：cold中L1已在TC等待于t2939，L0真DMA RLAST为t3379，sticky于3380可见，L1在共享selector首次offer为3383。不是为了使M胜出而把普通lookup拖到walk后。真实数据/4B/两个parent及quiet全部通过。详见[O实现](../out/first-pte-paired-local-20261016/o/README.md)和[prepared冷事件原文](../out/first-pte-paired-local-20261016/results/prepared-cold/events.csv#L651)。

## 3. 成对结果与完整成本

下表单位是**本RTL仿真的clock steps**，不与原模型时间或真实FPGA周期混用。`T_data`为双方最后DONE最大值；`T_return`为本夹具最终quiet，均包括40拍配置前缀。所有结果每lane读/写4096B、512个R beat、2AW/2B、1issue/1DONE，错误0、数据校验通过。

| 程序/历史 | T_V | T_A | T_data | T_return | payload AR / 总B |
| --- | ---: | ---: | ---: | ---: | ---: |
| 普通不分段 cold | 4473 | 3518 | 4473 | **4481** | 4 / 4 |
| M cold | 4473 | 3518 | 4473 | **4481** | 4 / 4 |
| O-prepared cold | 4513 | 3558 | 4513 | 4519 | 5 / 4 |
| 普通不分段 hot | 1744 | 1225 | 1744 | **1749** | 4 / 4 |
| M hot | 1744 | 1225 | 1744 | **1749** | 4 / 4 |
| O-prepared hot | 1784 | 1265 | 1784 | 1794 | 5 / 4 |

原始数据：[base冷](../out/first-pte-paired-local-20261016/results/base-cold/result.json)、[M冷](../out/first-pte-paired-local-20261016/results/m-cold/result.json)、[O冷](../out/first-pte-paired-local-20261016/results/prepared-cold/result.json)、[base热](../out/first-pte-paired-local-20261016/results/base-hot/result.json)、[M热](../out/first-pte-paired-local-20261016/results/m-hot/result.json)、[O热](../out/first-pte-paired-local-20261016/results/prepared-hot/result.json)。hot历史自身费用另列：base/M 4481，O 4519，没有免费warm。

**决定性事件，而不只是最终相等：**M cold中A的两次源RLAST为3220、3507；合格V目的walk start及M arm为3514，A parent DONE为3518；P1 local首次VALID为3515，selector接受为3570。M及时、身份匹配，但A源已经全部回来，**实际mask事件为0**。hot没有PTW start，也没有mask。去掉M独有观察事件后，base/M的全部已记录事务事件逐行相等；结构化完整结果除policy标签外也相等。[事件原文](../out/first-pte-paired-local-20261016/results/m-cold/events.csv#L1676)／[比较结果](../out/first-pte-paired-local-20261016/root/paired-analysis.json)。

O-prepared确实多一AR/翻译请求，12/0笔PTE未增加；其`T_data`在cold/hot均多40，但最终quiet差分别38/45。**不将40认作任何系统每个AR固定费用**：原夹具逐request startup、TC占用/回授与完成相位共同作用。行为DDR的R/W通道独立推进，未采用旧Python的单一串行R/W资源。这也是旧2060/2058不能搬来的原因。

### 局部综合：有实际数值，也有明确边界

Vivado2023.1、Genesys2参考part，OOC综合，无place/route。O两组使用同typed端口和同锥：两个完整legalizer＋实际backend ready/sticky与TC匹配/COMPLETE片段；FIFO/AR head/PA/TC旧状态处于边界，没有人为新buffer。

| 同范围O控制锥 | 普通policy0 | prepared policy2 | 增量 |
| --- | ---: | ---: | ---: |
| Slice LUT（组合合并后的工具口径） | 1208 | 1288 | +80 |
| FF | 710 | 711 | +1 |
| CARRY4 | 188 | 202 | +14 |

原语LUT计数另为1402→1512，不能混作Slice LUT。新增sticky在网表保留；原两legalizer各355FF不变。M单独gate实际综合为9FF/28原语LUT，但它**不含TL资格、长线和selector改动，不能拿这个较小锥给M/O全成本排名**。来源及原始报告见[局部成本结果](../out/first-pte-paired-local-20261016/cost/execution-results.md)、[O0 utilization](../out/first-pte-paired-local-20261016/cost/reports/o-p0-cone1-report-fix1/utilization.rpt#L34)、[O2 utilization](../out/first-pte-paired-local-20261016/cost/reports/o-p2-cone1/utilization.rpt#L34)。

映射时未读clock XDC；综合后才用20ns及零外部延迟分析。O0/O2寄存路径data delay估计5.571/7.020ns，不是布局后Fmax或f1长线时序。首次O0/M均在综合成功后遇Tcl报告命令不兼容；一次明确修正后只重做O0及完成O2报告，M不重跑，M时序unknown。软件资格生成、source/owner合同、跨层route、可能新增pipeline、旧槽占用、全系统面积/频率、CPU及实际物理内存收益仍未测。共同2×4KiB payload buffer保留，两侧均无新增payload/header RAM；不由此宣称物理内存收益。

## 4. 关键证据与执行身份

输入顶层`main@0aee28402b8c40155aaee8737e7d4907a9da7be8`；当前CVA6/IOMMU既有工作树bytes作为base，组件与lock差异沿用上轮绑定，没有修改生产。唯一源码新写入本轮独立输出。[M原/新hash](../out/first-pte-paired-local-20261016/m/manifest.json)、[O原/新hash](../out/first-pte-paired-local-20261016/o/base-manifest.json)、各build-manifest绑定实际编译输入，具体行号以这些副本为准。

| 决定内容 | 直接来源 |
| --- | --- |
| 原parent、普通配置/启动、不同冷/热历史 | [runner `paired_program/run_paired`](../out/first-pte-paired-local-20261016/runner/paired_dma.cpp#L2391)，不是旧64次batch包装脚本 |
| 真实MMIO及选定source/vendor/编译参数 | [build入口](../out/first-pte-paired-local-20261016/runner/build.py)、[M实际编译身份](../out/first-pte-paired-local-20261016/build-policy1-v3/build-manifest.json)、[O实际编译身份](../out/first-pte-paired-local-20261016/build-policy3/build-manifest.json) |
| 首PTE/held/READY/H/escape/F4 | [真实gate](../out/first-pte-paired-local-20261016/m/files/tests/m_first_pte_gate.sv#L53)、[有限RTL单测执行](../out/first-pte-paired-local-20261016/m/unit/result.json)、[逐拍原文](../out/first-pte-paired-local-20261016/m/unit/execute.log) |
| descriptor原资源复用、真RLAST sticky、TC同时门控 | [legalizer](../out/first-pte-paired-local-20261016/o/files/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv#L148)、[backend](../out/first-pte-paired-local-20261016/o/files/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv#L485)、[TC](../out/first-pte-paired-local-20261016/o/files/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv#L339) |
| source完成没有冒用frontend permit | [只读观察代码](../out/first-pte-paired-local-20261016/root/instrument.py#L70)记录真实backend接受＋读侧已派完/metadata清空；frontend先进入jobFIFO不算backend开始 |
| 仿真服务原样复用 | runner [`drive_ddr`:652](../out/first-pte-paired-local-20261016/runner/paired_dma.cpp#L652)：AW/W独立ready、读FIFO、startup31、原held-R保护；未调用旧DS-return priority/新服务模型 |
| 实际局部综合与报告失败/修复 | [工具账本](../out/first-pte-paired-local-20261016/cost/execution-ledger.json)、[机器结果](../out/first-pte-paired-local-20261016/cost/machine-summary.json) |

M有限单测覆盖fresh start/P1一次消费、H到期保关联、retire/escape不重放、heldA遇晚意图、READY守恒及F4不抢held。它不是一般AXI/活性证明。现行NO_ERROR_HANDLING共同缺项保留；真实运行为OKAY路径，未声称错误恢复已完成。

编译准备中修复的缺失fifo_v2、观察端口拼写和C++ packed/unpacked类型问题均有失败日志；不是将失败实验重标成功。旧版Verilator不支持`--binary`，单测改C++时钟驱动、DUT不变。**新性能模型0、旧70重跑0、生产/ABI修改0、全板构建/布线/加载/板测0**。实际执行止于这三个policy的cold/hot及必要单测/局部综合，不扩大tail-local/job128性能矩阵。

## 5. 论文贡献判断

| 判断层次 | 本轮结论 |
| --- | --- |
| 功能可实现 | M及强O-prepared已整合运行、完整工作守恒；M特定保持/退出边界有有限RTL单测。tail-local/job源码保留，未运行验收。 |
| 条件因果链成立 | 旧条件模型仍支持存在有利首PTE顺序，cold/hot/短gap/长gap/迟到边界不变；本轮没有把它的选择性冷/特定owner前缀重现成正常运行。 |
| 相对普通方案有差异 | M保留原burst；prepared实际提前翻译且多一个child。新接口与分段在状态/线路/时机确有不同，已有实现证据。 |
| 差异有实际价值 | **这份程序中没有M的增量价值：O0无意图控制就逐事件复现完整结果。**相对主动分段更快，不构成M有效。局部较小电路锥也不能越过这个对照。 |
| 独立论文增量 | 本版本不支持论文方法主贡献；不再用未知完整成本延长它。接口家族在其他来源的价值未被否定，也未被本轮证明。 |

最近工作沿用[上一轮具名前作比较](first-pte-interface-cost-20261016.md#5-论文贡献判断)：MASK覆盖翻译/数据联合调度，AXI-REALM覆盖透明合法分段，iDMA覆盖legalizer、读写解耦及fullbuffer原语。本轮没有新查新，也不从“接口在本平台新增”推首创。真正结束当前主张的是**可执行普通基线的相同完整服务＋M零实际动作**，不是模型投票、前作名称或泛泛“缺创新”。

## 6. 后继裁决

**本次判别已结束，不再为该程序自动增加协议、窗口搜索或生产实现任务。**未执行的O-local/job性能与O1/O3综合止于源码资产，不为凑矩阵继续。MIG、B1/I/D/A5/A2、映射采集、p5与D-CS1均保持已有边界；原10月15—28日窗口不重启。

若今后重新讨论早意图，需要的是**另一个正常来源自然提供“合格V walk启动时，独立A载荷尚未首次承诺”的有用窗口，并与普通不分段/合法提前准备一起比较**；这不是本轮已满足条件，也不是一份自动排队的后继合同。当前没有必要因完整route/CPU成本仍unknown而追加开发。
