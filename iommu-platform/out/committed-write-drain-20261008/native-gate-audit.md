# §42 Native Codex：budget-off AW准入门与有限source继续审计

2026-10-08，`innovation_rules_40`。只读直接核§39 out源码及其实际AXI/iDMA依赖；未写控制RTL、未run/build、未改生产、未扩文献，未调用DeepSeek/Claude。root负责实际控制设计/实现及验收；本稿是并发临界接口的独立审核，不是硬件通过结论。

## 结论

**可以只冻结A的新destination准入、保留之前门后已可见的一笔AW，让source继续供给有限既有W债务。** 对当前CONF1、同bank、正常静态copy及§39 prepared-prefix child，它是普通可行接口，不需要原parent或job完成才能让路；但有限进度仍需真实AW/TL/memory服务和准确两W序，不能由门的位置自动推出H周期界。

最小安全AW门在top `619–622`复制`pool_m_req`到`slv_reqs`的位置，位于shared mux的仲裁、`w_fifo_push`和AW spill之前。门必须同时控制A的**门后AWVALID**与返回给A的**AWREADY**；维持以前门后可见却未握手的那一笔，禁止后续新笔。不能只把mux内部`lock_aw_valid_q`作为保留条件：RR仲裁器可能已经锁定A，尚未到W-owner push。

quiet至少检查最终AW gate-held已清、全部A虚拟/物理W承诺已到WLAST、无A已offer/busy child及held R、无A背景TL offer/slow；还要检查未计入accepted-AW ring的physical first offer和transport。**`i_pre.busy_q==0`绝不等于release。**

## 1. AW门：位置与最少状态

### 1.1 门放在A输入端；共享链内已存在的token照常走

§39 top `610–618`以独立perflow credit处理source AR；`619–622`复制其余通道到4输入mux，source AR已被置0。可在这里仅改变A的AW准入。W、RREADY、BREADY和所有已进入mux/TC/physical链的AW不在此处冻结。

定义run为新AW准入开启，h_A为上一拍“门后A AWVALID=1而門后AWREADY=0”。建议逻辑关系（设计约束，非RTL补丁）：

```
allow_A      = run OR h_A
AWVALID_out  = AWVALID_in AND allow_A
AWREADY_in   = AWREADY_out AND allow_A
h_A(next)    = AWVALID_out AND NOT AWREADY_out
```

其余AW字段始终从同一A上游请求传递。原DMA/fall-through register负责在VALID未握手期间保持全部字段；门应对该保持条件做观察断言。h_A为功能性保持状态，header副本只在需要诊断时记录，不能把观察header声明成免费功能存储。

预算关闭时：

- 门后此前没有待接收A AW：直接禁止新的门后VALID、返回READY0；门前DMA的pending VALID可以继续保持，它未成为新的共享W债务。
- 门后此前已可见但未握手：h_A维持它直到真实握手。这个请求可能尚未`w_fifo_push`，也可能已push而在mux锁住；两者都必须放完。
- 请求已经被AW spill接收：A上游可已前进，h_A不再保护旧笔；旧spill及其W-owner row自行继续，门仅挡住下一笔。

若A未被选中但其请求已呈现在`slv_reqs`且READY0，h_A仍需保持。`rr_arb_tree`锁住时保存整组未服务req，而非只保存最终winner；不能因A暂时不是winner就撤回该门后req。

### 1.2 几个明确错误位置

- `slv_reqs[A].aw_valid = input_valid AND run`：关预算可能撤回上一拍已可见的AW；RR `LockIn`也可能仍持该输入。
- 只mask VALID而不mask上游READY：A可能观察到一次握手、弹出请求，而mux未得到相应VALID，造成吞request。
- 在`mux_mst_req.aw_valid`、TC `complete_write`或physical AW后统一mask：旧AW已被锁/spill/Owner FIFO承诺；此处可能撤VALID或使内部握手与外部接收不一致。
- 仅阻止`w_fifo_push`、但保留AW输出：下游收到AW而没有对应W-owner行，路由/计数丢失。
- 在RR输出新push条件加budget门而不处理RR锁：A可能在`w_fifo_full`时先被RR锁定，还未push；关门后会一直挡住其grant，连V也无法重新被选中。

