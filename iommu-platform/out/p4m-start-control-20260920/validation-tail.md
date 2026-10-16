
范围严格限定软件计时E1、DS50/50。完整E0/P2仍未签核：S2正向、溢出IRQ、空窗口开销和硬件请求重叠缺接口，DDTW相邻事件漏计及时钟/CDC/IO待办保留；PTW字段不是独立页表访存量。无纯硬件慢化/HOL结论。下载哈希/SD回读按用户明确要求未做，不将期望哈希写成实测值。


## 2026-09-16 首错回传及 DMA 缓存同步修正版（最新）

诊断版已上板：新启动 warmup32 通过，随后完整 smoke 首组 warmup0 失败、余13组未运行。offset912 的 dst=eb 经 FENCE.I 变为期望14，源始终14，支持旧缓存视图；Linux正常、taint0。两条JSON和首错已到服务器，完整串口仍在Mac。

修正及边界见 [DMA 缓存同步](../DMA_CACHE_SYNC.md)：DMA前与安全排空后各同步一次，均在测量窗外；另修正软件 IOVA→PA 查询的页内偏移，不将旧诊断大地址误作硬件翻译。Linux `3b47ab1645e4`，构建顶层 `76de9ee73656cc90bdae5331be7020b5f905e205`，内核 `6.5.0-rc2-00080-g3b47ab1645e4`。19个调度/诊断/缓存场景、96小chunk组合、16软件查询场景通过；删除任一同步能被回归检出，旧软件查询12/16失败、修正后0/16失败。内核目标、checkpatch、21组件版本检查、完整软件构建和交付内容/哈希核对通过。保留原DTS警告及用户文档脏改动。

交付 `out/dma-cache-sync-20260916/delivery/`，payload SHA256 `6f52ad85c4d2fc87e97301d98f1394216b843cd8be601b2965eca429caf74053`；沿用SC.W修复BIT，未重建FPGA。**本轮缓存同步版尚未上板，数据故障未宣布解决。** 下一步更新SD软件，按交付说明覆盖两种启动/调用顺序的完整smoke；再做HPM校准，不能混用修复前后性能数据。以下诊断和上板准备状态为历史记录。

## 2026-09-16 实板 DMA smoke 失败与诊断

用户提供的后续记录确认 SC.W 修复 BIT 已启动到 Linux，登录/proc 检查正常、taint=0；DMA 功能验收仍失败。原 smoke 缺 BusyBox timeout，在提交前退出127；临时兼容脚本只跑两例，DMA0 单页8次 warmup0通过、warmup32数据校验EILSEQ，其余12例未执行。收到的是摘要，原始串口和JSON尚未传到本服务器。

定向核查、最小首错/缓存探针和后续诊断交付见 [DMA smoke 修正记录](../DMA_SMOKE_FIX.md)。源码查到缓存一致性契约缺口，但还没有把它确认为此次失败的唯一根因；未将主机测试、诊断探针或此前SC.W修复解释为DMA数据问题已解决。

最小诊断 Linux `53d3113dee74`、顶层构建 `38774213f7cb8f3ea4240e6e127e363ae5f6be02` 已完成软件构建；16 个调度/诊断场景、96 小 chunk 组合、9 项 watchdog、5 项 rootfs 检查通过。交付 `out/dma-smoke-debug-20260916/delivery/`，内容/架构/SHA256 核查通过；旧交付保留，新版尚未上板。源码组件全部固定，已有顶层文档 dirty 状态保留并记入 manifest。

## 2026-09-15：更新软件交付

- `make software JOBS=12` 退出 0；已更新 `out/experiment-v1/` 的 Image、rootfs、OpenSBI payload、用户程序、smoke、manifest 和版本/SHA256 清单，纳入小 chunk 修复及多页部分覆盖 smoke。
- 源码：顶层 `0bb9851bd12fc054bc40d217756d6f0b3662badb`，Linux `031e91dbadc115dafac957067135002b74035a10`。顶层已有研究文档改动，dirty 状态保存在构建清单中。
- 旧交付完整保留在 `out/experiment-v1-before-20260915T140327Z/`。本次日志 `out-experiment-software-20260915.log` 仅保存，按用户要求未逐项审查；既有主机测试未重跑，未重建 FPGA 或上板。

