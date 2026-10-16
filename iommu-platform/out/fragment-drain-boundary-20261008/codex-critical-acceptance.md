# §41 根 Codex：几何边界接受与真实释放问题

2026-10-08。本文件是根的机制分析和关键证据审定，不是 DeepSeek 摘要。前阶段结果只引用；本阶段只执行了一次静态数学计算，没有 DUT build/run、BIT、SD 或板卡操作。

## 1. 接受的计算与纠正

实际命令 `python3 out/fragment-drain-boundary-20261008/geometry_oracle.py > out/fragment-drain-boundary-20261008/geometry-oracle-result.json`，exit0。根已读完整实际脚本，确认 brute 按新增 cut 子集独立搜索，以端点 DAG 判断可达，不调用 greedy 的选择规则；随后只执行一次。

- 全网格 N=1..5：7585 分类；任意合法 A，N=1..4：3805 分类；合计11390，greedy/brute 不一致0。两组有重复覆盖，不能当独立 DUT 实验。
- N10、D={0,4,6,10}、R0={0,10}、B2、H4：新增 cut6 支持 q4 和 q6，真实最少1；旧逐目的节点收费模型算2。B0时最少2。
- 32KiB、source边界每2KiB、destination边界1KiB后每2KiB，B0/24且 H4KiB：8新增 source cuts，理想source24/AW17（每DMA）。H2KiB需16 cuts/source32；H32KiB需0 cuts/source16。B1024时三个H均不用新增cuts。
- 普通软件 tiling `3KiB + 7×4KiB + 1KiB`，9 subjobs，B0/H4KiB 的静态 source24/AW17、8 cuts 与下界一致。没有运行这组软件，也没有测 TL/PTW、CPU、供给、DONE 或排空成本。

根纠正§40模型：B>0时一个cut支持多个停止点，不得继续使用逐节点收费DAG。新的模型只证明目的安全点的最大间距H；H不是budget-off之后的额外工作量或周期上界。有限穷举支持一般交换证明，不能代替物理协议证明。

## 2. 根的证明判断与创新判断

窗口 `I(q)=[q,min(q+B,N)]`。免费安全点先按≤H推进到x；选(x,x+H]内最大的可支持q*，再选I(q*)最右合法c*。任意最优解的第一新增支持c_o对应q_o≤q*。若c_o≥q*，它是I(q*)的合法候选，所以c_o≤c*；若c_o<q*也有c_o≤c*。对旧路径任一t≥q*且受c_o支持，有t≤c_o≤c*及t≥q*≥c*−B，仍受c*支持。q*从x可达，且可接回原路径，不增cut数；归纳成立。这个证明依赖静态固定A、统一单位费用和只计目的间距的H。

因为这只是静态窗口覆盖与普通分段的贪心构造，且普通内部splitter与软件tiling均能达到同一cuts下界，所以不能将“聪明分片”或切分位置作为独立机制。不能只击败统一1KiB片或大缓冲弱基线。

同时必须区分：普通通用splitter能执行某一策略，并不自动证明已有文献发表了那项策略；创新审核必须比较决策规则、信息、保证与真实成本。当前否决理由是几何问题及其构造本身普通、没有独有决定/保证/成本证据，不能把“可实现”当作所有未来策略的先验否决。

## 3. 已直接核验的源事实与 worker 纠正

根本阶段直接核实际源/冻结源：

- §39 `multi_actual_dma_top.sv:354–370`、`dma_core_wrap.sv:306–318`、`idma_buffer.sv:33–52`：实际DmaBufferDepth3、64bit、8字节lane，每DMA24B，两路48B。64bit连接已直接核，不再保留worker的条件假设。
- `idma_axi_transport_layer.sv:254–257,343–360`：按当前有效mask/byte lane的FIFO可写空间接收R，W逐拍消耗；不是完整R空间预约。总24B也不自动等于任意状态下可独立停车B24。
- `out/natural-write-order-20261008/trans_ctl_write_limit.sv:226–247`：两bank各16拍W FIFO，名义128B/bank、256B总。这是共享目的W通路，不是任意流可用的私有R尾缓冲；固定写顺序可能阻止使用。
- `trans_ctl_write_limit.sv`是out中的C0修正副本，**不是生产已修正**。DeepSeek inventory 中把它泛称为production的表述不接受；生产C0状态不因本阶段改变。
- `dma_core_wrap.sv:143–184`：frontend以`~|idma_busy`作为backend_idle；`idma_reg64_frontend.sv:588–598` ADMIT在backend idle后才允许下一copy。这是软件subjob候选接口，未证明其覆盖全部外部route/response资源。
- `dma_core_wrap.sv:318`配置`ErrorCap=NO_ERROR_HANDLING`。前阶段“普通error+discard”是文献/未来强控制，不能报告当前DMA已有安全失败copy接口，更不能偷换成成功copy。
- `idma_fullbuffer_certificate.sv:1–44`只观察job fire/done、容量≥4096和对齐4KiB sole copy的资格；它不改变握手，不预约每个R的完整空间。本例24B/32KiB permit条件不成立。不能把此名字当作已存在的完整R预约机制。
- §39 `compact_pretl_owner_fragment.sv:136–141,252–270` R仍直通，busy到真实child RLAST清除；parent只在真实父尾交付后退休。暂停tail可保留parent，不得人为强迫其退休来制造创新。

