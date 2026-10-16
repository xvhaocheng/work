# 未来消耗信用：独立关键审核

Codex亲自推演并直接核本目录design、qwriter/source/top及必要的vendor mux、TC、banked路径；直接阅读下述两篇primary的指定段。未改RTL/executable/log/CSV，未构建或运行。root报告§62已有37场景、§63普通包络15PASS，本稿没有独立复核这些日志，也不把这些结果当作尚未实现的future-prefix规则验证。

本稿SV行号指本目录同名文件；原路径完整映射为：vendor mux/demux在`cva6/corev_apu/riscv-iommu/vendor/axi/src/{axi_mux,axi_demux_simple}.sv`，TC在`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`，bank wrapper在`tests/nonblock_completion_banked_ctl.sv`。以下引用是本次读取的实际源，不是worker摘要。

**结论：`end_new <= min_i(P_i+Q)`在明确的共同W顺序及持续进度前提下是成立的W请求预约界；P不是当前空闲payload容量。当前single-job、单child/B后再发的原型，可直接用已有input-mux `source_coverage/8`实现该P，普通有序AW预约使用相同字段、状态和比较器即可同效，因此拒绝将这条公式或“未来消费信用”单独列为创新贡献。两篇指定primary直接覆盖普通消耗信用、缓冲延迟与内部流抢占，但没有证明它们逐字提出此AXI前缀公式。** 同成本否定依靠下面具体构造，而非声称任何算法都可被普通实现抄写。

## 1. 最小精确定义：信用必须在哪个边界兑现

计量为8B full beat，所有游标都是单个2KiB job内的绝对beat端点0..256，不是地址、cycle或physical B数。

- `w_i`：DMA i实际raw W握手、已从private FIFO移走的连续前缀。qwriter `:29–30` 为parent相对job base+offset+当前got，`:95–99`仅真实W握手增长got；offset在child B后累进（`:100–106`）。W进入TC就释放了DMA payload，**无需等DDR写或B完成**。
- `r_i`：实际logical R握手交给DMA的连续前缀。source `:317–322`按logical handshake增长；shared skid全部成员返回后才接下一physical R。空skid时共同流的r_i相同；暂态可相差一拍，不能每拍假定相同。
- `n`：一个尚未成为本边界W头的新child，immutable范围`[s_n,e_n)`，长度<=Q，逐flow按job offset连续。原型qwriter `:83–94`在创建时注册child/firstVALID；既有offered/WVALID均不可撤回（`:65–70`）。同flow仅一live child，必须真child B后才可发下一child。
- `≺`：**释放private payload那个边界的、已不可撤销的共同W顺序**。这里就是input `i_dma_iommu_tr_mux.gen_mux.w_fifo_push`记录的顺序；它早于或等于external virtual AW握手，不能只按AWREADY统计。
- `P_i(n)`：已消费的w_i，加上在这个共同顺序中确实位于n前、按flow i连续无hole的承诺W前缀端点。它的定义要求：n成为本边界W头前，flow i实际w_i一定已至少到P_i(n)。准备成功的WRITE recipe、原parent长度、另一路AR、任意本地VALID或“预计下次选择”本身不是这个事件。

界须对所有会阻塞同一shared R的成员i成立，包括n自身。Q是W最大child长度及可停车的受限容量，不是额外零成本共享FIFO。当前top `:1041–1042`明确DmaBufferDepth>WriteQuantum；root运行配置是B=Q+1。证明只需实际private容量至少Q，并保留实际DMA ready和skid容量检查；原型预留一拍的实现选择不能被当成免费buffer。

## 2. 受限证明与它不保证的事

对共同W顺序作归纳。设H是当前最早尚未在raw边界完整消费的child。对H的每个P_i，所有用于该P的较早W此前已消费，故现在`w_i>=P_i(H)`。于是其准入界给出：

`e_H <= min_i(P_i(H)+Q) <= min_i(w_i(now)+Q)`。

如果共享源在空skid状态被成员k的Q拍库存挡住，`r_k-w_k>=Q`，则`e_H<=w_k+Q<=r_k`。共同r相等，H全部payload已到它的DMA或已交入TC；H继续消耗不需要当前被挡的下一R。公平的normal downstream服务可移走W头、兑付旧预约，让实际w增长。若阻塞在skid的某成员DMA ready，容量至少Q且真正已满的成员r_k同样足以覆盖e_H；其它成员已收同一前缀，至多已收一拍更多。已有logical skid不可改变成员或丢弃数据。

