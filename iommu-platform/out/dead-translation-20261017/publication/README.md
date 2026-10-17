# Git记录

内部平台只提交本任务的490个文件；父级提交保留并发版本整理。生产CVA6/IOMMU/Linux及版本锁未改。公开work把本目录作为普通文件同步，不写gitlink。

[receipt.json](receipt.json)列出内部提交、父提交、恢复包哈希与依赖链；[platform-results.bundle](platform-results.bundle)仅包含本次平台提交增量。按receipt所列前置包顺序，再导入本包，可以在新bare仓库恢复该结果提交；已实际恢复并核对HEAD/tree及完整对象。

[core-artifacts.json](core-artifacts.json)固定内部提交的任务文件与SHA256；恢复包和本发布记录在内部提交之后生成，由公开work保存，避免包包含自身。构建obj、未压缩重复trace及临时路径清单不发布。