根本阶段另直接核§39 `compact_pretl_owner_fragment.sv:87–123`：每个source child首次offer之前，capacity由**两层匹配且物理已准备**的连续W prefix界定。因此§39正常当前child已被普通准确门覆盖，不能把第5节“当前AR需要尚未准备AW”的纸面反例说成这版DUT新漏洞。真正可能的budget-off义务是已有较远W owner/token要清退，可能必须继续source child及同job必要raw parent接收；其是否实际可达/最小成本未运行。

`multi_actual_dma_top.sv:610–622`把两DMA raw AR移出共享axi_mux，按perflow capture credit进入保存槽。这个研究原型可保留局部parent同时服务另一流；生产常规mux有LockIn/spill的接口义务不同。不能只从child busy0推全通路可复用，也不能据研究原型反推生产具备同能力。

inventory保留全文不覆盖历史；本文件优先于其条件猜测。shared spills、mask和所有分支的独立停车保证尚未完整证明，不将其简单加和成B。

根另核两份实际vendor `axi_mux.sv`：`cva6/vendor/pulp-platform/axi/src/axi_mux.sv:207–235`及IOMMU vendor副本`:287–314`，在`!w_fifo_full && aw_valid`时先push owner，mst_aw_ready为0时锁住AW。W队列承诺不能只按最终DDR AW握手采样；应包括首次offer和更早内部不可撤回选择。只盯物理pending计数可能漏掉仍占共享W头的义务。

## 4. 主源边界

AXI-REALM扩展论文 arXiv:2501.10161，§III-A/III-B；官方axi_rt冻结`50153a346b753dc2bc7723c446656a43db35d02d`，依赖axi`fccffb5953ec8564218ba05e20adbedec845e014`。根直接核`axi_rt_unit.sv:197–219,278–282,352–385,393–415`与`axi_isolate.sv:245–378`、论文txt368–397/414–443。当前RTL在物理AR/AW握手按整burst bytes扣预算；不要把论文的per-beat文字直接写成该代码扣费位置。代码冻结版未证明等于论文评估revision。

isolation Hold保留已有VALID；Drain停未来AR/AW，继续已有W/B/R，RREADY仍取决于master。tail `isolated`允许splitter内parent metadata保留；top输出用head/tail的OR，不能据此推父槽全部清空。没有声称production实例化AXI-REALM，也没有运行其源码。

Arm DMA350 TRM Issue04 §4.3.4/§5.8.1：完整R空间足够才AR，完整W已有才AW，直接针对R/W循环依赖；是强普通控制，必须搭配同B小burst比较。不能只拿大FIFO版本作对照，也不能声称TRM的pause已保证全部总线事务清空。

2018 MMU-aware DMA §III/§IV-C/§V-D：一端local SPM，miss返回失败、排空、重试；64B例是metadata成本，不能当整个系统payload只有64B。该合同与本平台两个共享外部端点的无错误streaming copy不同，这种不同本身不是创新正证据。

## 5. 根进一步提出的严格问题：暂停的义务集合

选定释放点g，目标是g处没有held AR/AW、没有必须在g继续的W、没有未完成R/B；允许非共享的copy/parent进度metadata保留，并可正常恢复。不同释放点须分别声明，不能将tail、TC、原parent或frontend busy混为一个状态。

请求暂停时t0的种子义务包括：已经首次在g输出VALID的AR/AW（包括未握手），已经接收且尚未返回的R/B，对已经承诺AW必须供给的W，以及已占用的共享通道状态。停止新增source work之前也要算pipeline中已经不可撤回的offer。

对每流，若其种子读义务全部返回后，未写出的有效byte不能在独立、按lane保留的空间中停车，则必须继续某些目的W。这些W需要实际AW、合法译址、route/W队列位置和足够source数据。两个写队列中排在所需W之前的前缀也成为义务，可能引入另一流的数据/译址；它们又引入新的source或destination工作。仅按(p−q)≤B检查一次不够。

