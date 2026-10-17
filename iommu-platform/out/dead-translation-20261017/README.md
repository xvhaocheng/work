# 主线刷新与死条目实验（2026-10-17）

术语：死条目＝已unmap、尚未失效的驻留翻译；walk＝页表遍历；FQ＝延迟刷新队列；PSCID＝地址空间编号；ORACLE＝不等待的理想即时删除。

**1．主线行为。** 查询固定Linux [`58161ead29e5`](sources/linux/findings.md)：普通空槽4KiB map不刷新；strict清PTE后按地址＋PSCID失效，再发一个PR=PW=1的IOFENCE并等待。逐地址条数为0或超过512时退回本PSCID整域，但支持S时优先范围命令；非叶改动且无NL也退回。FQ按本PSCID整域刷新，每CPU256槽／255条，入队前满或首次入队后10ms触发，刷新完成前IOVA不交回分配器。RISC-V策略选项默认strict；若DMA-IOMMU未编入，core默认IDENTITY，不能宣称默认已运行FQ。

本轮为启用DMA-IOMMU的条件仿真：原版16项PLRU＋既有必要修复，真实双iDMA，A64／B256次4KiB COPY；同域／异域各跑且数字完全相同。表中为**完整A操作／完整B DMA周期**，A包含最后unmap。FQ按50MHz、HZ=1000等效10ms为500,000拍；128条入队后只触发一次timer，最终排空均为501,475拍，单列且不冒充A的API等待。仅测试副本补PR/PW等待，以DDR RLAST/B为模型全局点；没有运行CPU/Linux或板测。

| 模式 | B足迹4页：A／B | B足迹8页：A／B |
| --- | ---: | ---: |
| 无失效M0 | 81,739／199,740 | 86,639／204,875 |
| ML-strict | 164,845／204,205 | 144,795／209,940 |
| ML-FQ | 81,739／199,740 | 86,639／204,875 |
| ORACLE | 81,739／199,740 | 86,639／204,875 |

**2．A为什么慢？** 上一轮的细粒度代价来自**(b)并发交互，(a)执行拍数差为0**：A单跑三种失效均62拍／条，IOTINVAL均1拍；异域8页共跑时A多19,512拍，其中等待整轮IOFENCE完成＋37,854（命令取数＋38,503，其余−649）、PTW活跃−8,503、载荷读队列等待0、其余COPY−9,517、配置−322，恰好相加；失效／fence解码增量均0。单跑差0、共跑差19,512，故此差分中(a)0%、(b)100%；占用分解不等于逐项可回收收益。[源码行号与逐命令诊断](diagnosis/results.md)。

**3．死条目污染不显著，判断门槛未通过。** 占比以16槽为分母，采样至双方工作完成，排除之后空等timer；表中为平均／峰值百分比。

| 模式 | 4页死占比 | 8页死占比 | B walk：4／8页 |
| --- | ---: | ---: | ---: |
| ML-strict | 1.130／6.25% | 0.784／6.25% | 0／83 |
| ML-FQ | 46.819／50% | 36.028／50% | 0／65 |
| ORACLE | 0.004／6.25% | 0.004／6.25% | 0／65 |

ORACLE单拍unmap→清除也计入采样。FQ虽有大量死条目，B却比strict快2.19%／2.41%；ORACLE改善均为**0%**，四场景都未过双3%门槛。原版PLRU选victim不看valid，删死条目不会自动改选空槽；四对FQ／ORACLE关键服务事件逐拍一致。结论限于本实现／负载，上一轮walk减少不能据此归因于死条目清除。**停止：未做第四部分检索和第五部分提示原型。**

[归一化图](analysis/final/B_completion_normalized.png)／[PDF](analysis/final/B_completion_normalized.pdf) · [全部数字与门槛](analysis/final/gate.json)／[CSV](analysis/final/results.csv) · [原始数据](runs/final-28/)／[压缩追踪与校验](runs/compression.json) · [测试副本diff](runner/changes.diff)／[构建和复现](runner/README.md) · [28组回归日志](analysis/final/validation.log)／[逐项复核](analysis/final/validation.json)。失败预检保留在[initial-four](runs/initial-four/)，未混入最终结果。
