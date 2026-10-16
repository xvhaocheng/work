# DMA smoke 失败：诊断与最小修正

后续更新：诊断版已上板，目标旧值经 FENCE.I 恢复正确；最新修正及复测入口见 [DMA 缓存同步](DMA_CACHE_SYNC.md)。下文保留上一版诊断的历史状态与交付。

日期：2026-09-16。输入是用户提供的 `DMA_SMOKE.md`，副本和哈希保存在 `out/dma-smoke-debug-20260916/`。目前服务器收到的是摘要；原始串口和两条 JSON 仍在 Mac，没有把摘要当作完整原始日志。

## 已确认发生什么

据实板记录，SC.W 修复版 `migration-4dma-scfix` 已完成 Linux 启动、root 登录和 proc 检查，内核为 `031e91dbadc1`，四个 DMA 节点存在、taint 为 0，没有新 Oops/BUG。此前“新 BIT 尚未上板”的状态已由这份后续记录更新；这不表示 DMA 功能通过。

| 问题 | 实板结果 | 结论 |
|---|---|---|
| 原 smoke 依赖 BusyBox timeout | applet 不存在，退出 127，未提交 DMA | 打包/脚本依赖缺失 |
| 临时兼容版第 1 例 | DMA0，1 页，4 KiB chunk，8 次，warmup=0，校验通过 | 该次调用成功 |
| 临时兼容版第 2 例 | 相同 DMA/页数/chunk/次数，warmup=32，8/8 完成但 `data_errors=1`、`status=-84` | 内容校验失败，尚未定位源或目标 |
| 后续 12 例 | 脚本遇错即停 | 未执行，包括双路用例 |

`-84` 是 `-EILSEQ`；终端的 multibyte/wide character 字样来自通用 errno 文案，与串口编码无关。校验遇首错就返回，因此 `data_errors=1` 不表示只坏了一个字节。测量完成也不覆盖最终内容校验。

## 为什么现在不能只归因于预热

两例同时改变了预热次数和调用顺序。每次 ioctl 都重新分配、映射并最终释放缓冲区；第二次可能复用 IOVA 或物理页。需要把预热、缓存状态和跨试次重映射分开。

单页、chunk=4096 时，预热和测量都访问偏移 0，`touched_end` 为 4096；当前软件最终期望值没有因预热次数改变而不同。第二例测量段 miss/S1 为 0 也不是旧翻译残留的证据，因为它在 HPM 前快照之前已经完成 32 次预热。

DMA 的 DONE ID 不能证明没有 AXI 错误：当前 wrapper 配置 `NO_ERROR_HANDLING`，完成响应不向这个软件接口报告总线错误。因此仍需内容检查及 IOMMU/总线诊断，不能由 8/8 完成排除地址或响应问题。

## 优先核实的缓存契约缺口

定向源码核查确认：

1. 当前 Linux 配置 `ARCH_DMA_DEFAULT_COHERENT=y`；DTS 没有 `dma-noncoherent`，OF 将设备当作 coherent。
2. 连续 DMA 分配在这条路径返回普通 `page_address()`；FPGA 的 DDR 区域可被 CPU 缓存。
3. CVA6 WT cache 不因普通 store miss 分配新行，但 store hit 会更新已有缓存行；外部 DMA 写入没有对应的远端失效通知。
4. WT 下普通 FENCE 不清 D-cache；当前平台的 FENCE.I 会额外清 D-cache。这个行为来自当前 CVA6 实现，不是通用 RISC-V DMA 缓存维护保证。

因此可能出现“第一次 DMA 校验读把页面带入缓存；下一次复用该页时，CPU 初始化更新缓存，DMA 更新内存却没有使 CPU 缓存失效；CPU 校验读到旧值”。**这是源码支持的候选因果链，现有摘要尚未证明该次试验发生了物理页复用，也未证明这就是唯一根因。**

仅改设备树的 coherent 标志或加 `dma_sync_*` 还不足以解决：需要平台实际可执行的缓存维护或有效不可缓存映射。当前不能假定已有 Zicbom/Svpbmt 支持，也不先通过关闭校验或改成重读成功来掩盖故障。

## 本次最小改动

- 为 smoke 提供镜像内可用的超时保护，避免继续调用不存在的 BusyBox applet；保留原 14 个用例和遇错停止规则，失败时也采集最近内核日志。
- 驱动仅在首个校验错误时记录偏移、源/目标实际和期望值、触及范围、IOVA 与物理映射。
- 在任何日志输出前保存错误字节，DMA 已排空后执行一次当前 CVA6 的 FENCE.I 探针，再保存同址字节。**原来的 EILSEQ 和 data_errors 保留，刷新后读对也不能变成 PASS。**
- 不新增实验模式或 JSON ABI，不在正常测量窗口加入缓存刷新，不修改 DMA/IOMMU/CPU RTL。

