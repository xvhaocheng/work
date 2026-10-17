# E2 timer probe：诊断软件构建候选，尚未 finalize

本批只完成 source tuple 固定、原输入准备和 30 项有限主机检查。实际输出是 `artifacts/e2-timer-probe-software01/`，状态 `prepared`、`commands=[]`；未导出构建源码、未编译 Image/OpenSBI/SPI 模块、未执行板操作。Root 审查实际代码后才决定是否运行一次 finalize。探针的 runtime 错误码、每次 open 的 snapshot 生命周期及实际成本仍无板验证；本批也不构成根因或性能结论。

## 固定源码与实际差异

| 项目 | 固定身份及范围 |
| --- | --- |
| 平台基线 | `ec99648e3c9a1e48c1e9721e3662bb632eb439bd` |
| 平台候选 | `5baab2217eb4877da29d5c38f12749a80c5cdf4f`；独立 worktree `/home/xvhaocheng/work-e2-irq-diagnosis-platform`，branch `ispass/e2-irq-diagnosis` |
| 平台改动 | 只改 `linux` gitlink 和 `sources.lock.json` 的 Linux SHA，共两行；[实际 patch](platform-probe.patch)、[bundle](platform-probe.bundle)、[锁快照](sources.lock.snapshot.json) |
| Linux | `65b427946152e3aca27babab0a437821729805d4` → `1facb309758e3d41622355de71dbc033cd0fac97`，只改 `drivers/clocksource/timer-riscv.c`；探针原件、权限证据与 off 路径验证见 [probe-v1](../probe-v1/README.md) |
| OpenSBI | 保持 `e8d21bfdc2dd753314627968e496c3a1188f6dfb` |
| CVA6 | 软件 manifest 引用保持 `1d587125eee690e623e6ffae13e70f9bbe989adc`；这是源码引用，不是已加载 BIT 身份。既有 E2 BIT 实际 CPU 源为 `8aac3fcd345af7686a55af1867783fd6b91a0069` |
| 诊断 release | `6.5.0-rc2-ispass2027-e2-timer-probe-v1` |

[builder 实际代码](build_timer_probe.py) 来自 [原 passed full-E3 builder 字节副本](source/build_full_software.as-passed.py)；[完整差异](builder-vs-full-e3.diff)可直接逐行审。复用 helper 字节副本 SHA `5a7f410d…`，archive/parser 的实际工作树文件须与固定平台源码逐字节一致才允许使用。helper 的其他入口不由本 builder 调用；实际 finalize 调用只包含源导出、配置、Linux/lloader/OpenSBI 与 SPI 模块构建。平台无 remote，故用 bundle/patch 交付本地 commit，不向上游组件库推送。

## 原 passed 软件输入：不重建 CLI、不重打包 rootfs

唯一软件输入为 `/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/full-e3-software01/`。其 `build-manifest.json` 原 SHA 是 `7415dcc4d076fa434bc22294f4309582af84086964f1b47bb05c695555f0bcc4`，状态 `passed`。原 manifest、软件 tuple、7 个 CLI 哈希及输入角色见 [source-manifest](source-manifest.json)。

rootfs 直接复制原字节，7,888,384 bytes，SHA256 `784a147961a9a51a49dd451bba2aaae72f937dd396eeb2a6fbfb940b4788800d`。保留 `idma_experiment`、`idma_p0`、`idma_pages`、`idma_scaling`、`idma_workload`、`platform-sd-update`、`platform-timeout` 全部原 binary；检查它们与原 manifest、CPIO 中唯一同名路径的正文/模式一致，CPIO 本身也须匹配原 SHA。原 CLI source 与平台5baa一致，UAPI 与 Linux1fac一致。

实际 passed defconfig/DTS 是 full-E3 从 baseline 继承的输入，不是 ec996/5baa 模板导出：[defconfig 原件](source/linux_defconfig.baseline-input) SHA `cb3e51c0d7f018dae35990bdc1226456aee01beb4e77892990c12fc6581348eb`；[DTS 原件](source/platform.baseline-input.dts) SHA `cb61ac9ec1c9d2f50c111fbebb3bb09ed41ae7bdce72ca844580caf892131f16`。二者按实际 passed manifest 锁定；平台模板保持原样。[逐行角色对照](validation/input-role-comparison.json)记录差异：平台模板多出三个网络/SPI config 行；SPI/以太网 interrupt cell 写法也不同。本批没有修补这些输入。

