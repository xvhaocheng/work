# 源读提前放行：8次仿真与创新判别

日期：2026-10-08。目录名沿用开始时间20261007。**有限DDR写尾反压下，修正过严源读准入有改善；完整前缀扫描与更简单普通规则效果相同，独立创新未成立。**

## 实际结果

两真实iDMA、正常分bank、A8/gap0/V64、warmup256、4KiB copy、每源一活跃job。每个测量窗口V/A都完成64次copy；PN保留62次FAST，P为0。

| 路径 | P耗时cycles | PN耗时cycles | PN额外窗口grant |
|---|---:|---:|---:|
| 原Mode7严格虚拟/物理写队头门 | 101731 | 102686 | 0 |
| 扫描物理前缀已完整供给payload | 95692 | 96267 | 96 |
| 普通虚拟写队头＋匹配physical AW准备 | 95692 | 96267 | 96 |

此表为最后三个模型各P/PN一次，共6项。夹具对每个256beat completion写，在DDR已接受248beat时暂停WREADY64cycles，然后自动释放；所有路径使用同一夹具。每窗ddr_w_stall=16384、DMA读/写burst各256、写65536拍相同；累计1284个写尾暂停、82176cycles包含warm及排空。

相对本原型旧严格门，PN耗时少6419（6.2511%）、固定V/A工作量对应吞吐高6.6679%；P耗时少6039（5.9362%）。改进后PN仍比P多575cycles（0.6009%），没有PN超过P的证据。CPU软件轮询次数随耗时改变，不能说全部DDR请求逐个相同，或把收益全归于某个内部周期计数。

此前另有两项无夹具早期校准：严格/扫描PN均92374cycles，额外grant/eligible-wait均0。它们编译于加入写尾夹具/ScanEnable之前，保留作为原自然状态校准，不称最后模型的默认路径回归。

完整扫描/普通简化规则P、PN的已记录事件CSV各逐字相同，见[轨迹比较](trace-equivalence.json)。这是本矩阵的等价证据；没有所有输入/异常/四DMA的普遍等价或活性证明。

## 状态含义和因果

virtual WLAST说明整个段已进入TC，写尾还可能占着physical W或DDR。旧规则继续要求physical W队头也一致，错过了用这段尾部等待启动下一源读的机会。新扫描先检查旧写payload全部供给；普通版用当前virtual W头和matching prepared AW做到同样效果。

例：两PN轨迹A第二段在cycle653开始写尾暂停；扫描/普通版654发V源读、657进入DDR，严格版725才发；暂停717释放，V首数据707返回。仅解释受控模型下的可重叠状态，不是板上MIG行为或既有板端PN变慢的因果证明。

frontier_wait_window按每cycle符合“额外准入”条件的源个数累加，严格PN6816、P6592；新两版都96。它包含实际额外grant那一个周期，不能叫96个等待请求，也不能与grant数量一般等同。raw含warm/排空，window通过posedge之后的开闭快照对应pre-edge事件(open,close]。

## 实际改动和验收

新增功能只在4个tests源：late pool增加默认关闭的准入变体、只读统计和fault16见证；两层top传参/传观测；C++新增有限反压及开闭窗口统计。准确地址/长度匹配、VALID锁定、所有旧E2 fault、实际DMA数据/排空检查保留；没有将测试计数器放到生产RTL。

5次SV测试构建成功，1次因Codex将trace_event写成event而失败，已改正并保留失败记录。实际仿真8次全部exit0/PASS；2项早期校准＋6项最后模型。终端READ-TAG fault/queued0，E2 pending/occupancy/metaqueued0，READ-ADMIT捕获=派发1284、empty1/debt0，数据接收=回放328704拍；原字节核对、quiet及fault监视通过。fault16没有单独JSON字段，PASS结合C++逐tick检查dbg_e2_fault==0提供本次未触发证据，不能称模型检查器证明。

76个实际编译生产SV前后SHA256相同。48B只计两iDMA的3beat小FIFO，排除共同TC FIFO、元数据、控制和扫描监视器；没有LUT/FF/BRAM/总面积或生产时序结果。未改Linux、生成BIT、写SD或操作板卡。

## 创新判断与下一主线

**将完整扫描降为工程对照，不推荐为当前独立论文机制。** 简化版同效，正常负载额外动作0；组合通用写数据ready、普通选择队列或读写依赖调度不足以说明新增贡献。细节与强前作见[Codex关键审定](codex-review.md)。

继续原多DMA共享IOMMU性能/成本主线。下一先规范真实四DMA/两bank的完整身份、copy epoch、burst及共享TC头顺序，寻找普通准确准备门/共享选择队列＋旁路/每DMA队列/完整返回预约在同资源、同信息下无法兼顾提前翻译与进度的合法边界。若仍等价，就换候选，不扩充复杂度掩盖差异。没有创新验收或二区水平承诺。

## 证据入口和作者

- [全部运行、指标及日志行号](runs.json)
- [构建/运行/改动阶段清点](stage-evidence.json)
- [本阶段测试差分](test-source.patch)；before/after为本阶段前后快照。
- [生产前hash](production-before.json)／[后hash](production-after.json)
- [DeepSeek完整指标草案](evidence-answer.md)／[完整原回复](evidence-raw.json)
- [三次DeepSeek实际使用量](model-usage.json)：378225 provider tokens，high/393216，均完整返回。
- 首次/修订instrument原答复、task、patch均保存。Codex拒绝其“job_fire可以推断正常job早于最终B复用”等无完整证据说法，并修正修订中的wait定义和pre-edge差分偏一；未把模型意见当验证。

DeepSeek负责观测接口/C++字段草案、有限证据整理及文档草案；Codex亲自设计新谓词与强普通对照、修改关键RTL/夹具、修订应用、执行编译仿真、核源码/原论文和最终验收。本阶段未调用Claude或新Codex子agent。

