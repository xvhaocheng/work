# 复现与计时说明

本任务全部由Codex完成；未调用DeepSeek或Claude。实际编译原版IOTLB配置＋既有修复冻结源：CVA6 `ba8cdd10659a777ccff0080bf020be64b5c1e064`，IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`；适配／修复来历见[上一轮清单](../../invalidation-interference-20261017/sources/baseline/manifest.json)。未编译活动研究checkout，不含P/PN、policy_mask、提前命中旁路或DS-first/F4。原source/include/license原字节见[frozen manifest](../frozen/source-manifest.json)，复制核对见[frozen-copy.json](../frozen-copy.json)。

最终实际构建为[build02 manifest](../build/build02/build-manifest.json)，其pre-build身份和[runs/final-28/campaign.json](../runs/final-28/campaign.json)的pre-run身份包含分支、完整commit、相关dirty输入、父子指针、锁及实际源hash。五份overlay增加ORACLE删除和PR/PW模拟等待，均仅`ifndef SYNTHESIS`；[逐文件diff](rtl-overlay/)及[端口](../diagnosis/ports.md)描述准确范围。命令编码为主线单条IOTINVAL＋单条PR/PW fence，每命令经真实CQ/MMIO；CQH须在旧device payload cohort到达模型DDR RLAST/B后才能越过fence。此保守全局点不代表所有真实内存系统的周期。

Linux主线源只作行为依据；并未运行内核或实际IOVA分配器。每次A map取上一轮同样递减新IOVA，保留独立分配集合，FQ成功flush后才释放；单CPU，无其他域刷新。本轮每次unmap一页，最多128个entry，未触发255条满条件；满条件逻辑已实现但本轮没有实测溢出场景。所有页表层预先存在，4KiB leaf的map不替换下级表，unmap不拆表；S/NL/大页例外仅由来源核查，不冒充已测。

DDR全局FIFO，队首先等16拍再每拍返回8B，无读重排；主机PTE写不计CPU指令费用。10ms定时器按50MHz、HZ1000作确定性500,000拍等效，无OS调度抖动。A完整操作从共同t0至最后unmap入FQ／strict失效完成；`T_A`另为最后硬件COPYDONE，`A_final_flush_end`为完整API操作，`fq_drain_end`是之后独立延迟排空。B时间从共同t0至最后硬件COPYDONE。dead采样窗口为`t0`起至`max(A API完成,B DONE+1)`，前沿状态采样；ORACLE unmap后第一沿清valid，单拍也计死占用。victim按每次真实fill的原PLRU选择和此前valid/dead标志记录。

构建与运行均接受新目录，失败即停且保留原结果。在本任务目录运行：

```sh
python3 runner/build.py --from-frozen --cpp runner/invalidation_dma.cpp --out build/rebuild01 --jobs 8
python3 runner/execute.py --stage all --binary build/rebuild01/obj/Vinvalidation_dma_top --build-manifest build/rebuild01/build-manifest.json --output runs/reproduce01
```

分析环境依赖版本沿用[上一轮固定列表](../../invalidation-interference-20261017/runner/requirements-analysis.txt)；生成新分析目录：

```sh
python3 runner/analysis.py --campaign runs/reproduce01 --output analysis/reproduce01
```

最终统计的独立原始追踪积分校验由`python3 runner/validate_results.py`完成，检查28组占用积分、victim标志、wall/命令时延可加分解、payload cohort守恒。`analysis.py`另核主线无map刷新、unmap/入队/释放时序与命令数，门槛逐场景保留。诊断脚本及逐事件FQ/ORACLE对照见[diagnosis](../diagnosis/)；每配置一次，是确定性局部仿真，不估计统计置信区间。

公开版保留无损`.gz`追踪；解压到同名无`.gz`路径即可恢复原manifest对应文件，[compression.json](../runs/compression.json)列明原／压缩hash、大小与解压校验。生成物`obj/`不发布，源闭包、构建输入快照、命令、工具版本和日志均发布。预检build01忽略PR/PW且FQ timer触发后看门狗误停；其数据保留且从最终28组排除，修复后统一build02，无覆盖旧运行。
