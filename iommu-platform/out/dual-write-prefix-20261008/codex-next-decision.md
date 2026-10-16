# §43之后：准入前的排空负担，先判等价再实现

Codex root，2026-10-08。§42单序普通续行已达字节下界；§43双bank源证书＋demux使物理接受AW全局保序。旧“双W序倒置”枝到此停止。以下是下一判别规格，未实现/未build/未run，不算新算法。

## Codex具体提案及其危险之处

由stop之后被迫继续转移数据，改为**在第一次不可撤AW offer之前**决定能否接纳。设释放设备i当前已承诺W的必要后续工作为C_i：需要完成直到它最后一笔承诺W的真实队列前驱，保留已offer/held source、扣除已返回或已交的数据，并区分R、W、必要翻译工作。预算H应约束这项后续负担，而非仅已传字节或自家AW数量。首次内部owner push/锁选择本身也是承诺，不能等外部AWREADY才记账或撤掉已VALID请求。

这句话**不等于独立创新**。普通global pending-byte cap、每owner记录last committed queue position、短burst、相同容量完整child预约都可实现大部分甚至全部。§43已排除顺序冲突，不能再把普通FIFO前驱求和包装为新图算法；byte bound也不自动变成时间bound。

## 一对必须首先区分的状态（仅纸面规格）

同A自家2KiB义务、同总6KiB未写、相同4KiB V义务：

- `[A 2KiB, V 4KiB]`，排空A不需写V后缀；
- `[V 4KiB, A 2KiB]`，排空A必须先交V前驱。

这只说明per-owner outstanding统计不足，不是新发现；普通记录队列位置立即区分二者。两状态在真实first-offer门/两bank/source证书下的可达性必须验证，不能当已完成DUT结果。全局6KiB cap不区分它们，但较强普通prefix记账必须纳入，不能只击败弱cap。

## 下一唯一门槛

1. 定向核AXI-REALM官方冻结代码的预算扣费、NumPending、isolation/held语义、完整W缓存与可选outstanding throttle；补主源边界。已有预算/分片/进度保证不得再列为新意。
2. 将候选与**同信息、同payload/metadata成本、同释放合同**的普通owner-prefix记账＋源字节/响应预约逐字段比较。源/目的页边界、准备cache、翻译回退都已是现有原型信息；不能将只有候选可见的oracle赐给候选。
3. 只有能指出一个真实翻译相关义务，使普通准确prefix方法无法同时满足约定释放界和效率/容量，而候选有合法可执行差异，才进入下一最小DUT。这需写出完整可达事件及前作差异，不能用有限永久关预算、异常丢W、新加TL锁或不允许合法普通续行求胜。
4. 如果所需状态仅是pending bytes、last-owner位置、已返回R及常规translation-ready，就归约普通准入，停止该规则的新机制路线；没有必要先编一套阈值扫描证明普通规则能工作。若有前作未覆盖的资源/延迟锐界，分析贡献可另外审定；搜索没有找到不能算证据。

当前贡献判定：尚无通过这一步的新机制；因为已有普通方法达到已验证现场的排空下界，就不能继续把它们换名立新。下一最小设计的实际执行尚未发生。

## 本轮补充判断

Codex已把pure byte-debt准入与普通owner-prefix记账作构造比较，见[codex-admission-equivalence.md](codex-admission-equivalence.md)：这层规则归为强普通对照，不作为下一待实现新机制。AXI-REALM冻结写缓冲的完整W-before-AW门也由根直接确认。下一继续仅找翻译相关的真实额外约束/锐界；没有找到就不能称独立创新，当前不安排该阈值规则build/run。
