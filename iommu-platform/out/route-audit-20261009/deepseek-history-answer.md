# 历史证据整理（只做证据整理，不替代 Codex 研究路线与创新裁决）

> 说明：以下全部为对**已提供文件**的转述、交叉引用与边界标注；未执行任何命令、未运行实验、未新增 RTL、未改写文档。凡“通过项数”均已显式标为功能/执行计数，不当作性能或创新证明。

---

## 0. 精简索引

| 主题 | 关键落点 | 一句话状态 |
|---|---|---|
| 初始目标 | 研究总览_2026-09-08.md:9,11-14,18 | 多加速器/Chiplet 下 IOMMU 翻译代价 + 共享翻译资源跨设备干扰；承接旧学位论文 Remote PTW |
| 首篇收敛建议 | research-review-2026-09-09.md:7,15 | 收敛到“一个影响设计选择的主张”；中性问题=状态变化主导 vs 在途占用主导 |
| 平台/E1-E4 设计 | experiment-plan.md:15,27-31,39-45 | E0校准→E1性能→E2翻译状态→E3阻塞因果→E4路径/软件对照 |
| P4M | HANDOFF.md:1146-1154,1172,1651; p4m-pilot-results-review.md:20,29 | formal60 24在线+第25离线=25可用，35未运行；开窗A inflight ID 4/4/1448 |
| P/N/PN | composition-history-isolation-20261007.md:3,30,40,121; HANDOFF.md:2450-2455 | 隔离后额外miss差1465→0，PN仍慢12.67%–12.79%；172/172仅功能 |
| 10/7-10/9 机制探索 | composition-history-isolation-20261007.md:226-241,276-308,442-452,474-496; HANDOFF.md:3,178,282,810 | 多轮“普通对照复现→停止独立机制”；§92/round05 有界暂停 |
| 证据等级 | 见 §2 | 实板 / RTL仿真 / 抽象模型 / 理论 / 未验证提案严格分列 |
| 内部矛盾 | 见 §4 | 旧文档“最新”与后续状态冲突、84 vs 80+2+2、旧目录/旧ABI等 |
| 最少核读 | 见 §5 | 先 CURRENT_STATUS、HANDOFF 顶部 §92/§91、round05 root acceptance、各 out 原始目录 |

---

## 1. 初始研究目标与有日期阶段时间线

### 1.1 初始目标（原文）

- 大方向：`研究总览_2026-09-08.md:9` —“面向多加速器／Chiplet 系统，研究 IOMMU 地址翻译带来的性能代价，以及共享翻译资源下的设备间性能干扰。”
- 两阶段：`研究总览_2026-09-08.md:13-14` —（1）原学位论文：Accelerator-side IOMMU 部署动机 + Remote Page Walk 影响 DMA 性能；（2）当前延伸：多 iDMA 共享一个 RISC-V IOMMU，分析一设备翻译活动如何影响另一设备，并评估轻量缓解。
- 连接点：`研究总览_2026-09-08.md:16` —“IOTLB miss 与 PTW 延迟”。
- 个人目标：`研究总览_2026-09-08.md:18` —“刚研究生毕业、准备读博士……目标不必过高……形成‘现象—根因—缓解—评价’的完整证据链。”
- 当前中性题目：`研究总览_2026-09-08.md:68` —“多个 DMA 共享 RISC-V IOMMU 时的跨设备地址翻译干扰分析”；英文暂定 `:72`“Characterizing Cross-Device Translation Interference in a Multi-DMA RISC-V IOMMU”；角色 `:74` iDMA0=Victim，其他=Aggressor。
- 四个当前研究问题：`研究总览_2026-09-08.md:88-91` —（1）Aggressor 是否导致 Victim miss/延迟增加；（2）Victim 保持 hit 时是否仍被 foreign PTW 阻塞；（3）设备数/粒度/工作集/页大小/PTW 延迟如何影响；（4）Page Batching/大页是否降低翻译压力。
- 候选原因表：`研究总览_2026-09-08.md:76-82` — IOTLB 容量干扰 / Miss 引起 HOL / 前端仲裁竞争 / 内存互连竞争 / 测量方式影响；并注明“可能同时存在”（`:84`）。
- 首篇收敛：`research-review-2026-09-09.md:7` —“继续这个方向，但首篇应收敛到一个会影响设计选择的主张”；`:15` 中性问题“任务损失何时由翻译状态变化主导，何时由在途请求占用主导”。
- 方法论文目标修正：`research-review-2026-09-09.md:5` —“以方法论文为目标，机制表征用于寻找和验证方法的落点，不将首篇预先限定为表征论文。”
- 三段证据层次：`experiment-plan.md:29-31` — 状态证据 / 时序证据 / 性能证据，分别测量再建立联系。

### 1.2 分组时间线

#### A. 2026-09-08：总方向与平台/实验总设计

- `研究总览_2026-09-08.md:3` 整理日期 2026-09-08；`:5` 当天后续建立新平台 `/home/xvhaocheng/work/iommu-platform`，完成软件/FPGA 构建和 HPM 定向测试。
- `experiment-plan.md:15` 日期 2026-09-08：“E0/E1 的最小软件入口已实现并构建，板上实验尚未执行。”
- 平台配置（当时确认）：`研究总览_2026-09-08.md:116-128` — Genesys 2 / XC7K325T；CVA6 RV64IMAFDC, Sv39, 单核, 50 MHz（旧论文记录）；4 iDMA；DID 10–13、PID 20–23；IOTLB 16 entries；DDTC/PDTC 各 8；HPM 31 事件；`NumOutstandingTrans=8`；DS 路径 input 50 + output 50 cycles（`:124-125`，明确“不能解释为 8 次 PTW 并行”“不能简化为一次完整 PTW 恒等于 100 cycles”）。
- 原论文与当前工程不可混用：`研究总览_2026-09-08.md:139-145` — 原论文 working-set 用 8 entries IOTLB，当前 RTL 16；原论文 Linux 写 Kernel 6.15，当前源码标 6.5.0；原论文 SRAM 1.5 MiB / DDR3 1 GiB，本次未完整复核新工程。

#### B. 2026-09-08 ～ 09-18：9月平台、E1–E4、E3/P4M 前置

- E0/E1 实现与构建：`experiment-plan.md:15`；`HANDOFF.md:1483-1488` 记 2026-09-15 软件构建 exit 0 并更新 `out/experiment-v1/`。
- SC.W 修复：`HANDOFF.md:1471-1473`（2026-09-16，CVA6 提交 `7f086b73…`，旧生产模块回归旧版 54 场景/6 失败→修复版 54/0）；`HANDOFF.md:1617-1625`（新 BIT/MCS 构建通过，BIT SHA256 `df7c2400…`）。
- DMA cache sync 修正：`HANDOFF.md:1453-1459`（Linux `3b47ab16…`，内核 00080；19 调度/诊断/缓存场景、96 小 chunk、16 软件查询场景通过）。
- E1 实板 144/144：`HANDOFF.md:1442-1448` — 双 DID/单 DID 功能试次 + 18 探索性能样本 + 144/144（16 有限 HPM/冷热/回绕、48 预热/窗口敏感性、80 主样本 Solo/A1/A8/A32 各 10）。**关键边界**：`:1436` “主样本 V miss 全 0；A1 也全 0 但软件耗时已为 Solo 的 1.728 倍，A8/32 为 1.787～1.795 倍且 A miss 8190”，`:1436` “不能把约 79% 慢化归为纯翻译损失或 HOL”。
- E4 供给/计时核查：`HANDOFF.md:1428-1432`（方案）；`:1418-1426`（observer 构建 00081，BIT SHA256 `41fa23d0…`）；`:1410-1416`（实板 105/105：9 短 + 36 开关开销 + 60 主样本）；`:1404` — “60 主样本 H 约 56ms、全部 V miss 0；按同角色均值差，新增时间 99.08%～99.92% 在 P”，并明确“不把 P 全称 CPU，不宣称 HOL”。
- E4 硬件批量版：`HANDOFF.md:1394-1400`（构建交付，内核 00082）；`:1388-1392`（实板 14 短 + 60 正式 = 74/74；硬件中位约 283/149/103 MiB/s；双路在途重叠 99.69%～99.79%；第 42 项 V miss/PTW 2，其余 59 项 V 0）；`:1382-1386`（A16/A32/A64 各 10 次 = 30/30；硬件 MiB/s 中位 98.27/95.95/94.84）。
- DS 0/0 对照：`HANDOFF.md:1372-1374`（`baseline-4dma-batch` BIT SHA256 `c6555d14…`）；`:1364-1370`（实板 60/60；硬件中位 283.17/149.59/135.38/131.61/129.98/128.64；A8～A64 40 项 V miss 均 2）；`:1354-1356`（50/50 回切复测 30/30；硬件中位 283.33/149.51/103.31）；`:1360`（DS0 相对历史 50/50 A8～A64 +31.14%～+35.63%，但“未证明 HOL 或纯翻译代价”）。
- E3 因果实验线：`HANDOFF.md:1312-1318`（v4 实现，内核 00084，BIT SHA256 `a99ce956…`）；`:1304-1310`（2026-09-17 实板 3 短+12 正式=30 子轮，正式 10 对 A8 第一轮触发 0/10、第二轮 8/10）；`:1294-1302`（服务器复核；`:1300` 明确“静态代码确认触发只看 AR FIFO 头为 V，A 读在服务期间仍占头，漏掉 V 在其后的场景”）；`:1272-1278`（00085 D0/D0 诊断），`:1280-1286`（ioctl 0x0a 诊断软件构建）；`:1260-1268`（v5 实施，内核 00086）；`:1252-1258`（v5 实板 26 子轮/494 条去重观测；A8 目标 V 首次 miss 16/16，插入记录截断 9/16）；`:1240-1248`（v5 复核 9472 项检查 0 失败；但因果目标未达成；v6 受控实验方案）；`:1230-1238`（v6 实板 22 轮，418 条唯一观测）；`:1222-1228`（v6 服务器复核 11 对/22 轮、9 组检查）。
- E3 v6 定稿数字：`e3-controlled-results-review.md:63-68` — 4 对 cold 互补对 ΔA remaining / ΔV remaining = {127,129,129,130} 周期；V 自身翻译恒为 4 周期；完整排队增量为 {129,136,129,126}；`:70` D0/D0 基线 main case 1 Vremaining −2、总量 −8，main case 5 Vremaining 0、总量 +1；`:78` 限制“互补对 N=4，单次启动，64 B 有限诊断……不做显著性检验”。
- E3 范围声明：`e3-controlled-results-review.md:82` —不主张完整 E3 矩阵、不主张普适 HOL、不解释全部旧 A8 减速、无显著性/容差。

