# MAP-OWNER-RECALL-PUBLISH-1：NAPI真实API合同

2026-10-16。独占范围为NAPI schedule/completion/disable与poll返回的语义，交root用于请求/owner控制；不设计pool/B资源协议。Codex-only，沿用已定ARS来源核查。请求Sol/high；实际独立model/effort元数据unknown，未声称原位切换。

**结论：发布应用pending后调用有效NAPI schedule，在存活、enabled、普通completion路径中能封住“正在完成”的调度竞争；NAPI既不保存请求身份，也不确认owner已执行。disable/enable、busy-poll/netpoll及应用自己清pending仍需单独责任。`napi_disable()`只等NAPI所有权释放，不等poll函数最终返回，更不证明B卸图、K返还、CPU退休或DMA排空。**

## 来源及证据等级

全部固定官方Linux v6.12 commit `adc218676eef25575469234709c2d87185ca223a`。`D`=`mapping-admission-20261016/v612-source/raw/net/core/dev.c`，`M`=同raw根下`drivers/net/ethernet/mellanox/mlx5/core/en_main.c`，`P`=同目录`en_txrx.c`。它们复用已存字节。`H`=本目录`raw/include/linux/netdevice.h`，`N`=本目录`raw/Documentation/networking/napi.rst`，本次仅补取这两份，均HTTP200；原文、header收据与hash已保存，详见[source-manifest.json](source-manifest.json)及[HTTP收据](http-receipts.json)。

这是直接源码与官方API文档证据，不是运行结果。没有新原生/模型/测试/采集/driver/RTL/板测，也没有新论文或旧数据入口搜索。源码注释`outstanding processing completes`的简写不能覆盖官方文档更精确的ownership边界；以下以实际状态转移交叉核对。

## 真实调用链、锁与状态

| 路径 | 原文位置 | 可用保证与边界 |
|---|---|---|
| `mlx5e_trigger_napi_sched(napi)` | M:2646–2650 | `local_bh_disable()`→`napi_schedule()`→`local_bh_enable()`；返回void，丢弃schedule的bool。BH关闭不是跨CPU应用请求锁，也不保证调用时NAPI/pool仍活 |
| `napi_schedule(n)` | H:503–523 | prep为true才`__napi_schedule()`并返回true，否则false。false有“已SCHED并记MISSED”或“DISABLE拒绝”等原因，不能统一当请求失败或owner完成 |
| `napi_schedule_prep(n)` | D:6178–6197 | 从READ_ONCE(state)开始，以`try_cmpxchg`重试；见DISABLE立即false；否则置SCHED，旧SCHED已置则同时置MISSED并false。成功从SCHED=0取得所有权才true |
| `__napi_schedule`→`____napi_schedule` | D:6159–6165，4510–4543 | `local_irq_save/restore`保护本CPU调度入口；后者要求IRQ关。普通模式添本CPUpoll_list并按需raise NET_RX_SOFTIRQ；threaded模式设SCHED_THREADED并wake线程。不是任意caller可直接操作poll_list；H:349–354将list责任绑定到原子SCHED所有权 |
| `napi_complete_done(n,work)` | D:6220–6288；H:537–551 | NPSVC/IN_BUSY_POLL时直接false；正常先清理poll_list等，再CAS清SCHED/MISSED/THREADED相关位，旧MISSED则保SCHED并`__napi_schedule()`，返回false。无MISSED才释放所有权；false还可能来自IRQ defer timeout，不能视为错误或owner请求处理状态 |
| `napi_disable(n)` | D:6687–6707 | 可睡；设DISABLE，等待SCHED/NPSVC都清，再CAS自己占SCHED/NPSVC、清THREADED/PREFER_BUSY_POLL，cancel timer，最后清DISABLE。返回时以保留SCHED/NPSVC阻止新的正常调度。它没有等待poll尾段退出的步骤 |
| `napi_enable(n)` | D:6718–6728 | 要求当前SCHED置位；CAS清SCHED/NPSVC，按配置设置THREADED。没有检查应用pending、调用poll或kick。控制API须配对且非幂等，N:44–52 |
| 正常poll/repoll | D:6765–6855 | 调`n->poll`后才读其返回work；work<weight立即返回，不能据此重新证明driver已服务自定义请求；耗尽weight通常repoll，disable pending会complete，prefer-busy有特殊分支。`napi_is_scheduled`只best-effort，H:480–500，不是应用idle/ack证书 |

