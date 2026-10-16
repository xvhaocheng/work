# 固定 Ramulator2 的写完成合同核查

## Material Passport

- 阶段：ARS-Codex 有界源码核查／实验准入合同；不是实验结果。
- 输入：固定 `be93be78055d922aa1d4d33e15bcc8f2b0c61a9d`、既有 boundary-config、M0 的 memory drive/consume 局部。
- 交付：本卡、`bridge-contract.json`、`source-read-ranges.json`、逐行原文 `source-excerpts.txt`。
- 请求配置：Sol/high；实际模型和强度运行元数据 unknown。没有改源码、构建或运行模型；没有 DeepSeek 调用。

**结论：该 Generic controller 只提供“写请求被接收”和“最终 WR 已发出”；没有写数据结束／写恢复结束回调。可以定义两种新的 private bridge B 模型，但必须标为桥接假设。真实 AXI/MIG B 和真实 DDR 数据提交时间仍未知。**

下列 `src/...` 位置均以 `/home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/implementation/ramulator2-be93be78055d922aa1d4d33e15bcc8f2b0c61a9d/` 为固定源码根；各绝对路径、hash和逐行原文在伴随文件中。排空和 REF 规则复用上一轮 `interface/boundary-card.md`，不重复审计。

## 源码直接保证和缺口

| 边界 | 固定源码证据 | 可接受含义 |
| --- | --- | --- |
| `send=true` | Generic controller `:144–160` enqueue；GenericDRAM `:50–72` 返回结果 | 接收进入模型队列，不是 WR、更不是数据完成。 |
| 最终 WR issue | controller `:204–214` 先 issue，Write 分支只有 TODO，随即 remove | 实际写命令发出；写请求不进入 pending、没有设置写 depart、没有调用写 callback。 |
| pending callback | controller `:9,208–210,298–318` | pending 用于正常读及 write-buffer forwarding 的读。没有写完成实现。 |
| 写数据末 | DDR4 `:443–460,477,487` | 时序公式分开出现 nCWL、nBL、nWR；可据此**新增推导期限**，源码没有独立 data-end 事件或数据管线。 |
| 写恢复／关行许可 | DDR4 `:460,487` | WR→PREA/PRE 的下界是 nCWL+nBL+nWR；还受其他历史约束，达到这个下界不等于一定可立即 PRE，更不等于已经关行。 |
| precharge 完成／再 ACT | DDR4 `:465,468,485` | 实际 PRE/PREA 之后还需 nRP；nRP 不能无条件加到未关行 WR 的“完成时间”。 |
| future action | DDR4 `:220–243` | 该模型只有 REFab 的 future action，WR 不会留下 future token。因此容器和 future 全空仍可处在最后一次写的恢复期。 |

`IDRAM` 只有 `m_read_latency`（`dram.h:59`，DDR4 `:435` 为 nCL+nBL），没有对称的 `m_write_latency`。Request 的 `m_payload` 是通用指针，不含官方 byte-size/mask/atomic 协议（`request.h:12–43`）。不能把泛用 payload 字段说成库已经处理数据或写提交。

DDR4 规则：若 `t` 为**实际** WR issue tick，则定义新桥接推导量：

```text
model_data_end = t + nCWL + nBL
model_recovery_mature = t + nCWL + nBL + nWR
WR→RD(other bankgroup in rank) >= t+nCWL+nBL+nWTRS
WR→RD(same bankgroup)          >= t+nCWL+nBL+nWTRL
```

这是从本实现时序项导出的私有模型约定，**不是该库已有 write-completion 回调，也没有 PHY/DQS 结束测量**。取半开数据区间 `[t+nCWL, t+nCWL+nBL)`；post-tick `now>=deadline` 可标期限成熟，不能照抄 REF 的 `−1`。node `:145–176,209–232` 用 `past+latency` 更新，`clk<ready_clk` 才拒绝；真正命令 ready 还须沿地址路径核查完整 timing 与前置状态。

固定 boundary-config 是 64-bit channel、DDR4_2400R：表 `DDR4.cpp:38` 给 nCWL=12、nBL=4、nWR=18、nRP=16、nWTRS=3、nWTRL=9。因此上述期限为 WR+16、WR+34；相应读下界 WR+19/25。运行时仍须记录解析后的数值，配置可覆盖 timing（`:417–424`），不能仅复制 preset 名称。库还另有 sibling-rank 约束 `:456–457`；本卡不以简化单rank公式替换它。

