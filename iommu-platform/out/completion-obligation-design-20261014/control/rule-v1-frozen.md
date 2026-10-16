# OC1：阶段引用计数、有界 pin 与释放任务优先

2026-10-14，独占普通强对照席。规则版本 `OC1-frozen-v1`。下面第 1–8 节先冻结，尚未读取 root 的 CO1 候选卡；第 9 节以后才做配对。它是本轮具体可执行的普通构件组合，不声称已有论文完整发表这一组合。

结论：普通控制可以使用同样的可信有限 DAG、真实阶段交接、未来登记使用和 buffer 释放资料。它既能保护未来仍需的 resident translation，也能在最后翻译用途交接后立即解除保护，同时保留整 task、R/W/B、CPU consumer 与 buffer 的安全账本。保护满时，由释放优先、公平轮转和有界 pin 租期让路，使用原 16 项原生 PLRU；没有逃生 walker、结果槽或额外 buffer。

## 1. 固定范围和接口

- 同时登记 `N<=4` 个任务、`K<=16` 个不同正式 page key、`B<=2` 个有所有权的数据 buffer；原 IOTLB `C=16`，单正常 TL/PTW。超范围的请求保守走普通路径，不伪装成已登记任务。
- `key=(正式地址空间标识, S1/S2 stage, page VPN, mapping epoch)`。正式上下文在本平台至少包含 PSCID/GSCID 与 stage。上下文 ID 的复用、根和权限变化必须进入可信 epoch/失效合同。global/superpage 依原生匹配语义处理；相同 TFID、IOVA 数字或数据 buffer 地址不建立权限。
- 每个登记 task 有不可变 src/dst/len、正式上下文、前驱、真实 ready、输入/输出 buffer 使用、真正释放 buffer 的退休任务。src/dst 页集合可从有限范围生成；超过 16 个记录立即拒绝优化登记。软件不能提交“未来会有的 PTE”、未来 DDR 内容或预计完成时间。
- 页面资料、DAG、阶段最后使用反馈、buffer 交接、常驻 cache 观察均是新增付费接口。普通对照与候选看到相同资料、用同样精度和同样安全边界。没有把纸面接口算成现生产能力。
- 只有实际启用且合法的 pre-first-VALID `ADMIT/HOLD` 或软件 pre-kick 可供给闸门参与选择。`COUNT`、已 held VALID、已接受 AR/AW/W/R/B 一律不能因后来策略决定撤回。
- **当前配置范围很窄**：HOLD 的 `launch_cfg_ok` 要求 64B、DEFAULT、64B 对齐、batch-disabled、backend-idle；生产 launcher 仅 IOMMU 的前两源可配。旧 ResearchPort ADMIT 只配研究 source1，且 research role 与 banked nonblock mode 有互斥检查。一般四源、一般长度的闸门及注册/反馈路由是双方共同新增费用；软件在 next_id kick 之前作决定是同功能可行位置，但也计供给和交接成本。

## 2. 固定状态；翻译用途与任务安全分开

软件/协调器保留一个有限登记表。任务状态是 `REGISTERED / WAIT_READY / PREVALID / OFFERED / RUNNING / DMA_DONE / OWNER_WAIT / RETIRED / CANCEL_DRAIN / ERROR_DRAIN`。`OFFERED` 从首次进入 COUNT 开始，不能回到 PREVALID。

每 key 保留两个 4 位位图 `read_users[k]`、`write_users[k]`，分别表示已登记任务仍需的 IOMMU 读/写翻译用途。`refcount(k)=popcount(read_users[k] | write_users[k])` 从位图派生，不强加另一份计数器。同任务重复读/写或多个原生 burst 只有在该侧最后真实用途交接后才清该 bit。位图可以从 src/dst 范围直接计算；这里只选 materialized 16-key 实现便于成本逐项比较。

每 key 的软保护还保留 `pin_enable` 和 0–16 的剩余协调器 round 租期 `ttl`；`pin_enable` 不是访问授权。每 buffer 另保留真实 owner、ownership generation、所需交接状态和唯一登记释放义务，绝不由 translation bit 或 cache eviction 推导其空闲。

任务页资料、native in-flight 状态和 data owner 状态可以合法复用已有字段或软件记录。缓存索引/PLRU、已消费阶段信息也可复用；不要求 ordinary 为同一事实多造副本。

## 3. 事件顺序和更新规则

