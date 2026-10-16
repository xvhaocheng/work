# 带入的修改

| 层次 | 修改 | 当前处理 |
|---|---|---|
| IOMMU | `6cc8ed3`：AXI MMU metadata 接口 | 保留原提交 |
| IOMMU | `7fbb2b9`：AW 优先，处理大传输 deadlock | 保留原提交；新板上进度／公平性回归待做 |
| IOMMU | HPM 捕获时设置 `edged_event_n` | 从旧工作树保存为新提交；定向仿真验证 |
| CVA6/iDMA | `b98b0ef7`：新版接口、DS 延迟、active cycles、延迟计数器位宽 | 保留原历史 |
| CVA6 | `b63c5523`：多 DMA mux 和控制地址空间 | 保留原历史 |
| CVA6 | 四路 DMA、地址解码及 16 KiB 控制窗口 | 从旧工作树保存为新提交 |
| Linux | 现有驱动、DTS、HPM 读取、Pair/Multi ioctl | 保留原历史 |
| Linux | 双路 Independent、超时 drain／quarantine、测量边界调整 | 从旧工作树保存为新提交 |
| Linux | 去掉 defconfig 中旧 rootfs 绝对路径 | 新的迁移提交，构建时生成本机路径 |
| OpenSBI | timer 频率从 1 MHz 改为 25 MHz | 从旧工作树保存为新提交，与当前 DTS 对齐 |

上游锚点：IOMMU `c80bba7`（mc/dti-ats），CVA6 `1f992d80`（feat/iommu-refact）。这些是来源，不是本平台最终版本；最终版本见 sources.lock.json。

HPM 当前的补丁按一个连续 valid 区间捕获一次事件。定向测试验证单脉冲、持续 valid、结束后不继续计数、DID 过滤、禁止／重新使能，以及独立 cycles 计数。它不替代全部事件源的端到端校准，尤其还需确认事件源是否允许 valid 不落下时切换为另一种事件。

AW 优先补丁会改变仲裁行为；两种实验 PROFILE 都包含同一补丁。后续比较缓解方案时应始终使用相同正确性基线，检查读请求进度，不能用可能卡死的未修复版本作正常性能基线。

双路 Independent 已实现，但 shared_cycles／shared HPM 仍是公共窗口。四路 Multi 仍为逐 chunk Barrier。没有在本次整理中将这些程序改称为已完成 Victim 独立 latency 或四路独立流量。

provenance 中保留迁移前 diff，便于审查；它们已体现在组件提交中，不能再 git apply 一次。重复分支上的等价接口/HPM 补丁未重复应用。
