# 冷关联回收：请求、唤醒、确认与关闭协议裁决

2026-10-16 JST。**本轮完成一个源码绑定的条件协议：K满时合并回收请求；源allocation owner在positive-budget poll中显式取出private cache冷页；B撤图和CPU metadata退休分别完成，最后才返K。** 请求不等待，当前帧继续O0。首版每个源池／固定B配置使用有界mailbox、显式rearm；不含ring，不增加逐TX或逐alloc的独立use token。

协议可表达而未实现。新增miss/control锁、代际、唤醒、ACK及ctx寿命都要付费。**本轮没有找到相对同接口R/O3的独占策略或接口能力；保留的是普通page_pool整合的成本／部署假设，不是论文主线或实测净赢。** 一次源码合同闭合后，不继续用重复协议表替代贡献证据，也不因缺成本自动开发系统。

## 1. 问题、范围与原文

旧V1-N/K只在源页真正离池或B整体关闭时撤图。页已无在途frame、却留在源private cache且以后不再自然return时，冷B关联仍占K。上轮选择让实际source owner主动取页，避免外部扫描`pp_ref==1`和每alloc重新激活同步；本轮只解决新控制入口的发布、关闭和寿命。它是来源绑定的容量场景，未测发生频率；K满不等应用死锁，O0仍服务。

固定Linux v6.12 commit `adc218676eef25575469234709c2d87185ca223a`，mlx5e cyclic／linear native-XDP、order0、单buffer、非XSK，固定B/config。T允许专用池实际映射范围持续授权，仍是条件部署；不向S细粒度隔离分支偷渡该许可。设备domain、共享IOMMU、模式和后端在实际机器上均未取得。

本轮只补取同commit的两个官方文件：`include/linux/netdevice.h`与`Documentation/networking/napi.rst`；其他源码复用已保存字节。[NAPI原文与核查](../napi-source/findings.md)、[关闭与寿命核查](../close-protocol/findings.md)、[来源清单](source-manifest.json)可定位。下文D=`net/core/dev.c`，M=`mlx5/.../en_main.c`，P=`en_txrx.c`，PP=`net/core/page_pool.c`，H/N为上述两个新文件。路径、hash和行界核对仅证明来源定位，不证明协议正确性。

## 2. 五项改变设计的源码事实

| 已核事实 | 设计含义 |
| --- | --- |
| H:503–523：`napi_schedule`返回bool；D:6178–6197：已SCHED时设MISSED，DISABLE时不接纳 | bool不是owner ACK。driver的void包装器丢弃该bool；应用pending不能交给MISSED存储 |
| D:6264–6288：普通complete看到MISSED会再调度；H与D的enqueue不内联执行poll | enabled普通完成路径可覆盖hook末检查与complete的竞态；请求字段仍须先发布，且关闭不能在检查后抢先释放NAPI |
| D:6687–6728：disable占住SCHED/NPSVC；enable清这两位，不重放应用pending；N:103–113：schedule成功也不保证poll发生 | 已接受请求遇关闭必须明确终结／接管；不能指望enable替应用重新kick |
| N:109–113：disable等所有权释放，不等poll函数返回；P:214–255：complete之后仍有arm/DIM等尾段，最后才退出RCU | 新hook的cache／ctx访问必须在complete前结束。拆除attachment／释放资源需合适CPU同步，不凭disable返回推整个回调已退出 |
| M:2765–2776关闭NAPI/channel；PP:1115–1134允许pool延迟销毁；PP:684–685最后release后不可回读pool；PP:1005–1018最终disconnect后free | pool、NAPI/channel、ctx、B/config不是同一寿命。ctx须有明确source anchor和记录责任；ctx活着也不等pool被pin |

P:143–254为RCU读侧；budget0在P:165–166退出，不允许本回收hook。positive-budget、RXwork0仍可达原post之前的位置，不要求再收到一个包。普通空闲回收只需本页合法frame责任结束和实际owner隔离，不需要B全局DMA drain；真正B销毁则另需DMA停止证据。

## 3. 首版控制合同

### 3.1 状态及执行位置

