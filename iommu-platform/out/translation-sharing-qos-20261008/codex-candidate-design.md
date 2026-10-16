# §47 root：从历史过滤转向已注册任务的剩余翻译用途

2026-10-08。Codex亲自设计与分析，直接核 iDMA frontend/legalizer、IOTLB/fast接口及三项专利关键原文；两席另查强前作/构造反例。下文是源码和纸面设计，没有新DUT、数学运行、生产修改或板上结果。DeepSeek证据全文另存。

## 1. 先停止两个普通机制枝

共享hit不必修改对方替换历史；accept正确结果与history touch分开即可，无须拒绝合法hit转去PTW制造额外工作。US7415575B1的直接前作继续作为对照。

一次共享PTE读cost=1、两个beneficiary时，“物理记账总和=1”与“每beneficiary都独立扣1”不能同时满足。普通均摊、由发起者扣物理工作并另记logical请求量、closed cohort都可明确合同；迟加入者不额外耗同笔PTE并不等于不耗响应/lookup/MSHR资源。普通动态priority继承还须核Li2015 late merger/age机制。不能把改变公平指标或多waiter bookkeeping叫独立算法。本平台normal单walker、fast只hit，不能把合并miss例冒称当前可运行路径。

## 2. 新候选的准确输入

历史告诉我们以前命中了什么，但设备已经接受的copy job还提供src/dst/len。在已注册任务范围内，它可告诉我们后面哪些虚拟地址仍需要翻译；不提供未知未来job。

实际frontend `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:325–327`直接给length/src_addr/dst_addr；同文件525–526的batch路径另加offset，不能把循环任务future写死成一个不变descriptor。legalizer `src/idma_legalizer.sv:241,260`推进各方向地址；454–464的推进点是下游r_ready/w_ready、coupled或decoupled选择，不是IOMMU translation materialized事件。其cursor不能直接宣布所有之前的地址已经完成译址。

当前IOMMU单笔req/fast lookup没有job范围和剩余byte长度；新信息接口、上下文绑定、队列深度、握手/失效与元数据开销都要计入。引擎内部本来有字段不等于IOMMU免费有了可信信息。

## 3. 建议试判的微架构与规则（未实现）

限定起步合同：静态合法4KiB页、有限已注册incremental copy jobs、无地址溢出；未知future job照常到达、按普通策略处理。消息J=(job/epoch,context-ref,src,dst,len)，上下文必须与最终译址语义匹配；它只影响缓存替换，不替代权限核查，不跳过正常失效。

为每job两个方向维护：
- 连续已经materialize到private request metadata的虚拟byte前缀；
- 已offer或已发但尚未得到足够翻译结果的pending区间。
- 若同direction结果无序，增加已计费的hole/window结构；只有证明其顺序与完整覆盖，才可压成一cursor。取已接受地址最大值不等于连续前缀。
- resolved消息必须说明实际覆盖的start/len/dir/job epoch；一个page lookup不能默认整页所有未来burst都已翻译。

对cache entry e的范围，计算
`known_live(e)=OR_{registered job,src/dst} [range(e) intersects still-unmaterialized suffix or pending range]`。
这里“未materialize”是译址结果没有在原请求private metadata中保存；不以RLAST/WLAST/B response结束为必要条件。数据还在途而地址/权限已保存，通常不再需要全局IOTLB留着该条目。当前上游legalizer推进与private result生成之间的gap须保守包含，不能漏掉held请求。

初始替换规则：正常共享hit照常返回；在同一可分配容量内优先回收known_live=false项，其他项用普通替换/任务感知准入。该规则不强pin全部live页，不让cache满阻塞正常miss，也不承诺仅凭range就得知精确下一访问cycle。若按V remaining-use区别保留，要同时计算A的真实用途并评价整体性能；把A热页强行丢掉只为提高V不能冒充双赢。

这是一项有明确接口与错误边界的候选，但仅“range比较+dead-first”还不是独立贡献。真正要核的是：能否以可承担的job/translation窗口状态实现，较同信息、同总容量的普通任务感知方法有稳定完整copy性能或成本优势。16个entry×多个job×两方向的比较逻辑、跨模块连接/队列也算成本，不宣称一个bit或免费已有寄存器。

