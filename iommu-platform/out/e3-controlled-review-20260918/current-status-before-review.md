# 论文平台当前状态（精简版）

## 2026-09-18 E3 v5 复核与 v6 受控实验（构建交付完成）

v5 服务器独立复核 PASS：9472 项检查、0 失败，26 子轮、494 条唯一观测，输入哈希未变。执行/覆盖达成，因果目标未达成：A8 目标 V 首次 miss 16/16，7/8 对目标身份不同；第 5 对身份/前缀相同、A 剩余 +128、V 剩余 +134，但预热不同且插入截断，仍不是合格因果配对。详见 [结果复核](iommu-platform/docs/e3-queue-results-review.md)。

下一步已实施独立 64 B 受控有限实验：每轮全局 S1 IOTLB 失效、hot/cold A、V prime/probe；先保留两路任务，硬件在 A 读被选中后释放 V，CPU 保留完成轮询。固定 3 短 + 8 正式对，不扩旧矩阵。方案见 [E3_CONTROLLED](iommu-platform/E3_CONTROLLED.md)。

组件已固定：CVA6 2b04936d、Linux 2de370cd344e、IOMMU 137f5e5；顶层硬件/内核锁定提交 57b3bcf。RTL 定向及旧模式回归、IOMMU 清空接口主机测试、相关内核对象交叉编译已通过；新驱动 19 场景故障模型、板端脚本、新旧 CLI 与镜像打包检查均通过。完整软件与 FPGA 构建均退出 0、manifest passed，内核 00087；BIT/MCS 非空，产物哈希、测试源码与硬件导出源码、实际 rootfs 内新程序均已核对。FPGA 于 UTC 06:24:37 完成，成功日志仅保存、未审查时序报告。[交付说明](iommu-platform/out/e3-controlled-20260918/delivery/README.md)，[构建与哈希状态](iommu-platform/out/e3-controlled-20260918/build-status.json)。新 v6 尚未上板；下一步配套更新 BIT 与 SD 软件，只跑 3 短 + 8 正式对，完整回传 JSON/UART 后再判断因果条件。

Codex 负责原始结果分析、架构/计划、DMA 关键修正、最终审查与执行；DeepSeek 提供证据整理及实现/测试/文档草案。用户已长期授权向配置的 DeepSeek 服务发送任务必要项目资料；不含密钥和无关个人信息。

## 2026-09-18 E3 v5 / Linux00086 实板完成并回传

Mac 已完成 3 对短验收 + 10 对正式实验，26 子轮全部通过，26 个 sequence、494 条去重观测，data_errors/quarantine/taint 均为 0。新 BIT 为 migration-4dma-e3-queue，实读 observer magic 0x49444d414f425305，STAT0x501079fc，Linux 6.5.0-rc2-00086-gd2cd32d78918。未补采或重编；printk 已恢复，串口已关闭。

完整证据：/home/xvhaocheng/work/iommu-platform/out/mac-e3-queue-20260918-130545-run-d01lc9wp/，含原始 short/main JSON、验收/摘要、完整 console.raw.log、版本 run.json、host-validation.json 及 E3_QUEUE_RESULTS.md。归档 run/summary 保留上传前状态快照，回传完成以本节及 Mac upload-receipt.json 为准。正式 A8 16/16 触发、A1 0/4；A8 目标 V 首次 miss 16/16、插入记录截断 9/16、实际触发时旧队头为 A 9/16。中位吞吐 A8 109.29 MiB/s、A1 149.51 MiB/s；前史及插入差异仍在，不证明纯 D128 效应或 HOL。所有不可比样本保留，不扩大矩阵。

用户持续授权：以后每轮结果必须默认回传 Dell 并更新交接，不以仅 Mac 本地保存视为交付完成。此授权不扩大硬件改动或重跑范围。DeepSeek 起草辅助脚本/文档，Codex 审查、修正语义并执行测试、上板及回传。

> 本文件用于快速接手当前状态、有效结论、最新交付和下一步。各阶段做了什么及对应证据见 [HANDOFF.md](HANDOFF.md)。

## 平台与构建基线（上板前记录，最新结果见顶部）

- 平台为 CVA6 + 四 iDMA + RISC-V IOMMU + Linux。开发目录 `/home/xvhaocheng/work/iommu-platform`；旧工程和 `platform-archives/` 只作历史/备份，默认不在其中开发。
- 硬件最小身份：Genesys 2、CVA6 50 MHz、timer 25 MHz、4 DMA、IOTLB 16。
- FPGA 是实验主线，仿真按需辅助，不是开始实验的前置门槛。
- 研究多 DMA 共享 IOMMU 的地址翻译干扰；容量干扰、HOL、互连竞争和软件供给影响仍需区分。
- 最新 E3 v5 已实现真实 V 已接收未选中计数、触发门槛/前史/插入记录；主机验证、软件完整构建和 FPGA 完整构建通过，manifest passed，BIT/MCS 非空。v5 已上板并回传，见顶部记录。
- Mac 已完成 v5 BIT 上板；Dell 本机未接板，新 v6 尚未上板。
- 构建成功只证明构建。原时钟定义、CDC 和外部端口约束审查未完成；E0/P2/HPM 仍有未签核边界，历史验证见 [validation.md](iommu-platform/docs/validation.md)。正 WNS/构建通过不等于完整时序、CDC、外部 IO 或机制签核。v5 成功日志仅保存，时序报告尚未审查。
- 历史多轮板上采样按用户要求跳过下载哈希/SD 回读，尚未补做；交付中的期望哈希不能写成 Mac 实测。