每source pool／固定B配置预留一个ctx和固定mailbox，区分source代际、B/config代际、owner attachment代际及record槽复用代际。记录代际不以PFN或指针相等替代。mailbox至多一active及一pending合并批次，owner每次最多处理一页；不保存packet等待队列，K个记录连退休节点共占K预算。

短irq-safe控制锁L统一OPEN／epoch、pending、显式rearm和close。**拟议使用直接`napi_schedule`，不把含`local_bh_enable`的mlx5包装器直接塞进锁内。** 已读enqueue源码没有内联poll；其他CPU或线程可被唤醒并等待L。实际锁序／模式合法性仍须由实现验证，不把“源码未内联”等同完整并发证明。scan、unmap、等待grace、disable及join都在L外。

请求者先从仍有效的source/frame责任或受保护的登记表取得合法ctx引用。无frame控制者不能先读裸指针再补ref；请求路径不增加每TX独立binding use ref。target hit原有索引／DMA地址读取、同步及frame保护仍在，不称零热成本。

### 3.2 发布、服务与显式rearm

1. **发布。** 在L内核source/B及attachment仍OPEN、epoch正确；先存pending及ticket，再在同一临界区kick，后解锁。关闭也用L关门，故publisher不能通过OPEN检查后在NAPI释放才迟到schedule。已有同代pending／active／终结结果时只合并或返回原结果；当前帧继续O0，不等回收。
2. **取snapshot。** 源owner在positive-budget且complete前的合格位置，L内只取／清当前旧pending s并标active；L外检查实际private cache、独占取页及唯一closer。无候选／已有其他closer时skip并报告，不在NAPI等对方或CPU grace。
3. **执行及ACK。** 合法取页后先断新使用入口，完成同后端合法B unmap／同步。旧record读者若只读metadata、不再触页或提交DMA，可重发布页到源cache；否则保留私人页至相应责任结束。L内只发布s的结果及清active=s，不能盲清更新代pending。
4. **终结与rearm。** NO_ELIGIBLE终结本pressure代；重复miss合并／O0，不自增代际或无限自唤醒。首版仅在显式控制决定／已确认库存变化后REARM；ACK存储有界，旧结果被覆盖后报STALE_RESULT，不凭更大ack_seq猜成功。新代可在上一ACK之后、poll complete之前发布。没有自动cache-entry通知，也没有“以后有冷页必会自治回收”的保证；自动return rearm是另需分支、lookup及关闭协议费用的可选扩展。

### 3.3 四层完成不能折叠

| 结果 | 含义 | 不能推导 |
| --- | --- | --- |
| REQUEST_ACCEPTED | 合法pending已记录并合法kick／合并 | owner会及时运行，甚至一定运行 |
| OWNER_ACK(ticket,result) | snapshot得到一次有界尝试／具名终结结果 | 撤图成功或K已空 |
| UNMAP_DONE(record_epoch) | 唯一closer已按配对参数完成撤图／必要同步并撤去live入口 | metadata旧读者消失、全B DMA排空 |
| RETIRED(record_epoch) | 必要CPU读者、索引、配置引用及异步责任实际结束 | payload RAM减少 |

**只在RETIRED后返K。** 同步分支的ACK可以携带UNMAP_DONE，但仍须独立记录退休状态。hook处于RCU读侧，不能为马上返槽而等待覆盖自己的grace。退休通知只触ctx／记录及受保护配置，不沿过期pool查NAPI。

## 4. 正常完成竞态与关闭寿命

在**enabled、普通完成路径、实际获得合格positive-budget poll**的条件下：

- publish在owner检查之前：本poll可取请求，MISSED可能多带一次空poll。
- publish在hook检查之后、complete原子释放之前：prep设MISSED，complete再调度；新pending必须仍在。
- complete先释放SCHED：publisher取得SCHED并排下一poll。

这些是源码绑定的手工偏序，没有执行LKMM／状态模型。MISSED不是payload或ACK。busy-poll／NPSVC早退、netpoll、threaded、prefer-busy分支不能免费继承这个三分证明；运行模式未知时仅保留条件身份。不得以`napi_is_scheduled`或`napi_synchronize`优化掉应用握手。

