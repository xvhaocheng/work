# PTE准备期读准入：冻结普通控制与数学边界

本席只裁普通控制与数学边界，实际goal为`1791946350`。结论：**在保护这笔已拥有PTE的等待目标下，准备期暂停新payload AR已被O-earlyPause明确覆盖；在下述有序R交换域，是否将一个payload读塞进准备空档，精确约化为O-cheapDebt的标量阈值。没有获得当前copy来源的完整R/W/B收益正例，也没有证明这四个普通规则覆盖完整joint-max/sum家族。** 所有符号式均非运行结果；本席没有运行模型、测试、脚本、构建、validator或旧trace，也没有修改生产/bench/公共文档。

## 1. 来源与授权边界

接受根席已经绑定的Genesys2 staged RTL，不再检查生成IP、netlist、manifest或livebit。DS的`axi_delayer`实例由配置输入/输出延迟，关闭随机stall，连接在IOMMU DS与系统互连之间：`out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1353-1383`；AR使用输入延迟，R使用输出延迟：同目录`cva6/vendor/pulp-platform/axi/src/axi_delayer.sv:58-72,106-120`。已绑定profile记录输入/输出均50，**不把50当PTE的DDR服务墙钟、剩余准备时长或system/UI同域周期**：`out/ddr-service-boundary-20261014/root/profile-binding.json:7-25`。根席负责`stream_delay`计数、spill接收、PTW身份与可合法关断的首次VALID位置，本席不重复该协议诊断。

该参考的活跃DMA0/1缓冲为512拍；FIFO8与AXI在途8仍为容量：`ariane_peripherals_xilinx.sv:1262-1277`。原来源的sole 4KiB copy、最后B后产生下一job、读准入与W owner边界，以及A target2^32/V16 STOP造成的可变工作量，沿用`out/joint-source-choice-20261014/root/source-choice-card.md:38-43,62-76`。不会制造异构512/3源、虚构8个并发job、把PTE响应或R-last当copy退休，或重定义原窗口为固定双方任务。

实际MIG参考只有已接受读的顺序边界：L先接受、还欠k>0拍时，P的首次MIG R不得先于L的最后R握手；不存在已接受read promotion。它没有建立固定读服务时间、跨策略相同读时延、R/W独立服务或固定turnaround：`out/ddr-service-boundary-20261014/root/mechanism-card.md:15-36`。本席只继承该根席已经核过的结论，不将摘要变成新增IP核验。

## 2. 新决策点与共同包装

P是一笔已经由原DS输入侧实际握手拥有、且根席能证明为PTW-PTE的请求；P的共享DDR AR outlet尚未接受。L是一个原容量内已经合法拥有的payload AR，且**在根席选定的新gate下游尚未首次VALID承诺**。本动作只决定让这个既定L首次offer，或保持其原owner直到P在根席绑定的共享outlet实际接受后重新评价。它不重选NormalTL，不撤销已MIG接受读，不改变upstream已经承诺的L身份。

四规则均按以下顺序执行：

1. 已首次VALID/held的grant与完整header持续到原握手；P待准备、历史改变或owner优先都不能撤销它。
2. 原read-admission、fullbuffer证书、AW/W owner、same-ID、R/B/错误退休与原credit优先；未合法的L不能因预测获准。
3. 原native仲裁/公平约束优先；若根席增加公共公平reservation，四规则与candidate必须同用实际定义并计费。本席未假设已有免费跨DS/payload的reservation。
4. 然后才使用下述规则。必要history缺失、class/epoch无效或任何输入/运算溢出，明确回退O-currentDS+RR，不能wraparound。

公平的有效范围必须写清：payload源间的RR沿用原实际接受事件推进；PTE保护暂停只约束**当前已拥有P的outlet接受之前**。持续PTE流下的payload有界等待、以及持续payload流下PTE的有界等待，不由“hold+RR”三个字自动成立。没有根席证明/新增有界公平包装时，这两个全流保证保持unknown；本单P/L交换也不以不合法强行准入解决死锁。若现有R backpressure/owner使P永远无法前进，不能声称暂停终会结束。没有新增one-child/one-burst上限来人为削弱普通规则；本文冻结一次L决策，循环策略在每个后续合法新offer点继续执行同一规则。

