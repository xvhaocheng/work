# 承接既有成果的机制续推：两项规则和入口裁决

本轮完成有界机制设计与普通强对照裁决：R1关闭当前动作/性能主张，R2保留为具体重设计问题，没有候选进入新性能实验。已有微benchmark、平台、E2/E3、完整任务测量、Mode7和stage117结果继续有效。论文未完成。

用户已批准先探索机制，也允许冻结条件的新合成微benchmark；实际工作流验证后置。本轮没有把“先拿到真实工作流”设为所有机制探索的入口，也没有重启8/9月已做的实验。实际goal已登记且无额外token预算；完整交付核验后才完成，状态与实际用量见[工具原样记录](../out/mechanism-continuation-20261013/root/goal-state.json)。

## 具体问题、已有证据和条件

问题是四个实际DMA共享两bank翻译控制时，源身份、写地址接受/翻译/派发次序、写数据前进和返回容量之间的关系。目标是找出普通方法不能等价表达的安全动作，或同信息、完整逻辑资源预算下更好的成本点。

已有证据包括RemotePTW/E2/E3、M0完整任务测量、P/N/PN与旧反馈、Mode7的两实际DMA/48B活跃payload结果、四来源抽象分析及stage117真实缓存/所有权/六种普通交付控制。本轮继承这些资产，不把四来源抽象模型或生产四实例当作新的四实际DMA测试。

进入条件：先亲核实际调用链和额度，root最多提出两条具体规则，独占新席提供完整普通算法。只有有源码约束的可达动作/完整逻辑成本差异存活，才选一条做私有模型，冻结正例、相邻反例和规则后做两条留出，共最多四条序列。IOTLB16项、一个PTW、原生队列，完整任务不超过512KiB；不得递增延迟、工作集或到达压力寻找胜例。

结束条件：明确推进/重设计/归档，保存规则、原始证据、完整成本类别及未知量，同步状态、总计划、局部计划、交接和验证，经保留性核验后实际goal complete。没有候选时不自动建夹具、加矩阵或改成表征论文。首轮最多两个工作日，计入原立题窗口，不重新计算两周。

## 本轮新增原始事实

生产参数确实是四个iDMA；local ID固定0，四路mux后的完整ID为0/4/8/12，DID10/11/12/13。bank选择bit2，因此bank0包含DMA0/2，bank1包含DMA1/3。**bank不能替代完整源身份。** 每bank有4项AW、4项AR、16拍W；每bank一个活动翻译owner，两bank共享单请求/单响应正常翻译口和一个PTW。

关键源入口：`ariane_peripherals_xilinx.sv:870–873,1230–1281,1400–1411`，`rv_iommu_banked_ctl.sv:162–164,280–309,314–408`。完整编号原文与源码hash见[root源码证据](../out/mechanism-continuation-20261013/root/source-evidence.json)；独立强对照另保存31个关键原件hash。

输入demux第一次选择AW时就承诺W归属，停住的VALID必须保持；这不等于写许可完成。跨bank接受AW要等旧bank的虚拟WLAST释放写锁。TC输入W可先存入有界FIFO，输出W必须等真实physical AW握手产生credit。当前TC的翻译结果与原AW头绑定，COMPLETE到实际AW派发才释放，不能假设已有任意访问/提取准备头接口。

由这些源结构得到三个范围明确的命题：

|命题|结论与条件|不能外推的范围|
|---|---|---|
|L1|正常成功写在同bank按AW输入接受序派发；FIFO、单owner和COMPLETE退休保证。|错误、取消等新接口行为。|
|L2|一个bank run最后一笔写长度大于未获AW许可前的TC W容量时，必须先取得physical AW才能接受虚拟WLAST、切换到另一bank。当前共同分段4KiB任务的256拍burst满足256>16，因此正常成功AW的全局序不逆转。|任意短包、ATOP、错误路径、更改spill/容量。|
|L3|T仍是全局虚拟W队头且其准确physical AW已握手时，T之前的正常成功physical W必已把全部payload送入TC；有限、公平W服务可独立排完。|任意future读、同ID读乱序、无限背压下保证完成。|

