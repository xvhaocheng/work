# §41 下一判别：成功 copy 的最小续行工作量，先判论文增量

根 Codex，2026-10-08。不是已实现机制、不是未来BIT指令。固定正常映射；不额外加译址寿命锁，不制造fault，不永久关闭预算，不假定端点无条件接收每cycle一个响应。

## 要回答的具体命题

暂停在t0提出后，为真正让其它DMA使用指定共享资源，当前DMA必须继续完成多少**原本还未发出的**请求？能否以紧界而非整job等待来表达；这项界是否已有强前作给出？只发现端点等待环、普通分段、反馈停source或保守drain界没有新意。

成功与可恢复copy不要求清除所有parent metadata。指定释放点g和其它流可继续服务的判据，再定义old requests、内部owner锁与允许续行的最小集合。tail无pending不代表上游共享AW/W/AR slot都可供其它流；不能因目标含糊创造优势。

## 一个能手推的受限子问题

每流固定未来物理source端点R、destination端点D；L_R是已不可撤回source/已接收数据要求达到的最低前沿，L_W是已不可撤回destination/已写出数据要求达到的最低前沿。B是最终可独立停车suffix容量，不等于所有中途传输都不需buffer。地址正常、mask可服务、私有停车、两侧顺序兼容且所需翻译/外部服务独立进展时，最终(p,q)须满足：

`p∈R, q∈D, p≥L_R, q≥L_W, q≤p≤q+B`。

从q=next_D(L_W)开始，反复 `p=next_R(max(L_R,q))`、`q'=next_D(max(L_W,q,p−B))`。端点有限且含N，单调到固定点。对任意可行(p*,q*)，从q≤q*有p≤p*继而q'≤q*，所以得到逐分量最小可行最终前沿。这是普通守恒/取整固定点，**不是新的算法名称或创新证据**。如果合法source端点可以选择新增，必须另计cut成本，不偷偷让next_R免费取所有字节。

代表假定状态：N32KiB、source2KiB端点、destination 1KiB/3KiB/5KiB…/31KiB/32KiB、L_R2KiB、L_W1KiB、B24。不允许新增source端点时q=3/5/…/31/32KiB，source取整4/6/…/32KiB，最小释放前沿到整job结束；允许在3KiB加一个合法未来source endpoint，则p=q3KiB即可。首个软件subjob3KiB、普通内部非均匀splitter同样可达到这个一cut点。

这只是受限子问题的纸面演算，**未证明该状态能在§39 DUT实际出现**，也未运行。实际t0已接收/写出位置要记录，postpause工作应扣除t0已完成部分；3KiB/32KiB是绝对copy前沿，不是新增传输字节或周期。源原parent若为2..4KiB，可以保存其剩余1KiB身份后暂停；是否占其它流需要的shared资源要沿真实连线证明。

此演算还假定所需目的W路径/译址能独立进展：若当前immutable 2KiB R已占共享返回、剩余1KiB无法停车，而目的[1,3) AW的PTE只能排在该R之后，则未来加cut3KiB也救不了当前R，不能报告有限continuation。§39本来的prepared-full-child-prefix门已经排除了这个未准备目的的source准入状态。

因此下一实际状态优先从**已经承诺的较远W队列前缀需要更多source/raw-parent**这一种义务选取，不能再为现有child伪造未准备AW。尚未运行它；生产常规shared AR mux和§39按flow私有capture接口也必须分别核。

## 多DMA真正增加的条件

目的续行必须同时尊重虚拟与物理两层W FIFO前驱。它可能增加其它流L_W，从而又增加其它流source需求；source受PTE/共享R路径影响。粗transaction图会把能逐beat交替的普通流式copy误判环，须用beat/可消费前缀或证明等效的压缩状态。不能用普通图搜索本身当创新。

必须核的状态包括：首次VALID未握手、内部mux `w_fifo_push`比DDR AW握手更早、pendingR/B、有效byte lanes及已占空间、needed目的permission/prepared、两W owner/seq前缀、PTW返回路径。若压缩表示遗漏其中之一，其“低成本”是漏义务，不能接受。

## 强对照与论文门槛

首先用已取得primary作命题表：已覆盖什么、所提紧界具体多出什么、前提是否更宽/成本是否更紧、是否有实际平台可达证据。对照 AXI-REALM、DMA350、2018 MMU-aware DMA、2026 Preemptive VCs、2026 AXI TMU、AMD PG313；不把error-recovery和successful-copy保证混用。

工程强控制同时包含内部nonuniform切分、同B短child完整R/完整W预约、选择性停source并有限destination续行、software tiling与macrocopy完成合并。相同候选算法被放入通用splitter不证明前人已发表这个策略，但也不能用只具较弱规则的baseline伪造硬件位置优势；必须比较已知最强决策及真实总成本。

如果完整多DMA命题可直接化为已有预约/普通闭包、且没有新的紧界/表示/真实成本证据，就将此枝条归为实验解释与对照，停止作为主创新。如果前作未给特定紧界而我们能证明必要/充分与真实可达性，可以发展分析贡献；如果还有低成本可实现策略并以同资源强对照验证优势，才可发展机制贡献。检索没有找到不等于世界没有，论文等级不预定。

后续只选一个真实可达状态和对应最强普通控制作最小判别；目标是该命题真伪与成本，不扩大cuts/吞吐矩阵，不进行BIT或实板构建。此文件的模型及实现均未运行。

两席最终审查均已完成：见[前缀与真实义务证明](release-closure-proof-agent.md)、[前作合同审定](release-composition-prior-agent.md)。根直接核全稿后确认：当前没有新的独立机制见证；下一阶段是紧分析界的可达性/增量判别，不是把普通debt allowlist编成新算法。
