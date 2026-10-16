# DDR服务边界首关：保留前序读债务，性能入口unknown（2026-10-14）

已执行批准方案并登记实际goal1791941230，无额外token预算。**生成MIG参考没有“让后来PTE越过已接受payload读”的外部接口；这揭示私有DS优先模型不能直接代表实际DDR，但尚未给出两合法翻译队头的独有完整收益。** 本轮完成服务边界交付，停止进入双头来源/A-B/预测器。F1全族、论文主问题、既有8/9月以来的资产与关闭裁决保留。

唯一硬件参考是现有Genesys2 composition构建目录。24项源码overlay、构建输入和bit哈希一致，顶层只有构建器规定的legacy observer1→0、nonblock0→3；XCI为MIG4.2、AXI64/fullID6，4个共享源端、12个目的端，PTW/DDTW/payload通过原atomics及异步clock converter进入同一DDR。参考配置NORM/RD_PRI_REG，内部64→256 upsizer。完整[来源绑定](../out/ddr-service-boundary-20261014/root/profile-binding.json)列出SHA、profile变换、版本及证据限度；当前加载身份未核，生成IP未在build_start单独封存，未独立证明全部bit内部语义。

| payload承诺阶段 | 已有外部普通优先能力 |
|---|---|
| 未接受、未在该点首次VALID | 同时拥有两个合法请求时可以先offer DS；held和CDC/spill接受边界须分别核。 |
| MIG AR已接受、R未完 | 不能撤读或read-vs-read提升；QoS用于读/写非零提示，RD_PRI_REG不选读队列。 |
| MIG R已held | 保持RID/DATA/RESP/LAST直到握手，不能替换为PTE。 |

native bank重排与外部返回顺序不同。生成源码的metadata/数据FIFO及解包路径保序，与[AMD UG5864.2 Reordering](https://docs.amd.com/r/en-US/ug586_7Series_MIS/Reordering)一致。厂商RD_PRI_REG正文未获得，精确逻辑由生成4.2源码支撑，不用旧4.1PDF代证。读FIFO32个native256bit单元、30个native命令metadata及2级skid，都不是可调度整burst队列，256/16阈值也不是PTE墙钟延迟保证。

条件因果为：已拥有数据读L先被MIG接受；后来PTE P接受时L还余k>0个R握手，则P.firstR晚于L.RLAST握手。k只由已接受ARLEN及真实MIG R握手定义；不使用隐藏DDR状态或构造满FIFO。k=0或P先接受时该正前序债务不成立。这里只给结构/机会下界，未做新运行，没有周期收益、A/B结果或全任务完成数。

本轮缺的是**首个NormalTL选择能改变P前序债务且普通冻结规则不能复现的完整反事实**。共同选择前已接受的L对两顺序都是不可撤销公用债务，不能计为候选收益。九项冻结普通合同、现有命中旁路、承诺前DS优先及37项成本方法继续有效；没有自然双头源证，既不接受独有收益，也不宣称普通规则完整覆盖F1。

新增观察、fullID/退休关联、PTE/service历史、CDC、一致snapshot、端口、预测虚拟状态和算术、fairness、选择延迟、历史生成及CPU费用逐项保留；精确总成本、双方完成、面积/Fmax/应用均null。[完整机制卡](../out/ddr-service-boundary-20261014/root/mechanism-card.md)给出所有权、三阶段链、强对照、成本与接受/停止理由。

后继只接一个现存正常来源的新原始事实，先证明两合法队头选择造成普通规则外的前序债务差，再核该一个安全双头来源。两关过才一对A/B，净完整收益及成本存活才≤4条件，随后GPU/AI工作流与RTL/FPGA。没有新事实不自动恢复旧分支、扫相位、增加压力或更换返回模型。

本轮root和唯一新独占MIG席实际gpt-6.1-sol/ultra，原生有效475000，未手改窗口；没有DeepSeek/Claude/Astra、旧席恢复、源码修改、build/仿真/板操作、旧实验/validator/trace重解析。只进行新交付核验。[导航与账本](../out/ddr-service-boundary-20261014/README.md)／[稳定裁决](../out/ddr-service-boundary-20261014/root-result-acceptance.json)／[下一门](../out/ddr-service-boundary-20261014/next-goal-contract.json)。五文档同步和新交付核验先完成，再调用actual complete。原立题窗口累计，本轮完成不代表论文完成。
