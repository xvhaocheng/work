# 最小成本的比较边界

这是源宽度/直接构造与参数式，不是完成RTL后的最小面积证明。[结构化账本](minimum-cost-comparison.json)。普通规则允许删掉未用的观察字段；不能强迫RR、owner与cheapDebt携带全包络元数据再宣称差距小。既有native队列/credit保留，新增读取、匹配和时序全部收费。

| 规则 | 至少需要的新增观察/状态 | 不需要被强塞的字段 | 选择与时序费用 |
|---|---|---|---|
| RR | 原native头/hold/RR；服务history新增0 | W/B包络/history | 原RR，不计假想新predictor |
| currentDS+RR | 当前DS真实VALID、native承诺，必要gate/hold接口 | 完整服务预测history | priority/gate/端口；实现后才能给周期 |
| earlyPause | P pending与new-gate held可直接2bit；实际DS接受/clear事件、身份匹配 | W/B服务history | pending判断/源匹配/held，不是2bit就全部免费 |
| owner | 实际input W owner或physical FIFO头、job/阻塞路径绑定，合法native复用 | 完整W/B延迟历史 | owner匹配/回退，关联状态参数化 |
| cheapDebt | P准备、已接受先行欠账、同端点R服务；计时/有效/epoch | W/B包络与全部future-AW记录 | 加/欠账比较；观测/历史更新时间计费 |
| dualGap | cheapDebt加owned release gap、return/fullwalk尾、类/引用有效性 | 不要求完整B预测 | 门限、端点对齐、timestamp与更新成本 |
| envelope | reserved顺序、各端点剩余R/W/B/数据、策略相关AW许可、独立B历史、完整max/sum、age/fair | 无权用未来真值/免费隐藏DDR | 前处理/算术/溢出/快照/持有/端口/更新/延迟/CDC完整计费 |

源码能固定的数量：job ordinal原33bit；current burst原8bit；FullBuffer原3bit；全局8/32、bank4/16；输出W queue原8个bank-index entry，每entry1bit。原字段的意义与寿命见[input map](observable-input-map.md)，不是全部可直接复用为prediction状态。

normal aligned DEFAULT4KiB/64bit是512beat，0..512需要10bit。可构造每bankR进度、raw W接受、physical输出W接受三组各10bit，两个bank共60bit；窄域reservation2bit/bank共4bit，normal-history坏资格2bit，pending/held2bit：**列出分项小计68bit**。这不是总新增量、不是已实现FF或全局下界；缺B/AW记账、不可复用关联、T宽时间戳/未完成记录、K类完整历史、有效/epoch/年龄公平/快照和选择pipeline。一般变burst的20bit reservation是4bit的替代，不重复相加。

完整新增状态仅可参数化：

`Snew = Sgate + Sassoc + Sprefix + Sprogress + Stimestamp(T,M) + Shistory(T,K) + Sepoch_valid + Sage_fair + Ssnapshot_select`。

T必须由真正时钟/有界观察窗/wrap规则定；K/M由真实service class与原合法在途义务定。此轮没有冻结这些值，不从名字猜bit数。事件tap至少涉及每bank实际R、rawW、physicalW、AW firstreserve/actualaccept/B和job birth/done，以及PTE/walk事件；具体多读端口、fanout、更新时间与组合路径未知，不能把“可RO看信号”当免费端口。

父窄域已经准备好a/g/b/D/E时，两score为4次加法、2次max、1次compare；不含U/H与逐拍data包络准备、长度×服务、per-action AW许可、两份B、完整sum、tie/年龄/溢出/快照和pipeline。节点数不能替代面积/Fmax，也不能替代选择延迟。共享observer若为所有规则提供数据仍须计实际费用；最便宜普通实现可以不保留未用记录。

仅CPU/AXI域真实endpoint反馈时无须强添MIG CDC；读取MIG域事件则另计CDC/事件不丢失/时钟对齐。实际CDC设计、选择周期、完整总位数/端口/FF/LUT/BRAM/Fmax/面积/CPU成本/应用收益全部null。没有通过“数字未知”省略应收费类别，也没有对未成立的literal版本动RTL来测成本。
