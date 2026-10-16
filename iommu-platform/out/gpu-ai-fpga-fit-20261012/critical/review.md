# 独立适配裁决：AI来源的有限权重准备可以研究什么

**接受root拟定的完整OPT125m MLP权重包静态／逻辑功能合同。它不需要真实compute；真实两iDMA功能验收和强普通对照可执行后，可以登记一个明确限定范围的transfer-stage G0。当前没有AI应用失效或G1证据。**

源衍生准备任务的结果是“六个必要张量都已正确到达指定最终目的，可以由消费者使用”，这本身是有限、有用的服务完成对象。不要求商用GPU或OPT6.7B完整上FPGA。但不能把这个结果写成生成了token，也不能把两个DDR搬运角色称为真实GPU／NVMe协议复现。

详细机器可读合同见 [independent-fit-gate.json](independent-fit-gate.json)。本席亲核关键源码和既有stage100/101边界，没有跑新模型、构建、板测或外模型，没有改其他文件。

## 1. 来源和当前控制边界

固定作者源码的MLP确实是六张量整包：`fc1.weight=(4h,h)`、`fc1.bias=(4h)`、`fc2.weight=(h,4h)`、`fc2.bias=(h)`、两个layer-norm向量各`(h)`；全部进入同一weight-read-buffer，后续MLP共同消费。[MLP源码](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:477)。本轮来源席归档的作者配置明确支持OPT125m的`h=768`及float16。[模型配置](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-fpga-fit-20261012/source/sources/opt_config.py:65)。

这给出完整包`16h²+14h=9,447,936B`，其中两矩阵各4,718,592B，`fc1.bias`为6,144B，其余三个向量各1,536B。来源约束是整包和依赖，不能只保留大矩阵或把bias尾段当无用字节。

源程序已有首层准备、下一层权重预取与逐层`disk.synchronize()+cuda.synchronize()`。[源循环](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:1011)。disk backend是memmap加CPU relay worker；DISK标签、enqueue和queue-join并不单独证明物理SSD DMA或CUDA copy已经结束。[搬运后端](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:791)。因此不把源预取当新能力，也不把同一GPU的多个CUDA stream拆成不同Requester。

现测试wrapper有两条真实`dma_core_wrap`，DID10/PID20和DID11/PID21，真实请求mux及cfg AXI。[wrapper](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_top.sv:12)。private供给可以在读取`NEXT_ID`之前选择是否发布下一copy；读出的reserved ID与真实DONE匹配。[触发和完成](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:1499)。external hold/go在这个wrapper绑零，不能臆造撤回已发布copy的能力。[绑零位置](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_top.sv:389)。

既有full-batch不能直接换标签：它把`ready[j]`定为本通道前一笔DONE，并固定262,144B与64+64。因此它验证的是两个独立环，不是`同tensor全部fill.done→copy_i.ready`的跨Requester依赖。[现有断言](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2957)。应复用硬件完成／守恒／quiet校验，私有适配DAG供给和实际包计数。

## 2. 最低可执行合同

1. 固定完整六张量manifest：dtype、形状、排序、offset、source/destination地址和原始字节hash。传输功能可用可复算fixture数据，但声明它不是预训练权重，未产生数值推理输出。
2. source、wholepackage relay、最终destination是三个不重叠DDR区域，各9,447,936B，净载荷28,343,808B；额外alignment、guards、PTE/DDT和metadata单列。wholepackage relay可以避免人为两小slot制造缺口，但它不是原四个巨大pinned worker的忠实复制。
3. 两个DID分别固定为fill与copy的不同物理Requester角色投影。fill将数据送到relay，copy将真实ready的relay送到最终destination；存储命令／CPU relay／CUDA／GPU内部MMU未实现。多个GPU搬运任务共享同一个copy DID。
4. 最大copy chunk为4KiB，尾段精确计字节。按tensor独立切块每hop为2309笔；若按整包连续切块则为2307笔，必须冻结一个口径，不能混用边界。新任务不用旧64笔参数。
5. 同一tensor全部fill chunk真实完成后，该tensor任一copy chunk才ready；保留源tensor smart-copy的整tensor提交边界，不能仅凭本chunk完成提前copy。同一range的后续写须等原reader完成。wholepackage relay范围互不重叠，所以**tensor内A→B屏障不能被扩成整个任务只有全局串行调度**。普通pipeline可同时填另一个独立tensor、拷已完整填好的tensor。仅用于功能的串行seed须明确未验证性能或最强普通调度。
6. common t0后实际发布、配置、等待、搬运和排空都记入。最终目的六张量全部字节到齐且正确是package-ready；所有已经接受的DMA、TL、DDR事务自然完成和quiet另记return。两hop有用工作仍为9,447,936B；逻辑copy字节18,895,872B，DDR读＋写37,791,744B。PTE、relay副本或padding不能增加有用工作。

