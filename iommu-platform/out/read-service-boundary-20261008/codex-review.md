# Codex 审定：提前翻译收益被后续等待压住（2026-10-08）

## 对用户问题的回答

复杂方法与强普通方法的已记录轨迹逐字相同，首先说明这组输入下作了同样的放行决定；这本身不能证明其他瓶颈。另一个问题是PN已完成额外提前翻译，却没有总耗时收益。本轮新证据显示：提前翻译后的写顺序/源读准备等待很长；行为内存模型的串行准备延迟也影响绝对耗时，但它不是PN略慢的唯一原因。

## 当前实验边界

复用§25的ordinary virtual-W模型，不重建SV。参数SharedPoolMode7、FrontierEnable1、FrontierScanEnable0、HeaderParkMode0、每真实iDMA三拍FIFO；两真实DMA、两bank、正常aligned4KiB job。A8、V64、gap0、warmup256，原DS50/50延迟及PTW模型延迟31，普通read proxy buffer、不使用walk预算。去掉写尾夹具；AW/W模型READY正常为1。

Codex仅修改`tests/nonblock_actual_dma_test.cpp`，加入读服务状态计数及默认serial的模型选项。一次原obj C++编译/重链接成功，四次实际RTL仿真exit0/PASS。生产编译清单76个SV前后哈希相同。原§25二进制冻结为[legacy-binary](legacy-binary)；原obj中的同名二进制现已重链接，旧实验日志/CSV不变。

## 读延迟因果对照

原serial：每个请求进入内存返回队头后才开始等待31cycle。overlap：从该AR被接受开始计时，排队时可与前序请求重叠；孤立请求首次R仍为AR后32cycle。只在measurement_phase应用，预热仍用原serial路径；每模式serial/overlap的WARM-SUMMARY逐字一致。

不改RTL或R返回顺序，不加跨ID beat交错，不提高数据口每cycle一拍的服务速率；保留队列容量、R VALID保持及原字节/排空检查。它是单个行为模型因素的干预，不是MIG准确模型。

| 模型 | P cycles | PN cycles | PN−P |
|---|---:|---:|---:|
| 原串行准备延迟 | 92109 | 92374 | +265（+0.288%） |
| 请求接受后延迟可重叠 | 85339 | 85984 | +645（+0.756%） |

四项每窗口均V/A各64，PN FAST62。两法都加快，但PN仍略慢，所以不能用串行准备延迟单独解释PN退化。

## 精确服务账本与其限度

窗口沿用pre-edge(open,close]，每cycle只计一种状态：队列empty、head延迟setup、R transfer、R backpressure。四类之和严格等于W，transfer/bp等于原DDR读拍/反压计数。

| 项目 | serial P | serial PN | PN−P | overlap P | overlap PN | PN−P |
|---|---:|---:|---:|---:|---:|---:|
| 队列空 | 4351 | 4592 | +241 | 7570 | 8013 | +443 |
| 等待准备 | 19638 | 19607 | −31 | 9381 | 9509 | +128 |
| 返回数据拍 | 67901 | 67893 | −8 | 68085 | 68149 | +64 |
| R反压 | 219 | 282 | +63 | 303 | 313 | +10 |
| 总计 | 92109 | 92374 | +265 | 85339 | 85984 | +645 |

这是差值的完整分解，不是“241cycles全由某个模块造成”的因果证明。PN略慢的差额主要表现为queue empty，而不是更多内存读拍。serial两法DMA data AR均256、DS AR均93，但CPU polling数分别285/284；overlap为307/315。CPU数随窗口/软件模型节奏改变，不能称所有DDR工作量相同，更不能据此认定板上内存带宽饱和。

## 轨迹事件配对结果

DeepSeek提出解析脚本，Codex核事件字段、源ID、bank、FIFO匹配、窗口边界及实际代码后执行。所有capture→physical arfire→DDR data AR按每源顺序、地址、len255配对：P与PN均V128/A128。端口2是DMA completion，CPU端口0、DS端口3未混入。

