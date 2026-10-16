# 完整ADMIT/WAIT因果链及被撤回的边

## 1. 多端点定义

`AWd[j,k]`：DMA master源侧AWVALID&&READY，private AW fall-through输出，可先被原4DMA mux的AW spill接住。

`AWi[j,k]`：IOMMU input demux前global AWVALID&&READY，header从upstream spill出来；不是AWd。

`AWt[j,k]`：TC full普通翻译结果与target AW spill输入接受；由它加原bank W credit，不等于DDR接受。

`AWo[j,k]`：output bank mux该AW首次VALID/w_fifo_push，预留W顺序；另外记录输出真实AW接受。

`WR[j,k]`：raw W在input demux对应burst的真实WLAST；`WO[j,k]`为输出W真实WLAST；`BR[j,k]`为原backend收到且接受对应B并消费last metadata。`Cj`为该job实际正常退休，不能用WR/WO代替。

## 2. 第二AR正确的生成关系

HardwareLegalizer=1，RAWCouplingAvail=0；AW private fall_through depth1，r/w metadata仍原NumAxInFlight8。在accepted decouple_rw=0/non-deburst4KiB时，第二对请求只有r_ready与w_ready同时满足才推进。AW0占private槽且DMA源侧不ready时w_ready=0。因此：

`emit(AR1) >= AWd0`（必要边；同时还受原AR/data/meta/last-flag就绪约束）。

若A AW0在input demux因B raw W open阻挡，则`AWi0 >= WR_B_last`；但原upstream AW spill允许`AWd0 < WR_B_last <= AWi0`。所以**不能传递出emit(AR1)>=WR_B_last**。private单槽上限与下游raw gate之间必须包含共享两槽spill。decouple_rw=1时也没有w_ready造成的该成对生成边。

具体原文：backend:472–473/811–858；legalizer:452–465；frontend:366–373；upstream axi_mux:47–54/259–271；spill_register/flushable两寄存器；FullBuffer:16–18未核decouple/deburst。历史原稿归档，修正不是改原RTL或配置。

## 3. 每策略s必须走完的链

| 阶段 | ADMIT | WAIT同一个P |
|---|---|---|
| 新下游承诺 | L先首次VALID/握手；原held优先 | L不首次VALID，直到同P shared AR接受；立即可重评，不能等整walk |
| 当前与后续PTE | L与原共享PTE/R/W服务竞争 | 当前P先进入，之后L仍会竞争后续PTE；next PTE只有真实返回非leaf后产生 |
| A read translation | 完整walk/权限/slow response/TC COMPLETE进入physical AR spill | 同样全walk；history只预测，不生成未来PTE |
| B数据与raw W owner | 实际R→byte buffer→source W route→raw input WL LAST | 随L实际数据与W反压改变；延后可被已存数据遮住，也可能传给WR |
| A AW native准备 | AWd可早；AWi要等原raw owner；TC只在IDLE选择本bank可见AW | 各端点分别预测，不给共用H；同bank N不能穿过自己的active read P |
| A剩余AR生成/服务 | actual job选项＋private/global spill状态；accepted head或unemitted意图区分 | 不从P提前推整份R都ready；也不人工添加AWi前不得生成AR1的限制 |
| 两份W streaming | 每beat要真实数据、原W owner/credit、各target/output流水与共享服务 | 不强制R-last后才开始W；raw WL LAST不是WO/BR |
| B与任务退休 | 每job各B/last metadata/真实RESP/fault、后继供给与最终quiet | 不设跨job同z，不先验CA=max，不因B先done就free STOP |

对A的每burst k，source只给`T_AWselect[k,s] >= max(T_visibleAWfifo[k,s], T_TC_IDLE_A[s])`。当前own read P未结束时TC不是IDLE；P释放旧AR packet到target spill后TC可先前进，但内存R尚可未完成。AWi早/晚直接影响AW FIFO visible，仍需真实事件/历史。

设数据真正对原W可用为D[j,k,s]、native许可为H[j,k,s]。完整逐拍关系仍是原顺序下W服务函数对`max(D,H,此前已承诺W进展)`的作用；B用独立终结函数和原AW/WO/lastmetadata。两函数都随策略共享服务变化，source没有给出免费固定延迟。实际所有R/W/B/errors/quiet核对为空，没有运行新模型。

## 4. 两个有界手推筛选，均不构成完整收益正例

**AW门槛遮蔽。** 对第一个A write选择，若两策略都在AW FIFO可见以前结束当前read/TC占用，则此处max由AW FIFO就绪决定。只提前P不会提前这个端点。若进一步真实R/W/B共享服务、缓存资格和原后继均满足单调/相同的声明条件，推进WR/原AW链的owner规则即可覆盖这个局部作用；未据此作全任务/全候选覆盖。

**后续PTE准备重叠。** 只在声明的ordered-R服务、a>g、相同prefix和paid端点记录条件中，L在P接受后按原承诺先于后续PTE进入服务，选择/dispatch已计入声明端点；L服务a、当前P准备g/服务b，当前P正常非leaf后下个PTE的准备k/服务b1，余下完整walk/slow/TC尾r保持同服务条件：

`p_ADMIT = a+b+k+b1+r`；

`p_WAIT = g+b+max(a,k)+b1+r`；

`p_ADMIT-p_WAIT=min(a,k)-g`。

WAIT仍在当前P AR接受时释放L；不是等P返回才放行。k包含真实返回/处理/下一请求准备的声明端点，r含全部剩余walk，不能拿固定50/52代替DDR deadline；未来nonleaf是条件、不是当前免费输入。此式为手推筛选，未执行任何server模型。

若a>g、k>g且frozen dualGap的同端点eta>=k有效，它也WAIT。这个具体动作匹配说明“遮住next PTE准备”单独不是对该普通规则的新选择差异。它不证明两job完成改善，因为AW/R/W/B/原后继尚未闭合；也不证明dualGap统一覆盖完整家族。故不将此手推包装成独有正例，不生成四条件。

## 5. 最终来源裁决

实际服务/historical class尚未给定，不能从source相对次序推CA/CB/max/sum数值。未取得固定原供给的前驱、有效完整history、实际所有义务和选择费用，因此此轮走“准确缺项”结束，不接受source-bound full-RWB positive或创新/净收益；future-AW机制仍open，旧已关闭分支保持。
