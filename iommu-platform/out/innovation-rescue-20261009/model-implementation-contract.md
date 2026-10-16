# 受限事件模型的实现合同

作者Codex，2026-10-09。承接codex-open-copy-design.md，模型不是CVA6/IOMMU RTL仿真，不测其cycles/PTW/面积。

请用stdlib Python实现独立源文件open_copy_model.py。接口是`python3 open_copy_model.py --output result.json`，一次完整运行输出schema_version、scope、scenarios、checks。结果由实际event状态推导，不写死baseline会停/某策略完工。文件只能落在本out目录，由Codex应用并执行。

## 简化的首版合同

两流分别拥有已成功授权的源PA相同、不可变、普通memory的N拍logical source parent，长度N<=256（一个4KiB内，8B拍）；不同不重叠的输出。permissions/mapping stable仅作为明确前提，不假装执行TL。每流有B拍privateFIFO，两流共一个single-beat shared skid；所有对照相同容量。write-owner全序。source和write准备动作是事件，不是平台周期。

privateFIFO必须是真实payload队列，每个beat同时含logical_index/可非重复deterministic value；W必须取对应head，只在其逻辑index与write burst期望index一致时前进。每sink parent returned只在RVALID/RREADY handshake增加。R写FIFO、W读FIFO、单beat skid持有/释放都具体执行。每flow最终R父exactly一个RLAST；每short/longW exactly一个Wlast、B；AW首offer后即为非撤销义务，即使READY延迟。

每flow最多1个未退休AW；共同阶段q<=B。下一band只有上一band所有成员W/B清完才提交，便于局部正确性；这不是吞吐优化版。AW短片只能在首次offer前选len。实际上游legalizer将负责Wlast/B，不把下游拆分错误当控制。

## 策略（同一数据路径，仅不同controller动作）

1. `independent_long`：两job独立长W与各自物理R，V可优先完成；没有source共享。无pause可source总2N。A暂停点若其长W早已offer，必须继续履行长W，因此其pause tail可能长；不能偷偷撤长AW。
2. `naive_long_lockstep`：两个长W ordered V,A，一物理R向两sink逐拍broadcast，required mask固定3，不使用额外N拍return buffer。应从状态查出容量循环，保留stall snapshot，无超容量，不人为掐mask。属于缺陷复现，不能作唯一strong对照。
3. `closed_short_long_read`：q短W，物理R仍N，每拍固定required mask3，响应可为下一band预送最多B拍（若采用debt-gated首版，必须标明更强闭合控制也获同gate）；允许A pause挡未来AW。无pause应完成；有限pause时V可能必须等A恢复，永久pause时应从运行状态识别停滞。已heldR不准withdraw。
4. `ordinary_tile_exit`：按q拍共同tile发独立physical R，每tile完成后A可退出，剩余V单独长copy，A后续从returned补读。当前tile的旧AW/R必须清完。给予和候选相同的私有容量/短W/授权/恢复cursor/firstVALID合同，不能使此强对照人为卡住。
5. `open_long_read`：一physicalR N拍，required mask每拍入skid时固定；当前mask不随pause撤。A pause后只继续其已offerW覆盖以及已firstVALID的R，下一拍达到A write-end且旧R送清后可不给A返R。V继续原R，下一笔未offerW可恢复长burst；A原rawparent仍live，保存其returned精确offset，resume后独立child读剩余重建父RLAST。

首版open禁止给A任何尚未offerW覆盖的RVALID，故held A R必属于<=q write debt。V若等待下一AW可以背压phys R；为让V继续而给A跳过的beat只能发生在A paused。未pause的A应等其AW覆盖而不擅自丢R；初共同band需在第一次R交给任何sink前offer双方AW。ordinary/closed都获同debt-gate以公平。不要把A quiet等同共享物理R/整logicalparent退休；记录其剩余rawparent。

补充critical入拍约束：在physicalR→skid接收并冻结mask之前，检查所有仍选中active成员的offeredW已覆盖该拍；不能先mask3入skid再等尚未offer A AW，否则随后pause让freeze-mask与RVALID gate矛盾。A pause后的新增R<=q、剩余W<=q，总payload work<=2q；不是周期界。A early resume若还有index gap不能重join原R，应等其退休后cursor补读，计其延迟；包含early/late resume两个probe。

## 暂停调度

primary N=16/17, B=3,q=2。A在达到指定actual returned拍数P后pause，可处band中间；pause时当前AW已经offer必须走完。用受限有限resume（例如事件t=200）允许两个job最后完成，同时记录V是否在A resume前完成。另一permanent pause运行展示open/tile V能够完而A仍live，closed出现阻挡。

根据actual physical child start/end和dequeuedpayload累计source_read_beats/physical_AR_count；不得简单按公式赋值。AW/B/Wlast计数也从事件取。暂停时A debt end与最终停接R cursor由实际记录得到；source读量应满足open等式2N-D（仅当前完整恢复/单次退出无额外buffer场景）。所有策略必输出headers数量，不能只报read节约、不报W碎片开销。

扫描可小而有意义：N=8,9,16,17；B=2,3；q=1,2且q<=B；暂停P=0,1,q,q+1,N-1（去重）。可按可审查实际calls报告；这是model scenario非DUT run或实板实验。

## 并发critical probes（可单独纯state组件）

- 一个R拍已经给A VALID但A READY=0，下一事件pause：old data/valid保持，A最终READY后恰返1次，future beat可退出，不withdraw。
- AW firstVALID但READY=0，pause：len/address保持，完成旧AW，不能生成下一笔A AW；停止边界包括旧offer，不能只算accepted AW。
- 输入源保持VALID时不同sink只接一次；一侧先接、另一侧延迟，delivered mask阻止重复。
- 准入cert guard：PA相同但A READ失败、普通RAM/immutable前提不成立、range/attrs不同均不coalesce。guard不等价实际IOMMU权限测试。
- 如negative-naive停滞，不计整suite失败；必须状态解释FIFO满、谁是Whead、其剩余、source held、可推进事件缺失。

## 验收与限制

最终成功场景逐byte/beat对比两输出，不只长度；容量、稳定VALID、parent RLAST、AW/Wlast/B计数和controller无新A首次offer检查。若模型设计不能满足某点，应交付真实反例和精确修订建议，不删断言/不伪造通过。公开所有状态/内部event实现供根审。

candidate extra state须列实际Python逻辑所代表的硬件字段，估计位数及共享/每flow，明确不是综合面积。无需大量架构穷举/最优声称；源读节约和退出能力可检验，论文新颖性需primary及真实out DUT/cost另证。
