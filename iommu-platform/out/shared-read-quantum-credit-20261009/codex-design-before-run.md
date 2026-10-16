# §62：改变真实 W 量子和 private 容量，核准入边界

Codex关键参数化设计，C++原夹具复用，不新写框架。§61 q2/B3下peer与普通resident策略差多数<0.3%，不能据旧groups barrier的8.7%命名独立机制。

新elaboration参数WriteQuantum（默认2）同时传给真实qwriter Q和source owner Q；未W覆盖的private借槽上限从常数2改为Q；实际每DMA BufferDepth配为Q+1。所有三策略同q/FIFO预算/WRITE预取/独立真实TL权限/normal/physicalsourceR256；比较Q2/B3、Q8/B9、Q32/B33，全部mode2、nohandover/pause。原handover关闭且顶层禁止Q!=2时启用，原J-even证明不泛化。

peer受限证明从2自然替为Q：当共享R游标r因某flow库存r−w_i=Q阻塞，旧W头j的start_j<=创建时peerW<=现在w_i、len_j<=Q，故end_j<=w_i+Q=r，数据已交给j，正常最终服务可排头。普通resident则所有firstVALID W头数据预先到齐，更强但可能无法隐藏AW/TC延迟。未prefetch的peer仍共同成功WRITE前沿裁child；自主future-WR成功链和source物理页尾RLAST释放保持；不是对目标WRITE缺授权时直接写。

关键DUT验证：Q2默认原CSV exact；Q8/32每策略不同F1/127/128/255完整copy、真实targetW/srcR计数，压力B64/背压时再比较。如果大q收益仅需更多buffer或简单own-data准入也同效，就不能立peer独立机制；若同预算提前承诺获得新进度/性能，仍须原文核最强credit/order-aware相关方案和综合成本。声明私有payload随Q增加是实际配置，非默认平台、非零cost；一次reset单job/静态映射限定不变。
