# §68 两路并行 prefix repair：独立关键源码审核

结论：本席直接核 design/source/top/q 的实际源，在下面严格合同内，未构造出 RID 混路、phase1转移与capture覆盖、held logical R/AR变化、提前共用suffix或phase2缺payload的W头阻断反例。结论由下面的有限状态与事件链给出，**不是形式工具证明，也不是本席执行的实验验收**。root报告18项copy PASS，本席没有读这些run/CSV或执行build/test，不能把该报告写成本席独立复验。

合同为每reset单个2KiB normal job、独立成功READ和完整目的WRITE grants、源PA/normal属性相同且immutable、无invalidate/fault/revocation/pause、新child Q2、既有B3 private FIFO/一个global skid、合法AXI同ID顺序及正常路径最终服务。只覆盖此实际route、此固定metadata重用。架构新颖性仍需强普通并行/顺序基线及准确前作，源码安全归纳本身不确认创新。

## 实际读取的版本

本目录 `shared_pretl_read.sv`、`q_write_splitter.sv` 全源，`multi_actual_dma_top.sv` 的参数、direct ID、W child准入和grants，`multi_system_top.sv` 的参数、AR字段与直接R返回。line citations均指下面SHA的实际文件。

| 文件 | SHA256 |
|---|---|
| shared_pretl_read.sv | `9edadae8d27006d68dcc246d73a5a35a83f982d575a0a48850dc23c1d72c609d` |
| q_write_splitter.sv | `d1079adf9a710ba7871749711d8ed1aa82aad24dd8922f40c72e4ec7e60d948e` |
| multi_actual_dma_top.sv | `83bef15ce0908149987ab93b5827590c07690e80817c7cad61d803f50145826c` |
| multi_system_top.sv | `76304ee6e816c8292c8a26cdbd4daeb6198333bee269d020c8ee449b5dc9e214` |
| codex-design-before-run.md（E256说明修订后） | `3e8b81671856a53f3339e1e88ec95738661418af717ffadc7d75a1df7abffbc0` |

以下source简称S、qwriter简称Q、actual top简称T、system top简称Y。

## 1. 进入phase2前，旧V metadata可以被回收的准确时点

start_cutover要求旧physical child正在busy、global skid为空、V/A两个原parent存在且未logical done、两个READ成功、目的WRITE成功前沿都为256（S:110–115）。late join另核物理PA相同及cache/prot/qos/region/user相同（:241–252），mode3要求A尚未收到任何logical R（:253–256）。原parent只能是src对应、len255/size3/INCR、正常属性、ID0/4的单父（:211–218）；这些parent寄存器此后不再被另一job覆盖。

busy旧child确为V：普通共同child用parentV/owner0（:315–335），独立/已detach分支中，A只能在doneV==1后选（:338–340），而start_cutover要求doneV==0。因此old route可以使用原parentV ID，无需另存任意owner。

phase1的R门在beat_index>=J时关（:153）。转移条件同时要求skid空、returnedV==J、真实rawWV==J、returnedV>=promisedV（:262–268）。因为旧physical每次capture立即推进child_got，而下一physical capture必须等global skid完成logical交付，skid为空时 `child_offset+child_got==returnedV`。于是转移边沿旧R索引恰J，RREADY已被phase1门关：不会在把busy复制到old_v_busy并清busy的同一个边沿再capture旧V beat；转移还在phase1，不会同时进入phase2 A builder。

若J==E，旧physical RLAST必已capture并logical交付，旧busy=0，转移old_v_busy=0。若J<E，旧busy=1，old_v_busy=1保留它。仅靠skid为空不足以证明这个分岔；实际beat/RLAST和returned相等提供了所需事件链。

## 2. 两个physical RID与一个skid为什么能重建索引

T:595发送原source ID或bit0，故V direct ID为1、A direct ID为5；T:986只清bit0，恢复S看到的ID0/4。normal TC R只走偶数ID支路（T:589–592），source的pre_direct固定1（S:89），异常TC R重叠被fatal（T:615）。Y:1335–1337直接把completion Rvalid/channel/ready接回，没有在这里按“只有一个child”重写RID；Y:562–572也直接保留新direct AR的ID和正常字段。

phase2中，只有 `old_v_busy && RID==parentV.ID` 被认成old V（S:122–123），其next index取returnedV（:126）、成员仅V（:129–130）；其它合法physical child必须是main A，其index取child_offset+got。old V capture不会增长main got，也不会清main busy（:376–381）；A capture核RID==child.id并核main RLAST计数（:367–368）。old V RLAST核returnedV+1==E（:364–365），只清old_v_busy。

