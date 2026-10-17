# Baseline100：已完成的结果、固定源码与配套镜像/BIT

**正式100次已全部完成**：[campaign](results/campaign.json)为`completed`，[启动任务结果](results/launch-result.json)为returncode 0。两种DS配置、五条件各10次；历史G1的独立9次不计入这100次。完整配对见[manifest.json](manifest.json)。

| 实际阶段 | 样本 | BIT | 原始结果 | 启动身份 |
| --- | --- | --- | --- | --- |
| DS50前段 | 25 | [ds50.bit](artifacts/ds50.bit) | [records](results/ds50-first/records.json) · [review](results/ds50-first/review.json) | [load](results/ds50-first-load/load.json) |
| DS0中段 | 50 | [ds0.bit](artifacts/ds0.bit) | [records](results/ds0-middle/records.json) · [review](results/ds0-middle/review.json) | [load](results/ds0-middle-load/load.json) |
| DS50后段 | 25 | [ds50.bit](artifacts/ds50.bit) | [records](results/ds50-last/records.json) · [review](results/ds50-last/review.json) | [load](results/ds50-last-load/load.json) |

三个子run均passed，实际ABA顺序、三个boot完整保留，无新增失败或补采。每阶段内按区组轮换Solo/A1/A8/A16/A64。后续默认AB：一套50次完成再切另一套50次；同payload不重写SD。当前有效DS50启动可通过runner的`--initial-load-manifest`复用[最后load回执](results/ds50-last-load/load.json)，前提是板仍处于该boot；不会因此启动新实验。

## 结果展示

硬件ms为原始`observer.time.cycles / 50000`；DS50合并前后原始样本后取每条件10次中位数。下表不是额外验收。

| 条件 | 每DS样本数 | DS50硬件中位数ms | DS0硬件中位数ms |
| --- | --- | --- | --- |
| Solo | 10 | 56.54995 | 56.53868 |
| A1 | 10 | 107.00204 | 107.09009 |
| A8 | 10 | 154.93961 | 118.22441 |
| A16 | 10 | 162.78033 | 121.73598 |
| A64 | 10 | 168.70064 | 124.60276 |

DS0的A8/A16/A64中，V IOTLB miss及S1 PTW快照中位数均为2，S2 PTW为0。HPM由软件前后采样，非原子、非硬件时间窗口；不能把它写成V硬件窗口中的精确miss/PTW计数。详细口径与原始记录均保留。

## 代码、镜像与BIT配对

- [source.json](source.json)与[完整source manifest](metadata/source-manifest.json)：CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`、IOMMU F5 `85b157ebe71a623e326e51fff7b7126fe77e8eb9`，平台锁提交 `885ed0a7ea90ed42dae2b7a2dadfdd0acba15273`，Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e`。
- [共用fw_payload.bin](artifacts/fw_payload.bin)已有SHA `2de1e155…`，内核 `6.5.0-rc2-ispass2027-pathv3`；实际写SD用它。[Linux Image](artifacts/Image)另存内核组件。
- [DS50 BIT](artifacts/ds50.bit)已有SHA `580b3401…`；[DS0 BIT](artifacts/ds0.bit)已有SHA `e9b6554f…`。两者50MHz、WNS +0.177ns，完整原始报告见[delivery](metadata/delivery.json)。
- 版本锁、profiles、最小bundle与完整patch由source.json关联，重建说明见[BUILD](metadata/BUILD.md)。
- 本轮执行来源：[启动时协议](metadata/protocol-at-launch.json)、[原ABA runner](metadata/run_baseline100.aba-as-dispatched.py)、[前两阶段collector](metadata/collect.baseline100-as-dispatched.py)、[最后阶段collector原字节](metadata/collect.ds50-last-as-executed.py)。最后collector仅扩展phase接口，原阶段及测量口径保持。

历史G1结果与失败恢复链在旁边的[g1-20261017](../g1-20261017/)。`artifacts/`是本机实际副本，不进Git；metadata和结果使用相对入口。此收尾只引用既有hash和回执，没有新hash、测试或板上操作。
