# §80 B：授权 W 数据债驱动的 PTW 准入——有限合同与具体简单复现

作者：Codex 独立机制设计与关键源码核查。2026-10-09。本文只有手算合同；没有模型执行、RTL 改动、构建或 DUT/板上 PASS。

## 1. 判定先行

这一轮最强正向规则是：**当前已承诺 W 的源数据仍未齐时，如果 own READ/WRITE 许可已完整成功，就暂缓新的无关 PTW AR；把保护从占着唯一源 route 的 B 尾拍交接给 A 源读，直到 A 的真实源 RLAST。若 A 还缺 READ 许可，就先走 A 自己的 U→L 两级 walk。**动作发生在共享目标 AR 首次 VALID 之前，不干预 MIG 已经选择/持有的 R。

热头手算可以把 A/B 剩余 W 闭合从 9/10 提前到 7/8；冷头沿 own walk 链可得到 9/10。但在下面这个完整有限合同中，**普通“当前 W 头准入屏障”用同样的 own 许可、W 数据缺口、同容量与同一超时，也得到热 7/8、冷 9/10，逐拍相同，不需要追溯 B 的 route 身份。**因此，这个合同不能支持“新 wait-chain 调度算法”，也不能靠新反馈线的名字把它作为独立创新。

如果以后在真正多个 route、不同权限/缓冲债同时存在的架构中，精确链反馈能完成粗 W-head 屏障无法完成的动作，或实现明确更低的状态/控制成本，才可以重新审查窄架构贡献。本文不借此可能性扩展未知场景。本轮 B **停止，不建议为了确认已推导的同轨迹而新增 RTL 或大矩阵**。

## 2. 当前源码门与新架构合同的边界

### 2.1 当前 §39 compact 原型不能生成本例

- `out/prepared-source-bypass-20261008/compact_pretl_owner_fragment.sv:87–132`：源 child 候选必须匹配虚拟/物理 W 头 owner、seq，只计连续同 owner 的已准备 W 前缀；`candidate.len` 被这个前缀覆盖长度截断。
- 同文件 `:241–254`：一笔 child 在首次 offered 时捕获，AR 握手后全局 `busy_q=1`。
- 同文件 `:256–270`：child 的真实 R ID/长度/响应检查后，仅实际物理 RLAST 清 `busy_q`。
- `out/prepared-source-bypass-20261008/multi_actual_dma_top.sv:549–589`：本原型 normal/direct source AR 共用一个全局 child，真实 direct R 必须有 captured child route。
- `cva6/vendor/pulp-platform/axi/src/axi_mux.sv:215–235,253–255,277–289`：第一次 AW offer 就形成 W owner FIFO 承诺；stall 时锁住 AW，实际 WLAST 才推进 W owner。不能为了赶源数据重新安排这条 W 序列。

因此，B 的一次源 child 跨越 `B0→A0→B1` 三个 W 片段，在旧 compact 的“只覆盖连续当前 owner W”门下不能发生。本例不是现有 DUT 的遗漏 PASS，也不把旧原型门解释为全领域不可能。

### 2.2 允许的有限架构变化（所有对照相同）

在原先 B3 私有 FIFO 内，为 **完整的短源返回**预留容量后，允许一个 normal-memory 源 child 先返回到该 FIFO，即使其中的后一拍对应后面的已承诺 B1 W。这里 B 完整返回为 2×8 B，容量为 3×8 B，能够预留整段。B 的 own READ 和两个目标片段 own WRITE 都已成功，seq/epoch/PA/属性稳定。

这需要改变旧 source candidate 的“只允许连续 W 前缀”条件，添加实际已预留容量的占用/释放状态，不能只删除门。容量预留、剩余 R 计数、父/child byte offset、返回目的 FIFO、own READ/WRITE 成功和完整 attrs 都必须可检查。不是增加 payload 容量；控制状态与新反馈布线仍有成本，本文未综合、未给 FF/LUT 数。源 READ 与目标 WRITE 长度可不同，但真实父字节次序与每 ID 返回次序保留。

这个 full-return 读入私有缓冲的能力本身是普通有限先读/完整容量预留，候选与每个普通策略均拥有。不能把改变 source 门带来的改善全部归给新调度。

