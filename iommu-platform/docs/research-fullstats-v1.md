# 全量搬运分组统计 v1（2026-10-02 实施契约）

状态：实现与主机验证完成，软件与FPGA完整构建均通过，未上板。Codex冻结接口和时序，DeepSeek起草有界实现/测试，Codex审查及实际验证。

## 目的与范围

补足每第16次固定抽样的总体覆盖。保持原串行IOMMU、DMA供给、轮询和gap机制不变，不实现缓解方法M。对共同硬件窗口内完整的A搬运全部聚合；旧每16次抽样、E2及COST旧寄存器保留。全量是指满足窗口完整性条件的搬运，不包括跨窗口在途搬运。

## 时序

聚合提交事件严格等于 `win_act && a_done_ok && lane_in_window_q[1]`，与原 `eligible_q` 增量一致，不带抽样相位条件。数据使用同一边沿的 `cur_a_main_n/comp_n/dur_n/req_n/miss_n`，包括完成边沿的计数；关闭边沿完成计入，关闭之后完成不计入，窗口外开始的搬运不计入。issue处于opening edge沿用原win_act_issue定义，统计周期仍为(open,close]。

硬复位、acc_clear以及合法p4m_arm清零聚合。合法arm精确定义为 `p4m_arm_i && mode_q==MODE_COST && quiet_i && !cost_active_q && e2_phase_q==E2_IDLE`。清零优先于提交。非法arm/clear不得擦除聚合证据；ABORT/close冻结已有结果，后续仅合法clear/arm清零。

聚合器不能驱动DMA、IOMMU、ready、仲裁、控制状态或中断，只接收观察信号和提供只读数据。所有新错误通过独立聚合错误字报告，不改变现有错误位定义。

## 分组与字段

六组：组0..4是requests=4且IOTLB misses分别为0..4；组5是其他所有组合（不得静默丢弃）。每组记录count、sum_main、sum_complete、sum_duration、sum_req、sum_miss、main/complete/duration的min/max。周期main和complete沿用现有互斥状态计数，duration为整个搬运区间，三者不得混称。请求数是select事件，miss是IOTLB lookup miss；没有DDT/PDT/PTW级别类型细分，不宣称完整类型加权基线已经被覆盖。

每组count和各sum为64位无符号；min/max为32位。空组所有字段为0。溢出可回卷但必须sticky报错；任何非零错误整次不可作有效统计。每组累计采用单次提交，无延迟遗漏，连续边沿事件亦需正确。sum_main+sum_complete<=sum_duration、sum_miss<=sum_req；逐copy也检查相同关系。均值=sum/count，不能从这些字段推导中位数或分位数。组5仍列出实际计数和范围，不给它虚构固定req/miss键。

## 本地raw索引（现有4096字中未用区域）

| 索引 | 含义 |
|---|---|
|0xD00|magic `0x49444d4141474731`（IDMAAGG1）|
|0xD01|版本1|
|0xD02|分组数6|
|0xD03|每组步长16字|
|0xD04|sticky错误：bit0计数溢出；bit1总和溢出；bit2单copy miss>req；bit3单copy main+complete>duration；其余0|
|0xD05|独立累计的提交总数，必须等于C14 eligible以及所有组count之和|
|0xD06..0xD0F|保留0|

组g基址0xD10+g*16：0=count；1=sum_main；2=sum_complete；3=sum_duration；4=sum_req；5=sum_miss；6={max_main[31:0],min_main[31:0]}；7={max_complete,min_complete}；8={max_duration,min_duration}；9..15保留0。未映射地址读0，配置写不改变聚合内容。

## 软件和兼容性

原UAPI结构、raw长度、JSON schema及旧模式保持。新增CLI `--require-fullstats`（仅--cost合法、重复选项拒绝）要求magic/版本/布局/错误/总数及组内守恒正确。新模式必须带此选项，旧BIT缺magic必须失败，不能假装全量观测存在。无此flag时旧BIT仍可用；如读到新magic仍验证新字段。

驱动capture在检测到新magic时至少检查版本/布局/错误和total==C14，错误返回EIO；无新magic按旧规则处理。CLI和离线分析须独立验证所有组、空组、min<=mean<=max、已知组req=4*count/miss=g*count、组和count==eligible、总和不超过共同窗口对应计数、Solo零A。所有整数运算检查溢出，不用double作64位守恒判据。非零未知magic不可静默接受。

## 执行计划

新增固定模式 `cost-fullstats`，从原不变84行TSV筛出全部21个cost用例：短Solo/A8g0/A16g0三个，六个主条件Solo/A1g0/A8g0/A16g0/A8g1000/A8g2000各3，保持原顺序/id/ordinal。short主参数不变，4096/256/4KiB，短64/8。新select_cases返回所选case的副本，给argv追加--require-fullstats并标记require_fullstats=True，不改变build_plan/旧模式输出或原TSV。runner完整复制验证原84行，仅本模式选择cost行并给每个CLI追加flag；argv记录与实际调用一致。

离线分析模式期望21项且强制全量字段；旧模式仍兼容历史JSON。报告全量均值/范围与旧抽样均值/范围、分组覆盖数，明确抽样中位与总体均值不是同一量。没有原始全量记录就不宣称总体中位。短与主、各次run/软件/BIT版本分开，不能把组内搬运当板级重复。

新BIT+软件同包交付Mac，Dell不接板。21项是独立采集段，旧E2/旧84不重跑，不拼接旧版本Solo。全错即停、不重跑/剔除；初次上板结果不是方法M收益。FPGA和软件必须完整构建；正常编译日志仅落盘，失败才检查。

## 必要验证

聚合器：六组/other、空组、连续事件、首样本min/max、各sum独立、reset/clear优先、计数/总和溢出和单copy非法关系。集成：carry-in排除、open/close同沿、close后冻结、ABORT/非法arm不擦除、C14与全量数/旧抽样数对应。旧observer、P4M和Solo驱动回归；CLI旧BIT缺magic/损坏字段/大整数/正常新数据；分析缺组/伪均值/边界及模式选择；新runner21项/首错停止；原84 TSV不变。测试和构建不等于板测。

## 构建与交付验收

2026-10-02：Linux `6.5.0-rc2-00092-g5853e425caf8`；CVA6 `0dc5e333`；软件实现平台 `3f060f3`。软件及FPGA构建退出0；首次受限FPGA构建因许可证连接失败，保留日志，获网络权限后完整构建通过。正常构建日志未巡查，本记录不新增时钟约束审查或板上时序结论。

主机验证：统计器189＋21项，观察器261＋2项，P4M双路293种配置、gap17种场景；CLI77项，计划/runner49项，分析器86项，Solo/capture真实驱动函数各1套。原84行TSV不变，rootfs内CLI/runner/计划与实际源码/编译产物匹配。FPGA暂存观察器源码与已提交源码逐字节一致；两次构建的组件锁一致。硬件构建从平台de34dbc启动，期间只提交了平台软件/分析器改动至3f060f3；锁定CVA6、Linux、配置未变。

[Mac步骤](research-fullstats-mac.md)；交付在 `out/research-fullstats-20261002/delivery/`。当前新板测 **0/21**，没有M收益或新增论文结论。完整校验值与验收记录见包内DELIVERY.json、SHA256SUMS。
