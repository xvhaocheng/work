# A2真实SVA实施交付

当前平台代码、软件构建与分层功能检查已完成；native Linux/DMA/cache/完整生命周期尚未上机验收。Hero另有专用平台gate实施缺口。完整时间预测未准入，原MIG暂停、旧实验不重跑。

- [实施报告](../../docs/a2-sva-implementation-20261015.md)
- [root接受结果](root-result-acceptance.json)
- [后继决定](next-decision-contract.json)与[完整native功能合同](validation/full-native-contract.json)
- [Linux补丁](patches/linux-a2.patch)、[平台补丁](patches/platform-a2.patch)、[前后源码SHA](patches/source-manifest.json)、[只读适用检查](patches/patch-checks.json)
- [当前软件完整构建锁](kernel/build-lock.json)、[最终输出](kernel/final-output-manifest.json)、[独立输出副本](kernel/artifacts)
- [Hero固定版本/配置](hero/config/version-lock.json)、[Hero交付](hero/README.md)
- [16项最终任务检查](task/locked-pte-batch/app/results.json)、[首批10份consumer](task/host-checks.json)、[实际驱动20案](task/locked-pte-batch/lifecycle/results.json)、[真实页表源码析构47项](iommu/host-test/result.json)、[生产CQ本体4案](model/cq-check-results.json)
- [root关键源审查](root/source-review.json)、[源码/产物核验](root/source-output-verification.json)、[交付核验](root/delivery-verification.json)

kernel与hero的日志/命令分别留在各自目录。两类平台和各验证层级不可混用：host-model不是DMA，CQ本体不是整平台，编译不是内核运行。当前新接口默认关闭；此交付不包含FPGA构建、加载或板端操作。源码增量基于as-found脏树；完整tracked overlay与untracked快照另存kernel，保留原有Linux贡献。

root负责机制、当前任务驱动、关键源码审查和最终接受；独占Sol执行席负责IOMMU、应用/功能夹具及Hero软件链。Codex-only，无DeepSeek/Claude。