#### C. 2026-09-20：P4M 转向与 formal60

- P4M 前置：`HANDOFF.md:1212-1220`（P4M-1 主机验证 14 组，软件/FPGA 构建 exit 0，内核 00088）；`:1206-1210`（Linux00088 启动，实验 0）；`:1196-1204`（四组 short：Solo/A1gap0/A8gap0/A8gap1000，各 64 transfers / warmup8 / 4KiB；V 硬件窗口 44279/83938/120679/85276 周期；同窗口 A 完成 0/64/64/32）。
- P4M pilot 21/21：`HANDOFF.md:1176-1186` — 7 条件各 3 次；V 硬件窗口 MiB/s 中位：Solo 283.508、A1gap0 149.346、batchA8 103.284、P4MA8gap0 103.243、gap500 98.075、gap1000 145.379、gap2000 171.394；A 同窗口：gap500 98.051、gap1000 72.689、gap2000 57.101。
- pilot 复核与启动物理混淆：`p4m-pilot-results-review.md:20` —“opening A inflight ID 前两样本=4……第三样本=1448，相差 1444 个 A copy；但三者窗口 A done 都是 4096”；`:29` 第三对 A8g0/legacy B8 硬件 cycles 比 0.93578（约 −6.42227%），n=3 描述性；`:31` gap500 三 block 均比同 block gap0 更慢（非单调反例）。
- P4M start-control 12/12：`HANDOFF.md:1156-1166` — 8 长 B/P 的 V 硬件 MiB/s 中位 108.81305/108.85767；硬件 cycles 中位 7352059/7349046；4 对 P/B 比值 0.99941/0.99970/0.999996/0.99989。
- formal60：`HANDOFF.md:1196-1204`、`:1146-1154`（24 在线通过，第 25 次 `formal-b09-g2000` runner exit5，错误原文“startup_line_count before=36 after=36 want=37”，判定为脚本记录计数假设失效，非 DMA 失败），`:1134-1144`（恢复脚本 resume35 prepared_not_run；前 24 相对 gap0 配对 V 硬件周期比中位 0.72242199/0.63492757，反比吞吐提升 38.42%/57.50%；同窗口 A 速率配对比中位 0.69211626/0.52448158，下降 30.79%/47.55%）。
- 补缺 29/29：`HANDOFF.md:1128-1130` —“原 25＋6＋本次 29＝60 份，每 gap20，合计 53 在线＋7 离线追认”；“跨启动补缺，不能当作同次启动 60 次在线通过”。
- 60 份分析与并行实验计划：`HANDOFF.md:1114-1124` — 全 60 V 中位 MiB/s 108.8134916/150.6025798/171.3890800；A 同窗口 108.8134916/75.3012899/57.0878518；整轮 A miss/S1 中位 5140/5140、3086/3086、2401/2401；gap1000 n20 V+38.4233%/耗时−27.7578%/A速率−30.7884%；gap2000 n18 +57.4856%/−36.5021%/−47.5539%；`:1120` 新样本 `formal-b15-g0.json`：V miss/S1=2/2，其余 59 份 0/0，A miss5141，V 吞吐 110.156634（gap0 最快），开窗 A 在途 ID 16。
- 2026-09-30 组会与后续讨论：`HANDOFF.md:1637-1643`（组会材料 8 页/540 秒；复算 ΔA/ΔV 127/129/129/130；P4M 前 24 三组同窗口速率与既有统计一致；`:1642` 明确“未新增实验……不把材料准备记为剩余 35 次已完成”）；`:1645-1657`（两次组会边界；`:1651` “formal60 状态：目前 24 在线通过 + 第 25 离线认可 = 25 可用；35 未运行”；`:1649` E3 精确措辞，`:1650` “E2 缓存替换：未证实，也未排除”）；`:1659-1667`（改善措施与创新点两轮讨论：尚无已确认创新；固定 gap 是基线，首选评估有限资源下 hit 与 pending miss 服务解耦）；`:1669-1674`（普通非阻塞/必要容量保护；静态离线设计不要求在线识别；补充 Valsan/Yun/Farshchi 非阻塞缓存隔离已有 MSHR 耗尽挡 hit 与并行度控制）；`:1676-1682`（N/P 可否决设计卡：N=合理有限非阻塞，P=同底座同各类容量的朴素静态预留，R=未形成）。

#### D. 2026-10-02 ～ 10-06：P-N-PN、E2 64、method88、composition

- 2026-10-02 正式五轮：`HANDOFF.md:1089-1100` —“唯一M仍是有条件候选”；R1 候选 C1（实占反馈）/C2（合法描述符重排）/C3（服务时间通道），DeepSeek D1–D4；“C1+D4 合 M，D1 留诊断；C2 缺真实语义 trace、D2 前提与接口成本不成立、C3/D3 先行技术/真实可见性/防护不足，均不做主线”（`:1094`）。
- 两轮交叉审查：`HANDOFF.md:1102-1112` — 保留 P4M60、E3 有限服务阻塞证据、E2 待证和方法论文目标；`：1107` 新增先行工作（WO2010117360A1、HPCA2017 hit-under-miss、Valsan 非阻塞资源干扰、MASK fill 控制、DWS walker 分区、RTCSA2025 表征）。
- E2 84 次实现：`HANDOFF.md:1684-1690`（E2 并行建议）；`:1692-1698`（单次部署方案）；`:1701-1709`（同包诊断实现进展：Linux `a8d28cd44f35`、IOMMU `238a39e`、CVA6 `2ccb6401`、顶层 `60b2d44`，内核 00090）；`:1712-1723`（最终交付：短 6 + E2 60 + 自然占用 18 = 84；`：1714` “本包实际板测 0/84”）。
- 00091 cost-followup 11/11：`HANDOFF.md:1760-1772`（交付包 SHA256 `2ec2c0b6…`；Mac 尚未收到/新版板测 0/11）；`:1774-1788`（正式回传：Solo 耗时 56.44032 ms、V 283.485281 MiB/s；g0 146.982 ms、V/A 108.856867；g2000 93.30072 ms、V 171.488494/A 57.114570；g2000 对 g0 V 速率 +57.5358%、耗时 −36.5223%、A 同窗速率 −47.5324%）。
- 全量分组统计 v1：`HANDOFF.md:1791-1807` — 84 行冻结计划选 21 cost：3 短 64/8 + 6 条件各 3 主 4096/256；内核 00092。
- fullstats21 回传：`HANDOFF.md:1809-1819` — 主条件各 3 次中位 V/A MiB/s：Solo 283.050/无 A，A1g0 149.548/149.548，A8g0 108.907/108.907，A16g0 100.994/100.994，A8g1000 150.764/75.382，A8g2000 171.375/57.079；`：1813` 全量 req4/miss2 每 run 均值约 g0 1406–1410、g1000 1100–1103、g2000 840–842 周期；明确“旧固定 16 抽样在 g1000 只取 g0，漏掉 1536/2047 份 g2”“不能因抽中组均值接近而忽略采样总体偏差”。
- 两 BIT v2：`HANDOFF.md:1833-1835`（用户接受两 BIT 共用 Linux，Mac 端 AI 轮流执行）；`:1838-1844`（UAPI 0x0f/0x10、frontend ADMIT/反向、D80 计时及 F 能力区、E 策略区；主机通过）；`:1847-1853`（共用软件 00093 完整构建通过；E2 64 + 方法 462 清单冻结）；`:1856-1862`（方法初版 WNS −0.322 ns/TNS −59.515 ns/598 建立失败端点/3 组合回路，拒绝交付；修 registered-abort）；`:1865-1869`（直接解码研究 ARM；registered-abort 版 WNS −3.979 ns/TNS −17075.918 ns/9454 setup 失败，拒绝）；`:1872-1876`（直接 ARM 版 WNS −0.150 ns/TNS −0.179 ns/2 setup 失败，DDR200MHz 写缓冲）；`:1879-1891`（最终 E2 WNS +0.177/WHS +0.028/WPWS +0.062 ns；方法经独立物理优化 +0.048/+0.028/+0.062 ns；role1 BIT SHA256 `319b415e…`，role2 `31e18ee9…`，payload `af9f703b…`）。
- E2 64 结果：`HANDOFF.md:1899-1911` — W7 正反各 5 次保持 V 两目标；W8 正反各 5 次均 A 阶段 DID11 更新覆盖 V 条目、探测前消失、A 停止排空后 V 首 miss；128 target 中 62 替换后首 miss、66 保持命中；W7 4KiB 单次探测中位 696 周期，W8 正/反 1483/1486；D89 翻译服务占用中位 20 与 822/821；50 MHz 下 13.92 与 29.66/29.72 µs；`：1909` “E2 现象不单独构成创新”。
- method462/method88：`HANDOFF.md:1956-1990` — 462/462 summary ok、10041 回传文件 unchanged true；`：1972` 正式 p5 vs fixedgap t150/t170 公平匹配 A 增益 +15.289%/+24.211%；正式 t170 vs p1/p2 −6.464%/−6.110%；`：1973` 反向 p5 相对 p6 +40.672% 但正式 −3.791%；`：1974` 反向 p5 相对 p4 +12.878%、t150 +5.247%（CI 跨 0）；`：1976` A1/A16 p5 V 中位 149.581/147.257，未达 150；`：1977` 参考 solo 283.099、A1 149.507、A8 108.904、A16 100.930 MiB/s。
- method88 结论：`HANDOFF.md:1038-1040` — 88/88 host_exit0；p5 对最强匹配简单对照四格全负（−5.68952% / −8.23689% / −5.15142% / −4.43680%），跨方向 False；`：128` E2 已完成 64；`:1044` 明确不要写成“窄幅反转、actual cost 独有收益、每 copy miss 固定……E2 尚无条目替换直接证据、SMMUv3 已实测显著吸收干扰”。
- N 原型：`HANDOFF.md:1993-2008`（跨 DID 两银行请求生命周期标记、实际 DDTC/IOTLB 命中路径、单响应口仲裁和失效回退）；`:2014-2016`（原版对照 15/15，boot `161dcc93…`）；`:2030-2032`（serial-03 短 A8 未返回，计数全 0，根因=信号作用域 implicit wire，未重编；`:2032` 明确“此观测接线错误不自动解释 A8 停滞”）；`:2059-2061`（v1 轮询交错重叠功能拒绝：banked 8/12 停滞，Solo 4 通过，legacy 12/12 通过）；`:2069-2071`（v2 两版布局失败，combined LUT S207069/N207226 > 203800）；`:2079-2081`（v3 FIFO RAM 验证 9/9、454278 检查/13150 周期）；`:2094-2096`（v3 N 版 15/15 但 fastV/A=0）；`:2099-2105`（v4 静态 MSI 兼容性修正，DID10/11 msiptp.mode=1）；`:2128-2132`（v4 S/N 30/30，N candidate bit2 从未出现，probe/DC/LEAF/fast 全 0）；`:2135-2137`（v5 资格模块；banked_ctl 旧首项 2032 checks 失败）；`:2145-2147`（v5 首次编译作用域错误）；`:2165-2167`（用户收窄停止条件）；`:2184-2190`（v5 S/N 30/30：N 快速处理计数 A8 每轮 6144/6146/6146、A16 每轮 8193；N/S 速度中位变化 A8 +4.15%、A16 −2.69%；N 相对原版 A8 −14.52%、A16 −22.09%；A8 三轮 104.263/92.987/93.048 MiB/s）。
- composition-v1：`HANDOFF.md:2199-2212`（五轮复审启动；P 主基线 2/14、后备 B 不实施）；`paper-route-five-round-review-20261004.md:9,14-22`（同 BIT 四模式 0=S/1=N/2=P/3=PN；RQ：持续热 V 的容量保护/命中先服务结论能否外推到间歇 V；Phase1 16/Phase2 12/Phase3 144 = 172）。
- composition 修复与 172：`HANDOFF.md:2328-2334`（原 172 仅 52 通过/1 失败/119 未运行；N 单项也失败；N64 通过、N1024/4096 卡死）；`:2337-2343`（用户提示旧 IOMMU 修复 7fbb2b9 AW 先于 AR、6cc8ed3 双 DMA MMU 侧带；`:2341` “症状相似不等于同根因”）；`:2356-2364`（诊断 BIT 上板，N4096 卡死快照：COMP WVALID1/WREADY0，DDR 入口 AWVALID1/AWREADY0，PTWactive0）；`:2398-2404`（定位到 DS 写归属：DDR xbar Wowner FIFO head3=DS，内部 Wvalidmask5=CPU0+COMP2，readymask8=仅 DS3；修正 `rv_iommu_ds_if.sv` AWID 映射 CQ0/FQ1/MRIF2/MSI3/PQ4 错位）；`:2434-2440`（DS 修复后 172 第 53 项 PN 报 F15 写页故障，52/1/119；`rv_iommu_ptw.sv` STAGE_1 LVL2 后独立 if(LVL1)/else 误置 ERROR）；`:2443-2447`（PTW 旧 4K 读写错误 cause 13/15 → 新 7 场景 pass）；`:2450-2455`（PTW 修复 BIT 新 boot `2695d848…`，完整 172 全部通过，short16/noise12/explore144；“功能验收不等于性能收益或完整 IO/CDC 签核”）。