**不是生产唯一 route 的声明：**生产有多 iDMA 与多个 outstanding；这里使用 §39 研究原型的一 global source-child 资源，作为一个修改后的有限架构合同。新机制没有改 Linux、BootROM、BIT 或实际 MIG。

## 3. 完整有限合同

### 3.1 作业、W 与缓冲

| 对象 | 有限合同 |
|---|---|
| A | 独立 normal-memory copy，源 32 B=4×8 B；目的 A0 的 W 为 4 拍 |
| B | 独立 normal-memory copy，源 16 B=2×8 B；目的 B0、B1 各 W 1 拍 |
| 已固定的物理/虚拟 W 顺序 | B0→A0→B1，同 owner/seq 配对，所有策略相同；不再发新 AW |
| 起点以前 | B 的完整两拍源 AR 已实际握手；第一拍于 c=-2 实际 R 握手，B0 于 c=-1 实际 W 握手完成 |
| c=0 的状态 | 当前 W 头是 A0；B 的第二源拍还未返回，其 RLAST 占着唯一源 child route；A 的确切源父请求已登记，因该 route 忙尚未首次发出物理 AR |
| B 尾拍 | 自 c=1 起在固定存储服务中 eligible，直到被实际 R 服务；c=0 没有已持有但未握手的 B RVALID |
| payload | A3+B3=6×8 B=48 B，各自私有；没有新增共享 payload/skid；B 整个短返回容量事前预留。A 每拍 R 在下一周期由当前 A W 消费，峰值≤1 拍 |
| route 释放 | B 的真实 RLAST 于 c 握手，仅从 c+1 起可发 A 源 AR；不得靠“预计返回”提前重用 route |
| W 消费 | A 真实源拍 c 入 FIFO，最早 c+1 握手到当前 W 头；B1 尾拍即使更早入 B FIFO，也只能在 A0 真实 WLAST 的下一周期输出 |
| identity | A、B 源地址及 payload 不相同，不做 shared read；每个父/child 都有 owner、seq、epoch、精确 byte interval；成功 READ/WRITE 授权均为 own，未用对方 recipe |

地址可选为 B 源 16 B 对齐、B 目的距页末 8 B，以形成一个两拍源片段与两个一拍目的片段；A 放在独立页内。实际 iDMA 构造是否采用此切分必须另核，本文只允许这个 byte/beat 合法的架构合同，不声称旧 compact 已产生该握手序列。

### 3.2 可控接口、固定服务与逐周期语义

1. 共享 DDR 目标每周期最多接受一个新的物理 AR；可选择 source-data 或 DS/PTW 请求，或不发。选择器拥有同样的两槽**未首次 VALID 的 DS 描述符**缓冲，带 request identity、owner/walk identity、level。冷例 U 返回后，尚未发出的 N 可等待，L 可先选；不是撤销已 held 的 AXI VALID。
2. 一个请求首次成为共享目标 output ARVALID 后，地址、ID、长度、属性保持直到 ARREADY；其后不可撤销。原有 input VALID 也保持；调度所选对象为描述符，而不是把已经 offered 的 N 换成 L。每 ID 已接受请求的响应顺序不变。U/L 用 A walk identity，N 用另一 identity。
3. 全部策略共享以下固定下游服务：新 AR 于 c 接受，最早从 c+1 起返回；同周期 eligible PTW 优先于 source-data，PTW 内按已接受序，data 内保持 burst/ID 顺序；每周期最多一拍 R。c=1 的 B 尾拍可因此被后来 eligible 的 PTW 延后。已形成且被 backpressure 持有的 RVALID/data 永不切换。
4. 这是为了检验准入动作的**抽象共享服务合同**，不是“真实 MIG 识别 PTE 并恒定优先”的事实。请求可能因 QoS、row-hit 或既有控制策略获得更高服务优先级，但本文没有测定 MIG 的该轨迹。若实际服务不会让新 PTW 延后这个尾拍，热态收益即不存在。
5. 一次 U（上级 PTE）于 c R 握手，L（叶级 PTE）仅在 c+1 才准备；L 于 c R 握手，full postlookup success/own READ valid 仅在 c+1 生效，此后才允许 A 物理源 AR。不能把 PTE 到达前当作源 READ 已授权。
6. 所有 RREADY 均由真实容量支撑，本例为 1；ARREADY 与 WREADY 为 1。W 与 R 可同周期独立握手，但 FIFO 新入拍到下一周期才可 W 消费。B responses 可统一在相应 WLAST 后 1 周期出现，仅加相同常数；下表指标是 **真实剩余 W 数据债闭合周期，不是 iDMA DONE 或板端耗时**。
7. 每笔 DS 与 source 请求都有实际 beat/byte 账：热例总 4 笔物理读事务（B 两拍 + X 一拍 + N 一拍 + A 四拍）；冷例 5 笔（B 两拍 + U 一拍 + L 一拍 + N 一拍 + A 四拍）。U→L 必须串行；所有完整 copy 字节各仅输出一次。背景 X/N 的完成也列出，不能把它们从代价消掉。

