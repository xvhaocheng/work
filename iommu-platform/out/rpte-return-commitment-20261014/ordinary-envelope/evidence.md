# 完整普通返回合同：源核与边界

本席只核普通控制的原始接口、准确WRITE准备与完整accepted-source返回义务；root另核PTW/共享queue/fresh双头，另一席另核NativeN/RREADY/drain。本轮0新模型、0仿真、0旧test/validator/build，未改旧源码或公共文档。以下是源和手推合同；所有新数值费用为null。

## 先行结论

如果完整普通规则从同一个可用descriptor t0先付准确WRITE准备，再在源AR首次VALID前取得整返回容量或唯一非全缓冲lease，它能预防“后续目的WRITE/PTE尚未准备，payload先heldR”这一类环。stage83已经给出同一条完整普通envelope规则，不需要把整个F1预测器复制给它才能作对照。

但该规则不是当前production免费已有的能力：nonclaiming准备/可信PA复用接口不存在，RAWCouplingAvail=0；真实512拍全缓冲只适用于nonblock前两路及证书限制。若root的fresh-TL t0已经有缺WRITE覆盖的长源AR被接受，不能把ordinary所需的先准备/先预约补进共同初态。比较必须从该accepted初态只执行仍合法的动作，或另从共同descriptor t0计完整准备/预约费用。

## 原始证据入口

| 证据 | 直接原文入口 | 本席核到的事实 |
| --- | --- | --- |
| 九普通在线规则 | `out/tl-memory-feedback-20261014/ordinary-cost/baseline-contract.json`完整原件 | common held/fair/domain及九条冻结策略保留；scene best-of不能称online。 |
| stage83完整普通合同 | `out/innovation-mechanism-focus-20261009/c-mainline-positive-design.json:14–29,65–71` | 独立READ/WRITE准备、完整source envelope、唯一nonfull primer、最后true physical WLAST放lease、R/B独立退休是具体正向普通构造；不是已实现native接口或整套已发表方案。 |
| native WRITE请求 | `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:625–643,674–702,714–724` | WRITE从实际AW队列进入TL，响应实收后才走completion；没有AW外的非claiming准备路径。 |
| 顶层类型 | `cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv:135–165` | DID/PID/ttype/priv/debug/bank与PA/range/error/permission在真实消息中；没有prepared-parent origin/lease完成口。 |
| 真实W所有权 | `cva6/vendor/pulp-platform/axi/src/axi_mux.sv:207–257,273–289` | `w_fifo_push`在首次AW offer就发生；W队列只在真实LAST握手pop。因此不能把AW未握手当可免费重选准备。 |
| 准确准备与当前/下一门 | `out/payload-frontier-poc-20261007/after/nonblock_completion_pool_late.sv:94–124,184–228,261–270` | 地址offset/len/size、当前W头、next在两个order queue的紧邻、真实comp AW准备和旧source有序共同决定。 |
| 准备观测口 | 同轮`after/nonblock_actual_dma_top.sv:638–665` | job_fire是frontend→job FIFO，prepared来自实际comp AW handshake，虚拟/物理W owner来自实际mux first offer/真实WLAST；comp不是最终DDR端。 |
| 简化普通frontier | 同轮`after/nonblock_completion_pool_late.sv:146–169,207–211,448–450` | simple功能门可省full-prefix scan；scan在旧测试是独立fault16观测。不能把观测硬件当已经省掉/已有面积数据。 |
| 全预约Mode5 | 同轮`after/nonblock_completion_pool_late.sv:189–218,274–297,448–475,483–509` | 每源独占完整256拍slot在completion admission前配置；物理收到与回放到source分别计，upstream最后R交付后才释放该metadata/slot。 |
| 原生全缓冲证书 | `cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:14–45` | 4096B容量、normal/aligned 4KiB、受序列化单job限制，未知重叠/退休fault撤permit；不是任意burst动态分配器。 |
| coupler实际触发 | `cva6/corev_apu/iDMA/src/idma_channel_coupler.sv:105,144–157,162–178` | 真实首R握手才给一个AW信用；输出AW有效锁有真实fall-through保持。 |
| RW与AW解耦分离 | `cva6/corev_apu/iDMA/src/idma_legalizer.sv:209–214,303–304,403–439`；frontend`:365–367` | RW可以独立切段；AW decouple是另一独立字段，frontend保持0，不能自动补足多个destination child信用。 |
| production配置 | `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1255–1281`；backend`:811–858` | coupling关闭；nonblock DMA0/1为512拍，其余3拍。源available不等于该模块当前综合启用。 |
| 当前数据就绪 | `cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:343–348` | first_possible按实际W首拍mask的全部byte lane；任意对齐不能用一个R pulse代替。 |
| descriptor/B寿命 | `cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:173–202,306–343`；backend`:519–532` | accepted descriptor先入job FIFO，backend领取后FIFO可pop；normal completion来自最后write response。生产NO_ERROR_HANDLING不能冒充完整故障合同。 |
| DSpriority原始规则 | `tests/nonblock_actual_dma_test.cpp:652–699,1290–1305,1371–1399,1691–1725` | accepted队列才可promotion；heldR不能换；目前原代码窗口截止Vdone，本轮对照必须扩至所有任务/响应/faultquiet。 |

