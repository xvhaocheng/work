# E4 DS0：4KiB与2MiB实际页大小对照

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Verification Status: VERIFIED（20/20 FPGA试次通过，10个同boot/block配对）
- Version Label: e4-common-v2-final01

**同一负载下，2MiB映射相对4KiB的受害者时间下降中位数为23.2191%，吞吐提升中位数30.2406%。** 两个百分比均先逐block计算，再取10对中位数；不以两个cell中位数相除替代配对。

| IOMMU实际leaf | n | V硬件时间中位数(ms) | Q1–Q3(ms) | V吞吐中位数(MiB/s) | V IOTLB miss / S1 PTW中位数 |
| --- | --- | --- | --- | --- | --- |
| 4KiB | 10 | 139.19783 | 139.19540–139.21306 | 114.944 | 8192 / 8192 |
| 2MiB | 10 | 106.88277 | 106.86477–106.90964 | 149.697 | 0 / 0 |

2MiB/4KiB硬件时间比：中位数0.767809464，Q1–Q3 0.767513656–0.768156702，n=10。IQR是描述性离散度，不是置信区间。所有试次保留，无删除首点、失败或替代。

V=DMA0/DID10，A=DMA1/DID11；双方每方向2MiB缓冲区、4KiB COPY、512页环形足迹，V4096次/16MiB，V warmup256。A运行至V完成后STOP/drain，实际A完整工作含启动和排空，详见逐试次值；它不是固定16MiB的A工作预算。两种条件只改变IOMMU映射leaf大小，不改变物理缓冲区长度或COPY大小。驱动逐个4KiB基址及末字节验证实际PTE/PA，四buffer前后mask均15，安全unmap/free/data检查与taint0门全部通过。

IOTLB/PTW是原非原子软件HPM bracket内的计数，不能追认为与硬件V窗口完全同边界；2MiB下两设备本轮测得miss/PTW均0。stage1是翻译及IOMMU内部处理至请求呈现，不等于纯PTW。V read stage1的逐试次均值中位数1138.4877→25cycles，write为495.9906→11cycles；各段不能直接相加为descriptor延迟。

[统一17列长表](paper.csv)共3824行，[完整统计](summary.json)、[原始数据绑定清单](analysis-manifest.json)、[Root独立有理数复算](../../root-validation/final-board01.json)。同一20点的[E5分布](../../../E5-latency/paper/e4-common-v2-final01/README.md)不另计新增试次。

运行从2026-10-17T15:54:43Z开始，16:01:33Z完成（约410s）；boot `bf3063bb-b908-4333-bcb9-54a08511210e`、seq1–20。DS0/50MHz；BIT `d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c`，实际CVA6 6c5c666d45979d3a03fb98cff89ba7ad095f6125，IOMMU85b157。共同Linux65b427/platform ec996/OpenSBIe8d，release `6.5.0-rc2-ispass2027-full-e3-v1`。软件构建CVA6 1d587只是reference；硬件原manifest中的Linux acece引用也不是本轮运行内核。

只将本完整包并入共享表/论文索引；E5复用本20点，不能重复计数。Root仅推任务分支，不自行合main。