### 3.3 热/冷输入的唯一差别

- **热头：**A source own READ 与 A0 own WRITE 已完整成功；X、N 是两个不解锁当前 A0 的后台 leaf PTW 描述符，c=0 都已进入相同两槽选择缓冲，X 比 N 老。
- **冷头：**A0 own WRITE 已成功，source own READ 缺失；以 A 的 U 取代 X，N 相同；U 于 c=0 pending，L 只能等待 U 真实返回才形成。A 不允许拿旧/对方许可或猜 PA 直接读。

除这个 READ 成功与 walk 请求差异外，source route、W 序、payload、AR/R 服务、输入容量与所有目标不变。

## 4. 正向规则与公平/不可撤约束

候选增加一个有限 `protect` 记录：当前 W owner/seq/epoch、该 W 精确未齐字节范围、own source parent、是否处于 blocker-tail→own-source 阶段、剩余保护量、H 计时与一次用尽标志。旧 route owner/child 及 B 的完整剩余容量预留用于确认保护不会让已承诺 R 无处返回。Q=5 是保护的 B 尾拍 1 + A 源 4，不是延迟保证；H=16 是准入公平上限，不是假造 CQ/slot lifetime。

| 可见状态 | 动作 |
|---|---|
| A READ cold；own U ready | 普通 own-walk 链优先：选择 U，不选无关 N |
| A own walk 在途，下一层尚未 ready | 无关 PTW 暂不首次发出；不阻塞已经接受的响应；等待真正的 own 下一层 |
| A own L ready | 选择 L；真实 success 后转到源数据债阶段 |
| A READ/WRITE hot；源未齐；B 占 route 且其完整剩余返回预留已成立 | 暂缓新无关 PTW AR，保护旧 B 尾 R；不对 B 的已选 R 下命令 |
| B 实际 RLAST 后，A 源 AR ready | 延续同一保护，允许 A AR 首次发出并稳定到握手 |
| A 源 AR 已接受但源 RLAST 尚未握手 | 延续准入暂停，保护实际 A 数据返回；不以 A AR 握手即释放保护 |
| A 所需真实源 RLAST | 下一周期解除保护；后台 DS 恢复，随后仍按固定 W 顺序完成 W |
| 证书不成立、seq/epoch 不匹配、Q 无法有限描述、容量不能支撑旧 R | 不启动/停止阻止新的请求；不能用未授权 PA 或取消旧 AR/AW/W/R 解决问题 |
| H 到期或最老 DS 达 H | 解除准入暂停，至少让一次已等待最久 DS 首次发出；该 W seq 本次额度消耗，不能立即重启同一保护形成无限暂停 |

授权失效等边界不能回收已经发出的数据债，需执行既有 drain/错误协议。本文只用稳定 successful normal-memory 合同；不提出新 remap/fault 恢复。背景 DS 的老化强制一次 admission、已有 VALID 锁定以及 H 是所有准入屏障的公共约束，候选不能享有更大 stall 额度。

## 5. 最小判别表及强普通复现

### 5.1 热头：4 笔读请求

下表中的 `B*` 是已经接受的 B 源 burst 尾拍，`A0…3` 是 A 四拍源 R。空白/`—` 是没有新 AR 或没有 R，不是暂停已经 held 的 VALID。

