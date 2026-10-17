# 本任务的 Git 记录

测量输出、冻结测试源与说明先提交到内部 `iommu-platform` 的当前分支，再把本目录的普通文件同步到公开 `work/main`。IOMMU、CVA6、Linux、OpenSBI 的生产源码和分支不随本任务改变；实际前后检查见上层 `production-before.json`、`production-after.json`。

内部平台提交完成后才生成 `platform-results.bundle` 与 `receipt.json`，由公开 `work` 保存，避免提交包含自己的恢复包。它们是平台 Git 的新增量，不包含子仓库对象；固定 prerequisite 为平台 `8a2e42d8b3617b638f2033bd16eae6ed2531b88b`。

既有完整平台包位于公开仓库 `repository-recovery/internal-integration-20261017/iommu-platform-all-heads.bundle`。在另一个新目录先由该包恢复平台，再从本次增量包获取 `refs/heads/codex/organized-current-20261017`，可恢复准确的内部结果提交。不要在有未提交工作的目录中覆盖 checkout。

`receipt.json` 记录实际提交、恢复包 SHA256、prerequisite 和在全新 bare 目录中导入两个包、核对 HEAD/tree 与完整对象检查的结果。该临时对象目录是可再生检查产物，不发布。
