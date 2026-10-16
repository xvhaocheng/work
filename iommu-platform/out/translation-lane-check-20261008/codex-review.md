# Codex关键审定（2026-10-08，§29）
1. 已直接核生产peripherals的DMA mux→IOMMU→comp、DS独立延迟器，以及FPGA top最终唯一DDR目标。共享R服务≠共享payload byte池，MaxTrans8≠八份完整数据FIFO，单AXI口≠MIG固定FIFO出序。生产四源与测试两源分开。
2. 既有C++ds-priority跨不同源ID挑最老DS，只在未保持RVALID时切换，同ID顺序不改。一次首R每cycle最多一拍；队列16、准备31不改。这是行为模型对照，不是生产MIG配置验证。
3. 两项新PASS原日志已核。DS延迟是DDR AR→firstR，不是全翻译时长；A PTW-active相同也不证明所有准备轨迹相同。P/PN总耗时仍不同，只能排除这个局部排队差是唯一解释。
4. 合成CPU源一次完成后才等待并发下一笔，221/253不同是闭环背景流；不是Linux轮询、也不能从账本恒等式推出独立因果。
5. root独立G是虚拟AR首次VALID前选L，不取消已accept AXI。credit=0或p≥Lmax时与准确普通greedy相同；0<p<Lmax且有credit时更保守，不等于已获收益。p=0有安全整预约回退，worker将其写成绝对禁止不采。
6. root独立H条件保留进展资源，但未证非零保护量/独有决定。当前W-owner若有正可消费p，普通动态切短＋直通可能不需byte池；不得虚构其需要完整长R预算。
7. worker的条件反例把DS翻译返回和payload池合为同一credit，并称p_B很大但翻译未好，缺清楚的已备目的/未备源区分和资源账本。当前没有该共享池；不作为真实可达见证，不人为加入这条争用来证明创新。
8. VDC2011原文支持buffer-aware最大burst；TaPaSCo/H2RC2019原文已有多引擎R/W循环等待和完整buffer解决；Arm DMA350官方§5.8.1同样完整R预约＋完整W就绪后提交。不能把循环发现、动态burst、固定priority/VC当新贡献。TAPA/CTC关键段与worker描述相符，但各自可表达部分能力不表示发表过G完整机制。
9. routing额外patch、challenge建议直接实现G/自定5%面积阈值均未采。先给同信息/资源的真实决定或成本差别，未有见证不构建BIT。没有独立创新/二区认证。

