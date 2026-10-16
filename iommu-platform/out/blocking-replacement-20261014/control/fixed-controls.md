# BR1：固定同信息普通控制与具体覆盖边界

2026-10-14，`BR1-controls-v1`。Codex 新构造的固定对照；没有运行模型、性能、旧程序、trace、validator 或构建。以下程序不是把某篇论文完整复现后改名。准确原作、普通新组合和候选等价实现分别登记在 `prior-boundaries.md`、`ledger.json`。

**结论先行：** 若 horizon 内换 victim 仅增加固定 DAG 的一个节点延迟，B7 静态关键路径/slack 程序精确覆盖 BR1；若真实 horizon 的全部共享 lookup key 都可被原合法16way保存，B8-Hpin给出更便宜的具体零影响对照，但完整覆盖需要本文件列明的 horizon／后续 fill 条件。两项都是有固定程序及条件证明的具体 coverage；都不是“普通可以任意添加候选模块”。共享服务顺序随 victim 改变的族没有在本席证明被 B7 覆盖。

## 1. 共同付费输入、正式提交和回退

作用域严格沿 `../root/mechanism-card.md:9–23`：完整正常成功 walk 得到 q；已有 invalid/free way 先按原规则使用；满 cache 时仅选原合法 victim 并正常插入 q。保持 native post-fill hit、正常权限检查、原响应关联、held VALID、同 ID 顺序、hit bypass 及 R/W guard。无新增不填入动作、direct-result 通道、walker、cache entry、请求额度或数据缓冲。

全部程序看到同一份可信、有版本的一致 snapshot：正式 resident key/context/permission/epoch，源/job/slot generation，已接受 PTE/R/W/B 的进度及真实 ownership，已支付登记和回执的 immediate next R/W lookup，用于下一 real release 的公开依赖／publication，原合法 mask 和同一原 tree-PLRU history。最多原两 active copies、四 logical tasks；登记不能制造 active owner。过去已完成且公开的服务样本可用于共同估计；未接受的 VALID、未来 PTE 内容、隐藏 DDR 状态及完整未来 trace 禁用。

`O` 是当前真实 owner 的集合。`J` 是有有效登记且与 O 的释放义务相关的 logical tasks。`U[t]` 是**本轮已付费、明确列出的 bounded horizon**内 task t 仍需共享 lookup 的不同正式 key 集合，不能凭登记意图扩成完整未来轨迹。若 root 的接口只给 immediate next R/W，则 U 也只能包含那些用途；B2/B3不能偷读RCS1旧51copy的后续列表。root 必须在 gate 明确 U 是否足以覆盖 release horizon。已 stable private permission 且不存在后续共享 lookup 的用途从 U 删除，不由此删除 data/B/consumer owner。

```text
R          := snapshot中正式resident、permission/epoch有效的keys
refs[k]    := {t in J: k in U[t]}        # 不同task，而非重复R/W次数
missing[t] := U[t] - R
age[t]     := 可信登记年龄；tie以task slot数值小优先
L          := 此次原合法ways
P(M)       := 原masked tree-PLRU在非空M中的唯一victim
```

`P` 复用继承的实际 tree 定义，不能用 LRU 链替代；全部way合法时重现原 victim。一般按标量／tuple评分取最小值，再对所有 score-tied ways 用 P。任务保护程序 `PROTECT(S)` 固定使用 `P({w in L: key(w) not in S})`，若空立即 `P(L)`；无等待、TTL或新增供给顺序。未知关联、权限/epoch冲突、超出支持类别、估计/graph信息不足或 snapshot变更时，整个优化分支立即普通 PLRU；正式失效／错误／取消／drain优先，不能为评分挡住真实进度。

选择可预计算，但实际写入仍等正常正式 fill 提交点。决定延迟、并发 generation核对和新增 mask路由全部付费。不同普通规则可合法复用已消费字段；不强迫重复数组、完整16世界或一样慢的实现。

