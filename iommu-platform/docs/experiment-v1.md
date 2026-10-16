# 双路实验入口 v1

日期：2026-09-08。已实现最小软件入口；FPGA 运行与 per-DID HPM 端到端校准尚未完成。当前主机没有连接 FPGA，用户也尚未准备 SD 卡。

2026-09-09 按用户“只做必要的软件修改”的要求，已修复小 chunk 错源校验盲区，并用现有参数补充多页、部分覆盖 smoke；接口、调度和 HPM 配置未扩展。主机回归与驱动目标交叉编译通过。**2026-09-15 已编译并更新 `out/experiment-v1/`，包含修复和新增 smoke；尚未上板验证。** [实现复审](experiment-v1-review.md)中的其他扩展按具体实验需要选取；HPM 相邻事件漏计 DDTW 的 RTL 问题仍待修，当前六槽未选择此事件。

用于组会讲解时，先看 [访问模式与组会讲解稿](experiment-v1-presentation.md)：有实际地址序列、工作集与总拷贝量的区别、执行时序图、Solo/Corun 对照逻辑和一分钟口头稿。本文侧重运行与字段定义。

## 当前入口与退役内容

`bench/idma_experiment.c` 是唯一当前用户程序，使用 `linux/include/uapi/linux/idma.h` 中 ABI 1。查询命令为 0x06，运行命令为 0x07；旧 0x01～0x05 返回 ENOTTY。

旧 `idma_bench`、`idma_pair_bench`、`idma_multi_bench`、`idma_once` 源码和旧内核执行流程已退役，保留在 Git 历史。构建只编译新程序，rootfs 明确清除旧副本/重复条目，不从 out/bench 任意打包文件。HPM RTL 定向测试、DMA 本体和正确性补丁保留。

本轮修改顶层平台与 Linux，分支均为 `codex/idma-experiment-v1`；CVA6/IOMMU RTL 未修改。开始时顶层为 `151c9a7ef2f2aea707c8f587b9b1906691a65430`，Linux 为 `2ef3f4b82afd3d2b039d6c67f214e50f27f10198`，工作树干净。源码/提交最终版本以 lock 与构建 manifest 为准。

## 在 FPGA Linux 上怎样运行

下列命令用于新版内核及 rootfs，不能在本机 x86 上直接运行交叉编译程序：

```sh
idma_experiment --device /dev/idma0 --caps
idma_experiment --device /dev/idma0 --aggressor none --transfers 4096 --warmup 256
idma_experiment --device /dev/idma0 --aggressor 1 --aggressor-pages 8 --transfers 4096 --warmup 256
```

每次调用只运行一个试次，输出一行 JSON。默认 V 源/目标各 1 页，A 源/目标各 8 页，4 KiB 页、4 KiB chunk、4096 个被测 V 描述符、256 个 V 预热描述符、60 秒 warmup+measure 时间预算。A 从预热前开始，由同一 CPU 循环补交，V 完成测量后停止 A 补交并排空。

参数为十进制整数。pages 每侧 1～4096；chunk 为 4 KiB 的正因子；transfers 1～2^32，warmup 0～2^32，timeout-ms 1～300000。仅支持当前 4 KiB 基础页平台的顺序循环手动映射；本版没有大页、random、BARE、四路独立流量或自动实验矩阵。

`platform-smoke` 包含四路无预热/有预热 Solo、每路 8 页中只拷贝前 4 页一次并校验剩余未触及区，以及 DMA0/1 角色互换。它验证调用/拷贝和错误传播，输出需与内核日志一起检查；warmup=0 不自动证明硬件 cold，退出成功也不等于 per-DID 校准通过。

## 结果的准确含义

