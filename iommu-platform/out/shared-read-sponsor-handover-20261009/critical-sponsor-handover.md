# §57：旧 physical R 退休后修补、恢复双成员

Codex 独立关键架构审核。先根据 §56 基线推导反例，再直接核根的新 patch：`shared_pretl_read.sv`、`multi_actual_dma_top.sv`、`multi_system_top.sv` 与实际 `idma_axi_transport_layer.sv`。本稿没有构建、仿真或功能 RTL 修改，不能当作 copy PASS 或普遍无死锁证明。

**裁决：根当前修订版在指定静态正常单 job/q2 合同下排除了本稿发现的 held-R、W-head、错位子写及同沿快照反例，可以进入根的 actual 验证。** 关键不只是 sink：它先把 V 的真实 raw W 数据游标推进到固定偶数 J，再让 A 修补 W 精确止于 J，从 J 开始以双方实际 W 游标限制下一 q2。这三项共同建立了 repair 的可服务边界。

## 1. 精确合同和状态

每流一个 source raw AR256，2KiB、完整 8B 拍、src 页对齐；V 目标页对齐且一个 WRITE raw parent256，A 目标 offset0/3072/4088。两独立 READ recipe 成功、初始 PA 相同和 physical memory 属性相同；源不可变。两个实际 WRITE parent 各自授权成功，V WRITE 覆盖256，A下一页 WRITE成功后累计覆盖256。无错误、动态失效、外部 pause、多 job或全成员停服。每流真实 private FIFO B3，既有一份 shared skid，不新增 payload。强制保留短 q2 W，不能把 repair 期间 A 的 child 放大为255拍。

记 `r_f` 为 DMA 已接受 logical R 的 job 全局拍游标，`w_f` 为实际 raw W 已取走 payload 的游标；`E_f` 为 qwriter 本地已firstVALID child的绝对末端。该三个量的实际握手与库存证明已在 §56 `critical-data-obligation-exit.md` 核过。本轮又直接读 transport `:255–270`：R 真握手才入 FIFO，logical RLAST 才 pop R metadata；`:392–438` 的 WLAST 单独由 actual WRITE长度计算，`:451–455` 的 B接受不等 READ RLAST。

新增声明是 `cutover_phase_q[1:0]`、`v_cut_q[8:0]`、`old_end_q[8:0]` 共20bit（source `:70–71`）；old_end 目前用于观察输出，是否可去掉不影响本次机制审核。无综合面积或完整总资源结论，公共 parents、certificates、FIFO及计数器仍需双方计费。

## 2. 两个最强原始反例

**held physical R：** 单物理响应通道中，先停 V 的 logical RREADY，再发 A gap AR，合法 memory 可以把旧 V RVALID held 在前。A gap 等这个 held V 响应离开；V又等 A 修补后才接收，成环。RR 只能改尚未 offer 的响应顺序，不能撤 held RVALID。本方案不发第二 AR 争抢：phase2 接收并丢弃旧 AR 的真实剩余响应，先释放该通路。

**W-head：** FA1、J2，若解除 A 的旧 V-RLAST fence 后准许 A long W[1,256)，此 AW可以在未来 V W前。R1仅A，后续双返回使V小FIFO填满；V W等A long W完成，A long W又缺因V满而收不下的 shared R。keep-q门是实际必要条件，不能只保证 initial qW。

**即使 q2，也有错位反例：** FA1、J3、V w=2，V保留一拍未承诺 payload。A repair先完成[1,3)，然后offer[3,5)；V需[2,4)。R3双返回后 V库存2，V[2,4)排在A[3,5)后；R4无V AW覆盖、基线借槽阈值2阻接，A缺R4不能WLAST，V又等A WHEAD，成环。不能只冻结 V缺payload W，任其旧尾部长度错位。这促成根的偶数 J 与实际 `wV==J` 修订。

## 3. 触发和 close 阶段的实际门

当前 patch `shared_pretl_read.sv:95–104` 用

`J = ceil(max(rV,E_V)/2)*2`

