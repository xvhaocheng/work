# E4 页大小变更请求入口

当前实验 driver 逐4KiB调用映射接口；2MiB对齐的IOVA或大缓冲区不能使它自动变成2MiB IOMMU叶项。现有generic驱动和RTL虽有大页路径，4KiB/2MiB对照仍未执行。

具体原文、最小软件改动、物理/IOVA对齐及连续性、实际叶级验证、相同工作量与映射清理验收要求见[能力与变更请求](capability-and-change-request.md)。本项内核/driver变动等待用户决定；E3 P0授权不包含本项。