已核vendor `axi_mux:264–281`用`AxiVldRdy=1, LockIn=1`；`301–309`在首次内部AW可见push W row，早于下游握手；`335–347`另有AW spill。`rr_arb_tree:149–182`保持未服务req集合，`171–172`明确不允许撤未服务req。这个输入保持门同时覆盖这些阶段。

## 2. Snapshot：t0不是自动封口

若在t0仅采旧`awseq_q[A]`，会漏掉两种笔：t0同拍的新push，以及门后已held、以后才push的A AW。必须明确事件的前后状态。

推荐两阶段：

1. **closing**：关新AW准入，保留h_A。继续累计所有真实virtual `w_fifo_push`的A seq及有效end；将h_A待接收AW的copy end也列入未封口上界。已有shared AW/spill/TC照常前进。
2. **sealed**：确认h_A已清且没有新的A门后AW可能出现，在包含最后一次握手/push的更新状态中封存seq/end。若同一个always_ff读取旧h_A/旧计数，应在后续安全拍封口，或显式使用事件更新后的counter；不依赖非阻塞赋值偶然顺序。

封口之前的所有A晚push都是已可见义务，不应报成budget violation。封口之后再出现新A virtual push则应直接失败。physical AW在封口后仍可以继续first offer/accepted，它们是这些virtual行的后续推进，不是新destination admission。

最低承诺记录为：最终virtual A seq数量/最后copy end、尚未退的virtual/physical行进度、physical first-offer与accepted AW数量、最后gate-held状态及其stable header、当前A source child、A背景TL offer/slow。保留队列和真实流水状态，不能把最高end一个数作为完整quiet证书。

SameBank配置尤其要保持**flow身份**：physical mux端口号只代表bank，两个DMA同用bank0；top `493–499`从真实physical first-offer的`id[2]`另存flow顺序，再在physical WLAST弹出。不能把physical mux `w_fifo_data=0`误读成只有V。

## 3. Source：既有AW债务必须仍可取得payload

### 3.1 最小变更是保留原child服务规则

`compact_pretl_owner_fragment:87–123`只从两W序owner/seq都匹配且physical AW已接受的连续prepared prefix生成child；固定上限128拍=1KiB，进一步截到parent余部。AW准入封口之后，这个有限W集合不会再增长，继续原规则即可清退必要payload。

**当前child无需再新建目的AW。** 它首次offer时已经有足够prepared W capacity。实际需要新source的部分是较远已承诺A AW；可能跨当前child，也可能跨当前已捕获的raw parents。如果冻结全部新child或raw AR，既有A W head会缺payload而不退休。

如果另外加source permission，放在`241`锁存candidate为offered之前；保持`offered_q`产生的`134` ARVALID以及已busy RREADY不受budget关闭。不能对`m_ar_valid_o`或`m_r_ready_o`直接乘run。V的source服务不应被A budget门统一关闭。

### 3.2 冻结无关raw parent，允许必要父header越过最终end留存

对mode0当前专用raw接口，可在top `610–618`选择eligible owner时筛A：仅接收同copy、source parent起点小于最终debt_end_A的必要前缀。原2KiB header可能越过3KiB等最终W end；接收header不意味着必须物理读取其剩余部分，允许留存。不要用“parent末端必须≤end”阻断这个必要父请求。

返回ARREADY必须仍只给被实际选择且由wrapper接收的owner。禁止A时READY0，不能沿用未筛选的ready让DMA提前弹出。当前`pre_parent_valid=>pre_parent_ready`由相同active/credit条件保障，这个wrapper入口没有持续stall的shared held AR；关门时同拍真实已接受parent按实际事件计入。