## 3. 四个完整普通在线规则

| 规则 | 固定规则与缺项处理 |
|---|---|
| O-currentDS+RR | 在实际相关outlet，当前合法且尚可选择的DS VALID优先；否则给当前合法payload，多个payload按native RR。已held先行。未来PTE与准备中P不算当前DS VALID。DS之间按原实际仲裁，不凭ID假设PTW始终先于DDTW。没有history依赖。 |
| O-earlyPause | 根席认证的accepted-PTE-pending谓词为真时，不首次offer新的L；既有held/已接受payload仍正常完成。直到**这一笔P**的真实shared-out AR接受，或根席认证的原生终结/reset使该owner消失，然后回到O-currentDS+RR。不是spill接收、计数到期或DS VALID出现就清pending。身份未知时不推测未来P，回到O-currentDS+RR。 |
| O-owner | 若L的准入在保持原安全规则后，能由现存owner/reference证书直接释放当前阻塞owner/已拥有路径，允许该L；同样的合法P有直接释放关系时，按原owner优先域及age/RR选。不同owner域不合并成bank标签。没有认证关系时不给owner bonus，使用O-currentDS+RR；不会按未来job、设想中的W尾或仅同bank来判释放。关系冲突/未定义owner优先域时交原native规则，不凭预测强行放行。 |
| O-cheapDebt | 用冻结公共快照计算当前L的**R-last预计完成**`Fhat_L`与当前P的shared-out AR预计就绪`Rhat_P`；若`Fhat_L <= Rhat_P`允许L，否则像O-earlyPause等到这笔P实际outlet接受。相等允许填空档；不增加新的W/B服务动作。此R-last绝不算copy完成。历史无效/overflow走O-currentDS+RR，held/native安全/公共公平始终先于阈值。 |

O-cheapDebt的一个完整、可在线实现的冻结标量估计如下；它是普通启发式合同，不是假装其已经等于实际MIG服务：

- `G[classP]`覆盖同class最近一次正常完成P的DS输入实际AR接受→同shared-out实际AR接受间隔。`Shat_R[class]`覆盖同class最近一次完整读的shared-out AR接受→真实R-last退休间隔，除以其接受ARLEN+1向上取整，最小为一个该统计时域单位。class键、epoch、时间位宽与两端事件域必须由根席冻结并付费。
- `Rhat_P = now + max(0, G[classP] - observed_ageP)`。如果原生已可观察的准备状态提供合法剩余量，可以在根席接口冻结时以该确定量替换G项；本轮不宣称counter到期等于outlet接受。
- 仅统计已经owned且根席证明在该outlet次序中位于新L之前的剩余R拍`n_c`；`Dhat = sum_c(n_c * Shat_R[c])`。L既存ARLEN+1为`n_L`；`Fhat_L = now + paid_dispatch_estimate + Dhat + n_L * Shat_R[classL]`。未被证明为前序的aggregate bank debt不冒充此`n_c`，CDC中接受与MIG接受不能混用。若普通控制只能获得更粗scalar，必须明说启发式误差，不能仍称精确交换定理。
- 每次真实sample正常退休时更新一次对应历史；预测快照固定到本L首次offer/本次决定结束，不在想象的反事实内偷偷更新历史。乘加用足宽或饱和+overflow invalid；elapsed不wrap。history尚未正常产生时不运行阈值。生成history的真实字节、时间与CPU/ABI如适用均计费。

最近sample含队列与backpressure；`Dhat`另加前序债务可能使估计保守，也不证明因果分解正确。这一明示启发式足以冻结对照，不能据此推导物理cycle gain。所有规则允许接入candidate使用的同一**付费**公共信息，但普通实现可以删除自己不用的历史/算术，不强行承担joint rollout成本。

## 4. 精确的PTE目标阈值——只在明确交换域成立

这是抽象定理，不是实际DDR返回律。假设：(i) 公共已接受R前缀完成于d且两轨迹完全相同；(ii) P最早可服务时刻r与策略无关；(iii) L/P在同一有序R服务器上，各自服务a>0/b>0，跨两轨迹不变；(iv) 本交换没有新的W/B、backpressure、其他AR或owner联动改变这些量；(v) 等待轨迹P接受后L可以入队，但在P之后；无假accepted queue promotion。定义`g=max(0,r-d)`。

