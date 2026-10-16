# DATE 2025 作者实现：启用 SVA 的任务来源缺口

2026-10-15 有界只读核查。结论：**论文原件证明作者确实评估了启用 IOMMU/SVA 的 DMA 工作负载；作者公开分支也有实际 IOMMU 硬件连接。但本次限定源码链没有给出“映射→SVA 应用→tile/DMA→结果消费/撤销”的完整可导航实现，不能立即补齐 HERO 四案到真实共享 IOMMU 的绑定。** 这不是论文不真实、源码从未公开或 SVA 方向无价值的结论。

本轮仅沿论文脚注的一个作者分支 `pulp-platform/carfield:date_iommu_evaluation`；未搜索替代项目、未克隆大仓、未构建/实验、未改冻结四案、旧验收或生产代码。GitHub 网页读取返回 restricted URL；随后用公开 GitHub API 和 raw 固定提交取得原件，未使用凭据。完整检索指纹和必要原件保存在同名 JSON。

## 论文已经证明什么

原件是 `out/shared-translation-literature-20261006/papers/riscvsva2025.txt`，SHA256 `f2dbc6021b5b7d2a7b2a97ac8ba5b2deb179d006cd7507acd733ba398ab275a4`；对应 arXiv:2502.17398v1。

- TXT 119–124 / PDF p.2 的脚注明确指向 [Carfield/date_iommu_evaluation](https://github.com/pulp-platform/carfield/tree/date_iommu_evaluation)。
- TXT 137–164 / §III-B：Linux 6.9、IOMMU domain attach、host IOVA mapping、自定义设备 driver 与用户态库/OpenMP runtime；FP32 RajaPERF 子集使用 tiling、double buffering 与 DMA。论文明确描述实际 enabled 路径，而不是 HERO 的 `if(0)` SVM 声明。
- TXT 174–181 / §IV-A：axpy 对比 host、copy+cluster、IOVA mapping+zero-copy cluster。
- TXT 193–202 / Listing 1：`create_iommu_mapping`，前后 flush，`target device(1)` 接收 `a_iova`，kernel 使用 `a_iova + LLC_BYPASS_OFFSET`。
- TXT 204–213：kernel 结果排除 host offload/synchronization；DMA region 是等数据周期。因而论文性能数字不能直接填成本轮完整任务的 mapping、供给、consumer、unmap/free 成本。

这些原文足以支持“存在实际 SVA 搬运工作负载”，**尚不足以枚举该 workload 的精确调用前缀与合法选择集合**。Listing 1 是说明性伪代码，不是已经取得的可执行源码位置。

## 固定作者分支与实际源码证据

2026-10-15 公开 branch API 将该分支解析为提交 **`dbb4ad3380e6e81fa7eac3955e2340752f29dab0`**。下面链接全部固定该提交，不用可变 branch head 冒充不可变原件；尚未独立证明该提交就是论文实验时使用的完整快照。

| 已取得原件 | 精确位置 | 能证明的范围 |
|---|---|---|
| [Bender.yml](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/Bender.yml#L28) | 28、32 | Snitch dependency 固定 `269830af872fa943d8b079470120d42d62e1cb69`；作者 IOMMU dependency 固定 `4897be154f5e12e14d5a85bb8d94b1bbf5755b84`。仅构建依赖，不证明某应用已启用翻译。 |
| [hw/cheshire_wrap.sv](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/hw/cheshire_wrap.sv#L469) | 469–529 | FPCluster master 不走其他 master 的 bypass；其事务接到 `riscv_iommu`，completion 回原 FPCluster 下游，DS/PTW 使用独立 IOMMU master。AW/AR stream_id=0、ss_id_valid=1、substream_id=0。这是实际硬件插入；具体 driver 配置是否使一次任务走 translated mode 仍需软件原件。 |
| [sw/boot/carfield_soc.dtsi](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/boot/carfield_soc.dtsi#L154) | 154–166 | `spatz-cluster@51000000` 具有 `iommus = <&riscv_iommu>`；IOMMU 节点地址 0x2000a000。设备树有绑定，但不能单独证明 domain attach/map/unmap 实际成功。 |
| [sw/tests/bare-metal/snitchd/omptarget.c](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/tests/bare-metal/snitchd/omptarget.c#L24) | 24–74 | mailbox 收 offloadFn/Args，执行 `offloadFn(offloadArgs)`，barrier 后 DONE。只提供 generic offload dispatch，没有 axpy/gemm 计算、DMA tile、IOVA mapping 或 SVA 模式开关。 |
| [docs/um/sw.md](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/docs/um/sw.md#L214) | 214–216 | OpenMP offload 小节只有 `TODO Cyril`，没有把论文应用链接到实现的步骤。118–121 行仍有其他 domain 不支持翻译的通用旧文字；与本分支实际 IOMMU 接线并存，不能用它否定硬件或论文。 |
| [sw/sw.mk](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/sw.mk#L182) | 182–202 | Linux 用户程序入口是 `sw/tests/linux/*.c`；固定 sw 树中只有 `addressability_test.c`，没有同名 RajaPERF SVA kernel/driver 链。没有把该 addressability 测试误当论文工作负载。 |
| [.gitmodules](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/.gitmodules) / sw git tree | 1–3 / `sw/deps/cva6-sdk` | 唯一 gitlink 指向 `pulp-platform/cva6-sdk` 的 `98611e6c4d14c308e930af2bfb0b703c361d4d05`。其内容未在本次有界链继续展开；不能说依赖中也没有 driver 或应用。 |

API 的 `sw` subtree 查询为 **`truncated=false`**：`sw/benchmarks` 与 `sw/lib` 均仅有 `.gitkeep`；没有因 API 截断而漏掉该树内的 kernel。这个事实仅限固定 Carfield **自身 sw 树**，不覆盖 SDK gitlink、Snitch 依赖、其他历史提交/分支、作者其他仓库或未公开补充物。

## 能否立即闭合本轮 HERO 缺口

不能。现有 HERO 材料给 height32 双缓冲任务的精确数据/所有权图，但 active 路径是 copy-based，SVM 在关闭的 `if(0)` 内。DATE 原件给 actual SVA 任务与映射方法，公开分支给 actual IOMMU 接线，**两者没有自动组成同一个已绑定可执行任务**。不能把 DATE 的 SVA 配置借给 HERO，并宣称 HERO 的 A1/B00 发布顺序已在该硬件成立；也不能把不同 FP32 kernel 的 kernel-only 实验数字当作 height32 完整输出/consumer 时延。

因此本轮准入状态是：

- source-contract 层的语义执行、发布/义务分析可以按其明确假设继续；这些结果单独标为合同层。
- “实际启用共享 IOMMU 的完整四案时延验证”这一层仍缺来源和接口绑定，不能由本次核查验收。
- 当前没有取得平台真实竞争见证，也没有新的 SVA source-level 正例执行结果。本次不据此自动启动实验。

## 最小后继输入，不扩地图

继续只选 **论文同一实现的一条 axpy 或 gemm 链**，首先取得作者实验软件的**固定提交与构建入口**。已知可导航后继是 Carfield 的 SDK gitlink `pulp-platform/cva6-sdk@98611e6c4d14c308e930af2bfb0b703c361d4d05` 及已固定 Snitch dependency；如果其中没有论文软件，需要作者给准确 repository/ref，不能自行猜另一个工作负载替代。

闭合所需的最小原件集合如下；取得后先核对，不表示授权运行：

1. host benchmark 的分配/初始化、真实 map/attach/ioctl 参数、启用 SVA 的构建/运行开关、传给 target 的 IOVA 与 bypass offset，以及 unmap/free 和完整 consumer；
2. target kernel 的 tile 大小、两个 buffer、实际 DMA helper 的完整调用/等待/发布位置；
3. driver/runtime 将该用户 buffer 绑定到这里 stream/process 0、正确 IOMMU domain/页表的实际实现，以及 host/accelerator 可见性操作；
4. 一份固定输入/配置，能从这些原件枚举完整初始前缀、compute、最终输出、消费和释放。若保留 HERO 图，另需显式适配证明，不能只复用 DATE 的总述。

当前 unknown：论文软件是否在 pinned SDK/依赖内、确切 SVA workload 文件、实验软件版本、是否能表达与 A1/B00 同型的合法可改选择、与当前四 iDMA/CVA6 平台的实际对应。没有用标题、设备树节点或 generic mailbox 补全这些未知。