`ordinary-contract.json.source_sha256`冻结上述必要原件摘要；未重跑全76源hash、旧37分类审计或R2费用计算。

## 准备、返回、W顺序和B是不同寿命

1. 准备授权须成功验证完整source READ和独立destination WRITE。返回envelope是每一条已经offer/接受的源AR的所有尚未到达/仍resident字节，不能只算当前AW剩余债务。
2. 非预约长AR首VALID前，必须已有完整WRITE覆盖和合法可排空顺序，或有真实全返回容量。瞬时WREADY、同PA、缓存valid和仅第一个child准备都不够。
3. 可分离的完整普通lease在源AR firstVALID前取得，所有后续child都受同lease保护。prepared-but-waiting task可以继续准备，却不能偷偷接受第二个无容量长read。
4. lease在整个envelope最后**true physical WLAST**结束。虚拟WLAST只说明payload已进入下游storage；first child LAST和null LAST不能归还whole-source lease。
5. PA/WRITE授权与child/B账本仍要保留到每个实际B结算；source READ完成另由实际source RLAST确认。task成功晚于完整true bytes和实际响应；W-order释放不等于task成功。
6. 失效/代次wrap必须stop-new-use和完整drain/command completion。固定stable mapping是首轮限域，不把16-bit generation或cache replacement当一般revocation证明。

## 两个源级补正

**一AR→多AW不能原封不动使用stock coupler。** 首R事件只产生一次AW credit。手推：假定无旧信用、decouple_aw=0，一个AR首R握手释放child0；剩余R不是first，child1等待而没有第二次信用。RW独立分段本身不改AW coupling。因此old C卡“existing coupler”对齐一对一子段是准确的；泛化多child必须加按真实child首可写byte放行的envelope绑定接口，或约束等分段。双方都计这个边界成本。此补正不把普通整合同未实现反过来当候选创新证据。

**旧source gate的sticky注释没有对应正常赋值。** saved payload snapshot在always_ff的reset分支`:301–349`中，`:327–328`更新source_read_release_q，正常else分支没有该更新；更早write-order snapshot`:272–273`同样如此。这只能证明该变量没有实现所述正常sticky行为，不能证明旧受限实验已发生VALID撤回。原有predicate在某些restricted场景可能自然稳定；若新场景依赖跨predicate变化保持源AR offer，必须单独证明或支付真正锁定逻辑。本轮只标接口限制，不修旧源、不重跑旧测试、不更改旧结果。

## 覆盖分类与root待决接口

root提供的新增源事实是：当前private fixture只有两源，每源512拍；normal/aligned sole4KiB源返回全部能驻留，所以这组本身不能造capacity-heldR。production是四source IDs0/4/8/12、buffer512/512/3/3，而bankread_admit是coarse bank，upstream W owner是fullsource；src0 W头和src2同bank小FIFO的危险必须按fullsource与每层返回槽收费。本席没有重复NativeN/RREADY调用链，该事实最终由native席和root的原始源证据接受。