原始ready时刻应由source/依赖／ownership决定，不等于scheduler实际issue时间。区分逻辑ready、NEXT_ID触发、硬件issue、硬件DONE、包ready和quiet；超时／错误是验收失败，不当科学阴性。不STOP后台或截去长尾。

## 3. 公平普通对照

至少保留合法两hop FIFO pipeline、用当前完成／ready／ownership反馈选择下游或推迟下一fill的普通控制，以及可行的resident/source-direct一hop。首fill或wholepackage提前准备允许；搬运准备的时间不能藏在计时起点外。普通控制获得与候选相同的manifest和当前事件，已有metadata／inflight额度可合法复用，不加人为single-child或固定字段用途限制。

**当前source、relay、destination全在DDR，没有事实证明relay不可省。** 若合法resident source能让copy直接准备同一最终包，这就是必须执行的强普通一hop。要声称真实AI两hop是必要条件，必须另有实际源域／内存驻留／介质需求事实，而不是引用执行服务器的RAM容量，也不能先把源数据放DDR，再禁止它被直接读取。

大页是合法辅助：核真实IOMMU leaf和物理alignment／权限跨度，允许2MiB映射、布局及提前准备；计独占保留、allocation/map、额外授权、quiet/invalidate/config/reheat。CPU hugepage名称不证明IOMMU大页。对某mapping计算shared slowdown须有匹配mapping对照，不能跨4KiB／2MiB减数后统归PTW。

反馈只能影响尚未发布的copy。已承诺VALID和已issued事务继续完成，不能撤回；mode/page切换的drain和重热计成本。无deadline时可以比较有限包准备时间，不凭空添加首笔期限。准确prior-work、新规则与普通反馈分别判断；“现有控制也可以装进新规则”不是前作覆盖证据。

## 4. 进G0和只算微基准的明确条件

**现在足够做静态／逻辑功能交付：** 完整源支持包、有限工作、每hop依赖和字节成本已经可以冻结。逻辑checker可以查覆盖、提前读／复用、丢尾／重复、hash和abstract quiet。它不能证明真实RTL issue/done、shared translation或板端性能。

**足够登记有限transfer-stage G0的条件：** 私有DAG adapter驱动现真实两iDMA，cross-role ready来自真实完成，reserved ID／事务归属／AR→R／AW→B／写strobes守恒和最终quiet通过；强普通一hop／preparation-feedback／合法mapping也可执行，完成和成本边界预先冻结。该G0回答有无普通失效，不预先认定失效。它是source-derived role projection，结果只在该合同下解释。

**要支持实际AI应用失效的附加条件：** 给出当前平台的实际consumer/release、source可用性、Requester身份和驻留限制；若重放GPU trace，必须有独立取得的真实release／DMA角色与保真解释。完整GPU并非必要，但主张的关键路径不能来自猜delay。测token速度、持续compute overlap或跨层slot寿命，则要真实计算或独立源迹，而不是更改计算时间寻找有利点。

仅把两个独立环叫“GPU＋SSD”、遗漏完整包、按issue自造ready、声称不存在的媒体DMA、人为缩小内存或调compute delay，可以得到合法事务微基准，不能证明AI普通失效。即使完整包角色投影通过G0，它也不是真实GPU／NVMe部署测量。

## 5. 只留一个有依据的问题和邻近反例

本席只保留一个G0问题：**在合法resident／direct、准备、大页和当前完成反馈之后，如果仍有必要的两个Requester工作，那么它们的重叠是否让已ready的最终copy反复在共享翻译路径等待，并形成完整包准备的实质损失？** 当前没有这项性能证据，不接受新门控候选。