这只排除“private R库存满→最老raw W仍缺下一R→库存不能释放”的持续环。还必须满足：

1. 已承诺W的实际权限和正常目的数据路径能最终完成；source后续必要WRITE成功前沿不可低于所需拍。当前helper `:103–106`用真实WRITE `.w`或bypass、正常成功和匹配seq作为cache_valid；prepared不是成功、READ `.r`不是WRITE证书。
2. R没有在库存未满时被其它永久门挡住。当前source `:119`成功cert前沿仍硬守；`:128–132`在write_progress或phase3中去掉额外logical-LAST/frontier AW coverage门，允许有授权payload私有停车。保留旧额外门会破坏证明。
3. 已offer physical AR/R、logical R和AW/W均稳定，任意选择不能撤firstVALID；正常memory/TL/B最终服务。该界不提供cycle上界，不保证动态fault/invalidation、永久budget-off、所有成员同时暂停或持续新job到达下的公平完成。
4. raw W顺序与下游完整AW/W路由兼容，不能用这个局部定理忽略另一层TC/physical W-order环。当前same-bank/static normal路径满足下文的受限结构检查；不提升为生产任意bank总定理。

**最小模型反例（界成立但旧R门仍死锁）**：Q2、两个原logical读各2拍；V W `[0,2)`在头、A第一页W `[0,1)`在后，初始两准入都满足P=0的界。R0已给两DMA，V头缺R1；A有R0但不能越过V，A第一页B不回、AW2覆盖不建立。若logical LAST要求A的AW2 coverage，则R1被挡，永久成环。该2拍模型不是本RTL支持的job长度；真实原型的同构末尾已由root §59 run13/F255观察：V `[254,256)`、A `[254,255)`、rV=rA255，均权限cert256，却因AW coverage255而停。本稿没有运行它，原日志保持。这个真实反例否定“单靠前缀界保证整个平台进展”的无条件说法，不否定加入上述R门前提的定理。

## 3. 实际可用的归属位置，不混用TC COMPLETE/physical AW

实际input mux `multi_actual_dma_top.sv:866–890` 未覆盖vendor SpillAw默认1/SpillW默认0。vendor `axi_mux.sv:264–280`有锁定AW仲裁；`:301–310`在首次选择且W FIFO有空间时立即push owner，AW未ready也进入lock；`:329–332`记录owner，`:335–346`另有AW spill。因此正确记录事件是内部push；等外部AWREADY才记会遗漏已经锁定的W义务，两个raw端各自AWVALID则还未给出共同顺序。

同vendor `:353–365`只从owner FIFO头送W，在真实WLAST握手时pop。故成为本边界W头前，所有较早owner token的raw W都已消费。这个事件链直接证明P兑现，不依赖DDR队首或B时间。

top `:1001–1006`已经把这个push及`id[2]`真实DMA owner/地址/len送source；source `:201–203`由header计算job-relative byte end并更新`coverage_o[owner]`。本scope每flow prefix连续，且新child尚未创建时，所有已global push的其它flow token必早于它；已pop的token前缀则已由w兑现。所以直接取：

`P_i = max(w_i, source_coverage_i/8)`。

单job正常无hole时coverage本来>=w，max仅用于把定义写完整。使用前一拍coverage自然保守；不能把同拍两个尚未仲裁的local child互相算成“较早”。本地qwriter offered/proposed_end不是普适global order证书；当前两flow/单child/等B结构可能额外限定其可达顺序，但实现不必为省一个已存在的反馈字段赌这个推论。

source_coverage只记录最近端点、没有epoch，也不会在job_done清零。当前每reset单job因此可用；多job必须在可靠epoch切换重置/区分且保留旧held义务，不能取跨jobmax。per-flow offset与parent地址递增及source/write键不能有hole。归属DID与bank不同：两DMA raw AxID0、input mux ID0/4，owner取bit2；samebank时bank取bit3，不能把common bank0误当DMA V。

后层核查限定：top `:1115`/system `:1343` samebank用BankIdBit3，两flow进同一个TC。TC `rv_iommu_trans_ctl_wrap.sv:121–139` AW FIFO保序，只译head；`:693–702`接译址结果，`:721–725`在**内部**axi_comp ready才pop/IDLE。`:553–568`内部normal/error demux有AW/W spill，因此COMPLETE pop可能先于external physical AW accept。tests `nonblock_completion_banked_ctl.sv:432–444`原HeaderPark0路径不改AW，`:592–622`output mux Spill0也不能消除TC内部spill。正常同bank FIFO+spill保序，WRITE prepared只加速head授权，不改变这个W顺序。top `:517–523`physical owner ring是后层观察，不能自动当free credit或用B推断private consumption。

