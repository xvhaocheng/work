# 历史实验与源码身份索引（2026-10-17）

本目录把已有历史报告、版本锁、构建清单、dirty/overlay 与实际运行参数串到本轮源码整理的 feature ID。它只整理既有证据，不改变历史题名、结论、日期、数据和接受记录；没有执行新实验、数据归约、构建、综合、布线或板测，也不恢复暂停分支或 MIG。

## 历史版本身份补填

2026-10-17 后续按用户要求补查当时 branch、commit 与实际源码。**以下表是新增的来源说明，原 `index`、build/run manifest、数据和旧核验回执保持原字节。** 查询一轮时先按实验 ID 找 CSV，再到同名 JSON 的字段证据；每处引用提供原文件路径、SHA256 和 JSON pointer／行号。不同表的行数不是相同统计口径，不能相加作为运行总次数。

| 范围 | 可读表格 | 字段证据与源码对应 |
| --- | --- | --- |
| 原索引全部 79 条，包括源码变更、配置、准备失败和实际运行 | [historical-identities.csv](historical-identities.csv) | [historical-identities.json](historical-identities.json)：历史 checkout、具名旧锁、dirty／overlay、13 份核心构建源表及原运行清单 |
| 失效实验 28 个测量配置，另列 9 个 lint／build／smoke 准备项 | [invalidation-identities.csv](invalidation-identities.csv) | [invalidation-identities.json](invalidation-identities.json)：生产 checkout 与冻结选源分开；28 测量逐项绑定 build03，Linux 只读核查另列 |
| 成果导航原索引未单列的 60 个轮次／构建组 | [supplemental-identities.csv](supplemental-identities.csv) | [supplemental-identities.json](supplemental-identities.json)：R107、stage117、A／D／B、mapping、功能测试及 MIG 等；每组给原件及覆盖边界，非逐次运行计数 |

字段按原件能证明的角色填写：`checkout_commit` 是具名时点工作树基座；`selected_commit` 可能是明确选源、版本锁配置、dirty 基座或只读来源，以 `identity_role`／`role` 为准。它们都不能吞掉未提交改动和测试副本。`selected_ref` 未记录时留 `null`，不从当前分支猜测。`null` 旁有具体原因；它不表示干净、detached HEAD 或未运行。后来功能分支与归档提交单列，只用于导航／恢复。

已确认的关键边界：

