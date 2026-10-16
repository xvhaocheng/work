# 部分W前缀＋策略相关AW准备：最终有界机制卡

actualgoal1791959885；用户恢复后主席实际gpt-6.1-sol/ultra/native有效475000。原medium/blocked记录与初稿完整保留。此轮源码与手推，0实验/model/观测/RTL；**按准确缺项结束，未接受完整性能差异**。

## 问题与唯一动作

在B存活physical payload AR L尚未下游首次VALID、A当前完整read walk拥有P时，比较ADMIT L与WAIT同一个P的shared-out AR接受；之后释放L，重新按原规则服务，不延长到整个walk结束。目标为相同当前任务及原后继供给的完整R/W/B正常退休max，再sum、年龄/RR。保持一个PTW、IoTLB16、原FIFO/credit/fullsource/same-ID/native hit/held/error路径与容量。

B当前job的output AW reservation计数2、A计数0只证明已预留W次序。它不证明DMA端AW握手、input demux接受、AW全准备、AR全释放或job完成。须同时验证原upstream AW序列、packet/job/P所有权、实际accepted选项和当前合法head。

## 新增能力及本轮source修订

候选仅以完整任务完成包络选择，不新增prepare/缓冲/Walker。必须区分：private AW slot → DMA source AW握手 → 原共享两槽AW spill → input demux AW接受 → TC普通翻译与target spill → output AW reservation/接受 → 两级W端点 → 各自B/退休。

关键纠正：前稿`emit(A.AR1)>=accept_input(A.AW0)>=raw_WLAST(B)`撤回。coupled legalizer第二AR只依赖第一AW离开private slot的DMA源侧握手；原upstream AW spill可以在input demux仍阻挡A时接受它。**不因decouple_rw=0人工添加raw-W屏障，不给普通控制施加单槽/单child限制。** 正式边与进入条件见entry-and-predecessor.md、causal-chain.md。

## 在线安全与输入合同

- 原held VALID/已承诺选择继续至握手；新gate只能阻挡尚未首次VALID的L，不能撤回原source AW/AR、改ID或授权W。
- 只使用存活spilled header/select和已accepted job意图；源侧header前进后不能用新header替代旧spill，也不能用event-only issue_id当当前job。current slow req_data与P自身完整walk保持。
- decouple_rw/deburst来自该job已accepted并保持的legalizer选项；FullBuffer只核容量/正常copy/length/alignment/sole资格，不能代替它们。两分支均按原选项处理，不切换配置造收益。
- 未发AR/AW可属于已accepted job剩余意图，但还不是合法可选head；只能预测原生生成/翻译路径，不能提前发送或给未来PTE/DDR/B真值。
- 需要的关联、normal history/class/epoch、原供给或服务包络无效/未成熟/overflow时回退currentDS+RR。无新免费保留槽。公平约束优先：持续另一合法头被越过一次，下一次服务它；公平状态/选择延迟收费。
- 预测不能减少walk或改变数据/错误完成。实际RRESP/BRESP/fault单列，NO_ERROR_HANDLING的done不足以证明正常字节与RESP。

本卡是完整能力与安全接口合同；缺必要数据时如何回退明确，但没有假称已实现完整数字预测器。

## 接受、停止及未解项

接受：真实多端点AW/AR生成链；原upstream两槽spill及private单槽的不同寿命；同bank写不能穿过自己的active read P；raw W/out W/B独立；完整原后继不能free STOP。

撤回：前稿跨端点不等式，以及据此从coupled选项推出全部后续AR等待B raw WLAST的推断。

不接受：符合原供给的自然入口、同端点normal history、完整ADMIT/WAIT收益、统一普通覆盖或独立创新/面积/Fmax。历史父窄域O-envelope-derived覆盖保留，关闭R1/R2/R107/C112和literal current-full-AW版本保持；此轮不关闭future-AW包络/F1/全族。

结束理由：source接口给出必须边，却没有实际选项/有序spill快照、共享R/W/B反馈/有效history、固定原供给和完整费用。按合同“准确缺项”结束本轮，不扩写更大预测器或正反矩阵。microbenchmark未准入，论文未完成。