- 两法、两源每一次：capture→arfire=2cycles；capture→DDR AR=5cycles；arfire→DDR AR=3cycles。
- PN bank0的62次FAST response与62次带FAST的V capture一一配对，非负，数量与原计数一致。
- 第一源burst31次：FAST→capture全部1051cycles，→DDR AR1056cycles。
- 第二源burst31次：FAST→capture全部1052cycles，→DDR AR1057cycles。
- FIFO重建只假设window开端pending=0，按AR加一、RLAST减一；无负数，重建空区间总长分别4351/4592，精确匹配独立新计数。这个边界假设经本轨迹一致性验证，不推广到其他运行。

长等待发生在proxy捕获之前；当前只匹配同bank唯一V源，不能推广为四源FAST配对或所有作业的因果规则。等待时间不能当成可全部消除的收益，不能相加成总耗时改善。

## 首个FAST的直接证据链

[serial-pn-events.csv](serial-pn-events.csv)中的相对窗口cycle：

| cycle | 事件 |
|---:|---|
| 2478 | V AW等待flags17＝stall标记16＋跨bank W所有权冲突bit0 |
| 2483 | V第一burst FAST翻译返回 |
| 2489 | V第二burst FAST翻译返回 |
| 2933 | A bank页表fill |
| 2934 | A PTW结束 |
| 2944 / 2951 | A两段源数据AR到内存 |
| 2976 | A第一段首R |
| 3518 | A第二段最后R |
| 3524 | V AW等待解除 |
| 3534 / 3541 | V两段读被proxy捕获 |
| 3539 / 3546 | V两段数据AR到内存 |

源逻辑直接核查：
- `tests/nonblock_actual_dma_system_top.sv:1392–1394`的flags bit0是输入demux已有W未排空、且新AW选择另一bank。
- `tests/nonblock_completion_pool_late.sv:142–171`普通virtual门仍要求当前virtual W head以及准确matching prepared physical AW。
- `tests/nonblock_actual_dma_top.sv:646–661`这些输入来自真实两个mux的写队列与实际AW/W握手。

因此本样本V地址早就译好，当前写路径仍在等待A，并且源读准入还要求目的写准备；更复杂的物理前缀扫描无法凭空解除同一等待。已能定位“提前翻译之后的约束”，但**尚未通过只改变写路由/准备条件的安全干预，独立量化它对265cycle PN总差的因果贡献**。不把长等待等同于bug，AXI/W依赖必须保留。

## 研究判断与下一主线

停止继续扩复杂扫描。下一先针对跨bank AW/W锁定与源读供给关系，比较普通顺序队列/小头暂存/准备门是否已经能做出相同决定，再决定是否有额外信息或可证明的成本—进度差。单纯增加队列、提前翻译或预留额度已有强普通方法，当前没有通过创新门槛，不能承诺二区。

四DMA抽象结果已独立保存：[审定](../four-dma-boundary-20261008/codex-review.md)。普通当前W-bank预留两头也排除了全部36读交错中的模型停滞；该结果不支持盲目共享头作为新规则。先证明真实状态可达和最强普通方法无法兼顾的限制，再改生产RTL/上板，避免堆复杂度。

[四运行及日志行号](runs.json) / [轨迹分析](trace-analysis.json) / [实际C++补丁](test-source.patch) / [模型计数器版本](after.cpp) / [DeepSeek原提案](trace-analysis-answer.md)。

DeepSeek整理四路接口证据、起草抽象脚本及轨迹解析脚本；Codex亲自设计模型合同/因果干预、关键源审查、诊断和实际执行。没有调用Claude或新Codex子agent，没有Linux/BIT/SD/板卡动作。本轮三次DeepSeek完整high/max393216返回共385750 provider tokens；DeepSeek未执行命令。