同 round 顺序固定为：失效/epoch/错误禁止旧优化 → native 已接受工作继续与完成反馈 → 可信阶段交接 → task/data owner 退休 → 登记/真实 ready 更新 → pin 租期 tick → preVALID 选择。并发同 key 的失效优先于 fill，遵守原生 IOTLB 的 invalidation-before-update 规则。

| 事件 | 固定更新 |
| --- | --- |
| `REGISTER` | 校验有限 DAG、范围、正式 epoch、buffer 归属和登记规模；给还未真实交接的 read/write 用途置 bit。只有已观察到当前 resident mapping 的 key 才可能 pin；未来用途本身不产生翻译。 |
| `NATIVE_FILL / REAL_DEMAND_HIT` | 付费接收实际 key/context/way/有效性反馈；若 key 还有登记用途且未 stale，开启软 pin 并置 `ttl=16`。策略观察/probe 不当作真实 demand hit，不刷新 pin。 |
| `TRANSLATION_END(task, side, key)` | **仅当该 key/side 的最后翻译结果已真实交给原生 physical-address owner、所有可能再 lookup/replay 的用途已排除**时清相应 bit。若需要重新翻译的错误恢复仍可能发生，就尚未发生此事件。bit 全清即解除该 key pin；不等整个 task 的 B 或 CPU 退休。 |
| `DMA_DONE` | 保存真实整个 DMA 完成，含任务要求的完整读、写及最后独立 B；不能用源 RLAST/翻译结束冒充。实际错误响应不能直接作为成功 DMA_DONE。 |
| `OWNER_RETIRED` | 满足该 task 的原生排空、必要 cache 所有权交接/同步及 CPU consumer 合同时，置 RETIRED；此时才清图依赖和释放约定 buffer。 |
| `TICK` | 每个有限、持续运行的协调器 round，对已开 pin 的非零 ttl 减一；到零关闭 pin，但保留未完成用户位图。旧 bit 未改变不自动重新 pin；需新登记使用或真实 demand/fill 事件刷新。 |
| `INVALIDATE / EPOCH_CHANGE` | 立即撤销受影响 cache 的软保护和绑定；原生失效不可被 pin 延后。旧未完成 data obligation 保持，旧 mapping 资料停止优化，后续重登记/重查。 |
| `CANCEL / ERROR` | 按第 7 节处理，不以取消 hint 冒充已发出的任务排空。 |

`TRANSLATION_END` 是通用阶段引用计数的付费精确信号，不是当前前端已暴露的功能。一个 AR/AW 握手是否足够，取决于实际 physical result owner、拆分、重发及错误恢复路径；本轮不从 `super_last` 或 source RLAST 猜这个信号。接口拿不到该证明时，对该 bit 保守留到 native drain，记录阶段粒度不可用；不声称此降级具有提前解除能力。

## 4. 固定任务资格与优先序

最多四个 DMA source 的实际 preVALID head 参与；未来已登记但未进入合法入口的任务不会被凭空发出。CPU consumer 的调度/所有权资料经软件接口更新；本规则不能靠 IOMMU 闸门提前完成 CPU。

`eligible(task)` 同时要求：真实 ready；所需前驱已在指定完整退休点满足；输入/输出 ownership/acquire 合法；该源真实处于未发 VALID 的闸门；该闸门的 native 前置条件满足。数据 owner 或 buffer 不满足时，不进入公平候选。

固定排名来自普通 DAG 调度构件，不使用预计完成时间：

1. `release_class=2`：该 eligible task 真实退休将释放目前占用的登记 buffer，且 owner/generation 匹配。
2. `release_class=1`：它是尚未完成的、已登记 buffer-release task 的必要前驱；使用固定 DAG 可达闭包计算，不凭空新增前驱。
3. 其他 `release_class=0`。
4. 同 class 优先较大的 structural critical rank：到任一登记后继终点的最长 DAG 边数，范围 0–3。它是结构深度，不是 oracle latency/EDF。
5. 同 rank 以四源 RR 顺序 tie-break。

全局授予计数 `g mod 4` 初值 0；`g==0` 的 round 为公平授予，忽略 release/rank，从所有 eligible source head 按 RR 选一个。其余三次按上述排名。每次真实允许进入 COUNT/pre-kick 后才 `g++` 并将 RR 指针移到所选源之后。READY 但被 buffer/native 安全阻塞的任务没有虚假保底授予。

持续 eligible 的 source head 最迟在 4 个公平 slot 内获选，即最多 16 次真实供给授予；这是授予数界，不是 wall-clock latency 界。没有 native 响应/CPU 就绪，任何控制都不能保证具体完成时间。

## 5. 固定 pin 与 cache allocation 规则

