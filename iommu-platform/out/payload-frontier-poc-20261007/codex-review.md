# Codex 关键审定：物理写尾仍在排出时的源读准入

日期：2026-10-08。延续10月7日原型；本次设计、关键RTL修改、诊断、源码审定和执行由Codex承担。当前只是tests中的两真实iDMA/两bank实验，不是四DMA实现或板上结果。

## 独立设计与最强普通对照

旧Mode7要求虚拟W队头、物理W队头和同源段序号都一致。新假设：如果目标物理AW已经握手，其前面的物理写已经把整个payload送入TC，则它们无需新的源R，可以在物理W仍被占有时开始当前虚拟W队头的源读。

完整扫描版按物理AW首次offer/WLAST队列重建每源段序号，检查目标之前每个段都已virtual-WLAST。简化普通版只检查当前virtual-W owner、准确源/目的burst偏移/len/size，以及该段physical AW已握手。两者仍继承原来的准确当前段/连续下一段规则，不增加payload池。fault16在每次额外grant时独立检查完整扫描见证；普通版的扫描只作测试观测，不作为功能门，因此本轮没有其去掉观测后面积证据。

关键代码：
- [完整/简化谓词](../../tests/nonblock_completion_pool_late.sv:143)：seq以每源physical W已完成数初始化，只扫描实际未消费物理前缀；检查virtual W整段已进入下游。
- [准入和只读统计](../../tests/nonblock_completion_pool_late.sv:181)：保留launch==tx与VALID稳定锁定；wait计数独立于FrontierEnable，是每cycle满足额外准入条件的源个数之和。
- [完整见证监视器](../../tests/nonblock_completion_pool_late.sv:448)：fault16；所有旧fault以及R流式安全检查保留。
- [具体观测接口](../../tests/nonblock_actual_dma_top.sv:635)：虚拟/物理AW按各mux真实w_fifo_push首次offer，physical-prepared按comp AW真正握手，W按相应WLAST握手。comp不是DDR端口，写尾夹具在更下游DDR模型。
- [写尾反压](../../tests/nonblock_actual_dma_test.cpp:566)：completion端口、len255、已接收248拍时，仅撤WREADY64cycles。每段一次，有限释放；不撤VALID、不暂停AR/R。默认pause0沿原DDR驱动路径。
- [窗口快照](../../tests/nonblock_actual_dma_test.cpp:1044)：posedge之后取累计量，使差值对应旧窗口pre-edge事件(open,close]；不是只读寄存器的pre-edge差分。旧window/PASS断言不删。

virtual-WLAST意味着该段全部W数据已由TC入口接受，并不表示DDR已经收到或写响应B完成。TC stream_fifo无正常flush，W存入真实FIFO；这里仅讨论正常成功copy。源R无需另一个source的DMA数据缓冲。仍需下游有限释放，不能声称无限背压下无条件进度。

## 为什么复杂扫描没有独立价值证据

六条最后模型结果中，完整扫描和简化普通版的P、PN窗口/终端字段分别相同，已记录的CSV事件文件也逐字相同。两者分别96次额外窗口准入，fault16及所有旧E2检查未触发。不能因此推出所有请求、异常或四DMA状态都等价；CSV只覆盖已记录事件。

源码给出了条件性解释：
- [输入bank demux](../../tests/nonblock_completion_banked_ctl.sv:291)没有AW/W spill，依据原ID bank bit选路。
- [axi_demux_simple](../../cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv:168)在w_open非零时，只向同bank继续发AW；直到虚拟W末拍完成才允许切bank。
- 在本原型中，源读只有匹配physical AW已准备才放行；准确连续下一段亦要求相应AW准备。因此已送完虚拟W的前序段，其AW已准备。正常bank内部AW不越过更早AW。
- 这些条件使“当前虚拟W队头的物理前序写仍有未供给数据”的差异难以出现；多出来的是已有payload在物理/DDR尾部排出。当前证据足以否定这个矩阵下扫描状态的必要性，尚未构成一般AW序保留定理。

不要靠解除AXI正常W所有权规则来制造反例，也不要把测试模型准入过严称为原生产RTL的已确认bug。

## 已核正常job reset边界

[DEFAULT iDMA完成](../../cva6/corev_apu/iDMA/src/idma_backend.sv:519)由最后burst写响应产生；[写响应通路](../../cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:451)接实际B。正常最后B并非源R收到就job_done。本驱动每源一次正常4KiB对齐copy，job重叠及未排空由旧fault1监视。
但job_fire是frontend到job FIFO的接受，不等于backend开始执行；硬件并非普遍禁止提前排下一job。多个job、错误、取消、失效/地址更新与序号复用都未覆盖，不能把本计数账本直接放入生产。

## 因果证据与结论

同一PN事件窗口，A第二写在cycle653触发64cycle写尾暂停。完整扫描/简化版在654发V源读、657进入DDR；严格版直到725才发该读。夹具在717释放；最早数据返回707，允许读返回与已完整供给的前序写尾重叠。本证据解释这个受控状态，不能推导MIG实际尾延迟或板上PN退化原因。见原始CSV；无额外上板观测器。

PN严格102686 -> 扫描/普通96267 cycles；P严格101731 -> 扫描/普通95692；V/A各64。复杂扫描未超过普通规则，PN仍比最强普通P多575cycles（0.6009%）；正常无夹具PN两版92374，额外动作0。

**验收：工程假设可达且有限反压下有改善；独立扫描机制创新未成立，不作为二区主机制推荐。** 不把改善幅度直接等同论文创新，不把一次确定性模型比较当统计/硬件结果。

## 前作边界

[AXI-REALM, DATE2024官方全文](https://past.date-conference.com/proceedings-archive/2024/DATA/493_pdf_upload.pdf)，III-A Write Buffer：在收齐fragment数据后才向下游提供AW/W；说明按写数据准备状态解除等待已有明确前作。它没有证明本平台全部正常/异常状态或这个exact谓词等价，不能说前作已实现同一RTL。

[Physically-Aware Preemptive Virtual Channels for Deadlock-Free AXI NoCs，2026预印本](https://arxiv.org/html/2607.01430v1)，I：DMA的R到W依赖及与有序传输共同构成的死锁问题已有研究；一般解除读写等待环不能单独声称首创。原网页/PDF浏览文本保留在literature-primary-raw.txt。未做穷尽性认证。

## 下一主线的具体门槛

继续多DMA共享IOMMU的性能/成本主线，停止围绕此扫描加复杂度。下一先把四DMA IDs0/4/8/12、两bank映射0/2和1/3、真实共享TC队头和写数据顺序放入同一模型规格，区分完整DMA身份、bank身份、copy epoch和burst序号。本次没有实施或跑四DMA。

要进入下一个机制PoC，先明确三项：
1. 合法且现平台可达的状态，使普通shared选择队列+旁路、每DMA队列、准确写准备门、完整返回预约这些强方法，在相同容量/同信息下无法同时保住提前翻译和进度；给请求顺序/等待关系，不能只改变普通方法参数使其变弱。
2. 新规则究竟增加哪一项信息或改变哪一条依赖，并能给出什么额外可验证保证；普通写数据ready也能表达的条件降为基线。
3. 比较V/A共同吞吐、FAST保留、payload/请求头/控制全部成本；若只有bank改身份或普通队列扩容，按工程扩展处理。没有区别证据就不做BIT或大实验，不承诺二区。

这是下一判别问题及入场条件，不是已提出并验证的新机制。论文价值也可来自严格的组合边界/成本结论，但仍需超出普通方法能直接推出的事实。