**本轮root提供的新原生边界／旧模型勘误：** root直接核 `src/idma_fullbuffer_certificate.sv:16–18`，eligible要求 `CapacityBytes>=4096 && normal && length==4096 && src/dst[11:0]==0`；`dma_core_wrap:173–183` 以 `be_valid&&ready/job_rsp` 接认证，`fpga:1466` 接至 TC fullbuffer。旧RCS1条件engine给64B copy的fullbuffer例外是模型外加能力，旧文档称“native fullbuffer保留”过宽。本席所有普通程序／BR1都不得继承该64B原生许可；新native解释必须保持真实normal guards，只在完整证书条件成立时用原例外。本席没有重复核这些源码、重跑旧模型或恢复旧性能主张。

## 2. 七个指定固定普通程序

### B0-PLRU

每个正常 cacheable full result 插入 P(L)。不主动 bypass。

### B1-minrefs

每个 victim w 的固定费用 `|refs[key(w)]|`；选最小费用，tie P。没有 E_q／release评分、TTL或独立必须存储的 refcount。原task-user位图可直接 popcount。

### B2-SJF-horizon（准确说明代理量）

在 J 中 U 非空的 task 按 `( |missing[t]|, |U[t]|, age[t], slot[t] )` 选一项 t，`PROTECT(U[t] intersect R)`。这是继承 RCS1 fixed-program.md:70–72 的相同 missing/remaining-key 程序施于**本轮合法 U**，不是知道完整 task 实际时长的 oracle。若 U 的接口范围不同于 RCS1，不能声称此前胜出的51copy动作已在本轮重现。

### B3-fractionInsert

在相同 J／U 中选 `(fraction降序, missing数升序, U数升序, age升序, slot升序)` 的 t，其中 `fraction=|U[t] intersect R|/|U[t]|`，整数交叉相乘比较；`PROTECT(U[t] intersect R)`。无 task 则 PLRU。**每个正常full result总是插入**，没有 mostly-miss bypass 阈值，cold开始能正常形成自己的 resident 前缀。此为继承普通组合，不是 MeDiC 原程序。

### B4-owner-first

只在实际持有未完成 native R/W/B 或 data/buffer/consumer owner 的 J 中选 task；按 `(当前具有正常guard允许的数据推进优先, age升序, slot升序)` 排序，保护它的 U∩R。没有此 owner 则 PLRU。未接受 intention、仅未来依赖未发布且没有当前owned资源的任务不参加排序。优先级是 victim soft hint，不发未许可 payload，不解除 guard，不等任务 ready。

### B5-next-use

对每个 key，仅用共同已登记的 immediate next R/W用途及其公开 ready/dependency进度估计最早 lookup eligibility `eta[k]`；多用户取最小。对合同证明 horizon 完全无共享 lookup 的key设 +∞，对未知使用时间**不能**设 +∞。选择 eta 最大的 victim，tie P。任何需要的eta未知时整个优化回PLRU。估计不得把未来请求已被接受或 future permission 当事实；只是替换预测。

### B6-local-exposed-cost+slack

固定只计算直接使用 victim key 的 task 对自身下一release的局部影响，**不沿 task之间的publication或共享服务重排传播**：在同一无此次eviction的基准投影取得直接用途 i 的附加完整正常walk时间 `d[i]`、对其owner o的局部slack `s_local[i,o]` 和基准release `F0[o]`；`delta_local[o,w]=max(0, max_{i直接使用key(w),属o}(d[i]-s_local[i,o]))`，空集为0。选 `(max_o(F0[o]+delta_local[o,w]), sum_o(F0[o]+delta_local[o,w]), 附加PTE请求数)` 最小，tie P。

这是明确的同信息强普通控制；F0/d/slack来源、端点与generation也必须付费。它没有重复计算虚拟16世界。它对多次同页walk和共享队列的相互作用只是局部近似，不能假称一般精确。无可靠基准或局部slack则PLRU。

