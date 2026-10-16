# §48 根级设计与源码判断

2026-10-08。root Codex亲自核latest§43 producer/helper、iDMA legalizer和三项primary；不是worker结论转述。本轮新模型另记执行账本，本文本身是源码/纸面推演。

## 1. 四种事件必须分开

- fragment parent accept: compact:223–236将AR存到parent_q并推进pa_q。
- helper offer/accept: helper:190–216置prepared_q；尚无译址结果。
- helper materialization: helper:218–230响应握手、seq/key/成功normal检查后置valid_q并保存83bit normal response。
- fragment parent data completion: compact:299–310收R、最后parent beat推进done_q。done不是TL frontier。

top:1014–1025由done和pa合成live ring，过滤为2KiB/full normal parents；cache_valid在helper:101–106又核当前generation/权限。没有全局job epoch或write-direction frontier输出。IOTLB本身没有这些job/window状态，跨模块反馈与context/epoch成本不能免费。

## 2. 给§47“可能有洞”一个准确范围

### 2.1 最新窄fixture

multi_test:1039–1049设置CONF1 decouple_rw，source page-aligned、length32KiB；legalizer:139–163在64bit下默认256beat即2KiB伪page，按241递增。source父为全长2KiB，top full-parent谓词可满足；pause eligibility在1017–1024为递增地址prefix。helper:139–150对每flow oldest-rank选择尚未valid且尚未prepared者；slow只有一笔。fragment只创建done队首的source child。因而在成功、无flush且始终enable/prepare的限定路径，源parent成功准备可以保持前缀。cached child的快返回本身不是提前materialize新parent，不能仅凭快慢并存宣称该fixture有洞。

这是条件性源码归纳，未运行新DUT；error/unsupported parent、动态模式与跨job不包含在此归纳。

### 2.2 一般正常对齐要求只到8B，合法短首parent给无fault洞

CONF1、src=3072、dst=0x10000（页内offset0且与源不重叠）、len4096；legalizer2KiB边界形成源parent：
seq0:[3072,4096)1KiB/len127；
seq1:[4096,6144)2KiB/len255；
seq2:[6144,7168)1KiB/len127。
compact接受条件223–233允许这些长度/8B对齐、不越4KiB。ParentDepth4且R metadata容量允许前两笔驻留。

取模块已有独立输入pause_reads_a=1、pause_a=0，阻止创建新的A source child，仍允许raw parents与背景准备。top:1018使seq0的helper parent_valid=0，seq1=1。helper选择并成功准备seq1后，其valid可为1而done仍0、seq0无source译址。这是合法模块接口的纸面握手序列，不是旧CLI已跑case。C++natural固定源对齐，不能用旧结果声称测试过此例。

若取最大成功start-page=1，错误宣告page0用途结束。**需要连续成功边界或保守窗口，而不是max地址。** 不用fault来推翻初始no-fault合同。

## 3. 最强普通能利用同一窗口，不能把8bit hole当candidate独占

对每flow从done依次扫描至pa，当前seq与helper cache_valid都匹配且parentextent有可靠证书时，连续成功前缀可以组合计算。遇首个不支持/未准备/未成功parent立即停；未发parent仍属于已登记job剩余用途。过去done之前已完成R，正常copy中译址用途必已消费，但此证据比materialization晚。

2×Depth4的cache_valid接口已有8线/状态。normal value、32bit seq和prepared也已在helper。source-only ordinary可复用这些字段，不必额外存一个hole bitmap；rank reduction、extent/epoch/context验证、IOTLB连接仍要计成本。可选保存游标只是在combinational fanout与寄存器/更新成本间取舍。unsupported短parent用保守done证据或另加成功extent证书，两方同计。write路径没有相同private prepared window，不由source的8bit推得两方向都免费。

这条接口/前沿实现属于普通窗口归并。若新反馈方法与它同信息同更新、无实际性能/成本差异，就不能把重新编码当独立算法。逐entry用途bitmap也是派生表示，不比job-range输入免费；pointer/ref方案必须计entry generation/invalidated pointer恢复和占用容量。

## 4. 接近前作限制

root直接读primary/pichai2014.raw.txt:881–943,1360–1378,1687–1735：
TA-CCWS按TLB miss加权，TCWS按TLB victim tags/LLS调warp集合，还在hit按LRU depth更新，CommonPageMatrix按warp-pair过去共用PTE重组线程。不能把“调producer促进translation reuse”原则立新。固定原稿§7.2/8.2，与其他摘要版§6.2分页不同。

root直接读primary/locality-descriptor2018.raw.txt:855–994：
已知shared ranges指导CTA clustering；no-reuse bypass、soft pin共享数据等其他线程访问，并按cache容量/active tiles限制prefetch distance。它不是IOMMU，也未在列明规则给精确live DMA materialization prefix；但known-range＋共享保留＋生产者分组的组合原则已明确。不能因领域/字段改成DMA就称新。

root直接读primary/orchestrated2023.raw.txt:627–685：
按instant TLB missrate和SM资源放置new TB，并动态partition/share；这项规则没有要求暂停两个已运行stream来约束相位差。它仍是具体强调度/容量对照，不能说全部前作只管cache而不管producer。

原作完整导航/URL/Hash由prior-critical-agent.md与primary manifest补全。不以全文未列某字段证明无人做过。

## 5. root进一步提出的可判定增量（只作下一机制合同）

不再靠“知道后续还有用途”本身立算法。可以进一步比较：**以译址结果真正保存的进度约束共享源页的超前距离，结合不可撤写义务的有限继续；不按R/B数据结束做全页barrier。**

要区分三个点：
- translation materialized：迟到flow已保存结果，即使payload尚未完成，此共享entry已无该flow的未来翻译义务；
- new offers：可延后尚未首次提出的新请求以限制超前工作集；
- committed/held work：不能撤已有VALID、AW/W/response obligation，必须继续必要source/TL来排空。

输入是registered同context共享source范围、两个可靠materialization前沿、当前held/committed W义务与真实cache/metadata容量；输出仅门控未offer的新工作/调整bounded background preparations。若只是固定page-pair barrier/lag-cap+普通pin/bypass，就是普通强对照。具体增量要体现在相同信息下允许数据继续重叠、较少重读或较低端到端成本；闭环可能由data bottleneck抵消，必须实际wholecopy验证。

本轮逻辑cache模型先判断是否存在非compulsory translation work、是否简单用途保留/private last-page已达下界；不证明上述调度满足最新W链或周期收益。下一实际接口证书和对照必须包含普通translate-first scheduler、hit-first/RR、page barrier、bounded lookahead/lag cap，不只击败LRU或慢data barrier。若这些具体强组合已同成本同效，就停止独立机制命名；否则才实现最小out对照，不启动BIT阈值扫描。
