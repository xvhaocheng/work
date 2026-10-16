# 论文平台当前状态（2026-10-09，§87）

## 当前结论

- 统一载荷存储 9→6 只对重复 payload baseline：独立 history 3 字 + 两个 FIFO 各 3 字共 9；新候选和同资源普通缓存均为 6，所以 9→6 不能证明独有，端口/metadata 收费另算。
- 弹性等待许可前缀 p5/R11 对固定 p3/R13 同为 trueW16，但同池基线可完整复现；这些是手推读数，不是新仿真 cycles。
- 许可组合 A/C 共享 3 份数据、各缺许可：单项 release 0；两项许可完成并由真实写出退休后可 free 3；B/D 各 free 1，共 free 2。所选单项/组合调度手例分别被 stage-aware 完成优先、“占用量+伙伴延续”完整复现，不报已领先。
- 动态跨 SID 许可依赖规则保留为候选，不称已验证独立贡献。因为共享载荷、引用退休、泛用分组已有前作，且具体控制已复现当前收益，不能把这些原语/手例算独立创新；如果真实可达动态跨 SID 组合取得同预算强对照无法复现的有用收益，并完整计入成本，且准确前作未覆盖，就可作为独立机制贡献。
- 真实接口仅 2 个待许可生命周期；4 prepare 须按已计费的准备队列与结果归属、access_ok、firstVALID 不可换、真实 Wbuffer 计价。推导中的共享关系还需证明能由真实 DMA 请求产生，并计入全部请求和缓冲成本；未证可达不能当实验。
- 主线仍为多 DMA 共享 IOMMU；候选设计不称生产完成。

## 范围

- 本轮模型/RTL/编译/上板均为 0；无生产实现、无新测试通过声明。

## 最新实际 DUT

- 既有 §76–78：6 个成功构建步骤、29 次实际完整拷贝通过；[DUT 入口](iommu-platform/out/shared-read-inflight-join-20261009/codex-result-acceptance.md)；本轮仅引用。

## 下一决定题

- 从原始 workload 起点，同价比较全许可预准备/WRITE lookahead、occupancy+partner、stage-aware group，找一个决定场景；无差异不进入 RTL。

## 证据入口

- [中文说明](iommu-platform/docs/research-permission-release-20261009.md)；[接受](iommu-platform/out/innovation-unified-cost-20261009/root-result-acceptance.json)；[手推核对](iommu-platform/out/innovation-unified-cost-20261009/root-hand-review.json)；[许可组合设计](iommu-platform/out/innovation-unified-cost-20261009/root-permission-coalition-design.json)；[完整强对照](iommu-platform/out/innovation-unified-cost-20261009/b-permission-release-design.json)；[真实接口边界](iommu-platform/out/innovation-unified-cost-20261009/c-permission-scheduling-boundary.json)。
- 历史归档：[§86](iommu-platform/docs/status-stage86-20261009.md)；阶段交接见 [HANDOFF.md](HANDOFF.md)。

- [交给同工作区高级模型的提示词](iommu-platform/out/higher-model-prompt-20261009/prompt-workspace.txt)：完整研究要求、阅读顺序、源码与证据绝对路径、成果保存位置。

- 归属：DeepSeek 源码/成本证据与文档修订；Codex root 机制/关键原文/源码核对与接受；Claude 0。
