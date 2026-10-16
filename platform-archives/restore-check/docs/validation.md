# 验证记录（2026-09-08）

## 已完成

- 原工程 22 个已初始化仓库（包括旧 demo 顶层）的 Git bundle 已验证，另存 staged/working diff、未跟踪文件、关键旧构建产物及 SHA256。
- 新平台固定 21 个组件；所有组件工作树干净，各级 gitlink 与 lock 对应；没有 Git alternates 或指向旧目录的 origin。
- 当前四个用户态程序已交叉编译：idma_bench、idma_pair_bench、idma_multi_bench、idma_once。
- rootfs 打包成功并校验新程序内容，保留原有设备节点，无需 root 解包。
- Linux Image、DTB、lloader、OpenSBI fw_payload.bin 已从新目录完整构建成功。
- 实际 HPM RTL 的定向测试通过：pulse、held-valid、idle、DID filter、inhibit；独立 cycles 正常增长。
- 同一个 HPM 测试应用于旧 RTL，单脉冲后空闲检查失败：得到 10，预期 1。证明该测试能够识别旧问题。
- 新平台的 bundle 已在 `../platform-archives/restore-check` 完整离线恢复，并通过全部 21 个组件、各级 gitlink、工作树及 Git alternates 检查。
- Genesys 2 板卡定义已随源码保存，来源提交及文件哈希见 `configs/boards/provenance.json`。
- 仓库内的板卡定义已用 Vivado 创建内存项目验证，正确识别 `digilentinc.com:genesys2:part0:1.1`。
- FPGA 的 8 个 IP、主体综合、布局和布线成功，新 BIT（约 11 MiB）和 MCS（约 31 MiB）已生成，完整 `make fpga` 返回 0。
- 本次 FPGA 构建树的 2,332 个 Git 跟踪源码 blob 与锁定提交逐项一致，记录在 `out/migration-4dma/staged-source-audit.json`。

## FPGA 报告

`migration-4dma`，Genesys 2 / xc7k325tffg900-2，Vivado 2023.1。布线后的正式 timing summary：

| 项目 | 结果 |
|---|---:|
| WNS / TNS | 0.177 ns / 0 |
| WHS / THS | 0.038 ns / 0 |
| WPWS / TPWS | 0.062 ns / 0 |
| 未约束的内部端点 | 0 |
| LUT（布局后） | 132,584 / 203,800，65.06% |
| Slice（布局后） | 47,974 / 50,950，94.16% |

Vivado 判定当前已设置的时序约束全部满足。这不等于约束完整性或全部 CDC 检查通过：

- check_timing 仍报告 21 个输入缺少 input delay、23 个输出缺少 output delay、3 个部分 input delay。
- methodology 报告 6 项 Critical Warning：TIMING-4 ×1、TIMING-6 ×2、TIMING-7 ×2、TIMING-27 ×1，涉及内部时钟重定义、相关时钟没有共同主时钟／共同节点。需单独审查原有时钟约束，不能仅凭正 WNS 宣布完整时序签核。
- 综合时 `clk_out2_xlnx_clk_gen` 查询为空的两条警告，在完整网表中对应时钟及异步分组已存在；见另存的 `out/migration-4dma/reports-synthesis/ariane.clock_interaction.rpt`。
- 上游清单还生成了未实例化的两个 DMA 宽度转换 IP；其 XDC 没有作用于任何实例的警告已保留，未通过降级警告隐藏。

详细布线报告位于 `out/migration-4dma/fpga-src/cva6/corev_apu/fpga/work-fpga/`，与 BIT/MCS 一起保存。上游 MCS 步骤重新创建 Vivado 项目，原来的 `ariane.runs/impl_1` 不作为归档位置。

## 待完成

- 原有时钟定义、CDC 与外部端口约束审查，具体项目见上述 FPGA 报告。
- 新 bitstream 上板、四路 Solo、大传输无 hang、多设备并发、DID-filtered HPM 端到端校准：尚未执行。本机未发现 ttyUSB/ttyACM 串口设备。
- 无人为延迟 PROFILE 的 FPGA 构建与性能实验：尚未执行。

完整许可证地址 `2100@opus4.apal.cs.waseda.ac.jp` 已验证可直接使用。

编译成功不证明硬件功能通过，也不替代约束完整性审查。完成上述审查并保存新 bitstream 对应的板上测试日志后，才能给平台标记为 validated baseline。

详细输出位于 out/manifests、out-hpm.log、out-software.log、out-fpga.log。软件已在干净的顶层提交 `de942152a8e7` 上重复增量构建通过；以实际 manifest 完整提交号为准。本次 FPGA 启动于平台整理期间，旧版入口的 manifest 中顶层状态反映结束时状态；固定组件与实际导出源码另有逐项校验记录。启动时使用本机板卡定义，其内容与随后保存到 configs/boards 的文件相同；新版入口显式使用仓库中的板卡定义并记录启动状态。主体逻辑未在构建中修改。
