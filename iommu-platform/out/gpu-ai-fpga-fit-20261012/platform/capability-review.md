# 现有 FPGA 对 GPU／AI 来源任务的能力契约

2026-10-12，Codex 独占关键源码核验。本轮只检查当前源码和必要既有证据；没有调用 DeepSeek／Claude，没有改生产代码或公共文档，没有构建、仿真性能或板测。[机器可导航契约](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-fpga-fit-20261012/platform/capability-contract.json)包含 24 项事实、24 份当前文件哈希和逐项行号。

**结论：现有 FPGA 可以真实执行经过共同 IOMMU 的内存搬运，也可以增加一个有成本的应用适配层，承接有限 AI 数据流；现装 Linux ioctl 仍是驱动生成 pattern 的双路微基准。** GPU 计算、显存层次和 CUDA 完成语义没有随 iDMA 一起存在。诊断模型能承接来源任务的地址、页工作集和依赖投影，但自身不能给出真实 GPU 算时、Linux 供给或 CPU cache 成本。此核验不登记 G0，也不裁定某个普通策略已失效。

## 生产路径确实提供什么

| 项目 | 当前可核事实 | 直接入口／契约事实 ID |
|---|---|---|
| 执行平台 | Genesys2；一个 CVA6；CPU 配置 50MHz，timebase 25MHz | [builder](/home/xvhaocheng/work/iommu-platform/scripts/platform.py:228)、[单核实例](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_xilinx.sv:712)；P01 |
| DMA 设备 | 四个 iDMA，DID 固定 10、11、12、13；DTS 与 RTL 对应 | [四设备定义](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:870)、[DID 参数](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1279)、[DTS](/home/xvhaocheng/work/iommu-platform/configs/platform.dts:141)；P01、P04 |
| 共用翻译资源 | 四路 extended AXI 经一个 mux 进入一个 IOMMU；单 TL wrapper 内共用 IOTLB／PTW。不同 DID／页表根不等于不同翻译硬件 | [mux](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1228)、[IOMMU 实例](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1443)、[共用 IOTLB](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:657)、[PTW](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:759)；P04 |
| 控制供给路径 | CPU → 系统 crossbar 的 DMA_CFG → 按 MMIO 地址 [13:12] 选路的四路 demux → iDMA slave | [CPU 接口](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_xilinx.sv:849)、[demux](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1194)；P03 |
| 内存容量 | 物理 DDR 1GiB，PA `[0x80000000,0xc0000000)`；Linux DTS 暴露 `[0x80200000,0xc0000000)`，即 1022MiB。还要扣 OS／运行时／表和所有复制副本 | [地址预算](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/tb/ariane_soc_pkg.sv:62)、[Linux memory](/home/xvhaocheng/work/iommu-platform/configs/platform.dts:32)；P02 |
| 翻译配置 | 16 项 IOTLB，8 项 DDT cache，8 个翻译在途参数，32 深度 W FIFO；ATS 关闭 | [实际配置](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/tb/ariane_soc_pkg.sv:110)；P08 |
| 数据路径／任务 | 64bit，1D copy；每 DMA job FIFO 深度 8、AXI 在途参数 8。当前 frontend 发一笔后 WAIT 到完成，FIFO 8 不能写成八笔实际同时发布 | [生产参数](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1255)、[COUNT／WAIT](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:519)；P05 |
| 特定缓冲／控制 | 非阻塞 mode 的 DMA0／1 各 512 beat＝4KiB，其余或普通 mode 为 3 beat＝24B；Batch 四路有，Launch 只前两路，Research 只特定角色的 DMA1 | [条件参数](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1262)；P05、P06 |

全 copy 许可还要求唯一、对齐的 DEFAULT 4KiB 任务：[证书条件](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:14)。它不能免费扩成 16KiB／任意长度许可。裸寄存器确实有 runtime `src_addr`、`dst_addr`、`num_bytes`：[frontend](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:320)。较长或非对齐任务必须另验对应 mode 的安全边界；现有 4KiB 分段也要计提交和同步成本。

这些是当前工作树结构和配置。当前生产源码有既有修改；不能只用 HEAD 作为全部身份。此核验没有新 image 的对应、时序闭合或实际空闲 DDR 观测。

## Linux ioctl 的实际合同

**它能动态改变页数、chunk、次数等微基准条件，不能由应用提交任意源／目的／长度或用户 tensor。** [UAPI 输入](/home/xvhaocheng/work/iommu-platform/linux/include/uapi/linux/idma.h:54)没有应用 buffer 或 descriptor 列表；E3 `mapping[8]` 是输出，research `raw_user` 是观测词输出地址。[fops](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2927)只有 ioctl，没有 read／write／mmap。旧 `0x01..0x05` 已退役。契约 L01。

调用链是：

