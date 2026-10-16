# §45：root 设计的固定链判别模型

范围：两个同时存在的有限普通翻译请求，一条可切换 PTW，静态正常三层S/三层G、全部4KiB叶，DDT/PDT已解析。请求按PTE服务数计，不计cycle/DDR带宽；无PWC、无新增旁路。每笔读只推进当前请求，不允许跨请求共享PTE结果或合并读取。该有限图只比较列明的普通保存primitive，不是所有编码/全部架构方案的最优性定理。

## 1. 实际15步链与保存状态

位置 p 是已完成的有效 PTE 数，不是另一个免费的寄存器；可保存p只能通过下述已付费tag+payload恢复，模型只用它抽象该token。原请求/context C、active完整执行寄存器、owner/valid/终态归属成本双方同计，不在本模型的附加suspended frame预算B内；它们不是免费硬件，也不从B推出总LUT/面积。

链：G1,G2,G3,S1,G1,G2,G3,S2,G1,G2,G3,S3,G1,G2,G3(done)。

当前完整token的必要payload表（沿用§44完整IOTLB输出合同，未来错误所需target保留；没有运行fault场景）：

`data_bits[0..15] = [0,44,44,44,30,74,74,74,30,74,74,74,33,77,77,0]`。

这里44为current physical table/mapped S-PTE page；30为S-parent GPPN29+global1；33为Sleaf GPPN29+属性4；非root时加Q44。active信息只能导出当前token、当前S-semantic-root token或原请求C，不能免费回到任意以前的Q/PTE。

## 2. 五个具体普通primitive族

1. `whole_restart`：非终态仅回到0，无suspended进度payload。
2. `full_current`：当前p的data_bits+固定5bit generic phase tag，符合§44 27-state编码；空间不足可回到0。不能把该上界当最小编码。
3. `semantic_restart`：p<12时b=4*floor(p/4)，p∈[12,14]时b=12。b=0由C恢复；b=4/8要30+3=33bit，b=12要33+3=36bit。保存b丢当前G子进度，保留已处理S前缀；B不足可whole restart。
4. `mixed_indexed`：当前p用data_bits+4bit标签（限定固定15步链的14个非终态位置），也可保存b，用该族同一个4bit标签，或回0。标签足以区分phase和level；不计成支持大页/其它路径的27-state通用表示。
5. `semantic_chunks`：普通G整段返回后存mapped S-PTE page，下一turn只读S后可提前交接；只支持当前checkpoint集合{3,4,7,8,11,12}，payload按上表、统一3bit tag（6个记录状态；0是C/init，15为终态）。其它p只能回0；这是具体q≤3中间点，不是新编码概念。最大74+3=77bit，C/valid/owner/active/终态另计。

保存b在mixed中只是保存同phase root token，不额外隐含mode；generic与固定链编码不能混着拿最低成本。0与15的身份/valid/terminal状态另有双方同计的真实存储。

## 3. 合法服务和强制交接

图节点为(pA,pV,turn)。两请求均未完成时，当前请求可执行x∈[1,min(q,15-p)]笔正常PTE，然后必须交接给另一个未完成请求。q是最多连续服务数，允许较早主动让路，不能强迫普通方法填满q浪费工作。

执行后到p'=p+x。若p'=15，其terminal按合同交付并标done；否则将它按相应族的合法token保存为p'或b(p')或0，且token位数≤B。下一请求从其已保存token恢复。读的边权为x，放弃的进度需以后真实重读；禁止保留额外未计费的active-prefix、任意以前的checkpoint或oracle PWC。

另一请求已经完成时，剩下请求可连续完成，因不存在要等待的同伴，不再强制q切换。观察的是两个有限请求总PTE工作/完成可达性；不能把它扩为持续到达有界延迟。

## 4. root 命题与停止条件

令S_B是该族可直接保存的当前checkpoint位置，加0和15。相邻S_B点的最大间距记G(B)。root推演：在上述固定链、可主动早切换、无其它历史/PWC情况下，两个有限请求可完成 iff q≥G(B)；可完成时按这些checkpoint单调轮流即可总读30笔，没有必要restart。若gap>q，在first completion发生之前，任何请求都不能跨过这个gap形成可保留进度或终态；restart只回退，不提供新的点。

两席已交叉质疑，root的受限证明见`codex-proof-and-decision.md`；routine脚本只对有限图交叉检查，不作为通用理论证明。模型分别求最短完成路径、无解例的非进展cycle、S_B最大gap预测；若有不一致，保留原始诊断给root，不偷偷改模型/降低合同。命题是存在可完成策略；仅给max-q和PTE服务公平，并不保证任何合法策略都完成。

若命题成立，此固定已知链的frontier退化为普通checkpoint选择/路径图，不推荐独立创新。反复restart反例仍合法，但不能强迫普通每turn用完quota，再宣称它无法持续前进。未知大页/动态fault/外生即时交接、多context pool/PWC另外分析，不用这些未覆盖情形拯救本例的创新结论。

## 5. 最小执行规格

routine worker提出单个标准库Python脚本；root审查后只运行一次。枚举五族、q=1..15、B=0..85的有限模型实例，输出JSON与代表性路径/cycle及简洁表。Dijkstra/有限图与独立最大gap公式比较；原始counterexample与counts保留。6450是数学配置，不是DUT/FPGA实验次数。无RTL/Linux/BIT/板卡操作。
