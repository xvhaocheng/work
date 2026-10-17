# IOMMU 源码分层与恢复包（2026-10-17）

已建立 **12 个实际新提交**，把上游原样、必要接口适配、已知功能修复、被动观测与研究扩展分开。新链只存在于新增 `codex/organize-20261017-iommu-*` 引用及隔离工作树；原 `platform` 分支、HEAD、索引、未提交源码和旧历史保持原样。没有启动实验、RTL 仿真、编译、综合或板测。

新提交由当前配置身份 `xvhaocheng <xvhaocheng@gmail.com>` 在本次整理时创建；没有伪造旧日期、旧作者或历史实验身份。共享 IOMMU 本次整理为 Codex-only。

## 三个基线与当前研究终点

| 角色 | 固定提交 | 含义 |
| --- | --- | --- |
| 上游原样 U0 | `c80bba7ae74b54b50eb1028e6efd4292661ca75b` | `zero-day-labs/riscv-iommu` 的 `mc/dti-ats`，本地既有历史祖先；本次只读远端查询仍指向同一 SHA，原输出与时间见 [upstream-ls-remote.txt](upstream-ls-remote.txt) |
| 仅必要接口适配 A1 | `7bea3f202903279c18df99a2284d6f56ccdacdc6` | 仅恢复既有 `6cc8ed3` 的 AXI MMU sideband 接口；不带观测或研究逻辑 |
| 累积已知修复 F5 | `85b157ebe71a623e326e51fff7b7126fe77e8eb9` | A1 加五个独立功能修复提交；这是适配和修复后的基座，**不是 stock，也未运行验证** |
| 被动观察 O1 | `5304477888b627546fa5f46ef36735f78f0ee504` | 在 F5 上加只输出观测的生命周期、IOTLB 与 pending-queue 记录；物理移除主动延迟模块和控制输入 |
| 完整研究源码 P2 | `0b7939445298a23ea3e131ffd4e003cebb15ac59` | 与整理前实际工作树的 111 个普通文件、4 个 vendor gitlink 精确对应，包含原先两个未跟踪研究模块 |

终点 Git tree：`5ae79dbf6d2fc4abe511bca4c675a0c01c5a5331`。原实际 IOMMU HEAD 是 `57eb5621802ef9ed8d77c684b9a03205c6630391`，另有 8 个 tracked dirty 文件与 2 个未跟踪研究源；CVA6 原索引 child gitlink `238a39e` 不能代表这份实际源码。完整冻结身份与逐文件哈希见 [frozen-input.json](frozen-input.json)。

## 提交层次

下面是实际 Git 堆叠顺序。相邻行代表补丁应用顺序，不代表无关功能之间存在语义依赖；[manifest.json](manifest.json) 分别给出 `parent` / `stack_prerequisites` 与 `semantic_requires`。没有验证任意挑选或重排后的编译和运行。

| ID | 新提交 | 本层边界与历史来源 |
| --- | --- | --- |
| A1 | `7bea3f2` | 多 DMA sideband 接口；历史 `6cc8ed3` |
| F1 | `09fb4de` | AW-before-AR 进度修复；历史 `7fbb2b9`。**会改变服务先后，可能影响性能**，不能隐去 |
| F2 | `e52346e` | HPM `edged_event` 锁存修复；历史 `da8a801`，不是新增研究计数器 |
| F3 | `dabde0f` | DS 已接受 AW 到 WLAST 的 owner 队列、选择索引和满空门控修复；冻结的 dirty 源码 |
| F4 | `24443a8` | 接受合法 4 KiB/2 MiB 叶子的 PTW 分支修复；冻结的 dirty 源码 |
| F5 | `85b157e` | AXI 4 KiB 边界算术修复，保留已有回归源文件；历史 `57eb562`，本次未重跑 |
| O1 | `5304477` | 历史 `1519510` / `137f5e5` / `238a39e` 的被动投影：保留 queue/taps，PTW AXI 直接导通 |
| E1 | `027583c` | 单独恢复 E3 主动一次性 PTW response delay 和资格控制；明确属于研究干预 |
| B1 | `6a47d91` | 双 bank 控制器、生命周期 tag、W-owner 读准入及 full-buffer 例外；均列为研究组织和调度 |
| N1 | `011e7b5` | 命中旁路、真实 DDTC/IOTLB probe、接受时 touch、响应仲裁；需要 bank 生命周期 |
| P1 | `b35d230` | P 分配 mask、quiet/clear、固定 owner 配置；仍用共享 PLRU 历史，本地代码明确要求 nonblock-capable 16 项配置 |
| P2 | `0b79394` | 固定 2/14 way 分区的独立 replacement history；保留 resident-way touch owner 判定，完成冻结终点 |