已补一条**固定O-envelope-select**，它只投影已有普通完整envelope的安全判别：保持held/force/fair先行，两个native合法fresh头中，若精确READ的owned/即将由这个已captured头兑现的return footprint不能由当前真实可用容量或独立可前进W lease承接，另一头是唯一certified safe，则先safe头。safe READ可以由完整return footprint≤source真实可用容量+paid完整中间槽预约证成，不要求AW；safe WRITE必须有对应source真实W供给或独立可前进lease，不能凭类型。两safe/两unsafe/unknown按原paid age/RR，单头照native，不新增AR hold，不追认源prefix，不计算两个AB/BA未来轨迹。

这条规则的同动作结论是明确有条件的：**如果**root证成一个unheld、unforced、两个合法head的unsafe READ/unique safe head状态，**并且**候选在同snapshot下也先safe头，**那么**这对次序选择已由这一个ordinary规则覆盖。必须继续核实际头合法性、完整safe transition的R/W/B后果与同终点，不能仅凭WRITE名称或bank匹配宣布同事件。若source身份不可绑定、two hazards、强制fair选择或accepted prefix已unsafe，则该投影保留未知/限制；不能把不同t0先挡AR当解答。风险充分条件并不保证每种后续内存服务都一定触发heldR。

选择投影新增的精确信息费用是：native头到fullsource/job/byte envelope的可信关联、已offer/接受源AR的剩余返回与resident字节、每层真实空间及所有者、完整WRITE coverage和各W-owner路径/lease、两头safe/unsafe判断以及保持VALID/公平的选择。当前native消息和test观察未证明可免费组合这些wires；数值为null。若候选也用同snapshot，观察成本应两边同计；普通rule不需要F1双rollout/未来PTE、PA或RREADYoracle，不能强给它无用predictor。

| 新hold类型 | 已有具体普通可覆盖的条件 | 不能据此自动覆盖的情况 |
| --- | --- | --- |
| destination冷WRITE在长source开始后才发起 | 从共同descriptor t0使用O-accurate/current-next或O-full-envelope先完整准备，且其源门在firstVALID前合法 | fresh-TL t0已有不全准备的source AR被接受；不能倒填预史 |
| 多个已准备但不全缓冲source抢先返回并锁R | O-full-envelope固定单primer在各AR firstVALID前挡第二流，或O-complete-return-reservation实际有完整slots | 两AR已接受且相应lease/reservation未在初态存在 |
| payload先于下一PTE heldR | 当PTE已经入mem_q且R尚未held，O-DSpriority-all-retirement可以在保持full-ID序的条件下先选PTE | PTE未生成/未接受；或payload已held；优先级不能召唤未来请求或撤VALID |
| 前序physical W尾停顿但无需新R | 旧O-virtual-W-head+matching comp-AW在严格two-job正常domain覆盖；原source/独立observer记录可解释该动作 | 缺准确ID/job/child关联、仅AWVALID、任意多job/fault状态不直接外推 |
| 新fresh TL双头次序改变payload/PTE反馈 | unsafe READ/unique safe head对由冻结O-envelope-select先safe；另将同初态/同paid观测/所有权的AB/BA事件带入保留的九规则；若某**预先冻结的单规则**同动作/费用即本见证covered | 仍须root证实际两个legal heads、完整安全谓词和终点；未知身份/双危险/已unsafe前缀不自动覆盖；不同scene选不同ordinary只是offline envelope |

root交付时需要：全部accepted/held历史、合法fresh heads、各head到已owned R/W/B的真实关联、下个PTE生成/DS offer/acceptance与payload首次VALID/held时间、实际每个storage位置所有权、prepared全范围/child/W-order，以及共同完整退休终点。所有数字费用继续null；不以“ordinary能加整套F1”作否定，不称一页合同已经证明全部family/公开前作覆盖。
