# §42 后下一创新判别：两W序与有限续行的真实边界

根 Codex，2026-10-08。§42已run：普通控制能只读写A7KiB而释放共享通路，不必完成32KiB整job；V先时受不可撤W前驱约束，还必须返回V8KiB数据。该有限single-queue前驱界是普通守恒，不能单独立独立机制；完整结果见codex-result-acceptance.md。以下尚未build/run，不是BIT指令。

## 一个明确问题

当虚拟和物理两层不可撤W顺序不同、多个DMA的payload容量很小，而且必要source译址/返回也占共享路径时，是否存在**普通逐队列排空遗漏、但可准确证明的最小成功让路工作或最低停车容量**？先查真实可达，再核前作增量。不能为当前同bank/已prepared child伪造未准备AW或顺序失配。

## 最小下一步与停止条件

1. 定向核现有双bank实际AW仲裁、两W锁与TC FIFO：能否形成合法不同顺序？不能用强制owner顺序、改AW合法语义、error/discard或永久停memory制造例子。复用现有natural run与§42门/接口；若参数变更使owner计数含义变化，先修接口，不拿错误quiet为结果。
2. 对一个可达状态，列首次VALID/push、每flow前沿、已offer source child及未返回拍、两W有序前驱、真实可停车的私有/TC容量及pending PTE路径。source transaction必须按可消费拍处理，不用whole-transaction graph伪报cycle。释放不要求private父header/job退休。
3. 尝试必要/充分界：普通相同顺序只能化为FIFO前驱和已offer读端点守恒；不同顺序可能需要停车或再次准入目的AW。普通取整固定点、闭包/拓扑搜索本身不作为新算法。必须证明每项成本不可避免并给可执行达到界的构造，或给真正不可行证明。
4. 强普通对照至少含：保全W顺序的物理AW准入、普通短burst/非均匀splitter、同B完整child预约、有限需求读继续、software tiling。总payload、metadata、映射保证和释放点一致；只能使用实际容量，不把TC共享FIFO冒充独立private B。
5. 先一项真实状态/一项强普通对照作判别，不扩cuts或吞吐扫描，不重复§42四run。若合法状态不存在，或命题等价普通守恒且普通同成本达到，停止本枝主创新。如果有前作未覆盖的精确必要/充分界且可达性成立，可以发展分析贡献；若再有同资源下可测的额外收益/成本优势，可以发展机制贡献。分区等级取决于完成的证据，当前不承诺。

## 领域边界核对对象

继续沿既有map已核primary：AXI-REALM(2501.10161v2)的未来地址准入/held保持与master服务前提；DMA350完整R/W预约及同B短burst；MMU-aware DMA2018 retry/端点metadata；2026 Preemptive VCs的端点依赖类；AXI TMU2026 error recovery与successful-copy区分；AMD PG313端点依赖/反馈。下一命题必须明确比这些多出什么，不以“搜索没找到”当独立创新。

Codex亲自负责可达性、规则推演和创新判断；DeepSeek负责证据与普通实现草案。没有授权本轮调用Claude。
