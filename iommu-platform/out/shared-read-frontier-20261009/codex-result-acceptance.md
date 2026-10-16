# §51：普通可变分片与重复暂停的实际判别

2026-10-09，日本时间凌晨。Codex亲自设计接口、准入和根因修正，审读真实RTL/CSV并执行；DeepSeek起草参数、第二次暂停和观察器。out-only真实两iDMA/IOMMU及行为DDR，未改生产RTL/Linux，未编BIT/上板。

## 接受结果

两次SV构建成功，15次新DUT执行：14完整copy PASS（run01–08、10–15），run09保留失败。build02最终RTL6项（run10–15）全部执行两轮有限暂停；原build01八项no/one-pause成功只归对应旧版本，不混为最终版重测。run09虽最终两目标字节正确，却未满足第二次quiet，故整体失败；没有削弱断言。

新能力：始终保留每流唯一q2-W，B真实汇合，source quantum可1..256。共享两人可长读；退出后singleton用q读且整段已有first-offer W覆盖。pause只停止新W，已有W需要的R继续准入。run09原来用`!pause`挡住了已首VALID第8个A短AW所需的R；seal时end128/rbytes112，整个10000周期不推进，恢复后才完成。根亲自修为keep-q合同下允许这笔既有W的读，且保留整段coverage门，run10 second request13142→quiet13197（55cycles）通过。

四对CSV逐字节SHA相同：run01/05（无暂停）、06/07（一次10000暂停）、10/11（两次10000暂停）、13/15（两次1000暂停+B64/READY周期背压）。前者长读mode1，后者普通可变读mode2、quantum256，均同keep-q、recipe、cursor/skid和状态。**因为普通同信息同资源复现相同轨迹，这部分不能称独立新算法；§50相对q2的34%主要是分片粒度收益。**

| 场景 | 长读或普通q256：V/A cycles | 普通q2：V/A cycles | physical source AR（前/后） |
|---|---:|---:|---:|
| 无暂停 | 5657/5650 | 8579/8572 | 1/128 |
| 一次10000暂停 | 5161/19848 | 9928/19894 | 125/252 |
| 两次10000暂停 | 5161/29793 | 9928/29839 | 125/252 |
| 两次1000暂停+B64/背压 | 13277/28956 | 18033/33712 | 125/252 |

无暂停普通q4=7043/7036，q16=5987/5980，q256逐步到长读值；不是重复统计试验。暂停后保持q-W和q-singleton读，会增加真实header/setup成本，不能把§50退出后longW的数据混作同合同速度。无pause源R256，有pause504，所有目标各W256；根独立核14个CSV声明R与真实physical W退休计数，完整字节/未覆盖区域检查由DUT driver实际执行。

## 创新判别与下一步

已直接核一席独立边界证明：V继续消费时，A新增logical R/W≤2q的关键是不可撤W末端，不是长ARlen；最后成员物理尾R不可取消，不能把logical quiet当backend idle或统一cycle界。单页四recipe translate-first两者同给。具体证明在critical-obligation-boundary.md，纸面反例没有算成DUT次数。

另一路关键源发现：本平台dma_core_wrap采用NO_ERROR_HANDLING，TC的SLVERR/error slave不等于每job错误上报/可恢复abort。因此后续迟到/失败授权研究须区分成功时序与实际fault支持，不能拿一个fault布尔门宣称完成安全恢复。

下一先判断提前resume时退出/补读是否必要，以及相同资源的普通组成员策略能否取消不必要重读；同时定向核跨context许可前沿、源R返回交错和held VALID，寻找普通方法未覆盖的真实约束。研究继续到08:00 JST；本阶段收尾不是结束整晚目标。

全部原始argv/log/results、失败源码和SHA见execution-ledger.json。build01 executable被build02增量覆盖，未留原exeSHA，明确不补造；其原模块源码和构建/运行日志保留。生产、板卡结果为0。