#### E. 2026-10-07 ～ 10-09：机制探索与暂停

- composition-isolation 板测：`composition-history-isolation-20261007.md:21` — 22/22 通过（4 短 + 18 性能例），failed=0；原 IOTLB 回归 25834 次求值、TL 回归、新增隔离 9232 次求值；“这些数字是检查次数，不是板测用例数”。
- 核心结果：`composition-history-isolation-20261007.md:29-30` — P/PN 旧 A miss 5117/6582 → 新 2048/2048；旧 W 8960484–8994909 / 10001202–10006893；新 W 7032355–7039655 / 7931144–7931945；PN 相对 P 放慢 旧 +11.2200%–+11.6355%、新 +12.6744%–+12.7922%；`:32` 新 P V 普通 16384/快速 0/miss 0；新 PN V 普通 12288/快速 4096/miss 0，总 V 请求均 16384；`:34` n=3 描述性。
- gap 板测：`composition-history-isolation-20261007.md:119-123` — V gap0 P/PN 109839.3/123956.7 周期、+12.85%、A 速度 29133.55/25815.51、−11.39%；gap1000 171739.7/178143.7、+3.73%、36974.57/35551.88、−3.85%；gap4000 327404.7/327658.3、+0.08%、48207.52/48220.97、+0.03%；`:127` gap0 双方 A 完成 64、A miss/fill 均 32、V miss 均 0。
- 模型 DDR 因果链：`composition-history-isolation-20261007.md:90-99` — 原 FIFO P/PN 91878/93480（+1.744%）；窗口内 DS 读优先 79847/79484（−0.455%）；P→PN 服务延迟总和 10206→15711（+5505），PTW active 也 +5505；DS 优先时两种模式 93 次 DS 读延迟总和均 2976、PTW active 均 13489，PN 退化消失；`:99` 直接事件：PN V 快响应 2758/2764；V 数据读 2763；A 叶子 PTE 读 2766、PTE 到 3100 才返回（334 周期）、3156 填充；P 中该 PTE 2763 进入、2808 返回（45 周期）、2864 填充，V 数据读 2876 才进入。
- §13 撤回创新路线优先推荐：`composition-history-isolation-20261007.md:226-241` —“用户的判断成立：§12 给出的是工程优化入口及待核问题，没有交付可立项的具体创新点。先前称 B 为‘首选’、A 为‘有探索价值’，容易让人理解为已经通过创新预审，推荐强度过高。现在撤回该优先推荐”；`:237` 引 DWS §VII-D/Fig.11“MASK+DWS 可能不比单用显著增益”。
- §14 两条规则草案：`composition-history-isolation-20261007.md:243-274` — DeepSeek 规则：主 PTW 处于 S1 最后一级 MEM_ACCESS/PROC_PTE 时，另一 bank 的 AxLEN!=0 读不接受 FAST，走 REJECT；Codex 规则：准备译址和物理 offer 拆开，当前 PTE 未越过共同共享 AR 入口前不产生新旁路物理读 offer；`:256` 纠正“AR 被接收不等于整条数据读不可抢占”；`:258` 撤回“无条件全时在途量<=Q”。
- §15 两条规则创新查验：`composition-history-isolation-20261007.md:278-308` — “当前两条都不能作为已确立的论文新方法：DeepSeek 的最后一级多拍读门控仅保留工程对照；Codex 的入口确认＋逐 PTE 额度保留一个待验证差异，不直接进入生产 RTL/BIT”；`:286-287` 逐项对照 MASK §5.3–5.4、AXI-REALM §III-A-1/2/3、ABU ECRTS2019 §3；`:295` 旧 p4 按 PTW 返回事件收费、固定窗口末补额，不等于 B 的 PTE 进展补额。
- §16 B 最小差异：`composition-history-isolation-20261007.md:310-339` — 固定 V 64、A 8 页、V gap0、warmup256、模型内存延迟 31；P 91874、N 151947、PN 93487、PN+入口确认 92917、PN+B 92362、PN+固定周期补额（1024）92227、PN+整次遍历额度 91091、PN+普通在途额度 92362；`:327` “B 相对 PN 的 V 耗时降低 1.203%、A 吞吐增加 1.218%，但 V 仍比 P 慢 0.531%”；`:329` 默认不加缓冲 S/N/P/PN = 105588/152042/91878/93480。
- §19 普通准入 8/8：`composition-history-isolation-20261007.md:387-409` — 原热页 buffer P/PN 91874/93487；owner 保守准入 92898/92857；新冷目的 buffer 92334/94427；owner 保守准入 93778/93797；整 walk 额度 256 92334/91727；`:401` “owner 方法使 PN 比原 PN 快约 0.67%，但 FAST 全部消失，且仍比原 P 慢。已有整 walk 方法则保留 64 个 FAST，冷 PN 比原 PN 快 2.859%、比同条件 P 快 0.657%”；`:403` 新机制：owner cold PN 先读目的叶 0x50010 于 992/fill1096，随后源叶 0x50000 于 1473/fill1576，再发源 AR1585/1592；P 同样两次 V walk。
- §21–§24 共享返回缓冲/请求头暂存：`composition-history-isolation-20261007.md:425-496` — §21 13 场景：原 PN 热页 93487/FAST62；两槽共享 4144B 热 P97774/PN103348、FAST0/31；单槽 2096B 热 P112451/PN112448、FAST0/0；冷 P112931/PN112948；`:433` 首段 V 事件：原版第二 DDR AR+26 先于首 Rlast+334；选择性版第二 AR+1234 晚于首 Rlast+330。§22 最终 21 配置：Mode7 普通无池热 P92109/PN92374、冷 P92549/PN92814；Mode4 单槽 123804；Mode6 99432；Mode5 102746；`:444` “共享池在当前 fixture 无独立价值，不再推荐为主贡献”。§23 同 bank 停滞：两独立 bank+普通完成前缀门 48B PASS 129548/V64-A63；同 bank 相同门 48B 预热停滞；同 bank 普通完整私有缓冲 8192B PASS 127659/64-63；同 bank 普通源端准确写准备门 48B PASS 131464/64-64。§24 共享 2 槽+直通 PASS 127092、普通每 DMA2 槽共 4 槽 PASS 127147、普通共享 3 槽 PASS 127147；`:494` “该实现可由普通共享选择队列＋同样的旁路表达，停止把它作为独立新规则推荐”。
- §25–§26 写路径：`composition-history-isolation-20261007.md:504-541` — 原严格门 P/PN 101731/102686；完整物理前缀检查 95692/96267；普通虚拟写队头+目的 AW 准备 95692/96267；`:512` 普通 PN 仍慢 P575 cycles/0.6009%；`:532` 原队头串行 P/PN 92109/92374（+265/0.288%）；overlap 85339/85984（+645/0.756%）。
- §27–§48 论文筛选：`composition-history-isolation-20261007.md:549-812` — §27 A/B/C 不升级独立贡献，保留 D 窄候选（源 R2048、目的 W1024+1024）；§28 D/E/F 均未建立候选优于强简单基线；§29 G/H 未实现，专用翻译通道/VC、页表优先、朴素动态 burst 不是新贡献；§31 自然同 bank：C0 计数回绕为工程缺陷（配置 4 项、外部写计数 3bit，第 8 项使 q=0/overflow1），修正后 32KiB/CONF1 仍停滞；§33 固定与动态后译址分片：固定 64 物理源 AR、动态 55（9 父合并）；31 参数全体末 W 20213→20766 晚 553；`:641` “固定也保留原父逻辑请求并解除本例停滞，因此当前不把动态最长前缀作为独立主贡献”；§34 父槽 1/2/4/8 同效，六新 CSV 逐字节等同旧 8 槽；§35 前置 128 拍分片 V 先真实 issue→done 前 24162/27436 后 17471/20149，均值 25799→18810 低 27.0902%；§36 普通父翻译复用：关闭 24162/27436、复用 24084/27340、提前 24086/27342，均值 25799/25712/25714，改善 0.337/0.329%，旧 post 17471/20149 均 18810，剩 6902/6904；§37 同原窗口 1 后置均值 23989.5，普通提前 25714，残差低 6.706%；window8/单父槽 18810；window2/depth2 恢复 20151，window4/depth4 恢复 18824；§38 多父准备：最强普通缓存命中直通 AW 优先 V 先 23380/26122 均 24751，旧 post window4 V 先 17485/20163 均 18824，降低 23.946507%；§39 普通物理源读旁路：V 先 off 均 24751，cache 17704/20246 均 18975，wait 18146/20984 均 19565，旧 post 均 18824，原 gap 5927→151，缩小 97.4523%；§40 翻译寿命：源码可达性门槛否决当前 LTI-style 寿命反馈；§41 一次数学运行 11390 有限分类、greedy/brute mismatch0；§42 已承诺写有限排空：普通强对照 V 先/A 先 4115/5436 request→quiet，pause-end V W 32768/19456；严格停 V 先/A 先窗口内无 quiet，pause-end V W 11264/0；§43 双 bank 写序倒置不可达，停止机制分支；§44 三席核查后四规则停止作为独立机制，普通 retained frame 最大 82bit、partial G restart 最大 36bit；§45 数学 6450 配置/mismatch0/exit0/0.7781777381896973s，普通 q3/B77 按 3/1/3/1/3/1/3 零重读、q4/B36 按 4/4/4/3 同效、q3/B36 无解；§46 合法共享命中下两历史合同区别，NB-Walker 作者固定源码已有 shared fetch/private Info/有限 MSHR/PTW 预约；§47 剩余用途反馈，先过强普通与场景门；§48 模型 9trace×E4/8/16=9 组，每组四普通策略共 36 records，gap8 E4 32/30/32/29，E8 32/26/32/25，E16 全 24；gap4 E4 32/30/32/26，E8 30/24/26/24，E16 全 24；gap1 三 E 全 24；`:322` “停止纯 remaining-use 算法命名”。
- §49 共享长读：`HANDOFF.md:806-812` — run03 310 场景/7540 checks 与 run04 4/104 均 exit0，合计 314/7644 受限事件模型；run01 598 failed、run02 143 failed；N16/B3/q2/P3/resume200 候选/强 tile 同 R28/AW6/214 steps、AR2/4；no-pause 同 48 steps、AR1/8；payload 48B+skid8B=56B。
- §50 真实 iDMA/IOMMU：`HANDOFF.md:279-290` — 27 新 run 中最终 build06 接受 12（14–21、24–27），先前版本 7 PASS（03–07、10–11），6 失败；lat31/no-pause V/A 5657/5650 vs q2 读 8579/8572，mean 低 34.1%，AR1 vs 128，R 同 256、两个 W 各 256、各 128 child；`:286` “34% 显著依赖读请求模型成本，独立 pause 下 V 实际只低 1～4%，不能直接当独立创新/分区承诺”。
- §70–§74：`HANDOFF.md:219-228`、`research-morning-20261009.md:13-19` — §70 22/22 完整 copy PASS，带宽受限时共享整体耗时低约 19%–39% 但部分条件 V 变慢；§71 50 次执行 40 PASS + 10 严格拒绝，两在线规则首批 22 项选择相同；§72 48 次 40 PASS + 8 真实 watchdog，修后 24/24 PASS；§73 30/30 PASS，长预算 V 慢约 53.4%；§74 20/20 PASS，3 条件早 AR 只省 12/14/12 cycles（约 0.13%）；最后五阶段合计 170 DUT 执行：152 完整 copy PASS、8 旧 watchdog、10 严格拒绝。
- §76–§78：`HANDOFF.md:196-206` — §76 一 SV/7 PASS、§77 一 SV+一 C++ 增量/8 PASS、§78 三 SV/14 PASS，合计 6 成功 build 步骤、29 真实完整 copy PASS；主结果 join V19462/A19465、R257；弱普通 V23492/A23499、R320（平均低 17.1607%）；强普通许可已齐起点共享 V19402/A19411、R256，反而更快。
- §92/round05 暂停：`HANDOFF.md:3-5` —“本轮完成有界创新判断：得到具体复现边界，但没有新的已支持独立贡献……按本轮开始时说明的理解，因没有新的独立贡献证据，保存记录后暂停探索；这不是整个领域无法推进”；`CURRENT_STATUS.md:3` 同；`mechanism-design-handoff-20261009-round05.md:5`、`:17`、`:21` 给出 L-M/L-W 复现与两条可证伪重启条件。

