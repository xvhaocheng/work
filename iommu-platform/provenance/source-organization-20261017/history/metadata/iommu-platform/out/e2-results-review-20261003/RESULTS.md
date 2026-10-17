# E2 64项结果复核报告（2026-10-03）

保存路径：`/home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/RESULTS.md`

## 先给结论

1. E2 64项已完成并通过本轮验收：64 clean、0 failed、0 missing；1226个输入文件SHA未变；64项dmesg before/after无新增行，stderr为空。taint为0只说明该位为0，不解释成没有任何warning。
2. 本批达到E2局部替换机制及首次再用代价的阶段验收。它证明在本访问方式下，A与V需要保留的翻译记录总数超过IOTLB容量后，A可以替换掉V的两个target，并在下次探测时造成首miss与额外翻译占用；不证明所有共跑慢都由E2缓存替换或E3共享服务阻塞造成，也不把E2现象本身包装成创新。
3. 容量边界清楚：IOTLB共16项，V占2页；W7是14+2=16，未超过；W8是16+2=18，超过。因此W7组表现为resident_hit_negative，W8组表现为A_evicted_then_first_miss。W7 vs W8是本访问方式下的局部证据，不外推到所有替换策略。
4. E2阶段收尾，暂不增加board扫描、不改Linux/BIT、不为这批重新上板。下一主线是等Mac方法462完成回传后，按已冻结规则评估V保护程度相近时的A进度、控制开销、稳定性和独立验证。本任务未读取方法结果，不宣称方法完成或收益。
5. 归因诚实：DeepSeek负责证据整理、脚本草案、报告草案；Codex负责关键源码与原数据审查、实际执行校验、修订最终汇总及决策。两份早期DeepSeek解读/草案有错误已排除，正式报告只以accepted-summary和原始记录为据。

## 证据与验收依据

| 项目 | 路径或字段 | 结果 |
|---|---|---|
| 逐项汇总 | [证据文件](/home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/accepted-summary.json) | cases_read=64，cases_clean=64，problems为空，global_problems为空 |
| 输入完整性 | [证据文件](/home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/integrity-result.json) | files=1226，changed为空，unchanged=true |
| E2回传说明 | [证据文件](/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/E2_DELL_RECEIPT.md) | 64/64已执行并保存，rsync内容核对无差异 |
| E2冻结清单 | [证据文件](/home/xvhaocheng/work/iommu-platform/docs/research-two-bit-v2.md) | 短4、边界6条件各5、前缀W8/16正向各N1/8/16各5，共64 |
| 原始板端目录 | `/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/e2/` | 每项runs/<ID>/含record.json、archive.tar.gz、metadata.json、退出码、taint、dmesg、argv及哈希 |

Codex已核对的实际验收包括：全64 archive SHA、归档成员对board文件、record SHA、exit、taint、uname、argv；board上exit均为0，taint before/after均为0，uname与argv均匹配。冻结的research2_results.py输出64 ok、0 failed、0 missing。DeepSeek草案经两次验证接口失败后，由Codex限定修正的e2_audit_aggregate.py最终64 clean、exit 0。

实验步骤：每次先清理翻译缓存，再让V预热2次，保留一个源页和一个目的页的翻译记录；随后让A单独搬运指定次数。所有条件的A阶段固定为5,000,000周期（100ms），A完成且请求排空后，V再搬运一次4KiB。这样把“留下的缓存替换影响”与“正在共跑时的排队”分开。这里的PTW是查页表过程；miss是翻译缓存未命中。

## 术语大白话

- A：本实验中的竞争访问阶段，按顺序触碰源页和目的页，用来观察是否把V的缓存记录挤掉。
- W：A分配W个源页加W个目的页。例如W8表示A分配8源页+8目的页。
- N：实际4KiB搬运次数。N1、N8、N16表示本组实际搬运次数，不等同于分配了多少页。
- V：被探测目标侧；本实验关心V的两个target在探测前是否还在缓存里。
- target：V的两个被观察页，src和dst各一个。
- resident_hit_negative：对照含义，V的target仍在，首探命中，本组没有发生替换。
- A_evicted_then_first_miss：V的target先被A替换掉，随后V首探miss。
- D87：V完成减V发出，表示一次4KiB探测的总耗时。
- D88：首次选择减V发出，64项全部为5。
- D89：V翻译主状态累计占用，包含等待PTW的时间，不等于纯计算时间。
- IOTLB：地址翻译缓存。本平台共16项，V占2页。

