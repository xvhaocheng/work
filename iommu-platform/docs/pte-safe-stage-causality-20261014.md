# 两安全队头PTE因果门：本固定前缀未准入（2026-10-14）

本轮完成了源码因果门、一次新的被动RTL前缀和独立原始事件配对。**当前固定正常前缀没有出现合法双头选择点，不能执行ABBA；PTE阶段差异未成立，最多四个新性能条件没有运行。** 这个结论不证明其他合法供给时序不可能出现双头，也不关闭F1全族。

## 本轮实际做了什么

root登记实际goal，无额外token预算，亲核合法pre-firstVALID选择、PTW和DS返回阶段，负责探针源门/设计/实际执行和最后裁决；两个新独占Sol/ultra/forknone席分别核自然双READ capture与AR准入/完整普通控制，AR席在同goal继续对新trace独立核验，未恢复旧goal席。

保留当前原生1PTW、IoTLB16、NativeN、normal DEFAULT、每bank4 outstanding/16 W FIFO、fullID、同ID、heldVALID、真实W/R/B/fault。新前缀只有两个真实iDMA各一个对齐4KiB、私有buffer512；CPU/atomics/batch/warmup/旧prepare/pool/header/反序/gate功能关闭。两个descriptor普通配置后，串行正常NEXT_ID先source0后source1，没有增加工作量、pressure或容量，没有按debug／hiddenDDR状态启动。

source给出局部充分条件：source0最后**bank-local** completion AW接受与source1第一AR进FIFO同拍，随后两个bank可同步选择READ。但这个条件不是已证明的原生完整启动时序，internal AW也不是physicalDDR AW。因而root只批准一次被动前缀合法性/阶段观察：37个637声明bit的只读输出视图，无新RTL寄存器或native NormalTL/AR选择修改。memory仍共享R/PTE、W/B独立、16 read queue、head准备31；仅将已批准DS返回优先窗口覆盖所有job及R/W/B/faultquiet，未启用overlap或tail pause改变服务规律。

新build3次失败（漏base DMA封装依赖、只读mux generate scope、Tb时钟成员引用），逐次输入修正，1次成功；日志/result/manifest全部保留。没有未变build重复、oldrun_scenario/full_batch/64批旧实验或validator重跑。成功新binary仅运行1次。

## 新原始结果

| 项目 | 原始观测 |
|---|---|
| preedge范围 | absolute62..4212，共4151行 |
| 双NormalTL VALID／fresh安全双READ | 0／0 |
| comp2与DS3共享AR co-VALID | 0 |
| DDR queue峰值 | 3，未观察16槽饱和 |
| PTE数量与绑定 | 12；准确DID10/11、当前owner IOVA/type、walk epoch、stage/level、PTE_PA及消费数据 |
| PTE首次VALID→内部DS接受 | 全部0拍 |
| 内部DS接受→DDR AR接受 | 全部56拍 |
| DDR接受→首次RVALID/握手 | 全部32拍 |
| DDR接受→PTW消费 | 全部88拍 |
| 同walk前驱consume→下一birth | 全部1拍 |
| DDR held R | 208拍，全部DDTW ID49，每笔len7各104；payload32/36与PTW48为0 |
| 每源真实事务/数据 | 2AR、2AW、512R、512W、2B；main逐字节核两源各4096B目的数据、0response error |
| 实际jobquiet／完整quiet | 共同t0后3190/4145拍；完整quiet边界4145拍 |

DONE轮询3196/4150拍、check_quiet返回4151拍是后续轮询/采样上界，不替代实际jobquiet。全部payload/PTE/旧DDTW AR与R按fullID/LAST配对，AW/WLAST/B按无WID的真实队头顺序闭合，最终无ownedDDR义务。