---

## 2. 已有可复用成果与证据等级（严格区分）

> 说明：以下“通过项数”仅表示执行/功能/检查计数；“实板”=FPGA 板卡原始记录；“RTL 仿真”=真实 RTL 复制仿真（多为 out 副本）；“抽象模型”=数学/Python 有限状态或缓存模型；“理论”=手推、文献对照、设计论证；“未验证提案”=尚未实现或未运行。

### 2.1 实板（FPGA 板卡）

| 成果 | 原文与数字（含分母/基线/条件） | 等级与边界 |
|---|---|---|
| E1 功能/性能 | `HANDOFF.md:1444-1448`：144/144（16+48+80）；主样本 Solo/A1/A8/A32 各 10；A1 软件耗时 Solo 的 1.728 倍，A8/32 1.787–1.795 倍；V miss 全 0 | 实板；`:1436` 明确“不能把约 79% 慢化归为纯翻译损失或 HOL”；下载哈希/SD 回读未做 |
| E4 observer | `HANDOFF.md:1412-1414`：105/105（9 短+36 开关+60 主）；H 约 56 ms 基本不变；P 从 Solo 约 346–349 ms 增至 A1 约 767–769 ms、A8 约 804–805 ms；R 约 185–191 ms | 实板；`:1414` “不把 P 全部当 CPU、不把 H 全部当 IOMMU，不宣称 HOL” |
| E4 batch | `HANDOFF.md:1390`：74/74；硬件中位约 283/149/103 MiB/s；双路在途重叠 99.69%–99.79% | 实板；`:1390` “不能归为纯翻译或 HOL” |
| DS0/0 | `HANDOFF.md:1366-1368`：60/60；硬件中位 283.17/149.59/135.38/131.61/129.98/128.64 MiB/s；A8～A64 40 项 V miss 均 2 | 实板；`:1368` “不能直接把提升折算单次 PTW 代价或证明 HOL” |
| DS50 回切 | `HANDOFF.md:1356`：30/30；硬件中位 283.33/149.51/103.31 MiB/s | 实板 |
| E3 v6 | `HANDOFF.md:1232`：22 轮、418 条唯一观测；`e3-controlled-results-review.md:63-68`：4 对 cold ΔA/ΔV={127,129,129,130} 周期，V 自身 4 周期 | 实板；`:78` N=4、单次启动、64 B；`:82` 不主张普适 HOL |
| P4M pilot | `HANDOFF.md:1178-1184`：21/21；7 条件各 3；V 中位 283.508/149.346/103.284/103.243/98.075/145.379/171.394 MiB/s | 实板；`:1184` n=3 仅描述性 |
| P4M start-control | `HANDOFF.md:1158-1160`：12/12；8 长 B/P V 硬件 MiB/s 中位 108.81305/108.85767；4 对 P/B 比 0.99941/0.99970/0.999996/0.99989 | 实板；`:1160` n=4 仅描述性 |
| P4M formal60 | `HANDOFF.md:1150`：24 在线通过 + 第 25 离线认可，35 未运行；`HANDOFF.md:1144`：前 24 相对 gap0 配对 V 硬件周期比中位 0.7224/0.6349，反比吞吐 +38.42%/+57.50%；A 同窗速率下降 30.79%/47.55% | 实板；`:1130` 跨启动补缺，不当作同次启动 60 次在线 |
| E2 64 | `HANDOFF.md:1903-1907`：W7 正反各 5 保持 V 两目标；W8 正反各 5 均替换；128 target 中 62 首 miss、66 保持 hit；W7 单次 4KiB 探测中位 696 周期，W8 1483/1486；D89 占用 20 与 822/821 | 实板；`:1909` E2 现象不单独构成创新 |
| method88 | `HANDOFF.md:1038-1040`：88/88 host_exit0；p5 四格 −5.68952%/−8.23689%/−5.15142%/−4.43680%；`:1977` 参考 solo 283.099、A1 149.507、A8 108.904、A16 100.930 MiB/s | 实板；`:1972-1976` 公平匹配条件；`:1979` E25 许可关闭不等于实际 stall |
| fullstats21 | `HANDOFF.md:1811-1813`：21/21 ok；主条件各 3 次中位 V/A：Solo 283.050、A1g0 149.548、A8g0 108.907、A16g0 100.994、A8g1000 150.764/75.382、A8g2000 171.375/57.079 | 实板；`:1813` 全量 req4/miss2 均值 g0 1406–1410、g1000 1100–1103、g2000 840–842 周期 |
| composition 172 | `HANDOFF.md:2450-2455`：short16+noise12+explore144 全部通过；新 boot `2695d848…` | 实板；`:2455` “功能验收不等于性能收益或完整 IO/CDC 签核” |
| composition-isolated | `composition-history-isolation-20261007.md:21`：22/22（4 短+18 性能），failed=0；`:29-30` 旧 P/PN A miss 5117/6582、新 2048/2048；新 PN 相对 P 慢 12.6744%–12.7922% | 实板；`:34` n=3 描述性、跨 BIT/跨启动混杂 |
| gap64 板测 | `composition-history-isolation-20261007.md:119-123`：gap0 P/PN 109839.3/123956.7、+12.85%、A 速度 −11.39%；gap1000 +3.73%/−3.85%；gap4000 +0.08%/+0.03% | 实板；`:125` n=3；`:131` 无 PTW 占有或页表 AR/R 独立时序计数 |
| N v3/v4/v5 | `HANDOFF.md:2091`（S 15/15）、`:2096`（N 15/15 但 fast=0）、`:2125`/`:2130`（v4 S/N 30/30，candidate 从未出现）、`:2184-2186`（v5 S/N 30/30；N 快速处理 A8 6144/6146/6146、A16 8193；N/S A8 +4.15%、A16 −2.69%；N 相对原版 A8 −14.52%、A16 −22.09%） | 实板；`:2186` “机制已实际触发，但尚不能宣布性能改善成功”；不同 BIT/新启动 |
| §50 共享长读 | `HANDOFF.md:282-286`：27 run 中 build06 接受 12；lat31/no-pause V/A 5657/5650 vs q2 8579/8572，mean 低 34.1%，AR1 vs 128，R 同 256、W 各 256；`:286` 独立 pause 下 V 实际只低 1–4% | 实板/RTL；`:286` 34% 依赖读请求模型成本 |
| §70–§74 | `research-morning-20261009.md:11`：170 DUT 执行=152 完整 copy PASS + 8 旧 watchdog + 10 严格拒绝；`:19` §74 早 AR 省 12/14/12 cycles（约 0.13%） | 实板/RTL；`:11` “不同配置各运行一次，不是统计重复或板卡结果” |
| §76–§78 | `HANDOFF.md:198-201`：6 成功 build 步骤、29 真实完整 copy PASS；join V19462/A19465、R257；强普通 V19402/A19411、R256 反而更快 | RTL/DUT；`:201` “这组 17% 不能称独立性能算法” |