### 3.1 一个可降低状态的保守版本（待链路顺序判别）

第一版不追求精确byte last-use。若单job每direction请求严格INCR、译址成功返回与其请求同序，则记录最后成功materialize的start-page q；只有页e<q才认定该direction已经越过它。当前q页保守保留到进入下一页或可靠job结束；再与该job结束页、src/dst和其他job用途取并集。这样不从AXI len推断有效tail/strobe，也不把一个成功lookup当整页future burst全完成。这个page frontier可能保留多余条目，不能宣称精确next-use。

 root直接核TC wrapper:121–164的AW/AR各自FIFO、620–670只选其head、693–702的AWAIT_RESP接受响应并保留trans_resp_data、716–726在内部完成地址握手后才回IDLE；banked wrapper:42–46每bank最多一笔TL request。frontend:333将AxID作为固定接口ID，TL:303–308的fast仅不同bank。这支持native同bank每direction的返回FIFO序；bank是原AXI ID位，**不是方向/设备身份**（banked wrapper:17–18）。cursor必须按(job epoch,actual job owner/DID,dir)区分，仍须另证上游该job的地址递增、固定bank与epoch交接；legalizer推进不作materialized frontier。

若顺序证明成立，两个W-bit page cursor/注册job只是部分附加状态：W取实际有效VPN宽度、另计端页/原descriptor/context/epoch/有效位和全部接口逻辑。若同direction可能乱序，则保留前述pending/hole窗口，不偷省状态。普通job-aware interval比较或每DMA两项last-page ATC也可以利用这种信息；低成本版本的独立性仍须具体对比，不能用两个cursor本身作创新证明。

对于2M/1G条目，判断必须用entry覆盖末页<q或完整范围intersection，不能只看entry起页；本初始构造仍限4KiB。末页的释放需可靠last-translated marker与pending信息；没有这个接口时可保守等backend job done，但不称精准提前释放。

**最新out原型不能直接套native顺序证明**：§39/§43的prepared-parent/source helper另有slow准备与prepared-parent复用快返回，ParentDepth4的valid/seq窗口可能保留尚未完成的较前parent。必须用producer连续watermark或计费holes；prepared(已offer)不是valid(成功准备)。本轮没有证明全部eligible/pause选择构成前缀，不能宣布全路径只有两个cursor。

一个成本预警：若D=4、E=16、W=29，则仅两direction frontier为2DW=232bit；另存两端页再232bit，未计tuple/context/epoch/窗口。普通per-entry、per-DMA、per-direction用途bitmap为2DE=128bit（它也另计正确初始化/更新信息与pending）。这不是完整等成本比较，却足以否定“O(D)必比O(E×D)更小”的宣传；需要实算整体，不能靠大O判低开销。

## 4. 一个手工信息见证与其局限

只作两逻辑槽抽象（不是当前板端配置）。初始cache={X,Y}，到来cold Z；当前已登记A工作还会再访问Z。两世界之前lookup/history/PSCID全部相同：
- 世界X：已登记V job剩余只需要X；
- 世界Y：已登记V job剩余只需要Y。

后续有限请求分别Z,X或Z,Y。若要求保留即将重用的Z，纯history必须在同状态下选同一个X/Y victim；在其中一世界会多一次miss。绕过Z填充也会在后续Z产生再miss。知道已登记用途，可保留Z与对应X或Y；本抽象中只剩mandatory Z miss。对均匀两世界，固定history choice的期望miss=1.5，known-use choice=1。它只证明新增信息可以影响决策，不是本平台copy轨迹或吞吐测量。

普通descriptor-aware pin/admission、已注册job-aware lookahead甚至给同future信息的Belady对照也可达到1。因此不能拿这个见证直接宣称新算法；不限制普通对照拿同样job信息。更不能把其他队列/端口状态藏起来，说已在完整RTL边界证明不可区分。

## 5. 已直接读到的接近前作

