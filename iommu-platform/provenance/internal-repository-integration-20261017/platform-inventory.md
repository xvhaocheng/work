# 内部平台仓库只读盘点

原范围 **822 路径，零重叠、零遗漏**；HEAD `0a570deda89636631462bb188d39c058409df364`。26 M、5 D、1 T、790 未跟踪。根新建 `provenance/internal-repository-integration-20261017/` 另计 14 项，未混入原822。

| 组 | 数量 |
| --- | ---: |
| D_ENGINEERING_NAVIGATION | 14 |
| S07_BOARD_NETWORK_UPDATE | 13 |
| C_COMPONENT_GITLINKS | 3 |
| D_RESEARCH_REPORTS | 168 |
| D_HISTORY_MIGRATION | 20 |
| S08_BUILD_ROOTFS_INTEGRATION | 3 |
| S02_P4M_SUPPLY_OBSERVER | 8 |
| X_VIVADO_LOCAL_CACHE | 1 |
| S04_COMPOSITION_HOST_TOOLS | 21 |
| S06_A2_SVA_USER_TASK | 3 |
| P_SOURCE_ORGANIZATION_PACKAGE | 513 |
| S05_ACTUAL_DMA_M0 | 7 |
| S03_NONBLOCK_COMPOSITION_RTL | 41 |
| S01_IOMMU_FIX_REGRESSIONS | 6 |
| E_TIMING_REPORT_BYTES | 1 |

102 项源码分为下列8组；完整路径见 JSON 的 `functional_source_groups` 与 `groups`，822项逐路径状态见 `records`。

| 功能组 | 数量 | 未跟踪 | 依赖 |
| --- | ---: | ---: | --- |
| S01_IOMMU_FIX_REGRESSIONS：DS写归属与PTW合法叶修复的定向回归 | 6 | 6 | IOMMU:F3, IOMMU:F4 |
| S02_P4M_SUPPLY_OBSERVER：P4M供给、观测、formal60恢复工具 | 8 | 3 | CVA6:CVA6-P4M-GAP |
| S03_NONBLOCK_COMPOSITION_RTL：banked、fullbuffer、P/N/PN RTL夹具与模块回归 | 41 | 41 | IOMMU:B1/N1/P1/P2, CVA6:CVA6-BANKED-PN-WIRING |
| S04_COMPOSITION_HOST_TOOLS：组合实验软件、profile、host runner与观测/归约工具 | 21 | 21 | S03_NONBLOCK_COMPOSITION_RTL, Linux:LCOMP_ABI/LCOMP |
| S05_ACTUAL_DMA_M0：实际DMA系统与完整双方COPY/M0夹具 | 7 | 7 | S03_NONBLOCK_COMPOSITION_RTL |
| S06_A2_SVA_USER_TASK：A2用户任务、固定输入fixture与主机检查 | 3 | 3 | Linux:LSVA |
| S07_BOARD_NETWORK_UPDATE：板端网络、SD更新、软件交付与协议配置 | 13 | 11 | Linux:LNET, OpenSBI:payload-window |
| S08_BUILD_ROOTFS_INTEGRATION：构建/profile/显式overlay和rootfs统一接入 | 3 | 0 | S04_COMPOSITION_HOST_TOOLS, S06_A2_SVA_USER_TASK, S07_BOARD_NETWORK_UPDATE, component_gitlinks_and_sources_lock |

源码组中 92 项当前未跟踪；若这102项全部实际提交，原822范围预计未跟踪降至 **698**（当前仍为 790；这是待执行推算）。当前完整status未跟踪 804，含根新快照。

先实际提交并选定子仓库HEAD，再原子更新平台 `cva6/linux/opensbi` 三gitlink与 `sources.lock.json`。当前三HEAD/index/lock全是旧SHA，M主要表示内部dirty；只stage路径不能锁定新链。新目标SHA在 JSON `component_pins`。

5处删除和1处普通文件→symlink须连同历史目标一起保留。六个目标均存在，仅e4-supply旧正文逐字一致，其余目标有修订；不称全部纯rename。`docs/validation.md.orig/.rej`明确留在工程记录组，不假定拒绝补丁已应用。

Vivado `.lpr` 只做精确ignore并保留文件；时序报告按原字节纳入工程记录；1675B SVA输入fixture属于源码测试输入。`/out/`当前整体ignored，不能全量force-add或删除，其中旧数据与构建物混存，本轮未扫描。新整理包513项整体保留。

未改源码、ignore、index、refs或commit；未运行构建、测试、实验、归约或板测。Codex-only。
