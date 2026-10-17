# 内部平台 Git 恢复与最终检查

开发主体是 `iommu-platform` 及其子仓库。它们已分别提交并激活 `codex/organized-current-20261017`，父子指针与版本锁一致；公开 `work` 仅保存普通文件和 Git 恢复包。

- [最终静态核验](verification.json)：实际平台提交、22 个仓库干净状态、21 项版本锁、父子指针、旧历史／refs 保全及源码保持检查。调用既有平台 `check()`，没有运行构建或实验，也没有覆盖旧实验 manifest。
- [平台包清单与恢复核验](platform-bundle.json)：本次平台最终 HEAD／tree、全部本地分支和 bundle SHA256。
- [iommu-platform-all-heads.bundle](iommu-platform-all-heads.bundle)：平台自身的完整 Git 历史与全部本地分支，不要求另一个平台 Git prerequisite。
- [四组件的全部分支增量包](../../iommu-platform/provenance/internal-repository-integration-20261017/bundles/README.md)：IOMMU／CVA6／Linux／OpenSBI 共 43 个 head，固定来源 prerequisite 在各包清单中；817 个新增对象已实际解包并重算 Git object ID。
- [内部整合记录](../../iommu-platform/provenance/internal-repository-integration-20261017/README.md)：实际组件提交、平台功能提交、操作前快照和当前使用方式。

平台包放在平台外，避免“平台提交包含自身 bundle，而 bundle 又要包含该提交”的循环。在**另一个新目录**恢复平台本身，例如：

```sh
git clone --branch codex/organized-current-20261017 /absolute/path/iommu-platform-all-heads.bundle /new/path/iommu-platform
```

这个操作恢复平台 Git 和跟踪文件。子组件有独立 Git 对象库：先准备各自清单中固定来源的仓库与完整 prerequisite 历史，使用平台内 `component_bundle_package.py restore` 导入对应包的全部 head，再在干净的新组件目录选择 `codex/organized-current-20261017`。其他 vendor 仍使用锁文件点名的既有版本。详情与防覆盖规则见组件恢复说明；父仓库的 gitlink 不包含子仓库对象。

没有改动第三方 upstream，也没有向其分支推送。本地原始分支、数据和缓存保留；Git 整理不代表原版性能基线或新版本运行验证。无新编译、回归、仿真、性能实验、综合／布线或板测。
