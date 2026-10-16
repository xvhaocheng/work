# 控制位置方法的首个完整任务试验

2026年10月15日。**新固定任务完整执行成功，但没有触发当前头竞争，不能作为入口DS优先的收益正例。** 两份程序的所有DDR事件和完成向量相同；行为FIFO中仍有PTE排在已接受payload之后的实际记录。它支持将控制位置和承诺义务纳入研究，不能证明提前意图接口、独立方法新颖性或FPGA收益。

## 冻结输入与实现

[ACV-2x2合同](../validation/model/contract.json)及其SHA在执行前保存。两路真实iDMA，各两笔正常页对齐4096B；V重复一页，A走两页环，总4copy/16KiB；冷reset、无warm、CPU关闭、N开启、单PTW/16IoTLB/原队列和buffer。双方配置开销在共同任务ready计时中。初始PTE与payload镜像是共同前提，不将host初始化/校验壁钟当作Linux CPU费用。

只有所选测试用IOMMU vendor的四输入DDR mux AR仲裁替换为双模式组件；RR复用原rr_arb_tree，DS遵循current-VALID/F4。AR spill、AW/W/R/B和后端保持。C++中已接受读返回提升代码从私有副本删除，内存服务保持原FIFO及setup31、DS50/50。这是具名行为服务假设，不是MIG时延模型；测试选中的vendor副本也不直接代替board源绑定。

完整任务模型一次构建，RR和DS-first/F4各新运行一次；没有重跑旧M0或旧成功矩阵。已有数据值、源不变、未用页guard、真实issue/done、R/W/B字节与响应、独立watchdog、自然完成和最终quiet校验均保留。[实际构建](../validation/model/build-result.json)／[两次命令与退出](../validation/model/run-ledger.json)／[适配差分](../validation/model/driver.patch)。

## 原始结果

| 指标 | 原RR | current-VALID DS-first/F4 |
| --- | ---: | ---: |
| V完整完成 | 7009 | 7009 |
| A完整完成 | 6497 | 6497 |
| 双方最终quiet | 7019 | 7019 |
| 每路实际有效读／写字节 | 8192／8192 | 8192／8192 |
| 每路issue／done | 2／2 | 2／2 |
| 每路AR／AW／B | 4／4／4 | 4／4／4 |
| 本owner首次offer／接受 | 28／28 | 28／28 |
| 新选择时DS与nonDS同时有效 | 0 | 0 |
| held nonDS遇到DS | 0 | 0 |

时间单位为本条件RTL模型周期，从共同ready起算；不报告统计显著性或实板速度。28个读中8个payload、18个normal PTE单拍读、2个DDTW八拍读。所有R共2082拍、W共2048拍，W实际strobe字节16384，B共8。最终consumer是host离线检查源/目的完整值，不是CPU cache或真实应用计算的计时。

独立新解析器逐项关联owner完整packet与DDR AR，按真实接受序重建R退休，且每个同沿先扣旧R再登记新AR；两条CSV逐字节相同。两个PTE接受时仍有前序payload债务，最大940个64bit R拍。债务属于本行为FIFO且两案共有，**不是原暂停MIG观测的940拍记录**。完整出现时刻和残余值在[pilot-verification.json](../validation/model/pilot-verification.json)的pte_arrivals中。

## 怎样更新研究判断

1. **正确继承的正向结果。** 得到一个从真实reset经iDMA/PTW自然执行到完整完成的新有限任务；当前选择点没有竞争，之后PTE却仍可遇到前序payload债务。入口优先不能改变已经越过其控制边界的部分。方法不能仅凭“PTE有债务”推荐在这个位置增加优先级。
2. **本任务不提供的证据。** 首次offer竞争和held-late-DS都未覆盖，因此没有入口优先的正收益或四项任务级预测通过结论。两案等值也不证明其他来源没有作用机会。
3. **遵守预定停止。** 结束这个具名pilot，不增加页数、工作量、相位、CPU、延迟或压力寻胜例。后继需要新的来源/任务依据或明确分析假设，不恢复旧M0、R2或MIG采集工程。
4. **继续发展A。** 用本例作为“已看到债务但此处无新选择”的有来源边界材料。下一方法产物应能从源和协议推导这种部署决定，并在未参与设计的完整任务上预测；本次只支持这个有限位置判断。

## 协议组件的独立验证

完整任务没有覆盖F4或held，不能据两次PASS声称这些分支全测。另一个小型组件夹具分别测试四次竞争接受后的逃逸、nonDS轮转、held时DS迟到及完整包变更拒绝，结果独立保存于[component-results.json](../validation/model/component-results.json)。这些是普通选择器正确性检查，不增加DMA性能episode或论文科学样本数。

组件共三次构建尝试：第一次因主机Verilator不支持--binary而未编译，第二次改用--cc/--exe后缺少旧版sc_time_stamp链接函数，第三次补齐该接口后构建成功。只有第三次执行四个组件用例：RR-held和DS-F4-held正常退出；held包qos被篡改、非法mode=3分别触发准确的预期fatal。四项预期均接受，两条正常轨迹最终held=0；DS竞争接受顺序为3,3,3,3,0,1,2,0,2,3。[组件执行账本](../validation/model/component-execution-ledger.json)保存失败和唯一诊断恢复，没有重跑完整DMA模型。

根在编译完整任务前还纠正了私有生成脚本的AR定位和保留字问题，未让错误适配进入科学运行。这些工具/适配错误不是RTL性能负结果；所有原始失败与修订留在本轮目录，不改写既有结果。

实际输入清单中82份原文件哈希保持不变；生产RTL/Linux/driver/ABI和板操作无变化。完整费用包括原状态/端口/数据通路及新选择/观察；组件8bit只是一份DS策略编码，双模式、日志和监控为测试专用，不能用作净面积、Fmax或低成本部署证明。
