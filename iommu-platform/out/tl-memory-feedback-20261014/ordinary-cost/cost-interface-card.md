# 新增反馈选择的成本接口卡（第一阶段，未综合/运行）

本卡借用前轮37个成本分类，**不继承R2已关闭的prepare架构、1329bit、8个rawAW扩展或其head指针**，也不重跑旧calculator。当前base为未改原生两bank TC+一个PTW+IoTLB16和既有queues/credits。公共资源对所有规则保留同一实际对象；只有本轮真正新增且依然需要的状态、端口和逻辑计delta。物理面积、Fmax、energy、CPU/应用均null。

证据入口：`out/r2-ownership-cost-20261013/root/full-cost-comparison.json:124-660`的37分类；`:264-266`请求118/响应185及fault/MRIF字段；`:455-471`原生两bank TC请求/响应合计606bit、FSM合计6bit；`:560-570`两输入stable RR与mux账；`:112-121`公共未知项只支付一次。本文只用这些已验分类/接口，不将其旧扩展整数当本候选总成本。

## 完整公共字段与寿命

- 原生bank请求直接继承`trans_req_data_t`完整118bit，bank合流仅写`.bank`，其余iova/did/pid_valid/pid/ttype/priv/debug等必须逐位相同（`rv_iommu_banked_ctl.sv:366-377`）。新评分侧只读所需字段，不另复制完整请求。若实现跨周期评分且源头不能继续持有，才另付完整118bit shadow+valid；不能免费从已释放header重建属性。
- 响应完整185bit按原`.bank`返回（同文件`:395-408`）；fault/MRIF/PA/权限及normal response所有权、ready/backpressure继承。不新增成功结果槽、响应port或B reorder队列。
- `rv_iommu_trans_ctl_wrap.sv:625-669`已经为每bank选定一种R/W头并保存request。候选不能从同bank RAW FIFO再次挑不同事务，不改AW优先/Wcounter和read admission。请求ALREADY VALID时评分结果不能覆盖此选择。
- AR/AW完整地址、LEN/SIZE/BURST/ID/USER等由原packet owner保留。评分需要的`owned remaining bytes/beats`可由存活header和已验native计数导出；header迁移/回收后仍要继续追踪的剩余量需付新counter。只重用真实仍有消费者的字段，不能假设raw IOVA被覆盖后还能辨认任意依赖。
- 真实R-LAST、physical/error W-LAST、cutB capture、sourceB handshake分别有不同退休边界。W进入FIFO或upstream WL AST不能等同physical WRITE/B退休。原生input owner、output owner和same-ID target各自保留；一个bank标签不是完整ownership图。
- context/static map冻结且运行到policy_quiet才清配置/统计。若支持reset/cancel/remap/epoch变更，需新增invalidate/generation和迟到响应防护；第一阶段不宣称已处理。原native fault路径不增加free sink容量。

## 新观察、头元数据、历史和预测状态

下面T为时间统计位宽，N为允许记录的已完成PTE count位宽，Q为最大已拥有工作counter位宽，M为必须明确追踪的native obligations数。必须根据root冻结的来源参数取值，不能先猜16bit/32bit便宣称完整成本。所有加法比较须饱和或带overflow位；溢出使该预测无效而走held/fair/RR，不允许wraparound成为更年轻/更短。