### 2.2 RTL 仿真（真实 RTL 复制，非上板）

| 成果 | 原文与数字 | 边界 |
|---|---|---|
| E3 controlled TL 局部 | `e3-controlled-results-review.md:41-44`；`composition-history-isolation-20261007.md:66` 52 场景 | 仅单对 TL，不证明完整 DMA/DDR 上界 |
| §16 B 最小差异 | `composition-history-isolation-20261007.md:314-325`：P 91874、N 151947、PN 93487、入口确认 92917、B 92362、固定周期 92227、整次遍历 91091、普通在途 92362 | 测试代理，不是生产 TC 完整协议 |
| §18 C 可达性 | `composition-history-isolation-20261007.md:366-372`：FIFO DDR P/PN 92334/94427（+2.267%）、A 吞吐 2.8391/2.7762；DS 优先 80252/79911（−0.425%）、3.2665/3.2804 | 单次路径测试；首 V capture FAST 全 0 |
| §19 owner baseline | `composition-history-isolation-20261007.md:393-401`：8/8 | 普通方法，非 C 实现 |
| §21–§22 共享返回池 | `composition-history-isolation-20261007.md:429-444`：13 场景；§22 最终 21 配置 | 测试专用模块；`:437` 限 2DMA、正常对齐 4KiB、单执行 job |
| §23–§24 同 bank | `composition-history-isolation-20261007.md:459-484` | 测试别名到同 bank，非生产 BIT |
| §25 写路径 | `composition-history-isolation-20261007.md:508-510`：严格 101731/102686、完整前缀 95692/96267、普通 95692/96267 | 仅此矩阵等价 |
| §26 读服务 | `composition-history-isolation-20261007.md:532-533`：串行 92109/92374、overlap 85339/85984 | `:539` 未量化该写条件对 265 周期的独立因果贡献 |
| §31 自然同 bank | `HANDOFF.md:524-527`：13 运行 6 PASS/7 watchdog；`HANDOFF.md:527` 普通 CONF0 共同分段短读停滞、普通 Buffer512 两顺序 PASS | 两真实 iDMA；长错位 CONF1 不解释旧 PN |
| §32 普通源读门 | `HANDOFF.md:623`：4 运行 4 PASS；各流 32 R/32 AW/4096 拍；总 64 AR/64 AW | 普通门，不称新机制 |
| §33 固定/动态分片 | `HANDOFF.md:637`：6 运行 6 PASS；固定 64 物理源 AR、动态 55（9 父合并）；模块声明 5962bits+观测 160 | `:643` 固定保留原父逻辑请求并解除停滞，动态不立独立主贡献 |
| §34–§37 父槽/分片位置 | `HANDOFF.md:647`（六新 CSV 等同旧 8 槽）、`:663`（V 先 24162/27436→17471/20149，均值 25799→18810 低 27.0902%）、`:677`（复用/提前 24084/27340、24086/27342，均值 25712/25714，改善 0.337/0.329%）、`:692`（同原窗口 1 后置均 23989.5，普通提前 25714，残差低 6.706%） | 均为 out 原型；`:665` N 同 bank FAST 0 |
| §38–§39 多父准备/物理源读旁路 | `HANDOFF.md:703`（最强普通 24751 vs 旧 post 18824，降低 23.946507%）、`:712`（cache 18975、wait 19565、旧 post 18824，原 gap 5927→151，缩小 97.4523%） | `:715` Arm LTI 前史，不立独立机制 |
| §42–§43 已承诺写/双写序 | `HANDOFF.md:749-751`（普通强对照 V 先/A 先 4115/5436、pause-end V W 32768/19456；严格停 pause-end V W 11264/0；字节下界 V 先 ≥15360 R/≥15368 W，A 先 7168/7168） | `:752` 功能 ≥65 bit，未综合 |
| §45 数学 | `HANDOFF.md:776`：6450 配置、mismatch0、exit0、0.7781777381896973s | 列明 codec 下存在完成策略，非任意公平策略或全资源最优 |
| §48 缓存模型 | `HANDOFF.md:312-316`：9 组、36 records；gap8 E4 32/30/32/29、E8 32/26/32/25、E16 全 24；gap4 E4 32/30/32/26、E8 30/24/26/24、E16 全 24；gap1 三 E 全 24 | `:316` 每 miss×3PTE、仅无 PWC 三层 Sv39 模型，非 DUT 实测 |
| §49 事件模型 | `HANDOFF.md:810`：run03 310/7540 + run04 4/104 = 314/7644；run01 598 failed、run02 143 failed | 受限事件模型，非平台速度 |
| 四 DMA 抽象 | `composition-history-isolation-20261007.md:541`：324 抽象分析/34680 累计状态；盲目共享 H2..4 均 30/36 组合可达停滞、H5 18/36、H6 0；普通每 bank2 槽共 4 与当前 W-bank 预留 2 槽均 0/36 | 合同把 R→W 派发原子化/假定 AW 可备；非 RTL 性能实验 |
| PN 数据路径 | `composition-history-isolation-20261007.md:90-99` | 仅此模型与工作负载可达；板上 12.74% 未归因 |

### 2.3 抽象模型/数学

- §41：`HANDOFF.md:732` — 7585+3805=11390 有限分类、greedy/brute mismatch0、非 DUT 试验。
- §45：`HANDOFF.md:776` — 见上。
- §48：`HANDOFF.md:312-316` — 见上。
- §49：`HANDOFF.md:810` — 见上。
- 四 DMA：`composition-history-isolation-20261007.md:541` — 见上。
- 需求优先/partial translation：`mechanism-design-handoff-20261009-round05.md:13` — “L-W 正向手例的 root+middle+leafY=3 次 PTE，可由完整固定请求控制复现”。

### 2.4 理论/文献对照

- `research-review-2026-09-09.md:47-51`：RTCSA 2025 已在 Arm 平台做 DMA/IOMMU 共享干扰测量；TPDS 2017 软件管理 IOTLB 在 miss 处理中继续服务其他请求；MASK/DWS 继续约束。
- `research-literature-review-20261004.md:17-26`：Bermuda / ECRTS2023 / HPCA2017 / MemPol / Revisiting MemGuard / DATE2025 / RISC-V QoSID / US11144473B2 / MemCoRe / HyperTRIO。
- `performance-council-decision-20261005.md:69`：KPart 已把缓存分配与带宽争用联合考虑。
- `composition-history-isolation-20261007.md:284-289`：MASK §5.3–5.4、AXI-REALM §III-A-1/2/3、ABU ECRTS2019 §3。
- `mechanism-design-handoff-20261009-round05.md:25`：ICCD 2018 DOI 10.1109/ICCD.2018.00052、Barr ISCA 2010 DOI 10.1145/1815961.1815970、US9542332B2。

