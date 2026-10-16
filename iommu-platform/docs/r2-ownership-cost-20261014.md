# R2所有权生命周期与完整逻辑成本裁决（2026-10-14）

本轮闭合了具名正常WRITE成功/TL fault范围的生命周期与完整逻辑成本比较：具体普通O-B5S可用相同状态、端口和调度实现R2的提前准备写许可及原序AW/W派发。因此关闭**R2原位头复用/原序许可准备的独有动作与逻辑成本优势扩展**，没有进入性能microbenchmark。此结论不证明发表前作覆盖，也不否定整个共享IOMMU问题，论文未完成。

用户要求先闭合所有权与成本，差异成立才实验；实际goal于10月13日登记，本轮验收跨至日本时间10月14日。root与唯一必要新席实际gpt-6.1-sol/ultra、原生有效上下文475000、fork_turns=none、Codex-only，无额外token预算；旧席未恢复。原两周立题窗口累计。实际状态/工具用量见[goal原样记录](../out/r2-ownership-cost-20261013/root/goal-state.json)，交付核验通过后才goal complete。

## 问题、继承与入口

R2希望让两个bank正常WRITE许可提前准备，物理AW/W仍按原接受序完成；可能的成本目标是复用已有raw头字段与槽，避免多存一份prepared头。前轮只证明普通B5成功动作可表达，尚缺完整字段生命周期、错误W/B与同预算成本。本轮继承8/9月RemotePTW/E2/E3、M0、P/N/PN、Mode7、四来源抽象分析、stage117真实cache/consumer/六案普通交付及前轮源核，不从头表征，不重跑。

冻结范围为有限、公平、合法INCR64/non-ATOP写，完整ID0/4/8/12，bank=ID[2]、每ID上下文冻结；NormalTL成功或fault，每bank至多一个READ或WRITE活动owner，总单请求/单响应、IOTLB16/一个PTW，AW每bank4、TC W每bank16。承诺W必须真实供齐，正常目标最终接受并返回B，源最终接受B。不是包含reset、异步invalidation/remap、ATOP/MRIF/ignore、生产者丢W或新读许可的一般生产安全声明；也不证明某个真实iDMA在目的fault且读门阻挡时仍能供应W的完整任务活性。

只有源码约束的独立动作或完整逻辑状态/端口差异存活，才做一个私有prototype：正例、相邻反例及规则冻结后两留出，最多4条新性能序列、完整任务≤512KiB，不递增压力/延迟/工作集寻胜例。没有差异就关闭此扩展、保存正向设计，避免继续R2矩阵或例行核验。

## 所有权确切交接点

root形成[正向R2设计](../out/r2-ownership-cost-20261013/root/lifecycle-design.json)，独占新席形成[独立普通算法](../out/r2-ownership-cost-20261013/control/evidence.md:28)。关键原件在[root24组原文](../out/r2-ownership-cost-20261013/root/source-evidence.json)与[新席27原件hash/编号摘录](../out/r2-ownership-cost-20261013/control/source-manifest.json)。

|事件|交接后的实际拥有者|能够释放什么|
|---|---|---|
|R2入口virtual AW真实接受|本bank full raw cell、全局(bank,slot)顺序引用、输入W bank-owner|入口前未被R2接受的AW/W仍由上游source owner与AW spill持有；不撤回上游VALID。|
|正常TL请求firstVALID/接受|稳定prepare cursor与cell；每bank READ/WRITE owner互斥|不能回收RAW cell或更改被挂起的完整TL请求。|
|正常TL response真实接受|同cell的实际physical地址与READY_OK/FAULT|WRITE owner退休；原IOVA/DID/PID只有其他需要它们的消费者已拥有付费记录后才可复用。|
|完整AW与target进入内部spill|原生两项header/selection spill；独立3位held(bank,target,valid)|原bank cell和全局raw引用立即释放；held不再读被复用的cell。仍允许8个raw cell加1个移交头。|
|真正外部physical AW或错误sink AW接受|对应目标拥有不可更改的AW/W义务|held结束，才允许后继跨bank头进入内部spill；不能只按序投两个bank后任RR外部重排。|
|入口virtual WLAST接受|全部数据进入bank W FIFO/内部W spill|只释放输入W路由owner，不能冒充真实目标WLAST或B完成。|
|真实目标WLAST接受|正常目标实际消费；错误sink真实消费后生成SLVERR|原生W目标owner退休；错误写不能靠“已经缓存LAST”提前发B。|
|cut侧B捕获到B spill|有序两项B spill拥有完整ID/resp|内部同ID目标信用可转移；旧B实体仍存活，外层B debt不退。|
|真实上游B接受|原始source消费响应|完整ID外层信用/inflight退休；不能把全部B debt强行限定为8。|

