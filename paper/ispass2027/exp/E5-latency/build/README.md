# E5 DS0 构建与恢复入口

当前唯一活动构建输出为本工作树 `../artifacts/e5-ds0-build01/`，启动时间
2026-10-17T13:28:41.205090Z。`launch02.as-started.json` 只是启动快照，
最终状态以该输出的 `build-manifest.json` 为准，尚无合格 BIT 或板测结论。

固定来源为 CVA6 `6c5c666d45979d3a03fb98cff89ba7ad095f6125`、platform
`2f19f6c2de5530760f521bf771b6217066bc54d5`；Linux `acece3a3ab7d72ba1f936c1984b82a4a55f28d46`
仅作此 FPGA 来源组合的引用，本动作没有编译它。完整 source receipt 是 `e5-fpga-source.json`。
运行参数为 DS0直连、50MHz、observer v4、PathDepth4、16段×32桶×32bit、scheme1。

沿用已审 `source/build_baseline.as-reused.py` 的导出、实现及 routed 检查流程与原约束。
`build_e5_fpga.py` 只绑定 E5 来源，并在保存原通用构建 manifest 前把 v3专用元数据修正为真实 v4/直方图参数。
失败即停，WNS≥0、setup/hold/pulse失败端点0、routing errors0；不改约束或实现策略。

原dispatch曾误将新输出放到主 `/work/iommu-platform/out/ispass2027-20261017-e5-ds0-01`。
Root要求修正后，仅对经argv/cwd验证的独占PGID1755949发SIGTERM，整个进程组确认无活动进程。
原manifest/log/执行wrapper保存在 `cancelled-main-dispatch01/`，原目录保留未移动。
该目录的原manifest由于SIGTERM仍写running，必须结合独立的
`process-cancellation.json` 判为 **cancelled_output_scope**，不能当作仍运行或时序失败。
`restart02.json` 证明新构建全相同冻结SHA，RTL/约束未改，仅11个Tcl的board.repoPaths前缀随输出根改变。

最小增量组件恢复包在 `../metadata/`，已在拥有 prerequisite 的真实仓库执行 `git bundle verify`：

| bundle | 唯一 prerequisite | tip |
| --- | --- | --- |
| cva6-e5.bundle | `8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8` | `6c5c666d45979d3a03fb98cff89ba7ad095f6125` |
| platform-e5-fpga.bundle | `52c5fdd028bbf3b07deaf1d0f7e13e3a271b29d0` | `2f19f6c2de5530760f521bf771b6217066bc54d5` |

恢复方先在对应组件仓库 `git bundle verify <absolute-bundle>`，再按需要
`git fetch <absolute-bundle> refs/heads/ispass/e5-latency:refs/recovery/e5-latency`。
每个组件是不同仓库；不得将CVA6 bundle导入platform。保留原分支，不向第三方upstream推送。