| 项目 | 来源/寿命/完整成本 |
|---|---|
| 两head age | 原生head capture事件起计，至该head实际accept；若没有可直接重用的native时间戳，新增`2*T`bit+2个valid/overflow指示，或一套公共timer`T`+两timestamp`2*T`（择一真实方案，勿重复计）。比较器及同时arrival/RR tie逻辑另付。 |
| 当前owned R/W/B工作 | 只读native尚存header/owner/credit；聚合counter最少每bank每使用的R/W/B类Q bit+更新/饱和。若需要每transaction依赖身份而native不保留可读映射，付`M*(Q+owner/reference/type/phase widths)`，读/写及匹配端口也付；不是新payload/队列额度。聚合量不能冒充精确per-task completion。 |
| 已完成PTE数 | 单active slow-walk使用native active-bank owner计数`N`；完成时存入bank/context/ttype class历史，每class `N+valid`。fast-hit accept不能reset当前slow walk计数；DDT/PDT请求不伪计为PTW-PTE。 |
| TL/PTE service history | 每class完成TL service时间`T+valid/overflow`、PTE response→next accept gap`T+valid/overflow`及起止timer/capture逻辑。未来walk长度未知，仅把已完成count/service作估计。DID/PSCID/ttype若非固定class需pay key/tag/matching，否则只能冻结静态class。 |
| R/W service history | O-turn四conditional样本`4*(T+1valid+1overflow)`、上一已完成class1bit+valid、transaction接受时间及beat归一化/退休归属逻辑；tau_RW/tau_WR差分+clamp+min和gap比较。已有公共tracked时间可共享，不能每rule重复计又不能省掉必要事件端口。sample含队列/反压，是估计而非直接隐藏MIG direction。 |
| Candidate两轨迹predictor | 用完全相同snapshot分别算A→B/B→A。若逐obligation模拟，要支付两份虚拟remaining/phase/release状态及update/control；不得把这些虚拟数据存到真实payload queue而变相增加可用容量。若用闭式公式，则付每个真实算术、service/dependency递推和mux。至少两个max累加`2*T`与两个sum累加`2*(T+ceil(log2(max(1,M))))`，加两个顺序的控制/地址索引；最终按max/sum/age/RR比较。全completion向量若实际存储为`2*M*T`另付，可合法stream-reduce省去完整存储。尚未冻结实现，故没有伪造精确bit总数。 |
| VALID hold | 既有2input stable RR的grant/lock/mux可复用，native请求源继续保持118bit。若替换arbiter，原firstVALID lock义务必须继承，不能删除后称节省；额外pipeline snapshot/decision寄存器按真实实现计。locked header不随history变化。 |
| Fairness | 非RR策略通常新增forced-bank1+valid1=2bit；若已有锁/RR编码能合法表达且事件证明完整，可回收该重复项。计actualaccept而不是未握手的VALID；native RR自身无需强加重复2bit。连续等待head消失/替换须来源事件判别。 |
| Selection delay | `L_select`从共同legal-ready到首次merged offer实际经过cycles。native RR额外delay可为0，candidate两轨迹不能假定0。组合大算术影响critical path/Fmax；流水化增加寄存器/起动delay，期间其他合法native events如何纳入snapshot须冻结。不能为对齐而免费延迟普通。 |
| History/warm成本 | 获得四conditional samples、PTE gap和每class TL history所需事务是真实时间/byte/PTE成本，不能离线oracle免费注入。primary共同ready的测量若已有历史，要清楚报告其生成工作和同policy共享条件；hist缺失按合同RR。 |

O-short、O-debt、O-owner可删未用统计；O-turn需要其统计但无两份完整rollout。若为了同信息共享完整history，须分别给“共同paid观察+各rule选择逻辑”与“每rule可实现最小完整成本”，不能只比较metadata FF而漏logic/ports/delay。

## 当前真实观察接口的缺项和符号边界

现有NormalTL两个bank lifecycle tag只说明req/rsp归哪一bank，**不覆盖所有R/B义务，更不覆盖两个应用任务的最终完成**。NormalTL request的118bit不是原AXI AR/AW header，不能仅从它恢复LEN/SIZE/fullID/原始事务顺序；bank返回185bit也没有替代真实R-LAST、W-LAST和sourceB handshake。bank_read_admit中的input W owner不替代output W owner、AXI same-ID targetbinding或全部R debt。

当前源码还没有供新policy直接使用的统一、原子`owned obligation snapshot + per-head dependency + completed service-history`接口。具体缺项至少包括：存活header的剩余work可读视图/迁移后counter；原始fullID到真实R/B退休的观察关联；slow PTW active-owner到PTE accept/response事件（须与并发fast hit分开）；同沿多R/W/B完成的统计更新仲裁；history class/context键与有效性；过去事务接受时间。已有diagnostic/host trace即使记录某字段，也不等同有零成本可供在线policy读取的wire。跨clock事件若有异步路径，还缺CDC和snapshot一致性证明。

可机械审查的成本结构如下，宽度/事件域冻结前所有精确总数保持null：

`S_candidate = S_native + S_obs + S_hist + S_predict + S_select + S_fair`。