1. `idma_run_session2` 验证参数并持有全局 experiment mutex，选择一个 V 和可选的一个 A；两个用户进程各开一个 ioctl 会被这个锁串行化。[选路／锁](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2384)；L08。
2. `idma_stream_prepare` 给每路分配连续 source／destination，填 source pattern 和 destination 补码；取非 identity 的 IOMMU domain，寻找空闲 IOVA 别名，再以 4KiB RW 叶子手工映射。IOVA／PA 是本次分配所得，不是应用指定，也不是仿真固定常数。[分配／映射](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:354)；L02。
3. 当前每路最多 4096 页＝16MiB source＋16MiB destination；双路总 payload 最多 64MiB，不含 OS／PTE／队列／复制副本。`chunk≤4KiB` 且必须整除 4KiB；Batch 还强制 `chunk=4KiB`。连续内存分配可能早于总 DDR 容量耗尽而失败。[约束](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:937)、[Batch 检查](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2384)；L03。
4. source／destination 以相同 offset 在该 ring 内循环；本轮执行期间不做真实用户 buffer 周转、CPU 消费或逐 descriptor map／unmap。[提交参数](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:411)；L04。
5. 写 SRC／DST／NB／CONF，读 NEXT 保留并触发 functional ID，软件以 DONE 等于该 ID 确认完成。失败排空不能证明安全时保留 buffer 并隔离到重启；成功结束后核双方数据。[NEXT／ID](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:448)、[poll](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:668)、[drain](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:751)；L06。

普通软件补发每路保持一笔软件观察到的 in-flight；循环先看 V，再看 A，含 MMIO／NEXT、poll、`cond_resched`。`refill_gap` 是软件观察完成到软件观察补发，不能直接叫硬件空闲：[实际循环](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:690)。Batch 把逐笔补发搬到 frontend，因此两者的供给合同不同。

**后台的结束规则也必须改。** 通用 Batch 给 A 的 target 是 `2^32`，V 结束后停止 A；软件模式同样在 V 结束后停止补发。它没有独立用户可指定的有限 A 工作量与自然完成点：[A 配置](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2544)、[V 结束停止 A](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2591)。现成 controlled E3 是双方有限单 copy，但 ABI 固定 DMA0／1、每路一页、64B、一次：[限定](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:1500)。这些不能冒充任意 AI 独立任务生命周期。

## 数据所有权、完成和真实计算

启动前调用 `local_flush_icache_all()`；双方排空后再次调用再核数据。源码明示这是本 CVA6 无 DMA snoop 的 WT-cache 专用补救：[启动交接](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2491)、[结束取回](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2643)。根因边界也核了 RTL：FENCE.I 触发 D-cache flush，[controller](/home/xvhaocheng/work/iommu-platform/cva6/core/controller.sv:98)；missunit 等写 buffer／MSHR 排空，然后失效各项 valid bit，[drain](/home/xvhaocheng/work/iommu-platform/cva6/core/cache_subsystem/wt_dcache_missunit.sv:398)、[失效](/home/xvhaocheng/work/iommu-platform/cva6/core/cache_subsystem/wt_dcache_missunit.sv:346)。契约 L05。

当前 session 的 CPU 只初始化和最后验证，运行中不读写这些数据。增加逐 slot 计算后，需要逐 slot 的 CPU→DMA→CPU 交接；不能把会话首尾的 flush 直接当并发消费已支持，也不能只写一次 `dma_sync_*` 就假定本核已经有可用的通用 cache-maintenance 实现。全局 FENCE.I 本身会影响 CPU 计算和其他缓存，须计入完整成本。

DEFAULT copy 完成链来自最终写 B 响应：[transport](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:444) → [backend rsp](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:519) → [frontend completion](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:206) → DONE。该 wrapper 选 `NO_ERROR_HANDLING`，backend 将 `idma_rsp.error=0`；DONE 不能单独证明有效 tensor 正确。新适配要保留数据、守恒、fault／drain 证明，并另加计算输出和最终消费者完成。契约 L07。

已有控制能挡住**下一笔尚未发布的任务**：HOLD 可在 VALID 前 cancel，ADMIT／gap 控制未来发布；COUNT 后 VALID 和地址保持到握手，STOP 排空已提供的任务：[控制状态](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:519)。不能用“控制寄存器可写”推得可撤销已提交 copy。

CVA6 能作为真实 CPU 消费者，实际 ISA／配置有标量浮点；F16／F8／向量 FP 未启用。[配置](/home/xvhaocheng/work/iommu-platform/cva6/core/include/cv64a6_imafdc_sv39_config_pkg.sv:13)。CVXIF 当前例子只加操作数：[执行](/home/xvhaocheng/work/iommu-platform/cva6/core/cvxif_example/cvxif_example_coprocessor.sv:143)。可以选一个符合原语义的有限 CPU kernel 适配，取得实际计算和完成事件；这还没有实现，也不能由核数／乘法器峰值推出速度。修改精度、维度或 kernel 算法必须作为来源合同的变化明确交代。

## 现有两 iDMA 诊断模型的边界