且要求 detached、旧 physical child busy、无 skid、两源 READ cache仍有效、两路成功 WRITE end256、J<256、J<=旧 physical child end。J=256回退继续旧V，不做无尾部的cutover。旧 child offset/count 自身已经提供 oldEnd；J固定后不追着新promise无限增长。

`v_hold_o` 包含**组合 start_cutover本身**（`:90–91`）；top `:712–716` 只允许其 `next_end<=rV` 的全数据 child继续。故触发同拍任何准许的新 child末端<=rV<=J，不会漏进超出J的firstVALID义务。`start_cutover` 同时把 physical RREADY 置0（`:133`），避免锁存J的同拍又建一份新的logical skid。已经held的physical R只延迟ready，VALID/data未改。

进入phase1后，V继续 logical R直到J，但 `beat_index>=J` 时明确停止（`:132`）。已有firstVALID E_V<=J；尚未承诺的 q2 在数据全到后才能新offer。对于需要补一拍才能凑偶数J的情况，已存在、已拥有完整payload的旧 W会正常服务腾出FIFO，之后才能把该拍交入；不靠memory永久偏袒任何流。

真正转sink要求 `!skid && rV==J && wV==J && rV>=E_V`（`:222–225`）。因此V的 `[0,J)` 所有payload都已离开DMA，旧 W/B仍可能在TC/memory，但未来进展不再依赖V R。不存在旧FIFO一拍尾部冒充可用credit的问题。

这个转移也没有“空skid但同拍再收R”竞态：当条件rV==J成立且skid为空时，物理下一拍索引就是J，phase1的末端门已经把RREADY关掉。qwriter的同拍新child也需next_end<=rV，而它若从wV=J开工必超过J，故不能再创建。

## 4. sink 不取消 AXI，不提前结束 DMA

phase2 `required=0`（`:114`），但 `busy&&!skid` 时 RREADY明确为1（`:134–136`），不依赖DMA FIFO或WRITE前沿。physical R握手仍核原 child RID 和正确位置RLAST，非OKAY fatal（`:285–288`）；只推进原 `child_got_q`、在真实RLAST清busy（`:291–295`），不建logical skid、不增任何returned/done。

旧 AR已经accept或首次VALID后，其 child字段保持不变，直到busy、offered、skid全部清空，repair builder才覆写（`:226–238`）。没有早撤 ARVALID、没有取消 memory 响应，也没有把physical RLAST伪装成logical RLAST。短K若J==oldEnd，旧busy已经清掉时phase2可直接builder，不等待第二个不存在的RLAST；首轮oldEnd256/J<256则确有tail可sink。

实际iDMA的 R metadata一直保留：`idma_axi_transport_layer.sv:268–270` 只有 **logical** R.last真握手才pop；`:242–247` 的 first-read标记也仅在该逻辑接口更新。因而原 raw AR256可以在一段logical前缀后等待，再由另一次physical read补完；coalescer拥有这一转换，iDMA没有看到被截短的raw AR或缺失的logical R。

这只适用于允许合并、重复读取的 normal immutable source。physical tail error不能默丢成成功；当前代码明确fatal并排除该合同。故障恢复/动态授权撤销需要另有协议，不能由本稿推出。

## 5. repair 的权限、ID、返回连续性

phase2只有无旧physical义务和skid时才发repair，使用A原 parent字段、A实际READ PA加其当前returned offset，长度256−rA（`:228–238`）。两READ cache仍有效、两WRITE成功覆盖256是builder门。首次共享时的PA和cache/prot/qos/region/user一致检查仍在 `:246–252`；static单job中两源recipe没有epoch改变，不能用A WRITE权限替V READ权限。

repair beat<J仅送A；从J开始 `required=11`，覆盖detached对旧成员位的抑制（`:112–115`）。**detached保持真**，不拿历史q groups重新判相等。physical RID是A child，响应按该RID核；logical ID与LAST重新从两原parent分别生成（`:138–142`），并且每个required cursor必须等于当前beat_index（`:289–290`），只在各自真实logical握手时增加returned、处理RLAST（`:297–301`）。

