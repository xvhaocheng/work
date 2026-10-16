# stage107 分散物理块布局静态见证

结论：原 stage106 十二个完整张量 copy 可以保留原 IOVA、张量偏移和有效字节，同时让四个 10MiB 槽各由五个独立 2MiB 物理块组成。大权重 copy 跨三个物理 extent，仍保留原整张量 descriptor；下面的分段仅用于证明字节覆盖，不要求软件逐 extent 发 descriptor。

**所有 PA 均为示例地址，没有实际分配、pin、预留、写页表或执行 DMA。** RAM 范围只按当前 [platform.dts:34](/home/xvhaocheng/work/iommu-platform/configs/platform.dts:34) 的几何检查，不证明这些地址在运行 Linux 中空闲。本轮不编译、不运行 DUT、不连板，不改原任务、生产或共享文档。

| 槽 | 连续 IOVA 范围（右端不含） | 五个示例 PA 起址（每块 2MiB） |
| --- | --- | --- |
| role0-source | 0x40000000–0x40a00000 | 0x90000000, 0x90400000, 0x90800000, 0x90c00000, 0x91000000 |
| role0-destination | 0x40a00000–0x41400000 | 0x91400000, 0x91800000, 0x91c00000, 0x92000000, 0x92400000 |
| role1-source | 0x41400000–0x41e00000 | 0x92800000, 0x92c00000, 0x93000000, 0x93400000, 0x93800000 |
| role1-destination | 0x41e00000–0x42800000 | 0x93c00000, 0x94000000, 0x94400000, 0x94800000, 0x94c00000 |

二十个物理块全局不重叠，并且任意相邻块之间都有 2MiB 空隙。合计块容量仍为40MiB；示例 PA 地址跨度78MiB，空隙不预留、不计作已分配存储。stage106 原模型的48MiB dense host backing 原样保留作为输入预算证据；未把该 dense 模型重映射或执行，也没有主张这个示例直接可用于原夹具。

两 role 各保留以下六张量；每 role 9,447,936B、双方18,895,872B有效传输。源和目标各验证所有有效字节，因此覆盖检查合计37,791,744B。

| index | 张量 | 原槽内偏移 B | 原有效字节 B | 槽内2MiB叶index |
| --- | --- | ---: | ---: | --- |
| 0 | fc1.weight | 0 | 4,718,592 | 0, 1, 2 |
| 1 | fc1.bias | 4,718,592 | 6,144 | 2 |
| 2 | fc2.weight | 4,726,784 | 4,718,592 | 2, 3, 4 |
| 3 | fc2.bias | 9,445,376 | 1,536 | 4 |
| 4 | final_layer_norm.weight | 9,449,472 | 1,536 | 4 |
| 5 | final_layer_norm.bias | 9,453,568 | 1,536 | 4 |

原 `commands-2m-parallel.tsv`、`commands-2m-single.tsv` 的十二个 descriptor 完整保留，仍使用原 src/dst IOVA、长度、role、channel 与 ready0。20个叶的源目标物理地址无 alias；每叶的整个2MiB范围，包括 tensor间空洞和尾部padding，仅标给其同一个槽 allocation owner。这是静态布局标签，沿用原模型 trusted coordinator 授权假设；不构成真实应用、进程或多租户安全证明，也不新增性能前提之外的安全需求。

实际 `dma_map_sg` 返回成功只证明它建立了可用 DMA 映射，**不能单凭成功推出返回 IOVA 为2MiB对齐或已安装20个2MiB叶**。本见证主动选择对齐 IOVA；真实 adapter 必须核实际 IOVA/PA run/叶大小与必要缓存交接。原模型 sourceRO/destinationRW是拟保留权限，不能当成当前 Linux helper 的实现情况。

完整数据和输入 SHA256 见 [layout-witness.json](layout-witness.json)。一次实际静态校验由 [layout-witness-check.py](layout-witness-check.py) 执行，结果见 [layout-validation-record.json](layout-validation-record.json)。申请页、准备、SG/页表同步、cache、drain和回收时间均保持未测；没有新增性能结果或 G1 算法。
