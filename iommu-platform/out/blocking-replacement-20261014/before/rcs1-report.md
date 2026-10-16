> actualgoal1791976511已实际complete，523,102 tokens／44分30秒，无额外预算；五页同步与308＋133项检查先完成。[原样回执](../out/rcs1-completion-set-20261014/root/goal-completion.json)。本轮完成不等于论文完成。

# RCS1：完整翻译集合填入／替换的有界结果（2026-10-14）

**关闭本版完整性能主张，保留新的规则与真实选择差异。** RCS1没有被CO1的普通pin程序逐动作覆盖；但在冻结的四条件中，一个固定SJF整任务保护规则始终等于或优于它的完整完成时间。不能因保全更多完整页集合就宣称任务更快。

actualgoal1791976511，无额外token预算；root与两个新增独占席实际gpt-6.1-sol／ultra／native有效475000，Codex-only。最终actual工具回执在产物导航登记。没有恢复旧席或关闭分支；论文仍在立题。

## 具体新规则和选择差异

在完整成功walk结束时，比较原16way的每个合法victim和不填入：首先最多保全可信登记任务的完整剩余共享lookup集合，再最小化缺key并集；tie优先插入、原masked tree-PLRU。私有结果只交给已接受的当前请求，后续同页lookup仍保留；全集合齐仅用于评分，不增加DMA barrier，也不回收数据owner。

精确化简为：先构造`E_q={除当前返回q外没有其他缺key的任务}`，再比较`refs(victim)∩E_q`。无需复制17份虚拟cache或强制17拍扫描；新增缺key检测、refs过滤、计数／比较、读口、同步和选择延迟全部计费。

单缺key的17-key草案已被固定最少不同任务引用数B5全子族覆盖，未运行。最终沿用原计划18-key纸面范围：16resident是`a,b,p0..p13`，当前q结果属于生产者最后q用途，r仍缺。生产者和大任务剩`p0..p13,r`；两小任务分别剩`a,q`和`b,q`。RCS换p3可保全两小集合；普通minrefs/pin/fractionInsert换a、SJF换b，各保全一个。原tree-PLRU映射亲核，未套LRU顺序。

新能力是精确E_q过滤。MeDiC已给出组完成／高命中比例保护，ISCA2018已给出同指令分组／翻译工作量排序；不能把这些宽泛动机当首次创新。当前有限原文核查未证准确E_q程序已发表，也不是穷尽查新。[前作与具体普通覆盖](../out/rcs1-completion-set-20261014/root/prior-and-closed-boundaries.md)。

## 四个新条件与完整结果

只有一个冻结有限工作：生产者32个64Bcopy，大消费者15个、两个小消费者各2个，51copy。真实前缀初始化a/b及14个p，随后生产q并由B＋阶段收据发布依赖；生产者仍有尾部。所有规则各自冷起，完整前缀工作计时，不复制另一个策略的cache，不增缓存、walker、压力、相位或预热。

四条件是：C1正常登记反馈，C2同工作量禁用反馈，C3交换生产者／小任务的身份和年龄，C4冻结后的反向尾部和合法streaming写地址交错。共享FIFO每tick服务一个8B PTE／数据beat；每walk三个依赖的Sv39 PTE读，返回内容后才产生下一请求。不是固定walk延迟，不是实际DDR／RTL逐拍／板测。各规则同付费1tick选择和1tick结果许可，原单查找口／单返回口、原fullbuffer例外、AW顺序／WLAST所有权和同源串行均保留。

| 条件 | RCS1全部完成／完成时间之和 | 同一个固定SJF | RCS1 PTE请求／字节 |
| --- | ---: | ---: | ---: |
| C1 | 921／3491 | 920／3478 | 57／456 |
| C2 | 923／3484 | 923／3484；全部规则相同 | 60／480 |
| C3 | 921／3491 | 920／3478 | 57／456 |
| C4 | 907／3411 | 900／3408 | 63／504 |

