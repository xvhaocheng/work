**指定清单与默认位置中，最近的实际生产构建仍是2026年10月7日 composition；未找到晚于它的登记BIT。** 用户现场确认“当前bit就是最近的编译结果，也是当前工作区代码”作为用户证据保留；本席没有把该确认转换成实测的BIT／源码认证。root正在做设备验证，运行中observer差异原因也不由这次文件索引推断。

`out/manifests` 顶层37项中，18份 `fpga-*.json` 已定向查看。带构建timestamp的最新条目是 [composition manifest](/home/xvhaocheng/work/iommu-platform/out/manifests/fpga-migration-4dma-composition.json:2)：2026-10-07T02:26:15Z、action=fpga、status=passed。:163指向独立stage，:32登记原版BIT哈希 `f15f8a72…`。这份manifest的passed并不包含最终时序裁决：[执行记录](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/execution-state.json:33) 给出原版WNS−0.219ns、setup失败1；后来接受的是另一份route-opt输出。

| 文件角色 | 本次实际修改时间 UTC | 本次SHA256 |
|---|---|---|
| [默认migration BIT](/home/xvhaocheng/work/iommu-platform/out/migration-4dma/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit) | 2026-09-08 11:07:34 | `cd49d4aba0195d5176618fb7d03071154bcd5b347425b43f854945149cceed70` |
| [最新登记composition原版BIT](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit) | 2026-10-07 02:25:59 | `f15f8a72c02da3f19e4ed5beafe3bae61407636b99505d9ace31bf0f9716715a` |
| [最新已接受route-opt BIT](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/route-opt/ariane_xilinx.bit) | 2026-10-07 02:30:01 | `f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca` |

三份BIT均11,443,722B，文件大小不能区分。[route-opt结果](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/route-opt/result.json:5) 指向第三份，:8/9记WNS+0.022／WHS+0.051ns，失败端点和布线错误为0；[上板接受记录](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/board-acceptance.json:16) 同样指向第三份。执行记录:40记优化完成于10月7日02:30:03Z。此次只核清单与实际文件，没有重跑原构建或历史实验。

默认入口由 [Makefile](/home/xvhaocheng/work/iommu-platform/Makefile:2) 的`PROFILE=migration-4dma`和:14调用确定；[platform.py](/home/xvhaocheng/work/iommu-platform/scripts/platform.py:235) 默认stage为`out/<profile>/fpga-src`，:295产物在stage内部`cva6/corev_apu/fpga/work-fpga`。CLI允许:367的`--fpga-stage`，必须在out内、启动前不存在，实际stage写入manifest。裸工作树`cva6/corev_apu/fpga/work-fpga`和`out/fpga-src`此次都不存在。清单按`<action>-<profile>.json`覆盖同名记录，不是完整时间序列；手动构建或未登记自定义路径在本次范围之外，是否有更新BIT仍为未知，不能据此做全工作区／全机器否定。

[机器记录](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/current-workspace-bit-candidates.json) 包含精确绝对路径、mtime、实测hash、用户确认与判断边界；[原文及源码行号](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/current-workspace-bit-provenance.json)、[限定manifest索引](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/current-workspace-fpga-manifest-index.json)、[实际账本](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/current-workspace-bit-ledger.json) 留供root亲核。本席仅Codex只读核查，零设备操作／重建／源码改动。
