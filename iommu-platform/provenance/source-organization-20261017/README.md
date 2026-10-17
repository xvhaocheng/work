# 原版基线、研究扩展与历史源码对应（2026-10-17）

本轮按照用户要求整理源码与既有证据，**保留原历史、工作区和数据，不启动新实验**。原版、必要适配、功能修复分别固定；研究功能拆成实际提交及补丁，历史实验保留自己的版本身份，不把新提交追认为旧运行来源。

## 先选清楚基线

| 源码配置 | CVA6 | IOMMU | 用途与边界 |
| --- | --- | --- | --- |
| `upstream-original` | `1f992d8` | `f8f918a` | 未改动的上游平台及其原始 child 配对 |
| `iommu-upstream-target` | — | `c80bba7` | 单独固定 `mc/dti-ats` 目标组件；不能无适配替换上一行 child |
| `platform-adapted` | `54c6257` | `7bea3f2` | 必要 DTI/AXI 接口、四 DMA 地址/ID 集成，不含研究扩展 |
| `platform-corrected` | `ba8cdd1` | `85b157e` | 在适配层上叠加独立列明的 SC、AW 进度、HPM、DS owner、PTW 与边界修复 |
| `research-source-capture` | `991074e` | `0b79394` | 当前研究源码的精确整理终点，包含观测、延迟、banked、P/N/PN 等功能 |

完整 SHA、父子绑定和来源见 [source-profiles.json](source-profiles.json)。其中原版的 IOMMU child 与目标组件不是同一版本，已明确保留这个差别。`platform-corrected` 包含会改变服务先后的 AW 修复，不能称为 stock 或行为完全不变的适配。

这五项固定硬件源码及其 vendor 依赖；Linux／OpenSBI 在独立清单中保留，没有拼装新的整机启动配置。

**这五项目前都只是固定源码，没有新编译或运行结果。原版性能基线尚未建立。** 后续实验须从相应原版层先建立基线，再逐项说明必要适配、修复和研究扩展。旧 `baseline-4dma` 名称、P/PN 关闭或 mode=0 都不能作为原版证明。

## 功能、依赖与历史

- [IOMMU 分层](iommu/README.md)：12 个实际提交，分开接口、五项修复、被动观测、主动 E3 延迟、双 bank、命中旁路、分区和分区替换历史。
- [CVA6／iDMA 分层](cva6/README.md)：27 个功能提交，分开四 DMA 集成、DS 延迟、观测与存储、供给／gap、控制和 P/PN 接线；逐层绑定相容 child。
- [Linux 分层](linux/README.md)：保留既有软件提交，独立拆分当前未提交修复、CQ／domain 生命周期研究接口、SVA／composition 与可选网卡适配；不把研究内核叫作 stock。
- [OpenSBI 记录](opensbi/README.md)：保留既有 timer/platform 历史，单独固定当前 reset 注册适配。
- [历史实验索引](history/README.md)：79 条记录对应原报告、参数、构建清单、dirty／overlay 和功能族；JSON／CSV 可检索。16 份历史版本锁已从当时 commit 恢复并逐字节核对。

组件 manifest 同时记录 `stack_prerequisites`（补丁应用顺序）和 `semantic_requires`（功能需求）。线性提交排在某功能之后，不代表语义上必须启用该功能。任意挑选、重排功能后的编译和运行尚未验证。

历史索引保留 13 份核心构建清单的 1115 条显式源输入、106 份去重 SV/C++ 原字节和 236 份选定元数据原件。成功核心构建的显式 86 项输入均与记录哈希相符；旧准备／失败输入中缺失的 6 处原字节只保留原哈希，不用新文件冒充。早期 dirty 全量源码、隐式 include 与环境闭包仍有缺口。**功能族关联不等于新旧源码逐字等价，也不等于新提交已复现旧结果。**

## 复用原导出流程

[export_sources.py](export_sources.py) 只复用平台现有 `scripts/platform.py` 的 archive staging，不调用构建、实验或硬件入口。它递归采用所选提交自己的 vendor gitlink，不拿当前锁覆盖旧上游依赖。输出目录必须不存在；不会覆盖现有工作区。

从平台根执行，例如：

```sh
python3 provenance/source-organization-20261017/export_sources.py --list
python3 provenance/source-organization-20261017/export_sources.py --profile platform-corrected --check-only
python3 provenance/source-organization-20261017/export_sources.py --profile upstream-original --output out/source-organization-upstream-copy
```

该 registry 是源码组合表，不是现有 Makefile 的可执行 profile。原 `sources.lock.json`、`configs/`、构建器与实际 DMA 研究夹具保持原样；研究夹具带有 banked 等前提，未来原版实验还需单独校准夹具和运行配置。

## 保全与恢复

所有组件新提交仅写入新增 `codex/organize-20261017-*` 引用；没有重写旧提交、移动旧分支、切换主 checkout、改原索引或向上游仓库推送。顶层公开仓库保存本目录的补丁、增量 Git bundle、源字节和证据；组件 `.git` 与原工作区仍留在本机。Bundle 的 prerequisite 和 child 仓库边界见各组件说明，不能把顶层文件快照当成已经重建内部 Git。

[initial-state.json](initial-state.json) 保存整理前身份；[verification.json](verification.json) 汇总静态核验，[source-export-verification.json](source-export-verification.json) 核对原版与修复基线导出的源码。每个组件另有逐步补丁/tree、源文件身份及 bundle 实际恢复回执。临时工作树、恢复仓库和导出副本在平台 `out/source-organization-20261017/`，不作为历史实验结果。

本轮没有新编译、仿真、回归测试、性能实验、综合、布线或板测；已有实验结果、负结果、暂停分支和 MIG 状态保持。共享 IOMMU 整理为 Codex-only。
