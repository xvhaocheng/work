# 论文平台当前状态（精简版）

## 2026-09-20 P4M 板上短验收完成，复核脚本修订中

P4M 板上短验收已执行：`platform-p4m short /tmp/p4m-v7-01`，Solo/A1gap0/A8gap0/A8gap1000 四例 64 jobs、warmup 8、4 KiB，板卡退出 0、final taint=0、collection_failed=0。原始证据：[console.raw.log](iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/console.raw.log)、[records.json](iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/records.json)、[run.json](iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/run.json)。复核脚本与报告：[p4m-short-results-review.md](iommu-platform/docs/p4m-short-results-review.md)、[review.json](iommu-platform/out/p4m-short-review-20260920/review.json)。修订后复核结果：{{REVIEW_RESULT}}（待实际运行填充）。

版本仍为 P4M profile `migration-4dma-p4m`/observer v7，内核 `6.5.0-rc2-00088-g9f8da9e7a6a0`，CVA6 `e822bc9d8adf246a5d098585fb229744bafd9cc1`，Linux `9f8da9e7a6a08fb19134146241018eaff0eee124`，IOMMU `137f5e5fb1f2cc65a1346359226b16247c3f8445`。短验收只做功能验收，不证明正式收益；n=1。

P4M-1/P4M-3 短验收功能完成；P4M-2 的 legacy `--batch` 与 `--p4m` gap0 显式对照本轮未做（本轮全为 `--p4m`），P4M-4 预试为下一步。正式 gap 点与重复数未冻结；不做全矩阵或复杂硬件方法。

## 2026-09-18 E3 v6 上板及服务器复核完成（历史）

E3 v6 / Linux00087 已完成 3 对短验收 + 8 对正式实验；服务器独立复核 exit 0、9 组检查通过。11 对 / 22 轮、418 条唯一 UART、114 条相同重复去重，11 份完整 CLI JSON 与 UART 对象逐字段一致，原始输入哈希未变；8 项程序检查（正常对照 1、故障注入 7）均符合预期。原始回传在 [mac-e3-controlled](iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/)，[复核证据](iommu-platform/out/e3-controlled-review-20260918/server-review.json)。run.json 的 pending_upload 是上传前快照；现已在 Dell，未宣称验证 upload receipt。

核心结果：4 对 cold 的 D128−D0，A 剩余服务和 V 剩余等待均增加 127/129/129/130 周期，V 自身翻译恒为 4 周期；14 个 cold 轮次 V 被选中均比 A 翻译响应晚 9 周期。身份、目标 A 的 PTE、前缀计数和完整插入身份配对一致，V 首查命中；D0/D0 的剩余等待差为 −2/0。结合串行控制器/AR FIFO 代码，支持本实现、此配置下 A miss 引起 V hit 等待的机制。样本仅 4 对，预触发耗时并非全部相同；不宣称完整 E3 矩阵完成、所有旧 A8 损失已解释或已有方法收益。[结果分析](iommu-platform/docs/e3-controlled-results-review.md)。

整体方向保留，当前优先推进 P4M：共同 V 硬件窗口内的 A 完成量与边界在途计数，以及 A 完整拷贝结束后的硬件固定间隔基线。先验证 gap0/短正确性，再预试、冻结少量参数并画 V slowdown—A 完成速率曲线；具体间隔和重复数待预试，不自动扩旧 E3 矩阵。E2 按证据触发，P5 在规则冻结后评价。[下一步实施卡](iommu-platform/docs/e3-next-method-plan.md)。该阶段只做分析程序和文档；DeepSeek 提供程序/文档草案，Codex 核对关键证据与 RTL、做科学决策并实际执行复核。

## 平台、版本与证据边界

- 开发目录 `iommu-platform/`；Genesys 2、50 MHz、4 DMA、IOTLB 16、DS50/50，CPU 保留完成轮询。原工程和 archives 只作历史参考。
- 最新板上版本：CVA6 `2b04936d`、Linux `2de370cd344e` / 00087、IOMMU `137f5e5`，observer v6。上板与回传由 Mac 完成，Dell 本机仍未接板。
- [v6 配套产物](iommu-platform/out/e3-controlled-20260918/delivery/README.md)的软硬件构建已通过；该阶段未改实验源码或重新构建。旧 build-status 的 board 状态是交付时快照。
- 时钟/CDC/外部 IO 审查、下载哈希和 SD 回读仍有未验证边界；本轮 run.json 的 smoke_passed/hpm_calibrated 为 null，不补写通过。详见 [验证记录](iommu-platform/docs/validation.md)。
- E1 共跑现象、硬件连续供给、DS 回切复现及分段等待定位证据保留。v6 是独立 64 B 机制诊断；下一阶段回到既有连续 4 KiB 工作量，两类数据不混算。
- 已获得范围明确的 E3 干预证据，尚无经过评价的缓解方法或已确认创新点。总体目标仍是方法论文；E2 与剩余 E3/E4 分支按方法设计需要展开。

