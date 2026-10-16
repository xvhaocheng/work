# 机制设计交接稿 round05（stage 92，2026年10月9日）

## 1. 本轮结论与暂停

本轮完成一次有界创新判断：得到具体复现边界，但没有形成新的已支持独立贡献。按本轮开始时明确说明的理解：如果没有新的独立贡献证据，就按用户条件先暂停；保存记录后结束本轮探索。该暂停是本轮有界终点，不是整个硬件或研究领域无法推进；保留假设与可证伪重启条件。[round05/root-result-acceptance.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round05/root-result-acceptance.json) 为唯一裁决权威，两份控制分析提供证据。实际执行：模型/RTL/Linux/构建/上板/Claude 均 0，仅定向源码/原始文献阅读、手推和 1 个关键 Codex 席。

## 2. L-M：静态 {X,Y}/{Y} 主例被普通 early-bind+pin 复现

对同一原始任务起点：V 接受 {X,Y}、A 接受 {Y}，X/Y 为真实 hot FREE 输出槽，父请求合法同时在场，源地址池外，输出租约共存。普通早分配在源 READ 前固定 A->Y、V->X，短 pin 保护实际 resident 行，一直保留到成功的正常 WRITE 翻译被稳定物理请求头实际接收；源填充时做完整权限/原 owner 检查，普通 PLRU 在 allocation_mask 减去 protected rows 中选择，allowed 非输出行为空时走同一已付费 checked no-fill 路径，把成功检查结果交给原源请求而不共享填充。随后真实源 AR/R、真实 WRITE 翻译权限检查、AW/W/B、完成/缓存同步/消费者释放全部支付。该样本中 E16 实际 victim mask 与 L-M 保秩 mask 相同，双方都不产生被指控的额外目的 walk；早前 M20-delta3=17 相对此强对照不构成收益。late-choice 的实际晚语义轨迹尚未证明，动态释放/到达/重复/多上下文/未来源重用也没有等价证明。普通 E-P 是独立静态强对照，是该静态样本的构建可执行方案和手推，不是已运行模型，也不是对全部动态场景的等价证明。不要把完全复制新规则的 set-aware witness-pin 实现视为已发表覆盖；它是备选实现/对照，不是自动发表覆盖或自动反证。

## 3. L-W：三次 PTE 可由有界 checkpoint 取消 + 一条虚拟前缀 partial-translation cache 复现

L-W 正向手例的 root+middle+leafY=3 次 PTE，可由完整固定请求控制复现：可选 prepareX 在已校验共同 2MiB 前缀内到 middle 后有界 checkpoint，发布一条虚拟前缀索引 partial-translation cache（prefix/context/epoch -> 叶表 PPN），在下一 leaf AR 首次 VALID 前取消可选 X，随后普通晚分配固定 Y，PWC 精确命中后只需 leafY。一条物理索引 PTE 缓存不能正确跳过两层；该控制是按 Barr ISCA2010 的虚拟前缀 partial translation 组织。L-W 的 Y 本身是 cold leaf，真实固定 Y WRITE miss 就是更高优先级需求，即使没有竞争 A 到达，也可按 US9542332B2 已发表的需求优先规则中断低优先级 prepare；不要求集合特定的跨地址取消见证。但持有点、取消/排空已发 AR、reissue、PWC 查找/失效/驻留状态都不是免费；完整面积、重启延迟、Fmax 未知，163-bit 示例字段子集不是净节省，不能写成净优势。

## 4. 三类判断要分开

规则新颖性：pin/cancel/PWC、matching、early binding、demand-priority 等组件已有前作支持；精确 set-valued COPY 目标集成仍是假设，未证明已发表，也未证明全领域空白。独有收益：当前静态 L-M 与当前 L-W 手例的节省都被独立可执行强对照复现，因此不能据此证明独立更快/更省算法。完整工程成本：无完整面积/Fmax/实测周期/双边状态端口台账；306/319/466/479/163 等命名 bit 子计只是示例，不是净面积，不能加总成优势。因果结论保持条件式：只有在真实原始 COPY/consumer trace 产生合法未解决晚选择，且完整付费比较下独立强对照同预算不能复现有用联合 V/A 或成本前沿，并且最近前作未覆盖该动作/合同时，集成机制才可能形成独立贡献。

## 5. 两条可证伪重启条件

1) L-M：真实原始 COPY/consumer 轨迹需要合法未解决晚选择，并有完整付费的 early binding/pin、普通准备、合法原生容量/布局对照；不得发明基线延迟、强迫两路 victim，或要求需求先打败“整个候选规则编译成另一实现”。2) L-W：两份最小完整状态/端口/协议台账显示 live prefix continuation 与付费 cancel+virtual-prefix cache 之间存在不可避免的有用面积/延迟差；须执行 firstVALID、权限、返回所有权和真实 W/B。若出现新具体事实或用户明确恢复，仅重审受影响声明；暂停时不自动继续广泛机制、模型、RTL 或编译。

## 6. 归属、范围与证据

本稿为 DeepSeek 起草的普通中文记录，未执行命令、未修改文件、未运行测试；Codex root 亲自设计、关键核查与验收并保存，1 个关键 Codex 席。证据：[裁决](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round05/root-result-acceptance.json)、[L-M 控制](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round05/root-assignment-pin-review.json)、[L-W 控制](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round05/prefix-control-critical.json)、[round04 机制卡](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round04/root-frontier-design.json)。前作组件范围：[ICCD 2018，DOI 10.1109/ICCD.2018.00052](https://arxiv.org/pdf/1808.09751)（锁翻译后再编程 DMA、并发成本；支持 pin 组件旧，不覆盖精确 set-valued 合同）；[Barr 等，ISCA 2010，DOI 10.1145/1815961.1815970](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)（虚拟前缀部分翻译跳多级）；[US9542332B2](https://patents.google.com/patent/US9542332B2/en)（高优先级需求中断低优先级准备）。以上只支持组件/边界，不宣称全文合同已覆盖。