同bank每4槽由head/tail/prepare和occupancy确定有效范围。prepare资格是最老有效RAW；TL response后变READY并推进，issue只释放READY，所以pending TL不会指向复用槽。移交后held只存bank/target，不留原cell引用；冻结、无取消且响应恰一次的范围内不需要generation位。范围改变须重做epoch/取消合同。

错误路径保留原生success/error的full-ID目标绑定。较年轻同ID异target AW等旧target B已捕获入有序spill后才通过；若旧错误B尚未生成，则target lock直接阻止越过，不需要新增逐笔B重排池。被捕获B可等待真正上游接受，所有权始终存在。静态两ID各5个B debt例说明不能把W owner8当全局B额度；这不是新原生iDMA运行。

READ/AR旧控制仍保留每bank request118、response185、FSM3，共612逻辑位。WRITE prepare不能在该bank READ REQ/AWAIT/COMPLETE期间另增一个owner。bank响应只有bank回显，没有免费的READ/WRITE区分tag或四个在途上下文。

## 一个必须修正的前轮推断

前轮我把TC内部AW握手当成外部physical AW握手，推出“256>16，所以physical AW必早于virtual WLAST”。这个推断过强：实际内部AW与W均先经过enabled spill，每个spill有A/B两份真实packet寄存器和两valid；W还可在共享AW首次VALID承诺后、AWREADY之前前进。

本轮明确区分NormalTL完成、内部AW完整移交、外部AW首次VALID及真正接受。两bank额外内部W spill data共32B，所以TC加内部W spill是288B；这些也不是完整系统存储。

修正后的条件命题L2′用**首次VALID锁**：当前256拍>16+2=18，最后一写的virtual WLAST完成前，其数据必已越过这18拍容纳量，到达共享W owner；该owner来自共享AW首次VALID。后来bank的AW要等旧virtual WLAST才能输入，因此不能越过已锁的旧共享AW。它不要求旧AW已在virtual WLAST前真正握手。

这是源码条件推导和合法AXI wrapper反例，不是新增RTL/native四DMA/性能运行。前轮原始文件及实际结果不改写、不重跑。L3/current资格独立依赖同bank顺序、输入W锁及准确外部AW接受，不依赖被纠正的内部=外部前提；R1/关闭分支不因此恢复。[精确修正和适用范围](../out/r2-ownership-cost-20261013/root/source-inference-correction.json)。

## 完整逻辑成本差异

比较允许双方合法复用阶段字段、去掉显式rank、提前释放cell、使用原生信用与错误sink。普通算法可以做到同样的原位准备，不能强迫它另复制prepared头或让cell一直活到B。全成本37类别与common未知项见[成本目录](../out/r2-ownership-cost-20261013/control/cost-envelope.json)；root接受具体O-B5S安全低成本编码，未将较高1340位/2W方案作为唯一比较对象。

生产总ID宽4、地址64、物理地址56，具名配置U=1时raw AW154bit、physical AW104bit、raw AR148bit、physical AR98bit、W74bit、B7bit、R72bit；旧local ID2的146bit raw AR/70bit R不能照搬。当前U=1由具名源码配置推得，没有重建或验当前板端bitstream。