单位均为条件模型周期；从共同登记就绪0计至所有任务真实消费／释放／quiet，含全部生产前缀。相同SJF程序贯穿四条件，不拼事后赢家。[完整32项结果及双方／四任务向量](../out/rcs1-completion-set-20261014/model/results-table.csv)。

C1中RCS向量`P/B/S2/S3=839/847/884/921`，SJF是`835/843/880/920`。原事件显示RCS的新增再walk落在P的p3（tick446），普通minrefs在晚到小消费者的a（tick843）；总PTE数同为57，但早期活数据路径更迟，集合优势没有兑现。C4中P局部提前，RCS`795/837/872/907`仍比SJF最后完成900更迟；B额外再walk p3/p5/p10，不能拿P局部收益当双方收益。不同条件没有改变成本或搜参数追结果。

每个完整episode实计模型数据读／写各3264B，51AR／51AW／408R／408W／51B、四个consumer收据／owner release，实际值和最终quiet通过。全部响应OKAY、errors0；没有注错／失效／取消安全验证。原结果碰撞和stale fallback实际均0，不声称已测试这些分支。

## 全成本和平台边界

当前普通TL正常完成依赖IOTLB update之后命中。cache mask0只禁填入，不能免费正常完成；因此通用bypass要新增普通完整结果直达TC路径，所有规则共同付费。当前TL只接固定分区mask，新动态victim路由也要实现。多copy登记／精确最后用途回调／权限epoch读口／owner收据均不是现成免费接口；当前生产NO_ERROR_HANDLING done不能认证全部错误和consumer／cache交接。

18条key/users的例示组件`1368+18E`，四年龄8bit；完整两阶段结果content104bit仅是一个可能latch组件，不能相加冒充最低总面积。源／job／slot generation、字段复用、端口、同步、全部权限／所有权／软件登记与错误收据成本仍保留。[完整接口和成本](../out/rcs1-completion-set-20261014/root/ownership-and-full-cost.md)。

可能软件路线每copy四个配置writeq＋NEXT_ID read，共255次MMIO操作、名义2040寄存器字节，另计poll／登记／发布／缓存维护；不是实际总线字节或CPU实测周期。条件模型抽象软件供给，不能说CPU-inclusive净收益。面积、Fmax、总FF、实际CPU、GPU／AI应用、生产RTL及FPGA净收益均null。即使给全部规则相同模型时钟，本版已败给普通完整对照，没有理由前移到工作流或FPGA。

## 执行和裁决边界

4条件×8固定规则，共32个完整新episode。最初C1/PLRU在tick0因日志参数冲突、尚无PTE／数据服务就失败；保留原账本／事件／失败，修该日志错误后只重试失败项，再执行未跑31项。输入和机制不变；未重跑成功项、旧模型／validator／trace／实验或构建。root亲读关键源码／完整模型程序，并独立读取116750条新事件，308项核验通过：值／FIFO／依赖PTE／单口／身份／真实用途退休／RWB／完成守恒；未再执行模型。

**关闭的是本版“最大完整剩余页集合数”带来完整性能优势的主张。** 18-key选择差异和E_q精简设计保留，未证明全族普通逐动作覆盖；未否定所有完成感知cache方法，也不自动改论文定位。8/9月资产、CO1/R2/R107/C112和其他关闭结论继续有效。

后继应从活数据owner路径及下一真实R/W用途的有符号代价立不同动作合同，而非调同一集合计数。一个未批准方向是：当前owner已无再用且剩余用户均未data-eligible时，暂不填入该结果，保留活owner路径的页，等实际首次lookup再填；必须与同样延期填入／owner优先普通规则比较，不能凭改名或ready过滤声称创新。本轮不自动启动该候选、工作流、RTL或FPGA。[后继边界](../out/rcs1-completion-set-20261014/next-goal-contract.json)。

[完整导航](../out/rcs1-completion-set-20261014/README.md)／[稳定根裁决](../out/rcs1-completion-set-20261014/root-result-acceptance.json)／[新事件核验](../out/rcs1-completion-set-20261014/root/event-verification.json)。五页状态同步和必要新交付检查先于actual complete；工具回执及用量在完整导航登记。本轮结束不代表论文完成。
