# 本轮前作差异的限定

复用本地作者稿`out/shared-translation-literature-20261006/papers/mask2018.{pdf,txt}`，root定向读文本866–965及1205–1258，含§5.4和§7.4。MASK已经在内存控制器中以翻译队列优先、应用数据队列及配额处理PTE/data和应用间争用；因此“联合考虑”“翻译优先”“加入配额”不是本轮新意。§7.4给其队列和统计存储组织，不能直接拿GPU队列数量当本平台净新增硬件成本。

候选若成立，具体位置在不可改内部read排序的生成MIG之前，以已拥有walk的早意图避免尚未承诺的独立载荷排到首PTE前；借此覆盖PTE之后的本地处理空档。它不复现MASK完整GPU架构、不声称MASK不能适配，也不从论文未描述本接线推出首创。要形成贡献，须证明该接口或组织在目标部署以可接受、相对具名普通方法有价值的费用提供能力。当前只检验这一条件机会，线路/时序/面积和实际收益未知。

具名普通对照包含既有current-VALID DS-first/F4、固定等待、提前准备/源序列化、原合法deburst或同能力分段；同一新增early-intent接口上的简单门控是接口能力控制，不是强迫候选击败自身的独立前作。AXI-REALM的透明分片及既有准备/优先级前史保留，不重新认领位置或通用门控公式。

本次两项网页打开（DOI和作者PDF）分别403、timeout，未取得新原件；依据是已保留作者稿及前次来源记录，不把失败访问当重新核验成功。原始返回见[网页记录](mask-web-receipt.json)，本地hash/读取范围见[亲核记录](critical-reads.json)。[MASK作者稿](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)／[DOI](https://doi.org/10.1145/3173162.3173169)。