### 2.5 未验证提案（未实现/未运行/未综合）

- `CURRENT_STATUS.md:6-12`：L-M 静态 {X,Y}/{Y} 主例由普通 early-bind+pin 从原始起点复现；E16 实际 mask 相同、额外目的 walk 节省为 0；M20-delta3=17 不是对该强对照的净优势；L-M 需真实原始 COPY/consumer trace 出现合法未解决晚选择；L-W 需两份最小完整状态/端口/协议台账。
- `mechanism-design-handoff-20261009-round05.md:9,13,17`：E-P 是独立静态强对照和手推、不是已运行模型；L-W 完整面积/重启延迟/Fmax 未知，163-bit 示例字段子集不是净节省。
- `composition-history-isolation-20261007.md:274`：Codex 规则“需要新接口/状态及排空证明，尚不能立项或直接改生产 RTL”。
- `composition-history-isolation-20261007.md:304`：B 仍需强普通对照验证。
- `composition-history-isolation-20261007.md:435`：下一主线预约位置候选未实施。
- `mechanism-design-handoff-20261009-round05.md:21`：两条可证伪重启条件。
- `research-morning-20261009.md:39-41`：下一机制假设（A 在旧 V physical R 仍在途时追到 J<E）尚未实现/测试。
- `experiment-plan.md:132-148`：E2 单次部署方案“设计中，未实现”。
- `first-paper-options-20261006.md:5`：A/B 均未通过进入机制实现的创新预审。

---

## 3. 每次转向：触发证据、是否解决前一问题、原目标仍未回答

### 转向 1：从“接口研究优先”回到“性能主线”（2026-10-05）

- 触发：`HANDOFF.md:950` —用户追问“既然当前性能的现有成果为什么突然转向？”根 Codex 承认“先前将 P/N/PN 等具体提案未证新颖性，扩大成接口研究应优先于性能研究，缺少证据和路线比较”。
- 是否解决：部分。`HANDOFF.md:951` 计划修订为继续多 DMA 共享 IOMMU 性能干扰与缓解主线，安全复用接口降为备选。
- 未回答：PN 具体提案仍未被证明是创新，也未默认重编 BIT 或重跑 172（`HANDOFF.md:951`）。

### 转向 2：E1 → E4（2026-09-16）

- 触发：`HANDOFF.md:1436` —E1 主样本 V miss 全 0、A1 也全 0 但软件耗时已为 Solo 的 1.728 倍；`:1438` “先区分 CPU 供给/发现延迟与共享硬件等待，再按证据安排 E2/E3”。
- 是否解决：只定位了供给/发现延迟方向。`HANDOFF.md:1404` —“新增时间 99.08%～99.92% 在 P”；`:1414` “不把 P 全部当 CPU、不把 H 全部当 IOMMU，不宣称 HOL”。
- 未回答：A8 的共享硬件等待形态仍未定量分解。

### 转向 3：E3 触发条件修正（2026-09-17 → 09-18）

- 触发：`HANDOFF.md:1300` —正式 A8 双触发 0/10；静态代码确认“触发只看 AR FIFO 头为 V，A 读在服务期间仍占头，漏掉 V 在其后的场景”。
- 是否解决：v5 新增“V 已接收未选中计数”，v6 受控得到 4 对 cold ΔA/ΔV={127,129,129,130}（`e3-controlled-results-review.md:63-68`）。
- 未回答：`:78` N=4、单次启动、64 B，不解释全部旧 A8 减速、不主张普适 HOL。

### 转向 4：E3 → P4M（2026-09-18/20）

- 触发：`experiment-plan.md:13` —“v6 独立复核已支持……有限机制结论。整体方法论文路线保留，优先进入 P4M 共同窗口测量与固定间隔基线”。
- 是否解决：P4M 建立了共同 V 硬件窗口与 gap 取舍的板测数据；`HANDOFF.md:1172` 却发现开窗 A inflight ID 4/4/1448 的启动相位混淆。
- 是否解决：未完全。`HANDOFF.md:1173` 计划先做最小 driver batch start 协议控制；`HANDOFF.md:1651` 至 2026-09-30 formal60 仍 24 在线+第25离线=25 可用、35 未运行。

### 转向 5：P4M → N/P 设计卡与 M 候选（2026-09-30 → 10-02）

- 触发：`HANDOFF.md:1662` —“尚无已确认创新；固定 gap 是基线，首选评估有限资源下 hit 与 pending miss 的服务解耦”。
- 是否解决：形成 N/P 可否决设计卡（`HANDOFF.md:1678`），并完成五轮审查确定“唯一 M 仍是有条件候选”（`HANDOFF.md:1091`）。
- 未回答：M 只剩“真实控制占用相对强计数/固定类型权重/简单反馈的额外价值假设，尚无实验”（`HANDOFF.md:1095`）。

### 转向 6：method88 阴性停止固定 A8 p5 主创新（2026-10-04）

- 触发：`HANDOFF.md:1040` —p5 对最强匹配简单对照四格全负（−5.68952% / −8.23689% / −5.15142% / −4.43680%），跨方向 False。
- 是否解决：`HANDOFF.md:1040` “固定 A8 主创新路线结束，p5 降为已测试候选/基线；不继续为 p5 挑条件找有利条件”。
- 未回答：`HANDOFF.md:1047` “当前没有 N/P 实现和足够 novelty 差异，不能称已选好新方法或创新成立”。

### 转向 7：N 原型（2026-10-04）

- 触发：`HANDOFF.md:294` 用户要求创新、允许复现以发现新问题；根 Codex 设计 shared source/independent logical job 的 first-VALID 义务控制。
- 是否解决：功能上 v3/v4/v5 各 15/15 完成（`HANDOFF.md:2091,2096,2125,2130,2184`）；但 v3/v4 fast 全 0，v5 触发后 N/S A8 中位 +4.15%、A16 中位 −2.69%、N 相对原版 A8 −14.52%、A16 −22.09%（`HANDOFF.md:2186`）。
- 未回答：`HANDOFF.md:2186` “机制已实际触发，但尚不能宣布性能改善成功”；`:2190` “完整缓冲/命中先服务为已知原则，本轮不宣称新方法贡献”。

### 转向 8：composition 四模式 P/N/PN（2026-10-04/05）

- 触发：`HANDOFF.md:2199-2201` 论文主线五轮复审启动；“主候选收敛为同 BIT 容量保护×命中先服务的有界可行性验证，P 主基线 2/14、后备失效路线本批不实施”。
- 是否解决：172/172 功能通过（`HANDOFF.md:2450-2455`），且隔离使额外 A miss 差 1465→0（`composition-history-isolation-20261007.md:30`）。
- 未回答：PN 仍比 P 慢 12.67%–12.79%（新隔离）与 11.22%–11.64%（旧），剩余约 0.90M 周期差额未拆开（`composition-history-isolation-20261007.md:40-42`）。

### 转向 9：撤回“A/B 为首选”（2026-10-06/07）

- 触发：`first-paper-options-20261006.md:5` —A/B 均未通过进入机制实现的创新预审；`composition-history-isolation-20261007.md:228` —“用户的判断成立：§12 给出的是工程优化入口及待核问题，没有交付可立项的具体创新点”。
- 是否解决：撤回优先推荐；A/B 保留为问题记录，C/D 保留为普通基线。
- 未回答：`composition-history-isolation-20261007.md:239` “后续研究先交付一个具体反例或约束……当前尚未交付这个反例”。

### 转向 10：§44–§48 停止四条规则/remaining-use 独立机制（2026-10-08）

- 触发：`HANDOFF.md:768` —root＋两席核查：scope-aware fast 取消有 2022 前作、每 PTE continuation 有 2009 token/tracking 前作、父/leaf 复用被 Rocket2022 source 覆盖、FQ queue/credit/去重同效；`HANDOFF.md:322` —`ordinary interval/refcount` 同 need/决策。
- 是否解决：停止作为独立机制，保留“下一次只查真实共享结果复用与进度保护”的边界。
- 未回答：`HANDOFF.md:802` “具体增量成立才做最小 out copy”；无 DUT/build/production/Linux/BIT/board 动作。

### 转向 11：§49–§50 共享长读 out-only RTL

- 触发：`HANDOFF.md:294` 用户创新授权 + 复现发现问题。
- 是否解决：功能/事件模型通过有限合同；`:286` “34% 显著依赖读请求模型成本，独立 pause 下 V 实际只低 1～4%，不能直接当独立创新/分区承诺”。
- 未回答：连续 job/重复 pause/多页未验（`:288`）。

### 转向 12：§92/round05 有界暂停（2026-10-09）

- 触发：`mechanism-design-handoff-20261009-round05.md:5` —“本轮完成一次有界创新判断：得到具体复现边界，但没有形成新的已支持独立贡献”；`:17` 静态 L-M/L-W 手例节省均被独立可执行强对照复现。
- 是否解决：把 L-M/L-W 的复现边界和重启条件写清（`:19-21`）。
- 未回答：真实原始 COPY/consumer trace 是否出现合法未解决晚选择；两份最小完整状态/端口/协议台账是否存在不可避免的有用面积/延迟差（`CURRENT_STATUS.md:11-12`）。

### 始终未被回答的原目标（跨阶段）

- “缓存保护何时足够，何时还需干预共享服务”（`experiment-plan.md:11`）—— 未形成结论。
- E2 替换与 E3 阻塞在整体共跑损失中的占比 —— `research-progress-presentation-zh-20261007.md:316` “E2 的 64 项……没有计算它在实板整体延迟中所占的比例”；`e3-controlled-results-review.md:82` 不解释全部旧 A8 减速。
- 强普通基线之外的独立增量 —— `research-literature-review-20261004.md:41` “区别（可争取，但未成立）”；`HANDOFF.md:2210` “创新未成立”。
- 真实应用/工作流外推 —— `research-review-2026-09-09.md:63` “真实应用不是机制研究的前置条件，应用收益声明再配相应证据”；`first-paper-options-20261006.md:86` “无需先搭起 NIC–GPU–CXL 整条系统，也不把微基准当实际部署”。
- 完整 IO/CDC/时钟约束签核 —— 多处标注未完成（如 `HANDOFF.md:1424`、`:2455`、`composition-history-isolation-20261007.md:42`）。
- 是否达到投稿门槛/分区 —— `研究总览_2026-09-08.md:228` “是否足够形成投稿，需要后续数据和独立的相关工作核查”；`paper-route-five-round-review-20261004.md:61` “仍不承诺可发表”。