保护预算 `P=C=16`，允许保护整个原 cache；不人为给 ordinary 划小保护区或强制保留新资源。只有现在合法观察到 resident、refcount>0、ttl>0、epoch 有效的 mapping 可保护。多个登记 4KiB key 落在同一原生 superpage way 时，保护该 way 一次；解除必须等该 way 所覆盖的登记用途都允许解除。

选择时通过付费当前 cache 查询/反馈得到 `resident_seen` 和保护 way mask。它仅判断是否有 resident translation 可被替换，不能绕过真正请求的 native 权限/上下文检查。若要把“合法 hit 不耗新解析 credit”加入其他基线，应另外计 native 许可查找；本 OC1 不拿 tag-only hit 作访问许可或免费 miss oracle。

常规 fill 使用 `policy_mask = all_ways & ~protected_ways`，保留原生 masked PLRU。冷页多于当前可用 unprotected ways 不要求一次同时预留全部页：原生按真实请求顺序流式执行，普通允许复用已消费阶段/被解除保护的 way。已有 normal TL/PTW 容量继续约束请求，本程序不增加 MSHR、join 或 walker。

在 preVALID：

```text
E := 当前所有满足 native/data/ready 条件的 source heads
if E 为空: 不发新任务；drain/feedback/tick 持续运行
if g mod 4 == 0:
    i := E 中按 RR 选出的 head
    grant(i)，必要时以原生 PLRU 牺牲软 pin
else:
    F := E 中“所有被观察到的用途均 resident，或有 unprotected victim”的 head
         加上 E 中 release_class > 0 的 head
    if F 非空:
        i := release_class、structural rank、RR 的固定最大者
        grant(i)，release head 必要时以原生 PLRU 牺牲软 pin
    else:
        HOLD 本次新供给；16-round pin 租期仍走，不等未来事件永久保持
```

对未登记/资料失效/查询 unavailable 的 native head，不能宣称“全 resident”；在已有保护与正常查询都可信的局部模式里按可能 cold 处理。若普通闸门/缓存反馈整体不可用，则取消该作用域的优化保护并走原 native/RR，不靠缺失信息 HOLD。

每个 native fill 都有同一个固定逃生规则：如果有效保护令 allocation mask 为零，或一个已承诺 task 的实际需求超出决策快照，临时使用 `all_ways` 的原生 PLRU 完成这次 fill。接受实际 eviction 反馈后关闭 victim 的软 pin，**保留仍活的用户位图与完整 task/data obligation**。不加一项 cache、一份结果寄存器或一个逃生 walker；没有 claim 这些 future translation 仍驻留。该 fill 规则只改变 replacement，不能修改 held VALID、permission 或 W owner。

`policy_mask` 必须在原生 update 使用它前稳定，或由付费同周期控制逻辑给出；不能在已发出的 update pulse 后补决定。若当前接入无法满足这一时序，禁用动态 pin，回普通 PLRU；本轮不偷偷引入 update-hold result slot。重读/跨时钟/组合选择/反馈同步的具体时延和面积未测，均保留 null。

## 6. 循环等待和真实释放任务

页 pin 是软缓存约束，没有数据 ownership 能力。故它不得造成 `release task 等翻译 → translation fill 等被 pin way → pin 等 release task 退休` 的永久环：就绪直接 release 或其必要前驱可以穿过全保护，已承诺任务的 fill 永远按原生 PLRU 逃生；后台还有 RR 与 pin 租期。

全保护且所有登记 release task 都未 ready 时，不凭空令其 ready。保守等待最多本次 soft lease；此后页面保护让路，后台普通任务可继续。CPU/外部就绪延迟、原生 R/W/B/owner 阻塞仍真实存在。

仅有“任务 DAG 无环”不足以证明两个有限 buffer 一定能调度。登记时另核 buffer acquire/owner/release 关系：若必须拿已占 buffer 的任务又是释放它的唯一前驱，或已占 buffer 根本没有真实可达释放者，则登记无合法进度合同。此时取消 cache 优化并向软件报告资源登记无效；不能覆盖旧 owner 或用页 pin 推导 free。原有数据资源死锁的解决需软件改变合法 buffer/task 安排，不在此 cache 控制的完成保证内。

## 7. stale、取消、错误和容量不足

