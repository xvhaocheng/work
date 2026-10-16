本席结论：**近期两个正例被推翻有根据，但由此判定整条路线没价值没有根据。审查曾出现门槛滑动风险，最新 round05 已作实质纠正。**

1. **L-M 淘汰的是一个静态性能正例，不是集合式 COPY 机制的一切可能。**
   `{X,Y}/{Y}` 且两个父任务都已在场时，先合法固定 A→Y、V→X，再短期保护翻译条目，确实能形成与候选相同的 victim mask，不产生那次额外目的 walk。因此 M20−delta3=17 的正例不能成立。这是完整规则的纸面构造与手推，**没有运行模型，更没有证明动态到达/释放下等价**。
   证据：[控制算法](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round05/root-assignment-pin-review.json:7)、[掩码证明及限制](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round05/root-assignment-pin-review.json:26)。
   我核过 [ICCD2018 §IV-C 原文](https://arxiv.org/pdf/1808.09751)：它描述 DMA 前锁定所需翻译、完成后解锁，并指出这限制并发；只能证明 **pin 组件有前史**，不证明 COPY_TO_SET 整体合同已发表。

2. **L-W 的 3 次 PTE 访问不独有，但更低成本的集成还没有被证伪。**
   普通控制增加适当 checkpoint，再取消 X、保留虚拟前缀翻译、发 Y，确实能得到 `root,middle,leafY`。然而 checkpoint、取消、reissue、PWC 和候选直接续传之间的延迟/面积尚未完整比较。尤其 [控制第42行](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round05/prefix-control-critical.json:42) 的“未绑定输出集合在前缀后停止”，是本项目构造的集成规则，不能直接叫作现成已发表 COPY 方案。
   [Barr ISCA2010 §3.2](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)确实支持虚拟前缀索引部分翻译跳过多层；[US9542332B2 FIG4](https://patents.google.com/patent/US9542332B2/en)确实支持高优先级需求终止低优先级预取；[US11210232B2 FIG10–11](https://patents.google.com/patent/US11210232B2/en)确实支持取消冗余 walk、缓存前缀再重放。**这些证明组件及组合思路有前史，没有证明当前集合 COPY 的完整新合同或完整成本已被覆盖。**

3. **“把整套新规则给基线，再以相同否定创新”的风险真实存在，但最新裁决没有继续犯到底。**
   [round04:63](/home/xvhaocheng/work/iommu-platform/docs/mechanism-design-handoff-20261009-round04.md:63)以“精确 set-aware pin+bypass 复现”否定独有收益，容易混淆“另一种实现”和“独立旧方法”。最新 [round05:34–39](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round05/root-assignment-pin-review.json:34)已明确：每次 fill 重新找相容匹配，只是把候选规则编译为 witness-pin，**不能要求候选击败自己**；真正否定静态样例的是独立 early-bind 对照。应保留这个纠正。
   同样，“普通 per-segment 信息能够表达规则”不足以证明已有策略做到同进度/成本，项目早在 [10月8日:99–106](/home/xvhaocheng/work/iommu-platform/docs/paper-driven-innovation-20261008.md:99)就纠正过。说明这种审查偏差反复出现过，值得在总体路线中承认。

4. **真正的路线风险是问题不断换、工作流证据没有同步跟上，而不是领域已无创新空间。**
   10月6日 A/B 尚缺选定工作流、具体规则和可推翻预测；[预审表](/home/xvhaocheng/work/iommu-platform/docs/novelty-gate-20261006.md:54)有直接记录。10月8日转向读写分段，却明言不能解释原来的 CONF=0、对齐 4KiB PN 退化，[文档:3–5](/home/xvhaocheng/work/iommu-platform/docs/paper-driven-innovation-20261008.md:3)；当前实验也未启用独立分段，[文档:47–60](/home/xvhaocheng/work/iommu-platform/docs/paper-driven-innovation-20261008.md:47)。10月9日又转成集合式输出、晚选择，但 [round05:42–45](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round05/root-assignment-pin-review.json:42)仍无真实 COPY/consumer 轨迹支持必须晚选。
   **我的判断：创新探索从“解释真实差异”滑向“为新机制寻找成立条件”，这比单次创新否决更影响效率。** 应先选定一条有应用约束的研究问题，允许低成本概念验证检验差异；不要要求进入概念验证前已经拿出完整面积/性能胜利，也不必击败所有可以想象的组合。原 [预审:7](/home/xvhaocheng/work/iommu-platform/docs/novelty-gate-20261006.md:7)本来就允许这种顺序。

范围：我直接核了指定设计稿、两份关键控制原件、10月6日预审及四项原始来源；未修改文件、未运行模型/构建，也未调用 DeepSeek。