---

## 4. 文档内部矛盾/过期结论

### 4.1 明确的“最新/下一步”失效

- `HANDOFF.md:1250-1251` —“本文件保留各阶段工作与证据；段落中的‘最新’‘下一步’按当时日期理解。当前状态速览见 CURRENT_STATUS.md。”因此文件内多个“最新”不能按字面当前。
- `HANDOFF.md:1292` —“接手时点提醒：上述地图早于本文件顶部的 E3 v5 实施，地图中的‘当前没有 D0/D0 接口’、旧触发条件和版本指针均属历史状态；当前操作读 E3_QUEUE_PAIR”。
- `experiment-plan.md:3` 标注 2026-10-02 为“当前执行路线”，而 `:15` 仍保留 2026-09-08 的“板上实验尚未执行”，`:13` 又保留 2026-09-18 的 v6 更新；`:132-148` 的 E2 单次部署方案与 `:150-152` “本节取代上面的 64 B／90 次候选”、`:175-179` “本规格实施完成（尚未上板）”三层叠置，需按日期取最新。
- `first-paper-options-20261006.md:15` 表格仍写 A“首选做立题验证”，但 `:5` 已明确“A/B 均未通过进入机制实现的创新预审”；`:7` 又说“先证明新 A 相对字节、请求、miss、事件加权、实占、仅 V 反馈的实质差异”。三处需以 `:5` 为准。
- `performance-council-decision-20261005.md:5` 说“最新优先级……不再以四个 PLRU 对照为论文探索前置条件”，但 `:7` 仍写“下一步先复用现成测试，完成四个真实 IOTLB 的局部对照”；`:9` 又说“下文四个用例……均未实现或运行”。这是同文内历史层与最新层并存。

### 4.2 数据口径冲突

- 84 次 vs 80+2+2：`HANDOFF.md:1083` 明确“合并首次记录为 80 通过＋2 失败＋2 未运行”；`:1130` “原 25＋6＋本次 29＝60 份”；`research-progress-presentation-zh-20261007.md:282` “上次 84 项（6＋60＋18）……旧 84 为 80 成功／2 失败／2 未执行。之后通过 Solo 修正另行补充 11 项。新的 64 项与 21 项另行统计，不按 84/84 计。”
- E2 64 的 128 target：“128 target 中 62 个替换后 firstmiss、66 保持 hit”（`HANDOFF.md:1905`、`research-literature-review-20261004.md:79`）；`research-progress-presentation-zh-20261007.md:296` 明确“128 是对象页面的累计数，不是 128 次独立试验”。
- P4M 不同集合的中位：pilot 21 的 V 中位（Solo 283.508、A8gap0 103.243、gap1000 145.379、gap2000 171.394，`HANDOFF.md:1182`）与 formal 前 24 的中位（108.8275/150.6503/171.4546，`HANDOFF.md:1651`）不可混用；后者按 gap0/gap1000/gap2000 三组。
- P4M 开窗 A 硬件 ID：pilot 为 4/4/1448（`HANDOFF.md:1172`），start-control 为均 6（`HANDOFF.md:1160`）——不同实验/口径，不可拼接。
- E3 “e41/e73 记的是翻译响应，不是数据返回”（`e3-controlled-results-review.md:44`）；若引用 e43/e75 需按数据返回。
- composition-isolation 的 12.67–12.79%（4096 copy，`composition-history-isolation-20261007.md:30`）与 12.85%（gap0 短段 64 copy，`:121`）及旧 11.22–11.64%（`:30`）是不同实验，不可互推。
- §50 “34.1%”与 §51 之后“独立 pause 下 V 实际只低 1–4%”（`HANDOFF.md:286`）是同一阶段不同口径，不能只取 34.1%。

### 4.3 旧目录/旧 ABI/旧接口过期

- `研究总览_2026-09-08.md:101` 主要开发目录写 `/home/xvhaocheng/riscv-iommu-demo-refact`；`:132-135` 源码入口同属旧目录。`HANDOFF.md:1481` 明确“当前开发目录：`/home/xvhaocheng/work/iommu-platform`。旧工程 `/home/xvhaocheng/riscv-iommu-demo-refact` 和 `platform-archives/` 用作历史/备份，默认不在其中开发”。
- `研究总览_2026-09-08.md:161-169` “双路 Independent 已有本地源码实现”基于旧仓库；`HANDOFF.md:1546` “旧 Solo/Pair/Multi/Once 当前均已删除，不能继续写‘旧 independent 已实现所以可直接使用’”，新 ABI 为 0x06/0x07。
- `研究总览_2026-09-08.md:142` 原论文 IOTLB 8 entries vs 当前 RTL 16；`:144` 原论文 Kernel 6.15 vs 当前源码 6.5.0；`:145` 原论文 SRAM 1.5 MiB / DDR3 1 GiB 未复核新工程。
- `research-review-2026-09-09.md:71` 顶层 HEAD `25e452a7…`、组件 HEAD 与 lock 一致——这是当时快照，后续 HANDOFF 有大量新提交，不可当当前。
- `experiment-plan.md:15` “E2/E3 的控制和观测仍待实施”——已被后续 E2/E3 实现取代。

### 4.4 结论层面的内部张力

- `composition-history-isolation-20261007.md:3` 与 `:5`：前者说“PN 仍比 P 慢 12.67%～12.79%”，后者说“gap0 短段中 PN 让 V 耗时增加 12.85%、A 有效速度下降 11.39%”；同一文档不同小节、不同实验规模，引用时必须带条件。
- `composition-history-isolation-20261007.md:32` 明确“A 未命中次数相同只表示数量相同，不表示访问页相同、请求顺序相同、PTW 开销相同或请求时间线相同”；若引用“等 miss”，不可写成“同 miss 同代价”。
- `HANDOFF.md:1967` “原始 E2 报告已接受 64 项……原始 README 只采集禁分析已被本对话明确‘继续分析并制定下一步计划’覆盖，原始 README 不改。”——原始 README 的“禁分析”已过期。
- `研究总览_2026-09-08.md:5` 已自注“当天后续已建立新平台……当前开发目录、补丁提交状态和验收进展以 HANDOFF.md 及 validation.md 为准”；因此该总览中“当前”字样大多为历史。
- `paper-route-five-round-review-20261004.md:73` 已删除/降级“8/8、安全隔离叙事、DID 分区保护匹配域、original=S、B 作主线、hit-under-miss 等创新主张”；若引用早期 P/N/PN 文档中的“安全隔离”“8/8”，属被撤销表述。
- `performance-council-decision-20261005.md:105` 不采纳“单独翻译队列/改变仲裁位置”作为已成立创新；`composition-history-isolation-20261007.md:577` 同样“专用翻译通道/VC、页表优先、朴素动态 burst 不是新贡献”。
- `first-paper-options-20261006.md:35` 引 IOTLB Wall 的“页局部性不是新发现”；`:36` Bermuda 不能宣称首次发现干扰或首次提出限速；`:38` MASK 翻译感知准入不是空白。
- `mechanism-design-handoff-20261009-round05.md:17` 明确“306/319/466/479/163 等命名 bit 子计只是示例，不是净面积，不能加总成优势”；`CURRENT_STATUS.md:10` 同。

---

## 5. 导航式关键原始实验或源码路径 + 下一步应定向核读的最少文件

### 5.1 关键原始实验路径（按主题）

