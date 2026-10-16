# 模式边界的实际硬件含义

## Material Passport

D-BOUNDARY-SOURCE-2；ARS-Codex experiment-agent/plan的有界源码与设计产物。不是统计验证、仿真或原生运行结果。root直接核查源级边界；未修改生产源码、未构建、未启动硬件。实际model/effort元数据未知。精确文件hash与实际阅读范围见[硬件来源清单](hardware-source-manifest.json)。

**硬件允许在整体quiet时原子配置，但没有“单个task完成即可切换”或“切换保留IOTLB驻留”的承诺。** 以下是当前生产源码的能力，不等同于当前板上已加载实现。

| 边界 | 实际源码与语义 | 对D的约束 |
| --- | --- | --- |
| 允许配置 | `dma_composition_control.sv:155–168,183–208`：quiet输入成立，窗口非ARMED/ACTIVE，且本拍无arm/open/close，数据合法、supported且generation未溢出，才接受offset2配置 | 单task DONE、一个DMA idle、窗口计数结束都不能替代整体quiet；同模式重写也会clear并增加generation |
| 上层整体quiet | `ariane_peripherals_xilinx.sv:1033–1035`：IOMMU quiet AND observer quiet AND全部job_quiet，并排除issue、launch_active与launch_held | 至少要停止全部相关供给并完成其义务，不能免费冻结另一owner |
| IOMMU quiet | `rv_iommu_top.sv:280–287`排除in_flight与TR/COMP/DS所有VALID；`rv_iommu_tl_wrap.sv:291–302`另排除arbiter、普通/ATS/debug、fast/slow/fault、walk、update与失效 | 必须完成旧响应，不能在有pending PTE/payload时直接改模式或销毁队列 |
| DMA observer quiet | `dma_observer.sv:105–108`排除batch_active、AR/AW VALID、tracking错误及未归还read/write计数 | 自然warm→measure保持请求流动时，不能假设此条件已经满足 |
| 配置作用 | `ariane_peripherals_xilinx.sv:1446–1450`把mode[0]接N、mode[1]接P、clear接policy_clear；`rv_iommu_iotlb.sv:215–216,424`clear所有valid并将PLRU置0 | 旧命中驻留与旧PLRU不是跨切换的有效预测状态；tags/content残字节不能当有效cache条目。不是全RTL/global reset |
| 配置确认 | `idma.c:2283–2307`读magic/ABI、旧generation、quiet，写配置后核status、generation+1与实际config | 源码给出可执行握手，不给出耗时0或已成功板测的证明 |
| 恢复默认 | `idma.c:2343–2355`最多等1秒quiet，之后configure(mode0,DID10)；不强行清未完成DMA | 正常composition调用后的恢复又有一次clear/配置费用；超时不能当成功边界 |
| 已有观测 | `dma_composition_control.sv:470–515`已有配置/generation/quiet、窗口周期、per-DID lookup/hit/miss、fast、gap、fill/repl/inval、PLRU和IOTLB open/close条目快照 | 普通基线须同样得到合法可读字段；未在这些原文中发现PTE-pending与payload服务的带身份联合时序观测 |

配置clear信号在本次已核连线上针对IOTLB，不是DDR复位。硬件quiet只陈述可见翻译与AXI生命周期，不证明MC内部恢复全部成熟或DRAM状态固定；上轮私有WR+16/WR+34服务完成域也不是此硬件接口的原生保证。这里不外推所有DDTC/PDT/MC内部状态相同或不同。

`idma.h:262–270`定义128个raw word，`idma.c:2315–2316`逐word调用composition_read；一次完整capture仅此循环就包含128次index写和128次data读。这个确定工作量不是耗时测量，128word也不是128个彼此独立且已校准的指标；旧HPM字段仍不能当已验证banked指标。

若外部研究adapter能暂停全部供给、保存task/buffer、drain全部旧义务，再调用合法配置并resume，硬件边界本身不排除这种设计。但暂停、barrier、保存/恢复、clear后的重新翻译及时间扰动都属额外实际工作；stock来源是否已有这条路径，须由[native](../native/findings.md)和[persistent](../persistent/findings.md)原文分别确认。缺接口不能直接成为新颖性证据。