- `success`：本次调用、测量和最终校验是否成功。错误、超时、隔离、ABI/返回数据不一致都会导致非零退出。
- `measurement_started/complete`：是否进入/完成测量窗口。complete 不取代最终数据校验；失败试次即使窗口完整也不能作为成功样本。
- `victim_elapsed_ns`：首个被测提交之前到最后一个 V 完成被软件观察到的时间，包含配置 MMIO、补交、轮询和调度。A 的尾部排空不计入其中。
- 每路 `submitted/completed/completed_bytes`：在测量循环中软件观察到的提交/完成；warmup 与尾部 drain 不计入。A 的首个完成可能对应 t0 前已提交的描述符，末端可能有尚未观察到的完成，不能当成精确总线字节计数。
- `refill_gap_ns_*`：从软件观察完成到下一次提交触发读完成，包括四次配置 MMIO 和 NEXT_ID 读。仍不等于硬件空闲时间，完成后到软件发现之间的延迟不包含在内。
- `in_flight_at_start/end`：软件已提交、尚未观察完成的状态；不能据此证明实际请求重叠。
- HPM 和 active 是在 V 窗口前后做的差值。`snapshot_before_ns/after_ns` 记录两侧采样耗时；多个寄存器非原子快照，A 在边界外仍可能贡献事件。active 只是 backend 非 idle 累加。
- `hpm_configured`：本次六个 selector/过滤等配置及回读成功，不代表事件语义已经上板校准。不可用测量在 JSON 中为 null，不能把缺失数据解释为 0。
- 当前 RTL 在 IOTLB miss 处按翻译阶段同时产生 S1/S2_PTW 事件；`s1_ptw/s2_ptw` 不代表独立 walker 完成数、页表内存访问量或占用时间。现有输出也没有 UT_REQ 请求数量分母，不能将描述符数直接当作翻译请求数。

本程序不输出自动 slowdown；对相同 V 输入的有效 Solo/Corun 样本计算任务耗时之比，需要先完成测量/压力校准。未取得直接硬件请求重叠或队列状态证据，本版不能证明 HOL。

## HPM 所有权与错误处理

新 `riscv_iommu_hpm` 会话独占该 IOMMU 的 HPM 配置，并阻止 sysfs 同时写 HPM。保存 0～5 槽、冻结、配置 V/A 各三个事件（IOTLB miss、S1 PTW、S2 PTW），结束恢复 selector、原事件计数和 inhibit。原事件统计不包含被实验占用的期间。cycles 不清零，暂时启用后恢复其 inhibit 状态。

能力查询检查 IOMMU 驱动归属和实际 CAP；运行时还检查六槽及精确 DID 过滤回读，故支持查询成功后运行仍可能返回 EOPNOTSUPP。计数读取采用 high-low-high 处理 32 位桥上的跨位进位，但不同计数器仍不同时采样。

所有实验由同一锁串行，避免 idma 会话相互干扰。超时/中断后停止补交并排空两路；无法确认排空或提交返回无效 ID 时保留可能仍被访问的映射/缓冲区，隔离设备至重启，并拒绝后续实验。超时不等于硬件取消。

数据模式区分设备和字内位置，最终检查源完整性、已触及目标范围及未触及目标范围。反复写相同内容时，最终正确仍不能证明每个重复描述符都正确搬运；描述符完成计数和定向数据验证需结合使用。这不是逐描述符在线校验。

## 验证记录

已通过：65 组用户态解析/ABI/错误传播测试；动态抽取真实调度函数的 12 个场景；25 组真实 HPM 会话控制逻辑测试；4 项 rootfs 白名单/去重/设备节点测试；相关内核目标交叉编译。完整软件构建状态见 [验证记录](validation.md)。

在平台根执行主机检查：

```sh
python3 tests/experiment_cli_test.py
python3 tests/test_idma_scheduler.py
python3 tests/test_iommu_hpm_session.py
python3 tests/test_rootfs.py
```

HPM 测试检出了索引宏未完整括住参数、导致 Aggressor PTW 读取错误地址的问题；两个生产宏已修复，六个受影响内核目标重新编译通过。测试覆盖六槽配置回读失败、会话互斥、状态恢复和计数进位，不验证 RTL 的事件产生语义。

调度测试覆盖 V 独立结束、A 循环、warmup 隔离、超时/中断排空及 quarantine；临时注入 warmup 计数污染、释放保留缓冲、删除 A 补交三类回归均被检出。主机 harness 不验证真实 DMA 映射、MMIO 时序或硬件事件。

此前软件和退役可执行文件在覆盖新构建前保存到 `out/pre-experiment-v1-20260908T124529Z/`，附校验清单；FPGA BIT/MCS 不变。SD 卡布局与实际启动文件尚未确定；不能把新文件任意重命名或写入未经确认的分区。此任务只生成交付文件，不写 SD 卡。

新版完整软件构建通过，交付集中在 [out/experiment-v1](../out/experiment-v1/README.md)，包含组合 payload、各阶段软件产物、manifest、lock、日志和 SHA256；目录内全部校验通过。软件构建提交为顶层 `ffe55a40175937122b3d6be92a6c005099f16dfd`、Linux `affbfbf6adda41d5b33f7b36880279d8ee8f48f7`。后续文档提交不改变这些已构建源码。