两路 wrapper、DID10／11、真实 frontend／backend、64bit 和四路 mux 的结构直接存在：[实例](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_top.sv:352)、[第二实例](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_top.sv:411)。另外两端静默；这不是四路应用模型。cfg selector 通过实际 AXI 访问某路 slave，但绕生产 CPU crossbar 和 DMA cfg demux；`cfg_xact` 的 tick 成本不是 Linux MMIO／软件成本：[cfg](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:1461)。当前 `program_single` helper 固定 4KiB，底层寄存器支持长度不代表其所有测试长度已验证。契约 S01、S02。

内存是 C++ sparse byte map，静态 DDT、两个不同 Sv39 根和 4KiB 叶子：[映射](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:509)。系统 wrapper 把 `<4GiB` 地址都指向行为 DDR；不能当成板上 1GiB 空间或 Linux 分配预算，DDT `0x10000` 在板上还是 ROM 区。系统版增加真实 DS merge、delayer、crossbar，却没有真实 CVA6／cache、MIG／时钟转换和应用设备：[明确限制](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_system_top.sv:5)。契约 S03、S04。

| GPU／AI 来源特征 | 此平台的状态 | 合法主张／必须付成本 |
|---|---|---|
| 内存 copy 的页局部性、固定映射寿命、buffer 复用 | 现 FPGA 真实路径支持受控微基准 | 能核 shared-IOMMU 机制；尚无选定 AI 程序普通策略失效 |
| 原任务 descriptor／DAG／页工作集 | 现诊断可加定向回放 | 保留原数据、字节、依赖、有限结束；实际时间未取得时只做功能依赖，不猜 compute delay |
| 输入搬入→真实 CPU 计算→输出搬出 | 要新增适配层 | 真实 payload、持久 pool、所有权、编译／运行、输出核验及全部供给成本 |
| 动态映射周转、任意 tensor slice、独立 source／destination offset | 现 ioctl 未提供 | 新 buffer／descriptor ABI，映射权限、invalidations、生命周期／失败排空；不靠改页数代替 |
| 四路并发应用 DMA | 硬件有，应用接口未提供 | 扩 session 调度和互斥边界；DMA2／3 当前没有前两路的 whole-copy buffer／launch 能力 |
| CUDA／GPU stream、GPU 显存／HBM、tensor core、GPU P2P、UVM／ATS | 当前核验合同未支持 | 不据此否定 DMA 翻译投影；这些语义若成核心主张必须另有实现和证据 |
| 原 GPU／NVMe／存储层的端到端时间 | 当前 copy 模型不能给出 | copy 完成不等于 GPU kernel／媒介／消费者完成；服务器只作参考或构建 |

## 最小可实现适配入口与接受检查

优先在 Linux 的 buffer 会话／descriptor 提交层增加**独立 ABI**，再加一个小 bench。用既有 SRC／DST／NB／NEXT／DONE 和 translated domain，保留旧实验接口语义。为首个有界 1D copy＋真实 CVA6 计算基线，没有必然的新 RTL 要求。硬件计算加速、2D／scatter-gather、更多全 copy buffer 都是另外工程。

最小接口应有：持久 kernel-owned pool 的 opaque handle；payload upload／download；`lane, src_handle/offset, dst_handle/offset, length, task_id, dependencies`；submit／wait／release；每个 buffer 的 CPU-owned、DMA-read、DMA-write、ready-for-CPU 状态。首版可用显式 upload／download 让 CPU 消费，必须计复制和系统调用；mmap／零拷贝会增加 cache／生命周期证明。每路要有自己的有限任务量与自然结束，不能沿用旧 V 完成就 STOP A 的合同。

仅诊断先行时，在新私有目录加 descriptor／数据／依赖适配即可；必须显式扣板级 DDR 预算并保存真实发布、完成和两路尾段。没有原 compute 时间或实际 CVA6 kernel 时，不填一个有利等待常数，不按 GPU／FPGA 速度缩放。

接受前须固定并验证：

1. 一项具体来源任务与被保留／丢失的属性，完整 tensor／缓冲／副本／表预算；缩小问题规模后仍匹配被研究需求的理由。
2. 输入就绪、CPU／DMA 可见性、真正的计算完成、输出与 buffer 重用依赖，以及控制在 VALID 前可达的证据。
3. 映射／准备／invalidations、cache 交接、MMIO／poll／调度、复制、真实 compute、最终输出与两路完整尾段的成本。
4. 数据与计算输出正确、双方独立有限任务完整完成，错误／超时能证明安全排空或隔离；保留既有守恒验证。
5. 新实现对应源码、profile、image、时钟及 timing 的 FPGA 证据；用实际结果识别最强合法普通方法的具体失效后才登记 G0。

这份核验的决定问题已经解决：应用适配缺口位于软件数据／控制合同，而共享 IOMMU 硬件路径是真实存在的。它没有提供选定 AI 工作流的普通失效，也没有批准新机制或性能结论。