应求的是这些依赖扩展后的最小可完成集合及可执行顺序。集合有限不自动意味着可完成：两层W顺序和阻塞的PTE返回可能形成环；总byte足够也不保证每条lane/owner可停车。普通图闭包本身不是新算法。充分性仍须给真实存储、offer栏杆、顺序相容、所需译址/服务独立可进展与公平响应前提。

不能把整transaction完成当作所有依赖的节点：一个2beat R→2beat W、FIFO仅1beat、AW可服务的普通流式copy，R完整完成要等W腾空间、W完整完成要等R数据；粗图画成环，实际`R0/W0/R1/W1`可正常前进。后续模型必须保留beat或已可消费prefix milestone。此纸面反例由第二轮proof席提出，根同意；不是新的DUT运行。

最小纸面反例：某已接收source AR还有L有效bytes待回，独立可停车B<L；没有任何已承诺destination AW可释放空间，必需AW尚未首次offer。同时关闭未来AR/AW后，旧R只能接收到B处，未来AW被gate阻止，完整旧R不能在不重新准入的情况下排空。周期续额可解除，不称永久死锁；完整R预约、小child、预先承诺destination或允许有限destination续行都属普通解。该反例是组合适用性问题，不是新的生产测量。

这个反例还要求没有其它可达的payload停车/消耗路径；若shared W FIFO允许先收数据，该容量及其是否须回收也要纳入。不能只用名义DMA24B忽略共享W存储，声称已证明当前系统同样停滞。

## 6. 下一工作与明确退出条件

先把上述义务界对齐真实g和状态，不做新的cuts性能矩阵。强控制同时包含：同信息内部非均匀splitter、同B短child完整R/完整W预约、只停source但有限destination续行、AXI-REALM透明child/完整W缓存、软件macrocopy tiling（CPU/TL/DONE/供给成本计入）。不用人工新加TLB锁、长期不续额或要求全部原parent退休求胜。

如果必要/充分组合条件超出已有普通合同，且能给真实多DMA的紧成本界，可发展分析贡献；如果新的准入/状态表示在同释放点、同工作量目标、同总资源下达到上述强控制做不到的点，才可发展机制贡献。因为目前没有这样的独有保证或成本证据，不能认定独立创新，也不承诺期刊分区。

如果整理后只是普通完整R预约、区间覆盖或依赖图闭包的重述，就停止把该枝条当主创新，将它作为平台解释和实验控制保留。不是无限加层次/改名字延长候选。两席第二轮已完成，根已直接读完整返回稿并审定：`release-closure-proof-agent.md`、`release-composition-prior-agent.md`。受限单flow固定点证明正确但属普通守恒；多flow/真实状态最小续行与成本点未证明，也未运行。未用一份候选算法被通用模块实现这一事实替代文献新颖性核查。

归属：静态模型和上述机制推演为Codex根，两席作独立关键审核；DeepSeek整理inventory并提出数学脚本，Codex审阅完整脚本、实际执行和接受输出。未调用Claude。

## 7. 本阶段新增前沿的根核查

根经官方arXiv题录与全文再次直接核两份2026稿，均不是仅靠agent摘要：

- [Physically-Aware Preemptive Virtual Channels for Deadlock-Free AXI Networks-on-Chip](https://arxiv.org/html/2607.01430v1)，v1 2026-07-01，§I Fig1已经展示DMA逐beat R→W及外部W占路导致端点环；§III比较交通类分离、credit和preemptive VC。不能将“发现端点读写耦合环”或“另辟通道”当新意。本文没有因此自动证明我们的IOMMU组合成本界。
- [Scalable AXI4 Transaction Monitoring for Mixed-Criticality SoCs](https://arxiv.org/html/2608.30435v1)，v1 2026-08-31，§III-A6及§IV-D已给cut-and-drain和容量/长度排空界；其方法合成error响应，界假定每通道每cycle接受一个合成beat，ILT另需接受单beat RLAST的manager。不能直接套为成功copy、小FIFO及外部共享PTE路径的无条件cycle界，也不能把“给排空界”本身称新。
- [AMD PG313 NoC Deadlock Avoidance](https://docs.amd.com/r/en-US/pg313-network-on-chip/NoC-Deadlock-Avoidance)，发布2026-06-23，明示NoC进度依赖端点没有外部协议依赖，并给分离路由/VC和near-full反馈选择性停source方案。这直接覆盖宽泛的组合适用性警告及反馈节流。

因此，普通端点依赖、beat级进度图、选择性停source与给一个保守drain界都不能成为本阶段的独立贡献。只剩“成功并可恢复copy在同资源/同释放点下的紧工作量与可实现成本界”需要具体增量证明。未检得全文覆盖该特定命题，也不是全领域未有前作的证据；不得提前判创新。
