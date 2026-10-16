# Codex关键审定，§34

## 设计和源边界
- 独立参数ParentDepth只控制已译址父AR、offset和returned数组及其物理索引/准入；虚拟关联数组、AW环、iDMA in-flight与TC深度保留原配置。`p_index(seq)`取模1/2/4/8，容量守护不大于ParentDepth；重置只写存在的物理数组元素，虚拟关联仍重置全部8。
- `s_ar_ready_o`依据对应flow活跃与pa-done额度，没有未握手覆盖。物理输出首次offer后child_q保持，`busy_q`一子片在途，父末RLAST只在返回父最后拍恢复。每流序号mod容量与虚拟mod8独立，不能误用一个索引。
- 源offset来自原virtualAR与真实descriptor关联，物理ID0/4用bit2标流；同bank由BankIdBit=3重分类，不能把bit2说成bank位。下游DDR ID32/36来自completion mux标签。
- `rv_iommu_banked_ctl.sv:246–278`准入guard按bank及实际AW/W balance，不是perflow1槽保证。out修正TC的COMPLETE在物理AR握手后返回IDLE，不应声称TC必定等原RLAST。iDMA RAWCouplingAvail=0，读AR直通和R FIFO反压也不能被误说成R-AW强耦合。
- `natural_fragment_test.cpp:854–1018`实际reset(false)/CONF1、两个真实NEXT_ID→DONE、quiet、源/目的全区字节与guard、virtualAW队列排空；本轮沿用此驱动，不支持把mode改n/p/pn就直接用natural测试。

## 接受和拒绝的推论
- 接受：6次新运行PASS，1/2/4容量与2份复用8容量参照输出CSV逐字节相同；V先最后W17530/20213、A先镜像。当前物理父保存深度1无可观测损失。
- 拒绝：8容量实际内部高水位=1（未观测）、所有队列都可减1、通用死锁自由、当前总系统成本最小、原PN已解释、最终FF/LUT节省、动态独立主创新成立。
- 声明物理父每项98+64+9=171bit；2×(8−1)项减少2394bit，总声明5962→3568，另外统计160bit。未综合、其它队列未缩小。
- AMD2009 Fig10直接讨论IOMMU PTW／设备P2P依赖环及独立VC，不能把发现问题或独立翻译通道当首创；未证明覆盖双W-prefix的精确同式实现。
- 下一判别需在同CONF1/AW几何下比较preTL固定片、postTL固定片，以及必要时preTL预翻译准备强对照；原§32 CONF0不满足该控制要求。此为研究规格，未实现，保留反证门槛。

## 验收脚本修订
- DeepSeek草案初次用dict解析实际argv list导致AttributeError；修订增加pwlast为真实W拍、ID/父子/覆盖核验和复用分类，Markdown顶层键错误再经局部修正。Codex追加同ID父子实际打印次序的验收检查；最终summarize-revised.py退出0、480项条件全通过、6项trace等同为true。没有新增DUT运行。
- Codex最初对worker代码用Delete+Add同路径apply_patch，被工具拒绝（未改该文件）；随后锚定代码围栏提取到summarize-revised.py。不是RTL或实验失败。
- 保存worker完整回复、实际诊断、原始命令及exit status；不重新运行DUT来修汇总格式，不把旧8槽数据计作本轮新运行。
