# §56 firstVALID 写数据债与读成员退出：根级验收

两次 out-only RTL 构建成功，10次新执行：旧规则1次预期死锁，新规则9次真实完整 copy PASS。账本逐CSV独立核实际目标W各256拍与声明source AR总拍；每次驱动检查真实logicalR各2048B、目标完整内容/未写区、issue/done各1。没有production/BIT/Linux/板卡动作。

旧F1实测：A首WRITE页仅1拍，V先qW2在全局W队首；两路各接收R0后V等R1才能W-LAST，A的旧B等V，而旧退组又等A B。原始run01 watchdog失败、源码/可执行快照保留；这是有界原型的实测反例，不能倒推默认大FIFO生产死锁。

根新规则用原qwriter游标计算promised_end/next_end。实际logicalR进入真实iDMA FIFO覆盖全部已firstVALID W数据需求后，即可清A future-R成员；旧AW/W/B继续真实退休。退组期允许next_end<=returned的完整驻留数据短W，不许需要未来A R的新W。无新增payload或功能寄存器；组合端口/门及完整公共成本仍需综合。独立关键审查见critical-data-obligation-exit.md。

F1无延迟四方法 V/A cycles：mode1 5152/7786，强普通prefix/suffix mode0 5191/7825，共同R/W按权限切分mode2 6154/6153，独立mode3 4707/7838。候选和prefix都sourceR511，AR2 vs3；共同切分R256，实际Wchildren129/129，候选128/129。共同切分平均时间6153.5比候选6469更低，不能只用V更快宣称整体胜利。

F1 A先、PTE额外1000、B64/背压：mode1 13257/23630，mode0 13296/23669，共同切分15322/15331。同128拍边界延迟1000/实际交错：mode1 5404/6915，mode0 5425/6936。参数、实际交错计数、原始CSV及SHA见账本/各run。

贡献判断：因为共同切R/W也能消环，不能将“解除死锁”或这39/21 cycles直接命名独立性能算法。firstVALID数据债而非B退休作为读组成员退出合同，是具体接口候选；公开广播、逐burst重试与一般deadlock原理已有前作。DeepSeek文献挑战仅摘录核对，提出的任意“至少3项/刊会级别”不是验收标准。若一般化合同/可达反例与可信完整成本构成前作未覆盖的机制增量，可以作为架构贡献；当前只有窄单作业、静态映射、成功权限、小缓冲实证，不能据此宣布独立新颖性。

下一根级设计：在实际A下一WRITE准备好后，先闭合V旧W数据债，真实sink旧physical R尾段，再由A补读供应V剩余逻辑尾段。要同时排除held-R与旧W队首两层环，与普通共同切分/独立读及同规则普通prefix比较。DeepSeek起草夹具/证据，Codex设计、关键源核验、实际应用执行和最终判断；无Claude。