- E3 v6 原始与复核：`iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/`、`iommu-platform/out/e3-controlled-review-20260918/server-review.json`、`review-summary.json`（`e3-controlled-results-review.md:13,106-112`）。
- E4 observer/batch：`iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/`（`HANDOFF.md:1416`）；`iommu-platform/out/mac-batch-main-20260916-112954-run-9sa69rhc/`（`HANDOFF.md:1392`）。
- DS0/DS50：`iommu-platform/out/mac-batch-ds0-20260916-132332-run-ebgxhirr/`（`HANDOFF.md:1370`）；`iommu-platform/out/mac-batch-ds50-20260916-135544-run-eawziww2/`（`HANDOFF.md:1356`）。
- P4M：`iommu-platform/out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/{RESULTS.md,records.json,observations-unique.log,console.raw.log}`（`p4m-pilot-results-review.md:3`）；`iommu-platform/out/mac-p4m-formal60-20260920-215406-run-cicggil4/`（`HANDOFF.md:1148`）；`iommu-platform/out/p4m-formal60-recovery-20260920/{delivery/README.md,...}`（`HANDOFF.md:1140`）；`iommu-platform/out/mac-p4m-remaining29-20261002/`（`HANDOFF.md:1128`）。
- E2 64：`iommu-platform/out/e2-results-review-20261003/{RESULTS.md,offline-v2,audit-final,accepted-summary.json,integrity-result.json}`（`HANDOFF.md:1911`）。
- method88：`iommu-platform/out/method88-execution-20261004/RESULTS.md`；`iommu-platform/out/method-results-review-20261004/{accepted-evidence.json,RESULTS.md}`（`HANDOFF.md:1961-1965`）。
- fullstats21：`iommu-platform/out/research-fullstats-review-20261003/{acceptance.json,direct-check.json,condition-statistics.json,analysis/results.json,RESULTS.md}`（`HANDOFF.md:1811`）。
- 00091 cost-followup：`iommu-platform/out/research-followup-review-20261002/{direct-check.json,acceptance.json,RESULTS.md}`（`HANDOFF.md:1776`）。
- N 原型：`iommu-platform/out/n-prototype-20261004/analysis-v5-pilot/{RESULTS_REVIEW.md,RESULTS.md,nonblock-pilot-analysis.json}`（`HANDOFF.md:2190`）；各版原始目录 `nonblock-pilot-20261004-serial-v3-01/`、`-hit-v3-01/`、`-serial-v4-01/`、`-serial-v5-01/`、`-hit-v5-01/`（`HANDOFF.md:2091,2096,2125,2130,2184`）。
- composition 修复：`iommu-platform/out/composition-fix-20261005/execution-state.json`、`iommu-platform/out/composition-hangdiag-20261005/{n4096-hang-snapshot.json,n4096-hang-decoded.json,acceptance.json,mac-records/diagnostic-board.tar}`、`iommu-platform/out/composition-dsfix-20261005/`、`iommu-platform/out/composition-ptwfix-20261005/{board-acceptance.json,targeted-test-acceptance.json}`（`HANDOFF.md:2360,2404,2445,2454`）。
- composition-isolated：`iommu-platform/out/composition-isolated-20261007/{board-acceptance.json,comparison.json,production.patch,mac-records/composition-isolated-20261007-formal01/summary.json}`（`composition-history-isolation-20261007.md:15,21,36,52`）。
- PN 数据路径/板端 gap：`iommu-platform/out/pn-data-path-20261007/{comparison.json,trace-evidence.json,acceptance.json,source-provenance.json}`；`iommu-platform/out/pn-board-path-20261007/{reduction.json,board-acceptance.json,reviewed-plan.json,mac-records/pn-gap64-20261007-formal01/}`（`composition-history-isolation-20261007.md:101,135`）。
- §19–§26：`iommu-platform/out/owner-baseline-20261007/{acceptance.json,runs.json}`、`iommu-platform/out/pte-progress-poc-20261007/{measurements.json,comparison.json,acceptance.json}`、`iommu-platform/out/payload-frontier-poc-20261007/{runs.json,trace-equivalence.json,README.md}`、`iommu-platform/out/read-service-boundary-20261008/{runs.json,trace-analysis.json}`（`composition-history-isolation-20261007.md:337,389,500,526`）。
- §33–§39：`iommu-platform/out/posttl-independent-20261008/{analysis.json,codex-review.md,stage-evidence.json}`、`iommu-platform/out/parent-capacity-20261008/`、`iommu-platform/out/fragment-placement-20261008/`、`iommu-platform/out/parent-translation-reuse-20261008/{lookahead-witness.json,codex-metric-audit.json}`、`iommu-platform/out/original-window-20261008/`、`iommu-platform/out/multi-parent-preparation-20261008/codex-raw-audit.json`、`iommu-platform/out/prepared-source-bypass-20261008/{codex-raw-audit.json,stage-evidence.json,codex-next-boundary.md}`（`HANDOFF.md:643,653,669,685,698,702,716`）。
- §50–§92：`iommu-platform/out/shared-read-rtl-20261009/{codex-result-acceptance.md,execution-ledger.json,README.md,critical-write-recipe-review.md}`（`HANDOFF.md:290`）；后续 `shared-read-*/codex-result-acceptance.md`、`execution-ledger.json`（`HANDOFF.md:242,256,260,274` 等）；`iommu-platform/out/higher-model-mechanism-20261009/round05/root-result-acceptance.json`（`CURRENT_STATUS.md:5`）。

### 5.2 关键源码路径

- 四 DMA / DID/PID：`cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:867`（`研究总览_2026-09-08.md:132`）。
- IOMMU 容量/HPM：`cva6/corev_apu/tb/ariane_soc_pkg.sv:110`（`研究总览_2026-09-08.md:133`）。
- DS 路径延迟：`cva6/corev_apu/fpga/src/ariane_xilinx.sv:163`（`研究总览_2026-09-08.md:134`）。
- 事务控制器：`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:646,669`（`research-review-2026-09-09.md:39`）、`:579` AW 优先（`：41`）、`:623-647`（`composition-history-isolation-20261007.md:170`）。
- IOTLB：`cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv`（`experiment-plan.md:83`；`performance-council-decision-20261005.md:33`）。
- PTW：`cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:385-417`（`completion-reserve-exploration` 引用，见 `composition-history-isolation-20261007.md:201` 段落）。
- DS IF：`cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_ds_if.sv`（`HANDOFF.md:2404`）。
- iDMA coupler：`cva6/corev_apu/iDMA/src/idma_channel_coupler.sv:105,140-179`（`composition-history-isolation-20261007.md:170`）。
- banked ctl：`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:246-278,372-408`（`composition-history-isolation-20261007.md:170,197`）。
- AXI demux：`cva6/vendor/pulp-platform/axi/src/axi_demux_simple.sv:175-177`（`composition-history-isolation-20261007.md:64,170`）。
- HPM：`cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_hpm.sv:258`（`研究总览_2026-09-08.md:179`）。
- iDMA frontend：`cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:175`（`研究总览_2026-09-08.md:157`）。
- Linux 驱动：`linux/drivers/misc/idma.c`（`研究总览_2026-09-08.md:171`）；`linux/drivers/iommu/riscv/iommu.c:2136`（`：179`）。

### 5.3 下一步应定向核读的最少文件（建议顺序）

1. `CURRENT_STATUS.md`（14 行，整体状态与暂停语义）。
2. `iommu-platform/docs/mechanism-design-handoff-20261009-round05.md`（25 行，L-M/L-W 复现边界与重启条件）。
3. `iommu-platform/out/higher-model-mechanism-20261009/round05/root-result-acceptance.json`（唯一裁决权威，`CURRENT_STATUS.md:5`）。
4. `HANDOFF.md:1-30`（§92/§91）、`:196-228`（§76–§78）、`:279-290`（§50）。
5. `iommu-platform/docs/composition-history-isolation-20261007.md:1-46`（PN 板端与解释边界）、`:87-105`（模型 DDR 因果）、`:226-308`（撤回优先推荐与两条规则查验）、`:442-541`（共享池停止/同 bank 边界/请求头暂存）。
6. `iommu-platform/out/composition-isolated-20261007/{comparison.json,board-acceptance.json}`。
7. `iommu-platform/out/e2-results-review-20261003/RESULTS.md`。
8. `iommu-platform/out/method88-execution-20261004/RESULTS.md` 与 `iommu-platform/out/method-results-review-20261004/accepted-evidence.json`。
9. `iommu-platform/docs/e3-controlled-results-review.md`（E3 有限机制，勿扩读为普适 HOL）。
10. `iommu-platform/docs/p4m-formal60-recovery.md` 与 `iommu-platform/out/p4m-formal60-recovery-20260920/delivery/README.md`（formal60 状态、resume35 前校验）。
11. `iommu-platform/docs/research-review-2026-09-09.md`（首篇收敛原则与测量限制）。
12. `iommu-platform/docs/paper-route-five-round-review-20261004.md`（composition 172 规格与停止门禁）。

> 不建议在未读上述 1–4 之前重读早期 `研究总览_2026-09-08.md` 全文；该文已自注“以 HANDOFF.md 及 validation.md 为准”（`研究总览_2026-09-08.md:5`）。

---

## 6. 不确定性

1. **板上 bitstream/Linux/rootfs 与源码的对应关系**：`研究总览_2026-09-08.md:126-128` 明确“板上实际运行版本未核实”“当前烧录版本未确认，需要 bitstream、Image、rootfs hash 与运行日志对应”。后续 HANDOFF 多次标注“SD 回读未验证”“下载哈希未做”（如 `e3-controlled-results-review.md:95-97`），因此任何“实板”结论都必须带版本对应条件。
2. **HPM 校准与 per-DID 归属**：`研究总览_2026-09-08.md:155` 明确“现有驱动使用全局事件配置，不能当成已经校准的 per-DID miss 数”；`:175-179` 记旧 bitstream 的 miss/PTW 计数曾出现接近 elapsed cycles 的异常，本地 HPM RTL 有未提交 edge-latch 修改。后续 E1 也标注“HPM 归属尚未校准”（`HANDOFF.md:1543`）。因此本文引用的 HPM/miss/PTW 数字不能直接当硬件事件真值。
3. **时间/统计单位**：`research-review-2026-09-09.md:33` 明确“试次内几千次重复拷贝不是几千个独立样本”；`e3-controlled-results-review.md:78` N=4；`composition-history-isolation-20261007.md:34` n=3 描述性；method88 每条件 n=3（`HANDOFF.md:1987`）。本文未做显著性判断。
4. **“等 miss / 等工作”不等于“同地址/同顺序/同代价”**：`composition-history-isolation-20261007.md:32` 已明确；引用时必须保留。
5. **模型不等于平台**：`HANDOFF.md:2186`、`composition-history-isolation-20261007.md:103,131` 均标注行为 DDR/合成 CPU 与真实 MIG/CVA6/Linux 不等价；`HANDOFF.md:800` 标注“只是 cache/miss 模型，非 DUT 或 copy 速度”。
6. **文档层叠**：HANDOFF 是累积文件，内含多轮“最新”；本文已尽量按日期与自注取最新，但无法保证未漏掉更晚条目。凡引用具体数字，建议回到对应 out 原始 JSON/CSV 复核。
7. **未取得/未核的文献**：NB-Walker 全文（`composition-history-isolation-20261007.md:153,306`）、HyperTRIO 全文（`research-literature-review-20261004.md:26`）、CATO 全文（`performance-council-decision-20261005.md:895` 附近）等；因此本文不把任何“未见同名”当作空白证明。
8. **归属分工**：多份文档均标注 Codex 负责关键设计/源码核/执行/验收，DeepSeek 负责证据/草案，Claude 仅在明确授权时调用；这不影响证据本身，但影响“谁验证过”的审计链。
9. **本文未做**：未重跑任何实验、未读未提供的 out 原始文件、未核源码行号是否仍与当前工作树一致、未判断 L-M/L-W 是否值得继续、未给创新或投稿建议。以上属 Codex/用户裁决范围。