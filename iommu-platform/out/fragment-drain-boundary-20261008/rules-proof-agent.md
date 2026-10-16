# §41 Codex 独立理论核查：停止点条件、B>0反例与普通等效构造

2026-10-08，`innovation_rules_40`。Codex 独立推演并定向核源码与 primary；未调用 DeepSeek/Claude，未执行抽象枚举、DUT run/build或板卡动作，未修改生产代码。本稿只纠正并收尾受限数学问题，不称新算法、新锐界或已验证端到端保证。

## 结论

1. 上轮 `safe_stop(q) => r(q)-q<=B` 只能作为受限必要条件。要把它用于当前状态，必须把所有已经不可撤销的源/目的请求和真实W队头包含进来；旧未来partition的 `r(q)` 不能忽略更远的在途AR。
2. B=0时，共同结束点是必要的几何条件，最少新增source cuts可用节点代价表示。B>0时，旧node-cost1模型错误：一个源cut可使多个目的停止点安全，也会改变此前各q的 `r(q)`。
3. 在明确的静态几何模型里，B>0可以写成“选择目的检查点＋source结束点刺中窗口”的问题，甚至有简单的最右可支持前沿贪心和交换证明。**因为普通非均匀descriptor splitter加同准入栏杆能产生完全相同的结束点，这不是排除强普通后的独立调度机制。**
4. H在此只限制相邻安全目的前沿的距离。没有限制已offer AR/AW的栏杆、真实可独立停车容量、W序一致性与translation/DDR进度证据，就不能把H称为实际停发后的排空工作量或周期界。

## 1. 定义：严格区分数学分段与端到端停车

### 1.1 Copy、端点和合法新增cut

考虑一个正常、固定映射、线性byte-preserving的copy，逻辑有效区间为 `[0,N)`。源/目的物理地址可不同，以下坐标均为copy中的有效byte offset，不把地址差当已复制字节。暂不含memmove重叠、fault、invalidation、ATS、撤销或其他计算依赖。

`R0={0=r0<r1<...<rm=N}` 为现有物理source burst的强制有效byte结束点；`D={0=d0<d1<...<dn=N}` 为现有destination burst的有效byte结束点。这里destination结束意味着对应的完整WLAST已经交付并从两侧W序列退出；不是仅AW握手。若一个层次还保持W route，必须等该层次也退出。

默认只细分source，不合并或删除R0、D的已有端点。新增cut集合C只能取合法集合A，且不能落在已首拍VALID的物理burst内部。离线问题在所有相关physical offer之前规划，所以可使用整个合法A；在线问题要从A删去已经不可改变的范围，且下面的贪心最优性不能不经审查就延用。

为证明静态贪心，进一步限定aligned INCR、固定beat网格g；A为该网格上的合法source有效byte端点，允许全部合法细分，细分不引入额外跨page/最大len违规。mandatory 0/N不收费。最终partial beat可由真实有效byte mask代表N；若存在source discard、realignment、多种AxSIZE、非modifiable或atomic交易，则必须用实际合法结束点集合，不能直接按任意byte切分。D中的q若 `[q,q+B]` 不含合法source cut，就不是可支持检查点；B<g时尤其不能忽略这一项。

新增C后的物理source端点集 `R=R0∪C`，定义

`r_C(q)=min {p∈R : p>=q}`。

新增cut会使一部分此前q的r下降；因此r必须带C，不能固定为旧partition下的r0。

### 1.2 Payload B及真实停车状态

“B可停车”是一个保证：相关DMA拥有容量B，能持有已读取但尚未写出的连续有效suffix，同时释放共享R返回route和两侧当前W head；这个存储不依赖被释放通路继续服务来保住数据，且不与其他DMA重复预约同一byte槽。

B不是拿两个DMA的48B FIFO总配置数直接代入。必须核每DMA真正可用容量、realigner/流水占用、private pending W、共享spill以及当前已占的suffix。B=0可以理解为不允许在一个停车点残留payload；正常流式服务的短FIFO仍可存在，它在该点必须排空。