探针的解释：刷新后同址数据从错变对，支持 CPU 缓存旧视图方向；仍需适当对照排除额外等待等影响。刷新后仍错，则继续检查软件映射、IOMMU 失效、实际总线响应和搬运数据，不能宣布排除全部缓存问题。

日志字段 `src`/`dst` 是刷新前保存的首错字节；`src_after_fence_i`/`dst_after_fence_i` 是刷新后保存的字节，`expected_src`/`expected_dst` 是原始期望值。所有地址都已加上首次错误偏移，指向出错字节；`src_pa`/`dst_pa` 来自 CPU 缓冲区对应物理页，`src_pte_pa`/`dst_pte_pa` 来自软件当前页表查询，**后者不是硬件 IOTLB 内容**。

探针会清整个本核 D-cache。若双路中 V 先失败，它会改变随后 A 校验的缓存状态，A 不报错不能证明其刷新前视图正确。因此本轮先做 Solo 诊断，不能拿这个探针作为双路缓存错误的独立统计工具。

## 下一次上板怎样缩小范围

先使用本次诊断软件配合相同 SC.W 修复 BIT，保存版本和完整串口日志。暂不继续双路或性能矩阵。

第一步只复现 DMA0 的单页失败条件，并保留首错日志。若要区分预热和运行次序，在独立的新启动中分别比较：

- 第一次调用直接 warmup=32。
- 连续两次 warmup=0，各自保留退出状态；出现首次错误即停止。

所有这些调用均保持 pages=1、chunk=4096、transfers=8、aggressor=none，并使用外层超时。不能在未经检查的异常运行后继续累积测试，也不能把小批量试跑替代 E0 校准。

在新启动的诊断版 FPGA Linux 中执行（不是在 Mac/服务器执行）：

```sh
uname -a
/usr/bin/platform-timeout 120 /usr/bin/idma_experiment \
  --device /dev/idma0 --aggressor none --victim-pages 1 \
  --chunk 4096 --transfers 8 --warmup 32 --timeout-ms 10000
rc=$?
echo "DMA diagnostic exit=$rc"
dmesg | tail -80
```

先保存这一次的完整输出。若出现 `experiment data mismatch:`，需要原样保留整行；不要因为 `*_after_fence_i` 等于期望值而继续性能测试。这仍然是失败的诊断试次。

根因确定并修正后，再回到完整 smoke、per-DID HPM 校准和 E1 先导实验。已有时钟/CDC/IO 约束待办仍保留。

## 代码依据

- 校验与映射生命周期：`linux/drivers/misc/idma.c` 的 `idma_stream_prepare`、`idma_run_experiment`、`idma_verify`。
- coherent 分配与默认值：`linux/drivers/iommu/dma-iommu.c` 的 `iommu_dma_alloc_pages`；`linux/drivers/of/address.c` 的 `of_dma_is_coherent`；`out/linux/.config`。
- 缓存行为：`cva6/core/cache_subsystem/wt_dcache_wbuffer.sv`、`wt_axi_adapter.sv`、`wt_dcache_missunit.sv`；`cva6/core/controller.sv`。
- DMA 错误处理配置：`cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv`；`cva6/corev_apu/iDMA/src/idma_backend.sv`。

## 构建与交付结果

- Linux 最小诊断提交：`53d3113dee74761515fc08b6a4f984d41227c50f`；软件构建顶层提交：`38774213f7cb8f3ea4240e6e127e363ae5f6be02`。已有研究/计划文档脏改动保留，manifest 如实标记顶层 dirty；21 个组件源码、gitlink 和 lock 检查通过。
- 主机检查通过：原 12 个调度场景加 4 个诊断场景、96 个小 chunk 正常/错源组合；watchdog 9 项（含快速退出竞态、超时/信号和回收、失败日志保留退出码）；rootfs 5 项。驱动目标和 watchdog 的 RISC-V 交叉编译通过，相关 checkpatch/diff 检查通过。
- `make software PROFILE=migration-4dma-scfix JOBS=12` 退出 0，内核版本 `6.5.0-rc2-00079-g53d3113dee74`。保留原有 DTS interrupt provider 缺 `#address-cells` 警告。
- 新交付：[out/dma-smoke-debug-20260916/delivery/README.md](out/dma-smoke-debug-20260916/delivery/README.md)。rootfs 中新入口、watchdog 与 smoke 内容已逐项核对，两个 ELF 的 RISC-V 架构核对通过，内核包含首错诊断字符串，目录内 SHA256 全部通过。
- 新 `fw_payload.bin` SHA256：`58ba4d4ae9601ba8fad6064e217dabef51f897ac5461ca07fcc77eb9232836bb`。保持使用 SC.W 修复 BIT，SHA256 `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3`，已核对未变；本轮未重建 FPGA。
- 原失败软件交付 `out/experiment-v1/` 保留。**新镜像需要更新 SD 上的 Linux 软件并重新启动，不能只替换用户态程序得到内核诊断。** 新版 watchdog 和缓存探针尚未在 FPGA 执行，DMA 数据错误尚未确认解决。