上述CAS给NAPI状态转移明确的竞争顺序。**这些源码没有应用pending的读写、锁配对或请求序号**；不能由NAPI的READ_ONCE、IRQ/BH屏蔽或state CAS自行宣布应用payload的无竞争出版、消费、清除和寿命已经正确。root需要明确应用侧共同锁或已证明的原子/发布读取配对。这里不额外下载通用atomic文档，不扩展为LKMM验证，不声称已验证某套release/acquire实现。

root拟用短irq-safe gate包住publish+wake，owner在complete前消费且不等待grace；这是明确付费的应用同步前提。已核`__napi_schedule/____napi_schedule`函数体只enqueue/raise-softirq或wake线程，不inline调用`n->poll`。**这不自动证明整个mlx5e helper的`local_bh_enable()`返回前绝无BH执行**；其与应用gate的实际IRQ/BH嵌套需按所选调用形式另核，本席未读softirq实现，不以enqueue函数体代言helper全部执行环境。并发线程/其他CPU实际获得调度仍可访问同一应用gate。

## 为什么普通completion竞争可以封住

条件：NAPI对象/配置仍活，enabled且未进入关闭；应用请求已经以正确同步发布；owner在每次合格positive-budget poll的固定hook读取/消费pending；对pending的消费不会覆盖更新请求；调度系统实际获得执行机会。只证明API偏序，不保证时限。

1. **请求在owner最后检查之后、complete CAS之前到达**：owner检查为空→发布R→schedule prep看到SCHED=1，以CAS置MISSED并false→complete CAS看见MISSED，保SCHED、清MISSED，重新排poll→下次合格hook消费R。即使prep与complete CAS同时竞争，也会有一方重试，不能两方都按过时state成功。
2. **complete CAS先释放SCHED**：owner检查为空→complete CAS清SCHED→发布R→schedule prep从SCHED=0取得所有权→`__napi_schedule`排poll→合格hook消费R。旧poll函数可以仍在尾段；因此自定义cache owner动作及对其需要的状态访问必须在释放ownership之前完成。
3. **请求先于本轮owner检查**：发布R→schedule因已有SCHED记MISSED→本轮hook消费R；complete可能多排一次poll。多排一次是合并调度结果，不是重复执行R的义务；应用自己确定请求合并/ack。

MISSED只是一位“还应再poll”的合并提示，没有请求ID、source/B epoch、数量、页指针或容量释放状态。多次请求可以合并成一个MISSED，owner仍必须从应用pending取得真实工作；不能把MISSED直接当pressure队列。

## disable/enable与真正poll返回

N:103–113明确：poll可能没有driver显式schedule也被调用；schedule成功也不保证poll被调用（例如随后disable）；disable及后续poll只等待ownership释放，不等poll方法退出。D:6687–6707与complete的CAS支持这个边界。

mlx5e的具体证据是P:214调用complete，成功后继续P:217–249的统计、DIM与CQ arm，P:253才`rcu_read_unlock()`，P:255才return。因此存在合法序列：`poll complete释放ownership → disable取得ownership并返回 → 旧poll仍在尾段 → unlock/return`。不能以disable返回作为自定义请求对象、ctx、binding或driver尾段字段立即free的普遍证书。已有driver的真实关闭同步可另行覆盖，不能由本席仅靠disable函数代言。

另一个容易漏的序列：

- `disable开始，DISABLE=1 → 请求R已发布 → prep直接false`，无新排队；R的存续/取消/关闭接管由应用负责。
- `disable已返回，SCHED=NPSVC=1且DISABLE=0 → 发布R → prep只置MISSED并false → enable清SCHED/NPSVC`。enable不排poll，故即使R仍在应用pending，也可能没有任何后继触发。**pending-before-wake必要但不足以覆盖禁用阶段；重启/关闭协议须接管、取消或在合法重新enable后重kick。** 不能看到DISABLE位已清就称NAPI enabled。
- `schedule成功 → 关闭赢得控制序列`也不能承诺R必执行；官方文档已否定无条件保证。调用trigger返回以及schedule bool均不是ACK。

