# §41 排空成本边界（2026-10-08）

结果先行：本轮实际执行数学程序 1 次，exit 0；没有 DUT/BIT 构建或仿真、板卡/SD 操作，也没有改生产 RTL、Linux 或测试。数学只给出有限分类，不是硬件验证，也不能替代物理协议与排空证明。两个必要纠正成立：B>0 时一个 source cut 能支持多个 destination 停止点；H 只界相邻安全目的检查点的间距，不是 budget-off 后的真实工作量或周期上界。

## 实际执行与数学结果

数学程序 `geometry_oracle.py` 经一次运行输出 `geometry-oracle-result.json`。全网格 N=1..5 为 7585 分类，任意合法 A、N=1..4 为 3805 分类，合计 11390 分类；两组有部分重复覆盖，不是 11390 个 DUT 试验。greedy/brute mismatch=0，未发现实现错误；有限穷举支持一般交换证明，但不替代物理协议证明。

反例 N=10、R0={0,10}、D={0,4,6,10}、B=2、H=4 中，新增 cut 6 同时支持 q=4 和 q=6，真实最少 1，旧逐目的节点收费模型算 2。因为旧模型把每个 q 的覆盖独立计费，所以 B>0 不能再沿用该 DAG。H 只是安全目的点间距；因为实际还含首次 offer、mux owner 提前 push、held requests、两个 W 前缀与译址/服务等待，所以不能把 H 当排空时间。

## 32KiB 静态表与软件 tiling

| 条件（每 DMA） | source physical requests | destination bursts | 说明 |
| --- | --- | --- | --- |
| B=0/24, H=2KiB | 32 | 17 | 需 16 cuts |
| B=0/24, H=4KiB | 24 | 17 | 8 cuts 达静态下界 |
| B=0/24, H=32KiB | 16 | 17 | 0 cuts |
| B=1024, H=2/4/32KiB | 16 | 17 | 三档 H 均不用新增 cuts |

B=24 只是假设的数学停车容量；实际名义是 24B/DMA、两路合计 48B，不是总系统 buffer。TC W 128B/bank 是 shared 目的路径，不能自动加成 private B。普通软件 tiling 为 3KiB+7×4KiB+1KiB，共 9 个 subjob；理想 source=24、AW=17，8 cuts 达到该静态几何下界。但这组没有运行，也没有测 TL/PTW、CPU 供给、DONE 或排空成本；因此不声称同 TL、同 cost、同 interface。根核到 `ErrorCap=NO_ERROR_HANDLING`，且 fullbuffer certificate 只观察 job fire/done、容量与对齐资格，不逐 R 预约完整空间；`out/natural-write-order-20261008/trans_ctl_write_limit.sv` 只是 out 中 C0 修正副本，不能写成生产已修正。

## 根新增源码审定：§39 准入门与真实剩余问题

根核到 §39 `compact_pretl_owner_fragment.sv:87–123`：每个 source child 首次 offer 之前，capacity 由两层匹配且物理已准备的连续 W prefix 准确门界定。因此当前 child 不会因“未来目的 AW 还未准备”而准入；这版不能使用纸面未准备 AW 反例当新 bug。实际剩余问题改为预算暂停时已有较远 W-owner 债务需要更多 source child 或同 job raw parent 接收；这只是待验证假设，尚未运行。`multi_actual_dma_top.sv:610–622` 把两 DMA raw AR 移出共享 mux，按 perflow credit 捕获，保留 parent 能力；该能力限 out 原型，不是生产通用保证。

真实义务必须含首次 offer、mux owner 在 AW 握手前已 push、held requests、以及两个 W FIFO 中排在所需 W 之前的前缀。整 transaction 图会把 2beat R→2beat W、FIFO 仅 1beat、AW 可服务的正常流式进度误判成环；必须保留 beat 或可消费 prefix milestone。

## 下一判别与受限单流 fixedpoint

[下一判别规格](../out/fragment-drain-boundary-20261008/codex-next-decision.md) 已真实存在。受限单 flow 取整 fixedpoint 只是普通守恒，不是新算法名称或创新证据。在源端点每2KiB、目的端点1/3/5…KiB、已不可撤回源前沿2KiB、目的前沿1KiB、最终独占停车B24的纸面例中，不加源cut需到32KiB末端；允许未来cut3KiB则可到3KiB。必须保持独立目的译址/服务前提，未证此状态在 DUT 可达；前沿不是新增 byte 或 cycles。不能说新 cut 救实际当前 AR，也不能说已证明论文 gap。

## 强对照与创新表述

强对照至少包括：普通内部 nonuniform splitter；同 B 的短 child 完整 R/完整 W 预约；软件 tiling；仅停 source 但允许有限 destination 续行；AXI-REALM 透明 child+Wbuffer；AMD PG313 的选择性停 source。因为普通端点依赖、beat 级图、选择性停 source 与保守 drain 界已有 2026 Preemptive VC、2026 AXI TMU、AMD PG313、DMA350 等覆盖，所以不能作为独立贡献。如果整理后只是普通完整 R 预约、区间覆盖或依赖图闭包的重述，就不能立为主创新；如果能在同资源/同释放点给出成功可恢复 copy 的紧 continuation work/cost 界，并证明真实多 DMA 可达性，才可以发展分析贡献；如果机制在同资源强对照下达到普通方案做不到的点，才可以发展机制贡献。

## 初审、文献与归属

两名研究席的两轮审核均已完成，全文已保存，根已直接核关键原文、实际源码及两份最终分析；这不等于硬件验收。文献 map 仅用两份 2026 稿、RTAS2025、AMD PG313 做定向补证，尚未并入原 123 计数，不声称完成全域 novelty 检索或论文 gap。

## 导航

- [codex-critical-acceptance.md](../out/fragment-drain-boundary-20261008/codex-critical-acceptance.md)
- [codex-model-contract.md](../out/fragment-drain-boundary-20261008/codex-model-contract.md)
- [geometry_oracle.py](../out/fragment-drain-boundary-20261008/geometry_oracle.py) / [geometry-oracle-result.json](../out/fragment-drain-boundary-20261008/geometry-oracle-result.json)
- [rules-proof-agent.md](../out/fragment-drain-boundary-20261008/rules-proof-agent.md)、[map-boundary-agent.md](../out/fragment-drain-boundary-20261008/map-boundary-agent.md)、[release-composition-prior-agent.md](../out/fragment-drain-boundary-20261008/release-composition-prior-agent.md)、[release-closure-proof-agent.md](../out/fragment-drain-boundary-20261008/release-closure-proof-agent.md)
- [codex-next-decision.md](../out/fragment-drain-boundary-20261008/codex-next-decision.md)
- 主源：[Preemptive VC 2026](https://arxiv.org/html/2607.01430v1)、[AXI TMU 2026](https://arxiv.org/html/2608.30435v1)、[AMD PG313](https://docs.amd.com/r/en-US/pg313-network-on-chip/NoC-Deadlock-Avoidance)、[RTAS2025](https://retis.sssup.it/~a.biondi/papers/RTAS25.pdf)

## 假设/未验证

- B=24/1024 仅为静态假设停车容量，未识别为实际可独立停车容量。
- H 不是周期/成本界；静态计数不是运行请求数。
- 软件 tiling 未运行；C0 修正只在 out；无 production、Linux 或 tests 改动。
- §39 较远 W-owner 债务是否实际可达/最小成本未运行；top 610–622 为 out 原型，不是生产通用保证。

归属：DeepSeek整理证据、提出脚本与文档草案；Codex设计、核源、实际执行并审定；未调用Claude。
