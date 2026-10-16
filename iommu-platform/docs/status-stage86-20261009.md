# 论文平台当前状态（2026-10-09，§86）

> §86 当前页。只保留本轮当前、实际范围、最新实际DUT与下一判别题；旧历史不再叠加。

## 当前

- 流程：强对照先包含实际在途额度、合法字段回收与简单物理缓存，再判断是否需要新接口；本轮结论已稳定接受。
- 结论：所检手推时序未建立独立性能贡献。单child H3所有目的端仅有部分写许可手例29/33对sponsor31/33只是小手例差；同等两child强普通为27/33，可复现所选H3点；其余相邻条件持平或变慢。上述均为手推槽位，不是实际DUT cycles。
- 实际配置支持8在途；不能为保留单child正例而人为削弱可负担的两child强普通对照。
- WRITE记录数量不等于总物理存储；READ阶段字段在源AR封存/消费后可回收，源许可与上下文义务仍保留。
- 原前置许可隔离缓存方案，由更简单的IOMMU后物理前缀历史具体对照复现相同读次数；不为相同能力新增raw-adopt/prepare。该对照是架构对照，不是已发表覆盖证明。
- 保留安全准入能力；所选性能试验停止，不进入RTL。许可：提前结果仍需真实prepResp握手与已计费owned slot；READY0的TL VALID不等于已取得许可。
- 正向下一候选：root-unified-store-design.json，未知许可历史与正常DMA共用真实载荷池，权限成功后原子引用转移，保持真实原READ/W/B/error合同；手例6载荷槽对独立history3加两个FIFO3共9槽，仅载荷数，3槽64位仅省192bit，token/元数据/端口成本可能抵消；无LUT/面积测量，当前不入RTL。
- 关键新前作：SDM PACT2023 [SDM论文](https://jaewoong.org/pubs/pact23-sdm.pdf) 已重叠permission/data，评估假定推测成功；SpecTLB/InvisiSpec/SafeSpec覆盖推测/缓存复用原语，不从搜索未命中宣称首创。

## 实际scope

- 本轮无新模型/RTL/Linux/构建/上板；无新production RTL；源码与论文直接检查加手推。历史DUT只引用不重跑。
- 证据：[接受](iommu-platform/out/innovation-native-cost-20261009/root-result-acceptance.json)、[统一存储正设计](iommu-platform/out/innovation-native-cost-20261009/root-unified-store-design.json)、[物理历史对照](iommu-platform/out/innovation-native-cost-20261009/root-physical-history-control.json)、[关键前作](iommu-platform/out/innovation-native-cost-20261009/root-primary-review.json)、[源码核对](iommu-platform/out/innovation-native-cost-20261009/root-source-review.json)、[调用账本](iommu-platform/out/innovation-native-cost-20261009/worker-record.json)。
- 创新判断：只有统一存储的许可认领合同在同预算真实接口上降低成本或提供强对照缺少的有用能力，且准确最近前作未覆盖，才可把该差异作为架构贡献。

## 最新actualDUT

- §76–78：6成功build步骤、29实际完整copy PASS；true late join实现通过，但五组强普通许可齐的起点共享均更快。仅引用。
- 链接：[最近真实DUT接受](iommu-platform/out/shared-read-inflight-join-20261009/codex-result-acceptance.md)。

## 下一判别题

- 一个决定问题：统一存储相对简单共享物理cache/token的完整成本，以及独立许可认领差异是否真实存在且同预算更优。
- 只检查一个对齐的不可变冷读/前缀正例及其拒绝或容量邻例；不再扩手例矩阵。
- 若简单统一post-IOMMU缓存有相同合同与成本，则采用该架构并重设计附加规则；只有完整成本与准确最近前作门通过才进入RTL。

## 归属与记录

- DeepSeek提供源码证据与本次文档proposal；Codex亲自机制/关键源核/审查与实际应用；Claude调用0。
- 历史归档：[状态历史全文](iommu-platform/docs/status-history-through-stage84-20261009.md)；[§85当前页归档](iommu-platform/docs/status-stage85-20261009.md)，阶段交接见[HANDOFF.md](HANDOFF.md)。无未经测量节省声明，无生产实现声明。
