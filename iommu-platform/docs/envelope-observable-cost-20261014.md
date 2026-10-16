# 便宜完成包络：可观察输入、W顺序证书与最小成本（2026-10-14）

本轮形成源码裁决：**停止将父轮两score直接落地为“两份当前copy已完整写就绪”的在线版本；保留部分W顺序证书和future-AW-aware修订方向。** 不关闭F1、完整准入家族或论文问题。无新模型/实验/观测，不准入microbenchmark，未测收益/面积/Fmax为null。

actualgoal1791952824，无额外token预算，半工作日范围；root独自gpt-6.1-sol/ultra，运行记录有效上下文475000，Codex-only，0新增/恢复席。已获用户明确选择“仅源码与推导”。

## 问题、进入条件与完成合同

父轮已补齐完整R/W/B完成链，给出条件两score，并由O-envelope-derived统一覆盖该纸面域。此次只核其在线可观察输入、真实W顺序、所有权寿命和最低完整费用。选择点仍在physical payload AR首次下游VALID前；held/native安全、原容量/ID/完整walk/read-admission和后续供给先行。

进入推导要求输入能绑定当前存活packet/job/P；完成要求可观察非平凡选择及相邻条件，或者准确证书/缺项与停止范围，完整成本和五文档同步、新交付检查后再actual complete。实际收益或论文完成不是本轮完成条件。

## 决定性新源事实

1. 输出W FIFO在AW首次转发VALID时预留，早于AW实际接受；只存bank index。原8entry，WLAST接受即释放，B另管。因此不能把它称为已接受AW队列或整job写就绪证书。
2. 输入demux在当前bank raw W reservations未清空时阻止另bank AW转发。它是原路由避免W死锁的结构约束；不能用外层w_unfilled推测，也不能误说必须先AW handshake才W。
3. DEFAULT写数据来自同job实际读数据。若两当前copy的raw AW都已真正接受且physical AW已准备，最后AW所属bank之前的另bank必须已完成全部raw W、消耗完整R。故在两job/normal/普通P限定下，它不可能同时还留下其中一份copy未首次VALID的必要L和另一份copy仍必要P。direct current-full-AW入口全回退。

逐行证据：banked_ctl:155–172/236–310/409–452；IOMMU axi_mux:282–332/350–367、CVA6 axi_mux:208–256/276–290；两份axi_demux_simple:140–198/233–253/410–437；DEFAULT transport:238–360/390–454。[冻结原文与SHA](../out/envelope-observable-cost-20261014/root/source-snapshot.json)／[编号摘录](../out/envelope-observable-cost-20261014/root/source-excerpts.md)。不重做旧52 timing或FPGA绑定；两个mux副本不同，只分别核本轮语义。

## 正向产物和停止边界

在原normal aligned DEFAULT4KiB/64bit、deburst=0选项下，每job两份256beat AW。每bank2bit记录当前job首次AW预留数0/1/2/invalid，共4bit，可证明“B已预留所有W槽在未来A槽前”；一般变burst替代为每bank10bit累计0..512与invalid，共20bit。无新queue/credit/payload，未实施，不声称总面积最小。

部分证书不证明未来A AW已就绪。合法read-P分支可因FullBuffer与native hit继续存在，但A写许可还受原B raw WLAST、A header/原队列、自己的write NormalTL/hit/physicalAW准备与credit约束。这些项与策略有关；跨job真实B尾也独立。literal两score缺它们时不能沿用共同H/z与先验CA=max。

父轮AW前提是“数据需要时可合法支持”，不是“任何预测都必须当前全部AW已接受”；本轮直接证书定理只否决明确的current-ready实例。依靠历史预测未来AW本身合法，须明确所预测项及费用；不因为当前非事实就关闭预测全族。未取得自然prefix或有效history，不虚构正例/收益，也不扩写同纸面域。

## 所有权与完整成本

physical AR spill/header/select可比TC req metadata活得更久；慢P req_data至真实响应握手保持；frontend33bit native ordinal可在sole current job寿命内读取，但obs_issue_id只在issue事件有效。legalizer剩余length只是未发请求，transport8bit只是当前burst；不能代替已拥有真实R/W/B余量。NO_ERROR_HANDLING done不能证明RESP正常或byte正确，正常history必须另核真实RESP/fault。

三组每bank10bit的R/rawW/输出W进度共60bit，加4bit窄域prefix、2bit坏资格、2bitpending/held，列出组件小计68bit。**小计不是完整新增总量或测得FF**：B/AW记账、T/M/K时间/类/在途记录、不可复用关联、valid/epoch/fair/快照和选择pipeline仍参数化。普通RR/currentDS+RR不被迫加服务history，owner/cheapDebt/dualGap可以删未用W/B字段。所有读取、更新、算术、选择延迟、必要CDC计费；完整bit/port/周期/FF/LUT/BRAM/area/Fmax/CPU/应用/netgain均null。

完整在线卡、input/lifetime map、证书定理与cost表见[导航](../out/envelope-observable-cost-20261014/README.md)。父O-envelope-derived对窄域统一覆盖有效；本轮没有新增发表前作覆盖、事后拼接普通或全族关闭。

## 下一门与验证范围

下一步是明确机制修订：把input W owner释放、peer AW完整准备与两任务独立B完成纳入一个小包络；若形成新的源合法选择差异，再核同源完整历史/成本和原固定工作量。缺这些输入不自动安排观测/model/微benchmark；用户已选source-only继续有效。通过才另立有限microbenchmark合同，后续最多四条件、一个GPU/AI共享翻译工作流、RTL/FPGA关键验证。

0新旧科学实验/模型/观测/trace回放/build/test/生产/板测/容量压力/旧席或关闭分支恢复；8/9月成果和R1/R2/R107/C112等关闭结论保持。新核验只检查本轮源码快照/摘录、裁决范围、成本字段/空值、交付JSON/链接与五文档归档同步，不是性能测试。

完成状态以[root goal真实回执](../out/envelope-observable-cost-20261014/root/goal-completion.json)为准；本轮结束不代表论文完成，原立题窗口累计。

交付核验：已pass，22源文件/51编号摘录及本轮范围/成本/元数据/五文档/导航一致性通过；首次检查在报告自引用尚未创建时失败，修正创建顺序后只重检导航。无科学实验或模型回放。

actual goal已complete；真实工具回执260,015 tokens、69分55秒，无额外token预算。五文档同步及交付核验在complete之前完成；本轮不是论文完成。