- stale callback 的 graph/task incarnation、source binding、key/context/epoch、buffer generation 不匹配：不清 translation bit、不解 data owner、不给后继 ready；停止相应 hint 并要求 native drain/软件重登记。槽位仅在所有旧回调/已接受事务排空后复用，generation wrap 必须 quiesce；不能因有限位宽默许 ABA。
- preVALID/HOLD 取消：确认没有首次 VALID 后清该未发任务的用途；后继不能当其成功完成。只有软件已确认无外部使用的相应 buffer 才可释放。HOLD ID 退休与真正 copy 完成分别记账。
- COUNT/held VALID/已接受请求后取消或错误：保留 task/data owner，按原生错误/abort 协议排空；冻结成功依赖发布。收到可信 no-further-translation 事件可清相应翻译阶段引用，但 task/buffer 直到 error drain 与必要所有权交接完成才释放。源读完成不清目的数据债务。
- invalidation 立即胜过 pin。映射 epoch 变化后不复用旧 PA；正常 native 失效、必要 fence/ack 和 cache/data 同步费用双方全计。本轮不证明生产失效/错误恢复协议。
- `N>4/K>16/B>2`、不可信 DAG、未知 src/dst 范围、任务 mode 不能绑定、原生闸门未实际启用：拒绝优化登记/停止受影响 hint，走原 native 供给与 RR；已有完整 owner 义务继续保留到安全排空。无额外压力任务或 walker。
- native cache 查询不具备完整 epoch 一致快照、mask 接口没有实际接通或更新竞态不能闭合：停止 pin；同资料任务排序仍可在合法软件 pre-kick 位置执行。资料缺失的费用/能力记 unknown，不能记 0。

## 8. 有界纸面例子和相邻反例（非模型、非 trace）

例子 E1：最多四个任务 `A -> B -> D` 加独立 `U`。A 已真实完成 producer copy；buffer0 的真实 owner 是 B 的 consumer 使用，B ready，B 的完整退休释放 buffer0；D 尚因 B/该 buffer 不可供给。U ready 但不解任何 buffer。登记页最多 `{p,q,r,s}` 四个：B 读 p、写 q；D 将读 q；U 读 r、写 s。p 当前 resident，B 的 read bit=1；q 未 resident。令 `g mod4=1`（一个明确合法控制状态）。

- 普通程序选 B（release_class2）而不是 U；p 被 pin，q 可在原 16 项 unprotected way 上正常 fill。没有未来 PTE 或完成时间。
- B 最后一次 p 翻译交给真实 physical owner，且已无该 key 的后续 lookup/replay 时，立刻清 B-read(p)，p 无后继用户即 unpin。B 的 R/W/B 与 CPU/owner obligation 继续活；不能因此发 D 或覆盖 buffer0。
- B 对 q 的 write 翻译阶段交接只清 B-write(q)，D-read(q) 保留，因此 q 仍保护。等 B 的完整退休与必要交接发生，D 才可成为 eligible。D 的最后真实 q 翻译交接后保护解除。
- 在相邻输入 E1' 中，B 已完整退休且 p 无登记后继，p 不保护，U 可普通推进。两个输入可以有相同当前 miss 数和字节；区别来自同信息普通引用计数。这说明具体普通也能读取 H1 的信息区分，不是性能数字。

例子 E2：同一可信窗口内至多 16 个登记 key 都 resident 且有未来用户，故全 way soft-protected。若 ready release task 只用这些热页，则普通立即准入，原生安全路径仍检查。如果 ready release 或公平后台的真实 fill 需要新页，固定 escape 使用原 16 项原生 PLRU 牺牲一个保护；被牺牲 key 的 future bit 继续存在。未登记请求的 key 不占登记表新槽、也不假装属于可信 DAG；若要求它一并正式登记使 K 超界，则优化登记拒绝。这只是容量满状态的逻辑分支，不添加 walker/压力流或声称生产已出现它。

相邻反例：B 实际未 ready 或前驱/owner 未满足，ordinary 不把它当释放者；pin 不能缩短 CPU/外部等待。只有 U ready 且还有 unprotected way，普通立刻准入 U 并可保留 p；全 pin 则有限 HOLD 后让路。没有真实后继复用、合法大页/原 residency 已覆盖全部、保护/查询成本高于 rewalk、native 已发布不可撤回的请求、以及 buffer 登记本身无合法进度时，都没有新收益保证。`g==0` 的公平 round 也可以先让 U 走，这个固定选择不是事后改赢家。

上述例子只检验状态转换和法定动作，0 performance model/experiment、0 build/test/validator/trace；area、Fmax、周期收益、物理接入时延没有测量。

## 9. 配对状态

待冻结文件保存后，读取 root CO1 卡并追加一次固定配对；不改第 1–8 节来适配场景。