**source关闭：** L内先关请求／rearm和新持久准入门；所有已通过gate的kick在拆除前完成。随后在合法控制上下文走驱动deactivate／disable及必要CPU同步，既有hook完成或交接已分离页；未服务ticket报SOURCE_CLOSING，不依赖未来poll。新hook不在complete后读cache／ctx；拆attachment前确认相关尾段／读者结束，不能只拿disable返回充证明，也不能在L或RCU读侧等待自身。最后清NAPI指针，DETACHED后旧retire／late frame只走ctx／record职责。重新enable后须新attachment epoch和显式rearm，不自动重放旧请求。

**source anchor：** ctx需由pool从attach到最终disconnect显式拥有anchor，且组合保留现有XDP disconnect。driver free_rq不是最终anchor终点，page_pool user_cnt也不是任意后台worker pin。记录／退休／控制查询各持必要ctx责任；anchor结束后不得再解引用pool，最后source release后更不得回读pool。该组合回调／登记表／引用实现是新增整合工作，当前内核没有自动提供。

**B控制关闭：** 只关持久组织时禁新持久准入／pressure，结束已有控制动作，仍可保持B合法O0、共享source A继续运行；LIVE关联不能在合法frame在途时强卸。**B/config真正销毁**还必须禁所有新DMA（含O0）、排CPU发布者、取得B实际DMA完成／合法stop，并保证config活到最后unmap。`get_device`、ctx ref、disable或CPU grace单独不证明domain配置可销毁。M中只读到core destroy转发，未取得下游FW停止保证；不扩大为本轮已核。多source pool共享B时需全B gate／登记，单pair锁不自动覆盖。

## 5. 一条正向预测及七个相邻失败

**P1（条件预测，非运行结果）：** K满，p实际处于source private cache且仍占B关联；q当前走O0。合法请求被owner取到，p被独占摘下，唯一unmap完成；按record-only读者合同重发布p；ACK携UNMAP_DONE而K尚不返。实际metadata退休结束，返一个槽，后续合格需求才可接纳。若此时显式新代在hook后／complete前到达，普通MISSED路径保留应用pending并安排下一次服务。全程不要求p再次自然return、source退出或B全局停机。**旧V1没有此动作；R/O3加同接口也通过，不构成独占预测。**

| 相邻失败 | 结果及修复边界 |
| --- | --- |
| 旧请求ACK后，新的rearm／pending到达，owner末尾盲写pending=0 | 即使MISSED再poll也丢应用工作；snapshot只清旧代，ACK不擦新代 |
| schedule后立即source disable，或disabled期间发请求再期待enable重放 | schedule成功不保服务；gate关门、终结旧ticket、新epoch显式rearm |
| disable返回就释放poll尾段仍用状态 | UAF；新访问结束于complete前，attachment拆除另有合适CPU完成条件 |
| ctx或pool尚活就沿旧pool.napi kick | channel可能已free；gate＋kick线性化，解绑后禁止NAPI，ctx不假扮pool pin |
| hook等自身grace／source读者等一个反等它的B closer | 自等待或互等；NAPI skip、控制面有序关闭、退休异步且责任保留 |
| unmap或ACK即返K | 旧metadata/callback误写复用槽；实际退休＋record代际后返槽 |
| 无候选却保pending无限repoll，或只关persistent gate就销毁B | 前者空转，后者O0仍可能DMA；NO_ELIGIBLE终结、显式rearm；B真实销毁另禁全部DMA及真实stop |

ring-only、所有关联仍在途、owner不服务、后端在实际上下文不合法、CPU退休不前进，都可不返槽；O0正常服务不被写成失败。没有wall-time保证。

## 6. 与具名普通方案的实质差别