## 有效结论

- 既有分段观测将 V 的内部等待增量定位到获得服务前的排队阶段；这说明等待位置，尚不能证明具体阻塞机制。证据见 [E3 结果复核](iommu-platform/docs/e3-results-review.md)。
- 配对因果未成立：已有数据不能证明 D128 已通过共享服务传给 V，也不能宣布 HOL、纯 PTW 代价或缓解方法有效。
- 00085 D0/D0 前史不一致：预热 HPM/首查状态仍不同，旧约 8 ms 轮次差未稳定复现。诊断有扰动，不称修复。
- v5 修补 A 读占 AR 队头时遗漏后续 V 的触发覆盖缺口；不能据此认定全部实验根因已解决。DS50/50 与整批轮询保持。

## E3 v5 有限矩阵（已执行，保留协议口径）

- 配套：本轮 BIT 与 SD `fw_payload.bin` 必须同时更新，不能沿用旧 v4 BIT。新配置名 `migration-4dma-e3-queue`。
- ABI：`--diagnostic 1` → ioctl `0x0a`、D0/D0、schema `idma.e3_diag.v1`；`--diagnostic 2` → ioctl `0x0b`、D0/D128、要求 v5 BIT、schema `idma.e3_queue_pair.v1`。
- 先 3 条短验收，每条两轮；短验收通过后只做 10 对正式：2 对 A8 D0/D0、6 对 A8 D0/D128、2 对 A1 阴性控制。总计 13 对、26 子轮。
- 判定顺序：先执行验收，再看触发覆盖/可比性，最后才比较 D128−D0。先看 D0/D0 是否仍有顺序相关差异；无触发、V 首次 miss、前史/身份不同、插入截断单列。
- 不补采、不自动扩矩阵；真实错误即停并保留失败 JSON/UART。详细参数、字段和自洽检查见 [E3_QUEUE_PAIR.md](iommu-platform/E3_QUEUE_PAIR.md)。
- v5 数据独立成组，不与 v4 旧数据混算。

## 证据与产物入口

- 最新 v6 交付：[交付说明](iommu-platform/out/e3-controlled-20260918/delivery/README.md)、[实验协议](iommu-platform/E3_CONTROLLED.md)、[validation.json](iommu-platform/out/e3-controlled-20260918/validation.json)。新 v6 尚未上板。
- v5 历史交付：[交付说明](iommu-platform/out/e3-followup-20260918/delivery/README.md)、[validation.json](iommu-platform/out/e3-followup-20260918/validation.json)、[build-status.json](iommu-platform/out/e3-followup-20260918/build-status.json)。
- 00085 D0/D0 Mac 原始回传：[短诊断](iommu-platform/out/mac-e3-diag-short-20260918-081411-run-1z6a407q/) 和 [正式诊断](iommu-platform/out/mac-e3-diag-main-20260918-081411-run-1z6a407q/)。
- 旧版 E3 v4 实板原始回传：[短版](iommu-platform/out/mac-e3-short-20260917-222837-run-p0lbvbv7/) 和 [正式版](iommu-platform/out/mac-e3-main-20260917-222837-run-p0lbvbv7/)。
- 更早 E4/路径/批量/DS/E1 原始 JSON、observer、summary 和 UART 保留在 `iommu-platform/out/` 对应日期目录；历史链接和判定见 [validation.md](iommu-platform/docs/validation.md) 及对应 results-review。
- 结果复核：[e3-diag-results-review.md](iommu-platform/docs/e3-diag-results-review.md)、[e3-results-review.md](iommu-platform/docs/e3-results-review.md)、[e4-results-review.md](iommu-platform/docs/e4-results-review.md)、[path-results-review.md](iommu-platform/docs/path-results-review.md)。

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
- 当前 E3 v5 协议：[E3_QUEUE_PAIR.md](iommu-platform/E3_QUEUE_PAIR.md)。
- E3 原协议/字段：[E3_SERVICE.md](iommu-platform/E3_SERVICE.md)；D0/D0 软件诊断字段：[E3_DIAGNOSTIC.md](iommu-platform/E3_DIAGNOSTIC.md)。
- 当前代码地图：[repository-map-2026-09-18.md](iommu-platform/docs/repository-map-2026-09-18.md)。
- 结果复核：[e3-diag-results-review.md](iommu-platform/docs/e3-diag-results-review.md)、[e3-results-review.md](iommu-platform/docs/e3-results-review.md)。
- 验证历史：[validation.md](iommu-platform/docs/validation.md)；补丁来历：[patches.md](iommu-platform/docs/patches.md)。
- 各阶段工作与证据：[HANDOFF.md](HANDOFF.md)；清理前快照：[历史归档](docs/history/HANDOFF-2026-09-18-before-cleanup.md)。

## 历史任务来源

- 需要历史决策原文时定向查源任务：当前标题「平台构建」，曾称「整理论文工作文件夹」；任务 ID `01a0806d-e7d2-7092-964c-0edd0919d17e`，host ID `remote-ssh-discovered:dell`。不要默认重读完整聊天。

更新：2026-09-18。