|配对状态项|R2|普通O-B5S|
|---|---:|---:|
|8个full raw/prepared union cell|1232 bit|1232 bit|
|READY/FAULT状态|16|16|
|两bank head/tail/prepare/occupancy|18|18|
|8个全局(bank,slot)引用|24|24|
|全局FIFO指针/占用|10|10|
|WRITE TL phase|4|4|
|移交后的held|3|3|
|共享RR/lock保守上界|4|4|
|真实AW接受时push的W bank-owner FIFO|18|18|
|写请求管理所列逻辑状态合计|**1329 bit**|**1329 bit**|

这个1329是状态字段上界，不是总模块面积、最小界或新增1329位。原AW FIFO被新cell/ring替换，不重复计入common；共享RR只计一次。全部prepared时packet有效字段832bit不能替代mixed/raw峰值1232bit，也不能推物理存储缩水。将prepared完整另存8份需832bit只是朴素副本例子的字段算术，双方都可避免，不能归为R2独有收益。

同吞吐合同采用每bank两个header读视图、一个data写口；response优先，冲突时背压该bank的新AW capture。prepare与commit可指向不同项，不是免费FIFO head访问。occupancy/ring-membership比较、phase控制、mux/仲裁/连线均收费；2W更高并发是另外付费配置。18位W选择FIFO在真实入口AW接受push、真实入口WLAST pop，occupancy即已接受前缀；上游已有owner承担未被R2接受头，普通无需另付4计数+1锁。另一个付费source-owner tap普通上界1315位要新增读接口及2位线，R2也能选择它，没有免费窥队列或独有降成本。

公共成本保留原生source/AR/R、READ612位、完整AW/AR/W/B/R两项spills、W字段/队列、全部ID计数/overflow/映射、真实B与error sink、NormalTL/IOTLB/PTW及软件/应用。两者采用同primitive状态、端口、事件和公共组件图，因此：

`R2 =1329 + C_common`，`O-B5S =1329 + 同一个 C_common`。

**具名配对的逻辑状态/资源/端口差异为0。** 某些公共组件内部未在本次拆解数完，但双方实例/参数/字段和端口逐项一致，作为相同项保留而不是当零。物理LUT/FF/BRAM/area/Fmax/energy、真实CPU/应用值和其差异均`null`；没有把未测物理成本称为0。完整37项、十类事件与状态/调度对应见[root完整比较](../out/r2-ownership-cost-20261013/root/full-cost-comparison.json)。

## 验证、交付及结束

新增root一个正向生命周期设计及24组亲核原文，独立新席27原件、普通完整算法、37成本目录与执行账本。字段计算器两次尝试：首次原件103行却请求104，算术前失败，纠正后一次14项算术通过；1329位编码为有界独立人工证明/公式补充，没有重跑未变calculator。root直接核关键源/所有权与配对成本；这是有条件手推设计接受，不是RTL形式验证或板端安全接受。

新增性能序列、RTL仿真、构建、综合、板操作、生产driver/ABI/RTL修改、旧validator/实验重跑均0。没有候选准入，完整任务完成向量/实际新字节RWB测量为`null`。因此无需搭四DMA夹具、调参数或做新性能验证来搜差异。

稳定结论与后继范围见[root裁决](../out/r2-ownership-cost-20261013/root-result-acceptance.json)和[关闭后合同](../out/r2-ownership-cost-20261013/next-goal-contract.json)：这项R2扩展关闭，不继续重复“成本待闭合”文书或自动换题。只有新的原始能力/完整成本族差异不被具体O-B5S实现覆盖，才可重开；物理未知本身不是差异见证。一般机制探索与原研究资产保留，实际工作流/GPU-AI、共享翻译路径及FPGA验证仍是存活机制后的应用关口，不是所有机制的统一前提。

五文档已同步。完整导航在[本轮README](../out/r2-ownership-cost-20261013/README.md)，新交付/源hash/历史/既有dirty保留核验在[交付记录](../out/r2-ownership-cost-20261013/root/delivery-verification.json)。本轮goal完成只代表这次有界判别完成，论文未完成。