GEM5 wrapper `resources/gem5_wrappers/ramulator2.cc:208–234` 同时登记一个写 callback 和在 enqueue_success 时直接 `accessAndRespond(pkt)`。配合上述 Generic 源码，该 callback 不会被调用；`:259–290` 的 functional access／response 在包装层处理。它证明包装层可以采用 posted 语义，并暴露该 wrapper outstanding 写计数不能直接当本轮 drain 的问题（`:305–309`）。这不是 AXI，不能由此推出生产 AXI B 必然早于数据完成或本平台 B 无效；本轮也不修 GEM5 wrapper。

## 写缓冲、水位和统计

controller `:55–56` 默认 low=0.2、high=0.8；`:325–335`：read_buffer 空即进入 write mode，退出 write mode 需 write_buffer 低于 low **且 read_buffer 非空**。所以少量有限纯写不会因为未达 high 永久留着；pending 读不参与这一空读判据。active buffer 优先，维护 priority 在后、普通读写选择再后（`:342–375`），因此这里只排除“高水位自身阻止有限纯写”，不是任意流量下的有界公平性证明。finite drain 必须实际观察 WR，超时报告失败，不能切模式或塞假请求催写。

`ReqBuffer.max_size=32`，但 `enqueue` 条件是 `size<=max_size`（`request.h:47–65`），可到 33；高水位严格 `>25.6`，低水位严格 `<6.4`。不可把32当实际硬容量、不可为修此点改上游或人为缩小队列。

`num_write_reqs` 在 controller `send` 接收前就加（`:121–123`），失败重试也计；GenericDRAM 的 `total_num_write_requests` 只在成功 send 加（`:55–63`）。二者都不是 WR final-issue 数。write row stats 在首次被调度时更新（`:201–202,253–288`），也不是写完成。官方 CommandCounter/TraceRecorder 在 controller issue 之前由 plugin update 执行（controller `:193–204`；两个插件 `:50–53`／`:46–56`）；可作正常路径交叉核账，但完整完成账应在实际 issue 返回后记录。

## 两种可选 private bridge 模型 B

共同前置条件：每个 AXI parent 已接收 AW 及恰好 `LEN+1` 个 W beat、WLAST 精确匹配；所有 timing child 已成功 send；一笔 parent 只产生一个 B，保持 ID/顺序及 VALID held 规则。B 只表示选择的模型语义，最终整任务 drain 不得省略。

1. **posted-accept B**：满足上述共同条件即允许产生 B；表示 bridge 接管全部数据和 timing children，未表示子写已 issue。来源是 `send=true` 的接收语义及 GEM5 的接受后回应例子。B 后 bridge 仍拥有全部未 issue/未成熟写债务，不能因上游任务 done 就销毁对象。
2. **derived-data-end B**：共同条件并且该 parent 每个 child 都有一条真实 final-WR 事件，post-tick 已到达 `max(t_WR+nCWL+nBL)`，才允许 B。属于本轮新增推导数据结束模型；库没有提供这一回调。其后恢复债务仍需保留至 `max(t_WR+nCWL+nBL+nWR)`。

不另提出 recovery-B 选项；恢复期限用于两种方案共同的最终 drain。主会话应在执行前选定其中一个，不混合跨run解释，也不因模型排序好看换语义。二者都不等于 native MIG B、应用持久化或板上数据完成。

## 64B timing child、8B AXI beat 与所有权

DDR4 prefetch=8（`:67`）、64-bit channel（配置及 `:268`），mapper `linear_mappers.cpp:34–38` 算出 64B transaction，地址低6位在映射前丢弃（`:84–91`）。库并不自动把8B request合为64B，也不自动拆 AXI burst。应由 bridge 明确实施：每个 parent 按实际字节地址切为64B行片段，**每个 touched 64B行每个 parent 一个 child**；child 只在该片段所属所有 W beat 已握手、payload 已保存后允许 send。不得把8个8B beat分别当8个64B写，也不得把两个 parent/ID/先后版本碰巧同一64B免费合并。若 AW 64B对齐、8B beat且 `LEN+1` 是8倍数，child数=`(LEN+1)/8`；否则按实际触及块数，不能向下取整。AWLEN=255、8B每拍=2048B，对齐时32 children；4KiB是512拍，不能假设一个8-bit LEN AXI burst容纳。

M0 局部现状：`tests/nonblock_actual_dma_test.cpp:827–841` 捕获 AW地址/LEN/ID 和W数据/strobe/LAST；`:1331–1353` 用 AW FIFO 头给 W 归属，按 `addr+got*8` 每次W握手即 `write_beat`，最后W时把 ID放 cb_q；`:669–677,1356–1369` 再驱动和消费 B。这是已有功能内存语义，**没有 DDR4 WR服务时钟**。`:522–525` 保留未选字节。全调用链/真实 AXI size、burst、atomic 范围由root核查，不在本卡冒充已验。

