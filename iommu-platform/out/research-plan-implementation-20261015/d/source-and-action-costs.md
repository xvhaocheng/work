# D首批：来源与动作费用

2026-10-15。范围固定为**quiet边界，为下一次COPY任务选择P或PN**。本轮只有原件读取、记录比对和文档交付；0新实验/构建/板操作/goal/外部模型。全局文档、生产代码、旧证据和paused MIG均未改动。

全部必要源的完整SHA256、精确摘录、6份日志的完整producer JSON及原件位置保存在[assessment.json](/home/xvhaocheng/work/iommu-platform/out/research-plan-implementation-20261015/d/assessment.json)的`source_manifest`、`source_excerpts`、`record_evidence`。SHA标识本次读取的字节；当前源码SHA不等于已经证明旧日志由相同源码构建。

## 1. 三种证据身份

| 身份 | 本轮可复用内容 | 不能推出什么 |
|---|---|---|
| M0模型完整COPY | 实际两路iDMA、banked CTL/TL、生产DS_if/delayer/xbar，末端为C++行为DDR；双方固定64次4KiB、自然完成并quiet | 不是MIG/CVA6/Linux完整时序或真实应用；没有已实现的硬件观测成本 |
| 历史实板 | P/PN及gap原始record、配置/generation与窗口计数；用于确认接口历史和问题动机 | 旧V窗口不等于A固定工作量全完成；不能拼接模型或E3形成H-D状态对 |
| 应用完整费用 | 当前仅有待测清单 | 未给出真实consumer、cache同步、map/unmap、资源归还/free的全链时间与预算 |

模型结构见[system_top:5](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_system_top.sv:5)；历史口径见[结果文档:7](/home/xvhaocheng/work/iommu-platform/docs/workflow-g0-m0-results-20261010.md:7)；板端缺项见[历史边界:127](/home/xvhaocheng/work/iommu-platform/docs/composition-history-isolation-20261007.md:127)。

## 2. 实际动作与冻结guard

- 模型P/PN共用`PolicyEnable=1`构建：P为`nonblock_enable=0, policy_partition=1`，PN为`1,1`。本轮只读其接口，不运行包装器：[C++:3359](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:3359)、[构建参数:383](/home/xvhaocheng/work/iommu-platform/tests/test_nonblock_actual_dma.py:383)。
- 板端P=2、PN=3，配置为`DID<<8 | mode`。quiet且观察窗口未armed/active时才能提交；成功配置同拍clear、generation递增并读回核对。恢复也要先等quiet：[驱动:2283](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2283)、[恢复:2343](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2343)、[RTL:155](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_composition_control.sv:155)、[RTL clear:182](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_composition_control.sv:182)。**不假设运行中切换，也不假设切换免费。**
- 保留SYSTEM、V/A各64、每copy4096B、V/A gap0、warmup256、PTW延迟31、A页环仅1/8、`read-admit=none`；首批主对照CPU关闭、固定映射、launch-order=av。不能为寻找正例放宽guard：[C++限制:3322](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:3322)、[原登记:4](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/baseline-register.json:4)。
- gap暂不进首批菜单。板端虽有0/1000/4000及写回读接口，但M0完整模式只接受0：[驱动:1375](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:1375)、[驱动限制:2798](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2798)。

## 3. 原理级计时与全费用差别

M0在配置、warmup和排空后才取共同`t0`，随后计首发供给到双方done及协议quiet；逻辑ready由`t0/上次done`重建。正常路径不STOP A：[配置前后:2288](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2288)、[t0位置:2327](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2327)、[自然完成:2906](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2906)。`T_return`不是应用free时间，离线校验和之后的寄存器复核也不在其中。

| 费用 | 现有证据 | D部署全费用仍需什么 |
|---|---|---|
| 当前COPY内供给、DMA读写、翻译反馈与协议收尾 | M0硬件pulse/CSV与双方完整JSON | 保留该闭环和后台约束，不能拼局部最快轨迹 |
| 决策/观测/配置、quiet等待、cache clear | 接口及顺序已知，时间未知 | CPU/MMIO、状态维护、clear损失与观察造成的争用 |
| warmup、历史采样、一次校准/试选、切换与恢复 | 被主计时排除或未实施 | 按真实重复任务来源摊销，不能免费重复同任务 |
| map/准备/cache同步、consumer、释放/free | 当前COPY模型无应用合同 | 从共同可用输入到成功consumer和安全归还的完整计时 |
| 新观测硬件、频率、面积与时序 | 未实现/未测 | 模型日志费用不能当作板端零成本 |

上述未知费用均未估成0；本轮不预测净加速。

## 4. 实际检查旧记录：H-D没有通过

令`δ = T(PN) − T(P)`，正值表示PN更慢。直接读取6份日志各第21行完整JSON；全部producer字段与`results.json`对应记录一致，汇总仅多`case_id`。各路均issue=done=64、读写各262144B、AW=B=128、err=0、未STOP。这是旧原件核对，不是重新运行验收器。

| 同合同条件 | P的(T_V,T_A,T_return) | PN的同向量 | δ |
|---|---|---|---|
| A1/FIFO | (73539,72965,73544) | (73539,72965,73544) | (0,0,0) |
| A8/FIFO | (86594,86082,86601) | (90283,89771,90292) | (3689,3689,3691) |
| A8/DS优先，模型干预 | (76738,76164,76745) | (76738,76164,76745) | (0,0,0) |

入口：[a1-p.log:21](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/a1-p.log:21)、[a1-pn.log:21](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/a1-pn.log:21)、[a8-p.log:21](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/a8-p.log:21)、[a8-pn.log:21](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/a8-pn.log:21)、[a8-p-ds.log:21](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/a8-p-ds.log:21)、[a8-pn-ds.log:21](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/a8-pn-ds.log:21)。

**没有PN获益一侧，也没有完整净收益的反号状态对。** A1/A8还是事前已知页环配置，不能人为隐藏成O0不可分。A8虽普通慢路径miss均31、walk返回均93，fast为0/14、PTW活跃为16483/20172，完整O0并不相同；这些更是动作后的结果，不能泄漏给本次事前选择。

DS优先只是C++服务规则，保留同ID及held VALID，既有裁决已关闭用该差额支持新门控的分支：[干预:680](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:680)、[原裁决:45](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/root-result-acceptance.json:45)。其结果不当成部署可免费调用的动作。新D状态对继续待证，不拼接E3、旧板PN和旧窗口模型。

## 5. 执行隔离

实验包装器会构建；离线分析入口会覆盖旧结果/图片。本轮均未调用：[包装器:439](/home/xvhaocheng/work/iommu-platform/tests/test_nonblock_actual_dma.py:439)、[分析器:567](/home/xvhaocheng/work/iommu-platform/scripts/analyze_full_batch_g0.py:567)。未来任何模型修改、运行或硬件工作均另立任务；本交付不恢复MIG或旧门控。
