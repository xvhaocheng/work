# 版本、分支与实验源码入口

新 agent 先读根目录[当前状态](../../CURRENT_STATUS.md)与[研究导航](../../研究导航.md)，再从本页选需要的版本。这里只导航源码和已有结果；研究安排、实验与板测暂停状态仍由根入口维护。查旧源码无需切换生产工作树，也不触发构建或重跑。

## 当前开发用哪一版

平台、CVA6、IOMMU、Linux、OpenSBI 的活动分支均为 `codex/organized-current-20261017`。这是保留旧历史、合入功能分层链的**研究源码整合版**，包含研究扩展。实际平台 HEAD 用下节只读命令取得；后续文档或结果归档提交会继续推进它。

2026-10-17 本次核对的四个主要组件固定身份如下；平台父级 gitlink、CVA6 的 IOMMU gitlink 与 [sources.lock.json](../sources.lock.json) 一致。

| 实际仓库路径（相对平台） | 整合提交 |
| --- | --- |
| `cva6` | `74991bd2fa95db129f08b6b440fd4fe5ffcccf78` |
| `cva6/corev_apu/riscv-iommu` | `46ad7450443b99c7de9501d7fcf7d5b8cf5b6f9a` |
| `linux` | `f215540ad19dfe88c91b43b82c09cf40c514f323` |
| `opensbi` | `8d3779fc7fb529f6958f400ee4280d8fa637d019` |

另外 17 项 vendor 依赖直接查版本锁，不在本页复制。各仓库的来源远端、旧分支和功能锚保留；详细合并关系见[内部整合记录](../provenance/internal-repository-integration-20261017/README.md)与[组件提交表](../provenance/internal-repository-integration-20261017/component-integration.json)。本地开发先在实际子仓库提交，再按 IOMMU → CVA6 → 平台更新父级指针与版本锁，最后同步顶层 `work`。

## 原版、适配、修复与研究扩展怎么选

下表是 [source-profiles.json](../provenance/source-organization-20261017/source-profiles.json) 的固定源码组合。表中缩写在该 JSON 中均有完整 SHA；CVA6 各行自己的 IOMMU gitlink 已实际核对，不能任意混配。

| 源码组合 | CVA6 commit → IOMMU child commit | 用途 |
| --- | --- | --- |
| `upstream-original` | `1f992d8` → `f8f918a` | 未改上游及其原始 child 配对；按 commit 查阅，没有为该组合新建同名活动分支 |
| `iommu-upstream-target` | 单独 IOMMU `c80bba7` | 固定 `mc/dti-ats` 目标组件；它不是上一行 CVA6 的原始 child |
| `platform-adapted` | `54c6257` → `7bea3f2` | 必要 DTI/AXI 接口及四 DMA 地址、ID 集成，无研究扩展 |
| `platform-corrected` | `ba8cdd1` → `85b157e` | 适配层加 SC、AW 进度/排序、HPM、DS owner、PTW leaf、AXI 边界修复；含会改变服务先后的修复，须明确披露 |
| `research-source-capture` | `991074e` → `0b79394` | 整理时的研究功能终点，含观测、延迟、banked、P/N/PN；供追溯与导出 |

相应本地 ref 均以 `codex/organize-20261017-` 为前缀：

| 用途 | CVA6 ref 后缀 | IOMMU ref 后缀 |
| --- | --- | --- |
| 单独上游目标 | — | `iommu-upstream` |
| 必要适配 | `cva6-adapted` | `iommu-adapted` |
| 修复基座 | `cva6-baseline-fixed` | `iommu-baseline-fixed` |
| 研究功能终点 | `cva6-research` | `iommu-research-current` |

例如 CVA6 的修复 ref 全名为 `codex/organize-20261017-cva6-baseline-fixed`。这些 ref 用于定位分层链；当前开发仍用上一节的整合分支。IOMMU 整合提交与研究功能终点完整 tree 相同，CVA6 整合提交另将 child 绑定到 IOMMU 整合提交，不能仅因源码相同就把提交身份写成同一个。