## 3. B7：固定图的普通传播/关键路径程序

此程序另于 B6：允许从一个受影响 lookup 节点沿**所有已公开的当前真实owner依赖**传播，包括 producer到consumer、W/B到publication、consumer receipt到release；共享服务边若固定也可在图中。不是只赋予一个 task 一个criticality标签。

```text
B7-StaticCP:
  从同一snapshot构造一个固定有向无环图G，节点为本horizon真实义务
  节点持续时间来自共同paid估计，资源边/数据边由已知合法顺序固定
  前向max-plus取得baseline earliest finish T[i]，owner sinks F0[o]
  对每owner反向求i至sink的最长后缀tail[i,o]
  slack[i,o] := F0[o] - (T[i] + tail[i,o])  # 无路径记+∞
  对每合法w:
    必须有可证的单一victim受影响节点i(w)，其完整附加服务d[w]
    Fhat[o,w] := F0[o] + max(0,d[w]-slack[i(w),o])
    # 没有任何horizon共享用途的victim: d=0，全部Fhat=F0
    score[w] := (max_o Fhat[o,w], sum_o Fhat[o,w], added_pte[w])
  选score最小victim；tie同一P
```

**具体精确cover族：** (a) 图中的义务和所有资源先后顺序不随 victim 改变；(b) 每个victim在horizon内至多导致一个新增完整normal walk；(c) 新增walk对系统全部影响可化为同一个图节点 i 的持续时间增加 d，沿图出边传播，没有未计入的占用或插队；(d) 基准图、d和BR1投影一致，后续fill/epoch无另一次residency变化；(e) 决策时间与fallback/tie计入。若 victim影响多个服务节点，B7使用PLRU，不伪称单节点公式仍精确。

证明：固定DAG中通往owner sink的所有路径分成经过i与不经过i两类。经过i的最长基准路径长 `T[i]+tail[i,o]`；增加d后只有此类路径增加d，所以新sink值为 `max(F0[o], T[i]+tail[i,o]+d)=F0[o]+max(0,d-slack[i,o])`。所有o的向量、max、sum、PTE及PLRU tie因此与BR1精确相同。这涵盖全部满足(a)–(e)的图／权重，而非单一举例。

**决定性边界：** 若唯一正例仅是“同refs/age/task-size，但producer路径slack较小”，B7已经解释并复现那项选择；不能据此宣称新增因果能力超出具体普通传播控制。若victim改变PTE/data arrival、PTW/sharedFIFO真实合法order、cache复用或多个owner的相互排队，(a)/(c)/(d)可能失效；必须另给paid、可达且计全成本的差异。不能为了越过B7添加人为phase／delay。没有此类新见证时，本席不对一般BR1做coverage或收益裁决。

## 4. B8-Hpin：有限真实释放horizon的零影响普通对照

这是root要求单列的新固定普通程序，**不是所有未来task页集合pin或RCS1 E_q**。

```text
H := 所有当前真实owners至各自下一real-release仍需的全部正式共享lookup keys
     + 合同为释放所必需、已经可信登记的immediate uses
     # 已private获许可且不再shared lookup的用途不在H
若H完整性/ownership/epoch可证且L中至少一个resident key不在H:
  victim := P({w in L: key(w) not in H})
否则victim := P(L)
正常插入q，继续原请求/data/B/consumer顺序，不设ready gate
```

每个后续fill依据当时真实owner和完整release horizon重新使用同一程序；snapshot已陈旧时立即PLRU。不能只因某页登记为future-only而不插入q；Hpin动作域仍为normal fill victim。

**有条件零影响证明：** 若①H真覆盖全部old owners到real release的后继共享lookup；②formal失效/错误/新不相关供给不会改变比较中的服务；③后续fill也能持续保留H驻留key，或投影另外证明不发生H二次丢失；④合法mask中存在outside-H victim；⑤已private许可不由eviction撤销；⑥外部未观察cache状态不用于授权，则evict outside H与保留其驻留无任何horizon lookup差异。无额外walk/PTE，也不改变R/W/B到达和owner release。因此所有outside-H victims的F向量和PTE均与基准相同。

