# 当前状态 stage 91 2026年10月9日

- 范围与执行：接续 round03，仅机制设计、定向前作阅读与手算；本轮 0 模型/0 RTL/0 Linux/0 构建/0 上板/0 Claude，root 亲设＋2 关键 Codex 席，DeepSeek 起草中文，Codex root 审核保存。
- 主推 L-M：源填充时保持未解决合法热输出义务的最大同时分配容量；无保秩普通 victim 时走完整权限检查，把源 READ 请求经正常权限检查得到的地址翻译结果（PTE/翻译响应）直接交付原请求，不是源 payload，也不占用共享 IOTLB。
- 关键例：V 接受 {X,Y}、A 接受 {Y}，驱逐 X 后各自本地命中仍为 1，但 cU=1、秩 2→1；局部计数漏掉容量损失。
- 准入：重叠子集下必须完整 FREE 图匹配准入，标量 free credit 不够；精确元数据与完整权限路径都要付费。
- BIND：转移到固定拥有的单元素；普通输出绑定后直到 WRITE 翻译被稳定的物理请求头实际接收前必须保持，FREE→COPY 时清除保护是错的。
- 备选 L-W：不可变 set-valued WRITE 目标，在已校验共享前缀内、下一 PTE AR 首次 VALID 前选叶子。
- 取消：只作组件/对照，跨地址真实租约撤回可停在未发表 PTE AR 前的可选孤儿后缀。
- 旧组件不单独新：Matching/Hall、bypass、pin/cancel/PWC、hot-first、死页预测、shadow cache、延迟放置、通用 output-pool API。
- 等价：整套新规则等价实现不是自动发表覆盖，也不是自动反证，须用真实可执行强对照比较。
- 条件手算：19/51 与 22/54 只在带共同已付费前缀的条件后缀排程下有效；M20−delta3=17 只是边界手算，不是实测或原始起点胜利。
- 字段小计：319bit/40B；加 160bit 索引时 319+160=479bit/60B，479 包含 319 及其新增 160bit，319 与 479 不能再次相加；均为命名字段示例，不是净面积。
- 强对照：精确 set-aware pin+bypass、全 WRITE prepare 保留、无条件源 bypass、local k-choice 消融、更大 IOTLB 容量/合法重划分、超页与页局部池布局。
- 源码入口：[rv_iommu_tl_wrap.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv)、[rv_iommu_iotlb.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv)、[rv_iommu_ptw.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv)。
- 下一题与推进停止：先合法原始轨迹/契约/完整成本，再小事件模型，具体差异在独立强对照下存活才升 RTL；若源 prepare+普通分配/pin 或 cancel+PWC 在正区域等/低完整成本匹配，停掉单独性能算法声明。
- 七组场景：M1 重叠热例、M2 第三热槽/去义务、M3 BIND→WRITE 间隙、M4 16 项容量与等预算、W1 前缀内晚叶、W2 已发出 AR 后释放、C1 A 早到/晚到取消；M1 必须允许普通控制立刻合法早 BIND/提前 WRITE 准备，不能拖延对照，真实源完成、头容量背压或晚语义约束须从原始轨迹产生，若早绑定等低成本控制消除全部重叠就停单独 matching 准入收益。
- 权威入口：[验收裁决](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round04/root-result-acceptance.json)、[机制卡](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round04/root-frontier-design.json)、[前作](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round04/root-primary-review.json)、[源码](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round04/root-source-review.json)。
- 交接稿入口：[round04 中文交接稿](/home/xvhaocheng/work/iommu-platform/docs/mechanism-design-handoff-20261009-round04.md)。
- 历史与归属：stage90 原文已由 root 原样归档为 [docs/status-stage90-20261009.md](/home/xvhaocheng/work/iommu-platform/docs/status-stage90-20261009.md)；Codex root 亲自设计/关键核查与验收＋2 关键设计席，DeepSeek 起草中文，Codex root 审核保存。
