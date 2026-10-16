# shared-read RTL 进展 2026-10-09

## 一句话
最终 build06 的 12 个完整 copy（run14–21, 24–27）通过验收；旧版本 7 个 PASS、6 个失败、2 个参数未知排除均保留。

## 验收范围
真实 iDMA/IOMMU RTL 仿真，尚未上板。每次 reset 后 V、A 各做一次正常 2KiB 拷贝；同 bank、单页、静态成功许可。两 DMA 各 issue/done 一次、各返回 2048B、两目标各写 256 拍；源/目标未拷贝区域逐字节检查。

## 实际数字
下表是每路真实硬件 issue 到 done 的 cycles，包含冷启动翻译。每个条件一次确定性运行，不是多次统计平均。

| 场景 | 候选(mode1) V/A cycles | q2对照(mode2) V/A cycles | 耗时降低 |
|---|---|---|---|
| 无暂停 latency31 | 5657/5650 | 8579/8572 | 平均减 34.07%（约34.1%） |
| 无暂停 latency100 | 6692/6685 | 18377/18370 | 平均减 63.6% |
| 暂停10000 cut64 | 3438/13450 | 3571/13496 | V 耗时降低 3.72%，A 0.34% |
| early resume B64+背压 | 3763/4293 | 3802/4332 | V 耗时降低 1.03%，A 0.90% |
| long pause B64+背压 | 3763/13712 | 3802/13712 | V 耗时降低 1.03%，A 持平 |
| closed mode0 | 15635/15628 | — | tile 是必要较强对照 |

物理 AR：mode1 1 burst vs mode2 128 burst，payload 均 256 拍。暂停 cut64 时 R=512-8=504。early resume run20/21 实际 3181 恢复、3254 detach；long pause run24/25 13171 恢复。

## 失败保留
run01 bad ID guard root bug（exit -6）；run02 WRITE walk 与 held shared R 死锁；run08/09 提前 resume AW 抢在 V 前死锁；run12/13 字节通过但严格 observer 漏计同周期 AW/W（exit 1）；run22/23 exit0 但未知 +ptw-lat，实际 latency31，排除延迟对比。

## 成本边界
最终 iDMA BufferDepth 3/flow，payload 24B×2=48；新 skid 8B，合计 56B 仅三个位置。XBAR/cuts、TC W FIFO、原 parent/recipe/header、控制和观察 state 未计；无综合 LUT/FF/timing，不称全系统或同面积最优。

## 下一门
先强化普通可变分片/有限先读控制，并列全资源/暂停义务预算；再扩多页 READ/WRITE 成功前沿：物理 read 不得越过尚未授权 destination，resume 守写序。比较双流平均/最坏时间、pause 债务、重读量与状态成本，随后决定生产整合、综合和上板。

## 贡献边界与前作

[DMA-Latte v3](https://arxiv.org/html/2511.06605v3) §IV-B/VI 已有同源识别与一次读多写；[Multicast AXI XBAR](https://arxiv.org/html/2502.19215v1) §II-A 已有有限缓存下的 W 顺序死锁与 acquire/commit。因此不能把共享读、一般死锁修正或34%直接称为独立创新。如果在同信息、同总成本、同暂停义务下，更强普通分片控制仍不能同时获得此进度与效率，且前作没有覆盖具体控制，就可以论证独立微架构贡献；如果普通控制同效同成本，就不能将当前实现命名为新算法。

本轮两项必要控制是：共享读前取得两个 context 各自的 source READ 和 destination WRITE 成功 recipe；A 提前恢复时，V 原共享读的 logical RLAST 退休前不许 A 新 suffix AW 抢到前面。两个方案同用这些控制。只验收 A 一次暂停/恢复、V 继续的情形：退出后 suffix W 扩成长段，第二次暂停不能沿用 q 债务界。连续作业、所有成员同时暂停、跨页、fault/invalidation、device/ATS/atomic 不在本次验收范围。

DeepSeek 整理页表/测试/观察器提案与报告草稿；Codex 亲自设计 RTL、诊断两种依赖环、审读关键源、应用与执行验收。

[根级完整判断](../out/shared-read-rtl-20261009/codex-result-acceptance.md)／[执行清单与 SHA](../out/shared-read-rtl-20261009/execution-ledger.json)／[原始文件入口](../out/shared-read-rtl-20261009/README.md)／[声明状态审计](../out/shared-read-rtl-20261009/state-audit.md)。
