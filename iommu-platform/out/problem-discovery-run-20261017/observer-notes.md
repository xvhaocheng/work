# DISCOVERY-REUSE-1：只读观察接口

2026-10-17；Codex-only。实现席仅修改本轮独立副本的 `overlay/tests/nonblock_actual_dma_system_top.sv`，未修改生产RTL、旧实验、C++或共享研究文档，未编译/运行。编译及运行验收由root统一完成；本文描述源码绑定，不能替代实际仿真证据。

相对路径均以本轮目录为根；`S`=`overlay/tests/nonblock_actual_dma_system_top.sv`，`T`=`overlay/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv`，`W`=同目录`rv_iommu_ptw.sv`。未复制的N模块源码为 `../../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv`（该相对路径从本轮目录出发）。

## 只读与固定基座

新增端口声明在S:118–148，绑定在S:1539–1601。全部为连续赋值，只读已有层级信号；没有新增寄存器、DUT输入、控制反馈、握手门控或服务逻辑。条件generate只防止访问未elaborate的N层级，不改变原模块。原E3/lookup/PTW/selector/backend/drain/AW等tap保持原式。

S:954–955、974仍由PairPolicy选择M、固定`CommonDsFirstEnable(1)`；本轮要求PairPolicy=0、P1及N0/N1 runtime，同一elaboration。DS50/50仍为S:96–97、508–527；无新增延迟或R返回调度。M/O模块是否被工具删除不是本席声称的验收；其使能须由root构建参数及runner记录确认。

工作树身份只读核对：CVA6实际HEAD=`a1b08934391833aed21e6ad63b545634040808ac`，与版本锁一致；IOMMU实际HEAD=`57eb5621802ef9ed8d77c684b9a03205c6630391`，不同于锁中的`238a39e...`（已有独立维护状态）。本席没有提交/重置内部组件，也没有改锁；实际编译身份由root manifest记录。

## 新接口：宽度与有效时刻

下表中的`bit`为1位`logic`。所有元数据仅在对应VALID/accept/start有效时有意义；无效周期可能残留旧值。

| 输出 | 类型 | 语义 |
|---|---|---|
| `dbg_disc_lu_valid` | bit | 物理IOTLB端口lookup_i；普通PT/MSI或N共享端口。 |
| `dbg_disc_lu_is_n` | bit | 真正的`n_iotlb_lu`选择；1时VPN/上下文及下列身份属于N保存的probe，不能用main owner归属。 |
| `dbg_disc_lu_hit` | bit | 物理`cache_iotlb_hit`，包括N查询结果；不是故意屏蔽N的`iotlb_lu_hit`。 |
| `dbg_disc_lu_bank/did/type/iova` | bit/u24/u6/u64 | N时来自`gen_nonblock_hit.i_hit.req_q`；否则来自normal `req_data`。type保留原6位ttype。 |
| `dbg_disc_lu_first` | bit | N LEAF查询，或req_valid且PT状态且`!e3_seen_q`的首次normal查询；MSI不标记normal-first。 |
| `dbg_disc_normal_accept` | bit | `trans_req_valid_i && normal_trans_ready && normal_gate`，真实normal请求接受。 |
| `dbg_disc_n_accept` | bit | 原`n_accept`，真实N命中接受，非任意probe。 |
| `dbg_disc_req_bank/did/type/iova` | bit/u24/u6/u64 | 输入共享请求端的身份；normal_accept或n_accept成立的采样边沿绑定。 |
| `dbg_disc_main_valid` | bit | normal spill输出`req_valid`，是驻留VALID，不是每拍新接受。 |
| `dbg_disc_main_state` | u5 | 原3位TL状态零扩展：0 IDLE、1 DDT、2 PDT、3 PT、4 MSI、5 ERROR、6 COMPLETE。 |
| `dbg_disc_main_bank/did/type/iova` | bit/u24/u6/u64 | 稳定normal owner；PTW期间被N旁路使用物理cache端口也不改变此owner。 |
| `dbg_disc_ptw_start` | bit | PTW原`m_normal_s1_start_o`；IDLE/init/非edge_trigger且S1-on/S2-off/非implicit，未加W-only、M或root资格。 |
| `dbg_disc_ptw_state` | u2 | PTW 0 IDLE、1 MEM_ACCESS、2 PROC_PTE、3 ERROR。 |
| `dbg_disc_ptw_level` | u2 | 原`s1_lvl_q`：0 Sv39根/L2、1中层/L1、2叶层/L0；在local AR offer/accept绑定当前PTE。 |
| `dbg_disc_ptw_addr/id` | u64/u4 | local PTW AR地址/ID；与原`dbg_ptw_local_valid/ready`同端点。 |
| `dbg_disc_ptw_r_valid/ready/id/last` | bit/bit/u4/bit | DS返回到PTW的local R端点；valid&&ready才消费。 |
| `dbg_disc_ds_merge_valid/ready/addr/id/len` | bit/bit/u64/u4/u8 | 真实DS AR spill输出，50/50 delayer之前。 |
| `dbg_disc_ds_ar_addr/id/len` | u64/u4/u8 | delayer之后、xbar slave3入口；与原`dbg_ds_ar_valid/ready`同端点。 |
| `dbg_disc_ds_r_id/last` | u4/bit | 同一post-delayer接口返回R；与原`dbg_ds_r_valid/ready`合用。 |

## 查询、接受与响应不能混计

