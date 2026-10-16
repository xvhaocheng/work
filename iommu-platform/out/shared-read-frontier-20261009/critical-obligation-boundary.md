# §51：read quantum 与暂停义务的决定性边界

Codex 独立推演，2026-10-09。只读 §50 进展/验收稿及两个控制器，不执行构建、模型或 DUT，不改其他文件。下述新 trace 是纸面可达 AXI 序列，不冒称已实测。§50 已通过的12个 copy和曾经的死锁由根级验收稿记录，本席不重跑。

## 判定

**有另一个成员持续接收时，max read quantum≤q 不是 A 的≤2q 新增 logical R+W drain 的必要条件。** 该界来自 A 唯一不可撤的 q-W 末端，以及所有发给 A 的 R 都位于该 coverage 内；共享 physical AR 可以任意合法长度 L。当前强普通 mode2 已有同样 recipe、parent cursor、skid、成员退出和 q-W adapter；放开它的 AR 长度选择即可同合同、同资源实现，并非必须增添新的退组架构。

**如果合同还要求最后成员暂停后，physical AR/R route 在最坏相位内以≤2q 的 R+W 数据工作释放，则单个已承诺 AR 的 L≤q 是必要条件之一。** 最坏相位是 AR L 已 offer/accepted、q-W 已首 VALID、尚未供应 R/W；物理工作下界 L+q。即便 drop 尾 R，也仍要接收这 L 拍。多 AR outstanding 要约束总剩余量，不是每条 L≤q 就足够。只有公平而没有最大服务时延，不导出有限的统一 cycles 上界。

因此普通控制可以用长 R 达到当前 A 一次暂停、V 继续的合同。若进一步要求任一成员重复暂停/所有成员暂停的物理让路，候选本身也必须承担相同的尾 R 约束，不能只给普通方加 q 上限。

## 1. 固定计量与真实源码边界

以8B payload beat计量，q 是每流一个未退休 W child 的最大拍数。取暂停边沿后闭包：同边沿已经握手/首 VALID/注册并将持续 VALID 的请求全部计入；hold 不撤旧 VALID。定义三种不同的 quiet：

| 合同 | 必须清理 | 可留下 |
|---|---|---|
| A 逻辑服务 quiet | A 已承诺短 W 的所需 R、W、子 B，以及已经冻结给 A 的 R beat | A raw AR suffix、原长 AW/原 B、原 job 和 recipe；由 V 继续的共享 R |
| physical route quiet | 相关不可撤 AR 的 RLAST、held R、路径中的已接收 payload、已承诺 W/B | 私有 parent/recipe；但不能留下占该共享返回路径的尾 R |
| backend_idle / job done | 原 raw parent 的所有 R/W/B 和 job 完成 | 本合同中的原 job 剩余不能留下 |

第三项不能在正常暂停且原 copy 未完成时从前两项推出，任意 read quantum 都不能把尚未返回的 raw parent / 原 B 说成 backend_idle。若以 RID 归属定义“暂停者物理 quiet”，也不能把仍带原 RID 的 shared R 自动改名为另一个成员；逻辑退组与物理 header/tag 退休需明确区分。

真实实现的不可撤与覆盖事实：

- [q_write_splitter.sv:29](/home/xvhaocheng/work/iommu-platform/out/shared-read-rtl-20261009/q_write_splitter.sv:29) 的 AWVALID=offered_q；64–70 的 hold只限制**新**child创建，49–54检查旧AW/W不变。每流只有一个 child_live，81–90 收子 B 后才放下一组/最后原 B。
- [shared_pretl_read.sv:121](/home/xvhaocheng/work/iommu-platform/out/shared-read-rtl-20261009/shared_pretl_read.sv:121) 由真实全局 virtual AW first offer 提升 coverage；68–71 只接收落在所有 required 成员 coverage 内的 R；172–185 冻结 skid mask并按各自 logical handshake推进 cursor。
- 94–97 检查 held physical AR 与 held logical R不能撤改。所谓退出只修改尚未为 A 冻结的后续 beat membership，不修改已发 physical ARlen。
- 125–128 的首次 detach要等A短W/B quiet且无A skid；当前只有一次detach，150–164 的补读是A单独剩余长 R。q writer68–69 当前在detached后也把 W suffix扩大，所以§50源码本来不提供第二次pause的q界。