mode0中每flow的`pa_q-done_q<ParentDepth`独立；A accepted raw parent可保留，不占V的父槽，且source AR已移出shared mux（top `619–622`）。这就是这里可以不强制A parent done的真实边界。若改到普通production shared AR mux，已有AR `LockIn`/spill可能保持A占用共享锁，则需清该首VALID或提供独立捕获；不能直接迁移此结论。SharedPoolMode非0的pool/header隔离也须重审，本稿不替它背书。

### 3.3 CONF1使必需source不被新AW背压卡住

已直接核生成寄存器`idma_reg64_frontend_reg_top:196,519`：CONF bit0是decouple；frontend `367`把它送到`opt.beo.decouple_rw`。当前CONF1对应decoupled模式。

`idma_legalizer:453–458`使R根据r_ready独立前进，w_ready被新AW门拉低不会直接禁掉R；backend `611–625`有独立AR fall-through，`837–854`有独立AW寄存；top `364/423`配置RAWCouplingAvail0。**所以本配置可以只关新AW、继续取得先前AW所需source。** CONF0的`463–464`要求r_ready和w_ready同时满足，不可无条件外推；若更换配置必须重新核真实已排队的R是否覆盖义务。

## 4. Prepare：只筛新candidate，不能伪造parent失效

建议新增独立的每flow/perparent **new-preparation eligibility**，只用在helper `144–148`的candidate选取条件。V保持原资格；A在关闭期间只允许parent起点位于所需source prefix、或被已offer继续动作明确需要的key。

不要通过把`parent_valid`、`reuse_enable`、`cache_valid`或全局`prepare_i`关掉来筛A：`172–175`会根据parent validity清现有cache/prepared；`101–110`和`123–127`分别控制cached PA eligibility/reuse，并且全局prepare影响当前父起点复用。这样会使已承诺A source退回TC，改变原本服务路径，甚至制造新的等待。

`prep_offer_q`、`slow_q`、正常response READY均按原逻辑继续。已有background A prep可以越过冻结瞬间完成，应该列为snapshot extra obligation；新无关candidate才禁止。当前`151–157`和`188–233`支持这个区分。

同bank foreground request的`bank=0`不能识别A。Foreground TL与A已经锁存的source child绑定，child offered/busy本身可覆盖其未完成寿命；背景prep可用已有`slow_prepare_q/slow_owner_q`归属。不要把formal slow请求的`slow_owner_q`对所有子片一概当作可靠flow标签：helper `120–122`只对parent起点设置source_first，非起点fallback未必捕获owner。若需要独立foreground统计，应另用真实child/request归属，不能凭bank或默认owner。

## 5. Quiet：足够检查与观测分层

建议报告两个分开的完成事件：

- **tail physical quiet**：A不会再次占目标shared R/W通路；原parent和私有header可以保留。
- **protocol fully drained**：另要求A pending B/response/tracker均退完。若这些tracker实会背压V，它们必须前移到第一个定义；若完全独立，则第二个是额外观测，不能要求原job DONE才给第一个事件。

对本次tail目标，至少同时检查：

1. AW gate已sealed，h_A=0；封口后无新A virtual push。
2. snapshot内全部A virtual AW已到virtual WLAST；全部A physical first-offer/accepted AW已到真实physical WLAST。不能只看当前队头或accepted ring空；physical首VALID未握手也要覆盖。
3. `offered_q/busy_q`若置位，其child owner均不是A；无Γ上A held R/路由。无A已首VALID的normal/direct source AR悬挂，不把TC header里A child当已释放。
4. A相关TC W FIFO/共享transport中的有效payload已经交付；两W序A债务空以后也要依据stage/counter守恒排除残留。当前输入mux默认SpillW0；不能假造那里W spill，但TC W FIFO真实存在。
5. 无A background `prep_offer_q`或`slow_prepare_q&&slow_q`；停止新无关A prep。背景response必须已被实际消费，不能仅观察TL正在idle；相关PTW若尚有transport token也不能略过。
6. copy有效payload守恒；剩余private payload只在已预约B内并不占共享通路。本配置child只服务已prepared W前缀，所有这些W完成后预期无超越debt_end的A读取，但应实际用R/W counts证明。

