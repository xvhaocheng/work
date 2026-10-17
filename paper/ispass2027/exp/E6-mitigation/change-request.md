# E6：P/N/PN/P4M统一基线变更请求（未实施）

## Material Passport

- 2026-10-17；状态：仅变更请求，无RTL/内核/驱动修改、构建或板测。
- 基座：baseline100固定源码及共用pathv3 Linux，见 [运行包](run-package.md)。
- 当前可执行范围：V1/A0角色互换30点；不把它称为缓解复测。

现有基线来源清单 `board/packages/baseline100-20261017/metadata/source-manifest.json:exclusions` 明确排除E3/O1/banked/P/N/PN/service scheduling changes。冻结Linux `include/uapi/linux/idma.h:77–85`仅caps、普通RUN、RUN_BATCH三个ioctl，没有P4M或research bank的公开入口。冻结基线 `dma_observer.sv:29`仅v2/v3标识，`97–98`只提供旧计数与128个PATH字段。活动研究源码及历史BIT不能替代统一修复基线。

请求获准后，以相同CVA6/IOMMU修复基座与相同Linux/工作量做独立、显式版本：

1. **P4M**：移植可配置A间隔及双方共同窗口/完整完成观测。需核对batch前端、observer bank、对应内核接口，避免只记停止A时的V加速。Solo/A1/A8×间隔设置由用户选定；baseline与每个间隔用相同负载/映射/频率。
2. **P/N/PN**：仅加入各自实际必要的隔离/非阻塞组织，保持原版同负载对照；分开固定编译参数和运行选择，不能用关闭功能的研究树冒称原版。组合复测须同时保留A miss、A完整时间和V窗口进度，既有PN负结果保留。
3. **验证范围**：每次变化先定向软件/RTL检查，再独立构建并保留已有manifest/时序报告；未经批准不启动这些工作。Root决定各版本是否和何时进入上板排期；本请求不恢复旧机制探索或停止审查。

接受条件是同一工作量下V与A都正确完成/安全排空、配置和来源可追溯、窗口和完整任务代价分报。缓解收益、无收益和组合退化都原样保留，不用旧版本最好值拼成统一图。