所有研究层共同继承 F5 已知修复。B1 叠在 E1 后面是实际补丁链顺序，不能推成“banked 语义上必须开启 E3 注入”。P1 当前实现依赖 N1 的接口和 16 项约束；没有把这种依赖写成不存在。当前源码的 V1/V2 消融位于旧 `out/`，只作历史 overlay 映射，未合入此链。

O1 的 pending queue 只把计数/错误送往 `e3_o`，没有反馈接受、ready、read-admit 或其他调度。对 top、serial controller、IOTLB、PTW、TL 五个核心模块，移除新增观测声明和 tap、恢复直接线名后，与 F5 的功能源码逐字相同（只统一末尾换行）。这项源码检查不证明观测存储、扇出或物理时序免费。O1 也没有被追认为任何旧实验的 stock 结果。

## 恢复与审阅入口

- [manifest.json](manifest.json)：上游、原工作树、新引用、完整提交与 tree、两类依赖、改动文件和证据行。
- [critical-source-evidence.json](critical-source-evidence.json)：每层的提交、路径、精确行号与原文片段。
- [reconstruction-verification.json](reconstruction-verification.json)：实际静态验收回执。
- [series](series) 与 [patches/](patches/)：按顺序应用的 12 份标准补丁。每一步已实际应用到独立临时索引，并与该提交的 tree 比较相同。
- [iommu-new-series.bundle](iommu-new-series.bundle)：新链的增量 Git bundle；前提是已有上游 `c80bba7...`。它恢复**精确新提交 SHA**。补丁重新创建提交时，元数据可能不同，不能承诺相同 SHA。
- [frozen-source.tar.gz](frozen-source.tar.gz)：原 109 个普通 tracked 文件加 2 个未跟踪研究文件的原始内容；4 个 vendor gitlink 的对象号在冻结清单里。
- [original-working.patch](original-working.patch)、[original-staged.patch](original-staged.patch)：原工作树差异与原索引差异，保持历史状态可追溯。

在**另一个恢复仓库**中、拥有固定 upstream prerequisite 后，可导入精确的新研究链：

```sh
git fetch /absolute/path/iommu-new-series.bundle \
  refs/heads/codex/organize-20261017-iommu-research-current:refs/heads/recovered-iommu-research
git rev-parse recovered-iommu-research
git rev-parse 'recovered-iommu-research^{tree}'
```

期望分别是上面的 P2 commit 和终点 tree。Bundle 已实际导入一个新建 bare 恢复仓库，所有 12 个提交及 tree 精确恢复；并非仅执行 `bundle verify`。四个 vendor 子仓库本体不包含在组件 bundle 中，仍需各自既有仓库/历史；本链保存的 gitlink 没有改变。

[organize.py](organize.py) 与 [verify.py](verify.py) 记录本次整理和验收方法，含本机绝对路径、只允许新增引用的断言和首次运行约束；它们不是通用的一键实验或构建入口。不要在原工作树上用恢复操作覆盖未提交状态。供审阅的隔离工作树是 `iommu-platform/out/source-organization-20261017/worktrees/iommu`。

## 本次实际验证边界

已检查：12 步补丁应用/tree、一致的冻结终点文件哈希与模式、两份原未跟踪源完整保留、固定基线物理不含研究模块/控制、O1 功能源码投影、bundle 精确恢复、原 HEAD/旧 refs/索引/状态/源码/remote 未改变。新分支只使用本次新增前缀，未覆盖旧引用。

未运行：任何新编译/elaboration、主机回归、RTL 仿真、性能实验、综合/布线、FPGA/板测。不能把新中间提交称为已经可运行或已验证的原版基线，也不能把新 SHA 回填为历史实验身份。历史数据、旧结论与暂停状态均保留。