新 bridge 需显式保存 `parent_seq, AXI_ID, AW地址/LEN, beat_index, byte_mask, payload版本, child_index, block_addr, accepted/WR-issued/data-end/recovery/B状态`。跨相同ID父事务及别名读写的顺序必须守住；后续读若与 posted 写重叠，不能仅靠 Ramulator 的 forwarding（只按 `wreq.addr==req.addr` 且只查 write_buffer，controller `:131–140`）来提供实际数据正确性。若冻结输入没有此冲突，应有实际轨迹证据；若有而未定义 ordering/forwarding，则拒绝此版桥接验收。

**最小准入范围**：首个精确服务模型仅接收 root确认的8B INCR、64B对齐完整块、全strobe非atomic传输。partial/strobe在原M0功能上被处理不代表库为其建模 RMW；若实际轨迹含 partial、窄传输、atomic或特殊属性，必须明确停止／另行定义有来源的映射与成本，不能静默当成已验证的全块写。可以保存原功能strobe行为作功能对照，但不能据此声称部分写性能精确。本卡没有说这些情形一定在实际轨迹中缺席。

## 最终 drain 和最小 passive 记录

两种B模型统一通过：

- 已停止新工作注入；所有AW都有完整W及精确WLAST；不存在未归属W、未生成/未接收child；所有read child也已完成并交付。
- accepted write-child集合与真实 final-WR issue集合一一相等且无重复；不能只比较总数。每个parent恰好一个B并已握手；桥接payload/ownership/heldR/heldB/原功能队列均空，上游自然完成与quiet照原合同核。
- 在完整 `memory.tick()` 返回后，复用上一轮全部controller五容器空、future空、所有bank非Refreshing；不得只读统计、`ddr_model_empty`或callback时的局部快照。
- `now >= max(all accepted write children's t_WR+nCWL+nBL+nWR)`；如未有WR事件或期限无法匹配则失败。无写集合时该条件为真。继续正常tick并同时处理自然刷新，不停refresh/清私态。此判据是显式新增的“恢复债务已成熟”，不要求人为关行、清row/cap或额外idle来抹状态。若目标另要求PRE后ACT-ready，须记录实际PRE和nRP、完整ready路径，不由这个drain推得。
- root完整callchain定义的功能完成/quiet和模型服务drain必须各自成立。最终报告分列 `last_B_handshake`、`last_WR_issue`、`last_model_data_end`、`last_model_recovery`、`full_drain`；周期单位与RTL/memory tick换算须先冻结，不能把1:1当已测硬件关系。

最小记录方案（**只提案，未实施**）：在 controller `m_dram->issue_command(...)` 返回后、remove前，若type=Write且command=final_command=WR，复制 `{memory_tick, controller_tick, channel, raw_addr, addr_vec, bridge_child_token, parent_seq, child_index, nCWL,nBL,nWR}` 到独立只读sink。token可由新bridge置于现有 `Request.m_payload`，指向稳定且不可变的桥接对象；所选Generic/FRFCFS/mapper/rowpolicy不按该字段调度，实际patch仍须核不影响选择。禁止改 `source_id` 当唯一token（它有per-core含义），禁止根据地址猜重复请求归属；token至hook复制完成前不得释放。普通插件只有地址/命令记录不够给重复地址分配唯一child。

该记录钩子不插入命令、不加tick、不调用callback、不发新请求、不改buffer/ready_clk，不给P/PN控制器额外信息；若bridge用它形成模型B，只有bridge能消费，策略观察仍按root冻结O0。另记成功/失败send、W/B握手及全tick末快照。被动性验收应对固定输入比较开启/关闭记录的命令序列、接收结果和完整输出；未实际比较前只能称设计约束。传感器实现费、native观测费、clock换算和生产应用费用继续unknown，不填0。

必要负例：queue空但WR+1（未到数据末）；posted B早于WR但不得提前全任务结束；最後data-end已到但recovery未到；拒绝send后重复入账；同地址不同parent误合并；WLAST缺/早/晚；BREADY保持低导致held B；partial/atomic越域；少量纯写没到high仍需实际发出；有维护在途/已排空又新REF时不得沿用旧快照。未执行这些负例，不标测试通过。

本卡只接受源级完成域和待选模型合同；不接受 P/PN收益、真实DDR完成、硬件性能或免费状态读取主张。
