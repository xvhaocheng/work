# 四个活动组件的完整分支恢复包

本包保存四个内部仓库的 **43 个现有本地分支 head**，包括旧平台分支、功能锚、贡献分支与活动 `codex/organized-current-20261017`。只打包固定来源之后的增量，没有复制完整 Linux 历史或 checkout/hash 全内核源码。

| 组件 | 全部 heads | 增量对象 | Bundle 大小 | 排除的固定来源 prerequisite |
| --- | ---: | ---: | ---: | --- |
| IOMMU | 16 | 128 | 43,814 B | `c80bba7ae74b54b50eb1028e6efd4292661ca75b` |
| CVA6 | 9 | 370 | 108,861 B | `1f992d808dc0abe599329dedf73842789f8013b8` |
| Linux | 15 | 305 | 130,224 B | `d371fb0f95d5bae20af8135f976ffd8a06bfaa23` |
| OpenSBI | 3 | 14 | 1,777 B | `48f91ee9c960f048c4a7d1da4447d31e04931e38` |

所有包均低于 49 MiB。各来源是每个相应 head 的祖先，已实际核对；完整 head 名称、commit、tree、bundle SHA256 和来源前提均在 [component-bundles.json](../component-bundles.json)。打包以整合后 [component-integration.json](../component-integration.json) 的活动 HEAD/tree 为准。

原四个仓库保持 clean，HEAD、全部旧/新 refs、索引和 remote 前后一致。没有修改平台提交、版本锁或源码，没有构建、测试、实验、远端更改或推送。本次工作为 Codex-only。

## 实际验证

四个 bundle 均通过 Git 原生 `bundle verify`，并分别实际解包到新 bare 目录。解包时临时只读使用原仓库对象库满足被排除的 prerequisite 和 thin-pack 基对象；恢复所有 43 个 head 名称、commit 和 tree 后移除 alternate。

移除 alternate 后，对 `git rev-list --objects --branches ^base` 列出的 **817 个应打包对象**逐一从新 bare 的本地对象库读出，按原始 Git header 加 payload 重算 object ID，全部相符；同时记录 payload SHA256。核对覆盖所有增量对象，不仅是终点文件或根 tree。临时 bare 目录随后删除；静态回执和逐对象记录保留。

- [验收回执](../component-bundle-verification.json)
- [IOMMU 对象](iommu-object-verification.json)
- [CVA6 对象](cva6-object-verification.json)
- [Linux 对象](linux-object-verification.json)
- [OpenSBI 对象](opensbi-object-verification.json)

完整被排除来源的历史仍是明确恢复前提；上述验证没有声称 bundle 包含旧内核历史或其他 vendor 子仓库。

## 恢复

先在另一个恢复目录准备相应来源仓库，确保拥有表中固定 commit 及其完整历史。再运行：

```sh
python3 provenance/internal-repository-integration-20261017/component_bundle_package.py restore \
  --component linux --target /absolute/path/to/recovery-linux
```

`--component` 也可使用 `iommu`、`cva6` 或 `opensbi`。命令检查 bundle SHA256、来源与现有同名分支，解包对象并恢复全部清单 heads；同名分支已有不同值时拒绝覆盖。它不会 checkout 源码，也不会更改 remote 或推送。

IOMMU 的 `codex/organize-20261017-iommu-upstream` 恰好指向被排除的 base。标准 Git bundle 不重复列出该零增量 head；其名称、commit/tree 在 `prerequisite_only_heads` 中完整保存，上述恢复命令会利用已有来源对象恢复它，因此 16 个 IOMMU heads 一个也未丢。

各包只涵盖所属组件的 Git 历史；CVA6/IOMMU 中的其他子模块仍需要其既有来源仓库。Gitlink 指向的另外三个本次组件可分别使用对应包恢复。没有把子仓库对象伪装为父仓库包的一部分。

[component_bundle_package.py](../component_bundle_package.py) 的 `verify` 模式可重新进行上述静态验证，并要求所给 `--source-root` 的四仓库仍与打包时快照一致。普通验证只输出结果；`--write-receipt` 会更新验收回执。公开恢复使用独立的 `restore` 模式，不要求恢复目标具有原主机的索引字节或旧远端配置。
