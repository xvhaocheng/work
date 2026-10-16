# 强普通对照：小缓冲可以完成，创新门槛改为速度与总成本

2026-10-08，研究记录 §32，接[自然写序等待环 §31](natural-write-order-boundary-20261008.md)。**普通的准确源读准入门已用小数据 FIFO 完成本轮长任务，所以解除该等待环本身不能作为新贡献。当前剩下的是提前译址、数据服务顺序与资源成本之间的取舍；新 I 调度仍未实现。**

## 实际改动与执行

Codex 在独立 `out/` 写了 [ordinary_copy_owner_gate.sv](../out/write-owner-baseline-20261008/ordinary_copy_owner_gate.sv)，将其接到每 DMA 源 AR 与四输入 mux 之间。它只门控 AR VALID/READY；AW/W/B/R 和读请求字段直接透传。源读放行须满足：

1. 虚拟和物理 completion W 队头都是该 DMA，且该流两侧已完成写段序号相同。
2. 对应目的 AW 已在 completion 侧握手准备，不把 DDR 观察到的 AW 周期当作唯一准备时刻。
3. 当前 AR 的源偏移与该 AW 的目的偏移、长度、size 完全匹配，范围属于真实 normal descriptor。
4. 一旦 AR 首次 VALID，运行期 `held_q` 保持至握手，不因条件变化撤回 VALID；手写断言核字段保持。

每流使用 8 槽目的 AW ring 和宽序号，支持这次 32 KiB 错位任务；不是旧 test 门的 4 KiB 总量约束。输入仅为正常、静态有效映射、64 bit INCR、每流一个 active job、原 AXI ID0 的两个真实 DMA。没有宣称任意任务/故障/取消/epoch 下无死锁。

只新构建一个 SV+C++ 模型，并对上一轮 `obj-full-buffer` 用同一个 CONF0 驱动重链接 C++ 一次；两项构建都 exit0。新运行只有四项，全部 PASS。没有再跑旧两项失败/旧回归、没有编 BIT 或上板。生产 banked controller、§31 写容量修正副本、单序列 DDR 模型保持；没有 `TEST_COMPLETION_GATE`、逆读、AW 交错、header park 或额外返回优先。

## 同一工作量的结果

两流各 32768 B，源页偏移0、目的页偏移3072，CONF0共同分段；每路32个源AR、32个目的AW，总64AR/64AW、各路4096拍读/写。两个模型使用相同驱动和输入参数，区别是 FIFO 容量和源读门。源读门通过正常反馈也会改变生产者的 AW 出现顺序，没有强制固定或改排 AW。

| 模型 | 两路 iDMA 数据 FIFO 总量 | V先 / A先 | 最后 DDR W 周期 | 首虚拟源 AR | 首 DDR 源 AR |
|---|---:|---|---:|---:|---:|
| 普通准确源读门，BUF3 | 48 B | PASS / PASS | 27894 / 27894 | 1032 / 1032 | 2880 / 2880 |
| 普通无门，BUF512 | 8192 B | PASS / PASS | 25146 / 25146 | 68 / 68 | 1934 / 1934 |

48/8192 B 只统计两路 iDMA 的 byte FIFO，不含公共 TC W FIFO、AXI cuts、descriptor、AW ring/计数与物理 owner 队列。小缓冲门还增加元数据和控制，不能称总资源减少到48 B，也没有 LUT、BRAM 或综合面积结论。

27894−25146=2748 是共同复位基准下的**最后 DDR 写拍时刻差**，不是单独计时的 copy 完成窗口、V/A 平均吞吐或稳态加速比。两种启动顺序只是两个确定性输入，不是统计置信区间。

四项均通过完整目标/源/未写头尾逐字检查、真实 NEXT_ID→DONE 匹配、终止 quiet/队列排空，虚拟 W 归属和拍数无错。各路 payload R 反压为0；总208周期反压都在 other/DS 区域。源读门各放行64次，累计阻止量55046是每拍被挡流数之和，可以超过运行周期，不能当平均占用或实际等待总时长。

独立从两份门控轨迹重建虚拟 W 队列，对128次真实虚拟 AR核：当前 W owner匹配、当前写段未开始接收数据、源/目的偏移匹配、ARLEN=AWLEN=127。[逐次审计与行号](../out/write-owner-baseline-20261008/gate-admission-audit.json)。首次虚拟 AR=1032而DDR AW=1033是管线差：本门用较早的 completion AW握手，不能误判为未准备就放行。