L3的证明不要求所有AW全局同序：同bank前序由L1及虚拟W队头次序保证已供齐；异bank较早接受者由输入写锁保证已供齐。异bank较晚接受者若先于T派发，必须已接受T的虚拟WLAST才能切bank，与T仍是虚拟W队头矛盾。这里“供齐”是进入TC，不能冒充DDR完成、B退休或job_done。

一般AXI wrapper里，短W完全缓存后可解开输入bank锁，让另一bank先完成翻译并先发physical AW。这是未运行的条件性合法事件推导；真实四iDMA短包/独立分段的完整时间序仍未知，不作为性能准入见证。

原测试CONF=0共同分段不支持把一次source R随意跨成两个独立目的段。生产CONF.bit0=1允许独立边界，但需核实际时间序。另一个关键修正是：实际`RAWCouplingAvail=0`走独立AW bypass，**不能仅凭decouple_aw=0认定AW必等第一拍R**。这些源事实支持继续提出准确假设，不能冒充已有四DMA运行。

## 两项root规则与普通强对照

|规则|新增能力提案|完整普通比较|本轮裁决|
|---|---|---|---|
|R1：事件维护写前缀证书|按实际AW/WLAST维护读的身份、epoch和前序供齐资格，派发时查证书。|Mode7原本就是O(1)的current/紧邻next查验；L3及普通current虚拟W头+准确已接受目的AW覆盖所称current安全动作。|归档这项动作/性能主张。没有新增安全动作，也没有相对该强控制的完整降成本见证。|
|R2：提前准备目的写许可，按原序发physical AW|有界接受AW后，两个bank正常翻译可提前进行；physical AW与W按原接受序承诺。|独立B5：AW接收FIFO、每bank正常准备owner、全局FIFO头issue、W disposition及按ID的B退休。成功路径状态映射表达同样的issue动作，FIFO位置即可代替显式全局序号。|保留重设计。成功动作已被普通算法表达；完整异常/同预算等价和实际成本未知，没有准入性能实验。|

root保存[两项具体设计卡](../out/mechanism-continuation-20261013/root/design-cards.json)，新独占Sol/ultra席保存[独立普通算法与证明](../out/mechanism-continuation-20261013/baseline/evidence.md)。B5是手推可执行普通控制，未实现、未发表查新；其成功状态映射不证明完整异常等价，也不证明具体方法已被发表前作覆盖。没有用“别人也可复制这套模块”作为发表新颖性否决。

强对照不仅有B5，还包括B1 Mode7、B1v普通current门+B1 next、B2共享可选头队列及旁路、B3每DMA头队列、B4完整响应预约。每项保留原生在途额度、合法字段回收、稳定VALID、同ID和结果归属，不人为单child限流。全部算法、容量、约束及37项成本字段见[baseline合同](../out/mechanism-continuation-20261013/baseline/baseline-contract.json)。

特别注意：现有无扫描准入控制仍执行完整前缀扫描作为fault诊断。历史两组CSV相同只能支持决定/轨迹覆盖，不能证明诊断逻辑在综合后消失或面积更低。

## 成本与所有权边界