允许L：`C_P(admit)=max(d+a,r)+b`；等待P：`C_P(wait)=max(d,r)+b`。于是

`C_P(admit)-C_P(wait)=max(0,a-g)`。

因此：

- 若`a>g`，暂停新L保护P的收益方向与O-earlyPause完全相同；joint forecast若只因避免这一前序读债务而选择wait，不能把这次动作作为独有能力。
- 若`a<=g`，允许L不延长P；在本域它填入准备空档。一直earlyPause会放弃这个机会，O-cheapDebt用同一个`a<=g`精确区分正例与fit-gap相邻条件。不需要两份完整完成rollout。
- 精确性来自完整交换假设，不来自MIG只保证读顺序的事实。实际ordered-R只足以禁止已接受L被后来P越过；k拍下界不是上述墙钟差。

这里的“独立创新否决”范围很小且有明确原因：**准备期暂停这一动作已有本轮冻结普通earlyPause，PTE保护/填空档这一交换域的选择又已由一个scalar阈值实现**。没有声明发表前作已覆盖全部joint预测，或复制任务的完整完成问题已被否决。

## 5. 为什么不能把该阈值当完整max/sum定理

最小R-only域就会反驳这个外推。取第4节相同假设，且当前义务只有L/P各自R退休，没有任何尾部；移去公共d，令`g>0`。允许L的两终点为`(a,max(a,g)+b)`；等待的两终点为`(g+b+a,g+b)`。最大值分别为`max(a,g)+b`与`g+b+a`，允许L的max少`min(a,g)>0`。即使`a>g`延长P，joint max仍偏向admit；准备中DS未VALID时，O-currentDS+RR也admit。**这是选择覆盖，不是normalcopy完成收益。** 若g=0，max相等，sum按a与b排序，仍不能只按P保护目标判断。

更强的开放边界也只能写成条件代数，不能冒充source-bound positive。设P完成后经gap h>=0释放一个**已经拥有**、还待本P翻译的Q；Q服务c>0，也在同一有序R服务器。没有其他义务/W/B；a/b/c/h是两个顺序不变的条件参数。wait轨迹在P接受后已允许L入队，所以返回顺序P→L→Q，而不是免费把Q提升到L前。admit轨迹为L→P→Q。移去公共d后：

`C_L^A=a; C_Q^A=max(a,g)+b+h+c`；

`C_L^W=g+b+a; C_Q^W=g+b+max(a,h)+c`。

Q是两轨迹最后终点，故`M_A-M_W=max(a,g)+h-g-max(a,h)`；若max相等，sum差为`-(g+b)<0`，选admit。完整max/sum选择因此精确化为**`min(a,h)<=g`则admit，否则wait**。这是一个明确的cheap双阈值实现，不需虚拟队列rollout，但不是上述已经冻结的O-cheapDebt规则，不能事后将它混进对照取最佳。

例如保持`a>g>0`，只比较符号条件`h_high>g`与`h_low<g`：joint分别wait/admit。若没有认证blocking-owner关系，O-current与O-owner都admit两者；O-earlyPause与O-cheapDebt都wait两者。由此可见四规则不是完整joint家族的数学覆盖证明。这一pair**没有被接受为正例**：当前copy实际有流式R→W、AW/W owner、最后B与新job/STOP；没有证据允许删去这些义务，也没有证据把h及a/b/c固定为跨顺序常数。PTE的已配置return delay只贡献h的一部分，不能授权一个固定独立尾部或把W/B视为另一免费处理器。没有代入任何人为cycle数、固定walk长度、假队列满态或免费history。

要把这种开放边界变成可接受positive，必须同一现存来源证明当前P和L/Q的原owner/reference、正常达到新gate、两策略下同一完整owned工作、真实R/W/B释放及退休图、可在线获得的历史和有效误差边界。原A/V窗口的策略相关新job与STOP/drain成本也要保留。根席没有这包证据时，数值完整收益保持null，不能先做模型再补假前提。

## 6. 完整成本边界

继续使用`out/tl-memory-feedback-20261014/ordinary-cost/cost-interface-card.md:7-51,53-93`的37分类；仅借分类与原字段寿命，不借R2旧整数或架构。这里新增动作不是免费一根gate线：