控制API不可从本poll里同步等待自身ownership释放；不能在mlx5e poll的RCU读侧等待包含自身的RCU grace。P:141、253给出该读侧范围。`napi_synchronize`同样只是SCHED观察/等待，H:567–582，并不禁未来schedule，也不能升级为请求ack、函数尾段退出或DMA完成保证。

## busy-poll、budget及非普通分支

1. D:6231–6233在IN_BUSY_POLL/NPSVC下不执行正常complete转移。D:6400–6448的busy loop自行以CAS取得SCHED|IN_BUSY_POLL，并直接调用driver poll；它不依赖每次driver trigger。只按softirq“排一次即一轮”的推理不能覆盖这些分支。
2. D:6360–6387的busy_poll_stop先清MISSED/IN_BUSY_POLL，再调用一次driver poll；源码理由是这次再poll可以处理已错过工作。故不能把MISSED清除理解成R已消费。若wake与清除相遇，应用pending必须继续保存工作，且具体合格hook应在该poll真实检查；NAPI不会代其检查。
3. D:6328–6344在busy-stop特殊`skip_schedule`分支可以清SCHED而不普通排队；timeout、prefer-busy、netpoll和threaded路径不作为本轮普通偏序的无条件服务时限保证。源码已核其保守边界，未核实际部署启用了哪些config/sysctl。
4. N:68–98规定budget0不能用page_pool/XDP API，不能调用complete_done。mlx5e P:164–166确实直接跳out；自定义owner回收hook不能在那里假定服务。positive预算时即使RX work=0，P:176–195仍到正常post位置；这是候选hook可投影位置，**源码目前没有该hook/pending消费实现**。
5. mlx5e hook必须在complete之前。N:109–113的ownership规则和D:6388–6390的“complete后另一CPU可能已经运行”注释共同否定“函数未return所以仍独占cache”的推理。源码模式可能是softirq、线程或busy caller；不能把owner身份冻结成某一个CPU编号。

## 最小相邻反例

| 反例偏序 | 失败原因 |
|---|---|
| `wake→owner hook读空并complete→发布pending R→无后继kick` | wake在publication之前，R可长期滞留 |
| `owner见pending R1→publisher发布R2并schedule记MISSED→owner普通store清pending→repoll读空` | 应用消费覆盖新请求；MISSED没有R2身份，补一轮poll也救不回payload |
| `publisher测试napi_is_scheduled=true所以省略schedule→owner刚检查完pending再complete→publisher发布R→无后继kick` | best-effort SCHED观察没有MISSED握手；不能优化成只在看似idle时kick |
| `disable已返回→pending R→trigger false/MISSED→enable→无再次kick` | enable不消费pending也不排poll，disabled期间的请求不能依赖自动恢复 |
| `poll complete→disable返回→控制面free新增ctx→旧poll尾段访问ctx` | ownership释放早于函数return；还可能新poll已开始，不能把尾段当独占段 |
| `poll中等自己NAPI disable/等包含自己的RCU grace` | 自等待；不能用关门同步修补当前读者寿命 |

## root可用和不可用的保证

可用：保活enabled NAPI、先正确发布应用pending、每个请求都经过真实schedule握手、合格hook在ownership释放前读取，配上应用侧不覆盖新请求的消费协议，可以利用SCHED/MISSED与complete CAS处理普通完成竞争。owner可明确报告“已处理”或“本轮无合格页”，但报告的含义由root协议定义。

不可用：trigger返回/`napi_schedule` true或false/观察MISSED/complete bool/disable返回/poll返回/budget耗尽，任何一项均不自动等价于该请求已处理、某B图unmap成功、硬件失效已完成、CPU metadata grace完成、K已返还或设备DMA drain。不能把“调用到了poll”替代自定义hook执行证据，不能从API中推出owner服务或slot返还的时限。

未核：具体pending发布/消费与ack实现、pool/B关闭接管与引用、root的metadata退休和预算返还条件、选定DMA backend及设备排空、实际运行config与owner延迟。本轮未改production/canonical，文档和Git同步由root末次统一合并。
