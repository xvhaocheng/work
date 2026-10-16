# §49 Codex正向设计：可退出的共享读，与提交前写切分

2026-10-09。这是设计合同，不是DUT/板上结果。根Codex提出机制、构造反例并审定。DeepSeek负责接口证据及受限模型实现，两席Codex分别做关键设计质疑与独立候选。

## 研究方法调整

用户要求创新，复现仅用于发现具体缺陷并产生新规则。不能把“任意普通方案再加上本提案的全部模块也能做到”当作已有工作覆盖的证据。否决要指出已公开机制、准确适用合同或完整同资源具体对照。创新也不必满足所有普通可计算算法均无法模仿的不可实现性要求。

论文拟贡献是受限缓冲下共享读的独立调度能力及其实现/收益；广播、自动识别共同源、短burst、保存已返回偏移分别均不独占。

## 已有方法中的具体问题

两个独立copy任务V/A分别经过自己的IOMMU READ权限检查，源PA相同，输出区域不重叠。若一笔长物理R逐拍必须发给两个DMA，而两个目的长W在同一有序W-owner队列中，非队首DMA只有B拍private payload parking，则它收满B拍后阻塞广播；队首W如果长于B且需要后续R才能WLAST，整个流可停止。这是拟复现的抽象路径，不是当前原型已发现的生产死锁：§43没有双consumer fanout。

普通固定短W+闭合广播可绕开上述容量循环。但当A要求暂停且禁止下一笔A AW首次offer时，共同长R的固定接收mask仍把V绑在A身上。直接删A mask又可能撤回已经VALID未READY的A R，或丢失A已承诺W所需payload。

## 新机制的具体动作

1. 仅为已登记、8B对齐、INCR、不可变共享输入的两个copy，在每个参与者分别成功通过READ检查且physical byte range相等后组成共享读；目的WRITE分别检查，不沿用另一参与者权限。MMIO、atomic、exclusive、错误、MRIF、动态失效不进入首版合同。首次静态原型不宣称支持live invalidation。
2. 共享期间，在各DMA上游virtual AW首次offer之前将目的W限定为q拍，q<=每非队首DMA的可用private容量；首个探针q=2、每flow B=3拍（不是生产默认）。R保持一笔N拍原物理burst，不以q重新发N/q笔AR。
3. 每流只保留一个尚未退休的短W组，提交下一组前旧组W/B已排空；AW一旦首次VALID，即锁定header并计入该流write end。此合同使暂停时该流剩余W payload义务<=q拍。实际平台更深AW窗口以后要计完整债务，不能偷换这个界。
4. 一拍shared R有固定required mask、分别delivered mask与stable data。A暂停后继续满足所有已first-offer AW、已有R接受/firstVALID的义务；绝不撤回当前A RVALID。后续拍才允许不把A列入required mask，原物理AR/ID/LEN/RLAST不变，V继续接收。
5. A保留原logical AR parent及实际returned cursor。它没有完成job，也没有丢掉原AR的剩余响应义务。恢复后从实际未返回位置重新发physical child，按原parent重建RLAST；已缓存的A payload先正常消耗。对不可变输入，跳过部分以一次后续补读恢复，不能对一般可变输入擅自应用。
6. A退出后，V下一笔尚未offer的W可恢复长burst；A恢复时也可独立用长W/补读，不必永久付短W开销。已offer的W一律不合并、不改变。

## 暂停边界与正确性

一般安全退出end要覆盖first-offer W end、A已接受R end、A已firstVALID未handshake的R end。更简洁首版可把A RVALID限制在它已offer W覆盖内，禁止为尚未commit的A W预送payload；则held A R也是已知W债务的一部分。只在当前beat全部required sinks完成后清下一beat成员资格。

共享物理R口仍要给V走完，不能把A不再接收称整条R口quiet。A rawparent可悬挂，定义是A不再制造物理请求/写债、V不必等待A恢复。如果实际验收要求所有已接受logical AR均退休才算暂停，就不能满足这个首版接口，必须改合同或增加完整停车存储。

每个sink的returned只在该sink handshake递增；shared R beat只有required mask全部delivered后释放，防重复、漏发、退出瞬间的失稳。A补读从returned而非W accepted计数起，避免丢失private已缓存的尾拍。

关键入skid条件：给该拍冻结required mask前，所有仍active且被选入的成员必须已有覆盖该拍的first-offer W；不能先冻结A=required、却等未来AW才允许A RVALID，否则随后pause禁止新AW会产生人为等待环。可以在外部RVALID保持时等待W offer；还未接收入skid时允许按pause选择下一mask。A暂停后新增返R<=q拍、剩余W<=q拍，总payload transfer work<=2q拍；不是总R+W<=q，也不是周期界。

若A提前恢复而原V物理R还未结束，不能把A直接重join到较后的offset丢掉中间缺口。首版等待原物理R退休再从A cursor补读。这是resume延迟代价；测试应包含早恢复与晚恢复，不只选V已完成后恢复的有利场景。

无暂停、同段长度N：理想physical source payload N而非2N，写payload仍2N；这不等于速度翻倍。A在D拍处退出并后续补读：source payload=N+(N-D)=2N-D。短W头数量/成功译址recipe/private state/skid、pause/debt及B控制均计费。

## 最强前作与真实增量