PTE8/9出生时确有source1 payload36在途，相对间隔仍相同。**单trace的相同间隔不是改变顺序的受控因果证明。** 当前所有时刻只有一个NormalTL头，原生单头必须立即处理；候选不能暂停等另一个头，也不能在merged首次VALID后重选。因此没有合法ABBA分叉，不追加启动相位扫描。

## 真正残余和普通对照

PTE必须由前一PTE消费后生成，经DS_IF spill、DS delayer、xbar及真实DDR AR准入。已入DDR queue后，在没有既有held/更早DS阻塞、真实DS返回空间足够的域，完整DS优先屏蔽payload剩余beat顺序对该PTE准备/首次返回的影响。名义50不是直接接口50；实际Idle/count/Ready偏序给52，空路径内部→DDR56，本新trace符合。head-only准备计数仍可能占服务机会，不能称全部ready请求work-conserving。

准入前可选RR位移源码界：CPUoff且已有共享AR接受前史，未锁点至多一个comp grant；含已有comp lock至多两个。不是背压下的绝对clock界，也不拿复位rr0的局部两次例作任意合法前史。一个完整冻结O-DS-AR只优先当前VALID DS、否则RR，固定4次DS公平逃逸、held原头/已spill条目/fullID和全部退休保持。它覆盖可选RR位移，不能撤回在先承诺/预约真实槽或预测future PTE；该合同未实现运行，8bit声明选择状态不是总delta或面积。前轮九个普通NormalTL规则全部保留；不能将不同场景事后最佳拼成在线对照，O-two-stage仅在固定独立域成立时使用。

实际compiled mixed vendor已核。demux参数8形成各源16个本地ID、3bit counter正常满7；mux参数8管write事务，另有DDR queue16。没有把它们当一个共享8读额度，也没有假设两sole4KiB自动撑满16项。

## 完整费用、限制和结束

fullsource/job/context/bytes关联、合法头metadata读取、PTE各级接受/firstVALID/held与实际R/W/B追踪、history/key/valid/overflow、预测shadow/算术/选择延迟、held/fair、反馈/clock/routing和CPU完整成本均保留。此次只读simulation视图的637声明bit含padding；没有新增RTL状态，不代表物理观测免费。F1数值状态/端口/选择时间未冻结，预测/面积/Fmax/CPU/应用/gain均null，没有独立完整收益接受。

raw没有payload WDATA/WSTRB、numeric functional NEXT/DONE、直接credit寄存器、DDTW消费端。functional ID在本次main内部精确匹配，PTE数据独立对tableword。有限normal copy的W字节由实际8192B目的逐字节核对、固定aligned native全mask源及512 orderedW beats支持；不称逐beat rawstrobe独立验证。208 DDTW self-backpressure不等于payloadPTE反馈。无一般错误注入/物理MIG/新板测，原生fault路线不改。

全部源码SHA/实际build依赖、源偏序、12笔原始PTE表、独立验证、字节和quiet、失败构建及执行账本从[本轮导航](../out/pte-safe-stage-causality-20261014/README.md)进入。[稳定裁决](../out/pte-safe-stage-causality-20261014/root-result-acceptance.json)只关闭本固定前缀作为安全双头因果正例；[后继合同](../out/pte-safe-stage-causality-20261014/next-goal-contract.json)要求新的明确合法输入/捕获事实及普通控制外完整差异，不自动重跑当前prefix、扫相位、恢复R2/R107/C112或改论文方向。

root及两新席实际gpt-6.1-sol/ultra/native有效475000，Codex-only，无DeepSeek/Claude/Astra或manual context override。五文档同步和405项新交付核验通过后，actualgoal已complete，工具用量以[原样完成回执](../out/pte-safe-stage-causality-20261014/root/goal-completion.json)为准。原立题窗口累计；论文未完成。

实际goal已complete；五文档同步及405项新交付核验全部通过后调用。工具最终用量554,922 tokens、52分00秒，无额外token预算；getter清空当前goal槽，完成状态以原样update_goal回执为准。论文未完成。
