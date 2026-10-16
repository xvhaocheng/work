# 部分W前缀与AW准备：完整因果裁决（2026-10-14）

本轮按**准确缺项**结束：完整因果、输入寿命和费用接口已交付，未建立相对冻结普通规则的完整任务收益，不准入microbenchmark。future-AW包络/F1/完整准入家族保留；原关闭分支不恢复，论文未完成。

actualgoal1791959885，无额外token预算。用户恢复后实际主席gpt-6.1-sol/ultra/native有效475000，root独自；之前medium/blocked记录原样保留。没有新实验、模型、观测、构建、RTL或板测。

## 问题、进入与结束条件

唯一问题是：B的数据L先走，或A当前PTE P先走，经过原AW准备、两份真实读写和各自B以后，是否仍有完整任务差异。选择点在L新下游首次VALID前，WAIT只到同P的shared AR接受；原held、安全/同ID、hit/read-admission、容量与完整walk先行。

进入须B当前job累计两AW output reservation、A累计0、正确job/config/L/P/native许可和原source有序前驱；不是把reservation当accepted AW。完成允许源合法完整差异、具体统一普通覆盖或准确缺项，本轮交付第三结果；没有借source-only去承诺真实收益。

## 决定性修正：源侧AW不等于input AW

初稿漏掉了原4DMA upstream mux默认SpillAw=1、两槽spill，误写第二AR必须等input demux接受第一AW及B raw WLAST。

ultra亲核后，正确关系是：coupled legalizer第二AR需第一AW在**DMA master源侧**握手；原两槽spill可先接受它，input demux仍因B raw W open阻挡。因此第二AR可能早于B raw WLAST生成，decouple_rw=0也不能单独证明它被挡。旧稿跨端点不等式已撤回，原文及指纹完整归档。没有改RTL/配置，也没有用新缓存修补机制。

原文：peripherals:1228–1252的mux不覆盖SpillAw；CVA6 axi_mux:47–54/259–271默认AW spill；common_cells spill/flushable两数据寄存器；backend:472–473/811–858；legalizer:452–465。原private AW单槽与shared spill是不同寿命/所有者，普通对照也能利用它们。

FullBuffer不核实际accepted decouple_rw/deburst；live config也不能代替job opt。A同bank写不能用N穿过自己的active read P，须完整slow response/TC physical AR spill接受再选择后续AW。raw WLAST释放、output WLAST、独立B和真实退休仍分开。

## 原AW顺序与合法前驱

若原供给使B.AW1先于当前A.AW0进入原承诺AW序列，两个B AW可完成TC准备，而A AW由spill接住后在input demux等待。A的读headers可依原生成/FullBuffer/N配置形成P/L相关前缀；本轮未取得自然accepted snapshot、hit资格和逐拍原供给，只有条件接口证书。

相反，在同current-A-ordinal、normal256beat/no error-MRIF路径下，若A.AW0在该序列排在B.AW1前，B.AW1要等A的首raw WLAST。原bank WFIFO16＋target W spill2仅容18beat；256beat的raw WLAST之前至少238beat已过output W，必已有A output reservation。因此这种前驱不能形成B累计2/A累计0。

这个有界必要条件不否决deburst、换job/后继等整个家族，也不允许人为迟启动A来造B2/A0。详细条件和所有原文见[入口与前驱](../out/aw-barrier-causal-20261014/root/entry-and-predecessor.md)。

## 完整链、普通对照与费用

两策略都保留完整P walk/TC read/spill、AW源侧→input→target→output的不同事件、两个AR的生成/服务、真实数据到raw/output W、每job B/lastmetadata/RESP/fault/退休及原后继供给。不能以P先返回、单个写门槛提前或R-last代替完整任务。

RR/currentDS+RR/earlyPause/owner/cheapDebt/dualGap及明确current W owner优先已在新推导前冻结。后续PTE准备被L遮住的条件手推中，a>g/k>g且同端点eta>=k时dualGap也WAIT；这只能确认该理由的动作匹配，不能证明完整收益或全家族覆盖。AW门槛遮蔽同样只接受局部因果，不用局部结论关闭全族。

费用逐项包含所有不同AW端点/job/option/spill关联、生成/接受/准备/消耗进度、完整history正常资格、T/K/M时间/类/在途/epoch、读取/更新端口、包络算术/max/sum/tie、公平/held/快照/选择延迟、必要CDC和原CPU/poll/STOP/drain。普通可删未用观察。原68bit仅组件小计；源侧两槽spill和private单槽都是继承容量，读取/接口仍收费。完整总bit/port/cycle/area/Fmax/netgain未测为null。

## 准确缺项与下一门

缺项E1–E6：原accepted选项和有序AW/packet/job/P快照；同端点normal完整walk/history；策略相关共享R/W/PTE/AW/B反馈及独立B；原固定工作/后继/错误/quiet；全部实现数字费用；相对统一冻结普通的完整max/sum差异和相邻条件。

因此不生成正反/identity-swap/interleaving四条件，不运行model或bench。本轮没有全任务性能负例/独有正例，也没有新published-prior/创新贡献裁决；只纠正错误边并交付完整必要合同。

后继必须有新的同源原始证据包，或明确增加合法能力的新机制修订；不自动扩写本纸面图或再次搜源。当前source-only范围保持，新的观测/model/bench需后继明确合同。8/9月成果以及R1/R2/R107/C112、literal current-full-AW等关闭结论原样保留。

全部证据/原稿/冻结对照/费用/执行与运行记录见[导航](../out/aw-barrier-causal-20261014/README.md)。稳定裁决与五文档同步、新交付/需求核验完成后，才写实际goal complete回执；原立题窗口累计，本轮不代表论文完成。

本轮新交付核验已pass：21源文件/51编号原文、冻结规则、纠正与有限裁决、费用空值、实际ultra恢复、原稿归档和五文档保留同步、导航及逐项需求处置。该核验不是性能测试或新模型。实际complete回执在核验后生成。

同goal实际complete；工具回执188,339 tokens、27分50秒，无额外预算。计量字段停在原blocked值，未反映恢复段，不将其当全轮用量；完整记录见accounting-note.json。五文档及交付核验在complete之前完成；论文未完成。