下文“重复暂停”的证明对象是根席准备的强化合同：所有阶段都继续保持每流唯一 q-W、可冻结/解除成员位，不是声称当前§50 RTL已有重复暂停。

## 2. 精确的 A 义务闭包，与 ARlen 无关

设暂停时 A 当前不可撤 W 的末端为 e，之前各 child已收B；a是当前W起点，e−a≤q。r是A已经logical R握手的前缀，w是当前协议口已供应的W前缀。只考虑正常copy、源/目标对齐且每个W beat由对应R beat供应。当前firstVALID逻辑R/已经冻结skid的末端h也计入；coverage规则保证h≤e。

暂停后禁止A新的W child和不相关准备；旧AW/W/R照常兑现。最小 continuation 是供应到e，不是供应到 physical AR的末端：

\[
R_A^{new} = \max(0,e-r) \le q,\qquad
W_A^{new} = e-w \le q,\qquad
R_A^{new}+W_A^{new}\le 2q.
\]

已经返回/已经接入公共FIFO的payload并不因暂停变成新的logical R；已经给A冻结的h也不能丢掉。计量是新增payload操作，另有一个child B及AXI header/control事件，不能把B免费写成0 cycles。W前缀若在不同物理级使用，应固定计量口；从raw W握手到physical W退休的已在途payload仍要排空，不能重复遗漏或重复计费。

实现这个上界所需的进度前提也必须同给候选/普通：两个共享成员旧W child均受q限制，q payload有实际停车空间，global W顺序下前一个成员能消费其数据并排空，真实WRITE recipe已覆盖必须执行的目标区间。只有A的q限制、前方V却持有未覆盖的大W，仍可能发生§49的有限FIFO/Whead环；单看A字节数不能证明上界可完成。Cycles还依赖V前缀、AW/W/B ready和仲裁时延。

到e并收齐旧B/heldR后，A退出未来physical R的成员mask；若V持续接收且未来WRITE已授权，V使同一AR继续到L，A的logical parent只保存cursor r=e供resume补读。这里L未出现在A的义务公式里。

### 最小否定“必须L≤q”的trace

q=1，两个成员私有B≥1，页内READ/WRITE已分别成功，无invalidation，normal immutable DRAM。

1. V/A当前1-beat AW均首offer；V在W序前。
2. shared AR长2beat已accepted，L=2>q。
3. A提出一个有限pause，冻结新AW；V继续。
4. shared R0分别返回V/A，先排V当前W/B，再排A当前W/B；A只需新增R1+W1=2=2q。
5. A退组，V下一1-beat AW首offer；同一shared R1只返V，物理AR完成。A余下raw R保持私有，后来resume补读。

所有VALID可稳定不撤，没有非法W顺序，也没有不公平内存或永久关闭budget。将2换成任何页内合法L，只增加V后续工作，不增加A这次的logical drain。

## 3. 最后成员第二次暂停：物理尾 R 的下界

physical AR从firstVALID后不可改，accepted后必须接收规定拍数至RLAST。退组后的尾R只有以下合法去处：①继续logical供应/W；②私有停车且保留内容；③正常immutable DRAM下接收并丢弃，保rawparent cursor以后重读。③不撤physical AR，也不推进未logical交付的cursor；它节省logical A drain，但仍消耗尾部physical R工作。对device/side-effect/源会修改/不可重读合同不允许凭空采用③。

在pause瞬间，若一个sole physical AR还欠L个R beat，一个已首VALID q-W还欠q拍，physical route彻底释放的payload工作至少L+q。故要求每个最坏相位≤2q，需要L≤q；若还有o个已offer AR，则用其剩余总量ΣL_i替代L。普通同效实现是每流q-W不变，source AR只承诺不超过q的、已覆盖片段；维持同样permission recipe和logical cursor，不需要新“续命”状态。