## 总计与分类

64项按清单分为三块：短4、边界30、前缀30。边界6个正式条件各5轮，共30次运行；前缀6个正式条件各5轮，共30次运行；短4每项1次。每轮有src/dst两个target，因此分类计数中的128个target不是128次独立试验。

全部64项最终分类合计为：62个target为A_evicted_then_first_miss，66个target为resident_hit_negative，没有其他类别。这个计数是target级汇总，不是独立重启次数。全部64项都从预热快照逐条重放提交更新，重现了探测前及探测后的缓存快照。

## 边界6条件表

| 条件 | 次数 | 每次A触及源页＋目的页 | V两页首次探测 | V探测 miss／查页表 | 翻译占用中位数（周期） | 整次探测中位数（周期；微秒） |
|---|---:|---:|---|---:|---:|---:|
| W0-正向，N=0 | 5 | 0＋0 | 两页均命中 | 0/0 | 20 | 698；13.96 |
| W1-正向，N=112 | 5 | 1＋1 | 两页均命中 | 0/0 | 20 | 695；13.90 |
| W7-正向，N=112 | 5 | 7＋7 | 两页均命中 | 0/0 | 20 | 696；13.92 |
| W7-反向，N=112 | 5 | 7＋7 | 两页均命中 | 0/0 | 20 | 696；13.92 |
| W8-正向，N=112 | 5 | 8＋8 | 两页均未命中 | 2/2 | 822 | 1483；29.66 |
| W8-反向，N=112 | 5 | 8＋8 | 两页均未命中 | 2/2 | 821 | 1486；29.72 |

整次探测周期范围：w0-正向-n0 694–709；w1-正向-n112 694–708；w7-正向-n112 694–699；w7-反向-n112 694–717；w8-正向-n112 1480–1488；w8-反向-n112 1485–1498。

## 前缀6条件表

| 条件 | 次数 | 每次A触及源页＋目的页 | V两页首次探测 | V探测 miss／查页表 | 翻译占用中位数（周期） | 整次探测中位数（周期；微秒） |
|---|---:|---:|---|---:|---:|---:|
| W8-正向，N=1 | 5 | 1＋1 | 两页均命中 | 0/0 | 20 | 700；14.00 |
| W8-正向，N=8 | 5 | 8＋8 | 两页均未命中 | 2/2 | 822 | 1485；29.70 |
| W8-正向，N=16 | 5 | 8＋8 | 两页均未命中 | 2/2 | 822 | 1488；29.76 |
| W16-正向，N=1 | 5 | 1＋1 | 两页均命中 | 0/0 | 20 | 696；13.92 |
| W16-正向，N=8 | 5 | 8＋8 | 两页均未命中 | 2/2 | 823 | 1487；29.74 |
| W16-正向，N=16 | 5 | 16＋16 | 两页均未命中 | 2/2 | 821 | 1486；29.72 |

整次探测周期范围：w8-正向-n1 697–702；w8-正向-n8 1482–1487；w8-正向-n16 1485–1491；w16-正向-n1 695–702；w16-正向-n8 1481–1490；w16-正向-n16 1479–1495。

N1时两条V记录保留；N8和N16时两条都已被A替换，随后首次访问未命中。仅有1/8/16三个采样点，不能给出精确的最早替换次数。W16N8实际触及8源页＋8目的页；与W8N8页数相同，但目的页地址布局不同。

## 短4项（单次验收，不混入正式重复）

| 条件 | 次数 | 每次A触及源页＋目的页 | V两页首次探测 | V探测 miss／查页表 | 翻译占用中位数（周期） | 整次探测中位数（周期；微秒） |
|---|---:|---:|---|---:|---:|---:|
| W0-正向，N=0 | 1 | 0＋0 | 两页均命中 | 0/0 | 20 | 704；14.08 |
| W1-正向，N=112 | 1 | 1＋1 | 两页均命中 | 0/0 | 20 | 707；14.14 |
| W8-反向，N=8 | 1 | 8＋8 | 两页均未命中 | 2/2 | 827 | 1490；29.80 |
| W16-反向，N=1 | 1 | 1＋1 | 两页均命中 | 0/0 | 20 | 695；13.90 |

