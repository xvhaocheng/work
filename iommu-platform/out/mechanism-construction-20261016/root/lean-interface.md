# 从参考票据收窄到有条件的低成本接口

2026-10-16；root直接源码推导，非RTL实现、综合或时序测量。事件模型保持事前冻结的精确P1 marker，不改合同或重跑。来源读取范围与hash见[critical-reads.json](critical-reads.json)，bank/方向的独占补核见[资格说明](../source/independence-qualifier-note.md)。

## 参考版本 M-tag

实际正常S1 start产生一次带occurrence的意图，首PTE携带相应marker经过原DS仲裁及spill，到共享AR选择器取得顺序承诺后清票据。尚未首次offer的独立payload才可暂缓；原held、DS/R/W/B及owner排空不暂停。H到期释放新抑制，单次票据后保留non-DS逃逸和原F4。

这是模型已执行的关联语义。真实实现还需按原通路保存marker、在背压中保持、处理reset/epoch及故障。未执行这些协议分支不影响条件性能序列成立，但不能把它升级为生产可用模块。

## M-ID：哪些marker状态可能省去

以下正常单PTW切面有逐行依据：

1. PTW在IDLE且`init_ptw_i && !edge_trigger_q`才新启动（PTW:315–365）；start当拍默认ARVALID仍0（:275–283），下一MEM_ACCESS才给AR，接受后转PROC_PTE（:384–389）。
2. PROC_PTE须消费本笔R，才可能结束此walk或进入下一层MEM_ACCESS（:392–437,478–510）。所以正常S1、无隐式翻译、同reset域下，新walk开始之前不应有上一walk未返回的PTW PTE。这里不声称任意fault/reset重叠也成立。
3. 普通PTW local AR ID固定0（:275–280）；DS回程按ID0给PTW、其他ID给DDTW/PDTW/CQ等（DS:235–272）。所选mux对每个source做原ID prepend（axi_mux:135–181）。因此可使用**原有PTW完整身份**，不是把任意DS头认成P1。历史平台映射source3/local4bit给出数值48；本结论只依赖符号身份，接入前仍按实际实例核位宽/编号。

在上述条件及**意图到达早于该P1共享选择**时，pending置位后的下一笔匹配PTW身份AR就是本walk P1。可以不为PTE地址或整个DS管线新增marker存储，仅在门保存pending及必要的来源资格、计时/逃逸状态。这是删减参考实现的条件推导，不是已测面积节省。

必须区分两种迟到：L已首次offer时意图才到，只是失去本次收益，应保持L；P1已共享接受之后意图才到，若盲等“下一PTW ID”，会把P2误认P1。后者必须以传播顺序保证、显式确认或M-tag解决，不能靠timeout掩盖。

## 资格不能由busy位代替

TL锁定请求已有bank和方向，能在实际start处关联；这些字段当前没有免费送达共享选择器。新增通路须保留start与bank/is_store/occurrence的配对；也可只允许normal S1目的写而把部分资格做成入口条件，不能先把字段费用填零。

`other bank`仅能区分来源。模型S中的V源完整、L不生产V所需数据、V已持W owner，不能由bank不等自动证明。保守实现可以只在有正确独立性资格时启用，其余退回current-VALID；如果这种资格只能靠昂贵新证书或及时性不成立，本低成本版本就失去理由。模型与本次来源核查没有取得原生资格生成器或整个前缀。

## 与普通128beat的同口径成本问题

M-tag/M-ID均保持原payload burst、R返回组织及4个payload AR。普通分段必须得到相同原字段、现有legalizer/metadata复用机会，允许read-only128适配，而不是被迫重写DMA。它能以多1个AR的条件构造取得更好cold完成。

可直接比较的是事务数量、需要触及的通路与明确状态职责；不可直接比较的是净寄存器、面积、Fmax、真实每AR开销及CPU。这轮已把候选从宽泛“联合调度”收窄为一个具体接口成本竞争，而不是把未知成本再次扩成全面开发清单。
