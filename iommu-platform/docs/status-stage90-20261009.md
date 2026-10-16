# 当前状态 stage 90 2026年10月9日

- 范围：接续 round02，仅机制设计；实现与复现交下游。
- 主推 L-F：预留空闲容量，执行真实正常源 READ，在首次 AW 前绑定目的；用事件维护 resident/free 索引。
- 新能力假设：利用源阶段内真实释放的热输出，减少普通端口探测；保持正常 READ/WRITE 权限和实际 W/B。
- 新合同：pool-parent 字段从 first VALID 稳定；既有固定地址 COPY 不能改 dst；原生 decouple_aw 不提供重绑定。
- 有界备选 L-I：同索引放回原 pre-firstVALID 选择点；R 仍备选、T 低优先级。
- 强对照：独立 source prepare+晚分配可能匹配或更快；允许高效续用和完整 native 并发，不人为加重复 walk。
- 创新条件：真实可达、完整成本下提供强对照不能复现的有用能力，且最近前作未覆盖该集成。
- 等价实现不是发表覆盖证据；hot-first、额度、局部性或软件 generation 搬迁不单独算创新。
- 小例：source 自驱逐可被精确 victim/pin 复现；61/97 是未完整排程草案，不作已成立收益。
- 成本：O98；3355bit/420B 已命名字段，对照同信任3587bit/449B；差29B非净面积；另计≥1024B软件generation、1MiB池DDR与接口/端口/CPU。
- 执行：0 模型、0 RTL、0 Linux、0 构建、0 上板、0 Claude；数字均手算/字段小计。
- 下一批并行：F1合法warm/ownership轨迹、F5合同、F6完整成本；通过后下游F2小模型，按需F3/F4。
- 首批模型场景：源自驱逐、源阶段释放、延迟释放、到达前释放/热源、全冷、竞争fill；本轮未运行。
- 权威：[验收裁决](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round03/root-result-acceptance.json)、[机制卡](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round03/root-frontier-design.json)。
- 核查：[关键源码](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round03/root-source-review.json)、[前作](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round03/root-primary-review.json)。
- 入口：[round03中文交接稿](/home/xvhaocheng/work/iommu-platform/docs/mechanism-design-handoff-20261009-round03.md)。
- 历史：[stage89原文](/home/xvhaocheng/work/iommu-platform/docs/status-stage89-20261009.md)；本轮记录见HANDOFF/validation的§90。
- 归属：Codex root亲自设计/关键核查与验收＋1关键设计席；DeepSeek证据整理/中文稿；Codex审核保存。
