# 论文平台当前状态（2026-10-09，§85）

> Stage85 当前页（2026-10-09）。旧历史见归档入口；本页仅本轮当前、实际scope、下一判别题与最新actualDUT。

## 当前

- 流程：AGENTS.md“贡献筛选与记录流程”六规则已执行；稳定接受后一轮末一次普通文档；旧CURRENT 765行167734字节原样归档，SHA记录见本轮证据目录的status-archive-record.json。
- 历史归档：[状态历史全文](iommu-platform/docs/status-history-through-stage84-20261009.md)。
- 已核准入边界、关键前作和四个完整对照。保留安全准入能力；性能机制进入重设计，当前不进入RTL。
- 机制正向：绝对连续WRITE前沿Fi与每个目的端独占Qi联合约束 e<=min(Fi+Qi)；已有未授权积压计入Qi；最大安全段不等于最快。
- 许可：提前结果必须有actual prepResp handshake并进入已计费owned slot，可由trusted TC查到且旧区域保留至真B；READY0的TL VALID不等于已取得许可。普通one-sponsor与缓存复用允许。
- 本轮四控制为hand推导，不是真实cycles：长读ACK21/23 AR1；强split20/22 AR2；one-full sponsor22/24 AR1；all-full23/25 AR1。共同R8/W16、每flow3AWB、null0、尾leaf PTW2，own总准备6。
- 成本门：J_long-J_split=-e_AR+lambda_A+lambda_B+(k_long-k_split)；手例AR成本3时长读47对分片48，成本1时45对44；权重为解释性目标，非测量或论文性能。
- 决策：撤销“长读必快/全面领先”的解释；不得用弱split或要求所有目的fullprep制造优势。

## 实际scope

- 本轮无新model/RTL/Linux/build/上板；stage84模型仅引用不重跑；无新production RTL。
- 证据：[本轮接受](iommu-platform/out/innovation-frontier-boundary-20261009/root-result-acceptance.json)；[准入设计](iommu-platform/out/innovation-frontier-boundary-20261009/root-maximal-frontier-design.json)；[完整成本核对](iommu-platform/out/innovation-frontier-boundary-20261009/root-hand-accounting-review.json)；[前作卡](iommu-platform/out/innovation-frontier-boundary-20261009/stage85-c-exact-prior.json)；[关键原文与源码](iommu-platform/out/innovation-frontier-boundary-20261009/root-primary-review.json)；[调用账本](iommu-platform/out/innovation-frontier-boundary-20261009/worker-record.json)。
- 创新判断：共享读、消费标记和提前译址已有前作；如果联合准入能在同预算下增加有用能力或全成本收益，那个差异才可以成为贡献。

## 最新actualDUT

- §76–78：6成功build步骤、29实际完整copy PASS；true late join实现通过，但五组强普通许可齐的起点共享均更快。
- 链接：[最近真实DUT接受](iommu-platform/out/shared-read-inflight-join-20261009/codex-result-acceptance.md)。

## 下一判别题

- 真实握手/结果所有权合同下，长读越过WRITE前沿省去一次AR的成本，能否超过推迟tail PTW及全部ACK的代价；单目的fullprep/native复用能否复现。
- 先确定一项实际source AR setup服务成本与prepare grant可用时序，给完整方法同一初始可用信息/旧B生命周期；只选一个能力正例和相邻反例。
- 只有差异成立且真实接口闭合才进入RTL；若被普通方法完全复现，归档该性能主张并重设计。

## 归属与记录

- root提出绝对前沿规则、核关键primary/source、审四个完整控制并最终接受；DeepSeek流程证据与轮末文档proposal；Codex实际应用；Claude调用0。
- 无未经测量的节省声明；实测基线见root-result-acceptance.json，未来节省未测。
