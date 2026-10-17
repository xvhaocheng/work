# 上一轮细粒度失效使A变慢：RTL与周期来源

本报告只解释上一轮独立Verilator夹具，不外推Linux、FPGA或板上周期。输入是`out/invalidation-interference-20261017/frozen`：CVA6 `ba8cdd10659a777ccff0080bf020be64b5c1e064`、IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`，原版16项PLRU加上一轮列明的必要修复；无P/PN、policy_mask、提前命中旁路、DS-first/F4研究调度。下面行号均指该实际frozen源，不能替换成活动checkout行号。

## 已排除的解释

**宽、PSCID、VA失效没有不同的RTL执行拍数。** CQ对三者都在同一个DECODE状态转出，IOTINVAL只复制AV/GV/PSCV/VPN/PSCID等字段后产生`inval_vma`，没有按范围选择的延长状态或ready握手（[CQ L297](../../invalidation-interference-20261017/frozen/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv#L297)–338）。IOTLB在一个组合`for`内并行处理16项，GV/AV/PSCV只选择比较条件（[IOTLB L157](../../invalidation-interference-20261017/frozen/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv#L157)–264）；所有valid在同一上升沿从tags_n写入tags_q（L367–375）。没有16拍扫描或范围越精细越多拍的实现。这是RTL周期结论，未测FPGA组合路径/频率。

**不是旧A页表walk数增加。** different-b4中m1/m2/m3均为A128次walk、3072B PTE读、128个cold miss、0个replacement miss。细粒度保留B的翻译，改变B进入内存的数据流节奏；这不意味着A自己的翻译工作增加。

## 原trace已经能定位的周期差

[prior_cycles.py](prior_cycles.py)只读旧gzip CSV和summary，使用半开区间`[begin,end)`，校验所有A COPY/flush区间互不重叠、flush之和等于旧summary，再保存[prior-cycles.json](prior-cycles.json)。没有重跑旧实验。

下表以different-b4、full-api为例，完整A时间取最终失效结束；COPY为各job的真实issue到真实DONE，residual包含配置、DONE轮询和状态推进等其余时间。

| 可加的互斥区间 | m1宽 | m2 PSCID / m3 VA+PSCID | 差值 |
| --- | ---: | ---: | ---: |
| A COPY issue→DONE合计 | 91,611 | 73,105 | −18,506 |
| A flush等待合计 | 99,110 | 136,858 | +37,748 |
| 其余A wall-clock | 4,028 | 3,712 | −316 |
| A最终失效完成 | 194,749 | 213,675 | +18,926 |

硬件最后DONE的旧`T_A`是193,968→213,462（+19,494），与最终失效完成口径不同，不能混用。旧[宽summary](../../invalidation-interference-20261017/runs/matrix01/different-b4-m1-full-api/summary.json)、[PSCID summary](../../invalidation-interference-20261017/runs/matrix01/different-b4-m2-full-api/summary.json)保留原值。

同一flush区间内，以旧`cq_state`变化事件再作互斥状态占用分解：

| CQ状态占用 | m1宽 | m2 / m3 | 差值 |
| --- | ---: | ---: | ---: |
| IDLE0 | 8,764 | 8,178 | −586 |
| FETCH1：本地AR接受之前 | 895 | 768 | −127 |
| REGISTER2：命令R返回与组装 | 88,683 | 127,144 | +38,461 |
| DECODE3 | 768 | 768 | 0 |
| WRITE4 / ERROR5 | 0 | 0 | 0 |
| 合计 | 99,110 | 136,858 | +37,748 |

因此旧额外等待已定位到CQ **REGISTER等命令读数据返回**，不是DECODE比较范围，也不是marker写回。REGISTER已经从CQ接口获得ARREADY，但仍可能在DS spill、xbar、DDR排队/服务或返回缓冲中等待；仅这个状态占用不能将38,461拍全部归因到某一个前驱。

b8对应宽194,749=91,611+99,110+4,028；精细214,261=73,591+136,964+3,706。精细CQ REGISTER为127,309拍、DECODE仍768拍。same-domain的m2与宽相同，same-domain的m3与上述精细相同，符合PSCID覆盖范围；四个域/页数组合的完整12行都在JSON。

## 为什么命令读会与载荷竞争

1. CQ从head≠tail开始取16B命令，FETCH等ARREADY，REGISTER等待两拍R，最后R拍先令CQH加一，再在下一拍DECODE（[CQ L240](../../invalidation-interference-20261017/frozen/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv#L240)–299）。即使AV/PSCV不同，命令都只有两拍64bit数据。
2. CQ、PTW、DDTW在DS接口共享七路AR仲裁和一个非bypass spill；R按ID回送，PTW=0、DDTW=1、CQ=3（[DS L77](../../invalidation-interference-20261017/frozen/cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv#L77)–109，L221–272）。没有为CQ保留独立内存通路。
3. 独立top把完成接口comp和DS分别接入xbar slave2/3，再都送memory0（[上一轮top L230](../../invalidation-interference-20261017/runner/invalidation_dma_top.sv#L230)–253）。payload与页表/CQ到最终行为内存处会合。
4. 行为DDR为全局readq FIFO；只有队首倒计时，先16拍无RVALID，再每拍一beat；非队首不老化，接受的新AR追加队尾（[上一轮C++ L193](../../invalidation-interference-20261017/runner/invalidation_dma.cpp#L193)–202、L282–304、L329–335）。它不重排短CQ/PTE到长payload之前。因此即使CQ仅16B，其服务可能等在前方256beat载荷之后。读队列容量与R返回ready还会反向施压。
5. 旧宽失效增加B walk，在B的payload发起前插入串行翻译；细粒度使B命中更快、payload推进更密。源码和旧周期共同支持“B流量节奏改变，CQ返回等待增加”的待拆细解释。真正是谁排在某条CQ前面、多少周期处于服务前等待，须用本轮逐周期readq/请求身份确认；不能仅用时间重叠完成因果归因。

本地必要修复包含AW-before-AR进度/排序。事务控制器同拍有AW/AR时优先选择AW（[TC L578](../../invalidation-interference-20261017/frozen/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv#L578)–621），所以不能假设A读腿总先进入TL。TL正常请求在spill中串行持有到响应；`req_valid`和owner有原始来源（[TL L137](../../invalidation-interference-20261017/frozen/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv#L137)–184）。PTW MEM_ACCESS发AR，PROC_PTE等R（[PTW L382](../../invalidation-interference-20261017/frozen/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv#L382)–395）。本轮监测暴露这些握手，但不更改先后规则。

## IOFENCE的实际含义

原CQ的PR/PW支持仍为TODO（[CQ L348](../../invalidation-interference-20261017/frozen/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv#L348)–392）。PR/PW仅作为结构字段存在，没有读写计数drain；把命令位设为1也不会使该RTL提供设备读写全局排序。ML编码可忠实置PR=PW=1，但不能称此RTL已经原生实现主线Linux所要求的ordering。

ATS关闭、AV0时合法IOFENCE在DECODE直接回IDLE；本轮`dbg_iofence_complete`只观测这个具体状态机的提交。AV1进入WRITE，AW→W→B完成OK后回IDLE（[CQ L466](../../invalidation-interference-20261017/frozen/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv#L466)–491）。原CQH在命令取回后就前进，因此不能普遍把CQH越过命令当作marker/B完成。上一轮legacy每轮IOTINVAL+AV0 fence再单独提交AV0哨兵；完整费用包含三条fetch与相应doorbell/poll。新ML减少哨兵的费用必须按自己的真实执行路径核算。

## 本轮PR/PW模型测试hook（与原frozen分开）

为执行本轮要求的AV0、PR=PW=1 strict完成过程，新增独立CQ/SW overlay；没有修改上一轮frozen或生产源码。处理流程为：

1. 命令低64bit在第一个R beat锁存。REGISTER最后beat识别IOFENCE且PR/PW非零时保留CQH；其他命令/legacy PR=PW=0仍在原拍推进CQH。
2. 首次DECODE捕获root C++的comp口已接受设备请求cohort（包括同拍），读/写各自固定目标。模型DDR payload RLAST和B分别是本模型全局点，后续请求不扩大目标。
3. 合法AV0/ATS-off命令按PR/PW需要的done信号决定是否立即CQH++回IDLE；否则进入新增WAIT_PRPW6，DECODE不重复。等待达成后才CQH++回IDLE，并发出`dbg_iofence_complete`。
4. CQ WAIT只禁止它自己新发命令存取，不回控trans_ctl、TL、DS仲裁其他输入或comp。先前已被设备接口处理的请求可继续完成；等待期间不会因为CQ不发AR而关闭payload/PTE通路。
5. PR/PW非零结合AV1或ATS inflight不在本测试hook范围，显式报cmd_ill；旧AV1/PRPW0 smoke保持原行为。

实现文件为`runner/rtl-overlay/.../rv_iommu_cq_handler.sv`，输入经`rv_iommu_top.sv`和`rv_iommu_sw_if_wrap.sv`穿透，均置于`ifndef SYNTHESIS`内。移除这些guard内的全部新增代码后，五份overlay均已静态核对与原frozen代码一致（忽略空白）。v1原diff留在`diagnosis/overlay-v1`，不改build01快照。

这是为同负载仿真补齐的**明确测试实现**，不是原F5已经支持PR/PW，也不是Linux CPU/native运行。完整ordering主张限于固定设备cohort和本模型全局点；root C++必须实际断言目标满足、接受数/完成数守恒及请求不被等待反向卡住。统一build02与运行结果仍应绑定该overlay和C++实际hash。

## 本轮互斥wall-clock与重叠诊断

建议以A完整窗口`[t0, final_a)`逐拍选**一个**主类别，固定优先级：flush忙→COPY(issue已发生且DONE未发生)→host/config/其他。COPY内再按固定优先级细分为A拥有活跃PTW、A payload readq尚待服务、其余COPY。边界采用上升沿前观察状态，计数之和必须等于A完整时间，flush/COPY各有begin/end事件可回算。

CQ/PTW各stage可另拆ARVALID&&!ARREADY、AR已被接受但命令/PTE不在DDR队列、DDR队列服务前、DDR队首startup、R数据服务、返回缓冲、decode/计算等。只有显式request identity才能区分“尚未进入队列”和“已进入又已离开”；不能只见PROC_PTE就称DDR排队。读队列中一个请求位于前方不是它占满整个等待区间的证明；每拍按真实队首和startup/service状态记账。

重叠计数（例如A活跃PTW同时B payload服务、CQ等待同时A配置）单独保存，可以解释并发，但不与主类相加，也不能当作移除该资源后可回收的周期。A-alone与共跑差、command时间与完整A时间分别比较。

## 死条目与ORACLE观察

IOTLB update优先级低于失效；fill仅写PLRU选中way（[IOTLB L304](../../invalidation-interference-20261017/frozen/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv#L304)–319）。PLRU树只在lookup命中时改写；victim完全由树决定，没有valid/空项优先，fill分支没有专门更新tree（L328–364）。调用链需保留一个细节：TL仍在PT状态等`iotlb_lu_hit`才完成（[TL L1136](../../invalidation-interference-20261017/frozen/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv#L1136)–1138），因此成功fill后下一拍通常会重lookup并命中新条目，再经`lu_hit`更新tree。不能把“无fill端更新”写成“新条目从不成为MRU”。

对本轮永不重访的死A key，可以作更强但有明确条件的推导：在**相同请求/命令时序**下，若ORACLE只把未来不会再匹配任何lookup的A key置invalid，则所有后续活跃key命中相同，`lu_hit`相同，PLRU树相同，victim选择也相同。对每个fill，它会在两边覆写同一way，不因某边已有invalid而改选。按每个时钟沿归纳，清除死valid本身无法让B避免替换；死条目占比高不等于减少了这里实际可用的victim位置。这个推导依赖A地址不再使用、无global/大页别名及其他valid观测不反馈控制；本夹具的A source/destination每job唯一，B IOVA范围分离，均4KiB/G0，C++仍须实测核对。

本轮ML-FQ/ORACLE使用完全相同的FQ容量、10ms计时、域失效和PR/PW fence；ORACLE仅在unmap后额外清valid，不增加CQ命令或等待。因此这里具有同供给/同命令政策的直接配对，预期除ORACLE删除及valid观测外，关键服务事件可逐条逐周期完全一致；本轮分析脚本将核对lookup（不把miss归因aux当服务差）、ddr_ar、copy_launch/DONE、walk和CQ decode。若这些实测相同，死条目移除在本实现中没有改变服务；若不同，应保留首次分歧并回溯。相比之下，上一轮M3与M0确实改变命令时序，更窄失效下的B walk下降可能受交错影响，不能直接宣称死容量因果。本轮不改PLRU以制造ORACLE收益。本轮输出update、selected way、hit vector、树和全部旧tags，让C++按实际unmap追踪死key。

ORACLE只存在于`runner/rtl-overlay`的三份独立副本，输入穿过top→TL→IOTLB；匹配S1 host的PSCID+4KiB VPN，在tags_n的最后赋值中清valid，下一沿立即生效。该即时清除动作本身不发CQ、不加等待、不清PLRU；后续FQ域失效/fence仍与ML-FQ完全相同，也不是生产实现；输入、端口和清除逻辑均`ifndef SYNTHESIS`。同沿同key的fill也会被最后清除，观察者须记录或排除这种交集。完整端口定义见[ports.md](ports.md)，可重建变更见[make_monitor.py](make_monitor.py)。

本子任务已执行源码核查与旧trace聚合；未单独启动lint、编译、RTL仿真、综合、布线或板测。统一新构建由runner_support/root负责，构建结果以其实际manifest为准。