`S_native`为完整未改原生容量/packet/owner/credit图；`S_obs`至少包含没有真实原生可读替代的remaining counters、接受timestamp和transaction association。若M个原生义务的时间/剩余/归属字段均须新增，一种直接完整编码是`M*(T+Q+I_ref+P_owner+1valid)`，再加必要overflow/phase；其中M不得扩大native accepted envelope。可复用的存活字段逐项扣除并给具体原所有者/寿命证据，不能一次把整个项记0。

设K个冻结bank/context/ttype slow-history class，直接编码的PTE计数/历史为`(N+1overflow)+K*(N+1valid+1overflow)`；TL服务hist为`K*(T+1valid+1overflow)`；一个shared PTE-gap记录为`T+1valid+1overflow`，计时起点/accepttimestamp另在`S_obs`列。O-turn四conditional样本为`4*(T+1valid+1overflow)+1lastclass+1lastclassvalid`。如果相同时间寄存器被同一生命周期合法共享，只计一次；如果TL与memory域时间不可共用，就保留两套。

新增age的一种完整表达为`2*(T+1valid+1overflow)`，counter自增和比较另计logic；公平reservation通常2bit。native RR可不需要这两项，不能向RR硬加unused状态。candidate predictor至少记录/组合输出`2*T` max和`2*(T+ceil(log2(max(1,M))))` sum；scratch递推、虚拟状态、snapshot/decision register和各自overflow还未冻结，不能把这一下界误称完整新增state。O-turn不含这些两轨迹scratch/max/sum，仅付其真实statistics、差分/clamp/min、turn-gap比较、short-TL tie和共同持锁/公平。

逻辑/资源向量另为`(readports,updateports,comparators,adders,multipliers/dividers,muxes,fanout,scan_bound,L_select)`，不能由statebits代替。其中O-release-density至少有两个cross-product；O-turn perbeat归一化若非2的幂需整数division或付逐sample算时；candidate模型递推涉及的算术逐项列。history样本生成work/时间与真实CPU/ABI项另记。尚无参数化实现，所以这一接口卡闭合的是**所有新增费用应出现的边界**，不是数字面积/FF总和验收。

## 37分类逐项新增/继承接口