## 2026-09-09：最小软件修复

- 按用户要求仅修改数据模式，使用唯一 DMA 实例编号标记每个字节，修复小 chunk 短试次漏检另一设备源的问题。未扩展 ABI、CLI、调度或 HPM；RTL 未改。
- `python3 tests/test_idma_scheduler.py`：原 12 个调度场景和新增 96 个正常/错源组合全部通过。组合为 chunk=1/2/4/8 × DID 10～13 的 12 个不同有序配对 × 正常/错源；未修代码先出现 36 项预期失败。日志：`/tmp/idma-small-chunk-regression-{red,green}.log`。
- 现有 `board-smoke.sh` 每路只加一条多页命令：8 页中前 4 页各拷贝一次，余下 4 页保持未触及并接受校验。`sh -n tests/board-smoke.sh` 与 diff 检查通过；板上尚未执行。
- 驱动目标交叉编译通过：`make -C linux ARCH=riscv O=/home/xvhaocheng/work/iommu-platform/out/linux CROSS_COMPILE=riscv64-linux-gnu- -j4 drivers/misc/idma.o`。日志：`/tmp/idma-minimal-fix-build.log`。
- Linux 修复提交 `031e91dbadc115dafac957067135002b74035a10`；顶层测试/gitlink/lock 提交 `0bb9851`，均仅保存在本地。原研究文档继续保留。
- **当日仅完成目标编译；完整软件交付已于 2026-09-15 更新，见上一节。** 以上验证不代表真实 DMA/MMIO、板上功能或机制证据。HPM 相邻事件的 RTL 问题仍待修。

## 2026-09-09：实验计划与 v1 实现复审

- CLI 65 组、生产调度 12 场景、HPM 会话 25 组主机检查重跑通过。
- 新增生产函数主机注入：无预热、4 字节短试次交换两路 DMA 模拟源缓冲，实际校验仍返回成功；8 字节对照检出 EILSEQ。说明小 chunk 的模式归属校验有盲区，不否定默认完整 4 KiB 对该错源的检测。
- 新增实际 HPM RTL 定向仿真：UT_REQ→DDTW 连续 valid，DDTW 得 0/预期 1；加低 valid 间隔对照得 1。事件序列经翻译 FSM 静态核查，未进行完整翻译逻辑集成仿真。当前 v1 六槽不含 DDTW，不能推断它们全部计错。
- 审查阶段两项问题未修生产代码（后续软件修复见上一节）；复现程序的退出 0 表示成功复现缺陷/对照，不是生产正确性通过。源文件、结果、版本与 SHA256 见 [审查证据](../out/review-v1-20260909/README.md)，详细影响和建议见 [实现复审](experiment-v1-review.md)。
- 文档补入方法工作包、明确 E2/P2 依赖和 HPM 字段语义。Linux/CVA6/IOMMU HEAD 未变且与 lock 一致、工作树干净。未执行完整软件/FPGA 构建或上板实验。

## 2026-09-09：研究计划独立审查

完成[投稿导向审查](research-review-2026-09-09.md)，仅新增研究记录并更新交接。定向核对 DMA 分配/调度/预热、IOTLB 与事务控制状态机；顶层 HEAD 仍为 `25e452a7861f2208056180fb1faa7b9c0f5c1606`，Linux/CVA6/IOMMU HEAD 与 lock 一致、组件工作树干净。保留已有未跟踪研究文档。未改源码、运行构建/仿真或采集板上数据；以下既有验证边界与待办不变。

## 本轮：双路实验入口 v1

已用版本化 `idma_experiment` 替换四个旧 benchmark，修改 Linux 的测量调度与 per-DID HPM 会话；CVA6/IOMMU RTL 未修改。实现和指标解释见 [实验入口 v1](experiment-v1.md)。

