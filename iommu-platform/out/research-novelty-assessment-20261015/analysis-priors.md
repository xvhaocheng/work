# A的最近分析前作：原文复核与可发展的精确增量

2026-10-15；Codex独占关键前作席。先复用并核查[既有分析差异表](/home/xvhaocheng/work/iommu-platform/out/control-position-method-20261015/literature/analysis-delta.md:20)，再定向读三篇原件的决定性方法。本轮只增加两项会改变主张边界的近邻，不做全领域查新；没有实验、仿真、构建、生产修改或外部模型调用，不改旧裁决与goal。下文“A”是父会话交付的拟议输入/输出，未把现有源语义原型写成时序验证已通过。

**判断：应继续发展A，但主张应是“由真实共享翻译接口合同推出可核查的控制部署决定及完整任务后果”。三篇已核方法没有直接交付这一合同与判别；它们已覆盖任意任务内到达、在途约束、组合服务、外置调节和AW/W缓冲等大量构件。仅增加owner字段、画完整DAG或重新跑通用状态探索，不足以证明独立方法增量。**

## 1. 三项决定性前作的输入、假设、算法和输出

### TC2023：能保守覆盖依赖到达，不能直接吞掉所有反馈阻塞

[Restuccia等，Bounding Memory Access Times in Multi-Accelerator Architectures on FPGA SoCs](https://retis.sssup.it/~giorgio/paps/2023/TC23.pdf)，IEEE TC72(1)，154–167，2023。

- **输入**：周期硬件任务，每job读/写事务数`N_i^R/N_i^W`、burst长度`B`、每通道最大在途`f_i`、计算`C_i`、周期/期限`T_i`；层次AXI拓扑、RR粒度、各通道传播/保持和PS服务常数。§2.1–2.3，原文[txt:103](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-rtanalysis.txt:103)、[txt:132](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-rtanalysis.txt:132)。
- **范围**：明确允许job内任意事务时序（L122–130），所以“上一PTE返回才产生下一PTE/载荷请求”可以属于它的到达包络；不能声称内生到达本身使它无法给出响应上界。前提是总工作量、归属和服务可以安全映射。反之，§2.1明确假定读写事务独立（L118–122）；§2.2假定interconnect pipeline永不满，把数据阶段挂住留作扩展（L185–192）；§2.3采用接口处顺序返回与经文档/测量给定的MC延迟（L202–226）。
- **算法**：§3.2–3.5结合层次直接/间接干扰、周期预算和在途上限；§3.6 Lemma10给各事务流水干扰代价；§3.7 Algorithm1逐层累计尚未计数的干扰，Eq.(4)得到`R=C+自身无竞争读写代价+两类干扰`，判断`R≤T`。精确入口：[txt:507](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-rtanalysis.txt:507)、[txt:558](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-rtanalysis.txt:558)、[txt:590](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-rtanalysis.txt:590)、[txt:657](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-rtanalysis.txt:657)。
- **对A的准确边界**：已有carry-in、有限在途、流水与完整job响应上界。已发表公式没有逐请求依赖、owner、first-offer锁定头、观察历史和各控制位置的可选动作；同粗参数更换这些关系不改变该公式。A可以增加部署信息与精度，但这仅证明输入有用。若有限返回槽、源R→目的W或许可反馈改变服务/造成循环，必须另给安全抽象，不能把未知等待塞进`C_i`形成循环定义，也不能直接套不满pipeline的Lemma10。

### AXI-ICRT：已有可随机选缓冲与组合可调度性，代价是改互连

[Jiang等，AXI-ICRT: Towards a Real-Time AXI-Interconnect for Highly Integrated SoCs，作者原件](https://userweb.cs.txstate.edu/~k_y47/webpage/pubs/tc23c.pdf)。

- **能力/输入**：§4.2以RAQ替代FIFO，TCU按目的分组并作全局/局部组合调度；§5.1有可配置payload FIFO和PID/TID/valid元数据；§5.2经AW/ARUSER提供job/事务优先级并维持job内顺序。不是忽略有限缓冲的纯数学模型。入口：[txt:305](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-icrt.txt:305)、[txt:457](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-icrt.txt:457)、[txt:486](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-icrt.txt:486)。
- **时序假设**：§4.1将读写通道分别对称处理（L295–303）；§4.3的B路径pass-through并视传播时间可忽略（L421–442）。§6把事务执行按最坏处理时间`q`量化，Primary获周期服务器`(Π_i,Θ_i)`；sporadic任务`(T_h,C_h,D_h)`每job是一批连续事务，只能在事务之间抢占，限定`D_h≤T_h`。入口：[txt:717](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-icrt.txt:717)、[txt:831](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-icrt.txt:831)。
- **算法/输出**：§6.1比较任务DBF与按完整事务修正的SBF，Theorem1给其模型的deadline条件；§6.2界定可行period搜索范围，§6.3/Algorithms1–3搜索最小可满足带宽的服务器接口参数。入口：[txt:887](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-icrt.txt:887)、[txt:1014](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-icrt.txt:1014)。
- **对A的准确边界**：可选队列、隔离FIFO倒置、非抢占残余和组合服务已有具体实现/分析。ICRT的§3.1“dependency”特指路径间递归阻塞（L226–232），不能仅按这个词推为已给PTE生成依赖模型。A若不改固定FIFO/MC，不能免费继承RAQ随机选择。依赖返回才产生后继的非连续批次也须安全转换后才套其定理；补B/consumer/owner只是适用性修正，须进一步导出实际不同的部署或成本决定。

### AXI-REALM：外置分段、按region预算和先收W后发AW均已实现

[Benz等，AXI-REALM原件v1](https://arxiv.org/pdf/2501.10161v1)；另核现行[2025-07-07 v2正文](https://arxiv.org/html/2501.10161v2)。本轮所涉§III关键能力在v2仍明确存在。

- **输入/能力**：§III-A1/A2运行时burst粒度、地址region、两个AW和一个分段W缓冲；写数据全到才向后发AW/W，避免DMA慢源造成W带宽长期占有。v1[txt:323](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-realm.txt:323)；v2 §III-A1/A2。
- **机制/保证**：§III-A3按manager/region的period与budget逐beat扣账，预算耗尽后限制/隔离，周期续充；§III-B要求互连前进保证和RR。输出是受调节的事务流、监测及条件预算保证，不是由某任务PTE/owner图计算最小有效控制位置。v1[txt:370](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-realm.txt:370)、[txt:421](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-realm.txt:421)；v2 §III-A3/B。
- **对A的准确边界**：可复用为必须公平比较的具名普通控制，含其buffer、分段、预算和阶段监测费用。A的目标应是何时这种普通入口已足够、何时目标反馈在此之前被锁住、何时新增能力值得付费。v2 §III-C已追踪事务阶段，不能以“分开AW/W/B事件”作新意；加入consumer最终结果也不能单独立项。

## 2. 定向新近邻：确实改变措辞的两项

1. **RTAS2026的具体外层控制前移已有先例。** [Aromolo等，Time-Predictable Acceleration of Deep Neural Networks on FPGA SoCs with Multi-Core DPUs](https://retis.santannapisa.it/~giorgio/paps/2026/Aromolo-RTAS26.pdf)，DOI10.1109/RTAS68450.2026.00033。直接核§V-A/B/C、§VI：每DPU内部至多一个pending/执行请求，其他请求留在外层按优先级选择；DNN按合法层依赖边界分块，在chunk间有限抢占；AXI-REALM式预算后profile每chunk WCET，套有限抢占响应分析。其`C_i,j`聚合内部内存行为，未给逐PTE/owner反馈的控制位置判别。故泛称“前移FIFO外控制、合同抽取、完整任务分析”已过宽；A必须拿出共享翻译的具体新决定。本文不评价其profile是否构成所有硬件条件下的严格WCET证明。原文定位：§V-A（网页抽取L497–580）、§V-B/C（L581–688）、§VI（L709–823）；未审作者实现或复证所有分析。

   **准确覆盖裁决**：如果A的贡献仅限于“FIFO外控制＋合法分块＋WCET任务响应分析”，则这一版宽主张已被上述具体发表工作覆盖，不能再作为独立创新。这个范围不等于A拟议的逐PTE/owner反馈与控制位置判别合同已经被覆盖；该候选差异必须交付不同的、事前可验的部署判定实例。

   **发表身份**：[RTAS2026官方程序Session6](https://2026.rtas.org/program/)列完整标题与六名作者（网页L95–103），会议2026-05-12至14；作者PDF首页有IEEE版权与上述DOI、印刷页275起；[Biondi作者发表表](https://retis.santannapisa.it/~a.biondi/publications.html)列为RTAS2026 Proceedings。IEEE站点被robots阻止，本文未把无法访问的出版商页面写成已核证据。

2. **AXI端点耦合循环与缓冲代价已有明确新近微架构。** [Leone等，Physically-Aware Preemptive Virtual Channels for Deadlock-Free AXI Networks-on-Chip，arXiv2607.01430v1](https://arxiv.org/html/2607.01430v1)。已读全文§I–IV，尤其§I的DMA读返回→本地写循环与§III的四种读写交通分离方案。它解决NoC交通类分离及物理代价，没有交付共享翻译的部署判别合同。其preemptive是NoC共享链路选择；不能据此撤回已在AXI端点发布的VALID。本轮不把“读写依赖/有限buffer/循环等待/拆流”列为A新概念；也不把该文无IOMMU专节当A通过证据。

技术近邻的定向检索为两批查询：三篇标题及analysis/follow-up，随后ICRT/REALM近期引用与Restuccia/self-suspending；另对RTAS2026作准确标题/DOI的发表身份核查。未找到的内容不作不存在证明；RTAS2021事件驱动任务模型只在RTAS2026参考文献中见到，原方法本轮未读，不以转述判其覆盖。

## 3. 最强合理增量与普通建模的分界

**最有希望的主贡献**：在冻结的真实任务与合法接口下，从地址/context、依赖生成、有限资源owner、信息到达和first-offer/accept义务，抽取可检查模型；对一组具名控制位置/普通规则给出：普通入口足够、达到指定目标必须前移或增加某项付费能力、某个特定动作已被承诺挡住、条件缺失所以未知；附合法动作/不可改变义务与完整victim/aggressor/consumer及资源归还的预测。必要性只相对冻结的能力集合与目标，不冒充所有可能架构的全局不可能性。

关键可发展点是**内生PTE/载荷反馈如何与owner和承诺位置共同改变“值得在哪里控”**，以及如何从真实接口可靠抽取这项关系。通用求解器能表达它，不证明该研究问题已经发表；但使用通用工具也不自动产生新方法。

下列属于已有工具或正确建模义务：固定trace的DAG最长路/max-plus、非抢占残余、carry-in、有限状态可达性、同观测历史同动作、VALID锁定、B/consumer/资源释放分别计时、输出unknown而非编造服务常数。仅把这些拼起来或让同TC粗参数的两个例子不同，仍只能证明实例信息更完整。

## 4. 一条决定性接受标准

冻结一个有真实任务来源的**部署选择问题**，其TC粗流量参数、总预算和服务条件保持可比，但真实依赖/owner/承诺关系让两个合法控制位置对完整目标具有不同效果。先由A从源接口合同给出位置选择、目标后果区间和可复核证书；保留双方既有义务、真实许可与consumer、普通调节/提前准备机会和完整费用。再在未参与推导的一正一反实例验证，不用基线执行后的trace反向补模型。

公平具名比较只需覆盖真正最近问题：TC2023的合法保守应用、同信息的固定普通入口/REALM式调节，以及标准事件/状态工具按相同合同的直接应用。接受的增量可以是可复用的抽取/判别规则和新部署结论，不要求发明新求解器或击败所有想象程序；若同合同普通建模直接给出全部结论，应撤去求解/策略独有性，明确剩余领域合同与抽取贡献及其证据。

**不能仅靠“同粗参数两例不同”通过**：那只证明多给了信息。必须交付一个原方法直接应用没有给出的、正确且有用途的事前部署决定，并证明该结果来自可重用规则而非人工指定反事实顺序。真实服务没有有限依据时可验合法性/能力判别，时间结果必须保留未知；源语义通过不能代替时序/性能验收。

## 5. 最强审稿质疑及目前缺证

最强质疑是：“这是把标准有限状态/时序分析输入填完整；TC2023已给粗上界、ICRT/REALM已给控制机制，RTAS2026已展示FIFO外调度和真实合法边界。作者只是手写模型后复述已知协议限制，何来可复用新认识？”

应正面回应的证据是：源到模型抽取规则及正确性；至少一条由规则推出的、不会靠已观察trace才知道的控制位置/必要能力条件；与上述具名普通应用的具体差异；冻结留出预测；真实任务中完整收益/保护与成本。当前题目给出的原型只通过源语义，真实时序未验，因此本轮接受“有明确发展空间的精确主张”，不签发新颖性或性能通过。

## 6. 阅读完整性与未核

- 三份本地PDF SHA256与`sources.json`相同：TC=`7f7f874f77d45c688b8f4154d3ac22fa70ffd14761241fbad5e71efce50b971a`；ICRT=`adfc570ef617b51ab12908e26e30dcbbbedd5a1693073f04a4fdadb0084a1cfb`；REALMv1=`1297dc54f33edca31f27f0a0048968cc3ce1ba81e575d4c9770fa50702b7725e`。
- ARS `pdf_read_preflight.py`对三份均为`UNAVAILABLE`，理由`pypdf-not-installed`（本轮2026-10-15T09:24:25Z）。未安装依赖、未报PASS；本文用原文章节与既存txt精确行，不把本地页锚当结构验收通过。两篇新近邻通过作者/arXiv网页直接读正文；其网页行是工具抽取定位，不冒作本地txt。
- TC全套引理、ICRT定理与RTL、REALMv2源码/物理集成、两篇新近邻实现未复证；本轮没有当前A实现或真实服务的源核任务，也没有全领域穷尽查新。
- 作者PDF网页入口有两次明确获取失败：TC旧域超时、ICRT12.6MB超过网页读取大小；两者本地原文已按上述hash/章节/txt读到，未把网页失败算作原文不存在。REALMv2只核相关§III与新近状态，不声称全文逐行版本差异审计。
