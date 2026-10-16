import json, hashlib, datetime
from pathlib import Path
W=Path('/home/xvhaocheng/work'); P=W/'iommu-platform'; O=P/'out/pte-prepare-admission-20261014'; R=O/'root'
S=P/'out/composition-isolated-20261007/fpga-src'; F=S/'cva6/corev_apu/fpga'; I=S/'cva6/corev_apu/riscv-iommu/rtl'; C=S/'cva6/vendor/pulp-platform/common_cells/src'; A=S/'cva6/vendor/pulp-platform/axi/src'; D=S/'cva6/corev_apu/iDMA/src'
entries=[
(F/'src/ariane_peripherals_xilinx.sv',[(1263,1286),(1350,1389)],'active buffer capacities, accepted DS path and physical completion connections'),
(F/'src/ariane_xilinx.sv',[(158,172),(291,307),(845,853),(927,943)],'bound profile50/50, AXI64/local4/full6, crossbar, atomics and downstream CDC boundary'),
(I/'ext_interfaces/rv_iommu_ds_if.sv',[(75,109)],'PTW input handshake into non-bypass spill; not delay or DDR acceptance'),
(I/'translation_logic/rv_iommu_ptw.sv',[(262,279),(384,400),(507,513)],'PTE localID0 one beat; only accepted P enters response state; next P requires consumption'),
(C/'stream_delay.sv',[(26,136)],'Idle/Valid/Ready, direct payload and counter timing'),
(C/'counter.sv',[(24,40)],'fixed decrement wrapper'),
(C/'delta_counter.sv',[(49,74)],'load priority and wrapping decrement'),
(C/'spill_register_flushable.sv',[(69,99)],'two existing entries own payload while delay READY low'),
(A/'axi_delayer.sv',[(58,72),(106,120)],'input AR and output R delay distinction'),
(I/'transaction_controller/rv_iommu_banked_ctl.sv',[(236,281),(549,586)],'read admission, input W owner and true R/W/B events'),
(I/'transaction_controller/rv_iommu_trans_ctl_wrap.sv',[(625,742)],'native selected head, AW-before-AR and full-buffer admission'),
(D/'frontends/register_64bit/idma_reg64_frontend.sv',[(142,160),(470,571)],'sole job COUNT/WAIT and last-B lifecycle'),
(D/'idma_backend.sv',[(518,533)],'DEFAULT last-B task retirement, not R-last'),
(D/'idma_axi_transport_layer.sv',[(445,455)],'actual B response and streaming R/W obligations'),
(P/'out/tl-memory-feedback-20261014/source-prior/mask-asplos2018.txt',[(822,836),(882,908)],'primary cached MASK sections5.2 TLB-fill tokens and5.4 DRAM priority'),
(P/'out/tl-memory-feedback-20261014/ordinary-cost/cost-interface-card.md',[(1,101)],'retained37-category full-cost scope; no closed R2 totals reused'),
]
sources=[]; blocks=['# 本轮定向原始源码摘录\n\n仅对已经读取的关键文件建立身份与行号；不绑定新bit，不执行或重解析实验。\n']
for path,ranges,role in entries:
 data=path.read_bytes(); lines=data.decode().splitlines(); sha=hashlib.sha256(data).hexdigest(); snippets=[]
 for lo,hi in ranges:
  assert 1<=lo<=hi<=len(lines),(str(path),lo,hi,len(lines))
  content='\n'.join(f'{n}: {lines[n-1]}' for n in range(lo,hi+1))
  snippets.append({'first':lo,'last':hi,'numbered_text':content})
  blocks.append(f'## {path.relative_to(W)}:{lo}\n\nSHA256 `{sha}`；{role}。\n\n```text\n{content}\n```\n')
 sources.append({'path':str(path),'sha256':sha,'bytes':len(data),'role':role,'excerpts':snippets})
