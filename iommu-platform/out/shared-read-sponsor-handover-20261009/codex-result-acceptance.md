# §57 数据债闭合后的补读接力：根级结果

1次真实RTL构建＋1次C++接口增量，29次新执行：28完整copy PASS、1次require-actual正确拒绝实际未触发接力。每PASS均完整源/目标/未写区、两DMA logicalR/W各2048B、real issue/done各1；根另逐CSV数pw或pwlast各1拍/目标256拍及sourceAR声明sum(len+1)一致。源/可执行快照accepted-build02、参数/日志/CSV/SHA账本保留。无production/BIT/board/Linux/SD操作。

根设计：A下一WRITE实际成功后，触发同沿冻结V缺payload新W/新R；将V推进到固定偶数J并令已有payload全部被真实rawW取走，再真实sink旧AR尾段到原RID/RLAST。A修补FA..J仅A，J..256双返回原logicalID/LAST；A写prefix裁J、joint新W start<=peer实际W数据游标，避免两拍错位和全局W-head环。关键代理独立核实际source/transport，排除其构造的反例，但不是普遍形式证明。

F1/PTE0/RR1/无stress：关闭V/A5152/7472，整长R接力5907/5905，平均6312→5906低6.43%，V慢14.65%、A快20.97%，源R仍511/AR2。普通prefix也加入此新规则5946/5944，不能把其可集成候选当作published前作。共同R/W按成功页切分6154/6153、源256/AR2；独立4707/7524、源512/AR3。普通短K16＋接力5742/5740、源320/AR6（平均较共同切分低6.70%），比整长R接力更强；K64为5755/5753、R320/AR3。

F128/PTE0：整长R接力6016/6014 vs关闭5404/6573，平均略更差；短K16接力5948/5946、R320/AR13，K64为5975/5973、R320/AR4，共同切分6136/6127。F1/PTE1000：整长R接力6803/6801比关闭5152/7472平均更差；短K16为6732/6730，虽快于同PTE共同切分7154/7153，却仍不及关闭长R的平均6312。故不能无条件执行handover，也不能以A快宣称V/A综合全优。

F1 A先/PTE1000/B64背压接力15457/15454，实际J50，source511；尚需同条件强普通配对。F128/PTE1000actual0回退，与关闭CSV逐字同；关闭CSV也与§56匹配。run02同条件requireactual1拒绝，完整数据检查先过但不计PASS，避免空命中。

新增payload0，声明phase2＋vcut9＝11个功能状态位；oldend9只输出observer，计入源码新增20位但不当11位完整控制器面积。父表/两上下文READ/WRITE recipes/两DMA FIFO/TC buffers/AXI headers/ held observers均另计。无综合/板端/一般fault/失效/多job/第三方公平证据。

创新判断：源读共享、普通drop/repair、逐burst重试都有公开前作。具体候选是“真实W数据债闭合后，把受保护V剩余logical服务交给A修补流”的接口与进展规则；如果前作未覆盖这一组合约束、一般可达性及完整成本/整体性能成立，可以形成微架构贡献。因为当前只窄fixture且收益有反例，不能以这6.7%宣布新算法或论文分区。下一强普通用公开next-WR page lookahead/prefetch，核是否被消除；root继续亲自设计/原文核查。

DeepSeek起草driver接口与先前文献摘录挑战，Codex亲自设计/应用/构建/实际执行/原始计数验收；一席关键独立源码审核，无Claude。
