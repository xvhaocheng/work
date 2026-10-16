# RCS1纸面选择见证：17key覆盖边界与18key最终gate

2026-10-14。本席只做固定程序、代数和纸面手推；没有执行性能条件、枚举、实验或 trace。**18key 的最终 snapshot 确实区别冻结的 B0–B6；17key 单缺 key 草案由 B5 全子族覆盖，不能把它当独有机制。** 选择差异仅允许 root 决定是否立另一个有界测量合同，不预先证明完成时间、净收益或公开前作新颖性。

## 17key草案为什么停止

cache16 resident=`{x,p0,...,p14}`。当前 q full walk 属于 T0 最后一次 q 用途；合法 private handoff 后，`U0={p0,...,p14}`，`U1=U2=U3={x,q}`。T0最老、native victim=x。插入任一 pi 可保全三个小 task 的完整翻译集合；B0/B1/B2/B3/B4 可选择x，仅保全T0。

但这时每个 task 除 q 外没有其他当前 missing。固定 B5 把 victim 的不同 task refs 与 bypass 的 q refs 比较：pi refs1，x/q refs3，所以它与RCS1一样插入pi。更一般，对全部 `union U - (R union {q})=空集` 的输入，插入v完整task数=`Nlive-|refs(v)|`，bypass为`Nlive-|refs(q)|`；缺 key 并集只是0/1。固定同 tie 后两个程序全子族选择相同，完整证明在 `fixed-program.md`。

该草案没有运行性能模型。root 接受此覆盖后，最终纸面合同保留4task/cache16/单PTW/原队列，改为18个付费登记正式key；这不是扩大cache或新增walker。

## 最终18key snapshot

令 `P={p0,...,p13}`，共14个key。18个正式key为`{a,b,p0,...,p13,q,r}`。所有相关PTE的权限、上下文/epoch和正式key匹配在本合同内有效；r的VPN/用途已经可信登记，r的PTE及完成时间未知且不读取。

当前16个真实resident key是 `R={a,b} union P`；q的完整成功walk刚到达，r仍真实missing。当前q请求属于T0的最后q共享lookup；q结果只合法交给已接受T0 TC owner，随后从T0的共享用途删除。三条边界共同成立：T0仍有其他lookup/data义务；后继仍依真实producer B/ownership发布就绪；任何task不因为“全keys齐”才获准正常执行。

| task | 登记年龄 | handoff后U[t] | 当前missing | 当前fraction | 真实角色边界 |
| --- | ---: | --- | --- | ---: | --- |
| T0 | 0 | P ∪ {r} | {r} | 14/15 | 当前q owner；q已合法私有交接，未来仍有14p和r尾部用途。 |
| T1 | 1 | P ∪ {r} | {r} | 14/15 | 真实登记的后继任务；保留自己的用途与owner/发布条件。 |
| T2 | 2 | {a,q} | {q} | 1/2 | q生产B/owner发布后正常按原软件顺序执行。 |
| T3 | 3 | {b,q} | {q} | 1/2 | 与T2同一合法发布条件，其数据偏移/owner合同独立。 |

T0/T1的p引用不能因r missing而删除；它们仍需日后lookup。RCS1只是减少对当前尚无法形成完整集合的p引用的替换优先级，不提供新授权或buffer回收能力。

## 原16way tree与固定冷首次出现顺序

不能指定任意LRU排名。原 tree 使用breadth-first节点0..14；0走左、1走右。root冻结的冷首次出现顺序是：

```text
key: a,p0,p1,p2,p3,p4,p5,p6,b,p7,p8,p9,p10,p11,p12,p13
way: 0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15
```

这是同一个有限任务的实际64B工作copy前缀，不是预装cache/额外预热。纸面推导条件是：每个首次cold fill之后，该within-page copy的另一原生lookup真实命中并触摸该way；没有额外key/touch/失效夹入。fill本身不被假设为免费PLRU hit。一个way的重复hit触摸产生相同路径bit，不改变本推导。此native触摸条件及真正完整cold来源由root在后续唯一新合同中验证；本席没有运行它。

| way | key | way | key |
| ---: | --- | ---: | --- |
| 0 | a | 8 | p0 |
| 1 | b | 9 | p7 |
| 2 | p3 | 10 | p4 |
| 3 | p10 | 11 | p11 |
| 4 | p1 | 12 | p2 |
| 5 | p8 | 13 | p9 |
| 6 | p5 | 14 | p6 |
| 7 | p12 | 15 | p13 |

逐次原hit把所到路径节点设置为所用子树的反方向。上面bit-reversed遍历完成后，每个内部节点最后一次使用的是自己的右子树，所以全部15个tree bits恰为0。最后producer源p13对应way15，只再触摸节点0/2/6/14，仍写0，不改变该全0状态。q fullwalk到达且handoff之前没有虚构“q命中”来触摸tree。

