# E5 DS0 构建与恢复入口

本工作树 `../artifacts/e5-ds0-build01/` 已于2026-10-17T14:06:29.967265Z通过，
启动时间2026-10-17T13:28:41.205090Z，两个实际命令均exit0。
`launch02.as-started.json` 只是启动快照，最终完整manifest与原始routed timing/route/utilization报告
逐字复制到 `completed02/`。`completed02/acceptance.json` 关联实际路径、报告hash、BIT身份与资源比较。
50MHz下WNS +0.126ns、WHS +0.047ns、WPWS +0.062ns，三种失败端点及routing errors均0；
CPU周期20ns，DS delay cell和forbidden research cell均0。未上板，不声称完整IO/CDC签核。

BIT为11443722字节，SHA256 `d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c`，
复用最终build manifest已计算的hash，未重复hash二进制。
本机 `../artifacts/e5-ds0-delivery/ariane_xilinx.bit` 是完成后原件的硬链接；
Root/A3可按共同包流程复制或硬链接实际字节，最终软件身份使用A3的共同E4/E5 Linux/payload manifest。
本FPGA清单中的acece Linux引用保留实际原值，不能改写成随后共同软件的执行身份。

相对同工具/DS0/50MHz的已接受P0/multi `8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8`：
placed LUT 149400→166334（+16934，11.33%），registers126265→142751（+16486，13.06%），
BRAM50、DSP27不变，Slice50355→50581；当前Slice使用99.28%。这是整个设计的实测差值，
不是独立hist模块面积归因，也不作为未来OBS5/Depth64的可实现证明。

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