(R/'source-snapshot.json').write_text(json.dumps({'schema':'pte-prepare-root-source-v1','goal_createdAt':1791946350,'observed_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'critical source identity and exact excerpts, not experiment execution or new live hardware binding','sources':sources},ensure_ascii=False,indent=2)+'\n')
(R/'source-excerpts.md').write_text('\n'.join(blocks))
card='''# PTE准备期payload读准入机制卡（2026-10-14）

**稳定裁决：提前通知与承诺前gate有合法实现路径；仅保护PTE等待的版本被具体普通规则覆盖。完整已拥有任务的max/sum差异仍开放，当前来源没有完整R/W/B正例，不准入微benchmark。** 实际goal1791946350，无额外token预算，半工作日范围。root亲核协议、设计与裁决；唯一新Sol/ultra独占席推导普通控制。论文未完成。

## 问题、已有证据、进入和结束条件

PTE已经被PTW→DS接口接受，但还在现有spill/delay中；这时DDR尚看不到这笔PTE。一个当前合法数据读若先被下游接受，可能形成后继PTE不能越过的R债务。原NormalTL调度的firstVALID已锁结论保留，本轮另取physical payload AR的新下游首次VALID之前，比较ADMIT与WAIT；不重新选已held翻译请求。

已有资产：E1/E4硬件等待与供给判别、E2替换/首次再用、E3受控局部HOL；method88/M0/stage117的守恒、完整退休、普通强对照及成本接口。R1/R2/R107/C112与停止的short-mn-a1入口结论保留；F1全族open。上一轮MIG顺序边界仅是条件约束，未证明固定服务时延。本轮不重跑任何资产。

进入条件是：实际接受事件、合法gate、逐拍准备链和冻结普通外的完整选择差异。结束条件允许“具体覆盖/未成立”的稳定停止；先完整交付与五文档同步及必要新核验，后actualcomplete。

## 合法输入与两动作

P是`ptw_req.ar_valid && ptw_resp.ar_ready`已经握手的同一PTW-PTE，且未在绑定的共享DDR目的AR出口接受；单PTW、完整walk、IoTLB16不变。L是原source2物理completion路径当前已拥有、合法的DRAM payload AR，原read-admission/fullbuffer/owner/same-ID/credit均满足，新gate下游尚未首次VALID。不能把未经接受的未来PTE或下一job当义务。

ADMIT：让这个既定L首次向下游offer。WAIT：保留L的原producer owner和完整header，等这一笔P实际在共享DDR AR出口接受，再按同一规则评估当前合法头。比较的是当前已经拥有的义务最晚完整完成，其次完成时间和，再年龄/nativeRR；任何必要历史/class/epoch无效或overflow都回退O-currentDS+RR。预测仅改变准入，不改变服务返回、结果所有权或原容量。

数据可观察接口：PTW DS输入真实接受；对应P真实共享出口接受；源与job/sequence/context的有效关联；原header/LEN/SIZE；同一公共快照内可证明的R/W/B余额及退休；已正常完成样本及有效位。counter/phase只说明当前delay头准备，**P在旧头之后时不能借旧头counter给P预测**。active slow-walk owner须与fast-hit分开；localID0/fullID48只标PTW流，不能替代bank/job归属。全部端口与关联记录付费。

## 承诺前gate及held守恒

概念位置是`iommu_comp` physical AR生产端与系统xbar source2之间。旧上游VALID及header仍按原协议保持；本gate只控制新的**下游**firstVALID。不是NormalTL二次排序，也不关断已进xbar cut、atomics、CDC或MIG的AR。

```text
open = held || grant_new
out.ARVALID = in.ARVALID && open
in.ARREADY = out.ARREADY && open
out.AR = in.AR                    // 所有字段直接保留
held_next = out.ARVALID && !out.ARREADY
```

以上仅为未实施的协议合同。out第一次VALID即承诺，held必须持续至原握手；新的P通知、历史/owner变化不能撤回或更换header。before firstVALID可按付费一致快照重新评估。未知类别、非DRAM或缺身份不永远暂停，回退原合法路径。AW/W/B/R完整直通，不新增header/payload queue、成功许可、walker或hidden memory port。

O-currentDS+RR是本轮**普通比较合同**：共同系统selector在尚未锁定时，相关当前合法DS VALID优先，否则给合法payload/nativeRR。实际原xbar是native仲裁，不能把该DS优先当既有免费硬件。gate-only实现不能凭source2自封获得source3严格DDR优先；若实装共同selector，位置/hold/CPU与debug义务、选择成本均须另计。DS输入的可见VALID来自独立source3，不从受gate影响的ready反馈构造组合环。已held的共同winner永远先行。

单P_pending只在真实PTW接受时置位，在同一P的共享DDR目的AR出口接受时清位；该参考P fullID48、单在途PTW，仍须认证匹配。这里是**xbar出口/atomics与CDC之前**的CPU时域接受，不是MIG接受、返回或PTW消费；出口之后仍有旧债务。基本wrapper无需凭空加MIG CDC；若估计使用MIG事件或精确内部队列，CDC、时间对齐与合法观察接口必须付费，不能读取隐藏状态。

## 准备链与52拍的严格含义

关键原文见[source-excerpts](source-excerpts.md)、SHA见[source-snapshot](source-snapshot.json)。DSIF七输入AR仲裁后是两项非旁路spill，PTW handshake可在delay READY=0时把P放入其原有槽。stream_delay没有在准备阶段接受新AR或复制payload：payload直通，直到Ready才让上下游握手。DS input/output配置各50、randomstall0；本处只讨论input AR。

| preedge时刻（该头第一次delay输入VALID为0） | state/count | 输出与owner |
|---|---|---|
| 0 | Idle | valid_o/ready_o=0；edge0加载50 |
| 1 | Valid/50 | producer继续持header |
| 50 | Valid/1 | edge50减至0 |
| 51 | Valid/0 | edge51转Ready，counter可wrap，不能单独读数 |
| 52 | Ready | 首次输出VALID；只有ready_i=1才接受 |

所以F=50时是**52个同域时钟间隔至该头delay输出首次VALID**。空spill在preedge E接受P后，下一preedge才对delay首次VALID，因此首次输出最早E+53；若P在旧spill头后或delay忙，另加真实队头等待。Valid且count=k时到首次输出尚有k+1间隔；Ready剩余准备为0但ready_i可持续为0。上述全部是源码状态推导，不是新测量，也不是到DDR接受的deadline、DDR服务52拍或新的物理Fmax验收。

P共享出口先接受，随后R真正到PTW，PROC_PTE消费后才MEM_ACCESS下一P；正常一PTW两者之间有pending=0区间。四规则在该区间允许原合法payload，首次offer后即held，即使随后新P pending也继续完成。这里只给条件进展机会；需要原READY/返回/consumer最终前进，不声称连续多DS/异常/所有任务的墙钟有界公平。没有新增配额、one-burst/one-child限制或免费reservation。

## 四个冻结完整在线普通控制

完整公式与手推见[独占席结果](../ordinary-bound/controls-and-bound.md)。共同次序是held→原安全/owner/credit→实际native公平→本规则；缺历史/overflow回退O-current。

| 控制 | 本轮冻结规则 |
|---|---|
| O-currentDS+RR | 当前合法未锁DS VALID优先，否则当前合法payload按nativeRR；准备中的P不算DS VALID。 |
| O-earlyPause | 已认证P_pending时不offer新L；oldheld正常走。同一P出口真实接受或来源认证原生终结/reset才清暂停。无身份回退current。 |
| O-owner | 对X=L或P，使用认证的`(释放input-W-owner,释放output-W-owner,直接释放的已拥有阻塞路径数)`，按字典序；无证据字段为0。L优则allow，P优则wait，等分回退current及nativeRR。不得按bank标签造关联。新增关联接口/匹配同付费。 |
| O-cheapDebt | 冻结last-normal样本G（DS输入接受→共享出口接受）及每拍S_R（共享出口接受→真实R-last，向上取整最小1）；RhatP=now+max(0,G-ageP)，FhatL=now+paid_dispatch+sum(n_c*S_R[c])+(LEN_L+1)*S_R[L]；FhatL<=RhatP允许，否则等当前P真实出口接受。n_c仅已证明在L之前的已拥有R拍；class/域/epoch/历史/overflow无效回退。 |

root把独占席O-owner未定义域冲突的fallback细化为以上确定规则，未运行/用结果拟合。时间位宽/class细分与实现扫描尚未冻结，所以估计不等于精确物理周期、数字成本仍null。G/S含排队与反压，再加n_c可能保守；不是隐藏DDR服务分解。普通控制可删自己不用的历史，不能强加joint rollout费用；同信息是同付费可用接口，不是同无用逻辑。九个旧NormalTL排序合同不移植为此四个AR准入规则，不能逐场景拼最佳。

## 决定性边界、符号正反方向及拒绝理由

只在同一有序R服务器、相同公共前缀完成d、P策略无关ready r、L/P服务a/b不变且无W/B/其他义务/反压/owner联动的交换域，令g=max(0,r-d)：

`CP_admit=max(d+a,r)+b; CP_wait=max(d,r)+b; ΔCP=max(0,a-g)`。

a>g的PTE保护被O-earlyPause完整复现；a<=g填准备空档由O-cheapDebt的一个scalar门限精确区分。**因此关闭“减少PTE等待本身”“准备空档阈值本身”的独立贡献主张。** 这是具体普通实现/条件定理覆盖，不是发表前作全覆盖，更不是完整copy收益被否决。

只有L/P R退休终点、g>0时，ADMIT的max反而少min(a,g)，即使a>g仍应admit；O-current也admit。这提醒不能把PTE变快等同完整任务变快。g=0的max打平时，sum按a/b排序。

保留一个正向开放机制边界：P完成后经h释放**已拥有**Q，Q与L/P同一有序R服务器，Q服务c，仍没有W/B/其他义务，且所有参数跨策略不变。移去公共d：

`CL_A=a; CQ_A=max(a,g)+b+h+c`

`CL_W=g+b+a; CQ_W=g+b+max(a,h)+c`

`M_A-M_W=max(a,g)+h-g-max(a,h)`；max平时sum差`-(g+b)<0`，故ADMIT iff `min(a,h)<=g`。

保持a>g>0：h_high>g时WAIT，h_low<g时ADMIT。无认证owner关联时，O-current/O-owner均admit两者，O-earlyPause/O-cheapDebt均wait两者；四冻结规则没有一个作为统一在线规则覆盖这对条件。新cheap双阈值可覆盖这个窄代数域，**它不是预先冻结O-cheapDebt，不能事后加入替各场景选赢家**。

这对式子不是已接受正例：实际DEFAULT copy有R→W streaming、AW/input/output owner、真实W/B和最终job/STOP/drain；h与a不是可免费观测的常量，Q与当前任务的完整关联未证。不能删掉W/B、把stream强制串行、插入等待/额外尾部或变动A job数来落地该式。因此完整joint家族仍open/unknown，当前source-bound完整收益null，不运行模型或四条件。

## 全部工程成本与生命周期

37分类按原[成本接口](../../tl-memory-feedback-20261014/ordinary-cost/cost-interface-card.md)逐项保留，但本gate新增delta见[full-cost](full-cost.json)。不继承已关闭R2的1329bit或扩展raw槽。原onePTW/IoTLB16、DMA0/1 buffer512及队列/信用不增；概念wrapper至少P_pending与held两控制bit，**不是完整总state/FF**。counter6+phase2是8条既有状态tap，不是8个新增FF，也不代表读取端口/布线免费。

必须计：同P与L的source/job/epoch关联、真实错误终结、pending/head位置与prepare事件、原完整header保持、所有R/W/B及same-ID/无WID W owner、接受和退休事件读/更新端口、同时事件冲突、counter/历史valid/overflow/time域、一致快照、历史生成时间/字节、可证明前序debt、预测两反事实scratch/控制及max/sum算术、owner比较与共同DSselector、gate/hold/年龄/公平逻辑、选择延迟/critical path、必要CDC、CPU/ABI/config/STOP/drain。状态宽度、端口、延迟、LUT/FF/BRAM/面积/Fmax/energy/CPU/应用与双方完整收益全部null；零容量增量是范围约束，不是零成本结论。

正常copy只在最后backend B完成后可下一job；输入W进入FIFO、R-last、PTE消费、sharedAR接受互不等于完整task退休。原异常sink/错误B/R和reset所有权继续执行；本轮未为general cancel/remap增加free能力。

## 前作、接受/关闭及下一步

root本轮直接查MASK primary §5.2/§5.4与已有缓存原文，并打开官方论文URL。§5.2 tokens限制TLB fill而非payloadAR准入；§5.4在DRAM translation优先并为应用data公平分队列。它覆盖“下游翻译优先”组件，**没有在本轮核查中建立这个已接受PTE准备期gate及完整已拥有max/sum规则的全文覆盖**；同样未建立独立创新。准确[论文原文](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)，证据[prior-check](prior-check.json)。不把ISCA2018/Johnson/旧turn的结论扩大为本新动作全覆盖。

接受：真实upstream-PTE-owned→prepare→outlet链、合法承诺前动作、有限条件ordinary覆盖和开放joint依赖边界。关闭：PTE等待/fit-gap独立贡献版本。未接受：实际同源完整R/W/B正例/固定工作量收益/数字全成本/前作全族覆盖/论文贡献。五文档及必要新交付核验后可完成这个有界goal。

下一输入须一次包含同一正常来源的当前已拥有释放依赖、合法pre-firstVALID快照、固定可比较完整工作量、全R/W/B终点及同付费有效历史，形成冻结普通控制外具体选择。允许据新事实重设计，但不自动扩大来源、再开同类边界审查、恢复F1停止实例或关闭分支。过完整差异及成本依据才另立有限microbenchmark合同；最多四条件随后，GPU/AI实际共享路径与RTL/FPGA再后。原立题窗口累计，本轮完成不代表论文完成。
'''
(R/'mechanism-card.md').write_text(card)
nulls={k:None for k in ['state_bits_total','metadata_bits_total','read_ports','update_ports','matching_logic','history_generation_bytes','history_generation_time','selection_latency','lut','ff','bram','area','fmax','energy','cpu','application_time','both_task_completions','total_completion_gain','net_gain']}
cost={'schema':'pte-prepare-full-cost-v1','goal_createdAt':1791946350,'status':'lifetime and interface closure only; complete numeric costs unmeasured','retained_categories':37,'inherited_interface':'iommu-platform/out/tl-memory-feedback-20261014/ordinary-cost/cost-interface-card.md','closed_r2_totals_inherited':False,'conceptual_wrapper_control_bits_lower_bound':2,'lower_bound_not_complete_total':True,'existing_delay_state_tap_width':8,'tap_is_not_new_ff_or_free_port':True,'payload_or_header_queue_capacity_increase':0,'minimal_wrapper_event_domain':'CPU domain PTW DS accept and pre-atomics/CDC xbar DDR-out AR accept','mig_event_cdc_required_if_used':True,'comparison':'shared paid observable interfaces plus actual per-rule minimal required logic; ordinary may delete unused predictor history','scope_groups':['complete native fullID/header/W-owner/sameID/RWB/error lifetimes','P_pending exact identity and native terminal/reset','new downstream firstVALID held control','source-job-epoch and direct dependency associations','actual accepted prefix debt and retirement events','history class keys, timestamps, valid/overflow and acquisition work','read/update ports, simultaneous event arbitration, snapshot consistency','counter/phase fanout and head-membership proof','joint forecast virtual state and max/sum arithmetic','frozen ordinary scalar/owner and common DS selector cost','legal fairness and age; no free quota','decision delay, combinational path and any pipeline storage','only necessary CDC/time alignment','CPU ABI/config, STOP/drain and complete owned work'],**nulls}
(R/'full-cost.json').write_text(json.dumps(cost,ensure_ascii=False,indent=2)+'\n')
prior={'schema':'pte-prepare-prior-component-check-v1','goal_createdAt':1791946350,'primary':'MASK ASPLOS2018','url':'https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf','retained_pdf_sha256':'f72e642e761582784d7508dabc743b5e5176f1bb2cb11282260bef18545c41fb','retained_pdf_bytes':1145387,'cached_text':'iommu-platform/out/tl-memory-feedback-20261014/source-prior/mask-asplos2018.txt','critical_ranges':[{'lines':[822,836],'section':'5.2','finding':'token policy gates TLB fill, not payload AR admission'},{'lines':[882,908],'section':'5.4','finding':'downstream translation-first memory queues with application data fairness'}],'this_goal_browser':'primary PDF open succeeded; subsequent find calls returned internal errors, no evidence from them','new_direct_quoted_words':0,'coverage':'known priority/token components distinguished; exact prepare-gate plus owned max/sum full coverage not established','independent_novelty_accepted':False,'no_new_broad_prior_search':True}
(R/'prior-check.json').write_text(json.dumps(prior,ensure_ascii=False,indent=2)+'\n')
accept={'schema':'pte-prepare-root-acceptance-v1','goal_createdAt':1791946350,'verdict':'stable bounded stop before microbenchmark','gate_legal_as_contract':True,'rtl_implemented':False,'accepted':['actual PTW DS-interface ownership before delayed downstream offer','F50 delay first outputVALID is F+2=52 same-domain intervals, conditional spill-head mapping','pre-firstVALID payload gate preserves original upstream header and downstream held','four fixed online ordinary controls and scoped PTE-protection scalar theorem','R-only owned-Q condition min(a,h)<=g preserves open joint-completion boundary','complete interface/lifetime cost categories, numeric totals unknown'],'closed_claims':['PTE-wait protection action alone as independent contribution','fit-gap scalar selection alone as independent contribution in declared R-only exchange domain'],'ordinary_coverage_scope':'O-earlyPause covers protective pause; O-cheapDebt threshold covers fit-gap only under fixed ordered-R exchange assumptions','joint_full_completion_family':'open/unknown; no actual source-bound full R/W/B positive','F1_family':'open; stopped instances retained','source_bound_positive':None,'both_fixed_work_gain':None,'new_microbenchmark_admitted':False,'new_model_admitted':False,'published_full_prior_coverage':False,'independent_novelty_accepted':False,'numeric_full_cost_accepted':False,'root_owned_clarifications':['O-owner freezes a lexicographic certified direct-owner/path score and currentDS+RR fallback; no undefined conflict','child verdict preserved list corrected to distinguish open F1 family from closed R1/R2/R107/C112'],'physical_claim_limits':['52 is delay firstVALID not DDR acceptance or service deadline','past history includes queue/backpressure; cheapDebt not exact MIG latency','same-server R-only Q algebra removes W/B and is not actual DEFAULT copy completion','xbar DDR AR acceptance before atomics/CDC is not MIG acceptance'],'preserved':['one PTW','IoTLB16','original queue and buffer capacities','NormalTL and hit bypass','native read-admission and fullbuffer','AW/W/R/B ownership and error retirement','full source ID and same-ID order','held VALID','old stopped sources and all closed branches','8/9 month assets and cumulative research window'],'execution':{'new_experiments':0,'old_experiments_replayed':0,'models':0,'microbenchmarks':0,'builds':0,'old_validators':0,'trace_replays':0,'production_edits':0,'board_actions':0,'pressure_or_capacity_increases':0,'old_seats_restored':0,'new_exclusive_child_seats':1,'DeepSeek_Claude_Astra_calls':0},'delivery_requirement':'five live documents synchronized and necessary new artifact/source checks pass before actual goal complete','paper_complete':False}
(O/'root-result-acceptance.json').write_text(json.dumps(accept,ensure_ascii=False,indent=2)+'\n')
admission={'schema':'pte-prepare-microbenchmark-admission-v1','goal_createdAt':1791946350,'admitted':False,'reason':'PTE-only selection covered; full owned-task normal R/W/B counterfactual and valid paid history not proven','fake_ab_execution_contract_created':False,'new_conditions_run':0,'future_max_conditions':4,'source_or_workload_modified':False,'numeric_completion_results':None,'paper_complete':False}
(R/'microbenchmark-admission.json').write_text(json.dumps(admission,ensure_ascii=False,indent=2)+'\n')
nextc={'schema':'pte-prepare-next-gate-v1','automatic_execution':False,'active_candidate_family':'new physical-AR admission full-owned-completion family remains open','required_input_package':['one same normal source with new original facts','accepted PTW-PTE identity and prepare-head membership','current owned legal L before new downstream firstVALID','certified currently owned P->payload dependency, with all concurrent R/W/B','fixed comparable complete workload and actual ownership/terminal mapping','valid same paid public service history or lawful fallback','one concrete difference outside each applicable frozen online ordinary control','full implementation interface, acquisition and decision cost basis'],'not_accepted_as_package':['PTE/R-last wait only','R-only Q symbolic pair presented as actual copy','free h or hidden DDR state','policy-dependent A STOP work called fixed','posthoc per-scene best ordinary mix','extra phase/warmup/stall/capacity/concurrency to create an opportunity'],'on_missing':'preserve bounded result; no automatic source hunt, same-boundary review or branch reopening','if_survives':'register separate finite fixed-work microbenchmark goal/contract; full completions plus cost; frozen maximum four conditions only after first difference holds; then one GPU/AI shared-translation workflow and RTL/FPGA','retained_F1':'open family; no restore of stopped entry','closed_branches':['R1','R2','R107','C112','other prior closures'],'models':'Codex-only gpt-6.1-sol ultra native; necessary fresh exclusive seats fork none','extra_token_budget':False,'paper_complete':False}
(O/'next-goal-contract.json').write_text(json.dumps(nextc,ensure_ascii=False,indent=2)+'\n')
ledger={'schema':'pte-prepare-execution-ledger-v1','goal_createdAt':1791946350,'root_work':['register actual goal','direct critical source read and hand-derived protocol/timing/gate design','direct primary MASK component check','accept bounded ordinary math and complete-lifecycle distinctions','new artifacts and five-document preservation/synchronization','necessary new delivery checks only'],'child':'one fresh exclusive /root/prepare_ordinary_bound gpt-6.1-sol ultra fork none; algebra and strong-control boundary only','root_corrections':['clarified child verdict F1 open vs R1/R2/R107/C112 closed','made root O-owner certified direct score and fallback decision-complete'],'read_diagnostics':['one guessed translation-logic filename absent; no source evidence obtained there','PTW bank-name search had no hits; no bank/job association claimed free'],'browser_diagnostics':'primary open succeeded, later find internal errors not used as evidence','old_experiments_build_validators_traces_run':0,'new_scientific_executions':0,'production_or_bench_edits':0,'new_child_seats':1,'old_seats_restored':0,'external_models_called':0,'context_config_modified':False,'state':'stable verdict; completion awaiting new delivery check and actual tool receipt'}
(R/'execution-ledger.json').write_text(json.dumps(ledger,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'root_sources':len(sources),'root_artifacts_written':['source-snapshot.json','source-excerpts.md','mechanism-card.md','full-cost.json','prior-check.json','microbenchmark-admission.json','execution-ledger.json'],'stable_acceptance':True,'microbenchmark_admitted':False},ensure_ascii=False))