停车状态是一对 `(p,q)`：完整获取source prefix `[0,p)`，完整交付destination prefix `[0,q)`，`p>=q`，source与destination物理事务均在对应有效byte端点结束，无相关held physical R、未完成source child route或未退出W head；剩余 `[q,p)` 独立停放，长度不超过B。B response可以在另外独立response route上延后，只有在它不会反过来保持目标W/R通路时才排除；若要求整个AXI isolate确认，还须将它和header tracker清退条件加入。

原逻辑source parent可以仍有剩余child未offer，metadata和原RLAST恢复状态继续保留。**释放physical共享R route不等于完成原逻辑parent或整个DMA job。** 当前普通旁路原型也保留parent关联再逐child完成；不能把完成协议偷换为源原parent RLAST。

### 1.3 Immutable offer、在途范围和H

物理AR/AW首次VALID后的字段保持，不能用新的cut撤回或缩短。令E_R为当前所有不可撤销source physical offers的最远结束点，E_W为所有已不可撤销destination offers及W义务的最远结束点；source/目的请求按该copy前缀保序，若有洞则需要完整区间集合而不是单一最远值。

要在q停车，必须至少有 `q>=E_W`；source结束点p至少覆盖q及E_R。因此需要

`p>=max(q,E_R), p∈R, p-q<=B`。

若还未完全取得到E_R，则取最小可完成source前沿 `r_C(max(q,E_R))`；如果E_R不是未来规划中的合法端点，必须加入真实immutable端点，不能照旧R0计算。

`H`在本静态问题中定义为**相邻被选安全目的检查点的距离**：`q_(i+1)-q_i<=H`。它不是DDR周期、TC占用、译址service或总数据beat预算。要从这项几何性质得到请求到来后可达的实际排空范围，还须有准入栏杆：在该cell内，禁止source physical offers越过计划p_(i+1)，禁止destination offers/两侧W义务越过q_(i+1)，并逐一处理已有VALID。没有栏杆则实际E_R/E_W可远超该检查点。

有栏杆且从一个停车点 `(p_i,q_i)`推进到下一个 `(p_(i+1),q_(i+1))`，若每个停车点 `q<=p<=q+B`，则该cell的有效destination服务量≤H，source服务量≤H+B，两者合计≤2H+B。**该量界仍要求cell能实际进展，不包括协议的masked/丢弃byte、额外PTE读、response和队列等待，也不是周期界。** 若H定义为两者合计或read/write各自预算，必须改边条件，不能沿用destination间距模型。

## 2. 必要条件及不能省的反例

### 2.1 前缀守恒给出的必要条件

在停车状态，读取p有效bytes、写出q有效bytes，没有丢弃、重复、写重排或其他保存位置，所以恰好p-q有效bytes必须被保留。B容量只能容纳B，因此 `p-q<=B`。

在没有越过目标的immutable offers时，为完成目的prefix q，source必须到某个物理结束点p>=q；最省的为r_C(q)。因此几何上可支持q当且仅当

`q∈D 且 ∃p∈R∩[q,min(q+B,N)]`，等价于 `r_C(q)-q<=B`。

这是**几何支持**的充要条件，尚不是任意真实实现都能到达该状态的充分进度条件。总是可以读更多但它不会降低存储需求；在上述无洞保序copy中，选择最小r即足够判断几何容量。

### 2.2 已在途请求直接推翻旧q判定

令 `N=8, R0=D={0,4,8}, B=0`。旧式判断q=4满足r(4)-4=0。但若AR `[4,8)`已首拍VALID，或AW `[4,8)`已进入一侧W义务队列，那么在q=4释放相关物理通路不可能：必须把source推进至8或目的推进至8。没有准入栏杆，H=4的计划节点不能约束这个现场。

这也是“原窗口4”的边界：窗口只有项数，并不阻止四个未来parent覆盖很多copy bytes。不能把窗口配平代替byte端点栏杆。

### 2.3 个体几何安全不等于多DMA进度

两DMA的所有R/W均共同等分，B=0，每个q都满足p=q。若一侧W head为A、另一侧为V，已held的shared physical R为V，V不能写，A的必要source数据/PTE又不能越过held R，则仍存在等待环。

这是协议依赖反例，不需要错位burst、不需要LTI。若强普通用准确两侧owner门在首次offer前排除该现场，就恢复一个重要进度条件。几何算法本身没有替代owner/seq、prepared目的许可或共享返回服务证明。

### 2.4 缺目的译址也推翻几何“充分性”