因此full mask下的路径是node0→node1→node3→node7，四次走左，victim=way0=a。若只禁止way0，node7左叶0不合法但右叶1合法，因此选择way1=b。若mask仅为14个p即ways2..15，node3的左子树(0/1)不合法，走其右子树(node8)，再走左，选择way2=p3。只允许ways0/1时仍选择way0=a。

关键源码为当前工作树 `rv_iommu_iotlb.sv:384-400,405-422`。这也是为什么旧的首次顺序`a,b,p0...p13`不合本张gate：那会让b落way8；SJF仅保护way0后选择way1的p，从而与RCS1相同。root在任何模型执行前接受并冻结上述真实树修正；不能继续用旧顺序声称SJF差异。

## 逐动作分数：不把翻译集合当任务完成

所有动作之前当前完整翻译集合数为0；U0/U1缺r，U2/U3缺q。q的private receipt已经只服务T0，不免费私有交给T2/T3。

| 合法动作类别 | 完整U集合task | C | 缺key并集 | D |
| --- | --- | ---: | --- | ---: |
| insert q, evict任一pi | T2,T3 | 2 | {r,pi} | 2 |
| insert q, evict a | T3 | 1 | {r,a} | 2 |
| insert q, evict b | T2 | 1 | {r,b} | 2 |
| bypass q | 无 | 0 | {r,q} | 2 |

RCS1所有p victims第一项最优，第二项相同；tie使用原masked PLRU，所以唯一动作为`insert(q,way2=p3)`。它不会因T2/T3集合齐全就虚构B/数据完成，也不会等待T0/T1全key齐再服务。

## 七个固定普通程序的唯一选择

| 固定程序 | 在该snapshot的具体计算 | victim/动作 | C/D |
| --- | --- | --- | --- |
| B0 原PLRU | 全16way mask；tree全0。 | a，way0 | 1/2 |
| B1 phase refs+PLRU | 所有16resident key均有refs；all-protected固定原PLRU fallback。 | a，way0 | 1/2 |
| B2 oldest整任务 | T0最老；仅保护14个resident p；合法mask={a,b}。 | a，way0 | 1/2 |
| B3 minimum-missing整任务 | 四task missing均1；remaining数先选2-key T2/T3，再age选T2；保护a。 | b，way1 | 1/2 |
| B4 当前fraction普通MeDiC-like | T0/T1的14/15高于1/2；age选T0保护p。q未来用户T2/T3的fraction为1/2，超过固定1/5阈值，故插入。 | a，way0 | 1/2 |
| B5 distinct-task RefMin | a/b refs1，p refs2，q-as-bypass refs2；最小mask={a,b}，插入优先/原PLRU。 | a，way0 | 1/2 |
| B6 fractionInsert | 与B4相同fraction排序保护T0的p，但每个cacheable result固定插入；root在模型执行前独立增加此分支。 | a，way0 | 1/2 |
| RCS1 | E_q={T2,T3}；p在E_q内refs0，a/b各1；最大C后mask为所有p。 | p3，way2 | 2/2 |

这张见证真区别于oldest、最少missing/remaining、fraction及blind refs的完整固定程序；没有事后为每一场景拼接赢家。后续不得因模型结果修改这七程序；若它们优于RCS1，直接记录并裁决。B4在真正cold fraction=0时会持续bypass，未必能自产生这张共同snapshot；它在本表只是同输入选择函数对照。root在任何模型执行前明确加入B6 always-insert强分支，且保留B4原程序，后续整个episode必须让各policy自产生cache/prefix状态。

新增选择能力在于准确“除q外没有其他missing”的E_q过滤：T0/T1虽然fraction高、p refs多，r仍缺使本次保留p不能产生完整集合；两个小task则能立即获得完整翻译集合。使用共享task-key信息本身不是新信息贡献。

## 全成本、前作和gate

root最终合同为51次64B实际工作copy，名义payload `51*64=3264B`；全部cold来源/生产者/尾部工作都计入episode，不只截有利后缀。具体任务程序由root冻结，本席没有执行或独立声称其真实native路径可达。实际AXI R/W/B、PTE字节、软件登记/发布开销、完成向量、净gain、area、Fmax和GPU/AI适配仍为null。

RCS1可用E_q过滤refs的代数实现；不强迫17份虚拟cache或17tick扫描来制造成本劣势。E_q检测、refs AND过滤、计数/比较、同步cache读口及选择延迟仍有真实付费逻辑。18记录的`1368+18E`只是key/users组件小计，完整共同接口/生命周期费用另列，见 `cost.json`。

本席冻结的MeDiC-like、task pin组合与B5都是普通新组合，不冒充已发表完整控制。准确前作是否已有当前E_q filtered refs或同一集合目标，交root核原文；本席没有用自己的代数恒等式自动宣布已发表覆盖。

纸面gate结论：17key草案关闭；18key通过固定选择差异gate，仍不等于性能/物理成本/主论文贡献已通过。若后续原TC direct-result/精确最后q用途、登记/owner发布或冷native序列不合法，应停止这一实例；不能通过免费phase oracle、额外预热、压力、cache/walker增加或全key barrier补成优势。本席执行实验数为0。
