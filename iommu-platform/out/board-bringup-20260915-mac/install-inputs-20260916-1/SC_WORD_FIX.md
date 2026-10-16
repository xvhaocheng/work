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
