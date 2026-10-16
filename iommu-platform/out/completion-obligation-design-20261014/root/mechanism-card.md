# CO1：完成依赖与翻译用途分别退休

固定时间：2026-10-14，日本时间；actualgoal1791971625。root提出正向设计；H1报告仅作参考。该卡在独占普通席返回前冻结。不是已接受创新，也不是生产实现。

## 问题与条件主张

E2已给有限替换后首次再用的证据；原反馈路线要求评价完整任务，而不是只减当前walk等待。本卡检查已登记后继用途与buffer释放关系是否要求额外的准入/保护动作。原AW完整入口未见，不复用其未成立的性能正例，不恢复p5或P/PN差额。

条件主张：如果可信登记提供真实有限依赖与翻译用途，且合法pre-VALID决定还存在，则在有释放依赖的任务与无释放依赖的新页填充竞争时，可以优先推进前者；已经不可再需要翻译的用途先解除驻留保护，buffer和任务仍按真实完成退休。是否有具体普通实现之外的动作或成本优势，待配对裁决。

## 输入、有限状态与事件

- 最多Q=4登记任务、K=16同时跟踪的正式page keys、B=2逻辑buffer；这只是控制元数据界限，不新增IOTLB项/内存缓冲/walker/原生并发。超出范围整体进入普通路径；不得截断页集合后仍称已完整保护。
- 可信软件发布src/dst/len、原生source与functional-job关联、地址空间/stage/权限、映射代、DAG前驱、真实就绪和buffer的最终释放任务。所有记录/验证/读取/更新均新增计费。登记DAG只是义务，不是授权。
- key使用实际VPN、PSCID/GSCID、stage与有效映射代；global/superpage仍依正式lookup。有限首卡只保护经正式确认的S1/4KiB/non-global页，其余合法走普通。相同IOVA/TFID不得合并不同上下文。
- 每task/key/RW用途记录是否尚需翻译、已经取得被真实下游持有的结果、或正式取消且从未发布。有限任务单次src/dst范围访问可用布尔用途；重复访问必须注册全部用途或保守保留到task完成，不能自行把一次lookup当最后一次。
- `translation_use_done`只有在最后一个登记需要的真实许可已交接给保证其生命周期的原生owner、相应地址/许可不再读该IOTLB项且不会走需要重查的重放时成立。普通hit探测、first VALID、源AR接受、raw R/WLAST不自动成立。没有准确关联反馈就禁用提前解除。
- `task_done_ok`要求完整预定R/W/B、错误状态、所需CPU consumer与cache交接；`buffer_release`再要求最后数据owner解除。当前backend done不自动证明consumer/cache/全局回收。

### 生产接口精确绑定（关键源码亲核后的限定）

`ariane_peripherals_xilinx.sv:1276-1277,1400-1402`只给源0/1启用LaunchPort；旧ResearchPort只在非零research role的源1，而banked非零模式禁止该role组合。因此不能把源码有ADMIT称为当前banked实例已有通用四源硬件准入。`idma_reg64_frontend.sv:253-266`的HOLD还严格限制64B、DEFAULT、对齐、全零选项、batch关闭、backend idle。一般copy可以在软件读取NEXT_ID之前合法延后发起，但需新增任务调度/登记并计CPU/轮询/寄存器供给费用；通用硬件ADMIT或逐用途反馈是新的付费接口合同。没有在此goal修改生产代码。

四个是登记任务槽，不是四个同时运行的512-beat DMA；生产只有前两源在banked mode配置buffer512，后两源仍buffer3。本卡的任务可顺序复用源0/1，保留原生并发/容量。实际RWB守卫不因本卡变动。

当前原生TC在翻译响应握手保存结果，后在COMPLETE把地址交接至真实下游，见`rv_iommu_trans_ctl_wrap.sv:693-726`。本卡以最后一个登记需要的真实下游AR/AW交接作为保守用途退休点，连同最后用途及task/source关联反馈收费；不把TL响应或未握手VALID自行当交接。已移交的完整结果仍由原生spills/target持有，不新增免费prepared pool。

## 冻结的规则