- 主机测试通过：CLI 65 组、生产调度 12 场景、生产 HPM 控制 25 组、rootfs 4 项。
- 定向内核交叉编译通过；生产驱动/新 HPM 文件样式检查及 diff 检查通过。
- HPM 主机测试检出并验证修复两个索引宏的运算优先级错误，避免 Aggressor PTW 读取错误寄存器。
- `make check` 和 `make software JOBS=12` 通过。构建起止均为干净顶层提交 `ffe55a40175937122b3d6be92a6c005099f16dfd`，Linux 为 `affbfbf6adda41d5b33f7b36880279d8ee8f48f7`；21 个组件、gitlink 和 lock 一致。
- 新程序交叉编译、rootfs 内容/去重检查、Linux Image、DTB、lloader 与 OpenSBI payload 全部构建成功。DTC 保留原有 interrupt provider 缺少 `#address-cells` 的警告，本轮未修改 DTS。
- [软件交付目录说明](../out/experiment-v1/README.md) 与 [版本和 SHA256 清单](../out/experiment-v1/delivery.json) 已生成；目录内 `sha256sum -c SHA256SUMS` 全部通过。新软件 manifest 为 `out/manifests/software-migration-4dma.json`，日志为 `out-experiment-software.log`。
- 此前镜像和旧可执行文件保存在 `out/pre-experiment-v1-20260908T124529Z/`，带 SHA256 清单。原 BIT/MCS 哈希与历史清单一致，本轮没有重新构建 FPGA。
- 用户确认当前主机未连接 FPGA，也尚未准备 SD 卡。新版 smoke、per-DID 端到端校准和正式实验均未上板执行；继续使用原来的 SD 启动方向，实际介质布局待准备时确定。

以下为本轮替换前的平台迁移验证记录。旧软件 manifest、旧 `out/migration-4dma/delivery.json` 不代表新版软件；其中 FPGA 报告及 BIT/MCS 仍保留。

## 历史：平台迁移已完成

- 原工程 22 个已初始化仓库（包括旧 demo 顶层）的 Git bundle 已验证，另存 staged/working diff、未跟踪文件、关键旧构建产物及 SHA256。
- 新平台固定 21 个组件；所有组件工作树干净，各级 gitlink 与 lock 对应；没有 Git alternates 或指向旧目录的 origin。
- 当时四个用户态程序已交叉编译：idma_bench、idma_pair_bench、idma_multi_bench、idma_once（本轮已退役）。
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
- 上游清单还生成了当前 RV64 配置未实例化的两个 Debug Module 宽度转换 IP（`xlnx_axi_dwidth_converter_dm_master/slave`，仅在 `XLEN == 32` 时使用，不是 iDMA）；其 XDC 没有作用于任何实例的警告已保留，未通过降级警告隐藏。

详细布线报告位于 `out/migration-4dma/fpga-src/cva6/corev_apu/fpga/work-fpga/`，与 BIT/MCS 一起保存。上游 MCS 步骤重新创建 Vivado 项目，原来的 `ariane.runs/impl_1` 不作为归档位置。

## 待完成

- 原有时钟定义、CDC 与外部端口约束审查，具体项目见上述 FPGA 报告。
- bitstream 上板、四路 Solo、多设备并发、DID-filtered HPM 端到端校准：尚未执行。新版入口先运行新的 `platform-smoke`；大描述符回归暂不由这个只支持页内 chunk 的入口覆盖。
- 无人为延迟 PROFILE 的 FPGA 构建与性能实验：尚未执行。

完整许可证地址 `2100@opus4.apal.cs.waseda.ac.jp` 已验证可直接使用。

编译成功不证明硬件功能通过，也不替代约束完整性审查。完成上述审查并保存新 bitstream 对应的板上测试日志后，才能给平台标记为 validated baseline。

详细输出位于 out/manifests、out-hpm.log、out-software.log、out-fpga.log。软件已在干净的顶层提交 `de942152a8e7` 上重复增量构建通过；以实际 manifest 完整提交号为准。本次 FPGA 启动于平台整理期间，旧版入口的 manifest 中顶层状态反映结束时状态；固定组件与实际导出源码另有逐项校验记录。启动时使用本机板卡定义，其内容与随后保存到 configs/boards 的文件相同；新版入口显式使用仓库中的板卡定义并记录启动状态。主体逻辑未在构建中修改。
