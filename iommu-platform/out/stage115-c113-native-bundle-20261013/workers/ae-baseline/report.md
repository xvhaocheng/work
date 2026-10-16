# stage115：STRONGHOLD 作者 artifact 的 ZeRO-Infinity 基线入口

结论：已实际取得 DOI `10.5281/zenodo.6887214` 的作者原始归档并固定版本；该入口保存的两份 ZeRO-Infinity 运行记录均为 CPU offload，使用 client `torch.optim.adamw.AdamW`。归档另有 NVMe optimizer 启动脚本和配置，但没有与其关联的已保存 NVMe 运行记录。因此该入口不能作为 C113 的 NVMe tiled CPU-Adam 原生运行包，按父席给定规则停止继续检索。**取得作者归档不等于取得目标 NVMe nativebundle；stage115 goal 的目标证据仍未完成。** 无 G0/G1 准入、无科学阴性裁决。

本席只解决该作者入口的原始运行身份与准入边界。StrongHold 方法、其他 baseline、VM/notebook、容器部署及其他系统均未进入工作范围。全部原文、索引、哈希和解释保存在本目录；主文档与 goal 状态由 root 统一处理。

## 1. 来源身份已固定

| 项目 | 实际取得的记录 | 原始入口 |
| --- | --- | --- |
| 作者指定 artifact | DOI 6887214，`v0.1-ae-testing`，发布 2022-07-23 | `raw/zenodo-record-6887214.json:1`，JSON `/metadata/version`、`/metadata/related_identifiers/0`；[公开 API](https://zenodo.org/api/records/6887214) |
| 原 GitHub / 当前重定向 | `strongh2/sc22-ae` → `xshaun/sc22-ae` | `raw/repository.json:1`，JSON `/full_name`；[原仓库入口](https://github.com/strongh2/sc22-ae) |
| DOI 标签 exact commit | `3a98f71380ce172196156151fdaf121226076434`，commit 日期 `2022-07-22T23:44:31Z` | `raw/github-ref-v0.1-ae-testing.json:1` `/object/sha` 与 `raw/github-tag-commit.json:1` `/sha` |
| exact commit tree | `7cac9cf0953e5d07676b507fc6632fe073247374` | `raw/github-tag-commit.json:1` `/tree/sha` |
| 原始 ZIP | 5,129,504 B；SHA256 `f07982a89f179e32f6999d2c5fac15812913e757c9e82b7c0c37c9cb41adac62` | `raw/zenodo-v0.1-ae-testing.zip`，`source-manifest.json` |
| 原档校验 | MD5 `93e9554c7e12039933adebc1c945e7d2`，与 Zenodo `/files/0/checksum` 一致；抽取的 11 个 baseline 文件的 Git blob SHA1 全部匹配固定提交 tree | `derived/archive-github-identity-check.json` |
| 今天的 main，仅记录身份 | `229071a8919a6918323824da93c9011f5fb1bd84`，2025-11-07，README DOI badge 修改 | `raw/github-main-commit.json:1`；未把今日分支当 2022 运行，也未展开该分支的 tree/source/runtime |

论文 artifact 关联 DOI/GitHub 的原文导航为 `derived/paper-layout.txt:1154–1155`、`:1217` 附近；PDF 原件 `raw/paper.pdf`。论文说明默认比较采用 CPU RAM，NVMe 比较单独出现，见 `derived/paper-layout.txt:702–706`。这只是作者对论文实验的描述，不能补成归档中缺少的实际 NVMe 运行身份。[论文原件](https://issl-uk.com/publication/sc22-1/sc22-1.pdf)

## 2. 先查索引，再亲读 baseline 记录

完整 ZIP 目录索引共 1,156 条，含 1,030 个文件；`results/` 有 24 个作者保存的日志，其中只有两份 `zero-infinity` 日志，无 `nvme` 命名的 result 文件，也无 notebook 文件。索引：`derived/archive-file-index.json`、`derived/result-inventory.json`。这是该固定归档的目录事实，不是“作者历史从未运行 NVMe”的结论。

内容读取仅限根 README、基线 dispatcher/launcher/config 和这两份日志（11 文件、149,867 B）。未解压或读取 StrongHold/L2L 等方法源码。日志文件名只用于定位，以下 backend/version/completion 均以日志正文为证。

为缩短导航，下面 L32 / L48 分别指：

- L32：`raw/archive-selected/results/log_zero-infinity_l-32_hs-2048_bs-4_ws-4_2022-06-29.1656472684.txt`。
- L48：`raw/archive-selected/results/log_zero-infinity_l-48_hs-2048_bs-4_ws-4_2022-06-29.1656466598.txt`。

| 核查项 | L32 / L48 原始记录 | 可支持的结论与边界 |
| --- | --- | --- |
| 软件 | 两份 `:165` 报 `version=0.5.7, git-hash=unknown, git-branch=unknown` | 是 0.5.7 保存记录；不是 v0.16.4，runtime exact DeepSpeed source/binary hash 未知 |
| optimizer | 两份 `:174–176` client optimizer / basic `AdamW`，类型 `torch.optim.adamw.AdamW`；`:190` final `AdamW` | CPUAdam 实际 executable 身份未成立，不能把本文记录当 CPU-Adam |
| launcher 与有效配置 | 两份 `:1` / `:50` 都选 `ds_zero_stage_infinity-cpu.json`；`:339–382` 保存 effective input JSON | 两份 JSON 均与固定归档 CPU config 逐字段相等；日志 source 目录为 `DeepSpeedExample-Megatron-LM`，归档目录为 `DeepSpeedExample`，不能据此证明 runtime launcher 的原始 byte identity |
| node / rank | 两份 `:13–16`：`localhost:[0]`，一 node、一 local process、world size 1 | 是进程拓扑；`localhost` 不是物理服务器 UUID，rank 0 不是 PCI requester/domain |
| model | 两份 `:31` batch4，`:65` hidden2048，`:96` heads16，`:97` layers32/48，`:119` seq1024，`:139` world1 | 绑定已保存 CPU run 的软件 shape；`:162` 与末端 `:610` 的 1.717B/2.522B 为显示舍入值，不能当 exact P / N_i / file bytes |
| 实际 offload | 两份 `:309–326`：parameter/optimizer `device=cpu`，`nvme_path=null`，optimizer B4、pin true、pipeline read/write false；`:329` subgroup1e8 | 明确为 CPU offload；不存在该记录的 NVMe optimizer backend 或 tiled NVMe AIO 实际 H |
| 结果和程序完成 | L32 `:608`：50/50、loss9.047380、skipped0/nan0、11,174.4 ms/iteration；L48 `:608`：loss10.51464、skipped0/nan0、16,442.4 ms/iteration；两份 `:611–612` iteration50 后程序退出成功 | 作者保存的 CPU 训练进度、数值和进程 exit；不是独立数值对照，也不是 NVMe/AIO/DMA DONE、quiet、checked release |

已解析记录与对应原文行号见 `derived/saved-run-records.json`，逐字导航见 `critical-quotes.json`。所有 runtime JSON 对比均为本席在原始记录上实际执行的离线解析；没有重跑训练或测试。

## 3. 有 NVMe 配置，但未绑定实际运行

固定 archive 的 `examples/run.sh:40–46` 把 `zero-infinity` 分派到 CPU launcher，另有 `zero-infinity-nvme` 分支；`:56–65` 的 `all` 仅运行 CPU infinity，NVMe 调用为注释。

`DeepSpeedExample/examples/ds_pretrain_gpt2-infinity-nvme.sh:24` 选择 `ds_zero_stage_infinity-nvme.json`。该配置 `:14–29` 为 subgroup1e8、optimizer `device=nvme`、`nvme_path=/tmp/nvme_data`、B4、buffer_size1e9、pipeline read/write false、pin true；parameter `device=cpu`。`:42–47` AIO 模板为 block1,048,576 B、queue16、thread2、single_submit false、overlap_events true。它是原生作者配置模板，不是被保存运行采用的 effective config，也没有 actual H / resident slots / successful bytes / pages。

另一个 `ds_zero_stage_infinity_config.json:14–27` 配置 parameter/optimizer 都为 NVMe、路径 `/nvme_data`，但被检查的 NVMe launcher 不选它，也没有关联保存记录。`DeepSpeedExample/run.sh:81–84` 的命令和 `tee ...zero_infi_nvme...txt` 只是脚本与目标文件名；归档索引无该日志内容，不能当作实际 run。

路径字符串不能证明后端：`/tmp/nvme_data` 与 `/nvme_data` 没有伴随 mount/FS/block/PCI 数据，不能从名字推出 NVMe 盘、物理 requester 或 IOMMU domain。已保存 CPU 日志中的 GPU rank、NCCL、CPU optimizer 时间也不能填补这些身份。

## 4. 同一 C113 的证据缺口与准入判断

| 必须绑定到同一实际 NVMe run 的字段 | 本入口当前证据 |
| --- | --- |
| exact software/launcher/effective config/model/task/node/rank | 只绑定两个 CPU/AdamW/0.5.7 保存记录；NVMe 模板未绑定 runtime，DeepSpeed commit/binary 未知 |
| NVMe path → FS/mount/block/PCI/requester/IOMMU domain/common unit | 未取得；模板字符串不作硬件证据 |
| 自然同单元 peer 的 identity/activity/owner | 未取得；单 rank 不能排除或证明机器上的其他 requester 活动 |
| N_i / B / actual tile / AIO / H / page / layout | 只有软件 shape 与 template 参数；actual NVMe bytes、tiles、H、pages 未取得 |
| 成功字节/数值/DONE/quiet/release/lifetime | CPU 日志有 loss/iteration/成功 exit；NVMe 字节守恒、结果校验、异步完成与 checked release 未取得 |
| ordinary 完整成本与可避免 residual | CPU run 有软件阶段时间；没有同一自然 NVMe 工作流下 tile/pool 驻留、大页、persistent mapping、布局/批量/同 owner 单设备完整对照成本 |

因为实际保存记录的 backend 和 optimizer 已明确不匹配，且 NVMe 模板没有关联 runtime，不接受为 C113 nativebundle。没有根据信息缺失否定自然共享翻译路径或研究主问题；没有制作模型、压力场景、规则或第二 workflow。

如果取得**同一 workflow 的一个既有 NVMe optimizer CPU-Adam 原始运行包**，且以下三组证据能够用 run ID、时间、任务/owner 与文件哈希关联，root 才有新的可接收输入：

1. 已执行的 exact source/binary revision、launcher/arguments/effective JSON、模型/task/node/rank。若版本不同于 v0.16.4，由 root 明确接受差异，不能拼成 v0.16.4 运行。
2. 同次运行的 path/FS/block/NVMe PCI requester/IOMMU domain/common-unit 原始快照，及自然并发 peer 的 identity/activity/owner。
3. 同次运行的实际 tile/buffer/AIO/H/pages/layout、N_i/成功字节、数值结果、完成/quiet/checked release，以及 ordinary 完整成本和具体可避免残余。

这只是新 record 的接口，不授权新部署或继续搜索别的系统。该席已停止；goal 是否继续、需要什么新资料由 root 根据实际目标决定。

## 5. 交付与实际动作

- `source-manifest.json`：exact DOI/tag/archive/tree，网络原始来源和所选文件哈希。
- `critical-quotes.json`：关键配置、实际版本/backend/optimizer/完成原文与精确行号。
- `lookup-ledger.json`：公开源读取与离线解析动作、失败与停止条件；无 VM/notebook 或容器操作。
- `summary.json`：准入结论、已取得与未取得字段、最小新 record 接口。
- `raw/` 与 `derived/`：未改写的原始 API/PDF/ZIP/selected sources，完整文件索引、解析结果与 identity 检查。

本席由 Codex 独立取得、亲核并记录该关键原始运行身份；未使用 DeepSeek、Claude、Astra 或其他子席。没有注册/完成 goal、恢复旧席、源码改动、build/tests、板测或生产配置改动。