| # / 字段 | 本轮公共继承与新增delta |
|---|---|
| 1 dma_data_payload_bytes | 原DMA深度/字节不变；新增0，不借payload空间存虚拟预测状态而不付机会成本。 |
| 2 common_tc_w_data_payload_bytes | 原W FIFO/spill实际容量不变；新增0。 |
| 3 common_tc_w_packet_fields_bits | STRB/LAST/USER、valid和FIFO控制全继承；新增0。 |
| 4 additional_return_data_payload_bytes | native R spill/原R策略保留；新增payload0。 |
| 5 return_packet_nondata_bits | full ID/RESP/LAST/USER/valid保留；不以省payload忽略packet字段。 |
| 6 raw_ar_header_slots | 原rawAR FIFO/spill和完整header继承；不加一个便于选择的shadow槽。 |
| 7 prepared_ar_header_slots | 原physicalAR/target spill继承；本轮无新prepared AR槽。 |
| 8 aw_raw_prepared_slots | 原AW FIFO/Spill/target实值继承；R2新增结构不搬入；本轮新增0。 |
| 9 header_field_widths_bits | 请求118/响应185以及真实AXI字段逐位继承；若实际新增shadow按完整宽度付。 |
| 10 job_epoch_segment_metadata | 不假设已有任务/consumer ACK metadata；current-owned目标若需跨事务job归属必须显式新增、不能用scene name代替。冻结context无新增epoch；放宽另计。 |
| 11 source_id_metadata | 原immutable fullID/bank路由继承；观察所有权的额外tap/tag/mapping读口计费。 |
| 12 sequence_state_bits | 原sequence/RR不变；新增head-age/time/reference或虚拟预测sequence计费。 |
| 13 valid_phase_state_bits | 原各packet/owner phase继承；新增histvalid/overflow/snapshot/decisionvalid逐项计。 |
| 14 fifo_pointer_occupancy_bits | 原FIFOpointer/count继承；不增加queue；新预测scratch控制计费且不算nativecapacity。 |
| 15 aw_w_order_state_bits | 原input/output owner、W allowance、AW firstoffer locks全继承。新fairreservation不是Wowner替代。 |
| 16 ar_armed_held_state_bits | 已offered AR锁与header继续持有；TL排序不撤physical AR VALID。 |
| 17 aw_armed_held_state_bits | AW首次offer/trueendpoint accept继承；内部spill接收不冒充外部AW握手。 |
| 18 r_receive_replay_state_bits | 原R接收/held/策略不变；本轮无replay；serviceobserver只读并付统计。 |
| 19 b_ownership_order_state_bits | 原same-ID target/B spill/upstream credit继承；观察B debt不能简化成W-last退休。 |
| 20 error_cancel_retirement_state | 原normalfault/error sink保留；预测fault/timeout标记与hist无效化计费。通用cancel/remap不在本范围。 |
| 21 tl_owner_generation_state | native每bank TC owner、activewalk bank、slow/fast ownership继承；新观察必须分慢walk与并发hit。无额外walk/result-owner。 |
| 22 fast_classification_state | 同native hit-bypass/scan/diagnostic配置；不免费并行peek两个IoTLB结果；额外head type/historyclass逻辑付。 |
| 23 capture_ports_per_cycle | native actual accept/response接口不增；新增event tap和统计同沿写冲突/优先级付，不能丢service sample还宣称完整。 |
| 24 dispatch_ports_per_cycle | 1 NormalTL、1 PTW、原memory dispatch照旧；predictor算两顺序不产生2个真实许可。 |
| 25 metadata_read_write_ports | 两head可读字段及当前obligations/hist readports、各completion更新ports新增按实现付；若单口扫M项便付时间延迟。 |
| 26 payload_read_write_ports | 原数据R/Wports不变；新预测不能读取payload内容或偷偷拿port做预取。 |
| 27 matching_comparators | 新history class match、owned head关联、age、debt/ratio、turn/gap、max/sum/completion比较器与位宽计费；不只列FF。 |
| 28 selection_arbiter_and_mux | 继承118bit稳定mux/185bit回应bank fanout；新增scoremux、fairpriority、predicted order selector与adder/divider/crossmult计费。 |
| 29 scan_bound | 两真实合法heads上限2；若观察/预测遍历M个owed义务就明示M及cycles，不称常数2便忽略内部扫描。 |
| 30 fanout_and_update_events | accept/PTE/R/W/B事件额外统计fanout、多个hist更新与snapshot一致性付。存在跨clock观察需CDC接口/latency另付；本卡不虚称已free。 |
| 31 software_cpu_application_cost | 若由软件维护/注入history需真实CPU与同步/ABI成本；当前无实施、null，不当免费oracle。 |
| 32 lut | null，尚无综合；描述算术不等于LUT测量。 |
| 33 ff | 只有参数化新增状态公式，最终总FF仍null；未冻结Q/N/T/M和clock接口。 |
| 34 bram | null，不宣称历史array或predictor为零BRAM。 |
| 35 area | null，逻辑字段差不等于真实面积差。 |
| 36 timing_fmax | null，必须计L_select和稳定VALID路径；尚无时序/板验证。 |
| 37 energy | null，不据service次数或statebits推断energy收益。 |

## 本席接受/否决边界

如果root冻结的真实双头/共有服务时间线使candidate选择与所有实际冻结普通规则不同，且差别来自合法可观察历史对PTE+已拥有payload的联合反馈，才有进入至多四新条件的理由。预测质量和全成本仍须小实验验证。

如果真实source配置只能实现固定独立两stage，则两任务顺序选择被O-two-stage精确覆盖；如果仅是本手算turn pair，则O-turn无需两份完整rollout就能完整覆盖该pair，实际最小实现成本仍需冻结后比较；均不把对应排序能力列为独立贡献。两个结论都不等同于发表前作已覆盖所有kernel，也不扩大关闭范围。

目前未证明原生positive/neighbor witness，未获得冻结实现接口，**无microbenchmark、模型或生产实现授权执行**。下一步仅接收root source-bound snapshot/服务合同，按本baseline定义给具体等价/差异；不复跑R2整图或旧validator。