global skid已有数据时所有新physical capture都关闭（:132）。physical capture写skid/data/pending（:373–375），logical handshake才增长对应returned（:383–386）。因此old V在下一次允许physical capture时，其上一拍必已经logical交给V，returnedV恰是下一未交付索引；不需要再保存旧physical got。这个重建不允许第二个payload skid，也不能推广到两个old V child或乱序同ID。

当输入RVALID=0时RID可以无意义，incoming_old_v仍可组合改变RREADY，但所有capture更新都受RVALID&&RREADY控制（:362）；它不会凭invalid A/V字段更新状态。若合法main A还未AR握手，busy=0，A-ID分支RREADY为0；若未知RID实际VALID且被允许capture，现有RID核会fatal，不能当作合法source data交付。合法slave不得在A AR接受前产生其response。

## 3. 最危险同拍和相邻边沿的逐项检查

| 事件 | 实际旧状态/门 | 不发生覆盖的原因 |
|---|---|---|
| phase1 busy转old_v_busy，旧V又想送J拍 | returnedV==J且skid0 | phase1 beat_index==J使RREADY=0；只有route转移，无capture |
| phase2首次构造A prefix，同时capture旧V | main offered/busy/skid都0，old_v_busy可1 | builder只写main A header/range；oldV capture只写global skid和可能old_v_busy，不写main child_got/busy |
| A AR接受，同时capture旧V | offered A=1、main busy=0 | AR分支把main got清0、busy设1；oldV分支不触碰它们 |
| A AR接受，同拍出现A R | AR前main busy=0且RID A | A RREADY=0，不能同拍使用尚未建立的main route；下一拍才可capture |
| 旧V真实RLAST capture，同时形成A prefix | old_v_busy=1、main空、skid0 | main A header可以写；旧V只清old_v_busy并写V skid。下一拍skid为1，任何其它builder/capture被挡 |
| A prefix真实RLAST capture后立即尝试新prefix/suffix | capture前main busy=1；capture后skid=1 | 当拍builder被busy挡，下一拍被skid挡；logical A交付后returnedA才到E，prefix条件rA<E不再成立 |
| 旧V最后logical交付与shared suffix builder | 交付前skid=1 | builder同拍被挡；下一拍rV=E/skid0，才可检查完整join条件 |

main builder的统一入口是 `!offered&&!busy&&!skid`（S:270），所以held A AR不会因旧V前进而被重写。旧V提前完或A提前完都不绕过上述门，真实两个physical RLAST可以交错，但一个输入R channel每边沿最多capture一个beat。

held logical R也稳定：skid有效且未handshake时data/pending不变，return_owner由pending确定，对应returned不增长，原parent ID/len不改（S:159–163、383–387）；因此生成的ID和logical LAST不变。old_v_busy可能因physical最后beat先清，但其held logical beat仍由skid pendingV路由，不再靠incoming_old_v。S:185–188有完整AR/logicalR channel稳定检查，Q:78–84有AW/W检查；这里的论证没有把“有assertion”当作实际已通过。

## 4. split-E门同时堵住错误suffix与W数据依赖环

A prefix范围是 `[returnedA,E)`，header len=E-returnedA-1，成员A（S:274–284）。它结束时仅A physical RLAST，不生成original logical RLAST，除非E=256；logical LAST始终按原parent len255和returned==255生成（:163）。A不会在旧V仍欠tail时接着发 `[E,256)`，因为suffix builder还要求old_v_busy=0、main空、global skid空、returnedV=returnedA=E（:270、285–299）。

old_v_busy只在真实旧V RLAST清；main busy在A prefix AR握手设、真实A RLAST清（:356–380）。logical rA/rV到E又要求其末拍skid已经交付。因此上述条件同时证明两条physical事务都终止、两个logical前缀都已交到native DMA；不是拿old_v_busy=0或rA相等中的单一条件猜完成。

phase2新W同时受到两条新门：child quantum裁到E（T:712–716），且firstVALID的next_end必须<=自己的source_returned（T:739–741）。Q先组合取min(剩余/Q/quantum)再形成、保存固定child（Q:45–48、96–109），所以已firstVALID的AW没有事后缩短；形成时本child全部payload已logical交入native DMA。

旧V缺data的W债在phase1的rV=wV=J/promise<=J闭合。A旧债在已detach路径上由S:229的returnedA>=promisedA闭合；mode3路径A returned0且新A W被a_hold的缺payload门挡住，直到phase2。在phase1转phase2的边沿，新的V child若超过J会被T:718–723裁为0；新的A child仍受phase1 a_hold约束，只能resident（T:742–743、S:94–99）。所以不能利用“新phase2 resident门下一拍才生效”漏入一个missing-data child。

