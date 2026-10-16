# 固定MQSim覆盖写未完整完成：有界源码定位

2026-10-15。仅查AMU/FTL等待与GC解除屏障路径，随后按root新增的具体问题核TSU批次接口；不运行、不修改模拟器、不扩case、不重读transport/论文。请求Sol/high；actual metadata本席不可得。

**当前失败根因未确认。找到一条源码明确的“user事务进GC屏障后可能被删除而未完成”路径，但未与缺失CID53动态绑定。** root提供的最终receive_slots=1也不证明stuck；它可以是已经分发的上一批保留指针。

## 输入与证据层级

- root报告：原/observer四case各运行一次，512正常512/512；2048为2047、4096为4095、8192为8190；两版结果XML逐字相同。以上是root执行结果，本席未独立执行或重新核日志，不冒充本席测量。
- root一次2048 GDB报告（入口`../offline/diagnostic-2048/debugger.log`）：engine返回时host map有CID53；Locked_LPA/MVPN、read/write behind-barrier、ArrivingMappingEntries、overfull-plane waiting均空；TSU receive_slots有1指针、dispatch空、opened=0。未dump该指针内容。0事件/0GCprotected/0PCIe并不消除host剩余1这一失败。
- 本席直接核实offline副本的AMU、FTL、GC_and_WL_Unit_Base与已存fixed-ref originals字节一致；AMU SHA256为`2a306c386f0f03766eb1754218c1002c193ef03f6844689b160d38ddf967bc7a`。本文件以下行号均指该固定源码。

## 第一候选：LPA屏障解除只调用AMU自己的mapping handler

完整条件调用链（`Address_Mapping_Unit_Page_Level.cpp`，简称AMU）：

1. AMU:480–488，正常用户事务在LPA已锁时进入`manage_user_transaction_facing_barrier`；AMU:1890–1897将READ/WRITE分别保存在`Read_transactions_behind_LPA_barrier` / `Write_transactions_behind_LPA_barrier`。
2. AMU:492–506仅提交`Physical_address_determined`事务。未翻译、进屏障的事务不会因此自动进入正常TSU提交链。
3. GC搬页写完成时，`GC_and_WL_Unit_Base.cpp:143–151`调用AMU `Remove_barrier_for_accessing_lpa`，随后移除该搬页活动。
4. AMU:1797–1803解除LPA锁；1806–1811和1815–1820遍历behind-barrier事务，调用**AMU自己的**`handle_transaction_serviced_signal_from_PHY`，然后直接delete事务并从等待表移除。
5. 可是AMU:1665–1670的该handler第一件事是检查`transaction->Source != MAPPING`就return。对USERIO/CACHE，这不是正常服务完成广播，也没有重新翻译/提交；调用返回后仍被上一步delete。

因此，**若缺失host request的某个USERIO/CACHE事务实际走过1→4，则该路径能解释“GC/屏障表清空、eventlist清空、host请求仍未完成”**。最终表空既不能证明该事务曾走此路径，也不能否定曾被删掉；必须有身份对应的历史事件。这里不是以GC0推断无状态，也不是以成功exit认定完整完成。

这也收窄此前证据卡对AMU:1805–1814的说明：代码注释声称借用GC数据完成等待请求，但当前实现对非MAPPING来源调用的是会直接return的局部handler。应将“直接完成”读作**源码注释中的意图/模拟假设**，不能把它写成已实现且正确的用户完成语义。原文与原始来源仍保留，本补查明确该条件性实现缺口。

## 其余等待表：已核唤醒，不把未发生的可能性当根因

| 表/路径 | 正常进入、唤醒、移除位置 | 当前2048证据所支持的范围 |
|---|---|---|
| `Waiting_unmapped_{read,program}_transactions` | query CMT未取得entry时AMU:562–573插入；mapping read handler AMU:1694–1728按LPA恢复翻译并提交TSU，或转入barrier/overfull表，随后erase旧waiting项；1734移除ArrivingMappingEntries | 本席没有root最终这两个表的完整身份账本；不能从Arriving空就认定所有user事务完成 |
| `Write_transactions_for_overfull_planes` | AMU:598–604因Stop_servicing_writes返回false；1909–1912保存；GC erase完成`GC_and_WL_Unit_Base.cpp:153–164`调用AMU:1915–1933重新分配、提交并erase，失败则停在队首 | root报告最终overfull waiting为空，不支持“当前事务仍卡在待分配表” |
| MVPN barrier | AMU:1573–1578/1630–1641标记；AMU:1824–1886解除时构造Source=MAPPING伪完成事务，局部handler在此来源上可进入，区别于user LPA屏障 | root报告MVPN锁/标记及Arriving最终为空；未有CID53经此路径丢失的证据 |
| CMT WAITING/evict | AMU:111–140 reserve/evict；1694仅在reservation仍WAITING时恢复用户事务 | 本席最初发现条件候选，但root指出当前2MB容量足以驻留512项，没有实际淘汰证据。本轮停止追查，不以它解释当前失败 |

FTL.cpp本身Start_simulation/Execute_simulator_event为空（891–897）；未发现它提供周期性扫描上述等待表的恢复机制。正常唤醒依赖具体mapping/GC完成回调，故缺少事件与host未完成可以共存；这不是确认哪一个回调在当前运行中缺失。

## TSU receive_slots=1：明确排除这一错误推断

补存`TSU_Base.h:34–60`显示批次协议：Prepare使`opened_scheduling_reqs++`；嵌套层数>1立即返回，最外层开始时才clear receive_slots；Submit只push_back。`TSU_OutofOrder.cpp:144–160`的Schedule递减opened，仍>0则返回；降到0才处理批次。

`TSU_OutofOrder.cpp:162–206`将receive_slots中的指针push到分类user/mapping/GC队列；208–225尝试向chip分发，**Schedule返回前不清receive_slots**。下一次最外层Prepare才清。因此最终opened=0、receive_slots=1可以只是最后一批已经消费的指针，不能算live pending队列，也不能要求其为空才算排空。事务若已完成释放，该保留指针甚至可能悬空，不能无归属地解引用它来拼根因。

GC的Prepare/Submit/Schedule成对调用见`GC_and_WL_Unit_Page_Level.cpp:155–188`与`GC_and_WL_Unit_Base.cpp:130–136`；AMU翻译期间可触发GC，但上面的嵌套计数本身就是批次合并设计。没有观察到计数不配对或数据被clear覆盖的事件证据，**停止泛化“GC重入弄丢TSU staging”的猜测**。

## 有界下一定位合同（本席未执行）

只对固定2048原输入追缺失CID53的完整身份账本：从host command/request到device UserIORequest，再到各子transaction，记录指针与代际/唯一ID、Source/Type/Stream/LPA，以及创建→进屏障/待映射/待分配→提交TSU→真实PHY完成或delete的归属链。这样区分“仍在表里”“已删除但未通知owner”“已完成而host关联错误”。重点核上面AMU屏障删除是否实际命中同一个owner；仅最终空表或相同缺失数量不足以证明。

不再扩case、调参、试修代码或为追正例跳过缺失请求。root若本轮不再运行，可接受的结论是：正常512输入完整；三组覆盖写输入存在原版与observer一致的不完整完成，因而它们不能作为allhostquiet/自然GC残留边界见证；来源级GC机制仍成立，当前可执行见证和具体根因尚未验收。