[四项退出码与原始结果](../out/write-owner-baseline-20261008/runs.json)／[构建、哈希与阶段清点](../out/write-owner-baseline-20261008/stage-evidence.json)。§31 的两个 CONF0 小缓冲无门失败仅复用旧证据，没有重复运行。

## 对创新路线的判断

这轮已排除“必须用复杂新分段才能以小数据 FIFO 完成”的主张。普通准确整段门可以完成；它将读准入推迟到写准备后，轨迹确实显示更晚的源 AR 和写终点。但 AW 顺序、排队和缓存访问时序也随之改变，**不能把全部2748周期差独立归因为翻译等待**。

下一仍性能/成本主线：**保留较早的逻辑读译址，把实际数据服务切到可以按当前写序消费的范围，是否能用较少总资源接近大缓冲？** 先实现强普通固定后译址分段＋同一 owner 调度，才谈动态候选 I。若固定规则就做到同效同成本，动态规则不作独立主贡献。

已有几何提示但不是新机制结果：§31大缓冲CONF1同字节任务总32源AR+34目的AW=66个虚拟Ax，本轮CONF0为64+64=128。后译址分片若保留原逻辑请求，可能避免为每个物理子片再走源翻译；这只是实现目标，不能由请求数直接推算 miss、PTE次数或净性能。

复核 [AXI-REALM 原文 §III-A1](https://arxiv.org/html/2501.10161v1)：已支持可配粒度拆分、保留原事务元数据和读 RLAST 重建。**透明分片、改变位置或可调粒度本身都不作新贡献。** 原文还限制某些 atomic/non-modifiable 事务；下一 PoC 必须先核实际 AR flags与可分片条件，原样保留属性、同ID顺序和原RLAST。本文没有从网页“未出现IOMMU”推断无人做过本路线。

## 下一 PoC 的具体约束

- 用 CONF1 保留原大逻辑源请求，译址成功后才发物理 child。先固定1KiB，与动态最长合法前缀相比；两者使用相同 descriptor、parent槽、owner选择、公共缓冲和错误合同。
- 部分 W 段可能由两个 parent 的不同片拼成，不能再只比较源ARLEN与整段AWLEN；要核每拍目的消费偏移与已提交义务。
- 一个 parent 未完成时须能服务另一 ID，不能仅切片仍占唯一逻辑槽。每ID只能先处理最老parent；原RLAST仅全parent数据完成时给出。
- 多个同owner头可先填槽，上游TC/FIFO可能挡必要owner；须用真正可达队列证明暂存/旁路容量，不能宣称2槽足够。先保守实现，再同资源对普通每ID队列/旁路。
- 成本包含全部parent/目的关联、环形队列、顺序与版本状态；静态正常任务与故障/失效扩展分开。先验收数据/协议，再核同负载净收益与面积，最后才考虑实际四DMA/两bank和FPGA。

新I、固定后译址分片和通用证明本轮均未实现；论文创新与分区水平尚未成立。新场景仍不能解释旧4KiB/CONF0的PN退化。

## 分工、修订与生产边界

DeepSeek完成旧接口取证、普通准备脚本及聚焦修订；Codex亲自提出并实现关键准入RTL、核mux/握手/字节关联、执行构建运行并审定。两次成功 thinking/high/max393216完整返回共343532 provider tokens。没有本轮Claude或新增Codex子agent。

Codex直接核源后纠正worker两处推断：旧 `can_drain` 还有严格偏移/len/size，不是只看owner；旧 `source_read_release_q` 的327–328行更新实际在reset分支，不能按worker说法认作运行期有效sticky保持。本轮另写门的held更新明确在运行分支。该历史test模块在本轮Mode0不例化，未改其源码，也未据此声称旧运行都无效。[核查记录](../out/write-owner-baseline-20261008/codex-review.md)。

76个实际编译原生产SV实验前后哈希相同。所有新增RTL、驱动派生和命令均在out；生产/tests源、Linux、BIT、SD、UART/JTAG/板卡均未改。测试仅两真实DMA的同bank重分类，非生产四路512/3混合配置、非MIG证据。