## 工作规则

- 开任务先读本文件；需要行为约定读 [AGENTS.md](AGENTS.md)，平台内读 [iommu-platform/AGENTS.md](iommu-platform/AGENTS.md)。
- 按任务定向读说明和代码；不要默认扫描整个 Linux、`.git`、`out/` 或恢复副本；没有新改动/验证需求时不重跑已完成完整构建。
- 核查版本先用 `git --no-optional-locks status --short`、`git rev-parse HEAD`，再对照平台 `sources.lock.json`。`make check` 会写 `out/manifests`，不是严格只读。
- 组件修改按实际依赖顺序提交并更新父 gitlink 和 lock：IOMMU → CVA6 → 顶层。不要重复应用 `provenance/` 中已纳入提交的补丁。
- 构建成功只证明构建；仿真/主机模型不等于板卡验收；板上结果才写板卡结论。日志默认只保存，有实际错误再读。
- 当前构建/恢复脚本不自动烧板、写 SD、推送 GitHub 或升级依赖；不要假定本机接了 FPGA/SD。板上操作沿用已有流程。
- 旧 bundle/恢复副本覆盖迁移阶段，不能假定包含最新实验提交；迁移新实验前重新运行 `make bundle`。这里没有个人远端，直接 `clone --recursive` 不保证取到本地提交。
- 完成实质工作后同步更新本文件的当前状态，并在 [HANDOFF.md](HANDOFF.md) 中保留阶段进展与证据；详细过程链接到专门文档和 [validation.md](iommu-platform/docs/validation.md)，不复制完整日志。
- 研究目标是争取方法论文，尚无已确认创新点；论文逻辑与优先级见 [paper-logic.md](iommu-platform/docs/paper-logic.md)。

## 最小按需阅读路线

- 新任务入口：[AGENTS.md](AGENTS.md) → 本文件。
- 平台使用/构建：[iommu-platform/README.md](iommu-platform/README.md)、[build.md](iommu-platform/docs/build.md)。
- 最新 E3 v6 结果：[结果分析](iommu-platform/docs/e3-controlled-results-review.md)；下一步：[P4M 实施卡](iommu-platform/docs/e3-next-method-plan.md)、[P4M 基线](iommu-platform/P4M_BASELINE.md)。
- 已执行 v6 协议：[E3_CONTROLLED.md](iommu-platform/E3_CONTROLLED.md)；旧 v5：[E3_QUEUE_PAIR.md](iommu-platform/E3_QUEUE_PAIR.md)。
- E3 原协议/字段：[E3_SERVICE.md](iommu-platform/E3_SERVICE.md)；D0/D0 软件诊断字段：[E3_DIAGNOSTIC.md](iommu-platform/E3_DIAGNOSTIC.md)。
- 当前代码地图：[repository-map-2026-09-18.md](iommu-platform/docs/repository-map-2026-09-18.md)。
- 结果复核：[e3-diag-results-review.md](iommu-platform/docs/e3-diag-results-review.md)、[e3-results-review.md](iommu-platform/docs/e3-results-review.md)。
- 验证历史：[validation.md](iommu-platform/docs/validation.md)；补丁来历：[patches.md](iommu-platform/docs/patches.md)。
- 各阶段工作与证据：[HANDOFF.md](HANDOFF.md)；清理前快照：[历史归档](docs/history/HANDOFF-2026-09-18-before-cleanup.md)。

## 历史任务来源

- 需要历史决策原文时定向查源任务：当前标题「平台构建」，曾称「整理论文工作文件夹」；任务 ID `01a0806d-e7d2-7092-964c-0edd0919d17e`，host ID `remote-ssh-discovered:dell`。不要默认重读完整聊天。

更新：2026-09-20。DeepSeek 起草，Codex 负责关键设计/复核与实际执行。