如果损失真实落在必要完成关键路径，普通对照具体不足且完整成本未吞没收益，再去G1设计一条在发布前作用的依赖规则，并核准确前作。反例是整包源可直接送目的，或者普通ready-copy-first／大页已经覆盖余差；此时结束本seed的新门控分支。完整prepare、DAG标签、priority／credit反馈和AI换名都不能自己构成独立创新。

stage100/101只关闭固定慢R2、有限8MiB背景的首块主要损失：6006降为62，255次后续交接没有额外供给气泡。[R2结果](/home/xvhaocheng/work/iommu-platform/docs/workflow-stream-g0-results-20261012.md:16)。首笔背景服务被普通startup推迟13,549周期，是条件边界；没有独立来源期限时不能借它新造失败。[边界复核](/home/xvhaocheng/work/iommu-platform/docs/workflow-stream-platform-review-20261012.md:28)。这不否定所有AI prepare阶段，也不授权人为延长背景或缩短compute。

## 6. Root下一最小动作与模型成本边界

Root现在应完成这一个六张量包的private manifest／DAG compiler和逻辑checker，以“静态／逻辑功能角色投影”接受。随后如要实测，仅私有接上现两iDMA cfg／DONE，替换独立环ready与固定计数，先完成实际DMA功能验收；不改生产ABI、不新建计算RTL。功能通过并冻结普通对照之后，再登记一个bounded transfer-stage G0。

现SYSTEM模型没有CVA6 CPU／cache，也没有MIG／DDR controller／clock／width转换；cfg又绕过生产CPU互连。[模型声明](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_system_top.sv:46)。模型中的硬件DMA和IOMMU事务可以验证条件机制；CPU relay、pinning／cache维护、memmap／pagefault／pagecache、SSD和CUDA成本缺失必须披露。新的FPGA性能主张仍须自身源／image／时钟／实际地址接口和时序对应，不能以这次逻辑功能验收替代。

## 7. 后续实际RTL功能验收

Root完成新build后执行两个完整串行RTL功能种子。本席独立编写并实际运行 [validate_transfer_records.py](validate_transfer_records.py)，没有执行DUT或重跑旧模型。源形状／DAG／TSV／fixture核查见 [静态验收](static-contract-acceptance.json)；它没有import root compiler，也没有把host-reference当DMA执行。

| 新完整模式 | 独立接受 | 实际issue／DONE笔数 | 每份包有用字节 | 原始输出／padding |
|---|---|---:|---:|---|
| 两hop角色投影 | [通过](RTL-two-hop-acceptance.json) | 4618；双方各2309 | 9,447,936 | 六张量完整相等；9,728B holes保留`0xd9`且不计有用工作 |
| resident-direct一hop | [通过](RTL-resident-direct-acceptance.json) | 2309；未使用Requester零活动 | 9,447,936 | 同一份完整六张量及相同holes规则 |

每job唯一真实issue／DONE与NEXT_ID／DONE寄存器绑定通过；issue采样时尚未返回NEXT_ID的`hw_id=-1`按随后保留ID绑定，没有要求采样时预知它。两hopcopy逻辑ready由同tensor全部原始硬件DONE的最大cycle重建，逐tensorbarrier count／bytes核全，不能凭同chunk完成提前拷贝。raw AR／AW／B头的地址覆盖、事务归属和响应数独立核验；R／W／TL实际observer计数按job、Requester和global DDR核守恒／错误，**没有独立逐beat回放主张**。source／relay完整16MiB区域检查来自driver真实运行时断言；本席独立查raw源fixture以及目标六张量和holes。52页PTE＋4KiB DDT共217,088B额外metadata保留量也登记，不能当零成本。

双方DMA、TL in-flight、CPUoff、DDR VALIDs和model／observer事务队列最终排空。验收说明了真实两iDMA/IOMMU的有限功能部分适配；还没有强普通pipeline／合法大页／反馈的性能判别，不把这两个串行功能cycle相减或作AI吞吐。

[三类损坏负例](negative-case-acceptance.json)包含四个必要变体，全部拒绝：同tensor第一copy在自己的chunk fill完成后、晚chunk完成前提前ready；漏报硬件DONE；重复DONE；改变`fc2.bias`一个有用字节。负例保存在`critical/negative-records/`副本中，原events／summary／output的SHA256前后相同。它们验证独立checker拒绝损坏记录，没有额外DUT运行。