| 具名方案 | 本轮新增／保留的差别 |
| --- | --- |
| stock mlx5e O0 | 本来就B逐次unmap、A留池。候选减少有机会复用页的重复建撤图，并可周转持久关联；不是B-only首次出现 |
| V1-N/K无idle eviction | 新增“已沉cache、无未来自然归还时”owner主动回收动作及完整控制协议；这是版本能力增量 |
| DMA_PMD RFC | 已有last-use idle＋分配器同锁隔离＋退休，安全原理不新。RFC整PMD／多domain退休不同于普通order0 pool的B关联单独轮转；基础页PTE／leaf机会损失也计 |
| R-OB4K-T-N＋本轮owner接口／普通O3 | 同信息、同mailbox／hooks／K和合法退出，可用同表示实现整个条件能力；本轮未找到必须比候选多付的原语。它是具名普通构造，不是已发表同名网络系统，也不等实测完整成本相同 |
| dma-buf slot适配 | 同享owner cut及K个复用对象。标准sleepable unmap需合法交接／保留页，私有DMA前端可选同位置；双方新增适配和控制成本计入，不强加逐packet export/token |

DMA_PMD原文定位及系列最终卸图替换已在[上轮比较](../../mapping-recycle-reclaim-20261016/root/decision-analysis.md)第5节核清；本轮不重复阅读全文。普通R的[整合分析](../../mapping-integration-delta-20261016/owner/integration-analysis.md)和[dma-buf适配](../../mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)保持可追溯。NAPI lost-wake规避和引用寿命也是普通工程原则，不因给出新状态名而成为论文新机制。

可核查的novelty论证是：前作已有持久映射、idle隔离及摊销策略；本工作具体保留普通page_pool／消费者，拟在有限K下用实际source owner周转冷B关联而不加独立逐TX use token。该整合或许能以可接受费用落地，但相同付费接口下R/O3可完全采用，**所以当前只保留实现／部署价值假设，停止独占策略、安全原则和控制协议算法的创新主张。** 若未来相同工作／服务要求下取得比stock／具名合理适配更低的完整CPU或不可回收内存，且来源和适用边界充分，才有相应贡献依据；不用先证明共享翻译吞吐干扰，也不能由缺成本自动推出有价值。

## 7. 资源归因与轮末决定

回收的是B关联库存，p仍属于源池S；没有因此减少payload RAM，也不新增复制池C。临时取页、B真正延后退出的P、退休Q及之后替代分配按实际因果记；同页按`Sπ∪Pπ∪Cπ∪Qπ`去重。独立计ctx／mailbox／K记录／退休节点、PTE、IOVA及CPU。新增miss合并／短锁、kick和额外poll、显式rearm、扫描／lookup、unmap同步、异步退休、关闭CPU同步、冷项误驱逐及重建、源供给与后续分配反馈都不是零。

CPU、物理内存、带宽、有效完成／延迟分别报告；不把CPU省额直接写成系统净收益。控制占时及页重发布顺序会改变后续allocation，不能冻结O0的PFN再用轨迹。所有原生量仍为null；本轮只有源码与手工条件偏序。

**裁决：MAP-OWNER-RECALL-PUBLISH-1完成并接受为条件控制协议；低成本整合假设保留，未证独立创新或净赢。** 失唤醒、关闭裸指针、自等待及过早返槽反例已有具体修复义务；源anchor组合、B/config跨pool寿命、真实reader集合、后端/模式及锁序仍需实现接受时验证，不能把文字协议当并发正确性证明。

**下一最小动作是贡献门槛判断，而非继续拆相同协议或默认开发：** 只有出现会改变判断的具体新差项／代表性运行输入，才评估一种明确实现的完整成本／部署能力，并与同接口R/O3／O0比较。输入不足时保持本条件设计，不重复已结束的数据搜寻、旧阈值账或协议文档。若新证据只支持同一普通整合，就如实形成普通部署／工程结果；若有可归属的成本或能力差异，再冻结一个有界验证合同。详见[next-minimal-contract.json](next-minimal-contract.json)。这不是等用户确认一般授权或CPU／内存价值，也不要求用户先提供保证痛点。

未启动driver／collector／RTL／ABI、可执行状态或性能模型、原生／旧实验或板测；B1/I/D/A5/A2、并发独立工程及MIG暂停不变，10月15—28窗口不重起，未选论文主线。