| c | work-conserving PTW-first 新 AR | 其固定 R | 其实际 W | 候选新 AR | 候选固定 R | 候选实际 W |
|---:|---|---|---|---|---|---|
| 0 | X | — | — | — | — | — |
| 1 | N | X | — | — | B* (RLAST) | — |
| 2 | — | N | — | A(4拍) | — | — |
| 3 | — | B* (RLAST) | — | — | A0 | — |
| 4 | A(4拍) | — | — | — | A1 | A0 |
| 5 | — | A0 | — | — | A2 | A1 |
| 6 | — | A1 | A0 | — | A3 (RLAST) | A2 |
| 7 | — | A2 | A1 | X | — | A3 (WLAST) |
| 8 | — | A3 (RLAST) | A2 | N | X | B1 (WLAST) |
| 9 | — | — | A3 (WLAST) | — | N | — |
| 10 | — | — | B1 (WLAST) | — | — | — |

work-conserving PTW-first A/B W 闭合=9/10，背景 X/N R=1/2；候选=7/8，背景 X/N R=8/9。前者平均 W 闭合 9.5，后者 7.5；这是将背景 walk 延后换来当前两份 W 数据债早闭合，不能写成没有代价的整体 throughput 提升。

ordinary next-W-owner 仅给当前可发 own 请求更高优先级、否则按 ready queue 工作：c=0、1 的 A source 因唯一 route 不可发，A 又无待发 own walk，故会发 X/N，与表左相同。**若它同时对当前 W 数据债设置背景准入屏障，则已经是下一节的同轨迹强普通，不应藏起来。**

### 5.2 冷头：5 笔读请求；不能以 MASK 全 PTW 优先为唯一对照

候选 cold 分支采用普通 own-walk 链优先。U 返回后，N 尚未首次成为 output VALID，所以可以等待 L；不会违反 held VALID 或已接受同 ID 的顺序。

| c | work-conserving PTW-first 新 AR / R | 候选 own 链新 AR / R | 无条件 data-child embargo 后 PTW-first 新 AR / R |
|---:|---|---|---|
| 0 | U / — | U / — | — / — |
| 1 | N / U | — / U | — / B* (RLAST) |
| 2 | L / N | L / B* (RLAST) | U / — |
| 3 | — / L | — / L | N / U |
| 4 | — / B* (RLAST) | A(4拍) / — | L / N |
| 5 | A(4拍) / — | — / A0 | — / L |
| 6 | — / A0 | — / A1 | A(4拍) / — |
| 7 | — / A1 | — / A2 | — / A0 |
| 8 | — / A2 | — / A3 (RLAST) | — / A1 |
| 9 | — / A3 (RLAST) | N / — | — / A2 |
| 10 | — / — | — / N | — / A3 (RLAST) |

对应真实 W：

| 策略 | A 四拍 W | A/B 剩余 W 闭合 | N 实际 R |
|---|---|---|---:|
| work-conserving 全 PTW-first | 7,8,9,10 | 10/11 | 2 |
| 候选 own 链＋数据保护 | 6,7,8,9 | 9/10 | 10 |
| 无条件旧 data-child embargo 后 PTW-first | 8,9,10,11 | 11/12 | 4 |

第三策略的失败原因具体是 c=0 未发真正解锁 A 的 U；不是“数据优先一定差”。若普通 data-priority 也带 own READ miss 检测与链优先，就会趋向下面的强普通。

### 5.3 决定性强普通：当前 W 头屏障，同状态、同信息、同容量

**完整规则，不用给任意万能对照增添新模块：**

1. 观察当前已承诺 W owner 的精确源数据缺口、own READ/WRITE 成功与 own walk identity；这些候选已经读取，普通策略使用同接口。
2. 如果该头还有源数据缺口：own READ cold 时，只允许其 own U/L 等真正已 ready 的 walk 首次 AR；自己的下一层在途尚未 ready 时，不发无关 DS。own READ hot 时，允许真正 ready 的 own source AR，暂不发无关 DS。
3. 直到所需源字节真实到齐才解除。共享接口已有 immutable W seq、source-parent offset 与 R 实际计数，不需要查询“谁占着 A 前面的 route”。已 offered 的 AR/R 一律保持，保护量/超时/老化强制 admission 与候选相同。
4. 本有限架构所有允许提前的短 child 都经过相同完整返回预留；因此后台 B 尾拍会自行完成，它不需要 A 的 W 先 drain 才有 RREADY。本例不存在屏障被别人的未知许可/缓冲债卡死。