所以A收到 `[0,FA)` + `[FA,J)` + `[J,256)`，V收到 `[0,J)` + `[J,256)`；没有重送V gap、没有漏A suffix。physicalAR length和logical原长度可以不同，最终各original parent仍恰有一次真实RLAST。

## 6. 为什么修订后的 joint W 门排除第二层环

phase3 A修补prefix的最后child截到J，top `:698–703`；因此A可能最后用q1，接下来的双方W都从偶数J走同q2网格。phase3 V在A logical游标尚小于J时只准全payload W（source `:90–91`），A旧V logical-RLAST fence则解除（`:88–89`）。原W/B门本身未撤销任何existing firstVALID。

更强门：joint新child start>peer **实际raw_written** 时，quantum=0（top `:704–707`）。qwriter只在next_count非零且无existing child/B时注册child（§56 qwriter `:83–90`）。这是实际数据已取走的前沿，既不用晚B计数，也不用旧groups编号。

在第一joint边界：wV=J，A修补prefix只有数据自足的V旧W可能在前；这些旧W不需未来R，B虽可迟到但没有新增读依赖。V新child从J开始要等wA>=J，因此不会抢在A缺payload的prefix之前。两FIFO都没有V旧尾部；source可以先给两个joint拍，即使某个旧B暂未回来，基线两拍借槽也足够保存一个q2。

此后令k=J+2t。某流可以commit[k,k+2)，但若peer w仍为k，它不能再commit下一[k+2,k+4)。已在先的child只有两拍，并且每次physical R都经同一skid给完两member才收下一拍；先流若已经消费完整q2，则peer也已经获得完成其旧q2的全部payload。peer的旧W或B因此只等正常服务，没有“等下一组R才能交还W队首”的边。peer推进后下一组才能共同开放。早B、迟B、不同physical AW接受时间会改变等待，不改变该数据覆盖关系。

这是一条指定结构下的进展论证；需要正常AXI执行及既有same-bank TC保序/最终服务，不能推广为任意memory/任意routing的cycle期限。原held AW/W输出不引用新hold/quantum来撤valid；新的quantum0只阻未firstVALID的child。

## 7. 强普通与本次增量范围

固定首physical child N256已firstVALID后，全tail是真不可撤义务。sink只把该tail的logical用途丢掉，依然接收完physical R。若A原cut为FA，修补长度N−FA，所以候选总physical源R为

`N+(N−FA)=2N−FA`。

这与等待V logical RLAST再A补读相同，不随J减少；两种都可用2个physical AR。它的具体新动作是把V `[J,N)` 的logical服务迁移到后一次repair中的双返回，换取不再等全部V qW/B-paced logical消费后才开始A。V可能因此变慢，A可能变快，收益必须由根的actual结果决定。

强普通variable-K可让旧V source child较早到真实RLAST，再A补gap、双返回suffix，避免sink整段旧长tail；可少R但增加AR/边界等待。它应获得相同permission和cursor信息、相同W安全门与完整计费。不能用“普通也可抄算法”直接否决此接口机制，也不能把只与旧固定读/W策略比较所得改善当作已证实创新。

如果actual对照显示在同资源、同权限与进展合同下，这个接收旧物理义务而迁移未来logical服务的动作达到明确的V/A性能或状态成本差异，就可以讨论相应增量；因为当前仍未执行且普通variable-K尚待比较，本稿只接受其可实现、可核验的安全架构，不宣称新理论、独占收益或论文等级。

## 8. 根下一次actual验收应抓的必要事件

不需要为本稿另跑。根的计划执行至少应记录真实A WRITE2成功、start/phase1/J、wV==J、sink首末physical拍/原RID/旧RLAST、repairAR、A在J的prefix W截点，以及各logical完整copy/未写区检查。必须区分sink拍与logical返回拍，并核firstVALID稳定与两original RLAST各一次。

重点反例覆盖是odd rV触发、start同拍准备新V child、phase1末拍与skid握手、oldV B延迟跨repair、A FA1最后q1、joint某流B先回来。未触发或J==256的fallback应继续旧V原轨迹，而不是被新门半途冻结。上述均为建议/手工推理，未由本代理运行验证。