整次探测周期范围：w0-正向-n0 704–704；w1-正向-n112 707–707；w8-反向-n8 1490–1490；w16-反向-n1 695–695。

## 已核对的机制链

Codex直接看源码与代表record后确认的机制链是：warm阶段两个target驻留；A阶段DID11提交更新，把old V/pscid1变为new A/pscid2；探测前snapshot显示两个target均消失；quiet A已停止后才发Vprobe；两个target首探miss；探测期间Vmiss/PTW为2/2，main occupancy约820 cycles。这个过程在W8的N112边界组以及W8/W16的N8/N16前缀替换组中重复出现，并在全64重放中与warm/deadline/freeze匹配。

W8正向边界r1例中，事件16在epoch 40520替换dst(slot2)，事件17在epoch 40961替换src(slot10)；dst首查时刻为5028528，src首查时刻为5028939，均miss。顺序是先dst后src。src记录里的other live不能说明A后dst仍在，它更符合dst首查后回填的现象。

无效旧tag不能证明没有flush。驱动源码[idma_research.h](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma_research.h:589)实际显示flush_s1在warm前执行；因此不能用旧tag残留推断未flush。

## 计时口径与不能说的话

时钟为50MHz，每cycle 20ns。D87是V完成减V发出的总4KiB探测耗时；D88是首次选择减V发出，全64为5；D89是V REQ_TRANS/AWAIT_RESP主状态累计占用，包含等待PTW，不等于纯计算时间。两个target首查不等于全探测4次lookup；全hit组的D8C/D8D是4hit/0miss，替换组是2hit/2miss。

总时间差不能全部精确归因给纯PTW；total减main也不能直接叫纯排队。单次探测不是稳态吞吐，也不是method收益。A阶段固定5,000,000周期在50MHz下是100ms窗口，不是替换延迟。taint为0不等于没有warning，dmesg before/after无新增行与taint结论是两件事。

## 阶段结论与下一步

本批达到E2局部替换机制及首次再用代价的阶段验收，E2阶段收尾。暂不增加board扫描、不改Linux/BIT、不为这批重新上板。论文当前是机制证据层补齐；E2缓存替换与已有E3共享服务阻塞分开，不证明所有共跑慢都由此二者造成，也不把E2现象本身包装创新。

下一主线是等Mac方法462完成回传后，按已冻结规则评估V保护程度相近时A进度、控制开销、稳定性、独立验证。不调换当前参数，不打断在跑流程。本任务未读取方法结果，不宣称方法完成或收益。

E2保留为后续方法的适用边界：如果只限制发请求速率，不能自动保证V缓存记录不被替换；是否需额外缓存保护要先看方法结果，当前不叠加新硬件。只有论文主张需要更普遍布局、精确门槛或复用特征时，才另行设计针对性补测，现不立刻做。

## 数据保留与归因

完整worker原文保留在`/home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/`，未读取方法数据。首次tar -z采集兼容故障只重新取已有record，没有重跑第一项；阶段不是不间断采集，同一boot、工程重复，不是独立重启。正式结论以实际执行的汇总、原始记录和关键源码复核为依据。
## 复算入口与可追溯证据

- [冻结分析器输出](offline-v2/report.zh.txt)及[完整生命周期重放](offline-v2/results.json)。
- [最终逐项表](audit-final/per_run.tsv)、[分组统计](audit-final/summary.json)、[归档与板端核对](audit-final/board-audit.json)、[输入哈希核对](integrity-result.json)。
- [本次汇总脚本](e2_audit_aggregate.py)：仅写指定的新输出目录，不改原始结果。早期audit-draft/audit-v2及DeepSeek草案保留供追溯，不作最终数据来源。

```sh
python3 scripts/research2_results.py analyze out/mac-research-two-bit-20261003/e2 --output NEW_OFFLINE_DIR
python3 out/e2-results-review-20261003/e2_audit_aggregate.py out/mac-research-two-bit-20261003/e2 --output NEW_AUDIT_DIR --legacy-analyzer scripts/research_results.py --hash-archives
```