第1、2条可用单独A累计计数和seq upper bound检查，而不是要求全局两W队列空；V应继续服务。WLAST归属必须从正确真实flow序列取得。第3条必须同时含offered和busy，且保持完整AR属性；helper在途不能仅用全局busy位粗略归属。

`compact_pretl_owner_fragment:265–270`已经分别处理原parent done及child RLAST，因此A parent残留不是quiet失败。反例：A physical child已结束但snapshot较远AW仍在W序中，需要下一source child；busy0时仍不能发release。

## 6. 建议的本次验收断言和有限性边界

根实现后应一次核关键事件，覆盖下面临界条件；本席未运行这些检查：

- 关闭发生在门后A AW stalled、RR锁但尚未push、已push未接收、AW spill已收、同拍AW握手各阶段，先前VALID及全部字段保持。
- 被gate屏蔽的A AW/AR没有返回假READY；每个真正握手恰好计一次；virtual owner push没有重复或遗漏；sealed后A virtual push为0。
- A正常/direct child的首VALID不撤，RREADY不被预算阻断；source新增只在有限snapshot需求中，必要raw parent仍被接收。
- new prep eligibility不清旧cache、不中断已有prep offer/slow；V eligibility照常。
- 首次报告quiet时以上六项一起成立；随后保持budget关的一段V服务期间，A没有新R/W/TL注入及latent W head复活。恢复预算后原A wholecopy完整完成，证明没有丢parent余部/状态或吞request。

AW门关后最多仍放完门后此前可见的一笔，**snapshot大小不应笼统写成MaxWTrans8**：virtual W可以先到TC FIFO，physical债务与管线延迟另计。以真实sealed seq/end及stage inventory作准。Finite正常job只给N尺度上界；outstanding、同拍保留和prepare债务均须纳入实际额外work。

如果两W序兼容、必要raw source/prepare获服务、translation/DDR满足进度、且新A AW已封口，那么现有prepared-prefix child只会消耗有限snapshot，最终tail physical quiet可达。这个普通控制具有本次可测试的有限让路能力；它不推出连续4DMA、任意bank失配、fault/invalidation或硬实时周期界。原raw选择偏向V，背景prep亦受formal请求干扰；本次有限32KiB负载不能代替无穷victim注入下的公平证明。

因为同样的普通AW admission、held-preserving门、prepared-prefix供给及有限debt tracking已能表达全部动作，这一实现即使通过也只是强普通控制的验收，不将门、snapshot或继续allowlist改名立新。

## 7. 源码导航

- [top AW门/raw入口](/home/xvhaocheng/work/iommu-platform/out/prepared-source-bypass-20261008/multi_actual_dma_top.sv:610)：610–622；756–781输入mux；493–499 physical flow tracker；887–894实际双W观测。
- [source child与W-prefix](/home/xvhaocheng/work/iommu-platform/out/prepared-source-bypass-20261008/compact_pretl_owner_fragment.sv:87)：87–141；192–239入队/进展；241–270首offer与释放。
- [helper准备与cache](/home/xvhaocheng/work/iommu-platform/out/prepared-source-bypass-20261008/hit_bypass_parent_translation.sv:113)：113–158；170–233。
- [AXI mux AW锁与spill](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:264)：264–347；50–55默认spill配置。
- [RR未服务req保持](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/rr_arb_tree.sv:144)：144–182。
- [CONF decouple传递](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:367)；[legalizer独立R](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:453)；[独立AR寄存](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:611)。

