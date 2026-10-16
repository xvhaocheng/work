# A：控制位置与完整任务分析的方法增量核查

2026-10-15；独占关键原文核查。作者与判断：Codex。本文只检查方法前作、普通构件和可发展增量；未运行模型、未修改生产代码、未恢复旧实验。结构化证据见同目录 `analysis-delta.json`。所有页码从 PDF 第 1 页起；TC2023 印刷页=PDF页+153，DWS印刷页=PDF页+625。

**结论：保留并发展 A，定位为受真实接口约束的共享翻译部署判别方法。不能把它包装成新的通用事件图／状态空间求解器。** TC2023 已允许 job 内任意请求时序，能在模型假设成立且请求总量有界时保守覆盖依赖 PTE 返回才产生的请求；它的已发表算法不显式输入依赖、owner、first-offer 与动作位置。补充这些信息可以使实例分析更精确，但“同 C/N/f/T 得到不同结果”只证明增加了有用输入，单独不足以证明新方法。最有意义的版本是从真实接口抽取可检查合同，解释哪些位置能合法改变哪个后继、必须保留哪些已承诺义务，预测完整任务与代价，并使这些结论在未用于设计的实例中成立。

## 1. 直接核查入口

|ID|原件、阅读范围及定位|本文短摘录（每源<25英文词）|
|---|---|---|
|TC|Restuccia 等，*Bounding Memory Access Times in Multi-Accelerator Architectures on FPGA SoCs*，IEEE TC 72(1), 154–167, 2023；[作者PDF](https://retis.sssup.it/~giorgio/paps/2023/TC23.pdf)；本地 `../../research-reappraisal-20261015/analysis/adjacent/axi-rtanalysis.{pdf,txt}`。本次直接读§2.1–2.3、§3.1–3.7，PDF2–8；未逐条机械复证引理。|“an arbitrary temporal distribution of memory transactions”（§2.1，PDF2，txt L125–126）|
|ICRT|Jiang 等，*AXI-ICRT: Towards a Real-Time AXI-Interconnect for Highly Integrated SoCs*；[作者PDF](https://userweb.cs.txstate.edu/~k_y47/webpage/pubs/tc23c.pdf)；同一 adjacent 目录 `axi-icrt.{pdf,txt}`。本次定向读§4.1–4.2、§6/§6.1及Table1；未重读全篇或验作者实现。|“each job consists of a batch of consecutive transactions”（§6，PDF7，txt L847–850）|
|REALM|*AXI-REALM: Safe Modular and Lightweight Traffic Monitoring and Regulation for Heterogeneous Mixed-Criticality Systems*，固定arXiv v1；[原始PDF](https://arxiv.org/pdf/2501.10161v1)；同一 adjacent 目录 `axi-realm.{pdf,txt}`。定向重核§III-A2/A3、III-B及§IV成本。|“fully contained within the buffer”（§III-A2，PDF4，txt L352–354）|
|MASK|Ausavarungnirun 等，*MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency*，ASPLOS2018；[作者PDF](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)；本地 `../../shared-translation-literature-20261006/papers/mask2018.{pdf,txt}`。本次重核§5.3–5.5/§6、§7.4–7.5；复用完整旧卡作导航。|“always prioritizes requests in the Golden Queue”（§5.4，PDF8，txt L906–907）|
|DWS|Pratheek 等，*Improving GPU Multi-tenancy with Page Walk Stealing*，HPCA2021，626–639；[作者PDF](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)；本地同 papers 目录 `walk_stealing2021.{pdf,txt}`。本次重核§IV-A/V/VI、成本与动态tenant。|“at most one walk request from another tenant”（§V，PDF5，txt L375–376）|
|TIGA|Behrmann 等，*UPPAAL-Tiga: Time for Playing Games!*，CAV2007，LNCS4590，121–125；[作者原件](https://pagesperso.ls2n.fr/~lime-d/publis/TIGA-CAV07.pdf)，[另一作者入口](https://homes.cs.aau.dk/~adavid/publications/29-tigatool.pdf)。本轮仅为“通用可控/不可控状态求解是否已有”定向补读§2–4（PDF1–4）；参考文献未再展开。|“partitioned into controllable actions and uncontrollable actions”（§2，PDF1；内存中layout文本 L44–46）|

既有五源的 PDF/txt SHA256 均重新计算；已有账本列出的PDF与两篇核心论文txt哈希一致。新 TIGA 从作者镜像在内存读取 PDF 并抽取文本，不另写原件；确切字节哈希和可重取入口在 JSON。AAU地址的本机下载曾返回代理503，但网页工具成功读完整PDF；最终LS2N作者镜像成功取275161字节。未将任何失败取证记为已读。标准max-plus书的目录入口检索没有取得原书，**不作为已核原文或优先权证据**。

## 2. TC2023 到底覆盖到哪里

### 2.1 已经能保守包括的行为

§2.1（PDF2／155，txt L111–130）给每job的 `N_i^R,N_i^W,B,f_i,C_i,T_i`，明确不限制访问模式，而考虑job内任意时间分布。因而，某次读完成才生成下一PTE、某次翻译完成才释放payload，**都可以是它允许时序集合的子集**。只要总请求量、计算量、在途上限、物理拓扑与服务假设能被正确保守抽象，不能以“到达是内生的”或“没有IOMMU专节”宣称它无法给上界。cache命中/未命中变化也可以用最坏请求总量保守覆盖，代价是丢掉关联性与精度。

§3.3 Lemma7（PDF7／160，L505–548）按周期job预算限制干扰请求数：`Y_time(i,I_l)=Σ_j ceil((T_i+T_j)/T_j) N_j`。§3.4 Lemma8（同页，L499–520及558–577；公式/证明因双栏文本位于前段）用 `Y_outs(i,I_l)=N_i Σ_j f_j` 限制任意时刻的在途干扰。故有限在途、其他任务此前已发出的工作、跨层传递干扰本来就在它的保守包络里；不能把“计入carry-in”或“分析流水”单列为新意。

### 2.2 已发表算法并没有显式表示的关系

§3.2–3.5（PDF4–7）将RR层次、周期预算与在途上限组合成各层干扰数量界；§3.6 Lemma10（PDF7–8，L578–655）按事务类型给流水干扰代价。§3.7 Algorithm1和Eq.(4)（PDF8／161，L591–675）累加各层未重复计数的干扰，最终：

`R_z = C_z + N_z^R*d_NoCont,read(I_j) + N_z^W*d_NoCont,write(I_j) + d_z^interf,read + d_z^interf,write`。

这个算法输入中没有逐请求的依赖边、返回slot owner、已first-offer但未accept的锁定头、控制器可见信息或控制位置的动作集合。保持这些粗参数及拓扑/服务常数完全相同，交换A1/B0所占slot的owner或改变哪条依赖先释放，不改变其Eq.(4)计算。它因此**没有直接回答**“在这个合法前缀中这个位置还能改变哪一项义务／是否值得迁移控制”。这与“给不出任何保守响应界”是两个结论。

不能通过任意给等待时间增加到 `C_z` 就称已直接应用：等待若由相同共享资源反馈决定，需要另外给独立安全界，否则存在循环定义或漏计。也不能把共享IOMMU发出的混合流量直接假作独立物理叶节点：§2.1/§3的叶是有AXI manager的HW-task；若多个逻辑owner在一端合流，必须先证明拓扑/归属抽象保守。能保守聚合的情况应承认其适用，不能人为排除。

### 2.3 有限缓冲和AW/W依赖的准确限制

§2.2（PDF3／156，L179–204）假设interconnect pipeline永不满，并将data phase被manager挂住排在该模型之外，扩展留给未来工作。§2.1（PDF2，L118–122）把读写传播作为独立；§2.3（PDF3，L202–225）对FPGA–PS口顺序返回建模，用经文档/测量取得的MC端延迟常数。Lemma10明确依赖pipeline不满与顺序返回，不能直接套到有限R buffer反压、目的许可阻挡源读、AW锁住W等反馈模型。

这不意味着TC不能处理依赖请求。准确说法是：**请求释放依赖可被任意时序包络吞掉；由有限资源或AW/W因果关系改变服务、形成额外阻塞甚至循环时，必须证明原假设仍成立或给新的安全抽象。** 调节器故意hold请求时，其延期若不在既有模型参数中也要另计。论文不是MIG真实服务或当前平台的时钟保证。

## 3. MASK与DWS覆盖、能力与成本

|前作|已覆盖的实质|需要实际支付的能力／成本|对A的边界|
|---|---|---|---|
|MASK|翻译与data联合调度；PTE Golden优先、Silver轮转帮助data公平，TLB填充和PTE cache bypass协同。§5.3–5.4，PDF8，L867–932。|可修改MC、传递3bit walk-depth标签；Golden16/Silver64/Normal192队列和per-app/per-MSHR计数等；§7.4–7.5，PDF12，L1201–1242。存储估计不等于移植AXI/FPGA的完整资源和时序成本。|不能以“翻译很关键”“翻译+data一起考虑”“分离队列”作为新策略。MASK内MC调度可作更强能力参照；不能假定不可修改MC的外部端口具有等价控制。其论文没有first-offer/accept多边界的部署判别与有限DMA/consumer闭合预测。|
|DWS|owner软分区+空闲借用；正在执行的stolen walk不撤回，owner后来到达可能等至多一个其他tenant walk。§V，PDF5，L351–378；§VI-B，PDF7，L493–516。|tenantID、可路由per-walker queues、完成时跨队列选下一walk、owner映射；192总queue entries仍保留，但改组织及选择逻辑。新增metadata作者记192bit，另需阈值/epoch/运算逻辑；§VI-A/B，PDF6–8，L449–563。|不能把“已启动工作不可抢占”“只控下一项”“局部残余借用阻塞”当新发现。至多一个walk是局部顺序/数量性质；walk还等cache/DRAM，不能直接变成一个周期或完整DMA任务界。|

MASK §6（PDF9，L936–944）明确模拟walker至多64并发threads，并真实生成多级依赖PTE请求；故“有限walker+依赖请求”也不是A首次建模。DWS §VII-D对MASK无限walkers的转述与此不同；实现/版本冲突本次未解决，本文采MASK自己原文。两篇主性能来自模拟；本次未运行其实现。它们的固定入口信息和可操作资源应原样保留，不得只换一个名称作为A新意，也不得由它们缺某个部署接口反推接口已经新颖。

## 4. 邻近AXI工作和标准求解构件

ICRT §4.2（PDF4，L305–325）已经把FIFO优先级倒置变成可随机访问的RAQ并以TCU选择；§6（PDF7，L730–884）把一批连续事务当sporadic job，用periodic server `(Π_i,Θ_i)`分配服务，§6.1（PDF8，L923–976）比较DBF与按完整事务修正的SBF。Table1（PDF11，L1141–1156）给本配置4745 LUT/4184 registers/44mW。故“可选缓冲”“组合调度”“非抢占事务服务”都是已付费已有能力。它没有替固定MIG/现有FIFO自动创造随机选项；有依赖中断的批次也不能未经转换直接当连续可执行job。

REALM §III-A2（PDF4，L323–354）明确AW会保留W资源，DMA从高延迟端复制可长期拖延W；它先收齐片段W再发AW，实际配置含2个AW和1个片段W缓冲。§III-A3/III-B（PDF5，L398–466）给按region预算/周期的外置门控，并以前进保证和RR为条件；§IV（PDF7，L625–670）给参数化综合面积，活动write buffer增加1拍。故“外部调节”“细粒度分段”“AW/W先后有代价”并非A新增。若A的许可/owner合同改变了能否先读源、所需缓冲/排空与完整任务结果，应核这项具体增量，不能只说REALM没谈IOMMU。

TIGA §2–3（PDF1–3／121–123）已经把控制动作、环境动作和安全/到达目标放入状态空间，求控制策略或环境反策略；§4（PDF4／124）把一般部分可观测合成列为该版本的未完成方向。本文据此只确认通用求解器非新；**不据2007工具的限制声称2026仍缺部分可观测方法**。[作者原件](https://pagesperso.ls2n.fr/~lime-d/publis/TIGA-CAV07.pdf)

以下分类是本文对A拟用构件的数学/语义分析，不声称这些构件由A首次提出：

|拟用构件|准确归属及限度|
|---|---|
|固定顺序的event DAG，`t_v=max_u(t_u+d_uv)`|普通最长路/max-plus递推。固定合法顺序后求完成时刻，不会自行证明该顺序可达、可控或buffer有容量。|
|先枚举可达状态／合法事件，再对trace计时|有限状态reachability/model checking的直接用法；加“不允许撤回first-offer”是协议语义输入。不能仅因状态字段多或带IOMMU名字就声称新算法。|
|控制器选动作，环境选服务时序，求forall服务条件的完成界|标准对抗式控制/游戏语义；若只评估固定策略，则无需声称新控制合成。|
|信息到达前不得使用未来值|实现策略必须非预知。对同一观测历史选择同一动作；“每个真实状态各选最优动作”会偷偷给控制器不可见信息。观察映射和检验可实现性本身也是标准建模责任。|
|既有accepted/held工作必须保留，计carry-in；双方+consumer结束|非抢占残余阻塞与完整目标定义。是正确性必要条件，不独立证明新颖性。|
|改变控制位置／owner合同再比较|可能有研究增量的领域问题；价值来自从实际接口推出此前粗模型无法给的部署、必要能力或成本结论，并由真实证据/留出案例检验。通用工具能表达它不等于论文问题已被解决；反过来可表达性也不等于新的求解方法。|

服务给常数时可能精确计算该有界模型；服务给区间时须遍历/证明包络。没有上界时完整任务最坏值可为无穷或未知，不能拿一次模拟延迟代替。存在资源环/死锁时，强行排成完成DAG会漏掉未完成路径。以上都是求解正确性边界，不另单列A创新。

## 5. 最低有意义增量与验证接口

**可发展的最小核心**：把输入从“多少请求+多快服务”提升为有原始接口依据的 `(生成依赖, 有限容量与owner, offer/accept义务, 观测历史, 各位置合法动作, 服务条件, 全部任务完成条件)`，输出可部署/需迁移或付费能力/在当前前缀无可改变选择，以及对应完整任务后果。无需先发明新的通用定理；需要实际完成这套判别，而不是把必要字段列成清单。

“同粗参数不同owner／承诺，方法区分结果”是**必要的输入价值展示，单独仍只是更精确建模**。使它成为有意义方法贡献的最低证据应包含下列连贯链条，不扩成无界基线竞赛：

1. **合同可检查**：给出真实调用链/握手/owner证据和有限状态转换。区分来自硬件的事实、软件ready适配、新增接口及条件服务。每个输出有合法动作/阻塞环/完成trace等可复核证书。首次offer和accept分开，AW债务与W/返回buffer所有权不能事后消失。
2. **给同信息简单方法公平机会**：具名比较TC2023合法的保守应用、预先固定trace的DAG回放、同接口RR/固定PTE优先或已有普通调节。DAG回放能验证既定顺序的时间，但如果缺少合法替换集合不能据此作控制部署证明；若加上同样合同后复现策略，则撤下策略独有性，把剩余贡献明确写为合同/判别/解释而非换名求解器。MASK/ICRT等更强能力单独完整计费。
3. **产生具体可行动结论**：至少一组相同TC粗参数的实例，由可观察owner/承诺或实际依赖差别推出不同的合法动作/最低部署能力；再预测完整victim、aggressor及consumer后果。若只能复述“VALID不能撤回”或重新播放既有零选择pilot，不足以证明方法带来新的认识。
4. **冻结后验未参与设计的情形**：保留四项原有预测的接受边界；对新的具名正例及相邻反例先给判断/时间区间/假设，再看执行结果。出现错误就定位合同/抽象/服务假设，不能事后改trace让预测成立。HERO A1未来准备/B0当前依赖是root正在核的候选入口，本文未读其源码，不代写已验证事实。

这条链验证的是部署判别与预测能力，不要求A击败所有可想象程序，也不要求MC不可改本身成为创新。即使动作被同接口普通程序复现，仍可检验方法是否形成有意义、可重用的新部署结论；若结果只剩常规模型检查器对人工例子的运行，则停止该版“独立方法贡献”的主张，保留合同与边界资产，继续寻找真实部署问题。

## 6. 可核查novelty论证草案（条件版本）

> 最接近的TC2023已对层次AXI、在途上限和任意job内请求时序给响应时间上界（§2.1–2.3、§3.4–3.7，PDF2–3、7–8）；MASK已联合处理依赖翻译与data调度（§5.4/§6，PDF8–9），DWS已限制owner借用中的局部非抢占干扰（§V/VI，PDF5–8），ICRT与AXI-REALM分别提供可选缓冲/组合服务及外置分段调节（各§4/6和§III）。我们拟推进的是：在固定接口和有限所有权资源下，由真实请求生成依赖、信息可见时刻及first-offer/accept后必须完成的义务，判别一个具体控制位置还可合法改变什么，以及需要付出何种部署能力，并预测双方与consumer的完整任务后果。通用事件图、max-plus和状态空间/游戏求解作为已有工具。若这套合同能在相同粗流量预算下给出此前直接应用无法给的具体部署结论，并在冻结的未用实例与完整成本核验中成立，它可以构成有意义的共享翻译方法贡献。当前已核前作支持上述能力边界；A合同的正确性、泛化范围、留出预测与FPGA收益仍待本轮实际验证，不能写成已证结果。

## 7. 未核与root下一步

- 本次没有证明所有可能的实时AXI/部分可观测控制/模型检查应用均未发表相同部署合同；不能用本批文献数量作独立新意通过证据。也没有发现一篇已核前作完整给出A拟议合同与该具体部署判别。
- TC2023全套引理未形式化复证；真实平台到其物理拓扑、RR、顺序返回、不满管线、MC延迟条件的映射必须由root/接口核查席判断。
- MASK/DWS作者实现、64/unlimited版本冲突、REALM/ICRT等价集成和当前平台全成本未验；原文成本只保留原配置口径。
- 新TIGA原文为作者PDF，PDF SHA256已取得；原件未落本地文件（本任务仅允许写本文与JSON）。可按JSON中的URL重取并校验；其layout行号是内存中`pdftotext -layout`输出，不能冒作已有本地txt行。
- 下一步由root完成A具体算法/合同及有界案例；本席不运行模型、不改旧裁决/全局状态。本文接受的是继续发展的精确问题与前作边界，**不是独立创新或性能结论的最终通过**。