- [US20230401160A1](https://patents.google.com/patent/US20230401160A1/en)：网页L352–356/683–685明确DMA stream setup command可broadcast给IOMMU作为prefetch；“把任务地址提前送过去”已经有具体机制。
- [US20250190351A1](https://patents.google.com/patent/US20250190351A1/en)：L187–198/333–343给VA/PA+DID/PASID/权限、重复抑制、覆盖优先级和禁止覆盖，并讨论transient DMA map→use→unmap。不能把预装/protect/job完软件unmap改名为新规则；列明段落未直接实现每job remaining-use自动反馈，不据此宣称领域没有。
- [US20070067505A1](https://patents.google.com/patent/US20070067505A1/en)：L153–169/218–226给per-port allocation/deallocation windows、completed translation lock-down至top-of-queue消费，避免未用就淘汰。它与当前private parent保存译址后无需pin的消费边界不同，但必须作为普通pending-use保护对照。
- [MMU-aware DMA2018](https://arxiv.org/abs/1808.09751)是额外直接强对照：root实读既有作者全文转换out/shared-translation-literature-20261006/papers/socdma2018.txt:367–424（与其他转换版本的590–637不同）。全transfer预装锁定直至完成是原引擎不能识别failed burst时的限制；其新设计明确每burst只在请求到IOMMU当下需翻译，保存元数据、记录失败、停后续请求并排空重试。不能强迫ordinary整job锁，或把译址消费早于data完成说成本轮新发现。候选区别只能在无fault/retry时的多registered-job有限remaining-use管理与成本/收益。
- AlphaServer1996 DMA/SG第三预取缓冲、rIOMMU2015 ring/burst-end及SHiP/Hawkeye/UCP的预测/容量控制均要比较，不能只打纯LRU。原文范围由证据worker与prior席定向补核，不称本轮重新读完全部论文。

三项网页关键原文返回保存在`root-primary-browser-evidence.json`；网页行号属该次browser版本，不是PDF本地行号。专利只作为技术前作，未作法律状态分析。

## 6. 下一接受门

保留这一条作为可实现性/成本候选；停止history过滤和coalesced charge本身的创新枝。接着核顺序、materialization事件、job队列可见性与同信息普通基线：最简job-aware admission/demotion、每DMA少量last-page microTLB、prefetch+bounded pin/release、动态容量都允许同资源。

先做工作负载必要性筛选：若每DMA只单调读一个独立source区间、写一个独立destination区间，且page越过后无人再用，普通每direction last-page cache/共享entry指针已经可避免该方向的容量重读；候选不能从‘知道用完了’再省掉这些compulsory walks。每DMA两项last-page跟踪既可用私有完整translation，也可用共享entry指针/普通ref保护；后者必须同计，不能只和会复制大量条目的private cache比。

更合理的最小场景是两DMA读同一只读输入、写不同输出，进度错开，使一方已经读过的source页稍后仍会被另一方使用。它不要求新coalescer或错误映射，也不会用相互覆盖payload制造收益；native研究席§2给32KiB两copy的合法页表/握手字典。unknown相位下范围用途不提供精确next-use次序，普通全局replacement、job-aware interval/protection union与last-page指针都自由比较。先证这种delayed reuse的额外非compulsory PTW存在，再评整体copy时间；若普通同信息方法已经消掉重复翻译，或实际性能由数据搬运决定，就停止纯替换算法贡献。

如果效果只来自额外future信息，或者普通同信息方法同成本达到同收益，就不能把“知道后续页”称独立算法；可记录为接口/测量结果。如果新硬件把可靠剩余用途与多DMA共享管理结合，以较低成本/较好完整copy性能超过这些具体强对照，并有失效/held/交叉方向正确性证据，就可以论证独立微架构贡献。原则与最终贡献分开，不承诺期刊分区。

不能要求“所有未来可想象普通程序都数学不可能”才能接受算法贡献；这会把任何可实现算法都称为普通记账。本轮门槛是具体已发表机制、合理强组合、相同信息/资源下的实际增量，而非无限拒绝。