|项目|能由当前原件确定的值|范围|
|---|---|---|
|生产四DMA活跃data buffer|[512,512,3,3]拍×8B=8240B|仅data；只有前两路是大buffer/launch配置。|
|假设四路都3拍|96B|没有实施或验证；不得称为四DMA新结果。两DMA48B是旧资产。|
|DMA last标志FIFO|原生产共1062项×2bit=2124bit；全小假设44项×2=88bit|MetaFifoDepth=BufferDepth+8+0；只计算这一个逻辑字段族，非总面积。|
|公共TC W|32拍×8B=256B，另有strb/last/user|48B或96B均未包括它；test字段非data共320bit，不能无核套用全部生产宽度。|
|B5全局AW FIFO引用|8项×(bank1bit+bank-slot2bit)=24bit，另加指针/占用|复用bank4+4前提是完整生命周期成立；不能免费借成单bank8项。|
|R2 ticket字段示例|24bit bank/AWdone/Wdone，加指针/占用16bit，共40bit|仅字段子集，未含held、错误、B、generation、所有header/端口；非净收益或总面积。|
|完整header与B退休|test raw AW154bit、physical AW104bit；新header须全计；native demux为完整ID的4bit计数/OR-full|不是全局16笔或只能8个B debt。4源localID0给4×15记录是保守tracker上界，非已测工作负载并发数。|
|物理面积/Fmax/能耗、原生CPU/应用成本|`null`|未综合、未板测、未执行新完整任务；不得用字段相加或宿主耗时替代。|

完整类别还包括raw/prepared AR/AW、结果字段、job/epoch/segment/源ID、W与B归属、错误W消费、迟到TL归属、armed/held头、并发读写口、宽mux/回放、比较/仲裁/fanout及软件供给/poll/ownership。对应可证算术见[root成本边界](../out/mechanism-continuation-20261013/root/cost-boundaries.json)；未知量逐项保留，不报成零。

R2接收头在实际准备/派发完成前一直有owner；复用字段必须保留仍活跃的W/B/TL/generation义务。physical AW第一次VALID后不可取消或换头；错误写不能删除已承诺的W，需真实consume/drop全部W后有信用地保序发B。当前生产准备结果不能在COMPLETE尚活跃时免费提走。全错误/cancel/reset/invalidation/ATOP行为尚未证明，因此没有一般接口无死锁或生产安全接受。

## 实际执行与下一步

本轮新增两项root设计、六项普通控制合同、24组root编号源码原文和独立31原件hash。新席的静态合同/算术检查一次通过：六控制各37成本字段、hash及256>16条件通过。该脚本不证明手推状态映射或完整异常协议；这些由独立推导与root关键源码审查限定接受。

新增RTL仿真、性能序列、构建、综合、板操作、生产driver/ABI/RTL修改均为零，旧实验和旧validator重跑为零。完成向量、字节/协议新运行结果为`null`，原因是机制准入门没有通过；不是零周期/零成本成功。

root及唯一新强对照席均实际gpt-6.1-sol/ultra、原生有效上下文475000，fork_turns=none；不调用Astra/DeepSeek/Claude，不恢复旧席，不修改配置/缓存/历史以伪造模型或窗口。该席已结束。运行记录见[root](../out/mechanism-continuation-20261013/root/runtime-root.json)与[新席](../out/mechanism-continuation-20261013/baseline/runtime-metadata.json)。

下一步只保留一个R2重设计问题：**提前准备许可能否在原生所有权和完整逻辑状态/端口预算下，形成一个普通B5无法等价处理的有用完成或成本点？** 先固定目标、owned raw/prepared头生命周期和实际时间序，明确与B5的差异；同样成功动作且没有独立完整成本差异，就结束该扩展。物理面积和Fmax可到原型阶段测，不把全部生产异常或原生应用先验作为所有受控success-path模型的入口。

只有这个差异存活才做最多四条冻结序列；之后再评估GPU/AI工作流的任务、依赖、生命周期、字节和实际共享翻译路径，再决定现有FPGA关键验证。没有自动登记后继goal或搭四DMA矩阵。[下一范围合同](../out/mechanism-continuation-20261013/next-goal-contract.json)与[稳定root裁决](../out/mechanism-continuation-20261013/root-result-acceptance.json)是后继入口；R107/C112及其他关闭收益主张继续关闭。

完整导航见[本轮README](../out/mechanism-continuation-20261013/README.md)。五文档同步及旧工作保留性/新产物一致性核验见[交付核验](../out/mechanism-continuation-20261013/root/delivery-verification.json)。历史原件原样保留，当前机制先行规则只按用户新授权替换当前入口，不改写历史科研裁决。