| 成本项 | 各规则/候选必须实际支付的差别 |
|---|---|
| 原生容量、header、hold、R/W/B与错误 | 四规则/candidate继承同一存活owner及完整header；不加payload、结果、walker或credit。Delta payload容量按本授权不增加；这不等于LUT/FF/面积为0。 |
| P pending与身份 | earlyPause/cheapDebt/candidate需正确PTW-PTE输入接受、准备中身份、与shared-out接受关联及生命周期；一个pending flag下界不等于完整identity/epoch/phase编码。currentDS不需要未使用的未来P状态。 |
| 新gate及稳定offer | 判断点必须在根席证实的新downstream firstVALID之前；native spill/header能否继续持有、READY路径/仲裁lock及组合环/时序须根席核。不得以一个新shadow header免费跨周期预测。 |
| owner证书 | O-owner需真实input/output W owner、same-ID/target等关联读取/tap/match；candidate的完整图也需付同项。bank名、DID或host scene标签不足。 |
| debt/退休观察 | cheapDebt需要public前序剩余R拍、L LEN、R握手匹配；candidate还需所有使用的AR/AW/W/B与job边界。重复ID需已有合法sequence关联或新增tag，不能以TL owner替代全部任务身份。 |
| history与时间 | class/valid/overflow、接受timestamp、样本更新、context失效；获得sample的真实工作时间/byte也算成本。candidate未来PTE count只能历史估计，不能读oracle。earlyPause不被强加不用的history。 |
| CDC与snapshot | DS接受、shared-out接受、MIG R退休可在不同域；付事件CDC、同步延迟/带宽、计数一致性与多事件同沿冲突处理。系统侧AR接受不能冒充MIG侧已服务。 |
| 端口/算术/扫描 | scalar需真实乘加/比较/饱和、history读写与匹配端口；joint需其两轨迹真实虚拟phase/remaining、max/sum、递推、mux、scan上限。不能只比较bit数。可闭式约化时按真实闭式实现支付，不强制最贵rollout。 |
| 公平与年龄 | native可复用字段只按真实寿命扣除；新增reservation、age/overflow、消失/替换判定及端口必须计费。未实现跨类有界公平保持unknown。 |
| 决策延迟与关键路径 | 记录`L_select`、新增offer启动时间、数据/READY路径fanout与Fmax。复杂预测或scalar都不能无证声称0cycle；缺historyfallback也不自动和native周期开销相等。 |
| CPU/ABI、软件工作量 | 若history注入/维护需软件，计CPU、同步/ABI与获取成本；原启动、V观察、STOP、drain及策略相关A工作不能删除。纯硬件实现未冻结，不假定CPU代价0。 |

所有实现宽度、端口总量、`L_select`、bits总量、LUT/FF/BRAM/面积/Fmax/energy、CPU、应用时间及固定双方完整净收益均**null**。共同观测可以真实共享一次，但要另给每规则最小可实现成本；普通控制不承担自己未用的joint历史/virtual state。根席的actual source/hold/ownership接受之前，本合同只裁规则边界，不接受hardware implementation。

## 7. 稳定裁决与后继

本席接受：四个明确在线规则；PTE目标域的earlyPause动作覆盖；有明确假设的cheapDebt精确阈值；R-only max/sum不能外推为PTE保护阈值；完整费用必须包含观察、history、ports、CDC、选择延迟与CPU。

本席不接受：当前source-bound完整copy positive、真实DDR固定服务law、四规则对full joint家族全覆盖、数字收益/成本/面积/Fmax/CPU、发表新颖性或A/B准入。因为当前证据只给顺序约束和已有准备路径，缺少同一正常来源的完整R/W/B反事实，**没有可信full-completion positive可交根席接受**。root若最终选择停止，应只停止本次准备期PTE等待贡献，不全局否定共享IOMMU问题。

前作边界：O-earlyPause/O-cheapDebt是本轮明确定义的普通controls，不宣称已发表。此前MASK、Johnson、O-turn等边界不扩大；本席未新增前作检索或把数学约化当论文覆盖。若后续完整依赖例子通过，准确前作仍由根席另核。没有原始新增依赖证据，不重开F1/R1/R2/R107/C112，不自动扩来源/扫相位/跑旧数据。
