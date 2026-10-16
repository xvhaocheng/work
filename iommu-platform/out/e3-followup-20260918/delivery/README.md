# E3 v5 等待识别与带诊断配对：完整交付

2026-09-18：软件和 FPGA 完整构建均退出 0、manifest passed，BIT/MCS 非空，清单哈希及关键导出源码与已测试源码一致。新版尚未上板。成功日志只保存，未审查时序报告，原时钟/CDC/IO 审查边界保留。

必须同时使用本目录的新 BIT 和 SD payload。保持 DS50/50、V0/A1、4 KiB 硬件接续及 CPU 整批轮询。

- `ariane_xilinx.bit`：observer v5 / E3 bank v2，修正 V 排在 A 读之后时的触发识别，加入门槛、前史和两条插入请求记录。
- `fw_payload.bin`：按已有 SD 原始分区流程更新；内核 `6.5.0-rc2-00086-gd2cd32d78918`。新程序已内置 rootfs，不能只在旧系统替换 CLI。
- [E3_QUEUE_PAIR.md](E3_QUEUE_PAIR.md)：本轮逻辑、完整字段、三对短验收与十对正式矩阵。新配对 `--diagnostic 2`；D0/D0 对照仍用 `--diagnostic 1`。采集器需接受新 schema 和 bank v2。
- [结果分析](e3-diag-results-review.md)：已完成的 00085 D0/D0 数据及解释，和新版实板结果区分。
- `idma_e3_pair`、`idma_experiment`、`platform-timeout`、`idma.h`：程序和配套接口；其余 JSON 与 `SHA256SUMS` 保存版本、验证和校验记录。

BIT SHA256：`8f90865ef1fb4f747e7a1f16c3a4992adf4499c56356e0d8d1679b8962d7342e`。
payload SHA256：`13dc664895f570925b6ae59f11cedbfefcb3de331c0d9d6518557b7ce4a7ea36`。
构建起点：`01199098590aac8db92c98a92f700ce4dc43da35`；CVA6 `147c4d4a825ef000b950a214bfeb08228d08f7ba`，IOMMU `137f5e5fb1f2cc65a1346359226b16247c3f8445`，Linux `d2cd32d78918fb84496ea0b8524464d0dfb27163`。文档后续提交不改变被编译组件。原文档脏改动保留在 manifest 的 dirty 标志中。

主机验证：真实控制器/FIFO/demux/等待计数/延迟开关各15场景；E3及旧observer/path RTL回归；驱动76控制+3缓存+96小chunk；E3 CLI49、旧CLI69、rootfs5。模型与编译成功不能代替上板验收。新增触发前摘要/队头需在结果回收时继续核对语义，不能只凭 success 判断因果成立。

先三对短验收，通过后仅十对正式：2对A8 D0/D0、6对A8 D0/D128交替首轮D、2对A1控制，顺序见协议。保存26子轮的全部JSON/diagnostic/256个E3字段、按sequence收齐494条观测、UART/退出码/taint和版本来源。真实错误停，无触发或不可比不补采。若可比性仍不足，再设计有限请求或共同启动，本轮不扩展。

历史原始数据保留在 Dell 工程中，分析文中的原始数据链接未随交付复制。软件日志在上一级 `software.log`，FPGA 成功日志在 `fpga.log`；首次许可证连接失败的日志及构建树另存，未覆盖旧实验产物。MCS 保存在 delivery.json 的 `mcs_source_path`，上板常用本目录 BIT。没有本机刷卡、JTAG 或板卡操作。

DeepSeek 整理原始证据并提供 CLI/桩/CLI 测试提案；Codex 独立复核原始数据与关键代码，负责 RTL/驱动、应用审查提案、运行验证与编译。
