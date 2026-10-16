# SPDK有限计数、完成驱动供给：无时钟语义原型

**仅核查应用回调层的计数、身份、补交与排空语义。所有事件均为 synthetic，未运行真实 SPDK。** `N=4,Q=2` 是小规模验证常数，不能作为来源工作负载、设备队列深度或性能参数。此结果不准入性能模型，不提供时间、IOVA/PFN、设备绑定、P/PN效应、共享IOMMU干扰或反号收益证据。

冻结输入：[semantic-prototype-contract.json](../root/semantic-prototype-contract.json)。普通实现由 Codex 执行，受当前共享IOMMU任务 Codex-only 约定约束；请求 Sol/medium，实际运行模型元数据未知，不以请求值冒充运行记录。没有恢复用户删除的旧研究方法文件。

## 源码依据与投影边界

- `../../real-dma-scenario-screen-20261012/network_storage/sources/spdk_nvme_perf.c:1470` 的 `submit_single_io`：成功提交增加 queue depth 与 `io_submitted`（1522–1528）；达到有限计数后进入 draining。原始源码 sha256 在冻结合同中。
- 同文件1533–1573：回调先减 queue depth，增加 `io_completed`；draining 时释放当前 task/buffer，否则同 task 补交。
- 同文件1581–1601：错误回调仍执行 `task_complete`。计数完成不等于成功。模型单独维护 `successful_completed`。
- 同文件1843–1859：退出时继续处理在途完成，直到排空。
- `../events/nvme_spec.h:1434` 位域为 P[0]、SC[8:1]、SCT[11:9]；4429–4433只以 SC/SCT 判断错误。因而 raw status `1` 是 phase=1 的成功状态。

投影为一个worker、一个namespace、纯read、metadata_size=0、无时间上限，所有submit均成功。初始提交Q个task，各自有稳定的不共享opaque buffer；同task新一代仅能在前一代完成后提交。模型没有地址、长度或设备完成延迟。没有提交失败、continue-on-error队列、driver retry、错误恢复、hotplug或低层NVMe重试；它是应用callback边界投影。普通错误样例注入在已drain的最后回调，故不假定错误后重试。Generic SC=0x0B触发源代码特殊提前drain，回放器明确拒绝该投影外路径。

## 可执行内容

- `prototype.py`：每一步从当前active task中分支选回调，并立即据状态生成同task补交或free；N次成功submit后不再补交。
- `replay.py`：独立事件消费者，不导入生成器；重建generation、buffer、queue、drain、完成与成功计数，核查初始化、紧随回调的动作、终态和成功声明。
- `verify.py`：生成并验证N4/Q2全部8种active回调选择；其数量是接口允许的过近似，**不声称8种都能发生于真实硬件**。每条4次提交、4次成功完成、峰值Q2、最终释放2个buffer。
- `synthetic-order-witness.json`：两种回调顺序产生不同的补交task序列。替代动作必须重新推进状态，不能无条件复用固定arrival列表。
- `synthetic-error-SC.json`与`synthetic-error-SCT.json`：分别在SC或SCT非零时保持4次`io_completed`而只有3次`successful_completed`，整段成功证据为false。
- 六份`synthetic-negative-*.json`：提前复用、drain后多提交、两类错误伪标成功，以及两类正确标错却伪称整段成功。独立回放器全部拒绝，原因见`verification.json`。
- 额外边界N1/Q1、N2/Q2、N5/Q1、N5/Q3只验证程序边界，不是工作负载校准。

从`/home/xvhaocheng/work`执行（Python标准库）：

```sh
python3 iommu-platform/out/d-spdk-contract-20261015/model/verify.py
python3 iommu-platform/out/d-spdk-contract-20261015/model/replay.py iommu-platform/out/d-spdk-contract-20261015/model/synthetic-success-example.json
```

实际已执行：主检查PASS，8条成功轨迹、6个拒绝变异、2个错误计数案例；独立CLI成功示例退出0，返回4提交/4完成/4成功、峰值2、释放2。运行时间、输入hash、请求配置与实际元数据限制见`run-receipt.json`。验收仍由root独立读代码与执行；此处未写root接受结论或正式状态文档。