单copy即使q=p、B=0，若source R已占共享返回route，而完成其所需W前还缺目的译址、该PTE R也必须走这个返回route，仍可能不能前进。证明充分性必须保证该destination数据路径已可独立消费，或提供PTE独立进度通路；不能仅根据地址区间把prepared位默认为1。

## 3. B=0：可以保留的准确模型

几何支持条件化为 `q∈D∩R`，即source和destination在同一copy byte位置结束。因为只允许细分source，选一个原来不在R0的内部目的停止点至少需要一个新增cut；各q不同，不能由同一个cut支持，所以这个计数下界可相加。

构造DAG：节点为D；从u到v有边当 `0<v-u<=H`；到达内部v的代价为 `1[v∉R0]`，mandatory N代价0。只保留合法v∈A或v∈R0，0起始。任一路径给出checks，代价是所需新增source cuts数；反过来任何可行source细分与checks都给出该图的一条路径和不大于其cut数的路径代价。故最短路径代价恰是这个受限问题的最少新增source cuts。

如果D里相邻端点间距>H，那么任何source-only细分都不能满足H。必须允许更细destination physical W bursts、增加/改变W通路，或放宽H。这一必要条件来自完整W burst必须交付WLAST，与translation位置无关。

错位周期例：source端点 `{0,2048,4096,...,N}`，destination端点 `{0,1024,3072,5120,...,N}`。如果内部没有共同结束点且B=0、不能改变已offer的物理bursts，那么没有内段停车点。有限maxburst不能独自界定整个copy可停车前沿距离。允许在destination端点细分source即可产生共同点，普通共同legalization也可做到。

这只是准确的分段/容量命题。为普通DAG/DP或缓冲守恒取新名称，不提供机制新颖性。

## 4. B>0：旧节点计费错误与正确几何问题

### 4.1 反例：一个cut支持两个必要stop

以一个合法beat单位为坐标：`N=10, R0={0,10}, D={0,4,6,10}, B=2, H=4`。为保持相邻目的检查点间距≤4，q=4和q=6都必须出现。

旧按每个q不足B便在q切一刀，会收费2。实际只新增 `c=6`：

- q=4时source完整到6，suffix2独立停车；
- q=6时同一个source前沿6，已有suffix再写2，source不需要新burst；
- N=10为强制末端。

真正新增cut数为1，达到下界：原R0下q4/q6都不被支持，至少需要一个新点。新增c6同时改变r_C(4)和r_C(6)。这反例的H是destination有效前沿量；0→q4时会读6，所以不能把它误说成“read工作也≤4”。

### 4.2 正确问题：检查点选择＋窗口刺点

对目的停止点q定义窗口 `I(q)=[q,min(q+B,N)]`。q几何安全当且仅当窗口含mandatory R0或新C中的某个合法端点。正确优化为：

选择 `S⊆D`，含0/N，相邻S的距离≤H；选择合法新增 `C⊆A\R0`；每个q∈S的I(q)都须被R0∪C刺中；最小化|C|。

一个cut c能支持所有 `q∈D∩[c-B,c]`，它不一定是目的端点。mandatory R0端点免费提供同样覆盖。先把r(q)固定、再给每个节点独立代价的DAG模型丢掉了这项共享覆盖；如果使用动态规划，则必须保留已有cut的覆盖/当前source前沿状态，不能把旧node-cost直接改名字继续使用。

固定S时，去掉被mandatory R0已支持的窗口，剩下的是标准区间刺点：依次处理最早的未覆盖窗口，在其中放最右合法cut。交换证明直接成立；这不是新问题种类。

### 4.3 静态纯几何子问题的简单贪心

在§1.1固定合法网格、端点细分相互兼容，且§1.3的H只计destination间距时，联合选择S/C也有简单最优构造：

1. 对当前R0∪C定义safeD。由0开始，沿safeD且步长≤H求最远可达x；把所有免费可达safe点扩尽。
2. 若x=N结束。否则在 `D∩(x,min(x+H,N)]` 中选择最大的**可支持**q*，即I(q*)必须含合法A端点。若不存在，就无解。
3. 选 `c*=max(A∩I(q*))`，新增此cut，再重复第1步。