上述 registry 是**静态源码导出组合，不是 Makefile 的 `PROFILE`**。`make ... PROFILE=...` 使用的是 `configs/` 中实际构建配置；不能把 `platform-corrected` 直接当作可执行 Make profile。Linux/OpenSBI 另有来源记录，这五组硬件组合没有自动定义整机软件与运行合同。

需要独立源码副本时，按[来源整理与导出说明](../provenance/source-organization-20261017/README.md#复用原导出流程)使用已有 `export_sources.py`，导出到不存在的新目录；它按所选提交自己的 vendor gitlink 递归取源，不借用当前版本锁替换旧依赖。原补丁、历史输入与恢复条件分别见[来源包](../provenance/source-organization-20261017/README.md#保全与恢复)、[历史实验索引](../provenance/source-organization-20261017/history/README.md)和[内部仓库恢复包](../../repository-recovery/internal-integration-20261017/README.md)。

## 最新实验到底用了什么

[IOTLB 失效干扰报告](../out/invalidation-interference-20261017/README.md)使用 `platform-corrected` 的冻结源码：CVA6 `ba8cdd10659a777ccff0080bf020be64b5c1e064`、IOMMU F5 `85b157ebe71a623e326e51fff7b7126fe77e8eb9`。实际输入与参数在 [build03 构建清单](../out/invalidation-interference-20261017/build/build03/build-manifest.json) 的 `baseline_refs`、`sources`、`includes`、`handwritten_inputs` 和 `parameters`；固定源码入口为 [frozen/source-manifest.json](../out/invalidation-interference-20261017/frozen/source-manifest.json)。该实验没有使用当前研究整合 HEAD 作为编译源。

这次是 Verilator 下真实 DMA/IOMMU 的 28 配置仿真；Linux `f215540ad19dfe88c91b43b82c09cf40c514f323` 仅作为来源 C 核查，CPU/Linux 未执行。原版 16 项 PLRU 加已列必要修复、无分区/旁路，不能把结果追认为 `upstream-original` 的运行。来源 registry 的 `source_fixed_only` 是整理时刻状态，后续实际编译/运行以各报告及清单为准。

平台 `734592d18fdeff7e69b01163bb572ab2a0d3f39d` 是该批结果的**固定归档提交**，见[发布回执](../out/invalidation-interference-20261017/publication/receipt.json)；它不充当永远最新的平台 HEAD，也不替代实际 frozen 组件与输入哈希。更早实验从[历史版本身份补填](../provenance/source-organization-20261017/history/README.md#历史版本身份补填)进入，查当时分支、checkout／选用 commit、dirty/overlay、实际构建清单和逐项缺失原因，不用后来整理提交倒填旧运行。失效实验的[28配置身份表](../provenance/source-organization-20261017/history/invalidation-identities.csv)另外列出生产 checkout 与冻结编译源。

## 只读核对与以后怎么记录

从平台根目录执行以下命令，只查看身份，不切换分支；主仓库及每个实际参与的子仓库都需单独记录。`make check` 会写 `out/manifests`，因此不列为只读查阅命令。

```sh
git branch --show-current
git rev-parse HEAD
git --no-optional-locks status --short
git ls-tree HEAD cva6 linux opensbi
git -C cva6 branch --show-current
git -C cva6 rev-parse HEAD
git -C cva6 ls-tree HEAD corev_apu/riscv-iommu
git -C cva6/corev_apu/riscv-iommu rev-parse HEAD
git -C cva6 show ba8cdd10659a777ccff0080bf020be64b5c1e064:.gitmodules
```

分支是可移动标签；固定实验身份需要完整 commit、未提交差异/新增输入，以及**实际编译源码哈希**共同约束。后续实验按[实验身份记录](../../docs/agent-policy.md#实验身份记录)保存平台/组件 branch 与 commit、dirty patch/overlay、父子指针与 lock hash、导出/夹具输入清单、编译参数、运行参数与服务模型，再关联产物和结果。独立冻结副本、当前 checkout、仅引用而未执行的 Linux/固件分别注明；没有使用的组件明确标记未执行。

本页此次更新仅作 Git、链接与已有输入哈希核对，没有新增构建、实验或板测，也不恢复暂停任务。