- 原 79 条中，75 条能找到至少一个具名角色的历史 commit；70 条含 checkout 时点证据，37 条含固定来源／版本锁等选源证据，统计有重叠。历史组件 checkout 分支均未留存；11 条 First-PTE 记录只能关联整轮输入记录中顶层 `work/main`，不能扩展为当时平台或子仓库分支。
- `EARLY-2DMA-BARRIER`、`ACTUAL-DMA-SYSTEM-PN`、`SHARED-READ-RTL`、`PROFILE-baseline-4dma` 未找到可据此确认的历史 commit。各条保留已知表格、源码／配置哈希及各自原证据；源码哈希已知与 commit 未知可以同时成立。
- E3 等 FPGA 构建的开始／结束 commit 不同时分别保留；旧锁只证明配置选择，dirty 和实际加载身份须另查。两份 composition 原构建输入补回了旧索引漏列的平台身份与旧锁。
- First-PTE／DISCOVERY／PLRU 的 13 份核心清单继续关联 1115 个显式输入出现项及保存源字节。失败／准备轮有 6 个缺失原字节出现项（4 种 SHA256）；隐式 include、环境及全部生成输入仍不具备完整闭包，不宣称已复现。
- 失效实验生产分支是 `codex/organized-current-20261017`，但真实编译选用 CVA6 `ba8cdd10659a777ccff0080bf020be64b5c1e064`／IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9` 的冻结源码及手写夹具。114 份冻结文件可核到具名 Git blob。早期 lint01 的旧 top 字节未找到；build04 只有重编记录，未作为测量运行。
- 补充表沿成果索引及其原报告定向整理，不冒称穷尽全部 `out/`、退役树或所有单次运行。D-CS1 原生成缺少完整源哈希，后来增强脚本的哈希不能补成当时生成版本；纯源码、条件模型、功能执行、RTL、FPGA 构建与板测分别标注。

[新增静态核验](identity-verification.json)由 [verify_identities.py](verify_identities.py) 检查引用哈希、JSON pointer、CSV 关联、旧证据保全以及 build03 → 28 case 的关系，不调用编译器、模拟器或原分析器。三个 `backfill_*_identities.py` 只生成这次补填表；不自动修订原实验记录，也不补造原来未记录的信息。新 agent 查阅现成表即可，无需重新收集或运行实验。

旧 inventory 中 `docs/build.md` 已在上一轮版本导航维护中更新；其历史原字节仍在 `metadata/iommu-platform/docs/build.md`，本次未改该文件。新核验分别列出当前路径漂移与历史副本身份，不覆盖旧 `verification.json`，也不把旧回执的“当时一致”冒充所有活动文档永远不变。

## 从哪里查

- [index.json](index.json)：79 条完整记录；[index.csv](index.csv) 是相同记录的表格入口。复杂字段为 JSON 单元格。
- [core-build-sources.json](core-build-sources.json)：13 份 10 月 16–17 日构建清单逐项展开的精确源哈希；[CSV](core-build-sources.csv) 便于比较路径、当时哈希和整理前工作区基座哈希。
- [evidence-files.json](evidence-files.json)：定向选取的原件路径、SHA256 及本目录中的元数据副本路径，数量见 collection summary。`metadata/` 保留选定的小型报告、清单和补丁原字节，未复制构建产物或事件轨迹。
- [historical-lock-recovery.json](historical-lock-recovery.json)：只对 16 份旧 FPGA manifest 点名的平台 commit 读取其 Git 原件 `sources.lock.json`，16/16 与当时 manifest 的锁文件 SHA256 完全一致，原字节在 `git-source-locks/`。当前 10 月工作区版本锁没有被套回 9 月迁移或其他旧实验。
- `source-blobs/`：仅根据上述构建清单逐个取得、且与当时 SHA256 完全一致的显式 SV/C++ 输入。文件名为内容 SHA256；原路径和逻辑路径在核心源表中。相同输入去重保存。
- [collection-summary.json](collection-summary.json) 与 [verification.json](verification.json)：覆盖数和实际完成的静态身份检查；后者不表示旧实验重新通过。

`collect.py` 只读取显式列出的元数据、17 个 profile，以及构建清单点名的源文件并计算哈希。`verify.py` 只核对索引、原件/副本/源 blob 的字节身份和表格关联，均不调用原分析器、编译器或实验入口。重新运行 collection 会重新捕获“核对当时”的工作区哈希，因此本次结果以已保存的 JSON 与回执为准。

## 各阶段覆盖与证据层

| 范围 | 本目录登记什么 | 不能据此认定什么 |
| --- | --- | --- |
| 7 月 DMA/第二 DMA、Pair/Multi 扩展 | `0cb9874`、`3e409bad`、`48185ad` 的带日期源码来源；早期双 DMA Barrier 表格单列 | 软件入口不证明当时四路硬件上板；原命令、dirty 源码与日志缺项仍 unknown。7 月父仓库指针不能重记为 6 月 AW 修复的开发日期 |
| 9 月迁移、E1/E4、batch/DS/path、E3 v4/v5/v6 | 既有报告与板端返回目录、FPGA manifest 的起止平台 commit/dirty、交付版本锁及已知参数 | `platform_dirty=true` 与交付产物 SHA 不等于全源码闭包；E3 早期触发/前史/截断问题不因索引消失 |
| E2、P4M、p5/method88、P/N/PN | 固定角色/profile、旧构建或交付身份、已有接受入口与覆盖边界 | 旧 profile 名、最新 HEAD 或关闭 P/PN 不能追认证为原版。method88 的最终交付采用独立 post-route 优化身份，初始构建不能单独代表最终交付 |
| 实际 DMA 系统、共享读 RTL、M0 完整 COPY | 选定源哈希/patch/执行账本，真实 RTL 与行为 DDR 分层，双方完整工作合同 | 模型时钟步不等于板端周期；局部源哈希不等于所有依赖完整锁定 |
| A2 / A5 | A2 的 Linux dirty 全补丁、untracked/config/compiler 软件锁及未运行边界；A5 的固定 HERO 来源与离线 DAG 检查 | A2 未运行新内核/真实 DMA，Hero 三个 gate 仍 default-reject；A5 变体未应用/编译，完整时间 unknown |
| First-PTE 10-16 | 6 次实际系统运行、2 个仅有源码未跑性能的 policy、3 个失败准备分别登记；保留 M/O overlay 来源 | `base` 共同使用 PN＋DS-first/F4，非 stock；不能把失败准备的旧 top 哈希替成后修订文件 |
| DISCOVERY 10-17 | 8 条实际条件各自 receipt/argv、组件 HEAD 与 tracked dirty、85 SV＋CPP | 输入身份不含完整 untracked 文件闭包；2MiB 诊断与 4KiB 主矩阵分开；无原版实验追认 |
| PLRU ablation 10-17 | 18 条主条件、24 次有效 attempt 的原始命令/构建/变体身份；V0/V1/V2 独立 | V0 是原策略＋被动 fill trace，仍是 P/PN＋DS-first/F4 研究夹具，非 stock。V2 同时包含 LRU/invalid-first/fill-MRU，不是仅改 mask 的消融 |
| baseline-4dma / migration-* | 17 份当前配置逐字节指纹、历史 FPGA manifest 关联及初始失败/后续 recovery 分开 | 配置存在或构建成功不证明该配置当时实际加载运行；描述中的旧“待验收”不是当前裁决 |

核心运行的 First-PTE `build-policy0/1-v3/3`、DISCOVERY `build01`、PLRU `v0/v1/v2-build01` 均为 **85 个显式 SV 输入＋1 个 CPP**，现存原路径在本次静态核对时全部 86/86 与当时清单 SHA256 一致，并已保存源字节。旧失败/干跑准备中有 6 处原路径的字节已被后修订替换；这些项只保留当时哈希，不声称已恢复其原字节。

显式输入一致仍不等于完整可复现认证：manifest 中 include 目录的全部隐式头文件、环境与生成中间输入没有形成已验证的完整闭包。本轮没有重编或重跑，故不声称整理后任何 commit 已复现旧数值。

## 字段与覆盖等级

每行包括 `experiment_id/date/evidence_layer/report/output_dir/input_commits/source_manifests/overlay/config/known_extension_flags/baseline_classification/reconstruction_status/feature_mapping/new_commits/unknowns`。报告、清单、补丁各自绑定原件路径和 SHA256；`build_identity.build_start` 与结束 manifest 的 commit 不相同时分别保留。`input_commits` 的 HEAD、交付锁和 dirty 状态各有来源角色，不能互相替代。

`reconstruction_status` 是来源覆盖描述：早期仅日期/Git 或元数据，之后有选定源/overlay，核心轮次有全部显式构建输入字节；它不把这些等级合并成“已重现”。`current_workspace_base_*` 是本次索引捕获时的逻辑基座文件，不是历史执行身份，也不是之后整理提交的承诺。CPP 的基座统一指向原系统夹具 `tests/nonblock_actual_dma_test.cpp`，其 runner/combined 派生文件另有原路径与哈希。

feature ID 只表示源码家族的可导航关联，**不是与新提交逐字一致、完整启用状态或已在新提交上执行的证明**。IOMMU 的 `A1/F1…/P2` 与 CVA6 的 `CVA6-*` 分属不同组件；A2 的 Linux 专有变更、A5 HERO 资产和历史 M/O/PLRU 变体不能被新 IOMMU/CVA6 生产链吞并。本次已按相邻 `iommu/manifest.json` 与 `cva6/manifest.json` 的正式 feature ID 补上 `new_commits`，同时绑定这两份 manifest 的 SHA256；每项显式写明 `historical_source_equivalence=not_asserted`。新链尚未重跑旧实验，旧实验原件没有改变。

10-17 检查到的上游 `main@18ad30a7d2fd8fc0c0f16b99e39b0fd276437445` 单列为**只读源码检查对象**。它不是这些历史实验的基线，也没有被本轮升级到平台。

原始导航仍是工作区的研究导航、`docs/research-results.md`、历史报告和各轮输出；本索引只补源码身份链。此次为 Codex-only 的静态整理，没有调用 DeepSeek 或 Claude。