**零影响不自动等于全族最优：** 新增victim miss可能改变sharedFIFO arrival、guard允许的数据窗口或N/服务交错，使另一个owner较早释放。因此 `F_o(v)>=F0[o]` 不是一般native单调定理，outside-H动作得到baseline不能据此否决所有dynamic-order BR1。

精确最优／tie覆盖另要求：⑦固定服务／单调投影已证明每个legal victim的score不优于baseline；⑧每个inside-H victim必定产生**严格新增PTE服务**且不会被其他本来就有的normal fill恢复，或其F目标已严格更差。满足⑦⑧时，第三评分项排除F目标同baseline的inside-H victims，B8的P(outside-H mask)精确复现BR1的最优score-tied选择。若存在inside-H同样零影响且没有更优动作，Hpin只得到同样最优F／PTE，不能声称逐动作tie精确覆盖。若⑦未知，Hpin只给出baseline普通对照，不保证最优。若普通PLRU足以覆盖其完整冷episode，则另由root实际核验，不能把本证明当运行证据。

若一个dynamic-order场景通过新增miss产生流量整形收益，下一审查需要固定、同信息且同样计费的合法source-pause／service-priority普通程序，核它能否在不支付无用walk的情况下产生相同到达顺序。只能使用真实可控的正常提交／service接口，不能靠人为delay或偷改guard制造新场景。本席没有构造新pause时长、授权生产调度改变或运行这类性能；此为具体后继对照要求，不是宣称已经普通全覆盖。

**容量前提由root／witness席核native，不在本席重核：** 如果H只含每个四logical task的下一R/W、每用途单正式页，则纸面 `|H|<=8`；原legal16全部可用时至少8个outside-H ways。这个算式不能证明“下一R/W就是到release全部用途”，也不能覆盖跨页、更多stage/context key、受限legalmask或尚未登记的后继lookup。两个active source当前private/accepted用途的可达上界仍需原源码与event链。不要把四logical task当四新增active copy。

**需要保留的二次替换问题：** 初次victim不同可改变PLRU history。如果BR1投影后来使用无保护普通PLRU，可能随后把H key换出；仅有初次 `|H|<=8` 不足以证明整个release horizon全部victim零影响。必须明确后续fill程序、使H保留的不变量或仅一个后续lookup的真实边界。root裁决前不得省略这个前提。

## 5. 候选等价实现和future-only延期纸面对照

`EQ-BR1`：对每合法victim重新构造 `R-v+q`，按候选完整service/dependency projection计算F，按相同max/sum/PTE/P评分。它是候选的可编程等价实现，不能仅因写在ordinary文件里就称准确前作或独立普通覆盖；同样不强迫BR1采用此16-world实现。

`D-future-only`（仅纸面成本对照，不在BR1动作域）：若q当前owner获合法完整结果后不再shared lookup且q没有当前owned-release horizon用途，延期填入q、保留resident；实际首次合法future lookup再normal walk/fill。当前TL normal completion依赖post-fill hit，故“不写cache”不能完成当前请求。必须新增/证明复用完整checked result到原TC的直达、关联/epoch、反压/permission/drain通道；继承104bit完整content只是可能组件，费用见full-cost.json。若BR1无该通道，不能把这个对照混入普通normal-fill性能程序或声称其通道免费。

## 6. 接受条件与交接

固定程序先于新性能执行；没有根据新性能结果调次序、阈值或算法。具体收费见 `full-cost.json`；准确prior见 `prior-boundaries.md`。本席只提交普通程序、条件定理与成本；生产可达性、冷供给、gate、总体机制设计、最多四条件、模型及状态同步归root。原RCS1／CO1和其他关闭结论继承，未恢复。