不能无条件取最大的D点：当目的partial endpoint、source beat网格与B不兼容时，它可能没有合法支持cut。0/N强制端点已作为免费源端点纳入；若c*已在R0∪C，则q*本应在第1步免费扩展，矛盾。

交换证明：设x免费闭合后某个最优方案M仍需要新增cuts。在它的可行路径中，取第一个超过x的节点q_o；它必须由某个新增 `c_o∈I(q_o)` 支持，且 `q_o<=x+H`。故 `q_o<=q*`，`c_o<=q_o+B<=q*+B`。由于c_o合法，而c*是I(q*)中最右合法点，有 `c*>=c_o`。如果c_o<q*，不等式仍成立，因为c*>=q*。

用c*替换c_o：q*可直接从x到达。任何旧路径上 `t>=q*` 且曾由c_o支持的节点，满足 `t<=c_o<=c*` 和 `t>=q*>=c*-B`，仍被c*支持。其他≥q*的节点由其原有端点支持不变。若旧路径首次超过q*的节点为t，它与q*之间的距离不大于它与旧前驱u<q*的距离，所以仍≤H。于是从q*可以接回旧路径后段；新增cut数不增加。故存在包含本轮greedy cut的最优方案，迭代归纳得到最少cuts。

这是**静态几何子问题**的最优性证明。若费用不同、H计总service/周期、cuts可合并/搬移、已经有越界在途请求、buffer被其他DMA动态占用、合法切点取决于之前选择或physical W只能动态失序提交，交换证明的支撑条件不成立，须重建问题；不能把这个贪心包装成端到端在线最优。

## 5. 何时几何支持能变成真实可达停车

下面是一个足以实现给定 `(p,q)` 的受限构造，不是生产已验事实：

- 当前active cell的两侧W序一致，只给该cell的owner提交相关source/目的physical事务；外流已有义务已排除或按兼容顺序完成。
- p/q准入栏杆已经在首次shared physical offer前生效，先前immutable requests都没有超出p/q。
- 每个即将发出的source burst所需的目的数据路径，在held source R期间能独立继续到需要的W边界；可以先准备相应目的译址、保留合法的AW/W路径，或有经过证明的独立PTE通路。不能只看瞬时WREADY。
- stream可以用原短FIFO直通；最终p-q尾部有保证的B私有停车空间，含有效mask/归属；所有必要翻译及memory service在公平假设下最终完成。

在这个构造中，把source读取到p，所有到q的payload流式交付，suffix停车，等待该cell source physical RLAST以及两侧W退出，即可达到目标状态。source parent header/剩余child在metadata中保留，普通原RLAST恢复按真实数据交付执行。

这些条件暴露实现成本：若为“所有所需目的都ready”新增很多header或完整response槽，也应计进cost，不能以纯byte下界声称整个系统最小面积。若准备依然依赖被hold的R，就不能把充分性写成定理。若选择对cell串行提交多个DMA AW，需报告其并行性能代价；不是无损假设。

## 6. Strongest ordinary：同信息、同预算的明确等效构造

普通基线拿相同descriptor/source-dst关联信息、N/R0/D/A/B/H和实时prepared/owner/seq位，使用同一个静态几何求解结果或同一个简单贪心。

它在普通nonuniform source splitter中生成相同physical cuts，保留原逻辑parent和ID/RLAST恢复；目的AW使用相同准入栏杆；缓存准备及完整permission/context/error响应复用，与§39一样绕过繁忙TC。它可以逐次做与候选相同的source/destination physical offer、相同suffix停放和相同停车选择。

上段的逐事件等效构造指内部合法子片、原job语义及相同父header复用。它不能由“Linux改成很多小job”直接代替后就宣称相同parent窗口、DONE、ID和供给费用；**但这不排除software/descriptor tiling作为必须纳入的另一强普通工程控制。** 用户允许改Linux，若真实目标仅为wholecopy成功及H内让路，硬件单job原子语义没有被要求，那么小job拼成macrocopy完全在授权范围内，没有协议理由拒绝。

