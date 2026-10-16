修订并定向补证据，不重复上一整轮，不负责代替Codex创新裁决。项目多DMA共享IOMMU，所有新候选未实现。输出中文、导航、准确file:line引用，原文事实/推断/待证分开。

上一evidence-answer.md需要以下修正：
1 socdma2018按failed burst PFN去重与重发不是payload源读→目的写依赖关系，不应凭PFN称它独有copy dependency。iDMA descriptor/src/dst/dataflow已明确copy语义，区分内部语义vs普通AXI-IOMMU外部接口，不可说iDMA没有copy依赖。
2 普通C伪代码表达D，只说明强普通方法可以构造相同规则，不是已发表C的存在证明。
3 reliable_prefix求和需与当前有效逻辑AR区间相交且连续，不能全段累加双计。credit如果逐消费归还后终止再free += reserved-unconsumed_tail会双重归还，给守恒正确的简短修订。
4 AXI-REALM有W完整后AW策略，不宜说它无drain而掩盖关键写资源承诺。

新证据附LTI2020/TAPA2023/MetaSys2022/XDMA2025全文+Codex本轮设计：
A 逐原文定位：LTI per-transaction lookaside、SPEC不能直接作真实事务、translationcompletion/tag/invalidations，是否规定跨两个不同src/dst地址的copy联合作业准入或有限prepared槽防相互占满规则？只据全文能确认的内容。
B TAPA用户控制流消除全burstbuffer、MetaSysmetadata接口、XDMA双端协作，这些直接覆盖E/F哪些宽泛主张？不能认为一般可编程能力等于特定策略前作。
C 对新的有限prepared-slot问题提出最强普通对照：K槽中每个copy需src READ+dst WRITE两个独立译址；若都先拿src、slot用尽，dst无法准备。在限制槽是译址结果pin而不是TLB容量、进入真实AXI前可取消未提交准备请求、相同DMA顺序和有限PTW/DDR最终响应前提下，whole-pair reservation、目的优先、总留1个mate槽、ready-copy queue 哪些消除该问题，资源/并行代价是什么？哪些已是普通reservation/deadlock避免，不包装创新。
D 检索不能自主浏览，只给可被Codex验证的明确最近邻题目/作者/年份/DOI/URL线索；不知道则标不确定，勿臆造。特别pair scheduling/co-admission、dependent translation preparation、banker's/escape credit、不完整translation transaction补齐，CTC/Soar/Alto/BIS等近邻是否有真实原文支持？
无需输出长文档全稿，仅必要证据、纠正段落、最多3个有确实可区分条件的研究问题/停止条件。不能说运行过代码或测试。