**热态构造等效：**c=0 当前 A W 有数据缺口、own READ hot，因此普通屏障不发 X/N；唯一 route 仍 B，但它不需要知道 B identity。B* 自 c=1 返回；c=2 A source 真正 ready，发 AR；一直不发无关 DS 到 A 源 RLAST=c6；c7、8 发 X/N。与候选表右完全相同。

**冷态构造等效：**c=0 发 own U；c1 own L 未 ready，不发 N；c2 发 own L，同时 B* 返回；c3 L 返回；c4 full own READ valid，发 A source；到 c8 源 RLAST 前不发 N；c9 N AR/c10 N R。与候选列完全相同。

这不是“普通策略也可以任意添加提案的模块”的空泛否决：它是只依赖 already-public W-head/own-recipe/own-byte-ready 的具体三分支 guard，删除了候选独有的 B blocker identity、两阶段 baton 与 dependency-chain 追踪，容量/授权/timeout 相同。**因此候选的额外链记录在本合同不提供额外能力。**

理想 total-data-priority R server 可以在不改 AR 的情况下减少此类干扰，但其 actuator 与本文不同：它要改变存储服务。不能用它证明 AR-only 成本相同；同时，也不能因为本文无权改变 R 就把“优先服务解锁当前 W 的数据”认作新原则。上述 AR-only 强普通已经足以否决本例的独立算法。

## 6. 前作关系、真正未解约束与停止门

- [MASK ASPLOS 2018，§5.4](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf) 的 Golden PTW 最高优先级及 Silver 公平服务是一个真实翻译优先参照；本文依赖 walk/数据耦合，不能把击败单一全 PTW-first 当作击败当前 W-head 控制或整个 MASK。
- [BIS ASPLOS 2012，§3.5.1](https://people.inf.ethz.ch/omutlu/pub/bottleneck-identification-and-scheduling_asplos12.pdf) 已沿阻塞链把等待归到实际 bottleneck，再用快核加速。它的具体执行对象是线程/临界区等，不是这里的 PTW AR/R 仲裁器；但“等待者把重要性传给实际阻塞者”的原则已存在。本例映射为 A W waiter→A source parent→B source route holder→B R tail，释放后接 A source，cold 则是 A W waiter→own source READ walk U→L。普通 inheritance 若已有这些边与 actuator，选择同一请求/屏障，不构成新优先级原则。
- root 正在核 NoC/router priority-inheritance 前作（[IPSJ 2013](https://ipsj.ixsq.nii.ac.jp/records/87950)、[JSA 2019 DOI](https://doi.org/10.1016/J.SYSARC.2019.101664)）。本文不把它们未出现 IOMMU 名字当成新颖性证据，也不在未读其完整实现前声称已经逐模块覆盖新接口。
- AMD 的 MIG 文档检索说明有 ordering/fairness 设置，但本次 [UG586 页面](https://docs.amd.com/r/en-US/ug586_7Series_MIS/Customizing-the-Core) 直接打开超时，**没有取得“Genesys2 当前 MIG 如何对这四/五笔 R 选择”的原始证据**。因此前面的固定 PTW-first server 只作为明确抽象合同，不能报告 MIG 本身 PASS 或瓶颈实测。

明确停止条件：

1. **当前停止：**同容量、同已公开信息的普通 W-head 屏障逐拍复现两例；新 wait-chain 规则的算法独立性已被本合同具体否决。不进行该合同 RTL。
2. 如果真实 MIG/互连不会让新 DS 延后旧 source 尾 R，本例改善为零，不人为调服务优先级制造收益。
3. 如果 source 门的 full-return 变化需要新 payload，或普通多 route/read queue 用相同/更低成本消除占位，必须计全部状态和缓冲后再判断，不能只报 AR 调度 FF。
4. 如果 future 精确 wait chain 能排除粗 head guard 的不可恢复 drain/authorization 环，同时保留更多无关 PTW 并有可测收益，可以把“可见义务＋受限准入接口”作为架构创新重新提案；必须有新的真实合同与强普通反例。此轮没有该正例，不能先预支论文贡献。

**给 root 的执行建议：**这一份文件已足够编写普通抽象模型，但手算与等效构造已直接关闭本候选的独立算法贡献门，优先把实现资源转给尚有明确差异的机制线。若执行，仅能称有限事件模型，不得称现 DUT 验证。