实际本例的普通tiling候选：N=32KiB、src0/dst3072、源2KiB固定边界、B=24B、H=4KiB，选目的停止点 `3,7,11,...,31,32 KiB`，拆成 `3KiB + 7×4KiB + 1KiB`，共9个subjob。每个descriptor尾部通过普通legalizer截短source/destination bursts；第一个subjob为2个source/2个AW，中间七个各3个source/2个AW，最后1个source/1个AW，所以纯几何合计source24、AW17（每DMA），在原source16/AW17上新增8个source cuts。本例D内部停止点是奇数KiB，B24不能令一个cut同时覆盖间距2KiB的两个D点；第一可取点不晚于3KiB，此后步距最多4KiB，至32KiB至少需8个内部停止点，所以该tiling达到本几何问题的8cuts下界。它不需要新IOMMU splitter。

现有 `idma_reg64_frontend.sv:588–596` 的ADMIT等待backend_idle再offer下一copy，是subjob边界让路的真实候选接口；仍需实际核/验证每个subjob backend_idle包含哪些R/B/W/共享route排空事件。不能把这个静态几何推算写成已运行的24/17或H周期保障。

费用必须另外完整核算：9个subjobs若全部排队超过jobFIFO8时的装载/补给组织；改变descriptor/job窗口；CPU写寄存器/轮询/批次开销；TL hit请求数及重复context/地址准备；macrocopy身份、错误传播与完成合并所需metadata；ADMIT等待完整job退休相对内部physical-cell停车的更强排空开销。同等性能、状态和语义是否成立须用实际实现证据，不用理想cuts数替代。software tiling若在这些真实费用后同保证同成本，应停止新增硬件主机制。

以上说明在相同接口下**决策与资源义务没有候选独占能力**，而非未经综合就声称两个RTL面积精确相等。若候选的求解器或额外certificate确实压缩metadata/组合路径，仍需实际对应RTL与面积/时序验收；目前没有此证据。

因为普通同信息实现可完全复现cuts与栏杆，本问题不能作为独立调度机制；如果此前未有严格相同锐界且后续证明对真实多DMA、错误及有限state有价值，可以考虑分析贡献，但简单守恒/区间刺点/贪心不能自己构成新颖性证据。

## 7. 已核实际与primary，以及明确下一切口

直接核实际legalizer `cva6/corev_apu/iDMA/src/idma_legalizer.sv:137–215`：source/destination各自边界及decouple_rw选择真实存在，共同模式取两侧较近边界。因此共同分段是现成能力；这类几何不能因post-TL位置而独占。

直接primary：[AXI-REALM作者预印本](https://arxiv.org/pdf/2501.10161)，§III-A1/A2 p3已有AR/AW/W分片、B合并、原RLAST恢复，且要求fragmented W burst完整在buffer后才forward AW/W；§III-B另要求互连progress和RR。它已经覆盖普通透明分片和write-data-ready控制，但不能仅凭该段认定共享IOMMU/PTE路径和多DMA组合的所有进度前提已经被证明。

直接primary：[MMU-Aware DMA / Scalable SVM](https://arxiv.org/html/1808.09751v1)，§IV-C已有每burst metadata retirement、same-ID保序、停止新增并等in-flight结束再retry；它使用private SPM，并不等同本平台两个small R→W FIFO的全部共享返回依赖。因此“等待in-flight结束”能否组成安全隔离，本平台应核实际依赖，而不是默认或给新名。

**明确下一切口是composition的释放证书及其真实成本，而不是另一分片算法名。** 对当前四DMA/共享IOMMU，将一个实际可选cell的下面事实定义成可核接口：

`已offer byte范围；两侧W owner/seq；目的有效permission是否已备；source suffix独立free bytes；所有PTE/数据返回是否存在不依赖该cell被阻塞通路的释放路径；原parent关联是否能在release后保留。`

先核这些事实能否从现有RTL低成本得知，再与强普通“完整fragment W buffer＋AW数据齐备门”和“准确owner＋prepared目的＋source边界切分”比较。同信息普通若给相同certificate/进度并且成本不高，停止独立机制；如果存在实际可达状态使普通certificate需要较多buffer/metadata，而新的可核压缩表示或准入规则以更低成本给出同等端到端保证，才有新问题及独有实现的立项理由。

此下一切口必须纳入已首拍VALID但未握手、两个W FIFO、PTW return、error响应及静态映射下就可达的状态；不增添LC、PRI或IOFENCE寿命锁求胜。H几何问题已可按普通构造收尾，本席不建议再为最少cuts扩性能DUT矩阵。
