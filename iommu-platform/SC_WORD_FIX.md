# SC.W 高半部响应修复（2026-09-16）

## 原因与边界

Mac 上板使用旧 migration-4dma BIT 配合新版 Linux，出现 proc/dentry 引用计数异常及 generic_permission Oops，无法登录。原始日志、版本和 QEMU 对照在 `out/board-bringup-20260915-mac/`。

实板小程序执行 LR/SC 后，不再 LR 而再次 SC：低半部 SC.W 和 SC.D 均正确报告失败；高半部 SC.W 实际未写入却返回成功，1000/1000 复现。原内核 s_active 位于 superblock 偏移 188（低三位 4），使用 LR.W/SC.W 更新，与缺陷吻合。新 BIT 上板前仍不能宣称启动异常彻底解决。

旧 BIT 启用 WT_DCACHE。`cva6/core/cache_subsystem/wt_axi_adapter.sv` 在请求出队后把 `amo_off_d` 清零，延迟 B 响应的失败标志因此落到错误字节位置。修复为保持 `amo_off_q`；仅改变这一处状态保持逻辑，不改 Linux、IOMMU、DMA、频率或延迟设置。

CVA6 修复提交：`7f086b73f19d8e4d48ed74a671e8315f827d2ce0`。

## 回归证据

测试实例化真实生产 `wt_axi_adapter`、`axi_shim`、FIFO/仲裁器及原始 package，不复制被测逻辑。覆盖 SC.W 低/高半部、SC.D，成功/失败响应，0/3/17 周期 B 延迟，AW/W 同时与分离握手，并连续切换地址偏移。

- 旧 RTL：54 场景，6 失败（高半部失败响应、B 延迟 3/17，各三种 AW/W 时序）。
- 修复 RTL：相同 54 场景，0 失败。
- 证据：`out/board-bringup-20260915-mac/rtl-before-v2/`、`rtl-after/`，含源文件 SHA256、命令、构建/测试输出。
- 初始 `rtl-before/` 缺少 ariane_dm_pkg 依赖，编译失败，是测试搭建历史，不是回归结果。

复现：

```sh
python3 tests/run_wt_sc_test.py --root . --output out/wt-sc-regression-new
```

输出目录必须不存在。此测试不是完整 CPU/IOMMU 仿真，也不能替代上板验收。

## 独立 BIT 构建

`migration-4dma-scfix` 与 migration-4dma 的实验参数相同，仅用于隔离新构建目录。旧 `out/migration-4dma/` 不移动、不覆盖。

```sh
PATH=/tools/Xilinx/Vivado/2023.1/bin:$PATH XILINXD_LICENSE_FILE=2100@opus4.apal.cs.waseda.ac.jp make fpga PROFILE=migration-4dma-scfix JOBS=12
```

新产物目标：`out/migration-4dma-scfix/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit`。仅用于易失 JTAG 配置，不自动写 Flash。

自动记录构建的入口为 `scripts/build_scfix.py`，日志和状态写入 `out/board-bringup-20260915-mac/`，已有日志时拒绝重复启动；重试先核对现有进程和结果，不能删除旧日志后盲目重跑。

## 当前验收

源码修复与 54 场景定向回归已通过；新 BIT 构建、时序审查、Linux 登录和 DMA smoke/HPM 仍需分别记录。旧时钟/CDC/IO 约束审查待办未由本修复消除。

## 2026-09-16 SC 修复构建完成（最新结果）

新 BIT/MCS 构建已通过，退出 0；完成 UTC：2026-09-15T15:56:47.509135+00:00。

BIT：`/home/xvhaocheng/work/iommu-platform/out/migration-4dma-scfix/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit`；11443722 bytes；SHA256 `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3`。

正式 routed timing：WNS 0.177 ns、WHS 0.010 ns、WPWS 0.062 ns，TNS/THS/TPWS 均为 0。已设置的时序约束满足，但 6 项时钟 Critical Warning 及 IO/CDC 审查待办保留。

源码与 54 场景回归已通过，旧 BIT 未覆盖。新 BIT 尚未上板；Linux 登录和 DMA smoke/HPM 尚未验收，下一步用新 BIT 重新配置板卡，SD 不改。

## 后续实板记录（2026-09-16）

用户随后提供的 DMA_SMOKE.md 确认该 BIT 已上板，Linux 登录/proc 正常、taint=0；DMA smoke 遇到独立的 timeout 依赖及内容校验错误，不能据此把 DMA 全功能标记通过。最新诊断见 [DMA smoke 修正记录](DMA_SMOKE_FIX.md)。该后续诊断修改 Linux，需单独更新 SD 软件；本 SC.W 修复 BIT 保持不变。