首次 preflight 曾错误要求继承输入也与平台模板相等，因 defconfig 不同退出1；当时尚未创建输出或编译。[保留的工具输出 transcript](validation/first-preflight-rejection.transcript.txt)明确其来源和归档局限。后续只修正身份角色核对，未为过门改配置。当前 [实际 prepared manifest](prepared-manifest.json)对应修正后唯一成功 prepare。

## 构建约束与有限验证

`prepare` 在创建输出前验证固定 tuple、source lock/gitlink、原 passed manifest、输入/CLI/UAPI/rootfs；输出只能位于本任务 `builder-v1/artifacts/` 子目录。准备阶段仅复制原字节，没有 compiler 调用。

`finalize` 仅接受原 `prepared` 包；builder SHA 或输入变化即拒绝。开始前写 `building`，构建异常持久化 `failed`，已有 `building/failed/passed` 均拒绝重试或覆盖。沿用原 fresh Linux/OpenSBI archive、`Image → dtc → lloader → OpenSBI` 顺序，payload 必须在 `1..33,554,432` bytes；超限即失败，不改配置。原 payload 为 29,363,656 bytes、余量 4,190,776 bytes；新诊断 payload 大小仍未知。

原 raw defconfig 经 `olddefconfig` 后与原 passed 完整 `.config` 比较，只允许 `CONFIG_INITRAMFS_SOURCE` 的绝对路径与 `CONFIG_LOCALVERSION` 的诊断后缀不同。HZ/PREEMPT/watchdog/taint/DMA 源码或预算均无改动。DEBUG_FS 在原完整 config 中已为 `y`。SPI 恢复保留原 `spi-bitbang.c`/`spi-xilinx.c` 字节；使用最终内核 release/exports、先 `modules_prepare`、核验 `module.lds`，再用 `vmlinux.symvers` 构建配套模块。模块只供软件更新，不能作为正式测量加载项。

[主机检查代码](check_builder.py)和 [30 项实际结果](validation/host-checks.json)覆盖正确 tuple/input、错误平台/Linux/lock/gitlink、rootfs/CLI/DTS/UAPI 篡改、builder/tuple 篡改、非 prepared 状态、HZ/watchdog 变化、输出范围/已有输出、0/-1/超32MiB 边界。篡改只作用于临时副本；检查禁止 archive/run，未调用 finalize，实际 prepared manifest 保持原字节。未验证真实构建失败后的落盘行为、实际最终 config/payload/module 结果、SPI 加载或探针 runtime。

复核命令（只读主机检查）：

```sh
python3 /home/xvhaocheng/work-e2-irq-diagnosis/paper/ispass2027/exp/E2-device-scaling/irq-diagnosis/builder-v1/check_builder.py \
  --prepared /home/xvhaocheng/work-e2-irq-diagnosis/paper/ispass2027/exp/E2-device-scaling/irq-diagnosis/builder-v1/artifacts/e2-timer-probe-software01 \
  --report /home/xvhaocheng/work-e2-irq-diagnosis/paper/ispass2027/exp/E2-device-scaling/irq-diagnosis/builder-v1/artifacts/host-checks-repeat.json
```

Root 审过本批实际 builder 并明确放行后，唯一候选构建命令是：

```sh
python3 /home/xvhaocheng/work-e2-irq-diagnosis/paper/ispass2027/exp/E2-device-scaling/irq-diagnosis/builder-v1/build_timer_probe.py finalize \
  --out /home/xvhaocheng/work-e2-irq-diagnosis/paper/ispass2027/exp/E2-device-scaling/irq-diagnosis/builder-v1/artifacts/e2-timer-probe-software01 \
  --platform-commit 5baab2217eb4877da29d5c38f12749a80c5cdf4f \
  --linux-commit 1facb309758e3d41622355de71dbc033cd0fac97 --jobs 8
```

本批尚未执行此命令。后续是否构建和最多一个低压控制加一个 N4_A8 诊断点，均由 Root 审核后另排；诊断点不进入正式论文性能数据。