## 4. 最小普通同字段实现与成本

普通端到端credit继续只按实际payload消费更新：actual slot由真实容量B扣已入FIFO但未消费库存，未交入的skid/reservation另行计费；选择Q的保守额度可写`max(0,Q-(r_i-w_i))`，不能将负值当unsigned credit。这里保留当前DMA FIFO ready及reserved skid路径，不另改R注入门。AW header是未来工作请求，不是已经入FIFO的数据。普通有序预约器另把input global AW队列里已commit的旧请求当作**按序兑付的预定消费**；新请求只有在其未覆盖tail相对这些旧prefix不超过Q时才入队。固定per-job连续offset使一般队列累加退化为上节已有coverage两个游标。这直接产生同一包络，未增加权限来源、payload或预取能力。

具体接现有接口：在top计算`budget_end=min_i(P_i+Q)`，把`child_quantum[f]`裁至`max(0,budget_end-start_f)`，再与自身rawparent剩余/Q/current real WRITE-valid取min；qwriter `:83–89`只在非零quantum和hold允许时注册新child。已有held child绕不过这一门、也不能被后来预算撤回。全size Q时它等价`start_new<=min_i(P_i)`；native短尾可比简单old-offset先后规则更早承诺，必须比较真实新Wchild数/B压力，不能把两种规则无条件称同轨迹。

所需字段：已有2×64bit source_coverage（有效端点各9bit）、2×9bit真实w、已有jobdst/parentoffset/count/key/valid以及Q。仅新增组合min/add/sub及反馈连接；没有新的必需功能寄存器。若另存9bit P游标/owner queue，普通和候选都同计，并须解释为何不复用已有coverage。own bound仍必要：当前一live child使P_self=w_self=start_new，实际child_len<=Q已满足。所有P<=256；`P+Q`范围可到512，**先用至少10bit或64bit计算再clamp到256**，不能9bit相加让256+256变0。新end/count用9bit，AxLEN另存count−1为8bit，保持256拍合法。

反向错误用法：把成功recipe_end256或完整parent_len当P会许可没有较早global W的flow领先；那不是预约兑付证书。把P>w当当前真实free-slot信用直接注入超过private容量也没有上述定理支撑。prepared key可以提前授权，不可以提前产生真实消耗。上述普通构造允许同预取、同q/FIFO、同global順序信息及相同RTL资源预算，因而未来prefix准入本身不存在已证明的独有能力；root正在把它当强普通实现，这是合适的贡献边界。

## 5. 指定primary的直接核验与不足

- [CTC: An End-To-End Flow Control Protocol for Multi-Core Systems-on-Chip](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf)，§3（PDF页4–5）、§4（页5–7）：端到端credit以输入队列空间为证，并在核心实际消费后返还；CTC先连接、以K成组给credit、限制并存请求并为请求队列计容量，信用耗尽时结束当前网络packet以释放路径，而connection保持到整message完。它覆盖普通消耗信用/消息依赖解环/缓存与控制开销，**未把未来未消耗的AXI W承诺直接算现存槽位**。本文不声称其已逐字提出本P公式。
- [Physically-Aware Preemptive Virtual Channels for Deadlock-Free AXI Networks-on-Chip](https://arxiv.org/html/2607.01430v1)，III-3/4：CreditBased在真实下游释放后返credit，注册延迟可能要求额外FIFO；Preemptive切换内部逻辑流的物理link所有权，未发送flit留源端。该内部多流接口不授权撤回已经held的外部AXI VALID/RDATA或重排既定W owner序。没有把其one-cycle VC切换当本平台held AR/R释放方案。

两primary没有给当前nested-IOMMU/static recipe+共享immutable READ的完整定理；上文proof是本席受限推演，不是从论文直接复制的实验或生产保证。因为同字段普通预约器确实达到同一可达准入集合、实际FIFO注入信用又已是经典机制，本轮应停止把此P包络独立命名。若未来贡献是在线权限未成功时仍能安全释放真实held通路、或在同state/payload下提高可证明服务边界，须另外给机制/反例/强对照；当前没有这样的已验证增量。

交付边界：源码审定input firstVALID归属、受限prefix归纳和普通同成本构造；未验证future-prefix RTL、synthesis、board/MIG、动态上下文或持续arrival。root后来15PASS是actual-w包络，不能预先归为本future-prefix结果。