T:277–283是实际物理lookup mux：N查找覆盖normal VPN、PSCID及stage选择，`iotlb_lu_hit`在N查询时人为置0。因此新增接口绑定原始`cache_iotlb_hit`及N保存身份，而原`dbg_main_bank`仍只代表normal owner。关键原文绑定在S:1542–1565；未把下游已知结果用于任何控制。

普通请求的`iotlb_lu`可在整个PTW期间连续保持，每拍重复检查不能计为多个接受或miss。T:717–725的`e3_seen_q`在IDLE清零、首次PT置位，本轮复用此已有标记；统计唯一normal首查应取`lu_first && !lu_is_n`。后续物理hit变化可以记为状态，不能再算独立请求。normal_accept早于首查；req_valid覆盖驻留生命周期；normal响应经原shared `dbg_tl_resp_valid/ready/bank`识别。没有名为st_valid/n_valid的新状态，以上精确接受与驻留VALID已满足去重。

`lu_first`的normal部分是**已有e3_seen＋实际physical-port的首查标记**，不是normal_accept的别名。冻结S1夹具中N不能抢走首次normal PT检查：T:290–291要求新main在IDLE、无req_valid、无n_intercept时接受，排除前一N start/DC/LEAF/OUT未清；同main的N start又要求T:309–312的`n_window`成立，即PT且PTW active。首次PT检查时PTW仍IDLE/active0，下一边沿才接受walk并置e3_seen；之后N才可start并经过DC到LEAF。若未来改接口或支持其他路径破坏此前提，应将缺失normal首查记为unknown，不以N端口结果制造normal miss。

N模块原源码:47–54、100–109、142–172：保存候选后依次DC→LEAF，LEAF仅一拍，随后OUT或REJECT。LEAF查询失败、权限不符、上下文不稳或2MiB叶不合N资格时均可能不接受；同一外部请求随后正常路径接受。因此N probe miss必须另列，不能计为已接受normal miss或新工作。N physical hit也不必等于N accept（例如普通2MiB叶）；真正接受取n_accept，响应经原fast/shared response VALID/READY计数。P runtime关闭N仍保留同一elaboration，不改变这些定义。

## PTW层级与端点身份

W:93–111定义PTW状态及层级；W:154–155的真实normal S1 start包含edge qualifier，不能从一直为1的init/active推测新walk。T:787–811将start/active/level与normal owner直连。一个walk开始时保存main身份，随后各PTE按local occurrence编号配对，直至原active下降；不能把缓存填充/最终hit当成第二个walk。错误路径保留原错误/状态，不能以active结束单独证明成功。

W:279–284固定local PTW AR ID0、len0、size8B，ID会复用；每个PTE occurrence须分端点，不能用ID永久代表首PTE。Sv39普通2MiB叶可在level1结束，4KiB通常继续level2；W:498–506保留原superpage对齐和A/D检查。新增tap不改变资格，也不把PN大页的N落回normal误称纯walk差异。

端点顺序为：

1. local PTW `dbg_ptw_local_valid/ready`＋`dbg_disc_ptw_addr/id`：PTW→真实DS arbiter/AR spill的接受。
2. DS spill输出 `dbg_disc_ds_merge_*`：spill→delayer入口。真实模块 `../../cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv:77–109` 将七路arbiter写入非bypass spill。
3. post-delayer `dbg_ds_ar_valid/ready`＋`dbg_disc_ds_ar_*`：xbar slave3入口，S:508–531。与上一步绝非同一接受。
4. 原`dbg_sel_valid/ready/id/addr/len`：共享DDR AR选择器输出，S:1504–1509；这是已选中输出offer，不代表所有输入都获得grant。
5. 顶层DDR AR VALID/READY：真实行为DDR接受；之后逐R VALID/READY/beat/last配对。selector→DDR之间仍有真实cut，二者不能合并时间。

DS还含冷DDTW；通过地址、实际ID及端点顺序匹配，不把所有DS流量都叫PTW。字段上游为u4 ID、共享selector/DDR为u6 ID；xbar加port bits，不能将不同宽度ID直接等同。单PTW单PTE的本轮能力允许顺序队列配对；无法证明匹配时应FAIL/unknown而非猜owner。first offer与accept分开，held期间保持同一occurrence；valid&&ready每个采样边沿才是一次接受。

原backend接受及source-drained在S:1519–1535保留。source-drained是raw读legalizer谓词，必须结合真实backend-active和parent beat守恒，不能把job FIFO中未开始的任务当source已结束。

## 等待及模型真值的边界

原AW flags S:1453–1469保持四个可重叠条件：bit0跨选路W open、bit1计数/容量、bit2同ID占其他路、bit3下游AW不ready；低bit优先只是旧分类，不是唯一根因。raw VALID/READY状态变化须由runner全时代实际写出，不能依赖旧full-era关闭的legacy trace。

新增tap只给实际局部端点和状态；没有提供consumer、CPU、MIG、实际DDR调度或cache victim/替换身份。C++行为DDR当前request、startup倒计时、剩余beat、held response等只能作为**模型真值**另列；本文件没有把这些信息送回RTL，也没有把它们声称为可部署控制接口。不同等待可重叠，不能把各tap持续拍数相加为可回收收益。

本席完成的检查仅为关键层级/类型/源绑定人工核查及差异格式检查，编译/运行0。所有首查、N拒绝、walk、各端点occurrence和最终parent守恒仍须root实际构建/运行验收。