- AXI multicast XBAR 2025：已有有限W缓冲死锁和all-target acquire/aw.commit解决。该机制不可被重新命名；它服务多slave写多播，需核本合同的独立partial logicalR退出。
- Torrent 2025：cfg→Grant→Data→Finish，endpoint链式复制及无暂存duplication均已有。不能用read-once或不改AXI声称新。
- DMA-Latte当前v3（2026-09-14）：已支持single source/two dest broadcast，并在runtime从hipMemcpyBatchAsync共同src/size透明识别广播。不能用独立copy自动组队声称新；其bcst/b2b/pcpy尺寸取舍也必须纳入成本对照。

本提案目前可具体检验的增量：同一物理长R中，成员按各自W/R不可撤义务退出，V无需等A恢复，A的原AR通过cursor补读独立完成，且在offer前调整W粒度以适应private容量。必须再查直接的preemptible multicast/partial-response replay前作，不能靠上述论文没写来证明全球空白。

## 最小模型合同与验收

模型只验证上述握手/容量/独立退出性质，不能产生CVA6/IOMMU完整copy周期或面积。无需新BIT。

- 输入序列source beat具有确定非重复payload；N>=8（包含非q整除尾段），B=3，q=2；可扫描小N/q/B与暂停点。
- 模型显式存private FIFO、共享single-beat skid、write-owner queue、AW首次offer/held header、每sink parent-returned cursor、physical child界及原parent RLAST、Wlast/B。
- 三强对照：独立长copy；固定短W闭合广播（无暂停应PASS）；允许每短tile单独物理AR的普通可退出广播（同private存储及firstVALID约束，允许独立暂停/恢复）。另外naive长W lockstep用于复现缺陷，不作为唯一性能对照。
- 候选保持N拍长R并使A退出后V恢复长W；普通tile基线按q拍R完成以后退出，安全性相同、physical AR次数可能不同。当前不得把AR数差当cycle优势；更强ordinary长期R+动态mask+cursor若是明确既有机制，必须纳入。
- 验收数据完全一致、每sink beat不漏不重、原parent exactly1 RLAST、AW/WLAST/B等数、firstVALID后的header/data不变、容量<=B/每flow+1共享skid，暂停之后A新增AW只有已held那笔能完成；V在A未恢复时完成，A恢复后完成。
- 负例须是模型运行到结构停滞，并列wait-for状态；不是写死某baseline一定失败。阶段结果数只从执行JSON读，不把脚本设计当运行。
- 暂停发生在held A R或held AW时，要完成旧offer、控制后续offer。固定mask闭合广播暂停时预期停滞；普通tile应安全退出，但physical读AR多。

## 源码核查与修正

`axi_mux.sv:301–308`的w_fifo_push发生在首次选中AW可见时，即使mst_aw_ready=0也先push，不是必须握手才push；`353–365`队首路由且WLAST握手pop。TC的w_cnt只是AW/WLAST配平门，并不单独代表跨DMA owner。

`idma_buffer.sv:32–49`每byte-lane各Depth项；64bit时B=Depth*8B。§43 buildargv选择DmaBufferDepth=3，即24B/flow，48B是两flow总量；生产默认Depth512即4KiB/flow，不能混用。

`idma_reg64_frontend.sv:367–373`当前只提供同R/W的一拍deburst，src/dst_max_llen都0；legalizer虽然有独立旋钮，现register接口不能直接独立设W=16B而保持R=2KiB。首版需要bounded上游adapter/新内部输入，不能说Linux直接改现寄存器就行。

§49未改变生产/旧out DUT；DeepSeek接口稿中关于AW握手才入owner及一律需要新增B聚合的说法不全：上游legalizer生成短AW时backend原B计数可自然工作，下游拆长AW才需聚合。相关缺失证据由Codex直接核原源。

## 既有部件的复用

根直接读`vendor/common_cells/src/stream_fork_dynamic.sv:16–23,48–91`：已有per-input动态mask、各selected sink各handshake一次、输入等全部selected sink完成且无额外data port；复用它与既有parent/returned逻辑，不把dynamic fanout原语当新。控制器须给该mask stream稳定、覆盖义务的输入，选择退出点并安排原parent补读。新增可能在控制连接/生命周期与W shaping，而不是重新发明fork。

## 后续out接口的必要限制

长physical R开始前，分别保存源READ及每目的WRITE的成功recipe，所选L限制在所有已授权recipe共同覆盖内（还受两个原parent剩余、4KiB边界及256beat限制）。不能等长R背压后再临时walk下一短W，或要求跨bank同时physicalAW接受。首个aligned 2KiB量子只需每流一个目的页；沿用旧dst3072 fixture会跨目的页，必须保存第二recipe或缩L，在结果账本计真实额外AR/TL，而非说仍恒一笔。

动态退出后的W扩大要在legalizer每次生成新W请求、其首次offer之前读取新runtime长度限制；现opt_tf_q仅随job装载，不能在已hold的AW上改dst_max_llen。上游合法化让backend自然生成各short WLAST/B并独立R父响应，无需下游假拆原AW。新增WRITE recipe、runtime限长口及context/epoch信息必须计完整成本，ordinary获同样接口。

实际backend还有深8的W metadata FIFO（backend590–608），w_ready同时受AW/metadata/last队列ready约束（471–472）。首版‘每flow一组’要连到最早W metadata接受/排空点，不能只在外部AW口限一组而忽略已入队的若干短W；已生成的metadata不能因退组改len。模型把该提交简化成AW firstoffer事件，实际集成要明确握手与credit，未证明当前RTL等于模型。若选择保留多组窗口，就计全部窗口的pause-debt及退出后仍待发送的短headers，不继续声称q单组界。
