# 普通owner与翻译后额度对照：Codex审定（2026-10-07）

**8/8短仿真验收通过。当前停止把旧版C作为首选创新方案；没有实现C，不能说“C没人做过”或“C实测效果差”。** 本轮先实现已有owner准入的保守测试配置，再因新源页重遍历问题补两组已有walk额度对照。Codex亲自定义/推演并核关键源码与实际数据；DeepSeek仅接口整理和两个C++草案。

## 1. 实际改动和验证范围

仅3个tests文件：
- nonblock_actual_dma_top.sv：owner_only_i控制送入harness的fullbuffer_jobs_i许可掩码；实际DMA缓冲仍4KiB，证书观测不变。
- nonblock_actual_dma_system_top.sv：仅透传输入，原DDR/DS/completion代理不变。
- nonblock_actual_dma_test.cpp：+owner-only、+read-admit-measure-only严格0/1；owner控制在warm结束后开启并保持至STOP/quiet，测量专用walk模式在warm用同一buffer模式。JSON明确记录。

[实际补丁](actual-test-changes.patch)、[预先定义和追加理由](codex-test-spec.md)、[8次命令/完整JSON](runs.json)、[验收](acceptance.json)。每个owner和walk case的WARM-SUMMARY与同mode原buffer结果逐字一致；默认关闭精确复现hot PN93487/cold PN94427。所有最终PASS须过源/目的完整字节、TL/DMA/CPU quiet、memory队列空与proxy capture==issued/empty/debt0，实际AR稳定性检查未报错。8份CSV均64个完整V job、无未完整V、无丢行/comp角色错误、字节账本一致。仅正常静态映射模型，不是全故障/并发协议保证。

本机Verilator不支持input默认声明，首次elaboration因此失败并保存；Codex去掉默认声明，由C++显式驱动。随后一次完整仿真模型构建通过，再一次C++编译/链接补测量专用flag。旧模型不覆盖。编译输入中的生产SV hash前后均不变（数量见acceptance.json），无生产RTL/Linux、BIT/板卡/SD动作。

## 2. 普通对照结果

每个窗口V/A都完成64个4KiB，A有效吞吐=262144/W。单次小测试，不是重复统计收益。

| 条件/普通方法 | P周期 | PN周期 | PN FAST总数 | V的fill次数 P/PN |
|---|---:|---:|---:|---:|
| 旧热页，原buffer许可 | 91874（此前） | 93487 | 62 | 0/0 |
| 旧热页，仅owner保守准入 | 92898 | 92857 | 0 | 0/0 |
| 新冷目的，原buffer许可 | 92334（此前） | 94427 | 64 | 1/1 |
| 新冷目的，仅owner保守准入 | 93778 | 93797 | 0 | 2/2 |
| 新冷目的，翻译后的整walk额度256 | 92334 | 91727 | 64 | 1/1 |

owner模式相对原PN耗时分别下降0.674%/0.667%，但FAST降到0；相对原P仍慢1.070%/1.584%。P和PN在owner模式差只有41/19周期，不能将这点差当N加速。N使能仍开，不等于所有内部探测都取消。

整walk普通法保留原64个FAST翻译接受，冷PN比原PN快2.859%，比同模式冷P快0.657%；A有效速度比同模式P提高0.662%。P对照精确92334。该法不是DDR返回优先：FIFO模型、原DS 50/50、同单槽buffer和相同job/工作集保留；只在完成翻译后的AR代理对限制的FAST读先确认PTE入口，再每个完整walk给256拍额度。其它读、既有排空豁免/VALID锁定规则仍保留，不是全体数据每walk最多256拍。PN gate stall9506周期，oversize/exempt0；窗口64个FAST中V62、A2。

CPU轮询依赖反馈，完成窗口改变时请求数也变化（cold原P289/PN313；walk P289/PN281）。这是同生成机制的实际结果，不是恒定CPU请求总数比较。无新S/N完整矩阵，不称超过N或正式板端收益。

## 3. 准入改变了本bank的翻译访问顺序

[source-rewalk-evidence.json](source-rewalk-evidence.json)：
- 原cold PN：先源AR16/26；目的叶PTE0x50010在2107读，fill2216。全窗V只有这一个walk/fill。
- owner cold P：目的叶0x50010于980读/fill1081；随后源叶0x50000于1454读/fill1557，源DDR AR1566/1573。
- owner cold PN：目的叶992/fill1096；源叶1473/fill1576；源DDR AR1585/1592。
- 整walk cold PN只有目的叶0x50010于1766读，没有源叶重读；V fill1、PTWactive670，而原PN1010、owner PN929。

Codex核过IOTLB原实现：update仅改replace_en选中的项（iotlb.sv:318–331）；命中/touch更新PLRU，victim由已有历史与allocation mask选择（375–422）。TL实例启用PartitionLocalHistory(PolicyEnable)，V mask固定2个位置；测试失效输入为0、policy_clear只reset。warm及初始化完全同序，原source已热。owner配置延迟了源翻译查找/touch，先填新目的后源发生miss；在这些源码约束下，新增填充替换了所需源项。没有重新发现跨bank共享PLRU旧缺陷，也不是该逻辑的数据正确性bug。无需再加板端观测线路。

这说明“控制数据何时进入DDR”与“控制翻译何时查找缓存”是不同操作。前者可以保留翻译访问次序；后者可能同时改变replacement和miss数。不是说翻译后门控一定更好，所有资源/owner/排空约束仍须核。

## 4. 本轮对创新的裁定

1. owner规则本身是原有机制的配置，只能作为工程/普通对照。
2. “保留FAST翻译、推迟部分数据”已由普通整walk额度在本case取得；不能重新命名为新C贡献。
3. 本轮没有C算法测试，也没有逐项排除世界前作。因此既不能断言C不存在新颖性，也不能将其称新颖却性能差。
4. **当前停止旧C首选推荐。** 没有证据证明新增自己的目的prepare表，在当前两个DMA、单job/每bank及该工作集下，提供了强普通对照之外的收益/成本价值。本结果不是C与所有普通算法全状态等价的证明。
5. 下一候选应先给出真实合法状态对：普通walk/在途额度及两端普通准备在同资源下必须作相同选择，而具体新增信息允许有用的不同选择；或者给出同效果的实测成本减少。当前还没有这样的反例，不为了投入C重构而先放宽对照。

继续多DMA共享IOMMU性能主线；将91727和普通walk规则列为冷页新强基线。不能再把“普通限流能令PN略胜P”当创新验收；也不需要强迫每个工作集PN领先。普通方法负结果/边界仍可提供机制分析，但单靠该表不保证论文贡献成立。

## 5. 前作边界和作者分工

重新核[MMU-aware DMA ICCD2018 §IV-C](https://arxiv.org/pdf/1808.09751)的TLB锁定代价及burst身份/排空重发，和[AXI-REALM §V-B](https://arxiv.org/pdf/2501.10161)的调节粒度/预算与缓存竞争、过度调节代价。不能把锁cache或增加metadata本身当新方法；这些论文并不逐项等于本次准入顺序/源重遍历事件。此前MASK、DWS、NB-Walker正文缺口等保持，未做穷尽新颖性检索。

DeepSeek三次完整high/393216思考：151450＋60735＋58361＝270546 provider tokens，均stop/未截断；完整任务/答案/JSON保留。它没有执行或写文件。Codex独立规格、关键SV/架构/HOL判断、源码与原文审查、应用草案、实际测试和最终裁定。worker“必须改生产banked才能取owner”的过强结论未采用，测试已有层级探针可访问；本对照直接复用原guard无需新owner接口。无Claude/新Codex子agent。