在phase2，所有未来W头都已有payload，pending旧B也都对应已消费完raw W的child。假设某个V或A physical R因该成员private FIFO/单skid背压而held，它可以通过已经resident的W消费释放库存，不需要另一路新的R来填这个W头。若B信用暂满，旧B最终返回即可解锁新的resident child；B不是未来READ完成的伪造信号。这个条件排除了本候选特有的“held A suffix先堵住oldV缺data W”环，因为根本没有A suffix in flight，且phase2 W不缺未来data。

这是正常下游最终服务下的data-dependency论证；它不保证永久关闭AWREADY/B响应、动态权限撤回、另一个未审查memory service依赖R完成才返回B的系统仍前进。R routing本身不要求slave实际在ID间交错；串行服务也可完成，只会使预期重叠收益消失。

## 5. E奇数、F255、两笔B与原父退休

E奇数时，phase2两flow最后W最多只剩1拍，T:714–716裁出这个短child；Q向下游WLAST按该count生成，native原parent WLAST仍按parent末拍核（Q:59、115–116）。源side旧V RLAST按returnedV+1==E，A prefix按got+1==child_beats，均无需假设E偶数。ClosureAtBurstEnd可令J=E为奇数；phase1的V quantum裁到J（T:718–723），且start仍须J>=旧promise（S:115），所以不会将已held的q2 W债截断。

F255时，A native第一页parent仅到job-relative255，第二parent只有1拍。其parent B间隙Q保留真实已消费端点（Q:37–44），新的parent捕获不会带旧B债（Q:55、130–146）。phase2 resident与clipE意味着第一页最后短W有data；第二parent首AW仍在真实旧parent B后捕获，grant来自future WRITE recipe（T:1035–1037）。如果E=255，两route都完成到255后suffix恰1拍。phase3不要求不存在的future AW coverage才能返回该末拍（S:145），可利用既有private停车；T:725–734仍保留按peer真实消费的suffix W准入。这里没有把源RLAST等价于第一页B或job完成。

MaxOutstanding=2时，只增加已握手AW未回B的数量；一个flow仍仅有一个当前payload child（Q:96、114–123）。AW/B同拍净计数仍一个case（:133–138）；最后original B只在所有raw W及所有child B完成后产生（:130–143），新parent AW等旧parent joined B真正被上游接受（:55、145–146）。parallel R route不改这些门。B先于另一路READ完成也不清source parent ID/E，故不会使oldV或A prefix orphan。

## 6. E256是明确的普通双独立读边界

初版design曾写E256拒绝handover；实际start_cutover只要求J<256而不要求E<256（S:114），故本席指出了文源不符。root在实验前修订design为E256普通并行独立读控制，本席已重读修订文本，不要求改变实际source分支。

当E=256/J<256，A prefix为 `[FA,256)`，旧V physical tail也到256。两route的真实末拍都会生成各自original logical RLAST并设置done（S:163、387），phase2的新W裁到256并要求resident，故两边native全部W/B可继续排完。suffix builder要求E<256（:286），因此不发零长度或重复suffix；phase2不进入phase3并不阻止job完成，job_done核的是done==1和returned==256（:205–207）。

prefix builder在首次phase2 main空/skid空时就注册A，最早一个旧V末拍可以与它同拍capture。V自己的最后logical R及W/B完成更晚，所以V job_done导致cache失效不能在这之前取消还未形成的A prefix。A AR一旦registered/offered，后续cache有效位变化也不撤AR；静态grant合同保证继续消费已承诺事务。这个论证不扩展到主动invalidate或新job。

FA=0时physical source R共E+(256-FA)=512，shared suffix长度256-E=0。old_v_busy转移、overlap_ars或two_routes_seen仅说明route有重叠，不能据此标记有效shared payload/收益；E<256才有正长度共用suffix。本席没有核原始R计数或byte比较，实际R512/PASS仍由root的记录证明。

## 接受限度

本稿提供两route/single-skid重建的直接源码归纳和具体同拍检查，未发现当前合同内新的阻断反例；不提供生产通用AXI、不限job epoch、fault/revocation或cycle上界的安全/活性证明。S新增functional状态仅old_v_busy1bit，另有33bit观察状态（:77–79）；仍须计既有parent/cursors/E/grants与Q writer及实际buffer成本，未综合不能称总面积只增加1bit。

接受实验须保留 ParallelPrefixRepair=0 的实际回归，分开E<256共享suffix与E256普通控制，核真实A AR早于oldV physicalRLAST、正确RID/RLAST、sourceR及目标W拍数、全部bytes与held VALID。单靠root或模型间同意不满足这些证据；本席没有运行它们。