1. 先服从原生R/W/B容量、W归属、同ID次序和已发布VALID。只在实际启用ADMIT/合法HOLD的任务边界选择，已进COUNT不能撤回。
2. 对每正式key，保护它当且仅当至少一个已登记、未取消且未完成翻译用途仍需要它；task或buffer未退休而所有翻译用途已交接，可以解除cache保护，不能回收物理页。
3. 对当前合法ready任务q，`release_path(q)`为：q自身或其DAG后继是当前已拥有buffer的登记最终释放任务。只据已登记DAG和拥有记录计算；不猜未来时间/PTE内容。
4. 本有限优化范围每个源0/1至多一个合法ready head，Q4只是登记槽。通常按release_path、登记年龄、source RR排序。保存唯一被越过head的defer token（task-slot+valid）；token仍eligible时下一选择先服务它；失去eligible/正式取消/退休则清token，不虚发，槽在旧回调与token解除后才能复用。**每次真实授予都记录唯一另一eligible head为下一defer，包括偿还token的轮次**；没有另一head才清token。年龄由固定登记序号定义，不用预测完成时间。超过两个活源不声称所有被越过者下轮同时服务；优化范围外走原普通路径。持续两head eligible时该公平合同使后续选择成为RR，不承诺一直保持release优先。
5. 非release优先head若可能需新fill、没有未保护victim且还有唯一另一eligible head，则只越过一次，记录它并让另一head推进；无另一head、release或defer任务需要推进就立即供给。每次实际native fill优先用`~protected_mask`的原生PLRU；零mask或原有承诺确实需要escape时在同一16项使用all_ways，保留被驱逐key的未来用途（允许未来重walk），不增加escape cache/槽/walker。mask在实际update之前必须一致/稳定，做不到就禁用优化；不把cache pin当访问许可。绝不等未ready后继来解除全部保护。
6. 缺登记、invalid/stale代、无法完整查驻留、没有准确最后用途反馈、队列/登记容量不足：禁用对应优化并按普通合法路径处理。取消已发布任务先按原生排空/错误终结，不早退数据义务。映射失效不得被pin阻挡；旧结果生命周期仍依正式失效/互连排空合同。

这是一个单规则，不按场景挑选策略。保护只是性能建议，绝不是禁止替换或延迟IOTINVAL的硬锁。

## 必须检验的差异

A. 相同当前字节/miss/速率，一个合法ready任务位于占用buffer的释放链，另一个只是独立后台：CO1可选择前者。此例只证明新增信息能改变低信息规则；普通对照也获得该DAG与释放信息。

B. 相同task未退休，两次src/dst翻译的最后用途已经实际交接，而B/consumer未完成：CO1解除cache保护但保留数据/buffer义务。对照必须包含阶段引用计数，不能强迫普通pin持有到整个task结束。

C. 全部保护但释放链下一ready任务需要新页：CO1牺牲部分保护继续合法推进；不得以增加总页数/相位/延迟/并发搜性能正例。这是符号边界反例，不是当前供给实际遇到的观测。

相邻反例：没有复用/释放依赖；普通保护已装下全部活用途；所有后继已完成；合法大页/驻留/提前准备覆盖；源读已完但目的写/consumer仍活；held VALID；重复访问未登记；失效代过旧；容量不足。全部保留，不借其选择更易赢的工作负载。

## 对照与结束

root核具体普通程序与本卡的事件/状态/输出/端口、选择延迟及完整费用。如果用普通引用计数、阶段退休、ready/release优先和有界公平回退即可逐事件映射，关闭CO1这一独有动作/逻辑成本主张；不能把其映射当发表前作或全族不可能证明。若存在明确剩余差异，才冻结另轮至多四新条件，不在本goal运行性能模型。

本卡文字合同的唯一具体实现由`control/min-rule.md`给出；精化的是两源token与实际fill时mask，不是性能测量后换赢家。初次精化错误地在偿还轮不记录另一head，会令新ready任务连续被越过两次；已撤回、原稿与手算反例保存在root/fairness-correction.json。持续双ready成为RR是严格公平的实际边界，不把release优先无限持续当收益。OC1-v1另有TTL/rank/公平次序，保留但不声称逐序列等价。

任务真实完成向量、gain、area、Fmax、能耗、CPU与应用收益均未测为null。原共享任务问题继续开放，论文未完成。