### 真实parent范围内的最小严格超界反例

仍用2KiB/256-beat raw parent，而非换成不被原型支持的小job。令q=1，A此前252拍已经正常写完，V已经完成；A还有4拍。在A第一次pause/resume之后，A独自补读suffix：

1. A短AW覆盖beat252，len0，已首offer/accepted；A新的physical AR覆盖252..255，len3，L=4，已accepted。
2. 在第一个R前A第二次提出有限20-cycle pause，停止新AW；这是合法正常事件序列。
3. 必须返R252并完成当前W/B，A逻辑服务只欠R1+W1=2。
4. 物理AR仍欠R253..255共3拍，**仅尾R已超过2q=2**。从pause时刻计完整物理payload工作则R4+W1=5。

如果只保留coverage gate、不增加尾R drain，下一R253等未offer的AW；pause禁止该AW，故physical route至少堵到20-cycle pause结束。不是内存不公平：memory可一直VALID，controller自己不给READY。如果给尾R drop/drain，则能释放，但真实3拍尾工作仍存在；若仅B=1停车，全部尾3拍不能同时私有保留。之后恢复可继续原copy，整个工作负载有限，不需要永久pause来构造反例。

若V也允许独立暂停，AR发出时“还有V active”不保证V持续到该AR结束。两个成员都在中途有限暂停，也会到达同一种最后成员边界。候选和普通须约定一致：只保障单方退出且另一方继续，或保障任意/全部成员暂停；不能把这两个合同混在一个公平性结果里。

## 4. q上限也不是独立的充分条件

即便AR量子=q，如果它已offer而对应W还没有first offer，pause禁止新AW后，当前coverage门仍可能永远等未承诺目标W。公平的内存不会解除这个局部门。故物理释放还需至少一项：AR admission本身保证全部已offer R都有可兑现W覆盖；或者有实际**free**停车容量；或者允许上述接收丢弃/以后重读。B总容量不能当作free容量：旧q payload尚占FIFO时，新q预读再停车需要另计。

当存在一个已offer AR、它的全q范围已被不可撤且可推进的q-W覆盖、没有同route新AR越界、mapping/recipe有效且ready公平，短AR+短W可有限退休；仍不保证统一cycles。任何宣称“≤2q cycles”都还须有AW/R/W/B及前方服务的显式最大时延，不是仅有AXI正确性或公平。

## 5. translation约束与最强普通构造

§50的实际新依赖是run02：长R将下一destination walk挡在返回路径后，而R又等此destination的未来W。单页四recipe门切断它；候选/普通都使用。对未来多页扩展，要么限制accepted R到仍将继续消费者的连续READ/WRITE成功前沿，要么允许尾R排空丢弃/充足停车后重读，以释放PTW服务通路。仅压read quantum不自动避免未准备目标页；仅member退组也不能帮仍需下一页的最后消费者。这个必要约束是真实translation边界，但当前已有translate-first控制同时满足，不能以它单独判定新贡献。

最强普通在当前窄合同的具体实现无需“凭空加候选模块”：现mode1/mode2共用全部四recipe、rawparent/header/cursor、skid pendingmask、detach/fence、q-W+B合并。shared read143–145目前唯一区分是ARlen和child_beats取Q或remaining。把此选择扩为 `min(K, remaining, prepared/legal frontier)`，K可取1..256：有保证持续的另一个消费者时可K=256，sole且要求物理q-work让路时K≤q，或者明确选择尾R drop/drain。child header/beat counter本来就存在；固定K不增状态，动态配置K需要公平计配置/策略位。没有新增walker、payload buffer或免费permission信息。

如果目标差异只是“普通q2读、候选长读但两者同用退出控制”，那么上面的同接口可变量子普通构造覆盖该点，因为A逻辑2q界与K无关。不能继续用q2必要性解释§50的无pause时间优势。若后续有独立贡献，必须落到实际不同的合同/控制成本，例如在相同prepared信息、相同尾R预算下更好的持续服务调度；当前推演没有给出该差异，也没有以“任何算法都可被普通抄写”来否定整个研究方向。
