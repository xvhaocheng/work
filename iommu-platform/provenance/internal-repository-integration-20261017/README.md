# 内部平台与子仓库的实际整合（2026-10-17）

本轮补齐内部仓库的交付：原版／适配／修复／研究功能分层不仅保存在顶层 `work`，也已落实到实际 IOMMU、CVA6、Linux、OpenSBI 以及 `iommu-platform` 的提交和活动工作区。顶层 `work` 只承担文件同步和恢复包分发，不能替代内部提交。

## 当前怎么用

五个主体仓库使用同名活动分支 `codex/organized-current-20261017`。旧 `platform`、`codex/idma-experiment-v1`、所有既有功能锚和贡献分支保留原指向；vendor 来源版本保持原样。后续直接在当前活动分支工作，组件改动按 IOMMU → CVA6 → 平台的顺序提交并更新 `sources.lock.json`。

| 组件 | 活动提交 | 与上轮功能终点的关系 |
| --- | --- | --- |
| IOMMU | `46ad7450443b99c7de9501d7fcf7d5b8cf5b6f9a` | 与 `0b79394` 完整 tree 相同 |
| CVA6 | `74991bd2fa95db129f08b6b440fd4fe5ffcccf78` | 与 `991074e` 普通源码相同，只另行绑定上行 IOMMU 的整合提交 |
| Linux | `f215540ad19dfe88c91b43b82c09cf40c514f323` | 与 `32b96fe` 完整 tree 相同 |
| OpenSBI | `8d3779fc7fb529f6958f400ee4280d8fa637d019` | 与 `9ed1f82` 完整 tree 相同 |

平台本身的最终身份以 `git rev-parse HEAD` 为准，分组提交表见 [platform-commits.json](platform-commits.json)。组件、21 项版本锁、全部父子 gitlink 和干净状态由 [verify_active.py](verify_active.py) 调用既有平台 `check()` 统一核对。最终实际回执及平台 bundle 在工作区 [恢复目录](../../../repository-recovery/internal-integration-20261017/README.md)，避免平台 bundle 包含自身而产生循环。

原版和修复源码锚仍见 [上一阶段](../source-organization-20261017/README.md)。本次活动分支是研究源码整合版本，不是 stock；没有因提交或合并就建立原版性能基线。

## 如何保留历史并收拢工作区

每个子仓库先在新分支提交原有未提交源文件，得到与已审查功能终点相同的 tree；再用正常 Git merge 合入那条功能链。这样旧历史和功能分层链都成为活动提交的祖先，不必重写旧提交或丢弃源码。保全 snapshot／merge 提交只是历史桥接，不代表新增机制或新实验。CVA6 另外用一笔提交绑定实际 IOMMU 整合 HEAD，避免父级继续指向旧 child。

平台原 822 项状态已逐路径归类：102 个源码／配置／脚本／测试路径分成 8 个功能提交；202 个文档路径分成研究报告、工程导航与历史迁移；513 项上轮来源包另行提交；组件 gitlink 与版本锁原子更新。共享构建／rootfs 脚本放在依赖功能后的统一接入提交，没有宣称每个中间提交都已独立编译或运行。

5 个旧计划删除及 1 个普通文件转 symlink 与对应历史目标一起记录；其中部分归档目标原先已有修订，按实际差异保存，没有伪称全部纯重命名。`docs/validation.md.orig`／`.rej` 作为旧补丁残留原字节保留，不据此假定拒绝的段落已应用。`tight_setup_hold_pins.txt` 是既有时序记录，保存不代表本轮运行时序分析。

只为 Vivado 的 `.Xil/` 会话缓存新增忽略项，文件仍在原处。没有用忽略规则隐藏研究源码；原 `out/` 规则保持，历史数据、构建物和输出均未删除、移动或重新生成。

## 身份、检查与恢复

- [before.json](before.json)、各 `*-before.patch`／`*-before-index.txt`：操作前的 HEAD、分支、引用、索引及差异。
- [component-preflight.json](component-preflight.json)：操作前 22 个仓库和 21 条父子关系的只读身份盘点；其中旧 HEAD/status 是有时间戳的快照。
- [component-integration.json](component-integration.json)：实际 snapshot／merge／活动提交及源码保持检查。
- [platform-inventory.json](platform-inventory.json)、[说明](platform-inventory.md)：原 822 路径的互斥分组、删除／类型变化及保留说明。
- [platform-commits.json](platform-commits.json)：平台实际功能提交顺序；[lock-update.json](lock-update.json) 单列 4 个组件锁值变化，来源仓库未改变。
- [component-bundles.json](component-bundles.json)、[bundles/](bundles/)：四个组件的全部 head、增量 bundle、来源 prerequisite 与实际解包回执。包括旧分支、功能锚和贡献分支。
- [verify_active.py](verify_active.py)：复用现有纯 Git `check()`，另核历史祖先、旧 refs／remotes、源码与保留文件；输出到指定新文件，不覆盖旧实验 manifest。

四组件目前只配置第三方 `upstream`，平台没有远端；这些设置保持。新增内部提交通过恢复包随公开 `work` 同步，没有向第三方上游分支推送。本地直接使用现有内部仓库；另处恢复须先取得每包列明的固定来源 prerequisite，再导入全部 head。增量包不冒充完整 Linux 历史或全部 vendor 仓库镜像。

上轮 `source-organization-20261017/verification.json` 的“旧 checkout 保持”描述属于上一阶段的操作时刻；本轮经用户明确要求激活新分支，使用本目录的新核验，不覆盖旧回执。

本轮仅版本整理和静态 Git／字节检查，无新编译、回归测试、仿真、性能实验、综合、布线或板测；原数据、研究结论、暂停分支和 MIG 状态保持。Codex-only。
